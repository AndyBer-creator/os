
#include <libsal/sal_l2.h>
#include <libsal/sal_trunk.h>
#include <libcmd/cmd.h>
#ifdef CONFIG_SYS_PORT_SECURITY
#include <libsal/sal_psecure.h>
#endif

int32 cmd_l2_macTable_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;

    if(1 == argc)
    {
        CMD_PORTLIST_GET(0, &logicPortmask);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
        {
            
            usleep(10000);
            CMD_ERR_CONTINUE_PORT(sal_l2_dynamicMac_clearByPort(logicPort), logicPort);
        }
    }
    else
    {
        SYS_ERR_CHK(sal_l2_dynamicMac_clear());
    }

    return SYS_ERR_OK;
}

int32 cmd_l2_macTableVlan_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_l2_dynamicMac_clearByVlan(SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}

int32 cmd_l2_macAging_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 aging_time;

    SYS_ERR_CHK(sal_l2_aging_get(&aging_time));
    XMORE("Mac Address Table aging time: %4d sec\n",aging_time);

    return SYS_ERR_OK;
}

int32 cmd_l2_macAddress_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlanmask_t vlanMask;
    uint32 tableSize;
    sys_mac_t mac;
    char macStr[SYS_BUF32_LEN];
    char portStr[CAPA_PORT_STR_LEN];
    uint32 i = 0;
    uint32 count = 0;
    sys_l2_entry_t *pEntry = NULL;

    VLANMASK_CLEAR_ALL(vlanMask);
    SYS_MEM_CLEAR(macStr);

    SYS_ERR_CHK(sys_util_str2Mac(argv[0], mac.octet));

    if(2 == argc)
        VLANMASK_SET_VID(vlanMask, SYS_STR2UINT(argv[1]));
    else
        VLANMASK_SET_ALL(vlanMask);

    SYS_ERR_CHK(sys_util_mac2Str(mac.octet, macStr));

    SYS_ERR_CHK(sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &tableSize));

    XMORE(" VID  | MAC Address       | Type              | Ports          \n");
    XMORE("------+-------------------+-------------------+----------------\n");

    for (i = 0; i < tableSize; i++)
    {
        pEntry = L2_SORT_ENTRY_PTR(i);

        if (0 != osal_memcmp(&pEntry->mac, &mac, sizeof(sys_mac_t)))
            continue;

        if (!VLANMASK_IS_VIDSET(vlanMask, pEntry->vid))
            continue;

        SYS_MEM_CLEAR(portStr);

        if (SYS_L2_TYPE_MGMT == pEntry->type)
            osal_strcpy(portStr, "CPU");
        else if (SYS_L2_TYPE_NEXTHOP_FLOOD == pEntry->type)
        	continue;
        else
            LP2STR(pEntry->port, portStr);

        XMORE(" %4d | %17s | %-17s | %s \n",
                  pEntry->vid,
                  macStr,
                  text_l2_type[pEntry->type].text,
                  portStr);

        count++;
    }

    XMORE("\nTotal number of entries: %d\n",count);

    return SYS_ERR_OK;
}

int32 cmd_l2_macAddressTable_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isShowStatic = TRUE;
    uint8 isShowDynamic = TRUE;
    uint8 isShowAll = TRUE;
    sys_vid_t showVid = 0;
    sys_logic_portmask_t showlpm;

    uint32 i;
    uint32 tableSize;
    uint32 tokenNum = 0;
    uint32 count = 0;
    char *ridx = NULL;
    char macStr[SYS_BUF32_LEN];
    char portStr[CAPA_PORT_STR_LEN];
#ifndef CONFIG_SYS_L3_ARCHITECTURE
    sys_mac_t cpuMac;
#endif
    sys_vid_t mgmtVid;
    sys_l2_entry_t *pEntry = NULL;

    SYS_MEM_CLEAR(portStr);
    LOGIC_PORTMASK_SET_ALL(showlpm);
    count = CMD_TOKENNUM_GET(cmd->vline);

#if defined(CONFIG_SYS_MANAGEMENT_VLAN) && !defined(CONFIG_SYS_L3_ARCHITECTURE)
    SYS_ERR_CHK(sal_vlan_mgmtVid_get(&mgmtVid));
#else
    SYS_ERR_CHK(sal_vlan_defaultVid_get(&mgmtVid));
#endif

    for (i = 1; i < count ; i++)
    {
        ridx = CMD_TOKENSTR_GET(cmd->vline, i);

        if ('s' == ridx[0])
        {
            isShowDynamic = FALSE;
            isShowAll = FALSE;
        }
        else if ('d' == ridx[0])
        {
            isShowStatic = FALSE;
            isShowAll = FALSE;
        }
        else if ('v' == ridx[0])
        {
            tokenNum++;
            showVid = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));

            if (showVid != mgmtVid)
                isShowAll = FALSE;
        }
        else if ('i' == ridx[0])
        {
            i++;
            CMD_PORTLIST_GET(tokenNum, &showlpm);
            isShowAll = FALSE;
        }
    }

    count = 0;
    SYS_ERR_CHK(sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &tableSize));

    XMORE(" VID  | MAC Address       | Type              | Ports          \n");
    XMORE("------+-------------------+-------------------+----------------\n");

    if (isShowAll)
    {
#ifndef CONFIG_SYS_L3_ARCHITECTURE
        SYS_ERR_CHK(sal_sys_macAddr_get(&cpuMac));

        sys_util_mac2Str(cpuMac.octet, macStr);

        XMORE(" %4d | %17s | %-17s | %s \n",
                  mgmtVid,
                  macStr,
                  text_l2_type[SYS_L2_TYPE_MGMT].text,
                  "CPU");

        count++;
#endif
    }

    for (i = 0; i < tableSize; i++)
    {
        pEntry = L2_SORT_ENTRY_PTR(i);

        if (SYS_L2_TYPE_DYNAMIC == pEntry->type)
        {
            if (!isShowDynamic)
                continue;
        }
        else if (SYS_L2_TYPE_MGMT == pEntry->type)
        {
            continue;
        }
        else if (SYS_L2_TYPE_STATIC == pEntry->type)
        {
            if (!isShowStatic)
                continue;
        }
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
        else if (SYS_L2_TYPE_BLACKLIST == pEntry->type)
        {
            if (!isShowStatic)
                continue;
        }
#endif
        else if (SYS_L2_TYPE_NEXTHOP_FLOOD == pEntry->type)
        {
            continue;
        }

        if (0 == pEntry->vid)
            continue;

        if ((0 != showVid) && (showVid != pEntry->vid))
            continue;

        if (!IS_LOGIC_PORTMASK_PORTSET(showlpm, pEntry->port))
            continue;

        sys_util_mac2Str(pEntry->mac.octet, macStr);

        SYS_MEM_CLEAR(portStr);
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
        if (SYS_L2_TYPE_BLACKLIST == pEntry->type)
            osal_strcpy(portStr, "N/A");
        else
#endif
		if (SYS_L2_TYPE_L3_MGMT == pEntry->type)
		{
			osal_strcpy(portStr,"CPU");
		}
		else
            LP2STR(pEntry->port, portStr);

        XMORE(" %4d | %17s | %-17s | %s \n",
                  pEntry->vid,
                  macStr,
                  text_l2_type[pEntry->type].text,
                  portStr);

        count++;
    }

    XMORE("\nTotal number of entries: %d\n", count);

    return SYS_ERR_OK;
}

int32 cmd_l2_macAddressTableCnt_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 table_size = 0;

    SYS_ERR_CHK(sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &table_size));
    XMORE("Total number of entries: %u\n", table_size);

    return SYS_ERR_OK;
}

int32 cmd_l2_agingtime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 aging_time;

    aging_time = SYS_STR2UINT(argv[0]);
    SYS_ERR_CHK(sal_l2_aging_set(aging_time));

    return SYS_ERR_OK;

}

int32 cmd_l2_agingtime_never_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 aging_time = 0;

    SYS_ERR_CHK(sal_l2_aging_set(aging_time));

    return SYS_ERR_OK;

}

int32 cmd_l2_staticMacEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l2_entry_t entry;

    SYS_MEM_CLEAR(entry);

    SYS_ERR_CHK(sys_util_str2Mac(argv[0], entry.mac.octet));
    entry.vid = SYS_STR2UINT(argv[1]);
    STR2LP(argv[2], &entry.port);

    if ((IS_LP_TRKMBR(entry.port)) || (IS_TRKLP(entry.port) && IS_TRKLP_EMPTY(entry.port)))
    {
        char portStr[CAPA_PORT_STR_LEN];

        SYS_MEM_CLEAR(portStr);

        LP2STR(entry.port, portStr);

        if(IS_LP_TRKMBR(entry.port))
            SYS_PRINTF("Cannot add static address on %s because the port is an LAG member\n", portStr);
        else
            SYS_PRINTF("Cannot add static address on %s because the LAG is empty\n", portStr);

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_l2_staticMac_add(&entry));

    return SYS_ERR_OK;
}

int32 cmd_l2_staticMacEntry_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l2_entry_t entry;

    SYS_MEM_CLEAR(entry);

    SYS_ERR_CHK(sys_util_str2Mac(argv[0], entry.mac.octet));
    entry.vid = SYS_STR2UINT(argv[1]);

    SYS_ERR_CHK(sal_l2_mac_search(&entry));

#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    if (SYS_L2_TYPE_BLACKLIST == entry.type)
        SYS_ERR_CHK(sal_l2_blacklistMac_del(&entry));
    else
#endif
        SYS_ERR_CHK(sal_l2_staticMac_del(&entry));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_BLACK_HOLE_MAC

int32 cmd_l2_filterMacEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l2_entry_t entry;
    sys_mac_t cpuMac;
    char macStr[CAPA_MAC_BYTE_NUM];

    SYS_MEM_CLEAR(entry);
    SYS_MEM_CLEAR(cpuMac);

    SYS_ERR_CHK(sal_sys_macAddr_get(&cpuMac));
    if((0 == osal_memcmp(&cpuMac.octet, &entry.mac, CAPA_MAC_BYTE_NUM))||(0x1 & entry.mac.octet[0]))
    {
        SYS_MEM_CLEAR(macStr);
        sys_util_mac2Str(entry.mac.octet, macStr);
        SYS_PRINTF("MAC:%s: Illegal MAC address\n", macStr);
        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sys_util_str2Mac(argv[0], entry.mac.octet));
    entry.vid = SYS_STR2UINT(argv[1]);

    SYS_ERR_CHK(sal_l2_blacklistMac_add(&entry));

    return SYS_ERR_OK;
}
#endif

static int32 _cmd_l2_staticMacEntry_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char macStr[CAPA_MAC_STR_LEN];
    char portStr[CAPA_PORT_STR_LEN];
    uint32 i = 0;
    sys_l2_entry_t *pCurr = NULL;

    FOR_EACH_L2_STATIC_ENTRY(i, pCurr)
    {
        sys_util_mac2Str(pCurr->mac.octet, macStr);
        LP2STR(pCurr->port, portStr);

        osal_sprintf(cmd, "mac address-table static %s vlan %u interfaces %s\n",
                     macStr,
                     pCurr->vid,
                     portStr);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_BLACK_HOLE_MAC
static int32 _cmd_l2_filterMacEntry_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char macStr[CAPA_MAC_STR_LEN];
    sys_l2_entry_t *pPrev = NULL;
    sys_l2_entry_t *pCurr = NULL;

    while (NULL != (pCurr = sal_l2_blacklistMacNext_ret(pPrev)))
    {
        sys_util_mac2Str(pCurr->mac.octet, macStr);

        osal_sprintf(cmd, "mac address-table static %s vlan %u drop\n", macStr, pCurr->vid);

        CMD_WRITE_TO_FILE(fp, cmd);

        pPrev = pCurr;
    }

    return SYS_ERR_OK;
}
#endif

static int32 _cmd_l2_agingtime_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 aging_time;

    osal_memset(cmd, 0, sizeof(cmd));
    SYS_ERR_CHK(sal_l2_aging_get(&aging_time));

    if(DFLT_L2_AGING_TIME != aging_time)
    {
    	if(0 == aging_time)
    	{
			osal_sprintf(cmd, "mac address-table aging-time 0\n");
			CMD_WRITE_TO_FILE(fp, cmd);		
    	}
    	else
    	{
	        osal_sprintf(cmd, "mac address-table aging-time %u\n",aging_time);
	        CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_l2_save(FILE *fp)
{
    
    SYS_ERR_CHK(_cmd_l2_staticMacEntry_save(fp));

#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    
    SYS_ERR_CHK(_cmd_l2_filterMacEntry_save(fp));
#endif

    SYS_ERR_CHK(_cmd_l2_agingtime_save(fp));

    return SYS_ERR_OK;
}

