#include <stdio.h>
#include <string.h>
#include <libipc/vos.h>
#include <libipc/vosMsg.h>
#include <libipc/vosSystem.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/smdProcess.h>
#include "smdMsg.h"
#include "smdCmdWrap.h"
#include "smdNetlink.h"
uchar_t g_smdMsgSendBuffer[SMD_MSG_BUFFER_LEN]; 
uchar_t g_smdMsgRecvBuffer[SMD_MSG_BUFFER_LEN]; 
int smdMsgCheck(void)
{
#ifdef GLN_SDK_KERNEL
    ulong_t ulHeaderLen = 0;
#endif

    vosAssert(0 == ((ulong_t)g_smdMsgSendBuffer)%sizeof(long));
    vosAssert(0 == ((ulong_t)g_smdMsgRecvBuffer)%sizeof(long));
    vosAssert(0 == sizeof(SMD_MSG)%sizeof(long));

#ifdef GLN_SDK_KERNEL
    smdMsgNetlinkHeaderLen(&ulHeaderLen);
    vosAssert(0 == ulHeaderLen%sizeof(long));
#endif

    return VOS_ERR_NONE;
}
static int smdMsgSendBufferGet(SMD_MSG_TYPE msgType, uchar_t **ppBuffer, ulong_t *pulBufLen)
{
    ulong_t ulHeaderLen = 0;
    
#ifdef GLN_SDK_KERNEL
    if ( (SMD_MSG_TYPE_DIRECTCONFIG == msgType) || (SMD_MSG_TYPE_INTERNAL == msgType) )
    {
        smdMsgNetlinkHeaderLen(&ulHeaderLen);
    }
#endif

    *ppBuffer = (g_smdMsgSendBuffer + ulHeaderLen);
    *pulBufLen = (SMD_MSG_BUFFER_LEN - ulHeaderLen);

    return VOS_ERR_NONE;
}
int smdMsgInit(SMD_MSG *pMsg, SMD_MSG_TYPE type, ushort_t usName, 
    ulong_t ulPort, ulong_t ulFlags)
{
    if ( NULL == pMsg )
    {
        
        return VOS_ERR_PARAM;
    }

    if ( 0 == (ulFlags & (SMD_MSG_INFO | SMD_MSG_SET | SMD_MSG_GET)) )
    {
        
        return VOS_ERR_PARAM;
    }

    memset(pMsg, 0, sizeof(SMD_MSG));

    pMsg->usType = type;
    pMsg->usName = usName;
    pMsg->usPort = ulPort;
    if(type == SMD_MSG_TYPE_TRAP)
    {
		pMsg->ucNeedRet = SMD_MSG_RET_NOT;
    }
    else 
    {
    	pMsg->ucNeedRet = SMD_MSG_RET_NEED;
    }
    pMsg->ulFlags = ulFlags;

    return VOS_ERR_NONE;
}
static int smdMsgArgsToStream(SMD_ARG args[], ulong_t ulArgsNum, 
    uchar_t *pStream, ushort_t *pusStreamLen)
{
    ulong_t i;
    ulong_t ulBufUsedLen = 0, ulBufLen = *pusStreamLen, ulTmp;
    SMD_ARG *pArgTmp = NULL;
    uchar_t *pBufTmp;
    if ( 0 != ulArgsNum )
    {
        ulBufUsedLen = 0;
        
        for ( i = 0; i < ulArgsNum; i++ )
        {
            pBufTmp = (pStream + ulBufUsedLen);
            
            ulBufUsedLen += (SMD_ARG_HEADER_LEN + args[i].usLen);

            if ( ulBufUsedLen > ulBufLen )
            {
                
                return VOS_ERR_FAIL;
            }

            memcpy(pBufTmp, &args[i], SMD_ARG_HEADER_LEN);
            if ( SMD_ARG_POINTER & args[i].usFlags )
            {
                
                if ( NULL ==  args[i].pBuf )
                {
                    pArgTmp = (SMD_ARG *)pBufTmp;
                    pArgTmp->usLen = 0;

                    ulBufUsedLen -= args[i].usLen;

                    continue;
                }
            }
            memcpy(pBufTmp + SMD_ARG_HEADER_LEN, args[i].pBuf, args[i].usLen);
            if ( i != (ulArgsNum - 1) )
            {
                ulTmp = (args[i].usLen % 4);
                if ( 0 != ulTmp )
                {
                    ulBufUsedLen += (4 - ulTmp);
                }
            }
        }
    }

    *pusStreamLen = ulBufUsedLen;

    return VOS_ERR_NONE;
}
static int smdMsgStreamParseArgs(SMD_ARG args[], ulong_t ulArgsNum, 
    uchar_t *pStream, ulong_t ulStreamLen)
{
    ulong_t i;
    ulong_t ulBufUsedLen = 0, ulTmp;
    SMD_ARG *pArgTmp = NULL;
    uchar_t *pBufTmp = pStream;
    if ( 0 != ulArgsNum )
    {
        for ( i = 0; i < ulArgsNum; i++ )
        {
            pArgTmp = (SMD_ARG *)(pBufTmp + ulBufUsedLen);
            if ( SMD_ARG_OUTPUT & args[i].usFlags )
            {
                if ( NULL != args[i].pBuf )
                {
                   memcpy(args[i].pBuf, pBufTmp + ulBufUsedLen + SMD_ARG_HEADER_LEN, 
                                pArgTmp->usLen);
                }
            }

            ulBufUsedLen += (SMD_ARG_HEADER_LEN + pArgTmp->usLen);
            if ( ulBufUsedLen >= ulStreamLen )
            {
                return VOS_ERR_NONE;
            }
            ulTmp = (pArgTmp->usLen % 4);
            if ( 0 != ulTmp )
            {
                ulBufUsedLen += (4 - ulTmp);
            }
        }
    }

    return VOS_ERR_NONE;
}
static int smdMsgSendDirectConfig(uchar_t *pStream, ulong_t ulStreamLen, ulong_t ulTimeOut)
{
#ifdef GLN_SDK_KERNEL    
    return smdMsgNetlinkRequest(pStream, ulStreamLen, ulTimeOut);
#else
    int iMsgRet;
    VOS_MSG_RET vosNeedRet;
    SMD_MSG *pMsg;

    pMsg = (SMD_MSG *)pStream;
    if ( SMD_MSG_RET_NEED == pMsg->ucNeedRet )
    {
        vosNeedRet = VOS_MSG_RET_NEED;
    }
    else
    {
        vosNeedRet = VOS_MSG_RET_NOT;
    }

    return vosMsgSend(SMD_PROCESS_NAME, (void *)pStream, ulStreamLen, vosNeedRet, 
        &iMsgRet, ulTimeOut, VOS_MSG_FLAG_NONE);
#endif
}
static int smdMsgSendRM(uchar_t *pStream, ulong_t ulStreamLen, ulong_t ulTimeOut)
{
    int iRet, iMsgRet;
    VOS_MSG_RET vosNeedRet;
    SMD_MSG *pMsg;

    pMsg = (SMD_MSG *)pStream;
    if ( SMD_MSG_RET_NEED == pMsg->ucNeedRet )
    {
        vosNeedRet = VOS_MSG_RET_NEED;
    }
    else
    {
        vosNeedRet = VOS_MSG_RET_NOT;
    }

    iRet = vosMsgSend(SMD_PROCESS_NAME, (void *)pStream, ulStreamLen, vosNeedRet, 
        &iMsgRet, ulTimeOut, VOS_MSG_FLAG_NONE);
    
    return iRet;
}
static int smdMsgSendInternal(uchar_t *pStream, ulong_t ulStreamLen, ulong_t ulTimeOut)
{
#ifdef GLN_SDK_KERNEL    
        return smdMsgNetlinkRequest(pStream, ulStreamLen, ulTimeOut);
#else
        
        return 0;
#endif    
}
int smdMsgRequest(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum, ulong_t ulTimeOut)
{
    int iRet; 
    ulong_t  ulBufLen = 0, ulBufUsedLen = 0;
    uchar_t *pMsgBuf = NULL, *pBufTmp;
    SMD_MSG *pMsgRet = NULL;
    if ( (NULL ==pMsg) || ((0 != ulArgsNum) && (NULL == args)) )
    {
        
        return VOS_ERR_PARAM;
    }

    if ( ulArgsNum > SMD_ARG_NUM_MAX )
    {
        
        return VOS_ERR_FAIL;
    }
    if ( VOS_ERR_NONE != smdMsgSendBufferGet(pMsg->usType, &pMsgBuf, &ulBufLen) )
    {
        
        return VOS_ERR_FAIL;
    }

    ulBufUsedLen += sizeof(SMD_MSG);        
    pBufTmp = (pMsgBuf + ulBufUsedLen);    
    pMsg->usArgsBufLen = (ulBufLen - ulBufUsedLen);
    pMsg->usArgsNum = ulArgsNum;

    iRet = smdMsgArgsToStream(args, ulArgsNum, pBufTmp, &pMsg->usArgsBufLen);
    if ( VOS_ERR_NONE != iRet )
    {
        
        return iRet;
    }
    
    memcpy(pMsgBuf, pMsg, ulBufUsedLen);
    ulBufUsedLen += pMsg->usArgsBufLen;
#if 0
    
    if ( smdDebugIsOn(SMD_DEBUG_MSG_DETAIL) )
    {
        ulong_t i;

        sprintf(szDebugBuf, "%s:send %ld msg stream 0x", __func__, ulBufUsedLen);
        iBufLen = strlen(szDebugBuf);

        for ( i = 0; i < ulBufUsedLen; i++ )
        {
            sprintf(szBufTmp, "%02x", *(pMsgBuf + i));
            iTmpLen = strlen(szBufTmp);
            
            if ( (iBufLen + iTmpLen) >= SMD_DEUBG_BUF_LEN )
            {
                smdDebugPrintf(SMD_DEBUG_MSG_DETAIL, szDebugBuf);
                sprintf(szDebugBuf, "%s:continue ", __func__);
                iBufLen = strlen(szDebugBuf);
            }
            
            strcat(szDebugBuf, szBufTmp);
            iBufLen += iTmpLen;
        }
        smdDebugPrintf(SMD_DEBUG_MSG_DETAIL, szDebugBuf);
    }
#endif
    
    switch ( pMsg->usType )
    {
        case SMD_MSG_TYPE_APP:
            iRet = smdMsgSendDirectConfig(pMsgBuf, ulBufUsedLen, ulTimeOut);
            break;

        case SMD_MSG_TYPE_FRR:
            iRet = smdMsgSendRM(pMsgBuf, ulBufUsedLen, ulTimeOut);
            break;

        case SMD_MSG_TYPE_INTERNAL:
            iRet = smdMsgSendInternal(pMsgBuf, ulBufUsedLen, ulTimeOut);
            break;

    #ifndef GLN_SDK_KERNEL
        case SMD_MSG_TYPE_VI:
            
            break;
    #endif

        default:
            iRet = VOS_ERR_PARAM;
            
            return iRet;
    }
    if ( VOS_ERR_NONE != iRet )
    {
        
        return iRet;
    }

    if ( SMD_MSG_RET_NOT == pMsg->ucNeedRet )
    {
        return VOS_ERR_NONE;
    }

    pMsgRet = (SMD_MSG *)pMsgBuf;

    if ( SMD_MSG_RET_NEED == pMsgRet->ucNeedRet )
    {
        
        smdMsgStreamParseArgs(args, ulArgsNum, pBufTmp, pMsg->usArgsBufLen);
    }
    return pMsgRet->ulRet;
}
int smdMsgRequestByName(char *name, SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum, ulong_t ulTimeOut)
{
    int iRet, iMsgRet;
    VOS_MSG_RET vosNeedRet;
    
    ulong_t  ulBufLen = 0, ulBufUsedLen = 0;
    uchar_t *pMsgBuf = NULL, *pBufTmp;
    SMD_MSG *pMsgRet = NULL;
    if ( (NULL == name) || (NULL ==pMsg) || ((0 != ulArgsNum) && (NULL == args)) )
    {
        
        return VOS_ERR_PARAM;
    }
#if 0
    if ( SMD_MSG_TYPE_RM != pMsg->usType )
    {
        
        return VOS_ERR_FAIL;
    }
#endif
    if ( ulArgsNum > SMD_ARG_NUM_MAX )
    {
        
        return VOS_ERR_FAIL;
    }
    if ( VOS_ERR_NONE != smdMsgSendBufferGet(pMsg->usType, &pMsgBuf, &ulBufLen) )
    {
        
        return VOS_ERR_FAIL;
    }

    ulBufUsedLen += sizeof(SMD_MSG);        
    pBufTmp = (pMsgBuf + ulBufUsedLen);    
    pMsg->usArgsBufLen = (ulBufLen - ulBufUsedLen);
    pMsg->usArgsNum = ulArgsNum;

    iRet = smdMsgArgsToStream(args, ulArgsNum, pBufTmp, &pMsg->usArgsBufLen);
    if ( VOS_ERR_NONE != iRet )
    {
        return iRet;
    }
    
    memcpy(pMsgBuf, pMsg, ulBufUsedLen);
    ulBufUsedLen += pMsg->usArgsBufLen;
#if 0
    
    if ( smdDebugIsOn(SMD_DEBUG_MSG_DETAIL) )
    {
        ulong_t i;

        sprintf(szDebugBuf, "%s:send %ld msg stream 0x", __func__, ulBufUsedLen);
        iBufLen = strlen(szDebugBuf);

        for ( i = 0; i < ulBufUsedLen; i++ )
        {
            sprintf(szBufTmp, "%02x", *(pMsgBuf + i));
            iTmpLen = strlen(szBufTmp);
            
            if ( (iBufLen + iTmpLen) >= SMD_DEUBG_BUF_LEN )
            {
                
                sprintf(szDebugBuf, "%s:continue ", __func__);
                iBufLen = strlen(szDebugBuf);
            }
            
            strcat(szDebugBuf, szBufTmp);
            iBufLen += iTmpLen;
        }
        
    }
#endif
	if(VOS_TIMEOUT_NOWAIT == ulTimeOut)
	{
		pMsg->ucNeedRet = VOS_MSG_RET_NOT;
	}
    if ( SMD_MSG_RET_NEED == pMsg->ucNeedRet )
    {
        vosNeedRet = VOS_MSG_RET_NEED;
    }
    else
    {
        vosNeedRet = VOS_MSG_RET_NOT;
    }
    iRet = vosMsgSend(name, (void *)pMsgBuf, ulBufUsedLen, vosNeedRet, &iMsgRet, ulTimeOut, VOS_MSG_FLAG_NONE);
    if ( VOS_ERR_NONE != iRet )
    {
        
        return iRet;
    }

    if ( SMD_MSG_RET_NOT == pMsg->ucNeedRet )
    {
        return VOS_ERR_NONE;
    }

    pMsgRet = (SMD_MSG *)pMsgBuf;

    if ( SMD_MSG_RET_NEED == pMsgRet->ucNeedRet )
    {
        
        smdMsgStreamParseArgs(args, ulArgsNum, pBufTmp, pMsg->usArgsBufLen);
    }
	pMsgRet->ulRet = vos_to_sys_errId_ret(pMsgRet->ulRet);
    return pMsgRet->ulRet;
}

