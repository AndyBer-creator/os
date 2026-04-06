
#ifndef __PROTO_ISG_H__
#define __PROTO_ISG_H__
#include <common/sys_def.h>
#define IPSG_ENTRY_UNLIMITED     -1

typedef enum sys_isg_db_type_e
{
    ISG_DB_TYPE_NONE = 0,
    ISG_DB_TYPE_FLASH,
    ISG_DB_TYPE_TFTP,
    ISG_DB_TYPE_END,
}sys_isg_db_type_t;

typedef enum sys_isg_entry_type_e
{
    ISG_ENTRY_STATIC = 0,
    ISG_ENTRY_DYNAMIC,
    ISG_ENTRY_ALL_DENY,
    ISG_ENTRY_END,
}sys_isg_entry_type_t;

typedef enum sys_isg_verify_type_e
{
    ISG_VERIFY_IP = 0,
    ISG_VERIFY_IP_MAC,
    ISG_VERIFY_END,
}sys_isg_verify_type_t;

typedef enum sys_isg_entry_filter_del_e
{
    ISG_ENTRY_DEL_ALL = 0,
    ISG_ENTRY_DEL_VID,
    ISG_ENTRY_DEL_IP,
    ISG_ENTRY_DEL_PORT,
    ISG_ENTRY_DEL_END,
}sys_isg_entry_filter_del_t;

typedef struct sys_isg_entry_s
{
    sys_mac_t mac;
    sys_vid_t vid;
    sys_ipv4_t ip;
    sys_ipv4_t mask;
    sys_logic_port_t port;
    int32 leaseTime;

    uint8 isUsed;
    uint8 isCareMac;
    sys_isg_entry_type_t type;
} sys_isg_entry_t;
typedef struct sys_isg_db_s
{
    uint32 timeout;
    uint32 writeDelay;
    sys_isg_db_type_t dbType;
    char server[CAPA_HOSTNAME_LEN + 1];
    char filename[CAPA_FILE_NAME_LEN+1];
} sys_isg_db_t;
extern int32 isg_bindingMaxEntryNum_get(uint32 *pMaxEntryNum);
extern int32 isg_port_enable_set(sys_logic_port_t port, sys_enable_t enable, sys_isg_verify_type_t verifyType);
extern int32 isg_port_maxEntryNum_set(sys_logic_port_t port, int32 num);
extern int32 isg_port_currentEntryNum_get(sys_logic_port_t port, uint32 *pNum);
extern int32 isg_bindingEntryByIndex_get(uint16 entryIdx, sys_isg_entry_t *pBindEntry);
extern int32 isg_bindingEntry_set(sys_isg_entry_t *pEntry, char isFromDhcp);
extern int32 isg_bindingEntry_del(sys_isg_entry_t *pEntry);
extern int32 isg_bindingEntryDhcp_del(sys_isg_entry_filter_del_t filterType, uint32 val);
extern int32 isg_bindingEntry_exist(sys_isg_entry_t *pEntry, uint8 isCarePort);
extern int32 isg_db_set(sys_isg_db_t *pDB);
extern int32 isg_db_timer_set(uint32 timer);
extern int32 isg_db_timer_get(uint32 *pTimer);
extern int32 isg_init(void);
extern int32 isg_exit(void);
#endif

