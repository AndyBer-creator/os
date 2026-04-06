
#include <libcfg/cfg.h>

int32 cfg_lacp_enable_set(sys_enable_t enable)
{
    cfg_lacp_t *pLacp = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    pLacp->enable = enable;

    CFG_CLOSE(CFG_ID_LACP);
}

int32 cfg_lacp_enable_get(sys_enable_t *pEnable)
{
    cfg_lacp_t *pLacp = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    *pEnable = pLacp->enable;

    CFG_CLOSE(CFG_ID_LACP);
}

int32 cfg_lacp_system_priority_set(uint32 sysPri)
{
    cfg_lacp_t *pLacp = NULL;

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    pLacp->system_priority = sysPri;

    CFG_CLOSE(CFG_ID_LACP);
}

int32 cfg_lacp_system_priority_get(uint32 *pSysPri)
{
    cfg_lacp_t *pLacp = NULL;

    SYS_PARAM_CHK(NULL == pSysPri, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    *pSysPri = pLacp->system_priority;

    CFG_CLOSE(CFG_ID_LACP);
}

int32 cfg_lacp_portPriority_set(sys_logic_port_t port, uint16 pri)
{
    cfg_lacp_t *pLacp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    pLacp->portPri[port] = pri;

    CFG_CLOSE(CFG_ID_LACP);
}

int32 cfg_lacp_portPriority_get(sys_logic_port_t port, uint16 *pPri)
{
    cfg_lacp_t *pLacp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pPri, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    *pPri = pLacp->portPri[port];

    CFG_CLOSE(CFG_ID_LACP);
}

int32 cfg_lacp_portTimeout_set(sys_logic_port_t port, sys_lacp_timeout_t timeout)
{
    cfg_lacp_t *pLacp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    pLacp->portTimeout[port] = timeout;

    CFG_CLOSE(CFG_ID_LACP);
}

int32 cfg_lacp_portTimeout_get(sys_logic_port_t port, sys_lacp_timeout_t *pTimeout)
{
    cfg_lacp_t *pLacp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pTimeout, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    *pTimeout = pLacp->portTimeout[port];

    CFG_CLOSE(CFG_ID_LACP);
}

int32 cfg_lacp_activeMode_set(uint32 trunkId, sys_logic_portmask_t *pActPm)
{
    cfg_lacp_t *pLacp = NULL;

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pActPm), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    pLacp->actPm[trunkId] = *pActPm;

    CFG_CLOSE(CFG_ID_LACP);
}

int32 cfg_lacp_activeMode_get(uint32 trunkId, sys_logic_portmask_t *pActPm)
{
    cfg_lacp_t *pLacp = NULL;

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pActPm), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LACP, (cfg_addr)&pLacp);

    *pActPm = pLacp->actPm[trunkId];

    CFG_CLOSE(CFG_ID_LACP);
}

