
#ifndef __SYS_SWDEF_STP_H__
#define __SYS_SWDEF_STP_H__
#define CAPA_STP_MST_INSTANCE_NUM               16      
#define CAPA_STP_MST_CFG_NAME_LEN               32
#define BOND_STP_HELLO_TIME_MIN                 1
#define BOND_STP_HELLO_TIME_MAX                 10
#define BOND_STP_FORWARD_DELAY_MIN              4
#define BOND_STP_FORWARD_DELAY_MAX              30
#define BOND_STP_MAX_AGE_MIN                    6
#define BOND_STP_MAX_AGE_MAX                    40
#define BOND_STP_TX_HOLD_COUNT_MIN              1
#define BOND_STP_TX_HOLD_COUNT_MAX              10
#define BOND_STP_PATHCOST_LONG_MAX              200000000
#define BOND_STP_PATHCOST_SHORT_MAX             65535
#define BOND_STP_PRI_MAX                        61440
#define BOND_STP_PORT_PRI_MAX                   240
#ifdef CONFIG_SYS_PROTO_STP_MSTP
#define BOND_STP_MST_INSTANCE_MAX               15      
#define BOND_STP_MST_CFG_REV_MAX                65535
#define BOND_STP_MAX_HOPS_MIN                   1
#define BOND_STP_MAX_HOPS_MAX                   40
#endif
#define DFLT_STP_EBL                            DEFDB_VAL(DEF_STP_EBL)
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

