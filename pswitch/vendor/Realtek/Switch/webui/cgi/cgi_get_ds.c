
#include <include/cgi.h>

static int32 cgi_ds_prop_get(void);
static int32 cgi_ds_portEdit_get(void);
static int32 cgi_ds_statistic_get(void);
static int32 cgi_ds_option82Prop_get(void);
static int32 cgi_ds_option82PortEdit_get(void);
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
static int32 cgi_ds_option82CID_get(void);
#endif

const cgi_cmdmap_t cmdGetDsTbl[] =
{
    {CGI_CMD_DS_PROP,               cgi_ds_prop_get},
    {CGI_CMD_DS_PORTEDIT,           cgi_ds_portEdit_get},
    {CGI_CMD_DS_STATISTIC,	        cgi_ds_statistic_get},
    {CGI_CMD_DS_82_PROP,     	    cgi_ds_option82Prop_get},
    {CGI_CMD_DS_82_PORTEDIT,        cgi_ds_option82PortEdit_get},
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    {CGI_CMD_DS_82_CID,     	    cgi_ds_option82CID_get},
#endif
    {NULL, NULL}
};

static int32 cgi_ds_prop_get(void)
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
    sys_enable_t chaddr = DISABLED;
    sys_logic_port_t lp = 0;
    sys_vid_t vid = 1;
    sys_vlanmask_t vmList;
    sys_vlanmask_t daivmList;
    sys_namestr_t name;

    sal_dhcp_snooping_enable_get(&enable);

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();

    cJSON_AddBoolToObject(data, CGI_STATE, enable);
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

	FOR_EACH_LOGIC_PORT(lp)
	{
		sal_dhcp_snooping_port_trust_get(lp, &trust);
		sal_dhcp_snooping_portChaddrChk_get(lp, &chaddr);
		sal_dhcp_snooping_rateLimit_get(lp, &rate);

	    port = cJSON_CreateObject();
	    cJSON_AddItemToArray(ports, port);

	    cJSON_AddBoolToObject(port, CGI_DS_TRUSTED, trust);
	    cJSON_AddBoolToObject(port, CGI_DS_VERIMAC, chaddr);

        if (BOND_RATE_UNLIMITED == rate)
            cJSON_AddNumberToObject(port, CGI_DS_RATE, 0);
        else
            cJSON_AddNumberToObject(port, CGI_DS_RATE, rate);
	}

	vlans = cJSON_CreateArray();
    selVlans = cJSON_CreateArray();

    SYS_MEM_CLEAR(vmList);
    SYS_MEM_CLEAR(daivmList);
    sal_vlan_operEntryExist_get(&vmList);
    sal_dhcp_snooping_vlanmask_enable_get(&daivmList);

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
    	cJSON_AddItemToObject(data, CGI_DS_VLANS, vlans);
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
    	cJSON_AddItemToObject(data, CGI_DS_SELVLANS, selVlans);
    else
    	cJSON_Delete(selVlans);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_ds_portEdit_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	char *value = NULL;
	uint32 rate = 0;
	sys_logic_port_t lp = 0;
	sys_enable_t trust = DISABLED;
	sys_enable_t chaddr = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

	sal_dhcp_snooping_port_trust_get(lp, &trust);
	sal_dhcp_snooping_portChaddrChk_get(lp, &chaddr);
	sal_dhcp_snooping_rateLimit_get(lp, &rate);

	CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_DS_TRUSTED, trust);
    cJSON_AddBoolToObject(data, CGI_DS_VERIMAC, chaddr);

    if (BOND_RATE_UNLIMITED == rate)
        cJSON_AddNumberToObject(data, CGI_DS_RATE, 0);
    else
        cJSON_AddNumberToObject(data, CGI_DS_RATE, rate);

	cJSON_AddNumberToObject(data, CGI_DS_DEFRATE, 0);
	cJSON_AddNumberToObject(data, CGI_DS_MINRATE, 0);
	cJSON_AddNumberToObject(data, CGI_DS_MAXRATE, 300);

    if (BOND_RATE_UNLIMITED == DFLT_DHCP_SNOOPING_RATE_LIMIT)
	    cJSON_AddNumberToObject(data, CGI_DS_DEFRATE, 0);
    else
        cJSON_AddNumberToObject(data, CGI_DS_DEFRATE, DFLT_DHCP_SNOOPING_RATE_LIMIT);

    cJSON_AddNumberToObject(data, CGI_DS_MINRATE, BOND_DHCP_SNOOPING_RATELIMIT_MIN);
	cJSON_AddNumberToObject(data, CGI_DS_MAXRATE, BOND_DHCP_SNOOPING_RATELIMIT_MAX);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_ds_statistic_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    sys_dhcp_mib_t counter;

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();

    FOR_EACH_LOGIC_PORT(lp)
    {
        sal_dhcp_snooping_portMib_get(lp, &counter);

	    port = cJSON_CreateObject();
        cJSON_AddNumberToObject(port, CGI_DS_FWD, counter.dhcpForward);
        cJSON_AddNumberToObject(port, CGI_DS_CHADDRERR, counter.dhcpChaddrChkDrop);
        cJSON_AddNumberToObject(port, CGI_DS_UNTRUSTERR, counter.dhcpUntrustDrop);
        cJSON_AddNumberToObject(port, CGI_DS_UNTRUST82ERR, counter.dhcpUntrustWithOption82Drop);
        cJSON_AddNumberToObject(port, CGI_DS_INVALIDERR, counter.dhcpInvalidDrop);
        cJSON_AddItemToArray(ports, port);
    }

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_ds_option82Prop_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	cJSON *ports = NULL;
	cJSON *port = NULL;
    sys_mac_t sysMac;
    sys_enable_t enable;
	sys_logic_port_t lp;
	sys_dhcp_option82_id_t id;
	sys_dhcp_option82_act_t action;

	CGI_GET_JSON_CREATE(root, data);
	ports = cJSON_CreateArray();

    SYS_MEM_CLEAR(id);
	SYS_ERR_CHK(sal_dhcp_option82_remoteID_get(&id));

	cJSON_AddStringToObject(data, CGI_DS_RID_STATIC, id.str);
	cJSON_AddNumberToObject(data, CGI_DS_RID_MAX_LEN, (sizeof(sys_dhcp_option82_id_t) - 1));

	if (0 == osal_strlen(id.str))
    {
        SYS_ERR_CHK(sal_sys_macAddr_get(&sysMac));
        osal_memset(&id, 0, sizeof(sys_dhcp_option82_id_t));
        osal_sprintf(id.str,"%02x:%02x:%02x:%02x:%02x:%02x (Switch Mac in Byte Order)",
        sysMac.octet[0], sysMac.octet[1], sysMac.octet[2],
        sysMac.octet[3], sysMac.octet[4], sysMac.octet[5]);
    }

    cJSON_AddStringToObject(data,CGI_DS_RID_OPER, id.str);
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

	FOR_EACH_LOGIC_PORT(lp)
	{
	    sal_dhcp_option82_enable_get(lp, &enable);
		sal_dhcp_option82_action_get(lp, &action);

	    port = cJSON_CreateObject();
	    cJSON_AddItemToArray(ports, port);

        cJSON_AddBoolToObject(port, CGI_STATE, enable);
		cJSON_AddNumberToObject(port, CGI_DS_ALLOWUNTRUSTED, action);
	}

	CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_ds_option82PortEdit_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	char *value = NULL;
	sys_logic_port_t lp = 0;
	sys_enable_t enable;
	sys_dhcp_option82_act_t action;

	if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    sal_dhcp_option82_enable_get(lp, &enable);
	sal_dhcp_option82_action_get(lp, &action);

	CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_STATE, enable);
	cJSON_AddNumberToObject(data, CGI_DS_ALLOWUNTRUSTED, action);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
static int32 cgi_ds_option82CID_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *dsTbl = NULL;
    cJSON *ds = NULL;
    uint32 entry = 0;
    uint32 i = 0;
    sys_logic_port_t lp;
    sys_dhcp_option82_circuit_id_t cid[SYS_DHCP_O82_CID_VLAN_MAX+1];

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_MAXLEN, (sizeof(sys_dhcp_option82_id_t) - 1));

    dsTbl = cJSON_CreateArray();
    entry = 0;

    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(cid);
    	sal_dhcp_option82_circuitID_get(lp, cid);

        for (i = 0; i <= SYS_DHCP_O82_CID_VLAN_MAX; i++)
        {
            if (0 == osal_strlen(cid[i].str))
                continue;

            ds = cJSON_CreateObject();
            cJSON_AddNumberToObject(ds, CGI_PORT, lp);
            cJSON_AddNumberToObject(ds, CGI_VLAN, (cid[i].vid == 0) ? DFLT_VLAN_RESERVED_VLAN : cid[i].vid);
            cJSON_AddStringToObject(ds, CGI_DS_CID, cid[i].str);
            cJSON_AddItemToArray(dsTbl, ds);
            entry++;
        }
    }

    if (0 != entry)
    	cJSON_AddItemToObject(data, CGI_DS_ENTRY, dsTbl);
    else
    {
    	cJSON_Delete(dsTbl);
    	cJSON_AddNumberToObject(data, CGI_EMPTY, 0);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif
