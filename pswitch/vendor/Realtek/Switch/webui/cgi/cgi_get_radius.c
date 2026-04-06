
#include <include/cgi.h>

static int32 cgi_radius_get(void);
static int32 cgi_radiusAdd_get(void);
static int32 cgi_radiusEdit_get(void);

const cgi_cmdmap_t cmdGetRadiusTbl[] =
{
    {CGI_CMD_RADIUS,        cgi_radius_get},
    {CGI_CMD_RADIUSADD,     cgi_radiusAdd_get},
    {CGI_CMD_RADIUSEDIT,    cgi_radiusEdit_get},
    {NULL, NULL}
};

static int32 cgi_radius_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;
    char                strBuf[CAPA_AAA_KEY_LEN + 1];
    cJSON               *srvs = NULL;
    cJSON               *srv = NULL;
    sys_radius_t        radiusSrv;
    uint32              i;
    uint32              cnt = 0;

    SYS_MEM_CLEAR(radiusSrv);

    CGI_GET_JSON_CREATE(root, data);

    sal_radiusSrv_dftParam_get(&radiusSrv);

    cJSON_AddStringToObject(data, CGI_RADIUS_KEY, radiusSrv.key);
    cJSON_AddNumberToObject(data, CGI_RADIUS_MAXLEN_KEY, CAPA_AAA_KEY_LEN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY, radiusSrv.retransmit);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY_MIN, BOND_RADIUS_RETRY_MIN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY_MAX, BOND_RADIUS_RETRY_MAX);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY_DFLT, DFLT_RADIUS_RETRY);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT, radiusSrv.timeout);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT_MIN, BOND_RADIUS_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT_MAX, BOND_RADIUS_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT_DFLT, DFLT_RADIUS_TIMEOUT);

    srvs = cJSON_CreateArray();

    for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
    {
        sal_radiusSrv_get(i, &radiusSrv);
        if (RADIUS_NOT_UESD_FLAG == radiusSrv.priority)
            continue;

        srv = cJSON_CreateObject();

        cJSON_AddStringToObject(srv, CGI_SRVHOST, radiusSrv.hostName);
        cJSON_AddNumberToObject(srv, CGI_RADIUS_PRI, radiusSrv.priority);

        sal_util_str_encrypt(radiusSrv.key, strBuf);
        cJSON_AddStringToObject(srv, CGI_RADIUS_KEY, strBuf);

        cJSON_AddNumberToObject(srv, CGI_RADIUS_TIMEOUT, radiusSrv.timeout);
        cJSON_AddNumberToObject(srv, CGI_RADIUS_AUTHPORT, radiusSrv.srvPort);
        cJSON_AddNumberToObject(srv, CGI_RADIUS_RETRY, radiusSrv.retransmit);
        if (SYS_RADIUS_SRV_USGTYPE_LOGIN == radiusSrv.usgType)
            cJSON_AddStringToObject(srv, CGI_RADIUS_USGTYPE, CGI_LANG("radius", "lblTypeLogin"));
        else if (SYS_RADIUS_SRV_USGTYPE_8021X == radiusSrv.usgType)
            cJSON_AddStringToObject(srv, CGI_RADIUS_USGTYPE, CGI_LANG("radius", "lblTypeDot1x"));
        else if (SYS_RADIUS_SRV_USGTYPE_ALL == radiusSrv.usgType)
            cJSON_AddStringToObject(srv, CGI_RADIUS_USGTYPE, CGI_LANG("radius", "lblTypeAll"));

        cJSON_AddItemToArray(srvs , srv);
        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_RADIUS_SERVERS, srvs);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_radiusAdd_get(void)
{
    cJSON        *root = NULL;
    cJSON        *data = NULL;
    sys_radius_t radiusSrv;

    CGI_GET_JSON_CREATE(root, data);

    sal_radiusSrv_dftParam_get(&radiusSrv);

    cJSON_AddStringToObject(data, CGI_RADIUS_KEY, radiusSrv.key);
    cJSON_AddNumberToObject(data, CGI_RADIUS_MAXLEN_KEY, CAPA_AAA_KEY_LEN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_PRI_MIN, BOND_RADIUS_PRI_MIN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_PRI_MAX, BOND_RADIUS_PRI_MAX);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY_MIN, BOND_RADIUS_RETRY_MIN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY_MAX, BOND_RADIUS_RETRY_MAX);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY_DFLT, radiusSrv.retransmit);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT_MIN, BOND_RADIUS_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT_MAX, BOND_RADIUS_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT_DFLT, radiusSrv.timeout);
    cJSON_AddNumberToObject(data, CGI_RADIUS_AUTHPORT_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_RADIUS_AUTHPORT_MAX, 65535);
    cJSON_AddNumberToObject(data, CGI_RADIUS_AUTHPORT_DFLT, DFLT_RADIUS_AUTHPORT);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_radiusEdit_get(void)
{
    cJSON        *root = NULL;
    cJSON        *data = NULL;
    char         *value = NULL;
    uint32       i = 0;
    sys_radius_t radiusSrv;

    SYS_MEM_CLEAR(radiusSrv);

    CGI_GET_JSON_CREATE(root, data);

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, value))
    {
        for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
        {
            SYS_MEM_CLEAR(radiusSrv);
            SYS_ERR_CHK(sal_radiusSrv_get(i, &radiusSrv));

            if (RADIUS_NOT_UESD_FLAG == radiusSrv.priority)
                continue;

            if (0 == osal_strcmp(radiusSrv.hostName, value))
                break;
        }
    }

    cJSON_AddBoolToObject(data, CGI_RADIUS_KEY_USEDFLT, radiusSrv.keyUseDft);
    cJSON_AddStringToObject(data, CGI_RADIUS_KEY, radiusSrv.key);
    cJSON_AddNumberToObject(data, CGI_RADIUS_MAXLEN_KEY, CAPA_AAA_KEY_LEN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_PRI, radiusSrv.priority);
    cJSON_AddNumberToObject(data, CGI_RADIUS_PRI_MIN, BOND_RADIUS_PRI_MIN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_PRI_MAX, BOND_RADIUS_PRI_MAX);
    cJSON_AddBoolToObject(data, CGI_RADIUS_RETRY_USEDFLT, radiusSrv.retransmitUseDft);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY, radiusSrv.retransmit);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY_MIN, BOND_RADIUS_RETRY_MIN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY_MAX, BOND_RADIUS_RETRY_MAX);
    cJSON_AddNumberToObject(data, CGI_RADIUS_RETRY_DFLT, DFLT_RADIUS_RETRY);
    cJSON_AddBoolToObject(data, CGI_RADIUS_TIMEOUT_USEDFLT, radiusSrv.timeoutUseDft);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT, radiusSrv.timeout);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT_MIN, BOND_RADIUS_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT_MAX, BOND_RADIUS_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_RADIUS_TIMEOUT_DFLT, DFLT_RADIUS_TIMEOUT);
    cJSON_AddNumberToObject(data, CGI_RADIUS_AUTHPORT, radiusSrv.srvPort);
    cJSON_AddNumberToObject(data, CGI_RADIUS_AUTHPORT_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_RADIUS_AUTHPORT_MAX, 65535);
    cJSON_AddNumberToObject(data, CGI_RADIUS_AUTHPORT_DFLT, DFLT_RADIUS_AUTHPORT);
    cJSON_AddNumberToObject(data, CGI_RADIUS_USGTYPE, radiusSrv.usgType);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

