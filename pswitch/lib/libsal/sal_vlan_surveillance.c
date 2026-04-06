
#include <libsal/sal_util.h>
#include <libsal/sal_vlan_surveillance.h>
#include <surveillancevlan/proto_surveillancevlan.h>

int32 sal_surveillance_vlanVid_get(sys_vid_t *pVid)
{
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_surveillance_vlanVid_get(pVid));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanVid_set(sys_vid_t vid)
{
    sys_vid_t otherVid;
    sys_enable_t enable;
    sys_logic_port_t lp;

    SYS_PARAM_CHK(IS_SYSTEM_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&otherVid));

    if(otherVid == vid)
        return SYS_ERR_OK;

    if(DFLT_VLAN_RESERVED_VLAN != vid)
    {
        SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
        SYS_ERR_CHK(sal_vlan_exclusive_chk(SYS_VLAN_CATE_SURVEILLANCE, vid));
    }
    else
    {
        SYS_ERR_CHK(sal_surveillance_vlanState_get(&enable));

        if(ENABLED == enable)
            return SYS_ERR_VLAN_SV_VID_ENABLE;
    }

    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_ERR_CHK(sal_surveillance_vlan_portEnable_get(lp, &enable));
        if(ENABLED == enable)
            return SYS_ERR_VLAN_SV_ENABLED_PORT;
    }

    SYS_ERR_CHK(surveillance_vlanVid_set(vid));
    SYS_ERR_CHK(cfg_surveillance_vlanVid_set(vid));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanPri_get(sys_pri_t *pPri)
{
    SYS_PARAM_CHK((NULL == pPri), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_surveillance_vlanPri_get(pPri));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanPri_set(sys_pri_t pri)
{
    sys_pri_t oldPri;

    SYS_PARAM_CHK(IS_PRI_INVALID(pri), SYS_ERR_PRIORITY);

    SYS_ERR_CHK(sal_surveillance_vlanPri_get(&oldPri));

    if (oldPri == pri)
        return SYS_ERR_OK;

    SYS_ERR_CHK(surveillance_vlanPri_set(pri));
    SYS_ERR_CHK(cfg_surveillance_vlanPri_set(pri));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanState_get(sys_enable_t *pState)
{
    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_surveillance_vlanState_get(pState));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanState_set(sys_enable_t state)
{
    sys_enable_t oldState;
    sys_logic_port_t lp;
    sys_vid_t vid;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(state), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_surveillance_vlanState_get(&oldState));
    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&vid));

    if(oldState == state)
        return SYS_ERR_OK;

    if(ENABLED == state && DFLT_VLAN_RESERVED_VLAN == vid)
        return SYS_ERR_VLAN_SV_NOT_EXIST;

    if(DISABLED == state)
    {
        FOR_EACH_LOGIC_PORT(lp)
        {
            SYS_ERR_CHK(sal_surveillance_vlan_portEnable_get(lp, &oldState));
            if(ENABLED == oldState)
                return SYS_ERR_VLAN_SV_ENABLED_PORT;
        }
    }

    SYS_ERR_CHK(surveillance_vlanState_set(state));
    SYS_ERR_CHK(cfg_surveillance_vlanState_set(state));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanAging_get(uint32 *pAging)
{
    SYS_PARAM_CHK((NULL == pAging), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_surveillance_vlanAging_get(pAging));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanAging_set(uint32 aging)
{
    uint32 oldAging;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(aging, BOND_VLAN_SURVEILLANCE_AGING_MIN, BOND_VLAN_SURVEILLANCE_AGING_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(sal_surveillance_vlanAging_get(&oldAging));

    if (oldAging == aging)
        return SYS_ERR_OK;

    SYS_ERR_CHK(surveillance_vlanAging_set(aging));
    SYS_ERR_CHK(cfg_surveillance_vlanAging_set(aging));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanOuibyIdx_get(uint32 idx, surveillance_vlan_oui_t *pOui)
{
    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_surveillance_vlanOui_get(idx, pOui));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanOuiByIdx_set(uint32 idx, surveillance_vlan_oui_t *pOui)
{
    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(surveillance_vlanOuiByIdx_set(idx, pOui));
    SYS_ERR_CHK(cfg_surveillance_vlanOui_set(idx, pOui));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanOui_add(surveillance_vlan_oui_t *pOui)
{
    uint32 i = 0;
    uint32 emptyIdx = CAPA_VLAN_SURVEILLANCE_OUI_NUM;
    surveillance_vlan_oui_t oui;

    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        SYS_MEM_CLEAR(oui);
        SYS_ERR_CHK(sal_surveillance_vlanOuibyIdx_get(i, &oui));

        if (!IS_OUI_CLEAR(oui.oui))
        {
            if ((oui.oui[0] == pOui->oui[0]) &&
                (oui.oui[1] == pOui->oui[1]) &&
                (oui.oui[2] == pOui->oui[2]))
                return SYS_ERR_VLAN_SV_OUI_EXIST;
            else
                continue;
        }
        else if (CAPA_VLAN_SURVEILLANCE_OUI_NUM == emptyIdx)
            emptyIdx = i;
    }

    if (CAPA_VLAN_SURVEILLANCE_OUI_NUM == emptyIdx)
        return SYS_ERR_ENTRY_FULL;

    SYS_ERR_CHK(sal_surveillance_vlanOuiByIdx_set(emptyIdx, pOui));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlanOui_del(surveillance_vlan_oui_t *pOui)
{
    uint32 i = 0;
    surveillance_vlan_oui_t oui;

    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);

    for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        SYS_MEM_CLEAR(oui);
        SYS_ERR_CHK(sal_surveillance_vlanOuibyIdx_get(i, &oui));

        if (0 == osal_memcmp(pOui->oui, oui.oui, SNAPOUI_LEN))
        {
            SYS_MEM_CLEAR(oui);
            SYS_ERR_CHK(sal_surveillance_vlanOuiByIdx_set(i, &oui));

            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_ENTRY_NOT_FOUND;
}

int32 sal_surveillance_vlan1pRemarkEnable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_surveillance_vlan1pRemarkEnable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlan1pRemarkEnable_set(sys_enable_t enable)
{
    sys_enable_t oldEnable;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_surveillance_vlan1pRemarkEnable_get(&oldEnable));

    if (oldEnable == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(surveillance_vlan1pRemarkEnable_set(enable));
    SYS_ERR_CHK(cfg_surveillance_vlan1pRemarkEnable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlan_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_surveillance_vlan_portEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlan_portEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    sys_vid_t vid;
    sys_enable_t oldEnable = DISABLED;
    sys_vlan_membership_t membership;
    surveillance_vlan_port_mode_t portMode;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(sal_surveillance_vlan_portEnable_get(port,&oldEnable));

    if (oldEnable == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_surveillance_vlanState_get(&oldEnable));

    if(ENABLED == enable && DISABLED == oldEnable)
         return SYS_ERR_VLAN_SV_GLOBAL_DISABLE;

    SYS_ERR_CHK(sal_surveillance_vlan_portMode_get(port,&portMode));

    if(SURVEILLANCE_VLAN_PORT_MODE_AUTO == portMode)
    {
        if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_GENERAL) | (1 << SYS_VLAN_PORT_MODE_TRUNK)))
            return SYS_ERR_VLAN_SV_PORT_MODE;

        SYS_ERR_CHK(sal_surveillance_vlanVid_get(&vid));
        SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_get(port,vid,&membership));

        if((SYS_VLAN_TAGGED == membership) ||(SYS_VLAN_UNTAGGED == membership))
            return SYS_ERR_VLAN_SV_PORT_STATIC;
    }

    SYS_ERR_CHK(surveillance_vlan_portEnable_set(port, enable));
    SYS_ERR_CHK(cfg_surveillance_vlan_portEnable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlan_portMode_get(sys_logic_port_t port, surveillance_vlan_port_mode_t *pMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_surveillance_vlan_portMode_get(port, pMode));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlan_portMode_set(sys_logic_port_t port, surveillance_vlan_port_mode_t mode)
{
    sys_vid_t vid;
    sys_enable_t enable;
    sys_vlan_membership_t membership;
    surveillance_vlan_port_mode_t oldMode;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((mode >= SURVEILLANCE_VLAN_PORT_MODE_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_surveillance_vlan_portMode_get(port,&oldMode));

    if (oldMode == mode)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_surveillance_vlan_portEnable_get(port,&enable));

    if((ENABLED == enable) && (SURVEILLANCE_VLAN_PORT_MODE_AUTO == mode))
    {
        if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_GENERAL) | (1 << SYS_VLAN_PORT_MODE_TRUNK)))
                return SYS_ERR_VLAN_SV_PORT_MODE;

        SYS_ERR_CHK(sal_surveillance_vlanVid_get(&vid));
        SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_get(port,vid,&membership));

        if((SYS_VLAN_UNTAGGED == membership) || (SYS_VLAN_TAGGED == membership))
            return SYS_ERR_VLAN_SV_PORT_STATIC;
    }

    SYS_ERR_CHK(surveillance_vlan_portMode_set(port, mode));
    SYS_ERR_CHK(cfg_surveillance_vlan_portMode_set(port, mode));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlan_portCosMode_get(sys_logic_port_t port, surveillance_vlan_cos_mode_t *pCosMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCosMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_surveillance_vlan_portCosMode_get(port,pCosMode));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlan_portCosMode_set(sys_logic_port_t port, surveillance_vlan_cos_mode_t cosMode)
{
    surveillance_vlan_cos_mode_t oldMode;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((cosMode>= SURVEILLANCE_VLAN_PORT_COS_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_surveillance_vlan_portCosMode_get(port,&oldMode));

    if (oldMode == cosMode)
        return SYS_ERR_OK;

    SYS_ERR_CHK(surveillance_vlan_portCosMode_set(port,cosMode));
    SYS_ERR_CHK(cfg_surveillance_vlan_portCosMode_set(port,cosMode));

    return SYS_ERR_OK;
}

int32 sal_surveillance_vlan_port_chk(sys_logic_port_t port)
{
    sys_enable_t enable;
    surveillance_vlan_port_mode_t portMode;

    SYS_ERR_CHK(sal_surveillance_vlanState_get(&enable));

    if(ENABLED == enable)
    {
        SYS_ERR_CHK(sal_surveillance_vlan_portEnable_get(port,&enable));
        SYS_ERR_CHK(sal_surveillance_vlan_portMode_get(port,&portMode));

        if((ENABLED == enable) &&(SURVEILLANCE_VLAN_PORT_MODE_AUTO == portMode))
            return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

