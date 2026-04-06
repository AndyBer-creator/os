
#include <include/cgi.h>

#include <libsal/sal_cablediag.h>
#ifdef CONFIG_SYS_TECH_SUPPORT
#include <libsal/sal_sys_tech.h>
#endif
#include <libsal/sal_fiber.h>
#include <libsal/sal_mirror.h>
#include <libclicmd/cmd_sys.h>

#ifdef CONFIG_SYS_CABLE_DIAG
static int32 cgi_diag_copper_set(void);
#endif
#ifdef CONFIG_SYS_MIRROR
static int32 cgi_diag_mirrorEdit_set(void);
#endif
#ifdef CONFIG_SYS_PING_TEST
static int32 cgi_diag_ping_set(void);
static int32 cgi_diag_pingAbort_set(void);
#endif
#ifdef CONFIG_SYS_TRACE_ROUTE
static int32 cgi_diag_traceroute_set(void);
static int32 cgi_diag_tracerouteAbort_set(void);
#endif
#ifdef CONFIG_SYS_TECH_SUPPORT
static int32 cgi_diag_techGen_set(void);
#endif

const cgi_cmdmap_t cmdSetDiagTbl[] =
{
#ifdef CONFIG_SYS_CABLE_DIAG
    { CGI_CMD_DIAG_COPPER,              cgi_diag_copper_set             },
#endif
#ifdef CONFIG_SYS_MIRROR
    { CGI_CMD_DIAG_MIRROREDIT,          cgi_diag_mirrorEdit_set         },
#endif
#ifdef CONFIG_SYS_PING_TEST
    { CGI_CMD_DIAG_PING,                cgi_diag_ping_set               },
    { CGI_CMD_DIAG_PING_ABORT,          cgi_diag_pingAbort_set          },
#endif
#ifdef CONFIG_SYS_TRACE_ROUTE
    { CGI_CMD_DIAG_TRACEROUTE,          cgi_diag_traceroute_set         },
    { CGI_CMD_DIAG_TRACEROUTE_ABORT,    cgi_diag_tracerouteAbort_set    },
#endif
#ifdef CONFIG_SYS_TECH_SUPPORT
    { CGI_CMD_DIAG_TECH_GEN,            cgi_diag_techGen_set            },
#endif
    { NULL,                             NULL                            }
};

#ifdef CONFIG_SYS_CABLE_DIAG
enum _cgi_copper_result_e
{
    CGI_COPPER_NORMAL = 0,
    CGI_COPPER_SHORT,
    CGI_COPPER_OPEN,
    CGI_COPPER_MISMATCH,
    CGI_COPPER_LINE_DRIVE,
    CGI_COPPER_END,
} _cgi_copper_result_t;

static int32 cgi_diag_copper_set(void)
{
    int32 ret = SYS_ERR_OK;
    int32 result = -1;
    uint32 length = 0;

    char *pStrValue = NULL;
    char str[SYS_BUF128_LEN];

    sys_logic_port_t lp = 0;
    sys_port_media_t media;
    sys_rtctResult_t rtctResult;

    cJSON *status = NULL;

    if (CGI_IS_PARAM_EXIST(CGI_PORT, pStrValue))
        lp = SYS_STR2UINT(pStrValue);
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_port_linkMedia_get(lp, &media), ret);

    if (PORT_MEDIA_COPPER == media)
    {
        CGI_SET_ERR_HDL(sal_cablediag_copper_set(lp), ret);

        ret = sal_cablediag_copper_get(lp, &rtctResult);

        if (SYS_ERR_OK != ret)
            goto cgi_end;

        if (PORT_SPEED_10M == rtctResult.linkType || PORT_SPEED_100M == rtctResult.linkType)
        {
            if (rtctResult.un.fe_result.isRxShort)
            {
                result = CGI_COPPER_SHORT;
                length = rtctResult.un.fe_result.rxLen;
            }
            else if (rtctResult.un.fe_result.isTxShort)
            {
                result = CGI_COPPER_SHORT;
                length = rtctResult.un.fe_result.txLen;
            }
            else if (rtctResult.un.fe_result.isRxOpen)
            {
                result = CGI_COPPER_OPEN;
                length = rtctResult.un.fe_result.rxLen;
            }
            else if (rtctResult.un.fe_result.isTxOpen)
            {
                result = CGI_COPPER_OPEN;
                length = rtctResult.un.fe_result.txLen;
            }
            else if (rtctResult.un.fe_result.isRxMismatch)
            {
                result = CGI_COPPER_MISMATCH;
                length = rtctResult.un.fe_result.rxLen;
            }
            else if (rtctResult.un.fe_result.isTxMismatch)
            {
                result = CGI_COPPER_MISMATCH;
                length = rtctResult.un.fe_result.txLen;
            }
            else if (rtctResult.un.fe_result.isRxLinedriver)
            {
                result = CGI_COPPER_LINE_DRIVE;
                length = rtctResult.un.fe_result.rxLen;
            }
            else if (rtctResult.un.fe_result.isTxLinedriver)
            {
                result = CGI_COPPER_LINE_DRIVE;
                length = rtctResult.un.fe_result.txLen;
            }
            else
            {
                result = CGI_COPPER_NORMAL;
                length = rtctResult.un.fe_result.txLen;
            }
        }
        else if (PORT_SPEED_1000M == rtctResult.linkType)
        {
            if (rtctResult.un.ge_result.channelAShort)     
            {
                result = CGI_COPPER_SHORT;
                length = rtctResult.un.ge_result.channelALen;
            }
            else if (rtctResult.un.ge_result.channelBShort)
            {
                result = CGI_COPPER_SHORT;
                length = rtctResult.un.ge_result.channelBLen;
            }
            else if (rtctResult.un.ge_result.channelCShort)
            {
                result = CGI_COPPER_SHORT;
                length = rtctResult.un.ge_result.channelCLen;
            }
            else if (rtctResult.un.ge_result.channelDShort)
            {
                result = CGI_COPPER_SHORT;
                length = rtctResult.un.ge_result.channelDLen;
            }
            else if (rtctResult.un.ge_result.channelAOpen) 
            {
                result = CGI_COPPER_OPEN;
                length = rtctResult.un.ge_result.channelALen;
            }
            else if (rtctResult.un.ge_result.channelBOpen)
            {
                result = CGI_COPPER_OPEN;
                length = rtctResult.un.ge_result.channelBLen;
            }
            else if (rtctResult.un.ge_result.channelCOpen)
            {
                result = CGI_COPPER_OPEN;
                length = rtctResult.un.ge_result.channelCLen;
            }
            else if (rtctResult.un.ge_result.channelDOpen)
            {
                result = CGI_COPPER_OPEN;
                length = rtctResult.un.ge_result.channelDLen;
            }
            else if (rtctResult.un.ge_result.channelAMismatch) 
            {
                result = CGI_COPPER_MISMATCH;
                length = rtctResult.un.ge_result.channelALen;
            }
            else if (rtctResult.un.ge_result.channelBMismatch)
            {
                result = CGI_COPPER_MISMATCH;
                length = rtctResult.un.ge_result.channelBLen;
            }
            else if (rtctResult.un.ge_result.channelCMismatch)
            {
                result = CGI_COPPER_MISMATCH;
                length = rtctResult.un.ge_result.channelCLen;
            }
            else if (rtctResult.un.ge_result.channelDMismatch)
            {
                result = CGI_COPPER_MISMATCH;
                length = rtctResult.un.ge_result.channelDLen;
            }
            else if (rtctResult.un.ge_result.channelALinedriver) 
            {
                result = CGI_COPPER_LINE_DRIVE;
                length = rtctResult.un.ge_result.channelALen;
            }
            else if (rtctResult.un.ge_result.channelBLinedriver)
            {
                result = CGI_COPPER_LINE_DRIVE;
                length = rtctResult.un.ge_result.channelBLen;
            }
            else if (rtctResult.un.ge_result.channelCLinedriver)
            {
                result = CGI_COPPER_LINE_DRIVE;
                length = rtctResult.un.ge_result.channelCLen;
            }
            else if (rtctResult.un.ge_result.channelDLinedriver)
            {
                result = CGI_COPPER_LINE_DRIVE;
                length = rtctResult.un.ge_result.channelDLen;
            }
            else
            {
                result = CGI_COPPER_NORMAL;
                length = rtctResult.un.ge_result.channelALen;
            }
        }
        else
        {
            ret = SYS_ERR_PORT_RTCT_NOT_COPPER;
        }
    }

    if (SYS_ERR_OK == ret)
    {
        status = cJSON_CreateObject();

        cJSON_AddStringToObject(status, "status", "ok");
        cJSON_AddStringToObject(status, "msgType", "save_success");

        switch (result)
        {
            case CGI_COPPER_NORMAL:
                snprintf(str, sizeof(str), "lblCopperNormal");
                break;

            case CGI_COPPER_SHORT:
                snprintf(str, sizeof(str), "lblCopperShort");
                break;

            case CGI_COPPER_OPEN:
                snprintf(str, sizeof(str), "lblCopperOpen");
                break;

            case CGI_COPPER_MISMATCH:
                snprintf(str, sizeof(str), "lblCopperMisMatch");
                break;

            case CGI_COPPER_LINE_DRIVE:
                snprintf(str, sizeof(str), "lblCopperLineDrive");
                break;

            default:
                snprintf(str, sizeof(str), "lblCopperNotTested");
                break;
        }

        cJSON_AddLangStringToObject(status, CGI_DIAG_COPPER_RESULT, "diag", str);

        if (CGI_COPPER_NORMAL < result && CGI_COPPER_END > result)
        {
            snprintf(str, sizeof(str), "%u.%u", length / 100, length % 100);
            cJSON_AddStringToObject(status, CGI_DIAG_COPPER_LENGTH, str);
        }

        CGI_RESPONSE_JSON(status);

        cJSON_Delete(status);

        return SYS_ERR_OK;
    }

cgi_end:

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_MIRROR
static int32 cgi_diag_mirrorEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 gId = 0;
    sys_enable_t state = DISABLED;
    sys_enable_t ingressState = DISABLED;
    sys_logic_port_t oriDstPort;
    sys_enable_t oriEnable;
    sys_logic_port_t     dstPort;
    sys_logic_portmask_t rxMbrs;
    sys_logic_portmask_t txMbrs;
    sys_logic_portmask_t emptyMbrs;

    LOGIC_PORTMASK_CLEAR_ALL(rxMbrs);
    LOGIC_PORTMASK_CLEAR_ALL(txMbrs);
    LOGIC_PORTMASK_CLEAR_ALL(emptyMbrs);

    if (CGI_IS_PARAM_EXIST(CGI_MIRROR_SESSION_SELECT, value))
    {
        gId = UGID2GID(SYS_STR2UINT(value));

        if (CGI_IS_PARAM_EXIST(CGI_MIRROR_SESSION_STATE, value))
            state = ENABLED;

        if (CGI_IS_PARAM_EXIST(CGI_MIRROR_SESSION_ALLOWIGRPKT, value))
            ingressState = ENABLED;

        if (CGI_IS_PARAM_EXIST(CGI_MIRROR_SESSION_DSTPORT, value))
            STR2LP(value, &dstPort);

        CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MIRROR_SESSION_RXMBRS, rxMbrs);
        CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MIRROR_SESSION_TXMBRS, txMbrs);

        sal_mirror_group_targetPort_get(gId,&oriDstPort,&oriEnable);

        CGI_SET_ERR_HDL(sal_mirror_group_igrState_set(gId, DISABLED), ret);
        CGI_SET_ERR_HDL(sal_mirror_group_targetPort_set(gId,oriDstPort,DISABLED), ret);

        if (ENABLED == state && !(IS_LOGIC_PORTMASK_CLEAR(rxMbrs) && IS_LOGIC_PORTMASK_CLEAR(txMbrs)))
        {
            CGI_SET_ERR_HDL(sal_mirror_group_targetPort_set(gId,dstPort,state), ret);
            CGI_SET_ERR_HDL(sal_mirror_group_igrState_set(gId,ingressState), ret);
        }

        if (ENABLED == state)
            CGI_SET_ERR_HDL(sal_mirror_group_sourcePort_set(gId,rxMbrs, txMbrs), ret);
        else
            CGI_SET_ERR_HDL(sal_mirror_group_sourcePort_set(gId, emptyMbrs, emptyMbrs), ret);

    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PING_TEST
static int32 cgi_diag_ping_set(void)
{
    int32 ret = SYS_ERR_OK;

    uint32 ipver = 4;
    uint32 count = 4;

    char *pStrValue = NULL;
    char ipaddr[CAPA_HOSTNAME_LEN + 1];
    char cmd[SYS_BUF512_LEN];

    FILE *fp = NULL;

    SYS_MEM_CLEAR(ipaddr);
    SYS_MEM_CLEAR(cmd);

    if (CGI_IS_PARAM_EXIST(CGI_DIAG_PING_NUM, pStrValue))
        count = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, pStrValue))
        ret = sal_util_hostname_resolv(pStrValue, ipaddr, sizeof(ipaddr));

    if (SYS_ERR_OK != ret)
        goto cgi_end;

    if (IS_IPV4_ADDRSTR_VALID(ipaddr))
    {
        ipver = 4;
    }
    else if (IS_IPV6_ADDRSTR_VALID(ipaddr))
    {
        ipver = 6;
    }
    else
    {
        ret = SYS_ERR_HOSTNAME;
        goto cgi_end;
    }

    fp = fopen(SYS_POLLD_ACT_WEB_PING, "w");

    if (NULL == fp)
    {
        ret = SYS_ERR_FAILED;
        goto cgi_end;
    }

    fprintf(fp, "handler -P %u %u %s &", ipver, count, ipaddr);

    fclose(fp);

    DO_POLLD_ACT_SIG();

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_diag_pingAbort_set(void)
{
    int32 ret = SYS_ERR_OK;

    sal_util_process_kill("ping -w");

    unlink(WEB_PING_LOCK_FILE);
    unlink(WEB_PING_RESULT_FILE);

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_TRACE_ROUTE
static int32 cgi_diag_traceroute_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 ttl = 30;
    char *pStrValue = NULL;
    char cmd[SYS_BUF512_LEN];

    FILE *fp = NULL;

    SYS_MEM_CLEAR(cmd);

    if (CGI_IS_PARAM_EXIST(CGI_DIAG_TRACE_TTL_USER, pStrValue))
    {
        if (CGI_IS_PARAM_EXIST(CGI_DIAG_TRACE_TTL, pStrValue))
            ttl = SYS_STR2UINT(pStrValue);
    }

    if (!CGI_IS_PARAM_EXIST("srvHost", pStrValue))
        goto cgi_end;

    fp = fopen(SYS_POLLD_ACT_WEB_TRACEROUTE, "w");

    if (NULL == fp)
    {
        ret = SYS_ERR_FAILED;
        goto cgi_end;
    }

    fprintf(fp, "handler -t %s %u &", pStrValue, ttl);

    fclose(fp);

    DO_POLLD_ACT_SIG();

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_diag_tracerouteAbort_set(void)
{
    int32 ret = SYS_ERR_OK;

    sal_util_process_kill("traceroute -v");

    sleep(1);

    unlink(SYS_TRACEROUTE_LOCK_FILE);
    unlink(SYS_TRACEROUTE_RESULT_FILE);

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_TECH_SUPPORT
static int32 cgi_diag_techGen_set(void)
{
    int32 ret = SYS_ERR_OK;
    FILE *fp = NULL;

    fp = fopen(SYS_POLLD_ACT_WEB_TECH_LOCK, "r");

    if (NULL != fp)
    {
        ret = SYS_ERR_FAILED;
        fclose(fp);
        goto cgi_end;
    }

    if (NULL != (fp = fopen(SYS_POLLD_ACT_TECH_SUPPORT_GEN, "w")))
    {
        fprintf(fp, "handler -f tech_support_gen &");
        fclose(fp);
    }
    else
    {
        ret = SYS_ERR_FAILED;
    }
    DO_POLLD_ACT_SIG();

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;

}
#endif

