# Table des labels : dictionnaire simple
labels = {}  # {"fail": 7, "start": 0, ...}

# Instructions et leur nombre d'arguments
INSTRUCTIONS = {
    "PUSH": 1,
    "POP": 0,
    "ADD": 0,
    "SUB": 0,
    "CMP": 0,
    "JMP": 1,
    "JEQ": 1,
    "JNE": 1,
    "LOAD": 1,
    "HALT": 0,
}