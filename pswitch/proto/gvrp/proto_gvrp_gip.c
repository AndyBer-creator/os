#include "gvrp_common.h"
extern Gvr *my_gvr;
extern gvrp_cfg_t gvrp_cfg;
extern Gvd   gGvd[GVRP_DB_SIZE+1];
static void gip_connect_into_ring(Gid *my_port)
{
    Gid *first_connected, *last_connected;

    my_port->is_connected = TRUE;
    my_port->next_in_connected_ring = my_port;
    first_connected = my_port;

    do
    {
        first_connected = first_connected->next_in_port_ring;
    } while (!first_connected->is_connected);

    my_port->next_in_connected_ring = first_connected;
    last_connected = first_connected;

    while(last_connected->next_in_connected_ring != first_connected)
    {
        last_connected = last_connected->next_in_connected_ring;
    }

    last_connected->next_in_connected_ring = my_port;
}

static void gip_disconnect_from_ring(Gid *my_port)
{
    Gid *first_connected, *last_connected;

    first_connected = my_port->next_in_connected_ring;
    my_port->next_in_connected_ring = my_port;
    my_port->is_connected = FALSE;
    last_connected = first_connected;

    while(last_connected->next_in_connected_ring != my_port)
    {
        last_connected = last_connected->next_in_connected_ring;
    }

    last_connected->next_in_connected_ring = first_connected;
}
void gip_connect_port(Garp *application, uint32 port_no)
{

    Gid     *my_port;

    if (gid_find_port((Gid *)application->gid, port_no, &my_port))
    {

        if ((!my_port->is_enabled) || (my_port->is_connected))
        {
            return;
        }

        gip_connect_into_ring(my_port);

#if 0
        for(gid_index = 1; gid_index <= application->last_gid_used;gid_index++)
        {
            if (gip_propagates_to(my_port, gid_index))
            {
                gid_join_request(my_port, gid_index);
            }
            if (gid_registered_here(my_port, gid_index))
            {
                gip_propagate_join(my_port, gid_index);
            }
        }
        gip_do_actions(my_port);
#endif

        my_port->is_connected = TRUE;
    } 

}
void gip_disconnect_port(Garp *application, uint32 port_no)
{

    Gid     *my_port;

    if (gid_find_port((Gid *)application->gid, port_no, &my_port))
    {

        if ((!my_port->is_enabled) || (!my_port->is_connected))
        {
            return;
        }
#if 0
        for (gid_index = 1; gid_index <= application->last_gid_used;gid_index++)
        {

            if (gip_propagates_to(my_port, gid_index))
            {
                if(!IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].dynBmp, port_no))
                    continue;
                gid_leave_request(my_port, gid_index);
            }
            if (gid_registered_here(my_port, gid_index))
            {
                if(!IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].dynBmp, port_no))
                    continue;
                gip_propagate_leave(my_port, gid_index);
            }
        }
#endif

        gip_do_actions(my_port);

        gip_disconnect_from_ring(my_port);
        my_port->is_connected = FALSE;

    }

} 
void gip_propagate_join(Gid *my_port, uint32 gid_index)
{
    uint32 joining_members;
    Gid     *to_port;

    if (my_port->is_connected)
    {
        
        my_port->application->gip[gid_index] += 1;
        joining_members = my_port->application->gip[gid_index];

        if (joining_members <= 2)
        {
            to_port = my_port;

            while ((to_port = to_port->next_in_connected_ring) != my_port)
            {
                
                if ((joining_members == 1) || (gid_registered_here(to_port, gid_index)))
                {
                    gid_join_request(to_port, gid_index);
                    to_port->application->join_propagated_fn(my_port->application, my_port, gid_index);  
                }
            }

        }
    }

}
void gip_propagate_leave(Gid *my_port, uint32 gid_index)
{
    uint32 remaining_members;
    Gid     *to_port;

    if (my_port->is_connected)
    {
        my_port->application->gip[gid_index] -= 1;
        remaining_members = my_port->application->gip[gid_index];

        if(remaining_members<0)
        {
            my_port->application->gip[gid_index]=0;
            remaining_members=0;
        }

       if (gid_registered_here(my_port, gid_index))
       {
            my_port->machines->registrar = Mt;
       }

        if(remaining_members < 1)
        {
            to_port = my_port;
            while ((to_port = to_port->next_in_connected_ring) != my_port)
            {
                
                if(gid_registered_here(to_port, gid_index))  
                {
                   gid_leave_request(to_port, gid_index);   
                   
                }
            }
        }
    } 

}
BOOL gip_propagates_to(Gid *my_port, uint32 gid_index)
{
    if ( (my_port->is_connected)&& ((my_port->application->gip[gid_index] == 2)
        ||((my_port->application->gip[gid_index] == 1) && (!gid_registered_here(my_port, gid_index)))))
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}
void gip_do_actions(Gid *my_port)
{
    Gid *this_port = my_port;
    do
    {
        gid_do_actions(this_port);
    } while ((this_port = this_port->next_in_connected_ring) != my_port);

}

void gip_ring_info(Garp *application, uint8 gip_ring[])
{
    Gid        *first_connected;
    uint32    first_port=0,i=0;

    if((first_connected=application->gid) !=NULL)
    {

        first_port=first_connected->port_no;

        do {
            first_connected = first_connected->next_in_port_ring;
        }   while (!first_connected->is_connected && first_connected->port_no!= first_port);

        if(first_connected->is_connected)
        {
            first_port=first_connected->port_no;

            do
            {
                gip_ring[i++]=(first_connected->port_no)+1;
                first_connected=first_connected->next_in_connected_ring;
            }   while(first_connected->port_no !=first_port && i< SYS_LOGICPORT_NUM_MAX);

        }

        gip_ring[i]=END_OF_GIP;

    }
    else
    { 
        gip_ring[0]=END_OF_GIP;
    }

}

