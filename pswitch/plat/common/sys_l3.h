
#ifndef __COMMON_SYS_L3_H__
#define __COMMON_SYS_L3_H__
#include <common/sys_type.h>
#define L3_INTF_INACTIVE    "inactive"
#define L3_INTF_LO          "lo"
#define L3_INTF_PREFIX_LEN  5
#define L3_INTF_PREFIX      "intf"
#define L3_INTF_PREFIX_VLAN "intfv"
#define L3_INTF_PREFIX_VRRP "vrrp"
#define L3_INTF_PREFIX_VRRP_LEN  4
#define L3_INTF_MGMTVLAN    "intfm"
#define L3_INTF_PREFIX_LO   "intfl"
#define L3_INTF_NAME_LO     "Loopback"
#define L3_INTF_NAME_VLAN   "VLAN "
#define L3_INTF_NULL        "Null0"
#define L3_INTF_NAME_VRRP   "VRRP "
#define L3DB_MAJOR 125
#define L3DB_NAME  "l3db"
#define L3DB_DEV   "/dev/"L3DB_NAME
typedef enum sys_l3_act_e
{
    SYS_L3_ACT_NONE = 0,
    SYS_L3_ACT_ROUTEADD,
    SYS_L3_ACT_ROUTEDEL,
    SYS_L3_ACT_END
} sys_l3_act_t;

typedef enum sys_l3_af_e
{
    SYS_L3_AF_IPV4 = 0,
    SYS_L3_AF_IPV6,
    SYS_L3_AF_END
} sys_l3_af_t;

typedef enum sys_l3_intfType_e
{
    SYS_L3_INTFTYPE_INACTIVE = 0,
    SYS_L3_INTFTYPE_LO,
    SYS_L3_INTFTYPE_VLAN,
    SYS_L3_INTFTYPE_MGMTVLAN,
    SYS_L3_INTFTYPE_NULL,
    SYS_L3_INTFTYPE_VRRP,
    SYS_L3_INTFTYPE_END
} sys_l3_intfType_t;

typedef enum sys_l3_intfLinkStatus_e
{
    SYS_L3_INTFLINK_DOWN = 0,
    SYS_L3_INTFLINK_UP,
    SYS_L3_INTFLINK_ADMINIPCHG,
    SYS_L3_INTFLINK_END
} sys_l3_intfLinkStatus_t;

typedef enum sys_l3_ipType_e
{
    SYS_L3_IPTYPE_EMPTY = 0,
    SYS_L3_IPTYPE_STATIC,
    SYS_L3_IPTYPE_DHCP,
    SYS_L3_IPTYPE_V6LINKLOCAL,
    SYS_L3_IPTYPE_V6AUTOCONF,
    SYS_L3_IPTYPE_V6MCAST,
    SYS_L3_IPTYPE_ANY,
    SYS_L3_IPTYPE_VRRP,
    SYS_L3_IPTYPE_END,
} sys_l3_ipType_t;

typedef enum sys_l3_routeType_e
{
    SYS_L3_ROUTETYPE_UNKNOWN = 0,
    SYS_L3_ROUTETYPE_KERNEL,
    SYS_L3_ROUTETYPE_CONNECT,
    SYS_L3_ROUTETYPE_STATIC,
    SYS_L3_ROUTETYPE_RIP,
    SYS_L3_ROUTETYPE_RIPNG,
    SYS_L3_ROUTETYPE_OSPF,
    SYS_L3_ROUTETYPE_OSPF6,
    SYS_L3_ROUTETYPE_ISIS,
    SYS_L3_ROUTETYPE_BGP,
    SYS_L3_ROUTETYPE_HSLS,
    SYS_L3_ROUTETYPE_END
} sys_l3_routeType_t;

typedef enum sys_l3_dhcpType_e
{
    SYS_L3_DHCPTYPE_START = 0,
    SYS_L3_DHCPTYPE_DISABLE,
    SYS_L3_DHCPTYPE_DHCP,
    SYS_L3_DHCPTYPE_BOOTP,
    SYS_L3_DHCPTYPE_END
} sys_l3_dhcpType_t;

typedef enum sys_l3_neighType_e
{
    SYS_L3_NEIGH_V4_DYNAMIC = 0,
    SYS_L3_NEIGH_V4_STATIC,
    SYS_L3_NEIGH_V6_DYNAMIC,
    SYS_L3_NEIGH_V6_STATIC,
    SYS_L3_NEIGH_END
} sys_l3_neighType_t;
typedef enum sys_l3_ipv6DadSts_e
{
    SYS_L3_IPV6_DADSTS_NONE = 0,
    SYS_L3_IPV6_DADSTS_ACTIVE,
    SYS_L3_IPV6_DADSTS_TENTATIVE,
    SYS_L3_IPV6_DADSTS_DUPLICATE,
    SYS_L3_IPV6_DADSTS_END
} sys_l3_ipv6DadSts_t;

typedef enum sys_l3_ipv6_raPref_e
{
    SYS_L3_IPV6_RAPREF_LOW = 0,
    SYS_L3_IPV6_RAPREF_MEDIUM,
    SYS_L3_IPV6_RAPREF_HIGH,
    SYS_L3_IPV6_RAPREF_END
} sys_l3_ipv6_raPref_t;

typedef struct sys_l3_intfId_s
{
    sys_l3_intfType_t type;
    uint32            id;
    uint32            vrid;
} sys_l3_intfId_t;

typedef struct sys_l3_ipv4_s
{
    sys_ipv4_t addr; 
    uint32     plen; 
} sys_l3_ipv4_t;

typedef struct sys_l3_ipv6_s
{
    sys_ipv6_t      addr;   
    uint32          plen;   
} sys_l3_ipv6_t;

typedef union sys_l3_ip_u
{
    sys_l3_ipv4_t v4;
    sys_l3_ipv6_t v6;
} sys_l3_ip_t;

typedef union sys_ip_u
{
    sys_ipv4_t v4;
    sys_ipv6_t v6;
} sys_ip_t;

typedef struct sys_l3_intfIPv6AddrsInfo_s
{
    uint32     cnt;
    sys_ipv6_t addr[CAPA_SYS_INTF_IPV6_ADDR_NUM];
} sys_l3_intfIPv6AddrsInfo_t;

typedef struct sys_l3_ipv4Admin_s
{
    sys_enable_t dhcpEnbl;
} sys_l3_ipv4Admin_t;

typedef struct sys_l3_intfDhcp6c_s
{
    sys_enable_t enbl;
#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS
    sys_enable_t stateless;
    uint32       refreshtime;
    uint32       min_refreshtime;
#endif
} sys_l3_intfDhcp6c_t;
typedef struct sys_l3_ipv6Admin_s
{
    uint8               enbl;
    uint8               mode;
    uint8               autoconfEnbl;
    uint8               unreachMsgSend;
    uint8               mldVer;
    uint8               redirect;
    uint16              dadTxNum;
    sys_l3_intfDhcp6c_t dhcp6c;
} sys_l3_ipv6Admin_t;

typedef struct sys_l3_intfAdmin_s
{
    uint16             enbl;
    uint16             exist;

    sys_l3_ipv4Admin_t ipv4;
    sys_l3_ipv6Admin_t ipv6;
} sys_l3_intfAdmin_t;

typedef struct sys_l3_intfMgmtAdmin_s
{
    struct {
        sys_l3_dhcpType_t dhcpType;
        sys_l3_ipv4_t     ip;
        sys_ipv4_t        gw;
    } v4;

    struct {
        sys_enable_t  enbl;
        sys_enable_t  autoconfEnbl;
        sys_enable_t  dhcpEnbl;
        sys_ipv6_t    gw;
        sys_l3_ipv6_t ip[CAPA_L3_INTFM_IPV6ADMIN_NUM];
    } v6;

    struct {
        sys_l3_dhcpType_t dhcpType;
        sys_l3_ipv4_t     ip;
        sys_ipv4_t        gw;
    } subv4;
} sys_l3_intfMgmtAdmin_t;

typedef struct sys_l3_intfMgmt_s
{
    sys_vid_t              mgmtVlan;
    sys_l3_ipv4_t          ipv4;
    sys_l3_ipv4_t          subIpv4;
    sys_l3_ipv6_t          ipv6[CAPA_L3_INTFM_IPV6STATUS_NUM];

    sys_l3_intfMgmtAdmin_t admin;
} sys_l3_intfMgmt_t;

typedef struct sys_l3_intfStsChg_s
{
    uint32 stsChg;
    uint32 process;
    uint32 linkup;
} sys_l3_intfStsChg_t;

typedef struct sys_l3_intf_s
{
    sys_dbofs_t          ipv4Head;
    sys_dbofs_t          ipv6Head;
    uint16               ipv4AddrNum;
    uint16               ipv6AddrNum;
    uint8                ipv4Enbl;
    uint8                ipv6Enbl;
    uint8                exist;
    uint8                linkup;
    int32                devIdx;
	int32                id;
} sys_l3_intf_t;

typedef struct sys_l3_routeV4_s
{
    sys_l3_intfId_t ointf;
    int16           pri;
    uint8           __padding;
    uint8           dstLen;
    sys_ipv4_t      dst;
    sys_ipv4_t      gw;
    char            descp[CAPA_L3_STATICROUTE_IPV4_DESCP_LEN + 1];
} sys_l3_routeV4_t;

typedef struct sys_l3_routeV6_s
{
    sys_l3_intfId_t ointf;
    int16           pri;
    uint8           __padding;
    uint8           dstLen;
    sys_ipv6_t      dst;
    sys_ipv6_t      gw;
} sys_l3_routeV6_t;

typedef struct sys_l3_routeV4Db_s
{
    struct sys_l3_routeV4Db_s *prev;
    struct sys_l3_routeV4Db_s *next;

    sys_l3_routeV4_t          entry;
    uint32                    active;
} sys_l3_routeV4Db_t;

typedef struct sys_l3_routeV6Db_s
{
    struct sys_l3_routeV6Db_s *prev;
    struct sys_l3_routeV6Db_s *next;

    sys_l3_routeV6_t          entry;
    uint32                    active;
} sys_l3_routeV6Db_t;

typedef struct sys_l3_staticRouteV4_s
{
    uint32           num;
    uint32           dfltRouteNum;
    sys_l3_routeV4_t route[CAPA_L3_STATICROUTE_IPV4_ENTRY_NUM_MAX];
} sys_l3_staticRouteV4_t;

typedef struct sys_l3_staticRouteV6_s
{
    uint32           num;
    sys_l3_routeV6_t route[CAPA_L3_STATICROUTE_IPV6_ENTRY_NUM_MAX];
} sys_l3_staticRouteV6_t;

typedef struct sys_l3_staticRoute_u
{
    sys_l3_staticRouteV4_t v4;
    sys_l3_staticRouteV6_t v6;
} sys_l3_staticRoute_t;

typedef struct sys_l3_routeV4Info_s
{
    uint8              select;
    uint8              fib;
    uint8              active;
    uint8              metric;
    uint32             distance;
    sys_l3_routeType_t type;
    sys_l3_routeV4_t   route;
} sys_l3_routeV4Info_t;

typedef struct sys_l3_routeV6Info_s
{
    uint8              select;
    uint8              fib;
    uint8              active;
    uint8              metric;
    uint32             distance;
    sys_l3_routeType_t type;
    sys_l3_routeV6_t   route;
} sys_l3_routeV6Info_t;

typedef struct sys_l3_routeV4InfoDb_s
{
    uint32               num;
    sys_l3_routeV4Info_t info[CAPA_L3_ROUTE_IPV4_ENTRY_NUM];
} sys_l3_routeV4InfoDb_t;

typedef struct sys_l3_routeV6InfoDb_s
{
    uint32               num;
    sys_l3_routeV6Info_t info[CAPA_L3_ROUTE_IPV6_ENTRY_NUM];
} sys_l3_routeV6InfoDb_t;

typedef struct sys_l3_ipv6MaddrInfo_s
{
    uint32     num;
    sys_ipv6_t addrs[CAPA_L3_IPV6_MADDR_ENTRY_NUM];
} sys_l3_ipv6MaddrInfo_t;

typedef struct sys_l3_hrouteEntry_s
{
    struct sys_l3_hrouteEntry_s *prev;
    struct sys_l3_hrouteEntry_s *next;

    void *n;
    sys_vid_t vid;
    sys_ip_t ip;
    sys_mac_t mac;
    uint16 reserve;
    sys_l3_neighType_t type;
    sys_l3_af_t af;
    sys_logic_port_t lp;
    uint32 ageTime;
    uint32 lastUpdate; 
    uint32 valid;
    uint32 isRouter; 
    uint32 nud_state;
    uint32 renew; 
    uint32 idx; 
} sys_l3_hrouteEntry_t;

typedef struct sys_l3_dbSort_s
{
    sys_dbofs_t   entryOfs;
} sys_l3_dbSort_t;

typedef struct sys_l3_ipDbHead_s
{
    uint32      maxNum;
    uint32      currNum;
    uint32      llAddrNum;
} sys_l3_ipDbHead_t;

typedef struct sys_l3_ipDbList_s
{
    sys_dbofs_t     prev;
    sys_dbofs_t     next;
    sys_l3_ipType_t type;
} sys_l3_ipDbList_t;

typedef struct sys_l3_ipv4Db_s
{
    sys_l3_ipDbList_t list;
    sys_l3_ipv4_t     ip;
    sys_l3_intfId_t   intfId;
} sys_l3_ipv4Db_t;

typedef struct sys_l3_ipv6Db_s
{
    sys_l3_ipDbList_t      list;
    sys_l3_ipv6_t          ip;
    sys_l3_intfId_t        intfId;
} sys_l3_ipv6Db_t;

typedef struct sys_l3_devMap_s
{
    int32 vlan[CAPA_L3_INTFVLAN_NUM];
    int32 lo[CAPA_L3_INTFLO_NUM];
    int32 vrrp[CAPA_L3_INTFVRRP_NUM];
} sys_l3_devMap_t;

typedef struct sys_l3_dbofs_s {
    sys_dbofs_t intfMgmt;
#ifdef CONFIG_SYS_L3_ROUTE
    sys_dbofs_t intfVlan;
    sys_dbofs_t intfLo;
    sys_dbofs_t intfVrrp;
    sys_dbofs_t devMap;
    sys_dbofs_t intfIpv4Pool;
    sys_dbofs_t intfIpv6Pool;
    sys_dbofs_t intfIpv4Sort;
    sys_dbofs_t intfIpv6Sort;
#endif
} sys_l3_dbofs_t;

typedef struct sys_l3_dbHead_s {
    uint32         dbSize;
    sys_l3_dbofs_t ofs;
    sys_ipv6_t     dfltIPv6LinkLocal;
#ifdef CONFIG_SYS_L3_ROUTE
    uint8          intfvNum;
    uint8          intflNum;
    uint8          intfvrrpNum;
    uint16         __padding;
#endif
} sys_l3_dbHead_t;

typedef struct sys_l3_intfdefine_s
{
    sys_l3_intfId_t        defId;
    struct {
        sys_l3_dhcpType_t dhcpType;
        sys_l3_ipv4_t     ip;
    } v4;
    
    struct {
        sys_enable_t  enbl;
        sys_enable_t  autoconfEnbl;
        sys_enable_t  dhcpEnbl;
        sys_l3_ipv6_t ip[CAPA_L3_INTFM_IPV6ADMIN_NUM];
    } v6;
} sys_l3_intfdef_t;

extern sys_l3_dbHead_t *g_l3Db;
extern sys_l3_intfMgmt_t *g_l3DbIntfMgmt;

#ifdef CONFIG_SYS_L3_ROUTE
extern sys_l3_intf_t   *g_l3DbIntfVlan;
extern sys_l3_intf_t   *g_l3DbIntfLo;
extern sys_l3_devMap_t *g_l3DbDevMap;
extern sys_l3_ipDbHead_t *g_l3DbIntfIPv4Pool;
extern sys_l3_ipDbHead_t *g_l3DbIntfIPv6Pool;
extern sys_l3_dbSort_t   *g_l3DbIntfIPv4Sort;
extern sys_l3_dbSort_t   *g_l3DbIntfIPv6Sort;
#ifdef CONFIG_SYS_L3_VRRP
extern sys_l3_intf_t     *g_l3DbIntfVrrp;
#endif
#endif
  #ifdef __KERNEL__
#define L3_DB_MAP() (TRUE)
  #else
#define L3_DB_MAP() (sys_l3_dbMem_map())
  #endif
#define L3_DB_PTR(ofs) ((0 == ofs) ? NULL : ((void*)g_l3Db + ofs))
#define L3_DB_OFS(ptr) ((NULL == ptr) ? 0 : ((void*)ptr - (void*)g_l3Db))
#define L3_DB_IPV4_PTR(ofs) ((sys_l3_ipv4Db_t*)(L3_DB_PTR(ofs)))
#define L3_DB_IPV4_EMPTYPTR() ((sys_l3_ipv4Db_t*) sys_l3_ipPoolEmptyEntryPtr_ret(SYS_L3_AF_IPV4))
#define L3_DB_IPV6_PTR(ofs) ((sys_l3_ipv6Db_t*)(L3_DB_PTR(ofs)))
#define L3_DB_IPV6_EMPTYPTR() ((sys_l3_ipv6Db_t*) sys_l3_ipPoolEmptyEntryPtr_ret(SYS_L3_AF_IPV6))
#define L3_DB_INTFNUM(type) (sys_l3_intfNum_ret(type))
#define L3_DB_INTFV(vid) g_l3DbIntfVlan[vid]
#define L3_DB_INTFM_ID() g_l3DbIntfMgmt->mgmtVlan
#define L3_DB_INTFM_ADMIN_PTR() &g_l3DbIntfMgmt->admin
#define IS_L3_INTFV_EXIST(vid) (TRUE == g_l3DbIntfVlan[vid].exist)
#define IS_L3_INTFV_LINKUP(vid) (TRUE == g_l3DbIntfVlan[vid].linkup)
#define IS_L3_INTFV_IPV4ENBL(vid) (TRUE == g_l3DbIntfVlan[vid].ipv4Enbl)
#define IS_L3_INTFV_IPV6ENBL(vid) (TRUE == g_l3DbIntfVlan[vid].ipv6Enbl)
#define L3_DB_INTFL(id) g_l3DbIntfLo[id]
#define IS_L3_INTFL_EXIST(id) (TRUE == g_l3DbIntfLo[id].exist)
#define IS_L3_INTFL_LINKUP(id) (TRUE == g_l3DbIntfLo[id].linkup)
#define IS_L3_INTFL_IPV4ENBL(id) (TRUE == g_l3DbIntfLo[id].ipv4Enbl)
#define IS_L3_INTFL_IPV6ENBL(id) (TRUE == g_l3DbIntfLo[id].ipv6Enbl)
#ifdef CONFIG_SYS_L3_VRRP
#define L3_DB_INTFVRRP(vrid) g_l3DbIntfVrrp[vrid]
#define IS_L3_INTFVRRP_EXIST(vrid) (TRUE == g_l3DbIntfVrrp[vrid].exist)
#define IS_L3_INTFVRRP_LINKUP(vrid) (TRUE == g_l3DbIntfVrrp[vrid].linkup)
#define IS_L3_INTFVRRP_IPV4ENBL(vrid) (TRUE == g_l3DbIntfVrrp[vrid].ipv4Enbl)
#define IS_L3_INTFVRRP_IPV6ENBL(vrid) (TRUE == g_l3DbIntfVrrp[vrid].ipv6Enbl)
#endif
#define L3_INTF_IPV4_NUM(pIntfId) (sys_l3_intfIpAddrNum_ret(SYS_L3_AF_IPV4, pIntfId))
#define L3_INTF_IPV6_NUM(pIntfId) (sys_l3_intfIpAddrNum_ret(SYS_L3_AF_IPV6, pIntfId))
#define IS_MGMT_VLAN_IPV4_ACTIVE() (sys_l3_intfMgmtVlanActive_ret(SYS_L3_AF_IPV4))
#define IS_MGMT_VLAN_IPV6_ACTIVE() (sys_l3_intfMgmtVlanActive_ret(SYS_L3_AF_IPV6))
#define IS_MGMT_VID(vid) (sys_l3_isVidMgmtVlan_ret(vid))
#define IS_L3_INTF_EXIST(pIntfId) (sys_l3_intfExist_ret(pIntfId))
#define IS_L3_INTF_LINKUP(pIntfId) (sys_l3_intfLinkup_ret(pIntfId))
#define IS_L3_INTF_IPV4ENBL(pIntfId) (sys_l3_intfIPv4Enbl_ret(pIntfId))
#define IS_L3_INTF_IPV6ENBL(pIntfId) (sys_l3_intfIPv6Enbl_ret(pIntfId))
#define IS_L3_INTF_IPV4_EXIST(pIntfId, pIp) (sys_l3_intfIpExist_ret(SYS_L3_AF_IPV4, pIntfId, (sys_l3_ip_t *)pIp))
#define IS_L3_INTF_IPV6_EXIST(pIntfId, pIp) (sys_l3_intfIpExist_ret(SYS_L3_AF_IPV6, pIntfId, (sys_l3_ip_t *)pIp))
#define IS_L3_INTF_EQUAL(pIntfId1, pIntfId2) (0 == osal_memcmp(pIntfId1, pIntfId2, sizeof(sys_l3_intfId_t)))
#define FOR_EACH_L3_IPV4(__i, __pDb) \
    for (__i = 0; __i < sys_l3_ipPoolNum_ret(SYS_L3_AF_IPV4); __i++) \
        if (NULL != (__pDb = (sys_l3_ipv4Db_t*)sys_l3_ipPoolSortEntryPtr_ret(SYS_L3_AF_IPV4, __i)))
#define FOR_EACH_L3_IPV4_IN_INTF(__pIntfId, __pDb) \
    while (NULL != (__pDb = (sys_l3_ipv4Db_t*)sys_l3_ipPoolIntfNextEntryPtr_ret(SYS_L3_AF_IPV4, __pIntfId, (sys_l3_ipDbList_t*)__pDb)))
#define FOR_EACH_L3_IPV6(__i, __pDb) \
    for (__i = 0; __i < sys_l3_ipPoolNum_ret(SYS_L3_AF_IPV6); __i++) \
        if (NULL != (__pDb = (sys_l3_ipv6Db_t*)sys_l3_ipPoolSortEntryPtr_ret(SYS_L3_AF_IPV6, __i)))
#define FOR_EACH_L3_IPV6_IN_INTF(__pIntfId, __pDb) \
    while (NULL != (__pDb = (sys_l3_ipv6Db_t*)sys_l3_ipPoolIntfNextEntryPtr_ret(SYS_L3_AF_IPV6, __pIntfId, (sys_l3_ipDbList_t*)__pDb)))

#define INTFID2IDX(pId) (sys_l3_intfId2Idx_ret(pId))
#define INTFIDX2ID(type, idx) (sys_l3_intfIdx2Id_ret(type, idx))
#define INTFID2STR(pId, pStr) (sys_l3_intfId2Str(pId, pStr))
#define INTFID2NAME(pId, pStr) (sys_l3_intfId2Name(pId, pStr))
#define STR2INTFID(pStr,pId) (sys_l3_str2IntfId(pStr, pId))
#define STR2INTFNAME(pDev,pName) (sys_l3_str2IntfName(pDev, pName))
#define INTFNAME2ID(pName, pId) (sys_l3_intfName2Id(pName, pId))
#define INTFNAME2STR(pName, pStr) (sys_l3_intfName2Str(pName, pStr))

#define IPV4_MASK2PFX(mask) (sys_l3_ipv4Mask2Prefix_ret(mask))
#define IPV4_PFX2MASK(pfx) (sys_l3_ipv4Prefix2Mask_ret(pfx))
#define IPV4_L3IP2SUBNET(pIp, pSubnet) (sys_l3_ip2Subnet_get(SYS_L3_AF_IPV4, (sys_l3_ip_t *)pIp, (sys_l3_ip_t *)pSubnet))
#define IS_IPV6_LOOPBACK(ip) (ip.hex[0] == 0 && ip.hex[1] == 0 && ip.hex[2] == 0 && ip.hex[3] == 0 && \
                              ip.hex[4] == 0 && ip.hex[5] == 0 && ip.hex[6] == 0 && ip.hex[7] == 0 && \
                              ip.hex[8] == 0 && ip.hex[9] == 0 && ip.hex[10] == 0 && ip.hex[11] == 0 && \
                              ip.hex[12] == 0 && ip.hex[13] == 0 && ip.hex[14] == 0 && ip.hex[15] == 0x1) 
#define IS_IPV6_LINKLOCAL(ip) ((ip.hex[0] == 0xFE) && ((ip.hex[1] & 0xC0) == 0x80)) 
#define IS_IPV6_MCAST(ip) ((ip.hex[0] == 0xFF))
#define IS_IPV6_UNIQLOCA_UCAST(ip) ((ip.hex[0] & 0xFE) == 0xFC)                     
#define IS_IPV4_ALLZERO(ip) (0 == ip)
#define IS_IPV4_BCAST(ip) (0xFFFFFFFF == ip)
#define IS_IPV4_MCAST(ip) ((ip >> 28) == 14)
#define IS_IPV6_ALLZERO(ip) (sys_l3_isIPv6AllZero_ret(ip))
#define IS_IPV4_EQUAL(ip1, ip2) (ip1 == ip2)
#define IS_IPV6_EQUAL(ip1, ip2) (0 == osal_memcmp(&ip1, &ip2, CAPA_IPV6_BYTE_NUM))
#define IS_IPV4_IN_SUBNET(pIp, pSubnet) (sys_l3_ipInSubnet_ret(SYS_L3_AF_IPV4, (sys_ip_t *)pIp, (sys_l3_ip_t *)pSubnet))
#define IS_IPV6_IN_SUBNET(pIp, pSubnet) (sys_l3_ipInSubnet_ret(SYS_L3_AF_IPV6, (sys_ip_t *)pIp, (sys_l3_ip_t *)pSubnet))
#define IPV6_MASK2PFX(mask) (sys_l3_ipv6Mask2Prefix_ret(mask))
#define IPV6_PFX2MASK(pfx, mask) (sys_l3_ipv6Prefix2Mask_ret(pfx, mask))
#define IPV6_L3IP2SUBNET(pIp, pSubnet) (sys_l3_ip2Subnet_get(SYS_L3_AF_IPV6, (sys_l3_ip_t *)pIp, (sys_l3_ip_t *)pSubnet))

#define FMT_IPV4 "%u.%u.%u.%u"

#define SHOW_IPV4(addr) \
        ((unsigned char *)&addr)[0], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[3]

#define FMT_IPV6 "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x"

#define SHOW_IPV6(addr) \
        ((addr).hex[0]), \
        ((addr).hex[1]), \
        ((addr).hex[2]), \
        ((addr).hex[3]), \
        ((addr).hex[4]), \
        ((addr).hex[5]), \
        ((addr).hex[6]), \
        ((addr).hex[7]), \
        ((addr).hex[8]), \
        ((addr).hex[9]), \
        ((addr).hex[10]), \
        ((addr).hex[11]), \
        ((addr).hex[12]), \
        ((addr).hex[13]), \
        ((addr).hex[14]), \
        ((addr).hex[15])
extern int32 sys_l3_init(void);
extern uint32 sys_l3_dbMem_map(void);

#ifdef CONFIG_SYS_L3_ROUTE
extern uint32 sys_l3_ipPoolNum_ret(sys_l3_af_t af);
extern sys_l3_ipDbList_t *sys_l3_ipPoolSortEntryPtr_ret(sys_l3_af_t af, int32 idx);
extern sys_l3_ipDbList_t *sys_l3_ipPoolIntfEntryPtr_ret(sys_l3_af_t af, sys_l3_intfId_t *pId, sys_l3_ip_t *pIp);
extern sys_l3_ipDbList_t *sys_l3_ipPoolIntfNextEntryPtr_ret(sys_l3_af_t af, sys_l3_intfId_t *pId, sys_l3_ipDbList_t *pIpDb);
#endif
extern int32 sys_l3_intfId2Idx_ret(sys_l3_intfId_t *pId);
extern int32 sys_l3_intfIdx2Id_ret(sys_l3_intfType_t type, int32 idx);
extern int32 sys_l3_intfId2Str(sys_l3_intfId_t *pId, char *devStr);
extern uint32 sys_l3_intfExist_ret(sys_l3_intfId_t *pId);
extern uint32 sys_l3_intfMgmtVlanActive_ret(sys_l3_af_t af);
extern uint32 sys_l3_isVidMgmtVlan_ret(sys_vid_t vid);
extern uint32 sys_l3_intfLinkup_ret(sys_l3_intfId_t *pId);
extern uint32 sys_l3_intfIPv4Enbl_ret(sys_l3_intfId_t *pId);
extern uint32 sys_l3_intfIPv6Enbl_ret(sys_l3_intfId_t *pId);
extern uint32 sys_l3_intfIpAddrNum_ret(sys_l3_af_t af, sys_l3_intfId_t *pId);
extern uint32 sys_l3_intfIpExist_ret(sys_l3_af_t af, sys_l3_intfId_t *pId, sys_l3_ip_t *pIp);
extern uint32 sys_l3_intfNum_ret(sys_l3_intfType_t intfType);
extern int32 sys_l3_str2IntfId(char *devStr, sys_l3_intfId_t *pId);
extern int32 sys_l3_intfId2Name(sys_l3_intfId_t *pId, char *intfName);
extern int32 sys_l3_str2IntfName(char *devStr, char *intfName);
extern int32 sys_l3_intfName2Id(char *intfName, sys_l3_intfId_t *pId);
extern int32 sys_l3_intfName2Str(char *intfName, char *devStr);
extern int32 sys_l3_ip2Intf(sys_l3_af_t af, sys_l3_ip_t ip, sys_l3_intfId_t *pIntfId);
extern int32 sys_l3_ipInSubnet_ret(sys_l3_af_t af, sys_ip_t *pIp, sys_l3_ip_t *pSubnet);
extern int32 sys_l3_ip2Subnet_get(sys_l3_af_t af, sys_l3_ip_t *pIp, sys_l3_ip_t *pSubnet);
extern int32 sys_l3_ipv4Mask2Prefix_ret(sys_ipv4_t mask);
extern sys_ipv4_t sys_l3_ipv4Prefix2Mask_ret(uint32 prefix);
extern int32 sys_l3_ipv6Mask2Prefix_ret(sys_ipv6_t mask);
extern void sys_l3_ipv6Prefix2Mask_ret(uint32 prefix, sys_ipv6_t *mask);
extern uint32 sys_l3_isIPv6AllZero_ret(sys_ipv6_t ip);
extern int32 sys_l3_ipv6Eui64_convert(sys_mac_t *pMac, sys_ipv6_t *pIp);
extern uint32 sys_l3_ipv6Eui64Check_ret(sys_mac_t *pMac, sys_ipv6_t ip);
extern int32 sys_l3_ipv6SysEui64_convert(sys_ipv6_t *pIp);
extern uint32 sys_l3_ipv6SysEui64Check_ret(sys_ipv6_t ip);
extern int32 sys_l3_ipv6SysLinkLocal_get(sys_l3_ipv6_t *pIp);
extern int32 sys_l3_ipv6LinkLocalAddr_get(sys_l3_intfId_t *pIntfId, sys_l3_ipv6_t *pIp);

#endif  

