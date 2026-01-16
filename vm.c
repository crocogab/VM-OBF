#include <stdio.h>

#define SETR 0x01
#define ADDR 0x02
#define PRINTR 0x03

// mémoire RAM
#define LOAD 0x04
#define STORE 0x05

// CMP + JMP
#define CMP 0x06

#define HALT 0xFF



typedef struct{
    __int8_t regs[4]; // 4 registres
    int pc; // pointeur instruction
    __uint8_t memory[256]; // RAM
    int flag; // flag resultat condition booleenne
}VM;



int main(void){
    VM vm ={0};
    vm.memory[0]='H';
    vm.memory[1]='I';
    
    __int8_t bytecode[]={LOAD,0,0,LOAD,1,1,PRINTR,0,PRINTR,1,HALT};

    while (bytecode[vm.pc]!=HALT)
    {
        switch (bytecode[vm.pc])
        {
        case SETR:{
            __uint8_t reg = bytecode[vm.pc+1];
            __uint8_t value =bytecode[vm.pc+2];

            vm.regs[reg]=value;
            vm.pc+=3;
            break;
        }    
        case ADDR:{
            __uint8_t reg_ret = bytecode[vm.pc+1];
            __uint8_t reg1 = bytecode[vm.pc+2];
            __uint8_t reg2 = bytecode[vm.pc+3];
            
            vm.regs[reg_ret]=vm.regs[reg1]+vm.regs[reg2];
            vm.pc+=4;
            break;
        }
        case PRINTR:{
            __uint8_t reg=bytecode[vm.pc+1];
            printf("%d\n",vm.regs[reg]);
            vm.pc+=2;
            break;
        }
        case LOAD:{
            __uint8_t reg = bytecode[vm.pc+1];
            __uint8_t addr = bytecode[vm.pc+2];

            vm.regs[reg] = vm.memory[addr];
            vm.pc+=3;
            break;
        }
        case STORE:{
            __uint8_t addr = bytecode[vm.pc+1];
            __uint8_t reg = bytecode[vm.pc+2];

            vm.memory[addr] = vm.regs[reg];
            vm.pc+=3;
            break;
        }
        }
        
    }
    

    return 0;
}