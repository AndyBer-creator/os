
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/vmalloc.h>

#include <defdb/defdb.h>

#undef DUMP_DB

static char *db = NULL;
static char *map = NULL;

static uint32 g_initCurrDataIdx = 0;
static uint32 g_totalSize = 0;
static uint32 g_mallocSize = 0;

extern uint32 defdb_data_init(uint32 dbAct);

void defdb_data_add(def_type_t type, def_t idx, int32 i, uint32 u, char *s)
{
    defdb_tok_t *pTok = NULL;

    pTok = (defdb_tok_t *) (map + (idx * sizeof(defdb_tok_t)));

    pTok->type = type;
    pTok->offset = g_initCurrDataIdx;

    switch (type)
    {
        case DEF_TYPE_INT:
        {
            int32 *pVal = NULL;
            pTok->size = sizeof(int32);
            pVal = (int32 *) (db + pTok->offset);
            osal_memcpy(pVal, &i, pTok->size);
            *pVal = i;
            break;
        }

        case DEF_TYPE_UINT:
        {
            uint32 *pVal = NULL;
            pTok->size = sizeof(uint32);
            pVal = (uint32 *) (db + pTok->offset);
            osal_memcpy(pVal, &u, pTok->size);
            break;
        }

        case DEF_TYPE_STR:
        {
            char *pVal = NULL;
            pTok->size = strlen(s) + 1;
            pVal = (char *) (db + pTok->offset);
            osal_memcpy(pVal, s, pTok->size);
            break;
        }

        default:
            break;
    }

    g_initCurrDataIdx += pTok->size;

    return;
}

uint32 defdb_val_ret(def_t key)
{
    defdb_tok_t *pTok = NULL;
    uint32 *pVal = NULL;

    pTok = (defdb_tok_t *) (map + (key * sizeof(defdb_tok_t)));

    if (DEF_TYPE_INT == pTok->type || DEF_TYPE_UINT == pTok->type)
    {
        pVal = (uint32 *) (db + pTok->offset);

        return *pVal;
    }

    return 0;
}

char *defdb_str_ret(def_t key)
{
    defdb_tok_t *pTok = NULL;

    pTok = (defdb_tok_t *) (map + (key * sizeof(defdb_tok_t)));

    if (DEF_TYPE_STR == pTok->type)
    {
        return ((char *) (db + pTok->offset));
    }

    return "";
}

#ifdef DUMP_DB
static void db_dump(void)
{
    int32 i = 0;
    uint32 *pSize = (uint32 *)db;
    defdb_tok_t *pTok = NULL;

    SYS_PRINTF("Size: %d/%d bytes\n\n", g_totalSize, *pSize);

    SYS_PRINTF("IDX TYPE SIZE    OFFSET  DATA\n");
    for (i = 0; i < DEF_END; i++)
    {
        pTok = (defdb_tok_t *) (map + (i * sizeof(defdb_tok_t)));

        if (0 == pTok->size)
            continue;

        SYS_PRINTF("%03d ", i);

        switch (pTok->type)
        {
            case DEF_TYPE_INT:
                SYS_PRINTF("INT  ");
                break;

            case DEF_TYPE_UINT:
                SYS_PRINTF("UINT ");
                break;

            case DEF_TYPE_STR:
                SYS_PRINTF("STR  ");
                break;

            default:
                break;
        }

        SYS_PRINTF("%-7d ", pTok->size);

        SYS_PRINTF("%-7d ", pTok->offset);

        switch (pTok->type)
        {
            case DEF_TYPE_INT:
            {
                int32 *pVal = NULL;
                pVal = (int32 *) (db + pTok->offset);
                SYS_PRINTF("%d", *pVal);
                break;
            }

            case DEF_TYPE_UINT:
            {
                uint32 *pVal = NULL;
                pVal = (uint32 *) (db + pTok->offset);
                SYS_PRINTF("%u", *pVal);
                break;
            }

            case DEF_TYPE_STR:
            {
                char *pVal = NULL;
                pVal = (char *) (db + pTok->offset);
                SYS_PRINTF("%s", pVal);
                break;
            }

            default:
                break;
        }

        SYS_PRINTF("\n");
    }

    return;
}
#endif

static int32 defdb_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int32 defdb_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static int32 _defdb_vmem(struct file *filp, struct vm_area_struct *vma)
{
    int32 ret;
    long length = vma->vm_end - vma->vm_start;
    unsigned long start = vma->vm_start;
    char *db_ptr = (char *)db;
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

static int32 defdb_mmap(struct file *filp, struct vm_area_struct *vma)
{
    
    if (vma->vm_pgoff == 0)
    {
        return _defdb_vmem(filp, vma);
    }
    
    return -EIO;
}

static struct file_operations defdb_fops = {
        .open = defdb_open,
        .release = defdb_release,
        .mmap = defdb_mmap,
        .owner = THIS_MODULE,
};

static int __init defdb_init(void)
{
    int32 ret = SYS_ERR_FAILED;
    int32 i;
    uint32 *pSize = NULL;
    uint32 mapAreaSize = (sizeof(defdb_tok_t) * DEF_END);

    g_initCurrDataIdx = mapAreaSize + sizeof(uint32);
    g_totalSize = (defdb_data_init(INIT_ACT_CALSIZE) + g_initCurrDataIdx);
    g_mallocSize = ((g_totalSize / PAGE_SIZE) + 1) * PAGE_SIZE;

    if ((db = (char *)vmalloc(g_mallocSize)) == NULL)
    {
        
        goto out;
    }

    for (i = 0; i < g_mallocSize; i+= PAGE_SIZE)
    {
        SetPageReserved(vmalloc_to_page((void *)(((unsigned long)db) + i)));
    }

    osal_memset(db, 0, g_mallocSize);

    pSize = (uint32*)db;
    *pSize = g_mallocSize;

    map = db + sizeof(uint32);

    defdb_data_init(INIT_ACT_CPDATA);

#ifdef DUMP_DB
    db_dump();
#endif

    if (register_chrdev(DEFDB_MAJOR, DEFDB_NAME, &defdb_fops) < 0 )
    {
        printk("unable to get major for defdb module\n");
        
        goto out;
    }

    ret = SYS_ERR_OK;

out:
    BOOTMSG_RET_PRINTF(ret, "Init Define Database Module....%s\n", (ret) ? "FAILED" : "OK");

    return ret;
}

static void __exit defdb_exit(void)
{
    unregister_chrdev(DEFDB_MAJOR, DEFDB_NAME);

#if 0
    
    for (i = 0; i < NPAGES * PAGE_SIZE; i+= PAGE_SIZE) {
            SetPageReserved(vmalloc_to_page((void *)(((unsigned long)db) + i)));
            SetPageReserved(virt_to_page(((unsigned long)kmalloc_area) + i));
    }
    
    vfree(db);
#endif
}

module_init(defdb_init);
module_exit(defdb_exit);

MODULE_DESCRIPTION ("Switch DEFDB Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

EXPORT_SYMBOL(defdb_val_ret);
EXPORT_SYMBOL(defdb_str_ret);

