/*
 * $Id: sprintf_irc.h 33 2010-05-10 02:46:01Z openglx $ 
 */

#ifndef SPRINTF_IRC
#define SPRINTF_IRC

#include <stdarg.h>

/*=============================================================================
 * Proto types
 */

extern int vsprintf_irc(register char *str, register const char *format,
    register va_list);
extern int ircsprintf(register char *str, register const char *format, ...);

#endif /* SPRINTF_IRC */
