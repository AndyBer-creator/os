
#ifndef __MOD_CUSTOM_ACLEX_H__
#define __MOD_CUSTOM_ACLEX_H__

#include <common/sys_def.h>

extern int32 mod_custom_portEnableExample_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 mod_custom_portEnableExample_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 mod_custom_enableExample_set(sys_enable_t enable);

extern int32 mod_custom_enableExample_get(sys_enable_t *pEnable);

extern int32 mod_custom_example_init(void);
#endif

