
#include <libsal/sal_sys_tech.h>
#include <libcmd/cmd.h>

static int32 _tech_sys_tftpArgv_get(char *string, char *filename, char *hostname)
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

int32 cmd_tech_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    FILE *fp;
    char cmdstr[MAX_CMD_STRLEN];
    int32 ret = SYS_ERR_FAILED;

    CLI_INTR_FORBID();

    ret = sal_tech_fileLock_check();

    if (SYS_ERR_FAILED == ret)
    {
        XMORE("Text file is generating from others\n");
        return SYS_ERR_OK;
    }

    if (NULL != (fp = fopen(SYS_POLLD_ACT_WEB_TECH_LOCK, "w")))
    {
        fprintf(fp, "LOCK");
        fclose(fp);
    }

    cmd_util_progressPrint_start();

    if (argc == 1)
        SYS_ERR_CHK(sal_tech_file_gen(argv[0]));
    else
        SYS_ERR_CHK(sal_tech_file_gen(""));

    if (NULL == (fp = fopen(CONF_TECH_SUPPORT_FILE, "r")))
    {
        cmd_util_progressPrint_stop();
        XMORE("Text file is not existed\n");
        return SYS_ERR_OK;
    }

    vty_more_forbid();

    cmd_util_progressPrint_stop();
    CLI_INTR_ALLOW();

    SYS_MEM_CLEAR(cmdstr);

    while (fgets(cmdstr, MAX_CMD_STRLEN, fp))
    {
        cmdstr[osal_strlen(cmdstr) - 1] = '\0';

        if ( (1 != argc) &&
        (0 == (osal_strncmp(cmdstr, "------------------ Mib Counters ------------------"
                    , osal_strlen(
"------------------ Mib Counters ------------------")))))
            break;

        xmore_out("%s\n", cmdstr);
        SYS_MEM_CLEAR(cmdstr);
    }
    fclose(fp);

    unlink(SYS_POLLD_ACT_WEB_TECH_LOCK);
    unlink(CONF_TECH_SUPPORT_FILE);

    vty_more_normal();

    return SYS_ERR_OK;
}

int32 cmd_tech_Support2Tftp_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int32 ret = SYS_ERR_FAILED;
    char localFile[CAPA_FILE_NAME_LEN];
    char remoteHost[CAPA_HOSTNAME_LEN + 1];
    char remoteFile[CAPA_FILE_NAME_LEN];
    FILE *fp;

    SYS_MEM_CLEAR(localFile);
    SYS_MEM_CLEAR(remoteHost);
    SYS_MEM_CLEAR(remoteFile);

    fp = fopen(SYS_POLLD_ACT_WEB_TECH_LOCK, "r");

    if (NULL != fp)
    {
        fclose(fp);
        XMORE("Text file is generating from others\n");
        return SYS_ERR_OK;
    }

    if (NULL != (fp = fopen(SYS_POLLD_ACT_WEB_TECH_LOCK, "w")))
    {
        fprintf(fp, "LOCK");
        fclose(fp);
    }

    SYS_ERR_CHK(sal_tech_file_gen(""));

    osal_strcpy(localFile, CONF_TECH_SUPPORT_NAME);

    SYS_ERR_CHK(_tech_sys_tftpArgv_get(argv[0], remoteFile, remoteHost));

    if (0 == osal_strlen(remoteFile))
        osal_strcpy(remoteFile, localFile);

    if (SYS_ERR_FILE_NOT_EXIST == sal_util_file_exist(CONF_TECH_SUPPORT_FILE))
    {
        XMORE("Local file is not support\n");
        return SYS_ERR_OK;
    }

    XMORE("Uploading file. Please wait...\n");

    ret = sal_tftp_config_upload(CONF_TECH_SUPPORT_FILE, remoteFile, remoteHost);

    XMORE("Uploading Done\n");

    if (SYS_ERR_OK == ret)
        XMORE("Success\n");
    else if(SYS_ERR_FILE_OPEN == ret)
        XMORE("Empty specified configuration. Please save your configuration with the appropriate command.\n");
    else if(SYS_ERR_FILE_WRITE == ret)
        XMORE("Save configuration failed.\n");
    else
        XMORE("Save configuration failed.\n");

    unlink(SYS_POLLD_ACT_WEB_TECH_LOCK);

    return SYS_ERR_OK;
}

