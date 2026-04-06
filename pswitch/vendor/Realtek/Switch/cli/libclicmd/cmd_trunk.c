
#include <libsal/sal_trunk.h>
#include <libcmd/cmd.h>

int32 cmd_trunk_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 trunkId;
    char strPortmask[CAPA_PORTMASK_STR_LEN];
    char strPortmaskFinal[CAPA_PORTMASK_STR_LEN*2];
    sys_trunk_hashAlgo_t algo;
    sys_logic_portmask_t memberPm;
    sys_logic_portmask_t activePm;
    sys_logic_portmask_t standbyPm;

    SYS_ERR_CHK(sal_trunk_hashAlgo_get(&algo));
    XMORE("Load Balancing: ");
    switch (algo)
    {
        case LAG_HASH_ALGO_MAC:
            XMORE("src-dst-mac.");
            break;

        case LAG_HASH_ALGO_IP_MAC:
            XMORE("src-dst-mac-ip.");
            break;
            
        case LAG_HASH_ALGO_DMAC:
            XMORE("dst-mac.");
            break;
            
        case LAG_HASH_ALGO_SMAC:
            XMORE("src-mac.");
            break;
            
        case LAG_HASH_ALGO_DIP:
            XMORE("dst-ip.");
            break;
            
        case LAG_HASH_ALGO_SIP:
            XMORE("src-ip.");
            break;
            
        default:
            break;
    }
    XMORE("\n\n");

    XMORE(" Group ID |  Type  |          Ports\n");
    XMORE("----------+--------+----------------------------------------------\n");
    FOR_EACH_TRUNK(trunkId)
    {
        osal_memset(&memberPm, 0, sizeof(memberPm));
        osal_memset(&activePm, 0, sizeof(activePm));
        osal_memset(&standbyPm, 0, sizeof(standbyPm));

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));
        SYS_ERR_CHK(sal_trunk_activeMember_get(trunkId, &activePm));

        osal_memset(strPortmaskFinal, 0, sizeof(strPortmaskFinal));

        XMORE("  %2d      |", TRK2UID(trunkId));
        if (IS_TRK_LACP(trunkId))
            XMORE("  LACP  |");
        else if (IS_TRK_STATIC(trunkId))
            XMORE(" Static |");
        else
            XMORE(" ------ |");

        LOGIC_PORTMASK_ANDNOT(standbyPm, memberPm, activePm);

        if (!IS_LOGIC_PORTMASK_CLEAR(activePm))
        {
            osal_memset(strPortmask, 0, sizeof(strPortmask));
            LPM2STR(&activePm, strPortmask);
            osal_strcat(strPortmaskFinal, " Active: ");
            osal_strcat(strPortmaskFinal, strPortmask);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(standbyPm))
        {
            osal_memset(strPortmask, 0, sizeof(strPortmask));
            LPM2STR(&standbyPm, strPortmask);
            osal_strcat(strPortmaskFinal, " Inactive: ");
            osal_strcat(strPortmaskFinal, strPortmask);
        }

        XMORE(" %s\n", strPortmaskFinal);
    }

    return SYS_ERR_OK;
}

int32 cmd_trunk_hashAlgo_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_trunk_hashAlgo_t algo;

    algo = SYS_STR2UINT(argv[0]);

    if (CMD_IS_NO_FORM_EXIST())
        algo = DFLT_LAG_HASH_ALGO;
    else
    {
    	if (0 == osal_strcmp(argv[0], "src-dst-mac"))
    	{
			algo = LAG_HASH_ALGO_MAC;
    	}
    	else if(0 == osal_strcmp(argv[0], "src-dst-mac-ip"))
    	{
			algo = LAG_HASH_ALGO_IP_MAC;
    	}
    	else if(0 == osal_strcmp(argv[0], "dst-mac"))
    	{
			algo = LAG_HASH_ALGO_DMAC;
    	}
    	else if(0 == osal_strcmp(argv[0], "dst-ip"))
    	{
			algo = LAG_HASH_ALGO_DIP;
    	}
    	else if(0 == osal_strcmp(argv[0], "src-mac"))
    	{
			algo = LAG_HASH_ALGO_SMAC;
    	}
    	else if(0 == osal_strcmp(argv[0], "src-ip"))
    	{
			algo = LAG_HASH_ALGO_SMAC;
    	}
    }

    SYS_ERR_CHK(sal_trunk_hashAlgo_set(algo));

    return SYS_ERR_OK;
}

int32 cmd_trunk_staticGroup_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    uint32 trunkId;
    sys_logic_port_t lp;
    sys_logic_portmask_t oldMask;
    sys_logic_portmask_t newMask;
 #ifdef CONFIG_SYS_VOICEVLAN
    sys_enable_t enable;
    voice_vlan_state_t state;
    sys_vid_t voiceVid;
    sys_vlan_membership_t mbrShip;
 #endif

    if (CMD_IS_NO_FORM_EXIST())
    {
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            trunkId = LP2TRKID(lp);

            if (SYS_TRUNK_INVALID_ID == trunkId)
                continue;

            SYS_MEM_CLEAR(newMask);
            SYS_ERR_CONTINUE(sal_trunk_member_get(trunkId, &newMask));

            LOGIC_PORTMASK_CLEAR_PORT(newMask, lp);

#ifdef CONFIG_SYS_PROTO_LACP
            if(IS_TRK_LACP(trunkId))
            {
                sys_logic_portmask_t newActMask;

                SYS_MEM_CLEAR(newActMask);

                SYS_ERR_CONTINUE(sal_lacp_activeMode_get(trunkId, &newActMask));

                LOGIC_PORTMASK_CLEAR_PORT(newActMask, lp);

                SYS_ERR_CONTINUE(sal_lacp_member_set(trunkId, &newMask));
                SYS_ERR_CONTINUE(sal_lacp_activeMode_set(trunkId, &newActMask));
            }
            else
#endif
            {
                SYS_ERR_CONTINUE(sal_trunk_staticMember_set(trunkId, &newMask));
            }
        }

        return SYS_ERR_OK;
    }

    osal_memset(&oldMask, 0, sizeof(oldMask));
    osal_memset(&newMask, 0, sizeof(newMask));

    trunkId = SYS_STR2UINT(argv[0]) - 1;

    SYS_ERR_CHK(sal_trunk_member_get(trunkId, &oldMask));

    LOGIC_PORTMASK_OR(newMask, oldMask, g_selLpm);

#ifdef CONFIG_SYS_VOICEVLAN
    SYS_ERR_CHK(sal_vlan_voiceVlanState_get(&state));
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
#endif

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        
        if (IS_LP_TRKMBR(lp))
            continue;

        SYS_MEM_CLEAR(portStr);
        LP2FULLSTR(lp, portStr);

#ifdef CONFIG_SYS_VOICEVLAN
        SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(lp,&enable));
        SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_get(lp,voiceVid, &mbrShip));

        if((VOICE_VLAN_OUI_ENABLED == state) &&
            (ENABLED == enable) &&(SYS_VLAN_TAGGED == mbrShip))
        {
            XMORE("port %s belong to voice VLAN\n", portStr);
            LOGIC_PORTMASK_CLEAR_PORT(newMask, lp);
        }
#endif

        if (sal_l2_staticMacExistOnPort_ret(lp))
        {
            XMORE("Static mac entries are associated with %s\n", portStr);
            XMORE("Remove those and reconfigure(if needed) on etherchannel.\n");
            XMORE("%s is not added to port channel %s\n", portStr, argv[0]);

            LOGIC_PORTMASK_CLEAR_PORT(newMask, lp);
        }
    }

    if (!IS_NORMAL_LOGIC_PORTMASK_CLEAR(oldMask) && IS_TRK_LACP(trunkId))
    {
        
        SYS_ERR_CHK(sal_trunk_memberNum_check(SYS_PORT_TRUNK_STATIC, &newMask));

        SYS_ERR_CHK(sal_trunk_group_del(trunkId));
    }

    SYS_ERR_CHK(sal_trunk_staticMember_set(trunkId, &newMask));

    return SYS_ERR_OK;
}

int32 cmd_trunk_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_trunk_hashAlgo_t algo;

    SYS_ERR_CHK(sal_trunk_hashAlgo_get(&algo));

    if (DFLT_LAG_HASH_ALGO != algo)
    {
        osal_memset(cmd, 0, sizeof(cmd));
        osal_sprintf(cmd, "lag load-balance ");

        switch (algo)
        {
            case LAG_HASH_ALGO_MAC:
                osal_strcat(cmd, "src-dst-mac\n");
                break;

            case LAG_HASH_ALGO_IP_MAC:
                osal_strcat(cmd, "src-dst-mac-ip\n");
                break;
                
            case LAG_HASH_ALGO_DMAC:
                osal_strcat(cmd, "dst-mac\n");
                break;
                
            case LAG_HASH_ALGO_SMAC:
                osal_strcat(cmd, "src-mac\n");
                break;
                
            case LAG_HASH_ALGO_DIP:
                osal_strcat(cmd, "dst-ip\n");
                break;
                
            case LAG_HASH_ALGO_SIP:
                osal_strcat(cmd, "src-ip\n");
                break;

            default:
                break;
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_trunk_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 trkId = 0;

    CMD_SAVE_IF_TRKLP_SKIP(port);

    trkId = LP2TRKID(port);

    if ((SYS_TRUNK_INVALID_ID != trkId) && (IS_TRK_STATIC(trkId)))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lag %d mode static\n", TRK2UID(trkId));

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

