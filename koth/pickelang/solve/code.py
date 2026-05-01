def ret_n1():
    return -1


def ret_0():
    return 0


def ret_1():
    return 1


def ret_arg1(ret_arg1_arg1):
    return ret_arg1_arg1


def ret_arg2(ret_arg2_arg1, ret_arg2_arg2):
    return ret_arg2_arg2


def ret_bin0():
    return 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0


def ret_bin1():
    return 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0


def check_is_0(check_is_0_n):
    check_is_0_dict = {check_is_0_n: 0, 0: 1}
    return check_is_0_dict[check_is_0_n]


def check_eq(check_eq_arg1, check_eq_arg2):
    check_eq_d = {check_eq_arg1: 0, check_eq_arg2: 1}
    return check_eq_d[check_eq_arg1]


def ret_empty_list():
    return []


def ret_empty_tuple():
    return ()


def pow2_impl(pow2_impl_n):
    pow2_impl_v = [pow2_impl, ret_1][check_eq(pow2_impl_n, 0)](pow2_impl_n+-1)
    return pow2_impl_v + pow2_impl_v

def pow2(pow2_n):
    return [pow2_impl, ret_1][check_eq(pow2_n, 0)](pow2_n+-1)


def pow2_str_impl(pow2_str_impl_c, pow2_str_impl_n):
    pow2_str_impl_v = [pow2_str_impl, ret_arg1][check_eq(pow2_str_impl_n, 0)](pow2_str_impl_c, pow2_str_impl_n+-1)
    return pow2_str_impl_v + pow2_str_impl_v

def pow2_str(pow2_str_c, pow2_str_n):
    return [pow2_str_impl, ret_arg1][check_eq(pow2_str_n, 0)](pow2_str_c, pow2_str_n+-1)


def negate16(negate16_v):
    return [15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0][negate16_v]


def get_nth_bit_lookuptable(gnbl_b):
    get_nth_bit_1 = pow2_str(b"\0", gnbl_b) + pow2_str(b"\1", gnbl_b)
    get_nth_bit_2 = negate16(gnbl_b)
    return pow2_str(get_nth_bit_1, get_nth_bit_2)


def binary(get_nth_bit_v, gnb_lookuptables):  # 17 bit
    return (
        gnb_lookuptables[0][get_nth_bit_v],
        gnb_lookuptables[1][get_nth_bit_v],
        gnb_lookuptables[2][get_nth_bit_v],
        gnb_lookuptables[3][get_nth_bit_v],
        gnb_lookuptables[4][get_nth_bit_v],
        gnb_lookuptables[5][get_nth_bit_v],
        gnb_lookuptables[6][get_nth_bit_v],
        gnb_lookuptables[7][get_nth_bit_v],
        gnb_lookuptables[8][get_nth_bit_v],
        gnb_lookuptables[9][get_nth_bit_v],
        gnb_lookuptables[10][get_nth_bit_v],
        gnb_lookuptables[11][get_nth_bit_v],
        gnb_lookuptables[12][get_nth_bit_v],
        gnb_lookuptables[13][get_nth_bit_v],
        gnb_lookuptables[14][get_nth_bit_v],
        gnb_lookuptables[15][get_nth_bit_v],
        0
    )


def findchar(findchar_str, findchar_char, findchar_index):
    findchar_stop = check_eq(findchar_str[findchar_index], findchar_char)
    return [
        findchar,
        ret_n1
    ][findchar_stop](findchar_str, findchar_char, findchar_index+1)+1


def char2num(char2num_c):
    return {'0': 0, '1': 1, '2': 2, '3': 3, '4': 4,
            '5': 5, '6': 6, '7': 7, '8': 8, '9': 9, ',': 0}[char2num_c]


def str2num_x10(str2num_x10_str, str2num_x10_si, str2num_x10_ei):
    str2num_x10_str2num_res = str2num(str2num_x10_str, str2num_x10_si, str2num_x10_ei)
    str2num_x10_str2num_res_2 = str2num_x10_str2num_res+str2num_x10_str2num_res
    str2num_x10_str2num_res_4 = str2num_x10_str2num_res_2+str2num_x10_str2num_res_2
    return str2num_x10_str2num_res_4+str2num_x10_str2num_res_4+str2num_x10_str2num_res_2


def str2num(str2num_str, str2num_si, str2num_ei):
    # [si, ei] range is inclusive
    str2num_1 = [str2num_x10, ret_0][check_eq(str2num_si, str2num_ei)](str2num_str, str2num_si, str2num_ei+-1)
    str2num_2 = char2num(str2num_str[str2num_ei])
    return str2num_1 + str2num_2


def bin_lt_impl(blti_a, blti_b, blti_i):
    blti_choice = [blti_a[blti_i]+blti_b[blti_i]+blti_b[blti_i], 1][check_eq(blti_i, -1)]
    return [bin_lt_impl, ret_0, ret_1][[0, 1, 2, 0][blti_choice]](blti_a, blti_b, blti_i+-1)


def bin_lt(blt_a, blt_b):
    # a < b
    return bin_lt_impl(blt_a, blt_b, 16)


def bin_addmod_fulladd_impl(bamfi_index, bamfi_l, bamfi_r, bamfi_carry):
    bamfi_trippple = bamfi_carry+bamfi_l[bamfi_index]+bamfi_r[bamfi_index]
    bamfi_bit = ([0, 1, 0, 1][bamfi_trippple],)
    bamfi_new_array = [0, 0, 1, 1][bamfi_trippple]
    return bamfi_bit + bin_addmod_fulladd(bamfi_index + 1, bamfi_l, bamfi_r, bamfi_new_array)


def bin_addmod_fulladd(bamf_index, bamf_l, bamf_r, bamf_carry):
    return [bin_addmod_fulladd_impl, ret_empty_tuple][check_eq(bamf_index, 17)](
        bamf_index, bamf_l, bamf_r, bamf_carry
    )


def bin_neg_impl(bni_v, bni_i):
    return ([1, 0][bni_v[bni_i]],) + [bin_neg_impl, ret_empty_tuple][check_eq(bni_i, 16)](bni_v, bni_i+1)


def bin_neg(bn_v):
    bn_1 = bin_neg_impl(bn_v, 0)
    return bin_addmod_fulladd(0, bn_1,
                              (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), 0)


def bin_a_mod_b_impl(bmi_v, bmi_m, bmi_neg_m):
    return [ret_arg1, bin_addmod][[1, 0][bin_lt(bmi_v, bmi_m)]](bmi_v, bmi_neg_m, bmi_m, bmi_neg_m)


def bin_a_mod_b(bm_v, bm_m, bm_neg_m):
    # optimization because we assume bm_m >= 2**15
    return [ret_arg1, bin_a_mod_b_impl, bin_a_mod_b_impl][bm_v[16]+bm_v[15]](bm_v, bm_m, bm_neg_m)


def bin_addmod_impl(bami_l, bami_r, bami_m, bami_neg_m):
    bami_res = bin_addmod_fulladd(0, bami_l, bami_r, 0)
    return bin_a_mod_b(bami_res, bami_m, bami_neg_m)


def bin_addmod(bam_l, bam_r, bam_m, bam_neg_m):
    bam_0 = 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    bam_1 = {bam_0: 0, bam_l: 1}[bam_0]
    bam_2 = {bam_0: 0, bam_r: 1}[bam_0]
    # optimization for 0s
    return [bin_addmod_impl, ret_arg1, ret_arg2, ret_arg2][bam_2+bam_1+bam_1](
            bam_l, bam_r, bam_m, bam_neg_m)


def bin_x2mod(bx2m_v, bx2m_m, bx2m_neg_m):
    bx2m_1 = (
        0,
        bx2m_v[0],
        bx2m_v[1],
        bx2m_v[2],
        bx2m_v[3],
        bx2m_v[4],
        bx2m_v[5],
        bx2m_v[6],
        bx2m_v[7],
        bx2m_v[8],
        bx2m_v[9],
        bx2m_v[10],
        bx2m_v[11],
        bx2m_v[12],
        bx2m_v[13],
        bx2m_v[14],
        bx2m_v[15],
    )
    return bin_a_mod_b(bx2m_1, bx2m_m, bx2m_neg_m)


def bin_mulmod_combine(bmmc_as, bmmc_b, bmmc_m, bmmc_neg_m, bmmc_i):
    return bin_addmod(
        [bin_mulmod_combine, ret_bin0][check_eq(bmmc_i, 16)](bmmc_as, bmmc_b, bmmc_m, bmmc_neg_m, bmmc_i+1),
        [ret_bin0(), bmmc_as[bmmc_i]][bmmc_b[bmmc_i]],
        bmmc_m,
        bmmc_neg_m
    )


def bin_mulmod(bmm_a, bmm_b, bmm_m, bmm_neg_m):
    bmm_ap2s = []
    bmm_ap2s += [bin_a_mod_b(bmm_a, bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [bin_x2mod(bmm_ap2s[-1], bmm_m, bmm_neg_m)]
    bmm_ap2s += [ret_bin0()]
    bmm_1 = bin_mulmod_combine(bmm_ap2s, bmm_b, bmm_m, bmm_neg_m, 0)
    return bmm_1

def bin_powmod_combine(bpmc_as, bpmc_b, bpmc_m, bpmc_neg_m, bpmc_i):
    return bin_mulmod(
        [bin_powmod_combine, ret_bin1][check_eq(bpmc_i, 16)](bpmc_as, bpmc_b, bpmc_m, bpmc_neg_m, bpmc_i + 1),
        [ret_bin1(), bpmc_as[bpmc_i]][bpmc_b[bpmc_i]],
        bpmc_m,
        bpmc_neg_m
    )


def bin_powmod(bpm_a, bpm_b, bpm_m, bpm_neg_m):
    bpm_ap2s = []
    bpm_ap2s += [bin_a_mod_b(bpm_a, bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [bin_mulmod(bpm_ap2s[-1], bpm_ap2s[-1], bpm_m, bpm_neg_m)]
    bpm_ap2s += [ret_bin1()]
    return bin_powmod_combine(bpm_ap2s, bpm_b, bpm_m, bpm_neg_m, 0)


def bin2int(b2i_v, b2i_i):
    return [bin2int, ret_0][check_eq(b2i_i, 16)](b2i_v, b2i_i+1)+[0, pow2(b2i_i)][b2i_v[b2i_i]]


def main():
    input_str = input()
    input_str_inner_length = findchar(input_str, "]", 1)
    first_num_length = findchar(input_str, ",", 1)
    second_num_length = findchar(input_str, ",", first_num_length+3)
    base = str2num(input_str, 1, first_num_length)
    expo = str2num(input_str, 3+first_num_length, 2+first_num_length+second_num_length)
    mod = str2num(input_str, first_num_length+second_num_length+5, input_str_inner_length)
    lookuptables = (
        get_nth_bit_lookuptable(0),
        get_nth_bit_lookuptable(1),
        get_nth_bit_lookuptable(2),
        get_nth_bit_lookuptable(3),
        get_nth_bit_lookuptable(4),
        get_nth_bit_lookuptable(5),
        get_nth_bit_lookuptable(6),
        get_nth_bit_lookuptable(7),
        get_nth_bit_lookuptable(8),
        get_nth_bit_lookuptable(9),
        get_nth_bit_lookuptable(10),
        get_nth_bit_lookuptable(11),
        get_nth_bit_lookuptable(12),
        get_nth_bit_lookuptable(13),
        get_nth_bit_lookuptable(14),
        get_nth_bit_lookuptable(15)
    )
    bin_base = binary(base, lookuptables)
    bin_expo = binary(expo, lookuptables)
    bin_mod = binary(mod, lookuptables)
    bin_neg_mod = bin_neg(bin_mod)
    bin_powmod_result = bin_powmod(bin_base, bin_expo, bin_mod, bin_neg_mod)
    return bin2int(bin_powmod_result, 0)
