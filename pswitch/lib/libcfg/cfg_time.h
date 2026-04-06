
#ifndef __CFG_TIME_H__
#define __CFG_TIME_H__

#include <libsal/sal_time.h>

typedef struct cfg_time_s
{
    
    sys_time_t systime;
} cfg_time_t;

extern int32 cfg_time_set(sys_time_t *systime);

extern int32 cfg_time_sntp_reason_set(sys_time_t *systime);

extern int32 cfg_time_get(sys_time_t *systime);
#endif

