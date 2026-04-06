
#ifndef __SKI_VLAN_H__
#define __SKI_VLAN_H__

#include <ski/ski_core.h>
#include <rsd/rsd_vlan.h>

typedef enum ski_vlan_cmd_e
{
    
    SKI_VLAN_SET_BASE = (SKI_CORE_MOD_VLAN * SKI_CORE_MODCMD_TOTAL),
    SKI_VLAN_DEFAULTVID_SET,
    SKI_VLAN_ADMINENTRY_CREATE,
    SKI_VLAN_ADMINENTRY_DESTROY,
    SKI_VLAN_ADMINENTRY_SET,
    SKI_VLAN_NAME_SET,
    SKI_VLAN_PORTADMINMODE_SET,
    SKI_VLAN_PORTADMINROLEUPLINKENABLE_SET,
    SKI_VLAN_PORTADMINACCEPTFRAMETYPE_SET,
    SKI_VLAN_PORTADMINIGRFILTERENABLE_SET,
    SKI_VLAN_PORTADMINPVID_SET,
    SKI_VLAN_PORTADMINVLANMASK_ADD,
    SKI_VLAN_PORTADMINVLANMASK_DEL,
    SKI_VLAN_PORTTRUNKALLOWEDVLAN_SET,
    SKI_VLAN_DOT1QTUNNELTPID_SET,
    SKI_VLAN_PORTDOT1QTUNNELTPID_SET,
#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    SKI_VLAN_MGMTVID_SET,           
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SKI_VLAN_GUESTVID_SET,          
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    SKI_VLAN_VOICEVID_SET,          
#endif

    SKI_VLAN_GET_BASE = (SKI_VLAN_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_VLAN_DEFAULTVID_GET,
    SKI_VLAN_ADMINENTRYEXIST_GET,
    SKI_VLAN_ADMINENTRY_GET,
    SKI_VLAN_NAME_GET,
    SKI_VLAN_OPERENTRYNUM_GET,
    SKI_VLAN_OPERENTRYEXIST_GET,
    SKI_VLAN_OPERENTRY_GET,
    SKI_VLAN_OPERENTRYSORT_GET,
    SKI_VLAN_PORTADMINMODE_GET,
    SKI_VLAN_PORTADMINROLEUPLINKENABLE_GET,
    SKI_VLAN_PORTADMINACCEPTFRAMETYPE_GET,
    SKI_VLAN_PORTADMINIGRFILTERENABLE_GET,
    SKI_VLAN_PORTADMINPVID_GET,
    SKI_VLAN_PORTADMINVLANMASK_GET,
    SKI_VLAN_PORTADMINVLANMEMBER_GET,
    SKI_VLAN_PORTADMINVLANMEMBERSHIP_GET,
    SKI_VLAN_PORTTRUNKALLOWEDVLAN_GET,
    SKI_VLAN_PORTOPERPVID_GET,
    SKI_VLAN_PORTOPERVLANMEMBER_GET,
    SKI_VLAN_PORTOPERVLANMEMBERSHIP_GET,
    SKI_VLAN_DOT1QTUNNELTPID_GET,
    SKI_VLAN_PORTDOT1QTUNNELTPID_GET,
    SKI_VLAN_MIBDOT1QVLANNUMDELETES_GET,
    SKI_VLAN_MIBDOT1QVLANCREATIONTIME_GET,
#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    SKI_VLAN_MGMTVID_GET,           
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SKI_VLAN_GUESTVID_GET,          
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    SKI_VLAN_VOICEVID_GET,          
    SKI_VLAN_PORTVOICEACTIVE_GET,   
#endif
    SKI_VLAN_ADMIN_VLAN_MAXENTRYNUM_GET,

    SKI_VLAN_CMD_END
} ski_vlan_cmd_t;

typedef union ski_vlan_ioctl_u
{
    uint32 adminVlanMaxEntryNum;

    struct
    {
        sys_vid_t vid;
    } defaultVid;

    struct
    {
        sys_vlanmask_t vmExist;
    } adminEntryExist;

    struct
    {
        sys_vid_t vid;
        sys_namestr_t name;
    } adminEntryCreate;

    struct
    {
        sys_vid_t vid;
    } adminEntryDestroy;

    struct
    {
        sys_vid_t vid;
        sys_vlan_adminEntry_t adminEntry;
    } adminEntry;

    struct
    {
        sys_vid_t vid;
        sys_namestr_t name;
    } name;

    struct
    {
        uint32 num;
    } operEntryNum;

    struct
    {
        sys_vlanmask_t vmExist;
    } operEntryExist;

    struct
    {
        sys_vid_t vid;
        sys_vlan_operEntry_t operEntry;
    } operEntry;

    struct
    {
        uint32 idx;
        sys_vid_t vid;
        sys_vlan_operEntry_t operEntry;
    } operEntrySort;

    struct
    {
        sys_logic_port_t port;
        sys_vlan_portMode_t mode;
    } portAdminMode;

    struct
    {
        sys_logic_port_t port;
        sys_enable_t enable;
    } portAdminRoleUplinkEnable;

    struct
    {
        sys_logic_port_t port;
        sys_vlan_frameType_t type;
    } portAdminAcceptFrameType;

    struct
    {
        sys_logic_port_t port;
        sys_enable_t enable;
    } portAdminIgrFilterEnable;

    struct
    {
        sys_logic_port_t port;
        sys_vid_t pvid;
    } portAdminPvid;

    struct
    {
        sys_logic_port_t port;
        sys_vlan_membership_t membership;
        sys_vlanmask_t vlanmask;
    } portAdminVlanmask;

    struct
    {
        sys_logic_port_t port;
        sys_vlan_portMember_t vlanMember;
    } portAdminVlanMember;

    struct
    {
        sys_logic_port_t port;
        sys_vid_t vid;
        sys_vlan_membership_t membership;
    } portAdminVlanMembership;

    struct
    {
        sys_logic_port_t port;
        sys_vlanmask_t vlanmask;
    } portTrunkAllowedVlan;

    struct
    {
        sys_logic_port_t port;
        sys_vid_t pvid;
    } portOperPvid;

    struct
    {
        sys_logic_port_t port;
        sys_vlan_portMember_t vlanMember;
    } portOperVlanMember;

    struct
    {
        sys_logic_port_t port;
        sys_vid_t vid;
        sys_vlan_membership_t membership;
    } portOperVlanMembership;

    struct
    {
        uint32 index;
        uint16 tpid;
    } dot1qTunnelTpid;

    struct
    {
        sys_logic_port_t port;
        uint16 tpid;
    } portDot1qTunnelTpid;

    struct
    {
        uint32 vlanNumDeletes;
    } mibDot1qVlanNumDeletes;

    struct
    {
        sys_vid_t vid;
        uint32 time;
    } mibDot1qVlanCreationTime;

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    struct
    {
        sys_vid_t vid;
    } mgmtVid;
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    struct
    {
        sys_vid_t vid;
    } guestVid;
#endif

#ifdef CONFIG_SYS_VOICEVLAN
    struct
    {
        sys_vid_t vid;
    } voiceVid;
    struct
    {
        sys_logic_port_t port;
        sys_enable_t enable;
    } portVoiceActive;
#endif
} ski_vlan_ioctl_t;

extern int32 ski_vlan_init(void);

#endif  

