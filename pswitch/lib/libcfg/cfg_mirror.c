
#include <libcfg/cfg.h>

int32 cfg_mirror_group_targetPort_get(sys_gid_t gid,sys_logic_port_t *pMirroring_port,sys_enable_t *pEnable)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK((NULL == pMirroring_port), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    *pMirroring_port = mirror->group[gid].mirroring_port;
    *pEnable = mirror->group[gid].mirroring_port_valid;

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_group_targetPort_set(sys_gid_t gid,sys_logic_port_t mirroring_port,sys_enable_t enable)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    mirror->group[gid].mirroring_port = mirroring_port;
    mirror->group[gid].mirroring_port_valid = enable;

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_group_igrState_get(sys_gid_t gid,sys_enable_t *pIgrState)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK((NULL == pIgrState), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    *pIgrState = (mirror->group[gid]).igrState;

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_group_igrState_set(sys_gid_t gid,sys_enable_t igrState)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    (mirror->group[gid]).igrState = igrState;

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_group_state_get(sys_gid_t gid,sys_mirror_state_t *pState)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    *pState = (mirror->group[gid]).state;

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_group_state_set(sys_gid_t gid,sys_mirror_state_t state)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    (mirror->group[gid]).state = state;

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_group_portMirror_get(sys_gid_t gid,sys_port_mirror_t *pPortMirror)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK((NULL == pPortMirror), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    osal_memcpy(pPortMirror,&(mirror->group[gid].port_mirror),sizeof(sys_port_mirror_t));

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_group_portMirror_set(sys_gid_t gid,sys_port_mirror_t portMirror)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    osal_memcpy(&(mirror->group[gid].port_mirror),&portMirror,sizeof(sys_port_mirror_t));

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32
cfg_mirror_group_del(sys_gid_t gid)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    osal_memset(&(mirror->group[gid]),0,sizeof(cfg_mirror_group_t));

    CFG_CLOSE(CFG_ID_MIRROR);
}

#if 0

int32 cfg_mirror_targetPort_get(sys_logic_port_t  *pMirroring_port, sys_enable_t *pEnable)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    *pMirroring_port = mirror->mirroring_port;
    *pEnable = mirror->mirroring_port_valid;

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_targetPort_set(sys_logic_port_t  mirroring_port, sys_enable_t enable)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    mirror->mirroring_port = mirroring_port;
    mirror->mirroring_port_valid = enable;

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_sourcePort_get(sys_logic_portmask_t  *pMirrored_rx_portmask, sys_logic_portmask_t  *pMirrored_tx_portmask)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(NULL == pMirrored_rx_portmask, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == pMirrored_tx_portmask, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    LOGIC_PORTMASK_COPY(*pMirrored_rx_portmask, mirror->mirrored_igrPorts);
    LOGIC_PORTMASK_COPY(*pMirrored_tx_portmask, mirror->mirrored_egrPorts);

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_sourcePort_set(sys_logic_portmask_t  *pMirrored_rx_portmask, sys_logic_portmask_t  *pMirrored_tx_portmask)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(NULL == pMirrored_rx_portmask, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == pMirrored_tx_portmask, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    LOGIC_PORTMASK_COPY(mirror->mirrored_igrPorts, *pMirrored_rx_portmask);
    LOGIC_PORTMASK_COPY(mirror->mirrored_egrPorts, *pMirrored_tx_portmask);

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_svlan_get(sys_vlanmask_t *pMirrored_vlanMask)
{
    cfg_mirror_t *mirror = NULL;

    SYS_PARAM_CHK(NULL == pMirrored_vlanMask, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    osal_memcpy(pMirrored_vlanMask,&(mirror->mirrored_vlanmask),sizeof(sys_vlanmask_t));

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_svlan_set(sys_vlanmask_t Mirrored_vlanMask)
{
    cfg_mirror_t *mirror = NULL;

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    osal_memcpy(&(mirror->mirrored_vlanmask),&Mirrored_vlanMask,sizeof(sys_vlanmask_t));

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_flow_get(uint32 entryIdx,sys_flow_mirror_t *pFlowMirrorcfg)
{
    cfg_mirror_t *mirror = NULL;

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    osal_memcpy(pFlowMirrorcfg,&(mirror->flow_mirror[entryIdx]),sizeof(sys_flow_mirror_t));

    CFG_CLOSE(CFG_ID_MIRROR);
}

int32 cfg_mirror_flow_set(uint32 entryIdx,sys_flow_mirror_t flowMirrorcfg)
{
    cfg_mirror_t *mirror = NULL;

    CFG_OPEN(CFG_ID_MIRROR, (cfg_addr)&mirror);

    osal_memcpy(&(mirror->flow_mirror[entryIdx]),&flowMirrorcfg,sizeof(sys_flow_mirror_t));

    CFG_CLOSE(CFG_ID_MIRROR);
}

#endif
