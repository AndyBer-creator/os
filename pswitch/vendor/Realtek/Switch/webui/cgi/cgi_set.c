
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <include/cgi.h>

#ifdef CONFIG_SYS_AAA
extern const cgi_cmdmap_t cmdSetAaaTbl[];
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
extern const cgi_cmdmap_t cmdSetAclTbl[];
#endif
#ifdef CONFIG_SYS_MGMT_ACL
extern const cgi_cmdmap_t cmdSetAclMgmtTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
extern const cgi_cmdmap_t cmdSetAuthmgrTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_DAI
extern const cgi_cmdmap_t cmdSetDaiTbl[];
#endif
extern const cgi_cmdmap_t cmdSetDiagTbl[];
#ifdef CONFIG_SYS_DOS_PROTECTION
extern const cgi_cmdmap_t cmdSetDosTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
extern const cgi_cmdmap_t cmdSetDsTbl[];
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
extern const cgi_cmdmap_t cmdSetDrTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
extern const cgi_cmdmap_t cmdSetGvrpTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_GARP
extern const cgi_cmdmap_t cmdSetGmrpTbl[];
#endif
extern const cgi_cmdmap_t cmdSetFileTbl[];
extern const cgi_cmdmap_t cmdSetHomeTbl[];
#ifdef CONFIG_SYS_PROTO_ISG
extern const cgi_cmdmap_t cmdSetIsgTbl[];
#endif
#ifdef CONFIG_SYS_L3_ROUTE
extern const cgi_cmdmap_t cmdSetL3Tbl[];
#endif
extern const cgi_cmdmap_t cmdSetLagTbl[];
#ifdef CONFIG_SYS_PROTO_LLDP
extern const cgi_cmdmap_t cmdSetLldpTbl[];
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
extern const cgi_cmdmap_t cmdSetErpsTbl[] ;
#endif
#ifdef CONFIG_SYS_SYSLOG
extern const cgi_cmdmap_t cmdSetLogTbl[];
#endif
extern const cgi_cmdmap_t cmdSetMacTbl[];
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
extern const cgi_cmdmap_t cmdSetMcastTbl[];
#endif
extern const cgi_cmdmap_t cmdSetPortTbl[];
#ifdef CONFIG_SYS_PORT_SECURITY
extern const cgi_cmdmap_t cmdSetPsecureTbl[];
#endif
extern const cgi_cmdmap_t cmdSetQosTbl[];
#ifdef CONFIG_SYS_LIB_RADIUS
extern const cgi_cmdmap_t cmdSetRadiusTbl[];
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
extern const cgi_cmdmap_t cmdSetRmonTbl[];
#endif
extern const cgi_cmdmap_t cmdSetStormTbl[];
#ifdef CONFIG_SYS_PROTO_STP
extern const cgi_cmdmap_t cmdSetStpTbl[];
#endif
extern const cgi_cmdmap_t cmdSetSysTbl[];
#ifdef CONFIG_SYS_UI_SNMP
extern const cgi_cmdmap_t cmdSetSnmpTbl[];
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
extern const cgi_cmdmap_t cmdSetTacplusTbl[];
#endif
extern const cgi_cmdmap_t cmdSetTimeTbl[];
#ifdef CONFIG_SYS_PROTO_UDLD
extern const cgi_cmdmap_t cmdSetUdldTbl[];
#endif
extern const cgi_cmdmap_t cmdSetVlanTbl[];
#ifdef CONFIG_SYS_MACVLAN
extern const cgi_cmdmap_t cmdSetVlanMacTbl[];
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
extern const cgi_cmdmap_t cmdSetVlanProtocolTbl[];
#endif
#ifdef CONFIG_SYS_IP_SUBNET
extern const cgi_cmdmap_t cmdSetVlanIpSubnetTbl[];
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
extern const cgi_cmdmap_t cmdSetVlanSurveillanceTbl[];
#endif
#ifdef CONFIG_SYS_VOICEVLAN
extern const cgi_cmdmap_t cmdSetVlanVoiceTbl[];
#endif
#ifdef CONFIG_SYS_POE
extern const cgi_cmdmap_t cmdSetPoeTbl[];
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
extern const cgi_cmdmap_t cmdSetRipTbl[];
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
extern const cgi_cmdmap_t cmdSetOspfTbl[];
#endif
#ifdef CONFIG_SYS_L3_VRRP
extern const cgi_cmdmap_t cmdSetVrrpTbl[];
#endif

const cgi_catmap_t catSetTbl[] =
{
#ifdef CONFIG_SYS_AAA
    {CGI_CAT_AAA,              cmdSetAaaTbl},
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    {CGI_CAT_ACL,              cmdSetAclTbl},
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    {CGI_CAT_ACL_MGMT,         cmdSetAclMgmtTbl},
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    {CGI_CAT_AUTHMGR,          cmdSetAuthmgrTbl},
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    {CGI_CAT_DAI,              cmdSetDaiTbl},
#endif
    {CGI_CAT_DIAG,             cmdSetDiagTbl},
#ifdef CONFIG_SYS_DOS_PROTECTION
    {CGI_CAT_DOS,              cmdSetDosTbl},
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {CGI_CAT_DS,               cmdSetDsTbl},
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
    {CGI_CAT_DR,               cmdSetDrTbl},
#endif
    {CGI_CAT_FILE,             cmdSetFileTbl},
#ifdef CONFIG_SYS_PROTO_GVRP
    {CGI_CAT_GVRP,             cmdSetGvrpTbl},
#endif
#ifdef CONFIG_SYS_PROTO_GARP
	{CGI_CAT_GMRP,			   cmdSetGmrpTbl},
#endif
    {CGI_CAT_HOME,             cmdSetHomeTbl},
#ifdef CONFIG_SYS_PROTO_DAI
    {CGI_CAT_IPSG,             cmdSetIsgTbl},
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    {CGI_CAT_L3,               cmdSetL3Tbl},
#endif
    {CGI_CAT_LAG,              cmdSetLagTbl},
#ifdef CONFIG_SYS_PROTO_LLDP
    {CGI_CAT_LLDP,             cmdSetLldpTbl},
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
	{CGI_CMD_SET_ERPS_VAL,     cmdSetErpsTbl},
#endif
#ifdef CONFIG_SYS_SYSLOG
    {CGI_CAT_LOG,              cmdSetLogTbl},
#endif
    {CGI_CAT_MAC,              cmdSetMacTbl},
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    {CGI_CAT_MCAST,          cmdSetMcastTbl},
#endif
    {CGI_CAT_PORT,             cmdSetPortTbl},
#ifdef CONFIG_SYS_PORT_SECURITY
    {CGI_CAT_PSECURE,          cmdSetPsecureTbl},
#endif
    {CGI_CAT_QOS,              cmdSetQosTbl},
#ifdef CONFIG_SYS_LIB_RADIUS
    {CGI_CAT_RADIUS,           cmdSetRadiusTbl},
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    {CGI_CAT_RMON,             cmdSetRmonTbl},
#endif
#ifdef CONFIG_SYS_UI_SNMP
    {CGI_CAT_SNMP,             cmdSetSnmpTbl},
#endif
#ifdef CONFIG_SYS_STORM_CTRL
    {CGI_CAT_STORM,            cmdSetStormTbl},
#endif
#ifdef CONFIG_SYS_PROTO_STP
    {CGI_CAT_STP,              cmdSetStpTbl},
#endif
    {CGI_CAT_SYS,              cmdSetSysTbl},
#ifdef CONFIG_SYS_LIB_TACPLUS
    {CGI_CAT_TACPLUS,          cmdSetTacplusTbl},
#endif
    {CGI_CAT_TIME,             cmdSetTimeTbl},
#ifdef CONFIG_SYS_PROTO_UDLD
    {CGI_CAT_UDLD,             cmdSetUdldTbl},
#endif
    {CGI_CAT_VLAN,             cmdSetVlanTbl},
#ifdef CONFIG_SYS_MACVLAN
    {CGI_CAT_VLAN_MAC,         cmdSetVlanMacTbl},
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {CGI_CAT_VLAN_PROTOCOL,    cmdSetVlanProtocolTbl},
#endif
#ifdef CONFIG_SYS_IP_SUBNET
	{CGI_CAT_VLAN_IP_SUBNET,    cmdSetVlanIpSubnetTbl},
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {CGI_CAT_VLAN_SURVEILLANCE,cmdSetVlanSurveillanceTbl},
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    {CGI_CAT_VLAN_VOICE,       cmdSetVlanVoiceTbl},
#endif
#ifdef CONFIG_SYS_POE
	{CGI_CAT_POE,	   cmdSetPoeTbl},
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	{CGI_CAT_RIP,	   cmdSetRipTbl},
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	{CGI_CAT_OSPF,	   cmdSetOspfTbl},
#endif
#ifdef CONFIG_SYS_L3_VRRP
	{CGI_CAT_VRRP,	   cmdSetVrrpTbl},
#endif

    {NULL, NULL},
};

typedef struct cgi_errMsg_s
{
    char                *pMsg;
    struct cgi_errMsg_s *next;
} cgi_errMsg_t;

static cgi_errMsg_t *g_errMsgs = NULL;

void cgi_set_respGeneral_send(int32 ret)
{
    if (NULL != g_errMsgs)
    {
        cJSON *status = NULL;
        cJSON *errMsgs = NULL;
        cgi_errMsg_t *pErrMsg = g_errMsgs;

        status = cJSON_CreateObject();
        cJSON_AddStringToObject(status, "status", "error");
        cJSON_AddStringToObject(status, "msgType", "err_msgs");

        errMsgs = cJSON_CreateArray();

        do
        {
            cJSON_AddItemToArray(errMsgs, cJSON_CreateString(pErrMsg->pMsg));
            osal_free(pErrMsg->pMsg);

            pErrMsg = pErrMsg->next;
        } while(pErrMsg != NULL);

        cJSON_AddItemToObject(status, "errMsgs", errMsgs);

        CGI_RESPONSE_JSON(status);
        cJSON_Delete(status);

        return;
    }

    if (SYS_ERR_OK == ret || CGI_ERR_OK == ret)
    {
        cJSON *status = NULL;
        status = cJSON_CreateObject();

        cJSON_AddStringToObject(status, "status", "ok");
        cJSON_AddStringToObject(status, "msgType", "save_success");
        cJSON_AddStringToObject(status, "msg", "");

        CGI_RESPONSE_JSON(status);

        cJSON_Delete(status);
    }
    else if (CGI_ERR_ACT_SUCCESS == ret)
    {
        cJSON *status = NULL;
        status = cJSON_CreateObject();

        cJSON_AddStringToObject(status, "status", "ok");
        cJSON_AddStringToObject(status, "msgType", "success");

        CGI_RESPONSE_JSON(status);

        cJSON_Delete(status);
    }
    else
    {
        cJSON *status = NULL;
        char *errStr = NULL;
        char type[SYS_BUF16_LEN];

        SYS_MEM_CLEAR(type);

        status = cJSON_CreateObject();

        errStr = SYS_ERR_DES(ret);

        if(0 == osal_strlen(SYS_ERR_DES(ret)))
        {
            errStr = SYS_ERR_DES(ret);
            strlcpy(type, "sys_org", sizeof(type));
        }
        else
        {
            strlcpy(type, "user_lang", sizeof(type));
        }

        cJSON_AddStringToObject(status, "status", "error");
        cJSON_AddStringToObject(status, "msgType", type);
        if(0 == osal_strlen(SYS_ERR_DES(ret)))
            cJSON_AddStringToObject(status, "msg", "errFailed");
        else 
            cJSON_AddStringToObject(status, "msg", errStr);

        CGI_RESPONSE_JSON(status);

        cJSON_Delete(status);
    }
}

void cgi_set_respLangErrWithArg_send(char *langCat, char *langKey, char* fmt, ...)
{
    cJSON *status = NULL;
    char tmp[SYS_BUF512_LEN];
    char errStr[SYS_BUF512_LEN];
    static va_list args;

    SYS_MEM_CLEAR(tmp);
    SYS_MEM_CLEAR(errStr);

    va_start(args, fmt);
    vsprintf(tmp, fmt, args);
    va_end(args);

    osal_sprintf(errStr, "lang('%s','%s',[%s])", langCat, langKey, tmp);

    status = cJSON_CreateObject();

    cJSON_AddStringToObject(status, "status", "error");
    cJSON_AddStringToObject(status, "msgType", "user_lang");
    cJSON_AddStringToObject(status, "msg", errStr);

    CGI_RESPONSE_JSON(status);

    cJSON_Delete(status);
}

void cgi_set_respErrMsgs_add(char *msg)
{
    cgi_errMsg_t *pErrMsg = g_errMsgs;
    cgi_errMsg_t *pNewMsg = NULL;

    if (NULL == g_errMsgs)
    {
        g_errMsgs = osal_alloc(sizeof(cgi_errMsg_t));
        osal_memset(g_errMsgs, 0, sizeof(cgi_errMsg_t));
        g_errMsgs->pMsg = osal_alloc((osal_strlen(msg) + 1));
        osal_memset(g_errMsgs->pMsg, 0, (osal_strlen(msg) + 1));
        strlcpy(g_errMsgs->pMsg, msg, (osal_strlen(msg) + 1));

        return;
    }

    while (NULL != pErrMsg->next)
    {
        pErrMsg = pErrMsg->next;
    }

    pNewMsg = osal_alloc(sizeof(cgi_errMsg_t));
    osal_memset(pNewMsg, 0, sizeof(cgi_errMsg_t));
    pNewMsg->pMsg = osal_alloc((osal_strlen(msg) + 1));
    osal_memset(pNewMsg->pMsg, 0, (osal_strlen(msg) + 1));
    strlcpy(pNewMsg->pMsg, msg, (osal_strlen(msg) + 1));
    pErrMsg->next = pNewMsg;

    return;
}

int32 main(int argc, char* argv[], char* envp[])
{
    int32 ret = SYS_ERR_FAILED;
    uint32 i = 0;
    uint32 j = 0;
    char *cmd = NULL;

    cgi_util_paraPairs_init(TRUE);

    if (!CGI_IS_POSTURL_PARAM_EXIST(CGI_CMD, cmd))
        goto exit;

    if (0 == osal_strlen(cmd))
        goto exit;

    if (!CGI_IS_VALUE_EQUAL(cmd, CGI_CAT_HOME"_"CGI_CMD_HOME_LOGINAUTH))
    {
        if (CGI_SESSION_ACTIVE != cgi_util_session_check(TRUE))
            return 0;
    }

    i = 0;
    index(cmd, '_')[0] = '\0';
    while (NULL != catSetTbl[i].catstr)
    {
        if (0 == osal_strncmp(catSetTbl[i].catstr, cmd, osal_strlen(cmd)))
        {
            j = 0;
            while (NULL != catSetTbl[i].cmdmap[j].cmdstr)
            {
                if (0 == osal_strcmp(cmd + osal_strlen(catSetTbl[i].catstr) + 1, catSetTbl[i].cmdmap[j].cmdstr))
                {
                    cgi_util_paraPairs_init(FALSE);

                    ret = catSetTbl[i].cmdmap[j].cmdfunc();

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

