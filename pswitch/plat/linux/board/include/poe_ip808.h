#ifndef IP808_H_
#define IP808_H_

#define DEBUG_IP808AR
#define IP808AR_MUN   1

#define POE_NO_INIT             0x7
#define STATE_DISABLE           0
#define STATE_ENABLE            1
#define RESET_FLAG              0xA5

#define PAGE_N_DEVICE_ADDR  0x0

#define ICUT_PORT0              0x13
#define POWER_DOWN              0x27
#define P0_ICUT                 0x48
#define EVENT_LOW_COUNT         0x55

#define DETECT_CLASS0_1        0x88
#define DETECT_CLASS2_3        0x89
#define DETECT_CLASS4_5        0x8A
#define DETECT_CLASS6_7        0x8B

#define CURRENT_MSB0            0xA0
#define CURRENT_LSB0            0xA1

#define TEMPER_MSB0            0xC0
#define TEMPER_LSB0            0xC1

#define SUPPLY_VOLTAGE_MSB      0xE0
#define SUPPLY_VOLTAGE_LSB      0xE1

#define FORCE_IVT_POLL          0xE2
#define IVT_POLL_CONTROL        0xE3

#define SYS_CONFIG_CONTROL  0x1
#define RESET_FLAG_REG_IP808A   0x06
#define PORT_EVENT0             0x70
#define PORT_POWER_STATUS       0x82
#define STATE_MACHINE_STATE_P0  0x90
#define POWER_CONTROL_P0        0x98

#define IP808_REG_PAGE0         0x0
#define IP808_REG_PAGE1         0x1

#endif

