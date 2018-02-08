.global _start
.data

a:
	.long 1

b:
	.long 5

.text

_start:
	movl $a, %ecx #move a and b into registers
	movl $b, %ebx	

	movl $0, %esp #this is the second half of a to add to edx
	movl $0, %ebp
	movl $1, %edi #this is the one that we right shift to compare

loop: #store in edx:eax
	cmpl $32, %ebp
	jge done
	
	movl %edi, %esi #save the value of edi in esi before and operation
	andl (%ebx), %edi #if(n2 & (1 << index)) - result is store in edi
	cmpl $0, %edi
	jz continue
	
	#result += (a << index)
	addl %esp, %edx
	addl (%ecx), %eax
	adc $0, %edx
	
continue:
	#left shift a every time and hold the remainder in reg esp
	shll $1, %esp
	shll $1, (%ecx)
	adc $0, %esp

	movl %esi, %edi
	shll $1, %edi #shift index one

	incl %ebp
	jmp loop

done:
	movl %eax, %eax

