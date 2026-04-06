
#include <libsal/sal_l3_route_bgp.h>
#include <libcmd/cmd.h>

static int32 cmd_str2L3IPv4(char *str, sys_l3_ipv4_t *pIp)
{
    char *buf[2];
    char tmp[SYS_BUF64_LEN];

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(tmp);

    osal_strcpy(tmp, str);

    buf[0] = strtok(tmp, "/");
    buf[1] = strtok(NULL, "/");

    SYS_ERR_CHK(STR2IPV4(buf[0], &pIp->addr));
    pIp->plen = SYS_STR2UINT(buf[1]);

    return SYS_ERR_OK;
}

#if 0
static int32 _cmd_l3_str2L3IPv6(char *str, sys_l3_ipv6_t *pIp)
{
    char *buf[2];
    char tmp[SYS_BUF128_LEN];

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(tmp);

    osal_strcpy(tmp, str);

    buf[0] = strtok(tmp, "/");
    buf[1] = strtok(NULL, "/");

    SYS_ERR_CHK(STR2IPV6(buf[0], &pIp->addr));
    pIp->plen = SYS_STR2UINT(buf[1]);

    return SYS_ERR_OK;
}
#endif

int32 cmd_l3_router_bgp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	uint32 as_number = 0;
	
	as_number = SYS_STR2UINT(argv[0]);

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_routeV4_bgp_cloce(as_number);
	}
	else
	{
		rc = sal_l3_routeV4_bgp_open(as_number);
		vty->node = BGP_NODE;
	}
    return rc;
}

int32 cmd_l3_bgp_network_area_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	sys_l3_ipv4_t dstIp;
	sys_ipv4_t addr;
	sys_ipv4_t mask;
	uint32  areaId;

	cmd_str2L3IPv4(argv[0], &dstIp);
	addr = dstIp.addr;
	mask = IPV4_PFX2MASK(dstIp.plen);
	  
  	if (strchr (argv[1], '.') != NULL)
  	{
		STR2IPV4(argv[1], &areaId);
  	}
  	else
  	{
		areaId = SYS_STR2UINT(argv[1]);
  	}

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_bgp_network_area_set(DISABLED, addr, mask, areaId);
	}
	else
	{
		rc = sal_l3_bgp_network_area_set(ENABLED, addr, mask, areaId);
	}
    return rc;
}
#if 0

int32 cmd_rip_version(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	int32 ripVersion = 0;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_rip_version_rip_set(0, FALSE);
	}
	else
	{
		ripVersion = SYS_STR2UINT(argv[0]);
		if (ripVersion != 1 && ripVersion != 2)
		{
			return SYS_ERR_FAILED;
		}
		rc = sal_rip_version_rip_set(ripVersion, TRUE);
	}
	return rc;
}

int32 cmd_rip_route_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	
	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_rip_route_config(FALSE, argv[0]);
	}
	else
	{
		rc = sal_rip_route_config(TRUE, argv[0]);
	}
	return rc;
}

int32 cmd_rip_distance_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	
	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_rip_distance_config(FALSE, 0);
	}
	else
	{
		if((atoi(argv[0]) > 255) || (atoi(argv[0]) < 1))
		{
			return SYS_ERR_FAILED;
		}
		rc = sal_rip_distance_config(TRUE, atoi(argv[0]));
	}
	return rc;
}

int32 cmd_rip_distance_source_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;

	SYS_PARAM_CHK((argc < 2), SYS_ERR_FAILED);
	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_rip_distance_source_config(FALSE, argv[0], argv[1]);
	}
	else
	{
		if((atoi(argv[0]) > 255) || (atoi(argv[0]) < 1))
		{
			return SYS_ERR_FAILED;
		}
		rc = sal_rip_distance_source_config(TRUE, argv[0], argv[1]);
	}
	return rc;
}

int32 cmd_rip_distribute_list_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	sys_l3_intfId_t intfId;
	char devName[SYS_BUF16_LEN];

	SYS_PARAM_CHK((argc < 2), SYS_ERR_FAILED);

	SYS_MEM_CLEAR(intfId);
	SYS_MEM_CLEAR(devName);
	
	intfId.id = SYS_STR2UINT(argv[2]);
	intfId.type = SYS_L3_INTFTYPE_VLAN;

	if(!IS_L3_INTF_EXIST(&intfId))
	{
		return SYS_ERR_FAILED;
	}
	INTFID2STR(&intfId, devName);
	
	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_rip_distribute_list_config(FALSE, argv[0], argv[1], devName);
	}
	else
	{
		rc = sal_rip_distribute_list_config(TRUE, argv[0], argv[1], devName);
	}
	return rc;
}

int32 cmd_rip_access_list_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	char *prefix_str;
	int32 exact_match = 0;

	if (argc > 2)
	{
		prefix_str = argv[2];
	}
	else 
	{
		prefix_str = "0.0.0.0/0";
	}
	
	if (CMD_IS_NO_FORM_EXIST())
	{
		if (6 == CMD_TOKENNUM_GET(cmd->vline))
		{
			exact_match = 1;
		}
		rc = sal_rip_access_list_config(FALSE, AFI_IP, argv[0], argv[1], prefix_str, exact_match);
	}
	else
	{
		if (5 == CMD_TOKENNUM_GET(cmd->vline))
		{
			exact_match = 1;
		}
		rc = sal_rip_access_list_config(TRUE, AFI_IP, argv[0], argv[1], prefix_str, exact_match);
	}
	return rc;
}
#endif
