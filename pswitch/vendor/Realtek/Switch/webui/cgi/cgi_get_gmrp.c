
#include <include/cgi.h>

static int32 cgi_gmrp_get(void);
static int32 cgi_gmrp_edit_get(void);

const cgi_cmdmap_t cmdGetGmrpTbl[] =
{
    {CGI_CMD_GMRP,            cgi_gmrp_get},
    {CGI_CMD_GMRP_INTFEDIT,        cgi_gmrp_edit_get},
    {NULL, NULL}
};

static int32 cgi_gmrp_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;

    sys_enable_t  gmrpEnable;
    sys_enable_t  gmrpPortEnable;
    sys_logic_port_t lp;

    sal_gmrp_enable_get(&gmrpEnable);

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddBoolToObject(data, CGI_GMRP_GLOBAL_STATUS, gmrpEnable);

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_gmrp_port_enable_get(lp, &gmrpPortEnable);
        cJSON_AddBoolToObject(port, CGI_GMRP_PORT_STATUS, gmrpPortEnable);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_gmrp_edit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;

    sys_enable_t gmrpPortEnable;
    sys_logic_port_t lp = 0;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_gmrp_port_enable_get(lp, &gmrpPortEnable);
    cJSON_AddBoolToObject(data, CGI_GMRP_PORT_EDIT_STATUS, gmrpPortEnable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

