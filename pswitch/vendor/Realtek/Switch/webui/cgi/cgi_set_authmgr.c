
#include <include/cgi.h>

static int32 cgi_authmgr_prop_set(void);
static int32 cgi_authmgr_propEdit_set(void);
static int32 cgi_authmgr_portEdit_set(void);
#ifdef CONFIG_SYS_PROTO_MACAUTH
static int32 cgi_authmgr_macAcctAddEdit_set(void);
static int32 cgi_authmgr_macAcctDel_set(void);
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
static int32 cgi_authmgr_webAcctAddEdit_set(void);
static int32 cgi_authmgr_webAcctDel_set(void);
#endif
static int32 cgi_authmgr_sessClear_set(void);

const cgi_cmdmap_t cmdSetAuthmgrTbl[] =
{
    {CGI_CMD_AUTHMGR_PROP,        cgi_authmgr_prop_set},
    {CGI_CMD_AUTHMGR_PROPEDIT,    cgi_authmgr_propEdit_set},
    {CGI_CMD_AUTHMGR_PORTEDIT,    cgi_authmgr_portEdit_set},
#ifdef CONFIG_SYS_PROTO_MACAUTH
    {CGI_CMD_AUTHMGR_MACACCTADD,  cgi_authmgr_macAcctAddEdit_set},
    {CGI_CMD_AUTHMGR_MACACCTEDIT, cgi_authmgr_macAcctAddEdit_set},
    {CGI_CMD_AUTHMGR_MACACCTDEL,  cgi_authmgr_macAcctDel_set},
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    {CGI_CMD_AUTHMGR_WEBACCTADD,  cgi_authmgr_webAcctAddEdit_set},
    {CGI_CMD_AUTHMGR_WEBACCTEDIT, cgi_authmgr_webAcctAddEdit_set},
    {CGI_CMD_AUTHMGR_WEBACCTDEL,  cgi_authmgr_webAcctDel_set},
#endif
    {CGI_CMD_AUTHMGR_SESSCLEAR,   cgi_authmgr_sessClear_set},
    {NULL, NULL}
};

static int32 cgi_authmgr_prop_set(void)
{
    int32           ret = SYS_ERR_OK;
    char         *value = NULL;
    sys_enable_t enable = DISABLED;

#ifdef CONFIG_SYS_PROTO_DOT1X
    enable = DISABLED;
    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_STATE_DOT1X, value))
        enable = ENABLED;

    CGI_SET_ERR_HDL(sal_authmgr_typeEnbl_set(AUTHMGR_TYPE_DOT1X, enable), ret);
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
    enable = DISABLED;
    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_STATE_MAC, value))
        enable = ENABLED;

    CGI_SET_ERR_HDL(sal_authmgr_typeEnbl_set(AUTHMGR_TYPE_MAC, enable), ret);
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    enable = DISABLED;
    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_STATE_WEB, value))
        enable = ENABLED;

    CGI_SET_ERR_HDL(sal_authmgr_typeEnbl_set(AUTHMGR_TYPE_WEB, enable), ret);
#endif

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_GUEST, value))
    {
        if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_GUESTVID, value))
            CGI_SET_ERR_HDL(sal_authmgr_guestVlan_set(SYS_STR2UINT(value)), ret);
    }
    else
    {
        CGI_SET_ERR_HDL(sal_authmgr_guestVlan_set(0), ret);
    }

#ifdef CONFIG_SYS_PROTO_MACAUTH
    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_SEL_FORMAT, value))
    {
        authmgr_radiusUserFmt_t userFmt;
        int32 delimiter;
        int32 gap;
        int32 macCase;
        SYS_MEM_CLEAR(userFmt);

        osal_sscanf(value, "%d_%d_%d", &delimiter, &macCase, &gap);

        userFmt.delimiter = delimiter;
        userFmt.gap = gap;
        userFmt.macCase = macCase;

        CGI_SET_ERR_HDL(sal_authmgr_radiusUserFmt_set(userFmt), ret);
    }
#endif

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_propEdit_set(void)
{
    sys_logic_port_t      lp = 0;
    sys_logic_portmask_t  lpm;
    int32                 ret = SYS_ERR_OK;
    char                  *value;
    char                  **array = NULL;
    uint32                i = 0;
    uint32                len = 0;
#ifdef CONFIG_SYS_PROTO_DOT1X
    sys_enable_t          dot1xState = DISABLED;
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    sys_enable_t          macState = DISABLED;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    sys_enable_t          webState = DISABLED;
#endif
    sys_enable_t          guestVlan = DISABLED;
    authmgr_mode_t        mode = 0;
    authmgr_order_t       order;
    authmgr_methodOrder_t methodOrder;
    authmgr_vlanAssign_t  vlanAssign = 0;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

#ifdef CONFIG_SYS_PROTO_DOT1X
    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_STATE_DOT1X, value))
        dot1xState = ENABLED;
    else
        dot1xState = DISABLED;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_STATE_MAC, value))
        macState = ENABLED;
    else
        macState = DISABLED;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_STATE_WEB, value))
        webState = ENABLED;
    else
        webState = DISABLED;
#endif

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_MODE, value))
        mode = SYS_STR2UINT(value);

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_AUTHMGR_PORT_ORDER_SEL, array, &len))
    {
        SYS_MEM_CLEAR(order);
        for (i = 0; i < len; i++)
        {
            order.order[i] = SYS_STR2UINT(array[i]);
        }
        free(array);
    }

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_AUTHMGR_PORT_METHOD_SEL, array, &len))
    {
        SYS_MEM_CLEAR(methodOrder);
        for (i = 0; i < len; i++)
        {
            methodOrder.order[i] = SYS_STR2UINT(array[i]);
        }
        free(array);
    }

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_GUEST, value))
        guestVlan = ENABLED;
    else
        guestVlan = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_VLANASSIGN, value))
        vlanAssign = SYS_STR2UINT(value);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
#ifdef CONFIG_SYS_PROTO_DOT1X
        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portTypeEnbl_set(lp, AUTHMGR_TYPE_DOT1X, dot1xState), lp);
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portTypeEnbl_set(lp, AUTHMGR_TYPE_MAC, macState), lp);
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portTypeEnbl_set(lp, AUTHMGR_TYPE_WEB, webState), lp);
#endif

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portMode_set(lp, mode), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portOrder_set(lp, order), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portMethodOrder_set(lp, methodOrder), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portGuestVlanEnbl_set(lp, guestVlan), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portVlanAssign_set(lp, vlanAssign), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_portEdit_set(void)
{
    sys_logic_port_t      lp = 0;
    sys_logic_portmask_t  lpm;
    int32                 ret = SYS_ERR_OK;
    char                  *value;
    authmgr_portCtrl_t    ctrl = 0;
    uint32                maxHosts = 0;
    sys_enable_t          reauthEnbl = DISABLED;
    uint32                timeReauth = 0;
    uint32                timeInact = 0;
    uint32                timeQuiet = 0;
#ifdef CONFIG_SYS_PROTO_DOT1X
    uint32                txPeriod = 0;
    uint32                suppTimeout = 0;
    uint32                srvTimeout = 0;
    uint32                maxReq = 0;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    uint32                maxLogin = 0;
#endif

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_CTRL, value))
        ctrl = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_MAXHOST, value))
        maxHosts = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_REAUTH, value))
        reauthEnbl = ENABLED;
    else
        reauthEnbl = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_TIME_REAUTH, value))
        timeReauth = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_TIME_INACT, value))
        timeInact = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_TIME_QUIET, value))
        timeQuiet = SYS_STR2UINT(value);

#ifdef CONFIG_SYS_PROTO_DOT1X
    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_TX_PERIOD, value))
        txPeriod = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_SUPP_TIMEOUT, value))
        suppTimeout = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_SRV_TIMEOUT, value))
        srvTimeout = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_MAXREQ, value))
        maxReq = SYS_STR2UINT(value);
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_PORT_MAXLOGIN, value))
        maxLogin = SYS_STR2UINT(value);
#endif

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portCtrl_set(lp, ctrl), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portMaxHost_set(lp, maxHosts), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portReauthEnbl_set(lp, reauthEnbl), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portReauthPeriod_set(lp, timeReauth), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portInactTimeout_set(lp, timeInact), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portQuietPeriod_set(lp, timeQuiet), lp);

#ifdef CONFIG_SYS_PROTO_DOT1X
        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portEapTxPeriod_set(lp, txPeriod), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portSuppTimeout_set(lp, suppTimeout), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portSrvTimeout_set(lp, srvTimeout), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portMaxEapReq_set(lp, maxReq), lp);
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        CGI_SET_PORT_ERR_CONTINUE(sal_authmgr_portWebauthLoginAttempt_set(lp, maxLogin), lp);
#endif
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_MACAUTH
static int32 cgi_authmgr_macAcctAddEdit_set(void)
{
    int32                   ret = SYS_ERR_OK;
    char                    *value = NULL;
    authmgr_macauth_local_t localInfo;

    SYS_MEM_CLEAR(localInfo);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_MAC, value))
        sys_util_str2Mac(value, localInfo.mac.octet);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_CTRL, value))
        localInfo.state = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_AUTHOR_REAUTH, value))
        localInfo.authorInfo.reauthTime = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_AUTHOR_INACT, value))
        localInfo.authorInfo.inactTime = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_AUTHOR_VID, value))
        localInfo.authorInfo.authVid = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_authmgr_macauthLocalInfo_set(localInfo), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_macAcctDel_set(void)
{
    int32     ret = SYS_ERR_OK;
    uint32    i = 0;
    uint32    len = 0;
    char      **array = NULL;
    sys_mac_t mac;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_AUTHMGR_ACCT_MAC, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(mac);
            sys_util_str2Mac(array[i], mac.octet);
            CGI_SET_ENTRY_ERR_CONTINUE(sal_authmgr_macauthLocalInfo_del(mac), array[i]);
        }
        free(array);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
static int32 cgi_authmgr_webAcctAddEdit_set(void)
{
    int32                   ret = SYS_ERR_OK;
    char                    *value = NULL;
    authmgr_webauth_local_t localInfo;
    char                    tmpPasswd[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];

    SYS_MEM_CLEAR(localInfo);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_USERNAME, value))
        strlcpy(localInfo.username, value, sizeof(localInfo.username));

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_PASSWORD, value))
    {
        SYS_MEM_CLEAR(tmpPasswd);
        sal_util_str_encrypt(value, tmpPasswd);
        osal_strncpy(localInfo.password, tmpPasswd, CAPA_SYS_CRYPT_PASSWORD_LEN);
    }

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_AUTHOR_REAUTH, value))
        localInfo.authorInfo.reauthTime = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_AUTHOR_INACT, value))
        localInfo.authorInfo.inactTime = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_AUTHMGR_ACCT_AUTHOR_VID, value))
        localInfo.authorInfo.authVid = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_authmgr_webauthLocalInfo_set(localInfo), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_authmgr_webAcctDel_set(void)
{
    int32     ret = SYS_ERR_OK;
    uint32    i = 0;
    uint32    len = 0;
    char      **array = NULL;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_AUTHMGR_ACCT_USERNAME, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            CGI_SET_ENTRY_ERR_CONTINUE(sal_authmgr_webauthLocalInfo_del(array[i]), array[i]);
        }
        free(array);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_authmgr_sessClear_set(void)
{
    int32                ret = SYS_ERR_OK;
    uint32               i = 0;
    uint32               len = 0;
    char                 **array = NULL;
    authmgr_sess_clear_t sessClear;
    authmgr_sessId_t     sessId;

    SYS_MEM_CLEAR(sessClear);

    sessClear.clearType = AUTHMGR_SESS_CLEAR_ID;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_AUTHMGR_SESS_SEL, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(sessId);
            osal_sscanf(array[i], "%08x%08x", &sessId.cnt, &sessId.time);
            osal_memcpy(&sessClear.data.sessId, &sessId, sizeof(authmgr_sessId_t));

            CGI_SET_ERR_HDL(sal_authmgr_sess_clear(&sessClear), ret);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

