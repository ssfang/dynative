
#include "resolve.S"

#ifdef INCLUDE_resolve_x86

.globl stubthunk_interpret_stdcall_x86
.extern dispatch

.text

stubthunk_interpret_stdcall_x86:
	push   %ebp					/* 55 */
 	mov    %esp, %ebp			/* 48 89 e5 */
 	/* sub	   $8, %esp				size_t argsize */
 	/* mov    %rax, -0xb8(%ebp)	argsize = %EAX */

	/* expect like [invoke PROC proto](https://support.microsoft.com/en-us/kb/73407) */
 	push   %eax			/* size_t argsize */
 	push   0xc(%ebp)	/* jclass cls or jobject obj */
 	push   0x8(%ebp)	/* JNIEnv *env */
	call   dispatch

 	leave	/* restore the stack frame */
 	ret

#endif
