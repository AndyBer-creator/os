
#ifndef __POE_H__
#define __POE_H__

#include <board/include/intf.h>
#include <board/include/gpio.h>
#include <board/include/i2c.h>

 #define POE_SCHEDULE_WEEK 7
 #define POE_SCHEDULE_HOUR 24

#define POE_DEV_MAX     12
#define POE_DEV_PORT    8

#ifndef POE_DEV_NUM
#define POE_DEV_NUM     15 
#endif
#ifndef PORT_START_NUM
#define PORT_START_NUM   0
#endif
#ifndef POE_MAXPORT_NUM 
#define POE_MAXPORT_NUM 50
#endif

#define PORT_PER_DEV     4

#define PSE_I2C_ADDR_UNIT0        0x20
#define PSE_I2C_ADDR_UNIT1        0x21
#define PSE_I2C_ADDR_UNIT2        0x22
#define PSE_I2C_ADDR_UNIT3        0x23
#define PSE_I2C_ADDR_UNIT4        0x24
#define PSE_I2C_ADDR_UNIT5        0x25
#define PSE_I2C_ADDR_UNIT6        0x26
#define PSE_I2C_ADDR_UNIT7        0x27
#define PSE_I2C_ADDR_UNIT8        0x70
#define PSE_I2C_ADDR_UNIT9        0x71
#define PSE_I2C_ADDR_UNIT10       0x72
#define PSE_I2C_ADDR_UNIT11       0x73

#define PSE_I2C_ADDR_UNIT12       0x28
#define PSE_I2C_ADDR_UNIT13       0x29
#define PSE_I2C_ADDR_UNIT14       0x2A
#define PSE_I2C_ADDR_UNIT15       0x2B
#define PSE_I2C_ADDR_UNIT16       0x2C
#define PSE_I2C_ADDR_UNIT17       0x2D

#define PSE_I2C_ADDR_END           0xFF

#define PSE_I2C_ADDR_GLOBAL        0x30

#define PSE_I2C_ADDR_TEMP          0x10   
#define PSE_I2C_ADDR_TEMP_A3H     0x18

#define PSE_I2C_AD_23880_0       0x20
#define PSE_I2C_AD_23880_1       0x21
#define PSE_I2C_AD_23880_2       0x22
#define PSE_I2C_AD_23880_3       0x23

#define PSE_I2C_AD_23882_0       0x20
#define PSE_I2C_AD_23882_1       0x21

#define PSE_I2C_SCL_PIN_ID_838X   4
#define PSE_I2C_SDA_PIN_ID_838X   5
#define PSE_I2C_SCL_PIN_839X_SS   9
#define PSE_I2C_SDA_PIN_839X_SS   8
#define PSE_I2C_SCL_PIN_ID_930X   3
#define PSE_I2C_SDA_PIN_ID_930X   4

#define PSE_I2C_SCL_PIN_ID_931X   29
#define PSE_I2C_SDA_PIN_ID_931X   30
#define PSE_I2C_SCL_P931X_2388X   27
#define PSE_I2C_SDA_P931X_2388X   28

#define PM_EN                   1
#define PM_POWER_BUDGET         42000       
#define PM_CLASS0_POWER         15400       
#define PM_CLASS1_POWER         4000        
#define PM_CLASS2_POWER         7000        
#define PM_CLASS3_POWER         15400       
#define PM_CLASS4_POWER         30000       

#define POE_WATCH_MACHINE_POWER    0x001
#define POE_WATCH_MACHINE_FLOW     0x100
#define POE_WATCH_MACHINE_UP       0x010
#define POE_WATCH_SCAN_TIME        0x03
#define POE_WATCH_REBOOT_TIMES     0x05
#define POE_WATCH_REBOOT_INTERVAL  (2*60)

#define POE_SCHEDULE_SCAN_TIME     60

typedef enum {
    Low           = 0x0,
    Norm          = 0x1,
    High          = 0x2
}PM_Priority_t;

typedef enum {
    PM_CHECK      = 0x0,
    PM_OVERLIMIT  = 0x1,
    PM_POWERUP    = 0x2,
    PM_POWERDOWN  = 0x3
}PM_STATE_t;

typedef enum PSE_DEV_TYPE_e{
    PSE_TYPE_TI23861 = 0x0,
    PSE_TYPE_TI23880 = 0x1,
    PSE_TYPE_RTL8238B_L = 0x2,
    PSE_TYPE_RTL8238B_H = 0x3,
    PSE_TYPE_RTL8238 = 0x4,
    PSE_TYPE_RTL8239_L = 0x5,
    PSE_TYPE_RTL8239_H = 0x6,
    PSE_TYPE_PD69204 = 0x7,
    PSE_TYPE_PD69208 = 0x8,
    PSE_TYPE_TI23881 = 0x9,
    PSE_TYPE_TI23882 = 0xA,
    PSE_TYPE_MP3924 = 0xB,
    PSE_TYPE_IP808 = 0xC,
    PSE_TYPE_END
}PSE_DEV_TYPE_t;

typedef enum PSE_DEV_MODE_e{
    PSE_MODE_AT_AF,
    PSE_MODE_BT,
    PSE_MODE_END
}PSE_DEV_MODE_t;

typedef enum sys_poe_msg_e
{
    POE_MSG_GET = 0,
    POE_MSG_GET_PORT_ENABLE,
    POE_MSG_GET_PORT_ABILITY,
    POE_MSG_GET_PORT_PWGOOD,
    POE_MSG_GET_PORT_STATUS,
    POE_MSG_GET_PORT_CURRENT,
    POE_MSG_GET_PORT_VOLTAGE,
    POE_MSG_GET_PORT_ICUT,
    POE_MSG_GET_DEV_POWER_STATUS,
    POE_MSG_GET_DEV_TEMPERATURE,
    POE_MSG_GET_DEV_INTR_STATUS,
    POE_MSG_GET_DEV_CLASS_STATUS,
    POE_MSG_GET_DEV_DISC_STATUS,
    POE_MSG_GET_DEV_DETECT_STATUS,
    POE_MSG_GET_PORT_WATCH_DOG_ENABLE,

    POE_MSG_SET_PORT_ENABLE,
    POE_MSG_SET_PORT_ICUT,
    POE_MSG_SET_PORT_SCHEDULE,
    POE_MSG_SET_TIMEZONE,
    POE_MSG_SET_PORT_POEP,
    POE_MSG_SET_PORT_MODE,
	POE_MSG_SET_PORT_WATCH_DOG_ENABLE,
	POE_MSG_SET_PORT_TIME_OPEN,
	POE_MSG_SET_PORT_TIME_CLOSE,
    POE_MSG_END
}sys_poe_msg_t;

typedef enum sys_pd_class_e{
	SYS_PD_CLASS_UNKNOWN,
    SYS_PD_CLASS1_3_84W,
    SYS_PD_CLASS2_6_49W,
    SYS_PD_CLASS3_13W,
    SYS_PD_CLASS4_25_5W,
    SYS_PD_CLASS5_40W,
    SYS_PD_CLASS6_51W,
    SYS_PD_CLASS7_62W,
    SYS_PD_CLASS8_71W,
    SYS_PD_CLASS0_13W,
    SYS_PD_CLASS_END
}sys_pd_class_t;

typedef struct poe_time_s
{
	uint8           day;
	uint8           hour;
	uint8           minute;
	uint8           second;	
}poe_time_t;
typedef struct poe_timed_reboot_s
{
	uint32          portstat;   
	uint32          rebootflag;  
	uint32          rebootTime; 
	uint32          cntTimes;   
	uint32          portSchedule[POE_SCHEDULE_WEEK][POE_SCHEDULE_HOUR];
	poe_time_t      closeTime;
	poe_time_t      openTime; 
}poe_timed_reboot_t;

#ifdef CONFIG_SYS_POE_SWDOG
typedef enum poe_watch_dog_machine_s
{
	POE_WATCH_DOG_000 = 0x000,
	POE_WATCH_DOG_001 = 0x001,
	POE_WATCH_DOG_011 = 0x011,
	POE_WATCH_DOG_111 = 0x111,
	POE_WATCH_DOG_END
}poe_watch_dog_machine_t;
typedef struct poe_watch_dog_s
{
	uint32          watchDogEnable; 
	uint32          stateMachine; 
	uint32          scanTimes; 
	uint32          rebootTimes; 
	uint32          rebootFlag;  
	uint64          ingStat; 
}poe_watch_dog_t;
#endif
typedef struct sys_poe_devConf_s
{
    PSE_DEV_TYPE_t  devType; 
    uint32          devNum; 
    uint32          startp; 
    uint32          portNum; 
    i2c_devConf_t   i2c;
} sys_poe_devConf_t;

typedef struct sys_poe_portConf_s
{
    uint32          port; 
    uint32          devNum; 
    uint32          devPort; 
    PSE_DEV_MODE_t  mode;   
	uint32          portTime;
	uint32          portSchedule[7];
#ifdef CONFIG_SYS_POE_SWDOG
    poe_watch_dog_t portPoeWatchDog;
#endif
	poe_timed_reboot_t poeTime;
} sys_poe_portConf_t;

typedef struct sys_poe_portData_s
{
	uint32  portEnable;
	uint32  portPwGood;
    uint32  portStatus;
    uint32  portClassSts;
    uint32  portDetClass;
	uint32  portCurrent;
	uint32  portVoltage;
	uint32  portWDogEn;
	uint32  portTemperature;
} sys_poe_portData_t;

typedef struct sys_poe_config_s
{
    uint32                     portNum;
    uint32                     startport;
    uint32                     poeDevNum;
    uint32                     poeLedEntity;
    const sys_poe_devConf_t   *pPoeDevConf;
    const sys_poe_portConf_t  *pPoePortConf;
} sys_poe_config_t;

typedef struct sys_poe_portDrv_s
{
    uint32   poe_portIndex;
    uint32   (*poe_portEnable_set)(uint32, uint32, uint32);
}sys_poe_portDrv_t;

extern int32 board_poe_init(void);

extern int32 board_poe_msg_get(uint32 logicPort, sys_poe_msg_t cmd, uint32 *pData);
extern int32 board_poe_msg_set(uint32 logicPort, sys_poe_msg_t cmd, uint32 Data);

#endif 

