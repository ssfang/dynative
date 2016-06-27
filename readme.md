

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
#endif

.globl stubthunk_interpret_stdcall_x64

.text
mybar:
    call func
```

Also http://stackoverflow.com/questions/1034852/adding-leading-underscores-to-assembly-symbols-with-gcc-on-win32

### Compiling

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