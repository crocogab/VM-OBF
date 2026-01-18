#!/usr/bin/env python3
import random

INSTRUCTIONS = {
    "PUSH": (0x01, 1),   # (opcode, nb_arguments)
    "POP":  (0x02, 0),
    "ADD":  (0x03, 0),
    "SUB":  (0x04, 0),
    "CMP":  (0x05, 0),
    "JMP":  (0x06, 1),
    "JEQ":  (0x07, 1),
    "JNE":  (0x08, 1),
    "LOAD": (0x09, 1),
    "STORE":(0x0A, 1),
    "DUP":  (0x0B, 0),
    "SWAP": (0x0C, 0),
    "HALT": (0xFF, 0),
}


DEAD_CODE_SEQUENCES = [
    ["PUSH 42", "POP"],
    ["DUP", "POP"],
    ["SWAP", "SWAP"],
    ["PUSH 0", "ADD"],
    ["PUSH 1", "PUSH 1", "SUB", "POP"],
    ["PUSH 0","PUSH 0","ADD","DUP","POP","PUSH 0","ADD"]
    
]

def insert_dead_code(lines, probability=0.3):
    """
    Insère du dead code aléatoirement entre les instructions.
    probability : chance d'insérer du dead code après chaque instruction
    """
    result = []
    
    for line in lines:
        result.append(line)
        
        stripped = line.strip()
        
        # Ne pas insérer après labels, commentaires ou lignes vides
        if not stripped or stripped.startswith(";") or stripped.endswith(":"):
            continue
        
        # Insertion aléatoire
        if random.random() < probability:
            sequence = random.choice(DEAD_CODE_SEQUENCES)
            for dead_instr in sequence:
                result.append(f"    {dead_instr}  ; dead code")
    
    return result

def encrypt_bytecode(bytecode, key=0x37):
    encrypted = []
    for b in bytecode:
        encrypted.append(b ^ key)
        key = (key + 7) & 0xFF
    return encrypted

def get_random_opcode(base_opcode):
    """Retourne un alias aléatoire de l'opcode."""
    if base_opcode == 0xFF:  # HALT est spécial
        return random.choice([0xFF, 0xFE, 0xFD, 0xFC])
    else:
        return base_opcode + random.choice([0x00, 0x10, 0x20, 0x30])

def pass1(lines):
    """
    Première passe : collecter les labels et leurs adresses.
    Retourne : dictionnaire {nom_label: adresse}
    """
    labels = {}
    address = 0
    
    for line in lines:
        line = line.strip()
        
        # Ignorer lignes vides et commentaires
        if not line or line.startswith(";"):
            continue
        
        # C'est un label ?
        if line.endswith(":"):
            label_name = line[:-1]  # Enlever le ":"
            labels[label_name] = address
            # Le label ne génère pas de bytecode
            continue
        
        # C'est une instruction
        parts = line.split()
        instr = parts[0].upper()
        
        if instr not in INSTRUCTIONS:
            raise ValueError(f"Instruction inconnue : {instr}")
        
        opcode, nb_args = INSTRUCTIONS[instr]
        address += 1 + nb_args  # opcode + arguments
    
    return labels


def pass2(lines, labels):
    """
    Deuxième passe : générer le bytecode.
    Retourne : liste d'entiers (le bytecode)
    """
    bytecode = []
    
    for line in lines:
        line = line.strip()
        
        # Ignorer lignes vides, commentaires et labels
        if not line or line.startswith(";") or line.endswith(":"):
            continue
        
        parts = line.split()
        instr = parts[0].upper()
        opcode, nb_args = INSTRUCTIONS[instr]
        
        bytecode.append(get_random_opcode(opcode)) 
        
        # Traiter l'argument si présent
        if nb_args == 1:
            arg = parts[1]
            
            if arg in labels:
                # c'est un label
                bytecode.append(labels[arg])
            else:
                # ce n'est pas un bytecode -> soit un PUSH / JMP/ JEQ / JNE /LOAD STORE -> dans tous les cas on ajoute juste bytecode
                bytecode.append(int(arg))
                
    
    return bytecode


def generate_c_array(bytecode):
    encrypted = encrypt_bytecode(bytecode)
    elements = ", ".join(f"0x{b:02X}" for b in encrypted)
    return f"__uint64_t bytecode[] = {{\n    {elements}\n}};"


def assemble(filename):
    """Assemble un fichier .asm en bytecode."""
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    lines=insert_dead_code(lines,0.3)
    labels = pass1(lines)
    bytecode = pass2(lines, labels)
    
    return bytecode, labels


def main():
    import sys
    
    if len(sys.argv) != 2:
        print("Usage: python assembler.py <fichier.asm>")
        sys.exit(1)
    
    filename = sys.argv[1]
    bytecode, labels = assemble(filename)
    
    print("; Labels trouvés :")
    for name, addr in labels.items():
        print(f";   {name} = {addr}")
    
    print()
    print(generate_c_array(bytecode))
    print()
    print(f"; Taille : {len(bytecode)} mots")


if __name__ == "__main__":
    main()