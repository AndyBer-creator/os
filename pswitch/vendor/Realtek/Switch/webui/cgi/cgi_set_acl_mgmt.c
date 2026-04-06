
#include <include/cgi.h>
#include <libsal/sal_acl_mgmt.h>

static int32 cgi_acl_mgmt_acl_set(void);
static int32 cgi_acl_mgmt_aclDel_set(void);
static int32 cgi_acl_mgmt_aclActive_set(void);
static int32 cgi_acl_mgmt_aclDeactive_set(void);
static int32 cgi_acl_mgmt_ace_set(void);
static int32 cgi_acl_mgmt_aceEdit_set(void);
static int32 cgi_acl_mgmt_aceDel_set(void);

const cgi_cmdmap_t cmdSetAclMgmtTbl[] =
{
    {CGI_CMD_ACL_MGMT_ACL,          cgi_acl_mgmt_acl_set},
    {CGI_CMD_ACL_MGMT_ACL_DEL,      cgi_acl_mgmt_aclDel_set},
    {CGI_CMD_ACL_MGMT_ACL_ACT,      cgi_acl_mgmt_aclActive_set},
    {CGI_CMD_ACL_MGMT_ACL_DEACT,    cgi_acl_mgmt_aclDeactive_set},
    {CGI_CMD_ACL_MGMT_ACE,          cgi_acl_mgmt_ace_set},
    {CGI_CMD_ACL_MGMT_ACEEDIT,      cgi_acl_mgmt_aceEdit_set},
    {CGI_CMD_ACL_MGMT_ACEDEL,       cgi_acl_mgmt_aceDel_set},

    {NULL, NULL}
};

static int32 cgi_acl_mgmt_acl_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;

    if (CGI_IS_PARAM_EXIST(CGI_NAME, value))
        CGI_SET_ERR_HDL(sal_acl_mgmt_list_add(value), ret);
    else
        goto cgi_end;

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_acl_mgmt_aclDel_set(void)
{
    uint32 len = 0;
    uint32 i;
    int32 ret = SYS_ERR_OK;
    char **array = NULL;

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_NAME, array, &len))
    {
    	for(i = 0; i < len; i++)
            CGI_SET_ENTRY_ERR_CONTINUE(sal_acl_mgmt_list_del(array[i]), array[i]);
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_acl_mgmt_aclActive_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;

    if (CGI_IS_PARAM_EXIST(CGI_NAME, value))
        CGI_SET_ERR_HDL(sal_acl_mgmt_class_set(value), ret);
    else
        goto cgi_end;

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_acl_mgmt_aclDeactive_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;

    if (CGI_IS_PARAM_EXIST(CGI_NAME, value))
        CGI_SET_ERR_HDL(sal_acl_mgmt_class_del(), ret);
    else
        goto cgi_end;

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_acl_mgmt_ace_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 ipType;
    char *value;
    char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1];
    sys_acl_mgmt_ace_t  stCareValue;
    sys_acl_mgmt_ace_priFlag_t stAcepriFlag;

    SYS_MEM_CLEAR(stCareValue);
    SYS_MEM_CLEAR(stAcepriFlag);

    if (CGI_IS_PARAM_EXIST(CGI_NAME, value))
        strlcpy(acListName, value, sizeof(acListName));
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_PRI, value))
        stCareValue.uiMgmtAcePri = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_SERVICE, value))
        stCareValue.stCare.uiService = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_ACTION, value))
        stCareValue.stAction = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);

        if (ipType == IPSOURCE_IPV4)
        {
            stCareValue.stCare.stCareFlag.careipv4 = 1;
            stCareValue.stCare.stCareFlag.careipv4mask = 1;

            if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_IPV4, value))
                sys_util_str2Ip(value, &stCareValue.stCare.stIpV4addr.uiSIpv4Addr);
            else
                goto cgi_end;

            if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_MASK, value))
                sys_util_str2Ip(value, &stCareValue.stCare.stIpV4addr.uiSIpv4Netmask);
            else
                goto cgi_end;
        }
        else if (ipType == IPSOURCE_IPV6)
        {
            stCareValue.stCare.stCareFlag.careipv6 = 1;

            if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_IPV6, value))
                inet_pton(AF_INET6, value, stCareValue.stCare.stIpV6addr.uaSIpv6Addr);
            else
                goto cgi_end;

            if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_PF, value))
                stCareValue.stCare.stIpV6addr.uiSIpv6PfLen = SYS_STR2UINT(value);
            else
                goto cgi_end;
        }
    }
    else
        goto cgi_end;

    CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_ACL_MGMT_MBRS, stCareValue.stCare.logicPortmask);

    CGI_SET_ERR_HDL(sal_acl_mgmt_ace_add(acListName, PRIFLAG_VALID,  &stCareValue), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_acl_mgmt_aceEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 ipType;
    char *value;
    char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1];
    sys_acl_mgmt_ace_t  stCareValue;
    sys_acl_mgmt_ace_priFlag_t stAcepriFlag;

    SYS_MEM_CLEAR(stCareValue);
    SYS_MEM_CLEAR(stAcepriFlag);

    if (CGI_IS_PARAM_EXIST(CGI_NAME, value))
        strlcpy(acListName, value, sizeof(acListName));
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_PRI, value))
        stCareValue.uiMgmtAcePri = SYS_STR2UINT(value);
    else
        goto cgi_end;

    sal_acl_mgmt_ace_del(acListName, stCareValue.uiMgmtAcePri);

    if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_SERVICE, value))
        stCareValue.stCare.uiService = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_ACTION, value))
        stCareValue.stAction = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);

        if (ipType == IPSOURCE_IPV4)
        {
            stCareValue.stCare.stCareFlag.careipv4 = 1;
            stCareValue.stCare.stCareFlag.careipv4mask = 1;

            if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_IPV4, value))
                sys_util_str2Ip(value, &stCareValue.stCare.stIpV4addr.uiSIpv4Addr);
            else
                goto cgi_end;

            if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_MASK, value))
                sys_util_str2Ip(value, &stCareValue.stCare.stIpV4addr.uiSIpv4Netmask);
            else
                goto cgi_end;
        }
        else if (ipType == IPSOURCE_IPV6)
        {
            stCareValue.stCare.stCareFlag.careipv6 = 1;

            if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_IPV6, value))
                inet_pton(AF_INET6, value, stCareValue.stCare.stIpV6addr.uaSIpv6Addr);
            else
                goto cgi_end;

            if (CGI_IS_PARAM_EXIST(CGI_ACL_MGMT_PF, value))
                stCareValue.stCare.stIpV6addr.uiSIpv6PfLen = SYS_STR2UINT(value);
            else
                goto cgi_end;
        }
    }
    else
        goto cgi_end;

    CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_ACL_MGMT_MBRS, stCareValue.stCare.logicPortmask);

    CGI_SET_ERR_HDL(sal_acl_mgmt_ace_add(acListName, PRIFLAG_VALID,  &stCareValue), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_acl_mgmt_aceDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i = 0;
    char **array = NULL;
    char *value =  NULL;

    if (!CGI_IS_PARAM_EXIST(CGI_NAME, value))
        goto cgi_end;

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_ACL_MGMT_PRI, array, &len))
    {
    	for(i = 0; i < len; i++)
    	{
    		CGI_SET_ENTRY_ERR_CONTINUE(sal_acl_mgmt_ace_del(value, SYS_STR2UINT(array[i])), array[i]);
    	}
    }

cgi_end:
    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

