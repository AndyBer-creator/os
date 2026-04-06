
#include <libcfg/cfg.h>

int32 cfg_snmp_enable_set(sys_enable_t enable)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    pSnmp->enable = enable;

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_enable_get(sys_enable_t *pEnable)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    *pEnable = pSnmp->enable;

    CFG_CLOSE(CFG_ID_SNMP);
}

#ifdef CONFIG_SYS_MIB_TRAP

int32 cfg_snmp_trapEnable_get(uint32 *pMask)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pMask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    *pMask  = pSnmp->enableTrapMask;

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_trapEnable_set(uint32 mask)
{
    cfg_snmp_t *pSnmp = NULL;

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    pSnmp->enableTrapMask = mask;

    CFG_CLOSE(CFG_ID_SNMP);
}
#endif

int32 cfg_snmp_target_get(uint32 index, sal_snmp_target_t *pTarget)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pTarget), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pTarget, &pSnmp->target[index], sizeof(sal_snmp_target_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_target_set(uint32 index, sal_snmp_target_t *pTarget)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pTarget), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(&pSnmp->target[index], pTarget,  sizeof(sal_snmp_target_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_targetArry_set(sal_snmp_target_t *pHost, uint32 entryNum)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pHost), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((entryNum > CAPA_SNMP_HOST_NUM ), SYS_ERR_BUF_SIZE);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memset(&pSnmp->target, 0, sizeof(sal_snmp_target_t)*CAPA_SNMP_HOST_NUM);
    osal_memcpy(&pSnmp->target, pHost, sizeof(sal_snmp_target_t) * entryNum );

    CFG_CLOSE(CFG_ID_SNMP);
}

#ifdef CONFIG_SYS_SNMPV3

int32 cfg_snmp_view_get(uint32 index, sal_snmp_view_t *pView)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pView, &pSnmp->view[index], sizeof(sal_snmp_view_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_view_set(uint32 index, sal_snmp_view_t *pView)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(&pSnmp->view[index], pView, sizeof(sal_snmp_view_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_viewArry_set(sal_snmp_view_t *pView, uint32 entryNum)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((entryNum > CAPA_SNMP_VIEW_NUM ), SYS_ERR_BUF_SIZE);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memset(&pSnmp->view, 0, sizeof(sal_snmp_view_t)*CAPA_SNMP_VIEW_NUM);
    osal_memcpy(&pSnmp->view, pView, sizeof(sal_snmp_view_t) * entryNum );

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_group_get(uint32 index, sal_snmp_group_t *pGroup)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pGroup,&pSnmp->group[index], sizeof(sal_snmp_group_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_group_set(uint32 index, sal_snmp_group_t *pGroup)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(&pSnmp->group[index], pGroup,sizeof(sal_snmp_group_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_access_get(uint32 index, sal_snmp_access_t *pAccess)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pAccess), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pAccess, &pSnmp->access[index], sizeof(sal_snmp_access_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_access_set(uint32 index, sal_snmp_access_t *pAccess)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pAccess), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(&pSnmp->access[index], pAccess, sizeof(sal_snmp_access_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_groupArry_set(sal_snmp_access_t *pAccess, uint32 entryNum)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pAccess), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((entryNum > SYS_SNMP_ACCESS_NUM_MAX ), SYS_ERR_BUF_SIZE);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memset(&pSnmp->access, 0, sizeof(sal_snmp_access_t)*SYS_SNMP_ACCESS_NUM_MAX);
    osal_memcpy(&pSnmp->access, pAccess, sizeof(sal_snmp_access_t) * entryNum );

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_com2sec_get(uint32 index, sal_snmp_com2sec_t *pCom2sec)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pCom2sec), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pCom2sec, &pSnmp->com2sec[index], sizeof(sal_snmp_com2sec_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_com2sec_set(uint32 index, sal_snmp_com2sec_t *pCom2sec)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pCom2sec), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(&pSnmp->com2sec[index], pCom2sec, sizeof(sal_snmp_com2sec_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_comArry_set(sal_snmp_com2sec_t *pCom, uint32 entryNum)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pCom), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((entryNum > CAPA_SNMP_COMMUNITY_NUM ), SYS_ERR_BUF_SIZE);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memset(&pSnmp->com2sec, 0, sizeof(sal_snmp_com2sec_t)*CAPA_SNMP_COMMUNITY_NUM);
    osal_memcpy(&pSnmp->com2sec, pCom, sizeof(sal_snmp_com2sec_t) * entryNum );

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_user_get(uint32 index, sal_snmp_user_t *pUser)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pUser), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pUser, &pSnmp->user[index], sizeof(sal_snmp_user_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_user_set(uint32 index, sal_snmp_user_t *pUser)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pUser), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy( &pSnmp->user[index], pUser, sizeof(sal_snmp_user_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_userArry_set(sal_snmp_user_t *pUser, uint32 entryNum)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pUser), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((entryNum > CAPA_SNMP_USER_NUM ), SYS_ERR_BUF_SIZE);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memset(&pSnmp->user, 0, sizeof(sal_snmp_user_t)*CAPA_SNMP_USER_NUM);
    osal_memcpy(&pSnmp->user, pUser, sizeof(sal_snmp_user_t) * entryNum );

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_remoteEngine_get(uint32 index, sal_snmp_remoteEngine_t *pRemote)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pRemote), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pRemote, &pSnmp->remote[index], sizeof(sal_snmp_remoteEngine_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_remoteEngine_set(uint32 index, sal_snmp_remoteEngine_t *pRemote)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pRemote), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy( &pSnmp->remote[index], pRemote, sizeof(sal_snmp_remoteEngine_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_rmtEngineArry_set(sal_snmp_remoteEngine_t *pRemote, uint32 entryNum)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pRemote), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((entryNum > CAPA_SNMP_REMOTE_ENGINEID_NUM ), SYS_ERR_BUF_SIZE);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memset(&pSnmp->remote, 0, sizeof(sal_snmp_remoteEngine_t)*CAPA_SNMP_REMOTE_ENGINEID_NUM);
    osal_memcpy(&pSnmp->remote, pRemote, sizeof(sal_snmp_remoteEngine_t) * entryNum );

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_engineid_get(char *pEngineID)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pEngineID), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pEngineID,  pSnmp->engineId, sizeof(pSnmp->engineId));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_engineid_set(char *pEngineID)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pEngineID), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pSnmp->engineId, pEngineID,  sizeof(pSnmp->engineId));

    CFG_CLOSE(CFG_ID_SNMP);
}

#else

int32 cfg_snmp_community_set(uint32 index, sal_snmp_community_t *pCommunity)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pCommunity), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(&pSnmp->community[index], pCommunity, sizeof(sal_snmp_community_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_community_get(uint32 index, sal_snmp_community_t *pCommunity)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pCommunity), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pCommunity, &pSnmp->community[index], sizeof(sal_snmp_community_t));

    CFG_CLOSE(CFG_ID_SNMP);
}
#endif

int32 cfg_snmp_config_reload_get(sal_snmp_reload_t *pReload)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pReload), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    *pReload = pSnmp->reload;

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_config_reload_set(sal_snmp_reload_t reload)
{
    cfg_snmp_t *pSnmp = NULL;

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    pSnmp->reload = reload;

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_statistic_get(uint32 index, uint32 *pCnt)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pCnt), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= SNMP_STAT_MAX_STATS), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    *pCnt = pSnmp->snmpStatics[index];

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_statistic_set(uint32 index, uint32 cnt)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((index >= SNMP_STAT_MAX_STATS), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    pSnmp->snmpStatics[index] = cnt;

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_notifyMsg_init(void)
{
    cfg_snmp_t *pSnmp = NULL;

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memset(pSnmp->notifyMsg, 0, sizeof(sys_snmp_notifyMsg_t) * SNMP_NOTIFY_MAX_BUF);

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_notifyMsg_set(uint32 index, sys_snmp_notifyMsg_t *pMsg)
{
    cfg_snmp_t *pSnmp = NULL;
    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index > SNMP_NOTIFY_MAX_BUF), SYS_ERR_ENTRY_INDEX);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(&pSnmp->notifyMsg[index], pMsg, sizeof(sys_snmp_notifyMsg_t));

    CFG_CLOSE(CFG_ID_SNMP);
}

int32 cfg_snmp_notifyMsg_get(uint32 index,sys_snmp_notifyMsg_t *pMsg)
{
    cfg_snmp_t *pSnmp = NULL;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index > SNMP_NOTIFY_MAX_BUF), SYS_ERR_ENTRY_INDEX);

    CFG_OPEN(CFG_ID_SNMP, (cfg_addr)&pSnmp);

    osal_memcpy(pMsg, &pSnmp->notifyMsg[index], sizeof(sys_snmp_notifyMsg_t));

    CFG_CLOSE(CFG_ID_SNMP);
}
