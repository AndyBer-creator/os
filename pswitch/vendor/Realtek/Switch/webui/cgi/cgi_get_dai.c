
#include <include/cgi.h>
#include <libsal/sal_dai.h>

static int32 cgi_dai_prop_get(void);
static int32 cgi_dai_portEdit_get(void);
static int32 cgi_dai_statistic_get(void);

const cgi_cmdmap_t cmdGetDaiTbl[] =
{
    {CGI_CMD_DAI_PROP,          cgi_dai_prop_get},
    {CGI_CMD_DAI_PORTEDIT,      cgi_dai_portEdit_get},
    {CGI_CMD_DAI_STATISTIC,     cgi_dai_statistic_get},
    {NULL, NULL}
};

static int32 cgi_dai_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
	cJSON *port = NULL;
	cJSON *vlans = NULL;
    cJSON *selVlans = NULL;
    cJSON *existVlan = NULL;
    cJSON *enabledVlan = NULL;
	uint32 rate = 0;
    uint32 entry  =0;
    sys_enable_t enable = DISABLED;
    sys_enable_t trust = DISABLED;
    sys_enable_t smacChk = DISABLED;
    sys_enable_t dmacChk = DISABLED;
    sys_enable_t ipChk = DISABLED;
    sys_enable_t allowZeros = DISABLED;
    sys_logic_port_t lp;
    sys_vid_t vid = 1;
    sys_vlanmask_t vmList;
    sys_vlanmask_t daivmList;
    sys_namestr_t name;

    sal_dai_enable_get(&enable);

	CGI_GET_JSON_CREATE(root, data);

	ports = cJSON_CreateArray();

    cJSON_AddBoolToObject(data, CGI_STATE, enable);
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

	FOR_EACH_LOGIC_PORT(lp)
	{
		sal_dai_port_trust_get(lp, &trust);
		sal_dai_port_smacChk_get(lp, &smacChk);
        sal_dai_port_dmacChk_get(lp, &dmacChk);
        sal_dai_port_ipChk_get(lp, &ipChk);
        sal_dai_port_ipZeroAllow_get(lp, &allowZeros);
		sal_dai_rateLimit_get(lp, &rate);

	    port = cJSON_CreateObject();
	    cJSON_AddItemToArray(ports, port);

	    cJSON_AddBoolToObject(port, CGI_DAI_TRUSTED, trust);
        cJSON_AddBoolToObject(port, CGI_DAI_VERISMAC, smacChk);
        cJSON_AddBoolToObject(port, CGI_DAI_VERIDMAC, dmacChk);
        cJSON_AddBoolToObject(port, CGI_DAI_VERIIP, ipChk);
        cJSON_AddBoolToObject(port, CGI_DAI_ALLOWZEROS, allowZeros);

        if (BOND_RATE_UNLIMITED == rate)
            cJSON_AddNumberToObject(port, CGI_DAI_RATE, 0);
        else
            cJSON_AddNumberToObject(port, CGI_DAI_RATE, rate);
	}

	vlans = cJSON_CreateArray();
    selVlans = cJSON_CreateArray();

    SYS_MEM_CLEAR(vmList);
    SYS_MEM_CLEAR(daivmList);
    sal_vlan_operEntryExist_get(&vmList);
    sal_dai_vlanmask_enable_get(&daivmList);

    FOR_EACH_VID_IN_VLANMASK(vid, vmList)
    {
        if (!IS_VLAN_OPER_ENTRY_EXIST(vid))
            continue;

        if (VLANMASK_IS_VIDSET(daivmList, vid))
            continue;

        existVlan = cJSON_CreateObject();

    	osal_sprintf(name.str, "VLAN %d", vid);
        cJSON_AddStringToObject(existVlan, CGI_NAME, name.str);
    	cJSON_AddNumberToObject(existVlan, CGI_ID, vid);
	    cJSON_AddItemToArray(vlans, existVlan);
	    entry++;
    }

    if (0 != entry)
    	cJSON_AddItemToObject(data, CGI_DAI_VLANS, vlans);
    else
    	cJSON_Delete(vlans);

    entry = 0;

    FOR_EACH_VID_IN_VLANMASK(vid, daivmList)
    {
        enabledVlan = cJSON_CreateObject();

    	 osal_sprintf(name.str, "VLAN %d", vid);
         cJSON_AddStringToObject(enabledVlan, CGI_NAME, name.str);
    	 cJSON_AddNumberToObject(enabledVlan, CGI_ID, vid);
    	 cJSON_AddItemToArray(selVlans, enabledVlan);
    	 entry++;
    }

    if (0 != entry)
    	cJSON_AddItemToObject(data, CGI_DAI_SELVLANS, selVlans);
    else
    	cJSON_Delete(selVlans);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_dai_portEdit_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	uint32 rate = 0;
	char *value = NULL;
    sys_logic_port_t lp = 0;
    sys_enable_t trust = DISABLED;
    sys_enable_t smacChk = DISABLED;
    sys_enable_t dmacChk = DISABLED;
    sys_enable_t ipChk = DISABLED;
    sys_enable_t allowZeros = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    sal_dai_port_trust_get(lp, &trust);
	sal_dai_port_smacChk_get(lp, &smacChk);
    sal_dai_port_dmacChk_get(lp, &dmacChk);
    sal_dai_port_ipChk_get(lp, &ipChk);
    sal_dai_port_ipZeroAllow_get(lp, &allowZeros);
	sal_dai_rateLimit_get(lp, &rate);

	CGI_GET_JSON_CREATE(root, data);

	cJSON_AddBoolToObject(data, CGI_DAI_TRUSTED, trust);
    cJSON_AddBoolToObject(data, CGI_DAI_VERISMAC, smacChk);
    cJSON_AddBoolToObject(data, CGI_DAI_VERIDMAC, dmacChk);
    cJSON_AddBoolToObject(data, CGI_DAI_VERIIP, ipChk);
    cJSON_AddBoolToObject(data, CGI_DAI_ALLOWZEROS, allowZeros);
    if (BOND_RATE_UNLIMITED == rate)
        cJSON_AddNumberToObject(data, CGI_DAI_RATE, 0);
    else
        cJSON_AddNumberToObject(data, CGI_DAI_RATE, rate);

    if (BOND_RATE_UNLIMITED == DFLT_DAI_RATELIMIT_RATE)
	    cJSON_AddNumberToObject(data, CGI_DAI_DEFRATE, 0);
    else
        cJSON_AddNumberToObject(data, CGI_DAI_DEFRATE, DFLT_DAI_RATELIMIT_RATE);

    cJSON_AddNumberToObject(data, CGI_DAI_MINRATE, BOND_DAI_RATELIMIT_MIN);
	cJSON_AddNumberToObject(data, CGI_DAI_MAXRATE, BOND_DAI_RATELIMIT_MAX);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_dai_statistic_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_arp_mib_t counter;
    sys_logic_port_t lp;

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();

    FOR_EACH_LOGIC_PORT(lp)
    {
	    sal_dai_portMib_get(lp, &counter);

	    port = cJSON_CreateObject();

        cJSON_AddNumberToObject(port, CGI_DAI_FWD,  counter.arpForward);
        cJSON_AddNumberToObject(port, CGI_DAI_SMACERR,  counter.arpSmacDrop);
        cJSON_AddNumberToObject(port, CGI_DAI_DMACERR,  counter.arpDmacDrop);
        cJSON_AddNumberToObject(port, CGI_DAI_DIPERR,  counter.arpDIPDrop);
        cJSON_AddNumberToObject(port, CGI_DAI_SIPERR,  counter.arpSIPDrop);
        cJSON_AddNumberToObject(port, CGI_DAI_IPMACERR,  counter.arpIPMACMismatchDrop);

        cJSON_AddItemToArray(ports, port);
    }

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

