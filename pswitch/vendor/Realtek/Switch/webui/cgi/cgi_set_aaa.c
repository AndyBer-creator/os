
#include <include/cgi.h>

static int32 cgi_aaa_loginAdd_set(void);
static int32 cgi_aaa_loginEdit_set(void);
static int32 cgi_aaa_loginDel_set(void);
static int32 cgi_aaa_loginAuth_set(void);

const cgi_cmdmap_t cmdSetAaaTbl[] =
{
    {CGI_CMD_AAA_LOGINADD,     cgi_aaa_loginAdd_set},
    {CGI_CMD_AAA_LOGINEDIT,    cgi_aaa_loginEdit_set},
    {CGI_CMD_AAA_LOGINDEL,     cgi_aaa_loginDel_set},
    {CGI_CMD_AAA_LOGINAUTH,    cgi_aaa_loginAuth_set},
    {NULL, NULL}
};

static int32 cgi_aaa_loginAdd_set(void)
{
    int32           ret = SYS_ERR_OK;
    char            *value;
    char            listName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    char            buf[SYS_BUF8_LEN];
    uint32          i = 0;
    sys_auth_type_t authTypes[CAPA_AAA_AUTHTYPE_PRI_NUM];

    SYS_MEM_CLEAR(listName);
    SYS_MEM_CLEAR(authTypes);

    if (CGI_IS_PARAM_EXIST(CGI_AAA_LIST, value))
        strlcpy(listName, value, sizeof(listName));

    if (osal_strlen(listName) > CAPA_AAA_AUTHLIST_NAME_LEN)
    {
        ret =  SYS_ERR_STR_LEN_EXCEED;
        goto cgi_end;
    }

    for (i = 0; i < CAPA_AAA_AUTHTYPE_PRI_NUM; i++)
    {
        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s%d", CGI_AAA_METHOD, i);

        if (CGI_IS_PARAM_EXIST(buf, value))
            authTypes[i] = SYS_STR2UINT(value);
    }

    CGI_SET_ERR_HDL(sal_aaa_authList_set(listName, authTypes, SYS_AUTHLIST_TYPE_AUTHEN_LOGIN), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_aaa_loginEdit_set(void)
{
    int32           ret = SYS_ERR_OK;
    char            *value;
    char            listName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    char            buf[SYS_BUF8_LEN];
    uint32          i = 0;
    sys_auth_type_t authTypes[CAPA_AAA_AUTHTYPE_PRI_NUM];

    SYS_MEM_CLEAR(listName);
    SYS_MEM_CLEAR(authTypes);

    if (CGI_IS_PARAM_EXIST(CGI_AAA_LIST, value))
        strlcpy(listName, value, sizeof(listName));

    if (osal_strlen(listName) > CAPA_AAA_AUTHLIST_NAME_LEN)
    {
        ret =  SYS_ERR_STR_LEN_EXCEED;
        goto cgi_end;
    }

    for (i = 0; i < CAPA_AAA_AUTHTYPE_PRI_NUM; i++)
    {
        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s%d", CGI_AAA_METHOD, i);

        if (CGI_IS_PARAM_EXIST(buf, value))
            authTypes[i] = SYS_STR2UINT(value);
    }

    CGI_SET_ERR_HDL(sal_aaa_authList_set(listName, authTypes, SYS_AUTHLIST_TYPE_AUTHEN_LOGIN), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_aaa_loginDel_set(void)
{
    int32        ret = SYS_ERR_OK;
    uint32       i = 0;
    uint32       len = 0;
    char         **array = NULL;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_AAA_LIST, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            CGI_SET_ENTRY_ERR_CONTINUE(sal_aaa_authList_del(array[i], SYS_AUTHLIST_TYPE_AUTHEN_LOGIN), array[i]);
        }
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_aaa_loginAuth_set(void)
{
    int32           ret = SYS_ERR_OK;
    char            *value;
    char            buf[SYS_BUF8_LEN];
    uint32          i = 0;

    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s_%d", CGI_AAA_LIST, i);

        if (CGI_IS_PARAM_EXIST(buf, value))
            CGI_SET_ERR_HDL(sal_aaa_loginAuthList_set(i, value), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

