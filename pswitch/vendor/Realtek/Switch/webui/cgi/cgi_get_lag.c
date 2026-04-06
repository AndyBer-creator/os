
#include <include/cgi.h>

#include <libsal/sal_port.h>
#include <libsal/sal_trunk.h>

static int32 cgi_lag_mgmt_get(void);
static int32 cgi_lag_mgmtEdit_get(void);
static int32 cgi_lag_port_get(void);
static int32 cgi_lag_portEdit_get(void);
#ifdef CONFIG_SYS_PROTO_LACP
static int32 cgi_lag_lacp_get(void);
static int32 cgi_lag_lacpEdit_get(void);
#endif

const cgi_cmdmap_t cmdGetLagTbl[] =
{
    {CGI_CMD_LAG_MGMT,              cgi_lag_mgmt_get},
    {CGI_CMD_LAG_MGMTEDIT,          cgi_lag_mgmtEdit_get},
    {CGI_CMD_LAG_PORT,              cgi_lag_port_get},
    {CGI_CMD_LAG_PORTEDIT,          cgi_lag_portEdit_get},
#ifdef CONFIG_SYS_PROTO_LACP
    {CGI_CMD_LAG_LACP,              cgi_lag_lacp_get},
    {CGI_CMD_LAG_LACPEDIT,          cgi_lag_lacpEdit_get},
#endif
    {NULL, NULL}
};

static int32 cgi_lag_mgmt_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *lags = NULL;
    cJSON *lag = NULL;
    char name[CAPA_PORTDESC_STR_LEN + 1];
    char portmaskStr[CAPA_PORTMASK_STR_LEN + 1];
    uint32 trkId;
    sys_trunk_hashAlgo_t algo;
    sys_port_linkStatus_t linkStatus;
    sys_logic_portmask_t mbrs;
    sys_logic_portmask_t activeMbrs;
    sys_logic_portmask_t standbyMbrs;

    sal_trunk_hashAlgo_get(&algo);

    CGI_GET_JSON_CREATE(root, data);

	if(algo == LAG_HASH_ALGO_MAC)
   		cJSON_AddStringToObject(data, CGI_LAG_BALANCE_TYPE, CGI_LAG_VAL_BALANCE_MAC);
	else if(algo == LAG_HASH_ALGO_IP_MAC)
		cJSON_AddStringToObject(data, CGI_LAG_BALANCE_TYPE, CGI_LAG_VAL_BALANCE_IPMAC);
	else if(algo == LAG_HASH_ALGO_DMAC)
		cJSON_AddStringToObject(data, CGI_LAG_BALANCE_TYPE, CGI_LAG_VAL_BALANCE_DSTMAC);
	else if(algo == LAG_HASH_ALGO_SMAC)
		cJSON_AddStringToObject(data, CGI_LAG_BALANCE_TYPE, CGI_LAG_VAL_BALANCE_SRCMAC);
	else if(algo == LAG_HASH_ALGO_DIP)
		cJSON_AddStringToObject(data, CGI_LAG_BALANCE_TYPE, CGI_LAG_VAL_BALANCE_DSTIP);
	else if(algo == LAG_HASH_ALGO_SIP)
		cJSON_AddStringToObject(data, CGI_LAG_BALANCE_TYPE, CGI_LAG_VAL_BALANCE_SRCIP);

    lags = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_LAG_LAGS, lags);

    FOR_EACH_TRUNK(trkId)
    {
        SYS_MEM_CLEAR(name);
        SYS_MEM_CLEAR(mbrs);
        SYS_MEM_CLEAR(activeMbrs);
        SYS_MEM_CLEAR(standbyMbrs);

        sal_port_descp_get(TRK2LP(trkId), name);
        sal_port_linkStatus_get(TRK2LP(trkId), &linkStatus);
        sal_trunk_member_get(trkId, &mbrs);
        sal_trunk_activeMember_get(trkId, &activeMbrs);
        LOGIC_PORTMASK_ANDNOT(standbyMbrs, mbrs, activeMbrs);

        lag = cJSON_CreateObject();

        cJSON_AddStringToObject(lag, CGI_LAG_PORT_NAME, name);
        cJSON_AddBoolToObject(lag, CGI_LAG_LACP_STATE, IS_TRK_LACP(trkId));
        if (IS_LOGIC_PORTMASK_CLEAR(mbrs))
        {
            cJSON_AddBoolToObject(lag, CGI_LAG_ACTIVE, FALSE);
        }
        else
        {
            cJSON_AddBoolToObject(lag, CGI_LAG_ACTIVE, TRUE);
            cJSON_AddBoolToObject(lag, CGI_LAG_PORT_LINKUP, (PORT_LINKUP == linkStatus));
        }

        SYS_MEM_CLEAR(portmaskStr);
        LPM2WEBSTR(&activeMbrs, portmaskStr);
        cJSON_AddStringToObject(lag, CGI_LAG_ACTIVE_MBRS, portmaskStr);

        SYS_MEM_CLEAR(portmaskStr);
        LPM2WEBSTR(&standbyMbrs, portmaskStr);
        cJSON_AddStringToObject(lag, CGI_LAG_STANDBY_MBRS, portmaskStr);

        cJSON_AddItemToArray(lags, lag);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lag_mgmtEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *port = NULL;
    cJSON *notMbrs = NULL;
    cJSON *lagMbrs = NULL;
    char *value = NULL;
    char name[CAPA_PORTDESC_STR_LEN + 1];
    char portStr[CAPA_PORT_STR_LEN + 1];
    uint32 trkId = 1;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpmMbrs;
    sys_logic_portmask_t lpmNotMbrs;

    if (CGI_IS_PARAM_EXIST(CGI_LAG, value))
    {
        trkId = UID2TRK(SYS_STR2UINT(value));
    }

    SYS_MEM_CLEAR(name);
    SYS_MEM_CLEAR(lpmMbrs);
    SYS_MEM_CLEAR(lpmNotMbrs);

    sal_port_descp_get(TRK2LP(trkId), name);
    sal_trunk_member_get(trkId, &lpmMbrs);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        if (IS_LP_TRKMBR(lp))
            continue;

        LOGIC_PORTMASK_SET_PORT(lpmNotMbrs, lp);
    }

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddStringToObject(data, CGI_LAG_PORT_NAME, name);
    cJSON_AddNumberToObject(data, CGI_LAG_PORT_NAME_MAXLEN, CAPA_PORTDESC_STR_LEN);

    if (IS_TRK_LACP(trkId))
        cJSON_AddStringToObject(data, CGI_LAG_LAG_TYPE, CGI_LAG_TYPE_LACP);
    else
        cJSON_AddStringToObject(data, CGI_LAG_LAG_TYPE, CGI_LAG_TYPE_STATIC);

    if (!IS_LOGIC_PORTMASK_CLEAR(lpmNotMbrs))
    {
        notMbrs = cJSON_CreateArray();

        cJSON_AddItemToObject(data, CGI_LAG_NOT_MBRS, notMbrs);

        FOR_EACH_NORMAL_LOGIC_PORT(lp)
        {
            if (IS_LP_TRKMBR(lp))
                continue;

            SYS_MEM_CLEAR(portStr);
            LP2WEBSTR(lp, portStr);

            port = cJSON_CreateObject();

            cJSON_AddStringToObject(port, CGI_PORT, portStr);

            cJSON_AddItemToArray(notMbrs, port);
        }
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(lpmMbrs))
    {
        lagMbrs = cJSON_CreateArray();

        cJSON_AddItemToObject(data, CGI_LAG_MBRS, lagMbrs);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpmMbrs)
        {
            SYS_MEM_CLEAR(portStr);
            LP2WEBSTR(lp, portStr);

            port = cJSON_CreateObject();

            cJSON_AddStringToObject(port, CGI_PORT, portStr);

            cJSON_AddItemToArray(lagMbrs, port);
        }
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lag_port_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    sys_logic_port_t lp_tmp;
    uint32 trkId = 0;
    char descp[CAPA_PORTDESC_STR_LEN + 1];
    sys_enable_t enable = DISABLED;
    sys_enable_t flowCtrlStatus = DISABLED;
    sys_enable_t flowAuto = DISABLED;
    sys_port_linkStatus_t link;
    sys_port_speedDuplex_t status;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_logic_portmask_t mbrs;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_TRUNK(trkId)
    {
        lp = TRK2LP(trkId);

        SYS_MEM_CLEAR(descp);

        sal_port_descp_get(lp, descp);

        port = cJSON_CreateObject();

        cJSON_AddStringToObject(port, CGI_PORT_DESCP, descp);

        sal_trunk_member_get(trkId, &mbrs);
        if(!IS_LOGIC_PORTMASK_CLEAR(mbrs))
        {
            FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp_tmp, mbrs)
            {
                switch(LP2TYPE(lp_tmp))
                {
                    case PORT_TYPE_100M:
                        cJSON_AddLangStringArgToObject(port, CGI_LAG_TYPE, "lag","lblTypeEthArg", "%d,\'%c\'", 100, 'M');
                        break;
                    case PORT_TYPE_1000M:
                        cJSON_AddLangStringArgToObject(port, CGI_LAG_TYPE, "lag","lblTypeEthArg", "%d,\'%c\'", 1000, 'M');
                        break;
                    case PORT_TYPE_10G:
                        cJSON_AddLangStringArgToObject(port, CGI_LAG_TYPE, "lag","lblTypeEthArg", "%d,\'%c\'", 10, 'G');
                        break;
                    default:
                        cJSON_AddLangStringArgToObject(port, CGI_LAG_TYPE, "lag","lblTypeEthArg", "%d,\'%c\'", 1000, 'M');
                        break;
                }
                break;
            }
        }

        sal_port_enable_get(lp, &enable);
        cJSON_AddBoolToObject(port, CGI_PORT_ADMIN_STATUS, enable);

        SYS_MEM_CLEAR(speed);
        sal_port_speed_get(lp, &speed);
        if (ENABLED == speed.autoSpeed)
        {
            switch (speed.ability)
            {
                case SYS_PORT_SPEED_ALL:
                    cJSON_AddLangStringToObject(port, CGI_PORT_ADMIN_SPEED, "port","lblAuto");
                    break;

                case SYS_PORT_SPEED_10G:
                    cJSON_AddLangStringArgToObject(port, CGI_PORT_ADMIN_SPEED, "port","lblAutoCapArg", "'10G'");
                    break;

                case SYS_PORT_SPEED_1000M:
                    cJSON_AddLangStringArgToObject(port, CGI_PORT_ADMIN_SPEED, "port","lblAutoCapArg", "'1000M'");
                    break;

                case SYS_PORT_SPEED_100M:
                    cJSON_AddLangStringArgToObject(port, CGI_PORT_ADMIN_SPEED, "port","lblAutoCapArg", "'100M'");
                    break;

                case SYS_PORT_SPEED_10M:
                    cJSON_AddLangStringArgToObject(port, CGI_PORT_ADMIN_SPEED, "port","lblAutoCapArg", "'10M'");
                    break;

                case SYS_PORT_SPEED_10M_100M:
                    if (PORT_TYPE_100M == LP2TYPE(lp))
                        cJSON_AddLangStringToObject(port, CGI_PORT_ADMIN_SPEED, "port","lblAuto");
                    else
                        cJSON_AddLangStringArgToObject(port, CGI_PORT_ADMIN_SPEED, "port","lblAutoCapArg", "'10M/100M'");
                    break;

                default:
                    break;
            }
        }
        else
        {
            switch (speed.ability)
            {
                case SYS_PORT_SPEED_10G:
                    cJSON_AddStringToObject(port, CGI_PORT_ADMIN_SPEED, "10G");
                    break;

                case SYS_PORT_SPEED_1000M:
                    cJSON_AddStringToObject(port, CGI_PORT_ADMIN_SPEED, "1000M");
                    break;

                case SYS_PORT_SPEED_100M:
                    cJSON_AddStringToObject(port, CGI_PORT_ADMIN_SPEED, "100M");
                    break;

                case SYS_PORT_SPEED_10M:
                    cJSON_AddStringToObject(port, CGI_PORT_ADMIN_SPEED, "10M");
                    break;

                default:
                    break;
            }
        }

        SYS_MEM_CLEAR(duplex);
        sal_port_duplex_get(lp, &duplex);
        if (ENABLED == duplex.autoDuplex)
        {
            cJSON_AddLangStringToObject(port, CGI_PORT_ADMIN_DUPLEX, "port","lblAuto");
        }
        else
        {
            if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                cJSON_AddLangStringToObject(port, CGI_PORT_ADMIN_DUPLEX, "port","txtDuplexFull");
            else
                cJSON_AddLangStringToObject(port, CGI_PORT_ADMIN_DUPLEX, "port","txtDuplexHalf");
        }

        sal_port_flowCtrlAuto_get(lp, &flowAuto);
        sal_port_flowCtrl_get(lp, &enable, &flowCtrlStatus);
        if (ENABLED == flowAuto)
        {
            cJSON_AddLangStringToObject(port, CGI_PORT_ADMIN_FLOWCTRL, "port","lblAuto");
        }
        else
        {
            if (ENABLED == enable)
                cJSON_AddLangStringToObject(port, CGI_PORT_ADMIN_FLOWCTRL, "common","txtEnabled");
            else
                cJSON_AddLangStringToObject(port, CGI_PORT_ADMIN_FLOWCTRL, "common","txtDisabled");
        }

        sal_port_linkStatus_get(lp, &link);

        if (PORT_LINKUP == link)
        {
            sal_port_speedDuplex_get(lp, &status);
            cJSON_AddTrueToObject(port, CGI_PORT_OPER_STATUS);

            cJSON_AddBoolToObject(port, CGI_PORT_OPER_FLOWCTRL, flowCtrlStatus);

            switch (status.speed)
            {
                case PORT_SPEED_10M:
                    cJSON_AddStringToObject(port, CGI_PORT_OPER_SPEED, "10M");
                    break;

                case PORT_SPEED_100M:
                    cJSON_AddStringToObject(port, CGI_PORT_OPER_SPEED, "100M");
                    break;

                case PORT_SPEED_1000M:
                    cJSON_AddStringToObject(port, CGI_PORT_OPER_SPEED, "1000M");
                    break;

                case PORT_SPEED_10G:
                    cJSON_AddStringToObject(port, CGI_PORT_OPER_SPEED, "10G");
                    break;

                default:
                    break;
            }

            if (PORT_HALF_DUPLEX == status.duplex)
                cJSON_AddLangStringToObject(port, CGI_PORT_OPER_DUPLEX, "port", "txtDuplexHalf");
            else
                cJSON_AddLangStringToObject(port, CGI_PORT_OPER_DUPLEX, "port", "txtDuplexFull");
        }
        else
        {
            cJSON_AddFalseToObject(port, CGI_PORT_OPER_STATUS);
        }

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lag_portEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;
    sys_logic_port_t lp = 0;
    char descp[CAPA_PORTDESC_STR_LEN + 1];
    sys_enable_t enable = DISABLED, flowAuto = DISABLED;
    sys_enable_t flowCtrlStatus = DISABLED;
    sys_port_speed_t speed;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = TRK2LP(SYS_STR2UINT(value));

    CGI_GET_JSON_CREATE(root, data);

    SYS_MEM_CLEAR(descp);

    sal_port_descp_get(lp, descp);

    cJSON_AddStringToObject(data, CGI_PORT_DESCP, descp);
    cJSON_AddNumberToObject(data, CGI_PORT_DESCP_MAXLEN, CAPA_PORTDESC_STR_LEN);

    sal_port_enable_get(lp, &enable);
    cJSON_AddBoolToObject(data, CGI_PORT_ADMIN_STATUS, enable);

    SYS_MEM_CLEAR(speed);
    sal_port_speed_get(lp, &speed);
    if (ENABLED == speed.autoSpeed)
    {
        switch (speed.ability)
        {
            case SYS_PORT_SPEED_ALL:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "auto");
                break;

            case SYS_PORT_SPEED_1000M:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "auto_1000");
                break;

            case SYS_PORT_SPEED_100M:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "auto_100");
                break;

            case SYS_PORT_SPEED_10M:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "auto_10");
                break;

            case SYS_PORT_SPEED_10M_100M:
                if (PORT_TYPE_100M == LP2TYPE(lp))
                    cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "auto");
                else
                    cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "auto_10_100");
                break;

            default:
                break;
        }
    }
    else
    {
        switch (speed.ability)
        {
            case SYS_PORT_SPEED_10G:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "10000");
                break;

            case SYS_PORT_SPEED_1000M:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "1000");
                break;

            case SYS_PORT_SPEED_100M:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "100");
                break;

            case SYS_PORT_SPEED_10M:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "10");
                break;

            default:
                break;
        }
    }

    sal_port_flowCtrlAuto_get(lp, &flowAuto);
    sal_port_flowCtrl_get(lp, &enable, &flowCtrlStatus);
    if(ENABLED == flowAuto)
    {
        cJSON_AddStringToObject(data, CGI_PORT_ADMIN_FLOWCTRL, "auto");
    }
    else
    {
        if(ENABLED == enable)
            cJSON_AddStringToObject(data, CGI_PORT_ADMIN_FLOWCTRL, "enable");
        else
            cJSON_AddStringToObject(data, CGI_PORT_ADMIN_FLOWCTRL, "disable");
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LACP
static int32 cgi_lag_lacp_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *entry = NULL;

   uint32 sysPri;
    sys_logic_port_t lp;
    sys_lacp_port_t portState;

    CGI_GET_JSON_CREATE(root, data);

    sal_lacp_systemPriority_get(&sysPri);

    cJSON_AddNumberToObject(data, CGI_LAG_SYSPRI, sysPri);
    cJSON_AddNumberToObject(data, CGI_LAG_SYSPRI_MIN, 1);
    cJSON_AddNumberToObject(data, CGI_LAG_SYSPRI_MAX, 65535);
    cJSON_AddNumberToObject(data, CGI_LAG_SYSPRI_DFLT, DFLT_LACP_SYSTEM_PRIORITY);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        entry = cJSON_CreateObject();

        sal_lacp_portState_get(lp, &portState);

        cJSON_AddNumberToObject(entry, CGI_LAG_PRI, portState.pv.actor_port_pri);

        if(portState.pv.actor_oper_port_state & PS_LACP_TIMEOUT)
        {
            cJSON_AddLangStringToObject(entry, CGI_LAG_TIMEOUT, "lag", "lblShort");
        }
        else
            cJSON_AddLangStringToObject(entry, CGI_LAG_TIMEOUT, "lag", "lblLong");

        cJSON_AddItemToArray(ports, entry);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_lag_lacpEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;
    uint16 portPri = 0;
    sys_logic_port_t lp = 0;
    sys_lacp_timeout_t timeout;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    sal_lacp_portPriority_get(lp, &portPri);
    sal_lacp_portTimeout_get(lp, &timeout);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_LAG_PORTPRI, portPri);
    cJSON_AddNumberToObject(data, CGI_LAG_PORTPRI_MIN, 1);
    cJSON_AddNumberToObject(data, CGI_LAG_PORTPRI_MAX, 65535);
    cJSON_AddNumberToObject(data, CGI_LAG_PORTPRI_DFLT, DFLT_LACP_PORT_PRIORITY);

    if(LACP_TIMEOUT_SHORT == timeout)
        cJSON_AddStringToObject(data, CGI_LAG_TIMEOUT, "short");
    else
        cJSON_AddStringToObject(data, CGI_LAG_TIMEOUT, "long");

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

