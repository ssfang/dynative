
#ifdef __x86_64__
.text
.globl stubthunk_interpret_stdcall_x64
.extern dispatch

stubthunk_interpret_stdcall_x64:
	push   %rbp					/* 55 */
 	mov    %rsp, %rbp			/* 48 89 e5 */
 	/* sub	   $8, %rsp				size_t argsize */
 	/* mov    %rax, -0xb8(%rbp)	argsize = %RAX */

 	push   %rax			/* size_t argsize */
	call   dispatch

 	leave	/* restore the local scoped stack frame considering `call dispatch`*/
 	leave	/* restore the stack frame of the interpreted stdcall */
 	ret
#endif
