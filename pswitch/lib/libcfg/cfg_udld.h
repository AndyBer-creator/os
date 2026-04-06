
#ifndef __CFG_UDLD_H__
#define __CFG_UDLD_H__

#include <udld/proto_udld.h>

typedef struct cfg_udld_s
{
    sys_udld_mode_t portMode[SYS_LOGICPORT_NUM_MAX];
    uint32       messageTime;
} cfg_udld_t;

extern int32 cfg_udld_messageTime_set(uint32 interval);

extern int32 cfg_udld_messageTime_get(uint32 *pInterval);

extern int32 cfg_udld_portMode_set(sys_logic_port_t port, sys_udld_mode_t mode);

extern int32 cfg_udld_portMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode);
#endif

