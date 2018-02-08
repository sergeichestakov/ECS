.global _start
.data

string1:
	.space 100

string2:
	.space 100

oldDist:
	.space 400

currDist:
	.space 400

length_1:
	.space 4

length_2:
	.space 4

temp:
	.space 4

.equ ws, 4

.text

strlen: #calculates length + 1
	movl $0, %ebx #string length stored in ebx, string is in edx
	for_strlen:
		cmpb $0, (%edx, %ebx)
		jz end_for_strlen

		incl %ebx
		jmp for_strlen
	end_for_strlen:
	incl %ebx
	ret

initializeArrays:
	/*
	for(i = 0; i < word2_len + 1; i++){
   		oldDist[i] = i;
   		curDist[i] = i;
  	}
	*/
	movl $0, %esi #i = 0
	start_init:
		cmpl length_2, %esi
		jge end_init
	
		movl %esi, (%ebx, %esi, ws)
		movl %esi, (%ecx, %esi, ws) 

		incl %esi
		jmp start_init
	end_init:
	ret
	
min: #Compares the values in registers eax and edx and returns in eax
	cmpl %edx, %eax
	jge bsmaller
		jmp end_min
	bsmaller:
		movl %edx, %eax
	end_min:
	ret

swap: #swaps the values in oldDist (ebx) and currDist (ecx)
	movl %ebx, %edx #temp = oldDist
	movl %ecx, %ebx #oldDist = currDist
	movl %edx, %ecx #currDist = temp
	ret


_start: #Computes the edit distance between two strings and stores it in eax
	
	#store the length of both strings in memory
	movl $string1, %edx
	call strlen
	movl %ebx, length_1

	movl $string2, %edx
	call strlen 
	movl %ebx, length_2

	movl $oldDist, %ebx #store oldDist in ebx and currDist in ecx
	movl $currDist, %ecx 
	call initializeArrays

	movl $1, %esi # i = 1

	outer_for_loop:
		cmpl length_1, %esi
		jge end_outer

		movl %esi, (%ecx) #curDist[0] = i;
	
		movl $1, %edi # j = 1
		inner_for_loop:
			cmpl length_2, %edi
			jge end_inner
			
			movb string1-1(,%esi), %al #word1[i-1] 

			movb string2-1(,%edi), %dl #word2[j-1]

			cmpb %al, %dl#if(word1[i-1] == word2[j-1])
			jnz else
			
			movl -ws(%ebx, %edi, ws), %eax
			movl %eax, (%ecx, %edi, ws) #curDist[j] = oldDist[j - 1];
			jmp continue
			else:
				/*curDist[j] = min(min(oldDist[j], //deletion
                     curDist[j-1]), //insertion
                     oldDist[j-1]) + 1; //subtitution*/
				movl (%ebx, %edi, ws), %eax #oldDist[j]
				movl -ws(%ecx, %edi, ws), %edx #curDist[j-1]

				call min #smallest of the two is currently in eax
	
				movl -ws(%ebx, %edi, ws), %edx #oldDist[j-1]
				call min #smallest of the three is now in eax
			
				incl %eax #add one first, then transfer over to array
				movl %eax, (%ecx, %edi, ws) #currDist[j] = min value + 1
			
			continue:

			incl %edi
			jmp inner_for_loop
		end_inner:
		call swap
		incl %esi
		jmp outer_for_loop

	end_outer:
		movl length_2, %edx
		decl %edx #decrement beacuse length_2 stores length + 1
		movl (%ebx,%edx,ws), %eax #dist = oldDist[word2_len]

done:
	movl %eax, %eax
