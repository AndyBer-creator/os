
#ifndef __CFG_LOOPBACK_H__
#define __CFG_LOOPBACK_H__

#include <libsal/sal_loopback.h>

 typedef struct loopback_port_table_d
 {
	 uint8 port_enable[SYS_NORMALPORT_NUM_MAX+1];
	 uint8 resume_mode[SYS_NORMALPORT_NUM_MAX+1];
 } loopback_port_table_f;

 typedef struct cfg_loopback_s
{
    
	uint32 detectionTime;
	uint32 resumeTime;
	sys_vlanmask_t controlVlanForLoopback;
	int32 bControlVlanAll;
	loopback_port_table_f port_value;
	uint32 mode;
	uint32 port;
	uint32 enable;
	sys_enable_t protoEnable;
	sys_enable_t protoResumeCheckEnable;
} cfg_loopback_t;

extern int32 cfg_loopback_init(void);
extern int32 cfg_loopback_detectionTime_set(uint32 timeValue);
extern int32 cfg_loopback_detectionTime_get(uint32 *timeValue);
extern int32 cfg_loopback_resumeTime_set(uint32 timeValue);
extern int32 cfg_loopback_resumeTime_get(uint32 *timeValue);
extern int32 cfg_loopback_port_enable_set(uint32 port, uint32 isEnable);
extern int32 cfg_loopback_port_enable_get(uint32 port, uint32 *isEnable);
extern int32 cfg_loopback_port_resume_mode_set(uint32 port, uint32 reMode);
extern int32 cfg_loopback_port_resume_mode_get(uint32 port, uint32 *reMode);
extern int32 cfg_loopback_proto_enable_set(sys_enable_t enable);
extern int32 cfg_loopback_proto_enable_get(sys_enable_t *pEnable);
extern int32 cfg_loopback_control_vlan_set(int32 bAdd);
extern int32 cfg_loopback_control_vlan_get(int32 *controlVlan);
extern int32 cfg_loopback_proto_resume_check_set(sys_enable_t enable);
extern int32 cfg_loopback_proto_resume_check_get(sys_enable_t *pEnable);

#endif

