
#ifndef __RSD_ACL_MGMT_H__
#define __RSD_ACL_MGMT_H__

#include <common/sys_def.h>

#define SERPORT_TCP_HTTP        80
#define SERPORT_TCP_HTTPS       443
#define SERPORT_UDP_SNMP        161
#define SERPORT_TCP_SSH         22
#define SERPORT_TCP_TELNET      23

#define SYS_MGMT_ACL_NAME_LENTH_MAX    32
#define SYS_MGMT_ACL_ACENUM_MAX        256
#define SYS_MGMT_ACL_PRIORITY_MAX      65535
#define SYS_MGMT_ACL_LISTNUM_MAX       256
#define SYS_MGMT_ACL_GETNEXT_FIRST_ID  0xffffffff
#define SYS_MGMT_ACL_PRIOFFSET         1
#define SYS_MGMT_ACL_IPV6_ONEBYTE      8

#define MGMT_ACL_LIST_SEM_LOCK()    \
do {\
    if (osal_sem_mutex_take( acl_mgmt_List_sem_db, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)\
    {\
        SYS_DBG(DBG_ACLMGR_MGMT, "MGMT_ACL list semaphore lock failed");\
    }\
} while(0)

#define MGMT_ACL_LIST_SEM_UNLOCK() \
do {\
    if (osal_sem_mutex_give( acl_mgmt_List_sem_db) != SYS_ERR_OK)\
    {\
        SYS_DBG(DBG_ACLMGR_MGMT, "MGMT_ACL list semaphore unlock failed");\
    }\
} while(0)

typedef enum sys_acl_mgmt_ListFlag_s
{
    LIST_NONE       = 0,
    LIST_OWNED   = 1,
}sys_acl_mgmt_ListFlag_t;

typedef enum sys_acl_mgmt_ListActive_s
{
    LIST_INACTIVE = 0,
    LIST_ACTIVE     = 1,
}sys_acl_mgmt_ListActive_t;

typedef enum sys_acl_mgmt_AceFlag_s
{
    ACE_NONE       = 0,
    ACE_OWNED   = 1,
  }sys_acl_mgmt_AceFlag_t;

typedef enum sys_acl_mgmt_care_ipSource_s
{
    IPSOURCE_NONE    = 0,
    IPSOURCE_IPV4       = 1,
    IPSOURCE_IPV6 ,
    IPSOURCE_ALL,
    IPSOURCE_END,
}sys_acl_mgmt_care_ipSource_t;

typedef enum sys_acl_mgmt_care_service_s
{
    SERVICE_HTTP  = 1,
    SERVICE_HTTPS,
    SERVICE_SNMP,
    SERVICE_SSH,
    SERVICE_TELNET,
    SERVICE_ALL,
    SERVICE_END,
}sys_acl_mgmt_care_service_t;

typedef enum sys_acl_mgmt_action_s
{
    MGMTACTION_NONE    = 0,
    MGMTACTION_PERMIT = 1,
    MGMTACTION_DENY,
    MGMTACTION_END,
}sys_acl_mgmt_action_t;

typedef enum sys_acl_mgmt_ace_priFlag_s
{
    PRIFLAG_NONE     = 0,
    PRIFLAG_INVALID = 1,
    PRIFLAG_VALID  ,
    PRIFLAG_END,
  }sys_acl_mgmt_ace_priFlag_t;

typedef enum sys_acl_mgmt_class_list_s
{
    CLASS_NONE                       = 0,
    CLASS_CONSOLEONLY    = 1,
    CLASS_MGMTLIST,
    CLASS_END,
  }sys_acl_mgmt_class_list_t;

typedef struct sys_acl_mgmt_ipSource_ipV6_s
{
    uint8    uaSIpv6Addr[CAPA_IPV6_BYTE_NUM];
    uint32  uiSIpv6PfLen ;
}sys_acl_mgmt_ipSource_ipV6_t;

typedef struct sys_acl_mgmt_ipSource_ipV4_s
{
    uint32  uiSIpv4Addr;
    uint32  uiSIpv4Netmask;
}sys_acl_mgmt_ipSource_ipV4_t;

typedef struct sys_acl_mgmt_careflag_s
{
    uint8 careipv4;
    uint8 careipv4mask;
    uint8 careipv6;
    uint8 carevlan;
}sys_acl_mgmt_careflag_t;

typedef struct sys_acl_mgmt_care_value_s
{
    sys_acl_mgmt_careflag_t  stCareFlag;
    sys_acl_mgmt_ipSource_ipV4_t stIpV4addr;
    sys_acl_mgmt_ipSource_ipV6_t stIpV6addr;
    sys_logic_portmask_t logicPortmask;
    sys_vid_t   vid;
    sys_acl_mgmt_care_service_t uiService;
}sys_acl_mgmt_care_value_t;

 typedef struct sys_acl_mgmt_ace_s
 {
    sys_acl_mgmt_action_t stAction;
    uint32 uiMgmtAcePri;
    sys_acl_mgmt_care_value_t stCare;
    sys_acl_mgmt_AceFlag_t stFlag;
    uint32 uiPoolNum;
    struct sys_acl_mgmt_ace_s   *pstMgmtAceNext;
  }sys_acl_mgmt_ace_t;

 typedef struct sys_acl_mgmt_list_s
 {
   char    chListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1];
   sys_acl_mgmt_ace_t  *pAce;
   sys_acl_mgmt_ListFlag_t stFlag;
   uint32 uiPoolNum;
   uint32 uiAceNum;
   sys_acl_mgmt_ListActive_t stIsActive;
   struct sys_acl_mgmt_list_s  *pNext;
  }sys_acl_mgmt_list_t;

 typedef struct sys_acl_mgmt_class_s
 {
    char    acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1];
  }sys_acl_mgmt_class_t;

 typedef struct sys_acl_mgmt_source_s
 {
    uint8 isIPv4;
    uint8 isIPv6;
    uint32  uiSIpv4Addr;
    uint8    uaSIpv6Addr[CAPA_IPV6_BYTE_NUM];
    sys_logic_port_t   sport;
    sys_vid_t   vid;
    uint16 uiTcpPort;
    uint16 uiUdpPort;
}sys_acl_mgmt_source_t;

extern int32 rsd_acl_mgmt_init(void);

extern int32 rsd_acl_mgmt_list_add(char *pcName);

extern int32 rsd_acl_mgmt_list_num_get(uint32 *pcListNum);

extern int32 rsd_acl_mgmt_list_acenum_get(char *pcName,uint32 *puiAceNum);

extern int32 rsd_acl_mgmt_all_acenum_get(uint32 *puiAceNum);

extern int32 rsd_acl_mgmt_list_del(char *pcName);

extern int32 rsd_acl_mgmt_ace_add( char *pcListName,  sys_acl_mgmt_ace_t  *pstAce);

extern int32 rsd_acl_mgmt_name_get(char *pcListName,uint32 idx);

extern int32 rsd_acl_mgmt_ace_get( char *pcListName,uint32 idx,sys_acl_mgmt_ace_t *pstAce);

extern int32 rsd_acl_mgmt_ace_del( char *pcListName, uint32 uiMgmtAcePri);

extern int32 rsd_acl_mgmt_class_add( char *pcListName);

extern  int32 rsd_acl_mgmt_class_get( char *pcListName);

extern  int32 rsd_acl_mgmt_class_del(void );

extern int32 rsd_acl_mgmt_exit(void);

#endif 

