
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common/sys_error.h>
#include "cgi_util.h"

cgi_requestParameter_t reqParameters;
cgi_postUrlParam_t postUrlParams;

static int32 _urlDecode(char *dst, char *src)
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

static int32 _readHttpParam(char *buf, int isPostUrlParam)
{
    char    *qryStr;

    if (FALSE == isPostUrlParam)
    {
        if (!osal_strcmp("POST", getenv("REQUEST_METHOD")))
        {
            fgets(buf, HTTP_PARAM_LEN_MAX, stdin);
        }
        else
        {
            qryStr = getenv("QUERY_STRING");
            strlcpy(buf, qryStr, HTTP_PARAM_LEN_MAX);
        }
    }
    else
    {
        qryStr = getenv("QUERY_STRING");
        strlcpy(buf, qryStr, HTTP_PARAM_LEN_MAX);
    }
    return SYS_ERR_OK;
}

int32 cgi_util_paraPairs_init(int isPostUrlParam)
{
    char *pCh, *pre, *e, strLen;
    char tmpBuf[HTTP_PARAM_LEN_MAX];
    char paramBuf[HTTP_PARAM_LEN_MAX];
    cgi_paramPair_t *paramPair = NULL;
    int tmplen = 0;

    osal_memset(paramBuf, 0, sizeof(paramBuf));

    _readHttpParam(paramBuf, isPostUrlParam);
    pCh = osal_strchr(paramBuf, '&');
    pre = paramBuf;
    while (pCh != NULL)
    {
        if (FALSE == isPostUrlParam)
            paramPair = &reqParameters.pair[reqParameters.num];
        else
            paramPair = &postUrlParams.pair[postUrlParams.num];

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
        if (FALSE == isPostUrlParam)
            reqParameters.num++;
        else
            postUrlParams.num++;

        pre = pCh + 1;

        pCh = osal_strchr(pCh + 1, '&');
    }

    strLen = osal_strlen(paramBuf);
    if (strLen > (pCh - paramBuf))
    {
        if (FALSE == isPostUrlParam)
            paramPair = &reqParameters.pair[reqParameters.num];
        else
            paramPair = &postUrlParams.pair[postUrlParams.num];

        e = osal_strchr(pre, '=');
        if (e)
        {
            paramPair->name = osal_alloc(e - pre + 1);
            osal_memset(paramPair->name, 0, (e - pre + 1));
            osal_memcpy(paramPair->name, pre, (e - pre));

            tmplen = (pre + osal_strlen(pre)) - e;
            paramPair->val = osal_alloc(tmplen);
            osal_memset(paramPair->val, 0, tmplen);
            osal_memcpy(paramPair->val, e + 1, tmplen - 1);
        }
        if (FALSE == isPostUrlParam)
            reqParameters.num++;
        else
            postUrlParams.num++;
    }

    return SYS_ERR_OK;
}

int32 cgi_util_inputVal_chkIsNull(char *val)
{
    if (*(val) == '\0')
        return WEB_EXIST;
    else
        return WEB_NOT_EXIST;
}

int32 cgi_util_inputVal_chkExist(char *val)
{
    if ((val) != NULL && *(val) != '\0')
        return WEB_EXIST;
    else
        return WEB_NOT_EXIST;
}

char* cgi_util_paramVal_ret(char *param)
{
    int i;

    for (i = 0; i < reqParameters.num; i++)
    {
        //CGI_DBG("name=%s value=%s\n", reqParameters.pair[i].name, reqParameters.pair[i].val);
        if (!osal_strcmp(reqParameters.pair[i].name, param))
        {
            //CGI_DBG("name=%s value=%s\n", reqParameters.pair[i].name, reqParameters.pair[i].val);
            return reqParameters.pair[i].val;
        }
    }

    return NULL;
}

char* cgi_util_postUrlParamVal_ret(char *param)
{
    int i;

    for (i = 0; i < postUrlParams.num; i++)
        if (!osal_strcmp(postUrlParams.pair[i].name, param))
            return postUrlParams.pair[i].val;

    return NULL;
}

void* cgi_util_paramValArray_ret(char *param, uint32 *pItemNum)
{
    int i, num = 0, cnt = 0;
    char **pValAry;

    for (i = 0; i < reqParameters.num; i++)
        if (!osal_strcmp(reqParameters.pair[i].name, param))
            num++;

    *pItemNum = num;
    pValAry = malloc(sizeof(pValAry) * num);
    for (i = 0; i < reqParameters.num; i++)
    {
        if (!osal_strcmp(reqParameters.pair[i].name, param))
        {
            pValAry[cnt] = reqParameters.pair[i].val;
            cnt++;
        }
    }

    if (num)
        return pValAry;

    if (NULL != pValAry)
        osal_free(pValAry);

    return NULL;
}

int32 cgi_util_webPortlistStr2lPortmask(char *str, sys_logic_portmask_t *pLpm, uint32 isLag)
{
    if (sal_util_validStringCheck_ret(str, "1234567890,-"))
    {
        char buf1[CAPA_PORTMASK_STR_LEN];
        char buf2[CAPA_PORTMASK_STR_LEN];
        char *ptr1 = NULL;
        char *ptr2 = NULL;
        char strBuf[CAPA_PORTMASK_STR_LEN];
        int32 len1 = 0;
        int32 len2 = 0;
        uint32 i = 0;
        uint32 j = 0;
        sys_logic_port_t port;
        sys_logic_port_t portEnd;
        sys_logic_port_t portStart;
        sys_logic_port_t portTmp;
        sys_logic_portmask_t portmaskTmp;

        SYS_PARAM_CHK((NULL == pLpm), SYS_ERR_NULL_POINTER);

        osal_memset(strBuf, 0, sizeof(strBuf));
        osal_memset(&portmaskTmp, 0, sizeof(portmaskTmp));
        osal_memset(pLpm, 0, sizeof(sys_logic_portmask_t));

        if (0 == osal_strcmp(str, "all"))
        {
            LOGIC_PORTMASK_SET_ALL(*pLpm);

            return SYS_ERR_OK;
        }

        i = 0;
        for (j = 0; j < osal_strlen(str); j++)
        {
            if (' ' == str[j])
                continue;

            strBuf[i] = str[j];
            i++;
        }

        ptr1 = strBuf;

        while (0 != osal_strlen(ptr1))
        {
            osal_memset(buf1, 0, sizeof(buf1));
            osal_memset(buf2, 0, sizeof(buf2));

            SYS_ERR_CHK(sys_util_getnext(ptr1, ',', buf1, &len1));
            ptr2 = buf1;

            if ((NULL != osal_strchr(ptr2, '-')))
            {
                SYS_ERR_CHK(sys_util_getnext(ptr2, '-', buf2, &len2));

                if (isLag)
                    portStart = TRK2LP(UID2TRK(SYS_STR2UINT(buf2)));
                else
                    portStart = SYS_STR2UINT(buf2) - 1;

                if (IS_LOGIC_PORT_INVALID(portStart))
                    return SYS_ERR_PORT_RANGE;

                ptr2 += len2;
                SYS_ERR_CHK(sys_util_getnext(ptr2, '-', buf2, &len2));

                if (isLag)
                    portEnd = TRK2LP(UID2TRK(SYS_STR2UINT(buf2)));
                else
                    portEnd = SYS_STR2UINT(buf2) - 1;

                if (IS_LOGIC_PORT_INVALID(portEnd))
                    return SYS_ERR_PORT_RANGE;

                if (portEnd < portStart)
                {
                    
                    portTmp = portStart;
                    portStart = portEnd;
                    portEnd = portTmp;
                }

                for (port = portStart; port <= portEnd; port++)
                {
                    LOGIC_PORTMASK_SET_PORT(portmaskTmp, port);
                }
            }
            
            else
            {
                if (isLag)
                    port = TRK2LP(UID2TRK(SYS_STR2UINT(buf1)));
                else
                    port = SYS_STR2UINT(buf1) - 1;

                if (IS_LOGIC_PORT_INVALID(port))
                    return SYS_ERR_PORT_RANGE;

                LOGIC_PORTMASK_SET_PORT(portmaskTmp, port);
            }

            ptr1 += len1;
        }

        *pLpm = portmaskTmp;
    }
    else
    {
        STR2LPM(str, pLpm);
    }

    return SYS_ERR_OK;
}

