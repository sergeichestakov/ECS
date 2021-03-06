.global matMult

.text
matMult:
	.equ ws, 4
	.equ a, (2*ws)
	.equ num_rows_a, (3*ws)
	.equ num_cols_a, (4*ws)
	.equ b, (5*ws)
	.equ num_rows_b, (6*ws)
	.equ num_cols_b, (7*ws)
	.equ result, (-1*ws)
	.equ row,(-2*ws)
	.equ ans,(-3*ws)

#prologue
	push %ebp
	movl %esp, %ebp 
	subl $3*ws, %esp #local vars
	push %ebx
	push %esi
	push %edi
	
	call mallocArray
	
	movl $0, %ecx #ecx = i
	for1:
		cmpl num_rows_a(%ebp), %ecx
		jge end_for1
	
		movl $0, %ebx #ebx = j
		for2:
			cmpl num_cols_b(%ebp), %ebx
			jge end_for2
		
			movl $0, ans(%ebp) #ans will be what is actually placed at [i][j] of the matrix
			movl $0, %esi #esi = k
			for3:
				cmpl num_cols_a(%ebp), %esi
				jge end_for3
				
				movl b(%ebp), %edi
				movl (%edi, %esi, ws), %edi #edi = b[k]
				movl (%edi, %ebx, ws), %edi #edi = b[k][j]

				movl a(%ebp), %eax
				movl (%eax, %ecx, ws), %eax
				movl (%eax, %esi, ws), %eax #a[i][k]
	
				mull %edi #eax = a[i][k] * b[k][j]

				addl %eax, ans(%ebp) #result[i][j] += a[i][k] * b[k][j]

				incl %esi
				jmp for3
			end_for3:

			movl result(%ebp), %edi
			movl (%edi, %ecx, ws), %edi #edi = result[i]

			#actually sets the value of the array
			movl ans(%ebp), %eax 
			movl %eax, (%edi, %ebx, ws) #put the answer in result[i][j]

			incl %ebx
			jmp for2
		end_for2:


		incl %ecx
		jmp for1
	end_for1:
	movl result(%ebp), %eax

#epilogue
	pop %edi
	pop %esi
	pop %ebx
	movl %ebp, %esp
	pop %ebp
	ret

#allocate space for the array
mallocArray:
	movl num_rows_a(%ebp), %eax
	shll $2, %eax #eax = num_rows_a * sizeof(int*)
	push %eax
	call malloc
	addl $1*ws, %esp #clear args
	movl %eax, result(%ebp)

	movl $0, %edx
	for:
		cmpl num_rows_a(%ebp), %edx
		jge end_for

		movl %edx, row(%ebp) #save row
		movl num_cols_b(%ebp), %eax
		shll $2, %eax
		push %eax
		call malloc
		addl $1*ws, %esp
		movl row(%ebp), %edx #restore row

		movl result(%ebp), %ecx
		movl %eax, (%ecx, %edx, ws) 

		incl %edx
		jmp for
	end_for:
	ret

