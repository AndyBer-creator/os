
#include <common/sys_def.h>
#include <libsal/sal_util.h>
#include <libsal/sal_dhcp_snooping.h>

int32 sal_dhcp_snooping_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dhcp_snooping_enable_set(enable));
    SYS_ERR_CHK(cfg_dhcp_snooping_enable_set(enable));

    return SYS_ERR_OK;
} 

int32 sal_dhcp_snooping_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_snooping_enable_get(pEnable));

    return SYS_ERR_OK;
} 

int32 sal_dhcp_snooping_vlan_enable_set (sys_vid_t vid, sys_enable_t enable)
{
    sys_vlanmask_t vm;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dhcp_snooping_vlan_enable_set(vid, enable));
    SYS_ERR_CHK(cfg_dhcp_snooping_vlanmask_enable_get(&vm));

    if (ENABLED == enable)
        VLANMASK_SET_VID(vm, vid);
    else
        VLANMASK_CLEAR_VID(vm, vid);

    SYS_ERR_CHK(cfg_dhcp_snooping_vlanmask_enable_set(&vm));

    return SYS_ERR_OK;
} 

int32 sal_dhcp_snooping_vlan_enable_get (sys_vid_t vid, sys_enable_t *pEnable)
{
    sys_vlanmask_t vm;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_dhcp_snooping_vlanmask_enable_get(&vm));

    if (VLANMASK_IS_VIDSET(vm, vid))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}

int32 sal_dhcp_snooping_vlanmask_enable_get (sys_vlanmask_t *vlanmask)
{
    SYS_PARAM_CHK((NULL == vlanmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_snooping_vlanmask_enable_get(vlanmask));

    return SYS_ERR_OK;
}

int32 sal_dhcp_snooping_port_trust_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dhcp_snooping_port_trust_set(port, enable));
    SYS_ERR_CHK(cfg_dhcp_snooping_port_trust_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_snooping_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_snooping_port_trust_get(port, pEnable));

    return SYS_ERR_OK;
} 

int32 sal_dhcp_snooping_portChaddrChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dhcp_snooping_portChaddrChk_set(port, enable));
    SYS_ERR_CHK(cfg_dhcp_snooping_port_chaddrChk_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_snooping_portChaddrChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_snooping_port_chaddrChk_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_snooping_portMib_get(sys_logic_port_t port, sys_dhcp_mib_t *pCounter)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCounter), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(dhcp_snooping_portMib_get(port, pCounter));

    return SYS_ERR_OK;
}

int32 sal_dhcp_snooping_portMib_clear(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(dhcp_snooping_portMib_clear(port));

    return SYS_ERR_OK;
}

int32 sal_dhcp_snooping_rateLimit_get(sys_logic_port_t port, uint32 *pRate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pRate), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_snooping_rateLimit_get(port, pRate));

    return SYS_ERR_OK;
}

int32 sal_dhcp_snooping_rateLimit_set(sys_logic_port_t port, uint32 rate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(dhcp_snooping_rateLimit_set(port, rate));

    SYS_ERR_CHK(cfg_dhcp_snooping_rateLimit_set(port, rate));

    return SYS_ERR_OK;
}

int32 sal_dhcp_option82_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dhcp_option82_enable_set(port, enable));
    SYS_ERR_CHK(cfg_dhcp_option82_enable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_option82_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pEnable == NULL), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_option82_enable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_option82_action_set(sys_logic_port_t port, sys_dhcp_option82_act_t action)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((action >= DHCP_OPTION82_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(dhcp_option82_action_set(port, action));
    SYS_ERR_CHK(cfg_dhcp_option82_action_set(port, action));

    return SYS_ERR_OK;
}

int32 sal_dhcp_option82_action_get(sys_logic_port_t port, sys_dhcp_option82_act_t *pAction)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pAction == NULL), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_option82_action_get(port, pAction));

    return SYS_ERR_OK;
}

int32 sal_dhcp_option82_remoteID_set(sys_dhcp_option82_id_t *pID)
{
    SYS_PARAM_CHK((pID == NULL), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(dhcp_option82_remoteID_set(pID));
    SYS_ERR_CHK(cfg_dhcp_option82_remoteID_set(pID));

    return SYS_ERR_OK;
}

int32 sal_dhcp_option82_remoteID_get(sys_dhcp_option82_id_t *pID)
{
    SYS_PARAM_CHK((pID == NULL), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_dhcp_option82_remoteID_get(pID));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID

int32 sal_dhcp_option82_format_set(sys_logic_port_t port, sys_dhcp_option82_format_t format)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((format >= DHCP_OPTION82_FORMAT_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(dhcp_option82_format_set(port, format));
    SYS_ERR_CHK(cfg_dhcp_option82_format_set(port, format));

    return SYS_ERR_OK;
}

int32 sal_dhcp_option82_format_get(sys_logic_port_t port, sys_dhcp_option82_format_t *pFormat)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pFormat == NULL), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_option82_format_get(port, pFormat));

    return SYS_ERR_OK;
}

int32 sal_dhcp_option82_circuitID_set(sys_logic_port_t port, sys_dhcp_option82_circuit_id_t *pID)
{
    uint32 i = 0;
    int32 emptyIndex = -1;
    sys_dhcp_option82_circuit_id_t cid[SYS_DHCP_O82_CID_VLAN_MAX+1];

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pID == NULL), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(dhcp_option82_circuitID_set(port, pID));

    SYS_MEM_CLEAR(cid);
    SYS_ERR_CHK(cfg_dhcp_option82_circuitID_get(port, cid));

    if (0 == pID->vid)
    {
        if (0 == osal_strlen(pID->str))
            SYS_MEM_CLEAR(cid[SYS_DHCP_O82_CID_VLAN_MAX]);
        else
            osal_memcpy(&cid[SYS_DHCP_O82_CID_VLAN_MAX], pID, sizeof(sys_dhcp_option82_circuit_id_t));

        SYS_ERR_CHK(cfg_dhcp_option82_circuitID_set(port, cid));

        return SYS_ERR_OK;
    }
    else
    {
        if (0 == osal_strlen(pID->str))
        {
            for (i = 0 ; i < SYS_DHCP_O82_CID_VLAN_MAX ; i++)
            {
                if (cid[i].vid == pID->vid)
                {
                    SYS_MEM_CLEAR(cid[i]);
                    SYS_ERR_CHK(cfg_dhcp_option82_circuitID_set(port, cid));

                    return SYS_ERR_OK;
                }
            }
        }
        else
        {
            for (i = 0 ; i < SYS_DHCP_O82_CID_VLAN_MAX ; i++)
            {
                if ((-1 == emptyIndex) &&  (0 == cid[i].vid))
                    emptyIndex = i;

                if (cid[i].vid == pID->vid)
                {
                    osal_memcpy(&cid[i], pID, sizeof(sys_dhcp_option82_circuit_id_t));
                    break;
                }
            }

            if (i == SYS_DHCP_O82_CID_VLAN_MAX)
            {
                if (-1 == emptyIndex)
                    return SYS_ERR_ENTRY_FULL;

                if (0 == osal_strlen(pID->str))
                    return SYS_ERR_OK;

                osal_memcpy(&cid[emptyIndex], pID, sizeof(sys_dhcp_option82_circuit_id_t));
            }

            SYS_ERR_CHK(cfg_dhcp_option82_circuitID_set(port, cid));
        }
    }

    return SYS_ERR_OK;
}

int32 sal_dhcp_option82_circuitID_get(sys_logic_port_t port, sys_dhcp_option82_circuit_id_t *pID)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pID == NULL), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_dhcp_option82_circuitID_get(port, pID));

    return SYS_ERR_OK;
}
#endif
