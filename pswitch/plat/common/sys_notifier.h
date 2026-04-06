#ifndef __COMMON_SYS_NOTIFIER_H__
#define __COMMON_SYS_NOTIFIER_H__
#define SYS_NT_EVENT_ALL   0xffffffff
typedef uint32 sys_notifier_event_mask_t;
typedef int32 (*sys_notifier_event_cb_t)(uint32 event, char *pData);
typedef int32 (*sys_notifier_event_sync_t)(sys_notifier_event_cb_t eventCallback);

#define SYS_NTS_L2      "L2"
#define SYS_NTS_L3      "L3"
#define SYS_NTS_LLDP    "LLDP"
#define SYS_NTS_MIRROR  "MIRROR"
#define SYS_NTS_PORT    "PORT"
#define SYS_NTS_QOS     "QoS"
#define SYS_NTS_SYSTEM  "SYSTEM"
#define SYS_NTS_TRUNK   "TRUNK"
#define SYS_NTS_VLAN    "VLAN"

typedef enum sys_notifier_subject_type_e
{
    
    SYS_NOTIFIER_SUBJECT_L2,
    SYS_NOTIFIER_SUBJECT_L3,
    SYS_NOTIFIER_SUBJECT_LLDP,
    SYS_NOTIFIER_SUBJECT_MIRROR,
    SYS_NOTIFIER_SUBJECT_PORT,
    SYS_NOTIFIER_SUBJECT_QOS,
    SYS_NOTIFIER_SUBJECT_SYSTEM,
    SYS_NOTIFIER_SUBJECT_TRUNK,
    SYS_NOTIFIER_SUBJECT_VLAN,
    SYS_NOTIFIER_SUBJECT_AUTH,
    SYS_NOTIFIER_SUBJECT_ERPS,
    SYS_NOTIFIER_SUBJECT_LOOPBACK,
    SYS_NOTIFIER_SUBJECT_END,
}sys_notifier_subject_type_t;
typedef enum sys_notifier_subject_l2_event_e
{
    SYS_NT_L2_MCAST_NUM_CHG  = (1 << 0),
}sys_notifier_subject_l2_event_t;
typedef enum sys_notifier_subject_l3_event_e
{
    SYS_NT_L3_INTF_ADD  = (1 << 0),
    SYS_NT_L3_INTF_DEL  = (1 << 1),
    SYS_NT_L3_INTF_LINK_CHG  = (1 << 2),
    SYS_NT_L3_INTF_IP_ADD  = (1 << 3),
    SYS_NT_L3_INTF_IP_DEL  = (1 << 4),
    SYS_NT_L3_MGMT_IP_CHG  = (1 << 5),
    SYS_NT_L3_MGMT_IPV6_ADD  = (1 << 6),
    SYS_NT_L3_MGMT_IPV6_DEL  = (1 << 7),
}sys_notifier_subject_l3_event_t;
typedef enum sys_notifier_subject_lldp_event_e
{
    SYS_NT_LLDP_VOICE_VLAN_PORT_ADD  = (1 << 0),
    SYS_NT_LLDP_VOICE_VLAN_PORT_DEL  = (1 << 1),
    SYS_NT_LLDP_TLV_CHG              = (1 << 2),
}sys_notifier_subject_lldp_event_t;
typedef enum sys_notifier_subject_mirror_event_e
{
    SYS_NT_MIRROR_GROUP_CHG     = (1 << 0),
}sys_notifier_subject_mirror_event_t;
typedef enum sys_notifier_subject_port_event_e
{
    SYS_NT_PORT_LINK_CHG        = (1 << 0),
    SYS_NT_PORT_ADMIN_CHG       = (1 << 1),
    SYS_NT_PORT_DESCRIPTION_CHG = (1 << 2),
}sys_notifier_subject_port_event_t;
typedef enum sys_notifier_subject_qos_event_e
{
    SYS_NT_QOS_COS2QUEUE_CHG    = (1 << 0),
}sys_notifier_subject_qos_event_t;
typedef enum sys_notifier_subject_sys_event_e
{
    SYS_NT_SYS_NAME_CHG           = (1 << 0),
    SYS_NT_SYS_DESCRIPTION_CHG    = (1 << 1),
    SYS_NT_SYS_MGMT_IP_CHG        = (1 << 2),
    SYS_NT_SYS_MGMT_IPV6_CHG      = (1 << 3),
    SYS_NT_SYS_PORT_MAX_FRAME_CHG = (1 << 4),
    SYS_NT_SYS_STATUS_CHG         = (1 << 5),
}sys_notifier_subject_sys_event_t;
typedef enum sys_notifier_subject_trunk_event_e
{
    SYS_NT_TRK_ACT_MBR_JOIN     = (1 << 0),
    SYS_NT_TRK_ACT_MBR_LEAVE    = (1 << 1),
    SYS_NT_TRK_MBR_JOIN         = (1 << 2),
    SYS_NT_TRK_MBR_LEAVE        = (1 << 3),
}sys_notifier_subject_trunk_event_t;
typedef enum sys_notifier_subject_vlan_event_e
{
    SYS_NT_VLAN_ADMINVLAN_CREATE        = (1 << 0),             
    SYS_NT_VLAN_ADMINVLAN_DESTROY       = (1 << 1),
    SYS_NT_VLAN_ADMINENTRY_CHG          = (1 << 2),
    SYS_NT_VLAN_OPERVLAN_CREATE         = (1 << 3),             
    SYS_NT_VLAN_OPERVLAN_DESTROY        = (1 << 4),
    SYS_NT_VLAN_OPERPVID_CHG            = (1 << 5),
    SYS_NT_VLAN_OPERENTRY_CHG           = (1 << 6),
    SYS_NT_VLAN_QINQ_TPID_CHG           = (1 << 7),             
    SYS_NT_VLAN_VOICE_VID_CHG           = (1 << 8),             
    SYS_NT_VLAN_VOICE_QOS_CHG           = (1 << 9),            
    SYS_NT_VLAN_MGMT_VID_CHG            = (1 << 10),            
    SYS_NT_VLAN_VLANRANGE_CHG           = (1 << 11),            
}sys_notifier_subject_vlan_event_t;
typedef enum sys_notifier_subject_auth_event_e
{
    SYS_NT_AUTH_UNAUTHLPM_CHG        = (1 << 0),             
}sys_notifier_subject_auth_event_t;
typedef enum sys_notifier_subject_erps_event_e
{
    SYS_NT_ERPS_INST_ADD        = (1 << 0),
    SYS_NT_ERPS_INST_DEL        = (1 << 1),
    SYS_NT_ERPS_PORT_ADD        = (1 << 2),
    SYS_NT_ERPS_PORT_DEL        = (1 << 3),
}sys_notifier_subject_erps_event_t;
typedef enum sys_notifier_subject_loopback_event_e
{
    SYS_NT_LOOPBACK_PORT_ADD        = (1 << 0),
    SYS_NT_LOOPBACK_PORT_DEL        = (1 << 1),
}sys_notifier_subject_loopback_event_t;
typedef enum sys_notifier_priority_e
{
    SYS_NOTIFIER_PRI_LOW,
    SYS_NOTIFIER_PRI_MEDIUM,
    SYS_NOTIFIER_PRI_HIGH,
    SYS_NOTIFIER_PRI_END,
}sys_notifier_priority_t;

typedef struct sys_notifier_event_handler_list_s
{
    char callerName[SYS_BUF32_LEN];
    sys_notifier_priority_t priority;
    sys_notifier_event_mask_t mask;
    sys_notifier_event_cb_t eventCallback;
    struct sys_notifier_event_handler_list_s *next;
} sys_notifier_event_handler_list_t;
extern int32 sys_notifier_subject_register(sys_notifier_subject_type_t type,
                sys_notifier_event_mask_t mask, sys_notifier_event_sync_t syncEvent);
extern int32 sys_notifier_event_dispatcher(sys_notifier_subject_type_t type,
                sys_notifier_event_mask_t event, char *pData);
extern int32 sys_notifier_observer_register(char *callerName, sys_notifier_subject_type_t type,
                sys_notifier_event_mask_t mask, sys_notifier_event_cb_t eventCallback,
                sys_notifier_priority_t priority);
extern int32 sys_notifier_observer_unregister(char *callerName, sys_notifier_subject_type_t type, sys_notifier_event_cb_t eventCallback);
extern int32 sys_notifier_init(void);

#endif 

