
#include <include/cgi.h>

static int32 cgi_aaa_login_get(void);
static int32 cgi_aaa_loginAdd_get(void);
static int32 cgi_aaa_loginEdit_get(void);
static int32 cgi_aaa_loginAuth_get(void);

const cgi_cmdmap_t cmdGetAaaTbl[] =
{
    {CGI_CMD_AAA_LOGIN,        cgi_aaa_login_get},
    {CGI_CMD_AAA_LOGINADD,     cgi_aaa_loginAdd_get},
    {CGI_CMD_AAA_LOGINEDIT,    cgi_aaa_loginEdit_get},
    {CGI_CMD_AAA_LOGINAUTH,    cgi_aaa_loginAuth_get},
    {NULL, NULL}
};

const sys_text_t cgi_text_aaa_authMethod[] =
{
    {       "lang('aaa','lblMethodEmpty')"  },
    {        "lang('aaa','lblMethodNone')"   },
    {       "lang('aaa','lblMethodLocal')"  },
    {        ""                },
    {      "lang('aaa','lblMethodEnable')" },
#ifdef CONFIG_SYS_LIB_RADIUS
    {      "lang('aaa','lblMethodRadius')" },
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
    {     "lang('aaa','lblMethodTacplus')"},
#endif
};

const sys_text_t cgi_text_aaa_line[] =
{
    {    "lang('line','lblConsole')"  },
    {     "lang('line','lblTelnet')"   },
#ifdef CONFIG_USER_SSH_SSHD
    {        "lang('line','lblSsh')"      },
#endif
    {       "lang('line','lblHttp')"     },
    {      "lang('line','lblHttps')"    },
};

static int32 cgi_aaa_login_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;
    cJSON               *lists = NULL;
    cJSON               *list = NULL;
    cJSON               *methods = NULL;
    uint32              i = 0;
    uint32              j = 0;
    uint32              cnt = 0;
    char                authListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    sys_auth_type_t     authTypePrio[CAPA_AAA_AUTHTYPE_PRI_NUM];
    sys_authList_type_t authListType = SYS_AUTHLIST_TYPE_NONE;

    CGI_GET_JSON_CREATE(root, data);

    lists = cJSON_CreateArray();

    for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
    {
        SYS_MEM_CLEAR(authListName);
        SYS_MEM_CLEAR(authTypePrio);

        sal_aaa_authList_getByIdx(i, authListName, authTypePrio, &authListType);

        if (0 == osal_strlen(authListName) || (authListType != SYS_AUTHLIST_TYPE_AUTHEN_LOGIN))
            continue;

        list = cJSON_CreateObject();
        cJSON_AddStringToObject(list, CGI_AAA_LIST, authListName);

        methods = cJSON_CreateArray();

        cnt = 0;
        for(j = 0; j < CAPA_AAA_AUTHTYPE_PRI_NUM; j++)
        {
            if (SYS_AUTH_TYPE_EMPTY == authTypePrio[j])
                break;

            cJSON_AddItemToArray(methods, cJSON_CreateString(cgi_text_aaa_authMethod[authTypePrio[j]].text));

            cnt ++;
        }

        if (cnt > 0)
            cJSON_AddItemToObject(list, CGI_AAA_METHODS, methods);

        cJSON_AddItemToArray(lists, list);
    }

    cJSON_AddItemToObject(data, CGI_AAA_LISTS, lists);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_aaa_loginAdd_get(void)
{
    cJSON  *root = NULL;
    cJSON  *data = NULL;
    cJSON  *methods = NULL;
    cJSON  *method = NULL;
    uint32 i = 0;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_AAA_LIST_MAXLEN, CAPA_AAA_KEY_LEN);

    methods = cJSON_CreateArray();

    for (i = 0; i < SYS_AUTH_TYPE_END; i++)
    {
        if ((SYS_AUTH_TYPE_LINE == i))
            continue;

        method = cJSON_CreateObject();

        cJSON_AddStringToObject(method, CGI_NAME, cgi_text_aaa_authMethod[i].text);
        cJSON_AddNumberToObject(method, CGI_VALUE, i);

        cJSON_AddItemToArray(methods, method);
    }

    cJSON_AddItemToObject(data, CGI_AAA_METHODS, methods);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_aaa_loginEdit_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;
    cJSON               *methods = NULL;
    cJSON               *method = NULL;
    cJSON               *selMethods = NULL;
    uint32              i = 0;
    uint32              j = 0;
    uint32              cnt = 0;
    char                *value = NULL;
    char                selListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    char                authListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    sys_auth_type_t     authTypePrio[CAPA_AAA_AUTHTYPE_PRI_NUM];
    sys_authList_type_t authListType = SYS_AUTHLIST_TYPE_NONE;

    SYS_MEM_CLEAR(selListName);

    if (CGI_IS_PARAM_EXIST(CGI_AAA_LIST, value))
        osal_strncpy(selListName, value, sizeof(selListName));

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_AAA_LIST_MAXLEN, CAPA_AAA_KEY_LEN);

    methods = cJSON_CreateArray();

    for (i = 0; i < SYS_AUTH_TYPE_END; i++)
    {
        if ((SYS_AUTH_TYPE_LINE == i))
            continue;

        method = cJSON_CreateObject();

        cJSON_AddStringToObject(method, CGI_NAME, cgi_text_aaa_authMethod[i].text);
        cJSON_AddNumberToObject(method, CGI_VALUE, i);

        cJSON_AddItemToArray(methods, method);
    }

    cJSON_AddItemToObject(data, CGI_AAA_METHODS, methods);

    for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
    {
        SYS_MEM_CLEAR(authListName);
        SYS_MEM_CLEAR(authTypePrio);

        sal_aaa_authList_getByIdx(i, authListName, authTypePrio, &authListType);

        if (0 == osal_strlen(authListName) || (authListType != SYS_AUTHLIST_TYPE_AUTHEN_LOGIN))
            continue;

        if (0 != osal_strcmp(authListName, selListName))
            continue;

        selMethods = cJSON_CreateArray();

        cnt = 0;
        for(j = 0; j < CAPA_AAA_AUTHTYPE_PRI_NUM; j++)
        {
            cJSON_AddItemToArray(selMethods, cJSON_CreateNumber(authTypePrio[j]));

            cnt ++;
        }

        if (cnt > 0)
            cJSON_AddItemToObject(data, CGI_AAA_SELMETHOD, selMethods);

        break;
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_aaa_loginAuth_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;
    cJSON               *lists = NULL;
    cJSON               *list = NULL;
    cJSON               *methods = NULL;
    cJSON               *lines = NULL;
    cJSON               *line = NULL;
    uint32              i = 0;
    uint32              j = 0;
    uint32              cnt = 0;
    char                authListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    sys_auth_type_t     authTypePrio[CAPA_AAA_AUTHTYPE_PRI_NUM];
    sys_authList_type_t authListType = SYS_AUTHLIST_TYPE_NONE;

    CGI_GET_JSON_CREATE(root, data);

    lists = cJSON_CreateArray();

    for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
    {
        SYS_MEM_CLEAR(authListName);
        SYS_MEM_CLEAR(authTypePrio);

        sal_aaa_authList_getByIdx(i, authListName, authTypePrio, &authListType);

        if (0 == osal_strlen(authListName) || (authListType != SYS_AUTHLIST_TYPE_AUTHEN_LOGIN))
            continue;

        list = cJSON_CreateObject();
        cJSON_AddStringToObject(list, CGI_AAA_LIST, authListName);

        methods = cJSON_CreateArray();

        cnt = 0;
        for(j = 0; j < CAPA_AAA_AUTHTYPE_PRI_NUM; j++)
        {
            if (SYS_AUTH_TYPE_EMPTY == authTypePrio[j])
                break;

            cJSON_AddItemToArray(methods, cJSON_CreateString(cgi_text_aaa_authMethod[authTypePrio[j]].text));

            cnt ++;
        }

        if (cnt > 0)
            cJSON_AddItemToObject(list, CGI_AAA_METHODS, methods);

        cJSON_AddItemToArray(lists, list);
    }

    cJSON_AddItemToObject(data, CGI_AAA_LISTS, lists);

    lines = cJSON_CreateArray();

    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        line = cJSON_CreateObject();

        cJSON_AddStringToObject(line, CGI_NAME, cgi_text_aaa_line[i].text);
        cJSON_AddNumberToObject(line, CGI_VALUE, i);

        SYS_MEM_CLEAR(authListName);
        sal_aaa_loginAuthListName_get(i, authListName);
        cJSON_AddStringToObject(line, CGI_AAA_LIST, authListName);

        cJSON_AddItemToArray(lines, line);
    }

    cJSON_AddItemToObject(data, CGI_AAA_LINES, lines);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

