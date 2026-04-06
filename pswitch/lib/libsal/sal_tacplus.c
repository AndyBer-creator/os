
#include <sys/timeb.h>
#include <libsal/sal_util.h>
#include <libsal/sal_tacplus.h>

int32 sal_tacplus_dftParam_set(sys_tacplus_t tacplusSrv)
{
    uint32 i;
    sys_tacplus_t   tmpTacplusSrv;

    SYS_PARAM_CHK((tacplusSrv.timeout < BOND_TACPLUS_TIMEOUT_MIN
                  || tacplusSrv.timeout > BOND_TACPLUS_TIMEOUT_MAX), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_tacplus_dftParam_set(tacplusSrv));

    for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
    {
        SYS_ERR_CHK(cfg_tacplus_server_get(i, &tmpTacplusSrv));
        if (TACPLUS_NOT_UESD_FLAG == tmpTacplusSrv.priority)
            continue;

        if (tmpTacplusSrv.keyUseDft || tmpTacplusSrv.timeoutUseDft)
        {
            if (tmpTacplusSrv.keyUseDft)
                osal_strcpy(tmpTacplusSrv.key, tacplusSrv.key);
            if (tmpTacplusSrv.timeoutUseDft)
                tmpTacplusSrv.timeout = tacplusSrv.timeout;
            sal_tacplus_server_set(i, tmpTacplusSrv);
        }
    }

    return SYS_ERR_OK;
}

int32 sal_tacplus_dftParam_get(sys_tacplus_t *pTacplusSrv)
{
    SYS_PARAM_CHK((NULL == pTacplusSrv), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_tacplus_dftParam_get(pTacplusSrv));

    return SYS_ERR_OK;
}

int32 sal_tacplus_server_set(int32 index, sys_tacplus_t tacplusSrv)
{
    uint32 i, j, used;
    int32 emptyIdx = -1;
    int32 matchIdx = -1;
    sys_tacplus_t tmpTacplusSrv;

    SYS_PARAM_CHK((index >= 0) && (index >= CAPA_TACPLUS_HOST_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK(((tacplusSrv.priority < TACPLUS_NOT_UESD_FLAG)
                  || (tacplusSrv.priority > BOND_TACPLUS_PRI_MAX))
                  && (tacplusSrv.priority != SYS_AAA_TACPLUS_SRV_PRIO_AUTO_SEL), SYS_ERR_INPUT);
    SYS_PARAM_CHK(((tacplusSrv.timeout < BOND_TACPLUS_TIMEOUT_MIN)
                  || (tacplusSrv.timeout > BOND_TACPLUS_TIMEOUT_MAX)), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_TCPUDP_PORT_INVALID(tacplusSrv.srvPort), SYS_ERR_TCPUDP_PORT);

    if (-1 == index)
    {
        for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
        {
            osal_memset(&tmpTacplusSrv, 0, sizeof(tmpTacplusSrv));

            SYS_ERR_CHK(cfg_tacplus_server_get(i, &tmpTacplusSrv));

            if (0 == osal_strcmp(tacplusSrv.srvHost, tmpTacplusSrv.srvHost))
            {
                matchIdx = i;
                if (SYS_AAA_TACPLUS_SRV_PRIO_AUTO_SEL == tacplusSrv.priority)
                    tacplusSrv.priority = tmpTacplusSrv.priority;

                break;
            }
            
            else if (TACPLUS_NOT_UESD_FLAG == tmpTacplusSrv.priority)
            {
                if (-1 == emptyIdx)
                    emptyIdx = i;
            }
        }

        if (-1 == matchIdx)
        {
            
            if (-1 != emptyIdx)
                matchIdx = emptyIdx;
            
            else
                return SYS_ERR_TACPLUS_SRV_FULL;
        }
    }
    else
    {
        matchIdx = index;
    }

    if (SYS_AAA_TACPLUS_SRV_PRIO_AUTO_SEL == tacplusSrv.priority)
    {
        
        for (j = 1; j <= CAPA_TACPLUS_HOST_NUM; j++)
        {
            used = FALSE;
            for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
            {
                SYS_ERR_CHK(cfg_tacplus_server_get(i, &tmpTacplusSrv));
                if (RADIUS_NOT_UESD_FLAG == tmpTacplusSrv.priority)
                    continue;
                if (j == tmpTacplusSrv.priority)
                {
                    used = TRUE;
                    break;
                }
            }

            if (FALSE == used)
            {
                tacplusSrv.priority = j;
                break;
            }
        }
    }
    else
    {
        
        for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
        {
            SYS_ERR_CHK(cfg_tacplus_server_get(i, &tmpTacplusSrv));
            if (TACPLUS_NOT_UESD_FLAG == tmpTacplusSrv.priority)
                continue;
            if (i == matchIdx)
                continue;

            if (tacplusSrv.priority == tmpTacplusSrv.priority)
                return SYS_ERR_TACPLUS_SRV_PRIORITY_EXIST;
        }
    }

    SYS_ERR_CHK(cfg_tacplus_server_set(matchIdx, tacplusSrv));

    return SYS_ERR_OK;
}

int32 sal_tacplus_server_get(uint32 index, sys_tacplus_t *pTacplusSrv)
{
    SYS_PARAM_CHK((index >= CAPA_TACPLUS_HOST_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pTacplusSrv), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_tacplus_server_get(index, pTacplusSrv));

    return SYS_ERR_OK;
}

int32 sal_tacplus_server_del(uint32 index)
{
    sys_tacplus_t tacplusSrv;
    sys_tacplus_t logTacplusSrv;

    SYS_PARAM_CHK((index >= CAPA_TACPLUS_HOST_NUM), SYS_ERR_INPUT);

    osal_memset(&logTacplusSrv, 0, sizeof(logTacplusSrv));

    SYS_ERR_CHK(cfg_tacplus_server_get(index, &logTacplusSrv));

    if (0 == osal_strlen(logTacplusSrv.srvHost))
        return SYS_ERR_OK;

    osal_memset(&tacplusSrv, 0, sizeof(tacplusSrv));
    tacplusSrv.priority = TACPLUS_NOT_UESD_FLAG;

    SYS_ERR_CHK(cfg_tacplus_server_set(index, tacplusSrv));

    return SYS_ERR_OK;
}

