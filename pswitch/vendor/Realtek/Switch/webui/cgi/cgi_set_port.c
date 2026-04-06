
#include <include/cgi.h>
#include <libsal/sal_eee.h>
#include <libsal/sal_mib.h>

static int32 cgi_port_settingEdit_set(void);
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
static int32 cgi_port_errDisable_set(void);
static int32 cgi_port_errDisableRecover_set(void);
#endif
#ifdef CONFIG_SYS_JUMBO_FRAME
static int32 cgi_port_jumbo_set(void);
#endif
#ifdef CONFIG_SYS_EEE
static int32 cgi_port_eeeEdit_set(void);
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
static int32 cgi_port_protectEdit_set(void);
#endif
static int32 cgi_port_cntClear_set(void);

const cgi_cmdmap_t cmdSetPortTbl[] =
{
    {CGI_CMD_PORT_PORTEDIT,             cgi_port_settingEdit_set},
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    {CGI_CMD_PORT_ERRDISABLE,           cgi_port_errDisable_set},
    {CGI_CMD_PORT_ERRDISABLESTATUS,     cgi_port_errDisableRecover_set},
#endif
#ifdef CONFIG_SYS_JUMBO_FRAME
    {CGI_CMD_PORT_JUMBO,                cgi_port_jumbo_set},
#endif
#ifdef CONFIG_SYS_EEE
    {CGI_CMD_PORT_EEEEDIT,              cgi_port_eeeEdit_set},
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
    {CGI_CMD_PORT_PROTECTEDIT,          cgi_port_protectEdit_set},
#endif
    {CGI_CMD_PORT_CNTCLEAR,             cgi_port_cntClear_set},
    {NULL, NULL}
};

static int32 cgi_port_settingEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    char portDescp[CAPA_PORTDESC_STR_LEN + 1];
    sys_enable_t state = DISABLED;
    sys_enable_t flowCtrl = DISABLED;
    sys_enable_t flowCtrlAuto = DISABLED;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_port_duplex_t tmpFullDuplex;

    SYS_MEM_CLEAR(portDescp);
    SYS_MEM_CLEAR(lpm);
    SYS_MEM_CLEAR(speed);
    SYS_MEM_CLEAR(duplex);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORT_DESCP, value))
        strlcpy(portDescp, value, sizeof(portDescp));

    if (CGI_IS_PARAM_EXIST(CGI_PORT_ADMIN_STATUS, value))
        state = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_PORT_ADMIN_SPEED, value))
    {
        if (0 == osal_strcmp(value, CGI_PORT_AUTO))
            speed.ability = SYS_PORT_SPEED_ALL;
        else if (0 == osal_strcmp(value, CGI_PORT_SPEED_1000M) || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_1000M))
            speed.ability = SYS_PORT_SPEED_1000M;
        else if (0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_10M100M))
            speed.ability = SYS_PORT_SPEED_10M_100M;
        else if (0 == osal_strcmp(value, CGI_PORT_SPEED_100M) || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_100M))
            speed.ability = SYS_PORT_SPEED_100M;
        else if (0 == osal_strcmp(value, CGI_PORT_SPEED_10M) || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_10M))
            speed.ability = SYS_PORT_SPEED_10M;
        else if (0 == osal_strcmp(value, CGI_PORT_SPEED_10G))
            speed.ability = SYS_PORT_SPEED_10G;
#if 0
        else if (0 == osal_strcmp(value, CGI_PORT_SPEED_2500M))
            speed.ability = SYS_PORT_SPEED_2500M;
#endif 

        if (0 == osal_strcmp(value, CGI_PORT_AUTO) || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_1000M)
            || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_10M100M) || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_100M)
            || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_10M))
        {
            speed.autoSpeed = ENABLED;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_PORT_ADMIN_DUPLEX, value))
    {
        if (0 == osal_strcmp(value, CGI_PORT_AUTO))
        {
            duplex.autoDuplex = ENABLED;
        }
        else if (0 == osal_strcmp(value, CGI_PORT_DUPLEX_FULL))
        {
            duplex.autoDuplex = DISABLED;
            duplex.ability = SYS_PORT_DUPLEX_FULL;
        }
        else if (0 == osal_strcmp(value, CGI_PORT_DUPLEX_HALF))
        {
            duplex.autoDuplex = DISABLED;
            duplex.ability = SYS_PORT_DUPLEX_HALF;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_PORT_ADMIN_FLOWCTRL, value))
    {
        if (0 == osal_strcmp(value, CGI_PORT_AUTO))
            flowCtrlAuto = ENABLED;
        else if (0 == osal_strcmp(value, CGI_ENABLE))
            flowCtrl = ENABLED;
        else
            flowCtrl = DISABLED;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_port_descp_set(lp, portDescp), lp);

        tmpFullDuplex.autoDuplex = DISABLED;
        tmpFullDuplex.ability = SYS_PORT_DUPLEX_FULL;
        CGI_SET_PORT_ERR_CONTINUE(sal_port_duplex_set(lp, tmpFullDuplex), lp); 

        CGI_SET_PORT_ERR_CONTINUE(sal_port_speed_set(lp, speed), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_port_duplex_set(lp, duplex), lp);

        if (ENABLED == flowCtrlAuto)
        {
            CGI_SET_PORT_ERR_CONTINUE(sal_port_flowCtrlAuto_set(lp, ENABLED), lp);
        }
        else
        {
            CGI_SET_PORT_ERR_CONTINUE(sal_port_flowCtrlAuto_set(lp, DISABLED), lp);
            CGI_SET_PORT_ERR_CONTINUE(sal_port_flowCtrl_set(lp, flowCtrl), lp);
        }

        CGI_SET_PORT_ERR_CONTINUE(sal_port_enable_set(lp, state), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
static int32 cgi_port_errDisable_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 i = 0;
    sys_enable_t enable;
    const sys_text_t cgi_port_errDisable[] =
    {
        { ""                            },
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

    if (CGI_IS_PARAM_EXIST(CGI_PORT_ERRDISABLE_INTERVAL, value))
    {
        CGI_SET_ERR_HDL(sal_port_errDisableTime_set(SYS_STR2UINT(value)), ret);
    }

    for (i = (SYS_PORT_ERRDISABLE_NONE + 1); i < SYS_PORT_ERRDISABLE_END; i++)
    {
        if (CGI_IS_PARAM_EXIST((char*)cgi_port_errDisable[i].text, value))
            enable = ENABLED;
        else
            enable = DISABLED;

        CGI_SET_ENTRY_ERR_CONTINUE(sal_port_errDisableRecovery_set(i, enable), cgi_port_errDisable[i].text);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_port_errDisableRecover_set(void)
{
    char *pStrValue;
    int32 ret = SYS_ERR_OK;

    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, pStrValue))
        STR2LPM(pStrValue, &lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_port_errDisable_recover(lp), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_JUMBO_FRAME
static int32 cgi_port_jumbo_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    if (CGI_IS_PARAM_EXIST(CGI_PORT_JUMBO_SIZE, value))
    {
        CGI_SET_ERR_HDL(sal_switch_maxFrameSize_set(SYS_STR2UINT(value)), ret);
    }
    else
    {
        CGI_SET_ERR_HDL(sal_switch_maxFrameSize_set(DFLT_PORT_MAX_FRAME_SIZE), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_EEE
int32 cgi_port_eeeEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    sys_enable_t enable = DISABLED;
    char *value = NULL;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    if (CGI_IS_PARAM_EXIST(CGI_EEE_STATE, value))
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_eee_portEnable_set(lp, enable), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PORT_ISOLATION
int32 cgi_port_protectEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    sys_enable_t enable = DISABLED;
    char *value = NULL;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORT_PROTECT, value))
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_port_protected_set(lp, enable), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_port_cntClear_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_logic_port_t lp;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_PORT, value))
    {
        lp = SYS_STR2UINT(value);

        CGI_SET_ERR_HDL(sal_mib_cntr_clr(lp), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

