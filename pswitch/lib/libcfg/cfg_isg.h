
#ifndef __CFG_ISG_H__
#define __CFG_ISG_H__

#include <libsal/sal_isg.h>
#include <isg/proto_isg.h>

typedef struct cfg_isg_s
{
    int32 portMaxEntryNum[SYS_LOGICPORT_NUM_MAX]; 
    uint32 maxEntryNum;
    sys_enable_t portEnable[SYS_LOGICPORT_NUM_MAX];
    sys_isg_verify_type_t portVerifyType[SYS_LOGICPORT_NUM_MAX];
    sys_isg_db_t db;
    sys_isg_db_statistics_t dbStatistics;
} cfg_isg_t;

extern int32 cfg_isg_port_set(sys_logic_port_t port, sys_enable_t enable, sys_isg_verify_type_t verifyType);

extern int32 cfg_isg_port_get(sys_logic_port_t port, sys_enable_t *pEnable, sys_isg_verify_type_t *pVerifyType);

extern int32 cfg_isg_portMaxEntryNum_set(sys_logic_port_t port, int32 num);

extern int32 cfg_isg_portMaxEntryNum_get(sys_logic_port_t port, int32 *pNum);

extern int32 cfg_isg_maxEntryNum_get(uint32 *pMaxEntryNum);

extern int32 cfg_isg_maxEntryNum_set(uint32 maxEntryNum);

extern int32 cfg_isg_db_get(sys_isg_db_t *pDB);

extern int32 cfg_isg_db_set(sys_isg_db_t *pDB);

extern int32 cfg_isg_db_statistics_get(sys_isg_db_statistics_t *pDBS);

extern int32 cfg_isg_db_statistics_set(sys_isg_db_statistics_t *pDBS);
#endif 

