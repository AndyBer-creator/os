
#include <include/cgi.h>

static int32 cgi_lag_mgmt_set(void);
static int32 cgi_lag_mgmtEdit_set(void);
static int32 cgi_lag_portEdit_set(void);
#ifdef CONFIG_SYS_PROTO_LACP
static int32 cgi_lag_lacp_set(void);
static int32 cgi_lag_lacpEdit_set(void);
#endif

const cgi_cmdmap_t cmdSetLagTbl[] =
{
    {CGI_CMD_LAG_MGMT,          cgi_lag_mgmt_set},
    {CGI_CMD_LAG_MGMTEDIT,      cgi_lag_mgmtEdit_set},
    {CGI_CMD_LAG_PORTEDIT,      cgi_lag_portEdit_set},
#ifdef CONFIG_SYS_PROTO_LACP
    {CGI_CMD_LAG_LACP,          cgi_lag_lacp_set},
    {CGI_CMD_LAG_LACPEDIT,      cgi_lag_lacpEdit_set},
#endif
    {NULL, NULL}
};

static int32 cgi_lag_mgmt_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_trunk_hashAlgo_t algo = LAG_HASH_ALGO_MAC;

    if (CGI_IS_PARAM_EXIST(CGI_LAG_BALANCE_TYPE, value))
    {
        if (CGI_IS_VALUE_EQUAL(CGI_LAG_VAL_BALANCE_MAC, value))
            algo = LAG_HASH_ALGO_MAC;
        else if (CGI_IS_VALUE_EQUAL(CGI_LAG_VAL_BALANCE_IPMAC, value))
            algo = LAG_HASH_ALGO_IP_MAC;
        else if (CGI_IS_VALUE_EQUAL(CGI_LAG_VAL_BALANCE_DSTMAC, value))
            algo = LAG_HASH_ALGO_DMAC;
        else if (CGI_IS_VALUE_EQUAL(CGI_LAG_VAL_BALANCE_SRCMAC, value))
            algo = LAG_HASH_ALGO_SMAC;
        else if (CGI_IS_VALUE_EQUAL(CGI_LAG_VAL_BALANCE_DSTIP, value))
            algo = LAG_HASH_ALGO_DIP;
        else if (CGI_IS_VALUE_EQUAL(CGI_LAG_VAL_BALANCE_SRCIP, value))
            algo = LAG_HASH_ALGO_SIP;

        CGI_SET_ERR_HDL(sal_trunk_hashAlgo_set(algo), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_lag_mgmtEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 trkId = 0;
    char name[CAPA_PORTDESC_STR_LEN + 1];
    sys_enable_t lacpState = DISABLED;
    sys_logic_portmask_t lpmMbrs;
    sys_logic_portmask_t oldMbrPm;

    osal_memset(&lpmMbrs, 0, sizeof(lpmMbrs));
    osal_memset(&oldMbrPm, 0, sizeof(oldMbrPm));

    if (CGI_IS_PARAM_EXIST(CGI_SEL_LAG, value))
    {
        trkId = UID2TRK(SYS_STR2UINT(value));

        CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_LAG_MBRS, lpmMbrs);

        if (CGI_IS_PARAM_EXIST(CGI_LAG_LAG_TYPE, value))
        {
            if (CGI_IS_VALUE_EQUAL(value, CGI_LAG_TYPE_LACP))
                lacpState = ENABLED;
        }

        if (CGI_IS_PARAM_EXIST(CGI_LAG_PORT_NAME, value))
        {
            SYS_MEM_CLEAR(name);
            strlcpy(name, value, sizeof(name));

            CGI_SET_ERR_HDL(sal_port_descp_set(TRK2LP(trkId), name), ret);
        }
        else
            CGI_SET_ERR_HDL(sal_port_descp_set(TRK2LP(trkId), ""), ret);

        CGI_SET_ERR_HDL(cfg_trunk_member_get(trkId, &oldMbrPm),ret);

        if (ENABLED == lacpState)
        {
            
            if (!IS_NORMAL_LOGIC_PORTMASK_CLEAR(oldMbrPm) && !IS_TRK_LACP(trkId))
                CGI_SET_ERR_HDL(sal_trunk_group_del(trkId),ret);
#ifdef CONFIG_SYS_PROTO_LACP
            CGI_SET_ERR_HDL(sal_lacp_member_set(trkId, &lpmMbrs), ret);
            CGI_SET_ERR_HDL(sal_lacp_activeMode_set(trkId, &lpmMbrs), ret);
#endif
        }
        else
        {
            
            if (!IS_NORMAL_LOGIC_PORTMASK_CLEAR(oldMbrPm) && IS_TRK_LACP(trkId))
                SYS_ERR_CHK(sal_trunk_group_del(trkId));

            CGI_SET_ERR_HDL(sal_trunk_staticMember_set(trkId, &lpmMbrs), ret);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_lag_portEdit_set(void)
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

        if (0 == osal_strcmp(value, CGI_PORT_AUTO) || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_1000M)
            || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_10M100M) || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_100M)
            || 0 == osal_strcmp(value, CGI_PORT_SPEED_AUTO_10M))
        {
            speed.autoSpeed = ENABLED;
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
        CGI_SET_ERR_HDL(sal_port_descp_set(lp, portDescp), ret);
        CGI_SET_ERR_HDL(sal_port_enable_set(lp, state), ret);

        if(SYS_PORT_SPEED_10G == speed.ability)
        {
            
            duplex.autoDuplex = DISABLED;
            duplex.ability = SYS_PORT_DUPLEX_FULL;
            CGI_SET_ERR_HDL(sal_port_duplex_set(lp, duplex), ret);
        }
        CGI_SET_ERR_HDL(sal_port_speed_set(lp, speed), ret);

        if (ENABLED == flowCtrlAuto)
        {
            CGI_SET_ERR_HDL(sal_port_flowCtrlAuto_set(lp, ENABLED), ret);
        }
        else
        {
            CGI_SET_ERR_HDL(sal_port_flowCtrlAuto_set(lp, DISABLED), ret);
            CGI_SET_ERR_HDL(sal_port_flowCtrl_set(lp, flowCtrl), ret);
        }
    }

    sleep(4);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LACP
static int32 cgi_lag_lacp_set(void)
{
    int32                   ret = SYS_ERR_OK;
    char                    *val;
    uint16                  pri;

    if (CGI_IS_PARAM_EXIST(CGI_LAG_SYSPRI, val))
    {
        pri = SYS_STR2UINT(val);
        sal_lacp_systemPriority_set(pri);
    }
    else
    {
        goto cgi_end;
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_lag_lacpEdit_set(void)
{
    int32                   ret = SYS_ERR_OK;
    char                    *value;
    uint16                  pri;
    sys_logic_port_t        lp;
    sys_logic_portmask_t    lpm;
    sys_lacp_timeout_t      timeout;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        if (CGI_IS_PARAM_EXIST(CGI_LAG_PORTPRI, value))
        {
            pri = SYS_STR2UINT(value);
            sal_lacp_portPriority_set(lp, pri);
        }

        if (CGI_IS_PARAM_EXIST(CGI_LAG_TIMEOUT, value))
        {
            if (0 == osal_strcmp(value, "short"))
                timeout = LACP_TIMEOUT_SHORT;
            else
                timeout = LACP_TIMEOUT_LONG;

            sal_lacp_portTimeout_set(lp, timeout);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

