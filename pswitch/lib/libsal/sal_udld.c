
#include <libsal/sal_util.h>
#include <libsal/sal_udld.h>

const sys_text_t text_salUdldMode[] =
{
    {         "disabled"    },
    {           "normal"     },
    {       "aggressive"     },
};

const sys_text_t text_udldBidirState[] =
{
    {          "Unknown" },
    {                 "transmit/receive loop"},
    {       "Neighbor's echo is empty"},
    {    "Mismatch with neighbor state reported"},
    {             "Bidirectional"},
    {            "Unidirectional"},
};

const sys_text_t text_udldOperState[] =
{
    {             "Link down"},
    {                 ""},
    {                "Link up"},
    {                "Advertisement"},
    {                "Detection"},
    {      "Extended Detection"},
    {     "Advertisement - SINGLE NEIGHBOR DETECTED"},
    {      "Advertisement - MULTIPLE NEIGHBORS DETECTED"},
    {            "Disabled port"},
    {              "Link up"},
};

int32 sal_udld_messageTime_set(uint32 interval)
{
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(interval, BOND_UDLD_INTERVAL_MIN, BOND_UDLD_INTERVAL_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(udld_messageTime_set(interval));
    SYS_ERR_CHK(cfg_udld_messageTime_set(interval));

    return SYS_ERR_OK;
}

int32 sal_udld_messageTime_get(uint32 *pInterval)
{
    SYS_PARAM_CHK((NULL == pInterval), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_udld_messageTime_get(pInterval));

    return SYS_ERR_OK;
}

int32 sal_udld_portMode_set(sys_logic_port_t port, sys_udld_mode_t mode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(mode > UDLD_MODE_END, SYS_ERR_INPUT);

    SYS_ERR_CHK(udld_portMode_set(port, mode, FALSE));
    SYS_ERR_CHK(cfg_udld_portMode_set(port, mode));

    return SYS_ERR_OK;
}

int32 sal_udld_portMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_udld_portMode_get(port, pMode));

    return SYS_ERR_OK;
}

int32 sal_udld_portOperMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode)
{
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(udld_portOperMode_get(port, pMode));

    return SYS_ERR_OK;
}

int32 sal_udld_portBidiretionState_get(sys_logic_port_t port, sys_udld_bidir_state_t *pBidirState)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pBidirState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(udld_portBidiretionState_get(port, pBidirState));

    return SYS_ERR_OK;
}

int32 sal_udld_portOperState_get(sys_logic_port_t port, sys_udld_op_state_t *pOperational_state)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pOperational_state), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(udld_portOperState_get(port, pOperational_state));

    return SYS_ERR_OK;
}

int32 sal_udld_portNeighborNumber_get(sys_logic_port_t port, uint16 *pNeighborNum)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pNeighborNum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(udld_portNeighborNumber_get(port, pNeighborNum));

    return SYS_ERR_OK;
}

int32 sal_udld_portNeighbor_get(sys_logic_port_t port, uint16 neighborId, sys_udld_neighbor_info_t *pNbrInfo)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pNbrInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(udld_portNeighbor_get(port, neighborId, pNbrInfo));

    return SYS_ERR_OK;
}

int32 sal_udld_portNeighborEcho_get(sys_logic_port_t port, uint16 neighborId, uint16 echoId, sys_udld_echo_info_t *pEchoInfo)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEchoInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(udld_portNeighborEcho_get(port, neighborId, echoId, pEchoInfo));

    return SYS_ERR_OK;
}

int32 sal_udld_error_disable_reset(void)
{
    SYS_ERR_CHK(udld_error_disable_reset());

    return SYS_ERR_OK;
}

