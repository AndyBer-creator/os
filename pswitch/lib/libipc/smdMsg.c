#ifdef MAKE_KERNEL_MODULE
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "vos.h"
#include <libipc/vosMsg.h>
#include "smdMsg.h"
int smdMsgStreamToArgs(uchar_t *pStream, ulong_t ulStremLen, SMD_ARG args[], ulong_t *pulArgsNum)
{
    ulong_t i;
    ulong_t ulArgsMaxNum = *pulArgsNum, ulArgsNum = 0;
    ulong_t ulBufUsedLen = 0, ulTmp;
    SMD_ARG *pArgTmp = NULL;
    for ( i = 0; i < ulArgsMaxNum; i++ )
    {
        pArgTmp = (SMD_ARG *)(pStream + ulBufUsedLen);
        
        ulBufUsedLen += (SMD_ARG_HEADER_LEN + pArgTmp->usLen);
        if ( ulBufUsedLen > ulStremLen )
        {
            goto done;
        }

        memcpy(&args[i], pArgTmp, SMD_ARG_HEADER_LEN);
        
        if ( (SMD_ARG_POINTER & pArgTmp->usFlags) && (0 == pArgTmp->usLen) )
        {
            args[i].pBuf = NULL;
        }
        else
        {
            args[i].pBuf = (uchar_t *)pArgTmp + SMD_ARG_HEADER_LEN;
        }
        
        ulArgsNum++;
        ulTmp = (pArgTmp->usLen % 4);
        if ( 0 != ulTmp )
        {
            ulBufUsedLen += (4 - ulTmp);
        }
    }

done:
    *pulArgsNum = ulArgsNum;
    
    return VOS_ERR_NONE;
}

