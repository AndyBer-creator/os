
#include <include/cgi.h>

static int32 cgi_storm_intf_set(void);
static int32 cgi_storm_intfEdit_set(void);

const cgi_cmdmap_t cmdSetStormTbl[] =
{
    {CGI_CMD_STORM,        cgi_storm_intf_set},
    {CGI_CMD_STORMEDIT,    cgi_storm_intfEdit_set},
    {NULL, NULL}
};

static int32 cgi_storm_intf_set(void)
{
    int32               ret = SYS_ERR_OK;
    char               *val;

    if (CGI_IS_PARAM_EXIST("unit", val))
    {
        if (BASED_ON_PKT == SYS_STR2UINT(val))
            CGI_SET_ERR_HDL(sal_rate_stormCtrlRefreshMode_set(BASED_ON_PKT), ret);
        else
            CGI_SET_ERR_HDL(sal_rate_stormCtrlRefreshMode_set(BASED_ON_BYTE), ret);
    }

    if (CGI_IS_PARAM_EXIST("type", val))
    {
        if (DISABLED == SYS_STR2UINT(val))
            CGI_SET_ERR_HDL(sal_rate_stormCtrlIncludeIfg_set(DISABLED), ret);
        else
            CGI_SET_ERR_HDL(sal_rate_stormCtrlIncludeIfg_set(ENABLED), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_storm_intfEdit_set(void)
{
    int32                       ret = SYS_ERR_OK;
    sys_logic_port_t            lp;
    sys_logic_portmask_t        lpm;
    char                       *val;
    sys_enable_t                state, bcEnable, umcEnable, uncEnable;
    sys_rate_storm_action_t     action = STORM_ACTION_NONE;
    uint32                      broadcast, unknownMC, unknownUC;

    SYS_MEM_CLEAR(lpm);
    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, val))
        STR2LPM(val, &lpm);

    if (IS_LOGIC_PORTMASK_CLEAR(lpm))
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST("state", val))
        state = ENABLED;
    else
        state = DISABLED;

    if (CGI_IS_PARAM_EXIST("bcEnable", val))
        bcEnable = ENABLED;
    else
        bcEnable = DISABLED;

    if (CGI_IS_PARAM_EXIST("broadcast", val))
        broadcast = SYS_STR2UINT(val);
    else
        broadcast = DFLT_RATE_STORM_CTRL;

    if (CGI_IS_PARAM_EXIST("umcEnable", val))
        umcEnable = ENABLED;
    else
        umcEnable = DISABLED;

    if (CGI_IS_PARAM_EXIST("unknownMC", val))
        unknownMC = SYS_STR2UINT(val);
    else
        unknownMC = DFLT_RATE_STORM_CTRL;

    if (CGI_IS_PARAM_EXIST("uncEnable", val))
        uncEnable = ENABLED;
    else
        uncEnable = DISABLED;

    if (CGI_IS_PARAM_EXIST("unknownUC", val))
        unknownUC = SYS_STR2UINT(val);
    else
        unknownUC = DFLT_RATE_STORM_CTRL;

    if (CGI_IS_PARAM_EXIST("action", val))
    {
        if (SYS_STR2UINT(val))
            action = STORM_ACTION_SHUTDOWN;
        else
            action = STORM_ACTION_NONE;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_rate_stormControlPortEnable_set(lp, state), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_rate_stormControlTypeEnable_set(lp, STORM_GROUP_BROADCAST, bcEnable), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_rate_stormControlRate_set(lp, STORM_GROUP_BROADCAST, broadcast), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_rate_stormControlTypeEnable_set(lp, STORM_GROUP_MULTICAST, umcEnable), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_rate_stormControlRate_set(lp, STORM_GROUP_MULTICAST, unknownMC), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_rate_stormControlTypeEnable_set(lp, STORM_GROUP_UNICAST, uncEnable), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_rate_stormControlRate_set(lp, STORM_GROUP_UNICAST, unknownUC), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_rate_stormControlAction_set(lp, action), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

