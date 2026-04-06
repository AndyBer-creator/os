
#ifndef _INCLUDE_VOSPRINT_H_
#define _INCLUDE_VOSPRINT_H_
int vosPrintf(char *fmt, ...)
#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
;

int vosPrintConsole(char *fmt, ...)
#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
;

#endif 

