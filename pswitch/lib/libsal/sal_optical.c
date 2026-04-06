
#include <libsal/sal_port.h>
#include <libsal/sal_optical.h>
#include <libsal/sal_fiber.h>

#if defined(CONFIG_SYS_OPTICALMOD_STATUS)
const sys_text_t text_optical_trxType[] =
{
    {   "Unknown"   },
    {   "GBIC"      },
    {   "SFF"       },
    {   "SFP/SFP+"  },
};

const sys_text_t text_optical_connectType[] =
{
	{   "Unknown"                       },
	{   "SC"                            },
	{   "FC style1 copper connector"    },
	{   "FC style2 copper connector"    },
	{   "BNC/TNC"                       },
	{   "FC coaxial headers"            },
	{   "Fiber jack"                    },
	{   "LC"                            },
	{   "MT-RJ"                         },
	{   "MU"                            },
	{   "SG"                            },
	{   "Optical pigtail"               },
	{   "MPO parallel optics"           },
	{   "HSSDC II"                      },
	{   "Copper pigtail"                },
	{   "RJ45"                          },
};

const sys_text_t text_optical_ethCompliance[] =
{
    {   "Unknown"       },
    {   "10G BASE-SR"   },
    {   "10G BASE-LR"   },
    {   "10G BASE-LRM"  },
    {   "10G BASE-ER"   },
    {   "1000BASE-SX"   },
    {   "1000BASE-LX"   },
    {   "1000BASE-CX"   },
    {   "1000BASE-T"    },
    {   "100BASE-LX"    },
    {   "100BASE-FX"    },
    {   "BASE-BX10"     },
    {   "BASE-PX"       },
};

const sys_text_t text_optical_trxMedia[] =
{
    {   "Unknown"               },
    {   "Single Mode"           },
    {   "Multi Mode (50um)"     },
    {   "Multi Mode (62.5um)"   },
    {   "Video Coqax"           },
    {   "Miniature Boax"        },
    {   "Twisted Pair"          },
    {   "Two Axial Pair"        },
};
#endif

#if defined(CONFIG_SYS_OPTICALMOD_STATUS)

#define _OPTICAL_DIAG_MON_TYPE_LEGACY           (1 << 7)
#define _OPTICAL_DIAG_MON_TYPE_DITIGAL          (1 << 6)
#define _OPTICAL_DIAG_MON_INTERNAL_CALIBRATION  (1 << 5)
#define _OPTICAL_DIAG_MON_EXTERNAL_CALIBRATION  (1 << 4)

#define _OPTICAL_ENHANCED_SEVERITY              (1 << 7)
#define _OPTICAL_ENHANCED_TX_DISABLED           (1 << 6)
#define _OPTICAL_ENHANCED_TX_FAULT              (1 << 5)
#define _OPTICAL_ENHANCED_RX_LOS                (1 << 4)

#define _OPTICAL_OPTIONAL_STATUS_TX_FAULT       (1 << 2)
#define _OPTICAL_OPTIONAL_STATUS_RX_LOS         (1 << 1)
#define _OPTICAL_OPTIONAL_STATUS_DATA_READY     (1 << 0)

#define _OPTICAL_DDM_ALARM_TEMP_HI              ((1 << 7) << 8)
#define _OPTICAL_DDM_ALARM_TEMP_LO              ((1 << 6) << 8)
#define _OPTICAL_DDM_ALARM_VOLT_HI              ((1 << 5) << 8)
#define _OPTICAL_DDM_ALARM_VOLT_LO              ((1 << 4) << 8)
#define _OPTICAL_DDM_ALARM_TX_BIAS_HI           ((1 << 3) << 8)
#define _OPTICAL_DDM_ALARM_TX_BIAS_LO           ((1 << 2) << 8)
#define _OPTICAL_DDM_ALARM_TX_PWR_HI            ((1 << 1) << 8)
#define _OPTICAL_DDM_ALARM_TX_PWR_LO            ((1 << 0) << 8)
#define _OPTICAL_DDM_ALARM_RX_PWR_HI            (1 << 7)
#define _OPTICAL_DDM_ALARM_RX_PWR_LO            (1 << 6)

#define _OPTICAL_DDM_WARN_TEMP_HI               ((1 << 7) << 8)
#define _OPTICAL_DDM_WARN_TEMP_LO               ((1 << 6) << 8)
#define _OPTICAL_DDM_WARN_VOLT_HI               ((1 << 5) << 8)
#define _OPTICAL_DDM_WARN_VOLT_LO               ((1 << 4) << 8)
#define _OPTICAL_DDM_WARN_TX_BIAS_HI            ((1 << 3) << 8)
#define _OPTICAL_DDM_WARN_TX_BIAS_LO            ((1 << 2) << 8)
#define _OPTICAL_DDM_WARN_TX_PWR_HI             ((1 << 1) << 8)
#define _OPTICAL_DDM_WARN_TX_PWR_LO             ((1 << 0) << 8)
#define _OPTICAL_DDM_WARN_RX_PWR_HI             (1 << 7)
#define _OPTICAL_DDM_WARN_RX_PWR_LO             (1 << 6)
#endif

#if defined(CONFIG_SYS_OPTICALMOD_STATUS)
static int32 _sal_optical_internalCalibration_get(sys_logic_port_t port, sys_optical_diagMeasure_t *pMeasure)
{
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMeasure), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_ENHANCED_OPTION, &data));

    if (data.word[0] & _OPTICAL_OPTIONAL_STATUS_DATA_READY)
        return SYS_ERR_FAILED;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_TEMP, &data));

    pMeasure->temp = (int16)data.word[0];

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_VOLT, &data));

    pMeasure->volt = data.word[0];

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_TX_BIAS, &data));

    pMeasure->bias = data.word[0];

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_TX_PWR, &data));

    pMeasure->txPwr = data.word[0];

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_RX_PWR, &data));

    pMeasure->rxPwr = data.word[0];

    return SYS_ERR_OK;
}

static int32 _sal_optical_externalCalibration_get(sys_logic_port_t port, sys_optical_diagMeasure_t *pMeasure)
{
    sys_optical_data_t data;

    int32  result   = 0;
    int32  offset   = 0;
    uint32 slope    = 0;
    uint32 internal = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMeasure), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_ENHANCED_OPTION, &data));

    if (data.word[0] & _OPTICAL_OPTIONAL_STATUS_DATA_READY)
        return SYS_ERR_FAILED;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_TEMP, &data));

    internal = data.word[0];

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_TEMP_CAL, &data));

    slope = data.word[0] >> 16;
    offset = (int16)(data.word[0] & 0xFFFF);

    result = offset;
    result += (slope >> 8) * (int16)internal;
    result += ((slope & 0xFF) * (int16)internal) >> 8;

    pMeasure->temp = result;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_VOLT, &data));

    internal = data.word[0];

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_VOLT_CAL, &data));

    slope = data.word[0] >> 16;
    offset = data.word[0] & 0xFFFF;

    result = offset;
    result += (slope >> 8) * internal;
    result += ((slope & 0xFF) * internal) >> 8;

    pMeasure->volt = result;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_TX_BIAS, &data));

    internal = data.word[0];

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_TX_BIAS_CAL, &data));

    slope = data.word[0] >> 16;
    offset = data.word[0] & 0xFFFF;

    result = offset;
    result += (slope >> 8) * (int16)internal;
    result += ((slope & 0xFF) * internal) >> 8;

    pMeasure->bias = result;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_TX_PWR, &data));

    internal = data.word[0];

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_TX_PWR_CAL, &data));

    slope = data.word[0] >> 16;
    offset = data.word[0] & 0xFFFF;

    result = offset;
    result += (slope >> 8) * (int16)internal;
    result += ((slope & 0xFF) * internal) >> 8;

    pMeasure->txPwr = result;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_RX_PWR, &data));

    internal = data.word[0];

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_RX_PWR_CAL, &data));

    result = data.word[0];
    result += ((data.word[1] * internal) & 0xFFFF);
    result += ((data.word[2] * (internal * internal)) & 0xFFFF);
    result += ((data.word[3] * (internal * internal * internal)) & 0xFFFF);
    result += ((data.word[4] * (internal * internal * internal * internal)) & 0xFFFF);

    pMeasure->rxPwr = result;

    return SYS_ERR_OK;
}

int32 sal_optical_trxType_get(sys_logic_port_t port, sys_optical_trxType_t *pType)
{
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pType), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_TRANSCEIVER_TYPE, &data));

    switch (data.word[0])
    {
        case 0x01:
            *pType = SYS_OPTICAL_TRX_GBIC;
            break;

        case 0x02:
            *pType = SYS_OPTICAL_TRX_SFF;
            break;

        case 0x3:
            *pType = SYS_OPTICAL_TRX_SFP;
            break;

        default:
            *pType = SYS_OPTICAL_TRX_UNKNOWN;
            break;
    }

    return SYS_ERR_OK;
}

int32 sal_optical_connectType_get(sys_logic_port_t port, sys_optical_connectType_t *pType)
{
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pType), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_CONNECTOR_TYPE, &data));

    switch (data.word[0])
    {
        case 0x00:
            *pType = SYS_OPTICAL_CONNECT_UNKNOWN;
            break;

        case 0x01:
            *pType = SYS_OPTICAL_CONNECT_SC;
            break;

        case 0x02:
            *pType = SYS_OPTICAL_CONNECT_FC1_COPPER;
            break;

        case 0x03:
            *pType = SYS_OPTICAL_CONNECT_FC2_COPPER;
            break;

        case 0x04:
            *pType = SYS_OPTICAL_CONNECT_BNC_TNC;
            break;

        case 0x05:
            *pType = SYS_OPTICAL_CONNECT_FC_COAXIAL;
            break;

        case 0x06:
            *pType = SYS_OPTICAL_CONNECT_FIBER_JACK;
            break;

        case 0x07:
            *pType = SYS_OPTICAL_CONNECT_LC;
            break;

        case 0x08:
            *pType = SYS_OPTICAL_CONNECT_MTRJ;
            break;

        case 0x09:
            *pType = SYS_OPTICAL_CONNECT_MU;
            break;

        case 0x0A:
            *pType = SYS_OPTICAL_CONNECT_SG;
            break;

        case 0x0B:
            *pType = SYS_OPTICAL_CONNECT_OPTICAL_PIGTAIL;
            break;

        case 0x0C:
            *pType = SYS_OPTICAL_CONNECT_MPO_PARALLEL_OPTICS;
            break;

        case 0x20:
            *pType = SYS_OPTICAL_CONNECT_HSSDCII;
            break;

        case 0x21:
            *pType = SYS_OPTICAL_CONNECT_COPPER_PIGTAIL;
            break;

        case 0x22:
            *pType = SYS_OPTICAL_CONNECT_RJ45;
            break;

        default:
            *pType = SYS_OPTICAL_CONNECT_UNKNOWN;
            break;
    }

    return SYS_ERR_OK;
}

int32 sal_optical_trxCapacity_get(sys_logic_port_t port, sys_optical_trxCapacity_t *pCapacity)
{
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCapacity), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_COMPLIANCE_CODE, &data));

    if (data.word[0] & (1 << 28))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_10G_BASE_SR;
    else if (data.word[0] & (1 << 29))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_10G_BASE_LR;
    else if (data.word[0] & (1 << 30))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_10G_BASE_LRM;
    else if (data.word[0] & (1 << 31))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_10G_BASE_ER;
    else if (data.word[0] & (1 << 0))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_1000BASE_SX;
    else if (data.word[0] & (1 << 1))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_1000BASE_LX;
    else if (data.word[0] & (1 << 2))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_1000BASE_CX;
    else if (data.word[0] & (1 << 3))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_1000BASE_T;
    else if (data.word[0] & (1 << 4))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_100BASE_LX;
    else if (data.word[0] & (1 << 5))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_100BASE_FX;
    else if (data.word[0] & (1 << 6))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_BASE_BX10;
    else if (data.word[0] & (1 << 7))
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_BASE_PX;
    else
        pCapacity->ethCompliance = SYS_OPTICAL_ETH_COMPLIANCE_UNKNOWN;

    if (data.word[1] & (1 << 8))
        pCapacity->txMedia = SYS_OPTICAL_TX_MEDIA_SM;
    else if (data.word[1] & (1 << 10))
        pCapacity->txMedia = SYS_OPTICAL_TX_MEDIA_M5;
    else if (data.word[1] & (1 << 11))
        pCapacity->txMedia = SYS_OPTICAL_TX_MEDIA_M6;
    else if (data.word[1] & (1 << 12))
        pCapacity->txMedia = SYS_OPTICAL_TX_MEDIA_TV;
    else if (data.word[1] & (1 << 13))
        pCapacity->txMedia = SYS_OPTICAL_TX_MEDIA_MI;
    else if (data.word[1] & (1 << 14))
        pCapacity->txMedia = SYS_OPTICAL_TX_MEDIA_TP;
    else if (data.word[1] & (1 << 15))
        pCapacity->txMedia = SYS_OPTICAL_TX_MEDIA_TW;
    else
        pCapacity->txMedia = SYS_OPTICAL_TX_MEDIA_UNKNOWN;

    return SYS_ERR_OK;
}

int32 sal_optical_vendorInfo_get(sys_logic_port_t port, sys_optical_vendorInfo_t *pVendor)
{
    uint32 i = 0;
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVendor), SYS_ERR_NULL_POINTER);

    osal_memset(pVendor, 0, sizeof(sys_optical_vendorInfo_t));

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_VENDOR_OUI, &data));

    pVendor->oui = data.word[0];

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_VENDOR_NAME, &data));

    for (i = sizeof(data.byte); i >= 0; i--)
    {
        if (0x20 < data.byte[i - 1])
            break;
    }

    if (i)
        osal_memcpy(pVendor->name, data.byte, (i >= sizeof(pVendor->name)) ? sizeof(pVendor->name) - 1 : i);

    strncpy(pVendor->name, (char *)data.byte, sizeof(pVendor->name));

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_VENDOR_PN, &data));

    for (i = sizeof(data.byte); i >= 0; i--)
    {
        if (0x20 < data.byte[i - 1])
            break;
    }

    if (i)
        osal_memcpy(pVendor->pn, data.byte, (i >= sizeof(pVendor->pn)) ? sizeof(pVendor->pn) - 1 : i);

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_VENDOR_REV, &data));

    strncpy(pVendor->rev, (char *)data.byte, sizeof(pVendor->rev));

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_VENDOR_SN, &data));

    strncpy(pVendor->sn, (char *)data.byte, sizeof(pVendor->sn));

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DATE_CODE, &data));

    snprintf(pVendor->date, sizeof(pVendor->date), "%u-%02u-%02u", data.word[0], data.word[1], data.word[2]);

    return SYS_ERR_OK;
}

int32 sal_optical_wavelength_get(sys_logic_port_t port, uint32 *pWavelength)
{
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pWavelength), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    *pWavelength = 0;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_COMPLIANCE_CODE, &data));

    if (!(((data.word[1] >> 16) & 0xFF) & 0x0C))
    {
        SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_WAVELENGTH, &data));

        *pWavelength = data.word[0] & 0xFFFF;
    }

    return SYS_ERR_OK;
}

int32 sal_optical_diagStatus_get(sys_logic_port_t port, sys_optical_diagStatus_t *pStatus)
{
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    pStatus->temp = SYS_OPTICAL_DIAG_STATUS_UNKNOWN;
    pStatus->volt = SYS_OPTICAL_DIAG_STATUS_UNKNOWN;
    pStatus->bias = SYS_OPTICAL_DIAG_STATUS_UNKNOWN;
    pStatus->txPwr = SYS_OPTICAL_DIAG_STATUS_UNKNOWN;
    pStatus->rxPwr = SYS_OPTICAL_DIAG_STATUS_UNKNOWN;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DM_TYPE, &data));

    if (!(data.word[0] & _OPTICAL_DIAG_MON_TYPE_DITIGAL))
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_ENHANCED_OPTION, &data));

    if (!(data.word[0] & _OPTICAL_ENHANCED_SEVERITY))
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_WARNING_FLAG, &data));

    pStatus->temp = (data.word[0] & (_OPTICAL_DDM_WARN_TEMP_HI | _OPTICAL_DDM_WARN_TEMP_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_WARNING : SYS_OPTICAL_DIAG_STATUS_NORMAL;
    pStatus->volt = (data.word[0] & (_OPTICAL_DDM_WARN_VOLT_HI | _OPTICAL_DDM_WARN_VOLT_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_WARNING : SYS_OPTICAL_DIAG_STATUS_NORMAL;
    pStatus->bias = (data.word[0] & (_OPTICAL_DDM_WARN_TX_BIAS_HI | _OPTICAL_DDM_WARN_TX_BIAS_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_WARNING : SYS_OPTICAL_DIAG_STATUS_NORMAL;
    pStatus->txPwr = (data.word[0] & (_OPTICAL_DDM_WARN_TX_PWR_HI | _OPTICAL_DDM_WARN_TX_PWR_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_WARNING : SYS_OPTICAL_DIAG_STATUS_NORMAL;
    pStatus->rxPwr = (data.word[0] & (_OPTICAL_DDM_WARN_RX_PWR_HI | _OPTICAL_DDM_WARN_RX_PWR_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_WARNING : SYS_OPTICAL_DIAG_STATUS_NORMAL;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DDM_ALARM_FLAG, &data));

    pStatus->temp = (data.word[0] & (_OPTICAL_DDM_ALARM_TEMP_HI | _OPTICAL_DDM_ALARM_TEMP_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_ALARM : pStatus->temp;
    pStatus->volt = (data.word[0] & (_OPTICAL_DDM_ALARM_VOLT_HI | _OPTICAL_DDM_ALARM_VOLT_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_ALARM : pStatus->volt;
    pStatus->bias = (data.word[0] & (_OPTICAL_DDM_ALARM_TX_BIAS_HI | _OPTICAL_DDM_ALARM_TX_BIAS_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_ALARM : pStatus->bias;
    pStatus->txPwr = (data.word[0] & (_OPTICAL_DDM_ALARM_TX_PWR_HI | _OPTICAL_DDM_ALARM_TX_PWR_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_ALARM : pStatus->txPwr;
    pStatus->rxPwr = (data.word[0] & (_OPTICAL_DDM_ALARM_RX_PWR_HI | _OPTICAL_DDM_ALARM_RX_PWR_LO)) ?
                        SYS_OPTICAL_DIAG_STATUS_ALARM : pStatus->rxPwr;

    return SYS_ERR_OK;
}

int32 sal_optical_diagMeasure_get(sys_logic_port_t port, sys_optical_diagMeasure_t *pMeasure)
{
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMeasure), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DM_TYPE, &data));

    if (!(data.word[0] & _OPTICAL_DIAG_MON_TYPE_DITIGAL))
    {
        pMeasure->enable = DISABLED;
        return SYS_ERR_OK;
    }

    pMeasure->enable = ENABLED;

    if (data.word[0] & _OPTICAL_DIAG_MON_INTERNAL_CALIBRATION)
        SYS_ERR_CHK(_sal_optical_internalCalibration_get(port, pMeasure));
    else if (data.word[0] & _OPTICAL_DIAG_MON_EXTERNAL_CALIBRATION)
        SYS_ERR_CHK(_sal_optical_externalCalibration_get(port, pMeasure));
    else
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}

int32 sal_optical_diagOptionalStatus_get(sys_logic_port_t port, sys_optical_diagOptionalStatus_t *pStatus)
{
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    pStatus->txFault = SYS_OPTICAL_DIAG_NOT_SUPPORTED;
    pStatus->rxLos = SYS_OPTICAL_DIAG_NOT_SUPPORTED;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_DM_TYPE, &data));

    if (!(data.word[0] & _OPTICAL_DIAG_MON_TYPE_DITIGAL))
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_optical_data_get(port, OPTICAL_CMD_GET_ENHANCED_OPTION, &data));

    pStatus->txFault = (data.word[0] & _OPTICAL_ENHANCED_TX_FAULT) ? TRUE : FALSE;
    pStatus->rxLos = (data.word[0] & _OPTICAL_ENHANCED_RX_LOS) ? TRUE : FALSE;

    return SYS_ERR_OK;
}
#endif

int32 sal_optical_bitRateNominal_get(sys_logic_port_t port, uint32 *pSpeed)
{
    int32 ret = SYS_ERR_OK;
    sys_optical_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pSpeed), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    ret = rsd_optical_data_get(port, OPTICAL_CMD_GET_BR_NOMINAL, &data);

    *pSpeed = data.word[0] * 100;

    return ret;
}

int32 sal_optical_autoSensingConfig_set(sys_logic_port_t logicPort)
{
    uint32 bitrate = SYS_PORT_SPEED_ALL;

    sys_port_media_t media;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_enable_t fiberAutoNegoEbl;
	sys_fiber_detect_status_t detect_oe_status, detect_los_status;

    SYS_ERR_CHK(sal_port_media_get(logicPort, &media));

    if (PORT_MEDIA_COPPER == media)
        return SYS_ERR_OK;

    if (IS_LP_TRKMBR(logicPort))
        return SYS_ERR_OK;

	sal_fiber_oeStatus_get(logicPort, &detect_oe_status);
	sal_fiber_losStatus_get(logicPort, &detect_los_status);

	if((detect_oe_status ==  FIBER_DETECT_STATUS_OFF) && (detect_los_status  ==  FIBER_DETECT_STATUS_OFF))
	{
		return SYS_ERR_OK;
	}
    SYS_ERR_CHK(sal_optical_bitRateNominal_get(logicPort, &bitrate));

    if(IS_TRKLP(logicPort))
        fiberAutoNegoEbl = ENABLED;
    else
        SYS_ERR_CHK(cfg_port_fiberAutoNegoEnable_get(logicPort, &fiberAutoNegoEbl));       

    if (bitrate > 1000)
    {
        if (DISABLED == fiberAutoNegoEbl)
        {
            
            duplex.autoDuplex = DISABLED;
            duplex.ability = SYS_PORT_DUPLEX_FULL;

            speed.autoSpeed = DISABLED;
            speed.ability = SYS_PORT_SPEED_1000M;
        }
        else
        {
            
            duplex.autoDuplex = ENABLED;
            duplex.ability = SYS_PORT_DUPLEX_FULL;

            speed.autoSpeed = ENABLED;
            speed.ability = SYS_PORT_SPEED_ALL;
        }
    }
    else if (bitrate > 100)
    {
        
        duplex.autoDuplex = DISABLED;
        duplex.ability = SYS_PORT_DUPLEX_FULL;

        speed.autoSpeed = DISABLED;
        speed.ability = SYS_PORT_SPEED_100M;

    }
    else
    {
        
        duplex.autoDuplex = ENABLED;
        duplex.ability = SYS_PORT_DUPLEX_FULL;

        speed.autoSpeed = ENABLED;
        speed.ability = SYS_PORT_SPEED_ALL;
    }

    SYS_ERR_CHK(sal_port_speedDuplex_set(logicPort, speed, duplex, PORT_MEDIA_FIBER, FALSE));

    return SYS_ERR_OK;
}

