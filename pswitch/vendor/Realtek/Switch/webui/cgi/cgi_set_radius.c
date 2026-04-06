
#include <include/cgi.h>

static int32 cgi_radius_set(void);
static int32 cgi_radiusAdd_set(void);
static int32 cgi_radiusEdit_set(void);
static int32 cgi_radiusDel_set(void);

const cgi_cmdmap_t cmdSetRadiusTbl[] =
{
    {CGI_CMD_RADIUS,        cgi_radius_set},
    {CGI_CMD_RADIUSADD,     cgi_radiusAdd_set},
    {CGI_CMD_RADIUSEDIT,    cgi_radiusEdit_set},
    {CGI_CMD_RADIUSDEL,     cgi_radiusDel_set},
    {NULL, NULL}
};

static int32 cgi_radius_set(void)
{
    int32                   ret = SYS_ERR_OK;
    char                    *value;
    sys_radius_t            radiusSrv;

    SYS_MEM_CLEAR(radiusSrv);

    sal_radiusSrv_dftParam_get(&radiusSrv);

    if (CGI_IS_PARAM_EXIST(CGI_RADIUS_KEY, value))
        strlcpy(radiusSrv.key, value, sizeof(radiusSrv.key));

    if (CGI_IS_PARAM_EXIST(CGI_RADIUS_RETRY, value))
        radiusSrv.retransmit = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_RADIUS_TIMEOUT, value))
        radiusSrv.timeout = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_radiusSrv_dftParam_set(radiusSrv), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_radiusAdd_set(void)
{
    int32        ret = SYS_ERR_OK;
    char         *value;
    sys_radius_t radiusSrv;

    SYS_MEM_CLEAR(radiusSrv);
    sal_radiusSrv_dftParam_get(&radiusSrv);

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, value))
        strlcpy(radiusSrv.hostName, value, sizeof(radiusSrv.hostName));

    if (CGI_IS_PARAM_EXIST(CGI_RADIUS_PRI, value))
        radiusSrv.priority = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_RADIUS_AUTHPORT, value))
        radiusSrv.srvPort = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_RADIUS_USGTYPE, value))
        radiusSrv.usgType = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_NOT_EXIST(CGI_RADIUS_KEY_USEDFLT,value))
    {
        radiusSrv.keyUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_RADIUS_KEY, value))
            strlcpy(radiusSrv.key, value, sizeof(radiusSrv.key));
    }
    else
    {
        radiusSrv.keyUseDft = TRUE;
    }

    if (CGI_IS_PARAM_NOT_EXIST(CGI_RADIUS_RETRY_USEDFLT,value))
    {
        radiusSrv.retransmitUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_RADIUS_RETRY, value))
            radiusSrv.retransmit = SYS_STR2UINT(value);
    }
    else
    {
        radiusSrv.retransmitUseDft = TRUE;
    }

    if (CGI_IS_PARAM_NOT_EXIST(CGI_RADIUS_TIMEOUT_USEDFLT,value))
    {
        radiusSrv.timeoutUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_RADIUS_TIMEOUT, value))
            radiusSrv.timeout = SYS_STR2UINT(value);
    }
    else
    {
        radiusSrv.timeoutUseDft = TRUE;
    }

    CGI_SET_ERR_HDL(sal_radiusSrv_set(-1, radiusSrv), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_radiusEdit_set(void)
{
    int32        ret = SYS_ERR_OK;
    char         *value;
    sys_radius_t radiusSrv;

    SYS_MEM_CLEAR(radiusSrv);
    sal_radiusSrv_dftParam_get(&radiusSrv);

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, value))
        strlcpy(radiusSrv.hostName, value, sizeof(radiusSrv.hostName));

    if (CGI_IS_PARAM_EXIST(CGI_RADIUS_PRI, value))
        radiusSrv.priority = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_RADIUS_AUTHPORT, value))
        radiusSrv.srvPort = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_RADIUS_USGTYPE, value))
        radiusSrv.usgType = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_NOT_EXIST(CGI_RADIUS_KEY_USEDFLT,value))
    {
        radiusSrv.keyUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_RADIUS_KEY, value))
            strlcpy(radiusSrv.key, value, sizeof(radiusSrv.key));
    }
    else
    {
        radiusSrv.keyUseDft = TRUE;
    }

    if (CGI_IS_PARAM_NOT_EXIST(CGI_RADIUS_RETRY_USEDFLT,value))
    {
        radiusSrv.retransmitUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_RADIUS_RETRY, value))
            radiusSrv.retransmit = SYS_STR2UINT(value);
    }
    else
    {
        radiusSrv.retransmitUseDft = TRUE;
    }

    if (CGI_IS_PARAM_NOT_EXIST(CGI_RADIUS_TIMEOUT_USEDFLT,value))
    {
        radiusSrv.timeoutUseDft = FALSE;

        if (CGI_IS_PARAM_EXIST(CGI_RADIUS_TIMEOUT, value))
            radiusSrv.timeout = SYS_STR2UINT(value);
    }
    else
    {
        radiusSrv.timeoutUseDft = TRUE;
    }

    CGI_SET_ERR_HDL(sal_radiusSrv_set(-1, radiusSrv), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_radiusDel_set(void)
{
    int32        ret = SYS_ERR_OK;
    uint32       i = 0;
    uint32       j = 0;
    uint32       len = 0;
    char         **array = NULL;
    sys_radius_t radiusSrv;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SRVHOST, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            for (j = 0; j < CAPA_RADIUS_HOST_NUM; j++)
            {
                osal_memset(&radiusSrv, 0, sizeof(radiusSrv));
                CGI_SET_ERR_HDL(sal_radiusSrv_get(j, &radiusSrv), ret);
                if (RADIUS_NOT_UESD_FLAG == radiusSrv.priority)
                    continue;

                if (0 == osal_strcmp(array[i], radiusSrv.hostName))
                {
                    CGI_SET_ENTRY_ERR_CONTINUE(sal_radiusSrv_del(j), radiusSrv.hostName);
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

