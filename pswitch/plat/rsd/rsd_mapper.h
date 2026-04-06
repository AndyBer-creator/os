
#ifndef __RSD_MAPPER_H__
#define __RSD_MAPPER_H__

#include <common/sys_def.h>
#include <rsd/rsd_mib.h>

typedef struct rsd_mapper_s
{
    
    int32   (*rsd_mib_init)(void );
    int32   (*rsd_mib_debug_globalCntr_get)(sys_mib_debug_globalCtr_type_t type, uint64 *pResult);
    int32   (*rsd_mib_debug_portCntr_get)(sys_logic_port_t port, sys_mib_debug_portCtr_type_t type, uint64 *pResult);
    int32   (*rsd_mib_cntrDebug_clr)(void);
    int32   (*rsd_mib_ifInNUcastPkts_get)(sys_logic_port_t port, uint64 *pResult);
    int32   (*rsd_mib_ifInMulticastPkts_get)(sys_logic_port_t port, uint64 *pResult);
    int32   (*rsd_mib_ifInBroadcastPkts_get)(sys_logic_port_t port, uint64 *pResult);
    int32   (*rsd_mib_dot3_status_alignment_errors_get)(sys_logic_port_t port, uint64 *pResult);
    int32   (*rsd_mib_dot3StatsFCSErrors_get)(sys_logic_port_t port, uint64 *pResult);
    int32   (*rsd_mib_dot3StatsFrameTooLongs_get)(sys_logic_port_t port, uint64 *pResult);
    int32   (*rsd_mib_etherStatsOctets_get)(sys_logic_port_t port, uint64 *pResult);
    int32   (*rsd_mib_etherStatsCRCAlignErrs_get)(sys_logic_port_t port, uint64 *pResult);
    int32   (*rsd_mib_etherStatsPkts_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_etherStatsBcastPkts_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_etherStatsMcastPkts_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_etherStatsPkts64Octets_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_etherStatsPkts65to127Octets_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_etherStatsPkts128to255Octets_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_etherStatsPkts256to511Octets_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_etherStatsPkts512to1023Octets_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_etherStatsPkts1024to1518Octets_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_ifInUcastPkts_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_ifOutUcastPkts_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_ifOutMcastPkts_get)(sys_logic_port_t port, uint64 * pResult);
    int32   (*rsd_mib_ifOutBcastPkts_get)(sys_logic_port_t port, uint64 * pResult);
} rsd_mapper_t;

extern rsd_mapper_t *rsd_mapper;

extern int32 rsd_mapper_init(void);

#endif

