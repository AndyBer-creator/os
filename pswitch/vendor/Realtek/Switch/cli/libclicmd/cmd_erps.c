
#include <libsal/sal_erps.h>
#include <libcmd/cmd.h>

static uint32 g_ErpsInstId = 0;

int32 cmd_erps_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_erps_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_erps_inst_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 inst_id = 0;

    SYS_PARAM_CHK((NULL == argv[0]), SYS_ERR_NULL_POINTER);

	inst_id = SYS_STR2UINT(argv[0]);

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_erps_inst_del(inst_id));
		g_ErpsInstId = 0;
    }
    else
    {
        SYS_ERR_CHK(sal_erps_inst_create(inst_id));
		g_ErpsInstId = inst_id;
		vty->node = ERPS_INST_NODE;
    }

    return SYS_ERR_OK;
}

int32 cmd_erps_ctrlVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 ctrlVlanId;

    if (CMD_IS_NO_FORM_EXIST())
	{
        ctrlVlanId = SAL_ERPS_INVALID_CTRLVLAN_ID;
	}
    else
	{
		ctrlVlanId = SYS_STR2UINT(argv[0]);
	}

	SYS_ERR_CHK(sal_erps_ctrlVlan_set(g_ErpsInstId, ctrlVlanId));

    return SYS_ERR_OK;
}

int32 cmd_erps_wtrTimer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 time;

    if (CMD_IS_NO_FORM_EXIST())
	{
        time = SAL_ERPS_DFT_WTR_TIMEOUT;
	}
    else
	{
		time = SYS_STR2UINT(argv[0]);
	}

    SYS_ERR_CHK(sal_erps_wtrTimer_set(g_ErpsInstId, time));

    return SYS_ERR_OK;
}

int32 cmd_erps_guardTimer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 time;

    if (CMD_IS_NO_FORM_EXIST())
	{
        time = SAL_ERPS_DFT_GUARD_TIMEOUT;
	}
    else
	{
		time = SYS_STR2UINT(argv[0]);
	}

    SYS_ERR_CHK(sal_erps_guardTimer_set(g_ErpsInstId, time));

    return SYS_ERR_OK;
}

int32 cmd_erps_workMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 workMode;

    if (CMD_IS_NO_FORM_EXIST())
    {
        workMode = SAL_ERPS_MODE_REVERTIVE;
    }
    else
    {
        if ('r' == argv[0][0])
            workMode = SAL_ERPS_MODE_REVERTIVE; 
        else
            workMode = SAL_ERPS_MODE_NON_REVERTIVE; 
    }

    SYS_ERR_CHK(sal_erps_workMode_set(g_ErpsInstId, workMode));

    return SYS_ERR_OK;
}

int32 cmd_erps_ringId_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 ringId;

	ringId = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_erps_ringId_set(g_ErpsInstId, ringId));

    return SYS_ERR_OK;
}

int32 cmd_erps_ringLevel_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 ringLevel;

	ringLevel = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_erps_ringLevel_set(g_ErpsInstId, ringLevel));

    return SYS_ERR_OK;
}

int32 cmd_erps_ringPort_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isPort0 = TRUE;
	uint32 portId;
	uint32 portRole = SAL_ERPS_RPL_PORT;
	char *tmpStr = NULL;

    if (CMD_IS_NO_FORM_EXIST())
    {
		if ('1' == argv[0][4])
			isPort0 = FALSE;

		SYS_ERR_CHK(sal_erps_ringPort_del(g_ErpsInstId, isPort0));
    }
    else
    {
        tmpStr = CMD_TOKENSTR_GET(cmd->vline, 0);
        if(0 == osal_strcmp("port1", tmpStr))
        {
            isPort0 = FALSE;
        }

		SYS_ERR_CHK(STR2LP(argv[0], &portId));

		if (2 == argc)
		{
			if ('o' == argv[1][0])
			{
				portRole = SAL_ERPS_PORT_OWNER;
			}
			else if (('n' == argv[1][0]) && ('i' == argv[1][2]))
			{
				portRole = SAL_ERPS_PORT_NEIGHBOUR;
			}
			else if (('n' == argv[1][0]) && ('x' == argv[1][2]))
			{
				portRole = SAL_ERPS_PORT_NEXT_NEIGHBOUR;
			}
		}
		SYS_ERR_CHK(sal_erps_ringPort_add(g_ErpsInstId, (int32)portId, portRole, isPort0));
    }

	return SYS_ERR_OK;
}

int32 cmd_erps_ring_protected_instance_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 pg_inst;

    if (CMD_IS_NO_FORM_EXIST())
    {
    	pg_inst = SYS_STR2UINT(argv[0]);
		SYS_ERR_CHK(sal_erps_ring_protected_instance_del(g_ErpsInstId, pg_inst));
    }
    else
    {
		pg_inst = SYS_STR2UINT(argv[0]);

		SYS_ERR_CHK(sal_erps_ring_protected_instance_add(g_ErpsInstId, pg_inst));
    }

	return SYS_ERR_OK;
}

int32 cmd_erps_mel_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 mel;

	mel = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_erps_mel_set(g_ErpsInstId, mel));

    return SYS_ERR_OK;
}

int32 cmd_erps_ringState_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_enable_t state;

	if ('e' == argv[0][0])
		state = ENABLED;
	else
		state = DISABLED;
    SYS_ERR_CHK(sal_erps_ringState_set(g_ErpsInstId, state));

    return SYS_ERR_OK;
}

int32 cmd_show_erps(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	char	 *modeStr[]		= {"revertive", "non-revertive"};
	uint32   instId = 0;
	uint32   pCtrlVlanId = 0;
	sys_enable_t erpsEnable = DISABLED;
	uint32 wtrTime = 0;
	uint32 guardTime = 0;
	uint32 workMode = 0;
	uint32 ringId  = 0;
	uint32 ringLevel = 0;
	uint8  isPort0 = 0;
	int32 portId = 0;
	uint32 portRole = 0;				
	char   *portRoleStr[] = {"rpl", "owner", "neighbour", "next-neighbour"};
	char   *instRingStatusStr[] = {"init", "idle", "protection", "manual", "force", "pending"};
	char   *instRingPortStatusStr[] = {"disabled", "listening", "learning", "forwarding", "blocking"};

	uint32 mel = 0;
	uint32 ringState = 0;
	int totalCount = 0;
	uint32 bInstance = 0;
	char	 tmpStr[SYS_BUF64_LEN] = {0};
	int32    ringStateStatus = 0;
	int32    ringPortStateStatus = 0;
	uint32   gp_inst = 0;

	if ('a' == argv[0][0])   
	{
		cfg_erps_enable_get(&erpsEnable);	
		if(erpsEnable == ENABLED)
		{
			XMORE("  Erps status         : enable\n");
		}
		else
		{
			XMORE("  Erps status         : disable\n");
		}
		
		for(instId = 0; instId < MSTP_INSTANCE_NUM; instId++)
		{
			sal_erps_inst_get(instId, &bInstance);
			if(!bInstance)
			{
				continue;
			}
			XMORE("  Erps instance                      : %d\n", instId);
			SYS_ERR_CHK( sal_erps_ringState_get(instId, &ringState));
			XMORE("  Erps ring status                   :%s \n", ringState ? "enable":"disable");
			#if 0
			SYS_ERR_CHK( sal_erps_ring_protected_instance_get(instId, &gp_inst));
			XMORE("  Erps ring protected instance      :%u \n", gp_inst);
			#endif
			SYS_ERR_CHK( sal_erps_mel_get(instId, &mel));
			XMORE("  Erps mel                           :%u \n", mel);
			SYS_ERR_CHK(sal_erps_ctrlVlan_get(instId, &pCtrlVlanId));
			XMORE("  Erps control vlan                  : %u\n", pCtrlVlanId);
			SYS_ERR_CHK(sal_erps_wtrTimer_get(instId, &wtrTime));
			XMORE("  Erps WTR time                      : %u min\n", wtrTime);
			SYS_ERR_CHK( sal_erps_guardTimer_get(instId, &guardTime));
			XMORE("  Erps guard time                    : %u ms\n", guardTime);
			SYS_ERR_CHK( sal_erps_workMode_get(instId, &workMode));
			XMORE("  Erps work-mode                     :%s \n", modeStr[workMode]);
			SYS_ERR_CHK( sal_erps_ringId_get(instId, &ringId));
			XMORE("  Erps ring ID                       :%u \n", ringId);
			SYS_ERR_CHK( sal_erps_ringLevel_get(instId, &ringLevel));
			XMORE("  Erps ring-level                    :%u \n", ringLevel);

			SYS_ERR_CHK(sal_erps_ring_protected_instance_get(instId, &gp_inst));
			if(SAL_ERPS_PROTECT_INSTANCE != gp_inst)
			{
				XMORE("  Erps protected-instance            :%u \n", gp_inst);
			}
			else
			{
				XMORE("  Erps protected-instance            :N/A \n");
			}
			
			isPort0 = TRUE;
			SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));
			if(SAL_ERPS_RING_PORT_ID_DEF != portId)
			{
				SYS_ERR_CHK(sal_erps_get_inst_port_state_status(portId, &ringPortStateStatus));
				SYS_ERR_CHK(LP2STR(portId, tmpStr));
				XMORE("  Erps port0 portId:%s, port role  :%s, port status:%s\n", tmpStr, portRoleStr[portRole], instRingPortStatusStr[ringPortStateStatus]);
			}
			else
			{
				XMORE("  Erps port0 portId:N/A, port role  :N/A, port status:N/A\n");
			}
			isPort0 = FALSE;
			SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));
			if(SAL_ERPS_RING_PORT_ID_DEF != portId)
			{
				SYS_ERR_CHK(sal_erps_get_inst_port_state_status(portId, &ringPortStateStatus));
				SYS_ERR_CHK(LP2STR(portId, tmpStr));
				XMORE("  Erps port1 portId:%s, port role  :%s, port status:%s\n", tmpStr, portRoleStr[portRole], instRingPortStatusStr[ringPortStateStatus]);
			}
			else
			{
				XMORE("  Erps port1 portId:N/A, port role  :N/A, port status:N/A\n");
			}

			SYS_ERR_CHK(sal_erps_get_inst_state_status(instId, &ringStateStatus));
			XMORE("  Erps ring node state              :%s \n", instRingStatusStr[ringStateStatus]);
			XMORE("\n");
			++totalCount;	
		}
		XMORE("  \nTotal  ring(s). :%d \n", totalCount);
	}
	else  
	{		 
		instId = SYS_STR2UINT(argv[0]);
		sal_erps_inst_get(instId, &bInstance);
		if(!bInstance)
		{
			return SYS_ERR_OK;
		}
		XMORE("  Erps instance                      : %d\n", instId);
		SYS_ERR_CHK( sal_erps_ringState_get(instId, &ringState));
		XMORE("  Erps ring status                   :%s \n", ringState ? "enable":"disable");
		#if 0
		SYS_ERR_CHK( sal_erps_ring_protected_instance_get(instId, &gp_inst));
		XMORE("  Erps ring protected instance:%u \n", gp_inst);
		#endif
		SYS_ERR_CHK( sal_erps_mel_get(instId, &mel));
		XMORE("  Erps mel                           :%u \n", mel);
		SYS_ERR_CHK(sal_erps_ctrlVlan_get(instId, &pCtrlVlanId));
		XMORE("  Erps control vlan                  : %u\n", pCtrlVlanId);
		SYS_ERR_CHK(sal_erps_wtrTimer_get(instId, &wtrTime));
		XMORE("  Erps WTR time                      : %u min\n", wtrTime);
		SYS_ERR_CHK( sal_erps_guardTimer_get(instId, &guardTime));
		XMORE("  Erps guard time                    : %u ms\n", guardTime);
		SYS_ERR_CHK( sal_erps_workMode_get(instId, &workMode));
		XMORE("  Erps work-mode                     :%s \n", modeStr[workMode]);
		SYS_ERR_CHK( sal_erps_ringId_get(instId, &ringId));
		XMORE("  Erps ring ID                       :%u \n", ringId);
		SYS_ERR_CHK( sal_erps_ringLevel_get(instId, &ringLevel));
		XMORE("  Erps ring-level                    :%u \n", ringLevel);

		SYS_ERR_CHK(sal_erps_ring_protected_instance_get(instId, &gp_inst));
		if(SAL_ERPS_PROTECT_INSTANCE != gp_inst)
		{
			XMORE("  Erps protected-instance            :%u \n", gp_inst);
		}
		else
		{
			XMORE("  Erps protected-instance            :N/A \n");
		}
		
		isPort0 = TRUE;
		SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));	
		if(SAL_ERPS_RING_PORT_ID_DEF != portId)
		{
			SYS_ERR_CHK(sal_erps_get_inst_port_state_status(portId, &ringPortStateStatus));
			SYS_ERR_CHK(LP2STR(portId, tmpStr));
			XMORE("  Erps port0 portId:%s, port role  :%s, port status:%s\n", tmpStr, portRoleStr[portRole], instRingPortStatusStr[ringPortStateStatus]);
		}
		else
		{
			XMORE("  Erps port0 portId:N/A, port role  :N/A, port status:N/A\n");
		}
		isPort0 = FALSE;
		SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));
		if(SAL_ERPS_RING_PORT_ID_DEF != portId)
		{
			SYS_ERR_CHK(sal_erps_get_inst_port_state_status(portId, &ringPortStateStatus));
			SYS_ERR_CHK(LP2STR(portId, tmpStr));
			XMORE("  Erps port1 portId:%s, port role  :%s, port status:%s\n", tmpStr, portRoleStr[portRole], instRingPortStatusStr[ringPortStateStatus]);
		}
		else
		{
			XMORE("  Erps port1 portId:N/A, port role  :N/A, port status:N/A\n");
		}

		SYS_ERR_CHK(sal_erps_get_inst_state_status(instId, &ringStateStatus));
		XMORE("  Erps ring node state              :%s \n", instRingStatusStr[ringStateStatus]);
	}

	return SYS_ERR_OK;

}

int32 cmd_erps_save(FILE *fp)
{
	char     cmd[MAX_CMD_STRLEN];
	sys_enable_t erpsEnable = DISABLED;
	char     *portRoleStr[] = {"rpl", "owner", "neighbour", "next-neighbour"};
	uint32	 instId = 0;
	uint32   bInstance = 0;
	uint32	 pCtrlVlanId = 0;
	uint32   wtrTime = 0;
	uint32   guardTime = 0;
	uint32   workMode = 0;
	uint32   ringId  = 0;
	uint32   ringLevel = 0;
	uint8    isPort0 = 0;
	int32   portId = 0;
	uint32   portRole = 0;
	uint32	 gp_inst = 0;
	uint32   mel = 0;
	uint32   ringState = 0;
	char     tmpStr[SYS_BUF64_LEN] = {0};

	cfg_erps_enable_get(&erpsEnable);
	if(erpsEnable == ENABLED)
	{
		SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "erps\n");
        CMD_WRITE_TO_FILE(fp, cmd);
	}
	for(instId = 0; instId < MSTP_INSTANCE_NUM; instId++)
	{
		sal_erps_inst_get(instId, &bInstance);
		if(!bInstance)
		{
			continue;
		}
		SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "erps instance %u\n", instId);
        CMD_WRITE_TO_FILE(fp, cmd);
		SYS_ERR_CHK( sal_erps_mel_get(instId, &mel));
		if(SAL_ERPS_DFT_INST_MEL != mel)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "mel %u\n",mel);
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		SYS_ERR_CHK(sal_erps_wtrTimer_get(instId, &wtrTime));
		if(SAL_ERPS_DFT_WTR_TIMEOUT != wtrTime)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "wtr-timer %u\n", wtrTime);
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		SYS_ERR_CHK( sal_erps_guardTimer_get(instId, &guardTime));
		if(SAL_ERPS_DFT_GUARD_TIMEOUT != guardTime)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "guard-timer %u\n", guardTime);
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		SYS_ERR_CHK( sal_erps_workMode_get(instId, &workMode));
		if(SAL_ERPS_MODE_REVERTIVE != workMode)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "work-mode non_revertive\n");
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		SYS_ERR_CHK( sal_erps_ringId_get(instId, &ringId));
		if(SAL_ERPS_DFT_RING_ID != ringId)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "ring %u\n",ringId);
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		SYS_ERR_CHK( sal_erps_ringLevel_get(instId, &ringLevel));
		if(SAL_ERPS_RING_LEVEL_DEF != ringLevel)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "ring-level %u\n",ringLevel);
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		isPort0 = TRUE;
		SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));
		if(SAL_ERPS_RING_PORT_ID_DEF != portId)
		{
			SYS_MEM_CLEAR(cmd);
			SYS_ERR_CHK(LP2STR(portId, tmpStr));
			osal_sprintf(cmd, "port0 %s %s\n", tmpStr, (portRole ? portRoleStr[portRole]:""));
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		isPort0 = FALSE;
		SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));
		if(SAL_ERPS_RING_PORT_ID_DEF != portId)
		{
			SYS_MEM_CLEAR(cmd);
			SYS_ERR_CHK(LP2STR(portId, tmpStr));
			osal_sprintf(cmd, "port1 %s %s\n", tmpStr, (portRole ? portRoleStr[portRole]:""));
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		SYS_ERR_CHK( sal_erps_ring_protected_instance_get(instId, &gp_inst));
		if(SAL_ERPS_PROTECT_INSTANCE != gp_inst)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "protected-instance %u\n",gp_inst);
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		SYS_ERR_CHK(sal_erps_ctrlVlan_get(instId, &pCtrlVlanId));
		if(SAL_ERPS_INVALID_CTRLVLAN_ID != pCtrlVlanId)
		{
			SYS_MEM_CLEAR(cmd);
	        osal_sprintf(cmd, "control-vlan %u\n", pCtrlVlanId);
	        CMD_WRITE_TO_FILE(fp, cmd);
		}
		SYS_ERR_CHK( sal_erps_ringState_get(instId, &ringState));
		if(DISABLED != ringState)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "ring enable\n");
			CMD_WRITE_TO_FILE(fp, cmd);
		}
	}
    return SYS_ERR_OK;
}

