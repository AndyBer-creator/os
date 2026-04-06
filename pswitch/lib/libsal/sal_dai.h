
#ifndef __SAL_DAI_H__
#define __SAL_DAI_H__

#include <dai/proto_dai.h>

extern int32 sal_dai_enable_set(sys_enable_t enable);

extern int32 sal_dai_enable_get(sys_enable_t *pEnable);

extern int32 sal_dai_vlan_enable_set (sys_vid_t vid, sys_enable_t enable);

extern int32 sal_dai_vlan_enable_get (sys_vid_t vid, sys_enable_t *pEnable);

extern int32 sal_dai_vlanmask_enable_get (sys_vlanmask_t *vlanmask);

extern int32 sal_dai_port_trust_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dai_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dai_port_smacChk_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dai_port_smacChk_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dai_port_dmacChk_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dai_port_dmacChk_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dai_port_ipChk_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dai_port_ipChk_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dai_port_ipZeroAllow_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dai_port_ipZeroAllow_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dai_portMib_get(sys_logic_port_t port, sys_arp_mib_t *pCounter);

extern int32 sal_dai_portMib_clear(sys_logic_port_t port);

extern int32 sal_dai_vlanMib_get(sys_vid_t vid, sys_arp_mib_t *pCounter);

extern int32 sal_dai_vlanMib_clear(sys_vid_t vid);

extern int32 sal_dai_rateLimit_get(sys_logic_port_t port, uint32 *pRate);

extern int32 sal_dai_rateLimit_set(sys_logic_port_t port, uint32 rate);
#endif

