
#include <include/cgi.h>
#include <libsal/sal_udld.h>

static int32 cgi_udld_prop_set(void);
static int32 cgi_udld_portEdit_set(void);

const cgi_cmdmap_t cmdSetUdldTbl[] =
{
    {CGI_CMD_UDLD_PROP,          cgi_udld_prop_set},
    {CGI_CMD_UDLD_PORTEDIT,      cgi_udld_portEdit_set},
    {NULL, NULL}
};

static int32 cgi_udld_prop_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 txTime;

    if (CGI_IS_PARAM_EXIST(CGI_UDLD_TXINTV, value))
        txTime = SYS_STR2UINT(value);
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_udld_messageTime_set(txTime), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_udld_portEdit_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_udld_mode_t mode = UDLD_MODE_DISABLE;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_UDLD_PORTMODE, value))
        mode = SYS_STR2UINT(value);
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_udld_portMode_set(lp, mode), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

