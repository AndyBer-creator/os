
#include <include/cgi.h>
#include <libsal/sal_acl_usr.h>

static int32 cgi_acl_acl_get(void);
static int32 cgi_acl_ace_get(void);
static int32 cgi_acl_binding_get(void);

const cgi_cmdmap_t cmdGetAclTbl[] =
{
    {CGI_CMD_ACL_ACL,       cgi_acl_acl_get},
    {CGI_CMD_ACL_ACE,       cgi_acl_ace_get},
    {CGI_CMD_ACL_BINDING,   cgi_acl_binding_get},
    {NULL, NULL}
};

static int32 cgi_acl_acl_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *acls = NULL;
    cJSON *acl = NULL;
    char tmp[CAPA_PORTMASK_STR_LEN];
    char *value;
    uint32 i;
    uint32 aclcnt = 0;
    uint32 acecnt = 0;
    sys_namestr_t name;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_acl_bind_names_t names[SYS_LOGICPORT_NUM_MAX];
    sys_acl_type_t aclType = SYS_ACL_MAC_BASE;

    SYS_MEM_CLEAR(names);

    if (CGI_IS_PARAM_EXIST(CGI_ACL_TYPE, value))
        aclType = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_MAXLEN, CAPA_NAME_STR_LEN);

    sal_acl_entry_countByType_get(aclType, &aclcnt);
    sal_acl_entry_portBinding_get(names);

    if (0 != aclcnt)
    {
        acls = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_ACL_ACLS, acls);

        for (i = 0; i < aclcnt; ++i)
        {
            acl = cJSON_CreateObject();
            cJSON_AddItemToArray(acls, acl);

            SYS_MEM_CLEAR(lpm);
            SYS_MEM_CLEAR(name);
            SYS_MEM_CLEAR(tmp);
            sal_acl_entry_nameByIdx_get(name.str, aclType, i);
            sal_ace_entry_count_get(name.str, aclType, &acecnt);

            FOR_EACH_LOGIC_PORT(lp)
            {
                if (0 == osal_strcmp(names[lp].aclname[aclType].str, name.str))
                {
                    LOGIC_PORTMASK_SET_PORT(lpm, lp);
                }
            }

            LPM2STR(&lpm, tmp);

            cJSON_AddStringToObject(acl, CGI_NAME, name.str);
            cJSON_AddStringToObject(acl, CGI_ACL_BINDING, tmp);
            cJSON_AddNumberToObject(acl, CGI_ACL_ACENUM, acecnt);
        }
    }
    else
    {
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 _cgi_acl_aceMacField_get(cJSON *ace, sys_ace_content_t *ace_content)
{
    sys_ace_mac_t *mac = NULL;
    char tmp[SYS_BUF32_LEN];

    mac = &ace_content->mac;

    if (TRUE == mac->isCareDmac)
    {
        SYS_MEM_CLEAR(tmp);
        SYS_ERR_CHK(sys_util_mac2Str(mac->dmac.octet, tmp));
        cJSON_AddStringToObject(ace, CGI_ACL_DMAC, tmp);

        SYS_MEM_CLEAR(tmp);
        SYS_ERR_CHK(sys_util_mac2Str(mac->dmacMask.octet, tmp));
        cJSON_AddStringToObject(ace, CGI_ACL_DMASK, tmp);
    }
    else
    {
        cJSON_AddStringToObject(ace, CGI_ACL_DMAC, "Any");
        cJSON_AddStringToObject(ace, CGI_ACL_DMASK, "Any");
    }

    if (TRUE == mac->isCareSmac)
    {
        SYS_MEM_CLEAR(tmp);
        SYS_ERR_CHK(sys_util_mac2Str(mac->smac.octet, tmp));
        cJSON_AddStringToObject(ace, CGI_ACL_SMAC, tmp);

        SYS_MEM_CLEAR(tmp);
        SYS_ERR_CHK(sys_util_mac2Str(mac->smacMask.octet, tmp));
        cJSON_AddStringToObject(ace, CGI_ACL_SMASK, tmp);
    }
    else
    {
        cJSON_AddStringToObject(ace, CGI_ACL_SMAC, "Any");
        cJSON_AddStringToObject(ace, CGI_ACL_SMASK, "Any");
    }

    if (TRUE == mac->isCareEthtype)
    {
        SYS_MEM_CLEAR(tmp);
        sprintf(tmp, "%X", mac->ethType);
        cJSON_AddStringToObject(ace, CGI_ACL_ETHTYPE, tmp);
    }
    else
        cJSON_AddStringToObject(ace, CGI_ACL_ETHTYPE, "Any");

    if (TRUE == mac->isCareVid)
        cJSON_AddNumberToObject(ace, CGI_VLAN, mac->vid);
    else
        cJSON_AddStringToObject(ace, CGI_VLAN, "Any");

    if (TRUE == mac->isCare8021p)
    {
        cJSON_AddNumberToObject(ace, CGI_ACL_DOT1P, mac->value8021p);
        cJSON_AddNumberToObject(ace, CGI_ACL_DOT1PMASK, mac->mask8021p);
    }
    else
    {
        cJSON_AddStringToObject(ace, CGI_ACL_DOT1P, "Any");
        cJSON_AddStringToObject(ace, CGI_ACL_DOT1PMASK, "Any");
    }

    return SYS_ERR_OK;
}

static int32 _cgi_acl_aceIpField_get(cJSON *ace, sys_ace_content_t *ace_content)
{
    sys_ace_ip_t *ip = NULL;
    char tmp[SYS_BUF64_LEN];
    uint16 j;

    ip = &ace_content->ip;

#define V4IP2STR(_ip)                       \
do{                                         \
    osal_memset(tmp, 0, sizeof(tmp));       \
    SYS_ERR_CHK(sys_util_ip2Str(_ip, tmp)); \
}while(0);

#define V6IP2STR(_ip)                       \
do{                                         \
    osal_memset(tmp, 0, sizeof(tmp));       \
    inet_ntop(AF_INET6, _ip, tmp, sizeof(tmp));  \
}while(0);

    if (ip->isCareSip)
    {
        if (ip->isV4)
        {
            V4IP2STR(ip->v4sip);
            cJSON_AddStringToObject(ace, CGI_ACL_SIP, tmp);
            V4IP2STR(ip->v4sipMask);
            cJSON_AddStringToObject(ace, CGI_ACL_SMASK, tmp);
        }
        else
        {
            V6IP2STR(ip->v6sip.hex);
            cJSON_AddStringToObject(ace, CGI_ACL_SIP, tmp);
            cJSON_AddNumberToObject(ace, CGI_ACL_SMASK, ip->v6sip_prefix);
        }
    }
    else
    {
        cJSON_AddStringToObject(ace, CGI_ACL_SIP, "Any");
        cJSON_AddStringToObject(ace, CGI_ACL_SMASK, "Any");
    }

    if (ip->isCareDip)
    {
        if (ip->isV4)
        {
            V4IP2STR(ip->v4dip);
            cJSON_AddStringToObject(ace, CGI_ACL_DIP, tmp);
            V4IP2STR(ip->v4dipMask);
            cJSON_AddStringToObject(ace, CGI_ACL_DMASK, tmp);
        }
        else
        {
            V6IP2STR(ip->v6dip.hex);
            cJSON_AddStringToObject(ace, CGI_ACL_DIP, tmp);
            cJSON_AddNumberToObject(ace, CGI_ACL_DMASK, ip->v6dip_prefix);
        }
    }
    else
    {
        cJSON_AddStringToObject(ace, CGI_ACL_DIP, "Any");
        cJSON_AddStringToObject(ace, CGI_ACL_DMASK, "Any");
    }

    if (TRUE == ip->isCareProto)
    {
        cJSON_AddNumberToObject(ace, CGI_ACL_PROTOCOL, ip->protol);

        switch (ip->protol)
        {
            case 1:
                if (!ip->isV4)
                    break;

                if (TRUE == ip->isCareICMPType)
                    cJSON_AddNumberToObject(ace, CGI_ACL_ICMP_TYPE, ip->icmpType);
                else
                    cJSON_AddStringToObject(ace, CGI_ACL_ICMP_TYPE, "Any");

                if (TRUE == ip->isCareICMPCode)
                    cJSON_AddNumberToObject(ace, CGI_ACL_ICMP_CODE, ip->icmpCode);
                else
                    cJSON_AddStringToObject(ace, CGI_ACL_ICMP_CODE, "Any");

                break;
            case 6:
                {
                    int k;
                    
                    SYS_MEM_CLEAR(tmp);
                    for (k = 5; k >= 0; --k)
                    {
                        j = 6 - k - 1;
                        if (ip->tcpflagMask & (1 << k))
                        {
                            if (ip->tcpflag & (1 << k))
                                sprintf(&tmp[j], "%d", 1);
                            else
                                sprintf(&tmp[j], "%d", 0);
                        }
                        else
                            sprintf(&tmp[j], "%s", "x");
                    }

                    cJSON_AddStringToObject(ace, CGI_ACL_TCP_FLAG, tmp);
                }

                break;
            case 58:
                if (ip->isV4)
                    break;

                if (TRUE == ip->isCareICMPType)
                    cJSON_AddNumberToObject(ace, CGI_ACL_ICMP_TYPE, ip->icmpType);
                else
                    cJSON_AddStringToObject(ace, CGI_ACL_ICMP_TYPE, "Any");

                if (TRUE == ip->isCareICMPCode)
                    cJSON_AddNumberToObject(ace, CGI_ACL_ICMP_CODE, ip->icmpCode);
                else
                    cJSON_AddStringToObject(ace, CGI_ACL_ICMP_CODE, "Any");

                break;
            default:
                break;
        }
    }
    else
        cJSON_AddStringToObject(ace, CGI_ACL_PROTOCOL, "Any");

    if (6 == ip->protol || 17 == ip->protol)
    {
        sys_l4port_range_t  pr;

        if (ip->isCareSrcPort)
        {
            cJSON_AddNumberToObject(ace, CGI_ACL_SPORT_S, ip->srcPort);
            cJSON_AddNumberToObject(ace, CGI_ACL_SPORT_E, ip->srcPort);
        }
        else if (TRUE == ip->isCareSrcPortRange)
        {
            SYS_ERR_CHK(sal_acl_l4port_range_get(&pr, ip->l4SrcPortRangeIdx));
            cJSON_AddNumberToObject(ace, CGI_ACL_SPORT_S, pr.portStart);
            cJSON_AddNumberToObject(ace, CGI_ACL_SPORT_E, pr.portEnd);
        }
        else
        {
            cJSON_AddStringToObject(ace, CGI_ACL_SPORT_S, "Any");
            cJSON_AddStringToObject(ace, CGI_ACL_SPORT_E, "Any");
        }

        if (ip->isCareDstPort)
        {
            cJSON_AddNumberToObject(ace, CGI_ACL_DPORT_S, ip->dstPort);
            cJSON_AddNumberToObject(ace, CGI_ACL_DPORT_E, ip->dstPort);
        }
        else if (TRUE == ip->isCareDstPortRange)
        {
            SYS_ERR_CHK(sal_acl_l4port_range_get(&pr, ip->l4DstPortRangeIdx));
            cJSON_AddNumberToObject(ace, CGI_ACL_DPORT_S, pr.portStart);
            cJSON_AddNumberToObject(ace, CGI_ACL_DPORT_E, pr.portEnd);
        }
        else
        {
            cJSON_AddStringToObject(ace, CGI_ACL_DPORT_S, "Any");
            cJSON_AddStringToObject(ace, CGI_ACL_DPORT_E, "Any");
        }
    }

    if (TRUE == ip->isCareDscp)
        cJSON_AddNumberToObject(ace, CGI_ACL_DSCP, ip->dscp);
    else if (TRUE == ip->isCareIpPrecedence)
        cJSON_AddNumberToObject(ace, CGI_ACL_IPP, ip->ipPrecedence);
    else
    {
        cJSON_AddStringToObject(ace, CGI_ACL_DSCP, "Any");
        cJSON_AddStringToObject(ace, CGI_ACL_IPP, "Any");
    }

    return SYS_ERR_OK;
}

static int32 cgi_acl_ace_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
	cJSON *acls = NULL;
    cJSON *acl = NULL;
    cJSON *aces = NULL;
    cJSON *ace = NULL;
    cJSON *ranges = NULL;
    cJSON *range = NULL;
    char *value;
    uint32 i = 0;
    uint32 aclcnt = 0;
    uint32 acecnt = 0;
    sys_namestr_t name;
    sys_namestr_t selectName;
    sys_logic_port_t lp;
    sys_ace_content_t ace_content;
    sys_acl_type_t aclType = SYS_ACL_MAC_BASE;
    sys_acl_bind_names_t names[SYS_LOGICPORT_NUM_MAX];

    SYS_MEM_CLEAR(selectName);
    SYS_MEM_CLEAR(names);

    if (CGI_IS_PARAM_EXIST(CGI_ACL_TYPE, value))
        aclType = SYS_STR2UINT(value);
    else
        goto cgi_end;

    sal_acl_entry_countByType_get(aclType, &aclcnt);

    CGI_GET_JSON_CREATE(root, data);
	acls = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_ACL_ACLS, acls);
    cJSON_AddNumberToObject(data, CGI_ACL_MINPRI, BOND_ACL_USER_PRI_MIN);
    cJSON_AddNumberToObject(data, CGI_ACL_MAXPRI, BOND_ACL_USER_PRI_MAX);

    if (0 != aclcnt)
    {
        if (!CGI_IS_PARAM_EXIST(CGI_NAME, value))
            sal_acl_entry_nameByIdx_get(selectName.str, aclType, 0);
        else
            strlcpy(selectName.str, value, sizeof(selectName.str));

        sal_acl_entry_portBinding_get(names);
        sal_ace_entry_count_get(selectName.str, aclType, &acecnt);

        cJSON_AddStringToObject(data, CGI_NAME, selectName.str);

        FOR_EACH_LOGIC_PORT(lp)
        {
            if (0 == osal_strcmp(names[lp].aclname[aclType].str, selectName.str))
            {
                cJSON_AddBoolToObject(data, CGI_ACL_BINDING, TRUE);
                break;
            }
        }

        for (i = 0; i < aclcnt; ++i)
        {
            SYS_MEM_CLEAR(name);
            sal_acl_entry_nameByIdx_get(name.str, aclType, i);

            acl = cJSON_CreateObject();
            cJSON_AddItemToArray(acls, acl);
            cJSON_AddStringToObject(acl, CGI_NAME, name.str);
            cJSON_AddStringToObject(acl, CGI_VALUE, name.str);
        }
    }
    else
    {
        acl = cJSON_CreateObject();
        cJSON_AddItemToArray(acls, acl);

        SYS_MEM_CLEAR(name);
        osal_sprintf(name.str, "None");
        cJSON_AddStringToObject(acl, CGI_NAME, name.str);
        cJSON_AddStringToObject(acl, CGI_VALUE, name.str);
    }

    if (0 != acecnt)
    {
        aces = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_ACL_ACES, aces);

        for (i = 0; i < acecnt; ++i)
        {
            ace = cJSON_CreateObject();

            SYS_MEM_CLEAR(ace_content);
            sal_ace_entry_byIdx_get(selectName.str, &ace_content, aclType, i);

            cJSON_AddNumberToObject(ace, CGI_ACL_PRI, ace_content.priority);

            if (SYS_PKT_ACTION_FORWARD == ace_content.action)
                cJSON_AddNumberToObject(ace, CGI_ACL_ACTION, 0);
            else if (SYS_PKT_ACTION_DROP == ace_content.action)
                cJSON_AddNumberToObject(ace, CGI_ACL_ACTION, 1);
            else
                cJSON_AddNumberToObject(ace, CGI_ACL_ACTION, 2);

            {
                cJSON_AddStringToObject(ace, CGI_SYS_TR_NAME, "");
                cJSON_AddStringToObject(ace, CGI_SYS_TR_ISON, "");
            }

            if (SYS_ACL_MAC_BASE == aclType)
                _cgi_acl_aceMacField_get(ace, &ace_content);
            else if (SYS_ACL_IPV4_BASE == aclType)
                _cgi_acl_aceIpField_get(ace, &ace_content);
            else if (SYS_ACL_IPV6_BASE == aclType)
                _cgi_acl_aceIpField_get(ace, &ace_content);

            cJSON_AddItemToArray(aces, ace);
        }
    }

    ranges = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_SYS_TR_RANGES, ranges);
    range = cJSON_CreateObject();
    cJSON_AddItemToArray(ranges, range);
    cJSON_AddStringToObject(range, CGI_NAME, "None");
    cJSON_AddStringToObject(range, CGI_VALUE, "");

cgi_end:
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_acl_binding_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *acls = NULL;
    cJSON *acl = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    uint32 i = 0;
    uint32 j = 0;
    uint32 aclcnt = 0;
    uint32 acecnt = 0;
    sys_namestr_t name;
    sys_logic_port_t lp;
    sys_acl_bind_names_t names[SYS_LOGICPORT_NUM_MAX];

    SYS_MEM_CLEAR(names);

    CGI_GET_JSON_CREATE(root, data);

    sal_acl_entry_portBinding_get(names);

    for (i = SYS_ACL_MAC_BASE; i < SYS_ACL_TYPE_END ; i++)
    {
        sal_acl_entry_countByType_get(i, &aclcnt);

        acls = cJSON_CreateArray();

        acl = cJSON_CreateObject();
        cJSON_AddStringToObject(acl, CGI_NAME, "None");
        cJSON_AddStringToObject(acl, CGI_VALUE, "None");
        cJSON_AddItemToArray(acls, acl);

        for (j = 0 ; j < aclcnt ; j++)
        {
            SYS_MEM_CLEAR(name);
            sal_acl_entry_nameByIdx_get(name.str, i, j);

            sal_ace_entry_count_get(name.str, i, &acecnt);

            if (0 == acecnt)
                continue;

            acl = cJSON_CreateObject();
            cJSON_AddStringToObject(acl, CGI_NAME, name.str);
            cJSON_AddStringToObject(acl, CGI_VALUE, name.str);
            cJSON_AddItemToArray(acls, acl);
        }

        if (i == SYS_ACL_MAC_BASE)
            cJSON_AddItemToObject(data, CGI_ACL_MAC, acls);
        else if (i == SYS_ACL_IPV4_BASE)
            cJSON_AddItemToObject(data, CGI_ACL_IPV4, acls);
        else if (i == SYS_ACL_IPV6_BASE)
            cJSON_AddItemToObject(data, CGI_ACL_IPV6, acls);
    }

    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();
        cJSON_AddItemToArray(ports, port);

        for (i = SYS_ACL_MAC_BASE; i < SYS_ACL_TYPE_END ; i++)
        {
            if (i == SYS_ACL_MAC_BASE)
                cJSON_AddStringToObject(port, CGI_ACL_MAC, names[lp].aclname[i].str);
            else if (i == SYS_ACL_IPV4_BASE)
                cJSON_AddStringToObject(port, CGI_ACL_IPV4, names[lp].aclname[i].str);
            else if (i == SYS_ACL_IPV6_BASE)
                cJSON_AddStringToObject(port, CGI_ACL_IPV6, names[lp].aclname[i].str);
        }
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

