#!/usr/bin/env python3
import random
import sys

# === CONFIGURATION ===
# Mapping des instructions : "NOM": (Opcode de base, Nombre d'arguments IMMÉDIATS)
# Note: SYS prend ses arguments sur la PILE, pas en immédiat, donc nb_args=0
INSTRUCTIONS = {
    "PUSH": (0x01, 1),
    "POP":  (0x02, 0),
    "ADD":  (0x03, 0),
    "SUB":  (0x04, 0),
    "CMP":  (0x05, 0),
    "JMP":  (0x06, 1),
    "JEQ":  (0x07, 1),
    "JNE":  (0x08, 1),
    "LOAD": (0x09, 1),
    "STORE":(0x0A, 1), # STORE <index> (pop value)
    "DUP":  (0x0B, 0),
    "SWAP": (0x0C, 0),
    "SYS":  (0x0D, 0), # <--- NOUVELLE INSTRUCTION SYSCALL
    "HALT": (0xFF, 0),
}

# Séquences de code mort pour l'obfuscation
DEAD_CODE_SEQUENCES = [
    ["PUSH 42", "POP"],
    ["DUP", "POP"],
    ["SWAP", "SWAP"],
    ["PUSH 0", "ADD"],
    ["PUSH 1", "PUSH 1", "SUB", "POP"],
]

def insert_dead_code(lines, probability=0.3):
    """Insère du code inutile pour brouiller les pistes."""
    result = []
    for line in lines:
        result.append(line)
        stripped = line.strip()
        if not stripped or stripped.startswith(";") or stripped.endswith(":"):
            continue
        # Pas de dead code après un saut ou un halt, ça pourrait être inatteignable ou casser la logique
        if stripped.split()[0] in ["JMP", "HALT", "SYS"]:
            continue
            
        if random.random() < probability:
            sequence = random.choice(DEAD_CODE_SEQUENCES)
            result.append(f"    ; --- dead code start ---")
            for dead_instr in sequence:
                result.append(f"    {dead_instr}")
            result.append(f"    ; --- dead code end ---")
    return result

def encrypt_bytecode(bytecode, key=0x37):
    """Chiffrement Rolling XOR"""
    encrypted = []
    for b in bytecode:
        # Simulation d'un uint64 pour le XOR, mais on garde en 64bit
        # Attention: en Python les ints sont illimités, on masque avec 0xFFFFFFFFFFFFFFFF si besoin
        # Mais ici on encrypte mot par mot (qui sont des opcodes ou des valeurs)
        enc_val = b ^ key
        encrypted.append(enc_val)
        key = (key + 7) & 0xFF
    return encrypted

def get_random_opcode(base_opcode):
    """Ajoute du polymorphisme aux opcodes (0x01 -> 0x11, 0x21...)"""
    if base_opcode == 0xFF:
        return random.choice([0xFF, 0xFE, 0xFD, 0xFC])
    if base_opcode == 0x0D: # SYS a aussi des variantes
        return base_opcode + random.choice([0x00, 0x10, 0x20, 0x30])
        
    return base_opcode + random.choice([0x00, 0x10, 0x20, 0x30])

def pass1(lines):
    """Récupère les adresses des labels"""
    labels = {}
    address = 0
    for line in lines:
        line = line.split(";")[0].strip() # Enlever commentaires
        if not line: continue
        
        if line.endswith(":"):
            labels[line[:-1]] = address
            continue
            
        parts = line.split()
        instr = parts[0].upper()
        if instr not in INSTRUCTIONS:
            raise ValueError(f"Instruction inconnue : {instr}")
        
        opcode, nb_args = INSTRUCTIONS[instr]
        address += 1 + nb_args
    return labels

def pass2(lines, labels):
    """Génère le bytecode brut"""
    bytecode = []
    for line in lines:
        line = line.split(";")[0].strip()
        if not line or line.endswith(":"): continue
        
        parts = line.split()
        instr = parts[0].upper()
        opcode, nb_args = INSTRUCTIONS[instr]
        
        # Ajout Opcode
        bytecode.append(get_random_opcode(opcode))
        
        # Ajout Argument (si existe)
        if nb_args == 1:
            arg = parts[1]
            if arg in labels:
                bytecode.append(labels[arg])
            else:
                # Supporte hex (0x...) et int décimal
                try:
                    bytecode.append(int(arg, 0))
                except ValueError:
                     raise ValueError(f"Argument invalide : {arg} à la ligne : {line}")

    return bytecode

def generate_c_array(bytecode):
    encrypted = encrypt_bytecode(bytecode)
    # Formatage propre pour le C
    elements = ", ".join(f"0x{b:016X}" for b in encrypted)
    return f"__uint64_t bytecode[] = {{\n    {elements}\n}};"

def main():
    if len(sys.argv) != 2:
        print("Usage: python assembler.py <fichier.asm>")
        sys.exit(1)
    
    filename = sys.argv[1]
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    # Injection de code mort (désactivable en mettant 0.0)
    lines_obf = insert_dead_code(lines, probability=0.3)
    
    # Assemblage
    labels = pass1(lines_obf)
    bytecode = pass2(lines_obf, labels)
    
    print(f"// Assemblé depuis {filename}")
    print(f"// Taille: {len(bytecode)} mots (uint64)")
    print(generate_c_array(bytecode))

if __name__ == "__main__":
    main()  