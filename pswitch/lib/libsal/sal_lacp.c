
#include <libsal/sal_util.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_lacp.h>

static int32 _sal_lacp_group_valid_check(
    sys_logic_portmask_t *pPortmask,
    sys_logic_portmask_t *pActmask);

static int32 _sal_lacp_group_valid_check(
    sys_logic_portmask_t *pPortmask,
    sys_logic_portmask_t *pActmask)
{
    sys_logic_portmask_t resultPm;

    LOGIC_PORTMASK_ANDNOT(resultPm, *pActmask, *pPortmask);

    if (!IS_LOGIC_PORTMASK_CLEAR(resultPm))
        return SYS_ERR_LACP_ACTIVE_PORTS;

    return SYS_ERR_OK;
}

int32 sal_lacp_portPriority_set(sys_logic_port_t port, uint16 pri)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(lacp_portPriority_set(port, pri));

    SYS_ERR_CHK(cfg_lacp_portPriority_set(port, pri));

    return SYS_ERR_OK;
}

int32 sal_lacp_portPriority_get(sys_logic_port_t port, uint16 *pPri)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(cfg_lacp_portPriority_get(port, pPri));

    return SYS_ERR_OK;
}

int32 sal_lacp_portTimeout_set(sys_logic_port_t port, sys_lacp_timeout_t timeout)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(lacp_portTimeout_set(port, timeout));

    SYS_ERR_CHK(cfg_lacp_portTimeout_set(port, timeout));

    return SYS_ERR_OK;
}

int32 sal_lacp_portTimeout_get(sys_logic_port_t port, sys_lacp_timeout_t *pTimeout)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(cfg_lacp_portTimeout_get(port, pTimeout));

    return SYS_ERR_OK;
}

int32 sal_lacp_activeMode_set(uint32 trunkId, sys_logic_portmask_t *pActPm)
{
    char activeportstr[CAPA_PORTMASK_STR_LEN];
    char passiveportstr[CAPA_PORTMASK_STR_LEN];
    sys_logic_port_t lp;
    sys_logic_portmask_t mbrPm;
    sys_logic_portmask_t passivePm;

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pActPm), SYS_ERR_NULL_POINTER);

    osal_memset(activeportstr, 0, sizeof(activeportstr));
    osal_memset(passiveportstr, 0, sizeof(passiveportstr));
    osal_memset(&mbrPm, 0, sizeof(mbrPm));
    osal_memset(&passivePm, 0, sizeof(passivePm));

    SYS_ERR_CHK(cfg_trunk_member_get(trunkId, &mbrPm));

    SYS_ERR_CHK(_sal_lacp_group_valid_check (&mbrPm, pActPm));

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, mbrPm)
    {
        if (IS_LOGIC_PORTMASK_PORTSET(*pActPm, lp))
            lacp_portActivity_set(lp, TRUE);
        else
        {
            lacp_portActivity_set(lp, FALSE);
            LOGIC_PORTMASK_SET_PORT(passivePm, lp);
        }
    }

    SYS_ERR_CHK(cfg_lacp_activeMode_set(trunkId, pActPm));

    LPM2STR(pActPm, activeportstr);
    SYS_LOG(LOG_CAT_LAG, LOG_LAG_LACP_ACTIVE, activeportstr);

    if (!IS_LOGIC_PORTMASK_CLEAR(passivePm))
    {
        LPM2STR(&passivePm, passiveportstr);
        SYS_LOG(LOG_CAT_LAG, LOG_LAG_LACP_PASSIVE, passiveportstr);
    }

    return SYS_ERR_OK;
}

int32 sal_lacp_activeMode_get(uint32 trunkId, sys_logic_portmask_t *pActPm)
{
    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pActPm), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lacp_activeMode_get(trunkId, pActPm));

    return SYS_ERR_OK;
}

int32 sal_lacp_member_set(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t oldMbrPm;
    sys_logic_portmask_t actPm;

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);

    osal_memset(&actPm, 0, sizeof(actPm));

    SYS_ERR_CHK(cfg_trunk_member_get(trunkId, &oldMbrPm));

    SYS_ERR_CHK(cfg_lacp_activeMode_get(trunkId, &actPm));

    if (!IS_NORMAL_LOGIC_PORTMASK_CLEAR(oldMbrPm) && !IS_TRK_LACP(trunkId))
        return SYS_ERR_LACP_STATIC_TRUNK_GROUP;

    SYS_ERR_CHK(sal_trunk_memberNum_check(SYS_PORT_TRUNK_LACP, pPortmask));

    SYS_ERR_CHK(sal_trunk_valid_check(trunkId, pPortmask));

    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        if (IS_LOGIC_PORTMASK_PORTSET(*pPortmask, logicPort)
                && !IS_LOGIC_PORTMASK_PORTSET(oldMbrPm, logicPort))
        {
            SYS_ERR_CHK(sal_trunk_portRole_change(TRK2LP(trunkId), logicPort));
        }
    }

    SYS_ERR_CHK(lacp_group_set(trunkId, pPortmask, &actPm));

    SYS_ERR_CHK(rsd_trunk_lacpGroup_set(trunkId, pPortmask));

    SYS_ERR_CHK(cfg_trunk_member_set(trunkId, pPortmask));

    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        if (IS_LOGIC_PORTMASK_PORTSET(oldMbrPm, logicPort)
                && !IS_LOGIC_PORTMASK_PORTSET(*pPortmask, logicPort))
        {
            SYS_ERR_CHK(sal_trunk_portRole_change(logicPort, logicPort));
        }
    }

    SYS_ERR_CHK(sal_trunk_setting_refresh(trunkId));

    return SYS_ERR_OK;
}

int32 sal_lacp_backupMember_get(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pPortmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lacp_backupMember_get(trunkId, pPortmask));

    return SYS_ERR_OK;
}

int32 sal_lacp_group_del(uint32 trunkId)
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t mbrPm;

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);

    SYS_ERR_CHK(rsd_trunk_lacpGroup_del(trunkId));
    SYS_ERR_CHK(lacp_group_del(trunkId));

    osal_memset(&mbrPm, 0, sizeof(mbrPm));
    SYS_ERR_CHK(cfg_trunk_member_get(trunkId, &mbrPm));

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, mbrPm)
    {
        
        SYS_ERR_CHK(sal_trunk_portRole_change(logicPort, logicPort));
    }

    osal_memset(&mbrPm, 0, sizeof(mbrPm));
    SYS_ERR_CHK(cfg_trunk_member_set(trunkId, &mbrPm));
    SYS_ERR_CHK(cfg_lacp_activeMode_set(trunkId, &mbrPm));

    SYS_ERR_CHK(sal_trunk_setting_refresh(trunkId));

    return SYS_ERR_OK;
}

int32 sal_lacp_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(lacp_enable_set(enable));
    SYS_ERR_CHK(cfg_lacp_enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_lacp_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lacp_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_lacp_systemPriority_set(uint32 sysPri)
{
    SYS_PARAM_CHK((sysPri > 65535), SYS_ERR_LACP_SYSTEM_PRIORITY);

    SYS_ERR_CHK(lacp_system_priority_set(sysPri));
    SYS_ERR_CHK(cfg_lacp_system_priority_set(sysPri));

    return SYS_ERR_OK;
}

int32 sal_lacp_systemPriority_get(uint32 *pSysPri)
{
    SYS_PARAM_CHK((NULL == pSysPri), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lacp_system_priority_get(pSysPri));

    return SYS_ERR_OK;
}

int32 sal_lacp_portState_get(sys_logic_port_t port, sys_lacp_port_t *pPortState)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPortState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lacp_port_state_get(port, pPortState));

    return SYS_ERR_OK;
}

int32 sal_lacp_portCounter_get(sys_logic_port_t port, sys_lacp_counter_t *pPortCounter)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPortCounter), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lacp_portCounter_get(port, pPortCounter));

    return SYS_ERR_OK;
}

int32 sal_lacp_portCounter_reset(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(lacp_portCounter_reset(port));

    return SYS_ERR_OK;
}

char *sal_lacp_select_state_to_str_ret(sys_mosm_t mosm_state)
{
    if (mosm_state & MOSM_SELECTED_YES)
        return "S";
    else if (mosm_state & MOSM_SELECTED_STANDBY)
        return "D";
    else
        return "U";

    return NULL;
}

char *sal_lacp_mux_state_to_str_ret(sys_muxm_state_t mux)
{
    switch (mux)
    {
        case DETACHED:
            return "DETACH";

        case WAITING:
            return "WAIT";

        case ATTACHED:
            return "ATTACH";

        case COLLECTING:
            return "CLLCT";

        case DISTRIBUTING:
            return "DSTRBT";

        default:
            return "?";
    }

    return NULL;
}

char *sal_lacp_rcv_state_to_str_ret(sys_rcvm_state_t rcv)
{
    switch (rcv)
    {
        case INITIALIZE:
            return "INIT";

        case PORT_DISABLED:
            return "PORTds";

        case EXPIRED:
            return "EXPR";

        case LACP_DISABLED:
            return "LACPds";

        case DEFAULTED:
            return "DFLT";

        case CURRENT:
            return "CRRNT";

        default:
            return "?";
    }

    return NULL;
}

char *sal_lacp_perio_state_to_str_ret(sys_perio_state_t perio)
{
    switch (perio)
    {
        case NO_PERIODIC:
            return "NoPRD";

        case FAST_PERIODIC:
            return "FstPRD";

        case SLOW_PERIODIC:
            return "SlwPRD";

        case PERIODIC_TX:
            return "PrdTX";

        default:
            return "?";
    }

    return NULL;
}

