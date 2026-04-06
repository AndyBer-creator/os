
#include <include/cgi.h>

static int32 cgi_gvrp_set(void);
static int32 cgi_gvrp_intfEdit_set(void);
static int32 cgi_gvrp_intfStat_clr_set(void);

const cgi_cmdmap_t cmdSetGvrpTbl[] =
{
    {CGI_CMD_GVRP,          cgi_gvrp_set},
    {CGI_CMD_GVRP_INTFEDIT,      cgi_gvrp_intfEdit_set},
    {CGI_CMD_GVRP_INTFCNTCLR,   cgi_gvrp_intfStat_clr_set},
    {NULL, NULL}
};

static int32 cgi_gvrp_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 join = DFLT_GVRP_JOIN_TIME;
    uint32 leave = DFLT_GVRP_LEAVE_TIME;
    uint32 leaveAll = DFLT_GVRP_LEAVEALL_TIME;
    char *value = NULL;

    if (CGI_IS_PARAM_EXIST(CGI_GVRP_GLOBAL_STATUS, value))
        CGI_SET_ERR_HDL(sal_gvrp_enable_set(ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_gvrp_enable_set(DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_GVRP_TIME_JOIN, value))
    {
        join = SYS_STR2UINT(value);
    }

    if (CGI_IS_PARAM_EXIST(CGI_GVRP_TIME_LEAVE, value))
    {
        leave = SYS_STR2UINT(value);
    }

    if (CGI_IS_PARAM_EXIST(CGI_GVRP_TIME_LEAVEALL, value))
    {
        leaveAll = SYS_STR2UINT(value);
    }

    CGI_SET_ERR_HDL(sal_gvrp_all_timer_set(join,leave,leaveAll),ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_gvrp_intfEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_logic_portmask_t lpm;

    sys_enable_t gvrpPortState = DISABLED, portForbidState = DISABLED;
    register_mode_t regMode = GVRP_NORMAL;
    sys_logic_port_t lp;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
    {
        STR2LPM(value, &lpm);
    }

    if (CGI_IS_PARAM_EXIST(CGI_GVRP_PORT_EDIT_STATUS, value))
    {
        gvrpPortState = ENABLED;
    }

    if (CGI_IS_PARAM_EXIST(CGI_GVRP_PORT_EDIT_CREATEVLAN_STATUS, value))
    {
        portForbidState = DISABLED;
    }
    else
        portForbidState = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_GVRP_PORT_EDIT_REGMODE, value))
    {
        if (CGI_IS_VALUE_EQUAL(value, "Normal"))
            regMode = GVRP_NORMAL;
        else if (CGI_IS_VALUE_EQUAL(value, "Fixed"))
            regMode = GVRP_FIXED;
        else
            regMode = GVRP_FORBIDDEN;
    }
    else
    {
        regMode = GVRP_NORMAL;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_gvrp_port_enable_set(lp, gvrpPortState), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_gvrp_port_forbidVlan_set(lp, portForbidState), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_gvrp_port_regmode_set(lp, regMode), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_gvrp_intfStat_clr_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_logic_port_t lp;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_PORT, value))
    {
        lp = SYS_STR2UINT(value);

        CGI_SET_ERR_HDL(sal_gvrp_statistics_clear(lp), ret);
        CGI_SET_ERR_HDL(sal_gvrp_error_statistics_clear(lp), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

