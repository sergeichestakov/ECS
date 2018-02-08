.global _start
.data

num1:
	.long 5 
	.long 4

num2:

	.long 12
	.long 3

.text

_start:

	movl $num1, %esi #store arrays in registers
	movl $num2, %edi

	movl (%esi), %edx #sum up upper 32 bits in edx
	addl (%edi), %edx
	
	addl $4, %esi #increment both arrays by an integer
	addl $4, %edi

	movl (%esi), %eax #sum up lower 32 bits in eax
	addl (%edi), %eax
	adc $0, %edx #add carry bit

done:
	movl %eax, %eax

