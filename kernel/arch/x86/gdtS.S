.text
.global load_segments
.type load_segments, @function
load_segments:
	pushl %eax
	jmp $0x08, $.reload_cs
.reload_cs:
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	popl %eax
	ret
