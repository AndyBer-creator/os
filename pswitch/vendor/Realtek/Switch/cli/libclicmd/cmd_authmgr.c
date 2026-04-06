
#include <libsal/sal_authmgr.h>
#include <libcmd/cmd.h>

int32 cmd_authmgr_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32               isGlbShow = FALSE;
    uint32               i = 0;
    sys_logic_port_t     lp;
    sys_logic_portmask_t lpm;
    sys_enable_t         enable;
    char                 tmp[SYS_BUF64_LEN];
#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
    uint32               isTitleShow = FALSE;
#endif

    if (0 == argc)
    {
        isGlbShow = TRUE;
        LOGIC_PORTMASK_SET_ALL(lpm);
    }
    else
    {
        CMD_NORMALPORTLIST_GET(0, &lpm);
    }

    if (TRUE == isGlbShow)
    {
        sys_vid_t               guestVid = 0;
#ifdef CONFIG_SYS_PROTO_MACAUTH
        char                    radiusUser[CAPA_MAC_STR_LEN];
        authmgr_macauth_local_t localInfo;
        sys_mac_t               emptyMac;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
        authmgr_webauth_local_t webLocalInfo;
#endif

        for (i = 0; i < AUTHMGR_TYPE_END; i++)
        {
            if (AUTHMGR_TYPE_NONE == i)
                continue;

            SYS_ERR_CHK(sal_authmgr_typeEnbl_get(i, &enable));

            SYS_MEM_CLEAR(tmp);
            osal_sprintf(tmp, "Autentication %s state", text_authmgr_type[i].text);
            XMORE("%-30s: %s\n", tmp, text_enable[enable].text);
        }

        SYS_ERR_CHK(sal_authmgr_guestVlan_get(&guestVid));

        if (0 == guestVid)
            XMORE("%-30s: %s\n", "Guest VLAN", text_enable[DISABLED].text);
        else
            XMORE("%-30s: %s (%u)\n", "Guest VLAN", text_enable[ENABLED].text, guestVid);

#ifdef CONFIG_SYS_PROTO_MACAUTH
        
        SYS_MEM_CLEAR(radiusUser);
        SYS_ERR_CHK(sal_authmgr_radiusUser_get(NULL, NULL, radiusUser));

        XMORE("%-30s: %s\n", "Mac-auth Radius User ID Format", radiusUser);

        XMORE("\n");
        SYS_MEM_CLEAR(emptyMac);

        XMORE("%-30s: \n", "Mac-auth Local Entry");

        for (i = 0; i < CAPA_AUTHMGR_MACAUTH_LOCAL_NUM; i++)
        {
            SYS_MEM_CLEAR(localInfo);
            SYS_ERR_CONTINUE(sal_authmgr_macauthLocalInfo_getByIdx(i, &localInfo));

            if (0 == osal_memcmp(&localInfo.mac, &emptyMac, sizeof(sys_mac_t)))
                continue;

            if (FALSE == isTitleShow)
            {
                XMORE("                                          Reauth     Inactive\n");
                XMORE("MAC Address         Control        VLAN   Period     Timeout\n");
                XMORE("------------------- -------------- ------ ---------- ---------\n");

                isTitleShow = TRUE;
            }

            XMORE("%02X:%02X:%02X:%02X:%02X:%02X  ",
                  localInfo.mac.octet[0],
                  localInfo.mac.octet[1],
                  localInfo.mac.octet[2],
                  localInfo.mac.octet[3],
                  localInfo.mac.octet[4],
                  localInfo.mac.octet[5]);

            XMORE(" %-12s  ", (AUTHMGR_PORTCTRL_FORCE_AUTHORIZED == localInfo.state) ? "Authorized" : "UnAuthorized");

            if (localInfo.authorInfo.authVid != 0)
                XMORE(" %-6u", localInfo.authorInfo.authVid);
            else
                XMORE(" %-6s", "N/A");

            if (localInfo.authorInfo.reauthTime != 0)
                XMORE(" %-10u", localInfo.authorInfo.reauthTime);
            else
                XMORE(" %-10s", "N/A");

            if (localInfo.authorInfo.inactTime != 0)
                XMORE(" %-5u", localInfo.authorInfo.inactTime);
            else
                XMORE(" %-5s", "N/A");

            XMORE("\n");
        }
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        
        XMORE("\n");

        XMORE("%-30s: \n", "Web-auth Local Entry");

        isTitleShow = FALSE;
        for (i = 0; i < CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM; i++)
        {
            SYS_MEM_CLEAR(webLocalInfo);
            SYS_ERR_CONTINUE(sal_authmgr_webauthLocalInfo_getByIdx(i, &webLocalInfo));

            if (0 == osal_strlen(webLocalInfo.username))
                continue;

            if (FALSE == isTitleShow)
            {
                XMORE("                                        Reauth     Inactive\n");
                XMORE("User Name                        VLAN   Period     Timeout\n");
                XMORE("-------------------------------- ------ ---------- ---------\n");

                isTitleShow = TRUE;
            }

            XMORE("%-32s", webLocalInfo.username);

            if (webLocalInfo.authorInfo.authVid != 0)
                XMORE(" %-6u", webLocalInfo.authorInfo.authVid);
            else
                XMORE(" %-6s", "N/A");

            if (webLocalInfo.authorInfo.reauthTime != 0)
                XMORE(" %-10u", webLocalInfo.authorInfo.reauthTime);
            else
                XMORE(" %-10s", "N/A");

            if (webLocalInfo.authorInfo.inactTime != 0)
                XMORE(" %-5u", webLocalInfo.authorInfo.inactTime);
            else
                XMORE(" %-5s", "N/A");

            XMORE("\n");
        }
#endif

        XMORE("\n");
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(lpm))
    {
        authmgr_cfg_port_t portCfg;

        XMORE("%-30s\n", "Interface Configurations");

        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            SYS_MEM_CLEAR(portCfg);

            SYS_ERR_CONTINUE(sal_authmgr_portCfg_get(lp, &portCfg));

            XMORE("\n");
            XMORE("Interface %s\n", LPSTR(lp));

            XMORE("  %-23s: %s\n", "Admin Control", text_authmgr_ctrlType[portCfg.ctrlType].text);

            XMORE("  %-23s: %s\n", "Host Mode", text_authmgr_mode[portCfg.mode].text);

            for (i = 0; i < AUTHMGR_TYPE_END; i++)
            {
                if (AUTHMGR_TYPE_NONE == i)
                    continue;

                SYS_MEM_CLEAR(tmp);
                osal_sprintf(tmp, "Type %s State", text_authmgr_type[i].text);
                XMORE("  %-23s: %s\n", tmp, text_enable[portCfg.typeEnbl[i]].text);
            }

            SYS_MEM_CLEAR(tmp);
            for (i = 0; i < (AUTHMGR_TYPE_END - 1); i++)
            {
                if (AUTHMGR_TYPE_NONE == portCfg.typeOrder.order[i])
                    break;

                osal_strcat(tmp, text_authmgr_type[portCfg.typeOrder.order[i]].text);
                osal_strcat(tmp, " ");
            }
            XMORE("  %-23s: %s\n", "Type Order", tmp);

#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
            
            SYS_MEM_CLEAR(tmp);
            for (i = 0; i < (AUTHMGR_METHOD_END - 1); i++)
            {
                if (AUTHMGR_METHOD_NONE == portCfg.methodOrder.order[i])
                    break;

                osal_strcat(tmp, text_authmgr_method[portCfg.methodOrder.order[i]].text);
                osal_strcat(tmp, " ");
            }
            XMORE("  %-23s: %s\n", "MAC/WEB Method Order", tmp);
#endif

            XMORE("  %-23s: %s\n", "Guest VLAN", text_enable[portCfg.guestVlanEnbl].text);

            XMORE("  %-23s: %s\n", "Reauthentication", text_enable[portCfg.reauthEnbl].text);

            XMORE("  %-23s: %u\n", "Max Hosts", portCfg.maxHosts);

            XMORE("  %-23s: %s\n", "VLAN Assign Mode", text_authmgr_vlanAssign[portCfg.vlanAssignMode].text);

            XMORE("  %-23s\n", "Common Timers");

            XMORE("    %-21s: %u\n", "Reauthenticate Period", portCfg.reauthPeriod);

            XMORE("    %-21s: %u\n", "Inactive Timeout", portCfg.inactTimeout);

            XMORE("    %-21s: %u\n", "Quiet Period", portCfg.quietPeriod);

#ifdef CONFIG_SYS_PROTO_DOT1X
            
            XMORE("  %-23s\n", "802.1x Parameters");

            XMORE("    %-21s: %u\n", "EAP Max Request", portCfg.maxReq);

            XMORE("    %-21s: %u\n", "EAP TX Period", portCfg.txPeriod);

            XMORE("    %-21s: %u\n", "Supplicant Timeout", portCfg.suppTimeout);

            XMORE("    %-21s: %u\n", "Server Timeout", portCfg.serverTimeout);
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
            
            XMORE("  %-23s\n", "Web-auth Parameters");

            if (0 == portCfg.loginAttempt)
                XMORE("    %-21s: %s\n", "Login Attempt", "infinite");
            else
                XMORE("    %-21s: %u\n", "Login Attempt", portCfg.loginAttempt);
#endif
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_authmgr_sessDetail_print(authmgr_sess_info_t *pSessInfo)
{
    char macStr[CAPA_MAC_STR_LEN];

    XMORE("%-23s: %s\n", "Interface", LPSTR(pSessInfo->suppInfo.port));

    SYS_MEM_CLEAR(macStr);
    sys_util_mac2Str(pSessInfo->suppInfo.mac.octet, macStr);
    XMORE("%-23s: %s\n", "MAC Address", macStr);

    XMORE("%-23s: %08X%08X\n", "Session ID", pSessInfo->sessId.cnt, pSessInfo->sessId.time);

    XMORE("%-23s: %s\n", "Current Type", text_authmgr_type[pSessInfo->currType].text);

    XMORE("%-23s: %s\n", "Status", text_authmgr_status[pSessInfo->status.authStatus].text);

    XMORE("%s\n", "Authorized Information");

    if (0 != pSessInfo->status.authorInfo.authVid)
        XMORE("  %-21s: %u (from %s)\n", "VLAN", pSessInfo->status.authorInfo.authVid, text_authmgr_authorSrc[pSessInfo->status.authorSrc.vidSrc].text);
    else
        XMORE("  %-21s: N/A\n", "VLAN");

    if (0 != pSessInfo->status.authorInfo.reauthTime)
        XMORE("  %-21s: %u (from %s)\n", "Reauthenticate Period", pSessInfo->status.authorInfo.reauthTime, text_authmgr_authorSrc[pSessInfo->status.authorSrc.reauthTimeSrc].text);
    else
        XMORE("  %-21s: N/A\n", "Reauthenticate Period");

    if (0 != pSessInfo->status.authorInfo.inactTime)
        XMORE("  %-21s: %u (from %s)\n", "Inactive Timeout", pSessInfo->status.authorInfo.inactTime, text_authmgr_authorSrc[pSessInfo->status.authorSrc.inactTimeSrc].text);
    else
        XMORE("  %-21s: N/A\n", "Inactive Timeout");

    XMORE("%s\n", "Operational Information");

    XMORE("  %-21s: %u\n", "VLAN", pSessInfo->status.currVid);

    if (AUTHMGR_STATUS_AUTHED == pSessInfo->status.authStatus)
    {
        
        XMORE("  %-21s: %u\n", "Session Time", pSessInfo->status.authedWhile);

        XMORE("  %-21s: %u\n", "Inactive Time", pSessInfo->status.inactWhile);

        XMORE("  %-21s: %s\n", "Quiet Time", "N/A");
    }
    else
    {
        
        XMORE("  %-21s: %s\n", "Session Time", "N/A");

        XMORE("  %-21s: %s\n", "Inactive Time", "N/A");

        XMORE("  %-21s: %u\n", "Quiet Time", pSessInfo->status.quietWhile);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_authmgr_session_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{

    uint32 i = 0;
    uint32 sessCnt = 0;
    authmgr_sess_info_t sessInfo;

    SYS_ERR_CHK(sal_authmgr_sessCnt_get(&sessCnt));

    if (0 == sessCnt)
    {
        XMORE("No Auth Manager sessions currently exist\n");

        return SYS_ERR_OK;
    }

    if ((0 == argc) && (3 == CMD_TOKENNUM_GET(cmd->vline)))
    {
        char portStr[CAPA_PORT_STR_LEN];
        char macStr[CAPA_MAC_STR_LEN];

        XMORE("Total Session Number: %u\n\n", sessCnt);

        XMORE("Interface  MAC Address       Type    Status       Session ID\n");
        XMORE("---------- ----------------- ------- ------------ ----------------\n");

        for (i = 0; i < sessCnt; i++)
        {
            SYS_MEM_CLEAR(sessInfo);

            SYS_ERR_CONTINUE(sal_authmgr_sessInfo_getByIdx(i, &sessInfo));

            SYS_MEM_CLEAR(portStr);
            LP2STR(sessInfo.suppInfo.port, portStr);
            XMORE("%-10s ", portStr);

            SYS_MEM_CLEAR(macStr);
            sys_util_mac2Str(sessInfo.suppInfo.mac.octet, macStr);
            XMORE("%-17s ", macStr);

            XMORE("%-7s ", text_authmgr_type[sessInfo.currType].text);

            XMORE("%-12s ", text_authmgr_status[sessInfo.status.authStatus].text);

            XMORE("%08X%08X\n", sessInfo.sessId.cnt, sessInfo.sessId.time);
        }
    }
    
    else if ((0 == argc) && (4 == CMD_TOKENNUM_GET(cmd->vline)))
    {
        for (i = 0; i < sessCnt; i++)
        {
            SYS_MEM_CLEAR(sessInfo);

            SYS_ERR_CONTINUE(sal_authmgr_sessInfo_getByIdx(i, &sessInfo));

            _cmd_authmgr_sessDetail_print(&sessInfo);
        }
    }
    
    else if ('i' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        sys_logic_portmask_t lpm;

        LOGIC_PORTMASK_CLEAR_ALL(lpm);

        CMD_NORMALPORTLIST_GET(0, &lpm);

        for (i = 0; i < sessCnt; i++)
        {
            SYS_MEM_CLEAR(sessInfo);

            SYS_ERR_CONTINUE(sal_authmgr_sessInfo_getByIdx(i, &sessInfo));

            if (!IS_LOGIC_PORTMASK_PORTSET(lpm, sessInfo.suppInfo.port))
                continue;

            _cmd_authmgr_sessDetail_print(&sessInfo);
        }
    }
    
    else if ('s' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        authmgr_sessId_t sessId;
        char tmp[SYS_BUF32_LEN];

        if (osal_strlen(argv[0]) != 16)
            return SYS_ERR_OK;

        osal_strncpy(tmp, argv[0], sizeof(tmp));
        sal_util_str2lowerStr_xlate(tmp);

        if (!sal_util_validStringCheck_ret(tmp, "1234567890abcdef"))
            return SYS_ERR_OK;

        SYS_MEM_CLEAR(sessId);
        osal_sscanf(tmp, "%08x%08x", &sessId.cnt, &sessId.time);

        for (i = 0; i < sessCnt; i++)
        {
            SYS_MEM_CLEAR(sessInfo);

            SYS_ERR_CONTINUE(sal_authmgr_sessInfo_getByIdx(i, &sessInfo));

            if (0 == osal_memcmp(&sessId, &sessInfo.sessId, sizeof(authmgr_sessId_t)))
            {
                _cmd_authmgr_sessDetail_print(&sessInfo);

                break;
            }
        }
    }
    
    else if ('t' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        authmgr_type_t type = 0;

        switch (argv[0][0])
        {
#ifdef CONFIG_SYS_PROTO_DOT1X
            case 'd':
                type = AUTHMGR_TYPE_DOT1X;
                break;
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
            case 'm':
                type = AUTHMGR_TYPE_MAC;
                break;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
            case 'w':
                type = AUTHMGR_TYPE_WEB;
                break;
#endif
            default:
                break;
        }

        for (i = 0; i < sessCnt; i++)
        {
            SYS_MEM_CLEAR(sessInfo);

            SYS_ERR_CONTINUE(sal_authmgr_sessInfo_getByIdx(i, &sessInfo));

            if (sessInfo.currType != type)
                continue;

            _cmd_authmgr_sessDetail_print(&sessInfo);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_session_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 sessCnt = 0;
    authmgr_sess_clear_t sessClear;

    SYS_ERR_CHK(sal_authmgr_sessCnt_get(&sessCnt));

    if (0 == sessCnt)
    {
        XMORE("No Auth Manager sessions currently exist\n");

        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(sessClear);

    if (0 == argc)
    {
        sessClear.clearType = AUTHMGR_SESS_CLEAR_ALL;
    }
    
    else if ('i' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        sessClear.clearType = AUTHMGR_SESS_CLEAR_INTF;

        CMD_NORMALPORTLIST_GET(0, &sessClear.data.lpm);
    }
    
    else if ('m' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        sessClear.clearType = AUTHMGR_SESS_CLEAR_MAC;

        sys_util_str2Mac(argv[0], sessClear.data.mac.octet);
    }
    
    else if ('s' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        authmgr_sessId_t sessId;
        char tmp[SYS_BUF32_LEN];

        if (osal_strlen(argv[0]) != 16)
            return SYS_ERR_OK;

        osal_strncpy(tmp, argv[0], sizeof(tmp));
        sal_util_str2lowerStr_xlate(tmp);

        if (!sal_util_validStringCheck_ret(tmp, "1234567890abcdef"))
            return SYS_ERR_OK;

        SYS_MEM_CLEAR(sessId);
        osal_sscanf(tmp, "%08x%08x", &sessId.cnt, &sessId.time);

        sessClear.clearType = AUTHMGR_SESS_CLEAR_ID;

        osal_memcpy(&sessClear.data.sessId, &sessId, sizeof(authmgr_sessId_t));
    }
    
    else if ('t' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        sessClear.clearType = AUTHMGR_SESS_CLEAR_TYPE;

        switch (argv[0][0])
        {
#ifdef CONFIG_SYS_PROTO_DOT1X
            case 'd':
                sessClear.data.type = AUTHMGR_TYPE_DOT1X;
                break;
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
            case 'm':
                sessClear.data.type = AUTHMGR_TYPE_MAC;
                break;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
            case 'w':
                sessClear.data.type = AUTHMGR_TYPE_WEB;
                break;
#endif
            default:
                break;
        }
    }

    SYS_ERR_CHK(sal_authmgr_sess_clear(&sessClear));

    return SYS_ERR_OK;
}

int32 cmd_authmgr_typeEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 idx = 0;
    authmgr_type_t type = AUTHMGR_TYPE_NONE;
    sys_enable_t enable;

    if (CMD_IS_NO_FORM_EXIST())
    {
        idx = 1;
        enable = DISABLED;
    }
    else
    {
        idx = 0;
        enable = ENABLED;
    }

#ifdef CONFIG_SYS_PROTO_DOT1X
    if ('d' == CMD_TOKENSTR_GET(cmd->vline, idx)[0])
    {
        type = AUTHMGR_TYPE_DOT1X;
    }
    else
#endif
    {
        switch (argv[0][0])
        {
#ifdef CONFIG_SYS_PROTO_DOT1X
            case 'd':
                type = AUTHMGR_TYPE_DOT1X;
                break;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
            case 'm':
                type = AUTHMGR_TYPE_MAC;
                break;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
            case 'w':
                type = AUTHMGR_TYPE_WEB;
                break;
#endif

            default:
                break;
        }
    }

    SYS_ERR_CHK(sal_authmgr_typeEnbl_set(type, enable));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_MACAUTH

int32 cmd_authmgr_radiusUserFmt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    authmgr_radiusUserFmt_t userFmt;

    SYS_MEM_CLEAR(userFmt);

    SYS_ERR_CHK(sal_authmgr_radiusUserFmt_get(&userFmt));

    if ('d' == CMD_TOKENSTR_GET(cmd->vline, 3)[4])
    {
        switch (argv[0][0])
        {
            case 'c':
                userFmt.delimiter = AUTHMGR_DELIMITER_COLON;
                break;

            case 'd':
                userFmt.delimiter = AUTHMGR_DELIMITER_DOT;
                break;

            case 'h':
                userFmt.delimiter = AUTHMGR_DELIMITER_HYPHEN;
                break;

            case 'n':
                userFmt.delimiter = AUTHMGR_DELIMITER_NONE;
                break;

            default:
                break;
        }

        if (2 == argc)
        {
            userFmt.gap = SYS_STR2UINT(argv[1]);
        }
    }
    else if ('c' == CMD_TOKENSTR_GET(cmd->vline, 3)[4])
    {
        switch (argv[0][0])
        {
            case 'l':
                userFmt.macCase = AUTHMGR_MACCASE_LOWER;
                break;

            case 'u':
                userFmt.macCase = AUTHMGR_MACCASE_UPPER;
                break;

            default:
                break;
        }
    }

    SYS_ERR_CHK(sal_authmgr_radiusUserFmt_set(userFmt));

    return SYS_ERR_OK;
}

int32 cmd_authmgr_macauthLocalInfo_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    authmgr_macauth_local_t localInfo;
    uint32 i = 0;
    uint32 j = 0;
    uint32 k = 0;

    SYS_MEM_CLEAR(localInfo);

    SYS_ERR_CHK(sys_util_str2Mac(argv[0], localInfo.mac.octet));

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_authmgr_macauthLocalInfo_del(localInfo.mac));

        return SYS_ERR_OK;
    }

    if ('u' == CMD_TOKENSTR_GET(cmd->vline, 5)[0])
    {
        localInfo.state = AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED;

        SYS_ERR_CHK(sal_authmgr_macauthLocalInfo_set(localInfo));

        return SYS_ERR_OK;
    }

    localInfo.state = AUTHMGR_PORTCTRL_FORCE_AUTHORIZED;

    for (i = 0; i < (argc - 1); i++)
    {
        j = 6 + (i * 2);
        k = i + 1;

        if ('r' == CMD_TOKENSTR_GET(cmd->vline, j)[0])
            localInfo.authorInfo.reauthTime = SYS_STR2UINT(argv[k]);
        else if ('i' == CMD_TOKENSTR_GET(cmd->vline, j)[0])
            localInfo.authorInfo.inactTime = SYS_STR2UINT(argv[k]);
        else if ('v' == CMD_TOKENSTR_GET(cmd->vline, j)[0])
            localInfo.authorInfo.authVid = SYS_STR2UINT(argv[k]);
    }

    SYS_ERR_CHK(sal_authmgr_macauthLocalInfo_set(localInfo));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH

int32 cmd_authmgr_webauthLocalInfo_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    authmgr_webauth_local_t localInfo;
    uint32 i = 0;
    uint32 j = 0;
    uint32 k = 0;
    char tmpPasswd[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];

    SYS_MEM_CLEAR(localInfo);

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_authmgr_webauthLocalInfo_del(argv[0]));

        return SYS_ERR_OK;
    }

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], FALSE))
        return SYS_ERR_STRING;

    osal_strncpy(localInfo.username, argv[0], CAPA_SYS_USERNAME_LEN);

    if (CMD_TOKENNUM_GET(cmd->vline) == ((argc - 2) * 2) + 8)
    {
        if (osal_strlen(argv[1]) > CAPA_SYS_CRYPT_PASSWORD_LEN)
            return SYS_ERR_STR_LEN_EXCEED;

        osal_strncpy(localInfo.password, argv[1], sizeof(localInfo.password));
    }
    else
    {
        if (osal_strlen(argv[1]) > CAPA_SYS_PASSWORD_LEN)
            return SYS_ERR_STR_LEN_EXCEED;

        if (SYS_ERR_OK != STRING_VALIDATE(argv[1], TRUE))
            return SYS_ERR_STRING;

        SYS_MEM_CLEAR(tmpPasswd);
        sal_util_str_encrypt(argv[1], tmpPasswd);
        osal_strncpy(localInfo.password, tmpPasswd, sizeof(localInfo.password));
    }

    for (i = 0; i < (argc - 2); i++)
    {
        if (CMD_TOKENNUM_GET(cmd->vline) == ((argc - 2) * 2) + 8)
            j = 8 + (i * 2);
        else
            j = 7 + (i * 2);
        k = i + 2;

        if ('r' == CMD_TOKENSTR_GET(cmd->vline, j)[0])
            localInfo.authorInfo.reauthTime = SYS_STR2UINT(argv[k]);
        else if ('i' == CMD_TOKENSTR_GET(cmd->vline, j)[0])
            localInfo.authorInfo.inactTime = SYS_STR2UINT(argv[k]);
        else if ('v' == CMD_TOKENSTR_GET(cmd->vline, j)[0])
            localInfo.authorInfo.authVid = SYS_STR2UINT(argv[k]);
    }

    SYS_ERR_CHK(sal_authmgr_webauthLocalInfo_set(localInfo));

    return SYS_ERR_OK;
}
#endif

int32 cmd_authmgr_guestVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t vlanId;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_authmgr_guestVlan_set(0));

        return SYS_ERR_OK;
    }

    vlanId = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_authmgr_guestVlan_set(vlanId));

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portGuestVlanEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_enable_t  enable;

    if (CMD_IS_NO_FORM_EXIST())
    {
        enable = DISABLED;
    }
    else
    {
        enable = ENABLED;
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portGuestVlanEnbl_set(lp, enable));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portTypeEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 idx = 0;
    authmgr_type_t type = AUTHMGR_TYPE_NONE;
    sys_enable_t enable;

    if (CMD_IS_NO_FORM_EXIST())
    {
        idx = 1;
        enable = DISABLED;
    }
    else
    {
        idx = 0;
        enable = ENABLED;
    }

#ifdef CONFIG_SYS_PROTO_DOT1X
    if ('d' == CMD_TOKENSTR_GET(cmd->vline, idx)[0])
    {
        type = AUTHMGR_TYPE_DOT1X;
    }
    else
#endif
    {
        switch (argv[0][0])
        {
#ifdef CONFIG_SYS_PROTO_DOT1X
            case 'd':
                type = AUTHMGR_TYPE_DOT1X;
                break;
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
            case 'm':
                type = AUTHMGR_TYPE_MAC;
                break;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
            case 'w':
                type = AUTHMGR_TYPE_WEB;
                break;
#endif
            default:
                break;
        }
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portTypeEnbl_set(lp, type, enable));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portCtrl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    authmgr_portCtrl_t ctrlType = AUTHMGR_PORTCTRL_FORCE_AUTHORIZED;

    if (CMD_IS_NO_FORM_EXIST())
    {
        ctrlType = DFLT_AUTHMGR_PORT_CTRL;
    }
    else
    {
        if ('a' == argv[0][0])
            ctrlType = AUTHMGR_PORTCTRL_AUTO;
        else if ('a' == argv[0][6])
            ctrlType = AUTHMGR_PORTCTRL_FORCE_AUTHORIZED;
        else if ('u' == argv[0][6])
            ctrlType = AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED;
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portCtrl_set(lp, ctrlType));
    }

#ifdef CONFIG_SYS_PROTO_DOT1X
    
    if ('d' == CMD_TOKENSTR_GET(cmd->vline, 0)[0] && 2 == CMD_TOKENNUM_GET(cmd->vline))
    {
        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            SYS_ERR_CHK(sal_authmgr_portTypeEnbl_set(lp, AUTHMGR_TYPE_DOT1X, ENABLED));
        }
    }
#endif

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    authmgr_mode_t mode = AUTHMGR_MODE_MULTI_AUTH;

    if (CMD_IS_NO_FORM_EXIST())
    {
        mode = DFLT_AUTHMGR_PORT_MODE;
    }
    else
    {
        if ('m' == argv[0][0])
        {
            if ('a' == argv[0][6])
                mode = AUTHMGR_MODE_MULTI_AUTH;
            else if ('h' == argv[0][6])
                mode = AUTHMGR_MODE_MULTI_HOST;
        }
        else if ('s' == argv[0][0])
        {
            mode = AUTHMGR_MODE_SINGLE_HOST;
        }
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portMode_set(lp, mode));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portOrder_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    authmgr_order_t order;
    uint32 i = 0;

    SYS_MEM_CLEAR(order);

    if (CMD_IS_NO_FORM_EXIST())
    {
        order.order[0] = DFLT_AUTHMGR_PORT_ORDER_FIRST;
    }
    else
    {
        for (i = 2; i < CMD_TOKENNUM_GET(cmd->vline); i++)
        {
            switch (CMD_TOKENSTR_GET(cmd->vline, i)[0])
            {
#ifdef CONFIG_SYS_PROTO_DOT1X
                case 'd':
                    order.order[i - 2] = AUTHMGR_TYPE_DOT1X;
                    break;
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
                case 'm':
                    order.order[i - 2] = AUTHMGR_TYPE_MAC;
                    break;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
                case 'w':
                    order.order[i - 2] = AUTHMGR_TYPE_WEB;
                    break;
#endif
                default:
                    break;
            }
        }
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portOrder_set(lp, order));
    }

    return SYS_ERR_OK;
}

#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)

int32 cmd_authmgr_portMethodOrder_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    authmgr_methodOrder_t order;
    uint32 i = 0;

    SYS_MEM_CLEAR(order);

    if (CMD_IS_NO_FORM_EXIST())
    {
        order.order[0] = DFLT_AUTHMGR_PORT_METHOD_ORDER_FIRST;
    }
    else
    {
        for (i = 2; i < CMD_TOKENNUM_GET(cmd->vline); i++)
        {
            if ('r' == CMD_TOKENSTR_GET(cmd->vline, i)[0])
                order.order[i - 2] = AUTHMGR_METHOD_RADIUS;
            else if ('l' == CMD_TOKENSTR_GET(cmd->vline, i)[0])
                order.order[i - 2] = AUTHMGR_METHOD_LOCAL;
        }
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portMethodOrder_set(lp, order));
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_authmgr_portReauthEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_enable_t enable = ENABLED;

    if (CMD_IS_NO_FORM_EXIST())
    {
        enable = DISABLED;
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portReauthEnbl_set(lp, enable));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portMaxHost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 hostNum = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        hostNum = DFLT_AUTHMGR_PORT_MAX_HOSTS;
    }
    else
    {
        hostNum = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portMaxHost_set(lp, hostNum));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portVlanAssignMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    authmgr_vlanAssign_t mode = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        mode = AUTHMGR_VLAN_ASSIGN_DISABLE;
    }
    else
    {
        if ('r' == argv[0][0])
            mode = AUTHMGR_VLAN_ASSIGN_REJECT;
        else
            mode = AUTHMGR_VLAN_ASSIGN_STATIC;
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portVlanAssign_set(lp, mode));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portReauthPeriod_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 time = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        time = DFLT_AUTHMGR_PORT_REAUTH_PERIOD;
    }
    else
    {
        time = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portReauthPeriod_set(lp, time));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portInactTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 time = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        time = DFLT_AUTHMGR_PORT_INACT_PERIOD;
    }
    else
    {
        time = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portInactTimeout_set(lp, time));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portQuietPeriod_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 time = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        time = DFLT_AUTHMGR_PORT_QUIET_PERIOD;
    }
    else
    {
        time = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portQuietPeriod_set(lp, time));
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_DOT1X

int32 cmd_authmgr_portSuppTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 time = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        time = DFLT_AUTHMGR_PORT_SUPP_TIMEOUT;
    }
    else
    {
        time = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portSuppTimeout_set(lp, time));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portSrvTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 time = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        time = DFLT_AUTHMGR_PORT_SRV_TIMEOUT;
    }
    else
    {
        time = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portSrvTimeout_set(lp, time));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portEapTxPeriod_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 time = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        time = DFLT_AUTHMGR_PORT_EAP_TX_PERIOD;
    }
    else
    {
        time = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portEapTxPeriod_set(lp, time));
    }

    return SYS_ERR_OK;
}

int32 cmd_authmgr_portMaxEapReq_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 num = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        num = DFLT_AUTHMGR_PORT_MAX_REQ;
    }
    else
    {
        num = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portMaxEapReq_set(lp, num));
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH

int32 cmd_authmgr_portMaxLoginAttempt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    uint32 loginAttempt = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        loginAttempt = DFLT_AUTHMGR_PORT_WEBAUTH_LOGIN_ATTEMPT;
    }
    else
    {
        if ('i' == argv[0][0])
            loginAttempt = 0;
        else
            loginAttempt = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_authmgr_portWebauthLoginAttempt_set(lp, loginAttempt));
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_authmgr_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;
    sys_vid_t guestVlan = 0;
#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
    uint32 i = 0;
    char tmpStr[SYS_BUF32_LEN];
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    sys_mac_t emptyMac;
    authmgr_radiusUserFmt_t userFmt;
    authmgr_macauth_local_t localInfo;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    authmgr_webauth_local_t webLocalInfo;
#endif

#ifdef CONFIG_SYS_PROTO_DOT1X
    SYS_ERR_CHK(sal_authmgr_typeEnbl_get(AUTHMGR_TYPE_DOT1X, &enable));
    if (DFLT_AUTHMGR_DOT1X_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sauthentication dot1x\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
    SYS_ERR_CHK(sal_authmgr_typeEnbl_get(AUTHMGR_TYPE_MAC, &enable));
    if (DFLT_AUTHMGR_MAC_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sauthentication mac\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    SYS_ERR_CHK(sal_authmgr_typeEnbl_get(AUTHMGR_TYPE_WEB, &enable));
    if (DFLT_AUTHMGR_WEB_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sauthentication web\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

    SYS_ERR_CHK(sal_authmgr_guestVlan_get(&guestVlan));
    if (DFLT_AUTHMGR_GUEST_VLAN != guestVlan)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "authentication guest-vlan %d\n", guestVlan);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

#ifdef CONFIG_SYS_PROTO_MACAUTH
    
    SYS_ERR_CHK(sal_authmgr_radiusUserFmt_get(&userFmt));

    if (DFLT_AUTHMGR_RADIUS_USER_DELIMIT != userFmt.delimiter
        || DFLT_AUTHMGR_RADIUS_USER_GAP != userFmt.gap)
    {
        char delimiter[SYS_BUF16_LEN];

        SYS_MEM_CLEAR(cmd);
        SYS_MEM_CLEAR(delimiter);

        switch(userFmt.delimiter)
        {
            case AUTHMGR_DELIMITER_NONE:
                osal_strcpy(delimiter, "none");
                break;

            case AUTHMGR_DELIMITER_COLON:
                osal_strcpy(delimiter, "colon");
                break;

            case AUTHMGR_DELIMITER_HYPHEN:
                osal_strcpy(delimiter, "hyphen");
                break;

            case AUTHMGR_DELIMITER_DOT:
                osal_strcpy(delimiter, "dot");
                break;

            default:
                break;
        }

        if (DFLT_AUTHMGR_RADIUS_USER_GAP != userFmt.gap)
            osal_sprintf(cmd, "authentication mac radius mac-delimiter %s gap %d\n", delimiter, userFmt.gap);
        else
            osal_sprintf(cmd, "authentication mac radius mac-delimiter %s\n", delimiter);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (DFLT_AUTHMGR_RADIUS_USER_CASE != userFmt.macCase)
    {
        char macCase[SYS_BUF16_LEN];

        SYS_MEM_CLEAR(cmd);
        SYS_MEM_CLEAR(macCase);

        if (AUTHMGR_MACCASE_UPPER == userFmt.macCase)
            osal_strcpy(macCase, "upper");
        else if (AUTHMGR_MACCASE_LOWER == userFmt.macCase)
            osal_strcpy(macCase, "lower");

        osal_sprintf(cmd, "authentication mac radius mac-case %s\n", macCase);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_MEM_CLEAR(emptyMac);
    for (i = 0; i < CAPA_AUTHMGR_MACAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(localInfo);

        SYS_ERR_CONTINUE(sal_authmgr_macauthLocalInfo_getByIdx(i, &localInfo));

        if (0 == osal_memcmp(&localInfo.mac, &emptyMac, sizeof(sys_mac_t)))
            continue;

        SYS_MEM_CLEAR(cmd);
        
        osal_sprintf(cmd, "authentication mac local %02X:%02X:%02X:%02X:%02X:%02X control %s ",
                      localInfo.mac.octet[0],
                      localInfo.mac.octet[1],
                      localInfo.mac.octet[2],
                      localInfo.mac.octet[3],
                      localInfo.mac.octet[4],
                      localInfo.mac.octet[5],
                      (AUTHMGR_PORTCTRL_FORCE_AUTHORIZED == localInfo.state) ? "auth" : "unauth");

        if (localInfo.authorInfo.authVid != 0)
        {
            SYS_MEM_CLEAR(tmpStr);
            osal_sprintf(tmpStr, "vlan %u ", localInfo.authorInfo.authVid);
            osal_strcat(cmd, tmpStr);
        }

        if (localInfo.authorInfo.reauthTime != 0)
        {
            SYS_MEM_CLEAR(tmpStr);
            osal_sprintf(tmpStr, "reauth-period %u ", localInfo.authorInfo.reauthTime);
            osal_strcat(cmd, tmpStr);
        }

        if (localInfo.authorInfo.inactTime != 0)
        {
            SYS_MEM_CLEAR(tmpStr);
            osal_sprintf(tmpStr, "inactive-timeout %u ", localInfo.authorInfo.inactTime);
            osal_strcat(cmd, tmpStr);
        }

        osal_strcat(cmd, "\n");

        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    
    for (i = 0; i < CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(webLocalInfo);

        SYS_ERR_CONTINUE(sal_authmgr_webauthLocalInfo_getByIdx(i, &webLocalInfo));

        if (0 == osal_strlen(webLocalInfo.username))
            continue;

        SYS_MEM_CLEAR(cmd);
        
        osal_sprintf(cmd, "authentication web local username %s password encrypted %s ",
                      webLocalInfo.username,
                      webLocalInfo.password);

        if (webLocalInfo.authorInfo.authVid != 0)
        {
            SYS_MEM_CLEAR(tmpStr);
            osal_sprintf(tmpStr, "vlan %u ", webLocalInfo.authorInfo.authVid);
            osal_strcat(cmd, tmpStr);
        }

        if (webLocalInfo.authorInfo.reauthTime != 0)
        {
            SYS_MEM_CLEAR(tmpStr);
            osal_sprintf(tmpStr, "reauth-period %u ", webLocalInfo.authorInfo.reauthTime);
            osal_strcat(cmd, tmpStr);
        }

        if (webLocalInfo.authorInfo.inactTime != 0)
        {
            SYS_MEM_CLEAR(tmpStr);
            osal_sprintf(tmpStr, "inactive-timeout %u ", webLocalInfo.authorInfo.inactTime);
            osal_strcat(cmd, tmpStr);
        }

        osal_strcat(cmd, "\n");

        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

    return SYS_ERR_OK;
}

int32 cmd_authmgr_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    char tmp[SYS_BUF32_LEN];
    uint32 time = 0;
    uint32 num = 0;
    uint32 i = 0;
    sys_enable_t enable;
    authmgr_portCtrl_t ctrl;
    authmgr_mode_t mode;
    authmgr_order_t order;
    authmgr_vlanAssign_t vlanAssignMode;
#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
    authmgr_methodOrder_t methodOrder;
#endif

    CMD_SAVE_IF_TRKLP_SKIP(port);

#ifdef CONFIG_SYS_PROTO_DOT1X
    SYS_ERR_CHK(sal_authmgr_portTypeEnbl_get(port, AUTHMGR_TYPE_DOT1X, &enable));

    if (DFLT_AUTHMGR_PORT_DOT1X_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sauthentication dot1x\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
    SYS_ERR_CHK(sal_authmgr_portTypeEnbl_get(port, AUTHMGR_TYPE_MAC, &enable));

    if (DFLT_AUTHMGR_PORT_MAC_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sauthentication mac\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    SYS_ERR_CHK(sal_authmgr_portTypeEnbl_get(port, AUTHMGR_TYPE_WEB, &enable));

    if (DFLT_AUTHMGR_PORT_WEB_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sauthentication web\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

    SYS_ERR_CHK(sal_authmgr_portCtrl_get(port, &ctrl));

    if (DFLT_AUTHMGR_PORT_CTRL != ctrl)
    {
        SYS_MEM_CLEAR(cmd);

        osal_strcpy(cmd, "authentication port-control ");

        switch (ctrl)
        {
            case AUTHMGR_PORTCTRL_FORCE_AUTHORIZED:
                osal_strcat(cmd, "force-auth\n");
                break;

            case AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED:
                osal_strcat(cmd, "force-unauth\n");
                break;

            case AUTHMGR_PORTCTRL_AUTO:
                osal_strcat(cmd, "auto\n");
                break;

            default:
                break;
        }

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portMode_get(port, &mode));

    if (DFLT_AUTHMGR_PORT_MODE != mode)
    {
        SYS_MEM_CLEAR(cmd);

        osal_strcpy(cmd, "authentication host-mode ");

        switch (mode)
        {
            case AUTHMGR_MODE_MULTI_AUTH:
                osal_strcat(cmd, "multi-auth\n");
                break;

            case AUTHMGR_MODE_MULTI_HOST:
                osal_strcat(cmd, "multi-host\n");
                break;

            case AUTHMGR_MODE_SINGLE_HOST:
                osal_strcat(cmd, "single-host\n");
                break;

            default:
                break;
        }

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_MEM_CLEAR(order);
    SYS_ERR_CHK(sal_authmgr_portOrder_get(port, &order));

    if (DFLT_AUTHMGR_PORT_ORDER_FIRST != order.order[0] || AUTHMGR_TYPE_NONE != order.order[1])
    {
        SYS_MEM_CLEAR(cmd);

        osal_strcpy(cmd, "authentication order ");

        for (i = 0; i < AUTHMGR_TYPE_END - 1; i++)
        {
            if (AUTHMGR_TYPE_NONE == order.order[i])
                break;

            SYS_MEM_CLEAR(tmp);
            osal_strcpy(tmp, text_authmgr_type[order.order[i]].text);
            osal_strcat(cmd, tmp);
            osal_strcat(cmd, " ");
        }

        osal_strcat(cmd, "\n");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
    
    SYS_MEM_CLEAR(methodOrder);
    SYS_ERR_CHK(sal_authmgr_portMethodOrder_get(port, &methodOrder));

    if (DFLT_AUTHMGR_PORT_METHOD_ORDER_FIRST != methodOrder.order[0] || AUTHMGR_METHOD_NONE != methodOrder.order[1])
    {
        SYS_MEM_CLEAR(cmd);

        osal_strcpy(cmd, "authentication method ");

        for (i = 0; i < AUTHMGR_METHOD_END - 1; i++)
        {
            if (AUTHMGR_METHOD_NONE == methodOrder.order[i])
                break;

            SYS_MEM_CLEAR(tmp);
            osal_strcpy(tmp, text_authmgr_method[methodOrder.order[i]].text);
            osal_strcat(cmd, tmp);
            osal_strcat(cmd, " ");
        }

        osal_strcat(cmd, "\n");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

    SYS_ERR_CHK(sal_authmgr_portReauthEnbl_get(port, &enable));

    if (DFLT_AUTHMGR_PORT_REAUTH_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sauthentication reauth\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portMaxHost_get(port, &num));

    if (DFLT_AUTHMGR_PORT_MAX_HOSTS != num)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "authentication max-hosts %u\n", num);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portVlanAssign_get(port, &vlanAssignMode));

    if (DFLT_AUTHMGR_PORT_VLAN_ASSIGN_MODE != vlanAssignMode)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%sauthentication radius-attributes vlan ", (AUTHMGR_VLAN_ASSIGN_DISABLE != vlanAssignMode) ? "" : "no ");

        switch (vlanAssignMode)
        {
            case AUTHMGR_VLAN_ASSIGN_REJECT:
                osal_strcat(cmd, "reject");
                break;

            case AUTHMGR_VLAN_ASSIGN_STATIC:
                osal_strcat(cmd, "static");
                break;

            default:
                break;
        }

        osal_strcat(cmd, "\n");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portReauthPeriod_get(port, &time));

    if (DFLT_AUTHMGR_PORT_REAUTH_PERIOD != time)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "authentication timer reauth %u\n", time);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portInactTimeout_get(port, &time));

    if (DFLT_AUTHMGR_PORT_INACT_PERIOD != time)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "authentication timer inactive %u\n", time);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portQuietPeriod_get(port, &time));

    if (DFLT_AUTHMGR_PORT_QUIET_PERIOD != time)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "authentication timer quiet %u\n", time);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portGuestVlanEnbl_get(port, &enable));

    if (DFLT_AUTHMGR_PORT_GUEST_VLAN_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sauthentication guest-vlan\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

#ifdef CONFIG_SYS_PROTO_DOT1X
    
    SYS_ERR_CHK(sal_authmgr_portSuppTimeout_get(port, &time));

    if (DFLT_AUTHMGR_PORT_SUPP_TIMEOUT != time)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dot1x timeout supp-timeout %u\n", time);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portSrvTimeout_get(port, &time));

    if (DFLT_AUTHMGR_PORT_SRV_TIMEOUT != time)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dot1x timeout server-timeout %u\n", time);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portEapTxPeriod_get(port, &time));

    if (DFLT_AUTHMGR_PORT_EAP_TX_PERIOD != time)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dot1x timeout tx-period %u\n", time);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_authmgr_portMaxEapReq_get(port, &num));

    if (DFLT_AUTHMGR_PORT_MAX_REQ != num)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dot1x max-req %u\n", num);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    
    SYS_ERR_CHK(sal_authmgr_portWebauthLoginAttempt_get(port, &num));

    if (DFLT_AUTHMGR_PORT_WEBAUTH_LOGIN_ATTEMPT != num)
    {
        SYS_MEM_CLEAR(cmd);
        if (0 == num)
            osal_sprintf(cmd, "authentication web max-login-attempts infinite\n");
        else
            osal_sprintf(cmd, "authentication web max-login-attempts %u\n", num);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

    return SYS_ERR_OK;
}

