
#ifndef _INCLUDE_SMDCMDWRAP_H_
#define _INCLUDE_SMDCMDWRAP_H_

#include <libipc/vos.h>
#include <libipc/smdMsgRequest.h>

typedef struct SMD_WRAP_CMD_tag{
    ulong_t  ulFlags; 
    PTRWRAPCMD ptrFun;
}SMD_WRAP_CMD;

int smdCfgWrapRegister(ulong_t ulName, ulong_t ulFlags, PTRWRAPCMD func);
int smdCfgInternalWrapRegister(ulong_t ulName, ulong_t ulFlags, PTRWRAPCMD func);
int smdRmWrapRegister(ulong_t ulName, ulong_t ulFlags, PTRWRAPCMD func);

int smdCfgWrapProcess(SMD_MSG *pMsg);
int smdCfgInternalWrapProcess(SMD_MSG *pMsg);
int smdCfgRmWrapProcess(SMD_MSG *pMsg);
int smdCfgViWrapProcess(SMD_MSG *pMsg);

int smdCfgWrapFunction (void);
int smdCfgInternalWrapFunction (void);
int smdRmWrapFunction (void);
int smdCfgViWrapFunction (void);
int smdAppWrapInit(ulong_t ulMsgNumMax, SMD_WRAP_CMD* pSmdAppWrapFun);
int smdAppWrapRegister(ulong_t ulName, ulong_t ulFlags, PTRWRAPCMD func, SMD_WRAP_CMD* pCmdwrap, ulong_t ulSmdRMWrapNum);
int smdCfgAppWrapProcess(SMD_MSG *pMsg, SMD_WRAP_CMD* pSmdWrap, ulong_t ulSmdRMWrapNum);

#endif 

