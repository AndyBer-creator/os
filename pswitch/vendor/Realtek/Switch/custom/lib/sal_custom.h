
#ifndef __SAL_CUSTOM_H__
#define __SAL_CUSTOM_H__

#include <custom/ski_custom.h>
#include <libcustom/cfg_custom.h>

#ifdef CONFIG_SYS_SNMP
#include <libsal/sal_snmp.h>
#endif
#include <libsal/sal_stp.h>

typedef enum snmp_cmTrapCode_e
{
    SNMP_CMTRAP_CODE_START = 0x100,
    SNMP_CMTRAP_CODE_TEST,
    SNMP_CMTRAP_CODE_CPU,
    SNMP_CMTRAP_CODE_MEMORY,
	SNMP_CMTRAP_CODE_STORMCTRL,
	SNMP_CMTRAP_CODE_POE,
	SNMP_CMTRAP_CODE_OPERATE_START = 0x200,
	SNMP_CMTRAP_CODE_REBOOT,
	SNMP_CMTRAP_CODE_UPGRADE,
	SNMP_CMTRAP_CODE_LINK,
	SNMP_CMTRAP_CODE_OPERATE,
	SNMP_CMTRAP_CODE_LOGIN,
	SNMP_CMTRAP_CODE_PASSWORD_CHANGE,
	SNMP_CMTRAP_CODE_USER_OPERATION,
	SNMP_CMTRAP_CODE_USER_OPERATION_INFO,

    SNMP_CMTRAP_CODE_END
} snmp_cmTrapCode_t;

extern int32 sal_custom_portEnableExample_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_custom_portEnableExample_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_custom_enableExample_set(sys_enable_t enable);

extern int32 sal_custom_enableExample_get(sys_enable_t *pEnable);

#endif

