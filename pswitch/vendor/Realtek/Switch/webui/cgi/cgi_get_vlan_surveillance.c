
#include <include/cgi.h>

static int32 cgi_vlan_surveillance_prop_get(void);
static int32 cgi_vlan_surveillance_portEdit_get(void);
static int32 cgi_vlan_surveillance_oui_get(void);

const cgi_cmdmap_t cmdGetVlanSurveillanceTbl[] =
{
    {CGI_CMD_VLAN_SURVEILLANCEVLAN_PROP,          cgi_vlan_surveillance_prop_get},
    {CGI_CMD_VLAN_SURVEILLANCEVLAN_PORTEDIT,      cgi_vlan_surveillance_portEdit_get},
    {CGI_CMD_VLAN_SURVEILLANCEVLAN_OUI,           cgi_vlan_surveillance_oui_get},
    {NULL, NULL}
};

static int32 cgi_vlan_surveillance_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
	cJSON *port = NULL;
	cJSON *vlans = NULL;
    cJSON *vlan = NULL;
    uint32 aging;
    sys_pri_t pri;
    sys_enable_t state;
    sys_enable_t remark;
    sys_enable_t portEnable;
    sys_logic_port_t lp;
    sys_vid_t surVid;
    sys_vid_t vid = 1;
    sys_vid_t defvid = 1;
    sys_vlanmask_t vmList;
    sys_vlan_operEntry_t entry;
    surveillance_vlan_port_mode_t portMode;
    surveillance_vlan_cos_mode_t cosMode;

    sal_surveillance_vlanState_get(&state);
    sal_surveillance_vlanVid_get(&surVid);
    sal_surveillance_vlanPri_get(&pri);
    sal_surveillance_vlanAging_get(&aging);
    sal_surveillance_vlan1pRemarkEnable_get(&remark);

	CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_STATE, state);
    cJSON_AddNumberToObject(data, CGI_SURVEILLANCEVLAN_VLAN, surVid);
    cJSON_AddBoolToObject(data, CGI_SURVEILLANCEVLAN_COS_REMARK, remark);
    cJSON_AddNumberToObject(data, CGI_SURVEILLANCEVLAN_COS, pri);
    cJSON_AddNumberToObject(data, CGI_SURVEILLANCEVLAN_AGING, aging);
    cJSON_AddNumberToObject(data, CGI_SURVEILLANCEVLAN_MIN_AGING, BOND_VLAN_SURVEILLANCE_AGING_MIN);
    cJSON_AddNumberToObject(data, CGI_SURVEILLANCEVLAN_MAX_AGING, BOND_VLAN_SURVEILLANCE_AGING_MAX);
    cJSON_AddNumberToObject(data, CGI_SURVEILLANCEVLAN_DEF_AGING, DFLT_VLAN_SURVEILLANCE_AGING);

    vlans = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_VLANS, vlans);

    sal_vlan_adminEntryExist_get(&vmList);
    sal_vlan_defaultVid_get(&defvid);

    vlan = cJSON_CreateObject();
    cJSON_AddStringToObject(vlan, CGI_NAME, "None");
    cJSON_AddNumberToObject(vlan, CGI_VALUE, DFLT_VLAN_RESERVED_VLAN);
    cJSON_AddItemToArray(vlans, vlan);

    FOR_EACH_VID_IN_VLANMASK(vid, vmList)
    {
        if (vid == defvid)
            continue;

        SYS_MEM_CLEAR(entry);

        sal_vlan_operEntry_get(vid, &entry);

        vlan = cJSON_CreateObject();

        cJSON_AddStringToObject(vlan, CGI_NAME, entry.name.str);
    	cJSON_AddNumberToObject(vlan, CGI_VALUE, vid);
	    cJSON_AddItemToArray(vlans, vlan);
    }

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

	FOR_EACH_LOGIC_PORT(lp)
	{
        sal_surveillance_vlan_portEnable_get(lp, &portEnable);
        sal_surveillance_vlan_portMode_get(lp, &portMode);
        sal_surveillance_vlan_portCosMode_get(lp, &cosMode);

	    port = cJSON_CreateObject();
	    cJSON_AddItemToArray(ports, port);

	    cJSON_AddBoolToObject(port, CGI_STATE, portEnable);
	    cJSON_AddNumberToObject(port, CGI_SURVEILLANCEVLAN_PORT_MODE, portMode);
        cJSON_AddNumberToObject(port, CGI_SURVEILLANCEVLAN_COS_MODE, cosMode);
	}

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_surveillance_portEdit_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	char *value = NULL;
    sys_logic_port_t lp = 0;
    sys_enable_t portEnable = DISABLED;
    surveillance_vlan_port_mode_t portMode;
    surveillance_vlan_cos_mode_t cosMode;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    sal_surveillance_vlan_portEnable_get(lp, &portEnable);
    sal_surveillance_vlan_portMode_get(lp, &portMode);
    sal_surveillance_vlan_portCosMode_get(lp, &cosMode);

	CGI_GET_JSON_CREATE(root, data);

	cJSON_AddBoolToObject(data, CGI_STATE, portEnable);
    cJSON_AddNumberToObject(data, CGI_SURVEILLANCEVLAN_PORT_MODE, portMode);
    cJSON_AddNumberToObject(data, CGI_SURVEILLANCEVLAN_COS_MODE, cosMode);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_surveillance_oui_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
    cJSON *ouis = NULL;
	cJSON *oui = NULL;
	char tmp[SYS_BUF16_LEN];
    uint32 i = 0;
    surveillance_vlan_oui_t ouicfg;

	CGI_GET_JSON_CREATE(root, data);
	cJSON_AddNumberToObject(data, CGI_MAXLEN, CAPA_NAME_STR_LEN);

	ouis = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_SURVEILLANCEVLAN_OUIS, ouis);

    for (i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        SYS_MEM_CLEAR(ouicfg);

        sal_surveillance_vlanOuibyIdx_get(i, &ouicfg);
        oui = cJSON_CreateObject();

        if (IS_OUI_CLEAR(ouicfg.oui))
        {
            cJSON_AddStringToObject(oui, CGI_SURVEILLANCEVLAN_OUI1, "");
            cJSON_AddItemToArray(ouis, oui);
            continue;
        }

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%02X", ouicfg.oui[0]);
        cJSON_AddStringToObject(oui, CGI_SURVEILLANCEVLAN_OUI1, tmp);

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%02X", ouicfg.oui[1]);
        cJSON_AddStringToObject(oui, CGI_SURVEILLANCEVLAN_OUI2, tmp);

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%02X", ouicfg.oui[2]);
        cJSON_AddStringToObject(oui, CGI_SURVEILLANCEVLAN_OUI3, tmp);
        cJSON_AddStringToObject(oui, CGI_SURVEILLANCEVLAN_OUT_DESP, ouicfg.ouiName.str);

        cJSON_AddItemToArray(ouis, oui);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

