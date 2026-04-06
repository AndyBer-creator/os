
#ifndef __CFG_CUSTOM_H__
#define __CFG_CUSTOM_H__

typedef struct cfg_custom_s
{
    sys_enable_t enableExample;
    sys_enable_t portEnableExample[SYS_LOGICPORT_NUM_MAX];
} cfg_custom_t;

extern int32 cfg_custom_portEnableExample_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_custom_portEnableExample_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_custom_enableExample_set(sys_enable_t enable);

extern int32 cfg_custom_enableExample_get(sys_enable_t *pEnable);

extern int32 cfg_custom_init(void);

#endif  

