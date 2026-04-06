
#include <common/sys_def.h>
#include <common/sys_log.h>
#include <libfds/fds_default_init.h>
#include <libfds/fds_init.h>
#include <libsal/sal_sys.h>

typedef int32 (*fds_default_init_func)(void);
typedef struct fds_tbl_s
{
    fds_default_init_func dflt_init;
    char *decpstr;
} fds_tbl_t;

typedef uint32 (*fds_support_chk_func)(void);
typedef struct fds_condtbl_s
{
    fds_default_init_func dflt_init;
    char *decpstr;
    fds_support_chk_func check;
} fds_condtbl_t;

const fds_tbl_t fdsTbl[] =
{

    {fds_switch_default_init,          "Switch"},
    {fds_vlan_default_init,            "VLAN"},
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {fds_vlan_protocolVlan_init,       "Protocol VLAN"},
#endif
#ifdef CONFIG_SYS_IP_SUBNET
	{fds_vlan_ip_subnet_vlan_init,	   "IP Subnet VLAN"},
#endif

#ifdef CONFIG_SYS_VOICEVLAN
    {fds_vlan_voiceVlan_init,          "Vocie VLAN"},
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {fds_surveillance_vlan_init,       "Surveillance VLAN"},
#endif

#ifdef CONFIG_SYS_MIRROR
    {fds_mirror_default_init,          "Mirror"},
#endif 
    {fds_l2_default_init,              "L2"},
#ifdef CONFIG_SYS_L3_ROUTE
    {fds_l3_default_init,              "L3"},
#endif
    {fds_trunk_default_init,           "Trunk"},
    {fds_rate_default_init,            "Rate"},
    {fds_qos_default_init,             "QoS"},
#ifdef CONFIG_SYS_PROTO_LACP
    {fds_lacp_default_init,            "LACP"},
#endif
#ifdef CONFIG_SYS_EEE
    {fds_eee_default_init,             "EEE"},
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
    {fds_dos_default_init,             "DoS"},
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    {fds_igmp_default_init,            "IGMP"},
    #ifdef CONFIG_SYS_PROTO_MVR
    {fds_mvr_default_init,              "MVR"},
    #endif
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    {fds_mld_default_init,             "MLD"},
    #endif
#endif
#ifdef CONFIG_SYS_PROTO_STP
    {fds_stp_default_init,             "STP"},
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    {fds_lldp_default_init,            "LLDP"},
#endif
    {fds_sys_default_init,             "System"},
#ifdef CONFIG_SYS_UI_SNMP
    {fds_snmp_default_init,            "SNMP"},
#endif

#ifdef CONFIG_SYS_PORT_SECURITY
    {fds_psecure_default_init,         "Port Security"},
#endif
#ifdef CONFIG_SYS_SYSLOG
    {fds_log_default_init,             "Syslog"},
#endif
#ifdef CONFIG_SYS_AAA
    {fds_aaa_default_init,             "AAA"},
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    {fds_authmgr_default_init,         "Auth Manager"},
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
    {fds_radius_default_init,          "Radius"},
#endif
#ifdef CONFIG_SYS_PLAT_CUSMOD
    {fds_custom_default_init,          "Custom"},
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    {fds_isg_default_init,             "IP Source Guard"},
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {fds_dhcp_snooping_default_init,   "DHCP Snooping"},
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    {fds_dai_default_init,             "Dynamic ARP Inspection"},
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    {fds_gvrp_default_init,            "GVRP"},
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    {fds_garp_default_init,            "GARP"},
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
    {fds_udld_default_init,            "UDLD"},
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
	{fds_erps_default_init, 		   "ERPS"},
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
	{fds_loopback_default_init, 	   "LOOPBACK"},
#endif

#ifdef CONFIG_SYS_DHCP_SERVER
	{fds_dhcp_server_default_init,     "DHCP Server"},
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	{fds_easyCwmp_default_init,	       "CWMP"},
#endif
	{fds_oem_config_init,	           "Oem config"},
    {NULL, ""}
};

const fds_condtbl_t fdsBoardTbl[] =
{
#ifdef CONFIG_SYS_POE
	{fds_poe_default_init, 		       "POE",  fds_poe_Support_check},
#endif
    {NULL, "", NULL}
};

const fds_tbl_t fdsEntryTbl[] =
{

   
    {fds_vlan_dfltVlan_dfltEntry_init,         "VLAN"},
#ifdef CONFIG_SYS_VOICEVLAN
    {fds_vlan_voiceVlanOui_dfltEntry_init,     "Voice VLAN OUI"},
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {fds_surveillance_vlan_dfltEntry_init,     "Surveillance VLAN OUI"},
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    {fds_acl_mgmt_dfltEntry_init,      "MGMT ACL"},
#endif
    {NULL, ""}
};

const fds_tbl_t fdsPostTbl[] =
{
    {fds_sys_post_init,        "System"},
    {fds_port_post_init,       "Port"},
    {NULL, ""}
};

int32 fds_init(void)
{
    int32 ret = SYS_ERR_FAILED;
    uint32 i = 0;

    while (fdsTbl[i].dflt_init)
    {
        ret = fdsTbl[i].dflt_init();

        BOOTMSG_RET_PRINTF(ret, "Init %s Factory Default....%s\n", \
                           fdsTbl[i].decpstr, (SYS_ERR_OK != ret) ? "FAILED" : "OK");
        i++;
    }

    i = 0;
    while (fdsBoardTbl[i].dflt_init)
    {
        if (TRUE == fdsBoardTbl[i].check())
        {
            ret = fdsBoardTbl[i].dflt_init();

            BOOTMSG_RET_PRINTF(ret, "Init %s Factory Default....%s\n", \
                               fdsBoardTbl[i].decpstr, (SYS_ERR_OK != ret) ? "FAILED" : "OK");
        }

        i++;
    }

    return SYS_ERR_OK;
}

int32 fds_entry_init(void)
{
    int32 ret = SYS_ERR_FAILED;
    uint32 i = 0;

    while (fdsEntryTbl[i].dflt_init)
    {
        ret = fdsEntryTbl[i].dflt_init();

        BOOTMSG_RET_PRINTF(ret, "Init %s Default Entry....%s\n", \
                           fdsEntryTbl[i].decpstr, (SYS_ERR_OK != ret) ? "FAILED" : "OK");
        i++;
    }

    return SYS_ERR_OK;
}

int32 fds_post_init(void)
{
    int32 ret = SYS_ERR_FAILED;
    uint32 i = 0;

    while (fdsPostTbl[i].dflt_init)
    {
        ret = fdsPostTbl[i].dflt_init();

        BOOTMSG_RET_PRINTF(ret, "%s Post Initial....%s\n", \
                           fdsPostTbl[i].decpstr, (SYS_ERR_OK != ret) ? "FAILED" : "OK");
        i++;
    }

    return SYS_ERR_OK;
}

