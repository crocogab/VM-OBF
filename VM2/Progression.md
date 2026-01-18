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

## 📋 Étape 8 : Obfuscation niveau 3 (À VENIR)

### Techniques à implémenter
- [ ] Dead code insertion (dans l'assembleur Python)
- [ ] Super-operators (fusionner plusieurs opérations)
- [ ] Control Flow Flattening

---

## 📊 Progression globale

```
[███████████████████░] 90% - VM complète + Assembleur Python
```

---

## 📁 Fichiers du projet

| Fichier | Description |
|---------|-------------|
| `vm.c` | VM principale avec tous les handlers |
| `assembler.py` | Convertisseur ASM → bytecode C |
| `test.asm` | Programme de test |

---

## 🔗 Ressources

- Document de référence : "Architecture Forteresse" (3 piliers)
  1. Stack Machine ✅
  2. Computed Goto + MBA ✅
  3. Control Flow Flattening (à venir)

---

## 🎯 Prochaines étapes suggérées

1. **Dead code insertion** : Modifier l'assembleur pour insérer du bruit automatiquement
2. **Super-operators** : Fusionner `LOAD + PUSH + CMP` en un seul opcode
3. **Control Flow Flattening** : Variable d'état + dispatcher central