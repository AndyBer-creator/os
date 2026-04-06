/* Home */
var url_get_mainPage = 'data/main.json';
var url_get_loginInfo = 'data/login.json';
var url_set_loginAuth = 'data/login_ok.json';
var url_get_loginStatus = 'data/status_ok.json';
var url_set_save = 'data/status_ok.json';
var url_set_logout = 'data/status_ok.json';
var url_get_monitor = 'data/status_ok.json';
var url_get_cgiDbg = '../data/cgi_debug.json';
var url_set_cgiDbgDel = '../data/status_ok.json';

/* Status */
var url_get_statusSysinfo = '../data/status_sysinfo.json';
var url_get_statusSysinfoEdit = '../data/status_sysinfoEdit.json';
var url_set_statusSysinfo = '../data/status_ok.json';
var url_get_statusPanelLayout = '../data/status_panelLayout.json';
var url_get_statusPanel = '../data/status_panel.json';
var url_get_statusCpuMem = '../data/status_ok.json';
var url_get_statusSyslog = '../data/status_syslog.json';
var url_set_statusSyslogClear = '../data/status_ok.json';
var url_get_statusPortCnt = '../data/status_port_cnt.json';
var url_set_statusPortCntClear = '../data/status_ok.json';
<!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
var url_get_statusPortErrorDisabled = '../data/status_port_errDisable.json';
var url_set_statusPortErrorDisabled = '../data/status_ok.json';
<!--#endif-->
var url_get_statusPortBwutilz = '../data/status_port_bwutilz.json';
var url_get_statusMac = '../data/status_mac.json';
var url_set_statusMac = '../data/status_ok.json';
var url_get_statusLag = '../data/status_lag.json';

/* Network */
var url_get_networkIp = '../data/network_ip.json';
var url_set_networkIp = '../data/status_ok.json';
var url_get_networkIpv4 = '../data/network_ipv4.json';
var url_set_networkIpv4 = '../data/status_ok.json';
var url_get_networkIpv6 = '../data/network_ipv6.json';
var url_set_networkIpv6 = '../data/status_ok.json';
<!--#ifdef CONFIG_SYS_ADVANCE_DNS-->
var url_get_networkDns = '../data/network_dns.json';
var url_set_networkDns = '../data/status_ok.json';
var url_set_networkDnsAdd = '../data/status_ok.json';
var url_set_networkDnsDel = '../data/status_ok.json';
var url_get_networkHost = '../data/network_dnsHosts.json';
var url_set_networkHost = '../data/status_ok.json';
var url_set_networkHostAdd = '../data/status_ok.json';
var url_set_networkHostDel = '../data/status_ok.json';
<!--#endif-->
var url_get_networkTime = '../data/network_time_sys.json';
var url_set_networkTime = '../data/status_ok.json';

/* Port */
var url_get_portPort = '../data/port_port.json';
var url_get_portPortEdit = '../data/port_portEdit.json';
var url_set_portPortEdit = '../data/status_ok.json';
<!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
var url_get_portErrDisable = '../data/port_errDisable.json';
var url_set_portErrDisable = '../data/status_ok.json';
<!--#endif-->
var url_get_portJumbo = '../data/port_jumbo.json';
var url_set_portJumbo = '../data/status_ok.json';
var url_get_portEEE = '../data/port_eee.json';
var url_get_portEEEEdit = '../data/port_eeeEdit.json';
var url_set_portEEEEdit = '../data/status_ok.json';
var url_get_portLAGMgmt = '../data/port_lag_mgmt.json';
var url_set_portLAGMgmt = '../data/status_ok.json';
var url_get_portLAGMgmtEdit = '../data/port_lag_mgmtEdit.json';
var url_set_portLAGMgmtEdit = '../data/status_ok.json';
var url_get_portLAGPort = '../data/port_lag_port.json';
var url_get_portLAGPortEdit = '../data/port_lag_portEdit.json';
var url_set_portLAGPortEdit = '../data/status_ok.json';
var url_get_portLAGLacp = '../data/port_lag_lacp.json';
var url_set_portLAGLacp = '../data/status_ok.json';
var url_get_portLAGLacpEdit = '../data/port_lag_lacpEdit.json';
var url_set_portLAGLacpEdit = '../data/status_ok.json';

/* VLAN */
var url_get_vlanCreate = '../data/vlan_vlan_create.json';
var url_set_vlanCreate = '../data/status_ok.json';
var url_set_vlanEdit = '../data/status_ok.json';
var url_set_vlanDel = '../data/status_ok.json';
var url_get_vlanConf = '../data/vlan_vlan_configuration.json';
var url_set_vlanConf = '../data/status_ok.json';
var url_get_vlanPort = '../data/vlan_vlan_port.json';
var url_get_vlanPortEdit = '../data/vlan_vlan_portEdit.json';
var url_set_vlanPortEdit = '../data/status_ok.json';
var url_get_vlanMembership = '../data/vlan_vlan_membership.json';
var url_get_vlanMembershipEdit = '../data/vlan_vlan_membershipEdit.json';
var url_set_vlanMembershipEdit = '../data/status_ok.json';


<!--#ifdef CONFIG_SYS_MACVLAN-->
/* Mac VLAN */
var url_get_vlanMacGroup = '../data/vlan_mac_group.json';
var url_set_vlanMacGroup = '../data/status_ok.json';
var url_add_vlanMacGroup = '../data/status_ok.json';
var url_set_vlanMacGroupDel = '../data/status_ok.json';
var url_get_vlanMacBinding = '../data/vlan_mac_binding.json';
var url_set_vlanMacBinding = '../data/status_ok.json';
var url_set_vlanMacBindingDel = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_VOICEVLAN-->
/* Voice VLAN */
var url_get_vlanVoiceProp = '../data/vlan_voice_prop.json';
var url_set_vlanVoiceProp = '../data/status_ok.json';
var url_get_vlanVoicePortEdit = '../data/vlan_voice_portEdit.json';
var url_set_vlanVoicePortEdit = '../data/status_ok.json';
var url_get_vlanVoiceOui = '../data/vlan_voice_oui.json';
var url_set_vlanVoiceOui = '../data/status_ok.json';
var url_set_vlanVoiceOuiDel = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTOCOL_VLAN-->
/* Protocol VLAN */
var url_get_vlanProtocolGroup = '../data/vlan_protocol_group.json';
var url_set_vlanProtocolGroup = '../data/status_ok.json';
var url_set_vlanProtocolGroupDel = '../data/status_ok.json';
var url_get_vlanProtocolBinding = '../data/vlan_protocol_binding.json';
var url_set_vlanProtocolBinding = '../data/status_ok.json';
var url_set_vlanProtocolBindingDel = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_SURVEILLANCEVLAN-->
/* Surveillance VLAN */
var url_get_vlanSurveillanceProp = '../data/vlan_surveillance_prop.json';
var url_set_vlanSurveillanceProp = '../data/status_ok.json';
var url_get_vlanSurveillancePortEdit = '../data/vlan_surveillance_portEdit.json';
var url_set_vlanSurveillancePortEdit = '../data/status_ok.json';
var url_get_vlanSurveillanceOui = '../data/vlan_surveillance_oui.json';
var url_set_vlanSurveillanceOui = '../data/status_ok.json';
var url_set_vlanSurveillanceOuiDel = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_GVRP-->
var url_get_gvrpProp = '../data/vlan_gvrp_property.json';
var url_set_gvrpProp = '../data/status_ok.json';
var url_get_gvrpPropEdit = '../data/vlan_gvrp_propertyEdit.json';
var url_set_gvrpPropEdit = '../data/status_ok.json';
var url_get_gvrpMember = '../data/vlan_gvrp_member.json';
var url_get_gvrpPortCnt = '../data/vlan_gvrp_statistics.json';
var url_set_gvrpPortCntClear = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_GARP-->
var url_get_gmrpProp = '../data/gmrp_property.json';
var url_set_gmrpProp = '../data/status_ok.json';
var url_get_gmrpPropEdit = '../data/gmrp_propertyEdit.json';
var url_set_gmrpPropEdit = '../data/status_ok.json';;
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING-->
/* Multicast */
var url_get_mcastProp = '../data/mcast_general_prop.json';
var url_set_mcastProp = '../data/status_ok.json';

var url_get_mcastGroup = '../data/mcast_general_group.json';
var url_get_mcastGroupAdd = '../data/mcast_general_groupAdd.json';
var url_set_mcastGroupAdd = '../data/status_ok.json';
var url_get_mcastGroupEdit = '../data/mcast_general_groupEdit.json';
var url_set_mcastGroupEdit = '../data/status_ok.json';
var url_set_mcastGroupDel = '../data/status_ok.json';
var url_get_mcastRouter = '../data/mcast_general_router.json';

<!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER-->
var url_get_mcastRouterAdd = '../data/mcast_general_routerAdd.json';
var url_set_mcastRouterAdd = '../data/status_ok.json';
var url_get_mcastRouterEdit = '../data/mcast_general_routerEdit.json';
var url_set_mcastRouterEdit = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD-->
var url_get_mcastFwdAll = '../data/mcast_general_fwdAll.json';
var url_get_mcastFwdAllAdd = '../data/mcast_general_fwdAllAdd.json';
var url_set_mcastFwdAllAdd = '../data/status_ok.json';
var url_get_mcastFwdAllEdit = '../data/mcast_general_fwdAllEdit.json';
var url_set_mcastFwdAllEdit = '../data/status_ok.json';
var url_set_mcastFwdAllDel = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING-->
var url_get_mcastThrot = '../data/mcast_general_throt.json';
var url_get_mcastThrotEdit = '../data/mcast_general_ThrotEdit.json';
var url_set_mcastThrotEdit = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING-->
var url_get_mcastProfiles = '../data/mcast_general_profiles.json';
var url_get_mcastProfilesAdd = '../data/mcast_general_profilesAdd.json';
var url_set_mcastProfilesAdd = '../data/status_ok.json';
var url_get_mcastProfilesEdit = '../data/mcast_general_profiles.json';
var url_set_mcastProfilesEdit = '../data/status_ok.json';
var url_set_mcastProfilesDel = '../data/status_ok.json';

var url_get_mcastFilter = '../data/mcast_general_filter.json';
var url_get_mcastFilterEdit = '../data/mcast_general_FilterEdit.json';
var url_set_mcastFilterEdit = '../data/status_ok.json';
<!--#endif-->

var url_get_mcastIgmpProp = '../data/mcast_igmp_prop.json';
var url_get_mcastIgmpPropEdit = '../data/mcast_igmp_propEdit.json';
var url_set_mcastIgmpProp = '../data/status_ok.json';
var url_set_mcastIgmpPropEdit = '../data/status_ok.json';
<!--#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER-->
var url_get_mcastIgmpQuerier = '../data/mcast_igmp_querier.json';
var url_get_mcastIgmpQuerierEdit = '../data/mcast_igmp_querierEdit.json';
var url_set_mcastIgmpQuerierEdit = '../data/status_ok.json';
<!--#endif-->
var url_get_mcastIgmpStatic = '../data/mcast_igmp_statistics.json';
var url_set_mcastIgmpStaticClear = '../data/status_ok.json';

<!--#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING-->
var url_get_mcastMldProp = '../data/mcast_mld_prop.json';
var url_get_mcastMldPropEdit = '../data/mcast_mld_propEdit.json';
var url_set_mcastMldProp = '../data/status_ok.json';
var url_set_mcastMldPropEdit = '../data/status_ok.json';
var url_get_mcastMldStatic = '../data/mcast_mld_statistics.json';
var url_set_mcastMldStaticClear = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_MVR-->
var url_get_mvrProp = '../data/mcast_mvr_prop.json';
var url_set_mvrProp = '../data/status_ok.json';
var url_get_mvrGroup = '../data/mcast_mvr_group.json';
var url_get_mvrGroupAdd = '../data/mcast_mvr_groupAdd.json';
var url_set_mvrGroupAdd = '../data/status_ok.json';
var url_get_mvrGroupEdit = '../data/mcast_mvr_groupEdit.json';
var url_set_mvrGroupEdit = '../data/status_ok.json';
var url_set_mvrGroupDel = '../data/status_ok.json';
var url_get_mvrPort = '../data/mcast_mvr_port.json';
var url_get_mvrPortEdit = '../data/mcast_mvr_portEdit.json';
var url_set_mvrPortEdit = '../data/status_ok.json';
<!--#endif-->
<!--#endif-->

/* MAC Address Table */
var url_get_macDynamic = '../data/mac_dynamic.json';
var url_set_macDynamic = '../data/status_ok.json';
var url_get_macStatic = '../data/mac_static.json';
var url_set_macStaticAdd = '../data/status_ok.json';
var url_set_macStaticDel = '../data/status_ok.json';
var url_set_macStaticEdit = '../data/status_ok.json';
<!-- #ifdef CONFIG_SYS_BLACK_HOLE_MAC-->
var url_get_macFilter = '../data/mac_filter.json';
var url_set_macFilterAdd = '../data/status_ok.json';
var url_set_macFilterDel = '../data/status_ok.json';
var url_set_macFilterEdit = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PORT_SECURITY-->
var url_get_macPsecure = '../data/mac_psecure.json';
var url_set_macPsecureAdd = '../data/status_ok.json';
var url_set_macPsecureDel = '../data/status_ok.json';
var url_set_macPsecureEdit = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_STP-->
/* Spanning Tree */
var url_get_stpGlobal = '../data/stp_global.json';
var url_set_stpGlobal = '../data/status_ok.json';
var url_get_stpIntf = '../data/stp_intf.json';
var url_get_stpIntfEdit = '../data/stp_intfEdit.json';
var url_set_stpIntfEdit = '../data/status_ok.json';
var url_set_stpIntfMigrate = '../data/status_ok.json';
<!--#ifdef CONFIG_SYS_PROTO_STP_MSTP-->
var url_get_stpMsti = '../data/stp_msti.json';
var url_get_stpMstiEdit = '../data/stp_mstiEdit.json';
var url_set_stpMstiEdit = '../data/status_ok.json';
var url_get_stpMstiIntf = '../data/stp_mstiIntf.json';
var url_set_stpMstiIntf = '../data/status_ok.json';
var url_get_stpMstiIntfEdit = '../data/stp_mstiIntfEdit.json';
var url_set_stpMstiIntfEdit = '../data/status_ok.json';
<!--#endif-->
var url_get_stpStats = '../data/stp_statistics.json';
var url_get_stpStatsIntf = '../data/stp_statisticsIntf.json';
var url_set_stpStatsIntfClear = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_LLDP-->
/* LLDP */
var url_get_discoveryLldpProp = '../data/discovery_lldp_prop.json';
var url_set_discoveryLldpProp = '../data/status_ok.json';
var url_get_discoveryLldpPort = '../data/discovery_lldp_port.json';
var url_get_discoveryLldpPortEdit = '../data/discovery_lldp_portEdit.json';
var url_set_discoveryLldpPortEdit = '../data/status_ok.json';
<!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
var url_get_discoveryLldpMedPolicy = '../data/discovery_lldp_medPolicy.json';
var url_get_discoveryLldpMedPolicyAdd = '../data/discovery_lldp_medPolicyAdd.json';
var url_set_discoveryLldpMedPolicyAdd = '../data/status_ok.json';
var url_get_discoveryLldpMedPolicyEdit = '../data/discovery_lldp_medPolicyEdit.json';
var url_set_discoveryLldpMedPolicyEdit = '../data/status_ok.json';
var url_set_discoveryLldpMedPolicyDel = '../data/status_ok.json';
var url_get_discoveryLldpMedPort = '../data/discovery_lldp_medPort.json';
var url_set_discoveryLldpMedPort = '../data/status_ok.json';
var url_get_discoveryLldpMedPortEdit = '../data/discovery_lldp_medPortEdit.json';
var url_set_discoveryLldpMedPortEdit = '../data/status_ok.json';
<!--#endif-->
var url_get_discoveryLldpLocal = '../data/discovery_lldp_localInfo.json';
var url_get_discoveryLldpLocalDetail = '../data/discovery_lldp_localInfoDetail.json';
var url_get_discoveryLldpNeighbor = '../data/discovery_lldp_neighborInfo.json';
var url_set_discoveryLldpNeighborClear = '../data/status_ok.json';
var url_set_discoveryLldpNeighborRefresh = '../data/status_none.json';
var url_get_discoveryLldpNeighborDetail = '../data/discovery_lldp_neighborDetail.json';
var url_get_discoveryLldpStatistic = '../data/discovery_lldp_statistic.json';
var url_set_discoveryLldpStatisticGlobalClear = '../data/status_ok.json';
var url_set_discoveryLldpStatisticPortClear = '../data/status_ok.json';
var url_set_discoveryLldpStatisticRefresh = '../data/status_none.json';
var url_get_discoveryLldpOverload = '../data/discovery_lldp_overloading.json';
var url_get_discoveryLldpOverloadDetail = '../data/discovery_lldp_overloadingDetail.json';
<!--#endif-->


<!--#ifdef CONFIG_SYS_L3_ROUTE-->
/* IP Configuration */
var url_get_ipconfIpv4Intf = '../data/ipconf_ipv4_intf.json';
var url_set_ipconfIpv4Intf = '../data/status_ok.json';
var url_get_ipconfIpv4IntfAdd = '../data/ipconf_ipv4_intfAdd.json';
var url_set_ipconfIpv4IntfAdd = '../data/status_ok.json';
var url_get_ipconfIpv4IntfEdit = '../data/ipconf_ipv4_intfEdit.json';
var url_set_ipconfIpv4IntfEdit = '../data/status_ok.json';
var url_set_ipconfIpv4IntfDel = '../data/status_ok.json';
var url_get_ipconfIpv4Route = '../data/ipconf_ipv4_route.json';
var url_set_ipconfIpv4Route = '../data/status_ok.json';
var url_get_ipconfIpv4RouteAdd = '../data/ipconf_ipv4_routeAdd.json';
var url_set_ipconfIpv4RouteAdd = '../data/status_ok.json';
var url_get_ipconfIpv4RouteEdit = '../data/ipconf_ipv4_routeEdit.json';
var url_set_ipconfIpv4RouteEdit = '../data/status_ok.json';
var url_set_ipconfIpv4RouteDel = '../data/status_ok.json';
var url_get_ipconfIpv4Arp = '../data/ipconf_ipv4_arp.json';
var url_set_ipconfIpv4Arp = '../data/status_ok.json';
var url_get_ipconfIpv4ArpAdd = '../data/ipconf_ipv4_arpAdd.json';
var url_set_ipconfIpv4ArpAdd = '../data/status_ok.json';
var url_set_ipconfIpv4ArpEdit = '../data/status_ok.json';
var url_set_ipconfIpv4ArpDel = '../data/status_ok.json';
var url_get_ipconfIpv4ArpProxy = '../data/ipconf_ipv4_arpProxy.json';
var url_set_ipconfIpv4ArpProxy = '../data/status_ok.json';

var url_get_ipconfIpv6Intf = '../data/ipconf_ipv6_intf.json';
var url_set_ipconfIpv6Intf = '../data/status_ok.json';
var url_get_ipconfIpv6IntfAdd = '../data/ipconf_ipv6_intfAdd.json';
var url_set_ipconfIpv6IntfAdd = '../data/status_ok.json';
var url_get_ipconfIpv6IntfEdit = '../data/ipconf_ipv6_intfEdit.json';
var url_set_ipconfIpv6IntfEdit = '../data/status_ok.json';
var url_set_ipconfIpv6IntfDel = '../data/status_ok.json';
var url_get_ipconfIpv6Addr = '../data/ipconf_ipv6_addr.json';
var url_set_ipconfIpv6Addr = '../data/status_ok.json';
var url_get_ipconfIpv6AddrAdd = '../data/ipconf_ipv6_addrAdd.json';
var url_set_ipconfIpv6AddrAdd = '../data/status_ok.json';
var url_set_ipconfIpv6AddrDel = '../data/status_ok.json';
var url_get_ipconfIpv6Route = '../data/ipconf_ipv6_route.json';
var url_set_ipconfIpv6Route = '../data/status_ok.json';
var url_get_ipconfIpv6RouteAdd = '../data/ipconf_ipv6_routeAdd.json';
var url_set_ipconfIpv6RouteAdd = '../data/status_ok.json';
var url_get_ipconfIpv6RouteEdit = '../data/ipconf_ipv6_routeEdit.json';
var url_set_ipconfIpv6RouteEdit = '../data/status_ok.json';
var url_set_ipconfIpv6RouteDel = '../data/status_ok.json';
var url_get_ipconfIpv6Neigh = '../data/ipconf_ipv6_neigh.json';
var url_set_ipconfIpv6Neigh = '../data/status_ok.json';
var url_get_ipconfIpv6NeighAdd = '../data/ipconf_ipv6_neighAdd.json';
var url_set_ipconfIpv6NeighAdd = '../data/status_ok.json';
var url_get_ipconfIpv6NeighEdit = '../data/ipconf_ipv6_neighEdit.json';
var url_set_ipconfIpv6NeighEdit = '../data/status_ok.json';
var url_set_ipconfIpv6NeighDel = '../data/status_ok.json';

<!--#endif-->


/* Security */
<!--#ifdef CONFIG_SYS_LIB_RADIUS-->
var url_get_secRadius = '../data/sec_radius.json';
var url_set_secRadius = '../data/status_ok.json';
var url_get_secRadiusAdd = '../data/sec_radiusAdd.json';
var url_set_secRadiusAdd = '../data/status_ok.json';
var url_get_secRadiusEdit = '../data/sec_radiusEdit.json';
var url_set_secRadiusEdit = '../data/status_ok.json';
var url_set_secRadiusDel = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_LIB_TACPLUS-->
var url_get_secTacplus = '../data/sec_tacplus.json';
var url_set_secTacplus = '../data/status_ok.json';
var url_get_secTacplusAdd = '../data/sec_tacplusAdd.json';
var url_set_secTacplusAdd = '../data/status_ok.json';
var url_get_secTacplusEdit = '../data/sec_tacplusEdit.json';
var url_set_secTacplusEdit = '../data/status_ok.json';
var url_set_secTacplusDel = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_AAA-->
var url_get_secAaaLogin = '../data/sec_aaa_login.json';
var url_set_secAaaLogin = '../data/status_ok.json';
var url_get_secAaaLoginAdd = '../data/sec_aaa_loginAdd.json';
var url_set_secAaaLoginAdd = '../data/status_ok.json';
var url_get_secAaaLoginEdit = '../data/sec_aaa_loginEdit.json';
var url_set_secAaaLoginEdit = '../data/status_ok.json';
var url_set_secAaaLoginDel = '../data/status_ok.json';
var url_get_secAaaLoginAuth = '../data/sec_aaa_loginAuth.json';
var url_set_secAaaLoginAuth = '../data/status_ok.json';
<!--#endif-->
/* Management VLAN */
var url_get_secMgmtVLAN = '../data/sec_mgmt_vlan.json';
var url_set_secMgmtVLAN = '../data/status_ok.json';
var url_get_secMgmtMethod = '../data/sec_mgmt_method.json';
var url_set_secMgmtMethod = '../data/status_ok.json';
<!--#ifdef CONFIG_SYS_MGMT_ACL-->
/* Management ACL */
var url_get_secMgmtACL = '../data/sec_mgmt_acl.json';
var url_set_secMgmtACL = '../data/status_ok.json';
var url_set_secMgmtACLDel = '../data/status_ok.json';
var url_set_secMgmtACLActive = '../data/status_ok.json';
var url_set_secMgmtACLDeactive = '../data/status_ok.json';
var url_get_secMgmtACE = '../data/sec_mgmt_aclAce.json';
var url_set_secMgmtACE = '../data/status_ok.json';
var url_set_secMgmtACEEdit = '../data/status_ok.json';
var url_set_secMgmtACEDel = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PROTO_AUTHMGR-->
var url_get_authmgrProp = '../data/sec_authmgr_prop.json';
var url_set_authmgrProp = '../data/status_ok.json';
var url_get_authmgrPropEdit = '../data/sec_authmgr_propEdit.json';
var url_set_authmgrPropEdit = '../data/status_ok.json';
var url_get_authmgrPort = '../data/sec_authmgr_port.json';
var url_set_authmgrPort = '../data/status_ok.json';
var url_get_authmgrPortEdit = '../data/sec_authmgr_portEdit.json';
var url_set_authmgrPortEdit = '../data/status_ok.json';
var url_get_authmgrMacAcct = '../data/sec_authmgr_macAcct.json';
var url_get_authmgrMacAcctAdd = '../data/sec_authmgr_macAcctAdd.json';
var url_set_authmgrMacAcctAdd = '../data/status_ok.json';
var url_get_authmgrMacAcctEdit = '../data/sec_authmgr_macAcctEdit.json';
var url_set_authmgrMacAcctEdit = '../data/status_ok.json';
var url_set_authmgrMacAcctDel = '../data/status_ok.json';
var url_get_authmgrWebAcct = '../data/sec_authmgr_webAcct.json';
var url_get_authmgrWebAcctAdd = '../data/sec_authmgr_webAcctAdd.json';
var url_set_authmgrWebAcctAdd = '../data/status_ok.json';
var url_get_authmgrWebAcctEdit = '../data/sec_authmgr_webAcctEdit.json';
var url_set_authmgrWebAcctEdit = '../data/status_ok.json';
var url_set_authmgrWebAcctDel = '../data/status_ok.json';
var url_get_authmgrSess = '../data/sec_authmgr_sess.json';
var url_set_authmgrSessClear = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PORT_SECURITY-->
var url_get_secPort = '../data/sec_port.json';
var url_set_secPort = '../data/status_ok.json';
var url_get_secPortEdit = '../data/sec_portEdit.json';
var url_set_secPortEdit = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PORT_ISOLATION-->
var url_get_secProtect = '../data/sec_protect.json';
var url_get_secProtectEdit = '../data/sec_protectEdit.json';
var url_set_secProtectEdit = '../data/status_ok.json';
<!--#endif-->
/* Strom Control */
var url_get_secStormControl = '../data/sec_stormControl.json';
var url_set_secStormControl = '../data/status_ok.json';
var url_get_secStormControlEdit = '../data/sec_stormControlEdit.json';
var url_set_secStormControlEdit = '../data/status_ok.json';
<!--#ifdef CONFIG_SYS_PROTO_ISG-->
/* IPSG */
var url_get_secIpsgProp = '../data/sec_ipsg_prop.json';
var url_set_secIpsgProp = '../data/status_ok.json';
var url_get_secIpsgPortEdit = '../data/sec_ipsg_portEdit.json';
var url_set_secIpsgPortEdit = '../data/status_ok.json';
var url_get_secIpsgBinding = '../data/sec_ipsg_binding.json';
var url_set_secIpsgBinding = '../data/status_ok.json';
var url_set_secIpsgBindingDel = '../data/status_ok.json';
var url_get_secIpsgDatabase = '../data/sec_ipsg_database.json';
var url_set_secIpsgDatabase = '../data/status_ok.json';
<!--#endif-->
/* DoS */
<!--#ifdef CONFIG_SYS_DOS_PROTECTION-->
var url_get_secDosprop = '../data/sec_dos_prop.json';
var url_set_secDosprop = '../data/status_ok.json';
var url_get_secDosPort = '../data/sec_dos_port.json';
var url_get_secDosPortEdit = '../data/sec_dos_portEdit.json';
var url_set_secDosPortEdit = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PROTO_DAI-->
/* DAI */
var url_get_secDaiProp = '../data/sec_dai_prop.json';
var url_set_secDaiProp = '../data/status_ok.json';
var url_get_secDaiPortEdit = '../data/sec_dai_portEdit.json';
var url_set_secDaiPortEdit = '../data/status_ok.json';
var url_get_secDaiStatistics = '../data/sec_dai_statistics.json';
var url_set_secDaiStatistics = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING-->
/* DHCP Snooping */
var url_get_secDsProp = '../data/sec_ds_prop.json';
var url_set_secDsProp = '../data/status_ok.json';
var url_get_secDsPortEdit = '../data/sec_ds_portEdit.json';
var url_set_secDsPortEdit = '../data/status_ok.json';
var url_get_secDsStatistics = '../data/sec_ds_statistics.json';
var url_set_secDsStatistics = '../data/status_ok.json';
var url_get_secDsOption82Prop = '../data/sec_ds_option82Prop.json';
var url_set_secDsOption82Prop = '../data/status_ok.json';
var url_get_secDsOption82PortEdit = '../data/sec_ds_option82PortEdit.json';
var url_set_secDsOption82PortEdit = '../data/status_ok.json';
var url_get_secDsOption82CID = '../data/sec_ds_option82CID.json';
var url_set_secDsOption82CID = '../data/status_ok.json';
var url_set_secDsOption82CIDDel = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_USER_DEFINED_ACL-->
/* ACL */
var url_get_aclACL = '../data/acl_acl.json';
var url_set_aclACL = '../data/status_ok.json';
var url_set_aclACLDel = '../data/status_ok.json';
var url_get_aclMacACE = '../data/acl_mac_ace.json';
var url_get_aclIpv4ACE = '../data/acl_ipv4_ace.json';
var url_get_aclIpv6ACE = '../data/acl_ipv6_ace.json';
var url_set_aclACE = '../data/status_ok.json';
var url_set_aclACEDel = '../data/status_ok.json';
var url_get_aclBinding = '../data/acl_binding.json';
var url_set_aclBinding = '../data/status_ok.json';
var url_set_aclBindingDel = '../data/status_ok.json';

<!--#endif-->

/* QoS */
var url_get_qosProp = '../data/qos_prop.json';
var url_set_qosProp = '../data/status_ok.json';
var url_get_qosPortEdit = '../data/qos_portEdit.json';
var url_set_qosPortEdit = '../data/status_ok.json';
var url_get_qosQueue = '../data/qos_queue.json';
var url_set_qosQueue = '../data/status_ok.json';
var url_get_qosMapCos = '../data/qos_mapCos.json';
var url_set_qosMapCos = '../data/status_ok.json';
var url_get_qosMapDscp = '../data/qos_mapDscp.json';
var url_set_qosMapDscp = '../data/status_ok.json';
var url_get_qosMapIpp = '../data/qos_mapIpp.json';
var url_set_qosMapIpp = '../data/status_ok.json';
var url_get_qosRateCtrl = '../data/qos_rate_ctrl.json';
var url_get_qosRateCtrlEdit = '../data/qos_rate_ctrlEdit.json';
var url_set_qosRateCtrlEdit = '../data/status_ok.json';
var url_get_qosRateEgressQue = '../data/qos_rate_egressQue.json';
var url_get_qosRateEgressQueEdit = '../data/qos_rate_egressQueEdit.json';
var url_set_qosRateEgressQueEdit = '../data/status_ok.json';

/* Diagnostics */
var url_get_diagCopper = '../data/diag_copper.json';
var url_set_diagCopper = '../data/status_ok.json';
var url_get_diagLogGlobal = '../data/diag_log_global.json';
var url_set_diagLogGlobal = '../data/status_ok.json';
var url_get_diagLogRemote = '../data/diag_log_remote.json';
var url_set_diagLogRemote = '../data/status_ok.json';
var url_get_diagLogRemoteAdd = '../data/diag_log_remoteAdd.json';
var url_set_diagLogRemoteAdd = '../data/status_ok.json';
var url_get_diagLogRemoteEdit = '../data/diag_log_remoteEdit.json';
var url_set_diagLogRemoteEdit = '../data/status_ok.json';
var url_set_diagLogRemoteDel = '../data/status_ok.json';
<!--#ifdef CONFIG_SYS_MIRROR-->
var url_get_diagMirror = '../data/diag_mirror.json';
var url_get_diagMirrorEdit = '../data/diag_mirrorEdit.json';
var url_set_diagMirrorEdit = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_OPTICALMOD_STATUS-->
var url_get_diagOptical = '../data/diag_optical.json';
var url_get_diagOpticalIntf = '../data/diag_opticalIntf.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PING_TEST-->
var url_get_diagPing = '../data/diag_ping.json';
var url_set_diagPing = '../data/status_ok.json';
var url_set_diagReboot = '../data/status_ok.json';
var url_set_diagRestore = '../data/status_ok.json';
var url_set_diagPingAbort = '../data/status_ok.json';
var url_get_diagPingResult = '../data/diag_pingResult.json';
var url_get_diagPingRunning = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_TECH_SUPPORT-->
var url_get_diagTechGenStatus = '../data/status_ok.json';
var url_set_diagTechGen = '../data/status_ok.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_TRACE_ROUTE-->
var url_get_diagTraceroute = '../data/diag_traceroute.json';
var url_set_diagTraceroute = '../data/status_ok.json';
var url_set_diagTracerouteAbort = '../data/status_ok.json';
var url_get_diagTracerouteResult = '../data/diag_tracerouteResult.json';
var url_get_diagTracerouteRunning = '../data/diag_tracerouteRunning.json';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PROTO_UDLD-->
var url_get_diagUDLDProp = '../data/diag_udld_prop.json';
var url_set_diagUDLDProp = '../data/status_ok.json';
var url_get_diagUDLDPortEdit = '../data/diag_udld_portEdit.json';
var url_set_diagUDLDPortEdit = '../data/status_ok.json';
var url_get_diagUDLDNeighbor = '../data/diag_udld_neighbor.json';
<!--#endif-->

/* Management */
var url_get_mgmtImg = '../data/mgmt_img_action.json';
var url_set_mgmtBackup = '../data/status_ok.json';
var url_get_mgmtBackupStatus = '../data/status_ok.json';
var url_get_mgmtCfg = '../data/mgmt_cfg_action.json';
var url_set_mgmtUpgrade = '../data/status_ok.json';
var url_get_mgmtUpgradeStatus = '../data/status_ok.json';
var url_get_mgmtActImg = '../data/mgmt_img_act.json';
var url_set_mgmtActImg = '../data/status_ok.json';
var url_get_mgmtSavecfg = '../data/mgmt_cfg_save.json';
var url_set_mgmtSavecfg = '../data/status_ok.json';
var url_get_mgmtSavecfgStatus = '../data/status_ok.json';
var url_get_mgmtAcct = '../data/mgmt_acct.json';
var url_get_mgmtAcctAdd = '../data/mgmt_acctAdd.json';
var url_set_mgmtAcctAdd = '../data/status_ok.json';
var url_get_mgmtAcctEdit = '../data/mgmt_acctEdit.json';
var url_set_mgmtAcctEdit = '../data/status_ok.json';
var url_set_mgmtAcctDel = '../data/status_ok.json';

<!--#ifdef CONFIG_SYS_UI_SNMP-->
<!--#ifdef CONFIG_SYS_SNMPV3-->
var url_get_snmpView = '../data/mgmt_snmp_view.json';
var url_get_snmpViewAdd = '../data/mgmt_snmp_viewAdd.json';
var url_set_snmpViewAdd = '../data/status_ok.json';
var url_set_snmpViewDel = '../data/status_ok.json';
var url_get_snmpGroup = '../data/mgmt_snmp_group.json';
var url_get_snmpGroupAdd = '../data/mgmt_snmp_groupAdd.json';
var url_set_snmpGroupAdd = '../data/status_ok.json';
var url_get_snmpGroupEdit = '../data/mgmt_snmp_groupEdit.json';
var url_set_snmpGroupEdit = '../data/status_ok.json';
var url_set_snmpGroupDel = '../data/status_ok.json';
var url_get_snmpCommunity = '../data/mgmt_snmp_community.json';
var url_get_snmpCommunityAdd = '../data/mgmt_snmp_communityAdd.json';
var url_set_snmpCommunityAdd = '../data/status_ok.json';
var url_get_snmpCommunityEdit = '../data/mgmt_snmp_communityEdit.json';
var url_set_snmpCommunityEdit = '../data/status_ok.json';
var url_set_snmpCommunityDel = '../data/status_ok.json';
var url_get_snmpUser = '../data/mgmt_snmp_user.json';
var url_get_snmpUserAdd = '../data/mgmt_snmp_userAdd.json';
var url_set_snmpUserAdd = '../data/status_ok.json';
var url_get_snmpUserEdit = '../data/mgmt_snmp_userEdit.json';
var url_set_snmpUserEdit = '../data/status_ok.json';
var url_set_snmpUserDel = '../data/status_ok.json';
var url_get_snmpEngineID = '../data/mgmt_snmp_engineID.json';
var url_get_snmpEngineIDAdd = '../data/mgmt_snmp_engineIDAdd.json';
var url_set_snmpEngineIDAdd = '../data/status_ok.json';
var url_get_snmpEngineIDEdit = '../data/mgmt_snmp_engineIDEdit.json';
var url_set_snmpEngineIDEdit = '../data/status_ok.json';
var url_set_snmpEngineIDDel = '../data/status_ok.json';
var url_get_snmpTrapEvent = '../data/mgmt_snmp_trapEvent.json';
var url_set_snmpTrapEvent = '../data/status_ok.json';
var url_get_snmpNotify =    '../data/mgmt_snmp_notify.json';
var url_get_snmpNotifyAdd = '../data/mgmt_snmp_notifyAdd.json';
var url_set_snmpNotifyAdd = '../data/status_ok.json';
var url_get_snmpNotifyEdit = '../data/mgmt_snmp_notifyEdit.json';
var url_set_snmpNotifyEdit = '../data/status_ok.json';
var url_set_snmpNotifyDel = '../data/status_ok.json';
<!--#else-->
var url_get_snmpv2Community = '../data/mgmt_snmpv2_community.json';
var url_get_snmpv2CommunityAdd = '../data/mgmt_snmpv2_communityAdd.json';
var url_set_snmpv2CommunityAdd = '../data/status_ok.json';
var url_set_snmpv2CommunityDel = '../data/status_ok.json';
var url_get_snmpv2TrapEvent = '../data/mgmt_snmpv2_trapEvent.json';
var url_set_snmpv2TrapEvent = '../data/status_ok.json';
var url_get_snmpv2Notify =    '../data/mgmt_snmpv2_notify.json';
var url_get_snmpv2NotifyAdd = '../data/mgmt_snmpv2_notifyAdd.json';
var url_set_snmpv2NotifyAdd = '../data/status_ok.json';
var url_set_snmpv2NotifyDel = '../data/status_ok.json';
<!--#endif-->
<!--#endif-->

<!--#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI-->
var url_get_mgmtRmonStats = '../data/mgmt_rmon_statistics.json';
var url_set_mgmtRmonStatsClear = '../data/status_ok.json';
var url_get_mgmtRmonStatsIntf = '../data/mgmt_rmon_statisticsIntf.json';
var url_get_mgmtRmonHistory = '../data/mgmt_rmon_history.json';
var url_get_mgmtRmonHistoryAdd = '../data/mgmt_rmon_historyAdd.json';
var url_set_mgmtRmonHistoryAdd = '../data/status_ok';
var url_get_mgmtRmonHistoryEdit = '../data/mgmt_rmon_historyEdit.json';
var url_set_mgmtRmonHistoryEdit = '../data/status_ok';
var url_set_mgmtRmonHistoryDel = '../data/status_ok.json';
var url_get_mgmtRmonHistoryLog = '../data/mgmt_rmon_historyLog.json';
var url_get_mgmtRmonEvent = '../data/mgmt_rmon_event.json';
var url_get_mgmtRmonEventAdd = '../data/mgmt_rmon_eventAdd.json';
var url_set_mgmtRmonEventAdd = '../data/status_ok';
var url_get_mgmtRmonEventEdit = '../data/mgmt_rmon_eventEdit.json';
var url_set_mgmtRmonEventEdit = '../data/status_ok';
var url_set_mgmtRmonEventDel = '../data/status_ok.json';
var url_get_mgmtRmonEventLog = '../data/mgmt_rmon_eventLog.json';
var url_get_mgmtRmonAlarm = '../data/mgmt_rmon_alarm.json';
var url_get_mgmtRmonAlarmAdd = '../data/mgmt_rmon_alarmAdd.json';
var url_set_mgmtRmonAlarmAdd = '../data/status_ok';
var url_get_mgmtRmonAlarmEdit = '../data/mgmt_rmon_alarmEdit.json';
var url_set_mgmtRmonAlarmEdit = '../data/status_ok';
var url_set_mgmtRmonAlarmDel = '../data/status_ok.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_POE-->
/* POE */
var url_get_poePort = '../data/poe_port.json';
var url_get_poePortEdit = '../data/poe_portEdit.json';
var url_set_poePortEdit = '../data/status_ok.json';
var url_get_poePortTimer = '../data/poe_port_timer.json';
<!--#endif-->

<!--#ifdef CONFIG_SYS_DHCP_SERVER-->
/* DHCP */
var url_get_dhcpRelayProp = '../data/dhcpserver_prop.json';
var url_set_dhcpRelayProp = '../data/status_ok.json';
var url_get_dhcpRelayipPoolEdit = '../data/ip_pool_add.json';

<!--#endif-->
/* This ready flag should put at file end */
var ready_js_url = 1;

