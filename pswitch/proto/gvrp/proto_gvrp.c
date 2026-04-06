#include <rsd/rsd_gvrp.h>
#include <common/sys_log.h>
#include "gvrp_common.h"

#include <gvrp/proto_gvrp.h>
osal_mutex_t            gvrp_sem_pkt;
osal_mutex_t            gvrp_sem_db;

Gvd   gGvd[GVRP_DB_SIZE+1];   

gvrp_statistics_t   gvrp_statistics[SYS_LOGICPORT_NUM_MAX];
gvrp_error_statistics_t  gvrp_error_stat[SYS_LOGICPORT_NUM_MAX];
gvrp_cfg_t          gvrp_cfg;
Gid                 gGid[SYS_LOGICPORT_NUM_MAX];

Garp gGarp;
Gvr *my_gvr  = NULL;
extern uint32      Gid_leaveall_count;
static rsd_nic_handler_t gvrpHandler = {
    .priority       = RSD_NIC_HANDLER_PRI_GVRP,
    .dmac           = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x21},
    .dmac_caremask  = { 0xff,0xff,0xff,0xff,0xff,0xff },
    .ethertype      = 0,   
    .proto          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_TRAP,
    .rx_cbf         = gvrp_pdu_rcv,
    .rx_careMask    = RSD_NIC_RX_CARE_TRUNK,
    .pName          = "GVRP RX Thread",
    .pCookie        = NULL,
};
static int32 _gvrp_vlan_handler(uint32 event, char* pData)
{
    sys_vlan_event_t *pVlanEvent = NULL;
    Gvd vlandb;
    sys_vid_t vid;
    sys_vlan_adminEntry_t vlanEntry;
    uint32 curVlandb = 0;
    sys_vlanmask_t stVlanMsk;
    uint32 gvd_index;
    sys_logic_port_t lport;
    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pVlanEvent = (sys_vlan_event_t *)pData;

    if (DISABLED == GVRP_ENABLE())
        return SYS_ERR_OK;

    GVRP_DB_SEM_LOCK();

    if(SYS_NT_VLAN_ADMINVLAN_CREATE == event)
    {
        osal_memset(&vlandb, 0, sizeof(Gvd));
        gvrp_vlan_db_get(pVlanEvent->vid, &vlandb);
        if(pVlanEvent->vid == vlandb.vid)
        {
            osal_time_udelay(500);
            if (VLAN_DYNAMIC  == vlandb.isdynVlan)
            {
                vlandb.isdynVlan = VLAN_STATIC;
                gvrp_vlan_db_set(pVlanEvent->vid, &vlandb);
            }
        }
    }
    else if (SYS_NT_VLAN_ADMINVLAN_DESTROY == event)
    {
        gvrp_vlan_db_size_get(&curVlandb);
        gvrp_del_static_vlan(pVlanEvent->vid);
        if (GVRP_DB_SIZE == curVlandb)
        {
            VLANMASK_CLEAR_ALL(stVlanMsk);
            rsd_vlan_adminEntryExist_get(&stVlanMsk);
            FOR_EACH_VID_IN_VLANMASK(vid, stVlanMsk)
            {
                if (!gvd_find_entry(my_gvr->gvd, vid, &gvd_index))
                    break;
            }
            if (SYS_ERR_OK == rsd_vlan_adminEntry_get(vid, &vlanEntry))
            {
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(lport, vlanEntry.lpmMember)
                {
                    gvrp_add_staticPort(vid, lport);
                    osal_time_udelay(500);
                }
            }
        }

    }
    else if (SYS_NT_VLAN_ADMINENTRY_CHG == event)
    {
        FOR_EACH_VID_IN_VLANMASK(vid, pVlanEvent->vmChanged)
        {
            if (SYS_ERR_OK == rsd_vlan_adminEntry_get(vid, &vlanEntry))
            {
                if (IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmMember, pVlanEvent->port))
                {
                    if (IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmForbidden, pVlanEvent->port))
                    {
                        gvrp_del_staticPort(vid, pVlanEvent->port);
                    }
                    else
                    {
                        gvrp_add_staticPort(vid, pVlanEvent->port);
                    }
                }
                else
                {
                    gvrp_del_staticPort(vid, pVlanEvent->port);
                }

                osal_time_udelay(500);
            }
        }
    }

    GVRP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
static int32 _gvrp_trunk_handler(uint32 event, char *pData)
{
    sys_trunk_event_t *pTrunkEvent;
    sys_logic_portmask_t tmp, trunkMbr;
    uint8 isAdd = FALSE, isDel = FALSE;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    if (!GVRP_ENABLE())
        return SYS_ERR_OK;

    pTrunkEvent = (sys_trunk_event_t *)pData;

    LOGIC_PORTMASK_CLEAR_ALL(trunkMbr);

    SYS_ERR_CHK(rsd_trunk_member_get(pTrunkEvent->trunkIdx, &trunkMbr));
    LOGIC_PORTMASK_ANDNOT(tmp, trunkMbr, pTrunkEvent->portmask);

    if (SYS_NT_TRK_MBR_JOIN == event)
    {
        if (!IS_LOGIC_PORTMASK_CLEAR(tmp))
        {
            isAdd = TRUE; 
        }
    }
    else if (SYS_NT_TRK_MBR_LEAVE == event)
    {
        if (!IS_LOGIC_PORTMASK_CLEAR(trunkMbr))
        {
            isDel = TRUE;
        }
    }

    SYS_ERR_CHK(gvrp_trunk_process(pTrunkEvent, event, isAdd, isDel));

    return SYS_ERR_OK;
}

void gvrp_stats_init(int32 boot)
{
    sys_logic_port_t  port;
    if (boot) {
        osal_memset(&gvrp_cfg, 0, sizeof(gvrp_cfg_t));
        GVRP_EABLE_SET(DISABLED);
        GVRP_HOLD_TIME() = DFLT_GVRP_HOLD_TIME;
        GVRP_JOIN_TIME() =  DFLT_GVRP_JOIN_TIME ;
        GVRP_LEAVE_TIME() = DFLT_GVRP_LEAVE_TIME;
        GVRP_LEAVE_ALL_TIME() = DFLT_GVRP_LEAVEALL_TIME;

        FOR_EACH_LOGIC_PORT(port)
        {
            GVRP_PORT_REGMODE_SET(port, GVRP_NORMAL);
            GVRP_PORT_ENABLE_SET(port, DISABLED);
            GVRP_PORT_FORBIDVLAN_SET(port, DISABLED);
            gid_create_port(my_gvr->garp, port);
            gip_connect_port(my_gvr->garp, port);
        }

        rsd_gvrp_enable_set(DISABLED);
    }
    else
    {
        GVRP_EABLE_SET(0);
        FOR_EACH_LOGIC_PORT(port)
        {
            gvrp_statistics_clear(port);
            gvrp_error_statistics_clear(port);
        }
    }

    return;
}
int32  gvrp_statistics_clear(sys_logic_port_t port)
{

    gvrp_statistics[port].Total_rx = 0;
    gvrp_statistics[port].Total_tx = 0;

    gvrp_statistics[port].JoinEmpty_rx = 0;
    gvrp_statistics[port].JoinIn_rx = 0;
    gvrp_statistics[port].Empty_rx = 0;
    gvrp_statistics[port].LeaveIn_rx = 0;
    gvrp_statistics[port].LeaveEmpty_rx = 0;
    gvrp_statistics[port].LeaveAll_rx = 0;

    gvrp_statistics[port].JoinEmpty_tx = 0;
    gvrp_statistics[port].JoinIn_tx = 0;
    gvrp_statistics[port].Empty_tx = 0;
    gvrp_statistics[port].LeaveIn_tx = 0;
    gvrp_statistics[port].LeaveEmpty_tx = 0;
    gvrp_statistics[port].LeaveAll_tx = 0;
    return SYS_ERR_OK;

}
int32  gvrp_error_statistics_clear(sys_logic_port_t port)
{
    gvrp_error_stat[port].invProt= 0;
    gvrp_error_stat[port].invAtyp= 0;
    gvrp_error_stat[port].invAlen = 0;
    gvrp_error_stat[port].invAval = 0;
    gvrp_error_stat[port].invEvent = 0;

    return SYS_ERR_OK;
}
int32 gvrp_port_statistics_get(sys_logic_port_t port,  gvrp_statistics_t  *pStat)
{

    SYS_PARAM_CHK(NULL == pStat, SYS_ERR_NULL_POINTER);

    pStat->Total_rx = gvrp_statistics[port].Total_rx;
    pStat->Total_tx = gvrp_statistics[port].Total_tx;

    pStat->JoinEmpty_rx = gvrp_statistics[port].JoinEmpty_rx;
    pStat->JoinIn_rx  = gvrp_statistics[port].JoinIn_rx;
    pStat->Empty_rx =  gvrp_statistics[port].Empty_rx;
    pStat->LeaveIn_rx = gvrp_statistics[port].LeaveIn_rx;
    pStat->LeaveEmpty_rx = gvrp_statistics[port].LeaveEmpty_rx;
    pStat->LeaveAll_rx = gvrp_statistics[port].LeaveAll_rx;

    pStat->JoinEmpty_tx = gvrp_statistics[port].JoinEmpty_tx;
    pStat->JoinIn_tx = gvrp_statistics[port].JoinIn_tx;
    pStat->Empty_tx = gvrp_statistics[port].Empty_tx;
    pStat->LeaveIn_tx = gvrp_statistics[port].LeaveIn_tx;
    pStat->LeaveEmpty_tx = gvrp_statistics[port].LeaveEmpty_tx;
    pStat->LeaveAll_tx = gvrp_statistics[port].LeaveAll_tx;
    return SYS_ERR_OK;
}
int32 gvrp_port_error_statistics_get(sys_logic_port_t port,  gvrp_error_statistics_t  *pStat)
{

    SYS_PARAM_CHK(NULL == pStat, SYS_ERR_NULL_POINTER);

    pStat->invProt = gvrp_error_stat[port].invProt;
    pStat->invAtyp = gvrp_error_stat[port].invAtyp;
    pStat->invAlen = gvrp_error_stat[port].invAlen;
    pStat->invAval = gvrp_error_stat[port].invAval;
    pStat->invEvent = gvrp_error_stat[port].invEvent;

    return SYS_ERR_OK;
}
int32 gvrp_vlanMaskExist_get(sys_vlanmask_t *pVlanMask)
{
    uint32 i;
    sys_vlanmask_t vlanMask;

    SYS_PARAM_CHK(NULL == pVlanMask, SYS_ERR_NULL_POINTER);
    osal_memset(&vlanMask,0, sizeof(sys_vlanmask_t));

    GVRP_DB_SEM_LOCK();

    if(my_gvr->garp != NULL)
    {
        for(i = 1; i <= GVRP_DB_SIZE; i++)
        {
            if(my_gvr->gvd[i] >= BOND_VLAN_MIN && my_gvr->gvd[i] <= BOND_VLAN_MAX)
            {
                VLANMASK_SET_VID(vlanMask, my_gvr->gvd[i]);
            }
        }
    }

    osal_memcpy(pVlanMask, &vlanMask, sizeof(sys_vlanmask_t));

    GVRP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 gvrp_gidtt_machine_get(sys_logic_port_t port, sys_vid_t vid, Gid_machine *pMachine)
{
    uint32   gid_index=0;
    Gid       *my_gid;

    SYS_PARAM_CHK(NULL == pMachine, SYS_ERR_NULL_POINTER);

    GVRP_DB_SEM_LOCK();
    if(!gvd_find_entry(my_gvr->gvd,vid, &gid_index))
    {
        GVRP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    if(!gid_find_port( (Gid *)my_gvr->garp->gid, port , &my_gid))
    {
        pMachine->applicant = Vo;
        pMachine->registrar = Mt;
    }
    else
    {
        pMachine->applicant = my_gid->machines[gid_index].applicant;
        pMachine->registrar = my_gid->machines[gid_index].registrar;
    }

    GVRP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;

}
int32 gvrp_enable_set(sys_enable_t enable)
{
    sys_logic_port_t port;
    sys_vid_t vid;
    sys_vlanmask_t  vlanExistMsk;
    
    uint32 gid_index;
    Gid *my_port;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    GVRP_DB_SEM_LOCK();
    if(enable != GVRP_ENABLE() )
    {
        GVRP_EABLE_SET(enable);
        osal_time_usleep(10 * 1000);

        if(enable == ENABLED)
        {
            set_gvrp_db_from_reg();

            FOR_EACH_LOGIC_PORT(port)
            {
                if(!gid_find_port( (Gid *)my_gvr->garp->gid,  port, &my_port))
                    continue;

                if(GVRP_PORT_IS_ENABLE(port))
                {
                    systime_stop_timer(&my_port->sLeaveAll_timer);
                    systime_start_random_timer(my_port->application->process_id,gid_leaveall_timer_expired,
                    my_port->port_no,GVRP_LEAVE_ALL_TIME_N(),&my_port->sLeaveAll_timer, GVRP_TIMER_LEAVEALL);
                    my_port->leaveall_countdown = Gid_leaveall_count;
                    my_port->leaveall_timer_running = TRUE;
                }
            }
        }
        else
        {
            
            FOR_EACH_LOGIC_PORT(port)
            {
                gvrp_statistics_clear(port);
                gvrp_error_statistics_clear(port);

                if(gid_find_port( (Gid *)my_gvr->garp->gid,  port, &my_port))
                {
                    gid_stop_all_timer(my_port);
                    my_port->leaveall_timer_running = FALSE;
                    my_port->join_timer_running = FALSE;
                    my_port->leave_timer_running = FALSE;
                    my_port->hold_timer_running = FALSE;

                }

                for (gid_index=1;gid_index <= GVRP_DB_SIZE;gid_index++)
                {
                        gGid[port].machines[gid_index].applicant= Vo;
                        gGid[port].machines[gid_index].registrar= Mt;
                }

            }
            rsd_vlan_operEntryExist_get(&vlanExistMsk);

            FOR_EACH_VID_IN_VLANMASK(vid, vlanExistMsk)
            {
                if(!gvd_find_entry(my_gvr->gvd,vid,&gid_index))
                    continue;

                if(gvd_is_dynamic_entry(gGvd, vid))
                {
                    SYS_DBG(DBG_VLAN_GVRP, "Delete dynamic VLAN-%d", vid);
                }
                else
                {
                    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, gGvd[gid_index].dynBmp)
                    {
                        LOGIC_PORTMASK_CLEAR_PORT(gGvd[gid_index].dynBmp, port);
                        rsd_vlan_portGvrpVlan_del(port, gGvd[gid_index].vid);
                    }
                }
                gvd_delete_entry(my_gvr->gvd, gid_index);
            }
           gvd_destroy_gvd();
        }

        rsd_gvrp_enable_set(enable);
    }

    GVRP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 gvrp_timer_set(gvrp_timer_type_t time_type, uint32 timeout)
{
    switch(time_type)
    {
    case GVRP_TIMER_HOLD:
        GVRP_HOLD_TIME() = timeout;
        break;
    case GVRP_TIMER_JOIN:
        GVRP_JOIN_TIME() = timeout;
        break;
    case GVRP_TIMER_LEAVE:
        GVRP_LEAVE_TIME() = timeout;
        break;
    case GVRP_TIMER_LEAVEALL:
        GVRP_LEAVE_ALL_TIME() = timeout;
        break;
    default:
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}
int32 gvrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    Gid *my_gid, *to_port = NULL;
    uint32  gid_index;
    sys_vid_t vid;
    sys_vlanmask_t  vlanExistMsk;
    sys_vlan_operEntry_t  operEntry;

    GVRP_DB_SEM_LOCK();
    if(GVRP_PORT_ENABLE(port) == enable)
    {
        GVRP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
    }

    GVRP_PORT_ENABLE_SET(port, enable);
    rsd_vlan_operEntryExist_get(&vlanExistMsk);

    if(ENABLED == enable)
    {
        gid_port_enable(my_gvr->garp, port, ENABLED);
        FOR_EACH_VID_IN_VLANMASK(vid, vlanExistMsk)
        {
            osal_memset(&operEntry, 0, sizeof(sys_vlan_operEntry_t));
            rsd_vlan_operEntry_get(vid, &operEntry);

            if( !gvd_is_dynamic_entry(gGvd, vid)  &&
                gid_find_port((Gid *)my_gvr->garp->gid, port , &my_gid))
            {
                if(IS_LOGIC_PORTMASK_PORTSET(operEntry.lpmMember, port))
                    gvrp_add_staticPort(vid, port);
            }
        }

        if(gid_find_port( (Gid *)my_gvr->garp->gid, port,  &my_gid))
        {
            systime_stop_timer(&my_gid->sLeaveAll_timer);
            systime_start_random_timer(my_gid->application->process_id,gid_leaveall_timer_expired,
                my_gid->port_no,GVRP_LEAVE_ALL_TIME_N(),&my_gid->sLeaveAll_timer, GVRP_TIMER_LEAVEALL);
            my_gid->leaveall_countdown = Gid_leaveall_count;
            my_gid->leaveall_timer_running = TRUE;
        }
    }
    else
    {
        if(!gid_find_port((Gid *)my_gvr->garp->gid, port , &my_gid))
        {
            GVRP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

       gid_port_enable(my_gvr->garp, port, DISABLED);

        FOR_EACH_VID_IN_VLANMASK(vid, vlanExistMsk)
        {
            osal_memset(&operEntry, 0, sizeof(sys_vlan_operEntry_t));
            rsd_vlan_operEntry_get(vid, &operEntry);

            if(IS_LOGIC_PORTMASK_PORTSET(operEntry.lpmMember, port))
            {
                if(gvd_find_entry(my_gvr->gvd,vid,&gid_index))
                {
                    if(IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].dynBmp, port))
                    {
                        my_gvr->garp->gip[gid_index] -= 1;
                        if (0 == my_gvr->garp->gip[gid_index])
                        {
                            to_port = my_gid;
                            while ((to_port = to_port->next_in_connected_ring) != my_gid)
                            {
                               to_port->machines[gid_index].applicant = Vo;
                            }
                        }
                        vfdb_filter(port, vid);
                    }

                    if(IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].portBmp, port))
                    {
                        LOGIC_PORTMASK_CLEAR_PORT(gGvd[gid_index].portBmp, port);
                    }
                }
            }
        }

    }

    GVRP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 gvrp_port_regmode_set(sys_logic_port_t port, register_mode_t  regmode)
{
    sys_vlanmask_t vlanExistMask;
    sys_vid_t  vid;
    uint32 gid_index;
    Gid *my_gid, *to_port;

    GVRP_DB_SEM_LOCK();

    if(regmode == GVRP_PORT_REGMODE_GET(port))
    {
        GVRP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
    }

    GVRP_PORT_REGMODE_SET(port, regmode);

    if(!gid_find_port((Gid *)my_gvr->garp->gid, port, &my_gid))
    {
        GVRP_DB_SEM_UNLOCK();
         return SYS_ERR_FAILED;
    }

   rsd_vlan_operEntryExist_get(&vlanExistMask);

    if (GVRP_NORMAL != regmode)
    {
        if(my_gvr->gvd==NULL)
        {
            GVRP_DB_SEM_UNLOCK();
            return SYS_ERR_FAILED;
        }

        FOR_EACH_VID_IN_VLANMASK(vid, vlanExistMask)
        {
            if( !gvd_find_entry(my_gvr->gvd,vid,&gid_index))
                continue;

            if(IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].dynBmp, port))
            {
                gid_machine_set(my_gid,gid_index,Vo,Mt);
                my_gid->application->gip[gid_index] -= 1;
                if (0 == my_gvr->garp->gip[gid_index])
                {
                    to_port = my_gid;
                    while ((to_port = to_port->next_in_connected_ring) != my_gid)
                    {
                       to_port->machines[gid_index].applicant = Vo;
                    }
                }

                LOGIC_PORTMASK_CLEAR_PORT(gGvd[gid_index].portBmp, port);
                LOGIC_PORTMASK_CLEAR_PORT(gGvd[gid_index].dynBmp, port);
                rsd_vlan_portGvrpVlan_del(port, vid);

                if(IS_LOGIC_PORTMASK_CLEAR(gGvd[gid_index].dynBmp))
                {
                    if(gvd_is_dynamic_entry(gGvd, vid))
                    {
                        gvd_delete_entry(my_gvr->gvd, gid_index);
                    }
                }
            }
        }
    }
    else
    {
        FOR_EACH_VID_IN_VLANMASK(vid, vlanExistMask)
        {
            if( !gvd_find_entry(my_gvr->gvd,vid,&gid_index))
                continue;

            if(IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].portBmp, port))
            {
                my_gid->machines[gid_index].applicant = Vp;
            }
            else
            {
                if (0 != my_gvr->garp->gip[gid_index])
                    my_gid->machines[gid_index].applicant = Vp;
            }
        }
    }

    GVRP_DB_SEM_UNLOCK();
    
    return SYS_ERR_OK;
}
int32 gvrp_port_forbidVlan_set(sys_logic_port_t port, sys_enable_t enable)
{
    if(enable == GVRP_PORT_FORBIDVLAN_GET(port))
        return SYS_ERR_OK;

    GVRP_PORT_FORBIDVLAN_SET(port, enable);

    return SYS_ERR_OK;
}
int32 gvrp_info_get(sys_gvrp_info_t *pInfo)
{

    SYS_PARAM_CHK(NULL == pInfo, SYS_ERR_NULL_POINTER);
    GVRP_DB_SEM_LOCK();
    pInfo->enable =    GVRP_ENABLE();
    pInfo->holdtime = GVRP_HOLD_TIME();
    pInfo->jointime = GVRP_JOIN_TIME();
    pInfo->leavetime = GVRP_LEAVE_TIME();
    pInfo->leavealltime = GVRP_LEAVE_ALL_TIME();
    GVRP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 gvrp_vlan_db_get(sys_vid_t vid, Gvd  *show_db)
{
    uint32    found_at_index;

    if(gvd_find_entry(my_gvr->gvd, vid, &found_at_index))
    {
        osal_memcpy(show_db, &gGvd[found_at_index], sizeof(Gvd));
    }
    else
    {
        osal_memset(show_db, 0, sizeof(Gvd));
    }
    return SYS_ERR_OK;
}
int32 gvrp_vlan_db_size_get(uint32 *pSize)
{
    Vlan_id *my_gvd = my_gvr->gvd;
    uint32 size = 0;
    uint32 i;

    if(my_gvr->garp != NULL)
    {
        for(i = 1; i <= GVRP_DB_SIZE; i++)
        {
            if(my_gvd[i] != 0)
                size++;
        }
        *pSize = size;
        return SYS_ERR_OK;
    }

    return SYS_ERR_FAILED;
}
int32 gvrp_vlan_db_set(sys_vid_t vid, Gvd  *vlandb)
{
    uint32    found_at_index;
    if(gvd_find_entry(my_gvr->gvd, vid, &found_at_index))
    {
        gGvd[found_at_index].isdynVlan =  vlandb->isdynVlan;
        osal_memcpy(&gGvd[found_at_index].portBmp, &vlandb->portBmp, sizeof(sys_logic_portmask_t));
        osal_memcpy(&gGvd[found_at_index].dynBmp, &vlandb->dynBmp, sizeof(sys_logic_portmask_t));
    }
    return SYS_ERR_OK;
}
int  gvrp_init (void)
{
    uint32 port;

    gvrp_sem_pkt = osal_sem_mutex_create();
    gvrp_sem_db = osal_sem_mutex_create();

    osal_memset(gGvd, 0, sizeof(Gvd) * (GVRP_DB_SIZE+1));
    osal_memset(gGid, 0, sizeof(Gid) * SYS_LOGICPORT_NUM_MAX);

    rsd_gvrp_init();

    if ( !gvr_create_gvr(GVRP_PROCESS_ID,  &my_gvr) )
    {
        SYS_DBG(DBG_VLAN_GVRP, "GVRP initialize failed!");
        return SYS_ERR_FAILED;
    }

    gvrp_stats_init(1);

    FOR_EACH_LOGIC_PORT(port)
    {
        gvrp_statistics_clear(port);
        gvrp_error_statistics_clear(port);
        SYS_ERR_CHK(osal_timer_init(&gGid[port].sHold_timer, gid_hold_timer_expired, port));
        SYS_ERR_CHK(osal_timer_init(&gGid[port].sJoin_timer, gid_join_timer_expired, port));
        SYS_ERR_CHK(osal_timer_init(&gGid[port].sLeave_timer, gid_leave_timer_expired, port));
        SYS_ERR_CHK(osal_timer_init(&gGid[port].sLeaveAll_timer, gid_leaveall_timer_expired, port));
    }

    SYS_ERR_CHK(rsd_nic_handler_register(&gvrpHandler));
    SYS_ERR_CHK(sys_notifier_observer_register("gvrp", SYS_NOTIFIER_SUBJECT_TRUNK,
        (SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE), _gvrp_trunk_handler, SYS_NOTIFIER_PRI_MEDIUM));
    SYS_ERR_CHK(sys_notifier_observer_register("gvrp", SYS_NOTIFIER_SUBJECT_VLAN,
        (SYS_NT_VLAN_ADMINVLAN_CREATE | SYS_NT_VLAN_ADMINVLAN_DESTROY | SYS_NT_VLAN_ADMINENTRY_CHG), _gvrp_vlan_handler, SYS_NOTIFIER_PRI_MEDIUM));

    return SYS_ERR_OK;
}
int  gvrp_exit(void)
{
    sys_logic_port_t port;
    osal_sem_mutex_destroy(gvrp_sem_pkt);
    osal_sem_mutex_destroy(gvrp_sem_db);

    SYS_ERR_CHK(rsd_nic_handler_unregister(&gvrpHandler));
    SYS_ERR_CHK(sys_notifier_observer_unregister("gvrp", SYS_NOTIFIER_SUBJECT_TRUNK, _gvrp_trunk_handler ));
    SYS_ERR_CHK(sys_notifier_observer_unregister("gvrp", SYS_NOTIFIER_SUBJECT_VLAN, _gvrp_vlan_handler ));

    FOR_EACH_LOGIC_PORT(port)
    {
        osal_timer_stop(&gGid[port].sHold_timer);
        osal_timer_stop(&gGid[port].sJoin_timer);
        osal_timer_stop(&gGid[port].sLeave_timer);
        osal_timer_stop(&gGid[port].sLeaveAll_timer);
    }
    return SYS_ERR_OK;
}
int32 gvrp_add_staticPort(sys_vid_t vid, sys_logic_port_t lPort)
{
    Garp *application= my_gvr->garp;
    uint32 gid_index;
    Gid *my_gid = NULL;

    if(!GVRP_ENABLE())
    {
        return SYS_ERR_OK;
    }

    if(!GVRP_PORT_ENABLE(lPort))
    {
        return SYS_ERR_OK;
    }

    if(my_gvr->gvd==NULL)
    {
        return SYS_ERR_FAILED;
    }

    if(!gid_find_port((Gid *)application->gid, lPort, &my_gid))
    {
        return SYS_ERR_FAILED;
    }

    SYS_DBG(DBG_VLAN_GVRP, "GVRP Database add static port %s", LPSTR(lPort));

    if (!gvd_find_entry(my_gvr->gvd,vid,&gid_index))
    {
        if (!gvd_create_entry(my_gvr->gvd, vid , VLAN_STATIC, &gid_index))
        {
            if(gid_find_unused( application, 0 , &gid_index))
            {
                gvd_delete_entry(my_gvr->gvd, gid_index);
                gvd_create_entry(my_gvr->gvd, vid, VLAN_STATIC, &gid_index);
            }
            else
            {
                gvr_db_full(my_gvr, my_gid);
                return SYS_ERR_OK;
            }
        }

        gid_machine_set(my_gid,gid_index,Va,Inr);
        LOGIC_PORTMASK_SET_PORT(gGvd[gid_index].portBmp, lPort);
    }
    else
    {
        gid_machine_set(my_gid,gid_index,Va,Inr);
        if (IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].dynBmp, lPort))
        {
            LOGIC_PORTMASK_CLEAR_PORT(gGvd[gid_index].dynBmp, lPort);
            my_gid->application->gip[gid_index] -= 1;
            if (SYS_ERR_OK != rsd_vlan_portGvrpVlan_del(lPort, vid))
            {
                SYS_DBG(DBG_VLAN_GVRP,"delete the the dynimic vlan port failure!");
            }
        }

        LOGIC_PORTMASK_SET_PORT(gGvd[gid_index].portBmp , lPort);
        gGvd[gid_index].isdynVlan = VLAN_STATIC;

    }

    if(!my_gid->join_timer_running && !IS_LP_TRKMBR(lPort))
    {
        systime_stop_timer(&my_gid->sJoin_timer);
        systime_start_timer(my_gid->application->process_id,gid_join_timer_expired,
            my_gid->port_no,GVRP_JOIN_TIME(),&my_gid->sJoin_timer);
        my_gid->join_cnt = GVRP_JOIN_CNT;
        my_gid->join_timer_running=TRUE;
    }

    return SYS_ERR_OK;

}

int32 gvrp_del_staticPort(sys_vid_t vid, sys_logic_port_t lPort)
{
    Garp *application= my_gvr->garp;
    uint32 gid_index;
    sys_logic_portmask_t staticPmsk;
    Gid *my_gid = NULL;
    enum Applicant_states app = Vo;
    enum Registrar_states reg = Mt;

    if(!GVRP_ENABLE())
    {
        return SYS_ERR_OK;
    }

    if(!GVRP_PORT_ENABLE(lPort))
    {
        return SYS_ERR_OK;
    }

    SYS_DBG(DBG_VLAN_GVRP, "GVRP Database delete static port %s", LPSTR(lPort));

    if(my_gvr->gvd==NULL)
    {
        return SYS_ERR_FAILED;
    }

    if(!gid_find_port((Gid *)application->gid, lPort, &my_gid))
    {
        return SYS_ERR_FAILED;
    }

    if (!gvd_find_entry(my_gvr->gvd,vid,&gid_index))
    {
        SYS_DBG(DBG_VLAN_GVRP,"The GVRP VLAN %d entry is not exist", vid);
        return SYS_ERR_FAILED;
    }

    LOGIC_PORTMASK_CLEAR_ALL(staticPmsk);
    LOGIC_PORTMASK_ANDNOT(staticPmsk, gGvd[gid_index].portBmp, gGvd[gid_index].dynBmp);

    if (!IS_LOGIC_PORTMASK_PORTSET(staticPmsk, lPort))
    {
        SYS_DBG(DBG_VLAN_GVRP,"The GVRP VLAN %d  port %s  not exist", vid, LPSTR(lPort));
        return SYS_ERR_OK;
    }

    LOGIC_PORTMASK_CLEAR_PORT(gGvd[gid_index].portBmp , lPort);
    gid_machine_get(my_gid,gid_index,&app, &reg);
    if (0 == my_gvr->garp->gip[gid_index])
        app = Vo;
    gid_machine_set(my_gid,gid_index,app,Mt);

    return SYS_ERR_OK;

}

int32 gvrp_del_static_vlan(sys_vid_t vid)
{
    Garp *application= my_gvr->garp;
    Gid *my_gid;
    uint32 gid_index;
    sys_logic_port_t port;

    if(!GVRP_ENABLE())
    {
        return SYS_ERR_OK;
    }

    if(my_gvr->gvd==NULL)
    {
        return SYS_ERR_FAILED;
    }

    if(gvd_find_entry(my_gvr->gvd,vid ,&gid_index))
    {
        FOR_EACH_LOGIC_PORT(port)
        {
            if(!gid_find_port((Gid *)application->gid, port , &my_gid))
            {
                continue;
            }

            my_gid->application->leave_propagated_fn(my_gid->application,my_gid, gid_index);

            gid_machine_set(my_gid,gid_index,Vo,Mt);
        }
        
        gvd_delete_entry(my_gvr->gvd, gid_index);
    }

    return SYS_ERR_OK;
}
int32 gvrp_stp_connect_port(sys_logic_port_t  port_no)
{

    return SYS_ERR_OK;
}
int32 gvrp_stp_disconnect_port(sys_logic_port_t port_no)
{
    return SYS_ERR_OK;
}
