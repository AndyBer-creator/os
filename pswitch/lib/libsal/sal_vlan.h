
#ifndef __SAL_VLAN_H__
#define __SAL_VLAN_H__

#include <rsd/rsd_vlan.h>

typedef enum sys_vlan_category_e
{
    SYS_VLAN_CATE_DEFAULT = 0,
#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    SYS_VLAN_CATE_MGMT,
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_VLAN_CATE_GUEST,
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    SYS_VLAN_CATE_DYNAMIC,
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    SYS_VLAN_CATE_VOICE_MANUAL,
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    SYS_VLAN_CATE_SURVEILLANCE,
#endif
    SYS_VLAN_CATE_END
} sys_vlan_category_t;

extern const sys_text_t text_vlan_portMode[];

#define SYS_VLAN_OPER_ENTRY_NUM                 \
    (sal_vlan_operEntryNum_ret())

#define SYS_VLAN_STATIC_ENTRY_NUM               \
    (sal_vlan_adminEntryNum_ret())

#define FOR_EACH_VLAN_STATIC_ENTRY_IDX(idx)     \
    for((idx) = 0; (idx) < SYS_VLAN_STATIC_ENTRY_NUM; (idx)++)

#define IS_VLAN_OPER_ENTRY_EXIST(vid)           \
    (sal_vlan_operEntryExist_ret(vid))

#define IS_STATIC_VLAN_EXIST(vid)               \
    (sal_vlan_adminEntryExist_ret(vid))

#define IS_DYNAMIC_VLAN_EXIST(vid)              \
    (IS_VLAN_OPER_ENTRY_EXIST(vid) && !IS_STATIC_VLAN_EXIST(vid))

extern int32 sal_vlan_exclusive_chk(sys_vlan_category_t subject, sys_vid_t vid);

extern int32 sal_vlan_nameStr_get(sys_vid_t vid, char *pPrefix, sys_namestr_t *pName);

extern int32 sal_vlan_name_set(sys_vid_t vid, sys_namestr_t *pName);

extern int32 sal_vlan_vidByName_get(sys_namestr_t *pName, sys_vid_t *pVid);

extern int32 sal_vlan_adminEntryNum_ret(void);

extern int32 sal_vlan_adminEntryExist_get(sys_vlanmask_t *pVlanmask);

extern int32 sal_vlan_adminEntryExist_ret(sys_vid_t vid);

extern int32 sal_vlan_adminEntry_create(sys_vid_t vid, sys_namestr_t *pName);

extern int32 sal_vlan_adminEntry_destroy(sys_vid_t vid);

extern int32 sal_vlan_adminEntry_set(sys_vid_t vid, sys_vlan_adminEntry_t *pEntry);

extern int32 sal_vlan_adminEntry_get(sys_vid_t vid, sys_vlan_adminEntry_t *pEntry);

extern int32 sal_vlan_adminEntrySort_get(uint32 idx, sys_vid_t *pVid, sys_vlan_adminEntry_t *pEntry);

extern int32 sal_vlan_defaultVid_set(sys_vid_t vid);

extern int32 sal_vlan_defaultVid_get(sys_vid_t *pVid);

extern int32 sal_vlan_nameExist_ret(sys_namestr_t *pName);

extern int32 sal_vlan_name_get(sys_vid_t vid, sys_namestr_t *pName);

extern int32 sal_vlan_operEntryNum_get(uint32 *pNum);

extern int32 sal_vlan_operEntryNum_ret(void);

extern int32 sal_vlan_operEntryExist_get(sys_vlanmask_t *pVlanmask);

extern int32 sal_vlan_operEntryExist_ret(sys_vid_t vid);

extern int32 sal_vlan_operEntry_get(sys_vid_t vid, sys_vlan_operEntry_t *pEntry);

extern int32 sal_vlan_operEntrySort_get(uint32 idx, sys_vid_t *pVid, sys_vlan_operEntry_t *pEntry);

extern int32 sal_vlan_portAdminMode_set(sys_logic_port_t port, sys_vlan_portMode_t mode);

extern int32 sal_vlan_portAdminMode_get(sys_logic_port_t port, sys_vlan_portMode_t *pMode);

extern int32 sal_vlan_portAdminMode_chk(sys_logic_port_t port, uint32 allowMask);

extern int32 sal_vlan_portAdminRoleUplinkEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_vlan_portAdminRoleUplinkEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_vlan_portAdminAcceptFrameType_set(sys_logic_port_t port, sys_vlan_frameType_t type);

extern int32 sal_vlan_portAdminAcceptFrameType_get(sys_logic_port_t port, sys_vlan_frameType_t *pType);

extern int32 sal_vlan_portAdminIgrFilterEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_vlan_portAdminIgrFilterEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_vlan_portAdminPvid_set(sys_logic_port_t port, sys_vid_t pvid);

extern int32 sal_vlan_portAdminPvid_get(sys_logic_port_t port, sys_vid_t *pPvid);

extern int32 sal_vlan_portAdminVlanmask_add(sys_logic_port_t port, sys_vlan_membership_t membership, sys_vlanmask_t *pVlanmask);

extern int32 sal_vlan_portAdminVlanmask_del(sys_logic_port_t port, sys_vlan_membership_t membership, sys_vlanmask_t *pVlanmask);

extern int32 sal_vlan_portAdminVlanmask_get(sys_logic_port_t port, sys_vlan_membership_t membership, sys_vlanmask_t *pVlanmask);

extern int32 sal_vlan_portAdminVlanMember_get(sys_logic_port_t port, sys_vlan_portMember_t *pVlanMember);

extern int32 sal_vlan_portAdminVlanMembership_set(sys_logic_port_t port, sys_vid_t vid, sys_vlan_membership_t membership);

extern int32 sal_vlan_portAdminVlanMembership_get(sys_logic_port_t port, sys_vid_t vid, sys_vlan_membership_t *pMembership);

extern int32 sal_vlan_portTrunkAllowedVlan_set(sys_logic_port_t port, sys_vlanmask_t *pVlanmask);

extern int32 sal_vlan_portTrunkAllowedVlan_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask);

extern int32 sal_vlan_portOperPvid_get(sys_logic_port_t port, sys_vid_t *pPvid);

extern int32 sal_vlan_portOperVlanMember_get(sys_logic_port_t port, sys_vlan_portMember_t *pVlanMember);

extern int32 sal_vlan_portOperVlanMembership_get(sys_logic_port_t port, sys_vid_t vid, sys_vlan_membership_t *pMembership);

extern int32 sal_vlan_dot1qTunnelTpid_set(uint32 index, uint16 tpid);

extern int32 sal_vlan_dot1qTunnelTpid_get(uint32 index, uint16 *pTpid);

extern int32 sal_vlan_portDot1qTunnelTpid_set(sys_logic_port_t port, uint16 tpid);

extern int32 sal_vlan_portDot1qTunnelTpid_get(sys_logic_port_t port, uint16 *pTpid);

extern int32 sal_vlan_mibDot1qVlanNumDeletes_get(uint32 *pVlanNumDeletes);

extern int32 sal_vlan_mibDot1qVlanCreationTime_get(sys_vid_t vid, uint32 *pTime);

extern int32 sal_vlan_isChkVlanPortMode_set(sys_enable_t enable);

extern int32 sal_vlan_isChkVlanPortMode_get(sys_enable_t *pEnable);

#ifdef CONFIG_SYS_MANAGEMENT_VLAN

extern int32 sal_vlan_mgmtVid_set(sys_vid_t vid);

extern int32 sal_vlan_mgmtVid_get(sys_vid_t *pVid);
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

extern int32 sal_vlan_guestVid_set(sys_vid_t vid);

extern int32 sal_vlan_guestVid_get(sys_vid_t *pVid);
#endif
#endif

