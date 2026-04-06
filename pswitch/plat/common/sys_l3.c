#include <common/sys_def.h>
#include <common/sys_l3.h>

#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <rsd/rsd_sys.h>
#else
#include <sys/mman.h>
#include <sys/sem.h>
#include <libcfg/cfg.h>
#include <libsal/sal_l3.h>
#endif
sys_l3_dbHead_t   *g_l3Db = NULL;
sys_l3_intfMgmt_t *g_l3DbIntfMgmt = NULL;
#ifdef CONFIG_SYS_L3_ROUTE
sys_l3_intf_t     *g_l3DbIntfVlan = NULL;
sys_l3_intf_t     *g_l3DbIntfLo = NULL;
#ifdef CONFIG_SYS_L3_VRRP
sys_l3_intf_t     *g_l3DbIntfVrrp = NULL;
#endif
sys_l3_devMap_t   *g_l3DbDevMap = NULL;
sys_l3_ipDbHead_t *g_l3DbIntfIPv4Pool = NULL;
sys_l3_ipDbHead_t *g_l3DbIntfIPv6Pool = NULL;
sys_l3_dbSort_t   *g_l3DbIntfIPv4Sort = NULL;
sys_l3_dbSort_t   *g_l3DbIntfIPv6Sort = NULL;
#endif

  #ifdef __KERNEL__
static uint32     g_l3DbSize = 0;
  #else
  #endif
#ifdef __KERNEL__

static int32 l3db_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int32 l3db_release(struct inode *inode, struct file *filp)
{
    return 0;
}
static int32 _l3db_vmem(struct file *filp, struct vm_area_struct *vma)
{
    int32 ret;
    long length = vma->vm_end - vma->vm_start;
    unsigned long start = vma->vm_start;
    char *db_ptr = (char *)g_l3Db;
    unsigned long pfn;

    if (length > g_l3DbSize)
        return -EIO;

    while (length > 0)
    {
        pfn = vmalloc_to_pfn(db_ptr);
        if ((ret = remap_pfn_range(vma, start, pfn, PAGE_SIZE, PAGE_SHARED)) < 0)
        {
            return ret;
        }
        start += PAGE_SIZE;
        db_ptr += PAGE_SIZE;
        length -= PAGE_SIZE;
    }
    return 0;
}
static int32 l3db_mmap(struct file *filp, struct vm_area_struct *vma)
{
    
    if (vma->vm_pgoff == 0)
    {
        return _l3db_vmem(filp, vma);
    }
    
    return -EIO;
}
static struct file_operations l3db_fops = {
    .open = l3db_open,
    .release = l3db_release,
    .mmap = l3db_mmap,
    .owner = THIS_MODULE,
};
int32 sys_l3_init(void)
{
    uint32 i = 0;
    uint32 dbSize = 0;
    int32 ret = 0;
    uint32 currOffset = 0;

    dbSize = (sizeof(sys_l3_dbHead_t)
              + (sizeof(sys_l3_intfMgmt_t))
#ifdef CONFIG_SYS_L3_ROUTE
              + (sizeof(sys_l3_intf_t) * (SYS_VLAN_NUM_MAX))
              + (sizeof(sys_l3_intf_t) * (CAPA_L3_INTFLO_NUM_MAX))
#ifdef CONFIG_SYS_L3_VRRP
              + (sizeof(sys_l3_intf_t) * (CAPA_L3_INTFVRRP_NUM_MAX))
#endif
              + (sizeof(sys_l3_devMap_t))
              + (sizeof(sys_l3_ipv4Db_t) * (CAPA_L3_IPV4POOL_NUM))
              + (sizeof(sys_l3_ipv6Db_t) * (CAPA_L3_IPV6POOL_NUM))
              + (sizeof(sys_l3_dbSort_t) * (CAPA_L3_IPV4POOL_NUM))
              + (sizeof(sys_l3_dbSort_t) * (CAPA_L3_IPV6POOL_NUM))
#endif
             );

    g_l3DbSize = ((dbSize / PAGE_SIZE) + 1) * PAGE_SIZE;

    if (NULL == (g_l3Db = (sys_l3_dbHead_t *)vmalloc(g_l3DbSize)))
    {
        SYS_DBG(DBG_L3, "Allocate memory failed");
        return SYS_ERR_FAILED;
    }
    
    for (i = 0; i < g_l3DbSize; i+= PAGE_SIZE)
    {
        SetPageReserved(vmalloc_to_page((void *)(((unsigned long)g_l3Db) + i)));
    }
    osal_memset(g_l3Db, 0, g_l3DbSize);

    if ((ret = register_chrdev(L3DB_MAJOR, L3DB_NAME, &l3db_fops)) < 0 )
    {
        SYS_DBG(DBG_L3, "unable to get major device %d", L3DB_MAJOR);
        return SYS_ERR_FAILED;
    }

    g_l3Db->dbSize = g_l3DbSize;

    currOffset += sizeof(sys_l3_dbHead_t);

    g_l3Db->ofs.intfMgmt = currOffset;
    g_l3DbIntfMgmt = (sys_l3_intfMgmt_t*)((void*)g_l3Db + currOffset);
    g_l3DbIntfMgmt->mgmtVlan = DFLT_VLAN_MGMT_VLAN;
    currOffset += (sizeof(sys_l3_intfMgmt_t));

#ifdef CONFIG_SYS_L3_ROUTE
    g_l3Db->ofs.intfVlan = currOffset;
    g_l3DbIntfVlan = (sys_l3_intf_t*)((void*)g_l3Db + currOffset);
    currOffset += (sizeof(sys_l3_intf_t) * (SYS_VLAN_NUM_MAX));
    g_l3Db->ofs.intfLo = currOffset;
    g_l3DbIntfLo = (sys_l3_intf_t*)((void*)g_l3Db + currOffset);
    currOffset += (sizeof(sys_l3_intf_t) * (CAPA_L3_INTFLO_NUM_MAX));

#ifdef CONFIG_SYS_L3_VRRP
    g_l3Db->ofs.intfVrrp = currOffset;
    g_l3DbIntfVrrp = (sys_l3_intf_t*)((void*)g_l3Db + currOffset);
    currOffset += (sizeof(sys_l3_intf_t) * (CAPA_L3_INTFVRRP_NUM_MAX + 1));
#endif

    g_l3Db->ofs.devMap = currOffset;
    g_l3DbDevMap = (sys_l3_devMap_t*)((void*)g_l3Db + currOffset);
    currOffset += (sizeof(sys_l3_devMap_t));

    g_l3Db->ofs.intfIpv4Pool = currOffset;
    g_l3DbIntfIPv4Pool = (sys_l3_ipDbHead_t*)((void*)g_l3Db + currOffset);
    g_l3DbIntfIPv4Pool->maxNum = CAPA_L3_IPV4POOL_NUM;
    currOffset += sizeof(sys_l3_ipDbHead_t) + (sizeof(sys_l3_ipv4Db_t) * (CAPA_L3_IPV4POOL_NUM));

    g_l3Db->ofs.intfIpv6Pool = currOffset;
    g_l3DbIntfIPv6Pool = (sys_l3_ipDbHead_t*)((void*)g_l3Db + currOffset);
    g_l3DbIntfIPv6Pool->maxNum = CAPA_L3_IPV6POOL_NUM;
    currOffset += sizeof(sys_l3_ipDbHead_t) + (sizeof(sys_l3_ipv6Db_t) * (CAPA_L3_IPV6POOL_NUM));

    g_l3Db->ofs.intfIpv4Sort = currOffset;
    g_l3DbIntfIPv4Sort = (sys_l3_dbSort_t*)((void*)g_l3Db + currOffset);
    currOffset += (sizeof(sys_l3_dbSort_t) * (CAPA_L3_IPV4POOL_NUM));

    g_l3Db->ofs.intfIpv6Sort = currOffset;
    g_l3DbIntfIPv6Sort = (sys_l3_dbSort_t*)((void*)g_l3Db + currOffset);
    currOffset += (sizeof(sys_l3_dbSort_t) * (CAPA_L3_IPV6POOL_NUM));

    for (i = 0; i < CAPA_L3_INTFVLAN_NUM; i++)
    {
        g_l3DbDevMap->vlan[i] = -1;
    }

    for (i = 0; i < CAPA_L3_INTFLO_NUM; i++)
    {
        g_l3DbDevMap->lo[i] = i;
    }
#ifdef CONFIG_SYS_L3_VRRP   
    for (i = 0; i < CAPA_L3_INTFVRRP_NUM; i++)
    {
        g_l3DbDevMap->vrrp[i] = i;
    }
#endif
#endif

    g_l3Db->dfltIPv6LinkLocal.hex[0] = 0xFE;
    g_l3Db->dfltIPv6LinkLocal.hex[1] = 0x80;
    sys_l3_ipv6SysEui64_convert(&g_l3Db->dfltIPv6LinkLocal);

    return SYS_ERR_OK;
}
#else 
uint32 sys_l3_dbMem_map(void)
{
    int32 fd = -1;
    int32 len = 0;
    int32 dbSize = 0;
    int32 ret = -1;

    USER_LOCK(SYS_USER_LOCK_L3DB);

    if (NULL != g_l3Db)
        goto exit;

    if ((fd = open(L3DB_DEV, O_RDONLY)) < 0)
        goto fail_exit;

    len = getpagesize();
    g_l3Db = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);

    osal_memcpy(&dbSize, g_l3Db, sizeof(int32));

    if (dbSize > len)
    {
        ret = munmap(g_l3Db, len);
        if (ret < 0)
            goto fail;

        len = dbSize;
        g_l3Db = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
        if (MAP_FAILED == g_l3Db)
            goto fail;
    }

    g_l3DbIntfMgmt = (sys_l3_intfMgmt_t*)((void*)g_l3Db + g_l3Db->ofs.intfMgmt);
#ifdef CONFIG_SYS_L3_ROUTE
    g_l3DbIntfVlan = (sys_l3_intf_t*)((void*)g_l3Db + g_l3Db->ofs.intfVlan);
    g_l3DbDevMap = (sys_l3_devMap_t*)((void*)g_l3Db + g_l3Db->ofs.devMap);
    g_l3DbIntfLo = (sys_l3_intf_t*)((void*)g_l3Db + g_l3Db->ofs.intfLo);
#ifdef CONFIG_SYS_L3_VRRP
    g_l3DbIntfVrrp = (sys_l3_intf_t*)((void*)g_l3Db + g_l3Db->ofs.intfVrrp);
#endif
    g_l3DbIntfIPv4Pool = (sys_l3_ipDbHead_t*)((void*)g_l3Db + g_l3Db->ofs.intfIpv4Pool);
    g_l3DbIntfIPv6Pool = (sys_l3_ipDbHead_t*)((void*)g_l3Db + g_l3Db->ofs.intfIpv6Pool);
    g_l3DbIntfIPv4Sort = (sys_l3_dbSort_t*)((void*)g_l3Db + g_l3Db->ofs.intfIpv4Sort);
    g_l3DbIntfIPv6Sort = (sys_l3_dbSort_t*)((void*)g_l3Db + g_l3Db->ofs.intfIpv6Sort);
#endif

    close(fd);

exit:
    USER_UNLOCK(SYS_USER_LOCK_L3DB);

    return TRUE;

fail:
    close(fd);

fail_exit:
    USER_UNLOCK(SYS_USER_LOCK_L3DB);

    return FALSE;
}
#endif

#ifdef CONFIG_SYS_L3_ROUTE
uint32 sys_l3_ipPoolNum_ret(sys_l3_af_t af)
{
    sys_l3_ipDbHead_t *pDbHead = NULL;

    if (!L3_DB_MAP())
        return 0;

    if (SYS_L3_AF_IPV4 == af)
        pDbHead = g_l3DbIntfIPv4Pool;
    else
        pDbHead = g_l3DbIntfIPv6Pool;

    return pDbHead->currNum;
}

sys_l3_ipDbList_t *sys_l3_ipPoolSortEntryPtr_ret(sys_l3_af_t af, int32 idx)
{
    sys_l3_dbSort_t *pDbSort = NULL;

    if (!L3_DB_MAP())
        return NULL;

    if (SYS_L3_AF_IPV4 == af)
        pDbSort = g_l3DbIntfIPv4Sort;
    else
        pDbSort = g_l3DbIntfIPv6Sort;

    return L3_DB_PTR(pDbSort[idx].entryOfs);
}

sys_l3_ipDbList_t *sys_l3_ipPoolIntfNextEntryPtr_ret(sys_l3_af_t af, sys_l3_intfId_t *pId, sys_l3_ipDbList_t *pCurr)
{
    sys_l3_ipDbList_t *pHead = NULL;
    sys_l3_ipDbList_t *pNext = NULL;
    sys_l3_intf_t *pIntf = NULL;

    if (!L3_DB_MAP())
        return NULL;

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        pIntf = &L3_DB_INTFV(pId->id);
    else if (SYS_L3_INTFTYPE_LO == pId->type)
        pIntf = &L3_DB_INTFL(pId->id);
#ifdef CONFIG_SYS_L3_VRRP
   	else if (SYS_L3_INTFTYPE_VRRP == pId->type)
        pIntf = &L3_DB_INTFVRRP(pId->vrid);
#endif
    else
        goto _exit;

    if (SYS_L3_AF_IPV4 == af)
        pHead = L3_DB_PTR(pIntf->ipv4Head);
    else
        pHead = L3_DB_PTR(pIntf->ipv6Head);

    if (NULL == pHead)
        goto _exit;

    if (NULL == pCurr)
    {
        pNext = pHead;
        goto _exit;
    }

    pNext = L3_DB_PTR(pCurr->next);

    if (pNext == pHead || pNext == pCurr)
        pNext = NULL;

_exit:
    return pNext;
}

sys_l3_ipDbList_t *sys_l3_ipPoolIntfEntryPtr_ret(sys_l3_af_t af, sys_l3_intfId_t *pId, sys_l3_ip_t *pIp)
{
    uint32 cmpSize = 0;
    sys_l3_ipDbList_t *pHead = NULL;
    sys_l3_ipDbList_t *pCurr = NULL;
    sys_l3_intf_t *pIntf = NULL;
    sys_l3_ip_t *pIpCmp = NULL;

    if (!L3_DB_MAP())
        return NULL;

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
        pIntf = &L3_DB_INTFV(pId->id);
    else if (SYS_L3_INTFTYPE_LO == pId->type)
        pIntf = &L3_DB_INTFL(pId->id);
#ifdef CONFIG_SYS_L3_VRRP
    else if (SYS_L3_INTFTYPE_VRRP == pId->type)
        pIntf = &L3_DB_INTFVRRP(pId->vrid);
#endif
    else
        goto _exit;

    if (SYS_L3_AF_IPV4 == af)
    {
        pHead = L3_DB_PTR(pIntf->ipv4Head);
        cmpSize = sizeof(sys_ipv4_t);
    }
    else
    {
        pHead = L3_DB_PTR(pIntf->ipv6Head);
        cmpSize = sizeof(sys_ipv6_t);
    }

    if (NULL == pHead)
        return NULL;

    pCurr = pHead;

    do
    {
        pIpCmp = (void*)pCurr + sizeof(sys_l3_ipDbList_t);

        if (0 == osal_memcmp(pIp, pIpCmp, cmpSize))
            return pCurr;

        pCurr = L3_DB_PTR(pCurr->next);
    } while (pCurr != pHead);

_exit:
    return NULL;
}
uint32 sys_l3_intfExist_ret(sys_l3_intfId_t *pId)
{
    if (NULL == pId)
        return FALSE;

    if (!L3_DB_MAP())
        return FALSE;

    switch (pId->type)
    {
        case SYS_L3_INTFTYPE_VLAN:
            return IS_L3_INTFV_EXIST(pId->id);
            break;

        case SYS_L3_INTFTYPE_MGMTVLAN:
            if (pId->id == L3_DB_INTFM_ID())
            {
                if (!IS_L3_INTFV_EXIST(pId->id))
                    return TRUE;

                if ((0 == L3_INTF_IPV4_NUM(pId)) || !IS_L3_INTF_IPV6ENBL(pId))
                    return TRUE;
            }
            break;
        case SYS_L3_INTFTYPE_LO:
            return IS_L3_INTFL_EXIST(pId->id);
            break;
#ifdef CONFIG_SYS_L3_VRRP
        case SYS_L3_INTFTYPE_VRRP:
            return IS_L3_INTFVRRP_EXIST(pId->vrid);
            break;
#endif            
        default:
            break;
    }

    return FALSE;
}
#endif
uint32 sys_l3_intfMgmtVlanActive_ret(sys_l3_af_t af)
{
    if (!L3_DB_MAP())
        return FALSE;

#ifdef CONFIG_SYS_L3_ROUTE
    if (IS_L3_INTFV_EXIST(L3_DB_INTFM_ID()))
    {
        sys_l3_intfId_t intfId;

        SYS_MEM_CLEAR(intfId);

        intfId.type = SYS_L3_INTFTYPE_VLAN;
        intfId.id = L3_DB_INTFM_ID();

        if (SYS_L3_AF_IPV4 == af)
        {
            if (L3_INTF_IPV4_NUM(&intfId) > 0)
                return FALSE;
        }
        else
        {
            if (IS_L3_INTF_IPV6ENBL(&intfId))
                return FALSE;
        }
    }
#endif

    return TRUE;
}
uint32 sys_l3_isVidMgmtVlan_ret(sys_vid_t vid)
{
    if (!L3_DB_MAP())
        return FALSE;

    if (vid == L3_DB_INTFM_ID())
        return TRUE;

    return FALSE;
}

#ifdef CONFIG_SYS_L3_ROUTE

uint32 sys_l3_intfLinkup_ret(sys_l3_intfId_t *pId)
{
    if (NULL == pId)
        return FALSE;

    if (!L3_DB_MAP())
        return FALSE;

    switch (pId->type)
    {
        case SYS_L3_INTFTYPE_VLAN:
            return IS_L3_INTFV_LINKUP(pId->id);
            break;
        case SYS_L3_INTFTYPE_LO:
            return IS_L3_INTFL_LINKUP(pId->id);
            break;
#ifdef CONFIG_SYS_L3_VRRP
        case SYS_L3_INTFTYPE_VRRP:
            return IS_L3_INTFVRRP_LINKUP(pId->vrid);
            break;
#endif
        default:
            break;
    }

    return FALSE;
}
uint32 sys_l3_intfIPv4Enbl_ret(sys_l3_intfId_t *pId)
{
    if (NULL == pId)
        return FALSE;

    if (!L3_DB_MAP())
        return FALSE;

    switch (pId->type)
    {
        case SYS_L3_INTFTYPE_VLAN:
            return IS_L3_INTFV_IPV4ENBL(pId->id);
            break;
        case SYS_L3_INTFTYPE_LO:
            return IS_L3_INTFL_IPV4ENBL(pId->id);
            break;
#ifdef CONFIG_SYS_L3_VRRP
        case SYS_L3_INTFTYPE_VRRP:
            return IS_L3_INTFVRRP_IPV4ENBL(pId->vrid);
            break;
#endif
        default:
            break;
    }
    return FALSE;
}
uint32 sys_l3_intfIPv6Enbl_ret(sys_l3_intfId_t *pId)
{
#ifdef __KERNEL__
    uint32 ret = FALSE;
    if (NULL == pId)
        return FALSE;

    switch (pId->type)
    {
        case SYS_L3_INTFTYPE_VLAN:
            ret = IS_L3_INTFV_IPV6ENBL(pId->id);
            break;
        case SYS_L3_INTFTYPE_LO:
            ret = IS_L3_INTFL_IPV6ENBL(pId->id);
            break;

        default:
            break;
    }

    if (ENABLED == ret)
        return TRUE;
#else
  #ifdef CONFIG_SYS_L3_ROUTE
    int32 ret = SYS_ERR_FAILED;
    sys_l3_ipv6Admin_t ipAdmin;

    ret = sal_l3_adminIntfIPv6_get(pId, &ipAdmin);

    if (SYS_ERR_OK == ret)
    {
        if (ENABLED == ipAdmin.enbl)
            return TRUE;
    }
  #else
    return TRUE;
  #endif
#endif
    return FALSE;
}
uint32 sys_l3_intfIpAddrNum_ret(sys_l3_af_t af, sys_l3_intfId_t *pId)
{
    if (NULL == pId)
        return 0;

    if (!L3_DB_MAP())
        return 0;

    switch (pId->type)
    {
        case SYS_L3_INTFTYPE_VLAN:
            if (SYS_L3_AF_IPV4 == af)
                return g_l3DbIntfVlan[pId->id].ipv4AddrNum;
            else
                return g_l3DbIntfVlan[pId->id].ipv6AddrNum;
            break;
        case SYS_L3_INTFTYPE_LO:
            if (SYS_L3_AF_IPV4 == af)
                return g_l3DbIntfLo[pId->id].ipv4AddrNum;
            else
                return g_l3DbIntfLo[pId->id].ipv6AddrNum;
            break;
#ifdef CONFIG_SYS_L3_VRRP
        case SYS_L3_INTFTYPE_VRRP:
            if (SYS_L3_AF_IPV4 == af)
                return g_l3DbIntfVrrp[pId->vrid].ipv4AddrNum;
            else
                return g_l3DbIntfVrrp[pId->vrid].ipv6AddrNum;
            break;
#endif
        default:
            break;
    }

    return 0;
}
uint32 sys_l3_intfIpExist_ret(sys_l3_af_t af, sys_l3_intfId_t *pId, sys_l3_ip_t *pIp)
{
    sys_l3_ipDbList_t *pIpDb = NULL;

    if (NULL == pId || NULL == pIp)
        return FALSE;

    pIpDb = sys_l3_ipPoolIntfEntryPtr_ret(af, pId, pIp);

    if (NULL != pIpDb)
        return TRUE;

    return FALSE;
}
uint32 sys_l3_intfNum_ret(sys_l3_intfType_t intfType)
{
    if (!L3_DB_MAP())
        return 0;

    switch (intfType)
    {
        case SYS_L3_INTFTYPE_VLAN:
            return g_l3Db->intfvNum;
        case SYS_L3_INTFTYPE_LO:
            return g_l3Db->intflNum;
#ifdef CONFIG_SYS_L3_VRRP
        case SYS_L3_INTFTYPE_VRRP:
            return g_l3Db->intfvrrpNum;
#endif
        default:
            break;
    }

    return 0;
}
#endif
int32 sys_l3_intfId2Idx_ret(sys_l3_intfId_t *pId)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    if (!L3_DB_MAP())
        return -1;

    switch (pId->type)
    {
        case SYS_L3_INTFTYPE_MGMTVLAN:
            return 0;

#ifdef CONFIG_SYS_L3_ROUTE
        case SYS_L3_INTFTYPE_LO:
            
            return pId->id;

        case SYS_L3_INTFTYPE_VLAN:
            return L3_DB_INTFV(pId->id).devIdx;
#ifdef CONFIG_SYS_L3_VRRP
        case SYS_L3_INTFTYPE_VRRP:
            return L3_DB_INTFVRRP(pId->vrid).devIdx;
#endif
#endif

        default:
            break;
    }

    return -1;
}
int32 sys_l3_intfIdx2Id_ret(sys_l3_intfType_t type, int32 idx)
{
    if (!L3_DB_MAP())
        return -1;

    switch (type)
    {
        case SYS_L3_INTFTYPE_MGMTVLAN:
            return 0;

#ifdef CONFIG_SYS_L3_ROUTE
        case SYS_L3_INTFTYPE_LO:
            if (idx < 0 || idx >= CAPA_L3_INTFLO_NUM)
                return -1;
            
            return idx;

        case SYS_L3_INTFTYPE_VLAN:
            if (idx < 0 || idx >= CAPA_L3_INTFVLAN_NUM)
                return -1;
            return g_l3DbDevMap->vlan[idx];
#ifdef CONFIG_SYS_L3_VRRP
        case SYS_L3_INTFTYPE_VRRP:
            if (idx < 0 || idx >= CAPA_L3_INTFVRRP_NUM)
                return -1;
            return g_l3DbDevMap->vrrp[idx];
#endif
#endif

        default:
            break;
    }

    return -1;
}
int32 sys_l3_intfId2Str(sys_l3_intfId_t *pId, char *devStr)
{
    int32 devIdx = -1;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == devStr), SYS_ERR_NULL_POINTER);

    devIdx = INTFID2IDX(pId);

    if (devIdx < 0)
        return SYS_ERR_FAILED;

    switch (pId->type)
    {
        case SYS_L3_INTFTYPE_LO:
            osal_sprintf(devStr, "%s%d", L3_INTF_PREFIX_LO, devIdx);
            break;

        case SYS_L3_INTFTYPE_VLAN:
            osal_sprintf(devStr, "%s%d", L3_INTF_PREFIX_VLAN, devIdx);
            break;

        case SYS_L3_INTFTYPE_MGMTVLAN:
            osal_sprintf(devStr, "%s", L3_INTF_MGMTVLAN);
            break;
#ifdef CONFIG_SYS_L3_VRRP
        case SYS_L3_INTFTYPE_VRRP:
            osal_sprintf(devStr, "%s%d", L3_INTF_PREFIX_VRRP, devIdx);
            break;
#endif
        default:
            break;
    }

    return SYS_ERR_OK;
}
int32 sys_l3_str2IntfId(char *devStr, sys_l3_intfId_t *pId)
{
    char *pIdxStart = NULL;
    int32 devIdx = -1;
    int32 id = -1;

    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == devStr), SYS_ERR_NULL_POINTER);

    if (!L3_DB_MAP())
        return SYS_ERR_FAILED;

    if (0 == osal_strncmp(devStr, L3_INTF_MGMTVLAN, osal_strlen(L3_INTF_MGMTVLAN)))
    {
        pId->type = SYS_L3_INTFTYPE_MGMTVLAN;
        pId->id = L3_DB_INTFM_ID();

        return SYS_ERR_OK;
    }
#ifdef CONFIG_SYS_L3_VRRP
	else if (0 == osal_strncmp(devStr, L3_INTF_PREFIX_VRRP, osal_strlen(L3_INTF_PREFIX_VRRP)))
	{
		pIdxStart = devStr + L3_INTF_PREFIX_VRRP_LEN;
	
#ifdef __KERNEL__
		devIdx = simple_strtol(pIdxStart, NULL, 10);
#else
		devIdx = SYS_STR2UINT(pIdxStart);
#endif
		pId->type = SYS_L3_INTFTYPE_VRRP;

		id = INTFIDX2ID(SYS_L3_INTFTYPE_VRRP, devIdx);

		if (id < 0)
			return SYS_ERR_FAILED;
		else
			pId->vrid = id;

		if ((g_l3DbIntfVrrp[pId->vrid].exist && g_l3DbIntfVrrp[pId->vrid].id))
		{
			pId->id = g_l3DbIntfVrrp[pId->vrid].id;
		}
		else
		{
			return SYS_ERR_FAILED;
		}
		
        return SYS_ERR_OK;
	}
#endif
    pIdxStart = devStr + L3_INTF_PREFIX_LEN;

#ifdef __KERNEL__
    devIdx = simple_strtol(pIdxStart, NULL, 10);
#else
    devIdx = SYS_STR2UINT(pIdxStart);
#endif

    if (0 == osal_strncmp(devStr, L3_INTF_PREFIX_VLAN, osal_strlen(L3_INTF_PREFIX_VLAN)))
    {
        pId->type = SYS_L3_INTFTYPE_VLAN;

        id = INTFIDX2ID(SYS_L3_INTFTYPE_VLAN, devIdx);

        if (id < 0)
            return SYS_ERR_FAILED;
        else
            pId->id = id;
    }
    else if (0 == osal_strncmp(devStr, L3_INTF_PREFIX_LO, osal_strlen(L3_INTF_PREFIX_LO)))
    {
        pId->type = SYS_L3_INTFTYPE_LO;

        id = INTFIDX2ID(SYS_L3_INTFTYPE_LO, devIdx);

        if (id < 0)
            return SYS_ERR_FAILED;
        else
            pId->id = id;
    }
    else if (0 == osal_strcmp(devStr, L3_INTF_LO))
    {
        pId->type = SYS_L3_INTFTYPE_END;
    }
    else if (0 == osal_strcmp(devStr, L3_INTF_INACTIVE))
    {
        pId->type = SYS_L3_INTFTYPE_INACTIVE;
    }
    else if (0 == osal_strcmp(devStr, L3_INTF_NULL))
    {
        pId->type = SYS_L3_INTFTYPE_NULL;
    }

    return SYS_ERR_OK;
}
int32 sys_l3_intfId2Name(sys_l3_intfId_t *pId, char *intfName)
{
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == intfName), SYS_ERR_NULL_POINTER);

    switch (pId->type)
    {
        case SYS_L3_INTFTYPE_INACTIVE:
            osal_sprintf(intfName, "inactive");
            break;

        case SYS_L3_INTFTYPE_LO:
            osal_sprintf(intfName, "%s%d", L3_INTF_NAME_LO, (pId->id + 1));
            break;

        case SYS_L3_INTFTYPE_VLAN:
            osal_sprintf(intfName, "%s%d", L3_INTF_NAME_VLAN, pId->id);
            break;

        case SYS_L3_INTFTYPE_MGMTVLAN:
            osal_sprintf(intfName, "MGMT VLAN");
            break;

        case SYS_L3_INTFTYPE_NULL:
            osal_sprintf(intfName, "Null0");
            break;
#ifdef CONFIG_SYS_L3_VRRP
        case SYS_L3_INTFTYPE_VRRP:
            osal_sprintf(intfName, "%s%d", L3_INTF_NAME_VRRP, pId->vrid);
            break;
#endif
        default:
            break;
    }

    return SYS_ERR_OK;
}
int32 sys_l3_str2IntfName(char *devStr, char *intfName)
{
    sys_l3_intfId_t intfId;

    SYS_PARAM_CHK((NULL == devStr), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == intfName), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(intfId);

    SYS_ERR_CHK(sys_l3_str2IntfId(devStr, &intfId));
    SYS_ERR_CHK(sys_l3_intfId2Name(&intfId, intfName));

    return SYS_ERR_OK;
}
int32 sys_l3_intfName2Id(char *intfName, sys_l3_intfId_t *pId)
{
    SYS_PARAM_CHK((NULL == intfName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pId), SYS_ERR_NULL_POINTER);

    if ('v' == tolower(intfName[0]))
    {
#ifdef CONFIG_SYS_L3_VRRP
    	if('r' == tolower(intfName[1]))
    	{
			pId->type = SYS_L3_INTFTYPE_VRRP;
        	pId->vrid = SYS_STR2UINT(intfName + osal_strlen(L3_INTF_NAME_VRRP));
    	}
    	else
#endif
    	{
        	pId->type = SYS_L3_INTFTYPE_VLAN;
        	pId->id = SYS_STR2UINT(intfName + osal_strlen(L3_INTF_NAME_VLAN));
        }
    }
    else if ('l' == tolower(intfName[0]))
    {
        uint32 numIdx = 0;
        uint32 loId = 0;
        uint32 i = 0;

        pId->type = SYS_L3_INTFTYPE_LO;

        for (i = 0; i < osal_strlen(intfName); i++)
        {
            if (intfName[i] >= 0x30 && intfName[i] <= 0x39)
            {
                numIdx = i;

                break;
            }
        }

        loId = SYS_STR2UINT(intfName + numIdx) - 1;

        if (loId < 0 || loId >= CAPA_L3_INTFLO_NUM)
            return SYS_ERR_L3_INTF_LO_ID_INVALID;

        pId->id = loId;
    }

    return SYS_ERR_OK;
}
int32 sys_l3_intfName2Str(char *intfName, char *devStr)
{
    sys_l3_intfId_t intfId;

    SYS_PARAM_CHK((NULL == intfName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == devStr), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(intfId);

    SYS_ERR_CHK(sys_l3_intfName2Id(intfName, &intfId));
    SYS_ERR_CHK(sys_l3_intfId2Str(&intfId, devStr));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_L3_ROUTE

int32 sys_l3_ip2Intf(sys_l3_af_t af, sys_l3_ip_t ip, sys_l3_intfId_t *pIntfId)
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 mask = 0;
    uint32 Remainder = 0;
    uint32 Quotient = 0;
    sys_l3_ipv4Db_t *v4Db = NULL;
    sys_l3_ipv6Db_t *v6Db = NULL;

    SYS_PARAM_CHK((NULL == pIntfId), SYS_ERR_NULL_POINTER);

    if (SYS_L3_AF_IPV4 == af)
    {
        FOR_EACH_L3_IPV4(i, v4Db)
        {

            mask = IPV4_PFX2MASK(v4Db->ip.plen);

            if ((v4Db->ip.addr & mask) == (ip.v4.addr & mask))
            {
                
                osal_memcpy(pIntfId, &v4Db->intfId, sizeof(v4Db->intfId));
                break;
            }
        }
    }
    else
    {
        FOR_EACH_L3_IPV6(i, v6Db)
        {
            Remainder = v6Db->ip.plen % 8;
            Quotient = v6Db->ip.plen / 8;

            for (j = 0; j < Quotient; j++)
            {
                if (v6Db->ip.addr.hex[j] != ip.v6.addr.hex[j])
                    break;
            }

            if (j != Quotient)
                continue;

            mask = 0xff << (8 - Remainder);

            if ((v6Db->ip.addr.hex[j] & mask) == (ip.v6.addr.hex[j] & mask))
            {
                
                osal_memcpy(pIntfId, &v6Db->intfId, sizeof(v6Db->intfId));
                break;
            }
        }
    }

    return SYS_ERR_OK;
}
#endif
int32 sys_l3_ipInSubnet_ret(sys_l3_af_t af, sys_ip_t *pIp, sys_l3_ip_t *pSubnet)
{
    uint32 j = 0;
    uint32 mask = 0;
    uint32 Remainder = 0;
    uint32 Quotient = 0;

    if (SYS_L3_AF_IPV4 == af)
    {
        mask = IPV4_PFX2MASK(pSubnet->v4.plen);

        if ((pSubnet->v4.addr & mask) == (pIp->v4 & mask))
        {
            
            return TRUE;
        }
    }
    else
    {
        Remainder = pSubnet->v6.plen % 8;
        Quotient = pSubnet->v6.plen / 8;

        for (j = 0; j < Quotient; j++)
        {
            if (pSubnet->v6.addr.hex[j] != pIp->v6.hex[j])
                break;
        }

        if (j != Quotient)
            return FALSE;

        mask = 0xff << (8 - Remainder);

        if ((pSubnet->v6.addr.hex[j] & mask) == (pIp->v6.hex[j] & mask))
        {
            
            return TRUE;
        }
    }

    return FALSE;
}
int32 sys_l3_ip2Subnet_get(sys_l3_af_t af, sys_l3_ip_t *pIp, sys_l3_ip_t *pSubnet)
{
    if (SYS_L3_AF_IPV4 == af)
    {
        sys_ipv4_t v4Mask = 0;

        v4Mask = sys_l3_ipv4Prefix2Mask_ret(pIp->v4.plen);

        pSubnet->v4.addr = pIp->v4.addr & v4Mask;
        pSubnet->v4.plen = pIp->v4.plen;
    }
    else
    {
        uint32 i = 0;
        sys_ipv6_t v6Mask;

        SYS_MEM_CLEAR(v6Mask);

        if (128 == pIp->v6.plen)
        {
            osal_memcpy(&pSubnet->v6.addr, &pIp->v6.addr, sizeof(sys_ipv6_t));
        }
        else
        {
            sys_l3_ipv6Prefix2Mask_ret(pIp->v6.plen, &v6Mask);

            for (i = 0; i < CAPA_IPV6_BYTE_NUM; i++)
            {
                pSubnet->v6.addr.hex[i] = pIp->v6.addr.hex[i] & v6Mask.hex[i] & 0xFF;
            }
        }
        pSubnet->v6.plen = pIp->v6.plen;
    }

    return SYS_ERR_OK;
}
int32 sys_l3_ipv4Mask2Prefix_ret(sys_ipv4_t mask)
{
    uint32 prefix = 0;

    while (mask > 0)
    {
        mask = mask << 1;
        prefix++;
    }

    return prefix;
}
sys_ipv4_t sys_l3_ipv4Prefix2Mask_ret(uint32 prefix)
{
    return (0 == prefix) ? 0 : (0xFFFFFFFF << (32 - prefix)) & 0xFFFFFFFF;
}
int32 sys_l3_ipv6Mask2Prefix_ret(sys_ipv6_t mask)
{
    uint32 prefix = 0;
    uint32 i = 0;

    for (i = 0; i < 16; i++)
    {
        if (mask.hex[i] == 0xFF)
            prefix += 8;
        else
        {
            switch (mask.hex[i])
            {
                case 0xFE: prefix += 7; break;
                case 0xFC: prefix += 6; break;
                case 0xF8: prefix += 5; break;
                case 0xF0: prefix += 4; break;
                case 0xE0: prefix += 3; break;
                case 0xC0: prefix += 2; break;
                case 0x80: prefix += 1; break;
                case 0x00: prefix += 0; break;
            }
        }
    }

    return prefix;
}
void sys_l3_ipv6Prefix2Mask_ret(uint32 prefix, sys_ipv6_t *mask)
{
    uint32 i = 0;
    uint32 Remainder = prefix % 8;
    uint32 Quotient = prefix / 8;

    for (i = 0; i < Quotient; i++)
        mask->hex[i] = 0xff;

    mask->hex[i] = 0xff << (8 - Remainder);

    return;
}
uint32 sys_l3_isIPv6AllZero_ret(sys_ipv6_t ip)
{
    sys_ipv6_t allZero;

    SYS_MEM_CLEAR(allZero);

    if (0 == osal_memcmp(&ip, &allZero, sizeof(sys_ipv6_t)))
        return TRUE;

    return FALSE;
}
int32 sys_l3_ipv6Eui64_convert(sys_mac_t *pMac, sys_ipv6_t *pIp)
{
    pIp->hex[8] = pMac->octet[0] | 0x2;
    pIp->hex[9] = pMac->octet[1];
    pIp->hex[10] = pMac->octet[2];
    pIp->hex[11] = 0xFF;
    pIp->hex[12] = 0xFE;
    pIp->hex[13] = pMac->octet[3];
    pIp->hex[14] = pMac->octet[4];
    pIp->hex[15] = pMac->octet[5];

    return SYS_ERR_OK;
}
uint32 sys_l3_ipv6Eui64Check_ret(sys_mac_t *pMac, sys_ipv6_t ip)
{
    if ((ip.hex[8] == (pMac->octet[0] | 0x2)) &&
            (ip.hex[9] == (pMac->octet[1])) &&
            (ip.hex[10] == (pMac->octet[2])) &&
            (ip.hex[11] == (0xFF)) &&
            (ip.hex[12] == (0xFE)) &&
            (ip.hex[13] == (pMac->octet[3])) &&
            (ip.hex[14] == (pMac->octet[4])) &&
            (ip.hex[15] == (pMac->octet[5])) )
    {
        return TRUE;
    }

    return FALSE;
}
int32 sys_l3_ipv6SysEui64_convert(sys_ipv6_t *pIp)
{
    sys_mac_t sysMac;

    SYS_MEM_CLEAR(sysMac);

#ifdef __KERNEL__
    SYS_ERR_CHK(rsd_sys_macAddr_get(SYS_DEV_ID_DEFAULT, &sysMac));
#else
    SYS_ERR_CHK(sal_sys_macAddr_get(&sysMac));
#endif

    SYS_ERR_CHK(sys_l3_ipv6Eui64_convert(&sysMac, pIp));

    return SYS_ERR_OK;
}
uint32 sys_l3_ipv6SysEui64Check_ret(sys_ipv6_t ip)
{
    sys_mac_t sysMac;

    SYS_MEM_CLEAR(sysMac);

#ifdef __KERNEL__
    SYS_ERR_CHK(rsd_sys_macAddr_get(SYS_DEV_ID_DEFAULT, &sysMac));
#else
    SYS_ERR_CHK(sal_sys_macAddr_get(&sysMac));
#endif

    return sys_l3_ipv6Eui64Check_ret(&sysMac, ip);
}
int32 sys_l3_ipv6SysLinkLocal_get(sys_l3_ipv6_t *pIp)
{
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    if (!L3_DB_MAP())
        return SYS_ERR_FAILED;

    osal_memcpy(&pIp->addr, &g_l3Db->dfltIPv6LinkLocal, sizeof(sys_ipv6_t));
    pIp->plen = 64;

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_L3_ROUTE

int32 sys_l3_ipv6LinkLocalAddr_get(sys_l3_intfId_t *pIntfId, sys_l3_ipv6_t *pIp)
{
    sys_l3_ipv6Db_t *pCurr = NULL;

    SYS_PARAM_CHK((NULL == pIntfId), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

    FOR_EACH_L3_IPV6_IN_INTF(pIntfId, pCurr)
    {
        if (SYS_L3_IPTYPE_V6LINKLOCAL == pCurr->list.type)
        {
            osal_memcpy(pIp, &pCurr->ip, sizeof(sys_l3_ipv6_t));

            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_L3_INTF_IP_NOT_EXIST;
}
#endif

#ifdef __KERNEL__
EXPORT_SYMBOL(sys_l3_intfId2Str);
EXPORT_SYMBOL(sys_l3_ipv4Mask2Prefix_ret);
EXPORT_SYMBOL(sys_l3_ipv4Prefix2Mask_ret);
EXPORT_SYMBOL(sys_l3_ipv6Mask2Prefix_ret);
EXPORT_SYMBOL(sys_l3_ipv6Prefix2Mask_ret);
#endif
