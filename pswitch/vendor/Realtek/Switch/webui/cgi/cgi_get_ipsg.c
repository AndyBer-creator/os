
#include <include/cgi.h>
#include <libsal/sal_isg.h>

static int32 cgi_isg_prop_get(void);
static int32 cgi_isg_portEdit_get(void);
static int32 cgi_isg_bindingTable_get(void);
static int32 cgi_ipsg_database_get(void);

const cgi_cmdmap_t cmdGetIsgTbl[] =
{
    {CGI_CMD_IPSG_PROP,             cgi_isg_prop_get},
    {CGI_CMD_IPSG_PORTEDIT,         cgi_isg_portEdit_get},
    {CGI_CMD_IPSG_BINDING,          cgi_isg_bindingTable_get},
    {CGI_CMD_IPSG_DATABASE,         cgi_ipsg_database_get},
    {NULL, NULL}
};

static int32 cgi_isg_prop_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	cJSON *ports = NULL;
	cJSON *port = NULL;
	int32 num = IPSG_ENTRY_UNLIMITED;
	uint32 cnum = 0;
	sys_logic_port_t lp;
	sys_enable_t enable = DISABLED;
	sys_isg_verify_type_t verifySrcMac = ISG_VERIFY_IP;

	CGI_GET_JSON_CREATE(root, data);

	ports = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_PORTS, ports);

	FOR_EACH_LOGIC_PORT(lp)
	{
		sal_isg_port_get(lp, &enable, &verifySrcMac);
		sal_isg_portMaxEntryNum_get(lp, &num);
		sal_isg_portCurrentEntryNum_get(lp, &cnum);

        port = cJSON_CreateObject();
        cJSON_AddItemToArray(ports, port);

    	cJSON_AddBoolToObject(port, CGI_ISG_STATUS, enable);
		cJSON_AddBoolToObject(port, CGI_ISG_VERIFY_SRCMAC, verifySrcMac);
		cJSON_AddNumberToObject(port, CGI_ISG_CURENTRY, cnum);

		if(-1 == num)
			cJSON_AddNumberToObject(port, CGI_ISG_MAXENTRY, 0);
		else
			cJSON_AddNumberToObject(port, CGI_ISG_MAXENTRY, num);
	}

	CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_isg_portEdit_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	char *value = NULL;
	int32 cnum = 0;
	sys_logic_port_t lp = 0;
	sys_enable_t enable = DISABLED;
	sys_isg_verify_type_t verifySrcMac = ISG_VERIFY_IP;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

	sal_isg_port_get(lp, &enable, &verifySrcMac);
	sal_isg_portMaxEntryNum_get(lp, &cnum);

	CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_ISG_STATUS, enable);
	cJSON_AddBoolToObject(data, CGI_ISG_VERIFY_SRCMAC, verifySrcMac);

	if(IPSG_ENTRY_UNLIMITED == cnum)
    	cJSON_AddNumberToObject(data, CGI_ISG_CURENTRY, 0);
    else
        cJSON_AddNumberToObject(data, CGI_ISG_CURENTRY, cnum);

	cJSON_AddNumberToObject(data, CGI_ISG_MINENTRY, BOND_IPSG_PORT_ENTRY_MIN);
	cJSON_AddNumberToObject(data, CGI_ISG_MAXENTRY, BOND_IPSG_PORT_ENTRY_MAX);

	if (IPSG_ENTRY_UNLIMITED == DFLT_IPSG_PORT_MAX_ENTRY)
    	cJSON_AddNumberToObject(data, CGI_ISG_DEFENTRY, 0);
    else
        cJSON_AddNumberToObject(data, CGI_ISG_DEFENTRY, DFLT_IPSG_PORT_MAX_ENTRY);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_isg_bindingTable_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *isgTbl = NULL;
    cJSON *isg = NULL;
    char macStr[CAPA_MAC_STR_LEN];
    char ipStr[CAPA_IPV4_STR_LEN];
    char ipmaskStr[CAPA_IPV4_STR_LEN];

    uint32 i = 0;
    uint32 cnt = 0;
    sys_mac_t zeroMac;
    sys_isg_entry_t entry;

    SYS_MEM_CLEAR(zeroMac);

    CGI_GET_JSON_CREATE(root, data);
    isgTbl = cJSON_CreateArray();

    for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
    {
    	SYS_MEM_CLEAR(entry);
    	sal_isg_bind_entry_get(i, &entry);

         if (!entry.isUsed || entry.type == ISG_ENTRY_ALL_DENY)
            continue;

        isg = cJSON_CreateObject();

    	SYS_MEM_CLEAR(macStr);
    	SYS_MEM_CLEAR(ipStr);
    	SYS_MEM_CLEAR(ipmaskStr);

    	sys_util_ip2Str(entry.ip, ipStr);

    	if (0 == osal_memcmp(&zeroMac, &entry.mac, sizeof(sys_mac_t)))
    	{
            sys_util_ip2Str(entry.mask, ipmaskStr);
            cJSON_AddStringToObject(isg, CGI_ISG_IPMASK, ipmaskStr);
            cJSON_AddStringToObject(isg, CGI_ISG_MAC, "");
    	}
    	else
    	{
    	    sys_util_mac2Str(entry.mac.octet, macStr);
            cJSON_AddStringToObject(isg, CGI_ISG_IPMASK, "255.255.255.255");
            cJSON_AddStringToObject(isg, CGI_ISG_MAC, macStr);
    	}

        cJSON_AddNumberToObject(isg, CGI_ISG_VLAN, entry.vid);
        cJSON_AddStringToObject(isg, CGI_ISG_IP, ipStr);
        cJSON_AddNumberToObject(isg, CGI_ISG_PORT, entry.port);
        cJSON_AddNumberToObject(isg, CGI_ISG_TYPE, entry.type);
        cJSON_AddNumberToObject(isg, CGI_ISG_TIME, entry.leaseTime);
        cJSON_AddItemToArray(isgTbl, isg);
        cnt++;
    }

    if (0 != cnt)
        cJSON_AddItemToObject(data, CGI_ISG_ENTRY, isgTbl);
    else
    {
        cJSON_Delete(isgTbl);
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_ipsg_database_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_isg_db_t db;

    CGI_GET_JSON_CREATE(root, data);

    SYS_ERR_CHK(sal_isg_db_get(&db));

    cJSON_AddNumberToObject(data, CGI_ISG_BKDBTYPE, db.dbType);
    cJSON_AddStringToObject(data, CGI_ISG_BKDBFILENAME, ((ISG_DB_TYPE_TFTP == db.dbType) ? db.filename : ""));
    cJSON_AddNumberToObject(data, CGI_MAXLEN, CAPA_FILE_NAME_LEN);
    cJSON_AddBoolToObject(data, CGI_ISG_DBISV4, sal_util_ipv4StrValidate_ret(db.server, SYS_IP_TYPE_UNICAST));
    cJSON_AddStringToObject(data, CGI_ISG_BKDBTFTPSERVER, db.server);
    cJSON_AddNumberToObject(data, CGI_ISG_BKDBWRDELAY, db.writeDelay);
    cJSON_AddNumberToObject(data, CGI_ISG_BKDBMINWRDELAY, BOND_IPSG_DB_WRITE_DELAY_MIN);
    cJSON_AddNumberToObject(data, CGI_ISG_BKDBMAXWRDELAY, BOND_IPSG_DB_WRITE_DELAY_MAX);
    cJSON_AddNumberToObject(data, CGI_ISG_BKDBDEFWRDELAY, DFLT_IPSG_DB_WRITE_DELAY);
    cJSON_AddNumberToObject(data, CGI_ISG_BKDBTIMEOUT, db.timeout);
    cJSON_AddNumberToObject(data, CGI_ISG_BKDBMINTIMEOUT, BOND_IPSG_DB_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_ISG_BKDBMAXTIMEOUT, BOND_IPSG_DB_TIMEOUT_MAX);
    cJSON_AddNumberToObject(data, CGI_ISG_BKDBDEFTIMEOUT, DFLT_IPSG_DB_TIMEOUT);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
