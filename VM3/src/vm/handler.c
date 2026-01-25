#include "handler.h"
#include "vm.h"
#include <stdio.h>

void handler_nop(vm_state_t * vm){
    // ne fait rien
    (void) vm;
    VM_IP(vm)+=4;

}

void handler_halt(vm_state_t * vm){
    vm->running=0;  
}

void handler_break(vm_state_t *vm) {
    if (vm->config.debug_enabled) {
        fprintf(stderr, "[BREAK] at IP=0x%04llX\n", (unsigned long long)VM_IP(vm));
        vm_dump(vm);
        
        // Pause et attend une touche pour continuer
        fprintf(stderr, "Press Enter to continue...\n");
        getchar();
    }
    VM_IP(vm)+=4;
}

void handler_mov_rr(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1){
    /* Mov register register (copie contenu d'u registre vers un autre registre)*/
    vm->regs[instr_dst] = vm->regs[instr_src1];
    VM_IP(vm)+=4;
}

void handler_mov_ri(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16){
    /* Charge dans registre valeur immédiate 16 bits etendue à 64 bits*/
    vm->regs[instr_dst] = (uint64_t)instr_imm16;
    VM_IP(vm)+=4;
}

void handler_mov_w0(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16){
    /* Charge dans registre valeur immédiate 16 bits dans les bits 0-15 d'un registre*/
    vm->regs[instr_dst] = (uint64_t)instr_imm16;
    VM_IP(vm)+=4;
}
void handler_mov_w1(vm_state_t *vm, uint8_t instr_dst, uint16_t instr_imm16) {
    /*Charge dans registre valeur immédiate 16 bits dans les bits 16-31 d'un registre
    On utilise le masque 0xFFFFFFFF0000FFFF qui permet de "nettoyer" les bits 16-31 tout en
    conservant le reste. Ensuite on insere.
    */
    vm->regs[instr_dst] = (vm->regs[instr_dst] & 0xFFFFFFFF0000FFFF) | ((uint64_t)instr_imm16 << 16);
    VM_IP(vm) += 4;
}
void handler_mov_w2(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16){
    /* Charge dans registre valeur immédiate 16 bits dans les bits 32-47 d'un registre*/
    vm->regs[instr_dst] = (vm->regs[instr_dst] & 0xFFFF0000FFFFFFFF) | ((uint64_t)instr_imm16 << 32);
    VM_IP(vm)+=4;
}

void handler_mov_w3(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16){
    /* Charge dans registre valeur immédiate 16 bits dans les bits 48-63 d'un registre*/
    vm->regs[instr_dst] = (vm->regs[instr_dst] & 0x0000FFFFFFFFFFFF) | ((uint64_t)instr_imm16 << 48);
    VM_IP(vm)+=4;
}

void handler_xchg(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1){
    /* Echange valeur de deux registres*/
    uint64_t temp = vm->regs[instr_dst];
    vm->regs[instr_dst]=vm->regs[instr_src1];
    vm->regs[instr_src1]=temp;
    VM_IP(vm)+=4;
}

void handler_lea(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1,uint8_t instr_imm8){
    /* REG[INSTR_DST] = REG[SRC1] + IMM8 */

    vm->regs[instr_dst] = vm->regs[instr_src1] + instr_imm8;
    VM_IP(vm)+=4;
}

void handler_add_rr(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1){
    /* 
    REG[INSTR_DST] = REG[INSTR_DST] + REG[INSTR_SRC1]
    
    RESULT = 0 -> ZF
    RESULTAT < 0 -> SF (63 eme bit à 1)
    RESULTAT depassement non signé (depasse mais reste bornes) ->CF
    RESULTAT depassement non signé -> OF

    */
    uint64_t a = (uint64_t)vm->regs[instr_dst];
    uint64_t b = (uint64_t)vm->regs[instr_src1];
    uint64_t result = a + b;

    vm->regs[instr_dst] = (int64_t)result;
    VM_FLAGS(vm)=0;
    
    if (result ==0 ){
        // |= (or composé) -> permet de pas perdre ancien flag
        VM_FLAGS(vm)|= FLAG_ZF;
    }
    if ((int64_t)result <0){
        VM_FLAGS(vm)|= FLAG_SF;
    }
    if (result < a){
        VM_FLAGS(vm) |= FLAG_CF;
    }
        
    if (((a ^ result) & (b ^ result)) >> 63){
        VM_FLAGS(vm) |= FLAG_OF;
    }
        
    VM_IP(vm) += 4;

}
void handler_add_ri(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16){
    /* 
    REG[INSTR_DST] = REG[INSTR_DST] + imm16
    
    RESULT = 0 -> ZF
    RESULTAT < 0 -> SF (63 eme bit à 1)
    RESULTAT depassement non signé (depasse mais reste bornes) ->CF
    RESULTAT depassement non signé -> OF

    */
    uint64_t a = (uint64_t)vm->regs[instr_dst];
    uint64_t b = (uint64_t)instr_imm16;
    uint64_t result = a + b;
    vm->regs[instr_dst] = (int64_t)result;
    VM_FLAGS(vm)=0;
    
    if (result ==0 ){
        // |= (or composé) -> permet de pas perdre ancien flag
        VM_FLAGS(vm)|= FLAG_ZF;
    }
    if ((int64_t)result <0){
        VM_FLAGS(vm)|= FLAG_SF;
    }
    if (result < a){
        VM_FLAGS(vm) |= FLAG_CF;
    }
        
    if (((a ^ result) & (b ^ result)) >> 63){
        VM_FLAGS(vm) |= FLAG_OF;
    }
        
    
    VM_IP(vm) += 4;
}

void handler_sub_rr(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1){
    /* 
    REG[INSTR_DST] = REG[INSTR_DST] - REG[INSTR_SRC1]
    
    RESULT = 0 -> ZF
    RESULTAT < 0 -> SF (63 eme bit à 1)
    RESULTAT depassement non signé (depasse mais reste bornes) ->CF
    RESULTAT depassement non signé -> OF

    */
    uint64_t a = (uint64_t)vm->regs[instr_dst];
    uint64_t b = (uint64_t)vm->regs[instr_src1];
    uint64_t result = a - b;

    vm->regs[instr_dst] = (int64_t)result;
    VM_FLAGS(vm)=0;
    
    if (result ==0 ){
        // |= (or composé) -> permet de pas perdre ancien flag
        VM_FLAGS(vm)|= FLAG_ZF;
    }
    if ((int64_t)result <0){
        VM_FLAGS(vm)|= FLAG_SF;
    }
    if (a < b){
        VM_FLAGS(vm) |= FLAG_CF;
    }
        
    if (((a ^ b) & (a ^ result)) >> 63){
        VM_FLAGS(vm) |= FLAG_OF;
    }
        
    
    VM_IP(vm) += 4;
}


void handler_sub_ri(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16){
    /* 
    REG[INSTR_DST] = REG[INSTR_DST] - REG[INSTR_SRC1]
    
    RESULT = 0 -> ZF
    RESULTAT < 0 -> SF (63 eme bit à 1)
    RESULTAT depassement non signé (depasse mais reste bornes) ->CF
    RESULTAT depassement non signé -> OF

    */
    uint64_t a = (uint64_t)vm->regs[instr_dst];
    uint64_t b = (uint64_t)instr_imm16;
    uint64_t result = a - b;

    vm->regs[instr_dst] = (int64_t)result;
    VM_FLAGS(vm)=0;
    
    if (result ==0 ){
        // |= (or composé) -> permet de pas perdre ancien flag
        VM_FLAGS(vm)|= FLAG_ZF;
    }
    if ((int64_t)result <0){
        VM_FLAGS(vm)|= FLAG_SF;
    }
    if (a < b){
        VM_FLAGS(vm) |= FLAG_CF;
    }
        
    if (((a ^ b) & (a ^ result)) >> 63){
        VM_FLAGS(vm) |= FLAG_OF;
    }
        
    
    VM_IP(vm) += 4;
}

void handler_mul_rr(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1){
    /* 
    REG[INSTR_DST] = REG[INSTR_DST] * REG[INSTR_SRC1]
    
    RESULT = 0 -> ZF
    RESULTAT < 0 -> SF (63 eme bit à 1)
    RESULTAT depassement non signé (depasse mais reste bornes) ->CF
    RESULTAT depassement non signé -> OF

    */
    uint64_t a = (uint64_t)vm->regs[instr_dst];
    uint64_t b = (uint64_t)vm->regs[instr_src1];
    uint64_t result = a * b;

    vm->regs[instr_dst] = (int64_t)result;
    VM_FLAGS(vm)=0;
    
    if (result ==0 ){
        // |= (or composé) -> permet de pas perdre ancien flag
        VM_FLAGS(vm)|= FLAG_ZF;
    }
    if ((int64_t)result <0){
        VM_FLAGS(vm)|= FLAG_SF;
    }
        
    if (b != 0 && result / b != a){
        VM_FLAGS(vm) |= FLAG_CF | FLAG_OF;
    }
    
    VM_IP(vm) += 4;
}


void handler_mul_ri(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16){
    /* 
    REG[INSTR_DST] = REG[INSTR_DST] * INSTR_SRC1
    
    RESULT = 0 -> ZF
    RESULTAT < 0 -> SF (63 eme bit à 1)
    RESULTAT depassement non signé (depasse mais reste bornes) ->CF
    RESULTAT depassement non signé -> OF

    */
    uint64_t a = (uint64_t)vm->regs[instr_dst];
    uint64_t b = (uint64_t)instr_imm16;
    uint64_t result = a * b;

    vm->regs[instr_dst] = (int64_t)result;
    VM_FLAGS(vm)=0;
    
    if (result ==0 ){
        // |= (or composé) -> permet de pas perdre ancien flag
        VM_FLAGS(vm)|= FLAG_ZF;
    }
    if ((int64_t)result <0){
        VM_FLAGS(vm)|= FLAG_SF;
    }
    if (b != 0 && result / b != a){
        VM_FLAGS(vm) |= FLAG_CF | FLAG_OF;
    }
    
    VM_IP(vm) += 4;
}