#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include "hde64.h"

struct clone_args {
    uint64_t flags, pidfd, child_tid, parent_tid, exit_signal;
    uint64_t stack, stack_size, tls, set_tid, set_tid_size, cgroup;
};

union peano { union peano* next; uint64_t cur; };
typedef union peano peano_t;

#define NEXT(var) ((uint64_t)(var = var->next))
#define END (g_minus_one)

register void *g_trap asm("r15");
register uint64_t g_minus_one asm("r14");

static inline long _sc0(long nr) {
    long r;
    asm volatile("syscall" : "=a"(r) : "a"(nr) : "rcx","r11","memory");
    return r;
}
static inline long _sc1(long nr, long a1) {
    long r;
    asm volatile("syscall" : "=a"(r) : "a"(nr), "D"(a1) : "rcx","r11","memory");
    return r;
}
static inline long _sc2(long nr, long a1, long a2) {
    long r;
    asm volatile("syscall" : "=a"(r) : "a"(nr), "D"(a1), "S"(a2) : "rcx","r11","memory");
    return r;
}
static inline long _sc3(long nr, long a1, long a2, long a3) {
    long r;
    asm volatile("syscall" : "=a"(r) : "a"(nr), "D"(a1), "S"(a2), "d"(a3) : "rcx","r11","memory");
    return r;
}
static inline long _sc4(long nr, long a1, long a2, long a3, long a4) {
    long r;
    register long r10 asm("r10") = a4;
    asm volatile("syscall" : "=a"(r) : "a"(nr), "D"(a1), "S"(a2), "d"(a3), "r"(r10) : "rcx","r11","memory");
    return r;
}
static inline long _sc6(long nr, long a1, long a2, long a3, long a4, long a5, long a6) {
    long r;
    register long r10 asm("r10") = a4;
    register long r8  asm("r8")  = a5;
    register long r9  asm("r9")  = a6;
    asm volatile("syscall" : "=a"(r) : "a"(nr), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx","r11","memory");
    return r;
}

#define iputs(msg) _sc3(SYS_write, 1, (long)(msg "\n"), sizeof(msg "\n")-1)
#define iprint(msg) _sc3(SYS_write, 1, (long)(msg), sizeof(msg)-1)
#define ivarprint(msg, len) _sc3(SYS_write, 1, (long)(msg), len)

__attribute__((noinline))
peano_t* pnew(void) {
    peano_t* ret = (peano_t*)malloc(sizeof(peano_t));
    ret->cur = END;
    return ret;
}

// __attribute__((noinline))
// void print(peano_t* p) {
//     int cnt = 0;
//     while (NEXT(p) != END) {cnt++;}
//     printf("Value = %d\n", cnt);
// }

__attribute__((noinline))
void init(peano_t* p, unsigned char val) {
    while (val--) {
        asm volatile(".byte 0x66, 0x45, 0x0f, 0xb6, 0x67, 0x36" : : "D"(p) : "memory");
    }
}

__attribute__((noinline))
void iadd(peano_t* a, peano_t* b) {
    while (NEXT(b) != END) {
        asm volatile(".byte 0x45, 0x0f, 0xb7, 0x44, 0x67, 0x25" : : "D"(a) : "memory");
    }
}

__attribute__((noinline))
void isub(peano_t* a, peano_t* b) {
    while (NEXT(b) != END) {
        asm volatile(".byte 0x66, 0x43, 0x33, 0x6f, 0x27" : : "D"(a) : "memory");
    }
}

__attribute__((noinline))
void imul(peano_t* a, peano_t* b) {
    peano_t result;
    result.cur = END;
    peano_t* a_ptr = a;

    while (a_ptr->cur != END) {
        peano_t* b_ptr = b;
        while (NEXT(b_ptr) != END) {
            asm volatile(".byte 0x43, 0x0f, 0xbf, 0x6f, 0x12" : : "D"(&result) : "memory");
        }
        a_ptr = a_ptr->next;
    }

    while ((uint64_t)a->next != END) {
        asm volatile(".byte 0x66, 0x47, 0x11, 0x6f, 0x37" : : "D"(a) : "memory");
    }

    a->next = result.next;
}

__attribute__((noinline))
peano_t* copy(peano_t* p) {
    peano_t* result = pnew();
    while (NEXT(p) != END) {
        asm volatile(".byte 0x47, 0x0f, 0xb7, 0x47, 0x1a" : : "D"(result) : "memory");
    }
    return result;
}

__attribute__((naked, noreturn, used))
void g_succ_fn(void) {
    asm(
        ".intel_syntax noprefix\n"
        "mov rbx, rdi\n"
        "mov r13, r12\n"
        "and rsp, -16\n"
        "mov edi, 8\n"
        "call malloc\n"
        "mov rcx, [rbx]\n"
        "mov [rax], rcx\n"
        "mov [rbx], rax\n"
        "mov rsp, r13\n"
        "mov eax, 15\n"  /* SYS_rt_sigreturn */
        "syscall\n"
        ".att_syntax\n"
    );
}

__attribute__((naked, noreturn, used))
void g_pred_fn(void) {
    asm(
        ".intel_syntax noprefix\n"
        "mov rax, [rdi]\n"
        "mov rcx, [rax]\n"
        "mov [rdi], rcx\n"
        "mov r13, r12\n"
        "mov rdi, rax\n"
        "and rsp, -16\n"
        "call free\n"
        "mov rsp, r13\n"
        "mov eax, 15\n"  /* SYS_rt_sigreturn */
        "syscall\n"
        ".att_syntax\n"
    );
}

__attribute__((noinline, used))
void g_fake_succ(int fake) {
    if (fake == 676767) {
        iputs("Correct!");
        _sc1(SYS_exit, 0);
        __builtin_unreachable();
    }
}

__attribute__((noinline, used))
void g_fake_pred(int fake) {
    g_minus_one = fake;
}

static void *sret_frame;
static void build_sret_frame(struct user_regs_struct *regs, unsigned int insn_len) {
    memset(sret_frame, 0, 512);
    struct sigcontext *sc = (struct sigcontext *)((char *)sret_frame + 48);

    sc->r8  = regs->r8;    sc->r9  = regs->r9;
    sc->r10 = regs->r10;   sc->r11 = regs->r11;
    sc->r12 = regs->r12;   sc->r13 = regs->r13;
    sc->r14 = regs->r14;   sc->r15 = regs->r15;
    sc->rdi = regs->rdi;   sc->rsi = regs->rsi;
    sc->rbp = regs->rbp;   sc->rbx = regs->rbx;
    sc->rdx = regs->rdx;   sc->rax = regs->rax;
    sc->rcx = regs->rcx;   sc->rsp = regs->rsp;
    sc->rip = regs->rip + insn_len;
    sc->eflags = regs->eflags;
    sc->cs = regs->cs;
    sc->gs = regs->gs;
    sc->fs = regs->fs;
}

__attribute__((naked, noinline))
static void* get_child_peano_fn(const uint8_t* insn/*, unsigned int len*/) {
    asm(
        ".intel_syntax noprefix\n"
        "push rbx\n"
        "push rax\n"
        "xor rbx, rbx\n"
        "xor rbx, 1\n"
        "call $+5\n"
        "xor rbx, 1\n"
        "cmp rbx, 1\n"
        "jz continue_inner\n"
        "movzx eax, byte ptr [rdi+1]\n"
        "shr al, 2\n"
        "and al, 1\n"
        "test al, al\n"
        "push rbx\n"
        "lea rax, g_fake_succ\n"
        "lea rbx, g_fake_pred\n"
        "cmovz rax, rbx\n"
        "pop rbx\n"
        "ret\n"
        "continue_inner:\n"
        "pop rsi\n"
        "movzx eax, byte ptr [rdi]\n"
        "xor edx, edx\n"
        "div esi\n"
        "movzx eax, byte ptr [rdi+rdx]\n"
        "test al, 4\n"
        "lea rax, g_succ_fn\n"
        "lea rbx, g_pred_fn\n"
        "cmovz rax, rbx\n"
        "pop rbx\n"
        "ret\n"
        ".att_syntax\n"
    );
}

__attribute__((noreturn))
static void parent_loop(long cpid) {
    int status;
    struct user_regs_struct regs;
    long word;

    _sc4(SYS_wait4, cpid, (long)&status, 0, 0);
    _sc4(SYS_ptrace, PTRACE_CONT, cpid, 0, 0);

    while (1) {
        _sc4(SYS_wait4, cpid, (long)&status, 0, 0);

        if (WIFEXITED(status)) {
            _sc1(SYS_exit, WEXITSTATUS(status));
            __builtin_unreachable();
        }

        if (WIFSTOPPED(status)) {
            int sig = WSTOPSIG(status);

            if (sig == SIGBUS) {
                _sc4(SYS_ptrace, PTRACE_GETREGS, cpid, 0, (long)&regs);
                _sc4(SYS_ptrace, PTRACE_PEEKTEXT, cpid, regs.rip, (long)&word);

                uint8_t *insn = (uint8_t *)&word;
                hde64s hs;
                unsigned int insn_len = hde64_disasm(insn, &hs);

                void* fn = get_child_peano_fn(insn/*, insn_len*/);
                build_sret_frame(&regs, insn_len);

                regs.rip = (unsigned long)fn;
                regs.r12 = (unsigned long)sret_frame + 8;

                _sc4(SYS_ptrace, PTRACE_SETREGS, cpid, 0, (long)&regs);
                _sc4(SYS_ptrace, PTRACE_CONT, cpid, 0, 0);
            }
            else if (sig == SIGSEGV) {
                iputs("Wrong");
                _sc2(SYS_kill, cpid, SIGKILL);
                _sc1(SYS_exit, 1);
                __builtin_unreachable();
            }
            else {
                _sc4(SYS_ptrace, PTRACE_CONT, cpid, 0, sig);
            }
        }
    }
}

__attribute__((constructor))
void init_new(void) {
    sret_frame = (void *)_sc6(SYS_mmap, 0, 4096,
        PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    long fd = _sc2(SYS_memfd_create, (long)"", 0);
    g_trap = (void *)_sc6(SYS_mmap, 0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    _sc1(SYS_close, fd);

    g_minus_one = -1ULL;
}

extern void func_0(peano_t* p);
extern void func_1(peano_t* p);
extern void func_2(peano_t* p);
extern void func_3(peano_t* p);
extern void func_4(peano_t* p);
extern void func_5(peano_t* p);
extern void func_6(peano_t* p);
extern void func_7(peano_t* p);
extern void func_8(peano_t* p);
extern void func_9(peano_t* p);
extern void func_10(peano_t* p);
extern void func_11(peano_t* p);
extern void func_12(peano_t* p);
extern void func_13(peano_t* p);
extern void func_14(peano_t* p);
extern void func_15(peano_t* p);
extern void func_16(peano_t* p);
extern void func_17(peano_t* p);
extern void func_18(peano_t* p);
extern void func_19(peano_t* p);
extern void func_20(peano_t* p);
extern void func_21(peano_t* p);
extern void func_22(peano_t* p);
extern void func_23(peano_t* p);
extern void func_24(peano_t* p);
extern void func_25(peano_t* p);
extern void func_26(peano_t* p);
extern void func_27(peano_t* p);
extern void func_28(peano_t* p);
extern void func_29(peano_t* p);
extern void func_30(peano_t* p);
extern void func_31(peano_t* p);
extern void func_32(peano_t* p);
extern void func_33(peano_t* p);
extern void func_34(peano_t* p);
extern void func_35(peano_t* p);
extern void func_36(peano_t* p);
extern void func_37(peano_t* p);
extern void func_38(peano_t* p);
extern void func_39(peano_t* p);
extern void func_40(peano_t* p);
extern void func_41(peano_t* p);
extern void func_42(peano_t* p);
extern void func_43(peano_t* p);
extern void func_44(peano_t* p);
extern void func_45(peano_t* p);
extern void func_46(peano_t* p);
extern void func_47(peano_t* p);
extern void func_48(peano_t* p);
extern void func_49(peano_t* p);
extern void func_50(peano_t* p);
extern void func_51(peano_t* p);
extern void func_52(peano_t* p);
extern void func_53(peano_t* p);
extern void func_54(peano_t* p);
extern void func_55(peano_t* p);
extern void func_56(peano_t* p);
extern void func_57(peano_t* p);
extern void func_58(peano_t* p);
extern void func_59(peano_t* p);
extern void func_60(peano_t* p);
extern void func_61(peano_t* p);
extern void func_62(peano_t* p);
extern void func_63(peano_t* p);
extern void func_64(peano_t* p);
extern void func_65(peano_t* p);
extern void func_66(peano_t* p);
extern void func_67(peano_t* p);
extern void func_68(peano_t* p);
extern void func_69(peano_t* p);
extern void func_70(peano_t* p);
extern void func_71(peano_t* p);
extern void func_72(peano_t* p);
extern void func_73(peano_t* p);
extern void func_74(peano_t* p);
extern void func_75(peano_t* p);
extern void func_76(peano_t* p);
extern void func_77(peano_t* p);
extern void func_78(peano_t* p);
extern void func_79(peano_t* p);
extern void func_80(peano_t* p);
extern void func_81(peano_t* p);
extern void func_82(peano_t* p);
extern void func_83(peano_t* p);
extern void func_84(peano_t* p);
extern void func_85(peano_t* p);
extern void func_86(peano_t* p);
extern void func_87(peano_t* p);
extern void func_88(peano_t* p);
extern void func_89(peano_t* p);
extern void func_90(peano_t* p);
extern void func_91(peano_t* p);
extern void func_92(peano_t* p);
extern void func_93(peano_t* p);
extern void func_94(peano_t* p);
extern void func_95(peano_t* p);
extern void func_96(peano_t* p);
extern void func_97(peano_t* p);
extern void func_98(peano_t* p);
extern void func_99(peano_t* p);
extern void func_100(peano_t* p);
extern void func_101(peano_t* p);
extern void func_102(peano_t* p);
extern void func_103(peano_t* p);
extern void func_104(peano_t* p);
extern void func_105(peano_t* p);
extern void func_106(peano_t* p);
extern void func_107(peano_t* p);
extern void func_108(peano_t* p);
extern void func_109(peano_t* p);
extern void func_110(peano_t* p);
extern void func_111(peano_t* p);
extern void func_112(peano_t* p);
extern void func_113(peano_t* p);
extern void func_114(peano_t* p);
extern void func_115(peano_t* p);
extern void func_116(peano_t* p);
extern void func_117(peano_t* p);
extern void func_118(peano_t* p);
extern void func_119(peano_t* p);
extern void func_120(peano_t* p);
extern void func_121(peano_t* p);
extern void func_122(peano_t* p);
extern void func_123(peano_t* p);
extern void func_124(peano_t* p);
extern void func_125(peano_t* p);
extern void func_126(peano_t* p);
extern void func_127(peano_t* p);
extern void func_128(peano_t* p);
extern void func_129(peano_t* p);

int main(int argc, char** argv) {
    if (argc != 2) {
        iprint("Usage: ");
        ivarprint(argv[0], strlen(argv[0]));
        iprint(" <flag>\n");
        return 1;
    }

    unsigned char* flag = (unsigned char*)argv[1];
    if (strlen((char*)flag) != 36) {
        iputs("Wrong");
        return 1;
    }

    struct clone_args cargs;
    memset(&cargs, 0, sizeof(cargs));
    cargs.exit_signal = SIGCHLD;
    long pid = _sc2(SYS_clone3, (long)&cargs, sizeof(cargs));

    if (pid != 0) {
        parent_loop(pid);
        __builtin_unreachable();
    }

    if (_sc4(SYS_ptrace, PTRACE_TRACEME, 0, 0, 0) != 0) {
        iputs("Wrong");
        return 1;
    }
    _sc2(SYS_kill, _sc0(SYS_getpid), SIGSTOP);

    peano_t* v0 = pnew();
    peano_t* v1 = pnew();
    peano_t* v2 = pnew();
    peano_t* v3 = pnew();
    peano_t* v4 = pnew();
    peano_t* v5 = pnew();
    peano_t* v6 = pnew();
    peano_t* v7 = pnew();
    peano_t* v8 = pnew();
    peano_t* v9 = pnew();
    peano_t* v10 = pnew();
    peano_t* v11 = pnew();
    peano_t* v12 = pnew();
    peano_t* v13 = pnew();
    peano_t* v14 = pnew();
    peano_t* v15 = pnew();
    peano_t* v16 = pnew();
    peano_t* v17 = pnew();
    peano_t* v18 = pnew();
    peano_t* v19 = pnew();
    peano_t* v20 = pnew();
    peano_t* v21 = pnew();
    peano_t* v22 = pnew();
    peano_t* v23 = pnew();
    peano_t* v24 = pnew();
    peano_t* v25 = pnew();
    peano_t* v26 = pnew();
    peano_t* v27 = pnew();
    peano_t* v28 = pnew();
    peano_t* v29 = pnew();
    peano_t* v30 = pnew();
    peano_t* v31 = pnew();
    peano_t* v32 = pnew();
    peano_t* v33 = pnew();
    peano_t* v34 = pnew();
    peano_t* v35 = pnew();

    init(v0, *flag++);
    init(v1, *flag++);
    init(v2, *flag++);
    init(v3, *flag++);
    init(v4, *flag++);
    init(v5, *flag++);
    init(v6, *flag++);
    init(v7, *flag++);
    init(v8, *flag++);
    init(v9, *flag++);
    init(v10, *flag++);
    init(v11, *flag++);
    init(v12, *flag++);
    init(v13, *flag++);
    init(v14, *flag++);
    init(v15, *flag++);
    init(v16, *flag++);
    init(v17, *flag++);
    init(v18, *flag++);
    init(v19, *flag++);
    init(v20, *flag++);
    init(v21, *flag++);
    init(v22, *flag++);
    init(v23, *flag++);
    init(v24, *flag++);
    init(v25, *flag++);
    init(v26, *flag++);
    init(v27, *flag++);
    init(v28, *flag++);
    init(v29, *flag++);
    init(v30, *flag++);
    init(v31, *flag++);
    init(v32, *flag++);
    init(v33, *flag++);
    init(v34, *flag++);
    init(v35, *flag++);
    isub(v26, v13);
    func_0(v30);
    isub(v28, v20);
    iadd(v29, v28);
    isub(v14, v17);
    iadd(v16, v8);
    iadd(v26, v24);
    isub(v8, v20);
    func_1(v22);
    func_2(v3);
    iadd(v14, v28);
    func_3(v2);
    iadd(v10, v29);
    isub(v17, v8);
    isub(v6, v17);
    func_4(v4);
    isub(v24, v28);
    func_5(v6);
    isub(v29, v11);
    func_6(v35);
    func_7(v1);
    func_8(v7);
    func_9(v6);
    func_10(v5);
    func_11(v16);
    iadd(v7, v8);
    iadd(v34, v21);
    isub(v33, v20);
    isub(v26, v24);
    iadd(v22, v18);
    isub(v8, v28);
    isub(v30, v2);
    isub(v33, v11);
    iadd(v9, v31);
    func_12(v21);
    isub(v7, v15);
    func_13(v18);
    func_14(v24);
    iadd(v15, v9);
    iadd(v8, v15);
    iadd(v22, v32);
    func_15(v19);
    isub(v19, v13);
    func_16(v2);
    func_17(v15);
    isub(v35, v0);
    isub(v2, v13);
    isub(v28, v11);
    func_18(v24);
    iadd(v3, v12);
    func_19(v29);
    isub(v21, v30);
    func_20(v17);
    isub(v17, v13);
    iadd(v26, v8);
    func_21(v27);
    isub(v28, v33);
    func_22(v29);
    iadd(v30, v7);
    func_23(v27);
    isub(v5, v21);
    func_24(v27);
    isub(v20, v28);
    iadd(v23, v20);
    isub(v2, v25);
    func_25(v14);
    func_26(v1);
    iadd(v2, v26);
    isub(v29, v15);
    func_27(v7);
    isub(v13, v33);
    func_28(v16);
    iadd(v1, v29);
    func_29(v24);
    func_30(v31);
    isub(v26, v12);
    isub(v0, v28);
    iadd(v17, v1);
    isub(v31, v26);
    iadd(v29, v4);
    isub(v16, v8);
    isub(v21, v32);
    iadd(v19, v10);
    iadd(v18, v34);
    isub(v30, v31);
    func_31(v4);
    iadd(v20, v12);
    func_32(v9);
    iadd(v25, v14);
    func_33(v30);
    isub(v8, v28);
    iadd(v22, v8);
    isub(v24, v21);
    iadd(v24, v25);
    func_34(v6);
    func_35(v16);
    iadd(v34, v12);
    iadd(v29, v7);
    iadd(v9, v2);
    iadd(v23, v34);
    func_36(v31);
    iadd(v7, v9);
    func_37(v10);
    func_38(v3);
    isub(v5, v0);
    func_39(v1);
    isub(v10, v21);
    isub(v10, v33);
    func_40(v16);
    func_41(v7);
    func_42(v20);
    iadd(v7, v12);
    iadd(v29, v12);
    isub(v13, v10);
    func_43(v24);
    isub(v9, v28);
    isub(v17, v8);
    func_44(v35);
    isub(v30, v21);
    func_45(v31);
    isub(v3, v27);
    isub(v5, v32);
    func_46(v23);
    func_47(v19);
    func_48(v8);
    iadd(v32, v5);
    iadd(v16, v24);
    isub(v32, v8);
    iadd(v0, v24);
    isub(v11, v13);
    iadd(v35, v26);
    iadd(v16, v11);
    iadd(v18, v11);
    iadd(v3, v15);
    iadd(v17, v10);
    iadd(v7, v4);
    isub(v17, v15);
    iadd(v34, v14);
    iadd(v11, v5);
    func_49(v25);
    iadd(v0, v15);
    func_50(v32);
    func_51(v30);
    func_52(v26);
    iadd(v14, v10);
    isub(v23, v7);
    isub(v27, v18);
    func_53(v7);
    isub(v19, v15);
    func_54(v32);
    iadd(v35, v7);
    func_55(v9);
    func_56(v20);
    isub(v32, v23);
    func_57(v12);
    func_58(v16);
    isub(v6, v8);
    iadd(v31, v7);
    func_59(v18);
    func_60(v15);
    func_61(v32);
    isub(v5, v16);
    isub(v3, v9);
    iadd(v33, v15);
    iadd(v22, v20);
    isub(v16, v2);
    iadd(v32, v23);
    isub(v26, v13);
    isub(v13, v28);
    func_62(v2);
    func_63(v16);
    iadd(v30, v5);
    iadd(v35, v20);
    iadd(v4, v2);
    iadd(v8, v20);
    iadd(v7, v33);
    iadd(v9, v22);
    isub(v29, v20);
    func_64(v13);
    func_65(v16);
    isub(v9, v30);
    isub(v20, v3);
    iadd(v17, v8);
    isub(v22, v32);
    iadd(v0, v8);
    func_66(v22);
    isub(v31, v26);
    iadd(v9, v0);
    iadd(v33, v1);
    func_67(v26);
    isub(v34, v24);
    iadd(v12, v34);
    func_68(v32);
    func_69(v1);
    isub(v14, v24);
    func_70(v28);
    func_71(v7);
    iadd(v33, v19);
    func_72(v29);
    iadd(v31, v22);
    isub(v2, v18);
    iadd(v33, v6);
    isub(v11, v31);
    iadd(v12, v31);
    func_73(v30);
    isub(v21, v20);
    func_74(v23);
    iadd(v13, v19);
    iadd(v23, v11);
    isub(v1, v23);
    func_75(v30);
    func_76(v19);
    iadd(v0, v20);
    func_77(v2);
    func_78(v27);
    isub(v6, v10);
    iadd(v34, v28);
    func_79(v6);
    iadd(v35, v15);
    func_80(v8);
    isub(v20, v21);
    func_81(v21);
    iadd(v35, v28);
    iadd(v22, v3);
    iadd(v13, v28);
    iadd(v9, v13);
    isub(v2, v28);
    iadd(v5, v6);
    func_82(v25);
    isub(v6, v3);
    isub(v9, v10);
    func_83(v10);
    func_84(v2);
    iadd(v20, v12);
    func_85(v7);
    isub(v18, v19);
    isub(v20, v21);
    iadd(v24, v3);
    iadd(v19, v7);
    iadd(v7, v3);
    isub(v12, v25);
    func_86(v12);
    func_87(v17);
    isub(v5, v14);
    iadd(v3, v24);
    isub(v3, v30);
    func_88(v32);
    isub(v12, v18);
    isub(v7, v3);
    func_89(v6);
    isub(v11, v8);
    func_90(v0);
    func_91(v17);
    func_92(v19);
    iadd(v31, v21);
    isub(v11, v6);
    iadd(v3, v11);
    isub(v22, v6);
    isub(v33, v10);
    func_93(v2);
    iadd(v21, v35);
    func_94(v0);
    func_95(v1);
    func_96(v2);
    func_97(v3);
    func_98(v4);
    func_99(v5);
    func_100(v6);
    func_101(v7);
    func_102(v8);
    func_103(v9);
    func_104(v10);
    func_105(v11);
    func_106(v12);
    func_107(v13);
    func_108(v14);
    func_109(v15);
    func_110(v16);
    func_111(v17);
    func_112(v18);
    func_113(v19);
    func_114(v20);
    func_115(v21);
    func_116(v22);
    func_117(v23);
    func_118(v24);
    func_119(v25);
    func_120(v26);
    func_121(v27);
    func_122(v28);
    func_123(v29);
    func_124(v30);
    func_125(v31);
    func_126(v32);
    func_127(v33);
    func_128(v34);
    func_129(v35);

    iputs("Correct!");
}
