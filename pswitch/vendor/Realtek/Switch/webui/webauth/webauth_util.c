
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common/sys_error.h>
#include <webui/webauth/webauth_util.h>

cgi_requestParameter_t reqParameters;

int32 _urlDecode(char *dst, char *src)
{
    int code;

    while (*src != 0)
    {
        if (*src == '+')
            *dst = ' ';
        else if (*src == '%')
        {
            if (osal_sscanf(++src, "%2x", &code) != 1) {
                code = '?';
            }
            *dst = code;
            src++;
        }
        else
            *dst = *src;

        src++;
        dst++;
    }
    return 0;
}

static int32 _readHttpParam(char *buf)
{
    char    *qryStr = NULL;

    if (!osal_strcmp("POST", getenv("REQUEST_METHOD")))
    {
        fgets(buf, HTTP_PARAM_LEN_MAX, stdin);
    }
    else
    {
        qryStr = getenv("QUERY_STRING");

        if (NULL != qryStr && 0 != strlen(qryStr))
            strlcpy(buf, qryStr, HTTP_PARAM_LEN_MAX);
    }
    return SYS_ERR_OK;
}

int32 web_util_inputVal_chkIsNull(char *val)
{
    if (*(val) == '\0')
        return WEB_EXIST;
    else
        return WEB_NOT_EXIST;
}

int32 web_util_inputVal_chkExist(char *val)
{
    if ((val) != NULL && *(val) != '\0')
        return WEB_EXIST;
    else
        return WEB_NOT_EXIST;
}

int32 web_util_initParaPairs(void)
{
    char *pCh, *pre, *e, strLen;
    char tmpBuf[HTTP_PARAM_LEN_MAX];
    char paramBuf[HTTP_PARAM_LEN_MAX];
    cgi_paramPair_t *paramPair = NULL;
    int tmplen = 0;

    osal_memset(paramBuf, 0, sizeof(paramBuf));

    _readHttpParam(paramBuf);

    if (0 == strlen(paramBuf))
        return SYS_ERR_OK;

    pCh = osal_strchr(paramBuf, '&');
    pre = paramBuf;
    while (pCh != NULL)
    {
        paramPair = &reqParameters.pair[reqParameters.num];

        e = osal_strchr(pre, '=');
        if (e)
        {
            paramPair->name = osal_alloc(e - pre + 1);
            osal_memset(paramPair->name, 0, (e - pre + 1));
            osal_memcpy(paramPair->name, pre, (e - pre));

            paramPair->val = osal_alloc(pCh - e);
            osal_memset(paramPair->val, 0, (pCh - e));
            osal_memset(tmpBuf, 0, sizeof(tmpBuf));
            osal_memcpy(tmpBuf, e + 1, (pCh - e - 1));
            _urlDecode(paramPair->val, tmpBuf);

        }
        reqParameters.num++;

        pre = pCh + 1;

        pCh = osal_strchr(pCh + 1, '&');
    }

    strLen = osal_strlen(paramBuf);
    if (strLen > (pCh - paramBuf))
    {
        paramPair = &reqParameters.pair[reqParameters.num];

        e = osal_strchr(pre, '=');
        if (e)
        {
            paramPair->name = osal_alloc(e - pre + 1);
            osal_memset(paramPair->name, 0, (e - pre + 1));
            osal_memcpy(paramPair->name, pre, (e - pre));

            tmplen = (pre + osal_strlen(pre)) - e;
            paramPair->val = osal_alloc(tmplen);
            osal_memset(paramPair->val, 0, tmplen);

            osal_memset(tmpBuf, 0, sizeof(tmpBuf));
            osal_memcpy(tmpBuf, e + 1, tmplen - 1);

            _urlDecode(paramPair->val, tmpBuf);
        }
        reqParameters.num++;
    }

    return SYS_ERR_OK;
}

char* web_util_paramVal_ret(char *param)
{
    int i;

    for (i = 0; i < reqParameters.num; i++)
        if (!osal_strcmp(reqParameters.pair[i].name, param))
        {
            return reqParameters.pair[i].val;
        }

    return NULL;
}

void cgi_util_authingSessionName_get(uint32 remotePort, char *pName)
{
    authmgr_suppInfo_t supp;

    SYS_MEM_CLEAR(supp);

    sal_authmgr_webauthSuppInfoByKey_get(remotePort, &supp);

    osal_sprintf(pName, "/tmp/webauthSess_%02X%02X%02X%02X%02X%02X",
                        supp.mac.octet[0],
                        supp.mac.octet[1],
                        supp.mac.octet[2],
                        supp.mac.octet[3],
                        supp.mac.octet[4],
                        supp.mac.octet[5]);
}

void cgi_util_authingSession_create(cgi_webauthSess_t *pSession)
{
    FILE *fp = NULL;
    char file[SYS_BUF64_LEN];
    char file2[SYS_BUF64_LEN];
    char macStr[CAPA_MAC_STR_LEN];

    SYS_MEM_CLEAR(file);
    SYS_MEM_CLEAR(file2);
    SYS_MEM_CLEAR(macStr);

    osal_sprintf(file, "/tmp/webauthSess_info_%d", getpid());
    cgi_util_authingSessionName_get(pSession->remotePort, file2);

    if (NULL != (fp = fopen(file, "w")))
    {
        fprintf(fp, "%s\n", pSession->username);
        fprintf(fp, "%s\n", pSession->password);
        fprintf(fp, "%d\n", pSession->remotePort);
        fprintf(fp, "%s\n", file2);
        fclose(fp);
    }

    return;
}

void cgi_util_authingSession_destroy(uint32 remotePort)
{
    char file[SYS_BUF64_LEN];

    SYS_MEM_CLEAR(file);

    cgi_util_authingSessionName_get(remotePort, file);

    unlink(file);

    return;
}

void cgi_util_authingSessionStatus_get(uint32 remotePort, cgi_session_authingStatus_t *pStatus)
{
    FILE *fp = NULL;
    char file[SYS_BUF64_LEN];

    SYS_MEM_CLEAR(file);

    cgi_util_authingSessionName_get(remotePort, file);

    *pStatus = CGI_SESSION_AUTHING_NOT_EXIST;

    if (NULL != (fp = fopen(file, "r")))
    {
        fscanf(fp, "~webauth~%d", (int32 *)pStatus);

        fclose(fp);
    }

    return;
}

