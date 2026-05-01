from z3 import *

slide = [0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0]


def truth(offset, inp1, inp2):
    dat = slide[offset : offset + 8]
    if inp1 and inp2:
        dat[3] = dat[1]
        dat[4] = dat[1]
    elif inp1 and not inp2:
        dat[3] = dat[1]
        dat[4] = dat[0]
    elif not inp1 and inp2:
        dat[3] = dat[2]
        dat[4] = dat[2]
    return dat[4]


for i in range(13):
    print("i=%d:" % i, end=" ")
    for j in range(2):
        for k in range(2):
            print(truth(i, j, k), end=" ")
    print()

truth_lookups = [
    # (a,b): (0,0) (0,1) (1,0) (1,1)
    # 1 1 0 1
    lambda a, b: Or(Not(a), (b)),
    # 0 1 1 1
    lambda a, b: Or(a, b),
    # 1 1 1 1
    lambda a, b: BoolVal(True),
    # 0 0 1 1
    lambda a, b: a,
    # 1 1 1 0
    lambda a, b: Not(And(a, b)),
    # 0 0 0 1
    lambda a, b: And(a, b),
    # 0 1 1 0
    lambda a, b: Xor(a, b),
    # 0 0 0 1
    lambda a, b: And(a, b),
    # 1 0 1 0
    lambda a, b: Not(b),
    # 1 0 0 0
    lambda a, b: Not(Or(a, b)),
    # 0 1 0 0
    lambda a, b: And(Not(a), b),
    # 0 1 0 1
    lambda a, b: b,
    # 1 0 1 1,
    lambda a, b: Or(a, Not(b)),
]

for i in range(13):
    print("i=%d:" % i, end=" ")
    for j in range(2):
        for k in range(2):
            print(
                1
                if str(simplify(truth_lookups[i](BoolVal(j), BoolVal(k)))) == "True"
                else 0,
                end=" ",
            )
    print()

# 1 1 0 1
# 0 1 1 1
# 1 1 1 1
# 0 0 1 1
# 1 1 1 0
# 0 0 0 1
# 0 1 1 0
# 0 0 0 1
# 1 0 1 0
# 1 0 0 0
# 0 1 0 0
# 0 1 0 1
# i=12: 1 0 1 1

DMA3SAD = 0

slide_addr = 0x08008B7C


def op(a, b):
    offset = (DMA3SAD - slide_addr) // 2
    # print(offset, hex(DMA3SAD))
    return truth_lookups[offset](a, b)


dat = []
for i in range(0x80):
    dat.append(Bool("dat_%d" % i))

DMA3SAD = 0x8008B86
uVar1 = op(dat[0x52], dat[0x3F])
uVar2 = op(dat[10], dat[0xE])
uVar3 = op(dat[0x6C], dat[0x7B])
uVar4 = op(dat[0x6F], dat[0x77])
uVar5 = op(dat[0x51], dat[0xF])
uVar6 = op(dat[0x1E], dat[0xB])
uVar7 = op(dat[0x62], dat[0x6F])
uVar8 = op(dat[0x40], dat[0x39])
uVar9 = op(dat[0xB], dat[6])
uVar10 = op(dat[0xF], dat[0x21])
uVar11 = op(dat[0x7F], dat[0x21])
uVar12 = op(dat[0x73], dat[0x22])
uVar13 = op(dat[0x46], dat[0x3D])
uVar14 = op(dat[0x39], dat[0x1C])
uVar15 = op(dat[0x1A], dat[0x48])
uVar16 = op(dat[0x1B], dat[0x61])
DMA3SAD = slide_addr
uVar17 = op(dat[0x42], dat[0x6E])
uVar18 = op(dat[0x5D], dat[0x1E])
uVar19 = op(dat[0x71], dat[0x25])
uVar20 = op(dat[0x3F], dat[0x6D])
uVar21 = op(dat[0x2D], dat[0x43])
uVar22 = op(dat[0x68], dat[0x12])
uVar23 = op(dat[0x75], dat[0x4E])
uVar24 = op(dat[0x2C], dat[0x2D])
uVar25 = op(dat[0x48], dat[0x7A])
uVar26 = op(dat[0x37], dat[3])
uVar27 = op(dat[6], dat[0x4B])
uVar28 = op(dat[0x21], dat[4])
uVar29 = op(dat[0x10], dat[0x50])
uVar30 = op(dat[0x6D], dat[0x50])
uVar31 = op(dat[0x18], dat[0x3A])
uVar32 = op(dat[0x17], dat[0x55])
uVar33 = op(dat[0x20], dat[0x56])
uVar34 = op(dat[0x5C], dat[7])
uVar35 = op(dat[0x32], dat[0x53])
uVar36 = op(dat[0x27], dat[0x3B])
DMA3SAD = 0x8008B84
uVar37 = op(dat[0x36], dat[0x20])
uVar38 = op(dat[0x1D], dat[0x4C])
uVar39 = op(dat[0x65], dat[0xD])
uVar40 = op(dat[0x26], dat[0x2F])
uVar41 = op(dat[4], dat[0x5B])
uVar42 = op(dat[0x50], dat[0x4A])
uVar43 = op(dat[0x3E], dat[0x20])
uVar44 = op(dat[0x4E], dat[0x54])
uVar45 = op(dat[0x58], dat[0x34])
uVar46 = op(dat[0x41], dat[8])
uVar47 = op(dat[0x11], dat[0x77])
uVar48 = op(dat[0x14], dat[0x72])
uVar49 = op(dat[100], dat[0x37])
uVar50 = op(dat[0x78], dat[0x58])
uVar51 = op(dat[3], dat[3])
uVar52 = op(dat[0x52], dat[0x52])
uVar53 = op(dat[0x49], dat[0x49])
uVar54 = op(dat[0x3C], dat[0x3C])
uVar55 = op(dat[0x3F], dat[0x3F])
uVar56 = op(dat[0x6E], dat[0x6E])
uVar57 = op(dat[0x37], dat[0x37])
uVar58 = op(dat[0x1F], dat[0x1F])
uVar59 = op(dat[2], dat[2])
uVar60 = op(dat[0x57], dat[0x57])
uVar61 = op(dat[100], dat[100])
uVar62 = op(dat[0x51], dat[0x51])
uVar63 = op(dat[0x3A], dat[0x3A])
uVar64 = op(dat[0x2E], dat[0x2E])
uVar65 = op(dat[0x5B], dat[0x5B])
uVar66 = op(dat[0xF], dat[0xF])
uVar67 = op(dat[0x19], dat[0x19])
uVar68 = op(dat[0x43], dat[0x43])
uVar69 = op(dat[0x14], dat[0x14])
uVar70 = op(dat[0x6A], dat[0x6A])
uVar71 = op(dat[0x24], dat[0x24])
uVar72 = op(dat[0x2B], dat[0x2B])
uVar73 = op(dat[0x48], dat[0x48])
uVar74 = op(dat[0x4F], dat[0x4F])
uVar75 = op(dat[0x46], dat[0x46])
uVar76 = op(dat[0x36], dat[0x36])
uVar77 = op(dat[0x22], dat[0x22])
uVar78 = op(dat[0x7D], dat[0x7D])
uVar79 = op(dat[0x7A], dat[0x7A])
uVar80 = op(dat[0x45], dat[0x45])
uVar81 = op(dat[0x69], dat[0x69])
uVar82 = op(dat[0x44], dat[0x44])
uVar83 = op(dat[0x54], dat[0x54])
uVar84 = op(dat[0x7E], dat[0x7E])
uVar85 = op(dat[0x2F], dat[0x2F])
uVar86 = op(dat[0x3B], dat[0x3B])
uVar87 = op(dat[99], dat[99])
uVar88 = op(dat[5], dat[5])
uVar89 = op(dat[0x76], dat[0x76])
uVar90 = op(dat[0x32], dat[0x32])
uVar91 = op(dat[0x60], dat[0x60])
uVar92 = op(dat[0x25], dat[0x25])
uVar93 = op(dat[0x68], dat[0x68])
uVar94 = op(dat[0x2C], dat[0x2C])
uVar95 = op(dat[0x30], dat[0x30])
uVar96 = op(dat[0x16], dat[0x16])
uVar97 = op(dat[0x1A], dat[0x1A])
uVar98 = op(dat[0x5C], dat[0x5C])
uVar99 = op(dat[9], dat[9])
uVar100 = op(dat[0x67], dat[0x67])
uVar101 = op(dat[0x47], dat[0x47])
uVar102 = op(dat[0x5A], dat[0x5A])
uVar103 = op(dat[0x1E], dat[0x1E])
uVar104 = op(dat[0x71], dat[0x71])
uVar105 = op(dat[0x73], dat[0x73])
uVar106 = op(dat[0x55], dat[0x55])
uVar107 = op(dat[0x59], dat[0x59])
uVar108 = op(dat[0x31], dat[0x31])
uVar109 = op(dat[0x75], dat[0x75])
uVar110 = op(dat[0x53], dat[0x53])
uVar111 = op(dat[0x50], dat[0x50])
uVar112 = op(dat[0x3D], dat[0x3D])
uVar113 = op(dat[0x77], dat[0x77])
uVar114 = op(dat[0xC], dat[0xC])
uVar115 = op(dat[0x38], dat[0x38])
uVar116 = op(dat[0x20], dat[0x20])
uVar117 = op(dat[0x35], dat[0x35])
uVar118 = op(dat[1], dat[1])
DMA3SAD = 0x8008B8E
uVar119 = op(dat[0x45], dat[0x45])
uVar120 = op(dat[0xE], dat[0x57])
uVar121 = op(dat[0x13], dat[0x45])
uVar122 = op(dat[0x47], dat[0x3C])
uVar123 = op(dat[0], dat[0x1D])
uVar124 = op(dat[0x5A], dat[0x57])
uVar125 = op(dat[0x31], dat[0x73])
uVar126 = op(dat[0x54], dat[0x29])
uVar127 = op(dat[0x44], dat[0x6D])
uVar128 = op(dat[0x6E], dat[0x4B])
uVar129 = op(dat[0x2F], dat[0x53])
uVar130 = op(dat[0x56], dat[0x6A])
uVar131 = op(dat[0x25], dat[0x2A])
uVar132 = op(dat[0x4F], dat[0x5F])
uVar133 = op(dat[0x53], dat[0x21])
uVar134 = op(dat[0x4C], dat[0x6B])
uVar135 = op(dat[9], dat[6])
uVar136 = op(dat[0x2A], dat[0x56])
uVar137 = op(dat[0x35], dat[0x70])
DMA3SAD = 0x8008B7E
uVar138 = op(dat[0x79], dat[0x35])
uVar139 = op(dat[0x24], dat[0x34])
uVar140 = op(dat[0x77], dat[0x78])
uVar141 = op(dat[0x1C], dat[0x1B])
uVar142 = op(dat[0x34], dat[2])
uVar143 = op(dat[0x3C], dat[0x1D])
uVar144 = op(dat[0x4A], dat[0x14])
uVar145 = op(dat[0x4B], dat[0x60])
uVar146 = op(dat[0x5B], dat[0x1B])
uVar147 = op(dat[0x72], dat[0x3A])
uVar148 = op(dat[0x19], dat[0x52])
uVar149 = op(dat[0x43], dat[0x3C])
uVar150 = op(dat[0x16], dat[0x74])
uVar151 = op(dat[0x57], dat[0x7A])
uVar152 = op(dat[0x76], dat[0x2A])
uVar153 = op(dat[0x7B], dat[0x51])
uVar154 = op(dat[0x66], dat[0x11])
uVar155 = op(dat[0x3D], dat[0x59])
uVar156 = op(dat[0x6A], dat[0x3C])
uVar157 = op(dat[0x15], dat[0x2E])
uVar158 = op(dat[1], dat[0x20])
DMA3SAD = 0x8008B94
uVar159 = op(dat[3], dat[0x3B])
uVar160 = op(dat[0x6B], dat[0x4E])
uVar161 = op(dat[99], dat[0x44])
uVar162 = op(dat[0x12], dat[99])
uVar163 = op(dat[0x60], dat[0x7A])
uVar164 = op(dat[0x3A], dat[0x33])
uVar165 = op(dat[0x74], dat[0x6F])
uVar166 = op(dat[0x2E], dat[0x11])
uVar167 = op(dat[0x23], dat[10])
uVar168 = op(dat[0x7E], dat[0x26])
uVar169 = op(dat[0xD], dat[0x74])
uVar170 = op(dat[0x55], dat[4])
uVar171 = op(dat[5], dat[0x3E])
uVar172 = op(dat[0x30], dat[0x43])
uVar173 = op(dat[0x1F], dat[0x17])
uVar174 = op(dat[2], dat[0x66])
uVar175 = op(dat[0x59], dat[0x14])
uVar176 = op(dat[0x70], dat[0xB])
uVar177 = op(dat[0x4D], dat[0x4F])
uVar178 = op(dat[0xC], dat[10])
uVar179 = op(dat[0x38], dat[4])
uVar180 = op(dat[0x7A], dat[0x4D])
DMA3SAD = 0x8008B88
uVar181 = op(dat[0x67], dat[0x20])
uVar182 = op(dat[0x5F], dat[0x46])
uVar183 = op(dat[0x49], dat[0x11])
uVar184 = op(dat[0x33], dat[0x75])
uVar185 = op(dat[8], dat[0xC])
uVar186 = op(dat[0x22], dat[0x4B])
uVar187 = op(dat[0x7C], dat[0x7F])
uVar188 = op(dat[0x7D], dat[2])
uVar189 = op(dat[0x2B], dat[0x2E])
uVar190 = op(dat[0x5E], dat[0x4E])
uVar191 = op(dat[7], dat[0x7B])
uVar192 = op(dat[0x61], dat[0x22])
uVar193 = op(dat[0x29], dat[0xD])
uVar194 = op(dat[0x3B], dat[0x3B])
uVar195 = op(dat[0x28], dat[0x49])
uVar196 = op(dat[0x69], dat[0xE])
DMA3SAD = slide_addr + 5 * 2
uVar119 = op(uVar119, uVar138)
DMA3SAD = slide_addr
uVar119 = op(uVar119, uVar120)
DMA3SAD = slide_addr + 1 * 2
uVar119 = op(uVar119, uVar159)
DMA3SAD = slide_addr + 4 * 2
uVar17 = op(uVar119, uVar17)
DMA3SAD = slide_addr
uVar17 = op(uVar17, uVar181)
uVar17 = op(uVar17, uVar139)
DMA3SAD = slide_addr + 4 * 2
uVar17 = op(uVar17, uVar140)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar17, uVar1)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar2)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar121)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar3)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar160)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar122)
DMA3SAD = slide_addr + 1 * 2
uVar1 = op(uVar1, uVar4)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar5)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar37)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar18)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar161)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar182)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar162)
uVar1 = op(uVar1, uVar141)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar123)
uVar1 = op(uVar1, uVar142)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar163)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar124)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar164)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar38)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar125)
uVar1 = op(uVar1, uVar19)
uVar1 = op(uVar1, uVar183)
uVar1 = op(uVar1, uVar143)
DMA3SAD = slide_addr + 1 * 2
uVar1 = op(uVar1, uVar6)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar126)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar39)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar184)
uVar1 = op(uVar1, uVar20)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar144)
uVar1 = op(uVar1, uVar165)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar145)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar166)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar21)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar185)
DMA3SAD = slide_addr + 1 * 2
uVar1 = op(uVar1, uVar7)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar146)
DMA3SAD = slide_addr + 1 * 2
uVar1 = op(uVar1, uVar40)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar8)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar186)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar187)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar9)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar188)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar41)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar10)
uVar1 = op(uVar1, uVar127)
uVar1 = op(uVar1, uVar22)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar167)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar23)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar168)
uVar1 = op(uVar1, uVar128)
uVar1 = op(uVar1, uVar129)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar24)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar147)
uVar1 = op(uVar1, uVar189)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar130)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar169)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar42)
DMA3SAD = slide_addr + 1 * 2
uVar1 = op(uVar1, uVar190)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar43)
uVar1 = op(uVar1, uVar148)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar191)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar11)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar170)
uVar1 = op(uVar1, uVar131)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar44)
uVar1 = op(uVar1, uVar171)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar12)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar25)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar132)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar26)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar27)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar172)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar149)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar28)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar29)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar192)
uVar1 = op(uVar1, uVar173)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar150)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar174)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar151)
uVar1 = op(uVar1, uVar45)
uVar1 = op(uVar1, uVar175)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar152)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar30)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar31)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar153)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar176)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar154)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar13)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar14)
DMA3SAD = slide_addr + 1 * 2
uVar1 = op(uVar1, uVar46)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar47)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar155)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar177)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar32)
DMA3SAD = slide_addr + 4 * 2
uVar1 = op(uVar1, uVar48)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar49)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar33)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar133)
DMA3SAD = slide_addr + 5 * 2
uVar1 = op(uVar1, uVar156)
DMA3SAD = slide_addr + 1 * 2
uVar1 = op(uVar1, uVar157)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar15)
DMA3SAD = slide_addr + 1 * 2
uVar1 = op(uVar1, uVar178)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar34)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar35)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar179)
DMA3SAD = 0x8008B7E
uVar1 = op(uVar1, uVar193)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar134)
DMA3SAD = slide_addr
uVar1 = op(uVar1, uVar135)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar16)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar136)
DMA3SAD = 0x8008B7E
uVar1 = op(uVar1, uVar194)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar36)
uVar1 = op(uVar1, uVar180)
DMA3SAD = 0x8008B94
uVar1 = op(uVar1, uVar195)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar50)
DMA3SAD = 0x8008B7E
uVar1 = op(uVar1, uVar158)
DMA3SAD = 0x8008B8E
uVar1 = op(uVar1, uVar137)
DMA3SAD = 0x8008B88
uVar1 = op(uVar1, uVar196)
DMA3SAD = 0x8008B86
uVar1 = op(uVar51, uVar1)
uVar1 = op(uVar1, dat[0x42])
uVar1 = op(uVar52, uVar1)
uVar1 = op(uVar1, dat[0x13])
uVar1 = op(uVar1, dat[0x6C])
uVar1 = op(uVar1, dat[0x6B])
uVar1 = op(uVar53, uVar1)
uVar1 = op(uVar54, uVar1)
uVar1 = op(uVar55, uVar1)
uVar1 = op(uVar1, dat[0x74])
uVar1 = op(uVar1, dat[0x4B])
uVar1 = op(uVar1, dat[0xB])
uVar1 = op(uVar56, uVar1)
uVar1 = op(uVar1, dat[0x5E])
uVar1 = op(uVar57, uVar1)
uVar1 = op(uVar58, uVar1)
uVar1 = op(uVar59, uVar1)
uVar1 = op(uVar60, uVar1)
uVar1 = op(uVar1, dat[0x7B])
uVar1 = op(uVar1, dat[0x11])
uVar1 = op(uVar61, uVar1)
uVar1 = op(uVar1, dat[0x15])
uVar1 = op(uVar1, dat[0x28])
uVar1 = op(uVar62, uVar1)
uVar1 = op(uVar63, uVar1)
uVar1 = op(uVar1, dat[0x4A])
uVar1 = op(uVar64, uVar1)
uVar1 = op(uVar1, dat[0x2D])
uVar1 = op(uVar65, uVar1)
uVar1 = op(uVar1, dat[0x40])
uVar1 = op(uVar1, dat[0x7C])
uVar1 = op(uVar66, uVar1)
uVar1 = op(uVar1, dat[0x72])
uVar1 = op(uVar1, dat[0x56])
uVar1 = op(uVar67, uVar1)
uVar1 = op(uVar1, dat[0x7F])
uVar1 = op(uVar68, uVar1)
uVar1 = op(uVar1, dat[0x21])
uVar1 = op(uVar1, dat[0x6D])
uVar1 = op(uVar1, dat[0x18])
uVar1 = op(uVar69, uVar1)
uVar1 = op(uVar70, uVar1)
uVar1 = op(uVar71, uVar1)
uVar1 = op(uVar72, uVar1)
uVar1 = op(uVar1, dat[0x65])
uVar1 = op(uVar1, dat[0x33])
uVar1 = op(uVar1, dat[0xD])
uVar1 = op(uVar1, dat[0x29])
uVar1 = op(uVar73, uVar1)
uVar1 = op(uVar1, dat[0x5F])
uVar1 = op(uVar74, uVar1)
uVar1 = op(uVar75, uVar1)
uVar1 = op(uVar76, uVar1)
uVar1 = op(uVar77, uVar1)
uVar1 = op(uVar78, uVar1)
uVar1 = op(uVar1, dat[0x66])
uVar1 = op(uVar1, dat[0x4D])
uVar1 = op(uVar79, uVar1)
uVar1 = op(uVar1, dat[0x78])
uVar1 = op(uVar80, uVar1)
uVar1 = op(uVar1, dat[0x79])
uVar1 = op(uVar1, dat[0xE])
uVar1 = op(uVar81, uVar1)
uVar1 = op(uVar1, dat[0x5D])
uVar1 = op(uVar82, uVar1)
uVar1 = op(uVar1, dat[0x6F])
uVar1 = op(uVar83, uVar1)
uVar1 = op(uVar1, dat[0x62])
uVar1 = op(uVar84, uVar1)
uVar1 = op(uVar85, uVar1)
uVar1 = op(uVar1, dat[0x3E])
uVar1 = op(uVar1, dat[0x1B])
uVar1 = op(uVar86, uVar1)
uVar1 = op(uVar1, dat[0x27])
uVar1 = op(uVar87, uVar1)
uVar1 = op(uVar1, dat[0x12])
uVar1 = op(uVar1, dat[0])
uVar1 = op(uVar1, dat[4])
uVar1 = op(uVar88, uVar1)
uVar1 = op(uVar1, dat[0x10])
uVar1 = op(uVar89, uVar1)
uVar1 = op(uVar90, uVar1)
uVar1 = op(uVar1, dat[0x2A])
uVar1 = op(uVar91, uVar1)
uVar1 = op(uVar1, dat[7])
uVar1 = op(uVar92, uVar1)
uVar1 = op(uVar93, uVar1)
uVar1 = op(uVar94, uVar1)
uVar1 = op(uVar1, dat[0x4E])
uVar1 = op(uVar95, uVar1)
uVar1 = op(uVar96, uVar1)
uVar1 = op(uVar97, uVar1)
uVar1 = op(uVar98, uVar1)
uVar1 = op(uVar99, uVar1)
uVar1 = op(uVar100, uVar1)
uVar1 = op(uVar101, uVar1)
uVar1 = op(uVar102, uVar1)
uVar1 = op(uVar103, uVar1)
uVar1 = op(uVar104, uVar1)
uVar1 = op(uVar105, uVar1)
uVar1 = op(uVar106, uVar1)
uVar1 = op(uVar107, uVar1)
uVar1 = op(uVar108, uVar1)
uVar1 = op(uVar1, dat[0x34])
uVar1 = op(uVar1, dat[0x61])
uVar1 = op(uVar1, dat[0x58])
uVar1 = op(uVar1, dat[0x1D])
uVar1 = op(uVar1, dat[0x4C])
uVar1 = op(uVar109, uVar1)
uVar1 = op(uVar1, dat[0x23])
uVar1 = op(uVar1, dat[0x26])
uVar1 = op(uVar110, uVar1)
uVar1 = op(uVar111, uVar1)
uVar1 = op(uVar1, dat[0x1C])
uVar1 = op(uVar1, dat[8])
uVar1 = op(uVar1, dat[0x39])
uVar1 = op(uVar1, dat[0x41])
uVar1 = op(uVar112, uVar1)
uVar1 = op(uVar113, uVar1)
uVar1 = op(uVar1, dat[0x17])
uVar1 = op(uVar1, dat[10])
uVar1 = op(uVar114, uVar1)
uVar1 = op(uVar1, dat[6])
uVar1 = op(uVar115, uVar1)
uVar1 = op(uVar116, uVar1)
uVar1 = op(uVar117, uVar1)
iVar197 = op(uVar118, uVar1)

s = Solver()
s.add(iVar197 != False)

if s.check() == sat:
    m = s.model()
    bitvec = ""
    for i in range(0x80):
        bitvec += "FT"[str(m[dat[i]]) == "True"]
    print(bitvec)
    # print(bitvec.split("T"))
    out = "".join(["absSRLUDrl"[len(x)] for x in bitvec.split("T")])
    print(out)
    keymap = {
        "a": "x",
        "b": "z",
        "s": "Back",
        "S": "Enter",
        "R": "Right",
        "L": "Left",
        "U": "Up",
        "D": "Down",
        "r": "s",
        "l": "a",
    }
    print(" ".join([keymap[c] for c in out]))
