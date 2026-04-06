
#include <libcfg/cfg.h>

int32 cfg_switch_maxFrameSize_set(uint32 maxSize)
{
    cfg_switch_t *pSwitch = NULL;

    CFG_OPEN(CFG_ID_SWITCH, (cfg_addr)&pSwitch);

    pSwitch->maxFrameSize = maxSize;

    CFG_CLOSE(CFG_ID_SWITCH);
}

int32 cfg_switch_maxFrameSize_get(uint32 *pMaxSize)
{
    cfg_switch_t *pSwitch = NULL;

    SYS_PARAM_CHK((NULL == pMaxSize), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SWITCH, (cfg_addr)&pSwitch);

    *pMaxSize = pSwitch->maxFrameSize;

    CFG_CLOSE(CFG_ID_SWITCH);
}

