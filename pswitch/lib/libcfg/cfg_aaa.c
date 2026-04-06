
#include <libcfg/cfg.h>

int32 cfg_aaa_init(void)
{
    cfg_aaa_t   *pAaa = NULL;

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    pAaa->loginConsole = -1;
    pAaa->loginTelnet = -1;
    pAaa->loginSsh = -1;
    pAaa->loginHttp = -1;
    pAaa->loginHttps = -1;
    pAaa->enableConsole = -1;
    pAaa->enableTelnet = -1;
    pAaa->enableSsh = -1;

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_authList_set(int32 index, char *pAuthListName, sys_auth_type_t *pTypePrio, sys_authList_type_t authListType)
{
    cfg_aaa_t *pAaa = NULL;

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pTypePrio), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    osal_memset(pAaa->authLists[index].name, 0, sizeof(pAaa->authLists[index].name));
    osal_strcpy(pAaa->authLists[index].name, pAuthListName);

    osal_memcpy(pAaa->authLists[index].typePrio, pTypePrio, sizeof(pAaa->authLists[index].typePrio));

    pAaa->authLists[index].authListType = authListType;

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_authList_get(int32 index, char *pAuthListName, sys_auth_type_t *pTypePrio, sys_authList_type_t *pAuthListType)
{
    cfg_aaa_t *pAaa = NULL;

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pTypePrio), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pAuthListType), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    osal_strcpy(pAuthListName, pAaa->authLists[index].name);
    osal_memcpy(pTypePrio, pAaa->authLists[index].typePrio, sizeof(pAaa->authLists[index].typePrio));
    *pAuthListType = pAaa->authLists[index].authListType;

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_authListIndex_get(char *pAuthListName, sys_authList_type_t authListType, int32 *pIndex)
{
    cfg_aaa_t *pAaa = NULL;
    uint32 i;

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    *pIndex = -1;

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
    {
        if (0 == osal_strcmp(pAaa->authLists[i].name, pAuthListName)
            && pAaa->authLists[i].authListType == authListType)
        {
            *pIndex = i;
            break;
        }
    }

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_authListFirstEmptyIndex_get(int32 *pIndex)
{
    cfg_aaa_t *pAaa = NULL;
    uint32 i;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    *pIndex = -1;

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
    {
        if (0 == osal_strcmp(pAaa->authLists[i].name, ""))
        {
            *pIndex = i;
            break;
        }
    }

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_authListName_get(int32 index, char *pAuthListName)
{
    cfg_aaa_t *pAaa = NULL;

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    osal_strcpy(pAuthListName, pAaa->authLists[index].name);

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_authTypePrio_get(int32 index, sys_auth_type_t *pTypePrio)
{
    cfg_aaa_t *pAaa = NULL;

    SYS_PARAM_CHK((NULL == pTypePrio), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    osal_memcpy(pTypePrio, pAaa->authLists[index].typePrio, sizeof(pAaa->authLists[index].typePrio));

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_loginAuthListIndex_set(sys_access_type_t type, int32 index)
{
    cfg_aaa_t *pAaa = NULL;

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    switch(type)
    {
        case SYS_ACCESS_CLI_CONSOLE:
            pAaa->loginConsole = index;
            break;
        case SYS_ACCESS_CLI_TELNET:
            pAaa->loginTelnet = index;
            break;
        case SYS_ACCESS_CLI_SSH:
            pAaa->loginSsh = index;
            break;
        case SYS_ACCESS_WEB_HTTP:
            pAaa->loginHttp = index;
            break;
        case SYS_ACCESS_WEB_HTTPS:
            pAaa->loginHttps = index;
            break;
        default:
            break;
    }

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_loginAuthListIndex_get(sys_access_type_t type, int32 *pIndex)
{
    cfg_aaa_t *pAaa = NULL;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    switch(type)
    {
        case SYS_ACCESS_CLI_CONSOLE:
            *pIndex = pAaa->loginConsole;
            break;
        case SYS_ACCESS_CLI_TELNET:
            *pIndex = pAaa->loginTelnet;
            break;
        case SYS_ACCESS_CLI_SSH:
            *pIndex = pAaa->loginSsh;
            break;
        case SYS_ACCESS_WEB_HTTP:
            *pIndex = pAaa->loginHttp;
            break;
        case SYS_ACCESS_WEB_HTTPS:
            *pIndex = pAaa->loginHttps;
            break;
        default:
            break;
    }

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_enableAuthListIndex_set(sys_access_type_t type, int32 index)
{
    cfg_aaa_t *pAaa = NULL;

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    switch(type)
    {
        case SYS_ACCESS_CLI_CONSOLE:
            pAaa->enableConsole = index;
            break;
        case SYS_ACCESS_CLI_TELNET:
            pAaa->enableTelnet = index;
            break;
        case SYS_ACCESS_CLI_SSH:
            pAaa->enableSsh = index;
            break;
        default:
            break;
    }

    CFG_CLOSE(CFG_ID_AAA);
}

int32 cfg_aaa_enableAuthListIndex_get(sys_access_type_t type, int32 *pIndex)
{
    cfg_aaa_t *pAaa = NULL;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AAA, (cfg_addr)&pAaa);

    switch(type)
    {
        case SYS_ACCESS_CLI_CONSOLE:
            *pIndex = pAaa->enableConsole;
            break;
        case SYS_ACCESS_CLI_TELNET:
            *pIndex = pAaa->enableTelnet;
            break;
        case SYS_ACCESS_CLI_SSH:
            *pIndex = pAaa->enableSsh;
            break;
        default:
            break;
    }

    CFG_CLOSE(CFG_ID_AAA);
}

