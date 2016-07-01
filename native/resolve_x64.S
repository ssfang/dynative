
/*
https://gcc.gnu.org/onlinedocs/gcc/Overall-Options.html

file.s          Assembler code.
file.S, file.sx Assembler code that must be preprocessed.

*/
#ifdef __x86_64__

.globl stubthunk_interpret_stdcall_x64

/**int dispatch(JNIEnv *env, jclass receiver, jmethodID mid);*/
.extern dispatch
.extern printf

format:
        .asciz  "%20ld\n"

stubthunk_interpret_stdcall_x64:
	push   %rbp					/* 55 */
 	mov    %rsp, %rbp			/* 48 89 e5 */
 	/* RCX, RDX, R8, R9; (XMM0, YMM0), (XMM1, YMM1), (XMM2, YMM2), (XMM3, YMM3): Volatile */
	push   %rcx
	push   %rdx
	push   %r8
	push   %r9

 	mov	   %r10, %r8	/* r10: jmethodID mid */
	call   dispatch

	pop    %r9
	pop    %r8
	pop    %rdx
	pop    %rcx

 	leave	/* restore the local scoped stack frame considering `call dispatch`*/
 	add    %rax, %rsp /* TODO here wrong, be deliberate!! restore the stack frame of the interpreted stdcall */
 	ret
#endif
