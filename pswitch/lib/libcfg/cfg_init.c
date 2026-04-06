
#include <libcfg/cfg.h>

int32 cfg_init(void)
{
    SYS_ERR_CHK(cfg_shm_init());
    SYS_ERR_CHK(cfg_sys_devInfo_init());
    SYS_ERR_CHK(cfg_sys_boardModel_init());
    SYS_ERR_CHK(cfg_sys_userInfo_init());
    SYS_ERR_CHK(cfg_vlan_init());
    SYS_ERR_CHK(cfg_vlan_list_init(DFLT_VLAN_DATABASE_KEY));

#ifdef CONFIG_SYS_AAA
    SYS_ERR_CHK(cfg_aaa_init());
#endif

#ifdef CONFIG_SYS_L3_ROUTE
    SYS_ERR_CHK(cfg_l3_init());
#endif

#ifdef CONFIG_SYS_LIB_RADIUS
    SYS_ERR_CHK(cfg_radius_init());
#endif

#ifdef CONFIG_SYS_LIB_TACPLUS
    SYS_ERR_CHK(cfg_tacplus_init());
#endif

#ifdef CONFIG_SYS_SYSLOG
    SYS_ERR_CHK(cfg_log_init());
#endif

#ifdef CONFIG_SYS_PROTO_STP
    SYS_ERR_CHK(cfg_stp_init());
#endif

#ifdef CONFIG_SYS_PROTO_ERPS
    SYS_ERR_CHK(cfg_erps_init());
#endif

#ifdef CONFIG_SYS_PROTO_LOOPBACK
    SYS_ERR_CHK(cfg_loopback_init());
#endif

#ifdef CONFIG_SYS_POE
    SYS_ERR_CHK(cfg_poe_init());
#endif
#ifdef CONFIG_SYS_LIB_CUSTOM
    SYS_ERR_CHK(cfg_custom_init());
#endif

#ifdef CONFIG_SYS_DHCP_SERVER
	SYS_ERR_CHK(cfg_dhcp_server_init());
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	SYS_ERR_CHK(cfg_easycwmp_db_init());
#endif

    return SYS_ERR_OK;
}

