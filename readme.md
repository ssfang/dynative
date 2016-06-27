

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



[jps]: http://docs.oracle.com/javase/7/docs/technotes/tools/share/jps.html "jps - Java Virtual Machine Process Status Tool"