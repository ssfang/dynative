

# My note

## development

### Files


* **file.s**         Assembler code.
* **file.S, file.sx**  Assembler code that must be preprocessed.

Also https://gcc.gnu.org/onlinedocs/gcc/Overall-Options.html

### C symbols will lead an underscore than asm

use asm labels in `c` file, see[5.39 Controlling Names Used in Assembler Code](https://gcc.gnu.org/onlinedocs/gcc-4.4.0/gcc/Asm-Labels.html#Asm-Labels)
```c
 // It will be referred by this symbol `myfoo` in asm, `foo` may be mangled as `_foo` after complied
 int foo asm ("myfoo") = 2;
 // It will be referred by this symbol `mybar` in asm
 extern int bar(int x) asm("mybar"); //only definition

void func() asm("func"); // only function declaration
void func(){ // definition
}
```
In `asm`
```
#if defined(HAVE_ASM_USCORE)
# define SYMBOL_NAME(s) _##s
#else
# define SYMBOL_NAME(s) s
#endif420

.globl stubthunk_interpret_stdcall_x64

.text
mybar:
    call func
```

Also http://stackoverflow.com/questions/1034852/adding-leading-underscores-to-assembly-symbols-with-gcc-on-win32

### Compiling

The java head file or exactly jdk is needed.  `${varname}` is ok both in makefile and shell. 

For example,

In cygwin console:
```
x86_64-w64-mingw32-gcc -I${JAVA_HOME}/include -I${JAVA_HOME}/include/win32 dynative.c jn.cpp resolve_x64.S -shared -o jn-x64.dll
i686-w64-mingw32-gcc -I${JAVA_HOME}/include -I${JAVA_HOME}/include/win32 dynative.c jn.cpp resolve_x64.S -shared -o jn-x86.dll

"/cygdrive/d/ProgramFiles/Microsoft Visual Studio 12.0/VC/bin/dumpbin.exe" /DEPENDENTS jn-x86.dll
"/cygdrive/d/ProgramFiles/Microsoft Visual Studio 12.0/VC/bin/dumpbin.exe" /EXPORTS jn-x86.dll
```

In cmd console:
```cmd
x86_64-w64-mingw32-gcc -I%JAVA_HOME%/include -I%JAVA_HOME%/include/win32 dynative.c jn.cpp resolve_x64.S -shared -o jn-x64.dll
i686-w64-mingw32-gcc -I%JAVA_HOME%/include -I%JAVA_HOME%/include/win32 dynative.c jn.cpp resolve_x64.S -shared -o jn-x86.dll

"D:\ProgramFiles\Microsoft Visual Studio 12.0\VC\bin\dumpbin.exe" /DEPENDENTS jn-x86.dll
```
ps
```ps
$files=ls dynative -Recurse -Force
$ctime=$(Get-Date "2016-02-22 22:22:22")
foreach($f in $files)
{
    $f.LastWriteTime=$ctime.Add($f.LastWriteTime-$f.CreationTime)
    $f.LastAccessTime=$ctime.Add($f.LastAccessTime-$f.CreationTime)
    $f.CreationTime=$ctime
}
```

### Debug

First, add a breakpointer before the native method is called in eclipse. Then, using one of jdk tools, [jps], to list java processes via win cmd rather than cygwin term.
Or procexp.exe. For example: 
```
C:\Users\fang>jps
8784 DynativeMain
6132
7832 Jps

C:\Users\fang>where printf
C:\cygwin\bin\printf.exe

C:\Users\fang>printf "%x\n" 8784
2250
```
However, cygwin gdb in eclipse or cmd and x64dbg cannot attach the process, I don't know the real reason. (runas administator on windows 10 x64).
```
C:\Users\fang>gdb --pid=8784
GNU gdb (GDB) (Cygwin 7.10.1-1) 7.10.1
...
Can't attach to process.


C:\Users\fang>ps --help
Usage: ps [-aefls] [-u UID] [-p PID]

Report process status

 -a, --all       show processes of all users
 -e, --everyone  show processes of all users
 -f, --full      show process uids, ppids
 -h, --help      output usage information and exit
 -l, --long      show process uids, ppids, pgids, winpids
 -p, --process   show information for specified PID
 -s, --summary   show process summary
 -u, --user      list processes owned by UID
 -V, --version   output version information and exit
 -W, --windows   show windows as well as cygwin processes

With no options, ps outputs the long format by default
```
So, windbg is a good choice on windows.

## Document

### JNI

[Resolving Native Method Names](http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/design.html#wp615) mentions  
> Both the native methods and the interface APIs follow the standard library-calling convention on a given platform. For example, UNIX systems use the `C` calling convention, while Win32 systems use `__stdcall`.  

### On unix


### On windows

* [__stdcall](https://msdn.microsoft.com/en-us/library/zxk0tw93.aspx)
> **Argument-passing**: order Right to left.  
> **Argument-passing convention**: By value, unless a pointer or reference type is passed.  
> **Stack-maintenance responsibility**: Called function pops its own arguments from the stack.  
> **Name-decoration convention**: An underscore (_) is prefixed to the name. The name is followed by the at sign (@) followed by the number of bytes (in decimal) in the argument list. Therefore, the function declared as `int func( int a, double b )` is decorated as follows: `_func@12`  
> **Case-translation convention**: None  

* [__cdecl](https://msdn.microsoft.com/en-us/library/zkwh89ks.aspx)
> Argument-passing order: Right to left. The stack is cleaned up by the caller, it can do vararg functions.   
> On ARM and x64 processors, `__cdecl` is accepted but typically ignored by the compiler. By convention on ARM and x64, arguments are passed in registers when possible, and subsequent arguments are passed on the stack. In x64 code, use `__cdecl` to override the `/Gv` compiler option and use the default x64 calling convention.

* [Floating Point Coprocessor and Calling Conventions](https://msdn.microsoft.com/en-us/library/ha59cbfz.aspx)
> If you are writing assembly routines for the floating point coprocessor, you must preserve the floating point control word and clean the coprocessor stack unless you are returning a float or double value (which your function should return in ST(0)).

#### x64

* [x64 Software Conventions](https://msdn.microsoft.com/en-us/library/7kcdt6fy.aspx), [Parameter Passing](https://msdn.microsoft.com/en-us/library/zthk2dkh.aspx)
> **Floating point**: First 4 parameters – XMM0 through XMM3. Others passed on stack.  
> **Integer**: First 4 parameters – RCX, RDX, R8, R9. Others passed on stack.  
> **Aggregates (8, 16, 32, or 64 bits) and `__m64`**: First 4 parameters – RCX, RDX, R8, R9. Others passed on stack.   
> **Aggregates (other)**: By pointer. First 4 parameters passed as pointers in RCX, RDX, R8, and R9  
> **`__m128`**: By pointer. First 4 parameters passed as pointers in RCX, RDX, R8, and R9  

* [Floating-Point Support for Older Code (Visual C++)](https://msdn.microsoft.com/en-us/library/a32tsf7t.aspx)
> The MMX and floating-point stack registers (MM0-MM7/ST0-ST7) are preserved across context switches. There is no explicit calling convention for these registers. The use of these registers is strictly prohibited in kernel mode code.

* [Return Values (C++)](https://msdn.microsoft.com/en-us/library/7572ztz4.aspx). Basically: 
> A scalar return value that can fit into 64 bits is returned through `RAX`—this includes `__m64` types.   
> Non-scalar types including floats, doubles, and vector types such as `__m128`, `__m128i`, `__m128d` are returned in XMM0.   
> The state of unused bits in the value returned in RAX or XMM0 is undefined.  


#### [Naked Function Calls](https://msdn.microsoft.com/en-us/library/5ekezyy2.aspx)
* [Rules and Limitations for Naked Functions](https://msdn.microsoft.com/en-us/library/4d12973a.aspx)
* [naked (C++)](https://msdn.microsoft.com/en-us/library/h5w10wxs.aspx) mentions : 
> Note that the naked attribute is only valid on x86 and ARM, and is not available on x64.
* [Considerations for Writing Prolog/Epilog Code](https://msdn.microsoft.com/en-us/library/6xy06s51.aspx)


### Other documents

* gnu [6.31 Declaring Attributes of Functions](https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html)
* [x86 Disassembly/Calling Conventions](https://en.wikibooks.org/wiki/X86_Disassembly/Calling_Conventions)
* [x86 calling conventions](https://en.wikipedia.org/wiki/X86_calling_conventions)

[jps]: http://docs.oracle.com/javase/7/docs/technotes/tools/share/jps.html "jps - Java Virtual Machine Process Status Tool"
