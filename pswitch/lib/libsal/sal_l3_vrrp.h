
#ifndef __SAL_L3_VRRP_H__
#define __SAL_L3_VRRP_H__

#include <common/sys_def.h>
#include <libipc/quaggaTask.h>

#define VRRP_SOCK_PATH "vrrp_ipc_"

#define VRRP_OPCODE_ADDIP          1
#define VRRP_OPCODE_DELIP          2
#define VRRP_OPCODE_DELVR          3

#define VRRP_ADVER_DFL             1
#define VRRP_PRIO_DFL              100
#define VRRP_PRI_TRACK             10

#define VRRP_CFGERR_IFWRONG        1
#define VRRP_CFGERR_SUBNETDIFF     2
#define VRRP_CFGERR_MAXVSRV        3
#define VRRP_CFGERR_MAXVIP         4
#define VRRP_CFGERR_VSRVNOTEXIST   5
#define VRRP_CFGERR_VIPNOTEXIST    6
#define VRRP_CFGERR_MAXTRACK       7
#define VRRP_CFGERR_TRACK          8

typedef enum {
    VRRP_FUNC_VSRV_CONFIG_SET,
    VRRP_FUNC_VSRV_DEL_ALL_BY_IFINDEX,
    VRRP_FUNC_VSRV_PRIO_SET,
    VRRP_FUNC_VSRV_PREEMPT_SET,
    VRRP_FUNC_VSRV_ADVER_TIME_SET,
    VRRP_FUNC_VSRV_GETNEXT,
    VRRP_FUNC_VSRV_IFTRACK_ADD,
    VRRP_FUNC_VSRV_IFTRACK_DEL,
    VRRP_FUNC_ROUTE_PROTO_REG,
    VRRP_FUNC_ROUTE_PROTO_DEREG,
    VRRP_FUNC_TRACK_STATE_CHG,
    VRRP_FUNC_MAX
}VRRP_FUNC_ENT;

int32 sal_vrrp_vrid_vip_config(int ifindex, int vrid, sys_ipv4_t vipaddr, int opcode);
int32 sal_vrrp_vrid_priority_config(int ifindex, int vrid, ulong_t value, int *piErrNo);
int32 sal_vrrp_vrid_preempt_mode_config(int ifindex, int vrid, sys_enable_t bPreempt,
	ulong_t value, int *piErrNo);
int32 sal_vrrp_vrid_advertise_timer_config(int ifindex, int vrid, ulong_t value, int *piErrNo);
int32 sal_vrrp_vsrv_get_next(int ifindex, vrrp_rt *pVsrvInfo);
int32 sal_vrrp_vsrv_if_track_add(int ifindex, int vrid, 
	ulong_t ulIfTracIndex, ulong_t ulPrio, int *piErrNo);
int32 sal_vrrp_vsrv_if_track_del(int ifindex, int vrid, 
	ulong_t ulIfTracIndex, int *piErrNo);
int32 sal_vrrp_route_proto_reg(char *szProcessName, ulong_t ulFunCallBackId);
int32 sal_vrrp_route_proto_dereg(char *szProcessName, ulong_t ulFunCallBackId);

#endif 

