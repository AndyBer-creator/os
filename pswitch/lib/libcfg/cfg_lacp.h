
#ifndef __CFG_LACP_H__
#define __CFG_LACP_H__

#include <libsal/sal_lacp.h>

typedef struct cfg_lacp_s
{
    sys_enable_t         enable;
    uint32               system_priority;
    sys_logic_portmask_t actPm[CAPA_LAG_MBR_NORMAL_NUM];
    uint16               portPri[SYS_NORMALPORT_NUM_MAX];
    sys_lacp_timeout_t   portTimeout[SYS_NORMALPORT_NUM_MAX];
} cfg_lacp_t;

extern int32 cfg_lacp_enable_set(sys_enable_t enable);

extern int32 cfg_lacp_enable_get(sys_enable_t *pEnable);

extern int32 cfg_lacp_system_priority_set(uint32 system_priority);

extern int32 cfg_lacp_system_priority_get(uint32 *pSysPri);

extern int32 cfg_lacp_portPriority_set(sys_logic_port_t port, uint16 pri);

extern int32 cfg_lacp_portPriority_get(sys_logic_port_t port, uint16 *pPri);

extern int32 cfg_lacp_portTimeout_set(sys_logic_port_t port, sys_lacp_timeout_t timeout);

extern int32 cfg_lacp_portTimeout_get(sys_logic_port_t port, sys_lacp_timeout_t *pTimeout);

extern int32 cfg_lacp_activeMode_set(uint32 trunkId, sys_logic_portmask_t *pActPm);

extern int32 cfg_lacp_activeMode_get(uint32 trunkId, sys_logic_portmask_t *pActPm);

#endif

