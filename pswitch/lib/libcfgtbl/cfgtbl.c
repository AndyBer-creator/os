
#include <libcfg/cfg.h>

const cfg_tbl_t cfgTbl[] =
{

    {CFG_ID_DEVINFO,     sizeof(cfg_devInfo_t)},
    {CFG_ID_BOARD_MODEL, sizeof(cfg_boardModel_t)},
    {CFG_ID_SYSTEM,      sizeof(cfg_sys_t)},
    {CFG_ID_TIME,        sizeof(cfg_time_t)},
    {CFG_ID_PORT,        sizeof(cfg_port_t)},
#ifdef CONFIG_SYS_PORT_SECURITY
    {CFG_ID_PSECURE,     sizeof(cfg_psecure_t)},
#endif
    {CFG_ID_SWITCH,      sizeof(cfg_switch_t)},
    {CFG_ID_VLAN,        sizeof(cfg_vlan_t)},
#ifdef CONFIG_SYS_VOICEVLAN
    {CFG_ID_VLAN_VOICE,  sizeof(cfg_vlan_voice_t)},
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {CFG_ID_VLAN_PROTO,  sizeof(cfg_vlan_proto_t)},
#endif
#ifdef CONFIG_SYS_IP_SUBNET
	{CFG_ID_VLAN_IP_SUBNET,  sizeof(cfg_vlan_ipsubnet_t)},
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {CFG_ID_VLAN_SURVEILLANCE,  sizeof(cfg_surveillance_vlan_t)},
#endif
#ifdef CONFIG_SYS_MIRROR
    {CFG_ID_MIRROR,      sizeof(cfg_mirror_t)},
#endif
    {CFG_ID_L2,          sizeof(cfg_l2_t)},
#ifdef CONFIG_SYS_L3_ROUTE
    {CFG_ID_L3,          sizeof(cfg_l3_t)},
#endif
    {CFG_ID_TRUNK,       sizeof(cfg_trunk_t)},
#ifdef CONFIG_SYS_PROTO_LACP
    {CFG_ID_LACP,        sizeof(cfg_lacp_t)},
#endif
    {CFG_ID_QOS,         sizeof(cfg_qos_t)},
#ifdef CONFIG_SYS_UI_SNMP
    {CFG_ID_SNMP,        sizeof(cfg_snmp_t)},
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    {CFG_ID_IGMP,        sizeof(cfg_igmp_t)},
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    {CFG_ID_MLD,        sizeof(cfg_mld_t)},
    #endif
#endif
    {CFG_ID_RATE,        sizeof(cfg_rate_t)},
#ifdef CONFIG_SYS_PROTO_STP
    {CFG_ID_STP,         sizeof(cfg_stp_t)},
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    {CFG_ID_LLDP,        sizeof(cfg_lldp_t)},
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    {CFG_ID_LLDP_MED,    sizeof(cfg_lldp_med_t)},
  #endif
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
    {CFG_ID_DOS,         sizeof(cfg_dos_t)},
#endif
    {CFG_ID_LOG,         sizeof(cfg_log_t)},
#ifdef CONFIG_SYS_AAA
    {CFG_ID_AAA,         sizeof(cfg_aaa_t)},
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    {CFG_ID_AUTHMGR,     sizeof(cfg_authmgr_t)},
#endif
#ifdef CONFIG_SYS_LIB_CUSTOM
    {CFG_ID_CUSTOM,      sizeof(cfg_custom_t)},
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    {CFG_ID_ISG,      sizeof(cfg_isg_t)},
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {CFG_ID_DHCP_SNOOPING,      sizeof(cfg_dhcp_snooping_t)},
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    {CFG_ID_DAI,      sizeof(cfg_dai_t)},
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
    {CFG_ID_RADIUS,   sizeof(cfg_radius_t)},
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
    {CFG_ID_TACPLUS,   sizeof(cfg_tacplus_t)},
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    {CFG_ID_GVRP,   sizeof(cfg_gvrp_t)},
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    {CFG_ID_GARP,   sizeof(cfg_gmrp_t)},
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    {CFG_ID_RMON,        sizeof(cfg_rmon_t)},
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
    {CFG_ID_UDLD,        sizeof(cfg_udld_t)},
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
	{CFG_ID_ERPS,		 sizeof(cfg_erps_t)},
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
    {CFG_ID_LOOPBACK,	 sizeof(cfg_loopback_t)},
#endif
#ifdef CONFIG_SYS_POE
	{CFG_ID_POE,		 sizeof(cfg_poe_t)},
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	{CFG_ID_DHCP_SERVER, sizeof(cfg_dhcp_server_t)},
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	{CFG_ID_EASYCWMP,   sizeof(cfg_easycwmp_t)},
#endif

    {-1, 0}
};

uint32 cfgtbl_size_ret(void)
{
    return (sizeof(cfgTbl));
}
