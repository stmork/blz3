#ifndef __OS_H
#define __OS_H

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
# define SYS_WINDOWS
# if defined __MINGW32__
#  define COMP_MINGW
# elif defined __CYGWIN__
#  define COMP_CYGWIN
# else
#  define COMP_MSC
# endif
#elif defined(__APPLE__)
# define SYS_APPLE
# define COMP_GCC
# define SYS_UNIX
# define SYS_BSD
#else
# define COMP_GCC
# define SYS_UNIX
# if defined(__bsdi__) || defined(__FreeBSD__) 
#  define SYS_BSD
# else
#  define SYS_LINUX
# endif
#endif

#if 0
#if !defined(COMP_CYGWIN)
#include <stdint.h>
#endif // !COMP_CYGWIN
#endif

#endif
