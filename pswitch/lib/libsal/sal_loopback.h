
#ifndef __SAL_LOOPBACK_H__
#define __SAL_LOOPBACK_H__

#include <libsal/sal_type.h>
#include <loopback/proto_sys_loopback.h>

typedef struct
{
	uint32 port;
	int32 status_old_1;
	int32 status_old_2;
}loopbackPortStatus_t;

#define SAL_LOOPBACK_Disabled                 0
#define SAL_LOOPBACK_Listening                1
#define SAL_LOOPBACK_Learning                 2
#define SAL_LOOPBACK_Forwarding               3
#define SAL_LOOPBACK_Blocking                 4
#define SAL_DEFAULT_DETECTIONTIME             5
#define SAL_DEFAULT_RESUMETIME         	      30
#define SAL_LOOPBACK_AUTO 					  1
#define SAL_LOOPBACK_MANUAL                   0
#define SAL_DEFAULT_BCONTROL_VLAN         	  1
#define SAL_DEFAULT_PORT_STATUS         	  0

int32 sal_loopback_detectionTime_set(uint32 timeValue);

int32 sal_loopback_detectionTime_get(uint32 *timeValue);

int32 sal_loopback_resumeTime_set(uint32 timeValue);

int32 sal_loopback_resumeTime_get(uint32 *timeValue);

int32 sal_loopback_port_enable_set(uint32 port, uint32 isEnable);

int32 sal_loopback_port_enable_get(uint32 port, uint32 *isEnable);

int32 sal_loopback_port_resume_mode_set(uint32 port, uint32 reMode);

int32 sal_loopback_port_resume_mode_get(uint32 port, uint32 *reMode);

int32 sal_loopback_port_resume_manual_set(uint32 port, uint32 reManual);

int32 sal_loopback_control_vlan_set(int32 bConVlan);

int32 sal_loopback_control_vlan_get(int32 *controlVlan);

int32 sal_loopback_port_status_state_get(uint32 port, uint32 *state);

int32 sal_loopback_proto_enable_set(sys_enable_t enable);

int32 sal_loopback_proto_enable_get(sys_enable_t *pEnable);

int32 sal_loopback_proto_resume_check_set(sys_enable_t enable);

int32 sal_loopback_proto_resume_check_get(sys_enable_t *pEnable);

#if 0
 
 int32 sal_erps_ringState_set(uint32 instId, sys_enable_t state);

 int32 sal_erps_ringState_get(uint32 instId, uint32 *pState);

#endif

#endif 

