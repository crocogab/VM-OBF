#!/usr/bin/env python3
import sys

KEY = 0x9B  

OPCODES = {
    'SETR': 0x01,
    'ADDR': 0x02,
    'PRINTR': 0x03,
    'LOAD': 0x04,
    'STORE': 0x05,
    'CMP': 0x06,
    'JMP': 0x07,
    'JEQ': 0x08,
    'JNE': 0x09,
    'HALT': 0xFF,
}

SIZES = {
    'SETR': 3,
    'ADDR': 4,
    'PRINTR': 2,
    'LOAD': 3,
    'STORE': 3,
    'CMP': 3,
    'JMP': 2,
    'JEQ': 2,
    'JNE': 2,
    'HALT': 1,
}

def assemble(filename):
    with open(filename, "r") as f:
        lines = [line.strip() for line in f if line.strip()]
    
    # Passe 1 : trouver les labels
    labels = {}
    pc = 0
    for line in lines:
        if line.endswith(':'):
            labels[line[:-1]] = pc
        else:
            instr = line.split()[0]
            pc += SIZES[instr]
    
    print("Labels:", labels)
    
    # Passe 2 : générer le bytecode
    bytecode = []
    pc = 0
    
    for line in lines:
        if line.endswith(':'):
            continue
        
        parts = line.replace(',', ' ').split()
        instr = parts[0]
        
        bytecode.append(OPCODES[instr])
        
        if instr == 'HALT':
            pass
        
        elif instr == 'SETR':
            reg = int(parts[1][1:])
            val = int(parts[2])
            bytecode.extend([reg, val])
        
        elif instr == 'PRINTR':
            reg = int(parts[1][1:])
            bytecode.append(reg)
        
        elif instr == 'ADDR':
            bytecode.extend([
                int(parts[1][1:]),
                int(parts[2][1:]),
                int(parts[3][1:])
            ])
        
        elif instr == 'LOAD':
            reg = int(parts[1][1:])
            addr = int(parts[2])
            bytecode.extend([reg, addr])
        
        elif instr == 'STORE':
            addr = int(parts[1])
            reg = int(parts[2][1:])
            bytecode.extend([addr, reg])
        
        elif instr == 'CMP':
            reg1 = int(parts[1][1:])
            reg2 = int(parts[2][1:])
            bytecode.extend([reg1, reg2])
        
        elif instr in ['JMP', 'JEQ', 'JNE']:
            target_label = parts[1]
            target_pc = labels[target_label]
            offset = target_pc - pc
            bytecode.append(offset)
        
        pc += SIZES[instr]
    
    # Afficher en format C
    encrypted = [(b ^ KEY) & 0xFF for b in bytecode]
    bytecode_str = ', '.join(f'0x{b & 0xFF:02X}' for b in encrypted)
    print(f"__int8_t bytecode[] = {{{bytecode_str}}};")
    

if __name__ == '__main__':
    assemble(sys.argv[1])