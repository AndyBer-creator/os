
#ifndef __CFG_DAI_H__
#define __CFG_DAI_H__

#include <libsal/sal_dai.h>

typedef struct cfg_dai_s
{
    sys_enable_t enable;
    sys_enable_t trust[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t smacChk[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t dmacChk[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t ipChk[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t ipZeroAllow[SYS_LOGICPORT_NUM_MAX];
    sys_vlanmask_t vlanEnable;
    uint32 arpLimitRate[SYS_LOGICPORT_NUM_MAX];
} cfg_dai_t;

extern int32 cfg_dai_enable_set(sys_enable_t enable);

extern int32 cfg_dai_enable_get(sys_enable_t *pEnable);

extern int32 cfg_dai_vlanmask_enable_set (sys_vlanmask_t *vlanmask);

extern int32 cfg_dai_vlanmask_enable_get (sys_vlanmask_t *vlanmask);

extern int32 cfg_dai_port_trust_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_dai_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_dai_port_smacChk_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_dai_port_smacChk_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_dai_port_dmacChk_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_dai_port_dmacChk_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_dai_port_ipChk_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_dai_port_ipChk_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_dai_port_ipZeroAllow_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_dai_port_ipZeroAllow_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_dai_rateLimit_get(sys_logic_port_t port, uint32 *pRateValue);

extern int32 cfg_dai_rateLimit_set(sys_logic_port_t port, uint32 rate);

#endif

