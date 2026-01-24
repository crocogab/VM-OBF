// src/vm/opcodes.h

#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>


typedef enum {
    
    // CONTROLE VM (0x00 - 0x0F)
    
    OP_NOP          = 0x00,     // No operation
    OP_HALT         = 0x01,     // Arrêt de la VM
    OP_BREAK        = 0x02,     // Breakpoint (debug)

    
    // TRANSFERT DE DONNÉES (0x10 - 0x1F)
    
    OP_MOV_RR       = 0x10,     // MOV Rdst, Rsrc       : Rdst = Rsrc
    OP_MOV_RI       = 0x11,     // MOV Rdst, #imm16     : Rdst = zero_extend(imm16)
    OP_MOV_W0       = 0x12,     // MOV.W0 Rdst, #imm16  : Rdst = imm16 (clear, set bits 0-15)
    OP_MOV_W1       = 0x13,     // MOV.W1 Rdst, #imm16  : Rdst[31:16] = imm16
    OP_MOV_W2       = 0x14,     // MOV.W2 Rdst, #imm16  : Rdst[47:32] = imm16
    OP_MOV_W3       = 0x15,     // MOV.W3 Rdst, #imm16  : Rdst[63:48] = imm16
    OP_XCHG         = 0x16,     // XCHG Rdst, Rsrc      : Swap Rdst <-> Rsrc
    OP_LEA          = 0x17,     // LEA Rdst, [Rsrc+imm8]: Rdst = Rsrc + sign_extend(imm8)

    
    // ARITHMÉTIQUE (0x20 - 0x3F)
    
    OP_ADD_RR       = 0x20,     // ADD Rdst, Rsrc       : Rdst = Rdst + Rsrc
    OP_ADD_RI       = 0x21,     // ADD Rdst, #imm16     : Rdst = Rdst + imm16
    OP_SUB_RR       = 0x22,     // SUB Rdst, Rsrc       : Rdst = Rdst - Rsrc
    OP_SUB_RI       = 0x23,     // SUB Rdst, #imm16     : Rdst = Rdst - imm16
    OP_MUL_RR       = 0x24,     // MUL Rdst, Rsrc       : Rdst = Rdst * Rsrc (unsigned)
    OP_MUL_RI       = 0x25,     // MUL Rdst, #imm16     : Rdst = Rdst * imm16
    OP_IMUL_RR      = 0x26,     // IMUL Rdst, Rsrc      : Rdst = Rdst * Rsrc (signed)
    OP_DIV_RR       = 0x27,     // DIV Rdst, Rsrc       : Rdst = Rdst / Rsrc (unsigned)
    OP_DIV_RI       = 0x28,     // DIV Rdst, #imm16     : Rdst = Rdst / imm16
    OP_IDIV_RR      = 0x29,     // IDIV Rdst, Rsrc      : Rdst = Rdst / Rsrc (signed)
    OP_MOD_RR       = 0x2A,     // MOD Rdst, Rsrc       : Rdst = Rdst % Rsrc (unsigned)
    OP_MOD_RI       = 0x2B,     // MOD Rdst, #imm16     : Rdst = Rdst % imm16
    OP_IMOD_RR      = 0x2C,     // IMOD Rdst, Rsrc      : Rdst = Rdst % Rsrc (signed)
    OP_INC          = 0x2D,     // INC Rdst             : Rdst = Rdst + 1
    OP_DEC          = 0x2E,     // DEC Rdst             : Rdst = Rdst - 1
    OP_NEG          = 0x2F,     // NEG Rdst             : Rdst = -Rdst
    OP_ABS          = 0x30,     // ABS Rdst             : Rdst = |Rdst|

    
    // LOGIQUE BINAIRE (0x40 - 0x5F)
    
    OP_AND_RR       = 0x40,     // AND Rdst, Rsrc       : Rdst = Rdst & Rsrc
    OP_AND_RI       = 0x41,     // AND Rdst, #imm16     : Rdst = Rdst & imm16
    OP_OR_RR        = 0x42,     // OR Rdst, Rsrc        : Rdst = Rdst | Rsrc
    OP_OR_RI        = 0x43,     // OR Rdst, #imm16      : Rdst = Rdst | imm16
    OP_XOR_RR       = 0x44,     // XOR Rdst, Rsrc       : Rdst = Rdst ^ Rsrc
    OP_XOR_RI       = 0x45,     // XOR Rdst, #imm16     : Rdst = Rdst ^ imm16
    OP_NOT          = 0x46,     // NOT Rdst             : Rdst = ~Rdst
    OP_SHL_RR       = 0x48,     // SHL Rdst, Rsrc       : Rdst = Rdst << (Rsrc & 63)
    OP_SHL_RI       = 0x49,     // SHL Rdst, #imm8      : Rdst = Rdst << imm8
    OP_SHR_RR       = 0x4A,     // SHR Rdst, Rsrc       : Rdst = Rdst >> Rsrc (logical)
    OP_SHR_RI       = 0x4B,     // SHR Rdst, #imm8      : Rdst = Rdst >> imm8 (logical)
    OP_SAR_RR       = 0x4C,     // SAR Rdst, Rsrc       : Rdst = Rdst >> Rsrc (arithmetic)
    OP_SAR_RI       = 0x4D,     // SAR Rdst, #imm8      : Rdst = Rdst >> imm8 (arithmetic)
    OP_ROL_RR       = 0x4E,     // ROL Rdst, Rsrc       : Rotate left
    OP_ROL_RI       = 0x4F,     // ROL Rdst, #imm8      : Rotate left
    OP_ROR_RR       = 0x50,     // ROR Rdst, Rsrc       : Rotate right
    OP_ROR_RI       = 0x51,     // ROR Rdst, #imm8      : Rotate right
    OP_BSWAP        = 0x52,     // BSWAP Rdst           : Byte swap (endian)
    OP_CTZ          = 0x53,     // CTZ Rdst, Rsrc       : Count trailing zeros
    OP_CLZ          = 0x54,     // CLZ Rdst, Rsrc       : Count leading zeros
    OP_POPCNT       = 0x55,     // POPCNT Rdst, Rsrc    : Population count

    
    // COMPARAISON & FLAGS (0x60 - 0x6F)
    
    OP_CMP_RR       = 0x60,     // CMP Rdst, Rsrc       : FLAGS = Rdst - Rsrc
    OP_CMP_RI       = 0x61,     // CMP Rdst, #imm16     : FLAGS = Rdst - imm16
    OP_TEST_RR      = 0x62,     // TEST Rdst, Rsrc      : FLAGS = Rdst & Rsrc
    OP_TEST_RI      = 0x63,     // TEST Rdst, #imm16    : FLAGS = Rdst & imm16
    OP_CLC          = 0x64,     // CLC                  : Clear carry flag
    OP_STC          = 0x65,     // STC                  : Set carry flag
    OP_CMC          = 0x66,     // CMC                  : Complement carry flag

    
    // MÉMOIRE (0x70 - 0x7F)
    
    OP_LOAD64       = 0x70,     // LOAD64 Rdst, Rsrc    : Rdst = MEM64[Rsrc]
    OP_LOAD32       = 0x71,     // LOAD32 Rdst, Rsrc    : Rdst = zext(MEM32[Rsrc])
    OP_LOAD16       = 0x72,     // LOAD16 Rdst, Rsrc    : Rdst = zext(MEM16[Rsrc])
    OP_LOAD8        = 0x73,     // LOAD8 Rdst, Rsrc     : Rdst = zext(MEM8[Rsrc])
    OP_LOAD32S      = 0x74,     // LOAD32S Rdst, Rsrc   : Rdst = sext(MEM32[Rsrc])
    OP_LOAD16S      = 0x75,     // LOAD16S Rdst, Rsrc   : Rdst = sext(MEM16[Rsrc])
    OP_LOAD8S       = 0x76,     // LOAD8S Rdst, Rsrc    : Rdst = sext(MEM8[Rsrc])
    OP_STORE64      = 0x77,     // STORE64 Rdst, Rsrc   : MEM64[Rdst] = Rsrc
    OP_STORE32      = 0x78,     // STORE32 Rdst, Rsrc   : MEM32[Rdst] = Rsrc[31:0]
    OP_STORE16      = 0x79,     // STORE16 Rdst, Rsrc   : MEM16[Rdst] = Rsrc[15:0]
    OP_STORE8       = 0x7A,     // STORE8 Rdst, Rsrc    : MEM8[Rdst] = Rsrc[7:0]
    OP_MEMCPY       = 0x7B,     // MEMCPY Rdst, Rsrc, Rlen
    OP_MEMSET       = 0x7C,     // MEMSET Rdst, Rsrc, Rlen
    OP_MEMCMP       = 0x7D,     // MEMCMP Rdst, Rsrc, Rlen : R0 = result

    
    // STACK (0x80 - 0x8F)
    
    OP_PUSH         = 0x80,     // PUSH Rsrc            : SP -= 8, MEM64[SP] = Rsrc
    OP_POP          = 0x81,     // POP Rdst             : Rdst = MEM64[SP], SP += 8
    OP_PUSH_RI      = 0x82,     // PUSH #imm16          : SP -= 8, MEM64[SP] = imm16
    OP_PUSHF        = 0x83,     // PUSHF                : Push FLAGS
    OP_POPF         = 0x84,     // POPF                 : Pop FLAGS
    OP_DUP          = 0x85,     // DUP                  : Duplicate top of stack
    OP_DROP         = 0x86,     // DROP                 : SP += 8
    OP_SWAP_STK     = 0x87,     // SWAP                 : Swap top two stack elements

    
    // FLUX DE CONTRÔLE - JUMPS (0x90 - 0x9F)
    
    OP_JMP          = 0x90,     // JMP #addr16          : IP = addr16
    OP_JMP_R        = 0x91,     // JMP Rsrc             : IP = Rsrc
    OP_JZ           = 0x92,     // JZ #addr16           : if ZF, IP = addr16
    OP_JNZ          = 0x93,     // JNZ #addr16          : if !ZF, IP = addr16
    OP_JG           = 0x94,     // JG #addr16           : if greater (signed)
    OP_JGE          = 0x95,     // JGE #addr16          : if greater or equal
    OP_JL           = 0x96,     // JL #addr16           : if less (signed)
    OP_JLE          = 0x97,     // JLE #addr16          : if less or equal
    OP_JA           = 0x98,     // JA #addr16           : if above (unsigned)
    OP_JAE          = 0x99,     // JAE #addr16          : if above or equal
    OP_JB           = 0x9A,     // JB #addr16           : if below (unsigned)
    OP_JBE          = 0x9B,     // JBE #addr16          : if below or equal
    OP_JS           = 0x9C,     // JS #addr16           : if sign
    OP_JNS          = 0x9D,     // JNS #addr16          : if not sign
    OP_JO           = 0x9E,     // JO #addr16           : if overflow
    OP_JNO          = 0x9F,     // JNO #addr16          : if not overflow

    
    // FLUX DE CONTRÔLE - CALLS/RET/LOOPS (0xA0 - 0xAF)
    
    OP_CALL         = 0xA0,     // CALL #addr16         : PUSH IP+4, IP = addr16
    OP_CALL_R       = 0xA1,     // CALL Rsrc            : PUSH IP+4, IP = Rsrc
    OP_RET          = 0xA2,     // RET                  : POP IP
    OP_RET_RI       = 0xA3,     // RET #imm16           : POP IP, SP += imm16
    OP_LOOP         = 0xA4,     // LOOP Rcnt, #addr16   : Rcnt--, if !=0: jump
    OP_LOOPZ        = 0xA5,     // LOOPZ Rcnt, #addr16
    OP_LOOPNZ       = 0xA6,     // LOOPNZ Rcnt, #addr16
    OP_ENTER        = 0xA7,     // ENTER #imm16         : PUSH BP, BP=SP, SP -= imm16
    OP_LEAVE        = 0xA8,     // LEAVE                : SP=BP, POP BP

    
    // RÉSEAU (0xB0 - 0xBF)
    
    OP_NET_INIT     = 0xB0,     // NET_INIT             : R0 = status
    OP_NET_SOCKET   = 0xB1,     // NET_SOCKET           : R0 = socket fd
    OP_NET_CONNECT  = 0xB2,     // NET_CONNECT Rs,Rip,Rp
    OP_NET_BIND     = 0xB3,     // NET_BIND Rsock, Rport
    OP_NET_LISTEN   = 0xB4,     // NET_LISTEN Rsock, Rbacklog
    OP_NET_ACCEPT   = 0xB5,     // NET_ACCEPT Rsock     : R0 = new socket
    OP_NET_SEND     = 0xB6,     // NET_SEND Rs,Rbuf,Rlen: R0 = bytes sent
    OP_NET_RECV     = 0xB7,     // NET_RECV Rs,Rbuf,Rlen: R0 = bytes recv
    OP_NET_CLOSE    = 0xB8,     // NET_CLOSE Rsock
    OP_NET_CLEANUP  = 0xB9,     // NET_CLEANUP
    OP_NET_SETSOPT  = 0xBA,     // NET_SETSOPT Rs,Ro,Rv
    OP_NET_SELECT   = 0xBB,     // NET_SELECT Rs,Rt

    
    // CRYPTO (0xC0 - 0xCF)
    
    OP_CRYPTO_RAND  = 0xC0,     // CRYPTO_RAND Rbuf, Rlen
    OP_CRYPTO_HASH  = 0xC1,     // CRYPTO_HASH Rout,Rin,Rlen
    OP_CRYPTO_HMAC  = 0xC2,     // CRYPTO_HMAC Ro,Ri,Rl,Rk
    OP_CRYPTO_KEYGEN= 0xC3,     // CRYPTO_KEYGEN Rpub, Rpriv
    OP_CRYPTO_DH    = 0xC4,     // CRYPTO_DH Rshared,Rpriv,Rpub
    OP_CRYPTO_ENC   = 0xC5,     // CRYPTO_ENC Ro,Ri,Rl,Rk,Rn
    OP_CRYPTO_DEC   = 0xC6,     // CRYPTO_DEC Ro,Ri,Rl,Rk,Rn
    OP_CRYPTO_VERIFY= 0xC7,     // CRYPTO_VERIFY Ra,Rb,Rlen : R0 = result
    OP_CRYPTO_KDF   = 0xC8,     // CRYPTO_KDF Ro,Ri,Rl,Rs
    OP_CRYPTO_SIGN  = 0xC9,     // CRYPTO_SIGN Rsig,Rm,Rk
    OP_CRYPTO_VSIGN = 0xCA,     // CRYPTO_VSIGN Rsig,Rm,Rpk : R0 = result

    
    // SYSTÈME (0xD0 - 0xDF)
    
    OP_SYSCALL      = 0xD0,     // SYSCALL Rnum
    OP_DEBUG        = 0xD1,     // DEBUG Rsrc           : Print register
    OP_GETTIME      = 0xD2,     // GETTIME              : R0 = timestamp ms
    OP_SLEEP        = 0xD3,     // SLEEP Rms
    OP_RDTSC        = 0xD4,     // RDTSC                : R0 = counter
    OP_CPUID        = 0xD5,     // CPUID                : R0 = VM capabilities

    
    // OPCODES DUPLIQUÉS POUR OBFUSCATION (0xE0 - 0xEF)
    
    OP_ADD_RR_V2    = 0xE0,
    OP_ADD_RR_V3    = 0xE1,
    OP_SUB_RR_V2    = 0xE2,
    OP_MOV_RR_V2    = 0xE3,
    OP_MOV_RR_V3    = 0xE4,
    OP_XOR_RR_V2    = 0xE5,
    OP_JMP_V2       = 0xE6,
    OP_JZ_V2        = 0xE7,
    OP_JNZ_V2       = 0xE8,
    OP_CALL_V2      = 0xE9,
    OP_RET_V2       = 0xEA,
    OP_PUSH_V2      = 0xEB,
    OP_POP_V2       = 0xEC,
    OP_LOAD64_V2    = 0xED,
    OP_STORE64_V2   = 0xEE,
    OP_AND_RR_V2    = 0xEF,

    
    // NOP VARIANTES & PIÈGES (0xF0 - 0xFF)
    
    OP_NOP_V2       = 0xF0,
    OP_NOP_V3       = 0xF1,
    OP_NOP_V4       = 0xF2,
    OP_NOP_V5       = 0xF3,
    OP_NOP_V6       = 0xF4,
    OP_NOP_V7       = 0xF5,
    // 0xF6 - 0xFC réservés
    OP_TRAP         = 0xFD,     // Halt with error
    OP_UD           = 0xFE,     // Undefined instruction
    OP_INVALID      = 0xFF,     // Invalid opcode

    OP_COUNT        = 256
} opcode_t;


// MACROS D'EXTRACTION D'INSTRUCTION
// Format: [opcode:8][dst:8][src1:8][src2:8] = 32 bits little-endian


#define INSTR_OPCODE(instr)     ((uint8_t)((instr) & 0xFF))
#define INSTR_DST(instr)        ((uint8_t)(((instr) >> 8) & 0xFF))
#define INSTR_SRC1(instr)       ((uint8_t)(((instr) >> 16) & 0xFF))
#define INSTR_SRC2(instr)       ((uint8_t)(((instr) >> 24) & 0xFF))

// Immédiat 16-bit (src1 = low, src2 = high)
#define INSTR_IMM16(instr)      ((uint16_t)(INSTR_SRC1(instr) | ((uint16_t)INSTR_SRC2(instr) << 8)))

// Immédiat 8-bit
#define INSTR_IMM8(instr)       ((int8_t)INSTR_SRC1(instr))
#define INSTR_IMM8U(instr)      ((uint8_t)INSTR_SRC1(instr))

// Construction d'instruction
#define MAKE_INSTR(op, dst, s1, s2) \
    ((uint32_t)(op) | ((uint32_t)(dst) << 8) | ((uint32_t)(s1) << 16) | ((uint32_t)(s2) << 24))

#define MAKE_INSTR_RI(op, dst, imm16) \
    MAKE_INSTR(op, dst, (imm16) & 0xFF, ((imm16) >> 8) & 0xFF)


// VALIDATION


#define IS_VALID_GP_REG(r)      ((r) < REG_COUNT)
#define IS_VALID_REG(r)         ((r) < REG_TOTAL)
#define IS_VALID_OPCODE(op)     ((op) < OP_COUNT && (op) != OP_INVALID)


// NOMS (DEBUG)


#ifdef VM_DEBUG

static const char* const opcode_names[OP_COUNT] = {
    [OP_NOP]        = "NOP",
    [OP_HALT]       = "HALT",
    [OP_BREAK]      = "BREAK",
    [OP_MOV_RR]     = "MOV",
    [OP_MOV_RI]     = "MOV",
    [OP_MOV_W0]     = "MOV.W0",
    [OP_MOV_W1]     = "MOV.W1",
    [OP_MOV_W2]     = "MOV.W2",
    [OP_MOV_W3]     = "MOV.W3",
    [OP_XCHG]       = "XCHG",
    [OP_LEA]        = "LEA",
    [OP_ADD_RR]     = "ADD",
    [OP_ADD_RI]     = "ADD",
    [OP_SUB_RR]     = "SUB",
    [OP_SUB_RI]     = "SUB",
    [OP_MUL_RR]     = "MUL",
    [OP_MUL_RI]     = "MUL",
    [OP_IMUL_RR]    = "IMUL",
    [OP_DIV_RR]     = "DIV",
    [OP_DIV_RI]     = "DIV",
    [OP_IDIV_RR]    = "IDIV",
    [OP_MOD_RR]     = "MOD",
    [OP_MOD_RI]     = "MOD",
    [OP_IMOD_RR]    = "IMOD",
    [OP_INC]        = "INC",
    [OP_DEC]        = "DEC",
    [OP_NEG]        = "NEG",
    [OP_ABS]        = "ABS",
    [OP_AND_RR]     = "AND",
    [OP_AND_RI]     = "AND",
    [OP_OR_RR]      = "OR",
    [OP_OR_RI]      = "OR",
    [OP_XOR_RR]     = "XOR",
    [OP_XOR_RI]     = "XOR",
    [OP_NOT]        = "NOT",
    [OP_SHL_RR]     = "SHL",
    [OP_SHL_RI]     = "SHL",
    [OP_SHR_RR]     = "SHR",
    [OP_SHR_RI]     = "SHR",
    [OP_SAR_RR]     = "SAR",
    [OP_SAR_RI]     = "SAR",
    [OP_ROL_RR]     = "ROL",
    [OP_ROL_RI]     = "ROL",
    [OP_ROR_RR]     = "ROR",
    [OP_ROR_RI]     = "ROR",
    [OP_BSWAP]      = "BSWAP",
    [OP_CTZ]        = "CTZ",
    [OP_CLZ]        = "CLZ",
    [OP_POPCNT]     = "POPCNT",
    [OP_CMP_RR]     = "CMP",
    [OP_CMP_RI]     = "CMP",
    [OP_TEST_RR]    = "TEST",
    [OP_TEST_RI]    = "TEST",
    [OP_CLC]        = "CLC",
    [OP_STC]        = "STC",
    [OP_CMC]        = "CMC",
    [OP_LOAD64]     = "LOAD64",
    [OP_LOAD32]     = "LOAD32",
    [OP_LOAD16]     = "LOAD16",
    [OP_LOAD8]      = "LOAD8",
    [OP_LOAD32S]    = "LOAD32S",
    [OP_LOAD16S]    = "LOAD16S",
    [OP_LOAD8S]     = "LOAD8S",
    [OP_STORE64]    = "STORE64",
    [OP_STORE32]    = "STORE32",
    [OP_STORE16]    = "STORE16",
    [OP_STORE8]     = "STORE8",
    [OP_MEMCPY]     = "MEMCPY",
    [OP_MEMSET]     = "MEMSET",
    [OP_MEMCMP]     = "MEMCMP",
    [OP_PUSH]       = "PUSH",
    [OP_POP]        = "POP",
    [OP_PUSH_RI]    = "PUSH",
    [OP_PUSHF]      = "PUSHF",
    [OP_POPF]       = "POPF",
    [OP_DUP]        = "DUP",
    [OP_DROP]       = "DROP",
    [OP_SWAP_STK]   = "SWAP",
    [OP_JMP]        = "JMP",
    [OP_JMP_R]      = "JMP",
    [OP_JZ]         = "JZ",
    [OP_JNZ]        = "JNZ",
    [OP_JG]         = "JG",
    [OP_JGE]        = "JGE",
    [OP_JL]         = "JL",
    [OP_JLE]        = "JLE",
    [OP_JA]         = "JA",
    [OP_JAE]        = "JAE",
    [OP_JB]         = "JB",
    [OP_JBE]        = "JBE",
    [OP_JS]         = "JS",
    [OP_JNS]        = "JNS",
    [OP_JO]         = "JO",
    [OP_JNO]        = "JNO",
    [OP_CALL]       = "CALL",
    [OP_CALL_R]     = "CALL",
    [OP_RET]        = "RET",
    [OP_RET_RI]     = "RET",
    [OP_LOOP]       = "LOOP",
    [OP_LOOPZ]      = "LOOPZ",
    [OP_LOOPNZ]     = "LOOPNZ",
    [OP_ENTER]      = "ENTER",
    [OP_LEAVE]      = "LEAVE",
    [OP_NET_INIT]   = "NET_INIT",
    [OP_NET_SOCKET] = "NET_SOCKET",
    [OP_NET_CONNECT]= "NET_CONNECT",
    [OP_NET_BIND]   = "NET_BIND",
    [OP_NET_LISTEN] = "NET_LISTEN",
    [OP_NET_ACCEPT] = "NET_ACCEPT",
    [OP_NET_SEND]   = "NET_SEND",
    [OP_NET_RECV]   = "NET_RECV",
    [OP_NET_CLOSE]  = "NET_CLOSE",
    [OP_NET_CLEANUP]= "NET_CLEANUP",
    [OP_NET_SETSOPT]= "NET_SETSOPT",
    [OP_NET_SELECT] = "NET_SELECT",
    [OP_CRYPTO_RAND]  = "CRYPTO_RAND",
    [OP_CRYPTO_HASH]  = "CRYPTO_HASH",
    [OP_CRYPTO_HMAC]  = "CRYPTO_HMAC",
    [OP_CRYPTO_KEYGEN]= "CRYPTO_KEYGEN",
    [OP_CRYPTO_DH]    = "CRYPTO_DH",
    [OP_CRYPTO_ENC]   = "CRYPTO_ENC",
    [OP_CRYPTO_DEC]   = "CRYPTO_DEC",
    [OP_CRYPTO_VERIFY]= "CRYPTO_VERIFY",
    [OP_CRYPTO_KDF]   = "CRYPTO_KDF",
    [OP_CRYPTO_SIGN]  = "CRYPTO_SIGN",
    [OP_CRYPTO_VSIGN] = "CRYPTO_VSIGN",
    [OP_SYSCALL]    = "SYSCALL",
    [OP_DEBUG]      = "DEBUG",
    [OP_GETTIME]    = "GETTIME",
    [OP_SLEEP]      = "SLEEP",
    [OP_RDTSC]      = "RDTSC",
    [OP_CPUID]      = "CPUID",
    [OP_ADD_RR_V2]  = "ADD",
    [OP_ADD_RR_V3]  = "ADD",
    [OP_SUB_RR_V2]  = "SUB",
    [OP_MOV_RR_V2]  = "MOV",
    [OP_MOV_RR_V3]  = "MOV",
    [OP_XOR_RR_V2]  = "XOR",
    [OP_JMP_V2]     = "JMP",
    [OP_JZ_V2]      = "JZ",
    [OP_JNZ_V2]     = "JNZ",
    [OP_CALL_V2]    = "CALL",
    [OP_RET_V2]     = "RET",
    [OP_PUSH_V2]    = "PUSH",
    [OP_POP_V2]     = "POP",
    [OP_LOAD64_V2]  = "LOAD64",
    [OP_STORE64_V2] = "STORE64",
    [OP_AND_RR_V2]  = "AND",
    [OP_NOP_V2]     = "NOP",
    [OP_NOP_V3]     = "NOP",
    [OP_NOP_V4]     = "NOP",
    [OP_NOP_V5]     = "NOP",
    [OP_NOP_V6]     = "NOP",
    [OP_NOP_V7]     = "NOP",
    [OP_TRAP]       = "TRAP",
    [OP_UD]         = "UD",
    [OP_INVALID]    = "INVALID",
};

static const char* const reg_names[REG_TOTAL] = {
    "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
    "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15",
    "IP", "SP", "FLAGS"
};

#endif // VM_DEBUG

#endif // OPCODES_H