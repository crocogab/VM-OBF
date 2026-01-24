# Pingback VM — Outil de Pingback Obfusqué

## Vue d'ensemble

Outil de pingback cross-platform (Windows/Linux) protégé par une VM custom à obfuscation avancée.

```
┌─────────────────────────────────────────────────────────────┐
│                      ARCHITECTURE                           │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   [Assembleur Python] ──► [Bytecode chiffré]                │
│                                 │                           │
│                                 ▼                           │
│   ┌─────────────────────────────────────────────┐           │
│   │              VM Runtime (C)                 │           │
│   │  ┌───────────────────────────────────────┐  │           │
│   │  │  Déchiffrement bytecode               │  │           │
│   │  └───────────────────────────────────────┘  │           │
│   │                    │                        │           │
│   │                    ▼                        │           │
│   │  ┌───────────────────────────────────────┐  │           │
│   │  │  Dispatcher (threaded goto)           │  │           │
│   │  │  + Opcode shuffling                   │  │           │
│   │  └───────────────────────────────────────┘  │           │
│   │                    │                        │           │
│   │                    ▼                        │           │
│   │  ┌───────────────────────────────────────┐  │           │
│   │  │  Handlers (MBA obfuscated)            │  │           │
│   │  │  + Handlers dupliqués                 │  │           │
│   │  │  + Dead code                          │  │           │
│   │  └───────────────────────────────────────┘  │           │
│   │                    │                        │           │
│   │                    ▼                        │           │
│   │  ┌───────────────────────────────────────┐  │           │
│   │  │  Instructions natives                 │  │           │
│   │  │  (NET_CONNECT, NET_SEND, CRYPTO_...)  │  │           │
│   │  └───────────────────────────────────────┘  │           │
│   └─────────────────────────────────────────────┘           │
│                         │                                   │
│                         ▼                                   │
│   ┌─────────────────────────────────────────────┐           │
│   │  Couche native (sockets + monocypher)       │           │
│   └─────────────────────────────────────────────┘           │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## Spécifications techniques

### Registres

| Registre | Alias | Description |
|----------|-------|-------------|
| `R0-R15` | — | Registres généraux 64-bit |
| `R16` | `IP` | Instruction pointer (lecture seule) |
| `R17` | `SP` | Stack pointer |
| `R18` | `FLAGS` | Flags de comparaison |

**FLAGS bits :**
```
Bit 0 : ZF (Zero Flag)
Bit 1 : SF (Sign Flag)
Bit 2 : CF (Carry Flag)
Bit 3 : OF (Overflow Flag)
```

### Mémoire

- Mémoire linéaire unique
- Taille configurable (défaut : 64KB)
- Little-endian
- Stack grandit vers le bas (adresses décroissantes)

```
┌─────────────────┐ 0x00000000
│                 │
│   Code/Data     │
│                 │
├─────────────────┤
│       ▼         │
│   Heap          │
│                 │
│                 │
│   Stack         │
│       ▲         │
├─────────────────┤
│   Reserved      │
└─────────────────┘ MEM_SIZE
```

### Format des instructions

Encodage fixe 4 bytes :

```
┌──────────┬──────────┬──────────┬──────────┐
│ Opcode   │ Dst/Reg  │ Src1/Imm │ Src2/Imm │
│ (1 byte) │ (1 byte) │ (1 byte) │ (1 byte) │
└──────────┴──────────┴──────────┴──────────┘
```

Pour les immédiats 16-bit : `Src1` = low byte, `Src2` = high byte

### Syntaxe assembleur

```asm
; Commentaire
LABEL:
    MOV R0 #42          ; R0 = 42 (immédiat)
    MOV R1 R0           ; R1 = R0 (registre)
    ADD R0 R1           ; R0 = R0 + R1
    LOAD R2 R0          ; R2 = MEM[R0]
    STORE R0 R2         ; MEM[R0] = R2
    CMP R0 R1           ; Compare, set FLAGS
    JZ LABEL            ; Jump if zero
    CALL FUNC           ; Push IP, jump
    RET                 ; Pop IP, jump
    
    ; Instructions natives
    NET_CONNECT R0 R1   ; Connect (IP in R0, port in R1)
    NET_SEND R0 R1 R2   ; Send (sock R0, buf R1, len R2)
    NET_RECV R0 R1 R2   ; Recv (sock R0, buf R1, len R2)
    CRYPTO_ENC R0 R1 R2 ; Encrypt (buf R0, len R1, key R2)
```

---

## Structure du projet

```
pingback-vm/
├── README.md
├── Makefile
│
├── src/
│   ├── main.c                  # Point d'entrée
│   │
│   ├── vm/
│   │   ├── vm.h                # Structures VM (état, config)
│   │   ├── vm.c                # Init, run, cleanup
│   │   ├── opcodes.h           # Enum des opcodes
│   │   ├── dispatcher.c        # Threaded goto dispatcher
│   │   ├── handlers.c          # Implémentation des handlers
│   │   ├── handlers_mba.h      # Macros MBA pour obfuscation
│   │   └── decoder.c           # Déchiffrement bytecode
│   │
│   ├── net/
│   │   ├── net.h               # Interface cross-platform
│   │   ├── net_linux.c         # Implémentation Linux
│   │   └── net_win.c           # Implémentation Windows
│   │
│   └── crypto/
│       ├── monocypher.h        # Lib crypto (vendored)
│       ├── monocypher.c
│       └── crypto_wrapper.c    # Interface simplifiée
│
├── assembler/
│   ├── assembler.py            # Assembleur principal
│   ├── lexer.py                # Tokenization
│   ├── parser.py               # Parsing
│   ├── encoder.py              # Génération bytecode
│   ├── obfuscator.py           # Opcode shuffling, dead code
│   └── encryptor.py            # Chiffrement bytecode final
│
├── programs/
│   └── pingback.asm            # Programme pingback
│
└── tests/
    ├── test_vm.c               # Tests unitaires VM
    └── test_asm/               # Programmes de test
        ├── test_arithmetic.asm
        ├── test_memory.asm
        ├── test_flow.asm
        └── test_native.asm
```

---

## TODO — Étapes de développement

### Phase 1 : VM Core (sans obfuscation)

#### Étape 1.1 — Structures de base
- [x] Créer `src/vm/vm.h`
  - [x] Structure `vm_state_t` (registres, mémoire, IP, SP, FLAGS)
  - [X] Structure `vm_config_t` (taille mémoire, options debug)
  - [X] Constantes (nombre de registres, taille par défaut)
- [X] Créer `src/vm/opcodes.h`
  - [X] Enum de tous les opcodes (sans shuffling pour l'instant)
  - [X] Macros pour extraire dst/src1/src2 d'une instruction

**Validation :** Le code compile, les structures sont cohérentes.

#### Étape 1.2 — Cycle de vie VM
- [ ] Créer `src/vm/vm.c`
  - [ ] `vm_init()` : allocation mémoire, init registres
  - [ ] `vm_load()` : charger bytecode en mémoire
  - [ ] `vm_destroy()` : libération propre
  - [ ] `vm_dump()` : debug, afficher état (registres, stack)

**Validation :** Tu peux init/destroy une VM, charger un buffer, dumper l'état.

#### Étape 1.3 — Dispatcher simple (switch)
- [ ] Créer `src/vm/dispatcher.c`
  - [ ] Boucle fetch-decode-execute avec switch/case
  - [ ] Fetch : lire 4 bytes à IP
  - [ ] Decode : extraire opcode, dst, src1, src2
  - [ ] Execute : switch sur opcode
  - [ ] Incrémenter IP

**Validation :** La VM peut exécuter une séquence de NOP et HALT.

#### Étape 1.4 — Handlers arithmétiques/logiques
- [ ] Dans `src/vm/handlers.c`
  - [ ] `MOV` (reg ← reg, reg ← imm)
  - [ ] `ADD`, `SUB`, `MUL`, `DIV`, `MOD`
  - [ ] `AND`, `OR`, `XOR`, `NOT`
  - [ ] `SHL`, `SHR`
  - [ ] `CMP` (set FLAGS)

**Validation :** Programme test qui calcule (5 + 3) * 2 et stocke dans R0.

#### Étape 1.5 — Handlers mémoire
- [ ] `LOAD` (reg ← MEM[reg])
- [ ] `STORE` (MEM[reg] ← reg)
- [ ] `PUSH` (SP -= 8, MEM[SP] = reg)
- [ ] `POP` (reg = MEM[SP], SP += 8)

**Validation :** Programme test qui push/pop des valeurs.

#### Étape 1.6 — Handlers flux de contrôle
- [ ] `JMP` (IP = addr)
- [ ] `JZ`, `JNZ`, `JG`, `JL`, `JGE`, `JLE` (conditionnels sur FLAGS)
- [ ] `CALL` (push IP+4, JMP)
- [ ] `RET` (pop IP)

**Validation :** Programme avec boucle qui compte de 0 à 10.

---

### Phase 2 : Assembleur Python

#### Étape 2.1 — Lexer
- [ ] Créer `assembler/lexer.py`
  - [ ] Tokenizer : OPCODE, REGISTER, IMMEDIATE, LABEL, NEWLINE, COMMENT
  - [ ] Gérer les formats : `R0`, `#42`, `#0xFF`, `LABEL:`

**Validation :** Tokenize un fichier .asm de test, affiche les tokens.

#### Étape 2.2 — Parser
- [ ] Créer `assembler/parser.py`
  - [ ] Construire liste d'instructions
  - [ ] Première passe : collecter les labels et leurs adresses
  - [ ] Deuxième passe : résoudre les références aux labels

**Validation :** Parse un programme avec labels, affiche la représentation intermédiaire.

#### Étape 2.3 — Encoder
- [ ] Créer `assembler/encoder.py`
  - [ ] Convertir chaque instruction en 4 bytes
  - [ ] Gérer les différents formats (reg-reg, reg-imm, etc.)
  - [ ] Générer le bytecode brut

**Validation :** Assemble un programme simple, la VM l'exécute correctement.

#### Étape 2.4 — Tests aller-retour
- [ ] Créer programmes de test dans `tests/test_asm/`
  - [ ] `test_arithmetic.asm` : toutes les opérations arithmétiques
  - [ ] `test_memory.asm` : load/store/push/pop
  - [ ] `test_flow.asm` : jumps, calls, boucles

**Validation :** Tous les programmes de test produisent les résultats attendus.

---

### Phase 3 : Instructions natives (réseau/crypto)

#### Étape 3.1 — Abstraction réseau
- [ ] Créer `src/net/net.h` (interface)
- [ ] Créer `src/net/net_linux.c`
  - [ ] `net_init()`, `net_cleanup()`
  - [ ] `net_tcp_connect()`, `net_tcp_listen()`, `net_tcp_accept()`
  - [ ] `net_send()`, `net_recv()`, `net_close()`
- [ ] Créer `src/net/net_win.c` (même interface)

**Validation :** Programme C natif qui fait un ping TCP simple.

#### Étape 3.2 — Handlers réseau VM
- [ ] Ajouter opcodes dans `opcodes.h`
  - [ ] `NET_INIT`, `NET_CONNECT`, `NET_LISTEN`, `NET_ACCEPT`
  - [ ] `NET_SEND`, `NET_RECV`, `NET_CLOSE`
- [ ] Implémenter handlers qui appellent la couche native
- [ ] Convention : résultat/erreur dans R0

**Validation :** Programme VM qui se connecte à un serveur de test.

#### Étape 3.3 — Intégration crypto
- [ ] Vendorer `monocypher.c/.h` dans `src/crypto/`
- [ ] Créer `src/crypto/crypto_wrapper.c`
  - [ ] `crypto_keypair()` : génération clés X25519
  - [ ] `crypto_exchange()` : échange de clés
  - [ ] `crypto_encrypt()` : ChaCha20-Poly1305
  - [ ] `crypto_decrypt()`

**Validation :** Test unitaire de chiffrement/déchiffrement.

#### Étape 3.4 — Handlers crypto VM
- [ ] Opcodes : `CRYPTO_KEYGEN`, `CRYPTO_EXCHANGE`, `CRYPTO_ENC`, `CRYPTO_DEC`
- [ ] Les clés/buffers sont en mémoire VM, handlers font le pont

**Validation :** Programme VM qui chiffre et déchiffre un buffer.

---

### Phase 4 : Obfuscation

#### Étape 4.1 — Threaded goto dispatcher
- [ ] Refactorer `dispatcher.c`
  - [ ] Remplacer switch par computed goto (`&&label`)
  - [ ] Table de labels : `void* dispatch_table[]`
  - [ ] Chaque handler termine par `goto *dispatch_table[next_opcode]`

**Validation :** Même comportement, mais dispatcher plus rapide et obfusqué.

#### Étape 4.2 — Handlers dupliqués
- [ ] Modifier `opcodes.h`
  - [ ] Plusieurs opcodes pour la même opération (ex: ADD1, ADD2, ADD3)
- [ ] Modifier `dispatcher.c`
  - [ ] Tous pointent vers le même handler (ou des variantes)
- [ ] Modifier `assembler/encoder.py`
  - [ ] Choisir aléatoirement parmi les opcodes équivalents

**Validation :** Deux assemblages du même code donnent des bytecodes différents.

#### Étape 4.3 — Opcode shuffling
- [ ] Créer `assembler/obfuscator.py`
  - [ ] Générer une permutation aléatoire des opcodes
  - [ ] Sauvegarder la table de mapping
- [ ] Modifier `assembler/encoder.py`
  - [ ] Appliquer le mapping lors de l'encodage
- [ ] Générer `src/vm/opcode_table.h`
  - [ ] Table inverse pour le dispatcher
  - [ ] Régénérée à chaque build

**Validation :** Chaque build a une table d'opcodes différente.

#### Étape 4.4 — MBA dans les handlers
- [ ] Créer `src/vm/handlers_mba.h`
  - [ ] Macros MBA pour opérations courantes
  - [ ] Ex: `MBA_ADD(a,b)` → expression équivalente obfusquée
  - [ ] `(a ^ b) + 2 * (a & b)` au lieu de `a + b`
- [ ] Appliquer dans les handlers arithmétiques

**Validation :** Résultats identiques, code désassemblé incompréhensible.

#### Étape 4.5 — Dead code insertion
- [ ] Modifier `assembler/obfuscator.py`
  - [ ] Insérer des instructions sans effet entre les vraies
  - [ ] Faux jumps avec prédicats opaques
  - [ ] Opérations qui s'annulent
- [ ] Configurable (niveau d'obfuscation)

**Validation :** Bytecode plus gros, même résultat, CFG complexifié.

#### Étape 4.6 — Chiffrement bytecode
- [ ] Créer `assembler/encryptor.py`
  - [ ] Chiffrer le bytecode final (ChaCha20)
  - [ ] Générer la clé (hardcodée ou dérivée)
- [ ] Créer `src/vm/decoder.c`
  - [ ] Déchiffrer au chargement
  - [ ] Option : déchiffrement par blocs à la demande

**Validation :** Bytecode illisible sans la clé, VM l'exécute correctement.

---

### Phase 5 : Programme Pingback

#### Étape 5.1 — Protocole
- [ ] Définir le protocole dans `docs/PROTOCOL.md`
  - [ ] Format des messages
  - [ ] Handshake (échange de clés)
  - [ ] Types : PING, PONG, ERROR

#### Étape 5.2 — Écrire pingback.asm
- [ ] `programs/pingback.asm`
  - [ ] Init réseau
  - [ ] Attendre connexion entrante OU se connecter (selon mode)
  - [ ] Handshake crypto
  - [ ] Boucle : recevoir PING, répondre PONG chiffré
  - [ ] Cleanup

**Validation :** Communication fonctionnelle entre deux instances.

#### Étape 5.3 — Tests end-to-end
- [ ] Test local (deux terminaux)
- [ ] Test cross-platform (Linux ↔ Windows)
- [ ] Test avec Wireshark (vérifier que c'est chiffré)

---

### Phase 6 : Hardening & Polish

#### Étape 6.1 — Anti-debug (optionnel)
- [ ] Détection debugger basique
- [ ] Timing checks
- [ ] Intégrité du bytecode

#### Étape 6.2 — Build system
- [ ] Makefile robuste
  - [ ] `make` : build standard
  - [ ] `make release` : avec obfuscation max
  - [ ] `make test` : lance les tests
- [ ] Cross-compilation Windows depuis Linux (MinGW)

#### Étape 6.3 — Documentation
- [ ] Documenter l'ISA complet
- [ ] Guide d'utilisation de l'assembleur
- [ ] Notes sur les techniques d'obfuscation utilisées

---

## Conventions de code

### C
- C11 standard
- Préfixe `vm_` pour les fonctions VM
- Préfixe `net_` pour les fonctions réseau
- Pas de variables globales (tout dans `vm_state_t`)
- `snake_case` pour fonctions et variables
- `UPPER_CASE` pour constantes et macros

### Python (assembleur)
- Python 3.8+
- Type hints
- Un fichier = un module cohérent
- `black` pour le formatage

---

## Ressources

- [Computed goto GCC](https://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html)
- [Monocypher](https://monocypher.org/)
- [MBA Obfuscation paper](https://www.usenix.org/conference/usenixsecurity21/presentation/liu-binbin)
- [Tigress (inspiration)](https://tigress.wtf/)

---

## Journal de progression

| Date | Étape | Status | Notes |
|------|-------|--------|-------|
| | 1.1 | ⬜ | |
| | 1.2 | ⬜ | |
| | ... | | |

---

**Bon courage ! Envoie-moi ton code à chaque étape pour review.**