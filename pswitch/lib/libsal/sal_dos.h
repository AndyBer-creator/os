
#ifndef __SAL_DOS_H__
#define __SAL_DOS_H__

#include <rsd/rsd_dos.h>

extern int32 sal_dos_typeEnable_set(sys_dos_type_t type, sys_enable_t enable);

extern int32 sal_dos_typeEnable_get(sys_dos_type_t type, sys_enable_t *pEnable);

extern int32 sal_dos_maxPingLen_set(uint32 length);

extern int32 sal_dos_maxPingLen_get(uint32 *pLength);

extern int32 sal_dos_minIPv6FragLen_set(uint32 length);

extern int32 sal_dos_minIPv6FragLen_get(uint32 *pLength);

extern int32 sal_dos_smurfNetmaskLen_set(uint32 length);

extern int32 sal_dos_smurfNetmaskLen_get(uint32 *pLength);

extern int32 sal_dos_minTCPHdrLen_set(uint32 length);

extern int32 sal_dos_minTCPHdrLen_get(uint32 *pLength);

extern int32 sal_dos_portAttackPreventEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dos_portAttackPreventEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dos_portGarpEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dos_portGarpEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dos_portEnable_set(sys_logic_port_t port, sys_dos_type_t type, sys_enable_t enable);

extern int32 sal_dos_portEnable_get(sys_logic_port_t port, sys_dos_type_t type, sys_enable_t *pEnable);

extern int32 sal_dos_portLength_set(sys_logic_port_t port, sys_dos_type_t type, uint32 length);

extern int32 sal_dos_portLength_get(sys_logic_port_t port, sys_dos_type_t type, uint32 *pLength);

#endif
