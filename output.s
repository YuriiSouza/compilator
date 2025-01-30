section .data
section .bss
section .text
global _start
_start:
    t0 resd 1
    mov eax, 5
    mov [t2], eax
    mov [t2], a
    mov eax, 1
    xor ebx, ebx
    int 0x80
