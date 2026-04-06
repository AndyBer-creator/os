
#include <include/cgi.h>

typedef struct cgi_snmp_mibTree_s
{
    char mibName[SYS_BUF32_LEN];
    char oidStr[CAPA_SNMP_OID_STR_LEN];
} cgi_snmp_mibTree_t;

typedef struct cgi_snmp_accGrp_s
{
    uint8   isSecValid[SNMP_SECLEVEL_END];
    char name[CAPA_SNMP_GROUP_NAME_LEN+1];
    sal_snmp_secmode_t secMode;

    uint32 idx;
    uint32 dataBaseIdx[SNMP_SECLEVEL_END];
    char readView[SNMP_SECLEVEL_END][CAPA_SNMP_VIEW_NAME_LEN+1];
    char writeView[SNMP_SECLEVEL_END][CAPA_SNMP_VIEW_NAME_LEN+1];
    char notifyView[SNMP_SECLEVEL_END][CAPA_SNMP_VIEW_NAME_LEN+1];
}cgi_snmp_accGrp_t;

const char *cgiSecLevel[] =
{
    "lblNoAuth",
    "lblAuth",
    "lblPriv",
};

const char *cgiSslType[] =
{
    "lblNone",
    "lblMD5",
    "lblSHA",
    "lblDES",
};

const char *cgiAccess[] =
{
    "lblReadOnly",
    "lblReadWrite",
};

const char *cgiSecModel[] =
{
    "lblSNMPv1",
    "lblSNMPv2",
    "lblSNMPv3",
};

const char *cgiTraptype[] =
{
    "lblTraps",
    "lblTraps",
    "lblInforms",
    "lblTraps",
    "lblInforms",
};

const char *cgiSecModelVal[] =
{
    "v1",
    "v2",
    "v3"
};

const char *cgiTrapVerVal[] =
{
    "v1",
    "v2",
    "v2",
    "v3",
    "v3",
};

static int32 cgi_snmp_community_get(void);
static int32 cgi_snmp_communityAdd_get(void);

static int32 cgi_snmp_trapEvent_get(void);

static int32 cgi_snmp_notify_get(void);
static int32 cgi_snmp_notifyAdd_get(void);

#ifdef CONFIG_SYS_SNMPV3
static int32 cgi_snmp_communityEdit_get(void);
static int32 cgi_snmp_notifyEdit_get(void);

static int32 cgi_snmp_view_get(void);
static int32 cgi_snmp_viewAdd_get(void);

static int32 cgi_snmp_group_get(void);
static int32 cgi_snmp_groupEdit_get(void);
static int32 cgi_snmp_groupAdd_get(void);

static int32 cgi_snmp_user_get(void);
static int32 cgi_snmp_userEdit_get(void);
static int32 cgi_snmp_userAdd_get(void);

static int32 cgi_snmp_engineId_get(void);
static int32 cgi_snmp_engineIdEdit_get(void);
static int32 cgi_snmp_engineIdAdd_get(void);

#endif

const cgi_cmdmap_t cmdGetSnmpTbl[] =
{
    {CGI_CMD_SNMP_COMMUNITY,            cgi_snmp_community_get},
    {CGI_CMD_SNMP_COMMUNITYADD,            cgi_snmp_communityAdd_get},

    {CGI_CMD_SNMP_TRAPEVENT,           cgi_snmp_trapEvent_get},

    {CGI_CMD_SNMP_NOTIFY,            cgi_snmp_notify_get},
    {CGI_CMD_SNMP_NOTIFYADD,            cgi_snmp_notifyAdd_get},

    #ifdef CONFIG_SYS_SNMPV3
    {CGI_CMD_SNMP_COMMUNITYEDIT,            cgi_snmp_communityEdit_get},
    {CGI_CMD_SNMP_NOTIFYEDIT,            cgi_snmp_notifyEdit_get},

    {CGI_CMD_SNMP_VIEW,        cgi_snmp_view_get},
    {CGI_CMD_SNMP_VIEWADD,        cgi_snmp_viewAdd_get},

    {CGI_CMD_SNMP_GROUP,            cgi_snmp_group_get},
    {CGI_CMD_SNMP_GROUPEDIT,            cgi_snmp_groupEdit_get},
    {CGI_CMD_SNMP_GROUPADD,            cgi_snmp_groupAdd_get},

    {CGI_CMD_SNMP_USER,            cgi_snmp_user_get},
    {CGI_CMD_SNMP_USEREDIT,            cgi_snmp_userEdit_get},
    {CGI_CMD_SNMP_USERADD,            cgi_snmp_userAdd_get},

    {CGI_CMD_SNMP_ENGINEID,            cgi_snmp_engineId_get},
    {CGI_CMD_SNMP_ENGINEIDEDIT,            cgi_snmp_engineIdEdit_get},
    {CGI_CMD_SNMP_ENGINEIDADD,            cgi_snmp_engineIdAdd_get},
    #endif
    {NULL, NULL}
};

static int32 cgi_snmp_trapEvent_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_enable_t authEnable = DISABLED;
    sys_enable_t linkEnable = DISABLED;
    sys_enable_t warmEnable = DISABLED;
    sys_enable_t coldEnable = DISABLED;

    CGI_GET_JSON_CREATE(root, data);

    sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &authEnable);
    sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_LINKUPDOWN, &linkEnable);
    sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_COOL, &coldEnable);
    sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_WARM, &warmEnable);

    cJSON_AddBoolToObject(data, CGI_SNMP_TRAP_AUTHFAIL, authEnable);
    cJSON_AddBoolToObject(data, CGI_SNMP_TRAP_LINKUPDOWN, linkEnable);
    cJSON_AddBoolToObject(data, CGI_SNMP_TRAP_COLDSTART, coldEnable);
    cJSON_AddBoolToObject(data, CGI_SNMP_TRAP_WARMSTART, warmEnable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);
    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_SNMPV3
static int32 _cgi_snmp_viewNameList_get(char (*pViewNameArry)[CAPA_SNMP_VIEW_NAME_LEN+1], uint32 *pViewNameCnt)
{
    uint32 index;
    uint32 viewNameCnt = 0, viewIdx;
    sal_snmp_view_t view;

    SYS_PARAM_CHK((NULL == pViewNameArry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pViewNameCnt), SYS_ERR_NULL_POINTER);

    for (index = 0; index < CAPA_SNMP_VIEW_NUM; index++)
    {
        if (NULL == pViewNameArry[index])
            return SYS_ERR_NULL_POINTER;

        SYS_MEM_CLEAR(pViewNameArry[index]);
        SYS_ERR_CHK(sal_snmp_view_get(index, &view));
        if (0 != osal_strlen(view.viewName))
        {
            for(viewIdx = 0; viewIdx < viewNameCnt; viewIdx++)
            {
                if (0 == osal_strcmp(view.viewName, pViewNameArry[viewIdx]))
                {
                    break;
                }
            }

            if (viewIdx == viewNameCnt)
            {
                strlcpy(pViewNameArry[viewNameCnt], view.viewName, (CAPA_SNMP_VIEW_NAME_LEN+1));
                viewNameCnt++;
            }
        }
    }

    *pViewNameCnt = viewNameCnt;
    return SYS_ERR_OK;
}

static int32 _cgi_snmp_accGrpList_get(cgi_snmp_accGrp_t *pGrpArry, uint32 *pAccGrpCnt)
{
    uint32 index;
    uint32 groupCnt = 0, grpIdx = 0;
    sal_snmp_access_t accGrpEntry;

    SYS_PARAM_CHK((NULL == pGrpArry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pGrpArry), SYS_ERR_NULL_POINTER);

    for (index = 0; index < SYS_SNMP_ACCESS_NUM_MAX; index++)
    {
        SYS_MEM_CLEAR(accGrpEntry);
        sal_snmp_access_get(index, &accGrpEntry);
         if(0 != osal_strlen(accGrpEntry.groupName))
         {
            if (0 == osal_memcmp(accGrpEntry.groupName, SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
                continue;

            if (SNMP_SECMODEL_USM !=  accGrpEntry.secMode)
            {
                pGrpArry[groupCnt].secMode = accGrpEntry.secMode;
                pGrpArry[groupCnt].isSecValid[accGrpEntry.secLevel] = TRUE;
                pGrpArry[groupCnt].dataBaseIdx[accGrpEntry.secLevel] = index+1;
                pGrpArry[groupCnt].idx= index + 1;

                strlcpy(pGrpArry[groupCnt].name, accGrpEntry.groupName, sizeof(pGrpArry[groupCnt].name));
                strlcpy(pGrpArry[groupCnt].readView[accGrpEntry.secLevel], accGrpEntry.readView, sizeof(pGrpArry[groupCnt].readView[accGrpEntry.secLevel]));
                strlcpy(pGrpArry[groupCnt].writeView[accGrpEntry.secLevel], accGrpEntry.writeView, sizeof(pGrpArry[groupCnt].writeView[accGrpEntry.secLevel]));
                strlcpy(pGrpArry[groupCnt].notifyView[accGrpEntry.secLevel], accGrpEntry.notifyView, sizeof(pGrpArry[groupCnt].notifyView[accGrpEntry.secLevel]));
                groupCnt++;
            }
            else
            {
                for(grpIdx = 0; grpIdx < groupCnt; grpIdx++)
                {
                    if ((0 == osal_strcmp(accGrpEntry.groupName, pGrpArry[grpIdx].name)) && (SNMP_SECMODEL_USM == pGrpArry[grpIdx].secMode))
                    {
                        pGrpArry[grpIdx].secMode = accGrpEntry.secMode;
                        pGrpArry[grpIdx].isSecValid[accGrpEntry.secLevel] = TRUE;

                        pGrpArry[grpIdx].dataBaseIdx[accGrpEntry.secLevel] = index+1;
                        pGrpArry[grpIdx].idx += ((index+1) << (8 * accGrpEntry.secLevel));

                        strlcpy(pGrpArry[grpIdx].readView[accGrpEntry.secLevel], accGrpEntry.readView, sizeof(pGrpArry[grpIdx].readView[accGrpEntry.secLevel]));
                        strlcpy(pGrpArry[grpIdx].writeView[accGrpEntry.secLevel], accGrpEntry.writeView, sizeof(pGrpArry[grpIdx].writeView[accGrpEntry.secLevel]));
                        strlcpy(pGrpArry[grpIdx].notifyView[accGrpEntry.secLevel], accGrpEntry.notifyView, sizeof(pGrpArry[grpIdx].notifyView[accGrpEntry.secLevel]));
                        break;
                    }
                }

                if (grpIdx == groupCnt)
                {
                    pGrpArry[groupCnt].secMode = accGrpEntry.secMode;
                    pGrpArry[groupCnt].isSecValid[accGrpEntry.secLevel] = TRUE;

                    pGrpArry[groupCnt].dataBaseIdx[accGrpEntry.secLevel] = index+1;
                    pGrpArry[groupCnt].idx = ((index+1) << (8 * accGrpEntry.secLevel));

                    strlcpy(pGrpArry[groupCnt].name, accGrpEntry.groupName, sizeof(pGrpArry[groupCnt].name));
                    strlcpy(pGrpArry[groupCnt].readView[accGrpEntry.secLevel], accGrpEntry.readView, sizeof(pGrpArry[groupCnt].readView[accGrpEntry.secLevel]));
                    strlcpy(pGrpArry[groupCnt].writeView[accGrpEntry.secLevel], accGrpEntry.writeView, sizeof(pGrpArry[groupCnt].writeView[accGrpEntry.secLevel]));
                    strlcpy(pGrpArry[groupCnt].notifyView[accGrpEntry.secLevel], accGrpEntry.notifyView, sizeof(pGrpArry[groupCnt].notifyView[accGrpEntry.secLevel]));
                    groupCnt++;
                }
            }
         }
    }

    *pAccGrpCnt = groupCnt;

    return SYS_ERR_OK;
}

 static int32 cgi_snmp_view_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *views = NULL;
    cJSON *view = NULL;
    uint32 entry = 0;

    int32 index;
    sal_snmp_view_t viewEntry;
    char buf[SYS_BUF128_LEN];

    for (index = 0; index < CAPA_SNMP_VIEW_NUM; index++)
    {
        SYS_MEM_CLEAR(viewEntry);
        sal_snmp_view_get(index, &viewEntry);
        if(0 != osal_strlen(viewEntry.viewName))
            entry++;
    }

    CGI_GET_JSON_CREATE(root, data);

    if (0  != entry)
    {
        views = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_SNMP_VIEWS, views);
        for (index = 0; index < CAPA_SNMP_VIEW_NUM; index++)
        {
            SYS_MEM_CLEAR(viewEntry);
            sal_snmp_view_get(index, &viewEntry);
             if(0 != osal_strlen(viewEntry.viewName))
             {
                 view = cJSON_CreateObject();
                 cJSON_AddNumberToObject(view, CGI_SNMP_IDX, index);
                 cJSON_AddStringToObject(view, CGI_SNMP_VIEW_NAME, viewEntry.viewName);

                SYS_MEM_CLEAR(buf);
                sal_util_oid2str(viewEntry.subtreeOid.len, viewEntry.subtreeOid.id, buf);
                cJSON_AddStringToObject(view, CGI_SNMP_VIEW_SUBTREE, buf);
                if (SNMP_VIEWTYPE_INCLUDE == viewEntry.viewType)
                    cJSON_AddStringToObject(view, CGI_SNMP_VIEW_TYPE,  CGI_LANG("snmp",  "lblIncluded"));
                else
                    cJSON_AddStringToObject(view, CGI_SNMP_VIEW_TYPE,  CGI_LANG("snmp",  "lblExcluded"));
                cJSON_AddItemToArray(views, view);
             }
        }
    }
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_viewAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_SNMP_VIEW_MAX_LEN, CAPA_SNMP_VIEW_NAME_LEN);
    cJSON_AddBoolToObject(data, CGI_SNMP_VIEW_TYPE_INCLUDE, TRUE);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);
    return SYS_ERR_OK;
}

static int32 cgi_snmp_group_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *groups = NULL;
    cJSON *group = NULL;
    cJSON *views = NULL;

    int32 index;
    int32 entry = 0;
    char viewName[CAPA_SNMP_VIEW_NUM][CAPA_SNMP_VIEW_NAME_LEN+1];
    uint32 viewNameCnt = 0;
    sal_snmp_access_t access;

    SYS_MEM_CLEAR(viewName);

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    _cgi_snmp_viewNameList_get(viewName, &viewNameCnt);
    views = cJSON_CreateArray();
    for (index = 0; index < viewNameCnt; index++)
    {
        cJSON_AddItemToArray(views, cJSON_CreateString(viewName[index]));
    }
    cJSON_AddItemToObject(data, CGI_SNMP_VIEWS, views);

    groups = cJSON_CreateArray();
    for (index = 0; index < SYS_SNMP_ACCESS_NUM_MAX; index++)
    {
        SYS_ERR_CHK(sal_snmp_access_get(index, &access));
        if (osal_strlen(access.groupName) != 0)
        {
            if (0 == osal_memcmp(access.groupName, SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
                continue;

            group = cJSON_CreateObject();
            cJSON_AddNumberToObject(group, CGI_SNMP_INDEX, index);

            cJSON_AddStringToObject(group, CGI_SNMP_GROUP_NAME, access.groupName);

            if (SNMP_SECMODEL_V1 ==  access.secMode)
                cJSON_AddStringToObject(group, CGI_SNMP_GROUP_SECMODE, CGI_LANG("snmp", "lblSNMPv1"));
            else if (SNMP_SECMODEL_V2C ==  access.secMode)
                cJSON_AddStringToObject(group, CGI_SNMP_GROUP_SECMODE, CGI_LANG("snmp", "lblSNMPv2"));
            else
                cJSON_AddStringToObject(group, CGI_SNMP_GROUP_SECMODE, CGI_LANG("snmp", "lblSNMPv3"));

            if (SNMP_SECLEVEL_NOAUTHNOPRIV ==  access.secLevel)
                cJSON_AddStringToObject(group, CGI_SNMP_GROUP_SECLEVEL, CGI_LANG("snmp", "lblNoAuth"));
            else if (SNMP_SECLEVEL_AUTHNOPRIV ==  access.secLevel)
                cJSON_AddStringToObject(group, CGI_SNMP_GROUP_SECLEVEL, CGI_LANG("snmp", "lblAuth"));
            else
                cJSON_AddStringToObject(group, CGI_SNMP_GROUP_SECLEVEL, CGI_LANG("snmp", "lblPriv"));

            cJSON_AddStringToObject(group, CGI_SNMP_GROUP_VIEW_READ, access.readView);
            if (osal_strlen(access.writeView) > 0)
                cJSON_AddStringToObject(group, CGI_SNMP_GROUP_VIEW_WRITE, access.writeView);
            if (osal_strlen(access.notifyView) > 0)
                cJSON_AddStringToObject(group, CGI_SNMP_GROUP_VIEW_NOTIFY, access.notifyView);

            cJSON_AddItemToArray(groups, group);
            entry++;
        }
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_SNMP_GROUPS, groups);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_groupAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *views = NULL;

    int32 index;
    char viewName[CAPA_SNMP_VIEW_NUM][CAPA_SNMP_VIEW_NAME_LEN+1];
    uint32 viewNameCnt = 0;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_SNMP_GROUP_MAX_LEN, CAPA_SNMP_GROUP_NAME_LEN);

    _cgi_snmp_viewNameList_get(viewName, &viewNameCnt);

    views = cJSON_CreateArray();
    for (index = 0; index < viewNameCnt; index++)
    {
        cJSON_AddItemToArray(views, cJSON_CreateString(viewName[index]));
    }

    cJSON_AddItemToObject(data, CGI_SNMP_VIEWS, views);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_groupEdit_get(void)
{
    cgi_snmp_groupAdd_get();

    return SYS_ERR_OK;
}

static int32 cgi_snmp_community_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *coms = NULL;
    cJSON *com = NULL;

    int32 index;
    sal_snmp_com2sec_t comEntry;
    int32 entry = 0;

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    coms = cJSON_CreateArray();
    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_MEM_CLEAR(comEntry);
        sal_snmp_com2sec_get(index, &comEntry);
         if(0 != osal_strlen(comEntry.comName))
         {
            com = cJSON_CreateObject();
            cJSON_AddNumberToObject(com, CGI_SNMP_INDEX, index);
            if (SNMP_COM_MODE_BASIC == comEntry.mode)
            {
                cJSON_AddStringToObject(com, CGI_SNMP_COM_NAME, comEntry.comName);
                cJSON_AddStringToObject(com, CGI_SNMP_COM_TYPE, CGI_LANG("snmp", "lblBasic"));
                cJSON_AddLangStringToObject(com, CGI_SNMP_COM_ACCESS, "snmp", cgiAccess[comEntry.access]);
                cJSON_AddStringToObject(com, CGI_SNMP_COM_VIEW, comEntry.viewName);
            }
            else
            {
                cJSON_AddStringToObject(com, CGI_SNMP_COM_NAME, comEntry.comName);
                cJSON_AddStringToObject(com, CGI_SNMP_COM_TYPE, CGI_LANG("snmp", "lblAdvanced"));
                cJSON_AddStringToObject(com, CGI_SNMP_COM_GROUP, comEntry.groupName);
            }

            cJSON_AddItemToArray(coms, com);
            entry++;
         }
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_SNMP_COMS, coms);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_communityAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *groups = NULL;
    cJSON *views = NULL;

    int32 index;
    char viewName[CAPA_SNMP_VIEW_NUM][CAPA_SNMP_VIEW_NAME_LEN+1];
    uint32 viewNameCnt = 0;
    sal_snmp_access_t groupEntry;
    cgi_snmp_accGrp_t accGrpEntry[SYS_SNMP_ACCESS_NUM_MAX];
    uint32 grpCnt = 0, grpIdx;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_SNMP_COM_MAX_LEN, CAPA_SNMP_COMMUNITY_LEN);

    _cgi_snmp_viewNameList_get(viewName, &viewNameCnt);
    views = cJSON_CreateArray();
    for (index = 0; index < viewNameCnt; index++)
    {
        cJSON_AddItemToArray(views, cJSON_CreateString(viewName[index]));
    }
    cJSON_AddItemToObject(data, CGI_SNMP_VIEWS, views);

    SYS_MEM_CLEAR(accGrpEntry);
    groups = cJSON_CreateArray();
    for (index = 0; index < SYS_SNMP_ACCESS_NUM_MAX; index++)
    {
        SYS_MEM_CLEAR(groupEntry);
        sal_snmp_access_get(index, &groupEntry);
        if(0 != osal_strlen(groupEntry.groupName))
        {
            if (SNMP_SECMODEL_USM != groupEntry.secMode)
            {
                if (0 == osal_memcmp(groupEntry.groupName, SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
                    continue;

                for(grpIdx = 0; grpIdx < grpCnt; grpIdx++)
                {
                    if (0 == osal_strcmp(groupEntry.groupName, accGrpEntry[grpIdx].name))
                    {
                        break;
                    }
                }

                if (grpIdx == grpCnt)
                {
                    strlcpy(accGrpEntry[grpIdx].name, groupEntry.groupName, sizeof(accGrpEntry[grpIdx].name));
                    cJSON_AddItemToArray(groups, cJSON_CreateString(groupEntry.groupName));
                    grpCnt++;
                }
            }
        }
    }

    if (grpCnt > 0)
        cJSON_AddItemToObject(data, CGI_SNMP_COM_USE_GRP, groups);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_communityEdit_get(void)
{
    cgi_snmp_communityAdd_get();

    return SYS_ERR_OK;
}

static int32 cgi_snmp_user_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *users = NULL;
    cJSON *user = NULL;

    int32 index;
    sal_snmp_user_t userEntry;
    int32 entry = 0;
    cgi_snmp_accGrp_t accGrpEntry[SYS_SNMP_ACCESS_NUM_MAX];
    uint32 grpCnt = 0, grpV3Cnt = 0;

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    SYS_MEM_CLEAR(accGrpEntry);
    _cgi_snmp_accGrpList_get(accGrpEntry, &grpCnt);
    for (index = 0; index < grpCnt; index++)
    {
        if (SNMP_SECMODEL_USM == accGrpEntry[index].secMode)
            grpV3Cnt++;
    }
    cJSON_AddBoolToObject(data, CGI_SNMP_USER_HASGRP, (0 == grpV3Cnt) ? FALSE:TRUE);

    users = cJSON_CreateArray();

    for (index = 0; index < CAPA_SNMP_USER_NUM; index++)
    {
        SYS_MEM_CLEAR(userEntry);
        sal_snmp_user_get(index, &userEntry);
         if(0 != osal_strlen(userEntry.userName))
         {
            user = cJSON_CreateObject();
            cJSON_AddStringToObject(user, CGI_SNMP_USER_NAME, userEntry.userName);
            cJSON_AddStringToObject(user, CGI_SNMP_USER_GROUP_NAME, userEntry.groupName);
            cJSON_AddLangStringToObject(user, CGI_SNMP_USER_SECURITY_LEVEL, "snmp", cgiSecLevel[userEntry.selLevel]);
            cJSON_AddLangStringToObject(user, CGI_SNMP_USER_AUTH_METHOD, "snmp", cgiSslType[userEntry.auth_protp]);
            cJSON_AddLangStringToObject(user, CGI_SNMP_USER_PRIV_METHOD, "snmp", cgiSslType[userEntry.encrypt_proto]);
            cJSON_AddItemToArray(users, user);
            entry++;
         }
    }

    if (entry > 0)
         cJSON_AddItemToObject(data, CGI_SNMP_USERS, users);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_userAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *groups = NULL;
    cJSON *group = NULL;

    int32 index;
    uint32 entry = 0;
    cgi_snmp_accGrp_t accGrpEntry[SYS_SNMP_ACCESS_NUM_MAX];
    uint32 grpCnt = 0;
    sal_snmp_seclevel_t secLev = SNMP_SECLEVEL_NOAUTHNOPRIV, getLev;
    char grpNameVal[CAPA_SNMP_GROUP_NAME_LEN+3];

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_SNMP_USER_NAME_MAX, CAPA_SNMP_USER_NAME_LEN);
    cJSON_AddNumberToObject(data, CGI_SNMP_USER_AUTHPSWD_MAX, BOND_SNMP_AUTH_KEY_LEN_MAX);
    cJSON_AddNumberToObject(data, CGI_SNMP_USER_PRIVPSWD_MAX, BOND_SNMP_PRIV_KEY_LEN_MAX);

    SYS_MEM_CLEAR(accGrpEntry);
    _cgi_snmp_accGrpList_get(accGrpEntry, &grpCnt);

    groups = cJSON_CreateArray();
    for (index = 0; index < grpCnt; index++)
    {
        if (SNMP_SECMODEL_USM == accGrpEntry[index].secMode)
        {
            group = cJSON_CreateObject();
            cJSON_AddStringToObject(group, CGI_NAME, accGrpEntry[index].name);
            getLev = 0;
            for (secLev = SNMP_SECLEVEL_NOAUTHNOPRIV ; secLev <= SNMP_SECLEVEL_AUTHPRIV; secLev++ )
            {
                if (accGrpEntry[index].isSecValid[secLev])
                        getLev += (1 << secLev);
            }
            SYS_MEM_CLEAR(grpNameVal);
            osal_sprintf(grpNameVal,"%s_%d", accGrpEntry[index].name, getLev);
            cJSON_AddStringToObject(group, CGI_VALUE, grpNameVal);
            cJSON_AddItemToArray(groups, group);
            entry++;
        }
    }

    if (entry > 0)
    {
         cJSON_AddItemToObject(data, CGI_SNMP_USER_GRP, groups);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_userEdit_get(void)
{
    cgi_snmp_userAdd_get();
    return SYS_ERR_OK;
}

static int32 cgi_snmp_engineId_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *remotes = NULL;
    cJSON *remote = NULL;

    char dfltEngineStr[BOND_SNMP_ENGINEID_LEN_MAX + 1];
    char engineStr[BOND_SNMP_ENGINEID_LEN_MAX + 1];
    uint8 isUserdef = FALSE;
    int32 index;
    sal_snmp_remoteEngine_t   remoteEngine;
    uint8 entry = 0;

    SYS_MEM_CLEAR(dfltEngineStr);
    SYS_MEM_CLEAR(engineStr);

    sal_snmp_defaultEngineId_get(dfltEngineStr);
    sal_snmp_engineid_get(engineStr);
    if (0 != osal_strcmp(dfltEngineStr, engineStr))
         isUserdef = TRUE;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_SNMP_ENGINEID_USEDEF, isUserdef);
    cJSON_AddStringToObject(data, CGI_SNMP_ENGINEID, engineStr);
    cJSON_AddNumberToObject(data, CGI_SNMP_ENGINEID_MIN_LEN, 10);
    cJSON_AddNumberToObject(data, CGI_SNMP_ENGINEID_MAX_LEN, BOND_SNMP_ENGINEID_LEN_MAX);
    cJSON_AddNumberToObject(data, CGI_MAXLEN_SRVHOST, CAPA_HOSTNAME_LEN);

    remotes = cJSON_CreateArray();
    for (index = 0; index < CAPA_SNMP_REMOTE_ENGINEID_NUM; index++)
    {
        SYS_MEM_CLEAR(remoteEngine);
        sal_snmp_remoteEngine_get(index, &remoteEngine);
         if(0 != osal_strlen(remoteEngine.server))
         {
             remote = cJSON_CreateObject();
             cJSON_AddStringToObject(remote, CGI_SNMP_REMOTE_ENGINEID_IP, remoteEngine.server);
             cJSON_AddStringToObject(remote, CGI_SNMP_REMOTE_ENGINEID_NO, remoteEngine.engineId);
             cJSON_AddItemToArray(remotes, remote);
             entry++;
         }
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_SNMP_REMOTE_ENGINEID, remotes);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_engineIdAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_MAXLEN_SRVHOST, CAPA_HOSTNAME_LEN);
    cJSON_AddNumberToObject(data, CGI_SNMP_ENGINEID_MIN_LEN, BOND_SNMP_ENGINEID_LEN_MIN);
    cJSON_AddNumberToObject(data, CGI_SNMP_ENGINEID_MAX_LEN, BOND_SNMP_ENGINEID_LEN_MAX);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_engineIdEdit_get(void)
{
    cgi_snmp_engineIdAdd_get();
    return SYS_ERR_OK;
}

static int32 cgi_snmp_notify_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *hosts = NULL;
    cJSON *host = NULL;

    int32 index;
    sal_snmp_com2sec_t comEntry;
    sal_snmp_user_t  userEntry;
    sal_snmp_target_t  hostEntry;
    int32 entry = 0;

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_MEM_CLEAR(comEntry);
        sal_snmp_com2sec_get(index, &comEntry);
        if(0 != osal_strlen(comEntry.comName))
            entry++;
    }
    cJSON_AddBoolToObject(data, CGI_SNMP_HOST_HASCOM, (0 == entry) ? FALSE:TRUE);

    entry = 0;
    for (index = 0; index < CAPA_SNMP_USER_NUM; index++)
    {
        SYS_MEM_CLEAR(userEntry);
        sal_snmp_user_get(index, &userEntry);
        if(0 != osal_strlen(userEntry.userName))
            entry++;
    }
    cJSON_AddBoolToObject(data, CGI_SNMP_HOST_HASUSER, (0 == entry) ? FALSE:TRUE);

    entry = 0;
    hosts = cJSON_CreateArray();
    for (index = 0; index < CAPA_SNMP_HOST_NUM; index++)
    {
        SYS_MEM_CLEAR(hostEntry);
        sal_snmp_target_get(index, &hostEntry);
         if(0 != osal_strlen(hostEntry.server))
         {
            host = cJSON_CreateObject();
            cJSON_AddNumberToObject(host, CGI_SNMP_INDEX, index);
            cJSON_AddStringToObject(host, CGI_SNMP_HOST_SERVER, hostEntry.server);
            cJSON_AddLangStringToObject(host, CGI_SNMP_HOST_VERSION, "snmp", cgiSecModel[hostEntry.secModel]);
            cJSON_AddLangStringToObject(host, CGI_SNMP_HOST_TYPE, "snmp", cgiTraptype[hostEntry.trapType]);
            cJSON_AddLangStringToObject(host, CGI_SNMP_HOST_SECLVL, "snmp", cgiSecLevel[hostEntry.secLevel]);
            cJSON_AddStringToObject(host, CGI_SNMP_HOST_SECNAMESTR, hostEntry.name);
            cJSON_AddNumberToObject(host, CGI_SNMP_HOST_UDPPORT, hostEntry.portNum);

            if (SNMP_INFORM_V2 == hostEntry.trapType || SNMP_INFORM_V3 == hostEntry.trapType)
            {
                cJSON_AddNumberToObject(host, CGI_SNMP_HOST_TIMEOUT, hostEntry.timeout);
                cJSON_AddNumberToObject(host, CGI_SNMP_HOST_RETRIES, hostEntry.retries);
            }

            cJSON_AddItemToArray(hosts, host);
            entry++;
         }
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_SNMP_HOST, hosts);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_notifyAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *coms = NULL;
    cJSON *sec_user = NULL;
    cJSON *users = NULL;
    cJSON *user = NULL;

    int32 index;
    sal_snmp_com2sec_t  comEntry;
    sal_snmp_user_t   userEntry;
    int32 entry = 0;
    char user_selv[CAPA_SNMP_USER_NAME_LEN+3];
    uint8 binLvl = 0;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_MAXLEN_SRVHOST, CAPA_HOSTNAME_LEN);

    cJSON_AddNumberToObject(data, CGI_SNMP_HOST_PORT_DLF, DFLT_SNMP_NOTIFACTION_PORT);
    cJSON_AddNumberToObject(data, CGI_SNMP_HOST_DLF_TIMEOUT, DFLT_SNMP_INFORM_TIMEOUT);
    cJSON_AddNumberToObject(data, CGI_SNMP_HOST_RETRY_DLT, DFLT_SNMP_INFORM_RETRY);

    cJSON_AddNumberToObject(data, CGI_SNMP_HOST_PORT_MIN, BOND_SNMP_NOTIFY_PORT_MIN);
    cJSON_AddNumberToObject(data, CGI_SNMP_HOST_PORT_MAX, BOND_SNMP_NOTIFY_PORT_MAX);

    cJSON_AddNumberToObject(data, CGI_SNMP_HOST_TIMEOUT_MIN, BOND_SNMP_NOTIFY_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_SNMP_HOST_TIMEOUT_MAX, BOND_SNMP_NOTIFY_TIMEOUT_MAX);

    cJSON_AddNumberToObject(data, CGI_SNMP_HOST_RETRY_MIN, BOND_SNMP_NOTIFY_RETRY_MIN);
    cJSON_AddNumberToObject(data, CGI_SNMP_HOST_RETRY_MAX, BOND_SNMP_NOTIFY_RETRY_MAX);

    coms = cJSON_CreateArray();
    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_MEM_CLEAR(comEntry);
        sal_snmp_com2sec_get(index, &comEntry);
        if (0 != osal_strlen(comEntry.comName))
        {
            cJSON_AddItemToArray(coms, cJSON_CreateString(comEntry.comName));
            entry++;
        }
    }

    cJSON_AddBoolToObject(data, CGI_SNMP_HOST_HASCOM, (0 == entry) ? FALSE:TRUE);

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_SNMP_HOST_SECCOM, coms);

    entry = 0;
    users = cJSON_CreateArray();
    sec_user = cJSON_CreateArray();
    for (index = 0; index < CAPA_SNMP_USER_NUM; index++)
    {
        SYS_MEM_CLEAR(userEntry);
        sal_snmp_user_get(index, &userEntry);
        if (0 != osal_strlen(userEntry.userName))
        {
            cJSON_AddItemToArray(sec_user, cJSON_CreateString(userEntry.userName));

            user = cJSON_CreateObject();
            SYS_MEM_CLEAR(user_selv);
            cJSON_AddStringToObject(user, CGI_NAME, userEntry.userName);

            binLvl = 0;
            if (SNMP_SECLEVEL_NOAUTHNOPRIV == userEntry.selLevel)
                binLvl = 0x1;
            else if (SNMP_SECLEVEL_AUTHNOPRIV == userEntry.selLevel)
                binLvl = 0x2;
            else
                binLvl = 0x4;
            osal_sprintf(user_selv, "%s_%d", userEntry.userName, binLvl);

            cJSON_AddStringToObject(user, CGI_VALUE, user_selv);
            cJSON_AddItemToArray(users, user);
            entry++;
        }
    }

    cJSON_AddBoolToObject(data, CGI_SNMP_HOST_HASUSER, (0 == entry) ? FALSE:TRUE);
    if (entry > 0)
    {
        cJSON_AddItemToObject(data, CGI_SNMP_HOST_SECUSER, sec_user);
        cJSON_AddItemToObject(data, CGI_SNMP_USERS, users);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_notifyEdit_get(void)
{
    cgi_snmp_notifyAdd_get();
    return SYS_ERR_OK;
}
#else
static int32 cgi_snmp_community_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *coms = NULL;
    cJSON *com = NULL;

    int32 index;
    sal_snmp_community_t comEntry;
    int32 entry = 0;

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    coms = cJSON_CreateArray();
    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_MEM_CLEAR(comEntry);
        sal_snmp_community_get(index, &comEntry);
         if(0 != osal_strlen(comEntry.name))
         {
            com = cJSON_CreateObject();
            cJSON_AddStringToObject(com, CGI_SNMP_COM_NAME, comEntry.name);
            cJSON_AddLangStringToObject(com, CGI_SNMP_COM_ACCESS, "snmp", cgiAccess[comEntry.access]);

            cJSON_AddItemToArray(coms, com);
            entry++;
         }
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_SNMP_COMS, coms);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_communityAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_SNMP_COM_MAX_LEN, CAPA_SNMP_COMMUNITY_LEN);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_notify_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *hosts = NULL;
    cJSON *host = NULL;

    int32 index;
    sal_snmp_target_t  hostEntry;
    sal_snmp_community_t comEntry;
    int32 entry = 0;

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_MEM_CLEAR(comEntry);
        sal_snmp_community_get(index, &comEntry);
        if(0 != osal_strlen(comEntry.name))
            entry++;
    }
    cJSON_AddBoolToObject(data, CGI_SNMP_HOST_HASCOM, (0 == entry) ? FALSE:TRUE);

    entry = 0;
    hosts = cJSON_CreateArray();
    for (index = 0; index < CAPA_SNMP_HOST_NUM; index++)
    {
        SYS_MEM_CLEAR(hostEntry);
        sal_snmp_target_get(index, &hostEntry);
         if(0 != osal_strlen(hostEntry.server))
         {
            host = cJSON_CreateObject();
            cJSON_AddNumberToObject(host, CGI_SNMP_INDEX, index);
            cJSON_AddStringToObject(host, CGI_SNMP_HOST_SERVER, hostEntry.server);
            cJSON_AddLangStringToObject(host, CGI_SNMP_HOST_VERSION, "snmp", cgiSecModel[hostEntry.secModel]);
            cJSON_AddLangStringToObject(host, CGI_SNMP_HOST_TYPE, "snmp", cgiTraptype[hostEntry.trapType]);
            cJSON_AddStringToObject(host, CGI_SNMP_HOST_SECNAMESTR, hostEntry.name);

            cJSON_AddItemToArray(hosts, host);
            entry++;
         }
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_SNMP_HOST, hosts);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_notifyAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *coms = NULL;

    int32 index;
    sal_snmp_community_t  comEntry;
    int32 entry = 0;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_MAXLEN_SRVHOST, CAPA_HOSTNAME_LEN);

    coms = cJSON_CreateArray();
    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_MEM_CLEAR(comEntry);
        sal_snmp_community_get(index, &comEntry);
        if (0 != osal_strlen(comEntry.name))
        {
            cJSON_AddItemToArray(coms, cJSON_CreateString(comEntry.name));
            entry++;
        }
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_SNMP_HOST_SECCOM, coms);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#endif

