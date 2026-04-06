
#include <libsal/sal_util.h>
#include <libsal/sal_psecure.h>

const sys_text_t text_psecure_violation[] =
{
    {       "Protect"  },
    {       "Restrict" },
    {       "Shutdown" },
};

int32 sal_psecure_enable_set(sys_enable_t enable)
{
    sys_enable_t oriEnable = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_psecure_enable_get(&oriEnable));

    if(oriEnable != enable)
    {
        SYS_ERR_CHK(rsd_psecure_enable_set(enable));
        SYS_ERR_CHK(cfg_psecure_enable_set(enable));
    }

    return SYS_ERR_OK;
}

int32 sal_psecure_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_psecure_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_psecure_rateLimit_set(uint32 rate)
{
    SYS_ERR_CHK(rsd_psecure_rateLimit_set(rate));
    SYS_ERR_CHK(cfg_psecure_rateLimit_set(rate));

    return SYS_ERR_OK;
}

int32 sal_psecure_rateLimit_get(sys_enable_t *pRate)
{
    SYS_PARAM_CHK((NULL == pRate), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_psecure_rateLimit_get(pRate));

    return SYS_ERR_OK;
}

int32 sal_psecure_port_set(sys_logic_port_t port, sys_psecure_port_t *pPortCfg)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPortCfg), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_psecure_port_set(port, pPortCfg));
    SYS_ERR_CHK(cfg_psecure_port_set(port, pPortCfg));

    return SYS_ERR_OK;
}

int32 sal_psecure_port_get(sys_logic_port_t port, sys_psecure_port_t *pPortCfg)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPortCfg), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_psecure_port_get(port, pPortCfg));

    return SYS_ERR_OK;
}

int32 sal_psecure_portEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    sys_psecure_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);

    SYS_ERR_CHK(cfg_psecure_port_get(port, &portCfg));

    if(portCfg.enable != enable)
    {
        portCfg.enable = enable;

        if (ENABLED == enable)
        {
            if (sal_l2_staticMacExistOnPort_ret(port))
                return SYS_ERR_PSECURE_PORT_STATIC_MAC_EXIST;
        }

        SYS_ERR_CHK(rsd_psecure_port_set(port, &portCfg));
        SYS_ERR_CHK(cfg_psecure_port_set(port, &portCfg));
    }

    return SYS_ERR_OK;
}

int32 sal_psecure_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    sys_psecure_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);

    SYS_ERR_CHK(cfg_psecure_port_get(port, &portCfg));

    *pEnable = portCfg.enable;

    return SYS_ERR_OK;
}

int32 sal_psecure_portLimitNum_set(sys_logic_port_t port, uint32 limitNum)
{
    sys_psecure_port_t portCfg;
    sys_psecure_portInfo_t portInfo;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((limitNum < BOND_PSECURE_LIMITNUM_MIN || limitNum > BOND_PSECURE_LIMITNUM_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);

    SYS_ERR_CHK(cfg_psecure_port_get(port, &portCfg));
    SYS_ERR_CHK(rsd_psecure_portInfo_get(port, &portInfo));

    if (limitNum < portInfo.staticAddr + portInfo.stickyAddr)
    {
        if (ENABLED == portCfg.enable)
            return  SYS_ERR_PSECURE_MAX_LESS_CURRENT;
    }

    if(portCfg.limitNum != limitNum)
    {
        portCfg.limitNum = limitNum;

        SYS_ERR_CHK(rsd_psecure_port_set(port, &portCfg));
        SYS_ERR_CHK(cfg_psecure_port_set(port, &portCfg));

        SYS_LOG(LOG_CAT_PSECURE, LOG_PSECURE_COUNT_LIMIT, limitNum, LPSTR(port));
    }

    return SYS_ERR_OK;
}

int32 sal_psecure_portLimitNum_get(sys_logic_port_t port, sys_enable_t *pLimitNum)
{
    sys_psecure_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLimitNum), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);

    SYS_ERR_CHK(cfg_psecure_port_get(port, &portCfg));

    *pLimitNum = portCfg.limitNum;

    return SYS_ERR_OK;
}

int32 sal_psecure_portViolation_set(sys_logic_port_t port, sys_psecure_violation_t violation)
{
    sys_psecure_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(portCfg);

    SYS_ERR_CHK(cfg_psecure_port_get(port, &portCfg));

    if(portCfg.violation != violation)
    {
        portCfg.violation = violation;

        SYS_ERR_CHK(rsd_psecure_port_set(port, &portCfg));
        SYS_ERR_CHK(cfg_psecure_port_set(port, &portCfg));
    }

    return SYS_ERR_OK;
}

int32 sal_psecure_portViolation_get(sys_logic_port_t port, sys_psecure_violation_t *pViolation)
{
    sys_psecure_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pViolation), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);

    SYS_ERR_CHK(cfg_psecure_port_get(port, &portCfg));

    *pViolation = portCfg.violation;

    return SYS_ERR_OK;
}

int32 sal_psecure_portSticky_set(sys_logic_port_t port, sys_enable_t enable)
{
    sys_psecure_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(portCfg);

    SYS_ERR_CHK(cfg_psecure_port_get(port, &portCfg));

    if(portCfg.sticky != enable)
    {
        portCfg.sticky = enable;

        SYS_ERR_CHK(rsd_psecure_port_set(port, &portCfg));
        SYS_ERR_CHK(cfg_psecure_port_set(port, &portCfg));
    }

    return SYS_ERR_OK;
}

int32 sal_psecure_portSticky_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    sys_psecure_port_t portCfg;

    SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);

    SYS_ERR_CHK(cfg_psecure_port_get(port, &portCfg));

    *pEnable = portCfg.sticky;

    return SYS_ERR_OK;
}

int32 sal_psecure_portInfo_get(sys_logic_port_t port, sys_psecure_portInfo_t *pInfo)
{
    SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_psecure_portInfo_get(port, pInfo));

    return SYS_ERR_OK;
}

int32 sal_psecure_mac_add(sys_l2_entry_t *pEntry)
{
    uint32 i;
    sys_l2_entry_t *pCurr = NULL;
    sys_logic_port_t lp;
    sys_psecure_portInfo_t portInfo;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    if (pEntry->type != SYS_L2_TYPE_PSECURE_STATIC && pEntry->type != SYS_L2_TYPE_PSECURE_STICKY)
        return SYS_ERR_PSECURE_TYPE_INVALID;

    if (IS_SYS_MAC(pEntry))
        return SYS_ERR_PSECURE_EQ_SYSMAC;

    if (IS_MCAST_MAC(pEntry))
        return SYS_ERR_PSECURE_BE_MCAST;

    if (IS_ALLZERO_MAC(pEntry))
        return SYS_ERR_PSECURE_AZERO_INVALID;

    if (sal_l2_staticMacExist_ret(pEntry))
        return SYS_ERR_L2_MAC_CONFIT_WITH_STATIC;

#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    
    if (sal_l2_blacklistMacExist_ret(pEntry))
        return SYS_ERR_L2_MAC_CONFIT_WITH_FILTER;
#endif

    sal_l2_db_gen();

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(portInfo);
        sal_psecure_portInfo_get(lp, &portInfo);

        if (lp != pEntry->port)
        {
            if (0 == portInfo.totalAddr)
                continue;
        }

        for (i = 0; i < portInfo.totalAddr; i++)
        {
            pCurr = sal_psecure_portMacEntryByIdx_ret(lp, i);

            if (IS_PSECURE_ENTRY_EMPTY(pCurr))
                continue;

            if (IS_L2_ENTRY_EQUAL(pCurr, pEntry))
            {
                
                if (lp == pEntry->port && pCurr->type == pEntry->type)
                {
                    return SYS_ERR_OK;
                }
                
                else if (pCurr->type == SYS_L2_TYPE_PSECURE_STATIC
                            && pEntry->type == SYS_L2_TYPE_PSECURE_STICKY)
                {
                    return SYS_ERR_OK;
                }
                
                else if(lp != pEntry->port && pCurr->type == pEntry->type)
                {
					return SYS_ERR_ENTRY_IS_EXIST;
                }
                else
                {
                    SYS_ERR_CHK(rsd_psecure_mac_del(pCurr));
                    goto add_mac;
                }
            }
        }

        if (lp == pEntry->port)
        {
            if (portInfo.totalAddr >= portInfo.maxAddr)
                return SYS_ERR_PSECURE_MAC_FULL;
        }
    }

add_mac:
    SYS_ERR_CHK(rsd_psecure_mac_add(pEntry));

    return SYS_ERR_OK;
}

int32 sal_psecure_mac_del(sys_l2_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    if (pEntry->type != SYS_L2_TYPE_PSECURE_STATIC && pEntry->type != SYS_L2_TYPE_PSECURE_STICKY)
        return SYS_ERR_PSECURE_TYPE_INVALID;

    SYS_ERR_CHK(rsd_psecure_mac_del(pEntry));

    return SYS_ERR_OK;
}

int32 sal_psecure_mac_clear(sys_psecure_clearCfg_t *pClear)
{
    uint32 i;
    sys_logic_port_t lp;
    sys_l2_entry_t *pCurr = NULL;

    SYS_PARAM_CHK((NULL == pClear), SYS_ERR_NULL_POINTER);

    sal_l2_db_gen();

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        if ((SYS_PSECURE_CLEAR_ALL_BY_PORT == pClear->type)
            || (SYS_PSECURE_CLEAR_DYNAMIC_BY_PORT == pClear->type)
            || (SYS_PSECURE_CLEAR_STICKY_BY_PORT == pClear->type)
            || (SYS_PSECURE_CLEAR_STATIC_BY_PORT == pClear->type))
        {
            if (lp != pClear->port)
                continue;
        }

        for (i = 0; i < CAPA_PSECURE_LIMIT_NUM; i++)
        {
            pCurr = sal_psecure_portMacEntryByIdx_ret(lp, i);

            if (IS_PSECURE_ENTRY_EMPTY(pCurr))
                continue;

            if ((SYS_PSECURE_CLEAR_DYNAMIC_ALL == pClear->type)
                || (SYS_PSECURE_CLEAR_DYNAMIC_BY_PORT == pClear->type)
                || (SYS_PSECURE_CLEAR_DYNAMIC_BY_MAC == pClear->type))
            {
                if (pCurr->type != SYS_L2_TYPE_PSECURE_DYNAMIC)
                    continue;
            }
            else if ((SYS_PSECURE_CLEAR_STICKY_ALL == pClear->type)
                     || (SYS_PSECURE_CLEAR_STICKY_BY_PORT == pClear->type)
                     || (SYS_PSECURE_CLEAR_STICKY_BY_MAC == pClear->type))
            {
                if (pCurr->type != SYS_L2_TYPE_PSECURE_STICKY)
                    continue;
            }
            else if ((SYS_PSECURE_CLEAR_STATIC_ALL == pClear->type)
                     || (SYS_PSECURE_CLEAR_STATIC_BY_PORT == pClear->type)
                     || (SYS_PSECURE_CLEAR_STATIC_BY_MAC == pClear->type))
            {
                if (pCurr->type != SYS_L2_TYPE_PSECURE_STATIC)
                    continue;
            }

            if ((SYS_PSECURE_CLEAR_ALL_BY_MAC == pClear->type)
                || (SYS_PSECURE_CLEAR_DYNAMIC_BY_MAC == pClear->type)
                || (SYS_PSECURE_CLEAR_STICKY_BY_MAC == pClear->type)
                || (SYS_PSECURE_CLEAR_STATIC_BY_MAC == pClear->type))
            {
                if (0 == pClear->entry.vid)
                {
                    if (0 != osal_memcmp(&pCurr->mac, &pClear->entry.mac, sizeof(sys_mac_t)))
                        continue;
                }
                else if (!IS_L2_ENTRY_EQUAL(pCurr, &pClear->entry))
                {
                    continue;
                }
            }

            SYS_ERR_CHK(rsd_psecure_mac_del(pCurr));
        }
    }

    return SYS_ERR_OK;
}

sys_l2_entry_t *sal_psecure_portMacEntryByIdx_ret(sys_logic_port_t port, uint32 idx)
{
    sys_l2_entry_t *pEntry = NULL;

    if (!sys_l2_dbMem_map())
        return NULL;

    pEntry = L2_PSECURE_ENTRY_PTR(port, idx);

    if (!IS_PSECURE_ENTRY_EMPTY(pEntry))
        return pEntry;

    return NULL;
}

uint32 sal_psecure_macExist_ret(sys_l2_entry_t *pChkEntry)
{
    uint32 i;
    sys_l2_entry_t *pCurr = NULL;
    sys_logic_port_t lp;

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {

        for (i = 0; i < CAPA_PSECURE_LIMIT_NUM; i++)
        {
            pCurr = sal_psecure_portMacEntryByIdx_ret(lp, i);

            if (IS_PSECURE_ENTRY_EMPTY(pCurr))
                continue;

            if (IS_L2_ENTRY_EQUAL(pCurr, pChkEntry))
            {
                pChkEntry->type = pCurr->type;
                return TRUE;
            }
        }
    }

    return FALSE;
}

