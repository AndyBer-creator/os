
#ifndef __SKI_BOARD_H__
#define __SKI_BOARD_H__

#include <ski/ski_core.h>
#include <rsd/rsd_sys.h>

typedef enum ski_board_cmd_e
{
    
    SKI_BOARD_SET_BASE = (SKI_CORE_MOD_BOARD * SKI_CORE_MODCMD_TOTAL),

    SKI_BOARD_GET_BASE = (SKI_BOARD_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_BOARD_GET_DEVICE_INFO,
    SKI_BOARD_GET_BOARD_MODEL,
    SKI_BOARD_GET_BOARD_PORTCONF,

    SKI_BOARD_CMD_END
} ski_board_cmd_t;

typedef struct ski_sys_devInfo_s
{
    uint32               dev;
    sys_switch_devInfo_t devInfo;
} ski_sys_devInfo_t;

typedef struct ski_sys_boardPortConf_s
{
    sys_logic_port_t port;
    sys_port_conf_t portConf;
} ski_sys_boardPortConf_t;

typedef union ski_board_ioctl_u
{
    ski_sys_devInfo_t devInfo;
    sys_boardModel_t  boardModel;
    ski_sys_boardPortConf_t   boardPortconf;
} ski_board_ioctl_t;

extern int32 ski_board_init(void);

#endif  

