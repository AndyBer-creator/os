#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libipc/vosMsg.h>
#include "smdMsg.h"

#include "smdCmdWrap.h"
#include "smdMsgApp.h"
SMD_WRAP_CMD *pg_smdRmWrapFun = NULL;
ulong_t g_ulSmdRMWrapNum = 0;
int smdRMWrapInit(ulong_t ulMsgNumMax)
{
    if ( NULL != pg_smdRmWrapFun )
    {
        
        return VOS_ERR_FAIL;
    }

    pg_smdRmWrapFun = malloc(ulMsgNumMax * sizeof(SMD_WRAP_CMD));
    if ( NULL == pg_smdRmWrapFun )
    {
        return VOS_ERR_FAIL;
    }

    memset(pg_smdRmWrapFun, 0, (ulMsgNumMax * sizeof(SMD_WRAP_CMD)));
    
    g_ulSmdRMWrapNum = ulMsgNumMax;

    return VOS_ERR_NONE;
}
int smdAppWrapInit(ulong_t ulMsgNumMax, SMD_WRAP_CMD* pSmdAppWrapFun)
{
    if ( NULL == pSmdAppWrapFun )
    {
        
        printf("\r\n %s %d app wrap fun == null", __func__, __LINE__);
        return VOS_ERR_FAIL;
    }
	printf("\r\n %s %d", __func__, __LINE__);
#if 0
    pSmdAppWrapFun = malloc(ulMsgNumMax * sizeof(SMD_WRAP_CMD));
    if ( NULL == pSmdAppWrapFun )
    {
    	osal_printf("\r\n %s %d", __func__, __LINE__);
        return VOS_ERR_FAIL;
    }
#endif
	printf("\r\n %s %d", __func__, __LINE__);

    memset(pSmdAppWrapFun, 0, (ulMsgNumMax * sizeof(SMD_WRAP_CMD)));
    printf("\r\n %s %d", __func__, __LINE__);
    return VOS_ERR_NONE;
}
int smdRmWrapRegister(ulong_t ulName, ulong_t ulFlags, PTRWRAPCMD func)
{
    if ( NULL == pg_smdRmWrapFun )
    {
        
        return VOS_ERR_FAIL;
    }
    
    if ( ulName >= g_ulSmdRMWrapNum )
    {
        
        return VOS_ERR_FAIL;
    }
    pg_smdRmWrapFun[ulName].ulFlags = ulFlags;
    pg_smdRmWrapFun[ulName].ptrFun = func;
	printf("\r\n %s %d", __func__, __LINE__);
    return VOS_ERR_NONE;
}
int smdCfgRmWrapProcess(SMD_MSG *pMsg)
{
    int iRet;
    
    SMD_ARG args[SMD_ARG_NUM_MAX];
    ulong_t ulArgsNum = SMD_ARG_NUM_MAX;
    SMD_WRAP_CMD *pWrapCmd = NULL;
#if 0
    if ( smdDebugIsOn(SMD_DEBUG_WRAP_DETAIL) )
    {
        int i;
        uchar_t *pStream = (uchar_t *)pMsg;

        iRecvLen = pMsg->usArgsBufLen + sizeof(SMD_MSG);
        
        sprintf(szDebugBuf, "%s:recv %d msg stream 0x", __func__, iRecvLen);
        iBufLen = strlen(szDebugBuf);

        for ( i = 0; i < iRecvLen; i++ )
        {
            sprintf(szBufTmp, "%02x", *(pStream + i));
            iTmpLen = strlen(szBufTmp);
            
            if ( (iBufLen + iTmpLen) >= SMD_DEUBG_BUF_LEN )
            {
                smdDebugPrintf(SMD_DEBUG_WRAP_DETAIL, szDebugBuf);
                sprintf(szDebugBuf, "%s:continue ", __func__);
                iBufLen = strlen(szDebugBuf);
            }
            
            strcat(szDebugBuf, szBufTmp);
            iBufLen += iTmpLen;
        }
        smdDebugPrintf(SMD_DEBUG_WRAP_DETAIL, szDebugBuf);
    }
#endif
    if ( SMD_MSG_TYPE_FRR != pMsg->usType )
    {
        
        return VOS_ERR_FAIL;
    }

    if ( pMsg->usName >= g_ulSmdRMWrapNum )
    {
        
        return VOS_ERR_FAIL;
    }

    if ( NULL == pg_smdRmWrapFun )
    {
        
        return VOS_ERR_FAIL;
    }

    pWrapCmd = &pg_smdRmWrapFun[pMsg->usName];
    
    if ( NULL == pWrapCmd->ptrFun )
    {
        
        return VOS_ERR_FAIL;
    }
    smdMsgStreamToArgs((uchar_t *)(pMsg + 1), pMsg->usArgsBufLen, args, &ulArgsNum);
    iRet = pWrapCmd->ptrFun(pMsg, args, ulArgsNum);

    pMsg->ulRet = iRet;

    return VOS_ERR_NONE;
}
int smdAppWrapRegister(ulong_t ulName, ulong_t ulFlags, PTRWRAPCMD func, SMD_WRAP_CMD* pCmdwrap, ulong_t ulSmdRMWrapNum)
{
    if ( NULL == pCmdwrap )
    {
        
        printf("\r %s %d pSmdWrap == NULL\n", __func__, __LINE__);
        return VOS_ERR_FAIL;
    }
    
    if ( ulName >= ulSmdRMWrapNum )
    {
        
        return VOS_ERR_FAIL;
    }
    pCmdwrap[ulName].ulFlags = ulFlags;
    pCmdwrap[ulName].ptrFun = func;
	
    return VOS_ERR_NONE;
}
int smdCfgAppWrapProcess(SMD_MSG *pMsg, SMD_WRAP_CMD* pSmdWrap, ulong_t ulSmdRMWrapNum)
{
    int iRet;
    
    SMD_ARG args[SMD_ARG_NUM_MAX];
    ulong_t ulArgsNum = SMD_ARG_NUM_MAX;
    SMD_WRAP_CMD *pWrapCmd = NULL;
#if 0
    if ( smdDebugIsOn(SMD_DEBUG_WRAP_DETAIL) )
    {
        int i;
        uchar_t *pStream = (uchar_t *)pMsg;

        iRecvLen = pMsg->usArgsBufLen + sizeof(SMD_MSG);
        
        sprintf(szDebugBuf, "%s:recv %d msg stream 0x", __func__, iRecvLen);
        iBufLen = strlen(szDebugBuf);

        for ( i = 0; i < iRecvLen; i++ )
        {
            sprintf(szBufTmp, "%02x", *(pStream + i));
            iTmpLen = strlen(szBufTmp);
            
            if ( (iBufLen + iTmpLen) >= SMD_DEUBG_BUF_LEN )
            {
                smdDebugPrintf(SMD_DEBUG_WRAP_DETAIL, szDebugBuf);
                sprintf(szDebugBuf, "%s:continue ", __func__);
                iBufLen = strlen(szDebugBuf);
            }
            
            strcat(szDebugBuf, szBufTmp);
            iBufLen += iTmpLen;
        }
        smdDebugPrintf(SMD_DEBUG_WRAP_DETAIL, szDebugBuf);
    }
#endif
    #if 0
    if ( SMD_MSG_TYPE_APP != pMsg->usType || SMD_MSG_TYPE_FRR != pMsg->usType)
    {
        
        return VOS_ERR_FAIL;
    }
    #endif
    if ( pMsg->usName >= ulSmdRMWrapNum )
    {
        
        return VOS_ERR_FAIL;
    }
    if ( NULL == pSmdWrap )
    {
        
        printf("\r %s %d pSmdWrap == NULL\n", __func__, __LINE__);
        return VOS_ERR_FAIL;
    }
    pWrapCmd = &pSmdWrap[pMsg->usName];
    
    if ( NULL == pWrapCmd->ptrFun )
    {
        
        return VOS_ERR_FAIL;
    }
    smdMsgStreamToArgs((uchar_t *)(pMsg + 1), pMsg->usArgsBufLen, args, &ulArgsNum);
    iRet = pWrapCmd->ptrFun(pMsg, args, ulArgsNum);

    pMsg->ulRet = iRet;

    return VOS_ERR_NONE;
}

