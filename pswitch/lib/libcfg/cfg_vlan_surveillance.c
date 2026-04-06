
#include <libcfg/cfg.h>

int32 cfg_surveillance_vlanVid_get(sys_vid_t *pVid)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    *pVid = pSV->vid;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlanVid_set(sys_vid_t vid)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    pSV->vid = vid;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlanPri_get(sys_pri_t *pPri)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK((NULL == pPri), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    *pPri = pSV->pri;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlanPri_set(sys_pri_t pri)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK(IS_PRI_INVALID(pri), SYS_ERR_PRIORITY);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    pSV->pri = pri;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlanState_get(sys_enable_t *pState)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    *pState = pSV->state;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlanState_set(sys_enable_t state)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    pSV->state = state;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlanAging_get(uint32 *pAging)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK((NULL == pAging), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    *pAging = pSV->aging;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlanAging_set(uint32 aging)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    pSV->aging = aging;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlanOui_get(uint32 idx, surveillance_vlan_oui_t *pOui)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    osal_memcpy(pOui, &pSV->ouiInfo[idx], sizeof(surveillance_vlan_oui_t));

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlanOui_set(uint32 idx, surveillance_vlan_oui_t *pOui)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    osal_memcpy(&pSV->ouiInfo[idx], pOui, sizeof(surveillance_vlan_oui_t));

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlan1pRemarkEnable_get(sys_enable_t *pEnable)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    *pEnable = pSV->remarkEnable;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlan1pRemarkEnable_set(sys_enable_t enable)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    pSV->remarkEnable = enable;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlan_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    *pEnable = pSV->portEnable[port];

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlan_portEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    pSV->portEnable[port] = enable;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlan_portMode_get(sys_logic_port_t port, surveillance_vlan_port_mode_t *pMode)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    *pMode = pSV->portMode[port];

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlan_portMode_set(sys_logic_port_t port, surveillance_vlan_port_mode_t mode)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((mode >= SURVEILLANCE_VLAN_PORT_MODE_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    pSV->portMode[port] = mode;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

int32 cfg_surveillance_vlan_portCosMode_get(sys_logic_port_t port, surveillance_vlan_cos_mode_t *pCosMode)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCosMode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE, (cfg_addr)&pSV);

    *pCosMode = pSV->cosMode[port];

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);

}

int32 cfg_surveillance_vlan_portCosMode_set(sys_logic_port_t port, surveillance_vlan_cos_mode_t cosMode)
{
    cfg_surveillance_vlan_t *pSV = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((cosMode >= SURVEILLANCE_VLAN_PORT_COS_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_VLAN_SURVEILLANCE,(cfg_addr)&pSV);

    pSV->cosMode[port]= cosMode;

    CFG_CLOSE(CFG_ID_VLAN_SURVEILLANCE);
}

