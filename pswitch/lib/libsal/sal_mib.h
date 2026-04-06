
#ifndef __SAL_MIB_H__
#define __SAL_MIB_H__

#include <rsd/rsd_mib.h>

#define SAL_MIB_PORT_STATISTIC_FILE "/tmp/portstat"
#define MIB_DEBUG_GLOBAL_TBL_SIZE   DEBUG_TYPE_END
#define MIB_DEBUG_PORT_TBL_SIZE     (25)

typedef struct mib_cntr_tbl_s
{
    char *name;
    int32 (*sal_mib_func)(sys_logic_port_t port, uint64 *pResult);
} mib_cntr_tbl_t;

typedef struct mib_cntr_tbls_s
{
    char *name;
    mib_cntr_tbl_t  *mib_cntr;
} mib_cntr_tbls_t;

typedef struct mib_cntr_glbtbl_s
{
    char *name;
    int32 (*sal_mib_func)(uint64 *pResult);
} mib_cntr_glbtbl_t;

extern mib_cntr_tbl_t rmonMibTbl[];
extern const mib_cntr_tbls_t dbg_mibtbls[];
extern const mib_cntr_tbls_t user_mibtbls[];
extern const mib_cntr_glbtbl_t mibGlbTbl[];

extern int32 sal_mib_init(void);

extern int32 sal_mib_cntr_clr(sys_logic_port_t port);

extern int32 sal_mib_cntrGlobal_clr(void);

extern int32 sal_mib_cntrDebug_clr(void);

extern int32 sal_mib_debug_globalCntr_get(sys_mib_debug_globalCtr_type_t type, uint64 *pResult);

extern int32 sal_mib_debug_portCntr_get(sys_logic_port_t port, sys_mib_debug_portCtr_type_t type, uint64 *pResult);

extern int32 sal_mib_txAbortPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_collisionPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsDropEvents_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_cpuStatsPkts_get(uint64 *pResult);

extern int32 sal_mib_etherStatsBcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsMcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsCRCAlignErrs_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsUnderSizePkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsOverSizePkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsFragments_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsJabbers_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsCollisions_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsPkts64Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsPkts65to127Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsPkts128to255Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsPkts256to511Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsPkts512to1023Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_etherStatsPkts1024to1518Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot1dTpLearnedEntryDiscards_get(uint64 *pResult);

extern int32 sal_mib_dot1dTpPortInDiscards_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3_statsAlignmentErrors_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3StatsFCSErrors_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3StatsSingleCollisionFrames_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3StatsMultipleCollisionFrames_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3StatsDeferredTransmissions_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3StatsLateCollisions_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3StatsExcessiveCollisions_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3StatsSymbolErrors_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3StatsFrameTooLongs_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3ControlInUnknownOpcodes_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3OutPauseFrames_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3InPauseFrames_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_dot3StatsInternalMacReceiveErrors_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifInOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifInUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifInNUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifInDiscards_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifInErrors_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifOutOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifOutNUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifOutDiscards_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifOutErrors_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifInMulticastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifInBroadcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifOutQLen_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifHCInOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifHCInUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifHCInMulticastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifHCInBroadcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifHCOutOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifHCOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifHCOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_ifHCOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 sal_mib_privateInErr_get(sys_logic_port_t port, uint64 *pResult);
#endif 

