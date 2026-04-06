#include "gvrp_common.h"

extern Gvr *my_gvr;
extern gvrp_cfg_t gvrp_cfg;
extern Gvd gGvd[GVRP_DB_SIZE+1];
extern osal_mutex_t            gvrp_sem_db;
void gvd_destroy_gvd(void)
{
    Garp *application=my_gvr->garp;

    application->last_gid_used=0;
    osal_memset(application->gip, 0, sizeof(uint32) * (GVRP_DB_SIZE +1));
    osal_memset(gGvd, 0, sizeof(Gvd) * (GVRP_DB_SIZE+1));
}
BOOL gvd_find_entry(void *gvd, Vlan_id key,uint32 *found_at_index)
{
    Vlan_id *my_gvd=(Vlan_id *)gvd;
    uint32 i=1;

    for(i=1;i<= GVRP_DB_SIZE;i++)
    {
        if( my_gvd[i]==key)
        {
            *found_at_index=i;
            return (TRUE);
        }
    }

    return (FALSE);
}

BOOL gvd_index_isExsit(void *gvd, uint32 index)
{
    Vlan_id *my_gvd=(Vlan_id *)gvd;
    uint32 i=1;

    for(i=1;i<= GVRP_DB_SIZE;i++)
    {
        if( my_gvd[index] != 0)
        {
            return (TRUE);
        }
    }

    return (FALSE);
}

BOOL gvd_is_dynamic_entry(Gvd *gvd, Vlan_id vid)
{
    uint32 i;

    for(i=1;i<=GVRP_DB_SIZE;i++)
    {
        if(gvd[i].vid == vid)
        {
            if (VLAN_DYNAMIC == gvd[i].isdynVlan)
                return TRUE;
        }
    }

    return FALSE;
}

int32 gvr_vlan_nameStr_get(sys_vid_t vid, sys_namestr_t *pName)
{
    char *pVlanStr;
    char *pPrefix = DFLT_VLAN_PREFIX_NAME;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pPrefix), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    if (!(vid / 10))
        pVlanStr = "%s000%d";
    else if (!(vid /100))
        pVlanStr = "%s00%d";
    else if (!(vid /1000))
        pVlanStr = "%s0%d";
    else
        pVlanStr = "%s%d";

    osal_sprintf(pName->str, pVlanStr, pPrefix, vid);

    return SYS_ERR_OK;
}
BOOL gvd_create_entry(void *gvd, Vlan_id key, uint8 isDyn, uint32 *created_at_index)
{
    Vlan_id *my_gvd=(Vlan_id *)gvd;
    Garp *application= my_gvr->garp;
    uint32 i=1;
    sys_namestr_t sysVlanName;

    for(i=1;i<=GVRP_DB_SIZE;i++)
    {
        if(my_gvd[i]==0)
        { 
            *created_at_index=i;
            my_gvd[i]=key;

            if(application->last_gid_used < i)
                application->last_gid_used=i;

            osal_memset(&gGvd[i], 0, sizeof(Gvd));

            gGvd[i].vid = key;
            gGvd[i].isdynVlan = isDyn;

            if(isDyn)
            {
                SYS_DBG(DBG_VLAN_GVRP, "Create dynamic vlan = %d", key);

                osal_memset(&sysVlanName,0, sizeof(sys_namestr_t));
                if (SYS_ERR_OK != gvr_vlan_nameStr_get(key, &sysVlanName))
                        return FALSE;

                if(SYS_ERR_OK != rsd_vlan_gvrpVlan_create(key, &sysVlanName))
                {
                    SYS_DBG(DBG_VLAN_GVRP,"GVD create vlan failure!");
                    return FALSE;
                }
                else
                {
                    GVRP_LOG(LOG_VLAN_GVRP_CREATE, key);
                }
            }

            return(TRUE);
        }
    }

    return (FALSE);
}
int32 gvd_delete_entry(void *gvd,uint32  delete_at_index)
{
    Vlan_id *my_gvd=(Vlan_id *)gvd;
    sys_logic_port_t port;
    Garp *application= my_gvr->garp;
    Gid   *my_gid;
    sys_vid_t  vid;

    vid = my_gvd[delete_at_index];
    my_gvd[delete_at_index] = 0;
    osal_memset(&gGvd[delete_at_index],0,sizeof(Gvd));
    if (SYS_ERR_OK != rsd_vlan_gvrpVlan_destroy(vid))
        SYS_DBG(DBG_VLAN_GVRP,"delete the vlan-database vlan %d hw failure!", vid);

     FOR_EACH_LOGIC_PORT(port)
     {
        if(!gid_find_port((Gid *)application->gid, port , &my_gid))
        {
            continue;
        }

       VLANMASK_SET_VID(my_gid->leave_vlanmask, vid);

        if (FALSE == my_gid->hold_timer_running)
        {
            systime_stop_timer(&my_gid->sHold_timer);
            systime_start_timer(my_gid->application->process_id, gid_hold_timer_expired,
                my_gid->port_no, GVRP_HOLD_TIME(),&my_gid->sHold_timer);
                my_gid->hold_timer_running = TRUE;
        }
     }

    return SYS_ERR_OK;
}
BOOL gvd_get_key(void *gvd, uint32 index, Vlan_id *key)
{
    Vlan_id *my_gvd=(Vlan_id *)gvd;

    if(my_gvd[index]!=0)
    { 
        *key=my_gvd[index];
        return(TRUE);
    }

    *key=0; 
    return(FALSE);
}
void set_gvrp_db_from_reg(void)
{
    sys_vlan_operEntry_t  operEntry;
    sys_vlanmask_t vlanExistMask;
    
    sys_vid_t vid;
    
    sys_logic_port_t port;
    rsd_vlan_operEntryExist_get(&vlanExistMask);

    FOR_EACH_VID_IN_VLANMASK(vid, vlanExistMask)
    {
        if( !gvd_is_dynamic_entry(gGvd, vid))
        {
            osal_memset(&operEntry, 0, sizeof(sys_vlan_operEntry_t));
            SYS_ERR_CONTINUE(rsd_vlan_operEntry_get(vid, &operEntry));

             FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, operEntry.lpmMember)
             {
                if (GVRP_PORT_ENABLE(port))
                    gvrp_add_staticPort(vid, port);
             }
         }
    }
}
void set_gvrp_db_from_reg_init(void)
{
    sys_vlan_operEntry_t  operEntry;
    sys_vlanmask_t vlanExistMask;
    
    sys_vid_t vid;
    uint32 gid_index, dynVlanIndex;
    sys_logic_port_t port;
    Gid *my_gid;

    rsd_vlan_operEntryExist_get(&vlanExistMask);

    FOR_EACH_VID_IN_VLANMASK(vid, vlanExistMask)
    {
        if( !gvd_is_dynamic_entry(gGvd, vid))
        {
            osal_memset(&operEntry, 0, sizeof(sys_vlan_operEntry_t));
            if(SYS_ERR_OK != rsd_vlan_operEntry_get(vid, &operEntry))
                continue;

            if(!gvd_find_entry(my_gvr->gvd, vid, &dynVlanIndex))  
            {
                gvd_create_entry(my_gvr->gvd, vid, VLAN_STATIC, &gid_index);

                FOR_EACH_LOGIC_PORT(port)
                {
                    
                    if(IS_LOGIC_PORTMASK_PORTSET(operEntry.lpmMember, port))
                    {
                        if(!gid_find_port( (Gid *)(my_gvr->garp->gid) ,port , &my_gid))
                            continue;

                        gid_machine_set(my_gid,gid_index,Va,Inr);
                        gip_propagate_join(my_gid,gid_index);

                    }
                }
            }
        }
    }
}
void reset_gvrp_db(void)
{
    uint32  i=0;
    sys_logic_port_t port;
    Garp   *application= my_gvr->garp;
    Gid    *my_gid;

    FOR_EACH_LOGIC_PORT(port)
    {

        if(!gid_find_port((Gid *)application->gid, port , &my_gid))
        {
            continue;
        }

        for(i = 1; i <= application->last_gid_used; i++)
        {
            gid_machine_set(my_gid,i,Vo,Mt);
        }
    }

    for (i = 1; i <= application->last_gid_used; i++)
    {
        application->gip[i]=0; 
        gvd_delete_entry(my_gvr->gvd,i); 
    }

}
