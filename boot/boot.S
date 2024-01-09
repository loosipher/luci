.set ALIGN,	1 << 0
.set MEMNF,	1 << 1
.set FLAGS,	ALIGN | MEMNF
.set MAGIC,	0x1badb002
.set CHKSM,	-(MAGIC+FLAGS)


.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHKSM


.section .bss
.align 16
stack_bottom:
.skip 1024*16
stack_top:


.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	call kmain
	cli
1:	hlt
	jmp 1b
.size _start,	. - _start
