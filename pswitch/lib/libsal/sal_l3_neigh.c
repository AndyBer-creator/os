
#include <common/sys_log.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>

int32 sal_l3_neigh_ageTime_set(uint32 ageTime)
{
    uint32 i = 0;
    uint32 old = 0;
    char intfName[SYS_BUF8_LEN];

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(ageTime, BOND_L3_NEIGH_AGE_TIME_MIN, BOND_L3_NEIGH_AGE_TIME_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(cfg_l3_neigh_ageTime_get(&old));

    if (old == ageTime)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_l3_neigh_ageTime_set(ageTime));
    SYS_ERR_CHK(cfg_l3_neigh_ageTime_set(ageTime));

    for (i = 0; i < CAPA_L3_INTFVLAN_NUM_MAX; i++)
    {
        SYS_MEM_CLEAR(intfName);
        osal_sprintf(intfName, "%s%d", L3_INTF_PREFIX_VLAN, i);
        SYSTEM_CMD("echo %u > /proc/sys/net/ipv4/neigh/%s/base_reachable_time_ms;"
                          "echo %u > /proc/sys/net/ipv6/neigh/%s/base_reachable_time_ms",
                       ageTime*1000, intfName, ageTime*1000, intfName);
    }

    SYSTEM_CMD("echo %u > /proc/sys/net/ipv4/neigh/%s/base_reachable_time_ms;"
                      "echo %u > /proc/sys/net/ipv6/neigh/%s/base_reachable_time_ms",
                       ageTime*1000, "default", ageTime*1000, "default");

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_ageTime_get(uint32 *pAgeTime)
{
    SYS_PARAM_CHK((NULL == pAgeTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_neigh_ageTime_get(pAgeTime));

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_respTime_set(uint32 respTime)
{
    uint32 i = 0;
    uint32 old = 0;
    char intfName[SYS_BUF8_LEN];

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(respTime, BOND_L3_NEIGH_RESP_TIME_MIN, BOND_L3_NEIGH_RESP_TIME_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(cfg_l3_neigh_respTime_get(&old));

    if (old == respTime)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_l3_neigh_respTime_set(respTime));
    SYS_ERR_CHK(cfg_l3_neigh_respTime_set(respTime));

    for (i = 0; i < CAPA_L3_INTFVLAN_NUM_MAX; i++)
    {
        SYS_MEM_CLEAR(intfName);
        osal_sprintf(intfName, "%s%d", L3_INTF_PREFIX_VLAN, i);
        SYSTEM_CMD("echo %u > /proc/sys/net/ipv4/neigh/%s/retrans_time_ms;"
                       "echo %u > /proc/sys/net/ipv6/neigh/%s/retrans_time_ms",
                       respTime*1000, intfName, respTime*1000, intfName);
    }

    SYSTEM_CMD("echo %u > /proc/sys/net/ipv4/neigh/%s/retrans_time_ms;"
                      "echo %u > /proc/sys/net/ipv6/neigh/%s/retrans_time_ms",
                       respTime*1000, "default", respTime*1000, "default");

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_respTime_get(uint32 *pRespTime)
{
    SYS_PARAM_CHK((NULL == pRespTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_neigh_respTime_get(pRespTime));

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_retryTimes_set(uint32 retryTimes)
{
    uint32 i = 0;
    uint32 old = 0;
    char intfName[SYS_BUF8_LEN];

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(retryTimes, BOND_L3_NEIGH_RETRY_TIMES_MIN, BOND_L3_NEIGH_RETRY_TIMES_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(cfg_l3_neigh_retryTimes_get(&old));

    if (old == retryTimes)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_l3_neigh_retryTimes_set(retryTimes));
    SYS_ERR_CHK(cfg_l3_neigh_retryTimes_set(retryTimes));

    for (i = 0; i < CAPA_L3_INTFVLAN_NUM_MAX; i++)
    {
        SYS_MEM_CLEAR(intfName);
        osal_sprintf(intfName, "%s%d", L3_INTF_PREFIX_VLAN, i);
        SYSTEM_CMD("echo %u > /proc/sys/net/ipv4/neigh/%s/mcast_solicit;"
                       "echo %u > /proc/sys/net/ipv6/neigh/%s/mcast_solicit",
                       retryTimes+1, intfName, retryTimes+1, intfName);
    }

    SYSTEM_CMD("echo %u > /proc/sys/net/ipv4/neigh/%s/mcast_solicit;"
                      "echo %u > /proc/sys/net/ipv6/neigh/%s/mcast_solicit",
                       retryTimes+1, "default", retryTimes+1, "default");

    return SYS_ERR_OK;

}

int32 sal_l3_neigh_retryTimes_get(uint32 *pRetryTimes)
{
    SYS_PARAM_CHK((NULL == pRetryTimes), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_neigh_retryTimes_get(pRetryTimes));

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_cacheSize_set(uint32 cacheSize)
{
    uint32 old = 0;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(cacheSize, BOND_L3_NEIGH_CACHE_MIN, BOND_L3_NEIGH_CACHE_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(cfg_l3_neigh_cacheSize_get(&old));

    if (old == cacheSize)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_l3_neigh_cacheSize_set(cacheSize));
    SYS_ERR_CHK(cfg_l3_neigh_cacheSize_set(cacheSize));

    SYSTEM_CMD("echo %u > /proc/sys/net/ipv4/neigh/%s/gc_thresh3;"
                      "echo %u > /proc/sys/net/ipv6/neigh/%s/gc_thresh3",
                       cacheSize, "default", cacheSize, "default");

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_cacheSize_get(uint32 *pCacheSize)
{
    SYS_PARAM_CHK((NULL == pCacheSize), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_neigh_cacheSize_get(pCacheSize));

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_renew_set(sys_enable_t enable)
{

    uint32 i = 0;
    uint32 cnt = 0;
    char intfName[SYS_BUF8_LEN];
    sys_enable_t old = DISABLED;

    SYS_ERR_CHK(cfg_l3_neigh_renew_get(&old));

    if (old == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_l3_neigh_renew_set(enable));
    SYS_ERR_CHK(cfg_l3_neigh_renew_set(enable));

    if (DISABLED == enable)
        cnt = 3;
    else
        cnt = DFLT_L3_NEIGH_RENEW_COUNT;

    for (i = 0; i < CAPA_L3_INTFVLAN_NUM_MAX; i++)
    {
        SYS_MEM_CLEAR(intfName);
        osal_sprintf(intfName, "%s%d", L3_INTF_PREFIX_VLAN, i);
        SYSTEM_CMD("echo %u > /proc/sys/net/ipv4/neigh/%s/ucast_solicit;"
                          "echo %u > /proc/sys/net/ipv6/neigh/%s/ucast_solicit",
                       cnt, intfName, cnt, intfName);
    }

    SYSTEM_CMD("echo %u > /proc/sys/net/ipv4/neigh/%s/ucast_solicit;"
                      "echo %u > /proc/sys/net/ipv6/neigh/%s/ucast_solicit",
                       cnt, "default", cnt, "default");

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_renew_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_neigh_renew_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_clear(sys_l3_neighType_t type, sys_l3_intfId_t intf)
{
    SYS_ERR_CHK(rsd_l3_neigh_clear(type, intf));

    return SYS_ERR_OK;
}

int32 sal_l3_neighCnt_get(uint32 *pCnt)
{
    SYS_ERR_CHK(rsd_l3_neighCnt_get(pCnt));

    return SYS_ERR_OK;
}

int32 sal_l3_neigh_mac_validate(sys_mac_t *pMac)
{
    sys_mac_t mac;
    sys_logic_port_t lp;

    SYS_PARAM_CHK((NULL == pMac), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(mac);
    SYS_ERR_CHK(sal_sys_macAddr_get(&mac));

    if (0 == osal_memcmp(&mac, pMac, sizeof(sys_mac_t)))
        return SYS_ERR_L3_STATIC_NEIGH_MAC_SAMEDUT;

    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(mac);
        SYS_ERR_CHK(sal_sys_portMacAddr_get(lp, &mac));

        if (0 == osal_memcmp(&mac, pMac, sizeof(sys_mac_t)))
            return SYS_ERR_L3_STATIC_NEIGH_MAC_SAMEDUT;
    }

    return SYS_ERR_OK;
}

extern int32 sal_l3_hrouteEntry_del(sys_ip_t ip, sys_mac_t mac)
{
    SYS_ERR_CHK(rsd_l3_hrouteEntry_del(ip, mac));

    return SYS_ERR_OK;
}

int32 sal_l3_arpProxy_set(sys_logic_port_t lp, sys_enable_t enable)
{
    SYS_ERR_CHK(cfg_l3_arpProxy_set(lp, enable));

    return SYS_ERR_OK;
}

int32 sal_l3_arpProxy_get(sys_logic_port_t lp, sys_enable_t *pEnable)
{
    SYS_ERR_CHK(cfg_l3_arpProxy_get(lp, pEnable));

    return SYS_ERR_OK;
}

