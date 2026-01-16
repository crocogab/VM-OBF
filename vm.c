#include <stdio.h>
#include "bytecode.h"
#include "opcodes.h"

#define READ(offset) ((__uint8_t)(bytecode[vm.pc + offset] ^ KEY))


typedef struct{
    __int8_t regs[4]; // 4 registres
    int pc; // pointeur instruction
    __uint8_t memory[256]; // RAM
    int flag; // flag resultat condition booleenne
}VM;



int main(void){
    VM vm ={0};
    

    printf("Password: ");
    scanf("%255s", (char*)vm.memory);


    while (READ(0)!=HALT)
    {   
        
        switch (READ(0))
        {
        
        case SETR:{
            __uint8_t reg = READ(1);
            __uint8_t value = READ(2);

            vm.regs[reg]=value;
            vm.pc+=3;
            break;
        }    
        case ADDR:{
            __uint8_t reg_ret = READ(1);
            __uint8_t reg1 =READ(2);
            __uint8_t reg2 = READ(3);
            
            vm.regs[reg_ret]=vm.regs[reg1]+vm.regs[reg2];
            vm.pc+=4;
            break;
        }
        case PRINTR:{
            __uint8_t reg=READ(1);
            printf("%d\n",vm.regs[reg]);
            vm.pc+=2;
            break;
        }
        case LOAD:{
            __uint8_t reg = READ(1);
            __uint8_t addr = READ(2);

            vm.regs[reg] = vm.memory[addr];
            vm.pc+=3;
            break;
        }
        case STORE:{
            __uint8_t addr = READ(1);
            __uint8_t reg = READ(2);

            vm.memory[addr] = vm.regs[reg];
            vm.pc+=3;
            break;
        }
        case CMP:{
            __uint8_t reg1 = READ(1);
            __uint8_t reg2 = READ(2);

            vm.flag= (vm.regs[reg1] == vm.regs[reg2]);
            vm.pc+=3;
            break;
        }
        case JMP:{
            // attention __int8_t et non pas __uint8_t car offset signé
            __int8_t offset = READ(1);
            vm.pc+=offset;
            break;
        }
        case JEQ:{
            __int8_t offset = READ(1);
            if (vm.flag){
                vm.pc+=offset;
            }else{
                vm.pc+=2;
            }
            break;
        }
        case JNE:{
            __int8_t offset = READ(1);
            if (!vm.flag){
                vm.pc+=offset;
            }else{
                vm.pc+=2;
            }
            break;

        }

        }
        
    }
    

    return 0;
}