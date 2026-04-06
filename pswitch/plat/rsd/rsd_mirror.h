
#ifndef __RSD_MIRROR_H__
#define __RSD_MIRROR_H__

#include <rtk/mirror.h>
#include <common/sys_def.h>

typedef rtk_mirror_egrMode_t rsd_mirror_egrMode_t;

typedef rtk_mirror_rspanIgrMode_t rsd_mirror_rspanIgrMode_t;

typedef rtk_mirror_rspanEgrMode_t rsd_mirror_rspanEgrMode_t;

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)

typedef rtk_mirror_type_t rsd_mirror_type_t;

typedef rtk_mtp_type_t rsd_mirror_mtpType_t;
#endif

typedef enum
{
    MIRROR_PORT_MIRROR_OFF,
    MIRROR_PORT_MIRROR_RX,
    MIRROR_PORT_MIRROR_TX,
    MIRROR_PORT_MIRROR_BOTH,
    MIRROR_PORT_MIRROR_END,
}rsd_mirror_direction_e;

typedef struct rsd_mirror_entry_s
{
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
    rsd_mirror_type_t     mirror_type;
    rsd_mirror_mtpType_t  mtp_type;
#endif
    uint32                mirroring_devID;
    sys_logic_port_t      mirroring_port;
    sys_logic_portmask_t  mirrored_igrPorts;
    sys_logic_portmask_t  mirrored_egrPorts;
    uint32          oper_of_igr_and_egr_ports;
                    
    sys_enable_t    cross_vlan;
                    
    sys_enable_t    mirror_orginalPkt;
                    
    sys_enable_t    flowBasedOnly;

    sys_enable_t    mirroring_port_iso;
                    
} rsd_mirror_entry_t;

typedef struct rsd_mirror_sampleStat_s
{
    uint32  totalSamples;
    uint32  totalPkts;
} rsd_mirror_sampleStat_t;

typedef enum sys_mirror_state_e
{
    MIRROR_DISABLED,
    PORTBASED_ENABLED,
    MIRROR_STATE_END
}sys_mirror_state_t;

typedef struct sys_port_mirror_s
{
    sys_logic_portmask_t  mirrored_igrPorts;
    sys_logic_portmask_t  mirrored_egrPorts;

}sys_port_mirror_t;

typedef struct sys_mirror_event_s
{
    sys_gid_t gid;
    rsd_mirror_entry_t mirrorEntry;
} sys_mirror_event_t;

extern uint32 RangeId[];

#define FOR_EACH_GID(gid) \
    for((gid) = 0; (gid) < CAPA_MIRROR_SESSION_NUM; (gid)++)

extern int32 rsd_mirror_init(void);

extern int32 rsd_mirror_group_get(sys_gid_t gid, rsd_mirror_entry_t *pMirrorEntry);

extern int32 rsd_mirror_group_set(sys_gid_t gid, rsd_mirror_entry_t *pMirrorEntry);

extern int32 rsd_mirror_group_del(sys_gid_t gid);

extern int32 rsd_mirror_egrMode_get(sys_logic_port_t  port, rsd_mirror_egrMode_t  *pEgrMode);

extern int32 rsd_mirror_egrMode_set(sys_logic_port_t  port, rsd_mirror_egrMode_t  egrMode);
#endif 
