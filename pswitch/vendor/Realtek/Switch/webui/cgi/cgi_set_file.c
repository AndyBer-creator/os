
#include <include/cgi.h>
#include <libcmd/cmd.h>

static int32 cgi_file_backup_set(void);
static int32 cgi_file_upgrade_set(void);
#ifdef CONFIG_SYS_DUAL_IMAGE
static int32 cgi_file_dualImage_set(void);
#endif
static int32 cgi_file_cfgSave_set(void);

const cgi_cmdmap_t cmdSetFileTbl[] =
{
    {CGI_CMD_FILE_BACKUP,            cgi_file_backup_set},
    {CGI_CMD_FILE_UPGRADE,           cgi_file_upgrade_set},
#ifdef CONFIG_SYS_DUAL_IMAGE
    {CGI_CMD_FILE_DUAL_IMAGE,        cgi_file_dualImage_set},
#endif
    {CGI_CMD_FILE_CFG_SAVE,          cgi_file_cfgSave_set},
    {NULL, NULL}
};

static int32 _cgi_file_imgLockStatus_check(void)
{
    int32 ret = SYS_ERR_OK;

    ret = sal_sys_fwImageLock_check();

    if (SYS_ERR_OK != ret)
    {
        cJSON *status = NULL;
        status = cJSON_CreateObject();

        cJSON_AddStringToObject(status, "status", "fail");
        cJSON_AddStringToObject(status, "msg", SYS_ERR_STR(ret));

        CGI_RESPONSE_JSON(status);

        cJSON_Delete(status);
    }

    return ret;
}

static int32 cgi_file_backup_set(void)
{
    int32 ret = SYS_ERR_OK;
    FILE *fp = NULL;
    char *value = NULL;
    uint32 partition = 0;
    char method[SYS_BUF16_LEN];
    char srcFileType[SYS_BUF16_LEN];
    char host[CAPA_HOSTNAME_LEN + 1];
    char dstFilename[CAPA_FILE_NAME_LEN];
    char filename[CAPA_FILE_NAME_LEN];
    char fullname[CAPA_FILE_FULLNAME_LEN];
    char filename_web[CAPA_FILE_NAME_LEN];
    char fullname_web[CAPA_FILE_FULLNAME_LEN];
    char fullname_web_real[CAPA_FILE_FULLNAME_LEN];

    SYS_MEM_CLEAR(method);
    SYS_MEM_CLEAR(srcFileType);
    SYS_MEM_CLEAR(host);
    SYS_MEM_CLEAR(dstFilename);
    SYS_MEM_CLEAR(fullname);
    SYS_MEM_CLEAR(filename_web);
    SYS_MEM_CLEAR(fullname_web);
    SYS_MEM_CLEAR(fullname_web_real);

    if (CGI_IS_PARAM_EXIST(CGI_FILE_METHOD, value))
        strlcpy(method, value, sizeof(method));

    if (CGI_IS_PARAM_EXIST(CGI_FILE_TYPE, value))
        strlcpy(srcFileType, value, sizeof(srcFileType));

    if (CGI_IS_PARAM_EXIST(CGI_FILE_SRVHOST, value))
        strlcpy(host, value, sizeof(host));

    if (CGI_IS_PARAM_EXIST(CGI_FILE_DSTFILENAME, value))
        strlcpy(dstFilename, value, sizeof(dstFilename));

#ifdef CONFIG_SYS_DUAL_IMAGE
    if (CGI_IS_PARAM_EXIST(CGI_FILE_DUALIMG_SELIDX, value))
        partition = SYS_STR2UINT(value);
#endif

    if (0 == osal_strcmp(CGI_FILE_METHOD_HTTP, method))
    {
        cJSON *status = NULL;

        status = cJSON_CreateObject();

        if (0 == osal_strcmp(CGI_FILE_TYPE_IMAGE, srcFileType))
        {
            strlcpy(filename, DFLT_SYS_FW_NAME, sizeof(filename));
            osal_sprintf(fullname, "%s", TMP_FWIMG_FILENAME);
            osal_sprintf(fullname_web, "%s/%s", WEB_TMP_DIR, filename);
            osal_sprintf(fullname_web_real, "%s/%s", WEB_TMP_REAL_DIR, filename);

            ret = sal_sys_fwImageLock_check();
            if (SYS_ERR_OK != ret)
                goto cgi_end;

            sal_sys_fwImageLock_take();
            ret = sal_sys_fwImage_backup(partition, TMP_FWIMG_FILENAME);

            if (SYS_ERR_OK != ret)
            {
                sal_sys_fwImageLock_give();

                SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_BACKUP_FAIL,
                            partition,
                            "WEB",
                            "HTTP");
                goto cgi_end;
            }
            else
            {
                SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_BACKUP_SUCCESS,
                            partition,
                            "WEB",
                            "HTTP");
            }

            sal_sys_fwImageLock_give();
        }
        else if (0 == osal_strcmp(CGI_FILE_CFG_STARTUP, srcFileType))
        {
            osal_sprintf(fullname, "%s", CONF_STARTUP_FILE);
            osal_sprintf(filename_web, "%s%s", CONF_STARTUP_BASENAME, DFLT_SYS_CFG_EXTNAME);
            osal_sprintf(fullname_web, "%s/%s", WEB_TMP_DIR, filename_web);
            osal_sprintf(fullname_web_real, "%s/%s", WEB_TMP_REAL_DIR, filename_web);
        }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        else if (0 == osal_strcmp(CGI_FILE_CFG_BACKUP, srcFileType))
        {
            strlcpy(fullname, CONF_BACKUP_FILE, sizeof(fullname));
            osal_sprintf(filename_web, "%s%s", CONF_BACKUP_BASENAME, DFLT_SYS_CFG_EXTNAME);
            osal_sprintf(fullname_web, "%s/%s", WEB_TMP_DIR, filename_web);
            osal_sprintf(fullname_web_real, "%s/%s", WEB_TMP_REAL_DIR, filename_web);
        }
#endif
        else if (0 == osal_strcmp(CGI_FILE_CFG_RUNNING, srcFileType))
        {
            cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE);

            osal_sprintf(fullname, "%s", CONF_RUNNING_FILE);
            osal_sprintf(filename_web, "%s%s", CONF_RUNNING_BASENAME, DFLT_SYS_CFG_EXTNAME);
            osal_sprintf(fullname_web, "%s/%s", WEB_TMP_DIR, filename_web);
            osal_sprintf(fullname_web_real, "%s/%s", WEB_TMP_REAL_DIR, filename_web);
        }
        else if (0 == osal_strcmp(CGI_FILE_LOG_BUFFER, srcFileType))
        {
            osal_sprintf(fullname, "%s", LOG_FILE_RAM);
            osal_sprintf(filename_web, "%s", LOG_FILE_RAM_BASENAME);
            osal_sprintf(fullname_web, "%s/%s", WEB_TMP_DIR, filename_web);
            osal_sprintf(fullname_web_real, "%s/%s", WEB_TMP_REAL_DIR, filename_web);
        }
        else if (0 == osal_strcmp(CGI_FILE_LOG_FLASH, srcFileType))
        {
            osal_sprintf(fullname, "%s", LOG_FILE_FLASH);
            osal_sprintf(filename_web, "%s", LOG_FILE_FLASH_BASENAME);
            osal_sprintf(fullname_web, "%s/%s", WEB_TMP_DIR, filename_web);
            osal_sprintf(fullname_web_real, "%s/%s", WEB_TMP_REAL_DIR, filename_web);
        }

        unlink(fullname_web);

        if (SYS_ERR_OK == sal_util_file_exist(fullname))
        {
            symlink(fullname, fullname_web_real);

            cJSON_AddStringToObject(status, "status", "ok");
            cJSON_AddStringToObject(status, "filename", fullname_web);
        }
        else
        {
            cJSON_AddStringToObject(status, "status", "fail");
            cJSON_AddStringToObject(status, "msg", "File not exist");
        }

        CGI_RESPONSE_JSON(status);

        cJSON_Delete(status);

        return SYS_ERR_OK;
    }
    
    else
    {
        SYS_MEM_CLEAR(filename);
        SYS_MEM_CLEAR(fullname);

        if (0 == osal_strcmp(CGI_FILE_TYPE_IMAGE, srcFileType))
        {
            ret = _cgi_file_imgLockStatus_check();

            if (SYS_ERR_OK != ret)
                return SYS_ERR_OK;

            if (NULL == (fp =  fopen("/tmp/tftp_ulinfo", "w")))
            {
                sal_sys_fwImageSaveMem_gen(TMP_FWIMG_FILENAME);
                ret =  SYS_ERR_FILE_OPEN;
                goto cgi_end;
            }

            fprintf(fp, "%s %% %s %% %d", host, dstFilename, partition);
            fclose(fp);

            if (NULL != (fp = fopen(SYS_POLLD_ACT_CUSTOM, "w")))
            {
                fprintf(fp, "handler -f web_tftp_action ul_fw &");
                fclose(fp);
            }
            DO_POLLD_ACT_SIG();

            ret = SYS_ERR_OK;
        }
        else
        {
            if (0 == osal_strcmp(CGI_FILE_CFG_STARTUP, srcFileType))
            {
                strlcpy(filename, CONF_STARTUP_BASENAME, sizeof(filename));
                strlcpy(fullname, CONF_STARTUP_FILE, sizeof(fullname));
            }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
            else if (0 == osal_strcmp(CGI_FILE_CFG_BACKUP, srcFileType))
            {
                strlcpy(filename, CONF_BACKUP_BASENAME, sizeof(filename));
                strlcpy(fullname, CONF_BACKUP_FILE, sizeof(fullname));
            }
#endif
            else if (0 == osal_strcmp(CGI_FILE_CFG_RUNNING, srcFileType))
            {
                cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE);

                strlcpy(filename, CONF_RUNNING_BASENAME, sizeof(filename));
                osal_sprintf(fullname, "%s", CONF_RUNNING_FILE);
            }
            else if (0 == osal_strcmp(CGI_FILE_LOG_BUFFER, srcFileType))
            {
                strlcpy(filename, LOG_FILE_RAM_BASENAME, sizeof(filename));
                osal_sprintf(fullname, "%s", LOG_FILE_RAM);
            }
            else if (0 == osal_strcmp(CGI_FILE_LOG_FLASH, srcFileType))
            {
                strlcpy(filename, LOG_FILE_FLASH_BASENAME, sizeof(filename));
                osal_sprintf(fullname, "%s", LOG_FILE_FLASH);
            }

            ret = sal_tftp_config_upload(fullname, dstFilename, host);

            if (SYS_ERR_OK != ret)
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_FAIL,
                            srcFileType);
            else
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_SUCCESS,
                            srcFileType);

        }

        if (SYS_ERR_OK == ret)
        {
            cJSON *status = NULL;

            status = cJSON_CreateObject();

            cJSON_AddStringToObject(status, "status", "ok");

            CGI_RESPONSE_JSON(status);

            cJSON_Delete(status);

            return SYS_ERR_OK;
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_file_upgrade_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    FILE *fp = NULL;
    uint32 currSelPart = 0;
    char fileType[SYS_BUF16_LEN];
    char host[CAPA_HOSTNAME_LEN + 1];
    char remoteFile[CAPA_FILE_NAME_LEN];

    SYS_MEM_CLEAR(fileType);
    SYS_MEM_CLEAR(host);
    SYS_MEM_CLEAR(remoteFile);

    if (CGI_IS_PARAM_EXIST(CGI_FILE_TYPE, value))
        strlcpy(fileType, value, sizeof(fileType));

    if (CGI_IS_PARAM_EXIST(CGI_FILE_SRVHOST, value))
        strlcpy(host, value, sizeof(host));

    if (CGI_IS_PARAM_EXIST(CGI_FILE_SRCFILENAME, value))
        strlcpy(remoteFile, value, sizeof(remoteFile));

    if (0 == osal_strcmp(CGI_FILE_TYPE_IMAGE, fileType))
    {
        ret = _cgi_file_imgLockStatus_check();

        if (SYS_ERR_OK != ret)
            return SYS_ERR_OK;

        sal_sys_currActiveImageIdx_get(&currSelPart);

        if (NULL == (fp =  fopen("/tmp/tftp_write2partition", "w")))
        {
            sal_sys_fwImageSaveMem_gen(TMP_FWIMG_FILENAME);
            ret =  SYS_ERR_FILE_OPEN;
            goto cgi_end;
        }

    #ifdef CONFIG_SYS_DUAL_IMAGE
        fprintf(fp, "%s %% %s %% %d", host, remoteFile, currSelPart);
    #else
        fprintf(fp, "%s %% %s %% %d", host, remoteFile, currSelPart);
    #endif
        fclose(fp);

        if (NULL != (fp = fopen(SYS_POLLD_ACT_CUSTOM, "w")))
        {
            fprintf(fp, "handler -f web_tftp_action dl_fw &");
            fclose(fp);
        }
        DO_POLLD_ACT_SIG();
    }
    else if (0 == osal_strcmp(CGI_FILE_CFG_STARTUP, fileType))
    {
        ret = sal_tftp_config_download(CONF_STARTUP_TMPFILE, remoteFile, host);

        if (SYS_ERR_OK != ret)
            goto cgi_end;

        if (IS_CONFIG_FILEHDR_INVALID(CONF_STARTUP_TMPFILE))
        {
            unlink(CONF_STARTUP_TMPFILE);
            ret = SYS_ERR_CFG_INVALID;
            goto cgi_end;
        }

        ret = cmd_conf_startupConfig_copy(CONF_STARTUP_TMPFILE);

        if (SYS_ERR_OK != ret)
            goto cgi_end;

        cmd_configValidFile_convert(CONF_STARTUP_FILE);

        unlink(CONF_STARTUP_TMPFILE);

        if (SYS_ERR_OK == ret)
        {
            cJSON *status = NULL;

            status = cJSON_CreateObject();

            cJSON_AddStringToObject(status, "status", "ok");
            cJSON_AddNumberToObject(status, "bytes", sal_util_fileSize_ret(CONF_STARTUP_FILE));

            CGI_RESPONSE_JSON(status);

            cJSON_Delete(status);

            ret = SYS_ERR_OK;

            goto syslog;
        }
    }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if (0 == osal_strcmp(CGI_FILE_CFG_BACKUP, fileType))
    {
        ret = sal_tftp_config_download(CONF_HTTP_SAVERUNNING_TMPFILE, remoteFile, host);

        if (SYS_ERR_OK != ret)
            goto cgi_end;

        if (IS_CONFIG_FILEHDR_INVALID(CONF_HTTP_SAVERUNNING_TMPFILE))
        {
            unlink(CONF_HTTP_SAVERUNNING_TMPFILE);
            ret =  SYS_ERR_CFG_INVALID;
            goto cgi_end;
        }

        sal_util_file_copy(CONF_HTTP_SAVERUNNING_TMPFILE, CONF_BACKUP_FILE);

        unlink(CONF_HTTP_SAVERUNNING_TMPFILE);

        cmd_configValidFile_convert(CONF_BACKUP_FILE);

        if (SYS_ERR_OK == ret)
        {
            cJSON *status = NULL;

            status = cJSON_CreateObject();

            cJSON_AddStringToObject(status, "status", "ok");
            cJSON_AddNumberToObject(status, "bytes", sal_util_fileSize_ret(CONF_BACKUP_FILE));

            CGI_RESPONSE_JSON(status);

            cJSON_Delete(status);

            ret = SYS_ERR_OK;

            goto syslog;
        }
    }
#endif
    else if (0 == osal_strcmp(CGI_FILE_CFG_RUNNING, fileType))
    {
        ret = sal_tftp_config_download(CONF_HTTP_SAVERUNNING_TMPFILE, remoteFile, host);

        if (SYS_ERR_OK != ret)
            goto cgi_end;

        if (IS_CONFIG_FILEHDR_INVALID(CONF_HTTP_SAVERUNNING_TMPFILE))
        {
            unlink(CONF_HTTP_SAVERUNNING_TMPFILE);
            ret =  SYS_ERR_CFG_INVALID;
            goto cgi_end;
        }

        system("killall -q -SIGTSTP polld");

        ret = SYS_ERR_OK;
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

syslog:
    if (SYS_ERR_CFG_INVALID == ret)
        SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_CFG_INVALID);

    if (SYS_ERR_OK != ret)
        SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL,
                    fileType);
    else
        SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS,
                    fileType);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_DUAL_IMAGE
static int32 cgi_file_dualImage_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 currSelPart = 0;

    if (CGI_IS_PARAM_EXIST(CGI_FILE_DUALIMG_SELIDX, value))
        currSelPart = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_sys_dualImage_sel(currSelPart), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_file_cfgSave_set(void)
{
    int32 ret = SYS_ERR_FAILED;
    char *value = NULL;
    char srcFile[SYS_BUF64_LEN];
    char dstFile[SYS_BUF64_LEN];
    char statusFile[SYS_BUF64_LEN];
    FILE *fp = NULL;
    uint32 copy2Running = FALSE;

    SYS_MEM_CLEAR(srcFile);
    SYS_MEM_CLEAR(dstFile);
    SYS_MEM_CLEAR(statusFile);

    if (CGI_IS_PARAM_EXIST(CGI_FILE_CFG_SAVE_SRCFILE, value))
    {
        if (CGI_IS_VALUE_EQUAL(value, CGI_FILE_CFG_STARTUP))
            strlcpy(srcFile, CONF_STARTUP_FILE, sizeof(srcFile));

#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        if (CGI_IS_VALUE_EQUAL(value, CGI_FILE_CFG_BACKUP))
            strlcpy(srcFile, CONF_BACKUP_FILE, sizeof(srcFile));
#endif

        if (CGI_IS_VALUE_EQUAL(value, CGI_FILE_CFG_RUNNING))
            strlcpy(srcFile, CONF_RUNNING_FILE, sizeof(srcFile));
    }

    if (CGI_IS_PARAM_EXIST(CGI_FILE_CFG_SAVE_DSTFILE, value))
    {
        if (CGI_IS_VALUE_EQUAL(value, CGI_FILE_CFG_STARTUP))
            strlcpy(dstFile, CONF_STARTUP_FILE, sizeof(dstFile));

#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        if (CGI_IS_VALUE_EQUAL(value, CGI_FILE_CFG_BACKUP))
            strlcpy(dstFile, CONF_BACKUP_FILE, sizeof(dstFile));
#endif

        if (CGI_IS_VALUE_EQUAL(value, CGI_FILE_CFG_RUNNING))
            strlcpy(dstFile, CONF_RUNNING_FILE, sizeof(dstFile));
    }

    sal_util_randomfileName_gen("/tmp", "cgiCfgStat", statusFile);

    fp = fopen(statusFile, "w");

    if (SYS_ERR_OK != sal_util_file_exist(srcFile) && 0 != osal_strcmp(srcFile, CONF_RUNNING_FILE))
    {
        fprintf(fp, "%s\n%s\n", "fail", "The source file/directory does not exist");
    }
    else
    {
        if (0 == osal_strcmp(srcFile, CONF_RUNNING_FILE))
        {
            if (0 == osal_strcmp(dstFile, CONF_STARTUP_FILE))
                ret = cmd_conf_startupConfig_save();
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
            else if (0 == osal_strcmp(dstFile, CONF_BACKUP_FILE))
                ret = cmd_conf_backupConfig_save(CONF_BACKUP_BASENAME);
#endif
        }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        else if (0 == osal_strcmp(srcFile, CONF_STARTUP_FILE))
        {
            if (0 == osal_strcmp(dstFile, CONF_BACKUP_FILE))
                ret = sal_util_file_copy(srcFile, dstFile);
        }
        else if (0 == osal_strcmp(srcFile, CONF_BACKUP_FILE))
        {
            if (0 == osal_strcmp(dstFile, CONF_STARTUP_FILE))
                ret = cmd_conf_startupConfig_copy(srcFile);
        }
#endif

        if (0 == osal_strcmp(dstFile, CONF_RUNNING_FILE))
        {
            sal_util_file_copy(srcFile, CONF_HTTP_SAVERUNNING_TMPFILE);
            system("killall -q -SIGTSTP polld");
            copy2Running = TRUE;
        }

        if (TRUE == copy2Running)
            fprintf(fp, "%s\n%d\n", "uploading", sal_util_fileSize_ret(srcFile));
        else if (ret == SYS_ERR_OK)
             fprintf(fp, "%s\n%d\n", "success", sal_util_fileSize_ret(srcFile));
        else
             fprintf(fp, "%s\n%s\n", "fail", SYS_ERR_STR(ret));
    }

    fclose(fp);

    {
        cJSON *status = NULL;
        status = cJSON_CreateObject();

        cJSON_AddStringToObject(status, CGI_FILE_CFG_STATUS_FILE, statusFile + 5);

        CGI_RESPONSE_JSON(status);

        cJSON_Delete(status);
    }

    return SYS_ERR_OK;
}

