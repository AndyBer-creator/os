
#include <libsal/sal_util.h>
#include <libsal/sal_radius.h>

int32 sal_radiusSrv_dftParam_set(sys_radius_t radiusSrv)
{
    uint32 i;
    sys_radius_t    tmpRadiusSrv;

    SYS_PARAM_CHK((radiusSrv.retransmit < BOND_RADIUS_RETRY_MIN
                   || radiusSrv.retransmit > BOND_RADIUS_RETRY_MAX), SYS_ERR_INPUT);
    SYS_PARAM_CHK((radiusSrv.timeout < BOND_RADIUS_TIMEOUT_MIN
                   || radiusSrv.timeout > BOND_RADIUS_TIMEOUT_MAX), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_radiusSrv_dftParam_set(radiusSrv));

    for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
    {
        SYS_ERR_CHK(sal_radiusSrv_get(i, &tmpRadiusSrv));
        if (RADIUS_NOT_UESD_FLAG == tmpRadiusSrv.priority)
            continue;

        if (tmpRadiusSrv.keyUseDft || tmpRadiusSrv.timeoutUseDft || tmpRadiusSrv.retransmitUseDft)
        {
            if (tmpRadiusSrv.keyUseDft)
                osal_strcpy(tmpRadiusSrv.key, radiusSrv.key);
            if (tmpRadiusSrv.timeoutUseDft)
                tmpRadiusSrv.timeout = radiusSrv.timeout;
            if (tmpRadiusSrv.retransmitUseDft)
                tmpRadiusSrv.retransmit = radiusSrv.retransmit;
            sal_radiusSrv_set(i, tmpRadiusSrv);
        }
    }

    return SYS_ERR_OK;
}

int32 sal_radiusSrv_dftParam_get(sys_radius_t *pRadiusSrv)
{
    SYS_PARAM_CHK((NULL == pRadiusSrv), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_radiusSrv_dftParam_get(pRadiusSrv));

    return SYS_ERR_OK;
}

int32 sal_radiusSrv_set(int32 index, sys_radius_t radiusSrv)
{
    char ipstr[CAPA_IPV4_STR_LEN];
    int32 emptyIdx = -1;
    int32 matchIdx = -1;
    uint32 i, j, used;
    sys_radius_t tmpRadiusSrv;

    SYS_PARAM_CHK((index >= 0) && (index >= CAPA_RADIUS_HOST_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK(((radiusSrv.priority < BOND_RADIUS_PRI_MIN)
                  || radiusSrv.priority > BOND_RADIUS_PRI_MAX)
                  && (radiusSrv.priority != SYS_RADIUS_SRV_PRIO_AUTO_SEL), SYS_ERR_INPUT);
    SYS_PARAM_CHK((radiusSrv.timeout < BOND_RADIUS_TIMEOUT_MIN
                  || radiusSrv.timeout > BOND_RADIUS_TIMEOUT_MAX), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_TCPUDP_PORT_INVALID(radiusSrv.srvPort), SYS_ERR_TCPUDP_PORT);

    osal_memset(ipstr, 0, sizeof(ipstr));

    if (-1 == index)
    {
        for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
        {
            osal_memset(&tmpRadiusSrv, 0, sizeof(tmpRadiusSrv));
            tmpRadiusSrv.priority = RADIUS_NOT_UESD_FLAG;
            SYS_ERR_CHK(cfg_radiusSrv_get(i, &tmpRadiusSrv));

            if (0 == osal_strcmp(radiusSrv.hostName, ""))
            {
                if ((radiusSrv.srvIp == tmpRadiusSrv.srvIp) && (RADIUS_NOT_UESD_FLAG != tmpRadiusSrv.priority))
                {
                    matchIdx = i;
                    if (SYS_RADIUS_SRV_PRIO_AUTO_SEL == radiusSrv.priority)
                        radiusSrv.priority = tmpRadiusSrv.priority;
                    break;
                }
                
                else if (RADIUS_NOT_UESD_FLAG == tmpRadiusSrv.priority)
                {
                    if (-1 == emptyIdx)
                    emptyIdx = i;
                }
            }
            else
            {
                if ((0 == osal_strcmp(radiusSrv.hostName, tmpRadiusSrv.hostName)) && (RADIUS_NOT_UESD_FLAG != tmpRadiusSrv.priority))
                {
                    matchIdx = i;
                    if (SYS_RADIUS_SRV_PRIO_AUTO_SEL == radiusSrv.priority)
                        radiusSrv.priority = tmpRadiusSrv.priority;

                    break;
                }
                
                else if (RADIUS_NOT_UESD_FLAG == tmpRadiusSrv.priority)
                {
                    if (-1 == emptyIdx)
                    emptyIdx = i;
                }
            }
     }

        if (-1 == matchIdx)
        {
            
            if (-1 != emptyIdx)
                matchIdx = emptyIdx;
            
            else
            {
                return SYS_ERR_RADIUS_SRV_FULL;
            }
        }
    }
    else
    {
        matchIdx = index;
    }

    if (SYS_RADIUS_SRV_PRIO_AUTO_SEL == radiusSrv.priority)
    {
        
        for (j = 1; j <= CAPA_RADIUS_HOST_NUM; j++)
        {
            used = FALSE;
            for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
            {
                SYS_ERR_CHK(cfg_radiusSrv_get(i, &tmpRadiusSrv));
                if (RADIUS_NOT_UESD_FLAG == tmpRadiusSrv.priority)
                    continue;
                if (j == tmpRadiusSrv.priority)
                {
                    used = TRUE;
                    break;
                }
            }

            if (FALSE == used)
            {
                radiusSrv.priority = j;
                break;
            }
        }
    }
    else
    {
        
        for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
        {
            SYS_ERR_CHK(cfg_radiusSrv_get(i, &tmpRadiusSrv));
            if (RADIUS_NOT_UESD_FLAG == tmpRadiusSrv.priority)
                continue;
            if (i == matchIdx)
                continue;

            if (radiusSrv.priority == tmpRadiusSrv.priority)
                return SYS_ERR_RADIUS_SRV_PRIORITY_EXIST;
        }
    }

    SYS_ERR_CHK(cfg_radiusSrv_set(matchIdx, radiusSrv));

    SYS_ERR_CHK(sys_util_ip2Str(radiusSrv.srvIp, ipstr));

    return SYS_ERR_OK;
}

int32 sal_radiusSrv_get(uint32 index, sys_radius_t *pRadiusSrv)
{
    SYS_PARAM_CHK((index >= CAPA_RADIUS_HOST_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pRadiusSrv), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_radiusSrv_get(index, pRadiusSrv));

    return SYS_ERR_OK;
}

int32 sal_radiusSrv_del(uint32 index)
{
    char ipstr[CAPA_IPV4_STR_LEN];
    sys_radius_t radiusSrv;
    sys_radius_t logRadiusSrv;

    SYS_PARAM_CHK((index >= CAPA_RADIUS_HOST_NUM), SYS_ERR_INPUT);

    osal_memset(ipstr, 0, sizeof(ipstr));
    osal_memset(&logRadiusSrv, 0, sizeof(logRadiusSrv));

    SYS_ERR_CHK(cfg_radiusSrv_get(index, &logRadiusSrv));

    if (0 == logRadiusSrv.srvIp && RADIUS_NOT_UESD_FLAG == logRadiusSrv.priority)
        return SYS_ERR_OK;

    osal_memset(&radiusSrv, 0, sizeof(radiusSrv));
    radiusSrv.priority = RADIUS_NOT_UESD_FLAG;
    radiusSrv.srvIp = logRadiusSrv.srvIp;

    SYS_ERR_CHK(cfg_radiusSrv_set(index, radiusSrv));

    SYS_ERR_CHK(sys_util_ip2Str(logRadiusSrv.srvIp, ipstr));

    return SYS_ERR_OK;
}

