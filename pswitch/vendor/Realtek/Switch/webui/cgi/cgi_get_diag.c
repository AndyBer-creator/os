
#include <sys/sysinfo.h>

#include <include/cgi.h>

#include <libsal/sal_cablediag.h>
#include <libsal/sal_fiber.h>
#include <libsal/sal_optical.h>
#include <libsal/sal_mirror.h>
#include <libsal/sal_sys.h>

#ifdef CONFIG_SYS_CABLE_DIAG
static int32 cgi_diag_copper_get(void);
#endif
#ifdef CONFIG_SYS_MIRROR
static int32 cgi_diag_mirror_get(void);
static int32 cgi_diag_mirrorEdit_get(void);
#endif
#ifdef CONFIG_SYS_PING_TEST
static int32 cgi_diag_ping_get(void);
static int32 cgi_diag_pingResult_get(void);
static int32 cgi_diag_pingRunning_get(void);
#endif
#ifdef CONFIG_SYS_TRACE_ROUTE
static int32 cgi_diag_traceroute_get(void);
static int32 cgi_diag_tracerouteResult_get(void);
static int32 cgi_diag_tracerouteRunning_get(void);
#endif
#ifdef CONFIG_SYS_OPTICALMOD_STATUS
static int32 cgi_diag_optical_get(void);
static int32 cgi_diag_opticalIntf_get(void);
#endif
#ifdef CONFIG_SYS_TECH_SUPPORT
static int32 cgi_diag_techGenStatus_get(void);
#endif

const cgi_cmdmap_t cmdGetDiagTbl[] =
{
#ifdef CONFIG_SYS_CABLE_DIAG
    { CGI_CMD_DIAG_COPPER,              cgi_diag_copper_get             },
#endif
#ifdef CONFIG_SYS_MIRROR
    { CGI_CMD_DIAG_MIRROR,              cgi_diag_mirror_get             },
    { CGI_CMD_DIAG_MIRROREDIT,          cgi_diag_mirrorEdit_get         },
#endif
#ifdef CONFIG_SYS_PING_TEST
    { CGI_CMD_DIAG_PING,                cgi_diag_ping_get               },
    { CGI_CMD_DIAG_PING_RESULT,         cgi_diag_pingResult_get         },
    { CGI_CMD_DIAG_PING_RUNNING,        cgi_diag_pingRunning_get        },
#endif
#ifdef CONFIG_SYS_TRACE_ROUTE
    { CGI_CMD_DIAG_TRACEROUTE,          cgi_diag_traceroute_get         },
    { CGI_CMD_DIAG_TRACEROUTE_RESULT,   cgi_diag_tracerouteResult_get   },
    { CGI_CMD_DIAG_TRACEROUTE_RUNNING,  cgi_diag_tracerouteRunning_get  },
#endif
#ifdef CONFIG_SYS_OPTICALMOD_STATUS
    { CGI_CMD_DIAG_OPTICAL,             cgi_diag_optical_get            },
    { CGI_CMD_DIAG_OPTICAL_INTF,        cgi_diag_opticalIntf_get        },
#endif
#ifdef CONFIG_SYS_TECH_SUPPORT
    { CGI_CMD_DIAG_TECH_GEN_STATUS,     cgi_diag_techGenStatus_get      },
#endif

    { NULL,                             NULL                            }
};

#ifdef CONFIG_SYS_CABLE_DIAG
static int32 cgi_diag_copper_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
	cJSON *ports = NULL;
	cJSON *port = NULL;
	char *value = NULL;
	sys_logic_port_t selPort = 0;
	sys_logic_port_t lp;
	char descp[CAPA_PORTDESC_STR_LEN + 1];
	sys_port_media_t media;

    CGI_GET_JSON_CREATE(root, data);

    if (CGI_IS_PARAM_EXIST(CGI_SEL_PORT, value))
        selPort = SYS_STR2UINT(value);

	cJSON_AddNumberToObject(data, CGI_SEL_PORT, selPort);
    ports = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_PORTS, ports);
	FOR_EACH_NORMAL_LOGIC_PORT(lp)
	{
        sal_port_linkMedia_get(lp, &media);

        if (PORT_MEDIA_COPPER != media)
            continue;

		port = cJSON_CreateObject();
		cJSON_AddItemToArray(ports, port);
		cJSON_AddNumberToObject(port, CGI_VALUE, lp);
		sys_port_lPort2uStr(lp, SYS_PORT_STRFMT_WEB, descp);
        cJSON_AddStringToObject(port, CGI_NAME, descp);
	}

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_MIRROR
static int32 cgi_diag_mirror_get(void)
{
    cJSON                    *root = NULL;
    cJSON                    *data = NULL;
    cJSON                    *sessions = NULL;
    cJSON                    *session = NULL;
    sys_gid_t                 gid = 0;
    sys_mirror_state_t        state;
    sys_logic_portmask_t      mirRxPm,mirTxPm;
    sys_logic_port_t          dstPort;
    sys_enable_t              enable;
    sys_enable_t              igrState;
    char                      dstPortStr[CAPA_PORT_STR_LEN + 1];
    char                      rxPmStr[CAPA_PORTMASK_STR_LEN + 1];
    char                      txPmStr[CAPA_PORTMASK_STR_LEN + 1];

    CGI_GET_JSON_CREATE(root, data);

    sessions = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_MIRROR_SESSIONS, sessions);

    FOR_EACH_GID(gid)
    {
         LOGIC_PORTMASK_CLEAR_ALL(mirRxPm);
         LOGIC_PORTMASK_CLEAR_ALL(mirTxPm);

         sal_mirror_group_igrState_get(gid,&igrState);
         sal_mirror_group_state_get(gid,&state);
         sal_mirror_group_targetPort_get(gid,&dstPort,&enable);
         sal_mirror_group_sourcePort_get(gid,&mirRxPm,&mirTxPm);

         LP2WEBSTR(dstPort, dstPortStr);
         LPM2WEBSTR(&mirRxPm, rxPmStr);
         LPM2WEBSTR(&mirTxPm, txPmStr);

        session = cJSON_CreateObject();

        if(PORTBASED_ENABLED == state)
            cJSON_AddLangStringToObject(session, CGI_MIRROR_SESSION_STATE, "common", "txtEnabled");
        else
            cJSON_AddLangStringToObject(session, CGI_MIRROR_SESSION_STATE, "common", "txtDisabled");

        if(ENABLED == igrState)
            cJSON_AddLangStringToObject(session, CGI_MIRROR_SESSION_ALLOWIGRPKT, "mirror", "lblMirrorIngressState");
        else
            cJSON_AddStringToObject(session, CGI_MIRROR_SESSION_ALLOWIGRPKT,"");

        if(PORTBASED_ENABLED == state)
        {
            cJSON_AddStringToObject(session, CGI_MIRROR_SESSION_RXPORTLIST,rxPmStr);
            cJSON_AddStringToObject(session, CGI_MIRROR_SESSION_TXPORTLIST,txPmStr);
            cJSON_AddStringToObject(session, CGI_MIRROR_SESSION_DSTPORT,dstPortStr);
        }
        else
        {
            cJSON_AddStringToObject(session, CGI_MIRROR_SESSION_RXPORTLIST,"---");
            cJSON_AddStringToObject(session, CGI_MIRROR_SESSION_TXPORTLIST,"---");
            cJSON_AddStringToObject(session, CGI_MIRROR_SESSION_DSTPORT,"---");
        }

        cJSON_AddItemToArray(sessions, session);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_diag_mirrorEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *port = NULL;
    cJSON *notMbrs = NULL;
    cJSON *mbrs = NULL;
    sys_logic_port_t lp;
    sys_gid_t                 gId = 0;
    sys_mirror_state_t        state;
    sys_logic_portmask_t      mirRxPm,mirTxPm;
    sys_logic_portmask_t      mirRxNotPm,mirTxNotPm;
    sys_logic_port_t          dstPort;
    sys_enable_t              enable;
    sys_enable_t              igrState;
    char *value = NULL;
    char                      dstPortStr[CAPA_PORT_STR_LEN + 1];
    char                      portStr[CAPA_PORT_STR_LEN + 1];

    LOGIC_PORTMASK_CLEAR_ALL(mirRxPm);
    LOGIC_PORTMASK_CLEAR_ALL(mirTxPm);
    LOGIC_PORTMASK_CLEAR_ALL(mirRxNotPm);
    LOGIC_PORTMASK_CLEAR_ALL(mirTxNotPm);

    if (CGI_IS_PARAM_EXIST(CGI_MIRROR_SESSION_MIRROR, value))
    {
        gId = UGID2GID(SYS_STR2UINT(value));
    }

    sal_mirror_group_state_get(gId,&state);
    sal_mirror_group_igrState_get(gId,&igrState);
    sal_mirror_group_targetPort_get(gId,&dstPort,&enable);
    sal_mirror_group_sourcePort_get(gId,&mirRxPm,&mirTxPm);

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (!IS_LOGIC_PORTMASK_PORTSET(mirRxPm, lp))
            LOGIC_PORTMASK_SET_PORT(mirRxNotPm, lp);

        if (!IS_LOGIC_PORTMASK_PORTSET(mirTxPm, lp))
            LOGIC_PORTMASK_SET_PORT(mirTxNotPm, lp);
    }

    LP2WEBSTR(dstPort, dstPortStr);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_MIRROR_SESSION_STATE,state);
    cJSON_AddBoolToObject(data, CGI_MIRROR_SESSION_ALLOWIGRPKT,igrState);
    cJSON_AddStringToObject(data, CGI_MIRROR_SESSION_DSTPORT,dstPortStr);

    if (!IS_LOGIC_PORTMASK_CLEAR(mirRxNotPm))
    {
        notMbrs = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_MIRROR_SESSION_RXNOTMBRS, notMbrs);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, mirRxNotPm)
        {
            SYS_MEM_CLEAR(portStr);
            LP2WEBSTR(lp, portStr);

            port = cJSON_CreateObject();
            cJSON_AddStringToObject(port, CGI_MIRROR_SESSION_PORT, portStr);
            cJSON_AddItemToArray(notMbrs, port);
        }
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(mirRxPm))
    {
        mbrs = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_MIRROR_SESSION_RXMBRS, mbrs);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, mirRxPm)
        {
            SYS_MEM_CLEAR(portStr);
            LP2WEBSTR(lp, portStr);

            port = cJSON_CreateObject();
            cJSON_AddStringToObject(port, CGI_MIRROR_SESSION_PORT, portStr);
            cJSON_AddItemToArray(mbrs, port);
        }
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(mirTxNotPm))
    {
        notMbrs = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_MIRROR_SESSION_TXNOTMBRS, notMbrs);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, mirTxNotPm)
        {
            SYS_MEM_CLEAR(portStr);
            LP2WEBSTR(lp, portStr);

            port = cJSON_CreateObject();
            cJSON_AddStringToObject(port, CGI_MIRROR_SESSION_PORT, portStr);
            cJSON_AddItemToArray(notMbrs, port);
        }
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(mirTxPm))
    {
        mbrs = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_MIRROR_SESSION_TXMBRS, mbrs);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, mirTxPm)
        {
            SYS_MEM_CLEAR(portStr);
            LP2WEBSTR(lp, portStr);

            port = cJSON_CreateObject();
            cJSON_AddStringToObject(port, CGI_MIRROR_SESSION_PORT, portStr);
            cJSON_AddItemToArray(mbrs, port);
        }
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#endif

#ifdef CONFIG_SYS_PING_TEST
static int32 cgi_diag_ping_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_DIAG_PING_RUNNING,
                          (SYS_ERR_OK == sal_util_file_exist(WEB_PING_LOCK_FILE)));

    cJSON_AddNumberToObject(data, CGI_DIAG_REMOTE_HOST_MAXLEN, CAPA_HOSTNAME_LEN);
    cJSON_AddNumberToObject(data, CGI_DIAG_PING_NUM_MIN, 1);
    cJSON_AddNumberToObject(data, CGI_DIAG_PING_NUM_MAX, 32);
    cJSON_AddNumberToObject(data, CGI_DIAG_PING_NUM_DFLT, 4);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_diag_pingResult_get(void)
{
    FILE *fp = NULL;
    cJSON *root = NULL;
    cJSON *data = NULL;

    uint32 rx = 0;
    uint32 tx = 0;
    uint32 loss = 0;
    uint32 min = 0;
    uint32 avg = 0;
    uint32 max = 0;
    uint32 status = 1;

    char buf[SYS_BUF1024_LEN];
    char tmp[SYS_BUF32_LEN];
    char *val;

    CGI_GET_JSON_CREATE(root, data);

    if (SYS_ERR_OK == sal_util_file_exist(WEB_PING_LOCK_FILE))
    {
        cJSON_AddBoolToObject(data, CGI_DIAG_PING_RUNNING, TRUE);
        cJSON_AddStringToObject(data, "empty", "''");
    }
    else
    {
        cJSON_AddBoolToObject(data, CGI_DIAG_PING_RUNNING, FALSE);

        fp = fopen(WEB_PING_RESULT_FILE, "r");

        if (fp)
        {
            while ((fgets(buf, sizeof(buf), fp)) != NULL)
            {
                buf[strlen(buf)-1] = '\0';

                if (NULL != strstr(buf, "transmitted"))
                {
                    sscanf(buf, "%d %*s %*s %d %*s %*s %d %*s %*s", &tx, &rx, &loss);
                }
                else if (NULL != strstr(buf, "round-trip"))
                {
                    memset(tmp, '\0', sizeof(tmp));
                    sscanf(buf, "%*s %*s %*s %s %*s", tmp);

                    val = strtok(tmp, "/");
                    min = atoi(val);

                    val = strtok(NULL, "/");
                    avg = atoi(val);

                    val = strtok(NULL, "/");
                    max = atoi(val);

                    status = 0;
                }

            }
            fclose(fp);

            cJSON_AddNumberToObject(data, CGI_DIAG_PING_PKT_TX, tx);
            cJSON_AddNumberToObject(data, CGI_DIAG_PING_PKT_RX, rx);
            cJSON_AddNumberToObject(data, CGI_DIAG_PING_PKT_LOSS, loss);
            cJSON_AddNumberToObject(data, CGI_DIAG_PING_TIME_MIN, min);
            cJSON_AddNumberToObject(data, CGI_DIAG_PING_TIME_MAX, max);
            cJSON_AddNumberToObject(data, CGI_DIAG_PING_TIME_AVG, avg);
            cJSON_AddNumberToObject(data, CGI_DIAG_PING_STATUS, status);
        }

        unlink(WEB_PING_RESULT_FILE);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_diag_pingRunning_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_DIAG_PING_RUNNING,
                          (SYS_ERR_OK == sal_util_file_exist(WEB_PING_LOCK_FILE)));

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_TRACE_ROUTE
static int32 cgi_diag_traceroute_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_DIAG_REMOTE_HOST_MAXLEN, CAPA_HOSTNAME_LEN);
    cJSON_AddNumberToObject(data, CGI_DIAG_TRACE_TTL, 30);
    cJSON_AddNumberToObject(data, CGI_DIAG_TRACE_TTL_MIN, 2);
    cJSON_AddNumberToObject(data, CGI_DIAG_TRACE_TTL_MAX, 255);
    cJSON_AddNumberToObject(data, CGI_DIAG_TRACE_TTL_DFLT, 30);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_diag_tracerouteResult_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    int32 r = 0;
    int32 offset = 0;

    FILE *fp = NULL;
    char result[SYS_BUF2048_LEN];
    char *pStrValue = NULL;

    SYS_MEM_CLEAR(result);

    if (CGI_IS_PARAM_EXIST(CGI_DIAG_TRACE_RESULT_LEN, pStrValue))
        offset = SYS_STR2UINT(pStrValue);

    CGI_GET_JSON_CREATE(root, data);

    fp = fopen(SYS_TRACEROUTE_RESULT_FILE, "r");

    if (fp)
    {
        fseek(fp, offset, SEEK_SET);
        r = fread(result, 1, 2047, fp);

        fclose(fp);

        cJSON_AddStringToObject(data, CGI_DIAG_TRACE_RESULT, result);
        cJSON_AddNumberToObject(data, CGI_DIAG_TRACE_RESULT_LEN, r);
    }
    else
    {
        cJSON_AddStringToObject(data, "empty", "''");
    }

    if (SYS_ERR_OK == sal_util_file_exist(SYS_TRACEROUTE_LOCK_FILE))
    {
        cJSON_AddBoolToObject(data, CGI_DIAG_TRACE_RUNNING, TRUE);
    }
    else
    {
        cJSON_AddBoolToObject(data, CGI_DIAG_TRACE_RUNNING, FALSE);
        cJSON_AddStringToObject(data, CGI_DIAG_TRACE_RESULT, "Trace complete");
        cJSON_AddNumberToObject(data, CGI_DIAG_TRACE_RESULT_LEN, 14);
        unlink(SYS_TRACEROUTE_RESULT_FILE);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_diag_tracerouteRunning_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_DIAG_TRACE_RUNNING,
                          (SYS_ERR_OK == sal_util_file_exist(SYS_TRACEROUTE_LOCK_FILE)));

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_OPTICALMOD_STATUS
static int32 cgi_diag_optical_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;

    char str[SYS_BUF128_LEN];

    sys_logic_port_t lp;
    sys_port_media_t media;
    sys_optical_diagMeasure_t diagMeasure;
    sys_fiber_detect_status_t detectStatus;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        media = PORT_MEDIA_COPPER;

        sal_port_linkMedia_get(lp, &media);

        if (PORT_MEDIA_FIBER != media)
            continue;

        port = cJSON_CreateObject();

        cJSON_AddNumberToObject(port, CGI_DIAG_OPTICAL_PORT, lp);

        if (SYS_ERR_OK == sal_optical_diagMeasure_get(lp, &diagMeasure))
        {
            if (DISABLED == diagMeasure.enable)
            {
                cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_TEMP, "diag", "lblNS");
                cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_VOLT, "diag", "lblNS");
                cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_BIAS, "diag", "lblNS");
                cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_TX_PWR, "diag", "lblNS");
                cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_RX_PWR, "diag", "lblNS");
            }
            else
            {
                snprintf(str, sizeof(str), "%6.2f", diagMeasure.temp * 0.00391f);
                cJSON_AddStringToObject(port, CGI_DIAG_OPTICAL_TEMP, str);

                snprintf(str, sizeof(str), "%5.2f", diagMeasure.volt * 0.0001f);
                cJSON_AddStringToObject(port, CGI_DIAG_OPTICAL_VOLT, str);

                snprintf(str, sizeof(str), "%5.2f", diagMeasure.bias * 0.002f);
                cJSON_AddStringToObject(port, CGI_DIAG_OPTICAL_BIAS, str);

                snprintf(str, sizeof(str), "%5.2f", diagMeasure.txPwr * 0.0001f);
                cJSON_AddStringToObject(port, CGI_DIAG_OPTICAL_TX_PWR, str);

                snprintf(str, sizeof(str), "%5.2f", diagMeasure.rxPwr* 0.0001f);
                cJSON_AddStringToObject(port, CGI_DIAG_OPTICAL_RX_PWR, str);
            }
        }
        else
        {
            cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_TEMP, "diag", "lblNA");
            cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_VOLT, "diag", "lblNA");
            cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_BIAS, "diag", "lblNA");
            cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_TX_PWR, "diag", "lblNA");
            cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_RX_PWR, "diag", "lblNA");
        }

        if (SYS_ERR_OK == sal_fiber_oeStatus_get(lp, &detectStatus))
        {
            SYS_MEM_CLEAR(str);

            switch (detectStatus)
            {
                case FIBER_DETECT_STATUS_OFF:
                    snprintf(str, sizeof(str), "lblRemove");
                    break;

                case FIBER_DETECT_STATUS_ON:
                    snprintf(str, sizeof(str), "lblInsert");
                    break;

                default:
                    snprintf(str, sizeof(str),  "lblNS");
                    break;
            }
        }
        else
        {
            snprintf(str, sizeof(str), "lblNA");
        }

        cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_OE, "diag", str);

        if (SYS_ERR_OK == sal_fiber_losStatus_get(lp, &detectStatus))
        {
           SYS_MEM_CLEAR(str);

           switch (detectStatus)
            {
                case FIBER_DETECT_STATUS_OFF:
                    snprintf(str, sizeof(str), "lblLoss");
                    break;

                case FIBER_DETECT_STATUS_ON:
                    snprintf(str, sizeof(str), "lblNormal");
                    break;

                default:
                    snprintf(str, sizeof(str), "lblNS");
                    break;
            }
        }
        else
        {
            snprintf(str, sizeof(str), "lblNA");
        }

        cJSON_AddLangStringToObject(port, CGI_DIAG_OPTICAL_LOS, "diag", str);

        cJSON_AddItemToArray(ports, port);
    }

    if (0 == cJSON_GetArraySize(ports))
    {
        cJSON_Delete(ports);
        ports = cJSON_CreateNull();
    }

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_diag_opticalIntf_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    char str[SYS_BUF128_LEN];
    char *pStrValue = NULL;

    sys_logic_port_t lp = 0;
    sys_port_media_t media;

    uint32 bitrate;
    uint32 wavelength;
    sys_optical_trxType_t trxType;
    sys_optical_vendorInfo_t vendor;
    sys_optical_trxCapacity_t trxCapacity;
    sys_optical_connectType_t connectType;
    sys_optical_diagMeasure_t diagMeasure;
    sys_fiber_detect_status_t detectStatus;

    if (CGI_IS_PARAM_EXIST(CGI_PORT, pStrValue))
        lp = SYS_STR2UINT(pStrValue);

    media = PORT_MEDIA_COPPER;

    sal_port_linkMedia_get(lp, &media);

    CGI_GET_JSON_CREATE(root, data);

    if (PORT_MEDIA_FIBER != media)
    {
        cJSON_AddStringToObject(data, "empty", "''");
        goto cgi_end;
    }

    snprintf(str, sizeof(str), "lblNA");

    if (SYS_ERR_OK == sal_fiber_losStatus_get(lp, &detectStatus))
    {
       SYS_MEM_CLEAR(str);

       switch (detectStatus)
        {
            case FIBER_DETECT_STATUS_OFF:
                snprintf(str, sizeof(str), "lblLoss");
                break;

            case FIBER_DETECT_STATUS_ON:
                snprintf(str, sizeof(str), "lblNormal");
                break;

            default:
                snprintf(str, sizeof(str), "lblNS");
                break;
        }
    }

    cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_LOS, "diag", str);

    snprintf(str, sizeof(str), "lblNA");

    if (SYS_ERR_OK == sal_fiber_oeStatus_get(lp, &detectStatus))
    {
        SYS_MEM_CLEAR(str);

        switch (detectStatus)
        {
            case FIBER_DETECT_STATUS_OFF:
                snprintf(str, sizeof(str), "lblRemove");
                break;

            case FIBER_DETECT_STATUS_ON:
                snprintf(str, sizeof(str), "lblInsert");
                break;

            default:
                snprintf(str, sizeof(str), "lblNS");
                break;
        }
    }

    cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_OE, "diag", str);

    if (SYS_ERR_OK == sal_optical_trxType_get(lp, &trxType))
    {
        cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_TRANSCEIVER_TYPE,
                                text_optical_trxType[trxType].text);
    }
    else
    {
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_TRANSCEIVER_TYPE,
                                    "diag", "lblNA");
    }

    if (SYS_ERR_OK == sal_optical_connectType_get(lp, &connectType))
    {
        cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_CONNECTOR_TYPE,
                                text_optical_connectType[connectType].text);
    }
    else
    {
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_CONNECTOR_TYPE,
                                    "diag", "lblNA");
    }

    if (SYS_ERR_OK == sal_optical_trxCapacity_get(lp, &trxCapacity))
    {
        cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_ETH_COMPLIANCE_CODE,
                                text_optical_ethCompliance[trxCapacity.ethCompliance].text);
        cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_TRANSMISSION_MEDIA,
                                text_optical_trxMedia[trxCapacity.txMedia].text);
    }
    else
    {
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_ETH_COMPLIANCE_CODE,
                                    "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_TRANSMISSION_MEDIA,
                                    "diag", "lblNA");
    }

    if (SYS_ERR_OK == sal_optical_wavelength_get(lp, &wavelength))
    {
        if (wavelength)
        {
            snprintf(str, sizeof(str), "%u (nm)", wavelength);
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_WAVELENGTH, str);
        }
        else
        {
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_WAVELENGTH,
                                        "diag", "lblNS");
        }
    }
    else
    {
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_WAVELENGTH,
                                    "diag", "lblNA");
    }

    if (SYS_ERR_OK == sal_optical_bitRateNominal_get(lp, &bitrate))
    {
        if (bitrate)
        {
            snprintf(str, sizeof(str), "%u Mbps", bitrate);
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_BITRATE, str);
        }
        else
        {
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_BITRATE,
                                        "diag", "lblNS");
        }
    }
    else
    {
       cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_BITRATE,
                                   "diag", "lblNA");
    }

    SYS_MEM_CLEAR(vendor);

    if (SYS_ERR_OK == sal_optical_vendorInfo_get(lp, &vendor))
    {
        if (vendor.oui)
        {
            snprintf(str, sizeof(str), "%02x-%02x-%02x",
                     (vendor.oui >> 16) & 0xFF,
                     (vendor.oui >> 8) & 0xFF,
                     vendor.oui & 0xFF);
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_OUI, str);
        }
        else
        {
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_OUI,
                                        "diag", "lblNS");
        }

        if (strlen(vendor.name))
        {
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_NAME, vendor.name);
        }
        else
        {
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_NAME,
                                       "diag", "lblNS");
        }

        if (strlen(vendor.pn))
        {
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_PN, vendor.pn);
        }
        else
        {
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_PN,
                                       "diag", "lblNS");
        }

        if (strlen(vendor.rev))
        {
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_REVISION, vendor.rev);
        }
        else
        {
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_REVISION,
                                       "diag", "lblNS");
        }

        if (strlen(vendor.sn))
        {
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_SN, vendor.sn);
        }
        else
        {
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_SN,
                                       "diag", "lblNS");
        }

        if (strlen(vendor.date))
        {
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_DATE_CODE, vendor.date);
        }
        else
        {
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_DATE_CODE,
                                       "diag", "lblNS");
        }
    }
    else
    {
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_OUI,
                                    "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_NAME,
                                    "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_PN,
                                    "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_REVISION,
                                    "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VENDOR_SN,
                                    "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_DATE_CODE,
                                    "diag", "lblNA");
    }

    SYS_MEM_CLEAR(diagMeasure);

    if (SYS_ERR_OK == sal_optical_diagMeasure_get(lp, &diagMeasure))
    {
        if (DISABLED == diagMeasure.enable)
        {
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_TEMP, "diag", "lblNS");
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VOLT, "diag", "lblNS");
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_BIAS, "diag", "lblNS");
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_TX_PWR, "diag", "lblNS");
            cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_RX_PWR, "diag", "lblNS");
        }
        else
        {
            snprintf(str, sizeof(str), "%6.2f", diagMeasure.temp * 0.00391f);
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_TEMP, str);

            snprintf(str, sizeof(str), "%5.2f", diagMeasure.volt * 0.0001f);
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_VOLT, str);

            snprintf(str, sizeof(str), "%5.2f", diagMeasure.bias * 0.002f);
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_BIAS, str);

            snprintf(str, sizeof(str), "%5.2f", diagMeasure.txPwr * 0.0001f);
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_TX_PWR, str);

            snprintf(str, sizeof(str), "%5.2f", diagMeasure.rxPwr* 0.0001f);
            cJSON_AddStringToObject(data, CGI_DIAG_OPTICAL_RX_PWR, str);
        }
    }
    else
    {
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_TEMP, "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_VOLT, "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_BIAS, "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_TX_PWR, "diag", "lblNA");
        cJSON_AddLangStringToObject(data, CGI_DIAG_OPTICAL_RX_PWR, "diag", "lblNA");
    }

cgi_end:

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_TECH_SUPPORT
static int32 cgi_diag_techGenStatus_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    FILE *fp = NULL;
    char buf[SYS_BUF256_LEN];
    char fullname[CAPA_FILE_FULLNAME_LEN];
    char filename_web[CAPA_FILE_NAME_LEN];
    char fullname_web[CAPA_FILE_FULLNAME_LEN];
    char fullname_web_real[CAPA_FILE_FULLNAME_LEN];

    CGI_GET_JSON_CREATE(root, data);

    if((fp = fopen(CONF_TECH_GEN_FAIL, "r")) != NULL)
    {
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);
        fclose(fp);
        unlink(CONF_TECH_GEN_FAIL);

        cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_FAIL);
        cJSON_AddStringToObject(data, CGI_FILE_STATUS_MESSAGE, buf);
    }
    else if ((fp = fopen(CONF_TECH_GEN_DONE, "r")) != NULL)
    {
        fclose(fp);
        unlink(CONF_TECH_GEN_DONE);

        SYS_MEM_CLEAR(fullname);
        SYS_MEM_CLEAR(filename_web);
        SYS_MEM_CLEAR(fullname_web);
        SYS_MEM_CLEAR(fullname_web_real);

        osal_sprintf(fullname, "%s", CONF_TECH_SUPPORT_FILE);
        osal_sprintf(filename_web, "%s", CONF_TECH_SUPPORT_NAME);
        osal_sprintf(fullname_web, "%s/%s", WEB_TMP_DIR, filename_web);
        osal_sprintf(fullname_web_real, "%s/%s", WEB_TMP_REAL_DIR, filename_web);

        if (SYS_ERR_OK == sal_util_file_exist(fullname))
        {
          symlink(fullname, fullname_web_real);

          cJSON_AddStringToObject(data, "filename", fullname_web);
        }
        else
        {
          cJSON_AddStringToObject(data, "msg", "File not exist");
        }

        cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_SUCCESS);
    }
    else
    {
        cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_DOWNLOADING);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;

}
#endif

