#include <gba.h>
#include <stdio.h>
#include <stdlib.h>

#define REG_DMA0CNT_H (((vu16*) &REG_DMA0CNT)[1])
#define REG_DMA1CNT_H (((vu16*) &REG_DMA1CNT)[1])
#define REG_DMA2CNT_H (((vu16*) &REG_DMA2CNT)[1])
#define REG_DMA3CNT_H (((vu16*) &REG_DMA3CNT)[1])

EWRAM_BSS u8 buf[16];

const u16 ops[] = {
    0, 0x80, 0x80, 0x80, 0x80, 0, 0x80, 0, 0x80, 0,
    0, 0,    0x80, 0x80, 0,    0, 0x80, 0, 0,    0,
};

#define IMPL (ops + 0)
#define OR (ops + 1)
#define NAND (ops + 4)
#define AND (ops + 5)
#define XOR (ops + 6)
#define NOR (ops + 9)
#define RIMPL (ops + 12)
#define SETOP(op) (REG_DMA3SAD = (u32) (op))

#define TEST(op)                                                               \
    SETOP(op);                                                                 \
    iprintf("0 " #op " 0 = %d\n", do_op(0, 0) >> 15);                          \
    iprintf("0 " #op " 1 = %d\n", do_op(0, DMA_ENABLE >> 16) >> 15);           \
    iprintf("1 " #op " 0 = %d\n", do_op(DMA_ENABLE >> 16, 0) >> 15);           \
    iprintf("1 " #op " 1 = %d\n",                                              \
            do_op(DMA_ENABLE >> 16, DMA_ENABLE >> 16) >> 15);

#define INPUTLEN 128
u16 inputs[INPUTLEN];
u32 inputcnt = 0;

IWRAM_CODE ARM_CODE void irq_handler() {
    REG_IF = 0xffff;
    return;
}

void wait_vblank() {
    REG_IE = IRQ_VBLANK;
    Halt();
}

IWRAM_CODE void wait_line0() {
    REG_IE = IRQ_VCOUNT;
    Halt();
}

void setupdma() {
    REG_DMA0SAD = (u32) (buf + 2);
    REG_DMA0DAD = (u32) (buf + 6);
    REG_DMA0CNT = DMA_SRC_DEC | DMA_DST_INC | DMA16 | DMA_HBLANK | 2;
    REG_DMA1SAD = (u32) (buf + 4);
    REG_DMA1DAD = (u32) (buf + 6);
    REG_DMA1CNT = DMA_SRC_INC | DMA_DST_INC | DMA16 | DMA_IMMEDIATE | 2;
    REG_DMA3DAD = (u32) buf;
    REG_DMA3CNT = DMA_SRC_INC | DMA_DST_INC | DMA16 | DMA_IMMEDIATE | 8;
}

__attribute__((noinline)) IWRAM_CODE u16 do_op(u16 a, u16 b) {
    wait_line0();
    asm volatile("mov r0, #172\n"
                 "1:\n"
                 "sub r0, #1\n"
                 "cmp r0, #0\n"
                 "bne 1b\n"
                 :
                 :
                 : "r0");
    REG_DMA3CNT_H |= DMA_ENABLE >> 16;
    REG_DMA0CNT_H |= a;
    REG_DMA1CNT_H |= b;
    while (!(REG_DISPSTAT | LCDC_HBL_FLAG)) {}
    u32 res;
    asm("ldr %0, [%1]" : "=r"(res) : "r"(buf + 11));
    return res;
    return 0;
}

void emucheck() {
    SETOP(AND);
    if (!do_op(DMA_ENABLE >> 16, DMA_ENABLE >> 16)) {
        iprintf("your emulator sucks :(\n");
        asm volatile("b .");
    }
}

bool verifysol() {
#include "ops.inc"
    return res != 0;
}

int main() {
    INT_VECTOR = irq_handler;
    REG_DISPSTAT = VCOUNT(0) | LCDC_VCNT | LCDC_VBL;
    REG_IME = 1;

    consoleDemoInit();

    setupdma();
    emucheck();

    iprintf("Press Button: \n");

    while (1) {
        wait_vblank();
        scanKeys();

        int k = keysDown();
        int ki = -1;
        while (k) {
            if (inputcnt < INPUTLEN) inputs[inputcnt++] = (k & 1) << 15;
            k >>= 1;
            ki++;
        }
        static const char keynames[] = "absSRLUDrl";
        if (ki >= 0) iprintf("%c", keynames[ki]);
        if (inputcnt == INPUTLEN) {
            inputcnt++;
            if (verifysol()) {
                iprintf("\nflag is 'bctf{<above chars>}'\n");
                break;
            }
        }
    }

    asm volatile("b .");
}
