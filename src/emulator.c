#include "../include/emulator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void cpu_init(CPU *cpu) {
    memset(cpu->gpr, 0, sizeof(cpu->gpr));
    cpu->status  = 0;
    cpu->handler = 0;
    cpu->cause   = 0;
    cpu->halted  = 0;

    cpu->ram = (uint8_t*)calloc(1, RAM_SIZE);

    if (!cpu->ram) {
        fprintf(stderr, "Failed to allocate RAM\n");
        exit(1);
    }

    cpu_reset(cpu);
}

void cpu_destroy(CPU *cpu) {
    free(cpu->ram);
    cpu->ram = NULL;
}

void cpu_reset(CPU *cpu) {
    /* r0 stays hardwired to 0; pc goes to the reset vector */
    cpu->gpr[PC_REG] = RESET_VECTOR;
    cpu->gpr[SP_REG] = RAM_END;
}

/* -------------------------
   Memory access
   ------------------------- */
static int ram_contains(uint32_t addr)
{
    return addr >= RAM_BASE &&
           addr < RAM_BASE + RAM_SIZE;
}
static uint32_t ram_offset(uint32_t addr)
{
    return addr - RAM_BASE;
}
uint8_t mem_read8(CPU *cpu, uint32_t addr)
{
    if (ram_contains(addr))
        return cpu->ram[ram_offset(addr)];

    fprintf(stderr, "Invalid memory read: 0x%08X\n", addr);
    return 0;
}

uint32_t mem_read32(CPU *cpu, uint32_t addr)
{
    /* little-endian */
    uint32_t value = 0;

    value |= (uint32_t)mem_read8(cpu, addr);
    value |= (uint32_t)mem_read8(cpu, addr + 1) << 8;
    value |= (uint32_t)mem_read8(cpu, addr + 2) << 16;
    value |= (uint32_t)mem_read8(cpu, addr + 3) << 24;

    return value;
}

void mem_write8(CPU *cpu, uint32_t addr, uint8_t value)
{
    if (ram_contains(addr)) {
        cpu->ram[ram_offset(addr)] = value;
        return;
    }

    fprintf(stderr, "Invalid memory write: 0x%08X\n", addr);
}

void mem_write32(CPU *cpu, uint32_t addr, uint32_t value)
{
    /* little-endian */
    mem_write8(cpu, addr,     value & 0xFF);
    mem_write8(cpu, addr + 1, (value >> 8) & 0xFF);
    mem_write8(cpu, addr + 2, (value >> 16) & 0xFF);
    mem_write8(cpu, addr + 3, (value >> 24) & 0xFF);
}
/* -------------------------
   Interrupts
   ------------------------- */
int cpu_interrupts_enabled(CPU *cpu, uint32_t cause) {
    if (cpu->status & STATUS_I_MASK)
        return 0; /* globally masked */

    switch (cause) {
        case CAUSE_TIMER:    return !(cpu->status & STATUS_TR_MASK);
        case CAUSE_TERMINAL: return !(cpu->status & STATUS_TL_MASK);
        default:              return 1; /* invalid instr / software: not maskable */
    }
}

void cpu_enter_interrupt(CPU *cpu, uint32_t cause) {
    cpu->cause = cause;

    /* push status, then pc, in that order (stack grows down) */
    cpu->gpr[SP_REG] -= 4;
    mem_write32(cpu, cpu->gpr[SP_REG], cpu->status);

    cpu->gpr[SP_REG] -= 4;
    mem_write32(cpu, cpu->gpr[SP_REG], cpu->gpr[PC_REG]);

    cpu->status |= STATUS_I_MASK;   /* globally mask further interrupts */
    cpu->gpr[PC_REG] = cpu->handler;
}

void cpu_request_interrupt(CPU *cpu, uint32_t cause) {
    if (cpu_interrupts_enabled(cpu, cause))
        cpu_enter_interrupt(cpu, cause);
    /* if masked, caller/peripheral should keep the request pending
       and retry after the current instruction, per your design */
}

/* -------------------------
   Fetch-decode-execute (stub)
   ------------------------- */
/* -------------------------
   Field extraction helpers
   ------------------------- */
static int32_t sign_extend12(uint32_t val12) {
    val12 &= 0xFFF;
    if (val12 & 0x800)          /* bit 11 set -> negative */
        return (int32_t)(val12 | 0xFFFFF000u);
    return (int32_t)val12;
}

static uint32_t csr_read(CPU *cpu, uint8_t idx) {
    switch (idx) {
        case CSR_STATUS:  return cpu->status;
        case CSR_HANDLER: return cpu->handler;
        case CSR_CAUSE:   return cpu->cause;
        default:          return 0; /* unreachable if decode is correct */
    }
}

static void csr_write(CPU *cpu, uint8_t idx, uint32_t value) {
    switch (idx) {
        case CSR_STATUS:  cpu->status  = value; break;
        case CSR_HANDLER: cpu->handler = value; break;
        case CSR_CAUSE:   cpu->cause   = value; break;
        default: break; /* unreachable if decode is correct */
    }
}

/* -------------------------
   Fetch-decode-execute
   ------------------------- */
void cpu_step(CPU *cpu) {
    uint32_t pc = cpu->gpr[PC_REG];

    uint8_t b0 = mem_read8(cpu, pc);
    uint8_t b1 = mem_read8(cpu, pc + 1);
    uint8_t b2 = mem_read8(cpu, pc + 2);
    uint8_t b3 = mem_read8(cpu, pc + 3);

    uint8_t oc  = (b3 >> 4) & 0xF;
    uint8_t mod = b3 & 0xF;
    uint8_t a   = (b2 >> 4) & 0xF;
    uint8_t b   = b2 & 0xF;
    uint8_t c   = (b1 >> 4) & 0xF;
    uint32_t disp12 = ((b1 & 0xF) << 8) | b0;
    int32_t d = sign_extend12(disp12);

    
    // printf("%2x\n", b0);
    // printf("%2x\n", b1);
    // printf("%2x\n", b2);
    // printf("%2x\n", b3);
    int invalid = 0;
    cpu->gpr[PC_REG] = pc + 4;
    switch (oc) {

    case 0x0: /* halt */
        cpu->halted = 1;
        break;

    case 0x1: /* software interrupt */
        if (cpu->status & STATUS_I_MASK) break;
        cpu->gpr[SP_REG] -= 4;
        mem_write32(cpu, cpu->gpr[SP_REG], cpu->status);
        cpu->gpr[SP_REG] -= 4;
        mem_write32(cpu, cpu->gpr[SP_REG], cpu->gpr[PC_REG]);
        cpu->cause = CAUSE_SOFTWARE;
        cpu->status &= ~STATUS_I_MASK;
        cpu->gpr[PC_REG] = cpu->handler;
        break;

    case 0x2: /* call */
        switch (mod) {
            case 0x0: /* push pc; pc <= gpr[A]+gpr[B]+D */
                cpu->gpr[SP_REG] -= 4;
                mem_write32(cpu, cpu->gpr[SP_REG], cpu->gpr[PC_REG]);
                cpu->gpr[PC_REG] = cpu->gpr[a] + cpu->gpr[b] + d;
                break;
            case 0x1: /* push pc; pc <= mem32[gpr[A]+gpr[B]+D] */
                cpu->gpr[SP_REG] -= 4;
                mem_write32(cpu, cpu->gpr[SP_REG], cpu->gpr[PC_REG]);
                cpu->gpr[PC_REG] = mem_read32(cpu, cpu->gpr[a] + cpu->gpr[b] + d);
                break;
            default: invalid = 1; break;
        }
        break;

    case 0x3: /* jump */
        switch (mod) {
            case 0x0: cpu->gpr[PC_REG] = cpu->gpr[a] + d; break;
            case 0x1: if (cpu->gpr[b] == cpu->gpr[c]) cpu->gpr[PC_REG] = cpu->gpr[a] + d; break;
            case 0x2: if (cpu->gpr[b] != cpu->gpr[c]) cpu->gpr[PC_REG] = cpu->gpr[a] + d; break;
            case 0x3: if ((int32_t)cpu->gpr[b] > (int32_t)cpu->gpr[c]) cpu->gpr[PC_REG] = cpu->gpr[a] + d; break;
            case 0x8: cpu->gpr[PC_REG] = mem_read32(cpu, cpu->gpr[a] + d); break;
            case 0x9: if (cpu->gpr[b] == cpu->gpr[c]) cpu->gpr[PC_REG] = mem_read32(cpu, cpu->gpr[a] + d); break;
            case 0xA: if (cpu->gpr[b] != cpu->gpr[c]) cpu->gpr[PC_REG] = mem_read32(cpu, cpu->gpr[a] + d); break;
            case 0xB: if ((int32_t)cpu->gpr[b] > (int32_t)cpu->gpr[c]) cpu->gpr[PC_REG] = mem_read32(cpu, cpu->gpr[a] + d); break;
            default: invalid = 1; break;
        }
        break;

    case 0x4: /* atomic exchange */
        if (mod == 0x0) {
            uint32_t tmp = cpu->gpr[b];
            cpu->gpr[b] = cpu->gpr[c];
            cpu->gpr[c] = tmp;
        } else {
            invalid = 1;
        }
        break;

    case 0x5: /* arithmetic */
        switch (mod) {
            case 0x0: cpu->gpr[a] = cpu->gpr[b] + cpu->gpr[c]; break;
            case 0x1: cpu->gpr[a] = cpu->gpr[b] - cpu->gpr[c]; break;
            case 0x2: cpu->gpr[a] = cpu->gpr[b] * cpu->gpr[c]; break;
            case 0x3:
                if (cpu->gpr[c] == 0) { invalid = 1; break; }
                cpu->gpr[a] = (int32_t)cpu->gpr[b] / (int32_t)cpu->gpr[c];
                break;
            default: invalid = 1; break;
        }
        break;

    case 0x6: /* logic */
        switch (mod) {
            case 0x0: cpu->gpr[a] = ~cpu->gpr[b]; break;
            case 0x1: cpu->gpr[a] = cpu->gpr[b] & cpu->gpr[c]; break;
            case 0x2: cpu->gpr[a] = cpu->gpr[b] | cpu->gpr[c]; break;
            case 0x3: cpu->gpr[a] = cpu->gpr[b] ^ cpu->gpr[c]; break;
            default: invalid = 1; break;
        }
        break;

    case 0x7: /* shift */
        switch (mod) {
            case 0x0: cpu->gpr[a] = cpu->gpr[b] << (cpu->gpr[c] & 0x1F); break;
            case 0x1: cpu->gpr[a] = cpu->gpr[b] >> (cpu->gpr[c] & 0x1F); break;
            default: invalid = 1; break;
        }
        break;

    case 0x8: /* store */
        switch (mod) {
            case 0x0: mem_write32(cpu, cpu->gpr[a] + cpu->gpr[b] + d, cpu->gpr[c]); break;
            case 0x2: mem_write32(cpu, mem_read32(cpu, cpu->gpr[a] + cpu->gpr[b] + d), cpu->gpr[c]); break;
            case 0x1:
                cpu->gpr[a] = cpu->gpr[a] + d;
                mem_write32(cpu, cpu->gpr[a], cpu->gpr[c]);
                break;
            default: invalid = 1; break;
        }
        break;

    case 0x9: /* load */
        switch (mod) {
            case 0x0: cpu->gpr[a] = csr_read(cpu, b); break;
            case 0x1: cpu->gpr[a] = cpu->gpr[b] + d; break;
            case 0x2: cpu->gpr[a] = mem_read32(cpu, cpu->gpr[b] + cpu->gpr[c] + d); break;
            case 0x3:
                cpu->gpr[a] = mem_read32(cpu, cpu->gpr[b]);
                cpu->gpr[b] = cpu->gpr[b] + d;
                break;
            case 0x4: csr_write(cpu, a, cpu->gpr[b]); break;
            case 0x5: csr_write(cpu, a, csr_read(cpu, b) | (uint32_t)d); break;
            case 0x6: csr_write(cpu, a, mem_read32(cpu, cpu->gpr[b] + cpu->gpr[c] + d)); break;
            case 0x7:
                csr_write(cpu, a, mem_read32(cpu, cpu->gpr[b]));
                cpu->gpr[b] = cpu->gpr[b] + d;
                break;
            default: invalid = 1; break;
        }
        break;

    default:
        invalid = 1;
        break;
    }

    cpu->gpr[0] = 0; /* r0 hardwired to zero, enforced after every step */
    
    if (invalid) {
        printf("Invalid instruction");
        cpu->gpr[SP_REG] -= 4;
        mem_write32(cpu, cpu->gpr[SP_REG], cpu->status);
        cpu->gpr[SP_REG] -= 4;
        mem_write32(cpu, cpu->gpr[SP_REG], pc); /* return to the faulting instruction */
        cpu->cause = CAUSE_INVALID_INSTR;
        cpu->status &= ~STATUS_I_MASK;
        cpu->gpr[PC_REG] = cpu->handler;
    }
}

int load_hex_file(CPU *cpu, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Cannot open input file '%s'\n", filename);
        return -1;
    }

    char line[512];
    int lineno = 0;

    while (fgets(line, sizeof(line), f)) {
        lineno++;

        char *colon = strchr(line, ':');
        if (!colon) continue; /* skip blank / comment / malformed lines */

        *colon = '\0';
        char *addr_str = line;
        char *rest = colon + 1;

        while (isspace((unsigned char)*addr_str)) addr_str++;
        if (*addr_str == '\0') continue;

        char *endptr;
        unsigned long addr = strtoul(addr_str, &endptr, 16);
        if (endptr == addr_str) {
            fprintf(stderr, "Malformed address on line %d\n", lineno);
            fclose(f);
            return -1;
        }

        char *tok = strtok(rest, " \t\r\n");
        while (tok) {
            unsigned int byte;
            if (sscanf(tok, "%2x", &byte) != 1) {
                fprintf(stderr, "Malformed byte '%s' on line %d\n", tok, lineno);
                fclose(f);
                return -1;
            }
            mem_write8(cpu, (uint32_t)addr, (uint8_t)byte);
            addr++;
            tok = strtok(NULL, " \t\r\n");
        }
    }

    fclose(f);
    return 0;
}
static void print_state(const CPU *cpu, int cycle) {
    printf("----- state at cycle %d (after instruction at pc=0x%08X) -----\n", cycle, cpu->gpr[PC_REG] - 4);
    for (int i = 0; i < NUM_GPR; i++) {
        printf("r%-2d=0x%08X%s", i, cpu->gpr[i], ((i + 1) % 4 == 0) ? "\n" : " ");
    }
    printf("status=0x%08X handler=0x%08X cause=0x%08X\n\n",
           cpu->status, cpu->handler, cpu->cause);
}
 
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.hex>\n", argv[0]);
        return 1;
    }
 
    CPU cpu;
    cpu_init(&cpu);
 
    if (load_hex_file(&cpu, argv[1]) != 0) {
        cpu_destroy(&cpu);
        return 1;
    }
    int cycle = 0;
    while (!cpu.halted) {
        
        cpu_step(&cpu);
        print_state(&cpu, cycle);
        cycle++;
        
    }
 
    printf("-----------------------------------------------------------------\n");
    printf("Emulated processor executed halt instruction\n");
    printf("Emulated processor state:\n");
    for (int i = 0; i < NUM_GPR; i++) {
        printf("r%d=0x%08X%s", i, cpu.gpr[i], ((i + 1) % 4 == 0) ? "\n" : " ");
    }
 
    cpu_destroy(&cpu);
    return 0;
}
