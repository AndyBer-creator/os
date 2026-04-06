
#include <include/cgi.h>
#include <libsal/sal_acl_mgmt.h>

static int32 cgi_acl_mgmt_get(void);
static int32 cgi_acl_mgmt_ace_get(void);

const cgi_cmdmap_t cmdGetAclMgmtTbl[] =
{
    {CGI_CMD_ACL_MGMT_ACL,             cgi_acl_mgmt_get},
    {CGI_CMD_ACL_MGMT_ACE,             cgi_acl_mgmt_ace_get},
    {NULL, NULL}
};

static int32 cgi_acl_mgmt_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
	cJSON *acls = NULL;
    cJSON *acl = NULL;
    uint32 uiAceNum = 0;
    uint32 uiListNum = 0;
    uint32 i;
    char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1];
    char activeName[SYS_MGMT_ACL_NAME_LENTH_MAX+1];

	CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_MAXLEN, SYS_MGMT_ACL_NAME_LENTH_MAX);

	acls = cJSON_CreateArray();

    SYS_MEM_CLEAR(activeName);
    sal_acl_mgmt_list_num_get( &uiListNum);
    sal_acl_mgmt_class_get(activeName);

    for (i = 0; i < uiListNum; i++)
    {
        acl = cJSON_CreateObject();

        SYS_MEM_CLEAR(acListName);

        SYS_ERR_CHK(sal_acl_mgmt_name_get(acListName,i));
        SYS_ERR_CHK(sal_acl_mgmt_list_acenum_get(acListName, &uiAceNum));

        if (0 != osal_strlen(activeName) && 0 == osal_strcmp(activeName, acListName))
        	cJSON_AddBoolToObject(acl, CGI_ACL_MGMT_ACTIVE, TRUE);
        else
        	cJSON_AddBoolToObject(acl, CGI_ACL_MGMT_ACTIVE, FALSE);

        cJSON_AddStringToObject(acl, CGI_NAME, acListName);
    	cJSON_AddNumberToObject(acl, CGI_ACL_MGMT_ACENUM, uiAceNum);
	    cJSON_AddItemToArray(acls, acl);
    }

    if (0 != uiListNum)
        cJSON_AddItemToObject(data, CGI_ACL_MGMT_ACLS, acls);
    else
    {
        cJSON_Delete(acls);
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_acl_mgmt_ace_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
	cJSON *acls = NULL;
    cJSON *acl = NULL;
    cJSON *aces = NULL;
    cJSON *ace = NULL;
    cJSON *port = NULL;
    cJSON *notMbrs = NULL;
    cJSON *mbrs = NULL;
    uint32 uiAceNum = 0;
    uint32 uiListNum = 0;
    uint32 i;
    char *value;
    char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1];
    char activeName[SYS_MGMT_ACL_NAME_LENTH_MAX+1];
    char selectName[SYS_MGMT_ACL_NAME_LENTH_MAX+1];
    char ipStr[CAPA_IPV6_STR_LEN];
    char strPortmask[CAPA_PORTMASK_STR_LEN];
    char portStr[CAPA_PORT_STR_LEN + 1];
    sys_acl_mgmt_ace_t  stacl_mgmt_ace;
    sys_logic_portmask_t lpmMbrs;
    sys_logic_portmask_t lpmNotMbrs;
    sys_logic_port_t lp;

	CGI_GET_JSON_CREATE(root, data);
	acls = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_ACL_MGMT_ACLS, acls);

    SYS_MEM_CLEAR(activeName);
    SYS_MEM_CLEAR(selectName);
    sal_acl_mgmt_list_num_get( &uiListNum);

    if (0 == uiListNum)
    {
        acl = cJSON_CreateObject();
        cJSON_AddStringToObject(acl, CGI_NAME, "None");
        cJSON_AddStringToObject(acl, CGI_VALUE, "None");
        cJSON_AddItemToArray(acls, acl);

        CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

        return SYS_ERR_OK;
    }

    if (!CGI_IS_PARAM_EXIST(CGI_NAME, value))
        sal_acl_mgmt_name_get(selectName, 0);
    else
        strlcpy(selectName, value, sizeof(selectName));

    sal_acl_mgmt_class_get(activeName);
    sal_acl_mgmt_list_acenum_get(selectName,&uiAceNum);

    cJSON_AddStringToObject(data, CGI_NAME, selectName);

    if (0 != osal_strlen(activeName) && 0 == osal_strcmp(activeName, selectName))
        cJSON_AddBoolToObject(data, CGI_ACL_MGMT_ACTIVE, TRUE);
    else
        cJSON_AddBoolToObject(data, CGI_ACL_MGMT_ACTIVE, FALSE);

    for (i = 0; i < uiListNum; i++)
    {
        acl = cJSON_CreateObject();

        SYS_MEM_CLEAR(acListName);
        SYS_ERR_CHK(sal_acl_mgmt_name_get(acListName,i));
        cJSON_AddStringToObject(acl, CGI_NAME, acListName);
        cJSON_AddStringToObject(acl, CGI_VALUE, acListName);
	    cJSON_AddItemToArray(acls, acl);
    }

    if (uiAceNum == 0)
    {
        
        CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

        return SYS_ERR_OK;
    }

    aces = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_ACL_MGMT_ACES, aces);

    for (i = 0; i < uiAceNum; i++)
    {
        ace = cJSON_CreateObject();

        SYS_MEM_CLEAR(stacl_mgmt_ace);

        sal_acl_mgmt_ace_get(selectName, i, &stacl_mgmt_ace);

        cJSON_AddNumberToObject(ace, CGI_ACL_MGMT_PRI, stacl_mgmt_ace.uiMgmtAcePri);
        cJSON_AddNumberToObject(ace, CGI_ACL_MGMT_SERVICE, stacl_mgmt_ace.stCare.uiService);
        cJSON_AddNumberToObject(ace, CGI_ACL_MGMT_ACTION, stacl_mgmt_ace.stAction);

        //CGI_DBG("%u %u %u", stacl_mgmt_ace.uiMgmtAcePri, stacl_mgmt_ace.stCare.uiService, stacl_mgmt_ace.stAction);

        SYS_MEM_CLEAR(strPortmask);
        LPM2WEBSTR(&(stacl_mgmt_ace.stCare.logicPortmask), strPortmask);
        cJSON_AddStringToObject(ace, CGI_PORTLIST, strPortmask);

        if ( 1 == stacl_mgmt_ace.stCare.stCareFlag.careipv4)
        {
            cJSON_AddNumberToObject(ace, CGI_ACL_MGMT_IPTYPE, IPSOURCE_IPV4);

            SYS_MEM_CLEAR(ipStr);
            sys_util_ip2Str(stacl_mgmt_ace.stCare.stIpV4addr.uiSIpv4Addr, ipStr);
            cJSON_AddStringToObject(ace, CGI_ACL_MGMT_IP, ipStr);

            SYS_MEM_CLEAR(ipStr);
            sys_util_ip2Str(stacl_mgmt_ace.stCare.stIpV4addr.uiSIpv4Netmask, ipStr);
            cJSON_AddStringToObject(ace, CGI_ACL_MGMT_MASK, ipStr);

        }
        else if ( 1 == stacl_mgmt_ace.stCare.stCareFlag.careipv6)
        {
            cJSON_AddNumberToObject(ace, CGI_ACL_MGMT_IPTYPE, IPSOURCE_IPV6);

            SYS_MEM_CLEAR(ipStr);
            inet_ntop(AF_INET6, stacl_mgmt_ace.stCare.stIpV6addr.uaSIpv6Addr,  ipStr, sizeof(ipStr));
            cJSON_AddStringToObject(ace, CGI_ACL_MGMT_IP, ipStr);
            cJSON_AddNumberToObject(ace, CGI_ACL_MGMT_MASK, stacl_mgmt_ace.stCare.stIpV6addr.uiSIpv6PfLen);
        }
        else
            cJSON_AddNumberToObject(ace, CGI_ACL_MGMT_IPTYPE, IPSOURCE_ALL);

        SYS_MEM_CLEAR(lpmMbrs);
        SYS_MEM_CLEAR(lpmNotMbrs);

        LOGIC_PORTMASK_SET_ALL(lpmNotMbrs);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, stacl_mgmt_ace.stCare.logicPortmask)
        {
            LOGIC_PORTMASK_CLEAR_PORT(lpmNotMbrs, lp);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(lpmNotMbrs))
        {
            notMbrs = cJSON_CreateArray();
            cJSON_AddItemToObject(ace, CGI_ACL_MGMT_NOTMBRS, notMbrs);

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpmNotMbrs)
            {
                SYS_MEM_CLEAR(portStr);
                LP2WEBSTR(lp, portStr);

                port = cJSON_CreateObject();
                cJSON_AddStringToObject(port, CGI_PORT, portStr);
                cJSON_AddItemToArray(notMbrs, port);
            }
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(stacl_mgmt_ace.stCare.logicPortmask))
        {
            mbrs = cJSON_CreateArray();

            cJSON_AddItemToObject(ace, CGI_ACL_MGMT_MBRS, mbrs);

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, stacl_mgmt_ace.stCare.logicPortmask)
            {
                SYS_MEM_CLEAR(portStr);
                LP2WEBSTR(lp, portStr);

                port = cJSON_CreateObject();
                cJSON_AddStringToObject(port, CGI_PORT, portStr);
                cJSON_AddItemToArray(mbrs, port);
            }
        }

	    cJSON_AddItemToArray(aces, ace);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
