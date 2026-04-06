
#include <libcfg/cfg.h>

int32 cfg_udld_messageTime_set(uint32 interval)
{
    cfg_udld_t *pUdld = NULL;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(interval, BOND_UDLD_INTERVAL_MIN, BOND_UDLD_INTERVAL_MAX), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_UDLD, (cfg_addr)&pUdld);

    pUdld->messageTime = interval;

    CFG_CLOSE(CFG_ID_UDLD);
}

int32 cfg_udld_messageTime_get(uint32 *pInterval)
{
    cfg_udld_t *pUdld = NULL;

    SYS_PARAM_CHK((NULL == pInterval), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_UDLD, (cfg_addr)&pUdld);

    *pInterval = pUdld->messageTime;

    CFG_CLOSE(CFG_ID_UDLD);
}

int32 cfg_udld_portMode_set(sys_logic_port_t port, sys_udld_mode_t mode)
{
    cfg_udld_t *pUdld = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(mode > UDLD_MODE_END, SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_UDLD, (cfg_addr)&pUdld);

    pUdld->portMode[port] = mode;

    CFG_CLOSE(CFG_ID_UDLD);
}

int32 cfg_udld_portMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode)
{
    cfg_udld_t *pUdld = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_UDLD, (cfg_addr)&pUdld);

    *pMode = pUdld->portMode[port];

    CFG_CLOSE(CFG_ID_UDLD);
}

