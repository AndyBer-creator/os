
#include <include/cgi.h>

static int32 cgi_stp_global_set(void);
static int32 cgi_stp_intfEdit_set(void);
static int32 cgi_stp_intfMigrate_set(void);
static int32 cgi_stp_statsIntfClear_set(void);
#ifdef CONFIG_SYS_PROTO_STP_MSTP
static int32 cgi_stp_mstiEdit_set(void);
static int32 cgi_stp_mstiIntfEdit_set(void);
#endif

const cgi_cmdmap_t cmdSetStpTbl[] =
{
    { CGI_CMD_STP_GLOBAL,           cgi_stp_global_set          },
    { CGI_CMD_STP_INTF_EDIT,        cgi_stp_intfEdit_set        },
    { CGI_CMD_STP_INTF_MIGRATE,     cgi_stp_intfMigrate_set     },
    { CGI_CMD_STP_STATS_INTF_CLEAR, cgi_stp_statsIntfClear_set  },
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    { CGI_CMD_STP_MSTI_EDIT,        cgi_stp_mstiEdit_set        },
    { CGI_CMD_STP_MSTI_INTF_EDIT,   cgi_stp_mstiIntfEdit_set    },
#endif
    { NULL,                         NULL                        }
};

static int32 cgi_stp_global_set(void)
{
    char   *pStrValue;
    int32  ret = SYS_ERR_OK;
    uint32 val = 0;
    uint32 fwdDelay = DFLT_STP_FORWARD_DELAY;
    uint32 maxAge = DFLT_STP_MAX_AGE;
    uint32 helloTime = DFLT_STP_HELLO_TIME;
    sys_enable_t enable = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_STP_ENABLE, pStrValue))
        enable = ENABLED;
    else
        enable = DISABLED;

    CGI_SET_ERR_HDL(sal_stp_enable_set(enable), ret);

    if (CGI_IS_PARAM_EXIST(CGI_STP_VERSION, pStrValue))
    {
        if (0 == osal_strcmp(pStrValue, "stp"))
            val = STP_VERSION_STP;
        else if (0 == osal_strcmp(pStrValue, "rstp"))
            val = STP_VERSION_RSTP;
        else if (0 == osal_strcmp(pStrValue, "mstp"))
            val = STP_VERSION_MSTP;
        else
            val = STP_VERSION_STP;

        CGI_SET_ERR_HDL(sal_stp_forceVersion_set(val), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_STP_BPDU_FORWARD, pStrValue))
    {
        enable  = DFLT_STP_BPDU_FORWARD_EBL;

        if (0 == osal_strcmp(pStrValue, "filtering"))
            enable = DISABLED;
        else if (0 == osal_strcmp(pStrValue, "flooding"))
            enable = ENABLED;

        CGI_SET_ERR_HDL(sal_stp_forwardBpdu_set(enable), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_STP_PATHCOST_METHOD, pStrValue))
    {
        uint32 val  = DFLT_STP_PATHCOST_METHOD;

        if (0 == osal_strcmp(pStrValue, "short"))
            val = STP_PATHCOST_METHOD_SHORT;
        else if (0 == osal_strcmp(pStrValue, "long"))
            val = STP_PATHCOST_METHOD_LONG;

        CGI_SET_ERR_HDL(sal_stp_pathcostMethod_set(val), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_STP_PRIORITY, pStrValue))
    {
        val = SYS_STR2UINT(pStrValue);
        CGI_SET_ERR_HDL(sal_stp_priority_set(val), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_STP_HELLO_TIME, pStrValue))
    {
        helloTime = SYS_STR2UINT(pStrValue);
    }

    if (CGI_IS_PARAM_EXIST(CGI_STP_MAX_AGE, pStrValue))
    {
        maxAge = SYS_STR2UINT(pStrValue);
    }

    if (CGI_IS_PARAM_EXIST(CGI_STP_FORWARD_DELAY, pStrValue))
    {
        fwdDelay = SYS_STR2UINT(pStrValue);
    }

    CGI_SET_ERR_HDL(sal_stp_timer_set(helloTime,fwdDelay,maxAge),ret);

    if (CGI_IS_PARAM_EXIST(CGI_STP_TX_HOLD_COUNT, pStrValue))
    {
        val = SYS_STR2UINT(pStrValue);
        CGI_SET_ERR_HDL(sal_stp_txHoldCount_set(val), ret);
    }

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    
    if (CGI_IS_PARAM_EXIST(CGI_STP_MSTP_REGION_NAME, pStrValue))
    {
        CGI_SET_ERR_HDL(sal_stp_mstConfigName_set(pStrValue), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_STP_MSTP_REVISION, pStrValue))
    {
        val = SYS_STR2UINT(pStrValue);
        CGI_SET_ERR_HDL(sal_stp_mstConfigRevision_set(val), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_STP_MSTP_MAXHOP, pStrValue))
    {
        val = SYS_STR2UINT(pStrValue);
        CGI_SET_ERR_HDL(sal_stp_maxHops_set(val), ret);
    }
#endif

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;

}

static int32 cgi_stp_intfEdit_set(void)
{
    char   *pStrValue;

    int32  ret = SYS_ERR_OK;

    uint32 pathcost = DFLT_STP_PORT_PATHCOST;
    uint32 priority = DFLT_STP_PORT_PRI;
    uint32 edge = DFLT_STP_PORT_EDGE;
    uint32 p2p = DFLT_STP_PORT_P2P;

    sys_enable_t enable = DISABLED;
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    sys_enable_t bpduFilter = DISABLED;
#endif
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    sys_enable_t bpduGuard = DISABLED;
#endif

    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, pStrValue))
        STR2LPM(pStrValue, &lpm);

    if (IS_LOGIC_PORTMASK_CLEAR(lpm))
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_ENABLE, pStrValue))
        enable = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_PATHCOST, pStrValue))
        pathcost = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_PRIORITY, pStrValue))
        priority = SYS_STR2UINT(pStrValue);

	#if 0
    if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_EDGE, pStrValue))
        edge = STP_PORT_CONF_TRUE;
    else
        edge = STP_PORT_CONF_FALSE;
	#endif
	if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_EDGE, pStrValue))
    {
        if (0 == osal_strcmp(pStrValue, "auto"))
            edge = STP_PORT_CONF_AUTO;
        else if (0 == osal_strcmp(pStrValue, "enable"))
            edge = STP_PORT_CONF_TRUE;
        else if (0 == osal_strcmp(pStrValue, "disable"))
            edge = STP_PORT_CONF_FALSE;
    }

#ifdef CONFIG_SYS_STP_BPDU_FILTER
    
    if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_BPDU_FILTER, pStrValue))
        bpduFilter = ENABLED;
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD
    
    if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_BPDU_GUARD, pStrValue))
        bpduGuard = ENABLED;
#endif

    if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_P2P, pStrValue))
    {
        if (0 == osal_strcmp(pStrValue, "auto"))
            p2p = STP_PORT_CONF_AUTO;
        else if (0 == osal_strcmp(pStrValue, "enable"))
            p2p = STP_PORT_CONF_TRUE;
        else if (0 == osal_strcmp(pStrValue, "disable"))
            p2p = STP_PORT_CONF_FALSE;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_portAdminEnable_set(lp, enable), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_portPathCost_set(lp, pathcost), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_portPriority_set(lp, priority), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_portEdgePort_set(lp, edge), lp);
#ifdef CONFIG_SYS_STP_BPDU_FILTER
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_portBpduFilter_set(lp, bpduFilter), lp);
#endif
#ifdef CONFIG_SYS_STP_BPDU_GUARD
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_portBpduGuard_set(lp, bpduGuard), lp);
#endif
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_portPointToPointMac_set(lp, p2p), lp);
    }

    sleep(1);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_stp_intfMigrate_set(void)
{
    char *pStrValue;
    int32 ret = SYS_ERR_OK;

    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, pStrValue))
        STR2LPM(pStrValue, &lpm);

    if (IS_LOGIC_PORTMASK_CLEAR(lpm))
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_portMcheck_set(lp, TRUE), lp);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_stp_statsIntfClear_set(void)
{
    char *pStrValue;
    int32 ret = SYS_ERR_OK;

    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, pStrValue))
        STR2LPM(pStrValue, &lpm);

    if (IS_LOGIC_PORTMASK_CLEAR(lpm))
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_portCounter_clear(lp), lp);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP
static int32 cgi_stp_mstiEdit_set(void)
{
    int32  ret = SYS_ERR_OK;
    uint32 msti = 0;
    uint32 pri = DFLT_STP_MST_PRI;
    char *pStrValue;

    sys_vid_t vid;
    sys_vlanmask_t vlanmask;
    sys_vlanmask_t vlanmask_old;

    SYS_MEM_CLEAR(vlanmask);
    SYS_MEM_CLEAR(vlanmask_old);

    if (CGI_IS_PARAM_EXIST(CGI_STP_MSTI, pStrValue))
        msti = SYS_STR2UINT(pStrValue);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VLANLIST, pStrValue))
        STR2VMSK(pStrValue, &vlanmask);
    else if (MSTP_INSTANCE_CIST != msti)
        goto cgi_end;

    if (MSTP_INSTANCE_CIST != msti)
    {
        ret = sal_stp_mstVlanmask_get(msti, &vlanmask_old);

        if (SYS_ERR_OK == ret)
        {
            FOR_EACH_VID_IN_VLANMASK(vid, vlanmask_old)
            {
                if (!VLANMASK_IS_VIDSET(vlanmask, vid))
                    CGI_SET_ERR_HDL(sal_stp_mstVlanInstance_set(vid, MSTP_INSTANCE_CIST), ret);
            }

            CGI_SET_ERR_HDL(sal_stp_mstVlanmask_set(msti, vlanmask), ret);
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_STP_PRIORITY, pStrValue))
    {
        pri = SYS_STR2UINT(pStrValue);

        CGI_SET_ERR_HDL(sal_stp_mstPriority_set(msti, pri), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_stp_mstiIntfEdit_set(void)
{
    int32  ret = SYS_ERR_OK;

    uint32 msti = 0;
    uint32 pri = DFLT_STP_MST_PRI;
    uint32 cost = DFLT_STP_PORT_PATHCOST;

    char   *pStrValue;

    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_STP_MSTI, pStrValue))
        msti = SYS_STR2UINT(pStrValue);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, pStrValue))
        STR2LPM(pStrValue, &lpm);

    if (IS_LOGIC_PORTMASK_CLEAR(lpm))
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_PATHCOST, pStrValue))
        cost = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_STP_PORT_PRIORITY, pStrValue))
        pri = SYS_STR2UINT(pStrValue);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_mstPortPathCost_set(msti, lp, cost), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_stp_mstPortPriority_set(msti, lp, pri), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif
