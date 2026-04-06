#include "gvrp_common.h"
extern      Gvr *my_gvr;
extern      gvrp_cfg_t  gvrp_cfg;
extern      Gvd  gGvd[GVRP_DB_SIZE+1];
extern      osal_mutex_t            gvrp_sem_db;
extern      Gid                 gGid[SYS_LOGICPORT_NUM_MAX];
extern       uint32  Gid_leaveall_count;
BOOL vfdb_filter( uint32 port_no, uint16 vlan_id)
{
    sys_vlan_operEntry_t  operEntry;
    sys_vlan_adminEntry_t  adminEntry;
    uint32 dynvlanIndex = 0;
    uint8 isDyn = FALSE;

    SYS_DBG(DBG_VLAN_GVRP,"Delete port_no=%s vlan_id=%u", LPSTR(port_no),vlan_id);

    osal_memset(&operEntry, 0, sizeof(sys_vlan_operEntry_t));
    if (SYS_ERR_OK != rsd_vlan_operEntry_get(vlan_id, &operEntry))
    {
        SYS_DBG(DBG_VLAN_GVRP,"get the vlan hw failure!");
        return FALSE;
    }
    else
    {
        
        if (!gvd_find_entry(my_gvr->gvd, vlan_id, &dynvlanIndex))
        {
            return FALSE;
        }

        osal_memset(&adminEntry, 0, sizeof(sys_vlan_adminEntry_t));
        rsd_vlan_adminEntry_get(vlan_id, &adminEntry);

        if (IS_LOGIC_PORTMASK_PORTSET(adminEntry.lpmMember, port_no))
        {
            SYS_DBG(DBG_VLAN_GVRP,"Port %s is static port in the VLAN %d",LPSTR(port_no), vlan_id);
            return FALSE;
        }

        LOGIC_PORTMASK_CLEAR_PORT(gGvd[dynvlanIndex].dynBmp, port_no);
        LOGIC_PORTMASK_CLEAR_PORT(gGvd[dynvlanIndex].portBmp, port_no);

        SYS_DBG(DBG_VLAN_GVRP,"Delete Port %s for Vlan %d", vlan_id, LPSTR(port_no));
        GVRP_LOG(LOG_VLAN_GVRP_MBR_DEL, LPSTR(port_no), vlan_id);

        if (IS_LOGIC_PORTMASK_CLEAR(gGvd[dynvlanIndex].portBmp))
        {
            if(gvd_is_dynamic_entry(gGvd, vlan_id))
                isDyn = TRUE;

            if (SYS_ERR_OK == gvd_delete_entry(my_gvr->gvd, dynvlanIndex))  
            {
                if (isDyn)
                    GVRP_LOG(LOG_VLAN_GVRP_DESTROY, vlan_id);

                return TRUE;
            }
            else
            {
                SYS_DBG(DBG_VLAN_GVRP,"delete the dynimic vlan hw failure!");
                return FALSE;
            }
        }

        if (IS_LP_TRKMBR(port_no))
            return TRUE;

        if (SYS_ERR_OK != rsd_vlan_portGvrpVlan_del(port_no, vlan_id))
        {
            SYS_DBG(DBG_VLAN_GVRP,"set the dynimic vlan hw failure!");
            return FALSE;
        }
    }

    return TRUE;
}
BOOL vfdb_forward(uint32 port_no, uint16 vlan_id)
{
    sys_vlan_operEntry_t  operEntry;
    sys_vlan_adminEntry_t  adminEntry;
    uint32 dynvlanIndex = 0;
    SYS_DBG(DBG_VLAN_GVRP,"Add  port_no=%s vlan_id=%u", LPSTR(port_no),vlan_id);

    osal_memset(&operEntry, 0, sizeof(sys_vlan_operEntry_t));
    if (SYS_ERR_OK != rsd_vlan_operEntry_get(vlan_id, &operEntry))
    {
        SYS_DBG(DBG_VLAN_GVRP,"get the vlan hw failure!");
        return FALSE;
    }
    if (!gvd_find_entry(my_gvr->gvd, vlan_id, &dynvlanIndex))
    {
        return FALSE;
    }

    osal_memset(&adminEntry, 0, sizeof(sys_vlan_adminEntry_t));
    rsd_vlan_adminEntry_get(vlan_id, &adminEntry);

    if (IS_LOGIC_PORTMASK_PORTSET(adminEntry.lpmMember, port_no))
    {
        SYS_DBG(DBG_VLAN_GVRP,"port %s is static port in vlan %d", LPSTR(port_no), vlan_id);
        return FALSE;
    }

    LOGIC_PORTMASK_SET_PORT(gGvd[dynvlanIndex].dynBmp, port_no);
    LOGIC_PORTMASK_SET_PORT(gGvd[dynvlanIndex].portBmp, port_no);

    SYS_DBG(DBG_VLAN_GVRP,"Add Port %s for Vlan %d", LPSTR(port_no), vlan_id);
    GVRP_LOG(LOG_VLAN_GVRP_MBR_ADD, LPSTR(port_no), vlan_id);

    if (SYS_ERR_OK != rsd_vlan_portGvrpVlan_add(port_no, vlan_id))
    {
        SYS_DBG(DBG_VLAN_GVRP,"set the dynimic vlan hw failure!");
        return FALSE;
    }

    return TRUE;
}

int32 gvrp_trunk_process(sys_trunk_event_t *pTrunkEvent,  uint32 event, uint8 isAdd, uint8 isDel)
{
    sys_logic_port_t port;
    Garp *application =  my_gvr->garp;
    Gid *my_gid = NULL, *to_port;
    uint32 gid_index;

    SYS_PARAM_CHK((NULL == pTrunkEvent), SYS_ERR_NULL_POINTER);

    GVRP_DB_SEM_LOCK();

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, pTrunkEvent->portmask)
    {
        if(!gid_find_port((Gid *)application->gid, port, &my_gid))
        {
            continue;
        }

        if (isAdd || SYS_NT_TRK_MBR_JOIN == event)
        {
            my_gid->is_enabled = DISABLED;
            gid_stop_all_timer(my_gid);

            for (gid_index=1;gid_index <= GVRP_DB_SIZE;gid_index++)
            {
                if (IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].dynBmp, port))
                {
                    gGid[port].machines[gid_index].applicant= Vo;
                    gGid[port].machines[gid_index].registrar= Mt;

                     application->gip[gid_index] -= 1;
                     if (0 == application->gip[gid_index])
                     {
                            to_port = my_gid;
                            while ((to_port = to_port->next_in_connected_ring) != my_gid)
                            {
                               to_port->machines[gid_index].applicant = Vo;
                            }
                    }
                     vfdb_filter(port, gGvd[gid_index].vid);
                }
            }
         }

         if (isDel || SYS_NT_TRK_MBR_LEAVE == event)
         {
            my_gid->is_enabled = ENABLED;
            systime_stop_timer(&my_gid->sLeaveAll_timer);
            systime_start_random_timer(my_gid->application->process_id,gid_leaveall_timer_expired,
                my_gid->port_no,GVRP_LEAVE_ALL_TIME_N(),&my_gid->sLeaveAll_timer, GVRP_TIMER_LEAVEALL);
                my_gid->leaveall_countdown = Gid_leaveall_count;
                 my_gid->leaveall_timer_running = TRUE;

            for (gid_index=1;gid_index <= GVRP_DB_SIZE;gid_index++)
            {
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
    }

#if 0
    rsd_vlan_operEntryExist_get(&vlanExist);
    FOR_EACH_VID_IN_VLANMASK(vid, vlanExist)
    {
        SYS_ERR_CHK(rsd_vlan_operEntry_get(vid, &operEntry));

        if (!gvd_find_entry(my_gvr->gvd, vid, &dynvlanIndex))
        {
            continue;
        }

        if (isAdd || isDel)
        {
            if (!IS_LOGIC_PORTMASK_PORTSET(gGvd[dynvlanIndex].dynBmp, TRK2LP(pTrunkEvent->trunkIdx)))
                continue;
        }
        else
        {
            LOGIC_PORTMASK_CLEAR_ALL(chgMbr);
            LOGIC_PORTMASK_AND(chgMbr, gGvd[dynvlanIndex].dynBmp, pTrunkEvent->portmask);
            if (IS_LOGIC_PORTMASK_CLEAR(chgMbr))
                continue;

            if (SYS_NT_TRK_MBR_JOIN == event)
            {
                
                LOGIC_PORTMASK_ANDNOT(operEntry.lpmMember, operEntry.lpmMember, chgMbr);
            }
            else
            {
                LOGIC_PORTMASK_OR(operEntry.lpmMember, operEntry.lpmMember, chgMbr);
            }
        }
        rsd_vlan_operEntry_set(vid, &operEntry);
    }
#endif

    GVRP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}

