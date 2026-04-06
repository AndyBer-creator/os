
#ifndef __CFG_PSECURE_H__
#define __CFG_PSECURE_H__

#include <libsal/sal_psecure.h>

typedef struct cfg_psecure_s
{
    sys_psecure_cfg_t cfg;
} cfg_psecure_t;

extern int32 cfg_psecure_enable_set(sys_enable_t enable);

extern int32 cfg_psecure_enable_get(sys_enable_t *pEnable);

extern int32 cfg_psecure_rateLimit_set(uint32 rate);

extern int32 cfg_psecure_rateLimit_get(sys_enable_t *pRate);

extern int32 cfg_psecure_port_set(sys_logic_port_t port, sys_psecure_port_t *pSecure);

extern int32 cfg_psecure_port_get(sys_logic_port_t port, sys_psecure_port_t *pSecure);

#endif 

