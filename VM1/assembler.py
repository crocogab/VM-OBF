#!/usr/bin/env python3
import sys
import random

INITIAL_KEY = 0x9B

SIZES = {
    'SETR': 3,
    'ADDR': 4,
    'PRINTR': 2,
    'PRINTC': 2,
    'LOAD': 3,
    'STORE': 3,
    'CMP': 3,
    'JMP': 3,
    'JEQ': 3,
    'JNE': 3,
    'HALT': 1,
}

def generate_opcodes():
    values = list(range(0x10, 0xF0))
    random.shuffle(values)
    names = ['SETR', 'ADDR', 'PRINTR', 'LOAD', 'STORE', 'CMP', 'JMP', 'JEQ', 'JNE','PRINTC']
    opcodes = {}
    for i, name in enumerate(names):
        opcodes[name] = values[i]
    opcodes['HALT'] = 0xFF
    return opcodes

def encrypt_rolling(bytecode, initial_key):
    encrypted = []
    key = initial_key
    for b in bytecode:
        enc = (b ^ key) & 0xFF
        encrypted.append(enc)
        key = (key ^ b) & 0xFF  # Clé évolue avec octet EN CLAIR
    return encrypted

def assemble(filename, opcodes):
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

        bytecode.append(opcodes[instr])

        if instr == 'HALT':
            pass
        elif instr == 'SETR':
            reg = int(parts[1][1:])
            val = int(parts[2])
            bytecode.extend([reg, val])
        elif instr == 'PRINTR':
            reg = int(parts[1][1:])
            bytecode.append(reg)
        elif instr == 'PRINTC':
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
            
            #eviter overflow
            val = offset & 0xFFFF
            high_byte = (val >> 8) & 0xFF
            low_byte = val & 0xFF
            bytecode.extend([high_byte, low_byte])
            

        pc += SIZES[instr]

    # Générer opcodes.h
    with open("opcodes.h", "w") as f:
        f.write("// Auto-généré - ne pas modifier\n")
        f.write(f"#define INITIAL_KEY 0x{INITIAL_KEY:02X}\n")
        for name, value in opcodes.items():
            f.write(f"#define {name} 0x{value:02X}\n")

    # Générer bytecode.h
    encrypted = encrypt_rolling(bytecode, INITIAL_KEY)
    with open("bytecode.h", "w") as f:
        f.write(f"#define BYTECODE_SIZE {len(encrypted)}\n")
        
        bytecode_str = ', '.join(f'0x{b:02X}' for b in encrypted)
        f.write(f"__uint8_t bytecode[] = {{{bytecode_str}}};\n")
        

    print("Fichiers générés : opcodes.h, bytecode.h")

if __name__ == '__main__':
    opcodes = generate_opcodes()
    assemble(sys.argv[1], opcodes)