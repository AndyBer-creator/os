
#include <libcfg/cfg.h>

int32 cfg_dos_typeEnable_set(sys_dos_type_t type, sys_enable_t enable)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK((type >= DOS_TYPE_END), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    dos->typeEnable[type] = enable;

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_typeEnable_get(sys_dos_type_t type, sys_enable_t *pEnable)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK((type >= DOS_TYPE_END), SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    *pEnable = dos->typeEnable[type];

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_maxPingLen_set(uint32 length)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK((length > BOND_DOS_PINGMAXLENGTH_MAX), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    dos->maxPingLen = length;

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_maxPingLen_get(uint32 *pLength)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK(NULL == pLength, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    *pLength = dos->maxPingLen;

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_minIPv6FragLen_set(uint32 length)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK((length > BOND_DOS_IPV6MINFRAGLENGTH_MAX), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    dos->minIPv6FragLen = length;

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_minIPv6FragLen_get(uint32 *pLength)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK(NULL == pLength, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    *pLength = dos->minIPv6FragLen;

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_smurfNetmaskLen_set(uint32 length)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK((length > BOND_DOS_SMURFLENGTH_MAX), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    dos->smurfNetmaskLen = length;

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_smurfNetmaskLen_get(uint32 *pLength)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK(NULL == pLength, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    *pLength = dos->smurfNetmaskLen;

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_minTCPHdrLen_set(uint32 length)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK((length > BOND_DOS_TCPHDRMINCHKLENGTH_MAX), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    dos->minTCPHdrLen = length;

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_minTCPHdrLen_get(uint32 *pLength)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK(NULL == pLength, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    *pLength = dos->minTCPHdrLen;

    CFG_CLOSE(CFG_ID_DOS);

    return SYS_ERR_OK;
}

int32 cfg_dos_portEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    dos->enable[port] = enable;

    CFG_CLOSE(CFG_ID_DOS);
}

int32 cfg_dos_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dos_t *dos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DOS, (cfg_addr)&dos);

    *pEnable = dos->enable[port];

    CFG_CLOSE(CFG_ID_DOS);
}

