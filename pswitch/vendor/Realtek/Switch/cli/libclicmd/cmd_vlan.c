
#include <libcmd/cmd.h>

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
#include <libsal/sal_igmp.h>
#endif
#include <libsal/sal_l2.h>
#ifdef CONFIG_SYS_PROTO_AUTHMGR
#include <libsal/sal_authmgr.h>
#endif
#ifdef CONFIG_SYS_L3_ROUTE
#include <common/sys_l3.h>
#endif

int32 cmd_vlan_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char strPort[CAPA_PORT_STR_LEN];
    char *pVlanModeStr[] = {
        "Hybrid",       
        "Access",       
        "Trunk",        
        "Tunnel"        
    };
    char *pStrVlanFrameType[] = { "all",
                                  "tagged-only",
                                  "untagged-only" };
    sys_vid_t vid;
    sys_enable_t enable;
    sys_enable_t uplink = DISABLED;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    sys_vlan_portMode_t mode = 0;
    sys_vlan_frameType_t type;
    sys_vlan_operEntry_t operEntry;
    sys_vlan_portMember_t portMember;
    sys_namestr_t vlanName;
    sys_vlanmask_t vlanmask;
    char strVlan[CAPA_VLANLIST_STR_LEN];

    CMD_PORTLIST_GET(0, &lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        
        LP2STR(lp, strPort);

        XMORE("Port : %s\n", strPort);

        SYS_ERR_CHK(sal_vlan_portAdminMode_get(lp, &mode));
        SYS_ERR_CHK(sal_vlan_portAdminRoleUplinkEnable_get(lp, &uplink));

        XMORE("Port Mode : %s%s\n", \
            pVlanModeStr[mode], \
            (ENABLED == uplink)? " (uplink)" : "");

#ifdef CONFIG_SYS_PROTO_GVRP
        
        SYS_ERR_CHK(sal_gvrp_port_enable_get(lp, &enable));

        XMORE("Gvrp Status : %s\n", text_enable[enable].text);
#endif
        
        SYS_ERR_CHK(sal_vlan_portAdminIgrFilterEnable_get(lp, &enable));

        XMORE("Ingress Filtering : %s\n", text_enable[enable].text);

        SYS_ERR_CHK(sal_vlan_portAdminAcceptFrameType_get(lp, &type));

        XMORE("Acceptable Frame Type : %s\n", pStrVlanFrameType[type]);

        SYS_ERR_CHK(sal_vlan_portOperPvid_get(lp, &vid));

        XMORE("Ingress UnTagged VLAN ( NATIVE ) : %d\n", vid);

        SYS_ERR_CHK(sal_vlan_portTrunkAllowedVlan_get(lp, &vlanmask));
        SYS_ERR_CHK(VMSK2STR(&vlanmask, strVlan));
        XMORE("Trunking VLANs Enabled: %s\n", strVlan);

        XMORE("\nPort is member in:\n");
        XMORE(" Vlan            Name              Egress rule  \n");
        XMORE("------- ----------------------- -----------------\n");
        FOR_EACH_VID(vid)
        {
            if (!IS_VLAN_OPER_ENTRY_EXIST(vid))
                continue;

            SYS_MEM_CLEAR(operEntry);
            SYS_ERR_CHK(sal_vlan_operEntry_get(vid, &operEntry));

            if (!IS_LOGIC_PORTMASK_PORTSET(operEntry.lpmMember, lp))
                continue;

            XMORE(" %4d ", vid);
            XMORE(" %21s ", operEntry.name.str);
            if (IS_LOGIC_PORTMASK_PORTSET(operEntry.lpmUntag, lp))
                XMORE(" %13s \n", "Untagged");
            else
                XMORE(" %13s \n", "Tagged");
        }

        SYS_ERR_CHK(rsd_vlan_portAdminVlanMember_get(lp, &portMember));

        XMORE("\nForbidden VLANs:\n");
        XMORE(" Vlan            Name          \n");
        XMORE("------- -----------------------\n");
        FOR_EACH_VID_IN_VLANMASK(vid, portMember.vmForbidden)
        {
            SYS_ERR_CHK(sal_vlan_name_get(vid, &vlanName));

            XMORE(" %4d  %21s \n", vid, vlanName.str);
        }
        XMORE("\n\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_dfltVlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t defaultVid;

    SYS_ERR_CHK(sal_vlan_defaultVid_get(&defaultVid));

    XMORE("    Default VLAN-ID : %u\n", defaultVid);

    return SYS_ERR_OK;
}

int32 cmd_vlan_portMembership_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlanmask_t vlanmask;
    sys_vlanmask_t vmExist;
    sys_vid_t vlanId;
    sys_vid_t vid;
    sys_vlan_operEntry_t operVlan;
    sys_logic_port_t logicPort = 0;
    sys_logic_portmask_t logicPortmask;
    sys_vlan_membership_t membership;
    char strPort[CAPA_PORT_STR_LEN] = {0};
    char *pVlanTypeStr[] = {
        "None",
        "Default",
        "Static",
        "Dynamic"
    };
    char *pVlanMembershipStr[] = {
        "Excluded",
        "Forbidden",
        "Tagged",
        "Untagged"
    };

    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));
    CMD_PORTLIST_GET(1, &logicPortmask);

    SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        if (VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_MEM_CLEAR(operVlan);

            vid = vlanId & 0xFFF;
            SYS_ERR_CHK(sal_vlan_operEntry_get(vid, &operVlan));

            XMORE("\n------------------------\n");
            XMORE(" VLAN ID   : %d\n", vid);
            
            XMORE(" VLAN Type : %s\n", pVlanTypeStr[operVlan.type]);
            XMORE("---------+--------------\n");
            XMORE("  Port   |  Membership\n");
            XMORE("---------+--------------\n");

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
            {
                SYS_ERR_CHK(sal_vlan_portOperVlanMembership_get(logicPort, vid, &membership));
                LP2STR(logicPort, strPort);
                XMORE("  %-6s | %11s\n", strPort, pVlanMembershipStr[membership]);
            }

            XMORE("---------+--------------\n");
        }
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_vlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
#define NAME_LEN_PER_LINE 16    

    sys_vlanmask_t vmList;
    sys_vlan_type_t disType = SYS_VLAN_ALL;
    sys_vid_t vid;
    sys_vlan_operEntry_t operEntry;
    sys_logic_portmask_t lpmTagged;
    int     nameLine, vlanNameLen, k;
    int     tagLine, untagLine, maxLine;
    char strPmTagged[CAPA_PORTMASK_STR_LEN] = {0};
    char strPmUntagged[CAPA_PORTMASK_STR_LEN] = {0};
    char    nameList[CAPA_NAME_STR_LEN / NAME_LEN_PER_LINE + 1][NAME_LEN_PER_LINE + 1];
    char    tagPortBuf[5][128];
    char    untagPortBuf[5][128];
    char *pVlanTypeStr[] = {
        "None",
        "Default",
        "Static",
        "Dynamic"
    };
    
    char   *ObjectN2Print[] =
    {
         "VID", 
         "VLAN Name", 
         "Untagged Ports", 
         "Tagged Ports", 
         "Type",
    };
	char  cInputChar = 0;
	vector printor = NULL;
	int rc = SYS_ERR_OK;
    SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vmList));

    if (argc)
    {
        if (NULL != osal_strstr(cmd->string, "static"))
        {
            disType = SYS_VLAN_STATIC;
        }
        else if(NULL != osal_strstr(cmd->string, "dynamic"))
        {
            disType = SYS_VLAN_DYNAMIC;
        }
        else
        {
            disType = SYS_VLAN_ALL;
            SYS_ERR_CHK(STR2VMSK(argv[0], &vmList));
        }
    }
	add_value_to_new_line_printor_bylen(PRINT_STRING, ObjectN2Print[0], &printor, 5);
	add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[1], &printor, NAME_LEN_PER_LINE+1);
	add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[2], &printor, 28);
	add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[3], &printor, 28);
	add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[4], &printor, 10);

    FOR_EACH_VID_IN_VLANMASK(vid, vmList)
    {
        if (!IS_VLAN_OPER_ENTRY_EXIST(vid))
            continue;

        SYS_MEM_CLEAR(operEntry);
        rc = sal_vlan_operEntry_get(vid, &operEntry);
        if( rc != SYS_ERR_OK)
        {	
			return free_printor_vector(&printor);
        }
			
        if (SYS_VLAN_STATIC == disType)
        {
            if ((SYS_VLAN_ENTRY_TYPE_DEFAULT != operEntry.type) &&
                (SYS_VLAN_ENTRY_TYPE_STATIC != operEntry.type))
                continue;
        }
        else if (SYS_VLAN_DYNAMIC == disType)
        {
            if (SYS_VLAN_ENTRY_TYPE_DYNAMIC != operEntry.type)
                continue;
        }
		
		nameLine = 0;
        vlanNameLen = strlen(operEntry.name.str);
		SYS_MEM_CLEAR(nameList);
        for (k = 0; ((vlanNameLen > 0) && (k < (vlanNameLen / NAME_LEN_PER_LINE))); k++)
        {
            memcpy(nameList[k], &operEntry.name.str[k * NAME_LEN_PER_LINE], NAME_LEN_PER_LINE);
            nameLine++;
        }
        if (vlanNameLen - (k * NAME_LEN_PER_LINE) > 0)
        {
            memcpy(nameList[k], &operEntry.name.str[k * NAME_LEN_PER_LINE], vlanNameLen - (k * NAME_LEN_PER_LINE));
            nameLine++;
        }
        
        SYS_MEM_CLEAR(strPmUntagged);
        LPM2STR(&operEntry.lpmUntag, strPmUntagged);
        SYS_MEM_CLEAR(untagPortBuf);
        untagLine = 0;
		untagLine = transLongStrToStrArray(strPmUntagged, untagPortBuf, 26);

        LOGIC_PORTMASK_CLEAR_ALL(lpmTagged);
        LOGIC_PORTMASK_ANDNOT(lpmTagged, operEntry.lpmMember, operEntry.lpmUntag);
        SYS_MEM_CLEAR(strPmTagged);
        LPM2STR(&lpmTagged, strPmTagged);
        SYS_MEM_CLEAR(tagPortBuf);
        tagLine = 0;
        tagLine = transLongStrToStrArray(strPmTagged, tagPortBuf, 26);

        maxLine = (tagLine > untagLine) ? tagLine : untagLine;
        maxLine = (maxLine > nameLine) ? maxLine : nameLine;
        
		add_value_to_new_line_printor_bylen(PRINT_DECIMAL, &vid, &printor, 5);
		add_value_to_last_line_printor_bylen(PRINT_STRING, nameList[0], &printor, NAME_LEN_PER_LINE+1);
		add_value_to_last_line_printor_bylen(PRINT_STRING, untagPortBuf[0], &printor, 28);
		add_value_to_last_line_printor_bylen(PRINT_STRING, tagPortBuf[0], &printor, 28);
		add_value_to_last_line_printor_bylen(PRINT_STRING, pVlanTypeStr[operEntry.type], &printor, 10);

        for (k = 1; k < maxLine; k++)
        {
            add_value_to_new_line_printor_bylen(PRINT_STRING, "", &printor, 5);
            if (k < nameLine)
            {
                add_value_to_last_line_printor_bylen(PRINT_STRING, nameList[k], &printor, NAME_LEN_PER_LINE + 1);
            }
            else
            {
                add_value_to_last_line_printor_bylen(PRINT_STRING, "", &printor, NAME_LEN_PER_LINE + 1);
            }

            if ((k < tagLine) || (k < untagLine))
            {
                add_value_to_last_line_printor_bylen(PRINT_STRING, untagPortBuf[k], &printor, 28);
                add_value_to_last_line_printor_bylen(PRINT_STRING, tagPortBuf[k], &printor, 28);
            }
            else
            {
                add_value_to_last_line_printor_bylen(PRINT_STRING, "", &printor, 28);
                add_value_to_last_line_printor_bylen(PRINT_STRING, "", &printor, 28);
            }
            add_value_to_last_line_printor_bylen(PRINT_STRING, "", &printor, 10);
        }
    }

	if (printor != NULL)
	{
		cmdPrintOutResult(printor, &cInputChar);
		free_printor_vector(&printor);
	}

    XMORE("\n");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MANAGEMENT_VLAN

int32 cmd_vlan_mgmt_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t mgmtVid;
    sys_namestr_t vlanName;

    SYS_ERR_CHK(sal_vlan_mgmtVid_get(&mgmtVid));
    SYS_ERR_CHK(sal_vlan_name_get(mgmtVid, &vlanName));
    XMORE("    Management VLAN-ID : %s(%u)\n", vlanName.str, mgmtVid);

    return SYS_ERR_OK;
}
#endif

int32 cmd_vlan_create_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t vlanId;
    sys_vid_t vid;
    sys_namestr_t name;
#ifdef CONFIG_SYS_L3_ROUTE
    sys_l3_intfId_t l3intf;
#endif
	int delay = 0;

    SYS_ERR_CHK(STR2VMSK(argv[0], &g_selVmsk));

    FOR_EACH_VID_IN_VLANMASK(vlanId, g_selVmsk)
    {
        vid = vlanId & 0xFFF;

        if (CMD_IS_NO_FORM_EXIST())
        {
#ifdef CONFIG_SYS_L3_ROUTE
            SYS_MEM_CLEAR(l3intf);
            l3intf.type = SYS_L3_INTFTYPE_VLAN;
            l3intf.id = vid;
            if (IS_L3_INTF_EXIST(&l3intf))
                CMD_ERR_CHK_VLAN(SYS_ERR_L3_INTF_EXIST, vid);
#endif

            CMD_ERR_CONTINUE_VLAN_NOTEXIST(vid);
            CMD_ERR_CONTINUE_VLAN(sal_vlan_adminEntry_destroy(vid), vid);

            SYS_ERR_CHK(sal_l2_dynamicMac_clearByVlan(vid));
        }
        else
        {
            if (TRUE == IS_STATIC_VLAN_EXIST(vid))
            {
                continue;
            }
            CMD_ERR_CONTINUE_VLAN(sal_vlan_nameStr_get(vid, DFLT_VLAN_PREFIX_NAME, &name), vid);
            CMD_ERR_CONTINUE_VLAN(sal_vlan_adminEntry_create(vid, &name), vid);
        }
        delay ++;
        if(delay % 10 == 0)
        {
			sal_msleep(10);
        }
    }

    if (!CMD_IS_NO_FORM_EXIST())
        vty->node = VLAN_DB_NODE;

    return SYS_ERR_OK;
}

int32 cmd_vlan_name_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 cnt = 0;
    sys_namestr_t origName;
    sys_namestr_t name;
    sys_vid_t vid;
    sys_vid_t defVid;

    VMSK2CNT(&g_selVmsk, &cnt);

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_vlan_defaultVid_get(&defVid));

        FOR_EACH_VID_IN_VLANMASK(vid, g_selVmsk)
        {
            SYS_MEM_CLEAR(name);
            if (vid == defVid)
                osal_strcpy(name.str, DFLT_VLAN_DFLT_NAME);
            else
                CMD_ERR_CONTINUE_VLAN(sal_vlan_nameStr_get(vid, DFLT_VLAN_PREFIX_NAME, &name), vid);

            sal_vlan_name_set(vid, &name);
        }
    }
    else
    {
        if (cnt > 1)
        {
            XMORE("Can't modify name for multiple VLANs\n");
            return SYS_ERR_OK;
        }

        FOR_EACH_VID_IN_VLANMASK(g_selVid, g_selVmsk)
        {
            break;
        }

        if (SYS_ERR_OK != STRING_VALIDATE(argv[0], FALSE))
            return SYS_ERR_STRING;

        if (SYS_ERR_STR_LEN_EXCEED == sys_util_namestrcpy(&name, argv[0]))
        {
            XMORE("Warning: vlan %d name length exceeds recommended maximum %d characters\n", g_selVid, CAPA_NAME_STR_LEN);
            return SYS_ERR_OK;
        }

        SYS_ERR_CHK(sal_vlan_name_get(g_selVid, &origName));
        if (osal_strcmp(name.str, origName.str))
        {
            if (SYS_ERR_OK == sal_vlan_vidByName_get(&name, &vid))
            {
                XMORE("%%VLAN #%u and #%u have an identical name: %s\n", g_selVid, vid, name.str);
                return SYS_ERR_OK;
            }
            else
            {
                SYS_ERR_CHK(sal_vlan_name_set(g_selVid, &name));
            }
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlan_portMode_t vlanMode;
    sys_logic_port_t logicPort;
    sys_enable_t uplink = DISABLED;
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    authmgr_cfg_port_t authPortCfg;
#endif

    if (CMD_IS_NO_FORM_EXIST())
    {
        vlanMode = DFLT_VLAN_PORT_MODE;
    }
    else
    {
        if ('a' == CMD_TOKENSTR_GET(cmd->vline,2)[0])
        {   
            vlanMode = SYS_VLAN_PORT_MODE_ACCESS;
        }
        else if ('t' == CMD_TOKENSTR_GET(cmd->vline,2)[0])
        {   
            if ('r' == CMD_TOKENSTR_GET(cmd->vline,2)[1])
            {   
                vlanMode = SYS_VLAN_PORT_MODE_TRUNK;
                if ((CMD_TOKENNUM_GET(cmd->vline) > 3) && ('u' == CMD_TOKENSTR_GET(cmd->vline,3)[0]))
                {
                    uplink = ENABLED;
                }
            }
            else
            {   
                vlanMode = SYS_VLAN_PORT_MODE_DOT1QTUNNEL;
            }
        }
        else
        {   
            vlanMode = SYS_VLAN_PORT_MODE_GENERAL;
        }
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
#ifdef CONFIG_SYS_PROTO_AUTHMGR
        uint32 i = 0;
        uint32 isAnyEnable = FALSE;

        SYS_MEM_CLEAR(authPortCfg);
        CMD_ERR_CONTINUE_PORT(sal_authmgr_portCfg_get(logicPort, &authPortCfg), logicPort);

        for (i = 0; i < AUTHMGR_TYPE_END; i++)
        {
            if (ENABLED == authPortCfg.typeEnbl[i])
            {
                isAnyEnable = TRUE;
                break;
            }
        }

        if ((AUTHMGR_PORTCTRL_DISABLE != authPortCfg.ctrlType) && isAnyEnable)
        {
            char __portStr[CAPA_PORT_STR_LEN];
            LP2STR(logicPort, __portStr);
            XMORE("Port %s: 802.1x/Mac-auth/Web-auth must be disabled before changing port mode.\n", __portStr);
            continue;
        }
#endif
        CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminMode_set(logicPort, vlanMode), logicPort);
        if (SYS_VLAN_PORT_MODE_TRUNK == vlanMode)
        {
            CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminRoleUplinkEnable_set(logicPort, uplink), logicPort);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portPvid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 pvid;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        pvid = DFLT_VLAN_PVID;
    else
        pvid = SYS_STR2UINT(argv[0]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_VLAN_PORTMODE(SYS_VLAN_PORT_MODE_GENERAL, logicPort);

        CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminPvid_set(logicPort, pvid), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portIgrFilterEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_VLAN_PORTMODE(SYS_VLAN_PORT_MODE_GENERAL, logicPort);

        CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminIgrFilterEnable_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portAcceptFrameType_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_vlan_frameType_t type;

    if (CMD_IS_NO_FORM_EXIST())
    {
        type = DFLT_VLAN_ACCEPT_FRAME_TYPE;
    }
    else
    {
        switch (argv[0][0])
        {
            case 'a': type = ACCEPT_FRAME_TYPE_ALL;        break;
            case 't': type = ACCEPT_FRAME_TYPE_TAG_ONLY;   break;
            default : type = ACCEPT_FRAME_TYPE_UNTAG_ONLY; break;
        }
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_VLAN_PORTMODE(SYS_VLAN_PORT_MODE_GENERAL, logicPort);

        CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminAcceptFrameType_set(logicPort, type), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portGeneralVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_vlanmask_t vlanmask;
    sys_vlan_membership_t vlanMembership;
	sys_vid_t vlanId;

    SYS_MEM_CLEAR(vlanmask);

    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    if ('a' == CMD_TOKENSTR_GET(cmd->vline, 4)[0])
    {   
        if (2 == argc)
        {   
            if ('t' == argv[1][0])
                vlanMembership = SYS_VLAN_TAGGED;
            else
                vlanMembership = SYS_VLAN_UNTAGGED;
        }
        else
        {   
            vlanMembership = SYS_VLAN_TAGGED;
        }
        
        FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
		{
			if (FALSE == sal_vlan_adminEntryExist_ret(vlanId))
			{
				VLANMASK_CLEAR_VID(vlanmask, vlanId);
				SYS_PRINTF("VLAN %d: %s\n", vlanId, SYS_ERR_STR(SYS_ERR_VLAN_NOT_EXIST));
				continue;
			}	
		}
    }
    else
    {   
        vlanMembership = SYS_VLAN_EXCLUDED;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_VLAN_PORTMODE(SYS_VLAN_PORT_MODE_GENERAL, lp);

        CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminVlanmask_add(lp, vlanMembership, &vlanmask), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portAccessVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_vid_t vid;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_vlan_defaultVid_get(&vid));
    }
    else
    {
        vid = SYS_STR2UINT(argv[0]);
    }

    if (!IS_STATIC_VLAN_EXIST(vid))
        return SYS_ERR_VLAN_WAS_NOT_CREATED_BY_USER;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_VLAN_PORTMODE(SYS_VLAN_PORT_MODE_ACCESS, lp);
        CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminPvid_set(lp, vid), lp);
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_VLAN_QINQ

int32 cmd_vlan_portDot1qtunnelVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp = 0;
    sys_vid_t vid = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        vid = DFLT_VLAN_RESERVED_VLAN;
    }
    else
    {
        vid = SYS_STR2UINT(argv[0]);
    }

    if (!IS_STATIC_VLAN_EXIST(vid))
        return SYS_ERR_VLAN_WAS_NOT_CREATED_BY_USER;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_VLAN_PORTMODE(SYS_VLAN_PORT_MODE_DOT1QTUNNEL, lp);
        CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminPvid_set(lp, vid), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portDot1qTunnelTpid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port = 0;
    uint32 tpid;
    sys_enable_t uplink;

    tpid = SYS_STR2UINT(argv[0]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        
        SYS_ERR_CHK(sal_vlan_portAdminRoleUplinkEnable_get(port, &uplink));
        CMD_ERR_CONTINUE_PORT((ENABLED == uplink)? SYS_ERR_OK : SYS_ERR_VLAN_PORTMODE_NOT_UPLINK, port);
        SYS_ERR_CHK(sal_vlan_portDot1qTunnelTpid_set(port, tpid));
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_vlan_portTrunkVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 isNative = FALSE;
    uint32 isAdd = FALSE;
    sys_logic_port_t lp = 0;
    sys_vid_t vid = 0;
    sys_vid_t vlanId;
    
    sys_vlanmask_t vlanmask;
    sys_vlanmask_t vmAllowed;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_vlan_defaultVid_get(&vid));

        isNative = TRUE;
    }
    
    else if (1 == argc)
    {
        vid = SYS_STR2UINT(argv[0]);

        isNative = TRUE;
    }
    
    else
    {
        isAdd = ('a' == argv[0][0])? TRUE : FALSE;

        SYS_MEM_CLEAR(vlanmask);
        if ('a' == argv[1][0])
        {
            VLANMASK_SET_ALL(vlanmask);
        }
        else
        {
            SYS_ERR_CHK(STR2VMSK(argv[1], &vlanmask));
        }

        FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
		{
			if (FALSE == sal_vlan_adminEntryExist_ret(vlanId))
			{
				VLANMASK_CLEAR_VID(vlanmask, vlanId);
				SYS_PRINTF("VLAN %d: %s\n", vlanId, SYS_ERR_STR(SYS_ERR_VLAN_NOT_EXIST));
				continue;
			}	
		}
    }

    if (TRUE == isNative)
    {
        
        if (!IS_STATIC_VLAN_EXIST(vid))
            return SYS_ERR_VLAN_WAS_NOT_CREATED_BY_USER;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            CMD_ERR_CONTINUE_VLAN_PORTMODE(SYS_VLAN_PORT_MODE_TRUNK, lp);
            CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminPvid_set(lp, vid), lp);
        }
    }
    else
    {
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            CMD_ERR_CONTINUE_VLAN_PORTMODE(SYS_VLAN_PORT_MODE_TRUNK, lp);

            CMD_ERR_CONTINUE_PORT(sal_vlan_portTrunkAllowedVlan_get(lp, &vmAllowed),lp);
            if (isAdd)
            {
                VLANMASK_OR(vmAllowed, vmAllowed, vlanmask);
            }
            else
            {
                VLANMASK_ANDNOT(vmAllowed, vmAllowed, vlanmask);
            }
            CMD_ERR_CONTINUE_PORT(sal_vlan_portTrunkAllowedVlan_set(lp, &vmAllowed),lp);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portDfltVlanTagged_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_vid_t vid;
    sys_vlan_membership_t vlanMembership;

    SYS_ERR_CHK(sal_vlan_defaultVid_get(&vid));

    if (CMD_IS_NO_FORM_EXIST())
        vlanMembership = SYS_VLAN_UNTAGGED;
    else
        vlanMembership = SYS_VLAN_TAGGED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_VLAN_PORTMODE(SYS_VLAN_PORT_MODE_GENERAL, lp);

        CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminVlanMembership_set(lp, vid, vlanMembership), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portForbidDfltVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t defaultVid;
    sys_logic_port_t lp;
    sys_vlan_membership_t oldMembership;
    sys_vlan_portMode_t portMode;
    sys_vid_t pvid;

    SYS_ERR_CHK(sal_vlan_defaultVid_get(&defaultVid));

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminVlanMembership_get(lp, defaultVid, &oldMembership), lp);

        if (CMD_IS_NO_FORM_EXIST())
        {
            if (SYS_VLAN_FORBIDDEN == oldMembership)
            {
                CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminMode_get(lp, &portMode), lp);
                CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminPvid_get(lp, &pvid), lp);
                if ((SYS_VLAN_PORT_MODE_GENERAL != portMode) && (DFLT_VLAN_RESERVED_VLAN == pvid))
                {
                    CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminVlanMembership_set(lp, defaultVid, SYS_VLAN_UNTAGGED), lp);
                }
                else
                {
                    CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminVlanMembership_set(lp, defaultVid, SYS_VLAN_EXCLUDED), lp);
                }
            }
        }
        else
        {
            
            CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminVlanMembership_set(lp, defaultVid, SYS_VLAN_FORBIDDEN), lp);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portForbidVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlanmask_t vlanmask;
    sys_logic_port_t lp = 0;

    SYS_MEM_CLEAR(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[1], &vlanmask));

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        if ('a' == argv[0][0])
        {
            CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminVlanmask_add(lp, SYS_VLAN_FORBIDDEN, &vlanmask), lp);
        }
        else
        {
            CMD_ERR_CONTINUE_PORT(sal_vlan_portAdminVlanmask_del(lp, SYS_VLAN_FORBIDDEN, &vlanmask), lp);
        }
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MANAGEMENT_VLAN

int32 cmd_vlan_mgmVid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t mgmtVid;

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (SYS_ERR_OK != sal_vlan_defaultVid_get(&mgmtVid))
        {
            mgmtVid = DFLT_VLAN_MGMT_VLAN;
        }
        SYS_ERR_CHK(sal_vlan_mgmtVid_set(mgmtVid));

        return SYS_ERR_OK;
    }

    mgmtVid = SYS_STR2UINT(argv[0]);
    CMD_ERR_CHK_VLAN_NOTEXIST(mgmtVid);
    SYS_ERR_CHK(sal_vlan_mgmtVid_set(mgmtVid));

    return SYS_ERR_OK;
} 

 #endif 

static int32 _cmd_vlan_database_save(FILE *fp)
{
    char *pVlanStrPrefix = DFLT_VLAN_PREFIX_NAME;
    uint32 i;
    sys_vlanmask_t vmStatic;
    char cmd[MAX_CMD_STRLEN];
    char strVlanList[CAPA_VLANLIST_STR_LEN];
    sys_vid_t entryVid;
    sys_vlan_adminEntry_t adminEntry;
    sys_vid_t defaultVid = DFLT_VLAN_DFLT_VLAN;
    sys_vlanmask_t vm;
    sys_namestr_t name;

    SYS_ERR_CHK(sal_vlan_defaultVid_get(&defaultVid));
    if (DFLT_VLAN_DFLT_VLAN != defaultVid)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "vlan default-vlan %u\n", defaultVid);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    VLANMASK_CLEAR_ALL(vmStatic);
    VLANMASK_CLEAR_ALL(vm);

    FOR_EACH_VLAN_STATIC_ENTRY_IDX(i)
    {
        SYS_MEM_CLEAR(adminEntry);
        SYS_MEM_CLEAR(name);

        SYS_ERR_CHK(sal_vlan_adminEntrySort_get(i, &entryVid, &adminEntry));
        SYS_ERR_CHK(sal_vlan_nameStr_get(entryVid, pVlanStrPrefix, &name));

        if (entryVid == defaultVid && 0 == osal_strcmp(adminEntry.name.str, DFLT_VLAN_DFLT_NAME))
            continue;

        if (0 == osal_strcmp(name.str, adminEntry.name.str))
        {
            VLANMASK_SET_VID(vmStatic, entryVid);
        }
        else
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "vlan %u\n", entryVid);
            CMD_WRITE_TO_FILE(fp, cmd);

            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "name \"%s\"\n", adminEntry.name.str);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    if (!IS_VLANMASK_CLEAR(vmStatic))
    {
        SYS_MEM_CLEAR(strVlanList);

        SYS_ERR_CHK(VMSK2STR(&vmStatic, strVlanList));

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "vlan %s\n", strVlanList);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

#if defined(CONFIG_SYS_MANAGEMENT_VLAN) && !defined(CONFIG_SYS_L3_ARCHITECTURE)
static int32 _cmd_vlan_mgmVid_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_vid_t mgmtVid;

    SYS_ERR_CHK(sal_vlan_mgmtVid_get(&mgmtVid));

    if (DFLT_VLAN_MGMT_VLAN != mgmtVid)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "management-vlan vlan %u\n", IS_VID_INVALID(mgmtVid) ? DFLT_VLAN_MGMT_VLAN : mgmtVid);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_vlan_save(FILE *fp)
{
    
    SYS_ERR_CHK(_cmd_vlan_database_save(fp));

#if defined(CONFIG_SYS_MANAGEMENT_VLAN) && !defined(CONFIG_SYS_L3_ARCHITECTURE)
    SYS_ERR_CHK(_cmd_vlan_mgmVid_save(fp));
#endif

    return SYS_ERR_OK;
}

static int32 _cmd_vlan_portSwitchPort_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    char strVlanList[CAPA_VLANLIST_STR_LEN];
    char *pStrVlanMode[] = {
        "hybrid",   
        "access",   
        "trunk",    
        "tunnel"    
        };
    char *pStrVlanFrameType[] = { "all",
                                  "tagged-only",
                                  "untagged-only" };
    sys_vid_t vid;
    sys_vid_t defaultVid;
    sys_enable_t enable;
    sys_vlanmask_t tmpVm;
    sys_vlan_portMode_t mode;
    sys_enable_t uplink;
    uint16 tpid;
    sys_vlan_frameType_t type;
    sys_vlan_portMember_t portVlanMbr;
    sys_vlan_adminEntry_t vlanEntry;
    sys_vlanmask_t vmAllowed;
    uint32 result;

    SYS_ERR_CHK(sal_vlan_defaultVid_get(&defaultVid));

    SYS_ERR_CHK(sal_vlan_portAdminMode_get(port, &mode));
    SYS_ERR_CHK(sal_vlan_portAdminRoleUplinkEnable_get(port, &uplink));

    if (DFLT_VLAN_PORT_MODE != mode)
    {
        SYS_MEM_CLEAR(cmd);

        if (SYS_VLAN_PORT_MODE_TRUNK == mode)
        {
            osal_sprintf(cmd, "switchport mode trunk%s\n", (ENABLED == uplink)? " uplink" : "");
        }
        else
        {
            osal_sprintf(cmd, "switchport mode %s\n", pStrVlanMode[mode]);
        }

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
    else
    {
        
        if ((SYS_VLAN_PORT_MODE_TRUNK == mode) && \
            (DFLT_VLAN_TRUNK_UPLINK != uplink))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "switchport mode trunk%s\n", (ENABLED == uplink)? " uplink" : "");
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    if (ENABLED == uplink)
    {
        SYS_ERR_CHK(sal_vlan_portDot1qTunnelTpid_get(port, &tpid));

        if (DFLT_VLAN_PORT_TPID != tpid)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "switchport vlan tpid 0x%04x\n", tpid);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    SYS_MEM_CLEAR(portVlanMbr);
    SYS_ERR_CHK(sal_vlan_portAdminVlanMember_get(port, &portVlanMbr));

    if (SYS_VLAN_PORT_MODE_GENERAL == mode)
    {
        
        SYS_ERR_CHK(sal_vlan_portAdminPvid_get(port, &vid));

        if (DFLT_VLAN_PVID != vid)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "switchport hybrid pvid %d\n", vid);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        SYS_ERR_CHK(sal_vlan_portAdminIgrFilterEnable_get(port, &enable));

        if (DFLT_VLAN_IGR_FILTER != enable)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "%sswitchport hybrid ingress-filtering\n", (DISABLED == enable) ? "no " : "");

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        SYS_ERR_CHK(sal_vlan_portAdminAcceptFrameType_get(port, &type));

        if (DFLT_VLAN_ACCEPT_FRAME_TYPE != type)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "switchport hybrid acceptable-frame-type %s\n", pStrVlanFrameType[type]);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        VLANMASK_CLEAR_ALL(tmpVm);
        VLANMASK_ANDNOT(tmpVm, portVlanMbr.vmMember, portVlanMbr.vmUntag);
        if (!IS_VLANMASK_CLEAR(tmpVm))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(strVlanList);

            SYS_ERR_CHK(VMSK2STR(&tmpVm, strVlanList));

            osal_sprintf(cmd, "switchport hybrid allowed vlan add %s tagged\n", strVlanList);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        VLANMASK_CLEAR_ALL(tmpVm);
        VLANMASK_COPY(tmpVm, portVlanMbr.vmUntag);
        
        VLANMASK_CLEAR_VID(tmpVm, defaultVid);
        if (!IS_VLANMASK_CLEAR(tmpVm))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(strVlanList);

            SYS_ERR_CHK(VMSK2STR(&tmpVm, strVlanList));

            osal_sprintf(cmd, "switchport hybrid allowed vlan add %s untagged\n", strVlanList);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        VLANMASK_CLEAR_ALL(tmpVm);
        VLANMASK_SET_VID(tmpVm, defaultVid);
        VLANMASK_ANDNOT(tmpVm, tmpVm, portVlanMbr.vmMember);
        if (!IS_VLANMASK_CLEAR(tmpVm))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(strVlanList);

            SYS_ERR_CHK(VMSK2STR(&tmpVm, strVlanList));

            osal_sprintf(cmd, "switchport hybrid allowed vlan remove %s\n", strVlanList);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }
    else if (SYS_VLAN_PORT_MODE_ACCESS == mode)
    {
        VLANMASK_CLEAR_ALL(tmpVm);
        VLANMASK_COPY(tmpVm, portVlanMbr.vmUntag);

        VLANMASK_CLEAR_VID(tmpVm, defaultVid);

        if (!IS_VLANMASK_CLEAR(tmpVm))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(strVlanList);

            FOR_EACH_VID_IN_VLANMASK(vid, tmpVm)
            {
                break;
            }

            osal_sprintf(cmd, "switchport access vlan %d\n", vid);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }
    else if (SYS_VLAN_PORT_MODE_TRUNK == mode)
    {
        
        SYS_ERR_CHK(sal_vlan_portAdminPvid_get(port, &vid));
        if (defaultVid != vid)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "switchport trunk native vlan %d\n", vid);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        SYS_ERR_CHK(sal_vlan_portTrunkAllowedVlan_get(port, &vmAllowed));

        SYS_ERR_CHK(STR2VMSK(DFLT_VLAN_TRUNK_ALLOW_VLAN, &tmpVm));
        VLANMASK_ANDNOT(tmpVm, vmAllowed, tmpVm);
        VLANMASK_IS_CLEAR(tmpVm, result);
        if (!result)
        {
            SYS_MEM_CLEAR(cmd);
            SYS_ERR_CHK(VMSK2STR(&tmpVm, strVlanList));
            osal_sprintf(cmd, "switchport trunk allowed vlan add %s\n", strVlanList);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        SYS_ERR_CHK(STR2VMSK(DFLT_VLAN_TRUNK_ALLOW_VLAN, &tmpVm));
        VLANMASK_ANDNOT(tmpVm, tmpVm, vmAllowed);
        VLANMASK_IS_CLEAR(tmpVm, result);
        if (!result)
        {
            SYS_MEM_CLEAR(cmd);
            SYS_ERR_CHK(VMSK2STR(&tmpVm, strVlanList));
            osal_sprintf(cmd, "switchport trunk allowed vlan remove %s\n", strVlanList);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }
    else if (SYS_VLAN_PORT_MODE_DOT1QTUNNEL == mode)
    {
        VLANMASK_CLEAR_ALL(tmpVm);
        VLANMASK_COPY(tmpVm, portVlanMbr.vmUntag);

        if (!IS_VLANMASK_CLEAR(tmpVm))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(strVlanList);

            FOR_EACH_VID_IN_VLANMASK(vid, tmpVm)
            {
                break;
            }

            osal_sprintf(cmd, "switchport tunnel vlan %d\n", vid);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    if (!IS_VLANMASK_CLEAR(portVlanMbr.vmForbidden))
    {
        SYS_MEM_CLEAR(cmd);
        SYS_MEM_CLEAR(strVlanList);

        SYS_ERR_CHK(VMSK2STR(&portVlanMbr.vmForbidden, strVlanList));

        osal_sprintf(cmd, "switchport forbidden vlan add %s\n", strVlanList);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if ((SYS_VLAN_PORT_MODE_ACCESS != mode) && \
        (SYS_VLAN_PORT_MODE_DOT1QTUNNEL != mode))
    {
        SYS_MEM_CLEAR(vlanEntry);
        SYS_ERR_CHK(sal_vlan_adminEntry_get(defaultVid, &vlanEntry));

        if (IS_LOGIC_PORTMASK_PORTCLEAR(vlanEntry.lpmUntag,port)
                && IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmMember,port))
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "switchport default-vlan tagged\n");

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_port_save(FILE *fp, sys_logic_port_t port)
{
    SYS_ERR_CHK(_cmd_vlan_portSwitchPort_save(fp, port));

    return SYS_ERR_OK;
}

