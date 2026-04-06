
#ifndef __KSI_CORE_H__
#define __KSI_CORE_H__

#include <common/sys_def.h>
#include <common/sys_log.h>
#include <common/sys_portview.h>
#include <common/sys_util.h>
#include <ksi/ksi_lkm.h>

#define KSI_MAX_PAYLOAD 1024

typedef enum ksi_msgtype_e
{
    KSI_MSGTYPE_INIT = 0,     
    KSI_MSGTYPE_NOTIFY,       
    KSI_MSGTYPE_SEND,         
    KSI_MSGTYPE_SEND_FIN,     
    KSI_MSGTYPE_REPLY,        
    KSI_MSGTYPE_REPLY_FIN,    
    KSI_MSGTYPE_END
} ksi_msgtype_t;

typedef struct ksi_hdr_s
{
    uint32         cmd;
    ksi_msgtype_t  msgtype;
    uint16         tousr_totlen;
    uint16         tokrn_totlen;
    uint16         curr_len;
    uint16         curr_offset;
} ksi_hdr_t;

typedef struct ksi_msg_s
{
    uint32    id;
    ksi_hdr_t hdr;
    uint8     *tousr_data;
    uint8     *tokrn_data;
} ksi_msg_t;

typedef struct ksi_msg_list_s
{
    ksi_msg_t *msg;
    struct ksi_msg_list_s *next;
} ksi_msg_list_t;

#define KSI_NOTIFY_MSG(__cmd)                   \
do {                                            \
    ksi_msg_t __msg;                            \
    osal_memset(&__msg, 0, sizeof(ksi_msg_t));  \
    __msg.hdr.msgtype = KSI_MSGTYPE_NOTIFY;     \
    __msg.hdr.cmd = __cmd;                      \
    ksi_msg_notify(&__msg);                     \
} while(0)

#define KSI_NOTIFY_MSG_DATA(__cmd, __datalen, __pData) \
do {                                                   \
    ksi_msg_t __msg;                                   \
    osal_memset(&__msg, 0, sizeof(ksi_msg_t));         \
    __msg.hdr.msgtype = KSI_MSGTYPE_SEND;              \
    __msg.hdr.cmd = __cmd;                             \
    __msg.hdr.tousr_totlen = __datalen;                \
    __msg.tousr_data = (void*)__pData;                 \
    ksi_msg_notify(&__msg);                            \
} while(0)

#define KSI_NOTIFY_MSG_DATA_RET(__pMsg) (__pMsg->tousr_data)

extern int32 ksi_msg_notify(ksi_msg_t *pMsg);

extern int32 ksi_msg_add(ksi_msg_t *pMsg);

extern int32 ksi_msg_del(ksi_msg_t *pMsg);

#endif  

