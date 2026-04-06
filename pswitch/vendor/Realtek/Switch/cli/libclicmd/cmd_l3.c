
#include <libsal/sal_l3.h>
#include <libcmd/cmd.h>

static int32 _cmd_l3_str2L3IPv4(char *str, sys_l3_ipv4_t *pIp)
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

int32 cmd_l3_intfIPv4_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char buf[SYS_BUF64_LEN];
    char ipStr[CAPA_IPV4_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    uint32 i = 0;
    uint32 showVlan = TRUE;
    uint32 showLo = TRUE;
    sys_vlanmask_t selVmsk;
    sys_enable_t adminEnbl;
    sys_l3_ipv4Db_t *pCurr = NULL;
    sys_vid_t dfltVid = 0;
	sys_l3_ipv4_t firstIpv4;

    if (0 == argc)
    {
        VLANMASK_SET_ALL(selVmsk);
    }
    else
    {
        if ('v' == tolower(argv[0][0]))
        {
            SYS_ERR_CHK(STR2VMSK(argv[0], &selVmsk));
            showLo = FALSE;
        }
        else if ('l' == tolower(argv[0][0]))
        {
            sys_l3_intfId_t loIntfId;

            SYS_ERR_CHK(INTFNAME2ID(argv[0], &loIntfId));

            showVlan = FALSE;
        }
    }

    sal_vlan_defaultVid_get(&dfltVid);

    XMORE("\n");
    XMORE("IP Address         I/F        I/F Status Type    Status    Roles\n");
    XMORE("                              admin/oper\n");
    XMORE("------------------ ---------- ---------- ------- ------ ---------\n");

    FOR_EACH_L3_IPV4(i, pCurr)
    {
        if ((SYS_L3_INTFTYPE_LO == pCurr->intfId.type) && !showLo)
            continue;

        if ((SYS_L3_INTFTYPE_VLAN == pCurr->intfId.type)
                && (!VLANMASK_IS_VIDSET(selVmsk, pCurr->intfId.id) || !showVlan))
            continue;

        SYS_MEM_CLEAR(buf);
        IPV42STR(pCurr->ip.addr, ipStr);
        osal_sprintf(buf, "%s/%d", ipStr, pCurr->ip.plen);
        XMORE("%-18s ", buf);

        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&pCurr->intfId, intfName);
        XMORE("%-10s ", intfName);

        sal_l3_adminIntfEnbl_get(&pCurr->intfId, &adminEnbl);
        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s/%s",
                        (adminEnbl) ? "UP" : "DOWN",
                        (IS_L3_INTF_LINKUP(&pCurr->intfId)) ? "UP" : "DOWN");
        XMORE("%-10s ", buf);

        if (SYS_L3_INTFTYPE_VLAN == pCurr->intfId.type && pCurr->intfId.id == dfltVid)
        {
            sys_enable_t dhcpEnbl;

            sal_l3_adminIntfDhcpV4_get(&pCurr->intfId, &dhcpEnbl);

            if (SYS_L3_IPTYPE_STATIC == pCurr->list.type)
            {
                if (ENABLED == dhcpEnbl)
                    XMORE("%-7s ", "Default");
                else
                    XMORE("%-7s ", "Static");
            }
            else
            {
                XMORE("%-7s ", "DHCP");
            }

        }
        else
        {
            XMORE("%-7s ", (SYS_L3_IPTYPE_STATIC == pCurr->list.type) ? "Static" : "DHCP");
        }

        XMORE("%-7s ", (IS_IPV4_ALLZERO(pCurr->ip.addr)) ? "Not Received" : "Valid");

		SYS_MEM_CLEAR(firstIpv4);
        if(sal_l3_intf_primary_IPv4_find(&pCurr->intfId, &firstIpv4) == SYS_ERR_OK)
        {
			if(osal_memcmp(&firstIpv4, &pCurr->ip, sizeof(sys_l3_ipv4_t)) == 0)
			{
				XMORE("primary ");
			}
			else
			{
				XMORE("sub ");
			}
        }
        else 
        {
			XMORE("N/A ");
        }

        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_l3_routeV4_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char ipStr[CAPA_IPV4_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    uint32 i = 0;
    uint32 isAll = FALSE;
    uint32 isConnected = FALSE;
    uint32 isStatic = FALSE;
    sys_l3_routeV4InfoDb_t infoDb;

    if (argc > 0)
    {
        switch (argv[0][0])
        {
            case 'd':
                isAll = TRUE;
                break;

            case 'c':
                isConnected = TRUE;
                break;

            case 's':
                isStatic = TRUE;
                break;

            default:
                break;
        }
    }

    SYS_MEM_CLEAR(infoDb);
    SYS_ERR_CHK(sal_l3_routeV4Info_get(&infoDb));

    if (isStatic)
        XMORE("Codes: A - active, I - inactive\n\n");
    else
    #if defined (CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
        XMORE("Codes: > - best, C - connected, S - static \n \r \n");
    #else
   	 	XMORE("Codes: > - best, C - connected, S - static R - rip \n \r O - ospf, I - isis, B - BGP\n\n");
    #endif

    for (i = 0; i < infoDb.num; i++)
    {
        if (isConnected && SYS_L3_ROUTETYPE_CONNECT != infoDb.info[i].type)
            continue;

        if (isStatic && (SYS_L3_ROUTETYPE_STATIC != infoDb.info[i].type))
            continue;

        if (!isAll && !isStatic && !infoDb.info[i].select)
            continue;

        if (isStatic)
        {
            if (TRUE == infoDb.info[i].select)
                XMORE("A");
            else
                XMORE("I");

            XMORE(" ");
        }
        else
        {
            
            switch (infoDb.info[i].type)
            {
                case SYS_L3_ROUTETYPE_KERNEL:
                    XMORE("D");
                    break;

                case SYS_L3_ROUTETYPE_CONNECT:
                    XMORE("C");
                    break;

                case SYS_L3_ROUTETYPE_STATIC:
                    XMORE("S");
                    break;
                    
                case SYS_L3_ROUTETYPE_RIP:
                    XMORE("R");
                    break;
                    
                case SYS_L3_ROUTETYPE_OSPF:
                    XMORE("O");
                    break;
                    
                case SYS_L3_ROUTETYPE_ISIS:
                    XMORE("I");
                    break;
                    
                case SYS_L3_ROUTETYPE_BGP:
                    XMORE("B");
                    break;

                default:
                    XMORE(" ");
                    break;
            }

            if (TRUE == infoDb.info[i].select)
                XMORE(">");
            else
                XMORE(" ");
        }

        XMORE(" ");

        XMORE(" ");

        SYS_MEM_CLEAR(ipStr);
        IPV42STR(infoDb.info[i].route.dst, ipStr);
        XMORE("%s/%d ", ipStr, infoDb.info[i].route.dstLen);

        if (SYS_L3_ROUTETYPE_CONNECT == infoDb.info[i].type)
        {
            XMORE("is directly connected, ");
        }
        else
        {
            XMORE("[%d/%d] ", infoDb.info[i].distance, infoDb.info[i].metric);

            XMORE("via ");

            SYS_MEM_CLEAR(ipStr);
            IPV42STR(infoDb.info[i].route.gw, ipStr);
            XMORE("%s, ", ipStr);
        }

        INTFID2NAME(&infoDb.info[i].route.ointf, intfName);
        XMORE("%s", intfName);

        XMORE("\n");
    }

    return SYS_ERR_OK;
}

static int32 _cmd_l3_intfIPv6_showById(sys_l3_intfId_t *pIntfId)
{
    char buf[SYS_BUF64_LEN];
    char buf2[SYS_BUF64_LEN];
    char ipStr[CAPA_IPV6_STR_LEN];
    uint32 glbAddrNum = 0;
    uint32 llAddrNum = 0;
    char intfName[CAPA_L3_INTFNAME_LEN];
    sys_enable_t adminEnbl = 0;
    sys_enable_t fwdEnbl = 0;
    sys_l3_ipv6Admin_t admin;
    sys_l3_ipv6Db_t *pCurr = NULL;
    sys_l3_ipv6DadSts_t dadSts = SYS_L3_IPV6_DADSTS_NONE;

    sal_l3_fwdIPv6Enable_get(&fwdEnbl);

    SYS_MEM_CLEAR(admin);
    sal_l3_adminIntfIPv6_get(pIntfId, &admin);

    SYS_MEM_CLEAR(intfName);
    INTFID2NAME(pIntfId, intfName);
    sal_l3_adminIntfEnbl_get(pIntfId, &adminEnbl);
    XMORE("%s is %s/%s\n",
              intfName,
              (adminEnbl) ? "up" : "down",
              (IS_L3_INTF_LINKUP(pIntfId)) ? "up" : "down");

    FOR_EACH_L3_IPV6_IN_INTF(pIntfId, pCurr)
    {
        if (SYS_L3_IPTYPE_V6LINKLOCAL != pCurr->list.type)
        {
            if (!((SYS_L3_IPTYPE_STATIC == pCurr->list.type) && IS_IPV6_LINKLOCAL(pCurr->ip.addr)))
                continue;
        }

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(pCurr->ip.addr, ipStr);
        XMORE("IPv6 is enabled, link-local address is %s", ipStr);

        sal_l3_intfIPv6DadStatus_get(pIntfId, &pCurr->ip, &dadSts);
        if (SYS_L3_IPV6_DADSTS_TENTATIVE == dadSts || SYS_L3_IPV6_DADSTS_DUPLICATE == dadSts)
            XMORE(" (%s)\n", text_ipv6_dadSts[dadSts].text);
        else
            XMORE("\n");

        llAddrNum++;
    }

    if (0 == llAddrNum)
    {
        sys_l3_ipv6_t sysLinkLocalAddr;

        SYS_MEM_CLEAR(sysLinkLocalAddr);

        sys_l3_ipv6SysLinkLocal_get(&sysLinkLocalAddr);

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(sysLinkLocalAddr.addr, ipStr);

        XMORE("IPv6 is enabled, link-local address is %s (TENTATIVE)\n", ipStr);
    }

    XMORE("IPv6 Forwarding is %s\n", text_enable[fwdEnbl].text);

    glbAddrNum = 0;
    FOR_EACH_L3_IPV6_IN_INTF(pIntfId, pCurr)
    {
        if (SYS_L3_IPTYPE_V6LINKLOCAL == pCurr->list.type)
            continue;

        if (0 == glbAddrNum)
        {
            XMORE("Global unicast address(es):\n");
            XMORE("IPv6 Global Address                        Type\n");
        }

        glbAddrNum++;

        SYS_MEM_CLEAR(buf);
        SYS_MEM_CLEAR(buf2);
        SYS_MEM_CLEAR(ipStr);

        IPV62STR(pCurr->ip.addr, ipStr);
        osal_sprintf(buf, "%s/%d", ipStr, pCurr->ip.plen);

        sal_l3_intfIPv6DadStatus_get(pIntfId, &pCurr->ip, &dadSts);
        if (SYS_L3_IPV6_DADSTS_TENTATIVE == dadSts || SYS_L3_IPV6_DADSTS_DUPLICATE == dadSts)
        {
            char tmp[SYS_BUF8_LEN];
            SYS_MEM_CLEAR(tmp);
            osal_strncpy(tmp, text_ipv6_dadSts[dadSts].text, 3);
            osal_sprintf(buf2, "%s (%s)", buf, tmp);
        }
        else
        {
            osal_strcpy(buf2, buf);
        }

        switch (pCurr->list.type)
        {
            case SYS_L3_IPTYPE_V6AUTOCONF:
                XMORE("%-42s %s\n", buf2, "Auto");
                break;

            case SYS_L3_IPTYPE_DHCP:
                XMORE("%-42s %s\n", buf2, "DHCP");
                break;

            default:
                XMORE("%-42s %s\n", buf2, "Manual");
                break;
        }
    }

    if (0 == glbAddrNum)
        XMORE("No global unicast address is configured\n");

    if (IS_L3_INTF_LINKUP(pIntfId))
    {
        sys_l3_ipv6MaddrInfo_t maddrs;
        uint32 j = 0;

        SYS_MEM_CLEAR(maddrs);

        sal_l3_intfIPv6Maddr_get(pIntfId, &maddrs);

        XMORE("Joined group address(es):\n");

        for (j = 0; j < maddrs.num; j++)
        {
            SYS_MEM_CLEAR(ipStr);

            IPV62STR(maddrs.addrs[j], ipStr);

            XMORE("%s\n", ipStr);
        }
    }

    XMORE("ND DAD is %s, number of DAD attempts: %d\n", (0 == admin.dadTxNum) ? "disabled" : "enabled", admin.dadTxNum);
    XMORE("Stateless autoconfiguration is %s\n", text_enable[admin.autoconfEnbl].text);

    return SYS_ERR_OK;
}

int32 cmd_l3_intfIPv6_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i = 0;
    int32 selId = -1;
    uint32 showVlan = TRUE;
    uint32 showLo = TRUE;
    sys_vlanmask_t selVmsk;
    sys_l3_intfId_t intfId;

    if (0 == argc)
    {
        VLANMASK_SET_ALL(selVmsk);
    }
    else
    {
        if ('v' == tolower(argv[0][0]))
        {
            showLo = FALSE;
            SYS_ERR_CHK(STR2VMSK(argv[0], &selVmsk));
        }
        else if ('l' == tolower(argv[0][0]))
        {
            showVlan = FALSE;
            SYS_ERR_CHK(INTFNAME2ID(argv[0], &intfId));
        }
    }

    SYS_MEM_CLEAR(intfId);

    if (showLo)
    {
        if (SYS_L3_INTFTYPE_LO == intfId.type)
            selId = intfId.id;

        intfId.type = SYS_L3_INTFTYPE_LO;

        for (i = 0; i < CAPA_L3_INTFLO_NUM; i++)
        {
            if (!(selId < 0 || (selId >= 0 && i == selId)))
                continue;

            intfId.id = i;

            if (!IS_L3_INTF_EXIST(&intfId))
            continue;

            if (!IS_L3_INTF_IPV6ENBL(&intfId))
                continue;

            _cmd_l3_intfIPv6_showById(&intfId);

            XMORE("\n");
        }
    }

    if (showVlan)
    {
        intfId.type = SYS_L3_INTFTYPE_VLAN;
        FOR_EACH_VID_IN_VLANMASK(i, selVmsk)
        {
            intfId.id = i;

            if (!IS_L3_INTF_EXIST(&intfId))
                continue;

            if (!IS_L3_INTF_IPV6ENBL(&intfId))
                continue;

            _cmd_l3_intfIPv6_showById(&intfId);

            XMORE("\n");
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_l3_DHCPv6_client_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_vlanmask_t	vlanmask;
	sys_vid_t vid;
	sys_l3_intfId_t intfId;
	sys_l3_ipv6Admin_t ipv6Admin;
	
	XMORE("\n");
	
    if (1 == argc)
    {
        SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));
    }
    else
    {
        SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vlanmask));
    }
	XMORE(" VLAN |        Stateless        |  Information Refresh Time  |  Minimum Information Refresh Time   \n");

    intfId.type = SYS_L3_INTFTYPE_VLAN;
    FOR_EACH_VID_IN_VLANMASK(vid, vlanmask)
    {
    	intfId.id = vid;
    	if (!IS_L3_INTF_EXIST(&intfId))
            continue;

        if (!IS_L3_INTF_IPV6ENBL(&intfId))
            continue;
        SYS_MEM_CLEAR(ipv6Admin);
        SYS_ERR_CONTINUE(sal_l3_adminIntfIPv6_get(&intfId, &ipv6Admin));

		XMORE(" %-4u          %7s             %-10u                   %-10u", vid, ipv6Admin.dhcp6c.stateless?"enable":"disable", ipv6Admin.dhcp6c.refreshtime, ipv6Admin.dhcp6c.min_refreshtime);
        XMORE("\n");
    }
	XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_l3_routeV6_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char ipStr[CAPA_IPV6_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    uint32 i = 0;
    uint32 isAll = FALSE;
    uint32 isConnected = FALSE;
    uint32 isStatic = FALSE;
    sys_l3_routeV6InfoDb_t infoDb;

    if (argc > 0)
    {
        switch (argv[0][0])
        {
            case 'd':
                isAll = TRUE;
                break;

            case 'c':
                isConnected = TRUE;
                break;

            case 's':
                isStatic = TRUE;
                break;

            default:
                break;
        }
    }

    SYS_MEM_CLEAR(infoDb);
    SYS_ERR_CHK(sal_l3_routeV6Info_get(&infoDb));

    if (isStatic)
        XMORE("Codes: A - active, I - inactive\n\n");
    else
        XMORE("Codes: > - best, C - connected, S - static\n\n");

    for (i = 0; i < infoDb.num; i++)
    {
        if (isConnected && SYS_L3_ROUTETYPE_CONNECT != infoDb.info[i].type)
            continue;

        if (isStatic && (SYS_L3_ROUTETYPE_STATIC != infoDb.info[i].type))
            continue;

        if (!isAll && !isStatic && !infoDb.info[i].select)
            continue;

        if (isStatic)
        {
            if (TRUE == infoDb.info[i].select)
                XMORE("A");
            else
                XMORE("I");

            XMORE(" ");
        }
        else
        {
            
            switch (infoDb.info[i].type)
            {
                case SYS_L3_ROUTETYPE_KERNEL:
                    XMORE("D");
                    break;

                case SYS_L3_ROUTETYPE_CONNECT:
                    XMORE("C");
                    break;

                case SYS_L3_ROUTETYPE_STATIC:
                    XMORE("S");
                    break;

                default:
                    XMORE(" ");
                    break;
            }

            if (TRUE == infoDb.info[i].select)
                XMORE(">");
            else
                XMORE(" ");
        }

        XMORE(" ");

        XMORE(" ");

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(infoDb.info[i].route.dst, ipStr);
        XMORE("%s/%d ", ipStr, infoDb.info[i].route.dstLen);

        if (SYS_L3_INTFTYPE_NULL == infoDb.info[i].route.ointf.type)
        {
            XMORE("reject ");
        }
        else if (SYS_L3_ROUTETYPE_CONNECT == infoDb.info[i].type)
        {
            XMORE("is directly connected, ");
        }
        else
        {
            if (IS_IPV6_ALLZERO(infoDb.info[i].route.dst))
                XMORE("[1/1] ");
            else
                XMORE("[%d/%d] ", infoDb.info[i].distance, infoDb.info[i].metric);

            XMORE("via ");

            SYS_MEM_CLEAR(ipStr);
            IPV62STR(infoDb.info[i].route.gw, ipStr);
            XMORE("%s, ", ipStr);
        }

        INTFID2NAME(&infoDb.info[i].route.ointf, intfName);
        XMORE("%s", intfName);

        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_l3_neigh_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 tokenCnt = CMD_TOKENNUM_GET(cmd->vline);
    sys_l3_intfId_t intf;

    SYS_MEM_CLEAR(intf);
    SYS_ERR_CHK(sal_l3_neigh_clear((tokenCnt == 2) ? SYS_L3_NEIGH_V4_DYNAMIC : SYS_L3_NEIGH_V6_DYNAMIC, intf));

    return SYS_ERR_OK;
}

int32 cmd_l3_neigh_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    FILE *fp = NULL;
    char buf[SYS_BUF256_LEN];
    sys_ipv6_t v6addr;
    char v6addrStr[CAPA_IPV6_STR_LEN];
    char *tok[6];
    char *ridx = NULL;
    char *vlan = NULL;
    char *ip = NULL;
    char *mac = NULL;
    uint32 i = 0;
    uint32 addrNum = 0;
    uint32 tokenCnt = 0;
    uint32 ridx_len = 0;
    uint32 isV4 = FALSE;
    char nudStr[8][SYS_BUF16_LEN] = {"Incomplete", "Reachable", "Stale", "Delay", "Probe", "Failed", "Permanent", " "};
    char *nudTmp = NULL;

    tokenCnt = CMD_TOKENNUM_GET(cmd->vline);

    if ('a' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
        isV4 = TRUE;

    for (i = 2; i < tokenCnt ; i++)
    {
        ridx = CMD_TOKENSTR_GET(cmd->vline, i);
        ridx_len = osal_strlen(ridx);

        if (0 == osal_strncmp(ridx, "vlan", ridx_len))
            vlan = CMD_TOKENSTR_GET(cmd->vline, ++i);
        else if (0 == osal_strncmp(ridx, "ip-address", ridx_len))
            ip = CMD_TOKENSTR_GET(cmd->vline, ++i);
        else if (0 == osal_strncmp(ridx, "ipv6-address", ridx_len))
            ip = CMD_TOKENSTR_GET(cmd->vline, ++i);
        else if (0 == osal_strncmp(ridx, "mac-address", ridx_len))
            mac = CMD_TOKENSTR_GET(cmd->vline, ++i);
    }

    if (isV4)
    {
        XMORE("  VLAN Interface    IP address        HW address       Status  \n");
        XMORE("----------------- --------------- ------------------ ----------\n");

        if (NULL == (fp = popen("cat /proc/"L3_NEIGH_V4_PROC_NAME, "r")))
            return SYS_ERR_OK;
    }
    else
    {
        XMORE("  VLAN Interface                  IPv6 address                 HW address       Status   Router State\n");
        XMORE("----------------- ---------------------------------------- ------------------ ---------- ------ ----------\n");

        if (NULL == (fp = popen("cat /proc/"L3_NEIGH_V6_PROC_NAME, "r")))
            return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    while (fgets(buf, sizeof(buf), fp))
    {
        tok[0] = strtok(buf, ", "); 

        if (!isdigit(tok[0][0])) 
            break;

        if (NULL != vlan && (0 != strcmp(tok[0], vlan))) 
            continue;

        tok[1] = strtok(NULL, ", "); 

        if (NULL != ip && !isV4)
        {
            SYS_MEM_CLEAR(v6addr);
            SYS_MEM_CLEAR(v6addrStr);
            STR2IPV6(tok[1], &v6addr);
            IPV62STR(v6addr, v6addrStr);

            if (0 != strcmp(v6addrStr, ip)) 
                continue;
        }
        else
        {
            if (NULL != ip && (0 != strcmp(tok[1], ip))) 
                continue;
        }

        tok[2] = strtok(NULL, ", "); 
        if (NULL != mac && (0 != strcmp(tok[2], mac))) 
            continue;

        tok[3] = strtok(NULL, ", "); 
        tok[4] = strtok(NULL, ", "); 
        tok[5] = strtok(NULL, ", "); 

        if (isV4)
            xmore_out("vlan %-12s %-15s %-18s %s\n", tok[0], tok[1], tok[2], (tok[3][0] == '1') ? "Static": "Dynamic");
        else
        {
            if (NULL == ip)
            {
                SYS_MEM_CLEAR(v6addr);
                SYS_MEM_CLEAR(v6addrStr);
                STR2IPV6(tok[1], &v6addr);
                IPV62STR(v6addr, v6addrStr);
            }

            switch (atoi(tok[5]))
            {
                case 0x1:
                    nudTmp = nudStr[0];
                    break;
                case 0x2:
                    nudTmp = nudStr[1];
                    break;
                case 0x4:
                    nudTmp = nudStr[2];
                    break;
                case 0x8:
                    nudTmp = nudStr[3];
                    break;
                case 0x10:
                    nudTmp = nudStr[4];
                    break;
                case 0x20:
                    nudTmp = nudStr[5];
                    break;
                case 0x80:
                    nudTmp = nudStr[6];
                    break;
                default:
                    nudTmp = nudStr[7];
            }

            xmore_out("vlan %-12s %-40s %-18s %-10s %-6s %s\n", tok[0], v6addrStr, tok[2],
                (tok[3][0] == '3') ? "Static": "Dynamic", (tok[4][0] == '1') ? "Yes": "No", nudTmp);
        }

        SYS_MEM_CLEAR(buf);
        addrNum++;
    }

    pclose(fp);

    XMORE("\nTotal number of entries: %u\n\n", addrNum);

    return SYS_ERR_OK;
}

int32 cmd_l3_arpConf_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 arpAgeTime = 0;
    uint32 arpRespTime = 0;
    uint32 arpRetryTimes = 0;
    uint32 arpCacheSize = 0;
    sys_enable_t renew = DISABLED;
    sys_enable_t arpProxy = DISABLED;

    SYS_ERR_CHK(sal_l3_neigh_ageTime_get(&arpAgeTime));
    SYS_ERR_CHK(sal_l3_neigh_respTime_get(&arpRespTime));
    SYS_ERR_CHK(sal_l3_neigh_retryTimes_get(&arpRetryTimes));
    SYS_ERR_CHK(sal_l3_neigh_cacheSize_get(&arpCacheSize));
    SYS_ERR_CHK(sal_l3_neigh_renew_get(&renew));
    SYS_ERR_CHK(sal_l3_arpProxy_get(0, &arpProxy));

    XMORE("\nGlobal configuration:\n");
    XMORE("ARP timeout:  %u Seconds\n", arpAgeTime);
    XMORE("ARP cache:    %u\n", arpCacheSize);
#if 0
    XMORE("ARP response: %u Seconds\n", arpRespTime);
    XMORE("ARP retry:    %u times\n", arpRetryTimes);
    XMORE("ARP renew:    %s\n\n", text_enable[renew].text);
#endif

    return SYS_ERR_OK;
}

int32 cmd_l3_intfEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;
    sys_vid_t vid;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
    {
        FOR_EACH_VID_IN_VLANMASK(vid, g_selVmsk)
        {
            id.id = vid;

            if (CMD_IS_NO_FORM_EXIST())
            {
                SYS_ERR_CHK(sal_l3_adminIntfEnbl_set(&id, ENABLED));

                continue;
            }

            SYS_ERR_CHK(sal_l3_adminIntfEnbl_set(&id, DISABLED));

            return SYS_ERR_OK;
        }
    }
    else if (SYS_L3_INTFTYPE_LO == id.type)
    {
        id.id = g_selLo;

        if (CMD_IS_NO_FORM_EXIST())
        {
            SYS_ERR_CHK(sal_l3_adminIntfEnbl_set(&id, ENABLED));

            return SYS_ERR_OK;
        }

        SYS_ERR_CHK(sal_l3_adminIntfEnbl_set(&id, DISABLED));
    }

    return SYS_ERR_OK;

}

int32 cmd_l3_intfExist_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;
    sys_vid_t vid;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
    {
        FOR_EACH_VID_IN_VLANMASK(vid, g_selVmsk)
        {
            id.id = vid;

            if (CMD_IS_NO_FORM_EXIST())
            {
                SYS_ERR_CHK(sal_l3_adminIntfExist_set(&id, DISABLED));

                continue;
            }

            SYS_ERR_CHK(sal_l3_adminIntfExist_set(&id, ENABLED));

            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_OK;

}

int32 cmd_l3_fwdIPv4Enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_l3_fwdIPv4Enable_set(enable));

    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_L3_ROUTE_RIP
int32 l3_bool_interface_ip_in_rip_network(sys_l3_intfId_t id)
{
        sys_l3_ipv4Db_t *pCurr = NULL;
    	sys_l3_ip_t tmpIpValue;
		int ret = SYS_ERR_OK;
    	
    	SYS_MEM_CLEAR(tmpIpValue);
    	FOR_EACH_L3_IPV4_IN_INTF(&id, pCurr)
    	{
    		if (pCurr->list.type != SYS_L3_IPTYPE_STATIC)
            	continue;
        	tmpIpValue.v4.addr = pCurr->ip.addr;
        	tmpIpValue.v4.plen = pCurr->ip.plen;
        	IPV4_L3IP2SUBNET(&tmpIpValue, &tmpIpValue);
        	ret = cmd_bool_interface_ip_in_rip_network(tmpIpValue.v4.addr, tmpIpValue.v4.plen);
       		if(ret)
       		{
       			XMORE("The segment of the interface in the RIP\n");
       			return SYS_ERR_FAILED;
       		}
    	}
    	return SYS_ERR_OK;
}
#endif

int32 cmd_l3_intIPv4Addr_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;
    sys_l3_ipv4_t ip;
    sys_ipv4_t mask;
    char addr[CAPA_IPV4_STR_LEN];
    char maskDel[CAPA_IPV4_STR_LEN];
    char gateway[CAPA_IPV4_STR_LEN];

    SYS_MEM_CLEAR(addr);
    SYS_MEM_CLEAR(maskDel);
    SYS_MEM_CLEAR(gateway);
    SYS_MEM_CLEAR(id);
    SYS_MEM_CLEAR(ip);
    SYS_MEM_CLEAR(addr);
    SYS_MEM_CLEAR(maskDel);
    SYS_MEM_CLEAR(gateway);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;
    else if (SYS_L3_INTFTYPE_LO == id.type)
        id.id = g_selLo;

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (CMD_TOKENSTR_GET(cmd->vline, 2)[0] == 'd')
        {
            SYS_ERR_CHK(sal_l3_adminIntfDhcpV4_set(&id, DISABLED));
        }

        if (argc > 0)
        {
        	#ifdef CONFIG_SYS_L3_ROUTE_RIP
			SYS_ERR_CHK(l3_bool_interface_ip_in_rip_network(id));
        	#endif
            STR2IPV4(argv[0], &ip.addr);
            SYS_ERR_CHK(sal_l3_intfStaticIPv4_del(&id, &ip));
        }
        else
        {
        	#ifdef CONFIG_SYS_L3_ROUTE_RIP
			SYS_ERR_CHK(l3_bool_interface_ip_in_rip_network(id));
        	#endif
            SYS_ERR_CHK(sal_l3_intfStaticIPv4_clear(&id));
        }

        return SYS_ERR_OK;
    }

    if (CMD_TOKENSTR_GET(cmd->vline, 1)[0] == 'd')
    {
        SYS_ERR_CHK(sal_l3_adminIntfDhcpV4_set(&id, ENABLED));

        return SYS_ERR_OK;
    }

    if (1 == argc)
    {
        _cmd_l3_str2L3IPv4(argv[0], &ip);
    }
    else
    {
        STR2IPV4(argv[0], &ip.addr);
        STR2IPV4(argv[1], &mask);
        ip.plen = IPV4_MASK2PFX(mask);
    }

    SYS_ERR_CHK(sal_l3_intfStaticIPv4_add(&id, &ip));
    
    return SYS_ERR_OK;

}

int32 cmd_l3_intIPv4Addr_sub_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;
    sys_l3_ipv4_t ip;
    sys_ipv4_t mask;
    char addr[CAPA_IPV4_STR_LEN];
    char maskDel[CAPA_IPV4_STR_LEN];
    char gateway[CAPA_IPV4_STR_LEN];

    SYS_MEM_CLEAR(addr);
    SYS_MEM_CLEAR(maskDel);
    SYS_MEM_CLEAR(gateway);
    SYS_MEM_CLEAR(id);
    SYS_MEM_CLEAR(ip);
    SYS_MEM_CLEAR(addr);
    SYS_MEM_CLEAR(maskDel);
    SYS_MEM_CLEAR(gateway);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;
    else if (SYS_L3_INTFTYPE_LO == id.type)
        id.id = g_selLo;

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (argc > 0)
        {
        	#ifdef CONFIG_SYS_L3_ROUTE_RIP
			SYS_ERR_CHK(l3_bool_interface_ip_in_rip_network(id));
        	#endif
            STR2IPV4(argv[0], &ip.addr);
            SYS_ERR_CHK(sal_l3_intfStaticIPv4_sub_del(&id, &ip));
        }

        return SYS_ERR_OK;
    }

    if (1 == argc)
    {
        _cmd_l3_str2L3IPv4(argv[0], &ip);
    }
    else
    {
        STR2IPV4(argv[0], &ip.addr);
        STR2IPV4(argv[1], &mask);
        ip.plen = IPV4_MASK2PFX(mask);
    }

    SYS_ERR_CHK(sal_l3_intfStaticIPv4_sub_add(&id, &ip));
    return SYS_ERR_OK;

}

int32 cmd_l3_staticRouteV4_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_routeV4_t route;
    sys_l3_ipv4_t dstIp;
    uint32 usingMask = FALSE;

    SYS_MEM_CLEAR(route);
    SYS_MEM_CLEAR(dstIp);

    if (NULL == osal_strchr(argv[0], '/'))
        usingMask = TRUE;

    if (usingMask)
    {
        STR2IPV4(argv[0], &route.dst);
        STR2IPV4(argv[1], &dstIp.addr);
        route.dstLen = IPV4_MASK2PFX(dstIp.addr);
    }
    else
    {
        _cmd_l3_str2L3IPv4(argv[0], &dstIp);
        route.dst = dstIp.addr;
        route.dstLen = dstIp.plen;
    }

    if (CMD_IS_NO_FORM_EXIST())
    {
        uint32 delAll = FALSE;

        if (usingMask)
        {
            if (argc > 2)
                STR2IPV4(argv[2], &route.gw);
            else
                delAll = TRUE;
        }
        else
        {
            if (argc > 1)
                STR2IPV4(argv[1], &route.gw);
            else
                delAll = TRUE;
        }

        if (delAll)
        {
            uint32 i = 0;
            uint32 delNum = 0;
            sys_l3_staticRouteV4_t rules;

            SYS_MEM_CLEAR(rules);

            sal_l3_staticRouteV4_get(&rules);

            for (i = 0; i < CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX; i++)
            {
                if (IS_IPV4_ALLZERO(rules.route[i].gw))
                    continue;

                if (rules.route[i].dst == route.dst
                        && rules.route[i].dstLen == route.dstLen)
                {
                    route.gw = rules.route[i].gw;

                    SYS_ERR_CONTINUE(sal_l3_staticRouteV4_del(&route));

                    delNum++;
                }
            }

            XMORE("%s%d entries were deleted\n", (0 == delNum) ? "No such entry, " : "", delNum);
        }
        else
        {
            SYS_ERR_CHK(sal_l3_staticRouteV4_del(&route));

            XMORE("1 entry was deleted\n");
        }

        return SYS_ERR_OK;
    }

    if (usingMask)
    {
        STR2IPV4(argv[2], &route.gw);

        if (argc > 4)
        {
            if ('m' == CMD_TOKENSTR_GET(cmd->vline, 5)[0])
            {
                route.pri = SYS_STR2UINT(argv[3]);
                osal_strcpy(route.descp, argv[4]);
            }
            else
            {
                route.pri = SYS_STR2UINT(argv[4]);
                osal_strcpy(route.descp, argv[3]);
            }
        }
        else if (argc > 3)
        {
            if ('m' == CMD_TOKENSTR_GET(cmd->vline, 5)[0])
            {
                route.pri = SYS_STR2UINT(argv[3]);
            }
            else
            {
                osal_strcpy(route.descp, argv[3]);
                route.pri = DFLT_L3_STATICROUTE_METRIC;
            }
        }
        else
        {
            route.pri = DFLT_L3_STATICROUTE_METRIC;
        }
    }
    else
    {
        STR2IPV4(argv[1], &route.gw);

        if (argc > 3)
        {
            if ('m' == CMD_TOKENSTR_GET(cmd->vline, 4)[0])
            {
                route.pri = SYS_STR2UINT(argv[2]);
                osal_strcpy(route.descp, argv[3]);
            }
            else
            {
                route.pri = SYS_STR2UINT(argv[3]);
                osal_strcpy(route.descp, argv[2]);
            }
        }
        else if (argc > 2)
        {
            if ('m' == CMD_TOKENSTR_GET(cmd->vline, 4)[0])
            {
                route.pri = SYS_STR2UINT(argv[2]);
            }
            else
            {
                osal_strcpy(route.descp, argv[2]);
                route.pri = DFLT_L3_STATICROUTE_METRIC;
            }
        }
        else
        {
            route.pri = DFLT_L3_STATICROUTE_METRIC;
        }
    }

    SYS_ERR_CHK(sal_l3_staticRouteV4_add(&route));

    return SYS_ERR_OK;
}

int32 cmd_l3_fwdIPv6Enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_l3_fwdIPv6Enable_set(enable));

    return SYS_ERR_OK;
}

int32 cmd_l3_icmpv6RateInterval_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 time = 0;

    if (CMD_IS_NO_FORM_EXIST())
        time = DFLT_L3_IPV6_ICMP_RATE_INTVL;
    else
        time = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_l3_icmpv6RateInterval_set(time));

    return SYS_ERR_OK;
}

int32 cmd_l3_icmpv6RateBurst_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 time = 0;

    if (CMD_IS_NO_FORM_EXIST())
        time = DFLT_L3_IPV6_ICMP_RATE_BURST;
    else
        time = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_l3_icmpv6RateBurst_set(time));

    return SYS_ERR_OK;
}

int32 cmd_l3_routev6Prefer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 val = 0;

    if (CMD_IS_NO_FORM_EXIST())
        val = DFLT_L3_IPV6_ROUTE_PREFER;
    else
        val = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_l3_routev6Prefer_set(val));

    return SYS_ERR_OK;
}

int32 cmd_l3_intfIPv6Enbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;
    else if (SYS_L3_INTFTYPE_LO == id.type)
        id.id = g_selLo;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_l3_adminIntfIPv6Enbl_set(&id, DISABLED));

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_l3_adminIntfIPv6Enbl_set(&id, ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_l3_intfIPv6Mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;
    else if (SYS_L3_INTFTYPE_LO == id.type)
        id.id = g_selLo;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_l3_adminIntfIPv6Mode_set(&id, DISABLED));

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_l3_adminIntfIPv6Mode_set(&id, ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_l3_intfIPv6AutoconfEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_l3_intfIPv6AutoconfEnbl_set(&id, DISABLED));

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_l3_intfIPv6AutoconfEnbl_set(&id, ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_l3_intfIPv6DadAttempt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;
    uint32 num = 0;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;

    if (CMD_IS_NO_FORM_EXIST())
        num = DFLT_L3_IPV6_DADATTEMPT;
    else
        num = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_l3_intfIPv6DadAttempt_set(&id, num));

    return SYS_ERR_OK;
}

int32 cmd_l3_intfIPv6Dhcp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_l3_intfIPv6DhcpEnbl_set(&id, DISABLED));

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_l3_intfIPv6DhcpEnbl_set(&id, ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_l3_intfIPv6DestUnreach_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;
    sys_enable_t enable = DISABLED;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;

    if (CMD_IS_NO_FORM_EXIST())
    {
        enable = DISABLED;
    }
    else
    {
        enable = ENABLED;
    }

    SYS_ERR_CHK(sal_l3_intfIPv6DUnreachableEnable_set(&id, enable));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS

int32 cmd_l3_intfIPv6DhcpStateless_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_l3_intfIPv6DhcpStatelessEnbl_set(&id, DISABLED));

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_l3_intfIPv6DhcpStatelessEnbl_set(&id, ENABLED));

    SYS_ERR_CHK(sal_l3_intfIPv6DhcpStateless_refresh(&id));

    return SYS_ERR_OK;
}

int32 cmd_l3_intfIPv6DhcpRefreshTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;
    uint32 num = 0;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;

    if (CMD_IS_NO_FORM_EXIST())
        num = DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH;
    else
        num = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_l3_intfIPv6DhcpRefreshTime_set(&id, num));

    SYS_ERR_CHK(sal_l3_intfIPv6DhcpStateless_refresh(&id));

    return SYS_ERR_OK;
}

int32 cmd_l3_intfIPv6DhcpMinRefreshTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;
    uint32 num = 0;

    SYS_MEM_CLEAR(id);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;

    if (CMD_IS_NO_FORM_EXIST())
        num = DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH_MIN;
    else
        num = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_l3_intfIPv6DhcpMinRefreshTime_set(&id, num));

    SYS_ERR_CHK(sal_l3_intfIPv6DhcpStateless_refresh(&id));

    return SYS_ERR_OK;
}
#endif

int32 cmd_l3_intfIPv6Addr_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfId_t id;
    sys_l3_ipv6Admin_t admin;
    
    sys_l3_ipv6_t ip;

    SYS_MEM_CLEAR(id);
    SYS_MEM_CLEAR(ip);
    SYS_MEM_CLEAR(admin);

    id.type = g_l3IntfType;

    if (SYS_L3_INTFTYPE_VLAN == id.type)
        id.id = g_selVid;
    else if (SYS_L3_INTFTYPE_LO == id.type)
        id.id = g_selLo;

    SYS_ERR_CHK(sal_l3_adminIntfIPv6_get(&id, &admin));

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (0 == argc)
        {
            SYS_ERR_CHK(sal_l3_intfStaticIPv6_clear(&id));
        }
        else
        {
            SYS_ERR_CHK(_cmd_l3_str2L3IPv6(argv[0], &ip));
            SYS_ERR_CHK(sal_l3_intfStaticIPv6_del(&id, &ip));
        }

        return SYS_ERR_OK;
    }

    if (4 == CMD_TOKENNUM_GET(cmd->vline) && 'l' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        STR2IPV6(argv[0], &ip.addr);
        ip.plen = 64;

        if (!IS_IPV6_LINKLOCAL(ip.addr))
        {
            XMORE("Not a link local address (fe80:: prefix)\n");
            return SYS_ERR_OK;
        }
    }
    else
    {
        SYS_ERR_CHK(_cmd_l3_str2L3IPv6(argv[0], &ip));

        if (IS_IPV6_LINKLOCAL(ip.addr))
        {
            XMORE("IPv6 link-local address format is illegal\n");
            return SYS_ERR_OK;
        }
    }

    if (4 == CMD_TOKENNUM_GET(cmd->vline) && 'e' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        if (ip.plen != 64)
        {
            XMORE("eui-64 format requires prefix length 64\n");

            return SYS_ERR_OK;
        }
        sys_l3_ipv6SysEui64_convert(&ip.addr);
    }

    SYS_ERR_CHK(sal_l3_intfStaticIPv6_add(&id, &ip));

    return SYS_ERR_OK;

}

int32 cmd_l3_staticRouteV6_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_routeV6_t route;
    sys_l3_ipv6_t ip;

    SYS_MEM_CLEAR(route);
    SYS_MEM_CLEAR(ip);

    STR2IPV6(argv[0], &route.dst);
    _cmd_l3_str2L3IPv6(argv[0], &ip);
    osal_memcpy(&route.dst, &ip.addr, sizeof(sys_ipv6_t));
    route.dstLen = ip.plen;

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (argc > 1)
        {
            STR2IPV6(argv[1], &route.gw);
            SYS_ERR_CHK(sal_l3_staticRouteV6_del(&route));
        }
        else
        {
            uint32 i = 0;
            uint32 delNum = 0;
            sys_l3_staticRouteV6_t rules;

            SYS_MEM_CLEAR(rules);

            sal_l3_staticRouteV6_get(&rules);

            for (i = 0; i < rules.num; i++)
            {
                if (IS_IPV6_EQUAL(rules.route[i].dst, route.dst)
                        && rules.route[i].dstLen == route.dstLen)
                {
                    osal_memcpy(&route.gw, &rules.route[i].gw, sizeof(sys_ipv6_t));

                    SYS_ERR_CONTINUE(sal_l3_staticRouteV6_del(&route));

                    delNum++;
                }
            }
        }

        return SYS_ERR_OK;
    }

    STR2IPV6(argv[1], &route.gw);

    if (argc > 2)
        route.pri = SYS_STR2UINT(argv[2]);
    else
        route.pri = DFLT_L3_STATICROUTE_METRIC;

    SYS_ERR_CHK(sal_l3_staticRouteV6_add(&route));

    return SYS_ERR_OK;

}

int32 cmd_l3_staticRouteV6LinkLocal_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_routeV6_t route;
    sys_l3_ipv6_t ip;

    SYS_MEM_CLEAR(route);
    SYS_MEM_CLEAR(ip);

    STR2IPV6(argv[0], &route.dst);
    _cmd_l3_str2L3IPv6(argv[0], &ip);
    osal_memcpy(&route.dst, &ip.addr, sizeof(sys_ipv6_t));
    route.dstLen = ip.plen;

    STR2IPV6(argv[1], &route.gw);

    route.ointf.type = SYS_L3_INTFTYPE_VLAN;
    route.ointf.id = SYS_STR2UINT(argv[2]);

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_l3_staticRouteV6_del(&route));

        return SYS_ERR_OK;
    }

    if (argc > 3)
        route.pri = SYS_STR2UINT(argv[3]);
    else
        route.pri = DFLT_L3_STATICROUTE_METRIC;

    SYS_ERR_CHK(sal_l3_staticRouteV6_add(&route));

    return SYS_ERR_OK;

}

int32 cmd_l3_staticRouteV6Reject_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_routeV6_t route;
    sys_l3_ipv6_t ip;

    SYS_MEM_CLEAR(route);
    SYS_MEM_CLEAR(ip);

    STR2IPV6(argv[0], &route.dst);
    _cmd_l3_str2L3IPv6(argv[0], &ip);
    osal_memcpy(&route.dst, &ip.addr, sizeof(sys_ipv6_t));
    route.dstLen = ip.plen;

    if (CMD_IS_NO_FORM_EXIST())
    {
        uint32 i = 0;
        uint32 delNum = 0;
        sys_l3_staticRouteV6_t rules;

        SYS_MEM_CLEAR(rules);

        sal_l3_staticRouteV6_get(&rules);

        for (i = 0; i < rules.num; i++)
        {
            if (IS_IPV6_EQUAL(rules.route[i].dst, route.dst)
                    && rules.route[i].dstLen == route.dstLen)
            {
                osal_memcpy(&route.gw, &rules.route[i].gw, sizeof(sys_ipv6_t));

                SYS_ERR_CONTINUE(sal_l3_staticRouteV6_del(&route));

                delNum++;
            }
        }

        return SYS_ERR_OK;
    }

    if (argc > 1)
        route.pri = SYS_STR2UINT(argv[1]);
    else
        route.pri = DFLT_L3_STATICROUTE_METRIC;

    route.ointf.type = SYS_L3_INTFTYPE_NULL;

    SYS_ERR_CHK(sal_l3_staticRouteV6_add(&route));

    return SYS_ERR_OK;

}

int32 cmd_l3_neigh_ageTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 arpAgeTime = DFLT_L3_NEIGH_AGE_TIME;

    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_l3_neigh_ageTime_set(arpAgeTime));
    else
        SYS_ERR_CHK(sal_l3_neigh_ageTime_set(SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}

int32 cmd_l3_neigh_respTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 respTime = DFLT_L3_NEIGH_RESP_TIME;

    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_l3_neigh_respTime_set(respTime));
    else
        SYS_ERR_CHK(sal_l3_neigh_respTime_set(SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}

int32 cmd_l3_neigh_retryTimes_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 retryTimes = DFLT_L3_NEIGH_RETRY_TIMES;

    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_l3_neigh_retryTimes_set(retryTimes));
    else
        SYS_ERR_CHK(sal_l3_neigh_retryTimes_set(SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}

int32 cmd_l3_neigh_cacheSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 cacheSize = DFLT_L3_NEIGH_CACHE_SIZE;

    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_l3_neigh_cacheSize_set(cacheSize));
    else
        SYS_ERR_CHK(sal_l3_neigh_cacheSize_set(SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}

int32 cmd_l3_neigh_renew_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_l3_neigh_renew_set(DISABLED));
    else
        SYS_ERR_CHK(sal_l3_neigh_renew_set(ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_l3_neighStatic_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_mac_t mac;
    sys_l3_ip_t l3ip;
    sys_ip_t ip;
    sys_vid_t vid;
    sys_l3_intfId_t intf;
    sys_l3_ipv6Admin_t ipv6Admin;
    sys_l3_ipv4Db_t *pIPv4Entry = NULL;
    
    char intfName[CAPA_L3_INTFNAME_LEN];
    char buf[SYS_BUF256_LEN];
    uint32 usedCnt = 0;

    SYS_MEM_CLEAR(l3ip);
    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(intf);
    SYS_MEM_CLEAR(intfName);

#define _POSTFIX " 1>> /dev/null 2>> /dev/null"
    if (CMD_IS_NO_FORM_EXIST())
    {
        if ('a' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
        {
            if (FALSE == IS_IPV4_UADDRSTR_VALID(argv[0]))
                return SYS_ERR_IPV4_ADDR;

            SYS_ERR_CHK(STR2IPV4(argv[0], &l3ip.v4.addr));
            SYS_ERR_CHK(sys_l3_ip2Intf(SYS_L3_AF_IPV4, l3ip, &intf));

            if (intf.id == 0)
                return SYS_ERR_L3_STATIC_NEIGHV4_INTF_NOT_EXIST;

            if (argc == 2)
            {
                vid = SYS_STR2UINT(argv[1]);
                if (vid != intf.id)
                    return SYS_ERR_L3_STATIC_NEIGHV4_INTF_NOT_EXIST;
            }

            INTFID2STR(&intf, intfName);
            sprintf(buf, "ip neigh del %s dev %s"_POSTFIX, argv[0], intfName);
            system(buf);
        }
        else
        {

            SYS_MEM_CLEAR(intf);
            if (argc == 2)
            {
                if (1 != inet_pton(AF_INET6, argv[0], &l3ip.v6))
                    return SYS_ERR_IPV6_ADDR;
                 intf.id = SYS_STR2UINT(argv[1]);
                 INTFID2STR(&intf, intfName);
                 sprintf(buf, "ip -6 neigh del %s dev %s"_POSTFIX, argv[0], intfName);
                 system(buf);
            }
            else if (argc == 1)
            {
                if (4 == CMD_TOKENNUM_GET(cmd->vline))
                {
                    if (1 != inet_pton(AF_INET6, argv[0], &l3ip.v6))
                        return SYS_ERR_IPV6_ADDR;

                    SYS_ERR_CHK(sys_l3_ip2Intf(SYS_L3_AF_IPV6, l3ip, &intf));
                    if (0 == intf.id)
                        return SYS_ERR_OK;

                    INTFID2STR(&intf, intfName);
                    sprintf(buf, "ip -6 neigh del %s dev %s"_POSTFIX, argv[0], intfName);
                    system(buf);
                }
                else
                {
                    intf.type = SYS_L3_INTFTYPE_VLAN;
                    intf.id = SYS_STR2UINT(argv[0]);
                    SYS_ERR_CHK(sal_l3_neigh_clear(SYS_L3_NEIGH_V6_STATIC, intf));
                }
            }
            else
            {
                SYS_ERR_CHK(sal_l3_neigh_clear(SYS_L3_NEIGH_V6_STATIC, intf));
            }
        }
    }
    else
    {
        SYS_ERR_CHK(sal_l3_neighCnt_get(&usedCnt));

        if (usedCnt >= CAPA_L3_HOST_ROUTE_ENTRY_NUM)
            return SYS_ERR_ENTRY_FULL;

        if ('a' == CMD_TOKENSTR_GET(cmd->vline, 0)[0])
        {
            if (FALSE == IS_IPV4_UADDRSTR_VALID(argv[0]))
                return SYS_ERR_IPV4_ADDR;

            SYS_ERR_CHK(sys_util_str2Mac(argv[1], mac.octet));

            if ((0x1 & mac.octet[0]))
                return SYS_ERR_L3_STATIC_NEIGH_MAC;

            SYS_ERR_CHK(sal_l3_neigh_mac_validate(&mac));

            SYS_ERR_CHK(STR2IPV4(argv[0], &l3ip.v4.addr));
            SYS_ERR_CHK(sys_l3_ip2Intf(SYS_L3_AF_IPV4, l3ip, &intf));

            if (0 == intf.id)
                return SYS_ERR_L3_STATIC_NEIGHV4_NOT_EXIST;

            FOR_EACH_L3_IPV4_IN_INTF(&intf, pIPv4Entry)
            {
                if (l3ip.v4.addr == pIPv4Entry->ip.addr)
                    return SYS_ERR_L3_STATIC_NEIGHV4_SAME_DUT;
            }

            if (argc == 3)
            {
                vid = SYS_STR2UINT(argv[2]);

                if (vid != intf.id)
                    return SYS_ERR_L3_STATIC_NEIGHV4_NOT_MATCH;
            }

            INTFID2STR(&intf, intfName);
            SYS_MEM_CLEAR(buf);
            sprintf(buf, "ip neigh add %s lladdr %s nud permanent dev %s"_POSTFIX,
                argv[0], argv[1], intfName);

            if(0 != system(buf))
            {
                SYS_MEM_CLEAR(ip);
                ip.v4 = l3ip.v4.addr;
                sal_l3_hrouteEntry_del(ip, mac);
                SYS_MEM_CLEAR(buf);
                sprintf(buf, "ip neigh change %s lladdr %s nud permanent dev %s"_POSTFIX,
                    argv[0], argv[1], intfName);
                system(buf);
            }
        }
        else
        {
            if (FALSE == IS_IPV6_UADDRSTR_VALID(argv[0]))
                return SYS_ERR_IPV6_ADDR;

            SYS_ERR_CHK(sys_util_str2Mac(argv[2], mac.octet));

            if ((0x1 & mac.octet[0]))
                return SYS_ERR_L3_STATIC_NEIGH_MAC;

            SYS_ERR_CHK(STR2IPV6(argv[0], &l3ip.v6.addr));
            vid = SYS_STR2UINT(argv[1]);
            intf.type = SYS_L3_INTFTYPE_VLAN;
            intf.id = vid;
            SYS_MEM_CLEAR(ipv6Admin);
            SYS_ERR_CHK(sal_l3_adminIntfIPv6_get(&intf, &ipv6Admin));

            if (DISABLED == ipv6Admin.enbl)
                return SYS_ERR_L3_STATIC_NEIGHV6_INTF_NOTON;

            if (0xfe != l3ip.v6.addr.hex[0] || 0x80 != l3ip.v6.addr.hex[1])
            {
                SYS_MEM_CLEAR(intf);
                SYS_ERR_CHK(sys_l3_ip2Intf(SYS_L3_AF_IPV6, l3ip, &intf));

                if (vid != intf.id)
                    return SYS_ERR_L3_STATIC_NEIGHV6_NOT_MATCH;
            }

            INTFID2STR(&intf, intfName);
            SYS_MEM_CLEAR(buf);
            sprintf(buf, "ip -6 neigh add %s lladdr %s nud permanent dev %s"_POSTFIX,
                argv[0], argv[2], intfName);

            if(0 != system(buf))
            {
                SYS_MEM_CLEAR(ip);
                osal_memcpy(&ip.v6, &l3ip.v6.addr, sizeof(l3ip.v6.addr));
                sal_l3_hrouteEntry_del(ip, mac);
                SYS_MEM_CLEAR(buf);
                sprintf(buf, "ip -6 neigh change %s lladdr %s nud permanent dev %s"_POSTFIX,
                    argv[0], argv[2], intfName);
                system(buf);
            }
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_l3_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char ipstr[CAPA_IPV6_STR_LEN];
    char gwstr[CAPA_IPV6_STR_LEN];
    int32 i = 0;
    sys_l3_staticRouteV4_t v4Rules;
    sys_l3_staticRouteV6_t v6Rules;
    sys_enable_t enable = 0;
    uint32 value = 0;
    uint32 arpAgeTime = 0;
    uint32 arpRespTime = 0;
    uint32 arpRetryTimes = 0;
    uint32 arpCacheSize = 0;
    sys_enable_t renew = DISABLED;
    FILE *pfp = NULL;
    char buf[SYS_BUF128_LEN];
    char *tok[4];

    SYS_ERR_CHK(sal_l3_fwdIPv4Enable_get(&enable));
    if (DFLT_L3_IPV4_FORWARDING_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip unicast-routing\n", (enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_staticRouteV4_get(&v4Rules));

    for (i = 0; i < v4Rules.num; i++)
    {
        SYS_MEM_CLEAR(ipstr);
        SYS_MEM_CLEAR(gwstr);
        SYS_MEM_CLEAR(buf);

        IPV42STR(v4Rules.route[i].dst, ipstr);
        IPV42STR(v4Rules.route[i].gw, gwstr);

        if (0 != osal_strlen(v4Rules.route[i].descp))
        {
            osal_sprintf(buf, " description %s", v4Rules.route[i].descp);
        }

        SYS_MEM_CLEAR(cmd);
        if (DFLT_L3_STATICROUTE_METRIC == v4Rules.route[i].pri)
            osal_sprintf(cmd, "ip route %s/%d %s%s\n", ipstr, v4Rules.route[i].dstLen, gwstr, buf);
        else
            osal_sprintf(cmd, "ip route %s/%d %s metric %d%s\n",
                              ipstr,
                              v4Rules.route[i].dstLen,
                              gwstr,
                              v4Rules.route[i].pri,
                              buf);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_fwdIPv6Enable_get(&enable));
    if (DFLT_L3_IPV6_FORWARDING_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 unicast-routing\n", (enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_hopLimitIPv6_get(&value));
    if (DFLT_L3_IPV6_HOPLIMIT != value)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ipv6 hop-limit %u\n", value);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_icmpv6RateInterval_get(&value));
    if (DFLT_L3_IPV6_ICMP_RATE_INTVL != value)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ipv6 icmp-rate-limit interval %u\n", value);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_icmpv6RateBurst_get(&value));
    if (DFLT_L3_IPV6_ICMP_RATE_BURST != value)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ipv6 icmp-rate-limit burst %u\n", value);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_routev6Prefer_get(&value));
    if (DFLT_L3_IPV6_ROUTE_PREFER != value)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ipv6 route preference %u\n", value);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_staticRouteV6_get(&v6Rules));

    for (i = 0; i < v6Rules.num; i++)
    {
        SYS_MEM_CLEAR(ipstr);
        SYS_MEM_CLEAR(gwstr);

        IPV62STR(v6Rules.route[i].dst, ipstr);
        IPV62STR(v6Rules.route[i].gw, gwstr);

        SYS_MEM_CLEAR(cmd);
        if (SYS_L3_INTFTYPE_NULL == v6Rules.route[i].ointf.type)
        {
            if (DFLT_L3_STATICROUTE_METRIC == v6Rules.route[i].pri)
                osal_sprintf(cmd, "ipv6 route %s/%d reject\n", ipstr, v6Rules.route[i].dstLen);
            else
                osal_sprintf(cmd, "ipv6 route %s/%d metric %d reject\n",
                                  ipstr,
                                  v6Rules.route[i].dstLen,
                                  v6Rules.route[i].pri);
        }
        else if (SYS_L3_INTFTYPE_VLAN == v6Rules.route[i].ointf.type)
        {
            if (DFLT_L3_STATICROUTE_METRIC == v6Rules.route[i].pri)
                osal_sprintf(cmd, "ipv6 route %s/%d %s link-local interface vlan %u\n",
                                  ipstr,
                                  v6Rules.route[i].dstLen,
                                  gwstr,
                                  v6Rules.route[i].ointf.id);
            else
                osal_sprintf(cmd, "ipv6 route %s/%d %s link-local interface vlan %u metric %d\n",
                                  ipstr,
                                  v6Rules.route[i].dstLen,
                                  gwstr,
                                  v6Rules.route[i].ointf.id,
                                  v6Rules.route[i].pri);
        }
        else
        {
            if (DFLT_L3_STATICROUTE_METRIC == v6Rules.route[i].pri)
                osal_sprintf(cmd, "ipv6 route %s/%d %s\n", ipstr, v6Rules.route[i].dstLen, gwstr);
            else
                osal_sprintf(cmd, "ipv6 route %s/%d %s metric %d\n",
                                  ipstr,
                                  v6Rules.route[i].dstLen,
                                  gwstr,
                                  v6Rules.route[i].pri);
        }
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_neigh_ageTime_get(&arpAgeTime));
    if (DFLT_L3_NEIGH_AGE_TIME != arpAgeTime)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "arp timeout %u\n", arpAgeTime);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_neigh_respTime_get(&arpRespTime));
    if (DFLT_L3_NEIGH_RESP_TIME != arpRespTime)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "arp response %u\n", arpRespTime);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

        SYS_ERR_CHK(sal_l3_neigh_retryTimes_get(&arpRetryTimes));
    if (DFLT_L3_NEIGH_RETRY_TIMES != arpRetryTimes)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "arp retry %u\n", arpRetryTimes);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_neigh_cacheSize_get(&arpCacheSize));
    if (DFLT_L3_NEIGH_CACHE_SIZE != arpCacheSize)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "arp cache %u\n", arpCacheSize);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_l3_neigh_renew_get(&renew));
    if (DFLT_L3_NEIGH_RENEW != renew)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sarp renew\n", DISABLED == renew ? "no " : "");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (NULL == (pfp = popen("cat /proc/"L3_NEIGH_V4_PROC_NAME, "r")))
        return SYS_ERR_OK;

    while (fgets(buf, sizeof(buf), pfp))
    {
        tok[0] = strtok(buf, ", "); 

        if (!isdigit(tok[0][0])) 
            break;

        tok[1] = strtok(NULL, ", "); 
        tok[2] = strtok(NULL, ", "); 
        tok[3] = strtok(NULL, ", "); 

        if (SYS_STR2UINT(tok[3]) == SYS_L3_NEIGH_V4_STATIC)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "arp %s %s vlan %s\n", tok[1], tok[2], tok[0]);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    pclose(pfp);

    if (NULL == (pfp = popen("cat /proc/"L3_NEIGH_V6_PROC_NAME, "r")))
        return SYS_ERR_OK;

    while (fgets(buf, sizeof(buf), pfp))
    {
        tok[0] = strtok(buf, ", "); 

        if (!isdigit(tok[0][0])) 
            break;

        tok[1] = strtok(NULL, ", "); 
        tok[2] = strtok(NULL, ", "); 
        tok[3] = strtok(NULL, ", "); 

        if (SYS_STR2UINT(tok[3]) == SYS_L3_NEIGH_V6_STATIC)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "ipv6 neighbor %s vlan %s %s\n", tok[1], tok[0], tok[2]);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    pclose(pfp);

    return SYS_ERR_OK;
}

int32 cmd_l3_vlan_save(FILE *fp, sys_vid_t vid)
{
    char cmd[MAX_CMD_STRLEN];
    char ipstr[CAPA_IPV4_STR_LEN];
    char ipv6Str[CAPA_IPV6_STR_LEN];
    sys_l3_intfId_t id;
    sys_l3_intfAdmin_t adminCfg;
    sys_l3_ipv4Db_t *pIPv4Entry = NULL;
    sys_l3_ipv6Db_t *pIPv6Entry = NULL;
    int firstIp = 1;
#if defined(CONFIG_SYS_DHCP_SERVER) && !defined(CONFIG_SYS_L3_ARCHITECTURE)   
	sys_l3_intfMgmt_t intfMgmt;
#endif

    SYS_MEM_CLEAR(id);
    SYS_MEM_CLEAR(adminCfg);

    id.type = SYS_L3_INTFTYPE_VLAN;
    id.id = vid;

    SYS_ERR_CHK(sal_l3_adminIntf_get(&id, &adminCfg));

    if (ENABLED == adminCfg.ipv4.dhcpEnbl)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip dhcp\n");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
    else
    {
        FOR_EACH_L3_IPV4_IN_INTF(&id, pIPv4Entry)
        {
            SYS_MEM_CLEAR(ipstr);

            if (SYS_L3_INTFTYPE_VLAN != id.type)
                continue;

            if (SYS_L3_IPTYPE_STATIC != pIPv4Entry->list.type)
                continue;
		
            IPV42STR(pIPv4Entry->ip.addr, ipstr);
            SYS_MEM_CLEAR(cmd);
           	if(firstIp)
           	{
            	osal_sprintf(cmd, "ip address %s/%d\n", ipstr, pIPv4Entry->ip.plen);
            	firstIp = 0;
            }
            else
            {
				osal_sprintf(cmd, "ip address %s/%d sub\n", ipstr, pIPv4Entry->ip.plen);
            }
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

#if !defined(CONFIG_SYS_L3_ARCHITECTURE)
	SYS_ERR_CHK(sal_l3_intfMgmt_get(&intfMgmt));
	if(intfMgmt.mgmtVlan != vid)
#endif
	{
#if defined(CONFIG_SYS_DHCP_SERVER)
		cmd_dhcp_sever_l3intf_save(fp, vid);
#endif
#if defined(CONFIG_SYS_L3_ROUTE_OSPF)
		cmd_l3_ospf_interface_save(fp, vid);
#endif
#if defined(CONFIG_SYS_L3_VRRP)
		cmd_l3_vrrp_interface_save(fp, vid);
#endif
	}

    if (DFLT_L3_IPV6_AUTOCONF_EBL != adminCfg.ipv6.autoconfEnbl)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 address autoconfig\n", (adminCfg.ipv6.autoconfEnbl) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_L3_IPV6_DADATTEMPT != adminCfg.ipv6.dadTxNum)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ipv6 nd dad attempts %d\n", adminCfg.ipv6.dadTxNum);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_L3_IPV6_DHCPC_EBL != adminCfg.ipv6.dhcp6c.enbl)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 dhcp client\n", (adminCfg.ipv6.dhcp6c.enbl) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_L3_IPV6_DESTINATION_UNREACHABLE != adminCfg.ipv6.unreachMsgSend)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 destination-unreachable\n", (adminCfg.ipv6.unreachMsgSend) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS
    if (DFLT_L3_IPV6_DHCPC_STATELESS_EBL != adminCfg.ipv6.dhcp6c.stateless)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 dhcp client stateless\n", (adminCfg.ipv6.dhcp6c.stateless) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH != adminCfg.ipv6.dhcp6c.refreshtime)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ipv6 dhcp client information refresh %u\n", adminCfg.ipv6.dhcp6c.refreshtime);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH_MIN != adminCfg.ipv6.dhcp6c.min_refreshtime)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ipv6 dhcp client information refresh minimum %u\n", adminCfg.ipv6.dhcp6c.min_refreshtime);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

    FOR_EACH_L3_IPV6_IN_INTF(&id, pIPv6Entry)
    {
        
        if (SYS_L3_IPTYPE_STATIC == pIPv6Entry->list.type)
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(ipv6Str);
            IPV62STR(pIPv6Entry->ip.addr, ipv6Str);
            osal_sprintf(cmd, "ipv6 address %s/%d\n", ipv6Str, pIPv6Entry->ip.plen);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
        else if (SYS_L3_IPTYPE_V6LINKLOCAL == pIPv6Entry->list.type)
        {
            sys_l3_ipv6_t sysLinkLocalIp;

            SYS_MEM_CLEAR(sysLinkLocalIp);

            sys_l3_ipv6SysLinkLocal_get(&sysLinkLocalIp);

            if (!IS_IPV6_EQUAL(pIPv6Entry->ip.addr, sysLinkLocalIp.addr))
            {
                SYS_MEM_CLEAR(cmd);
                SYS_MEM_CLEAR(ipv6Str);
                IPV62STR(pIPv6Entry->ip.addr, ipv6Str);
                osal_sprintf(cmd, "ipv6 address %s link-local\n", ipv6Str);
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }
        }
    }

    if (DFLT_L3_INTF_IPV6_ENBL != adminCfg.ipv6.enbl)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 enable\n", (adminCfg.ipv6.enbl) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_L3_INTF_IPV6_ENBL != adminCfg.ipv6.mode)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 mode\n", (adminCfg.ipv6.mode) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_L3_INTFV_ENBL != adminCfg.enbl)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sshutdown\n", (ENABLED == adminCfg.enbl) ? "no " : "");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_L3_INTFV_EXIST != adminCfg.exist)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%son\n", (DISABLED == adminCfg.exist) ? "no " : "");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_l3_lo_save(FILE *fp, uint32 id)
{
    char cmd[MAX_CMD_STRLEN];
    char ipstr[CAPA_IPV6_STR_LEN];
    sys_l3_intfId_t intfId;
    sys_l3_ipv4Db_t *pIPv4Entry = NULL;
    sys_l3_ipv6Db_t *pIPv6Entry = NULL;
    sys_l3_intfAdmin_t adminCfg;

    intfId.type = SYS_L3_INTFTYPE_LO;
    intfId.id = id;

    SYS_MEM_CLEAR(adminCfg);

    SYS_ERR_CHK(sal_l3_adminIntf_get(&intfId, &adminCfg));

    FOR_EACH_L3_IPV4_IN_INTF(&intfId, pIPv4Entry)
    {
        SYS_MEM_CLEAR(ipstr);
        IPV42STR(pIPv4Entry->ip.addr, ipstr);

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip address %s/%d\n", ipstr, pIPv4Entry->ip.plen);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    FOR_EACH_L3_IPV6_IN_INTF(&intfId, pIPv6Entry)
    {
        
        if (SYS_L3_IPTYPE_STATIC == pIPv6Entry->list.type)
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(ipstr);
            IPV62STR(pIPv6Entry->ip.addr, ipstr);
            osal_sprintf(cmd, "ipv6 address %s/%d\n", ipstr, pIPv6Entry->ip.plen);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
        else if (SYS_L3_IPTYPE_V6LINKLOCAL == pIPv6Entry->list.type)
        {
            sys_l3_ipv6_t sysLinkLocalIp;

            SYS_MEM_CLEAR(sysLinkLocalIp);

            sys_l3_ipv6SysLinkLocal_get(&sysLinkLocalIp);

            if (!IS_IPV6_EQUAL(pIPv6Entry->ip.addr, sysLinkLocalIp.addr))
            {
                SYS_MEM_CLEAR(cmd);
                SYS_MEM_CLEAR(ipstr);
                IPV62STR(pIPv6Entry->ip.addr, ipstr);
                osal_sprintf(cmd, "ipv6 address %s link-local\n", ipstr);
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }
        }
    }

    if (DFLT_L3_INTF_IPV6_ENBL != adminCfg.ipv6.enbl)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 enable\n", (adminCfg.ipv6.enbl) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_L3_INTFV_ENBL != adminCfg.enbl)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sshutdown\n", (ENABLED == adminCfg.enbl) ? "no " : "");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

