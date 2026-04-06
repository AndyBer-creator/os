
#ifndef __CFG_L2_H__
#define __CFG_L2_H__

 #include <libsal/sal_l2.h>

 typedef struct cfg_l2_s
{
    uint32 aging_time;
    sys_logic_portmask_t lookupmissPortmask;
} cfg_l2_t;

extern int32 cfg_l2_aging_get(uint32 *pAging_time);

extern int32 cfg_l2_aging_set(uint32 aging_time);

extern int32 cfg_l2_lookupMissFloodPortMask_get(sys_logic_portmask_t *pFlood_portmask);

extern int32 cfg_l2_lookupMissFloodPortMask_set(sys_logic_portmask_t *pFlood_portmask);

#endif

