
#include <libsal/sal_util.h>
#include <libsal/sal_dos.h>

int32 sal_dos_typeEnable_set(sys_dos_type_t type, sys_enable_t enable)
{
    SYS_PARAM_CHK((type >= DOS_TYPE_END), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_dos_typeEnable_set(type, enable));
    SYS_ERR_CHK(cfg_dos_typeEnable_set(type, enable));

    return SYS_ERR_OK;
}

int32 sal_dos_typeEnable_get(sys_dos_type_t type, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((type >= DOS_TYPE_END), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dos_typeEnable_get(type, pEnable));

    return SYS_ERR_OK;
}

int32 sal_dos_maxPingLen_set(uint32 length)
{
    SYS_PARAM_CHK((length > BOND_DOS_PINGMAXLENGTH_MAX), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_dos_maxPingLen_set(length));
    SYS_ERR_CHK(cfg_dos_maxPingLen_set(length));

    return SYS_ERR_OK;
}

int32 sal_dos_maxPingLen_get(uint32 *pLength)
{
    SYS_PARAM_CHK(NULL == pLength, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dos_maxPingLen_get(pLength));

    return SYS_ERR_OK;
}

int32 sal_dos_minIPv6FragLen_set(uint32 length)
{
    SYS_PARAM_CHK((length > BOND_DOS_IPV6MINFRAGLENGTH_MAX), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_dos_minIPv6FragLen_set(length));
    SYS_ERR_CHK(cfg_dos_minIPv6FragLen_set(length));

    return SYS_ERR_OK;
}

int32 sal_dos_minIPv6FragLen_get(uint32 *pLength)
{
    SYS_PARAM_CHK(NULL == pLength, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dos_minIPv6FragLen_get(pLength));

    return SYS_ERR_OK;
}

int32 sal_dos_smurfNetmaskLen_set(uint32 length)
{
    SYS_PARAM_CHK((length > BOND_DOS_SMURFLENGTH_MAX), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_dos_smurfNetmaskLen_set(length));
    SYS_ERR_CHK(cfg_dos_smurfNetmaskLen_set(length));

    return SYS_ERR_OK;
}

int32 sal_dos_smurfNetmaskLen_get(uint32 *pLength)
{
    SYS_PARAM_CHK(NULL == pLength, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dos_smurfNetmaskLen_get(pLength));

    return SYS_ERR_OK;
}

int32 sal_dos_minTCPHdrLen_set(uint32 length)
{
    SYS_PARAM_CHK((length > BOND_DOS_TCPHDRMINCHKLENGTH_MAX), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_dos_minTCPHdrLen_set(length));
    SYS_ERR_CHK(cfg_dos_minTCPHdrLen_set(length));

    return SYS_ERR_OK;
}

int32 sal_dos_minTCPHdrLen_get(uint32 *pLength)
{
    SYS_PARAM_CHK(NULL == pLength, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dos_minTCPHdrLen_get(pLength));

    return SYS_ERR_OK;
}

int32 sal_dos_portAttackPreventEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_dos_portEnable_set(port, enable));

    SYS_ERR_CHK(cfg_dos_portEnable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dos_portAttackPreventEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dos_portEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

