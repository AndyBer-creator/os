
#ifndef _INCLUDE_VOSMSG_H_
#define _INCLUDE_VOSMSG_H_

#include <libipc/vos.h>
#ifdef VOS_MSG_PRIORITY_SUPPORT
#define VOS_MSG_HIGH_SUFFIX "High"
#endif

#define VOS_MSG_SOCKET_DIR "/var/run"

#define VOS_LOCAL_FILE_MAX_LEN 100

typedef enum VOS_MSG_SOURCE_tag{
  VOS_MSG_SOURCE_PROCESS           = 0x01,  
  VOS_MSG_SOURCE_KERNEL_UNICAST    = 0x02, 
  VOS_MSG_SOURCE_KERNEL_MULTICAST  = 0x04, 
  VOS_MSG_SOURCE_ALL               = 0xFF    
}VOS_MSG_SOURCE;

typedef enum VOS_MSG_RET_tag{
    VOS_MSG_RET_NEED     = 0, 
    VOS_MSG_RET_NOT           
}VOS_MSG_RET;

typedef enum VOS_MSG_FLAG_tag{
    VOS_MSG_FLAG_NONE              = 0x00, 
    VOS_MSG_FLAG_URGENT            = 0x01,    
    
    VOS_MSG_FLAG_SYN               = 0x02,    
    VOS_MSG_FLAG_MAX               = 0xFF
}VOS_MSG_FLAG;

typedef enum VOS_TIMER_TYPE_tag{
  VOS_TIMER_TYPE_ONE         = 0x0,   
  VOS_TIMER_TYPE_ALWAYS               
}VOS_TIMER_TYPE;

#define VOS_CLIENT_ADDR_LEN 150

typedef struct VOS_MSG_ADDR_tag{
    uchar_t addr[VOS_CLIENT_ADDR_LEN];
    int iSocketLen;
    ulong_t ulNeedRet;
    int iVosFd; 
}VOS_MSG_ADDR;

#define SMD_NETLINK_CMD 29 
#ifdef VOS_NETLINK_INCLUDE
#define VOS_NL_UI_IMP   30 

#undef VOS_NETLINK_MULTI_INCLUDE

#ifdef VOS_NETLINK_MULTI_INCLUDE
#define VOS_NL_MU_IMP   31 
#define VOS_ML_GROUP 1
#endif

#endif

#define VOS_TIMEOUT_FOREVER    0xFFFFFFFF  
#define VOS_TIMEOUT_DEFAULT    3000 
#define VOS_TIMEOUT_NOWAIT     0

#define VOS_PORT_GLOBAL     0 

typedef enum VOS_LINK_STATUS_tag{
    VOS_LINK_DOWND        = 0x0,    
    VOS_LINK_UP                      
}VOS_LINK_STATUS;

typedef enum VOS_PORT_TYPE_tag{
    VOS_PORT_TYPE_AP,          
    VOS_PORT_TYPE_AG,          
    VOS_PORT_TYPE_IP           
}VOS_PORT_TYPE;

typedef enum VOS_MSG_TYPE_tag{
  VOS_MSG_TYPE_BLDRUN,                
  VOS_MSG_TYPE_PACKET,                
  VOS_MSG_TYPE_LINK,                  
  VOS_MSG_TYPE_CCD,                   
  VOS_MSG_TYPE_CCD_OBJ_INFO_GET,      
  VOS_MSG_VOS_DEBUG,                  
  VOS_MSG_SMD_DEBUG,                  
  VOS_MSG_TYPE_SELF                   
}VOS_MSG_TYPE;

typedef struct VOS_MSG_tag{
  ulong_t        ulType;              
  ulong_t        ulNeedRet;           
  ulong_t        ulRet;               
  ulong_t        ulValue1;            
  ulong_t        ulValue2;            
  ulong_t        ulValue3;            
  ulong_t        ulValue4;            
  ulong_t        ulBufferLen;         
}VOS_MSG;

typedef int (*vosMsgUserSocketProcess)(int iSocketFd, void *pArg);

typedef struct VOS_SOCK_USER_tag{
    struct VOS_SOCK_USER_tag *pNext;
    int iSocketFd;
    vosMsgUserSocketProcess process;
    void *pArg;
}VOS_SOCK_USER;

#define BLDRUN_FLAG_USER_OFF            16 

typedef int (*vosMsgBldrunProcess) (char *pszBuf, int *pnBufLen, 
    ulong_t ulFlags, ulong_t ulPort);

typedef int (*vosTimerProcess)(ulong_t ulTimerOver, void *pArg);

typedef int (*vosMsgPortLinkProcess)(ulong_t ulPort, VOS_LINK_STATUS linkStatus);

typedef int (*vosMsgVlanintfLinkProcess)(ulong_t bSuperVlan, ulong_t ulID, 
    VOS_LINK_STATUS linkStatus);

typedef int (*vosMsgCcdProcess)(ulong_t request, char *pObjName, ulong_t *pIndex, 
        ulong_t *pIndexSubIdCount, void *pValueBuf, ulong_t *pValueLen, ushort_t *flags);

typedef int (*vosMsgCcdObjInfoGetProcess)(
    char         *pObjName, 
    ulong_t      valueMask,
    ulong_t      *pulLocalId,               
    ulong_t      *pulLastOidPrefixSubId,    
    ulong_t      *pulSize,                  
    ulong_t      *pulRangeMin,              
    ulong_t      *pulRangeMax,              
    int          *pnType,                   
    ulong_t      pOidPrefixWoLastSubId[],   
    ulong_t      *pulOidPrefixLen,          
    ulong_t      *pulIndexObjCount,         
    ulong_t      *pulIndexSubIdCount,       
    ulong_t      *pulIndexSubIdCountMax     
    );

int vosMsgCheck(void);

int vosMsgInit(char *name, VOS_MSG_SOURCE msgSrc, int *iHandle);

int vosMsgLocalSocket(ulong_t ulTimeOut);

int vosMsgLocalAddrInit(char *name, void *localAddr, int *piAddrLen);

int vosMsgRecv(int iHandle, void *pMsg, ulong_t ulMsgSize, ulong_t ulTimeOut, 
    VOS_MSG_ADDR *pClientAddr);

int vosMsgResp(int iHandle, int iRet, void *pMsg, ulong_t ulMsgSize, 
    VOS_MSG_ADDR *pClientAddr);

int vosMsgSend(char *name, void *pMsg, ulong_t ulMsgSize, 
    VOS_MSG_RET needRet, int *piRet, ulong_t ulTimeOut, ulong_t ulFlags);

int vosMsgRegisterPortLink(VOS_PORT_TYPE portType, vosMsgPortLinkProcess pProc);

int vosMsgRegisterVlanintfLink(vosMsgVlanintfLinkProcess pProc);

int vosMsgRegisterBldrun(vosMsgBldrunProcess pProc);

int vosMsgRegisterCcd(vosMsgCcdProcess pProc);

int vosMsgRegisterCcdObjInfoGet(vosMsgCcdObjInfoGetProcess pProc);

int vosMsgBldrunSend(char *name, char *pszBuf, int *pnBufLen, 
    ulong_t ulFlags, ulong_t ulPort);

int vosMsgCcdRequestSend(ulong_t *piRet, char *pProcName, 
    ulong_t request, char *pObjName, ulong_t *pIndex, ulong_t *pIndexSubIdCount, 
    void *pValueBuf, ulong_t *pValueLen, ushort_t *flags);

int vosMsgCcdObjInfoGetSend(
    ulong_t       *piRet, 
    char         *pProcName, 
    char         *pObjName, 
    ulong_t      valueMask,
    ulong_t      *pulLocalId,               
    ulong_t      *pulLastOidPrefixSubId,    
    ulong_t      *pulSize,                  
    ulong_t      *pulRangeMin,              
    ulong_t      *pulRangeMax,              
    int          *pnType,                   
    ulong_t      pOidPrefixWoLastSubId[],   
    ulong_t      *pulOidPrefixLen,          
    ulong_t      *pulIndexObjCount,         
    ulong_t      *pulIndexSubIdCount,       
    ulong_t      *pulIndexSubIdCountMax     
    );

int vosTimerRegister(VOS_TIMER_TYPE type, ulong_t ulTime, vosTimerProcess pProc, 
    ulong_t **ppulTimerHande, void *pArg);

int vosTimerCancel(ulong_t *pulTimerHande);

int vosTimerStart(ulong_t *pulTimerHande, ulong_t ulTime);

int vosTimerStop(ulong_t *pulTimerHande);

ulong_t vosMsgDebugSmdGet(void);

int vosMsgDebugSmdSet(ulong_t ulFlags);

int vosMsgSockAdd(int iSocketFd, vosMsgUserSocketProcess process, void *pArg);

int vosMsgSockDelete(int iSocketFd);

int vosMsgSockAddUser(VOS_SOCK_USER *pSocketNew);

int vosMsgSockDelUser(VOS_SOCK_USER *pSocketDel);

int vosMsgclientChangePath(char *name, char *newname);

int vos_to_sys_errId_ret(int vosErr);

#endif 

