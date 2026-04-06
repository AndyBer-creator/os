#include <common/sys_def.h>
#include <common/sys_log.h>
#include <common/sys_dbg.h>
#include <common/sys_dbg_desc.h>

#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
#include <syslog.h>
#include <ski/chip/ski_log.h>
#include <common/glue_util.h>
#include <libcfg/cfg_syslog.h>
#endif
#if (defined(CONFIG_SYS_KERNEL_LINUX) && defined(__KERNEL__))
static struct
{
    sys_enable_t        enable;
    sys_dbg_extmask_t   extmask;
} dbg_cat[DBG_CAT_ALL];
#endif

extern const sys_dbg_desc_t dbg_cat_desc[DBG_CAT_ALL];
int32 sys_dbg(uint32 dbg, const char *func, const int line, char *fmt, ...)
{
    char msg[SYS_LOG_BUF_SIZE];
    va_list args;

    sys_enable_t enable = DISABLED;
    sys_dbg_extmask_t extmask;

    int32 cat = (dbg < DBG_CAT_ALL) ? dbg : (dbg - DBG_CAT_ALL) / DBG_EXT_TOTAL;
    int32 ext = (dbg < DBG_CAT_ALL) ? -1 : (dbg - DBG_CAT_ALL) % DBG_EXT_TOTAL;

    SYS_PARAM_CHK(DBG_CAT_ALL <= cat, SYS_ERR_INPUT);
    SYS_PARAM_CHK(DBG_EXT_MAX <= ext, SYS_ERR_INPUT);

    if (-1 != ext && dbg_cat_desc[cat].extnum <= ext)
        return SYS_ERR_OK;

    SYS_MEM_CLEAR(extmask);

#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
    cfg_log_dbgCatMask_get(cat, &enable, &extmask);
#else
    enable = dbg_cat[cat].enable;
    extmask = dbg_cat[cat].extmask;
#endif

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (-1 != ext && SYS_BITMAP_IS_BITCLEAR(extmask.bits, ext))
        return SYS_ERR_OK;

    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    sys_log_dbg_set("%s(%u): %s", func, line, msg);

    return SYS_ERR_OK;
}
int32 sys_dbg_isSet_ret(uint32 dbg)
{
    sys_enable_t enable;
    sys_dbg_extmask_t extmask;

    int32 cat = (dbg < DBG_CAT_ALL) ? dbg : (dbg - DBG_CAT_ALL) / DBG_EXT_TOTAL;
    int32 ext = (dbg < DBG_CAT_ALL) ? -1 : (dbg - DBG_CAT_ALL) % DBG_EXT_TOTAL;

    SYS_PARAM_CHK(DBG_CAT_ALL <= cat, SYS_ERR_INPUT);
    SYS_PARAM_CHK(DBG_EXT_MAX <= ext, SYS_ERR_INPUT);

    if (-1 != ext && dbg_cat_desc[cat].extnum <= ext)
        return FALSE;

#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
    cfg_log_dbgCatMask_get(cat, &enable, &extmask);
#else
    enable = dbg_cat[cat].enable;
    extmask = dbg_cat[cat].extmask;
#endif

    if (DISABLED == enable)
        return FALSE;

    if (-1 != ext && SYS_BITMAP_IS_BITCLEAR(extmask.bits, ext))
        return FALSE;

    return TRUE;
}
int32 sys_dbg_pkt_dump(uint32 dbg, const char *func, const int line, char *pktData, uint32 pktLen)
{
    uint32 i = 0;
    uint32 j = 0;
    char msg[SYS_LOG_BUF_SIZE];

    sys_enable_t enable;
    sys_dbg_extmask_t extmask;

    int32 cat = (dbg < DBG_CAT_ALL) ? dbg : (dbg - DBG_CAT_ALL) / DBG_EXT_TOTAL;
    int32 ext = (dbg < DBG_CAT_ALL) ? -1 : (dbg - DBG_CAT_ALL) % DBG_EXT_TOTAL;

    if (-1 != ext && dbg_cat_desc[cat].extnum <= ext)
        return SYS_ERR_OK;

#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
    cfg_log_dbgCatMask_get(cat, &enable, &extmask);
#else
    enable = dbg_cat[cat].enable;
    extmask = dbg_cat[cat].extmask;
#endif

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (-1 != ext && SYS_BITMAP_IS_BITCLEAR(extmask.bits, ext))
        return SYS_ERR_OK;

    sys_log_dbg_set("%s(%u): ======== Packet Len: %d =====", func, line, pktLen);

    for (i = 0, j = 0; i < pktLen; i++)
    {
        if (0 == (i % 16))
            j = snprintf(msg, sizeof(msg), "[%0x4] ", i);

        j+= snprintf(msg + j, sizeof(msg) - j, "%02X ", *(uint8 *)(pktData + i));

        if (15 == (i % 16))
            sys_log_dbg_set("%s", msg);
    }

    sys_log_dbg_set("===============================================");

    return SYS_ERR_OK;
}
int32 sys_dbg_enable_set(uint32 dbg, sys_enable_t enable)
{
    int32 cat = (dbg < DBG_CAT_ALL) ? dbg : (dbg - DBG_CAT_ALL) / DBG_EXT_TOTAL;
    int32 ext = (dbg < DBG_CAT_ALL) ? -1 : (dbg - DBG_CAT_ALL) % DBG_EXT_TOTAL;

    int32 empty = 0;

    sys_enable_t active;
    sys_dbg_extmask_t extmask;

#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
#if 0
    int32 i = 0;
    int32 child = 0;
#endif

    ski_log_mask_t data;
#endif

    SYS_PARAM_CHK(DBG_CAT_ALL <= cat, SYS_ERR_INPUT);
    SYS_PARAM_CHK(DBG_EXT_MAX < ext, SYS_ERR_INPUT);

    if (-1 != ext && DBG_EXT_MAX != ext
        && dbg_cat_desc[cat].extnum < ext)
    {
        return SYS_ERR_INPUT;
    }

#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
    data.dbg.dbg = dbg;
    data.dbg.enable = enable;

    SYS_ERR_CHK(glue_util_ioctl(SKI_LOG_SET_DBG_ENABLE, &data));

    SYS_ERR_CHK(cfg_log_dbgCatMask_get(cat, &active, &extmask));
#else
    active = dbg_cat[cat].enable;
    extmask = dbg_cat[cat].extmask;
#endif

    if (ENABLED == enable)
    {
        active = ENABLED;

        if (dbg_cat_desc[cat].extnum)
        {
            if (dbg_cat_desc[cat].extnum == ext || DBG_EXT_MAX == ext)
            {
                SYS_BITMAP_SET_ALL(extmask.bits, dbg_cat_desc[cat].extnum);

#if 0
#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
                if (dbg_cat_desc[cat].children)
                {
                    while (-1 != (child = dbg_cat_desc[cat].children[i++]))
                    {
                        if (0 < child && DBG_CAT_ALL > child)
                            sys_dbg_enable_set(DBG_EXT_ALL(child), ENABLED);
                    }
                }
#endif
#endif
            }
            else if (-1 != ext)
            {
                SYS_BITMAP_SET_BIT(extmask.bits, ext);
            }
        }
    }
   else
    {
        if (-1 == ext)
        {
            active = DISABLED;
        }
        else
        {
            if (dbg_cat_desc[cat].extnum)
            {
                if (dbg_cat_desc[cat].extnum == ext || DBG_EXT_MAX == ext)
                {
                    SYS_BITMAP_CLEAR_ALL(extmask.bits, dbg_cat_desc[cat].extnum);

#if 0
#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
                    if (dbg_cat_desc[cat].children)
                    {
                        while (-1 != (child = dbg_cat_desc[cat].children[i++]))
                        {
                            if (0 < child && DBG_CAT_ALL > child)
                                sys_dbg_enable_set(DBG_EXT_ALL(child), DISABLED);
                        }
                    }
#endif
#endif
                }
                else
                {
                    SYS_BITMAP_CLEAR_BIT(extmask.bits, ext);
                }
            }

            SYS_BITMAP_IS_CLEAR(extmask.bits, dbg_cat_desc[cat].extnum, empty);

            if (empty)
                active = DISABLED;
        }
    }

#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
    cfg_log_dbgCatMask_set(cat, active, extmask);
#else
    dbg_cat[cat].enable = active;
    dbg_cat[cat].extmask = extmask;
#endif

    return SYS_ERR_OK;
}
int32 sys_dbg_enable_get(uint32 dbg, sys_enable_t *pEnable)
{
    int32 cat = (dbg < DBG_CAT_ALL) ? dbg : (dbg - DBG_CAT_ALL) / DBG_EXT_TOTAL;
    int32 ext = (dbg < DBG_CAT_ALL) ? -1 : (dbg - DBG_CAT_ALL) % DBG_EXT_TOTAL;

    sys_enable_t active;
    sys_dbg_extmask_t extmask;

    SYS_PARAM_CHK(DBG_CAT_ALL <= cat, SYS_ERR_INPUT);
    SYS_PARAM_CHK(DBG_EXT_MAX <= ext, SYS_ERR_INPUT);

    if (-1 != ext && dbg_cat_desc[cat].extnum <= ext)
    {
        *pEnable = DISABLED;
        return SYS_ERR_OK;
    }

#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
    cfg_log_dbgCatMask_get(cat, &active, &extmask);
#else
    active = dbg_cat[cat].enable;
    extmask = dbg_cat[cat].extmask;
#endif

    *pEnable = DISABLED;

    if (-1 == ext)
        *pEnable = active;

    if (SYS_BITMAP_IS_BITSET(extmask.bits, ext))
        *pEnable = ENABLED;

    return SYS_ERR_OK;
}

#ifdef __KERNEL__

#else

int32 sys_dbg_init(void)
{
    int32 cat = 0;
    ski_log_mask_t data;
    sys_dbg_extmask_t extmask;

    for (cat = 0 ; cat < DBG_CAT_ALL ; cat++)
    {
        data.dbg.dbg = cat;

        SYS_ERR_CONTINUE(glue_util_ioctl(SKI_LOG_GET_DBG_ENABLE, &data));

        if (ENABLED == data.dbg.enable)
            SYS_BITMAP_SET_ALL(extmask.bits, dbg_cat_desc[cat].extnum);
        else
            SYS_BITMAP_CLEAR_ALL(extmask.bits, dbg_cat_desc[cat].extnum);

        SYS_ERR_CONTINUE(cfg_log_dbgCatMask_set(cat, data.dbg.enable, extmask))
    }

    return SYS_ERR_OK;
}
int32 sys_dbg_reset(void)
{
    int32 cat = 0;

    for (cat = 0 ; cat < DBG_CAT_ALL ; cat++)
    {
        SYS_ERR_CONTINUE(sys_dbg_enable_set(DBG_EXT_ALL(cat), DISABLED));
    }

    return SYS_ERR_OK;
}

#endif
