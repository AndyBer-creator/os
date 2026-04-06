
#ifndef __SAL_SNMP_H__
#define __SAL_SNMP_H__

#include <libsal/sal_port.h>
#include <rsd/rsd_snmp.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/vosMsg.h>
#include <libipc/vos.h>

#define SNMP_LOG(mid, args...)               \
do {                                        \
        SYS_LOG(LOG_CAT_SNMP, mid, ##args); \
} while(0)

#define SNMP_CMD_TRAP(_fmt, args...)\
do{                              \
    char info[256];               \
    SYS_MEM_CLEAR(info);          \
    osal_sprintf(info, _fmt, ##args);\
	sal_snmp_operation_Trap_send(info);\
}while(0)

#define SNMP_DBG_TARGET_FILE2 "/tmp/SNMP_DEBUG2"
#if 0
#define SNMP_DBG2(a,args...) \
 do { \
	 char tmp_str[256] = ""; \
	 char cmd_str[300] = ""; \
	 sprintf(tmp_str, "[SNMP_DBG] %s:%d:"#a"", __FUNCTION__,__LINE__, ##args); \
	 sprintf(cmd_str, "echo \"%s\" >> %s", tmp_str, SNMP_DBG_TARGET_FILE2); \
	 system(cmd_str); \
 } while (0)
#else
#define SNMP_DBG2(a,args...) {}
#endif

#define MAX_SNMP_TRAP_ARG_MAX       10

#define SNMP_NOTIFY_MAX_BUF         256

#define MAX_SNMP_TRAP_STRLEN        512
#define MAX_SNMPV3_TRAP_HEAD_LEN    256

#define SNMP_STAT_MAX_STATS     56  
#define SNMP_STAT_SNMPOUTTRAPS  37  

#define SNMP_CONF_FILE              "/etc/snmp/snmpd.conf"  
#define SNMP_CONF_VAR_FILE          "/var/net-snmp/snmpd.conf"
#define SNMP_CONF_TRAP_FILE          "/var/net-snmp/trap.conf"
#define SNMP_SYS_STARTTIME_KEEP  60

#define SYS_SNMP_SECGROUP_NUM_MAX      (CAPA_SNMP_COMMUNITY_NUM * 2 + CAPA_SNMP_USER_NUM)
#define SYS_SNMP_ACCESS_NUM_MAX      (CAPA_SNMP_COMMUNITY_NUM * 2 + CAPA_SNMP_GROUP_NUM)

#define SNMP_ACCESS_GROUP_PREFIX  "NS_group_"

#define SNMP_TRAP_SERVER_PATH  "snmp_trap_server_ipc_"

extern const char *gviewTypeStr[];
extern const char *gsecModel[];
extern const char *gsecLevel[];
extern const char *gprefix[];
extern const char *gtrapStr[];
extern const char *gtrapType[];
extern const char *gtrapVersion[];
extern const char *gaccess[];
extern const char *gsslType[];
extern const char *snmpv3_ssl_type[];
extern const char *gsecCfModel[];
typedef enum {
	TRAP_FUNC_START,
    TRAP_FUNC_OPERATE,
    TRAP_FUNC_VLAN_DESTROY,
    TRAP_FUNC_PORT_LINK,
    TRAP_FUNC_LOGIN_MESSAGES,
    TRAP_FUNC_LOGIN_PASSWORD_CHANGE,
    TRAP_FUNC_REBOOT,
    TRAP_FUNC_USER_OPERATION,
    TRAP_FUNC_USER_OPERATION_INFO,
    TRAP_FUNC_MAX
}TRAP_FUNC_ENT;

typedef enum sal_snmp_community_access_e
{
    SNMP_COMMUNITY_ACCESS_READ_ONLY = 0,
    SNMP_COMMUNITY_ACCESS_READ_WRITE,
    SNMP_COMMUNITY_ACCESS_END
} sal_snmp_community_access_t;

typedef enum sal_snmp_comMode_e
{
    SNMP_COM_MODE_BASIC,
    SNMP_COM_MODE_ADV,
    SNMP_COM_MODE_END
}sal_snmp_comMode_t;

typedef enum sal_snmp_ssl_type_e
{
    SNMP_SSL_TYPE_NONE = 0,
    SNMP_SSL_TYPE_MD5,
    SNMP_SSL_TYPE_SHA,
    SNMP_SSL_TYPE_DES,
    SNMP_SSL_TYPE_END
} sal_snmp_ssl_type_t;

typedef enum sal_snmp_v3priv_e
{
    SNMP_SSL_NOAUTH_NOPRIV = 0,
    SNMP_SSL_AUTH_NOPRIV,
    SNMP_SSL_AUTH_PRIV,
    SNMP_SSL_PRIV_END
} sal_snmp_v3priv_t;

#ifdef CONFIG_SYS_MIB_TRAP
typedef enum sal_snmp_trapClass_e
{
    SNMP_TRAP_CLASS_AUTHENTICATE,
    SNMP_TRAP_CLASS_LINKUPDOWN,
    SNMP_TRAP_CLASS_BOOTUP_WARM,
    SNMP_TRAP_CLASS_BOOTUP_COOL,
    SNMP_TRAP_CLASS_PORT_SEC,
    SNMP_TRAP_CLASS_END
} sal_snmp_trapClass_t;
#endif

typedef enum sal_snmp_trapCode_e
{
    SNMP_TRAP_CODE_COLD_START,
    SNMP_TRAP_CODE_WARM_START,
    SNMP_TRAP_CODE_LINKDOWN,
    SNMP_TRAP_CODE_LINKUP,
    SNMP_TRAP_CODE_AUTH_FAILD,
    SNMP_TRAP_CODE_PORT_SECURITY,
    SNMP_TRAP_CODE_END
} sal_snmp_trapCode_t;

typedef enum sal_snmp_trapType_e
{
    SNMP_TRAP_V1,
    SNMP_TRAP_V2,
    SNMP_INFORM_V2,
    SNMP_TRAP_V3,
    SNMP_INFORM_V3,
    SNMP_TRAP_END
} sal_snmp_trapType_t;

typedef enum sal_snmp_secmodel_e
{
    SNMP_SECMODEL_V1,
    SNMP_SECMODEL_V2C,
    SNMP_SECMODEL_USM,
    SNMP_SECMODEL_END
} sal_snmp_secmode_t;

typedef enum sal_snmp_reload_e
{
    SNMP_RELOAD_AUTO,
    SNMP_RELOAD_MANUAL,
    SNMP_RELOAD_END
} sal_snmp_reload_t;

#ifdef CONFIG_SYS_SNMPV3
typedef enum sal_snmp_viewtype_e
{
    SNMP_VIEWTYPE_INCLUDE = 1,
    SNMP_VIEWTYPE_EXCLUDE,
    SNMP_VIEWTYPE_END
} sal_snmp_viewtype_t;

typedef enum sal_snmp_prefix_e
{
    SNMP_PREFIX_EXACT,
    SNMP_PREFIX_PREFIX,
    SNMP_PREFIX_END
} sal_snmp_prefix_t;

typedef enum sal_snmp_sslType_e
{
    SNMP_SSLTYPE_NONE,
    SNMP_SSLTYPE_MD5 ,
    SNMP_SSLTYPE_SHA,
    SNMP_SSLTYPE_DES,
    SNMP_SSLTYPE_END
} sal_snmp_sslType_t;
#endif

typedef enum sal_snmp_seclevel_e
{
    SNMP_SECLEVEL_NOAUTHNOPRIV,
    SNMP_SECLEVEL_AUTHNOPRIV,
    SNMP_SECLEVEL_AUTHPRIV,
    SNMP_SECLEVEL_END
} sal_snmp_seclevel_t;

typedef enum sal_snmp_configMode_e
{
    SNMP_CONFIG_MANUAL,
    SNMP_CONFIG_AUTO,
    SNMP_CONFIG_END
} sal_snmp_configMode_t;

typedef enum sal_snmp_acount_e
{
    SNMP_ACOUNT_ADD = 1,
    SNMP_ACOUNT_CHANGE,
    SNMP_ACOUNT_DELETE,
    SNMP_ACOUNT_END
} sal_snmp_acount_t;

typedef struct sal_snmp_target_s
{
    char   name[CAPA_SNMP_USER_NAME_LEN + 1];
    char   server[CAPA_HOSTNAME_LEN+1];
    sal_snmp_secmode_t   secModel;
    sal_snmp_trapType_t   trapType;
    sal_snmp_seclevel_t    secLevel;
    uint16  portNum;
    uint16  retries;
    uint16  timeout;
} sal_snmp_target_t;

#ifdef CONFIG_SYS_SNMPV3
typedef struct sal_snmp_remoteEngine_s
{
    char   server[CAPA_HOSTNAME_LEN+1];
    char   engineId[BOND_SNMP_ENGINEID_LEN_MAX+1];
}sal_snmp_remoteEngine_t;

typedef struct sal_snmp_oid_s
{
    uint32 len;
    uint32 id[CAPA_SNMP_VIEW_SUBTREE_LEN+1];
} sal_snmp_oid_t;

typedef struct sal_snmp_view_s
{
    char viewName[CAPA_SNMP_VIEW_NAME_LEN+1];
    uint8 viewType;
    sal_snmp_oid_t subtreeOid;
    char viewMaskStr[CAPA_SNMP_VIEW_SUBTREE_LEN+1];
} sal_snmp_view_t;

typedef struct sal_snmp_group_s
{
    char groupName[CAPA_SNMP_GROUP_NAME_LEN+1];
    uint8 secModel;
    char secName[CAPA_SNMP_SEC_NAME_LEN+1];
} sal_snmp_group_t;

typedef struct sal_snmp_com2sec_s
{
    char secName[CAPA_SNMP_SEC_NAME_LEN+1];
    char comName[CAPA_SNMP_COMMUNITY_LEN+1];
    char sourceNet[CAPA_SNMP_SOURCE_NET_LEN+1];
    char mask[CAPA_SNMP_SOURCE_NET_LEN+1];
    char groupName[CAPA_SNMP_GROUP_NAME_LEN+1];
    char viewName[CAPA_SNMP_VIEW_NAME_LEN+1];
    uint32 access;
    sal_snmp_comMode_t mode;
} sal_snmp_com2sec_t;

typedef struct sal_snmp_access_s
{
    char groupName[CAPA_SNMP_GROUP_NAME_LEN + 1];
    char context[CAPA_SNMP_CONTEXT_LEN+1];
    uint8 secMode;
    uint8 secLevel;
    uint8 prefix;
    char readView[CAPA_SNMP_VIEW_NAME_LEN+1];
    char writeView[CAPA_SNMP_VIEW_NAME_LEN+1];
    char notifyView[CAPA_SNMP_VIEW_NAME_LEN+1];
    uint8 groupcfMode;
    uint8 viewcfMode;
} sal_snmp_access_t;

typedef struct sal_snmp_user_s
{
    char userName[CAPA_SNMP_USER_NAME_LEN + 1];
    char groupName[CAPA_SNMP_GROUP_NAME_LEN + 1];
    uint8 access;
    sal_snmp_ssl_type_t auth_protp;
    char authKey[BOND_SNMP_AUTH_KEY_LEN_MAX+1];
    sal_snmp_ssl_type_t encrypt_proto;
    char privKey[BOND_SNMP_PRIV_KEY_LEN_MAX+1];
    sal_snmp_seclevel_t  selLevel;
} sal_snmp_user_t;
#else
typedef struct sal_snmp_community_s
{
    char name[CAPA_SNMP_COMMUNITY_LEN + 1];
    uint32 access;
} sal_snmp_community_t;
#endif 

extern const char *snmpv3_access[SNMP_COMMUNITY_ACCESS_END];

typedef struct sys_snmp_notifyMsg_s
{
    uint8  valid;
    char   trapMsg[MAX_SNMP_TRAP_STRLEN+1];
} sys_snmp_notifyMsg_t;

typedef struct TRAP_ARG_tag{
    uint16        usFlags;
    uint16        usLen;
    void          *pBuf; 
}TRAP_ARG;

typedef struct sal_snmp_trapMsg_s
{
	uint32 trapName;
    TRAP_ARG args[MAX_SNMP_TRAP_ARG_MAX]; 
    uint16 num;
} sal_snmp_trapMsg_t;

typedef struct sal_snmp_trapDetail_s
{
	uint32 trapName;
    char type[24];    
    char detail[512]; 
} sal_snmp_trapDetail_t;

extern int32 sal_snmp_enable_set(sys_enable_t enable);

extern int32 sal_snmp_enable_get(sys_enable_t *pEnable);

#ifdef CONFIG_SYS_MIB_TRAP

extern int32 sal_snmp_systemTrap_send(char *pInfo);

extern int32 sal_snmp_trapEnable_get(sal_snmp_trapClass_t trapclass, sys_enable_t *pEnable);

extern int32 sal_snmp_trapEnable_set(sal_snmp_trapClass_t trapclass, sys_enable_t enable);
#endif

extern int32 sal_snmp_sysUpTime_set(void);

extern int32 sal_snmp_sysUpTime_get(uint32 *pSec, uint32 *pUsec);

extern int32 sal_snmp_sysUpTimeGep_get(uint64 *time);

extern int32 sal_snmp_stop(void);

extern int32 sal_snmp_start(void);

extern int32 sal_snmp_restart(void);

#ifdef CONFIG_SYS_MIB_TRAP

extern int32 sal_snmp_linkUpDownTrap_send(sys_logic_port_t logicPort, sys_port_linkStatus_t link);

extern int32 sal_snmp_startupTrap_send(sys_enable_t restart);

extern int32 sal_snmp_authenticationFailure_send(void);

extern int32 sal_snmp_portSecurityTrap_send(char *pOid, sys_logic_port_t logicPort, char *pMsg);
extern int32 sal_snmp_CPUTrap_send(char *pInfo);
extern int32 sal_snmp_memoryTrap_send(char *pInfo);
extern int32 sal_snmp_storm_ctrl_Trap_send(char *pInfo);
extern int32 sal_snmp_systerm_reboot_Trap_send(char *pInfo);
extern int32 sal_snmp_systerm_upgrade_Trap_send(char *pInfo);
extern int32 sal_snmp_operation_Trap_send(char *pInfo);
extern int32 sal_snmp_poe_Trap_send(char *pInfo);
#endif

#ifdef CONFIG_SYS_MIB_RMON

extern int32 sal_snmp_rmonEventTrap_send(uint8 is_rising, uint32 alarm_index, uint32 value, uint32 the_threshold,
                                                                uint32 *alarmed_var,  uint32 alarmed_var_length, uint32 sample_type);
#endif

extern int32 sal_snmp_target_get(uint32 index, sal_snmp_target_t *pTarget);

extern int32 sal_snmp_target_set(uint32 index, sal_snmp_target_t *pTarget);

extern int32 sal_snmp_targetEmpty_get(uint32 *pIndex);

extern int32 sal_snmp_targetIndex_get(char *pServer, sal_snmp_secmode_t secModel, sal_snmp_trapType_t trapType, uint32 *index);

extern int32 sal_snmp_target_add(sal_snmp_target_t *pHost);

extern int32 sal_snmp_target_del(char *pServer,  sal_snmp_secmode_t secModel, sal_snmp_trapType_t trapType);

extern int32 sal_snmp_targetByIdx_del(uint32 idx);

extern int32 _sal_snmp_targetSort_update(void);

#ifdef CONFIG_SYS_SNMPV3

extern int32 sal_snmp_view_get(uint32 index, sal_snmp_view_t *pView);

extern int32 sal_snmp_view_set(uint32 index, sal_snmp_view_t *pView);

extern int32 sal_snmp_viewEmpty_get(uint32 *pIndex);

extern int32 sal_snmp_viewIndex_get(char *pView, sal_snmp_oid_t *pSubtree, uint32 *index);

extern int32 sal_snmp_viewIndexByName_get(char *pView, uint32 *index);

extern int32 sal_snmp_view_add(char *pView, sal_snmp_oid_t *pSubtree, uint8 viewType,  char *pmsk);

extern int32 sal_snmp_view_del(char *pView, sal_snmp_oid_t *pSubtree);

extern int32 sal_snmp_viewByIdx_del(uint32 idx);

extern int32 sal_snmp_group_get(uint32 index, sal_snmp_group_t *pGroup);

extern int32 sal_snmp_group_set(uint32 index, sal_snmp_group_t *pGroup);

extern int32 sal_snmp_groupEmpty_get(uint32 *pIndex);

extern int32 sal_snmp_groupIndex_get(char *pSecName, uint8 secModel, uint32 *index);

extern int32 sal_snmp_group_add(char *pSecName, uint8 secModel, char *pGroupName);

extern int32 sal_snmp_group_del(char *pSecName, uint8 secModel);

extern int32 sal_snmp_groupBySecurityName_get(char *pSecName, sal_snmp_group_t *pGroup);

extern int32 sal_snmp_access_get(uint32 index, sal_snmp_access_t *pAccess);

extern int32 sal_snmp_access_set(uint32 index, sal_snmp_access_t *pAccess);

extern int32 sal_snmp_accessEmpty_get(uint8 cfgMode, uint32 *pIndex);

extern int32 sal_snmp_accessIndex_get(char *pGroupName, char *pContext, uint8 secModel, uint8 secLevel, uint32 *index);

extern int32 sal_snmp_basic_accessAdd_test(sal_snmp_access_t  *access);

extern int32 sal_snmp_accessView_test(sal_snmp_access_t  *access);

extern int32 sal_snmp_access_add(sal_snmp_access_t  *pAccess);

extern int32 sal_snmp_access_del(char *pGroupName, char *pContext, uint8 secModel, uint8 secLevel);

extern int32 sal_snmp_accessByIdx_del(uint32 idx);

extern int32 sal_snmp_viewNameByGroupName_get(char *pGroup, uint32 access, char *pView);

extern int32 sal_snmp_accessGroupByGroupName_get(char *pGroup, sal_snmp_access_t *pAccess);

extern int32 sal_snmp_com2sec_get(uint32 index, sal_snmp_com2sec_t *pTarget);

extern int32 sal_snmp_com2sec_set(uint32 index, sal_snmp_com2sec_t *pTarget);

extern int32 sal_snmp_com2secEmpty_get(uint32 *pIndex);

extern int32 sal_snmp_com2secIndex_get(char *psecName, uint32 *index);

extern int32 sal_snmp_com2sec_add(sal_snmp_com2sec_t *pCom);

extern int32 sal_snmp_com2sec_del(char *pSecName);

extern int32 sal_snmp_user_get(uint32 index, sal_snmp_user_t *pUser);

extern int32 sal_snmp_userByuserName_get(char *pUserName, sal_snmp_user_t *pUser);

extern int32 sal_snmp_user_set(uint32 index, sal_snmp_user_t *pUser);

extern int32 sal_snmp_userEmpty_get(uint32 *pIndex);

extern int32 sal_snmp_userIndex_get(char *pUserName, uint32 *index);

extern int32 sal_snmp_user_add(sal_snmp_user_t *pUser);

extern int32 sal_snmp_user_del(char *pUserName);

extern int32 sal_snmp_remoteEngine_get(uint32 index, sal_snmp_remoteEngine_t *pRemote);

extern int32 sal_snmp_remoteByHostName_get(char *pHostName, sal_snmp_remoteEngine_t *pRemote);

extern int32 sal_snmp_remoteEngine_set(uint32 index, sal_snmp_remoteEngine_t *pRemote);

extern int32 sal_snmp_remoteEngineEmpty_get(uint32 *pIndex);

extern int32 sal_snmp_remoteEngineIndex_get(char *pHostName, uint32 *index);

extern int32 sal_snmp_remoteEngine_add(sal_snmp_remoteEngine_t *pRemote);

extern int32 sal_snmp_remoteEngine_del(char *pHostName);

extern int32 sal_snmp_engineid_get(char *pEngineid);

extern int32 sal_snmp_defaultEngineId_get(char *pEngineid);

extern int32 sal_snmp_engineid_set(char *pEngineid);

extern int32 sal_snmp_confFile_update(void);

extern uint32 sal_util_str2oid(char *pStr, uint32 *len, uint32 *id);

extern int32 sal_util_oid2str(uint32 len, uint32 *id,  char *pStr);

extern int32 sal_util_check_hex(const char *pHex);

extern int32 sal_util_snmpv3TrapHeader_get(sal_snmp_target_t *pHost, char *pHeader, int32 len);

extern int32 _sal_snmp_viewSort_update(void);

extern int32 _sal_snmp_comSort_update(void);

extern int32 _sal_snmp_accessSort_update(void);

extern int32 _sal_snmp_userSort_update(void);

extern int32 _sal_snmp_rmtEngineSort_update(void);
#else

extern int32 sal_snmp_community_set(uint32 index, sal_snmp_community_t *pCommunity);

extern int32 sal_snmp_community_get(uint32 index, sal_snmp_community_t *pCommunity);

extern int32 sal_snmp_communityEmpty_get(uint32 *pIndex);

extern int32 sal_snmp_communityIndex_get(char *pCommunity, uint32 *index);

extern int32 sal_snmp_community_add(char *pCommunity, uint32 access);

extern int32 sal_snmp_community_del(char *pCommunity);

extern int32 sal_snmp_confFile_update(void);
#endif

extern int32 sal_snmp_config_reload_get(sal_snmp_reload_t *pReload);

extern int32 sal_snmp_config_reload_set(sal_snmp_reload_t reload);

extern int32 sal_snmp_statistic_get(uint32 index, uint32 *pCnt);

extern int32 sal_snmp_statistic_set(uint32 index, uint32 cnt);

extern int32 sal_snmp_statistic_init(void);

extern int32 _sal_snmp_notifyMsg_init(void);

extern int32 _sal_snmp_notifyMsg_set(uint32 index, sys_snmp_notifyMsg_t *pMsg);

extern int32 _sal_snmp_notifyMsg_get(uint32 index,sys_snmp_notifyMsg_t *pMsg);

extern int32 sal_snmp_name_isValid(char *pName);

extern void sal_snmp_trap_static_vlan_create(int pid, uint32 vid, char* vlanName, int32 ret);
extern void sal_snmp_trap_static_vlan_destroy(int pid, uint32 vid, int32 ret);
extern int32 sal_snmp_linkUpDownTrap_send_new(sys_logic_port_t logicPort, sys_port_linkStatus_t link);
extern int32 sal_snmp_startupTrap_send_new(sys_enable_t restart);
extern int32 sal_snmp_systerm_reboot_Trap_send_new(int pid);

extern uint32 sal_snmp_system_auth_success_fail_Trap_send_new(uint32 sOrF,const char *connectType,char *username,char *hostIp);

extern uint32 sal_snmp_system_auth_logout_Trap_send_new(char *username, char *connectType, char *hostIp);

extern uint32 sal_snmp_system_auth_acount_Trap_send_new(uint32 opCode, int32 pid, char *username,uint32 accessMethod);
extern uint32 sal_snmp_system_user_operation_Trap_send_new(uint32 opCode,int32 sOrF, char *username);
extern void sal_snmp_trap_mac_option(int32 option,sys_l2_entry_t *pEntry);
extern uint32 sal_snmp_system_user_operation_info_Trap(int pid, char *cmdInfo);
extern uint32 sal_snmp_system_web_user_operation_info_Trap(char *cmdInfo);
extern uint32 sal_snmp_system_web_user_operation_Trap_send_new(uint32 opCode,int32 sOrF, char *username);

#endif

