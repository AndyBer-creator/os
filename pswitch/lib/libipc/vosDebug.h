
#ifndef _INCLUDE_VOSDEBUG_H_
#define _INCLUDE_VOSDEBUG_H_
typedef enum VOS_DEBUG_FUN_tag{
    VOS_DEBUG_NO_TIMEOUT        = 0x01, 
    VOS_DEBUG_ERR               = 0x02, 
    VOS_DEBUG_DETAIL            = 0x04,  
    VOS_DEBUG_PACKET            = 0x08, 
    VOS_DEBUG_TIMER             = 0x10, 
    VOS_DEBUG_TRAP              = 0x20, 
    VOS_DEBUG_ALL               = 0xFFFFFFFF  
}VOS_DEBUG_FUN;

int vosDebugConfig(ulong_t ulFlagsBitMask);

int vosMsgDebugVos(char *name, ulong_t ulFlagsBitMask);

int vosMsgDebugSmd(char *name, ulong_t ulFlagsBitMask);

int vosDebugPrintf(VOS_DEBUG_FUN fun, char *fmt, ...)
#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
;

#endif 

