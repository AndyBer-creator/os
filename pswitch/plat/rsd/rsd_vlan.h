
#ifndef __RSD_VLAN_H__
#define __RSD_VLAN_H__

#include <rtk/default.h>
#include <rtk/vlan.h>
#include <common/sys_vlan.h>
#include <rsd/rsd_sys.h>
#include <voice_vlan/proto_voice_vlan.h>

#define QINQ_TPID_NULL_MASK             (0x0)
#define QINQ_TPID_DOT1Q_INDEX           (1)
#define QINQ_TPID_DOT1Q_MASK            (1 << QINQ_TPID_DOT1Q_INDEX)

#define VLAN_PROC_NAME      "vlan"
#define VLAN_PROC_SEQNAME   vlan
#define VLAN_PROC_BUFLEN    (92 * CONFIG_SYS_VLAN_MAX_NUM)

typedef rtk_vlan_ifilter_t sys_vlan_ifilter_t;
typedef rtk_vlan_acceptFrameType_t sys_vlan_frameType_t;

typedef struct sys_vlan_operEntry_s
{
    sys_vlan_entryType_t type;
    sys_namestr_t name;                 
    sys_logic_portmask_t lpmForbidden;  
    sys_logic_portmask_t lpmMember;
    sys_logic_portmask_t lpmUntag;
} sys_vlan_operEntry_t;

typedef struct sys_vlan_adminEntry_s
{
    sys_namestr_t name;
    sys_logic_portmask_t lpmForbidden;
    sys_logic_portmask_t lpmMember;
    sys_logic_portmask_t lpmUntag;
} sys_vlan_adminEntry_t;

typedef struct sys_vlan_cfgEntry_s
{
    sys_vid_t vid;
    sys_namestr_t name;
} sys_vlan_cfgEntry_t;

typedef struct sys_vlan_portMember_s
{
    sys_vlanmask_t vmForbidden;
    sys_vlanmask_t vmMember;
    sys_vlanmask_t vmUntag;
} sys_vlan_portMember_t;

typedef struct sys_vlan_event_s
{
    sys_logic_port_t port;
    sys_vid_t vid;
    sys_vlanmask_t vmChanged;
    sys_logic_portmask_t lpmChanged;
    sys_logic_portmask_t lpmMember;
    sys_logic_portmask_t lpmUntag;
    uint32 tpid;
#ifdef CONFIG_SYS_VOICEVLAN
    sys_vlan_voiceQos_t voiceQos;
#endif
} sys_vlan_event_t;

extern int32 rsd_vlan_chip_init(void);

extern int32 rsd_vlan_chip_vlanEntry_set(sys_vid_t vid, sys_logic_portmask_t member, sys_logic_portmask_t untag);

extern int32 rsd_vlan_chip_vlanEntry_get(sys_vid_t vid, sys_logic_portmask_t *pMember, sys_logic_portmask_t *pUntag);

extern int32 rsd_vlan_chip_portMode_set(sys_logic_port_t port, sys_vlan_portMode_t mode);

extern int32 rsd_vlan_chip_portRoleUplinkEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_vlan_chip_portAcceptFrameType_set(sys_logic_port_t port, sys_vlan_frameType_t type);

extern int32 rsd_vlan_chip_portIgrFilterEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_vlan_chip_portPvid_set(sys_logic_port_t port, sys_vid_t pvid);

extern int32 rsd_vlan_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_vlan_init(void);

extern int32 rsd_vlan_defaultVid_set(sys_vid_t vid);

extern int32 rsd_vlan_defaultVid_get(sys_vid_t *pVid);

extern int32 rsd_vlan_adminEntryExist_get(sys_vlanmask_t *pVlanmask);

extern int32 rsd_vlan_adminEntry_create(sys_vid_t vid, sys_namestr_t *pName);

extern int32 rsd_vlan_adminEntry_destroy(sys_vid_t vid);

extern int32 rsd_vlan_adminEntry_set(sys_vid_t vid, sys_vlan_adminEntry_t *pEntry);

extern int32 rsd_vlan_adminEntry_get(sys_vid_t vid, sys_vlan_adminEntry_t *pEntry);

extern int32 rsd_vlan_name_set(sys_vid_t vid, sys_namestr_t *pName);

extern int32 rsd_vlan_name_get(sys_vid_t vid, sys_namestr_t *pName);

extern int32 rsd_vlan_operEntryNum_get(uint32 *pNum);

extern int32 rsd_vlan_operEntryExist_get(sys_vlanmask_t *pVlanmask);

extern int32 rsd_vlan_operEntry_get(sys_vid_t vid, sys_vlan_operEntry_t *pEntry);

extern int32 rsd_vlan_operEntrySort_get(uint32 idx, sys_vid_t *pVid, sys_vlan_operEntry_t *pEntry);

extern int32 rsd_vlan_portAdminMode_set(sys_logic_port_t port, sys_vlan_portMode_t mode);

extern int32 rsd_vlan_portAdminMode_get(sys_logic_port_t port, sys_vlan_portMode_t *pMode);

extern int32 rsd_vlan_portAdminRoleUplinkEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_vlan_portAdminRoleUplinkEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 rsd_vlan_portAdminAcceptFrameType_set(sys_logic_port_t port, sys_vlan_frameType_t type);

extern int32 rsd_vlan_portAdminAcceptFrameType_get(sys_logic_port_t port, sys_vlan_frameType_t *pType);

extern int32 rsd_vlan_portAdminIgrFilterEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_vlan_portAdminIgrFilterEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 rsd_vlan_portAdminPvid_set(sys_logic_port_t port, sys_vid_t pvid);

extern int32 rsd_vlan_portAdminPvid_get(sys_logic_port_t port, sys_vid_t *pPvid);

extern int32 rsd_vlan_portAdminVlanmask_add(sys_logic_port_t port, sys_vlan_membership_t membership, sys_vlanmask_t *pVlanmask);

extern int32 rsd_vlan_portAdminVlanmask_del(sys_logic_port_t port, sys_vlan_membership_t membership, sys_vlanmask_t *pVlanmask);

extern int32 rsd_vlan_portAdminVlanmask_get(sys_logic_port_t port, sys_vlan_membership_t membership, sys_vlanmask_t *pVlanmask);

extern int32 rsd_vlan_portAdminVlanMember_get(sys_logic_port_t port, sys_vlan_portMember_t *pVlanMember);

extern int32 rsd_vlan_portAdminVlanMembership_get(sys_logic_port_t port, sys_vid_t vid, sys_vlan_membership_t *pMembership);

extern int32 rsd_vlan_portTrunkAllowedVlan_set(sys_logic_port_t port, sys_vlanmask_t *pVlanmask);

extern int32 rsd_vlan_portTrunkAllowedVlan_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask);

extern int32 rsd_vlan_portOperPvid_get(sys_logic_port_t port, sys_vid_t *pPvid);

extern int32 rsd_vlan_portOperVlanMember_get(sys_logic_port_t port, sys_vlan_portMember_t *pVlanMember);

extern int32 rsd_vlan_portOperVlanMembership_get(sys_logic_port_t port, sys_vid_t vid, sys_vlan_membership_t *pMembership);

extern int32 rsd_vlan_chip_dot1qTunnelTpid_set(uint32 index, uint16 tpid);

extern int32 rsd_vlan_chip_dot1qTunnelTpid_get(uint32 index, uint16 *tpid);

extern int32 rsd_vlan_chip_portDot1qTunnelTpid_set(sys_logic_port_t port, uint16 tpid);

extern int32 rsd_vlan_dot1qTunnelTpid_set(uint32 index, uint16 tpid);

extern int32 rsd_vlan_dot1qTunnelTpid_get(uint32 index, uint16 *pTpid);

extern int32 rsd_vlan_portDot1qTunnelTpid_set(sys_logic_port_t port, uint16 tpid);

extern int32 rsd_vlan_portDot1qTunnelTpid_get(sys_logic_port_t port, uint16 *pTpid);

extern int32 rsd_vlan_mibDot1qVlanNumDeletes_get(uint32 *pVlanNumDeletes);

extern int32 rsd_vlan_mibDot1qVlanCreationTime_get(sys_vid_t vid, uint32 *pTime);

extern int32 rsd_vlan_adminVlanMaxEntryNum_get(uint32 *pMaxEntryNum);

#ifdef CONFIG_SYS_PROTO_STP

extern int32 rsd_vlan_stg_init(void);

extern int32 rsd_vlan_stg_set(sys_vid_t vid, sys_stg_t stg);

extern int32 rsd_vlan_stg_get(sys_vid_t vid, sys_stg_t *pStg);
#endif

#ifdef CONFIG_SYS_PROTO_GVRP
extern int32 rsd_vlan_gvrpVlan_create(sys_vid_t vid, sys_namestr_t *pName);
extern int32 rsd_vlan_gvrpVlan_destroy(sys_vid_t vid);
extern int32 rsd_vlan_portGvrpVlan_add(sys_logic_port_t port, sys_vid_t vid);
extern int32 rsd_vlan_portGvrpVlan_del(sys_logic_port_t port, sys_vid_t vid);

#endif

#ifdef CONFIG_SYS_MANAGEMENT_VLAN

extern int32 rsd_vlan_mgmtVid_set(sys_vid_t vid);

extern int32 rsd_vlan_mgmtVid_get(sys_vid_t *pVid);
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

extern int32 rsd_vlan_guestVid_set(sys_vid_t vid);

extern int32 rsd_vlan_guestVid_get(sys_vid_t *pVid);

extern int32 rsd_vlan_portGuestVlanActive_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_vlan_portGuestVlanActive_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 rsd_vlan_portAuthorVlan_set(sys_logic_port_t port, sys_vid_t vid);

extern int32 rsd_vlan_hostAuthorVlan_add(sys_logic_port_t port, sys_vid_t vid);

extern int32 rsd_vlan_hostAuthorVlan_del(sys_logic_port_t port, sys_vid_t vid);
#endif

#ifdef CONFIG_SYS_VOICEVLAN

extern int32 rsd_vlan_voiceVid_set(sys_vid_t vid);

extern int32 rsd_vlan_voiceVid_get(sys_vid_t *pVid);

extern int32 rsd_vlan_portVoiceVlanActive_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_vlan_portVoiceVlanActive_get(sys_logic_port_t port, sys_enable_t *pEnable);
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN

extern int32 rsd_vlan_surveillanceVid_set(sys_vid_t vid);

extern int32 rsd_vlan_portSurveillanceVlanActive_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_vlan_portSurveillanceVlanActive_get(sys_logic_port_t port, sys_enable_t *pEnable);
#endif
#endif

