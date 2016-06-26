#include "jn.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef _WIN32
# include <unistd.h>
# include <errno.h>
# include <sys/time.h>
# include <sys/ucontext.h>
/* mmap, mprotect, ...*/
# include <sys/mman.h>
/* [cacheflush](http://man7.org/linux/man-pages/man2/cacheflush.2.html) */
// #include <asm/cachectl.h>

#ifdef	_UNICODE
#define _tprintf wprintf
/*
 * UNICODE a constant string when _UNICODE is defined else returns the string
 * unmodified.  Also defined in w32api/winnt.h.
 */
#define _TEXT(x)	__TEXT(x)
#define	_T(x)		__TEXT(x)
#else
#define _tprintf printf
#define _TEXT(x) x
#define	_T(x) x
#endif

#else
#include <tchar.h>
#include <windows.h>
# endif

void dispatch(size_t argsize) asm("dispatch");
void dispatch(size_t argsize){
	//int argsize;
	_tprintf(_T("------dispatch, %d\n"), (int)argsize);
}


void stubthunk_call_test(void *stub) {

	// [Inline Assembler Overview](https://msdn.microsoft.com/en-us/library/5f7adz6y.aspx)
#ifdef _MSC_VER
	__asm {
		//MOV  ECX stub  ; prepare the function pointer
		//CALL ECX       ;
		CALL stub;
	}
#else
	__asm__ __volatile(
		"call *%0\n\t" /* the complier will gen like "movl	-16(%ebp), %eax; call %eax", indirect call with `*'*/
		: /* No output registers. */
		:"r" (stub)
		: /* clobbered register.  */
	);
#endif
}

//return NULL if failed.
void *alloc_code(size_t size){
	void *code;
#ifdef _WIN32
	//https://msdn.microsoft.com/en-us/library/windows/desktop/aa366887(v=vs.85).aspx
	code = VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	return code;
#else
	//http://man7.org/linux/man-pages/man2/mmap.2.html
	code = mmap(NULL, size, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if(MAP_FAILED != code){
		return code;
	}
	_tprintf(_T("errno(%d):%s\n"), errno, strerror(errno));
	return NULL;
#endif
}

/* allow to run code in memory */
int set_executable(void* addr, size_t size) {
#ifdef _WIN32
	unsigned long old_protect;
	if (VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &old_protect)) {
		// write block from data cache and flush from instruction cache
		//WINBASEAPI BOOL WINAPI FlushInstructionCache(
		// __in HANDLE hProcess,
		// __in_bcount_opt(dwSize) LPCVOID lpBaseAddress,
		// __in SIZE_T dwSize
		//);
		if(FlushInstructionCache(GetCurrentProcess(), addr, size))
			return 0;
	}
	TCHAR szMessageBuffer[128];
	DWORD_PTR dwError = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, "GetLastError = %1!x!, %0", dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), szMessageBuffer, 128, (char**) &dwError);
	//MessageBox(NULL, szMessageBuffer, _T("Error"), MB_OK);
	_tprintf(_T("GetLastError = %u, %s\n"), dwError, szMessageBuffer);
	return -1;
#else
	const int pagesize = sysconf(_SC_PAGE_SIZE);
	_tprintf(_T("pagesize = %d\n"), pagesize);
	/* https://en.wikipedia.org/wiki/Data_structure_alignment#Computing_padding
	 * 	padding = (align - (offset & (align - 1))) & (align - 1) = (-offset) & (align - 1)
	 * 	new offset = (offset + align - 1) & ~(align - 1)
	 */
	intptr_t start, end;
	start = REINTERPRET_CAST(intptr_t, addr) & ~(pagesize - 1); //align down to a page boundary
	end = REINTERPRET_CAST(intptr_t, addr) + size;
	end = (end + pagesize - 1) & ~(pagesize - 1); // align up to a page boundary
	/*
	 * mprotect() changes protection for the calling process's memory page(s) containing any part of
	 * the address range in the interval [addr, addr+len-1].  addr must be aligned to a page boundary.
	 * [mprotect](http://man7.org/linux/man-pages/man2/mprotect.2.html)
	 */
	if(0 == mprotect(REINTERPRET_CAST(void *, start), end - start, PROT_READ | PROT_WRITE | PROT_EXEC)){

	}else{
		_tprintf(_T("errno(%d):%s\n"), errno, strerror(errno));
		return -1;
	}
	/** Well, on windows-x64, I cannot change the memory executable using cygwin gcc on eclipse. */
	extern void __clear_cache(void *beginning, void *end);
	/* http://man7.org/linux/man-pages/man2/cacheflush.2.html */
	//cacheflush(addr, REINTERPRET_CAST(void *, addr) + size, 0);
#ifndef __PCC__
	__clear_cache(addr, REINTERPRET_CAST(void *, addr) + size);
#else
	/* pcc 1.2.0.DEVEL 20141206 don't have such proc */
#endif
#endif
	return 0;
}


/**
 * objcopy --only-keep-debug "${tostripfile.exe}" "${debugfile.pdb}"
 *
 * gcc.exe dynative.c resolve_x64.S -o dynative_cygwin.exe
 * i686-w64-mingw32-gcc.exe dynative.c resolve_x86.S -o dynative_win86.exe
 * x86_64-w64-mingw32-gcc.exe dynative.c resolve_x64.S -o dynative_win64.exe
 **/
/*
int main(int argc, char *argv[]) {
	void *p = (void*)0x1234567890123456;

	_tprintf(_T("sizeof(_stubthunk) = %d, %p\n"), (int) sizeof(stubthunk), p);

	mid_t mid = (mid_t) (argc == 2 ? strtoul(argv[1], NULL, 0) : 123456);

	stubthunk *stub = (stubthunk*)alloc_code(sizeof(stubthunk));
	assert(NULL != stub);

	stubthunk_init(stub, mid);
	stubthunk_call_test(stub);

	return 0;
}
*/

