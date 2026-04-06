
#include <include/cgi.h>

#include <libsal/sal_sys.h>
#include <libfds/fds_export.h>

static int32 cgi_home_login_get(void);
static int32 cgi_home_loginStatus_get(void);
static int32 cgi_home_main_get(void);
static int32 cgi_home_monitor_get(void);
static int32 cgi_home_empty_get(void);
static int32 cgi_home_debug_get(void);
static int32 cgi_home_systemSet_get(void);
static int32 cgi_home_sysOemset_get(void);

const cgi_cmdmap_t cmdGetHomeTbl[] =
{
    {CGI_CMD_HOME_LOGIN,            cgi_home_login_get},
    {CGI_CMD_HOME_LOGINSTATUS,      cgi_home_loginStatus_get},
    {CGI_CMD_HOME_MAIN,             cgi_home_main_get},
    {CGI_CMD_HOME_MONITOR,          cgi_home_monitor_get},
    {CGI_CMD_HOME_EMPTY,            cgi_home_empty_get},
    {CGI_CMD_HOME_DEBUG,            cgi_home_debug_get},
    {CGI_CMD_HOME_SYSTEMSET,        cgi_home_systemSet_get},
    {CGI_CMD_HOME_SYSOEMSET,        cgi_home_sysOemset_get},
    {NULL, NULL}
};

static int32 cgi_home_login_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_board_conf_t boardConf;
    sys_enable_t httpsEnbl = DISABLED;

    char modulus[SYS_BUF1024_LEN];
    char sysname[CAPA_SYS_NAME_LEN + 1];
    char macStr[CAPA_MAC_STR_LEN];
    sys_mac_t sysMacAddr;
    char *accessTypeEnv;
    char sysLogo[CAPA_SYS_NAME_LEN + 1];

    SYS_MEM_CLEAR(boardConf);
    SYS_MEM_CLEAR(sysname);
    SYS_MEM_CLEAR(macStr);
    SYS_MEM_CLEAR(sysMacAddr);
    SYS_MEM_CLEAR(sysLogo);

    accessTypeEnv = getenv("ACCESS_TYPE");

    sal_sys_boardConf_get(&boardConf);
#ifdef CONFIG_USER_BOA_WITH_SSL
    sal_sys_httpsEnable_get(&httpsEnbl);
#endif
    sal_sys_systemName_get(sysname);
    sal_sys_macAddr_get(&sysMacAddr);
    sal_sys_system_login_logo_get(sysLogo);

    sys_util_mac2Str(sysMacAddr.octet, macStr);

    SYS_MEM_CLEAR(modulus);

    if (accessTypeEnv == NULL)
    {
        if(SYS_ERR_OK != sal_util_rsaModulus_get(modulus))
            SYS_MEM_CLEAR(modulus);
    }

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddStringToObject(data, CGI_HOME_TITLE, sysname);
    cJSON_AddBoolToObject(data, CGI_SYS_HTTPS_ENBL, httpsEnbl);
    cJSON_AddBoolToObject(data, "https", (accessTypeEnv != NULL && 0 == osal_strcmp(accessTypeEnv, "https")));
    cJSON_AddStringToObject(data, CGI_SYS_MODULUS, modulus);
    cJSON_AddStringToObject(data, CGI_LOGIN_LOGO, sysLogo);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_home_loginStatus_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cgi_session_authingStatus_t status;

    CGI_GET_JSON_CREATE(root, data);

    cgi_util_authingSessionStatus_get(&status);

    if (CGI_SESSION_AUTHING_SUCCESS == status)
    {
        cJSON_AddStringToObject(data, "status", "ok");
        cgi_util_authingSession_destroy();
    }
    else if (CGI_SESSION_AUTHING_FAIL == status)
    {
        cJSON_AddStringToObject(data, "status", "fail");
        cJSON_AddStringToObject(data, "failReason", CGI_LANG("login", "errLoginUserInvalid"));
        cgi_util_authingSession_destroy();
    }
    else if (CGI_SESSION_AUTHING_SESSION_EXCEED == status)
    {
        cJSON_AddStringToObject(data, "status", "fail");
        cJSON_AddStringToObject(data, "failReason", CGI_LANG("login", "errLoginSessionNum"));
        cgi_util_authingSession_destroy();
    }
    else
    {
        cJSON_AddStringToObject(data, "status", "authing");
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static void _menu_folder_create(cJSON **parent, cJSON **folder, cJSON **leaf, char *name)
{
    *folder = cJSON_CreateObject();
    cJSON_AddStringToObject(*folder, CGI_HOME_MENU_NAME, name);
    cJSON_AddItemToArray(*parent, *folder);

    *leaf = cJSON_CreateArray();
    cJSON_AddItemToObject(*folder, CGI_HOME_MENU_LEAF, *leaf);
}

static void _menu_doc_create(cJSON **parent, cJSON **doc, char *name, char *url)
{
    *doc = cJSON_CreateObject();
    cJSON_AddStringToObject(*doc, CGI_HOME_MENU_NAME, name);
    cJSON_AddStringToObject(*doc, CGI_HOME_MENU_URL, url);
    cJSON_AddItemToArray(*parent, *doc);
}

static void _cgi_home_menu_get(cJSON *data)
{
#define CREATE_FOLDER1(name) do{_menu_folder_create(&menu,&folder1,&leaf1,name);}while(0)
#define CREATE_FOLDER2(name) do{_menu_folder_create(&leaf1,&folder2,&leaf2,name);}while(0)
#define CREATE_DOC1(name,url) do{_menu_doc_create(&leaf1,&doc,name,url);}while(0)
#define CREATE_DOC2(name,url) do{_menu_doc_create(&leaf2,&doc,name,url);}while(0)
#define LANG_MENU(key) CGI_LANG("menu",key)

#ifdef CONFIG_SYS_UI_WEB_THREE_LEVEL_TREE
#define CREATE_FOLDER3(name) do{_menu_folder_create(&leaf2,&folder3,&leaf3,name);}while(0)
#define CREATE_DOC3(name,url) do{_menu_doc_create(&leaf3,&doc,name,url);}while(0)
    cJSON *leaf3 = NULL;
    cJSON *folder3 = NULL;
#endif

    cJSON *menu = NULL;
    cJSON *folder1 = NULL;
    cJSON *folder2 = NULL;
    cJSON *leaf1 = NULL;
    cJSON *leaf2 = NULL;
    cJSON *doc = NULL;

    menu = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_HOME_MENU, menu);

    CREATE_FOLDER1(LANG_MENU("fld1Status"));
        CREATE_DOC1(LANG_MENU("doc1StatusInfo"), "html/status_sysinfo.html");
      #ifdef CONFIG_SYS_SYSLOG
        CREATE_DOC1(LANG_MENU("doc1StatusSyslog"), "html/status_syslog.html");
      #endif
        CREATE_FOLDER2(LANG_MENU("fld2StatusPort"));
            CREATE_DOC2(LANG_MENU("doc2StatusPortCnt"), "html/status_port_cnt.html");
          #ifdef CONFIG_SYS_PORT_ERR_DISABLED
            CREATE_DOC2(LANG_MENU("doc2StatusPortErrDisable"), "html/status_port_errDisable.html");
          #endif
          #ifdef CONFIG_SYS_BW_UTILIZATION
            CREATE_DOC2(LANG_MENU("doc2StatusPortBwUtiliz"), "html/status_port_bwutilz.html");
          #endif
        CREATE_DOC1(LANG_MENU("doc1StatusLAG"), "html/status_lag.html");
        CREATE_DOC1(LANG_MENU("doc1StatusMac"), "html/status_mac.html");

    if (g_priv == 1)
        return;

    CREATE_FOLDER1(LANG_MENU("fld1Network"));
      #ifndef CONFIG_SYS_L3_ARCHITECTURE
        CREATE_DOC1(LANG_MENU("doc1NetworkIPAddr"), "html/network_ip.html");
      #endif
      #ifdef CONFIG_SYS_ADVANCE_DNS
        CREATE_DOC1(LANG_MENU("doc1NetworkDNS"), "html/network_dns.html");
        CREATE_DOC1(LANG_MENU("doc1NetworkDNSHosts"), "html/network_dnsHosts.html");
      #endif

        CREATE_DOC1(LANG_MENU("doc1NetworkTimeSys"), "html/network_time_sys.html");
    
    CREATE_FOLDER1(LANG_MENU("fld1Port"));
        CREATE_DOC1(LANG_MENU("doc1PortSetting"), "html/port_port.html");
      #ifdef CONFIG_SYS_PORT_ERR_DISABLED
        CREATE_DOC1(LANG_MENU("doc1PortErrDisable"), "html/port_errDisable.html");
      #endif
        CREATE_FOLDER2(LANG_MENU("fld2PortLAG"));
            CREATE_DOC2(LANG_MENU("doc2PortLAGMgmt"), "html/port_lag_mgmt.html");
            CREATE_DOC2(LANG_MENU("doc2PortLAGPort"), "html/port_lag_port.html");
          #ifdef CONFIG_SYS_PROTO_LACP
            CREATE_DOC2(LANG_MENU("doc2PortLACP"), "html/port_lag_lacp.html");
          #endif
      #ifdef CONFIG_SYS_EEE
        CREATE_DOC1(LANG_MENU("doc1PortEEE"), "html/port_eee.html");
      #endif
      #ifdef CONFIG_SYS_JUMBO_FRAME
        CREATE_DOC1(LANG_MENU("doc1PortJumbo"), "html/port_jumbo.html");
      #endif
#ifdef CONFIG_SYS_PORT_SECURITY
        CREATE_DOC1(LANG_MENU("doc1SecPort"), "html/sec_port.html");
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
        CREATE_DOC1(LANG_MENU("doc1SecProtected"), "html/sec_protect.html");
#endif
#ifdef CONFIG_SYS_STORM_CTRL
        CREATE_DOC1(LANG_MENU("doc1SecStorm"), "html/sec_stromControl.html");
#endif

    #ifdef CONFIG_SYS_POE
    if (POE_IS_SYSTEM_READY())
    {
        CREATE_FOLDER1(LANG_MENU("fld1Poe"));
            CREATE_DOC1(LANG_MENU("doc1PoePort"), "html/poe_port.html");
            CREATE_DOC1(LANG_MENU("doc1PoePortTimer"), "html/poe_port_timer.html");
    }
    #endif

    CREATE_FOLDER1(LANG_MENU("fld1Vlan"));
        CREATE_FOLDER2(LANG_MENU("fld2Vlan"));
            CREATE_DOC2(LANG_MENU("doc1VlanCreate"), "html/vlan_vlan_create.html");
            CREATE_DOC2(LANG_MENU("doc1VlanVLANConf"), "html/vlan_vlan_configuration.html");
            CREATE_DOC2(LANG_MENU("doc1VlanMembership"), "html/vlan_vlan_membership.html");
            CREATE_DOC2(LANG_MENU("doc1VlanPort"), "html/vlan_vlan_port.html");
      #ifdef CONFIG_SYS_VOICEVLAN
        CREATE_FOLDER2(LANG_MENU("fld2VlanVoice"));
            CREATE_DOC2(LANG_MENU("doc2VlanVoiceProp"), "html/vlan_voice_prop.html");
            CREATE_DOC2(LANG_MENU("doc2VlanVoiceOUI"), "html/vlan_voice_oui.html");
      #endif
      #ifdef CONFIG_SYS_PROTOCOL_VLAN
        CREATE_FOLDER2(LANG_MENU("fld2VlanProtocol"));
            CREATE_DOC2(LANG_MENU("doc1VlanProtoGroup"), "html/vlan_protocol_group.html");
            CREATE_DOC2(LANG_MENU("doc1VlanProtoPort"), "html/vlan_protocol_binding.html");
      #endif
      #ifdef CONFIG_SYS_MACVLAN
        CREATE_FOLDER2(LANG_MENU("fld2VlanMac"));
            CREATE_DOC2(LANG_MENU("doc1VlanMacGroup"), "html/vlan_mac_group.html");
            CREATE_DOC2(LANG_MENU("doc1VlanMacGroupMapping"), "html/vlan_mac_binding.html");
      #endif
      #ifdef CONFIG_SYS_SURVEILLANCEVLAN
        CREATE_FOLDER2(LANG_MENU("fld2VlanSurveillance"));
            CREATE_DOC2(LANG_MENU("doc2VlanSurveillanceProp"), "html/vlan_surveillance_prop.html");
            CREATE_DOC2(LANG_MENU("doc2VlanSurveillanceOUI"), "html/vlan_surveillance_oui.html");
      #endif
      #ifdef CONFIG_SYS_IP_SUBNET
        CREATE_FOLDER2(LANG_MENU("fld2VlanIPsubnet"));
            CREATE_DOC2(LANG_MENU("doc1VlanIPsubnetGroup"), "html/vlan_ipsubnet_group.html");
            CREATE_DOC2(LANG_MENU("doc1VlanIPsubnetPort"), "html/vlan_ipsubnet_binding.html");
      #endif
      #ifdef CONFIG_SYS_PROTO_GVRP
        CREATE_FOLDER2(LANG_MENU("fld2VlanGVRP"));
            CREATE_DOC2(LANG_MENU("doc2VlanGVRPProperty"), "html/vlan_gvrp_property.html");
            CREATE_DOC2(LANG_MENU("doc2VlanGVRPMembership"), "html/vlan_gvrp_members.html");
            CREATE_DOC2(LANG_MENU("doc2VlanGVRPStatistics"), "html/vlan_gvrp_statistics.html");
      #endif

    CREATE_FOLDER1(LANG_MENU("fld1Mac"));
        CREATE_DOC1(LANG_MENU("doc1MacDynamic"), "html/mac_dynamic.html");
        CREATE_DOC1(LANG_MENU("doc1MacStatic"),  "html/mac_static.html");
      #ifdef CONFIG_SYS_BLACK_HOLE_MAC
        CREATE_DOC1(LANG_MENU("doc1MacFilter"), "html/mac_filter.html");
      #endif
      #ifdef CONFIG_SYS_PORT_SECURITY
        CREATE_DOC1(LANG_MENU("doc1MacPsecure"), "html/mac_psecure.html");
      #endif

  #ifdef CONFIG_SYS_PROTO_STP
    
    CREATE_FOLDER1(LANG_MENU("fld1STP"));
        CREATE_DOC1(LANG_MENU("doc1StpGlobal"), "html/stp_global.html");
        CREATE_DOC1(LANG_MENU("doc1StpIntf"), "html/stp_intf.html");
#ifdef CONFIG_SYS_PROTO_STP_MSTP
        CREATE_DOC1(LANG_MENU("doc1StpMstInstance"), "html/stp_msti.html");
        CREATE_DOC1(LANG_MENU("doc1StpMstIntf"), "html/stp_mstiIntf.html");
#endif
        CREATE_DOC1(LANG_MENU("doc1StpStatistics"), "html/stp_statistics.html");
  #endif
#ifdef CONFIG_SYS_PROTO_ERPS
    CREATE_FOLDER1(LANG_MENU("ringPort"));
        CREATE_DOC1(LANG_MENU("ringStatusErps"), "html/erps_status_setting.html");
        CREATE_DOC1(LANG_MENU("ringPortErps"), "html/erps_setting.html");
#endif

    CREATE_FOLDER1(LANG_MENU("fld1Discov"));
      #ifdef CONFIG_SYS_PROTO_LLDP
        
        CREATE_FOLDER2(LANG_MENU("fld2DiscovLLDP"));
            CREATE_DOC2(LANG_MENU("doc2DiscovLLDPProp"), "html/discovery_lldp_prop.html");
            CREATE_DOC2(LANG_MENU("doc2DiscovLLDPPort"), "html/discovery_lldp_port.html");
          #ifdef CONFIG_SYS_PROTO_LLDP_MED
            CREATE_DOC2(LANG_MENU("doc2DiscovLLDPMedPolicy"), "html/discovery_lldp_medPolicy.html");
            CREATE_DOC2(LANG_MENU("doc2DiscovLLDPMedPort"), "html/discovery_lldp_medPort.html");
          #endif
            CREATE_DOC2(LANG_MENU("doc2DiscovLLDPOverloading"), "html/discovery_lldp_overloading.html");
            CREATE_DOC2(LANG_MENU("doc2DiscovLLDPLocalInfo"), "html/discovery_lldp_localInfo.html");
            CREATE_DOC2(LANG_MENU("doc2DiscovLLDPNeighbor"), "html/discovery_lldp_neighborInfo.html");
            CREATE_DOC2(LANG_MENU("doc2DiscovLLDPStatistics"), "html/discovery_lldp_statistic.html");
      #endif

#ifdef CONFIG_SYS_DHCP_SERVER
      
      CREATE_FOLDER1(LANG_MENU("fld1Dhcp"));
          CREATE_DOC1(LANG_MENU("doc1Dhcp"), "html/dhcpserver_prop.html");
          CREATE_DOC1(LANG_MENU("doc1IPPool"), "html/ip_pool.html");
          CREATE_DOC1(LANG_MENU("doc1vlanifpool"), "html/dhcpserver_vlanif_prop.html");
          CREATE_DOC1(LANG_MENU("doc1DhcpClients"), "html/dhcpserver_client_list.html");
          CREATE_DOC1(LANG_MENU("doc1DhcpStatic"), "html/dhcpserver_client_static.html");
#endif

  #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    
    CREATE_FOLDER1(LANG_MENU("fld1Mcast"));
        CREATE_FOLDER2(LANG_MENU("fld2McastGeneral"));
            CREATE_DOC2(LANG_MENU("doc2McastGeneralProperty"), "html/mcast_general_prop.html");
            CREATE_DOC2(LANG_MENU("doc2McastGeneralGroupAddr"), "html/mcast_general_group.html");
            CREATE_DOC2(LANG_MENU("doc2McastGeneralRouterPort"), "html/mcast_general_router.html");
            #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
            CREATE_DOC2(LANG_MENU("doc2McastGeneralForwardAll"), "html/mcast_general_fwdAll.html");
            #endif
            #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
            CREATE_DOC2(LANG_MENU("doc2McastGeneralThrottling"), "html/mcast_general_throt.html");
            #endif
            #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
            CREATE_DOC2(LANG_MENU("doc2McastIGMPGeneralFilterProfile"), "html/mcast_general_profiles.html");
            CREATE_DOC2(LANG_MENU("doc2McastIGMPGeneralFilterBinding"), "html/mcast_general_filter.html");
            #endif

        CREATE_FOLDER2(LANG_MENU("fld2McastIGMP"));
            CREATE_DOC2(LANG_MENU("doc2McastIGMPProperty"), "html/mcast_igmp_prop.html");
          #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
            CREATE_DOC2(LANG_MENU("doc2McastIGMPQuerier"), "html/mcast_igmp_querier.html");
          #endif
            CREATE_DOC2(LANG_MENU("doc2McastIGMPStatistics"), "html/mcast_igmp_statistics.html");

      #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        CREATE_FOLDER2(LANG_MENU("fld2McastMLD"));
            CREATE_DOC2(LANG_MENU("doc2McastMLDProperty"), "html/mcast_mld_prop.html");
            CREATE_DOC2(LANG_MENU("doc2McastMLDStatistics"), "html/mcast_mld_statistics.html");
      #endif 

      #ifdef CONFIG_SYS_PROTO_MVR
        CREATE_FOLDER2(LANG_MENU("fld2McastMVR"));
            CREATE_DOC2(LANG_MENU("doc2McastMVRProperty"), "html/mcast_mvr_prop.html");
            CREATE_DOC2(LANG_MENU("doc2McastMVRPortSetting"), "html/mcast_mvr_port.html");
            CREATE_DOC2(LANG_MENU("doc2McastMVRGroupAddress"), "html/mcast_mvr_group.html");
      #endif 
#ifdef CONFIG_SYS_PROTO_GARP
        CREATE_FOLDER2(LANG_MENU("fld2GMRP"));
            CREATE_DOC2(LANG_MENU("doc2GMRPProperty"), "html/gmrp_property.html");
#endif

  #endif 

#ifdef CONFIG_SYS_L3_ROUTE
    
    CREATE_FOLDER1(LANG_MENU("fld1IPConfig"));
        CREATE_FOLDER2(LANG_MENU("fld2IPv4"));
            CREATE_DOC2(LANG_MENU("doc2IPv4Intf"), "html/ipconf_ipv4_intf.html");
            CREATE_DOC2(LANG_MENU("doc2IPv4Route"), "html/ipconf_ipv4_route.html");
            CREATE_DOC2(LANG_MENU("doc2IPv4Arp"), "html/ipconf_ipv4_arp.html");
        CREATE_FOLDER2(LANG_MENU("fld2IPv6"));
            CREATE_DOC2(LANG_MENU("doc2IPv6Intf"), "html/ipconf_ipv6_intf.html");
            CREATE_DOC2(LANG_MENU("doc2IPv6Addr"), "html/ipconf_ipv6_addr.html");
            CREATE_DOC2(LANG_MENU("doc2IPv6Route"), "html/ipconf_ipv6_route.html");
            CREATE_DOC2(LANG_MENU("doc2IPv6Neigh"), "html/ipconf_ipv6_neigh.html");
    #ifdef CONFIG_SYS_L3_ROUTE_RIP
        CREATE_FOLDER2(LANG_MENU("fld2Rip"));
            CREATE_DOC2(LANG_MENU("doc2IPv4Rip"), "html/route_rip.html");
    #endif
    #ifdef CONFIG_SYS_L3_ROUTE_OSPF
        CREATE_FOLDER2(LANG_MENU("fld2Ospf"));
            CREATE_DOC2(LANG_MENU("doc2IPv4Ospf"), "html/route_ospf.html");
    #endif
    #ifdef CONFIG_SYS_L3_VRRP
        CREATE_FOLDER2(LANG_MENU("fld2Vrrp"));
            CREATE_DOC2(LANG_MENU("doc2IPv4Vrrp"), "html/ipv4_vrrp.html");
    #endif
#endif

    CREATE_FOLDER1(LANG_MENU("fld1Sec"));
      #ifdef CONFIG_SYS_LIB_RADIUS
        CREATE_DOC1(LANG_MENU("doc1SecRadius"), "html/sec_radius.html");
      #endif
      #ifdef CONFIG_SYS_LIB_TACPLUS
        CREATE_DOC1(LANG_MENU("doc1SecTacplus"), "html/sec_tacplus.html");
      #endif
      #ifdef CONFIG_SYS_AAA
        CREATE_FOLDER2(LANG_MENU("fld2SecAAA"));
            CREATE_DOC2(LANG_MENU("doc2SecAAALoginList"), "html/sec_aaa_login.html");
            CREATE_DOC2(LANG_MENU("doc2SecAAALoginAuthen"), "html/sec_aaa_loginAuth.html");
      #endif

        CREATE_FOLDER2(LANG_MENU("fld2SecMgmtAccess"));
          #if defined(CONFIG_SYS_MANAGEMENT_VLAN) && !defined(CONFIG_SYS_L3_ARCHITECTURE)
            CREATE_DOC2(LANG_MENU("doc2SecMgmtVLAN"), "html/sec_mgmt_vlan.html");
          #endif
            CREATE_DOC2(LANG_MENU("doc2SecMgmtService"), "html/sec_mgmt_method.html");
          #ifdef CONFIG_SYS_MGMT_ACL
            CREATE_DOC2(LANG_MENU("doc2SecMgmtACL"), "html/sec_mgmt_acl.html");
            CREATE_DOC2(LANG_MENU("doc2SecMgmtACE"), "html/sec_mgmt_aclAce.html");
          #endif
      #ifdef CONFIG_SYS_PROTO_AUTHMGR
        CREATE_FOLDER2(LANG_MENU("fld2SecAuthmgr"));
            CREATE_DOC2(LANG_MENU("doc2SecAuthmgrProp"), "html/sec_authmgr_prop.html");
            CREATE_DOC2(LANG_MENU("doc2SecAuthmgrPort"), "html/sec_authmgr_port.html");
         #ifdef CONFIG_SYS_PROTO_MACAUTH
            CREATE_DOC2(LANG_MENU("doc2SecAuthmgrMacLocal"), "html/sec_authmgr_macAcct.html");
         #endif
         #ifdef CONFIG_SYS_PROTO_WEBAUTH
            CREATE_DOC2(LANG_MENU("doc2SecAuthmgrWebLocal"), "html/sec_authmgr_webAcct.html");
         #endif
            CREATE_DOC2(LANG_MENU("doc2SecAuthmgrSess"), "html/sec_authmgr_sess.html");
      #endif
      #ifdef CONFIG_SYS_PORT_SECURITY
        CREATE_DOC1(LANG_MENU("doc1SecPort"), "html/sec_port.html");
      #endif
      #ifdef CONFIG_SYS_PORT_ISOLATION
        CREATE_DOC1(LANG_MENU("doc1SecProtected"), "html/sec_protect.html");
      #endif
      #ifdef CONFIG_SYS_STORM_CTRL
        CREATE_DOC1(LANG_MENU("doc1SecStorm"), "html/sec_stromControl.html");
      #endif
      #ifdef CONFIG_SYS_DOS_PROTECTION
        CREATE_FOLDER2(LANG_MENU("fld2SecDos"));
            CREATE_DOC2(LANG_MENU("doc2SecDosProp"), "html/sec_dos_prop.html");
            CREATE_DOC2(LANG_MENU("doc2SecDosPort"), "html/sec_dos_port.html");
      #endif
      #ifdef CONFIG_SYS_PROTO_DAI
        CREATE_FOLDER2(LANG_MENU("fld2SecDAI"));
            CREATE_DOC2(LANG_MENU("doc2SecDAI"), "html/sec_dai_prop.html");
            CREATE_DOC2(LANG_MENU("doc2SecDAIStatistic"), "html/sec_dai_statistics.html");
      #endif
      #ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
        CREATE_FOLDER2(LANG_MENU("fld2SecDS"));
            CREATE_DOC2(LANG_MENU("doc2SecDS"), "html/sec_ds_prop.html");
            CREATE_DOC2(LANG_MENU("doc2SecDSStatistic"), "html/sec_ds_statistics.html");
            CREATE_DOC2(LANG_MENU("doc2SecDSOpt82Global"), "html/sec_ds_option82Prop.html");
        #ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
            CREATE_DOC2(LANG_MENU("doc2SecDSOpt82Circuit"), "html/sec_ds_option82CID.html");
        #endif
      #endif
      #ifdef CONFIG_SYS_PROTO_ISG
        CREATE_FOLDER2(LANG_MENU("fld2SecISG"));
            CREATE_DOC2(LANG_MENU("doc2SecISGPort"), "html/sec_ipsg_prop.html");
            CREATE_DOC2(LANG_MENU("doc2SecISGBind"), "html/sec_ipsg_binding.html");
            CREATE_DOC2(LANG_MENU("doc2SecISGDatabase"), "html/sec_ipsg_database.html");
      #endif

  #ifdef CONFIG_SYS_USER_DEFINED_ACL
    
    CREATE_FOLDER1(LANG_MENU("fld1Acl"));
        CREATE_DOC1(LANG_MENU("doc1AclMAC"), "html/acl_acl.html?type=1");
        CREATE_DOC1(LANG_MENU("doc1AclMACAce"), "html/acl_mac_ace.html");
        CREATE_DOC1(LANG_MENU("doc1AclIPv4"), "html/acl_acl.html?type=2");
        CREATE_DOC1(LANG_MENU("doc1AclIPv4Ace"), "html/acl_ip_ace.html?type=2");
        CREATE_DOC1(LANG_MENU("doc1AclIPv6"), "html/acl_acl.html?type=3");
        CREATE_DOC1(LANG_MENU("doc1AclIPv6Ace"), "html/acl_ip_ace.html?type=3");
        CREATE_DOC1(LANG_MENU("doc1AclBind"), "html/acl_binding.html");
  #endif

    CREATE_FOLDER1(LANG_MENU("fld1Qos"));
        CREATE_FOLDER2(LANG_MENU("fld2QosGeneral"));
            CREATE_DOC2(LANG_MENU("doc2QosGeneralProp"), "html/qos_prop.html");
            CREATE_DOC2(LANG_MENU("doc2QosGeneralQueue"), "html/qos_queue.html");
            CREATE_DOC2(LANG_MENU("doc2QosGeneralCoS"), "html/qos_mapCos.html");
            CREATE_DOC2(LANG_MENU("doc2QosGeneralDSCP"), "html/qos_mapDscp.html");
            CREATE_DOC2(LANG_MENU("doc2QosGeneralIPP"), "html/qos_mapIpp.html");
      #ifdef CONFIG_SYS_BW_CTRL
        CREATE_FOLDER2(LANG_MENU("fld2QosRate"));
            CREATE_DOC2(LANG_MENU("doc2QosRateBwctl"), "html/qos_rate_ctrl.html");
            CREATE_DOC2(LANG_MENU("doc2QosRateEgressQueue"), "html/qos_rate_egressQue.html");
      #endif

    CREATE_FOLDER1(LANG_MENU("fld1Diag"));
      #ifdef CONFIG_SYS_SYSLOG
        CREATE_FOLDER2(LANG_MENU("fld2DiagLog"));
            CREATE_DOC2(LANG_MENU("doc2DiagLogProp"), "html/diag_log_global.html");
            CREATE_DOC2(LANG_MENU("doc2DiagLogRemote"), "html/diag_log_remote.html");
      #endif

      #ifdef CONFIG_SYS_MIRROR
        CREATE_DOC1(LANG_MENU("doc1DiagMirror"), "html/diag_mirror.html");
      #endif
      #ifdef CONFIG_SYS_PING_TEST
        CREATE_DOC1(LANG_MENU("doc1DiagPing"), "html/diag_ping.html");
      #endif
      #ifdef CONFIG_SYS_TRACE_ROUTE
        CREATE_DOC1(LANG_MENU("doc1DiagTraceRoute"), "html/diag_traceroute.html");
      #endif
      #ifdef CONFIG_SYS_CABLE_DIAG
        CREATE_DOC1(LANG_MENU("doc1DiagCopper"), "html/diag_copper.html");
      #ifdef CONFIG_SYS_OPTICALMOD_STATUS
        CREATE_DOC1(LANG_MENU("doc1DiagFiber"), "html/diag_optical.html");
      #endif
      #endif
      #ifdef CONFIG_SYS_PROTO_UDLD
        CREATE_FOLDER2(LANG_MENU("fld2SecUDLD"));
            CREATE_DOC2(LANG_MENU("doc2SecUDLDProp"), "html/diag_udld_prop.html");
            CREATE_DOC2(LANG_MENU("doc2SecUDLDNeighbors"), "html/diag_udld_neighbor.html");
      #endif

    CREATE_FOLDER1(LANG_MENU("fld1Mgmt"));
        CREATE_DOC1(LANG_MENU("doc1MgmtAccount"), "html/mgmt_acct.html");
        CREATE_FOLDER2(LANG_MENU("fld2MgmtFirmware"));
            
      #ifdef CONFIG_SYS_DUAL_IMAGE
            CREATE_DOC2(LANG_MENU("doc2MgmtUpgradeBackup"), "html/mgmt_dualimg_action.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtActiveImg"), "html/mgmt_img_act.html");
      #else
            CREATE_DOC2(LANG_MENU("doc2MgmtUpgradeBackup"), "html/mgmt_img_action.html");
      #endif
        CREATE_FOLDER2(LANG_MENU("fld2MgmtConfig"));
            CREATE_DOC2(LANG_MENU("doc2MgmtUpgradeBackup"), "html/mgmt_cfg_action.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtConfigSave"), "html/mgmt_cfg_save.html");
      #ifdef CONFIG_SYS_UI_SNMP
        CREATE_FOLDER2(LANG_MENU("fld2MgmtSNMP"));
          #ifdef CONFIG_SYS_SNMPV3
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPView"), "html/mgmt_snmp_view.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPGroup"), "html/mgmt_snmp_group.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPCommunity"), "html/mgmt_snmp_community.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPUser"), "html/mgmt_snmp_user.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPEngine"), "html/mgmt_snmp_engineID.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPTrapEvent"), "html/mgmt_snmp_trapEvent.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPNotify"), "html/mgmt_snmp_notify.html");
          #else
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPCommunity"), "html/mgmt_snmpv2_community.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPTrapEvent"), "html/mgmt_snmpv2_trapEvent.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtSNMPNotify"), "html/mgmt_snmpv2_notify.html");
          #endif
      #endif
      #ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
        CREATE_FOLDER2(LANG_MENU("fld2MgmtRMON"));
            CREATE_DOC2(LANG_MENU("doc2MgmtRMONStatistic"), "html/mgmt_rmon_statistics.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtRMONHistory"), "html/mgmt_rmon_history.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtRMONEvent"), "html/mgmt_rmon_event.html");
            CREATE_DOC2(LANG_MENU("doc2MgmtRMONAlarm"), "html/mgmt_rmon_alarm.html");
      #endif
}

static int32 cgi_home_main_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    cJSON *lags = NULL;
    cJSON *lag = NULL;
    char portStr[CAPA_PORT_STR_LEN + 1];
    char modulus[SYS_BUF1024_LEN];
    char *accessTypeEnv;
    sys_board_conf_t boardConf;
    sys_logic_port_t lp;
    sys_vid_t vid;
    char sysModel[CAPA_SYS_NAME_LEN];
    char sysHomeLogo[CAPA_SYS_NAME_LEN + 1];
    char systitle[CAPA_SYS_NAME_LEN];
    char sysname[CAPA_SYS_NAME_LEN + 1];

    SYS_MEM_CLEAR(sysname);
    SYS_MEM_CLEAR(boardConf);
    SYS_MEM_CLEAR(sysModel);
    SYS_MEM_CLEAR(sysHomeLogo);
    SYS_MEM_CLEAR(systitle);
    
    sal_sys_boardConf_get(&boardConf);
    sal_vlan_defaultVid_get(&vid);
    sal_sys_bdinfoVar_get("modelname", sysModel);
    sal_sys_systemName_get(sysname);
    sal_sys_system_home_logo_get(sysHomeLogo);
    sal_sys_title_get(systitle);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddStringToObject(data, CGI_HOME_USER, g_username);
    cJSON_AddNumberToObject(data, CGI_HOME_PRIV, g_priv);
    cJSON_AddStringToObject(data, CGI_HOME_MODEL, sysModel);
    cJSON_AddStringToObject(data, CGI_HOME_TITLE, sysname);
    cJSON_AddNumberToObject(data, CGI_VLAN_DEFAULT_VLAN, vid);
    cJSON_AddStringToObject(data, CGI_HOME_LOGO, sysHomeLogo);

    accessTypeEnv = getenv("ACCESS_TYPE");

    if (accessTypeEnv == NULL)
    {
        SYS_MEM_CLEAR(modulus);
        if(SYS_ERR_OK != sal_util_rsaModulus_get(modulus))
            SYS_MEM_CLEAR(modulus);
    }
    cJSON_AddStringToObject(data, CGI_SYS_MODULUS, modulus);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(portStr);

        port = cJSON_CreateObject();

        LP2WEBSTR(lp, portStr);
        cJSON_AddStringToObject(port, CGI_PORT, portStr);

        cJSON_AddItemToArray(ports, port);
    }

    lags = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_LAGS, lags);

    FOR_EACH_TRUNK(lp)
    {
        SYS_MEM_CLEAR(portStr);

        lag = cJSON_CreateObject();
        osal_sprintf(portStr, "%d", TRK2UID(lp));
        cJSON_AddStringToObject(lag, CGI_LAG, portStr);

        cJSON_AddItemToArray(lags, lag);
    }

    _cgi_home_menu_get(data);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_home_monitor_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, "logAlarm", (SYS_ERR_OK == sal_util_file_exist(SYS_LOG_USER_ALARM_FILE)));

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_home_empty_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, "empty", TRUE);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_home_debug_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *msgs = NULL;
    FILE *fp = NULL;
    char str[SYS_BUF2048_LEN];

    CGI_GET_JSON_CREATE(root, data);

    if (NULL == (fp = fopen(CGI_DBG_TARGET_FILE, "r")))
    {
        cJSON_AddBoolToObject(data, "empty", TRUE);
    }
    else
    {
        msgs = cJSON_CreateArray();
        cJSON_AddItemToObject(data, "msgs", msgs);

        SYS_MEM_CLEAR(str);
        while(fgets(str, sizeof(str), fp))
        {
            cJSON_AddItemToArray(msgs, cJSON_CreateString(str));
            SYS_MEM_CLEAR(str);
        }

        fclose(fp);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_home_systemSet_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char macStr[CAPA_MAC_STR_LEN];
    sys_mac_t sysMacAddr;
    char sysModel[CAPA_SYS_NAME_LEN + 1];
    char tmp[SYS_BUF64_LEN];
    char sn[SYS_BUF64_LEN];
    
    SYS_MEM_CLEAR(sysMacAddr);
    SYS_MEM_CLEAR(macStr);
    SYS_MEM_CLEAR(sysModel);
    SYS_MEM_CLEAR(tmp);
    SYS_MEM_CLEAR(sn);

    sal_sys_macAddr_get(&sysMacAddr);
    sys_util_mac2Str(sysMacAddr.octet, macStr);
    sal_sys_bdinfoVar_get("modelname", sysModel);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddStringToObject(data, CGI_HOME_SYSTEM_MAC, macStr);
        
    if (SYS_ERR_OK == sal_sys_bdinfoVar_get("serialnumber", sn))
        cJSON_AddStringToObject(data, CGI_HOME_SYSTEM_SN, sn);

    cJSON_AddStringToObject(data, CGI_HOME_SYSTEM_ID, sysModel);   

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_home_sysOemset_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    
    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddStringToObject(data, CGI_HOME_OEM_LOGO, "logo_en");   

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

