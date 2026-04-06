
#include <libsal/sal_util.h>
#include <libsal/sal_switch.h>

#ifdef CONFIG_SYS_JUMBO_FRAME

int32 sal_switch_maxFrameSize_set(uint32 maxSize)
{
    SYS_PARAM_CHK(((maxSize > BOND_PORT_JUMBO_FRAME_MAX) || (maxSize < BOND_PORT_JUMBO_FRAME_MIN)), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_switch_maxFrameSize_set(maxSize));
    SYS_ERR_CHK(cfg_switch_maxFrameSize_set(maxSize));

    return SYS_ERR_OK;
}

int32 sal_switch_maxFrameSize_get(uint32 *pMaxSize)
{
    SYS_PARAM_CHK((NULL == pMaxSize), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_switch_maxFrameSize_get(pMaxSize));

    return SYS_ERR_OK;
}
#endif 

