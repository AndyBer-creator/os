
#include <common/sys_log.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>
#include <libsal/sal_l3_route_zebra.h>

#ifdef CONFIG_SYS_L3_ROUTE
static void _v4Route2EngCmd(uint32 isAdd, sys_l3_routeV4_t *pRoute, char *cmd)
{
    char dst[CAPA_IPV4_STR_LEN];
    char gw[CAPA_IPV4_STR_LEN];

    SYS_MEM_CLEAR(dst);
    SYS_MEM_CLEAR(gw);

    sys_util_ip2Str(pRoute->dst, dst);
    sys_util_ip2Str(pRoute->gw, gw);

    if (SYS_L3_INTFTYPE_NULL == pRoute->ointf.type)
        osal_sprintf(cmd, "%sip route %s/%d lo%s", (isAdd ? "" : "no "), dst, pRoute->dstLen, (isAdd ? " reject" : ""));
    else
        osal_sprintf(cmd, "%sip route %s/%d %s", (isAdd ? "" : "no "), dst, pRoute->dstLen, gw);

    return;
}

static void _v6Route2EngCmd(uint32 isAdd, sys_l3_routeV6_t *pRoute, char *cmd)
{
    char dst[CAPA_IPV6_STR_LEN];
    char gw[CAPA_IPV6_STR_LEN];

    SYS_MEM_CLEAR(dst);
    SYS_MEM_CLEAR(gw);

    IPV62STR(pRoute->dst, dst);
    IPV62STR(pRoute->gw, gw);

    if (SYS_L3_INTFTYPE_NULL == pRoute->ointf.type)
    {
        osal_sprintf(cmd, "%sipv6 route %s/%d lo%s", (isAdd ? "" : "no "), dst, pRoute->dstLen, (isAdd ? " reject" : ""));
    }
    else if (SYS_L3_INTFTYPE_VLAN == pRoute->ointf.type)
    {
        char intfStr[CAPA_L3_INTFNAME_LEN];

        SYS_MEM_CLEAR(intfStr);

        INTFID2STR(&pRoute->ointf, intfStr);

        osal_sprintf(cmd, "%sipv6 route %s/%d %s %s", (isAdd ? "" : "no "), dst, pRoute->dstLen, gw, intfStr);
    }
    else
    {
        osal_sprintf(cmd, "%sipv6 route %s/%d %s", (isAdd ? "" : "no "), dst, pRoute->dstLen, gw);
    }

    return;
}
#if 0
static void _v4Zroute2Info(char *rtStr, sys_l3_routeV4Info_t *pInfo, sys_l3_routeV4Info_t *pPrev)
{
    uint32 i = 0;
    char *buf[10];

    switch (rtStr[0])
    {
        case 'K':
            pInfo->type = SYS_L3_ROUTETYPE_KERNEL;
            break;

        case 'C':
            pInfo->type = SYS_L3_ROUTETYPE_CONNECT;
            break;

        case 'S':
            pInfo->type = SYS_L3_ROUTETYPE_STATIC;
            break;

        case ' ':
            if (NULL != pPrev)
                pInfo->type = pPrev->type;
            break;

        default:
            pInfo->type = SYS_L3_ROUTETYPE_UNKNOWN;
            break;
    }

    switch (rtStr[1])
    {
        case '>':
            pInfo->select = TRUE;
            break;

        case ' ':
        default:
            pInfo->select = FALSE;
            break;
    }

    switch (rtStr[2])
    {
        case '*':
            pInfo->fib = TRUE;
            break;

        case ' ':
        default:
            pInfo->fib = FALSE;
            break;
    }

    SYS_MEM_CLEAR(buf);

    buf[i] = strtok(rtStr + 3, " ,/[]\n");

    while (buf[i] != NULL)
    {
        i++;
        buf[i] = strtok(NULL, " ,/[]\n");
    }

#if 0
    {
        uint32 j = 0;
        for (j = 0; j < i; j++)
        {
            SYS_PRINTF("%d %s\n", j, buf[j]);
        }
    }
#endif

    if (pInfo->select && pInfo->fib)
        pInfo->active = TRUE;

    if ('v' == buf[0][0] && NULL != pPrev)
    {
        osal_memcpy(&pInfo->route, &pPrev->route, sizeof(sys_l3_routeV4_t));
        pInfo->distance = pPrev->distance;

        STR2IPV4(buf[1], &pInfo->route.gw);
        STR2INTFID(buf[2], &pInfo->route.ointf);

        if (!pPrev->active && pInfo->fib)
            pInfo->active = TRUE;
    }
    else if (i == 6 && 'i' == buf[2][0] && 'd' == buf[3][0])
    {
        pInfo->type = SYS_L3_ROUTETYPE_CONNECT;

        STR2IPV4(buf[0], &pInfo->route.dst);
        pInfo->route.dstLen = SYS_STR2UINT(buf[1]);
        STR2INTFID(buf[5], &pInfo->route.ointf);
    }
    
    else if (i == 9 && 'i' == buf[4][0] && 'd' == buf[5][0] && 'r' == buf[8][0])
    {
        STR2IPV4(buf[0], &pInfo->route.dst);
        pInfo->route.dstLen = SYS_STR2UINT(buf[1]);

        pInfo->route.ointf.type = SYS_L3_INTFTYPE_NULL;

        pInfo->route.pri = 255;
    }
    else
    {
        STR2IPV4(buf[0], &pInfo->route.dst);
        pInfo->route.dstLen = SYS_STR2UINT(buf[1]);

        if (pInfo->type == SYS_L3_ROUTETYPE_KERNEL)
        {
            pInfo->distance = 1;
            STR2IPV4(buf[3], &pInfo->route.gw);
            STR2INTFID(buf[4], &pInfo->route.ointf);
        }
        
        else
        {
            pInfo->distance = SYS_STR2UINT(buf[2]);
            STR2IPV4(buf[5], &pInfo->route.gw);
            STR2INTFID(buf[6], &pInfo->route.ointf);
        }
    }

    pInfo->metric = DFLT_L3_STATICROUTE_METRIC;
}

static void _v6Zroute2Info(char *rtStr, sys_l3_routeV6Info_t *pInfo, sys_l3_routeV6Info_t *pPrev)
{
    uint32 i = 0;
    char *buf[10];

    switch (rtStr[0])
    {
        case 'K':
            pInfo->type = SYS_L3_ROUTETYPE_KERNEL;
            break;

        case 'C':
            pInfo->type = SYS_L3_ROUTETYPE_CONNECT;
            break;

        case 'S':
            pInfo->type = SYS_L3_ROUTETYPE_STATIC;
            break;

        case ' ':
            if (pPrev != NULL)
                pInfo->type = pPrev->type;
            break;

        default:
            pInfo->type = SYS_L3_ROUTETYPE_UNKNOWN;
            break;
    }

    switch (rtStr[1])
    {
        case '>':
            pInfo->select = TRUE;
            break;

        case ' ':
        default:
            pInfo->select = FALSE;
            break;
    }

    switch (rtStr[2])
    {
        case '*':
            pInfo->fib = TRUE;
            break;

        case ' ':
        default:
            pInfo->fib = FALSE;
            break;
    }

    SYS_MEM_CLEAR(buf);

    buf[i] = strtok(rtStr + 3, " ,/[]\n");

    while (buf[i] != NULL)
    {
        i++;
        buf[i] = strtok(NULL, " ,/[]\n");
    }

#if 0
    {
        uint32 j = 0;
        for (j = 0; j < i; j++)
        {
            SYS_PRINTF("%d %s\n", j, buf[j]);
        }
    }
#endif

    if (pInfo->select && pInfo->fib)
        pInfo->active = TRUE;

    if ('v' == buf[0][0])
    {
        osal_memcpy(&pInfo->route, &pPrev->route, sizeof(sys_l3_routeV6_t));
        pInfo->distance = pPrev->distance;

        STR2IPV6(buf[1], &pInfo->route.gw);
        STR2INTFID(buf[2], &pInfo->route.ointf);

        if (!pPrev->active && pInfo->fib)
            pInfo->active = TRUE;
    }
    
    else if (i == 6 && 'i' == buf[2][0] && 'd' == buf[3][0])
    {
        STR2IPV6(buf[0], &pInfo->route.dst);
        pInfo->route.dstLen = SYS_STR2UINT(buf[1]);
        STR2INTFID(buf[5], &pInfo->route.ointf);
    }
    
    else if (i > 7 && 'i' == buf[4][0] && 'd' == buf[5][0] && 'l' == buf[7][0] && 'r' == buf[8][0])
    {
        STR2IPV6(buf[0], &pInfo->route.dst);
        pInfo->route.dstLen = SYS_STR2UINT(buf[1]);

        pInfo->route.ointf.type = SYS_L3_INTFTYPE_NULL;
    }
    
    else if (i > 7 && 'i' == buf[4][0] && 'd' == buf[5][0])
    {
        if (0 == osal_strncmp(buf[7], "unknow", 6) || 0 == osal_strncmp(buf[7], "reject", 6))
        {
            STR2IPV6(buf[0], &pInfo->route.dst);
            pInfo->route.dstLen = SYS_STR2UINT(buf[1]);

            pInfo->route.ointf.type = SYS_L3_INTFTYPE_NULL;
        }
    }
    else
    {
        STR2IPV6(buf[0], &pInfo->route.dst);
        pInfo->route.dstLen = SYS_STR2UINT(buf[1]);

        if (pInfo->type == SYS_L3_ROUTETYPE_KERNEL)
        {
            pInfo->distance = 1;
            STR2IPV6(buf[3], &pInfo->route.gw);
            STR2INTFID(buf[4], &pInfo->route.ointf);
        }
        
        else
        {
            pInfo->distance = SYS_STR2UINT(buf[2]);
            STR2IPV6(buf[5], &pInfo->route.gw);
            STR2INTFID(buf[6], &pInfo->route.ointf);
        }
    }

    pInfo->metric = DFLT_L3_STATICROUTE_METRIC;
}
#endif

static int32 _sal_l3_routeV4Valid_chk(sys_l3_routeV4_t *pRoute)
{
    uint32 i = 0;
    uint32 isDhcpEnbl = FALSE;
    sys_l3_ipv4Db_t *pCurr = NULL;

    FOR_EACH_L3_IPV4(i, pCurr)
    {
        if (IS_IPV4_ALLZERO(pCurr->ip.addr))
            continue;

        if (IS_IPV4_EQUAL(pCurr->ip.addr, pRoute->gw))
            return SYS_ERR_L3_ROUTE_GW_INTF_IP;

        if (IS_IPV4_IN_SUBNET(&pRoute->gw, &pCurr->ip))
        {
            if (SYS_L3_INTFTYPE_LO == pCurr->intfId.type)
                return SYS_ERR_L3_ROUTE_ON_LO_INTF;

            if (SYS_L3_IPTYPE_DHCP == pCurr->list.type)
                isDhcpEnbl = TRUE;
        }
    }
#ifndef CONFIG_SYS_L3_ARCHITECTURE

    if (IS_MGMT_VLAN_IPV4_ACTIVE())
    {
        sys_l3_intfMgmtAdmin_t mgmtAdmin;

        SYS_MEM_CLEAR(mgmtAdmin);
        SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&mgmtAdmin));

        if (IS_IPV4_IN_SUBNET(&pRoute->gw, &mgmtAdmin.v4.ip))
            return SYS_ERR_L3_ROUTE_GW_MGMT_SUBNET;
    }
#endif
    if (isDhcpEnbl)
        return SYS_ERR_L3_ROUTE_GW_DHCP_ENBL;

    return SYS_ERR_OK;
}

static int32 _sal_l3_routeV6Valid_chk(sys_l3_routeV6_t *pRoute)
{
    uint32 i = 0;

    sys_l3_ipv6Db_t *pCurr = NULL;

    if (pRoute->dstLen <= 3 && !IS_IPV6_ALLZERO(pRoute->dst))
        return SYS_ERR_L3_IPV6_PREFIX_INVALID;

    if (SYS_L3_INTFTYPE_VLAN == pRoute->ointf.type)
    {
        if (!IS_IPV6_LINKLOCAL(pRoute->gw))
            return SYS_ERR_L3_ROUTE_GW_ADDR_INVALID;
    }
    
    else if (SYS_L3_INTFTYPE_INACTIVE == pRoute->ointf.type)
    {
        if (IS_IPV6_LINKLOCAL(pRoute->gw))
            return SYS_ERR_L3_ROUTE_GW_ADDR_INVALID;
    }

    FOR_EACH_L3_IPV6(i, pCurr)
    {
        if (IS_IPV6_ALLZERO(pCurr->ip.addr))
            continue;

        if (IS_IPV6_EQUAL(pCurr->ip.addr, pRoute->gw))
            return SYS_ERR_L3_ROUTE_GW_INTF_IP;

        if (IS_IPV6_IN_SUBNET(&pRoute->gw, &pCurr->ip))
        {
            if (SYS_L3_INTFTYPE_LO == pCurr->intfId.type)
                return SYS_ERR_L3_ROUTE_ON_LO_INTF;

        }
    }

    return SYS_ERR_OK;
}

int32 sal_l3_routeV4_add(sys_l3_routeV4_t *pRoute)
{
    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l3_routeV4_add(pRoute));

    return SYS_ERR_OK;
}

int32 sal_l3_routeV4_del(sys_l3_routeV4_t *pRoute)
{
    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l3_routeV4_del(pRoute));

    return SYS_ERR_OK;
}

int32 sal_l3_routeV6_add(sys_l3_routeV6_t *pRoute)
{
    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l3_routeV6_add(pRoute));

    return SYS_ERR_OK;
}

int32 sal_l3_routeV6_del(sys_l3_routeV6_t *pRoute)
{
    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l3_routeV6_del(pRoute));

    return SYS_ERR_OK;
}

#define _ZEBAR_ROUTE_TITLE_LINE_NUM 5

int32 sal_l3_routeV4Info_get(sys_l3_routeV4InfoDb_t *pInfo)
{

    int32 ret = -1;

    uint32 routeNum = 0;
    uint32 i = 0;
    uint32 j = 0;

    sys_l3_staticRouteV4_t staticRoutes;
	sys_l3_routeV4Info_t routeTmp;

    SYS_ERR_CHK(cfg_l3_staticRouteV4_get(&staticRoutes));

    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);
#if 0
    fp = tmpfile();

    ret = sal_l3_engineCmd2File_exec("do show ip route", fp);

    if (SYS_ERR_OK != ret)
    {
        fclose(fp);
        return ret;
    }

    rewind(fp);

    while (!feof(fp))
    {
        if (NULL == fgets(buf, sizeof(buf), fp))
            break;

        line++;

        if (line <= _ZEBAR_ROUTE_TITLE_LINE_NUM)
            continue;

        osal_memset(&pInfo->info[routeNum], 0, sizeof(sys_l3_routeV4Info_t));
        _v4Zroute2Info(buf, &pInfo->info[routeNum], pPrev);

        if (SYS_L3_INTFTYPE_NULL == pInfo->info[routeNum].route.ointf.type)
            continue;

        if (0x7F000000 == pInfo->info[routeNum].route.dst)
            continue;

        pPrev = &pInfo->info[routeNum];

        routeNum++;
    }

    fclose(fp);

    pInfo->num = routeNum;

    j = 0;
    for (i = 0; i < staticRoutes.num; i++)
    {
        j = 0;
        while (j < routeNum)
        {
            #if 0
            SYS_PRINTF("i %d j %d sroute=%x_%d_%x pInfo=%x_%d_%x\n",
                            i,
                            j,
                            staticRoutes.route[i].dst,
                            staticRoutes.route[i].dstLen,
                            staticRoutes.route[i].gw,
                            pInfo->info[j].route.dst,
                            pInfo->info[j].route.dstLen,
                            pInfo->info[j].route.gw);
            #endif
            if ((staticRoutes.route[i].dst == pInfo->info[j].route.dst)
                && (staticRoutes.route[i].dstLen == pInfo->info[j].route.dstLen)
                && (staticRoutes.route[i].gw == pInfo->info[j].route.gw))
            {
                pInfo->info[j].metric = staticRoutes.route[i].pri;
                pInfo->info[j].route.pri = staticRoutes.route[i].pri;
                j++;
                break;
            }

            j++;
        }
    }
#endif
	SYS_MEM_CLEAR(routeTmp);
	routeTmp.route.dst = 0;
	routeTmp.route.dstLen = 32;
	while(1)
	{
		ret = sal_l3_ip_route_getnext(&routeTmp);
		if(ret != SYS_ERR_OK)
		{
			break;
		}
		#if 0
		SYS_PRINTF("routeTmp=%x_%d_%x\n",
                            routeTmp.route.dst,
                            routeTmp.route.dstLen,
                            routeTmp.route.gw);
            
        #endif
		if (SYS_L3_INTFTYPE_NULL == routeTmp.route.ointf.type)
            continue;

        if (0x7F000000 == routeTmp.route.dst)
            continue;
        osal_memcpy(&pInfo->info[routeNum], &routeTmp, sizeof(sys_l3_routeV4Info_t));   
		routeNum++;
	}
	pInfo->num = routeNum;

    j = 0;
    for (i = 0; i < staticRoutes.num; i++)
    {
        j = 0;
        while (j < routeNum)
        {
		#if 0
            SYS_PRINTF("i %d j %d sroute=%x_%d_%x pInfo=%x_%d_%x\n",
                            i,
                            j,
                            staticRoutes.route[i].dst,
                            staticRoutes.route[i].dstLen,
                            staticRoutes.route[i].gw,
                            pInfo->info[j].route.dst,
                            pInfo->info[j].route.dstLen,
                            pInfo->info[j].route.gw);
		#endif
            if ((staticRoutes.route[i].dst == pInfo->info[j].route.dst)
                && (staticRoutes.route[i].dstLen == pInfo->info[j].route.dstLen)
                && (staticRoutes.route[i].gw == pInfo->info[j].route.gw))
            {
                pInfo->info[j].metric = staticRoutes.route[i].pri;
                pInfo->info[j].route.pri = staticRoutes.route[i].pri;
                j++;
                break;
            }

            j++;
        }
    }

    return SYS_ERR_OK;
}

uint32 sal_l3_routeV4TakeEffect_ret(sys_l3_routeV4_t *pRoute)
{
    uint32 i = 0;
    sys_l3_ipv4Db_t *pCurr = NULL;

    FOR_EACH_L3_IPV4(i, pCurr)
    {
        if (IS_IPV4_IN_SUBNET(&pRoute->gw, &pCurr->ip))
        {
            if (IS_L3_INTF_LINKUP(&pCurr->intfId))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

int32 sal_l3_routeV6Info_get(sys_l3_routeV6InfoDb_t *pInfo)
{

    int32 ret = -1;

    uint32 routeNum = 0;
    uint32 i = 0;
    uint32 j = 0;

    sys_l3_staticRouteV6_t staticRoutes;
	sys_l3_routeV6Info_t  routeTmp;

    SYS_ERR_CHK(cfg_l3_staticRouteV6_get(&staticRoutes));

    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);
#if 0

    fp = tmpfile();

    ret = sal_l3_engineCmd2File_exec("do show ipv6 route", fp);

    if (SYS_ERR_OK != ret)
    {
        fclose(fp);
        return ret;
    }

    rewind(fp);

    while (!feof(fp))
    {
        if (NULL == fgets(buf, sizeof(buf), fp))
            break;

        line++;

        if (line <= _ZEBAR_ROUTE_TITLE_LINE_NUM)
            continue;

        pCurr = &pInfo->info[routeNum];
        osal_memset(pCurr, 0, sizeof(sys_l3_routeV6Info_t));
        _v6Zroute2Info(buf, pCurr, pPrev);

        if (IS_IPV6_LINKLOCAL(pCurr->route.dst))
            continue;

        if (IS_IPV6_MCAST(pCurr->route.dst))
            continue;

        if ((SYS_L3_ROUTETYPE_KERNEL == pCurr->type)
                && !pCurr->select)
            continue;

        if ((SYS_L3_INTFTYPE_END == pCurr->route.ointf.type)
                || (SYS_L3_ROUTETYPE_UNKNOWN == pCurr->type))
            continue;

        if (SYS_L3_ROUTETYPE_CONNECT == pCurr->type)
        {
            uint32 isSkip = FALSE;
            sys_l3_ipv6_t subnet;
            sys_l3_ipv6Db_t *pCurrDb = NULL;
            sys_l3_ipv6DadSts_t dadSts = SYS_L3_IPV6_DADSTS_NONE;

            FOR_EACH_L3_IPV6_IN_INTF(&pCurr->route.ointf, pCurrDb)
            {
                SYS_MEM_CLEAR(subnet);

                osal_memcpy(&subnet.addr, &pCurr->route.dst, sizeof(sys_ipv6_t));
                subnet.plen = pCurr->route.dstLen;

                if (IS_IPV6_IN_SUBNET(&pCurrDb->ip.addr, &subnet))
                {
                    sal_l3_intfIPv6DadStatus_get(&pCurr->route.ointf, &pCurrDb->ip, &dadSts);

                    if (SYS_L3_IPV6_DADSTS_DUPLICATE == dadSts)
                    {
                        isSkip = TRUE;
                        break;
                    }
                }
            }

            if (isSkip)
                continue;
        }

        pPrev = &pInfo->info[routeNum];

        routeNum++;
    }

    fclose(fp);
#endif
	SYS_MEM_CLEAR(routeTmp);
	routeTmp.route.dstLen = 128;
	while(1)
	{
		ret = sal_l3_ipv6_route_getnext(&routeTmp);
		if(ret != SYS_ERR_OK)
		{
			break;
		}
		
        if (IS_IPV6_LINKLOCAL(routeTmp.route.dst))
            continue;

        if (IS_IPV6_MCAST(routeTmp.route.dst))
            continue; 

        if ((SYS_L3_ROUTETYPE_KERNEL == routeTmp.type) && !routeTmp.select)
            continue; 
            
		if (SYS_L3_INTFTYPE_NULL == routeTmp.route.ointf.type)
            continue;

        if ((SYS_L3_INTFTYPE_END == routeTmp.route.ointf.type) || (SYS_L3_ROUTETYPE_UNKNOWN == routeTmp.type))
            continue;

        if (SYS_L3_ROUTETYPE_CONNECT == routeTmp.type)
        {
            uint32 isSkip = FALSE;
            sys_l3_ipv6_t subnet;
            sys_l3_ipv6Db_t *pCurrDb = NULL;
            sys_l3_ipv6DadSts_t dadSts = SYS_L3_IPV6_DADSTS_NONE;

            FOR_EACH_L3_IPV6_IN_INTF(&routeTmp.route.ointf, pCurrDb)
            {
                SYS_MEM_CLEAR(subnet);

                osal_memcpy(&subnet.addr, &routeTmp.route.dst, sizeof(sys_ipv6_t));
                subnet.plen = routeTmp.route.dstLen;

                if (IS_IPV6_IN_SUBNET(&pCurrDb->ip.addr, &subnet))
                {
                    sal_l3_intfIPv6DadStatus_get(&routeTmp.route.ointf, &pCurrDb->ip, &dadSts);

                    if (SYS_L3_IPV6_DADSTS_DUPLICATE == dadSts)
                    {
                        isSkip = TRUE;
                        break;
                    }
                }
            }

            if (isSkip)
                continue;
        }

        osal_memcpy(&pInfo->info[routeNum], &routeTmp, sizeof(sys_l3_routeV4Info_t));   
		routeNum++;
	}

    pInfo->num = routeNum;

    j = 0;
    for (i = 0; i < staticRoutes.num; i++)
    {
        j = 0;
        while (j < routeNum)
        {
        #if 0
            SYS_PRINTF("i %d j %d sroute="FMT_IPV6"_%d_"FMT_IPV6" pInfo="FMT_IPV6"_%d_"FMT_IPV6"\n",
                            i,
                            j,
                            SHOW_IPV6(staticRoutes.route[i].dst),
                            staticRoutes.route[i].dstLen,
                            SHOW_IPV6(staticRoutes.route[i].gw),
                            SHOW_IPV6(pInfo->info[j].route.dst),
                            pInfo->info[j].route.dstLen,
                            SHOW_IPV6(pInfo->info[j].route.gw));
        #endif
            if (IS_IPV6_EQUAL(staticRoutes.route[i].dst, pInfo->info[j].route.dst)
                && (staticRoutes.route[i].dstLen == pInfo->info[j].route.dstLen)
                && IS_IPV6_EQUAL(staticRoutes.route[i].gw, pInfo->info[j].route.gw))
            {
                pInfo->info[j].metric = staticRoutes.route[i].pri;
                pInfo->info[j].route.pri = staticRoutes.route[i].pri;
                if (0 == pInfo->info[j].route.pri && SYS_L3_ROUTETYPE_CONNECT != pInfo->info[j].type)
                    pInfo->info[j].route.pri = 1;
                j++;
                break;
            }

            j++;
        }
    }

    return SYS_ERR_OK;

}

uint32 sal_l3_routeV6TakeEffect_ret(sys_l3_routeV6_t *pRoute)
{
    uint32 i = 0;
    sys_l3_ipv6Db_t *pCurr = NULL;

    FOR_EACH_L3_IPV6(i, pCurr)
    {
        if (IS_IPV6_IN_SUBNET(&pRoute->gw, &pCurr->ip))
        {
            if (IS_L3_INTF_LINKUP(&pCurr->intfId))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

int32 sal_l3_staticRouteV4_add(sys_l3_routeV4_t *pRoute)
{
    char engcmd[SYS_BUF256_LEN];
    int32 i = -1;
    int32 j = -1;
    int32 insertIdx = -1;
    int32 replaceIdx = -1;
    int32 dupStartIdx = -1;
    int32 dupNum = 0;
    uint32 old_dfltRouteNum = 0;
    sys_l3_staticRouteV4_t rules;
    sys_l3_staticRouteV4_t rulesFinal;

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    pRoute->dst = pRoute->dst & IPV4_PFX2MASK(pRoute->dstLen);

    SYS_ERR_CHK(_sal_l3_routeV4Valid_chk(pRoute));

    if (0 == pRoute->pri)
        pRoute->pri = DFLT_L3_STATICROUTE_METRIC;
    else if (255 == pRoute->pri) 
        pRoute->ointf.type = SYS_L3_INTFTYPE_NULL;

    SYS_MEM_CLEAR(rulesFinal);
    SYS_ERR_CHK(cfg_l3_staticRouteV4_get(&rules));

    for (i = 0; i < CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX; i++)
    {
        if (i >= rules.num)
            break;

        if (IS_IPV4_ALLZERO(rules.route[i].dst) && (0 == rules.route[i].dstLen) && !IS_IPV4_ALLZERO(rules.route[i].gw))
        {
            if (IS_IPV4_ALLZERO(pRoute->dst) && (0 == pRoute->dstLen) && !IS_IPV4_ALLZERO(pRoute->gw))
                return SYS_ERR_L3_ROUTE_MAX_NH_NUM_EXCEED;
        }
        else if (0 == osal_memcmp(&pRoute->dst, &rules.route[i].dst, sizeof(sys_ipv4_t)))
        {
            if ((SYS_L3_INTFTYPE_NULL == pRoute->ointf.type && SYS_L3_INTFTYPE_NULL != rules.route[i].ointf.type)
                || (SYS_L3_INTFTYPE_NULL != pRoute->ointf.type && SYS_L3_INTFTYPE_NULL == rules.route[i].ointf.type))
            {
                return SYS_ERR_L3_ROUTE_ON_LO_INTF;
            }
        }
    }

    for (i = 0; i <= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX; i++)
    {
        if (i == rules.num)
        {
            insertIdx = i;
            break;
        }

        if (i >= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX)
            return SYS_ERR_INPUT;

        if (pRoute->dst > rules.route[i].dst)
            continue;

        if (pRoute->dst < rules.route[i].dst)
        {
            insertIdx = i;
            break;
        }

        if (pRoute->dstLen > rules.route[i].dstLen)
            continue;

        if (pRoute->dstLen > rules.route[i].dstLen)
        {
            insertIdx = i;
            break;
        }

        dupStartIdx = i;

        if (dupStartIdx >= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX)
            return SYS_ERR_INPUT;

        for (j = i; j <= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX; j++)
        {
            if (j == rules.num)
            {
                if (insertIdx < 0)
                    insertIdx = j;
                break;
            }

            if (j >= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX)
                return SYS_ERR_INPUT;

            if (pRoute->gw == rules.route[j].gw)
            {
                replaceIdx = j;
            }

            if (pRoute->dst != rules.route[j].dst || pRoute->dstLen != rules.route[j].dstLen)
            {
                if (insertIdx < 0)
                    insertIdx = j;
                break;
            }

            dupNum++;

            if (pRoute->pri > rules.route[j].pri)
                continue;

            if (pRoute->pri < rules.route[j].pri)
            {
                if (insertIdx < 0)
                    insertIdx = j;
                continue;
            }

            if (pRoute->gw > rules.route[j].gw)
                continue;

            if (insertIdx < 0)
                insertIdx = j;
        }

        break;
    }

    if (replaceIdx >= 0)
    {
        if (dupNum > 0 && replaceIdx != insertIdx && insertIdx >=0)
        {
            SYS_MEM_CLEAR(rulesFinal);
            rulesFinal.num = rules.num;

            if (replaceIdx > insertIdx)
            {
                if (replaceIdx >= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX - 1
                        || insertIdx >= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX - 1)
                    return SYS_ERR_INPUT;

                osal_memcpy(&rulesFinal.route[0],
                                &rules.route[0],
                                sizeof(sys_l3_routeV4_t) * (insertIdx));
                osal_memcpy(&rulesFinal.route[insertIdx],
                                pRoute,
                                sizeof(sys_l3_routeV4_t));
                osal_memcpy(&rulesFinal.route[insertIdx + 1],
                                &rules.route[insertIdx],
                                sizeof(sys_l3_routeV4_t) * (replaceIdx - insertIdx));
                osal_memcpy(&rulesFinal.route[replaceIdx + 1],
                                &rules.route[replaceIdx + 1],
                                sizeof(sys_l3_routeV4_t) * (rules.num - replaceIdx - 1));
            }
            else
            {
                if (replaceIdx >= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX - 1
                       || insertIdx >= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX)
                    return SYS_ERR_INPUT;

                osal_memcpy(&rulesFinal.route[0],
                                &rules.route[0],
                                sizeof(sys_l3_routeV4_t) * (replaceIdx));
                osal_memcpy(&rulesFinal.route[replaceIdx],
                                &rules.route[replaceIdx + 1],
                                sizeof(sys_l3_routeV4_t) * (insertIdx - replaceIdx));
                osal_memcpy(&rulesFinal.route[insertIdx - 1],
                                pRoute,
                                sizeof(sys_l3_routeV4_t));
                osal_memcpy(&rulesFinal.route[insertIdx],
                                &rules.route[insertIdx],
                                sizeof(sys_l3_routeV4_t) * (rules.num - insertIdx));
            }

            for (i = dupStartIdx; i < dupNum + dupStartIdx; i++)
            {

                sal_l3_ip_route_delete(rulesFinal.route[i].dst, IPV4_PFX2MASK(rulesFinal.route[i].dstLen), rulesFinal.route[i].gw, 0, 0);
            }

            for (i = dupStartIdx; i < dupNum + dupStartIdx; i++)
            {

                sal_l3_ip_route_add(rulesFinal.route[i].dst, IPV4_PFX2MASK(rulesFinal.route[i].dstLen), rulesFinal.route[i].gw, 0, 0);
            }

            SYS_ERR_CHK(cfg_l3_staticRouteV4_set(&rulesFinal));
        }
        else if (dupStartIdx >= 0)
        {
            osal_memcpy(&rules.route[dupStartIdx], pRoute, sizeof(sys_l3_routeV4_t));

            SYS_ERR_CHK(cfg_l3_staticRouteV4_set(&rules));
        }

        goto arp_send;
    }

    if (insertIdx > 0 && rules.num >= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX)
        return SYS_ERR_L3_STATIC_ROUTE_FULL;

    if (insertIdx < 0 || insertIdx >= CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX - 1)
        return SYS_ERR_FAILED;

    if (insertIdx == rules.num)
    {
        osal_memcpy(&rules.route[insertIdx], pRoute, sizeof(sys_l3_routeV4_t));

		sal_l3_ip_route_add(pRoute->dst, IPV4_PFX2MASK(pRoute->dstLen), pRoute->gw, 0, 0);

        rules.num++;

        if (IS_IPV4_ALLZERO(pRoute->dst))
        {
            old_dfltRouteNum = rules.dfltRouteNum;
            rules.dfltRouteNum++;
        }
        SYS_ERR_CHK(cfg_l3_staticRouteV4_set(&rules));

        if (0 == old_dfltRouteNum && rules.dfltRouteNum > 0)
        {
            SYSTEM_CMD_BG("handler -d dflt_route");
        }

        goto arp_send;
    }

    if (dupNum > 0 && insertIdx < (dupStartIdx + dupNum))
    {
        for (i = dupStartIdx; i < dupNum + dupStartIdx; i++)
        {

            sal_l3_ip_route_delete(rules.route[i].dst, IPV4_PFX2MASK(rules.route[i].dstLen), rules.route[i].gw, 0, 0);
        }
    }

    SYS_MEM_CLEAR(rulesFinal);
    rulesFinal.num = rules.num;
    osal_memcpy(&rulesFinal.route[0], &rules.route[0], sizeof(sys_l3_routeV4_t) * (insertIdx));
    osal_memcpy(&rulesFinal.route[insertIdx], pRoute, sizeof(sys_l3_routeV4_t));
    osal_memcpy(&rulesFinal.route[insertIdx + 1],
                    &rules.route[insertIdx],
                    sizeof(sys_l3_routeV4_t) * (rules.num - insertIdx));

    if (dupNum > 0 && insertIdx < (dupStartIdx + dupNum))
    {
        for (i = dupStartIdx; i < dupNum + dupStartIdx + 1; i++)
        {

            sal_l3_ip_route_add(rulesFinal.route[i].dst, IPV4_PFX2MASK(rulesFinal.route[i].dstLen), rulesFinal.route[i].gw, 0, 0);
        }
    }
    else
    {

        sal_l3_ip_route_add(pRoute->dst, IPV4_PFX2MASK(pRoute->dstLen), pRoute->gw, 0, 0);
    }

    rulesFinal.num++;

    if (IS_IPV4_ALLZERO(pRoute->dst))
    {
        old_dfltRouteNum = rulesFinal.dfltRouteNum;
        rulesFinal.dfltRouteNum++;
    }
    SYS_ERR_CHK(cfg_l3_staticRouteV4_set(&rulesFinal));

    if (0 == old_dfltRouteNum && rulesFinal.dfltRouteNum > 0)
    {
        SYSTEM_CMD_BG("handler -d dflt_route");
    }

arp_send:
    
    if (IS_SYSTEM_READY())
    {
        SYS_MEM_CLEAR(engcmd);
        IPV42STR(pRoute->gw, engcmd);
        SYSTEM_CMD_HID("ping -q -c 1 -W 1 -w 1 %s", engcmd);
    }

    return SYS_ERR_OK;
}

int32 sal_l3_staticRouteV4_del(sys_l3_routeV4_t *pRoute)
{

    int32 i = -1;
    int32 delIdx = -1;
    uint32 old_dfltRouteNum = 0;
    sys_l3_staticRouteV4_t rules;

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_staticRouteV4_get(&rules));

    if (rules.num < 0)
        return SYS_ERR_L3_STATIC_ROUTE_NOT_EXIST;

    for (i = 0; i < CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX; i++)
    {
        if (pRoute->dst == rules.route[i].dst
                && pRoute->dstLen == rules.route[i].dstLen
                && pRoute->gw == rules.route[i].gw)
        {
            delIdx = i;
            break;
        }
    }

    if (delIdx < 0)
        return SYS_ERR_L3_STATIC_ROUTE_NOT_EXIST;

    if (255 == rules.route[delIdx].pri)
        pRoute->ointf.type = SYS_L3_INTFTYPE_NULL;

    if (delIdx < CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX - 1)
    {
        osal_memcpy(&rules.route[delIdx],
                        &rules.route[delIdx + 1],
                        sizeof(sys_l3_routeV4_t) * (rules.num - delIdx - 1));
    }

    osal_memset(&rules.route[rules.num - 1], 0, sizeof(sys_l3_routeV4_t));

    sal_l3_ip_route_delete(pRoute->dst, IPV4_PFX2MASK(pRoute->dstLen), pRoute->gw, 0, 0);

    rules.num--;
    if (IS_IPV4_ALLZERO(pRoute->dst))
    {
        old_dfltRouteNum = rules.dfltRouteNum;
        rules.dfltRouteNum--;
    }
    SYS_ERR_CHK(cfg_l3_staticRouteV4_set(&rules));

    if (old_dfltRouteNum > 0 && 0 == rules.dfltRouteNum)
    {
        SYSTEM_CMD_BG("handler -d dflt_route");
    }

    return SYS_ERR_OK;
}

int32 sal_l3_staticRouteV4_get(sys_l3_staticRouteV4_t *pRoute)
{
    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_staticRouteV4_get(pRoute));

    return SYS_ERR_OK;
}

int32 sal_l3_staticRouteV4Engine_add(sys_l3_routeV4_t *pRoute)
{
    char engcmd[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(engcmd);
    _v4Route2EngCmd(TRUE, pRoute, engcmd);
    sal_l3_engineCmd_exec(engcmd);

    return SYS_ERR_OK;
}

int32 sal_l3_staticRouteV6_add(sys_l3_routeV6_t *pRoute)
{

    int32 i = -1;
    int32 j = -1;
    int32 insertIdx = -1;
    int32 replaceIdx = -1;
    int32 dupStartIdx = -1;
    int32 dupNum = 0;
    sys_l3_routeV6_t *pRuleDst = NULL;
    sys_l3_routeV6_t *pRuleSrc = NULL;
    sys_l3_staticRouteV6_t rules;
    sys_l3_staticRouteV6_t rulesFinal;
    sys_l3_ipv6_t ip;
    sys_l3_ipv6_t subnet;

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ip);
    SYS_MEM_CLEAR(subnet);
    osal_memcpy(&ip.addr, &pRoute->dst, sizeof(sys_ipv6_t));
    ip.plen = pRoute->dstLen;
    IPV6_L3IP2SUBNET(&ip, &subnet);

    osal_memcpy(&pRoute->dst, &subnet.addr, sizeof(sys_ipv6_t));

    SYS_ERR_CHK(_sal_l3_routeV6Valid_chk(pRoute));

    if (0 == pRoute->pri)
        pRoute->pri = DFLT_L3_STATICROUTE_METRIC;

    SYS_MEM_CLEAR(rulesFinal);
    SYS_ERR_CHK(cfg_l3_staticRouteV6_get(&rules));

    for (i = 0; i < CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX; i++)
    {
        if (i >= rules.num)
            break;

        if (0 != osal_memcmp(&pRoute->dst, &rules.route[i].dst, sizeof(sys_ipv6_t)))
            continue;

        if ((SYS_L3_INTFTYPE_NULL == pRoute->ointf.type && SYS_L3_INTFTYPE_NULL != rules.route[i].ointf.type)
            || (SYS_L3_INTFTYPE_NULL != pRoute->ointf.type && SYS_L3_INTFTYPE_NULL == rules.route[i].ointf.type))
        {
            return SYS_ERR_L3_ROUTE_ON_LO_INTF;
        }

        return SYS_ERR_L3_ROUTE_MAX_NH_NUM_EXCEED;
    }

    for (i = 0; i <= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX; i++)
    {
        if (i == rules.num)
        {
            insertIdx = i;

            break;
        }

        if (i >= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX)
            return SYS_ERR_INPUT;

        if (osal_memcmp(&pRoute->dst, &rules.route[i].dst, sizeof(sys_ipv6_t)) > 0)
            continue;

        if (osal_memcmp(&pRoute->dst, &rules.route[i].dst, sizeof(sys_ipv6_t)) < 0)
        {
            insertIdx = i;
            break;
        }

        if (pRoute->dstLen > rules.route[i].dstLen)
            continue;

        if (pRoute->dstLen > rules.route[i].dstLen)
        {
            insertIdx = i;
            break;
        }

        dupStartIdx = i;

        for (j = i; j <= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX; j++)
        {
            if (j == rules.num)
            {
                if (insertIdx < 0)
                    insertIdx = j;
                break;
            }

            if (j >= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX)
                return SYS_ERR_INPUT;

            if (osal_memcmp(&pRoute->gw, &rules.route[j].gw, sizeof(sys_ipv6_t)) == 0)
            {
                replaceIdx = j;
            }

            if ((osal_memcmp(&pRoute->dst, &rules.route[j].dst, sizeof(sys_ipv6_t)) != 0)
                || pRoute->dstLen != rules.route[j].dstLen)
            {
                if (insertIdx < 0)
                    insertIdx = j;
                break;
            }

            dupNum++;

            if (pRoute->pri > rules.route[j].pri)
                continue;

            if (pRoute->pri < rules.route[j].pri)
            {
                if (insertIdx < 0)
                    insertIdx = j;
                continue;
            }

            if (osal_memcmp(&pRoute->gw, &rules.route[j].gw, sizeof(sys_ipv6_t)) > 0)
                continue;

            if (insertIdx < 0)
                insertIdx = j;
        }

        break;
    }

    if (replaceIdx >= 0)
    {
        if (dupNum > 0 && replaceIdx != insertIdx)
        {
            SYS_MEM_CLEAR(rulesFinal);
            rulesFinal.num = rules.num;

            if (insertIdx < 0)
                return SYS_ERR_FAILED;

            if (replaceIdx > insertIdx)
            {
                if (replaceIdx >= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX - 1
                        || insertIdx >= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX - 1)
                    return SYS_ERR_INPUT;

                pRuleDst = &rulesFinal.route[0];
                pRuleSrc = &rules.route[0];
                osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t) * (insertIdx));

                pRuleDst = &rulesFinal.route[insertIdx];
                pRuleSrc = pRoute;
                osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t));

                pRuleDst = &rulesFinal.route[insertIdx + 1];
                pRuleSrc = &rules.route[insertIdx];
                osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t) * (replaceIdx - insertIdx));

                pRuleDst = &rulesFinal.route[replaceIdx + 1];
                pRuleSrc = &rules.route[replaceIdx + 1];
                osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t) * (rules.num - replaceIdx - 1));
            }
            else
            {
                if (replaceIdx >= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX - 1
                       || insertIdx >= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX)
                    return SYS_ERR_INPUT;

                pRuleDst = &rulesFinal.route[0];
                pRuleSrc = &rules.route[0];
                osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t) * (replaceIdx));

                pRuleDst = &rulesFinal.route[replaceIdx];
                pRuleSrc = &rules.route[replaceIdx + 1];
                osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t) * (insertIdx - replaceIdx));

                pRuleDst = &rulesFinal.route[insertIdx - 1];
                pRuleSrc = pRoute;
                osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t));

                pRuleDst = &rulesFinal.route[insertIdx];
                pRuleSrc = &rules.route[insertIdx];
                osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t) * (rules.num - insertIdx));
            }

            for (i = dupStartIdx; i < dupNum + dupStartIdx; i++)
            {

                sal_l3_ipv6_route_delete(rulesFinal.route[i].dst, rulesFinal.route[i].dstLen, rulesFinal.route[i].gw, 
                	rulesFinal.route[i].ointf);
            }

            for (i = dupStartIdx; i < dupNum + dupStartIdx; i++)
            {

                sal_l3_ipv6_route_add(rulesFinal.route[i].dst, rulesFinal.route[i].dstLen, rulesFinal.route[i].gw, 
                	rulesFinal.route[i].ointf);
            }

            SYS_ERR_CHK(cfg_l3_staticRouteV6_set(&rulesFinal));
        }
        else
        {
            osal_memcpy(&rules.route[dupStartIdx], pRoute, sizeof(sys_l3_routeV6_t));

            SYS_ERR_CHK(cfg_l3_staticRouteV6_set(&rules));
        }

        return SYS_ERR_OK;
    }

    if (insertIdx > 0 && rules.num >= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX)
        return SYS_ERR_L3_STATIC_ROUTE_FULL;

    if (insertIdx < 0 || insertIdx >= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX - 1)
        return SYS_ERR_FAILED;

    if (insertIdx == rules.num)
    {
        osal_memcpy(&rules.route[insertIdx], pRoute, sizeof(sys_l3_routeV6_t));

        sal_l3_ipv6_route_add(pRoute->dst, pRoute->dstLen, pRoute->gw, pRoute->ointf);

        rules.num++;
        SYS_ERR_CHK(cfg_l3_staticRouteV6_set(&rules));

        return SYS_ERR_OK;
    }

    if (dupNum > 0 && insertIdx < (dupStartIdx + dupNum))
    {
        for (i = dupStartIdx; i < dupNum + dupStartIdx; i++)
        {

            sal_l3_ipv6_route_delete(rules.route[i].dst, rules.route[i].dstLen, rules.route[i].gw, 
                	rules.route[i].ointf);
        }
    }

    SYS_MEM_CLEAR(rulesFinal);
    rulesFinal.num = rules.num;
    osal_memcpy(&rulesFinal.route[0], &rules.route[0], sizeof(sys_l3_routeV6_t) * (insertIdx));

    pRuleDst = &rulesFinal.route[insertIdx];
    pRuleSrc = pRoute;
    osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t));

    pRuleDst = &rulesFinal.route[insertIdx + 1];
    pRuleSrc = &rules.route[insertIdx];
    osal_memcpy(pRuleDst, pRuleSrc, sizeof(sys_l3_routeV6_t) * (rules.num - insertIdx));

    if (dupNum > 0 && insertIdx < (dupStartIdx + dupNum))
    {
        for (i = dupStartIdx; i < dupNum + dupStartIdx + 1; i++)
        {

            sal_l3_ipv6_route_add(rulesFinal.route[i].dst, rulesFinal.route[i].dstLen, rulesFinal.route[i].gw, 
                	rulesFinal.route[i].ointf);
        }
    }
    else
    {

        sal_l3_ipv6_route_add(pRoute->dst, pRoute->dstLen, pRoute->gw, pRoute->ointf);
    }

    rulesFinal.num++;
    SYS_ERR_CHK(cfg_l3_staticRouteV6_set(&rulesFinal));

    if (IS_IPV6_ALLZERO(pRoute->dst) && 0 == pRoute->dstLen)
    {
        SYSTEM_CMD_HID("ip -6 route del default dev %s", L3_INTF_MGMTVLAN);
    }

    return SYS_ERR_OK;
}

int32 sal_l3_staticRouteV6_del(sys_l3_routeV6_t *pRoute)
{

    int32 i = -1;
    int32 delIdx = -1;
    sys_l3_staticRouteV6_t rules;
    sys_l3_routeV6_t oldRoute;

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_staticRouteV6_get(&rules));

    if (rules.num < 0)
        return SYS_ERR_L3_STATIC_ROUTE_NOT_EXIST;

    for (i = 0; i < CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX; i++)
    {
        if ((osal_memcmp(&pRoute->dst, &rules.route[i].dst, sizeof(sys_ipv6_t)) == 0)
                && pRoute->dstLen == rules.route[i].dstLen
                && (osal_memcmp(&pRoute->gw, &rules.route[i].gw, sizeof(sys_ipv6_t)) == 0))
        {
            delIdx = i;
            break;
        }
    }

    if (delIdx < 0 || delIdx >= CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX)
        return SYS_ERR_L3_STATIC_ROUTE_NOT_EXIST;

    SYS_MEM_CLEAR(oldRoute);
    osal_memcpy(&oldRoute, &rules.route[delIdx], sizeof(sys_l3_routeV6_t));

    if (delIdx < CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX - 1)
    {
        osal_memcpy(&rules.route[delIdx],
                        &rules.route[delIdx + 1],
                        sizeof(sys_l3_routeV6_t) * (rules.num - delIdx - 1));
    }

    osal_memset(&rules.route[rules.num - 1], 0, sizeof(sys_l3_routeV6_t));

    sal_l3_ipv6_route_delete(oldRoute.dst, oldRoute.dstLen, oldRoute.gw, oldRoute.ointf);

    rules.num--;
    SYS_ERR_CHK(cfg_l3_staticRouteV6_set(&rules));

    return SYS_ERR_OK;
}

int32 sal_l3_staticRouteV6_get(sys_l3_staticRouteV6_t *pRoute)
{
    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_staticRouteV6_get(pRoute));

    return SYS_ERR_OK;
}

int32 sal_l3_staticRouteV6Engine_add(sys_l3_routeV6_t *pRoute)
{
    char engcmd[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(engcmd);
    _v6Route2EngCmd(TRUE, pRoute, engcmd);
    sal_l3_engineCmd_exec(engcmd);

    return SYS_ERR_OK;
}

int32 sal_l3_routev6Prefer_set(uint32 val)
{
    SYS_ERR_CHK(cfg_l3_routev6Prefer_set(val));

    return SYS_ERR_OK;
}

int32 sal_l3_routev6Prefer_get(uint32 *pVal)
{
    SYS_PARAM_CHK((NULL == pVal), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_routev6Prefer_get(pVal));

    return SYS_ERR_OK;
}
#endif

int32 sal_l3_routeV4DefalutGw_get(sys_l3_routeV4_t *pGw)
{
    char buf[SYS_BUF128_LEN];
    FILE *fp = NULL;
    char *tok[5];

    SYS_PARAM_CHK((NULL == pGw), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(buf);

    osal_sprintf(buf, "ip route show table %s", L3_INTF_MGMTVLAN);

    if (NULL == (fp = popen(buf, "r")))
        return SYS_ERR_FILE_OPEN;

    while (fgets(buf, sizeof(buf), fp))
    {
        tok[0] = strtok(buf, " \n"); 

        if (NULL == osal_strstr(tok[0], "default"))
            continue;

        tok[1] = strtok(NULL, " \n"); 

        tok[2] = strtok(NULL, " \n"); 

        STR2IPV4(tok[2], &pGw->gw);

        break;
    }

    pclose(fp);

    pGw->ointf.id = L3_DB_INTFM_ID();
    pGw->ointf.type = SYS_L3_INTFTYPE_MGMTVLAN;

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_IPV6

int32 sal_l3_routeV6DefalutGw_get(sys_l3_routeV6_t *pGw)
{
    char buf[SYS_BUF128_LEN];
    FILE *fp = NULL;
    char *tok[5];

    SYS_PARAM_CHK((NULL == pGw), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(buf);

    osal_sprintf(buf, "ip -6 route show table %s", L3_INTF_MGMTVLAN);

    if (NULL == (fp = popen(buf, "r")))
        return SYS_ERR_FILE_OPEN;

    while (fgets(buf, sizeof(buf), fp))
    {
        tok[0] = strtok(buf, " \n"); 

        if (NULL == osal_strstr(tok[0], "default"))
            continue;

        tok[1] = strtok(NULL, " \n"); 

        tok[2] = strtok(NULL, " \n"); 

        STR2IPV6(tok[2], &pGw->gw);

        break;
    }

    pclose(fp);

    pGw->ointf.id = L3_DB_INTFM_ID();
    pGw->ointf.type = SYS_L3_INTFTYPE_MGMTVLAN;

    return SYS_ERR_OK;
}
#endif

