#
# simple function to reload the segments after loading GDT
#
.text
.global load_segments
.type load_segments, @function
load_segments:
	pushl %eax			# save eax since we'll be using it to set the segments
	jmp $0x08, $.reload_cs		# far jump to set cs
.reload_cs:
	movw $0x10, %ax			# load ax into the segment registers
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	popl %eax
	ret
