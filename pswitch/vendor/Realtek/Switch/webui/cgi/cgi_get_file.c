
#include <include/cgi.h>

static int32 cgi_file_img_get(void);
static int32 cgi_file_cfg_get(void);
static int32 cgi_file_backupStatus_get(void);
static int32 cgi_file_upgradeStatus_get(void);
#ifdef CONFIG_SYS_DUAL_IMAGE
static int32 cgi_file_dualImage_get(void);
#endif
static int32 cgi_file_cfgSave_get(void);
static int32 cgi_file_cfgSaveStatus_get(void);

const cgi_cmdmap_t cmdGetFileTbl[] =
{
    {CGI_CMD_FILE_IMG,               cgi_file_img_get},
    {CGI_CMD_FILE_CFG,               cgi_file_cfg_get},
    {CGI_CMD_FILE_BACKUP_STATUS,     cgi_file_backupStatus_get},
    {CGI_CMD_FILE_UPGRADE_STATUS,    cgi_file_upgradeStatus_get},
#ifdef CONFIG_SYS_DUAL_IMAGE
    {CGI_CMD_FILE_DUAL_IMAGE,        cgi_file_dualImage_get},
#endif
    {CGI_CMD_FILE_CFG_SAVE,          cgi_file_cfgSave_get},
    {CGI_CMD_FILE_CFG_SAVE_STATUS,   cgi_file_cfgSaveStatus_get},
    {NULL, NULL}
};

static int32 cgi_file_img_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *imgs = NULL;
    uint32 i = 0;
    uint32 size;
    uint32 tmp_time;
    uint32 magic;
    char name[SYS_BUF32_LEN + 1];

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_FILE_UPBAK_HOST_MAXLEN, CAPA_HOSTNAME_LEN);
    cJSON_AddNumberToObject(data, CGI_FILE_UPBAK_FILE_MAXLEN, CAPA_FILE_NAME_LEN);

    imgs = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_FILE_IMAGE_NAME, imgs);

    for (i = 0; i <= 1; i++)
    {
        sal_sys_fwImageHdrInfo_get(i, &magic, &tmp_time, &size, name);

        cJSON_AddItemToArray(imgs, cJSON_CreateString(name));
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_file_cfg_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_FILE_UPBAK_HOST_MAXLEN, CAPA_HOSTNAME_LEN);
    cJSON_AddNumberToObject(data, CGI_FILE_UPBAK_FILE_MAXLEN, CAPA_FILE_NAME_LEN);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    cJSON_AddBoolToObject(data, CGI_FILE_BAKCFG_SUPPORT, TRUE);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_file_backupStatus_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    FILE *fp = NULL;
    char buf[SYS_BUF256_LEN];
    int32 bytes = 0;

    CGI_GET_JSON_CREATE(root, data);

    if((fp = fopen("/tmp/tftp_backupfail", "r")) != NULL)
    {
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);
        fclose(fp);
        unlink("/tmp/tftp_backupfail");

        cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_FAIL);
        cJSON_AddStringToObject(data, CGI_FILE_STATUS_MESSAGE, buf);
    }
    else if ((fp = fopen("/tmp/tftp_backupDone", "r")) != NULL)
    {
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);
        fclose(fp);
        unlink("/tmp/tftp_backupDone");

        bytes = SYS_STR2UINT(buf);

        cJSON_AddNumberToObject(data, CGI_FILE_BYTES, bytes);
        cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_SUCCESS);
    }
    else
    {
        cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_DOWNLOADING);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_file_upgradeStatus_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    FILE *fp = NULL;
    char *value = NULL;
    char type[SYS_BUF8_LEN] = "http";
    char fileType[SYS_BUF16_LEN];
    char buf[SYS_BUF256_LEN];
    int32 ret = SYS_ERR_OK;

    SYS_MEM_CLEAR(type);
    SYS_MEM_CLEAR(fileType);

    if (CGI_IS_PARAM_EXIST(CGI_FILE_IMG_UP_TYPE, value))
        strlcpy(type, value, sizeof(type));

    if (CGI_IS_PARAM_EXIST(CGI_FILE_TYPE, value))
        strlcpy(fileType, value, sizeof(fileType));

    CGI_GET_JSON_CREATE(root, data);

    if (0 == osal_strcmp(CGI_FILE_TYPE_IMAGE, fileType))
    {
        if (0 == osal_strcmp(type, "http"))
        {
            if((fp = fopen("/tmp/http_uploadfail", "r")) != NULL)
            {
                SYS_MEM_CLEAR(buf);
                fgets(buf, sizeof(buf), fp);
                fclose(fp);
                unlink("/tmp/http_uploadfail");
                unlink("/tmp/http_uploadsize");

                cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_FAIL);
                cJSON_AddStringToObject(data, CGI_FILE_STATUS_MESSAGE, buf);
            }
            else if ((fp = fopen(TMP_FWIMG_WRITESTATUS, "r")) != NULL)
            {
                fclose(fp);
                unlink(TMP_FWIMG_WRITESTATUS);
                unlink("/tmp/http_uploadsize");

                cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_SUCCESS);
            }
            else
            {
                cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_UPLOADING);
            }
        }
        else if (0 == osal_strcmp(type, "tftp"))
        {
            if((fp = fopen("/tmp/tftp_uploadfail", "r")) != NULL)
            {
                SYS_MEM_CLEAR(buf);
                fgets(buf, sizeof(buf), fp);
                fclose(fp);
                unlink("/tmp/tftp_uploadfail");
                unlink("/tmp/tftp_uploadsize");

                cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_FAIL);
                cJSON_AddStringToObject(data, CGI_FILE_STATUS_MESSAGE, buf);
            }
            else if ((fp = fopen(TMP_FWIMG_WRITESTATUS, "r")) != NULL)
            {
                fclose(fp);
                unlink(TMP_FWIMG_WRITESTATUS);
                unlink("/tmp/tftp_uploadsize");

                cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_SUCCESS);
            }
            else
            {
                cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_UPLOADING);
            }
        }
    }
    else
    {
        if((fp = fopen(CONF_HTTP_SAVERUNNING_STATUS, "r")) != NULL)
        {
            fscanf(fp, "%d", &ret);
            fclose(fp);

            if (SYS_ERR_OK != ret)
            {
                cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_FAIL);
                cJSON_AddStringToObject(data, CGI_FILE_STATUS_MESSAGE, SYS_ERR_STR(ret));
                unlink(CONF_HTTP_SAVERUNNING_STATUS);
                goto cgi_end;
            }
            else
            {
                cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_SUCCESS);
                unlink(CONF_HTTP_SAVERUNNING_STATUS);
            }
        }
        else
        {
            cJSON_AddStringToObject(data, CGI_FILE_STATUS, CGI_FILE_STATUS_UPLOADING);
        }
    }

cgi_end:
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_DUAL_IMAGE
static int32 cgi_file_dualImage_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    int32 ret;
    uint32 currActPart = 0;
    uint32 currSelPart = 0;
    uint32 i;
    uint32 size;
    uint32 tmp_time;
    uint32 magic;
    char name[SYS_BUF32_LEN + 1];
    sys_filenamestr_t imgname;
    time_t time;
    struct tm *tm_time;
    char timeStr[SYS_BUF32_LEN + 1];

    SYS_MEM_CLEAR(name);

    sal_sys_currActiveImageIdx_get(&currActPart);
    sal_sys_dualImageCurrSel_get(&currSelPart);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_FILE_DAULIMG_ACTIDX, currActPart);
    cJSON_AddNumberToObject(data, CGI_FILE_DUALIMG_SELIDX, currSelPart);

    for (i = 0; i <= 1; i++)
    {
        ret = sal_sys_fwImageHdrInfo_get(i, &magic, &tmp_time, &size, name);

        if (SYS_ERR_OK != ret)
            continue;

        SYS_MEM_CLEAR(timeStr);
        time = (time_t) ntohl(tmp_time);
        tm_time = gmtime(&time);
        osal_sprintf(timeStr, "%4d-%02d-%02d %02d:%02d:%02d\n",
               (1900 + tm_time->tm_year),
               (1 + tm_time->tm_mon),
               tm_time->tm_mday,
               tm_time->tm_hour,
               tm_time->tm_min,
               tm_time->tm_sec);

        SYS_MEM_CLEAR(imgname);
        sal_sys_dualImageFileName_get(i, &imgname);

        if (i == currSelPart)
        {
            cJSON_AddStringToObject(data, CGI_FILE_DUALIMG_ACTVER, name);
            cJSON_AddStringToObject(data, CGI_FILE_DUALIMG_ACTNAME, imgname.str);
            cJSON_AddNumberToObject(data, CGI_FILE_DUALIMG_ACTSIZE, size);
            cJSON_AddStringToObject(data, CGI_FILE_DUALIMG_ACTTIME, timeStr);
        }
        else
        {
            cJSON_AddStringToObject(data, CGI_FILE_DUALIMG_BAKVER, name);
            cJSON_AddStringToObject(data, CGI_FILE_DUALIMG_BAKNAME, imgname.str);
            cJSON_AddNumberToObject(data, CGI_FILE_DUALIMG_BAKSIZE, size);
            cJSON_AddStringToObject(data, CGI_FILE_DUALIMG_BAKTIME, timeStr);
        }
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_file_cfgSave_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    cJSON_AddBoolToObject(data, CGI_FILE_BAKCFG_SUPPORT, TRUE);
#else
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_file_cfgSaveStatus_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;
    char statusFile[SYS_BUF64_LEN];
    char firstLine[SYS_BUF64_LEN];
    char secondLine[SYS_BUF64_LEN];
    FILE *fp = NULL;

    SYS_MEM_CLEAR(statusFile);
    SYS_MEM_CLEAR(firstLine);
    SYS_MEM_CLEAR(secondLine);

    if (CGI_IS_PARAM_EXIST(CGI_FILE_CFG_STATUS_FILE, value))
    {
        strlcpy(statusFile, "/tmp/", sizeof(statusFile));
        strlcat(statusFile, value, sizeof(statusFile));
    }

    if (NULL != (fp = fopen(statusFile, "r")))
    {
        fgets(firstLine, sizeof(firstLine), fp);
        fgets(secondLine, sizeof(secondLine), fp);
        firstLine[strlen(firstLine) - 1] = '\0';
        secondLine[strlen(secondLine) - 1] = '\0';

        fclose(fp);
    }

    if (0 == osal_strcmp(firstLine, "uploading"))
    {
        if (NULL != (fp = fopen(CONF_HTTP_SAVERUNNING_STATUS, "r")))
        {
            fclose(fp);
            unlink(CONF_HTTP_SAVERUNNING_STATUS);
            strlcpy(firstLine, "success", sizeof(firstLine));
        }
    }

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddStringToObject(data, CGI_FILE_STATUS, firstLine);

    if (0 == osal_strcmp(firstLine, "success"))
    {
        cJSON_AddStringToObject(data, CGI_FILE_BYTES, secondLine);
        unlink(statusFile);
    }
    else if (0 == osal_strcmp(firstLine, "fail"))
    {
        cJSON_AddStringToObject(data, CGI_FILE_STATUS_MESSAGE, secondLine);
        unlink(statusFile);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

