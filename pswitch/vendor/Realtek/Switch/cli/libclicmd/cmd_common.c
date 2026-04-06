
#include <libcmd/cmd.h>

uint32               g_isIntfRange = FALSE;
sys_logic_port_t     g_selLp;
sys_logic_portmask_t g_selLpm;
sys_vid_t            g_selVid;
sys_vlanmask_t       g_selVmsk;
#ifdef CONFIG_SYS_L3_ROUTE
uint32               g_selLo;
sys_l3_intfType_t    g_l3IntfType;
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
extern uint32 g_ospf_prosseId;
#endif

int32 cmd_do_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    return SYS_ERR_OK;
}

int32 cmd_exit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    switch (vty->node)
    {
        case CONFIG_NODE:
            vty->node = ENABLE_NODE;
            break;
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
        case AREA_NODE:
            vty->node = OSPF_NODE;
			SYS_MEM_CLEAR(vty->exPrompt);
			osal_sprintf(vty->exPrompt, "%d", g_ospf_prosseId);
            break;
#endif
        case IF_NODE:
        case IF_TRK_NODE:
#ifdef CONFIG_SYS_L3_ROUTE
        case IF_VLAN_NODE:
        case IF_LO_NODE:
#endif
        case VTY_NODE:
        default:
            vty->node = CONFIG_NODE;
            break;
    }

    return SYS_ERR_OK;
}

int32 cmd_intf_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp = 0;
	char *pTemp = NULL;
	char *pSpring = NULL;

    if (CMD_TOKENNUM_GET(cmd->vline) > 2)
        g_isIntfRange = TRUE;
    else
        g_isIntfRange = FALSE;
        
	pSpring = argv[0];

#ifdef CONFIG_SYS_L3_ROUTE
    
    if ('v' == tolower(argv[0][0]))
    {
        uint32 cnt = 0;
        sys_vid_t vid = 0;

        VLANMASK_CLEAR_ALL(g_selVmsk);

        SYS_ERR_CHK(STR2VMSK(argv[0], &g_selVmsk));

        if (FALSE == g_isIntfRange)
        {
            VMSK2CNT(&g_selVmsk, &cnt);

            if (cnt > 1)
                return SYS_ERR_VLAN_ID;

            FOR_EACH_VID_IN_VLANMASK(vid, g_selVmsk)
            {
                g_selVid = vid;
                break;
            }
        }

        g_l3IntfType = SYS_L3_INTFTYPE_VLAN;

        FOR_EACH_VID_IN_VLANMASK(vid, g_selVmsk)
        {
            if (FALSE == sal_vlan_operEntryExist_ret(vid))
            {
                int32 ret = SYS_ERR_FAILED;
                sys_namestr_t name;

                SYS_MEM_CLEAR(name);

                sal_vlan_nameStr_get(vid, DFLT_VLAN_PREFIX_NAME, &name);
                ret = sal_vlan_adminEntry_create(vid, &name);

                if (SYS_ERR_OK != ret)
                {
                    XMORE("Create VLAN %d failed: %s\n", vid, SYS_ERR_STR(ret));
                }
            }
            else
            {
            	#ifndef CONFIG_SYS_L3_ARCHITECTURE
				if (IS_MGMT_VID(vid))
            		return SYS_ERR_L3_VLAN_IS_MGMT_VLAN;
            	#endif
            }
        }

        vty->node = IF_VLAN_NODE;
    }
    
    else if ('l' == tolower(CMD_TOKENSTR_GET(cmd->vline, 1)[0])
                 && 'o' == tolower(CMD_TOKENSTR_GET(cmd->vline, 1)[1]))
    {
        sys_l3_intfId_t loIntfId;

        SYS_ERR_CHK(INTFNAME2ID(argv[0], &loIntfId));

        g_selLo = loIntfId.id;

        g_l3IntfType = SYS_L3_INTFTYPE_LO;

        vty->node = IF_LO_NODE;
    }
    
    else
#endif
    {
        LOGIC_PORTMASK_CLEAR_ALL(g_selLpm);

        if (TRUE == g_isIntfRange)
        {
            if (SYS_ERR_OK != STR2LPM(argv[0], &g_selLpm))
                return SYS_ERR_PORT_ID;

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
            {
                g_selLp = lp;

                break;
            }
        }
        else
        {
            if (SYS_ERR_OK != STR2LP(argv[0], &g_selLp))
                return SYS_ERR_PORT_ID;

            LOGIC_PORTMASK_SET_PORT(g_selLpm, g_selLp);
        }

        switch (LP2TYPE(g_selLp))
        {
            case PORT_TYPE_LAG:
                vty->node = IF_TRK_NODE;
                break;

            default:
                vty->node = IF_NODE;
                break;
        }
    }
	SYS_MEM_CLEAR(vty->exPrompt);
	pTemp = osal_strchr(pSpring, ' ');
	if (NULL != pTemp)
	{
		osal_memcpy(pTemp, pTemp+1, osal_strlen(pTemp));
		osal_sprintf(vty->exPrompt, "%s", pSpring);
	}

	osal_sprintf(vty->exPrompt, "%s", pSpring);

    return SYS_ERR_OK;
}

int32 cmd_intf_config_print(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char cmdStr[MAX_CMD_STRLEN];
    FILE *fp;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    if (NULL == (fp =  fopen(CONF_RUNNING_FILE, "w+")))
        return SYS_ERR_OK;

#ifdef CONFIG_SYS_L3_ROUTE
    
    if ('v' == tolower(argv[0][0]))
    {
        sys_vid_t vid;
        sys_vlanmask_t vmsk;

        VLANMASK_CLEAR_ALL(vmsk);

        STR2VMSK(argv[0], &vmsk);

        FOR_EACH_VID_IN_VLANMASK(vid, vmsk)
        {
            cmd_parser_intfVlanCmd_save(CAT_ID_END, vid, fp);
        }
    }
    
    else if ('l' == tolower(argv[0][0]) && (osal_strlen(argv[0]) > 1 && 'o' == tolower(argv[0][1])))
    {
        uint32 i = 0;

        for (i = 0; i < CAPA_L3_INTFLO_NUM; i++)
        {
            cmd_parser_intfLoCmd_save(CAT_ID_END, i, fp);
        }
    }
    else
#endif
    {
        LOGIC_PORTMASK_CLEAR_ALL(lpm);

        STR2LPM(argv[0], &lpm);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            cmd_parser_intfPortCmd_save(CAT_ID_END, lp, fp);
        }
    }

    rewind(fp);

    SYS_MEM_CLEAR(cmdStr);
    while (fgets(cmdStr, MAX_CMD_STRLEN, fp))
    {
        cmdStr[osal_strlen(cmdStr) - 1] = '\0';
        XMORE("%s\n", cmdStr);
        SYS_MEM_CLEAR(cmdStr);
    }

    fclose(fp);

    return SYS_ERR_OK;
}

