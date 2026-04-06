
#include <include/cgi.h>
#include <libsal/sal_acl_usr.h>

static int32 cgi_acl_acl_set(void);
static int32 cgi_acl_aclDel_set(void);
static int32 cgi_acl_ace_set(void);
static int32 cgi_acl_aceDel_set(void);
static int32 cgi_acl_binding_set(void);
static int32 cgi_acl_bindingDel_set(void);

const cgi_cmdmap_t cmdSetAclTbl[] =
{
    {CGI_CMD_ACL_ACL,          cgi_acl_acl_set},
    {CGI_CMD_ACL_ACL_DEL,      cgi_acl_aclDel_set},
    {CGI_CMD_ACL_ACE,          cgi_acl_ace_set},
    {CGI_CMD_ACL_ACEDEL,       cgi_acl_aceDel_set},
    {CGI_CMD_ACL_BINDING,      cgi_acl_binding_set},
    {CGI_CMD_ACL_BINDING_DEL,  cgi_acl_bindingDel_set},
    {NULL, NULL}
};

static int32 cgi_acl_acl_set(void)
{
    int32 ret = SYS_ERR_OK;
    char  *value;
    sys_acl_type_t aclType;
    sys_acl_type_t oldtype;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_TYPE, value))
        aclType = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_NAME, value))
    {
        if (SYS_ERR_ACL_LIST_EMPTY != sal_acl_entry_typeByName_get(value, &oldtype))
        {
            ret = SYS_ERR_ACL_LIST_EXISTS;
            goto cgi_end;
        }

        CGI_SET_ERR_HDL(sal_acl_entry_create(value, aclType), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_acl_aclDel_set(void)
{
    int32 i;
    int32 ret = SYS_ERR_OK;
    uint32 len;
    char **array = NULL;
    sys_acl_type_t type;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_NAME, array, &len))
    {
        for (i = 0; i < len; ++i)
        {
            sal_acl_entry_typeByName_get(array[i], &type);
            CGI_SET_ENTRY_ERR_CONTINUE(sal_acl_entry_del(array[i], type), array[i]);
        }
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 _cgi_acl_aceMacField_set(sys_ace_content_t *ace)
{
    sys_ace_mac_t *mac = NULL;
    char tmp[SYS_BUF32_LEN];
    char *value = NULL;

    mac = &(ace->mac);

    if (CGI_IS_PARAM_EXIST(CGI_ACL_SMAC, value))
    {
        mac->isCareSmac = TRUE;
        sys_util_str2Mac(value, mac->smac.octet);

        if (CGI_IS_PARAM_EXIST(CGI_ACL_SMASK, value))
            sys_util_str2Mac(value, mac->smacMask.octet);
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_DMAC, value))
    {
        mac->isCareDmac = TRUE;
        sys_util_str2Mac(value, mac->dmac.octet);

        if (CGI_IS_PARAM_EXIST(CGI_ACL_DMASK, value))
            sys_util_str2Mac(value, mac->dmacMask.octet);
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_ETHTYPE, value))
    {
        sprintf(tmp, "0x%s", value);
        mac->ethType = SYS_STR2UINT(tmp);
        mac->isCareEthtype = TRUE;
    }

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
    {
        mac->isCareVid = TRUE;
        mac->vid = SYS_STR2UINT(value);
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_DOT1P, value))
    {
        mac->isCare8021p = TRUE;
        mac->value8021p = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_ACL_DOT1PMASK, value))
            mac->mask8021p = SYS_STR2UINT(value);
    }

    return SYS_ERR_OK;
}

static int32 _cgi_acl_aceIpField_set(sys_acl_type_t aclType, sys_ace_content_t *ace)
{
    sys_ace_ip_t *ip = NULL;
    uint32 i;
    char *flagStr[] = {"fin", "syn", "rst", "psh", "ack", "urg"};
    char *value;

    ip = &(ace->ip);

    if (SYS_ACL_IPV4_BASE == aclType)
        ip->isV4 = TRUE;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_PROTOCOL"_1", value) || CGI_IS_PARAM_EXIST(CGI_ACL_PROTOCOL"_2", value))
    {
        ip->protol = SYS_STR2UINT(value);
        ip->isCareProto = TRUE;
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_SIP, value))
    {
        if (SYS_ACL_IPV4_BASE == aclType)
        {
            sys_util_str2Ip(value, &ip->v4sip);

            if (CGI_IS_PARAM_EXIST(CGI_ACL_SMASK, value))
                sys_util_str2Ip(value, &ip->v4sipMask);
            else
                return SYS_ERR_INPUT;

        }
        else
        {
            struct sockaddr_in6 sin6_dummy;

            osal_memset(&sin6_dummy, 0, sizeof(sin6_dummy));

            inet_pton(AF_INET6, value, &sin6_dummy.sin6_addr);
            osal_memcpy(&ip->v6sip, &sin6_dummy.sin6_addr, sizeof(sin6_dummy.sin6_addr));

            if (CGI_IS_PARAM_EXIST(CGI_ACL_SMASK, value))
                ip->v6sip_prefix = SYS_STR2UINT(value);
            else
                return SYS_ERR_INPUT;

            //        ip->v6sip.hex[1],ip->v6sip.hex[2],ip->v6sip.hex[3],ip->v6sip.hex[4],ip->v6sip.hex[5],
            //        ip->v6sip.hex[6],ip->v6sip.hex[7],ip->v6sip.hex[8],ip->v6sip.hex[9],ip->v6sip.hex[10],
            //        ip->v6sip.hex[11],ip->v6sip.hex[12],ip->v6sip.hex[13],ip->v6sip.hex[14],ip->v6sip.hex[15],
            
        }

        ip->isCareSip = TRUE;
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_DIP, value))
    {
        if (SYS_ACL_IPV4_BASE == aclType)
        {
            sys_util_str2Ip(value, &ip->v4dip);

            if (CGI_IS_PARAM_EXIST(CGI_ACL_DMASK, value))
                sys_util_str2Ip(value, &ip->v4dipMask);
            else
                return SYS_ERR_INPUT;

        }
        else
        {
            struct sockaddr_in6 sin6_dummy;

            osal_memset(&sin6_dummy, 0, sizeof(sin6_dummy));

            inet_pton(AF_INET6, value, &sin6_dummy.sin6_addr);
            osal_memcpy(&ip->v6dip, &sin6_dummy.sin6_addr, sizeof(sin6_dummy.sin6_addr));

            if (CGI_IS_PARAM_EXIST(CGI_ACL_DMASK, value))
                ip->v6dip_prefix = SYS_STR2UINT(value);
            else
                return SYS_ERR_INPUT;

            //        ip->v6dip.hex[1],ip->v6dip.hex[2],ip->v6dip.hex[3],ip->v6dip.hex[4],ip->v6dip.hex[5],
            //        ip->v6dip.hex[6],ip->v6dip.hex[7],ip->v6dip.hex[8],ip->v6dip.hex[9],ip->v6dip.hex[10],
            //        ip->v6dip.hex[11],ip->v6dip.hex[12],ip->v6dip.hex[13],ip->v6dip.hex[14],ip->v6dip.hex[15],
            
        }

        ip->isCareDip = TRUE;
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_DSCP, value))
    {
        ip->dscp = SYS_STR2UINT(value);
        ip->isCareDscp = TRUE;
        
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_IPP, value))
    {
        ip->ipPrecedence = SYS_STR2UINT(value);
        ip->isCareIpPrecedence = TRUE;
        
    }

    switch (ip->protol)
    {
        case 1:
        case 58: 
            if (CGI_IS_PARAM_EXIST(CGI_ACL_ICMP_TYPE"_1", value) || CGI_IS_PARAM_EXIST(CGI_ACL_ICMP_TYPE"_2", value))
            {
                ip->icmpType = SYS_STR2UINT(value);
                ip->isCareICMPType = TRUE;
                
            }

            if (CGI_IS_PARAM_EXIST(CGI_ACL_ICMP_CODE, value))
            {
                ip->icmpCode = SYS_STR2UINT(value);
                ip->isCareICMPCode = TRUE;
                
            }
            break;
        case 6:
            
            for (i = 0; i <= SYS_ACL_TCP_FLAG_URG; ++i)
            {
                if (CGI_IS_PARAM_EXIST(flagStr[i], value))
                {
                    if (0 == SYS_STR2UINT(value))
                    {
                        ip->tcpflag |= (1 << i);
                        ip->tcpflagMask |= (1 << i);
                    }
                    else if (1 == SYS_STR2UINT(value))
                    {
                        ip->tcpflagMask |= (1 << i);
                    }

                    if (0 != ip->tcpflagMask)
                        ip->isCareTcpFlag = TRUE;
                }
            }
            
            break;
    }

    if (6 == ip->protol || 17 == ip->protol)
    {
        if (CGI_IS_PARAM_EXIST(CGI_ACL_SPORT_S"_2", value))
        {
            sys_l4port_range_t portRange;
            SYS_MEM_CLEAR(portRange);

            portRange.portStart = SYS_STR2UINT(value);

            if (CGI_IS_PARAM_EXIST(CGI_ACL_SPORT_E"_2", value))
                 portRange.portEnd = SYS_STR2UINT(value);
            else
                return SYS_ERR_INPUT;

            portRange.direct = RNGCHK_L4PORT_DIRECTION_SRC;
            SYS_ERR_CHK(sal_acl_l4port_range_set(&portRange, &ip->l4SrcPortRangeIdx));
            ip->isCareSrcPortRange = TRUE;
            
        }
        else if (CGI_IS_PARAM_EXIST(CGI_ACL_SPORT_S"_1", value))
        {
            ip->srcPort = SYS_STR2UINT(value);
            ip->isCareSrcPort = TRUE;
            
        }

        if (CGI_IS_PARAM_EXIST(CGI_ACL_DPORT_S"_2", value))
        {
            sys_l4port_range_t portRange;
            SYS_MEM_CLEAR(portRange);

            portRange.portStart = SYS_STR2UINT(value);

            if (CGI_IS_PARAM_EXIST(CGI_ACL_DPORT_E"_2", value))
                 portRange.portEnd = SYS_STR2UINT(value);
            else
                return SYS_ERR_INPUT;

            portRange.direct = RNGCHK_L4PORT_DIRECTION_DST;
            SYS_ERR_CHK(sal_acl_l4port_range_set(&portRange, &ip->l4DstPortRangeIdx));
            ip->isCareDstPortRange = TRUE;
            
        }
        else if (CGI_IS_PARAM_EXIST(CGI_ACL_DPORT_S"_1", value))
        {
            ip->dstPort = SYS_STR2UINT(value);
            ip->isCareDstPort = TRUE;
            
        }
    }

    return SYS_ERR_OK;
}

static int32 cgi_acl_ace_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 isBind = FALSE;
    char  *value = NULL;
    sys_acl_type_t aclType = SYS_ACL_TYPE_NONE;
    sys_namestr_t selectName;
    sys_ace_content_t ace;

    SYS_MEM_CLEAR(selectName);
    SYS_MEM_CLEAR(ace);

    if (CGI_IS_PARAM_EXIST(CGI_NAME, value))
    {
        osal_sprintf(selectName.str, "%s", value);
        sal_acl_entry_typeByName_get(selectName.str, &aclType);
    }

    sal_acl_isACLBind_ret(&selectName, &isBind);

    if (TRUE == isBind)
    {
        ret = SYS_ERR_ACL_LIST_BINDING;
        goto cgi_end;
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_PRI, value))
        ace.priority = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_ACL_ACTION, value))
    {
        switch (SYS_STR2UINT(value))
        {
            case 0:
                ace.action = SYS_PKT_ACTION_FORWARD;
                break;
            case 1:
                ace.action = SYS_PKT_ACTION_DROP;
                break;
            case 2:
                ace.action = SYS_PKT_ACTION_SHUTDOWN_PORT;
                break;
            default:
                goto cgi_end;
        }
    }
    else
        goto cgi_end;

    if (SYS_ACL_MAC_BASE == aclType)
        CGI_SET_ERR_HDL(_cgi_acl_aceMacField_set(&ace), ret);
    else if (SYS_ACL_IPV4_BASE == aclType)
        CGI_SET_ERR_HDL(_cgi_acl_aceIpField_set(aclType, &ace), ret);
    else if (SYS_ACL_IPV6_BASE == aclType)
        CGI_SET_ERR_HDL(_cgi_acl_aceIpField_set(aclType, &ace), ret);

    if (CGI_IS_PARAM_EXIST(CGI_TXT, value))
        CGI_SET_ERR_HDL(sal_ace_entry_modify(selectName.str, &ace), ret);
    else
        CGI_SET_ERR_HDL(sal_ace_entry_create(selectName.str, &ace), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_acl_aceDel_set(void)
{
    int8 i;
    int32 ret = SYS_ERR_OK;
    uint32 len;
    char  *value;
    char **array = NULL;

    if (!CGI_IS_PARAM_EXIST(CGI_NAME, value))
        goto cgi_end;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_ACL_PRI, array, &len))
    {
        for (i = 0; i < len; ++i)
        {
            CGI_SET_ENTRY_ERR_CONTINUE(sal_ace_entry_del(value, SYS_STR2UINT(array[i])), array[i]);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_acl_binding_set(void)
{
    int8 i;
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_logic_portmask_t lpm;
    sys_acl_bind_names_t names;

    SYS_MEM_CLEAR(lpm);
    SYS_MEM_CLEAR(names);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    for (i = SYS_ACL_MAC_BASE; i < SYS_ACL_TYPE_END ; i++)
        CGI_SET_ERR_HDL(sal_acl_entry_unbind(i, lpm),ret);

    if (CGI_IS_PARAM_EXIST(CGI_ACL_MAC, value) && 0 != strcmp(value, "None"))
    {
        strlcpy(names.aclname[SYS_ACL_MAC_BASE].str, value, sizeof(names.aclname[SYS_ACL_MAC_BASE].str));
        names.type = SYS_ACL_MAC_BASE;
        CGI_SET_ERR_HDL(sal_acl_entry_bind(&names, lpm), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_IPV4, value) && 0 != strcmp(value, "None"))
    {
        strlcpy(names.aclname[SYS_ACL_IPV4_BASE].str, value, sizeof(names.aclname[SYS_ACL_IPV4_BASE].str));
        names.type = SYS_ACL_IPV4_BASE;
        CGI_SET_ERR_HDL(sal_acl_entry_bind(&names, lpm), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_ACL_IPV6, value) && 0 != strcmp(value, "None"))
    {
        strlcpy(names.aclname[SYS_ACL_IPV6_BASE].str, value, sizeof(names.aclname[SYS_ACL_IPV6_BASE].str));
        names.type = SYS_ACL_IPV6_BASE;
        CGI_SET_ERR_HDL(sal_acl_entry_bind(&names, lpm), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_acl_bindingDel_set(void)
{
    int8 i;
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_logic_portmask_t lpm;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    for (i = SYS_ACL_MAC_BASE; i < SYS_ACL_TYPE_END ; i++)
        CGI_SET_ENTRY_ERR_CONTINUE(sal_acl_entry_unbind(i, lpm), "Unbind");

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
