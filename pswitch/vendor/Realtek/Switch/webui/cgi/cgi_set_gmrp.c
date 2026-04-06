
#include <include/cgi.h>

static int32 cgi_gmrp_set(void);
static int32 cgi_gmrp_intfEdit_set(void);

const cgi_cmdmap_t cmdSetGmrpTbl[] =
{
    {CGI_CMD_GMRP,          cgi_gmrp_set},
    {CGI_CMD_GMRP_INTFEDIT,      cgi_gmrp_intfEdit_set},
    {NULL, NULL}
};

static int32 cgi_gmrp_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_logic_port_t port;

    if (CGI_IS_PARAM_EXIST(CGI_GMRP_GLOBAL_STATUS, value))
        CGI_SET_ERR_HDL(sal_gmrp_enable_set(ENABLED), ret);
    else
  	{
  			FOR_EACH_LOGIC_PORT(port)
				{
			        sal_gmrp_port_enable_set(port, DISABLED);
				}
        CGI_SET_ERR_HDL(sal_gmrp_enable_set(DISABLED), ret);
  	}

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_gmrp_intfEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_logic_portmask_t lpm;
    sys_enable_t gmrpPortState = DISABLED;
    sys_logic_port_t lp;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
    {
        STR2LPM(value, &lpm);
    }

    if (CGI_IS_PARAM_EXIST(CGI_GMRP_PORT_EDIT_STATUS, value))
    {
        gmrpPortState = ENABLED;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_gmrp_port_enable_set(lp, gmrpPortState), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

