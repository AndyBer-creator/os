
#ifndef __FIBER_H__
#define __FIBER_H__

#include <board/include/intf.h>
#include <board/include/gpio.h>
#include <board/include/i2c.h>

typedef enum sys_optiacl_smi_e
{
    OPTICAL_SMI_GET = 0,
    OPTICAL_SMI_GET_BR_NOMINAL,
    OPTICAL_SMI_GET_VENDOR_NAME,
    OPTICAL_SMI_GET_VENDOR_PN,
    OPTICAL_SMI_GET_CABLE_LENGTH,
#if defined(CONFIG_SYS_OPTICALMOD_STATUS)
    OPTICAL_SMI_GET_TRANSCEIVER_TYPE,
    OPTICAL_SMI_GET_CONNECT_TYPE,
    OPTICAL_SMI_GET_COMPLIANCE_CODE,
    OPTICAL_SMI_GET_FIBER_TYPE,
    OPTICAL_SMI_GET_ETH_COMPLIANCE_CODE,
    OPTICAL_SMI_GET_TX_DISTANCE,
    OPTICAL_SMI_GET_WAVELENGTH,
    OPTICAL_SMI_GET_VENDOR_OUI,
    OPTICAL_SMI_GET_VENDOR_REV,
    OPTICAL_SMI_GET_VENDOR_SN,
    OPTICAL_SMI_GET_DATE_CODE_YEAR,
    OPTICAL_SMI_GET_DATE_CODE_MONTH,
    OPTICAL_SMI_GET_DATE_CODE_DAY,
    OPTICAL_SMI_GET_DDM_TEMP,
    OPTICAL_SMI_GET_DDM_TEMP_SLOPE,
    OPTICAL_SMI_GET_DDM_TYPE,
    OPTICAL_SMI_GET_DDM_VOLT,
    OPTICAL_SMI_GET_DDM_VOLT_SLOPE,
    OPTICAL_SMI_GET_DDM_TX_BIAS,
    OPTICAL_SMI_GET_DDM_TX_L_SLOPE,
    OPTICAL_SMI_GET_DDM_TX_PWR,
    OPTICAL_SMI_GET_DDM_TX_PWR_SLOPE,
    OPTICAL_SMI_GET_DDM_RX_PWR,
    OPTICAL_SMI_GET_DDM_RX_PWR4,
    OPTICAL_SMI_GET_DDM_RX_PWR3,
    OPTICAL_SMI_GET_DDM_RX_PWR2,
    OPTICAL_SMI_GET_DDM_RX_PWR1,
    OPTICAL_SMI_GET_DDM_RX_PWR0,
    OPTICAL_SMI_GET_DDM_ALARM_FLAG,
    OPTICAL_SMI_GET_DDM_WARN_FLAG,
    OPTICAL_SMI_GET_DDM_OPTINAL_STATUS,
    OPTICAL_SMI_GET_ENHANCED_OPTION,
#endif
    OPTICAL_SMI_END,
} sys_optiacl_smi_t;

typedef struct sys_fiber_conf_optical_s
{
    uint32          port; 
    i2c_devConf_t   i2c;
} sys_fiber_conf_optical_t;

typedef enum sys_fiber_detect_method_e
{
    FIBER_DETECT_METHOD_NONE = 0,
    FIBER_DETECT_METHOD_OE,
    FIBER_DETECT_METHOD_LOS,
    FIBER_DETECT_METHOD_END
} sys_fiber_detect_method_t;

typedef enum sys_fiber_detect_status_e
{
    FIBER_DETECT_STATUS_OFF = 0,
    FIBER_DETECT_STATUS_ON,
    FIBER_DETECT_STATUS_END
} sys_fiber_detect_status_t;

typedef struct sys_fiber_conf_detect_s
{
    uint32                     port; 
    sys_fiber_detect_method_t  present;
    sys_fiber_detect_method_t  mediaChange;
    uint32                     enableOe;
    uint32                     enableLos;
    sys_gpio_t                 *pGpio_oe;
    sys_gpio_t                 *pGpio_los;
} sys_fiber_conf_detect_t;

typedef struct sys_fiber_conf_s
{
    uint32                         portNum;
    const sys_fiber_conf_detect_t  *pDetect;
    const sys_fiber_conf_optical_t *pOptical;
} sys_fiber_conf_t;

extern int32 board_fiber_init(void);

extern int32 board_fiber_detect_init(sys_fiber_conf_t *pFiberConf);

extern int32 board_fiber_oeStatus_get(uint32 port, sys_fiber_detect_status_t *pStatus);

extern int32 board_fiber_losStatus_get(uint32 port, sys_fiber_detect_status_t *pStatus);

extern int32 board_fiber_detectStatus_get(uint32 port, sys_fiber_detect_status_t *pStatus);

extern int32 board_fiber_optical_init(sys_fiber_conf_t *pFiberConf);
extern int32 board_fiber_prefer_init(sys_fiber_conf_t *pFiberConf);

extern int32 board_fiber_opticalStatus_get(uint32 logicPort, sys_optiacl_smi_t cmd, uint32 *pData);

#endif 

