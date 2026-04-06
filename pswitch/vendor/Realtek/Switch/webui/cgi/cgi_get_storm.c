
#include <include/cgi.h>

static int32 cgi_storm_intf_get(void);
static int32 cgi_storm_intfEdit_get(void);

const cgi_cmdmap_t cmdGetStormTbl[] =
{
    {CGI_CMD_STORM,        cgi_storm_intf_get},
    {CGI_CMD_STORMEDIT,    cgi_storm_intfEdit_get},
    {NULL, NULL}
};

static int32 cgi_storm_intf_get(void)
{
    cJSON                       *root = NULL;
    cJSON                       *data = NULL;
    cJSON                       *ports = NULL;
    cJSON                       *port = NULL;
    sys_enable_t                enable;
    sys_logic_port_t            lp;
    sys_enable_t                include;
    sys_rate_storm_rateMode_t   unit;
    sys_rate_storm_action_t     action;
    uint32                      rate;

    CGI_GET_JSON_CREATE(root, data);

    sal_rate_stormCtrlRefreshMode_get(&unit);
    cJSON_AddNumberToObject(data, "unit", unit);

    sal_rate_stormCtrlIncludeIfg_get(&include);
    cJSON_AddNumberToObject(data, "type", include);

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_rate_stormControlPortEnable_get(lp, &enable);
        cJSON_AddBoolToObject(port, "state", enable);

        sal_rate_stormControlTypeEnable_get(lp, STORM_GROUP_BROADCAST, &enable);
        cJSON_AddBoolToObject(port, "bcEnable", enable);

        sal_rate_stormControlRate_get(lp, STORM_GROUP_BROADCAST, &rate);
        cJSON_AddNumberToObject(port, "broadcast", rate);

        sal_rate_stormControlTypeEnable_get(lp, STORM_GROUP_MULTICAST, &enable);
        cJSON_AddBoolToObject(port, "umcEnable", enable);

        sal_rate_stormControlRate_get(lp, STORM_GROUP_MULTICAST, &rate);
        cJSON_AddNumberToObject(port, "unknownMC", rate);

        sal_rate_stormControlTypeEnable_get(lp, STORM_GROUP_UNICAST, &enable);
        cJSON_AddBoolToObject(port, "uncEnable", enable);

        sal_rate_stormControlRate_get(lp, STORM_GROUP_UNICAST, &rate);
        cJSON_AddNumberToObject(port, "unknownUC", rate);

        sal_rate_stormControlAction_get(lp, &action);
        cJSON_AddNumberToObject(port, "action", action);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_storm_intfEdit_get(void)
{
    cJSON                       *root = NULL;
    cJSON                       *data = NULL;
    sys_enable_t                enable;
    sys_logic_port_t            lp = 0;
    sys_rate_storm_action_t     action;
    uint32                      rate;
    char                        *value;
    sys_rate_storm_rateMode_t   mode;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_rate_stormCtrlRefreshMode_get(&mode);
    cJSON_AddNumberToObject(data, "unit", mode);

    sal_rate_stormControlPortEnable_get(lp, &enable);
    cJSON_AddBoolToObject(data, "state", enable);

    if (mode == BASED_ON_BYTE)
    {
        cJSON_AddNumberToObject(data, "min_rate", BOND_RATE_STORM_BPS_MIN);
        cJSON_AddNumberToObject(data, "max_rate", BOND_RATE_STORM_BPS_MAX);
    }
    else
    {
        cJSON_AddNumberToObject(data, "min_rate", BOND_RATE_STORM_PPS_MIN);
        cJSON_AddNumberToObject(data, "max_rate", BOND_RATE_STORM_PPS_MAX);
    }

    cJSON_AddNumberToObject(data, "dflt_rate", DFLT_RATE_STORM_CTRL);

    sal_rate_stormControlTypeEnable_get(lp, STORM_GROUP_BROADCAST, &enable);
    cJSON_AddBoolToObject(data, "bcEnable", enable);

    sal_rate_stormControlRate_get(lp, STORM_GROUP_BROADCAST, &rate);
    cJSON_AddNumberToObject(data, "broadcast", rate);

    sal_rate_stormControlTypeEnable_get(lp, STORM_GROUP_MULTICAST, &enable);
    cJSON_AddBoolToObject(data, "umcEnable", enable);

    sal_rate_stormControlRate_get(lp, STORM_GROUP_MULTICAST, &rate);
    cJSON_AddNumberToObject(data, "unknownMC", rate);

    sal_rate_stormControlTypeEnable_get(lp, STORM_GROUP_UNICAST, &enable);
    cJSON_AddBoolToObject(data, "uncEnable", enable);

    sal_rate_stormControlRate_get(lp, STORM_GROUP_UNICAST, &rate);
    cJSON_AddNumberToObject(data, "unknownUC", rate);

    sal_rate_stormControlAction_get(lp, &action);
    cJSON_AddBoolToObject(data, "action", action);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

