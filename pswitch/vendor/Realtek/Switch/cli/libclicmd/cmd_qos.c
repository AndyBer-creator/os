
#include <libcmd/cmd.h>

int32 cmd_qos_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_qos_mode_t mode;
    sys_qos_trustType_t trust;

    SYS_ERR_CHK(sal_qos_mode_get(&mode));

    XMORE("QoS Mode: %s\n", text_qos_mode[mode].text);

    if (SYS_QOS_MODE_BASIC == mode)
    {
        SYS_ERR_CHK(sal_qos_trustType_get(&trust));
        XMORE("Basic trust: %s\n", text_qos_trust[trust].text);
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_queue_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 spqNum;
    sys_qid_t qid;
    sys_qos_queueWeight_t qWeight;

    SYS_MEM_CLEAR(qWeight);

    SYS_ERR_CHK(sal_qos_strictPriQueueNum_get(&spqNum));

    SYS_ERR_CHK(sal_qos_queueWeight_get(&qWeight));

    XMORE("qid-weights    Ef - Priority\n");
    for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
    {
        if (qid < (CAPA_QOS_QUEUE_NUM - spqNum))
            XMORE("%d - %3d        dis- N/A\n", QID2UQID(qid), qWeight.weights[qid]);
        else
            XMORE("%d - N/A        ena- %d\n", QID2UQID(qid), QID2UQID(qid));
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    sys_enable_t state;
    sys_enable_t remarkCos;
    sys_enable_t remarkDscp;
    sys_enable_t remarkIpp;
    sys_qos_cos_t cos = 0;
    sys_logic_port_t logicPort = 0;
    sys_logic_portmask_t logicPortmask;

    SYS_MEM_CLEAR(logicPortmask);

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE("   Port  |  CoS   |  Trust State  | Remark Cos | Remark DSCP | Remark IP Prec \n");
    XMORE("---------+--------+---------------+------------+-------------+----------------\n");
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        LP2STR(logicPort, portStr);

        SYS_ERR_CONTINUE(sal_qos_portTrustState_get(logicPort, &state));

        SYS_ERR_CONTINUE(sal_qos_portCos_get(logicPort, &cos));

        SYS_ERR_CONTINUE(sal_qos_portRemarkCos_get(logicPort, &remarkCos));

        SYS_ERR_CONTINUE(sal_qos_portRemarkDscp_get(logicPort, &remarkDscp));

        SYS_ERR_CONTINUE(sal_qos_portRemarkIpp_get(logicPort, &remarkIpp));

        XMORE(" %7s |", portStr);
        XMORE(" %4d   |", cos);
        XMORE(" %13s |", text_enable[state].text);
        XMORE(" %10s |", text_enable[remarkCos].text);
        XMORE(" %11s |", text_enable[remarkDscp].text);
        XMORE(" %14s |", text_enable[remarkIpp].text);

        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_map_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 showCosQueue = FALSE;
    uint32 showDscpQueue = FALSE;
    uint32 showIppQueue = FALSE;
    uint32 showQueueCos = FALSE;
    uint32 showQueueDscp = FALSE;
    uint32 showQueueIpp = FALSE;
    uint32 d1 = 0;
    sys_qos_cos_t cos = 0;
    sys_qos_ipp_t ipp = 0;
    sys_qos_dscp_t dscp = 0;
    sys_qid_t qid = 0;

    if (1 == argc)
    {
        if ('c' == argv[0][0] && 'q' == argv[0][4])
            showCosQueue = TRUE;
        else if ('d' == argv[0][0] && 'q' == argv[0][5])
            showDscpQueue = TRUE;
        else if ('p' == argv[0][0] && 'q' == argv[0][11])
            showIppQueue = TRUE;
        else if ('q' == argv[0][0] && 'c' == argv[0][6])
            showQueueCos = TRUE;
        else if ('q' == argv[0][0] && 'd' == argv[0][6])
            showQueueDscp = TRUE;
        else if ('q' == argv[0][0] && 'p' == argv[0][6])
            showQueueIpp = TRUE;
    }
    else
    {
        showCosQueue = TRUE;
        showDscpQueue = TRUE;
        showIppQueue = TRUE;
        showQueueCos = TRUE;
        showQueueDscp = TRUE;
        showQueueIpp = TRUE;
    }

    if (TRUE == showCosQueue)
    {
        XMORE("\nCoS to Queue mappings\n");
        XMORE("   COS   0  1  2  3  4  5  6  7 \n");
        XMORE("--------------------------------\n");
        XMORE(" Queue ");
        for (cos = 0; cos < CAPA_QOS_COS_NUM; cos++)
        {
            sal_qos_cosQueue_get(cos, &qid);
            XMORE(" %2d", QID2UQID(qid));
        }
        XMORE("\n");
    }

    if (TRUE == showDscpQueue)
    {
        d1 = 0;
        XMORE("\nDSCP to Queue mappings\n");
        XMORE("d1: d2  0  1  2  3  4  5  6  7  8  9  \n");
        XMORE("--------------------------------------\n");
        XMORE(" %d:   ", d1);
        for (dscp = 0; dscp < CAPA_QOS_DSCP_NUM; dscp++)
        {
            sal_qos_dscpQueue_get(dscp, &qid);

            XMORE(" %2d", QID2UQID(qid));

            if (((dscp + 1) / 10) > d1)
            {
                d1++;
                XMORE("\n %d:   ", d1);
            }
        }
        XMORE("\n");
    }

    if (TRUE == showIppQueue)
    {
        XMORE("\nIP Precedence to Queue mappings\n");
        XMORE(" IP Precedence   0  1  2  3  4  5  6  7 \n");
        XMORE("----------------------------------------\n");
        XMORE("         Queue ");
        for (ipp = 0; ipp < CAPA_QOS_IPP_NUM; ipp++)
        {
            sal_qos_ippQueue_get(ipp, &qid);

            XMORE(" %2d", QID2UQID(qid));
        }
        XMORE("\n");
    }

    if (TRUE == showQueueCos)
    {
        XMORE("\nQueue to CoS mappings\n");
        XMORE(" Queue   1  2  3  4  5  6  7  8 \n");
        XMORE("--------------------------------\n");
        XMORE("   CoS ");
        for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
        {
            sal_qos_queueCos_get(qid, &cos);
            XMORE(" %2d", cos);
        }
        XMORE("\n");
    }

    if (TRUE == showQueueDscp)
    {
        XMORE("\nQueue to DSCP mappings\n");
        XMORE(" Queue   1  2  3  4  5  6  7  8 \n");
        XMORE("--------------------------------\n");
        XMORE("  DSCP ");
        for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
        {
            sal_qos_queueDscp_get(qid, &dscp);
            XMORE(" %2d", dscp);
        }
        XMORE("\n");
    }

    if (TRUE == showQueueIpp)
    {
        XMORE("\nQueue to IP Precedence mappings\n");
        XMORE(" Queue   1  2  3  4  5  6  7  8 \n");
        XMORE("--------------------------------\n");
        XMORE("ipprec ");
        for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
        {
            sal_qos_queueIpp_get(qid, &ipp);
            XMORE(" %2d", ipp);
        }
        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_qos_mode_t mode = SYS_QOS_MODE_DISABLE;

    if (CMD_IS_CLI_RUN())
    {
        int32 ret;
        char confirm[SYS_BUF8_LEN];
        sys_qos_mode_t oldMode;

        SYS_MEM_CLEAR(confirm);

        if (CMD_IS_NO_FORM_EXIST())
            mode = SYS_QOS_MODE_DISABLE;
        else
            mode = SYS_QOS_MODE_BASIC;

        SYS_ERR_CHK(sal_qos_mode_get(&oldMode));

        if ((oldMode != mode) && (oldMode != SYS_QOS_MODE_DISABLE))
        {
            XMORE("This action will cause loss of configuration.Proceed?(y)  (Y/N)[Y]");
            ret = input_str(confirm, 1, 0);
            if (ret < 0)
                return SYS_ERR_OK;

            if (confirm[0] == 'n' || confirm[0] == 'N')
            {
                return SYS_ERR_OK;
            }
        }
    }

    if (0 == argc)
    {
        if (CMD_IS_NO_FORM_EXIST())
            mode = SYS_QOS_MODE_DISABLE;
        else
            mode = SYS_QOS_MODE_BASIC;
    }

    SYS_ERR_CHK(sal_qos_mode_set(mode));

    return SYS_ERR_OK;
}

int32 cmd_qos_strictPriQueueNum_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 spqNum = 0;

    spqNum = SYS_STR2UINT(argv[0]);

    if (spqNum == (CAPA_QOS_QUEUE_NUM - 1))
    {
        XMORE("Wrong weight queue number, at least 2 queues should be weight queue.\n");

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_qos_strictPriQueueNum_set(spqNum));

    return SYS_ERR_OK;
}

int32 cmd_qos_queueWeight_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    uint32 spqNum = 0;
    uint32 weightNum = 0;
    char weightAry[CAPA_QOS_QUEUE_NUM][SYS_BUF8_LEN];
    sys_qos_queueWeight_t weight;

    SYS_MEM_CLEAR(weightAry);
    SYS_MEM_CLEAR(weight);

    SYS_ERR_CHK(sal_qos_strictPriQueueNum_get(&spqNum));

    SYS_ERR_CHK(cmd_util_seqTokAry_get(argv[0], CAPA_QOS_QUEUE_NUM, SYS_BUF8_LEN, (char**) weightAry));

    for (i = 0; i < CAPA_QOS_QUEUE_NUM; i++)
    {
        if (0 == osal_strlen(weightAry[i]))
            break;

        if (IS_QUEUE_WEIGHT_INVALID(SYS_STR2UINT(weightAry[i])))
        {
            XMORE("Invalid Queue Weight. <%d-%d>\n", BOND_QOS_QUEUE_WEIGHT_MIN, BOND_QOS_QUEUE_WEIGHT_MAX);

            return SYS_ERR_OK;
        }

        weightNum++;

        if (weightNum > (CAPA_QOS_QUEUE_NUM - spqNum))
            continue;

        weight.weights[i] = SYS_STR2UINT(weightAry[i]);
    }

    if (weightNum < (CAPA_QOS_QUEUE_NUM - spqNum))
    {
        XMORE("Wrong weights number, must be set for all %d weight queues\n",
                  (CAPA_QOS_QUEUE_NUM - spqNum));

        return SYS_ERR_OK;
    }
    else if (weightNum > (CAPA_QOS_QUEUE_NUM - spqNum))
    {
        XMORE("Too many weights (%d instead of %d). The last %d weights are ignored.\n",
                  weightNum,
                  (CAPA_QOS_QUEUE_NUM - spqNum),
                  (weightNum - (CAPA_QOS_QUEUE_NUM - spqNum)));
    }

    SYS_ERR_CHK(sal_qos_queueWeight_set(weight));

    return SYS_ERR_OK;
}

int32 cmd_qos_cosQueue_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_qid_t qid = 0;
    char cosAry[CAPA_QOS_COS_NUM][SYS_BUF8_LEN];

    SYS_MEM_CLEAR(cosAry);

    qid = UQID2QID(SYS_STR2UINT(argv[1]));
    SYS_ERR_CHK(cmd_util_seqTokAry_get(argv[0], CAPA_QOS_COS_NUM, SYS_BUF8_LEN, (char**) cosAry));

    for (i = 0; i < CAPA_QOS_COS_NUM; i++)
    {
        if (0 == osal_strlen(cosAry[i]))
            break;

        SYS_ERR_CHK(sal_qos_cosQueue_set(SYS_STR2UINT(cosAry[i]), qid));
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_dscpQueue_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_qid_t qid = 0;
    char dscpAry[CAPA_QOS_DSCP_NUM][SYS_BUF8_LEN];

    SYS_MEM_CLEAR(dscpAry);

    qid = UQID2QID(SYS_STR2UINT(argv[1]));
    SYS_ERR_CHK(cmd_util_seqTokAry_get(argv[0], CAPA_QOS_DSCP_NUM, SYS_BUF8_LEN, (char**) dscpAry));

    for (i = 0; i < CAPA_QOS_DSCP_NUM; i++)
    {
        if (0 == osal_strlen(dscpAry[i]))
            break;

        SYS_ERR_CHK(sal_qos_dscpQueue_set(SYS_STR2UINT(dscpAry[i]), qid));
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_ippQueue_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_qid_t qid = 0;
    char ippAry[CAPA_QOS_IPP_NUM][SYS_BUF8_LEN];

    SYS_MEM_CLEAR(ippAry);

    qid = UQID2QID(SYS_STR2UINT(argv[1]));
    SYS_ERR_CHK(cmd_util_seqTokAry_get(argv[0], CAPA_QOS_IPP_NUM, SYS_BUF8_LEN, (char**) ippAry));

    for (i = 0; i < CAPA_QOS_IPP_NUM; i++)
    {
        if (0 == osal_strlen(ippAry[i]))
            break;

        SYS_ERR_CHK(sal_qos_ippQueue_set(SYS_STR2UINT(ippAry[i]), qid));
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_trust_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_qos_trustType_t trust = SYS_QOS_TRUST_NONE;

    if (CMD_IS_CLI_RUN())
    {
        SYS_ERR_CHK(QOS_MODE_CHK(SYS_QOS_MODE_BASIC));
    }

    if (CMD_IS_NO_FORM_EXIST())
    {
        trust = SYS_QOS_TRUST_NONE;
    }
    else
    {
        if ('-' == argv[0][3])
            trust = SYS_QOS_TRUST_COS_DSCP;
        else if ('c' == argv[0][0])
            trust = SYS_QOS_TRUST_COS;
        else if ('d' == argv[0][0])
            trust = SYS_QOS_TRUST_DSCP;
        else if ('p' == argv[0][0])
            trust = SYS_QOS_TRUST_IPP;
    }

    SYS_ERR_CHK(sal_qos_trustType_set(trust));

    return SYS_ERR_OK;
}

int32 cmd_qos_portCos_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_qos_cos_t cosVal = 0;
    sys_logic_port_t lp;

    cosVal = SYS_STR2UINT(argv[0]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_qos_portCos_set(lp, cosVal), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_portTrust_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t lp;

    if (CMD_IS_CLI_RUN())
    {
        SYS_ERR_CHK(QOS_MODE_CHK(SYS_QOS_MODE_BASIC));
    }

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_qos_portTrustState_set(lp, enable), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_queueCos_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_qos_cos_t cos = 0;
    char qidAry[CAPA_QOS_QUEUE_NUM][SYS_BUF8_LEN];

    SYS_MEM_CLEAR(qidAry);

    cos = SYS_STR2UINT(argv[1]);
    SYS_ERR_CHK(cmd_util_seqTokAry_get(argv[0], CAPA_QOS_QUEUE_NUM, SYS_BUF8_LEN, (char**) qidAry));

    for (i = 0; i < CAPA_QOS_QUEUE_NUM; i++)
    {
        if (0 == osal_strlen(qidAry[i]))
            break;

        SYS_ERR_CHK(sal_qos_queueCos_set(UQID2QID(SYS_STR2UINT(qidAry[i])), cos));
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_queueDscp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_qos_dscp_t dscp = 0;
    char qidAry[CAPA_QOS_QUEUE_NUM][SYS_BUF8_LEN];

    SYS_MEM_CLEAR(qidAry);

    dscp = SYS_STR2UINT(argv[1]);
    SYS_ERR_CHK(cmd_util_seqTokAry_get(argv[0], CAPA_QOS_QUEUE_NUM, SYS_BUF8_LEN, (char**) qidAry));

    for (i = 0; i < CAPA_QOS_QUEUE_NUM; i++)
    {
        if (0 == osal_strlen(qidAry[i]))
            break;

        SYS_ERR_CHK(sal_qos_queueDscp_set(UQID2QID(SYS_STR2UINT(qidAry[i])), dscp));
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_queueIpp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_qos_ipp_t ipp = 0;
    char qidAry[CAPA_QOS_QUEUE_NUM][SYS_BUF8_LEN];

    SYS_MEM_CLEAR(qidAry);

    ipp = SYS_STR2UINT(argv[1]);
    SYS_ERR_CHK(cmd_util_seqTokAry_get(argv[0], CAPA_QOS_QUEUE_NUM, SYS_BUF8_LEN, (char**) qidAry));

    for (i = 0; i < CAPA_QOS_QUEUE_NUM; i++)
    {
        if (0 == osal_strlen(qidAry[i]))
            break;

        SYS_ERR_CHK(sal_qos_queueIpp_set(UQID2QID(SYS_STR2UINT(qidAry[i])), ipp));
    }

    return SYS_ERR_OK;
}

int32 cmd_qos_portRemark_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 remarkCos = 0;
    uint32 remarkDscp = 0;
    sys_enable_t enable = 0;
    sys_logic_port_t lp;
    sys_logic_portmask_t dscpRmkLpm;
    sys_logic_portmask_t ippRmkLpm;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    if ('c' == argv[0][0])
        remarkCos = TRUE;
    else if ('d' == argv[0][0])
        remarkDscp = TRUE;
    else
        remarkDscp = FALSE;

    if (remarkCos != TRUE)
    {
        SYS_ERR_CHK(sal_qos_dscpRemarkPortMask_get(&dscpRmkLpm));
        SYS_ERR_CHK(sal_qos_ippRemarkPortMask_get(&ippRmkLpm));
        if (remarkDscp == TRUE)
        {
            LOGIC_PORTMASK_OR(dscpRmkLpm, dscpRmkLpm, g_selLpm);
        }
        else
        {
            LOGIC_PORTMASK_OR(ippRmkLpm, ippRmkLpm, g_selLpm);
        }

        if (enable == ENABLED)
        {
            SYS_ERR_CHK(sal_qos_ipRemarkAbility_check(ippRmkLpm, dscpRmkLpm));
        }
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        if (TRUE == remarkCos)
        {
            CMD_ERR_CONTINUE_PORT(sal_qos_portRemarkCos_set(lp, enable), lp);
        }
        else if (TRUE == remarkDscp)
        {
            CMD_ERR_CONTINUE_PORT(sal_qos_portRemarkDscp_set(lp, enable), lp);
        }
        else
        {
            CMD_ERR_CONTINUE_PORT(sal_qos_portRemarkIpp_set(lp, enable), lp);
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_qos_queueWeight_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char str[SYS_BUF8_LEN];
    uint32 found = FALSE;
    uint32 tmpIsDiff = FALSE;
    uint32 spqNum = 0;
    sys_qid_t qid;
    sys_qos_queueWeight_t weight;
    sys_qos_queueWeight_t dfltWeight;

    SYS_MEM_CLEAR(cmd);
    SYS_MEM_CLEAR(weight);
    SYS_MEM_CLEAR(dfltWeight);

    dfltWeight.weights[0] = DFLT_QOS_QUEUE0_WEIGHT;
    dfltWeight.weights[1] = DFLT_QOS_QUEUE1_WEIGHT;
    dfltWeight.weights[2] = DFLT_QOS_QUEUE2_WEIGHT;
    dfltWeight.weights[3] = DFLT_QOS_QUEUE3_WEIGHT;
    dfltWeight.weights[4] = DFLT_QOS_QUEUE4_WEIGHT;
    dfltWeight.weights[5] = DFLT_QOS_QUEUE5_WEIGHT;
    dfltWeight.weights[6] = DFLT_QOS_QUEUE6_WEIGHT;
    dfltWeight.weights[7] = DFLT_QOS_QUEUE7_WEIGHT;

    SYS_ERR_CHK(sal_qos_queueWeight_get(&weight));
    SYS_ERR_CHK(sal_qos_strictPriQueueNum_get(&spqNum));

    tmpIsDiff = FALSE;
    FOR_EACH_QID(qid)
    {
        if ((qid + 1) > (CAPA_QOS_QUEUE_NUM - spqNum))
            break;

        if (weight.weights[qid] != dfltWeight.weights[qid])
        {
            tmpIsDiff = TRUE;

            break;
        }
    }

    if (TRUE == tmpIsDiff)
    {
        osal_sprintf(cmd, "qos queue weight ");

        FOR_EACH_QID(qid)
        {
            if ((qid + 1) > (CAPA_QOS_QUEUE_NUM - spqNum))
                break;

            if (0 == weight.weights[qid])
                break;

            SYS_MEM_CLEAR(str);

            osal_sprintf(str, "%d ", weight.weights[qid]);

            osal_strcat(cmd, str);

            found = TRUE;
        }

        if (TRUE == found)
        {
            osal_strcat(cmd, "\n");
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_qos_cosQueue_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char str[SYS_BUF8_LEN];
    uint32 found;
    sys_qos_cos_t cos;
    sys_qid_t qid;
    sys_qid_t qidAry[CAPA_QOS_COS_NUM];

    static const  sys_qid_t dfltCosQueueMap[] =
    {
         DFLT_QOS_COS0_QUEUE,
         DFLT_QOS_COS1_QUEUE,
         DFLT_QOS_COS2_QUEUE,
         DFLT_QOS_COS3_QUEUE,
         DFLT_QOS_COS4_QUEUE,
         DFLT_QOS_COS5_QUEUE,
         DFLT_QOS_COS6_QUEUE,
         DFLT_QOS_COS7_QUEUE,
    };

    SYS_MEM_CLEAR(qidAry);

    for (cos = 0; cos < CAPA_QOS_COS_NUM; cos++)
    {
        SYS_ERR_CHK(sal_qos_cosQueue_get(cos, &qidAry[cos]));
    }

    for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
    FOR_EACH_QID(qid)
    {
        found = FALSE;
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "qos map cos-queue ");

        for (cos = 0; cos < CAPA_QOS_COS_NUM; cos++)
        {
            if ((qidAry[cos] == qid) && (dfltCosQueueMap[cos] != qidAry[cos]))
            {
                SYS_MEM_CLEAR(str);
                osal_sprintf(str, "%d ", cos);

                osal_strcat(cmd, str);

                found = TRUE;
            }
        }

        if (TRUE == found)
        {
            SYS_MEM_CLEAR(str);
            osal_sprintf(str, "to %d\n", QID2UQID(qid));

            osal_strcat(cmd, str);

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_qos_dscpQueue_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char str[SYS_BUF8_LEN];
    uint32 found;
    sys_qos_dscp_t dscp;
    sys_qid_t qid;
    sys_qid_t qidAry[CAPA_QOS_DSCP_NUM];

    static const  sys_qid_t dfltDscpQueueMap[] =
    {
         DFLT_QOS_DSCP0_QUEUE,
         DFLT_QOS_DSCP1_QUEUE,
         DFLT_QOS_DSCP2_QUEUE,
         DFLT_QOS_DSCP3_QUEUE,
         DFLT_QOS_DSCP4_QUEUE,
         DFLT_QOS_DSCP5_QUEUE,
         DFLT_QOS_DSCP6_QUEUE,
         DFLT_QOS_DSCP7_QUEUE,
         DFLT_QOS_DSCP8_QUEUE,
         DFLT_QOS_DSCP9_QUEUE,
         DFLT_QOS_DSCP10_QUEUE,
         DFLT_QOS_DSCP11_QUEUE,
         DFLT_QOS_DSCP12_QUEUE,
         DFLT_QOS_DSCP13_QUEUE,
         DFLT_QOS_DSCP14_QUEUE,
         DFLT_QOS_DSCP15_QUEUE,
         DFLT_QOS_DSCP16_QUEUE,
         DFLT_QOS_DSCP17_QUEUE,
         DFLT_QOS_DSCP18_QUEUE,
         DFLT_QOS_DSCP19_QUEUE,
         DFLT_QOS_DSCP20_QUEUE,
         DFLT_QOS_DSCP21_QUEUE,
         DFLT_QOS_DSCP22_QUEUE,
         DFLT_QOS_DSCP23_QUEUE,
         DFLT_QOS_DSCP24_QUEUE,
         DFLT_QOS_DSCP25_QUEUE,
         DFLT_QOS_DSCP26_QUEUE,
         DFLT_QOS_DSCP27_QUEUE,
         DFLT_QOS_DSCP28_QUEUE,
         DFLT_QOS_DSCP29_QUEUE,
         DFLT_QOS_DSCP30_QUEUE,
         DFLT_QOS_DSCP31_QUEUE,
         DFLT_QOS_DSCP32_QUEUE,
         DFLT_QOS_DSCP33_QUEUE,
         DFLT_QOS_DSCP34_QUEUE,
         DFLT_QOS_DSCP35_QUEUE,
         DFLT_QOS_DSCP36_QUEUE,
         DFLT_QOS_DSCP37_QUEUE,
         DFLT_QOS_DSCP38_QUEUE,
         DFLT_QOS_DSCP39_QUEUE,
         DFLT_QOS_DSCP40_QUEUE,
         DFLT_QOS_DSCP41_QUEUE,
         DFLT_QOS_DSCP42_QUEUE,
         DFLT_QOS_DSCP43_QUEUE,
         DFLT_QOS_DSCP44_QUEUE,
         DFLT_QOS_DSCP45_QUEUE,
         DFLT_QOS_DSCP46_QUEUE,
         DFLT_QOS_DSCP47_QUEUE,
         DFLT_QOS_DSCP48_QUEUE,
         DFLT_QOS_DSCP49_QUEUE,
         DFLT_QOS_DSCP50_QUEUE,
         DFLT_QOS_DSCP51_QUEUE,
         DFLT_QOS_DSCP52_QUEUE,
         DFLT_QOS_DSCP53_QUEUE,
         DFLT_QOS_DSCP54_QUEUE,
         DFLT_QOS_DSCP55_QUEUE,
         DFLT_QOS_DSCP56_QUEUE,
         DFLT_QOS_DSCP57_QUEUE,
         DFLT_QOS_DSCP58_QUEUE,
         DFLT_QOS_DSCP59_QUEUE,
         DFLT_QOS_DSCP60_QUEUE,
         DFLT_QOS_DSCP61_QUEUE,
         DFLT_QOS_DSCP62_QUEUE,
         DFLT_QOS_DSCP63_QUEUE,
    };

    SYS_MEM_CLEAR(qidAry);

    for (dscp = 0; dscp < CAPA_QOS_DSCP_NUM; dscp++)
    {
        SYS_ERR_CHK(sal_qos_dscpQueue_get(dscp, &qidAry[dscp]));
    }

    FOR_EACH_QID(qid)
    {
        found = FALSE;
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "qos map dscp-queue ");

        for (dscp = 0; dscp < CAPA_QOS_DSCP_NUM; dscp++)
        {
            if ((qidAry[dscp] == qid) && (dfltDscpQueueMap[dscp] != qidAry[dscp]))
            {
                SYS_MEM_CLEAR(str);
                osal_sprintf(str, "%d ", dscp);

                osal_strcat(cmd, str);

                found = TRUE;
            }
        }

        if (TRUE == found)
        {
            SYS_MEM_CLEAR(str);
            osal_sprintf(str, "to %d\n", QID2UQID(qid));

            osal_strcat(cmd, str);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_qos_ippQueue_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char str[SYS_BUF8_LEN];
    uint32 found;
    sys_qos_ipp_t ipp;
    sys_qid_t qid;
    sys_qid_t qidAry[CAPA_QOS_COS_NUM];

    static const  sys_qid_t dfltIppQueueMap[] =
    {
         DFLT_QOS_IPP0_QUEUE,
         DFLT_QOS_IPP1_QUEUE,
         DFLT_QOS_IPP2_QUEUE,
         DFLT_QOS_IPP3_QUEUE,
         DFLT_QOS_IPP4_QUEUE,
         DFLT_QOS_IPP5_QUEUE,
         DFLT_QOS_IPP6_QUEUE,
         DFLT_QOS_IPP7_QUEUE,
    };

    SYS_MEM_CLEAR(qidAry);

    for (ipp = 0; ipp < CAPA_QOS_IPP_NUM; ipp++)
    {
        SYS_ERR_CHK(sal_qos_ippQueue_get(ipp, &qidAry[ipp]));
    }

    FOR_EACH_QID(qid)
    {
        found = FALSE;
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "qos map precedence-queue ");

        for (ipp = 0; ipp < CAPA_QOS_IPP_NUM; ipp++)
        {
            if ((qidAry[ipp] == qid) && (dfltIppQueueMap[ipp] != qidAry[ipp]))
            {
                SYS_MEM_CLEAR(str);
                osal_sprintf(str, "%d ", ipp);

                osal_strcat(cmd, str);

                found = TRUE;
            }
        }

        if (TRUE == found)
        {
            SYS_MEM_CLEAR(str);
            osal_sprintf(str, "to %d\n", QID2UQID(qid));

            osal_strcat(cmd, str);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_qos_remarkMap_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char str[SYS_BUF8_LEN];
    uint32 i = 0;
    uint32 found = FALSE;
    sys_qid_t qid;
    sys_qos_cos_t cos;
    sys_qos_dscp_t dscp;
    sys_qos_ipp_t ipp;

    static const  sys_qos_cos_t dfltQueueCosMap[] =
    {
         DFLT_QOS_QUEUE0_COS,
         DFLT_QOS_QUEUE1_COS,
         DFLT_QOS_QUEUE2_COS,
         DFLT_QOS_QUEUE3_COS,
         DFLT_QOS_QUEUE4_COS,
         DFLT_QOS_QUEUE5_COS,
         DFLT_QOS_QUEUE6_COS,
         DFLT_QOS_QUEUE7_COS,
    };

    static const  sys_qos_dscp_t dfltQueueDscpMap[] =
    {
         DFLT_QOS_QUEUE0_DSCP,
         DFLT_QOS_QUEUE1_DSCP,
         DFLT_QOS_QUEUE2_DSCP,
         DFLT_QOS_QUEUE3_DSCP,
         DFLT_QOS_QUEUE4_DSCP,
         DFLT_QOS_QUEUE5_DSCP,
         DFLT_QOS_QUEUE6_DSCP,
         DFLT_QOS_QUEUE7_DSCP,
    };

    static const  sys_qos_ipp_t dfltQueueIppMap[] =
    {
         DFLT_QOS_QUEUE0_IPP,
         DFLT_QOS_QUEUE1_IPP,
         DFLT_QOS_QUEUE2_IPP,
         DFLT_QOS_QUEUE3_IPP,
         DFLT_QOS_QUEUE4_IPP,
         DFLT_QOS_QUEUE5_IPP,
         DFLT_QOS_QUEUE6_IPP,
         DFLT_QOS_QUEUE7_IPP,
    };

    for (i = 0; i < CAPA_QOS_COS_NUM; i++)
    {
        found = FALSE;
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "qos map queue-cos ");

        FOR_EACH_QID(qid)
        {
            SYS_ERR_CHK(sal_qos_queueCos_get(qid, &cos));

            if ((i == cos) && (cos != dfltQueueCosMap[qid]))
            {
                SYS_MEM_CLEAR(str);
                osal_sprintf(str, "%d ", QID2UQID(qid));

                osal_strcat(cmd, str);

                found = TRUE;
            }
        }

        if (TRUE == found)
        {
            SYS_MEM_CLEAR(str);
            osal_sprintf(str, "to %d\n", i);

            osal_strcat(cmd, str);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    for (i = 0; i < CAPA_QOS_DSCP_NUM; i++)
    {
        found = FALSE;
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "qos map queue-dscp ");

        FOR_EACH_QID(qid)
        {
            SYS_ERR_CHK(sal_qos_queueDscp_get(qid, &dscp));

            if ((i == dscp) && (dscp != dfltQueueDscpMap[qid]))
            {
                SYS_MEM_CLEAR(str);
                osal_sprintf(str, "%d ", QID2UQID(qid));

                osal_strcat(cmd, str);

                found = TRUE;
            }
        }

        if (TRUE == found)
        {
            SYS_MEM_CLEAR(str);
            osal_sprintf(str, "to %d\n", i);

            osal_strcat(cmd, str);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    for (i = 0; i < CAPA_QOS_IPP_NUM; i++)
    {
        found = FALSE;
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "qos map queue-precedence ");

        FOR_EACH_QID(qid)
        {
            SYS_ERR_CHK(sal_qos_queueIpp_get(qid, &ipp));

            if ((i == ipp) && (ipp != dfltQueueIppMap[qid]))
            {
                SYS_MEM_CLEAR(str);
                osal_sprintf(str, "%d ", QID2UQID(qid));

                osal_strcat(cmd, str);

                found = TRUE;
            }
        }

        if (TRUE == found)
        {
            SYS_MEM_CLEAR(str);
            osal_sprintf(str, "to %d\n", i);

            osal_strcat(cmd, str);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }
    return SYS_ERR_OK;
}

int32 cmd_qos_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 spqNum = 0;
    sys_qos_mode_t mode;
    sys_qos_trustType_t trust;

    SYS_ERR_CHK(sal_qos_mode_get(&mode));

    if (DFLT_QOS_MODE != mode)
    {
        SYS_MEM_CLEAR(cmd);

        switch (mode)
        {
            case SYS_QOS_MODE_DISABLE:
                osal_sprintf(cmd, "no qos\n");
                break;

            case SYS_QOS_MODE_BASIC:
                osal_sprintf(cmd, "qos\n");
                break;

            default:
                break;
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (SYS_QOS_MODE_BASIC == mode)
    {
        SYS_ERR_CHK(sal_qos_trustType_get(&trust));

        if (DFLT_QOS_TRUST_TYPE != trust)
        {
            SYS_MEM_CLEAR(cmd);

            if (SYS_QOS_TRUST_NONE == trust)
                osal_sprintf(cmd, "no qos trust\n");
            else
                osal_sprintf(cmd, "qos trust %s\n", text_qos_trust[trust].text);

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    SYS_ERR_CHK(sal_qos_strictPriQueueNum_get(&spqNum));

    if (DFLT_QOS_STRICT_PRI_QUEUE_NUM != spqNum)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "qos queue strict-priority-num %d\n", spqNum);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(_cmd_qos_queueWeight_save(fp));

    SYS_ERR_CHK(_cmd_qos_cosQueue_save(fp));

    SYS_ERR_CHK(_cmd_qos_dscpQueue_save(fp));

    SYS_ERR_CHK(_cmd_qos_ippQueue_save(fp));

    SYS_ERR_CHK(_cmd_qos_remarkMap_save(fp));

    return SYS_ERR_OK;
}

int32 cmd_qos_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_qos_cos_t cos;
    sys_enable_t enable;
    sys_qos_mode_t mode;

    SYS_ERR_CHK(sal_qos_portCos_get(port, &cos));

    if (DFLT_QOS_PORT_COS != cos)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "qos cos %d\n", cos);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_qos_mode_get(&mode));
    if (SYS_QOS_MODE_BASIC == mode)
    {
        SYS_ERR_CHK(sal_qos_portTrustState_get(port, &enable));

        if (DFLT_QOS_PORT_TRUST_STATE != enable)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "%sqos trust\n", (ENABLED == enable) ? "" : "no ");
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    SYS_ERR_CHK(sal_qos_portRemarkCos_get(port, &enable));

    if (DFLT_QOS_PORT_REMARK_COS != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sqos remark cos\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_qos_portRemarkDscp_get(port, &enable));

    if (DFLT_QOS_PORT_REMARK_DSCP != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sqos remark dscp\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_qos_portRemarkIpp_get(port, &enable));

    if (DFLT_QOS_PORT_REMARK_IPP != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sqos remark precedence\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

