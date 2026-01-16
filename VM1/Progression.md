# Progression : Création d'une VM obfusquée

## 📊 Vue d'ensemble
```
Phase 1 [████████████████████] 100%
Phase 2 [████████████████████] 100%
Phase 3 [████████████████████] 100%
Phase 4 [████████████░░░░░░░░] 66%
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

## ✅ Phase 3 : Obfuscation niveau 1 (TERMINÉE)

| Étape | Description | Statut |
|-------|-------------|--------|
| 3.1 | Chiffrement XOR simple | ✅ |
| 3.2 | Opcodes randomisés | ✅ |
| 3.3 | Chiffrement rolling (clé qui évolue) | ✅ |

**Ce que tu as appris :**
- Chiffrement XOR et ses limites
- Rolling XOR pour une meilleure sécurité
- Problème des sauts avec clé évolutive
- Solution par pré-calcul des clés

---

## 🔄 Phase 4 : Obfuscation niveau 2 (EN COURS)

| Étape | Description | Statut |
|-------|-------------|--------|
| 4.1 | Indirect threading (table de handlers) | ✅ |
| 4.2 | Self-modifying code | 🔄 En cours |
| 4.3 | Anti-debug basique | ⏳ À faire |

**Étape 4.1 — Terminée :**
- ✅ Fonction séparée pour chaque instruction (handlers)
- ✅ Table de pointeurs de fonctions `handlers[]`
- ✅ Appel via `handlers[opcode](&vm)`
- ✅ Plus de `switch` visible

**Étape 4.2 — En cours :**
- ✅ Compris le concept du self-modifying code
- ✅ Compris le problème des boucles
- ✅ Choix de l'approche : détruire sans garder (sécurité maximale)
- ⏳ Implémentation à faire

**Ce que tu as appris :**
- Pointeurs de fonctions en C
- Indirect threading pour cacher la structure du switch
- Self-modifying code : modifier le bytecode après lecture
- Compromis boucles vs sécurité

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
├── vm.c                  # VM avec indirect threading
├── assembler.py          # Génère opcodes.h et bytecode.h
├── opcodes.h             # Opcodes aléatoires + INITIAL_KEY (auto-généré)
├── bytecode.h            # Bytecode chiffré + BYTECODE_SIZE (auto-généré)
└── check_password.asm    # Vérifie "ABC"
```

---

## 🛡️ Niveau de protection actuel

| Technique | Implémenté |
|-----------|-----------|
| VM custom | ✅ |
| Bytecode chiffré (XOR simple) | ✅ |
| Opcodes mutés | ✅ |
| Rolling XOR | ✅ |
| Indirect threading | ✅ |
| Self-modifying | 🔄 |
| Anti-debug | ❌ |

**Difficulté estimée pour un reverser : ⭐⭐⭐⭐ (Difficile)**

Objectif final : ⭐⭐⭐⭐⭐ (Très difficile)