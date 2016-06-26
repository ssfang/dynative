
#ifdef __i386__
.text
.globl stubthunk_interpret_stdcall_x86
.extern _dispatch

stubthunk_interpret_stdcall_x86:
	push   %ebp					/* 55 */
 	mov    %esp, %ebp			/* 48 89 e5 */
 	/* sub	   $8, %esp				size_t argsize */
 	/* mov    %rax, -0xb8(%ebp)	argsize = %EAX */

	/* expect like [invoke PROC proto](https://support.microsoft.com/en-us/kb/73407) */
 	push   %eax			/* size_t argsize */
	call   _dispatch

 	leave	/* restore the local scoped stack frame considering `call dispatch`*/
 	leave	/* restore the stack frame of the interpreted stdcall */
 	ret
#endif
