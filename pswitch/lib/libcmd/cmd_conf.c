
#include <libcmd/cmd.h>
#include <dirent.h>

static int32 _cmd_config_startupPostAction_exec(void)
{
    return SYS_ERR_OK;
}

int32 cmd_conf_startupConfig_save(void)
{
    SYS_ERR_CHK(cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE));

    SYS_ERR_CHK(sal_util_file_copy(CONF_RUNNING_FILE, CONF_STARTUP_FILE));

    SYS_ERR_CHK(_cmd_config_startupPostAction_exec());

    unlink(CONF_FIRST_LOGIN_FLAG);

    sal_sys_backupConfig_sync(SYS_CONF_SYNC_STARTUP);

    return SYS_ERR_OK;
}

int32 cmd_conf_startupConfig_copy(char *srcFname)
{
    SYS_ERR_CHK(sal_util_file_copy(srcFname, CONF_STARTUP_FILE));

    SYS_ERR_CHK(_cmd_config_startupPostAction_exec());

    unlink(CONF_FIRST_LOGIN_FLAG);

    sal_sys_backupConfig_sync(SYS_CONF_SYNC_STARTUP);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_BACKUP_CONFIGURATION

int32 cmd_conf_backupConfig_save(char *fname)
{
    SYS_ERR_CHK(cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE));

    SYS_ERR_CHK(sal_util_file_copy(CONF_RUNNING_FILE, CONF_BACKUP_FILE));

    sal_sys_backupConfig_sync(SYS_CONF_SYNC_BACKUP);

    return SYS_ERR_OK;
}
#endif

uint32 cmd_conf_configIdentical_ret(char *cfgFile1, char *cfgFile2)
{
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    FILE *fp_base = NULL;
    FILE *fp_comp = NULL;
    uint32 cfgsize1 = 0;
    uint32 cfgsize2 = 0;
    char cmdstr1[MAX_CMD_STRLEN];
    char cmdstr2[MAX_CMD_STRLEN];

    cfgsize1 = sal_util_fileSize_ret(cfgFile1);
    cfgsize2 = sal_util_fileSize_ret(cfgFile2);

    if (NULL == (fp1 = fopen(cfgFile1, "r")))
        return FALSE;
    if (NULL == (fp2 = fopen(cfgFile2, "r")))
    {
        fclose(fp1);
        return FALSE;
    }

    if (cfgsize1 >= cfgsize2)
    {
        fp_base = fp1;
        fp_comp = fp2;
    }
    else
    {
        fp_base = fp2;
        fp_comp = fp1;
    }

    osal_memset(cmdstr1, 0, sizeof(cmdstr1));
    while (fgets(cmdstr1, sizeof(cmdstr1), fp_base))
    {
        osal_memset(cmdstr2, 0, sizeof(cmdstr2));
        if (NULL == fgets(cmdstr2, sizeof(cmdstr2), fp_comp))
            continue;

        if (('#' == cmdstr1[0])
                || ('!' == cmdstr1[0])
                || ('\n' == cmdstr1[0])
           )
        {
            osal_memset(cmdstr1, 0, sizeof(cmdstr1));
            continue;
        }

        if (0 != osal_strcmp(cmdstr1, cmdstr2))
        {
            fclose(fp1);
            fclose(fp2);

            return FALSE;
        }

        osal_memset(cmdstr1, 0, sizeof(cmdstr1));
    }

    fclose(fp1);
    fclose(fp2);

    return TRUE;
}

int32 cmd_conf_consistent_check(void)
{
    FILE *fp_cfg = NULL;
    FILE *fp_bak = NULL;

    fp_cfg = fopen(CONF_STARTUP_FILE, "r");

    fp_bak = fopen(CONF_BAKPART_STARTUP_FILE, "r");

    if ((NULL == fp_cfg) && (NULL != fp_bak))
    {
        sal_sys_backupConfig_restore();
    }
    
    else if ((NULL != fp_cfg) && (NULL == fp_bak))
    {
        sal_sys_backupConfig_sync(SYS_CONF_SYNC_ALL);
    }
    
    else if ((NULL != fp_cfg) && (NULL != fp_bak))
    {
        if (!sal_util_isFileConsistent_ret(CONF_STARTUP_FILE, CONF_BAKPART_STARTUP_FILE)
            && IS_CONFIG_FILE_VALID(CONF_BAKPART_STARTUP_FILE))
        {
            sal_sys_backupConfig_restore();
        }
    }

    if (NULL != fp_cfg)
        fclose(fp_cfg);

    if (NULL != fp_bak)
        fclose(fp_bak);

    return SYS_ERR_OK;
}

