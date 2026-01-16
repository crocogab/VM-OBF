#include <stdio.h>


#define KEY 0x9B

#define SETR 0x01
#define ADDR 0x02
#define PRINTR 0x03

// mémoire RAM
#define LOAD 0x04
#define STORE 0x05

// CMP + JMP
#define CMP 0x06
#define JMP 0x07
#define JEQ 0x08
#define JNE 0x09

#define HALT 0xFF

#define READ(offset) ((uint8_t)(bytecode[vm.pc + offset] ^ KEY))


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

    __uint8_t bytecode[] = {0x9F, 0x9B, 0x9B, 0x9A, 0x9A, 0xDA, 0x9D, 0x9B, 0x9A, 0x92, 0x86, 0x9F, 0x9B, 0x9A, 0x9A, 0x9A, 0xD9, 0x9D, 0x9B, 0x9A, 0x92, 0x89, 0x9F, 0x9B, 0x99, 0x9A, 0x9A, 0xD8, 0x9D, 0x9B, 0x9A, 0x92, 0x9C, 0x9A, 0x99, 0x9A, 0x9C, 0x9E, 0x9A, 0x99, 0x9B, 0x98, 0x99, 0x64};
    
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