
#ifndef __SAL_ISG_H__
#define __SAL_ISG_H__

#include <isg/proto_isg.h>
#include <libsal/sal_time.h>

#define ISG_DB_BACKUP_PREFIX       "/mnt/isg_%s"

typedef struct sys_isg_db_statistics_s
{
    sys_time_str_t lastSuccessedTime;
    sys_time_str_t lastFailedTime;
    char lastFaildReason[SYS_BUF128_LEN];
    uint32 totalAttempts;
    uint32 successfulTransfer;
    uint32 successfulReads;
    uint32 successfulWrites;
    uint32 startupFailures;
    uint32 failedTransfer;
    uint32 failedReads;
    uint32 failedWrites;

    uint8 isRunning;
    uint32 startTime;
} sys_isg_db_statistics_t;

#define ISG_DB_BACKUP()                 \
do{                                     \
    SYS_ERR_CHK(isg_db_timer_set(1));   \
}while(0);

#define ISG_DB_RESTORE()                \
do{                                     \
    char __cmd[SYS_BUF128_LEN];         \
    SYS_MEM_CLEAR(__cmd);               \
    osal_sprintf(__cmd, "handler -i isgdb_restore &");  \
    system(__cmd);                                      \
}while(0);

extern int32 sal_isg_port_set(sys_logic_port_t port, sys_enable_t enable, sys_isg_verify_type_t verifyType);

extern int32 sal_isg_port_get(sys_logic_port_t port, sys_enable_t *pEnable, sys_isg_verify_type_t *pVerifyType);

extern int32 sal_isg_portMaxEntryNum_set(sys_logic_port_t portId, int32 num);

extern int32 sal_isg_portMaxEntryNum_get(sys_logic_port_t port, int32 *pNum);

extern int32 sal_isg_portCurrentEntryNum_get(sys_logic_port_t port, uint32 *pNum);

extern int32 sal_isg_bind_entry_set(sys_isg_entry_t *pBindTblEntry);

extern int32 sal_isg_bind_entry_del(sys_isg_entry_t *pBindTblEntry);

extern int32 sal_isg_bind_entry_get(uint16 entryIdx, sys_isg_entry_t *pBindTblEntry);

extern int32 sal_isg_db_get(sys_isg_db_t *pDB);

extern int32 sal_isg_db_set(sys_isg_db_t *pDB);

extern int32 sal_isg_db_clear(void);

extern int32 sal_isg_db_statistics_get(sys_isg_db_statistics_t *pDBS);

extern int32 sal_isg_db_statistics_set(sys_isg_db_statistics_t *pDBS);

extern int32 sal_isg_db_timer_get(uint32 *pTimer);

extern int32 sal_isg_db_timer_set(uint32 timer);

extern int32 sal_isg_bind_entry_backup(void);

extern int32 sal_isg_bind_entry_restore(void);

#endif

