
#ifndef __SAL_EASYCWMP_H__
#define __SAL_EASYCWMP_H__

#include <common/sys_def.h> 
#include <libipc/vos.h>

#define EASYCWMP_PID_FILE_PATH "/var/run/easycwmp.pid"
#define EASYCWMP_SERVER_PATH "easycwmp_server_ipc_"

typedef enum{
    EASYCWMP_DEVICE_MANUFACTURER = 1,
    EASYCWMP_DEVICE_OUI,
    EASYCWMP_DEVICE_PRODUCT_CLASS,
    EASYCWMP_DEVICE_SN,
    EASYCWMP_DEVICE_SOFTWAREVERSION,
    EASYCWMP_DEVICE_HARDWAREVERSION,
    EASYCWMP_DEVICE_MODENAME,
    EASYCWMP_DEVICE_END,
}EASYCWMP_DEVICE_T; 

typedef enum{
    EASYCWMP_ACS_URL = 1,
    EASYCWMP_ACS_USERNAME,
    EASYCWMP_ACS_PASSWORD,
    EASYCWMP_ACS_PERIODIC_ENABLE,
    EASYCWMP_ACS_PERIODIC_INTERVAL,
    EASYCWMP_ACS_PERIODIC_TIME,
    EASYCWMP_ACS_END,
}EASYCWMP_ACS_T;

typedef enum{
    EASYCWMP_LOCAL_ENABLE = 1,
    EASYCWMP_LOCAL_INTERFACE,
    EASYCWMP_LOCAL_PORT,
    EASYCWMP_LOCAL_USERNAME,
    EASYCWMP_LOCAL_PASSWORD,
    EASYCWMP_LOCAL_AUTH,
    EASYCWMP_LOCAL_LOG_LEVEL,
    EASYCWMP_LOCAL_END,
}EASYCWMP_LOCAL_T;

typedef enum {
    EASYCWMP_FUNC_START,
    EASYCWMP_FUNC_ENABLE,
    EASYCWMP_FUNC_MAX
}EASYCWMP_FUNC_ENT;

extern int32 sal_easycwmp_db_init(void);
extern int32 sal_easycwmp_swVersion_set(char* ver);
extern int32 sal_easycwmp_swVersion_get(char* pVersion);
extern int32 sal_easycwmp_sn_set(char* sn);
extern int32 sal_easycwmp_sn_get(char* sn);
extern int32 sal_easycwmp_cpe_enable_set(sys_enable_t enable);
extern int32 sal_easycwmp_modelName_set(char* model);
extern int32 sal_easycwmp_device_cfg_get(int index, char* strValue);
extern int32 sal_easycwmp_acs_cfg_get(int index, char* strValue);
extern int32 sal_easycwmp_acs_cfg_set(int index, char* strValue);
extern int32 sal_easycwmp_local_cfg_get(int index, char* strValue);
extern int32 sal_easycwmp_local_cfg_set(int index, char* strValue);
#endif 

