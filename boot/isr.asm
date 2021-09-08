.section .text
.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15




	# Tell the assembler theres an external symbol in use
	.extern isr_handler
	.extern irq_handler

	
	# irq common stub
	.global irq_common_stub
	.type irq_common_stub, @function
	irq_common_stub:
		pusha
		mov %ds, %ax
		push %eax
		mov $0x10, %ax
		mov %ax, %ds 
		mov %ax, %es
		mov %ax, %fs
		mov %ax, %gs
		
		push %esp
		cld
		
		call irq_handler
		
		pop %ebx
		pop %ebx
		mov %bx, %ds
		mov %bx, %es
		mov %bx, %fs
		mov %bx, %gs
		
		popa
		addl $8, %esp
		sti
		iret

	# isr common stub
	.global isr_common_stub
	.type isr_common_stub, @function
	isr_common_stub:
		pusha
		mov %ds, %ax
		push %eax
		mov $0x10, %ax
		mov %ax, %ds 
		mov %ax, %es
		mov %ax, %fs
		mov %ax, %gs
		
		push %esp
		cld
		
		call isr_handler
		
		pop %eax
		pop %eax
		mov %bx, %ds
		mov %bx, %es
		mov %bx, %fs
		mov %bx, %gs
		
		popa
		addl $8, %esp
		sti
		iret

		
isr0:
    cli
    pushl $0
    pushl $0
    jmp isr_common_stub


isr1:
    cli
    pushl $ 0
    pushl $ 1
    jmp isr_common_stub


isr2:
    cli
    pushl $ 0
    pushl $ 2
    jmp isr_common_stub


isr3:
    cli
    pushl $ 0
    pushl $ 3
    jmp isr_common_stub


isr4:
    cli
    pushl $ 0
    pushl $ 4
    jmp isr_common_stub


isr5:
    cli
    pushl $ 0
    pushl $ 5
    jmp isr_common_stub


isr6:
    cli
    pushl $0
    pushl $6
    jmp isr_common_stub


isr7:
    cli
    pushl $ 0
    pushl $ 7
    jmp isr_common_stub


isr8:
    cli
    pushl $ 8
    jmp isr_common_stub


isr9:
    cli
    pushl $ 0
    pushl $ 9
    jmp isr_common_stub


isr10:
    cli
    pushl $ 10
    jmp isr_common_stub


isr11:
    cli
    pushl $ 11
    jmp isr_common_stub


isr12:
    cli
    pushl $ 12
    jmp isr_common_stub


isr13:
    cli
    pushl $ 13
    jmp isr_common_stub


isr14:
    cli
    pushl $ 14
    jmp isr_common_stub


isr15:
    cli
    pushl $ 0
    pushl $ 15
    jmp isr_common_stub


isr16:
    cli
    pushl $ 0
    pushl $ 16
    jmp isr_common_stub


isr17:
    cli
    pushl $ 0
    pushl $ 17
    jmp isr_common_stub


isr18:
    cli
    pushl $ 0
    pushl $ 18
    jmp isr_common_stub


isr19:
    cli
    pushl $ 0
    pushl $ 19
    jmp isr_common_stub


isr20:
    cli
    pushl $ 0
    pushl $ 20
    jmp isr_common_stub


isr21:
    cli
    pushl $ 0
    pushl $ 21
    jmp isr_common_stub


isr22:
    cli
    pushl $ 0
    pushl $ 22
    jmp isr_common_stub


isr23:
    cli
    pushl $ 0
    pushl $ 23
    jmp isr_common_stub


isr24:
    cli
    pushl $ 0
    pushl $ 24
    jmp isr_common_stub


isr25:
    cli
    pushl $ 0
    pushl $ 25
    jmp isr_common_stub


isr26:
    cli
    pushl $ 0
    pushl $ 26
    jmp isr_common_stub


isr27:
    cli
    pushl $ 0
    pushl $ 27
    jmp isr_common_stub


isr28:
    cli
    pushl $ 0
    pushl $ 28
    jmp isr_common_stub


isr29:
    cli
    pushl $ 0
    pushl $ 29
    jmp isr_common_stub


isr30:
    cli
    pushl $ 0
    pushl $ 30
    jmp isr_common_stub
isr31:
    cli
    pushl $ 0
    pushl $ 31
    jmp isr_common_stub

	

irq0:
	pushl $ 0
	pushl $ 32
	jmp irq_common_stub

irq1:
	pushl $ 1
	pushl $ 33
	jmp irq_common_stub

irq2:
	pushl $ 2
	pushl $ 34
	jmp irq_common_stub

irq3:
	pushl $ 3
	pushl $ 35
	jmp irq_common_stub

irq4:
	pushl $ 4
	pushl $ 36
	jmp irq_common_stub

irq5:
	pushl $ 5
	pushl $ 37
	jmp irq_common_stub

irq6:
	pushl $ 6
	pushl $ 38
	jmp irq_common_stub

irq7:
	pushl $ 7
	pushl $ 39
	jmp irq_common_stub

irq8:
	pushl $ 8
	pushl $ 40
	jmp irq_common_stub

irq9:
	pushl $ 9
	pushl $ 41
	jmp irq_common_stub

irq10:
	pushl $ 10
	pushl $ 42
	jmp irq_common_stub

irq11:
	pushl $ 11
	pushl $ 43
	jmp irq_common_stub

irq12:
	pushl $ 12
	pushl $ 44
	jmp irq_common_stub

irq13:
	pushl $ 13
	pushl $ 45
	jmp irq_common_stub

irq14:
	pushl $ 14
	pushl $ 46
	jmp irq_common_stub

irq15:
	pushl $ 15
	pushl $ 47
	jmp irq_common_stub