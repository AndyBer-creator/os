
#ifndef __SYS_SWDEF_LAG_H__
#define __SYS_SWDEF_LAG_H__
#define CAPA_LAG_NAME_LEN                   64
#define CAPA_LAG_NUM                        DEFDB_VAL(DEF_LAG_NUM) 
#define CAPA_LAG_NUM_MAX                    16                     
#define CAPA_LAG_MBR_NORMAL_NUM             8
#define CAPA_LAG_MBR_DUMB_NUM               8
#define DFLT_LAG_HASH_ALGO                  DEFDB_VAL(DEF_LAG_HASH_ALGO)
#define DFLT_LAG_NUM                        DEFDB_VAL(DEF_LAG_NUM)

#ifdef CONFIG_SYS_PROTO_LACP
#define DFLT_LACP_EBL                       DEFDB_VAL(DEF_LACP_EBL)
#define DFLT_LACP_SYSTEM_PRIORITY           32768
#define DFLT_LACP_PORT_PRIORITY             1
#define DFLT_LACP_PORT_TIMEOUT              DEFDB_VAL(DEF_LACP_PORT_TIMEOUT)
#endif

#endif 

