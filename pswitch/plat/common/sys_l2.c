#include <common/sys_def.h>
#include <common/sys_l2.h>

#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <rtk/switch.h>
#include <rsd/rsd_sys.h>
#else
#include <sys/mman.h>
#include <libcfg/cfg.h>
#endif
sys_l2_dbHead_t   *g_l2Db = NULL;
sys_l2_hashHead_t *g_l2DbHash = NULL;
sys_l2_dbEntry_t  *g_l2DbEntries = NULL;
sys_l2_dbLink_t   *g_l2DbStatic = NULL;
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
sys_l2_dbLink_t   *g_l2DbBlacklist = NULL;
#endif
#ifdef CONFIG_SYS_L3_ROUTE
sys_l2_dbLink_t   *g_l2DbRoute = NULL;
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
sys_l2_entry_t    *g_l2DbPsecure = NULL;
#endif
#ifdef CONFIG_SYS_FLAPPING_MAC
sys_move_entry_t   *g_l2DbMove = NULL;
#endif
sys_l2_dbSort_t   *g_l2DbSort = NULL;

#ifdef __KERNEL__
static uint32     g_mallocSize = 0;
#else
const sys_text_t text_l2_type[] =
{
    {           "Dynamic"          },
    {           "Suspend"          },
    {              "Management"       },
    {            "Static"           },
  #ifdef CONFIG_SYS_BLACK_HOLE_MAC
    {         "Drop"             },
  #endif
  #ifdef CONFIG_SYS_PROTO_AUTHMGR
    {              "Auth"             },
  #endif
  #ifdef CONFIG_SYS_PORT_SECURITY
    {   "SecureDynamic"    },
    {    "SecureSticky"     },
    {    "SecureConfigured" },
  #endif
    {     ""                 },
    {           "Management"       },
};
#endif
#ifdef __KERNEL__

static int32 l2db_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int32 l2db_release(struct inode *inode, struct file *filp)
{
    return 0;
}
static int32 _l2db_vmem(struct file *filp, struct vm_area_struct *vma)
{
    int32 ret;
    long length = vma->vm_end - vma->vm_start;
    unsigned long start = vma->vm_start;
    char *db_ptr = (char *)g_l2Db;
    unsigned long pfn;

    if (length > g_mallocSize)
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
static int32 l2db_mmap(struct file *filp, struct vm_area_struct *vma)
{
    
    if (vma->vm_pgoff == 0)
    {
        return _l2db_vmem(filp, vma);
    }
    
    return -EIO;
}
static struct file_operations l2db_fops = {
    .open = l2db_open,
    .release = l2db_release,
    .mmap = l2db_mmap,
    .owner = THIS_MODULE,
};
int32 sys_l2_init(void)
{
    sys_switch_devInfo_t *pDevInfo = NULL;
    uint32 i = 0;
    uint32 dbSize = 0;
    uint32 currOffset = 0;
    uint32 hwMaxEntry = 0;

    pDevInfo = sys_dev_info_ret(SYS_DEV_ID_DEFAULT);

    hwMaxEntry = (pDevInfo->capacityInfo.l2_learn_limit_cnt_wo_cam_max);

    dbSize = (sizeof(sys_l2_dbHead_t)
              + (sizeof(sys_l2_hashHead_t) * (hwMaxEntry))
              + (sizeof(sys_l2_dbEntry_t) * (hwMaxEntry))
              + (sizeof(sys_l2_dbLink_t) * (CAPA_L2_STATIC_MAC_NUM))
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
              + (sizeof(sys_l2_dbLink_t) * (CAPA_L2_FILTER_MAC_NUM))
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
              + (sizeof(sys_l2_entry_t) * (CAPA_PSECURE_LIMIT_NUM * SYS_NORMAL_LOGIC_PORT_NUM))
#endif
#ifdef CONFIG_SYS_FLAPPING_MAC
			  + (sizeof(sys_move_entry_t) * (CAPA_L2_MOVE_MAC_NUM ))
#endif
              + (sizeof(sys_l2_dbSort_t) * (hwMaxEntry))
             );

    g_mallocSize = ((dbSize / PAGE_SIZE) + 1) * PAGE_SIZE;

    if (NULL == (g_l2Db = (sys_l2_dbHead_t *)vmalloc(g_mallocSize)))
    {
        SYS_DBG(DBG_L2, "Allocate memory failed");
        return SYS_ERR_FAILED;
    }
    
    for (i = 0; i < g_mallocSize; i+= PAGE_SIZE)
    {
        SetPageReserved(vmalloc_to_page((void *)(((unsigned long)g_l2Db) + i)));
    }
    osal_memset(g_l2Db, 0, g_mallocSize);

    if (register_chrdev(L2DB_MAJOR, L2DB_NAME, &l2db_fops) < 0 )
    {
        SYS_DBG(DBG_L2, "unable to get major device %d", L2DB_MAJOR);
        return SYS_ERR_FAILED;
    }

    g_l2Db->dbSize = g_mallocSize;
    g_l2Db->entryNum.hwTotal = hwMaxEntry;
    g_l2Db->entryNum.hwDisable = pDevInfo->capacityInfo.l2_learn_limit_cnt_disable;

    currOffset += sizeof(sys_l2_dbHead_t);

    g_l2Db->offset.hash = currOffset;
    g_l2DbHash = (sys_l2_hashHead_t*)((void*)g_l2Db + currOffset);
    currOffset += (sizeof(sys_l2_hashHead_t) * (CAPA_L2_MAC_HASH_ENTRY_NUM));

    g_l2Db->offset.entries = currOffset;
    g_l2DbEntries = (sys_l2_dbEntry_t*)((void*)g_l2Db + currOffset);
    currOffset += (sizeof(sys_l2_dbEntry_t) * (hwMaxEntry));

    g_l2Db->offset.statics = currOffset;
    g_l2DbStatic = (sys_l2_dbLink_t*)((void*)g_l2Db + currOffset);
    currOffset += (sizeof(sys_l2_dbLink_t) * (CAPA_L2_STATIC_MAC_NUM));
    for (i = 0; i < CAPA_L2_STATIC_MAC_NUM; i++)
    {
        g_l2DbStatic[i].dbIdx = L2_DB_ENTRY_EMPTY;
    }

#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    g_l2Db->offset.blacklist = currOffset;
    g_l2DbBlacklist = (sys_l2_dbLink_t*)((void*)g_l2Db + currOffset);
    currOffset += (sizeof(sys_l2_dbLink_t) * (CAPA_L2_FILTER_MAC_NUM));
    for (i = 0; i < CAPA_L2_FILTER_MAC_NUM; i++)
    {
        g_l2DbBlacklist[i].dbIdx = L2_DB_ENTRY_EMPTY;
    }
#endif

#ifdef CONFIG_SYS_L3_ROUTE
    g_l2Db->offset.route = currOffset;
    g_l2DbRoute = (sys_l2_dbLink_t*)((void*)g_l2Db + currOffset);
    currOffset += (sizeof(sys_l2_dbLink_t) * (CAPA_L3_HOST_ROUTE_ENTRY_NUM));
    for (i = 0; i < CAPA_L3_HOST_ROUTE_ENTRY_NUM; i++)
    {
        g_l2DbRoute[i].dbIdx = L2_DB_ENTRY_EMPTY;
    }
#endif

#ifdef CONFIG_SYS_PORT_SECURITY
    g_l2Db->offset.psecure = currOffset;
    g_l2DbPsecure = (sys_l2_entry_t*)((void*)g_l2Db + currOffset);
    currOffset += (sizeof(sys_l2_entry_t) * (CAPA_PSECURE_LIMIT_NUM * SYS_NORMAL_LOGIC_PORT_NUM));
#endif
#ifdef CONFIG_SYS_FLAPPING_MAC
	g_l2Db->offset.move = currOffset;
	g_l2DbMove = (sys_move_entry_t*)((void*)g_l2Db + currOffset);
	currOffset += (sizeof(sys_move_entry_t) * (CAPA_L2_MOVE_MAC_NUM));
#endif

    g_l2Db->offset.sort = currOffset;
    g_l2DbSort = (sys_l2_dbSort_t*)((void*)g_l2Db + currOffset);
    currOffset += (sizeof(sys_l2_dbSort_t) * (hwMaxEntry));

    g_l2Db->swDbMode = DFLT_L2_SOFTWARE_DATABSE_MODE;
    if (ENABLED == g_l2Db->swDbMode)
    {
#if defined(CONFIG_SYS_RTL8380)
        if (IS_CHIP_MAPLE())
            g_l2Db->useNotify = DISABLED;
        else
#endif
            g_l2Db->useNotify = ENABLED;
    }
    else
    {
        g_l2Db->useNotify = DISABLED;
    }

    return SYS_ERR_OK;
}
#else 
uint32 sys_l2_dbMem_map(void)
{
    int32 fd = -1;
    int32 len = 0;
    int32 dbSize = 0;
    int32 ret = -1;

    USER_LOCK(SYS_USER_LOCK_L2DB);

    if (NULL != g_l2Db)
        goto exit;

    if ((fd = open(L2DB_DEV, O_RDONLY)) < 0)
        goto fail_exit;

    len = getpagesize();
    g_l2Db = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == g_l2Db)
        goto fail;

    osal_memcpy(&dbSize, g_l2Db, sizeof(int32));

    if (dbSize > len)
    {
        ret = munmap(g_l2Db, len);
        if (ret < 0)
            goto fail;

        len = dbSize;
        g_l2Db = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
        if (MAP_FAILED == g_l2Db)
            goto fail;
    }

    g_l2DbHash = (sys_l2_hashHead_t*)((void*)g_l2Db + g_l2Db->offset.hash);
    g_l2DbEntries = (sys_l2_dbEntry_t*)((void*)g_l2Db + g_l2Db->offset.entries);
    g_l2DbStatic = (sys_l2_dbLink_t*)((void*)g_l2Db + g_l2Db->offset.statics);
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    g_l2DbBlacklist = (sys_l2_dbLink_t*)((void*)g_l2Db + g_l2Db->offset.blacklist);
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    g_l2DbRoute = (sys_l2_dbLink_t*)((void*)g_l2Db + g_l2Db->offset.route);
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
    g_l2DbPsecure = (sys_l2_entry_t*)((void*)g_l2Db + g_l2Db->offset.psecure);
#endif
#ifdef CONFIG_SYS_FLAPPING_MAC
	g_l2DbMove = (sys_move_entry_t*)((void*)g_l2Db + g_l2Db->offset.move);
#endif
    g_l2DbSort = (sys_l2_dbSort_t*)((void*)g_l2Db + g_l2Db->offset.sort);

    close(fd);

exit:
    USER_UNLOCK(SYS_USER_LOCK_L2DB);

    return TRUE;

fail:
    close(fd);

fail_exit:
    USER_UNLOCK(SYS_USER_LOCK_L2DB);

    return FALSE;
}
#endif
uint32 sys_l2_maxEntryNum_ret(void)
{
    if (!L2_DB_MAP())
        return 0;

    return (g_l2Db->entryNum.hwTotal);
}
uint32 sys_l2_disableLearningNum_ret(void)
{
    if (!L2_DB_MAP())
        return 0;

    return (g_l2Db->entryNum.hwDisable);
}
int32 sys_l2_dbNum_get(sys_l2_dbNum_t *pNums)
{
    if (!L2_DB_MAP())
        return SYS_ERR_FAILED;

    osal_memcpy(pNums, &g_l2Db->entryNum, sizeof(sys_l2_dbNum_t));

    return SYS_ERR_OK;
}
sys_l2_entry_t *sys_l2_dbLinkNext_ret(sys_l2_dbLink_t *pHead, uint32 maxEntries, sys_l2_entry_t *pPrevEntry)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sys_l2_dbLink_t *pCurr = NULL;
    sys_l2_entry_t *pEntry = NULL;

    for (i = 0; i < maxEntries; i++)
    {
        pCurr = pHead + i;

        if (L2_DB_ENTRY_EMPTY == pCurr->dbIdx)
            continue;

        pEntry = L2_ENTRY_PTR(pCurr->dbIdx);

        if (NULL == pPrevEntry)
            return pEntry;

        if (found)
            return pEntry;

        if (pEntry == pPrevEntry)
            found = TRUE;
    }

    return NULL;
}

static void _sys_l2_qsort_swap(int8 *a, int8 *b, uint32 width)
{
    int8 tmp;

    if (a != b)
    {
        while (width--)
        {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
    }
}

static int32 _sys_l2_qsort_cmp(const void *s1, const void *s2, sys_l2_sortData_t data, sys_l2_sortMeth_t meth)
{
    int32 ret = 0;
    sys_l2_entry_t *e1;
    sys_l2_entry_t *e2;

    if (SYS_L2_SORTDATA_DBENTRY == data)
    {
        sys_l2_dbEntry_t *d1;
        sys_l2_dbEntry_t *d2;

        d1 = L2_DB_ENTRY_PTR(((sys_l2_dbSort_t*)s1)->entryIdx);
        d2 = L2_DB_ENTRY_PTR(((sys_l2_dbSort_t*)s2)->entryIdx);

        e1 = &d1->l2Entry;
        e2 = &d2->l2Entry;
    }
    else
    {
        e1 = (sys_l2_entry_t *)s1;
        e2 = (sys_l2_entry_t *)s2;
    }

    if (SYS_L2_SORTMETH_MACVLAN == meth)
    {
        ret = osal_memcmp(&e1->mac, &e2->mac, sizeof(sys_mac_t));

        if (0 == ret)
        {
            if (e1->vid == e2->vid)
            {
                return 0;
            }
            else if (e1->vid > e2->vid)
            {
                return 1;
            }
            else if (e1->vid < e2->vid)
            {
                return -1;
            }
        }
    }
    else if (SYS_L2_SORTMETH_VLANMAC == meth)
    {
        if (e1->vid == e2->vid)
        {
            ret = osal_memcmp(&e1->mac, &e2->mac, sizeof(sys_mac_t));

            return ret;
        }
        else if (e1->vid > e2->vid)
        {
            return 1;
        }
        else if (e1->vid < e2->vid)
        {
            return -1;
        }
    }

    return ret;
}

void sys_l2_qsort(void *base, uint32 num, uint32 width, sys_l2_sortData_t data, sys_l2_sortMeth_t meth)
{
#define CUTOFF 8

    int8 *lo;
    int8 *hi;
    int8 *mid;
    int8 *loptr;
    int8 *hiptr;
    uint32 size;
    int8 *lostk[30];
    int8 *histk[30];
    int32 stkptr;
    int32 (*comp)(const void *, const void *, sys_l2_sortData_t, sys_l2_sortMeth_t) = _sys_l2_qsort_cmp;

    if (num < 2 || 0 == width)
        return;

    stkptr = 0;

    lo = base;
    hi = (int8 *) base + width * (num - 1);

    recurse:
    size = (hi - lo) / width + 1;

    if (size <= CUTOFF)
    {
        int8 *p;
        int8 *max;

        while (hi > lo)
        {
            max = lo;

            for (p = lo+width; p <= hi; p += width)
            if (comp(p, max, data, meth) > 0)
                max = p;

            _sys_l2_qsort_swap(max, hi, width);
            hi -= width;
        }
    }
    else
    {
        mid = lo + (size / 2) * width;
        _sys_l2_qsort_swap(mid, lo, width);

        loptr = lo;
        hiptr = hi + width;

        for (;;)
        {
            do { loptr += width; }
                while (loptr <= hi && comp(loptr, lo, data, meth) <= 0);

            do { hiptr -= width; }
                while (hiptr > lo && comp(hiptr, lo, data, meth) >= 0);

            if (hiptr < loptr)
                break;

            _sys_l2_qsort_swap(loptr, hiptr, width);
        }

        _sys_l2_qsort_swap(lo, hiptr, width);

        if (hiptr - 1 - lo >= hi - loptr)
        {
            if (lo + width < hiptr)
            {
                lostk[stkptr] = lo;
                histk[stkptr] = hiptr - width;
                ++stkptr;
            }

            if (loptr < hi)
            {
                lo = loptr;
                goto recurse;
            }
        }
        else
        {
            if (loptr < hi)
            {
                lostk[stkptr] = loptr;
                histk[stkptr] = hi;
                ++stkptr;
            }

            if (lo + width < hiptr)
            {
                hi = hiptr - width;
                goto recurse;
            }
        }
    }

    --stkptr;

    if (stkptr >= 0)
    {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;
    }
    else
        return;
}

