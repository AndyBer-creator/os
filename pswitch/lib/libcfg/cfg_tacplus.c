
#include <libcfg/cfg.h>

int32 cfg_tacplus_init(void)
{
    uint32          i;
    cfg_tacplus_t   *pTacplus = NULL;

    CFG_OPEN(CFG_ID_TACPLUS, (cfg_addr)&pTacplus);

    pTacplus->tacDftParam.timeout = DFLT_TACPLUS_TIMEOUT;
    for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
    {
        pTacplus->tacplusSrv[i].priority = -1;
    }

    CFG_CLOSE(CFG_ID_TACPLUS);
}

int32 cfg_tacplus_dftParam_set(sys_tacplus_t tacplusSrv)
{
    cfg_tacplus_t *pTacplus = NULL;

    CFG_OPEN(CFG_ID_TACPLUS, (cfg_addr)&pTacplus);

    osal_memcpy(&pTacplus->tacDftParam, &tacplusSrv, sizeof(sys_tacplus_t));

    CFG_CLOSE(CFG_ID_TACPLUS);
}

int32 cfg_tacplus_dftParam_get(sys_tacplus_t *pTacplusSrv)
{
    cfg_tacplus_t *pTacplus = NULL;

    SYS_PARAM_CHK((NULL == pTacplusSrv), SYS_ERR_NULL_POINTER);

    osal_memset(pTacplusSrv, 0, sizeof(sys_tacplus_t));

    CFG_OPEN(CFG_ID_TACPLUS, (cfg_addr)&pTacplus);

    osal_memcpy(pTacplusSrv, &pTacplus->tacDftParam, sizeof(sys_tacplus_t));

    CFG_CLOSE(CFG_ID_TACPLUS);
}

int32 cfg_tacplus_server_set(uint32 index, sys_tacplus_t tacplusSrv)
{
    cfg_tacplus_t *pTacplus = NULL;

    SYS_PARAM_CHK(index >= CAPA_TACPLUS_HOST_NUM, SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_TACPLUS, (cfg_addr)&pTacplus);

    osal_memcpy(&pTacplus->tacplusSrv[index], &tacplusSrv, sizeof(sys_tacplus_t));

    CFG_CLOSE(CFG_ID_TACPLUS);
}

int32 cfg_tacplus_server_get(uint32 index, sys_tacplus_t *pTacplusSrv)
{
    cfg_tacplus_t *pTacplus = NULL;

    SYS_PARAM_CHK(index >= CAPA_TACPLUS_HOST_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pTacplusSrv), SYS_ERR_NULL_POINTER);

    osal_memset(pTacplusSrv, 0, sizeof(sys_tacplus_t));

    CFG_OPEN(CFG_ID_TACPLUS, (cfg_addr)&pTacplus);

    osal_memcpy(pTacplusSrv, &pTacplus->tacplusSrv[index], sizeof(sys_tacplus_t));

    CFG_CLOSE(CFG_ID_TACPLUS);
}

