
#include <libsal/sal_l3_route_rip.h>
#include <libcmd/cmd.h>
#include <libipc/quaggaTask.h>
#include <libipc/vos.h>

#define FRR_RIP_ROUTE_RTE                    0
#define FRR_RIP_ROUTE_STATIC                 1
#define FRR_RIP_ROUTE_DEFAULT                2
#define FRR_RIP_ROUTE_REDISTRIBUTE           3
#define FRR_RIP_ROUTE_INTERFACE              4

#define FRR_RIP_METRIC_INFINITY             16

const char *
frr_rip_route_type_print (int sub_type)
{
  switch (sub_type)
    {
      case FRR_RIP_ROUTE_RTE:
	return "n";
      case FRR_RIP_ROUTE_STATIC:
	return "s";
      case FRR_RIP_ROUTE_DEFAULT:
	return "d";
      case FRR_RIP_ROUTE_REDISTRIBUTE:
	return "r";
      case FRR_RIP_ROUTE_INTERFACE:
	return "i";
      default:
	return "?";
    }
}

static int32 cmd_rip_str2L3IPv4(char *str, sys_l3_ipv4_t *pIp)
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

int32 cmd_l3_router_rip_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_routeV4_rip_cloce();
	}
	else
	{
		rc = sal_l3_routeV4_rip_open();
		vty->node = RIP_NODE;
	}
    return rc;
}

int32 rip_util_ipv4StrValidate_ret(const char *str, uint32 maskLen)
{
    const char *sp;
    int32 dots = 0;
    int32 nums = 0;
    char buf[4];
    uint32 groups[4];

    if (str == NULL)
        return FALSE;

    if (strlen(str) > CAPA_IPV4_STR_LEN)
        return FALSE;

    for (;;)
    {
        SYS_MEM_CLEAR(buf);

        sp = str;
        while (*str != '\0')
        {
            if (*str == '.')
            {
                if (dots >= 3)
                    return FALSE;

                if (*(str + 1) == '.')
                    return FALSE;

                if (*(str + 1) == '\0')
                    return FALSE;

                dots++;
                break;
            }
            if (!isdigit((int) *str))
                return FALSE;

            str++;
        }

        if (str - sp > 3)
            return FALSE;

        osal_strncpy(buf, sp, str - sp);

        groups[nums] = atoi(buf);

        if (groups[nums] > 255)
            return FALSE;

        nums++;

        if (*str == '\0')
            break;

        str++;
    }

    if (nums != 4 || dots != 3)
        return FALSE;

    if (groups[0] >= 1 && groups[0] <= 126)
    {
    	if(maskLen != 8)
        	return FALSE;
    }
    else if (groups[0] >= 128 && groups[0] <= 191)
    {
        if(maskLen != 16)
            return FALSE;
    }
    else if (groups[0] >= 192 && groups[0] <=223)
    {
        if(maskLen != 24)
            return FALSE;
    } 

    return TRUE;
}

int32 cmd_l3_rip_network_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 rc = SYS_ERR_OK;
	sys_l3_ipv4_t dstIp;
	sys_ipv4_t addr;
	sys_ipv4_t mask;
	sys_l3_ip_t tmpIpValue;
	char tmpIpAddress[SYS_BUF64_LEN] = {0};
	int32	sendVersion = 0;		
    int32	recvVersion = 0;

	cmd_rip_str2L3IPv4(argv[0], &dstIp);
	addr = dstIp.addr;
	mask = IPV4_PFX2MASK(dstIp.plen);

	tmpIpValue.v4.addr = addr;
	tmpIpValue.v4.plen = dstIp.plen;
	rc = sal_rip_version_get(&sendVersion, &recvVersion);
	if(SYS_ERR_OK == rc)
	{
		if(sendVersion == 1)
		{
			IPV4_L3IP2SUBNET(&tmpIpValue, &tmpIpValue);
			IPV42STR(tmpIpValue.v4.addr, tmpIpAddress);

			rc = rip_util_ipv4StrValidate_ret(tmpIpAddress, dstIp.plen);
			if(!rc)
			{
				XMORE("Invalid ip address\n");
				return rc;
			}
		}
	}

	if (CMD_IS_NO_FORM_EXIST())
	{
		rc = sal_l3_rip_network_set(DISABLED, addr, mask, NULL);
	}
	else
	{
		rc = sal_l3_rip_network_set(ENABLED, addr, mask, NULL);
	}
    return rc;
}

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

enum cmd_filter_type 
{
  CMD_FILTER_DENY,
  CMD_FILTER_PERMIT,
  CMD_FILTER_DYNAMIC
};

char *
filter_type_str (int type)
{
  switch (type)
    {
    case CMD_FILTER_PERMIT:
      return "permit";
      break;
    case CMD_FILTER_DENY:
      return "deny";
      break;
    case CMD_FILTER_DYNAMIC:
      return "dynamic";
      break;
    default:
      return "";
      break;
    }
}

int32 cmd_show_access_list(void)
{
	int32 rc = SYS_ERR_OK;
	int32 write = 0;
	ALIST_INFO_T alistInfo;
	DATA_OP_ENT op = DATA_OP_GETNEXT, op2 = DATA_OP_GETNEXT;
	FILTER_INFO_T filterInfo;
	char szTmpString[SYS_BUF256_LEN] = {0};

	FILTER_ZEBRA_INFO_T *filter;
	char buf[SYS_BUF64_LEN];
	int k = 0;

	SYS_MEM_CLEAR(alistInfo.alistName);

	alistInfo.alistName[0] = '\0';
	while (1)
	{
		rc = sal_access_list_get_alist_info(op, AFI_IP, &alistInfo);
		if (VOS_ERR_NONE != rc)
        {
            if (DATA_OP_GET == op)
            {
                return VOS_ERR_NOT_EXISTS;
            }
            else
            {
                break;
            }
        }
        
        osal_memcpy(filterInfo.alistName, alistInfo.alistName, sizeof(alistInfo.alistName));
        filterInfo.bfirst = TRUE;
        write = 1;
        while(1)
        {
        	rc = sal_access_list_get_filter_info(op2, AFI_IP, &filterInfo);
        	if (VOS_ERR_NONE != rc)
            {
                break;
            }
            SYS_MEM_CLEAR(szTmpString);
            
            if (write)
            {
            	sprintf(szTmpString,  "%s IP access list %s", "Zebra",alistInfo.alistName);
				XMORE("%s\n", szTmpString); 
		        write = 0;
	        }

			filter = &filterInfo.u.zfilter;

			k = 0;
			k += sprintf(szTmpString+k, "    %s%s", filter_type_str (filterInfo.type),
			         filterInfo.type == CMD_FILTER_DENY ? "  " : "");

			if (filter->prefixlen == 0 && ! filter->exact)
			{
			    k += sprintf(szTmpString+k, " any");
			}
			else
			{
				IPV42STR (filter->prefix4, buf);
			    k += sprintf(szTmpString+k, " %s/%d%s",
			         buf,
			         filter->prefixlen,
			         filter->exact ? " exact-match" : "");
			}
			XMORE("%s\n", szTmpString); 		
        }      
	}
	return SYS_ERR_OK;
}

int32 cmd_show_distribute_list(void)
{
    int rc;
    DISTRIBUTE_INFO_T distribute;
    char szTmpStr[SYS_BUF256_LEN];
    ulong_t k = 0; 
    char szName[SYS_BUF128_LEN];
	
	XMORE("\ndistribute list:\n");
    k = 0;
    distribute.ifname[0] = '\0';
    distribute.index = 0;
    while (1)
    {
        rc = sal_distribute_list_get_info(DATA_OP_GETNEXT, &distribute);
        if (VOS_ERR_NONE != rc)
        {
            break;
        }

		osal_memcpy(szName, distribute.ifname, sizeof(distribute.ifname));
        
        if ('\0' != distribute.list[TYPE_OUT][0] ||
            '\0' != distribute.prefix[TYPE_OUT][0])
        {
            k = 0;
            k += sprintf(szTmpStr+k, " %s outgoing filtered by", szName);
            if ('\0' != distribute.list[TYPE_OUT][0])
            {
                k += sprintf(szTmpStr+k, " %s", distribute.list[TYPE_OUT]);
            }
            if ('\0' != distribute.prefix[TYPE_OUT][0])
            {
                k += sprintf(szTmpStr+k, "%s (prefix-list) %s",
                        ('\0' != distribute.list[TYPE_OUT][0]) ? "," : "",
                        distribute.prefix[TYPE_OUT]);
                
            }
            k += sprintf(szTmpStr+k, "%s", RN); 
			XMORE("%s\n", szTmpStr);            
            
        }
        
        if ('\0' != distribute.list[TYPE_IN][0] ||
            '\0' != distribute.prefix[TYPE_IN][0])
        {
            k = 0;
            k += sprintf(szTmpStr+k, "    %s incoming filtered by", szName);
            if ('\0' != distribute.list[TYPE_IN][0])
            {
                k += sprintf(szTmpStr+k, " %s", distribute.list[TYPE_IN]);
            }
            if ('\0' != distribute.prefix[TYPE_IN][0])
            {
                k += sprintf(szTmpStr+k, "%s (prefix-list) %s",
                        ('\0' != distribute.list[TYPE_IN][0]) ? "," : "",
                        distribute.prefix[TYPE_IN]);
                
            }
            k += sprintf(szTmpStr+k, "%s", RN);
            
			XMORE("%s\n", szTmpStr);           
        }
      
    }
	return SYS_ERR_OK;

}

int32 cmd_rip_debug(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 			rc = SYS_ERR_OK;
	char *pDebugTypeStr = NULL;

    pDebugTypeStr = argv[0];
    
    SYS_PARAM_CHK((NULL == pDebugTypeStr), SYS_ERR_NULL_POINTER);
    if (CMD_IS_NO_FORM_EXIST())
    {
    	rc = sal_rip_debug(FALSE, pDebugTypeStr);
    }
    else
    {
    	rc = sal_rip_debug(TRUE, pDebugTypeStr);
    }
    
    if(SYS_ERR_OK != rc)
    {
    	return SYS_ERR_FAILED;
    }

	return rc;
}

int32 cmd_rip_log_to_fileName(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 			rc = SYS_ERR_OK;
	char *pModeTypeStr = NULL;

    pModeTypeStr = argv[0];
    
    SYS_PARAM_CHK((NULL == pModeTypeStr), SYS_ERR_NULL_POINTER);
    if (CMD_IS_NO_FORM_EXIST())
    {
    	if (pModeTypeStr[0] == 'r')
    	{
    		rc = sal_rip_log_to_fileName(FALSE);
    	}
    	else
    	{
    		printf("no support\n");
    		return SYS_ERR_FAILED;
    	}
    }
    else
    {
    	if (pModeTypeStr[0] == 'r')
    	{
    		rc = sal_rip_log_to_fileName(TRUE);
    	}
    	else
    	{
    		printf("no support\n");
    		return SYS_ERR_FAILED;
    	}
    }
    
    if(SYS_ERR_OK != rc)
    {
    	return SYS_ERR_FAILED;
    }

	return rc;
}

int32 cmd_rip_log_to_stdout(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 			rc = SYS_ERR_OK;
	char *pModeTypeStr = NULL;

    pModeTypeStr = argv[0];
    
    SYS_PARAM_CHK((NULL == pModeTypeStr), SYS_ERR_NULL_POINTER);
    if (CMD_IS_NO_FORM_EXIST())
    {
    	if (pModeTypeStr[0] == 'r')
    	{
    		rc = sal_rip_log_to_stdout(FALSE);
    	}
    	else
    	{
    		printf("no support\n");
    		return SYS_ERR_FAILED;
    	}
    }
    else
    {
    	if (pModeTypeStr[0] == 'r')
    	{
    		rc = sal_rip_log_to_stdout(TRUE);
    	}
    	else
    	{
    		printf("no support\n");
    		return SYS_ERR_FAILED;
    	}
    }
    
    if(SYS_ERR_OK != rc)
    {
    	return SYS_ERR_FAILED;
    }

	return rc;
}

int32 cmd_show_rip_status(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 	rc = SYS_ERR_OK;
    int32	bEnable = FALSE;
    int32	sendVersion = 0;		
    int32	recvVersion = 0;
    RIP_ENABLE_NW_INFO_T ripEnableNwInfo;
    char ripNetworkIp[SYS_BUF32_LEN] = {0};
    int32 ripNWMaskLen = 0;
    RIP_INFO_T 		ripInfo;
    
    rc = sal_rip_status_get(&bEnable);
    if(SYS_ERR_OK != rc)
    {
    	return SYS_ERR_FAILED;
    }
    if(FALSE == bEnable)
    {
    	XMORE("  RIP protocol is not active!\n");
    	return SYS_ERR_OK;
    }
	XMORE("\n");
	XMORE("  Rip status     : on\n");
	rc = sal_rip_version_get(&sendVersion, &recvVersion);
	if(SYS_ERR_OK != rc)
    {
    	return SYS_ERR_FAILED;
    }
    if(3 == recvVersion)
    {
    	XMORE("  Rip version    : V%d (send V%d, receive V1/2)\n", sendVersion, sendVersion);
    }
    else
    {
    	XMORE("  Rip version    : V%d (send V%d, receive V%d)\n", sendVersion, sendVersion, recvVersion);
    }

    SYS_MEM_CLEAR(ripInfo);
    rc = sal_rip_information_info_get(&ripInfo);
    if(SYS_ERR_OK != rc)
    {
    	XMORE("	RIP protocol is not active!\n");
    	return SYS_ERR_OK;
    }

    XMORE("  Updates time   : %d sec\n",ripInfo.update_time);
    XMORE("  Age  time      : %d sec\n", ripInfo.timeout_time);
    XMORE("  Garbage-collect time  : %d sec\n", ripInfo.garbage_time);
    XMORE("  Default redistribution metric : %d\n",ripInfo.default_metric);
    
	SYS_MEM_CLEAR(ripEnableNwInfo);
	ripEnableNwInfo.ulDestIp = 0;
    ripEnableNwInfo.ulDestMask = 0xffffffff;
    XMORE("  Routing for Networks:\n");
    while (1)
    {
        rc = sal_rip_enable_network_info_get(DATA_OP_GETNEXT, &ripEnableNwInfo);
        ripEnableNwInfo.ulDestIp = htonl(ripEnableNwInfo.ulDestIp);
        IPV42STR(ripEnableNwInfo.ulDestIp, ripNetworkIp);
        ripNWMaskLen = IPV4_MASK2PFX(ripEnableNwInfo.ulDestMask);
        if (SYS_ERR_OK != rc)
        {
            break;
        }
        XMORE("      %s/%d\n", ripNetworkIp, ripNWMaskLen);
    }
    rc = cmd_show_distribute_list();
    rc = cmd_show_access_list();
    rc = SYS_ERR_OK;
	XMORE("\n");

	return rc;
}

uint32 cmd_bool_interface_ip_in_rip_network(uint32 ipPrefix, uint32 ipPreLen)
{
	RIP_ENABLE_NW_INFO_T ripEnableNwInfo;
	int32 	rc = SYS_ERR_OK;
	sys_l3_ip_t tmpIpValue;
	int32 ripNWMaskLen = 0;

	SYS_MEM_CLEAR(ripEnableNwInfo);
	ripEnableNwInfo.ulDestIp = 0;
    ripEnableNwInfo.ulDestMask = 0xffffffff;

	while (1)
	{
		rc = sal_rip_enable_network_info_get(DATA_OP_GETNEXT, &ripEnableNwInfo);
		if(SYS_ERR_OK != rc)
		{
			return 0;
		}
		ripEnableNwInfo.ulDestIp = htonl(ripEnableNwInfo.ulDestIp);
		ripNWMaskLen = IPV4_MASK2PFX(ripEnableNwInfo.ulDestMask);
		tmpIpValue.v4.addr = ripEnableNwInfo.ulDestIp;
		tmpIpValue.v4.plen = ripNWMaskLen;
		IPV4_L3IP2SUBNET(&tmpIpValue, &tmpIpValue);
		if(IS_IPV4_EQUAL(tmpIpValue.v4.addr, ipPrefix))
		{
			return TRUE;
		}
		
	}
	return 0;
}

int32 cmd_show_rip_route_information(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 	             rc = SYS_ERR_OK;
	RIP_IP_ROUTE_STRC    ripRoute;
    DATA_OP_ENT          op;
	char        szDstIP[ 16 ] = { 0 };
	char        szTmpString[ 512 ] = { 0 };
	char        szNexthop[ 16 ] = { 0 };
	char        szFrom[ 16 ] = { 0 };
	int32 		len;
	int32		k = 0;
	int32 		ulPrintNum = 0;
	int32 		i = 0;

	SYS_MEM_CLEAR(ripRoute);
	op = DATA_OP_GETNEXT;
	ripRoute.ulDestIp = 0;
	ripRoute.prefixlen = 32;
	XMORE("  Codes: R - RIP, C - connected, S - Static, O - OSPF, B - BGP\n"
            "	Sub-codes:\n"
            "      (n) - normal, (s) - static, (d) - default, (r) - redistribute,\n"
            "      (i) - interface\n\n"
            "     Network            Next Hop         Metric   From                 Tag   Time\n");

	while ( 1 )
    {
        rc = sal_rip_ip_route_get(op, &ripRoute);
        if ( SYS_ERR_OK != rc )
        {
            break;
        }
        SYS_MEM_CLEAR(szTmpString);
        ulPrintNum++;
        ripRoute.ulDestIp = htonl(ripRoute.ulDestIp);
        IPV42STR(ripRoute.ulDestIp, szDstIP);

        len = sprintf(szTmpString, "%c(%s) %s/%d",
               ripRoute.cType,
               frr_rip_route_type_print (ripRoute.sub_type),
               szDstIP, ripRoute.prefixlen);
   	       
        k = len;
        len = 24 - len; 
        if (len > 0)
        {
            k += sprintf(szTmpString+k, "%*s", len, " ");
        }
        
        for ( i = 0; i < ripRoute.ulNexthopCnt; i++ )
        {
            IPV42STR(ripRoute.ulNexthop[i], szNexthop);
            
            if (ripRoute.ulNexthop[i]) 
                k += sprintf(szTmpString+k, "%-20s %2d ", szNexthop, ripRoute.metric);
            else
                k += sprintf(szTmpString+k, "0.0.0.0              %2d ", ripRoute.metric);
        }

        if ((ripRoute.cType == 'R') && 
            (ripRoute.sub_type == FRR_RIP_ROUTE_RTE))
        {
            ripRoute.from = htonl(ripRoute.from);
            IPV42STR(ripRoute.from, szFrom);
            
            k += sprintf(szTmpString+k, "  %-20s ", szFrom);
            k += sprintf(szTmpString+k, "%3d ", ripRoute.tag);
            k += sprintf(szTmpString+k, "%5s", ripRoute.szTime);
        }
        else if (ripRoute.metric == FRR_RIP_METRIC_INFINITY)
        {
            k += sprintf(szTmpString+k, "  %-20s ", "self");
            k += sprintf(szTmpString+k, "%3d ", ripRoute.tag);
            k += sprintf(szTmpString+k, "%5s", ripRoute.szTime);
        }
        else
        {
            if (ripRoute.external_metric)
            {
                len = sprintf(szTmpString+k, "  self (%s:%d)", 
                               ripRoute.szProto,
                               ripRoute.external_metric);
                k += len;               
                len = 21 - len;
                if (len > 0)
                    k += sprintf(szTmpString+k, "%*s", len, " ");
            }
            else
                k += sprintf(szTmpString+k, "  %-20s ", "self");

            k += sprintf(szTmpString+k, "%3d", ripRoute.tag);
        }
        
        XMORE("%s\n", szTmpString);  
    }

    XMORE("\nPrinted entries: %d\n", ulPrintNum);
	rc = SYS_ERR_OK;
	return rc;
}

int32 cmd_l3_router_rip_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
	int rc = SYS_ERR_OK;
	sys_enable_t ripEnable;
	RIP_ENABLE_NW_INFO_T ripEnableNwInfo;
	char ripNetworkIp[SYS_BUF32_LEN];
	int32 ripNWMaskLen = 0;
	int32	sendVersion = 0;		
    int32	recvVersion = 0;

	DISTRIBUTE_INFO_T distribute;
	char szName[SYS_BUF128_LEN];
	sys_l3_intfId_t intfId;

	ALIST_INFO_T alistInfo;
	DATA_OP_ENT op = DATA_OP_GETNEXT, op2 = DATA_OP_GETNEXT;
	FILTER_INFO_T filterInfo;
	FILTER_ZEBRA_INFO_T *filter;
	char buf[SYS_BUF64_LEN];

    SYS_ERR_CHK(sal_rip_status_get((int32*)&ripEnable));
    if(ripEnable)
    {
    	SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "rip\n");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    else 
    {
		
		return SYS_ERR_OK;
    }
    
    while (1)
    {
        rc = sal_rip_enable_network_info_get(DATA_OP_GETNEXT, &ripEnableNwInfo);
        if (SYS_ERR_OK != rc)
        {
            break;
        }
        ripEnableNwInfo.ulDestIp = htonl(ripEnableNwInfo.ulDestIp);
        SYS_MEM_CLEAR(ripNetworkIp);
        IPV42STR(ripEnableNwInfo.ulDestIp, ripNetworkIp);
        ripNWMaskLen = IPV4_MASK2PFX(ripEnableNwInfo.ulDestMask);
        
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, " network %s/%d\n", ripNetworkIp, ripNWMaskLen);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
	rc = sal_rip_version_get(&sendVersion, &recvVersion);
	if(SYS_ERR_OK == rc)
	{
		if(sendVersion != 2)
		{
			SYS_MEM_CLEAR(cmd);
			osal_sprintf(cmd, " version %d\n", sendVersion);
			CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
		}
	}
		  
	distribute.ifname[0] = '\0';
	distribute.index = 0;
	while (1)
	{
		rc = sal_distribute_list_get_info(DATA_OP_GETNEXT, &distribute);
		if (VOS_ERR_NONE != rc)
		{
			break;
		}

		osal_memcpy(szName, distribute.ifname, sizeof(distribute.ifname));
		STR2INTFID(szName, &intfId);
		if ('\0' != distribute.list[TYPE_OUT][0] ||
			'\0' != distribute.prefix[TYPE_OUT][0])
		{
			if ('\0' != distribute.list[TYPE_OUT][0])
			{
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, "distribute-list %s out interface vlan %u\n", distribute.list[TYPE_OUT], intfId.id);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);					
			}							
		}
		
		if ('\0' != distribute.list[TYPE_IN][0] ||
			'\0' != distribute.prefix[TYPE_IN][0])
		{
			if ('\0' != distribute.list[TYPE_IN][0])
			{
				SYS_MEM_CLEAR(cmd);
				osal_sprintf(cmd, "distribute-list %s in interface vlan %u\n", distribute.list[TYPE_IN], intfId.id);
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);	
			}		   
		}
	  
	}
	
	SYS_MEM_CLEAR(alistInfo.alistName);

	alistInfo.alistName[0] = '\0';
	while (1)
	{
		rc = sal_access_list_get_alist_info(op, AFI_IP, &alistInfo);
		if (VOS_ERR_NONE != rc)
        {
            if (DATA_OP_GET == op)
            {
                return VOS_ERR_NOT_EXISTS;
            }
            else
            {
                break;
            }
        }
        
        osal_memcpy(filterInfo.alistName, alistInfo.alistName, sizeof(alistInfo.alistName));
        filterInfo.bfirst = TRUE;

        while(1)
        {
        	rc = sal_access_list_get_filter_info(op2, AFI_IP, &filterInfo);
        	if (VOS_ERR_NONE != rc)
            {
                break;
            }

			filter = &filterInfo.u.zfilter;
			if (filter->prefixlen == 0 && ! filter->exact)
			{
				SYS_MEM_CLEAR(cmd);
			    osal_sprintf(cmd, "access-list %s %s any\n", alistInfo.alistName, filter_type_str (filterInfo.type));
			    CMD_WRITE_TO_FILE_INDENT1(fp, cmd);	
			}
			else
			{
				SYS_MEM_CLEAR(cmd);
				IPV42STR (filter->prefix4, buf);
			    osal_sprintf(cmd, "access-list %s %s %s/%d %s\n", alistInfo.alistName, filter_type_str (filterInfo.type), buf, filter->prefixlen, filter->exact ? " exact-match" : "");
				CMD_WRITE_TO_FILE_INDENT1(fp, cmd);	
			}
		
        }      
	}

    return SYS_ERR_OK;

}

