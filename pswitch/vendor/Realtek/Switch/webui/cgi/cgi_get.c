
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <include/cgi.h>

#ifdef CONFIG_SYS_AAA
extern const cgi_cmdmap_t cmdGetAaaTbl[];
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
extern const cgi_cmdmap_t cmdGetAclTbl[];
#endif
extern const cgi_cmdmap_t cmdGetDiagTbl[];
#ifdef CONFIG_SYS_MGMT_ACL
extern const cgi_cmdmap_t cmdGetAclMgmtTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
extern const cgi_cmdmap_t cmdGetAuthmgrTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_DAI
extern const cgi_cmdmap_t cmdGetDaiTbl[];
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
extern const cgi_cmdmap_t cmdGetDosTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
extern const cgi_cmdmap_t cmdGetDsTbl[];
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
extern const cgi_cmdmap_t cmdGetDrTbl[];
#endif
extern const cgi_cmdmap_t cmdGetFileTbl[];
#ifdef CONFIG_SYS_PROTO_GVRP
extern const cgi_cmdmap_t cmdGetGvrpTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_GARP
extern const cgi_cmdmap_t cmdGetGmrpTbl[];
#endif
extern const cgi_cmdmap_t cmdGetHomeTbl[];
#ifdef CONFIG_SYS_PROTO_ISG
extern const cgi_cmdmap_t cmdGetIsgTbl[];
#endif
#ifdef CONFIG_SYS_L3_ROUTE
extern const cgi_cmdmap_t cmdGetL3Tbl[];
#endif
extern const cgi_cmdmap_t cmdGetLagTbl[];
#ifdef CONFIG_SYS_PROTO_LLDP
extern const cgi_cmdmap_t cmdGetLldpTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
extern const cgi_cmdmap_t cmdGetErpsTbl[];
#endif
#ifdef CONFIG_SYS_SYSLOG
extern const cgi_cmdmap_t cmdGetLogTbl[];
#endif
extern const cgi_cmdmap_t cmdGetMacTbl[];
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
extern const cgi_cmdmap_t cmdGetMcastTbl[];
#endif
extern const cgi_cmdmap_t cmdGetPanelTbl[];
extern const cgi_cmdmap_t cmdGetPortTbl[];
#ifdef CONFIG_SYS_PORT_SECURITY
extern const cgi_cmdmap_t cmdGetPsecureTbl[];
#endif
extern const cgi_cmdmap_t cmdGetQosTbl[];
#ifdef CONFIG_SYS_LIB_RADIUS
extern const cgi_cmdmap_t cmdGetRadiusTbl[];
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
extern const cgi_cmdmap_t cmdGetRmonTbl[];
#endif
#ifdef CONFIG_SYS_UI_SNMP
extern const cgi_cmdmap_t cmdGetSnmpTbl[];
#endif
#ifdef CONFIG_SYS_STORM_CTRL
extern const cgi_cmdmap_t cmdGetStormTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_STP
extern const cgi_cmdmap_t cmdGetStpTbl[];
#endif
extern const cgi_cmdmap_t cmdGetSysTbl[];
#ifdef CONFIG_SYS_LIB_TACPLUS
extern const cgi_cmdmap_t cmdGetTacplusTbl[];
#endif
extern const cgi_cmdmap_t cmdGetTimeTbl[];
#ifdef CONFIG_SYS_PROTO_UDLD
extern const cgi_cmdmap_t cmdGetUdldTbl[];
#endif
extern const cgi_cmdmap_t cmdGetVlanTbl[];
#ifdef CONFIG_SYS_MACVLAN
extern const cgi_cmdmap_t cmdGetVlanMacTbl[];
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
extern const cgi_cmdmap_t cmdGetVlanProtocolTbl[];
#endif
#ifdef CONFIG_SYS_IP_SUBNET
extern const cgi_cmdmap_t cmdGetVlanIpSubNetTbl[];
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
extern const cgi_cmdmap_t cmdGetVlanSurveillanceTbl[];
#endif
#ifdef CONFIG_SYS_VOICEVLAN
extern const cgi_cmdmap_t cmdGetVlanVoiceTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_ERPS

#endif
#ifdef CONFIG_SYS_POE
extern const cgi_cmdmap_t cmdGetPoeTbl[];
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
extern const cgi_cmdmap_t cmdGetRipTbl[];
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
extern const cgi_cmdmap_t cmdGetOspfTbl[];
#endif
#ifdef CONFIG_SYS_L3_VRRP
extern const cgi_cmdmap_t cmdGetVrrpTbl[];
#endif

const cgi_catmap_t catGetTbl[] =
{
#ifdef CONFIG_SYS_AAA
    {CGI_CAT_AAA,              cmdGetAaaTbl},
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    {CGI_CAT_ACL,              cmdGetAclTbl},
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    {CGI_CAT_ACL_MGMT,         cmdGetAclMgmtTbl},
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    {CGI_CAT_AUTHMGR,          cmdGetAuthmgrTbl},
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    {CGI_CAT_DAI,              cmdGetDaiTbl},
#endif
    {CGI_CAT_DIAG,             cmdGetDiagTbl},
#ifdef CONFIG_SYS_DOS_PROTECTION
    {CGI_CAT_DOS,              cmdGetDosTbl},
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {CGI_CAT_DS,               cmdGetDsTbl},
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
    {CGI_CAT_DR,               cmdGetDrTbl},
#endif
    {CGI_CAT_FILE,             cmdGetFileTbl},
#ifdef CONFIG_SYS_PROTO_GVRP
    {CGI_CAT_GVRP,             cmdGetGvrpTbl},
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    {CGI_CAT_GMRP,			   cmdGetGmrpTbl},
#endif
    {CGI_CAT_HOME,             cmdGetHomeTbl},
#ifdef CONFIG_SYS_PROTO_ISG
    {CGI_CAT_IPSG,             cmdGetIsgTbl},
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    {CGI_CAT_L3,               cmdGetL3Tbl},
#endif
    {CGI_CAT_LAG,              cmdGetLagTbl},
#ifdef CONFIG_SYS_PROTO_LLDP
    {CGI_CAT_LLDP,             cmdGetLldpTbl},
#endif
#ifdef CONFIG_SYS_SYSLOG
    {CGI_CAT_LOG,              cmdGetLogTbl},
#endif
    {CGI_CAT_MAC,              cmdGetMacTbl},
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    {CGI_CAT_MCAST,          cmdGetMcastTbl},
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
	{CGI_CMD_GET_ERPS_VAL,	   cmdGetErpsTbl},
#endif
    {CGI_CAT_PANEL,            cmdGetPanelTbl},
    {CGI_CAT_PORT,             cmdGetPortTbl},
#ifdef CONFIG_SYS_PORT_SECURITY
    {CGI_CAT_PSECURE,          cmdGetPsecureTbl},
#endif
    {CGI_CAT_QOS,              cmdGetQosTbl},
#ifdef CONFIG_SYS_LIB_RADIUS
    {CGI_CAT_RADIUS,           cmdGetRadiusTbl},
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    {CGI_CAT_RMON,             cmdGetRmonTbl},
#endif
#ifdef CONFIG_SYS_UI_SNMP
    {CGI_CAT_SNMP,             cmdGetSnmpTbl},
#endif
#ifdef CONFIG_SYS_STORM_CTRL
    {CGI_CAT_STORM,            cmdGetStormTbl},
#endif
#ifdef CONFIG_SYS_PROTO_STP
    {CGI_CAT_STP,              cmdGetStpTbl},
#endif
    {CGI_CAT_SYS,              cmdGetSysTbl},
#ifdef CONFIG_SYS_LIB_TACPLUS
    {CGI_CAT_TACPLUS,          cmdGetTacplusTbl},
#endif
    {CGI_CAT_TIME,             cmdGetTimeTbl},
#ifdef CONFIG_SYS_PROTO_UDLD
    {CGI_CAT_UDLD,             cmdGetUdldTbl},
#endif
    {CGI_CAT_VLAN,             cmdGetVlanTbl},
#ifdef CONFIG_SYS_MACVLAN
    {CGI_CAT_VLAN_MAC,         cmdGetVlanMacTbl},
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {CGI_CAT_VLAN_PROTOCOL,cmdGetVlanProtocolTbl},
#endif
#ifdef CONFIG_SYS_IP_SUBNET
	{CGI_CAT_VLAN_IP_SUBNET, cmdGetVlanIpSubNetTbl},
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {CGI_CAT_VLAN_SURVEILLANCE,cmdGetVlanSurveillanceTbl},
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    {CGI_CAT_VLAN_VOICE,       cmdGetVlanVoiceTbl},
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
	
#endif
#ifdef CONFIG_SYS_POE
	{CGI_CAT_POE,	   cmdGetPoeTbl},
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	{CGI_CAT_RIP,	   cmdGetRipTbl},
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	{CGI_CAT_OSPF,	   cmdGetOspfTbl},
#endif
#ifdef CONFIG_SYS_L3_VRRP
	{CGI_CAT_VRRP,	   cmdGetVrrpTbl},
#endif

    {NULL, NULL},
};

int32 main(int argc, char* argv[], char* envp[])
{
    int32 ret = SYS_ERR_FAILED;
    uint32 i = 0;
    uint32 j = 0;
    char *cmd = NULL;

    cgi_util_paraPairs_init(FALSE);

    if (!CGI_IS_PARAM_EXIST(CGI_CMD, cmd))
        goto exit;

    if (0 == osal_strlen(cmd))
        goto exit;

    if (!CGI_IS_VALUE_EQUAL(cmd, CGI_CAT_HOME"_"CGI_CMD_HOME_LOGIN)
        && !CGI_IS_VALUE_EQUAL(cmd, CGI_CAT_HOME"_"CGI_CMD_HOME_LOGINSTATUS))
    {
        if (CGI_IS_VALUE_EQUAL(cmd, CGI_CAT_HOME"_"CGI_CMD_HOME_MONITOR)
            || CGI_IS_VALUE_EQUAL(cmd, CGI_CAT_SYS"_"CGI_CMD_SYS_CPUMEM)
            || CGI_IS_VALUE_EQUAL(cmd, CGI_CAT_PANEL"_"CGI_CMD_PANEL_INFO))
        {
            if (CGI_SESSION_ACTIVE != cgi_util_session_check(FALSE))
                return 0;
        }
        else
        {
            if (CGI_SESSION_ACTIVE != cgi_util_session_check(TRUE))
                return 0;
        }
    }

    i = 0;
    index(cmd, '_')[0] = '\0';
    while (NULL != catGetTbl[i].catstr)
    {
        if (0 == osal_strncmp(catGetTbl[i].catstr, cmd, osal_strlen(cmd)))
        {
            j = 0;
            while (NULL != catGetTbl[i].cmdmap[j].cmdstr)
            {
                if (0 == osal_strcmp(cmd + osal_strlen(catGetTbl[i].catstr) + 1, catGetTbl[i].cmdmap[j].cmdstr))
                {
                    ret = catGetTbl[i].cmdmap[j].cmdfunc();

                    return ret;
                }

                j++;
            }

            return 0;
        }

        i++;
    }

exit:
    CGI_RESPONSE("");
    return 0;
}

