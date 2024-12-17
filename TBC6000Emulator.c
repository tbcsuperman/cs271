#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int reg[4];
    int mem[16];
    int pc;
    bool z;
    bool c;
    bool running;
} cpu;

void run(cpu *emulator);

int main() {
    cpu emulator = {{0}, {0}, 0, false, false, true};

    run(&emulator);
    
    printf("%d",emulator.pc);

    return 0;
}

void run(cpu *cpu) {
    int ins, opr, addr, op1, op2;

    while (cpu->running == true) {
        ins = cpu->mem[cpu->pc];
        cpu->pc++;

        opr = (ins & 0xf0) >> 4;
        addr = ins & 0x0f;
        op1 = (ins & 0x0c) >> 2;
        op2 = ins & 0x03;

        switch (opr) {
            case 0x0:
                cpu->reg[op1] = (cpu->reg[op1] + 1) & 0xff;
                break;
            case 0x1:
                cpu->reg[op1] = (cpu->reg[op1] - 1) & 0xff;
                break;
            case 0x2:
                cpu->reg[op1] = (cpu->reg[op1] + cpu->reg[op2]) & 0xff;
                break;
            case 0x3:
                cpu->reg[op1] = (cpu->reg[op1] - cpu->reg[op2]) & 0xff;
                break;
            case 0x4:
                cpu->reg[0] = cpu->mem[addr];
                break;
            case 0x5:
                cpu->reg[1] = cpu->mem[addr];
                break;
            case 0x6:
                cpu->reg[2] = cpu->mem[addr];
                break;
            case 0x7:
                cpu->reg[3] = cpu->mem[addr];
                break;
            case 0x8:
                cpu->mem[addr] = cpu->reg[0];
                break;
            case 0x9:
                cpu->mem[addr] = cpu->reg[1];
                break;
            case 0xA:
                cpu->mem[addr] = cpu->reg[2];
                break;
            case 0xB:
                cpu->mem[addr] = cpu->reg[3];
                break;
            case 0xC:
                cpu->pc = addr;
                break;
            case 0xD:
                if (cpu->z == true) {
                    cpu->pc = addr;
                }
                break;
            case 0xE:
                if (cpu->c == true) {
                    cpu->pc = addr;
                }
                break;
            case 0xF:
                cpu->running = false;
                break;
        }
    }
}