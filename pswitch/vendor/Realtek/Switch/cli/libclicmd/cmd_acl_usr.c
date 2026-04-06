
#include <libsal/sal_acl_usr.h>
#include <libcmd/cmd.h>

sys_namestr_t g_aclname;
sys_acl_type_t g_acltype;
uint32 g_isAclBind = FALSE;

static int32 _protoName_parse(sal_acl_proto_id_t *proto, uint32 protoCnt, char *input)
{
    uint32 i = 0;
    int32 partialResult = -1;

    for (i = 0; i < protoCnt ; i++)
    {
        if (0 == osal_strncmp(input, proto[i].proto, strlen(input)))
        {
            
            if (strlen(input) == strlen(proto[i].proto))
                return proto[i].value;
            else if (partialResult < 0 || strlen(proto[partialResult].proto) > strlen(proto[i].proto))
                    partialResult = i;
        }
    }

    if (-1 != partialResult)
        return proto[partialResult].value;

    return SYS_ERR_OK;
}

static int32 _l4Port_parse(char *portStr, sys_ace_ip_t *ip, uint8 isSrc, uint8 proto)
{
    char buf[SYS_BUF32_LEN];
    char *tmpStr = NULL;
    uint8 isPortRange = FALSE;
    uint8 protoCnt = 0;
    uint32 portStart = 0;
    uint32 portEnd = 0;
    sys_l4port_range_t portRange;
    sal_acl_proto_id_t *protoId = NULL;

    osal_memset(&portRange, 0, sizeof(portRange));

    if ((NULL == rindex(portStr, '-')) && isdigit(portStr[0]))
    {
        if (isSrc)
        {
            ip->srcPort = SYS_STR2UINT(portStr);
            ip->isCareSrcPort = 1;
        }
        else
        {
            ip->dstPort = SYS_STR2UINT(portStr);
            ip->isCareDstPort = 1;
        }

    }
    else if (!((strlen(portStr) == 3) && (0 == osal_strcmp(portStr, "any"))))
    {
        if (0x6 == proto) 
        {
            protoId = acl_tcp_type;
            protoCnt = ACL_TCP_TYPE_NUM;
        }
        if (0x11 == proto) 
        {
            protoId = acl_udp_type;
            protoCnt = ACL_UDP_TYPE_NUM;
        }

        if (isSrc)
        {
            ip->srcPort = _protoName_parse(protoId, protoCnt, portStr);

            if (0 != ip->srcPort)
                ip->isCareSrcPort = 1;
            else
                isPortRange = TRUE;
        }
        else
        {
            ip->dstPort = _protoName_parse(protoId, protoCnt, portStr);

            if (0 != ip->dstPort)
                ip->isCareDstPort = 1;
            else
                isPortRange = TRUE;
        }

        if (TRUE == isPortRange)
        {
            tmpStr = rindex(portStr, '-');

            if (NULL == tmpStr)
                return SYS_ERR_ACL_PORT_RANGE_INVALID;

            memset(buf, 0, sizeof(buf));
            strncpy(buf, portStr, tmpStr - portStr);
            portStart = SYS_STR2UINT(buf);

            memset(buf, 0, sizeof(buf));
            strcpy(buf, tmpStr+1);
            portEnd = SYS_STR2UINT(buf);

            if (IS_PARAM_NUM_INVALID(portStart, 0, 65535))
                return SYS_ERR_ACL_PORT_RANGE_INVALID;

            if (IS_PARAM_NUM_INVALID(portEnd, 0, 65535))
                return SYS_ERR_ACL_PORT_RANGE_INVALID;

            if (portStart >= portEnd)
                return SYS_ERR_ACL_PORT_RANGE_INVALID;

            portRange.portStart = portStart;
            portRange.portEnd = portEnd;
            portRange.direct = (TRUE == isSrc) ? RNGCHK_L4PORT_DIRECTION_SRC : RNGCHK_L4PORT_DIRECTION_DST;

            if (TRUE == isSrc)
                SYS_ERR_CHK(sal_acl_l4port_range_set(&portRange, &ip->l4SrcPortRangeIdx));
            else
                SYS_ERR_CHK(sal_acl_l4port_range_set(&portRange, &ip->l4DstPortRangeIdx));

            if (isSrc)
            {
                ip->isCareSrcPortRange = 1;
            }
            else
            {
                ip->isCareDstPortRange = 1;
            }

            SYS_DBG(DBG_ACLMGR_USER_DEF, "l4 port range %u to %u use chip idx %u", portStart, portEnd, (isSrc) ? ip->l4SrcPortRangeIdx : ip->l4DstPortRangeIdx);
        }
    }

    return SYS_ERR_OK;
}

static int32 _idtoName_convert(sal_acl_proto_id_t *proto, uint32 protoCnt, uint16 id, char *buf)
{
    uint32 i = 0;

    for (i = 0; i < protoCnt ; i++)
    {
        if (proto[i].value == id)
        {
            strcpy(buf, proto[i].proto);
            break;
        }
    }

    return SYS_ERR_OK;
}

static int32 _ipv4_str_parse(char *ipStr, sys_ace_ip_t *ip, uint8 isSrc)
{
    char *tmpStr = NULL;
    char buf[SYS_BUF64_LEN];

    tmpStr = rindex(ipStr, '/');

    memset(buf, 0, sizeof(buf));
    strncpy(buf, ipStr, tmpStr - ipStr);

    if (isSrc)
        SYS_ERR_CHK(sys_util_str2Ip(buf, &ip->v4sip));
    else
        SYS_ERR_CHK(sys_util_str2Ip(buf, &ip->v4dip));

    memset(buf, 0, sizeof(buf));
    strcpy(buf, tmpStr+1);

    if (isSrc)
        SYS_ERR_CHK(sys_util_str2Ip(buf, &ip->v4sipMask));
    else
        SYS_ERR_CHK(sys_util_str2Ip(buf, &ip->v4dipMask));

    if (isSrc)
        ip->isCareSip = 1;
    else
        ip->isCareDip = 1;

    if (isSrc)
        SYS_DBG(DBG_ACLMGR_USER_DEF, "v4 sip %x mask %x", ip->v4sip, ip->v4sipMask);
    else
        SYS_DBG(DBG_ACLMGR_USER_DEF, "v4 dip %x mask %x", ip->v4dip, ip->v4dipMask);

    return SYS_ERR_OK;
}

static int32 _ipv6_str_parse(char *ipStr, sys_ace_ip_t *ip, uint8 isSrc)
{
    char *tmpStr = NULL;
    char buf[SYS_BUF64_LEN];
    struct sockaddr_in6 sin6_dummy;

    osal_memset(&sin6_dummy, 0 , sizeof(sin6_dummy));

    tmpStr = rindex(ipStr, '/');
    memset(buf, 0, sizeof(buf));
    strncpy(buf, ipStr, tmpStr - ipStr);
    inet_pton(AF_INET6, buf, &sin6_dummy.sin6_addr);

    memset(buf, 0, sizeof(buf));
    strcpy(buf, tmpStr+1);

    if (isSrc)
    {
        osal_memcpy(&ip->v6sip, &sin6_dummy.sin6_addr, sizeof(sin6_dummy.sin6_addr));
        ip->v6sip_prefix = SYS_STR2UINT(buf);
        ip->isCareSip = 1;
    }
    else
    {
        osal_memcpy(&ip->v6dip, &sin6_dummy.sin6_addr, sizeof(sin6_dummy.sin6_addr));
        ip->v6dip_prefix = SYS_STR2UINT(buf);
        ip->isCareDip = 1;
    }

    if (isSrc)
        SYS_DBG(DBG_ACLMGR_USER_DEF, "v6 sip %x%x:%x%x:%x%x:%x%x:%x%x:%x%x:%x%x:%x%x mask %u", ip->v6sip.hex[0],
            ip->v6sip.hex[1],ip->v6sip.hex[2],ip->v6sip.hex[3],ip->v6sip.hex[4],ip->v6sip.hex[5],
            ip->v6sip.hex[6],ip->v6sip.hex[7],ip->v6sip.hex[8],ip->v6sip.hex[9],ip->v6sip.hex[10],
            ip->v6sip.hex[11],ip->v6sip.hex[12],ip->v6sip.hex[13],ip->v6sip.hex[14],ip->v6sip.hex[15],
            ip->v6sip_prefix);
    else
        SYS_DBG(DBG_ACLMGR_USER_DEF, "v6 dip %x%x:%x%x:%x%x:%x%x:%x%x:%x%x:%x%x:%x%x mask %u", ip->v6dip.hex[0],
            ip->v6dip.hex[1],ip->v6dip.hex[2],ip->v6dip.hex[3],ip->v6dip.hex[4],ip->v6dip.hex[5],
            ip->v6dip.hex[6],ip->v6dip.hex[7],ip->v6dip.hex[8],ip->v6dip.hex[9],ip->v6dip.hex[10],
            ip->v6dip.hex[11],ip->v6dip.hex[12],ip->v6dip.hex[13],ip->v6dip.hex[14],ip->v6dip.hex[15],
            ip->v6dip);

    return SYS_ERR_OK;
}

static int32 _tcpflag_str_parse(char *flagStr, sys_ace_ip_t *ip)
{
    char isPlus = FALSE;
    uint8 tmpFlag = 0;
    uint8 tmpFlagMask = 0;
    uint8 hasSet = 0;
    uint32 i = 0;

    for (i = 0; i < strlen(flagStr); i++)
    {
        isPlus = FALSE;

        if ('+' == flagStr[i])
            isPlus = TRUE;
        else if ('-' == flagStr[i])
            isPlus = FALSE;
        else
            return SYS_ERR_ACL_TCPFLAG_INVALID;

        if ((0 != flagStr[i+4]) && ('+' != flagStr[i+4]) && ('-' != flagStr[i+4]))
            return SYS_ERR_ACL_TCPFLAG_INVALID;

#define FLAG_CHK(flag, bit)               \
do{                                         \
    if (0 != osal_strncmp(flagStr+i, flag, 3)) \
        return SYS_ERR_ACL_TCPFLAG_INVALID;     \
    if (hasSet & 1<<bit)                        \
        return SYS_ERR_ACL_TCPFLAG_INVALID;     \
    hasSet |= 1<<bit;                           \
    tmpFlagMask |= 1<<bit;                      \
    if (TRUE == isPlus)                         \
        tmpFlag |= 1 << bit;                    \
}while(0);

        i++;

        switch(flagStr[i])
        {
            case 'f':
                FLAG_CHK("fin", SYS_ACL_TCP_FLAG_FIN);
                break;

            case 's':
                FLAG_CHK("syn", SYS_ACL_TCP_FLAG_SYN);
                break;

            case 'r':
                FLAG_CHK("rst", SYS_ACL_TCP_FLAG_RST);
                break;

            case 'p':
                FLAG_CHK("psh", SYS_ACL_TCP_FLAG_PSH);
                break;

            case 'a':
                FLAG_CHK("ack", SYS_ACL_TCP_FLAG_ACK);
                break;

            case 'u':
                FLAG_CHK("urg", SYS_ACL_TCP_FLAG_URG);
                break;

            default:
                return SYS_ERR_ACL_TCPFLAG_INVALID;
        }

        i+=2;
    }

    ip->tcpflag = tmpFlag;
    ip->tcpflagMask = tmpFlagMask;
    ip->isCareTcpFlag = TRUE;

    return SYS_ERR_OK;
}

int32 cmd_acl_aceMac2cmd_convert(sys_ace_content_t *ace_content, char *buf)
{
    char tmp[SYS_BUF32_LEN];
    uint32 len = 0;
    sys_ace_mac_t *mac = NULL;

    SYS_PARAM_CHK((NULL == ace_content), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == buf), SYS_ERR_NULL_POINTER);

    mac = &ace_content->mac;

    len += sprintf(buf + len , "sequence %u ", ace_content->priority);

    if (SYS_PKT_ACTION_FORWARD == ace_content->action)
        len += sprintf(buf + len , "permit ");
    else
        len += sprintf(buf + len , "deny ");

    if (TRUE == mac->isCareSmac)
    {
        osal_memset(tmp, 0, sizeof(tmp));
        SYS_ERR_CHK(sys_util_mac2Str(mac->smac.octet, tmp));
        len += sprintf(buf + len , "%s/", tmp);
        osal_memset(tmp, 0, sizeof(tmp));
        SYS_ERR_CHK(sys_util_mac2Str(mac->smacMask.octet, tmp));
        len += sprintf(buf + len , "%s ", tmp);
    }
    else
    {
        len += sprintf(buf + len , "any ");
    }

    if (TRUE == mac->isCareDmac)
    {
        osal_memset(tmp, 0, sizeof(tmp));
        SYS_ERR_CHK(sys_util_mac2Str(mac->dmac.octet, tmp));
        len += sprintf(buf + len , "%s/", tmp);
        osal_memset(tmp, 0, sizeof(tmp));
        SYS_ERR_CHK(sys_util_mac2Str(mac->dmacMask.octet, tmp));
        len += sprintf(buf + len , "%s ", tmp);
    }
    else
    {
        len += sprintf(buf + len , "any ");
    }

    if (TRUE == mac->isCareVid)
        len += sprintf(buf + len , "vlan %u ", mac->vid);

    if (TRUE == mac->isCare8021p)
        len += sprintf(buf + len , "cos %u %u ", mac->value8021p, mac->mask8021p);

    if (TRUE == mac->isCareEthtype)
        len += sprintf(buf + len , "ethtype 0x%x ", mac->ethType);

    if (SYS_PKT_ACTION_SHUTDOWN_PORT == ace_content->action)
        len += sprintf(buf + len , "shutdown ");

    len += sprintf(buf + len , "\n");

    return SYS_ERR_OK;
}

int32 cmd_acl_aceIp2cmd_convert(sys_ace_content_t *ace_content, char *buf)
{
    char tmp[SYS_BUF64_LEN];
    uint32 len = 0;
    sys_ace_ip_t *ip = NULL;

    SYS_PARAM_CHK((NULL == ace_content), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == buf), SYS_ERR_NULL_POINTER);

    ip = &ace_content->ip;

#define V4IP2STR(_ip, _mask)                \
do{                                         \
    osal_memset(tmp, 0, sizeof(tmp));       \
    SYS_ERR_CHK(sys_util_ip2Str(_ip, tmp)); \
    len += sprintf(buf + len , "%s/", tmp);  \
    osal_memset(tmp, 0, sizeof(tmp));        \
    SYS_ERR_CHK(sys_util_ip2Str(_mask, tmp));  \
    len += sprintf(buf + len , "%s ", tmp);   \
}while(0);

#define V6IP2STR(_ip, _prefix)                \
do{                                         \
    osal_memset(tmp, 0, sizeof(tmp));       \
    inet_ntop(AF_INET6, _ip, tmp, sizeof(tmp));  \
    len += sprintf(buf + len , "%s/", tmp);  \
    len += sprintf(buf + len , "%u ", _prefix);   \
}while(0);

#define SIP2STR()                \
do{                                         \
    if (ip->isCareSip){                     \
        if (ip->isV4)                       \
        {    V4IP2STR(ip->v4sip, ip->v4sipMask);} \
        else                                \
        {    V6IP2STR(ip->v6sip.hex, ip->v6sip_prefix);}}\
    else                                        \
        len += sprintf(buf + len , "any ");     \
}while(0);

#define DIP2STR()                \
do{                                         \
    if (ip->isCareDip){                     \
        if (ip->isV4)                       \
        {   V4IP2STR(ip->v4dip, ip->v4dipMask);} \
        else                                \
        {   V6IP2STR(ip->v6dip.hex, ip->v6dip_prefix);}}\
    else                                        \
        len += sprintf(buf + len , "any ");     \
}while(0);

#define SPORT2STR(_proto, _protonum)    \
do{                                     \
    sys_l4port_range_t __pr;       \
    osal_memset(&__pr, 0, sizeof(__pr));      \
    if (TRUE == ip->isCareSrcPort) {        \
        osal_memset(tmp, 0, sizeof(tmp));   \
        _idtoName_convert(_proto, _protonum, ip->srcPort, tmp); \
        if (0 == strlen(tmp))                                   \
            len += sprintf(buf + len , "%u ", ip->srcPort);      \
        else                                                     \
            len += sprintf(buf + len , "%s ", tmp);}             \
    else if (TRUE == ip->isCareSrcPortRange) {                   \
        SYS_ERR_CHK(sal_acl_l4port_range_get(&__pr, ip->l4SrcPortRangeIdx));    \
        len += sprintf(buf + len , "%u-%u ", __pr.portStart, __pr.portEnd);} \
    else                                        \
        len += sprintf(buf + len , "any ");     \
}while(0);

#define DPORT2STR(_proto, _protonum)    \
do{                                     \
    sys_l4port_range_t __pr;       \
    osal_memset(&__pr, 0, sizeof(__pr));      \
    if (TRUE == ip->isCareDstPort) {        \
        osal_memset(tmp, 0, sizeof(tmp));   \
        _idtoName_convert(_proto, _protonum, ip->dstPort, tmp); \
        if (0 == strlen(tmp))                                   \
            len += sprintf(buf + len , "%u ", ip->dstPort);      \
        else                                                     \
            len += sprintf(buf + len , "%s ", tmp);}             \
    else if (TRUE == ip->isCareDstPortRange) {                   \
        SYS_ERR_CHK(sal_acl_l4port_range_get(&__pr, ip->l4DstPortRangeIdx));    \
        len += sprintf(buf + len , "%u-%u ", __pr.portStart, __pr.portEnd);} \
    else                                        \
        len += sprintf(buf + len , "any ");     \
}while(0);

    len += sprintf(buf + len , "sequence %u ", ace_content->priority);

    if (SYS_PKT_ACTION_FORWARD == ace_content->action)
        len += sprintf(buf + len , "permit ");
    else
        len += sprintf(buf + len , "deny ");

    if (TRUE == ip->isCareProto)
    {
        if (((ip->isV4) && (1 == ip->protol)) || ((!ip->isV4) && (58 == ip->protol))) 
        {
            len += sprintf(buf + len , "icmp ");

            SIP2STR();
            DIP2STR();

            if (TRUE == ip->isCareICMPType)
            {
                osal_memset(tmp, 0, sizeof(tmp));
                if (ip->isV4)
                    _idtoName_convert(acl_icmp_type, ACL_ICMP_TYPE_NUM, ip->icmpType, tmp);
                else
                    _idtoName_convert(acl_icmpv6_type, ACL_ICMPV6_TYPE_NUM, ip->icmpType, tmp);

                if (0 == strlen(tmp))
                    len += sprintf(buf + len , "%u ", ip->icmpType);
                else
                    len += sprintf(buf + len , "%s ", tmp);
            }
            else
            {
                len += sprintf(buf + len , "any ");
            }

            if (TRUE == ip->isCareICMPCode)
                len += sprintf(buf + len , "%u ", ip->icmpCode);
            else
                len += sprintf(buf + len , "any ");
        }
#if 0
        else if ((ip->isV4) && (2 == ip->protol)) 
        {
            len += sprintf(buf + len , "igmp ");

            SIP2STR();
            DIP2STR();

            if (TRUE == ip->isCareIGMPType)
            {
                osal_memset(tmp, 0, sizeof(tmp));
                _idtoName_convert(acl_igmp_type, ACL_IGMP_TYPE_NUM, ip->igmpType, tmp);

                if (0 == strlen(tmp))
                    len += sprintf(buf + len , "%u ", ip->igmpType);
                else
                    len += sprintf(buf + len , "%s ", tmp);
            }
            else
            {
                len += sprintf(buf + len , "any ");
            }
        }
#endif
        else if (6 == ip->protol) 
        {
            len += sprintf(buf + len , "tcp ");

            SIP2STR();
            SPORT2STR(acl_tcp_type, ACL_TCP_TYPE_NUM);
            DIP2STR();
            DPORT2STR(acl_tcp_type, ACL_TCP_TYPE_NUM);

            if (ip->isCareTcpFlag)
            {
                len += sprintf(buf + len , "match-all ");

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_FIN))
                    len += sprintf(buf + len , "%cfin", ip->tcpflag & ((1 << SYS_ACL_TCP_FLAG_FIN)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_SYN))
                    len += sprintf(buf + len , "%csyn", ip->tcpflag & ((1 << SYS_ACL_TCP_FLAG_SYN)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_RST))
                    len += sprintf(buf + len , "%crst", (ip->tcpflag & (1 << SYS_ACL_TCP_FLAG_RST)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_PSH))
                    len += sprintf(buf + len , "%cpsh", (ip->tcpflag & (1 << SYS_ACL_TCP_FLAG_PSH)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_ACK))
                    len += sprintf(buf + len , "%cack", (ip->tcpflag & (1 << SYS_ACL_TCP_FLAG_ACK)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_URG))
                    len += sprintf(buf + len , "%curg", (ip->tcpflag & (1 << SYS_ACL_TCP_FLAG_URG)) ? '+' : '-');

                len += sprintf(buf + len , " ");
            }
        }
        else if (17 == ip->protol) 
        {
            len += sprintf(buf + len , "udp ");

            SIP2STR();
            SPORT2STR(acl_udp_type, ACL_UDP_TYPE_NUM);
            DIP2STR();
            DPORT2STR(acl_udp_type, ACL_UDP_TYPE_NUM);
        }
        else
        {
            osal_memset(tmp, 0, sizeof(tmp));

            if (ip->isV4)
                _idtoName_convert(acl_ip_proto, ACL_IP_PROTO_NUM, ip->protol, tmp);
            else
                _idtoName_convert(acl_ipv6_proto, ACL_IPV6_PROTO_NUM, ip->protol, tmp);

            if (0 == strlen(tmp))
                len += sprintf(buf + len , "%u ", ip->protol);
            else
                len += sprintf(buf + len , "%s ", tmp);

            SIP2STR();
            DIP2STR();
        }

    }
    else
    {
         len += sprintf(buf + len , (ip->isV4) ? "ip ": "ipv6 ");
         SIP2STR();
         DIP2STR();
    }

    if (TRUE == ip->isCareDscp)
        len += sprintf(buf + len , "dscp %u ", ip->dscp);
    else if (TRUE == ip->isCareIpPrecedence)
        len += sprintf(buf + len , "precedence %u ", ip->ipPrecedence);

    if (SYS_PKT_ACTION_SHUTDOWN_PORT == ace_content->action)
        len += sprintf(buf + len , "shutdown");

    len += sprintf(buf + len , "\n");

    return SYS_ERR_OK;
}

int32 cmd_acl_all_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char tmp[SYS_BUF512_LEN];
    uint32 i = 0;
    uint32 j = 0;
    uint32 aclType = 0;
    uint32 aclCnt = 0;
    uint32 aceCnt = 0;
    sys_namestr_t name;
    sys_ace_content_t ace_content;

    osal_memset(tmp, 0, sizeof(tmp));

    for (aclType = SYS_ACL_MAC_BASE; aclType < SYS_ACL_TYPE_END; aclType++)
    {
        SYS_ERR_CHK(sal_acl_entry_countByType_get(aclType, &aclCnt));

        for (i = 0; i < aclCnt ; i++)
        {
            osal_memset(&name, 0 , sizeof(name));
            SYS_ERR_CHK(sal_acl_entry_nameByIdx_get(name.str, aclType, i));

            if (SYS_ACL_MAC_BASE == aclType)
                XMORE("\nMAC access list %s\n", name.str);
            else if (SYS_ACL_IPV4_BASE == aclType)
                XMORE("\nIP access list %s\n", name.str);
            else if (SYS_ACL_IPV6_BASE == aclType)
                XMORE("\nIPv6 access list %s\n", name.str);

            SYS_ERR_CHK(sal_ace_entry_count_get(name.str, aclType, &aceCnt));

            for (j = 0; j < aceCnt ; j++)
            {
                osal_memset(tmp, 0 , strlen(tmp));
                osal_memset(&ace_content, 0 , sizeof(ace_content));
                SYS_ERR_CHK(sal_ace_entry_byIdx_get(name.str, &ace_content, aclType, j));

                if (SYS_ACL_MAC_BASE == aclType)
                    cmd_acl_aceMac2cmd_convert(&ace_content, tmp);
                else
                    cmd_acl_aceIp2cmd_convert(&ace_content, tmp);

                XMORE("    %s", tmp);
            }
        }
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_acl_byName_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char tmp[SYS_BUF512_LEN];
    uint32 i = 0;
    uint32 aclType = 0;
    uint32 aceCnt = 0;
    sys_ace_content_t ace_content;

    osal_memset(tmp, 0, sizeof(tmp));
    SYS_ERR_CHK(sal_acl_entry_typeByName_get(argv[1], &aclType));

    if (SYS_ACL_MAC_BASE == aclType)
        XMORE("\nMAC access list %s\n", argv[1]);
    else if (SYS_ACL_IPV4_BASE == aclType)
        XMORE("\nIP access list %s\n", argv[1]);
    else if (SYS_ACL_IPV6_BASE == aclType)
        XMORE("\nIPv6 access list %s\n", argv[1]);

    SYS_ERR_CHK(sal_ace_entry_count_get(argv[1], aclType, &aceCnt));

    for (i = 0; i < aceCnt ; i++)
    {
        osal_memset(tmp, 0 , strlen(tmp));
        osal_memset(&ace_content, 0 , sizeof(ace_content));
        SYS_ERR_CHK(sal_ace_entry_byIdx_get(argv[1], &ace_content, aclType, i));

        if (SYS_ACL_MAC_BASE == aclType)
            cmd_acl_aceMac2cmd_convert(&ace_content, tmp);
        else
            cmd_acl_aceIp2cmd_convert(&ace_content, tmp);

        XMORE("    %s", tmp);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_acl_byType_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char tmp[SYS_BUF512_LEN];
    uint32 i = 0;
    uint32 j = 0;
    uint32 aclType = 0;
    uint32 aclCnt = 0;
    uint32 aceCnt = 0;
    sys_namestr_t name;
    sys_ace_content_t ace_content;

    if ('m' == argv[0][0])
        aclType = SYS_ACL_MAC_BASE;
    else if (0 == strcmp("ip", argv[0]))
        aclType = SYS_ACL_IPV4_BASE;
    else if (0 == strncmp("ipv6", argv[0], strlen(argv[0])))
        aclType = SYS_ACL_IPV6_BASE;

    SYS_ERR_CHK(sal_acl_entry_countByType_get(aclType, &aclCnt));

    for (i = 0; i < aclCnt ; i++)
    {
        osal_memset(&name, 0 , sizeof(name));
        SYS_ERR_CHK(sal_acl_entry_nameByIdx_get(name.str, aclType, i));

        if (SYS_ACL_MAC_BASE == aclType)
            XMORE("\nMAC access list %s\n", name.str);
        else if (SYS_ACL_IPV4_BASE == aclType)
            XMORE("\nIP access list %s\n", name.str);
        else if (SYS_ACL_IPV6_BASE == aclType)
            XMORE("\nIPv6 access list %s\n", name.str);

        SYS_ERR_CHK(sal_ace_entry_count_get(name.str, aclType, &aceCnt));

        for (j = 0; j < aceCnt ; j++)
        {
            SYS_MEM_CLEAR(tmp);
            SYS_MEM_CLEAR(ace_content);
            SYS_ERR_CHK(sal_ace_entry_byIdx_get(name.str, &ace_content, aclType, j));

            if (SYS_ACL_MAC_BASE == aclType)
                cmd_acl_aceMac2cmd_convert(&ace_content, tmp);
            else
                cmd_acl_aceIp2cmd_convert(&ace_content, tmp);

            XMORE("    %s", tmp);
        }
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_acl_ace_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char tmp[SYS_BUF512_LEN];
    uint32 i = 0;
    uint32 aceCnt = 0;
    sys_ace_content_t ace_content;

    SYS_ERR_CHK(sal_ace_entry_count_get(g_aclname.str, g_acltype, &aceCnt));

    if (SYS_ACL_MAC_BASE == g_acltype)
        XMORE("\nMAC access list %s\n", g_aclname.str);
    else if (SYS_ACL_IPV4_BASE == g_acltype)
        XMORE("\nIP access list %s\n", g_aclname.str);
    else if (SYS_ACL_IPV6_BASE == g_acltype)
        XMORE("\nIPv6 access list %s\n", g_aclname.str);

    for (i = 0; i < aceCnt ; i++)
    {
        SYS_MEM_CLEAR(tmp);
        SYS_MEM_CLEAR(ace_content);
        SYS_ERR_CHK(sal_ace_entry_byIdx_get(g_aclname.str, &ace_content, g_acltype, i));

        if (SYS_ACL_MAC_BASE == g_acltype)
            cmd_acl_aceMac2cmd_convert(&ace_content, tmp);
        else
            cmd_acl_aceIp2cmd_convert(&ace_content, tmp);

        XMORE("    %s", tmp);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_acl_utilization_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 i = 0;
    sys_logic_block_4show_t pieUsage;

    char *typeName[] = { "System Reserve",
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    "Mac ACL", "IPv4 ACL", "IPv6 ACL",
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    "IP Source Guard",
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    "Voice VLAN", "Voice VLAN Remark",
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    "Surveillance VLAN", "Surveillance VLAN Remark",
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    "Protocol-based VLAN",
#endif
#ifdef CONFIG_SYS_MACVLAN
    "MAC-based VLAN",
#endif
#if CONFIG_SYS_PROTO_AUTHMGR
    "Auth",
#endif
#if CONFIG_SYS_L3_ROUTE
    "Host Route", "Net Route",
#endif
    };

    SYS_MEM_CLEAR(pieUsage);

    SYS_ERR_CHK(sal_acl_pieUsage_get(&pieUsage));

    for (i = 0; i < ACL_PROTOTYPE_END; i++)
    {
        if (0 != pieUsage.usage[i])
        {
            XMORE("Type: %-30s usage: %u\n", typeName[i], pieUsage.usage[i]);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_acl_enter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char *typeStr = NULL;
    sys_acl_type_t type = SYS_ACL_TYPE_NONE;

    if (CMD_IS_NO_FORM_EXIST())
        typeStr = CMD_TOKENSTR_GET(cmd->vline, 1);
    else
        typeStr = CMD_TOKENSTR_GET(cmd->vline, 0);

    if (NULL == typeStr)
        return SYS_ERR_NULL_POINTER;

    if (osal_strlen(argv[0]) > CAPA_NAME_STR_LEN)
        return SYS_ERR_STR_LEN_EXCEED;

    if (0 == osal_strlen(argv[0]))
        return SYS_ERR_STR_LEN_EMPTY;

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], FALSE))
        return SYS_ERR_STRING;

    if (0 == osal_strcmp("mac", typeStr))
        type = SYS_ACL_MAC_BASE;
    else if (0 == osal_strcmp("ip", typeStr))
        type = SYS_ACL_IPV4_BASE;
    else if (0 == osal_strncmp("ipv6", typeStr, strlen(typeStr)))
        type = SYS_ACL_IPV6_BASE;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_acl_entry_del(argv[0], type));
    }
    else
    {
        SYS_ERR_CHK(sal_acl_entry_create(argv[0], type));

        if (SYS_ACL_MAC_BASE == type)
            vty->node = ACL_MAC_NODE;
        else if (SYS_ACL_IPV4_BASE == type)
            vty->node = ACL_IPV4_NODE;
        else if (SYS_ACL_IPV6_BASE == type)
            vty->node = ACL_IPV6_NODE;

        osal_memset(&g_aclname,0, sizeof(g_aclname));
        osal_memcpy(g_aclname.str, argv[0], strlen(argv[0]));
        g_acltype = type;

        sal_acl_isACLBind_ret(&g_aclname, &g_isAclBind);
    }

    return SYS_ERR_OK;
}

int32 cmd_acl_exit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    osal_memset(&g_aclname,0, sizeof(g_aclname));
    vty->node = CONFIG_NODE;

    return SYS_ERR_OK;
}

int32 cmd_acl_aceMac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char *ridx = NULL;
    char buf[SYS_BUF32_LEN];
    uint8 isSequenceFound = 0;
    uint32 i = 0;
    uint32 tokenCnt = 0;
    uint32 ridx_len = 0;
    uint32 aceCnt = 0;
    sys_ace_content_t old_ace_content; 
    sys_ace_content_t ace_content;
    sys_ace_mac_t *mac = NULL;

    if (TRUE == g_isAclBind)
        return SYS_ERR_ACL_LIST_BINDING;

    SYS_MEM_CLEAR(ace_content);

    mac = &(ace_content.mac);

    if ('s' == CMD_TOKENSTR_GET(cmd->vline, 0)[0])
    {
        isSequenceFound = 1;
        ace_content.priority = SYS_STR2UINT(argv[0]);
        SYS_DBG(DBG_ACLMGR_USER_DEF, "sequence %u", ace_content.priority);
    }

    if (strlen(argv[0 + isSequenceFound]) > 3)
    {
        ridx = rindex(argv[0 + isSequenceFound], '/');

        memset(buf, 0, sizeof(buf));
        strncpy(buf, argv[0 + isSequenceFound], ridx - argv[0 + isSequenceFound]);
        SYS_ERR_CHK(sys_util_str2Mac(buf, mac->smac.octet));

        memset(buf, 0, sizeof(buf));
        strcpy(buf, ridx+1);
        SYS_ERR_CHK(sys_util_str2Mac(buf, mac->smacMask.octet));

        mac->isCareSmac = TRUE;
    }

    if (strlen(argv[1 + isSequenceFound]) > 3)
    {
        ridx = rindex(argv[1 + isSequenceFound], '/');

        memset(buf, 0, sizeof(buf));
        strncpy(buf, argv[1 + isSequenceFound], ridx - argv[1 + isSequenceFound]);
        SYS_ERR_CHK(sys_util_str2Mac(buf, mac->dmac.octet));

        memset(buf, 0, sizeof(buf));
        strcpy(buf, ridx+1);
        SYS_ERR_CHK(sys_util_str2Mac(buf, mac->dmacMask.octet));

        mac->isCareDmac = TRUE;
    }

    tokenCnt = CMD_TOKENNUM_GET(cmd->vline);

    for (i = (3 + (isSequenceFound ? 2 : 0)); i < tokenCnt ; i++)
    {
        ridx = CMD_TOKENSTR_GET(cmd->vline, i);
        ridx_len = osal_strlen(ridx);

        if (0 == osal_strncmp(ridx, "vlan", ridx_len)) 
        {
            mac->vid = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
            mac->isCareVid = TRUE;
            SYS_DBG(DBG_ACLMGR_USER_DEF, "care vlan %u", mac->vid);
        }
        else if (0 == osal_strncmp(ridx, "cos", ridx_len)) 
        {
            mac->value8021p = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
            mac->mask8021p = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
            mac->isCare8021p = TRUE;
            SYS_DBG(DBG_ACLMGR_USER_DEF, "care 8021p %u %u", mac->value8021p, mac->mask8021p);
        }
        else if (0 == osal_strncmp(ridx, "ethtype", ridx_len)) 
        {
            mac->ethType = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
            mac->isCareEthtype = TRUE;
            SYS_DBG(DBG_ACLMGR_USER_DEF, "care etype %u", mac->ethType);
        }
        else if (0 == osal_strncmp(ridx, "shutdown", ridx_len)) 
        {
            ace_content.action = SYS_PKT_ACTION_SHUTDOWN_PORT;
            SYS_DBG(DBG_ACLMGR_USER_DEF, "shutdown port");
        }
    }

    if (SYS_PKT_ACTION_SHUTDOWN_PORT != ace_content.action)
    {
        if ('p' == CMD_TOKENSTR_GET(cmd->vline, (isSequenceFound ? 2 : 0))[0])
            ace_content.action = SYS_PKT_ACTION_FORWARD;
        else
            ace_content.action = SYS_PKT_ACTION_DROP;
    }

    if (TRUE == isSequenceFound)
    {
        SYS_ERR_CHK(sal_ace_entry_count_get(g_aclname.str, g_acltype, &aceCnt));

        for (i = 0; i < aceCnt ; i++)
        {
            SYS_MEM_CLEAR(old_ace_content);
            SYS_ERR_CHK(sal_ace_entry_byIdx_get(g_aclname.str, &old_ace_content, g_acltype, i));

            if (old_ace_content.priority == ace_content.priority)
            {
                SYS_ERR_CHK(sal_ace_entry_modify(g_aclname.str, &ace_content));
                return SYS_ERR_OK;
            }
        }
    }

    SYS_ERR_CHK(sal_ace_entry_create(g_aclname.str, &ace_content));

    return SYS_ERR_OK;
}

int32 cmd_acl_aceIP_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char *ipProto = NULL;
    char *sip = NULL;
    char *dip = NULL;
    char *l4SrcPort = NULL;
    char *l4DstPort = NULL;
    char *l4Proto = NULL;
    char *ridx = NULL;
    uint32 ridx_len = 0;
    uint32 i = 0;
    uint32 tokenCnt = 0;
    uint32 aceCnt = 0;
    int8 tmpVal = 0;
    uint8 isSequenceFound = 0;
    char protoStr[SYS_BUF8_LEN];

    sys_ace_content_t ace_content;
    sys_ace_content_t old_ace_content; 
    sys_ace_ip_t *ip = NULL;
    sys_err_code_t err;

    if (TRUE == g_isAclBind)
        return SYS_ERR_ACL_LIST_BINDING;

    SYS_MEM_CLEAR(ace_content);
    SYS_MEM_CLEAR(protoStr);

    ip = &(ace_content.ip);

    if ('s' == CMD_TOKENSTR_GET(cmd->vline, 0)[0])
    {
        isSequenceFound = 1;
        ace_content.priority = SYS_STR2UINT(argv[0]);
        SYS_DBG(DBG_ACLMGR_USER_DEF, "sequence %u", ace_content.priority);
    }

    ipProto = argv[0 + isSequenceFound];
    sip = argv[1 + isSequenceFound];
    dip = argv[2 + isSequenceFound];

    if (SYS_ACL_IPV4_BASE == g_acltype)
    {
        ip->isV4 = TRUE;
        osal_sprintf(protoStr, "%s", "ip");
    }
    else
    {
        osal_sprintf(protoStr, "%s", "ipv6");
    }

    if (isdigit(ipProto[0]))
    {
        ip->protol = SYS_STR2UINT(ipProto);
        ip->isCareProto = 1;
    }
    else if (strlen(ipProto) > strlen(protoStr) || osal_strncmp(ipProto, protoStr, strlen(ipProto)))
    {
        ip->protol = _protoName_parse(acl_ip_proto, ACL_IP_PROTO_NUM, ipProto);
        ip->isCareProto = 1;

        if (ip->protol == 1) 
        {
            if (FALSE == ip->isV4)
                ip->protol = 58;

            l4Proto = argv[3 + isSequenceFound];
            if (isdigit(l4Proto[0]))
            {
                ip->icmpType = SYS_STR2UINT(l4Proto);
                ip->isCareICMPType = 1;
            }
            else if (!((strlen(l4Proto) == 3) && (0 == osal_strcmp(l4Proto, "any"))))
            {
                if (TRUE == ip->isV4)
                    ip->icmpType = _protoName_parse(acl_icmp_type, ACL_ICMP_TYPE_NUM, l4Proto);
                else
                    ip->icmpType = _protoName_parse(acl_icmpv6_type, ACL_ICMPV6_TYPE_NUM, l4Proto);

                ip->isCareICMPType = 1;
            }

            l4Proto = argv[4 + isSequenceFound];
            if (isdigit(l4Proto[0]))
            {
                ip->icmpCode = SYS_STR2UINT(l4Proto);
                ip->isCareICMPCode = 1;
            }

            SYS_DBG(DBG_ACLMGR_USER_DEF, "icmp type %u code %u is is %u %u", ip->icmpType, ip->icmpCode, ip->isCareICMPType, ip->isCareICMPCode);
        }
        else if (ip->protol == 2) 
        {
            
            l4Proto = argv[3 + isSequenceFound];
            if (isdigit(l4Proto[0]))
            {
                ip->igmpType = SYS_STR2UINT(l4Proto);
                ip->isCareIGMPType = 1;
            }
            else if (!((strlen(l4Proto) == 3) && (0 == osal_strcmp(l4Proto, "any"))))
            {
                ip->igmpType = _protoName_parse(acl_igmp_type, ACL_IGMP_TYPE_NUM, l4Proto);
                ip->isCareIGMPType = 1;
            }

            SYS_DBG(DBG_ACLMGR_USER_DEF, "igmp type %u is %u", ip->igmpType, ip->isCareIGMPType);
        }
        else if (ip->protol == 6) 
        {
            l4SrcPort = argv[2 + isSequenceFound];
            dip = argv[3 + isSequenceFound];
            l4DstPort = argv[4 + isSequenceFound];

            SYS_ERR_CHK(_l4Port_parse(l4SrcPort, ip, TRUE, ip->protol));
            err = _l4Port_parse(l4DstPort, ip, FALSE, ip->protol);

            if (SYS_ERR_ACL_L4_PORT_RANGE_CHK_FULL == err)
            {
                if (ip->isCareSrcPortRange)
                    SYS_ERR_CHK(sal_acl_l4port_range_del(ip->l4SrcPortRangeIdx));

                return SYS_ERR_ACL_L4_PORT_RANGE_CHK_FULL;
            }
            else
                SYS_ERR_CHK(err);

            SYS_DBG(DBG_ACLMGR_USER_DEF, "tcp sp %u is %u dp %u is %u", ip->srcPort, ip->isCareSrcPort, ip->dstPort, ip->isCareDstPort);
        }
        else if (ip->protol == 17) 
        {
            l4SrcPort = argv[2 + isSequenceFound];
            dip = argv[3 + isSequenceFound];
            l4DstPort = argv[4 + isSequenceFound];
            SYS_ERR_CHK(_l4Port_parse(l4SrcPort, ip, TRUE, ip->protol));
            err = _l4Port_parse(l4DstPort, ip, FALSE, ip->protol);

            if (SYS_ERR_ACL_L4_PORT_RANGE_CHK_FULL == err)
            {
                if (ip->isCareSrcPortRange)
                    SYS_ERR_CHK(sal_acl_l4port_range_del(ip->l4SrcPortRangeIdx));

                return SYS_ERR_ACL_L4_PORT_RANGE_CHK_FULL;
            }
            else
                SYS_ERR_CHK(err);

            SYS_DBG(DBG_ACLMGR_USER_DEF, "udp sp %u is %u dp %u is %u", ip->srcPort, ip->isCareSrcPort, ip->dstPort, ip->isCareDstPort);
        }
    }

    SYS_DBG(DBG_ACLMGR_USER_DEF, "iscare ip proto %u %u", ip->protol, ip->isCareProto);

    if (strlen(sip) > 3)
    {
        if (TRUE == ip->isV4)
            _ipv4_str_parse(sip, ip, TRUE);
        else
            _ipv6_str_parse(sip, ip, TRUE);
    }

    if (strlen(dip) > 3)
    {
        if (TRUE == ip->isV4)
            _ipv4_str_parse(dip, ip, FALSE);
        else
            _ipv6_str_parse(dip, ip, FALSE);
    }

    tokenCnt = CMD_TOKENNUM_GET(cmd->vline);

    for (i = 3; i < tokenCnt ; i++)
    {
        ridx = CMD_TOKENSTR_GET(cmd->vline, i);
        ridx_len = osal_strlen(ridx);

        if (0 == osal_strncmp(ridx, "dscp", ridx_len)) 
        {
            tmpVal = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));

            if (IS_PARAM_NUM_INVALID(tmpVal, 0, 63))
                return SYS_ERR_ACL_DSCP_INVALID;

            ip->dscp = tmpVal;
            ip->isCareDscp = 1;

            SYS_DBG(DBG_ACLMGR_USER_DEF, "care dscp %u", ip->dscp);
        }
        else if (0 == osal_strncmp(ridx, "precedence", ridx_len)) 
        {
            tmpVal = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));

            if (IS_PARAM_NUM_INVALID(tmpVal, 0, 7))
                return SYS_ERR_ACL_PRECEDENCE_INVALID;

            ip->ipPrecedence = tmpVal;
            ip->isCareIpPrecedence = 1;

            SYS_DBG(DBG_ACLMGR_USER_DEF, "care ipPrecedence %u", ip->ipPrecedence);
        }
        else if (0 == osal_strncmp(ridx, "match-all", ridx_len)) 
        {
            SYS_ERR_CHK(_tcpflag_str_parse(CMD_TOKENSTR_GET(cmd->vline, ++i), ip));

            SYS_DBG(DBG_ACLMGR_USER_DEF, "care tcp flag %x %u", ip->tcpflag, ip->isCareTcpFlag);
        }
        else if (0 == osal_strncmp(ridx, "shutdown", ridx_len)) 
        {
            ace_content.action = SYS_PKT_ACTION_SHUTDOWN_PORT;
            SYS_DBG(DBG_ACLMGR_USER_DEF, "disable-port");
        }
    }

    if (SYS_PKT_ACTION_SHUTDOWN_PORT != ace_content.action)
    {
        if ('p' == CMD_TOKENSTR_GET(cmd->vline, (isSequenceFound ? 2 : 0))[0])
            ace_content.action = SYS_PKT_ACTION_FORWARD;
        else
            ace_content.action = SYS_PKT_ACTION_DROP;
    }

    if (TRUE == isSequenceFound)
    {
        SYS_ERR_CHK(sal_ace_entry_count_get(g_aclname.str, g_acltype, &aceCnt));

        for (i = 0; i < aceCnt ; i++)
        {
            SYS_MEM_CLEAR(old_ace_content);
            SYS_ERR_CHK(sal_ace_entry_byIdx_get(g_aclname.str, &old_ace_content, g_acltype, i));

            if (old_ace_content.priority == ace_content.priority)
            {
                SYS_ERR_CHK(sal_ace_entry_modify(g_aclname.str, &ace_content));

                return SYS_ERR_OK;
            }
        }
    }

    SYS_ERR_CHK(sal_ace_entry_create(g_aclname.str, &ace_content));

    return SYS_ERR_OK;
}

int32 cmd_acl_ace_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_ace_entry_del(g_aclname.str, SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}

int32 cmd_acl_bind_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp = 0;
    sys_acl_type_t aclType = SYS_ACL_TYPE_NONE;
    sys_acl_bind_names_t names;
    sys_acl_bind_names_t bindingNames[SYS_LOGICPORT_NUM_MAX];

    if (CMD_IS_CLI_RUN())
    {
        CLI_SLEEP_FOR_REMOTE();
    }

    if (osal_strlen(argv[1]) > CAPA_NAME_STR_LEN)
        return SYS_ERR_INPUT;

    SYS_ERR_CHK(sal_acl_entry_typeByName_get(argv[1], &aclType));

    if (SYS_ACL_MAC_BASE == aclType)
    {
        if ('m' != argv[0][0])
            return SYS_ERR_ACL_ILLEGAL_BINDING;
    }
    else if (SYS_ACL_IPV4_BASE == aclType)
    {
        if (0 != strcmp("ip", argv[0]))
            return SYS_ERR_ACL_ILLEGAL_BINDING;
    }
    else if (SYS_ACL_IPV6_BASE == aclType)
    {
        if (0 != strncmp("ipv6", argv[0], strlen(argv[0])))
            return SYS_ERR_ACL_ILLEGAL_BINDING;
    }

    SYS_MEM_CLEAR(names);
    SYS_MEM_CLEAR(bindingNames);

    osal_strcpy(names.aclname[aclType].str, argv[1]);
    SYS_ERR_CHK(sal_acl_entry_portBinding_get(bindingNames));

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        if (MAX_ACL_BINDTOIF == bindingNames[lp].cnt)
            return SYS_ERR_ACL_PORT_HAS_BINDED_ACL;

        if (SYS_ACL_MAC_BASE == aclType)
        {
            if (0 != osal_strlen(bindingNames[lp].aclname[SYS_ACL_MAC_BASE].str))
                return SYS_ERR_ACL_PORT_HAS_BINDED_TYPE;
        }
        else if (SYS_ACL_IPV4_BASE == aclType)
        {
            if (0 != osal_strlen(bindingNames[lp].aclname[SYS_ACL_IPV4_BASE].str))
                return SYS_ERR_ACL_PORT_HAS_BINDED_TYPE;

            if (0 != osal_strlen(bindingNames[lp].aclname[SYS_ACL_IPV6_BASE].str))
                return SYS_ERR_ACL_ILLEGAL_BINDING;
        }
        else if (SYS_ACL_IPV6_BASE == aclType)
        {
            if (0 != osal_strlen(bindingNames[lp].aclname[SYS_ACL_IPV6_BASE].str))
                return SYS_ERR_ACL_PORT_HAS_BINDED_TYPE;

            if (0 != osal_strlen(bindingNames[lp].aclname[SYS_ACL_IPV4_BASE].str))
                return SYS_ERR_ACL_ILLEGAL_BINDING;
        }
    }

    names.type = aclType;
    SYS_ERR_CHK(sal_acl_entry_bind(&names, g_selLpm));

    return SYS_ERR_OK;
}

int32 cmd_acl_unbind_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_acl_type_t aclType = SYS_ACL_TYPE_NONE;

    if (CMD_IS_CLI_RUN())
    {
        CLI_SLEEP_FOR_REMOTE();
    }

    if ('m' == argv[0][0])
        aclType = SYS_ACL_MAC_BASE;
    else if (0 == strcmp("ip", argv[0]))
        aclType = SYS_ACL_IPV4_BASE;
    else if (0 == strncmp("ipv6", argv[0], strlen(argv[0])))
        aclType = SYS_ACL_IPV6_BASE;

    SYS_ERR_CHK(sal_acl_entry_unbind(aclType, g_selLpm));

    return SYS_ERR_OK;
}

int32 cmd_acl_save(FILE *fp)
{
    char buf[SYS_BUF1024_LEN];
    uint32 i = 0;
    uint32 j = 0;
    uint32 aclType = 0;
    uint32 aclCnt = 0;
    uint32 aceCnt = 0;
    sys_namestr_t name;
    sys_ace_content_t ace_content;

    osal_memset(&buf, 0 , sizeof(buf));

    for (aclType = SYS_ACL_MAC_BASE; aclType < SYS_ACL_TYPE_END; aclType++)
    {
        SYS_ERR_CHK(sal_acl_entry_countByType_get(aclType, &aclCnt));

        for (i = 0; i < aclCnt ; i++)
        {
            osal_memset(&name, 0 , sizeof(name));
            osal_memset(&buf, 0 , strlen(buf));

            SYS_ERR_CHK(sal_acl_entry_nameByIdx_get(name.str, aclType, i));

            if (SYS_ACL_MAC_BASE == aclType)
                sprintf(buf, "mac acl \"%s\"\n", name.str);
            else if (SYS_ACL_IPV4_BASE == aclType)
                sprintf(buf, "ip acl \"%s\"\n", name.str);
            else if (SYS_ACL_IPV6_BASE == aclType)
                sprintf(buf, "ipv6 acl \"%s\"\n", name.str);

            CMD_WRITE_TO_FILE(fp, buf);

            SYS_ERR_CHK(sal_ace_entry_count_get(name.str, aclType, &aceCnt));

            for (j = 0; j < aceCnt ; j++)
            {
                osal_memset(&ace_content, 0 , sizeof(ace_content));
                SYS_ERR_CHK(sal_ace_entry_byIdx_get(name.str, &ace_content, aclType, j));

                osal_memset(&buf, 0 , strlen(buf));

                if (SYS_ACL_MAC_BASE == aclType)
                    SYS_ERR_CHK(cmd_acl_aceMac2cmd_convert(&ace_content, buf));
                else
                    SYS_ERR_CHK(cmd_acl_aceIp2cmd_convert(&ace_content, buf));

                CMD_WRITE_TO_FILE_INDENT1(fp, buf);
            }
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_acl_port_save(FILE *fp, sys_logic_port_t port)
{
    char portStr[CAPA_PORT_STR_LEN];
    char buf[SYS_BUF64_LEN];
    sys_acl_type_t aclType = SYS_ACL_TYPE_NONE;
    sys_acl_bind_names_t names[SYS_LOGICPORT_NUM_MAX];

    osal_memset(names, 0, sizeof(names));

    SYS_ERR_CHK(sal_acl_entry_portBinding_get(names));

    for (aclType = SYS_ACL_MAC_BASE; aclType < SYS_ACL_TYPE_END ; aclType++)
    {
        if (0 != osal_strlen(names[port].aclname[aclType].str))
        {
            osal_memset(buf, 0, sizeof(buf));
            osal_memset(portStr, 0, sizeof(portStr));
            LP2STR(port, portStr);

            if (SYS_ACL_MAC_BASE == aclType)
                sprintf(buf, "mac acl \"%s\"\n", names[port].aclname[aclType].str);
            else if (SYS_ACL_IPV4_BASE == aclType)
                sprintf(buf, "ip acl \"%s\"\n", names[port].aclname[aclType].str);
            else if (SYS_ACL_IPV6_BASE == aclType)
                sprintf(buf, "ipv6 acl \"%s\"\n", names[port].aclname[aclType].str);

            CMD_WRITE_TO_FILE_INDENT1(fp, buf);
        }
    }

    return SYS_ERR_OK;
}

