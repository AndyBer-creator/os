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
#include <libcfg/cfg_dhcp_server.h>
#include <libsal/sal_dhcp_server.h>
#include <libsal/sal_sys.h>
#include <libvty/vty.h>
#include <libipc/vosMsg.h>
#include <libipc/smdMsgRequest.h>

#include "dhcps.h"
#include "dhcpr.h"
#include "dhcpSocket.h"
#include "ip_pool.h"
#include "dhcpserverFuncParse.h"

#define DHCPS_VTY_PORT 2300
#define DHCP_SERVER_PID_FILE "/var/run/dhcp_server_clent.pid"


/* RIP VTY bind address. */
char *vty_addr = NULL;

/* RIP VTY connection port. */
int vty_port = DHCPS_VTY_PORT;

#define DHCPSD_VTYSH_PATH "/var/run/dhcpsd.vty"
#define VTYSH_DHCPS     0x00001

/*
 * Symbol Definition
 */
static sys_enable_t g_dhcpserverStatus = FALSE;
struct thread_master *dhcps_master;
extern DHCP_SERVER_S g_dhcpgroup[DHCP_MAX_GROUP_NUM];
extern VLANIfToDHCPS_S g_dhcpInterface[DHCP_MAX_INTERFACE_NUM];
extern AAA_IPPOOL_S gIpPool[IPPOOL_MAX_POOL_NUM];
extern ulong_t ipPool_RowStatus[IPPOOL_MAX_POOL_NUM];
extern ulong_t g_bDhcpRelayEnable;
extern BOOL gbDHCPRelayPortEnable[SYS_LOGICPORT_NUM_MAX];


/*
 * Data Declaration
 */
static int finish = 1;

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */
#if 0
int32 dhcp_read_from_server (int filedes)
{
	osal_printf("dhcp client recv server send message-----OK\n");
    cmd_msg_hdr_t hdr;
    char *buffer = NULL;
    int nbytes;

    SYS_MEM_CLEAR(hdr);

    /* Read hdr first */
    nbytes = sal_recv(filedes, (void*)&hdr, sizeof(cmd_msg_hdr_t), 0);

    if (nbytes < 0 || nbytes != sizeof(cmd_msg_hdr_t))
    {
        /* Read error. */
        perror ("read");
        exit (EXIT_FAILURE);
    }
    else if (nbytes == 0)
    {
        /* End-of-file. */
        return -1;
    }
    else
    {
        uint32 dataSize = hdr.msg_len - sizeof(cmd_msg_hdr_t);

        /* Data read. */
        buffer = osal_alloc(dataSize);
        osal_memset(buffer, 0, dataSize);

        nbytes = sal_recv(filedes, buffer, dataSize, 0);

        if (nbytes < 0)
        {
            /* Read error. */
            perror ("read");
            exit (EXIT_FAILURE);
        }
        else if (nbytes == 0)
        {
            /* End-of-file. */
            osal_free(buffer);
            return -1;
        }

        //route_transl(buffer, nbytes);

        osal_free(buffer);
        return 0;
    }
}
#endif
extern int32 sal_dhcp_server_get(cfg_dhcp_server_t *pDhcpServer);
extern STATUS InitIpPool(uchar_t ucIndex);
extern STATUS DeleteIpPool(uchar_t ucIndex);


int dhcpsrsTimerProcess(ulong_t ulTimerOver, void *pArg)
{
#ifdef GBN_INCLUDE_DHCPRELAYSERVER
    dhcpserver_rx_timertick();
#endif
    return 0;
}

int main(int argc, char **argv)
{
	int iMsgHd;
	int iRet = 0;
	uchar_t buff[2048]; /*Receive message stream buffer*/
	VOS_MSG_ADDR             clientAddr;
	SMD_MSG                  *pSmdMsg;
    ulong_t                 *pulTimerHande;

	/*Initialization*/
	dhcpserver_init();
	dhcprelay_init();
	vosMsgInit(DHCP_SERVER_PATH, VOS_MSG_SOURCE_PROCESS, &iMsgHd);

    /*Register timer handler functions*/
    vosTimerRegister(VOS_TIMER_TYPE_ALWAYS, 1000,
        dhcpsrsTimerProcess, &pulTimerHande, NULL);

    /*Register command parameter parsing functions*/
    dhcpserverAppWrapFunction();

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
                iRet = dhcpserverAppWrapProcess(pSmdMsg);
                break;

            default:
                continue;
        }
		iRet = vosMsgResp(iMsgHd, iRet, pSmdMsg, pSmdMsg->usArgsBufLen + sizeof(SMD_MSG), &clientAddr);
    }

    return SYS_ERR_OK;
}

