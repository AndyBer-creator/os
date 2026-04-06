
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <webui/webauth/webauth.h>

typedef enum webauth_page_e
{
    WEBAUTH_PAGE_LOGIN = 0,
    WEBAUTH_PAGE_AUTHING,
    WEBAUTH_PAGE_BLOCK,
    WEBAUTH_PAGE_SUCCESS,
    WEBAUTH_PAGE_FAIL,
    WEBAUTH_PAGE_END
} webauth_page_t;

typedef enum webauth_cmd_e
{
    WEBAUTH_CMD_LOGINPAGE = 0,
    WEBAUTH_CMD_LOGINAPPLY,
    WEBAUTH_CMD_LOGINSTATUS,
    WEBAUTH_CMD_LOGINSUCCESS,
    WEBAUTH_CMD_LOGINFAIL,
    WEBAUTH_CMD_END
} webauth_cmd_t;

char *reqUri = NULL;

static void _page_print(webauth_page_t type)
{
    WEB_PRINTF("\n\n");
    WEB_PRINTF("\n<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">");
    WEB_PRINTF("<head>");
    WEB_PRINTF("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
    WEB_PRINTF("<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/style.css\">");
    switch (type)
    {
        case WEBAUTH_PAGE_LOGIN:
            WEB_PRINTF("<title>Login Page</title>");
            break;

        case WEBAUTH_PAGE_BLOCK:
            WEB_PRINTF("<title>Login Blocked</title>");
            break;

        case WEBAUTH_PAGE_SUCCESS:
            WEB_PRINTF("<title>Login Success</title>");
            break;

        case WEBAUTH_PAGE_FAIL:
            WEB_PRINTF("<title>Login Fail</title>");
            break;

        default:
            break;
    }
    if (WEBAUTH_PAGE_AUTHING == type)
    {
        WEB_PRINTF("<script type=\"text/javascript\" language=\"javascript\">");
        WEB_PRINTF("function initPage(){");
        WEB_PRINTF("    window.setTimeout(\"pageRefresh();\", 1200);");
        WEB_PRINTF("}");
        WEB_PRINTF("function pageRefresh(){");
        WEB_PRINTF("    window.setTimeout(\"makeRequest('/cgi/webauth.cgi?cmd=%d&dummy='+new Date().getTime());\", 500);", WEBAUTH_CMD_LOGINSTATUS); 
        WEB_PRINTF("}");
        WEB_PRINTF("function makeRequest(url) {");
        WEB_PRINTF("    var http_request = false;");
        WEB_PRINTF("    if (window.XMLHttpRequest) { // Mozilla, Safari, ...");
        WEB_PRINTF("        http_request = new XMLHttpRequest();");
        WEB_PRINTF("        if (http_request.overrideMimeType) {");
        WEB_PRINTF("            http_request.overrideMimeType('text/xml');");
        WEB_PRINTF("        }");
        WEB_PRINTF("    } else if (window.ActiveXObject) { // IE");
        WEB_PRINTF("        try {");
        WEB_PRINTF("            http_request = new ActiveXObject(\"Msxml2.XMLHTTP\");");
        WEB_PRINTF("        } catch (e) {");
        WEB_PRINTF("            try {");
        WEB_PRINTF("                http_request = new ActiveXObject(\"Microsoft.XMLHTTP\");");
        WEB_PRINTF("            } catch (e) {}");
        WEB_PRINTF("        }");
        WEB_PRINTF("    }");
        WEB_PRINTF("    if (!http_request) {");
        WEB_PRINTF("        alert('Giving up :( Cannot create an XMLHTTP instance');");
        WEB_PRINTF("        return false;");
        WEB_PRINTF("    }");
        WEB_PRINTF("    http_request.onreadystatechange = function() { alertContents(http_request, url); };");
        WEB_PRINTF("    http_request.open('GET', url, true);");
        WEB_PRINTF("    http_request.send(null);");
        WEB_PRINTF("}");
        WEB_PRINTF("function alertContents(http_request, url) {");
        WEB_PRINTF("    var tmpStr;");
        WEB_PRINTF("    if (http_request.readyState == 4) {");
        WEB_PRINTF("        if (http_request.status == 200) {");
        WEB_PRINTF("            tmpStr = http_request.responseText;");
        WEB_PRINTF("            if (tmpStr.indexOf(\"~webauth~%d\") != -1) {", CGI_SESSION_AUTHING);
        WEB_PRINTF("                pageRefresh();");
        WEB_PRINTF("                return;");
        WEB_PRINTF("            }");
        WEB_PRINTF("        }");
        WEB_PRINTF("    }");
        WEB_PRINTF("    window.location.replace('%s');", ((NULL == reqUri) ? "/" : reqUri));
        WEB_PRINTF("}");
        WEB_PRINTF("</script>");
    }

    WEB_PRINTF("</head>");
    if (WEBAUTH_PAGE_AUTHING == type)
        WEB_PRINTF("<body onload=\"initPage();\">");
    else
        WEB_PRINTF("<body>");
    WEB_PRINTF("<table width=100%% height=100%%>");
    WEB_PRINTF("<tr>");
    WEB_PRINTF("<td align=center>");

    if (WEBAUTH_PAGE_LOGIN == type || WEBAUTH_PAGE_FAIL == type)
    {
        WEB_PRINTF("  <form method=\"post\" action=\"%s\">", (NULL == reqUri) ? "/" : reqUri);
        WEB_PRINTF("    <fieldset class=\"login\">");
        WEB_PRINTF("      <legend>Authentication</legend>");
        WEB_PRINTF("      <br>");
        if (WEBAUTH_PAGE_FAIL == type)
        {
            WEB_PRINTF("      <div id=\"fail\">");
            WEB_PRINTF("        <b>Login Fail!!!  Please make suer you enter a correct user name and password!!</b><br><br>");
            WEB_PRINTF("      </div>");
        }
        WEB_PRINTF("      <div>");
        WEB_PRINTF("        <label for=\"username\">User Name</label> <input type=\"text\" id=\"username\" name=\"username\">");
        WEB_PRINTF("      </div>");
        WEB_PRINTF("      <div>");
        WEB_PRINTF("        <label for=\"password\">Password</label> <input type=\"password\" id=\"password\" name=\"password\">");
        WEB_PRINTF("      </div>");
        WEB_PRINTF("      <input type=\"hidden\" name=\"cmd\" value=\"1\">");
        WEB_PRINTF("      <br>");
        WEB_PRINTF("    </fieldset>");
        WEB_PRINTF("    <br>");
        WEB_PRINTF("    <div align=right><button class=\"button\" type=\"submit\">&nbsp;&nbsp;Login&nbsp;&nbsp;</button></div>");
        WEB_PRINTF("  </form>");
    }
    else if (WEBAUTH_PAGE_SUCCESS == type || WEBAUTH_PAGE_BLOCK == type)
    {
        WEB_PRINTF("  <fieldset class=\"login\" style=\"margin:1em;width:27em;\">");
        WEB_PRINTF("    <legend>Authentication Status</legend>");
        WEB_PRINTF("    <br>");
        if (WEBAUTH_PAGE_SUCCESS == type)
        {
            WEB_PRINTF("    <div>");
            WEB_PRINTF("    <b>Login Success.</b><br><br>");
            WEB_PRINTF("    </div>");
        }
        else
        {
            WEB_PRINTF("    <div>");
            WEB_PRINTF("    <b>You are blocked because of fail login retry too many times. Please retry after block timeout.</b><br><br>");
            WEB_PRINTF("    </div>");
        }
        WEB_PRINTF("    <br>");
        WEB_PRINTF("  </fieldset>");
    }
    else if (WEBAUTH_PAGE_AUTHING == type)
    {
        WEB_PRINTF("  <fieldset class=\"login\" style=\"margin:1em;width:27em;\">");
        WEB_PRINTF("    <legend>Authentication Status</legend>");
        WEB_PRINTF("    <br>");
        WEB_PRINTF("    <div>");
        WEB_PRINTF("    <b>Authing.</b><br><br>");
        WEB_PRINTF("    </div>");
        WEB_PRINTF("    <br>");
        WEB_PRINTF("  </fieldset>");
    }
    WEB_PRINTF("</td>");
    WEB_PRINTF("</tr>");
    WEB_PRINTF("</table>");
    WEB_PRINTF("</body>");
    WEB_PRINTF("</html>");
}

int32 main(int argc, char* argv[], char* envp[])
{
    char *valStr = NULL;
    uint32 cmd = 0;
    uint32 remotePort = 0;

    char username[SYS_BUF256_LEN + 1];
    char password[SYS_BUF256_LEN + 1];
    authmgr_status_t state;

    web_util_initParaPairs();

    remotePort = SYS_STR2UINT(getenv("REMOTE_PORT"));
    reqUri = getenv("REQUEST_URI");

    sal_authmgr_webauthStatusByKey_get(remotePort, &state);

    if (AUTHMGR_STATUS_LOCKED == state)
    {
        _page_print(WEBAUTH_PAGE_BLOCK);

        return 0;
    }

    if (WEB_IS_FORM_PARAM_EXIST("cmd", valStr))
    {
        cmd = SYS_STR2UINT(valStr);
    }

    if (WEBAUTH_CMD_LOGINPAGE == cmd)
    {
        cgi_session_authingStatus_t status;

        cgi_util_authingSessionStatus_get(remotePort, &status);

        if (CGI_SESSION_AUTHING_FAIL == status)
        {
            _page_print(WEBAUTH_PAGE_FAIL);
            cgi_util_authingSession_destroy(remotePort);
        }
        else if (CGI_SESSION_AUTHING == status)
        {
            _page_print(WEBAUTH_PAGE_AUTHING);
        }
        else
        {
            _page_print(WEBAUTH_PAGE_LOGIN);
        }
    }
    else if (WEBAUTH_CMD_LOGINAPPLY == cmd)
    {
        webauth_authInfo_t authInfo;
        cgi_webauthSess_t webauthSess;
        FILE *fp = NULL;

        SYS_MEM_CLEAR(authInfo);
        SYS_MEM_CLEAR(webauthSess);

        authInfo.tcpPort = remotePort;
        authInfo.authSts = SYS_AUTHING;

        sal_authmgr_webauthStatus_set(&authInfo);

        SYS_MEM_CLEAR(username);
        SYS_MEM_CLEAR(password);

        if (WEB_IS_FORM_PARAM_EXIST("username", valStr))
            osal_strncpy(username, valStr, SYS_BUF256_LEN);

        if (WEB_IS_FORM_PARAM_EXIST("password", valStr))
            osal_strncpy(password, valStr, SYS_BUF256_LEN);

        webauthSess.username = username;
        webauthSess.password = password;
        webauthSess.remotePort = remotePort;

        cgi_util_authingSession_create(&webauthSess);

        _page_print(WEBAUTH_PAGE_AUTHING);

        if (NULL != (fp = fopen(SYS_POLLD_ACT_WEB_LOGIN, "w")))
        {
            fprintf(fp, "/home/webauth/cgi/webauth_login.cgi %d &", getpid());
            fclose(fp);
        }
        
        system("handler -w login 1> /dev/null 2> /dev/null &");
    }
    else if (WEBAUTH_CMD_LOGINSTATUS == cmd)
    {
        cgi_session_authingStatus_t status = CGI_SESSION_AUTHING_SUCCESS;

        cgi_util_authingSessionStatus_get(remotePort, &status);

        WEB_PRINTF("\n\n");
        WEB_PRINTF("~webauth~%d", status);
    }

    return 0;
}

