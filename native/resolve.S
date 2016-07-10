
/* https://sourceforge.net/p/predef/wiki/Architectures */
/* #ifdef __LP64__ || _LP64 */
#if defined(__i386__) || defined(_M_IX86)
# define INCLUDE_resolve_x86
#elif defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
# define INCLUDE_resolve_x64
#else
# error Unsupported architecture
#endif
