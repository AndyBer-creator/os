
#ifndef __CFG_VLAN_H__
#define __CFG_VLAN_H__

#include <common/sys_list.h>
#include <libsal/sal_vlan.h>

#define DFLT_VLAN_DATABASE_KEY  SYS_LIST_KEY_ID

typedef struct cfg_vlan_s
{
    
    uint32 maxAdminVlanNum;
    sys_enable_t isChkVlanPortMode;

    sys_list_array_t vlanlist;
    sys_vlan_cfgEntry_t cfgEntry[CAPA_VLAN_NUM];

    struct
    {
        struct
        {
            sys_vlan_portMode_t mode;
            sys_enable_t uplink;
            sys_vlanmask_t vmTrunkAllowed;
        } port[SYS_LOGICPORT_NUM_MAX];
    } adminVlan;

    struct
    {
        sys_vid_t vid;
    } defaultVlan;

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    struct
    {
        sys_vid_t vid;
    } mgmtVlan;
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    struct
    {
        sys_vid_t vid;
    } guestVlan;
#endif
} cfg_vlan_t;

extern int32 cfg_vlan_list_init(sys_list_key_t key);

extern int32 cfg_vlan_list_destroy(void);

extern int32 cfg_vlan_list_add(sys_vid_t vid, sys_namestr_t *pName);

extern int32 cfg_vlan_list_update(sys_vid_t vid, sys_vlan_cfgEntry_t *pEntry);

extern int32 cfg_vlan_list_del(sys_vid_t vid);

extern int32 cfg_vlan_list_vidExist_ret(sys_vid_t vid);

extern int32 cfg_vlan_list_get(sys_vid_t vid, sys_vlan_cfgEntry_t *pEntry);

extern int32 cfg_vlan_list_sortId_get(sys_nodeid_t idx, sys_vlan_cfgEntry_t *pEntry);

extern int32 cfg_vlan_listPtr_ret(void);

extern int32 cfg_vlan_listNameExist_ret(sys_namestr_t *pName);

extern int32 cfg_vlan_vidByName_get(sys_namestr_t *pName, sys_vid_t *pVid);

extern int32 cfg_vlan_init(void);

extern int32 cfg_vlan_defaultVid_set(sys_vid_t vid);

extern int32 cfg_vlan_defaultVid_get(sys_vid_t *pVid);

extern int32 cfg_vlan_portAdminMode_set(sys_logic_port_t port, sys_vlan_portMode_t mode);

extern int32 cfg_vlan_portAdminMode_get(sys_logic_port_t port, sys_vlan_portMode_t *pMode);

extern int32 cfg_vlan_portAdminRoleUplinkEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_vlan_portAdminRoleUplinkEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_vlan_portTrunkAllowedVlan_set(sys_logic_port_t port, sys_vlanmask_t *pVlanmask);

extern int32 cfg_vlan_portTrunkAllowedVlan_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask);

extern int32 cfg_vlan_adminVlanMaxEntryNum_get(uint32 *pMaxEntryNum);

extern int32 cfg_vlan_adminVlanMaxEntryNum_set(uint32 maxEntryNum);

extern int32 cfg_vlan_isChkVlanPortMode_set(sys_enable_t enable);

extern int32 cfg_vlan_isChkVlanPortMode_get(sys_enable_t *pEnable);

#ifdef CONFIG_SYS_MANAGEMENT_VLAN

extern int32 cfg_vlan_mgmtVid_set(sys_vid_t vid);

extern int32 cfg_vlan_mgmtVid_get(sys_vid_t *pVid);
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

extern int32 cfg_vlan_guestVid_set(sys_vid_t vid);

extern int32 cfg_vlan_guestVid_get(sys_vid_t *pVid);
#endif
#endif

