
#include <include/cgi.h>

static int32 cgi_tacplus_get(void);
static int32 cgi_tacplusAdd_get(void);
static int32 cgi_tacplusEdit_get(void);

const cgi_cmdmap_t cmdGetTacplusTbl[] =
{
    {CGI_CMD_TACPLUS,        cgi_tacplus_get},
    {CGI_CMD_TACPLUSADD,     cgi_tacplusAdd_get},
    {CGI_CMD_TACPLUSEDIT,    cgi_tacplusEdit_get},
    {NULL, NULL}
};

static int32 cgi_tacplus_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;
    char                strBuf[CAPA_AAA_KEY_LEN + 1];
    cJSON               *srvs = NULL;
    cJSON               *srv = NULL;
    sys_tacplus_t        tacplusSrv;
    uint32              i;
    uint32              cnt = 0;

    SYS_MEM_CLEAR(tacplusSrv);

    CGI_GET_JSON_CREATE(root, data);

    sal_tacplus_dftParam_get(&tacplusSrv);

    cJSON_AddStringToObject(data, CGI_TACPLUS_KEY, tacplusSrv.key);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_MAXLEN_KEY, CAPA_AAA_KEY_LEN);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT, tacplusSrv.timeout);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT_MIN, BOND_TACPLUS_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT_MAX, BOND_TACPLUS_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT_DFLT, DFLT_TACPLUS_TIMEOUT);

    srvs = cJSON_CreateArray();

    for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
    {
        sal_tacplus_server_get(i, &tacplusSrv);
        if (TACPLUS_NOT_UESD_FLAG == tacplusSrv.priority)
            continue;

        srv = cJSON_CreateObject();

        cJSON_AddStringToObject(srv, CGI_SRVHOST, tacplusSrv.srvHost);
        cJSON_AddNumberToObject(srv, CGI_TACPLUS_PRI, tacplusSrv.priority);

        sal_util_str_encrypt(tacplusSrv.key, strBuf);
        cJSON_AddStringToObject(srv, CGI_TACPLUS_KEY, strBuf);

        cJSON_AddNumberToObject(srv, CGI_TACPLUS_TIMEOUT, tacplusSrv.timeout);
        cJSON_AddNumberToObject(srv, CGI_TACPLUS_AUTHPORT, tacplusSrv.srvPort);

        cJSON_AddItemToArray(srvs , srv);
        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_TACPLUS_SERVERS, srvs);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_tacplusAdd_get(void)
{
    cJSON        *root = NULL;
    cJSON        *data = NULL;
    sys_tacplus_t tacplusSrv;

    CGI_GET_JSON_CREATE(root, data);

    sal_tacplus_dftParam_get(&tacplusSrv);

    cJSON_AddStringToObject(data, CGI_TACPLUS_KEY, tacplusSrv.key);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_MAXLEN_KEY, CAPA_AAA_KEY_LEN);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_PRI_MIN, BOND_TACPLUS_PRI_MIN);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_PRI_MAX, BOND_TACPLUS_PRI_MAX);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT_MIN, BOND_TACPLUS_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT_MAX, BOND_TACPLUS_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT_DFLT, tacplusSrv.timeout);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_AUTHPORT_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_AUTHPORT_MAX, 65535);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_AUTHPORT_DFLT, DFLT_TACPLUS_AUTHPORT);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_tacplusEdit_get(void)
{
    cJSON        *root = NULL;
    cJSON        *data = NULL;
    char         *value = NULL;
    uint32       i = 0;
    sys_tacplus_t tacplusSrv;

    CGI_GET_JSON_CREATE(root, data);

    SYS_MEM_CLEAR(tacplusSrv);

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, value))
    {
        for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
        {
            SYS_MEM_CLEAR(tacplusSrv);
            SYS_ERR_CHK(sal_tacplus_server_get(i, &tacplusSrv));

            if (TACPLUS_NOT_UESD_FLAG == tacplusSrv.priority)
                continue;

            if (0 == osal_strcmp(tacplusSrv.srvHost, value))
                break;
        }
    }

    cJSON_AddBoolToObject(data, CGI_TACPLUS_KEY_USEDFLT, tacplusSrv.keyUseDft);
    cJSON_AddStringToObject(data, CGI_TACPLUS_KEY, tacplusSrv.key);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_MAXLEN_KEY, CAPA_AAA_KEY_LEN);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_PRI, tacplusSrv.priority);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_PRI_MIN, BOND_TACPLUS_PRI_MIN);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_PRI_MAX, BOND_TACPLUS_PRI_MAX);
    cJSON_AddBoolToObject(data, CGI_TACPLUS_TIMEOUT_USEDFLT, tacplusSrv.timeoutUseDft);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT, tacplusSrv.timeout);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT_MIN, BOND_TACPLUS_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT_MAX, BOND_TACPLUS_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_TIMEOUT_DFLT, DFLT_TACPLUS_TIMEOUT);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_AUTHPORT, tacplusSrv.srvPort);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_AUTHPORT_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_AUTHPORT_MAX, 65535);
    cJSON_AddNumberToObject(data, CGI_TACPLUS_AUTHPORT_DFLT, DFLT_TACPLUS_AUTHPORT);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

