#include "vm.h"
#include "opcodes.h"
#include "handler.h"
#include <stdio.h>

void vm_run(vm_state_t *vm) {
    while (vm->running) {
        uint32_t instruction = *(uint32_t *)(vm->memory + VM_IP(vm));
        uint8_t instr_opcode = INSTR_OPCODE(instruction);
        uint8_t instr_dst = INSTR_DST(instruction);
        uint8_t instr_src1 = INSTR_SRC1(instruction);
        uint8_t instr_src2 = INSTR_SRC2(instruction);
        uint16_t instr_imm16 = INSTR_IMM16(instruction);
        int8_t instr_imm8 = INSTR_IMM8(instruction);

        switch (instr_opcode) {

        case OP_NOP:
        case OP_NOP_V2:
        case OP_NOP_V3:
        case OP_NOP_V4:
        case OP_NOP_V5:
        case OP_NOP_V6:
        case OP_NOP_V7:
            handler_nop(vm);
            break;

        case OP_HALT:
            handler_halt(vm);
            break;

        case OP_BREAK:
            handler_break(vm);
            break;

        case OP_MOV_RR:
        case OP_MOV_RR_V2:
        case OP_MOV_RR_V3:
            handler_mov_rr(vm, instr_dst, instr_src1);
            break;

        case OP_MOV_RI:
            handler_mov_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_MOV_W0:
            handler_mov_w0(vm, instr_dst, instr_imm16);
            break;

        case OP_MOV_W1:
            handler_mov_w1(vm, instr_dst, instr_imm16);
            break;

        case OP_MOV_W2:
            handler_mov_w2(vm, instr_dst, instr_imm16);
            break;

        case OP_MOV_W3:
            handler_mov_w3(vm, instr_dst, instr_imm16);
            break;

        case OP_XCHG:
            handler_xchg(vm, instr_dst, instr_src1);
            break;

        case OP_LEA:
            handler_lea(vm, instr_dst, instr_src1, instr_imm8);
            break;

        case OP_ADD_RR:
        case OP_ADD_RR_V2:
        case OP_ADD_RR_V3:
            handler_add_rr(vm, instr_dst, instr_src1);
            break;

        case OP_ADD_RI:
            handler_add_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_SUB_RR:
        case OP_SUB_RR_V2:
            handler_sub_rr(vm, instr_dst, instr_src1);
            break;

        case OP_SUB_RI:
            handler_sub_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_MUL_RR:
            handler_mul_rr(vm, instr_dst, instr_src1);
            break;

        case OP_MUL_RI:
            handler_mul_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_IMUL_RR:
            handler_imul_rr(vm, instr_dst, instr_src1);
            break;

        case OP_DIV_RR:
            handler_div_rr(vm, instr_dst, instr_src1);
            break;

        case OP_DIV_RI:
            handler_div_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_IDIV_RR:
            handler_idiv_rr(vm, instr_dst, instr_src1);
            break;

        case OP_MOD_RR:
            handler_mod_rr(vm, instr_dst, instr_src1);
            break;

        case OP_MOD_RI:
            handler_mod_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_IMOD_RR:
            handler_imod_rr(vm, instr_dst, instr_src1);
            break;

        case OP_INC:
            handler_inc(vm, instr_dst);
            break;

        case OP_DEC:
            handler_dec(vm, instr_dst);
            break;

        case OP_NEG:
            handler_neg(vm, instr_dst);
            break;

        case OP_ABS:
            handler_abs(vm, instr_dst);
            break;

        case OP_AND_RR:
        case OP_AND_RR_V2:
            handler_and_rr(vm, instr_dst, instr_src1);
            break;

        case OP_AND_RI:
            handler_and_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_OR_RR:
            handler_or_rr(vm, instr_dst, instr_src1);
            break;

        case OP_OR_RI:
            handler_or_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_XOR_RR:
        case OP_XOR_RR_V2:
            handler_xor_rr(vm, instr_dst, instr_src1);
            break;

        case OP_XOR_RI:
            handler_xor_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_NOT:
            handler_not(vm, instr_dst);
            break;

        case OP_SHL_RR:
            handler_shl_rr(vm, instr_dst, instr_src1);
            break;

        case OP_SHL_RI:
            handler_shl_ri(vm, instr_dst, instr_src1);
            break;

        case OP_SHR_RR:
            handler_shr_rr(vm, instr_dst, instr_src1);
            break;

        case OP_SHR_RI:
            handler_shr_ri(vm, instr_dst, instr_src1);
            break;

        case OP_SAR_RR:
            handler_sar_rr(vm, instr_dst, instr_src1);
            break;

        case OP_SAR_RI:
            handler_sar_ri(vm, instr_dst, instr_src1);
            break;

        case OP_ROL_RR:
            handler_rol_rr(vm, instr_dst, instr_src1);
            break;

        case OP_ROL_RI:
            handler_rol_ri(vm, instr_dst, instr_src1);
            break;

        case OP_ROR_RR:
            handler_ror_rr(vm, instr_dst, instr_src1);
            break;

        case OP_ROR_RI:
            handler_ror_ri(vm, instr_dst, instr_src1);
            break;

        case OP_BSWAP:
            handler_bswap(vm, instr_dst);
            break;

        case OP_CTZ:
            handler_ctz(vm, instr_dst, instr_src1);
            break;

        case OP_CLZ:
            handler_clz(vm, instr_dst, instr_src1);
            break;

        case OP_POPCNT:
            handler_popcnt(vm, instr_dst, instr_src1);
            break;

        case OP_CMP_RR:
            handler_cmp_rr(vm, instr_dst, instr_src1);
            break;

        case OP_CMP_RI:
            handler_cmp_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_TEST_RR:
            handler_test_rr(vm, instr_dst, instr_src1);
            break;

        case OP_TEST_RI:
            handler_test_ri(vm, instr_dst, instr_imm16);
            break;

        case OP_CLC:
            handler_clc(vm);
            break;

        case OP_STC:
            handler_stc(vm);
            break;

        case OP_CMC:
            handler_cmc(vm);
            break;

        case OP_LOAD64:
        case OP_LOAD64_V2:
            handler_load64(vm, instr_dst, instr_src1);
            break;

        case OP_LOAD32:
            handler_load32(vm, instr_dst, instr_src1);
            break;

        case OP_LOAD16:
            handler_load16(vm, instr_dst, instr_src1);
            break;

        case OP_LOAD8:
            handler_load8(vm, instr_dst, instr_src1);
            break;

        case OP_LOAD32S:
            handler_load32s(vm, instr_dst, instr_src1);
            break;

        case OP_LOAD16S:
            handler_load16s(vm, instr_dst, instr_src1);
            break;

        case OP_LOAD8S:
            handler_load8s(vm, instr_dst, instr_src1);
            break;

        case OP_STORE64:
        case OP_STORE64_V2:
            handler_store64(vm, instr_dst, instr_src1);
            break;

        case OP_STORE32:
            handler_store32(vm, instr_dst, instr_src1);
            break;

        case OP_STORE16:
            handler_store16(vm, instr_dst, instr_src1);
            break;

        case OP_STORE8:
            handler_store8(vm, instr_dst, instr_src1);
            break;

        case OP_MEMCPY:
            handler_memcpy(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_MEMSET:
            handler_memset(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_MEMCMP:
            handler_memcmp(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_PUSH:
        case OP_PUSH_V2:
            handler_push(vm, instr_dst);
            break;

        case OP_POP:
        case OP_POP_V2:
            handler_pop(vm, instr_dst);
            break;

        case OP_PUSH_RI:
            handler_push_ri(vm, instr_imm16);
            break;

        case OP_PUSHF:
            handler_pushf(vm);
            break;

        case OP_POPF:
            handler_popf(vm);
            break;

        case OP_DUP:
            handler_dup(vm);
            break;

        case OP_DROP:
            handler_drop(vm);
            break;

        case OP_SWAP_STK:
            handler_swap_stk(vm);
            break;

        case OP_JMP:
        case OP_JMP_V2:
            handler_jmp(vm, instr_imm16);
            break;

        case OP_JMP_R:
            handler_jmp_r(vm, instr_dst);
            break;

        case OP_JZ:
        case OP_JZ_V2:
            handler_jz(vm, instr_imm16);
            break;

        case OP_JNZ:
        case OP_JNZ_V2:
            handler_jnz(vm, instr_imm16);
            break;

        case OP_JG:
            handler_jg(vm, instr_imm16);
            break;

        case OP_JGE:
            handler_jge(vm, instr_imm16);
            break;

        case OP_JL:
            handler_jl(vm, instr_imm16);
            break;

        case OP_JLE:
            handler_jle(vm, instr_imm16);
            break;

        case OP_JA:
            handler_ja(vm, instr_imm16);
            break;

        case OP_JAE:
            handler_jae(vm, instr_imm16);
            break;

        case OP_JB:
            handler_jb(vm, instr_imm16);
            break;

        case OP_JBE:
            handler_jbe(vm, instr_imm16);
            break;

        case OP_JS:
            handler_js(vm, instr_imm16);
            break;

        case OP_JNS:
            handler_jns(vm, instr_imm16);
            break;

        case OP_JO:
            handler_jo(vm, instr_imm16);
            break;

        case OP_JNO:
            handler_jno(vm, instr_imm16);
            break;

        case OP_CALL:
        case OP_CALL_V2:
            handler_call(vm, instr_imm16);
            break;

        case OP_CALL_R:
            handler_call_r(vm, instr_dst);
            break;

        case OP_RET:
        case OP_RET_V2:
            handler_ret(vm);
            break;

        case OP_RET_RI:
            handler_ret_ri(vm, instr_imm16);
            break;

        case OP_LOOP:
            handler_loop(vm, instr_dst, instr_imm16);
            break;

        case OP_LOOPZ:
            handler_loopz(vm, instr_dst, instr_imm16);
            break;

        case OP_LOOPNZ:
            handler_loopnz(vm, instr_dst, instr_imm16);
            break;

        case OP_ENTER:
            handler_enter(vm, instr_imm16);
            break;

        case OP_LEAVE:
            handler_leave(vm);
            break;

        case OP_NET_INIT:
            handler_net_init(vm);
            break;

        case OP_NET_SOCKET:
            handler_net_socket(vm);
            break;

        case OP_NET_CONNECT:
            handler_net_connect(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_NET_BIND:
            handler_net_bind(vm, instr_dst, instr_src1);
            break;

        case OP_NET_LISTEN:
            handler_net_listen(vm, instr_dst, instr_src1);
            break;

        case OP_NET_ACCEPT:
            handler_net_accept(vm, instr_dst);
            break;

        case OP_NET_SEND:
            handler_net_send(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_NET_RECV:
            handler_net_recv(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_NET_CLOSE:
            handler_net_close(vm, instr_dst);
            break;

        case OP_NET_CLEANUP:
            handler_net_cleanup(vm);
            break;

        case OP_NET_SETSOPT:
            handler_net_setsopt(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_NET_SELECT:
            handler_net_select(vm, instr_dst, instr_src1);
            break;

        case OP_CRYPTO_RAND:
            handler_crypto_rand(vm, instr_dst, instr_src1);
            break;

        case OP_CRYPTO_HASH:
            handler_crypto_hash(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_CRYPTO_HMAC:
            handler_crypto_hmac(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_CRYPTO_KEYGEN:
            handler_crypto_keygen(vm, instr_dst, instr_src1);
            break;

        case OP_CRYPTO_DH:
            handler_crypto_dh(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_CRYPTO_ENC:
            handler_crypto_enc(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_CRYPTO_DEC:
            handler_crypto_dec(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_CRYPTO_VERIFY:
            handler_crypto_verify(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_CRYPTO_KDF:
            handler_crypto_kdf(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_CRYPTO_SIGN:
            handler_crypto_sign(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_CRYPTO_VSIGN:
            handler_crypto_vsign(vm, instr_dst, instr_src1, instr_src2);
            break;

        case OP_SYSCALL:
            handler_syscall(vm, instr_dst);
            break;

        case OP_DEBUG:
            handler_debug(vm, instr_dst);
            break;

        case OP_GETTIME:
            handler_gettime(vm);
            break;

        case OP_SLEEP:
            handler_sleep(vm, instr_dst);
            break;

        case OP_RDTSC:
            handler_rdtsc(vm);
            break;

        case OP_CPUID:
            handler_cpuid(vm);
            break;

        case OP_TRAP:
            handler_trap(vm);
            break;

        case OP_UD:
        case OP_INVALID:
        default:
            handler_invalid(vm, instr_opcode);
            break;
        }
    }
}