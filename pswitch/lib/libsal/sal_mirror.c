
#include <libsal/sal_util.h>
#include <libsal/sal_mirror.h>

static int32 sal_mirror_group_target_chk(sys_gid_t cur_gid,sys_logic_port_t mirroring_port)
{
    sys_gid_t gid;
    sys_logic_port_t logicPort;
    sys_enable_t enable;
    sys_mirror_state_t state;
    sys_logic_portmask_t rxPortmask,txPortmask;

    SYS_PARAM_CHK(IS_GID_INVALID(cur_gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(mirroring_port), SYS_ERR_PORT_ID);

    osal_memset(&rxPortmask, 0, sizeof(rxPortmask));
    osal_memset(&txPortmask, 0, sizeof(txPortmask));

    if(IS_LP_TRKMBR(mirroring_port))
        return SYS_ERR_MIRROR_TRUNK_MEMBER;
    if(!IS_LP_NML(mirroring_port))
        return SYS_ERR_MIRROR_TRUNK;

    FOR_EACH_GID(gid)
    {
        SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid,&logicPort,&enable));
        if(ENABLED == enable)
        {
#if 0
            if((gid == cur_gid) && (mirroring_port != logicPort))
                return SYS_ERR_MIRROR_TARGET_EXIST;
#endif

            if((gid != cur_gid) && (mirroring_port == logicPort))
                return SYS_ERR_MIRROR_TARGET_EXIST;
        }

        SYS_ERR_CHK(sal_mirror_group_state_get(gid,&state));
        if(PORTBASED_ENABLED == state)
        {
            SYS_ERR_CHK(sal_mirror_group_sourcePort_get(gid,&rxPortmask, &txPortmask));

            if (IS_LOGIC_PORTMASK_PORTSET(rxPortmask, mirroring_port))
                return SYS_ERR_MIRROR_SELF_MIRROR;

            if (IS_LOGIC_PORTMASK_PORTSET(txPortmask, mirroring_port))
                return SYS_ERR_MIRROR_SELF_MIRROR;
        }
    }

#ifdef CONFIG_SYS_PROTO_GVRP
    SYS_ERR_CHK(sal_gvrp_enable_get(&enable));
    if(ENABLED == enable)
    {
        SYS_ERR_CHK(sal_gvrp_port_enable_get(mirroring_port,&enable));
        if(ENABLED == enable)
            return SYS_ERR_MIRROR_GVRP_ENABLE;
    }
#endif

    return SYS_ERR_OK;

}

int32 sal_mirror_group_targetPort_get(sys_gid_t gid,sys_logic_port_t *pMirroring_port,sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK((NULL == pMirroring_port), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_mirror_group_targetPort_get(gid,pMirroring_port,pEnable));

    return SYS_ERR_OK;
}

int32 sal_mirror_group_targetPort_set(sys_gid_t gid, sys_logic_port_t mirroring_port, sys_enable_t enable)
{
    sys_logic_port_t port;
    sys_mirror_state_t state;
    rsd_mirror_entry_t  mirrorEntry;
    sys_logic_portmask_t  mirrored_rx_portmask;
    sys_logic_portmask_t  mirrored_tx_portmask;
  #ifdef CONFIG_SYS_PROTO_STP
    sys_enable_t enableTmp;
  #endif

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(mirroring_port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    if(ENABLED == enable)
    {
        SYS_ERR_CHK(sal_mirror_group_target_chk(gid,mirroring_port));
        SYS_ERR_CHK(sal_mirror_group_state_get(gid,&state));

#ifdef CONFIG_SYS_PROTO_STP
        SYS_ERR_CHK(stp_portAdminEnable_set(mirroring_port, DISABLED));
#endif

        LOGIC_PORTMASK_CLEAR_ALL(mirrored_rx_portmask);
        LOGIC_PORTMASK_CLEAR_ALL(mirrored_tx_portmask);
        osal_memset(&mirrorEntry, 0, sizeof(rsd_mirror_entry_t));

        if (MIRROR_DISABLED != state)
        {
            if(PORTBASED_ENABLED == state)
            {
                SYS_ERR_CHK(sal_mirror_group_sourcePort_get(gid,&mirrored_rx_portmask, &mirrored_tx_portmask));
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(port,mirrored_rx_portmask)
                {
                    if(!IS_LP_TRKMBR(port))
                        LOGIC_PORTMASK_SET_PORT(mirrorEntry.mirrored_igrPorts, port);
                }
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(port,mirrored_tx_portmask)
                {
                    if(!IS_LP_TRKMBR(port))
                        LOGIC_PORTMASK_SET_PORT(mirrorEntry.mirrored_egrPorts, port);
                }
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)                
                mirrorEntry.flowBasedOnly = DISABLED;
                mirrorEntry.mirror_type = PORT_BASED_MIRROR;
#endif             
            }
            else
            {
                NORMAL_LOGIC_PORTMASK_SET_ALL(mirrorEntry.mirrored_igrPorts);
                NORMAL_LOGIC_PORTMASK_SET_ALL(mirrorEntry.mirrored_egrPorts);
                LOGIC_PORTMASK_CLEAR_PORT(mirrorEntry.mirrored_igrPorts,mirroring_port);
                LOGIC_PORTMASK_CLEAR_PORT(mirrorEntry.mirrored_egrPorts,mirroring_port);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                mirrorEntry.flowBasedOnly = ENABLED;
                mirrorEntry.mirror_type = FLOW_BASED_MIRROR;
#endif
            }

            mirrorEntry.mirroring_port = mirroring_port;
            SYS_ERR_CHK(rsd_mirror_group_set(gid, &mirrorEntry));
        }
    }
    else
    {
        SYS_ERR_CHK(rsd_mirror_group_del(gid));

        SYS_ERR_CHK(sal_mirror_egrMode_set(mirroring_port,FORWARD_ALL_PKTS));

    #ifdef CONFIG_SYS_PROTO_STP
        SYS_ERR_CHK(sal_stp_portAdminEnable_get(mirroring_port, &enableTmp));
        SYS_ERR_CHK(sal_stp_portAdminEnable_set(mirroring_port, enableTmp));
    #endif
        SYS_ERR_CHK(sal_port_rxEnable_set(mirroring_port, ENABLED));

      #if 0
        SYS_LOG(LOG_CAT_MIRROR, LOG_MIRROR_PORT_DEL,
                LPSTR(mirroring_port));
      #endif
    }

    SYS_ERR_CHK(cfg_mirror_group_targetPort_set(gid, mirroring_port, enable));

    return SYS_ERR_OK;

}

int32 sal_mirror_group_igrState_get(sys_gid_t gid,sys_enable_t *pIgrState)
{
    SYS_PARAM_CHK((NULL == pIgrState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_mirror_group_igrState_get(gid,pIgrState));

    return SYS_ERR_OK;
}

int32 sal_mirror_group_igrState_set(sys_gid_t gid,sys_enable_t igrState)
{
    sys_logic_port_t port;
    sys_enable_t enable;
    sys_enable_t adminEbl;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(igrState), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid,&port,&enable));
    SYS_ERR_CHK(sal_port_enable_get(port, &adminEbl));

    if(ENABLED == enable)
    {
        if(ENABLED == igrState)
        {
            if(ENABLED == adminEbl)
                SYS_ERR_CHK(sal_port_rxEnable_set(port, ENABLED));
            else
                SYS_ERR_CHK(sal_port_rxEnable_set(port, DISABLED));
            SYS_ERR_CHK(sal_mirror_egrMode_set(port,FORWARD_ALL_PKTS));
        }
        else
        {
            SYS_ERR_CHK(sal_port_rxEnable_set(port,DISABLED));
            SYS_ERR_CHK(sal_mirror_egrMode_set(port,FORWARD_MIRRORED_PKTS_ONLY));
        }
    }
    else
    {
        if(ENABLED == adminEbl)
            SYS_ERR_CHK(sal_port_rxEnable_set(port, ENABLED));
        else
            SYS_ERR_CHK(sal_port_rxEnable_set(port, DISABLED));
    }

    SYS_ERR_CHK(cfg_mirror_group_igrState_set(gid,igrState));

    return SYS_ERR_OK;

}

int32 sal_mirror_group_state_get(sys_gid_t gid, sys_mirror_state_t *pState)
{
    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_mirror_group_state_get(gid,pState));

    return SYS_ERR_OK;
}

int32 sal_mirror_group_sourcePort_get(sys_gid_t gid,
            sys_logic_portmask_t  *pMirrored_rx_portmask,
            sys_logic_portmask_t  *pMirrored_tx_portmask)
{
    sys_mirror_state_t state;
    sys_port_mirror_t portMirror;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);
    SYS_PARAM_CHK((NULL == pMirrored_rx_portmask), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pMirrored_tx_portmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_mirror_group_state_get(gid,&state));

    if(PORTBASED_ENABLED != state)
        return SYS_ERR_MIRROR_NOT_PORTBASED;

    SYS_ERR_CHK(cfg_mirror_group_portMirror_get(gid,&portMirror));
    LOGIC_PORTMASK_COPY(*pMirrored_rx_portmask,portMirror.mirrored_igrPorts);
    LOGIC_PORTMASK_COPY(*pMirrored_tx_portmask,portMirror.mirrored_egrPorts);

    return SYS_ERR_OK;
}

int32 sal_mirror_group_sourcePort_set(sys_gid_t gid,
            sys_logic_portmask_t  mirrored_rx_portmask,
            sys_logic_portmask_t  mirrored_tx_portmask)
{
    sys_gid_t mirId;
    sys_mirror_state_t state;
    sys_port_mirror_t portMirror;
    char rxportstr[CAPA_PORTMASK_STR_LEN];
    char txportstr[CAPA_PORTMASK_STR_LEN];
    sys_enable_t enable,egrMode_enable;
    sys_logic_port_t port,targetPort;

    rsd_mirror_entry_t  mirrorEntry;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);

    SYS_ERR_CHK(sal_mirror_group_state_get(gid,&state));

    if(IS_LOGIC_PORTMASK_CLEAR(mirrored_rx_portmask) &&
            IS_LOGIC_PORTMASK_CLEAR(mirrored_tx_portmask))
    {
        SYS_ERR_CHK(rsd_mirror_group_del(gid));

        osal_memset(&portMirror, 0, sizeof(sys_port_mirror_t));
        SYS_ERR_CHK(cfg_mirror_group_portMirror_set(gid,portMirror));
        if(PORTBASED_ENABLED == state)
        {
            SYS_ERR_CHK(cfg_mirror_group_state_set(gid, MIRROR_DISABLED));
        }

        return SYS_ERR_OK;
    }

    FOR_EACH_GID(mirId)
    {
        SYS_ERR_CHK(sal_mirror_group_targetPort_get(mirId,&targetPort, &enable));

        if (ENABLED == enable)
        {
            if (IS_LOGIC_PORTMASK_PORTSET(mirrored_rx_portmask, targetPort))
                return SYS_ERR_MIRROR_MONITOR_PORT_MIRRORED;

            if (IS_LOGIC_PORTMASK_PORTSET(mirrored_tx_portmask, targetPort))
                return SYS_ERR_MIRROR_MONITOR_PORT_MIRRORED;
        }
    }

    osal_memset(rxportstr, 0, sizeof(rxportstr));
    osal_memset(txportstr, 0, sizeof(txportstr));

    SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid,&targetPort, &enable));

    if (ENABLED == enable)
    {
        osal_memset(&mirrorEntry, 0, sizeof(rsd_mirror_entry_t));

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port,mirrored_rx_portmask)
        {
            if(!IS_LP_TRKMBR(port))
                LOGIC_PORTMASK_SET_PORT(mirrorEntry.mirrored_igrPorts, port);
        }
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port,mirrored_tx_portmask)
        {
            if(!IS_LP_TRKMBR(port))
                LOGIC_PORTMASK_SET_PORT(mirrorEntry.mirrored_egrPorts, port);
        }
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
        mirrorEntry.mirror_type = PORT_BASED_MIRROR;
#endif
        mirrorEntry.mirroring_port = targetPort;
        mirrorEntry.flowBasedOnly = DISABLED;

        SYS_ERR_CHK(cfg_mirror_group_igrState_get(gid,&egrMode_enable));
        mirrorEntry.mirroring_port_iso = (egrMode_enable==ENABLED) ? DISABLED : ENABLED;

        SYS_ERR_CHK(rsd_mirror_group_set(gid, &mirrorEntry));
    }

    SYS_ERR_CHK(cfg_mirror_group_state_set(gid, PORTBASED_ENABLED));

    SYS_ERR_CHK(cfg_mirror_group_portMirror_get(gid,&portMirror));
    LOGIC_PORTMASK_COPY(portMirror.mirrored_igrPorts,mirrored_rx_portmask);
    LOGIC_PORTMASK_COPY(portMirror.mirrored_egrPorts,mirrored_tx_portmask);
    SYS_ERR_CHK(cfg_mirror_group_portMirror_set(gid,portMirror));

    LPM2STR(&mirrored_rx_portmask, rxportstr);
    LPM2STR(&mirrored_tx_portmask, txportstr);

  #if 0
    SYS_LOG(LOG_CAT_MIRROR, LOG_MIRROR_SPORT_SET,
            rxportstr,
            txportstr);
  #endif

    return SYS_ERR_OK;
}

int32 sal_mirror_group_del(sys_gid_t gid)
{
    sys_enable_t enable;
    sys_mirror_state_t state;
    sys_logic_port_t mirroring_port;

    SYS_PARAM_CHK(IS_GID_INVALID(gid), SYS_ERR_MIRROR_GROUP_INVALID);

    SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid,&mirroring_port,&enable));

    if(ENABLED == enable)
    {
        SYS_ERR_CHK(sal_mirror_group_targetPort_set(gid, mirroring_port, DISABLED));
    }

    SYS_ERR_CHK(rsd_mirror_group_del(gid));

    SYS_ERR_CHK(sal_mirror_group_state_get(gid,&state));

    SYS_ERR_CHK(cfg_mirror_group_del(gid));

    return SYS_ERR_OK;
}

int32 sal_mirror_egrMode_get(sys_logic_port_t port,rsd_mirror_egrMode_t *pEgrMode)
{
    SYS_ERR_CHK(rsd_mirror_egrMode_get(port,pEgrMode));

    return SYS_ERR_OK;
}

int32 sal_mirror_egrMode_set(sys_logic_port_t port,rsd_mirror_egrMode_t egrMode)
{
    SYS_ERR_CHK(rsd_mirror_egrMode_set(port,egrMode));

    return SYS_ERR_OK;
}

