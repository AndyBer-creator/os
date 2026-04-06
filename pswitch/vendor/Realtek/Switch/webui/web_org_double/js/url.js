/* Home */
var url_get_mainPage = 'cgi/get.cgi?cmd=home_main';
var url_get_loginInfo = 'cgi/get.cgi?cmd=home_login';
var url_set_loginAuth = 'cgi/set.cgi?cmd=home_loginAuth';
var url_get_loginStatus = 'cgi/get.cgi?cmd=home_loginStatus';
var url_set_save = 'cgi/set.cgi?cmd=home_save';
var url_set_logout = 'cgi/set.cgi?cmd=home_logout';
var url_get_monitor = 'cgi/get.cgi?cmd=home_monitor';
var url_get_cgiDbg = '../cgi/get.cgi?cmd=home_debug';
var url_set_cgiDbgDel = '../cgi/set.cgi?cmd=home_debugDel';
var url_get_systemSet = '../cgi/get.cgi?cmd=home_systemSet';
var url_set_systemSet = '../cgi/set.cgi?cmd=home_systemSet';
var url_get_sysOemSet = '../cgi/get.cgi?cmd=home_sysOemSet';
var url_set_sysOemSet = '../cgi/set.cgi?cmd=home_sysOemSet';

/* Status */
var url_get_statusSysinfo = '../cgi/get.cgi?cmd=sys_sysinfo';
var url_get_statusSysinfoEdit = '../cgi/get.cgi?cmd=sys_sysinfoEdit';
var url_set_statusSysinfo = '../cgi/set.cgi?cmd=sys_sysinfo';
var url_get_statusPanelLayout = '../cgi/get.cgi?cmd=panel_layout';
var url_get_statusPanel = '../cgi/get.cgi?cmd=panel_info';
var url_get_statusCpuMem = '../cgi/get.cgi?cmd=sys_cpumem';
var url_get_statusSyslog = '../cgi/get.cgi?cmd=log_syslog';
var url_set_statusSyslogClear = '../cgi/set.cgi?cmd=log_clear';
var url_get_statusPortCnt = '../cgi/get.cgi?cmd=port_cnt';
var url_set_statusPortCntClear = '../cgi/set.cgi?cmd=port_cntClear';
var url_get_statusSysTime = '../cgi/get.cgi?cmd=sys_sysTime';
<!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
var url_get_statusPortErrorDisabled = '../cgi/get.cgi?cmd=port_errDisableStatus';
var url_set_statusPortErrorDisabled = '../cgi/set.cgi?cmd=port_errDisableStatus';
<!--#endif-->
var url_get_statusPortBwutilz = '../cgi/get.cgi?cmd=port_bwutilz';
var url_get_statusLag = '../cgi/get.cgi?cmd=lag_mgmt';
var url_get_statusMac = '../cgi/get.cgi?cmd=mac_miscStatus';
var url_set_statusMac = '../cgi/set.cgi?cmd=mac_miscStatus';

/* Network */
var url_get_networkIp = '../cgi/get.cgi?cmd=sys_ip';
var url_set_networkIp = '../cgi/set.cgi?cmd=sys_ip';
var url_get_networkIpv4 = '../cgi/get.cgi?cmd=sys_ipv4';
var url_set_networkIpv4 = '../cgi/set.cgi?cmd=sys_ipv4';
var url_get_networkIpv6 = '../cgi/get.cgi?cmd=sys_ipv6';
var url_set_networkIpv6 = '../cgi/set.cgi?cmd=sys_ipv6';
<!--#ifdef CONFIG_SYS_ADVANCE_DNS-->
var url_get_networkDns = '../cgi/get.cgi?cmd=sys_dns';
var url_set_networkDns = '../cgi/set.cgi?cmd=sys_dns';
var url_set_networkDnsAdd = '../cgi/set.cgi?cmd=sys_dnsAdd';
var url_set_networkDnsDel = '../cgi/set.cgi?cmd=sys_dnsDel';
var url_get_networkHost = '../cgi/get.cgi?cmd=sys_dnsHosts';
var url_set_networkHost = '../cgi/set.cgi?cmd=sys_dnsHosts';
var url_set_networkHostAdd = '../cgi/set.cgi?cmd=sys_dnsHostsAdd';
var url_set_networkHostDel = '../cgi/set.cgi?cmd=sys_dnsHostsDel';
<!--#endif-->
var url_get_networkTime = '../cgi/get.cgi?cmd=time_time';
var url_set_networkTime = '../cgi/set.cgi?cmd=time_time';

/* Port */
var url_get_portPort = '../cgi/get.cgi?cmd=port_port';
var url_get_portPortEdit = '../cgi/get.cgi?cmd=port_portEdit';
var url_set_portPortEdit = '../cgi/set.cgi?cmd=port_portEdit';
<!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
var url_get_portErrDisable = '../cgi/get.cgi?cmd=port_errDisable';
var url_set_portErrDisable = '../cgi/set.cgi?cmd=port_errDisable';
<!--#endif-->
var url_get_portJumbo = '../cgi/get.cgi?cmd=port_jumbo';
var url_set_portJumbo = '../cgi/set.cgi?cmd=port_jumbo';
var url_get_portEEE = '../cgi/get.cgi?cmd=port_eee';
var url_get_portEEEEdit = '../cgi/get.cgi?cmd=port_eeeEdit';
var url_set_portEEEEdit = '../cgi/set.cgi?cmd=port_eeeEdit';
var url_get_portLAGMgmt = '../cgi/get.cgi?cmd=lag_mgmt';
var url_set_portLAGMgmt = '../cgi/set.cgi?cmd=lag_mgmt';
var url_get_portLAGMgmtEdit = '../cgi/get.cgi?cmd=lag_mgmtEdit';
var url_set_portLAGMgmtEdit = '../cgi/set.cgi?cmd=lag_mgmtEdit';
var url_get_portLAGPort = '../cgi/get.cgi?cmd=lag_port';
var url_get_portLAGPortEdit = '../cgi/get.cgi?cmd=lag_portEdit';
var url_set_portLAGPortEdit = '../cgi/set.cgi?cmd=lag_portEdit';
var url_get_portLAGLacp = '../cgi/get.cgi?cmd=lag_lacp';
var url_set_portLAGLacp = '../cgi/set.cgi?cmd=lag_lacp';
var url_get_portLAGLacpEdit = '../cgi/get.cgi?cmd=lag_lacpEdit';
var url_set_portLAGLacpEdit = '../cgi/set.cgi?cmd=lag_lacpEdit';

/* VLAN */
var url_get_vlanCreate = '../cgi/get.cgi?cmd=vlan_create';
var url_set_vlanCreate = '../cgi/set.cgi?cmd=vlan_create';
var url_set_vlanEdit = '../cgi/set.cgi?cmd=vlan_edit';
var url_set_vlanDel = '../cgi/set.cgi?cmd=vlan_del';
var url_get_vlanConf = '../cgi/get.cgi?cmd=vlan_conf';
var url_set_vlanConf = '../cgi/set.cgi?cmd=vlan_conf';
var url_get_vlanPort = '../cgi/get.cgi?cmd=vlan_port';
var url_get_vlanPortEdit = '../cgi/get.cgi?cmd=vlan_portEdit';
var url_set_vlanPortEdit = '../cgi/set.cgi?cmd=vlan_portEdit';
var url_get_vlanMembership = '../cgi/get.cgi?cmd=vlan_membership';
var url_get_vlanMembershipEdit = '../cgi/get.cgi?cmd=vlan_membershipEdit';
var url_set_vlanMembershipEdit = '../cgi/set.cgi?cmd=vlan_membershipEdit';

<!--#ifdef CONFIG_SYS_MACVLAN-->
/* Mac VLAN */
var url_get_vlanMacGroup = '../cgi/get.cgi?cmd=vlanmac_group';
var url_set_vlanMacGroup = '../cgi/set.cgi?cmd=vlanmac_group';
var url_add_vlanMacGroup = '../cgi/set.cgi?cmd=vlanmac_groupAdd';
var url_set_vlanMacGroupDel = '../cgi/set.cgi?cmd=vlanmac_groupDel';
var url_get_vlanMacBinding = '../cgi/get.cgi?cmd=vlanmac_binding';
var url_set_vlanMacBinding = '../cgi/set.cgi?cmd=vlanmac_binding';
var url_set_vlanMacBindingDel = '../cgi/set.cgi?cmd=vlanmac_bindingDel';
<!--#endif-->

<!--#ifdef CONFIG_SYS_VOICEVLAN-->
/* Voice VLAN */
var url_get_vlanVoiceProp = '../cgi/get.cgi?cmd=vlanvoice_prop';
var url_set_vlanVoiceProp = '../cgi/set.cgi?cmd=vlanvoice_prop';
var url_get_vlanVoicePortEdit = '../cgi/get.cgi?cmd=vlanvoice_portEdit';
var url_set_vlanVoicePortEdit = '../cgi/set.cgi?cmd=vlanvoice_portEdit';
var url_get_vlanVoiceOui = '../cgi/get.cgi?cmd=vlanvoice_oui';
var url_set_vlanVoiceOui = '../cgi/set.cgi?cmd=vlanvoice_oui';
var url_set_vlanVoiceOuiDel = '../cgi/set.cgi?cmd=vlanvoice_ouiDel';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTOCOL_VLAN-->
/* Protocol VLAN */
var url_get_vlanProtocolGroup = '../cgi/get.cgi?cmd=vlanprotocol_group';
var url_set_vlanProtocolGroup = '../cgi/set.cgi?cmd=vlanprotocol_group';
var url_set_vlanProtocolGroupDel = '../cgi/set.cgi?cmd=vlanprotocol_groupDel';
var url_get_vlanProtocolBinding = '../cgi/get.cgi?cmd=vlanprotocol_binding';
var url_set_vlanProtocolBinding = '../cgi/set.cgi?cmd=vlanprotocol_binding';
var url_set_vlanProtocolBindingDel = '../cgi/set.cgi?cmd=vlanprotocol_bindingDel';
<!--#endif-->
<!--#ifdef CONFIG_SYS_IP_SUBNET-->
/* ip subnet VLAN */
var url_get_vlanIpsubnetGroup = '../cgi/get.cgi?cmd=vlanipsubnet_group';
var url_set_vlanIpsubnetGroup = '../cgi/set.cgi?cmd=vlanipsubnet_group';
var url_set_vlanIpsubnetGroupDel = '../cgi/set.cgi?cmd=vlanipsubnet_groupDel';
var url_get_vlanIpsubnetBinding = '../cgi/get.cgi?cmd=vlanipsubnet_binding';
var url_set_vlanIpsubnetBinding = '../cgi/set.cgi?cmd=vlanipsubnet_binding';
var url_set_vlanIpsubnetBindingDel = '../cgi/set.cgi?cmd=vlanipsubnet_bindingDel';
<!--#endif-->
<!--#ifdef CONFIG_SYS_SURVEILLANCEVLAN-->
/* Surveillance VLAN */
var url_get_vlanSurveillanceProp = '../cgi/get.cgi?cmd=vlansurveillance_prop';
var url_set_vlanSurveillanceProp = '../cgi/set.cgi?cmd=vlansurveillance_prop';
var url_get_vlanSurveillancePortEdit = '../cgi/get.cgi?cmd=vlansurveillance_portEdit';
var url_set_vlanSurveillancePortEdit = '../cgi/set.cgi?cmd=vlansurveillance_portEdit';
var url_get_vlanSurveillanceOui = '../cgi/get.cgi?cmd=vlansurveillance_oui';
var url_set_vlanSurveillanceOui = '../cgi/set.cgi?cmd=vlansurveillance_oui';
var url_set_vlanSurveillanceOuiDel = '../cgi/set.cgi?cmd=vlansurveillance_ouiDel';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_GVRP-->
var url_get_gvrpProp = '../cgi/get.cgi?cmd=gvrp_prop';
var url_set_gvrpProp = '../cgi/set.cgi?cmd=gvrp_prop';
var url_get_gvrpPropEdit = '../cgi/get.cgi?cmd=gvrp_propEdit';
var url_set_gvrpPropEdit = '../cgi/set.cgi?cmd=gvrp_propEdit';
var url_get_gvrpMember = '../cgi/get.cgi?cmd=gvrp_member';
var url_get_gvrpPortCnt = '../cgi/get.cgi?cmd=gvrp_portCnt';
var url_set_gvrpPortCntClear = '../cgi/set.cgi?cmd=gvrp_portClr';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_GARP-->
var url_get_gmrpProp = '../cgi/get.cgi?cmd=gmrp_prop';
var url_set_gmrpProp = '../cgi/set.cgi?cmd=gmrp_prop';
var url_get_gmrpPropEdit = '../cgi/get.cgi?cmd=gmrp_propEdit';
var url_set_gmrpPropEdit = '../cgi/set.cgi?cmd=gmrp_propEdit';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING-->
/* Multicast */
var url_get_mcastProp = '../cgi/get.cgi?cmd=mcast_genProp';
var url_set_mcastProp = '../cgi/set.cgi?cmd=mcast_genProp';

var url_get_mcastGroup = '../cgi/get.cgi?cmd=mcast_genGroup';
var url_get_mcastGroupAdd = '../cgi/get.cgi?cmd=mcast_genGroupAdd';
var url_set_mcastGroupAdd = '../cgi/set.cgi?cmd=mcast_genGroupAdd';
var url_get_mcastGroupEdit = '../cgi/get.cgi?cmd=mcast_genGroupEdit';
var url_set_mcastGroupEdit = '../cgi/set.cgi?cmd=mcast_genGroupEdit';
var url_set_mcastGroupDel = '../cgi/set.cgi?cmd=mcast_genGroupDel';
var url_get_mcastRouter = '../cgi/get.cgi?cmd=mcast_genRouter';

<!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER-->
var url_get_mcastRouterAdd = '../cgi/get.cgi?cmd=mcast_genRouterAdd';
var url_set_mcastRouterAdd = '../cgi/set.cgi?cmd=mcast_genRouterAdd';
var url_get_mcastRouterEdit = '../cgi/get.cgi?cmd=mcast_genRouterEdit';
var url_set_mcastRouterEdit = '../cgi/set.cgi?cmd=mcast_genRouterEdit';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD-->
var url_get_mcastFwdAll = '../cgi/get.cgi?cmd=mcast_genFwdAll';
var url_get_mcastFwdAllAdd = '../cgi/get.cgi?cmd=mcast_genFwdAllAdd';
var url_set_mcastFwdAllAdd = '../cgi/set.cgi?cmd=mcast_genFwdAllAdd';
var url_get_mcastFwdAllEdit = '../cgi/get.cgi?cmd=mcast_genFwdAllEdit';
var url_set_mcastFwdAllEdit = '../cgi/set.cgi?cmd=mcast_genFwdAllEdit';
var url_set_mcastFwdAllDel = '../cgi/set.cgi?cmd=mcast_genFwdAllDel';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING-->
var url_get_mcastThrot = '../cgi/get.cgi?cmd=mcast_genThrot';
var url_get_mcastThrotEdit = '../cgi/get.cgi?cmd=mcast_genThrotEdit';
var url_set_mcastThrotEdit = '../cgi/set.cgi?cmd=mcast_genThrotEdit';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING-->
var url_get_mcastProfiles = '../cgi/get.cgi?cmd=mcast_genProfile';
var url_get_mcastProfilesAdd = '../cgi/get.cgi?cmd=mcast_genProfileAdd';
var url_set_mcastProfilesAdd = '../cgi/set.cgi?cmd=mcast_genProfileAdd';
var url_get_mcastProfilesEdit = '../cgi/get.cgi?cmd=mcast_genProfileEdit';
var url_set_mcastProfilesEdit = '../cgi/set.cgi?cmd=mcast_genProfileEdit';
var url_set_mcastProfilesDel = '../cgi/set.cgi?cmd=mcast_genProfileDel';

var url_get_mcastFilter = '../cgi/get.cgi?cmd=mcast_genFilter';
var url_get_mcastFilterEdit = '../cgi/get.cgi?cmd=mcast_genFilterEdit';
var url_set_mcastFilterEdit = '../cgi/set.cgi?cmd=mcast_genFilterEdit';
<!--#endif-->

var url_get_mcastIgmpProp = '../cgi/get.cgi?cmd=mcast_igmpProp';
var url_get_mcastIgmpPropEdit = '../cgi/get.cgi?cmd=mcast_igmpPropEdit';
var url_set_mcastIgmpProp = '../cgi/set.cgi?cmd=mcast_igmpProp';
var url_set_mcastIgmpPropEdit = '../cgi/set.cgi?cmd=mcast_igmpPropEdit';

<!--#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER-->
var url_get_mcastIgmpQuerier = '../cgi/get.cgi?cmd=mcast_igmpQuerier';
var url_get_mcastIgmpQuerierEdit = '../cgi/get.cgi?cmd=mcast_igmpQuerierEdit';
var url_set_mcastIgmpQuerierEdit = '../cgi/set.cgi?cmd=mcast_igmpQuerierEdit';
<!--endif-->
var url_get_mcastIgmpStatic = '../cgi/get.cgi?cmd=mcast_igmpStat';
var url_set_mcastIgmpStaticClear = '../cgi/set.cgi?cmd=mcast_igmpStatClear';

<!--#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING-->
var url_get_mcastMldProp = '../cgi/get.cgi?cmd=mcast_mldProp';
var url_get_mcastMldPropEdit = '../cgi/get.cgi?cmd=mcast_mldPropEdit';
var url_set_mcastMldProp = '../cgi/set.cgi?cmd=mcast_mldProp';
var url_set_mcastMldPropEdit = '../cgi/set.cgi?cmd=mcast_mldPropEdit';
var url_get_mcastMldStatic = '../cgi/get.cgi?cmd=mcast_mldStat';
var url_set_mcastMldStaticClear = '../cgi/set.cgi?cmd=mcast_mldStatClear';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_MVR-->
var url_get_mvrProp = '../cgi/get.cgi?cmd=mcast_mvrProp';
var url_set_mvrProp = '../cgi/set.cgi?cmd=mcast_mvrProp';
var url_get_mvrGroup = '../cgi/get.cgi?cmd=mcast_mvrGroup';
var url_get_mvrGroupAdd = '../cgi/get.cgi?cmd=mcast_mvrGroupAdd';
var url_set_mvrGroupAdd = '../cgi/set.cgi?cmd=mcast_mvrGroupAdd';
var url_get_mvrGroupEdit = '../cgi/get.cgi?cmd=mcast_mvrGroupEdit';
var url_set_mvrGroupEdit = '../cgi/set.cgi?cmd=mcast_mvrGroupEdit';
var url_set_mvrGroupDel = '../cgi/set.cgi?cmd=mcast_mvrGroupDel';
var url_get_mvrPort = '../cgi/get.cgi?cmd=mcast_mvrPort';
var url_get_mvrPortEdit = '../cgi/get.cgi?cmd=mcast_mvrPortEdit';
var url_set_mvrPortEdit = '../cgi/set.cgi?cmd=mcast_mvrPortEdit';
<!--#endif-->
<!--#endif-->

/* MAC Address Table */
var url_get_macDynamic = '../cgi/get.cgi?cmd=mac_dynamic';
var url_set_macDynamicAging = '../cgi/set.cgi?cmd=mac_dynamicAging';
var url_set_macDynamicToStatic = '../cgi/set.cgi?cmd=mac_dynamicToStatic';
var url_get_macStatic = '../cgi/get.cgi?cmd=mac_static';
var url_set_macStaticAdd = '../cgi/set.cgi?cmd=mac_staticAdd';
var url_set_macStaticEdit = '../cgi/set.cgi?cmd=mac_staticEdit';
var url_set_macStaticDel = '../cgi/set.cgi?cmd=mac_staticDelete';
<!--#ifdef CONFIG_SYS_BLACK_HOLE_MAC-->
var url_get_macFilter = '../cgi/get.cgi?cmd=mac_filter';
var url_set_macFilterAdd = '../cgi/set.cgi?cmd=mac_filterAdd';
var url_set_macFilterEdit = '../cgi/set.cgi?cmd=mac_filterEdit';
var url_set_macFilterDel = '../cgi/set.cgi?cmd=mac_filterDelete';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PORT_SECURITY-->
var url_get_macPsecure = '../cgi/get.cgi?cmd=psecure_mac';
var url_set_macPsecureAdd = '../cgi/set.cgi?cmd=psecure_macAdd';
var url_set_macPsecureDel = '../cgi/set.cgi?cmd=psecure_macDel';
var url_set_macPsecureEdit = '../cgi/set.cgi?cmd=psecure_macEdit';
<!--#endif-->

/* Spanning Tree */
<!--#ifdef CONFIG_SYS_PROTO_STP-->
var url_get_stpGlobal = '../cgi/get.cgi?cmd=stp_global';
var url_set_stpGlobal = '../cgi/set.cgi?cmd=stp_global';
var url_get_stpIntf = '../cgi/get.cgi?cmd=stp_intf';
var url_get_stpIntfEdit = '../cgi/get.cgi?cmd=stp_intfEdit';
var url_set_stpIntfEdit = '../cgi/set.cgi?cmd=stp_intfEdit';
var url_set_stpIntfMigrate = '../cgi/set.cgi?cmd=stp_intfMigrate';
<!--#ifdef CONFIG_SYS_PROTO_STP_MSTP-->
var url_get_stpMsti = '../cgi/get.cgi?cmd=stp_msti';
var url_get_stpMstiEdit = '../cgi/get.cgi?cmd=stp_mstiEdit';
var url_set_stpMstiEdit = '../cgi/set.cgi?cmd=stp_mstiEdit';
var url_get_stpMstiIntf = '../cgi/get.cgi?cmd=stp_mstiIntf';
var url_set_stpMstiIntf = '../cgi/set.cgi?cmd=stp_mstiIntf';
var url_get_stpMstiIntfEdit = '../cgi/get.cgi?cmd=stp_mstiIntfEdit';
var url_set_stpMstiIntfEdit = '../cgi/set.cgi?cmd=stp_mstiIntfEdit';
<!--#endif-->
var url_get_stpStats = '../cgi/get.cgi?cmd=stp_statistics';
var url_get_stpStatsIntf = '../cgi/get.cgi?cmd=stp_statisticsIntf';
var url_set_stpStatsIntfClear = '../cgi/set.cgi?cmd=stp_statisticsIntfClear';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_LLDP-->
/* LLDP */
var url_get_discoveryLldpProp = '../cgi/get.cgi?cmd=lldp_prop';
var url_set_discoveryLldpProp = '../cgi/set.cgi?cmd=lldp_prop';
var url_get_discoveryLldpPort = '../cgi/get.cgi?cmd=lldp_port';
var url_get_discoveryLldpPortEdit = '../cgi/get.cgi?cmd=lldp_portEdit';
var url_set_discoveryLldpPortEdit = '../cgi/set.cgi?cmd=lldp_portEdit';
<!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
var url_get_discoveryLldpMedPolicy = '../cgi/get.cgi?cmd=lldp_medPolicy';
var url_get_discoveryLldpMedPolicyAdd = '../cgi/get.cgi?cmd=lldp_medPolicyAdd';
var url_set_discoveryLldpMedPolicyAdd = '../cgi/set.cgi?cmd=lldp_medPolicyAdd';
var url_get_discoveryLldpMedPolicyEdit = '../cgi/get.cgi?cmd=lldp_medPolicyEdit';
var url_set_discoveryLldpMedPolicyEdit = '../cgi/set.cgi?cmd=lldp_medPolicyEdit';
var url_set_discoveryLldpMedPolicyDel = '../cgi/set.cgi?cmd=lldp_medPolicyDel';
var url_get_discoveryLldpMedPort = '../cgi/get.cgi?cmd=lldp_medPort';
var url_get_discoveryLldpMedPortEdit = '../cgi/get.cgi?cmd=lldp_medPortEdit';
var url_set_discoveryLldpMedPortEdit = '../cgi/set.cgi?cmd=lldp_medPortEdit';
<!--#endif-->
var url_get_discoveryLldpLocal = '../cgi/get.cgi?cmd=lldp_local';
var url_get_discoveryLldpLocalDetail = '../cgi/get.cgi?cmd=lldp_localDetail';
var url_get_discoveryLldpNeighbor = '../cgi/get.cgi?cmd=lldp_neighbor';
var url_set_discoveryLldpNeighborClear = '../cgi/set.cgi?cmd=lldp_neighborClear';
var url_set_discoveryLldpNeighborRefresh = '../cgi/set.cgi?cmd=home_refresh';
var url_get_discoveryLldpNeighborDetail = '../cgi/get.cgi?cmd=lldp_neighborDetail';
var url_get_discoveryLldpStatistic = '../cgi/get.cgi?cmd=lldp_statistic';
var url_set_discoveryLldpStatisticGlobalClear = '../cgi/set.cgi?cmd=lldp_globalStatisticsClear';
var url_set_discoveryLldpStatisticPortClear = '../cgi/set.cgi?cmd=lldp_portStatisticsClear';
var url_set_discoveryLldpStatisticRefresh = '../cgi/set.cgi?cmd=home_refresh';
var url_get_discoveryLldpOverload = '../cgi/get.cgi?cmd=lldp_overload';
var url_set_discoveryLldpOverloadRefresh = '../cgi/set.cgi?cmd=home_refresh';
var url_get_discoveryLldpOverloadDetail = '../cgi/get.cgi?cmd=lldp_overloadDetail';
<!--#endif-->

<!--#ifdef CONFIG_SYS_L3_ROUTE-->
/* IP Configuration */
var url_get_ipconfIpv4Intf = '../cgi/get.cgi?cmd=l3_v4Intf';
var url_get_ipconfIpv4IntfAdd = '../cgi/get.cgi?cmd=l3_v4IntfAdd';
var url_set_ipconfIpv4IntfAdd = '../cgi/set.cgi?cmd=l3_v4IntfAdd';
var url_get_ipconfIpv4IntfEdit = '../cgi/get.cgi?cmd=l3_v4IntfEdit';
var url_set_ipconfIpv4IntfEdit = '../cgi/set.cgi?cmd=l3_v4IntfEdit';
var url_set_ipconfIpv4IntfDel = '../cgi/set.cgi?cmd=l3_v4IntfDel';
var url_get_ipconfIpv4Route = '../cgi/get.cgi?cmd=l3_v4Route';
var url_get_ipconfIpv4RouteAdd = '../cgi/get.cgi?cmd=l3_v4RouteAdd';
var url_set_ipconfIpv4RouteAdd = '../cgi/set.cgi?cmd=l3_v4RouteAdd';
var url_get_ipconfIpv4RouteEdit = '../cgi/get.cgi?cmd=l3_v4RouteEdit';
var url_set_ipconfIpv4RouteEdit = '../cgi/set.cgi?cmd=l3_v4RouteEdit';
var url_set_ipconfIpv4RouteDel = '../cgi/set.cgi?cmd=l3_v4RouteDel';

var url_get_ipconfIpv6Intf = '../cgi/get.cgi?cmd=l3_v6Intf';
var url_set_ipconfIpv6Intf = '../cgi/set.cgi?cmd=l3_v6Intf';
var url_get_ipconfIpv6IntfAdd = '../cgi/get.cgi?cmd=l3_v6IntfAdd';
var url_set_ipconfIpv6IntfAdd = '../cgi/set.cgi?cmd=l3_v6IntfAdd';
var url_get_ipconfIpv6IntfEdit = '../cgi/get.cgi?cmd=l3_v6IntfEdit';
var url_set_ipconfIpv6IntfEdit = '../cgi/set.cgi?cmd=l3_v6IntfEdit';
var url_set_ipconfIpv6IntfDel = '../cgi/set.cgi?cmd=l3_v6IntfDel';
var url_get_ipconfIpv6Addr = '../cgi/get.cgi?cmd=l3_v6Addr';
var url_get_ipconfIpv6AddrAdd = '../cgi/get.cgi?cmd=l3_v6AddrAdd';
var url_set_ipconfIpv6AddrAdd = '../cgi/set.cgi?cmd=l3_v6AddrAdd';
var url_set_ipconfIpv6AddrDel = '../cgi/set.cgi?cmd=l3_v6AddrDel';
var url_get_ipconfIpv6Route = '../cgi/get.cgi?cmd=l3_v6Route';
var url_get_ipconfIpv6RouteAdd = '../cgi/get.cgi?cmd=l3_v6RouteAdd';
var url_set_ipconfIpv6RouteAdd = '../cgi/set.cgi?cmd=l3_v6RouteAdd';
var url_get_ipconfIpv6RouteEdit = '../cgi/get.cgi?cmd=l3_v6RouteEdit';
var url_set_ipconfIpv6RouteEdit = '../cgi/set.cgi?cmd=l3_v6RouteEdit';
var url_set_ipconfIpv6RouteDel = '../cgi/set.cgi?cmd=l3_v6RouteDel';

var url_get_ipconfIpv4Arp = '../cgi/get.cgi?cmd=l3_v4Arp';
var url_set_ipconfIpv4Arp = '../cgi/set.cgi?cmd=l3_v4Arp';
var url_get_ipconfIpv4ArpAdd = '../cgi/get.cgi?cmd=l3_v4ArpAdd';
var url_set_ipconfIpv4ArpAdd = '../cgi/set.cgi?cmd=l3_v4ArpAdd';
var url_set_ipconfIpv4ArpEdit = '../cgi/set.cgi?cmd=l3_v4ArpEdit';
var url_set_ipconfIpv4ArpDel = '../cgi/set.cgi?cmd=l3_v4ArpDel';
var url_get_ipconfIpv4ArpProxy = '../cgi/get.cgi?cmd=l3_v4ArpProxy';
var url_set_ipconfIpv4ArpProxy = '../cgi/set.cgi?cmd=l3_v4ArpProxy';

var url_get_ipconfIpv6Neigh = '../cgi/get.cgi?cmd=l3_v6Neigh';
var url_set_ipconfIpv6Neigh = '../cgi/set.cgi?cmd=l3_v6Neigh';
var url_get_ipconfIpv6NeighAdd = '../cgi/get.cgi?cmd=l3_v6NeighAdd';
var url_set_ipconfIpv6NeighAdd = '../cgi/set.cgi?cmd=l3_v6NeighAdd';
var url_set_ipconfIpv6NeighEdit = '../cgi/set.cgi?cmd=l3_v6NeighEdit';
var url_set_ipconfIpv6NeighDel = '../cgi/set.cgi?cmd=l3_v6NeighDel';
<!--#endif-->

/* Security */
<!--#ifdef CONFIG_SYS_LIB_RADIUS-->
var url_get_secRadius = '../cgi/get.cgi?cmd=radius_srv';
var url_set_secRadius = '../cgi/set.cgi?cmd=radius_srv';
var url_get_secRadiusAdd = '../cgi/get.cgi?cmd=radius_srvAdd';
var url_set_secRadiusAdd = '../cgi/set.cgi?cmd=radius_srvAdd';
var url_get_secRadiusEdit = '../cgi/get.cgi?cmd=radius_srvEdit';
var url_set_secRadiusEdit = '../cgi/set.cgi?cmd=radius_srvEdit';
var url_set_secRadiusDel = '../cgi/set.cgi?cmd=radius_srvDel';
<!--#endif-->
<!--#ifdef CONFIG_SYS_LIB_TACPLUS-->
var url_get_secTacplus = '../cgi/get.cgi?cmd=tacplus_srv';
var url_set_secTacplus = '../cgi/set.cgi?cmd=tacplus_srv';
var url_get_secTacplusAdd = '../cgi/get.cgi?cmd=tacplus_srvAdd';
var url_set_secTacplusAdd = '../cgi/set.cgi?cmd=tacplus_srvAdd';
var url_get_secTacplusEdit = '../cgi/get.cgi?cmd=tacplus_srvEdit';
var url_set_secTacplusEdit = '../cgi/set.cgi?cmd=tacplus_srvEdit';
var url_set_secTacplusDel = '../cgi/set.cgi?cmd=tacplus_srvDel';
<!--#endif-->
<!--#ifdef CONFIG_SYS_AAA-->
var url_get_secAaaLogin = '../cgi/get.cgi?cmd=aaa_login';
var url_get_secAaaLoginAdd = '../cgi/get.cgi?cmd=aaa_loginAdd';
var url_set_secAaaLoginAdd = '../cgi/set.cgi?cmd=aaa_loginAdd';
var url_get_secAaaLoginEdit = '../cgi/get.cgi?cmd=aaa_loginEdit';
var url_set_secAaaLoginEdit = '../cgi/set.cgi?cmd=aaa_loginEdit';
var url_set_secAaaLoginDel = '../cgi/set.cgi?cmd=aaa_loginDel';
var url_get_secAaaLoginAuth = '../cgi/get.cgi?cmd=aaa_loginAuth';
var url_set_secAaaLoginAuth = '../cgi/set.cgi?cmd=aaa_loginAuth';
<!--#endif-->
/* Management VLAN */
var url_get_secMgmtVLAN = '../cgi/get.cgi?cmd=vlan_mgmt';
var url_set_secMgmtVLAN = '../cgi/set.cgi?cmd=vlan_mgmt';
var url_get_secMgmtMethod = '../cgi/get.cgi?cmd=sys_line';
var url_set_secMgmtMethod = '../cgi/set.cgi?cmd=sys_line';
<!--#ifdef CONFIG_SYS_MGMT_ACL-->
/* Management ACL */
var url_get_secMgmtACL = '../cgi/get.cgi?cmd=aclMgmt_acl';
var url_set_secMgmtACL = '../cgi/set.cgi?cmd=aclMgmt_acl';
var url_set_secMgmtACLDel = '../cgi/set.cgi?cmd=aclMgmt_aclDel';
var url_set_secMgmtACLActive = '../cgi/set.cgi?cmd=aclMgmt_aclActive';
var url_set_secMgmtACLDeactive = '../cgi/set.cgi?cmd=aclMgmt_aclDeactive';
var url_get_secMgmtACE = '../cgi/get.cgi?cmd=aclMgmt_ace';
var url_set_secMgmtACE = '../cgi/set.cgi?cmd=aclMgmt_ace';
var url_set_secMgmtACEEdit = '../cgi/set.cgi?cmd=aclMgmt_aceEdit';
var url_set_secMgmtACEDel = '../cgi/set.cgi?cmd=aclMgmt_aceDel';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PROTO_AUTHMGR-->
var url_get_authmgrProp = '../cgi/get.cgi?cmd=authmgr_prop';
var url_set_authmgrProp = '../cgi/set.cgi?cmd=authmgr_prop';
var url_get_authmgrPropEdit = '../cgi/get.cgi?cmd=authmgr_propEdit';
var url_set_authmgrPropEdit = '../cgi/set.cgi?cmd=authmgr_propEdit';
var url_get_authmgrPort = '../cgi/get.cgi?cmd=authmgr_port';
var url_set_authmgrPort = '../cgi/set.cgi?cmd=authmgr_port';
var url_get_authmgrPortEdit = '../cgi/get.cgi?cmd=authmgr_portEdit';
var url_set_authmgrPortEdit = '../cgi/set.cgi?cmd=authmgr_portEdit';
var url_get_authmgrMacAcct = '../cgi/get.cgi?cmd=authmgr_macAcct';
var url_get_authmgrMacAcctAdd = '../cgi/get.cgi?cmd=authmgr_macAcctAdd';
var url_set_authmgrMacAcctAdd = '../cgi/set.cgi?cmd=authmgr_macAcctAdd';
var url_get_authmgrMacAcctEdit = '../cgi/get.cgi?cmd=authmgr_macAcctEdit';
var url_set_authmgrMacAcctEdit = '../cgi/set.cgi?cmd=authmgr_macAcctEdit';
var url_set_authmgrMacAcctDel = '../cgi/set.cgi?cmd=authmgr_macAcctDel';
var url_get_authmgrWebAcct = '../cgi/get.cgi?cmd=authmgr_webAcct';
var url_get_authmgrWebAcctAdd = '../cgi/get.cgi?cmd=authmgr_webAcctAdd';
var url_set_authmgrWebAcctAdd = '../cgi/set.cgi?cmd=authmgr_webAcctAdd';
var url_get_authmgrWebAcctEdit = '../cgi/get.cgi?cmd=authmgr_webAcctEdit';
var url_set_authmgrWebAcctEdit = '../cgi/set.cgi?cmd=authmgr_webAcctEdit';
var url_set_authmgrWebAcctDel = '../cgi/set.cgi?cmd=authmgr_webAcctDel';
var url_get_authmgrSess = '../cgi/get.cgi?cmd=authmgr_sess';
var url_set_authmgrSessClear = '../cgi/set.cgi?cmd=authmgr_sessClear';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PORT_SECURITY-->
var url_get_secPort = '../cgi/get.cgi?cmd=psecure_prop';
var url_set_secPort = '../cgi/set.cgi?cmd=psecure_prop';
var url_get_secPortEdit = '../cgi/get.cgi?cmd=psecure_portEdit';
var url_set_secPortEdit = '../cgi/set.cgi?cmd=psecure_portEdit';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PORT_ISOLATION-->
var url_get_secProtect = '../cgi/get.cgi?cmd=port_protect';
var url_get_secProtectEdit = '../cgi/get.cgi?cmd=port_protectEdit';
var url_set_secProtectEdit = '../cgi/set.cgi?cmd=port_protectEdit';
<!--#endif-->
<!--#ifdef CONFIG_SYS_STORM_CTRL-->
/* Strom Control */
var url_get_secStormControl = '../cgi/get.cgi?cmd=storm_control';
var url_set_secStormControl = '../cgi/set.cgi?cmd=storm_control';
var url_get_secStormControlEdit = '../cgi/get.cgi?cmd=storm_controlEdit';
var url_set_secStormControlEdit = '../cgi/set.cgi?cmd=storm_controlEdit';
<!--#endif-->
<!--#ifdef CONFIG_SYS_DOS_PROTECTION-->
/* DoS */
var url_get_secDosprop = '../cgi/get.cgi?cmd=dos_prop';
var url_set_secDosprop = '../cgi/set.cgi?cmd=dos_prop';
var url_get_secDosPort = '../cgi/get.cgi?cmd=dos_port';
var url_get_secDosPortEdit = '../cgi/get.cgi?cmd=dos_portEdit';
var url_set_secDosPortEdit = '../cgi/set.cgi?cmd=dos_portEdit';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PROTO_ISG-->
/* IPSG */
var url_get_secIpsgProp = '../cgi/get.cgi?cmd=ipsg_prop';
var url_set_secIpsgProp = '../cgi/set.cgi?cmd=ipsg_prop';
var url_get_secIpsgPortEdit = '../cgi/get.cgi?cmd=ipsg_portEdit';
var url_set_secIpsgPortEdit = '../cgi/set.cgi?cmd=ipsg_portEdit';
var url_get_secIpsgBinding = '../cgi/get.cgi?cmd=ipsg_binding';
var url_set_secIpsgBinding = '../cgi/set.cgi?cmd=ipsg_binding';
var url_set_secIpsgBindingDel = '../cgi/set.cgi?cmd=ipsg_bindingDel';
var url_get_secIpsgDatabase = '../cgi/get.cgi?cmd=ipsg_database';
var url_set_secIpsgDatabase = '../cgi/set.cgi?cmd=ipsg_database';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_DAI-->
/* DAI */
var url_get_secDaiProp = '../cgi/get.cgi?cmd=dai_prop';
var url_set_secDaiProp = '../cgi/set.cgi?cmd=dai_prop';
var url_get_secDaiPortEdit = '../cgi/get.cgi?cmd=dai_portEdit';
var url_set_secDaiPortEdit = '../cgi/set.cgi?cmd=dai_portEdit';
var url_get_secDaiStatistics = '../cgi/get.cgi?cmd=dai_statistics';
var url_set_secDaiStatistics = '../cgi/set.cgi?cmd=dai_statistics';
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING-->
/* DHCP Snooping */
var url_get_secDsProp = '../cgi/get.cgi?cmd=ds_prop';
var url_set_secDsProp = '../cgi/set.cgi?cmd=ds_prop';
var url_get_secDsPortEdit = '../cgi/get.cgi?cmd=ds_portEdit';
var url_set_secDsPortEdit = '../cgi/set.cgi?cmd=ds_portEdit';
var url_get_secDsStatistics = '../cgi/get.cgi?cmd=ds_statistics';
var url_set_secDsStatistics = '../cgi/set.cgi?cmd=ds_statistics';
var url_get_secDsOption82Prop = '../cgi/get.cgi?cmd=ds_option82Prop';
var url_set_secDsOption82Prop = '../cgi/set.cgi?cmd=ds_option82Prop';
var url_get_secDsOption82PortEdit = '../cgi/get.cgi?cmd=ds_option82PortEdit';
var url_set_secDsOption82PortEdit = '../cgi/set.cgi?cmd=ds_option82PortEdit';
var url_get_secDsOption82CID = '../cgi/get.cgi?cmd=ds_option82CID';
var url_set_secDsOption82CID = '../cgi/set.cgi?cmd=ds_option82CID';
var url_set_secDsOption82CIDDel = '../cgi/set.cgi?cmd=ds_option82CIDDel';
<!--#endif-->

<!--#ifdef CONFIG_SYS_USER_DEFINED_ACL-->
/* ACL */
var url_get_aclACL = '../cgi/get.cgi?cmd=acl_acl';
var url_set_aclACL = '../cgi/set.cgi?cmd=acl_acl';
var url_set_aclACLDel = '../cgi/set.cgi?cmd=acl_aclDel';
var url_get_aclMacACE = '../cgi/get.cgi?cmd=acl_ace';
var url_get_aclIpv4ACE = '../cgi/get.cgi?cmd=acl_ace';
var url_get_aclIpv6ACE = '../cgi/get.cgi?cmd=acl_ace';
var url_set_aclACE = '../cgi/set.cgi?cmd=acl_ace';
var url_set_aclACEDel = '../cgi/set.cgi?cmd=acl_aceDel';
var url_get_aclBinding = '../cgi/get.cgi?cmd=acl_binding';
var url_set_aclBinding = '../cgi/set.cgi?cmd=acl_binding';
var url_set_aclBindingDel = '../cgi/set.cgi?cmd=acl_bindingDel';
<!--#endif-->

/* QoS */
var url_get_qosProp = '../cgi/get.cgi?cmd=qos_prop';
var url_set_qosProp = '../cgi/set.cgi?cmd=qos_prop';
var url_get_qosPortEdit = '../cgi/get.cgi?cmd=qos_portEdit';
var url_set_qosPortEdit = '../cgi/set.cgi?cmd=qos_portEdit';
var url_get_qosQueue = '../cgi/get.cgi?cmd=qos_queue';
var url_set_qosQueue = '../cgi/set.cgi?cmd=qos_queue';
var url_get_qosMapCos = '../cgi/get.cgi?cmd=qos_mapCos';
var url_set_qosMapCos = '../cgi/set.cgi?cmd=qos_mapCos';
var url_get_qosMapDscp = '../cgi/get.cgi?cmd=qos_mapDscp';
var url_set_qosMapDscp = '../cgi/set.cgi?cmd=qos_mapDscp';
var url_get_qosMapIpp = '../cgi/get.cgi?cmd=qos_mapIpp';
var url_set_qosMapIpp = '../cgi/set.cgi?cmd=qos_mapIpp';
var url_get_qosRateCtrl = '../cgi/get.cgi?cmd=qos_rateCtrl';
var url_get_qosRateCtrlEdit = '../cgi/get.cgi?cmd=qos_rateCtrlEdit';
var url_set_qosRateCtrlEdit = '../cgi/set.cgi?cmd=qos_rateCtrlEdit';
var url_get_qosRateEgressQue = '../cgi/get.cgi?cmd=qos_rateEgrQue';
var url_get_qosRateEgressQueEdit = '../cgi/get.cgi?cmd=qos_rateEgrQueEdit';
var url_set_qosRateEgressQueEdit = '../cgi/set.cgi?cmd=qos_rateEgrQueEdit';

/* Diagnostics */
var url_get_diagCopper = '../cgi/get.cgi?cmd=diag_copper';
var url_set_diagCopper = '../cgi/set.cgi?cmd=diag_copper';
var url_get_diagCopperResult = '../cgi/get.cgi?cmd=diag_copperResult';
var url_set_diagReboot = '../cgi/set.cgi?cmd=sys_reboot';
var url_set_diagRestore = '../cgi/set.cgi?cmd=sys_restore';
<!--#ifdef CONFIG_SYS_TECH_SUPPORT-->
var url_get_diagTechGenStatus = '../cgi/get.cgi?cmd=diag_techGenStatus';
var url_set_diagTechGen = '../cgi/set.cgi?cmd=diag_techGen';
<!--#endif-->
var url_get_diagLogGlobal = '../cgi/get.cgi?cmd=log_global';
var url_set_diagLogGlobal = '../cgi/set.cgi?cmd=log_global';
var url_get_diagLogRemote = '../cgi/get.cgi?cmd=log_remote';
var url_get_diagLogRemoteAdd = '../cgi/get.cgi?cmd=log_remoteAdd';
var url_set_diagLogRemoteAdd = '../cgi/set.cgi?cmd=log_remoteAdd';
var url_get_diagLogRemoteEdit = '../cgi/get.cgi?cmd=log_remoteEdit';
var url_set_diagLogRemoteEdit = '../cgi/set.cgi?cmd=log_remoteEdit';
var url_set_diagLogRemoteDel = '../cgi/set.cgi?cmd=log_remoteDel';
<!--#ifdef CONFIG_SYS_PROTO_ERPS-->
var url_get_valErpsStatus = '../cgi/get.cgi?cmd=erps_erpsGet';
var url_set_valErpsStatus = '../cgi/set.cgi?cmd=erps_erpsSet';
var url_get_valErpsGlobalStatus = '../cgi/get.cgi?cmd=erps_erpsGetGlobalStatus';
var url_del_valErpsInstance = '../cgi/set.cgi?cmd=erps_erpsDelInstance';
var url_set_valErpsInstanceId = '../cgi/set.cgi?cmd=erps_erpsSetInstance';
var url_get_valErpsStatusEdit = '../cgi/get.cgi?cmd=erps_erpsGetEdit';
var url_set_valErpsStatusEdit = '../cgi/set.cgi?cmd=erps_erpsSetEdit';
<!--#endif-->

<!--#ifdef CONFIG_SYS_OPTICALMOD_STATUS-->
var url_get_diagOptical = '../cgi/get.cgi?cmd=diag_optical';
var url_get_diagOpticalIntf = '../cgi/get.cgi?cmd=diag_opticalIntf';
<!--#endif-->
<!--#ifdef CONFIG_SYS_MIRROR-->
var url_get_diagMirror = '../cgi/get.cgi?cmd=diag_mirror';
var url_get_diagMirrorEdit = '../cgi/get.cgi?cmd=diag_mirrorEdit';
var url_set_diagMirrorEdit = '../cgi/set.cgi?cmd=diag_mirrorEdit';
<!--#endif-->
<!--#ifdef CONFIG_SYS_PING_TEST-->
var url_get_diagPing = '../cgi/get.cgi?cmd=diag_ping';
var url_set_diagPing = '../cgi/set.cgi?cmd=diag_ping';
var url_set_diagPingAbort = '../cgi/set.cgi?cmd=diag_pingAbort';
var url_get_diagPingResult = '../cgi/get.cgi?cmd=diag_pingResult';
var url_get_diagPingRunning = '../cgi/get.cgi?cmd=diag_pingRunning';
<!--#endif-->
<!--#ifdef CONFIG_SYS_TRACE_ROUTE-->
var url_get_diagTraceroute = '../cgi/get.cgi?cmd=diag_traceroute';
var url_set_diagTraceroute = '../cgi/set.cgi?cmd=diag_traceroute';
var url_set_diagTracerouteAbort = '../cgi/set.cgi?cmd=diag_tracerouteAbort';
var url_get_diagTracerouteResult = '../cgi/get.cgi?cmd=diag_tracerouteResult';
var url_get_diagTracerouteRunning = '../cgi/get.cgi?cmd=diag_tracerouteRunning';
<!--#endif-->
var url_set_diagReboot = '../cgi/set.cgi?cmd=sys_reboot';
var url_set_diagRestore = '../cgi/set.cgi?cmd=sys_restore';
<!--#ifdef CONFIG_SYS_PROTO_UDLD-->
var url_get_diagUDLDProp = '../cgi/get.cgi?cmd=udld_prop';
var url_set_diagUDLDProp = '../cgi/set.cgi?cmd=udld_prop';
var url_get_diagUDLDPortEdit = '../cgi/get.cgi?cmd=udld_portEdit';
var url_set_diagUDLDPortEdit = '../cgi/set.cgi?cmd=udld_portEdit';
var url_get_diagUDLDNeighbor = '../cgi/get.cgi?cmd=udld_neighbor';
<!--#endif-->

/* Management */
var url_get_mgmtImg = '../cgi/get.cgi?cmd=file_img';
var url_set_mgmtBackup = '../cgi/set.cgi?cmd=file_backup';
var url_get_mgmtBackupStatus = '../cgi/get.cgi?cmd=file_bakStatus';
var url_get_mgmtCfg = '../cgi/get.cgi?cmd=file_cfg';
var url_set_mgmtUpgrade = '../cgi/set.cgi?cmd=file_upgrade';
var url_get_mgmtUpgradeStatus = '../cgi/get.cgi?cmd=file_upStatus';
var url_get_mgmtActImg = '../cgi/get.cgi?cmd=file_dualImg';
var url_set_mgmtActImg = '../cgi/set.cgi?cmd=file_dualImg';
var url_get_mgmtSavecfg = '../cgi/get.cgi?cmd=file_savecfg';
var url_set_mgmtSavecfg = '../cgi/set.cgi?cmd=file_savecfg';
var url_get_mgmtSavecfgStatus = '../cgi/get.cgi?cmd=file_saveStatus';
var url_get_mgmtAcct = '../cgi/get.cgi?cmd=sys_acct';
var url_get_mgmtAcctAdd = '../cgi/get.cgi?cmd=sys_acctAdd';
var url_set_mgmtAcctAdd = '../cgi/set.cgi?cmd=sys_acctAdd';
var url_get_mgmtAcctEdit = '../cgi/get.cgi?cmd=sys_acctEdit';
var url_set_mgmtAcctEdit = '../cgi/set.cgi?cmd=sys_acctEdit';
var url_set_mgmtAcctDel = '../cgi/set.cgi?cmd=sys_acctDel';

<!--#ifdef CONFIG_SYS_UI_SNMP-->
<!--#ifdef CONFIG_SYS_SNMPV3-->
var url_get_snmpView = '../cgi/get.cgi?cmd=snmp_view';
var url_get_snmpViewAdd = '../cgi/get.cgi?cmd=snmp_viewAdd';
var url_set_snmpViewAdd = '../cgi/set.cgi?cmd=snmp_viewAdd';
var url_set_snmpViewDel = '../cgi/set.cgi?cmd=snmp_viewDel';
var url_get_snmpGroup = '../cgi/get.cgi?cmd=snmp_group';
var url_get_snmpGroupAdd = '../cgi/get.cgi?cmd=snmp_groupAdd';
var url_set_snmpGroupAdd = '../cgi/set.cgi?cmd=snmp_groupAdd';
var url_get_snmpGroupEdit = '../cgi/get.cgi?cmd=snmp_groupEdit';
var url_set_snmpGroupEdit = '../cgi/set.cgi?cmd=snmp_groupEdit';
var url_set_snmpGroupDel = '../cgi/set.cgi?cmd=snmp_groupDel';
var url_get_snmpCommunity = '../cgi/get.cgi?cmd=snmp_community';
var url_get_snmpCommunityAdd = '../cgi/get.cgi?cmd=snmp_communityAdd';
var url_set_snmpCommunityAdd = '../cgi/set.cgi?cmd=snmp_communityAdd';
var url_get_snmpCommunityEdit = '../cgi/get.cgi?cmd=snmp_communityEdit';
var url_set_snmpCommunityEdit = '../cgi/set.cgi?cmd=snmp_communityEdit';
var url_set_snmpCommunityDel = '../cgi/set.cgi?cmd=snmp_communityDel';
var url_get_snmpTrapEvent = '../cgi/get.cgi?cmd=snmp_trapEvent';
var url_set_snmpTrapEvent = '../cgi/set.cgi?cmd=snmp_trapEvent';
var url_get_snmpUser = '../cgi/get.cgi?cmd=snmp_user';
var url_get_snmpUserAdd = '../cgi/get.cgi?cmd=snmp_userAdd';
var url_set_snmpUserAdd = '../cgi/set.cgi?cmd=snmp_userAdd';
var url_get_snmpUserEdit = '../cgi/get.cgi?cmd=snmp_userEdit';
var url_set_snmpUserEdit = '../cgi/set.cgi?cmd=snmp_userEdit';
var url_set_snmpUserDel = '../cgi/set.cgi?cmd=snmp_userDel';
var url_get_snmpEngineID = '../cgi/get.cgi?cmd=snmp_engineID';
var url_set_snmpEngineID = '../cgi/set.cgi?cmd=snmp_engineID';
var url_get_snmpEngineIDAdd = '../cgi/get.cgi?cmd=snmp_engineIDAdd';
var url_set_snmpEngineIDAdd = '../cgi/set.cgi?cmd=snmp_engineIDAdd';
var url_get_snmpEngineIDEdit = '../cgi/get.cgi?cmd=snmp_engineIDEdit';
var url_set_snmpEngineIDEdit = '../cgi/set.cgi?cmd=snmp_engineIDEdit';
var url_set_snmpEngineIDDel = '../cgi/set.cgi?cmd=snmp_engineIDDel';
var url_get_snmpNotify =    '../cgi/get.cgi?cmd=snmp_notify';
var url_get_snmpNotifyAdd = '../cgi/get.cgi?cmd=snmp_notifyAdd';
var url_set_snmpNotifyAdd = '../cgi/set.cgi?cmd=snmp_notifyAdd';
var url_get_snmpNotifyEdit = '../cgi/get.cgi?cmd=snmp_notifyEdit';
var url_set_snmpNotifyEdit = '../cgi/set.cgi?cmd=snmp_notifyEdit';
var url_set_snmpNotifyDel = '../cgi/set.cgi?cmd=snmp_notifyDel';
<!--#else-->
var url_get_snmpv2Community = '../cgi/get.cgi?cmd=snmp_community';
var url_get_snmpv2CommunityAdd = '../cgi/get.cgi?cmd=snmp_communityAdd';
var url_set_snmpv2CommunityAdd = '../cgi/set.cgi?cmd=snmp_communityAdd';
var url_set_snmpv2CommunityDel = '../cgi/set.cgi?cmd=snmp_communityDel';
var url_get_snmpv2TrapEvent = '../cgi/get.cgi?cmd=snmp_trapEvent';
var url_set_snmpv2TrapEvent = '../cgi/set.cgi?cmd=snmp_trapEvent';
var url_get_snmpv2Notify =    '../cgi/get.cgi?cmd=snmp_notify';
var url_get_snmpv2NotifyAdd = '../cgi/get.cgi?cmd=snmp_notifyAdd';
var url_set_snmpv2NotifyAdd = '../cgi/set.cgi?cmd=snmp_notifyAdd';
var url_set_snmpv2NotifyDel = '../cgi/set.cgi?cmd=snmp_notifyDel';
<!--#endif-->
<!--#endif-->

<!--#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI-->
var url_get_mgmtRmonStats = '../cgi/get.cgi?cmd=rmon_statistics';
var url_set_mgmtRmonStatsClear = '../cgi/set.cgi?cmd=rmon_statisticsClear';
var url_get_mgmtRmonStatsIntf = '../cgi/get.cgi?cmd=rmon_statisticsIntf';
var url_get_mgmtRmonHistory = '../cgi/get.cgi?cmd=rmon_history';
var url_get_mgmtRmonHistoryAdd = '../cgi/get.cgi?cmd=rmon_historyAdd';
var url_set_mgmtRmonHistoryAdd = '../cgi/set.cgi?cmd=rmon_historyAdd';
var url_get_mgmtRmonHistoryEdit = '../cgi/get.cgi?cmd=rmon_historyEdit';
var url_set_mgmtRmonHistoryEdit = '../cgi/set.cgi?cmd=rmon_historyEdit';
var url_set_mgmtRmonHistoryDel = '../cgi/set.cgi?cmd=rmon_historyDel';
var url_get_mgmtRmonHistoryLog = '../cgi/get.cgi?cmd=rmon_historyLog';
var url_get_mgmtRmonEvent = '../cgi/get.cgi?cmd=rmon_event';
var url_get_mgmtRmonEventAdd = '../cgi/get.cgi?cmd=rmon_eventAdd';
var url_set_mgmtRmonEventAdd = '../cgi/set.cgi?cmd=rmon_eventAdd';
var url_get_mgmtRmonEventEdit = '../cgi/get.cgi?cmd=rmon_eventEdit';
var url_set_mgmtRmonEventEdit = '../cgi/set.cgi?cmd=rmon_eventEdit';
var url_set_mgmtRmonEventDel = '../cgi/set.cgi?cmd=rmon_eventDel';
var url_get_mgmtRmonEventLog = '../cgi/get.cgi?cmd=rmon_eventLog';
var url_get_mgmtRmonAlarm = '../cgi/get.cgi?cmd=rmon_alarm';
var url_get_mgmtRmonAlarmAdd = '../cgi/get.cgi?cmd=rmon_alarmAdd';
var url_set_mgmtRmonAlarmAdd = '../cgi/set.cgi?cmd=rmon_alarmAdd';
var url_get_mgmtRmonAlarmEdit = '../cgi/get.cgi?cmd=rmon_alarmEdit';
var url_set_mgmtRmonAlarmEdit = '../cgi/set.cgi?cmd=rmon_alarmEdit';
var url_set_mgmtRmonAlarmDel = '../cgi/set.cgi?cmd=rmon_alarmDel';
<!--#endif-->

<!--#ifdef CONFIG_SYS_POE-->
var url_get_poePort = '../cgi/get.cgi?cmd=poe_poe';
var url_get_poePortEdit = '../cgi/get.cgi?cmd=poe_poeEdit';
var url_set_poePortEdit = '../cgi/set.cgi?cmd=poe_poeEdit';
var url_get_poePortTimer = '../cgi/get.cgi?cmd=poe_poeTimer';
var url_set_poePortTimer = '../cgi/set.cgi?cmd=poe_poeTimer';
<!--#endif-->
<!--#ifdef CONFIG_SYS_DHCP_SERVER-->
/* DHCP */
var url_get_dhcpRelayProp = '../cgi/get.cgi?cmd=dr_prop';
var url_set_dhcpRelayProp = '../cgi/set.cgi?cmd=dr_prop';
var url_get_dhcpRelayPortEdit = '../cgi/get.cgi?cmd=dr_portEdit';
var url_set_dhcpRelayPortEdit = '../cgi/set.cgi?cmd=dr_portEdit';
var url_get_dhcpRelayipPool = '../cgi/get.cgi?cmd=dr_ipPool';
var url_get_dhcpRelayipPoolAdd = '../cgi/get.cgi?cmd=dr_ipPoolAdd';
var url_set_dhcpRelayipPoolAdd = '../cgi/set.cgi?cmd=dr_ipPoolAdd';
var url_get_dhcpRelayipPoolEdit = '../cgi/get.cgi?cmd=dr_ipPoolEdit';
var url_set_dhcpRelayipPoolEdit = '../cgi/set.cgi?cmd=dr_ipPoolEdit';
var url_set_dhcpRelayipPoolDel = '../cgi/set.cgi?cmd=dr_ipPoolDel';
var url_get_dhcpRelayvlanifPool = '../cgi/get.cgi?cmd=dr_vlanifipPool'; 
var url_set_dhcpRelayvlanifPool = '../cgi/set.cgi?cmd=dr_vlanifipPool'; 
var url_get_dhcpRelaygroupEdit = '../cgi/get.cgi?cmd=dr_groupEdit'; 
var url_set_dhcpRelaygroupEdit = '../cgi/set.cgi?cmd=dr_groupEdit'; 
var url_set_dhcpRelaygroupAdd = '../cgi/set.cgi?cmd=dr_groupAdd'; 
var url_get_dhcpRelaygroupAdd = '../cgi/get.cgi?cmd=dr_groupAdd'; 
var url_set_dhcpRelaygroupDel = '../cgi/set.cgi?cmd=dr_groupDel';
var url_get_dhcpRelayClients = '../cgi/get.cgi?cmd=dr_clientLists'; 
var url_get_dhcpRelayStaticBind = '../cgi/get.cgi?cmd=dr_staticBind'; 
var url_set_dhcpRelayStaticBindDel = '../cgi/set.cgi?cmd=dr_staticBindDel'; 
var url_get_dhcpRelayStaticBindEdit = '../cgi/get.cgi?cmd=dr_staticBindEdit'; 
var url_set_dhcpRelayStaticBindEdit = '../cgi/set.cgi?cmd=dr_staticBindEdit'; 
var url_set_dhcpRelayStaticBindAdd = '../cgi/set.cgi?cmd=dr_staticBindAdd'; 
<!--#endif-->
<!--#ifdef CONFIG_SYS_L3_ROUTE_RIP-->
/* rip */
var url_get_ripConfig = '../cgi/get.cgi?cmd=rip_ripconfig';
var url_set_ripConfig = '../cgi/set.cgi?cmd=rip_ripconfig';
var url_set_ripNetworkAdd = '../cgi/set.cgi?cmd=rip_ripNetworkAdd';
var url_set_ripNetworkDel = '../cgi/set.cgi?cmd=rip_ripNetworkDel';
<!--#endif-->
<!--#ifdef CONFIG_SYS_L3_ROUTE_OSPF-->
/* ospf */
var url_get_ospfConfig = '../cgi/get.cgi?cmd=ospf_ospfconfig';
var url_set_ospfConfig = '../cgi/set.cgi?cmd=ospf_ospfconfig';
var url_set_ospfAreaNetworkAdd = '../cgi/set.cgi?cmd=ospf_ospfAreaNetworkAdd';
var url_set_ospfAreaNetworkDel = '../cgi/set.cgi?cmd=ospf_ospfAreaNetworkDel';
<!--#endif-->
<!--#ifdef CONFIG_SYS_L3_VRRP-->
/* vrrp */
var url_get_ipv4Vrrp = '../cgi/get.cgi?cmd=vrrp_ipv4Vrrp';
var url_get_ipv4VrrpAdd = '../cgi/get.cgi?cmd=vrrp_ipv4VrrpAdd';
var url_set_ipv4VrrpAdd = '../cgi/set.cgi?cmd=vrrp_ipv4VrrpAdd';
var url_get_ipv4VrrpEdit = '../cgi/get.cgi?cmd=vrrp_ipv4VrrpEdit';
var url_set_ipv4VrrpEdit = '../cgi/set.cgi?cmd=vrrp_ipv4VrrpEdit';
var url_set_ipv4VrrpDel = '../cgi/set.cgi?cmd=vrrp_ipv4VrrpDel';
<!--#endif-->

/* This ready flag should put at file end */
var ready_js_url = 1;
