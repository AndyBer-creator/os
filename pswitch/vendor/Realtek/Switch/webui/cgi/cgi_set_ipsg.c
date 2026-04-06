
#include <include/cgi.h>

static int32 cgi_isg_portEdit_set(void);
static int32 cgi_isg_binding_set(void);
static int32 cgi_isg_bindDel_set(void);
static int32 cgi_ipsg_database_set(void);

const cgi_cmdmap_t cmdSetIsgTbl[] =
{
    {CGI_CMD_IPSG_PORTEDIT,         cgi_isg_portEdit_set},
    {CGI_CMD_IPSG_BINDING,          cgi_isg_binding_set},
    {CGI_CMD_IPSG_BINDINGDEL,       cgi_isg_bindDel_set},
    {CGI_CMD_IPSG_DATABASE,         cgi_ipsg_database_set},
    {NULL, NULL}
};

static int32 cgi_isg_portEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    int32 num = IPSG_ENTRY_UNLIMITED;
    char *value = NULL;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    sys_enable_t enable = DISABLED;
    sys_isg_verify_type_t verifySrcMac = ISG_VERIFY_IP;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        if (CGI_IS_PARAM_EXIST(CGI_ISG_STATUS, value))
            enable = ENABLED;

        if (CGI_IS_PARAM_EXIST(CGI_ISG_VERIFY_SRCMAC, value))
            verifySrcMac = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_ISG_MAXENTRY, value))
        {
            num = SYS_STR2UINT(value);
            if(0 == num)
                num = IPSG_ENTRY_UNLIMITED;
        }

        CGI_SET_PORT_ERR_CONTINUE(sal_isg_port_set(lp, enable, verifySrcMac), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_isg_portMaxEntryNum_set(lp, num), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_isg_binding_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_isg_entry_t entry;

    SYS_MEM_CLEAR(entry);

    if (CGI_IS_PARAM_EXIST(CGI_PORT, value))
        STR2LP(value, &entry.port);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ISG_VLAN, value))
        entry.vid  = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ISG_MAC, value))
    {
        sys_util_str2Mac(value, entry.mac.octet);
        entry.isCareMac = TRUE;
        entry.mask = 0xffffffff;
    }
    else if (CGI_IS_PARAM_EXIST(CGI_ISG_IPMASK, value))
        sys_util_str2Ip(value, &entry.mask);

    if (CGI_IS_PARAM_EXIST(CGI_ISG_IP, value))
        sys_util_str2Ip(value, &entry.ip);

    entry.type = ISG_ENTRY_STATIC;
    entry.isUsed = TRUE;

    CGI_SET_ERR_HDL(sal_isg_bind_entry_set(&entry), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_isg_bindDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i;
    char **array = NULL;
    char *flag = "_";
    char *value =  NULL;
    char *tmp =  NULL;
    sys_isg_entry_t entry;

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array, &len))
    {
    	for(i = 0; i < len; i++)
    	{
            SYS_MEM_CLEAR(entry);

            tmp = strstr(array[i], "__");

            value = osal_strtok(array[i], flag);
    		value = osal_strtok(NULL, flag);

    		if (tmp != NULL)
    		{
    		    entry.vid  = SYS_STR2UINT(value);

    		    value = osal_strtok(NULL, flag);
    		    sys_util_str2Ip(value, &entry.ip);
    		}
    		else
    		{
        		sys_util_str2Mac(value, entry.mac.octet);

        		value = osal_strtok(NULL, flag);
        		entry.vid  = SYS_STR2UINT(value);

    	    	value = osal_strtok(NULL, flag);
    		    sys_util_str2Ip(value, &entry.ip);
            }

            entry.type = ISG_ENTRY_STATIC;

    		CGI_SET_ENTRY_ERR_CONTINUE(sal_isg_bind_entry_del(&entry), "Entry");
    	}
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_ipsg_database_set(void)
{
    char   *pStrValue;
    int32 ret = SYS_ERR_OK;
    sys_isg_db_t db;

    SYS_MEM_CLEAR(db);

    if (CGI_IS_PARAM_EXIST(CGI_ISG_BKDBTYPE, pStrValue))
        db.dbType = SYS_STR2UINT(pStrValue);
    else
        goto cgi_end;

    if (ISG_DB_TYPE_NONE != db.dbType)
    {
        SYS_ERR_CHK(sal_isg_db_clear());

        if (CGI_IS_PARAM_EXIST(CGI_ISG_BKDBFILENAME, pStrValue))
        {
            SYS_MEM_CLEAR(db.filename);
            osal_sprintf(db.filename, "%s", pStrValue);
        }

        if (CGI_IS_PARAM_EXIST(CGI_ISG_BKDBTFTPSERVER, pStrValue))
        {
            SYS_MEM_CLEAR(db.server);
            osal_sprintf(db.server, "%s", pStrValue);
        }

        if ( ISG_DB_TYPE_FLASH == db.dbType )
        {
            SYS_MEM_CLEAR(db.filename);
            SYS_MEM_CLEAR(db.server);
            osal_sprintf(db.filename, "isg_db_wanted");
        }
    }
    else
    {
        SYS_ERR_CHK(sal_isg_db_clear());
        SYS_MEM_CLEAR(db.filename);
    }

    if (CGI_IS_PARAM_EXIST(CGI_ISG_BKDBWRDELAY, pStrValue))
        db.writeDelay= SYS_STR2UINT(pStrValue);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ISG_BKDBTIMEOUT, pStrValue))
        db.timeout= SYS_STR2UINT(pStrValue);
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_isg_db_set(&db), ret);
    ISG_DB_BACKUP();

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
