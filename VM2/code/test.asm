; Vérifie si input[0] == 'S' (83)
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