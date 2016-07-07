
/*
https://gcc.gnu.org/onlinedocs/gcc/Overall-Options.html

file.s          Assembler code.
file.S, file.sx Assembler code that must be preprocessed.

http://www.sourceware.org/binutils/docs-2.12/as.info/Preprocessing.html#Preprocessing
*/

#if defined(SYMBOL_UNDERSCORE)
#define GLOBL_SYMBOL(x) _##x
#else
#define GLOBL_SYMBOL(x) x
/* #error "============" */
#endif

.globl st_call_win64
.globl stubthunk_interpret_stdcall_x64

/**int dispatch(JNIEnv *env, jclass receiver, jmethodID mid);*/
.extern dispatch

#define RCX %##rcx
#define RDX %##rdx
#define R8 %##r8
#define R9 %##r9

#define MOV(D, S) mov D S
#define OP(op, D, S) op D, S

/*
   Parameters
    rcx pointer to all arguments via stack, which will be refactored here.
    rdx target function pointer
    r8
    r9 new rsp
*/
st_call_win64:

	push %rsp
	mov %rdx, %rax
	mov 32(%rcx), %rsp /* rsp = &args[4] */

	mov 24(%rcx), %r9 /* r9 = args[3] */
	mov 16(%rcx), %r8 /* r8 = args[2] */
	mov 8(%rcx), %rdx /* rdx = args[1] */
	mov (%rcx), %rcx  /* rcx = args[0] */
	movd %r9, %xmm3
	movd %r8, %xmm2
	movd %rdx, %xmm1
	movd %rcx, %xmm0

	call *%rax

	pop %rsp

st_return_win64:


stubthunk_interpret_stdcall_x64:
	push   %rbp					/* 55 */
 	mov    %rsp, %rbp			/* 48 89 e5 */

 	/* Because RCX, RDX, R8, R9; XMM0/YMM0, XMM1/YMM1, XMM2/YMM2, XMM3/YMM3: Volatile.
 	   Need to be used again so they must be saved before calling dispatch.
 	   The first and second parameter slots are taken respectively by RCX and RDX.
 	   XMM0 and XMM1 have been excluded.
 	 */

	push   %rcx /* JNIEnv *env */
	push   %rdx /* jclass cls */
	push   %r8  /* param_int1 */
	push   %r9  /* param_int2 */
 	sub  $0x16, %rsp  /* Allocate local stack to preserve XMM2 and XMM3 */
 	movq %xmm2, (%rsp)    /* param_flt1 */
 	movq %xmm3, 0x8(%rsp) /* param_flt2 */

 	mov	   %r10, %r8  /* r10: jmethodID mid */
	call   dispatch   /* void* dispatch(JNIEnv *env, jclass cls, jmethodID mid) */

 	movq (%rsp), %xmm2
 	movq 0x8(%rsp), %xmm3
	add  $0x16, %rsp
	pop    %r9
	pop    %r8
	pop    %rdx
	pop    %rcx

	/*leaveq; c9 */
	mov    %rbp, %rsp /* 48 89 ec */
 	pop    %rbp		  /* 5d, restore the stack frame */
 	ret /* req; c3 */


/*
 Shift parameters by 2(JNIEnv *env, jclass cls/jobject obj) before calling the target `c function`.
 No need to save them, If we will not use them again.
 As to the paramters, the following is a layout snapshot of some registers and the stack frame.

https://msdn.microsoft.com/en-us/library/ew5tede7.aspx

ebp==> some high address
      +=======+=========+========================+========+============+
      | order | slot    | original               |        | shifted    |
      +=======+=========+========================+========+============+
more  |   N   | rsp+n   | parameterN             |
paras +-------+---------+------------------------+
on    |  ...                                     |
stack +-------+---------+------------------------+
      |   5   | rsp+8   | parameter3             |
----- +-------+---------+------------------------+
      |   4   | r9/XMM3 | parameter2             |        | parameter4 |
      +-------+---------+------------------------+        +------------+
      |   3   | r8/XMM2 | parameter1             |        | parameter3 |
      +-------+---------+------------------------+        +------------+
      |   2   | rdx     | jclass cls/jobject obj |        | parameter2 |
      +-------+---------+------------------------+        +------------+
      |   1   | rcx     | JNIEnv *env            |        | parameter1 |
------+-------+---------+------------------------+        +------------+
rsp==>|       | rsp     | return address         |
      +-------+---------+------------------------+

After `push %rbp; mov %rsp, %rbp`
*/

/** e.g. `native int sum(int a, int b)` in Java maps to `int sum(int a, int b)` in c */
st_interpret_native_direct_stdcall_x64:
	push   %rbp					/* 55 */
 	mov    %rsp, %rbp			/* 48 89 e5 */

	/* TODO consider alignment and float points */
	mov	   %r8, %rcx	/* shift the original 3th parameter to the first one `rcx` home  */
	mov	   %r9, %rdx	/* shift the original 4th parameter to the second one `rdx` home  */

	pop    %r8	/* shift the original 5th parameter to the third one `r8` home  */
	pop    %r9	/* shift the original 6th parameter to the fourth one `r9` home  */

	call   *%r10		/* r10: void* pfunc; it's a temporary setting */

	/*leaveq; c9 */
	mov    %rbp, %rsp /* 48 89 ec */
 	pop    %rbp		  /* 5d, restore the stack frame */

/*
                                   rsp +-----------------------+--o--
                                       | native_return_address | /|\
                                 rsp+8 +-----------------------+  |
                                       | old_xmm2_maybe_param1 |  |
                                rsp+16 +-----------------------+  |
                                       | old_xmm3_maybe_param2 |  |
                                rsp+24 +-----------------------+  |
                                       | old_r8_maybe_param1   |  |
rsp   +----------------------+  rsp+32 +-----------------------+  |
      |native_return_address |         | old_r9_maybe_param2   |slots
rsp+8 +----------------------+  rsp+40 +-----------------------+  |
      |java_method_param3    |         | java_method_param3    |  |
      +----------------------+         +-----------------------+  |
      |...                   |         | ...                   |  |
      +----------------------+         +-----------------------+  |
      |java_method_paramN    |         | java_method_paramN    | \|/
      +----------------------+         +-----------------------+--o--

      |<-- stack frame 1  -->|         |<--  stack frame 2  -->|

    @param env@rcx
    @param cls@rdx
    @param java_method_param1@r8/xmm2 maybe not exist
    @param java_method_param2@r9/xmm3 maybe not exist
    @param java_method_param3@[esp+8] maybe not exist
	@param java_method_paramN@[esp+8+8*(N-3)] maybe not exist
	#rax resolve_interpret_win64 or interpret_win64
	#r10 mid or callmeta
*/

stubthunk_interpret_win64:

	/* stack frame 1 */

	/* Refactor the stack frame about parameters and the return address(extra 4*8 bytes).
	  The area from `native_return_address` to `java_method_paramN` is called slots.
	*/

	sub $32, %rsp
	mov 32(%rsp), %rax /* save native_return_address */
	mov %rax, (%rsp)

	mov %r9, 32(%rsp) /* save the 3th and 4th parameters(maybe exist) on registers */
	mov %r8, 24(%rsp)
	movsd %xmm2, 16(%rsp)
	movsd %xmm3, 8(%rsp)

	/* stack frame 2 */

	mov %r10, %r8 /* here r10 is mid or callmeta */
	mov %rsp, %r9 /* slots */

	/* TODO here rax is one of
	  int resolve_interpret_win64(rcx_env, rdx_cls, mid, slots);
	  int interpret_win64(rcx_env, rdx_cls, callmeta, slots);
	  Regarding of win64 calling convention, no need stack maintain here.
	*/

	call *%rax

	/* same to stack frame 2 */

	/* %r8 is size in bytes of parameters on stack */
	mov (%rsp), %r9 /* r9 = native_return_address */

	/* refactor a scene for returning */
	add 40(%rsp, %r8, 1), %rsp /* now rsp is the address of paramN */
	mov %r9, (%rsp) /* [paramN] = r9 = native_return_address */

	ret

/*
    rdx +-----------------------+
        | java_method_param1    |
 rdx+8  +-----------------------+
        | java_method_param2    |
 rdx+16 +-----------------------+
        | java_method_param3    |
 rdx+24 +-----------------------+ new rsp +----------------------+
        | java_method_param4    |         |native_return_address |
 rdx+32 +-----------------------+   rsp+8 +----------------------+
        | java_method_param5    |         |java_method_param5    |
        +-----------------------+         +----------------------+
        | ...                   |         |...                   |
        +-----------------------+         +----------------------+
        | java_method_paramN    |         |java_method_paramN    |
        +-----------------------+         +----------------------+
   Refactor stack frame like calling the target function from
   native method and it will go to `rcx` and never return to the caller.

   Parameters
    rcx return address
    rdx pointer to all arguments via stack, which will be refactored here.
    r8 target function pointer
    r9 new rsp
*/
stubthunk_jmp_target_win64:
    /* set up call context */
	mov	%r8, %rax /* do rax = target_function_pointer */
	mov %r9, %rsp /* put new_rsp@r9 in RSP */

	mov 24(%rdx), %r9 /* param4 */

	mov %rcx, 24(%rdx) /* [virtual_param4_slot] = return_address@rcx */

	mov (%rdx), %rcx /* param1 */
	mov 16(%rdx), %r8 /* param3 */
	mov 8(%rdx), %rdx /* param2, behind other `mov` operations is due to using rdx */

	movd %rcx, %xmm0 /* if param1 is a float point  */
	movd %rdx, %xmm1 /* if param2 is a float point */
	movd %r8, %xmm2 /* if param3 is a float point  */
	movd %r9, %xmm3 /* if param4 is a float point  */

	jmp *%rax			/* Jump directly to the target function entry. */
