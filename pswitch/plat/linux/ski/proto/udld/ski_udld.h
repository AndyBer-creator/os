
#ifndef __SKI_UDLD_H__
#define __SKI_UDLD_H__

#include <ski/ski_core.h>
#include <udld/proto_udld.h>

typedef enum ski_udld_cmd_e
{
    
    SKI_UDLD_SET_BASE = (SKI_CORE_MOD_UDLD * SKI_CORE_MODCMD_TOTAL),
    SKI_UDLD_FIBER_MODE_SET,
    SKI_UDLD_PORT_MODE_SET,
    SKI_UDLD_MESSAGE_TIME_SET,

    SKI_UDLD_GET_BASE = (SKI_UDLD_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_UDLD_FIBER_MODE_GET,
    SKI_UDLD_PORT_MODE_GET,
    SKI_UDLD_MESSAGE_TIME_GET,
    SKI_UDLD_PORT_OPER_MODE_GET,
    SKI_UDLD_PORT_BIDIR_STATE_GET,
    SKI_UDLD_PORT_OPER_STATE_GET,
    SKI_UDLD_PORT_NEIGHBOR_NUMBER_GET,
    SKI_UDLD_PORT_NEIGHBOR_GET,
    SKI_UDLD_PORT_NEIGHBOR_ECHO_GET,
    SKI_UDLD_ERROR_DISABLE_RESET,

    SKI_UDLD_CMD_END
} ski_udld_cmd_t;

typedef struct ski_udld_portMode_s
{
    sys_logic_port_t    port;
    sys_udld_mode_t     mode;
    uint8     isFollow;
} ski_udld_portMode_t;

typedef struct ski_udld_portOperMode_s
{
    sys_logic_port_t    port;
    sys_udld_mode_t     mode;
} ski_udld_portOperMode_t;

typedef struct ski_udld_portBidirState_s
{
    sys_logic_port_t port;
    sys_udld_bidir_state_t bidirState;
} ski_udld_portBidirState_t;

typedef struct ski_udld_portOperState_s
{
    sys_logic_port_t port;
    sys_udld_op_state_t operational_state;
} ski_udld_portOperState_t;

typedef struct ski_udld_portNeighborNumber_s
{
    sys_logic_port_t port;
    uint16 neighborNum;
} ski_udld_portNeighborNumber_t;

typedef struct ski_udld_portNeighbor_s
{
    sys_logic_port_t port;
    uint16 neighborId;
    sys_udld_neighbor_info_t nbrInfo;
} ski_udld_portNeighbor_t;

typedef struct ski_udld_portNeighborEcho_s
{
    sys_logic_port_t port;
    uint16 neighborId;
    uint16 echoId;
    sys_udld_echo_info_t  echoInfo;
} ski_udld_portNeighborEcho_t;

typedef union ski_udld_ioctl_u
{
    sys_udld_mode_t                 udldFiberMode;
    ski_udld_portMode_t             udldPortMode;
    ski_udld_portOperMode_t         udldPortOperMode;
    ski_udld_portBidirState_t       udldPortBidirState;
    ski_udld_portOperState_t        udldPortOperState;
    ski_udld_portNeighborNumber_t   udldPortNeighborNumber;
    ski_udld_portNeighbor_t         udldPortNeighbor;
    ski_udld_portNeighborEcho_t     udldPortNeighborEcho;
    int32                           messageTime;
} ski_udld_ioctl_t;

extern int32 ski_udld_init(void);

#endif

