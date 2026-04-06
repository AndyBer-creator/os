/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 16227 $
 * $Date: 2011-03-10 18:23:52 +0800 (Thu, 10 Mar 2011) $
 *
 * Purpose : Process special event which can't be processed in normal thread
 *
 * Feature : Process special event which can't be processed in normal thread
 */

/*
 * Include Files
 */
#include <common/sys_util.h>
#include <libsal/sal_util.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */
#define DBS_FAILD_UPDATE(dbs, reason, isR)   \
do{                                     \
    dbs.failedTransfer++;               \
    if (isR) dbs.failedReads++;         \
    else dbs.failedWrites++;            \
    SYS_MEM_CLEAR(dbs.lastFailedTime);  \
    SYS_MEM_CLEAR(dbs.lastFaildReason); \
    sal_time_currentTimeStr_get(&dbs.lastFailedTime);   \
    sprintf(dbs.lastFaildReason, "%s", reason);        \
    sal_isg_db_statistics_set(&dbs);   \
}while(0);

#define DBS_SUCCESS_UPDATE(dbs, isR)    \
do{                                     \
    dbs.successfulTransfer++;           \
    if (isR) dbs.successfulReads++;     \
    else dbs.successfulWrites++;        \
    SYS_MEM_CLEAR(dbs.lastSuccessedTime);  \
    sal_time_currentTimeStr_get(&dbs.lastSuccessedTime);   \
    sal_isg_db_statistics_set(&dbs);      \
}while(0);
/*
 * Function Declaration
 */

int32 handleIpsgDBBackup(void)
{
    FILE *fp = NULL;
    char cmd[SYS_BUF128_LEN];
    char tmpFile[SYS_BUF128_LEN];
    char bkFile[SYS_BUF128_LEN];
    uint32 i = 0;
    uint32 tmpTime = 0;
    sys_isg_entry_t entry;
    sys_isg_db_t db;
    sys_isg_db_statistics_t dbs;
    sys_isg_entry_type_t filterType = ISG_ENTRY_DYNAMIC;

    SYS_MEM_CLEAR(db);
    SYS_MEM_CLEAR(dbs);
    SYS_MEM_CLEAR(cmd);
    SYS_MEM_CLEAR(tmpFile);
    SYS_MEM_CLEAR(bkFile);

    SYS_ERR_CHK(sal_isg_db_get(&db));
    SYS_ERR_CHK(sal_isg_db_statistics_get(&dbs));

    if (ISG_DB_TYPE_NONE == db.dbType)
        return SYS_ERR_OK;

    if (TRUE == dbs.isRunning)
        return SYS_ERR_OK;

    dbs.totalAttempts++;

    SYS_ERR_CHK(isg_db_timer_set(0));

    osal_sprintf(tmpFile, "/tmp/%s", db.filename);
    osal_sprintf(bkFile, ISG_DB_BACKUP_PREFIX, db.filename);

    fp = fopen(tmpFile, "w+");

    if (NULL == fp)
    {
        DBS_FAILD_UPDATE(dbs, "Binding database write file failed.", FALSE);

        SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_WRITE_FAIL);

        SYS_ERR_CHK(isg_db_timer_set(db.writeDelay));

        return SYS_ERR_FAILED;
    }

    for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
    {
        SYS_MEM_CLEAR(entry);
        SYS_ERR_CONTINUE(sal_isg_bind_entry_get(i, &entry));

        if (!entry.isUsed)
            continue;

        if ((filterType != ISG_ENTRY_END) && (filterType != entry.type))
            continue;

        if (ISG_ENTRY_ALL_DENY == entry.type)
            continue;

        fwrite(&entry, sizeof(sys_isg_entry_t), 1, fp);
    }

    fflush(fp);
    fclose(fp);

    if (ISG_DB_TYPE_FLASH == db.dbType)
    {
        osal_sprintf(cmd, "mv %s %s", tmpFile , bkFile);
        system(cmd);
    }
    else if (ISG_DB_TYPE_TFTP == db.dbType)
    {
        osal_sprintf(cmd, "tftp -p -l \"%s\" -r \"%s\" %s 1>> /dev/null 2>> /dev/null", tmpFile, db.filename, db.server);
        dbs.isRunning = TRUE;
        SYS_ERR_CHK(osal_time_sysUptime_get(&dbs.startTime));
        SYS_ERR_CHK(sal_isg_db_statistics_set(&dbs));

        while(0 != system(cmd))
        {
            SYS_ERR_CHK(sal_isg_db_get(&db));

            if (ISG_DB_TYPE_TFTP != db.dbType)
            {
                dbs.isRunning = FALSE;
                dbs.startTime = 0;
                DBS_FAILD_UPDATE(dbs, "Unable to access host", FALSE);
                SYS_ERR_CHK(isg_db_timer_set(db.writeDelay));
                SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_UPLOAD_FAIL, db.server);

                return SYS_ERR_FAILED;
            }
            else
            {
                SYS_MEM_CLEAR(cmd);
                SYS_MEM_CLEAR(tmpFile);
                osal_sprintf(tmpFile, "/tmp/%s", db.filename);
                osal_sprintf(cmd, "tftp -p -l \"%s\" -r \"%s\" %s 1>> /dev/null 2>> /dev/null", tmpFile, db.filename, db.server);
            }

            SYS_ERR_CHK(osal_time_sysUptime_get(&tmpTime));

            if ((tmpTime - dbs.startTime) > (db.timeout * 1000))
            {
                /* Never stop if timeout is zero */
                if (0 == db.timeout)
                {
                    dbs.totalAttempts++;
                    DBS_FAILD_UPDATE(dbs, "Unable to access host", FALSE);
                    SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_UPLOAD_FAIL, db.server);
                    continue;
                }
                else
                {
                    dbs.isRunning = FALSE;
                    dbs.startTime = 0;
                    DBS_FAILD_UPDATE(dbs, "Unable to access host", FALSE);
                    SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_UPLOAD_FAIL, db.server);
                    SYS_ERR_CHK(isg_db_timer_set(db.writeDelay));
                }

                return SYS_ERR_FAILED;
            }
        }

        SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_UPLOAD_SUCCESS, db.server);
    }

    dbs.isRunning = FALSE;
    dbs.startTime = 0;
    DBS_SUCCESS_UPDATE(dbs, FALSE);

    return SYS_ERR_OK;
}

int32 handleIpsgDBRestore(void)
{
    FILE *fp = NULL;
    char bkFile[SYS_BUF256_LEN];
    char cmd[SYS_BUF1024_LEN];
    uint32 tmpTime = 0;
    sys_isg_entry_t entry;
    sys_isg_db_t db;
    sys_isg_db_statistics_t dbs;

    SYS_MEM_CLEAR(dbs);
    SYS_MEM_CLEAR(db);
    SYS_MEM_CLEAR(bkFile);
    SYS_ERR_CHK(sal_isg_db_get(&db));
    SYS_ERR_CHK(sal_isg_db_statistics_get(&dbs));

    if (ISG_DB_TYPE_NONE == db.dbType)
        return SYS_ERR_OK;

    if (TRUE == dbs.isRunning)
        return SYS_ERR_OK;

    dbs.totalAttempts++;

    if (ISG_DB_TYPE_FLASH == db.dbType)
        osal_sprintf(bkFile, ISG_DB_BACKUP_PREFIX, db.filename);
    else if (ISG_DB_TYPE_TFTP == db.dbType)
    {
        osal_sprintf(bkFile, "/tmp/%s", db.filename);
        osal_sprintf(cmd, "tftp -g -l \"%s\" -r \"%s\" %s 1>> /dev/null 2>> /dev/null", bkFile, db.filename, db.server);

        dbs.isRunning = TRUE;
        SYS_ERR_CHK(osal_time_sysUptime_get(&dbs.startTime));
        SYS_ERR_CHK(sal_isg_db_statistics_set(&dbs));

        while(0 != system(cmd))
        {
            SYS_ERR_CHK(sal_isg_db_get(&db));
            SYS_ERR_CHK(osal_time_sysUptime_get(&tmpTime));
            sleep(5);
            if ((0 == db.timeout) || (tmpTime - dbs.startTime) > (db.timeout * 1000))
            {
                dbs.isRunning = FALSE;
                dbs.startTime = 0;
                DBS_FAILD_UPDATE(dbs, "Unable to access host", TRUE);
                SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_DOWNLOAD_FAIL, db.server);

                return SYS_ERR_FAILED;
            }
        }

        SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_DOWNLOAD_SUCCESS, db.server);
    }

    fp = fopen(bkFile, "r");

    if (NULL == fp)
    {
        dbs.isRunning = FALSE;
        dbs.startTime = 0;
        DBS_FAILD_UPDATE(dbs, "Binding database read from file failed.", TRUE);

        SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_READ_FAIL);

        return SYS_ERR_FAILED;
    }

    while (!feof(fp))
    {
        SYS_MEM_CLEAR(entry);

        if (1 != fread(&entry, sizeof(sys_isg_entry_t), 1, fp))
            break;

        if (IS_LOGIC_PORT_INVALID(entry.port))
        {
            SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_INTF_NOTEXIST);
            continue;
        }

        if (SYS_ERR_OK != sal_isg_bind_entry_set(&entry))
        {
            dbs.isRunning = FALSE;
            dbs.startTime = 0;
            DBS_FAILD_UPDATE(dbs, "Insert binding entry from file failed.", TRUE);
        }
    }

    fclose(fp);

    SYS_LOG(LOG_CAT_IPSG, LOG_IPSG_DB_RESTORE_SUCCESS);

    dbs.isRunning = FALSE;
    dbs.startTime = 0;
    DBS_SUCCESS_UPDATE(dbs, TRUE);

    return SYS_ERR_OK;
}

