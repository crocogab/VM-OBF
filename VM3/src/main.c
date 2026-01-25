#include "vm.h"
#include "dispatcher.h"
#include "opcodes.h"
#include <stdio.h>


int main(void){
    vm_state_t vm;

    /* 
    Test (5+3)*2
    LOAD r0,2
    LOAD r1,5
    LOAD r2,3
    ADD r1,r2,r1
    MUL r1,r0,r1
    */
    const uint8_t bytecode[]={OP_ADD_RR,1,2,0,OP_MUL_RR,1,0,0, OP_HALT,   0, 0, 0};
    vm_init(&vm,DEFAULT_MEMORY_SIZE,1);
    vm.regs[0]=2;
    vm.regs[1]=5;
    vm.regs[2]=3;

    vm_dump(&vm);
    vm_load(&vm,bytecode,sizeof(bytecode));
    vm_dump(&vm);
    vm_run(&vm);
    vm_dump(&vm);
    vm_destroy(&vm);
    return 0;
}