
#ifndef _INCLUDE_SMDMSGREQUEST_H_
#define _INCLUDE_SMDMSGREQUEST_H_

#include <libipc/vos.h>

#define SMD_MSG_BUFFER_LEN (2048)

extern uchar_t g_smdMsgRecvBuffer[SMD_MSG_BUFFER_LEN]; 

typedef enum SMD_MSG_TYPE_tag{
    SMD_MSG_TYPE_APP,
    SMD_MSG_TYPE_FRR, 
    SMD_MSG_TYPE_TRAP, 
    SMD_MSG_TYPE_INTERNAL, 
    SMD_MSG_TYPE_VI, 
    SMD_MSG_TYPE_MAX
}SMD_MSG_TYPE;

typedef enum SMD_MSG_FLAG_tag{
    SMD_MSG_INFO               = 0x01,  
    SMD_MSG_SET                = 0x02,  
    SMD_MSG_GET                = 0x04,  
    SMD_MSG_PRI_HIGH          = 0x08,  
    SMD_MSG_MAX
}SMD_MSG_FLAG;

#define SMD_GLOBAL 0 

typedef enum SMD_MSG_RET_tag{
    SMD_MSG_RET_NEED     = 0, 
    SMD_MSG_RET_NOT           
}SMD_MSG_RET;

typedef struct SMD_MSG_tag{
    ushort_t        usType; 
    ushort_t        usName; 
    ulong_t         ulRet;
    ushort_t        usPort;
    ushort_t        usArgsBufLen; 
    ushort_t        usArgsNum; 
    uchar_t         pad[1];
    uchar_t         ucNeedRet;
    ulong_t         ulFlags; 
}SMD_MSG;

typedef enum SMD_ARG_FLAG_tag{
    SMD_ARG_POINTER           = 0x01, 
    SMD_ARG_OUTPUT            = 0x02, 
    SMD_ARG_MAX
}SMD_ARG_FLAG;

#define SMD_ARG_HEADER_LEN  4

#define SMD_ARG_NUM_MAX 16 

typedef struct SMD_ARG_tag{
    ushort_t        usFlags;
    ushort_t        usLen;
    void           *pBuf; 
}SMD_ARG;

typedef int (*PTRWRAPCMD)(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);

#define ARG_DEF(num) SMD_ARG args[num]
#define ARG_GET() args
#define ARG_ADD(index, buf, flags, len) \
do \
{ \
    args[index].pBuf = buf; \
    args[index].usFlags = flags; \
    args[index].usLen = len; \
}while(0)

int smdMsgCheck(void);

int smdMsgInit(SMD_MSG *pMsg, SMD_MSG_TYPE type, ushort_t usName, 
    ulong_t ulPort, ulong_t ulFlags);

int smdMsgRequest(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum, ulong_t ulTimeOut);

int smdMsgRequestByName(char *name, SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum, ulong_t ulTimeOut);

int smdRMWrapInit(ulong_t ulMsgNumMax);

int smdRmWrapRegister(ulong_t ulName, ulong_t ulFlags, PTRWRAPCMD func);

int smdCfgRmWrapProcess(SMD_MSG *pMsg);
#endif 

