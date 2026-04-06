#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>

#include <include/cgi.h>
#include <libcmd/cmd.h>
#include <pswitch/image.h>

int main(int argc, char* argv[], char* envp[])
{
    char buf[200];
    FILE *fp;
    int size = 0;
    int ch;
    int ret = SYS_ERR_OK;

    signal(SIGINT, SIG_IGN);
    alarm(0);

    if (CGI_SESSION_ACTIVE != cgi_util_session_check(FALSE))
    {
        CGI_PRINTF("\n\nNot authorized");
        exit(1);
    }

    while (1)
    {
        fgets(buf, sizeof(buf), stdin);

        if (osal_memcmp(buf, "Content-Type", 12) == 0)
        {
            break;
        }
    }
    fgets(buf, sizeof(buf), stdin);

    if ((fp = fopen(CONF_STARTUP_TMPFILE, "w")) == NULL)
    {
        
        exit(1);
    }

    while ((ch = fgetc(stdin)) != EOF)
    {
        if (ch == '\r')
        {
            fread(buf, 1, 7, stdin);
            
            if (osal_memcmp(buf, "\n------", 7) == 0)
            {
                break;
            }
            fputc(ch, fp);
            fwrite(buf, 1, 7, fp);
            size += 8;
            continue;
        }
        fputc(ch, fp);
        size++;
    }

    if (fclose(fp) != 0)
    {
        
        exit(1);
    }

   if(size != 0)
   {
        if (IS_CONFIG_FILEHDR_INVALID(CONF_STARTUP_TMPFILE))
        {
            ret = SYS_ERR_CFG_INVALID;

            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_CFG_INVALID);

            goto end;
        }

        ret = sal_util_file_copy(CONF_STARTUP_TMPFILE, CONF_BACKUP_FILE);

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL,
                        CONF_BACKUP_BASENAME);

            goto end;
        }
        else
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS,
                        CONF_BACKUP_BASENAME);
        }

        cmd_configValidFile_convert(CONF_STARTUP_FILE);

        sal_sys_backupConfig_sync(SYS_CONF_SYNC_STARTUP);
    }
    else
    {
        ret = SYS_ERR_CFG_INVALID;
        goto end;
    }

end:
    if (NULL != (fp = fopen(CONF_HTTP_SAVERUNNING_STATUS, "w")))
    {
        fprintf(fp, "%d", ret);
        fclose(fp);
    }
    CGI_RESPONSE("");
    CGI_PRINTF("<html><head>");
    CGI_PRINTF("<script src=\"../js/fileload.js\" type=\"text/javascript\"></script>");
    CGI_PRINTF("<script type=\"text/javascript\" language=\"javascript\">");
    CGI_PRINTF("fileLoad(\"../js/styling.js\", \"js\");");
    CGI_PRINTF("fileLoad(\"../js/utility.js\", \"js\");");
    CGI_PRINTF("function init(){fileLoadWait(); openLink('html/mgmt_cfg_action.html','httpuploadbakcfg=1');}");
    CGI_PRINTF("</script>");
    CGI_PRINTF("</head>");
    CGI_PRINTF("<body onload=\"init();\">");
    CGI_PRINTF("</body></html>");

    return 0;
}

