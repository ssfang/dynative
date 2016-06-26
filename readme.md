

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

