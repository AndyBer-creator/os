
#include <include/cgi.h>
#include <libsal/sal_l2.h>

static int32 cgi_mac_miscStatus_set(void);
static int32 cgi_mac_dynamicAging_set(void);
static int32 cgi_mac_dynamicToStatic_set(void);
static int32 cgi_mac_staticAdd_set(void);
static int32 cgi_mac_staticEdit_set(void);
static int32 cgi_mac_staticDelete_set(void);
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
static int32 cgi_mac_filterAdd_set(void);
static int32 cgi_mac_filterEdit_set(void);
static int32 cgi_mac_filterDelete_set(void);
#endif

const cgi_cmdmap_t cmdSetMacTbl[] =
{
    {CGI_CMD_MAC_MISCSTATUS,          cgi_mac_miscStatus_set},
    {CGI_CMD_MAC_DYNAMICAGING,        cgi_mac_dynamicAging_set},
    {CGI_CMD_MAC_DYNAMICTOSTATIC,     cgi_mac_dynamicToStatic_set},
    {CGI_CMD_MAC_STATICADD,           cgi_mac_staticAdd_set},
    {CGI_CMD_MAC_STATICEDIT,          cgi_mac_staticEdit_set},
    {CGI_CMD_MAC_STATICDELETE,        cgi_mac_staticDelete_set},
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    {CGI_CMD_MAC_FILTERADD,           cgi_mac_filterAdd_set},
    {CGI_CMD_MAC_FILTEREDIT,          cgi_mac_filterEdit_set},
    {CGI_CMD_MAC_FILTERDELETE,        cgi_mac_filterDelete_set},
#endif
    {NULL, NULL}
};

static int32 cgi_mac_miscStatus_set(void)
{
    int32                   ret = SYS_ERR_OK;
    uint32                  table_size;
    uint32                  i;
    sys_l2_entry_t          *pEntry = NULL;
    sys_mac_t               cpuMac;

    sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &table_size);
    
    sal_sys_macAddr_get(&cpuMac);

    for (i = 0; i < table_size; i++)
    {
        pEntry = L2_SORT_ENTRY_PTR(i);

        if (SYS_L2_TYPE_DYNAMIC != pEntry->type)
            continue;

        if (osal_memcmp(cpuMac.octet, &pEntry->mac, CAPA_MAC_BYTE_NUM) == 0)
            continue;

        CGI_SET_ENTRY_ERR_CONTINUE(sal_l2_dynamicMac_del(pEntry), "Delete");
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;

}

static int32 cgi_mac_dynamicAging_set(void)
{
    int32   ret = SYS_ERR_OK;
    char    *value = NULL;
    uint32  aging_time;

    if (CGI_IS_PARAM_EXIST(CGI_MAC_AGING, value))
    {
        aging_time = SYS_STR2UINT(value);
        CGI_SET_ERR_HDL(sal_l2_aging_set(aging_time), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mac_dynamicToStatic_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i = 0;
    uint32 table_size;
    uint32 len = 0;
    char **array = NULL;
    sys_l2_entry_t          entry;
    uint32 mac0 = 0;
    uint32 mac1 = 0;
    uint32 mac2 = 0;
    uint32 mac3 = 0;
    uint32 mac4 = 0;
    uint32 mac5 = 0;

    sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &table_size);

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_MAC_INDEX, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(entry);

            osal_sscanf(array[i], "%02X%02X%02X%02X%02X%02X_%d_%d",
                        &mac0,
                        &mac1,
                        &mac2,
                        &mac3,
                        &mac4,
                        &mac5,
                        &entry.vid,
                        &entry.port);

            entry.mac.octet[0] = mac0;
            entry.mac.octet[1] = mac1;
            entry.mac.octet[2] = mac2;
            entry.mac.octet[3] = mac3;
            entry.mac.octet[4] = mac4;
            entry.mac.octet[5] = mac5;

            CGI_SET_ENTRY_ERR_CONTINUE(sal_l2_staticMac_add(&entry), "Add");
        }
        free(array);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mac_staticAdd_set(void)
{
    int32   ret = SYS_ERR_OK;
    sys_mac_t cpuMac,zeroMac;
    char *pStrValue;
    sys_l2_entry_t entry;

    SYS_MEM_CLEAR(entry);

    if (CGI_IS_PARAM_EXIST(CGI_MAC_PORT, pStrValue))
    {
        STR2LP(pStrValue, &entry.port);
    }

    if ((IS_LP_TRKMBR(entry.port)) || (IS_TRKLP(entry.port) && IS_TRKLP_EMPTY(entry.port)))
    {
        if(IS_LP_TRKMBR(entry.port))
            CGI_SET_ERR_HDL(SYS_ERR_L2_STATIC_PORT_TRUNKMBR_INVALID, ret);
        else
            CGI_SET_ERR_HDL(SYS_ERR_L2_STATIC_PORT_EMPTYTRUNK_INVALID, ret);

        return SYS_ERR_OK;
    }

    if (CGI_IS_PARAM_EXIST(CGI_MAC_ADDR, pStrValue))
    {
        if (sys_util_str2Mac(pStrValue, entry.mac.octet) != 0)
        {
            return SYS_ERR_INPUT;
        }
        SYS_ERR_CHK(sal_sys_macAddr_get(&cpuMac));

        if(0 == osal_memcmp(&cpuMac.octet, &entry.mac,CAPA_MAC_BYTE_NUM))
        {
            CGI_SET_ERR_HDL(SYS_ERR_L2_STATIC_EQ_SYSMAC, ret);
            return SYS_ERR_OK;
        }

        if(0x1 & entry.mac.octet[0])
        {
            CGI_SET_ERR_HDL(SYS_ERR_L2_STATIC_BE_MCAST, ret);
            return SYS_ERR_OK;
        }

        SYS_MEM_CLEAR(zeroMac);
        if (0 == osal_memcmp(&entry.mac,&zeroMac,sizeof(zeroMac)))
        {
            CGI_SET_ERR_HDL(SYS_ERR_L2_STATIC_AZERO_INVALID, ret);
            return SYS_ERR_OK;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_MAC_VID, pStrValue))
    {
        entry.vid = SYS_STR2UINT(pStrValue);
    }

    CGI_SET_ERR_HDL(sal_l2_staticMac_add(&entry), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mac_staticEdit_set(void)
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

    if ((IS_LP_TRKMBR(newEntry.port)) || (IS_TRKLP(newEntry.port) && IS_TRKLP_EMPTY(newEntry.port)))
    {
        if(IS_LP_TRKMBR(newEntry.port))
            CGI_SET_ERR_HDL(SYS_ERR_L2_STATIC_PORT_TRUNKMBR_INVALID, ret);
        else
            CGI_SET_ERR_HDL(SYS_ERR_L2_STATIC_PORT_EMPTYTRUNK_INVALID, ret);

        return SYS_ERR_OK;
    }

    if (CGI_IS_PARAM_EXIST(CGI_MAC_VID, pStrValue))
    {
        newEntry.vid = SYS_STR2UINT(pStrValue);
    }

    if (0 != osal_memcmp(&oldEntry,&newEntry,sizeof(oldEntry)))
    {
        CGI_SET_ERR_HDL(sal_l2_staticMac_add(&newEntry), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mac_staticDelete_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i=0;
    uint32 len=0;
    char **array=NULL;
    char *flag = "_";
    char *value =  NULL;
    sys_l2_entry_t macEntry;

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array ,&len))
    {
        for(i=0; i<len; i++)
        {
            SYS_MEM_CLEAR(macEntry);

            value = osal_strtok(array[i], flag);
            macEntry.vid = SYS_STR2UINT(value);

            value = osal_strtok(NULL, flag);
            sys_util_str2Mac(value, macEntry.mac.octet);

            value = osal_strtok(NULL, flag);
            STR2LP(value, &macEntry.port);

            CGI_SET_ENTRY_ERR_CONTINUE(sal_l2_staticMac_del(&macEntry), "Delete");
        }
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_BLACK_HOLE_MAC
static int32 cgi_mac_filterAdd_set(void)
{
    int32   ret = SYS_ERR_OK;
    sys_mac_t cpuMac,zeroMac;
    char *pStrValue;
    sys_l2_entry_t entry;

    SYS_MEM_CLEAR(entry);

    if (CGI_IS_PARAM_EXIST(CGI_MAC_ADDR, pStrValue))
    {
        if (sys_util_str2Mac(pStrValue, entry.mac.octet) != 0)
        {
            return SYS_ERR_INPUT;
        }
        SYS_ERR_CHK(sal_sys_macAddr_get(&cpuMac));

        if(0 == osal_memcmp(&cpuMac.octet, &entry.mac, CAPA_MAC_BYTE_NUM))
        {
            CGI_SET_ERR_HDL(SYS_ERR_L2_FILTER_EQ_SYSMAC, ret);
            return SYS_ERR_OK;
        }

        if(0x1 & entry.mac.octet[0])
        {
            CGI_SET_ERR_HDL(SYS_ERR_L2_FILTER_BE_MCAST, ret);
            return SYS_ERR_OK;
        }

        SYS_MEM_CLEAR(zeroMac);
        if (0 == osal_memcmp(&entry.mac,&zeroMac,sizeof(zeroMac)))
        {
            CGI_SET_ERR_HDL(SYS_ERR_L2_FILTER_AZERO_INVALID, ret);
            return SYS_ERR_OK;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_MAC_VID, pStrValue))
    {
        entry.vid = SYS_STR2UINT(pStrValue);
    }

    CGI_SET_ERR_HDL(sal_l2_blacklistMac_add(&entry), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mac_filterEdit_set(void)
{
    int32   ret = SYS_ERR_OK;
    char *pStrValue;
    sys_l2_entry_t oldEntry;
    sys_l2_entry_t newEntry;

    SYS_MEM_CLEAR(oldEntry);
    SYS_MEM_CLEAR(newEntry);

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

    osal_memcpy(&newEntry, &oldEntry, sizeof(sys_l2_entry_t));

    if (CGI_IS_PARAM_EXIST(CGI_MAC_VID, pStrValue))
    {
        newEntry.vid = SYS_STR2UINT(pStrValue);
    }

    if (0 != osal_memcmp(&oldEntry,&newEntry,sizeof(sys_l2_entry_t)))
    {
        CGI_SET_ERR_HDL(sal_l2_blacklistMac_del(&oldEntry), ret);
        CGI_SET_ERR_HDL(sal_l2_blacklistMac_add(&newEntry), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mac_filterDelete_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i=0;
    uint32 len=0;
    char **array=NULL;
    char *flag = "_";
    char *value =  NULL;
    sys_l2_entry_t macEntry;

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array ,&len))
    {
        for(i=0; i<len; i++)
        {
            SYS_MEM_CLEAR(macEntry);

            value = osal_strtok(array[i], flag);
            macEntry.vid = SYS_STR2UINT(value);

            value = osal_strtok(NULL, flag);
            sys_util_str2Mac(value, macEntry.mac.octet);

            CGI_SET_ENTRY_ERR_CONTINUE(sal_l2_blacklistMac_del(&macEntry), "Delete");
        }
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}
#endif
