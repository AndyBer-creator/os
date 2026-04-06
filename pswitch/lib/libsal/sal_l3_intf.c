
#include <common/sys_log.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>

#ifdef CONFIG_SYS_L3_ROUTE
static void _sal_l3_intfDhcpcV6_set(sys_l3_intfId_t *pId, sys_enable_t enable);

static void _sal_l3_vlanAuto_create(sys_vid_t vid)
{
    sys_namestr_t name;

    if (FALSE == sal_vlan_operEntryExist_ret(vid))
    {
        SYS_MEM_CLEAR(name);

        sal_vlan_nameStr_get(vid, DFLT_VLAN_PREFIX_NAME, &name);
        sal_vlan_adminEntry_create(vid, &name);
    }
}

static uint32 _sal_l3_isIntfNumExceed_ret(sys_l3_intfId_t *pId)
{
    uint32 intfNum = 0;

    if (IS_L3_INTF_EXIST(pId))
        return FALSE;

    intfNum = L3_DB_INTFNUM(pId->type);

    switch (pId->type)
    {
        case SYS_L3_INTFTYPE_VLAN:
            if (intfNum >= CAPA_L3_INTFVLAN_NUM)
                return TRUE;
            break;

        case SYS_L3_INTFTYPE_LO:
            if (intfNum >= CAPA_L3_INTFLO_NUM)
                return TRUE;
            break;

        default:
            break;
    }

    return FALSE;
}

static int32 _sal_l3_intfIPv4Addr_update(sys_l3_intfId_t *pId)
{
    char ipStr[CAPA_IPV6_STR_LEN];
    char devStr[CAPA_L3_INTFNAME_LEN];
    sys_l3_ipv4Db_t *pCurr = NULL;
	char bcastStr[CAPA_IPV6_STR_LEN];
	sys_ipv4_t bcast;

    SYS_MEM_CLEAR(devStr);

    INTFID2STR(pId, devStr);

    if (IS_L3_INTF_LINKUP(pId))
    {
        FOR_EACH_L3_IPV4_IN_INTF(pId, pCurr)
        {
            SYS_MEM_CLEAR(ipStr);
            bcast = (0xFFFFFFFF >> pCurr->ip.plen) | pCurr->ip.addr;
			IPV42STR(bcast, bcastStr);

            IPV42STR(pCurr->ip.addr, ipStr);
            SYSTEM_CMD_HID("ip -4 addr add %s/%d broadcast %s dev %s",
                             ipStr,
                             pCurr->ip.plen,
                             bcastStr,
                             devStr);
        }

        rsd_l3_intfGratuitousArp_send(pId);
    }
    else
    {
        
    	SYSTEM_CMD_HID("ip neigh flush dev %s", devStr);

        SYSTEM_CMD_HID("ip -4 addr flush dev %s", devStr);
    }

    return SYS_ERR_OK;
}

static int32 _sal_l3_intfIPv4Valid_chk(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp, sys_l3_ipv4_t *oldIp)
{
    uint32 i = 0;
    sys_l3_ipv4Db_t *pCurr = NULL;
    char ipStr[CAPA_IPV4_STR_LEN];
    char curripStr[CAPA_IPV4_STR_LEN];
    uint8 ipTok[4];
    sys_ipv4_t mask;
#ifndef CONFIG_SYS_L3_ARCHITECTURE
    sys_l3_intfMgmt_t mgmtIntf;
#endif
    
    ipTok[0] = ((pIp->addr >> 24) & 0xFF);
    ipTok[1] = ((pIp->addr >> 16) & 0xFF);
    ipTok[2] = ((pIp->addr >> 8) & 0xFF);
    ipTok[3] = (pIp->addr & 0xFF);

    if (ipTok[0] <= 0 || ipTok[0] >= 224 || ipTok[0] == 127)
        return SYS_ERR_L3_IP_ADDR_INVALID;

    if (pIp->plen < 8)
    {
        return SYS_ERR_L3_IP_MASK_INVALID;
    }
    else if (pIp->plen > 31)
    {
    	
        if (SYS_L3_INTFTYPE_LO != pId->type)
            return SYS_ERR_L3_IP_MASK_INVALID;
    }

    mask = IPV4_PFX2MASK(pIp->plen);

    if ((pIp->plen < 31) && ((pIp->addr & ~mask) == 0 || (pIp->addr & ~mask) == ~mask))
    {
        if (SYS_L3_INTFTYPE_LO != pId->type)
            return SYS_ERR_L3_IP_ADDR_INVALID;
    }

    if ((pIp->addr & mask) == 0)
    {
        if (SYS_L3_INTFTYPE_LO != pId->type)
            return SYS_ERR_L3_IP_ADDR_INVALID;
    }

    SYS_MEM_CLEAR(ipStr);
    IPV42STR(pIp->addr, ipStr);

    FOR_EACH_L3_IPV4(i, pCurr)
    {
        if (IS_IPV4_ALLZERO(pCurr->ip.addr))
            continue;

        SYS_MEM_CLEAR(curripStr);
        IPV42STR(pCurr->ip.addr, curripStr);

        if (pIp->plen <= pCurr->ip.plen)
        {
        #if 0
            if (IS_IPV4_IN_SUBNET(&pIp->addr, &pCurr->ip))
            {
                SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_DUPLICATE,
                                ipStr,
                                pIp->plen,
                                curripStr,
                                pCurr->ip.plen);

                return SYS_ERR_L3_IP_DUPLICATE;
            }
            else
        #endif
           	if (IS_IPV4_IN_SUBNET(&pIp->addr, &pCurr->ip))
            {
                SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_DUPLICATE, ipStr, pIp->plen, curripStr, pCurr->ip.plen);
				osal_memcpy(oldIp, &pCurr->ip, sizeof(sys_l3_ipv4_t));
                
            }
            else
            {
                if (!IS_L3_INTF_EQUAL(pId, &pCurr->intfId)
                        && IS_IPV4_IN_SUBNET(&pCurr->ip.addr, pIp))
                {
                    SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_OVERLAP,
                                ipStr,
                                pIp->plen,
                                curripStr,
                                pCurr->ip.plen);

                    return SYS_ERR_L3_IP_OVERLAP;
                }
            }
        }
        else
        {
            if (!IS_L3_INTF_EQUAL(pId, &pCurr->intfId)
                    && IS_IPV4_IN_SUBNET(&pIp->addr, &pCurr->ip))
            {
                SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_OVERLAP,
                                ipStr,
                                pIp->plen,
                                curripStr,
                                pCurr->ip.plen);

                return SYS_ERR_L3_IP_OVERLAP;
            }
        }
    }
#ifndef CONFIG_SYS_L3_ARCHITECTURE
    
    if (SYS_L3_INTFTYPE_VLAN == pId->type && !IS_MGMT_VID(pId->id))
    {
        SYS_MEM_CLEAR(mgmtIntf);
        sal_l3_intfMgmt_get(&mgmtIntf);
        
        if (IS_IPV4_ALLZERO(mgmtIntf.ipv4.addr))
			return SYS_ERR_OK;

        SYS_MEM_CLEAR(curripStr);
        IPV42STR(mgmtIntf.ipv4.addr, curripStr);

        if (pIp->plen <= mgmtIntf.ipv4.plen)
        {
            if (IS_IPV4_IN_SUBNET(&pIp->addr, &mgmtIntf.ipv4))
            {
                SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_DUPLICATE,
                                ipStr,
                                pIp->plen,
                                curripStr,
                                mgmtIntf.ipv4.plen);

                return SYS_ERR_L3_IP_DUPLICATE;
            }
            else
            {
                if (IS_IPV4_IN_SUBNET(&mgmtIntf.ipv4.addr, pIp))
                {
                    SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_OVERLAP,
                                ipStr,
                                pIp->plen,
                                curripStr,
                                mgmtIntf.ipv4.plen);

                    return SYS_ERR_L3_IP_OVERLAP;
                }
            }
        }
        else
        {
            if (IS_IPV4_IN_SUBNET(&pIp->addr, &mgmtIntf.ipv4))
            {
                SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_OVERLAP,
                                ipStr,
                                pIp->plen,
                                curripStr,
                                mgmtIntf.ipv4.plen);

                return SYS_ERR_L3_IP_OVERLAP;
            }
        }

    }
#endif
    return SYS_ERR_OK;
}

static int32 _sal_l3_intfIPv6Valid_chk(sys_l3_intfId_t *pId, sys_l3_ipv6_t *pIp)
{
    uint32 i = 0;
    sys_l3_ipv6_t subnet;
    sys_l3_ipv6Db_t *pCurr = NULL;
    char ipStr[CAPA_IPV6_STR_LEN];
    char curripStr[CAPA_IPV6_STR_LEN];

    if (IS_IPV6_MCAST(pIp->addr) || IS_IPV6_LOOPBACK(pIp->addr) || IS_IPV6_UNIQLOCA_UCAST(pIp->addr))
        return SYS_ERR_L3_IP_ADDR_INVALID;

    if (pIp->plen == 0)
        return SYS_ERR_L3_IP_MASK_INVALID;

    if (IS_IPV6_ALLZERO(pIp->addr))
        return SYS_ERR_L3_IP_ADDR_INVALID;

    if (pIp->plen != 128)
    {
        SYS_MEM_CLEAR(subnet);
        SYS_ERR_CHK(sys_l3_ip2Subnet_get(SYS_L3_AF_IPV6, (sys_l3_ip_t *)pIp, (sys_l3_ip_t *)&subnet));
        if (IS_IPV6_EQUAL(pIp->addr, subnet.addr))
            return SYS_ERR_L3_IP_ADDR_INVALID;
    }

    SYS_MEM_CLEAR(ipStr);
    IPV62STR(pIp->addr, ipStr);

    FOR_EACH_L3_IPV6(i, pCurr)
    {
        if (IS_IPV6_ALLZERO(pCurr->ip.addr))
            continue;

        if (IS_IPV6_LINKLOCAL(pCurr->ip.addr))
            continue;

        SYS_MEM_CLEAR(curripStr);
        IPV62STR(pCurr->ip.addr, curripStr);

        if (IS_L3_INTF_EQUAL(pId, &pCurr->intfId) && IS_IPV6_EQUAL(pIp->addr, pCurr->ip.addr))
        {
            return SYS_ERR_L3_INTF_IP_EXIST;
        }
        else if (pIp->plen <= pCurr->ip.plen)
        {
            if (IS_IPV6_IN_SUBNET(&pIp->addr, &pCurr->ip))
            {
                SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_DUPLICATE,
                                ipStr,
                                pIp->plen,
                                curripStr,
                                pCurr->ip.plen);

                return SYS_ERR_L3_IP_DUPLICATE;
            }
            else
            {
                if (!IS_L3_INTF_EQUAL(pId, &pCurr->intfId)
                        && IS_IPV6_IN_SUBNET(&pCurr->ip.addr, pIp))
                {
                    SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_OVERLAP,
                                ipStr,
                                pIp->plen,
                                curripStr,
                                pCurr->ip.plen);

                    return SYS_ERR_L3_IP_OVERLAP;
                }
            }
        }
        else
        {
            if (!IS_L3_INTF_EQUAL(pId, &pCurr->intfId)
                    && IS_IPV6_IN_SUBNET(&pIp->addr, &pCurr->ip))
            {
                SYS_LOG(LOG_CAT_L3, LOG_L3_IP_SUBNET_OVERLAP,
                                ipStr,
                                pIp->plen,
                                curripStr,
                                pCurr->ip.plen);

                return SYS_ERR_L3_IP_OVERLAP;
            }
        }
    }

    return SYS_ERR_OK;
}

static int32 _sal_l3_intfIPv6SysLinkLocal_refresh(sys_l3_intfId_t *pId)
{
    sys_l3_ipv6_t linkLocalIp;

    SYS_MEM_CLEAR(linkLocalIp);

    if (SYS_ERR_OK != sys_l3_ipv6LinkLocalAddr_get(pId, &linkLocalIp))
    {
        sys_l3_ipv6SysLinkLocal_get(&linkLocalIp);

        sal_l3_intfIPv6_add(pId, SYS_L3_IPTYPE_V6LINKLOCAL, &linkLocalIp);
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS
static int32 _sal_l3_intfIPv6DhcpStateless_refresh(sys_l3_intfId_t *pId, uint32 isRestart)
{
#define DHCP6C_STOP(_pidFile)                   \
do {                                            \
    int32 _pid = -1;                            \
    FILE *_fp = NULL;                           \
    if (NULL != (_fp = fopen(_pidFile, "r")))   \
    {                                           \
        fscanf(_fp, "%d", &_pid);               \
        fclose(_fp);                            \
        kill(_pid, SIGTERM);                    \
        unlink(_pidFile);                       \
        usleep(100000);                         \
    }                                           \
} while(0)

    FILE *fp = NULL;
    char devStr[CAPA_L3_INTFNAME_LEN];
    char pidFile[SYS_BUF32_LEN];
    char confFile[SYS_BUF32_LEN];
    sys_enable_t enbl;
    sys_l3_ipv6Admin_t ipv6Admin;

    SYS_ERR_CHK(sal_l3_intfIPv6DhcpStatelessEnbl_get(pId, &enbl));

    INTFID2STR(pId, devStr);
    osal_sprintf(pidFile, "/var/run/%s/dhcp6c.pid", devStr);
    osal_sprintf(confFile, "/var/run/%s/dhcp6c.conf", devStr);

    if (ENABLED == enbl && IS_L3_INTF_IPV6ENBL(pId))
    {
        if (IS_L3_INTF_LINKUP(pId))
        {
            SYS_MEM_CLEAR(ipv6Admin);

            sal_l3_adminIntfIPv6_get(pId, &ipv6Admin);

            if (isRestart)
            {
                DHCP6C_STOP(pidFile);
            }
            else
            {
                if (IS_FILE_EXIST(pidFile))
                    return SYS_ERR_OK;
            }

            if (NULL != (fp = fopen(confFile, "w")))
            {
                fprintf(fp, "option refreshtime %u;\n", ipv6Admin.dhcp6c.refreshtime);
                fprintf(fp, "option refreshtime-min %u;\n", ipv6Admin.dhcp6c.min_refreshtime);
                fprintf(fp, "interface %s {\n", devStr);
                fprintf(fp, " send ia-na 1;\n");
                fprintf(fp, " request domain-name-servers,domain-name,refreshtime;\n");
                fprintf(fp, "};\n");
                fprintf(fp, "id-assoc na 1 {\n");
                fprintf(fp, "};\n");

                fclose(fp);
            }

            sal_util_file_touch(pidFile);
            SYSTEM_CMD("dhcp6c -c /var/run/%s/dhcp6c.conf -m stateless %s -p %s",
                            devStr,
                            devStr,
                            pidFile);

            return SYS_ERR_OK;
        }
    }

    DHCP6C_STOP(pidFile);

    return SYS_ERR_OK;
}
#endif

static int32 _sal_l3_intfIPv6Addr_update(sys_l3_intfId_t *pId)
{
    char ipStr[CAPA_IPV6_STR_LEN];
    char devStr[CAPA_L3_INTFNAME_LEN];
    sys_enable_t enable;
    sys_enable_t dhcpEnable;
    uint32 num;
    sys_l3_ipv6Db_t *pCurr = NULL;

    SYS_MEM_CLEAR(devStr);

    INTFID2STR(pId, devStr);

    if (IS_L3_INTF_IPV6ENBL(pId))
    {
        
        SYSTEM_CMD("echo 0 > /proc/sys/net/ipv6/conf/%s/disable_ipv6", devStr);
    }
    else
    {

        SYSTEM_CMD("echo 1 > /proc/sys/net/ipv6/conf/%s/disable_ipv6", devStr);

        return SYS_ERR_OK;
    }

    if (IS_L3_INTF_LINKUP(pId))
    {
        
        sal_l3_intfIPv6AutoconfEnbl_get(pId, &enable);
        sal_l3_intfIPv6DhcpEnbl_get(pId, &dhcpEnable);
        SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/%s/forwarding",
                (ENABLED == enable || ENABLED == dhcpEnable) ? 0 : 1,
                devStr);
        SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/%s/autoconf",
                (ENABLED == enable) ? 1 : 0,
                devStr);

        sal_l3_intfIPv6DadAttempt_get(pId, &num);
        SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/%s/dad_transmits", num, devStr);

        sal_l3_intfIPv6DhcpEnbl_get(pId, &enable);
        _sal_l3_intfDhcpcV6_set(pId, enable);

        FOR_EACH_L3_IPV6_IN_INTF(pId, pCurr)
        {
            SYS_MEM_CLEAR(ipStr);

            IPV62STR(pCurr->ip.addr, ipStr);
            SYSTEM_CMD_HID("ip -6 addr add %s/%d dev %s",
                             ipStr,
                             pCurr->ip.plen,
                             devStr);
        }

        _sal_l3_intfIPv6SysLinkLocal_refresh(pId);
    }
    else
    {
        
        SYSTEM_CMD_HID("ip -6 addr flush dev %s", devStr);

        _sal_l3_intfDhcpcV6_set(pId, DISABLED);
    }

    return SYS_ERR_OK;
}

#endif

static void _sal_l3_intfBootpV4_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    char devName[CAPA_L3_INTFNAME_LEN];
    char dhcpcFilePrefix[SYS_BUF32_LEN];
    char dhcpcFilePid[SYS_BUF32_LEN];
    FILE *fp = NULL;
    int32 pid = 0;

    SYS_MEM_CLEAR(devName);
    SYS_MEM_CLEAR(dhcpcFilePrefix);
    SYS_MEM_CLEAR(dhcpcFilePid);

    INTFID2STR(pId, devName);
    osal_sprintf(dhcpcFilePrefix, "/etc/dhcpc/dhcpcd-%s.", devName);
    osal_sprintf(dhcpcFilePid, "%spid", dhcpcFilePrefix);

    if (ENABLED == enable)
    {
        if (!IS_FILE_EXIST(dhcpcFilePid))
        {
            SYSTEM_CMD_HID_BG("dhcpcd -b -i Switch -t 315360000 %s", devName);
        }
    }
    else
    {
        if (IS_FILE_EXIST(dhcpcFilePid))
        {
            fp = fopen(dhcpcFilePid, "r");
            fscanf(fp, "%d", &pid);
            fclose(fp);

            SYSTEM_CMD_HID("kill -9 %d", pid);

            SYSTEM_CMD("rm -f %s*", dhcpcFilePrefix);

            SYSTEM_CMD("handler -d dflt_route");
        }
    }
}

int32 sal_l3_intfDhcpcV4_renew(sys_l3_intfId_t *pId)
{
    char devName[CAPA_L3_INTFNAME_LEN];
    char dhcpcFilePrefix[SYS_BUF32_LEN];
    char dhcpcFilePid[SYS_BUF32_LEN];
    char maskStr[CAPA_IPV4_STR_LEN];
    FILE *fp = NULL;
    int32 pid = 0;
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(devName);
    SYS_MEM_CLEAR(dhcpcFilePrefix);
    SYS_MEM_CLEAR(dhcpcFilePid);
    SYS_MEM_CLEAR(maskStr);
    SYS_MEM_CLEAR(intfMgmt);

    INTFID2STR(pId, devName);
    osal_sprintf(dhcpcFilePrefix, "/etc/dhcpc/dhcpcd-%s.", devName);
    osal_sprintf(dhcpcFilePid, "%spid", dhcpcFilePrefix);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (SYS_L3_DHCPTYPE_DHCP == intfMgmt.v4.dhcpType)
    {
        if (IS_FILE_EXIST(dhcpcFilePid))
        {
            fp = fopen(dhcpcFilePid, "r");
            fscanf(fp, "%d", &pid);
            fclose(fp);

            SYSTEM_CMD_HID("kill -9 %d", pid);

            SYSTEM_CMD("rm -f %s*", dhcpcFilePrefix);

            SYSTEM_CMD("rm -f /etc/dhcpc/dfltGw");

            SYSTEM_CMD("handler -d dflt_route");
        }

        sleep(2);

        IPV42STR(IPV4_PFX2MASK(DFLT_L3_IPV4_PREFIXLEN), maskStr);
        SYSTEM_CMD_HID("ifconfig %s %s netmask %s", L3_INTF_MGMTVLAN, DFLT_L3_IPV4_ADDR, maskStr);

        if (!IS_FILE_EXIST(dhcpcFilePid))
        {
            SYSTEM_CMD_HID_BG("dhcpcd -i Switch -t 315360000 %s", devName);
        }
    }

    return SYS_ERR_OK;
}

static void _sal_l3_intfDhcpcV4_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    char devName[CAPA_L3_INTFNAME_LEN];
    char dhcpcFilePrefix[SYS_BUF32_LEN];
    char dhcpcFilePid[SYS_BUF32_LEN];
    FILE *fp = NULL;
    int32 pid = 0;

    SYS_MEM_CLEAR(devName);
    SYS_MEM_CLEAR(dhcpcFilePrefix);
    SYS_MEM_CLEAR(dhcpcFilePid);

    INTFID2STR(pId, devName);
    osal_sprintf(dhcpcFilePrefix, "/etc/dhcpc/dhcpcd-%s.", devName);
    osal_sprintf(dhcpcFilePid, "%spid", dhcpcFilePrefix);

    if (ENABLED == enable)
    {
        if (!IS_FILE_EXIST(dhcpcFilePid))
        {
            SYSTEM_CMD_HID_BG("dhcpcd -i Switch -t 315360000 %s", devName);
        }
    }
    else
    {
        if (IS_FILE_EXIST(dhcpcFilePid))
        {
            fp = fopen(dhcpcFilePid, "r");
            fscanf(fp, "%d", &pid);
            fclose(fp);

            SYSTEM_CMD_HID("kill -9 %d", pid);

            SYSTEM_CMD("rm -f %s*", dhcpcFilePrefix);

            SYSTEM_CMD("rm -f /etc/dhcpc/dfltGw");

            SYSTEM_CMD("handler -d dflt_route");
        }
    }
}

static void _sal_l3_intfDhcpcV6_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    char devStr[CAPA_L3_INTFNAME_LEN];
    char pidFile[SYS_BUF32_LEN];
    char confFile[SYS_BUF32_LEN];
    FILE *fp = NULL;
    int32 pid = -1;

#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS
    uint32 isStatelessActive = FALSE;

    if (SYS_ERR_OK == sal_util_file_exist("/var/run/dhcp6c_stateless_active"))
        isStatelessActive = TRUE;
#endif

    SYS_MEM_CLEAR(devStr);
    SYS_MEM_CLEAR(pidFile);
    SYS_MEM_CLEAR(confFile);

    INTFID2STR(pId, devStr);
    osal_sprintf(pidFile, "/var/run/%s/dhcp6c.pid", devStr);
    osal_sprintf(confFile, "/var/run/%s/dhcp6c.conf", devStr);

#ifdef CONFIG_SYS_L3_ROUTE
    if (pId->type != SYS_L3_INTFTYPE_MGMTVLAN)
    {
        sys_enable_t autoconfEnbl = DISABLED;

        sal_l3_intfIPv6AutoconfEnbl_get(pId, &autoconfEnbl);

        if (ENABLED == enable || ENABLED == autoconfEnbl)
        {
            SYSTEM_CMD("echo 0 > /proc/sys/net/ipv6/conf/%s/forwarding", devStr);
        }
        else
        {
            SYSTEM_CMD("echo 1 > /proc/sys/net/ipv6/conf/%s/forwarding", devStr);
        }
    }
#endif

    if (NULL != (fp = fopen(pidFile, "r")))
    {
        fscanf(fp, "%d", &pid);

#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS
        if (TRUE == isStatelessActive)
        {
            if (ENABLED == enable)
                sal_util_file_touch("/var/run/dhcp6c_stateful_start");
            else
                sal_util_file_touch("/var/run/dhcp6c_stateful_stop");

            kill(pid, SIGUSR1);

            usleep(500000); 

            fclose(fp);

            return;
        }
        else
#endif
        {
            kill(pid, SIGTERM);

            unlink(pidFile);

            usleep(500000); 
        }

        fclose(fp);
    }

    if (NULL != (fp = fopen(confFile, "w")))
    {
        fprintf(fp, "interface %s {\n", devStr);
        fprintf(fp, " request domain-name-servers;\n");
        fprintf(fp, " request domain-name;\n");
        fprintf(fp, " send rapid-commit;\n");
        fprintf(fp, " send ia-na 1;\n");

        fprintf(fp, "};\n");
        fprintf(fp, "id-assoc na 1 {\n");
        fprintf(fp, "};\n");

        fclose(fp);
    }

    if (ENABLED == enable)
    {
        
        SYSTEM_CMD("dhcp6c -c %s -p %s -m stateful %s", confFile, pidFile, devStr);

        usleep(500000); 
    }

    return;
}

int32 _sal_l3_intfMgmtV4_active(void)
{
    sys_l3_intfMgmtAdmin_t currMgmtAdmin;
    sys_l3_intfId_t intfId;

    if (!IS_MGMT_VLAN_IPV4_ACTIVE())
        return SYS_ERR_OK;

    SYSTEM_CMD_HID("ip link set dev "L3_INTF_MGMTVLAN" up");

    SYS_MEM_CLEAR(intfId);
    STR2INTFID(L3_INTF_MGMTVLAN, &intfId);

    SYS_MEM_CLEAR(currMgmtAdmin);
    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&currMgmtAdmin));

    if (SYS_L3_DHCPTYPE_DHCP == currMgmtAdmin.v4.dhcpType)
    {
        char maskStr[CAPA_IPV4_STR_LEN];

        SYSTEM_CMD_HID("ip route del default table %s", L3_INTF_MGMTVLAN);

        SYS_MEM_CLEAR(maskStr);
        IPV42STR(IPV4_PFX2MASK(DFLT_L3_IPV4_PREFIXLEN), maskStr);
        SYSTEM_CMD_HID("ifconfig %s %s netmask %s", L3_INTF_MGMTVLAN, DFLT_L3_IPV4_ADDR, maskStr);

        SYSTEM_CMD_HID("ip route add default via %s table %s", DFLT_L3_IPV4_GW, L3_INTF_MGMTVLAN);

        _sal_l3_intfBootpV4_set(&intfId, DISABLED);
        _sal_l3_intfDhcpcV4_set(&intfId, ENABLED);
    }
    else if (SYS_L3_DHCPTYPE_BOOTP == currMgmtAdmin.v4.dhcpType)
    {
        char maskStr[CAPA_IPV4_STR_LEN];

        SYSTEM_CMD_HID("ip route del default table %s", L3_INTF_MGMTVLAN);

        SYS_MEM_CLEAR(maskStr);
        IPV42STR(IPV4_PFX2MASK(DFLT_L3_IPV4_PREFIXLEN), maskStr);
        SYSTEM_CMD_HID("ifconfig %s %s netmask %s", L3_INTF_MGMTVLAN, DFLT_L3_IPV4_ADDR, maskStr);

        SYSTEM_CMD_HID("ip route add default via %s table %s", DFLT_L3_IPV4_GW, L3_INTF_MGMTVLAN);

        _sal_l3_intfDhcpcV4_set(&intfId, DISABLED);
        _sal_l3_intfBootpV4_set(&intfId, ENABLED);
    }
    else if (SYS_L3_DHCPTYPE_DISABLE == currMgmtAdmin.v4.dhcpType)
    {
        char ipStr[CAPA_IPV4_STR_LEN];
        char maskStr[CAPA_IPV4_STR_LEN];

        _sal_l3_intfDhcpcV4_set(&intfId, DISABLED);
        _sal_l3_intfBootpV4_set(&intfId, DISABLED);

        SYS_MEM_CLEAR(ipStr);
        SYS_MEM_CLEAR(maskStr);

        IPV42STR(currMgmtAdmin.v4.ip.addr, ipStr);
        IPV42STR(IPV4_PFX2MASK(currMgmtAdmin.v4.ip.plen), maskStr);
        SYSTEM_CMD_HID("ifconfig %s %s netmask %s", L3_INTF_MGMTVLAN, ipStr, maskStr);

        rsd_l3_intfGratuitousArp_send(&intfId);

        SYSTEM_CMD_HID("ip route del default table %s", L3_INTF_MGMTVLAN);

        if (!IS_IPV4_ALLZERO(currMgmtAdmin.v4.gw))
        {
            SYS_MEM_CLEAR(ipStr);

            if (IS_IPV4_IN_SUBNET(&currMgmtAdmin.v4.gw, &currMgmtAdmin.v4.ip))
            {
                IPV42STR(currMgmtAdmin.v4.gw, ipStr);

                SYSTEM_CMD_HID("ip route add default via %s table %s", ipStr, L3_INTF_MGMTVLAN);
            }
        }

    }

    return SYS_ERR_OK;
}

int32 _sal_l3_intfMgmtSubV4_active(void)
{
    sys_l3_intfMgmtAdmin_t currMgmtAdmin;
    sys_l3_intfId_t intfId;

    if (!IS_MGMT_VLAN_IPV4_ACTIVE())
        return SYS_ERR_OK;

    SYSTEM_CMD_HID("ip link set dev "L3_INTF_MGMTVLAN" up");

    SYS_MEM_CLEAR(intfId);
    STR2INTFID(L3_INTF_MGMTVLAN, &intfId);

    SYS_MEM_CLEAR(currMgmtAdmin);
    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&currMgmtAdmin));

	if (currMgmtAdmin.subv4.ip.addr)
    {
        char ipStr[CAPA_IPV4_STR_LEN];
        char maskStr[CAPA_IPV4_STR_LEN];

        SYS_MEM_CLEAR(ipStr);
        SYS_MEM_CLEAR(maskStr);

        IPV42STR(currMgmtAdmin.subv4.ip.addr, ipStr);
        IPV42STR(IPV4_PFX2MASK(currMgmtAdmin.subv4.ip.plen), maskStr);
        SYSTEM_CMD_HID("ifconfig %s:0 %s netmask %s", L3_INTF_MGMTVLAN, ipStr, maskStr);

        rsd_l3_intfGratuitousArp_send(&intfId);
    }
	else
	{
		SYSTEM_CMD_HID("ifconfig %s:0 down", L3_INTF_MGMTVLAN);
	}

    return SYS_ERR_OK;
}

int32 _sal_l3_intfMgmtV6_active(sys_l3_intfMgmtAdmin_t *pOldMgmtAdmin)
{
    sys_l3_intfMgmtAdmin_t currMgmtAdmin;
    sys_l3_intfId_t intfId;
    uint32 i = 0;
    uint32 j = 0;
    char ipStr[CAPA_IPV6_STR_LEN];

    if (!IS_MGMT_VLAN_IPV6_ACTIVE())
        return SYS_ERR_OK;

    sal_util_dir_create("/var/run/"L3_INTF_MGMTVLAN);

    SYSTEM_CMD_HID("ip link set dev "L3_INTF_MGMTVLAN" up");

    SYS_MEM_CLEAR(intfId);
    STR2INTFID(L3_INTF_MGMTVLAN, &intfId);

    SYS_MEM_CLEAR(currMgmtAdmin);
    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&currMgmtAdmin));

    _sal_l3_intfDhcpcV6_set(&intfId, currMgmtAdmin.v6.dhcpEnbl);

    SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/%s/disable_ipv6",
                   (ENABLED == currMgmtAdmin.v6.enbl) ? 0 : 1,
                   L3_INTF_MGMTVLAN);

    if (ENABLED == currMgmtAdmin.v6.enbl)
    {
        SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/%s/autoconf",
                       (ENABLED == currMgmtAdmin.v6.autoconfEnbl) ? 1 : 0,
                       L3_INTF_MGMTVLAN);

        if (NULL != pOldMgmtAdmin)
        {
            for (i = 0; i < CAPA_L3_INTFM_IPV6ADMIN_NUM; i++)
            {
                uint32 _deleted = TRUE;

                if (IS_IPV6_ALLZERO(pOldMgmtAdmin->v6.ip[i].addr))
                    continue;

                for (j = 0; j < CAPA_L3_INTFM_IPV6ADMIN_NUM; j++)
                {
                    if (IS_IPV6_EQUAL(pOldMgmtAdmin->v6.ip[i].addr, currMgmtAdmin.v6.ip[j].addr))
                    {
                        _deleted = FALSE;
                        break;
                    }
                }

                if (_deleted)
                {
                    SYS_MEM_CLEAR(ipStr);
                    IPV62STR(pOldMgmtAdmin->v6.ip[i].addr, ipStr);
                    SYSTEM_CMD_HID("ip -6 addr del %s/%d dev %s",
                                     ipStr,
                                     pOldMgmtAdmin->v6.ip[i].plen,
                                     L3_INTF_MGMTVLAN);
                }
            }
        }

        for (i = 0; i < CAPA_L3_INTFM_IPV6ADMIN_NUM; i++)
        {
            if (IS_IPV6_ALLZERO(currMgmtAdmin.v6.ip[i].addr))
                continue;

            SYS_MEM_CLEAR(ipStr);

            IPV62STR(currMgmtAdmin.v6.ip[i].addr, ipStr);
            SYSTEM_CMD_HID("ip -6 addr add %s/%d dev %s",
                             ipStr,
                             currMgmtAdmin.v6.ip[i].plen,
                             L3_INTF_MGMTVLAN);
        }

        if (!IS_IPV6_ALLZERO(currMgmtAdmin.v6.gw))
        {
            SYS_MEM_CLEAR(ipStr);

            if (IS_IPV6_IN_SUBNET(&currMgmtAdmin.v6.gw, &currMgmtAdmin.v6.ip))
            {
                IPV62STR(currMgmtAdmin.v6.gw, ipStr);

                SYSTEM_CMD_HID("ip -6 route del default table %s", L3_INTF_MGMTVLAN);
                SYSTEM_CMD_HID("ip -6 route add default via %s table %s", ipStr, L3_INTF_MGMTVLAN);
            }
        }
    }

    return SYS_ERR_OK;
}

int32 _sal_l3_intfMgmt_inactive(sys_l3_af_t af)
{
    sys_l3_intfId_t mgmtId;
    sys_l3_intfId_t intfId;

    SYS_MEM_CLEAR(mgmtId);
    SYS_MEM_CLEAR(intfId);

    STR2INTFID(L3_INTF_MGMTVLAN, &mgmtId);

    if (SYS_L3_AF_END == af || SYS_L3_AF_IPV4 == af)
    {
        SYSTEM_CMD_HID("ip route del default table %s", L3_INTF_MGMTVLAN);

        _sal_l3_intfDhcpcV4_set(&mgmtId, DISABLED);

        SYSTEM_CMD_HID("ip -4 addr flush dev %s", L3_INTF_MGMTVLAN);
    }
    else if (SYS_L3_AF_END == af || SYS_L3_AF_IPV6 == af)
    {
		SYSTEM_CMD_HID("ip -6 route del default table %s", L3_INTF_MGMTVLAN);

        _sal_l3_intfDhcpcV6_set(&mgmtId, DISABLED);

        SYSTEM_CMD_HID("ip -6 addr flush dev %s", L3_INTF_MGMTVLAN);
    }

    intfId.type = SYS_L3_INTFTYPE_VLAN;
    intfId.id = mgmtId.id;

    if ((SYS_L3_AF_END == af)
        || (SYS_L3_AF_IPV4 == af && !IS_MGMT_VLAN_IPV6_ACTIVE())
        || (SYS_L3_AF_IPV6 == af && !IS_MGMT_VLAN_IPV4_ACTIVE()))
    {
        SYSTEM_CMD_HID("ip link set dev "L3_INTF_MGMTVLAN" down");
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_L3_ROUTE

int32 sal_l3_adminIntf_get(sys_l3_intfId_t *pId, sys_l3_intfAdmin_t *pAdmin)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pAdmin), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_intf_get(pId, pAdmin));

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfEnbl_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    sys_l3_intfAdmin_t admin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    if ((ENABLED == enable) && (SYS_L3_INTFTYPE_VLAN == pId->type))
        _sal_l3_vlanAuto_create(pId->id);

    SYS_ERR_CHK(cfg_l3_intf_get(pId, &admin));

    if (admin.enbl == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_l3_adminIntfEnbl_set(pId, enable));

    admin.enbl = enable;
    SYS_ERR_CHK(cfg_l3_intf_set(pId, &admin));

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfEnbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    sys_l3_intfAdmin_t admin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_intf_get(pId, &admin));

    *pEnable = admin.enbl;

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfExist_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    sys_l3_intfAdmin_t admin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    if ((ENABLED == enable) && (SYS_L3_INTFTYPE_VLAN == pId->type))
        _sal_l3_vlanAuto_create(pId->id);

    SYS_ERR_CHK(cfg_l3_intf_get(pId, &admin));

    if (admin.exist == enable)
        return SYS_ERR_OK;

    admin.exist = enable;
    SYS_ERR_CHK(cfg_l3_intf_set(pId, &admin));

    if (DISABLED == enable)
    {
        cfg_l3_intf_init(pId);
    }

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfExist_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    sys_l3_intfAdmin_t admin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_intf_get(pId, &admin));

    *pEnable = admin.exist;

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfDhcpV4_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    sys_l3_ipv4Admin_t admin;
    sys_l3_ipv4_t ip;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(admin);

    SYS_ERR_CHK(cfg_l3_intfIPv4_get(pId, &admin));

    if (admin.dhcpEnbl == enable)
        return SYS_ERR_OK;

    if (ENABLED == enable && _sal_l3_isIntfNumExceed_ret(pId))
        return SYS_ERR_L3_INTF_NUM_EXCEED;

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        _sal_l3_vlanAuto_create(pId->id);

    admin.dhcpEnbl = enable;
    SYS_ERR_CHK(cfg_l3_intfIPv4_set(pId, &admin));

    if (ENABLED == enable)
    {
        
        sal_l3_intfIPv4_clear(pId, SYS_L3_IPTYPE_ANY);

        SYS_MEM_CLEAR(ip);
        ip.plen = 32;

        SYS_ERR_CHK(rsd_l3_intfIp_add(SYS_L3_AF_IPV4, pId, SYS_L3_IPTYPE_DHCP, (sys_l3_ip_t*)&ip));
    }
    else
    {
        
        SYSTEM_CMD("handler -d dflt_route");

        sal_l3_intfIPv4_clear(pId, SYS_L3_IPTYPE_ANY);
    }

    _sal_l3_intfDhcpcV4_set(pId, enable);

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfDhcpV4_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    sys_l3_ipv4Admin_t admin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(admin);

    SYS_ERR_CHK(cfg_l3_intfIPv4_get(pId, &admin));

    *pEnable = admin.dhcpEnbl;

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfIPv4_get(sys_l3_intfId_t *pId, sys_l3_ipv4Admin_t *pIpAdmin)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIpAdmin), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_intfIPv4_get(pId, pIpAdmin));

    return SYS_ERR_OK;
}

int32 sal_l3_intfStaticIPv4_add(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp)
{
    uint32 inactMgmtIntf = FALSE;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    if (SYS_L3_INTFTYPE_VLAN == pId->type && IS_MGMT_VID(pId->id) && 0 == L3_INTF_IPV4_NUM(pId))
        inactMgmtIntf = TRUE;

    SYS_ERR_CHK(sal_l3_intfIPv4_add(pId, SYS_L3_IPTYPE_STATIC, pIp));

    if (inactMgmtIntf)
        _sal_l3_intfMgmt_inactive(SYS_L3_AF_IPV4);

    return SYS_ERR_OK;
}
int32 sal_l3_intfStaticIPv4_sub_add(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp)
{
    uint32 inactMgmtIntf = FALSE;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    if (SYS_L3_INTFTYPE_VLAN == pId->type && IS_MGMT_VID(pId->id) && 0 == L3_INTF_IPV4_NUM(pId))
        inactMgmtIntf = TRUE;

    SYS_ERR_CHK(sal_l3_intfIPv4_sub_add(pId, SYS_L3_IPTYPE_STATIC, pIp));

    if (inactMgmtIntf)
        _sal_l3_intfMgmt_inactive(SYS_L3_AF_IPV4);

    return SYS_ERR_OK;
}

int32 sal_l3_intfStaticIPv4_del(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);
	uint32 delNum = 0;
	sys_l3_ipv4Db_t delAddrs[CAPA_L3_IPV4POOL_NUM];
	sys_l3_ipv4Db_t *pCurr = NULL;

	SYS_MEM_CLEAR(delAddrs);

    if (!IS_L3_INTF_IPV4_EXIST(pId, pIp))
        return SYS_ERR_L3_INTF_IP_NOT_EXIST;

    FOR_EACH_L3_IPV4_IN_INTF(pId, pCurr)
    {
        if (SYS_L3_IPTYPE_ANY != SYS_L3_IPTYPE_STATIC)
        {
            if (pCurr->list.type != SYS_L3_IPTYPE_STATIC)
                continue;
        }

        delNum++;
    }

    if(delNum > 1)
    	return SYS_ERR_L3_INTF_IP_SUB_EXIST;
	
    SYS_ERR_CHK(sal_l3_intfIPv4_del(pId, SYS_L3_IPTYPE_STATIC, pIp));

    return SYS_ERR_OK;
}

int32 sal_l3_intfStaticIPv4_sub_del(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);
	uint32 delNum = 0;

	sys_l3_ipv4Db_t *pCurr = NULL;

    if (!IS_L3_INTF_IPV4_EXIST(pId, pIp))
        return SYS_ERR_L3_INTF_IP_NOT_EXIST;

    FOR_EACH_L3_IPV4_IN_INTF(pId, pCurr)
    {
        if (SYS_L3_IPTYPE_ANY != SYS_L3_IPTYPE_STATIC)
        {
            if (pCurr->list.type != SYS_L3_IPTYPE_STATIC)
                continue;
        }
		delNum++;

        if(pIp->addr == pCurr->ip.addr && delNum == 1) 
        {
			return SYS_ERR_L3_INTF_IP_DEL_SUB;
        }   
    }
    		
    SYS_ERR_CHK(sal_l3_intfIPv4_del(pId, SYS_L3_IPTYPE_STATIC, pIp));

    return SYS_ERR_OK;
}

int32 sal_l3_intfStaticIPv4_clear(sys_l3_intfId_t *pId)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_l3_intfIPv4_clear(pId, SYS_L3_IPTYPE_STATIC));

    return SYS_ERR_OK;
}
#endif

int32 sal_l3_intf_primary_IPv4_find(sys_l3_intfId_t *pId, sys_l3_ipv4_t *priIPv4)
{
    sys_l3_ipv4Db_t *pCurr = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == priIPv4), SYS_ERR_NULL_POINTER);

    FOR_EACH_L3_IPV4_IN_INTF(pId, pCurr)
    {
        osal_memcpy(priIPv4, &pCurr->ip, sizeof(sys_l3_ipv4_t));
        return SYS_ERR_OK;
    }

    return SYS_ERR_ENTRY_NOT_FOUND;
}

int32 sal_l3_intfIPv4_add(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv4_t *pIp)
{
#ifdef CONFIG_SYS_L3_ROUTE
    sys_l3_ipv4Db_t *pIpDb = NULL;
    sys_l3_ipv4_t delIp;
#endif
	sys_ipv4_t bcast = 0;
	sys_l3_ipv4_t oldIpv4;
	sys_l3_ipv4_t firstIpv4;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    if (SYS_L3_INTFTYPE_MGMTVLAN == pId->type)
    {
        SYS_ERR_CHK(rsd_l3_intfIp_add(SYS_L3_AF_IPV4, pId, type, (sys_l3_ip_t*)pIp));

        return SYS_ERR_OK;
    }
#ifdef CONFIG_SYS_L3_VRRP
    else  if (SYS_L3_INTFTYPE_VRRP == pId->type)
    {
        SYS_ERR_CHK(rsd_l3_intfIp_add(SYS_L3_AF_IPV4, pId, type, (sys_l3_ip_t*)pIp));

        return SYS_ERR_OK;
    }
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    SYS_MEM_CLEAR(delIp);
	SYS_MEM_CLEAR(oldIpv4);
	SYS_MEM_CLEAR(firstIpv4);

    SYS_ERR_CHK(_sal_l3_intfIPv4Valid_chk(pId, pIp, &oldIpv4));
	if(oldIpv4.addr)
	{
		SYS_ERR_CHK(sal_l3_intfStaticIPv4_del(pId, &oldIpv4));
	}

	if(sal_l3_intf_primary_IPv4_find(pId, &firstIpv4) == SYS_ERR_OK && firstIpv4.addr)
	{
		SYS_ERR_CHK(sal_l3_intfStaticIPv4_del(pId, &firstIpv4));
	}
	
    if (_sal_l3_isIntfNumExceed_ret(pId))
        return SYS_ERR_L3_INTF_NUM_EXCEED;

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        _sal_l3_vlanAuto_create(pId->id);

    if (!IS_IPV4_ALLZERO(pIp->addr))
    {
        if (NULL != (pIpDb = (sys_l3_ipv4Db_t*)sys_l3_ipPoolIntfEntryPtr_ret(SYS_L3_AF_IPV4, pId, (sys_l3_ip_t *)pIp)))
            osal_memcpy(&delIp, &pIpDb->ip, sizeof(sys_l3_ipv4_t));
    }

    SYS_ERR_CHK(rsd_l3_intfIp_add(SYS_L3_AF_IPV4, pId, type, (sys_l3_ip_t*)pIp));

    if (SYS_L3_IPTYPE_STATIC == type)
    {
        char ipStr[CAPA_IPV4_STR_LEN];
        char devStr[CAPA_L3_INTFNAME_LEN];
        char bcastStr[CAPA_IPV4_STR_LEN];

        if (IS_L3_INTF_LINKUP(pId))
        {
            SYS_MEM_CLEAR(ipStr);
            IPV42STR(pIp->addr, ipStr);
            INTFID2STR(pId, devStr);
            
            bcast = (0xFFFFFFFF >> pIp->plen) | pIp->addr;
			IPV42STR(bcast, bcastStr);

            if (!IS_IPV4_ALLZERO(delIp.addr))
            {
                SYSTEM_CMD_HID("ip -4 addr del %s/%d dev %s", ipStr, delIp.plen, devStr);
            }

            SYSTEM_CMD_HID("ip -4 addr add %s/%d broadcast %s dev %s", ipStr, pIp->plen, bcastStr, devStr);

            rsd_l3_intfGratuitousArp_send(pId);
        }
    }
#endif

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv4_sub_add(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv4_t *pIp)
{
#ifdef CONFIG_SYS_L3_ROUTE
    sys_l3_ipv4Db_t *pIpDb = NULL;
    sys_l3_ipv4_t delIp;
#endif
	sys_ipv4_t bcast = 0;
	sys_l3_ipv4_t oldIpv4;
	sys_l3_ipv4_t firstIpv4;
	int rc = SYS_ERR_OK;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    if (SYS_L3_INTFTYPE_MGMTVLAN == pId->type)
    {
        SYS_ERR_CHK(rsd_l3_intfIp_add(SYS_L3_AF_IPV4, pId, type, (sys_l3_ip_t*)pIp));

        return SYS_ERR_OK;
    }

#ifdef CONFIG_SYS_L3_ROUTE
    SYS_MEM_CLEAR(delIp);
	SYS_MEM_CLEAR(oldIpv4);
	SYS_MEM_CLEAR(firstIpv4);

	rc = sal_l3_intf_primary_IPv4_find(pId, &firstIpv4);
	if(rc == SYS_ERR_ENTRY_NOT_FOUND)
	{
		return SYS_ERR_L3_INTF_PRIMARY_IP_NOT_EXIST;
	}
	
	if(osal_memcmp(&firstIpv4, pIp, sizeof(sys_l3_ipv4_t)) == 0)
	{
		return SYS_ERR_L3_INTF_PRIMARY_IP_EXIST;
	}

    SYS_ERR_CHK(_sal_l3_intfIPv4Valid_chk(pId, pIp, &oldIpv4));
	if(oldIpv4.addr)
	{
		SYS_ERR_CHK(sal_l3_intfStaticIPv4_del(pId, &oldIpv4));
	}

    if (_sal_l3_isIntfNumExceed_ret(pId))
        return SYS_ERR_L3_INTF_NUM_EXCEED;

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        _sal_l3_vlanAuto_create(pId->id);

    if (!IS_IPV4_ALLZERO(pIp->addr))
    {
        if (NULL != (pIpDb = (sys_l3_ipv4Db_t*)sys_l3_ipPoolIntfEntryPtr_ret(SYS_L3_AF_IPV4, pId, (sys_l3_ip_t *)pIp)))
            osal_memcpy(&delIp, &pIpDb->ip, sizeof(sys_l3_ipv4_t));
    }

    SYS_ERR_CHK(rsd_l3_intfIp_add(SYS_L3_AF_IPV4, pId, type, (sys_l3_ip_t*)pIp));

    if (SYS_L3_IPTYPE_STATIC == type)
    {
        char ipStr[CAPA_IPV4_STR_LEN];
        char devStr[CAPA_L3_INTFNAME_LEN];
        char bcastStr[CAPA_IPV4_STR_LEN];

        if (IS_L3_INTF_LINKUP(pId))
        {
            SYS_MEM_CLEAR(ipStr);
            IPV42STR(pIp->addr, ipStr);
            INTFID2STR(pId, devStr);
            
            bcast = (0xFFFFFFFF >> pIp->plen) | pIp->addr;
			IPV42STR(bcast, bcastStr);

            if (!IS_IPV4_ALLZERO(delIp.addr))
            {
                SYSTEM_CMD_HID("ip -4 addr del %s/%d dev %s", ipStr, delIp.plen, devStr);
            }

            SYSTEM_CMD_HID("ip -4 addr add %s/%d broadcast %s dev %s", ipStr, pIp->plen, bcastStr, devStr);

            rsd_l3_intfGratuitousArp_send(pId);
        }
    }
#endif

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv4_del(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv4_t *pIp)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l3_intfIp_del(SYS_L3_AF_IPV4, pId, type, (sys_l3_ip_t*)pIp));

#ifdef CONFIG_SYS_L3_ROUTE
    if (SYS_L3_INTFTYPE_MGMTVLAN == pId->type)
        return SYS_ERR_OK;

    if (SYS_L3_IPTYPE_STATIC == type || SYS_L3_IPTYPE_DHCP == type)
    {
        char ipStr[CAPA_IPV4_STR_LEN];
        char devStr[CAPA_L3_INTFNAME_LEN];

        SYS_MEM_CLEAR(ipStr);
        IPV42STR(pIp->addr, ipStr);
        INTFID2STR(pId, devStr);
        SYSTEM_CMD_HID("ip -4 addr del %s dev %s", ipStr, devStr);
    }

    SYSTEM_CMD_BG("handler -c ipchg");
#endif

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv4_sub_del(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l3_intfIp_del(SYS_L3_AF_IPV4, pId, SYS_L3_IPTYPE_STATIC, (sys_l3_ip_t*)pIp));

#ifdef CONFIG_SYS_L3_ROUTE
    {
        char ipStr[CAPA_IPV4_STR_LEN];
        char devStr[CAPA_L3_INTFNAME_LEN];

        SYS_MEM_CLEAR(ipStr);
        IPV42STR(pIp->addr, ipStr);
        INTFID2STR(pId, devStr);
        SYSTEM_CMD_HID("ip -4 addr del %s dev %s", ipStr, devStr);
    }

    SYSTEM_CMD_BG("handler -c ipchg");
#endif

    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_L3_VRRP

int32 sal_l3_vrrp_intf_IPv4_add(int vlanid, int vrid, sys_ipv4_t vip)
{

	char devStr[CAPA_L3_INTFNAME_LEN];
	char ipStr[CAPA_IPV4_STR_LEN];
	sys_l3_intfId_t ifid;
	sys_l3_ipv4_t vIpv4;

	SYS_MEM_CLEAR(ipStr);
    SYS_MEM_CLEAR(vIpv4);
	SYS_MEM_CLEAR(devStr);
	SYS_MEM_CLEAR(ifid);

	ifid.type = SYS_L3_INTFTYPE_VRRP;
	ifid.id = vlanid;
	ifid.vrid = vrid;
	vIpv4.addr = vip;
	vIpv4.plen = 24;
	sal_l3_intfIPv4_add(&ifid, SYS_L3_IPTYPE_VRRP, &vIpv4);

    {
        
        {
            
            IPV42STR(vip, ipStr);
			osal_sprintf(devStr, "vrrp%d", vrid);

        }
    }

    return SYS_ERR_OK;
}

int32 sal_l3_vrrp_intf_IPv4_del(int vlanid, int vrid, sys_ipv4_t vip)
{

	char devStr[CAPA_L3_INTFNAME_LEN];
	char ipStr[CAPA_IPV4_STR_LEN];
	sys_l3_intfId_t ifid;
	sys_l3_ipv4_t vIpv4;

	SYS_MEM_CLEAR(ipStr);
    SYS_MEM_CLEAR(vIpv4);
	SYS_MEM_CLEAR(devStr);
	SYS_MEM_CLEAR(ifid);

	ifid.type = SYS_L3_INTFTYPE_VRRP;
	ifid.id = vlanid;
	ifid.vrid = vrid;
	vIpv4.addr = vip;
	vIpv4.plen = 24;
	sal_l3_intfIPv4_del(&ifid, SYS_L3_IPTYPE_VRRP, &vIpv4);

    {
        
        {
            
            IPV42STR(vip, ipStr);
			osal_sprintf(devStr, "vrrp%d", vrid);

        }
    }

    return SYS_ERR_OK;
}

int32 sal_l3_vrrp_send_gratuitous_arp(uint32 vlanid, uint32 vrid, uint32 bvmac)
{
	sys_l3_intfId_t pId;
	
	SYS_MEM_CLEAR(pId);
	if(bvmac)
	{
		pId.type = SYS_L3_INTFTYPE_VRRP;
	}
	else 
	{
		pId.type = SYS_L3_INTFTYPE_VLAN;
	}
	pId.id = vlanid;
	pId.vrid = vrid;
	
	rsd_l3_intfGratuitousArp_send(&pId);

	return SYS_ERR_OK;
}

int32 sal_l3_vrrp_intf_netdev(int vlanid, int vrid)
{

    return SYS_ERR_OK;
}

#endif

#ifdef CONFIG_SYS_L3_ROUTE

int32 sal_l3_intfIPv4_clear(sys_l3_intfId_t *pId, sys_l3_ipType_t ipType)
{
    sys_l3_ipv4Db_t *pCurr = NULL;
    sys_l3_ipv4Db_t delAddrs[CAPA_L3_IPV4POOL_NUM];
    uint32 i = 0;
    uint32 delNum = 0;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(delAddrs);

    FOR_EACH_L3_IPV4_IN_INTF(pId, pCurr)
    {
        if (SYS_L3_IPTYPE_ANY != ipType)
        {
            if (pCurr->list.type != ipType)
                continue;
        }

        osal_memcpy(&delAddrs[delNum], pCurr, sizeof(sys_l3_ipv4Db_t));

        delNum++;
    }

    for (i = 0; i < delNum; i++)
    {
        sal_l3_intfIPv4_del(pId, delAddrs[i].list.type, &delAddrs[i].ip);
    }

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv4_get(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp)
{
    sys_l3_ipv4Db_t *pCurr = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    FOR_EACH_L3_IPV4_IN_INTF(pId, pCurr)
    {
        osal_memcpy(pIp, &pCurr->ip, sizeof(sys_l3_ipv4_t));
        break;
    }

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv4_ip_equal(sys_ipv4_t ip)
{
    sys_l3_ipv4Db_t *pCurr = NULL;
	int i = 0;

	FOR_EACH_L3_IPV4(i, pCurr)
	{
		if (IS_IPV4_ALLZERO(pCurr->ip.addr))
            continue;
#ifdef CONFIG_SYS_L3_VRRP
		if (pCurr->intfId.type == SYS_L3_INTFTYPE_VRRP)
            continue;
#endif
		if (IS_IPV4_EQUAL(ip, pCurr->ip.addr))
			return TRUE;
    }

    return FALSE;
}

int32 sal_l3_Mgmt_IPv4_ip_equal(sys_ipv4_t ip)
{

	sys_l3_intfMgmtAdmin_t intfMgmt;

	sal_l3_adminIntfMgmt_get(&intfMgmt);

	if(osal_memcmp(&intfMgmt.v4.ip.addr, &ip, sizeof(sys_ipv4_t)) == 0)
	{
		return TRUE;
	}

    return FALSE;
}

int32 sal_l3_intf_network_equal(int vidIndex, sys_ipv4_t pIp)
{
	sys_l3_intfId_t intfid;
	sys_l3_ipv4_t ipv4;
	sys_l3_ipv4_t check_ipv4;

	intfid.id = vidIndex;
	intfid.type = SYS_L3_INTFTYPE_VLAN;
	SYS_MEM_CLEAR(ipv4);
	
	sal_l3_intfIPv4_get(&intfid, &ipv4);
	check_ipv4.addr = pIp;
	check_ipv4.plen = ipv4.plen;
	if(osal_memcmp(&ipv4, &check_ipv4, sizeof(sys_l3_ipv4_t)) == 0)
	{
		return TRUE;
	}
    return FALSE;
}
int32 sal_l3_intf_network_chk(int vidIndex, sys_ipv4_t pIp)
{
	sys_l3_intfId_t intfid;
	sys_l3_ipv4_t ipv4;
	uint32 mask = 0;
	intfid.id = vidIndex;
	intfid.type = SYS_L3_INTFTYPE_VLAN;
	SYS_MEM_CLEAR(ipv4);
	sal_l3_intfIPv4_get(&intfid, &ipv4);
	mask = IPV4_PFX2MASK(ipv4.plen);
	if ((ipv4.addr & mask) == (pIp & mask))
	{
		return TRUE;
	}

    return FALSE;
}

int32 sal_IPv4Valid_chk(sys_ipv4_t ipv4)
{
    uint8 ipTok[4];

    ipTok[0] = ((ipv4 >> 24) & 0xFF);

    if (ipTok[0] <= 0 || ipTok[0] >= 224 || ipTok[0] == 127)
        return SYS_ERR_L3_IP_ADDR_INVALID;
    
    return SYS_ERR_OK;
}

int32 sal_l3_intf_ipv4_by_vid_get(int vidIndex, sys_ipv4_t* pIp)
{
	sys_l3_intfId_t intfid;
	sys_l3_ipv4_t ipv4;

	intfid.id = vidIndex;
	intfid.type = SYS_L3_INTFTYPE_VLAN;
	SYS_MEM_CLEAR(ipv4);
	
	sal_l3_intfIPv4_get(&intfid, &ipv4);
	if(ipv4.addr)
	{
		memcpy(pIp, &ipv4.addr, sizeof(sys_l3_ipv4_t));
		return SYS_ERR_OK;
	}
	
    return SYS_ERR_FAILED;
}

#endif

int32 sal_l3_intfIPv6_add(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv6_t *pIp)
{
#ifdef CONFIG_SYS_L3_ROUTE
    char ipStr[CAPA_IPV6_STR_LEN];
    char devStr[CAPA_L3_INTFNAME_LEN];
    sys_l3_ipv6_t linkLocalIp;
#endif

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    if (SYS_L3_INTFTYPE_MGMTVLAN == pId->type)
    {
        SYS_ERR_CHK(rsd_l3_intfIp_add(SYS_L3_AF_IPV6, pId, type, (sys_l3_ip_t*)pIp));

        return SYS_ERR_OK;
    }

#ifdef CONFIG_SYS_L3_ROUTE
    SYS_MEM_CLEAR(linkLocalIp);

    SYS_ERR_CHK(_sal_l3_intfIPv6Valid_chk(pId, pIp));

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        _sal_l3_vlanAuto_create(pId->id);

    if ((SYS_L3_IPTYPE_V6LINKLOCAL == type)
            && (SYS_ERR_OK == sys_l3_ipv6LinkLocalAddr_get(pId, &linkLocalIp)))
    {
        SYS_MEM_CLEAR(ipStr);
        IPV62STR(linkLocalIp.addr, ipStr);
        INTFID2STR(pId, devStr);
        SYSTEM_CMD_HID("ip -6 addr del %s/%d dev %s",
                       ipStr,
                       linkLocalIp.plen,
                       devStr);
    }

    SYS_ERR_CHK(rsd_l3_intfIp_add(SYS_L3_AF_IPV6, pId, type, (sys_l3_ip_t*)pIp));

    if ((SYS_L3_IPTYPE_STATIC == type || SYS_L3_IPTYPE_V6LINKLOCAL == type)
            && IS_L3_INTF_LINKUP(pId))
    {
        
        SYS_MEM_CLEAR(ipStr);
        IPV62STR(pIp->addr, ipStr);
        INTFID2STR(pId, devStr);
        SYSTEM_CMD_HID("ip -6 addr add %s/%d dev %s", ipStr, pIp->plen, devStr);

        SYSTEM_CMD_BG("handler -f ipv6_dup_log %s %s %d", devStr, ipStr, pIp->plen);
    }
#endif

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6_del(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv6_t *pIp)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l3_intfIp_del(SYS_L3_AF_IPV6, pId, type, (sys_l3_ip_t*)pIp));

#ifdef CONFIG_SYS_L3_ROUTE
    if (SYS_L3_INTFTYPE_MGMTVLAN == pId->type)
    {
        return SYS_ERR_OK;
    }

    if (SYS_L3_IPTYPE_STATIC == type || SYS_L3_IPTYPE_V6LINKLOCAL == type)
    {
        char ipStr[CAPA_IPV6_STR_LEN];
        char devStr[CAPA_L3_INTFNAME_LEN];

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(pIp->addr, ipStr);
        INTFID2STR(pId, devStr);
        SYSTEM_CMD_HID("ip -6 addr del %s/%d dev %s", ipStr, pIp->plen, devStr);
    }

    _sal_l3_intfIPv6SysLinkLocal_refresh(pId);

    SYSTEM_CMD_BG("handler -c ipchg");
#endif

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_L3_ROUTE

int32 sal_l3_intfIPv6_clear(sys_l3_intfId_t *pId, sys_l3_ipType_t ipType)
{
    sys_l3_ipv6Db_t *pCurr = NULL;
    sys_l3_ipv6Db_t delAddrs[CAPA_L3_IPV6POOL_NUM];
    uint32 i = 0;
    uint32 delNum = 0;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(delAddrs);

    FOR_EACH_L3_IPV6_IN_INTF(pId, pCurr)
    {
        if (SYS_L3_IPTYPE_ANY != ipType)
        {
            if (pCurr->list.type != ipType)
                continue;
        }

        osal_memcpy(&delAddrs[delNum], pCurr, sizeof(sys_l3_ipv6Db_t));

        delNum++;
    }

    for (i = 0; i < delNum; i++)
    {
        sal_l3_intfIPv6_del(pId, delAddrs[i].list.type, &delAddrs[i].ip);
    }

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6_refresh(sys_l3_intfId_t *pId)
{
    char intfName[CAPA_L3_INTFNAME_LEN];
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_MEM_CLEAR(intfName);
    INTFID2STR(pId, intfName);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    if (ENABLED == ipAdmin.enbl)
    {
        SYSTEM_CMD("echo 1 > /proc/sys/net/ipv6/conf/%s/disable_ipv6", intfName);
        SYSTEM_CMD("echo 0 > /proc/sys/net/ipv6/conf/%s/disable_ipv6", intfName);
    }

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfIPv6_get(sys_l3_intfId_t *pId, sys_l3_ipv6Admin_t *pIpAdmin)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIpAdmin), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, pIpAdmin));

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfIPv6Enbl_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);
    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    if (enable == ipAdmin.enbl)
        return SYS_ERR_OK;

    if (ENABLED == enable && _sal_l3_isIntfNumExceed_ret(pId))
        return SYS_ERR_L3_INTF_NUM_EXCEED;

    if (DISABLED == enable && (sal_l3_intfStaticIPv6AddrNum_ret(pId) > 0))
    {
        return SYS_ERR_L3_INTF_DISABLE_IPV6_ADDR_EXIST;
    }

    if (ENABLED == enable && SYS_L3_INTFTYPE_VLAN == pId->type)
        _sal_l3_vlanAuto_create(pId->id);

    SYS_ERR_CHK(rsd_l3_adminIntfIPv6Enbl_set(pId, enable));

    ipAdmin.enbl = enable;
    SYS_ERR_CHK(cfg_l3_intfIPv6_set(pId, &ipAdmin));

    _sal_l3_intfIPv6Addr_update(pId);
#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS
    _sal_l3_intfIPv6DhcpStateless_refresh(pId, TRUE);
#endif

    if (IS_MGMT_VID(pId->id))
    {
        if (ENABLED == enable)
            _sal_l3_intfMgmt_inactive(SYS_L3_AF_IPV6);
        else
            _sal_l3_intfMgmtV6_active(NULL);
    }

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfIPv6Enbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    *pEnable = ipAdmin.enbl;

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfIPv6Mode_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);
    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    ipAdmin.mode = enable;

    SYS_ERR_CHK(cfg_l3_intfIPv6_set(pId, &ipAdmin));

    if (ENABLED == enable)
    {
        sal_l3_adminIntfIPv6Enbl_set(pId, enable);
    }
    else
    {
        
        if (0 == sal_l3_intfStaticIPv6AddrNum_ret(pId))
        {
            sys_enable_t autoconf = DISABLED;

            sal_l3_intfIPv6AutoconfEnbl_get(pId, &autoconf);

            if (DISABLED == autoconf)
                sal_l3_adminIntfIPv6Enbl_set(pId, DISABLED);
        }
    }

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfIPv6Mode_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    *pEnable = ipAdmin.mode;

    return SYS_ERR_OK;
}

int32 sal_l3_intfStaticIPv6_add(sys_l3_intfId_t *pId, sys_l3_ipv6_t *pIp)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    if (IS_IPV6_LINKLOCAL(pIp->addr))
    {
        SYS_ERR_CHK(sal_l3_intfIPv6_add(pId, SYS_L3_IPTYPE_V6LINKLOCAL, pIp));
    }
    else
    {
        if (L3_INTF_IPV6_NUM(pId) >= (CAPA_L3_INTF_STATIC_IPV6_NUM + 1))
        {
            return SYS_ERR_L3_INTF_IP_NUM_EXCEED;
        }

        SYS_ERR_CHK(sal_l3_intfIPv6_add(pId, SYS_L3_IPTYPE_STATIC, pIp));

        if (!IS_L3_INTF_IPV6ENBL(pId))
        {
            sal_l3_adminIntfIPv6Enbl_set(pId, ENABLED);
        }
    }

    return SYS_ERR_OK;
}

int32 sal_l3_intfStaticIPv6_del(sys_l3_intfId_t *pId, sys_l3_ipv6_t *pIp)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    if (!IS_L3_INTF_IPV6_EXIST(pId, pIp))
        return SYS_ERR_L3_INTF_IP_NOT_EXIST;

    SYS_ERR_CHK(sal_l3_intfIPv6_del(pId, SYS_L3_IPTYPE_STATIC, pIp));

    return SYS_ERR_OK;
}

int32 sal_l3_intfStaticIPv6_clear(sys_l3_intfId_t *pId)
{
    sys_l3_ipv6_t linkLocalIp;
    sys_l3_ipv6_t sysLinkLocalIp;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_l3_intfIPv6_clear(pId, SYS_L3_IPTYPE_STATIC));

    SYS_MEM_CLEAR(linkLocalIp);

    if (SYS_ERR_OK == sys_l3_ipv6LinkLocalAddr_get(pId, &linkLocalIp))
    {
        SYS_MEM_CLEAR(sysLinkLocalIp);

        sys_l3_ipv6SysLinkLocal_get(&sysLinkLocalIp);

        if (!IS_IPV6_EQUAL(linkLocalIp.addr, sysLinkLocalIp.addr))
            SYS_ERR_CHK(sal_l3_intfIPv6_clear(pId, SYS_L3_IPTYPE_V6LINKLOCAL));
    }

    return SYS_ERR_OK;
}

int32 sal_l3_intfStaticIPv6AddrNum_ret(sys_l3_intfId_t *pId)
{
    uint32 num = 0;
    sys_l3_ipv6Db_t *pIPv6Entry = NULL;

    FOR_EACH_L3_IPV6_IN_INTF(pId, pIPv6Entry)
    {
        
        if (SYS_L3_IPTYPE_STATIC == pIPv6Entry->list.type)
        {
            num++;
        }
    }

    return num;
}

int32 sal_l3_intfIPv6AutoconfEnbl_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    char intfName[CAPA_L3_INTFNAME_LEN];
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);
    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    if (enable == ipAdmin.autoconfEnbl)
        return SYS_ERR_OK;

    ipAdmin.autoconfEnbl = enable;
    SYS_ERR_CHK(cfg_l3_intfIPv6_set(pId, &ipAdmin));

    if (IS_L3_INTF_LINKUP(pId) && IS_L3_INTF_IPV6ENBL(pId))
    {
        SYS_MEM_CLEAR(intfName);
        INTFID2STR(pId, intfName);

        SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/%s/forwarding",
                (ENABLED == enable || ENABLED == ipAdmin.dhcp6c.enbl) ? 0 : 1,
                intfName);

        SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/%s/autoconf",
                   (ENABLED == enable) ? 1 : 0,
                   intfName);
    }

    if (DISABLED == enable)
    {
        sys_l3_ipv6Db_t *pCurr = NULL;
        sys_l3_ipv6_t delIPv6[CAPA_L3_INTF_STATIC_IPV6_NUM];
        uint32 i = 0;
        uint32 cnt = 0;

        SYS_MEM_CLEAR(delIPv6);

        FOR_EACH_L3_IPV6_IN_INTF(pId, pCurr)
        {
            if (SYS_L3_IPTYPE_V6AUTOCONF == pCurr->list.type)
            {
                osal_memcpy(&delIPv6[cnt], &pCurr->ip, sizeof(sys_l3_ipv6_t));
                cnt++;
            }
        }

        for (i = 0; i < cnt; i++)
        {
            sal_l3_intfIPv6_del(pId, SYS_L3_IPTYPE_V6AUTOCONF, &delIPv6[i]);
        }

        if (0 == sal_l3_intfStaticIPv6AddrNum_ret(pId))
        {
            sys_enable_t mode = DISABLED;

            sal_l3_adminIntfIPv6Mode_get(pId, &mode);

            if (DISABLED == mode && DISABLED == enable)
                sal_l3_adminIntfIPv6Enbl_set(pId, DISABLED);
        }
    }
    else
    {
        sal_l3_adminIntfIPv6Enbl_set(pId, ENABLED);
    }

    sal_l3_intfIPv6_refresh(pId);

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6AutoconfEnbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    *pEnable = ipAdmin.autoconfEnbl;

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DUnreachableEnable_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);
    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    if (enable == ipAdmin.unreachMsgSend)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_l3_adminIntfIPv6DestUnreach_set(pId, enable));

    ipAdmin.unreachMsgSend = enable;
    SYS_ERR_CHK(cfg_l3_intfIPv6_set(pId, &ipAdmin));

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DUnreachableEnable_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    *pEnable = ipAdmin.unreachMsgSend;

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DadAttempt_set(sys_l3_intfId_t *pId, uint32 num)
{
    char intfName[CAPA_L3_INTFNAME_LEN];
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);
    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    ipAdmin.dadTxNum = num;
    SYS_ERR_CHK(cfg_l3_intfIPv6_set(pId, &ipAdmin));

    if (IS_L3_INTF_LINKUP(pId) && IS_L3_INTF_IPV6ENBL(pId))
    {
        SYS_MEM_CLEAR(intfName);
        INTFID2STR(pId, intfName);

        SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/%s/dad_transmits", num, intfName);
    }

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DadAttempt_get(sys_l3_intfId_t *pId, uint32 *pNum)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pNum), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    *pNum = ipAdmin.dadTxNum;

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DadStatus_get(sys_l3_intfId_t *pId, sys_l3_ipv6_t *pIp, sys_l3_ipv6DadSts_t *pStatus)
{
    FILE *fp = NULL;
    char devStr[CAPA_L3_INTFNAME_LEN];
    char ipStr[CAPA_IPV6_STR_LEN];
    char ipPlenStr[CAPA_IPV6_STR_LEN+5];
    char buf[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);
    *pStatus = SYS_L3_IPV6_DADSTS_NONE;
    SYS_PARAM_CHK((NULL == pId || NULL == pIp), SYS_ERR_NULL_POINTER);

    if (!IS_L3_INTF_LINKUP(pId))
    {
        *pStatus = SYS_L3_IPV6_DADSTS_TENTATIVE;

        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(devStr);
    SYS_MEM_CLEAR(buf);

    INTFID2STR(pId, devStr);
    osal_sprintf(buf, "ip -6 addr show dev %s", devStr);

    if (NULL == (fp = popen(buf, "r")))
        return SYS_ERR_FILE_OPEN;

    SYS_MEM_CLEAR(ipStr);
    SYS_MEM_CLEAR(ipPlenStr);
    IPV62STR(pIp->addr, ipStr);
    osal_sprintf(ipPlenStr, "%s/%u", ipStr, pIp->plen);

    while (fgets(buf, sizeof(buf), fp))
    {
        if (NULL == osal_strstr(buf, ipPlenStr))
            continue;

        if (NULL != osal_strstr(buf, "dadfailed"))
        {
            *pStatus = SYS_L3_IPV6_DADSTS_DUPLICATE;
            break;
        }

        if (NULL != osal_strstr(buf, "tentative"))
        {
            *pStatus = SYS_L3_IPV6_DADSTS_TENTATIVE;
            break;
        }

        *pStatus = SYS_L3_IPV6_DADSTS_ACTIVE;

        break;
    }

    pclose(fp);

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6Maddr_get(sys_l3_intfId_t *pId, sys_l3_ipv6MaddrInfo_t *pMaddrs)
{
    FILE *fp = NULL;
    char devStr[CAPA_L3_INTFNAME_LEN];
    char buf[SYS_BUF128_LEN];
    char *tok[5];
    uint32 cnt = 0;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pMaddrs), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(devStr);
    SYS_MEM_CLEAR(buf);

    INTFID2STR(pId, devStr);
    osal_sprintf(buf, "ip -6 maddr show dev %s", devStr);

     if (NULL == (fp = popen(buf, "r")))
        return FALSE;

    while (fgets(buf, sizeof(buf), fp))
    {
        if (NULL != osal_strstr(buf, devStr))
            continue;

        tok[0] = strtok(buf, " \n"); 

        if (NULL == osal_strstr(tok[0], "inet6"))
            continue;

        tok[1] = strtok(NULL, " \n"); 

        STR2IPV6(tok[1], &pMaddrs->addrs[cnt]);
        cnt++;

        SYS_MEM_CLEAR(buf);
    }

    pMaddrs->num = cnt;

    pclose(fp);

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DhcpEnbl_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);
    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    if (enable == ipAdmin.dhcp6c.enbl)
        return SYS_ERR_OK;

    ipAdmin.dhcp6c.enbl= enable;
    SYS_ERR_CHK(cfg_l3_intfIPv6_set(pId, &ipAdmin));

    if (IS_L3_INTF_LINKUP(pId))
        _sal_l3_intfDhcpcV6_set(pId, enable);

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DhcpEnbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    *pEnable = ipAdmin.dhcp6c.enbl;

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS

int32 sal_l3_intfIPv6DhcpStatelessEnbl_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);
    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    if (enable == ipAdmin.dhcp6c.stateless)
        return SYS_ERR_OK;

    ipAdmin.dhcp6c.stateless = enable;
    SYS_ERR_CHK(cfg_l3_intfIPv6_set(pId, &ipAdmin));

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DhcpStatelessEnbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    *pEnable = ipAdmin.dhcp6c.stateless;

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DhcpRefreshTime_set(sys_l3_intfId_t *pId, uint32 time)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);
    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    ipAdmin.dhcp6c.refreshtime = time;
    SYS_ERR_CHK(cfg_l3_intfIPv6_set(pId, &ipAdmin));

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DhcpRefreshTime_get(sys_l3_intfId_t *pId, uint32 *pTime)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    *pTime = ipAdmin.dhcp6c.refreshtime;

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DhcpMinRefreshTime_set(sys_l3_intfId_t *pId, uint32 time)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);
    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    ipAdmin.dhcp6c.min_refreshtime = time;
    SYS_ERR_CHK(cfg_l3_intfIPv6_set(pId, &ipAdmin));

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DhcpMinRefreshTime_get(sys_l3_intfId_t *pId, uint32 *pTime)
{
    sys_l3_ipv6Admin_t ipAdmin;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ipAdmin);

    SYS_ERR_CHK(cfg_l3_intfIPv6_get(pId, &ipAdmin));

    *pTime = ipAdmin.dhcp6c.min_refreshtime;

    return SYS_ERR_OK;
}

int32 sal_l3_intfIPv6DhcpStateless_refresh(sys_l3_intfId_t *pId)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(_sal_l3_intfIPv6DhcpStateless_refresh(pId, TRUE));

    return SYS_ERR_OK;
}
#endif

int32 sal_l3_intfLinkStatusChg_set(sys_l3_intfId_t *pId, sys_l3_intfStsChg_t *pStsChg)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pStsChg), SYS_ERR_NULL_POINTER);
	char devStr[CAPA_L3_INTFNAME_LEN];

	if(pStsChg->linkup == SYS_L3_INTFLINK_DOWN && pStsChg->stsChg)
	{
		INTFID2STR(pId, devStr);
		SYSTEM_CMD_HID("ip neigh flush dev %s", devStr);
	}

    SYS_ERR_CHK(cfg_l3_intfStatusChg_set(pId, pStsChg));

    return SYS_ERR_OK;
}

int32 sal_l3_intfLinkStatusChg_get(sys_l3_intfId_t *pId, sys_l3_intfStsChg_t *pStsChg)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pStsChg), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_intfStatusChg_get(pId, pStsChg));

    return SYS_ERR_OK;
}

int32 sal_l3_intfLinkStatus_set(sys_l3_intfId_t *pId, sys_l3_intfLinkStatus_t linkStatus)
{
    sys_enable_t dhcpv4Enbl;
    sys_l3_ipv4_t ip;

    sal_l3_adminIntfDhcpV4_get(pId, &dhcpv4Enbl);

    if (SYS_L3_INTFLINK_UP == linkStatus)
    {
        char devStr[CAPA_L3_INTFNAME_LEN];
        char dirpath[SYS_BUF32_LEN];

        SYS_MEM_CLEAR(devStr);
        SYS_MEM_CLEAR(dirpath);

        INTFID2STR(pId, devStr);

        osal_sprintf(dirpath, "/var/run/%s", devStr);

        sal_util_dir_create(dirpath);
    }

    if (ENABLED == dhcpv4Enbl)
    {
        if (SYS_L3_INTFLINK_DOWN == linkStatus)
        {
           
	        SYS_MEM_CLEAR(ip);
	        ip.plen = 32;
	        SYS_ERR_CHK(rsd_l3_intfIp_add(SYS_L3_AF_IPV4, pId, SYS_L3_IPTYPE_DHCP, (sys_l3_ip_t*)&ip));
            _sal_l3_intfDhcpcV4_set(pId, DISABLED);

        }
        else
        {
            _sal_l3_intfDhcpcV4_set(pId, ENABLED);
        }
    }
    else
    {
        _sal_l3_intfDhcpcV4_set(pId, DISABLED);
    }

    _sal_l3_intfIPv4Addr_update(pId);
    _sal_l3_intfIPv6Addr_update(pId);
#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS
    _sal_l3_intfIPv6DhcpStateless_refresh(pId, TRUE);
#endif

    return SYS_ERR_OK;
}

int32 sal_l3_intfvExist_set(sys_l3_intfId_t *pId, uint32 exist)
{
    sys_vid_t mgmtVid = 0;
    sys_enable_t adminExist = DISABLED;

    sal_vlan_mgmtVid_get(&mgmtVid);

    if (pId->id == mgmtVid)
    {
        if (TRUE == exist)
        {
            if (0 != L3_INTF_IPV4_NUM(pId))
            {
                L3_INTF_DBG("L3 VLAN %d interface exist, inactive mgmt vlan IPv4\n", pId->id);
                _sal_l3_intfMgmt_inactive(SYS_L3_AF_IPV4);
            }

            if (IS_L3_INTF_IPV6ENBL(pId))
            {
                _sal_l3_intfMgmt_inactive(SYS_L3_AF_IPV6);
            }
        }
        else
        {
            L3_INTF_DBG("L3 VLAN %d interface not exist, active mgmt vlan\n", pId->id);
            _sal_l3_intfMgmtV4_active();
            _sal_l3_intfMgmtV6_active(NULL);
        }
    }

    sal_l3_adminIntfExist_get(pId, &adminExist);

    if (FALSE == exist && DISABLED == adminExist)
    {
        usleep(200000);
        cfg_l3_intf_init(pId);
    }

    return SYS_ERR_OK;
}

int32 sal_l3_intf_del(sys_l3_intfId_t *pId)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    {
        uint32 i = 0;
        uint32 j = 0;
        sys_l3_staticRouteV6_t v6Rules;
        sys_l3_staticRouteV6_t v6RulesDel;

        SYS_MEM_CLEAR(v6Rules);
        SYS_MEM_CLEAR(v6RulesDel);

        sal_l3_staticRouteV6_get(&v6Rules);

        for (i = 0; i < v6Rules.num; i++)
        {
            if ((SYS_L3_INTFTYPE_VLAN == v6Rules.route[i].ointf.type)
                    && (pId->id == v6Rules.route[i].ointf.id))
            {
                osal_memcpy(&v6RulesDel.route[j], &v6Rules.route[i], sizeof(sys_l3_routeV6_t));
                j++;
            }
        }

        for (i = 0; i < j; i++)
        {
            sal_l3_staticRouteV6_del(&v6RulesDel.route[i]);
        }
    }

    sal_l3_intfStaticIPv6_clear(pId);

    sal_l3_adminIntfIPv6Enbl_set(pId, DISABLED);

    sal_l3_intfStaticIPv4_clear(pId);

    sal_l3_adminIntfExist_set(pId, DISABLED);

    return SYS_ERR_OK;
}

int32 sal_l3_intf_refresh(sys_l3_intfId_t *pId)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    rsd_l3_intfGratuitousArp_send(pId);

    return SYS_ERR_OK;
}
#endif

int32 sal_l3_adminIntfMgmt_set(sys_l3_intfMgmtAdmin_t *pMgmt)
{
    sys_l3_intfMgmtAdmin_t oldMgmt;
    sys_l3_intfId_t intfId;

    SYS_PARAM_CHK((NULL == pMgmt), SYS_ERR_NULL_POINTER);

    if (!L3_DB_MAP())
        return SYS_ERR_FAILED;

    SYS_MEM_CLEAR(oldMgmt);
    SYS_MEM_CLEAR(intfId);

    STR2INTFID(L3_INTF_MGMTVLAN, &intfId);

    L3_USER_LOCK();
    osal_memcpy(&oldMgmt, L3_DB_INTFM_ADMIN_PTR(), sizeof(sys_l3_intfMgmtAdmin_t));
    osal_memcpy(L3_DB_INTFM_ADMIN_PTR(), pMgmt, sizeof(sys_l3_intfMgmtAdmin_t));
    L3_USER_UNLOCK();

    if (0 != osal_memcmp(&oldMgmt.v4, &pMgmt->v4, sizeof(pMgmt->v4)))
        _sal_l3_intfMgmtV4_active();

    if (0 != osal_memcmp(&oldMgmt.subv4, &pMgmt->subv4, sizeof(pMgmt->subv4)))
	{
        _sal_l3_intfMgmtSubV4_active();
	}

    if (0 != osal_memcmp(&oldMgmt.v6, &pMgmt->v6, sizeof(pMgmt->v6)))
        _sal_l3_intfMgmtV6_active(&oldMgmt);

    return SYS_ERR_OK;
}

int32 sal_l3_adminIntfMgmt_get(sys_l3_intfMgmtAdmin_t *pMgmt)
{
    SYS_PARAM_CHK((NULL == pMgmt), SYS_ERR_NULL_POINTER);

    if (!L3_DB_MAP())
        return SYS_ERR_FAILED;

    L3_USER_LOCK();
    osal_memcpy(pMgmt, L3_DB_INTFM_ADMIN_PTR(), sizeof(sys_l3_intfMgmtAdmin_t));
    L3_USER_UNLOCK();

    return SYS_ERR_OK;
}

int32 sal_l3_intfMgmt_get(sys_l3_intfMgmt_t *pMgmt)
{
    SYS_PARAM_CHK((NULL == pMgmt), SYS_ERR_NULL_POINTER);

    if (!L3_DB_MAP())
        return SYS_ERR_FAILED;

    osal_memcpy(pMgmt, g_l3DbIntfMgmt, sizeof(sys_l3_intfMgmt_t));

    return SYS_ERR_OK;
}

int32 sal_l3_intfMgmt_refresh(void)
{
    _sal_l3_intfMgmt_inactive(SYS_L3_AF_END);
    _sal_l3_intfMgmtV4_active();
    _sal_l3_intfMgmtV6_active(NULL);

    return SYS_ERR_OK;
}

int32 sal_def_intfMgmt_get(char *ipaddr, char *netmask, char* gateway)
{

	FILE *confp = NULL;
	char *pTemp = NULL;
	char buf[SYS_BUF256_LEN];

	confp = fopen (SYS_OEM_CONF_FILE, "r");
	if (confp == NULL)
	{
		return SYS_ERR_FAILED;
	}
	
	SYS_MEM_CLEAR(buf);
	while(fgets(buf, sizeof(buf), confp))
	{
		pTemp = osal_strchr(buf, '\r');
		*pTemp = '\0';
		if(osal_strstr(buf, "ipaddr:") && (ipaddr != NULL))
		{
			osal_sprintf(ipaddr, "%s", buf + osal_strlen("ipaddr:"));
			SYS_MEM_CLEAR(buf);
			continue;
		}
		if(osal_strstr(buf, "ipmask:") && (netmask != NULL))
		{
			osal_sprintf(netmask, "%s", buf + osal_strlen("ipmask:"));
			SYS_MEM_CLEAR(buf);
			continue;
		}
		if(osal_strstr(buf, "gateway:") && (gateway != NULL))
		{
			osal_sprintf(gateway, "%s", buf + osal_strlen("gateway:"));
			SYS_MEM_CLEAR(buf);
			continue;
		}
		SYS_MEM_CLEAR(buf);
	}
	
	fclose (confp);

    return SYS_ERR_OK;
}

int32 sal_l3_define_interface_set(sys_l3_intfdef_t *pDefConfig)
{
    SYS_PARAM_CHK((NULL == pDefConfig), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_l3_intfIPv4_add(&pDefConfig->defId, SYS_L3_IPTYPE_STATIC, &pDefConfig->v4.ip));
	if(pDefConfig->v6.enbl == ENABLED)
	{
    	SYS_ERR_CHK(sal_l3_adminIntfIPv6Enbl_set(&pDefConfig->defId, pDefConfig->v6.enbl));
    }

    return SYS_ERR_OK;
}

