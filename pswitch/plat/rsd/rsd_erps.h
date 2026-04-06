
#ifndef __RSD_ERPS_H__
#define __RSD_ERPS_H__

#include <rtk/stp.h>
#include <common/sys_def.h>

typedef rtk_stp_state_t sys_erps_state_t;
extern sys_enable_t gERPSEnable;

typedef struct sys_erps_event_s
{
    sys_notifier_subject_erps_event_t event;
    uint32 erps_inst;
    sys_logic_portmask_t portmask;
} sys_erps_event_t;

extern int32 rsd_erps_init(void);

extern int32 rsd_erps_enable_set(sys_enable_t enable);

extern int32 rsd_erps_port_get(int portId);

extern int32 rsd_erps_port_set(int portId,int status);

#if 0

extern int32 rsd_erps_mstpInstance_create(uint32 msti);

extern int32 rsd_erps_mstpInstance_destroy(uint32 msti);

extern int32 rsd_erps_isMstpInstanceExist_get(uint32 msti, uint32 *pMsti_exist);

extern int32 rsd_stp_mstpState_get(uint32 msti, sys_logic_port_t port, sys_stp_state_t *pStp_state);

extern int32 rsd_stp_mstpState_set(uint32 msti, sys_logic_port_t port, sys_stp_state_t stp_state);
#endif

extern int32 rsd_stp_enable_get(sys_enable_t *pEnable);

extern int32 rsd_erps_ints_cfg_notice(uint32 ints,  sys_logic_port_t lport, uint8 IsAdd);

#endif 

