
#include <include/cgi.h>

static int32 cgi_authmgr_prop_get(void);
static int32 cgi_authmgr_propEdit_get(void);
static int32 cgi_authmgr_port_get(void);
static int32 cgi_authmgr_portEdit_get(void);
#ifdef CONFIG_SYS_PROTO_MACAUTH
static int32 cgi_authmgr_macAcct_get(void);
static int32 cgi_authmgr_macAcctAdd_get(void);
static int32 cgi_authmgr_macAcctEdit_get(void);
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
static int32 cgi_authmgr_webAcct_get(void);
static int32 cgi_authmgr_webAcctAdd_get(void);
static int32 cgi_authmgr_webAcctEdit_get(void);
#endif
static int32 cgi_authmgr_sess_get(void);

const cgi_cmdmap_t cmdGetAuthmgrTbl[] =
{
    {CGI_CMD_AUTHMGR_PROP,        cgi_authmgr_prop_get},
    {CGI_CMD_AUTHMGR_PROPEDIT,    cgi_authmgr_propEdit_get},
    {CGI_CMD_AUTHMGR_PORT,        cgi_authmgr_port_get},
    {CGI_CMD_AUTHMGR_PORTEDIT,    cgi_authmgr_portEdit_get},
#ifdef CONFIG_SYS_PROTO_MACAUTH
    {CGI_CMD_AUTHMGR_MACACCT,     cgi_authmgr_macAcct_get},
    {CGI_CMD_AUTHMGR_MACACCTADD,  cgi_authmgr_macAcctAdd_get},
    {CGI_CMD_AUTHMGR_MACACCTEDIT, cgi_authmgr_macAcctEdit_get},
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    {CGI_CMD_AUTHMGR_WEBACCT,     cgi_authmgr_webAcct_get},
    {CGI_CMD_AUTHMGR_WEBACCTADD,  cgi_authmgr_webAcctAdd_get},
    {CGI_CMD_AUTHMGR_WEBACCTEDIT, cgi_authmgr_webAcctEdit_get},
#endif
    {CGI_CMD_AUTHMGR_SESS,        cgi_authmgr_sess_get},
    {NULL, NULL}
};

static int32 cgi_authmgr_prop_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;
    cJSON                   *vlans = NULL;
    cJSON                   *vlan = NULL;
    cJSON                   *orders = NULL;
    cJSON                   *methods = NULL;
#ifdef CONFIG_SYS_PROTO_MACAUTH
    cJSON                   *fmts = NULL;
    cJSON                   *fmt = NULL;
    char                    radiusUser[CAPA_MAC_STR_LEN];
    uint32                  j = 0;
    uint32                  k = 0;
    authmgr_radiusUserFmt_t userFmt;
#endif
    char                    buf[SYS_BUF8_LEN];
    uint32                  i = 0;
    sys_enable_t            enable;
    sys_vid_t               guestVid = 0;
    sys_vlanmask_t          vmList;
    sys_vid_t               vid;
    sys_logic_port_t        lp;
    authmgr_cfg_port_t      portCfg;

    CGI_GET_JSON_CREATE(root, data);

#ifdef CONFIG_SYS_PROTO_DOT1X
    sal_authmgr_typeEnbl_get(AUTHMGR_TYPE_DOT1X, &enable);
    cJSON_AddBoolToObject(data, CGI_AUTHMGR_STATE_DOT1X, enable);
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
    sal_authmgr_typeEnbl_get(AUTHMGR_TYPE_MAC, &enable);
    cJSON_AddBoolToObject(data, CGI_AUTHMGR_STATE_MAC, enable);
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    sal_authmgr_typeEnbl_get(AUTHMGR_TYPE_WEB, &enable);
    cJSON_AddBoolToObject(data, CGI_AUTHMGR_STATE_WEB, enable);
#endif

    sal_authmgr_guestVlan_get(&guestVid);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_GUESTVID, guestVid);

    vlans = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_VLANS, vlans);

    sal_vlan_operEntryExist_get(&vmList);
    FOR_EACH_VID_IN_VLANMASK(vid, vmList)
    {
        vlan = cJSON_CreateObject();

        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%d", vid);
        cJSON_AddStringToObject(vlan, CGI_NAME, buf);
        cJSON_AddNumberToObject(vlan, CGI_VALUE, vid);
        cJSON_AddItemToArray(vlans, vlan);
    }

#ifdef CONFIG_SYS_PROTO_MACAUTH
    
    SYS_MEM_CLEAR(userFmt);
    SYS_MEM_CLEAR(buf);
    sal_authmgr_radiusUserFmt_get(&userFmt);
    osal_sprintf(buf, "%d_%d_%d", userFmt.delimiter, userFmt.macCase, userFmt.gap);
    cJSON_AddStringToObject(data, CGI_AUTHMGR_SEL_FORMAT, buf);

    fmts = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_AUTHMGR_FORMATS, fmts);

    for (k = 2; k <= 6; k += 2)
    {
        for (i = 0; i < AUTHMGR_DELIMITER_END; i++)
        {
            if (AUTHMGR_DELIMITER_NONE == i)
                continue;

            SYS_MEM_CLEAR(userFmt);

            for (j = 0; j < AUTHMGR_MACCASE_END; j++)
            {
                userFmt.delimiter = i;
                userFmt.macCase = j;
                userFmt.gap = k;

                SYS_MEM_CLEAR(radiusUser);
                sal_authmgr_radiusUser_get(NULL, &userFmt, radiusUser);

                fmt = cJSON_CreateObject();

                cJSON_AddStringToObject(fmt, CGI_NAME, radiusUser);

                SYS_MEM_CLEAR(buf);
                osal_sprintf(buf, "%d_%d_%d", i, j, k);

                cJSON_AddStringToObject(fmt, CGI_VALUE, buf);

                cJSON_AddItemToArray(fmts, fmt);
            }
        }
    }

    SYS_MEM_CLEAR(userFmt);
    SYS_MEM_CLEAR(radiusUser);
    SYS_MEM_CLEAR(buf);
    userFmt.delimiter = AUTHMGR_DELIMITER_NONE;
    userFmt.macCase = AUTHMGR_MACCASE_UPPER;
    userFmt.gap = DFLT_AUTHMGR_RADIUS_USER_GAP;
    osal_sprintf(buf, "%d_%d_%d", userFmt.delimiter, userFmt.macCase, userFmt.gap);
    sal_authmgr_radiusUser_get(NULL, &userFmt, radiusUser);
    fmt = cJSON_CreateObject();
    cJSON_AddStringToObject(fmt, CGI_NAME, radiusUser);
    cJSON_AddStringToObject(fmt, CGI_VALUE, buf);
    cJSON_AddItemToArray(fmts, fmt);

    SYS_MEM_CLEAR(userFmt);
    SYS_MEM_CLEAR(radiusUser);
    SYS_MEM_CLEAR(buf);
    userFmt.delimiter = AUTHMGR_DELIMITER_NONE;
    userFmt.macCase = AUTHMGR_MACCASE_LOWER;
    userFmt.gap = DFLT_AUTHMGR_RADIUS_USER_GAP;
    osal_sprintf(buf, "%d_%d_%d", userFmt.delimiter, userFmt.macCase, userFmt.gap);
    sal_authmgr_radiusUser_get(NULL, &userFmt, radiusUser);
    fmt = cJSON_CreateObject();
    cJSON_AddStringToObject(fmt, CGI_NAME, radiusUser);
    cJSON_AddStringToObject(fmt, CGI_VALUE, buf);
    cJSON_AddItemToArray(fmts, fmt);
#endif

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        SYS_MEM_CLEAR(portCfg);
        sal_authmgr_portCfg_get(lp, &portCfg);

#ifdef CONFIG_SYS_PROTO_DOT1X
        cJSON_AddBoolToObject(port, CGI_AUTHMGR_STATE_DOT1X, portCfg.typeEnbl[AUTHMGR_TYPE_DOT1X]);
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
        cJSON_AddBoolToObject(port, CGI_AUTHMGR_STATE_MAC, portCfg.typeEnbl[AUTHMGR_TYPE_MAC]);
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
        cJSON_AddBoolToObject(port, CGI_AUTHMGR_STATE_WEB, portCfg.typeEnbl[AUTHMGR_TYPE_WEB]);
#endif

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_MODE, portCfg.mode);

        orders = cJSON_CreateArray();
        cJSON_AddItemToObject(port, CGI_AUTHMGR_PORT_ORDER, orders);
        for (i = 0; i < AUTHMGR_TYPE_END - 1; i++)
        {
            if (AUTHMGR_TYPE_NONE == portCfg.typeOrder.order[i])
                break;
            cJSON_AddItemToArray(orders, cJSON_CreateNumber(portCfg.typeOrder.order[i]));
        }

        methods = cJSON_CreateArray();
        cJSON_AddItemToObject(port, CGI_AUTHMGR_PORT_METHOD, methods);
        for (i = 0; i < AUTHMGR_METHOD_END - 1; i++)
        {
            if (AUTHMGR_METHOD_NONE == portCfg.methodOrder.order[i])
                break;
            cJSON_AddItemToArray(methods, cJSON_CreateNumber(portCfg.methodOrder.order[i]));
        }

        cJSON_AddBoolToObject(port, CGI_AUTHMGR_GUEST, portCfg.guestVlanEnbl);

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_VLANASSIGN, portCfg.vlanAssignMode);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_propEdit_get(void)
{
    cJSON              *root = NULL;
    cJSON              *data = NULL;
    cJSON              *avlType = NULL;
    cJSON              *selType = NULL;
    cJSON              *avlMethod = NULL;
    cJSON              *selMethod = NULL;
    sys_logic_port_t   lp = 0;
    uint32             i = 0;
    uint32             num = 0;
    uint32             typeUsed[AUTHMGR_TYPE_END];
    uint32             methodUsed[AUTHMGR_METHOD_END];
    char               *value;
    authmgr_cfg_port_t portCfg;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    SYS_MEM_CLEAR(portCfg);
    sal_authmgr_portCfg_get(lp, &portCfg);

    CGI_GET_JSON_CREATE(root, data);

#ifdef CONFIG_SYS_PROTO_DOT1X
    cJSON_AddBoolToObject(data, CGI_AUTHMGR_STATE_DOT1X, portCfg.typeEnbl[AUTHMGR_TYPE_DOT1X]);
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    cJSON_AddBoolToObject(data, CGI_AUTHMGR_STATE_MAC, portCfg.typeEnbl[AUTHMGR_TYPE_MAC]);
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    cJSON_AddBoolToObject(data, CGI_AUTHMGR_STATE_WEB, portCfg.typeEnbl[AUTHMGR_TYPE_WEB]);
#endif

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MODE, portCfg.mode);

    SYS_MEM_CLEAR(typeUsed);
    num = 0;
    selType = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_AUTHMGR_PORT_ORDER_SEL, selType);
    for (i = 0; i < AUTHMGR_TYPE_END - 1; i++)
    {
        if (AUTHMGR_TYPE_NONE == portCfg.typeOrder.order[i])
            break;

        cJSON_AddItemToArray(selType, cJSON_CreateNumber(portCfg.typeOrder.order[i]));
        typeUsed[portCfg.typeOrder.order[i]] = TRUE;
        num++;
    }

    if (num != (AUTHMGR_TYPE_END - 1))
    {
        avlType = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_AUTHMGR_PORT_ORDER_AVL, avlType);
        for (i = 1; i < AUTHMGR_TYPE_END; i++)
        {
            if (FALSE == typeUsed[i])
            {
                cJSON_AddItemToArray(avlType, cJSON_CreateNumber(i));
            }
        }
    }

    SYS_MEM_CLEAR(methodUsed);
    num = 0;
    selMethod = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_AUTHMGR_PORT_METHOD_SEL, selMethod);
    for (i = 0; i < AUTHMGR_METHOD_END - 1; i++)
    {
        if (AUTHMGR_METHOD_NONE == portCfg.methodOrder.order[i])
            break;

        cJSON_AddItemToArray(selMethod, cJSON_CreateNumber(portCfg.methodOrder.order[i]));
        methodUsed[portCfg.methodOrder.order[i]] = TRUE;
        num++;
    }

    if (num != (AUTHMGR_METHOD_END - 1))
    {
        avlMethod = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_AUTHMGR_PORT_METHOD_AVL, avlMethod);
        for (i = 1; i < AUTHMGR_METHOD_END; i++)
        {
            if (FALSE == methodUsed[i])
            {
                cJSON_AddItemToArray(avlMethod, cJSON_CreateNumber(i));
            }
        }
    }

    cJSON_AddBoolToObject(data, CGI_AUTHMGR_GUEST, portCfg.guestVlanEnbl);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_VLANASSIGN, portCfg.vlanAssignMode);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;

}

static int32 cgi_authmgr_port_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;
    sys_logic_port_t        lp;
    authmgr_cfg_port_t      portCfg;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        SYS_MEM_CLEAR(portCfg);
        sal_authmgr_portCfg_get(lp, &portCfg);

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_CTRL, portCfg.ctrlType);

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_MAXHOST, portCfg.maxHosts);

        cJSON_AddBoolToObject(port, CGI_AUTHMGR_PORT_REAUTH, portCfg.reauthEnbl);

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_TIME_REAUTH, portCfg.reauthPeriod);

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_TIME_INACT, portCfg.inactTimeout);

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_TIME_QUIET, portCfg.quietPeriod);

#ifdef CONFIG_SYS_PROTO_DOT1X
        
        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_TX_PERIOD, portCfg.txPeriod);

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_SUPP_TIMEOUT, portCfg.suppTimeout);

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_SRV_TIMEOUT, portCfg.serverTimeout);

        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_MAXREQ, portCfg.maxReq);
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        
        cJSON_AddNumberToObject(port, CGI_AUTHMGR_PORT_MAXLOGIN, portCfg.loginAttempt);
#endif

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_portEdit_get(void)
{
    cJSON              *root = NULL;
    cJSON              *data = NULL;
    sys_logic_port_t   lp = 0;
    char               *value;
    authmgr_cfg_port_t portCfg;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    SYS_MEM_CLEAR(portCfg);
    sal_authmgr_portCfg_get(lp, &portCfg);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_CTRL, portCfg.ctrlType);

    cJSON_AddBoolToObject(data, CGI_AUTHMGR_PORT_REAUTH, portCfg.reauthEnbl);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXHOST, portCfg.maxHosts);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXHOST_MIN, BOND_AUTHMGR_MAX_HOST_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXHOST_MAX, BOND_AUTHMGR_MAX_HOST_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXHOST_DFLT, DFLT_AUTHMGR_PORT_MAX_HOSTS);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_REAUTH, portCfg.reauthPeriod);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_REAUTH_MIN, BOND_AUTHMGR_REAUTH_PERIOD_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_REAUTH_MAX, BOND_AUTHMGR_REAUTH_PERIOD_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_REAUTH_DFLT, DFLT_AUTHMGR_PORT_REAUTH_PERIOD);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_INACT, portCfg.inactTimeout);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_INACT_MIN, BOND_AUTHMGR_INACT_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_INACT_MAX, BOND_AUTHMGR_INACT_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_INACT_DFLT, DFLT_AUTHMGR_PORT_INACT_PERIOD);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_QUIET, portCfg.quietPeriod);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_QUIET_MIN, BOND_AUTHMGR_QUIET_PERIOD_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_QUIET_MAX, BOND_AUTHMGR_QUIET_PERIOD_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TIME_QUIET_DFLT, DFLT_AUTHMGR_PORT_QUIET_PERIOD);

#ifdef CONFIG_SYS_PROTO_DOT1X
    
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TX_PERIOD, portCfg.txPeriod);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TX_PERIOD_MIN, BOND_AUTHMGR_EAP_TX_PERIOD_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TX_PERIOD_MAX, BOND_AUTHMGR_EAP_TX_PERIOD_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_TX_PERIOD_DFLT, DFLT_AUTHMGR_PORT_EAP_TX_PERIOD);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_SUPP_TIMEOUT, portCfg.suppTimeout);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_SUPP_TIMEOUT_MIN, BOND_AUTHMGR_SUPP_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_SUPP_TIMEOUT_MAX, BOND_AUTHMGR_SUPP_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_SUPP_TIMEOUT_DFLT, DFLT_AUTHMGR_PORT_SUPP_TIMEOUT);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_SRV_TIMEOUT, portCfg.serverTimeout);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_SRV_TIMEOUT_MIN, BOND_AUTHMGR_SRV_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_SRV_TIMEOUT_MAX, BOND_AUTHMGR_SRV_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_SRV_TIMEOUT_DFLT, DFLT_AUTHMGR_PORT_SRV_TIMEOUT);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXREQ, portCfg.maxReq);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXREQ_MIN, BOND_AUTHMGR_MAX_REQ_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXREQ_MAX, BOND_AUTHMGR_MAX_REQ_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXREQ_DFLT, DFLT_AUTHMGR_PORT_MAX_REQ);
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXLOGIN, portCfg.loginAttempt);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXLOGIN_MIN, BOND_AUTHMGR_WEBAUTH_LOGIN_NUM_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXLOGIN_MAX, BOND_AUTHMGR_WEBAUTH_LOGIN_NUM_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_PORT_MAXLOGIN_DFLT, DFLT_AUTHMGR_PORT_WEBAUTH_LOGIN_ATTEMPT);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;

}

#ifdef CONFIG_SYS_PROTO_MACAUTH
static int32 cgi_authmgr_macAcct_get(void)
{
    cJSON              *root = NULL;
    cJSON              *data = NULL;
    cJSON              *accts = NULL;
    cJSON              *acct = NULL;
    uint32              i = 0;
    uint32              cnt = 0;
    authmgr_macauth_local_t localAcct;
    sys_mac_t           emptyMac;
    char                macStr[CAPA_MAC_STR_LEN + 1];

    CGI_GET_JSON_CREATE(root, data);

    accts = cJSON_CreateArray();

    SYS_MEM_CLEAR(emptyMac);

    for (i = 0; i < CAPA_AUTHMGR_MACAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(localAcct);

        SYS_ERR_CONTINUE(sal_authmgr_macauthLocalInfo_getByIdx(i, &localAcct));

        if (0 == osal_memcmp(&localAcct.mac, &emptyMac, sizeof(sys_mac_t)))
            continue;

        SYS_MEM_CLEAR(macStr);
        sys_util_mac2Str(localAcct.mac.octet, macStr);

        acct = cJSON_CreateObject();

        cJSON_AddStringToObject(acct, CGI_AUTHMGR_ACCT_MAC, macStr);

        cJSON_AddNumberToObject(acct, CGI_AUTHMGR_ACCT_CTRL, localAcct.state);

        if (0 != localAcct.authorInfo.reauthTime)
            cJSON_AddNumberToObject(acct, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH, localAcct.authorInfo.reauthTime);

        if (0 != localAcct.authorInfo.inactTime)
            cJSON_AddNumberToObject(acct, CGI_AUTHMGR_ACCT_AUTHOR_INACT, localAcct.authorInfo.inactTime);

        if (0 != localAcct.authorInfo.authVid)
            cJSON_AddNumberToObject(acct, CGI_AUTHMGR_ACCT_AUTHOR_VID, localAcct.authorInfo.authVid);

        cJSON_AddItemToArray(accts, acct);

        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_AUTHMGR_ACCTS, accts);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_macAcctAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_MAC_MAXLEN, CAPA_MAC_STR_LEN);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_MIN, BOND_AUTHMGR_REAUTH_PERIOD_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_MAX, BOND_AUTHMGR_REAUTH_PERIOD_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_DFLT, DFLT_AUTHMGR_PORT_REAUTH_PERIOD);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_MIN, BOND_AUTHMGR_INACT_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_MAX, BOND_AUTHMGR_INACT_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_DFLT, DFLT_AUTHMGR_PORT_INACT_PERIOD);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_macAcctEdit_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    char                    *value = NULL;
    char                    macStr[CAPA_MAC_STR_LEN + 1];
    uint32                  i = 0;
    authmgr_macauth_local_t localInfo;

    SYS_MEM_CLEAR(localInfo);
    SYS_MEM_CLEAR(macStr);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_MAC, value))
    {
        for (i = 0; i < CAPA_AUTHMGR_MACAUTH_LOCAL_NUM; i++)
        {
            SYS_MEM_CLEAR(localInfo);
            SYS_MEM_CLEAR(macStr);

            sal_authmgr_macauthLocalInfo_getByIdx(i, &localInfo);
            sys_util_mac2Str(localInfo.mac.octet, macStr);

            if (0 == osal_strcmp(macStr, value))
                break;
        }
    }

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_CTRL, localInfo.state);

    if (localInfo.authorInfo.reauthTime != 0)
        cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH, localInfo.authorInfo.reauthTime);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_MIN, BOND_AUTHMGR_REAUTH_PERIOD_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_MAX, BOND_AUTHMGR_REAUTH_PERIOD_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_DFLT, DFLT_AUTHMGR_PORT_REAUTH_PERIOD);

    if (localInfo.authorInfo.inactTime != 0)
        cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT, localInfo.authorInfo.inactTime);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_MIN, BOND_AUTHMGR_INACT_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_MAX, BOND_AUTHMGR_INACT_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_DFLT, DFLT_AUTHMGR_PORT_INACT_PERIOD);

    if (localInfo.authorInfo.authVid != 0)
        cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_VID, localInfo.authorInfo.authVid);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
static int32 cgi_authmgr_webAcct_get(void)
{
    cJSON              *root = NULL;
    cJSON              *data = NULL;
    cJSON              *accts = NULL;
    cJSON              *acct = NULL;
    uint32              i = 0;
    uint32              cnt = 0;
    authmgr_webauth_local_t localAcct;

    CGI_GET_JSON_CREATE(root, data);

    accts = cJSON_CreateArray();

    for (i = 0; i < CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(localAcct);

        SYS_ERR_CONTINUE(sal_authmgr_webauthLocalInfo_getByIdx(i, &localAcct));

        if (0 == osal_strlen(localAcct.username))
            continue;

        acct = cJSON_CreateObject();

        cJSON_AddStringToObject(acct, CGI_AUTHMGR_ACCT_USERNAME, localAcct.username);

        if (0 != localAcct.authorInfo.reauthTime)
            cJSON_AddNumberToObject(acct, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH, localAcct.authorInfo.reauthTime);

        if (0 != localAcct.authorInfo.inactTime)
            cJSON_AddNumberToObject(acct, CGI_AUTHMGR_ACCT_AUTHOR_INACT, localAcct.authorInfo.inactTime);

        if (0 != localAcct.authorInfo.authVid)
            cJSON_AddNumberToObject(acct, CGI_AUTHMGR_ACCT_AUTHOR_VID, localAcct.authorInfo.authVid);

        cJSON_AddItemToArray(accts, acct);

        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_AUTHMGR_ACCTS, accts);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_webAcctAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_USERNAME_MAXLEN, CAPA_SYS_USERNAME_LEN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_PASSWORD_MAXLEN, CAPA_SYS_PASSWORD_LEN);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_MIN, BOND_AUTHMGR_REAUTH_PERIOD_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_MAX, BOND_AUTHMGR_REAUTH_PERIOD_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_DFLT, DFLT_AUTHMGR_PORT_REAUTH_PERIOD);

    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_MIN, BOND_AUTHMGR_INACT_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_MAX, BOND_AUTHMGR_INACT_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_DFLT, DFLT_AUTHMGR_PORT_INACT_PERIOD);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_webAcctEdit_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    char                    *value = NULL;
    uint32                  i = 0;
    authmgr_webauth_local_t localInfo;

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_USERNAME, value))
    {
        for (i = 0; i < CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM; i++)
        {
            SYS_MEM_CLEAR(localInfo);

            sal_authmgr_webauthLocalInfo_getByIdx(i, &localInfo);

            if (0 == osal_strcmp(localInfo.username, value))
                break;
        }
    }

    CGI_GET_JSON_CREATE(root, data);

    if (localInfo.authorInfo.reauthTime != 0)
        cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH, localInfo.authorInfo.reauthTime);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_MIN, BOND_AUTHMGR_REAUTH_PERIOD_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_MAX, BOND_AUTHMGR_REAUTH_PERIOD_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_REAUTH_DFLT, DFLT_AUTHMGR_PORT_REAUTH_PERIOD);

    if (localInfo.authorInfo.inactTime != 0)
        cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT, localInfo.authorInfo.inactTime);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_MIN, BOND_AUTHMGR_INACT_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_MAX, BOND_AUTHMGR_INACT_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_INACT_DFLT, DFLT_AUTHMGR_PORT_INACT_PERIOD);

    if (localInfo.authorInfo.authVid != 0)
        cJSON_AddNumberToObject(data, CGI_AUTHMGR_ACCT_AUTHOR_VID, localInfo.authorInfo.authVid);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_authmgr_sess_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;
    cJSON               *sesses = NULL;
    cJSON               *sess = NULL;
    int32               ret = SYS_ERR_FAILED;
    uint32              i = 0;
    uint32              cnt = 0;
    uint32              sessNum = 0;
    char                macStr[CAPA_MAC_STR_LEN];
    char                sessIdStr[SYS_BUF32_LEN];
    authmgr_sess_info_t sessInfo;

    CGI_GET_JSON_CREATE(root, data);

    sesses = cJSON_CreateArray();

    sal_authmgr_sessCnt_get(&sessNum);

    for (i = 0; i < sessNum; i++)
    {
        SYS_MEM_CLEAR(sessInfo);

        CGI_SET_ERR_CONTINUE(sal_authmgr_sessInfo_getByIdx(i, &sessInfo), ret);

        sess = cJSON_CreateObject();

        SYS_MEM_CLEAR(sessIdStr);
        osal_sprintf(sessIdStr, "%08X%08X", sessInfo.sessId.cnt, sessInfo.sessId.time);
        cJSON_AddStringToObject(sess, CGI_AUTHMGR_SESS_ID, sessIdStr);

        cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_PORT, sessInfo.suppInfo.port);

        SYS_MEM_CLEAR(macStr);
        sys_util_mac2Str(sessInfo.suppInfo.mac.octet, macStr);
        cJSON_AddStringToObject(sess, CGI_AUTHMGR_SESS_MAC, macStr);

        cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_TYPE, sessInfo.currType);

        cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_STATUS, sessInfo.status.authStatus);

        cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_OPER_VID, sessInfo.status.currVid);

        if (AUTHMGR_STATUS_AUTHED == sessInfo.status.authStatus)
            cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_OPER_TIME_SESS, sessInfo.status.authedWhile);

        if (AUTHMGR_STATUS_AUTHED == sessInfo.status.authStatus)
            cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_OPER_TIME_INACT, sessInfo.status.inactWhile);

        if (AUTHMGR_STATUS_LOCKED == sessInfo.status.authStatus)
            cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_OPER_TIME_QUIET, sessInfo.status.quietWhile);

        if (0 != sessInfo.status.authorInfo.authVid)
            cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_AUTH_VID, sessInfo.status.authorInfo.authVid);

        if (0 != sessInfo.status.authorInfo.reauthTime)
            cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_AUTH_TIME_REAUTH, sessInfo.status.authorInfo.reauthTime);

        if (0 != sessInfo.status.authorInfo.inactTime)
            cJSON_AddNumberToObject(sess, CGI_AUTHMGR_SESS_AUTH_TIME_INACT, sessInfo.status.authorInfo.inactTime);

        cJSON_AddItemToArray(sesses, sess);
        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_AUTHMGR_SESSES, sesses);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

