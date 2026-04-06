
#ifndef __RSD_STP_H__
#define __RSD_STP_H__

#include <rtk/stp.h>
#include <common/sys_def.h>

typedef rtk_stp_state_t sys_stp_state_t;
extern sys_enable_t gSTPEnable;

extern int32 rsd_stp_init(void);

extern int32 rsd_stp_enable_set(sys_enable_t enable, sys_enable_t forwardBpdu);

extern int32 rsd_stp_mstpInstance_create(uint32 msti);

extern int32 rsd_stp_mstpInstance_destroy(uint32 msti);

extern int32 rsd_stp_isMstpInstanceExist_get(uint32 msti, uint32 *pMsti_exist);

extern int32 rsd_stp_mstpState_get(uint32 msti, sys_logic_port_t port, sys_stp_state_t *pStp_state);

extern int32 rsd_stp_mstpState_set(uint32 msti, sys_logic_port_t port, sys_stp_state_t stp_state);

extern int32 rsd_stp_enable_get(sys_enable_t *pEnable);

#endif 

