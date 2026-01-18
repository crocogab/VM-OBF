# Progression VM "Forteresse"

## 🎯 Objectif final
Créer une VM d'obfuscation robuste basée sur une stack machine pour protéger un check de mot de passe.

---

## ✅ Étape 1 : Fondations (COMPLÉTÉ)

### Concepts acquis
- [x] Comprendre la différence entre VM à registres et stack machine
- [x] Comprendre pourquoi la pile est plus difficile à reverser
- [x] Savoir exprimer des calculs en notation pile (ex: `A + B` → `PUSH A, PUSH B, ADD`)

### Code implémenté
- [x] Structure `Stack` avec tableau de 2048 éléments (uint64_t)
- [x] Structure `VMContext` (stack, stack_pointer, vip, flag, input, decrypt_key, running)
- [x] Fonction `push()`
- [x] Fonction `pop()`
- [x] Fonction `read_word()` avec déchiffrement
- [x] Fonction `init_vm()`

---

## ✅ Étape 2 : Dispatcher basique (COMPLÉTÉ)

### Instructions implémentées
- [x] `PUSH` (0x01) — empile une valeur
- [x] `POP` (0x02) — dépile une valeur
- [x] `ADD` (0x03) — dépile 2 valeurs, empile la somme
- [x] `SUB` (0x04) — dépile 2 valeurs, empile la différence
- [x] `HALT` (0xFF) — arrête la VM

### Test validé
- [x] Calcul de `10 + 5 = 15` fonctionne ✓

---

## ✅ Étape 3 : Comparaisons et sauts (COMPLÉTÉ)

### Instructions implémentées
- [x] `CMP` (0x05) — compare deux valeurs, met le flag
- [x] `JMP` (0x06) — saut inconditionnel
- [x] `JEQ` (0x07) — saut si flag = 1
- [x] `JNE` (0x08) — saut si flag = 0

### Concepts acquis
- [x] Choix de l'approche : CMP + flag (style x86)
- [x] Gestion des adresses absolues dans le bytecode
- [x] Synchronisation de la clé lors des sauts (`sync_key`)

---

## ✅ Étape 4 : Entrée/Sortie (COMPLÉTÉ)

### Instructions implémentées
- [x] `LOAD` (0x09) — charge un caractère de l'input sur la pile
- [x] Buffer `input[256]` dans VMContext

### Concepts acquis
- [x] Séparation mémoire input / pile d'exécution (architecture standard)

---

## ✅ Étape 5 : Programme "Check Password" (COMPLÉTÉ)

### Réalisations
- [x] Bytecode vérifiant "SECRET" caractère par caractère
- [x] Test mot de passe correct → empile 1
- [x] Test mot de passe incorrect → empile 0

---

## ✅ Étape 6 : Obfuscation niveau 1 (COMPLÉTÉ)

### Techniques implémentées
- [x] Chiffrement du bytecode (rolling XOR)
- [x] Fonction `encrypt_bytecode()`
- [x] Déchiffrement à la volée dans `read_word()`
- [x] Fonction `sync_key()` pour synchroniser après les sauts
- [x] Opcode mapping (4 aliases par instruction)

### Opcodes mappés
| Base | Alias 2 | Alias 3 | Alias 4 |
|------|---------|---------|---------|
| PUSH 0x01 | 0x11 | 0x21 | 0x31 |
| POP 0x02 | 0x12 | 0x22 | 0x32 |
| ADD 0x03 | 0x13 | 0x23 | 0x33 |
| SUB 0x04 | 0x14 | 0x24 | 0x34 |
| CMP 0x05 | 0x15 | 0x25 | 0x35 |
| JMP 0x06 | 0x16 | 0x26 | 0x36 |
| JEQ 0x07 | 0x17 | 0x27 | 0x37 |
| JNE 0x08 | 0x18 | 0x28 | 0x38 |
| LOAD 0x09 | 0x19 | 0x29 | 0x39 |
| STORE 0x0A | 0x1A | 0x2A | 0x3A |
| DUP 0x0B | 0x1B | 0x2B | 0x3B |
| SWAP 0x0C | 0x1C | 0x2C | 0x3C |
| HALT 0xFF | 0xFE | 0xFD | 0xFC |

---

## ✅ Étape 7 : Obfuscation niveau 2 (COMPLÉTÉ)

### Techniques implémentées
- [x] Computed goto (table de pointeurs de fonctions)
- [x] Handlers séparés pour chaque instruction
- [x] Table `handlers[256]` avec `handler_invalid` par défaut
- [x] MBA (Mixed Boolean-Arithmetic) pour ADD et SUB

### Formules MBA utilisées
```c
// ADD: a + b
t1 = 3 * (a | b);
t2 = 2 * (~(a ^ b));
t3 = ~(a & b);
result = t1 + t2 + t3 + 3;

// SUB: a - b
t1 = 3 * (a | ~b);
t2 = 2 * (a ^ b);
t3 = ~(a & ~b);
result = t1 + t2 + t3 + 4;
```

---

## ✅ Étape 7.5 : Instructions avancées + Assembleur (COMPLÉTÉ)

### Nouvelles instructions
- [x] `STORE` (0x0A) — memory[idx] = pop()
- [x] `DUP` (0x0B) — duplique le sommet de pile
- [x] `SWAP` (0x0C) — échange les 2 éléments du sommet
- [x] Mémoire RAM `memory[256]` dans VMContext

### Assembleur Python
- [x] Parsing de fichiers `.asm`
- [x] Support des labels (ex: `fail:`, `start:`)
- [x] Résolution en deux passes
- [x] Pass 1 : collecte des labels et adresses
- [x] Pass 2 : génération du bytecode
- [x] Support des commentaires (`;`)
- [x] Génération de tableau C prêt à copier
- [x] **Opcode aliasing aléatoire** à la compilation
- [x] **Bytecode pré-chiffré** (XOR appliqué par l'assembleur)
- [x] **Dead code insertion** automatique

### Exemple de syntaxe supportée
```asm
; Vérifie si input[0] == 'S'
start:
    LOAD 0
    PUSH 83
    CMP
    JNE fail

success:
    PUSH 1
    JMP end

fail:
    PUSH 0

end:
    HALT
```

---

## ✅ Étape 8 : Obfuscation niveau 3 — Control Flow Flattening (COMPLÉTÉ)

### Technique implémentée
- [x] Machine à états avec `VMState` enum
- [x] Dispatcher transformé en switch géant
- [x] 15 états : FETCH, DECODE, EXEC_*, HALT
- [x] Flux de contrôle non-linéaire

### États de la VM
```c
typedef enum {
    STATE_FETCH,        // Lecture de l'opcode
    STATE_DECODE,       // Détermination de l'instruction
    STATE_EXEC_PUSH,    // Exécution PUSH
    STATE_EXEC_POP,     // Exécution POP
    STATE_EXEC_ADD,     // Exécution ADD
    STATE_EXEC_SUB,     // Exécution SUB
    STATE_EXEC_CMP,     // Exécution CMP
    STATE_EXEC_JMP,     // Exécution JMP
    STATE_EXEC_JEQ,     // Exécution JEQ
    STATE_EXEC_JNE,     // Exécution JNE
    STATE_EXEC_LOAD,    // Exécution LOAD
    STATE_EXEC_STORE,   // Exécution STORE
    STATE_EXEC_DUP,     // Exécution DUP
    STATE_EXEC_SWAP,    // Exécution SWAP
    STATE_HALT,         // Arrêt de la VM
} VMState;
```

### Pourquoi c'est efficace
| Avant | Après |
|-------|-------|
| `while` → `call handler` | Switch géant avec 15 cases |
| Flux linéaire prévisible | Variable `state` imprévisible |
| IDA reconstruit facilement | IDA montre un spaghetti |

---

## 📋 Étape 9 : Améliorations avancées (À VENIR)

### Techniques à implémenter
- [ ] Opaque predicates (conditions toujours vraies/fausses)
- [ ] Mélange aléatoire de l'ordre des cases
- [ ] États factices (fake states)
- [ ] Transitions indirectes via table
- [ ] Super-operators (fusionner plusieurs opérations)
- [ ] Anti-debug / Anti-VM detection

---

## 📊 Progression globale

```
[█████████████████████] 95% - VM complète + Assembleur + Control Flow Flattening
```

---

## 🏰 Architecture Forteresse — Les 3 Piliers

| Pilier | Description | Status |
|--------|-------------|--------|
| 1. Stack Machine | Pas de registres visibles, tout sur la pile | ✅ |
| 2. Computed Goto + MBA | Table de handlers + expressions opaques | ✅ |
| 3. Control Flow Flattening | Machine à états, flux non-linéaire | ✅ |

---

## 📁 Fichiers du projet

| Fichier | Description |
|---------|-------------|
| `vm.c` | VM principale avec tous les handlers + flattening |
| `assembler.py` | Convertisseur ASM → bytecode C (avec obfuscation) |
| `test.asm` | Programme de test |

---

## 🔗 Techniques d'obfuscation implémentées

| Technique | Cible | Impact |
|-----------|-------|--------|
| Rolling XOR | Bytecode | Illisible statiquement |
| Opcode aliasing | Pattern matching | 4 représentations par instruction |
| MBA | Analyse symbolique | Expressions mathématiques opaques |
| Dead code | Analyse statique | Bruit dans le bytecode |
| Control Flow Flattening | Décompilateurs | Structure de contrôle détruite |

---

## 🎯 Prochaines améliorations suggérées

1. **Opaque predicates** : `if ((x*x) >= 0)` toujours vrai
2. **Shuffle des cases** : ordre aléatoire dans le switch
3. **Fake states** : états qui ne font rien mais existent
4. **Table de transitions** : `next_state = table[current][opcode]`
5. **Super-operators** : `LOAD_CMP` = LOAD + PUSH + CMP
6. **Anti-debug** : détecter GDB/strace/ptrace