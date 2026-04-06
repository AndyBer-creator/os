
#ifndef __SAL_MIRROR_H__
#define __SAL_MIRROR_H__

#include <rsd/rsd_mirror.h>

#define UGID2GID(ugid) (ugid - 1)
#define GID2UGID(gid)  (gid + 1)

extern int32
sal_mirror_group_targetPort_get(sys_gid_t gid,sys_logic_port_t *pMirroring_port,sys_enable_t *pEnable);

extern int32
sal_mirror_group_targetPort_set(sys_gid_t gid,sys_logic_port_t mirroring_port,sys_enable_t enable);

extern int32
sal_mirror_group_igrState_get(sys_gid_t gid,sys_enable_t *pIgrState);

extern int32
sal_mirror_group_igrState_set(sys_gid_t gid,sys_enable_t igrState);

extern int32
sal_mirror_group_state_get(sys_gid_t gid,sys_mirror_state_t *pState);

extern int32
sal_mirror_group_sourcePort_get(sys_gid_t gid,
            sys_logic_portmask_t  *pMirrored_rx_portmask,
            sys_logic_portmask_t  *pMirrored_tx_portmask);

extern int32
sal_mirror_group_sourcePort_set(sys_gid_t gid,
            sys_logic_portmask_t  mirrored_rx_portmask,
            sys_logic_portmask_t  mirrored_tx_portmask);

extern int32
sal_mirror_group_del(sys_gid_t gid);

extern int32
sal_mirror_egrMode_get(sys_logic_port_t port,rsd_mirror_egrMode_t *pEgrMode);

extern int32
sal_mirror_egrMode_set(sys_logic_port_t port,rsd_mirror_egrMode_t egrMode);

#endif 

