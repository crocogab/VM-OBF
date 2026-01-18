#include <stdio.h>
#include <string.h>

#define PUSH 0x01
#define POP  0x02
#define ADD  0x03
#define SUB  0x04
#define CMP  0x05
#define JMP  0x06
#define JEQ  0x07
#define JNE  0x08
#define LOAD 0x09
#define STORE 0x0A 
#define DUP  0x0B 
#define SWAP  0x0C
#define HALT 0xFF

// === OPCODE MAPPING ===
#define PUSH2 0x11
#define PUSH3 0x21
#define PUSH4 0x31

#define POP2  0x12
#define POP3  0x22
#define POP4  0x32

#define ADD2  0x13
#define ADD3  0x23
#define ADD4  0x33

#define SUB2  0x14
#define SUB3  0x24
#define SUB4  0x34

#define CMP2  0x15
#define CMP3  0x25
#define CMP4  0x35

#define JMP2  0x16
#define JMP3  0x26
#define JMP4  0x36

#define JEQ2  0x17
#define JEQ3  0x27
#define JEQ4  0x37

#define JNE2  0x18
#define JNE3  0x28
#define JNE4  0x38

#define LOAD2 0x19
#define LOAD3 0x29
#define LOAD4 0x39

#define STORE2 0x1A
#define STORE3 0x2A
#define STORE4 0x3A

#define DUP2  0x1B
#define DUP3  0x2B
#define DUP4  0x3B

#define SWAP2  0x1C
#define SWAP3  0x2C
#define SWAP4  0x3C

#define HALT2 0xFE
#define HALT3 0xFD
#define HALT4 0xFC

#define KEY  0x37


typedef enum {
    STATE_FETCH,
    STATE_DECODE,
    STATE_EXEC_PUSH,
    STATE_EXEC_POP,
    STATE_EXEC_ADD,
    STATE_EXEC_SUB,
    STATE_EXEC_CMP,
    STATE_EXEC_JMP,
    STATE_EXEC_JEQ,
    STATE_EXEC_JNE,
    STATE_EXEC_LOAD,
    STATE_EXEC_STORE,
    STATE_EXEC_DUP,
    STATE_EXEC_SWAP,
    STATE_HALT,
    STATE_FAKE1,
    STATE_FAKE2,
    STATE_FAKE3,
    STATE_FAKE4,
    STATE_FAKE5
} VMState;


VMState decode_opcode(__uint64_t opcode) {
    // normalise OPCODE
    __uint64_t base = opcode;
    
    // HALT est spécial (0xFF, 0xFE, 0xFD, 0xFC)
    if (opcode >= 0xFC) return STATE_HALT;
    
    base = opcode & 0x0F;
    
    switch (base) {
        case 0x01: return STATE_EXEC_PUSH;
        case 0x02: return STATE_EXEC_POP;
        case 0x03: return STATE_EXEC_ADD;
        case 0x04: return STATE_EXEC_SUB;
        case 0x05: return STATE_EXEC_CMP;
        case 0x06: return STATE_EXEC_JMP;
        case 0x07: return STATE_EXEC_JEQ;
        case 0x08: return STATE_EXEC_JNE;
        case 0x09: return STATE_EXEC_LOAD;
        case 0x0A: return STATE_EXEC_STORE;
        case 0x0B: return STATE_EXEC_DUP;
        case 0x0C: return STATE_EXEC_SWAP;
        default:   return STATE_HALT;
    }
}

// === STRUCTURES ===
typedef struct Stack {
    __uint64_t array[2048];
    __uint64_t capacity;
} Stack;

typedef struct VMContext {
    Stack stack;
    __uint64_t stack_pointer;
    __uint64_t vip;
    __uint64_t flag;
    char input[256];
    __uint64_t decrypt_key;
    int running;  
    __uint64_t memory[256]; // memoire RAM de la VM
} VMContext;

// Forward declaration pour le type Handler
typedef struct VMContext VMContext;
typedef void (*Handler)(VMContext*, __uint64_t*);

// === FONCTIONS UTILITAIRES ===
void push(VMContext *vm, __uint64_t value) {
    vm->stack.array[vm->stack_pointer] = value;
    vm->stack_pointer++;
}

__uint64_t pop(VMContext *vm) {
    vm->stack_pointer--;
    return vm->stack.array[vm->stack_pointer];
}

__uint64_t read_word(VMContext *vm, __uint64_t *bytecode) {
    __uint64_t value = bytecode[vm->vip] ^ vm->decrypt_key;
    vm->decrypt_key = (vm->decrypt_key + 7) & 0xFF;
    vm->vip++;
    return value;
}

void sync_key(VMContext *vm, __uint64_t new_vip) {
    vm->decrypt_key = KEY;
    for (__uint64_t i = 0; i < new_vip; i++) {
        vm->decrypt_key = (vm->decrypt_key + 7) & 0xFF;
    }
    vm->vip = new_vip;
}

// === HANDLERS ===
void handler_push(VMContext *vm, __uint64_t *bytecode) {
    __uint64_t value = read_word(vm, bytecode);
    push(vm, value);
}

void handler_pop(VMContext *vm, __uint64_t *bytecode) {
    (void)bytecode;  // unused
    pop(vm);
}

void handler_add(VMContext *vm, __uint64_t *bytecode) {
    (void)bytecode;
    __uint64_t a = pop(vm);
    __uint64_t b = pop(vm);

    __uint64_t t1 = 3*(a|b);
    __uint64_t t2 = 2*(~(a ^ b));
    __uint64_t t3 = ~(a & b);
    push(vm, t1+t2+t3+3);
}

void handler_sub(VMContext *vm, __uint64_t *bytecode) {
    (void)bytecode;
    __uint64_t a = pop(vm);
    __uint64_t b = pop(vm);

    __uint64_t t1 = 3*(a|~b);
    __uint64_t t2 = 2*(a ^ b);
    __uint64_t t3 = ~(a & ~b);

    push(vm, t1+t2+t3+4);
}

void handler_cmp(VMContext *vm, __uint64_t *bytecode) {
    (void)bytecode;
    __uint64_t a = pop(vm);
    __uint64_t b = pop(vm);

    __uint64_t d = (a + b) - 2 * (a & b);
    __uint64_t i = (d | (0 - d)) >> 63;
    vm->flag = i ^ 1;
}

void handler_jmp(VMContext *vm, __uint64_t *bytecode) {
    __uint64_t addr = read_word(vm, bytecode);
    sync_key(vm, addr);
}

void handler_jeq(VMContext *vm, __uint64_t *bytecode) {
    __uint64_t addr = read_word(vm, bytecode);
    if (vm->flag)
        sync_key(vm, addr);
}

void handler_jne(VMContext *vm, __uint64_t *bytecode) {
    __uint64_t addr = read_word(vm, bytecode);
    if (!vm->flag)
        sync_key(vm, addr);
}

void handler_load(VMContext *vm, __uint64_t *bytecode) {
    __uint64_t index = read_word(vm, bytecode);
    push(vm, vm->input[index]);
}

void handler_halt(VMContext *vm, __uint64_t *bytecode) {
    (void)bytecode;
    vm->running = 0;
}

void handler_invalid(VMContext *vm, __uint64_t *bytecode) {
    (void)bytecode;
    printf("ERREUR: Opcode invalide à vip=%llu\n", vm->vip - 1);
    vm->running = 0;
}

void handler_store(VMContext *vm,__uint64_t *bytecode){
    // memory[idx] = pop()
    __uint64_t index = read_word(vm,bytecode);
    __uint64_t value = pop(vm);
    vm->memory[index] = value;
}

void handler_dup(VMContext *vm,__uint64_t *bytecode){
    // push(stack[sp-1]) sans dépiler
    (void) bytecode;
    push(vm,vm->stack.array[vm->stack_pointer-1]);
}

void handler_swap(VMContext *vm,__uint64_t *bytecode){
    (void) bytecode;
    __uint64_t a = pop(vm); // top de la pile
    __uint64_t b = pop(vm);
    push(vm,a);
    push(vm,b);
}


// === TABLE DE HANDLERS ===
Handler handlers[256];

void init_handlers(void) {
    // Remplir avec handler_invalid par défaut
    for (int i = 0; i < 256; i++) {
        handlers[i] = handler_invalid;
    }
    
    // PUSH
    handlers[PUSH]  = handler_push;
    handlers[PUSH2] = handler_push;
    handlers[PUSH3] = handler_push;
    handlers[PUSH4] = handler_push;
    
    // POP
    handlers[POP]  = handler_pop;
    handlers[POP2] = handler_pop;
    handlers[POP3] = handler_pop;
    handlers[POP4] = handler_pop;
    
    // ADD
    handlers[ADD]  = handler_add;
    handlers[ADD2] = handler_add;
    handlers[ADD3] = handler_add;
    handlers[ADD4] = handler_add;
    
    // SUB
    handlers[SUB]  = handler_sub;
    handlers[SUB2] = handler_sub;
    handlers[SUB3] = handler_sub;
    handlers[SUB4] = handler_sub;
    
    // CMP
    handlers[CMP]  = handler_cmp;
    handlers[CMP2] = handler_cmp;
    handlers[CMP3] = handler_cmp;
    handlers[CMP4] = handler_cmp;
    
    // JMP
    handlers[JMP]  = handler_jmp;
    handlers[JMP2] = handler_jmp;
    handlers[JMP3] = handler_jmp;
    handlers[JMP4] = handler_jmp;
    
    // JEQ
    handlers[JEQ]  = handler_jeq;
    handlers[JEQ2] = handler_jeq;
    handlers[JEQ3] = handler_jeq;
    handlers[JEQ4] = handler_jeq;
    
    // JNE
    handlers[JNE]  = handler_jne;
    handlers[JNE2] = handler_jne;
    handlers[JNE3] = handler_jne;
    handlers[JNE4] = handler_jne;
    
    // LOAD
    handlers[LOAD]  = handler_load;
    handlers[LOAD2] = handler_load;
    handlers[LOAD3] = handler_load;
    handlers[LOAD4] = handler_load;

    //STORE
    handlers[STORE]  = handler_store;
    handlers[STORE2] = handler_store;
    handlers[STORE3] = handler_store;
    handlers[STORE4] = handler_store;

    //DUP
    handlers[DUP]  = handler_dup;
    handlers[DUP2] = handler_dup;
    handlers[DUP3] = handler_dup;
    handlers[DUP4] = handler_dup;

    //SWAP
    handlers[SWAP]  = handler_swap;
    handlers[SWAP2] = handler_swap;
    handlers[SWAP3] = handler_swap;
    handlers[SWAP4] = handler_swap;
    
    // HALT
    handlers[HALT]  = handler_halt;
    handlers[HALT2] = handler_halt;
    handlers[HALT3] = handler_halt;
    handlers[HALT4] = handler_halt;

    
}

// RUN
void run(VMContext *vm, __uint64_t *bytecode) {
    VMState state = STATE_FETCH;
    __uint64_t current_opcode = 0;
    vm->running = 1;
    
    while (1) {
        switch (state) {
            case STATE_FAKE1:  
                vm->flag = vm->stack_pointer ^ 0xDEAD;
                state = STATE_FAKE2;
            break;

            case STATE_EXEC_SUB:
                handler_sub(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            case STATE_EXEC_SWAP:
                handler_swap(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            // === DECODE : Déterminer l'instruction ===
            case STATE_DECODE:
                switch (current_opcode) {
                    case PUSH: case PUSH2: case PUSH3: case PUSH4:
                        state = STATE_EXEC_PUSH;
                        break;
                    case POP: case POP2: case POP3: case POP4:
                        state = STATE_EXEC_POP;
                        break;
                    case ADD: case ADD2: case ADD3: case ADD4:
                        state = STATE_EXEC_ADD;
                        break;
                    case SUB: case SUB2: case SUB3: case SUB4:
                        state = STATE_EXEC_SUB;
                        break;
                    case CMP: case CMP2: case CMP3: case CMP4:
                        state = STATE_EXEC_CMP;
                        break;
                    case JMP: case JMP2: case JMP3: case JMP4:
                        state = STATE_EXEC_JMP;
                        break;
                    case JEQ: case JEQ2: case JEQ3: case JEQ4:
                        state = STATE_EXEC_JEQ;
                        break;
                    case JNE: case JNE2: case JNE3: case JNE4:
                        state = STATE_EXEC_JNE;
                        break;
                    case LOAD: case LOAD2: case LOAD3: case LOAD4:
                        state = STATE_EXEC_LOAD;
                        break;
                    case STORE: case STORE2: case STORE3: case STORE4:
                        state = STATE_EXEC_STORE;
                        break;
                    case DUP: case DUP2: case DUP3: case DUP4:
                        state = STATE_EXEC_DUP;
                        break;
                    case SWAP: case SWAP2: case SWAP3: case SWAP4:
                        state = STATE_EXEC_SWAP;
                        break;
                    case HALT: case HALT2: case HALT3: case HALT4:
                        state = STATE_HALT;
                        break;
                    default:
                        // OPCODE invalide
                        printf("ERREUR: Opcode invalide 0x%02llX à vip=%llu\n", 
                               current_opcode, vm->vip - 1);
                        state = STATE_HALT;
                        break;
                }
                break;   
            
            case STATE_EXEC_PUSH:
                handler_push(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            case STATE_EXEC_POP:
                handler_pop(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            case STATE_EXEC_ADD:
                handler_add(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;  
            
            case STATE_EXEC_CMP:
                handler_cmp(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            case STATE_FAKE5:
                state = STATE_DECODE;
                break;
            
            // === lecture OPCODE ===
            case STATE_FETCH:
                current_opcode = read_word(vm, bytecode);

                int x = vm->vip;
                if ((x * (x + 1)) % 2 == 0) {  // Toujours vrai !
                    state = STATE_DECODE;
                } else {
                    state = STATE_HALT;  
                }
                
                break;
            case STATE_EXEC_JMP:
                handler_jmp(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            case STATE_EXEC_JEQ:
                handler_jeq(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            case STATE_FAKE4:
                handler_add(vm,bytecode);
                state = STATE_FAKE5;
                break;

            case STATE_EXEC_JNE:
                handler_jne(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            case STATE_EXEC_LOAD:
                handler_load(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            case STATE_EXEC_STORE:
                handler_store(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            case STATE_EXEC_DUP:
                handler_dup(vm, bytecode);
                state = vm->running ? STATE_FETCH : STATE_HALT;
                break;
            
            case STATE_FAKE2:
                handler_push(vm, bytecode);  
                state = STATE_HALT;
                break;

            case STATE_FAKE3:
                current_opcode=read_word(vm,bytecode);
                current_opcode = current_opcode & 0x3c62;
                break;
            
            
            case STATE_HALT:
                return;
        }
    }
}


void init_vm(VMContext *vm) {
    Stack stack = {0};
    vm->stack = stack;
    vm->stack_pointer = 0;
    vm->vip = 0;
    vm->flag = 0;
    vm->running = 0;
    memset(vm->input, 0, sizeof(vm->input));
    vm->decrypt_key = KEY;
    memset(vm->memory, 0, sizeof(vm->memory));
}


// === MAIN ===
int main(void) {
    init_handlers();  // IMPORTANT : initialiser la table
    
    VMContext vm;
    init_vm(&vm);
    printf("Entrez l'input : ");
    scanf("%255s", vm.input);

    __uint64_t bytecode[] = {
    0x1E, 0x3E, 0x74, 0x1F, 0x66, 0x62, 0x6A, 0x59, 0x6E, 0x50, 0x70, 0x85, 0x8B, 0x6C
    };
    
    
    

    run(&vm, bytecode);
    printf("Valeur sommet pile : %llu\n", vm.stack.array[vm.stack_pointer - 1]);

    return 0;
}