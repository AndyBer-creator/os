
#ifndef _INCLUDE_QUAGGATASK_H_
#define _INCLUDE_QUAGGATASK_H_

#include <libipc/vos.h>
#include <libipc/vosProcess.h>
#include <sys/time.h>

#define ZEBRA_ROUTE_SYSTEM               0
#define ZEBRA_ROUTE_KERNEL               1
#define ZEBRA_ROUTE_CONNECT              2
#define ZEBRA_ROUTE_STATIC               3
#define ZEBRA_ROUTE_RIP                  4
#define ZEBRA_ROUTE_RIPNG                5
#define ZEBRA_ROUTE_OSPF                 6
#define ZEBRA_ROUTE_OSPF6                7
#define ZEBRA_ROUTE_ISIS                 8
#define ZEBRA_ROUTE_BGP                  9
#define ZEBRA_ROUTE_HSLS                 10
#define ZEBRA_ROUTE_MAX                  11

#define DEFAULT_ROUTE		    ZEBRA_ROUTE_MAX

#define OSPF_AREA_DESC_STRING_MAXLEN  23
#define OSPF_OPTION_STR_MAXLEN		24

#define OSPF_AUTH_NULL                      0
#define OSPF_AUTH_SIMPLE                    1
#define OSPF_AUTH_CRYPTOGRAPHIC             2
#define OSPF_AUTH_NOTSET                   -1
#define OSPF_AUTH_CMD_NOTSEEN              -2

#define OSPF_AREA_ID_FORMAT_ADDRESS         1
#define OSPF_AREA_ID_FORMAT_DECIMAL         2

#define OSPF_NSSA_ROLE_NEVER     0
#define OSPF_NSSA_ROLE_CANDIDATE 1
#define OSPF_NSSA_ROLE_ALWAYS    2

#define OSPF_ABR_UNKNOWN	0
#define OSPF_ABR_STAND          1
#define OSPF_ABR_IBM            2
#define OSPF_ABR_CISCO          3
#define OSPF_ABR_SHORTCUT       4
#define OSPF_ABR_DEFAULT	OSPF_ABR_CISCO

#define OSPF_DEFAULT_REF_BANDWIDTH	100000  

#define OSPF_SPF_DELAY_DEFAULT              0
#define OSPF_SPF_HOLDTIME_DEFAULT           50
#define OSPF_SPF_MAX_HOLDTIME_DEFAULT	    5000

#define OSPF_LSA_REFRESH_INTERVAL_DEFAULT 10

#define DEFAULT_ORIGINATE_NONE		0
#define DEFAULT_ORIGINATE_ZEBRA		1
#define DEFAULT_ORIGINATE_ALWAYS	2

#define EXTERNAL_METRIC_TYPE_1      0
#define EXTERNAL_METRIC_TYPE_2      1

#define OSPF_IF_ACTIVE              0
#define OSPF_IF_PASSIVE		        1

#define OSPF_OUTPUT_COST_DEFAULT           10
#define OSPF_ROUTER_DEAD_INTERVAL_DEFAULT  40
#define OSPF_HELLO_INTERVAL_DEFAULT        10
#define OSPF_ROUTER_PRIORITY_DEFAULT        1
#define OSPF_RETRANSMIT_INTERVAL_DEFAULT    5
#define OSPF_TRANSMIT_DELAY_DEFAULT         1
#define OSPF_MTU_IGNORE_DEFAULT             0

#ifndef OSPF_AUTH_SIMPLE_SIZE
#define OSPF_AUTH_SIMPLE_SIZE     8U
#endif

#ifndef OSPF_AUTH_MD5_SIZE
#define OSPF_AUTH_MD5_SIZE       16U
#endif

#ifndef RMAP_MAX
#define RMAP_MAX                  4
#endif

#ifndef BGP_UPTIME_LEN
#define BGP_UPTIME_LEN            25
#endif

#ifndef FILTER_MAX
#define FILTER_MAX                2
#endif
#ifndef ETH_ALEN
#define ETH_ALEN                  6
#endif

#ifndef ISIS_SYS_ID_LEN
#define ISIS_SYS_ID_LEN           6
#endif

#ifndef MAX_AREA_ADDRESS_NUM
#define MAX_AREA_ADDRESS_NUM      3
#endif

#ifndef OSPF6_MULTI_PATH_LIMIT
#define OSPF6_MULTI_PATH_LIMIT    4
#endif

struct ospf_vllink_config_data {
  ulong_t area_id;
  int format;
  ulong_t vl_peer;
  int auth_type;
  char auth_key[OSPF_AUTH_SIMPLE_SIZE+1];
  int crypto_key_id;
  char md5_key[OSPF_AUTH_MD5_SIZE+1];
  int hello_interval;
  int retransmit_interval;
  int transmit_delay;
  int dead_interval;
};

#define NAME_SIZE  32

#define ZEBRA_PROCESS_NAME  GLN_PROC_DIR"zebra"
#define RIP_PROCESS_NAME  GLN_PROC_DIR"rip"
#define OSPF_PROCESS_NAME  GLN_PROC_DIR"ospf"
#define BGP_PROCESS_NAME  GLN_PROC_DIR"bgp"
#define ISIS_PROCESS_NAME  GLN_PROC_DIR"isis"
#define RIPNG_PROCESS_NAME  GLN_PROC_DIR"ripng"
#define OSPFV3_PROCESS_NAME GLN_PROC_DIR"ospfv3"

#define AREA_IP_FORMART_OID  0x10015
#define AREA_INT_FORMART_OID 0x10020

#define OSPF_TIME_DUMP_SIZE	16

#define OSPF_SOCK_PATH "ospf_ipc_"
#define RIP_SOCK_PATH "rip_ipc_"
#define BGP_SOCK_PATH "bgp_ipc_"
#define ZEBRA_SOCK_PATH "zebra_ipc_"
typedef enum{
    
    FRR_FUNC_BGP_OPEN, 
    FRR_FUNC_BGP_CLOSE,
    FRR_FUNC_BGP_NETWORK_SET,
    FRR_FUNC_BGP_NEIGHBOR_REMOTEAS_SET,
    FRR_FUNC_BGP_NEIGHBOR_GROUP_SET,
    FRR_FUNC_BGP_NEIGHBOR_LOCALAS_SET,
    FRR_FUNC_BGP_NEIGHBOR_PASSWD_SET,
    FRR_FUNC_BGP_NEIGHBOR_ACTIVATE_SET,
    FRR_FUNC_BGP_NEIGHBOR_GROUP_ADDR_SET,
    FRR_FUNC_BGP_NEIGHBOR_FLAG_SET,
    FRR_FUNC_BGP_NEIGHBOR_AF_FLAG_SET,
    FRR_FUNC_BGP_NEIGHBOR_DISTRIBUTELIST_SET,
    FRR_FUNC_BGP_NEIGHBOR_PREFIXLIST_SET,
    FRR_FUNC_BGP_NEIGHBOR_FILTERLIST_SET,
    FRR_FUNC_BGP_NEIGHBOR_ROUTEMAP_SET,
    FRR_FUNC_BGP_NEIGHBOR_ADV_INTERVAL_SET,
    FRR_FUNC_BGP_NEIGHBOR_UPDATE_SOURCE_SET,
    FRR_FUNC_BGP_TIMER_SET,
    FRR_FUNC_BGP_ROUTER_ID_SET,
    FRR_FUNC_BGP_CLUSTER_ID_SET,
    FRR_FUNC_BGP_LOCAL_PREF_SET,
    FRR_FUNC_BGP_FLAG_SET,
    FRR_FUNC_BGP_REDISTRIBUTE_SET,
    FRR_FUNC_BGP_PEER_INFO_GET,
    FRR_FUNC_BGP_NEIGHBOR_MULTIHOP_SET,
    FRR_FUNC_BGP_NEIGHBOR_TIMER_SET,
    FRR_FUNC_BGP_INFO_GET,
    FRR_FUNC_BGP_ROUTE_INFO_GET,
    FRR_FUNC_BGP_ROUTE_INDEX_INFO_GET,
    FRR_FUNC_BGP_ASPATH_LIST_SET,
    FRR_FUNC_BGP_ASPATH_LIST_GET,
    FRR_FUNC_BGP_ASPATH_FILTER_GET,
    FRR_FUNC_BGP_DEBUG_SET,
    FRR_FUNC_BGP_AGGREGATE_ADDRESS_SET,
    FRR_FUNC_BGP_DEBUG_INFO_GET,
    FRR_FUNC_BGP_COMMUNITY_SET,
    FRR_FUNC_BGP_EXTCOMMUNITY_SET,
    FRR_FUNC_BGP_COMMUNITY_LIST_INFO_GET,
    FRR_FUNC_BGP_COMMUNITY_ENTRY_INFO_GET,
    FRR_FUNC_BGP_CONFEDERATION_ID_SET,
    FRR_FUNC_BGP_CONFEDERATION_PEER_SET,
    FRR_FUNC_BGP_END
}FRR_BGP_FUN;

typedef enum{
    
    FRR_FUNC_OSPF_OPEN,
    FRR_FUNC_OSPF_CLOSE,
	FRR_FUNC_OSPF_AREA_SET,
    FRR_FUNC_OSPF_ROUTER_ID_SET,
    FRR_FUNC_OSPF_PASSIVE_INTF_DEFAULT_SET,
    FRR_FUNC_OSPF_PASSIVE_INTF_SET,
    FRR_FUNC_OSPF_AREA_NETWORK_SET,
    FRR_FUNC_OSPF_AREA_RANGE_SET,
    FRR_FUNC_OSPF_AREA_RANGE_SUB_SET,
    FRR_FUNC_OSPF_VIRTUAL_LINK_SET,
    FRR_FUNC_OSPF_AREA_SHORT_CUT_SET,
    FRR_FUNC_OSPF_STUB_AREA_SET,
    FRR_FUNC_OSPF_NSSA_AREA_SET,
    FRR_FUNC_OSPF_AREA_DEFAULT_COST_SET,
    FRR_FUNC_OSPF_AREA_EXPORT_LIST_SET,
    FRR_FUNC_OSPF_AREA_IMPORT_LIST_SET,
    FRR_FUNC_OSPF_AREA_FILTER_LIST_SET,
    FRR_FUNC_OSPF_AREA_AUTH_TYPE_SET,
    FRR_FUNC_OSPF_ABR_TYPE_SET,
    FRR_FUNC_OSPF_LOG_ADJACENCY_SET,
    FRR_FUNC_OSPF_RFC1583_SET,
    FRR_FUNC_OSPF_SPF_TIMER_SET,
    FRR_FUNC_OSPF_STATIC_NEIGHBOR_SET,
    FRR_FUNC_OSPF_REFRESH_TIMER_SET,
    FRR_FUNC_OSPF_AUTO_COST_SET,
    FRR_FUNC_IP_OSPF_AUTH_MODE_SET,
    FRR_FUNC_IP_OSPF_AUTH_SIMPLE_SET,
    FRR_FUNC_IP_OSPF_AUTH_MD5_SET,
    FRR_FUNC_IP_OSPF_PARAM_SET,
    FRR_FUNC_IP_OSPF_DEAD_INTERVAL_SET,
    FRR_FUNC_IP_OSPF_NETWORK_SET,
    FRR_FUNC_OSPF_REDISTRIBUTE_SET,
    FRR_FUNC_OSPF_DISTRI_LIST_OUT_SET,
    FRR_FUNC_OSPF_DFT_INFO_SET,
    FRR_FUNC_OSPF_DEFAULT_METRIC_SET,
    FRR_FUNC_OSPF_DISTANCE_SET,
    FRR_FUNC_OSPF_DISTANCE_OSPF_SET,
    FRR_FUNC_OSPF_DISTANCE_PERFIX_SET,
    FRR_FUNC_OSPF_MTU_IGNORE_SET,
    FRR_FUNC_IP_OSPF_IFP_INFO_GET,
    FRR_FUNC_IP_OSPF_INTF_INFO_GET,
    FRR_FUNC_IP_OSPF_VIR_INTF_INFO_GET,
    FRR_FUNC_IP_OSPF_NBR_INFO_GET,
    FRR_FUNC_IP_OSPF_NBMA_NBR_INFO_GET,
    FRR_FUNC_IP_OSPF_ROUTE_INFO_GET,
    FRR_FUNC_IP_OSPF_ROUTE_NUM_GET,
    FRR_FUNC_IP_OSPF_RTR_ROUTE_INFO_GET,
    FRR_FUNC_IP_OSPF_LSA_INFO_GET,
    FRR_FUNC_IP_OSPF_INFO_GET,
    FRR_FUNC_IP_OSPF_REDIST_INFO_GET,
    FRR_FUNC_IP_OSPF_STATUS_GET,
    FRR_FUNC_IP_OSPF_PROCEESS_GET,
    FRR_FUNC_IP_OSPF_AREA_INFO_GET,
    FRR_FUNC_IP_OSPF_AREA_NETWORK_GET,
    FRR_FUNC_IP_OSPF_AREA_RANGE_GET,
    FRR_FUNC_OSPF_PACKET_DEBUG_SET,
    FRR_FUNC_OSPF_COMMON_DEBUG_SET,
    FRR_FUNC_OSPF_DEBUG_INFO_GET,
    FRR_FUNC_OSPF_CAPABILITY_OPAQUE_SET,
    FRR_FUNC_OSPF_VRRP_NOTIFY,
    FRR_FUNC_IP_OSPF_BFD_SET,
    FRR_FUNC_OSPF_BFD_SEARCH_INTF,
    FRR_FUNC_OSPF_BFD_MSG,
    FRR_FUNC_OSPF_PASSIVE_INTF_GET,
    FRR_FUNC_OSPF_DISTANCE_GET,
    FRR_FUNC_OSPF_END
}FRR_OSPF_FUN;

typedef enum{
    FRR_FUNC_RIP_VRRP_NOTIFY,
    FRR_FUNC_RIP_STATUS_GET,
    FRR_FUNC_RIP_OPEN,
    FRR_FUNC_RIP_CLOSE,
    FRR_FUNC_RIP_VERSION_SET,
    FRR_FUNC_RIP_IP_RECEIVE_VERSION_SET,
    FRR_FUNC_RIP_IP_SEND_VERSION_SET,
    FRR_FUNC_RIP_DEFAULT_METRIC_SET,
    FRR_FUNC_RIP_DISTANCE_SET,
    FRR_FUNC_RIP_DISTANCE_SOURCE_SET,
    FRR_FUNC_RIP_SHOW,
    FRR_FUNC_RIP_STATUS_SHOW,
    FRR_FUNC_RIP_NETWORK_SET,
    FRR_FUNC_RIP_NEIGHBOR_SET,
    FRR_FUNC_RIP_AUTHENTICATION_SET,
    FRR_FUNC_RIP_SPLIT_HORIZON_SET,
    FRR_FUNC_RIP_SPLIT_HORIZON_POISONED_REVERSE_SET,
    FRR_FUNC_RIP_EVENT_DEBUG_SET,
    FRR_FUNC_RIP_PACKET_DEBUG_SET,
    FRR_FUNC_RIP_ZEBRA_DEBUG_SET,
    FRR_FUNC_RIP_PASSIVE_INTERFACE_SET,
    FRR_FUNC_RIP_TIMER_SET,
    FRR_FUNC_RIP_OFFSET_SET,
    FRR_FUNC_RIP_REDISTRIBUTE_SELF_SET,
    FRR_FUNC_RIP_REDISTRIBUTE_SET,
    FRR_FUNC_RIP_DEFAULT_ORIGINATE_SET,
    FRR_FUNC_RIP_STATIC_ROUTE_SET,
    FRR_FUNC_RIP_AGGREGATE_ADDRESS_SET,

    FRR_FUNC_RIP_ROUTE_GET,
    FRR_FUNC_RIP_ROUTE_NUM_GET,
    FRR_FUNC_RIP_INFO_GET,
    FRR_FUNC_RIP_REDIS_INFO_GET,
    FRR_FUNC_RIP_INTF_INFO_GET,
    FRR_FUNC_RIP_ENABLE_NW_INFO_GET,
    FRR_FUNC_RIP_ENABLE_INTF_INFO_GET,
    FRR_FUNC_RIP_NEIGHBOR_INFO_GET,
    FRR_FUNC_RIP_PASSIVE_INTF_INFO_GET,
    FRR_FUNC_RIP_PEER_INFO_GET,
    FRR_FUNC_RIP_DISTANCE_INFO_GET,
    FRR_FUNC_RIP_DEBUG_INFO_GET,
    FRR_FUNC_DISTRIBUTE_LIST_SET,
    FRR_FUNC_ACCESS_LIST_ZEBRA_SET,
    FRR_FUNC_RIP_DEBUG_SET,
    FRR_FUNC_RIP_LOG_TO_FILE_SET,
    FRR_FUNC_RIP_LOG_TO_STDOUT_SET,
    FRR_FUNC_RIP_VERSION_GET,
    FRR_FUNC_ACCESS_LIST_INFO_GET,
    FRR_FUNC_ACCESS_LIST_FILTER_INFO_GET,
    FRR_FUNC_DISTRIBUTE_INFO_GET,
    FRR_FUNC_RIP_END
}FRR_RIP_FUN;

typedef enum{
    
    FRR_FUNC_ZEBRA_ROUTERID = 0x0,
    FRR_FUNC_ZEBRA_ROUTERID_NO,
    FRR_FUNC_ZEBRA_ROUTERID_GET,
    FRR_FUNC_ZEBRA_IF_SHUTDOWN,
    FRR_FUNC_ZEBRA_IF_IP_ADD,
    FRR_FUNC_ZEBRA_IF_IP_DELETE,
    FRR_FUNC_ZEBRA_IP_ROUTE_ADD,
    FRR_FUNC_ZEBRA_IP_ROUTE_DELETE,
    FRR_FUNC_ZEBRA_IP_ROUTE_GET,
    FRR_FUNC_ZEBRA_IPV6_FORWARDING_SET,
    FRR_FUNC_ZEBRA_IPV6_ROUTE_GET,
    FRR_FUNC_ZEBRA_IPV6_ADDR_ADD,
    FRR_FUNC_ZEBRA_IPV6_ADDR_DEL,
    FRR_FUNC_ZEBRA_IPV6_ROUTE_ADD,
    FRR_FUNC_ZEBRA_IPV6_ROUTE_DEL,
    FRR_FUNC_ZEBRA_IPV6_LINKLOCAL_ADDR_GET,
    FRR_FUNC_ZEBRA_STATS_GET,
    FRR_FUNC_ZEBRA_STATS_LINKLOCAL_ADDR_COUNT,
    FRR_FUNC_ZEBRA_END
}FRR_ZEBRA_FUN;

typedef enum {
    
    QUAGGA_FUNC_ZEBRA_ROUTERID = 0x0,
    QUAGGA_FUNC_ZEBRA_ROUTERID_NO,
    QUAGGA_FUNC_ZEBRA_ROUTERID_GET,
    QUAGGA_FUNC_ZEBRA_IF_SHUTDOWN,
    QUAGGA_FUNC_ZEBRA_IF_IP_ADD,
    QUAGGA_FUNC_ZEBRA_IF_IP_DELETE,
    QUAGGA_FUNC_ZEBRA_IP_ROUTE_ADD,
    QUAGGA_FUNC_ZEBRA_IP_ROUTE_DELETE,
    QUAGGA_FUNC_ZEBRA_IP_ROUTE_GET,
    QUAGGA_FUNC_ZEBRA_IPV6_FORWARDING_SET,
    QUAGGA_FUNC_ZEBRA_IPV6_ROUTE_GET,
    QUAGGA_FUNC_ZEBRA_IPV6_ADDR_ADD,
    QUAGGA_FUNC_ZEBRA_IPV6_ADDR_DEL,
    QUAGGA_FUNC_ZEBRA_IPV6_ROUTE_ADD,
    QUAGGA_FUNC_ZEBRA_IPV6_ROUTE_DEL,
    QUAGGA_FUNC_ZEBRA_IPV6_LINKLOCAL_ADDR_GET,
    QUAGGA_FUNC_ZEBRA_STATS_GET,
    QUAGGA_FUNC_ZEBRA_STATS_LINKLOCAL_ADDR_COUNT,

    QUAGGA_FUNC_RIP_VRRP_NOTIFY,
    QUAGGA_FUNC_RIP_STATUS_GET,
    QUAGGA_FUNC_RIP_OPEN,
    QUAGGA_FUNC_RIP_CLOSE,
    QUAGGA_FUNC_RIP_VERSION_SET,
    QUAGGA_FUNC_RIP_IP_RECEIVE_VERSION_SET,
    QUAGGA_FUNC_RIP_IP_SEND_VERSION_SET,
    QUAGGA_FUNC_RIP_DEFAULT_METRIC_SET,
    QUAGGA_FUNC_RIP_DISTANCE_SET,
    QUAGGA_FUNC_RIP_DISTANCE_SOURCE_SET,
    QUAGGA_FUNC_RIP_SHOW,
    QUAGGA_FUNC_RIP_STATUS_SHOW,
    QUAGGA_FUNC_RIP_NETWORK_SET,
    QUAGGA_FUNC_RIP_NEIGHBOR_SET,
    QUAGGA_FUNC_RIP_AUTHENTICATION_SET,
    QUAGGA_FUNC_RIP_SPLIT_HORIZON_SET,
    QUAGGA_FUNC_RIP_SPLIT_HORIZON_POISONED_REVERSE_SET,
    QUAGGA_FUNC_RIP_EVENT_DEBUG_SET,
    QUAGGA_FUNC_RIP_PACKET_DEBUG_SET,
    QUAGGA_FUNC_RIP_ZEBRA_DEBUG_SET,
    QUAGGA_FUNC_RIP_PASSIVE_INTERFACE_SET,
    QUAGGA_FUNC_RIP_TIMER_SET,
    QUAGGA_FUNC_RIP_OFFSET_SET,
    QUAGGA_FUNC_RIP_REDISTRIBUTE_SELF_SET,
    QUAGGA_FUNC_RIP_REDISTRIBUTE_SET,
    QUAGGA_FUNC_RIP_DEFAULT_ORIGINATE_SET,
    QUAGGA_FUNC_RIP_STATIC_ROUTE_SET,
    QUAGGA_FUNC_RIP_AGGREGATE_ADDRESS_SET,

    QUAGGA_FUNC_RIP_ROUTE_GET,
    QUAGGA_FUNC_RIP_ROUTE_NUM_GET,
    QUAGGA_FUNC_RIP_INFO_GET,
    QUAGGA_FUNC_RIP_REDIS_INFO_GET,
    QUAGGA_FUNC_RIP_INTF_INFO_GET,
    QUAGGA_FUNC_RIP_ENABLE_NW_INFO_GET,
    QUAGGA_FUNC_RIP_ENABLE_INTF_INFO_GET,
    QUAGGA_FUNC_RIP_NEIGHBOR_INFO_GET,
    QUAGGA_FUNC_RIP_PASSIVE_INTF_INFO_GET,
    QUAGGA_FUNC_RIP_PEER_INFO_GET,
    QUAGGA_FUNC_RIP_DISTANCE_INFO_GET,
    QUAGGA_FUNC_RIP_DEBUG_INFO_GET,

    QUAGGA_FUNC_OSPF_OPEN,
    QUAGGA_FUNC_OSPF_CLOSE,
    QUAGGA_FUNC_OSPF_ROUTER_ID_SET,
    QUAGGA_FUNC_OSPF_PASSIVE_INTF_DEFAULT_SET,
    QUAGGA_FUNC_OSPF_PASSIVE_INTF_SET,
    QUAGGA_FUNC_OSPF_AREA_NETWORK_SET,
    QUAGGA_FUNC_OSPF_AREA_RANGE_SET,
    QUAGGA_FUNC_OSPF_AREA_RANGE_SUB_SET,
    QUAGGA_FUNC_OSPF_VIRTUAL_LINK_SET,
    QUAGGA_FUNC_OSPF_AREA_SHORT_CUT_SET,
    QUAGGA_FUNC_OSPF_STUB_AREA_SET,
    QUAGGA_FUNC_OSPF_NSSA_AREA_SET,
    QUAGGA_FUNC_OSPF_AREA_DEFAULT_COST_SET,
    QUAGGA_FUNC_OSPF_AREA_EXPORT_LIST_SET,
    QUAGGA_FUNC_OSPF_AREA_IMPORT_LIST_SET,
    QUAGGA_FUNC_OSPF_AREA_FILTER_LIST_SET,
    QUAGGA_FUNC_OSPF_AREA_AUTH_TYPE_SET,
    QUAGGA_FUNC_OSPF_ABR_TYPE_SET,
    QUAGGA_FUNC_OSPF_LOG_ADJACENCY_SET,
    QUAGGA_FUNC_OSPF_RFC1583_SET,
    QUAGGA_FUNC_OSPF_SPF_TIMER_SET,
    QUAGGA_FUNC_OSPF_STATIC_NEIGHBOR_SET,
    QUAGGA_FUNC_OSPF_REFRESH_TIMER_SET,
    QUAGGA_FUNC_OSPF_AUTO_COST_SET,
    QUAGGA_FUNC_IP_OSPF_AUTH_MODE_SET,
    QUAGGA_FUNC_IP_OSPF_AUTH_SIMPLE_SET,
    QUAGGA_FUNC_IP_OSPF_AUTH_MD5_SET,
    QUAGGA_FUNC_IP_OSPF_PARAM_SET,
    QUAGGA_FUNC_IP_OSPF_DEAD_INTERVAL_SET,
    QUAGGA_FUNC_IP_OSPF_NETWORK_SET,
    QUAGGA_FUNC_OSPF_REDISTRIBUTE_SET,
    QUAGGA_FUNC_OSPF_DISTRI_LIST_OUT_SET,
    QUAGGA_FUNC_OSPF_DFT_INFO_SET,
    QUAGGA_FUNC_OSPF_DEFAULT_METRIC_SET,
    QUAGGA_FUNC_OSPF_DISTANCE_SET,
    QUAGGA_FUNC_OSPF_DISTANCE_OSPF_SET,
    QUAGGA_FUNC_OSPF_DISTANCE_PERFIX_SET,
    QUAGGA_FUNC_OSPF_MTU_IGNORE_SET,
    QUAGGA_FUNC_IP_OSPF_IFP_INFO_GET,
    QUAGGA_FUNC_IP_OSPF_INTF_INFO_GET,
    QUAGGA_FUNC_IP_OSPF_VIR_INTF_INFO_GET,
    QUAGGA_FUNC_IP_OSPF_NBR_INFO_GET,
    QUAGGA_FUNC_IP_OSPF_NBMA_NBR_INFO_GET,
    QUAGGA_FUNC_IP_OSPF_ROUTE_INFO_GET,
    QUAGGA_FUNC_IP_OSPF_ROUTE_NUM_GET,
    QUAGGA_FUNC_IP_OSPF_RTR_ROUTE_INFO_GET,
    QUAGGA_FUNC_IP_OSPF_LSA_INFO_GET,
    QUAGGA_FUNC_IP_OSPF_INFO_GET,
    QUAGGA_FUNC_IP_OSPF_STATUS_GET,
    QUAGGA_FUNC_IP_OSPF_AREA_INFO_GET,
    QUAGGA_FUNC_OSPF_PACKET_DEBUG_SET,
    QUAGGA_FUNC_OSPF_COMMON_DEBUG_SET,
    QUAGGA_FUNC_OSPF_DEBUG_INFO_GET,
    QUAGGA_FUNC_OSPF_CAPABILITY_OPAQUE_SET,
    QUAGGA_FUNC_OSPF_VRRP_NOTIFY,
    QUAGGA_FUNC_IP_OSPF_BFD_SET,
    QUAGGA_FUNC_OSPF_BFD_SEARCH_INTF,
    QUAGGA_FUNC_OSPF_BFD_MSG,

    QUAGGA_FUNC_BGP_OPEN,
    QUAGGA_FUNC_BGP_CLOSE,
    QUAGGA_FUNC_BGP_NETWORK_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_REMOTEAS_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_GROUP_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_LOCALAS_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_PASSWD_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_ACTIVATE_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_GROUP_ADDR_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_FLAG_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_AF_FLAG_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_DISTRIBUTELIST_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_PREFIXLIST_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_FILTERLIST_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_ROUTEMAP_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_ADV_INTERVAL_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_UPDATE_SOURCE_SET,
    QUAGGA_FUNC_BGP_TIMER_SET,
    QUAGGA_FUNC_BGP_ROUTER_ID_SET,
    QUAGGA_FUNC_BGP_CLUSTER_ID_SET,
    QUAGGA_FUNC_BGP_LOCAL_PREF_SET,
    QUAGGA_FUNC_BGP_FLAG_SET,
    QUAGGA_FUNC_BGP_REDISTRIBUTE_SET,
    QUAGGA_FUNC_BGP_PEER_INFO_GET,
    QUAGGA_FUNC_BGP_NEIGHBOR_MULTIHOP_SET,
    QUAGGA_FUNC_BGP_NEIGHBOR_TIMER_SET,
    QUAGGA_FUNC_BGP_INFO_GET,
    QUAGGA_FUNC_BGP_ROUTE_INFO_GET,
    QUAGGA_FUNC_BGP_ROUTE_INDEX_INFO_GET,
    QUAGGA_FUNC_BGP_ASPATH_LIST_SET,
    QUAGGA_FUNC_BGP_ASPATH_LIST_GET,
    QUAGGA_FUNC_BGP_ASPATH_FILTER_GET,
    QUAGGA_FUNC_BGP_DEBUG_SET,
    QUAGGA_FUNC_BGP_AGGREGATE_ADDRESS_SET,
    QUAGGA_FUNC_BGP_DEBUG_INFO_GET,
    QUAGGA_FUNC_BGP_COMMUNITY_SET,
    QUAGGA_FUNC_BGP_EXTCOMMUNITY_SET,
    QUAGGA_FUNC_BGP_COMMUNITY_LIST_INFO_GET,
    QUAGGA_FUNC_BGP_COMMUNITY_ENTRY_INFO_GET,
    QUAGGA_FUNC_BGP_CONFEDERATION_ID_SET,
    QUAGGA_FUNC_BGP_CONFEDERATION_PEER_SET,

    QUAGGA_FUNC_ISIS_OPEN,
    QUAGGA_FUNC_ISIS_CLOSE,
    QUAGGA_FUNC_ISIS_NET_TITLE_SET,
    QUAGGA_FUNC_ISIS_LEVEL_SET,
    QUAGGA_FUNC_ISIS_LSP_GEN_INVL_SET,
    QUAGGA_FUNC_ISIS_LSP_LIFE_INVL_SET,
    QUAGGA_FUNC_ISIS_LSP_REFRESH_INVL_SET,
    QUAGGA_FUNC_ISIS_SPF_INVL_SET,
    QUAGGA_FUNC_ISIS_METRIC_STYTLE_SET,
    QUAGGA_FUNC_ISIS_OVERLOAD_BIT_SET,
    QUAGGA_FUNC_ISIS_LOG_ADJ_SET,
    QUAGGA_FUNC_ISIS_AREA_AUTH_SET,
    QUAGGA_FUNC_ISIS_DOMAIN_AUTH_SET,
    QUAGGA_FUNC_ISIS_DEBUG_SET,
    QUAGGA_FUNC_ISIS_STATUS_GET,
    QUAGGA_FUNC_ISIS_LSDB_INFO_GET,
    QUAGGA_FUNC_ISIS_CIRCUIT_INFO_GET,
    QUAGGA_FUNC_ISIS_ADJ_INFO_GET,
    QUAGGA_FUNC_ISIS_DEBUG_INFO_GET,
    QUAGGA_FUNC_ISIS_HOSTNAME_INFO_GET,
    QUAGGA_FUNC_ISIS_SUMMARY_INFO_GET,
    QUAGGA_FUNC_ISIS_DYN_HOSTNAME_INFO_SET,
    QUAGGA_FUNC_ISIS_ROUTE_INFO_GET,
    QUAGGA_FUNC_ISIS_ROUTE_NUM_GET,
    QUAGGA_FUNC_ISIS_CIRCUIT_SET,
    QUAGGA_FUNC_ISIS_CIRCUIT_PASSIVE_SET,
    QUAGGA_FUNC_ISIS_CIRCUIT_LEVEL_SET,
    QUAGGA_FUNC_ISIS_CIRCUIT_PASSWD_SET,
    QUAGGA_FUNC_ISIS_CIRCUIT_PARAM_SET,
    QUAGGA_FUNC_ISIS_CIRCUIT_HELLO_PAD_SET,
    QUAGGA_FUNC_ISIS_CIRCUIT_PTOP_SET,

    QUAGGA_FUNC_RIPNG_STATUS_GET,
    QUAGGA_FUNC_RIPNG_OPEN,
    QUAGGA_FUNC_RIPNG_CLOSE,
    QUAGGA_FUNC_RIPNG_DEFAULT_METRIC_SET,
    QUAGGA_FUNC_RIPNG_NETWORK_SET,
    QUAGGA_FUNC_RIPNG_SPLIT_HORIZON_SET,
    QUAGGA_FUNC_RIPNG_SPLIT_HORIZON_POISONED_REVERSE_SET,
    QUAGGA_FUNC_RIPNG_EVENT_DEBUG_SET,
    QUAGGA_FUNC_RIPNG_PACKET_DEBUG_SET,
    QUAGGA_FUNC_RIPNG_ZEBRA_DEBUG_SET,
    QUAGGA_FUNC_RIPNG_PASSIVE_INTERFACE_SET,
    QUAGGA_FUNC_RIPNG_TIMER_SET,
    QUAGGA_FUNC_RIPNG_OFFSET_SET,
    QUAGGA_FUNC_RIPNG_REDISTRIBUTE_SELF_SET,
    QUAGGA_FUNC_RIPNG_REDISTRIBUTE_SET,
    QUAGGA_FUNC_RIPNG_AGGREGATE_ADDR_SET,

    QUAGGA_FUNC_RIPNG_ROUTE_GET,
    QUAGGA_FUNC_RIPNG_ROUTE_NUM_GET,
    QUAGGA_FUNC_RIPNG_INFO_GET,
    QUAGGA_FUNC_RIPNG_REDIS_INFO_GET,
    QUAGGA_FUNC_RIPNG_PEER_INFO_GET,
    QUAGGA_FUNC_RIPNG_DEBUG_INFO_GET,
    QUAGGA_FUNC_RIPNG_INTF_INFO_GET,

    QUAGGA_FUNC_OSPF6_STATUS_GET,
    QUAGGA_FUNC_OSPF6_OPEN,
    QUAGGA_FUNC_OSPF6_CLOSE,
    QUAGGA_FUNC_OSPF6_AREA_RANGE_SET,
    QUAGGA_FUNC_OSPF6_AREA_EXPORT_LIST_SET,
    QUAGGA_FUNC_OSPF6_AREA_IMPORT_LIST_SET,
    QUAGGA_FUNC_OSPF6_AREA_FILTER_LIST_SET,
    QUAGGA_FUNC_IP_OSPF6_PARAM_SET,
    QUAGGA_FUNC_OSPF6_INTF_SET,
    QUAGGA_FUNC_OSPF6_DEBUG_SET,
    QUAGGA_FUNC_OSPF6_ROUTER_ID_SET,
    QUAGGA_FUNC_OSPF6_REDISTRIBUTE_SET,
    QUAGGA_FUNC_OSPF6_NBR_INFO_GET,
    QUAGGA_FUNC_OSPF6_ROUTE_INFO_GET,
    QUAGGA_FUNC_OSPF6_ROUTE_NUM_GET,
    QUAGGA_FUNC_OSPF6_DEBUG_INFO_GET,
    QUAGGA_FUNC_OSPF6_BORDER_ROUTE_INFO_GET,
    QUAGGA_FUNC_OSPF6_LSA_INFO_GET,
    QUAGGA_FUNC_OSPF6_AREA_INFO_GET,
    QUAGGA_FUNC_OSPF6_AREA_IF_INFO_GET,
    QUAGGA_FUNC_OSPF6_LINK_STATE_INFO_GET,
    QUAGGA_FUNC_OSPF6_INFO_GET,
    QUAGGA_FUNC_OSPF6_INTF_INFO_GET,

    QUAGGA_FUNC_RMAP_SET,
    QUAGGA_FUNC_RMAP_ONMATCH_NEXT_SET,
    QUAGGA_FUNC_RMAP_ONMATCH_GOTO_SET,
    QUAGGA_FUNC_RMAP_CALL_SET,
    QUAGGA_FUNC_RMAP_MATCH_SET,
    QUAGGA_FUNC_RMAP_SET_ACTION_SET,
    QUAGGA_FUNC_RMAP_GET_NEXT_RMAP_INFO,
    QUAGGA_FUNC_RMAP_GET_RMAP_INDEX_INFO,
    QUAGGA_FUNC_RMAP_GET_NEXT_RMAP_RULE_INFO,
    
    QUAGGA_FUNC_KEY_CHAIN_SET,
    QUAGGA_FUNC_KEY_SET,
    QUAGGA_FUNC_KEY_STRING_SET,
    QUAGGA_FUNC_KEY_LIFE_TIME_SET,
    QUAGGA_FUNC_KEY_LIFE_TIME_INFINITE_SET,
    QUAGGA_FUNC_KEY_LIFE_TIME_DURATION_SET,

    QUAGGA_FUNC_IF_RMAP_SET,

    QUAGGA_FUNC_DISTRIBUTE_LIST_SET,
    QUAGGA_FUNC_DISTRIBUTE_LIST_PREFIX_SET,
    QUAGGA_FUNC_DISTRIBUTE_INFO_GET,

    QUAGGA_FUNC_PREFIX_LIST_SET,
    QUAGGA_FUNC_PREFIX_LIST_CLEAR,
    QUAGGA_FUNC_PREFIX_LIST_SEQ_NUMBER_SET,
    QUAGGA_FUNC_PREFIX_LIST_MASTER_INFO_GET,
    QUAGGA_FUNC_PREFIX_LIST_INFO_GET,
    QUAGGA_FUNC_PREFIX_LIST_ENTRY_INFO_GET,
    QUAGGA_FUNC_PREFIX_LIST_ENTRY_PREFIX_INFO_GET,

    QUAGGA_FUNC_ACCESS_LIST_CISCO_SET,
    QUAGGA_FUNC_ACCESS_LIST_ZEBRA_SET,
    QUAGGA_FUNC_ACCESS_LIST_INFO_GET,
    QUAGGA_FUNC_ACCESS_LIST_FILTER_INFO_GET,
    QUAGGA_FUNC_ACCESS_LIST_DEL_SET,

    QUAGGA_FUNC_MAX
}QUAGGA_FUNC_ENT;

#ifdef GLN_INCLUDE_ROUTE_ECMP
#define ZEBRA_NEXTHOP_NUM GLN_ROUTE_ECMP_MAX_NUM
#else
#define ZEBRA_NEXTHOP_NUM 8
#endif

typedef enum {
    ZEBRA_NEXTHOP_FLAGS_NOT_INSTALL = 0x00000001
}ZEBRA_NEXTHOP_FLAGS_ENT;

typedef struct {
	ulong_t      type;
	ulong_t      select;
	ulong_t      fib;
	ulong_t      active;
    ulong_t      ulDestIp;
    ulong_t      ulpen;
    ulong_t      ulNexthop[ZEBRA_NEXTHOP_NUM];
    ulong_t      ulIfIndex[ZEBRA_NEXTHOP_NUM];
    
    ulong_t      ulDistance;
    ulong_t      ulMetric;
    char         devStr[24];
    
}ZEBRA_IP_ROUTE_STRC;

typedef struct {
	ulong_t 	 type;
	ulong_t      select;
	ulong_t      fib;
	ulong_t      active;
    uchar_t      ucDestIp[16];
    ulong_t      ulPrefixLen;
    ulong_t      ulNexthopNum;
    uchar_t      ucNexthop[ZEBRA_NEXTHOP_NUM][16];
    ulong_t      ulDistance;
    ulong_t      ulMetric;
    char         devStr[24];
}ZEBRA_IPV6_ROUTE_STRC;

typedef enum {
    ZEBRA_ROUTE_FLAG_REJECT = 0x00000001, 
    ZEBRA_ROUTE_FLAG_BLACKHOLE = 0x00000002 
}ZEBRA_ROUTE_FLAG_ENT;

typedef struct {
    ulong_t      ulDestIp;
    ulong_t      prefixlen;
    ulong_t      ulNexthop[ZEBRA_NEXTHOP_NUM];
    ulong_t      ulNexthopCnt; 
    ulong_t      ulRouteTotal;
    char         cType;
    int          sub_type;
    char         szProto[32];
    ulong_t      from;
    int          metric;
    int          external_metric;
    ushort_t     tag;
    char         szTime[32];
}RIP_IP_ROUTE_STRC;

typedef struct RIP_INFO_S
{
    ulong_t update_time;
    ulong_t remain_time;
    ulong_t timeout_time;
    ulong_t garbage_time;
    int default_metric;
    int distance;
    char version_send[32];
    char version_recv[32];
}RIP_INFO_T;

typedef struct RIP_REDIS_INFO_S
{
    int proto;
    char szName[32];
}RIP_REDIS_INFO_T;

typedef struct RIP_INTF_INFO_S
{
    char ifname[20];

    int ri_send;
    int ri_receive;

    int auth_type;

    char auth_str[20];

    int split;

    int passive;

}RIP_INTF_INFO_T;

typedef struct RIP_ENABLE_NW_INFO_S
{
    ulong_t ulDestIp;
    ulong_t ulDestMask;
}RIP_ENABLE_NW_INFO_T;

typedef struct RIP_ENABLE_INTF_INFO_S
{
    char ifname[32];
}RIP_ENABLE_INTF_INFO_T;

typedef struct RIP_NEIGHBOR_INFO_S
{
    ulong_t ulDestIp;
    ulong_t ulDestMask;
}RIP_NEIGHBOR_INFO_T;

typedef struct RIP_PASSIVE_INTF_INFO_S
{
    char ifname[32];
}RIP_PASSIVE_INTF_INFO_T;

typedef struct RIP_PEER_INFO_S
{
    ulong_t ulDestIp;
    int recv_badpackets;
    int recv_badroutes;
    int distance_default;
    char szUpTime[32];
}RIP_PEER_INFO_T;

typedef struct RIP_DISTANCE_INFO_S
{
    ulong_t ulDestIp;
    ulong_t ulDestMask;
    int distance;
    char szAcessListName[32];
}RIP_DISTANCE_INFO_T;

typedef struct RIP_DEBUG_INFO_S
{
    int isRipDebugEvent;
    int isRipDebugPacket;
    int isRipDebugSend;
    int isRipDebugRecv;
    int isRipDebugZebra;
}RIP_DEBUG_INFO_T;

enum
{
    OSPF_CONFIG_COST,
    OSPF_CONFIG_PRIORITY,
    OSPF_CONFIG_HELLO,
    OSPF_CONFIG_TRANSMIT,
    OSPF_CONFIG_RETRANSMIT
};

enum
{
    OSPF6_CONFIG_COST,
    OSPF6_CONFIG_PRIORITY,
    OSPF6_CONFIG_HELLO,
    OSPF6_CONFIG_DEAD,
    OSPF6_CONFIG_TRANSMIT,
    OSPF6_CONFIG_RETRANSMIT,
    OSPF6_CONFIG_PASSIVE,
    OSPF6_CONFIG_MTU_IGNORE
};

typedef struct OSPF_AREA_INFO_S
{
    BOOL bFirst;
    ushort_t instance;
    ulong_t area_id;
    int external_routing;
    BOOL bNoSummary;
    int shortcut_configured;
    int shortcut_capability;
    ulong_t  if_cnt;
    ulong_t act_ints;
    ulong_t full_nbrs;
    ulong_t full_vls;
    BOOL bABR;
    uchar_t NSSATranslatorState;
    uchar_t NSSATranslatorRole;
	ulong_t default_cost;
    int auth_type;
    BOOL bBackBone;
    ulong_t spf_calculation;
    ulong_t lsa_cnt;
    ulong_t rtr_lsa_cnt;
    uint_t rtr_lsa_chksum;
    ulong_t network_lsa_cnt;
    uint_t network_lsa_chksum;
    ulong_t summary_lsa_cnt;
    uint_t summary_lsa_chksum;
    ulong_t asbr_lsa_cnt;
    uint_t asbr_lsa_chksum;
    ulong_t nssa_lsa_cnt;
    uint_t nssa_lsa_chksum;
    ulong_t link_lsa_cnt;
    uint_t link_lsa_chksum;
    ulong_t area_lsa_cnt;
    uint_t area_lsa_chksum;
	char import_name[32];
	char export_name[32];
	char filter_in_name[32];
	char filter_out_name[32];
}OSPF_AREA_INFO_T;

#ifndef OSPF_TIME_DUMP_SIZE
#define OSPF_TIME_DUMP_SIZE 16
#endif
typedef struct OSPF_REDIST_S
{
    BOOL bFlag;
	int type;
	int value;
	char name[32];
	char distribute[32];
}OSPF_REDIST_T;

typedef struct OSPF_INFO_S
{
	ushort_t instance;
    ulong_t router_id;
	ulong_t router_id_static;
    char remain_time[OSPF_TIME_DUMP_SIZE];
    BOOL bRfc1583Compatible;
    BOOL bOpaqueCapable;
    BOOL bOrigBlocked;
    ulong_t opa_as_cnt;
    uint_t opa_as_chksum;
    unsigned int spf_delay;
    unsigned int spf_holdtime;
    unsigned int spf_max_holdtime;
    unsigned int spf_hold_multiplier;
    struct timeval ts_spf;
    char last_spf_time[OSPF_TIME_DUMP_SIZE];
    BOOL bSpfCalc;
    char spf_calc_time[OSPF_TIME_DUMP_SIZE];
    ulong_t min_ls_interval;
    ulong_t min_ls_arrival;
    int write_oi_count;
    ushort_t lsa_refresh_interval;
    uchar_t abr_type;
    BOOL bABR;
    BOOL bASBR;
    ulong_t ase_lsa_cnt;
    uint_t ase_lsa_chksum;
    ulong_t area_cnt;
    BOOL bLogAdj;
    BOOL bLogAdjDetail;
	ulong_t ref_bandwidth;
	ulong_t default_metric;
	uchar_t distance_all;
	uchar_t distance_intra;
	uchar_t distance_inter;
	uchar_t distance_external;
	uchar_t passive_interface_default;
}OSPF_INFO_T;

typedef struct OSPF_REDIST_INFO_S
{
	ushort_t instance;
	int default_originate;
	OSPF_REDIST_T redist[ZEBRA_ROUTE_MAX + 1];
}OSPF_REDIST_INFO_T;

typedef struct OSPF_AREA_NETWORK_S
{
	ushort_t instance;
	ulong_t areaId;
    ulong_t networkIp;
    ulong_t prefixlen;
}OSPF_AREA_NETWORK_T;

typedef struct OSPF_AREA_RANGE_S
{
	ushort_t instance;
	ulong_t areaId;
    ulong_t rangeIp;
    ulong_t prefixlen;
	ulong_t cost;
	ulong_t flags;
	ulong_t subst_addr;
	ulong_t subst_masklen;
}OSPF_AREA_RANGE_T;

typedef struct OSPF_INTF_INFO_S
{
    BOOL bFirst;
	ushort_t instance;
    char ifname[20];
    ulong_t prefix;
    ulong_t areaId;
    ulong_t dr;
    ulong_t bdr;
    int type;
    int priority;
    ulong_t output_cost;
    BOOL bPassive;
	uchar_t mtu_ignore;

    int hello_time;
    int dead_time;
    int transmit_delay;
    int retransmit_interval;

    int auth_type;
    int key_id;
    char auth_key[20];
    char szState[20];
}OSPF_INTF_INFO_T;

typedef struct OSPF_NBR_DETAIL_INFO_S
{
    uchar_t state;
    ulong_t src;
    ulong_t router_id;
    ulong_t address;
    char szAreaDesc[OSPF_AREA_DESC_STRING_MAXLEN];
    char ifname[32];
    int priority;
    char nsm_state[20];
    ulong_t state_change;
    struct timeval ts_last_progress;
    char last_progress_time[OSPF_TIME_DUMP_SIZE];
    struct timeval ts_last_regress;
    char last_regress_time[OSPF_TIME_DUMP_SIZE];
    char regress_str[32];
    ulong_t d_router;
    ulong_t bd_router;
    uchar_t options;
    char options_dump[OSPF_OPTION_STR_MAXLEN];
    char dead_time_dump[OSPF_TIME_DUMP_SIZE];
    ulong_t db_summary_cnt;
    ulong_t ls_request_cnt;
    ulong_t ls_retransmit_cnt;
    BOOL bThreadInactivity;
    BOOL bThreadDbDesc;
    BOOL bThreadLsReq;
    BOOL bThreadLsUpd;

}OSPF_NBR_DETAIL_INFO_T;

typedef struct OSPF_NBMA_NBR_DETAIL_INFO_S
{
    ulong_t addr;
    char szAreaDesc[OSPF_AREA_DESC_STRING_MAXLEN];
    char ifname[32];
    int priority;
    char nsm_state[20];
    ulong_t state_change;
    int v_poll;
    char poll_time_dump[OSPF_TIME_DUMP_SIZE];
    BOOL bThreadPoll;

}OSPF_NBMA_NBR_DETAIL_INFO_T;

typedef struct OSPF_NBMA_NBR_INFO_S
{
    ulong_t addr;
    int priority;
    char ifname[20];
}OSPF_NBMA_NBR_INFO_T;

typedef struct OSPF_NBR_INFO_S
{
	ushort_t instance;
    ulong_t src;
    ulong_t router_id;
    uchar_t state;
    int priority;
    ulong_t ls_retransmit_cnt;
    ulong_t ls_request_cnt;
    ulong_t db_summary_cnt;
    char ifname[20];
    char msgbuf[16];
    char timebuf[OSPF_TIME_DUMP_SIZE];
    uchar_t type;
    ulong_t areaId;
}OSPF_NBR_INFO_T;

struct LSA_HEADER_INFO
{
  ushort_t ls_age;
  uchar_t options;
  uchar_t type;
  ulong_t id;
  ulong_t adv_router;
  ulong_t ls_seqnum;
  ushort_t checksum;
  ushort_t length;
};

struct ROUTER_LSA_INFO
{
    struct LSA_HEADER_INFO header;
    int links;
};

struct SUMMARY_LSA_INFO
{
    struct LSA_HEADER_INFO header;
    char prefix_str[32];
};

struct AS_EXTERNAL_LSA_INFO
{
    struct LSA_HEADER_INFO header;
    BOOL bExternalMetric;
    char prefix_str[32];
    ulong_t route_tag;
};

struct COMMON_LSA_INFO
{
    struct LSA_HEADER_INFO header;
};

typedef struct OSPF_LSA_INFO_S
{
    BOOL bFirst;
    BOOL bAsExternal;
    ulong_t area_id;
    union
    {
        struct ROUTER_LSA_INFO router;
        struct SUMMARY_LSA_INFO summary;
        struct AS_EXTERNAL_LSA_INFO external;
        struct COMMON_LSA_INFO common;
    }u;
}OSPF_LSA_INFO_T;

typedef struct OSPF_ROUTE_PATH_INFO_S
{
    ulong_t nexthop;
    ulong_t adv_router;
    char ifname[32];
}OSPF_ROUTE_PATH_INFO_T;

#define MAX_OSPF_ROUTE_PATH_NUM    5
#define OSPF_ROUTE_TYPE_NETWORK    1
#define OSPF_ROUTE_TYPE_EXTERNAL   2
#define OSPF_ROUTE_TYPE_ROUTER     3

typedef struct OSPF_ROUTE_INFO_S
{
	ushort_t process;
    ulong_t DsIPv4;
    uchar_t prefixlen;
    uchar_t route_type; 
    uchar_t path_type;
    uchar_t type;
    ulong_t cost;
    ulong_t area_id;
    ulong_t tag;
    ulong_t type2_cost;
    ulong_t path_num;
    OSPF_ROUTE_PATH_INFO_T path[MAX_OSPF_ROUTE_PATH_NUM];
}OSPF_ROUTE_INFO_T;

typedef struct OSPF_RTR_ROUTE_INFO_S
{
	ushort_t process;
    ulong_t DsIPv4;
    uchar_t prefixlen;
    uchar_t route_index;
    uchar_t path_type;
    uchar_t flags;
    ulong_t cost;
    ulong_t area_id;
    ulong_t path_num;    
    OSPF_ROUTE_PATH_INFO_T path[MAX_OSPF_ROUTE_PATH_NUM];
}OSPF_RTR_ROUTE_INFO_T;

typedef struct OSPF_DEBUG_INFO_S
{
    ulong_t event;
    ulong_t ism;
    ulong_t nsm;
    ulong_t lsa;
    ulong_t zebra;
    ulong_t nssa;
    ulong_t packet[5];
}OSPF_DEBUG_INFO_T;
typedef struct OSPF_PASSIVE_INTF_INFO_S
{
	BOOL bFirst;
	ushort_t instance;
	BOOL flags;
	char name[20];
    ulong_t addr;
}OSPF_PASSIVE_INTF_INFO_T;

typedef struct OSPF_DISTANCE_INFO_S
{
	BOOL bFirst;
	ushort_t instance;
	BOOL flags;
	uchar_t distance;
    ulong_t addr;
    ulong_t prefixlen;
	char name[32];
}OSPF_DISTANCE_INFO_T;

typedef struct ALIST_INFO_S
{
    char alistName[NAME_SIZE+1];
}ALIST_INFO_T;

typedef struct FILTER_ZEBRA_INFO_S
{
    
    int exact;
    
    unsigned char family;
    unsigned char prefixlen;
    ulong_t prefix4;

}FILTER_ZEBRA_INFO_T;

typedef struct FILTER_INFO_S
{
    BOOL bfirst; 
    char alistName[NAME_SIZE+1];
    int type;

    int cisco;
    
    union
    {
       
        FILTER_ZEBRA_INFO_T zfilter;
    } u;

}FILTER_INFO_T;

enum 
{
  TYPE_IN, 
  TYPE_OUT,
  TYPE_MAX
};
typedef struct DISTRIBUTE_INFO_S
{
    int index;                     
    char ifname[32];
    char list[TYPE_MAX][NAME_SIZE+1];  
    char prefix[TYPE_MAX][NAME_SIZE+1]; 
}DISTRIBUTE_INFO_T;

#define VRRP_IF_TRACK_MAX     8
#define VRRP_VIP_NUMS_MAX     8
#define VRRP_VSRV_SIZE_MAX    5
#define VRRP_TIMER_HZ         1

typedef struct {    
    char        hwaddr[6];    
    uint_t      ipaddrM;      
    uint_t      ipaddrP;      
    int         ifindex;      
    char        ifname[32 + 1];       
    int         ifpindex;
    int         ifstate;	
	ulong_t     sockJoinState;
#if 0
    int         auth_type;    
    uchar_t     auth_data[8]; /
    #endif
} vrrp_if;

typedef struct {
    uint_t      addr;         
    #if 0
    int         deletable;    
    #endif
} vip_addr;

typedef struct {        
    #if 0                
    double    skew_time;         
    int    mast_down_int;    
    int    wantstate;  
    int    sockfd;         
    int    initF;          
    #endif
    int    vrid;         
    int    priority;     
    int    oldpriority;  
    int    nowner;       
    int    naddr;        
    vip_addr vaddr[8];     
    int    adver_int;    
    int    preempt;    
    int    delay;      
    int    state;      
    uint_t      ms_down_timer;
    uint_t      adver_timer;
    vrrp_if     vif;
    char        vhwaddr[6]; 
    int         no;         
    int         msgDone;    
    int         used;       
    int         adminState; 
    int         rowStatus;  
    ulong_t     upTime;     
    int         niftrack;   
    int         iftrack[VRRP_IF_TRACK_MAX]; 
    int         pritrack[VRRP_IF_TRACK_MAX]; 
    int         trackState[VRRP_IF_TRACK_MAX]; 

    ulong_t     staBecomeMaster;     
    ulong_t     staAdverRcvd;        
    ulong_t     staAdverIntErrors;   
    ulong_t     staAuthFailures;     
    ulong_t     staIpTtlErrors;      
    ulong_t     staPriZeroPktsRcvd;  
    ulong_t     staPriZeroPktsSent;  
    ulong_t     staInvTypePktsRcvd;  
    ulong_t     staAddrListErrors;   
    ulong_t     staInvAuthType;      
    ulong_t     staAuthTypeMismatch; 
    ulong_t     staPktsLenErrors;    
} vrrp_rt;

#if 0

typedef struct BGP_INFO_S
{
    char vpnName[NAME_SIZE+1];
    ushort_t afi;
    uchar_t safi;
    struct in_addr router_id;
    ulong_t as;
#if 0    
    ulong_t ribCnt;
    ulong_t peerCnt;
    ulong_t rsClientCnt;
    ulong_t groupCnt;
    BOOL bDampening;
#endif
    
}BGP_INFO_T;

typedef struct BGP_FILTER_INFO_S
{
  
  struct 
  {
      char name[NAME_SIZE+1];
  } dlist[FILTER_MAX];

  struct
  {
      char name[NAME_SIZE+1];
  } plist[FILTER_MAX];

  struct
  {
      char name[NAME_SIZE+1];
  } aslist[FILTER_MAX];

  struct
  {
      char name[NAME_SIZE+1];
  } map[RMAP_MAX];

  struct
  {
      char name[NAME_SIZE+1];
  } usmap;
}BGP_FILTER_INFO_T;

typedef struct BGP_NEXTHOP_INFO_S
{
  struct in_addr v4;
  struct in6_addr v6_global;
  struct in6_addr v6_local;
}BGP_NEXTHOP_INFO_T;

union sockunionEx 
{
  struct sockaddr sa;
  struct sockaddr_in sin;
  struct sockaddr_in6 sin6;
};

typedef struct BGP_PEER_INFO_S
{
    char vpnName[NAME_SIZE+1];
    ushort_t afi;
    uchar_t safi;
    struct in_addr hostIp;
    ulong_t as;
    ulong_t msgInputCnt;
    ulong_t msgOutputCnt;
    ulong_t obufCnt;
    char upTimeStr[BGP_UPTIME_LEN];
    int status;
    ulong_t pCnt;
    ulong_t flags;
    ushort_t sflags;
    char statusStr[32];
    
    ulong_t local_as;
    ulong_t change_local_as;
    char groupName[NAME_SIZE+1];
    struct in_addr remote_id;
    ulong_t config;
    int peers_check;
    char readtimeStr[BGP_UPTIME_LEN];
    ulong_t v_holdtime;
    ulong_t v_keepalive;
    ulong_t holdtime;
    ulong_t keepalive;    
    ushort_t cap;
    uchar_t afc[AFI_MAX][SAFI_MAX];
    uchar_t afc_nego[AFI_MAX][SAFI_MAX];
    uchar_t afc_adv[AFI_MAX][SAFI_MAX];
    uchar_t afc_recv[AFI_MAX][SAFI_MAX];
    ushort_t af_cap[AFI_MAX][SAFI_MAX];
    uchar_t af_group[AFI_MAX][SAFI_MAX];
    
    ushort_t af_sflags[AFI_MAX][SAFI_MAX];
    ulong_t af_flags[AFI_MAX][SAFI_MAX];
    ulong_t pcount[AFI_MAX][SAFI_MAX];
    
    ulong_t pmax[AFI_MAX][SAFI_MAX];
    uchar_t pmax_threshold[AFI_MAX][SAFI_MAX];
    ushort_t pmax_restart[AFI_MAX][SAFI_MAX];

#if 0    
    struct 
    {
        char name[NAME_SIZE+1];
    }default_rmap[AFI_MAX][SAFI_MAX];
    BGP_FILTER_INFO_T filter[AFI_MAX][SAFI_MAX];
#endif

    ulong_t v_gr_restart;
    BOOL bThreadGrRestart;
    BOOL bThreadGrStale;
    ulong_t restartRemain;
    ulong_t staleRemain;

    ulong_t open_in;		
    ulong_t open_out;		
    ulong_t update_in;		
    ulong_t update_out;		
    ulong_t keepalive_in;	
    ulong_t keepalive_out;	
    ulong_t notify_in;		
    ulong_t notify_out;		
    ulong_t refresh_in;		
    ulong_t refresh_out;	
    ulong_t dynamic_cap_in;	
    ulong_t dynamic_cap_out;	

    ulong_t established;	
    ulong_t dropped;		

    ulong_t v_routeadv;

    char updateSrcStr[16];
    ulong_t weight;
    char resettimeStr[BGP_UPTIME_LEN];
    char downStr[48];
    BOOL bThreadPmaxRestart;
    ulong_t pmaxRestartRemain;
    uchar_t sort;
    int ttl;			
    int gtsm_hops;		
    union sockunionEx su_local;
    union sockunionEx su_remote;
    BGP_NEXTHOP_INFO_T nexthop;
    int shared_network;
    BOOL bThreadStart;
    ulong_t startRemain;
    BOOL bThreadConnect;
    ulong_t connectRemain;
    BOOL bThreadRead;
    BOOL bThreadWrite; 
        
}BGP_PEER_INFO_T;

typedef struct BGP_ROUTE_INFO_S
{
    char vpnName[NAME_SIZE+1];
    ushort_t afi;
    uchar_t safi;
    struct in_addr prefix;
    ulong_t prefixlen;
    int type;
    char status_flag[5];
    struct in_addr mp_nexthop_global_in;
    struct in_addr nexthop;
    struct in6_addr mp_nexthop_global;
    BOOL bAttr;
    ulong_t flag;
    ulong_t med;
    ulong_t local_pref;
    ulong_t weight;
    char asPathStr[32];
    char origin[2];
    ulong_t ulRouteTotal;
    struct in_addr peer_addr;
}BGP_ROUTE_INFO_T;

typedef struct BGP_AS_LIST_INFO_S
{
    char aslistName[NAME_SIZE+1];
}BGP_AS_LIST_INFO_T;

typedef struct BGP_AS_LIST_FILTER_INFO_S
{
    BOOL bfirst;
    char aslistName[NAME_SIZE+1];
    int filter_type;
    char reg_str[64];
}BGP_AS_LIST_FILTER_INFO_T;

typedef struct BGP_DEBUG_INFO_S
{
    ulong_t as4;
    ulong_t fsm;
    ulong_t events;
    ulong_t filter;
    ulong_t keepalive;
    ulong_t update;
    ulong_t zebra;
    ulong_t normal;
}BGP_DEBUG_INFO_T;

typedef struct BGP_COMMUNITY_LIST_INFO_S
{
    uchar_t community_type;
	char szName[NAME_SIZE+1];

}BGP_COMMUNITY_LIST_INFO_T;

typedef struct BGP_COMMUNITY_ENTRY_INFO_S
{
    BOOL bFirst;

    char szName[NAME_SIZE+1];

    uchar_t community_type;

    uchar_t direct;

    uchar_t style;

    uchar_t any;
    
	char szConfig[100];

}BGP_COMMUNITY_ENTRY_INFO_T;

typedef struct ISIS_CIRCUIT_INFO_S
{
    char ifname[32];
    int state;
    int is_passive;
    uchar_t circuit_id;
    int circ_type;
    int is_type;
    u_char snpa[ETH_ALEN];
    char newmetric;
    ulong_t te_metric[2];
    ulong_t metric_default[2];
    ushort_t upadjcount[2];
    ulong_t hello_interval[2];	
    ushort_t hello_multiplier[2];	
    ushort_t csnp_interval[2];	
    ushort_t psnp_interval[2];
    int pad_hellos;
    uchar_t priority[2];
    char is_dr[2];
}ISIS_CIRCUIT_INFO_T;

typedef struct ISIS_ADJ_INFO_S
{
    u_char sysid[ISIS_SYS_ID_LEN];
    u_char hostname[256];
    BOOL bFirst;
    char ifname[32];
    int level;
    int adj_state;
    ulong_t last_upd;
    ushort hold_time;
    u_char snpa[ETH_ALEN];
}ISIS_ADJ_INFO_T;

typedef struct ISIS_DEBUG_INFO_S
{
    int packet;
    int spf;
    char zebra;
    int event;
}ISIS_DEBUG_INFO_T;

struct net_area_info
{
    u_char addr[20];
    u_char addr_len;
};

struct spf_info
{
    int pending;
    unsigned int runcount;        
    time_t last_run_timestamp;    
    time_t last_run_duration;     
};

typedef struct ISIS_SUMMARY_INFO_S
{
    int sysid_set;
    u_char sysid[ISIS_SYS_ID_LEN];
    time_t uptime;
    int areaCnt;
    struct net_area_info area_addr[MAX_AREA_ADDRESS_NUM];
    int is_type;
    char szMetricStyle[15];
    ushort_t min_spf_interval[2];
    struct spf_info spf[2];
    int lspGenInvl[2];
    int lspRefreInvl[2];
    int lspLifeTime[2];
    struct spf_info spf6[2];
}ISIS_SUMMARY_INFO_T;

typedef struct ISIS_HOSTNAME_INFO_S
{
    u_char sysid[ISIS_SYS_ID_LEN];
    int level;
    char hostname[256];
    BOOL bFirst;
}ISIS_HOSTNAME_INFO_T;

#define MAX_ISIS_NEXTHOP_NUM 5
typedef struct ISIS_ROUTE_INFO_S
{
    struct in_addr prefix;
    uchar_t prefixlen;
    int level;
    ulong_t      ulNexthopNum;
    struct in_addr      nexthop[MAX_ISIS_NEXTHOP_NUM];
    ulong_t      ulRouteTotal;
    ulong_t      ulCost;    
}ISIS_ROUTE_INFO_T;

#define MAX_SHOW_TLV_NUM 5
#define MAX_SHOW_SYSID_NUM 30
struct tlv_neighbor 
{
    char sysid[MAX_SHOW_SYSID_NUM];
    int metric;
};

struct tlv_reach 
{
  int metric;
  struct in_addr prefix;
  struct in_addr mask;
};

typedef struct ISIS_LSDB_INFO_S
{
    int level;
    BOOL bFirst;
    int totalCnt;
    char sysid[MAX_SHOW_SYSID_NUM];
    char lsp_id[ISIS_SYS_ID_LEN + 2];
    int own_lsp;
    ushort_t pdu_len;
    u_int16_t rem_lifetime;
    char lsp_bits[20];
    int age_out;
    u_int32_t seq_num;
    u_int16_t checksum;
    int areaCnt;
    struct net_area_info area_addr[MAX_AREA_ADDRESS_NUM];
    int nlpidCnt;
    uchar_t nlpids[4];
    char hostname[256];
    
    struct in_addr tlvAddr[MAX_SHOW_TLV_NUM];
    int tlvAddrCnt;
    struct tlv_neighbor tlvNeigh[MAX_SHOW_TLV_NUM];
    int tlvNeighCnt;
    struct tlv_reach tlvIntReach[MAX_SHOW_TLV_NUM];
    int tlvIntCnt;
    struct tlv_reach tlvExtReach[MAX_SHOW_TLV_NUM];
    int tlvExtCnt;
    
#if 0
    uchar_t auth_type;
    BOOL bRouterIdSet;
    struct in_addr router_id;    
#endif
}ISIS_LSDB_INFO_T;

typedef struct RIPNG_DEBUG_INFO_S
{
    int isDebugEvent;
    int isDebugPacket;
    int isDebugSend;
    int isDebugRecv;
    int isDebugZebra;
}RIPNG_DEBUG_INFO_T;

typedef struct 
{
    BOOL         bFirst;
    uchar_t      prefixlen;
    struct in6_addr prefix6;
    BOOL         bAggreate;
    struct in6_addr nexthop;
    ulong_t      ulRouteTotal;
    char         cType;
    int          type;
    int          sub_type;
    int          metric;
    int          tag;
    char         ifname[20];
    char         szTime[32];
}RIPNG_IP_ROUTE_STRC;

typedef struct RIPNG_INFO_S
{
    ulong_t update_time;
    ulong_t remain_time;
    ulong_t timeout_time;
    ulong_t garbage_time;
    int default_metric;
}RIPNG_INFO_T;

typedef struct RIPNG_REDIS_INFO_S
{
    int proto;
    char szName[32];
}RIPNG_REDIS_INFO_T;

typedef struct RIPNG_PEER_INFO_S
{
    BOOL bFirst;
    struct in6_addr addr6;
    int recv_badpackets;
    int recv_badroutes;
    int distance_default;
    char szUpTime[32];
}RIPNG_PEER_INFO_T;

typedef struct RIPNG_INTF_INFO_S
{
    char ifname[20];
    
    int split;
    
    int passive;

}RIPNG_INTF_INFO_T;

struct OSPF6_LSA_HEADER_INFO
{
    u_int16_t age;        
    u_int16_t type;       
    u_int32_t id;         
    u_int32_t adv_router; 
    u_int32_t seqnum;     
    u_int16_t checksum;   
    u_int16_t length;     
};

typedef struct OSPF6_LSA_INFO_S
{
    BOOL bFirst;
    struct in_addr area_id;
    struct OSPF6_LSA_HEADER_INFO header;    
    char ifname[20];
    char type_name[20];
    char duration[16];
}OSPF6_LSA_INFO_T;

typedef struct OSPF6_AREA_IF_INFO_S
{
    char ifname[20];
    struct in_addr area_id;
}OSPF6_AREA_IF_INFO_T;

typedef struct OSPF6_INFO_S
{
    struct in_addr router_id;
    BOOL isABR;
    BOOL isASBR;
}OSPF6_INFO_T;

typedef struct OSPF6_INTF_INFO_S
{
    BOOL bFirst;
    char ifname[20];
    struct in_addr areaId;
    struct in_addr dr;
    struct in_addr bdr;
    int type;
    int priority;
    ulong_t output_cost;
    BOOL bPassive;

    int hello_time;
    int dead_time;
    int transmit_delay;
    int retransmit_interval;

    char szState[20];
}OSPF6_INTF_INFO_T;

typedef struct OSPF6_AREA_INFO_S
{
    BOOL bFirst;
    struct in_addr area_id;
}OSPF6_AREA_INFO_T;

typedef struct OSPF6_NBR_INFO_S
{
    BOOL bFirst;
    struct in_addr router_id;
    ulong_t ifindex;
    u_char priority;
    u_char nbr_state;
    u_char if_state;
    char nstate[16];
    char duration[16];
    char deadtime[16];
    char ifname[20];   
}OSPF6_NBR_INFO_T;

typedef struct OSPF6_ROUTE_PATH_INFO_S
{
    struct in6_addr nexthop;
    char ifname[20];
}OSPF6_ROUTE_PATH_INFO_T;

typedef struct OSPF6_ROUTE_INFO_S
{
    BOOL bFirst;
    struct in6_addr prefix6;
    u_char prefixlen;
    BOOL bBest;
    char dst_type_str[8];
    char path_subtype_str[8];
    char destination[64];
    char duration[16];
    ulong_t metric;
    ulong_t pathCnt;
    OSPF6_ROUTE_PATH_INFO_T path[OSPF6_MULTI_PATH_LIMIT]; 
}OSPF6_ROUTE_INFO_T;

typedef struct OSPF6_LINKSTATE_INFO_S
{
    BOOL bFirst;
    struct in_addr areaId;
    struct in_addr advRtr;
    struct in_addr linkId;    
    char rbits[16];
    char options[16];
    ulong_t cost;
    
}OSPF6_LINKSTATE_INFO_T;

typedef struct OSPF6_BORDER_ROUTE_INFO_S
{
    BOOL bFirst;
    struct in_addr advRtr;
    struct in_addr areaId;    
    char rbits[16];
    char options[16];
    char path_type_name[16];
    
}OSPF6_BORDER_ROUTE_INFO_T;

typedef struct OSPF6_DEBUG_INFO_S
{
    ulong_t event;
    ulong_t ism;
    ulong_t nsm;
    ulong_t lsa;
    ulong_t zebra;
    ulong_t packet[6];
}OSPF6_DEBUG_INFO_T;

typedef struct RMAP_INFO_S
{
    char rmapName[NAME_SIZE+1];
} RMAP_INFO_T;

enum RULE_TYPE
{
    RULE_SET,
    RULE_MATCH
};

typedef struct RMAP_INDEX_INFO_S
{
    char rmapName[NAME_SIZE+1];
    int pref;
    int type;
    int exitpolicy;
    int nextpref;
    char nextrm[NAME_SIZE+1];  
}RMAP_INDEX_INFO_T;

typedef struct RMAP_RULE_INFO_S
{
    char rmapName[NAME_SIZE+1];
    int pref;
    int type;
    char cmd_str[NAME_SIZE+1];
    char rule_str[NAME_SIZE+1];
}RMAP_RULE_INFO_T;

enum 
{
  TYPE_IN, 
  TYPE_OUT,
  TYPE_MAX
};

typedef struct DISTRIBUTE_INFO_S
{
    int index;                     
    char ifname[32];
    char list[TYPE_MAX][NAME_SIZE+1];  
    char prefix[TYPE_MAX][NAME_SIZE+1]; 
}DISTRIBUTE_INFO_T;

typedef struct PLIST_MASTER_INFO_S
{
    char recentName[NAME_SIZE+1];
    int seqnum;
}PLIST_MASTER_INFO_T;

typedef struct PLIST_INFO_S
{
    char plistName[NAME_SIZE+1];
    int count;
    int rangecount;
    int headSeq;
    int tailSeq;
    char desc[80];
    
} PLIST_INFO_T;

typedef struct PLIST_ENTRY_INFO_S
{
    char plistName[NAME_SIZE+1];
    int seq;
    char typeStr[10];
    int any;
    char prefix[32];
    int ge;
    int le;
    ulong_t hitcnt;
    ulong_t refcnt;
    
}PLIST_ENTRY_INFO_T;

#define PREFIX_NO_MATCH   0x0
#define PREFIX_SAME       0x1
#define PREFIX_MATCH      0x3
typedef struct PLIST_ENTRY_PERFIX_INFO_S
{
    char plistName[NAME_SIZE+1];
    int seq;
    int same; 
    char typeStr[10];
    int any;
    char prefix[32];
    int ge;
    int le;
    ulong_t hitcnt;
    ulong_t refcnt;
    
}PLIST_ENTRY_PREFIX_INFO_T;

typedef struct ALIST_INFO_S
{
    char alistName[NAME_SIZE+1];
}ALIST_INFO_T;

typedef struct FILTER_CISCO_INFO_S
{
    
    int extended;
    struct in_addr addr;
    struct in_addr addr_mask;
    struct in_addr mask;
    struct in_addr mask_mask;
}FILTER_CISCO_INFO_T;

typedef struct FILTER_ZEBRA_INFO_S
{
    
    int exact;
    
    unsigned char family;
    unsigned char prefixlen;
    union 
    {
    struct in_addr prefix4;
    struct in6_addr prefix6;
    } u __attribute__ ((aligned (8)));
}FILTER_ZEBRA_INFO_T;

typedef struct FILTER_INFO_S
{
    BOOL bfirst; 
    char alistName[NAME_SIZE+1];
    int type;

    int cisco;
    
    union
    {
        FILTER_CISCO_INFO_T cfilter;
        FILTER_ZEBRA_INFO_T zfilter;
    } u;

}FILTER_INFO_T;
#endif

typedef struct {
    ulong_t          ulRouteTotalCount;
    ulong_t          ulRouteIpv4Count;
    ulong_t          ulRouteIpv6Count;
    ulong_t          ulSubnetTotalCount;
    ulong_t          ulSubnetIpv4Count;
    ulong_t          ulSubnetIpv6Count;
    ulong_t          ulStaticRouteTotalCount;
    ulong_t          ulStaticRouteIpv4Count;
    ulong_t          ulStaticRouteIpv6Count;
    ulong_t          ulRepRouteIpv4Count;
    ulong_t          ulRepRouteIpv6Count;
}RIB_STAT_STC;
int zebraIfIpAddressAdd(char *ifname, char *addr_str);

int zebraIfIpAddressDelete(char *ifname, char *addr_str);
int zebra_ipv6_forwarding(ulong_t bEnable);
int zebraIfIpv6AddressAdd(char *ifname, char *addr_str);

int zebraIfIpv6AddressDelete(char *ifname, char *addr_str);
int zebraIfIpv6LinkLocalAddrGet(char *ifname, 
    uchar_t *ucIpv6Addr, ulong_t *pulPrefixLen );

int zebraRibStatGet(RIB_STAT_STC *pStats);
int zebraRibStatLinklocalAddrCount(ulong_t bAdd);

extern int ripStatusGet(ulong_t *bEnable);
int ripVersionSet(int bNoFlag, int version);
int ripStaticRouteSet(int bNoFlag,char * szRoute);

int ripIpReceiveVersionSet(ulong_t bNoFlag, char* ifname, int version);

int ripIpSendVersionSet(ulong_t bNoFlag, char* ifname, int version);

int ripDefaultMetricSet(ulong_t bNoFlag, int metric);

int ripDistanceSet(int bNoFlag, int distance);

int ripDistanceSourceSet(int bNoFlag, const char *distance_str, const char *ip_str);

int rip_frr_distribute_list_set(int isNoFlag, const char *alist_name, const char *type, const char *ifname);
int frr_distribute_info_get(DATA_OP_ENT op,  DISTRIBUTE_INFO_T *pDistribute);
int rip_frr_access_list_zebra_set (int set, int afi,char *name_str, char *type_str, char *prefix_str, int exact);
int access_list_get_alist_info( DATA_OP_ENT op, int afi, ALIST_INFO_T *pAlistInfo);
int access_list_get_filter_info(DATA_OP_ENT op, int afi, FILTER_INFO_T *pFilterInfo);
int frrRipDebugSet(int bIsEnable, char *pRipDebugStr);

int frrLogStdoutSet(int isEnbale);

int frrLogFileNameSet(int isEnbale);

int ripdFrrStatusGet(int *bEnable);

int frrRipVersionGet(int *sendVersion, int *recvVersion);

int frrRipEnableNwInfoGet(DATA_OP_ENT op, RIP_ENABLE_NW_INFO_T *pRipEnableNwInfo);

int frrRipIpRouteGet(DATA_OP_ENT op, RIP_IP_ROUTE_STRC *pRipRoute);

int frrRipRouteNumGet(int *pulCnt);

int ripShow(char* szShowBuf, ulong_t bufSize);

int ripStatusShow(char* szShowBuf, ulong_t bufSize);

int ripNetworkSet(int bNoFlag, unsigned int ulIp, unsigned int ulMask, char *ifname);

int ripNeighborSet(ulong_t bNoFlag, ulong_t ulIp);

int ripAuthenticationSet(ulong_t bNoFlag, char* ifname,
                                     char* modeStr, char* keyStr);

int ripSplitHorizonSet(ulong_t bNoFlag, char* ifname);

int ripSplitHorizonPoisonedReverseSet(ulong_t bNoFlag, char* ifname);

int ripPassiveInterfaceSet(ulong_t bNoFlag, char* ifname);

int ripEventDebugSet(ulong_t bNoFlag);

int ripPacketDebugSet(ulong_t bNoFlag, char* str);

int ripZebraDebugSet(ulong_t bNoFlag);

int ripTimerSet(ulong_t bNoFlag, ulong_t update, ulong_t timeout, ulong_t garbage);

int ripOffsetListSet (ulong_t bNoFlag, char *alist, char *direct_str,
                         char *metric_str, char *ifname);
int ripRedistributeSelfSet(ulong_t bNoFlag);                        
int ripRedistributeSet(ulong_t bNoFlag, char *protoName, int metric, char *rmapName);                         
int ripDefaultOriginateSet(ulong_t bNoFlag);
int ripAggregateAddrSet(ulong_t bNoFlag, char *addr_str);
int ripRouteNumGet(ulong_t *pulCnt);
int ripInfoGet(RIP_INFO_T *pRipInfo);
int rip_vrrp_notify(char *szIfName, ulong_t ulVrid, ulong_t ulVrips[], 
    ulong_t ulVripNum, ulong_t bBackup);

#if 0
int ospfOpen();
int ospfClose();

int ospfPassiveIntfDefaultSet(BOOL bNoFlag);
int ospfPassiveIntfSet(BOOL bNoFlag, char *ifname, struct in_addr addr);
int ospfAreaNetworkSet(BOOL bNoFlag, ulong_t ulIp, ulong_t ulMask, struct in_addr areaId);
int ospfAreaRangeSet(BOOL bNoFlag, struct in_addr areaId, char *prefix_str, int advertise, 
                            ulong_t cost);
int ospfAreaRangeSubstituteSet(BOOL bNoFlag, struct in_addr areaId, char *prefix_src, 
                                        char *prefix_dst) ;
int ospfVirLinkSet(BOOL bNoFlag, struct ospf_vl_config_data *vl_config);
int ospfAreaShortCutSet(BOOL bNoFlag, struct in_addr areaId, char *type_str);
int ospfStubAreaSet(BOOL bNoFlag, struct in_addr areaID, int AreaSummaryStatus);
int ospfNssaAreaSet(BOOL bNoFlag, struct in_addr areaID, char *translator_role,
                          int AreaSummaryStatus);
int ospfAreaDefaultCostSet(BOOL bNoFlag, struct in_addr areaId, ulong_t cost);
int ospfAreaExportListSet(BOOL bNoFlag, struct in_addr areaId, char *alistName);
int ospfAreaImportListSet(BOOL bNoFlag, struct in_addr areaId, char *alistName);
int ospfAreaFilterListSet(BOOL bNoFlag, struct in_addr areaId, char *plistName, char *type_str);
int ospfAreaAuthTypeSet(BOOL bNoFlag, struct in_addr areaId, int auth_type);
int ospfAbrTypeSet(BOOL bNoFlag, char *abr_type_str);
int ospfLogAdjacencySet(BOOL bNoFlag, int detail);
int ospfRfc1583compatibilitySet(BOOL bNoFlag);
int ospfSpfTimerSet(BOOL bNoFlag, ulong_t delay, ulong_t hold, ulong_t max);
int ospfStaticNeighborSet(BOOL bNoFlag, struct in_addr nbr_addr, ulong_t priority, ulong_t interval);
int ospfRefreshTimerSet(BOOL bNoFlag, ulong_t time);
int ospfAutoCostSet(BOOL bNoFlag, ulong_t refbw);
int ospfIpAuthModeSet(BOOL bNoFlag, char *ifname, int auth_type, struct in_addr addr);
int ospfIpAuthSimpleSet(BOOL bNoFlag, char *ifname, char *passwdStr, struct in_addr addr);
int ospfIpAuthMd5Set(BOOL bNoFlag, char *ifname, ulong_t keyId, char *keyStr, struct in_addr addr);
int ospfIpParamSet(BOOL bNoFlag, char *ifname, ulong_t type, ulong_t value, struct in_addr addr);
int ospfIpDeadIntervalSet(BOOL bNoFlag, char *ifname, char *interval_str, char *fast_hello_str, struct in_addr addr);
int ospfIpNetworkSet(BOOL bNoFlag, char *ifname, int type);
int ospfRedistributeSet(BOOL bNoFlag, char *protoName, char *metric_str, 
                              char *type_str, char *rmapName);
int ospfDistrListOutSet(BOOL bNoFlag, char *alistName, char *protoName);
int ospfDftInfoSet(BOOL bNoFlag, BOOL always, char *metric_str, char *type_str, char *rmapName);
int ospfDefaultMetricSet(BOOL bNoFlag, ulong_t metric);
int ospfDistanceSet(BOOL bNoFlag, ulong_t distance);
int ospfDistanceOspfSet(BOOL bNoFlag, ulong_t intra_dis, ulong_t inter_dis, ulong_t external_dis);
int ospfDistancePrefixSet(BOOL bNoFlag, char *dis_str, char *prefix_str, char *alistName);
int ospfMtuIgnoreSet(BOOL bNoFlag, char *ifname, struct in_addr addr);
int ospfRouteNumGet(ulong_t *pulCnt);
int ospfStatusGet(BOOL *pBEnable);

int ospfPacketDebugSet(BOOL bNoFlag, char *type_str, char *direc_str);
int ospfCommonDebugSet(BOOL bNoFlag, char *type_str, char *detail_str);

int ospfCapabilityOpaqueSet(BOOL bNoFlag);
int ospf_vrrp_notify(char *szIfName, ulong_t ulVrid, ulong_t ulVrips[], 
    ulong_t ulVripNum, BOOL bBackup);
    
int ospfVlanIntfEnableBfdSet(BOOL bIsEnable, char *ifname);
int ospfBfdSearchIntfforVlan(ulong_t ulVlanId, BOOL bIsEnable);

#endif

#if 0
int isisOpen();
int isisClose();
int isisNetTitleSet(BOOL bNoFlag, char *net_title);
int isisLevelSet(BOOL bNoFlag, char *level_str);
int isisLspGenInvlSet(BOOL bNoFlag, char *level_str, int interval);
int isisLspLifeInvlSet(BOOL bNoFlag, char *level_str, int interval);

int isisLspRefreshInvlSet(BOOL bNoFlag, char *level_str, int interval);

int isisSpfInvlSet(BOOL bNoFlag, char *level_str, int interval);

int isisMetricStyleSet(BOOL bNoFlag, char *style_str);

int isisOverloadBitSet(BOOL bNoFlag);

int isisLogAdjacencySet(BOOL bNoFlag);

int isisAreaAuthSet(BOOL bNoFlag, char *auth_type, char *auth, char *direc_str);

int isisDomainAuthSet(BOOL bNoFlag, char *auth_type, char *auth, char *direc_str);

int isisDebugSet(BOOL bNoFlag, char *debug_str);

int isisStatusGet(BOOL *bEnable);

int isisLsdbInfoGet(DATA_OP_ENT op, ISIS_LSDB_INFO_T *pLsdbInfo);

int isisCircuitInfoGet(DATA_OP_ENT op, ISIS_CIRCUIT_INFO_T *pCircuitInfo);

int isisHostnameInfoGet(DATA_OP_ENT op, ISIS_HOSTNAME_INFO_T *pHostInfo);

int isisAdjInfoGet(DATA_OP_ENT op, ISIS_ADJ_INFO_T *pAdjInfo);

int isisDebugInfoGet(ISIS_DEBUG_INFO_T *pDebugInfo);

int isisSummaryInfoGet(ISIS_SUMMARY_INFO_T *pSummaryInfo);

int isisDynamicHostnameSet(BOOL bNoFlag);

int isisRouteInfoGet(DATA_OP_ENT op, ISIS_ROUTE_INFO_T *pRouteInfo);
int isisRouteNumGet(int level, ulong_t *pulCnt);
int isisCircuitSet(BOOL bNoFlag, char *ifname);

int isisCircuitPassiveSet(BOOL bNoFlag, char *ifname);

int isisCircuitLevelSet(BOOL bNoFlag, char *ifname, char *type_str); 

int isisCircuitPasswdSet(BOOL bNoFlag, char *ifname, char *type_str, char *passwd);

int isisCircuitParamSet(BOOL bNoFlag, int mode, char *ifname, char *level_str, int value);

int isisCircuitHelloPadSet(BOOL bNoFlag, char *ifname);

int isisCircuitPtoPSet(BOOL bNoFlag, char *ifname);
#endif

#if 0
int bgpOpen(ulong_t as_number);
int bgpClose(ulong_t as_number);
int bgpNetworkSet(BOOL bNoFlag, ushort_t afi, uchar_t safi, char *prefix_str, 
                        char *rmapName, int backdoor);
int bgpNeighborRemoteASSet(BOOL bNoFlag, ushort_t afi, uchar_t safi, 
                                      char *obj_str, ulong_t as_number);
int bgpNeighborGroupSet(BOOL bNoFlag, char *groupName);
int bgpNeighborLocalASSet(BOOL bNoFlag, char *obj_str, 
                                    ulong_t as_number, int no_prepend, int replace_as);
int bgpNeighborPasswdSet(BOOL bNoFlag, char *obj_str, char *passwd_str);
int bgpNeighborActivateSet(BOOL bNoFlag, ushort_t afi, uchar_t safi, char *obj_str);
int bgpNeighborGroupAddrSet(BOOL bNoFlag, ushort_t afi, uchar_t safi, char *addr_str, char *groupName);
int bgpNeighborFlagSet(BOOL bNoFlag, char *obj_str, char *flag_str);
int bgpNeighborEbgpMultihopSet(BOOL bNoFlag, char *obj_str, char *ttl_str);
int bgpNeighborTimerSet(BOOL bNoFlag, char *obj_str, char *keep_str, char *hold_str);
int bgpNeighborAfFlagSet(BOOL bNoFlag, ushort_t afi, uchar_t safi, char *obj_str, int flag);
int bgpNeighborDistributeListSet(BOOL bNoFlag, ushort_t afi, uchar_t safi, 
                                            char *obj_str, char *name_str, char *direc_str);
int bgpNeighborPrefixListSet(BOOL bNoFlag, ushort_t afi, uchar_t safi, 
                                            char *obj_str, char *name_str, char *direc_str);
int bgpNeighborFilterListSet(BOOL bNoFlag, ushort_t afi, uchar_t safi, 
                                            char *obj_str, char *name_str, char *direc_str);                                            
int bgpNeighborRoutemapSet(BOOL bNoFlag, ushort_t afi, uchar_t safi, 
                                            char *obj_str, char *name_str, char *direc_str);
int bgpNeighborAdvertiseIntervalSet(BOOL bNoFlag, char *ip_str, char *time_str);
int bgpNeighborUpdateSourceSet(BOOL bNoFlag, char *obj_str, char *ifname);
int bgpTimerSet(BOOL bNoFlag, char *keep_str, char *hold_str);
int bgpRouterIdSet(BOOL bNoFlag, char *ip_str);
int bgpClusterIdSet(BOOL bNoFlag, struct in_addr cluster_id);
int bgpDefaultLocalPreferenceSet(BOOL bNoFlag, char *local_pref_str);
int bgpFlagSet(BOOL bNoFlag, int flag);
int bgpRedistributeSet(BOOL bNoFlag, ushort_t afi, char *type_str, 
                             char *metric_str, char *rmapName);
int bgpPeerInfoGet(DATA_OP_ENT op, BGP_PEER_INFO_T *pBgpPeerInfo);
int bgpInfoGet(BGP_INFO_T *pBgpInfo);
int bgpRouteInfoGet(DATA_OP_ENT op, BGP_ROUTE_INFO_T *pRouteInfo);
int bgpRouteIndexInfoGet(BGP_ROUTE_INFO_T *pRouteInfo);

int bgpAsPathAlistSet(BOOL bNoFlag, char *name_str, char *direc_str, char *regex_str);
int bgpAslistInfoGet(DATA_OP_ENT op, BGP_AS_LIST_INFO_T *pAslistInfo);
int bgpAslistFilterInfoGet(DATA_OP_ENT op, BGP_AS_LIST_FILTER_INFO_T *pFilterInfo);
int bgpDebugSet(BOOL bNoFlag, char *debug_str);
int bgpDebugInfoGet(BGP_DEBUG_INFO_T *pDebugInfo);
int bgpAggregateAddressSet(BOOL bNoFlag, char *prefix_str, ushort_t afi, uchar_t safi,
                                     uchar_t summary_only, uchar_t as_set);
int bgpCommunitySet(BOOL bNoFlag, char *szName, char *szDirect, char *szCommunity, int style);
int bgpExtCommunitySet(BOOL bNoFlag, char *szName, char *szDirect, char *szType,
                                  char *szCommunity, int style);
int bgpCommunityListInfoGet(DATA_OP_ENT op, BGP_COMMUNITY_LIST_INFO_T *pBgpComListInfo);
int bgpCommunityEntryInfoGet(DATA_OP_ENT op, BGP_COMMUNITY_ENTRY_INFO_T *pBgpComEntryInfo);

int bgpConfederationIdSet(BOOL bNoFlag, ulong_t as);

int bgpConfederationPeerSet(BOOL bNoFlag, ulong_t as);
#endif

#if 0
int ripngStatusGet(BOOL *bEnable);
int ripngOpen();

int ripngClose();

int ripngDefaultMetricSet(BOOL bNoFlag, int metric);

int ripngEnableIfSet(BOOL bNoFlag, char *ifname);

int ripngSplitHorizonSet(BOOL bNoFlag, char* ifname);

int ripngSplitHorizonPoisonedReverseSet(BOOL bNoFlag, char* ifname);

int ripngEventDebugSet(BOOL bNoFlag);

int ripngPacketDebugSet(BOOL bNoFlag, char* str);

int ripngZebraDebugSet(BOOL bNoFlag);

int ripngPassiveInterfaceSet(BOOL bNoFlag, char* ifname);

int ripngTimerSet(BOOL bNoFlag, ulong_t update, ulong_t timeout, ulong_t garbage);

int ripngOffsetListSet (BOOL bNoFlag, char *alist, char *direct_str,
                      char *metric_str, char *ifname);

int ripngRedistributeSelfSet(BOOL bNoFlag);

int ripngAggregateAddrSet(BOOL bNoFlag, char *addr_str);

int ripngRedistributeSet(BOOL bNoFlag, char *protoName, int metric, char *rmapName);
int ripngRouteNumGet(ulong_t *pulCnt);
int ripngInfoGet(RIPNG_INFO_T *pRipngInfo);
#endif

#if 0
int ospf6StatusGet(BOOL *pBEnable);
int ospf6Open();
int ospf6Close();
int ospf6RouterIdSet(BOOL bNoFlag, struct in_addr router_id);
int ospf6IpParamSet(BOOL bNoFlag, char *ifname, ulong_t type, ulong_t value);
int ospf6AreaRangeSet(BOOL bNoFlag, struct in_addr areaId, char *prefix_str, char *adv_str);
int ospf6AreaExportListSet(BOOL bNoFlag, struct in_addr areaId, char *alistName);
int ospf6AreaImportListSet(BOOL bNoFlag, struct in_addr areaId, char *alistName);
int ospf6AreaFilterListSet(BOOL bNoFlag, struct in_addr areaId, char *plistName, char *type_str);
int ospf6RedistributeSet(BOOL bNoFlag, char *protoName,char *rmapName);
int ospf6IntfSet(BOOL bNoFlag, char *ifname, struct in_addr areaId);
int ospf6RouteNumGet(ulong_t *pulCnt);
int ospf6DebugSet(BOOL bNoFlag, char * type_str, char * detail_str, char * direc_str);
#endif

#if 0
int rmapSet(char *processName, char *rmapName, int permit, int pref, BOOL bNoFlag);
int rmapOnmatchNextSet(char *processName, char *rmapName, int permit, int pref, BOOL bNoFlag);
int rmapOnmatchGotoSet(char *processName, char *rmapName, int permit, int pref, int nextPos);
int rmapCallSet(char *processName, char *rmapName, int permit, int pref, BOOL bNoFlag, char *nextRmap);
int rmapMatchSet(char *processName, char *rmapName, int permit, int pref, BOOL bNoFlag, char *command, char *arg);
int rmapSetActionSet(char *processName, char *rmapName, int permit, int pref, BOOL bNoFlag, char *command, char *arg);
#endif

#if 0
int keySet(char *processName, BOOL bNoFlag, char *keyChainName, ulong_t index);
int keyChainSet(char *processName, BOOL bNoFlag, char *keyChainName);
int keyStringSet(char *processName, BOOL bNoFlag, char *keyChainName, ulong_t index, char *keyString);
int keyLifeTimeSet(char *processName, BOOL bNoFlag, char *keyChainName, ulong_t index, 
                        char *typeStr, 
                        char *stime_str, char *sday_str,
                        char *smonth_str, char *syear_str,
                        char *etime_str, char *eday_str,
                        char *emonth_str, char *eyear_str);
int keyLifeTimeInfiniteSet(char *processName, BOOL bNoFlag, char *keyChainName, 
                                ulong_t  index, char *typeStr, 
                                char *stime_str, char *sday_str,
                                char *smonth_str, char *syear_str);
int keyLifeTimeDurationSet(char *processName, BOOL bNoFlag, char *keyChainName, 
                                   ulong_t  index, char *typeStr, 
                                   char *stime_str, char *sday_str,
                                   char *smonth_str, char *syear_str,
                                   char *duration_str);
#endif

#if 0
int ifrmapSet(char * processName,BOOL bNoFlag,char * rmapName,int type,char * ifname);
#endif

#if 0
int distributeListSet(char * processName,BOOL bNoFlag,char * alistName,int type,char * ifname);
int distributeListPrefixSet(char * processName,BOOL bNoFlag,char * prefixName,int type,char * ifname);
int distributeInfoGet(char * processName,DATA_OP_ENT op,DISTRIBUTE_INFO_T * pDistribute);
#endif

#if 0
int plistSet(char *processName, BOOL bNoFlag, int afi, char *plistName, char *seq, char *typestr, char *prefix, 
              char *ge, char *le);

int plistSeqNumberSet(char *processName, BOOL bNoFlag, int afi);                                

int plistClear(char *processName, int afi, char *plistName, char *prefix);
int plistGetPlistInfo(char *processName, DATA_OP_ENT op, int afi, PLIST_INFO_T *pplist);
#endif

#if 0
int alistCiscoSet(char *processName, int afi, char *alistName, char *typestr, char *src_addr_str, 
                     char *src_mask_str, char *dst_addr_str, char *dst_mask_str, int extended, int set);

int alistZebraSet(char *processName, char *alistName, char *typestr, int afi, char *prefix_str, 
                 int exact_match, int set);

int alistGetAlistInfo(char *processName, DATA_OP_ENT op, int afi, ALIST_INFO_T *pAlistInfo);

int alistGetFilterInfo(char *processName, DATA_OP_ENT op, int afi, FILTER_INFO_T *pFilterInfo);

int alistDel(char *processName, char *alistName);
#endif
#endif 

