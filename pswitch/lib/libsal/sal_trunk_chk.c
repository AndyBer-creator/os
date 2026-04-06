
#include <libcfg/cfg.h>
#include <libsal/sal_util.h>
#include <libsal/sal_vlan.h>
#include <libsal/sal_lacp.h>
#include <libsal/sal_mirror.h>
#include <libsal/sal_switch.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_optical.h>
#ifdef CONFIG_SYS_PORT_SECURITY
#include <libsal/sal_psecure.h>
#endif

typedef int32 (*_sal_trunk_chg_func)(sys_logic_port_t srcPort, sys_logic_port_t dstPort);
typedef int32 (*_sal_trunk_chk_func)(uint32 trunkId, sys_logic_portmask_t *pPortmask);
typedef int32 (*_sal_trunk_ref_func)(uint32 trunkId);

#ifdef CONFIG_SYS_PROTO_AUTHMGR
static int32 _sal_trunk_authmgr_check(uint32 trunkId, sys_logic_portmask_t *pPortmask);
#endif
static int32 _sal_trunk_portInOtherTrunk_check(uint32 trunkId, sys_logic_portmask_t *pPortmask);
static int32 _sal_trunk_vlanConflict_check(uint32 trunkId, sys_logic_portmask_t *pPortmask);
#ifdef CONFIG_SYS_MIRROR
static int32 _sal_trunk_portMirroring_check(uint32 trunkId, sys_logic_portmask_t *pPortmask);
#endif
static int32 _sal_trunk_portType_check(uint32 trunkId, sys_logic_portmask_t *pPortmask);

static int32 _sal_trunk_port_change(sys_logic_port_t srcPort, sys_logic_port_t dstPort);
static int32 _sal_trunk_qos_change(sys_logic_port_t srcPort, sys_logic_port_t dstPort);
#ifdef CONFIG_SYS_PROTOCOL_VLAN
static int32 _sal_trunk_protoVlan_change(sys_logic_port_t srcPort, sys_logic_port_t dstPort);
#endif

static int32 _sal_trunk_port_refresh(uint32 trunkIs);
static int32 _sal_trunk_staticMacEntry_refresh(uint32 trunkIs);
#ifdef CONFIG_SYS_PROTOCOL_VLAN
static int32 _sal_trunk_protoVlan_refresh(uint32 trunkIs);
#endif
#ifdef CONFIG_SYS_MIRROR
static int32 _sal_trunk_mirror_refresh(uint32 trunkId);
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

static int32 _sal_trunk_authmgr_check(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    sys_logic_port_t  port;
    authmgr_portCtrl_t ctrl;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, *pPortmask)
    {
        sal_authmgr_portCtrl_get(port, &ctrl);

        if (AUTHMGR_PORTCTRL_DISABLE != ctrl)
        {
            return SYS_ERR_TRUNK_PORT_HAS_AUTH;
        }
    }

    return SYS_ERR_OK;
}

#endif

static int32 _sal_trunk_portInOtherTrunk_check(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    uint32 id;
    sys_logic_portmask_t andResult;
    sys_logic_portmask_t memberMask;

    FOR_EACH_TRUNK(id)
    {
        if (id == trunkId)
        {
            continue; 
        }

        SYS_ERR_CHK(cfg_trunk_member_get(id, &memberMask));

        LOGIC_PORTMASK_AND(andResult, memberMask, *pPortmask);

        if (!IS_NORMAL_LOGIC_PORTMASK_CLEAR(andResult))
            return SYS_ERR_TRUNK_PORT_USED_BY_TRK;
    }

    return SYS_ERR_OK;
}

static int32 _sal_trunk_vlanConflict_check(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
#if 0 
    sys_vid_t defaultVlan;
    sys_logic_portmask_t lpmResult;
    sys_vlanmask_t vmExist;
    sys_vid_t vid;
    sys_vlan_operEntry_t operEntry;

    SYS_ERR_CHK(sal_vlan_defaultVid_get(&defaultVlan));

    SYS_MEM_CLEAR(lpmResult);

    SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vid, vmExist)
    {
        if (defaultVlan == vid)
            continue;

        SYS_ERR_CHK(sal_vlan_operEntry_get(vid, &operEntry));

        LOGIC_PORTMASK_AND(lpmResult, operEntry.lpmMember, *pPortmask);

        if (!IS_NORMAL_LOGIC_PORTMASK_CLEAR(lpmResult))
            return SYS_ERR_TRUNK_PORT_VLAN_CONFLICT;
    }
#endif
    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MIRROR

static int32 _sal_trunk_portMirroring_check(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    sys_gid_t gid;
    sys_enable_t enable = DISABLED;
    sys_logic_port_t mirroring_port;

    FOR_EACH_GID(gid)
    {
        SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid,&mirroring_port, &enable));

        if ((ENABLED == enable) && IS_LOGIC_PORTMASK_PORTSET(*pPortmask, mirroring_port))
        {
            return SYS_ERR_TRUNK_PORT_MIRRORING;
        }
    }

    return SYS_ERR_OK;
}
#endif

static int32 _sal_trunk_portType_check(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    sys_logic_port_t port;
    sys_port_type_t type = PORT_TYPE_END;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, *pPortmask)
    {
        if (PORT_TYPE_END == type)
        {
            type = LP2TYPE(port);

            continue;
        }

        if (type != LP2TYPE(port))
            return SYS_ERR_TRUNK_PORT_SPEED;
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PORT_SECURITY

static int32 _sal_trunk_portSecurity_check(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    sys_enable_t        enable = DISABLED;
    sys_logic_port_t    port;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, *pPortmask)
    {
        SYS_ERR_CHK(sal_psecure_portEnable_get(port, &enable));

        if (ENABLED == enable)
        {
            return SYS_ERR_TRUNK_PORT_HAS_PORT_SECURITY;
        }
    }

    return SYS_ERR_OK;
}
#endif

static int32 _sal_trunk_port_change(sys_logic_port_t srcPort, sys_logic_port_t dstPort)
{
    int32 ret = SYS_ERR_OK;
    sys_enable_t enable;
    sys_enable_t fc;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_port_phyAbility_t ability;
    sys_port_speedDuplex_t speedDuplex;

    char src_portstr[CAPA_PORT_STR_LEN];
    char dst_portstr[CAPA_PORT_STR_LEN];

    sys_enable_t autoEnable = DISABLED;
    sys_port_media_t media = PORT_MEDIA_COPPER;

    osal_memset(&speed, 0, sizeof(speed));
    osal_memset(&duplex, 0, sizeof(duplex));

    SYS_ERR_CHK(cfg_port_speed_get(srcPort, &speed));
    SYS_ERR_CHK(cfg_port_duplex_get(srcPort, &duplex));

    if (ENABLED == speed.autoSpeed || ENABLED == duplex.autoDuplex)
        autoEnable = ENABLED;

    SYS_ERR_CHK(sal_port_speedDuplex_set(dstPort,
                                         speed,
                                         duplex,
                                         PORT_MEDIA_COPPER,
                                         (IS_TRKLP(srcPort) ? TRUE : FALSE)));

    if (IS_COMBO_PORT(dstPort))
    {
        sal_port_speedDuplex_set(dstPort,
                                 speed,
                                 duplex,
                                 PORT_MEDIA_FIBER,
                                 (IS_TRKLP(srcPort) ? TRUE : FALSE));
    }

    if (IS_COMBO_PORT(dstPort) || IS_FIBER_PORT(dstPort))
    {
        if (IS_TRKLP(srcPort) || IS_LP_TRKMBR(dstPort))
        {
            LP2STR(srcPort, src_portstr);
            LP2STR(dstPort, dst_portstr);

            SYS_PRINTF("Port %s is added to port-channel %s, "
                       "please ensure the speed/duplex setting on %s is correct for the SFP inserted.\n",
                       dst_portstr, src_portstr, src_portstr);
        }
    }

    SYS_ERR_CHK(cfg_port_flowCtrl_get(srcPort, &enable));

    if (ENABLED == autoEnable)
    {
        osal_memset(&ability, 0, sizeof(ability));
        SYS_ERR_CHK(rsd_port_autoNegoAbility_get(dstPort, &ability));

        ability.AsyFC = enable;
        ability.FC    = enable;
        SYS_ERR_CHK(rsd_port_autoNegoAbility_set(dstPort, &ability));
    }
    else
    {
        osal_memset(&speedDuplex, 0, sizeof(speedDuplex));
        SYS_ERR_CHK(rsd_port_forceModeAbility_get(dstPort, &speedDuplex, &fc));

        fc = enable;
        SYS_ERR_CHK(rsd_port_forceModeAbility_set(dstPort, &speedDuplex, fc));
    }

    if (!IS_TRKLP(srcPort) && !IS_LP_TRKMBR(dstPort))
    {
        ret = sal_port_linkMedia_get(dstPort, &media);

        if (SYS_ERR_OK == ret && PORT_MEDIA_FIBER == media)
        {
            
            duplex.autoDuplex = DISABLED;
            duplex.ability = SYS_PORT_DUPLEX_FULL;

            if (ENABLED == speed.autoSpeed && SYS_PORT_SPEED_ALL == speed.ability)
                sal_optical_autoSensingConfig_set(dstPort);
            else
                sal_port_speedDuplex_set(dstPort, speed, duplex, media, FALSE);
        }
    }

#ifdef CONFIG_SYS_PORT_ISOLATION
    SYS_ERR_CHK(cfg_port_protected_get(srcPort, &enable));
    SYS_ERR_CHK(rsd_port_protected_set(dstPort, enable));
#endif

    SYS_ERR_CHK(cfg_port_enable_get(srcPort, &enable));

    if (srcPort == dstPort)
    {
        SYS_ERR_CHK(rsd_port_enable_set(dstPort, enable));
    }
    
    else
    {
        sys_enable_t finalEnbl = enable;
    #ifdef CONFIG_SYS_PORT_ERR_DISABLED
        uint32 reasonMask = 0;

        SYS_ERR_CHK(rsd_port_errDisable_get(srcPort, &reasonMask));

        if (0 != reasonMask)
            finalEnbl = DISABLED;
    #endif

        SYS_ERR_CHK(rsd_port_enableForce_set(dstPort, finalEnbl));
    }

    return SYS_ERR_OK;
}

static int32 _sal_trunk_qos_change(sys_logic_port_t srcPort, sys_logic_port_t dstPort)
{
    sys_qos_cos_t cos;
    sys_enable_t state;
    sys_qos_mode_t mode;
    sys_qos_trustType_t trust;

    SYS_ERR_CHK(cfg_qos_portCos_get(srcPort, &cos));
    SYS_ERR_CHK(rsd_qos_portCos_set(dstPort, cos));

    SYS_ERR_CHK(cfg_qos_mode_get(&mode));

    if (SYS_QOS_MODE_BASIC == mode)
    {
        SYS_ERR_CHK(sal_qos_portTrustState_get(srcPort, &state));
        SYS_ERR_CHK(cfg_qos_trustType_get(&trust));

        if (DISABLED == state)
            trust = SYS_QOS_TRUST_NONE;

        SYS_ERR_CHK(rsd_qos_portTrustType_set(dstPort, trust));
    }

    SYS_ERR_CHK(cfg_qos_portRemarkCos_get(srcPort, &state));
    SYS_ERR_CHK(rsd_qos_portRemarkCos_set(dstPort, state));

    SYS_ERR_CHK(cfg_qos_portRemarkDscp_get(srcPort, &state));
    SYS_ERR_CHK(rsd_qos_portRemarkDscp_set(dstPort, state));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_DOS_PROTECTION
static int32 _sal_trunk_dos_change(sys_logic_port_t srcPort, sys_logic_port_t dstPort)
{
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(cfg_dos_portEnable_get(srcPort, &enable));
    SYS_ERR_CHK(rsd_dos_portEnable_set(dstPort, enable));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTOCOL_VLAN
static int32 _sal_trunk_protoVlan_change(sys_logic_port_t srcPort, sys_logic_port_t dstPort)
{
    uint32 groupIdx = 0;
    sys_vlan_protoVlanCfg_t pvlanCfg;
    sys_enable_t enable;

   for (groupIdx = 0; groupIdx < CAPA_VLAN_PROTO_GROUP_NUM; groupIdx++)
   {
        SYS_MEM_CLEAR(pvlanCfg);
        if(!IS_TRKLP(srcPort))
        {
            SYS_ERR_CONTINUE(sal_vlan_portProtoVlan_get(srcPort, groupIdx,&pvlanCfg, &enable));
            SYS_ERR_CONTINUE(sal_vlan_portProtoVlan_set(dstPort, groupIdx,&pvlanCfg, enable));
        }
    }

   return SYS_ERR_OK;
}
#endif

static int32 _sal_trunk_port_refresh(uint32 trunkId)
{
#ifdef CONFIG_SYS_PORT_ISOLATION
    
    sys_enable_t state;
    sys_logic_port_t trkLogicPort;

    trkLogicPort = TRK2LP(trunkId);

    SYS_ERR_CHK(cfg_port_protected_get(trkLogicPort, &state));
    SYS_ERR_TRK_CHK(rsd_port_protected_set(trkLogicPort, state));
#endif

    return SYS_ERR_OK;
}
static int32 _sal_trunk_staticMacEntry_refresh(uint32 trunkId)
{
    uint32 i = 0;
    sys_logic_port_t trkLogicPort;
    sys_l2_entry_t *pCurr = NULL;

    trkLogicPort = TRK2LP(trunkId);

    FOR_EACH_L2_STATIC_ENTRY(i, pCurr)
    {
        
        if (trkLogicPort != pCurr->port)
            continue;

        if(IS_TRKLP_EMPTY(trkLogicPort))
            SYS_ERR_CHK(rsd_l2_mac_del(pCurr));
        else
            SYS_ERR_TRK_CHK(rsd_l2_mac_add(pCurr));
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MIRROR
static int32 _sal_trunk_mirror_refresh(uint32 trunkId)
{
    sys_gid_t gid;
    sys_mirror_state_t state;
    sys_logic_portmask_t mirrored_rx_portmask;
    sys_logic_portmask_t mirrored_tx_portmask;

    FOR_EACH_GID(gid)
    {
        SYS_ERR_CHK(sal_mirror_group_state_get(gid,&state));

        if(PORTBASED_ENABLED == state)
        {
            SYS_ERR_CHK(sal_mirror_group_sourcePort_get(gid,
                                                &mirrored_rx_portmask,
                                                &mirrored_tx_portmask));

            SYS_ERR_CHK(sal_mirror_group_sourcePort_set(gid,
                                                mirrored_rx_portmask,
                                                mirrored_tx_portmask));

        }
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTOCOL_VLAN
static int32 _sal_trunk_protoVlan_refresh(uint32 trunkId)
{
    uint32 groupIdx = 0;
    sys_logic_port_t trkLogicPort;
    sys_vlan_protoVlanCfg_t pvlanCfg;
    sys_enable_t enable;

    trkLogicPort = TRK2LP(trunkId);

    for (groupIdx = 0; groupIdx < CAPA_VLAN_PROTO_GROUP_NUM; groupIdx++)
    {
        SYS_MEM_CLEAR(pvlanCfg);

        SYS_ERR_CONTINUE(sal_vlan_portProtoVlan_get(trkLogicPort, groupIdx,&pvlanCfg,&enable));
        SYS_ERR_CONTINUE(sal_vlan_portProtoVlan_set(trkLogicPort, groupIdx,&pvlanCfg,enable));
    }

   return SYS_ERR_OK;

}
#endif

int32 sal_trunk_memberNum_check(
    sys_trunk_portRole_t trunkRole,
    sys_logic_portmask_t *pPortmask)
{
    uint32 num = 0;
    sys_logic_port_t port;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, *pPortmask)
    {
        num++;
    }

        {
            if (num > CAPA_LAG_MBR_NORMAL_NUM)
                return SYS_TRK_MBR_PORT_NORMAL_MAX_ERR;
        }

    return SYS_ERR_OK;
}

int32 sal_trunk_valid_check(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    uint32 i = 0;
    const _sal_trunk_chk_func trkMbrChkTbl[] =
    {
#ifdef CONFIG_SYS_PROTO_AUTHMGR
        _sal_trunk_authmgr_check,
#endif
        _sal_trunk_portInOtherTrunk_check,
        _sal_trunk_vlanConflict_check,
#ifdef CONFIG_SYS_MIRROR
        _sal_trunk_portMirroring_check,
#endif
        _sal_trunk_portType_check,
#ifdef CONFIG_SYS_PORT_SECURITY
        _sal_trunk_portSecurity_check,
#endif
        NULL
    };

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pPortmask), SYS_ERR_NULL_POINTER);

    while (NULL != trkMbrChkTbl[i])
    {
        SYS_ERR_CHK(trkMbrChkTbl[i](trunkId, pPortmask));

        i++;
    }

    return SYS_ERR_OK;
}

int32 sal_trunk_portRole_change(sys_logic_port_t srcPort, sys_logic_port_t dstPort)
{
    uint32 i = 0;
    const _sal_trunk_chg_func trkMbrChgTbl[] =
    {
        _sal_trunk_port_change,
        _sal_trunk_qos_change,
#ifdef CONFIG_SYS_PROTOCOL_VLAN
        _sal_trunk_protoVlan_change,
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
        _sal_trunk_dos_change,
#endif
        NULL
    };

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(srcPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(dstPort), SYS_ERR_PORT_ID);

    while (NULL != trkMbrChgTbl[i])
    {
        SYS_ERR_CHK(trkMbrChgTbl[i](srcPort, dstPort));

        i++;
    }

    return SYS_ERR_OK;
}

int32 sal_trunk_setting_refresh(uint32 trunkId)
{
    uint32 i = 0;
    const _sal_trunk_ref_func trkMbrRefTbl[] =
    {
        _sal_trunk_port_refresh,
        _sal_trunk_staticMacEntry_refresh,
#ifdef CONFIG_SYS_PROTOCOL_VLAN
        _sal_trunk_protoVlan_refresh,
#endif
#ifdef CONFIG_SYS_MIRROR
        _sal_trunk_mirror_refresh,
#endif
        NULL
    };

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);

    while(NULL != trkMbrRefTbl[i])
    {
        SYS_ERR_CHK(trkMbrRefTbl[i](trunkId));

        i++;
    }

    return SYS_ERR_OK;
}

