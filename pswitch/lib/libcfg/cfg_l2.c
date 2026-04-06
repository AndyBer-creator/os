
#include <libcfg/cfg.h>
#include <libsal/sal_l2.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_type.h>

extern int32 cfg_l2_aging_get(uint32 *pAging_time)
{
    cfg_l2_t *macEntry = NULL;

    SYS_PARAM_CHK((NULL == pAging_time), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L2, (cfg_addr)&macEntry);

    *pAging_time = macEntry->aging_time;

    CFG_CLOSE(CFG_ID_L2);
}

int32 cfg_l2_aging_set(uint32 aging_time)
{
    cfg_l2_t *macEntry = NULL;

    CFG_OPEN(CFG_ID_L2, (cfg_addr)&macEntry);

    macEntry->aging_time = aging_time;

    CFG_CLOSE(CFG_ID_L2);
}

int32 cfg_l2_lookupMissFloodPortMask_get(sys_logic_portmask_t *pFlood_portmask)
{
    cfg_l2_t *macEntry = NULL;

    SYS_PARAM_CHK((NULL == pFlood_portmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L2, (cfg_addr)&macEntry);

    LOGIC_PORTMASK_COPY(*pFlood_portmask, macEntry->lookupmissPortmask);

    CFG_CLOSE(CFG_ID_L2);
}

int32 cfg_l2_lookupMissFloodPortMask_set(sys_logic_portmask_t *pFlood_portmask)
{
    cfg_l2_t *macEntry = NULL;

    SYS_PARAM_CHK((NULL == pFlood_portmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_L2, (cfg_addr)&macEntry);

    LOGIC_PORTMASK_COPY(macEntry->lookupmissPortmask, *pFlood_portmask);

    CFG_CLOSE(CFG_ID_L2);
}

