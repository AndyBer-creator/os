
#include <libcmd/cmd.h>
#include <libsal/sal_easycwmp.h>

int32 cmd_easycwmp_device_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	vector printor = NULL;
    char   *ObjectN2Print[] =
    {
         "Manufacturer", 
         "OUI", 
         "Product Class", 
         "Serial Number", 
         "Software Version",
         "Hardware Version",
         "Model Name",
    };

	char  strValue[80];
	char  cInputChar = 0;

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_device_cfg_get(EASYCWMP_DEVICE_MANUFACTURER, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[0], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_device_cfg_get(EASYCWMP_DEVICE_OUI, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[1], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_device_cfg_get(EASYCWMP_DEVICE_PRODUCT_CLASS, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[2], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_device_cfg_get(EASYCWMP_DEVICE_SN, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[3], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_device_cfg_get(EASYCWMP_DEVICE_SOFTWAREVERSION, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[4], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);
	
	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_device_cfg_get(EASYCWMP_DEVICE_HARDWAREVERSION, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[5], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_device_cfg_get(EASYCWMP_DEVICE_MODENAME, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[6], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	if (printor != NULL)
	{
		cmdPrintOutResult(printor, &cInputChar);
		free_printor_vector(&printor);
	}
	XMORE("\n");
    return SYS_ERR_OK;
}

int32 cmd_easycwmp_acs_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	vector printor = NULL;
    char   *ObjectN2Print[] =
    {
         "ACS URL", 
         "Username", 
         "Password", 
         "Periodic Enable", 
         "Periodic Interval",
         "Periodic Time",
    };

	char  strValue[80];
	char  cInputChar = 0;

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_acs_cfg_get(EASYCWMP_ACS_URL, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[0], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_acs_cfg_get(EASYCWMP_ACS_USERNAME, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[1], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_acs_cfg_get(EASYCWMP_ACS_PASSWORD, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[2], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_acs_cfg_get(EASYCWMP_ACS_PERIODIC_ENABLE, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[3], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_acs_cfg_get(EASYCWMP_ACS_PERIODIC_INTERVAL, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[4], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);
	
	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_acs_cfg_get(EASYCWMP_ACS_PERIODIC_TIME, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[5], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	if (printor != NULL)
	{
		cmdPrintOutResult(printor, &cInputChar);
		free_printor_vector(&printor);
	}
	XMORE("\n");
    return SYS_ERR_OK;
}

int32 cmd_easycwmp_local_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	vector printor = NULL;
    char   *ObjectN2Print[] =
    {
         "Status", 
         "Interface", 
         "Port",
         "Username",
         "Password", 
         "Authentication", 
         "Logging level",
    };

	char  strValue[80];
	char  cInputChar = 0;
	char  strInterfece[80];

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_local_cfg_get(EASYCWMP_LOCAL_ENABLE, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[0], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	SYS_MEM_CLEAR(strInterfece);
	sal_easycwmp_local_cfg_get(EASYCWMP_LOCAL_INTERFACE, strValue);
	STR2INTFNAME(strValue, strInterfece);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[1], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strInterfece, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_local_cfg_get(EASYCWMP_LOCAL_PORT, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[2], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_local_cfg_get(EASYCWMP_LOCAL_USERNAME, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[3], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_local_cfg_get(EASYCWMP_LOCAL_PASSWORD, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[4], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);
	
	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_local_cfg_get(EASYCWMP_LOCAL_AUTH, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[5], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	SYS_MEM_CLEAR(strValue);
	sal_easycwmp_local_cfg_get(EASYCWMP_LOCAL_LOG_LEVEL, strValue);
	add_value_to_new_line_printor(PRINT_STRING, ObjectN2Print[6], &printor);
	add_value_to_last_line_printor(PRINT_STRING, strValue, &printor);

	if (printor != NULL)
	{
		cmdPrintOutResult(printor, &cInputChar);
		free_printor_vector(&printor);
	}
	XMORE("\n");
    return SYS_ERR_OK;
}

int32 cmd_easycwmp_node_mode(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	vty->node = CWMP_NODE;
	
    return SYS_ERR_OK;
}

int32 cmd_easycwmp_cpe_enable(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{

	SYS_ERR_CHK(sal_easycwmp_cpe_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED: ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_easycwmp_local_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_vid_t vid = 0;
	
	sys_l3_intfId_t intVlan;
	char devstr[40];
	int logLev = 0;
	int i;
	
	if(argc)
	{
		for(i=0; i< argc; i++)
		{
			printf("\r\n %s %d argc:%d argv:%s\n", __func__, __LINE__, i, argv[i]);
		}
	}
	
	if ('i' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
	{
		
		vid = SYS_STR2UINT(argv[0]);
		intVlan.id = vid;
		intVlan.type = SYS_L3_INTFTYPE_VLAN;
		SYS_MEM_CLEAR(devstr);
		INTFID2STR(&intVlan, devstr);
		SYS_ERR_CHK(sal_easycwmp_local_cfg_set(EASYCWMP_LOCAL_INTERFACE, devstr));
	}
	else if ('p' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
	{
		
		SYS_ERR_CHK(sal_easycwmp_local_cfg_set(EASYCWMP_LOCAL_PORT, argv[0]));
	}
	else if ('u' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
	{
		
		SYS_ERR_CHK(sal_easycwmp_local_cfg_set(EASYCWMP_LOCAL_USERNAME, argv[0]));
		SYS_ERR_CHK(sal_easycwmp_local_cfg_set(EASYCWMP_LOCAL_PASSWORD, argv[1]));
	}
	else if ('a' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
	{
		
		SYS_ERR_CHK(sal_easycwmp_local_cfg_set(EASYCWMP_LOCAL_AUTH, argv[0]));
	}
	else if ('l' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
	{

		if(osal_strcmp(argv[0], "critic") == 0)
		{
			logLev = 0;
		}
		else if(osal_strcmp(argv[0], "warning") == 0)
		{
			logLev = 1;

		}
		else if(osal_strcmp(argv[0], "notice") == 0)
		{
			logLev = 2;

		}
		else if(osal_strcmp(argv[0], "info") == 0)
		{
			logLev = 3;

		}
		else if(osal_strcmp(argv[0], "debug") == 0)
		{
			logLev = 4;
		}
		SYS_MEM_CLEAR(devstr);
		osal_sprintf(devstr, "%d", logLev);
		SYS_ERR_CHK(sal_easycwmp_local_cfg_set(EASYCWMP_LOCAL_LOG_LEVEL, devstr));
	}

    return SYS_ERR_OK;
}

int32 cmd_easycwmp_acs_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int i;
	
	if(argc)
	{
		for(i=0; i< argc; i++)
		{
			printf("\r\n %s %d argc:%d argv:%s\n", __func__, __LINE__, i, argv[i]);
		}
	}
	if (CMD_IS_NO_FORM_EXIST())
	{
		
	}
	else 
	{
		if ('u' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
		{
			SYS_ERR_CHK(sal_easycwmp_acs_cfg_set(EASYCWMP_ACS_URL, argv[0]));
		}
		else if ('p' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
		{
			if(argc > 0)
			{
				
				SYS_ERR_CHK(sal_easycwmp_acs_cfg_set(EASYCWMP_ACS_PERIODIC_INTERVAL, argv[0]));
			}
			else 
			{
				SYS_ERR_CHK(sal_easycwmp_acs_cfg_set(EASYCWMP_ACS_PERIODIC_ENABLE, "1"));
			}
		}
		else if ('u' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
		{
			
			SYS_ERR_CHK(sal_easycwmp_acs_cfg_set(EASYCWMP_ACS_USERNAME, argv[0]));
			SYS_ERR_CHK(sal_easycwmp_acs_cfg_set(EASYCWMP_ACS_PASSWORD, argv[1]));
		}
	}
    return SYS_ERR_OK;
}

