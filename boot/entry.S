.set FLAGS, 0x00000003
.set MAGIC, 0x1badb002
.set CHKSM, -(MAGIC+FLAGS)

.align(4)
.section .multiboot
	.long MAGIC
	.long FLAGS
	.long CHKSM

.section .bss
.align 16
stack_bottom:
	.skip 1024*16
stack_top:

.text
.global _start
_start:
	xchg %bx, %bx	# breakpoint for bochs
	call kmain
	cli
1:	hlt
	jmp 1b
