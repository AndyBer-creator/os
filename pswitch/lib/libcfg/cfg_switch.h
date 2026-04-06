
#ifndef __CFG_SWITCH_H__
#define __CFG_SWITCH_H__

#include <libsal/sal_switch.h>

typedef struct cfg_switch_s
{
    uint32 maxFrameSize;
} cfg_switch_t;

extern int32 cfg_switch_maxFrameSize_set(uint32 maxSize);

extern int32 cfg_switch_maxFrameSize_get(uint32 *pMaxSize);
#endif

