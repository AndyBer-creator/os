
#ifndef __RSD_MIB_H__
#define __RSD_MIB_H__

#include <rtk/stat.h>
#include <common/sys_type.h>

typedef struct sys_mib_allPorts_s
{
    uint64 cntr[SYS_LOGICPORT_NUM_MAX];
} sys_mib_allPorts_t;

typedef struct sys_mib_allTrunks_s
{
    uint64 cntr[CAPA_LAG_NUM_MAX];
} sys_mib_allTrunks_t;

typedef enum sys_mib_debug_globalCtr_type_e
{
    
    DEBUG_MIB_GET_ALE_TX_GOOD_PKT_CNT,
    DEBUG_MIB_GET_CFI_PKT_CNT,
    DEBUG_MIB_GET_RMA_PKT_CNT,
    DEBUG_MIB_GET_NEW_SA_PKT_CNT,
    DEBUG_MIB_GET_PORT_MV_PKT_CNT,
    DEBUG_MIB_GET_SA_BLOCK_PKT_CNT,
    DEBUG_MIB_GET_DA_BLOCK_PKT_CNT,
    DEBUG_MIB_GET_ROUTE_EXCEPT_PKT_CNT,
    DEBUG_MIB_GET_PORT_ISO_PKT_CNT,

    DEBUG_MIB_GET_MAC_RX_DROP_PKT_CNT,
    DEBUG_MIB_GET_ACL_FWD_PKT_CNT,
    DEBUG_MIB_GET_ATTACK_PKT_CNT,
    DEBUG_MIB_GET_VLAN_IGR_PKT_CNT,
    DEBUG_MIB_GET_MAC_LIMIT_SYS_PKT_CNT,
    DEBUG_MIB_GET_MAC_LIMIT_VLAN_PKT_CNT,
    DEBUG_MIB_GET_MAC_LIMIT_PORT_PKT_CNT,
    DEBUG_MIB_GET_SWITCH_MAC_PKT_CNT,
    DEBUG_MIB_GET_DA_LK_MISS_PKT_CNT,
    DEBUG_MIB_GET_RSPAN_PKT_CNT,
    DEBUG_MIB_GET_ACL_LK_MISS_PKT_CNT,
    DEBUG_MIB_GET_ACL_DROP_PKT_CNT,
    DEBUG_MIB_GET_ING_BANDWIDTH_CONTROL_PKT_CNT,
    DEBUG_MIB_GET_ACL_METER_PKT_CNT,
    DEBUG_MIB_GET_ACCPET_FRAME_TYPE_PKT_CNT,
    DEBUG_MIB_GET_STP_IGR_PKT_CNT,
    DEBUG_MIB_GET_INVALID_SA_PKT_CNT,
    DEBUG_MIB_GET_L2_INVALID_DPM_PKT_CNT,
    DEBUG_MIB_GET_MCSTTBL_INVALID_DPM_PKT_CNT,
    DEBUG_MIB_GET_RX_FLOW_CONTROL_PKT_CNT,
    DEBUG_MIB_GET_STORM_SPPRS_PKT_CNT,
    DEBUG_MIB_GET_LINK_AGGR_LOAD_SHARING_PKT_CNT,
    DEBUG_MIB_GET_VLAN_EGR_PKT_CNT,
    DEBUG_MIB_GET_STP_EGR_PKT_CNT,
    DEBUG_MIB_GET_SRC_PORT_FILTER_PKT_CNT,
    DEBUG_MIB_GET_ACL_FILTER_PKT_CNT,
    DEBUG_MIB_GET_MIR_FILTER_PKT_CNT,
    DEBUG_MIB_GET_TX_MAX_LEN_PKT_CNT,
    DEBUG_MIB_GET_LINK_DOWN_DIS_TX_PKT_CNT,
    DEBUG_MIB_GET_FLOW_CONTROL_PKT_CNT,

    DEBUG_MIB_GET_ERR_PKT_CNT,
    DEBUG_MIB_GET_EGR_ACL_DROP_PKT_CNT,
    DEBUG_MIB_GET_EGR_METER_DROP_PKT_CNT,
    DEBUG_MIB_GET_OAM_PARSING_DROP_PKT_CNT,
    DEBUG_MIB_GET_CFM_PKT_CNT,
    DEBUG_MIB_GET_VLAN_IGR_FILTER_PKT_CNT,
    DEBUG_MIB_GET_VLAN_ERR_PKT_CNT,
    DEBUG_MIB_GET_VLAN_TAG_FORMAT_PKT_CNT,
    DEBUG_MIB_GET_SRC_PORT_SPT_PKT_CNT,
    DEBUG_MIB_GET_IGR_BANDWIDTH_CTRL_PKT_CNT,
    DEBUG_MIB_GET_ATTACK_PREVENT_PKT_CNT,
    DEBUG_MIB_GET_PROTO_STORM_PKT_CNT,
    DEBUG_MIB_GET_MCAST_SA_PKT_CNT,
    DEBUG_MIB_GET_IGR_ACL_DROP_PKT_CNT,
    DEBUG_MIB_GET_IGR_METER_DROP_PKT_CNT,
    DEBUG_MIB_GET_MISS_ACL_AND_C2SC_DROP_PKT_CNT,
    DEBUG_MIB_GET_SRC_PORT_SPT_NON_FORWARD_PKT_CNT,
    DEBUG_MIB_GET_MAC_LIMIT_CNT,
    DEBUG_MIB_GET_UNKNOW_STORM_CNT,
    DEBUG_MIB_GET_MISS_DROP_CNT,
    DEBUG_MIB_GET_CPU_MAC_DROP_CNT,
    DEBUG_MIB_GET_SRC_PORT_FILTER_BFR_EGR_ACL_PKT_CNT,
    DEBUG_MIB_GET_VLAN_EGR_FILTER_PKT_CNT,
    DEBUG_MIB_GET_STP_PKT_CNT,
    DEBUG_MIB_GET_OAM_MASK_DROP_PKT_CNT,
    DEBUG_MIB_GET_MIR_ISO_PKT_CNT,
    DEBUG_MIB_GET_MAX_LEN_BFR_EGR_ACL_PKT_CNT,
    DEBUG_MIB_GET_SRC_PORT_FILTER_BFR_MIR_PKT_CNT,
    DEBUG_MIB_GET_MAX_LEN_BFR_MIR_PKT_CNT,
    DEBUG_MIB_GET_SPECIAL_CONGEST_BFR_MIR,
    DEBUG_MIB_GET_LINK_STATUS_BFR_MIR,
    DEBUG_MIB_GET_WTD_WRED_BFR_MIR,
    DEBUG_MIB_GET_MAX_LEN_AFTR_MIR_PKT_CNT,
    DEBUG_MIB_GET_SPECIAL_CONGEST_AFTR_MIR,
    DEBUG_MIB_GET_LINK_STATUS_AFTR_MIR,
    DEBUG_MIB_GET_WTD_WRED_AFTR_MIR,

    DEBUG_TYPE_END,
} sys_mib_debug_globalCtr_type_t;

typedef enum sys_mib_debug_portCtr_type_e
{
    DEBUG_PORT_TYPE_END,
} sys_mib_debug_portCtr_type_t;

extern int32 rsd_mib_init(void);

extern int32 rsd_mib_dot1dTpLearnedEntryDiscards_get(uint64 *pResult);

extern int32 rsd_mib_dot1dTpPortInDiscards_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_cntr_clr(sys_logic_port_t port);

extern int32 rsd_mib_cntrGlobal_clr(void);

extern int32 rsd_mib_cntrDebug_clr(void);

extern int32 rsd_mib_etherStatsDropEvents_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsBcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsMcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsCRCAlignErrs_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsUnderSizePkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsOverSizePkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsFragments_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsJabbers_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsCollisions_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsPkts64Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsPkts65to127Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsPkts128to255Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsPkts256to511Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsPkts512to1023Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_etherStatsPkts1024to1518Octets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifOutNUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifOutOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3StatsFCSErrors_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3StatsSingleCollisionFrames_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3StatsMultipleCollisionFrames_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3StatsDeferredTransmissions_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3StatsLateCollisions_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3StatsExcessiveCollisions_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3StatsSymbolErrors_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3StatsFrameTooLongs_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifInDiscards_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifOutDiscards_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifInOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifInUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifInNUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifInMulticastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifInBroadcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifOutOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifOutNUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifHCInOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifHCInUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifHCInMulticastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifHCInBroadcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifHCOutOctets_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifHCOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifHCOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_ifHCOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3ControlInUnknownOpcodes_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3OutPauseFrames_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3InPauseFrames_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_dot3_status_alignment_errors_get(sys_logic_port_t port, uint64 *pResult);

extern int32 rsd_mib_debug_globalCntr_get(sys_mib_debug_globalCtr_type_t type, uint64 *pResult);

extern int32 rsd_mib_debug_portCntr_get(sys_logic_port_t port, sys_mib_debug_portCtr_type_t type, uint64 *pResult);

extern int32 rsd_mib_cpuStatsPkts_get(uint64 *pResult);

#ifdef CONFIG_SYS_RTL9310
extern int32 rsd_mango_mib_init(void);
extern int32 rsd_mango_mib_debug_globalCntr_get(sys_mib_debug_globalCtr_type_t type, uint64 *pResult);
extern int32 rsd_mango_mib_debug_portCntr_get(sys_logic_port_t port, sys_mib_debug_portCtr_type_t type, uint64 *pResult);
extern int32 rsd_mango_mib_cntrDebug_clr(void);
extern int32 rsd_mango_mib_dot3_status_alignment_errors_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_mango_mib_dot3StatsFCSErrors_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_mango_mib_dot3StatsFrameTooLongs_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_mango_mib_etherStatsOctets_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_mango_mib_etherStatsCRCAlignErrs_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_mango_mib_etherStatsPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_etherStatsBcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_etherStatsMcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_etherStatsPkts64Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_etherStatsPkts65to127Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_etherStatsPkts128to255Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_etherStatsPkts256to511Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_etherStatsPkts512to1023Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_etherStatsPkts1024to1518Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_ifInUcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_mango_mib_ifOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_mango_mib_ifOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_mango_mib_ifOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult);
#endif

#ifdef CONFIG_SYS_RTL9300 
extern int32 rsd_longan_mib_init(void);
extern int32 rsd_longan_mib_debug_globalCntr_get(sys_mib_debug_globalCtr_type_t type, uint64 *pResult);
extern int32 rsd_longan_mib_debug_portCntr_get(sys_logic_port_t port, sys_mib_debug_portCtr_type_t type, uint64 *pResult);
extern int32 rsd_longan_mib_cntrDebug_clr(void);
extern int32 rsd_longan_mib_dot3_status_alignment_errors_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_longan_mib_dot3StatsFCSErrors_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_longan_mib_dot3StatsFrameTooLongs_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_longan_mib_etherStatsOctets_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_longan_mib_etherStatsCRCAlignErrs_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_longan_mib_etherStatsPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_etherStatsBcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_etherStatsMcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_etherStatsPkts64Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_etherStatsPkts65to127Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_etherStatsPkts128to255Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_etherStatsPkts256to511Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_etherStatsPkts512to1023Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_etherStatsPkts1024to1518Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_ifInUcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_longan_mib_ifOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_longan_mib_ifOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_longan_mib_ifOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult);
#endif

#ifdef CONFIG_SYS_RTL8390
extern int32 rsd_cypress_mib_init(void);
extern int32 rsd_cypress_mib_debug_globalCntr_get(sys_mib_debug_globalCtr_type_t type, uint64 *pResult);
extern int32 rsd_cypress_mib_debug_portCntr_get(sys_logic_port_t port, sys_mib_debug_portCtr_type_t type, uint64 *pResult);
extern int32 rsd_cypress_mib_cntrDebug_clr(void);
extern int32 rsd_cypress_mib_dot3_status_alignment_errors_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_cypress_mib_dot3StatsFCSErrors_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_cypress_mib_dot3StatsFrameTooLongs_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_cypress_mib_etherStatsOctets_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_cypress_mib_etherStatsCRCAlignErrs_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_cypress_mib_etherStatsPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_etherStatsBcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_etherStatsMcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_etherStatsPkts64Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_etherStatsPkts65to127Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_etherStatsPkts128to255Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_etherStatsPkts256to511Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_etherStatsPkts512to1023Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_etherStatsPkts1024to1518Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_ifInUcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_cypress_mib_ifOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_cypress_mib_ifOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_cypress_mib_ifOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult);
#endif

#ifdef CONFIG_SYS_RTL8380
extern int32 rsd_maple_mib_init(void);
extern int32 rsd_maple_mib_debug_globalCntr_get(sys_mib_debug_globalCtr_type_t type, uint64 *pResult);
extern int32 rsd_maple_mib_debug_portCntr_get(sys_logic_port_t port, sys_mib_debug_portCtr_type_t type, uint64 *pResult);
extern int32 rsd_maple_mib_cntrDebug_clr(void);
extern int32 rsd_maple_mib_dot3_status_alignment_errors_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_maple_mib_dot3StatsFCSErrors_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_maple_mib_dot3StatsFrameTooLongs_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_maple_mib_etherStatsOctets_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_maple_mib_etherStatsCRCAlignErrs_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_maple_mib_etherStatsPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_etherStatsBcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_etherStatsMcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_etherStatsPkts64Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_etherStatsPkts65to127Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_etherStatsPkts128to255Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_etherStatsPkts256to511Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_etherStatsPkts512to1023Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_etherStatsPkts1024to1518Octets_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_ifInUcastPkts_get(sys_logic_port_t port, uint64 * pResult);
extern int32 rsd_maple_mib_ifOutUcastPkts_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_maple_mib_ifOutMcastPkts_get(sys_logic_port_t port, uint64 *pResult);
extern int32 rsd_maple_mib_ifOutBcastPkts_get(sys_logic_port_t port, uint64 *pResult);
#endif

#endif 

