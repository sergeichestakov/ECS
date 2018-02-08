.global get_combs

.text

.equ ws, 4
.equ len, (4*ws)
.equ k, (3*ws)
.equ items,(2*ws)
.equ combs,(-1*ws)
.equ currComb,(-2*ws)
.equ r,(-3*ws)
.equ index,(-4*ws)

#This is an iterative solution

store: #currComb stores the indeces of items to put into the array

	#malloc space for combs[curr] first
	movl k(%ebp), %ecx
	shll $2, %ecx
	push %ecx #ecx = k * sizeof(int)
	call malloc
	addl $1*ws, %esp #clear args
	movl combs(%ebp), %esi
	movl %eax, (%esi, %ebx, ws) #combs[curr] = malloc(k * sizeof(int))

	#for i = 0; i < k; i++
	#combs[curr][i] = items[currComb[i]]
	movl $0, %ecx #ecx = i
	for:
		cmpl k(%ebp), %ecx #while i < k	
		jge end_store
		
		movl combs(%ebp), %eax #eax = combs
		movl (%eax, %ebx, ws), %eax #eax = combs[curr]
		movl currComb(%ebp), %edx
		movl (%edx, %ecx, ws), %edx #edx = currComb[i]
		movl items(%ebp), %esi
		movl (%esi, %edx, ws), %esi #esi = items[currComb[i]]
		movl %esi, (%eax, %ecx, ws) #combs[curr][i] = items[currComb[i]]
		
		incl %ecx #i++
		jmp for
	end_store:

	ret

#this function adds the generated combinations to the 2D array
addCombos:
	#set r and index to 0
	movl $0, r(%ebp)
	movl $0, index(%ebp)
	movl $0, %ebx #ebx keeps track of the index of **combs

	whileloop:
		movl $0, %ecx
		cmpl r(%ebp), %ecx #while r >= 0
		jg end

		#ecx = len + (r - k)
		movl len(%ebp), %ecx
		addl r(%ebp), %ecx
		subl k(%ebp), %ecx

		cmpl index(%ebp), %ecx #index <= (len + (r - k))
		jl else
		if:
			movl r(%ebp), %edx
			movl currComb(%ebp), %ecx
			movl index(%ebp), %eax
			movl %eax, (%ecx, %edx, ws) #combination[r] = index;

			movl k(%ebp), %ecx
			decl %ecx
			cmpl r(%ebp), %ecx #if r == k-1
			jnz inner_else
			
			innerif:
			call store #This is where we actually add to combs
			incl %ebx #curr++
			incl index(%ebp) #index++
			jmp whileloop

			inner_else:
			movl r(%ebp), %edx
			movl currComb(%ebp), %ecx
			movl (%ecx, %edx, ws), %eax #combination[r] is in eax
			incl %eax
			movl %eax, index(%ebp) #index = combination[r] + 1
			incl r(%ebp) #r++
			jmp whileloop

		else:
			decl r(%ebp) #r--
			movl $0, %edx
			cmpl r(%ebp), %edx
			jge inner_else2
			inner_if2:
				movl r(%ebp), %edx
				movl currComb(%ebp), %ecx
				movl (%ecx, %edx, ws), %eax #combination[r] is in eax
				incl %eax
				movl %eax, index(%ebp) #index = combination[r] + 1
				jmp whileloop

			inner_else2:
				movl currComb(%ebp), %ecx
				movl (%ecx), %eax #combination[0] is in eax
				incl %eax
				movl %eax, index(%ebp) #index = combination[0] + 1
				jmp whileloop
	end:	
	ret

#int** get_combs(int* items, int k, int len)
get_combs:
	prologue:
	push %ebp
	movl %esp, %ebp 
	subl $4*ws, %esp #local vars
	push %ebx #save regs
	push %esi

	#first we need to find the number of rows in order to know how much to malloc
	push k(%ebp)	
	push len(%ebp)
	call num_combs #result is in eax
	addl $2*ws, %esp #clear args

	shll $2, %eax #eax = num_combs * sizeof(int*)
	push %eax
	call malloc
	addl $1*ws, %esp #clear args
	movl %eax, combs(%ebp) #malloced that much space for combs

	#next we malloc the space for the current combination (k * sizeof(int))
	movl k(%ebp), %eax
	shll $2, %eax #eax = k * sizeof(int)
	push %eax
	call malloc
	addl $1*ws, %esp
	movl %eax, currComb(%ebp)

	call addCombos #main function

	movl combs(%ebp), %eax #return the array
	
	epilogue:
	pop %esi
	pop %ebx
	movl %ebp, %esp
	pop %ebp
	ret

