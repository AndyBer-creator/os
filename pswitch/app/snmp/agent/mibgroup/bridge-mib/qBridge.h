

#ifndef QBRIDGE_H
#define QBRIDGE_H


#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>


#include <libsal/sal_vlan.h>
#ifdef CONFIG_SYS_PROTO_GVRP
#include <libsal/sal_gvrp.h>
#endif
#include <libsal/sal_igmp.h>
#include <rsd/rsd_l2.h>
#include <libsal/sal_qos.h>

#include "mib_util.h"


/*dot1dTpFdbStatus*/
#define DOT1D_STATUS_OTHERS     1
#define DOT1D_STATUS_INVALID    2
#define DOT1D_STATUS_LEARNED    3
#define DOT1D_STATUS_SELF      4
#define DOT1D_STATUS_MGMT     5

#define DOT1D_ADDR_TABLE_UPDATA_TIMER  2
#define RECREATE_TABLE_UPDATA_TIMER  (1)
#define SEC_TO_TENMSEC  100

#define SYS_USER_PORTMASK_LEN (SYS_USERPORT_NUM_MAX/8)


typedef struct fdb_entry_s{
    uint32  vlanId;
    uint32  dynCnt;
    struct fdb_entry_s *next;
}fdb_entry_t;

typedef struct tpfdb_entry_s{
    uint32  vlanId;
    uint32    port;
    uint8    mac[6];
    uint8    status;
    struct tpfdb_entry_s *next;
}tpfdb_entry_t;

typedef struct tpgroup_entry_s{
    uint32              vlanId;
    uint8                mac[6];
    uint8                reserver[2];
    sys_logic_portmask_t  stMbr;
    sys_logic_portmask_t  learnMbr;
    sys_logic_portmask_t  mbr;

    sys_user_portmask_t  upmsk_st;
    sys_user_portmask_t  upmsk_dyn;
    sys_user_portmask_t  upmsk_mbr;

    struct tpgroup_entry_s *next;
}tpgroup_entry_t;

typedef struct fwdAll_entry_s{
    uint32              vlanId;
    sys_logic_portmask_t  mbr;
    sys_logic_portmask_t  stMbr;
    sys_logic_portmask_t  fbMbr;

    sys_user_portmask_t upmsk_mbr;
    sys_user_portmask_t upmsk_stmbr;
    sys_user_portmask_t upmsk_fbmbr;

    struct fwdAll_entry_s *next;
}fwdAll_entry_t;

typedef struct vlan_cuurent_entry_s{
    uint32 timeMark;
    uint32 vlanId;
    uint32 fdbId;
    uint32 createTime;
    sys_logic_portmask_t egressMbr;
    sys_logic_portmask_t untagMbr;

    sys_user_portmask_t  upmsk_mbr;
    sys_user_portmask_t  upmsk_untagmbr;
    uint8 status;
    struct vlan_cuurent_entry_s *next;
}vlan_cuurent_entry_t;


typedef struct vlan_static_entry_s{
    uint32 vlanId;
    sys_logic_portmask_t egressMbr;
    sys_logic_portmask_t untagMbr;
    sys_logic_portmask_t forbidMbr;

    sys_user_portmask_t  upmsk_mbr;
    sys_user_portmask_t  upmsk_untagmbr;
    sys_user_portmask_t  upmsk_fbmbr;

    char  name[CAPA_NAME_STR_LEN+1];
    uint8 rowStatus;
    struct vlan_static_entry_s *next;
}vlan_static_entry_t;

typedef struct portVlan_entry_s{
    uint32  lp;
    uint32  userPort;
    uint32 pvid;
    uint32 frameType;
    uint32 filter;
    uint32 gvrpStatus;
    uint32 gvrpErrStats;
    uint32 portRegVlan;
    uint8 srcMac[6];
    struct portVlan_entry_s *next;
}portVlan_entry_t;

/*fdb*/
extern int32 qBridge_fdb_creatEntry(fdb_entry_t *pEntry);
extern fdb_entry_t *qbridge_fdb_getFirst(void);
extern fdb_entry_t *qbridge_fdb_getNext(fdb_entry_t *preEntry);

extern int32 qBridge_vlanMacNum_get(sys_vlanmask_t *vmlist);
/*tpfdbTable*/
extern tpfdb_entry_t *qbridge_tpfdb_getFirst(void);
extern tpfdb_entry_t *qbridge_tpfdb_getNext(tpfdb_entry_t *preEntry);
extern int32 qbridge_tpfdb_entry_get(uint32 *pEntryNum, uint8 isRmSameMac);
extern void qbridge_tpfdb_scanAccessInit(void);
extern tpfdb_entry_t *qbridge_tpfdb_scanFirst(void);
extern tpfdb_entry_t *qbridge_tpfdb_scanAccessNext(void);
extern tpfdb_entry_t *qbridge_getTpfdbEntry(uint32 vlanId, char *pMac);
extern tpfdb_entry_t *qbridge_getTpfdbEntry_byMac(char *pMac);

/*dot1qTpGroupTable*/
extern tpgroup_entry_t *qbridge_group_getFirst(void);
extern tpgroup_entry_t *qbridge_group_getNext(tpgroup_entry_t *preEntry);
extern uint8 is_groupExist(uint32 vlanId, char *pMac);
extern int32 qbridge_group_entry_get(uint32 *pEntryNum);
extern void qbridge_group_scanAccessInit(void);
extern tpgroup_entry_t *qbridge_group_scanFirst(void);
extern tpgroup_entry_t *qbridge_group_scanAccessNext(void);
extern tpgroup_entry_t *qbridge_getGroupEntry(uint32 vlanId, char *pMac);

/*dot1qStaticUnicastTable*/
extern tpfdb_entry_t *qbridge_staticUcast_getFirst(void);
extern tpfdb_entry_t *qbridge_staticUcast_getNext(tpfdb_entry_t *preEntry);
extern void qbridge_staticUcast_scanAccessInit(void);
extern tpfdb_entry_t *qbridge_staticUcast_scanFirst(void);
extern tpfdb_entry_t *qbridge_staticUcast_scanAccessNext(void);
extern tpfdb_entry_t *qbridge_getStaticUcastEntry(uint32 vlanId, char *pMac);
extern int32 qbridge_staticUnicast_entry_get(uint32 *pEntryNum);

/*dot1qStaticMulticastTable*/
extern tpgroup_entry_t *qbridge_stGroup_getFirst(void);
extern tpgroup_entry_t *qbridge_stGroup_getNext(tpgroup_entry_t *preEntry);
extern int32 qBridge_stGroup_creatEntry(tpgroup_entry_t *pEntry);
extern void qBridge_stGroup_removeAll(void);
extern void qbridge_stGroup_scanAccessInit(void);
extern tpgroup_entry_t *qbridge_stGroup_scanFirst(void);
extern tpgroup_entry_t *qbridge_stGroup_scanAccessNext(void);
extern tpgroup_entry_t *qbridge_getStGroupEntry(uint32 vlanId, char *pMac);
extern uint8 is_stGroupExist(uint32 vlanId, char *pMac);
extern int32 qbridge_stGroup_entry_get(uint32 *pEntryNum);

/*dot1qVlanCurrentTable*/
extern vlan_cuurent_entry_t *qbridge_curVlan_getFirst(void);
extern vlan_cuurent_entry_t *qbridge_curVlan_getNext(vlan_cuurent_entry_t *preEntry);
extern void qbridge_curVlan_scanAccessInit(void);
extern vlan_cuurent_entry_t *qbridge_curVlan_scanFirst(void);
extern vlan_cuurent_entry_t *qbridge_curVlan_scanAccessNext(void);
extern vlan_cuurent_entry_t *qbridge_getCurVlanEntry(uint32 vlanId, uint32 timeMark);
extern int32 qbridge_curVlan_entry_get(uint32 *pEntryNum);


/*dot1qVlanStaticTable*/
extern uint8 is_stGroupExist(uint32 vlanId, char *pMac);
extern void qbridge_stVlan_scanAccessInit(void);
extern vlan_static_entry_t *qbridge_stVlan_scanFirst(void);
extern vlan_static_entry_t *qbridge_stVlan_scanAccessNext(void);
extern vlan_static_entry_t *qbridge_getStVlanEntry(uint32 vlanId);
extern int32 qbridge_stVlan_entry_get(uint32 *pEntryNum);


//dot1qForwardAllTable
extern fwdAll_entry_t *qbridge_fwdAll_getFirst(void);
extern fwdAll_entry_t *qbridge_fwdAll_getNext(fwdAll_entry_t *preEntry);
extern int32 qBridge_fwdAll_creatEntry(fwdAll_entry_t *pEntry);
extern void qBridge_fwdAll_removeAll(void);
extern int32 qBridge_fwdAll_get(void);

// dot1qPortVlanTable
extern portVlan_entry_t *qbridge_portVlan_getFirst(void);
extern portVlan_entry_t *qbridge_portVlan_getNext(portVlan_entry_t *preEntry);
extern void qBridge_portVlan_removeAll(void);
extern int32 qBridge_portVlan_creatEntry(portVlan_entry_t *pEntry);
extern int32 qBridge_portVlan_get(void);
#endif


