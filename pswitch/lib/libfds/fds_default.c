
#include <libfds/fds_default_init.h>
#include <libfds/fds_if_restore.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>
#ifdef CONFIG_SYS_PROTO_GARP
#include <libsal/sal_garp.h>
#endif

#ifdef CONFIG_SYS_MGMT_ACL
#include <libsal/sal_acl_mgmt.h>
#endif

#ifdef CONFIG_SYS_PROTO_UDLD
#include <libsal/sal_udld.h>
#endif
#ifdef CONFIG_SYS_PROTO_ERPS

#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
#include <libsal/sal_easycwmp.h>
#endif

#ifdef CONFIG_SYS_PROTO_ERPS
extern int32 sal_erps_enable_set(sys_enable_t enable);
#endif

#ifdef CONFIG_SYS_PROTO_ERPS
extern int32 sal_loopback_proto_enable_set(sys_enable_t enable);
#endif

int32 fds_sys_localuser_dfltEntry_init(void)
{
    sys_userInfo_t userInfo;

    osal_memset(&userInfo, 0, sizeof(userInfo));
    
    
    sal_sys_get_sys_defaul_user(&userInfo);
    userInfo.priv = DFLT_SYS_ADMIN_PRIV;
    userInfo.passwdType = SYS_PASSWD_TYPE_ENCRYPT;
    userInfo.accessMethod = DFLT_SYS_USER_ACCESS_METH;
    SYS_ERR_CHK(sal_sys_localUserForce_add(userInfo));

    return SYS_ERR_OK;
}
#if 0 
static int32 _fds_sys_define_Vlan_l3inf_init(void)
{
    sys_l3_intfdef_t mgmt;

    SYS_MEM_CLEAR(mgmt);

    mgmt.defId.type = SYS_L3_INTFTYPE_VLAN;
    mgmt.defId.id = DFLT_VLAN_DFLT_VLAN;
    
    mgmt.v4.dhcpType = DFLT_L3_IPV4_DHCP_TYPE;
    STR2IPV4(DFLT_L3_IPV4_ADDR, &mgmt.v4.ip.addr);
    mgmt.v4.ip.plen = DFLT_L3_IPV4_PREFIXLEN;

    mgmt.v6.enbl = DFLT_L3_IPV6_ADMIN_EBL;
    mgmt.v6.autoconfEnbl = DFLT_L3_IPV6_AUTOCONF_EBL;
    mgmt.v6.dhcpEnbl = DFLT_L3_IPV6_DHCPC_EBL;

    SYS_ERR_CHK(sal_l3_define_interface_set(&mgmt));

    return SYS_ERR_OK;
}

static int32 _fds_sys_mgmtVlan_init(void)
{
    sys_l3_intfMgmtAdmin_t mgmt;

    SYS_MEM_CLEAR(mgmt);

    mgmt.v4.dhcpType = DFLT_L3_IPV4_DHCP_TYPE;
    STR2IPV4(DFLT_L3_IPV4_ADDR, &mgmt.v4.ip.addr);
    mgmt.v4.ip.plen = DFLT_L3_IPV4_PREFIXLEN;
    STR2IPV4(DFLT_L3_IPV4_GW, &mgmt.v4.gw);

    mgmt.v6.enbl = DFLT_L3_IPV6_ADMIN_EBL;
    mgmt.v6.autoconfEnbl = DFLT_L3_IPV6_AUTOCONF_EBL;
    mgmt.v6.dhcpEnbl = DFLT_L3_IPV6_DHCPC_EBL;

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&mgmt));

    return SYS_ERR_OK;
}
#endif

int32 fds_sys_default_init(void)
{
    sys_enblPasswd_t enblPasswd;

    osal_memset(&enblPasswd, 0, sizeof(enblPasswd));
    osal_strcpy(enblPasswd.password, DFLT_SYS_ENABLE_PASSWORD);
    enblPasswd.passwdType = SYS_PASSWD_TYPE_ENCRYPT;
#ifdef CONFIG_SYS_AAA
    SYS_ERR_CHK(sal_sys_enablePassword_set(SYS_LOCALUSER_PRIV_ADMIN, enblPasswd));
#else
    
    SYS_ERR_CHK(sal_sys_enablePassword_set(0, enblPasswd));
#endif

    SYS_ERR_CHK(sal_default_time_set());
	#ifdef CONFIG_SYS_L3_ARCHITECTURE
	
	#else
    
	#endif
    
#ifdef CONFIG_SYS_ADVANCE_DNS
    SYS_ERR_CHK(sal_sys_dnsLookup_set(DFLT_SYS_ADV_DNS_ENBL));
    SYS_ERR_CHK(sal_sys_dnsDomain_set(DFLT_SYS_ADV_DNS_DOMAIN, SYS_ADVDNS_SRC_STATIC));
#endif
#ifdef CONFIG_SYS_APP_TELNETD
    
    SYS_ERR_CHK(sal_sys_telnetd_port_set(DEFAULT_TELNET_PORT_VAL));
    SYS_ERR_CHK(sal_sys_telnetdEnable_set(DFLT_SYS_TELNET_EBL));
#endif

#ifdef CONFIG_SYS_UI_WEB
    SYS_ERR_CHK(sal_sys_httpEnable_set(DFLT_SYS_HTTP_EBL));
#endif

#ifdef CONFIG_USER_BOA_WITH_SSL
    SYS_ERR_CHK(sal_sys_httpsEnable_set(DFLT_SYS_HTTPS_EBL));
#endif

#ifdef CONFIG_SYS_APP_OPENSSL
    
    if (SYS_ERR_FILE_NOT_EXIST == sal_util_file_exist(SSL_CERT_FILE))
        SYS_ERR_CHK(system("cp /etc/ssl* "SYS_SEC_FLH_KEY_DIR));
#endif

#ifdef CONFIG_USER_SSH_SSHD
    SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_RSA_PUBKEY_V2));
    SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_DSA_PUBKEY_V2));

    SYS_ERR_CHK(sal_sys_sshd_port_set(DEFAULT_SSH_PORT_VAL));
    SYS_ERR_CHK(sal_ssh_sshdEnable_set(DFLT_SYS_SSH_EBL));
#endif

    SYS_ERR_CHK(sal_sys_cliHistory_count_set(SYS_LINE_CLI_CONSOLE, DFLT_CLI_HISTORY_MAX_ENTRY));
    SYS_ERR_CHK(sal_sys_cliHistory_count_set(SYS_LINE_CLI_TELNET, DFLT_CLI_HISTORY_MAX_ENTRY));
  #ifdef CONFIG_USER_SSH_SSHD
    SYS_ERR_CHK(sal_sys_cliHistory_count_set(SYS_LINE_CLI_SSH, DFLT_CLI_HISTORY_MAX_ENTRY));
  #endif

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    {
        SYS_ERR_CHK(sal_sys_cliSessionTimeout_set(SYS_LINE_CLI_CONSOLE, DFLT_SYS_SESSION_TIMEOUT_CONSOLE));
        SYS_ERR_CHK(sal_sys_cliSessionTimeout_set(SYS_LINE_CLI_TELNET, DFLT_SYS_SESSION_TIMEOUT_TELNET));
      #ifdef CONFIG_USER_SSH_SSHD
        SYS_ERR_CHK(sal_sys_cliSessionTimeout_set(SYS_LINE_CLI_SSH, DFLT_SYS_SESSION_TIMEOUT_SSH));
      #endif
    }
#endif 
#ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT
    {
        SYS_ERR_CHK(sal_sys_webSessionTimeout_set(SYS_LINE_WEB_HTTP, DFLT_SYS_SESSION_TIMEOUT_HTTP));
        SYS_ERR_CHK(sal_sys_webSessionTimeout_set(SYS_LINE_WEB_HTTPS, DFLT_SYS_SESSION_TIMEOUT_HTTPS));
    }
#endif 
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    {
        sys_cliRetrySilent_t retrySilent;

        SYS_MEM_CLEAR(retrySilent);
        retrySilent.retryNum = DFLT_CLI_PASSRETRY_CONSOLE;
        retrySilent.silentTime = DFLT_CLI_SILENTTIME_CONSOLE;
        SYS_ERR_CHK(sal_sys_cliRetrySilent_set(SYS_LINE_CLI_CONSOLE, retrySilent));

        SYS_MEM_CLEAR(retrySilent);
        retrySilent.retryNum = DFLT_CLI_PASSRETRY_TELNET;
        retrySilent.silentTime = DFLT_CLI_SILENTTIME_TELNET;
        SYS_ERR_CHK(sal_sys_cliRetrySilent_set(SYS_LINE_CLI_TELNET, retrySilent));

      #ifdef CONFIG_USER_SSH_SSHD
        SYS_MEM_CLEAR(retrySilent);
        retrySilent.retryNum = DFLT_CLI_PASSRETRY_SSH;
        retrySilent.silentTime = DFLT_CLI_SILENTTIME_SSH;
        SYS_ERR_CHK(sal_sys_cliRetrySilent_set(SYS_LINE_CLI_SSH, retrySilent));
      #endif
    }
#endif 

    return SYS_ERR_OK;
}

int32 fds_port_default_init(void)
{
    sys_logic_port_t port;

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    uint32 i = 0;

    for (i = (SYS_PORT_ERRDISABLE_NONE + 1); i < SYS_PORT_ERRDISABLE_END; i++)
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(i, DFLT_PORT_ERRDISABLE_RECOVERY));
    }

    SYS_ERR_CHK(sal_port_errDisableTime_set(DFLT_PORT_ERRDISABLE_RECOVERY_INTERVAL));
#endif

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_port_port_restore(port));
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PORT_SECURITY

int32 fds_psecure_default_init(void)
{
    sys_logic_port_t port;
    sys_psecure_port_t portCfg;

    SYS_ERR_CHK(sal_psecure_enable_set(DFLT_PSECURE_EBL));
    SYS_ERR_CHK(sal_psecure_rateLimit_set(DFLT_PSECURE_RATELIMIT));

    SYS_MEM_CLEAR(portCfg);
    portCfg.enable = DFLT_PSECURE_PORT_EBL;
    portCfg.limitNum = DFLT_PSECURE_PORT_LIMITNUM;
    portCfg.violation = DFLT_PSECURE_PORT_VIOLATION;
    portCfg.sticky = DFLT_PSECURE_PORT_STICKY;

    FOR_EACH_NORMAL_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_psecure_port_set(port, &portCfg));
    }

    return SYS_ERR_OK;
}
#endif

int32 fds_switch_default_init(void)
{
#ifdef CONFIG_SYS_JUMBO_FRAME
    SYS_ERR_CHK(sal_switch_maxFrameSize_set(DFLT_PORT_MAX_FRAME_SIZE));
#endif 

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_EEE

int32 fds_eee_default_init(void)
{
    sys_logic_port_t port;

    FOR_EACH_NORMAL_LOGIC_PORT(port)
    {
        
        SYS_ERR_CHK(fds_eee_port_restore(port));
    }

    return SYS_ERR_OK;
}
#endif 

int32 fds_vlan_default_init(void)
{
    sys_logic_port_t port;
    sys_vlanmask_t vlanmask;
    char *pVlanStrPrefix = DFLT_VLAN_PREFIX_NAME;
    sys_namestr_t name;

    SYS_ERR_CHK(fds_vlan_dfltVlan_init());
    SYS_ERR_CHK(sal_vlan_nameStr_get(DFLT_VLAN_DFLT_VLAN, pVlanStrPrefix, &name));

    SYS_ERR_CHK(STR2VMSK(DFLT_VLAN_TRUNK_ALLOW_VLAN, &vlanmask));

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    if (DFLT_VLAN_MGMT_VLAN != DFLT_VLAN_DFLT_VLAN)
    {
        SYS_ERR_CHK(sal_vlan_nameStr_get(DFLT_VLAN_MGMT_VLAN, pVlanStrPrefix, &name));
        SYS_ERR_CHK(sal_vlan_adminEntry_create(DFLT_VLAN_MGMT_VLAN, &name));
    }
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    if ((DFLT_AUTHMGR_GUEST_VLAN != DFLT_VLAN_DFLT_VLAN) && (DFLT_AUTHMGR_GUEST_VLAN != 0))
    {
        SYS_ERR_CHK(sal_vlan_nameStr_get(DFLT_AUTHMGR_GUEST_VLAN, pVlanStrPrefix, &name));
        SYS_ERR_CHK(sal_vlan_adminEntry_create(DFLT_AUTHMGR_GUEST_VLAN, &name));
    }
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    if ((DFLT_VLAN_VOICE_VLAN != DFLT_VLAN_DFLT_VLAN) && (DFLT_VLAN_VOICE_VLAN != DFLT_VLAN_RESERVED_VLAN))
    {
        SYS_ERR_CHK(sal_vlan_nameStr_get(DFLT_VLAN_VOICE_VLAN, pVlanStrPrefix, &name));
        SYS_ERR_CHK(sal_vlan_adminEntry_create(DFLT_VLAN_VOICE_VLAN, &name));
    }
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    if ((DFLT_VLAN_SURVEILLANCE_VLAN != DFLT_VLAN_DFLT_VLAN) && (DFLT_VLAN_SURVEILLANCE_VLAN != DFLT_VLAN_RESERVED_VLAN))
    {
        SYS_ERR_CHK(sal_vlan_nameStr_get(DFLT_VLAN_SURVEILLANCE_VLAN, pVlanStrPrefix, &name));
        SYS_ERR_CHK(sal_vlan_adminEntry_create(DFLT_VLAN_SURVEILLANCE_VLAN, &name));
    }
#endif

    SYS_ERR_CHK(sal_vlan_isChkVlanPortMode_set(DFLT_VLAN_PORT_MODE_CHK));

#ifdef CONFIG_SYS_VLAN_QINQ
    SYS_ERR_CHK(sal_vlan_dot1qTunnelTpid_set(0, DFLT_VLAN_DOT1QTUNNEL_TPID_0));
    SYS_ERR_CHK(sal_vlan_dot1qTunnelTpid_set(1, DFLT_VLAN_DOT1QTUNNEL_TPID_1));
    SYS_ERR_CHK(sal_vlan_dot1qTunnelTpid_set(2, DFLT_VLAN_DOT1QTUNNEL_TPID_2));
    SYS_ERR_CHK(sal_vlan_dot1qTunnelTpid_set(3, DFLT_VLAN_DOT1QTUNNEL_TPID_3));
#endif

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_vlan_portAdminMode_set(port, DFLT_VLAN_PORT_MODE));
        SYS_ERR_CHK(sal_vlan_portAdminRoleUplinkEnable_set(port, DFLT_VLAN_TRUNK_UPLINK));
        SYS_ERR_CHK(sal_vlan_portAdminAcceptFrameType_set(port, DFLT_VLAN_ACCEPT_FRAME_TYPE));
        SYS_ERR_CHK(sal_vlan_portAdminIgrFilterEnable_set(port, DFLT_VLAN_IGR_FILTER));
        SYS_ERR_CHK(sal_vlan_portAdminPvid_set(port, DFLT_VLAN_PVID));

        SYS_ERR_CHK(sal_vlan_portTrunkAllowedVlan_set(port, &vlanmask));

        SYS_ERR_CHK(sal_vlan_portDot1qTunnelTpid_set(port, DFLT_VLAN_PORT_TPID));
    }

#ifdef CONFIG_SYS_PROTOCOL_VLAN
    SYS_ERR_CHK(fds_vlan_protocolVlan_init());
#endif

#ifdef CONFIG_SYS_VOICEVLAN
    SYS_ERR_CHK(fds_vlan_voiceVlan_init());
#endif

    return SYS_ERR_OK;
}

int32 fds_vlan_dfltVlan_init(void)
{
    sys_namestr_t vlanName;
    sys_vid_t defaultVid = DFLT_VLAN_DFLT_VLAN;
    sys_logic_port_t port;

    SYS_MEM_CLEAR(vlanName);
    osal_strcpy(vlanName.str, DFLT_VLAN_DFLT_NAME);

    SYS_ERR_CHK(sal_vlan_adminEntry_create(defaultVid, &vlanName));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_set(port, defaultVid, DFLT_DEFAULT_VLAN_MEMBERSHIP));
    }

    SYS_ERR_CHK(sal_vlan_defaultVid_set(defaultVid));

        SYS_ERR_CHK(sal_vlan_defaultVid_get(&defaultVid));

#if defined(CONFIG_SYS_MANAGEMENT_VLAN) && !defined(CONFIG_SYS_L3_ARCHITECTURE)
    
    SYS_ERR_CHK(sal_vlan_mgmtVid_set(DFLT_VLAN_MGMT_VLAN));
#endif

    return SYS_ERR_OK;
}

int32 fds_vlan_dfltVlan_dfltEntry_init(void)
{

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTOCOL_VLAN

int32 fds_vlan_protocolVlan_init(void)
{
    uint32 i = 0;
    sys_logic_port_t port;
    sys_vlan_protoGroup_t protoGroup;

    SYS_MEM_CLEAR(protoGroup);
    protoGroup.frametype = FRAME_TYPE_END;

    for(i = 0; i < CAPA_VLAN_PROTO_GROUP_NUM; i++)
        SYS_ERR_CHK(sal_vlan_protoGroup_set(i, &protoGroup));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_vlan_protoVlan_port_restore(port));
    }

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_IP_SUBNET

int32 fds_vlan_ip_subnet_vlan_init(void)
{
    uint32 i = 0;
    sys_logic_port_t port;
    sys_ipSubnet_vlan_group_t protoGroup;

    SYS_MEM_CLEAR(protoGroup);

    for(i = 0; i < CAPA_VLAN_IP_SUBNET_GROUP_NUM; i++)
        SYS_ERR_CHK(sal_vlan_ip_subnet_group_init(i, &protoGroup));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_vlan_ip_subnet_port_init(port, DISABLED));
    }

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_VOICEVLAN

int32 fds_vlan_voiceVlanOui_dfltEntry_init(void)
{
    SYS_ERR_CHK(sal_vlan_voiceVlanOui_fdsEntry_set());

    return SYS_ERR_OK;
}

int32 fds_vlan_voiceVlan_init(void)
{
    sys_logic_port_t port;
    sys_vlan_voiceQos_t voiceQos;

    

    SYS_ERR_CHK(sal_vlan_voiceVlanState_set(DFLT_VLAN_VOICE_EBL));

    SYS_ERR_CHK(sal_vlan_voiceVlanAging_set(DFLT_VLAN_VOICE_AGING));

    SYS_ERR_CHK(sal_vlan_voiceVid_set(DFLT_VLAN_VOICE_VLAN));
    SYS_MEM_CLEAR(voiceQos);
    voiceQos.pri = DFLT_VLAN_VOICE_PRI;

    SYS_ERR_CHK(sal_vlan_voiceVlanQos_set(voiceQos));
    
    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_vlan_voiceVlan_port_restore(port));
    }
    SYS_ERR_CHK(sal_vlan_voiceVlan1pRemarkEnable_set(DFLT_VLAN_VOICE_REMARK_EBL));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN

int32 fds_surveillance_vlan_dfltEntry_init(void)
{
    surveillance_vlan_oui_t oui;

    SYS_MEM_CLEAR(oui);

    oui.oui[0] = 0xCC;
    oui.oui[1] = 0xEE;
    oui.oui[2] = 0xAA;
    osal_sprintf(oui.ouiName.str, "%s", "test");

    

    return SYS_ERR_OK;
}

int32 fds_surveillance_vlan_init(void)
{
    sys_logic_port_t lp;

    SYS_ERR_CHK(sal_surveillance_vlanVid_set(DFLT_VLAN_SURVEILLANCE_VLAN));
    SYS_ERR_CHK(sal_surveillance_vlanPri_set(DFLT_VLAN_SURVEILLANCE_PRI));
    SYS_ERR_CHK(sal_surveillance_vlanAging_set(DFLT_VLAN_SURVEILLANCE_AGING));
    SYS_ERR_CHK(sal_surveillance_vlan1pRemarkEnable_set(DFLT_VLAN_SURVEILLANCE_REMARK_EBL));
    SYS_ERR_CHK(sal_surveillance_vlanState_set(DFLT_VLAN_SURVEILLANCE_EBL));

    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_ERR_CHK(fds_surveillance_vlan_port_restore(lp));
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_MIRROR

int32 fds_mirror_default_init(void)
{
    return SYS_ERR_OK;
}
#endif

int32 fds_l2_default_init(void)
{
    SYS_ERR_CHK(sal_l2_aging_set(DFLT_L2_AGING_TIME));

    return SYS_ERR_OK;
} 

#ifdef CONFIG_SYS_L3_ROUTE

int32 fds_l3_default_init(void)
{
    SYS_ERR_CHK(sal_l3_fwdIPv4Enable_set(DFLT_L3_IPV4_FORWARDING_EBL));
    SYS_ERR_CHK(sal_l3_fwdIPv6Enable_set(DFLT_L3_IPV6_FORWARDING_EBL));
    SYS_ERR_CHK(sal_l3_hopLimitIPv6_set(DFLT_L3_IPV6_HOPLIMIT));
    SYS_ERR_CHK(sal_l3_icmpv6RateInterval_set(DFLT_L3_IPV6_ICMP_RATE_INTVL));
    SYS_ERR_CHK(sal_l3_icmpv6RateBurst_set(DFLT_L3_IPV6_ICMP_RATE_BURST));
    SYS_ERR_CHK(sal_l3_routev6Prefer_set(DFLT_L3_IPV6_ROUTE_PREFER));
    SYS_ERR_CHK(sal_l3_neigh_ageTime_set(DFLT_L3_NEIGH_AGE_TIME));
    SYS_ERR_CHK(sal_l3_neigh_respTime_set(DFLT_L3_NEIGH_RESP_TIME));
    SYS_ERR_CHK(sal_l3_neigh_retryTimes_set(DFLT_L3_NEIGH_RETRY_TIMES));
    SYS_ERR_CHK(sal_l3_neigh_cacheSize_set(DFLT_L3_NEIGH_CACHE_SIZE));
    SYS_ERR_CHK(sal_l3_neigh_renew_set(DFLT_L3_NEIGH_RENEW));

    return SYS_ERR_OK;
}
#endif

int32 fds_trunk_default_init(void)
{
    SYS_ERR_CHK(sal_trunk_hashAlgo_set(DFLT_LAG_HASH_ALGO));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LACP

int32 fds_lacp_default_init(void)
{
    sys_logic_port_t logicPort;

    SYS_ERR_CHK(sal_lacp_enable_set(DFLT_LACP_EBL));
    SYS_ERR_CHK(sal_lacp_systemPriority_set(DFLT_LACP_SYSTEM_PRIORITY));

    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        SYS_ERR_CHK(fds_lacp_port_restore(logicPort));
    }

    return SYS_ERR_OK;
}
#endif 

int32 fds_qos_default_init(void)
{
    sys_logic_port_t port;
    sys_qos_queueWeight_t qWeight;

    osal_memset(&qWeight, 0, sizeof(qWeight));

    qWeight.weights[0] = DFLT_QOS_QUEUE0_WEIGHT;
    qWeight.weights[1] = DFLT_QOS_QUEUE1_WEIGHT;
    qWeight.weights[2] = DFLT_QOS_QUEUE2_WEIGHT;
    qWeight.weights[3] = DFLT_QOS_QUEUE3_WEIGHT;
    qWeight.weights[4] = DFLT_QOS_QUEUE4_WEIGHT;
    qWeight.weights[5] = DFLT_QOS_QUEUE5_WEIGHT;
    qWeight.weights[6] = DFLT_QOS_QUEUE6_WEIGHT;
    qWeight.weights[7] = DFLT_QOS_QUEUE7_WEIGHT;

    SYS_ERR_CHK(sal_qos_queueWeight_set(qWeight));

    SYS_ERR_CHK(sal_qos_strictPriQueueNum_set(DFLT_QOS_STRICT_PRI_QUEUE_NUM));

    SYS_ERR_CHK(sal_qos_trustType_set(DFLT_QOS_TRUST_TYPE));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_qos_port_restore(port));
    }

    SYS_ERR_CHK(sal_qos_mapDefault_restore(SYS_QOS_MAP_COS2QUEUE));
    SYS_ERR_CHK(sal_qos_mapDefault_restore(SYS_QOS_MAP_DSCP2QUEUE));
    SYS_ERR_CHK(sal_qos_mapDefault_restore(SYS_QOS_MAP_IPP2QUEUE));

    SYS_ERR_CHK(sal_qos_mode_set(DFLT_QOS_MODE));

    SYS_ERR_CHK(sal_qos_mapDefault_restore(SYS_QOS_MAP_QUEUE2COS));
    SYS_ERR_CHK(sal_qos_mapDefault_restore(SYS_QOS_MAP_QUEUE2DSCP));
    SYS_ERR_CHK(sal_qos_mapDefault_restore(SYS_QOS_MAP_QUEUE2IPP));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_UI_SNMP

int32 fds_snmp_default_init(void)
{
#ifdef CONFIG_SYS_MIB_TRAP
    uint32 i = 0;
#endif
#ifdef CONFIG_SYS_SNMPV3
    char msk[20];
    char engineStr[BOND_SNMP_ENGINEID_LEN_MAX+1];
    char sourceNet[] = "default";
    sal_snmp_access_t access;
    sal_snmp_com2sec_t comEntry;
    sal_snmp_oid_t  subtree;
#endif

    SYS_ERR_CHK(sal_snmp_statistic_init());

#ifdef CONFIG_SYS_MIB_TRAP
    for (i = 0; i < SNMP_TRAP_CLASS_END ; i++)
        SYS_ERR_CHK(sal_snmp_trapEnable_set(i, DFLT_SNMP_TRAP_EBL));
#endif

#ifdef CONFIG_SYS_SNMPV3
    osal_memset(msk, 0, sizeof(msk));
    SYS_ERR_CHK(sal_util_str2oid(DFLT_SNMP_VIEW_SUBTREE_STR, &subtree.len, subtree.id));

    SYS_ERR_CHK(sal_snmp_view_add(DFLT_SNMP_VIEW_STR, &subtree, DFLT_SNMP_VIEWTYPE, msk));

    osal_memset(&subtree, 0, sizeof(sal_snmp_oid_t));
    osal_memset(&access, 0, sizeof(sal_snmp_access_t));
    osal_memset(&comEntry, 0, sizeof(sal_snmp_com2sec_t));

    osal_sprintf(access.groupName, "%s%s", SNMP_ACCESS_GROUP_PREFIX, DFLT_SNMP_COMMUNITY);
    access.secMode = SNMP_SECMODEL_V1;
    access.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
    access.context[0] = '\0';
    access.prefix = SNMP_PREFIX_EXACT;
    osal_strcpy(access.readView, DFLT_SNMP_VIEW_STR);
    osal_strcpy(access.writeView, "none");
    access.viewcfMode = SNMP_CONFIG_MANUAL;
    access.groupcfMode = SNMP_CONFIG_AUTO;
    SYS_ERR_CHK(sal_snmp_access_add(&access));

    access.secMode = SNMP_SECMODEL_V2C;
    SYS_ERR_CHK(sal_snmp_access_add(&access));

    SYS_ERR_CHK(sal_snmp_group_add(DFLT_SNMP_COMMUNITY, SNMP_SECMODEL_V1, access.groupName));
    SYS_ERR_CHK(sal_snmp_group_add(DFLT_SNMP_COMMUNITY, SNMP_SECMODEL_V2C, access.groupName));

    osal_strcpy(comEntry.secName, DFLT_SNMP_COMMUNITY);
    osal_strcpy(comEntry.comName, DFLT_SNMP_COMMUNITY);
    osal_strcpy(comEntry.sourceNet, sourceNet);
    osal_sprintf(comEntry.groupName, "%s%s", SNMP_ACCESS_GROUP_PREFIX, DFLT_SNMP_COMMUNITY);
    osal_strcpy(comEntry.viewName, DFLT_SNMP_VIEW_STR);
    comEntry.access = DFLT_SNMP_COMMUNITY_ACCESS;
    comEntry.mode =   SNMP_COM_MODE_BASIC;
    SYS_ERR_CHK(sal_snmp_com2sec_add(&comEntry));

    osal_memset(engineStr, 0, sizeof(engineStr));
    SYS_ERR_CHK(sal_snmp_defaultEngineId_get(engineStr));
    SYS_ERR_CHK(sal_snmp_engineid_set(engineStr));

    SYS_ERR_CHK(sal_snmp_config_reload_set(SNMP_RELOAD_AUTO));
#endif

    SYS_ERR_CHK(sal_snmp_enable_set(DFLT_SNMP_EBL));

    return SYS_ERR_OK;
}

#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING

int32 fds_igmp_default_init(void)
{
    sys_logic_port_t  port;
    mcast_vlan_entry_t mcast_vlan;

    osal_memset(&mcast_vlan, 0, sizeof(mcast_vlan));

    SYS_ERR_CHK(sal_igmp_lookupMode_set(DFLT_IGMP_LOOKUP_MODE));
    SYS_ERR_CHK(sal_igmp_enable_set(DFLT_IGMP_EBL));
    SYS_ERR_CHK(sal_igmp_suppre_enable_set(DFLT_IGMP_SUPPRE_EBL));
    SYS_ERR_CHK(sal_igmp_operVersion_set(DFLT_IGMP_OPERATION_VER));
    SYS_ERR_CHK(sal_igmp_unknMcastAction_set(DFLT_IGMP_UNKNOWN_MCAST_ACTION));

    
    SYS_ERR_CHK(cfg_igmp_unknMcastAction_set(DFLT_IGMP_UNKNOWN_MCAST_ACTION));

    FOR_EACH_LOGIC_PORT(port)
    {
        #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
        SYS_ERR_CHK(sal_igmp_portGroup_limit_set(port, DFLT_MCAST_PORT_LIMIT));
        SYS_ERR_CHK(sal_igmp_portGroup_excessAct_set( port, DFLT_IGMP_MAX_GROUP_ACT));
        #endif
        #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
        SYS_ERR_CHK(sal_igmp_port_filter_set(port, DFLT_IGMP_PORT_PROFILE));
        #endif
    }

    mcast_vlan.vid = DFLT_VLAN_DFLT_VLAN;
    mcast_vlan.ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan.enable = DFLT_IGMP_VLAN_EBL;
    mcast_vlan.pimDvmrpLearn = DFLT_IGMP_ROUTER_PIMDVMRP_LEARN;

    mcast_vlan.fastleave_enable = DFLT_IGMP_IMMEDIATE_LEAVE;

    mcast_vlan.router_port_timeout =  DFLT_IGMP_ROUTER_TIMEOUT;
    mcast_vlan.grpMembershipIntv = DFLT_IGMP_GROUP_MEMBERSHIP_INTV;

    mcast_vlan.robustnessVar = DFLT_IGMP_ROBUSTNESS_VAR;
    mcast_vlan.operRobNessVar = DFLT_IGMP_ROBUSTNESS_VAR;
    mcast_vlan.queryIntv = DFLT_IGMP_QUERY_INTV;
    mcast_vlan.operQueryIntv = DFLT_IGMP_QUERY_INTV;
    mcast_vlan.lastMmbrQueryCnt = DFLT_IGMP_ROBUSTNESS_VAR;
    mcast_vlan.operLastMmbrQueryCnt =  DFLT_IGMP_ROBUSTNESS_VAR;
    mcast_vlan.grpSpecificQueryIntv = DFLT_IGMP_GSQUERY_RESPONSE_INTV ;
    mcast_vlan.operGsqueryIntv = DFLT_IGMP_GSQUERY_RESPONSE_INTV ;
    mcast_vlan.responseTime = DFLT_IGMP_QUERY_RESPONSE_INTV;
    mcast_vlan.operRespTime = DFLT_IGMP_QUERY_RESPONSE_INTV;

    mcast_vlan.otherQuerierPresentInterval = (DFLT_IGMP_ROBUSTNESS_VAR * DFLT_IGMP_QUERY_INTV) + DFLT_IGMP_QUERY_RESPONSE_INTV/2;

    SYS_ERR_CHK(sal_mcast_vlan_set(&mcast_vlan));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_MVR
int32 fds_mvr_default_init(void)
{
    sys_logic_port_t  port;

    SYS_ERR_CHK(sal_igmp_mvr_enable_set(DFLT_IGMP_MVR_ENABLED));
    SYS_ERR_CHK(sal_igmp_mvr_mode_set(DFLT_IGMP_MVR_MODE));
    SYS_ERR_CHK(sal_igmp_mvr_querytime_set(DFLT_IGMP_MVR_QUERYTIME));

    
    if (DISABLED == DFLT_IGMP_MVR_ENABLED)
        SYS_ERR_CHK(cfg_igmp_mvr_vlanId_set(DFLT_IGMP_MVR_VLAN));
    else
        SYS_ERR_CHK(sal_igmp_mvr_vlanId_set(DFLT_IGMP_MVR_VLAN));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_igmp_mvr_portType_set(port, DFLT_IGMP_MVR_PORT_TYPE));
        SYS_ERR_CHK(sal_igmp_mvr_portImmediate_set(port, DFLT_IGMP_MVR_IMMEDIATE_LEAVE));
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING

int32 fds_mld_default_init(void)
{
    mcast_vlan_entry_t mcast_vlan;
    sys_logic_port_t port;

    osal_memset(&mcast_vlan, 0, sizeof(mcast_vlan));

    SYS_ERR_CHK(sal_mld_enable_set(DFLT_MLD_EBL));
    SYS_ERR_CHK(sal_mld_operVersion_set(DFLT_MLD_OPERATION_VER));
    SYS_ERR_CHK(sal_mld_suppre_enable_set(DFLT_MLD_SUPPRE_EBL));
    SYS_ERR_CHK(sal_mld_lookupMode_set(DFLT_MLD_LOOKUP_MODE));

    FOR_EACH_LOGIC_PORT(port)
    {
        #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
        SYS_ERR_CHK(sal_mld_portGroup_limit_set(port, DFLT_MCAST_PORT_LIMIT));
        SYS_ERR_CHK(sal_mld_portGroup_excessAct_set( port, DFLT_MLD_MAX_GROUP_ACT));
        #endif
        #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
        SYS_ERR_CHK(sal_mld_port_filter_set( port, DFLT_MLD_PORT_PROFILE));
        #endif
    }

    mcast_vlan.vid = DFLT_VLAN_DFLT_VLAN;
    mcast_vlan.ipType = MULTICAST_TYPE_IPV6;
    mcast_vlan.enable = DFLT_MLD_VLAN_EBL;
    mcast_vlan.pimDvmrpLearn = DFLT_MLD_ROUTER_PIMDVMRP_LEARN;

    mcast_vlan.fastleave_enable = DFLT_MLD_IMMEDIATE_LEAVE;

    mcast_vlan.router_port_timeout =  DFLT_MLD_ROUTER_TIMEOUT;
    mcast_vlan.grpMembershipIntv = DFLT_MLD_GROUP_MEMBERSHIP_INTV;

    mcast_vlan.robustnessVar = DFLT_MLD_ROBUSTNESS_VAR;
    mcast_vlan.operRobNessVar = DFLT_MLD_ROBUSTNESS_VAR;
    mcast_vlan.queryIntv = DFLT_MLD_QUERY_INTV;
    mcast_vlan.operQueryIntv = DFLT_MLD_QUERY_INTV;
    mcast_vlan.lastMmbrQueryCnt = DFLT_MLD_ROBUSTNESS_VAR;
    mcast_vlan.operLastMmbrQueryCnt =  DFLT_MLD_ROBUSTNESS_VAR;
    mcast_vlan.grpSpecificQueryIntv = DFLT_MLD_GS_QUERY_RESP_INTV;
    mcast_vlan.operGsqueryIntv = DFLT_MLD_GS_QUERY_RESP_INTV;
    mcast_vlan.responseTime = DFLT_MLD_QUERY_RESPONSE_INTV;
    mcast_vlan.operRespTime = DFLT_MLD_QUERY_RESPONSE_INTV;

    mcast_vlan.otherQuerierPresentInterval = (DFLT_MLD_ROBUSTNESS_VAR * DFLT_MLD_QUERY_INTV) + DFLT_MLD_QUERY_RESPONSE_INTV/2;

    SYS_ERR_CHK(sal_mcast_vlan_set(&mcast_vlan));

    return SYS_ERR_OK;
}
#endif

#endif

int32 fds_rate_default_init(void)
{
#if defined(CONFIG_SYS_STORM_CTRL) || defined(CONFIG_SYS_BW_CTRL)
    sys_rate_portDflt_t dfltInfo;

    SYS_MEM_CLEAR(dfltInfo);
#endif

#ifdef CONFIG_SYS_BW_CTRL
    SYS_ERR_CHK(sal_rate_igrBandwidthCtrlIncludeIfg_set(DFLT_RATE_FWD_IGR_PREABMLE));
    SYS_ERR_CHK(sal_rate_egrBandwidthCtrlIncludeIfg_set(DFLT_RATE_FWD_IGR_PREABMLE));
    
    SYS_ERR_CHK(sal_rate_igrBandwidthCtrlBurstSize_set(DFLT_RATE_IGR_BURST_THRESH));
    SYS_ERR_CHK(sal_rate_egrBandwidthCtrlBurstSize_set(DFLT_RATE_PORT_EGR_BURST_THRESH));
    SYS_ERR_CHK(sal_rate_egrSysQueueCtrlBurstSize_set(DFLT_RATE_QUEUE_EGR_BURST_THRESH));
#endif 
#ifdef CONFIG_SYS_STORM_CTRL
    SYS_ERR_CHK(sal_rate_stormCtrlIncludeIfg_set(DFLT_RATE_STORM_PREABMLE));
    SYS_ERR_CHK(sal_rate_stormCtrlRefreshMode_set(DFLT_RATE_STORM_MODE));
#endif 
#ifdef CONFIG_SYS_STORM_CTRL
    dfltInfo.strmRate = DFLT_RATE_STORM_CTRL;
    dfltInfo.strmEnbl = DFLT_RATE_STORM_PORT_EBL;
    dfltInfo.strmUucastEnbl = DFLT_RATE_STORM_UUCAST_EBL;
    dfltInfo.strmUmcastEnbl = DFLT_RATE_STORM_UMCAST_EBL;
    dfltInfo.strmMcastEnbl = DFLT_RATE_STORM_MCAST_EBL;
    dfltInfo.strmBcastEnbl = DFLT_RATE_STORM_BCAST_EBL;
    dfltInfo.strmAction = DFLT_RATE_STORM_ACTION;
#endif
#ifdef CONFIG_SYS_BW_CTRL
    dfltInfo.bwIgrsRate = DFLT_RATE_PORT_IGR;
    dfltInfo.bwIgrsEnbl = DFLT_RATE_PORT_IGR_EBL;
    dfltInfo.bwEgrsRateFE = DFLT_RATE_PORT_EGR_FE;
    dfltInfo.bwEgrsRateGE = DFLT_RATE_PORT_EGR_GE;
    dfltInfo.bwEgrsEnbl = DFLT_RATE_PORT_EGR_EBL;
    dfltInfo.bwEgrsQueueEnbl = DFLT_RATE_PORT_EGR_EBL;
#endif
#if defined(CONFIG_SYS_STORM_CTRL) || defined(CONFIG_SYS_BW_CTRL)
    SYS_ERR_CHK(sal_rate_portDflt_init(dfltInfo));
#endif

    return SYS_ERR_OK;
} 

#ifdef CONFIG_SYS_PROTO_STP

int32 fds_stp_default_init(void)
{
    sys_logic_port_t port;

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    sys_vlanmask_t vlanmask;
    sys_mac_t macaddr;
    char macaddr_str[32] = "";
    sys_stp_mst_dflt_t dfltInfo;
#endif

    SYS_ERR_CHK(sal_stp_enable_set(DFLT_STP_EBL));
    SYS_ERR_CHK(sal_stp_forwardBpdu_set(DFLT_STP_BPDU_FORWARD_EBL));
    SYS_ERR_CHK(sal_stp_forceVersion_set(DFLT_STP_FORCE_VERSION));
    SYS_ERR_CHK(sal_stp_priority_set(DFLT_STP_PRI));
    SYS_ERR_CHK(sal_stp_helloTime_set(DFLT_STP_HELLO_TIME));
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    SYS_ERR_CHK(sal_stp_maxHops_set(DFLT_STP_MAX_HOPS));
#endif
    SYS_ERR_CHK(sal_stp_forwardDelay_set(DFLT_STP_FORWARD_DELAY));
    SYS_ERR_CHK(sal_stp_maximumAge_set(DFLT_STP_MAX_AGE));
    SYS_ERR_CHK(sal_stp_txHoldCount_set(DFLT_STP_TX_HOLD_COUNT));
    SYS_ERR_CHK(sal_stp_pathcostMethod_set(DFLT_STP_PATHCOST_METHOD));

    FOR_EACH_LOGIC_PORT(port)
    {
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
    }

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    if(0 != osal_strlen(DFLT_STP_MST_CFG_NAME))
    {
        SYS_ERR_CHK(sal_stp_mstConfigName_set(DFLT_STP_MST_CFG_NAME));
    }
    else
    {
        SYS_ERR_CHK(sal_sys_macAddr_get(&macaddr));
        SYS_ERR_CHK(sys_util_mac2Str(macaddr.octet, macaddr_str));
        SYS_ERR_CHK(sal_stp_mstConfigName_set(macaddr_str));
    }
    SYS_ERR_CHK(sal_stp_mstConfigRevision_set(DFLT_STP_MST_CFG_REV));

    VLANMASK_SET_ALL(vlanmask);
    SYS_ERR_CHK(sal_stp_mstVlanmask_set(MSTP_INSTANCE_CIST, vlanmask));

    SYS_MEM_CLEAR(dfltInfo);
    dfltInfo.priority = DFLT_STP_MST_PRI;
    dfltInfo.portPathCost = DFLT_STP_MST_PORT_PATHCOST;
    dfltInfo.portPriority = DFLT_STP_MST_PORT_PRI;
    SYS_ERR_CHK(sal_stp_mstDflt_init(dfltInfo));
#endif

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PROTO_LLDP

int32 fds_lldp_default_init(void)
{
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    uint32 idx;
    lldp_med_networkPolicy_t policy;
  #endif
    sys_logic_port_t port;

    SYS_ERR_CHK(sal_lldp_enable_set(DFLT_LLDP_EBL));
    SYS_ERR_CHK(sal_lldp_tx_interval_set(DFLT_LLDP_TX_INTERVAL));
    SYS_ERR_CHK(sal_lldp_reinit_delay_set(DFLT_LLDP_REINIT_DELAY));
    SYS_ERR_CHK(sal_lldp_holdtime_set(DFLT_LLDP_HOLDTIME_MUL));
    SYS_ERR_CHK(sal_lldp_tx_delay_set(DFLT_LLDP_TX_DELAY));
    SYS_ERR_CHK(sal_lldp_disableAction_set(DFLT_LLDP_DISABLE_ACTION));
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_ERR_CHK(sal_lldp_medNetworkPolicyAuto_set(DFLT_LLDP_MED_NETWORK_POLICY_AUTO));
    SYS_ERR_CHK(sal_lldp_medFastRepeatCount_set(DFLT_LLDP_MED_FAST_RETRY));

    osal_memset(&policy, 0, sizeof(policy));

    for(idx = 0; idx < CAPA_LLDP_MED_NETWORK_POLICY_NUM; idx ++)
        SYS_ERR_CHK(sal_lldp_medNetworkPolicy_set(idx, policy));

  #endif

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_lldp_port_restore(port));
    }

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_SYSLOG

int32 fds_log_default_init(void)
{
    sal_log_target_t target;

    osal_memset(&target, 0, sizeof(target));
    if (ENABLED == DFLT_LOG_BUFFERED_EBL)
    {
        target.enable = DFLT_LOG_BUFFERED_EBL;
        target.severity = DFLT_LOG_BUFFERED_SEVERITY;
        target.totalCount = DFLT_LOG_BUFFERED_TOTAL;
    }
    SYS_ERR_CHK(sal_log_target_set(LOG_RAM, &target));

    osal_memset(&target, 0, sizeof(target));
    if (ENABLED == DFLT_LOG_CONSOLE_EBL)
    {
        target.enable = DFLT_LOG_CONSOLE_EBL;
        target.severity = DFLT_LOG_CONSOLE_SEVERITY;
        target.totalCount = 0;
    }
    SYS_ERR_CHK(sal_log_target_set(LOG_CONSOLE, &target));

    osal_memset(&target, 0, sizeof(target));
    if (ENABLED == DFLT_LOG_FILE_EBL)
    {
        target.enable = DFLT_LOG_FILE_EBL;
        target.severity = DFLT_LOG_FILE_SEVERITY; 
        target.totalCount = DFLT_LOG_FILE_TOTAL;
    }
    SYS_ERR_CHK(sal_log_target_set(LOG_FLASH, &target));

    SYS_ERR_CHK(sal_log_enable_set(DFLT_LOG_EBL));
    SYS_ERR_CHK(sal_log_ratelimitEnable_set(DFLT_LOG_AGGREGATION_EBL));
    SYS_ERR_CHK(sal_log_ratelimitInterval_set(DFLT_LOG_AGGREGATION_INTERVAL));
    SYS_ERR_CHK(sal_log_autoRestart_set(ENABLED));
    SYS_ERR_CHK(sal_log_config_create());

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_AAA

int32 fds_aaa_default_init(void)
{
    uint32 i;
    sys_auth_type_t authTypeAry[CAPA_AAA_AUTHTYPE_PRI_NUM] = {0};

    authTypeAry[0] = DFLT_AAA_LOGIN_AUTH_TYPE;   
    authTypeAry[1] = SYS_AUTH_TYPE_EMPTY;
    SYS_ERR_CHK(sal_aaa_authList_set(DFLT_AAA_LOGIN_AUTHLIST_NAME, authTypeAry, SYS_AUTHLIST_TYPE_AUTHEN_LOGIN));
    
    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        SYS_ERR_CHK(sal_aaa_loginAuthList_set(i, DFLT_AAA_LOGIN_AUTHLIST_NAME));
    }
    
    authTypeAry[0] = DFLT_AAA_ENABLE_AUTH_TYPE;   
    authTypeAry[1] = SYS_AUTH_TYPE_EMPTY;
    SYS_ERR_CHK(sal_aaa_authList_set(DFLT_AAA_ENABLE_AUTHLIST_NAME, authTypeAry, SYS_AUTHLIST_TYPE_AUTHEN_ENABLE));
    
    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        SYS_ERR_CHK(sal_aaa_enableAuthList_set(i, DFLT_AAA_ENABLE_AUTHLIST_NAME));
    }

  #ifdef CONFIG_SYS_LIB_TACPLUS
    {
        sys_tacplus_t tacSrv;

        SYS_MEM_CLEAR(tacSrv);

        tacSrv.timeout = DFLT_TACPLUS_TIMEOUT;

        SYS_ERR_CHK(sal_tacplus_dftParam_set(tacSrv));
    }
  #endif

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

int32 fds_authmgr_default_init(void)
{
    sys_logic_port_t lp;
#ifdef CONFIG_SYS_PROTO_MACAUTH
    authmgr_radiusUserFmt_t userFmt;
#endif

#ifdef CONFIG_SYS_PROTO_DOT1X
    SYS_ERR_CHK(sal_authmgr_typeEnbl_set(AUTHMGR_TYPE_DOT1X, DFLT_AUTHMGR_DOT1X_EBL));
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    SYS_ERR_CHK(sal_authmgr_typeEnbl_set(AUTHMGR_TYPE_MAC, DFLT_AUTHMGR_MAC_EBL));
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    SYS_ERR_CHK(sal_authmgr_typeEnbl_set(AUTHMGR_TYPE_WEB, DFLT_AUTHMGR_WEB_EBL));
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
    userFmt.delimiter = DFLT_AUTHMGR_RADIUS_USER_DELIMIT;
    userFmt.gap = DFLT_AUTHMGR_RADIUS_USER_GAP;
    userFmt.macCase = DFLT_AUTHMGR_RADIUS_USER_CASE;

    SYS_ERR_CHK(sal_authmgr_radiusUserFmt_set(userFmt));
#endif

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        SYS_ERR_CHK(fds_authmgr_port_restore(lp));
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_LIB_RADIUS

int32 fds_radius_default_init(void)
{
    sys_radius_t radSrv;

    SYS_MEM_CLEAR(radSrv);

    radSrv.retransmit = DFLT_RADIUS_RETRY;
    radSrv.timeout = DFLT_RADIUS_TIMEOUT;

    SYS_ERR_CHK(sal_radiusSrv_dftParam_set(radSrv));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_ISG

int32 fds_isg_default_init(void)
{
    sys_logic_port_t port;
    sys_isg_db_t db;
    sys_isg_db_statistics_t dbs;

    SYS_MEM_CLEAR(db);
    SYS_MEM_CLEAR(dbs);

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_isg_port_restore(port));
    }

    db.timeout = DFLT_IPSG_DB_TIMEOUT;
    db.writeDelay = DFLT_IPSG_DB_WRITE_DELAY;

    osal_sprintf(dbs.lastFaildReason, "No failure recorded.");

    SYS_ERR_CHK(sal_isg_db_set(&db));
    SYS_ERR_CHK(sal_isg_db_statistics_set(&dbs));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

int32 fds_dhcp_snooping_default_init(void)
{
    sys_logic_port_t port;

    SYS_ERR_CHK(sal_dhcp_snooping_enable_set(DFLT_DHCP_SNOOPING_EBL));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_dhcp_snooping_port_restore(port));
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_DAI

int32 fds_dai_default_init(void)
{
    sys_logic_port_t port;

    SYS_ERR_CHK(sal_dai_enable_set(DFLT_DAI_EBL));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_dai_port_restore(port));
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION

int32 fds_dos_default_init(void)
{
    sys_dos_type_t dosType = 0;
    sys_logic_port_t port;

    for (dosType = 0; dosType < DOS_TYPE_END; dosType++)
    {
        SYS_ERR_CONTINUE(sal_dos_typeEnable_set(dosType, DFLT_DOS_TYPE_EBL));
    }
    SYS_ERR_CHK(sal_dos_maxPingLen_set(DFLT_DOS_MAXPINGLEN));
    SYS_ERR_CHK(sal_dos_minIPv6FragLen_set(DFLT_DOS_MINIPV6FRAGLEN));
    SYS_ERR_CHK(sal_dos_smurfNetmaskLen_set(DFLT_DOS_SMURFNETMASKLEN));
    SYS_ERR_CHK(sal_dos_minTCPHdrLen_set(DFLT_DOS_MINTCPHDRLEN));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CONTINUE(fds_dos_port_restore(port));
    }

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PROTO_GVRP

int32 fds_gvrp_default_init(void)
{
    sys_logic_port_t port;

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_gvrp_port_enable_set(port, DFLT_GVRP_PORT_EBL));
        SYS_ERR_CHK(sal_gvrp_port_regmode_set(port, DFLT_GVRP_PORT_REG));
        SYS_ERR_CHK(sal_gvrp_port_forbidVlan_set(port, DFLT_GVRP_PORT_FORBID));
    }

    SYS_ERR_CHK(sal_gvrp_timer_set(GVRP_TIMER_HOLD, DFLT_GVRP_HOLD_TIME));
    SYS_ERR_CHK(sal_gvrp_timer_set(GVRP_TIMER_JOIN, DFLT_GVRP_JOIN_TIME));
    SYS_ERR_CHK(sal_gvrp_timer_set(GVRP_TIMER_LEAVE, DFLT_GVRP_LEAVE_TIME));
    SYS_ERR_CHK(sal_gvrp_timer_set(GVRP_TIMER_LEAVEALL, DFLT_GVRP_LEAVEALL_TIME));

    SYS_ERR_CHK(sal_gvrp_enable_set(DFLT_GVRP_EBL));

    return SYS_ERR_OK;
}

#endif 

#ifdef CONFIG_SYS_PROTO_GARP

int32 fds_garp_default_init(void)
{
    sys_logic_port_t port;

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_gmrp_port_enable_set(port, DFLT_GMRP_PORT_EBL));
        
        
    }

    SYS_ERR_CHK(sal_gmrp_enable_set(DFLT_GMRP_EBL));

    return SYS_ERR_OK;
}

#endif 

#ifdef CONFIG_SYS_MGMT_ACL

int32 fds_acl_mgmt_dfltEntry_init(void)
{
    sys_acl_mgmt_ace_priFlag_t stAcepriFlag = PRIFLAG_INVALID;
    sys_acl_mgmt_ace_t  stAce;
    sys_err_code_t ret = SYS_ERR_OK;
    char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1] = "";

    osal_strncpy(acListName, DFLT_ACL_MGMT_PROFILE, SYS_MGMT_ACL_NAME_LENTH_MAX);
    acListName[SYS_MGMT_ACL_NAME_LENTH_MAX] = '\0';

    osal_memset(&stAce, 0 ,sizeof(sys_acl_mgmt_ace_t));
    osal_memset(&(stAce.stCare.stCareFlag), 0 ,sizeof(sys_acl_mgmt_careflag_t));
    LOGIC_PORTMASK_SET_ALL(stAce.stCare.logicPortmask);

    if (0 == osal_strcmp(acListName, "console-only"))
    {
        stAce.stAction = MGMTACTION_DENY;
        stAce.stCare.uiService = SERVICE_ALL;
    }
    else if (0 == osal_strcmp(acListName, "default"))
    {
        stAce.stAction = MGMTACTION_PERMIT;
        stAce.stCare.uiService = SERVICE_ALL;
    }
    else
    {
         return SYS_ERR_OK;
    }

    ret = sal_acl_mgmt_list_add(acListName);
    if ((SYS_ERR_OK != ret ) && (SYS_ERR_MGMT_ACL_EXIST !=ret ))
    {
         return ret;
    }

    SYS_ERR_CHK(sal_acl_mgmt_ace_add(acListName, stAcepriFlag, &stAce));

    return SYS_ERR_OK;

}
#endif

#ifdef CONFIG_SYS_PROTO_UDLD

int32 fds_udld_default_init(void)
{
    sys_logic_port_t port;

    SYS_ERR_CHK(sal_udld_messageTime_set(DFLT_UDLD_INTERVAL_TIME));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_udld_port_restore(port));
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_ERPS

int32 fds_erps_default_init(void)
{
    SYS_ERR_CHK(sal_erps_enable_set(DFLT_ERPS_EBL));

    return SYS_ERR_OK;
}

#endif 

#ifdef CONFIG_SYS_PROTO_LOOPBACK

int32 fds_loopback_default_init(void)
{
    SYS_ERR_CHK(sal_loopback_proto_enable_set(DFLT_LOOPBACK_EBL));

    return SYS_ERR_OK;
}

#endif 

int32 fds_sys_post_init(void)
{
#ifdef CONFIG_SYS_UI_SNMP
    sys_enable_t enable;

    enable = DISABLED;
#endif

#ifdef CONFIG_SYS_UI_SNMP
    
    SYS_ERR_CHK(sal_snmp_enable_get(&enable));
    if (ENABLED == enable)
    {
        SYS_ERR_CHK(sal_snmp_confFile_update());
        SYS_ERR_CHK(sal_snmp_start());

  #ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
        system("handler -s rmonapply 1>> /dev/null 2>> /dev/null &");
  #endif
    }

    SYS_ERR_CHK(sal_snmp_sysUpTime_set());
#endif

#ifdef CONFIG_SYS_SYSLOG
    
    SYS_ERR_CHK(sal_log_daemon_restart(TRUE));
#endif

    return SYS_ERR_OK;
}

int32 fds_port_post_init(void)
{
    sys_logic_port_t port;
    sys_enable_t enable;

    FOR_EACH_LOGIC_PORT(port)
    {
        
        SYS_ERR_CONTINUE(sal_port_enable_get(port, &enable));
        SYS_ERR_CONTINUE(sal_port_enable_set(port, enable));
    }

    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_POE
uint32 fds_poe_Support_check(void)
{
    if (POE_IS_SYSTEM_READY())
    	return TRUE;

    return FALSE;
}

int32 fds_poe_default_init(void)
{
    sys_logic_port_t port;

    FOR_EACH_NORMAL_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_poe_port_restore(port));
    }
    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_DHCP_SERVER

int32 fds_dhcp_server_default_init(void)
{
    
	
	SYS_ERR_CHK(sal_dhcp_server_db_init());
    
    
    
#if 0
    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_dhcp_snooping_port_restore(port));
    }
#endif
    return SYS_ERR_OK;
}
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP

int32 fds_easyCwmp_default_init(void)
{
	

    return SYS_ERR_OK;
}
#endif

int32 fds_oem_password_config(char *password)
{
	sys_enblPasswd_t enblPasswd;

	osal_memset(&enblPasswd, 0, sizeof(enblPasswd));
	osal_strcpy(enblPasswd.password, password);
	enblPasswd.passwdType = SYS_PASSWD_TYPE_ENCRYPT;
#ifdef CONFIG_SYS_AAA
	SYS_ERR_CHK(sal_sys_enablePassword_set(SYS_LOCALUSER_PRIV_ADMIN, enblPasswd));
#else
	
	SYS_ERR_CHK(sal_sys_enablePassword_set(0, enblPasswd));
#endif

	return SYS_ERR_OK;
}
int32 fds_oem_local_user_password(char* username, char *password)
{
	sys_userInfo_t userInfo;

    osal_memset(&userInfo, 0, sizeof(userInfo));

    if(osal_strlen(username) != 0)
    {
    	osal_sprintf(userInfo.username, "%s", username);
    }
    else 
    {
    	osal_strcpy(userInfo.username, DFLT_SYS_ADMIN_USERNAME);
    }

    if(osal_strlen(password) != 0)
    {
		osal_sprintf(userInfo.password, "%s", password);
    }
    else
    {	
		osal_strcpy(userInfo.password, DFLT_SYS_ADMIN_PASSWORD);
	}

    userInfo.priv = DFLT_SYS_ADMIN_PRIV;
    userInfo.passwdType = SYS_PASSWD_TYPE_ENCRYPT;
    userInfo.accessMethod = DFLT_SYS_USER_ACCESS_METH;
    SYS_ERR_CHK(sal_sys_localUserForce_add(userInfo));
    return SYS_ERR_OK;
}

int32 fds_oem_mgmtVlan_init(char *ipaddr, char *netmask, char* gateway)
{
    sys_l3_intfMgmtAdmin_t mgmt;
	sys_ipv4_t mask = 0;

    SYS_MEM_CLEAR(mgmt);
    
    mgmt.v4.dhcpType = DFLT_L3_IPV4_DHCP_TYPE;
    if(osal_strlen(ipaddr) == 0)
    {
		STR2IPV4(DFLT_L3_IPV4_ADDR, &mgmt.v4.ip.addr);
    }
    else
    {
		STR2IPV4(ipaddr, &mgmt.v4.ip.addr);
	}
	if(osal_strlen(netmask) == 0)
    {
		STR2IPV4(DFLT_L3_IPV4_NETMASK, &mask);	
    }
    else
    {
		STR2IPV4(netmask, &mask);
	}
	mgmt.v4.ip.plen = IPV4_MASK2PFX(mask);
    
    if(osal_strlen(gateway) == 0)
    {
		STR2IPV4(DFLT_L3_IPV4_GW, &mgmt.v4.gw);
    }
    else
    {
		STR2IPV4(gateway, &mgmt.v4.gw);
	}

    mgmt.v6.enbl = DFLT_L3_IPV6_ADMIN_EBL;
    mgmt.v6.autoconfEnbl = DFLT_L3_IPV6_AUTOCONF_EBL;
    mgmt.v6.dhcpEnbl = DFLT_L3_IPV6_DHCPC_EBL;

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&mgmt));

    return SYS_ERR_OK;
}

int32 fds_sys_define_vlan_intf_init(char *ipaddr, char *netmask)
{
    sys_l3_intfdef_t mgmt;
	sys_ipv4_t mask = 0;

    SYS_MEM_CLEAR(mgmt);

    mgmt.defId.type = SYS_L3_INTFTYPE_VLAN;
    mgmt.defId.id = DFLT_VLAN_DFLT_VLAN;
    
    mgmt.v4.dhcpType = DFLT_L3_IPV4_DHCP_TYPE;
    if(osal_strlen(ipaddr) == 0)
    {
		STR2IPV4(DFLT_L3_IPV4_ADDR, &mgmt.v4.ip.addr);
    }
    else
    {
		STR2IPV4(ipaddr, &mgmt.v4.ip.addr);
	}
	
	if(osal_strlen(netmask) == 0)
    {
		STR2IPV4(DFLT_L3_IPV4_NETMASK, &mask);	
    }
    else
    {
		STR2IPV4(netmask, &mask);
	}
	mgmt.v4.ip.plen = IPV4_MASK2PFX(mask);

    mgmt.v6.enbl = DFLT_L3_IPV6_ADMIN_EBL;
    mgmt.v6.autoconfEnbl = DFLT_L3_IPV6_AUTOCONF_EBL;
    mgmt.v6.dhcpEnbl = DFLT_L3_IPV6_DHCPC_EBL;

    SYS_ERR_CHK(sal_l3_define_interface_set(&mgmt));

    return SYS_ERR_OK;
}

int32 fds_oem_sysInfo_init(char *sysname, char *location, char* contact)
{
    if(osal_strlen(sysname) == 0)
    {
        SYS_ERR_CHK(sal_sys_systemName_set(DFLT_SYS_NAME));
    }
    else
    {
        SYS_ERR_CHK(sal_sys_systemName_set(sysname));
    }
    if(osal_strlen(location) == 0)
    {
        SYS_ERR_CHK(sal_sys_systemLocation_set(DFLT_SYS_LOCATION));
    }
    else
    {
        SYS_ERR_CHK(sal_sys_systemLocation_set(location));
    }
    
    if(osal_strlen(contact) == 0)
    {
        SYS_ERR_CHK(sal_sys_systemContact_set(DFLT_SYS_CONTACT));
    }
    else
    {
        SYS_ERR_CHK(sal_sys_systemContact_set(contact));
    }

    return SYS_ERR_OK;
}

int32 fds_oem_logo_init(char *loginLogo, char *homeLogo)
{
    if(osal_strlen(loginLogo) == 0)
    {
        osal_sprintf(loginLogo, "%s", DFLT_SYS_LOGIN_LOGO);
    }
    
    if(osal_strlen(homeLogo) == 0)
    {
        osal_sprintf(homeLogo, "%s", DFLT_SYS_HOME_LOGO);
    }

	SYS_ERR_CHK(sal_sys_system_logo_set(loginLogo, homeLogo));

    return SYS_ERR_OK;
}

int32 fds_oem_timezone_init(char *timezone)
{
    sys_time_t systime;

    osal_memset(&systime, 0 , sizeof(sys_time_t));

#ifdef CONFIG_SYS_APP_MSNTP
    systime.sntpEnable  = DFLT_SYS_SNTP_EBL;
    systime.sntpPort = DFLT_SYS_SNTP_PORT;
#endif

    if(osal_strlen(timezone) == 0)
    {
        systime.tzdiff = DFLT_SYS_TIME_ZONE;
    }
    else
    {
        systime.tzdiff = atoi(timezone);
    }
    sprintf(systime.tzAcronym, "%s", DFLT_SYS_TIME_ZONE_ACRONYM);

    SYS_ERR_CHK(sal_time_set(&systime));
    
    return SYS_ERR_OK;
}

int32 fds_oem_title_init(char *pTitle)
{
    if(osal_strlen(pTitle) == 0)
    {
        sal_sys_bdinfoVar_get("modelname", pTitle);
    }

    SYS_ERR_CHK(sal_sys_title_set(pTitle));

    return SYS_ERR_OK;
}

int32 fds_oem_config_init(void)
{
    FILE *confp = NULL;
    char *pTemp = NULL;
    char buf[SYS_BUF256_LEN];
    #if 0
    char username[CAPA_SYS_USERNAME_LEN + 1];
    char password[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    char ipaddr[CAPA_SYS_IPADDR_LEN + 1];
    char ipmask[CAPA_SYS_IPADDR_LEN + 1];
    char gwipaddr[CAPA_SYS_IPADDR_LEN + 1];
    char location[CAPA_SYS_LOCATION_LEN + 1];
    char contact[CAPA_SYS_CONTACT_LEN + 1];
    char sysname[CAPA_SYS_NAME_LEN + 1];
    char timezone[SYS_BUF16_LEN];
    #endif
    char homelogo[CAPA_SYS_NAME_LEN + 1];
    char loginlogo[CAPA_SYS_NAME_LEN + 1];
	char title[CAPA_SYS_NAME_LEN + 1];

    confp = fopen (SYS_OEM_CONF_FILE, "r");
    if (confp == NULL)
    {
        return SYS_ERR_FAILED;
    }
    
    SYS_MEM_CLEAR(buf);
    #if 0
    SYS_MEM_CLEAR(username);
    SYS_MEM_CLEAR(password);
    SYS_MEM_CLEAR(ipaddr);
    SYS_MEM_CLEAR(gwipaddr);
    SYS_MEM_CLEAR(ipmask);
    SYS_MEM_CLEAR(location);
    SYS_MEM_CLEAR(contact);
    SYS_MEM_CLEAR(sysname);
    #endif
    SYS_MEM_CLEAR(homelogo);
    SYS_MEM_CLEAR(loginlogo);
    SYS_MEM_CLEAR(title);

    while(fgets(buf, sizeof(buf), confp))
    {
        pTemp = osal_strchr(buf, '\r');
        *pTemp = '\0';
        #if 0
        if(osal_strstr(buf, "username:"))
        {
            osal_strcpy(username, buf + osal_strlen("username:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "password:"))
        {
            osal_sprintf(password, "%s", buf + osal_strlen("password:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "ipaddr:"))
        {
            osal_sprintf(ipaddr, "%s", buf + osal_strlen("ipaddr:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "ipmask:"))
        {
            osal_sprintf(ipmask, "%s", buf + osal_strlen("ipmask:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "gateway:"))
        {
            osal_sprintf(gwipaddr, "%s", buf + osal_strlen("gateway:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "sysname:"))
        {
            osal_sprintf(sysname, "%s", buf + osal_strlen("sysname:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "location:"))
        {
            osal_sprintf(location, "%s", buf + osal_strlen("location:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "contact:"))
        {
            osal_sprintf(contact, "%s", buf + osal_strlen("contact:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "timezone:"))
        {
            osal_sprintf(timezone, "%s", buf + osal_strlen("timezone:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        #endif
        if(osal_strstr(buf, "homelogoname:"))
        {
            osal_sprintf(homelogo, "%s", buf + osal_strlen("homelogoname:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "loginlogoname:"))
        {
            osal_sprintf(loginlogo, "%s", buf + osal_strlen("loginlogoname:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        if(osal_strstr(buf, "title:"))
        {
            osal_sprintf(title, "%s", buf + osal_strlen("title:"));
            SYS_MEM_CLEAR(buf);
            continue;
        }
        SYS_MEM_CLEAR(buf);
    }

    fclose (confp);
    #if 0
    fds_oem_local_user_password(username, password);
    #ifdef CONFIG_SYS_L3_ARCHITECTURE
    fds_sys_define_vlan_intf_init(ipaddr, ipmask);
    #else
	fds_oem_mgmtVlan_init(ipaddr, ipmask, gwipaddr);
    #endif
    
    fds_oem_timezone_init(timezone);
    #endif

	fds_oem_title_init(title);
	SYS_ERR_CHK(fds_oem_local_user_password("", ""));
	SYS_ERR_CHK(sal_sys_system_logo_set(DFLT_SYS_LOGIN_LOGO, DFLT_SYS_HOME_LOGO));
	SYS_ERR_CHK(sal_sys_systemName_set(DFLT_SYS_NAME));
	SYS_ERR_CHK(sal_sys_systemLocation_set(DFLT_SYS_LOCATION));
	SYS_ERR_CHK(sal_sys_systemContact_set(DFLT_SYS_CONTACT));

    return SYS_ERR_OK;
}

