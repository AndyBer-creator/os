
#include <include/cgi.h>

static int32 cgi_vlan_voice_prop_get(void);
static int32 cgi_vlan_voice_portEdit_get(void);
static int32 cgi_vlan_voice_oui_get(void);

const cgi_cmdmap_t cmdGetVlanVoiceTbl[] =
{
    {CGI_CMD_VLAN_VOICE_PROP,          cgi_vlan_voice_prop_get},
    {CGI_CMD_VLAN_VOICE_PORTEDIT,      cgi_vlan_voice_portEdit_get},
    {CGI_CMD_VLAN_VOICE_OUI,           cgi_vlan_voice_oui_get},
    {NULL, NULL}
};

static int32 cgi_vlan_voice_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
	cJSON *port = NULL;
	cJSON *vlans = NULL;
    cJSON *vlan = NULL;
    uint32 aging;
    sys_vid_t voiceVid;
    voice_vlan_state_t state;
    sys_vlan_voiceQos_t voiceQos;
    sys_enable_t remark;
    sys_enable_t portEnable;
    sys_logic_port_t lp;
    sys_vid_t vid = 1;
    sys_vid_t defvid = 1;
    sys_vlanmask_t vmList;
    sys_vlan_operEntry_t entry;
    voice_vlan_port_mode_t portMode;
    voice_vlan_cos_mode_t cosMode;

    sal_vlan_voiceVlanState_get(&state);
    sal_vlan_voiceVid_get(&voiceVid);
    sal_vlan_voiceVlanQos_get(&voiceQos);
    sal_vlan_voiceVlanAging_get(&aging);
    sal_vlan_voiceVlan1pRemarkEnable_get(&remark);

	CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_STATE, (state == VOICE_VLAN_DISABLED ) ? FALSE : TRUE);
    cJSON_AddNumberToObject(data, CGI_VOICEVLAN_VLAN, voiceVid);
    cJSON_AddBoolToObject(data, CGI_VOICEVLAN_COS_REMARK, remark);
    cJSON_AddNumberToObject(data, CGI_VOICEVLAN_COS, voiceQos.pri);
    cJSON_AddNumberToObject(data, CGI_VOICEVLAN_AGING, aging);
    cJSON_AddNumberToObject(data, CGI_VOICEVLAN_MIN_AGING, BOND_VLAN_VOICE_AGING_MIN);
    cJSON_AddNumberToObject(data, CGI_VOICEVLAN_MAX_AGING, BOND_VLAN_VOICE_AGING_MAX);
    cJSON_AddNumberToObject(data, CGI_VOICEVLAN_DEF_AGING, DFLT_VLAN_VOICE_AGING);

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
        sal_vlan_portVoiceVlanEnable_get(lp, &portEnable);
        sal_vlan_portVoiceVlanMode_get(lp, &portMode);
        sal_vlan_portVoiceVlancosMode_get(lp, &cosMode);

	    port = cJSON_CreateObject();
	    cJSON_AddItemToArray(ports, port);

	    cJSON_AddBoolToObject(port, CGI_STATE, portEnable);
	    cJSON_AddNumberToObject(port, CGI_VOICEVLAN_PORT_MODE, portMode);
        cJSON_AddNumberToObject(port, CGI_VOICEVLAN_COS_MODE, cosMode);
	}

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_voice_portEdit_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	char *value = NULL;
    sys_logic_port_t lp = 0;
    sys_enable_t portEnable = DISABLED;
    voice_vlan_port_mode_t portMode;
    voice_vlan_cos_mode_t cosMode;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    sal_vlan_portVoiceVlanEnable_get(lp, &portEnable);
    sal_vlan_portVoiceVlanMode_get(lp, &portMode);
    sal_vlan_portVoiceVlancosMode_get(lp, &cosMode);

	CGI_GET_JSON_CREATE(root, data);

	cJSON_AddBoolToObject(data, CGI_STATE, portEnable);
    cJSON_AddNumberToObject(data, CGI_VOICEVLAN_PORT_MODE, portMode);
    cJSON_AddNumberToObject(data, CGI_VOICEVLAN_COS_MODE, cosMode);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_voice_oui_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
    cJSON *ouis = NULL;
	cJSON *oui = NULL;
	char tmp[SYS_BUF16_LEN];
    uint32 i = 0;
    sys_vlan_voiceVlan_oui_t ouicfg;
    sys_namestr_t name;

	CGI_GET_JSON_CREATE(root, data);
	cJSON_AddNumberToObject(data, CGI_MAXLEN, CAPA_NAME_STR_LEN);

	ouis = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_VOICEVLAN_OUIS, ouis);

    for (i = 0; i < CAPA_VLAN_VOICE_OUI_NUM; i++)
    {
        SYS_MEM_CLEAR(ouicfg);
        SYS_MEM_CLEAR(name);

        sal_vlan_voiceVlanOui_get(i, &ouicfg, &name);
        oui = cJSON_CreateObject();

        if (DISABLED == ouicfg.enable)
        {
            cJSON_AddStringToObject(oui, CGI_VOICEVLAN_OUI1, "");
            cJSON_AddItemToArray(ouis, oui);
            continue;
        }

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%02X", ouicfg.oui[0]);
        cJSON_AddStringToObject(oui, CGI_VOICEVLAN_OUI1, tmp);

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%02X", ouicfg.oui[1]);
        cJSON_AddStringToObject(oui, CGI_VOICEVLAN_OUI2, tmp);

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%02X", ouicfg.oui[2]);
        cJSON_AddStringToObject(oui, CGI_VOICEVLAN_OUI3, tmp);
        cJSON_AddStringToObject(oui, CGI_VOICEVLAN_OUT_DESP, name.str);

        cJSON_AddItemToArray(ouis, oui);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

