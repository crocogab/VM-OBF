# Progression : Création d'une VM obfusquée

## 📊 Vue d'ensemble
```
Phase 1 [████████████████████] 100%
Phase 2 [████████████████████] 100%
Phase 3 [████████████░░░░░░░░] 66%
Phase 4 [░░░░░░░░░░░░░░░░░░░░] 0%
Phase 5 [░░░░░░░░░░░░░░░░░░░░] 0%
```

---

## ✅ Phase 1 : Comprendre les bases (TERMINÉE)

| Étape | Description | Statut |
|-------|-------------|--------|
| 1.1 | VM minimale (accumulator, SET, ADD, PRINT) | ✅ |
| 1.2 | Ajouter plusieurs registres (r0-r3) | ✅ |
| 1.3 | Ajouter la mémoire (LOAD, STORE) | ✅ |
| 1.4 | Ajouter les sauts conditionnels (CMP, JMP, JEQ, JNE) | ✅ |

**Ce que tu as appris :**
- Le cycle fetch-decode-execute
- Gestion des registres et de la mémoire
- Les sauts et le calcul d'offsets
- Utiliser `pc` pour parcourir le bytecode

---

## ✅ Phase 2 : VM fonctionnelle (TERMINÉE)

| Étape | Description | Statut |
|-------|-------------|--------|
| 2.1 | Créer un assembleur Python | ✅ |
| 2.2 | Programme de vérification de password | ✅ |
| 2.3 | Lire le password depuis stdin | ✅ |

**Ce que tu as appris :**
- Assembleur deux passes (labels puis bytecode)
- Calcul automatique des offsets
- Génération de bytecode C

**Fichiers créés :**
- `vm.c` — La VM
- `assembler.py` — L'assembleur
- `check_password.asm` — Programme qui vérifie "ABC"

---

## 🔄 Phase 3 : Obfuscation niveau 1 (EN COURS)

| Étape | Description | Statut |
|-------|-------------|--------|
| 3.1 | Chiffrement XOR simple | ✅ |
| 3.2 | Opcodes randomisés | ✅ |
| 3.3 | Chiffrement rolling (clé qui évolue) | 🔄 En cours |

**Étape 3.1 — Terminée :**
- ✅ Assembleur modifié pour XOR le bytecode
- ✅ Macro `READ()` pour déchiffrer
- ✅ Bug corrigé : `__uint8_t` au lieu de `__int8_t`

**Étape 3.2 — Terminée :**
- ✅ Génération aléatoire des opcodes
- ✅ Fichiers `opcodes.h` et `bytecode.h` générés automatiquement
- ✅ VM synchronisée via `#include`

**Étape 3.3 — En cours :**
- ⏳ Implémenter le rolling XOR dans l'assembleur
- ⏳ Modifier la VM pour déchiffrer avec clé évolutive

---

## ⏳ Phase 4 : Obfuscation niveau 2 (À VENIR)

| Étape | Description | Statut |
|-------|-------------|--------|
| 4.1 | Indirect threading (table de handlers) | ⏳ |
| 4.2 | Self-modifying code | ⏳ |
| 4.3 | Anti-debug basique | ⏳ |

---

## ⏳ Phase 5 : Finalisation (À VENIR)

| Étape | Description | Statut |
|-------|-------------|--------|
| 5.1 | Intégrité croisée VM/bytecode | ⏳ |
| 5.2 | Faux chemins et code mort | ⏳ |
| 5.3 | Assemblage final et tests | ⏳ |

---

## 📁 Structure actuelle du projet
```
projet/
├── vm.c                  # VM avec déchiffrement XOR
├── assembler.py          # Génère opcodes.h et bytecode.h
├── opcodes.h             # Opcodes aléatoires (auto-généré)
├── bytecode.h            # Bytecode chiffré (auto-généré)
└── check_password.asm    # Vérifie "ABC"
```

---

## 🛡️ Niveau de protection actuel

| Technique | Implémenté |
|-----------|-----------|
| VM custom | ✅ |
| Bytecode chiffré (XOR simple) | ✅ |
| Opcodes mutés | ✅ |
| Rolling XOR | ❌ |
| Indirect threading | ❌ |
| Self-modifying | ❌ |
| Anti-debug | ❌ |

**Difficulté estimée pour un reverser : ⭐⭐⭐ (Moyen)**

Objectif final : ⭐⭐⭐⭐⭐ (Très difficile)