#include <linux/jiffies.h>
#include <linux/random.h>

#include <rsd/rsd_nic.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_sys.h>
#include <osal/sem.h>
#include "gvrp_common.h"
extern   Gvr *my_gvr;
extern   osal_mutex_t            gvrp_sem_pkt;
extern   osal_mutex_t            gvrp_sem_db;
extern   gvrp_statistics_t         gvrp_statistics[SYS_LOGICPORT_NUM_MAX];
uint32  Gid_leaveall_count        = 4;

extern  gvrp_cfg_t     gvrp_cfg;
BOOL sysmalloc(uint32 size, void **allocated)
{
    *allocated = osal_alloc(size);
    if(*allocated==NULL)
        return(FALSE);

    osal_memset(*allocated,0,size);
    return(TRUE);
}
void sysfree(void *allocated)
{
    osal_free(allocated);
}
int32 syspdu_alloc(uint16 size, sys_nic_pkt_t **pdu)
{
    int32 ret =  SYS_ERR_OK;

    if ((ret = rsd_nic_pkt_alloc(size, pdu)) != SYS_ERR_OK)
    {
        ;
    }

    return ret;
}

void syspdu_free( sys_nic_pkt_t *pdu)
{
    rsd_nic_pkt_free(pdu);
}
BOOL syspdu_tx(sys_nic_pkt_t *pPkt,uint32 pktLen, sys_logic_portmask_t pmsk, gvrp_statistics_t *pCnt)
{
    sys_nic_pkt_t           *new_pkt;
    uint16 len = htons((uint16)pktLen - 14);  
    sys_logic_portmask_t  dstportMsk;
    sys_logic_port_t         port;

    char port_str[CAPA_PORTMASK_STR_LEN];
    LOGIC_PORTMASK_CLEAR_ALL(dstportMsk);
    osal_memcpy(&dstportMsk, &pmsk, sizeof(sys_logic_portmask_t));

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, dstportMsk)
    {

        if (IS_LP_TRKMBR(port))
        {
            SYS_DBG(DBG_VLAN_GVRP,"port %s is trunk member port !", LPSTR(port));
            LOGIC_PORTMASK_CLEAR_PORT(dstportMsk, port);
            continue;
        }

#if 0
        rsd_vlan_portPvid_get(port, &pvid);
        if (FALSE == rsd_nic_isPortSTPForwad_ret(port, pvid))
        {
            SYS_DBG(DBG_VLAN_GVRP,"port %d stp state is not forwarding !", LP2UP(port));
            LOGIC_PORTMASK_CLEAR_PORT(dstportMsk, port);
            continue;
        }
#endif

        gvrp_statistics[port].LeaveAll_tx   += pCnt->LeaveAll_tx;
        gvrp_statistics[port].JoinEmpty_tx  +=pCnt->JoinEmpty_tx;
        gvrp_statistics[port].JoinIn_tx     +=pCnt->JoinIn_tx;
        gvrp_statistics[port].LeaveEmpty_tx +=pCnt->LeaveEmpty_tx;
        gvrp_statistics[port].LeaveIn_tx    +=pCnt->LeaveIn_tx;
        gvrp_statistics[port].Empty_tx      +=pCnt->Empty_tx;
        gvrp_statistics[port].Total_tx ++;
    }
    GVRP_FILL_PKT(pPkt, &len, 12, 2); 

    GVRP_PKT_SEM_LOCK();

    LPM2STR(&dstportMsk, port_str);
    SYS_DBG(DBG_VLAN_GVRP,"ports %s is send packets !", port_str);

    if(!IS_LOGIC_PORTMASK_CLEAR(dstportMsk))
    {

        if (SYS_ERR_OK != rsd_nic_pkt_alloc(pktLen, &new_pkt))
        {
            SYS_DBG(DBG_VLAN_GVRP,"syspdu_tx(): Failed to allocate memory for tagged pkt");

            GVRP_PKT_SEM_UNLOCK();
            return FALSE;
        }

        osal_memcpy(new_pkt->data, pPkt->data, pktLen);
        new_pkt->tail += pktLen;
        new_pkt->length = pktLen;

#if defined(CONFIG_SDK_RTL8380) || defined(CONFIG_SDK_RTL8390)
        new_pkt->tx_tag.l2_learning = 0;
#endif

#if 0
        if(gvrp_cfg.gvrp_debug_enable)
        {
            osal_printf("=== [GVRP TX Debug] ================================= Len: %d \n", pktLen);
            for (i = 0; i < pktLen; i++)
            {
                if (i == (new_pkt->length))
                    break;
                if (0 == (i % 16))
                    osal_printf("[%04X] ", i);
                osal_printf("%02X ", *(new_pkt->data + i));
                if (15 == (i % 16))
                    osal_printf("\n");
            }
            osal_printf("\n");
        }
#endif

        if(SYS_ERR_OK != rsd_nic_pkt_tx(new_pkt, &dstportMsk, SYS_CPU_PORT_PRIORITY_GVRP, 0, RSD_NIC_TX_CARE_TRUNK | RSD_NIC_TX_CARE_DOT1X))
        {
            SYS_DBG(DBG_VLAN_GVRP, "%s():%d syspdu_tx failure!", __FUNCTION__, __LINE__);
        }

    }
    GVRP_PKT_SEM_UNLOCK();

    return TRUE;

}
int32 gvrp_pdu_rcv(sys_nic_pkt_t *pPkt, void *pCookie)
{

    Gvr_pdu  *pdu = NULL;
    uint32 src_port   =   pPkt->rx_tag.source_port ;
#if 0
    uint32 i;
    if(GVRP_DEBUG())
    {
        osal_printf("=== [GVRP RX Debug] ================================= Len: %d \n", pPkt->length);
        for (i = 0; i < pPkt->length; i++)
        {
            if (i == (pPkt->length))
                break;
            if (0 == (i % 16))
                osal_printf("[%04X] ", i);
            osal_printf("%02X ", *(pPkt->data + i));
            if (15 == (i % 16))
                osal_printf("\n");
        }
        osal_printf("\n");
    }
#endif

    SYS_DBG(DBG_VLAN_GVRP,"GVRP: PDU Rcv (src port=%s pkt len=%d).......", LPSTR(src_port),  pPkt->length);

    if (DISABLED ==  GVRP_ENABLE())
    {
        SYS_DBG(DBG_VLAN_GVRP,"gvrp global is disabled !");
        goto gvrp_up_exit;
    }

    if (DISABLED == GVRP_PORT_ENABLE(src_port))
    {
        SYS_DBG(DBG_VLAN_GVRP,"Port-%s gvrp is disabled !", LPSTR(src_port));
        GVRP_LOG(LOG_VLAN_GVRP_DROP, LPSTR(src_port));
        goto gvrp_up_exit;

    }

    if( !sysmalloc(sizeof(Gvr_pdu),  (void **)&pdu))
    {
        goto gvrp_up_exit;
    }

    if ((TRUE == pPkt->rx_tag.cvid_tagged) && (TRUE == pPkt->rx_tag.svid_tagged))
    {
        pdu->pucContent = (uint8 *)(&pPkt->data[6+6+2+4+4]);
        pdu->uiLength = pPkt->length - 22;
    }
    else if ((FALSE == pPkt->rx_tag.cvid_tagged) && (FALSE == pPkt->rx_tag.svid_tagged))
    {
        pdu->pucContent = (uint8 *)(&pPkt->data[6+6+2]);
        pdu->uiLength = pPkt->length - 14;
    }
    else
    {
        pdu->pucContent = (uint8 *)(&pPkt->data[6+6+2+4]);
        pdu->uiLength = pPkt->length - 18;
    }
    if (FALSE == gvrp_remove_header(pdu))
    {
        SYS_DBG(DBG_VLAN_GVRP,"Error: The pkt header is wrong !!");
        goto gvrp_up_exit;
    }

 #if 0
    if (SYS_ERR_OK != rsd_nic_pktVid_get(pPkt, &vid))
    {
        SYS_DBG(DBG_VLAN_GVRP,"Error: Could not retrieve VID from the frame received !!");
        goto gvrp_up_exit;
    }
    if (FALSE == rsd_nic_isPortSTPForwad_ret(src_port, vid))
    {
        SYS_DBG(DBG_VLAN_GVRP,"Drop packet, Port-%d is not in STP Forwarding State", LP2UP(src_port));
        goto gvrp_up_exit;
    }
#endif

    GVRP_DB_SEM_LOCK();
    gid_rcv_pdu(my_gvr->garp , src_port , (void *)pdu);
    GVRP_DB_SEM_UNLOCK();
gvrp_up_exit:
    sysfree((void *)pdu);
    drv_nic_pkt_free(0, pPkt);

    return SYS_ERR_OK;

}
void systime_start_random_timer(uint32 process_id,
                                void (*expiry_fn)(unsigned long),uint32 instance_id,uint32 timeout,
struct timer_list *tEntry,
    gvrp_timer_type_t  tmType)
{
    int x;
    uint32 tv;

    get_random_bytes(&x, sizeof(x));
    tv = (x & RAND_MAX);

    if(GVRP_TIMER_JOIN == tmType)
    {
        
        if(tv < 0)
        {
            tv=((-tv) % timeout)+ 5;
        }
        else
        {
            tv=(tv % timeout)+ 5;
        }
    }
    else if (GVRP_TIMER_LEAVEALL == tmType)
    {
        if(tv < 0)
        {
            tv=((-tv) %(timeout/(2))) ;
        }
        else
        {
            tv=(tv % (timeout/(2)));
        }

        tv = tv + timeout;
    }

    osal_timer_start(tEntry, tv * 10);

}
void systime_start_timer(uint32 process_id,
                         void (*expiry_fn) (unsigned long),uint32 instance_id,uint32 timeout,struct timer_list *tEntry)
{
    osal_timer_start(tEntry, (timeout *10));
}

BOOL systime_stop_timer(struct timer_list *tEntry)
{
    if(tEntry->expires!=0)
    {
        osal_timer_stop(tEntry);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
void systime_schedule(uint32 process_id,
                      void (*expiry_fn) (unsigned long),uint32 instance_id,struct timer_list *tEntry)
{
    (*tEntry).data = instance_id;
    expiry_fn(instance_id);
}
void syserr_panic()
{
    SYS_DBG(DBG_VLAN_GVRP,"GVRP: system error panic...");
}
int32 gvrp_check_bridge_mode(uint16 srcPort)
{
    return TRUE;
}

int32 gvrp_remove_header(Gvr_pdu  *pdu)
{
    if(pdu->pucContent[0] != 0x42 && pdu->pucContent[1] != 0x42  \
        && pdu->pucContent[2] != 0x03)
        return FALSE;
    else
    {
        pdu->uiLength = pdu->uiLength - 3;
        pdu->pucContent = pdu->pucContent + 3;
        return TRUE;
    }
}
