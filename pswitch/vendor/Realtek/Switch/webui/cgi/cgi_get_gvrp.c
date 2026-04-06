
#include <include/cgi.h>

static int32 cgi_gvrp_get(void);
static int32 cgi_gvrp_edit_get(void);
static int32 cgi_gvrp_member_get(void);
static int32 cgi_gvrp_portCnt_get(void);

char *gvrpCntName[] =
{
    "Join empty",
    "Empty",
    "Leave Empty",
    "Join In",
    "Leave In",
    "Leave All"
};

char *gvrpErrCntName[] =
{
    "Invalid Protocol ID",
    "Invalid Attribute Type",
    "Invalid Attribute Value",
    "Invalid Attribute Length",
    "Invalid Event"
};

typedef struct gvrp_cnt_s
{
    char   *pName;
    uint32  val;
} gvrp_cnt_t;

typedef struct gvrp_stat_s
{
    gvrp_cnt_t rxCnt[6];
    gvrp_cnt_t txCnt[6];
    gvrp_cnt_t errCnt[5];
}gvrp_stat_t;

void gvrp_statArry_get(gvrp_statistics_t *pStat,  gvrp_error_statistics_t *pErrStat, gvrp_stat_t *pStatArry)
{
    int i;

    if (NULL == pStat || NULL == pStatArry ||NULL == pErrStat )
        return ;

    for (i = 0; i < 6; i++)
    {
        pStatArry->rxCnt[i].pName = gvrpCntName[i];
        pStatArry->txCnt[i].pName = gvrpCntName[i];
    }

    for (i = 0; i < 5; i++)
        pStatArry->errCnt[i].pName = gvrpErrCntName[i];

    pStatArry->rxCnt[0].val = pStat->JoinEmpty_rx;
    pStatArry->rxCnt[1].val = pStat->Empty_rx;
    pStatArry->rxCnt[2].val = pStat->LeaveEmpty_rx;
    pStatArry->rxCnt[3].val = pStat->JoinIn_rx;
    pStatArry->rxCnt[4].val = pStat->LeaveIn_rx;
    pStatArry->rxCnt[5].val = pStat->LeaveAll_rx;

    pStatArry->txCnt[0].val = pStat->JoinEmpty_tx;
    pStatArry->txCnt[1].val = pStat->Empty_tx;
    pStatArry->txCnt[2].val = pStat->LeaveEmpty_tx;
    pStatArry->txCnt[3].val = pStat->JoinIn_tx;
    pStatArry->txCnt[4].val = pStat->LeaveIn_tx;
    pStatArry->txCnt[5].val = pStat->LeaveAll_tx;

    pStatArry->errCnt[0].val = pErrStat->invProt;
    pStatArry->errCnt[1].val = pErrStat->invAtyp;
    pStatArry->errCnt[2].val = pErrStat->invAlen;
    pStatArry->errCnt[3].val = pErrStat->invAval;
    pStatArry->errCnt[4].val = pErrStat->invEvent;
}

const cgi_cmdmap_t cmdGetGvrpTbl[] =
{
    {CGI_CMD_GVRP,            cgi_gvrp_get},
    {CGI_CMD_GVRP_INTFEDIT,        cgi_gvrp_edit_get},
    {CGI_CMD_GVRP_MEMBER,       cgi_gvrp_member_get},
    {CGI_CMD_GVRP_INTFCNT,       cgi_gvrp_portCnt_get},

    {NULL, NULL}
};

static int32 cgi_gvrp_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;

    sys_enable_t  gvrpEnable;
    sys_enable_t  gvrpPortEnable;
    register_mode_t  mode;
    sys_logic_port_t lp;
    uint32 timeVal;

    sal_gvrp_enable_get(&gvrpEnable);

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddBoolToObject(data, CGI_GVRP_GLOBAL_STATUS, gvrpEnable);

	#if 0
    sal_gvrp_timer_get(GVRP_TIMER_JOIN, &timeVal);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_JOIN, timeVal);
    sal_gvrp_timer_get(GVRP_TIMER_LEAVE, &timeVal);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVE, timeVal);
     sal_gvrp_timer_get(GVRP_TIMER_LEAVEALL, &timeVal);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVEALL, timeVal);
	#endif

    SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_JOIN, &timeVal));

    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_JOIN, timeVal);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_JOIN_MIN, BOND_GVRP_JOIN_TIME_MIN);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_JOIN_MAX, BOND_GVRP_JOIN_TIME_MAX);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_JOIN_DFLT, DFLT_GVRP_JOIN_TIME);

    SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_LEAVE, &timeVal));

    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVE, timeVal);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVE_MIN, BOND_GVRP_LEAVE_TIME_MIN);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVE_MAX, BOND_GVRP_LEAVE_TIME_MAX);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVE_DFLT, DFLT_GVRP_LEAVE_TIME);

    SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_LEAVEALL, &timeVal));

    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVEALL, timeVal);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVEALL_MIN, BOND_GVRP_LEAVEALL_TIME_MIN);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVEALL_MAX, BOND_GVRP_LEAVEALL_TIME_MAX);
    cJSON_AddNumberToObject(data, CGI_GVRP_TIME_LEAVEALL_DFLT, DFLT_GVRP_LEAVEALL_TIME);

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        sal_gvrp_port_enable_get(lp, &gvrpPortEnable);
        cJSON_AddBoolToObject(port, CGI_GVRP_PORT_STATUS, gvrpPortEnable);

        sal_gvrp_port_forbidVlan_get(lp, &gvrpPortEnable);
        cJSON_AddBoolToObject(port, CGI_GVRP_PORT_CREATE_VLAN_STATUS, (ENABLED == gvrpPortEnable) ? FALSE:TRUE);

        sal_gvrp_port_regmode_get(lp, &mode);
        if (GVRP_NORMAL == mode)
            cJSON_AddStringToObject(port, CGI_GVRP_PORT_REGMODE, CGI_LANG("gvrp", "lblNormal"));
        else if (GVRP_FIXED == mode)
            cJSON_AddStringToObject(port, CGI_GVRP_PORT_REGMODE, CGI_LANG("gvrp", "lblFixed"));
        else
            cJSON_AddStringToObject(port, CGI_GVRP_PORT_REGMODE, CGI_LANG("gvrp", "lblForbidden"));

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_gvrp_edit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;

    sys_enable_t  gvrpPortEnable, forbidVlan;
    register_mode_t  mode;
    sys_logic_port_t lp = 0;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_gvrp_port_enable_get(lp, &gvrpPortEnable);
    cJSON_AddBoolToObject(data, CGI_GVRP_PORT_EDIT_STATUS, gvrpPortEnable);

    sal_gvrp_port_forbidVlan_get(lp, &forbidVlan);
    cJSON_AddBoolToObject(data, CGI_GVRP_PORT_EDIT_CREATEVLAN_STATUS, (ENABLED == forbidVlan) ? DISABLED : ENABLED);

    sal_gvrp_port_regmode_get(lp, &mode);
    if (GVRP_NORMAL == mode)
        cJSON_AddStringToObject(data, CGI_GVRP_PORT_EDIT_REGMODE, "Normal");
    else if (GVRP_FIXED == mode)
        cJSON_AddStringToObject(data, CGI_GVRP_PORT_EDIT_REGMODE, "Fixed");
    else
        cJSON_AddStringToObject(data, CGI_GVRP_PORT_EDIT_REGMODE, "Forbidden");

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_gvrp_member_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *gvrpDbVlans = NULL;
    cJSON *vlan = NULL;
    sys_vid_t vid ;
    sys_vlanmask_t operVlanMsk;
    sys_vlan_operEntry_t operEntry;
    Gvd  vlandb;
    char strPortmask[CAPA_PORTMASK_STR_LEN];
    uint32 gvrpVlanNum = 0;

    CGI_GET_JSON_CREATE(root, data);

    gvrpDbVlans = cJSON_CreateArray();
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    rsd_vlan_operEntryExist_get(&operVlanMsk);
    FOR_EACH_VID_IN_VLANMASK(vid, operVlanMsk)
    {
        SYS_MEM_CLEAR(vlandb);
        SYS_MEM_CLEAR(operEntry);

        SYS_ERR_CONTINUE(rsd_vlan_operEntry_get(vid, &operEntry));
        SYS_ERR_CONTINUE(sal_gvrp_vlan_get(vid, &vlandb));
        if (vid != vlandb.vid)
            continue;

        vlan = cJSON_CreateObject();
        cJSON_AddNumberToObject(vlan, CGI_GVRP_VLAN, vid);

        SYS_MEM_CLEAR(strPortmask);
        LPM2WEBSTR(&operEntry.lpmMember, strPortmask);
        
        cJSON_AddStringToObject(vlan, CGI_GVRP_MEMBER, strPortmask);

        SYS_MEM_CLEAR(strPortmask);
        LPM2WEBSTR(&vlandb.dynBmp, strPortmask);
        cJSON_AddStringToObject(vlan, CGI_GVRP_DYNMEMBER, strPortmask);

        cJSON_AddNumberToObject(vlan, CGI_GVRP_VLAN_TYPE, (vlandb.isdynVlan == VLAN_DYNAMIC) ? 2 : 1);

        cJSON_AddItemToArray(gvrpDbVlans, vlan);
        gvrpVlanNum++;
    }

    if (gvrpVlanNum > 0)
        cJSON_AddItemToObject(data, CGI_GVRP_DBVLAN, gvrpDbVlans);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_gvrp_portCnt_get(void)
{
    cJSON            *root = NULL;
    cJSON            *data = NULL;
    cJSON            *cnts_root = NULL;
    cJSON            *type = NULL;
    cJSON            *cnts = NULL;
    cJSON            *cnt = NULL;
    char             *value = NULL;
    uint32           i = 0;
    sys_logic_port_t lp = 0;
    gvrp_statistics_t stat;
    gvrp_error_statistics_t errStat;
    gvrp_stat_t disStat;

    CGI_GET_JSON_CREATE(root, data);

    if (CGI_IS_PARAM_EXIST(CGI_SEL_PORT, value))
        lp = SYS_STR2UINT(value);

    SYS_MEM_CLEAR(stat);
    SYS_MEM_CLEAR(errStat);
    SYS_MEM_CLEAR(disStat);
    sal_gvrp_port_statistics_get(lp, &stat);
    sal_gvrp_port_error_statistics_get(lp, &errStat);
    gvrp_statArry_get(&stat, &errStat, &disStat);

    cnts_root = cJSON_CreateArray();

    type = cJSON_CreateObject();
    cJSON_AddLangStringToObject(type, CGI_GVRP_CNT_TYPE, "gvrp", "lblRxCnt");
    cnts = cJSON_CreateArray();
    for (i = 0; i < 6; i++)
    {
        cnt = cJSON_CreateObject();
        cJSON_AddStringToObject(cnt, CGI_NAME, disStat.rxCnt[i].pName);
        cJSON_AddNumberToObject(cnt, CGI_VALUE, disStat.rxCnt[i].val);
        cJSON_AddItemToArray(cnts, cnt);
    }
    cJSON_AddItemToObject(type, CGI_GVRP_CNT, cnts);
    cJSON_AddItemToArray(cnts_root, type);

    type = cJSON_CreateObject();
    cJSON_AddLangStringToObject(type, CGI_GVRP_CNT_TYPE, "gvrp", "lblTxCnt");
    cnts = cJSON_CreateArray();
    for (i = 0; i < 6; i++)
    {
        cnt = cJSON_CreateObject();
        cJSON_AddStringToObject(cnt, CGI_NAME, disStat.txCnt[i].pName);
        cJSON_AddNumberToObject(cnt, CGI_VALUE, disStat.txCnt[i].val);
        cJSON_AddItemToArray(cnts, cnt);
    }
    cJSON_AddItemToObject(type, CGI_GVRP_CNT, cnts);
    cJSON_AddItemToArray(cnts_root, type);

    type = cJSON_CreateObject();
    cJSON_AddLangStringToObject(type, CGI_GVRP_CNT_TYPE, "gvrp", "lblErrCnt");
    cnts = cJSON_CreateArray();
    for (i = 0; i < 5; i++)
    {
        cnt = cJSON_CreateObject();
        cJSON_AddStringToObject(cnt, CGI_NAME, disStat.errCnt[i].pName);
        cJSON_AddNumberToObject(cnt, CGI_VALUE, disStat.errCnt[i].val);
        cJSON_AddItemToArray(cnts, cnt);
    }
    cJSON_AddItemToObject(type, CGI_GVRP_CNT, cnts);
    cJSON_AddItemToArray(cnts_root, type);

    cJSON_AddItemToObject(data, CGI_GVRP_DATA_CNT, cnts_root);
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

