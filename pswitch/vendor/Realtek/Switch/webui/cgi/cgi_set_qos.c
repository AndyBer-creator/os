
#include <include/cgi.h>

static int32 cgi_qos_prop_set(void);
static int32 cgi_qos_portEdit_set(void);
static int32 cgi_qos_queue_set(void);
static int32 cgi_qos_mapCos_set(void);
static int32 cgi_qos_mapDscp_set(void);
static int32 cgi_qos_mapIpp_set(void);
#ifdef CONFIG_SYS_BW_CTRL
static int32 cgi_qos_rateCtrlEdit_set(void);
static int32 cgi_qos_rateEgressQueueEdit_set(void);
#endif

const cgi_cmdmap_t cmdSetQosTbl[] =
{
    {CGI_CMD_QOS_PROP,             cgi_qos_prop_set},
    {CGI_CMD_QOS_PORTEDIT,         cgi_qos_portEdit_set},
    {CGI_CMD_QOS_QUEUE,            cgi_qos_queue_set},
    {CGI_CMD_QOS_MAP_COS,          cgi_qos_mapCos_set},
    {CGI_CMD_QOS_MAP_DSCP,         cgi_qos_mapDscp_set},
    {CGI_CMD_QOS_MAP_IPP,          cgi_qos_mapIpp_set},
#ifdef CONFIG_SYS_BW_CTRL
    {CGI_CMD_QOS_RATE_CTRLEDIT,    cgi_qos_rateCtrlEdit_set},
    {CGI_CMD_QOS_RATE_EGRQUEEDIT,  cgi_qos_rateEgressQueueEdit_set},
#endif
    {NULL, NULL}
};

static int32 cgi_qos_prop_set(void)
{
    int32               ret = SYS_ERR_OK;
    char                *value;
    sys_qos_mode_t      mode = SYS_QOS_MODE_DISABLE;
    sys_qos_trustType_t type;

    if (CGI_IS_PARAM_EXIST(CGI_QOS_MODE, value))
        mode = SYS_QOS_MODE_BASIC;

    CGI_SET_ERR_HDL(sal_qos_mode_set(mode), ret);

    if (CGI_IS_PARAM_EXIST(CGI_QOS_TRUSTTYPE, value))
    {
        type = SYS_STR2UINT(value);

        CGI_SET_ERR_HDL(sal_qos_mode_check(SYS_QOS_MODE_BASIC), ret);

        CGI_SET_ERR_HDL(sal_qos_trustType_set(type), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_qos_portEdit_set(void)
{
    int32                ret = SYS_ERR_OK;
    sys_logic_port_t     lp;
    sys_logic_portmask_t lpm;
    char                 *value;
    sys_qos_cos_t        cos = 0;
    sys_enable_t         trustState = DISABLED;
    sys_enable_t         remarkCos = DISABLED;
    sys_enable_t         remarkDscp = DISABLED;
    sys_enable_t         remarkIpp = DISABLED;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    if (CGI_IS_PARAM_EXIST(CGI_QOS_COS, value))
        cos = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_QOS_STATE, value))
        trustState = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_QOS_REMARK_COS, value))
        remarkCos = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_QOS_REMARK_DSCP, value))
        remarkDscp = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_QOS_REMARK_IPP, value))
        remarkIpp = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_qos_portCos_set(lp, cos), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_qos_portTrustState_set(lp, trustState), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_qos_portRemarkCos_set(lp, remarkCos), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_qos_portRemarkDscp_set(lp, remarkDscp), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_qos_portRemarkIpp_set(lp, remarkIpp), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_qos_queue_set(void)
{
    uint32                  spqNum = 0, i;
    sys_qos_queueWeight_t   qWeight;
    int32                   ret = SYS_ERR_OK;
    char                   *val;
    char                    tmp[32];

    SYS_MEM_CLEAR(qWeight);

    for (i = 0; i < CAPA_QOS_QUEUE_NUM; i++)
    {
        sprintf(tmp, "weight_%d", QID2UQID(i));
        if (CGI_IS_PARAM_EXIST(tmp, val))
            qWeight.weights[i] = SYS_STR2UINT(val);
        else
            spqNum++;
    }
    CGI_SET_ERR_HDL(sal_qos_strictPriQueueNum_set(spqNum), ret);
    CGI_SET_ERR_HDL(sal_qos_queueWeight_set(qWeight), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_qos_mapCos_set(void)
{
    uint32 i;
    int32  ret = SYS_ERR_OK;
    char   *val;
    char   tmp[SYS_BUF16_LEN];

    for (i = 0; i < CAPA_QOS_COS_NUM; i++)
    {
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "c2q_%d", i);
        if (CGI_IS_PARAM_EXIST(tmp, val))
        {
            CGI_SET_ERR_HDL(sal_qos_cosQueue_set(i, UQID2QID(SYS_STR2UINT(val))), ret);
        }
    }

    for (i = 0; i < CAPA_QOS_QUEUE_NUM; i++)
    {
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "q2c_%d", QID2UQID(i));
        if (CGI_IS_PARAM_EXIST(tmp, val))
        {
            CGI_SET_ERR_HDL(sal_qos_queueCos_set(i, SYS_STR2UINT(val)), ret);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_qos_mapDscp_set(void)
{
    uint32 i;
    int32  ret = SYS_ERR_OK;
    char   *val;
    char   tmp[SYS_BUF16_LEN];

    for (i = 0; i < CAPA_QOS_DSCP_NUM; i++)
    {
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "d2q_%d", i);
        if (CGI_IS_PARAM_EXIST(tmp, val))
        {
            CGI_SET_ERR_HDL(sal_qos_dscpQueue_set(i, UQID2QID(SYS_STR2UINT(val))), ret);
        }
    }

    for (i = 0; i < CAPA_QOS_QUEUE_NUM; i++)
    {
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "q2d_%d", QID2UQID(i));
        if (CGI_IS_PARAM_EXIST(tmp, val))
        {
            CGI_SET_ERR_HDL(sal_qos_queueDscp_set(i, SYS_STR2UINT(val)), ret);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_qos_mapIpp_set(void)
{
    uint32 i;
    int32  ret = SYS_ERR_OK;
    char   *val;
    char   tmp[SYS_BUF16_LEN];

    for (i = 0; i < CAPA_QOS_IPP_NUM; i++)
    {
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "i2q_%d", i);
        if (CGI_IS_PARAM_EXIST(tmp, val))
        {
            CGI_SET_ERR_HDL(sal_qos_ippQueue_set(i, UQID2QID(SYS_STR2UINT(val))), ret);
        }
    }

    for (i = 0; i < CAPA_QOS_QUEUE_NUM; i++)
    {
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "q2i_%d", QID2UQID(i));
        if (CGI_IS_PARAM_EXIST(tmp, val))
        {
            CGI_SET_ERR_HDL(sal_qos_queueIpp_set(i, SYS_STR2UINT(val)), ret);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_BW_CTRL
static int32 cgi_qos_rateCtrlEdit_set(void)
{
    char                 *value;
    int32                ret = SYS_ERR_OK;
    sys_logic_port_t     lp;
    sys_logic_portmask_t lpm;
    sys_enable_t         igrState = DISABLED;
    sys_enable_t         egrState = DISABLED;
    uint32               igrRate = 0;
    uint32               egrRate = 0;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    if (CGI_IS_PARAM_EXIST(CGI_QOS_IGRSTATE, value))
        igrState = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_QOS_IGRRATE, value))
        igrRate = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_QOS_EGRSTATE, value))
        egrState = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_QOS_EGRRATE, value))
        egrRate = SYS_STR2UINT(value);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_rate_igrBandwidthCtrlEnable_set(lp, igrState), lp);

        if (ENABLED == igrState)
            CGI_SET_PORT_ERR_CONTINUE(sal_rate_igrBandwidthCtrlRate_set(lp, igrRate), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_rate_egrBandwidthCtrlEnable_set(lp, egrState), lp);

        if (ENABLED == egrState)
            CGI_SET_PORT_ERR_CONTINUE(sal_rate_egrBandwidthCtrlRate_set(lp, egrRate), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_qos_rateEgressQueueEdit_set(void)
{
    char                 *value;
    char                 tmp[SYS_BUF32_LEN];
    int32                ret = SYS_ERR_OK;
    sys_logic_port_t     lp;
    sys_logic_portmask_t lpm;
    sys_qid_t           qid;

    osal_memset(&lpm, 0, sizeof(sys_logic_portmask_t));

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        for (qid = 1; qid <= CAPA_QOS_QUEUE_NUM; qid++)
        {
            osal_sprintf(tmp, "%s_%d", CGI_QOS_STATE, qid);
            if (CGI_IS_PARAM_EXIST(tmp, value))
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_rate_egrQueueCtrlEnable_set(lp, UQID2QID(qid), ENABLED), lp);

                osal_sprintf(tmp, "%s_%d", CGI_QOS_RATE, qid);
                if (CGI_IS_PARAM_EXIST(tmp, value))
                {
                    CGI_SET_PORT_ERR_CONTINUE(sal_rate_egrQueueCtrlRate_set(lp, UQID2QID(qid), SYS_STR2UINT(value)), lp);
                }
            }
            else
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_rate_egrQueueCtrlEnable_set(lp, UQID2QID(qid), DISABLED), lp);
            }
        }
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#endif

