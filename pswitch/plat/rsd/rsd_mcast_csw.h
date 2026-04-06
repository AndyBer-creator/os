
#ifndef __RSD_MCAST_CSW_H__
#define __RSD_MCAST_CSW_H__

#define SYS_MCAST_CSW_DFT_VLAN_PROFILE          (0)

extern int32 rsd_mcast_csw_vlanProfile_mode_set(multicast_ipType_t ipType, igmp_lookup_mode_t mode);

extern int32 rsd_mcast_csw_vlanProfile_action_set(multicast_ipType_t ipType, sys_action_t action);

extern int32 rsd_mcast_csw_vlanProfile_action_get(multicast_ipType_t ipType, sys_action_t *pAction);

#endif

