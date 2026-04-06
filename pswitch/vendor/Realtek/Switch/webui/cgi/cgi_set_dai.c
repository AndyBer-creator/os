
#include <include/cgi.h>

static int32 cgi_dai_prop_set(void);
static int32 cgi_dai_portEdit_set(void);
static int32 cgi_dai_statClear_set(void);

const cgi_cmdmap_t cmdSetDaiTbl[] =
{
    {CGI_CMD_DAI_PROP,          cgi_dai_prop_set},
    {CGI_CMD_DAI_PORTEDIT,      cgi_dai_portEdit_set},
    {CGI_CMD_DAI_STATISTIC,     cgi_dai_statClear_set},
    {NULL, NULL}
};

static int32 cgi_dai_prop_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    sys_vid_t vid;
    sys_vlanmask_t existVM;
    sys_vlanmask_t newVM;
    sys_enable_t enable = DISABLED;

    SYS_MEM_CLEAR(existVM);
    SYS_MEM_CLEAR(newVM);
    sal_dai_vlanmask_enable_get(&existVM);

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        enable = ENABLED;
    else
        enable = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_VLANLIST, value))
        STR2VMSK(value, &newVM);
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_dai_enable_set(enable), ret);

    FOR_EACH_VID_IN_VLANMASK(vid, newVM)
    {
	    CGI_SET_VID_ERR_CONTINUE(sal_dai_vlan_enable_set(vid, ENABLED), vid);
        VLANMASK_CLEAR_VID(existVM, vid);
    }

    FOR_EACH_VID_IN_VLANMASK(vid, existVM)
    {
        CGI_SET_VID_ERR_CONTINUE(sal_dai_vlan_enable_set(vid, DISABLED), vid);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dai_portEdit_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    uint32 rate = BOND_RATE_UNLIMITED;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_enable_t trust = DISABLED;
    sys_enable_t smacChk = DISABLED;
    sys_enable_t dmacChk = DISABLED;
    sys_enable_t ipChk = DISABLED;
    sys_enable_t allowZeros = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_DAI_TRUSTED, value))
        trust = ENABLED;
    else
        trust = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_DAI_VERISMAC, value))
        smacChk = ENABLED;
    else
        smacChk = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_DAI_VERIDMAC, value))
        dmacChk = ENABLED;
    else
        dmacChk = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_DAI_VERIIP, value))
        ipChk = ENABLED;
    else
        ipChk = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_DAI_ALLOWZEROS, value))
        allowZeros = ENABLED;
    else
        allowZeros = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_DAI_RATE, value))
    {
    	rate = SYS_STR2UINT(value);

    	if (0 == rate)
    	    rate = BOND_RATE_UNLIMITED;
    }
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_dai_port_trust_set(lp, trust), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_dai_port_smacChk_set(lp, smacChk), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_dai_port_dmacChk_set(lp, dmacChk), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_dai_port_ipChk_set(lp, ipChk), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_dai_port_ipZeroAllow_set(lp, allowZeros), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_dai_rateLimit_set(lp, rate), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dai_statClear_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_dai_portMib_clear(lp), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

