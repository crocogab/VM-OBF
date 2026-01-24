#include "vm.h"
#include <stdio.h>


int main(void){
    vm_state_t vm;

    const uint8_t bytecode[]={0x01,0x03,0x02,0x01};
    vm_init(&vm,DEFAULT_MEMORY_SIZE,1);
    vm_dump(&vm);
    vm_load(&vm,bytecode,sizeof(bytecode));
    vm_dump(&vm);
    vm_destroy(&vm);
    return 0;
}