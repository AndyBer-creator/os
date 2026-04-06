
#include <libcfg/cfg.h>

int32 cfg_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    pPort->enable[port] = enable;

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    *pEnable = pPort->enable[port];

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_fiberAutoNegoEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    pPort->fiberAutoNegoEbl[port] = enable;

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_fiberAutoNegoEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    *pEnable = pPort->fiberAutoNegoEbl[port];

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_speed_set(sys_logic_port_t port, sys_port_speed_t speed)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    osal_memcpy(&pPort->speed[port], &speed, sizeof(sys_port_speed_t));

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_speed_get(sys_logic_port_t port, sys_port_speed_t *pSpeed)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pSpeed, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    osal_memcpy(pSpeed, &pPort->speed[port], sizeof(sys_port_speed_t));

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_duplex_set(sys_logic_port_t port, sys_port_duplex_t duplex)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    osal_memcpy(&pPort->duplex[port], &duplex, sizeof(sys_port_duplex_t));

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_duplex_get(sys_logic_port_t port, sys_port_duplex_t *pDuplex)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pDuplex, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    osal_memcpy(pDuplex, &pPort->duplex[port], sizeof(sys_port_duplex_t));

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_fiberSpeed_set(sys_logic_port_t port, sys_port_speed_t speed)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    osal_memcpy(&pPort->fiberSpeed[port], &speed, sizeof(sys_port_speed_t));

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_fiberSpeed_get(sys_logic_port_t port, sys_port_speed_t *pSpeed)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pSpeed, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    osal_memcpy(pSpeed, &pPort->fiberSpeed[port], sizeof(sys_port_speed_t));

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_flowCtrlAuto_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    pPort->fcAuto[port] = enable;

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_flowCtrlAuto_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    *pEnable = pPort->fcAuto[port];

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_flowCtrl_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    pPort->fc[port] = enable;

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_flowCtrl_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    *pEnable = pPort->fc[port];

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_backpressureEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    *pEnable = pPort->backPressure[port];

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_backpressureEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    pPort->backPressure[port] = enable;

    CFG_CLOSE(CFG_ID_PORT);
}

#ifdef CONFIG_SYS_PORT_ISOLATION

int32 cfg_port_protected_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    pPort->pretected[port] = enable;

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_protected_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    *pEnable = pPort->pretected[port];

    CFG_CLOSE(CFG_ID_PORT);
}
#endif

#ifdef CONFIG_SYS_PORT_DESCRIPTION

int32 cfg_port_descp_set(sys_logic_port_t port, char *pDescp)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pDescp, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    osal_memset(pPort->description[port], 0, sizeof(pPort->description[port]));
    osal_strcpy(pPort->description[port], pDescp);

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_descp_get(sys_logic_port_t port, char *pDescp)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pDescp, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);

    osal_strcpy(pDescp, pPort->description[port]);

    CFG_CLOSE(CFG_ID_PORT);
}
#endif

int32 cfg_port_in_pkt_rate_set(sys_logic_port_t port, uint32 type, uint64 value)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);
	switch(type)
	{
    	case TYPE_IN_PKT:
    		pPort->last5inRatePkt[port] = value;
    		break;
    	case TYPE_IN_BYTE:
    		pPort->last5inRatebyte[port] = value;
    		break;
    	case TYPE_OUT_PKT:
    		pPort->last5OutRatePkt[port] = value;
    		break;
    	case TYPE_OUT_BYTE:
    		pPort->last5OutRatebyte[port] = value;
    		break;
    	default:
			break;
    }

    CFG_CLOSE(CFG_ID_PORT);
}

int32 cfg_port_in_pkt_rate_get(sys_logic_port_t port, uint32 type, uint64 *value)
{
    cfg_port_t *pPort = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_PORT, (cfg_addr)&pPort);
	switch(type)
	{
    	case TYPE_IN_PKT:
    		*value = pPort->last5inRatePkt[port];
    		break;
    	case TYPE_IN_BYTE:
    		*value = pPort->last5inRatebyte[port];
    		break;
    	case TYPE_OUT_PKT:
    		*value = pPort->last5OutRatePkt[port];
    		break;
    	case TYPE_OUT_BYTE:
    		*value = pPort->last5OutRatebyte[port];
    		break;
    	default:
			break;
    }

    CFG_CLOSE(CFG_ID_PORT);
}

