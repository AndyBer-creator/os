
#include <libcmd/cmd.h>

int32 cmd_vlan_ip_subnet_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_ipSubnet_vlan_group_t  protoGroup;
    uint32 groupIdx;
	char  cInputChar = 0;
	vector printor = NULL;
	int rc = SYS_ERR_OK;
	sys_ipv4_t  ipmask = 0;
	char strPort[CAPA_PORT_STR_LEN] = {0};
    char   *ObjectN2Print[] =
    {
         "GroupID", 
         "VlanID", 
         "IP-Address", 
         "IP-Mask", 
         "priority",
         "Bind-Port",
    };

    XMORE("\n");

	add_value_to_new_line_printor_bylen(PRINT_STRING, ObjectN2Print[0], &printor, 9);
	add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[1], &printor, 8);
	add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[2], &printor, 17);
	add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[3], &printor, 17);
	add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[4], &printor, 12);
	add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[5], &printor, 12);

    if (1 == argc)
    {
        groupIdx = SYS_STR2UINT(argv[0]);
		osal_memset(&protoGroup, 0, sizeof(sys_vlan_protoGroup_t));
        rc = sal_vlan_ip_subnet_group_get(groupIdx - 1, &protoGroup);
        if(( rc != SYS_ERR_OK)|| ((protoGroup.ipaddr.addr == 0) && (protoGroup.vid == 0)))
        {	
        	cmdPrintOutResult(printor, &cInputChar);
			return free_printor_vector(&printor);
        }
	        
		add_value_to_new_line_printor_bylen(PRINT_DECIMAL, &groupIdx, &printor, 9);
		add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &protoGroup.vid, &printor, 8);
		add_value_to_last_line_printor_bylen(PRINT_IP_ADDRESS, &protoGroup.ipaddr.addr, &printor, 17);
		ipmask = IPV4_PFX2MASK(protoGroup.ipaddr.plen);
		add_value_to_last_line_printor_bylen(PRINT_IP_ADDRESS, &ipmask, &printor, 17);
		add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &protoGroup.priority, &printor, 12);
		if(protoGroup.valid)
		{
			SYS_MEM_CLEAR(strPort);
			LP2STR(protoGroup.lport, strPort);
			add_value_to_last_line_printor_bylen(PRINT_STRING, strPort, &printor, 12);
		}
		else
		{
			SYS_MEM_CLEAR(strPort);
			osal_sprintf(strPort, "NA");
			add_value_to_last_line_printor_bylen(PRINT_STRING, strPort, &printor, 12);
		}
    }
    else
    {
        for (groupIdx = 1; groupIdx <= CAPA_VLAN_IP_SUBNET_GROUP_NUM; groupIdx++)
        {
        	osal_memset(&protoGroup, 0, sizeof(sys_vlan_protoGroup_t));
            rc = sal_vlan_ip_subnet_group_get(groupIdx - 1, &protoGroup);
            if( rc != SYS_ERR_OK)
	        {	
	        	cmdPrintOutResult(printor, &cInputChar);
				return free_printor_vector(&printor);
	        }
	        if((protoGroup.ipaddr.addr == 0) && (protoGroup.vid == 0))
	        	continue;
	        	
			add_value_to_new_line_printor_bylen(PRINT_DECIMAL, &groupIdx, &printor, 9);
			add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &protoGroup.vid, &printor, 8);
			add_value_to_last_line_printor_bylen(PRINT_IP_ADDRESS, &protoGroup.ipaddr.addr, &printor, 17);
			ipmask = IPV4_PFX2MASK(protoGroup.ipaddr.plen);
			add_value_to_last_line_printor_bylen(PRINT_IP_ADDRESS, &ipmask, &printor, 17);
			add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &protoGroup.priority, &printor, 12);
			if(protoGroup.valid)
			{
				SYS_MEM_CLEAR(strPort);
				LP2STR(protoGroup.lport, strPort);
				add_value_to_last_line_printor_bylen(PRINT_STRING, strPort, &printor, 12);
			}
			else
			{
				SYS_MEM_CLEAR(strPort);
				osal_sprintf(strPort, "NA");
				add_value_to_last_line_printor_bylen(PRINT_STRING, strPort, &printor, 12);
			}
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

int32 cmd_vlan_port_ip_subnet_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char strPort[CAPA_PORT_STR_LEN] = {0};
    sys_logic_port_t logicPort = 0;
    sys_logic_portmask_t logicPortmask;
    uint32 groupIdx = 0;
    uint32 bindgroupIdx = 0;
    sys_enable_t enable;
	char  cInputChar = 0;
	vector printor = NULL;
	int rc = SYS_ERR_OK;
	uint32 len = 0;
	char buf[SYS_BUF1024_LEN];
	
    char   *ObjectN2Print[] =
    {
         "Enable", 
         "Disable", 
    };

    char   *ObjectPrint[] =
    {
         "Port", 
         "Staus",
         "Bind Group",
         "\n"
    };

    osal_memset(&buf, 0, sizeof(buf));

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE("\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        LP2STR(logicPort, strPort);
		rc = sal_vlan_port_ipsubnet_vlan_status(logicPort, &enable);
        if( rc != SYS_ERR_OK)
        {	
        	cmdPrintOutResult(printor, &cInputChar);
			return free_printor_vector(&printor);
        }
        
        add_value_to_new_line_printor(PRINT_STRING, ObjectPrint[0], &printor);
        add_value_to_last_line_printor(PRINT_STRING, strPort, &printor);
        
        add_value_to_new_line_printor(PRINT_STRING, ObjectPrint[1], &printor);
        if(enable)
        {
			add_value_to_last_line_printor(PRINT_STRING, ObjectN2Print[0], &printor);
		}
		else
		{
			add_value_to_last_line_printor(PRINT_STRING, ObjectN2Print[1], &printor);
		}

		osal_memset(&buf, 0, sizeof(buf));
		len = 0;
		for (groupIdx = 0; groupIdx < CAPA_VLAN_IP_SUBNET_GROUP_NUM; groupIdx++)
		{
			rc = sal_vlan_port_ipsubnet_vlan_group_bind_get(logicPort, &groupIdx);
			if(rc == SYS_ERR_ENTRY_NOT_FOUND)
			{
				if(len > 0)
					len += sprintf(buf + len , "NA");	
				break;
			}

			bindgroupIdx = groupIdx + 1;
			if(len == 0)
				len += sprintf(buf + len , "%u", bindgroupIdx);
			else
				len += sprintf(buf + len , ",%u ", bindgroupIdx);
		}

		add_value_to_new_line_printor(PRINT_STRING, ObjectPrint[2], &printor);
		add_value_to_last_line_printor(PRINT_STRING, buf, &printor);

		add_value_to_new_line_printor(PRINT_STRING, ObjectPrint[3], &printor);
    }
    
   	if (printor != NULL)
	{
		cmdPrintOutResult(printor, &cInputChar);
		free_printor_vector(&printor);
	}
	
	XMORE("\n");
    return SYS_ERR_OK;
}

int32 cmd_vlan_ip_subnet_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_ipSubnet_vlan_group_t   ipsubnetGroup;
    
    uint32 groupIdx = 0;
	sys_l3_ipv4_t ip;
	sys_ipv4_t mask;
	sys_vid_t vid = 0;
	sys_pri_t priority = 0;
	
    osal_memset(&ipsubnetGroup, 0, sizeof(sys_ipSubnet_vlan_group_t));
    SYS_MEM_CLEAR(ip);
    SYS_MEM_CLEAR(mask);
    SYS_MEM_CLEAR(ipsubnetGroup);

    groupIdx = SYS_STR2UINT(argv[0]) - 1;

	STR2IPV4(argv[1], &ip.addr);
	STR2IPV4(argv[2], &mask);
	ip.plen = IPV4_MASK2PFX(mask);
	
	vid = SYS_STR2UINT(argv[3]);
	
	if(argc > 4)
		priority = SYS_STR2UINT(argv[4]);

	ipsubnetGroup.groupId = groupIdx;
	osal_memcpy(&ipsubnetGroup.ipaddr, &ip, sizeof(sys_l3_ipv4_t));
	ipsubnetGroup.vid = vid;
	ipsubnetGroup.priority = priority;

    SYS_ERR_CHK(sal_vlan_ip_subnet_group_set(groupIdx, &ipsubnetGroup));

    return SYS_ERR_OK;
}

int32 cmd_vlan_ip_subnet_group_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 groupIdx = 0;

    groupIdx = SYS_STR2UINT(argv[0]) - 1;

    SYS_ERR_CHK(sal_vlan_ip_subnet_group_del(groupIdx));

    return SYS_ERR_OK;
}

int32 cmd_vlan_ip_subnet_vlan_port_bind_group(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    uint32 groupIdx = 0;
	sys_enable_t enable = DISABLED;
    
    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    groupIdx = SYS_STR2UINT(argv[0]) - 1;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_vlan_port_ipsubnet_vlan_group_bind_set(logicPort, groupIdx, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_ip_subnet_vlan_port(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
	sys_enable_t enable = DISABLED;
    
    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_vlan_port_ipsubnet_vlan_enable(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_ip_subnet_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 groupIdx = 0;
    sys_ipSubnet_vlan_group_t  protoGroup;
	char ipStr[CAPA_IPV4_STR_LEN];
	char maskStr[CAPA_IPV4_STR_LEN];

    for (groupIdx = 0; groupIdx < CAPA_VLAN_IP_SUBNET_GROUP_NUM; groupIdx++)
    {
        osal_memset(&protoGroup, 0, sizeof(sys_ipSubnet_vlan_group_t));

        SYS_ERR_CONTINUE(sal_vlan_ip_subnet_group_get(groupIdx, &protoGroup));

		if((protoGroup.ipaddr.addr != 0) && (protoGroup.vid != 0))
        {
        	SYS_MEM_CLEAR(ipStr);
        	IPV42STR(protoGroup.ipaddr.addr, ipStr);
        	IPV42STR(IPV4_PFX2MASK(protoGroup.ipaddr.plen), maskStr);
            osal_memset(cmd, 0, sizeof(MAX_CMD_STRLEN));
            if(protoGroup.priority)
            {
            	osal_sprintf(cmd, "vlan ip-subnet-vlan group %u ip %s %s vlan %u priority %u\n",
                	groupIdx+1, ipStr, maskStr, protoGroup.vid, protoGroup.priority);
			}
			else
			{
				osal_sprintf(cmd, "vlan ip-subnet-vlan group %u ip %s %s vlan %u\n",
					groupIdx+1, ipStr, maskStr, protoGroup.vid);
			}
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_ip_subnet_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 groupIdx = 0;
    sys_enable_t enable;
	int rc = SYS_ERR_OK;

	SYS_ERR_CHK(sal_vlan_port_ipsubnet_vlan_status(port, &enable));
	
	if(enable)
	{
		SYS_MEM_CLEAR(cmd);
		osal_sprintf(cmd, "vlan ip-subnet-vlan enable\n");
		CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
	}

	for (groupIdx = 0; groupIdx < CAPA_VLAN_IP_SUBNET_GROUP_NUM; groupIdx++)
	{
		rc = sal_vlan_port_ipsubnet_vlan_group_bind_get(port, &groupIdx);
		if(rc == SYS_ERR_ENTRY_NOT_FOUND)
		{
			break;
		}
		SYS_MEM_CLEAR(cmd);
		osal_sprintf(cmd, "vlan ip-subnet-vlan group %u enable\n", groupIdx + 1);
		CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
	}

    return SYS_ERR_OK;
}

