
#ifndef __WEBAUTH_H__
#define __WEBAUTH_H__

#include <common/sys_error.h>
#include <webui/webauth/webauth_util.h>

#define WEB_CORE_MODCMD_TOTAL 256

extern char textBuf[SYS_BUF128_LEN];

#define PRINT_GOTO_PAGECMD(cmd)                         \
do{                                                     \
    WEB_PRINTF("<html><head>");                         \
    WEB_PRINTF("<script language=\"JavaScript\">");     \
    WEB_PRINTF("top.location.replace(\"/cgi/webauth.cgi?cmd=%d\")\n", cmd);\
    WEB_PRINTF("</script>\n");                          \
    WEB_PRINTF("</head><body></body></html>");          \
    return SYS_ERR_OK;                                  \
}while(0)

#define PRINT_GOTO_PAGE(file)                           \
do{                                                     \
    WEB_PRINTF("<html><head>\n");                       \
    WEB_PRINTF("<script language=\"JavaScript\">\n");   \
    WEB_PRINTF("top.location.replace(\"/%s\")\n", file);\
    WEB_PRINTF("</script>\n");                          \
    WEB_PRINTF("</head><body></body></html>");          \
    return SYS_ERR_OK;                                  \
}while(0)

#endif

