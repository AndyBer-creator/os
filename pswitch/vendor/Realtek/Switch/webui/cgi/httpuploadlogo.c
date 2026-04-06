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

    if ((fp = fopen(CONF_HTTP_LOGO_TMPFILE, "w")) == NULL)
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
        system("killall -q -SIGUSR2 polld");
        ret = SYS_ERR_OK;
    }
    else
    {
        ret = SYS_ERR_CFG_INVALID;
        if (NULL != (fp = fopen(CONF_HTTP_LOGO_TMPFILE_STATUS, "w")))
        {
            fprintf(fp, "%d", ret);
            fclose(fp);
        }
    }

    CGI_RESPONSE("");
    CGI_PRINTF("<html><head>");
    CGI_PRINTF("<script src=\"../js/fileload.js\" type=\"text/javascript\"></script>");
    CGI_PRINTF("<script type=\"text/javascript\" language=\"javascript\">");
    CGI_PRINTF("fileLoad(\"../js/styling.js\", \"js\");");
    CGI_PRINTF("fileLoad(\"../js/utility.js\", \"js\");");
    CGI_PRINTF("fileLoad(\"../lang/defLoginLang.js\", \"js\");");
    CGI_PRINTF("function init(){fileLoadWait(); setTimeout(function(){ window.location.href='../orig-equip-manufact-info-set.html';}, 2000);} ");
    CGI_PRINTF("</script>");
    CGI_PRINTF("</head>");
    CGI_PRINTF("<body onload=\"init();\">");
    if(ret == SYS_ERR_OK)
    {
    	CGI_PRINTF("</body>Device logo update OK, It will redirect in 2 seconds!</html>");
    }
    else
    {
		CGI_PRINTF("</body>Device logo update Failed, It will redirect in 2 seconds!</html>");
    }

    return 0;
}

