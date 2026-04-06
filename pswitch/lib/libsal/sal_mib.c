
#include <ski/chip/ski_mib.h>

#include <libsal/sal_util.h>
#include <libsal/sal_mib.h>

const mib_cntr_glbtbl_t mibGlbTbl[] =
{
    {"dot1dTpLearnedEntryDiscards",            sal_mib_dot1dTpLearnedEntryDiscards_get},
    {NULL, NULL}
};

mib_cntr_tbl_t ifMibTbl[] =
{
    {"ifInOctets",                          sal_mib_ifInOctets_get},
    {"ifInUcastPkts",                       sal_mib_ifInUcastPkts_get},
    {"ifInNUcastPkts",                      sal_mib_ifInNUcastPkts_get},
    {"ifInDiscards",                        sal_mib_ifInDiscards_get},
    {"ifOutOctets",                         sal_mib_ifOutOctets_get},
    {"ifOutUcastPkts",                      sal_mib_ifOutUcastPkts_get},
    {"ifOutNUcastPkts",                     sal_mib_ifOutNUcastPkts_get},
    {"ifOutDiscards",                       sal_mib_ifOutDiscards_get},
    {"ifInMulticastPkts",                   sal_mib_ifInMulticastPkts_get},
    {"ifInBroadcastPkts",                   sal_mib_ifInBroadcastPkts_get},
    {"ifOutMulticastPkts",                  sal_mib_ifOutMcastPkts_get},
    {"ifOutBroadcastPkts",                  sal_mib_ifOutBcastPkts_get},
    {NULL, NULL}
};

mib_cntr_tbl_t dot3MibTbl[] =
{
    {"dot3StatsAlignmentErrors",            sal_mib_dot3_statsAlignmentErrors_get},
    {"dot3StatsFCSErrors",                  sal_mib_dot3StatsFCSErrors_get},
    {"dot3StatsSingleCollisionFrames",      sal_mib_dot3StatsSingleCollisionFrames_get},
    {"dot3StatsMultipleCollisionFrames",    sal_mib_dot3StatsMultipleCollisionFrames_get},
    {"dot3StatsDeferredTransmissions",      sal_mib_dot3StatsDeferredTransmissions_get},
    {"dot3StatsLateCollisions",             sal_mib_dot3StatsLateCollisions_get},
    {"dot3StatsExcessiveCollisions",        sal_mib_dot3StatsExcessiveCollisions_get},
    {"dot3StatsFrameTooLongs",              sal_mib_dot3StatsFrameTooLongs_get},
    {"dot3StatsSymbolErrors",               sal_mib_dot3StatsSymbolErrors_get},
    {"dot3ControlInUnknownOpcodes",         sal_mib_dot3ControlInUnknownOpcodes_get},
    {"dot3InPauseFrames ",                  sal_mib_dot3InPauseFrames_get},
    {"dot3OutPauseFrames ",                 sal_mib_dot3OutPauseFrames_get},
    {NULL, NULL}
};

mib_cntr_tbl_t rmonMibTbl[] =
{
    {"etherStatsDropEvents",                sal_mib_etherStatsDropEvents_get},
    {"etherStatsOctets",                    sal_mib_etherStatsOctets_get},
    {"etherStatsPkts",                      sal_mib_etherStatsPkts_get},
    {"etherStatsBroadcastPkts",             sal_mib_etherStatsBcastPkts_get},
    {"etherStatsMulticastPkts",             sal_mib_etherStatsMcastPkts_get},
    {"etherStatsCRCAlignErrors",            sal_mib_etherStatsCRCAlignErrs_get},
    {"etherStatsUnderSizePkts",             sal_mib_etherStatsUnderSizePkts_get},
    {"etherStatsOverSizePkts",              sal_mib_etherStatsOverSizePkts_get},
    {"etherStatsFragments",                 sal_mib_etherStatsFragments_get},
    {"etherStatsJabbers",                   sal_mib_etherStatsJabbers_get},
    {"etherStatsCollisions",                sal_mib_etherStatsCollisions_get},
    {"etherStatsPkts64Octets",              sal_mib_etherStatsPkts64Octets_get},
    {"etherStatsPkts65to127Octets",         sal_mib_etherStatsPkts65to127Octets_get},
    {"etherStatsPkts128to255Octets",        sal_mib_etherStatsPkts128to255Octets_get},
    {"etherStatsPkts256to511Octets",        sal_mib_etherStatsPkts256to511Octets_get},
    {"etherStatsPkts512to1023Octets",       sal_mib_etherStatsPkts512to1023Octets_get},
    {"etherStatsPkts1024to1518Octets",      sal_mib_etherStatsPkts1024to1518Octets_get},
    {NULL, NULL}
};

mib_cntr_tbl_t miscMibTbl[] =
{
    {"ifHCInOctets",                        sal_mib_ifHCInOctets_get},
    {"ifHCInUcastPkts",                     sal_mib_ifHCInUcastPkts_get},
    {"ifHCInMulticastPkts",                 sal_mib_ifInMulticastPkts_get},
    {"ifHCInBroadcastPkts",                 sal_mib_ifInBroadcastPkts_get},
    {"ifHCOutOctets",                       sal_mib_ifOutOctets_get},
    {"ifHCOutUcastPkts",                    sal_mib_ifOutUcastPkts_get},
    {"ifHCOutMulticastPkts",                sal_mib_ifOutMcastPkts_get},
    {"ifHCOutBroadcastPkts",                sal_mib_ifOutBcastPkts_get},
    {"ipInReceives",                        NULL},
    {"ipInDiscards",                        NULL},
    {"dot1dTpPortInDiscards",               sal_mib_dot1dTpPortInDiscards_get},
    {NULL, NULL}
};

const mib_cntr_tbls_t dbg_mibtbls[] =
{
    {"IF mib",          ifMibTbl},
    {"Ether-Like mib",  dot3MibTbl},
    {"Rmon mib",        rmonMibTbl},
    {"Misc mib",        miscMibTbl},
    {NULL,                NULL}
};

const mib_cntr_tbls_t user_mibtbls[] =
{
    {"IF mib",          ifMibTbl},
    {"Ether-Like mib",  dot3MibTbl},
    {"Rmon mib",        rmonMibTbl},
    {NULL,                NULL}
};

int32 sal_mib_init(void)
{
    sys_logic_port_t port;

    FOR_EACH_LOGIC_PORT(port)
    {
        sal_mib_cntr_clr(port);
    }

    return SYS_ERR_OK;
}

int32 sal_mib_cntr_clr(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_TRK_CHK(rsd_mib_cntr_clr(port));

    return SYS_ERR_OK;
}

int32 sal_mib_cntrGlobal_clr(void)
{
    SYS_ERR_CHK(rsd_mib_cntrGlobal_clr());

    return SYS_ERR_OK;
}

int32 sal_mib_cntrDebug_clr(void)
{
    SYS_ERR_CHK(rsd_mib_cntrDebug_clr());

    return SYS_ERR_OK;
}

int32 sal_mib_debug_globalCntr_get(sys_mib_debug_globalCtr_type_t type, uint64 *pResult)
{
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((type >= DEBUG_TYPE_END), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(rsd_mib_debug_globalCntr_get(type, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_debug_portCntr_get(sys_logic_port_t port, sys_mib_debug_portCtr_type_t type, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((type >= DEBUG_PORT_TYPE_END), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_debug_portCntr_get(port, type, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_txAbortPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    uint64 tmpResult;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    tmpResult = 0;
    SYS_ERR_CHK(sal_mib_dot3StatsExcessiveCollisions_get(port, &tmpResult));
    *pResult = tmpResult;

    return SYS_ERR_OK;
}

int32 sal_mib_collisionPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    uint64 tmpResult;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    tmpResult = 0;
    SYS_ERR_CHK(sal_mib_etherStatsCollisions_get(port, &tmpResult));
    *pResult = tmpResult;

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsDropEvents_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsDropEvents_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsOctets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsOctets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_cpuStatsPkts_get(uint64 *pResult)
{
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_cpuStatsPkts_get(pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsBcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsBcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsMcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsMcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsCRCAlignErrs_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsCRCAlignErrs_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsUnderSizePkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsUnderSizePkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsOverSizePkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsOverSizePkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsFragments_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsFragments_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsJabbers_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsJabbers_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsCollisions_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsCollisions_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsPkts64Octets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsPkts64Octets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsPkts65to127Octets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsPkts65to127Octets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsPkts128to255Octets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsPkts128to255Octets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsPkts256to511Octets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsPkts256to511Octets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsPkts512to1023Octets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsPkts512to1023Octets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_etherStatsPkts1024to1518Octets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_etherStatsPkts1024to1518Octets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot1dTpLearnedEntryDiscards_get(uint64 *pResult)
{
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot1dTpLearnedEntryDiscards_get(pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot1dTpPortInDiscards_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot1dTpPortInDiscards_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3_statsAlignmentErrors_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3_status_alignment_errors_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3StatsFCSErrors_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3StatsFCSErrors_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3StatsSingleCollisionFrames_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3StatsSingleCollisionFrames_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3StatsMultipleCollisionFrames_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3StatsMultipleCollisionFrames_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3StatsDeferredTransmissions_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3StatsDeferredTransmissions_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3StatsLateCollisions_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3StatsLateCollisions_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3StatsExcessiveCollisions_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3StatsExcessiveCollisions_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3StatsSymbolErrors_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3StatsSymbolErrors_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3StatsFrameTooLongs_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3StatsFrameTooLongs_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3ControlInUnknownOpcodes_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3ControlInUnknownOpcodes_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3OutPauseFrames_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3OutPauseFrames_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3InPauseFrames_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_dot3InPauseFrames_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_dot3StatsInternalMacReceiveErrors_get(sys_logic_port_t port, uint64 *pResult)
{
    uint64 cntr = 0;

    *pResult = cntr;

    return SYS_ERR_OK;
}

int32 sal_mib_ifInOctets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifInOctets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifInUcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifInUcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifInNUcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifInNUcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifInDiscards_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifInDiscards_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifInErrors_get(sys_logic_port_t port, uint64 *pResult)
{
    uint64 cntrAligError = 0;
    uint64 cntrFcsError = 0;
    uint64 cntrFragment = 0;
    uint64 cntrJabbers = 0;
    uint64 cntrFrameToolong = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_mib_dot3_statsAlignmentErrors_get(port, &cntrAligError));
    SYS_ERR_CHK(sal_mib_dot3StatsFCSErrors_get(port, &cntrFcsError));
    SYS_ERR_CHK(sal_mib_etherStatsFragments_get(port,&cntrFragment));
    SYS_ERR_CHK(sal_mib_etherStatsJabbers_get(port,&cntrJabbers));
    SYS_ERR_CHK(sal_mib_dot3StatsFrameTooLongs_get(port,&cntrFrameToolong));

    *pResult = cntrAligError + cntrFcsError +cntrFragment + cntrJabbers + cntrFrameToolong;

    return SYS_ERR_OK;
}

int32 sal_mib_ifOutOctets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifOutOctets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifOutUcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifOutNUcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifOutNUcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifOutDiscards_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifOutDiscards_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifOutErrors_get(sys_logic_port_t port, uint64 *pResult)
{
    uint64 ccntr64 = 0;
    uint64 exCcntr64 = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_mib_dot3StatsLateCollisions_get(port, &ccntr64));
    SYS_ERR_CHK(sal_mib_dot3StatsExcessiveCollisions_get(port, &exCcntr64));

    *pResult = ccntr64 + exCcntr64;

    return SYS_ERR_OK;
}

int32 sal_mib_ifOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifOutMcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifOutBcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifInMulticastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifInMulticastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifInBroadcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifInBroadcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifOutQLen_get(sys_logic_port_t port, uint64 *pResult)
{
    uint64 ucntr64 = 0;
    uint64 nucntr64 = 0;
    uint64 dcntr64 = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_mib_ifOutUcastPkts_get(port, &ucntr64));
    SYS_ERR_CHK(sal_mib_ifOutNUcastPkts_get(port, &nucntr64));
    SYS_ERR_CHK(sal_mib_ifOutDiscards_get(port, &dcntr64));

    *pResult = ucntr64 + nucntr64 + dcntr64;

    return SYS_ERR_OK;
}

int32 sal_mib_ifHCInOctets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifHCInOctets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifHCInUcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifHCInUcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifHCInMulticastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifHCInMulticastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifHCInBroadcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifHCInBroadcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifHCOutOctets_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifHCOutOctets_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifHCOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifHCOutUcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifHCOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifHCOutMcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_ifHCOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_mib_ifHCOutBcastPkts_get(port, pResult));

    return SYS_ERR_OK;
}

int32 sal_mib_privateInErr_get(sys_logic_port_t port, uint64 *pResult)
{
    uint64 inRunts = 0;
    uint64 inGiants = 0;
    uint64 inDiscards = 0;
    uint64 inThrottles = 0;
    uint64 inCRCs = 0;
    uint64 inFrames = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_mib_etherStatsUnderSizePkts_get(port, &inRunts));
    SYS_ERR_CHK(sal_mib_etherStatsOverSizePkts_get(port, &inGiants));
    SYS_ERR_CHK(sal_mib_dot1dTpPortInDiscards_get(port, &inDiscards));
    SYS_ERR_CHK(sal_mib_etherStatsDropEvents_get(port, &inThrottles));
    SYS_ERR_CHK(sal_mib_dot3StatsFCSErrors_get(port, &inCRCs));
    SYS_ERR_CHK(sal_mib_dot3_statsAlignmentErrors_get(port, &inFrames));
    *pResult = inRunts + inGiants + inThrottles + inDiscards + inCRCs + inFrames;

    return SYS_ERR_OK;
}
