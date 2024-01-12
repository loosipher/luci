#
#	FAT pointers
#		these same constants may show up in C files later
#		so maybe find a way to reduce repetition
#
.equ FAT_BPS, 0x0b	# bytes per sector
.equ FAT_SPC, 0x0d	# sectors per cluster
.equ FAT_RES, 0x0e	# reserved sectors
.equ FAT_NFT, 0x10	# number of FATs
.equ FAT_NRE, 0x11	# number of root entries
.equ FAT_TSZ, 0x13	# total size (in sectors)
.equ FAT_FSZ, 0x16	# number of sectors per FAT

.code16
beginning:

	.skip 62

#
#	Get in the code segments
#
	jmp $0x07c0, $_start
_start:
	mov $0x07c0, %ax
	mov %ax, %ds
	jmp stack_top
#
#	Set up a minimal stack
#
stack_bottom:
	.zero 20
stack_top:
	movw stack_top, %sp

#
#	load FAT volume
#		this probably doesn't work because I believe
#		there is a limitation to how many sectors
#		can be read by this interrupt
#
load_fat:
	xorw %ax, %ax
	movw %ax, %sp
	movw $0x0003, %ax
	movb $0x02, %ah
	xorw %cx, %cx
	movb $0x02, %cl
	xorb %dh, %dh
	movw $0x7e00, %bx
	int $0x13

#
#	write message to the screen
#
clrscr:
	mov $0x000d, %ax
	int $0x10			# clearing the screen in the process
	pushw $DBG_LOADED
	pushw $0x00			# push index
write:
	popw %cx
	mov $0x02, %ah
	xorb %bh, %bh
	xorb %dh, %dh
	movb %cl, %dl
	int $0x10
	addw $0x0001, %cx
	popw %bx			# pop pointer into bx
	movb (%bx), %al			# put character of string into al
	cmpb $0x00, %al
	je write_end
	addw $0x0001, %bx
	pushw %bx
	pushw %cx
	mov $0x000a, %bx		# put page and style data into bx
	mov %bx, %cx			# set cx to print only once
	mov $0x09, %ah
	int $0x10			# print the character to screen
	jmp write
write_end:

	cli
1:	hlt
	jmp 1b

DBG_LOADED:
	.ascii "Volume loaded."
	.byte 0x00
DBG_LOADED_END:



	.zero 510-(.-beginning)
	.word 0xaa55
