/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 16227 $
 * $Date: 2011-03-10 18:23:52 +0800 (Thu, 10 Mar 2011) $
 *
 * Purpose : Process special event which can't be processed in normal thread
 *
 * Feature : Process special event which can't be processed in normal thread
 */

/*
 * Include Files
 */
#include <common/sys_util.h>
#include <libcmd/cmd.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */
#ifdef CONFIG_SYS_L3_ROUTE
void handleIpChgLineSessionClear(void)
{
    uint32 i;
    uint32 j;
    uint32 found = FALSE;
    uint32 doClear = FALSE;
    sys_enable_t enable;
    sys_line_session_t session;
    sys_ipv4_t localIpv4;
    sys_ipv6_t localIpv6;
    sys_l3_ipv4Db_t *pCurrV4 = NULL;
    sys_l3_ipv6Db_t *pCurrV6 = NULL;    

    sal_sys_telnetdEnable_get(&enable);

    if (ENABLED == enable)
    {
        for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_CLI_TELNET); i++)
        {
            SYS_MEM_CLEAR(session);
            sal_sys_lineSession_get(SYS_ACCESS_CLI_TELNET, i, &session);

            if (0 == session.pid)
                continue;
			
            if (NULL == strchr(session.localIp, ':'))
            {
                STR2IPV4(session.localIp, &localIpv4);

                found = FALSE;
                FOR_EACH_L3_IPV4(j, pCurrV4)
                {
                    if (IS_IPV4_EQUAL(pCurrV4->ip.addr, localIpv4))
                    {
                        found = TRUE;
                        break;
                    }
                }
            }
            else
            {
                STR2IPV6(session.localIp, &localIpv6);

                if (IS_IPV6_LINKLOCAL(localIpv6))
                    continue;
                    
                found = FALSE;
                FOR_EACH_L3_IPV6(j, pCurrV6)
                {
                    if (IS_IPV6_EQUAL(pCurrV6->ip.addr, localIpv6))
                    {
                        found = TRUE;
                        break;
                    }
                }
            }

            if (FALSE == found)
            {
                kill(session.pid, SIGKILL);
                doClear = TRUE;
            }
        }
    }

#ifdef CONFIG_USER_SSH_SSHD
    sal_ssh_sshdEnable_get(&enable);

    if (ENABLED == enable)
    {
        for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_CLI_SSH); i++)
        {
            SYS_MEM_CLEAR(session);
            sal_sys_lineSession_get(SYS_ACCESS_CLI_SSH, i, &session);

            if (0 == session.pid)
                continue;
                
			found = FALSE;

            if (NULL == strchr(session.localIp, ':'))
            {

                STR2IPV4(session.localIp, &localIpv4);

                FOR_EACH_L3_IPV4(j, pCurrV4)
                {
                    if (IS_IPV4_EQUAL(pCurrV4->ip.addr, localIpv4))
                    {
                        found = TRUE;
                        break;
                    }
                }
        
            }
            else
            {
                STR2IPV6(session.localIp, &localIpv6);

                if (IS_IPV6_LINKLOCAL(localIpv6))
                    continue;
                    
                FOR_EACH_L3_IPV6(j, pCurrV6)
                {
                    if (IS_IPV6_EQUAL(pCurrV6->ip.addr, localIpv6))
                    {
                        found = TRUE;
                        break;
                    }
                }
            }

            if (FALSE == found)
            {
                kill(session.pid, SIGKILL);
                doClear = TRUE;
            }
        }
    }
#endif

    if (doClear)
        sal_sys_lineSessionInfo_clear();
}

void handleDhcpDfltRoute(void)
{
    FILE *fp = NULL;
    FILE *fp_tmp = NULL;
    char buf[SYS_BUF512_LEN];
    char buf_tmp[SYS_BUF512_LEN];
    char *tok[10];
    uint32 isDfltRouteExist = FALSE;
    uint32 isOldDfltFromDhcpOrMgmt = FALSE;
    uint32 isSubnetExist = FALSE;
    sys_enable_t dhcpEnbl = DISABLED;
    sys_l3_staticRouteV4_t sroutes;
    sys_l3_routeV4_t tmpDfltRoute;
    sys_l3_routeV4_t oldDfltRoute;
    sys_l3_routeV4_t newDfltRoute;
    sys_l3_ipv4Db_t *pIPv4Entry = NULL;
    sys_l3_intfMgmtAdmin_t currMgmtAdmin;

    SYS_MEM_CLEAR(tok);
    SYS_MEM_CLEAR(oldDfltRoute);
    SYS_MEM_CLEAR(newDfltRoute);
    SYS_MEM_CLEAR(currMgmtAdmin);

    SYS_MEM_CLEAR(sroutes);
    sal_l3_staticRouteV4_get(&sroutes);
    sal_l3_adminIntfMgmt_get(&currMgmtAdmin);

    L3_INTF_DBG("=== Refresh default route\n");

    if (NULL == (fp = popen("ip route show", "r")))
        return;

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    pclose(fp);

    if (osal_strlen(buf) > 0)
    {
        buf[strlen(buf) - 1] = '\0';

        /* default via 22.22.22.1 dev intfv3  proto zebra */
        tok[0] = strtok(buf, " "); // default

        if (0 == osal_strcmp(tok[0], "default"))
        {
            tok[1] = strtok(NULL, " "); // via
            tok[2] = strtok(NULL, " "); // gateway
            tok[3] = strtok(NULL, " "); // dev
            tok[4] = strtok(NULL, " "); // intfx

            STR2INTFID(tok[4], &oldDfltRoute.ointf);
            STR2IPV4(tok[2], &oldDfltRoute.gw);

            tok[5] = strtok(NULL, " "); // proto

            if (NULL == tok[5])
            {
                isOldDfltFromDhcpOrMgmt = TRUE;
            }

            isDfltRouteExist = TRUE;

            L3_INTF_DBG("Default route exist with gateway %s on %s from %s\n",
                          tok[2],
                          tok[4],
                          isOldDfltFromDhcpOrMgmt ? "dhcp" : tok[5]);
        }
        else
        {
            SYS_MEM_CLEAR(buf);
        }
    }

    /* Static default route does exist */
    if (0 != sroutes.dfltRouteNum)
    {
        L3_INTF_DBG("Static default route exist (%u)\n", sroutes.dfltRouteNum);

        /* Old default route is from DHCP,
           so clear it and zebra will recovery default route automatically */
        if (isOldDfltFromDhcpOrMgmt)
        {
            L3_INTF_DBG("Delete old default route from dhcp or mgmt\n");

            SYSTEM_CMD_HID("ip route del default");
        }

        return;
    }
    else if (!isDfltRouteExist
                && IS_MGMT_VLAN_IPV4_ACTIVE()
                && SYS_L3_DHCPTYPE_DISABLE == currMgmtAdmin.v4.dhcpType
                && !IS_IPV4_ALLZERO(currMgmtAdmin.v4.gw)
                && IS_IPV4_IN_SUBNET(&currMgmtAdmin.v4.gw, &currMgmtAdmin.v4.ip))
    {

        char ipStr[CAPA_IPV4_STR_LEN];

        SYS_MEM_CLEAR(ipStr);

        IPV42STR(currMgmtAdmin.v4.gw, ipStr);
        SYSTEM_CMD_HID("ip route add default via %s", ipStr);

        L3_INTF_DBG("Add default route from MGMT VLAN config gateway %s\n", ipStr);

        return;
    }
    else if (isDfltRouteExist
                && isOldDfltFromDhcpOrMgmt
                && IS_MGMT_VLAN_IPV4_ACTIVE()
                && IS_IPV4_ALLZERO(currMgmtAdmin.v4.gw))
    {
        SYSTEM_CMD_HID("ip route del default");

        L3_INTF_DBG("Delete default route from MGMT VLAN\n");
    }
    /* Static default route does not exist, use dhcp default route if exist */
    {
        if (NULL == (fp = fopen("/etc/dhcpc/dfltGw", "r")))
            return;

        if (NULL == (fp_tmp = fopen("/etc/dhcpc/dfltGw_tmp", "w")))
        {
            fclose(fp);
            return;
        }

        SYS_MEM_CLEAR(buf);
        SYS_MEM_CLEAR(buf_tmp);

        /* If multiple default gateway from different interface's dhcp client, seletct best one. */
        while (fgets(buf, sizeof(buf), fp))
        {
            SYS_MEM_CLEAR(buf_tmp);
            SYS_MEM_CLEAR(tok);
            SYS_MEM_CLEAR(tmpDfltRoute);

            osal_memcpy(buf_tmp, buf, sizeof(buf_tmp));

            buf[strlen(buf) - 1] = '\0';

            L3_INTF_DBG("Default route data %s\n", buf);

            tok[0] = strtok(buf, " "); // intfx
            tok[1] = strtok(NULL, " "); // gateway
            if (0 == osal_strcmp(tok[0], L3_INTF_MGMTVLAN))
            {
                SYSTEM_CMD_HID("ip route del default");
                SYSTEM_CMD_HID("ip route add default via %s", tok[1]);

                L3_INTF_DBG("Add default route from MGMT VLAN dhcp gateway %s\n", tok[1]);

                fclose(fp);
                fclose(fp_tmp);
                return;
            }
            else
            {
                STR2INTFID(tok[0], &tmpDfltRoute.ointf);
                STR2IPV4(tok[1], &tmpDfltRoute.gw);

                if (!IS_L3_INTF_EXIST(&tmpDfltRoute.ointf))
                    continue;

                if (!IS_L3_INTF_LINKUP(&tmpDfltRoute.ointf))
                    continue;

                sal_l3_adminIntfDhcpV4_get(&tmpDfltRoute.ointf, &dhcpEnbl);

                if (DISABLED == dhcpEnbl)
                    continue;

                FOR_EACH_L3_IPV4_IN_INTF(&tmpDfltRoute.ointf, pIPv4Entry)
                {
                    if (IS_IPV4_IN_SUBNET(&tmpDfltRoute.gw, &pIPv4Entry->ip))
                    {
                        isSubnetExist = TRUE;
                        break;
                    }
                }

                if (!isSubnetExist)
                    continue;

                fprintf(fp_tmp, "%s", buf_tmp);

                L3_INTF_DBG("Original select default route "FMT_IPV4", tmp default route "FMT_IPV4"\n",
                                SHOW_IPV4(newDfltRoute.gw),
                                SHOW_IPV4(tmpDfltRoute.gw));

                if (IS_IPV4_ALLZERO(newDfltRoute.gw) || (tmpDfltRoute.gw < newDfltRoute.gw))
                {
                    newDfltRoute.gw = tmpDfltRoute.gw;

                    L3_INTF_DBG("New select default route "FMT_IPV4"\n",
                                SHOW_IPV4(newDfltRoute.gw));

                    osal_memcpy(&newDfltRoute.ointf, &tmpDfltRoute.ointf, sizeof(sys_l3_intfId_t));
                }

                SYS_MEM_CLEAR(buf);
            }
        }

        fclose(fp);
        fclose(fp_tmp);

        SYSTEM_CMD("mv /etc/dhcpc/dfltGw_tmp /etc/dhcpc/dfltGw");

        if (0 != osal_memcmp(&newDfltRoute, &oldDfltRoute, sizeof(sys_l3_routeV4_t)))
        {
            char ipStr[CAPA_IPV4_STR_LEN];
            char intfStr[CAPA_L3_INTFNAME_LEN];

            if (IS_IPV4_ALLZERO(newDfltRoute.gw))
                return;

            if (isDfltRouteExist)
                SYSTEM_CMD_HID("ip route del default");

            SYS_MEM_CLEAR(ipStr);
            SYS_MEM_CLEAR(intfStr);

            INTFID2STR(&newDfltRoute.ointf, intfStr);
            IPV42STR(newDfltRoute.gw, ipStr);

            SYSTEM_CMD_HID("ip route add default via %s dev %s", ipStr, intfStr);

            L3_INTF_DBG("Add default route via %s dev %s\n", ipStr, intfStr);
        }
    }

    return;
}

void handleIPv6DupLog(char *intfStr, char *ipStr, char *plenStr)
{
    sys_l3_intfId_t intfId;
    sys_l3_ipv6_t ip;
    uint32 i = 0;
    uint32 dadAttempt = 0;
    sys_l3_ipv6DadSts_t dadSts = 0;

    SYS_MEM_CLEAR(intfId);
    SYS_MEM_CLEAR(ip);

    STR2INTFID(intfStr, &intfId);
    STR2IPV6(ipStr, &ip.addr);
    ip.plen = SYS_STR2UINT(plenStr);

    sal_l3_intfIPv6DadAttempt_get(&intfId, &dadAttempt);

    if (0 == dadAttempt)
        return;

    /* Detect until the dad attempt finish. DAD detect send packet every 1 sec */
    for (i = 0; i < dadAttempt; i++)
    {
        sleep(1);

        sal_l3_intfIPv6DadStatus_get(&intfId, &ip, &dadSts);

        /* If IP does not exist, do nothing */
        if (SYS_L3_IPV6_DADSTS_NONE == dadSts)
            return;

        if (SYS_L3_IPV6_DADSTS_DUPLICATE == dadSts)
        {
            char devName[CAPA_L3_INTFNAME_LEN];

            INTFID2NAME(&intfId, devName);
            SYS_LOG(LOG_CAT_L3, LOG_L3_IPV6_INTFIP_DUPLICATE, ipStr, devName);

            return;
        }
    }

    return;
}

void handleDHCPv6AddrUpdate(int32 action, char *intfStr, char *ipStr)
{
    sys_l3_ipv6_t ipAddr;
    sys_l3_intfId_t intfId;

    SYS_MEM_CLEAR(ipAddr);
    SYS_MEM_CLEAR(intfId);

    STR2IPV6(ipStr, &ipAddr.addr);
    ipAddr.plen = 128;
    STR2INTFID(intfStr, &intfId);

    if (0 == action) /* Add */
    {
        sal_l3_intfIPv6_add(&intfId, SYS_L3_IPTYPE_DHCP, &ipAddr);
    }
    else /* Del */
    {
        sal_l3_intfIPv6_del(&intfId, SYS_L3_IPTYPE_DHCP, &ipAddr);
    }

    return;
}

void handleInitL3(void)
{
    uint32 i = 0;
    sys_l3_ipv4Db_t *pCurr = NULL;

    usleep(10000000); /* 10S */

    FOR_EACH_L3_IPV4(i, pCurr)
    {
        if (IS_L3_INTF_LINKUP(&pCurr->intfId))
        {
            sal_l3_intf_refresh(&pCurr->intfId);
        }
    }
}

void handleL3IntfStsChg(char *intfStr, char *statusStr)
{
    sys_l3_intfId_t intfId;
    sys_l3_intfStsChg_t stsChg;
    sys_l3_intfStsChg_t stsChgNew;

    SYS_MEM_CLEAR(intfId);
    STR2INTFID(intfStr, &intfId);

    SYS_MEM_CLEAR(stsChg);
    sal_l3_intfLinkStatusChg_get(&intfId, &stsChg);
    stsChg.process = TRUE;
    sal_l3_intfLinkStatusChg_set(&intfId, &stsChg);

    //SYS_PRINTF("%s:%d intfStr=%s ~~~~%s\n", __func__, __LINE__, intfStr, stsChg.linkup?"UP":"DOWN");
    sal_l3_intfLinkStatus_set(&intfId, stsChg.linkup);

    SYS_MEM_CLEAR(stsChgNew);
    sal_l3_intfLinkStatusChg_get(&intfId, &stsChgNew);
    stsChgNew.process = FALSE;
    if (stsChg.linkup == stsChgNew.linkup)
        stsChgNew.stsChg = FALSE;
    else
        stsChgNew.stsChg = TRUE;
    sal_l3_intfLinkStatusChg_set(&intfId, &stsChgNew);
}
#else
void handleDhcpDfltRoute(void)
{
    FILE *fp = NULL;
    char buf[SYS_BUF512_LEN];
    char *tok[10];

    SYS_MEM_CLEAR(tok);
    /* Static default route does not exist, use dhcp default route if exist */
    {
        if (NULL == (fp = fopen("/etc/dhcpc/dfltGw", "r")))
            return;

        SYS_MEM_CLEAR(buf);

        /* If multiple default gateway from different interface's dhcp client, seletct best one. */
        while (fgets(buf, sizeof(buf), fp))
        {
            SYS_MEM_CLEAR(tok);

            buf[strlen(buf) - 1] = '\0';

            L3_INTF_DBG("Default route data %s\n", buf);

            tok[0] = strtok(buf, " "); // intfx
            tok[1] = strtok(NULL, " "); // gateway
            if (0 == osal_strcmp(tok[0], L3_INTF_MGMTVLAN))
            {
                SYSTEM_CMD_HID("ip route del default");
                SYSTEM_CMD_HID("ip route add default via %s", tok[1]);

                L3_INTF_DBG("Add default route from MGMT VLAN dhcp gateway %s\n", tok[1]);

                fclose(fp);
                return;
            }

        }

        fclose(fp);
    }

    return;
}

#endif

