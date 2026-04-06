
#include <libsal/sal_lldp.h>
#include <libcmd/cmd.h>

int32 cmd_lldp_med_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 fastRepeatCount;
    int i;
    sys_enable_t enable;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    char lldpUserPort[SYS_BUF32_LEN];
    char strLine[SYS_BUF512_LEN], strTmp[SYS_BUF128_LEN];
    lldp_med_networkPolicy_t policy;
    lldp_med_tlv_sel_t lldpMedTlvSel;

    SYS_ERR_CHK(sal_lldp_medFastRepeatCount_get(&fastRepeatCount));

    XMORE("\n");
    XMORE(" Fast Start Repeat Count: %d\n", fastRepeatCount);
    XMORE("\n");
    for (i = 0; i < CAPA_LLDP_MED_NETWORK_POLICY_NUM; i ++)
    {
        SYS_ERR_CHK(sal_lldp_medNetworkPolicy_get(i, &policy));
        if (TRUE == policy.valid)
        {
            XMORE("Network policy %d\n", i + 1);
            XMORE("-------------------\n");
            osal_strcpy(strLine, "Application type: ");
            switch(policy.app)
            {
                case LLDP_APP_VOICE:
                    osal_strcat(strLine, "Voice");
                    break;
                case LLDP_APP_VOICE_SIGNALING:
                    osal_strcat(strLine, "Voice Signaling");
                    break;
                case LLDP_APP_GUEST_VOICE:
                    osal_strcat(strLine, "Guest Voice");
                    break;
                case LLDP_APP_GUEST_VOICE_SIGNALING:
                    osal_strcat(strLine, "Guest Voice Signaling");
                    break;
                case LLDP_APP_SOFTPHONE_VOICE:
                    osal_strcat(strLine, "Softphone Voice");
                    break;
                case LLDP_APP_VIDEO_CONFERENCING:
                    osal_strcat(strLine, "Conferencing");
                    break;
                case LLDP_APP_STREAMING_VIDEO:
                    osal_strcat(strLine, "Streaming Video");
                    break;
                case LLDP_APP_VIDEO_SIGNALING:
                    osal_strcat(strLine, "Video Signaling");
                    break;
                default:
                    break;
            }

            XMORE("%s\n", strLine);

            XMORE("VLAN ID: %d %s\n", policy.vid, policy.vlanTagged?"tagged":"untagged");
            XMORE("Layer 2 priority: %d\n", policy.userPriority);
            XMORE("DSCP: %d\n\n", policy.dscpValue);
        }
    }

    XMORE(" Port   | Capabilities | Network Policy | Location | Inventory | PoE PSE\n");
    XMORE(" ------ + ------------ + -------------- + -------- + --------- + -------\n");

    NORMAL_LOGIC_PORTMASK_SET_ALL(logicPortmask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        osal_sprintf(strLine, " %6s |", lldpUserPort);
        SYS_ERR_CHK(sal_lldp_portMedEnable_get(logicPort,&enable));
        if (ENABLED == enable)
        {
            osal_sprintf(strTmp, " %12s |", "Yes");
            osal_strcat(strLine, strTmp);
        }
        else
        {
            osal_sprintf(strTmp, " %12s |", "No");
            osal_strcat(strLine, strTmp);
        }

        SYS_ERR_CHK(sal_lldp_portMedTlvSel_get(logicPort, &lldpMedTlvSel));

        if (TRUE == lldpMedTlvSel.networkPolicySel)
        {
            osal_sprintf(strTmp, " %14s |", "Yes");
            osal_strcat(strLine, strTmp);
        }
        else
        {
            osal_sprintf(strTmp, " %14s |", "No");
            osal_strcat(strLine, strTmp);
        }
        if (TRUE == lldpMedTlvSel.locationSel)
        {
            osal_sprintf(strTmp, " %8s |", "Yes");
            osal_strcat(strLine, strTmp);
        }
        else
        {
            osal_sprintf(strTmp, " %8s |", "No");
            osal_strcat(strLine, strTmp);
        }
        if (TRUE == lldpMedTlvSel.inventorySel)
        {
            osal_sprintf(strTmp, " %9s |", "Yes");
            osal_strcat(strLine, strTmp);
        }
        else
        {
            osal_sprintf(strTmp, " %9s |", "No");
            osal_strcat(strLine, strTmp);
        }
        if (IS_POE_PORT(logicPort))
        {
            if (TRUE == lldpMedTlvSel.poePseSel)
            {
                osal_sprintf(strTmp, " %7s", "Yes");
                osal_strcat(strLine, strTmp);
            }
            else
            {
                osal_sprintf(strTmp, " %7s", "No");
                osal_strcat(strLine, strTmp);
            }
        }
        else
        {
            osal_sprintf(strTmp, " %7s", "N/A");
            osal_strcat(strLine, strTmp);
        }

        XMORE("%s", strLine);
        XMORE("\n");
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_lldp_portMed_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 fastRepeatCount;
    int i;
    sys_enable_t enable;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    char lldpUserPort[SYS_BUF32_LEN];
    char strLine[SYS_BUF512_LEN], strTmp[SYS_BUF512_LEN], strTmp2[SYS_BUF16_LEN];
    lldp_med_tlv_sel_t lldpMedTlvSel;
    lldp_med_networkPolicy_bind_t policyBind;
    lldp_med_location_t location;

    SYS_ERR_CHK(sal_lldp_medFastRepeatCount_get(&fastRepeatCount));
    SYS_ERR_CHK(sal_lldp_medNetworkPolicyAuto_get(&enable));

    XMORE("\n");

    XMORE(" Port   | Capabilities | Network Policy | Location | Inventory | PoE PSE\n");
    XMORE(" ------ + ------------ + -------------- + -------- + --------- + -------\n");

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        osal_sprintf(strLine, " %6s |", lldpUserPort);
        SYS_ERR_CHK(sal_lldp_portMedEnable_get(logicPort,&enable));
        if (ENABLED == enable)
        {
            osal_sprintf(strTmp, " %12s |", "Yes");
            osal_strcat(strLine, strTmp);
        }
        else
        {
            osal_sprintf(strTmp, " %12s |", "No");
            osal_strcat(strLine, strTmp);
        }

        SYS_ERR_CHK(sal_lldp_portMedTlvSel_get(logicPort, &lldpMedTlvSel));

        if (TRUE == lldpMedTlvSel.networkPolicySel)
        {
            osal_sprintf(strTmp, " %14s |", "Yes");
            osal_strcat(strLine, strTmp);
        }
        else
        {
            osal_sprintf(strTmp, " %14s |", "No");
            osal_strcat(strLine, strTmp);
        }
        if (TRUE == lldpMedTlvSel.locationSel)
        {
            osal_sprintf(strTmp, " %8s |", "Yes");
            osal_strcat(strLine, strTmp);
        }
        else
        {
            osal_sprintf(strTmp, " %8s |", "No");
            osal_strcat(strLine, strTmp);
        }
        if (TRUE == lldpMedTlvSel.inventorySel)
        {
            osal_sprintf(strTmp, " %9s |", "Yes");
            osal_strcat(strLine, strTmp);
        }
        else
        {
            osal_sprintf(strTmp, " %9s |", "No");
            osal_strcat(strLine, strTmp);
        }
        if (IS_POE_PORT(logicPort))
        {
            if (TRUE == lldpMedTlvSel.poePseSel)
            {
                osal_sprintf(strTmp, " %7s", "Yes");
                osal_strcat(strLine, strTmp);
            }
            else
            {
                osal_sprintf(strTmp, " %7s", "No");
                osal_strcat(strLine, strTmp);
            }
        }
        else
        {
            osal_sprintf(strTmp, " %7s", "N/A");
            osal_strcat(strLine, strTmp);
        }

        XMORE("%s", strLine);
        XMORE("\n");
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        XMORE("\nPort ID: %s\n", lldpUserPort);
        SYS_ERR_CHK(sal_lldp_portMedNetworkPolicyBind_get(logicPort, &policyBind));

        osal_memset(strTmp, 0, sizeof(strTmp));
        for (i = 0; i < CAPA_LLDP_MED_NETWORK_POLICY_NUM; i ++)
            if (policyBind.bindMask & (1 << i))
            {
                osal_sprintf(strTmp2, ", %d", i+1);
                osal_strcat(strTmp, strTmp2);
            }

        if (osal_strlen(strTmp) > 0)
            XMORE("Network policies: %s\n", &strTmp[2]);
        else
            XMORE("Network policies:\n");

        SYS_ERR_CHK(sal_lldp_portMedLocation_get(logicPort, &location));

        if ((location.coorLen + location.civicLen + location.ecsLen) > 0)
        {
            osal_memset(strTmp, 0, sizeof(strTmp));
            XMORE("Location:\n");
            if (location.coorLen> 0)
            {
                SYS_ERR_CHK(sys_util_byteArray2hexStr(location.coordinate, location.coorLen, strTmp));
                XMORE("Coordinates: %s\n", strTmp);
            }
            osal_memset(strTmp, 0, sizeof(strTmp));
            if (location.civicLen > 0)
            {
                SYS_ERR_CHK(sys_util_byteArray2hexStr(location.civicAddress, location.civicLen, strTmp));
                XMORE("Civic-address: %s\n", strTmp);
            }
            osal_memset(strTmp, 0, sizeof(strTmp));
            if (location.ecsLen > 0)
            {
                SYS_ERR_CHK(sys_util_byteArray2hexStr(location.ecsElin, location.ecsLen, strTmp));
                XMORE("Ecs-elin: %s\n", strTmp);
            }
        }
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_lldp_med_fast_start_repeat_count_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 fastRepeatCount = SYS_STR2UINT(argv[0]);

    if (CMD_IS_NO_FORM_EXIST())
        fastRepeatCount = DFLT_LLDP_MED_FAST_RETRY;
    else
        fastRepeatCount = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_lldp_medFastRepeatCount_set(fastRepeatCount));
    return SYS_ERR_OK;
}

int32 cmd_lldp_med_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        enable = DISABLED;
    }
    else
    {
        if (argc == 1)
        {
            if (argv[0][0] == 'e')
                enable = ENABLED;
            else
                enable = DISABLED;
        }
        else
        {
            enable = ENABLED;
        }
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lldp_portMedEnable_set(logicPort, enable), logicPort);
    }

    return CMD_SUCCESS;
}

int32 cmd_lldp_med_optional_tlv_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_logic_port_t port;
    lldp_med_tlv_sel_t lldpMedTlvSel;

    osal_memset(&lldpMedTlvSel, 0, sizeof(lldpMedTlvSel));

    if (CMD_IS_NO_FORM_EXIST())
    {
        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
        {
            CMD_ERR_CONTINUE_PORT(sal_lldp_portMedTlvSel_set(port, lldpMedTlvSel), port);

            CMD_ERR_CONTINUE_PORT(sal_lldp_txOverload_check(port), port);
        }

        return SYS_ERR_OK;
    }

    for (i = 0; i < argc; i++)
    {
        if (0 == osal_strcmp(argv[i], "network-policy"))
            lldpMedTlvSel.networkPolicySel = TRUE;
        else if (0 == osal_strcmp(argv[i], "location"))
            lldpMedTlvSel.locationSel = TRUE;
        else if (0 == osal_strcmp(argv[i], "inventory"))
            lldpMedTlvSel.inventorySel = TRUE;
        else if (0 == osal_strcmp(argv[i], "poe-pse"))
            lldpMedTlvSel.poePseSel = TRUE;
        else
        {
            XMORE("%s is not a valid MED optional TLV\n", argv[i]);
            return SYS_ERR_OK;
        }
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lldp_portMedTlvSel_set(port, lldpMedTlvSel), port);

        CMD_ERR_CONTINUE_PORT(sal_lldp_txOverload_check(port), port);
    }
    return SYS_ERR_OK;
}

int32 cmd_lldp_med_network_policy_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 idx;
    lldp_med_networkPolicy_t policy;

    osal_memset(&policy, 0, sizeof(policy));

    if (CMD_IS_NO_FORM_EXIST())
    {
        idx = SYS_STR2UINT(argv[0]) - 1;
        SYS_ERR_CHK(sal_lldp_medNetworkPolicy_set(idx, policy));

        return SYS_ERR_OK;
    }

    policy.valid = TRUE;
    idx = SYS_STR2UINT(argv[0]) - 1;
    if (0 == osal_strcmp(argv[1], "voice"))
        policy.app = LLDP_APP_VOICE;
    else if (0 == osal_strcmp(argv[1], "voice-signaling"))
        policy.app = LLDP_APP_VOICE_SIGNALING;
    else if (0 == osal_strcmp(argv[1], "guest-voice"))
        policy.app = LLDP_APP_GUEST_VOICE;
    else if (0 == osal_strcmp(argv[1], "guest-voice-signaling"))
        policy.app = LLDP_APP_GUEST_VOICE_SIGNALING;
    else if (0 == osal_strcmp(argv[1], "softphone-voice"))
        policy.app = LLDP_APP_SOFTPHONE_VOICE;
    else if (0 == osal_strcmp(argv[1], "video-conferencing"))
        policy.app = LLDP_APP_VIDEO_CONFERENCING;
    else if (0 == osal_strcmp(argv[1], "streaming-video"))
        policy.app = LLDP_APP_STREAMING_VIDEO;
    else if (0 == osal_strcmp(argv[1], "video-signaling"))
        policy.app = LLDP_APP_VIDEO_SIGNALING;

    policy.vid = SYS_STR2UINT(argv[2]);

    if (0 == osal_strcmp(argv[3], "tag"))
        policy.vlanTagged = TRUE;
    else
        policy.vlanTagged = FALSE;

    policy.userPriority = SYS_STR2UINT(argv[4]);
    policy.dscpValue = SYS_STR2UINT(argv[5]);

    SYS_ERR_CHK(sal_lldp_medNetworkPolicy_set(idx, policy));

    return SYS_ERR_OK;
}

int32 cmd_lldp_med_network_policy_add_remove_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 policyMask;
    uint32 isAdd = 0;
    sys_logic_port_t logicPort;
    lldp_med_networkPolicy_bind_t policyBind;

    SYS_ERR_CHK(sys_util_str2ulMask(argv[1], &policyMask));

    if (0 == osal_strcmp(argv[0], "add"))
        isAdd = TRUE;
    else if (0 == osal_strcmp(argv[0], "remove"))
        isAdd = FALSE;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lldp_portMedNetworkPolicyBind_get(logicPort, &policyBind), logicPort);
        policyBind.bindMask &= ~ policyMask;
        if (TRUE == isAdd)
            policyBind.bindMask |= policyMask;
        CMD_ERR_CONTINUE_PORT(sal_lldp_portMedNetworkPolicyBind_set(logicPort, policyBind), logicPort);
    }

    return CMD_SUCCESS;
}

#if 0

int32 cmd_lldp_med_network_policy_voice_auto_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_lldp_medNetworkPolicyAuto_set(enable));

    return CMD_SUCCESS;
}
#endif

int32 cmd_lldp_med_location_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port;
    lldp_med_location_t location;
    uint16 strLen;
    uint16 size;
    uint8 byteArray[256];
    int32 ret;

    if (CMD_IS_NO_FORM_EXIST())
    {
        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
        {
            CMD_ERR_CONTINUE_PORT(sal_lldp_portMedLocation_get(port, &location), port);

            if (0 == osal_strcmp(argv[0], "coordinate"))
            {
                osal_memset(location.coordinate, 0, sizeof(location.coordinate));
                location.coorLen = 0;
            }
            else if (0 == osal_strcmp(argv[0], "civic-address"))
            {
                osal_memset(location.civicAddress, 0, sizeof(location.civicAddress));
                location.civicLen = 0;
            }
            else if (0 == osal_strcmp(argv[0], "ecs-elin"))
            {
                osal_memset(location.ecsElin, 0, sizeof(location.ecsElin));
                location.ecsLen = 0;
            }

            CMD_ERR_CONTINUE_PORT(sal_lldp_portMedLocation_set(port, location), port);

            CMD_ERR_CONTINUE_PORT(sal_lldp_txOverload_check(port), port);
        }

        return SYS_ERR_OK;
    }

    strLen = osal_strlen(argv[1]);

    size = strLen/2;
    if (1 == (strLen % 2))
        return SYS_ERR_LLDP_LOCATION_FORMAT;

    osal_memset(byteArray, 0, sizeof(byteArray));
    ret = sys_util_hexStr2byteArray(argv[1], byteArray);
    if (SYS_ERR_OK != ret)
        return SYS_ERR_LLDP_LOCATION_FORMAT;

    if (0 == osal_strcmp(argv[0], "coordinate"))
    {
        SYS_PARAM_CHK((size != CAPA_LLDP_MED_COORDINATE_LEN), SYS_ERR_INPUT);
    }
    else if (0 == osal_strcmp(argv[0], "civic-address"))
    {
        SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(size, BOND_LLDP_MED_CIVIC_LEN_MIN , BOND_LLDP_MED_CIVIC_LEN_MAX ), SYS_ERR_INPUT);
    }
    else if (0 == osal_strcmp(argv[0], "ecs-elin"))
    {
        SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(size, BOND_LLDP_MED_ELIN_LEN_MIN, BOND_LLDP_MED_ELIN_LEN_MAX), SYS_ERR_INPUT);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lldp_portMedLocation_get(port, &location), port);

        if (0 == osal_strcmp(argv[0], "coordinate"))
        {
            osal_memcpy(location.coordinate, byteArray, sizeof(location.coordinate));
            location.coorLen = size;
        }
        else if (0 == osal_strcmp(argv[0], "civic-address"))
        {
            osal_memcpy(location.civicAddress, byteArray, sizeof(location.civicAddress));
            location.civicLen = size;
        }
        else if (0 == osal_strcmp(argv[0], "ecs-elin"))
        {
            osal_memcpy(location.ecsElin, byteArray, sizeof(location.ecsElin));
            location.ecsLen = size;
        }

        CMD_ERR_CONTINUE_PORT(sal_lldp_portMedLocation_set(port, location), port);
    }

    return CMD_SUCCESS;
}

int32 cmd_lldp_med_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char appStr[SYS_BUF32_LEN];
    uint8 val = 0;
    uint32 i = 0;
    lldp_med_networkPolicy_t policy;

    SYS_ERR_CHK(sal_lldp_medFastRepeatCount_get(&val));

    if (DFLT_LLDP_MED_FAST_RETRY != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lldp med fast-start-repeat-count %d\n", val);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    for (i = 0; i < CAPA_LLDP_MED_NETWORK_POLICY_NUM; i++)
    {
        SYS_ERR_CONTINUE(sal_lldp_medNetworkPolicy_get(i, &policy));

        if (FALSE == policy.valid)
        {
            continue;
        }

        switch(policy.app)
        {
            case LLDP_APP_VOICE:
                osal_strcpy(appStr, "voice");
                break;

            case LLDP_APP_VOICE_SIGNALING:
                osal_strcpy(appStr, "voice-signaling");
                break;

            case LLDP_APP_GUEST_VOICE:
                osal_strcpy(appStr, "guest-voice");
                break;

            case LLDP_APP_GUEST_VOICE_SIGNALING:
                osal_strcpy(appStr, "guest-voice-signaling");
                break;

            case LLDP_APP_SOFTPHONE_VOICE:
                osal_strcpy(appStr, "softphone-voice");
                break;

            case LLDP_APP_VIDEO_CONFERENCING:
                osal_strcpy(appStr, "video-conferencing");
                break;

            case LLDP_APP_STREAMING_VIDEO:
                osal_strcpy(appStr, "streaming-video");
                break;

            case LLDP_APP_VIDEO_SIGNALING:
                osal_strcpy(appStr, "video-signaling");
                break;

            default:
                break;
        }

        osal_sprintf(cmd, "lldp med network-policy %d app %s vlan %d vlan-type %s priority %d dscp %d\n",
                        i+1,
                        appStr,
                        policy.vid,
                        policy.vlanTagged ? "tag" : "untag",
                        policy.userPriority,
                        policy.dscpValue);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;

    return SYS_ERR_OK;
}

int32 cmd_lldp_med_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    char strPolicyMask[SYS_BUF128_LEN];
    char dataStr[SYS_BUF512_LEN];
    sys_enable_t enable = 0;
    lldp_med_tlv_sel_t dftTlvSel, emptyTlvSel;
    lldp_med_tlv_sel_t lldpMedTlvSel;
    lldp_med_networkPolicy_bind_t addPolicyBind;
    lldp_med_location_t location;

    SYS_ERR_CHK(sal_lldp_portMedEnable_get(port, &enable));

    if (DFLT_LLDP_MED_PORT_EBL   != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%slldp med\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_MEM_CLEAR(emptyTlvSel);
    SYS_MEM_CLEAR(dftTlvSel);
    SYS_MEM_CLEAR(lldpMedTlvSel);

    if (DFLT_LLDP_MED_TLV_SEL & (1 << LLDP_MED_CAPA_NETWORK_POLICY))
        dftTlvSel.networkPolicySel = 1;
    if (DFLT_LLDP_MED_TLV_SEL & (1 << LLDP_MED_CAPA_LOCATION))
        dftTlvSel.locationSel = 1;
    if (DFLT_LLDP_MED_TLV_SEL & (1 << LLDP_MED_CAPA_PSE) && IS_POE_PORT(port))
        dftTlvSel.poePseSel = 1;
    if (DFLT_LLDP_MED_TLV_SEL & (1 << LLDP_MED_CAPA_INVENTORY))
        dftTlvSel.inventorySel = 1;

    SYS_ERR_CHK(sal_lldp_portMedTlvSel_get(port, &lldpMedTlvSel));

    if (0 != osal_memcmp(&lldpMedTlvSel, &dftTlvSel, sizeof(lldp_med_tlv_sel_t)))
    {
        if (0 == osal_memcmp(&lldpMedTlvSel, &emptyTlvSel, sizeof(lldp_med_tlv_sel_t)))
        {
            CMD_WRITE_TO_FILE_INDENT1(fp, "no lldp med tlv-select\n");
        }
        else
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "lldp med tlv-select");

            if (TRUE == lldpMedTlvSel.networkPolicySel)
                osal_strcat(cmd, " network-policy");

            if (TRUE == lldpMedTlvSel.locationSel)
                osal_strcat(cmd, " location");

            if (TRUE == lldpMedTlvSel.poePseSel)
                osal_strcat(cmd, " poe-pse");

            if (TRUE == lldpMedTlvSel.inventorySel)
                osal_strcat(cmd, " inventory");

            osal_strcat(cmd, "\n");

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    SYS_MEM_CLEAR(addPolicyBind);

    SYS_ERR_CHK(sal_lldp_portMedNetworkPolicyBind_get(port, &addPolicyBind));

    if (0 != addPolicyBind.bindMask)
    {
        SYS_MEM_CLEAR(cmd);
        SYS_MEM_CLEAR(strPolicyMask);

        sys_util_ulMask2Str(addPolicyBind.bindMask, strPolicyMask);
        osal_sprintf(cmd, "lldp med network-policy add %s\n", strPolicyMask);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_lldp_portMedLocation_get(port, &location));

    if (0 != location.coorLen)
    {
        SYS_MEM_CLEAR(cmd);
        SYS_MEM_CLEAR(dataStr);

        sys_util_byteArray2hexStr(location.coordinate, location.coorLen, dataStr);

        osal_sprintf(cmd, "lldp med location coordinate %s\n", dataStr);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (0 != location.civicLen)
    {
        SYS_MEM_CLEAR(cmd);
        SYS_MEM_CLEAR(dataStr);

        sys_util_byteArray2hexStr(location.civicAddress, location.civicLen, dataStr);

        osal_sprintf(cmd, "lldp med location civic-address %s\n", dataStr);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (0 != location.ecsLen)
    {
        SYS_MEM_CLEAR(cmd);
        SYS_MEM_CLEAR(dataStr);

        sys_util_byteArray2hexStr(location.ecsElin, location.ecsLen, dataStr);

        osal_sprintf(cmd, "lldp med location ecs-elin %s\n", dataStr);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;

    return SYS_ERR_OK;
}

