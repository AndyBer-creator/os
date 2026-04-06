
#ifndef __SAL_UDLD_H__
#define __SAL_UDLD_H__

#include <udld/proto_udld.h>

extern const sys_text_t text_salUdldMode[];
extern const sys_text_t text_udldBidirState[];
extern const sys_text_t text_udldOperState[];

extern int32 sal_udld_messageTime_set(uint32 interval);

extern int32 sal_udld_messageTime_get(uint32 *pInterval);

extern int32 sal_udld_portMode_set(sys_logic_port_t port, sys_udld_mode_t mode);

extern int32 sal_udld_portMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode);

extern int32 sal_udld_portOperMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode);

extern int32 sal_udld_portBidiretionState_get(sys_logic_port_t port, sys_udld_bidir_state_t *pBidirState);

extern int32 sal_udld_portOperState_get(sys_logic_port_t port, sys_udld_op_state_t *pOperational_state);

extern int32 sal_udld_portNeighborNumber_get(sys_logic_port_t port, uint16 *pNeighborNum);

extern int32 sal_udld_portNeighbor_get(sys_logic_port_t port, uint16 neighborId, sys_udld_neighbor_info_t *pNbrInfo);

extern int32 sal_udld_portNeighborEcho_get(sys_logic_port_t port, uint16 neighborId, uint16 echoId, sys_udld_echo_info_t  *pEchoInfo);

extern int32 sal_udld_error_disable_reset(void);

#endif

