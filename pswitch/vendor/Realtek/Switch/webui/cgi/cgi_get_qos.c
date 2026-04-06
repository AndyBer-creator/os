
#include <include/cgi.h>

#include <libsal/sal_qos.h>

static int32 cgi_qos_prop_get(void);
static int32 cgi_qos_portEdit_get(void);
static int32 cgi_qos_queue_get(void);
static int32 cgi_qos_mapCos_get(void);
static int32 cgi_qos_mapDscp_get(void);
static int32 cgi_qos_mapIpp_get(void);
#ifdef CONFIG_SYS_BW_CTRL
static int32 cgi_qos_rateCtrl_get(void);
static int32 cgi_qos_rateCtrlEdit_get(void);
static int32 cgi_qos_rateEgressQueue_get(void);
static int32 cgi_qos_rateEgressQueueEdit_get(void);
#endif

#define CGI_QOS_RATE_IGR_MIN_RATE           BOND_RATE_BANDWIDTH_MIN
#define CGI_QOS_RATE_IGR_MAX_RATE           BOND_RATE_BANDWIDTH_MAX
#define CGI_QOS_RATE_EGR_MIN_RATE           BOND_RATE_BANDWIDTH_MIN
#define CGI_QOS_RATE_EGR_MAX_RATE           BOND_RATE_BANDWIDTH_MAX

const cgi_cmdmap_t cmdGetQosTbl[] =
{
    {CGI_CMD_QOS_PROP,             cgi_qos_prop_get},
    {CGI_CMD_QOS_PORTEDIT,         cgi_qos_portEdit_get},
    {CGI_CMD_QOS_QUEUE,            cgi_qos_queue_get},
    {CGI_CMD_QOS_MAP_COS,          cgi_qos_mapCos_get},
    {CGI_CMD_QOS_MAP_DSCP,         cgi_qos_mapDscp_get},
    {CGI_CMD_QOS_MAP_IPP,          cgi_qos_mapIpp_get},
#ifdef CONFIG_SYS_BW_CTRL
    {CGI_CMD_QOS_RATE_CTRL,        cgi_qos_rateCtrl_get},
    {CGI_CMD_QOS_RATE_CTRLEDIT,    cgi_qos_rateCtrlEdit_get},
    {CGI_CMD_QOS_RATE_EGRQUE,      cgi_qos_rateEgressQueue_get},
    {CGI_CMD_QOS_RATE_EGRQUEEDIT,  cgi_qos_rateEgressQueueEdit_get},
#endif
    {NULL, NULL}
};

static int32 cgi_qos_prop_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;
    cJSON               *ports = NULL;
    cJSON               *port = NULL;
    sys_qos_mode_t      mode;
    sys_qos_trustType_t trustType;
    sys_logic_port_t    lp;
    sys_qos_cos_t       cos;
    sys_enable_t        trustState;
    sys_enable_t        remarkCos;
    sys_enable_t        remarkDscp;
    sys_enable_t        remarkIpp;

    CGI_GET_JSON_CREATE(root, data);

    sal_qos_mode_get(&mode);
    cJSON_AddNumberToObject(data, CGI_QOS_MODE, mode);

    sal_qos_trustType_get(&trustType);
    cJSON_AddNumberToObject(data, CGI_QOS_TRUSTTYPE, trustType);

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_qos_portCos_get(lp, &cos);
        sal_qos_portTrustState_get(lp, &trustState);
        sal_qos_portRemarkCos_get(lp, &remarkCos);
        sal_qos_portRemarkDscp_get(lp, &remarkDscp);
        sal_qos_portRemarkIpp_get(lp, &remarkIpp);

        cJSON_AddNumberToObject(port, CGI_QOS_COS, cos);
        cJSON_AddBoolToObject(port, CGI_QOS_STATE, trustState);
        cJSON_AddBoolToObject(port, CGI_QOS_REMARK_COS, remarkCos);
        cJSON_AddBoolToObject(port, CGI_QOS_REMARK_DSCP, remarkDscp);
        cJSON_AddBoolToObject(port, CGI_QOS_REMARK_IPP, remarkIpp);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_qos_portEdit_get(void)
{
    cJSON            *root = NULL;
    cJSON            *data = NULL;
    sys_logic_port_t lp = 0;
    char             *value;
    sys_qos_cos_t    cos;
    sys_enable_t     trustState;
    sys_enable_t     remarkCos;
    sys_enable_t     remarkDscp;
    sys_enable_t     remarkIpp;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_qos_portCos_get(lp, &cos);
    sal_qos_portTrustState_get(lp, &trustState);
    sal_qos_portRemarkCos_get(lp, &remarkCos);
    sal_qos_portRemarkDscp_get(lp, &remarkDscp);
    sal_qos_portRemarkIpp_get(lp, &remarkIpp);

    cJSON_AddNumberToObject(data, CGI_QOS_COS, cos);
    cJSON_AddBoolToObject(data, CGI_QOS_STATE, trustState);
    cJSON_AddBoolToObject(data, CGI_QOS_REMARK_COS, remarkCos);
    cJSON_AddBoolToObject(data, CGI_QOS_REMARK_DSCP, remarkDscp);
    cJSON_AddBoolToObject(data, CGI_QOS_REMARK_IPP, remarkIpp);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_qos_queue_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *queues = NULL;
    cJSON *qid_entry = NULL;
    uint32 spqNum;
    sys_qid_t qid;
    sys_qos_queueWeight_t qWeight;

    CGI_GET_JSON_CREATE(root, data);
    queues = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_QOS_QUEUES, queues);

    SYS_MEM_CLEAR(qWeight);
    sal_qos_strictPriQueueNum_get(&spqNum);
    sal_qos_queueWeight_get(&qWeight);

    for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
    {
        qid_entry = cJSON_CreateObject();

        cJSON_AddNumberToObject(qid_entry, CGI_QOS_QID, QID2UQID(qid));

        if (qid < (CAPA_QOS_QUEUE_NUM - spqNum))
            cJSON_AddStringToObject(qid_entry, CGI_QOS_TYPE, "wrr");
        else
            cJSON_AddStringToObject(qid_entry, CGI_QOS_TYPE, "sp");

        cJSON_AddNumberToObject(qid_entry, CGI_QOS_WEIGHT, qWeight.weights[qid]);

        cJSON_AddItemToArray(queues, qid_entry);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_qos_mapCos_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *cos_array = NULL;
    cJSON *que_array = NULL;
    sys_qos_cos_t cos = 0;
    sys_qid_t qid = 0;

    CGI_GET_JSON_CREATE(root, data);
    cos_array = cJSON_CreateArray();
    que_array = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_QOS_COS2QUE, cos_array);
    cJSON_AddItemToObject(data, CGI_QOS_QUE2COS, que_array);

    for (cos = 0; cos < CAPA_QOS_COS_NUM; cos++)
    {
        sal_qos_cosQueue_get(cos, &qid);

        cJSON_AddItemToArray(cos_array, cJSON_CreateNumber(QID2UQID(qid)));
    }

    for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
    {
        sal_qos_queueCos_get(qid, &cos);

        cJSON_AddItemToArray(que_array, cJSON_CreateNumber(cos));
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_qos_mapDscp_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *dscp_array = NULL;
    cJSON *que_array = NULL;
    sys_qos_dscp_t dscp = 0;
    sys_qid_t qid = 0;

    CGI_GET_JSON_CREATE(root, data);
    dscp_array = cJSON_CreateArray();
    que_array = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_QOS_DSCP2QUE, dscp_array);
    cJSON_AddItemToObject(data, CGI_QOS_QUE2DSCP, que_array);

    for (dscp = 0; dscp < CAPA_QOS_DSCP_NUM; dscp++)
    {
        sal_qos_dscpQueue_get(dscp, &qid);

        cJSON_AddItemToArray(dscp_array, cJSON_CreateNumber(QID2UQID(qid)));
    }

    for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
    {
        sal_qos_queueDscp_get(qid, &dscp);

        cJSON_AddItemToArray(que_array, cJSON_CreateNumber(dscp));
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_qos_mapIpp_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ipp_array = NULL;
    cJSON *que_array = NULL;
    sys_qos_ipp_t ipp = 0;
    sys_qid_t qid = 0;

    CGI_GET_JSON_CREATE(root, data);
    ipp_array = cJSON_CreateArray();
    que_array = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_QOS_IPP2QUE, ipp_array);
    cJSON_AddItemToObject(data, CGI_QOS_QUE2IPP, que_array);

    for (ipp = 0; ipp < CAPA_QOS_IPP_NUM; ipp++)
    {
        sal_qos_ippQueue_get(ipp, &qid);

        cJSON_AddItemToArray(ipp_array, cJSON_CreateNumber(QID2UQID(qid)));
    }

    for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
    {
        sal_qos_queueIpp_get(qid, &ipp);

        cJSON_AddItemToArray(que_array, cJSON_CreateNumber(ipp));
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_BW_CTRL
static int32 cgi_qos_rateCtrl_get(void)
{
    cJSON            *root = NULL;
    cJSON            *data = NULL;
    cJSON            *ports = NULL;
    cJSON            *port = NULL;
    sys_enable_t     enable;
    sys_logic_port_t lp;
    uint32           rate;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_rate_igrBandwidthCtrlEnable_get(lp, &enable);
        cJSON_AddBoolToObject(port, CGI_QOS_IGRSTATE, enable);

        if (ENABLED == enable)
        {
            sal_rate_igrBandwidthCtrlRate_get(lp, &rate);
            cJSON_AddNumberToObject(port, CGI_QOS_IGRRATE, rate);
        }

        sal_rate_egrBandwidthCtrlEnable_get(lp, &enable);
        cJSON_AddBoolToObject(port, CGI_QOS_EGRSTATE, enable);

        if (ENABLED == enable)
        {
            sal_rate_egrBandwidthCtrlRate_get(lp, &rate);
            cJSON_AddNumberToObject(port, CGI_QOS_EGRRATE, rate);
        }

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_qos_rateCtrlEdit_get(void)
{
    cJSON            *root = NULL;
    cJSON            *data = NULL;
    sys_logic_port_t lp = 0;
    sys_enable_t     enable;
    char             *value;
    uint32           rate;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_rate_igrBandwidthCtrlEnable_get(lp, &enable);
    sal_rate_igrBandwidthCtrlRate_get(lp, &rate);

    cJSON_AddBoolToObject(data, CGI_QOS_IGRSTATE, enable);
    cJSON_AddNumberToObject(data, CGI_QOS_IGRRATE, rate);

    sal_rate_egrBandwidthCtrlEnable_get(lp, &enable);
    sal_rate_egrBandwidthCtrlRate_get(lp, &rate);

    cJSON_AddBoolToObject(data, CGI_QOS_EGRSTATE, enable);
    cJSON_AddNumberToObject(data, CGI_QOS_EGRRATE, rate);

    cJSON_AddNumberToObject(data, CGI_QOS_RATE_MIN, CGI_QOS_RATE_IGR_MIN_RATE);
    cJSON_AddNumberToObject(data, CGI_QOS_RATE_MAX, CGI_QOS_RATE_IGR_MAX_RATE);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_qos_rateEgressQueue_get(void)
{
    cJSON            *root = NULL;
    cJSON            *data = NULL;
    cJSON            *ports = NULL;
    cJSON            *port = NULL;
    cJSON            *queues = NULL;
    cJSON            *queue = NULL;
    sys_qid_t        qid;
    sys_enable_t     enable;
    sys_logic_port_t lp;
    uint32           rate;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();
        queues = cJSON_CreateArray();
        cJSON_AddItemToObject(port, CGI_QOS_QUEUES, queues);

        for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
        {
            queue = cJSON_CreateObject();

            sal_rate_egrQueueCtrlEnable_get(lp, qid, &enable);

            cJSON_AddBoolToObject(queue, CGI_QOS_STATE, enable);

            if (ENABLED == enable)
            {
                sal_rate_egrQueueCtrlRate_get(lp, qid, &rate);
                cJSON_AddNumberToObject(queue, CGI_QOS_RATE, rate);
            }

            cJSON_AddItemToArray(queues, queue);
        }

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_qos_rateEgressQueueEdit_get(void)
{
    cJSON            *root = NULL;
    cJSON            *data = NULL;
    cJSON            *queues = NULL;
    cJSON            *queue = NULL;
    sys_qid_t        qid;
    sys_logic_port_t lp = 0;
    sys_enable_t     enable;
    char             *value;
    uint32           rate;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    queues = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_QOS_QUEUES, queues);

    for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
    {
        queue = cJSON_CreateObject();

        sal_rate_egrQueueCtrlEnable_get(lp, qid, &enable);
        cJSON_AddBoolToObject(queue, CGI_QOS_STATE, enable);

        if (ENABLED == enable)
        {
            sal_rate_egrQueueCtrlRate_get(lp, qid, &rate);
            cJSON_AddNumberToObject(queue, CGI_QOS_RATE, rate);
        }

        cJSON_AddItemToArray(queues, queue);
    }

    cJSON_AddNumberToObject(data, CGI_QOS_RATE_MIN, CGI_QOS_RATE_EGR_MIN_RATE);
    cJSON_AddNumberToObject(data, CGI_QOS_RATE_MAX, CGI_QOS_RATE_EGR_MAX_RATE);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#endif

