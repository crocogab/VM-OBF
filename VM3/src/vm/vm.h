#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stddef.h>

// registres generaux
#define REG_COUNT       16

// registres speciaux
#define REG_IP          16
#define REG_SP          17
#define REG_FLAGS       18
#define REG_TOTAL       19

#define DEFAULT_MEMORY_SIZE (64 * 1024)  // 64 KB

// FLAGS bits
#define FLAG_ZF         (1 << 0)  // Zero Flag
#define FLAG_SF         (1 << 1)  // Sign Flag
#define FLAG_CF         (1 << 2)  // Carry Flag
#define FLAG_OF         (1 << 3)  // Overflow Flag

typedef struct vm_config_t
{
    size_t memory_size;
    int debug_enabled;
} vm_config_t;

typedef struct vm_state_t
{
    vm_config_t config;
    
    int64_t regs[REG_TOTAL];  // R0-R15 + IP + SP + FLAGS
    
    
    uint8_t *memory;
    
    
    int running;  // 0 = stopped, 1 = running
    
} vm_state_t;


#define VM_IP(vm)     ((vm)->regs[REG_IP])
#define VM_SP(vm)     ((vm)->regs[REG_SP])
#define VM_FLAGS(vm)  ((vm)->regs[REG_FLAGS])

void vm_dump(vm_state_t *vm);
void vm_destroy(vm_state_t * vm);
int vm_load(vm_state_t * vm,const uint8_t* bytecode,size_t size);
void vm_init(vm_state_t * vm,size_t memory_size,int debug_enabled);

#endif // VM_H