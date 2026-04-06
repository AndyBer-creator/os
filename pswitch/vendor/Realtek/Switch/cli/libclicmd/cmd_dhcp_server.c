
#include <libcmd/cmd.h>
#include <libsal/sal_dhcp_server.h>

sys_namestr_t g_ippoolname;

extern int32 sal_dhcp_server_get(cfg_dhcp_server_t *pDhcpServer);

static int32 cmd_ip_pool_l3_str2L3IPv4(char *str, sys_l3_ipv4_t *pIp)
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

int32 cmd_dhcp_server_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    CMD_PORTLIST_GET(0, &lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
     
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_server_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char vlanStr[CAPA_VLANLIST_STR_LEN];
    char ipStr[CAPA_IPV4_STR_LEN];
    sys_vlanmask_t vlanmask;
    sys_enable_t enable = DISABLED;
    sys_ipv4_t ipv4, tmpipv4;;
    uint32 group, intf;
    uint32 vid ;
    uint32 infgroup[8];
    uint32 index  = 0;
    char strGroups[100];
    int k = 0;

    VLANMASK_CLEAR_ALL(vlanmask);
    osal_memset(&vlanStr, 0, sizeof(vlanStr));
	osal_memset(&tmpipv4, 0, sizeof(sys_ipv4_t));

    SYS_ERR_CHK(sal_dhcp_server_enable_get(&enable));

    XMORE("\n");
    XMORE("DHCP server             : %s\n", text_enable[enable].text);
    for(group = 1; group <= 256; group++)
    {
    	osal_memset(&ipv4, 0, sizeof(sys_ipv4_t));
    	SYS_ERR_CHK(sal_dhcp_server_group_ip_get(group, &ipv4, &vid));
    	if(osal_memcmp(&ipv4, &tmpipv4, sizeof(sys_ipv4_t)) == 0)
    	{
			continue;
    	}
    	IPV42STR(ipv4, ipStr);
    	XMORE("DHCP server group %d ip : %s\n", group, ipStr);
    }

    XMORE("\ninterface  dhcp server group ip \n");
    intf = 0;
    vid = 0;
    while ( SYS_ERR_OK == sal_dhcp_server_intf_group_ip_get(DATA_OP_GETNEXT, &intf, & vid, infgroup) )
    {
		SYS_MEM_CLEAR(strGroups);

		k = 0;
		for ( index = 0; index < DHCPINTERFACEMAXNUM; index++ )
        {
            if ( infgroup[index] != DHCPEMPTYGROUP )
            {
                k += sprintf(strGroups+k, " %d,",  infgroup[index]);
            }
        }
        
        if(vid)
        {
			XMORE(" interface vlan %d server group  : %s\n", vid, strGroups);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_client_host_mac_table_all_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint16 usStart, usNextStart, usNum;
	dhcp_snooping_mac_lst_t nodeBuf[IPPOOL_MAC_BUF_LEN], *pNode;
	int i, cnt = 0;
	char macstr[CAPA_MAC_STR_LEN];
	char ipstr[CAPA_IPV4_STR_LEN];

    osal_memset(&macstr, 0, sizeof(macstr));
    osal_memset(&ipstr, 0, sizeof(ipstr));
    osal_memset(&nodeBuf, 0, sizeof(dhcp_snooping_mac_lst_t));

    XMORE("\n");
    XMORE("dhcp-client bind table info:\n");
    XMORE("     MAC Address            ipAddress       VlanId      UserName \n");
    XMORE("  -----------------    -----------------    -------    ----------\n");
    usNextStart = 1;
    while(1)
    {
        usStart = usNextStart;
        usNum = 0;
        if ( SYS_ERR_OK != sal_dhcp_client_host_mac_ip_get(usStart, &usNextStart, &usNum, nodeBuf) )
        {
            break;
        }

        for ( i = 0; i < usNum; i++ )
        {
           pNode = &nodeBuf[i];

           SYS_ERR_CHK(sys_util_mac2Str(pNode->mac, macstr));
           XMORE("  %-17s  ", macstr);

           SYS_ERR_CHK(IPV42STR(pNode->ulIpAddress, ipstr));
           XMORE("  %-15s  ", ipstr);

           XMORE("    %-4d  ", pNode->usVlanID);

           XMORE("  %s\n  ", pNode->szUserName);

		   XMORE("\n");

           cnt++;
        }

        if ( (0 == usNextStart) || (0 == usNum) )
        {
            break;
        }
    }

	XMORE("Total %d entry.\n", cnt);

    return SYS_ERR_OK;
}

int32 cmd_dhcp_client_ip_bind_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_enable_t ipBindEnable;
	sys_enable_t unBoundAssignEnable;

	SYS_ERR_CHK(sal_dhcp_server_IpBind_get(&ipBindEnable, &unBoundAssignEnable));
    XMORE("\n");
    XMORE("dhcp-client bind config:\n");
    XMORE("dhcp-client bind status:            %s\n", (ipBindEnable) ? "enable": "disable");
    XMORE("dhcp-client unbound assign status:  %s\n", (unBoundAssignEnable) ? "enable": "disable");
	XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_show_dhcp_ip_pool(ippool_t *ipPool)
{
	sys_ippool_section_t *pIpSection;
    char szGetPoolName[IP_POOL_MAX_NAME];
	char ipStr[CAPA_IPV4_STR_LEN];
	char maskStr[CAPA_IPV4_STR_LEN];
    char strTmp[80];
    char szRow[256] = {0};
    ulong_t ulPrintSectNum= 0;
    int  day, hour, minute, second;
	int i;
	sys_ipv4_t  ipv4[GET_NUM_ONE];
	int flag = 1;
    char *msgPrint[] =
    {
       "\r\nSystem error." ,
         "\r\nThis ip pool does not exist." ,
        "\r\nIP pool name : %s\r\n",
        "GateWay      : %s\r\n",
        "Mask         : %s\r\n",
       "Lease time   : %s\r\n",
        "Primary DNS  : %s\r\n",
       "Second DNS   : %s\r\n",
       "Third DNS    : %s\r\n",
        "Fourth DNS   : %s\r\n",
       "Primary wins : %s\r\n",
       "Second wins  : %s\r\n",
       "SectionNo",
       "StartIp  ",
       "EndIp    ",
       "Total    ",
       "Used/Idle",
       "\r\nNo ip pool configured" ,
       "\r\nTotal entries: %d pool(s), printed %d pool(s)  \r\n",
       "\r\nFailed to get primary dns" ,
       "\r\nFailed to get second dns" ,
       "\r\nFailed to get third dns" ,
       "\r\nFailed to get fourth dns" ,
       "\r\nFailed to get primary Wins" ,
       "\r\nFailed to get second Wins" ,
       "\r\nFailed to get lease time" ,
       "DNS Suffix   : %s\r\n\r\n",
         "Router       : %s\r\n",
         "Option43     : %s\r\n",
         "tftpserver   : %s\r\n",
         "bootfile name: %s\r\n",
         "ip disable addrss  : \r\n"
    };

    ulPrintSectNum = 0;
    memset(szGetPoolName,'\0',IP_POOL_MAX_NAME);

    if (0 == osal_strlen(ipPool->szName))
    {
		return SYS_ERR_OK;
    }

    XMORE("\n");

	sprintf(szRow, msgPrint[2], ipPool->szName);
    XMORE("%s", szRow);

	SYS_ERR_CHK(IPV42STR(ipPool->ulGatewayIp, ipStr));
	sprintf(szRow, msgPrint[3], ipStr);
    XMORE("%s", szRow);

	SYS_ERR_CHK(IPV42STR(ipPool->ulSubnetMask, ipStr));
	sprintf(szRow, msgPrint[4], ipStr);
    XMORE("%s", szRow);

    second = ipPool->ulLeaseTime;
    minute = second / 60;
    hour = minute / 60;
    day = hour / 24;
    hour %= 24;
    minute %= 60;
    second %= 60;
    if(day > 0)
    {
        sprintf(strTmp, "%d day %d hour %d minute", day, hour, minute);
    }
    else if(hour > 0)
    {
        sprintf(strTmp, "%d hour %d minute", hour, minute);
    }
    else if(minute > 0)
    {
        sprintf(strTmp, "%d minute",  minute);
    }

	sprintf(szRow, msgPrint[5], strTmp);
    XMORE("%s", szRow);

	SYS_ERR_CHK(IPV42STR(ipPool->ulDNSIp[0], ipStr));
	sprintf(szRow, msgPrint[6], ipStr);
    XMORE("%s", szRow);

	SYS_ERR_CHK(IPV42STR(ipPool->ulDNSIp[1], ipStr));
	sprintf(szRow, msgPrint[7], ipStr);
    XMORE("%s", szRow);

#if 0
	SYS_ERR_CHK(IPV42STR(ipPool->ulDNSIp[2], ipStr));
	sprintf(szRow, msgPrint[8], ipStr);
    XMORE("%s", szRow);

	SYS_ERR_CHK(IPV42STR(ipPool->ulDNSIp[3], ipStr));
	sprintf(szRow, msgPrint[9], ipStr);
    XMORE("%s", szRow);
#endif
   
    if (0 == osal_strlen(ipPool->szDnsSuffixName))
    {
		sprintf(szRow, msgPrint[26], "N/A");
		XMORE("%s", szRow);
    }
    else
    {
		sprintf(szRow, msgPrint[26], ipPool->szDnsSuffixName);
		XMORE("%s", szRow);
    }

    for(i = 0; i < IP_POOL_MAX_IPSECTION_NUM; i++)
    {
		pIpSection = &ipPool->IpSection[i];
        if ( pIpSection->ulStartIp && pIpSection->ulEndIp )
        {
            if(ulPrintSectNum==0)
            {
                ulPrintSectNum =1;
				sprintf(szRow, "%-15s %-15s %-15s\r\n", msgPrint[12], msgPrint[13], msgPrint[14]);
				XMORE("%s", szRow);
            }

			SYS_ERR_CHK(IPV42STR(pIpSection->ulStartIp, ipStr));
			SYS_ERR_CHK(IPV42STR(pIpSection->ulEndIp, maskStr));
			sprintf(szRow, "%-15d %-15s %-15s\r\n", i+1, ipStr, maskStr);
			XMORE("%s", szRow);
        }
        else 
        {
            if(ulPrintSectNum==0)
            {
                ulPrintSectNum =1;
				sprintf(szRow, "%-15s %-15s %-15s\r\n", msgPrint[12], msgPrint[13], msgPrint[14]);
				XMORE("%s", szRow);
            }

			sprintf(szRow, "%-15d %-15s %-15s\r\n", i+1, "N/A", "N/A");
			XMORE("%s", szRow);
        }
    }
	
next:
	SYS_MEM_CLEAR(ipv4);
	SYS_ERR_CHK(sal_ip_pool_disable_ip_get(ipPool->szName, ipv4));
	for(i = 0; i < GET_NUM_ONE; i++)
	{
		if(ipv4[i])
		{
			if(flag)
			{
				XMORE("%s", msgPrint[31]);
				flag = 0;
			}
			IPV42STR(ipv4[i], ipStr);
			XMORE("    %-15s\r\n", ipStr);			
			if(i == GET_NUM_ONE-1)
			{
				goto next;
			}
		}
	}
	XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_dhcp_ip_pool_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	cfg_dhcp_server_t dhcpServer;
	int i;

    SYS_MEM_CLEAR(dhcpServer);
    SYS_ERR_CHK(sal_dhcp_server_get(&dhcpServer));

	if (0 == argc)
	{
		for (i = 0; i < IP_POOL_MAX_NUM; i++)
		{
			cmd_show_dhcp_ip_pool(&dhcpServer.gIpPool[i]);
		}
	}
	else
	{
		for (i = 0; i < IP_POOL_MAX_NUM; i++)
		{
			if (!osal_strcmp(dhcpServer.gIpPool[i].szName, argv[0]))
			{
				cmd_show_dhcp_ip_pool(&dhcpServer.gIpPool[i]);
			}
		}
	}

    return SYS_ERR_OK;
}

int32 cmd_dhcp_ip_pool_brief_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	cfg_dhcp_server_t dhcpServer;
	ippool_t *pIpPool;
	char ipStr[CAPA_IPV4_STR_LEN];
	char dns1Str[CAPA_IPV4_STR_LEN];
	char dns2Str[CAPA_IPV4_STR_LEN];
    char szRow[256] = {0};
    char strTmp[80];
    char ipTmp[24];
    int day, hour, minute, second;
	int i, j;

    SYS_MEM_CLEAR(dhcpServer);
    SYS_ERR_CHK(sal_dhcp_server_get(&dhcpServer));

	XMORE("%-15s %-15s %-15s %-15s %-15s\r\n", "name", "gateway/mask", "pri-dns", "sec-dns", "lease-time(days:hours:minutes)");

	for (i = 0; i < IP_POOL_MAX_NUM; i++)
	{
		if (0 != osal_strlen(dhcpServer.gIpPool[i].szName))
	    {
			pIpPool = &dhcpServer.gIpPool[i];
			SYS_MEM_CLEAR(szRow);

			SYS_ERR_CHK(IPV42STR(pIpPool->ulGatewayIp, ipStr));
			for ( j = 0; j < 32; j++ )
			{
				if ( pIpPool->ulSubnetMask & (1 << j) )
				{
					break;
				}
			}
			j = 32 - j;
			sprintf(ipTmp, "%s/%d", ipStr, j);
			SYS_ERR_CHK(IPV42STR(pIpPool->ulDNSIp[0], dns1Str));
			SYS_ERR_CHK(IPV42STR(pIpPool->ulDNSIp[0], dns2Str));

			second = pIpPool->ulLeaseTime;
			minute = second / 60;
			hour = minute / 60;
			day = hour / 24;
			hour %= 24;
			minute %= 60;
			second %= 60;

			sprintf(strTmp, "%d:%d:%d", day, hour, minute);

			sprintf(szRow, "%-15s %-15s %-15s %-15s %-15s\r\n", pIpPool->szName,
					ipTmp, dns1Str, dns1Str, strTmp);
			XMORE("%s", szRow);
	    }
	}

	return SYS_ERR_OK;
}

int32 cmd_dhcp_server_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_ipv4_t ip = 0;
    uint32 group = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (1 == argc)
        {
            group = SYS_STR2UINT(argv[0]);

            SYS_ERR_CHK(sal_dhcp_server_group_ip_del(group));
        }
        else
        {

            SYS_ERR_CHK(sal_dhcp_server_group_ip_clear());
        }

        return SYS_ERR_OK;
    }
    else
    {
		group = SYS_STR2UINT(argv[0]);
    }

    if (2 == argc)
    {
        STR2IPV4(argv[1], &ip);
    }
    else
    {
		return SYS_ERR_FAILED;
    }

    SYS_ERR_CHK(sal_dhcp_server_group_ip_add(group, ip));

    return SYS_ERR_OK;

}

int32 cmd_show_dhcps_hash(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    ulong_t i = 0;
    ulong_t j = 0;
    MACHASH_S *pHashData;
    char szTmpVLAN[20];
    char szPrintString[128];
    ushort_t usStart, usNextStart, usNum;
    MACHASH_S hostBuf[DHCPRS_HOST_BUF_MAX];
	char ipStr[CAPA_IPV4_STR_LEN];
	char macstr[CAPA_MAC_STR_LEN];
    XMORE("\n");
    XMORE("dhcp-client table info:\n");
    XMORE("%-19s %-17s %-6s %-16s\r\n", "MAC Address", "ipAddress", "VlanId", "Hostname");
    XMORE("-------------------- ----------------- ------ ----------------\n");
    usNextStart = 1;

    while ( 1 )
    {
        usStart = usNextStart;
        usNum = 0;
        if ( SYS_ERR_OK != sal_dhcp_client_hash_get(usStart, &usNextStart, &usNum, hostBuf) )
        {
            break;
        }

        for ( i = 0; i < usNum; i++ )
        {
            pHashData = &hostBuf[i];

			SYS_ERR_CHK(sys_util_mac2Str(pHashData->ucUserMac, macstr));
            
            sprintf ( szTmpVLAN, "%d", pHashData->usVlanId );
            
			SYS_ERR_CHK(IPV42STR(pHashData->ulIpAddr, ipStr));
            j++;
            
            sprintf(szPrintString, "%-19s %-17s %-6s %-16s\r\n", macstr, ipStr, szTmpVLAN,
                    ((pHashData->hostname[0]!='\0')?pHashData->hostname:"(unknown)"));

			XMORE("%s", szPrintString);
        }

        if ( (0 == usNum) || (0 == usNextStart) )
        {
            break;
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_server_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_dhcp_server_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED: ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_dhcp_server_intf_group_ip_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 group = 0;
    sys_l3_intfId_t id;

    SYS_MEM_CLEAR(id);

	if(vty->node == CONFIG_NODE)
	{
		STR2INTFID(L3_INTF_MGMTVLAN, &id);
	}
	else
	{
	    id.type = g_l3IntfType;

	    if (SYS_L3_INTFTYPE_VLAN == id.type)
	        id.id = g_selVid;
	    else if (SYS_L3_INTFTYPE_LO == id.type)
	        id.id = g_selLo;
	}
	
    if (CMD_IS_NO_FORM_EXIST())
    {
        if (1 == argc)
        {
            group = SYS_STR2UINT(argv[0]);
            
            SYS_ERR_CHK(sal_dhcp_server_intf_group_ip_del(group, id.id));
        }
        return SYS_ERR_OK;
    }
    else
    {
		group = SYS_STR2UINT(argv[0]);
    }

    SYS_ERR_CHK(sal_dhcp_server_intf_group_ip_add(group, id.id));

    return SYS_ERR_OK;

}

int32 cmd_dhcp_relay_option82_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_dhcp_relay_option82_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED: ENABLED));
    return SYS_ERR_OK;
}

int32 cmd_dhcp_relay_option82_device_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_dhcp_relay_option82_device_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED: ENABLED));
    return SYS_ERR_OK;
}

int32 cmd_dhcp_relay_option82_format_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 format = OPTION82FORMAT_NORMAL;

	if (!CMD_IS_NO_FORM_EXIST())
	{
		if (0 == osal_strncmp("verbose", argv[0], osal_strlen(argv[0])))
			format = OPTION82FORMAT_VERBOSE;
	}

    SYS_ERR_CHK(sal_dhcp_relay_option82_format_set(format));
    return SYS_ERR_OK;
}

int32 cmd_dhcp_relay_option82_info_format_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 iFormat = OPTION82INFOFORMAT_HEX;

	if (!CMD_IS_NO_FORM_EXIST())
	{
		if (0 == osal_strncmp("ascii", argv[0], osal_strlen(argv[0])))
			iFormat = OPTION82INFOFORMAT_ASCII;
	}

	SYS_ERR_CHK(sal_dhcp_relay_option82_info_format_set(iFormat));
	return SYS_ERR_OK;
}

int32 cmd_dhcp_relay_option82_format_verbose_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char option82VerboseNodeId[OPTION82_VERBOS_NODEID_LEN + 1] = {0};

	if (!CMD_IS_NO_FORM_EXIST())
	{
		if (0 == osal_strncmp("hostname", argv[0], osal_strlen(argv[0])))
			strcpy((char *)option82VerboseNodeId, OPTION82_VERBOSE_NODEID_HOSTNAME);
		else if (0 == osal_strncmp("user-defined", argv[0], osal_strlen(argv[0])))
			osal_memcpy(option82VerboseNodeId, argv[1], strlen(argv[1]));
	}

	SYS_ERR_CHK(sal_dhcp_relay_option82_format_verbose_set(option82VerboseNodeId));
	return SYS_ERR_OK;
}

int32 cmd_dhcp_relay_option82_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t lp;

    enable = CMD_IS_NO_FORM_EXIST()? DISABLED : ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_relay_option82_port_enable_set(lp, enable), lp);
    }

	return SYS_ERR_OK;
}

int32 cmd_dhcp_relay_option82_port_strategy_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 strategy = OPTION82STRATEGY_REPLACE;
    sys_logic_port_t lp;

	if (!CMD_IS_NO_FORM_EXIST())
	{
		if (0 == osal_strncmp("drop", argv[0], osal_strlen(argv[0])))
	    {
	        strategy = OPTION82STRATEGY_DROP;
	    }
	    else if (0 == osal_strncmp("keep", argv[0], osal_strlen(argv[0])))
	    {
	        strategy = OPTION82STRATEGY_KEEP;
	    }
	    else if (0 == osal_strncmp("hostname", argv[0], osal_strlen(argv[0])))
    	{
			strategy = OPTION82STRATEGY_APPEND_HOSTNAME;
		}
	    else if (0 == osal_strncmp("hostname-ip", argv[0], osal_strlen(argv[0])))
    	{
			strategy = OPTION82STRATEGY_APPEND_HOSTNAME_IP;
		}
	    else if (0 == osal_strncmp("apend", argv[0], osal_strlen(argv[0])))
	    {
	        if (0 == osal_strncmp("hostname", argv[1], osal_strlen(argv[1])))
	        {
				strategy = OPTION82STRATEGY_APPEND_HOSTNAME;
	        }
	        else
	        {
				strategy = OPTION82STRATEGY_APPEND_HOSTNAME_IP;
	        }
	    }
	}

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_relay_option82_port_strategy_set(lp, strategy), lp);
    }

	return SYS_ERR_OK;
}

int32 cmd_dhcp_relay_option82_port_circuit_id_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	char option82CircuitId[OPTION82_CIRCUITID_LEN + 1] = {0};
    sys_logic_port_t lp;

	if (!CMD_IS_NO_FORM_EXIST())
	{
		osal_memcpy(option82CircuitId, argv[0], strlen(argv[0]));
	}

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_relay_option82_port_circuit_id_set(lp, option82CircuitId), lp);
    }

	return SYS_ERR_OK;
}

int32 cmd_dhcp_relay_option82_port_remote_id_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	char option82RemoteId[OPTION82_REMOTEID_LEN + 1] = {0};
    sys_logic_port_t lp;

	if (!CMD_IS_NO_FORM_EXIST())
	{
		if (0 == osal_strncmp("hostname", argv[0], osal_strlen(argv[0])))
	    {
	        strcpy(option82RemoteId, OPTION82_VERBOSE_NODEID_HOSTNAME);
	    }
	    else
	    {
			osal_memcpy(option82RemoteId, argv[0], strlen(argv[0]));
	    }
	}

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_relay_option82_port_remote_id_set(lp, option82RemoteId), lp);
    }

	return SYS_ERR_OK;
}

int32 cmd_ip_pool_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	osal_memset(&g_ippoolname,0, sizeof(g_ippoolname));

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_ip_pool_del(argv[0]));
    }
    else
    {
        SYS_ERR_CHK(sal_ip_pool_create(argv[0]));
		osal_memcpy(g_ippoolname.str, argv[0], strlen(argv[0]));
		vty->node = IP_POOL_NODE;
		SYS_MEM_CLEAR(vty->exPrompt); 
		osal_sprintf(vty->exPrompt, "%s", argv[0]);
    }

    return SYS_ERR_OK;
}

int32 cmd_ip_pool_gateway_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_l3_ipv4_t dstIp;
	sys_ipv4_t addr;
	sys_ipv4_t mask;
    uint32 usingMask = FALSE;

    SYS_MEM_CLEAR(dstIp);

    if (NULL == osal_strchr(argv[0], '/'))
        usingMask = TRUE;

    if (usingMask)
    {
        STR2IPV4(argv[0], &addr);
        STR2IPV4(argv[1], &mask);
    }
    else
    {
        cmd_ip_pool_l3_str2L3IPv4(argv[0], &dstIp);
		addr = dstIp.addr;
		mask = IPV4_PFX2MASK(dstIp.plen);
    }

	SYS_ERR_CHK(sal_ip_pool_gateway_set(g_ippoolname.str, addr, mask));

    return SYS_ERR_OK;
}

int32 cmd_no_ip_pool_gateway_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_ipv4_t addr;
	sys_ipv4_t mask;

	SYS_MEM_CLEAR(addr);
	SYS_MEM_CLEAR(mask);

	SYS_ERR_CHK(sal_ip_pool_gateway_set(g_ippoolname.str, addr, mask));

    return SYS_ERR_OK;
}

int32 cmd_ip_pool_section_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_ipv4_t start_ip;
	sys_ipv4_t end_ip;
	uint32     id;
	int rc = 0;

	id = SYS_STR2UINT(argv[0]);
	STR2IPV4(argv[1], &start_ip);
	STR2IPV4(argv[2], &end_ip);
	rc = sal_ip_pool_section_set(g_ippoolname.str, id-1, start_ip, end_ip);
    return rc;
}

int32 cmd_no_ip_pool_section(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32	   id;

	id = SYS_STR2UINT(argv[0]);
	SYS_ERR_CHK(sal_ip_pool_section_set(g_ippoolname.str, id-1, 0, 0));

	return SYS_ERR_OK;
}

int32 cmd_ip_pool_disable_ip(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_ipv4_t	   ipv4;
	
	STR2IPV4(argv[0], &ipv4);
	if (CMD_IS_NO_FORM_EXIST())
	{
		sal_ip_pool_disable_ip_set(g_ippoolname.str, FALSE, ipv4);
	}
	else 
	{
		
		sal_ip_pool_disable_ip_set(g_ippoolname.str, TRUE, ipv4);
	}

	return SYS_ERR_OK;
}

int32 cmd_dhcp_relay_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t lp;

    enable = CMD_IS_NO_FORM_EXIST()? DISABLED : ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_relay_port_enable_set(lp, enable), lp);
    }

	return SYS_ERR_OK;
}
int32 cmd_dhcp_server_dns_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 dns_num = IP_POOL_PRIMARY_DNS_NUM;
	sys_ipv4_t addr = 0;

	if (0 == osal_strncmp("primary-ip", argv[0], osal_strlen(argv[0])))
	{
		dns_num = IP_POOL_PRIMARY_DNS_NUM;
	}
	else if (0 == osal_strncmp("second-ip", argv[0], osal_strlen(argv[0])))
	{
		dns_num = IP_POOL_SECOND_DNS_NUM;
	}
	else if (0 == osal_strncmp("third-ip", argv[0], osal_strlen(argv[0])))
	{
		dns_num = IP_POOL_THIRD_DNS_NUM;
	}
	else if (0 == osal_strncmp("fourth-ip", argv[0], osal_strlen(argv[0])))
	{
		dns_num = IP_POOL_PFOURTH_DNS_NUM;
	}

	if (!CMD_IS_NO_FORM_EXIST())
	{
        STR2IPV4(argv[1], &addr);
	}

    SYS_ERR_CHK(sal_dhcp_server_dns_set(g_ippoolname.str, dns_num, addr));

	return SYS_ERR_OK;
}

int32 cmd_dhcp_server_dns_suffix_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	char dnsSuffixName[IPPOOL_MAX_DNS_SUFFIX_NAME] = {0};

	if (!CMD_IS_NO_FORM_EXIST())
		osal_memcpy(dnsSuffixName, argv[0], strlen(argv[0]));

    SYS_ERR_CHK(sal_dhcp_server_dns_suffix_set(g_ippoolname.str, dnsSuffixName));

	return SYS_ERR_OK;
}

int32 cmd_dhcp_server_wins_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	uint32 wins_num = IP_POOL_PRIMARY_WINS_NUM;
	sys_ipv4_t addr = 0;

	if (0 == osal_strncmp("primary-ip", argv[0], osal_strlen(argv[0])))
	{
		wins_num = IP_POOL_PRIMARY_WINS_NUM;
	}
	else if (0 == osal_strncmp("second-ip", argv[0], osal_strlen(argv[0])))
	{
		wins_num = IP_POOL_SECOND_WINS_NUM;
	}

	if (!CMD_IS_NO_FORM_EXIST())
	{
        STR2IPV4(argv[1], &addr);
	}

    SYS_ERR_CHK(sal_dhcp_server_wins_set(g_ippoolname.str, wins_num, addr));

	return SYS_ERR_OK;
}

int32 cmd_dhcp_server_lease_time_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 i = 0;
    char *ridx = NULL;
    char *tmp = NULL;
	uint32 days = 0;;
	uint32 hours = 0;;
	uint32 mins = 0;;
 	uint32 lease_time = IP_POOL_DEFAULT_LEASETIME;

	if (!CMD_IS_NO_FORM_EXIST())
	{
        
	    tmp = argv[0];
	    ridx = rindex(tmp, ':');

	    if (NULL == ridx)
	        return SYS_ERR_INPUT;

	    *ridx = '\0';
	    ridx++;
	    for (i = 0; i < osal_strlen(ridx) ; i++)
	    {
	        if (!isdigit(ridx[i]))
	            return SYS_ERR_INPUT;
	    }
	    mins = SYS_STR2UINT(ridx);

	    if ( mins > 59 )
	        return SYS_ERR_INPUT;

	    ridx = rindex(tmp, ':');

	    if (NULL == ridx)
	        return SYS_ERR_INPUT;

	    *ridx = '\0';
	    ridx++;
	    for (i = 0; i < osal_strlen(ridx) ; i++)
	    {
	        if (!isdigit(ridx[i]))
	            return SYS_ERR_INPUT;
	    }
	    hours = SYS_STR2UINT(ridx);

	    if ( hours > 23 )
	        return SYS_ERR_INPUT;

	    days = SYS_STR2UINT(tmp);
	    for (i = 0; i < osal_strlen(tmp) ; i++)
	    {
	        if (!isdigit(tmp[i]))
	            return SYS_ERR_INPUT;
	    }
	    
		if((days == IPPOOL_MAX_DAY) && (hours + mins > 0))
		{
			return SYS_ERR_DHCP_LEASE_MAX_TIME;
		}
		
		if((days > IPPOOL_MAX_DAY) || (hours > 23) || (mins > 59))
		{
			return SYS_ERR_DHCP_LEASE_TIME_ERR;
		}
		
		if(days == 0 && hours == 0 && mins == 0)
		{
			return SYS_ERR_DHCP_LEASE_TIME_EMPTY;
		}

		lease_time = ((days*24 + hours)*60 + mins)*60;
	}

    SYS_ERR_CHK(sal_dhcp_server_lease_time_set(g_ippoolname.str, lease_time));

	return SYS_ERR_OK;
}

int32 cmd_ip_pool_option43_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_ippool_option43_t option43;

	osal_memset(&option43, 0, sizeof(sys_ippool_option43_t));

	if (!CMD_IS_NO_FORM_EXIST())
	{
		if (0 == osal_strncmp("ascii", argv[0], osal_strlen(argv[0])))
		{
			option43.ucFormat = IP_POOL_OPTION_FORMAT_ASCII;
			option43.ucValueLen = strlen(argv[1]);
			osal_memcpy(option43.szValue, argv[1], strlen(argv[1]));
		}
		else if (0 == osal_strncmp("hex", argv[0], osal_strlen(argv[0])))
		{
			if (1 == (strlen(argv[1]) % 2))
				return SYS_ERR_FAILED;

			option43.ucFormat = IP_POOL_OPTION_FORMAT_HEX;
			option43.ucValueLen = strlen(argv[1])/2;
			sys_util_hexStr2byteArray(argv[1], option43.szValue);
		}
	}

    SYS_ERR_CHK(sal_ip_pool_option43_set(g_ippoolname.str, &option43));

	return SYS_ERR_OK;
}

int32 cmd_dhcp_client_config_ip_bind(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_dhcp_server_IpBind_Switch_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED: ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_dhcp_client_host_mac_ip_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_mac_t mac;
	sys_ipv4_t ipaddr;
	sys_vid_t vid;
	char userName[32];

    SYS_MEM_CLEAR(mac);
    SYS_MEM_CLEAR(ipaddr);
    SYS_MEM_CLEAR(vid);
    SYS_MEM_CLEAR(userName);

    if (CMD_IS_NO_FORM_EXIST())
    {
    	
    	SYS_ERR_CHK(sys_util_str2Mac(argv[0], mac.octet));
    	if ((0x1 & mac.octet[0]))
                return SYS_ERR_L3_STATIC_NEIGH_MAC;

        vid = SYS_STR2UINT(argv[1]);

        SYS_ERR_CHK(sal_dhcp_client_host_mac_ip_del(&mac, vid));
        return SYS_ERR_OK;
    }
    else
    {
    	
    	SYS_ERR_CHK(sys_util_str2Mac(argv[0], mac.octet));
    	if ((0x1 & mac.octet[0]))
                return SYS_ERR_L3_STATIC_NEIGH_MAC;

		if (FALSE == IS_IPV4_UADDRSTR_VALID(argv[1]))
                return SYS_ERR_IPV4_ADDR;
        SYS_ERR_CHK(STR2IPV4(argv[1], &ipaddr));

        vid = SYS_STR2UINT(argv[2]);

       	osal_sprintf(userName, "%s", argv[3]);
		SYS_ERR_CHK(sal_dhcp_client_host_mac_ip_add(&mac, ipaddr, vid, userName));
    }

    return SYS_ERR_OK;

}

int32 cmd_dhcp_client_unknown_client_assign(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_dhcp_client_unknown_client_assign((CMD_IS_NO_FORM_EXIST()) ? DISABLED: ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_dhcp_client_save(FILE *fp)
{
	char cmd[MAX_CMD_STRLEN];
	sys_enable_t ipBindEnable;
	sys_enable_t unBoundAssignEnable;
	uint16 usStart, usNextStart, usNum;
	dhcp_snooping_mac_lst_t nodeBuf[IPPOOL_MAC_BUF_LEN], *pNode;
	char macstr[CAPA_MAC_STR_LEN];
	char ipstr[CAPA_IPV4_STR_LEN];
	int i ;

    osal_memset(&macstr, 0, sizeof(macstr));
    osal_memset(&ipstr, 0, sizeof(ipstr));
    osal_memset(&nodeBuf, 0, sizeof(dhcp_snooping_mac_lst_t));

	SYS_ERR_CHK(sal_dhcp_server_IpBind_get(&ipBindEnable, &unBoundAssignEnable));
	if(ipBindEnable)
	{
		SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dhcp-client bind\n");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    if(unBoundAssignEnable)
	{
		SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dhcp-client unknown-client assign\n");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    usNextStart = 1;
    while(1)
    {
        usStart = usNextStart;
        usNum = 0;
        if ( SYS_ERR_OK != sal_dhcp_client_host_mac_ip_get(usStart, &usNextStart, &usNum, nodeBuf) )
        {
            break;
        }

        for ( i = 0; i < usNum; i++ )
        {
			pNode = &nodeBuf[i];
			SYS_MEM_CLEAR(cmd);

			SYS_ERR_CHK(sys_util_mac2Str(pNode->mac, macstr));
			SYS_ERR_CHK(IPV42STR(pNode->ulIpAddress, ipstr));
			osal_sprintf(cmd, "dhcp-client %s %s %d %s\n", macstr, ipstr, pNode->usVlanID, pNode->szUserName);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        if ( (0 == usNextStart) || (0 == usNum) )
        {
            break;
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_server_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char tmpStr[256];
	cfg_dhcp_server_t dhcpServer;
    char ipStr[CAPA_IPV4_STR_LEN];
    char ipEndStr[CAPA_IPV4_STR_LEN];
    char str[IP_POOL_MAX_NAME];
	ippool_t *ipPool;
	int i, j;
    int day, hour, minute, second;
	sys_ipv4_t	ipv4[GET_NUM_ONE];

	osal_memset(&str,0, IP_POOL_MAX_NAME);

    SYS_MEM_CLEAR(dhcpServer);
    SYS_ERR_CHK(sal_dhcp_server_get(&dhcpServer));
    if (ENABLED == dhcpServer.enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dhcp-server\n");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#ifndef CONFIG_SYS_L3_ARCHITECTURE
	
    SYS_ERR_CHK(cmd_dhcp_sever_mgmt_intf_save(fp));
#endif
    for (i = 0; i < DHCP_GROUP_MAX_NUM; i++)
    {
    	if (dhcpServer.dhcpGroup[i].serverIp)
		{
	        SYS_MEM_CLEAR(cmd);
            SYS_ERR_CONTINUE(sys_util_ip2Str(dhcpServer.dhcpGroup[i].serverIp, ipStr));
	        osal_sprintf(cmd, "dhcp-server group %d ip %s\n", i + 1, ipStr);
	        CMD_WRITE_TO_FILE(fp, cmd);
		}
    }

    for (i = 0; i < IP_POOL_MAX_NUM; i++)
    {
		if (memcmp(dhcpServer.gIpPool[i].szName, str, IP_POOL_MAX_NAME))
		{
			ipPool = &dhcpServer.gIpPool[i];
	        SYS_MEM_CLEAR(cmd);
	        osal_sprintf(cmd, "ip pool %s\n", dhcpServer.gIpPool[i].szName);
	        CMD_WRITE_TO_FILE(fp, cmd);

			if (ipPool->ulGatewayIp)
			{
				SYS_MEM_CLEAR(cmd);
				SYS_ERR_CONTINUE(sys_util_ip2Str(ipPool->ulGatewayIp, ipStr));
				osal_sprintf(cmd, " gateway %s/%d\n", ipStr, IPV4_MASK2PFX(ipPool->ulSubnetMask));
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if( ipPool->ulDNSIp[0])
			{
				SYS_MEM_CLEAR(cmd);
				SYS_ERR_CONTINUE(sys_util_ip2Str(ipPool->ulDNSIp[0], ipStr));
				osal_sprintf(cmd, " dns primary-ip %s\n", ipStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if( ipPool->ulDNSIp[1])
			{
				SYS_MEM_CLEAR(cmd);
				SYS_ERR_CONTINUE(sys_util_ip2Str(ipPool->ulDNSIp[1], ipStr));
				osal_sprintf(cmd, " dns second-ip %s\n", ipStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if( ipPool->ulDNSIp[2])
			{
				SYS_MEM_CLEAR(cmd);
				SYS_ERR_CONTINUE(sys_util_ip2Str(ipPool->ulDNSIp[2], ipStr));
				osal_sprintf(cmd, " dns third-ip %s\n", ipStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if( ipPool->ulDNSIp[3])
			{
				SYS_MEM_CLEAR(cmd);
				SYS_ERR_CONTINUE(sys_util_ip2Str(ipPool->ulDNSIp[3], ipStr));
				osal_sprintf(cmd, " dns fourth-ip %s\n", ipStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if( strlen( (char *)ipPool->szDnsSuffixName ) != 0 )
			{
			  SYS_MEM_CLEAR(cmd);
			  osal_sprintf(cmd, " dns suffix %s\n", ipPool->szDnsSuffixName);
			  CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if( ipPool->ulPrimaryNBNSIp)
			{
			   SYS_MEM_CLEAR(cmd);
			   SYS_ERR_CONTINUE(sys_util_ip2Str(ipPool->ulPrimaryNBNSIp, ipStr));
			   osal_sprintf(cmd, " wins primary-ip %s\n", ipStr);
			   CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if( ipPool->ulSecondNBNSIp)
			{
				SYS_MEM_CLEAR(cmd);
				SYS_ERR_CONTINUE(sys_util_ip2Str(ipPool->ulSecondNBNSIp, ipStr));
				osal_sprintf(cmd, " wins second-ip %s\n", ipStr);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if( ipPool->ulLeaseTime != IPPOOL_DEFAULT_LEASETIME)
			{
				second = ipPool->ulLeaseTime;
				minute = second /60;
				hour = minute /60;
				day = hour / 24;
				hour %= 24;
				minute %= 60;
				second %= 60;

				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " lease %d:%d:%d\n", day, hour, minute);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
			}

			if ( 0 != ipPool->option43.ucValueLen )
			{
				if ( IP_POOL_OPTION_FORMAT_ASCII == ipPool->option43.ucFormat )
				{
					SYS_MEM_CLEAR(cmd);
					osal_sprintf(cmd, " option 43 ascii %s\n", ipPool->option43.szValue);
					CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
				}
				else
				{
					SYS_MEM_CLEAR(cmd);
					SYS_MEM_CLEAR(tmpStr);
					sys_util_byteArray2hexStr(ipPool->option43.szValue, ipPool->option43.ucValueLen, tmpStr);
					osal_sprintf(cmd, " option 43 hex %s\n", tmpStr);
					CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
				}
			}

			for (j = 0; j < IP_POOL_MAX_IPSECTION_NUM; j++)
			{
				if (ipPool->IpSection[j].ulStartIp
					&& ipPool->IpSection[j].ulEndIp)
				{
					SYS_MEM_CLEAR(cmd);
					SYS_ERR_CONTINUE(sys_util_ip2Str(ipPool->IpSection[j].ulStartIp, ipStr));
					SYS_ERR_CONTINUE(sys_util_ip2Str(ipPool->IpSection[j].ulEndIp, ipEndStr));
					osal_sprintf(cmd, " section %d %s %s\n", j+1, ipStr, ipEndStr);
					CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
				}
			}
			
		next:
			SYS_MEM_CLEAR(ipv4);
			SYS_ERR_CHK(sal_ip_pool_disable_ip_get(ipPool->szName, ipv4));
			for(j = 0; j < GET_NUM_ONE; j++)
			{
				if(ipv4[j])
				{
					IPV42STR(ipv4[j], ipStr);
					SYS_MEM_CLEAR(cmd);
					osal_sprintf(cmd, " ip disable %s\n", ipStr);
					CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
					if(i == GET_NUM_ONE-1)
					{
						goto next;
					}
				}
			}
		}

    }

	SYS_ERR_CHK(cmd_dhcp_client_save(fp));

    if (ENABLED == dhcpServer.option82Enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip dhcp option82\n");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

	if (ENABLED == dhcpServer.option82DevEnabled)
	{
		SYS_MEM_CLEAR(cmd);
		osal_sprintf(cmd, "ip dhcp option82 device-id\n");
		CMD_WRITE_TO_FILE(fp, cmd);
	}

	if ( dhcpServer.option82Format == OPTION82FORMAT_VERBOSE)
    {
        if (strcmp(dhcpServer.option82VerboseNodeId, OPTION82_VERBOSE_NODEID_HOSTNAME) == 0)
        {
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "ip dhcp option82 format verbose node-identifier hostname\n");
			CMD_WRITE_TO_FILE(fp, cmd);
        }
        else if ( dhcpServer.option82VerboseNodeId[0] != 0 )
        {
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "ip dhcp option82 format verbose node-identifier %s\n", dhcpServer.option82VerboseNodeId);
			CMD_WRITE_TO_FILE(fp, cmd);
        }
        else
        {
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, "ip dhcp option82 format verbose\n");
			CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    if ( dhcpServer.opion82InfoFormat == OPTION82INFOFORMAT_ASCII)
    {
		SYS_MEM_CLEAR(cmd);
		osal_sprintf(cmd, "ip dhcp option82 information format ascii\n");
		CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_sever_l3intf_save(FILE *fp, sys_vid_t vid)
{
    char cmd[MAX_CMD_STRLEN];
    uint32  index;
	uint32 groupNo[DHCP_INTERFACE_MAX_NUM];
	uint32 intf = 0;
	SYS_MEM_CLEAR(groupNo);
   	if(sal_dhcp_server_intf_group_ip_get(DATA_OP_GET, &intf, &vid, groupNo) == SYS_ERR_OK)
    {
		for ( index = 0; index < DHCPINTERFACEMAXNUM; index++ )
        {
            if ( groupNo[index] != DHCPEMPTYGROUP )
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " dhcp-server group %d\n", groupNo[index]);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }
        }
    }

    return SYS_ERR_OK;
}
#ifndef CONFIG_SYS_L3_ARCHITECTURE
int32 cmd_dhcp_sever_mgmt_intf_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 vid, index;
    uint32 groupNo[DHCP_INTERFACE_MAX_NUM];
    uint32 intf = 0;
	sys_l3_intfId_t id;
	
	STR2INTFID(L3_INTF_MGMTVLAN, &id);
	vid = id.id;

	SYS_MEM_CLEAR(groupNo);
   	if(sal_dhcp_server_intf_group_ip_get(DATA_OP_GET, &intf, &vid, groupNo) == SYS_ERR_OK)
    {
		for ( index = 0; index < DHCPINTERFACEMAXNUM; index++ )
        {
            if ( groupNo[index] != DHCPEMPTYGROUP )
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, "dhcp-server group %d\n", groupNo[index]);
				CMD_WRITE_TO_FILE(fp, cmd);
            }
        }
    }

    return SYS_ERR_OK;
}
#endif
int32 cmd_dhcp_relay_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;
	cfg_dhcp_server_t dhcpServer;
	dhcpOption82_port_t *portOption82;

    SYS_MEM_CLEAR(dhcpServer);
    SYS_ERR_CHK(sal_dhcp_server_get(&dhcpServer));
    SYS_ERR_CHK(sal_dhcp_relay_port_enable_get(port, &enable));

    if (ENABLED == enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, " dhcp-relay\n");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

	portOption82 = &dhcpServer.portOption82[port];
	if (portOption82->opion82Strategy != OPTION82STRATEGY_REPLACE
            || portOption82->option82CircuitId[0] != 0
            || portOption82->option82RemoteId[0] != 0
            || portOption82->option82Enabled != ENABLED)
        {
            if ( portOption82->option82Enabled == DISABLED )
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " no ip dhcp option82\n");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }

            if ( portOption82->opion82Strategy == OPTION82STRATEGY_DROP )
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " ip dhcp option82 strategy drop\n");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }
            else if( portOption82->opion82Strategy == OPTION82STRATEGY_KEEP )
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " ip dhcp option82 strategy keep\n");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }
            else if( portOption82->opion82Strategy == OPTION82STRATEGY_APPEND_HOSTNAME)
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " ip dhcp option82 strategy append hostname\n");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }
            else if( portOption82->opion82Strategy == OPTION82STRATEGY_APPEND_HOSTNAME_IP)
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " ip dhcp option82 strategy append hostname-ip\n");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }

            if (portOption82->option82CircuitId[0] != 0 )
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " ip dhcp option82 circuit-id string %s\n", portOption82->option82CircuitId);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }

            if ( strcmp( portOption82->option82RemoteId, OPTION82_VERBOSE_NODEID_HOSTNAME) == 0 )
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " ip dhcp option82 remote-id string hostname\n");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }
            else if ( portOption82->option82RemoteId[0] != 0 )
            {
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, " ip dhcp option82 remote-id string %s\n", portOption82->option82RemoteId);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }
	}

    return SYS_ERR_OK;
}

