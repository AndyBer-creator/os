
#include <libcfg/cfg.h>

int32 cfg_radius_init(void)
{
    uint32          i;
    cfg_radius_t    *pRadius = NULL;

    CFG_OPEN(CFG_ID_RADIUS, (cfg_addr)&pRadius);

    pRadius->radDftParam.retransmit = DFLT_RADIUS_RETRY;
    pRadius->radDftParam.timeout = DFLT_RADIUS_TIMEOUT;

    for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
    {
        pRadius->radiusSrv[i].priority = -1;
    }
    CFG_CLOSE(CFG_ID_RADIUS);
}

int32 cfg_radiusSrv_dftParam_set(sys_radius_t radiusSrv)
{
    cfg_radius_t *pRad = NULL;

    CFG_OPEN(CFG_ID_RADIUS, (cfg_addr)&pRad);

    osal_memcpy(&pRad->radDftParam, &radiusSrv, sizeof(sys_radius_t));

    CFG_CLOSE(CFG_ID_RADIUS);
}

int32 cfg_radiusSrv_dftParam_get(sys_radius_t *pRadiusSrv)
{
    cfg_radius_t *pRad = NULL;

    SYS_PARAM_CHK(NULL == pRadiusSrv, SYS_ERR_NULL_POINTER);

    osal_memset(pRadiusSrv, 0, sizeof(sys_radius_t));

    CFG_OPEN(CFG_ID_RADIUS, (cfg_addr)&pRad);

    osal_memcpy(pRadiusSrv, &pRad->radDftParam, sizeof(sys_radius_t));

    CFG_CLOSE(CFG_ID_RADIUS);
}

int32 cfg_radiusSrv_set(uint32 index, sys_radius_t radiusSrv)
{
    cfg_radius_t *pRad = NULL;

    SYS_PARAM_CHK(index >= CAPA_RADIUS_HOST_NUM, SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_RADIUS, (cfg_addr)&pRad);

    osal_memcpy(&pRad->radiusSrv[index], &radiusSrv, sizeof(sys_radius_t));

    CFG_CLOSE(CFG_ID_RADIUS);
}

int32 cfg_radiusSrv_get(uint32 index, sys_radius_t *pRadiusSrv)
{
    cfg_radius_t *pRad = NULL;

    SYS_PARAM_CHK(index >= CAPA_RADIUS_HOST_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pRadiusSrv, SYS_ERR_NULL_POINTER);

    osal_memset(pRadiusSrv, 0, sizeof(sys_radius_t));

    CFG_OPEN(CFG_ID_RADIUS, (cfg_addr)&pRad);

    osal_memcpy(pRadiusSrv, &pRad->radiusSrv[index], sizeof(sys_radius_t));

    CFG_CLOSE(CFG_ID_RADIUS);
}

