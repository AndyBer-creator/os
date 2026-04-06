
#ifndef __SKI_ERPS_H__
#define __SKI_ERPS_H__

#include <ski/ski_core.h>
#include <erps/proto_sys_erps.h>

typedef enum ski_erps_cmd_e
{
    
    SKI_ERPS_SET_BASE = (SKI_CORE_MOD_ERPS * SKI_CORE_MODCMD_TOTAL),
    SKI_ERPS_SET_ENABLE,
    SKI_ERPS_INST_CREATE,
    SKI_ERPS_INST_DEL,
	SKI_ERPS_INST_CTRLVLAN,
	SKI_ERPS_INST_WTR_TIMER,
	SKI_ERPS_INST_WTB_TIMER,
	SKI_ERPS_INST_GUARD_TIMER,
	SKI_ERPS_INST_HOLDOFF_TIMER,
	SKI_ERPS_INST_WORKMODE,
	SKI_ERPS_INST_RINGID,
	SKI_ERPS_INST_RINGLEVEL,
	SKI_ERPS_INST_RINGPORT_ADD,
	SKI_ERPS_INST_RINGPORT_DEL,
	SKI_ERPS_INST_RING_PGINST_ADD,
	SKI_ERPS_INST_RING_PGINST_DEL,
	SKI_ERPS_INST_MEL,
	SKI_ERPS_INST_RINGENABLE,

    SKI_ERPS_GET_BASE = (SKI_ERPS_SET_BASE + SKI_CORE_MODCMD_RANGE),
    
    SKI_ERPS_GET_RING_STATE_STATUS,
    SKI_ERPS_GET_RING_PORT_STATE_STATUS,
    SKI_ERPS_GET_ENABLE,
    SKI_ERPS_GET_INST,
    SKI_ERPS_GET_INST_CTRLVLAN,
    SKI_ERPS_GET_INST_WTR_TIMER,
	SKI_ERPS_GET_INST_WTB_TIMER,
	SKI_ERPS_GET_INST_GUARD_TIMER,
	SKI_ERPS_GET_INST_HOLDOFF_TIMER,
	SKI_ERPS_GET_INST_WORKMODE,
	SKI_ERPS_GET_INST_RINGID,
	SKI_ERPS_GET_INST_RINGLEVEL,
	SKI_ERPS_GET_INST_RINGPORT,
	SKI_ERPS_GET_INST_RING_PGINST,
	SKI_ERPS_GET_INST_MEL,
	SKI_ERPS_GET_INST_RINGENABLE,

    SKI_STP_GET_INFO,
    SKI_STP_GET_PORT_INFO,
    SKI_STP_GET_MST_INFO,
    SKI_STP_GET_MST_PORT_INFO,
    SKI_STP_GET_MST_PORT_MSG,

    SKI_ERPS_CMD_END
} ski_erps_cmd_t;
#define ERPS_NAME_LEN_MAX            (32)
#define ERPS_VLAN_TO_INSTANCE_SIZE   (4096)

typedef struct ski_erps_ioctl_u
{
	sys_enable_t enable;
	int32 instId;				 
	uint32 mel; 			  
	int32 workMode; 			 
	int32 TxRAPSTime;
	int32 WTRTime;
	int32 WTBTime;
	int32 guardTime;
	int32 holdoffTime;
	int32 pg_inst;
	int32 ringId;							 
	int32 ringLevel;						 
	int32 ctrlVlanId;						 
	sys_enable_t ringEnable;					
	uint8 port0;					 
	int32 port0Role;
	uint8 port1;					 
	int32 port1Role;
	int32 state;
	int32 portState;
	uint32 portId;
	int32 isPort0;

} ski_erps_ioctl_t;

extern int32 ski_erps_init(void);

#endif  

