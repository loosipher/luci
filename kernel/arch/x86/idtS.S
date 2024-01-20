# IDT stubs


.macro ISR_NOEC it
.global ISR_stub_\it
ISR_stub_\it:
	pushl $0xdeadbeef
	pushl $\it
	jmp isr_common
	iret
.endm

.macro ISR_EC it
.global ISR_stub_\it
ISR_stub_\it:
	pushl $\it
	jmp isr_common
	iret
.endm

.text
.altmacro

	ISR_NOEC 0
	ISR_NOEC 1
	ISR_NOEC 2
	ISR_NOEC 3
	ISR_NOEC 4
	ISR_NOEC 5
	ISR_NOEC 6
	ISR_NOEC 7
	ISR_EC   8
	ISR_NOEC 9
	ISR_EC   10
	ISR_EC   11
	ISR_EC   12
	ISR_EC   13
	ISR_EC   14
	ISR_NOEC 15
	ISR_NOEC 16
	ISR_EC   17
	ISR_NOEC 18
	ISR_NOEC 19
	ISR_NOEC 20
	ISR_EC   21
	ISR_NOEC 22
	ISR_NOEC 23
	ISR_NOEC 24
	ISR_NOEC 25
	ISR_NOEC 26
	ISR_NOEC 27
	ISR_NOEC 28
	ISR_EC   29
	ISR_EC   30
	ISR_NOEC 31

.set i,32
.rept 256-32
	ISR_NOEC %i
.set i,i+1
.endr

isr_common:
	pushal			# pushes edi, esi, ep esp, ebx, edx, ecx, eax

	movw %ds, %ax		# save data segment
	pushl %eax

	movw $0x10, %ax		# set segments to kernel data segment
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	pushl %esp		# push pointer stack to C
	call exception_handler	# call C program
	add $0x04, %esp

	popl %eax		# restore previous data segments
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	popal			# restore registers
	add $0x08, %esp		# remove error code and int number
	iret
