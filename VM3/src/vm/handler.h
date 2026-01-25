#ifndef HANDLER_H
#define HANDLER_H

#include "vm.h"


void handler_nop(vm_state_t * vm);
void handler_halt(vm_state_t * vm);
void handler_break(vm_state_t * vm);
void handler_mov_rr(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1);
void handler_mov_ri(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16);
void handler_mov_w0(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16);
void handler_mov_w1(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16);
void handler_mov_w2(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16);
void handler_mov_w3(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16);
void handler_xchg(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1);
void handler_lea(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1,uint8_t instr_imm8);
void handler_add_rr(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1);
void handler_add_ri(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16);
void handler_sub_rr(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1);
void handler_sub_ri(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16);
void handler_mul_rr(vm_state_t * vm,uint8_t instr_dst, uint8_t instr_src1);
void handler_mul_ri(vm_state_t * vm,uint8_t instr_dst, uint16_t instr_imm16);

#endif