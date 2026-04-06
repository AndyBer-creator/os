
#ifndef _WEB_UTIL_H_
#define _WEB_UTIL_H_

#include <common/sys_log.h>
#include <libsal/sal_type.h>
#include <libsal/sal_util.h>
#include "cgi_session.h"

#define HTML_SPACE1   "&nbsp;"
#define HTML_SPACE2   "&nbsp;&nbsp;"
#define HTML_SPACE3   "&nbsp;&nbsp;&nbsp;"
#define HTML_SPACE4   "&nbsp;&nbsp;&nbsp;&nbsp;"
#define HTML_TAB      HTML_SPACE4

#define HTTP_PARAM_FIELD_LEN_MAX 192     
#define HTTP_PARAM_LEN_MAX 56320         
#define HTTP_PARAM_NUM_MAX 8200          
#define HTTP_PARAM_PORTURL_NUM_MAX 10

#define WEB_NOT_EXIST 0
#define WEB_EXIST 1

#define WEB_TMP_DIR        "/tmp"
#define WEB_TMP_REAL_DIR   "/webtmp"
#define PRINT_FILE_PATH    "/home/web/"

typedef struct cgi_paramPair_s
{
    char *name;
    char *val;
}cgi_paramPair_t;

typedef struct cgi_requestParameter_s
{
    cgi_paramPair_t pair[HTTP_PARAM_NUM_MAX];
    int num;
}cgi_requestParameter_t;

typedef struct cgi_postUrlParam_s
{
    cgi_paramPair_t pair[HTTP_PARAM_PORTURL_NUM_MAX];
    int num;
}cgi_postUrlParam_t;

#define CGI_IS_PARAM_IS_NULL(param, value) \
    (cgi_util_inputVal_chkIsNull(value = cgi_util_paramVal_ret(param)))

#define CGI_IS_PARAM_EXIST(param, value) \
    (cgi_util_inputVal_chkExist(value = cgi_util_paramVal_ret(param)))

#define CGI_IS_VALUE_EQUAL(value1, value2) \
    (0 == strcmp(value1, value2))

#define CGI_IS_POSTURL_PARAM_EXIST(param, value) \
    (cgi_util_inputVal_chkExist(value = cgi_util_postUrlParamVal_ret(param)))

#define CGI_IS_PARAM_NOT_EXIST(param, value) \
    !(cgi_util_inputVal_chkExist(value = cgi_util_paramVal_ret(param)))

#define CGI_GET_PARAM_ARRAY_DATA(param, value, num) \
    (NULL != (value = cgi_util_paramValArray_ret(param, num)))

#define CGI_GET_LPORTMSK_PARAM_ARRAY(param, lPortmask) \
do { \
    uint32 _i = 0; \
    uint32 _len = 0; \
    char **_array = NULL;\
    LOGIC_PORTMASK_CLEAR_ALL(lPortmask); \
    if (CGI_GET_PARAM_ARRAY_DATA(param, _array, &_len)) \
    { \
        sys_logic_portmask_t _tmpPortmask; \
        for (_i = 0; _i < _len; _i++) \
        { \
            LOGIC_PORTMASK_CLEAR_ALL(_tmpPortmask); \
            STR2LPM(_array[_i], &_tmpPortmask); \
            LOGIC_PORTMASK_OR(lPortmask, lPortmask, _tmpPortmask); \
        } \
        free(_array); \
    } \
} while (0)

#define CGI_GET_VLANMSK_PARAM_ARRAY(param, vlanmask) \
do { \
    uint32 _i = 0; \
    uint32 _len = 0; \
    char **_array = NULL;\
    LOGIC_PORTMASK_CLEAR_ALL(vlanmask); \
    if (CGI_GET_PARAM_ARRAY_DATA(param, _array, &_len)) \
    { \
        sys_vid_t _tmpVid; \
        for (_i = 0; _i < _len; _i++) \
        { \
            _tmpVid = SYS_STR2UINT(_array[_i]); \
            VLANMASK_SET_VID(vlanmask, _tmpVid); \
        } \
        free(_array); \
    } \
} while (0)

#define CGI_PRINTF(fmt, args...) printf(fmt"\n", ##args)
#define CGI_RESPONSE(fmt, args...) printf("\n\n"fmt, ##args)
#define CGI_RESPONSE_JSON(jsonObj) \
do{                                \
    char *_str = NULL;             \
    _str = cJSON_Print(jsonObj);   \
    if (NULL != _str)              \
    {                              \
        CGI_RESPONSE("%s", _str);  \
        free(_str);                \
    }                              \
}while(0)

#define CGI_PARSE_ENABLE(cgival, enbl)          \
do{                                             \
    if (0 == osal_strcmp(cgival, CGI_ENABLE))   \
        enbl = ENABLED;                         \
    else                                        \
        enbl = DISABLED;                        \
}while(0)

#define CGI_DBG_TARGET_FILE "/tmp/CGI_DEBUG"
#if 1
#define CGI_DBG(a,args...) \
do { \
    char tmp_str[256] = ""; \
    char cmd_str[300] = ""; \
    sprintf(tmp_str, "[CGI_DBG] %s:%d:"#a"", __FUNCTION__,__LINE__, ##args); \
    sprintf(cmd_str, "echo \"%s\" >> %s", tmp_str, CGI_DBG_TARGET_FILE); \
    system(cmd_str); \
} while (0)
#else
#define CGI_DBG(a,args...) {}
#endif

#define CGI_STR2LPM(str, pLPMsk) (cgi_util_webPortlistStr2lPortmask(str, pLPMsk, FALSE))
#define CGI_LAGSTR2LPM(str, pLPMsk) (cgi_util_webPortlistStr2lPortmask(str, pLPMsk, TRUE))

int32 cgi_util_inputVal_chkIsNull(char *val);
int32 cgi_util_inputVal_chkExist(char *val);
int32 cgi_util_paraPairs_init(int isPostUrlParam);
char* cgi_util_paramVal_ret(char *param);
char* cgi_util_postUrlParamVal_ret(char *param);
void* cgi_util_paramValArray_ret(char *param, uint32 *pItemNum);
int32 cgi_util_webPortlistStr2lPortmask(char *str, sys_logic_portmask_t *pLpm, uint32 isLag);
#endif

