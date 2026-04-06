/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 16116 $
 * $Date: 2011-03-07 09:20:10 +0800 (Mon, 07 Mar 2011) $
 *
 * Purpose : Initial processes on booting
 *
 * Feature : Execute
 */

 /*
 * Include Files
 */
#include <unistd.h>
#include <common/sys_def.h>
#include <common/sys_util.h>
#include <common/sys_log.h>
#include <libsal/sal_led.h>
#ifdef CONFIG_SYS_SNMP
#include <libsal/sal_snmp.h>
#endif
#include <libsal/sal_trunk.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_syslog.h>
#include <libsal/sal_init.h>
#include <libsal/sal_crash.h>
#include <libcmd/cmd_init.h>
#include <libcmd/cmd.h>
#include <libfds/fds_default_init.h>
#include <syslog.h>
#include <pswitch/image.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */
static int32 routing_daemon(void)
{
    SYSTEM_CMD("mkdir -p /etc/iproute2");
    SYSTEM_CMD("echo '200 %s' > /etc/iproute2/rt_tables", L3_INTF_MGMTVLAN);
    SYSTEM_CMD("ip rule add from all lookup %s", L3_INTF_MGMTVLAN);
#ifdef CONFIG_SYS_IPV6
    SYSTEM_CMD("ip -6 rule add priority 32765 from all lookup %s", L3_INTF_MGMTVLAN);
#endif

#ifdef CONFIG_SYS_L3_ROUTE
    SYSTEM_CMD("echo 0 > /proc/sys/net/ipv4/conf/%s/forwarding", L3_INTF_MGMTVLAN);
    SYSTEM_CMD("echo 0 > /proc/sys/net/ipv6/conf/%s/forwarding", L3_INTF_MGMTVLAN);
    SYSTEM_CMD("echo 3 > /proc/sys/net/ipv6/conf/%s/router_solicitations", L3_INTF_MGMTVLAN);

    SYSTEM_CMD_BG("zebra -P 0");
#ifdef CONFIG_SYS_L3_ROUTE_RIP
    SYSTEM_CMD_BG("ripd");
#endif
#endif
    SYSTEM_CMD_BG("fpmd");
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
    SYSTEM_CMD_BG("ospfd");
#endif
#ifdef CONFIG_SYS_L3_ROUTE_BGP
	SYSTEM_CMD_BG("bgpd");
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
    SYSTEM_CMD_BG("dhcpserver");
#endif
#ifdef CONFIG_SYS_L3_VRRP
	SYSTEM_CMD_BG("vrrpd");
#endif

    return SYS_ERR_OK;
}

static int system_ready(void)
{
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_log_enable_get(&enable));

#ifdef CONFIG_SYS_APP_MSNTP
    /* Update system time from sntp */
    system("handler -u time &");
#endif

#ifdef CONFIG_SYS_UI_SNMP
    _sal_snmp_notifyMsg_init();
#endif

    /* System ready LED */
    SYS_ERR_CHK(sal_led_switchEnable_set(LED_TYPE_SYS, ENABLED));
    /* Port ready LED */
   // SYS_ERR_CHK(sal_led_systemCtrl_set(SYS_LED_STATE_ALLPORT_NORMAL));

#ifdef CONFIG_SYS_PROTO_ISG
    ISG_DB_RESTORE();
#endif

    system("killall -q bootklogd");

    sal_sys_status_set(SYS_STATUS_READY);
    sal_util_file_touch(SYS_SYSTEM_READY_FLAG);

    /*
     * logging and trap
     */
    /* Don't remove this syslog. It's a trick for syslogd for first log message */
    syslog(LOG_DEBUG, SYS_LOG_JUNK_MSG);

    /* Print startup logging enable here */
    if (ENABLED == enable)
        SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_START);

    if (SYS_ERR_OK == sal_util_file_exist(SYS_SYSTEM_RESTART_FLAG))
    {
        unlink(SYS_SYSTEM_RESTART_FLAG);
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_WARMSTART);
#ifdef CONFIG_SYS_UI_SNMP
        sal_snmp_startupTrap_send(ENABLED);
#endif
    }
    else
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_COLDSTART);
#ifdef CONFIG_SYS_UI_SNMP
        sal_snmp_startupTrap_send(DISABLED);
#endif
    }


#ifdef CONFIG_SYS_SYSLOG
    /* Init klogd after "System Startup!" to make sure linkup logs show after it */
    if (SYS_ERR_OK == sal_util_file_exist("/bin/klogd"))
        system("klogd &");
#endif

    sys_dbg_reset();

    return SYS_ERR_OK;
}

static int txOverloading_check(sys_logic_port_t port)
{
    lldp_overLoading_t overload;

    SYS_ERR_CHK(sal_lldp_portTlvOverload_get(port, &overload));
    SYS_ERR_CHK(sal_lldp_txOverloadStatus_set(port, overload.isOverload));

    return SYS_ERR_OK;
}

int main(int argc, char **argv)
{
    uint32 currSelPart = 0;
    uint32 magic;
    uint32 time = 0;
    uint32 size;
    time_t usr_time = 0;
    char name[32];
    struct tm *tm_time;
#ifdef CONFIG_SYS_PROTO_LLDP
    sys_logic_port_t logicPort;
#endif


    SYS_ERR_CHK(sal_init());

    /* Init the year of time as fw image build time */
#ifdef CONFIG_SYS_DUAL_IMAGE
    sal_sys_dualImageCurrSel_get(&currSelPart);
#endif
    sal_sys_fwImageHdrInfo_get(currSelPart, &magic, &time, &size, name);
    usr_time = (time_t) ntohl(time);
    tm_time = gmtime(&usr_time);
    tm_time->tm_mon = 0;
    tm_time->tm_mday = 1;
    tm_time->tm_hour = 0;
    tm_time->tm_min = 0;
    tm_time->tm_sec = 0;
    usr_time = mktime(tm_time);
    stime(&usr_time);

    system("handler -f init_port &");

    SYS_ERR_CHK(routing_daemon());
#ifdef CONFIG_SYS_APP_EASYCWMP
    system("easycwmpd -b -f &");
#endif
#ifdef CONFIG_SYS_APP_KSID
    if (SYS_ERR_OK == sal_util_file_exist("/bin/ksid"))
        system("ksid &");
#endif

    SYS_ERR_CHK(cmd_run());

#ifdef CONFIG_SYS_PROTO_LLDP
    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        SYS_ERR_CHK(txOverloading_check(logicPort));
    }
#endif

    SYS_ERR_CHK(system_ready());

#ifdef CONFIG_SYS_L3_ROUTE
    SYSTEM_CMD_BG("handler -f init_l3");
#endif
	SYSTEM_CMD_BG("monitor");

    return SYS_ERR_OK;
}

