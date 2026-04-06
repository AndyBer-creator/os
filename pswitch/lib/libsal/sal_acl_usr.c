
#include <libsal/sal_util.h>
#include <libsal/sal_acl_usr.h>

sal_acl_proto_id_t acl_ip_proto[ACL_IP_PROTO_NUM] =
{
    {   .proto = "icmp",        .value = 1, },

    {   .proto = "ipinip",      .value = 4, },
    {   .proto = "tcp",         .value = 6, },
    {   .proto = "egp",         .value = 8, },
    {   .proto = "igp",         .value = 9, },
    {   .proto = "udp",         .value = 17, },
    {   .proto = "hmp",         .value = 20, },
    {   .proto = "rdp",         .value = 27, },
    {   .proto = "ipv6",        .value = 41, },
    {   .proto = "ipv6:rout",   .value = 43, },
    {   .proto = "ipv6:frag",   .value = 44, },
    {   .proto = "rsvp",        .value = 46, },
    {   .proto = "ipv6:icmp",   .value = 58, },
    {   .proto = "ospf",        .value = 89, },
    {   .proto = "pim",         .value = 103, },
    {   .proto = "l2tp",        .value = 115, },
};

sal_acl_proto_id_t acl_ipv6_proto[ACL_IPV6_PROTO_NUM] =
{
    {   .proto = "tcp",         .value = 6, },
    {   .proto = "udp",         .value = 17, },
    {   .proto = "icmp",        .value = 58, },
};

sal_acl_proto_id_t acl_icmp_type[ACL_ICMP_TYPE_NUM] =
{
    {   .proto = "echo-reply",              .value = 0, },
    {   .proto = "destination-unreachable", .value = 3, },
    {   .proto = "source-quench",           .value = 4, },
    {   .proto = "echo-request",            .value = 8, },
    {   .proto = "router-advertisement",    .value = 9, },
    {   .proto = "router-solicitation",     .value = 10, },
    {   .proto = "time-exceeded",           .value = 11, },
    {   .proto = "timestamp",               .value = 13, },
    {   .proto = "timestamp-reply",         .value = 14, },
    {   .proto = "traceroute",              .value = 30, },
};

sal_acl_proto_id_t acl_igmp_type[ACL_IGMP_TYPE_NUM] =
{
    {   .proto = "host-query",      .value = 0x11, },
    {   .proto = "host-report",     .value = 0x12, },
    {   .proto = "dvmrp",           .value = 0x13, },
    {   .proto = "pim",             .value = 0x14, },
    {   .proto = "cisco-trace",     .value = 0x15, },
    {   .proto = "host-report-v2",  .value = 0x16, },
    {   .proto = "host-leave-v2",   .value = 0x17, },
    {   .proto = "host-report-v3",  .value = 0x22, },
};

sal_acl_proto_id_t acl_icmpv6_type[ACL_ICMPV6_TYPE_NUM] =
{
    {   .proto = "destination-unreachable", .value = 1, },
    {   .proto = "packet-too-big",          .value = 2, },
    {   .proto = "time-exceeded",           .value = 3, },
    {   .proto = "parameter-problem",       .value = 4, },
    {   .proto = "echo-request",            .value = 128, },
    {   .proto = "echo-reply",              .value = 129, },

    {   .proto = "router-solicitation",     .value = 133, },
    {   .proto = "router-advertisement",    .value = 134, },
    {   .proto = "nd-ns",                   .value = 135, },
    {   .proto = "nd-na",                   .value = 136, },
};

sal_acl_proto_id_t acl_tcp_type[ACL_TCP_TYPE_NUM] =
{
    {   .proto = "echo",            .value = 7, },
    {   .proto = "discard",         .value = 9, },
    {   .proto = "daytime",         .value = 13, },
    {   .proto = "ftp-data",        .value = 20, },
    {   .proto = "ftp",             .value = 21, },
    {   .proto = "telnet",          .value = 23, },
    {   .proto = "smtp",            .value = 25, },
    {   .proto = "time",            .value = 37, },
    {   .proto = "hostname",        .value = 42, },
    {   .proto = "whois",           .value = 43, },
    {   .proto = "tacacs-ds",       .value = 49, },
    {   .proto = "domain",          .value = 53, },
    {   .proto = "www",             .value = 80, },
    {   .proto = "pop2",            .value = 109, },
    {   .proto = "pop3",            .value = 110, },
    {   .proto = "syslog",          .value = 514, },
    {   .proto = "talk",            .value = 517, },
    {   .proto = "klogin",          .value = 543, },
    {   .proto = "kshell",          .value = 544, },
    {   .proto = "sunrpc",          .value = 111, },
    {   .proto = "drip",            .value = 3949, },
};

sal_acl_proto_id_t acl_udp_type[ACL_UDP_TYPE_NUM] =
{
    {   .proto = "echo",            .value = 7, },
    {   .proto = "discard",         .value = 9, },
    {   .proto = "time",            .value = 37, },
    {   .proto = "nameserver",      .value = 42, },
    {   .proto = "tacacs-ds",       .value = 49, },
    {   .proto = "domain",          .value = 53, },
    {   .proto = "bootps",          .value = 67, },
    {   .proto = "bootpc",          .value = 68, },
    {   .proto = "tftp",            .value = 69, },
    {   .proto = "sunrpc",          .value = 111, },
    {   .proto = "ntp",             .value = 123, },
    {   .proto = "netbios-ns",      .value = 137, },
    {   .proto = "snmp",            .value = 161, },
    {   .proto = "snmptrap",        .value = 162, },
    {   .proto = "who",             .value = 513, },
    {   .proto = "syslog",          .value = 514, },
    {   .proto = "talk",            .value = 517, },
    {   .proto = "rip",             .value = 520, },
};

int32 sal_acl_entry_create(char *name, sys_acl_type_t type)
{
    sys_namestr_t aclname;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((type >= SYS_ACL_TYPE_END) || (SYS_ACL_TYPE_NONE == type)), SYS_ERR_INPUT);

    osal_memset(&aclname, 0, sizeof(aclname));
    osal_strcpy(aclname.str, name);

    SYS_ERR_CHK(rsd_acl_entry_create(&aclname, type));

    return SYS_ERR_OK;
}

int32 sal_acl_entry_del(char *name, sys_acl_type_t type)
{
    sys_namestr_t aclname;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((type >= SYS_ACL_TYPE_END) || (SYS_ACL_TYPE_NONE == type)), SYS_ERR_INPUT);

    osal_memset(&aclname, 0, sizeof(aclname));
    osal_strcpy(aclname.str, name);

    SYS_ERR_CHK(rsd_acl_entry_remove(&aclname, type));

    return SYS_ERR_OK;
}

int32 sal_acl_entry_bind(sys_acl_bind_names_t *names, sys_logic_portmask_t bindpm)
{
    sys_logic_port_t lp;

    SYS_PARAM_CHK((NULL == names), SYS_ERR_NULL_POINTER);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, bindpm)
    {
        if (IS_LP_TRKMBR(lp))
            return SYS_ERR_ACL_PORT_TRKMBR_BIND_ACL;
    }

    SYS_ERR_CHK(rsd_acl_entry_bind(names, bindpm));

    return SYS_ERR_OK;
}

int32 sal_acl_entry_unbind(sys_acl_type_t type, sys_logic_portmask_t unbindpm)
{
    sys_logic_port_t lp;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, unbindpm)
    {
        if (IS_LP_TRKMBR(lp))
            return SYS_ERR_ACL_PORT_TRKMBR_BIND_ACL;
    }

    SYS_ERR_CHK(rsd_acl_entry_unbind(type, unbindpm));

    return SYS_ERR_OK;
}

int32 sal_ace_entry_create(char *name, sys_ace_content_t *ace_content)
{
    sys_namestr_t aclname;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == ace_content), SYS_ERR_NULL_POINTER);

    osal_memset(&aclname, 0, sizeof(aclname));
    osal_strcpy(aclname.str, name);

    SYS_ERR_CHK(rsd_acl_ace_entry_create(&aclname, ace_content));

    return SYS_ERR_OK;
}

int32 sal_ace_entry_modify(char *name, sys_ace_content_t *ace_content)
{
    sys_namestr_t aclname;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == ace_content), SYS_ERR_NULL_POINTER);

    osal_memset(&aclname, 0, sizeof(aclname));
    osal_strcpy(aclname.str, name);

    SYS_ERR_CHK(rsd_acl_ace_entry_modify(&aclname, ace_content));

    return SYS_ERR_OK;
}

int32 sal_ace_entry_del(char *name, uint32 priority)
{
    sys_namestr_t aclname;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

    osal_memset(&aclname, 0, sizeof(aclname));
    osal_strcpy(aclname.str, name);

    SYS_ERR_CHK(rsd_acl_ace_entry_del(&aclname, priority));

    return SYS_ERR_OK;
}

int32 sal_acl_l4port_range_set(sys_l4port_range_t *portRange, uint8 *index)
{
    SYS_PARAM_CHK((NULL == portRange), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_acl_l4port_range_set(portRange, index));

    return SYS_ERR_OK;
}

int32 sal_acl_l4port_range_get(sys_l4port_range_t *portRange, uint8 index)
{
    SYS_PARAM_CHK((NULL == portRange), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= PORT_RANGE_CHK_MAX), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_acl_l4port_range_get(portRange, index));

    return SYS_ERR_OK;
}

int32 sal_acl_l4port_range_del(uint8 index)
{
    SYS_PARAM_CHK((index >= PORT_RANGE_CHK_MAX), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_acl_l4port_range_del(index));

    return SYS_ERR_OK;
}

int32 sal_acl_entry_countByType_get(sys_acl_type_t type, uint32 *pCnt)
{
    SYS_PARAM_CHK((NULL == pCnt), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((type >= SYS_ACL_TYPE_END) || (SYS_ACL_TYPE_NONE == type)), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_acl_entry_countByType_get(type, pCnt));

    return SYS_ERR_OK;
}

int32 sal_acl_entry_nameByIdx_get(char *name, sys_acl_type_t type, uint32 idx)
{
    sys_namestr_t aclname;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((type >= SYS_ACL_TYPE_END) || (SYS_ACL_TYPE_NONE == type)), SYS_ERR_INPUT);

    osal_memset(&aclname, 0, sizeof(aclname));

    SYS_ERR_CHK(rsd_acl_entry_nameByIdx_get(&aclname, type, idx));

    osal_strcpy(name, aclname.str);

    return SYS_ERR_OK;
}

int32 sal_acl_entry_typeByName_get(char *name, sys_acl_type_t *type)
{
    sys_namestr_t aclname;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == type), SYS_ERR_NULL_POINTER);

    osal_memset(&aclname, 0, sizeof(aclname));
    osal_strcpy(aclname.str, name);

    SYS_ERR_CHK(rsd_acl_entry_typeByName_get(&aclname, type));

    return SYS_ERR_OK;
}

int32 sal_ace_entry_count_get(char *name, sys_acl_type_t type, uint32 *pCnt)
{
    sys_namestr_t aclname;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pCnt), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((type >= SYS_ACL_TYPE_END) || (SYS_ACL_TYPE_NONE == type)), SYS_ERR_INPUT);

    osal_memset(&aclname, 0, sizeof(aclname));
    osal_strcpy(aclname.str, name);

    SYS_ERR_CHK(rsd_ace_entry_count_get(&aclname, type, pCnt));

    return SYS_ERR_OK;
}

int32 sal_ace_entry_byIdx_get(char *name, sys_ace_content_t *ace_content, sys_acl_type_t type, uint32 idx)
{
    sys_namestr_t aclname;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == ace_content), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((type >= SYS_ACL_TYPE_END) || (SYS_ACL_TYPE_NONE == type)), SYS_ERR_INPUT);

    osal_memset(&aclname, 0, sizeof(aclname));
    osal_strcpy(aclname.str, name);

    SYS_ERR_CHK(rsd_ace_entry_byIdx_get(&aclname, ace_content, type, idx));

    return SYS_ERR_OK;
}

int32 sal_acl_entry_portBinding_get(sys_acl_bind_names_t *names)
{
    sys_logic_port_t lp;
    sys_acl_bind_names_t name;

    SYS_PARAM_CHK((NULL == names), SYS_ERR_NULL_POINTER);

    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(name);
        SYS_ERR_CHK(rsd_acl_entry_portBinding_get(&name, lp));
        osal_memcpy(&(names[lp]), &name, sizeof(sys_acl_bind_names_t));
    }

    return SYS_ERR_OK;
}

int32 sal_acl_pieUsage_get(sys_logic_block_4show_t *pieUsage)
{
    SYS_PARAM_CHK((NULL == pieUsage), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_acl_pieUsage_get(pieUsage));

    return SYS_ERR_OK;
}

int32 sal_acl_isACLBind_ret(sys_namestr_t *pName, uint32 *pBind)
{
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pBind), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_acl_isACLBind_ret(pName, pBind));

    return SYS_ERR_OK;
}
