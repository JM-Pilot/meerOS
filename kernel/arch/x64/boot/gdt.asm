[BITS 64]
[DEFAULT REL]
section .text
global gdt_load

extern gdtr
gdt_load:
	lgdt [gdtr]

	push 0x8
	lea rax, [rel .reload_reg]
	push rax
	retfq

.reload_reg:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret