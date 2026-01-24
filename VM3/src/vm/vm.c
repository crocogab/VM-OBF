#include "opcodes.h"
#include "vm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void vm_init(vm_state_t * vm,size_t memory_size,int debug_enabled){
    /* Initialise VM state debug par default*/
    
    vm->config.debug_enabled=debug_enabled;
    vm->config.memory_size=memory_size;

    memset(vm->regs,0,sizeof(vm->regs));

    /* Allouer memoire a la VM*/

    vm->memory = malloc(memory_size);
    if (vm->memory){
        memset(vm->memory,0,memory_size);
    }

    VM_SP(vm)=memory_size;
    vm->running=0;
}

int vm_load(vm_state_t * vm,const uint8_t* bytecode,size_t size){
    /*
    Charge les bytescodes en memoire
    Renvoie 0 si le byetcode a été chargé
    Renvoie -1 si erreur (memoire pas initialisée / bytecode trop long)
    */

    if (!vm->memory || size>vm->config.memory_size){
        return -1;
    }

    memcpy(vm->memory,bytecode,size);
    VM_IP(vm)=0;
    
    return 0;

}

void vm_destroy(vm_state_t * vm){
    /* Supprime de manière propre la VM */
    if (vm->memory){
        free(vm->memory);
        vm->memory=NULL;
    }
    vm->running = 0;
}

void vm_dump(vm_state_t *vm) {
    printf("=== VM State ===\n");

    // Registres généraux
    for (int i = 0; i < REG_COUNT; i++) {
        printf("R%-2d = 0x%016lx (%ld)\n", i, (uint64_t)vm->regs[i], vm->regs[i]);
    }

    // Registres spéciaux
    printf("IP  = 0x%016lx\n", (uint64_t)VM_IP(vm));
    printf("SP  = 0x%016lx\n", (uint64_t)VM_SP(vm));
    printf("FLG = 0x%016lx [ZF=%d SF=%d CF=%d OF=%d]\n",
           (uint64_t)VM_FLAGS(vm),
           (VM_FLAGS(vm) & FLAG_ZF) ? 1 : 0,
           (VM_FLAGS(vm) & FLAG_SF) ? 1 : 0,
           (VM_FLAGS(vm) & FLAG_CF) ? 1 : 0,
           (VM_FLAGS(vm) & FLAG_OF) ? 1 : 0);

    // Mémoire à IP (bytecode)
    printf("\n=== Memory @ IP ===\n");
    uint64_t ip = VM_IP(vm);
    printf("[IP] ");
    for (int i = 0; i < 16 && ip + i < vm->config.memory_size; i++) {
        printf("%02x ", vm->memory[ip + i]);
    }
    printf("\n");

    // Stack (top 8 valeurs)
    printf("\n=== Stack (top) ===\n");
    uint64_t sp = VM_SP(vm);
    for (int i = 0; i < 8 && sp + i * 8 < vm->config.memory_size; i++) {
        uint64_t addr = sp + i * 8;
        uint64_t val = *(uint64_t *)(vm->memory + addr);
        printf("[SP+%d] 0x%04lx: 0x%016lx\n", i * 8, addr, val);
    }

    printf("================\n");
}
