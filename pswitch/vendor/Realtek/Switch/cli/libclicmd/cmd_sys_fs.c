
#include <dirent.h>
#include <libsal/sal_sys.h>
#include <libcmd/cmd.h>
#include <libfds/fds_if_restore.h>

uint32 isCopy2Running = FALSE;

static int32 _cli_sys_flashArgv_get(char *string, char *filename)
{
    char *file = NULL;

    file = string + osal_strlen("flash://");

    if (osal_strlen(file) > CAPA_FILE_NAME_LEN)
        return SYS_ERR_INPUT;

    osal_strcpy(filename, file);

    return SYS_ERR_OK;
}

static int32 _cli_sys_tftpArgv_get(char *string, char *filename, char *hostname)
{
    char *file = NULL;
    char *host = NULL;

    host = string + osal_strlen("tftp://");

    file = osal_strchr(host, '/');

    if (NULL == file)
    {
        if (osal_strlen(host) > CAPA_HOSTNAME_LEN)
            return SYS_ERR_INPUT;

        osal_strcpy(hostname, host);
    }
    else
    {
        if ((file - host) > CAPA_HOSTNAME_LEN)
            return SYS_ERR_INPUT;

        osal_strncpy(hostname, host, file - host);

        file = file + 1;

        if (osal_strlen(file) > CAPA_FILE_NAME_LEN)
            return SYS_ERR_INPUT;

        if (SYS_ERR_OK != FILENAME_VALIDATE(file, FALSE))
            return SYS_ERR_STRING;

        osal_strcpy(filename, file);
    }

    return SYS_ERR_OK;
}

static int32 _cli_sys_localFileType_get(char *filename, sys_local_file_type_t *pType)
{
    if (0 == osal_strcmp(filename, CONF_STARTUP_BASENAME))
    {
        *pType = SYS_LOCAL_FILE_CONFIG_STARTUP;

        return SYS_ERR_OK;
    }

#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    if (0 == osal_strcmp(filename, CONF_BACKUP_BASENAME))
    {
        *pType = SYS_LOCAL_FILE_CONFIG_BACKUP;

        return SYS_ERR_OK;
    }
#endif

    if (0 == osal_strcmp(filename, CONF_RUNNING_BASENAME))
    {
        *pType = SYS_LOCAL_FILE_CONFIG_RUNNING;

        return SYS_ERR_OK;
    }

    if (0 == osal_strcmp(filename, LOG_FILE_RAM_BASENAME))
    {
        *pType = SYS_LOCAL_FILE_LOG_RAM;

        return SYS_ERR_OK;
    }

    if (0 == osal_strcmp(filename, LOG_FILE_FLASH_BASENAME))
    {
        *pType = SYS_LOCAL_FILE_LOG_FLASH;

        return SYS_ERR_OK;
    }

    if (0 == osal_strcmp(filename, SYS_FW_NAME_IMG0))
    {
        *pType = SYS_LOCAL_FILE_IMAGE_0;

        return SYS_ERR_OK;
    }

    if (0 == osal_strcmp(filename, SYS_FW_NAME_IMG1))
    {
        *pType = SYS_LOCAL_FILE_IMAGE_1;

        return SYS_ERR_OK;
    }
#ifdef CONFIG_SYS_APP_SSH
    if (0 == osal_strcmp(filename, SSH_RSA1))
    {
        *pType = SYS_LOCAL_FILE_SSH_RSA1;

        return SYS_ERR_OK;
    }

    if (0 == osal_strcmp(filename, SSH_RSA2))
    {
        *pType = SYS_LOCAL_FILE_SSH_RSA2;

        return SYS_ERR_OK;
    }

    if (0 == osal_strcmp(filename, SSH_DSA2))
    {
        *pType = SYS_LOCAL_FILE_SSH_DSA2;

        return SYS_ERR_OK;
    }
#endif
    if (0 == osal_strcmp(filename, SSL_CERT))
    {
        *pType = SYS_LOCAL_FILE_SSL_CERT;

        return SYS_ERR_OK;
    }

    return SYS_ERR_FILE_NOT_EXIST;
}

static int32 _cli_sys_flash2tftp(char *localFile, char *remoteFile, char *remoteHost)
{
    int32 ret = SYS_ERR_OK;
    sys_local_file_type_t type;

    ret = _cli_sys_localFileType_get(localFile, &type);

    if (SYS_ERR_FILE_NOT_EXIST == ret)
    {
        XMORE("Local file is not support\n");
        return SYS_ERR_OK;
    }

    XMORE("Uploading file. Please wait...\n");

    switch (type)
    {
        case SYS_LOCAL_FILE_IMAGE_0:

                cmd_util_progressPrint_start();
                g_isCliFwProcess = TRUE;

                ret = sal_tftp_fwImage_upload(0, remoteFile, remoteHost);

                if (SYS_ERR_OK != ret)
                {
                    SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_BACKUP_FAIL,
                                0,
                                "CLI",
                                "TFTP");
                }
                else
                {
                    SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_BACKUP_SUCCESS,
                                0,
                                "CLI",
                                "TFTP");
                }

                g_isCliFwProcess = FALSE;
                cmd_util_progressPrint_stop();

                if (SYS_ERR_OK != ret)
                    return ret;
            break;

        case SYS_LOCAL_FILE_IMAGE_1:

                cmd_util_progressPrint_start();
                g_isCliFwProcess = TRUE;

                ret = sal_tftp_fwImage_upload(1, remoteFile, remoteHost);

                if (SYS_ERR_OK != ret)
                {
                    SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_BACKUP_FAIL,
                                1,
                                "CLI",
                                "TFTP");
                }
                else
                {
                    SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_BACKUP_SUCCESS,
                                1,
                                "CLI",
                                "TFTP");
                }

                g_isCliFwProcess = FALSE;
                cmd_util_progressPrint_stop();

                if (SYS_ERR_OK != ret)
                    return ret;
            break;

        case SYS_LOCAL_FILE_CONFIG_STARTUP:
            ret = sal_tftp_config_upload(CONF_STARTUP_FILE, remoteFile, remoteHost);

            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_FAIL,
                            CONF_STARTUP_BASENAME);
                return ret;
            }
            else
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_SUCCESS,
                            CONF_STARTUP_BASENAME);
            }
            break;

#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        case SYS_LOCAL_FILE_CONFIG_BACKUP:
            ret = sal_tftp_config_upload(CONF_BACKUP_FILE, remoteFile, remoteHost);

            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_FAIL,
                            CONF_BACKUP_BASENAME);
                return ret;
            }
            else
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_SUCCESS,
                            CONF_BACKUP_BASENAME);
            }
            break;
#endif

        case SYS_LOCAL_FILE_CONFIG_RUNNING:
            SYS_ERR_CHK(cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE));
            ret = sal_tftp_config_upload(CONF_RUNNING_FILE, remoteFile, remoteHost);

            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_FAIL,
                            CONF_RUNNING_BASENAME);
                return ret;
            }
            else
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_SUCCESS,
                            CONF_RUNNING_BASENAME);
            }
            break;

#ifdef CONFIG_SYS_SYSLOG
        case SYS_LOCAL_FILE_LOG_RAM:
            ret = sal_tftp_config_upload(LOG_FILE_RAM, remoteFile, remoteHost);

            if (SYS_ERR_TFTP_UPLOAD_CONFIG == ret)
            {
                SYS_PRINTF("Upload log file to TFTP server failed\n");
                return SYS_ERR_OK;
            }
            else if (SYS_ERR_OK != ret)
            {
                return ret;
            }
            break;

        case SYS_LOCAL_FILE_LOG_FLASH:
            ret = sal_tftp_config_upload(LOG_FILE_FLASH, remoteFile, remoteHost);

            if (SYS_ERR_TFTP_UPLOAD_CONFIG == ret)
            {
                SYS_PRINTF("Upload log file to TFTP server failed\n");
                return SYS_ERR_OK;
            }
            else if (SYS_ERR_OK != ret)
            {
                return ret;
            }
            break;
#endif
#ifdef CONFIG_SYS_APP_SSH
        case SYS_LOCAL_FILE_SSH_RSA1:
            SYS_ERR_CHK(sal_tftp_config_upload(SYS_SSH_RSAV1_PRIKEY_FILE, remoteFile, remoteHost));
            break;

        case SYS_LOCAL_FILE_SSH_RSA2:
            SYS_ERR_CHK(sal_tftp_config_upload(SYS_SSH_RSAV2_PRIKEY_FILE, remoteFile, remoteHost));
            break;

        case SYS_LOCAL_FILE_SSH_DSA2:
            SYS_ERR_CHK(sal_tftp_config_upload(SYS_SSH_DSAV2_PRIKEY_FILE, remoteFile, remoteHost));
            break;
#endif
        case SYS_LOCAL_FILE_SSL_CERT:
            SYS_ERR_CHK(sal_tftp_config_upload(SSL_CERT_FILE, remoteFile, remoteHost));
            break;

        default:
            break;
    }

    XMORE("Uploading Done\n");

    return SYS_ERR_OK;
}

static int32 _cli_sys_copyToRuuning(struct vty *vty, char *file)
{
    int32 ret = SYS_ERR_OK;
    FILE *fp;
    enum node_type old_node;

    if (NULL == (fp = fopen(file, "r")))
    {
        return SYS_ERR_FILE_OPEN;
    }

    old_node = vty->node;

    vty->node = CONFIG_NODE;

    isCopy2Running = TRUE;

    ret = config_init_from_file(vty, fp);

    isCopy2Running = FALSE;

    vty->node = old_node;

    fclose(fp);

    return ret;
}

static int32 _cli_sys_tftp2flash(struct vty *vty, char *localFile, char *remoteFile, char *remoteHost)
{
    int32 ret = SYS_ERR_OK;
    sys_local_file_type_t type;

    if (osal_strlen(localFile) >= sizeof(sys_filenamestr_t) ||
        osal_strlen(remoteFile) >= sizeof(sys_filenamestr_t))
        return SYS_ERR_INPUT;

    ret = _cli_sys_localFileType_get(localFile, &type);

    if (SYS_ERR_FILE_NOT_EXIST == ret)
    {
        XMORE("Local file is not support\n");

        return SYS_ERR_OK;
    }

    XMORE("Downloading file. Please wait...\n");

    if (SYS_LOCAL_FILE_IMAGE_0 == type || SYS_LOCAL_FILE_IMAGE_1 == type)
    {
        uint32 partition = 0;

        cmd_util_progressPrint_start();

        ret = sal_tftp_fwImage_download(TMP_FWIMG_FILENAME, remoteFile, remoteHost);

        if (SYS_ERR_OK != ret)
        {
            cmd_util_progressPrint_stop();

            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_FAIL,
                        partition,
                        "CLI",
                        "TFTP");

            return ret;
        }

        CLI_INTR_FORBID();

        if (SYS_LOCAL_FILE_IMAGE_0 == type)
            partition = 0;
        else
            partition = 1;

        ret = sal_sys_fwImage_write(TMP_FWIMG_FILENAME, partition, (sys_filenamestr_t *)remoteFile);

        if (SYS_ERR_OK != ret)
        {
            CLI_INTR_ALLOW();
            cmd_util_progressPrint_stop();

            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_FAIL,
                        partition,
                        "CLI",
                        "TFTP");
            return ret;
        }
        else
        {
            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_SUCCESS,
                        partition,
                        remoteFile,
                        "CLI",
                        "TFTP");
        }

        CLI_INTR_ALLOW();
        cmd_util_progressPrint_stop();

#ifdef CONFIG_SYS_AUTOREBOOT_IMAGE
        XMORE("Rebooting now...\n");
        sal_sys_reboot();
#else
        char confirm[2];

        XMORE("Upgrade firmware success. Do you want to reboot now? (y/n)");
        input_str(confirm, 1, 0);
        if (confirm[0] == 'y')
        {
            XMORE("Rebooting now...\n");
            sal_sys_reboot();
        }
#endif
    }
    else if (SYS_LOCAL_FILE_CONFIG_STARTUP == type
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
             || SYS_LOCAL_FILE_CONFIG_BACKUP == type
#endif
            )
    {
        ret = sal_tftp_config_download(CONF_STARTUP_TMPFILE, remoteFile, remoteHost);

        if (SYS_ERR_OK != ret)
        {
            if (SYS_LOCAL_FILE_CONFIG_STARTUP == type)
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL,
                            CONF_STARTUP_BASENAME);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
            else
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL,
                            CONF_BACKUP_BASENAME);
#endif
            return ret;
        }

        XMORE("Downloading Done\n");

        if (IS_CONFIG_FILEHDR_INVALID(CONF_STARTUP_TMPFILE))
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_CFG_INVALID);

            return SYS_ERR_CFG_INVALID;
        }

        if (SYS_LOCAL_FILE_CONFIG_STARTUP == type)
        {
            SYS_ERR_CHK(cmd_conf_startupConfig_copy(CONF_STARTUP_TMPFILE));

            SYS_ERR_CHK(config_valid_file_convert(CONF_STARTUP_FILE));

            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS,
                        CONF_STARTUP_BASENAME);
        }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        else
        {
            SYS_ERR_CHK(sal_util_file_copy(CONF_STARTUP_TMPFILE, CONF_BACKUP_FILE));

            SYS_ERR_CHK(config_valid_file_convert(CONF_BACKUP_FILE));

            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS,
                        CONF_BACKUP_BASENAME);
        }
#endif

#ifdef CONFIG_SYS_AUTOREBOOT_CONFIG
        XMORE("Rebooting now...\n");
        sal_sys_reboot();
#else
        char confirm[2];

        XMORE("Upgrade config success. Do you want to reboot now? (y/n)");
        input_str(confirm, 1, 0);
        if (confirm[0] == 'y')
        {
            XMORE("Rebooting now...\n");
            sal_sys_reboot();
        }
#endif
    }
    else if (SYS_LOCAL_FILE_CONFIG_RUNNING == type)
    {
        int32 ret = SYS_ERR_OK;
        char tmpFile[CAPA_FILE_FULLNAME_LEN];

        SYS_MEM_CLEAR(tmpFile);

        sal_util_randomfileName_gen("/tmp", "tmp_tftp", tmpFile);

        ret = sal_tftp_config_download(tmpFile, remoteFile, remoteHost);

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL,
                            CONF_RUNNING_BASENAME);

            unlink(tmpFile);
            return ret;
        }

        if (IS_CONFIG_FILEHDR_INVALID(tmpFile))
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_CFG_INVALID);

            unlink(tmpFile);
            return SYS_ERR_CFG_INVALID;
        }

        ret = _cli_sys_copyToRuuning(vty, tmpFile);

        unlink(tmpFile);

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL,
                        CONF_RUNNING_BASENAME);
        }
        else
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS,
                        CONF_RUNNING_BASENAME);
        }

        return ret;
    }
    else
    {
        XMORE("This destination is not supported.\n");
    }

    return SYS_ERR_OK;
}

static int32 _cli_sys_flash2flash(char *srcFile, char *dstFile)
{
    int32 ret = SYS_ERR_OK;
    uint32 support = TRUE;
    sys_local_file_type_t type;

    ret = _cli_sys_localFileType_get(srcFile, &type);

    if (SYS_ERR_FILE_NOT_EXIST == ret || SYS_LOCAL_FILE_CONFIG_RUNNING == type)
    {
        XMORE("Source local file is not support\n");
        return SYS_ERR_OK;
    }

    ret = _cli_sys_localFileType_get(dstFile, &type);

    if (SYS_ERR_FILE_NOT_EXIST == ret || SYS_LOCAL_FILE_CONFIG_RUNNING == type)
    {
        XMORE("Destination local file is not support\n");
        return SYS_ERR_OK;
    }

    ret = SYS_ERR_OK;

    if (0 == osal_strcmp(srcFile, CONF_RUNNING_BASENAME))
    {
        if (0 == osal_strcmp(dstFile, CONF_STARTUP_BASENAME))
            ret = cmd_conf_startupConfig_save();
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        else if (0 == osal_strcmp(dstFile, CONF_BACKUP_BASENAME))
            ret = cmd_conf_backupConfig_save(CONF_BACKUP_BASENAME);
#endif
        else
            support = FALSE;
    }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if (0 == osal_strcmp(srcFile, CONF_STARTUP_BASENAME))
    {
        if (0 == osal_strcmp(dstFile, CONF_BACKUP_BASENAME))
            ret = sal_util_file_copy(CONF_STARTUP_FILE, CONF_BACKUP_FILE);
        else
            support = FALSE;
    }
    else if (0 == osal_strcmp(srcFile, CONF_BACKUP_BASENAME))
    {
        if (0 == osal_strcmp(dstFile, CONF_STARTUP_BASENAME))
            ret = cmd_conf_startupConfig_copy(CONF_BACKUP_FILE);
        else
            support = FALSE;
    }
#endif

    if (FALSE == support)
    {
        XMORE("This source and destination combination is not supported.\n");
    }
    else
    {
        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_SAVE_FAIL,
                        dstFile,
                        srcFile);

            return ret;
        }
        else
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_SAVE_SUCCESS,
                        dstFile,
                        srcFile);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_sys_config_delete(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char confirm[2];
    char filename[CAPA_FILE_NAME_LEN];
    sys_local_file_type_t type = SYS_LOCAL_FILE_END;

    if ('s' == argv[0][0])
    {
        XMORE("Delete startup-config [y/n] ");
    }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if ('b' == argv[0][0])
    {
        XMORE("Delete backup-config [y/n] ");
    }
#endif
    else if ('f' == argv[0][0])
    {
        SYS_MEM_CLEAR(filename);

        _cli_sys_flashArgv_get(argv[0], filename);

        SYS_ERR_CHK(_cli_sys_localFileType_get(filename, &type));

        if (SYS_LOCAL_FILE_CONFIG_STARTUP != type
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
            && SYS_LOCAL_FILE_CONFIG_BACKUP != type
#endif
           )
        {
            XMORE("Delete %s is not permited\n", filename);

            return SYS_ERR_OK;
        }

        XMORE("Delete %s [y/n] ", argv[0]);
    }

    input_str(confirm, 1, 0);

    if (confirm[0] != 'y')
        return SYS_ERR_OK;

    if ('s' == argv[0][0])
    {
        SYS_ERR_CHK(sal_sys_restore_defaults(DISABLED));

        type = SYS_LOCAL_FILE_CONFIG_STARTUP;
    }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if ('b' == argv[0][0])
    {
        unlink(CONF_BACKUP_FILE);
    }
#endif
    else if ('f' == argv[0][0])
    {
        if (SYS_LOCAL_FILE_CONFIG_STARTUP == type)
            SYS_ERR_CHK(sal_sys_restore_defaults(DISABLED));
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        else if (SYS_LOCAL_FILE_CONFIG_BACKUP == type)
             unlink(CONF_BACKUP_FILE);
#endif
    }

    if (SYS_LOCAL_FILE_CONFIG_STARTUP == type)
    {
        XMORE("Do you want to reload the system to take effect? [y/n] ");

        input_str(confirm, 1, 0);

        if (confirm[0] != 'n')
            sal_sys_reboot();
    }

    return SYS_ERR_OK;
}

int32 cmd_sys_dfltConfig_restore(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    char confirm[2];

    SYS_MEM_CLEAR(lpm);

    if (1 == argc)
    {
        CMD_PORTLIST_GET(0, &lpm);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            CMD_ERR_CONTINUE_PORT(fds_port_restore(lp), lp);

            SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_INTF_RESTORE,
                        LPSTR(lp));

            XMORE("Interface %s: restore factory defaults.\n", LPSTR(lp));
        }
    }
    else
    {
        SYS_ERR_CHK(sal_sys_restore_defaults(DISABLED));
        XMORE("System: restore factory defaults. Do you want to reboot now? (y/n)");
        input_str(confirm, 1, 0);
        if (confirm[0] == 'y')
        {
            XMORE("Rebooting now...\n");
            sal_sys_reboot();
        }
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_DUAL_IMAGE

int32 cmd_sys_image_boot(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 partition = SYS_DFLT_FW_PARTITION;

    SYS_ERR_CHK(sal_sys_dualImageCurrSel_get(&partition));

    if ((0 == partition && (0 == osal_strcmp(SYS_FW_NAME_IMG0, argv[0])))
        || (1 == partition && (0 == osal_strcmp(SYS_FW_NAME_IMG1, argv[0]))))
    {
        XMORE("The image already be selected\n");
    }
    else if (0 == partition && (0 == osal_strcmp(SYS_FW_NAME_IMG1, argv[0])))
    {
        SYS_ERR_CHK(sal_sys_dualImage_sel(1));
        XMORE("Select \"%s\" Success\n", SYS_FW_NAME_IMG1);
    }
    else if (1 == partition && (0 == osal_strcmp(SYS_FW_NAME_IMG0, argv[0])))
    {
        SYS_ERR_CHK(sal_sys_dualImage_sel(0));
        XMORE("Select \"%s\" Success\n", SYS_FW_NAME_IMG0);
    }
    else
    {
        XMORE("Image file not exist\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_sys_image_delete(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 partition = SYS_DFLT_FW_PARTITION;

    SYS_ERR_CHK(sal_sys_dualImageCurrSel_get(&partition));

    if ((0 == partition && (0 == osal_strcmp(SYS_FW_NAME_IMG0, argv[0])))
        || (1 == partition && (0 == osal_strcmp(SYS_FW_NAME_IMG1, argv[0]))))
    {
        return SYS_ERR_IMAGE_ERASE_ACTIVE;
    }
    else if (0 == partition && (0 == osal_strcmp(SYS_FW_NAME_IMG1, argv[0])))
    {
        XMORE("It may take a few minutes to finish. Please wait...");
        SYS_ERR_CHK(sal_sys_dualImage_erase(1));
        XMORE("Delete \"%s\" Success\n", SYS_FW_NAME_IMG1);
    }
    else if (1 == partition && (0 == osal_strcmp(SYS_FW_NAME_IMG0, argv[0])))
    {
        XMORE("It may take a few minutes to finish. Please wait...");
        SYS_ERR_CHK(sal_sys_dualImage_erase(0));
        XMORE("Delete \"%s\" Success\n", SYS_FW_NAME_IMG0);
    }
    else
    {
        XMORE("Image file not exist\n");
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_sys_flash_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i = SYS_DFLT_FW_PARTITION;
    uint32 magic;
    uint32 tmp_time;
    time_t time;
    struct tm *tm_time;
    char name[32];
    uint32 size;
    char date[SYS_BUF32_LEN];
#ifdef CONFIG_SYS_DUAL_IMAGE
    int32 ret = SYS_ERR_FAILED;
    uint32 currSelPart = 0;
#endif

    XMORE("        File Name            File Size        Modified\n");
    XMORE("------------------------ ---------------- ------------------------\n");
    
    if (0 != (size = sal_util_fileSize_ret(CONF_STARTUP_FILE)))
    {
        SYS_MEM_CLEAR(date);
        sal_util_fileDateStr_get(CONF_STARTUP_FILE, date);
        XMORE(" %-22s   %-14u   %s\n", CONF_STARTUP_BASENAME, size, date);
    }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    
    if (0 != (size = sal_util_fileSize_ret(CONF_BACKUP_FILE)))
    {
        SYS_MEM_CLEAR(date);
        sal_util_fileDateStr_get(CONF_BACKUP_FILE, date);
        XMORE(" %-22s   %-14u   %s\n", CONF_BACKUP_BASENAME, size, date);
    }
#endif
    
    if (0 != (size = sal_util_fileSize_ret(LOG_FILE_FLASH)))
    {
        SYS_MEM_CLEAR(date);
        sal_util_fileDateStr_get(LOG_FILE_FLASH, date);
        XMORE(" %-22s   %-14u   %s\n", LOG_FILE_FLASH_BASENAME, size, date);
    }
#ifdef CONFIG_SYS_APP_SSH
    
    if (0 != (size = sal_util_fileSize_ret(SYS_SSH_RSAV1_PRIKEY_FILE)))
    {
        SYS_MEM_CLEAR(date);
        sal_util_fileDateStr_get(SYS_SSH_RSAV1_PRIKEY_FILE, date);
        XMORE(" %-22s   %-14u   %s\n", SSH_RSA1, size, date);
    }
    
    if (0 != (size = sal_util_fileSize_ret(SYS_SSH_RSAV2_PRIKEY_FILE)))
    {
        SYS_MEM_CLEAR(date);
        sal_util_fileDateStr_get(SYS_SSH_RSAV2_PRIKEY_FILE, date);
        XMORE(" %-22s   %-14u   %s\n", SSH_RSA2, size, date);
    }
    
    if (0 != (size = sal_util_fileSize_ret(SYS_SSH_DSAV2_PRIKEY_FILE)))
    {
        SYS_MEM_CLEAR(date);
        sal_util_fileDateStr_get(SYS_SSH_DSAV2_PRIKEY_FILE, date);
        XMORE(" %-22s   %-14u   %s\n", SSH_DSA2, size, date);
    }
#endif
    
    if (0 != (size = sal_util_fileSize_ret(SSL_CERT_FILE)))
    {
        SYS_MEM_CLEAR(date);
        sal_util_fileDateStr_get(SSL_CERT_FILE, date);
        XMORE(" %-22s   %-14u   %s\n", SSL_CERT, size, date);
    }

#ifdef CONFIG_SYS_DUAL_IMAGE
    SYS_ERR_CHK(sal_sys_dualImageCurrSel_get(&currSelPart));

    for (i = 0; i <= 1; i++)
#else
    i = 0;
#endif
    {
#ifdef CONFIG_SYS_DUAL_IMAGE
        ret = sal_sys_fwImageHdrInfo_get(i, &magic, &tmp_time, &size, name);

        osal_sprintf(name, "image%d (%s)", i, (i == currSelPart) ? "active" : "backup");

        if (ret == SYS_ERR_IMAGE_INVALID)
        {
            XMORE(" %-22s   %-14u\n", name, 0);
            continue;
        }
#else
        sal_sys_fwImageHdrInfo_get(i, &magic, &tmp_time, &size, name);

        osal_sprintf(name, "image");
#endif

        XMORE(" %-22s   %-14u", name, size);

        time = (time_t) ntohl(tmp_time);
        tm_time = gmtime(&time);
        XMORE("   %4d-%02d-%02d %02d:%02d:%02d\n",
               (1900 + tm_time->tm_year),
               (1 + tm_time->tm_mon),
               tm_time->tm_mday,
               tm_time->tm_hour,
               tm_time->tm_min,
               tm_time->tm_sec);

    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_DUAL_IMAGE

int32 cmd_sys_bootvar_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int32 ret;
    uint32 i;
    uint32 currSelPart = 0;
    uint32 currActPart = 0;
    uint32 magic;
    uint32 tmp_time;
    time_t time;
    struct tm *tm_time;
    char name[SYS_BUF32_LEN];
    sys_filenamestr_t filename;
    char dateStr[SYS_BUF32_LEN];
    char status[SYS_BUF16_LEN];
    uint32 size;

    sal_sys_currActiveImageIdx_get(&currActPart);
    sal_sys_dualImageCurrSel_get(&currSelPart);

    XMORE("Image  Version      Date                    Status       File Name   \n");
    XMORE("-----  ----------   ---------------------   -----------  ----------  \n");
    SYS_MEM_CLEAR(dateStr);
    SYS_MEM_CLEAR(name);
    SYS_MEM_CLEAR(status);
    SYS_MEM_CLEAR(filename);
    for (i = 0; i <= 1; i++)
    {
        ret = sal_sys_fwImageHdrInfo_get(i, &magic, &tmp_time, &size, name);

        if (SYS_ERR_OK != ret)
        {
            SYS_MEM_CLEAR(dateStr);
            SYS_MEM_CLEAR(name);
        }
        else
        {
            time = (time_t) ntohl(tmp_time);
            tm_time = gmtime(&time);
            osal_sprintf(dateStr, "%4d-%02d-%02d %02d:%02d:%02d",
                                  (1900 + tm_time->tm_year),
                                  (1 + tm_time->tm_mon),
                                  tm_time->tm_mday,
                                  tm_time->tm_hour,
                                  tm_time->tm_min,
                                  tm_time->tm_sec);
        }

        if (i == currActPart)
            osal_strcpy(status, "Active");
        else
            osal_strcpy(status, "Not active");

        if (i == currSelPart)
            osal_strcat(status, "*");

        sal_sys_dualImageFileName_get(i, &filename);

        XMORE("%d      %11s  %-21s   %-11s  %-s\n", i,
                                                  name,
                                                  dateStr,
                                                  status,
                                                  filename.str);

        SYS_MEM_CLEAR(dateStr);
        SYS_MEM_CLEAR(name);
        SYS_MEM_CLEAR(status);
        SYS_MEM_CLEAR(filename);
    }

    XMORE("\n\"*\" designates that the image was selected for the next boot\n\n");

    return SYS_ERR_OK;
}
#endif

int32 cmd_sys_config_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    FILE *fp;
    char fullname[CAPA_FILE_FULLNAME_LEN];
    char filename[CAPA_FILE_NAME_LEN];
    char cmdstr[MAX_CMD_STRLEN];

    SYS_MEM_CLEAR(fullname);
    SYS_MEM_CLEAR(filename);

    if ('s' == argv[0][0])
    {
        osal_strcpy(fullname, CONF_STARTUP_FILE);
        osal_strcpy(filename, CONF_STARTUP_BASENAME);
    }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if ('b' == argv[0][0])
    {
        osal_strcpy(fullname, CONF_BACKUP_FILE);
        osal_strcpy(filename, CONF_BACKUP_BASENAME);
    }
#endif
    else if ('r' == argv[0][0])
    {
        SYS_ERR_CHK(cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE));

        osal_strcpy(fullname, CONF_RUNNING_FILE);
        osal_strcpy(filename, CONF_RUNNING_BASENAME);
    }
    else
    {
        return SYS_ERR_FAILED;
    }

    if ((NULL == (fp = fopen(fullname , "r"))))
    {
        XMORE("Config file %s is not existed\n", filename);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(cmdstr);

    while (fgets(cmdstr, MAX_CMD_STRLEN, fp))
    {
        cmdstr[osal_strlen(cmdstr) - 1] = '\0';
        XMORE_HDL(_exit, "%s\n", cmdstr);
        SYS_MEM_CLEAR(cmdstr);
    }

_exit:
    fclose(fp);

    return SYS_ERR_OK;
}

int32 cmd_sys_config_show_features_module(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    FILE *fp;
    char fullname[CAPA_FILE_FULLNAME_LEN];
    char cmdstr[MAX_CMD_STRLEN];

    SYS_MEM_CLEAR(fullname);

	if(argv > 0)
		SYS_ERR_CHK(cmd_parser_config_by_module_print(argv[0], CONF_RUNNING_FILE));
	else 
		return SYS_ERR_OK;

    osal_strcpy(fullname, CONF_RUNNING_FILE);

    if ((NULL == (fp = fopen(fullname , "r"))))
    {
        XMORE("Config file %s is not existed\n", fullname);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(cmdstr);

    while (fgets(cmdstr, MAX_CMD_STRLEN, fp))
    {
        cmdstr[osal_strlen(cmdstr) - 1] = '\0';
        XMORE_HDL(_exit, "%s\n", cmdstr);
        SYS_MEM_CLEAR(cmdstr);
    }

_exit:
    fclose(fp);

    return SYS_ERR_OK;
}

int32 cmd_sys_config_save(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int32 ret = SYS_ERR_FAILED;
    char srcFile[CAPA_FILE_NAME_LEN];
    char dstFile[CAPA_FILE_NAME_LEN];

    SYS_MEM_CLEAR(srcFile);
    SYS_MEM_CLEAR(dstFile);

    if (0 == argc)
    {
        if (1 == CMD_TOKENNUM_GET(cmd->vline))
        {
            ret = cmd_conf_startupConfig_save();

            osal_strcpy(srcFile, CONF_RUNNING_BASENAME);
            osal_strcpy(dstFile, CONF_STARTUP_BASENAME);
        }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        else if ('s' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
        {
            ret = sal_util_file_copy(CONF_STARTUP_FILE, CONF_BACKUP_FILE);

            osal_strcpy(srcFile, CONF_STARTUP_BASENAME);
            osal_strcpy(dstFile, CONF_BACKUP_BASENAME);
        }
        else if ('b' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
        {
            ret = cmd_conf_startupConfig_copy(CONF_BACKUP_FILE);

            osal_strcpy(srcFile, CONF_BACKUP_BASENAME);
            osal_strcpy(dstFile, CONF_STARTUP_BASENAME);
        }
#endif
    }
    else if ('s' == argv[0][0])
    {
        ret = cmd_conf_startupConfig_save();

        osal_strcpy(srcFile, CONF_RUNNING_BASENAME);
        osal_strcpy(dstFile, CONF_STARTUP_BASENAME);
    }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if ('b' == argv[0][0])
    {
        ret = cmd_conf_backupConfig_save(CONF_BACKUP_BASENAME);

        osal_strcpy(srcFile, CONF_BACKUP_BASENAME);
        osal_strcpy(dstFile, CONF_STARTUP_BASENAME);
    }
#endif

    if (SYS_ERR_OK == ret)
        XMORE("Success\n");
    else if(SYS_ERR_FILE_OPEN == ret)
        XMORE("Empty specified configuration. Please save your configuration with the appropriate command.\n");
    else if(SYS_ERR_FILE_WRITE == ret)
        XMORE("Save configuration failed.\n");
    else
        XMORE("Save configuration failed.\n");

    if (SYS_ERR_OK != ret)
    {
        SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_SAVE_FAIL,
                    dstFile,
                    srcFile);
    }
    else
    {
        SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_SAVE_SUCCESS,
                    dstFile,
                    srcFile);
    }

    return SYS_ERR_OK;
}

int32 cmd_sys_file_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char localFile[CAPA_FILE_NAME_LEN];
    char localFile2[CAPA_FILE_NAME_LEN];
    char remoteHost[CAPA_HOSTNAME_LEN + 1];
    char remoteFile[CAPA_FILE_NAME_LEN];

    SYS_MEM_CLEAR(localFile);
    SYS_MEM_CLEAR(localFile2);
    SYS_MEM_CLEAR(remoteHost);
    SYS_MEM_CLEAR(remoteFile);

    if ((0 == osal_strncmp(argv[0], "flash://", 8))
            && (0 == osal_strncmp(argv[1], "tftp://", 7)))
    {
        SYS_ERR_CHK(_cli_sys_flashArgv_get(argv[0], localFile));
        SYS_ERR_CHK(_cli_sys_tftpArgv_get(argv[1], remoteFile, remoteHost));

        if (0 == osal_strlen(remoteFile))
            osal_strcpy(remoteFile, localFile);

        SYS_ERR_CHK(_cli_sys_flash2tftp(localFile, remoteFile, remoteHost));
    }
    else if ((0 == osal_strncmp(argv[0], "tftp://", 7))
                && (0 == osal_strncmp(argv[1], "flash://", 8)))
    {
        SYS_ERR_CHK(_cli_sys_tftpArgv_get(argv[0], remoteFile, remoteHost));
        SYS_ERR_CHK(_cli_sys_flashArgv_get(argv[1], localFile));

        if (0 == osal_strlen(remoteFile))
            osal_strcpy(remoteFile, localFile);

        SYS_ERR_CHK(_cli_sys_tftp2flash(vty, localFile, remoteFile, remoteHost));
    }
    else if ((0 == osal_strncmp(argv[0], "flash://", 8))
                && (0 == osal_strncmp(argv[1], "flash://", 8)))
    {
        SYS_ERR_CHK(_cli_sys_flashArgv_get(argv[0], localFile));
        SYS_ERR_CHK(_cli_sys_flashArgv_get(argv[1], localFile2));

        SYS_ERR_CHK(_cli_sys_flash2flash(localFile, localFile2));
    }

    return SYS_ERR_OK;
}

int32 cmd_sys_config2Tftp_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int32 ret = SYS_ERR_FAILED;
    char localFile[CAPA_FILE_NAME_LEN];
    char remoteHost[CAPA_HOSTNAME_LEN + 1];
    char remoteFile[CAPA_FILE_NAME_LEN];

    SYS_MEM_CLEAR(localFile);
    SYS_MEM_CLEAR(remoteHost);
    SYS_MEM_CLEAR(remoteFile);

    if ('s' == argv[0][0])
        osal_strcpy(localFile, CONF_STARTUP_BASENAME);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if ('b' == argv[0][0])
        osal_strcpy(localFile, CONF_BACKUP_BASENAME);
#endif
    else if ('r' == argv[0][0])
        osal_strcpy(localFile, CONF_RUNNING_BASENAME);

    SYS_ERR_CHK(_cli_sys_tftpArgv_get(argv[1], remoteFile, remoteHost));

    if (0 == osal_strlen(remoteFile))
        osal_strcpy(remoteFile, localFile);

    ret = _cli_sys_flash2tftp(localFile, remoteFile, remoteHost);

    if (SYS_ERR_OK == ret)
        XMORE("Success\n");
    else if(SYS_ERR_FILE_OPEN == ret)
        XMORE("Empty specified configuration. Please save your configuration with the appropriate command.\n");
    else if(SYS_ERR_FILE_WRITE == ret)
        XMORE("Save configuration failed.\n");
    else
        XMORE("Save configuration failed.\n");

    return SYS_ERR_OK;
}

int32 cmd_sys_tftp2Config_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char localFile[CAPA_FILE_NAME_LEN];
    char remoteHost[CAPA_HOSTNAME_LEN + 1];
    char remoteFile[CAPA_FILE_NAME_LEN];

    SYS_MEM_CLEAR(localFile);
    SYS_MEM_CLEAR(remoteHost);
    SYS_MEM_CLEAR(remoteFile);

    SYS_ERR_CHK(_cli_sys_tftpArgv_get(argv[0], remoteFile, remoteHost));

    if ('s' == argv[1][0])
        osal_strcpy(localFile, CONF_STARTUP_BASENAME);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if ('b' == argv[1][0])
        osal_strcpy(localFile, CONF_BACKUP_BASENAME);
#endif
    else if ('r' == argv[1][0])
        osal_strcpy(localFile, CONF_RUNNING_BASENAME);

    if (0 == osal_strlen(remoteFile))
        osal_strcpy(remoteFile, localFile);

    SYS_ERR_CHK(_cli_sys_tftp2flash(vty, localFile, remoteFile, remoteHost));

    return SYS_ERR_OK;
}

int32 cmd_sys_config2Running_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char localFile[CAPA_FILE_FULLNAME_LEN];
    int32 ret = SYS_ERR_FAILED;

    SYS_MEM_CLEAR(localFile);

    if ('s' == argv[0][0])
        osal_strcpy(localFile, CONF_STARTUP_FILE);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if ('b' == argv[0][0])
        osal_strcpy(localFile, CONF_BACKUP_FILE);
#endif

    ret = _cli_sys_copyToRuuning(vty, localFile);

    if (SYS_ERR_OK == ret)
        XMORE("Success\n");
    else if(SYS_ERR_FILE_OPEN == ret)
        XMORE("Empty specified configuration. Please save your configuration with the appropriate command.\n");
    else if(SYS_ERR_FILE_WRITE == ret)
        XMORE("Save configuration failed.\n");
    else
        XMORE("Save configuration failed.\n");

    return SYS_ERR_OK;
}

int32 cmd_sys_tftp2logo_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char localFile[CAPA_FILE_NAME_LEN];
    char remoteHost[CAPA_HOSTNAME_LEN + 1];
    char remoteFile[CAPA_FILE_NAME_LEN];

    SYS_MEM_CLEAR(localFile);
    SYS_MEM_CLEAR(remoteHost);
    SYS_MEM_CLEAR(remoteFile);

    SYS_ERR_CHK(_cli_sys_tftpArgv_get(argv[0], remoteFile, remoteHost));

    if ('s' == argv[1][0])
        osal_strcpy(localFile, CONF_STARTUP_BASENAME);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    else if ('b' == argv[1][0])
        osal_strcpy(localFile, CONF_BACKUP_BASENAME);
#endif
    else if ('r' == argv[1][0])
        osal_strcpy(localFile, CONF_RUNNING_BASENAME);

    if (0 == osal_strlen(remoteFile))
        osal_strcpy(remoteFile, localFile);

    SYS_ERR_CHK(_cli_sys_tftp2flash(vty, localFile, remoteFile, remoteHost));

    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_HW_USB
static int32 _cli_sys_usbArgv_get(char *string, char *filename)
{
    char *file = NULL;

    file = string + osal_strlen("usb://");

    if (osal_strlen(file) > CAPA_FILE_NAME_LEN)
        return SYS_ERR_INPUT;

    osal_strcpy(filename, file);

    return SYS_ERR_OK;
}

static int32 _cli_sys_flash2usb(char *localFile, char *remoteFile)
{
    int32 ret = SYS_ERR_OK;
    sys_local_file_type_t type;

    ret = _cli_sys_localFileType_get(localFile, &type);

    if (SYS_ERR_FILE_NOT_EXIST == ret)
    {
        XMORE("Local file is not support\n");
        return SYS_ERR_OK;
    }

    XMORE("Copying file. Please wait...\n");

    switch (type)
    {
        case SYS_LOCAL_FILE_CONFIG_STARTUP:
            ret = sal_config2usb(CONF_STARTUP_FILE, remoteFile);
            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_FAIL, CONF_STARTUP_BASENAME);
                SNMP_CMD_TRAP("#upload config *%d $upload startup-config file fail, file name is %s success", getpid(), remoteFile);
                return ret;
            }
            else
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_SUCCESS, CONF_STARTUP_BASENAME);
                SNMP_CMD_TRAP("#upload config *%d $upload startup-config file success, file name is %s success", getpid(), remoteFile);
            }
            break;

#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        case SYS_LOCAL_FILE_CONFIG_BACKUP:
            ret = sal_config2usb(CONF_BACKUP_FILE, remoteFile);

            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_FAIL, CONF_BACKUP_BASENAME);
                SNMP_CMD_TRAP("#upload config *%d $upload backup-config file fail, file name is %s success", getpid(), remoteFile);
                return ret;
            }
            else
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_SUCCESS, CONF_BACKUP_BASENAME);
                SNMP_CMD_TRAP("#upload config *%d $upload backup-config file success, file name is %s success", getpid(), remoteFile);
            }
            break;
#endif

        case SYS_LOCAL_FILE_CONFIG_RUNNING:
            SYS_ERR_CHK(cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE));
            ret = sal_config2usb(CONF_RUNNING_FILE, remoteFile);

            if (SYS_ERR_OK != ret)
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_FAIL, CONF_RUNNING_BASENAME);
                SNMP_CMD_TRAP("#upload config *%d $upload run-config file fail, file name is %s success", getpid(), remoteFile);
                return ret;
            }
            else
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_BACKUP_SUCCESS, CONF_RUNNING_BASENAME);
                SNMP_CMD_TRAP("#upload config *%d $upload run-config file success, file name is %s success", getpid(), remoteFile);
            }
            break;

#ifdef CONFIG_SYS_SYSLOG
        case SYS_LOCAL_FILE_LOG_RAM:
            ret = sal_config2usb(LOG_FILE_RAM, remoteFile);

            if (SYS_ERR_TFTP_UPLOAD_CONFIG == ret)
            {
                SYS_PRINTF("Upload log file to TFTP server failed\n");
                return SYS_ERR_OK;
            }
            else if (SYS_ERR_OK != ret)
            {
                return ret;
            }
            break;

        case SYS_LOCAL_FILE_LOG_FLASH:
            ret = sal_config2usb(LOG_FILE_FLASH, remoteFile);

            if (SYS_ERR_TFTP_UPLOAD_CONFIG == ret)
            {
                SYS_PRINTF("Upload log file to TFTP server failed\n");
                return SYS_ERR_OK;
            }
            else if (SYS_ERR_OK != ret)
            {
                return ret;
            }
            break;
#endif
#ifdef CONFIG_SYS_APP_SSH
        case SYS_LOCAL_FILE_SSH_RSA1:
            SYS_ERR_CHK(sal_config2usb(SYS_SSH_RSAV1_PRIKEY_FILE, remoteFile));
            break;

        case SYS_LOCAL_FILE_SSH_RSA2:
            SYS_ERR_CHK(sal_config2usb(SYS_SSH_RSAV2_PRIKEY_FILE, remoteFile));
            break;

        case SYS_LOCAL_FILE_SSH_DSA2:
            SYS_ERR_CHK(sal_config2usb(SYS_SSH_DSAV2_PRIKEY_FILE, remoteFile));
            break;
#endif
        case SYS_LOCAL_FILE_SSL_CERT:
            SYS_ERR_CHK(sal_config2usb(SSL_CERT_FILE, remoteFile));
            break;

        default:
            break;
    }

    XMORE("Copying Done\n");

    return SYS_ERR_OK;
}

static int32 _cli_sys_usb2flash(struct vty *vty, char *localFile, char *remoteFile)
{
    int32 ret = SYS_ERR_OK;
    sys_local_file_type_t type;

    if (osal_strlen(localFile) >= sizeof(sys_filenamestr_t) ||
        osal_strlen(remoteFile) >= sizeof(sys_filenamestr_t))
        return SYS_ERR_INPUT;

    ret = _cli_sys_localFileType_get(localFile, &type);

    if (SYS_ERR_FILE_NOT_EXIST == ret)
    {
        XMORE("Local file is not support\n");
        return SYS_ERR_OK;
    }

    XMORE("Copying file. Please wait...\n");

    if (SYS_LOCAL_FILE_IMAGE_0 == type || SYS_LOCAL_FILE_IMAGE_1 == type)
    {
        uint32 partition = 0;

        cmd_util_progressPrint_start();

        if (SYS_ERR_OK != ret)
        {
            cmd_util_progressPrint_stop();
            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_FAIL, partition, "CLI", "TFTP");
            return ret;
        }

        CLI_INTR_FORBID();

        if (SYS_LOCAL_FILE_IMAGE_0 == type)
            partition = 0;
        else
            partition = 1;

        ret = sal_sys_fwImage_write(TMP_FWIMG_FILENAME, partition, (sys_filenamestr_t *)remoteFile);

        if (SYS_ERR_OK != ret)
        {
            CLI_INTR_ALLOW();
            cmd_util_progressPrint_stop();
            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_FAIL, partition, "CLI", "TFTP");
            return ret;
        }
        else
        {
            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_SUCCESS, partition, remoteFile, "CLI", "TFTP");
        }

        CLI_INTR_ALLOW();
        cmd_util_progressPrint_stop();

        char confirm[2];
        XMORE("Upgrade firmware success. Do you want to reboot now? (y/n)");
        input_str(confirm, 1, 0);
        if (confirm[0] == 'y')
        {
            XMORE("Rebooting now...\n");
            sal_sys_reboot();
        }
    }
    else if (SYS_LOCAL_FILE_CONFIG_STARTUP == type
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
             || SYS_LOCAL_FILE_CONFIG_BACKUP == type
#endif
            )
    {
        ret = sal_usb_config_download(CONF_STARTUP_TMPFILE, remoteFile);

        if (SYS_ERR_OK != ret)
        {
            if (SYS_LOCAL_FILE_CONFIG_STARTUP == type)
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL, CONF_STARTUP_BASENAME);
				SNMP_CMD_TRAP("#download config *%d $download startup-config file success, file name is %s success", getpid(), remoteFile);
            }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
            else
            {
                SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL, CONF_BACKUP_BASENAME);
				SNMP_CMD_TRAP("#download config *%d $download startup-config file fail, file name is %s success", getpid(), remoteFile);
            }
#endif
            return ret;
        }

        XMORE("Copying Done\n");

        if (IS_CONFIG_FILEHDR_INVALID(CONF_STARTUP_TMPFILE))
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_CFG_INVALID);
            return SYS_ERR_CFG_INVALID;
        }

        if (SYS_LOCAL_FILE_CONFIG_STARTUP == type)
        {
            SYS_ERR_CHK(cmd_conf_startupConfig_copy(CONF_STARTUP_TMPFILE));
            
            SYS_ERR_CHK(config_valid_file_convert(CONF_STARTUP_FILE));
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS, CONF_STARTUP_BASENAME);
            SNMP_CMD_TRAP("#save config *%d $save startup-config file success", getpid());
        }
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        else
        {
            SYS_ERR_CHK(sal_util_file_copy(CONF_STARTUP_TMPFILE, CONF_BACKUP_FILE));
            
            SYS_ERR_CHK(config_valid_file_convert(CONF_BACKUP_FILE));
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS, CONF_BACKUP_BASENAME);
        }
#endif

        char confirm[2];

        XMORE("Upgrade config success. Do you want to reboot now? (y/n)");
        input_str(confirm, 1, 0);
        if (confirm[0] == 'y')
        {
            XMORE("Rebooting now...\n");
            sal_sys_reboot();
        }
    }
    else if (SYS_LOCAL_FILE_CONFIG_RUNNING == type)
    {
        int32 ret = SYS_ERR_OK;
        char tmpFile[CAPA_FILE_FULLNAME_LEN];

        SYS_MEM_CLEAR(tmpFile);

        sal_util_randomfileName_gen("/tmp", "tmp_tftp", tmpFile);

        ret = sal_usb_config_download(tmpFile, remoteFile);

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL, CONF_RUNNING_BASENAME);
            SNMP_CMD_TRAP("#download config *%d $download run-config file success, file name is %s success", getpid(), remoteFile);
            unlink(tmpFile);
            return ret;
        }

        if (IS_CONFIG_FILEHDR_INVALID(tmpFile))
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_CFG_INVALID);
            unlink(tmpFile);
            return SYS_ERR_CFG_INVALID;
        }

        XMORE("Copying Done, Start loading to running-config, Please wait...\n");
        ret = _cli_sys_copyToRuuning(vty, tmpFile);
        unlink(tmpFile);
        XMORE("Loading finish.\n");

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL, CONF_RUNNING_BASENAME);
            SNMP_CMD_TRAP("#run config *%d $run startup-config file fail", getpid());
        }
        else
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS, CONF_RUNNING_BASENAME);
            SNMP_CMD_TRAP("#run config *%d $run startup-config file success", getpid());
        }

        return ret;
    }
    else
    {
        XMORE("This destination is not supported.\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_sys_usb_enable(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 ret = SYS_ERR_FAILED;
	DIR *dir = NULL;

	if ((dir = opendir(SYS_USB_DIR)) != NULL)
	{
		closedir(dir);
		return SYS_ERR_OK;
	}
	
    ret = sal_sys_usb_enable();
	if(SYS_ERR_OK != ret)
	{
		XMORE("USB flash drive install Failed.\n");
	}
	else
    {
        XMORE("USB flash drive install Success.\n");
    }

	return SYS_ERR_OK;
}

int32 cmd_sys_usb_disable(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 ret = SYS_ERR_FAILED;
	DIR *dir = NULL;

	if ((dir = opendir(SYS_USB_DIR)) == NULL)
	{
		return SYS_ERR_OK;
	}
	closedir(dir);

    ret = sal_sys_usb_disable();
	if(SYS_ERR_OK != ret)
	{
		XMORE("USB flash drive remove Failed.\n");
	}
	else
    {
        XMORE("USB flash drive remove Success.\n");
    }
	return SYS_ERR_OK;

}

int32 cmd_sys_config2usb_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 ret = SYS_ERR_FAILED;
	char localFile[CAPA_FILE_NAME_LEN];
	char remoteFile[CAPA_FILE_NAME_LEN];
    DIR *dir = NULL;

	if ((dir = opendir(SYS_USB_DIR)) == NULL)
	{
		XMORE("Please install the usb device first. Use <usb install> command.\n");
		return SYS_ERR_OK;
	}
	closedir(dir);

	SYS_MEM_CLEAR(localFile);
	SYS_MEM_CLEAR(remoteFile);

	if ('s' == argv[0][0])
		osal_strcpy(localFile, CONF_STARTUP_BASENAME);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
	else if ('b' == argv[0][0])
		osal_strcpy(localFile, CONF_BACKUP_BASENAME);
#endif
	else if ('r' == argv[0][0])
		osal_strcpy(localFile, CONF_RUNNING_BASENAME);

	SYS_ERR_CHK(_cli_sys_usbArgv_get(argv[1], remoteFile));

	ret = _cli_sys_flash2usb(localFile, remoteFile);
	if (SYS_ERR_OK != ret)
	{
		XMORE("Save configuration failed.\n");
    	return SYS_ERR_OK;
	}

	XMORE("Save configuration Success.\n");
	return SYS_ERR_OK;

}

int32 cmd_sys_usb2config_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	int32 ret = SYS_ERR_FAILED;
	char localFile[CAPA_FILE_NAME_LEN];
	char remoteFile[CAPA_FILE_NAME_LEN];
	DIR *dir = NULL;

	if ((dir = opendir(SYS_USB_DIR)) == NULL)
	{
		XMORE("Please install the usb device first. Use <usb install> command.\n");
		return SYS_ERR_OK;
	}
	closedir(dir);

	SYS_MEM_CLEAR(localFile);
	SYS_MEM_CLEAR(remoteFile);

	SYS_ERR_CHK(_cli_sys_usbArgv_get(argv[0], remoteFile));

	if ('s' == argv[1][0])
		osal_strcpy(localFile, CONF_STARTUP_BASENAME);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
	else if ('b' == argv[1][0])
		osal_strcpy(localFile, CONF_BACKUP_BASENAME);
#endif
	else if ('r' == argv[1][0])
		osal_strcpy(localFile, CONF_RUNNING_BASENAME);

	ret = _cli_sys_usb2flash(vty, localFile, remoteFile);
	if (SYS_ERR_OK != ret)
	{
		XMORE("Copy file failed.\n");
    	return SYS_ERR_OK;
	}

	XMORE("Load config success.\n");
	return SYS_ERR_OK;

}
int32 cmd_sys_usbDev_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	DIR *dir = NULL;

	if ((dir = opendir(SYS_USB_DIR)) == NULL)
	{
		XMORE("Please install the usb device first. Use <usb install> command.\n");
		return SYS_ERR_OK;
	}
	closedir(dir);

    sal_sys_usb_show();
	XMORE("\n");

	return SYS_ERR_OK;
}
#endif

int32 cmd_banner_file_cfg(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char info[SYS_BUF1024_LEN+1];
	FILE *fp;

    SYS_MEM_CLEAR(info);

	XMORE("Please input banner content. and quit with the character '%%' \r\n");
	banner_input_str(info, sizeof(info), FALSE, TRUE);

	if(sal_util_file_exist (CONF_BANNER_USR_FILE) == 0)
	{
		unlink(CONF_BANNER_USR_FILE);
	}

	if (NULL == (fp = fopen(CONF_BANNER_USR_FILE, "w")))
		return SYS_ERR_FILE_OPEN;
		
	fwrite(info, 1, osal_strlen(info) + 1, fp);
	
	fclose(fp);
	XMORE("\r\n");

    return SYS_ERR_OK;
}

