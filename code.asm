SETR r0, 5
SETR r1, 5
CMP r0, r1
JNE fail
SETR r2, 1
JMP end
fail:
SETR r2, 0
end:
PRINTR r2
HALT