
#ifndef __RSD_LOOPBACK_H__
#define __RSD_LOOPBACK_H__

#include <rtk/stp.h>
#include <common/sys_def.h>

extern sys_enable_t gLOOPBACKEnable;

typedef struct sys_loopback_event_s
{
    sys_notifier_subject_loopback_event_t event;
	uint32 port;
} sys_loopback_event_t;

extern int32 rsd_loopback_init(void);

extern int32 rsd_loopback_enable_set(sys_enable_t enable);

extern int32 rsd_stp_enable_get(sys_enable_t *pEnable);

extern int32 rsd_loopback_ints_cfg_notice(sys_logic_port_t port, uint32 isEnable);

#endif 

