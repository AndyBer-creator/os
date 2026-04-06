
#include <include/cgi.h>

static int32 cgi_mac_miscStatus_get(void);
static int32 cgi_mac_dynamic_get(void);
static int32 cgi_mac_static_get(void);
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
static int32 cgi_mac_filter_get(void);
#endif

const cgi_cmdmap_t cmdGetMacTbl[] =
{
    {CGI_CMD_MAC_MISCSTATUS,        cgi_mac_miscStatus_get},
    {CGI_CMD_MAC_DYNAMIC,        cgi_mac_dynamic_get},
    {CGI_CMD_MAC_STATIC,        cgi_mac_static_get},
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    {CGI_CMD_MAC_FILTER,        cgi_mac_filter_get},
#endif
    {NULL, NULL}
};

static int32 cgi_mac_miscStatus_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *mac_array = NULL;
    cJSON                   *mac = NULL;
    sys_mac_t               cpuMac;
    char                    macStr[SYS_BUF32_LEN];
    char                    portStr[CAPA_PORT_STR_LEN];
    uint32                  mac_table_size;
    uint32                  i;
    sys_l2_entry_t          *pEntry = NULL;
    sys_vid_t               mgmtVid = 0;

    SYS_MEM_CLEAR(cpuMac);
    SYS_ERR_CHK(sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &mac_table_size));

    CGI_GET_JSON_CREATE(root, data);

    mac_array = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_MAC_ENTRYS, mac_array);

    SYS_ERR_CHK(sal_sys_macAddr_get(&cpuMac));
#if defined(CONFIG_SYS_MANAGEMENT_VLAN) && !defined(CONFIG_SYS_L3_ARCHITECTURE)
    SYS_ERR_CHK(sal_vlan_mgmtVid_get(&mgmtVid));
#else
    SYS_ERR_CHK(sal_vlan_defaultVid_get(&mgmtVid));
#endif
    sys_util_mac2Str(cpuMac.octet, macStr);

    mac = cJSON_CreateObject();
    cJSON_AddNumberToObject(mac, CGI_MAC_VID, mgmtVid);
    cJSON_AddStringToObject(mac, CGI_MAC_ADDR, macStr);
    cJSON_AddLangStringToObject(mac, CGI_MAC_PORT, "mac","lblCpuPort");
    cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE, "mac","lblMacTypeManage");
    cJSON_AddItemToArray(mac_array, mac);

    for (i = 0; i < mac_table_size; i++)
    {
        pEntry = L2_SORT_ENTRY_PTR(i);

        if (SYS_L2_TYPE_MGMT == pEntry->type)
            continue;
            
        if (mgmtVid == pEntry->vid && SYS_L2_TYPE_L3_MGMT == pEntry->type)
        	continue;

        mac = cJSON_CreateObject();

        cJSON_AddNumberToObject(mac, CGI_MAC_VID, pEntry->vid);

        SYS_ERR_CHK(sys_util_mac2Str(pEntry->mac.octet, macStr));
        cJSON_AddStringToObject(mac, CGI_MAC_ADDR, macStr);
        SYS_ERR_CHK(LP2WEBSTR(pEntry->port, portStr));
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
        if (SYS_L2_TYPE_BLACKLIST == pEntry->type)
            cJSON_AddStringToObject(mac, CGI_MAC_PORT, "N/A");
        else
#endif
		if (SYS_L2_TYPE_L3_MGMT == pEntry->type)
			cJSON_AddLangStringToObject(mac, CGI_MAC_PORT, "mac","lblCpuPort");
		else
            cJSON_AddStringToObject(mac, CGI_MAC_PORT, portStr);

        switch (pEntry->type)
        {
            case SYS_L2_TYPE_DYNAMIC:
                cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeDynamic");
                break;

            case SYS_L2_TYPE_STATIC:
                cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeStatic");
                break;
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
            case SYS_L2_TYPE_BLACKLIST:
                cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeBlacklist");
                break;
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
            case SYS_L2_TYPE_AUTH:
                cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeAuth");
                break;
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
            case SYS_L2_TYPE_PSECURE_DYNAMIC:
                cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeSecureDynamic");
                break;

            case SYS_L2_TYPE_PSECURE_STICKY:
                cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeSecureSticky");
                break;

            case SYS_L2_TYPE_PSECURE_STATIC:
                cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeSecureConfigured");
                break;
#endif
            case SYS_L2_TYPE_L3_MGMT:
                cJSON_AddLangStringToObject(mac, CGI_MAC_TYPE,  "mac", "lblMacTypeManage");
                break;

            default:
                break;
        }

        cJSON_AddItemToArray(mac_array, mac);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mac_dynamic_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *mac_array = NULL;
    cJSON                   *mac = NULL;

    uint32                  table_size;
    uint32                  dynamic_cnt = 0;
    uint32                  aging_time;
    uint32                  i;
    sys_l2_entry_t          *pEntry = NULL;
    char                    macStr[SYS_BUF32_LEN];
    char                    portStr[CAPA_PORT_STR_LEN];
    char                    keyStr[SYS_BUF64_LEN];

    sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &table_size);

    CGI_GET_JSON_CREATE(root, data);

    sal_l2_aging_get(&aging_time);
    cJSON_AddNumberToObject(data, CGI_MAC_AGING, aging_time);
    cJSON_AddNumberToObject(data, CGI_MAC_AGING_MIN, BOND_L2_MAC_AGING_MIN);
    cJSON_AddNumberToObject(data, CGI_MAC_AGING_MAX, BOND_L2_MAC_AGING_MAX);
    cJSON_AddNumberToObject(data, CGI_MAC_AGING_DFLT, DFLT_L2_AGING_TIME);

    mac_array = cJSON_CreateArray();

    for (i = 0; i < table_size; i++)
    {
        pEntry = L2_SORT_ENTRY_PTR(i);

        if (SYS_L2_TYPE_DYNAMIC != pEntry->type)
            continue;

        mac = cJSON_CreateObject();

        cJSON_AddNumberToObject(mac, CGI_MAC_VID, pEntry->vid);

        SYS_ERR_CHK(sys_util_mac2Str(pEntry->mac.octet, macStr));
        cJSON_AddStringToObject(mac, CGI_MAC_ADDR, macStr);
        SYS_ERR_CHK(LP2WEBSTR(pEntry->port, portStr));
        cJSON_AddStringToObject(mac, CGI_MAC_PORT, portStr);

        SYS_MEM_CLEAR(keyStr);
        osal_sprintf(keyStr, "%02X%02X%02X%02X%02X%02X_%d_%d",
                        pEntry->mac.octet[0],
                        pEntry->mac.octet[1],
                        pEntry->mac.octet[2],
                        pEntry->mac.octet[3],
                        pEntry->mac.octet[4],
                        pEntry->mac.octet[5],
                        pEntry->vid,
                        pEntry->port);
        cJSON_AddStringToObject(mac, "key", keyStr);

        cJSON_AddItemToArray(mac_array, mac);

        dynamic_cnt++;
    }

    if (dynamic_cnt > 0)
        cJSON_AddItemToObject(data, CGI_MAC_ENTRYS, mac_array);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mac_static_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *mac_array = NULL;
    cJSON                   *mac = NULL;
    sys_mac_t               emptyMac;
    char                    macStr[SYS_BUF32_LEN];
    char                    portStr[CAPA_PORT_STR_LEN];
    uint32                  mac_table_size;

    uint32                  i;
    sys_l2_entry_t          *pEntry = NULL;

    SYS_MEM_CLEAR(emptyMac);
    SYS_ERR_CHK(sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &mac_table_size));

    CGI_GET_JSON_CREATE(root, data);

    mac_array = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_MAC_ENTRYS, mac_array);

    mac = cJSON_CreateObject();
    cJSON_AddNumberToObject(mac, CGI_MAC_VID, 0);
    cJSON_AddStringToObject(mac, CGI_MAC_ADDR, "");
    cJSON_AddStringToObject(mac, CGI_MAC_PORT, "");

    cJSON_AddItemToArray(mac_array, mac);

    for (i = 0; i < mac_table_size; i++)
    {
        pEntry = L2_SORT_ENTRY_PTR(i);

        if (SYS_L2_TYPE_STATIC != pEntry->type)
            continue;

        mac = cJSON_CreateObject();

        cJSON_AddNumberToObject(mac, CGI_MAC_VID, pEntry->vid);

        SYS_ERR_CHK(sys_util_mac2Str(pEntry->mac.octet, macStr));
        cJSON_AddStringToObject(mac, CGI_MAC_ADDR, macStr);
        SYS_ERR_CHK(LP2WEBSTR(pEntry->port, portStr));
        cJSON_AddStringToObject(mac, CGI_MAC_PORT, portStr);

        cJSON_AddItemToArray(mac_array, mac);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_BLACK_HOLE_MAC
static int32 cgi_mac_filter_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *mac_array = NULL;
    cJSON                   *mac = NULL;
    sys_mac_t               emptyMac;
    char                    macStr[SYS_BUF32_LEN];
    uint32                  mac_table_size;
    uint32                  i;
    sys_l2_entry_t          *pEntry = NULL;

    SYS_MEM_CLEAR(emptyMac);
    SYS_ERR_CHK(sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &mac_table_size));

    CGI_GET_JSON_CREATE(root, data);

    mac_array = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_MAC_ENTRYS, mac_array);

    mac = cJSON_CreateObject();
    cJSON_AddNumberToObject(mac, CGI_MAC_VID, 0);
    cJSON_AddStringToObject(mac, CGI_MAC_ADDR, "");
    cJSON_AddStringToObject(mac, CGI_MAC_PORT, "");

    cJSON_AddItemToArray(mac_array, mac);

    for (i = 0; i < mac_table_size; i++)
    {
        pEntry = L2_SORT_ENTRY_PTR(i);

        if (SYS_L2_TYPE_BLACKLIST != pEntry->type)
            continue;

        mac = cJSON_CreateObject();

        cJSON_AddNumberToObject(mac, CGI_MAC_VID, pEntry->vid);

        SYS_ERR_CHK(sys_util_mac2Str(pEntry->mac.octet, macStr));
        cJSON_AddStringToObject(mac, CGI_MAC_ADDR, macStr);

        cJSON_AddItemToArray(mac_array, mac);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif
