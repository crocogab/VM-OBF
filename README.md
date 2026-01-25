# VM-OBF — Machines Virtuelles Obfusquées

Projet d'apprentissage et de recherche sur les techniques d'obfuscation par machine virtuelle. Ce dépôt contient 3 itérations progressives d'une VM custom, chacune introduisant des concepts et protections supplémentaires.

```
┌─────────────────────────────────────────────────────────────────┐
│                         VM-OBF                                  │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│   VM1 (Base)          VM2 (Stack)          VM3 (Registres)      │
│   ┌─────────┐         ┌─────────┐          ┌─────────┐          │
│   │ 4 regs  │         │ Stack   │          │ 16 regs │          │
│   │ XOR key │   ──►   │ Rolling │    ──►   │ Flags   │          │
│   │ Simple  │         │ MBA     │          │ Crypto  │          │
│   └─────────┘         └─────────┘          └─────────┘          │
│                                                                 │
│   Difficulté: ⭐⭐      Difficulté: ⭐⭐⭐⭐    Difficulté: ⭐⭐⭐⭐⭐   │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## Structure du projet

```
VM-OBF/
├── README.md           # Ce fichier
├── VM1/                # VM basique à registres
│   ├── vm.c            # Implémentation VM
│   ├── assembler.py    # Assembleur Python
│   ├── opcodes.h       # Définition des opcodes
│   └── code/           # Programmes exemples
│
├── VM2/                # VM à pile avec obfuscation avancée
│   ├── VMContext.c     # Implémentation VM
│   ├── assembler.py    # Assembleur avec chiffrement
│   ├── reverse_shell.py
│   └── code/           # Programmes exemples
│
└── VM3/                # VM complète type "production"
    ├── src/
    │   ├── main.c
    │   └── vm/
    │       ├── vm.h / vm.c
    │       ├── opcodes.h
    │       ├── dispatcher.c
    │       └── handler.c
    ├── Makefile
    └── README.md       # Documentation technique détaillée
```

---

## Les 3 Machines Virtuelles

### VM1 — Introduction aux concepts

**Architecture :** VM à registres (4 registres R0-R3)

**Caractéristiques :**
- Mémoire linéaire de 256 octets
- Chiffrement XOR avec clé évolutive (rolling key)
- Table de handlers (indirect threading)
- Assembleur Python générant du bytecode C

**Instructions :**
| Opcode | Description |
|--------|-------------|
| `SETR` | Charger une valeur dans un registre |
| `ADDR` | Addition de deux registres |
| `LOAD` | Charger depuis la mémoire |
| `STORE` | Écrire en mémoire |
| `CMP` | Comparer deux registres |
| `JMP/JEQ/JNE` | Sauts (in)conditionnels |
| `PRINTR/PRINTC` | Affichage debug |
| `HALT` | Arrêt de la VM |

**Exemple d'utilisation :**
```bash
cd VM1
python3 assembler.py code/check_password.asm
gcc -o vm vm.c
./vm
```

**Techniques d'obfuscation :**
- Bytecode chiffré avec XOR rolling
- Opcodes randomisés à chaque build
- Indirect threading (table de pointeurs de fonctions)

---

### VM2 — Machine à pile avec obfuscation avancée

**Architecture :** VM à pile (stack-based)

**Caractéristiques :**
- Pile de 2048 éléments (64-bit)
- Mémoire RAM de 256 mots
- Chiffrement rolling XOR avec resynchronisation
- Control Flow Flattening (machine à états)
- Support syscall (execve)

**Instructions :**
| Opcode | Description |
|--------|-------------|
| `PUSH` | Empiler une valeur |
| `POP` | Dépiler |
| `ADD/SUB` | Arithmétique (avec MBA) |
| `CMP` | Comparaison |
| `JMP/JEQ/JNE` | Contrôle de flux |
| `LOAD/STORE` | Accès mémoire |
| `DUP/SWAP` | Manipulation de pile |
| `SYS` | Appel système |
| `HALT` | Arrêt |

**Techniques d'obfuscation :**

1. **Opcode Aliasing** — Chaque instruction a 4 variantes équivalentes :
   ```c
   #define PUSH  0x01
   #define PUSH2 0x11
   #define PUSH3 0x21
   #define PUSH4 0x31
   ```

2. **Control Flow Flattening** — Boucle principale avec machine à états :
   ```c
   while (1) {
       switch (state) {
           case STATE_FETCH: ...
           case STATE_DECODE: ...
           case STATE_EXEC_PUSH: ...
           case STATE_FAKE1: ...  // Faux états
       }
   }
   ```

3. **MBA (Mixed Boolean-Arithmetic)** — Opérations mathématiques obscurcies :
   ```c
   // Addition obfusquée : a + b
   __uint64_t t1 = 3*(a|b);
   __uint64_t t2 = 2*(~(a ^ b));
   __uint64_t t3 = ~(a & b);
   result = t1 + t2 + t3 + 3;
   ```

4. **Prédicats opaques** — Conditions toujours vraies/fausses :
   ```c
   if ((x * (x + 1)) % 2 == 0) {  // Toujours vrai
       state = STATE_DECODE;
   }
   ```

**Exemple d'utilisation :**
```bash
cd VM2
python3 assembler.py code/payload.asm
gcc -o vm VMContext.c
./vm
```

---

### VM3 — Architecture complète de production

**Architecture :** VM à registres 64-bit (16 GPR + 3 spéciaux)

**Caractéristiques :**
- 16 registres généraux (R0-R15) 64-bit
- Registres spéciaux : IP, SP, FLAGS
- Mémoire configurable (défaut 64KB)
- Instructions natives réseau et crypto
- Format d'instruction fixe 32-bit

**Registres :**
```
┌────────────────────────────────────────────┐
│ R0-R15  : Registres généraux 64-bit        │
│ R16/IP  : Instruction Pointer (lecture)    │
│ R17/SP  : Stack Pointer                    │
│ R18/FLAGS : ZF, SF, CF, OF                 │
└────────────────────────────────────────────┘
```

**Jeu d'instructions complet :**

| Catégorie | Opcodes |
|-----------|---------|
| Contrôle | `NOP`, `HALT`, `BREAK` |
| Transfert | `MOV`, `XCHG`, `LEA` |
| Arithmétique | `ADD`, `SUB`, `MUL`, `DIV`, `MOD`, `INC`, `DEC`, `NEG` |
| Logique | `AND`, `OR`, `XOR`, `NOT`, `SHL`, `SHR`, `ROL`, `ROR` |
| Comparaison | `CMP`, `TEST`, flags |
| Mémoire | `LOAD8/16/32/64`, `STORE8/16/32/64`, `MEMCPY`, `MEMSET` |
| Pile | `PUSH`, `POP`, `DUP`, `DROP`, `SWAP` |
| Flux | `JMP`, `Jcc`, `CALL`, `RET`, `LOOP` |
| Réseau | `NET_INIT`, `NET_CONNECT`, `NET_SEND`, `NET_RECV`, ... |
| Crypto | `CRYPTO_ENC`, `CRYPTO_DEC`, `CRYPTO_HASH`, `CRYPTO_KEYGEN`, ... |
| Système | `SYSCALL`, `DEBUG`, `SLEEP`, `RDTSC` |

**Format d'instruction :**
```
┌──────────┬──────────┬──────────┬──────────┐
│ Opcode   │ Dst      │ Src1     │ Src2     │
│ (8 bits) │ (8 bits) │ (8 bits) │ (8 bits) │
└──────────┴──────────┴──────────┴──────────┘
```

**Compilation et exécution :**
```bash
cd VM3
make
./build/vm
```

**Techniques d'obfuscation prévues :**
- Threaded goto dispatcher (computed goto)
- Handlers dupliqués (même opération, code différent)
- Opcode shuffling par build
- MBA dans les handlers
- Dead code insertion
- Chiffrement bytecode (ChaCha20)

---

## Comparatif des versions

| Caractéristique | VM1 | VM2 | VM3 |
|-----------------|-----|-----|-----|
| Architecture | Registres | Pile | Registres |
| Registres | 4 (8-bit) | - | 16 (64-bit) |
| Mémoire | 256 octets | 256 mots | 64 KB |
| Chiffrement | XOR rolling | XOR rolling | ChaCha20 (prévu) |
| Opcode aliasing | Non | Oui (x4) | Oui (x3) |
| Control flow flat. | Non | Oui | Prévu |
| MBA | Non | Oui | Prévu |
| Instructions réseau | Non | Non | Oui |
| Instructions crypto | Non | Non | Oui |
| Assembleur | Python | Python | Python (prévu) |

---

## Techniques d'obfuscation expliquées

### 1. Chiffrement du bytecode
Le bytecode n'est jamais stocké en clair. Chaque octet est XORé avec une clé qui évolue :
```c
key = (key ^ decoded_byte) & 0xFF;  // VM1
key = (key + 7) & 0xFF;              // VM2
```

### 2. Indirect Threading
Au lieu d'un switch géant, utilisation d'une table de pointeurs de fonctions :
```c
typedef void (*handler_t)(VM*);
handler_t handlers[256];
handlers[opcode](&vm);  // Appel indirect
```

### 3. Opcode Aliasing
Plusieurs valeurs de bytecode correspondent à la même instruction :
```c
handlers[ADD]  = handler_add;
handlers[ADD2] = handler_add;  // Même handler
handlers[ADD3] = handler_add;
handlers[ADD4] = handler_add;
```

### 4. Control Flow Flattening
La structure de contrôle est aplatie en machine à états :
```c
while (running) {
    switch (state) {
        case STATE_A: ... state = STATE_C; break;
        case STATE_B: ... state = STATE_A; break;
        case STATE_C: ... state = STATE_B; break;
    }
}
```

### 5. MBA (Mixed Boolean-Arithmetic)
Les opérations simples sont remplacées par des équivalents complexes :
```c
// x + y devient :
(x ^ y) + 2 * (x & y)

// x - y devient :
(x ^ y) - 2 * (~x & y)
```

### 6. Prédicats Opaques
Conditions dont le résultat est connu à la compilation mais pas à l'analyse :
```c
if ((x * (x + 1)) % 2 == 0)  // Toujours vrai (produit de consécutifs)
if ((x | 1) != 0)            // Toujours vrai
```

---

## Objectifs pédagogiques

Ce projet permet d'apprendre :

1. **Conception de VM**
   - Cycle fetch-decode-execute
   - Architectures registres vs pile
   - Gestion de la mémoire et des flags

2. **Techniques d'obfuscation**
   - Chiffrement de code
   - Virtualisation
   - Control flow obfuscation
   - Data obfuscation (MBA)

3. **Programmation C avancée**
   - Pointeurs de fonctions
   - Manipulation de bits
   - Computed goto (GCC)

4. **Reverse engineering**
   - Comprendre les protections
   - Identifier les patterns
   - Développer des outils d'analyse

---

## Prérequis

- GCC ou Clang (C11)
- Python 3.8+
- Make

---

## Ressources

- [Computed goto GCC](https://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html)
- [MBA Obfuscation (USENIX)](https://www.usenix.org/conference/usenixsecurity21/presentation/liu-binbin)
- [Tigress Obfuscator](https://tigress.wtf/)
- [Monocypher (crypto)](https://monocypher.org/)

---

## Licence

Projet éducatif — Usage personnel et CTF uniquement.
