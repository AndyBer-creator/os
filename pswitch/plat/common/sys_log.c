#include <common/sys_def.h>
#include <common/sys_log.h>
#include <common/sys_log_desc.h>

#ifdef __KERNEL__
#include <rsd/rsd_sys.h>
#else
#include <libsal/sal_sys.h>
#include <ski/chip/ski_log.h>
#include <common/glue_util.h>
#include <libcfg/cfg_syslog.h>

#ifdef CONFIG_SYS_KERNEL_LINUX
#include <syslog.h>
#endif
#endif
extern const sys_log_desc_t log_cat_desc[LOG_CAT_ALL];

static sys_status_t sysStatus = SYS_STATUS_INITIAL;
static int32 _sys_log(uint32 sev, uint32 cat, char *mnem, char *msg)
{
    uint32 n = 0;
    char *mnemonic = NULL;
#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
    uint32 pri = (1 << 3) + sev;
#endif

    if (mnem && mnem[0] != '\0')
    {
        while (isupper(mnem[n]) || isdigit(mnem[n]) || mnem[n] == '_')
            n++;

        if (mnem[n] == '\0')
            mnemonic = mnem;
    }

#ifdef CONFIG_SYS_SYSLOG
#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
    if (mnemonic)
    {
        syslog(pri, "%s%%%s-%u-%s: %s\n", SYS_LOG_RTK_KEY, log_cat_desc[cat].text, sev, mnemonic, msg);
    }
    else
    {
        syslog(pri, "%s%%%s-%u: %s\n", SYS_LOG_RTK_KEY, log_cat_desc[cat].text, sev, msg);
    }
#else
    
    if (mnemonic)
    {
        SYS_PRINTF(KERN_INFO "%s<%u>%s%%%s-%u-%s: %s\n",
                   SYS_LOG_KERN_KEY, sev, SYS_LOG_RTK_KEY,
                   log_cat_desc[cat].text, sev, mnemonic, msg);
    }
    else
    {
        SYS_PRINTF(KERN_INFO "%s<%u>%s%%%s-%u: %s\n",
                   SYS_LOG_KERN_KEY, sev, SYS_LOG_RTK_KEY,
                   log_cat_desc[cat].text, sev, msg);
    }
#endif
#endif

    return SYS_ERR_OK;
}
int32 sys_log(uint32 cat, uint32 mid, ...)
{
    int32 r;
    va_list args;

    va_start(args, mid);
    r = sys_log2(cat, mid, args);
    va_end(args);

    return r;
}
int32 sys_log2(uint32 cat, uint32 mid, va_list args)
{
    int32 n = 0;
    char msg[SYS_LOG_BUF_SIZE];

    sys_log_desc_t *pLog = NULL;
    log_msg_t *pMsg = NULL;

    SYS_PARAM_CHK(LOG_CAT_ALL <= cat, SYS_ERR_INPUT);

    pLog = (sys_log_desc_t *)&log_cat_desc[cat];

    if (0 == pLog->msgnum)
        return SYS_ERR_OK;

    if (pLog->msgnum <= mid)
        return SYS_ERR_OUT_OF_RANGE;

    pMsg = (log_msg_t *)&pLog->msgtbl[mid];

    if (LOG_SEV_ALL <= pMsg->sev || NULL == pMsg->msg)
        return SYS_ERR_FAILED;

    n = vsnprintf(msg, sizeof(msg), pMsg->msg, args);

    if (n < 0)
        return SYS_ERR_FAILED;

    if (SYS_STATUS_INITIAL == sysStatus)
    {
#ifdef __KERNEL__
        SYS_ERR_CHK(rsd_sys_status_get(&sysStatus));
#else
        SYS_ERR_CHK(sal_sys_status_get(&sysStatus));
#endif
    }

    if (SYS_STATUS_INITIAL != sysStatus)
    {
        _sys_log(pMsg->sev, cat, pMsg->mnem, msg);
    }

    return SYS_ERR_OK;
}
int32 sys_log_dbg_set(char *fmt, ...)
{
    char msg[SYS_LOG_BUF_SIZE];
    va_list args;

    SYS_PARAM_CHK((NULL == fmt), SYS_ERR_NULL_POINTER);

    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    if (SYS_STATUS_INITIAL == sysStatus)
    {
#ifdef __KERNEL__
        SYS_ERR_CHK(rsd_sys_status_get(&sysStatus));
#else
        SYS_ERR_CHK(sal_sys_status_get(&sysStatus));
#endif
    }

    if (SYS_STATUS_INITIAL == sysStatus)
    {
        BOOTMSG_PRINTF("%s\n", msg);
    }
    else
    {
#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
        syslog((1 << 3) + LOG_SEV_DEBUG, "%s!%s\n", SYS_LOG_RTK_KEY, msg);
#else
        SYS_PRINTF(KERN_INFO "%s<%u>%s!%s\n",
                   SYS_LOG_KERN_KEY, LOG_SEV_DEBUG, SYS_LOG_RTK_KEY, msg);
#endif
    }

    return SYS_ERR_OK;
}
int32 sys_log_errStr_set(int32 errCode, char *pStr)
{
    int32 n = 0;
    uint32 cat;
    char msg[SYS_LOG_BUF_SIZE];

    SYS_PARAM_CHK(IS_SYS_ERR_INVALID(errCode), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);

    switch (errCode)
    {
        case SYS_ERR_COMMON_START ... SYS_ERR_COMMON_END:
            cat = LOG_CAT_SYSTEM;
            break;

        case SYS_ERR_PORT_START ... SYS_ERR_PORT_END:
            cat = LOG_CAT_PORT;
            break;

        case SYS_ERR_TRUNK_START ... SYS_ERR_TRUNK_END:
            cat = LOG_CAT_LAG;
            break;

        case SYS_ERR_L2_START ... SYS_ERR_L2_END:
            cat = LOG_CAT_L2;
            break;

#ifdef CONFIG_SYS_L3_ROUTE
        case SYS_ERR_L3_START ... SYS_ERR_L3_END:
            cat = LOG_CAT_L3;
            break;
#endif

        case SYS_ERR_VLAN_START ... SYS_ERR_VLAN_END:
            cat = LOG_CAT_VLAN;
            break;

        case SYS_ERR_RATE_START ... SYS_ERR_RATE_END:
            cat = LOG_CAT_STORM_CONTROL;
            break;

        case SYS_ERR_QOS_START ... SYS_ERR_QOS_END:
            cat = LOG_CAT_QOSMGR;
            break;

        default:
            cat = LOG_CAT_SYSTEM;
            break;
    }

    if (osal_strlen(pStr))
        n = snprintf(msg, sizeof(msg), "[%s] %s", pStr, SYS_ERR_STR(errCode));
    else
        n = snprintf(msg, sizeof(msg), "%s", SYS_ERR_STR(errCode));

    if (n < 0)
        return SYS_ERR_FAILED;

    msg[n] = 0;

    if (SYS_STATUS_INITIAL == sysStatus)
    {
#ifdef __KERNEL__
        SYS_ERR_CHK(rsd_sys_status_get(&sysStatus));
#else
        SYS_ERR_CHK(sal_sys_status_get(&sysStatus));
#endif
    }

    if (SYS_STATUS_INITIAL == sysStatus)
    {
        BOOTMSG_PRINTF("%s\n", msg);
    }
    else
    {
#ifdef CONFIG_SYS_SYSLOG
        _sys_log(LOG_SEV_ERR, cat, NULL, msg);
#else
        SYS_PRINTF("%s\n", msg);
#endif
    }

    return SYS_ERR_OK;
}
int32 sys_log_restart_set(void)
{
#if (defined(CONFIG_SYS_KERNEL_LINUX) && !defined(__KERNEL__))
    ski_log_mask_t data;
    glue_util_ioctl(SKI_LOG_SET_RESTART, &data);
    syslog(LOG_DEBUG, SYS_LOG_RTK_KEY SYS_LOG_JUNK_MSG);
#else
    SYS_PRINTF(KERN_INFO SYS_LOG_KERN_KEY SYS_LOG_RTK_KEY SYS_LOG_JUNK_MSG"\n");
#endif

    return SYS_ERR_OK;
}
