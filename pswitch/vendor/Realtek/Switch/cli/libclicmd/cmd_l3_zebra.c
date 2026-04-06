
#include <libsal/sal_l3_route_zebra.h>
#include <libcmd/cmd.h>

int32 cmd_l3_router_id_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char ipStr[SYS_BUF64_LEN];
	int rc = SYS_ERR_OK;

	SYS_MEM_CLEAR(ipStr);
	
    rc = sal_l3_route_id_get(ipStr);
    if(rc != 0) 
    {
		return rc;
    }
	
    XMORE("\n");
    if(osal_strlen(ipStr) == 0)
    {
		XMORE("No router id configured\n");
    }
    else
    {
    	XMORE("Current router id: %s\n", ipStr);
    }
    return SYS_ERR_OK;
}

int32 cmd_l3_router_id_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	char   szRouteId[SYS_BUF64_LEN];

	SYS_MEM_CLEAR(szRouteId);

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_route_id_del();
	}
	else
	{
		osal_sprintf(szRouteId, "%s", argv[0]);
		if ( 0 == osal_strcmp(szRouteId, "0.0.0.0") )
		{
			return SYS_ERR_IPV4_ADDR;
		}
		rc = sal_l3_route_id_add(szRouteId);
	}
    return rc;
}

int32 cmd_l3_router_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char ipStr[SYS_BUF64_LEN];
	int rc = SYS_ERR_OK;

	SYS_MEM_CLEAR(ipStr);
	
    rc = sal_l3_route_id_get(ipStr);
    if(rc != 0) 
    {
		return rc;
    }
    
    if(osal_strlen(ipStr) != 0)
    {
    	SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "router-id %s\n", ipStr);
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    return SYS_ERR_OK;

}

