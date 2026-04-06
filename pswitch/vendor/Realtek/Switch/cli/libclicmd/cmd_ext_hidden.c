
#include <libcmd/cmd.h>

int32 cmd_ext_hidden_parserOnly_chk(struct cmd_element *cmd, struct vty *vty)
{
    return TRUE;
}

int32 cmd_ext_hidden_if_chk(struct cmd_element *cmd, struct vty *vty)
{
    if (vty->node != CONFIG_NODE)
        return TRUE;

    return FALSE;
}

#ifdef CONFIG_SYS_L3_ROUTE

int32 cmd_ext_hidden_ifRange_chk(struct cmd_element *cmd, struct vty *vty)
{
    if (g_isIntfRange)
        return TRUE;

    return FALSE;
}
#endif

int32 cmd_ext_hidden_nonFiberPort_chk(struct cmd_element *cmd, struct vty *vty)
{
    sys_logic_port_t lp;
    sys_port_media_t media;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_port_linkMedia_get(lp, &media), lp);

        if(PORT_MEDIA_FIBER != media || IS_TRKLP(lp) || IS_TGE_PORT(lp))
        {
            return TRUE;
        }
    }

    return FALSE;
}

int32 cmd_ext_hidden_nonComboPort_chk(struct cmd_element *cmd, struct vty *vty)
{
    sys_logic_port_t lp;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        if(!IS_COMBO_PORT(lp) || IS_TRKLP(lp))
        {
            return TRUE;
        }
    }

    return FALSE;
}

int32 cmd_ext_hidden_nonGigaPort_chk(struct cmd_element *cmd, struct vty *vty)
{
    sys_logic_port_t lp, rPort;
    sys_port_type_t type;
    sys_logic_portmask_t memberPm;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        type = LP2TYPE(lp);
        if(PORT_TYPE_LAG == type) 
        {
            LP2TRKMBRLPM(lp, &memberPm);
            if(!IS_LOGIC_PORTMASK_CLEAR(memberPm))
            {
                FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(rPort,memberPm)
                    break;

                type = LP2TYPE(rPort);
            }
        }

        if(PORT_TYPE_100M == type || PORT_TYPE_COMBO_100M == type || PORT_TYPE_2500M == type || PORT_TYPE_10G == type)
        {
            return TRUE;
        }
    }

    return FALSE;
}

int32 cmd_ext_hidden_nonTenGigaPort_chk(struct cmd_element *cmd, struct vty *vty)
{
    sys_logic_port_t lp, rPort;
    sys_port_type_t type;
    sys_logic_portmask_t memberPm;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        type = LP2TYPE(lp);
        if(PORT_TYPE_LAG == type) 
        {
            LP2TRKMBRLPM(lp, &memberPm);
            if(!IS_LOGIC_PORTMASK_CLEAR(memberPm))
            {
                FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(rPort,memberPm)
                    break;

                type = LP2TYPE(rPort);
            }
        }

        if (PORT_TYPE_10G != type)
        {
            return TRUE;
        }
    }

    return FALSE;
}

int32 cmd_ext_hidden_tenGigaPort_chk(struct cmd_element *cmd, struct vty *vty)
{
    sys_logic_port_t lp, rPort;
    sys_port_type_t type;
    sys_logic_portmask_t memberPm;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        type = LP2TYPE(lp);
        if(PORT_TYPE_LAG == type) 
        {
            LP2TRKMBRLPM(lp, &memberPm);
            if(!IS_LOGIC_PORTMASK_CLEAR(memberPm))
            {
                FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(rPort,memberPm)
                    break;

                type = LP2TYPE(rPort);
            }
        }

        if (PORT_TYPE_10G == type)
        {
            return TRUE;
        }
    }

    return FALSE;
}

int32 cmd_ext_hidden_2_5GigaPort_chk(struct cmd_element *cmd, struct vty *vty)
{
    sys_logic_port_t lp, rPort;
    sys_port_type_t type;
    sys_logic_portmask_t memberPm;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        type = LP2TYPE(lp);
        if(PORT_TYPE_LAG == type) 
        {
            LP2TRKMBRLPM(lp, &memberPm);
            if(!IS_LOGIC_PORTMASK_CLEAR(memberPm))
            {
                FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(rPort,memberPm)
                    break;

                type = LP2TYPE(rPort);
            }
        }

        if(PORT_TYPE_100M == type || PORT_TYPE_COMBO_1000M == type || PORT_TYPE_10G == type || PORT_TYPE_1000M == type)
        {
            return TRUE;
        }
    }

    return FALSE;
}

int32 cmd_ext_hidden_autoAllPort_chk(struct cmd_element *cmd, struct vty *vty)
{
    sys_logic_port_t lp, rPort;
    sys_port_type_t type;
    sys_logic_portmask_t memberPm;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        type = LP2TYPE(lp);
        if(PORT_TYPE_LAG == type) 
        {
            LP2TRKMBRLPM(lp, &memberPm);
            if(!IS_LOGIC_PORTMASK_CLEAR(memberPm))
            {
                FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(rPort,memberPm)
                    break;

                type = LP2TYPE(rPort);
            }
        }
    }

    return FALSE;
}

int32 cmd_ext_hidden_auto1GPort_chk(struct cmd_element *cmd, struct vty *vty)
{
    sys_logic_port_t lp, rPort;
    sys_port_type_t type;
    sys_logic_portmask_t memberPm;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        type = LP2TYPE(lp);
        if(PORT_TYPE_LAG == type) 
        {
            LP2TRKMBRLPM(lp, &memberPm);
            if(!IS_LOGIC_PORTMASK_CLEAR(memberPm))
            {
                FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(rPort,memberPm)
                    break;

                type = LP2TYPE(rPort);
            }
        }
        
        if (PORT_TYPE_10G == type || PORT_TYPE_2500M == type)
        {
            return TRUE;
        }
    }

    return FALSE;
}

int32 cmd_ext_hidden_auto2500Port_chk(struct cmd_element *cmd, struct vty *vty)
{
    sys_logic_port_t lp, rPort;
    sys_port_type_t type;
    sys_logic_portmask_t memberPm;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        type = LP2TYPE(lp);
        if(PORT_TYPE_LAG == type) 
        {
            LP2TRKMBRLPM(lp, &memberPm);
            if(!IS_LOGIC_PORTMASK_CLEAR(memberPm))
            {
                FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(rPort,memberPm)
                    break;

                type = LP2TYPE(rPort);
            }
        }
        
        if (PORT_TYPE_10G == type || PORT_TYPE_1000M == type || PORT_TYPE_COMBO_1000M == type)
        {
            return TRUE;
        }
    }

    return FALSE;
}

