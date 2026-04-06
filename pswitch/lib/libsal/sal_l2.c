
#include <libsal/sal_util.h>
#include <libsal/sal_l2.h>

int32 sal_l2_db_gen(void)
{
    SYS_ERR_CHK(rsd_l2_db_gen());

    return SYS_ERR_OK;
}

int32 sal_l2_sort_start(sys_l2_sortMeth_t meth, uint32 *pNum)
{
    uint32 i = 0;
    uint32 currIdx = 0;
    uint32 sortIdx = 0;
    uint32 entryNum = 0;
    sys_l2_dbEntry_t *pCurr = NULL;

    SYS_PARAM_CHK((NULL == pNum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_l2_db_gen());

    if (!sys_l2_dbMem_map())
        return SYS_ERR_FAILED;

    osal_memset(g_l2DbSort, 0, g_l2Db->entryNum.hwTotal);

    for (i = 0; i < CAPA_L2_MAC_HASH_ENTRY_NUM; i++)
    {
        if (!IS_HASH_ACTIVE(i))
            continue;

        currIdx = g_l2DbHash[i].entryIdx;

        do
        {
            pCurr = L2_DB_ENTRY_PTR(currIdx);

            if (!pCurr->active)
                break;

            if (IS_LOGIC_PORT_INVALID(pCurr->l2Entry.port))
            {
                currIdx = pCurr->nextEntryIdx;
                continue;
            }

            g_l2DbSort[sortIdx].entryIdx = currIdx;
            sortIdx++;
            entryNum++;

            currIdx = pCurr->nextEntryIdx;
        } while (currIdx != L2_DB_ENTRY_EMPTY);
    }

    sys_l2_qsort(g_l2DbSort, entryNum, sizeof(sys_l2_dbSort_t), SYS_L2_SORTDATA_DBENTRY, meth);

    *pNum = entryNum;

    return SYS_ERR_OK;
}

int32 sal_l2_mac_search(sys_l2_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VID_INVALID(pEntry->vid), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_l2_mac_search(pEntry));

    return SYS_ERR_OK;
}

sys_l2_entry_t *sal_l2_macNext_ret(sys_l2_entry_t *pPrevEntry)
{
    int32 ret = -1;
    int32 i = 0;
    int32 currIdx = 0;
    sys_l2_dbEntry_t *pCurr = NULL;
    sys_l2_entry_t *pEntry = NULL;
    sys_l2_entry_t *pFoundPrev = NULL;

    ret = rsd_l2_db_gen();

    if (SYS_ERR_OK != ret)
        return NULL;

    if (!sys_l2_dbMem_map())
        return NULL;

    for (i = 0; i < CAPA_L2_MAC_HASH_ENTRY_NUM; i++)
    {
        if (!IS_HASH_ACTIVE(i))
            continue;

        currIdx = g_l2DbHash[i].entryIdx;

        do
        {
            pCurr = L2_DB_ENTRY_PTR(currIdx);

            if (!pCurr->active)
                break;

            pEntry = L2_ENTRY_PTR(currIdx);

            if (NULL == pPrevEntry)
                return pEntry;
            
            else if (NULL != pFoundPrev)
                return pEntry;
            
            else if (pEntry == pPrevEntry)
                pFoundPrev = pEntry;

            currIdx = pCurr->nextEntryIdx;
        } while (currIdx != L2_DB_ENTRY_EMPTY);
    }

    return NULL;
}

uint32 sal_l2_isSysMac_ret(sys_l2_entry_t *pChkEntry)
{
    sys_mac_t sysMac;

    if (NULL == pChkEntry)
        return FALSE;

    SYS_MEM_CLEAR(sysMac);

    sal_sys_macAddr_get(&sysMac);

    if (0 == osal_memcmp(&sysMac, &pChkEntry->mac, sizeof(sys_mac_t)))
        return TRUE;

    return FALSE;
}

uint32 sal_l2_isMcastMac_ret(sys_l2_entry_t *pChkEntry)
{
    if (NULL == pChkEntry)
        return FALSE;

    if (0x1 & pChkEntry->mac.octet[0])
        return TRUE;

    return FALSE;
}

uint32 sal_l2_isAllZeroMac_ret(sys_l2_entry_t *pChkEntry)
{
    sys_mac_t allzero;

    if (NULL == pChkEntry)
        return TRUE;

    SYS_MEM_CLEAR(allzero);

    if (0 == osal_memcmp(&allzero, &pChkEntry->mac, sizeof(sys_mac_t)))
        return TRUE;

    return FALSE;
}

int32 sal_l2_dynamicMac_del(sys_l2_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    pEntry->type = SYS_L2_TYPE_DYNAMIC;

    SYS_ERR_CHK(rsd_l2_mac_del(pEntry));

    return SYS_ERR_OK;
}

int32 sal_l2_dynamicMac_clear(void)
{
    sys_l2_clearCfg_t cfg;

    SYS_MEM_CLEAR(cfg);

    cfg.type = SYS_L2_CLEAR_ALL;

    SYS_ERR_CHK(rsd_l2_mac_clear(&cfg));

    return SYS_ERR_OK;
}

int32 sal_l2_dynamicMac_clearByPort(sys_logic_port_t port)
{
    sys_l2_clearCfg_t cfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(cfg);

    cfg.type = SYS_L2_CLEAR_BY_PORT;
    cfg.port = port;

    SYS_ERR_CHK(rsd_l2_mac_clear(&cfg));

    return SYS_ERR_OK;
}

int32 sal_l2_dynamicMac_clearByVlan(sys_vid_t vid)
{
    sys_l2_clearCfg_t cfg;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(cfg);

    cfg.type = SYS_L2_CLEAR_BY_VLAN;
    cfg.port = vid;

    SYS_ERR_CHK(rsd_l2_mac_clear(&cfg));

    return SYS_ERR_OK;
}

int32 sal_l2_staticMac_add(sys_l2_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    if (IS_SYS_MAC(pEntry))
        return SYS_ERR_L2_STATIC_EQ_SYSMAC;

    if (IS_MCAST_MAC(pEntry))
        return SYS_ERR_L2_STATIC_BE_MCAST;

    if (IS_ALLZERO_MAC(pEntry))
        return SYS_ERR_L2_STATIC_AZERO_INVALID;

    if (!sys_l2_dbMem_map())
        return SYS_ERR_FAILED;

#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    
    if (sal_l2_blacklistMacExist_ret(pEntry))
        return SYS_ERR_L2_MAC_CONFIT_WITH_FILTER;
#endif

#ifdef CONFIG_SYS_PORT_SECURITY
    
    if (sal_psecure_macExist_ret(pEntry))
        return SYS_ERR_PSECURE_MAC_CONFIT_WITH_PSECURE;

    {
         sys_enable_t psecureEnbl = DISABLED;

         sal_psecure_portEnable_get(pEntry->port, &psecureEnbl);

         if (ENABLED == psecureEnbl)
            return SYS_ERR_PSECURE_FORBID_STATIC;
    }
#endif

    if (sal_l2_staticMacExist_ret(pEntry))
        SYS_ERR_CHK(rsd_l2_mac_del(pEntry));

    if (g_l2Db->entryNum.statics >= CAPA_L2_STATIC_MAC_NUM)
        return SYS_ERR_L2_MAC_FULL;

    pEntry->type = SYS_L2_TYPE_STATIC;

    SYS_ERR_CHK(rsd_l2_mac_add(pEntry));

    return SYS_ERR_OK;
}

int32 sal_l2_staticMac_del(sys_l2_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    pEntry->type = SYS_L2_TYPE_STATIC;

    SYS_ERR_CHK(rsd_l2_mac_del(pEntry));

    return SYS_ERR_OK;
}

sys_l2_entry_t *sal_l2_staticMacNext_ret(sys_l2_entry_t *pPrevEntry)
{
    if (!sys_l2_dbMem_map())
        return NULL;

    return L2_STATIC_ENTRY_NEXT(pPrevEntry);
}

sys_l2_entry_t *sal_l2_staticMacByIdx_ret(uint32 idx)
{
    int32 dbIdx = L2_DB_ENTRY_EMPTY;

    if (!sys_l2_dbMem_map())
        return NULL;

    dbIdx = L2_STATIC_ENTRY_DBIDX(idx);

    if (L2_DB_ENTRY_EMPTY == dbIdx)
        return NULL;

    return L2_ENTRY_PTR(dbIdx);
}

uint32 sal_l2_staticMacExistOnPort_ret(sys_logic_port_t port)
{
    uint32 i;
    sys_l2_entry_t *pCurr = NULL;

    for (i = 0; i < CAPA_L2_STATIC_MAC_NUM; i++)
    {
        pCurr = sal_l2_staticMacByIdx_ret(i);

        if (NULL == pCurr)
            continue;

        if (pCurr->port == port)
            return TRUE;
    }

    return FALSE;
}

uint32 sal_l2_staticMacExist_ret(sys_l2_entry_t *pChkEntry)
{
    uint32 i;
    sys_l2_entry_t *pCurr = NULL;

    for (i = 0; i < CAPA_L2_STATIC_MAC_NUM; i++)
    {
        pCurr = sal_l2_staticMacByIdx_ret(i);

        if (NULL == pCurr)
            continue;

        if (IS_L2_ENTRY_EQUAL(pCurr, pChkEntry))
            return TRUE;
    }

    return FALSE;
}

#ifdef CONFIG_SYS_BLACK_HOLE_MAC

int32 sal_l2_blacklistMac_add(sys_l2_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    if (IS_SYS_MAC(pEntry))
        return SYS_ERR_L2_FILTER_EQ_SYSMAC;

    if (IS_MCAST_MAC(pEntry))
        return SYS_ERR_L2_FILTER_BE_MCAST;

    if (IS_ALLZERO_MAC(pEntry))
        return SYS_ERR_L2_FILTER_AZERO_INVALID;

    if (sal_l2_staticMacExist_ret(pEntry))
        return SYS_ERR_L2_MAC_CONFIT_WITH_STATIC;

#ifdef CONFIG_SYS_PORT_SECURITY
    
    if (sal_psecure_macExist_ret(pEntry))
        return SYS_ERR_PSECURE_MAC_CONFIT_WITH_PSECURE;
#endif

    if (sal_l2_blacklistMacExist_ret(pEntry))
        SYS_ERR_CHK(rsd_l2_mac_del(pEntry));

    if (g_l2Db->entryNum.blacklist >= CAPA_L2_FILTER_MAC_NUM)
        return SYS_ERR_L2_MAC_FULL;

    pEntry->type = SYS_L2_TYPE_BLACKLIST;

    SYS_ERR_CHK(rsd_l2_mac_add(pEntry));

    return SYS_ERR_OK;
}

int32 sal_l2_blacklistMac_del(sys_l2_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    pEntry->type = SYS_L2_TYPE_BLACKLIST;

    SYS_ERR_CHK(rsd_l2_mac_del(pEntry));

    return SYS_ERR_OK;
}

sys_l2_entry_t *sal_l2_blacklistMacNext_ret(sys_l2_entry_t *pPrevEntry)
{
    if (!sys_l2_dbMem_map())
        return NULL;

    return L2_BLACKLIST_ENTRY_NEXT(pPrevEntry);
}

sys_l2_entry_t *sal_l2_blacklistMacByIdx_ret(uint32 idx)
{
    int32 dbIdx = L2_DB_ENTRY_EMPTY;

    if (!sys_l2_dbMem_map())
        return NULL;

    dbIdx = L2_BLACKLIST_ENTRY_DBIDX(idx);

    if (L2_DB_ENTRY_EMPTY == dbIdx)
        return NULL;

    return L2_ENTRY_PTR(dbIdx);
}

uint32 sal_l2_blacklistMacExist_ret(sys_l2_entry_t *pChkEntry)
{
    uint32 i;
    sys_l2_entry_t *pCurr = NULL;

    for (i = 0; i < CAPA_L2_FILTER_MAC_NUM; i++)
    {
        pCurr = sal_l2_blacklistMacByIdx_ret(i);

        if (NULL == pCurr)
            continue;

        if (IS_L2_ENTRY_EQUAL(pCurr, pChkEntry))
            return TRUE;
    }

    return FALSE;
}

#endif

int32 sal_l2_aging_get(uint32 *pAging_time)
{
    SYS_PARAM_CHK((NULL == pAging_time), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l2_aging_get(pAging_time));

    return SYS_ERR_OK;
} 

int32 sal_l2_aging_set(uint32 aging_time)
{
    SYS_ERR_CHK(rsd_l2_aging_set(aging_time));
    SYS_ERR_CHK(cfg_l2_aging_set(aging_time));

    return SYS_ERR_OK;
}

