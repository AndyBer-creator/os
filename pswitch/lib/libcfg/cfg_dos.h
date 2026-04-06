
#ifndef __CFG_DOS_H__
#define __CFG_DOS_H__

#include <libsal/sal_dos.h>

typedef struct cfg_dos_s
{
    uint32 maxPingLen;
    uint32 minIPv6FragLen;
    uint32 smurfNetmaskLen;
    uint32 minTCPHdrLen;
    sys_enable_t typeEnable[DOS_TYPE_END];
    sys_enable_t enable[SYS_LOGICPORT_NUM_MAX];

} cfg_dos_t;

extern int32 cfg_dos_typeEnable_set(sys_dos_type_t type, sys_enable_t enable);

extern int32 cfg_dos_typeEnable_get(sys_dos_type_t type, sys_enable_t *pEnable);

extern int32 cfg_dos_maxPingLen_set(uint32 length);

extern int32 cfg_dos_maxPingLen_get(uint32 *pLength);

extern int32 cfg_dos_minIPv6FragLen_set(uint32 length);

extern int32 cfg_dos_minIPv6FragLen_get(uint32 *pLength);

extern int32 cfg_dos_smurfNetmaskLen_set(uint32 length);

extern int32 cfg_dos_smurfNetmaskLen_get(uint32 *pLength);

extern int32 cfg_dos_minTCPHdrLen_set(uint32 length);

extern int32 cfg_dos_minTCPHdrLen_get(uint32 *pLength);

extern int32 cfg_dos_portEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_dos_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

#endif

