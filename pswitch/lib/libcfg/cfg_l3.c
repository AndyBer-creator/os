
#include <libcfg/cfg.h>
#include <libsal/sal_l3.h>

static int32 _cfg_l3_intf_init(sys_l3_intfId_t *pId, cfg_l3_t *pL3)
{
    uint32 i = pId->id;

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
    {
        osal_memset(&pL3->intfv[i], 0, sizeof(sys_l3_intfAdmin_t));

        pL3->intfv[i].enbl = DFLT_L3_INTFV_ENBL;

        pL3->intfv[i].ipv6.enbl = DFLT_L3_INTF_IPV6_ENBL;
        pL3->intfv[i].ipv6.autoconfEnbl = DFLT_L3_IPV6_AUTOCONF_EBL;
        pL3->intfv[i].ipv6.unreachMsgSend = DFLT_L3_IPV6_DESTINATION_UNREACHABLE;
        pL3->intfv[i].ipv6.dadTxNum = DFLT_L3_IPV6_DADATTEMPT;
        pL3->intfv[i].ipv6.dhcp6c.enbl = DFLT_L3_IPV6_DHCPC_EBL;
#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS
        pL3->intfv[i].ipv6.dhcp6c.stateless = DFLT_L3_IPV6_DHCPC_STATELESS_EBL;
        pL3->intfv[i].ipv6.dhcp6c.refreshtime = DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH;
        pL3->intfv[i].ipv6.dhcp6c.min_refreshtime = DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH_MIN;
#endif

    }

    return SYS_ERR_OK;
}

int32 cfg_l3_init(void)
{
    uint32 i;
    cfg_l3_t *pL3 = NULL;
    sys_l3_intfId_t intfId;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->fwdIPv4 = !(DFLT_L3_IPV4_FORWARDING_EBL);
    pL3->fwdIPv6 = !(DFLT_L3_IPV6_FORWARDING_EBL);

    SYS_MEM_CLEAR(intfId);
    intfId.type = SYS_L3_INTFTYPE_VLAN;
    FOR_EACH_VID(i)
    {
        intfId.id = i;
        _cfg_l3_intf_init(&intfId, pL3);
    }

    for (i = 0; i < CAPA_L3_INTFLO_NUM_MAX; i++)
    {
        pL3->intfl[i].enbl = DFLT_L3_INTFV_ENBL;
    }

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intf_init(sys_l3_intfId_t *pId)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    _cfg_l3_intf_init(pId, pL3);

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_fwdIPv4Enable_set(sys_enable_t enable)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->fwdIPv4 = enable;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_fwdIPv4Enable_get(sys_enable_t *pEnable)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pEnable = pL3->fwdIPv4;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_fwdIPv6Enable_set(sys_enable_t enable)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->fwdIPv6 = enable;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_fwdIPv6Enable_get(sys_enable_t *pEnable)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pEnable = pL3->fwdIPv6;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_hopLimitIPv6_set(uint32 num)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->ipv6HopLimit = num;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_hopLimitIPv6_get(uint32 *pNum)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pNum = pL3->ipv6HopLimit;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_icmpv6RateInterval_set(uint32 time)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->icmpv6RateIntvl = time;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_icmpv6RateInterval_get(uint32 *pNum)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pNum = pL3->icmpv6RateIntvl;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_icmpv6RateBurst_set(uint32 time)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->icmpv6RateBurst = time;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_icmpv6RateBurst_get(uint32 *pNum)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pNum = pL3->icmpv6RateBurst;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_routev6Prefer_set(uint32 val)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->routeV6Pref = val;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_routev6Prefer_get(uint32 *pVal)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pVal = pL3->routeV6Pref;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intf_set(sys_l3_intfId_t *pId, sys_l3_intfAdmin_t *pAdmin)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pAdmin), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
    {
        if (IS_VID_INVALID(pId->id))
            goto _exit;

        osal_memcpy(&pL3->intfv[pId->id], pAdmin, sizeof(sys_l3_intfAdmin_t));
    }
    else if (SYS_L3_INTFTYPE_LO == pId->type)
    {
        if (pId->id >= CAPA_L3_INTFLO_NUM_MAX)
            goto _exit;

        osal_memcpy(&pL3->intfl[pId->id], pAdmin, sizeof(sys_l3_intfAdmin_t));
    }

_exit:
    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intf_get(sys_l3_intfId_t *pId, sys_l3_intfAdmin_t *pAdmin)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pAdmin), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
    {
        if (IS_VID_INVALID(pId->id))
            goto _exit;

        osal_memcpy(pAdmin, &pL3->intfv[pId->id], sizeof(sys_l3_intfAdmin_t));
    }
    else if (SYS_L3_INTFTYPE_LO == pId->type)
    {
        if (pId->id >= CAPA_L3_INTFLO_NUM_MAX)
            goto _exit;

        osal_memcpy(pAdmin, &pL3->intfl[pId->id], sizeof(sys_l3_intfAdmin_t));
    }

_exit:
    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intfStatusChg_set(sys_l3_intfId_t *pId, sys_l3_intfStsChg_t *pStsChg)
{
    int32 devIdx = -1;
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pStsChg), SYS_ERR_NULL_POINTER);

    devIdx = INTFID2IDX(pId);

    if (devIdx < 0)
        return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
    {
        if (IS_VID_INVALID(pId->id))
            goto _exit;

        osal_memcpy(&pL3->intfvStsChg[devIdx], pStsChg, sizeof(sys_l3_intfStsChg_t));
    }

_exit:
    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intfStatusChg_get(sys_l3_intfId_t *pId, sys_l3_intfStsChg_t *pStsChg)
{
    int32 devIdx = -1;
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pStsChg), SYS_ERR_NULL_POINTER);

    devIdx = INTFID2IDX(pId);

    if (devIdx < 0)
        return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
    {
        if (IS_VID_INVALID(pId->id))
            goto _exit;

        osal_memcpy(pStsChg, &pL3->intfvStsChg[devIdx], sizeof(sys_l3_intfStsChg_t));
    }

_exit:
    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intfEnable_set(sys_l3_intfId_t *pId, sys_enable_t enable)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        pL3->intfv[pId->id].enbl = enable;
    else if (SYS_L3_INTFTYPE_LO == pId->type)
        pL3->intfl[pId->id].enbl = enable;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intfEnable_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        *pEnable = pL3->intfv[pId->id].enbl;
    else if (SYS_L3_INTFTYPE_LO == pId->type)
        *pEnable = pL3->intfl[pId->id].enbl;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intfIPv4_set(sys_l3_intfId_t *pId, sys_l3_ipv4Admin_t *pIp)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        osal_memcpy(&pL3->intfv[pId->id].ipv4, pIp, sizeof(sys_l3_ipv4Admin_t));
    else if (SYS_L3_INTFTYPE_LO == pId->type)
        osal_memcpy(&pL3->intfl[pId->id].ipv4, pIp, sizeof(sys_l3_ipv4Admin_t));

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intfIPv4_get(sys_l3_intfId_t *pId, sys_l3_ipv4Admin_t *pIp)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        osal_memcpy(pIp, &pL3->intfv[pId->id].ipv4, sizeof(sys_l3_ipv4Admin_t));
    else if (SYS_L3_INTFTYPE_LO == pId->type)
        osal_memcpy(pIp, &pL3->intfl[pId->id].ipv4, sizeof(sys_l3_ipv4Admin_t));

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intfIPv6_set(sys_l3_intfId_t *pId, sys_l3_ipv6Admin_t *pIp)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        osal_memcpy(&pL3->intfv[pId->id].ipv6, pIp, sizeof(sys_l3_ipv6Admin_t));
    else if (SYS_L3_INTFTYPE_LO == pId->type)
        osal_memcpy(&pL3->intfl[pId->id].ipv6, pIp, sizeof(sys_l3_ipv6Admin_t));

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_intfIPv6_get(sys_l3_intfId_t *pId, sys_l3_ipv6Admin_t *pIp)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        osal_memcpy(pIp, &pL3->intfv[pId->id].ipv6, sizeof(sys_l3_ipv6Admin_t));
    else if (SYS_L3_INTFTYPE_LO == pId->type)
        osal_memcpy(pIp, &pL3->intfl[pId->id].ipv6, sizeof(sys_l3_ipv6Admin_t));

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_staticRouteV4_set(sys_l3_staticRouteV4_t *pRoute)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    osal_memcpy(&pL3->routeV4, pRoute, sizeof(sys_l3_staticRouteV4_t));

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_staticRouteV4_get(sys_l3_staticRouteV4_t *pRoute)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    osal_memcpy(pRoute, &pL3->routeV4, sizeof(sys_l3_staticRouteV4_t));

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_staticRouteV6_set(sys_l3_staticRouteV6_t *pRoute)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    osal_memcpy(&pL3->routeV6, pRoute, sizeof(sys_l3_staticRouteV6_t));

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_staticRouteV6_get(sys_l3_staticRouteV6_t *pRoute)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pRoute), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    osal_memcpy(pRoute, &pL3->routeV6, sizeof(sys_l3_staticRouteV6_t));

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_ageTime_set(uint32 ageTime)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->arpAgeTime = ageTime;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_ageTime_get(uint32 *pAgeTime)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pAgeTime), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pAgeTime = pL3->arpAgeTime;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_respTime_set(uint32 respTime)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->arpRespTime = respTime;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_respTime_get(uint32 *pRespTime)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pRespTime), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pRespTime = pL3->arpRespTime;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_retryTimes_set(uint32 retryTimes)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->arpRetryTimes = retryTimes;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_retryTimes_get(uint32 *pRetryTimes)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pRetryTimes), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pRetryTimes = pL3->arpRetryTimes;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_cacheSize_set(uint32 cacheSize)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->arpCacheSize = cacheSize;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_cacheSize_get(uint32 *pCacheSize)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pCacheSize), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pCacheSize = pL3->arpCacheSize;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_renew_set(sys_enable_t enable)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->dynamicRenew = enable;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_neigh_renew_get(sys_enable_t *pEnable)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pEnable = pL3->dynamicRenew;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_arpProxy_set(sys_logic_port_t lp, sys_enable_t enable)
{
    cfg_l3_t *pL3 = NULL;

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    pL3->arpProxy[lp] = enable;

    CFG_CLOSE(CFG_ID_L3);
}

int32 cfg_l3_arpProxy_get(sys_logic_port_t lp, sys_enable_t *pEnable)
{
    cfg_l3_t *pL3 = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L3, (cfg_addr)&pL3);

    *pEnable = pL3->arpProxy[lp];

    CFG_CLOSE(CFG_ID_L3);
}

