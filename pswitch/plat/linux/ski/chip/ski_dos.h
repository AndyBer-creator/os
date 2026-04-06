
#ifndef __SKI_DOS_H__
#define __SKI_DOS_H__

#include <ski/ski_core.h>
#include <rsd/rsd_dos.h>

typedef enum ski_dos_cmd_e
{
    
    SKI_DOS_SET_BASE = (SKI_CORE_MOD_DOS * SKI_CORE_MODCMD_TOTAL),
    SKI_DOS_TYPEENABLE_SET,
    SKI_DOS_MAXPINGLEN_SET,
    SKI_DOS_MINIPV6FRAGLEN_SET,
    SKI_DOS_SMURFNETMASKLEN_SET,
    SKI_DOS_MINTCPHDRLEN_SET,
    SKI_DOS_SET_PORT_ENABLE,

    SKI_DOS_GET_BASE = (SKI_DOS_SET_BASE + SKI_CORE_MODCMD_RANGE),

    SKI_DOS_CMD_END
} ski_dos_cmd_t;

typedef union ski_dos_ioctl_u
{
    struct {
        sys_dos_type_t type;
        sys_enable_t enable;
    } typeEnable;

    struct {
        uint32 length;
    } maxPingLen;

    struct {
        uint32 length;
    } minIPv6FragLen;

    struct {
        uint32 length;
    } smurfNetmaskLen;

    struct {
        uint32 length;
    } minTCPHdrLen;

    struct
    {
        sys_logic_port_t port;
        sys_enable_t enable;
    }portEnable;

} ski_dos_ioctl_t;

extern int32 ski_dos_init(void);

#endif  

