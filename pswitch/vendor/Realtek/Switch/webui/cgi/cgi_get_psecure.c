
#include <include/cgi.h>

#include <libsal/sal_psecure.h>

static int32 cgi_psecure_get(void);
static int32 cgi_psecure_portEdit_get(void);
static int32 cgi_psecure_mac_get(void);

const cgi_cmdmap_t cmdGetPsecureTbl[] =
{
    {CGI_CMD_PSECURE_PROP,       cgi_psecure_get},
    {CGI_CMD_PSECURE_PORTEDIT,   cgi_psecure_portEdit_get},
    {CGI_CMD_PSECURE_MAC,        cgi_psecure_mac_get},
    {NULL, NULL}
};

int32 cgi_psecure_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    sys_enable_t enable = DISABLED;
    uint32 rateLimit = 0;
    sys_psecure_port_t portCfg;
    sys_psecure_portInfo_t portInfo;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    sal_psecure_enable_get(&enable);
    sal_psecure_rateLimit_get(&rateLimit);

    cJSON_AddBoolToObject(data, CGI_PSECURE_STATE, enable);
    cJSON_AddNumberToObject(data, CGI_PSECURE_RATELIMIT, rateLimit);
    cJSON_AddNumberToObject(data, CGI_PSECURE_MIN_RATELIMIT, BOND_PSECURE_RATELIMIT_MIN);
    cJSON_AddNumberToObject(data, CGI_PSECURE_MAX_RATELIMIT, BOND_PSECURE_RATELIMIT_MAX);
    cJSON_AddNumberToObject(data, CGI_PSECURE_DFLT_RATELIMIT, DFLT_PSECURE_RATELIMIT);

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        SYS_MEM_CLEAR(portCfg);
        SYS_MEM_CLEAR(portCfg);

        sal_psecure_port_get(lp, &portCfg);
        sal_psecure_portInfo_get(lp, &portInfo);

        cJSON_AddBoolToObject(port, CGI_PSECURE_STATE, portCfg.enable);
        cJSON_AddNumberToObject(port, CGI_PSECURE_MAXNUMADDR, portCfg.limitNum);
        cJSON_AddNumberToObject(port, CGI_PSECURE_VIOLATE, portCfg.violation);
        cJSON_AddBoolToObject(port, CGI_PSECURE_STICKY, portCfg.sticky);
        cJSON_AddNumberToObject(port, CGI_PSECURE_TOTAL, portInfo.totalAddr);
        cJSON_AddNumberToObject(port, CGI_PSECURE_CONFIG, portInfo.staticAddr);
        cJSON_AddNumberToObject(port, CGI_PSECURE_VIOLATENUM, portInfo.violateNum);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

int32 cgi_psecure_portEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_logic_port_t lp = 0;
    char *value = NULL;
    sys_psecure_port_t portCfg;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    SYS_MEM_CLEAR(portCfg);

    sal_psecure_port_get(lp, &portCfg);

    if (ENABLED == portCfg.enable)
        cJSON_AddBoolToObject(data, CGI_PSECURE_STATE, TRUE);
    else
        cJSON_AddBoolToObject(data, CGI_PSECURE_STATE, FALSE);

    cJSON_AddNumberToObject(data, CGI_PSECURE_MAXNUMADDR, portCfg.limitNum);
    cJSON_AddNumberToObject(data, CGI_PSECURE_MIN_MAXNUMADDR, BOND_PSECURE_LIMITNUM_MIN);
    cJSON_AddNumberToObject(data, CGI_PSECURE_MAX_MAXNUMADDR, BOND_PSECURE_LIMITNUM_MAX);
    cJSON_AddNumberToObject(data, CGI_PSECURE_DFLT_MAXNUMADDR, DFLT_PSECURE_PORT_LIMITNUM);
    cJSON_AddNumberToObject(data, CGI_PSECURE_VIOLATE, portCfg.violation);
    cJSON_AddBoolToObject(data, CGI_PSECURE_STICKY, portCfg.sticky);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_psecure_mac_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *mac_array = NULL;
    cJSON                   *mac = NULL;
    char                    macStr[SYS_BUF32_LEN];
    char                    portStr[CAPA_PORT_STR_LEN];
    uint32                  i;
    uint32                  allocSize = 0;
    uint32                  portAddrs = 0;
    uint32                  totalAddrs = 0;
    sys_logic_port_t        lp;
    sys_l2_entry_t          *pEntry = NULL;
    sys_l2_entry_t          *pSortDb = NULL;

    SYS_ERR_CHK(sal_l2_db_gen());

    CGI_GET_JSON_CREATE(root, data);

    mac_array = cJSON_CreateArray();

    allocSize = (sizeof(sys_l2_entry_t) * CAPA_PSECURE_LIMIT_NUM);

    if (NULL == (pSortDb = osal_alloc(allocSize)))
        return SYS_ERR_OK;

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        portAddrs = 0;
        osal_memset(pSortDb, 0, allocSize);

        for (i = 0; i < CAPA_PSECURE_LIMIT_NUM; i++)
        {
            pEntry = sal_psecure_portMacEntryByIdx_ret(lp, i);

            if (IS_PSECURE_ENTRY_EMPTY(pEntry))
                continue;

            osal_memcpy(&pSortDb[portAddrs], pEntry, sizeof(sys_l2_entry_t));

            portAddrs++;
        }

        sys_l2_qsort(pSortDb, portAddrs, sizeof(sys_l2_entry_t), SYS_L2_SORTDATA_ENTRY, SYS_L2_SORTMETH_MACVLAN);

        for (i = 0; i < portAddrs; i++)
        {
            pEntry = &pSortDb[i];

            mac = cJSON_CreateObject();

            cJSON_AddNumberToObject(mac, CGI_MAC_VID, pEntry->vid);

            SYS_ERR_CHK(sys_util_mac2Str(pEntry->mac.octet, macStr));
            cJSON_AddStringToObject(mac, CGI_MAC_ADDR, macStr);
            SYS_ERR_CHK(LP2WEBSTR(pEntry->port, portStr));
            cJSON_AddStringToObject(mac, CGI_MAC_PORT, portStr);

            switch (pEntry->type)
            {
                case SYS_L2_TYPE_PSECURE_DYNAMIC:
                    cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeSecureDynamic");
                    break;

                case SYS_L2_TYPE_PSECURE_STICKY:
                    cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeSecureSticky");
                    break;

                case SYS_L2_TYPE_PSECURE_STATIC:
                    cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeSecureConfigured");
                    break;
                default:
                    break;
            }

            cJSON_AddItemToArray(mac_array, mac);

            totalAddrs++;
        }
    }

    osal_free(pSortDb);

    if (totalAddrs > 0)
        cJSON_AddItemToObject(data, CGI_MAC_ENTRYS, mac_array);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

