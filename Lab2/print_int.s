section .rodata

section .text ;code goes here
	global print_int
	global main

;----------------------------------------------
;this function will display each digit of the given integer
;this function has the signature:
;void print_int( long x)
;rdi will hold the input integer that will be displayed
;we'll print 1234 as '1' '2' '3' '4'
;currently, this function prints 4321, try storing backwards or printing in a loop to fix
;as an example for the algorithm
;1234/10 = 123 remainder 4
;123/10 = 12 r 3
;12/10 = 1 r 2
;1/10 = 0 r 1
;----------------------------------------------

print_int:
	;;prologue--taking care of stack stuff
	push rbp	;put base pointer on the stack - so we can go back to it
	mov rbp, rsp	;update the bp to this location (in case we call someone)

	sub rsp, 20	;make room for 20 digits/bytes on the stack (largest long will be 20)
	
	mov rax, rdi	;copy the first parameter (x -- rdi) into rax to do work there
 	mov rbx, 20  	;bx is the ith location in digit array (the index)
	
	mov byte [rsp+rbx], 10	;put "character return" (ascii 10) into the 1st location of array

dividing:		;a label, this is the start of the loop
	cmp rax, 0	;this is the end condition--if we got 0...
	je print	;if it was zero, go here
	
	mov rdx, 0	;if you don't initialize dx to 0 then div crashes
	mov rcx, 10	;give this the value we want to divide by
	div rcx		;rax <= rax/rcx -- remainder is put into DX
	
	add rdx, 48	;convert the number into the character based on ascii tables
	dec rbx		;the same as add rbx, 1 --increment the counter/index
	mov [rsp+rbx], dl ; store the remainder in the array

	jmp dividing	;start the loop again

print:			;handle the printing
	mov rax, 1	;tells the OS to use the WRITE function
	mov rdi, 1	;the file descriptor (1 = std out)
	
	mov rcx, rsp	;get the location of the first value of the character array
	add rcx, rbx
	;instead of next line, do mov rsi, rsp

	mov rsi, rcx	;the char array, which data starts one above the rsp
	
	;;NEW STUFF HERE
	mov rcx, 21	;put the number of bytes we allocated
	sub rcx, rbx
	;;NEW STUFF ENDS

	mov rdx, rcx	;holds the number of digits -- the number of things write needs to do
		
	syscall

	;;epilogue stuff - fix sp and bp
	;add rps, 20	;this could give you back the memory... you don't need this and moving

	mov rsp, rbp
	pop rbp		;give the base pointer back to whoever called us

	ret
	


main:
	mov rdi, 1234 	;put the function parameter here
	call print_int	;call the function

	mov rax, 60	; sys_exit system call
	mov rdi, 0	; 1st param: error code
	syscall		;without these three lines it crashes (actually maybe not... 
			;but this is a way around the epilogue forcing it to crash
