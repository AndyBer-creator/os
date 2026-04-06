
#include <include/cgi.h>

#include <libsal/sal_port.h>
#include <libsal/sal_optical.h>
#include <libsal/sal_eee.h>
#include <libsal/sal_mib.h>

static int32 cgi_port_setting_get(void);
static int32 cgi_port_settingEdit_get(void);
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
static int32 cgi_port_errDisable_get(void);
static int32 cgi_port_errDisableStatus_get(void);
#endif
#ifdef CONFIG_SYS_JUMBO_FRAME
static int32 cgi_port_jumbo_get(void);
#endif
#ifdef CONFIG_SYS_EEE
static int32 cgi_port_eee_get(void);
static int32 cgi_port_eeeEdit_get(void);
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
static int32 cgi_port_protect_get(void);
static int32 cgi_port_protectEdit_get(void);
#endif
static int32 cgi_port_cnt_get(void);
#ifdef CONFIG_SYS_BW_UTILIZATION
static int32 cgi_port_bwUtilz_get(void);
#endif

const cgi_cmdmap_t cmdGetPortTbl[] =
{
    {CGI_CMD_PORT_PORT,          cgi_port_setting_get},
    {CGI_CMD_PORT_PORTEDIT,      cgi_port_settingEdit_get},
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    {CGI_CMD_PORT_ERRDISABLE,    cgi_port_errDisable_get},
    {CGI_CMD_PORT_ERRDISABLESTATUS,    cgi_port_errDisableStatus_get},
#endif
#ifdef CONFIG_SYS_JUMBO_FRAME
    {CGI_CMD_PORT_JUMBO,         cgi_port_jumbo_get},
#endif
#ifdef CONFIG_SYS_EEE
    {CGI_CMD_PORT_EEE,           cgi_port_eee_get},
    {CGI_CMD_PORT_EEEEDIT,       cgi_port_eeeEdit_get},
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
    {CGI_CMD_PORT_PROTECT,       cgi_port_protect_get},
    {CGI_CMD_PORT_PROTECTEDIT,   cgi_port_protectEdit_get},
#endif
    {CGI_CMD_PORT_CNT,           cgi_port_cnt_get},
#ifdef CONFIG_SYS_BW_UTILIZATION
    {CGI_CMD_PORT_BWUTLIZ,       cgi_port_bwUtilz_get},
#endif

    {NULL, NULL}
};

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
const sys_text_t cgi_port_errDisable[] =
{
    { ""                          },
    { CGI_PORT_ERRDISABLE_BPDU      },
#ifdef CONFIG_SYS_PROTO_UDLD
    { CGI_PORT_ERRDISABLE_UDLD      },
#endif
#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
    { CGI_PORT_ERRDISABLE_SELFLOOP  },
#endif
    { CGI_PORT_ERRDISABLE_BCAST     },
    { CGI_PORT_ERRDISABLE_MCAST     },
    { CGI_PORT_ERRDISABLE_UCAST     },
    { CGI_PORT_ERRDISABLE_ACL       },
    { CGI_PORT_ERRDISABLE_PORTSEC   },
    { CGI_PORT_ERRDISABLE_DHCPRATE  },
    { CGI_PORT_ERRDISABLE_ARPINSPECT}
};

const sys_text_t cgi_port_errDisableTxt[] =
{
    { ""                        },
    { "lblBPDUGuard"            },
#ifdef CONFIG_SYS_PROTO_UDLD
    { "lblUdld"                 },
#endif
#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
    { "lblSelfLoop"             },
#endif
    { "lblBcastFlood"           },
    { "lblUnknownMcastFlood"    },
    { "lblUnicastFlood"         },
    { "lblACL"                  },
    { "lblPortSecurity"         },
    { "lblDHCPRateLimit"        },
    { "lblARPRateLimit"         }
};
#endif

static int32 cgi_port_setting_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    char descp[CAPA_PORTDESC_STR_LEN + 1];
    sys_enable_t enable = DISABLED;
    sys_enable_t flowCtrlStatus = DISABLED;
    sys_enable_t flowAuto = DISABLED;
    sys_port_media_t media;
    sys_port_linkStatus_t link;
    sys_port_speedDuplex_t status;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    uint32 bitrate=0;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(descp);

        sal_port_descp_get(lp, descp);

        port = cJSON_CreateObject();

        cJSON_AddStringToObject(port, CGI_PORT_DESCP, descp);

        if (IS_COMBO_PORT(lp))
        {
            sal_port_linkMedia_get(lp, &media);

            if (PORT_MEDIA_FIBER == media)
            {
            	if(IS_COMBO_FIBER_PORT(lp))
					cJSON_AddLangStringArgToObject(port, CGI_PORT_MEDIA_TYPE, "port","txtMediaFiber", "%d", 1000);
            	else
                	cJSON_AddLangStringArgToObject(port, CGI_PORT_MEDIA_TYPE, "port","txtMediaComboF", "%d", 1000);
            }
            else
                cJSON_AddLangStringArgToObject(port, CGI_PORT_MEDIA_TYPE, "port","txtMediaComboC", "%d", 1000);
        }
        else if (IS_FIBER_PORT(lp))
        {
            if (PORT_TYPE_100M == LP2TYPE(lp))
                cJSON_AddLangStringArgToObject(port, CGI_PORT_MEDIA_TYPE, "port","txtMediaFiber", "%d", 100);
            else if (PORT_TYPE_1000M == LP2TYPE(lp))
                cJSON_AddLangStringArgToObject(port, CGI_PORT_MEDIA_TYPE, "port","txtMediaFiber", "%d", 1000);
            else if (PORT_TYPE_10G == LP2TYPE(lp))
                cJSON_AddLangStringArgToObject(port, CGI_PORT_MEDIA_TYPE, "port","txtMediaGFiber", "%d", 10);
        }
        else
        {
            if (PORT_TYPE_100M == LP2TYPE(lp))
                cJSON_AddLangStringArgToObject(port, CGI_PORT_MEDIA_TYPE, "port","txtMediaCopper", "%d", 100);
            else if (PORT_TYPE_1000M == LP2TYPE(lp))
                cJSON_AddLangStringArgToObject(port, CGI_PORT_MEDIA_TYPE, "port","txtMediaCopper", "%d", 1000);
#if 0
			else if (PORT_TYPE_2500M == LP2TYPE(lp))
                cJSON_AddLangStringArgToObject(port, CGI_PORT_MEDIA_TYPE, "port","txtMediaCopper", "%d", 2500);
#endif 
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
#if 0
				case SYS_PORT_SPEED_2500M:
                    cJSON_AddLangStringArgToObject(port, CGI_PORT_ADMIN_SPEED, "port","lblAutoCapArg", "'2500M'");
                    break;
#endif 
                default:
                    break;
            }
        }
        else
        {
            switch (speed.ability)
            {
                case SYS_PORT_SPEED_10G:

                    if ((!IS_TRKLP(lp))&&
                        (SYS_ERR_OK == sal_optical_bitRateNominal_get(lp, &bitrate))&&
                        ((bitrate != 0)&&(bitrate <= 10000)))
                        cJSON_AddStringToObject(port, CGI_PORT_ADMIN_SPEED, "1000M");
                    else
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

#if 0
				case SYS_PORT_SPEED_2500M:
                    cJSON_AddStringToObject(port, CGI_PORT_ADMIN_SPEED, "2500M");
                    break;
#endif 

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

#if 0
				case PORT_SPEED_2_5G:
                    cJSON_AddStringToObject(port, CGI_PORT_OPER_SPEED, "2500M");
                    break;
#endif 
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

static int32 cgi_port_settingEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;
    sys_logic_port_t lp = 0;
    char descp[CAPA_PORTDESC_STR_LEN + 1];
    sys_enable_t enable = DISABLED, flowAuto = DISABLED;
    sys_enable_t flowCtrlStatus = DISABLED;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
	sys_port_type_t  portType;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

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
#if 0
            case SYS_PORT_SPEED_2500M:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "auto_2500");
                break;
#endif 

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
#if 0
            case SYS_PORT_SPEED_2500M:
                cJSON_AddStringToObject(data, CGI_PORT_ADMIN_SPEED, "2500");
                break;
#endif 

            default:
                break;
        }
    }

    SYS_MEM_CLEAR(duplex);
    sal_port_duplex_get(lp, &duplex);
    if (ENABLED == duplex.autoDuplex)
    {
        cJSON_AddStringToObject(data, CGI_PORT_ADMIN_DUPLEX, "auto");
    }
    else
    {
        if (SYS_PORT_DUPLEX_FULL == duplex.ability)
            cJSON_AddStringToObject(data, CGI_PORT_ADMIN_DUPLEX, "full");
        else
            cJSON_AddStringToObject(data, CGI_PORT_ADMIN_DUPLEX, "half");
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

	portType = LP2TYPE(lp);
	if(portType == PORT_TYPE_10G)
	{
		cJSON_AddBoolToObject(data, CGI_PORT_SHOW_TENGI, ENABLED);
	}
	else
	{
		cJSON_AddBoolToObject(data, CGI_PORT_SHOW_TENGI, DISABLED);
	}

#if 0
	if(portType == PORT_TYPE_2500M)
	{
		cJSON_AddBoolToObject(data, CGI_PORT_SHOW_MulGI, ENABLED);
	}
	else
	{
		cJSON_AddBoolToObject(data, CGI_PORT_SHOW_MulGI, DISABLED);
	}
#endif 

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
static int32 cgi_port_errDisableStatus_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    uint32 time = 0;
    sys_logic_port_t lp;
    sys_enable_t enable = DISABLED;
    sys_port_errDisable_t reason;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    FOR_EACH_LOGIC_PORT(lp)
    {
        
        sal_port_errDisable_get(lp, &reason);
        sal_port_errDisableCurrTime_get(lp, &time);
        sal_port_errDisableRecovery_get(reason, &enable);

        port = cJSON_CreateObject();
        cJSON_AddItemToArray(ports, port);

        if(ENABLED == enable)
        {
            cJSON_AddLangStringToObject(port, CGI_PORT_ERRDISABLE_REASON, "port", cgi_port_errDisableTxt[reason].text);
            cJSON_AddNumberToObject(port, CGI_PORT_ERRDISABLE_TIMELEFT, time);
        }
        else
        {
            cJSON_AddStringToObject(port, CGI_PORT_ERRDISABLE_REASON, "");
            cJSON_AddNumberToObject(port, CGI_PORT_ERRDISABLE_TIMELEFT, 0);
        }
    }

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_port_errDisable_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *reasons = NULL;
    cJSON *reason = NULL;
    uint32 i = 0;
    uint32 interval = 0;
    sys_enable_t enable = DISABLED;

    sal_port_errDisableTime_get(&interval);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_PORT_ERRDISABLE_INTERVAL, interval);
    cJSON_AddNumberToObject(data, CGI_PORT_ERRDISABLE_INTERVAL_MIN, BOND_PORT_ERRDISABLE_RECOVERY_INTERVAL_MIN);
    cJSON_AddNumberToObject(data, CGI_PORT_ERRDISABLE_INTERVAL_MAX, BOND_PORT_ERRDISABLE_RECOVERY_INTERVAL_MAX);

    reasons = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORT_ERRDISABLE_REASONS, reasons);

    for (i = (SYS_PORT_ERRDISABLE_NONE + 1); i < SYS_PORT_ERRDISABLE_END; i++)
    {
        sal_port_errDisableRecovery_get(i, &enable);

        reason = cJSON_CreateObject();
        cJSON_AddItemToArray(reasons, reason);

        cJSON_AddStringToObject(reason, CGI_NAME, cgi_port_errDisable[i].text);
        cJSON_AddLangStringToObject(reason, "txt", "port", cgi_port_errDisableTxt[i].text);
        cJSON_AddBoolToObject(reason, CGI_ENABLE, enable);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_JUMBO_FRAME
static int32 cgi_port_jumbo_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    uint32 maxSize = 0;

    sal_switch_maxFrameSize_get(&maxSize);

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_PORT_JUMBO_SIZE, maxSize);
    cJSON_AddNumberToObject(data, CGI_PORT_JUMBO_SIZE_MIN, BOND_PORT_JUMBO_FRAME_MIN);
    cJSON_AddNumberToObject(data, CGI_PORT_JUMBO_SIZE_MAX, BOND_PORT_JUMBO_FRAME_MAX);
    cJSON_AddNumberToObject(data, CGI_PORT_JUMBO_SIZE_DFLT, DFLT_PORT_MAX_FRAME_SIZE);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_EEE
int32 cgi_port_eee_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    sys_enable_t enable = DISABLED;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_eee_portEnable_get(lp, &enable);
        cJSON_AddBoolToObject(port, CGI_EEE_ADMINSTATE, enable);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

int32 cgi_port_eeeEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_logic_port_t lp = 0;
    sys_enable_t enable = DISABLED;
    char *value = NULL;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_eee_portEnable_get(lp, &enable);
    cJSON_AddBoolToObject(data, CGI_EEE_ADMINSTATE, enable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PORT_ISOLATION
int32 cgi_port_protect_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    sys_enable_t enable = DISABLED;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_port_protected_get(lp, &enable);
        cJSON_AddBoolToObject(port, CGI_PORT_PROTECT, enable);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

int32 cgi_port_protectEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_logic_port_t lp = 0;
    sys_enable_t enable = DISABLED;
    char *value = NULL;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_port_protected_get(lp, &enable);
    cJSON_AddBoolToObject(data, CGI_PORT_PROTECT, enable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

int32 cgi_port_cnt_get(void)
{
    cJSON            *root = NULL;
    cJSON            *data = NULL;
    cJSON            *cnts_root = NULL;
    cJSON            *type = NULL;
    cJSON            *cnts = NULL;
    cJSON            *cnt = NULL;
    char             *value = NULL;
    int32            ret = SYS_ERR_FAILED;
    uint32           i = 0;
    uint32           j = 0;
    uint64           result = 0;
    sys_logic_port_t lp = 0;

    CGI_GET_JSON_CREATE(root, data);

    if (CGI_IS_PARAM_EXIST(CGI_SEL_PORT, value))
        lp = SYS_STR2UINT(value);

    cnts_root = cJSON_CreateArray();

    i = 0;

    while(user_mibtbls[i].name != NULL)
    {
        j = 0;

        type = cJSON_CreateObject();

        if (i == 0)
            cJSON_AddLangStringToObject(type, CGI_PORT_MIBTYPE, "port", "lblIntf");
        else if (i == 1)
            cJSON_AddLangStringToObject(type, CGI_PORT_MIBTYPE, "port", "lblEther");
        else if (i == 2)
            cJSON_AddLangStringToObject(type, CGI_PORT_MIBTYPE, "port", "lblRmon");

        cnts = cJSON_CreateArray();

        while(user_mibtbls[i].mib_cntr[j].name !=NULL)
        {
            cnt = cJSON_CreateObject();

            result = 0;
            if (user_mibtbls[i].mib_cntr[j].sal_mib_func != NULL)
                ret = user_mibtbls[i].mib_cntr[j].sal_mib_func(lp, &result);

            if(ret == SYS_ERR_OK)
            {
                cJSON_AddStringToObject(cnt, CGI_NAME, user_mibtbls[i].mib_cntr[j].name);
                cJSON_AddNumberToObject(cnt, CGI_VALUE, result);
            }

            cJSON_AddItemToArray(cnts, cnt);

            j++;
        }

        cJSON_AddItemToObject(type, CGI_PORT_CNTS, cnts);

        cJSON_AddItemToArray(cnts_root, type);

        i++;
    }

    if (i > 0)
        cJSON_AddItemToObject(data, CGI_PORT_CNTS, cnts_root);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_BW_UTILIZATION
int32 cgi_port_bwUtilz_get(void)
{
#define IFG             20
#define HUNDRED_PERCENT 100
#define TYPE_NUM        5
    typedef enum bw_type_e
    {
        BW_TYPE_10G = 0,
        BW_TYPE_1000M,
        BW_TYPE_100M,
        BW_TYPE_10M,
        BW_TYPE_LD
    } bw_type_t;
    const sys_text_t text_bw_portLabel[] =
    {
        { CGI_PORT_BW_LABEL_10G       },
        { CGI_PORT_BW_LABEL_1000M     },
        { CGI_PORT_BW_LABEL_100M      },
        { CGI_PORT_BW_LABEL_10M       },
        { CGI_PORT_BW_LABEL_LD        },
    };

    const sys_text_t text_bw_portColor[] =
    {
        { CGI_PORT_BW_COLOR_10G       },
        { CGI_PORT_BW_COLOR_1000M     },
        { CGI_PORT_BW_COLOR_100M      },
        { CGI_PORT_BW_COLOR_10M       },
        { CGI_PORT_BW_COLOR_LD        },
    };

    cJSON            *root = NULL;
    cJSON            *data = NULL;
    cJSON            *ticks = NULL;
    cJSON            *tick = NULL;
    cJSON            *plotDataTx;
    cJSON            *plotDataRx;
    cJSON            *txType[TYPE_NUM];
    cJSON            *rxType[TYPE_NUM];
    cJSON            *dataTx[TYPE_NUM];
    cJSON            *dataRx[TYPE_NUM];
    cJSON            *dataTmp;
    
    uint32           typeDataCntTx[TYPE_NUM];
    uint32           typeDataCntRx[TYPE_NUM];
    uint32           i = 0;
    sys_logic_port_t lp = 0;
    char             portStr[CAPA_PORT_STR_LEN + 1];
    bw_type_t        currType = 0;
    uint32           jiffies_1;
    uint32           jiffies_2;
    int32            hz;
    uint64           tmpTxPkts;
    sys_port_linkStatus_t linkStatus_1[SYS_NORMALPORT_NUM_MAX];
    sys_port_speedDuplex_t speedDuplex_1[SYS_NORMALPORT_NUM_MAX];
    uint64           rxPkts_1[SYS_NORMALPORT_NUM_MAX];
    uint64           rxOctets_1[SYS_NORMALPORT_NUM_MAX];
    uint64           txPkts_1[SYS_NORMALPORT_NUM_MAX];
    uint64           txOctets_1[SYS_NORMALPORT_NUM_MAX];
    sys_port_linkStatus_t linkStatus_2[SYS_NORMALPORT_NUM_MAX];
    sys_port_speedDuplex_t speedDuplex_2[SYS_NORMALPORT_NUM_MAX];
    uint64           rxPkts_2[SYS_NORMALPORT_NUM_MAX];
    uint64           rxOctets_2[SYS_NORMALPORT_NUM_MAX];
    uint64           txPkts_2[SYS_NORMALPORT_NUM_MAX];
    uint64           txOctets_2[SYS_NORMALPORT_NUM_MAX];
    double           diff;
    double           percent;
    uint64           flow;
    uint64           lineRate = 0;
    uint32           rxUtil[SYS_NORMALPORT_NUM_MAX];
    uint32           txUtil[SYS_NORMALPORT_NUM_MAX];

    SYS_MEM_CLEAR(txType);
    SYS_MEM_CLEAR(rxType);
    SYS_MEM_CLEAR(dataTx);
    SYS_MEM_CLEAR(dataRx);
    SYS_MEM_CLEAR(plotDataTx);
    SYS_MEM_CLEAR(plotDataRx);
    SYS_MEM_CLEAR(typeDataCntTx);
    SYS_MEM_CLEAR(typeDataCntRx);
    SYS_MEM_CLEAR(linkStatus_1);
    SYS_MEM_CLEAR(speedDuplex_1);
    SYS_MEM_CLEAR(rxPkts_1);
    SYS_MEM_CLEAR(rxOctets_1);
    SYS_MEM_CLEAR(txPkts_1);
    SYS_MEM_CLEAR(txOctets_1);
    SYS_MEM_CLEAR(linkStatus_2);
    SYS_MEM_CLEAR(speedDuplex_2);
    SYS_MEM_CLEAR(rxPkts_2);
    SYS_MEM_CLEAR(rxOctets_2);
    SYS_MEM_CLEAR(txPkts_2);
    SYS_MEM_CLEAR(txOctets_2);
    SYS_MEM_CLEAR(rxUtil);
    SYS_MEM_CLEAR(txUtil);

    sal_sys_hz_get(&hz);
    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        SYS_ERR_CHK(sal_port_linkStatus_get(lp, &linkStatus_1[lp]));
        if (PORT_LINKDOWN == linkStatus_1[lp])
            continue;
        sal_port_speedDuplex_get(lp, &speedDuplex_1[lp]);
        sal_mib_etherStatsPkts_get(lp, &rxPkts_1[lp]);
        sal_mib_etherStatsOctets_get(lp, &rxOctets_1[lp]);
        tmpTxPkts = 0;
        sal_mib_ifOutUcastPkts_get(lp, &tmpTxPkts);
        txPkts_1[lp] = tmpTxPkts;
        tmpTxPkts = 0;
        sal_mib_ifOutNUcastPkts_get(lp, &tmpTxPkts);
        txPkts_1[lp] += tmpTxPkts;
        sal_mib_ifOutOctets_get(lp, &txOctets_1[lp]);
    }
    sal_sys_jiffies_get(&jiffies_1);
    
    usleep(500000);
    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        sal_port_linkStatus_get(lp, &linkStatus_2[lp]);
        if (PORT_LINKDOWN == linkStatus_2[lp])
            continue;
        sal_port_speedDuplex_get(lp, &speedDuplex_2[lp]);
        sal_mib_etherStatsPkts_get(lp, &rxPkts_2[lp]);
        sal_mib_etherStatsOctets_get(lp, &rxOctets_2[lp]);
        tmpTxPkts = 0;
        sal_mib_ifOutUcastPkts_get(lp, &tmpTxPkts);
        txPkts_2[lp] = tmpTxPkts;
        tmpTxPkts = 0;
        sal_mib_ifOutNUcastPkts_get(lp, &tmpTxPkts);
        txPkts_2[lp] += tmpTxPkts;
        sal_mib_ifOutOctets_get(lp, &txOctets_2[lp]);
    }
    sal_sys_jiffies_get(&jiffies_2);

    diff = ((int32)jiffies_2 - (int32)jiffies_1) * 1000 / hz;
    diff /= 1000;

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        if (linkStatus_2[lp] == PORT_LINKDOWN)
            lineRate = 10000000;
        else if (speedDuplex_2[lp].speed == PORT_SPEED_10M)
            lineRate = 10000000;
        else if (speedDuplex_2[lp].speed == PORT_SPEED_100M)
            lineRate = 100000000;
        else if (speedDuplex_2[lp].speed == PORT_SPEED_1000M)
            lineRate = 1000000000;
        else if (speedDuplex_2[lp].speed == PORT_SPEED_10G)
            lineRate = 10000000000;

        flow = rxOctets_2[lp] - rxOctets_1[lp];

        flow += (rxPkts_2[lp] - rxPkts_1[lp]) * IFG;

        percent = ((double)((int64)flow * 8)) / diff / (int64)lineRate;

        rxUtil[lp] = (uint32)(percent * HUNDRED_PERCENT);

        flow = txOctets_2[lp] - txOctets_1[lp];

        flow += (txPkts_2[lp] - txPkts_1[lp]) * IFG;

        percent = ((double)((int64)flow * 8)) / diff / (int64)lineRate;

        txUtil[lp] = (uint32)(percent * HUNDRED_PERCENT);
    }

    CGI_GET_JSON_CREATE(root, data);

    ticks = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORT_BW_TICKS, ticks);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(portStr);
        LP2WEBSTR(lp, portStr);

        tick = cJSON_CreateArray();
        cJSON_AddItemToArray(tick, cJSON_CreateNumber(lp));
        cJSON_AddItemToArray(tick, cJSON_CreateString(portStr));

        cJSON_AddItemToArray(ticks, tick);
    }

    tick = cJSON_CreateArray();
    cJSON_AddItemToArray(tick, cJSON_CreateNumber(lp));
    cJSON_AddItemToArray(tick, cJSON_CreateString(" "));
    cJSON_AddItemToArray(ticks, tick);

    plotDataTx = cJSON_CreateArray();
    plotDataRx = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORT_BW_DATATX, plotDataTx);
    cJSON_AddItemToObject(data, CGI_PORT_BW_DATARX, plotDataRx);

    for (i = 0; i < TYPE_NUM; i++)
    {
        txType[i] = cJSON_CreateObject();
        rxType[i] = cJSON_CreateObject();
        cJSON_AddStringToObject(txType[i], CGI_PORT_BW_LABEL, text_bw_portLabel[i].text);
        cJSON_AddStringToObject(rxType[i], CGI_PORT_BW_LABEL, text_bw_portLabel[i].text);
        cJSON_AddStringToObject(txType[i], CGI_PORT_BW_COLOR, text_bw_portColor[i].text);
        cJSON_AddStringToObject(rxType[i], CGI_PORT_BW_COLOR, text_bw_portColor[i].text);
        cJSON_AddItemToArray(plotDataTx, txType[i]);
        cJSON_AddItemToArray(plotDataRx, rxType[i]);
    }

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        if (linkStatus_2[lp] == PORT_LINKDOWN)
            currType = BW_TYPE_LD;
        else if (speedDuplex_2[lp].speed == PORT_SPEED_10M)
            currType = BW_TYPE_10M;
        else if (speedDuplex_2[lp].speed == PORT_SPEED_100M)
            currType = BW_TYPE_100M;
        else if (speedDuplex_2[lp].speed == PORT_SPEED_1000M)
            currType = BW_TYPE_1000M;
        else if (speedDuplex_2[lp].speed == PORT_SPEED_10G)
            currType = BW_TYPE_10G;

        if (0 == typeDataCntTx[currType])
        {
            dataTx[currType] = cJSON_CreateArray();
            cJSON_AddItemToObject(txType[currType], CGI_PORT_BW_DATA, dataTx[currType]);
        }

        dataTmp = cJSON_CreateArray();
        cJSON_AddItemToArray(dataTmp, cJSON_CreateNumber(txUtil[lp]));
        cJSON_AddItemToArray(dataTmp, cJSON_CreateNumber(lp));
        cJSON_AddItemToArray(dataTx[currType], dataTmp);
        typeDataCntTx[currType]++;

        if (0 == typeDataCntRx[currType])
        {
            dataRx[currType] = cJSON_CreateArray();
            cJSON_AddItemToObject(rxType[currType], CGI_PORT_BW_DATA, dataRx[currType]);
        }

        dataTmp = cJSON_CreateArray();
        cJSON_AddItemToArray(dataTmp, cJSON_CreateNumber(rxUtil[lp]));
        cJSON_AddItemToArray(dataTmp, cJSON_CreateNumber(lp));
        cJSON_AddItemToArray(dataRx[currType], dataTmp);
        typeDataCntRx[currType]++;
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

