#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

/* =========================
   Reset / memory layout
   ========================= */
#define RESET_VECTOR 0x40000000u

#define RAM_BASE     0x40000000u
#define RAM_SIZE     (1 * 1024 * 1024)   /* 1 MiB */
#define RAM_END      RAM_BASE + RAM_SIZE
#define MMIO_BASE    0xFFFFFF00u
#define MMIO_SIZE    256u
/* =========================
   Registers
   ========================= */
#define NUM_GPR   16
#define PC_REG    15   /* r15 = program counter */
#define SP_REG    14   /* r14 = stack pointer   */

/* status register bit masks (low 16 bits) */
#define STATUS_TR_MASK   (1u << 0)  /* Timer interrupt mask    */
#define STATUS_TL_MASK   (1u << 1)  /* Terminal interrupt mask */
#define STATUS_I_MASK    (1u << 2)  /* Global interrupt mask   */

/* cause register values */
#define CAUSE_INVALID_INSTR   1u
#define CAUSE_TIMER           2u
#define CAUSE_TERMINAL        3u
#define CAUSE_SOFTWARE        4u

#define CSR_STATUS   0
#define CSR_HANDLER  1
#define CSR_CAUSE    2


typedef struct {
    uint32_t gpr[NUM_GPR];  /* r0..r15, r0 hardwired to 0, r15 = pc, r14 = sp */
    uint32_t status;
    uint32_t handler;
    uint32_t cause;

    uint8_t *ram;         
    int halted;
} CPU;

/* lifecycle */
void cpu_init(CPU *cpu);
void cpu_destroy(CPU *cpu);
void cpu_reset(CPU *cpu);   /* cold or warm restart: pc = RESET_VECTOR */

/* memory access (little-endian) */
uint8_t  mem_read8 (CPU *cpu, uint32_t addr);
uint32_t mem_read32(CPU *cpu, uint32_t addr);
void     mem_write8 (CPU *cpu, uint32_t addr, uint8_t  value);
void     mem_write32(CPU *cpu, uint32_t addr, uint32_t value);

/* interrupts */
void cpu_request_interrupt(CPU *cpu, uint32_t cause);
int  cpu_interrupts_enabled(CPU *cpu, uint32_t cause);
void cpu_enter_interrupt(CPU *cpu, uint32_t cause);

/* main loop */
void cpu_step(CPU *cpu);
void cpu_run(CPU *cpu);

#endif