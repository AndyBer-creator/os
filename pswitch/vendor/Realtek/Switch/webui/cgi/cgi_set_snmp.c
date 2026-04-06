
#include <include/cgi.h>

const char *cgiSslVal[] =
{
    "None",
    "MD5",
    "SHA",
    "DES",
};

typedef struct cgi_snmp_accGrpIdx_s
{
    uint8   isSecValid[SNMP_SECLEVEL_END];
    uint32 idx;
    uint32 dataBaseIdx[SNMP_SECLEVEL_END];
}cgi_snmp_accGrpIdx_t;

static int32 cgi_snmp_reload(void);
static int32 cgi_snmp_trapEvent_set(void);

static int32 cgi_snmp_communityAdd_set(void);
static int32 cgi_snmp_communityDel_set(void);

static int32 cgi_snmp_notifyAdd_set(void);
static int32 cgi_snmp_notifyDel_set(void);

#ifdef CONFIG_SYS_SNMPV3
static int32 cgi_snmp_communityEdit_set(void);
static int32 cgi_snmp_notifyEdit_set(void);

static int32 cgi_snmp_viewAdd_set(void);
static int32 cgi_snmp_viewDel_set(void);

static int32 cgi_snmp_groupAdd_set(void);
static int32 cgi_snmp_groupEdit_set(void);
static int32 cgi_snmp_groupDel_set(void);

static int32 cgi_snmp_userAdd_set(void);
static int32 cgi_snmp_userEdit_set(void);
static int32 cgi_snmp_userDel_set(void);

static int32 cgi_snmp_notifyAdd_set(void);
static int32 cgi_snmp_notifyEdit_set(void);
static int32 cgi_snmp_notifyDel_set(void);

static int32 cgi_snmp_localEngineId_set(void);
static int32 cgi_snmp_remEngineIdAdd_set(void);
static int32 cgi_snmp_remEngineIdEdit_set(void);
static int32 cgi_snmp_remEngineIdDel_set(void);

#endif

const cgi_cmdmap_t cmdSetSnmpTbl[] =
{
    {CGI_CMD_SNMP_COMMUNITYADD,            cgi_snmp_communityAdd_set},
    {CGI_CMD_SNMP_COMMUNITYDEL,            cgi_snmp_communityDel_set},

    {CGI_CMD_SNMP_TRAPEVENT,            cgi_snmp_trapEvent_set},

    {CGI_CMD_SNMP_NOTIFYADD,            cgi_snmp_notifyAdd_set},
    {CGI_CMD_SNMP_NOTIFYDEL,            cgi_snmp_notifyDel_set},

    #ifdef CONFIG_SYS_SNMPV3
    {CGI_CMD_SNMP_COMMUNITYEDIT,            cgi_snmp_communityEdit_set},
    {CGI_CMD_SNMP_NOTIFYEDIT,            cgi_snmp_notifyEdit_set},

    {CGI_CMD_SNMP_VIEWADD,        cgi_snmp_viewAdd_set},
    {CGI_CMD_SNMP_VIEWDEL,        cgi_snmp_viewDel_set},

    {CGI_CMD_SNMP_GROUPADD,            cgi_snmp_groupAdd_set},
    {CGI_CMD_SNMP_GROUPEDIT,            cgi_snmp_groupEdit_set},
    {CGI_CMD_SNMP_GROUPDEL,            cgi_snmp_groupDel_set},

    {CGI_CMD_SNMP_USERADD,            cgi_snmp_userAdd_set},
    {CGI_CMD_SNMP_USEREDIT,            cgi_snmp_userEdit_set},
    {CGI_CMD_SNMP_USERDEL,            cgi_snmp_userDel_set},

    {CGI_CMD_SNMP_ENGINEID,             cgi_snmp_localEngineId_set},
    {CGI_CMD_SNMP_ENGINEIDADD,            cgi_snmp_remEngineIdAdd_set},
    {CGI_CMD_SNMP_ENGINEIDEDIT,            cgi_snmp_remEngineIdEdit_set},
    {CGI_CMD_SNMP_ENGINEIDDEL,            cgi_snmp_remEngineIdDel_set},

    #endif
    {NULL, NULL}
};

static int32 cgi_snmp_reload(void)
{
    sal_snmp_reload_t reload;
    SYS_ERR_CHK(sal_snmp_config_reload_get(&reload));
    if(reload == SNMP_RELOAD_AUTO)
    {
        SYS_ERR_CHK(sal_snmp_restart());
    }
    return SYS_ERR_OK;
}

static int32 cgi_snmp_trapEvent_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_enable_t authEnable = DISABLED;
    sys_enable_t linkEnable = DISABLED;
    sys_enable_t warmEnable = DISABLED;
    sys_enable_t coldEnable = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_TRAP_AUTHFAIL, value))
        authEnable = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_TRAP_LINKUPDOWN, value))
        linkEnable = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_TRAP_COLDSTART, value))
        coldEnable = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_TRAP_WARMSTART, value))
        warmEnable = ENABLED;

    CGI_SET_ERR_HDL(sal_snmp_trapEnable_set(SNMP_TRAP_CLASS_AUTHENTICATE, authEnable), ret);
    CGI_SET_ERR_HDL(sal_snmp_trapEnable_set(SNMP_TRAP_CLASS_LINKUPDOWN, linkEnable), ret);
    CGI_SET_ERR_HDL(sal_snmp_trapEnable_set(SNMP_TRAP_CLASS_BOOTUP_COOL, coldEnable), ret);
    CGI_SET_ERR_HDL(sal_snmp_trapEnable_set(SNMP_TRAP_CLASS_BOOTUP_WARM, warmEnable), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_SNMPV3
static int32 cgi_snmp_viewAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    char msk[CAPA_SNMP_VIEW_SUBTREE_LEN] ;
    char viewName[CAPA_SNMP_VIEW_NAME_LEN+1];
    uint8 viewType = SNMP_VIEWTYPE_INCLUDE;
    sal_snmp_oid_t subtree;

    SYS_MEM_CLEAR(msk);
    SYS_MEM_CLEAR(viewName);
    SYS_MEM_CLEAR(subtree);
    subtree.len = 0;

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_VIEW_NAME, value))
    {
        if (SYS_ERR_OK != SNMP_STRING_VALIDATE(value, FALSE))
           CGI_SET_ERR_HDL(SYS_ERR_SNMP_STR, ret);

        if (0 == osal_strcmp(DFLT_SNMP_VIEW_STR,value))
            CGI_SET_ERR_HDL(SYS_ERR_OID_DFLT, ret);

        if (0 == osal_strcmp(value, "none"))
            CGI_SET_ERR_HDL(SYS_ERR_SNMP_VIEW_NAME, ret);

        strlcpy(viewName, value, sizeof(viewName));

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_VIEW_SUBTREE, value))
        {
           CGI_SET_ERR_HDL(sal_util_str2oid(value, &subtree.len, subtree.id), ret);
        }

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_VIEW_TYPE, value))
        {
            if (1 == SYS_STR2UINT(value))
                viewType = SNMP_VIEWTYPE_INCLUDE;
            else
                viewType = SNMP_VIEWTYPE_EXCLUDE;
        }

        CGI_SET_ERR_HDL(sal_snmp_view_add(viewName, &subtree, viewType,msk), ret);
        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_viewDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;
    sal_snmp_view_t viewEntry;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SNMP_VIEW_INDEX, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(viewEntry);
            sal_snmp_view_get(SYS_STR2UINT(array[i]), &viewEntry);
            CGI_SET_ENTRY_ERR_CONTINUE(sal_snmp_viewByIdx_del(SYS_STR2UINT(array[i])), viewEntry.viewName);
        }
        cgi_snmp_reload();
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

static int32 cgi_snmp_groupAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sal_snmp_access_t accessGrp;

    SYS_MEM_CLEAR(accessGrp);
    if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_NAME, value))
    {
        if (SYS_ERR_OK != SNMP_STRING_VALIDATE(value, FALSE))
            CGI_SET_ERR_HDL(SYS_ERR_SNMP_STR, ret);

        if (0 == osal_memcmp(value, SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
            CGI_SET_ERR_HDL(SYS_ERR_SNMP_ACCESS_GROUP_NAME, ret);

        osal_memcpy(accessGrp.groupName, value, osal_strlen(value));
        if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_SECMODE, value))
        {
            switch(value[1])
            {
                case '1': 
                    accessGrp.secMode = SNMP_SECMODEL_V1;
                break;

                case '2': 
                    accessGrp.secMode = SNMP_SECMODEL_V2C;
                break;

                case '3' : 
                    accessGrp.secMode = SNMP_SECMODEL_USM;
                break;

                default:
                    CGI_SET_ERR_HDL(SYS_ERR_INPUT, ret);
                    break;
            }

            accessGrp.prefix = SNMP_PREFIX_EXACT;
            accessGrp.groupcfMode = SNMP_CONFIG_MANUAL;
            accessGrp.viewcfMode = SNMP_CONFIG_MANUAL;

            if (SNMP_SECMODEL_USM != accessGrp.secMode)
            {
                accessGrp.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
            }
            else
            {
                if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_SECLEVEL, value))
                {
                    accessGrp.secLevel = SYS_STR2UINT(value);
                }
            }

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_VIEW_READ, value))
            {
                strlcpy(accessGrp.readView, value, sizeof(accessGrp.readView));
            }
            else
            {
                CGI_SET_ERR_HDL(SYS_ERR_READ_VIEW_MUST_EXIST, ret);
            }

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_VIEW_WRITE, value))
            {
                strlcpy(accessGrp.writeView, value, sizeof(accessGrp.writeView));
            }

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_VIEW_NOTIFY, value))
            {
                strlcpy(accessGrp.notifyView, value, sizeof(accessGrp.notifyView));
            }

            CGI_SET_ERR_HDL(sal_snmp_access_add(&accessGrp), ret);
            cgi_snmp_reload();
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_groupEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 index = 0, setIdx = 0;
    sal_snmp_access_t accessGrp;
    sal_snmp_access_t tmpAccGrp;

    SYS_MEM_CLEAR(accessGrp);
    SYS_MEM_CLEAR(tmpAccGrp);

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_INDEX, value))
    {
        setIdx = SYS_STR2UINT(value);
        CGI_SET_ERR_HDL(sal_snmp_access_get(setIdx, &tmpAccGrp), ret);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_SECMODE, value))
    {
        switch(value[1])
        {
            case '1': 
                accessGrp.secMode = SNMP_SECMODEL_V1;
            break;

            case '2': 
                accessGrp.secMode = SNMP_SECMODEL_V2C;
            break;

            case '3' : 
                accessGrp.secMode = SNMP_SECMODEL_USM;
            break;

            default:
                CGI_SET_ERR_HDL(SYS_ERR_INPUT, ret);
                break;
        }

        accessGrp.prefix = SNMP_PREFIX_EXACT;
        accessGrp.groupcfMode = SNMP_CONFIG_MANUAL;
        accessGrp.viewcfMode = SNMP_CONFIG_MANUAL;
        strlcpy(accessGrp.groupName, tmpAccGrp.groupName, sizeof(accessGrp.groupName));

        if (SNMP_SECMODEL_USM != accessGrp.secMode)
        {
            accessGrp.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
        }
        else
        {
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_SECLEVEL, value))
            {
                accessGrp.secLevel = SYS_STR2UINT(value);
            }
        }

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_VIEW_READ, value))
        {
            strlcpy(accessGrp.readView, value, sizeof(accessGrp.readView));
        }
        else
        {
            CGI_SET_ERR_HDL(SYS_ERR_READ_VIEW_MUST_EXIST, ret);
        }

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_VIEW_WRITE, value))
            strlcpy(accessGrp.writeView, value, sizeof(accessGrp.writeView));

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_GROUP_VIEW_NOTIFY, value))
            strlcpy(accessGrp.notifyView, value, sizeof(accessGrp.notifyView));

        if ( tmpAccGrp.secMode != accessGrp.secMode || tmpAccGrp.secLevel != accessGrp.secLevel)
        {
            sal_snmp_accessIndex_get(accessGrp.groupName, accessGrp.context,accessGrp.secMode, accessGrp.secLevel, &index);
            if (SYS_SNMP_ACCESS_NUM_MAX != index)
              CGI_SET_ERR_HDL(SYS_ERR_DUPLICATE_ENTRY, ret);
        }

        CGI_SET_ERR_HDL(sal_snmp_access_set(setIdx, &accessGrp), ret);
        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_groupDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;
    uint32 index = 0;
    char desStr[SYS_BUF64_LEN];
    sal_snmp_access_t groupEntry;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SNMP_INDEX, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            index = SYS_STR2UINT(array[i]);
            SYS_MEM_CLEAR(groupEntry);
            sal_snmp_access_get(index,&groupEntry);
            osal_sprintf(desStr,"Group %s, Model:%s ", groupEntry.groupName, gsecModel[groupEntry.secMode]);
            CGI_SET_ENTRY_ERR_CONTINUE(sal_snmp_accessByIdx_del(index), desStr);
        }
        cgi_snmp_reload();
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

static int32 cgi_snmp_communityAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 index;
    uint8 isBasic = FALSE;
    uint8 Access = SNMP_COMMUNITY_ACCESS_READ_WRITE;
    uint8 access_v1 = 0;
    uint8 access_v2 = 0;

    char groupName[CAPA_SNMP_GROUP_NAME_LEN +1] ;

    sal_snmp_access_t accGrp;
    sal_snmp_com2sec_t comEntry;

    SYS_MEM_CLEAR(accGrp);
    SYS_MEM_CLEAR(groupName);
    SYS_MEM_CLEAR(comEntry);

    strlcpy(comEntry.sourceNet, "default", sizeof(comEntry.sourceNet));

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_NAME, value))
    {
         if (SYS_ERR_OK != SNMP_STRING_VALIDATE(value, FALSE))
            CGI_SET_ERR_HDL(SYS_ERR_SNMP_STR, ret);

        strlcpy(comEntry.secName, value, sizeof(comEntry.secName));
        strlcpy(comEntry.comName, value, sizeof(comEntry.comName));

        CGI_SET_ERR_HDL(sal_snmp_com2secIndex_get(value, &index), ret);
        if (index < CAPA_SNMP_COMMUNITY_NUM)
            CGI_SET_ERR_HDL(SYS_ERR_DUPLICATE_ENTRY, ret);

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_TYPE, value))
        {
            if (0 == osal_memcmp(value, "Basic", sizeof("Basic")))
                isBasic = TRUE;
        }

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_ACCESS, value))
        {
            if (0 == osal_strcmp(value, CGI_SNMP_ACCESS_RO))
                Access = SNMP_COMMUNITY_ACCESS_READ_ONLY;
        }

        accGrp.context[0] = '\0';
        accGrp.prefix = SNMP_PREFIX_EXACT;

        comEntry.access = Access;
        if (isBasic)
            comEntry.mode = SNMP_COM_MODE_BASIC;
        else
            comEntry.mode = SNMP_COM_MODE_ADV;

        if (isBasic)
        {
            osal_sprintf(groupName, "%s%s", SNMP_ACCESS_GROUP_PREFIX, comEntry.secName);
            strlcpy(accGrp.groupName, groupName, sizeof(accGrp.groupName));
            accGrp.secMode = SNMP_SECMODEL_V1;
            accGrp.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
            accGrp.viewcfMode = SNMP_CONFIG_AUTO;
            accGrp.groupcfMode = SNMP_CONFIG_AUTO;
            strlcpy(comEntry.groupName, groupName, sizeof(comEntry.groupName));

            accGrp.viewcfMode = SNMP_CONFIG_MANUAL;
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_VIEW, value))
            {
                strlcpy(accGrp.readView, value, sizeof(accGrp.readView));
                strlcpy(comEntry.viewName, value, sizeof(comEntry.viewName));
                if (SNMP_COMMUNITY_ACCESS_READ_WRITE == Access)
                {
                    strlcpy(accGrp.writeView, value, sizeof(accGrp.writeView));
                }
            }

            CGI_SET_ERR_HDL(sal_snmp_basic_accessAdd_test(&accGrp), ret);
            
            CGI_SET_ERR_HDL(sal_snmp_com2sec_add(&comEntry), ret);

            CGI_SET_ERR_HDL(sal_snmp_group_add(comEntry.secName, SNMP_SECMODEL_V1, groupName), ret);
            CGI_SET_ERR_HDL(sal_snmp_group_add(comEntry.secName, SNMP_SECMODEL_V2C, groupName), ret);

            CGI_SET_ERR_HDL(sal_snmp_access_add(&accGrp), ret);
            accGrp.secMode = SNMP_SECMODEL_V2C; 
            CGI_SET_ERR_HDL(sal_snmp_access_add(&accGrp), ret);
        }
        else
        {
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_GROUP, value))
            {
                strlcpy(accGrp.groupName, value, sizeof(accGrp.groupName));
                strlcpy(comEntry.groupName, value, sizeof(comEntry.groupName));

                CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(value, "", SNMP_SECMODEL_V1, SNMP_SECLEVEL_NOAUTHNOPRIV, &index), ret);
                if (index < SYS_SNMP_ACCESS_NUM_MAX)
                    access_v1 = 1;

                CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(value, "", SNMP_SECMODEL_V2C, SNMP_SECLEVEL_NOAUTHNOPRIV, &index), ret);
                    access_v2 = 1;

                if (0 == access_v1 && 0 == access_v2)
                    CGI_SET_ERR_HDL(SYS_ERR_GROUP_NOT_EXIST, ret);

                CGI_SET_ERR_HDL(sal_snmp_com2sec_add(&comEntry), ret);
                CGI_SET_ERR_HDL(sal_snmp_group_add(comEntry.secName, SNMP_SECMODEL_V1, value), ret);
                CGI_SET_ERR_HDL(sal_snmp_group_add(comEntry.secName, SNMP_SECMODEL_V2C, value), ret);
            }
            else
                CGI_SET_ERR_HDL(CGI_ERR_SNMP_COMMUNITY_NO_GROUP, ret);
        }

        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_communityEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    char comStr[CAPA_SNMP_COMMUNITY_LEN + 1];
    char sourceNet[] = "default";
    char groupName[CAPA_SNMP_GROUP_NAME_LEN +1] ;

    sal_snmp_access_t accGrp;
    sal_snmp_com2sec_t comEntry, tmpCom;

    uint8 isBasic = FALSE;
    uint32 index = 0, comIdx;
    uint8 Access = SNMP_COMMUNITY_ACCESS_READ_ONLY;
    uint8 access_v1 = 0;
    uint8 access_v2 = 0;

    SYS_MEM_CLEAR(comStr);
    SYS_MEM_CLEAR(accGrp);
    SYS_MEM_CLEAR(comEntry);
    SYS_MEM_CLEAR(tmpCom);

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_NAME, value))
    {
        strlcpy(comStr, value, sizeof(comStr));

        CGI_SET_ERR_HDL(sal_snmp_com2secIndex_get(comStr, &comIdx), ret);
        if (CAPA_SNMP_COMMUNITY_NUM == comIdx)
            CGI_SET_ERR_HDL( SYS_ERR_ENTRY_NOT_FOUND, ret);

        CGI_SET_ERR_HDL(sal_snmp_com2sec_get(comIdx, &tmpCom), ret);
        if (SNMP_COM_MODE_BASIC == tmpCom.mode)
            isBasic = TRUE;

        CGI_SET_ERR_HDL(sal_snmp_groupIndex_get(comStr,SNMP_SECMODEL_V1, &index), ret);
        if (index < SYS_SNMP_SECGROUP_NUM_MAX)
        {
            CGI_SET_ERR_HDL(sal_snmp_group_del(comStr, SNMP_SECMODEL_V1), ret);
        }

        CGI_SET_ERR_HDL(sal_snmp_groupIndex_get(comStr,SNMP_SECMODEL_V2C, &index), ret);
        if (index < SYS_SNMP_SECGROUP_NUM_MAX)
        {
            CGI_SET_ERR_HDL(sal_snmp_group_del(comStr, SNMP_SECMODEL_V2C), ret);
        }

        if (isBasic)
        {
            CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(tmpCom.groupName, "", SNMP_SECMODEL_V1, SNMP_SECLEVEL_NOAUTHNOPRIV, &index), ret);
            if (index < SYS_SNMP_ACCESS_NUM_MAX)
            {
                CGI_SET_ERR_HDL(sal_snmp_access_get(index, &accGrp), ret);
                if (0 == osal_memcmp(accGrp.groupName, SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
                {
                    
                    CGI_SET_ERR_HDL(sal_snmp_access_del(tmpCom.groupName, "", accGrp.secMode, SNMP_SECLEVEL_NOAUTHNOPRIV), ret);
                }
            }

            CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(tmpCom.groupName, "", SNMP_SECMODEL_V2C, SNMP_SECLEVEL_NOAUTHNOPRIV, &index), ret);
            if (index < SYS_SNMP_ACCESS_NUM_MAX)
            {
                CGI_SET_ERR_HDL(sal_snmp_access_get(index, &accGrp), ret);
                 if (0 == osal_memcmp(accGrp.groupName, SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
                {
                    
                    CGI_SET_ERR_HDL(sal_snmp_access_del(tmpCom.groupName, "", accGrp.secMode, SNMP_SECLEVEL_NOAUTHNOPRIV), ret);
                }
            }
        }

        comEntry.mode = SNMP_COM_MODE_ADV;
        if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_TYPE, value))
        {
            if (0 == osal_strcmp(value, "Basic"))
            {
                comEntry.mode = SNMP_COM_MODE_BASIC;
            }
        }

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_ACCESS, value))
        {
            if (0 == osal_strcmp(value, CGI_SNMP_ACCESS_RW))
                Access = SNMP_COMMUNITY_ACCESS_READ_WRITE;
        }

        comEntry.access = Access;

        SYS_MEM_CLEAR(accGrp);
        accGrp.context[0] = '\0';
        accGrp.prefix = SNMP_PREFIX_EXACT;

        if (SNMP_COM_MODE_BASIC == comEntry.mode)
        {
            osal_sprintf(groupName, "%s%s", SNMP_ACCESS_GROUP_PREFIX, comStr);
            strlcpy(accGrp.groupName, groupName, sizeof(accGrp.groupName));
            strlcpy(comEntry.groupName, groupName, sizeof(comEntry.groupName));
            accGrp.secMode = SNMP_SECMODEL_V1;
            accGrp.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
            accGrp.viewcfMode = SNMP_CONFIG_AUTO;
            accGrp.groupcfMode = SNMP_CONFIG_AUTO;

            accGrp.viewcfMode = SNMP_CONFIG_MANUAL;
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_VIEW, value))
            {
                strlcpy(accGrp.readView, value, sizeof(accGrp.readView));
                strlcpy(comEntry.viewName, value, sizeof(comEntry.viewName));
                if (SNMP_COMMUNITY_ACCESS_READ_WRITE == Access)
                {
                    strlcpy(accGrp.writeView, value, sizeof(accGrp.writeView));
                }
            }

            CGI_SET_ERR_HDL(sal_snmp_basic_accessAdd_test(&accGrp), ret);

            CGI_SET_ERR_HDL(sal_snmp_group_add(comStr, SNMP_SECMODEL_V1, groupName), ret);
            CGI_SET_ERR_HDL(sal_snmp_group_add(comStr, SNMP_SECMODEL_V2C, groupName), ret);

            CGI_SET_ERR_HDL(sal_snmp_access_add(&accGrp), ret);
            accGrp.secMode = SNMP_SECMODEL_V2C; 
            CGI_SET_ERR_HDL(sal_snmp_access_add(&accGrp), ret);

            strlcpy(comEntry.secName, comStr, sizeof(comEntry.secName));
            strlcpy(comEntry.comName, comStr, sizeof(comEntry.comName));
            strlcpy(comEntry.sourceNet, sourceNet, sizeof(comEntry.sourceNet));
            CGI_SET_ERR_HDL(sal_snmp_com2sec_set(comIdx, &comEntry), ret);
        }
        else
        {
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_GROUP, value))
            {
                CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(value, "", SNMP_SECMODEL_V1, SNMP_SECLEVEL_NOAUTHNOPRIV, &index), ret);
                if (index < SYS_SNMP_ACCESS_NUM_MAX)
                    access_v1 = 1;

                CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(value, "", SNMP_SECMODEL_V2C, SNMP_SECLEVEL_NOAUTHNOPRIV, &index), ret);
                    access_v2 = 1;

                if (0 == access_v1 && 0 == access_v2)
                    CGI_SET_ERR_HDL(SYS_ERR_GROUP_NOT_EXIST, ret);

                strlcpy(comEntry.secName, comStr, sizeof(comEntry.secName));
                strlcpy(comEntry.comName, comStr, sizeof(comEntry.comName));
                strlcpy(comEntry.sourceNet, sourceNet, sizeof(comEntry.sourceNet));
                strlcpy(comEntry.groupName, value, sizeof(comEntry.groupName));
                CGI_SET_ERR_HDL(sal_snmp_com2sec_set(comIdx, &comEntry), ret);
                CGI_SET_ERR_HDL(sal_snmp_group_add(comStr, SNMP_SECMODEL_V1, value), ret);
                CGI_SET_ERR_HDL(sal_snmp_group_add(comStr, SNMP_SECMODEL_V2C, value), ret);
            }
            else
                CGI_SET_ERR_HDL(CGI_ERR_SNMP_COMMUNITY_NO_GROUP, ret);
        }

        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_communityDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;
    uint32 index;

    sal_snmp_access_t access;
    sal_snmp_com2sec_t comEntry;
    sal_snmp_group_t  group;
    char groupName[CAPA_SNMP_GROUP_NAME_LEN+1];
    uint8 isBasic = FALSE;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SNMP_INDEX, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            CGI_SET_ERR_HDL(sal_snmp_com2secIndex_get(array[i], &index), ret);
            CGI_SET_ERR_HDL(sal_snmp_com2sec_get(index, &comEntry), ret);
            CGI_SET_ENTRY_ERR_CONTINUE(sal_snmp_com2sec_del(array[i]), comEntry.comName);
            if (SNMP_COM_MODE_BASIC == comEntry.mode)
                    isBasic = TRUE;

            SYS_MEM_CLEAR(groupName);
            
            CGI_SET_ERR_HDL(sal_snmp_groupIndex_get(array[i],SNMP_SECMODEL_V1, &index), ret);
            if (index < SYS_SNMP_SECGROUP_NUM_MAX)
            {
                CGI_SET_ERR_HDL(sal_snmp_group_get(index, &group), ret);
                strlcpy(groupName,group.groupName, sizeof(groupName));
                CGI_SET_ERR_HDL(sal_snmp_group_del(array[i], SNMP_SECMODEL_V1), ret);
            }

            CGI_SET_ERR_HDL(sal_snmp_groupIndex_get(array[i],SNMP_SECMODEL_V2C, &index), ret);
            if (index < SYS_SNMP_SECGROUP_NUM_MAX)
            {
                CGI_SET_ERR_HDL(sal_snmp_group_get(index, &group), ret);
                strlcpy(groupName,group.groupName, sizeof(groupName));
                CGI_SET_ERR_HDL(sal_snmp_group_del(array[i], SNMP_SECMODEL_V2C), ret);
            }

            if (isBasic)
            {
                CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(groupName, "", SNMP_SECMODEL_V1, SNMP_SECLEVEL_NOAUTHNOPRIV, &index), ret);
                if (index < SYS_SNMP_ACCESS_NUM_MAX)
                {
                    CGI_SET_ERR_HDL(sal_snmp_access_get(index, &access), ret);
                    if (SNMP_CONFIG_MANUAL != access.groupcfMode)
                    {
                        
                        CGI_SET_ERR_HDL(sal_snmp_access_del(groupName, "", access.secMode, SNMP_SECLEVEL_NOAUTHNOPRIV), ret);
                    }
                }

                CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(groupName, "", SNMP_SECMODEL_V2C, SNMP_SECLEVEL_NOAUTHNOPRIV, &index), ret);
                if (index < SYS_SNMP_ACCESS_NUM_MAX)
                {
                    CGI_SET_ERR_HDL(sal_snmp_access_get(index, &access), ret);
                    if (SNMP_CONFIG_MANUAL != access.groupcfMode)
                    {
                        
                        CGI_SET_ERR_HDL(sal_snmp_access_del(groupName, "", access.secMode, SNMP_SECLEVEL_NOAUTHNOPRIV), ret);
                    }
                }
            }
        }
        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

static int32 cgi_snmp_userAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sal_snmp_user_t userEntry;
    sal_snmp_access_t  access;
    uint32 index = 0;
    sal_snmp_sslType_t sslType;
    uint8 isGrpV3 = 0;
    sal_snmp_seclevel_t secLvl = SNMP_SECLEVEL_NOAUTHNOPRIV;

    SYS_MEM_CLEAR(userEntry);
    if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_NAME, value))
    {
        if (SYS_ERR_OK != SNMP_STRING_VALIDATE(value, FALSE))
            CGI_SET_ERR_HDL(SYS_ERR_SNMP_STR, ret);

        strlcpy(userEntry.userName, value, sizeof(userEntry.userName));
        userEntry.selLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
        userEntry.encrypt_proto = SNMP_SSL_TYPE_NONE;

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_GROUP_NAME, value))
        {
            osal_memcpy(userEntry.groupName, value, (osal_strlen(value)-2));
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_AUTH_METHOD, value))
            {
                for (sslType = SNMP_SSLTYPE_NONE; sslType <= SNMP_SSLTYPE_SHA; sslType++)
                {
                    if (0 == osal_strcmp(cgiSslVal[sslType], value))
                    {
                        userEntry.auth_protp = sslType;
                        break;
                    }
                }
            }

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_SECURITY_LEVEL, value))
            {
                if (0 == osal_strcmp(value, "NoAuth"))
                    secLvl = SNMP_SECLEVEL_NOAUTHNOPRIV;
                else if (0 == osal_strcmp(value, "AuthNoPriv"))
                    secLvl = SNMP_SECLEVEL_AUTHNOPRIV;
                else
                    secLvl = SNMP_SECLEVEL_AUTHPRIV;
            }

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_AUTHPSWD, value))
            {
                if (osal_strlen(value) < BOND_SNMP_AUTH_KEY_LEN_MIN)
                    CGI_SET_ERR_HDL(CGI_ERR_SNMP_AUTHPASSWD_LENGTH_MIN, ret);

                userEntry.selLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                strlcpy(userEntry.authKey, value, sizeof(userEntry.authKey));
            }
            else
            {
                if (secLvl > SNMP_SECLEVEL_NOAUTHNOPRIV)
                {
                    ret = CGI_ERR_SNMP_AUTHPASSWD_LENGTH_MIN;
                    goto cgi_end;
                }
            }

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_PRIVPSWD, value))
            {
                if (osal_strlen(value) < BOND_SNMP_PRIV_KEY_LEN_MIN)
                    CGI_SET_ERR_HDL(CGI_ERR_SNMP_PRIVPASSWD_LENGTH_MIN, ret);

                userEntry.selLevel = SNMP_SECLEVEL_AUTHPRIV;
                userEntry.encrypt_proto = SNMP_SSLTYPE_DES;
                strlcpy(userEntry.privKey, value, sizeof(userEntry.privKey));
            }
            else
            {
                if (secLvl > SNMP_SECLEVEL_AUTHNOPRIV)
                {
                    ret = CGI_ERR_SNMP_PRIVPASSWD_LENGTH_MIN;
                    goto cgi_end;
                }
            }

            CGI_SET_ERR_HDL(sal_snmp_groupIndex_get(userEntry.userName, SNMP_SECMODEL_USM, &index), ret);
            if (index < SYS_SNMP_SECGROUP_NUM_MAX)
                isGrpV3 = 1;

            CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(userEntry.groupName, "", SNMP_SECMODEL_USM, userEntry.selLevel, &index), ret);
            if (SYS_SNMP_ACCESS_NUM_MAX == index)
                return SYS_ERR_GROUP_NOT_EXIST;

            CGI_SET_ERR_HDL(sal_snmp_access_get(index, &access), ret);
            if (SNMP_CONFIG_MANUAL != access.groupcfMode)
                return SYS_ERR_GROUP_NOT_EXIST;

            if (0 != osal_strlen(access.writeView))
                userEntry.access = SNMP_COMMUNITY_ACCESS_READ_WRITE;
            else if (0 != osal_strlen(access.readView))
                userEntry.access = SNMP_COMMUNITY_ACCESS_READ_ONLY;

            CGI_SET_ERR_HDL(sal_snmp_user_add(&userEntry), ret);
            
            if (0 == isGrpV3)
                CGI_SET_ERR_HDL(sal_snmp_group_add(userEntry.userName, SNMP_SECMODEL_USM, userEntry.groupName), ret);

            cgi_snmp_reload();
        }
        else
            CGI_SET_ERR_HDL(CGI_ERR_SNMP_USER_NO_GROUP, ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_userEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sal_snmp_user_t userEntry;
    sal_snmp_access_t  access;
    uint32 index = 0, userIdx;
    sal_snmp_sslType_t sslType;
    sal_snmp_seclevel_t secLvl = SNMP_SECLEVEL_NOAUTHNOPRIV;

    SYS_MEM_CLEAR(userEntry);
    SYS_MEM_CLEAR(access);

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_NAME, value))
    {
        strlcpy(userEntry.userName, value, sizeof(userEntry.userName));
        CGI_SET_ERR_HDL(sal_snmp_userIndex_get(userEntry.userName, &userIdx), ret);
        if (CAPA_SNMP_USER_NUM == userIdx)
            CGI_SET_ERR_HDL( SYS_ERR_ENTRY_NOT_FOUND, ret);

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_GROUP_NAME, value))
        {
            osal_memcpy(userEntry.groupName, value, (osal_strlen(value)-2));
            CGI_SET_ERR_HDL(sal_snmp_groupIndex_get(userEntry.userName,SNMP_SECMODEL_USM, &index), ret);
            if (index < SYS_SNMP_SECGROUP_NUM_MAX)
            {
                CGI_SET_ERR_HDL(sal_snmp_group_del(userEntry.userName, SNMP_SECMODEL_USM), ret);
            }

            userEntry.selLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
            userEntry.encrypt_proto = SNMP_SSL_TYPE_NONE;

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_AUTH_METHOD, value))
            {
                for (sslType = SNMP_SSLTYPE_NONE; sslType <= SNMP_SSLTYPE_SHA; sslType++)
                {
                    if (0 == osal_strcmp(cgiSslVal[sslType], value))
                    {
                        userEntry.auth_protp = sslType;
                        break;
                    }
                }
            }

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_SECURITY_LEVEL, value))
            {
                if (0 == osal_strcmp(value, "NoAuth"))
                    secLvl = SNMP_SECLEVEL_NOAUTHNOPRIV;
                else if (0 == osal_strcmp(value, "AuthNoPriv"))
                    secLvl = SNMP_SECLEVEL_AUTHNOPRIV;
                else
                    secLvl = SNMP_SECLEVEL_AUTHPRIV;
            }

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_AUTHPSWD, value))
            {
                if (osal_strlen(value) < BOND_SNMP_AUTH_KEY_LEN_MIN)
                    CGI_SET_ERR_HDL(CGI_ERR_SNMP_AUTHPASSWD_LENGTH_MIN, ret);

                userEntry.selLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                strlcpy(userEntry.authKey, value, sizeof(userEntry.authKey));
            }
            else
            {
                if (secLvl > SNMP_SECLEVEL_NOAUTHNOPRIV)
                {
                    ret = CGI_ERR_SNMP_AUTHPASSWD_LENGTH_MIN;
                    goto cgi_end;
                }
            }

            if (CGI_IS_PARAM_EXIST(CGI_SNMP_USER_PRIVPSWD, value))
            {
                if (osal_strlen(value) < BOND_SNMP_PRIV_KEY_LEN_MIN)
                    CGI_SET_ERR_HDL(CGI_ERR_SNMP_PRIVPASSWD_LENGTH_MIN, ret);

                userEntry.selLevel = SNMP_SECLEVEL_AUTHPRIV;
                userEntry.encrypt_proto = SNMP_SSLTYPE_DES;
                strlcpy(userEntry.privKey, value, sizeof(userEntry.privKey));
            }
            else
            {
                if (secLvl > SNMP_SECLEVEL_AUTHNOPRIV)
                {
                    ret = CGI_ERR_SNMP_PRIVPASSWD_LENGTH_MIN;
                    goto cgi_end;
                }
            }

            CGI_SET_ERR_HDL(sal_snmp_accessIndex_get(userEntry.groupName, "", SNMP_SECMODEL_USM, userEntry.selLevel, &index), ret);
            if (SYS_SNMP_ACCESS_NUM_MAX == index)
                CGI_SET_ERR_HDL(SYS_ERR_GROUP_NOT_EXIST, ret);

             if (0 != osal_strlen(access.writeView))
                userEntry.access = SNMP_COMMUNITY_ACCESS_READ_WRITE;
            else if (0 != osal_strlen(access.readView))
                userEntry.access = SNMP_COMMUNITY_ACCESS_READ_ONLY;

            CGI_SET_ERR_HDL(sal_snmp_user_set(userIdx, &userEntry), ret);

            CGI_SET_ERR_HDL(sal_snmp_group_add(userEntry.userName, SNMP_SECMODEL_USM, userEntry.groupName), ret);

            cgi_snmp_reload();
        }
        else
             CGI_SET_ERR_HDL(CGI_ERR_SNMP_USER_NO_GROUP, ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_userDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;
    uint32 index;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SNMP_INDEX, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            CGI_SET_ENTRY_ERR_CONTINUE(sal_snmp_user_del(array[i]), array[i]);
            CGI_SET_ERR_HDL(sal_snmp_groupIndex_get(array[i],SNMP_SECMODEL_USM, &index), ret);
            if (index < SYS_SNMP_SECGROUP_NUM_MAX)
            {
                CGI_SET_ERR_HDL(sal_snmp_group_del(array[i], SNMP_SECMODEL_USM), ret);
            }
        }

        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

static int32 cgi_snmp_notifyAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint8 isTrap = TRUE;

    sal_snmp_target_t  hostEntry;

    SYS_MEM_CLEAR(hostEntry);

    hostEntry.secModel = SNMP_SECMODEL_V1;
    hostEntry.trapType = SNMP_TRAP_V1;
    hostEntry.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
    hostEntry.portNum = DFLT_SNMP_NOTIFACTION_PORT;
    hostEntry.retries = DFLT_SNMP_INFORM_RETRY;
    hostEntry.timeout = DFLT_SNMP_INFORM_TIMEOUT;

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, value))
    {
        strlcpy(hostEntry.server, value, sizeof(hostEntry.server));

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_UDPPORT, value))
            hostEntry.portNum = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_RETRIES, value))
            hostEntry.retries = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_TIMEOUT, value))
            hostEntry.timeout = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_VERSION, value))
            hostEntry.secModel = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_TYPE, value))
        {
            if (2 == SYS_STR2UINT(value))
                isTrap = FALSE;

            if (SNMP_SECMODEL_V1 == hostEntry.secModel)
                hostEntry.trapType = SNMP_TRAP_V1;
            else if (SNMP_SECMODEL_V2C == hostEntry.secModel)
            {
                if (isTrap)
                    hostEntry.trapType = SNMP_TRAP_V2;
                else
                    hostEntry.trapType = SNMP_INFORM_V2;
            }
            else
            {
                if (isTrap)
                    hostEntry.trapType = SNMP_TRAP_V3;
                else
                    hostEntry.trapType = SNMP_INFORM_V3;
            }
        }

        if (SNMP_SECMODEL_USM == hostEntry.secModel)
        {
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_V3NAME, value))
                strlcpy(hostEntry.name, value, sizeof(hostEntry.name));
            else
                CGI_SET_ERR_HDL(CGI_ERR_SNMP_NOTIFY_NO_USER, ret);

             if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_PRIMODE, value))
             {
                if (0 == osal_strcmp(value, "noAuth"))
                {
                    hostEntry.secLevel  = SNMP_SECLEVEL_NOAUTHNOPRIV;
                }
                else if (0 == osal_strcmp(value, "authNoPriv"))
                {
                    hostEntry.secLevel  = SNMP_SECLEVEL_AUTHNOPRIV;
                }
                else
                    hostEntry.secLevel  = SNMP_SECLEVEL_AUTHPRIV;
             }
        }
        else
        {
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_V1V2NAME, value))
                strlcpy(hostEntry.name, value, sizeof(hostEntry.name));
            else
                CGI_SET_ERR_HDL(CGI_ERR_SNMP_NOTIFY_NO_COMMUNITY, ret);

            hostEntry.secLevel  = SNMP_SECLEVEL_NOAUTHNOPRIV;
        }

        CGI_SET_ERR_HDL(sal_snmp_target_add(&hostEntry), ret);
        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_notifyEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 index = 0, tmpIdx = 0;
    uint8   isTrap = TRUE;

    sal_snmp_target_t  hostEntry, selEntry;

    SYS_MEM_CLEAR(hostEntry);
    SYS_MEM_CLEAR(selEntry);

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_INDEX, value))
    {
        index = SYS_STR2UINT(value);
        CGI_SET_ERR_HDL(sal_snmp_target_get(index, &selEntry), ret);
        if (0 == osal_strlen(selEntry.server))
            goto cgi_end;

        strlcpy(hostEntry.server, selEntry.server, sizeof(hostEntry.server));

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_VERSION, value))
            hostEntry.secModel = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_UDPPORT, value))
            hostEntry.portNum = SYS_STR2UINT(value);
        else
            hostEntry.portNum= DFLT_SNMP_NOTIFACTION_PORT;

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_TYPE, value))
        {
            if (2 == SYS_STR2UINT(value))
                isTrap = FALSE;

            if (SNMP_SECMODEL_V1 == hostEntry.secModel)
                hostEntry.trapType = SNMP_TRAP_V1;
            else if (SNMP_SECMODEL_V2C == hostEntry.secModel)
            {
                if (isTrap)
                    hostEntry.trapType = SNMP_TRAP_V2;
                else
                    hostEntry.trapType = SNMP_INFORM_V2;
            }
            else
            {
                if (isTrap)
                    hostEntry.trapType = SNMP_TRAP_V3;
                else
                    hostEntry.trapType = SNMP_INFORM_V3;
            }
        }

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_TIMEOUT, value))
            hostEntry.timeout = SYS_STR2UINT(value);
        else
            hostEntry.timeout = DFLT_SNMP_INFORM_TIMEOUT;

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_RETRIES, value))
            hostEntry.retries = SYS_STR2UINT(value);
        else
            hostEntry.retries = DFLT_SNMP_INFORM_RETRY;

        if (SNMP_SECMODEL_USM == hostEntry.secModel)
        {
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_V3NAME, value))
                strlcpy(hostEntry.name, value, sizeof(hostEntry.name));
            else
                CGI_SET_ERR_HDL(CGI_ERR_SNMP_NOTIFY_NO_USER, ret);

             if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_PRIMODE, value))
             {
                if (0 == osal_strcmp(value, "noAuth"))
                {
                    hostEntry.secLevel  = SNMP_SECLEVEL_NOAUTHNOPRIV;
                }
                else if (0 == osal_strcmp(value, "authNoPriv"))
                {
                    hostEntry.secLevel  = SNMP_SECLEVEL_AUTHNOPRIV;
                }
                else
                    hostEntry.secLevel  = SNMP_SECLEVEL_AUTHPRIV;
             }
        }
        else
        {
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_V1V2NAME, value))
                strlcpy(hostEntry.name, value, sizeof(hostEntry.name));
            else
                CGI_SET_ERR_HDL(CGI_ERR_SNMP_NOTIFY_NO_COMMUNITY, ret);

            hostEntry.secLevel  = SNMP_SECLEVEL_NOAUTHNOPRIV;
        }

        if ((hostEntry.secModel != selEntry.secModel) ||(hostEntry.trapType != selEntry.trapType))
        {
            CGI_SET_ERR_HDL(sal_snmp_targetIndex_get(hostEntry.server, hostEntry.secModel, hostEntry.trapType, &tmpIdx), ret);
            if (CAPA_SNMP_HOST_NUM != tmpIdx)
                CGI_SET_ERR_HDL(SYS_ERR_DUPLICATE_ENTRY, ret);
        }

        CGI_SET_ERR_HDL(sal_snmp_target_set(index, &hostEntry), ret);
        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_notifyDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;
    sal_snmp_target_t hostEntry;
    char desStr[SYS_BUF128_LEN];

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SNMP_INDEX, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(hostEntry);
            SYS_MEM_CLEAR(desStr);
            sal_snmp_target_get(SYS_STR2UINT(array[i]), &hostEntry);
            osal_printf(desStr, "ip: %s, version: %s, notify type:%s ", hostEntry.server, gtrapVersion[hostEntry.secModel],
            gtrapType[hostEntry.trapType]);
            CGI_SET_ENTRY_ERR_CONTINUE(sal_snmp_targetByIdx_del(SYS_STR2UINT(array[i])), desStr);
        }

        cgi_snmp_reload();
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

 static int32 cgi_snmp_localEngineId_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    char engineStr[BOND_SNMP_ENGINEID_LEN_MAX + 1];

    SYS_MEM_CLEAR(engineStr);
    if (CGI_IS_PARAM_EXIST(CGI_SNMP_ENGINEID_USEDEF_SET, value))
    {
        if (CGI_IS_PARAM_EXIST(CGI_SNMP_ENGINEID, value))
        {
            if (osal_strlen(value) % 2)
                CGI_SET_ERR_HDL(SYS_ERR_ENGINEID_LEN_ERR, ret);

            if (SYS_ERR_OK != sal_util_check_hex(value))
            {
                CGI_SET_ERR_HDL(SYS_ERR_ENGINEID_ERR, ret);
            }
            CGI_SET_ERR_HDL(sal_snmp_engineid_set(value), ret);
        }
    }
    else
    {
        CGI_SET_ERR_HDL(sal_snmp_defaultEngineId_get(engineStr), ret);
        CGI_SET_ERR_HDL(sal_snmp_engineid_set(engineStr),ret);
    }

    cgi_snmp_reload();

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

 static int32 cgi_snmp_remEngineIdAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sal_snmp_remoteEngine_t remoteEngine;

    SYS_MEM_CLEAR(remoteEngine);
    if (CGI_IS_PARAM_EXIST(CGI_SNMP_REMOTE_ENGINEID_IP, value))
    {
        strlcpy(remoteEngine.server, value, sizeof(remoteEngine.server));

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_REMOTE_ENGINEID_NO, value))
        {
             if (osal_strlen(value) % 2)
                    CGI_SET_ERR_HDL(SYS_ERR_ENGINEID_LEN_ERR, ret);

             if (SYS_ERR_OK != sal_util_check_hex(value))
             {
                CGI_SET_ERR_HDL(SYS_ERR_ENGINEID_ERR, ret);
             }
             strlcpy(remoteEngine.engineId, value, sizeof(remoteEngine.engineId));
        }

        CGI_SET_ERR_HDL(sal_snmp_remoteEngine_add(&remoteEngine), ret);
        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

 static int32 cgi_snmp_remEngineIdEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 index = 0;

    sal_snmp_remoteEngine_t remoteEngine;

    SYS_MEM_CLEAR(remoteEngine);
    if (CGI_IS_PARAM_EXIST(CGI_SNMP_REMOTE_ENGINEID_IP, value))
    {
        sal_snmp_remoteEngineIndex_get(value, &index);
        if (CAPA_SNMP_REMOTE_ENGINEID_NUM != index)
        {
            osal_memcpy(remoteEngine.server, value, osal_strlen(value));
            if (CGI_IS_PARAM_EXIST(CGI_SNMP_REMOTE_ENGINEID_NO, value))
            {
                 if (osal_strlen(value) % 2)
                    CGI_SET_ERR_HDL(SYS_ERR_ENGINEID_LEN_ERR, ret);

                if (SYS_ERR_OK != sal_util_check_hex(value))
                {
                    CGI_SET_ERR_HDL(SYS_ERR_ENGINEID_ERR, ret);
                }
                osal_memcpy(remoteEngine.engineId, value, osal_strlen(value));

                CGI_SET_ERR_HDL(sal_snmp_remoteEngine_set(index, &remoteEngine), ret);
                cgi_snmp_reload();
            }
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_remEngineIdDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SNMP_REMOTE_ENGINEID_IP, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            CGI_SET_ENTRY_ERR_CONTINUE(sal_snmp_remoteEngine_del(array[i]), array[i]);
        }
        cgi_snmp_reload();
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}
#else
static int32 cgi_snmp_communityAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 Access =SNMP_COMMUNITY_ACCESS_READ_WRITE;
    char comName[CAPA_SNMP_COMMUNITY_LEN + 1];

    if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_NAME, value))
    {
        SYS_MEM_CLEAR(comName);
        strlcpy(comName, value, sizeof(comName));

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_COM_ACCESS, value))
        {
            if (0 == osal_strcmp(value, CGI_SNMP_ACCESS_RO))
                Access = SNMP_COMMUNITY_ACCESS_READ_ONLY;
        }

        CGI_SET_ERR_HDL(sal_snmp_community_add(comName, Access), ret);
        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_communityDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SNMP_INDEX, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            CGI_SET_ENTRY_ERR_CONTINUE(sal_snmp_community_del(array[i]), array[i]);
        }
        cgi_snmp_reload();
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

static int32 cgi_snmp_notifyAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sal_snmp_target_t  hostEntry;
    uint8 isTrap = TRUE;

    SYS_MEM_CLEAR(hostEntry);

    hostEntry.secModel = SNMP_SECMODEL_V1;
    hostEntry.trapType = SNMP_TRAP_V1;
    hostEntry.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
    hostEntry.portNum = DFLT_SNMP_NOTIFACTION_PORT;
    hostEntry.retries = DFLT_SNMP_INFORM_RETRY;
    hostEntry.timeout = DFLT_SNMP_INFORM_TIMEOUT;

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, value))
    {
        strlcpy(hostEntry.server, value, sizeof(hostEntry.server));

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_VERSION, value))
            hostEntry.secModel = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_TYPE, value))
        {
            if (2 == SYS_STR2UINT(value))
                isTrap = FALSE;

            if (SNMP_SECMODEL_V1 == hostEntry.secModel)
                hostEntry.trapType = SNMP_TRAP_V1;
            else if (SNMP_SECMODEL_V2C == hostEntry.secModel)
            {
                if (isTrap)
                    hostEntry.trapType = SNMP_TRAP_V2;
                else
                    hostEntry.trapType = SNMP_INFORM_V2;
            }
        }

        if (CGI_IS_PARAM_EXIST(CGI_SNMP_HOST_V1V2NAME, value))
            strlcpy(hostEntry.name, value, sizeof(hostEntry.name));
        else
            CGI_SET_ERR_HDL(CGI_ERR_SNMP_NOTIFY_NO_COMMUNITY, ret);

        CGI_SET_ERR_HDL(sal_snmp_target_add(&hostEntry), ret);
        cgi_snmp_reload();
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_snmp_notifyDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;
    sal_snmp_target_t hostEntry;
    char desStr[SYS_BUF128_LEN];

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SNMP_INDEX, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(hostEntry);
            SYS_MEM_CLEAR(desStr);
            sal_snmp_target_get(SYS_STR2UINT(array[i]), &hostEntry);
            osal_printf(desStr, "ip: %s, version: %s, notify type:%s ", hostEntry.server, gtrapVersion[hostEntry.secModel],
            gtrapType[hostEntry.trapType]);
            CGI_SET_ENTRY_ERR_CONTINUE(sal_snmp_targetByIdx_del(SYS_STR2UINT(array[i])), desStr);
        }

        cgi_snmp_reload();
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

#endif

