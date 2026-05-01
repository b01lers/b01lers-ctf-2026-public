# ida script to dump all ops
func_table_addr = 0xA9020
id_table_addr = 0x5BD40
out_dir = "dumped_c/"

funcs = []

for i in range(0x5B2):
    func_addr = idc.get_qword(func_table_addr + i * 8)
    id = idc.get_wide_dword(id_table_addr + i * 4)
    funcs.append((id, func_addr))

funcs.sort(key=lambda x: x[1])

end = 0x503BE

for i, dat in enumerate(funcs):
    func_end = funcs[i + 1][1] if i + 1 < len(funcs) else end
    id, func_addr = dat
    idc.add_func(func_addr, func_end)
    f = ida_funcs.get_func(func_addr)
    cfunc = ida_hexrays.decompile(f)
    if cfunc is None:
        print("Failed to decompile!")

    sv = cfunc.get_pseudocode()
    with open(out_dir + f"{id}.c", "x+") as f:
        for sline in sv:
            f.write(ida_lines.tag_remove(sline.line) + "\n")
