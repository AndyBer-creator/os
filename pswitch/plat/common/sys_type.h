#ifndef __COMMON_SYS_TYPE_H__
#define __COMMON_SYS_TYPE_H__
#include <common/rt_type.h>
#include <common/swdef/sys_swdef.h>
#include <rtk/trap.h>
#include <rtk/led.h>

#define SYS_BITS_PER_BYTE       (8)
#define SYS_BITS_PER_LONG       (32)
#define SYS_BITS_TO_LONGS(bits) (((bits) + SYS_BITS_PER_LONG - 1) / SYS_BITS_PER_LONG)
#define SYS_STR_CONST(text)         SYS_STR_CONST_PREPROCESS(text)
#define SYS_STR_CONST_PREPROCESS(s) #s

#define SYS_DEV_NUM_MAX         (1) 
#define SYS_PORT_NUM_MAX        (RTK_MAX_PORT_PER_UNIT)
#define SYS_LOGICPORT_NUM_MAX   ((SYS_DEV_NUM_MAX * SYS_PORT_NUM_MAX) + CAPA_LAG_NUM_MAX)
#define SYS_NORMALPORT_NUM_MAX  ((SYS_DEV_NUM_MAX * SYS_PORT_NUM_MAX))
#define SYS_USERPORT_NUM_MAX    (1024) 
#define SYS_VLAN_NUM_MAX        (4096)
#define SNAPOUI_LEN             (3)

typedef uint32 sys_ipv4_t;      
typedef uint32 sys_ipv6_prefix_t;      
typedef uint32 sys_vid_t;       
typedef uint32 sys_fid_t;       
typedef uint32 sys_stg_t;       
typedef uint32 sys_pri_t;       
typedef uint32 sys_qid_t;       
typedef uint32 sys_pieid_t;     
typedef int32  sys_nodeid_t;    
typedef uint32 sys_gid_t;         
typedef uint32 sys_dbofs_t;       
typedef enum sys_init_state_e
{
    SYS_INIT_NOT_COMPLETED = 0,
    SYS_INIT_COMPLETED,
    SYS_INIT_STATE_END
} sys_init_state_t;

typedef struct sys_namestr_s
{
    char str[CAPA_NAME_STR_LEN + 1];
} sys_namestr_t;

typedef struct sys_filenamestr_s
{
    char str[CAPA_FILE_NAME_LEN + 1];
} sys_filenamestr_t;

typedef enum sys_vlan_type_e
{
    SYS_VLAN_ALL = 0,
    SYS_VLAN_STATIC,
    SYS_VLAN_DYNAMIC,
    SYS_VLAN_TYPE_END
} sys_vlan_type_t;

typedef enum sys_vlan_entryType_e
{
    SYS_VLAN_ENTRY_TYPE_NONE = 0,
    SYS_VLAN_ENTRY_TYPE_DEFAULT,
    SYS_VLAN_ENTRY_TYPE_STATIC,
    SYS_VLAN_ENTRY_TYPE_DYNAMIC,
    SYS_VLAN_ENTRY_TYPE_END
} sys_vlan_entryType_t;

typedef enum sys_vlan_portMode_e
{
    SYS_VLAN_PORT_MODE_GENERAL = 0,         
    SYS_VLAN_PORT_MODE_ACCESS,              
    SYS_VLAN_PORT_MODE_TRUNK,               
    SYS_VLAN_PORT_MODE_DOT1QTUNNEL,         
    SYS_VLAN_PORT_MODE_END
} sys_vlan_portMode_t;

typedef enum sys_vlan_membership_e
{
    SYS_VLAN_EXCLUDED = 0,
    SYS_VLAN_FORBIDDEN,
    SYS_VLAN_TAGGED,
    SYS_VLAN_UNTAGGED,
    SYS_VLAN_MEMBERSHIP_END
} sys_vlan_membership_t;

typedef struct sys_portStr_s
{
    char str[CAPA_PORT_STR_LEN];
} sys_portStr_t;

typedef struct sys_text_s
{
    char text[64];
} sys_text_t;

typedef struct sys_ipv4Str_s
{
    char str[CAPA_IPV4_STR_LEN];
} sys_ipv4Str_t;

typedef struct sys_ipv6Str_s
{
    char str[CAPA_IPV6_STR_LEN];
} sys_ipv6Str_t;

typedef struct sys_ipv6_s
{
    uint8 hex[CAPA_IPV6_BYTE_NUM];
} sys_ipv6_t;

typedef rtk_mac_t sys_mac_t;
typedef rtk_enable_t sys_enable_t;
typedef rtk_portmask_t sys_portmask_t;
typedef rtk_port_fiber_media_t sys_port_fiber_media_t;
typedef rtk_port_media_t sys_port_media_t;
typedef rtk_action_t sys_action_t;
typedef rtk_trap_bypassStpType_t sys_trap_bypassStpType_t;
typedef rtk_trap_bypassVlanType_t sys_trap_bypassVlanType_t;
typedef rtk_led_swCtrl_mode_t  sys_led_swCtrl_mode_t;

typedef enum sys_pkt_action_e
{
    SYS_PKT_ACTION_FORWARD              = ACTION_FORWARD,
    SYS_PKT_ACTION_DROP                 = ACTION_DROP,
    SYS_PKT_ACTION_TRAP2CPU             = ACTION_TRAP2CPU,
    SYS_PKT_ACTION_COPY2CPU             = ACTION_COPY2CPU,
    SYS_PKT_ACTION_FLOOD_IN_VLAN        = ACTION_FLOOD_IN_VLAN,
    SYS_PKT_ACTION_WITHDRAW_DROP,

    SYS_PKT_ACTION_REDIRECT             = 16,
    SYS_PKT_ACTION_MIRROR,
    SYS_PKT_ACTION_RATELIMIT,
    SYS_PKT_ACTION_PRIORITY,
    SYS_PKT_ACTION_LOG,
    SYS_PKT_ACTION_ASCVID,
    SYS_PKT_ACTION_ASSVID,
    SYS_PKT_ACTION_TAGCVID,
    SYS_PKT_ACTION_TAGSVID,
    SYS_PKT_ACTION_UNTAGCVID,
    SYS_PKT_ACTION_UNTAGSVID,
    SYS_PKT_ACTION_DSCPREMARK,
    SYS_PKT_ACTION_POLICER,
    SYS_PKT_ACTION_SHUTDOWN_PORT,
    SYS_PKT_ACTION_HIT_INDICATION,
    SYS_PKT_ACTION_OUTTER_PRI_REMARK,
    SYS_PKT_ACTION_END,
} sys_pkt_action_t;

typedef enum sys_callerType_e
{
    SYS_CALLER_USER = 0,
    SYS_CALLER_PROTO,
    SYS_CALLER_END
} sys_callerType_t;

#if 0
typedef struct sys_user_port_s
{
    uint32 unit;
    uint32 port;
} sys_user_port_t;
#endif

typedef uint32 sys_user_port_t;
typedef uint32 sys_logic_port_t;

typedef struct sys_physic_port_s
{
    uint32 dev;
    uint32 port;
} sys_physic_port_t;

typedef struct sys_user_portmask_s
{
    uint32 bits[SYS_BITS_TO_LONGS(SYS_USERPORT_NUM_MAX)];
} sys_user_portmask_t;

typedef struct sys_logic_portmask_s
{
    uint32 bits[SYS_BITS_TO_LONGS(SYS_LOGICPORT_NUM_MAX)];
} sys_logic_portmask_t;

typedef struct sys_physic_portmask_s
{
    sys_portmask_t portmask[SYS_DEV_NUM_MAX];
} sys_physic_portmask_t;
#endif 

