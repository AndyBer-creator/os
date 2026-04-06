/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 16227 $
 * $Date: 2011-03-10 18:23:52 +0800 (Thu, 10 Mar 2011) $
 *
 * Purpose : Polling some events and information
 *
 * Feature : Polling some events and information
 */

/*
 * Include Files
 */
#include <signal.h>
#include <dirent.h>
#include <libsal/sal_sys.h>
#include <libipc/vosPrint.h>
#include <libsal/sal_l3_vrrp.h>

#include "vrrpd.h"
#include "vrrpFuncParse.h"
/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */

/* 接收VRRP报文的原始IP插口 */
extern int gVrrp_sock;

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

int main(int argc, char **argv)
{
	int iMsgHd;
	int iRet = 0;
	uchar_t buff[2048]; /*接收消息流缓冲区*/
	VOS_MSG_ADDR             clientAddr;
	SMD_MSG                  *pSmdMsg;
    ulong_t                  *pulTimerHande;

    /*相关数据初始化*/
    if ( VOS_ERR_NONE != VrrpInit() )
    {
        printf("\r\n%s:VrrpInit failed.", __func__);
        return VOS_ERR_FAIL;
    }


    /*消息初始化*/
    iRet = vosMsgInit(VRRP_SOCK_PATH, VOS_MSG_SOURCE_PROCESS, &iMsgHd);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("\r\n%s:vosMsgInit failed", __func__);
        return VOS_ERR_FAIL;
    }
    
    /*命令参数解析函数注册*/
    vrrpAppWrapFunction();
    
    /*注册定时器处理函数*/
    vosTimerRegister(VOS_TIMER_TYPE_ALWAYS, 1000, VrrpTimer, &pulTimerHande, NULL);  

    /*报文接收注册*/
    vosMsgSockAdd(gVrrp_sock, VrrpRecv, NULL);

    while(1)
    {
		iRet = vosMsgRecv(iMsgHd, buff, sizeof(buff), VOS_TIMEOUT_FOREVER, &clientAddr);
		if ( iRet <= 0 )
        {
            continue;
        }

        pSmdMsg = (SMD_MSG *)buff;
        switch ( pSmdMsg->usType )
        {
            case SMD_MSG_TYPE_APP:
                iRet = vrrpAppWrapProcess(pSmdMsg);
                break;

            default:
                continue;
        }
		iRet = vosMsgResp(iMsgHd, iRet, pSmdMsg, pSmdMsg->usArgsBufLen + sizeof(SMD_MSG), &clientAddr);
    }

    return SYS_ERR_OK;
}

