
#include <include/cgi.h>

static int32 cgi_tacplus_set(void);
static int32 cgi_tacplusAdd_set(void);
static int32 cgi_tacplusEdit_set(void);
static int32 cgi_tacplusDel_set(void);

const cgi_cmdmap_t cmdSetTacplusTbl[] =
{
    {CGI_CMD_TACPLUS,        cgi_tacplus_set},
    {CGI_CMD_TACPLUSADD,     cgi_tacplusAdd_set},
    {CGI_CMD_TACPLUSEDIT,    cgi_tacplusEdit_set},
    {CGI_CMD_TACPLUSDEL,     cgi_tacplusDel_set},
    {NULL, NULL}
};

static int32 cgi_tacplus_set(void)
{
    int32                   ret = SYS_ERR_OK;
    char                    *value;
    sys_tacplus_t            tacplusSrv;

    SYS_MEM_CLEAR(tacplusSrv);

    sal_tacplus_dftParam_get(&tacplusSrv);

    if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_KEY, value))
        strlcpy(tacplusSrv.key, value, sizeof(tacplusSrv.key));

    if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_TIMEOUT, value))
        tacplusSrv.timeout = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_tacplus_dftParam_set(tacplusSrv), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_tacplusAdd_set(void)
{
    int32        ret = SYS_ERR_OK;
    char         *value;
    sys_tacplus_t tacplusSrv;

    SYS_MEM_CLEAR(tacplusSrv);
    sal_tacplus_dftParam_get(&tacplusSrv);

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, value))
        strlcpy(tacplusSrv.srvHost, value, sizeof(tacplusSrv.srvHost));

    if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_PRI, value))
        tacplusSrv.priority = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_AUTHPORT, value))
        tacplusSrv.srvPort = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_NOT_EXIST(CGI_TACPLUS_KEY_USEDFLT,value))
    {
        tacplusSrv.keyUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_KEY, value))
            strlcpy(tacplusSrv.key, value, sizeof(tacplusSrv.key));
    }
    else
    {
        tacplusSrv.keyUseDft = TRUE;
    }

    if (CGI_IS_PARAM_NOT_EXIST(CGI_TACPLUS_TIMEOUT_USEDFLT,value))
    {
        tacplusSrv.timeoutUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_TIMEOUT, value))
            tacplusSrv.timeout = SYS_STR2UINT(value);
    }
    else
    {
        tacplusSrv.timeoutUseDft = TRUE;
    }

    CGI_SET_ERR_HDL(sal_tacplus_server_set(-1, tacplusSrv), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_tacplusEdit_set(void)
{
    int32        ret = SYS_ERR_OK;
    char         *value;
    sys_tacplus_t tacplusSrv;

    SYS_MEM_CLEAR(tacplusSrv);
    sal_tacplus_dftParam_get(&tacplusSrv);

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, value))
        strlcpy(tacplusSrv.srvHost, value, sizeof(tacplusSrv.srvHost));

    if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_PRI, value))
        tacplusSrv.priority = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_AUTHPORT, value))
        tacplusSrv.srvPort = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_NOT_EXIST(CGI_TACPLUS_KEY_USEDFLT,value))
    {
        tacplusSrv.keyUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_KEY, value))
            strlcpy(tacplusSrv.key, value, sizeof(tacplusSrv.key));
    }
    else
    {
        tacplusSrv.keyUseDft = TRUE;
    }

    if (CGI_IS_PARAM_NOT_EXIST(CGI_TACPLUS_TIMEOUT_USEDFLT,value))
    {
        tacplusSrv.timeoutUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_TACPLUS_TIMEOUT, value))
            tacplusSrv.timeout = SYS_STR2UINT(value);
    }
    else
    {
        tacplusSrv.timeoutUseDft = TRUE;
    }

    CGI_SET_ERR_HDL(sal_tacplus_server_set(-1, tacplusSrv), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_tacplusDel_set(void)
{
    int32        ret = SYS_ERR_OK;
    uint32       i = 0;
    uint32       j = 0;
    uint32       len = 0;
    char         **array = NULL;
    sys_tacplus_t tacplusSrv;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SRVHOST, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            for (j = 0; j < CAPA_TACPLUS_HOST_NUM; j++)
            {
                osal_memset(&tacplusSrv, 0, sizeof(tacplusSrv));
                CGI_SET_ERR_HDL(sal_tacplus_server_get(j, &tacplusSrv), ret);
                if (TACPLUS_NOT_UESD_FLAG == tacplusSrv.priority)
                    continue;

                if (0 == osal_strcmp(array[i], tacplusSrv.srvHost))
                {
                    CGI_SET_ENTRY_ERR_CONTINUE(sal_tacplus_server_del(j), tacplusSrv.srvHost);
                }
            }
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

