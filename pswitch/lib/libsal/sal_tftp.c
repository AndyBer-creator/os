
#include <libsal/sal_util.h>
#include <libsal/sal_tftp.h>

#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
#define TFTP_CMD "tftp"
#else
#define TFTP_CMD "tftp6"
#endif

static int32 _sal_tftp_rmtFilename_refine(char *rfFilename,char *rFilename)
{
    SYS_PARAM_CHK((NULL == rfFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == rFilename), SYS_ERR_NULL_POINTER);

    while('\0' != *rFilename)
    {
        if(('('== (*rFilename))|| (')'== (*rFilename)))
        {
            *rfFilename = 92;
             rfFilename++;
            *rfFilename = *rFilename;
        }
        else
            *rfFilename = *rFilename;

        rfFilename++;
        rFilename++;
    }

    *rfFilename = '\0';

    return SYS_ERR_OK;
}

static int32 _sal_tftp_errId_parse(int32 status, sys_tftpFileType_t file, sys_tftpOpType_t op)
{
    FILE *fp = NULL;
    int32 ret = SYS_ERR_FAILED;
    int32 errId = 0;

    if (0 == status)
        return SYS_ERR_OK;

    if (NULL != (fp = fopen(SYS_TFTP_ERR_FILE, "r")))
    {
        fscanf(fp, "%d", &errId);

        switch (errId)
        {
            
            case 1:
                ret = SYS_ERR_TFTP_FILE_NOT_FOUND;
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TFTP_ERROR,
                            "file not found",
                            errId);
                break;

            case 2:
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TFTP_ERROR,
                            "access violation",
                            errId);
                break;
            
            case 3:
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TFTP_ERROR,
                            "disk full",
                            errId);
                break;
            
            case 4:
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TFTP_ERROR,
                            "bad operation",
                            errId);
                break;
            
            case 5:
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TFTP_ERROR,
                            "unknown transfer id",
                            errId);
                break;
            
            case 6:
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TFTP_ERROR,
                            "file already exists",
                            errId);
                break;
            
            case 7:
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TFTP_ERROR,
                            "no such user",
                            errId);
                break;
            
            case 8:
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TFTP_ERROR,
                            "bad option",
                            errId);
                break;

            default:
                ret = SYS_ERR_FAILED;
                break;
        }

        fclose(fp);

        unlink(SYS_TFTP_ERR_FILE);

        return ret;
    }

    if ((SYS_TFTP_FILETYPE_IMAGE == file) && (op == SYS_TFTP_OPTYPE_DOWNLOAD))
        ret = SYS_ERR_TFTP_DOWNLOAD_IMAGE;
    else if ((SYS_TFTP_FILETYPE_IMAGE == file) && (op == SYS_TFTP_OPTYPE_UPLOAD))
        ret = SYS_ERR_TFTP_UPLOAD_IMAGE;
    else if ((SYS_TFTP_FILETYPE_CONFIG == file) && (op == SYS_TFTP_OPTYPE_DOWNLOAD))
        ret = SYS_ERR_TFTP_DOWNLOAD_CONFIG;
    else if ((SYS_TFTP_FILETYPE_CONFIG == file) && (op == SYS_TFTP_OPTYPE_UPLOAD))
        ret = SYS_ERR_TFTP_UPLOAD_CONFIG;

    return ret;
}

int32 sal_tftp_fwImage_download(char *lFilename, char *rFilename, char *host)
{
    int32 ret;
    char cmd[SYS_BUF256_LEN];
    char rfFilename[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == lFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == rFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == host), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(rfFilename);
    SYS_ERR_CHK(sal_sys_fwImageLock_check());

#ifdef CONFIG_SYS_FW_UPGRADE_CHECKMEM
    SYS_ERR_CHK(sal_sys_fwImageFreeMem_check());
#endif

    SYS_ERR_CHK(sal_sys_fwImageLock_take());
    SYS_ERR_CHK(_sal_tftp_rmtFilename_refine(rfFilename,rFilename));

#ifdef CONFIG_SYS_DEFENCE_ENGINE
    sal_rate_cpuEgrQueueCtrlEnable_set(SYS_CPU_PORT_PRIORITY_MYMAC,  DISABLED);
#endif

    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, TFTP_CMD" -g -l %s -r %s %s 1> /dev/null 2> /dev/null", lFilename, rfFilename, host);

    ret = system(cmd);

#ifdef CONFIG_SYS_DEFENCE_ENGINE
    sal_rate_cpuEgrQueueCtrlEnable_set(SYS_CPU_PORT_PRIORITY_MYMAC,  ENABLED);
#endif

    ret = _sal_tftp_errId_parse(ret, SYS_TFTP_FILETYPE_IMAGE, SYS_TFTP_OPTYPE_DOWNLOAD);

    if (SYS_ERR_OK != ret)
    {
        sal_sys_fwImageLock_give();

        return ret;
    }

    ret = sal_sys_fwImageCrc32_check(lFilename);

    if (SYS_ERR_OK != ret)
        goto img_invalid;

    return ret;

img_invalid:
    sal_sys_fwImageLock_give();
#ifdef CONFIG_SYS_FW_UPGRADE_SAVEMEM
    sal_sys_fwImageSaveMem_gen(lFilename);
#else
    unlink(lFilename);
#endif
    return ret;
}

int32 sal_tftp_fwImage_upload(uint32 partition, char *rFilename, char *host)
{
    int32 ret;
    char cmd[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == rFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == host), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_sys_fwImageLock_check());

#ifdef CONFIG_SYS_FW_UPGRADE_CHECKMEM
    SYS_ERR_CHK(sal_sys_fwImageFreeMem_check());
#endif

    SYS_ERR_CHK(sal_sys_fwImageLock_take());

    ret = sal_sys_fwImage_backup(partition, TMP_FWIMG_FILENAME);

    if (SYS_ERR_OK != ret)
    {
        sal_sys_fwImageLock_give();

        return ret;
    }

#ifdef CONFIG_SYS_DEFENCE_ENGINE
    sal_rate_cpuEgrQueueCtrlEnable_set(SYS_CPU_PORT_PRIORITY_MYMAC,  DISABLED);
#endif

    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, TFTP_CMD" -p -l %s -r %s %s 1> /dev/null 2> /dev/null", TMP_FWIMG_FILENAME, rFilename, host);

    ret = system(cmd);

#ifdef CONFIG_SYS_DEFENCE_ENGINE
    sal_rate_cpuEgrQueueCtrlEnable_set(SYS_CPU_PORT_PRIORITY_MYMAC,  ENABLED);
#endif

    sal_sys_fwImageLock_give();

#ifdef CONFIG_SYS_FW_UPGRADE_SAVEMEM
    SYS_ERR_CHK(sal_sys_fwImageSaveMem_gen(TMP_FWIMG_FILENAME));
#else
    unlink(TMP_FWIMG_FILENAME);
#endif

    ret = _sal_tftp_errId_parse(ret, SYS_TFTP_FILETYPE_IMAGE, SYS_TFTP_OPTYPE_UPLOAD);

    if (SYS_ERR_OK != ret)
        return ret;

    return SYS_ERR_OK;
}

int32 sal_tftp_config_download(char *lFilename, char *rFilename, char *host)
{
    int32 ret;
    char cmd[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == lFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == rFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == host), SYS_ERR_NULL_POINTER);

    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, TFTP_CMD" -g -l %s -r %s %s 1> /dev/null 2> /dev/null", lFilename, rFilename, host);

    ret = system(cmd);

    ret = _sal_tftp_errId_parse(ret, SYS_TFTP_FILETYPE_CONFIG, SYS_TFTP_OPTYPE_DOWNLOAD);

    if (SYS_ERR_OK != ret)
        return ret;

    sal_sys_backupConfig_sync(SYS_CONF_SYNC_STARTUP);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    sal_sys_backupConfig_sync(SYS_CONF_SYNC_BACKUP);
#endif

    return SYS_ERR_OK;
}

int32 sal_tftp_config_upload(char *lFilename, char *rFilename, char *host)
{
    int32 ret;
    char cmd[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == lFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == rFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == host), SYS_ERR_NULL_POINTER);

    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, TFTP_CMD" -p -l %s -r %s %s 1> /dev/null 2> /dev/null", lFilename, rFilename, host);

    ret = system(cmd);

    ret = _sal_tftp_errId_parse(ret, SYS_TFTP_FILETYPE_CONFIG, SYS_TFTP_OPTYPE_UPLOAD);

    if (SYS_ERR_OK != ret)
        return ret;

    return SYS_ERR_OK;
}

