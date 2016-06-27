
/*
https://gcc.gnu.org/onlinedocs/gcc/Overall-Options.html

file.s          Assembler code.
file.S, file.sx Assembler code that must be preprocessed.

*/
#ifdef __x86_64__

.globl stubthunk_interpret_stdcall_x64
.extern dispatch

.text
stubthunk_interpret_stdcall_x64:
	push   %rbp					/* 55 */
 	mov    %rsp, %rbp			/* 48 89 e5 */
 	/* sub	   $8, %rsp				size_t argsize */
 	/* mov    %rax, -0xb8(%rbp)	argsize = %RAX */

 	push   %rax			/* size_t argsize */
	call   dispatch

 	leave	/* restore the local scoped stack frame considering `call dispatch`*/
 	leave	/* TODO here wrong, be deliberate!! restore the stack frame of the interpreted stdcall */
 	ret
#endif
