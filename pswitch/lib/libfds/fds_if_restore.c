
#include <libfds/fds_if_restore.h>
#include <libcfg/cfg.h>

#include <libsal/sal_sys.h>
#include <libsal/sal_port.h>
#include <libsal/sal_switch.h>
#include <libsal/sal_vlan.h>
#include <libsal/sal_mirror.h>
#include <libsal/sal_l2.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_qos.h>
#include <libsal/sal_rate.h>
#include <libsal/sal_mib_private.h>
#ifdef CONFIG_SYS_AAA
#include <libsal/sal_aaa.h>
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
#include <libsal/sal_authmgr.h>
#endif

#ifdef CONFIG_SYS_SYSLOG
#include <libsal/sal_syslog.h>
#endif

#ifdef CONFIG_SYS_EEE
#include <libsal/sal_eee.h>
#endif

#ifdef CONFIG_SYS_PROTO_LACP
#include <libsal/sal_lacp.h>
#endif

#ifdef CONFIG_SYS_PROTO_STP
#include <libsal/sal_stp.h>
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
#include <libsal/sal_igmp.h>
#endif

#ifdef CONFIG_SYS_PROTO_LLDP
#include <libsal/sal_lldp.h>
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION
#include <libsal/sal_dos.h>
#endif

#ifdef CONFIG_SYS_PROTO_GVRP
#include <libsal/sal_gvrp.h>
#endif
#ifdef CONFIG_SYS_PROTO_GARP
#include <libsal/sal_garp.h>
#endif

#ifdef CONFIG_SYS_LIB_CUSTOM
#include <libcustom/sal_custom.h>
#include <libcustom/fds_custom.h>
#endif

#ifdef CONFIG_SYS_PROTO_ISG
#include <libsal/sal_isg.h>
#endif

#ifdef CONFIG_SYS_MGMT_ACL
#include <libsal/sal_acl_mgmt.h>
#endif

#ifdef CONFIG_SYS_USER_DEFINED_ACL
#include <libsal/sal_acl_usr.h>
#endif

#ifdef CONFIG_SYS_PROTO_UDLD
#include <libsal/sal_udld.h>
#endif
#ifdef CONFIG_SYS_PROTO_ERPS

#endif
#ifdef CONFIG_SYS_POE
#include <libsal/sal_poe.h>
#endif

typedef int32 (*fds_port_restore_func)(sys_logic_port_t port);
typedef struct fds_port_restore_tbl_s
{
    fds_port_restore_func port_restore;
} fds_port_restore_tbl_t;

const fds_port_restore_tbl_t fdsPortRestoreTbl[] =
{

    {fds_trunk_port_restore},
#ifdef CONFIG_SYS_PROTO_LACP
    {fds_lacp_port_restore},
#endif
    {fds_switch_port_restore},
    {fds_port_port_restore},
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    {fds_igmp_port_restore},
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    {fds_mld_port_restore},
    #endif
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    {fds_gvrp_port_restore},
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    {fds_garp_port_restore},
#endif

    {fds_vlan_port_restore},

#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {fds_vlan_protoVlan_port_restore},
#endif

#ifdef CONFIG_SYS_VOICEVLAN
    {fds_vlan_voiceVlan_port_restore},
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {fds_surveillance_vlan_port_restore},
#endif

#ifdef CONFIG_SYS_MIRROR
    {fds_mirror_port_restore},
#endif
    {fds_rate_port_restore},
    {fds_qos_port_restore},
#ifdef CONFIG_SYS_PORT_SECURITY
    {fds_psecure_port_restore},
#endif
#ifdef CONFIG_SYS_EEE
    {fds_eee_port_restore},
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    {fds_authmgr_port_restore},
#endif
#ifdef CONFIG_SYS_PROTO_STP
    {fds_stp_port_restore},
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    {fds_lldp_port_restore},
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    {fds_isg_port_restore},
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {fds_dhcp_snooping_port_restore},
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    {fds_dai_port_restore},
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    {fds_acl_port_restore},
#endif
#ifdef CONFIG_SYS_LIB_CUSTOM
    {fds_custom_port_restore},
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
    {fds_udld_port_restore},
#endif
#ifdef CONFIG_SYS_POE
	{fds_poe_port_restore},
#endif
    {NULL}
};

int32 fds_port_restore(sys_logic_port_t port)
{
    uint32 i = 0;

    while (fdsPortRestoreTbl[i].port_restore)
    {
        fdsPortRestoreTbl[i].port_restore(port);

        i++;
    }

    sal_port_enable_set(port, DFLT_PORT_EBL);

    return SYS_ERR_OK;
}

int32 fds_port_port_restore(sys_logic_port_t port)
{
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_port_speed_t fiberSpeed;
    sys_port_conf_t portConf;

    osal_memset(&speed, 0, sizeof(speed));
    osal_memset(&duplex, 0, sizeof(duplex));
    osal_memset(&fiberSpeed, 0, sizeof(fiberSpeed));
    osal_memset(&portConf, 0, sizeof(portConf));

    if (IS_TRKLP(port))
    {
        portConf.speed = PORTCONF_SPEED_AUTO_ALL;
        portConf.duplex = PORTCONF_DUPLEX_AUTO;
    }
    else
    {
        SYS_ERR_CHK(sal_sys_boardPortConf_get(port, &portConf));
    }

    switch (portConf.speed)
    {
        case PORTCONF_SPEED_FORCE_10M:
            speed.autoSpeed = DISABLED;
            speed.ability = SYS_PORT_SPEED_10M;
            fiberSpeed.autoSpeed = DISABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
            break;

        case PORTCONF_SPEED_FORCE_100M:
            speed.autoSpeed = DISABLED;
            speed.ability = SYS_PORT_SPEED_100M;
            fiberSpeed.autoSpeed = DISABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
            break;

        case PORTCONF_SPEED_FORCE_1000M:
            speed.autoSpeed = DISABLED;
            speed.ability = SYS_PORT_SPEED_1000M;
            fiberSpeed.autoSpeed = DISABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
            break;

       case PORTCONF_SPEED_FORCE_2500M:
            speed.autoSpeed = DISABLED;
            speed.ability = SYS_PORT_SPEED_2500M;
            fiberSpeed.autoSpeed = DISABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_2500M;
            break;

        case PORTCONF_SPEED_FORCE_10G:
            speed.autoSpeed = DISABLED;
            speed.ability = SYS_PORT_SPEED_10G;
            fiberSpeed.autoSpeed = DISABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_10G;
            break;

        case PORTCONF_SPEED_AUTO_10M:
            speed.autoSpeed = ENABLED;
            speed.ability = SYS_PORT_SPEED_10M;
            fiberSpeed.autoSpeed = ENABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
            break;

        case PORTCONF_SPEED_AUTO_100M:
            speed.autoSpeed = ENABLED;
            speed.ability = SYS_PORT_SPEED_100M;
            fiberSpeed.autoSpeed = ENABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
            break;

        case PORTCONF_SPEED_AUTO_1000M:
            speed.autoSpeed = ENABLED;
            speed.ability = SYS_PORT_SPEED_1000M;
            fiberSpeed.autoSpeed = ENABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
            break;

        case PORTCONF_SPEED_AUTO_10M_100M:
            speed.autoSpeed = ENABLED;
            speed.ability = SYS_PORT_SPEED_10M_100M;
            fiberSpeed.autoSpeed = ENABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
            break;

       case PORTCONF_SPEED_FORCE_COMBO_100M:
            speed.autoSpeed = ENABLED;
            speed.ability = SYS_PORT_SPEED_ALL;
            fiberSpeed.autoSpeed = DISABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_100M;
            break;

        case PORTCONF_SPEED_AUTO_ALL:
        default:
            speed.autoSpeed = ENABLED;
            speed.ability = SYS_PORT_SPEED_ALL;
            fiberSpeed.autoSpeed = ENABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
            break;
    }

    switch (portConf.duplex)
    {
        case PORTCONF_DUPLEX_FORCE_FULL:
            duplex.autoDuplex = DISABLED;
            duplex.ability = SYS_PORT_DUPLEX_FULL;
            break;

        case PORTCONF_DUPLEX_FORCE_HALF:
            duplex.autoDuplex = DISABLED;
            duplex.ability = SYS_PORT_DUPLEX_HALF;

            break;

        case PORTCONF_DUPLEX_AUTO:
        default:
            duplex.autoDuplex = ENABLED;
            break;
    }

    if(IS_COMBO_PORT(port))
    {
        SYS_ERR_CHK(sal_port_comboMediaPrefer_set(port, DFLT_PORT_COMBO_AUTO));
    }

    if((IS_COMBO_PORT(port) || IS_FIBER_PORT(port)) && !IS_TGE_PORT(port))
    {
        SYS_ERR_CHK(sal_port_fiberAutoNegoEnable_set(port, DFLT_PORT_FIBER_NEGO_EBL));
    }

    SYS_ERR_CHK(sal_port_speed_set(port, speed));
    SYS_ERR_CHK(sal_port_duplex_set(port, duplex));

    SYS_ERR_CHK(sal_port_flowCtrl_set(port, DFLT_PORT_FLOW_CTRL));
    SYS_ERR_CHK(sal_port_flowCtrlAuto_set(port, DFLT_PORT_FLOW_CTRL_AUTO));
    SYS_ERR_CHK(sal_port_backpressureEnable_set(port, DFLT_PORT_BACK_PRESSURE));

    if ((PORT_MEDIA_COPPER != portConf.mediaType) && (PORT_MEDIA_FIBER != portConf.mediaType))
    {
        SYS_ERR_CHK(sal_port_fiberSpeed_set(port, fiberSpeed));
    }
    
    if (portConf.speed == PORTCONF_SPEED_FORCE_COMBO_100M)
        SYS_ERR_CHK(sal_port_combo_fiberSpeed_set(port,fiberSpeed));

    SYS_ERR_CHK(sal_port_enableCfg_set(port, DFLT_PORT_EBL));

#ifdef CONFIG_SYS_PORT_ISOLATION
    
    SYS_ERR_CHK(sal_port_protected_set(port, DFLT_PORT_PROTECTED));
#endif 

#ifdef CONFIG_SYS_PORT_DESCRIPTION
    SYS_ERR_CHK(sal_port_descp_set(port, DFLT_PORT_DESCRIPTION));
#endif
	SYS_ERR_CHK(sal_mib_last_5_rate_set(port, TYPE_IN_PKT_RATE, 0));
	SYS_ERR_CHK(sal_mib_last_5_rate_set(port, TYPE_IN_BYTE_RATE, 0));
	SYS_ERR_CHK(sal_mib_last_5_rate_set(port, TYPE_OUT_PKT_RATE, 0));
	SYS_ERR_CHK(sal_mib_last_5_rate_set(port, TYPE_OUT_BYTE_RATE, 0));
    return SYS_ERR_OK;
}

int32 fds_switch_port_restore(sys_logic_port_t port)
{
#ifdef CONFIG_SYS_JUMBO_FRAME

#endif 

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PORT_SECURITY

int32 fds_psecure_port_restore(sys_logic_port_t port)
{
    sys_psecure_port_t portCfg;

    if (IS_NORMAL_LOGIC_PORT_INVALID(port))
        return SYS_ERR_OK;

    SYS_MEM_CLEAR(portCfg);
    portCfg.enable = DFLT_PSECURE_PORT_EBL;
    portCfg.limitNum = DFLT_PSECURE_PORT_LIMITNUM;
    portCfg.violation = DFLT_PSECURE_PORT_VIOLATION;
    portCfg.sticky = DFLT_PSECURE_PORT_STICKY;

    SYS_ERR_CHK(sal_psecure_port_set(port, &portCfg));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_EEE

int32 fds_eee_port_restore(sys_logic_port_t port)
{
    SYS_ERR_CHK(sal_eee_portEnable_set(port, DFLT_PORT_EEE_EBL));

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PROTO_AUTHMGR

int32 fds_authmgr_port_restore(sys_logic_port_t port)
{
    authmgr_cfg_port_t portCfg;

    SYS_MEM_CLEAR(portCfg);

    portCfg.ctrlType = DFLT_AUTHMGR_PORT_CTRL;
    portCfg.mode = DFLT_AUTHMGR_PORT_MODE;
    portCfg.typeOrder.order[0] = DFLT_AUTHMGR_PORT_ORDER_FIRST;
#ifdef CONFIG_SYS_PROTO_DOT1X
    portCfg.typeEnbl[AUTHMGR_TYPE_DOT1X] = DFLT_AUTHMGR_PORT_DOT1X_EBL;
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    portCfg.typeEnbl[AUTHMGR_TYPE_MAC] = DFLT_AUTHMGR_PORT_MAC_EBL;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    portCfg.typeEnbl[AUTHMGR_TYPE_WEB] = DFLT_AUTHMGR_PORT_WEB_EBL;
#endif
    portCfg.methodOrder.order[0] = DFLT_AUTHMGR_PORT_METHOD_ORDER_FIRST;
    portCfg.reauthEnbl = DFLT_AUTHMGR_PORT_REAUTH_EBL;
    portCfg.guestVlanEnbl = DFLT_AUTHMGR_PORT_GUEST_VLAN_EBL;
    portCfg.maxHosts = DFLT_AUTHMGR_PORT_MAX_HOSTS;
    portCfg.vlanAssignMode = DFLT_AUTHMGR_PORT_VLAN_ASSIGN_MODE;
    portCfg.reauthPeriod = DFLT_AUTHMGR_PORT_REAUTH_PERIOD;
    portCfg.inactTimeout = DFLT_AUTHMGR_PORT_INACT_PERIOD;
    portCfg.quietPeriod = DFLT_AUTHMGR_PORT_QUIET_PERIOD;
#ifdef CONFIG_SYS_PROTO_DOT1X
    portCfg.maxReq = DFLT_AUTHMGR_PORT_MAX_REQ;
    portCfg.txPeriod = DFLT_AUTHMGR_PORT_EAP_TX_PERIOD;
    portCfg.suppTimeout = DFLT_AUTHMGR_PORT_SUPP_TIMEOUT;
    portCfg.serverTimeout = DFLT_AUTHMGR_PORT_SRV_TIMEOUT;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    portCfg.loginAttempt = DFLT_AUTHMGR_PORT_WEBAUTH_LOGIN_ATTEMPT;
#endif

    SYS_ERR_CHK(sal_authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}
#endif

int32 fds_vlan_port_restore(sys_logic_port_t port)
{
    sys_vid_t vid = 0;
    sys_vid_t defaultVid = DFLT_VLAN_DFLT_VLAN;
    sys_vlanmask_t vm;
    sys_vlan_portMember_t portVlanMbr;
    sys_vlanmask_t vlanmask;

    SYS_MEM_CLEAR(portVlanMbr);
    SYS_ERR_CHK(sal_vlan_portAdminVlanMember_get(port, &portVlanMbr));

    SYS_ERR_CHK(sal_vlan_defaultVid_get(&defaultVid));

    VLANMASK_OR(vm, portVlanMbr.vmForbidden, portVlanMbr.vmMember);
    VLANMASK_CLEAR_VID(vm, defaultVid);

    FOR_EACH_VID_IN_VLANMASK(vid, vm)
    {
        SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_set(port, vid, SYS_VLAN_EXCLUDED));
    }

    SYS_ERR_CHK(sal_vlan_portAdminMode_set(port, DFLT_VLAN_PORT_MODE));
    SYS_ERR_CHK(sal_vlan_portAdminRoleUplinkEnable_set(port, DFLT_VLAN_TRUNK_UPLINK));
    SYS_ERR_CHK(sal_vlan_portAdminAcceptFrameType_set(port, DFLT_VLAN_ACCEPT_FRAME_TYPE));
    SYS_ERR_CHK(sal_vlan_portAdminIgrFilterEnable_set(port, DFLT_VLAN_IGR_FILTER));
    SYS_ERR_CHK(sal_vlan_portAdminPvid_set(port, defaultVid));
    SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_set(port, defaultVid, DFLT_DEFAULT_VLAN_MEMBERSHIP));

    SYS_ERR_CHK(STR2VMSK(DFLT_VLAN_TRUNK_ALLOW_VLAN, &vlanmask));
    SYS_ERR_CHK(sal_vlan_portTrunkAllowedVlan_set(port, &vlanmask));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTOCOL_VLAN

int32 fds_vlan_protoVlan_port_restore(sys_logic_port_t port)
{
    uint32 index;
    sys_vlan_protoVlanCfg_t pVlan_cfg;

    SYS_MEM_CLEAR(pVlan_cfg);
    pVlan_cfg.vid = DFLT_VLAN_DFLT_VLAN;

    for(index = 0; index < CAPA_VLAN_PROTO_GROUP_NUM; index++)
    {
        SYS_ERR_CHK(sal_vlan_portProtoVlan_set(port,index,&pVlan_cfg,DISABLED));
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_VOICEVLAN

int32 fds_vlan_voiceVlan_port_restore(sys_logic_port_t port)
{
    SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_set(port, DFLT_VLAN_VOICE_PORT_EBL));
    SYS_ERR_CHK(sal_vlan_portVoiceVlanMode_set(port, DFLT_VLAN_VOICE_PORT_MODE));
    SYS_ERR_CHK(sal_vlan_portVoiceVlancosMode_set(port, DFLT_VLAN_VOICE_PORT_COS_MODE));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN

int32 fds_surveillance_vlan_port_restore(sys_logic_port_t port)
{
    SYS_ERR_CHK(sal_surveillance_vlan_portMode_set(port, DFLT_VLAN_SURVEILLANCE_PORT_MODE));
    SYS_ERR_CHK(sal_surveillance_vlan_portCosMode_set(port, DFLT_VLAN_SURVEILLANCE_PORT_COS_MODE));
    SYS_ERR_CHK(sal_surveillance_vlan_portEnable_set(port, DFLT_VLAN_SURVEILLANCE_PORT_EBL));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_MIRROR

int32 fds_mirror_port_restore(sys_logic_port_t port)
{
    return SYS_ERR_OK;
}
#endif

int32 fds_trunk_port_restore(sys_logic_port_t port)
{
    uint32 trunkId;
    sys_logic_portmask_t lpm;

    if (IS_TRKLP(port))
        return SYS_ERR_OK;

    FOR_EACH_TRUNK(trunkId)
    {
        if (!IS_TRK_STATIC(trunkId))
            continue;

        SYS_MEM_CLEAR(lpm);

        SYS_ERR_CONTINUE(sal_trunk_member_get(trunkId, &lpm));

        if (IS_LOGIC_PORTMASK_PORTSET(lpm, port))
        {
            LOGIC_PORTMASK_CLEAR_PORT(lpm, port);
            SYS_ERR_CHK(sal_trunk_staticMember_set(trunkId, &lpm));
            break;
        }
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LACP

int32 fds_lacp_port_restore(sys_logic_port_t port)
{
    uint32 trunkId;
    sys_logic_portmask_t lpm;
    sys_logic_portmask_t actLpm;

    if (IS_TRKLP(port))
        return SYS_ERR_OK;

    FOR_EACH_TRUNK(trunkId)
    {
        if (!IS_TRK_LACP(trunkId))
            continue;

        SYS_MEM_CLEAR(lpm);

        SYS_ERR_CONTINUE(sal_trunk_member_get(trunkId, &lpm));

        if (IS_LOGIC_PORTMASK_PORTSET(lpm, port))
        {
            SYS_MEM_CLEAR(actLpm);
            SYS_ERR_CHK(sal_lacp_activeMode_get(trunkId, &actLpm));

            LOGIC_PORTMASK_CLEAR_PORT(lpm, port);
            LOGIC_PORTMASK_CLEAR_PORT(actLpm, port);

            SYS_ERR_CHK(sal_lacp_member_set(trunkId, &lpm));
            SYS_ERR_CHK(sal_lacp_activeMode_set(trunkId, &actLpm));
            break;
        }
    }

    SYS_ERR_CHK(sal_lacp_portPriority_set(port, DFLT_LACP_PORT_PRIORITY));
    SYS_ERR_CHK(sal_lacp_portTimeout_set(port, DFLT_LACP_PORT_TIMEOUT));

    return SYS_ERR_OK;
}
#endif 

int32 fds_qos_port_restore(sys_logic_port_t port)
{
    SYS_ERR_CHK(sal_qos_portCos_set(port, DFLT_QOS_PORT_COS));

    SYS_ERR_CHK(sal_qos_portTrustState_set(port, DFLT_QOS_PORT_TRUST_STATE));

    SYS_ERR_CHK(sal_qos_portRemarkCos_set(port, DFLT_QOS_PORT_REMARK_COS));

    SYS_ERR_CHK(sal_qos_portRemarkDscp_set(port, DFLT_QOS_PORT_REMARK_DSCP));

    SYS_ERR_CHK(sal_qos_portRemarkIpp_set(port, DFLT_QOS_PORT_REMARK_IPP));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING

int32 fds_igmp_port_restore(sys_logic_port_t port)
{
    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    SYS_ERR_CHK(sal_igmp_portGroup_limit_set(port, DFLT_MCAST_PORT_LIMIT));
    SYS_ERR_CHK(sal_igmp_portGroup_excessAct_set( port, DFLT_IGMP_MAX_GROUP_ACT));
    #endif
    #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    SYS_ERR_CHK(sal_igmp_port_filter_set(port, DFLT_IGMP_PORT_PROFILE));
    #endif
    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING

int32 fds_mld_port_restore(sys_logic_port_t port)
{
    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    SYS_ERR_CHK(sal_mld_portGroup_limit_set(port, DFLT_MCAST_PORT_LIMIT));
    SYS_ERR_CHK(sal_mld_portGroup_excessAct_set( port, DFLT_MLD_MAX_GROUP_ACT));
    #endif
    #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    SYS_ERR_CHK(sal_mld_port_filter_set( port, DFLT_MLD_PORT_PROFILE));
    #endif
    return SYS_ERR_OK;
}
#endif

#endif

int32 fds_rate_port_restore(sys_logic_port_t port)
{
#ifdef CONFIG_SYS_BW_CTRL
    sys_qid_t qid;
#endif

#ifdef CONFIG_SYS_STORM_CTRL
    
    SYS_ERR_CHK(sal_rate_stormControlPortEnable_set(port, DFLT_RATE_STORM_PORT_EBL));
    SYS_ERR_CHK(sal_rate_stormControlRate_set(port, STORM_GROUP_UNICAST, DFLT_RATE_STORM_CTRL));
    SYS_ERR_CHK(sal_rate_stormControlRate_set(port, STORM_GROUP_MULTICAST, DFLT_RATE_STORM_CTRL));
    SYS_ERR_CHK(sal_rate_stormControlRate_set(port, STORM_GROUP_BROADCAST, DFLT_RATE_STORM_CTRL));
    SYS_ERR_CHK(sal_rate_portStormCtrlTypeSel_set(port, STORM_GROUP_UNICAST, STORM_SEL_UNKNOWN));
    SYS_ERR_CHK(sal_rate_portStormCtrlTypeSel_set(port, STORM_GROUP_MULTICAST, STORM_SEL_UNKNOWN));
    SYS_ERR_CHK(sal_rate_stormControlTypeEnable_set(port, STORM_GROUP_UNICAST, DFLT_RATE_STORM_UUCAST_EBL));
    SYS_ERR_CHK(sal_rate_stormControlTypeEnable_set(port, STORM_GROUP_MULTICAST, DFLT_RATE_STORM_MCAST_EBL));
    SYS_ERR_CHK(sal_rate_stormControlTypeEnable_set(port, STORM_GROUP_BROADCAST, DFLT_RATE_STORM_BCAST_EBL));

    SYS_ERR_CHK(sal_rate_stormControlAction_set(port, DFLT_RATE_STORM_ACTION));
#endif 

#ifdef CONFIG_SYS_BW_CTRL
    
    SYS_ERR_CHK(sal_rate_igrBandwidthCtrlRate_set(port, DFLT_RATE_PORT_IGR));
    SYS_ERR_CHK(sal_rate_igrBandwidthCtrlEnable_set(port, DFLT_RATE_PORT_IGR_EBL));

    if (PORT_TYPE_100M == LP2TYPE(port))
    {
        SYS_ERR_CHK(sal_rate_egrBandwidthCtrlRate_set(port, DFLT_RATE_PORT_EGR_FE));
    }
    else
    {
        SYS_ERR_CHK(sal_rate_egrBandwidthCtrlRate_set(port, DFLT_RATE_PORT_EGR_GE));
    }
    SYS_ERR_CHK(sal_rate_egrBandwidthCtrlEnable_set(port, DFLT_RATE_PORT_EGR_EBL));

    FOR_EACH_QID(qid)
    {
        if (PORT_TYPE_100M == LP2TYPE(port))
        {
            SYS_ERR_CHK(sal_rate_egrQueueCtrlRate_set(port,qid,DFLT_RATE_PORT_EGR_FE));
        }
        else
        {
            SYS_ERR_CHK(sal_rate_egrQueueCtrlRate_set(port,qid,DFLT_RATE_PORT_EGR_GE));
        }
        SYS_ERR_CHK(sal_rate_egrQueueCtrlEnable_set(port,qid,DFLT_RATE_PORT_EGR_EBL));
    }
#endif 

    return SYS_ERR_OK;
} 

#ifdef CONFIG_SYS_PROTO_STP

int32 fds_stp_port_restore(sys_logic_port_t port)
{
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    uint32 msti;
#endif

    SYS_ERR_CHK(sal_stp_portAdminEnable_set(port, DFLT_STP_PORT_EBL));
    SYS_ERR_CHK(sal_stp_portForwardBpdu_set(port, DFLT_STP_PORT_BPDU_FORWARD_EBL));
    SYS_ERR_CHK(sal_stp_portPathCost_set(port, DFLT_STP_PORT_PATHCOST));
    SYS_ERR_CHK(sal_stp_portPriority_set(port, DFLT_STP_PORT_PRI));
    SYS_ERR_CHK(sal_stp_portEdgePort_set(port, DFLT_STP_PORT_EDGE));
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    SYS_ERR_CHK(sal_stp_portBpduFilter_set(port, DFLT_STP_PORT_BPDU_FILTER));
#endif  
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    SYS_ERR_CHK(sal_stp_portBpduGuard_set(port, DFLT_STP_PORT_BPDU_GUARD));
#endif  
    SYS_ERR_CHK(sal_stp_portPointToPointMac_set(port, DFLT_STP_PORT_P2P));

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    for (msti=0; msti<MSTP_INSTANCE_NUM; msti++)
    {
        SYS_ERR_CHK(sal_stp_mstPortPathCost_set(msti, port, DFLT_STP_MST_PORT_PATHCOST));
        SYS_ERR_CHK(sal_stp_mstPortPriority_set(msti, port, DFLT_STP_MST_PORT_PRI));
    }
#endif

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PROTO_LLDP

int32 fds_lldp_port_restore(sys_logic_port_t port)
{
    lldp_tlv_sel_t lldpTlvSel;
    sys_vlanmask_t vlanmask;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    lldp_med_tlv_sel_t lldpMedTlvSel;
    lldp_med_networkPolicy_bind_t policyBind;
    lldp_med_location_t location;
  #endif

    osal_memset(&lldpTlvSel, 0, sizeof(lldpTlvSel));
    osal_memset(&vlanmask, 0, sizeof(vlanmask));
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    osal_memset(&lldpMedTlvSel, 0, sizeof(lldpMedTlvSel));
    if(DFLT_LLDP_MED_TLV_SEL & (1 << LLDP_MED_CAPA_NETWORK_POLICY))
        lldpMedTlvSel.networkPolicySel = 1;
    if(DFLT_LLDP_MED_TLV_SEL & (1 << LLDP_MED_CAPA_LOCATION))
        lldpMedTlvSel.locationSel = 1;
    if(DFLT_LLDP_MED_TLV_SEL & (1 << LLDP_MED_CAPA_PSE))
        lldpMedTlvSel.poePseSel = 1;
    if(DFLT_LLDP_MED_TLV_SEL & (1 << LLDP_MED_CAPA_INVENTORY))
        lldpMedTlvSel.inventorySel = 1;
    osal_memset(&policyBind, 0, sizeof(policyBind));
    osal_memset(&location, 0, sizeof(location));
  #endif

    SYS_ERR_CHK(sal_lldp_port_set(port, DFLT_LLDP_PORT_STATE));
    SYS_ERR_CHK(sal_lldp_portTlvSel_set(port, lldpTlvSel));
    SYS_ERR_CHK(sal_lldp_portTlvPvidEnable_set(port, DFLT_LLDP_PORT_PVID_TLV_EBL));
    SYS_ERR_CHK(sal_lldp_portTlvVlanmask_set(port, vlanmask));
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_ERR_CHK(sal_lldp_portMedEnable_set(port, DFLT_LLDP_MED_PORT_EBL  ));
    SYS_ERR_CHK(sal_lldp_portMedTlvSel_set(port, lldpMedTlvSel));
    SYS_ERR_CHK(sal_lldp_portMedNetworkPolicyBind_set(port, policyBind));
    SYS_ERR_CHK(sal_lldp_portMedLocation_set(port, location));
  #endif

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PROTO_ISG

int32 fds_isg_port_restore(sys_logic_port_t port)
{
    SYS_ERR_CHK(sal_isg_port_set(port, DFLT_IPSG_PORT_EBL, ISG_VERIFY_IP));
    SYS_ERR_CHK(sal_isg_portMaxEntryNum_set(port, DFLT_IPSG_PORT_MAX_ENTRY));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

int32 fds_dhcp_snooping_port_restore(sys_logic_port_t port)
{
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    uint32 i = 0;
    sys_dhcp_option82_circuit_id_t circuitID[SYS_DHCP_O82_CID_VLAN_MAX+1];
#endif

    SYS_ERR_CHK(sal_dhcp_snooping_rateLimit_set(port, DFLT_DHCP_SNOOPING_RATE_LIMIT));

    SYS_ERR_CHK(sal_dhcp_option82_enable_set(port, DFLT_DHCP_SNOOPING_PORT_OPTION82_ENABLE));
    SYS_ERR_CHK(sal_dhcp_snooping_port_trust_set(port, DFLT_DHCP_SNOOPING_PORT_TRUST));
    SYS_ERR_CHK(sal_dhcp_option82_action_set(port, DFLT_DHCP_SNOOPING_PORT_OPTION82_ACTION));

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    
    SYS_MEM_CLEAR(circuitID);
    SYS_ERR_CHK(sal_dhcp_option82_circuitID_get(port, circuitID));

    for (i = 0; i <= SYS_DHCP_O82_CID_VLAN_MAX; i++)
    {
        if (0 != osal_strlen(circuitID[i].str))
        {
            SYS_MEM_CLEAR(circuitID[i].str);
            SYS_ERR_CHK(sal_dhcp_option82_circuitID_set(port, &circuitID[i]));
        }
    }
#endif

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_DAI

int32 fds_dai_port_restore(sys_logic_port_t port)
{
    SYS_ERR_CHK(sal_dai_rateLimit_set(port, DFLT_DAI_RATELIMIT_RATE));
    SYS_ERR_CHK(sal_dai_port_trust_set(port, DFLT_DAI_PORT_TRUST));
    SYS_ERR_CHK(sal_dai_port_smacChk_set(port, DFLT_DAI_SMAC_CHK));
    SYS_ERR_CHK(sal_dai_port_dmacChk_set(port, DFLT_DAI_DMAC_CHK));
    SYS_ERR_CHK(sal_dai_port_ipChk_set(port, DFLT_DAI_IPMAC_CHK));
    SYS_ERR_CHK(sal_dai_port_ipZeroAllow_set(port, DFLT_DAI_IPZEROALLOW_CHK));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION

int32 fds_dos_port_restore(sys_logic_port_t port)
{
    
    SYS_ERR_CHK(sal_dos_portAttackPreventEnable_set(port, DFLT_DOS_PORT_ATTACKPREVENT_EBL));

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PROTO_GVRP

int32 fds_gvrp_port_restore(sys_logic_port_t port)
{
    SYS_ERR_CHK(sal_gvrp_port_enable_set(port, DFLT_GVRP_PORT_EBL));
    SYS_ERR_CHK(sal_gvrp_port_regmode_set(port, DFLT_GVRP_PORT_REG));
    SYS_ERR_CHK(sal_gvrp_port_forbidVlan_set(port, DFLT_GVRP_PORT_FORBID));

    return SYS_ERR_OK;
}

#endif 

#ifdef CONFIG_SYS_PROTO_GARP

int32 fds_garp_port_restore(sys_logic_port_t port)
{
    SYS_ERR_CHK(sal_gmrp_port_enable_set(port, DFLT_GMRP_PORT_EBL));

    return SYS_ERR_OK;
}

#endif 

#ifdef CONFIG_SYS_USER_DEFINED_ACL

int32 fds_acl_port_restore(sys_logic_port_t port)
{
    sys_acl_type_t type;
    sys_logic_portmask_t lpm;

    SYS_MEM_CLEAR(lpm);
    LOGIC_PORTMASK_SET_PORT(lpm, port);

    for (type = SYS_ACL_MAC_BASE ; type < SYS_ACL_TYPE_END; type++)
        SYS_ERR_CHK(sal_acl_entry_unbind(type, lpm));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_UDLD

int32 fds_udld_port_restore(sys_logic_port_t port)
{
    
    SYS_ERR_CHK(sal_udld_portMode_set(port, DFLT_UDLD_PORT_MODE));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_POE
int32 fds_poe_port_restore(sys_logic_port_t port)
{
    uint32 i = 0;
    static int flag = 1;
    if(IS_POE_PORT(port))
    {
	    SYS_ERR_CHK(sal_poe_portEnable_set(port, ENABLED));
        for (i = 0; i < POE_WEEK_MAX; i++)
    	{
    		SYS_ERR_CHK(sal_poe_portSchedule_set(port, i, 0xffffff));
    	}
    	#ifdef CONFIG_SYS_POE_SWDOG
    	SYS_ERR_CHK(sal_poe_port_watch_dog_Enable_set(port, DISABLED));
    	#endif
    }
    else 
    {
    	if(flag)
    	{
			SYS_ERR_CHK(sal_poe_port_total_num_set(port));
			flag = 0;
		}
    }
    return SYS_ERR_OK;
}
#endif
