
#ifndef __CFG_SNMP_H__
#define __CFG_SNMP_H__

#include <libsal/sal_snmp.h>

typedef struct cfg_snmp_s
{
    sys_enable_t enable;
#ifdef CONFIG_SYS_MIB_TRAP
    uint32 enableTrapMask;
#endif
    sal_snmp_reload_t   reload;
    sal_snmp_target_t target[CAPA_SNMP_HOST_NUM_MAX];

#ifdef CONFIG_SYS_SNMPV3
    sal_snmp_view_t     view[CAPA_SNMP_VIEW_NUM];
    sal_snmp_group_t    group[SYS_SNMP_SECGROUP_NUM_MAX];
    sal_snmp_com2sec_t  com2sec[CAPA_SNMP_COMMUNITY_NUM];
    sal_snmp_access_t   access[SYS_SNMP_ACCESS_NUM_MAX];
    sal_snmp_user_t     user[CAPA_SNMP_USER_NUM];
    sal_snmp_remoteEngine_t  remote[CAPA_SNMP_REMOTE_ENGINEID_NUM];
    char                engineId[BOND_SNMP_ENGINEID_LEN_MAX + 1];
#else
    sal_snmp_community_t community[CAPA_SNMP_COMMUNITY_NUM];
#endif
    sys_snmp_notifyMsg_t  notifyMsg[SNMP_NOTIFY_MAX_BUF + 1];
    uint32 snmpStatics[SNMP_STAT_MAX_STATS];
} cfg_snmp_t;

extern int32 cfg_snmp_enable_set(sys_enable_t enable);

extern int32 cfg_snmp_enable_get(sys_enable_t *pEnable);

#ifdef CONFIG_SYS_MIB_TRAP

extern int32 cfg_snmp_trapEnable_get(uint32 *pMask);

extern int32 cfg_snmp_trapEnable_set(uint32 mask);
#endif

extern int32 cfg_snmp_target_get(uint32 index, sal_snmp_target_t *pTarget);

extern int32 cfg_snmp_target_set(uint32 index, sal_snmp_target_t *pTarget);

extern int32 cfg_snmp_targetArry_set(sal_snmp_target_t *pHost, uint32 entryNum);

#ifdef CONFIG_SYS_SNMPV3

extern int32 cfg_snmp_view_get(uint32 index, sal_snmp_view_t *pView);

extern int32 cfg_snmp_view_set(uint32 index, sal_snmp_view_t *pView);

extern int32 cfg_snmp_viewArry_set(sal_snmp_view_t *pView, uint32 entryNum);

extern int32 cfg_snmp_group_get(uint32 index, sal_snmp_group_t *pGroup);

extern int32 cfg_snmp_group_set(uint32 index, sal_snmp_group_t *pGroup);

extern int32 cfg_snmp_access_get(uint32 index, sal_snmp_access_t *pAccess);

extern int32 cfg_snmp_access_set(uint32 index, sal_snmp_access_t *pAccess);

extern int32 cfg_snmp_groupArry_set(sal_snmp_access_t *pAccess, uint32 entryNum);

extern int32 cfg_snmp_com2sec_get(uint32 index, sal_snmp_com2sec_t *pCom2sec);

extern int32 cfg_snmp_com2sec_set(uint32 index, sal_snmp_com2sec_t *pCom2sec);

extern int32 cfg_snmp_comArry_set(sal_snmp_com2sec_t *pCom, uint32 entryNum);

extern int32 cfg_snmp_user_get(uint32 index, sal_snmp_user_t *pUser);

extern int32 cfg_snmp_user_set(uint32 index, sal_snmp_user_t *pUser);

extern int32 cfg_snmp_userArry_set(sal_snmp_user_t *pUser, uint32 entryNum);

extern int32 cfg_snmp_remoteEngine_get(uint32 index, sal_snmp_remoteEngine_t *pRemote);

extern int32 cfg_snmp_remoteEngine_set(uint32 index, sal_snmp_remoteEngine_t *pRemote);

extern int32 cfg_snmp_rmtEngineArry_set(sal_snmp_remoteEngine_t *pRemote, uint32 entryNum);

extern int32 cfg_snmp_engineid_get(char *pEngineID);

extern int32 cfg_snmp_engineid_set(char *pEngineID);

#else

extern int32 cfg_snmp_community_set(uint32 index, sal_snmp_community_t *pCommunity);

extern int32 cfg_snmp_community_get(uint32 index, sal_snmp_community_t *pCommunity);
#endif

extern int32 cfg_snmp_config_reload_get(sal_snmp_reload_t *pReload);

extern int32 cfg_snmp_config_reload_set(sal_snmp_reload_t reload);

extern int32 cfg_snmp_statistic_get(uint32 index, uint32 *pCnt);

extern int32 cfg_snmp_statistic_set(uint32 index, uint32 cnt);

extern int32 cfg_snmp_notifyMsg_init(void);

extern int32 cfg_snmp_notifyMsg_set(uint32 index, sys_snmp_notifyMsg_t *pMsg);

extern int32 cfg_snmp_notifyMsg_get(uint32 index,sys_snmp_notifyMsg_t *pMsg);

#endif

