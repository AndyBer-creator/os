
#ifndef __PROTO_LLDP_H__
#define __PROTO_LLDP_H__
#include <common/sys_def.h>
#define LLDP_DBG(string, args...) SYS_DBG(DBG_LLDP_COMMON, "[LLDP DBG] "string"", ##args)
#define LLDP_RX_DBG(string, args...) SYS_DBG(DBG_LLDP_RX, "[LLDP Rx DBG] "string"", ##args)
#define LLDP_TX_DBG(string, args...) SYS_DBG(DBG_LLDP_TX, "[LLDP Tx DBG] "string"", ##args)
#define LLDP_POE_DBG(string, args...) SYS_DBG(DBG_LLDP_POE, "[LLDP PoE DBG] "string"", ##args)
#define LLDP_PKT_DBG(string, args...) SYS_DBG(DBG_LLDP_PKT, "[LLDP Pkt DBG] "string"", ##args)
#define XVALIDTLV                       0
#define XEINVALIDTLV                    1
#define LLDP_ETHERTYPE                  0x88CC
#define ETHERTYPE_OFFSET                2
#define VLANID_OFFSET                   4
#define SUBTYPE_OFFSET                  1
#define TLVH_OFFSET                     2
#define CAPA_TLV_OFFSET                 2
#define SA_OFFSET                       6
#define MAC_HEADER_LEN                  12
#define TTL_TLV_LEN                     2
#define CAPA_TLV_LEN                    4
#define MAC_PHY_TLV_LEN                 9
#define END_TLV_LEN                     0
#define MAX_TLV_NUM                     128
#define MAX_NEIGHBOR_ENTRY              32
#define MAX_TLV_STRING_LEN              256
#define MAX_INVENTORY_STRING_LEN        64
#define VLAN_NAME_TLV_STR_LEN           512
#define MGMT_ADDR_STR_LEN               1
#define MGMT_ADDR_SUBTYPE               1
#define IPV4_LEN                        4
#define IPV6_LEN                        16
#define IF_NUM_SUBTYPE                  1
#define IF_NUM                          4
#define OID                             1
#define OBJ_IDENTIFIER                  0
#define MAX_LLDP_MED_NETWORK_POLICY_RX  8
#define MAX_LLDP_MANAGEMENT_ADDR_NUM    17  

#define ORG_UNIQUE_CODE_LEN             3
#define SUB_TYPE_LEN                 1
#define AUTO_NEGO_STATS_LEN             1
#define AUTO_NEGO_CAP_LEN               2
#define OPER_TYPE_LEN                   2

#define ORG_UNIQUE_CODE_802_3           0x120f
#define SUB_TYPE_MAC_PHY                1

#define AGG_STATS_LEN                   1
#define AGG_ID_LEN                      4

#define SUB_TYPE_AGG                    3

#define MAX_FRAME_SIZE_LEN              2

#define SUB_TYPE_MAX_FRAME_SIZE         4

#define MDI_POWER_SUPPORT_LEN           1
#define PSE_POWER_PAIR_LEN              1
#define POWER_CLASS_LEN                 1

#define SUB_TYPE_802_3_POWER_VIA_MDI    2
#define PORT_VLAN_ID_LEN                2

#define ORG_UNIQUE_CODE_802_1           0x80c2
#define SUB_TYPE_PORT_VLAN_ID           1

#define VLAN_ID_LEN                     2
#define VLAN_NAME_LEN_LEN               1

#define SUB_TYPE_PORT_VLAN_NAME         3
#define MEDIA_CAPA_LEN                  2
#define CLASS_TYPE_LEN                  1

#define ORG_UNIQUE_CODE_TIA             0x12bb
#define SUB_TYPE_MEDIA_CAPA             1

#define LOCATION_DATA_FMT_LEN           1

#define SUB_TYPE_LOCATION               3
#define LOCATION_DATA_FMT_COORDINATE    1
#define LOCATION_DATA_FMT_CIVIC_ADDR    2
#define LOCATION_DATA_FMT_ECS_ELIN      3

#define APP_TYPE_LEN                    1
#define POLICY_LEN                      3

#define SUB_TYPE_NETWORK_POLICY         2

#define POWER_INFO_LEN                  1
#define POWER_VAL_LEN                   2

#define SUB_TYPE_EXT_POWER_VIA_MDI      4
#define SAME_NEIGHBOR                   0
#define NEW_NEIGHBOR                    1
#define RX_QUEUE_LENGTH                 256
#define LLDP_MAX_ENTRY_PER_PORT         5
#define DEFAULT_TX_INTERVAL             30
#define DEFAULT_HOLDTIME_MUL            4
#define DEFAULT_TX_DELAY                2
#define DEFAULT_REINIT_DELAY            2
#define END_OF_LLDPDU_TLV               0    
#define CHASSIS_ID_TLV                  1    
#define PORT_ID_TLV                     2    
#define TIME_TO_LIVE_TLV                3    
#define PORT_DESCRIPTION_TLV            4    
#define SYSTEM_NAME_TLV                 5    
#define SYSTEM_DESCRIPTION_TLV          6    
#define SYSTEM_CAPABILITIES_TLV         7    
#define MANAGEMENT_ADDRESS_TLV          8    

#define ORG_SPECIFIC_TLV                127  
#define CHASSIS_ID_CHASSIS_COMPONENT    1
#define CHASSIS_ID_INTERFACE_ALIAS      2
#define CHASSIS_ID_PORT_COMPONENT       3
#define CHASSIS_ID_MAC_ADDRESS          4
#define CHASSIS_ID_NETWORK_ADDRESS      5
#define CHASSIS_ID_INTERFACE_NAME       6
#define CHASSIS_ID_LOCALLY_ASSIGNED     7
#define PORT_ID_INTERFACE_ALIAS         1
#define PORT_ID_PORT_COMPONENT          2
#define PORT_ID_MAC_ADDRESS             3
#define PORT_ID_NETWORK_ADDRESS         4
#define PORT_ID_INTERFACE_NAME          5
#define PORT_ID_AGENT_CIRCUIT_ID        6
#define PORT_ID_LOCALLY_ASSIGNED        7
#define SYSTEM_CAPABILITY_OTHER         1
#define SYSTEM_CAPABILITY_REPEATER      2
#define SYSTEM_CAPABILITY_BRIDGE        4
#define SYSTEM_CAPABILITY_WLAN          8
#define SYSTEM_CAPABILITY_ROUTER        16
#define SYSTEM_CAPABILITY_TELEPHONE     32
#define SYSTEM_CAPABILITY_DOCSIS        64
#define SYSTEM_CAPABILITY_STATION       128
#define NETWORK_POLICY_BINDING_NONE     0xff

typedef enum lldp_link_e
{
    LLDP_LINKDOWN = 0,
    LLDP_LINKUP,
    LLDP_LINKEND
} lldp_link_t;

typedef enum lldp_portState_e
{
    LLDP_PORT_RXONLY = 1,
    LLDP_PORT_TXONLY,
    LLDP_PORT_TXRX,
    LLDP_PORT_DISABLE,
    LLDP_PORT_END
} lldp_portState_t;

typedef enum lldp_classType_e
{
    LLDP_CLASS_TYPE_UNDEF = 0,
    LLDP_CLASS_TYPE_ENDPOINT_CLASS1,
    LLDP_CLASS_TYPE_ENDPOINT_CLASS2,
    LLDP_CLASS_TYPE_ENDPOINT_CLASS3,
    LLDP_CLASS_TYPE_NETWORK_CONN,
    LLDP_CLASS_TYPE_END
} lldp_classType_t;

typedef enum lldp_poe_pwrType_e
{
    LLDP_PWR_TYPE_2_PSE = 0,
    LLDP_PWR_TYPE_2_PD,
    LLDP_PWR_TYPE_1_PSE,
    LLDP_PWR_TYPE_1_PD,
    LLDP_PWR_TYPE_END
} lldp_poe_pwrType_t;

typedef enum lldp_poe_pwrSrc_e
{
    LLDP_PWR_SRC_UNKNOWN = 0,
    LLDP_PWR_SRC_PRIMARY,
    LLDP_PWR_SRC_BACKUP,
    LLDP_PWR_SRC_ALL,
    LLDP_PWR_SRC_END
} lldp_poe_pwrSrc_t;

typedef enum lldp_poe_pwrPri_e
{
    LLDP_PWR_PRI_UNKNOWN = 0,
    LLDP_PWR_PRI_CRITICAL,
    LLDP_PWR_PRI_HIGH,
    LLDP_PWR_PRI_LOW,
    LLDP_PWR_PRI_END,
} lldp_poe_pwrPri_t;

typedef enum lldp_poe_pwrPair_e
{
    LLDP_PWR_PAIR_SIGNAL = 0,
    LLDP_PWR_PAIR_SPARE,
    LLDP_PWR_PAIR_END
} lldp_poe_pwrPair_t;

#ifdef CONFIG_SYS_PROTO_LLDP_MED
typedef enum lldp_network_policy_app_e
{
    LLDP_APP_VOICE = 1,
    LLDP_APP_VOICE_SIGNALING,
    LLDP_APP_GUEST_VOICE,
    LLDP_APP_GUEST_VOICE_SIGNALING,
    LLDP_APP_SOFTPHONE_VOICE,
    LLDP_APP_VIDEO_CONFERENCING,
    LLDP_APP_STREAMING_VIDEO,
    LLDP_APP_VIDEO_SIGNALING,
    LLDP_APP_END
} lldp_network_policy_app_t;
#endif

typedef enum lldp_disable_action_e
{
    LLDP_DISABLE_ACTION_FILTER = 0,
    LLDP_DISABLE_ACTION_FLOOD_TO_VLAN,
    LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS,
    LLDP_DISABLE_ACTION_END
} lldp_disable_action_t;

typedef enum lldp_inventory_type_e
{
    LLDP_INVENTORY_TYPE_HARDWARE_REVISION = 5,
    LLDP_INVENTORY_TYPE_FIRMWARE_REVISION,
    LLDP_INVENTORY_TYPE_SOFTWARE_REVISION,
    LLDP_INVENTORY_TYPE_SERIA_NUMBER,
    LLDP_INVENTORY_TYPE_MANUFACTURER,
    LLDP_INVENTORY_TYPE_MODEL_NAME,
    LLDP_INVENTORY_TYPE_ASSET_ID,
    LLDP_INVENTORY_TYPE_END
} lldp_inventory_type_t;

typedef enum lldp_phy_capa_e
{
    LLDP_PHY_CAPA_1000BASETFD,
    LLDP_PHY_CAPA_1000BASET,
    LLDP_PHY_CAPA_1000BASEXFD,
    LLDP_PHY_CAPA_1000BASEX,
    LLDP_PHY_CAPA_FDXBPAUSE,
    LLDP_PHY_CAPA_FDXSPAUSE,
    LLDP_PHY_CAPA_FDXAPAUSE,
    LLDP_PHY_CAPA_FDXPAUSE,
    LLDP_PHY_CAPA_100BASET2FD,
    LLDP_PHY_CAPA_100BASET2,
    LLDP_PHY_CAPA_100BASETXFD,
    LLDP_PHY_CAPA_100BASETX,
    LLDP_PHY_CAPA_100BASET4,
    LLDP_PHY_CAPA_10BASETFD,
    LLDP_PHY_CAPA_10BASET,
    LLDP_PHY_CAPA_OTHER,
    LLDP_PHY_CAPA_END
} lldp_phy_capa_t;

typedef enum lldp_phy_oper_e
{
    LLDP_PHY_OPER_OTHER,
    LLDP_PHY_OPER_AUI,
    LLDP_PHY_OPER_10BASE5,
    LLDP_PHY_OPER_FOIRL,
    LLDP_PHY_OPER_10BASE2,
    LLDP_PHY_OPER_10BASET,
    LLDP_PHY_OPER_10BASEFP,
    LLDP_PHY_OPER_10BASEFB,
    LLDP_PHY_OPER_10BASEFL,
    LLDP_PHY_OPER_10BROAD36,
    LLDP_PHY_OPER_10BASETHD,
    LLDP_PHY_OPER_10BASETFD,
    LLDP_PHY_OPER_10BASEFLHD,
    LLDP_PHY_OPER_10BASEFLFD,
    LLDP_PHY_OPER_100BASET4,
    LLDP_PHY_OPER_100BASETXHD,
    LLDP_PHY_OPER_100BASETXFD,
    LLDP_PHY_OPER_100BASEFXHD,
    LLDP_PHY_OPER_100BASEFXFD,
    LLDP_PHY_OPER_100BASET2HD,
    LLDP_PHY_OPER_100BASET2FD,
    LLDP_PHY_OPER_1000BASEXHD,
    LLDP_PHY_OPER_1000BASEXFD,
    LLDP_PHY_OPER_1000BASELXHD,
    LLDP_PHY_OPER_1000BASELXFD,
    LLDP_PHY_OPER_1000BASESXHD,
    LLDP_PHY_OPER_1000BASESXFD,
    LLDP_PHY_OPER_1000BASECXHD,
    LLDP_PHY_OPER_1000BASECXFD,
    LLDP_PHY_OPER_1000BASETHD,
    LLDP_PHY_OPER_1000BASETFD,
    LLDP_PHY_OPER_10GBASEX,
    LLDP_PHY_OPER_10GBASELX4,
    LLDP_PHY_OPER_10GBASER,
    LLDP_PHY_OPER_10GBASEER,
    LLDP_PHY_OPER_10GBASELR,
    LLDP_PHY_OPER_10GBASESR,
    LLDP_PHY_OPER_10GBASEW,
    LLDP_PHY_OPER_10GBASEEW,
    LLDP_PHY_OPER_10GBASELW,
    LLDP_PHY_OPER_10GBASESW,
    LLDP_PHY_OPER_END
} lldp_phy_oper_t;

typedef enum lldp_tlv_notify_type_e
{
    LLDP_TLV_NOTIFY_SYSTEM_NAME = 0,
    LLDP_TLV_NOTIFY_SYSTEM_DESCRIPTION,
    LLDP_TLV_NOTIFY_PORT_DESCRIPTION,
    LLDP_TLV_NOTIFY_MANAGEMENT_ADDR,
    LLDP_TLV_NOTIFY_TRUNK,
    LLDP_TLV_NOTIFY_MAX_FRAME_SIZE,
    LLDP_TLV_NOTIFY_EXTEND_POE,
    LLDP_TLV_NOTIFY_PVID,
    LLDP_TLV_NOTIFY_VLAN_NAME,
    LLDP_TLV_NOTIFY_NETWORK_POLICY,
    LLDP_TLV_NOTIFY_INVENTORY,
    LLDP_TLV_NOTIFY_LOCATION,
    LLDP_TLV_NOTIFY_VOICE_AUTO_EBL,
    LLDP_TLV_NOTIFY_VOICE_AUTO_CONTENT,
    LLDP_TLV_NOTIFY_TLV_ADD_RMV,
    LLDP_TLV_NOTIFY_MED_TLV_ADD_RMV,
    LLDP_TLV_NOTIFY_END
} lldp_tlv_notify_type_t;

typedef enum lldp_capa_e
{
    LLDP_CAPA_PORT_DESC = 0,
    LLDP_CAPA_SYS_NAME,
    LLDP_CAPA_SYS_DESC,
    LLDP_CAPA_SYS_CAPA,
    LLDP_CAPA_MAC_PHY,
    LLDP_CAPA_LAG,
    LLDP_CAPA_MAX_FRAME_SIZE,
    LLDP_CAPA_MANAGEMENT_ADDR,
    LLDP_CAPA_END
} lldp_capa_t;
#ifdef CONFIG_SYS_PROTO_LLDP_MED
typedef enum lldp_med_capa_e
{
    LLDP_MED_CAPA_CAPA = 0,
    LLDP_MED_CAPA_NETWORK_POLICY,
    LLDP_MED_CAPA_LOCATION,
    LLDP_MED_CAPA_PSE,
    LLDP_MED_CAPA_PD,
    LLDP_MED_CAPA_INVENTORY,
    LLDP_MED_CAPA_END
} lldp_med_capa_t;
#endif
typedef struct lldp_tlv_sel_s
{
    uint8       portDesciptionSel;
    uint8       sysNameSel;
    uint8       sysDescriptionSel;
    uint8       sysCapabilitySel;
    uint8       macPhySel;
    uint8       linkAggregationSel;
    uint8       maxFrameSizeSel;
    uint8       managementAddrSel;
} lldp_tlv_sel_t;

#ifdef CONFIG_SYS_PROTO_LLDP_MED
typedef struct lldp_poe_portL2Data_s
{
    uint32             power;   
    lldp_poe_pwrPri_t  pri;     
    lldp_poe_pwrSrc_t  pwrSrc;  
    lldp_poe_pwrType_t pwrType; 
    lldp_poe_pwrPair_t pwrPair; 
} lldp_poe_portL2Data_t;
#endif

#ifdef CONFIG_SYS_PROTO_LLDP_MED
typedef struct lldp_med_tlv_sel_s
{
    uint8       networkPolicySel;
    uint8       locationSel;
    uint8       poePseSel;
    uint8       inventorySel;
} lldp_med_tlv_sel_t;

typedef struct lldp_med_networkPolicy_s
{
    uint8       valid;
    lldp_network_policy_app_t   app;
    char appStr[SYS_BUF32_LEN];
    sys_vid_t   vid;
    uint8       vlanTagged;
    uint8       userPriority;
    uint8       dscpValue;
} lldp_med_networkPolicy_t;

typedef struct lldp_med_networkPolicy_bind_s
{
    uint32  bindMask;
} lldp_med_networkPolicy_bind_t;

typedef struct lldp_med_location_s
{
    uint8       coorLen;
    uint8       coordinate[CAPA_LLDP_MED_COORDINATE_LEN+1];
    uint16      civicLen;
    uint8       civicAddress[BOND_LLDP_MED_CIVIC_LEN_MAX +1];
    uint16      ecsLen;
    uint8       ecsElin[BOND_LLDP_MED_ELIN_LEN_MAX+1];
} lldp_med_location_t;

typedef struct policy_s
{
    uint32    unknown:1                __attribute__((packed)) ;
    uint32    tag:1                    __attribute__((packed)) ;
    uint32    reserved:1               __attribute__((packed)) ;
    uint32    vlanId:12                __attribute__((packed)) ;
    uint32    l2Pri:3                  __attribute__((packed)) ;
    uint32    dscpPri:6                __attribute__((packed)) ;
} policy_t;
#endif 

typedef struct power_info_s
{
    uint8    type:2                 __attribute__((packed)) ;
    uint8    source:2               __attribute__((packed)) ;
    uint8    pri:4                  __attribute__((packed)) ;
} power_info_t;

typedef struct power_support_s
{
    uint8    portClass:1                 __attribute__((packed)) ;
    uint8    powerSupport:1               __attribute__((packed)) ;
    uint8    powerState:1                  __attribute__((packed)) ;
    uint8    pairsControl:1                  __attribute__((packed)) ;
    uint8    reserved:4                  __attribute__((packed)) ;
} power_support_t;

typedef struct lldp_port_entry_s
{
    sys_enable_t    tx_enabled;
    sys_enable_t    rx_enabled;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    sys_enable_t    medEnable;
    uint8    medReceive;
    uint8    isVoiceVlanPort;
  #endif
    sys_enable_t    remoteChangeFlag;
    lldp_tlv_sel_t  lldpTlvSel;
    sys_enable_t    tlvPvidEbl;
    sys_vlanmask_t  tlvVlanmask;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    lldp_med_tlv_sel_t  lldpMedTlvSel;
    lldp_med_networkPolicy_bind_t   policyBind;
    lldp_med_location_t location;
  #endif
    uint32          hold_timer;
    uint32          tx_timer;
    lldp_link_t     link_status;
    uint8           reinitdelay_timer;
    uint16          txdelay_timer;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    uint8           fast_repeat_remain;
  #endif
}lldp_port_entry_t;

typedef struct lldp_port_table_s
{
    lldp_port_entry_t port_entry[SYS_LOGICPORT_NUM_MAX];
} lldp_port_table_t;

typedef struct lldp_tlv_s
{
    uint8       tlv_type;
    uint8       sub_type;
    uint8       tlv_mac[CAPA_MAC_BYTE_NUM];
    uint16      tlv_info_len;
    uint16      tlv_value;
    uint16      sub_value;
    uint32      tlv_ip;
    char        tlv_string[MAX_TLV_STRING_LEN+1];
} lldp_tlv_t;

typedef struct lldp_manAddr_tlv_s
{
    uint8   valid;
    int8    len;
    int8    iSubtype;
    uint8   addr[16];
    int8    interfaceSubtype;
    int32   interfaceNum;
} lldp_manAddr_tlv_t;

typedef struct lldp_macPhy_tlv_s
{
    uint8 valid;
    uint8 autoNego;
    uint16 capa;
    uint16 oper;
} lldp_macPhy_tlv_t;

typedef struct lldp_trunk_tlv_s
{
    uint8 valid;
    uint8 aggInfo;
    uint32 aggId;
} lldp_trunk_tlv_t;

typedef struct lldp_maxFrameSize_tlv_s
{
    uint8 valid;
    uint16 maxFrameSize;
} lldp_maxFrameSize_tlv_t;

typedef struct lldp_powerViaMdi_tlv_s
{
    uint8 valid;
    power_support_t support;
    uint8 portPair;
    uint8 powerClass;
    uint8 extValid;
    power_info_t powerInfo;
    uint16  requestPower;
    uint16  allocatePower;
} lldp_powerViaMdi_tlv_t;

typedef struct lldp_pvid_tlv_s
{
    uint8 valid;
    uint16 pvid;
} lldp_pvid_tlv_t;

typedef struct lldp_vlanName_tlv_s
{
    uint8 valid;
    uint16 vid;
    uint8 len;
    sys_namestr_t name;
    struct lldp_vlanName_tlv_s *pNext;
} lldp_vlanName_tlv_t;

#ifdef CONFIG_SYS_PROTO_LLDP_MED
typedef struct lldp_med_inventory_tlv_s
{
    uint8 valid;
    char strInventory[MAX_INVENTORY_STRING_LEN+1];
} lldp_med_inventory_tlv_t;

typedef struct lldp_med_media_capa_tlv_s
{
    uint8 valid;
    uint16 capa;
    uint8 classType;
} lldp_med_media_capa_tlv_t;

typedef struct lldp_med_location_tlv_s
{
    uint8 valid;
    lldp_med_location_t location;
} lldp_med_location_tlv_t;

typedef struct lldp_med_networkPolicy_tlv_s
{
    uint8 valid;
    uint8 app;
    policy_t policyInfo;
} lldp_med_networkPolicy_tlv_t;

typedef struct lldp_med_poe_tlv_s
{
    uint8 valid;
    power_info_t powerInfo;
    uint16 powerVal;
} lldp_med_poe_tlv_t;
#endif 

typedef struct neighbor_entry_s
{
    uint8       enabled;
    uint8       port_index;
    lldp_tlv_t  chasis_id_tlv;      
    lldp_tlv_t  port_id_tlv;        
    uint16      TTL;                
    uint16      time_tlv_value;
    lldp_tlv_t  sys_capa_tlv;
    char        port_description[MAX_TLV_STRING_LEN+1];
    char        system_name[CAPA_SYS_NAME_LEN+1];
    char        system_description[CAPA_SYS_DESC_LEN+1];
    uint16      system_capabilities;
    lldp_manAddr_tlv_t manage_address[MAX_LLDP_MANAGEMENT_ADDR_NUM];
    lldp_tlv_t  sys_mana_tlv;
    lldp_macPhy_tlv_t   macPhy_tlv;
    lldp_trunk_tlv_t    trunk_tlv;
    lldp_maxFrameSize_tlv_t maxFrameSize_tlv;
    uint8       powerAllocBy; 
    lldp_powerViaMdi_tlv_t  powerViaMdi_tlv;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    lldp_med_inventory_tlv_t hardware_v_tlv;
    lldp_med_inventory_tlv_t firmware_v_tlv;
    lldp_med_inventory_tlv_t software_v_tlv;
    lldp_med_inventory_tlv_t serial_num_tlv;
    lldp_med_inventory_tlv_t manufacturer_tlv;
    lldp_med_inventory_tlv_t model_name_tlv;
    lldp_med_inventory_tlv_t asset_tlv;
    lldp_med_media_capa_tlv_t   media_capa_tlv;
    lldp_med_location_tlv_t location_tlv;
    uint8 isVoiceVidMatch;
    lldp_med_networkPolicy_tlv_t policy_tlv[MAX_LLDP_MED_NETWORK_POLICY_RX];
    lldp_med_poe_tlv_t  poe_tlv;
  #endif
    lldp_pvid_tlv_t pvid_tlv;
    uint8 vlanNum;
    lldp_vlanName_tlv_t vlanName_tlv;
} neighbor_entry_t;

typedef struct lldp_database_s
{
    neighbor_entry_t entry[SYS_NORMALPORT_NUM_MAX][LLDP_MAX_ENTRY_PER_PORT];
} lldp_database_t;

#ifdef CONFIG_SYS_PROTO_LLDP_MED

typedef struct sys_lldp_voice_vlan_event_s
{
    sys_notifier_subject_lldp_event_t event;
    sys_logic_port_t port;
} sys_lldp_voice_vlan_event_t;
#endif
typedef struct sys_lldp_tlv_change_event_s
{
    uint8 isAllPort;
    sys_logic_port_t    port;
    lldp_tlv_notify_type_t   type;
} sys_lldp_tlv_change_event_t;

typedef struct lldp_port_statistic_s
{
    uint32          tx_frame;
    uint32          rx_frame;
    
    uint32          rx_discard_frame;
    
    uint32          rx_error_frame;
    uint32          rx_discard_tlv;
    uint32          rx_unRecognize_tlv;
    uint32          rx_ageOut;
} lldp_port_statistic_t;

typedef struct lldp_statistic_s
{
    uint32  insertion;
    uint32  deletion;
    uint32  drop;
    uint32  age_out;
    lldp_port_statistic_t port_statistics[SYS_LOGICPORT_NUM_MAX];
} lldp_statistic_t;

typedef struct lldp_overLoading_tlv_s
{
    uint32 byte;
    uint8 isOverload;
} lldp_overLoading_tlv_t;

typedef struct lldp_overLoading_s
{
    lldp_overLoading_tlv_t mandatory;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    lldp_overLoading_tlv_t medCapability;
    lldp_overLoading_tlv_t medLocation;
    lldp_overLoading_tlv_t medPolicy;
    lldp_overLoading_tlv_t medPoe;
  #endif
    lldp_overLoading_tlv_t tlv802dot3;
    lldp_overLoading_tlv_t optional;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    lldp_overLoading_tlv_t medInventory;
  #endif
    lldp_overLoading_tlv_t tlv802dot1;
    uint8 isOverload;
    int32 maxSize;
    int32 totalSize;
    int32 leftSize;
} lldp_overLoading_t;

typedef struct manage_address_s{
    uint8   valid;
    int8    len;
    int8    iSubtype;
    uint8   addr[16];
    int8    interfaceSubtype;
    int32   interfaceNum;
} manage_address_t;

typedef struct display_entry_s
{
    uint32 index;
    sys_logic_port_t logicPort;
    struct {
        int16   iSubtype;
        char    subtype[SYS_BUF32_LEN];
        uint8   iId[CAPA_MAC_BYTE_NUM];
        char    id[MAX_TLV_STRING_LEN+1];
    } chasis_id;
    struct {
        int16   iSubtype;
        char    subtype[SYS_BUF32_LEN];
        char    id[MAX_TLV_STRING_LEN+1];
    } port_id;
    char    port_description[MAX_TLV_STRING_LEN+1];
    char    system_name[CAPA_SYS_NAME_LEN+1];
    char    system_description[CAPA_SYS_DESC_LEN+1];
    struct {
        uint16  iSupport;
        char    support[SYS_BUF64_LEN];
        uint16  iEnable;
        char    enable[SYS_BUF64_LEN];
    } system_capabilities;
    manage_address_t manage_address[MAX_LLDP_MANAGEMENT_ADDR_NUM];
    lldp_maxFrameSize_tlv_t maxFrameSize;
    struct {
        uint8   valid;
        uint8   support;
        uint8   enable;
        uint16  iCapa;
        char    capa[SYS_BUF512_LEN];
        uint16  iOper;
        char    oper[SYS_BUF32_LEN];
    } macPhy;
    struct {
        uint8   valid;
        uint8   powerAllocBy; 
        uint8   iPortClass;
        char    portClass[SYS_BUF8_LEN+1];
        uint8   iPowerSupport;
        char    powerSupport[SYS_BUF16_LEN+1];
        uint8   iPowerState;
        char    powerState[SYS_BUF16_LEN+1];
        uint8   iPairControlAbility;
        char    pairControlAbility[SYS_BUF16_LEN+1];
        uint8   iPowerPair;
        char    powerPair[SYS_BUF8_LEN+1];
        uint8   iPowerClass;
        char    powerClass[SYS_BUF8_LEN+1];
        uint8   extValid;
        uint8   iDevType;
        char    devType[SYS_BUF32_LEN];
        uint8   iSrc;
        char    src[SYS_BUF32_LEN];
        uint8   iPri;
        char    pri[SYS_BUF16_LEN];
        uint16  requestPower;
        uint16  allocatePower;
    } dot3power;
    struct {
        uint8   valid;
        uint8   iCapa;
        char    capa[SYS_BUF32_LEN];
        uint8   iState;
        char    state[SYS_BUF32_LEN];
        uint32  id;
    } trunk;
    struct {
        uint8   valid;
        uint16  pvid;
    } pvid;
    uint16 vlanNum;
    uint16  lldp_ttl;
    uint8   powerAllocBy; 
} display_entry_t;

#ifdef CONFIG_SYS_PROTO_LLDP_MED
typedef struct display_med_entry_s
{
    uint32 index;
    sys_logic_port_t logicPort;

    struct {
        uint8   valid;
        uint8   powerAllocBy; 
        uint8   iDevType;
        char    devType[SYS_BUF32_LEN];
        uint8   iSrc;
        char    src[SYS_BUF32_LEN];
        uint8   iPri;
        char    pri[SYS_BUF16_LEN];
        uint32  powerVal;
    } power;

    struct {
        uint8   valid;
        uint16  iCapa;
        char    capa[SYS_BUF256_LEN];
        uint16  iCapaCurrent;
        uint8   iDevType;
        char    devType[SYS_BUF32_LEN];
    } medCapa;
    struct {
        uint8   valid;
        uint8   iType;
        char    type[SYS_BUF32_LEN];
        uint8   isUnknown;
        char    flag[SYS_BUF32_LEN];
        uint8   isTagged;
        char    vlanTag[SYS_BUF32_LEN];
        sys_vid_t   vid;
        uint8   l2Pri;
        uint16  dscpPri;
    } networkPolicy[MAX_LLDP_MED_NETWORK_POLICY_RX];
    lldp_med_inventory_tlv_t hardware_v;
    lldp_med_inventory_tlv_t firmware_v;
    lldp_med_inventory_tlv_t software_v;
    lldp_med_inventory_tlv_t serial_num;
    lldp_med_inventory_tlv_t manufacturer;
    lldp_med_inventory_tlv_t model_name;
    lldp_med_inventory_tlv_t asset;

    struct {
        uint8   valid;
        uint8   iData[CAPA_LLDP_MED_COORDINATE_LEN+1];
        char    data[SYS_BUF64_LEN];
    } locationCoor;
    struct {
        uint8   valid;
        uint8   iData[BOND_LLDP_MED_CIVIC_LEN_MAX +1];
        char    data[SYS_BUF512_LEN];
    } locationCivic;
    struct {
        uint8   valid;
        uint8   iData[BOND_LLDP_MED_ELIN_LEN_MAX+1];
        char    data[SYS_BUF128_LEN];
    } locationEcs;
} display_med_entry_t;
#endif 

typedef struct tx_tlvh_s
{
    uint16    type:7                __attribute__((packed)) ;
    uint16    tlv_length:9          __attribute__((packed)) ;
} tx_tlvh_t;
#define HTONS(val) (val)
#define HTONL(val) (val)

#define NTOHS(val) (val)
#define READ16(ptr) ((*(ptr) << 8) | (*(ptr + 1)))
#define READ24(ptr) ((*(ptr) << 16) | (*(ptr + 1) << 8) | *(ptr + 2))
#define READ32(ptr) ((*(ptr) << 24) | (*(ptr + 1) << 16) | (*(ptr + 2) << 8) | *(ptr + 3))

#define WRITE32(ptr, val)               \
{                                       \
    *(ptr) = ((val) & 0xff000000) >> 24;   \
    *(ptr + 1) = ((val) & 0x00ff0000) >> 16;   \
    *(ptr + 2) = ((val) & 0x0000ff00) >> 8;   \
    *(ptr + 3) = (val) & 0x000000ff;\
}

#define WRITE24(ptr, val)               \
{                                       \
    *(ptr + 0) = ((val) & 0xff0000) >> 16;   \
    *(ptr + 1) = ((val) & 0x00ff00) >> 8;   \
    *(ptr + 2) = (val) & 0x0000ff;\
}

#define WRITE16(ptr, val)           \
{                                   \
    *(ptr) = ((val) & 0xff00) >> 8; \
    *(ptr + 1) = ((val) & 0x00ff);\
}
extern int32 lldp_globalStatistics_clear(void);
extern int32 lldp_portStatistics_clear(sys_logic_port_t port);
extern int32 lldp_statistic_get(lldp_statistic_t *pLldp_statistic);
extern int32 lldp_portEntry_get(sys_logic_port_t logicPort ,lldp_port_entry_t *pEntryInfo);
extern int32 lldp_enable_set(sys_enable_t enable);
extern int32 lldp_enable_get(sys_enable_t *pEnable);
extern int32 lldp_port_set(sys_logic_port_t port, uint8 lldpPortState);
extern int32 lldp_port_get(sys_logic_port_t port, uint8 *pLldpPortState);
extern int32 lldp_tx_interval_set(uint32 my_tx_interval);
extern int32 lldp_holdtime_set(uint8 my_holdtime);
extern int32 lldp_reinit_delay_set(uint8 my_reinit_delay);
extern int32 lldp_tlvChanged_notify(uint8 isAllPort, sys_logic_port_t port, lldp_tlv_notify_type_t type);

#ifdef CONFIG_SYS_PROTO_LLDP_MED

extern int32 lldp_medInventory_set(lldp_inventory_type_t inventoryType, char *strData);
#endif 
extern int32 lldp_portLocalEntryInfo_get(sys_logic_port_t port, display_entry_t *pEntry);

#ifdef CONFIG_SYS_PROTO_LLDP_MED

extern int32 lldp_portMedLocalEntryInfo_get(sys_logic_port_t port, display_med_entry_t *pEntry);
#endif 
extern int32 lldp_portRemoteEntryInfo_get(sys_logic_port_t port, uint8 index ,display_entry_t *pEntry);

#ifdef CONFIG_SYS_PROTO_LLDP_MED

extern int32 lldp_portMedRemoteEntryInfo_get(sys_logic_port_t port, uint8 index ,display_med_entry_t *pEntry);
#endif 
extern int32 lldp_portRemoteVlanNameTlv_get(sys_logic_port_t port, uint16 index, lldp_vlanName_tlv_t  *pVlanTlv);
extern int32 lldp_portRemoteEntry_clear(sys_logic_port_t port, uint8 index);
extern int32 lldp_portTlvOverload_get(sys_logic_port_t port, lldp_overLoading_t *pOverload);
extern int32 lldp_disableAction_set(lldp_disable_action_t action);
extern int32 lldp_disableAction_get(lldp_disable_action_t *pAction);
extern int32 lldp_tx_delay_set(uint16 txDelay);
extern int32 lldp_tx_delay_get(uint16 *pTxDelay);

#ifdef CONFIG_SYS_PROTO_LLDP_MED

extern int32 lldp_medFastRepeatCount_set(uint8 fastRepeatCount);
extern int32 lldp_medFastRepeatCount_get(uint8 *pFastRepeatCount);
#endif 
extern int32 lldp_portTlvPvidEnable_set(sys_logic_port_t port, sys_enable_t pvidEnable);
extern int32 lldp_portTlvPvidEnable_get(sys_logic_port_t port, sys_enable_t *pPvidEnable);
extern int32 lldp_portTlvVlanmask_set(sys_logic_port_t port, sys_vlanmask_t  vlanmask);
extern int32 lldp_portTlvVlanmask_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask);
extern int32 lldp_portTlvSel_set(sys_logic_port_t port, lldp_tlv_sel_t lldpTlvSel);
extern int32 lldp_portTlvSel_get(sys_logic_port_t port, lldp_tlv_sel_t *pLldpTlvSel);

#ifdef CONFIG_SYS_PROTO_LLDP_MED

extern int32 lldp_medNetworkPolicy_set(uint32 idx, lldp_med_networkPolicy_t policy);
extern int32 lldp_medNetworkPolicy_get(uint32 idx, lldp_med_networkPolicy_t *pPolicy);
extern int32 lldp_portMedTlvSel_set(sys_logic_port_t port, lldp_med_tlv_sel_t lldpMedTlvSel);
extern int32 lldp_portMedTlvSel_get(sys_logic_port_t port, lldp_med_tlv_sel_t *pLldpMedTlvSel);
extern int32 lldp_medNetworkPolicyAuto_set(sys_enable_t enable);
extern int32 lldp_medNetworkPolicyAuto_get(sys_enable_t *pEnable);
extern int32 lldp_portMedEnable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 lldp_portMedEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 lldp_portMedNetworkPolicyBind_set(sys_logic_port_t port, lldp_med_networkPolicy_bind_t policyBind);
extern int32 lldp_portMedNetworkPolicyBind_get(sys_logic_port_t port, lldp_med_networkPolicy_bind_t *pPolicyBind);
extern int32 lldp_portMedLocation_set(sys_logic_port_t port, lldp_med_location_t location);
extern int32 lldp_portMedLocation_get(sys_logic_port_t port, lldp_med_location_t *pLocation);
#endif 
extern int32 lldp_remoteTableChangeTime_get(uint32 *pChangeTime);
extern int32 lldp_init(void);
extern int32 lldp_exit(void);
#endif 

