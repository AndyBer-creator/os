
#include <libsal/sal_loopback.h>
#include <libcmd/cmd.h>

#define MAX_DETECTION_TIME 32767
#define MAX_RESUME_TIME    65535
#define MIN_RESUME_TIME    10

int32 cmd_loopback_proto_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_loopback_proto_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_loopback_detectionTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 timeValue = 0;
	
	timeValue = SYS_STR2UINT(argv[0]);
	SYS_PARAM_CHK(((timeValue < 0) || (timeValue > MAX_DETECTION_TIME)), SYS_ERR_OUT_OF_RANGE);
    SYS_ERR_CHK(sal_loopback_detectionTime_set(timeValue));

    return SYS_ERR_OK;
}

int32 cmd_loopback_resumeTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 timeValue = 0;
	
	timeValue = SYS_STR2UINT(argv[0]);
	SYS_PARAM_CHK(((timeValue < MIN_RESUME_TIME) || (timeValue > MAX_RESUME_TIME)), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(sal_loopback_resumeTime_set(timeValue));

    return SYS_ERR_OK;
}

int32 cmd_loopback_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 portId;
	uint32 isEnable = 0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		isEnable = 0;
		SYS_ERR_CHK(STR2LP(argv[0], &portId));
		SYS_ERR_CHK(sal_loopback_port_enable_set(portId,  isEnable));
		
	}
	else
	{
		isEnable = 1;
		SYS_ERR_CHK(STR2LP(argv[0], &portId));
		SYS_ERR_CHK(sal_loopback_port_enable_set(portId,  isEnable));
		
	}

    return SYS_ERR_OK;
}

int32 cmd_loopback_port_resume_mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 portId;
	uint32 reMode = SAL_LOOPBACK_AUTO;

	SYS_ERR_CHK(STR2LP(argv[0], &portId));

	if ('a' == argv[1][0])
	{
		reMode = SAL_LOOPBACK_AUTO;
	}
	else if ('m' == argv[1][0])
	{
		reMode = SAL_LOOPBACK_MANUAL;
	}
	SYS_ERR_CHK(sal_loopback_port_resume_mode_set(portId,  reMode));

	return SYS_ERR_OK;
}

int32 cmd_loopback_port_resume_manual_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 portId;

	SYS_ERR_CHK(STR2LP(argv[0], &portId));

	SYS_ERR_CHK(sal_loopback_port_resume_manual_set(portId,  1));

	return SYS_ERR_OK;
}

int32 cmd_show_loopback_port_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 portId;
	uint32 detecTime;
	uint32 resumeTime;
	uint32 isEnable;
	uint32 reMode;
	uint32 portState;
	sys_enable_t protoEnable;
	char	 tmpStr[SYS_BUF64_LEN] = {0};

	XMORE("  LoopBack gloabl Configs\n");
	
	XMORE("  -----------------------------------------------------------\n");

	SYS_ERR_CHK(sal_loopback_proto_enable_get(&protoEnable));
	if(protoEnable == DISABLED)
	{
		XMORE("  proto status              :disable \n");
	}
	else if(protoEnable == ENABLED)
	{
		
XMORE("  proto status              :enable \n");
	}	
	
	SYS_ERR_CHK(STR2LP(argv[0], &portId));
	SYS_ERR_CHK(LP2STR(portId, tmpStr));

	SYS_ERR_CHK(sal_loopback_detectionTime_get(&detecTime));
	XMORE("  Detection period          : %u\n", detecTime);

	SYS_ERR_CHK(sal_loopback_resumeTime_get(&resumeTime));
	XMORE("  Restore time              : %u\n", resumeTime);
	
	XMORE("  -----------------------------------------------------------\n");
	XMORE("  Port                      : %s\n", tmpStr);
	SYS_ERR_CHK(sal_loopback_port_enable_get(portId,  &isEnable));
	if(isEnable)
	{
		XMORE("  Enable                    : Enabled\n");
	}
	else
	{
		XMORE("  Enable                    : Disabled\n");
	}

	SYS_ERR_CHK(sal_loopback_port_status_state_get(portId,  &portState));
	if(SAL_LOOPBACK_Disabled == portState)
	{
		XMORE("  Port Status               : Disabled\n");
	}
	else if(SAL_LOOPBACK_Listening == portState)
	{
		XMORE("  Port Status               : Listening\n");
	}
	else if(SAL_LOOPBACK_Learning == portState)
	{
		XMORE("  Port Status               : Learning\n");
	}
	else if(SAL_LOOPBACK_Forwarding == portState)
	{
		XMORE("  Port Status               : Forwards\n");
	}
	else if(SAL_LOOPBACK_Blocking == portState)
	{
		XMORE("  Port Status               : Blocking\n");
	}
	
	SYS_ERR_CHK(sal_loopback_port_resume_mode_get(portId,  &reMode));
	if(SAL_LOOPBACK_AUTO == reMode)
	{
		XMORE("  Resume-mode               : Automation\n");
	}
	else if(SAL_LOOPBACK_MANUAL == reMode)
	{
		XMORE("  Resume-mode               : Manual\n");
	}

	XMORE("  -----------------------------------------------------------\n");

	XMORE("\n");

	return SYS_ERR_OK;
}

int32 cmd_loopback_save(FILE *fp)
{
	uint32 detecTime;
	uint32 resumeTime;
	uint32 isEnable;
	uint32 reMode;
	sys_enable_t protoEnable;
    sys_logic_port_t lp;
    
	char portStr[CAPA_PORT_STR_LEN];
	char     cmd[MAX_CMD_STRLEN];

	SYS_ERR_CHK(sal_loopback_proto_enable_get(&protoEnable));
	if(protoEnable == ENABLED)
	{
		SYS_MEM_CLEAR(cmd);
		osal_sprintf(cmd, "loopback\n");
		CMD_WRITE_TO_FILE(fp, cmd);
	}
	SYS_ERR_CHK(sal_loopback_detectionTime_get(&detecTime));
	if(SAL_DEFAULT_DETECTIONTIME != detecTime)
	{
		SYS_MEM_CLEAR(cmd);
		osal_sprintf(cmd, "loopback detection-time %u\n",detecTime);
		CMD_WRITE_TO_FILE(fp, cmd);
	}

	SYS_ERR_CHK(sal_loopback_resumeTime_get(&resumeTime));

	if(SAL_DEFAULT_RESUMETIME != resumeTime)
	{
		SYS_MEM_CLEAR(cmd);
		osal_sprintf(cmd, "loopback resume-time %u\n",resumeTime);
		CMD_WRITE_TO_FILE(fp, cmd);
	}
    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(portStr);
        LP2FULLSTR(lp, portStr);

		SYS_ERR_CHK(sal_loopback_port_enable_get(lp,  &isEnable));
		if(Enabled == isEnable)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "interface %s loopback enable\n",portStr);
			CMD_WRITE_TO_FILE(fp, cmd);
		}
		SYS_ERR_CHK(sal_loopback_port_resume_mode_get(lp,  &reMode));
		if(SAL_LOOPBACK_AUTO != reMode)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "interface %s loopback resume-mode manual\n", portStr);
			CMD_WRITE_TO_FILE(fp, cmd);
		}
    }

    return SYS_ERR_OK;
}

