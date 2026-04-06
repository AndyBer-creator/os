
#ifndef __SAL_OPTICAL_H__
#define __SAL_OPTICAL_H__

#include <libsal/sal_util.h>
#include <rsd/rsd_optical.h>

#if defined(CONFIG_SYS_OPTICALMOD_STATUS)
typedef enum sys_optical_trxType_e
{
    SYS_OPTICAL_TRX_UNKNOWN = 0,
    SYS_OPTICAL_TRX_GBIC,
    SYS_OPTICAL_TRX_SFF,
    SYS_OPTICAL_TRX_SFP,
    SYS_OPTICAL_TRX_END,
} sys_optical_trxType_t;

typedef enum sys_optical_connectType_e
{
    SYS_OPTICAL_CONNECT_UNKNOWN = 0,
    SYS_OPTICAL_CONNECT_SC,
    SYS_OPTICAL_CONNECT_FC1_COPPER,
    SYS_OPTICAL_CONNECT_FC2_COPPER,
    SYS_OPTICAL_CONNECT_BNC_TNC,
    SYS_OPTICAL_CONNECT_FC_COAXIAL,
    SYS_OPTICAL_CONNECT_FIBER_JACK,
    SYS_OPTICAL_CONNECT_LC,
    SYS_OPTICAL_CONNECT_MTRJ,
    SYS_OPTICAL_CONNECT_MU,
    SYS_OPTICAL_CONNECT_SG,
    SYS_OPTICAL_CONNECT_OPTICAL_PIGTAIL,
    SYS_OPTICAL_CONNECT_MPO_PARALLEL_OPTICS,
    SYS_OPTICAL_CONNECT_HSSDCII,
    SYS_OPTICAL_CONNECT_COPPER_PIGTAIL,
    SYS_OPTICAL_CONNECT_RJ45,
    SYS_OPTICAL_CONNECT_END,
} sys_optical_connectType_t;

typedef enum sys_optical_ethCompliance_e
{
    SYS_OPTICAL_ETH_COMPLIANCE_UNKNOWN = 0,
    SYS_OPTICAL_ETH_COMPLIANCE_10G_BASE_SR,
    SYS_OPTICAL_ETH_COMPLIANCE_10G_BASE_LR,
    SYS_OPTICAL_ETH_COMPLIANCE_10G_BASE_LRM,
    SYS_OPTICAL_ETH_COMPLIANCE_10G_BASE_ER,
    SYS_OPTICAL_ETH_COMPLIANCE_1000BASE_SX,
    SYS_OPTICAL_ETH_COMPLIANCE_1000BASE_LX,
    SYS_OPTICAL_ETH_COMPLIANCE_1000BASE_CX,
    SYS_OPTICAL_ETH_COMPLIANCE_1000BASE_T,
    SYS_OPTICAL_ETH_COMPLIANCE_100BASE_LX,
    SYS_OPTICAL_ETH_COMPLIANCE_100BASE_FX,
    SYS_OPTICAL_ETH_COMPLIANCE_BASE_BX10,
    SYS_OPTICAL_ETH_COMPLIANCE_BASE_PX,
    SYS_OPTICAL_ETH_COMPLIANCE_END,
} sys_optical_ethCompliance_t;

typedef enum sys_optical_txMedia_e
{
    SYS_OPTICAL_TX_MEDIA_UNKNOWN = 0,
    SYS_OPTICAL_TX_MEDIA_SM,
    SYS_OPTICAL_TX_MEDIA_M5,
    SYS_OPTICAL_TX_MEDIA_M6,
    SYS_OPTICAL_TX_MEDIA_TV,
    SYS_OPTICAL_TX_MEDIA_MI,
    SYS_OPTICAL_TX_MEDIA_TP,
    SYS_OPTICAL_TX_MEDIA_TW,
    SYS_OPTICAL_TX_MEDIA_END,
} sys_optical_txMedia_t;

typedef struct sys_optical_trxCapacity_s
{
    sys_optical_ethCompliance_t ethCompliance;
    sys_optical_txMedia_t       txMedia;
} sys_optical_trxCapacity_t;

typedef enum sys_optical_diagStatusFlag_e
{
    SYS_OPTICAL_DIAG_STATUS_UNKNOWN = 0,
    SYS_OPTICAL_DIAG_STATUS_NORMAL,
    SYS_OPTICAL_DIAG_STATUS_WARNING,
    SYS_OPTICAL_DIAG_STATUS_ALARM,
    SYS_OPTICAL_DIAG_STATUS_END,
} sys_optical_diagStatusFlag_t;

typedef struct sys_optical_diagStatus_s
{
    sys_optical_diagStatusFlag_t temp;
    sys_optical_diagStatusFlag_t volt;
    sys_optical_diagStatusFlag_t bias;
    sys_optical_diagStatusFlag_t txPwr;
    sys_optical_diagStatusFlag_t rxPwr;
} sys_optical_diagStatus_t;

typedef struct sys_optical_diagMeasure_s
{
    sys_enable_t enable;    
    int32        temp;      
    uint32       volt;      
    uint32       bias;      
    uint32       txPwr;     
    uint32       rxPwr;     
} sys_optical_diagMeasure_t;

typedef struct sys_optical_diagOptionalStatus_s
{
    int32 txFault;
    int32 rxLos;
} sys_optical_diagOptionalStatus_t;

typedef struct sys_optical_vendorInfo_s
{
    uint32 oui;     
    char name[32];  
    char pn[32];    
    char sn[32];    
    char rev[8];    
    char date[16];  
} sys_optical_vendorInfo_t;

extern const sys_text_t text_optical_trxType[];
extern const sys_text_t text_optical_connectType[];
extern const sys_text_t text_optical_ethCompliance[];
extern const sys_text_t text_optical_trxMedia[];

#endif

#if defined(CONFIG_SYS_OPTICALMOD_STATUS)

extern int32 sal_optical_trxType_get(sys_logic_port_t logicPort, sys_optical_trxType_t *pType);

extern int32 sal_optical_connectType_get(sys_logic_port_t port, sys_optical_connectType_t *pType);

extern int32 sal_optical_trxCapacity_get(sys_logic_port_t port, sys_optical_trxCapacity_t *pCapacity);

extern int32 sal_optical_vendorInfo_get(sys_logic_port_t port, sys_optical_vendorInfo_t *pVendor);

extern int32 sal_optical_wavelength_get(sys_logic_port_t port, uint32 *pWavelength);

extern int32 sal_optical_diagStatus_get(sys_logic_port_t port, sys_optical_diagStatus_t *pStatus);

extern int32 sal_optical_diagMeasure_get(sys_logic_port_t port, sys_optical_diagMeasure_t *pMeasure);

extern int32 sal_optical_diagOptionalStatus_get(sys_logic_port_t port, sys_optical_diagOptionalStatus_t *pStatus);
#endif

extern int32 sal_optical_bitRateNominal_get(sys_logic_port_t port, uint32 *pSpeed);

extern int32 sal_optical_autoSensingConfig_set(sys_logic_port_t port);

#endif 

