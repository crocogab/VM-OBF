LOAD r0, 0
SETR r1, 65
CMP r0, r1
JNE fail 
LOAD r0, 1
SETR r1, 66
CMP r0, r1
JNE fail 
LOAD r0, 2
SETR r1, 67
CMP r0, r1
JNE fail 
SETR r2, 1
JMP end
fail:
SETR r2, 0
end:
PRINTR r2
HALT