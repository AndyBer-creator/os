#ifndef __proto_sys_erps_h_
#define __proto_sys_erps_h_

#ifdef CONFIG_SYS_PROTO_ERPS

#define ERPS_MST_INST_ID         (CAPA_STP_MST_INSTANCE_NUM + 1)
#define ERPS_MAX_RING_INS        CAPA_STP_MST_INSTANCE_NUM
typedef enum erps_port_role_s{
    ERPS_RPL_PORT,
    ERPS_RPL_Owner_PORT,
    ERPS_RPL_Neighbour_PORT,
    ERPS_RPL_Next_Neighbour_PORT,
    ERPS_PORT_ROLE_NONE
}erps_port_role_t;

#if 0
#define ERPS_RPL_PORT                  1
#define ERPS_RPL_Owner_PORT            2
#define ERPS_RPL_Neighbour_PORT        3
#define ERPS_RPL_Next_Neighbour_PORT   4
#define ERPS_PORT_ROLE_NONE            5
#endif
#define Erps_Protected_Inst_Add  1
#define Erps_Protected_Inst_Del  2

#define ERPS_WTR_TIME        1
#define ERPS_WTB_TIME        2
#define ERPS_GUARD_TIME      3
#define ERPS_HOLDOFF_TIME    4
extern int32 erps_init(void);
extern int32 erps_exit(void);
extern int32 erps_enable_set(sys_enable_t enable);
extern int32 erps_enable_get(sys_enable_t *pEnable);
extern int erps_inst_create(uint32 instId);
extern int erps_inst_delete(uint32 instId);
extern int erps_timer_set(int code, uint32 instId, uint32 value);
extern int erps_ringPort_set(uint32 instId, int portId, int portRole, uint32 isPort0, uint32 isPortAdd);
extern int erps_ringId_set(uint32  instId, uint32 ringId);
extern int erps_ctrlVlan_set(uint32 instId, uint32 vlanId);
extern int erps_ringLevel_set(uint32 instId, uint32 level);
extern int erps_mel_set(uint32 instId, uint32 mel);
extern int erps_workMode_set(uint32 instId, uint32 mode);
extern int erps_inst_pgInst_set(uint32 instId, uint32 instList, uint32 status);
extern int32 erps_ringState_set(uint32 instId, sys_enable_t state);
extern int erps_get_inst_state_status(uint32 instId, int32 *state);
extern int erps_get_inst_port_state_status(uint32 portId, int32 *state);

#endif 

#endif 

