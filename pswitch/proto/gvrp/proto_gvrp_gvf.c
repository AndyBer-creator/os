#include "proto_gvrp_gvf.h"
#include <rsd/rsd_sys.h>
#include <rsd/rsd_nic.h>

extern gvrp_cfg_t gvrp_cfg;
extern gvrp_statistics_t  gvrp_statistics[SYS_LOGICPORT_NUM_MAX];
extern gvrp_error_statistics_t  gvrp_error_stat[SYS_LOGICPORT_NUM_MAX];
void gvf_rdmsg_init(Gvf *gvf, Gvr_pdu *pdu) 
{
    uint16  uiPrtclId;

    uint16  uiTmpPtr = 0;
    uint8   *pucPdu  = pdu->pucContent;

    uiPrtclId = (pucPdu[uiTmpPtr]<<8) + pucPdu[uiTmpPtr+1];

    if (uiPrtclId != GARP_ID)
    {
        gvf->uiPduPtr = DISCARDED_PDU;
        return;
    }

    gvf->uiPduPtr    = 2;    
    gvf->bBeginOfMsg = TRUE; 

    gvf->pdu = pdu;
}
BOOL gvf_rdmsg(Gvr *my_gvr, Gid *my_port, Gvf *gvf, Gvf_msg *msg)   
{
    uint8   *pucPdu = NULL;
    uint8   ucAttrType ;
    uint16  uiPtr       = gvf->uiPduPtr;

    uint8   ucAttrLen;

    if (uiPtr == DISCARDED_PDU)
    {
        gvrp_error_stat[my_port->port_no].invProt++;
        GVRP_LOG(LOG_VLAN_GVRP_PROTO_ID_INVALID);
        return FALSE;
    }

    pucPdu = gvf->pdu->pucContent;

    if (pucPdu[uiPtr] == END_MARK && !gvf->bBeginOfMsg)
        return FALSE;                    

    gvf->bBeginOfMsg = FALSE;

    if (!(pucPdu[uiPtr] == Vlan_attribute))
    {
        gvf_skipmsg(gvf);
        gvrp_error_stat[my_port->port_no].invAtyp++;
        GVRP_LOG(LOG_VLAN_GVRP_ATTR_TYPE_INVALID);
        return TRUE;
    }

    ucAttrType = pucPdu[uiPtr];
    uiPtr++;

    do {

        ucAttrLen = pucPdu[uiPtr];

        if (ucAttrLen != 2 && ucAttrLen !=4)
        {
            gvrp_error_stat[my_port->port_no].invAlen++;
            gvf_skip_event(gvf);
            GVRP_LOG(LOG_VLAN_GVRP_ATTR_LEN_INVALID);
            return FALSE;
        }

        if (uiPtr+(uint16)ucAttrLen > gvf->pdu->uiLength-2) 
            return FALSE;
        uiPtr++;
        
        msg->attribute = ucAttrType;
        msg->event = pucPdu[uiPtr];

        switch(pucPdu[uiPtr])
        {
        case LEAVE_ALL:
            msg->event = Gid_rcv_leaveall;
            break;

        case JOIN_EMPTY:
            msg->event = Gid_rcv_joinempty;
            break;

        case JOIN_IN:
            msg->event = Gid_rcv_joinin;
            break;

        case LEAVE_EMPTY:
            msg->event = Gid_rcv_leaveempty;
            break;

        case LEAVE_IN:
            msg->event = Gid_rcv_leavein;
            break;

        case EMPTY:
            msg->event = Gid_rcv_empty;
            break;

        default:
            msg->event = Gid_null;
            break;
        }

        uiPtr++;

        if ( (msg->event == Gid_rcv_leaveall) && (ucAttrLen != 2))
        {
                gvrp_error_stat[my_port->port_no].invAlen++;
                GVRP_LOG(LOG_VLAN_GVRP_ATTR_LEN_INVALID);
                return FALSE;
        }

        if (ucAttrLen == 4)
        {           
            msg->key1 = (pucPdu[uiPtr]<<8) + pucPdu[uiPtr+1];
            uiPtr=uiPtr+2;
            if (msg->key1 < BOND_VLAN_MIN || msg->key1 > BOND_VLAN_MAX)
            {
                gvrp_error_stat[my_port->port_no].invAval++;
                GVRP_LOG(LOG_VLAN_GVRP_ATTR_VALUE_INVALID);
                continue;
            }
        }
        else if (ucAttrLen == 2)
        {    
            msg->key1 = 0;
            if (Gid_rcv_leaveall != msg->event)
            {
                gvrp_error_stat[my_port->port_no].invAlen++;
                GVRP_LOG(LOG_VLAN_GVRP_ATTR_LEN_INVALID);
                return FALSE;
            }
        }

        if(msg->event==Gid_rcv_leaveall)
        {
            gvrp_statistics[my_port->port_no].LeaveAll_rx++;
        }
        else if(msg->event==Gid_rcv_joinempty)
        {
            gvrp_statistics[my_port->port_no].JoinEmpty_rx++;
        }
        else if(msg->event==Gid_rcv_joinin)
        {
            gvrp_statistics[my_port->port_no].JoinIn_rx++;
        }
        else if(msg->event==Gid_rcv_empty)
        {
            gvrp_statistics[my_port->port_no].Empty_rx++;
        }
        else if(msg->event==Gid_rcv_leavein)
        {
            gvrp_statistics[my_port->port_no].LeaveIn_rx++;
        }
        else if(msg->event==Gid_rcv_leaveempty)
        {
            gvrp_statistics[my_port->port_no].LeaveEmpty_rx++;
        }else if(msg->event == Gid_null)
        {
            gvrp_error_stat[my_port->port_no].invEvent++;
            GVRP_LOG(LOG_VLAN_GVRP_ATTR_EVENT_INVALID);
            return FALSE;
        }

        SYS_DBG(DBG_VLAN_GVRP, "port : %s ,  vlan: %d  gvrp msg event: %d", LPSTR(my_port->port_no),  msg->key1, msg->event);
        gvr_rcv_msg(my_gvr, my_port, msg);

    } while(pucPdu[uiPtr] != 0x00);

    gvrp_statistics[my_port->port_no].Total_rx++;
    
    gvf->ucAttrType  = ucAttrType;
    gvf->uiPduPtr    = uiPtr+1 ; 
    return TRUE;
}
void gvf_skipmsg(Gvf *gvf)
{
    uint8   *pucPdu = gvf->pdu->pucContent;
    uint16  uiPtr   = gvf->uiPduPtr;
    uint8   ucAttrLen;

    uiPtr++; 
    for (;;)
    {  
        ucAttrLen = pucPdu[uiPtr];
        uiPtr = uiPtr+ucAttrLen;

        if (pucPdu[uiPtr] == END_MARK)
        {
            gvf->uiPduPtr = uiPtr+1; 
            return;
        }
    }
}
void gvf_skip_event(Gvf *gvf)
{
    uint8   *pucPdu = gvf->pdu->pucContent;
    uint16  uiPtr   = gvf->uiPduPtr;

    uint8   ucAttrLen;

    ucAttrLen = pucPdu[uiPtr];
    uiPtr = uiPtr+ucAttrLen;

    gvf->uiPduPtr = uiPtr;

    return;
}
BOOL gvrp_wrmsg_init(sys_nic_pkt_t * pkt, uint16 *offset)
{
    sys_mac_t  switch_mac;

    uint8  dsap = htons(0x42);
    uint8  ssap = htons(0x42);
    uint8   cf = htons(0x03);
    
    uint16 len  =  htons(0x00);
    uint16 protocol_id = htons(0x0001);
    uint8 dst_mac[6]    = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x21};

    rsd_sys_macAddr_get(0, &switch_mac);

    GVRP_FILL_PKT(pkt, &dst_mac, 0, 6);
    GVRP_FILL_PKT(pkt, &switch_mac, 6, 6);

    GVRP_FILL_PKT(pkt, &len, 12, 2);  
    GVRP_FILL_PKT(pkt, &dsap, 14, 1);
    GVRP_FILL_PKT(pkt, &ssap, 15, 1);
    GVRP_FILL_PKT(pkt, &cf, 16, 1);

    GVRP_FILL_PKT(pkt, &protocol_id, 17, 2);
    *offset = 19;

    return TRUE;

}

BOOL gvrp_wrmsg(sys_nic_pkt_t *pkt, Gvf_msg *msg, uint16 startOffset, uint16 *endOffset)
{
    uint16  offset ;
    uint8 event;
    uint8 leaveall_len =  htons(0x02);            
    uint8 others_len  =   htons(0x04);               
    uint8 end_mark = htons(0x00);
    uint8 attr_type = htons(0x01);

    switch(msg->event)
    {
    case Gid_tx_leaveempty:
        msg->event=LEAVE_EMPTY;
        break;
    case Gid_tx_leavein:
        msg->event=LEAVE_IN;
        break;
    case Gid_tx_empty:
        msg->event=EMPTY;
        break;
    case Gid_tx_joinempty:
        msg->event=JOIN_EMPTY;
        break;
    case Gid_tx_joinin:
        msg->event=JOIN_IN;
        break;
    case Gid_tx_leaveall:
        msg->event=LEAVE_ALL;
        break;
    case Gid_tx_leaveall_range:
        msg->event=LEAVE_ALL;
        break;
    default:
        return(FALSE);

    }
    event = htons(msg->event);
    msg->key1 =   htons(msg->key1);

    offset = startOffset;

    GVRP_FILL_PKT(pkt, &attr_type, offset, 1);
    offset += 1;

    if(msg->event==0) { 
        GVRP_FILL_PKT(pkt, &leaveall_len, offset, 1);
        offset += 1;
        GVRP_FILL_PKT(pkt, &event, offset, 1);
        offset += 1;
        GVRP_FILL_PKT(pkt, &end_mark, offset, 1);
        offset += 1;
    }
    else { 
        GVRP_FILL_PKT(pkt, &others_len, offset, 1);
        offset += 1;
        GVRP_FILL_PKT(pkt, &event, offset, 1);
        offset += 1;
        GVRP_FILL_PKT(pkt, &msg->key1, offset, 2);
        offset += 2;
        GVRP_FILL_PKT(pkt, &end_mark, offset, 1);
        offset += 1;
    }

    *endOffset = offset;

    return TRUE;
}
BOOL gvrp_wrmsgEnd(sys_nic_pkt_t *pkt,  uint16 startOffset, uint16 *endOffset)
{
    uint8 endmark = 0x00;
    uint16  start = startOffset;

    GVRP_FILL_PKT(pkt, &endmark, start, 1);
    *endOffset = start +1;

    return TRUE;
}

