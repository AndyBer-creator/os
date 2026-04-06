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
 * $Revision: 15059 $
 * $Date: 2010-12-28 18:34:19 +0800 (Tue, 28 Dec 2010) $
 *
 * Purpose : Port configuration customize structure
 *
 * Feature : Port configuration customize structure
 *
 */

/*
 * Include Files
 */
#include <libsal/sal_mib.h>
#include <libsal/sal_port.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_mib_private.h>
#ifdef CONFIG_SYS_LOOPBACK
#include <libsal/sal_loopback.h>
#endif
#include <libsal/sal_snmp.h>
#include <libipc/vosMsg.h>
#include <libipc/smdCmdWrap.h>
#include <libvty/thread.h>
#include <common/sys_def.h>
#include <common/swdef/sys_swdef_lag.h>

#include "monitorFuncParse.h"
#ifdef CONFIG_SYS_POE
#include <libsal/sal_poe.h>
#endif
/*
 * Symbol Definition
 */
#define POE_POLL_INTERVAL                                  10  // s
#define CPU_POLL_INTERVAL                                  60  // s
#define DEFAULT_POLL_INTERVAL_60                           60  // s

#define  PORT_MAX_NUM                                      (64 * 2) 
#define DEFAULT_POLL_INTERVAL                              10  //s
#define DEFAULT_SPEED_PORT_POLL_INTERVAL                   60 * 5 //s
#define DEFAULT_SPEED_PORT_ZERO_POLL_INTERVAL              60 * 5 //s
#define DEFAULT_DROP_PORT_POLL_INTERVAL                    60  //s
#define DEFAULT_MAX_SEND_TRAP_TIMES                        5
#define SAL_LOOPBACK_FORWARDINGS                           3
#define SAL_LOOPBACK_BLOCKINTS                             4

#define DEFAULT_MEM_INTERVAL                               60  //s
#define MONITOR_SERVER_PATH  "monitor_server_ipc_"


/* Master of threads. */
struct thread_master *monitorMaster;


#define PORT_RATE_POLL_INTERVAL                            300

/*
 * Data Declaration
 */
uint32 gulPollInterval = DEFAULT_POLL_INTERVAL;

uint32 gulIfRateAvgInterval = PORT_RATE_POLL_INTERVAL;
#define MAX_PORT_STATS_CNT         (PORT_RATE_POLL_INTERVAL/DEFAULT_POLL_INTERVAL)

uint64 ulIfInFrameRate[PORT_MAX_NUM] = {0};
uint64 ulIfInOctetRate[PORT_MAX_NUM] = {0};
uint64 ulIfOutFrameRate[PORT_MAX_NUM] = {0};
uint64 ulIfOutOctetRate[PORT_MAX_NUM] = {0};

uint64 ulIfInFrame[PORT_MAX_NUM] = {0};
uint64 ulIfOutFrame[PORT_MAX_NUM] = {0};
uint64 ulIfInOctet[PORT_MAX_NUM] = {0};
uint64 ulIfOutOctet[PORT_MAX_NUM] = {0};

uint64 ulIfStatsCnt[PORT_MAX_NUM] = {0};

uint64 ulIfStatsTimeCur[PORT_MAX_NUM] = {0};
uint64 ulIfRxFrmStatsCur[PORT_MAX_NUM] = {0};
uint64 ulIfRxOctStatsCur[PORT_MAX_NUM] = {0};
uint64 ulIfTxFrmStatsCur[PORT_MAX_NUM] = {0};
uint64 ulIfTxOctStatsCur[PORT_MAX_NUM] = {0};

uint64  ulIfRxFrmStatsSum[PORT_MAX_NUM] = {0};
uint64  ulIfRxOctStatsSum[PORT_MAX_NUM] = {0};
uint64  ulIfTxFrmStatsSum[PORT_MAX_NUM] = {0};
uint64  ulIfTxOctStatsSum[PORT_MAX_NUM] = {0};

uint64 ulIfRxFrmStats[PORT_MAX_NUM][MAX_PORT_STATS_CNT] = {{0}};
uint64 ulIfRxOctStats[PORT_MAX_NUM][MAX_PORT_STATS_CNT] = {{0}};
uint64 ulIfTxFrmStats[PORT_MAX_NUM][MAX_PORT_STATS_CNT] = {{0}};
uint64 ulIfTxOctStats[PORT_MAX_NUM][MAX_PORT_STATS_CNT] = {{0}};

uint64 ulIfRxFrmRateAvg[PORT_MAX_NUM] = {0};
uint64 ulIfRxOctRateAvg[PORT_MAX_NUM] = {0};
uint64 ulIfTxFrmRateAvg[PORT_MAX_NUM] = {0};
uint64 ulIfTxOctRateAvg[PORT_MAX_NUM] = {0};


#ifdef CONFIG_SYS_LOOPBACK
static loopbackPortStatus_t  s_LoopBlackPortStatus[PORT_MAX_NUM+1];
#endif /* 2025/8/7 */
uint8 lastDupStatus[PORT_MAX_NUM] = {0};
uint64 lastPktResult[PORT_MAX_NUM] = {0};
//uint64 ulIfLastOctet[PORT_MAX_NUM] = {0};
uint32 lastSendTimes[PORT_MAX_NUM] = {0};

uint64 portDropIfInFrame[PORT_MAX_NUM] = {0};
uint64 portAllIfInFrame[PORT_MAX_NUM] = {0};


/* �������ʱ���gulIfRateAvgInterval�ڵ�ƽ���˿����� */
void calcAvgPortPktRateByTime(uint32 ulPort, uint64 ulRxFrm, uint64 ulRxOct, uint64 ulTxFrm, uint64 ulTxOct)
{
    int     idx = 0;
    
    /* ʱ���DEFAULT_POLL_INTERVAL�ڵ�ʱ��ͱ�����ͳ��?*/
    ulIfRxFrmStatsCur[ulPort] += ulRxFrm;
    ulIfRxOctStatsCur[ulPort] += ulRxOct;
    ulIfTxFrmStatsCur[ulPort] += ulTxFrm;
    ulIfTxOctStatsCur[ulPort] += ulTxOct;
    ulIfStatsTimeCur[ulPort]  += gulPollInterval;
    
    /* �ж��Ƿ����һ��ͳ�ƣ�ÿ��ͳ��ʱ����ΪDEFAULT_POLL_INTERVAL */
    if (ulIfStatsTimeCur[ulPort] >= DEFAULT_POLL_INTERVAL)
    {
        /* ������ͳ�ƴ������㽫���滻�����һ�ε�ͳ��ֵλ��?*/
        idx = (ulIfStatsCnt[ulPort] % (gulIfRateAvgInterval / DEFAULT_POLL_INTERVAL));
        
        /* ����ʱ������ܵı���ͳ��? �������� = �ϴ����� - ���һ�ε��?+ ���һ�ε��?*/
        ulIfRxFrmStatsSum[ulPort] = ulIfRxFrmStatsSum[ulPort]  + ulIfRxFrmStatsCur[ulPort] - ulIfRxFrmStats[ulPort][idx];
        ulIfRxOctStatsSum[ulPort] = ulIfRxOctStatsSum[ulPort]  + ulIfRxOctStatsCur[ulPort] - ulIfRxOctStats[ulPort][idx];
        ulIfTxFrmStatsSum[ulPort] = ulIfTxFrmStatsSum[ulPort]  + ulIfTxFrmStatsCur[ulPort] - ulIfTxFrmStats[ulPort][idx];
        ulIfTxOctStatsSum[ulPort] = ulIfTxOctStatsSum[ulPort]  + ulIfTxOctStatsCur[ulPort] - ulIfTxOctStats[ulPort][idx];
        
        /* �����һ�ε�ֵ�滻���һ�ε�ֵ */
        ulIfRxFrmStats[ulPort][idx] = ulIfRxFrmStatsCur[ulPort];
        ulIfRxOctStats[ulPort][idx] = ulIfRxOctStatsCur[ulPort];
        ulIfTxFrmStats[ulPort][idx] = ulIfTxFrmStatsCur[ulPort];
        ulIfTxOctStats[ulPort][idx] = ulIfTxOctStatsCur[ulPort];
        
        /* ����ʱ����ڵ�ƽ������?*/
        ulIfRxFrmRateAvg[ulPort] = ulIfRxFrmStatsSum[ulPort] / gulIfRateAvgInterval;
        sal_mib_last_5_rate_set(ulPort, TYPE_IN_PKT_RATE, ulIfRxFrmRateAvg[ulPort]);
        
        ulIfRxOctRateAvg[ulPort] = ulIfRxOctStatsSum[ulPort] / gulIfRateAvgInterval;
        sal_mib_last_5_rate_set(ulPort, TYPE_IN_BYTE_RATE, ulIfRxOctRateAvg[ulPort]);
        
        ulIfTxFrmRateAvg[ulPort] = ulIfTxFrmStatsSum[ulPort] / gulIfRateAvgInterval;
        sal_mib_last_5_rate_set(ulPort, TYPE_OUT_PKT_RATE, ulIfTxFrmRateAvg[ulPort]);
        
        ulIfTxOctRateAvg[ulPort] = ulIfTxOctStatsSum[ulPort] / gulIfRateAvgInterval;
        sal_mib_last_5_rate_set(ulPort, TYPE_OUT_BYTE_RATE, ulIfTxOctRateAvg[ulPort]);
        
        /* ����ͳ��ֵ��0��׼����һ�μ��� */
        ulIfStatsCnt[ulPort]++;
        ulIfRxFrmStatsCur[ulPort] = 0;
        ulIfRxOctStatsCur[ulPort] = 0;
        ulIfTxFrmStatsCur[ulPort] = 0;
        ulIfTxOctStatsCur[ulPort] = 0;
        ulIfStatsTimeCur[ulPort]  = 0;
    }
    
    return;
}


int32 port_pkt_statistics_monitor(void)
{
    uint32 portIdx = 0;
    uint64 pktResult, tmpPkt;
    uint32 lport;

    
    for(portIdx = 0; portIdx < SYS_LOGIC_PORT_NUM; portIdx++)
    {
        lport = portIdx;

        //��ȡ��˿�pkt ��    
        sal_mib_etherStatsPkts_get(lport, &pktResult);
        if(pktResult >= ulIfInFrame[lport])
        {
            ulIfInFrameRate[lport] =  pktResult - ulIfInFrame[lport];
        }
        else
        {
            ulIfInFrameRate[lport] = 0xFFFFFFFFFFFFFFFF - ulIfInFrame[lport] + pktResult;
        }
        ulIfInFrame[lport] = pktResult;

#if 0       
        if( ulIfInFrameRate[lport] < 0 )
        {
            ulIfInFrameRate[lport] += ulIfInFrame[lport];
        }
#endif
        if(pktResult >= ulIfInOctet[lport])
        {
            ulIfInOctetRate[lport] =  pktResult - ulIfInOctet[lport];       
        }
        else
        {
            ulIfInOctetRate[lport] = 0xFFFFFFFFFFFFFFFF - ulIfInOctet[lport] + pktResult;
        }
        ulIfInOctet[lport] = pktResult;
#if 0       
        if( ulIfInOctetRate[lport] < 0)
        {
            ulIfInOctetRate[lport] += ulIfInOctet[lport];
        }
#endif
        sal_mib_ifOutUcastPkts_get(lport, &pktResult);
        sal_mib_ifOutNUcastPkts_get(lport, &tmpPkt);
        if((pktResult + tmpPkt) >= ulIfOutFrame[lport])
        {
            ulIfOutFrameRate[lport] =  pktResult + tmpPkt - ulIfOutFrame[lport];        
        }
        else
        {
            ulIfOutFrameRate[lport] = 0xFFFFFFFFFFFFFFFF - ulIfOutFrame[lport] + pktResult;
        }
        ulIfOutFrame[lport] = pktResult + tmpPkt;

#if 0
        if( ulIfOutFrameRate[lport] < 0 )
        {
            ulIfOutFrameRate[lport] += ulIfOutFrame[lport];
        }
#endif
        sal_mib_ifOutOctets_get(lport, &pktResult);
        if(pktResult >= ulIfOutOctet[lport])
        {
            ulIfOutOctetRate[lport] =  pktResult - ulIfOutOctet[lport];     
        }
        else
        {
            ulIfOutOctetRate[lport] = 0xFFFFFFFFFFFFFFFF - ulIfOutOctet[lport] + pktResult;
        }
        ulIfOutOctet[lport] = pktResult;
#if 0
        if( ulIfOutOctetRate[lport] < 0 )
        {
            ulIfOutOctetRate[lport] += ulIfOutOctet[lport];
        }
#endif
        calcAvgPortPktRateByTime(lport, ulIfInFrameRate[lport], ulIfInOctetRate[lport], ulIfOutFrameRate[lport], ulIfOutOctetRate[lport]);
        ulIfInFrameRate[lport]  /= gulPollInterval;
        ulIfOutFrameRate[lport] /= gulPollInterval;
        ulIfInOctetRate[lport]  /= gulPollInterval;
        ulIfOutOctetRate[lport] /= gulPollInterval; 
    }
    return SYS_ERR_OK;
}

int port_pkt_monitor_init(void)
{
    uint32 portIndex;
    uint64 pktResult, tmpPkt;
    sys_logic_port_t lp;
    
    memset(ulIfRxFrmRateAvg, 0, sizeof(ulIfRxFrmRateAvg));
    memset(ulIfRxOctRateAvg, 0, sizeof(ulIfRxOctRateAvg));
    memset(ulIfTxFrmRateAvg, 0, sizeof(ulIfTxFrmRateAvg));
    memset(ulIfTxOctRateAvg, 0, sizeof(ulIfTxOctRateAvg));
    
    memset(ulIfRxFrmStats, 0, sizeof(ulIfRxFrmStats));
    memset(ulIfRxOctStats, 0, sizeof(ulIfRxOctStats));
    memset(ulIfTxFrmStats, 0, sizeof(ulIfTxFrmStats));
    memset(ulIfTxOctStats, 0, sizeof(ulIfTxOctStats));
    
    memset(ulIfRxFrmStatsSum, 0, sizeof(ulIfRxFrmStatsSum));
    memset(ulIfRxOctStatsSum, 0, sizeof(ulIfRxOctStatsSum));
    memset(ulIfTxFrmStatsSum, 0, sizeof(ulIfTxFrmStatsSum));
    memset(ulIfTxOctStatsSum, 0, sizeof(ulIfTxOctStatsSum));
    
    memset(ulIfRxFrmStatsCur, 0, sizeof(ulIfRxFrmStatsCur));
    memset(ulIfRxOctStatsCur, 0, sizeof(ulIfRxOctStatsCur));
    memset(ulIfTxFrmStatsCur, 0, sizeof(ulIfTxFrmStatsCur));
    memset(ulIfTxOctStatsCur, 0, sizeof(ulIfTxOctStatsCur));

    memset(ulIfInFrameRate, 0, sizeof(ulIfInFrameRate));
    memset(ulIfInOctetRate, 0, sizeof(ulIfInOctetRate));
    memset(ulIfOutFrameRate, 0, sizeof(ulIfOutFrameRate));
    memset(ulIfOutOctetRate, 0, sizeof(ulIfOutOctetRate));

    memset(ulIfInFrame, 0, sizeof(ulIfInFrame));
    memset(ulIfOutFrame, 0, sizeof(ulIfOutFrame));
    memset(ulIfInOctet, 0, sizeof(ulIfInOctet));
    memset(ulIfOutOctet, 0, sizeof(ulIfOutOctet));
    
    memset(ulIfStatsCnt, 0, sizeof(ulIfStatsCnt));
    memset(ulIfStatsTimeCur, 0, sizeof(ulIfStatsTimeCur));

    for(portIndex = 0; portIndex < PORT_MAX_NUM; portIndex++)
    {
        sal_mib_etherStatsPkts_get(portIndex, &pktResult);
        ulIfInFrame[portIndex] = pktResult;

        sal_mib_etherStatsOctets_get(portIndex, &pktResult);
        ulIfInOctet[portIndex] = pktResult;

        sal_mib_ifOutUcastPkts_get(portIndex, &pktResult);
        sal_mib_ifOutNUcastPkts_get(portIndex, &tmpPkt);
        ulIfOutFrame[portIndex] = pktResult + tmpPkt;

        sal_mib_ifOutOctets_get(portIndex, &pktResult);
        ulIfOutOctet[portIndex] = pktResult;
    }

    memset(lastDupStatus, 0xFF, sizeof(lastDupStatus));
    memset(lastPktResult, 0xFF, sizeof(lastPktResult));
    //memset(ulIfLastOctet, 0xFF, sizeof(ulIfLastOctet));
#ifdef CONFIG_SYS_LOOPBACK
    FOR_EACH_LOGIC_PORT(lp)
    {
        s_LoopBlackPortStatus[lp].port = lp;
        s_LoopBlackPortStatus[lp].status_old_1 = SAL_LOOPBACK_FORWARDINGS;
        s_LoopBlackPortStatus[lp].status_old_2 = SAL_LOOPBACK_FORWARDINGS;
        
    }
#endif /* 2025/8/7 */
    return SYS_ERR_OK;
}

#define MEM_HIGH 90
#define CPU_BUSY 70
#define TRAP_WARNING  4
int memory_used_monitor(void)
{
    sys_mem_info_t memInfo;
    uint32 value;
    char buf[125];
    static int Flag = 0;
    osal_memset(&memInfo, 0, sizeof(sys_mem_info_t));
    SYS_ERR_CHK(sal_sys_memInfo_get(&memInfo));
    value = ((double)(memInfo.mem_total - memInfo.mem_free - memInfo.mem_buffer - memInfo.mem_cached) / (double)memInfo.mem_total) * 100;
    if(value > MEM_HIGH && Flag == 0)
    {
        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "#Memory usage exception *Excessive memory usage, Memory usage %d%%", value);
        sal_snmp_memoryTrap_send(buf);
        Flag = 1;
     }
     else if(value < MEM_HIGH)
     {
        Flag = 0;
     }
    return SYS_ERR_OK;
}

int cpu_used_monitor(void)
{
    uint32 utilzVale = 0;
    static int delay = 0;
    #ifndef CONFIG_SYS_OEM_IKUAI
    char buf[125];
    static int flag = 0;
    #endif

    SYS_ERR_CHK(sal_sys_cpuUtilz_get(&utilzVale)); 
    #ifndef CONFIG_SYS_OEM_IKUAI
    if(utilzVale > CPU_BUSY && flag == 0)
    {
        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "#CPU usage exception *CPU is busy, CPU usage is %d%%", utilzVale);
        sal_snmp_CPUTrap_send(buf);

        flag = 1;
    }
    else if(utilzVale < CPU_BUSY)
    {
        flag = 0;
    }
    #endif
     return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_POE
int poe_status_monitor(void)
{
    sal_poe_status_info_updata();
    
    return SYS_ERR_OK;
}

int monitorPoeTimerProcess(struct thread *t)
{
    poe_status_monitor();
    thread_add_timer(monitorMaster, monitorPoeTimerProcess, NULL, POE_POLL_INTERVAL);
    return 0;
}
#endif

int monitorPortStatisticsTimerProcess(struct thread *t)
{
    port_pkt_statistics_monitor();
    thread_add_timer(monitorMaster, monitorPortStatisticsTimerProcess, NULL, DEFAULT_POLL_INTERVAL);
    return 0;
}


int monitorMemoryTimerProcess(struct thread *t)
{
    memory_used_monitor();
    thread_add_timer(monitorMaster, monitorMemoryTimerProcess, NULL, DEFAULT_MEM_INTERVAL);
    return 0;
}

int monitorCpuTimerProcess(struct thread *t)
{
    cpu_used_monitor();
    thread_add_timer(monitorMaster, monitorCpuTimerProcess, NULL, CPU_POLL_INTERVAL);
    return 0;
}




/* Read data via smd socket. */
int monitor_thread_read (struct thread *thread)
{
  int iRet = 0;
  int i;
  int nbytes;
  unsigned char buf[1024];
  SMD_MSG            *pSmdMsg;
  VOS_MSG_ADDR        clientAddr;

  int sock = THREAD_FD (thread);



    /* Read raw data from socket */
    iRet = vosMsgRecv(sock, buf, 1024, VOS_TIMEOUT_NOWAIT, &clientAddr);
    if (iRet < 0)
    {
          goto end;
    }
    else
    {
        pSmdMsg = (SMD_MSG *)buf;
        switch ( pSmdMsg->usType )
        {

            case SMD_MSG_TYPE_APP:
                iRet = monitorAppWrapProcess(pSmdMsg);
                break;
            
            default:
                printf("\r\n%s:vosMsgRecv receive unkown msg %d", __func__, pSmdMsg->usType);
                goto end;
        }

        iRet = vosMsgResp(sock, iRet, pSmdMsg, pSmdMsg->usArgsBufLen + sizeof(SMD_MSG), &clientAddr);
    }

end:
  thread_add_read (monitorMaster, monitor_thread_read, (void *)sock, sock);
  return 0;
}



/* Function Name:
 *      port_rate_monitor_init
 * Description:
 *      Init the port rate module
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 * Note:
 *      None
 */
int main(int argc, char **argv)
{
    int iMsgHd;
    struct thread thread;
    int iRet = 0;
    uchar_t buff[2048];
    SMD_MSG                  *pSmdMsg;
    VOS_MSG_ADDR             clientAddr;

    port_pkt_monitor_init();

    /* Prepare master thread. */
    monitorMaster = thread_master_create ();

    vosMsgInit(MONITOR_SERVER_PATH, VOS_MSG_SOURCE_PROCESS, &iMsgHd);

    thread_add_read (monitorMaster, monitor_thread_read, (void *)iMsgHd, iMsgHd);

    monitorAppWrapFunction();

    thread_add_timer(monitorMaster, monitorPortStatisticsTimerProcess, NULL, DEFAULT_POLL_INTERVAL);

    //thread_add_timer(monitorMaster, monitorPortSpeedAlarmStatisticsTimerProcess, NULL, DEFAULT_SPEED_PORT_POLL_INTERVAL);

    //thread_add_timer(monitorMaster, monitorPortPktZeroStatisticsTimerProcess, NULL, DEFAULT_SPEED_PORT_ZERO_POLL_INTERVAL);

    //thread_add_timer(monitorMaster, monitorPortPktDropTimerProcess, NULL, DEFAULT_DROP_PORT_POLL_INTERVAL);

    //thread_add_timer(monitorMaster, monitorMemoryTimerProcess, NULL, DEFAULT_MEM_INTERVAL);

    //thread_add_timer(monitorMaster, monitorCpuTimerProcess, NULL, CPU_POLL_INTERVAL);

    //thread_add_timer(monitorMaster, monitorPortDuplexTimerProcess, NULL, DEFAULT_POLL_INTERVAL_60);
    
#ifdef CONFIG_SYS_PROTO_LOOPBACK
    //thread_add_timer(monitorMaster, monitorPortLoopbackTimerProcess, NULL, DEFAULT_POLL_INTERVAL_60);
#endif
    
#ifdef CONFIG_SYS_POE
    if (POE_IS_SYSTEM_READY())
    {
        //thread_add_timer(monitorMaster, monitorPoeTimerProcess, NULL, POE_POLL_INTERVAL);
    }
#endif
    /* Execute each thread. */
    while (thread_fetch (monitorMaster, &thread))
      thread_call (&thread);

    return SYS_ERR_OK;
}

