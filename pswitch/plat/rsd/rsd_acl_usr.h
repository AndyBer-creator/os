
#ifndef __RSD_ACL_USR_H__
#define __RSD_ACL_USR_H__

#include <rtk/pie.h>
#include <rtk/acl.h>
#include <common/sys_def.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_acl.h>
#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
#include <rsd/rsd_acl_msw.h>
#endif
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
#include <rsd/rsd_acl_csw.h>
#endif

#define AGGCNT                  4
#define MAX_ACL_BINDTOIF        2
#define PORT_RANGE_CHK_MAX      16

typedef enum sys_acl_type_e
{
    SYS_ACL_TYPE_NONE,
    SYS_ACL_MAC_BASE,
    SYS_ACL_IPV4_BASE,
    SYS_ACL_IPV6_BASE,
    SYS_ACL_TYPE_END,
} sys_acl_type_t;

typedef enum sys_acl_tcp_flag_e
{
    SYS_ACL_TCP_FLAG_FIN,
    SYS_ACL_TCP_FLAG_SYN,
    SYS_ACL_TCP_FLAG_RST,
    SYS_ACL_TCP_FLAG_PSH,
    SYS_ACL_TCP_FLAG_ACK,
    SYS_ACL_TCP_FLAG_URG,
} sys_acl_tcp_flag_t;

typedef struct sys_l4port_range_s
{
    uint8 useCnt;
    uint16 portStart;
    uint16 portEnd;
    rtk_acl_rangeCheck_l4Port_direction_t direct;
} sys_l4port_range_t;

typedef struct sys_ace_mac_s
{
    sys_mac_t  dmac;
    sys_mac_t  smac;
    sys_mac_t  dmacMask;
    sys_mac_t  smacMask;

    uint16 vid;
    uint16 ethType;
    uint8 value8021p;
    uint8 mask8021p; 

    uint8 isCareSmac;
    uint8 isCareDmac;
    uint8 isCareVid;
    uint8 isCareEthtype;
    uint8 isCare8021p;
} sys_ace_mac_t;

typedef struct sys_ace_ip_s
{
    uint8 isV4;

    sys_ipv6_t v6sip;
    sys_ipv6_t v6dip;
    sys_ipv6_prefix_t v6sip_prefix;
    sys_ipv6_prefix_t v6dip_prefix;

    sys_ipv4_t v4sip;
    sys_ipv4_t v4dip;
    sys_ipv4_t v4sipMask;
    sys_ipv4_t v4dipMask;

    uint16 srcPort;
    uint16 dstPort;

    uint8 l4SrcPortRangeIdx;
    uint8 l4DstPortRangeIdx;

    uint8 protol;
    uint8 dscp;
    uint8 ipPrecedence;
    uint8 tcpflag;
    uint8 tcpflagMask;

    uint8 icmpType;
    uint8 icmpCode;
    uint8 igmpType;

    uint8 isCareSip;
    uint8 isCareDip;
    uint8 isCareSrcPort;
    uint8 isCareDstPort;
    uint8 isCareSrcPortRange;
    uint8 isCareDstPortRange;
    uint8 isCareProto;
    uint8 isCareDscp;
    uint8 isCareIpPrecedence;
    uint8 isCareTcpFlag;
    uint8 isCareICMPType;
    uint8 isCareICMPCode;
    uint8 isCareIGMPType;
} sys_ace_ip_t;

typedef struct sys_logic_pie_s
{
    struct sys_logic_pie_s *prev;
    struct sys_logic_pie_s *next;
    struct sys_ace_s *ace;
    rtk_pie_phase_t usedPhase;
    uint16 entryIdx[AGGCNT];
    uint8 lBlkArrayIdx;
    uint8 inUsed;
} sys_logic_pie_t;

typedef struct sys_ace_content_s
{
    sys_pri_t priority;
    sys_pkt_action_t action;

    union
    {
        sys_ace_mac_t mac;
        sys_ace_ip_t ip;
    };
} sys_ace_content_t;

typedef struct sys_ace_s
{
    struct sys_ace_s *prev;
    struct sys_ace_s *next;
    sys_acl_type_t type;
    sys_ace_content_t ace_content;
} sys_ace_t;

typedef struct sys_acl_s
{
    struct sys_acl_s *prev;
    struct sys_acl_s *next;
    struct sys_ace_s *ace_head;
    sys_logic_pie_t *lpie_head;
    sys_logic_portmask_t acl_binded_pm;
    sys_namestr_t name;
    sys_acl_type_t type;
    sys_ace_t denyAll;
    uint8 aceCnt;
    uint8 idx;
} sys_acl_t;

typedef struct sys_acl_bind_names_s
{
    uint8 cnt;
    sys_namestr_t aclname[SYS_ACL_TYPE_END];
    sys_acl_type_t  type;
} sys_acl_bind_names_t;

extern int32 rsd_acl_entry_bind(sys_acl_bind_names_t *names, sys_logic_portmask_t bindpm);

extern int32 rsd_acl_entry_unbind(sys_acl_type_t type, sys_logic_portmask_t unbindpm);

extern int32 rsd_acl_entry_create(sys_namestr_t *name, sys_acl_type_t type);

extern int32 rsd_acl_entry_remove(sys_namestr_t *name, sys_acl_type_t type);

extern int32 rsd_acl_ace_entry_create(sys_namestr_t *aclname, sys_ace_content_t *ace_content);

extern int32 rsd_acl_ace_entry_modify(sys_namestr_t *aclname, sys_ace_content_t *ace_content);

extern int32 rsd_acl_ace_entry_del(sys_namestr_t *aclname, uint32 priority);

extern int32 rsd_acl_l4port_range_set(sys_l4port_range_t *portRange, uint8 *index);

extern int32 rsd_acl_l4port_range_get(sys_l4port_range_t *portRange, uint8 index);

extern int32 rsd_acl_l4port_range_del(uint8 index);

extern int32 rsd_acl_entry_countByType_get(sys_acl_type_t type, uint32 *pCnt);

extern int32 rsd_acl_entry_nameByIdx_get(sys_namestr_t *name, sys_acl_type_t type, uint32 idx);

extern int32 rsd_acl_entry_typeByName_get(sys_namestr_t *name, sys_acl_type_t *type);

extern int32 rsd_ace_entry_count_get(sys_namestr_t *aclname, sys_acl_type_t type, uint32 *pCnt);

extern int32 rsd_ace_entry_byIdx_get(sys_namestr_t *aclname, sys_ace_content_t *ace_content, sys_acl_type_t type, uint32 idx);

extern int32 rsd_acl_entry_portBinding_get(sys_acl_bind_names_t *names, sys_logic_port_t lp);

extern int32 rsd_acl_pieUsage_get(sys_logic_block_4show_t *pieUsage);

extern int32 rsd_acl_isACLBind_ret(sys_namestr_t *pName, uint32 *pBind);

extern int32 rsd_acl_usr_init(void);
#endif

