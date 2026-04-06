#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <include/cgi.h>
#include <libsal/sal_sys_fw.h>
#include <pswitch/image.h>

char textBuf[SYS_BUF128_LEN];

static int32 _clean_useless_files(void)
{
    unlink(TMP_FWIMG_HTTPINFO);

#ifdef CONFIG_SYS_FW_UPGRADE_SAVEMEM
    SYS_ERR_CHK(sal_sys_fwImageSaveMem_gen(TMP_FWIMG_FILENAME));
#else
    unlink(TMP_FWIMG_FILENAME);
#endif

    return SYS_ERR_OK;
}

int main(int argc, char* argv[], char* envp[])
{
    int32 ret;
    char *p = NULL;
    FILE *fp = NULL;
#ifdef CONFIG_SYS_DUAL_IMAGE
    uint32 currSelPart = 0;
#endif

    if (CGI_SESSION_ACTIVE != cgi_util_session_check(FALSE))
    {
        CGI_PRINTF("\n\nNot authorized");
        exit(1);
    }

    p = getenv("UP_STS");
    if ((NULL != p) && (0 == osal_memcmp(p, "block", 5)))
    {
        if (NULL != (fp = fopen("/tmp/http_uploadfail", "w")))
        {

            fprintf(fp, "Another image is already processing.");
            fclose(fp);
        }
        goto end;
    }

#ifdef CONFIG_SYS_FW_UPGRADE_CHECKMEM
    p = getenv("SYS_MEM");
    if ((NULL != p) && (0 == osal_memcmp(p, "insuff", 6)))
    {
        _clean_useless_files();

        if (NULL != (fp = fopen("/tmp/http_uploadfail", "w")))
        {

            fprintf(fp, "System do not have sufficient memory to do firmware image operation.");

            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_MEMORY_LACK);

            fclose(fp);
        }
        goto end;
    }
#endif

    p = NULL;
    p = getenv("IMGHDR_CRC");
    if ((NULL != p) && (osal_memcmp(p, "fail", 4) == 0))
    {
        _clean_useless_files();
        sal_sys_fwImageLock_give();

        if (NULL != (fp = fopen("/tmp/http_uploadfail", "w")))
        {

            fprintf(fp, "Copy: Illegal software format");
            fclose(fp);
        }

        goto end;
    }

    ret = sal_sys_fwImageCrc32_check(TMP_FWIMG_FILENAME);
    if (ret != SYS_ERR_OK)
    {
        _clean_useless_files();
        sal_sys_fwImageLock_give();

        if (NULL != (fp = fopen("/tmp/http_uploadfail", "w")))
        {
            if (SYS_ERR_IMAGE_MAGIC_INVALID == ret)
                fprintf(fp, "Copy: Illegal software format");
            else
                fprintf(fp, "Copy: Illegal software format");

            fclose(fp);
        }
        goto end;
    }

#ifdef CONFIG_SYS_DUAL_IMAGE
    FILE *fp_info = NULL;
    char tmp_buf[512];

    if ((fp_info = fopen(TMP_FWIMG_HTTPINFO, "r")) != NULL)
    {
		while (fgets(tmp_buf, sizeof(tmp_buf), fp_info)!= NULL)
		{
			char *pStart = NULL, *pEnd = NULL, *pTmp = NULL;
			if ((pStart = osal_strstr(tmp_buf, "filename=\"")) != NULL)
			{
				if (NULL == (fp = fopen("/tmp/http_write2filename", "w")))
					continue;

				pStart = pStart + osal_strlen("filename=\"");
				while(NULL != (pTmp = strchr(pStart, '\\')))
				{
					pStart = pTmp + 1;
				}
		
				pEnd = osal_strchr(pStart, '"') - 1;
				fwrite(pStart, 1, pEnd - pStart + 1, fp);
				fclose(fp);
				break;
			}
		}
		fclose(fp_info);
    }
    else 
    {
		if (NULL != (fp = fopen("/tmp/http_write2filename", "w")))
		{
			fprintf(fp, "new_vmlinux.bix");
	        fclose(fp);
		}
    }

    sal_sys_currActiveImageIdx_get(&currSelPart);

    if (NULL != (fp = fopen("/tmp/http_write2partition", "w")))
    {
        fprintf(fp, "%d", !currSelPart);
        fclose(fp);
    }

    unlink(TMP_FWIMG_HTTPINFO);
#else
    if (NULL != (fp = fopen("/tmp/http_write2partition", "w")))
    {
        fprintf(fp, "%d", 0);
        fclose(fp);
    }
#endif 

    if (NULL != (fp = fopen("/tmp/http_uploadsize", "w")))
    {
        fprintf(fp, "%d", sal_util_fileSize_ret(TMP_FWIMG_FILENAME));
        fclose(fp);
    }

    system("killall -q -SIGHUP polld");

end:
    CGI_RESPONSE("");
    CGI_PRINTF("<html><head>");
    CGI_PRINTF("<script src=\"../js/fileload.js\" type=\"text/javascript\"></script>");
    CGI_PRINTF("<script type=\"text/javascript\" language=\"javascript\">");
    CGI_PRINTF("fileLoad(\"../js/styling.js\", \"js\");");
    CGI_PRINTF("fileLoad(\"../js/utility.js\", \"js\");");
    #ifdef CONFIG_SYS_DUAL_IMAGE
    CGI_PRINTF("function init(){fileLoadWait(); openLink('html/mgmt_dualimg_action.html','httpuploadimg=1');}");
    #else
    CGI_PRINTF("function init(){fileLoadWait(); openLink('html/mgmt_img_action.html','httpuploadimg=1');}");
    #endif
    CGI_PRINTF("</script>");
    CGI_PRINTF("</head>");
    CGI_PRINTF("<body onload=\"init();\">");
    CGI_PRINTF("</body></html>");

    return 0;
}

