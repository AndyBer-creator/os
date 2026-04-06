
#include <include/cgi.h>
#include <libsal/sal_port.h>
#include <libsal/sal_poe.h>

static int32 cgi_poe_setting_get(void);
static int32 cgi_poe_settingEdit_get(void);
static int32 cgi_poe_timer_get(void);
static int32 cgi_poe_reboot_timer_get(void);
static int32 cgi_poe_rebootEdit_get(void);

const cgi_cmdmap_t cmdGetPoeTbl[] =
{
    {CGI_CMD_POE_PORT,          cgi_poe_setting_get},
    {CGI_CMD_POE_PORTEDIT,      cgi_poe_settingEdit_get},
    {CGI_CMD_POE_PORTTIMER,     cgi_poe_timer_get},
    {CGI_CMD_POE_PORTREBOOTTIMER,     cgi_poe_reboot_timer_get},
    {CGI_CMD_POE_PORTREBOOTEDIT,     cgi_poe_rebootEdit_get},
    {NULL, NULL}
};

static int32 g_devPower = 0;

static int32 cgi_poe_setting_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    char descp[CAPA_PORTDESC_STR_LEN + 1];
    uint32 devTemp = 0;
    uint32 devPower = 0;
    uint32 tempVal = 0;
    
    sal_poe_ioctl_t portInfo;

    CGI_GET_JSON_CREATE(root, data);

    g_devPower = 0;

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        if(IS_NOT_POE_PORT(lp))
        {
            continue;
        }

        SYS_MEM_CLEAR(descp);

        sal_port_descp_get(lp, descp);

        port = cJSON_CreateObject();

        cJSON_AddStringToObject(port, CGI_PORT_DESCP, descp);

        SYS_MEM_CLEAR(portInfo);

        sal_poe_portInfo_get(lp, &portInfo);

        cJSON_AddBoolToObject(port, CGI_POE_PORT_ENABLE, portInfo.portEnable);
        cJSON_AddBoolToObject(port, CGI_POE_PORT_STATUS, portInfo.portStatus);

        if(0 == portInfo.portStatus)
        {
            cJSON_AddStringToObject(port, CGI_POE_PORT_TYPE, "N/A");
            cJSON_AddNumberToObject(port, CGI_POE_PORT_LEVEL, 0);
            cJSON_AddNumberToObject(port, CGI_POE_PORT_POWER, 0);
            cJSON_AddNumberToObject(port, CGI_POE_PORT_VOLTAGE, 0);
            cJSON_AddNumberToObject(port, CGI_POE_PORT_CURRENT, 0);
            cJSON_AddBoolToObject(port, CGI_POE_PORT_WATCH_DOG_CHECK, FALSE);
        }
        else
        {
            
            if(PD_MODE_AF == portInfo.portType)
            {
                cJSON_AddStringToObject(port, CGI_POE_PORT_TYPE, "AF");
            }
            else if(PD_MODE_AT == portInfo.portType)
            {
                cJSON_AddStringToObject(port, CGI_POE_PORT_TYPE, "AT");
            }
            else if(PD_MODE_BT == portInfo.portType)
            {
                cJSON_AddStringToObject(port, CGI_POE_PORT_TYPE, "BT");
            }
            else
            {
                cJSON_AddStringToObject(port, CGI_POE_PORT_TYPE, "N/A");
            }

            if(portInfo.portLevel == PD_CLASS_UNKNOWN)
            {
                tempVal = 0;
            }
            else
            {
                tempVal = portInfo.portLevel;
            }

            if (ENABLED == portInfo.portEnable)
            {
                cJSON_AddNumberToObject(port, CGI_POE_PORT_LEVEL, tempVal);
            }
            
            cJSON_AddNumberToObject(port, CGI_POE_PORT_POWER_LIMIT, portInfo.portPowerLimit);

            if (0 != portInfo.portPower)
                cJSON_AddNumberToObject(port, CGI_POE_PORT_POWER, portInfo.portPower);

            if (0 != portInfo.portVolatge)
                cJSON_AddNumberToObject(port, CGI_POE_PORT_VOLTAGE, portInfo.portVolatge);

            if (0 != portInfo.portCurrent)
                cJSON_AddNumberToObject(port, CGI_POE_PORT_CURRENT, portInfo.portCurrent);
        #ifdef CONFIG_SYS_POE_SWDOG
            cJSON_AddBoolToObject(port, CGI_POE_PORT_WATCH_DOG_CHECK, TRUE);
            
            cJSON_AddBoolToObject(port, CGI_POE_PORT_WATCH_DOG, portInfo.portWatchDog);
        #else
            cJSON_AddBoolToObject(port, CGI_POE_PORT_WATCH_DOG_CHECK, FALSE);
        #endif
            g_devPower += portInfo.portCurrent * portInfo.portVolatge;
        }
        cJSON_AddItemToArray(ports, port);
    }

    devPower = g_devPower;
    cJSON_AddNumberToObject(data, CGI_POE_DEV_POWER, devPower);

    sal_poe_devTemperature_get(15, &devTemp);
    cJSON_AddNumberToObject(data, CGI_POE_DEV_TEMPERATURE, devTemp);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_poe_settingEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;
    sys_logic_port_t lp = 0;

    sal_poe_ioctl_t portInfo;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
    {
        lp = SYS_STR2UINT(value);
    }

    CGI_GET_JSON_CREATE(root, data);

    SYS_MEM_CLEAR(portInfo);

    sal_poe_portInfo_get(lp, &portInfo);

    cJSON_AddBoolToObject(data, CGI_POE_PORT_ENABLE, portInfo.portEnable);
    #ifdef CONFIG_SYS_POE_SWDOG
    cJSON_AddBoolToObject(data, CGI_POE_PORT_WATCH_DOG_CHECK, TRUE);
    cJSON_AddBoolToObject(data, CGI_POE_PORT_WATCH_DOG, portInfo.portWatchDog);
    #else
    cJSON_AddBoolToObject(data, CGI_POE_PORT_WATCH_DOG_CHECK, FALSE);
    #endif
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_poe_timer_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *weeks = NULL;
    cJSON *week = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    char *value = NULL;
    sys_logic_port_t selPort = 0;
    uint32 ulWeek;
    uint32 ulWeek_tmp = 0;
    uint32 hour;
    sys_logic_port_t lp;
    char descp[CAPA_PORTDESC_STR_LEN + 1];

    if (CGI_IS_PARAM_EXIST(CGI_SEL_PORT, value))
        selPort = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_SEL_PORT, selPort);
    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);
    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        if(IS_NOT_POE_PORT(lp))
        {
            continue;
        }
        port = cJSON_CreateObject();
        cJSON_AddItemToArray(ports, port);
        cJSON_AddNumberToObject(port, CGI_VALUE, lp);
        sys_port_lPort2uStr(lp, SYS_PORT_STRFMT_WEB, descp);
        cJSON_AddStringToObject(port, CGI_NAME, descp);
    }

    weeks = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_POE_PORT_WEEKS, weeks);

    for (ulWeek = 1; ulWeek <= POE_WEEK_MAX; ulWeek++)
    {
        if (ulWeek < POE_WEEK_MAX)
        {
            ulWeek_tmp = ulWeek;
        }
        else
        {
            ulWeek_tmp = 0;
        }

        sal_poe_portSchedule_get(selPort, ulWeek_tmp, &hour);

        week = cJSON_CreateObject();
        cJSON_AddItemToArray(weeks, week);
        cJSON_AddNumberToObject(week, CGI_POE_PORT_WEEK, ulWeek_tmp);
        cJSON_AddNumberToObject(week, CGI_POE_PORT_HOUR, hour);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_poe_reboot_timer_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    char reTimestr[24] = {0};
    char DeTimestr[24] = {0};
    sal_poe_time_t rebTime;
    sal_poe_time_t delTime;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        if(IS_NOT_POE_PORT(lp))
        {
            continue;
        }
        SYS_MEM_CLEAR(rebTime);
        SYS_MEM_CLEAR(delTime);

        port = cJSON_CreateObject();

        sal_poe_portAssignTime_work_get(lp, &rebTime, &delTime);

        osal_sprintf(reTimestr, "%02u:%02u:%02u", rebTime.hour, rebTime.minute, rebTime.second);
        osal_sprintf(DeTimestr, "%02u:%02u:%02u", delTime.hour, delTime.minute, delTime.second);

        cJSON_AddStringToObject(port, CGI_POE_PORT_REBOOT_TIME, reTimestr);
        cJSON_AddStringToObject(port, CGI_POE_PORT_DELAY_TIME, DeTimestr);

        cJSON_AddItemToArray(ports, port);
        
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_poe_rebootEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;
    sys_logic_port_t lp = 0;
    sal_poe_time_t rebTime;
    sal_poe_time_t delTime;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    SYS_MEM_CLEAR(rebTime);
    SYS_MEM_CLEAR(delTime);

    sal_poe_portAssignTime_work_get(lp, &rebTime, &delTime);
    
    cJSON_AddNumberToObject(data, CGI_POE_PORT_EDIT_RE_HOUR, rebTime.hour);
    cJSON_AddNumberToObject(data, CGI_POE_PORT_EDIT_RE_MINUTE, rebTime.minute);
    cJSON_AddNumberToObject(data, CGI_POE_PORT_EDIT_RE_SECOND, rebTime.second);
    cJSON_AddNumberToObject(data, CGI_POE_PORT_EDIT_DE_HOUR, delTime.hour);
    cJSON_AddNumberToObject(data, CGI_POE_PORT_EDIT_DE_MINUTE, delTime.minute);
    cJSON_AddNumberToObject(data, CGI_POE_PORT_EDIT_DE_SECOND, delTime.second);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

