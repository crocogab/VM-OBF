#include <stdio.h>
#include "bytecode.h"
#include "opcodes.h"



typedef struct {
    __int8_t regs[4];
    __uint8_t memory[256];
    int flag;
    int pc;
} VM;

typedef void (*handler_t)(VM*);

// Tableau des clés pré-calculées
__uint8_t keys[BYTECODE_SIZE];

void init_keys() {
    __uint8_t key = INITIAL_KEY;
    for (int i = 0; i < BYTECODE_SIZE; i++) {
        keys[i] = key;
        __uint8_t dec = bytecode[i] ^ key;
        key = (key ^ dec) & 0xFF;
    }
}

__uint8_t read_byte(VM *vm) {
    __uint8_t dec = bytecode[vm->pc] ^ keys[vm->pc];
    vm->pc++;
    return dec;
}

__uint8_t peek_byte(VM *vm) {
    // Lire sans avancer (pour vérifier HALT)
    return bytecode[vm->pc] ^ keys[vm->pc];
}

void handle_setr(VM *vm) {
    __uint8_t reg = read_byte(vm);
    __uint8_t value = read_byte(vm);
    vm->regs[reg] = value;
}

void handle_addr(VM * vm){
    __uint8_t reg_ret = read_byte(vm);
    __uint8_t reg1 = read_byte(vm);
    __uint8_t reg2 = read_byte(vm);
    vm->regs[reg_ret] = vm->regs[reg1] + vm->regs[reg2];

}
void handle_printr(VM *vm) {
    __uint8_t reg = read_byte(vm);
    printf("%d\n", vm->regs[reg]);
}

void handle_load(VM * vm){
    __uint8_t reg = read_byte(vm);
    __uint8_t addr = read_byte(vm);
    vm->regs[reg] = vm->memory[addr];

}

void handle_store(VM * vm){
    __uint8_t addr = read_byte(vm);
    __uint8_t reg = read_byte(vm);
    vm->memory[addr] = vm->regs[reg];
}

void handle_cmp(VM * vm){
    __uint8_t reg1 = read_byte(vm);
    __uint8_t reg2 = read_byte(vm);
    vm->flag = (vm->regs[reg1] == vm->regs[reg2]);
}

void handle_jmp(VM * vm){
    __int8_t offset = (__int8_t)read_byte(vm);
    vm->pc = vm->pc - 2 + offset; 
}

void handle_jeq(VM * vm){
    __int8_t offset = (__int8_t)read_byte(vm);
    if (vm->flag) {
        vm->pc = vm->pc - 2 + offset;
    }
                
}

void handle_jne(VM * vm){
    __int8_t offset = (__int8_t)read_byte(vm);
    if (!vm->flag) {
        vm->pc = vm->pc - 2 + offset;
    }
                
}

int main(void) {
    VM vm = {0};
    init_keys();
    handler_t handlers[256]={0};
    handlers[SETR] = handle_setr;
    handlers[ADDR] = handle_addr;
    handlers[PRINTR] = handle_printr;
    handlers[LOAD] = handle_load;
    handlers[STORE] = handle_store;
    handlers[CMP] = handle_cmp;
    handlers[JMP] = handle_jmp;
    handlers[JEQ] = handle_jeq;
    handlers[JNE] = handle_jne;


    printf("Password: ");
    scanf("%255s", (char*)vm.memory);

    while (peek_byte(&vm) != HALT) {
        __uint8_t opcode = read_byte(&vm);
        handlers[opcode](&vm);
    }

    return 0;
}