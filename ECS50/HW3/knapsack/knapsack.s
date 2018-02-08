.global knapsack

.text

knapsack:
	.equ ws, 4
	.equ weights,(2*ws) #arguments
	.equ values,(3*ws)
	.equ num_items,(4*ws)
	.equ capacity,(5*ws)
	.equ cur_value,(6*ws)
	
	.equ bestVal,(-1*ws) #local var
	.equ temp,(-2*ws) #save i

	prologue:
	push %ebp
	movl %esp, %ebp
	push %ebx
	subl $2*ws, %esp #make space for locals

	movl $0, %ecx #i = 0
	movl cur_value(%ebp), %edx
	movl %edx, bestVal(%ebp) #best_value = cur_value

	for:
		cmpl num_items(%ebp), %ecx #while i < num_items
		jge end_for		

		movl weights(%ebp), %edx
		movl (%edx, %ecx, ws), %edx #edx = weights[i]
		cmpl %edx, capacity(%ebp) #capacity - weights[i]
		jl else #if(capacity - weights[i] >= 0 )

		if:
		/*
 		best_value = max(best_value, knapsack(weights + i + 1, values + i + 1, num_items - i - 1, 
                     capacity - weights[i], cur_value + values[i]));
		*/

 		#recursive call to the function: Push the arguments in reverse order
		
		movl values(%ebp), %ebx
		movl (%ebx, %ecx, ws), %ebx
		addl cur_value(%ebp), %ebx
		push %ebx #ebx contains cur_value + values[i]
		
		movl capacity(%ebp), %ebx
		movl weights(%ebp), %edx
		movl (%edx, %ecx, ws), %edx #edx = weights[i]
		subl %edx, %ebx
		push %ebx #ebx contains capacity - weights[i]

		movl num_items(%ebp), %ebx
		subl %ecx, %ebx
		decl %ebx
		push %ebx #ebx contains num_items - i - 1

		movl values(%ebp), %ebx
		leal ws(%ebx, %ecx, ws), %ebx
		push %ebx #ebx contains values + i + 1

		movl weights(%ebp), %ebx
		leal ws(%ebx, %ecx, ws), %ebx 
		push %ebx #ebx contains weights + i + 1

		movl %ecx, temp(%ebp)
		call knapsack #recursive call: result is in eax
		movl temp(%ebp), %ecx
		addl $5*ws, %esp #clear args

		#find the max of eax and bestVal

		cmpl bestVal(%ebp), %eax
		jl else 

		movl %eax, bestVal(%ebp) #set best value to result of recursive call

		else:
		incl %ecx #i++
		jmp for
	end_for:

	movl bestVal(%ebp), %eax
	
	epilogue:
	
	pop %ebx #restore reg
	movl %ebp, %esp
	pop %ebp
	ret
