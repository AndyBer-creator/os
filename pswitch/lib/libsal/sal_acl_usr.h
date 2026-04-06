
#ifndef __SAL_ACL_H__
#define __SAL_ACL_H__

#include <rsd/rsd_acl_usr.h>

#define ACL_IP_PROTO_NUM        17
#define ACL_IPV6_PROTO_NUM       3
#define ACL_ICMP_TYPE_NUM       10
#define ACL_ICMPV6_TYPE_NUM     10
#define ACL_IGMP_TYPE_NUM       8
#define ACL_TCP_TYPE_NUM        21
#define ACL_UDP_TYPE_NUM        18

typedef struct sal_acl_proto_id_s
{
    char proto[SYS_BUF32_LEN];
    uint16 value;
} sal_acl_proto_id_t;

extern sal_acl_proto_id_t acl_ip_proto[ACL_IP_PROTO_NUM];
extern sal_acl_proto_id_t acl_ipv6_proto[ACL_IPV6_PROTO_NUM];
extern sal_acl_proto_id_t acl_icmp_type[ACL_ICMP_TYPE_NUM];
extern sal_acl_proto_id_t acl_icmpv6_type[ACL_ICMPV6_TYPE_NUM];
extern sal_acl_proto_id_t acl_igmp_type[ACL_IGMP_TYPE_NUM];
extern sal_acl_proto_id_t acl_tcp_type[ACL_TCP_TYPE_NUM];
extern sal_acl_proto_id_t acl_udp_type[ACL_UDP_TYPE_NUM];

extern int32 sal_acl_entry_create(char *name, sys_acl_type_t type);

extern int32 sal_acl_entry_del(char *name, sys_acl_type_t type);

extern int32 sal_acl_entry_bind(sys_acl_bind_names_t *names, sys_logic_portmask_t bindpm);

extern int32 sal_acl_entry_unbind(sys_acl_type_t type, sys_logic_portmask_t unbindpm);

extern int32 sal_ace_entry_create(char *name, sys_ace_content_t *ace_content);

extern int32 sal_ace_entry_modify(char *name, sys_ace_content_t *ace_content);

extern int32 sal_ace_entry_del(char *name, uint32 priority);

extern int32 sal_acl_l4port_range_set(sys_l4port_range_t *portRange, uint8 *index);

extern int32 sal_acl_l4port_range_get(sys_l4port_range_t *portRange, uint8 index);

extern int32 sal_acl_l4port_range_del(uint8 index);

extern int32 sal_acl_entry_countByType_get(sys_acl_type_t type, uint32 *pCnt);

extern int32 sal_acl_entry_nameByIdx_get(char *name, sys_acl_type_t type, uint32 idx);

extern int32 sal_acl_entry_typeByName_get(char *name, sys_acl_type_t *type);

extern int32 sal_ace_entry_count_get(char *name, sys_acl_type_t type, uint32 *pCnt);

extern int32 sal_ace_entry_byIdx_get(char *name, sys_ace_content_t *ace_content, sys_acl_type_t type, uint32 idx);

extern int32 sal_acl_entry_portBinding_get(sys_acl_bind_names_t *names);

extern int32 sal_acl_pieUsage_get(sys_logic_block_4show_t *pieUsage);

extern int32 sal_acl_isACLBind_ret(sys_namestr_t *pName, uint32 *pBind);
#endif

