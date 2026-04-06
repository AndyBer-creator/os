
#ifndef __SAL_TRUNK_CHK_H__
#define __SAL_TRUNK_CHK_H__

#define SYS_TRK_MBR_PORT_NORMAL_MAX_ERR     SYS_ERR_TRUNK_MORE_THAN_8_PORTS
#define SYS_TRK_MBR_PORT_LACP_MAX_ERR       SYS_ERR_TRUNK_MORE_THAN_16_PORTS

extern int32 sal_trunk_memberNum_check(
    sys_trunk_portRole_t trunkRole,
    sys_logic_portmask_t *pPortmask);

extern int32 sal_trunk_valid_check(uint32 trunkId, sys_logic_portmask_t *pPortmask);

extern int32 sal_trunk_portRole_change(sys_logic_port_t srcPort, sys_logic_port_t dstPort);

extern int32 sal_trunk_setting_refresh(uint32 trunkId);

#endif 

