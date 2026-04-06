
#ifndef __SYS_SWDEF_ACL_H__
#define __SYS_SWDEF_ACL_H__
#ifdef CONFIG_SYS_USER_DEFINED_ACL

#define BOND_ACL_USER_PRI_MIN               1
#define BOND_ACL_USER_PRI_MAX               2147483647
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
#define DFLT_ACL_BLK_PRI_MAC                0 
#define DFLT_ACL_BLK_PRI_IPV6               1 
#define DFLT_ACL_BLK_PRI_IP                 2 
#endif

#ifdef CONFIG_SYS_L3_ROUTE
#define DFLT_ACL_BLK_PRI_L3_ROUTE           3 
#define DFLT_ACL_BLK_PRI_L3_NROUTE          4 
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
#define DFLT_ACL_BLK_PRI_SURVEILLANCEVLAN   6 
#endif
#ifdef CONFIG_SYS_VOICEVLAN
#define DFLT_ACL_BLK_PRI_VOICEVLAN          7 
#endif
#ifdef CONFIG_SYS_MACVLAN
#define DFLT_ACL_BLK_PRI_MACVLAN            8  
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
#define DFLT_ACL_BLK_PRI_PROTOVLAN          9  
#endif
#ifdef CONFIG_SYS_PROTO_ISG
#define DFLT_ACL_BLK_PRI_IPSG               13 
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
#define DFLT_ACL_BLK_PRI_AUTH               14 
#endif
#define DFLT_ACL_BLK_PRI_SYS                15 

#ifdef CONFIG_SYS_MGMT_ACL

#define DFLT_ACL_MGMT_PROFILE               DEFDB_STR(DEF_ACL_MGMT_PROFILE)
#endif

#ifdef CONFIG_SYS_USER_DEFINED_ACL

#define DFLT_ACL_USER_ACE_PRI_GAP           20
#endif
#endif 

