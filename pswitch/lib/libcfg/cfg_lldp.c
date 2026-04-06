
#include <libcfg/cfg.h>

int32 cfg_lldp_enable_set(sys_enable_t enable)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->enable = enable;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_enable_get(sys_enable_t *pEnable)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pEnable = pLldp->enable;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_port_set(sys_logic_port_t port, uint8 lldpPortState)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->lldpPortState[port] = lldpPortState;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_port_get(sys_logic_port_t port ,uint8 *pLldpPortState)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLldpPortState), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pLldpPortState = pLldp->lldpPortState[port];

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_holdtime_set(uint8 holdtime)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(holdtime, BOND_LLDP_HOLDTIME_MIN, BOND_LLDP_HOLDTIME_MAX), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->holdtime = holdtime;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_holdtime_get(uint8 *pHoldtime)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK((NULL == pHoldtime), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pHoldtime = pLldp->holdtime;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_txInterval_set(uint32 tx_interval)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(tx_interval, BOND_LLDP_TX_INTERVAL_MIN, BOND_LLDP_TX_INTERVAL_MAX), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->tx_interval = tx_interval;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_txInterval_get(uint32 *pTxInterval)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK((NULL == pTxInterval), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pTxInterval = pLldp->tx_interval;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_reinitDelay_set(uint8 reinit_delay)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(reinit_delay, BOND_LLDP_REINIT_DELAY_MIN, BOND_LLDP_REINIT_DELAY_MAX), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->reinit_delay = reinit_delay;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_reinitDelay_get(uint8 *pReinitDelay)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK((NULL == pReinitDelay), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pReinitDelay = pLldp->reinit_delay;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_tx_delay_set(uint16 txDelay)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(txDelay, BOND_LLDP_TX_DELAY_MIN, BOND_LLDP_TX_DELAY_MAX), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->tx_delay = txDelay;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_tx_delay_get(uint16 *pTxDelay)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK((NULL == pTxDelay), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pTxDelay = pLldp->tx_delay;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_txOverloadStatus_set(sys_logic_port_t port, uint8 txOverloadStatus)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->txOverloading[port] = txOverloadStatus;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_txOverloadStatus_get(sys_logic_port_t port ,uint8 *pTxOverloadStatus)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTxOverloadStatus), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pTxOverloadStatus = pLldp->txOverloading[port];

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_portTlvPvidEnable_set(sys_logic_port_t port, sys_enable_t pvidEnable)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->tlvPvidEnable[port] = pvidEnable;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_portTlvPvidEnable_get(sys_logic_port_t port, sys_enable_t *pPvidEnable)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPvidEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pPvidEnable = pLldp->tlvPvidEnable[port];

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_portTlvVlanmask_set(sys_logic_port_t port, sys_vlanmask_t vlanmask)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->tlvVlanmask[port] = vlanmask;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_portTlvVlanmask_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pVlanmask = pLldp->tlvVlanmask[port];

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_disableAction_set(lldp_disable_action_t action)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK((action >= LLDP_DISABLE_ACTION_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->action = action;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_disableAction_get(lldp_disable_action_t *pAction)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK((NULL == pAction), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pAction = pLldp->action;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_portTlvSel_set(sys_logic_port_t port, lldp_tlv_sel_t lldpTlvSel)
{
    cfg_lldp_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    pLldp->lldpTlvSel[port] = lldpTlvSel;

    CFG_CLOSE(CFG_ID_LLDP);
}

int32 cfg_lldp_portTlvSel_get(sys_logic_port_t port, lldp_tlv_sel_t *pLldpTlvSel)
{
    cfg_lldp_t *pLldp = NULL;

    CFG_OPEN(CFG_ID_LLDP, (cfg_addr)&pLldp);

    *pLldpTlvSel = pLldp->lldpTlvSel[port];

    CFG_CLOSE(CFG_ID_LLDP);
}

