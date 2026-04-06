
#include <include/cgi.h>
#include <libsal/sal_dos.h>

static int32 cgi_dos_prop_set(void);
static int32 cgi_dos_portEdit_set(void);

const cgi_cmdmap_t cmdSetDosTbl[] =
{
    {CGI_CMD_DOS_PROP,         cgi_dos_prop_set},
    {CGI_CMD_DOS_PORTEDIT,     cgi_dos_portEdit_set},
    {NULL, NULL}
};

static int32 cgi_dos_prop_set(void)
{
    int32  ret = SYS_ERR_OK;
    char   *val;

    if (CGI_IS_PARAM_EXIST(CGI_DOS_TCPSYNSPORT1024, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_SYN_SPORTL1024, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_SYN_SPORTL1024, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_TCPFRAGOFFMINCHK, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_TCP_FRAG_OFF_MIN, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_TCP_FRAG_OFF_MIN, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_XMA, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_XMA, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_XMA, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_NULLSCAN, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_NULLSCAN, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_NULLSCAN, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_ICMPFRAGPKT, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_ICMP_FRAG_PKTS, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_ICMP_FRAG_PKTS, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_POD, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_POD, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_POD, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_TCPBLAT, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_TCPBLAT, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_TCPBLAT, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_UDPBLAT, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_UDPBLAT, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_UDPBLAT, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_LAND, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_LAND, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_LAND, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_DAEQSA, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_DAEQSA, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_DAEQSA, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_TCPHDRMINCHK, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_TCPHDR_MIN, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_TCPHDR_MIN, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_DAEQSA, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_DAEQSA, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_DAEQSA, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_TCPHDRMINCHKLENGTH, val))
        sal_dos_minTCPHdrLen_set(SYS_STR2UINT(val));

    if (CGI_IS_PARAM_EXIST(CGI_DOS_V4PINGMAX, val))
    {
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_IPV4_PING_MAX, ENABLED), ret);
    }
    else
    {
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_IPV4_PING_MAX, DISABLED), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_DOS_V6PINGMAX, val))
    {
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_IPV6_PING_MAX, ENABLED), ret);
    }
    else
    {
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_IPV6_PING_MAX, DISABLED), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_DOS_PINGMAXLENGTH, val))
        sal_dos_maxPingLen_set(SYS_STR2UINT(val));

    if (CGI_IS_PARAM_EXIST(CGI_DOS_SMURF, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_SMURF, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_SMURF, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_SMURFLENGTH, val))
        sal_dos_smurfNetmaskLen_set(SYS_STR2UINT(val));

    if (CGI_IS_PARAM_EXIST(CGI_DOS_IPV6MINFRAG, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_IPV6_MIN_FRAG_SIZE, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_IPV6_MIN_FRAG_SIZE, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_IPV6MINFRAGLENGTH, val))
        sal_dos_minIPv6FragLen_set(SYS_STR2UINT(val));

    if (CGI_IS_PARAM_EXIST(CGI_DOS_SYNFIN, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_SYNFIN, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_SYNFIN, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_SYNRST, val))
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_SYNRST, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_dos_typeEnable_set(DOS_SYNRST, DISABLED), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dos_portEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    sys_enable_t enable = DISABLED;
    char *value = NULL;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    if (CGI_IS_PARAM_EXIST(CGI_DOS_STATUS, value))
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_dos_portAttackPreventEnable_set(lp, enable), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

