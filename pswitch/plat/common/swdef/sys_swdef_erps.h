
#ifndef __SYS_SWDEF_ERPS_H__
#define __SYS_SWDEF_ERPS_H__
#define CAPA_ERPS_MST_INSTANCE_NUM               16      
#define BOND_ERPS_HELLO_TIME_MIN                 1
#define BOND_ERPS_HELLO_TIME_MAX                 10
#define BOND_ERPS_MST_INSTANCE_MIN               0
#define BOND_ERPS_MST_INSTANCE_MAX               15
#define DFLT_ERPS_EBL                           DEFDB_VAL(DEF_ERPS_EBL)
#define DFLT_LOOPBACK_EBL                       DEFDB_VAL(DEF_LOOPBACK_EBL)
#define DFLT_STP_BPDU_FORWARD_EBL               DEFDB_VAL(DEF_STP_BPDU_FORWARD_EBL)
#define DFLT_STP_FORCE_VERSION                  DEFDB_VAL(DEF_STP_FORCE_VERSION)
#define DFLT_STP_HELLO_TIME                     2
#define DFLT_STP_FORWARD_DELAY                  15
#define DFLT_STP_MAX_AGE                        20
#define DFLT_STP_TX_HOLD_COUNT                  6
#define DFLT_STP_PATHCOST_METHOD                STP_PATHCOST_METHOD_LONG
#define DFLT_STP_PRI                            32768

#define DFLT_STP_PORT_EBL                       DEFDB_VAL(DEF_STP_PORT_EBL)
#define DFLT_STP_PORT_BPDU_FORWARD_EBL          DEFDB_VAL(DEF_STP_PORT_BPDU_FORWARD_EBL)
#define DFLT_STP_PORT_EDGE                      DEFDB_VAL(DEF_STP_PORT_EDGE)
#define DFLT_STP_PORT_P2P                       DEFDB_VAL(DEF_STP_PORT_P2P)
#define DFLT_STP_PORT_PATHCOST                  0
#define DFLT_STP_PORT_PRI                       128

#ifdef CONFIG_SYS_STP_BPDU_FILTER
#define DFLT_STP_PORT_BPDU_FILTER               DEFDB_VAL(DEF_STP_PORT_BPDU_FILTER)
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD
#define DFLT_STP_PORT_BPDU_GUARD                DEFDB_VAL(DEF_STP_PORT_BPDU_GUARD)
#endif

#ifdef CONFIG_SYS_PROTO_STP_MSTP
#define DFLT_STP_MAX_HOPS                       20
#define DFLT_STP_MST_CFG_NAME                   ""
#define DFLT_STP_MST_CFG_REV                    0
#define DFLT_STP_MST_PRI                        32768
#define DFLT_STP_MST_PORT_PATHCOST              0
#define DFLT_STP_MST_PORT_PRI                   128
#endif

#endif 

