
#include <include/cgi.h>
#include <libsal/sal_udld.h>

static int32 cgi_udld_prop_get(void);
static int32 cgi_udld_portEdit_get(void);
static int32 cgi_udld_neighbor_get(void);

const cgi_cmdmap_t cmdGetUdldTbl[] =
{
    {CGI_CMD_UDLD_PROP,          cgi_udld_prop_get},
    {CGI_CMD_UDLD_PORTEDIT,      cgi_udld_portEdit_get},
    {CGI_CMD_UDLD_NEIGHBOR,      cgi_udld_neighbor_get},
    {NULL, NULL}
};

static int32 cgi_udld_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *entry = NULL;
    uint32 interval;
    uint16 nbrNum = 0;
    sys_logic_port_t lp = 0;
    sys_udld_mode_t portUdldMode;
    sys_udld_bidir_state_t bidirState;
    sys_udld_op_state_t operational_state;

    CGI_GET_JSON_CREATE(root, data);

    sal_udld_messageTime_get(&interval);
    cJSON_AddNumberToObject(data, CGI_UDLD_TXINTV, interval);
    cJSON_AddNumberToObject(data, CGI_UDLD_MIN_TXINTV, BOND_UDLD_INTERVAL_MIN);
    cJSON_AddNumberToObject(data, CGI_UDLD_MAX_TXINTV, BOND_UDLD_INTERVAL_MAX);
    cJSON_AddNumberToObject(data, CGI_UDLD_DEF_TXINTV, DFLT_UDLD_INTERVAL_TIME);

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        entry = cJSON_CreateObject();

        sal_udld_portMode_get(lp, &portUdldMode);
        sal_udld_portBidiretionState_get(lp, &bidirState);
        sal_udld_portOperState_get(lp, &operational_state);
        sal_udld_portNeighborNumber_get(lp, &nbrNum);

        cJSON_AddNumberToObject(entry, CGI_UDLD_PORTMODE, portUdldMode);
        cJSON_AddNumberToObject(entry, CGI_UDLD_BIDIRECT_STATE, bidirState);

        if (portUdldMode == UDLD_MODE_DISABLE)
            cJSON_AddNumberToObject(entry, CGI_UDLD_OPER_STATE, 1);
        else
            cJSON_AddNumberToObject(entry, CGI_UDLD_OPER_STATE, operational_state);

        cJSON_AddNumberToObject(entry, CGI_UDLD_NUMBER_OF_NEIGHBORS, nbrNum);
        cJSON_AddItemToArray(ports, entry);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_udld_portEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value;
    sys_logic_port_t lp = 0;
    sys_udld_mode_t mode;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_udld_portMode_get(lp, &mode);

    cJSON_AddNumberToObject(data, CGI_UDLD_PORTMODE, mode);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_udld_neighbor_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *neighbors = NULL;
    cJSON *neighbor = NULL;
    uint16 nbrNum = 0;
    uint16 i = 0;
    uint32 cnt = 0;
    sys_logic_port_t lp = 0;
    sys_udld_mode_t portUdldMode;
    sys_udld_neighbor_info_t nbrInfo;

    CGI_GET_JSON_CREATE(root, data);
    neighbors = cJSON_CreateArray();

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        sal_udld_portMode_get(lp, &portUdldMode);
        sal_udld_portNeighborNumber_get(lp, &nbrNum);

        if(UDLD_MODE_DISABLE == portUdldMode || 0 == nbrNum)
            continue;

        for(i = 0; i < nbrNum; i++)
        {
            sal_udld_portNeighbor_get(lp, i, &nbrInfo);

            if(FALSE == nbrInfo.valid)
                continue;

            neighbor = cJSON_CreateObject();

            cJSON_AddNumberToObject(neighbor, CGI_UDLD_EXPIRATION_TIME, nbrInfo.hold_timer);
            cJSON_AddNumberToObject(neighbor, CGI_UDLD_CURNEIGHBORSTATE, nbrInfo.status);
            cJSON_AddStringToObject(neighbor, CGI_UDLD_DEVNAME, nbrInfo.deviceName);
            cJSON_AddStringToObject(neighbor, CGI_UDLD_DEVID, nbrInfo.device_id);
            cJSON_AddStringToObject(neighbor, CGI_UDLD_PORTID, nbrInfo.port_id);
            cJSON_AddNumberToObject(neighbor, CGI_UDLD_MSGINTV, nbrInfo.messageInterval);
            cJSON_AddNumberToObject(neighbor, CGI_UDLD_TIMEOUTINTV, nbrInfo.timeOutInterval);
            cJSON_AddItemToArray(neighbors, neighbor);
            cnt++;
        }
    }

    if (0 != cnt)
        cJSON_AddItemToObject(data, CGI_UDLD_NEIGHBORS, neighbors);
    else
    {
        cJSON_Delete(neighbors);
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

