
#include <include/cgi.h>
#include <libsal/sal_psecure.h>

static int32 cgi_psecure_set(void);
static int32 cgi_psecure_portEdit_set(void);
static int32 cgi_psecure_macAdd_set(void);
static int32 cgi_psecure_macEdit_set(void);
static int32 cgi_psecure_macDel_set(void);

const cgi_cmdmap_t cmdSetPsecureTbl[] =
{
    {CGI_CMD_PSECURE_PROP,             cgi_psecure_set},
    {CGI_CMD_PSECURE_PORTEDIT,         cgi_psecure_portEdit_set},
    {CGI_CMD_PSECURE_MACADD,           cgi_psecure_macAdd_set},
    {CGI_CMD_PSECURE_MACEDIT,          cgi_psecure_macEdit_set},
    {CGI_CMD_PSECURE_MACDEL,           cgi_psecure_macDel_set},
    {NULL, NULL}
};

static int32 cgi_psecure_set(void)
{
    int32 ret = SYS_ERR_OK;
    sys_enable_t enable = DISABLED;
    uint32 rateLimit = 0;
    char *value = NULL;

    if (CGI_IS_PARAM_EXIST(CGI_PSECURE_STATE, value))
        enable = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_PSECURE_RATELIMIT, value))
        rateLimit = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_psecure_enable_set(enable), ret);

    CGI_SET_ERR_HDL(sal_psecure_rateLimit_set(rateLimit), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

int32 cgi_psecure_portEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    char *value = NULL;
    sys_psecure_port_t portCfg;

    SYS_MEM_CLEAR(lpm);
    SYS_MEM_CLEAR(portCfg);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PSECURE_MAXNUMADDR, value))
        portCfg.limitNum = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_PSECURE_VIOLATE, value))
        portCfg.violation = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_PSECURE_STATE, value))
        portCfg.enable = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_PSECURE_STICKY, value))
        portCfg.sticky = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_psecure_portEnable_set(lp, portCfg.enable), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_psecure_portLimitNum_set(lp, portCfg.limitNum), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_psecure_portViolation_set(lp, portCfg.violation), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_psecure_portSticky_set(lp, portCfg.sticky), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_psecure_macAdd_set(void)
{
    int32   ret = SYS_ERR_OK;
    char *pStrValue;
    sys_l2_entry_t entry;

    SYS_MEM_CLEAR(entry);

    if (CGI_IS_PARAM_EXIST(CGI_MAC_PORT, pStrValue))
    {
        STR2LP(pStrValue, &entry.port);
    }

    if (CGI_IS_PARAM_EXIST(CGI_MAC_ADDR, pStrValue))
    {
        if (sys_util_str2Mac(pStrValue, entry.mac.octet) != 0)
        {
            return SYS_ERR_INPUT;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_MAC_VID, pStrValue))
    {
        entry.vid = SYS_STR2UINT(pStrValue);
    }

    entry.type = SYS_L2_TYPE_PSECURE_STATIC;
    CGI_SET_ERR_HDL(sal_psecure_mac_add(&entry), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_psecure_macEdit_set(void)
{
    int32   ret = SYS_ERR_OK;
    char *pStrValue;
    sys_l2_entry_t oldEntry;
    sys_l2_entry_t newEntry;

    SYS_MEM_CLEAR(oldEntry);
    SYS_MEM_CLEAR(newEntry);

    if (CGI_IS_PARAM_EXIST(CGI_MAC_SELECTENTRYPORT, pStrValue))
    {
        STR2LP(pStrValue, &oldEntry.port);
    }

    if (CGI_IS_PARAM_EXIST(CGI_MAC_SELECTENTRYADDR, pStrValue))
    {
        if (sys_util_str2Mac(pStrValue, oldEntry.mac.octet) != 0)
        {
            return SYS_ERR_INPUT;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_MAC_SELECTENTRYVID, pStrValue))
    {
        oldEntry.vid = SYS_STR2UINT(pStrValue);
    }

    osal_memcpy(&newEntry.mac, &oldEntry.mac, CAPA_MAC_BYTE_NUM);

    if (CGI_IS_PARAM_EXIST(CGI_MAC_PORT, pStrValue))
    {
        STR2LP(pStrValue, &newEntry.port);
    }

    if (CGI_IS_PARAM_EXIST(CGI_MAC_VID, pStrValue))
    {
        newEntry.vid = SYS_STR2UINT(pStrValue);
    }

    if (!sal_psecure_macExist_ret(&oldEntry))
    {
        ret = SYS_ERR_L2_MAC_NOT_EXIST;
        goto cgi_end;
    }

    if (SYS_L2_TYPE_PSECURE_STATIC != oldEntry.type)
    {
        ret = SYS_ERR_PSECURE_TYPE_INVALID;
        goto cgi_end;
    }

    newEntry.type = oldEntry.type;

    if (0 != osal_memcmp(&oldEntry,&newEntry,sizeof(oldEntry)))
    {
        CGI_SET_ERR_HDL(sal_psecure_mac_add(&newEntry), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_psecure_macDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i=0;
    uint32 len=0;
    char **array=NULL;
    char *flag = "_";
    char *value =  NULL;
    sys_psecure_clearCfg_t clearCfg;

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array ,&len))
    {
        for(i=0; i<len; i++)
        {
            SYS_MEM_CLEAR(clearCfg);

            value = osal_strtok(array[i], flag);
            clearCfg.entry.vid = SYS_STR2UINT(value);

            value = osal_strtok(NULL, flag);
            sys_util_str2Mac(value, clearCfg.entry.mac.octet);

            value = osal_strtok(NULL, flag);
            STR2LP(value, &clearCfg.entry.port);

            clearCfg.type = SYS_PSECURE_CLEAR_ALL_BY_MAC;

            CGI_SET_ENTRY_ERR_CONTINUE(sal_psecure_mac_clear(&clearCfg), "Delete");
        }
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

