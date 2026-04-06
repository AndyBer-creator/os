
#ifndef __SAL_GVRP_H__
#define __SAL_GVRP_H__

#include <libsal/sal_type.h>
#include <gvrp/proto_gvrp.h>

extern int32 sal_gvrp_enable_set(sys_enable_t enable);

extern int32 sal_gvrp_enable_get(sys_enable_t *pEnable);

extern int32 sal_gvrp_timer_set(gvrp_timer_type_t  timer_type, uint32 timeout);

extern int32 sal_gvrp_timer_get(gvrp_timer_type_t  timer_type, uint32 *pTimeout);

extern int32 sal_gvrp_all_timer_set(uint32 joinTime, uint32 leaveTime, uint32 leaveAllTime);

extern int32 sal_gvrp_port_statistics_get(sys_logic_port_t port,  gvrp_statistics_t  *pStat);

extern int32  sal_gvrp_statistics_clear(sys_logic_port_t port);

extern int32  sal_gvrp_error_statistics_clear(sys_logic_port_t port);

extern int32 sal_gvrp_port_error_statistics_get(sys_logic_port_t port,  gvrp_error_statistics_t  *pStat);

extern int32 sal_gvrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_gvrp_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_gvrp_info_get(sys_gvrp_info_t *pInfo);

extern int32 sal_gvrp_port_regmode_set(sys_logic_port_t port, register_mode_t  regmode);

extern int32 sal_gvrp_port_regmode_get(sys_logic_port_t port, register_mode_t *pRegMode);

extern int32 sal_gvrp_port_forbidVlan_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_gvrp_port_forbidVlan_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_gvrp_vlan_get(sys_vid_t vid, Gvd *vlandb);

extern int32 sal_gvrp_vlan_db_size_get(uint32 *pSize);

extern int32 sal_gvrp_vlanMaskExist_get(sys_vlanmask_t *pVlanMask);

extern int32 sal_gvrp_gidtt_machine_get(sys_logic_port_t port, sys_vid_t vid, Gid_machine *pMachine);

#endif 

