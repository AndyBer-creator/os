
#ifndef __CFG_MIRROR_H__
#define __CFG_MIRROR_H__

#include <rsd/rsd_mirror.h>

typedef struct cfg_mirror_group_s
{
    sys_enable_t          mirroring_port_valid;
    sys_logic_port_t      mirroring_port;
    sys_enable_t          igrState;

    sys_mirror_state_t    state;
    union {
        sys_port_mirror_t     port_mirror;
    };
}cfg_mirror_group_t;

typedef struct cfg_mirror_s
{
    cfg_mirror_group_t  group[CAPA_MIRROR_SESSION_NUM_MAX];
} cfg_mirror_t;

extern int32
cfg_mirror_group_targetPort_get(sys_gid_t gid,sys_logic_port_t *pMirroring_port,sys_enable_t *pEnable);

extern int32
cfg_mirror_group_targetPort_set(sys_gid_t gid,sys_logic_port_t mirroring_port,sys_enable_t enable);

extern int32
cfg_mirror_group_igrState_get(sys_gid_t gid,sys_enable_t *pIgrState);

extern int32
cfg_mirror_group_igrState_set(sys_gid_t gid,sys_enable_t igrState);

extern int32
cfg_mirror_group_state_get(sys_gid_t gid,sys_mirror_state_t *pState);

extern int32
cfg_mirror_group_state_set(sys_gid_t gid,sys_mirror_state_t state);

extern int32
cfg_mirror_group_portMirror_get(sys_gid_t gid,sys_port_mirror_t *pPortMirror);

extern int32
cfg_mirror_group_portMirror_set(sys_gid_t gid,sys_port_mirror_t portMirror);

extern int32
cfg_mirror_group_del(sys_gid_t gid);

#if 0

extern int32 cfg_mirror_targetPort_get(sys_logic_port_t  *pMirroring_port, sys_enable_t *pEnable);

extern int32 cfg_mirror_targetPort_set(sys_logic_port_t  mirroring_port, sys_enable_t enable);

extern int32 cfg_mirror_sourcePort_get(sys_logic_portmask_t  *pMirrored_rx_portmask, sys_logic_portmask_t  *pMirrored_tx_portmask);

extern int32 cfg_mirror_sourcePort_set(sys_logic_portmask_t  *pMirrored_rx_portmask, sys_logic_portmask_t  *pMirrored_tx_portmask);

#endif

#endif

