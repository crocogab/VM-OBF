#include <stdio.h>

#define SETR 0x01
#define ADDR 0x02
#define PRINTR 0x03
#define HALT 0xFF


typedef struct{
    __int8_t regs[4]; // 4 registres
    int pc; // pointeur instruction
}VM;



int main(void){
    VM vm ={0};

    
    __int8_t bytecode[]={SETR,0,10,SETR,1,20,ADDR,2,0,1,PRINTR,2,HALT};

    while (bytecode[vm.pc]!=HALT)
    {
        switch (bytecode[vm.pc])
        {
        case SETR:
            vm.regs[bytecode[vm.pc+1]]=bytecode[vm.pc+2];
            vm.pc+=3;
            break;
        case ADDR:
            vm.regs[bytecode[vm.pc+1]]=vm.regs[bytecode[vm.pc+2]]+vm.regs[bytecode[vm.pc+3]];
            vm.pc+=4;
            break;
        case PRINTR:
            printf("%d\n",vm.regs[bytecode[vm.pc+1]]);
            vm.pc+=2;
            break;
        }
    }
    

    return 0;
}