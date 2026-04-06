
#ifndef __RTL8238B_H_
#define __RTL8238B_H_

#define TPS_SUCCESSFUL                         0x00
#define TPS_ERR_PORT_IN_USE                    0x80
#define TPS_ERR_PORT_NOT_IN_USE                0x81
#define TPS_ERR_NO_PORT_AVAILABLE              0x82
#define TPS_ERR_I2C_ERROR                      0x83
#define TPS_ERR_I2C_ADDRESS_MISMATCH           0x40
#define TPS_ERR_I2C_AUTOBIT_MISMATCH           0x41

#define TPS_GIVE_ME_NEXT_PORT_AVAILABLE        0xff
#define TPS_PORT_NOT_REGISTERED_VALUE          0xff

#define SWIZZLE_BYTES(x)                       {unsigned short y; y = x; x = (((y & 0xff) << 8) | (y >> 8)); }     

#define TPS_MAX_SYSTEM_PORTS                   32
#define RTL8238B_VOLTAGE_SCALE_FACTOR          512	

typedef enum {
  RTL_OFF                                    = 0x0,   
  RTL_ON                                     = 0x1
} RTL8238B_On_Off_t;

#define CONVERT_TEMP_RTL8238B(x)                        (((x*652)-20000)/1000)  

#define RTL8238B_INTERRUPT_COMMAND                            0x00
#define RTL8238B_INTERRUPT_MASK_COMMAND                       0x01
#define RTL8238B_POWER_EVENT_COMMAND                          0x02
#define RTL8238B_POWER_EVENT_CLEAR_COMMAND                    0x03
#define RTL8238B_DETECTION_EVENT_COMMAND                      0x04
#define RTL8238B_DETECTION_EVENT_CLEAR_COMMAND                0x05
#define RTL8238B_FAULT_EVENT_COMMAND                          0x06
#define RTL8238B_FAULT_EVENT_CLEAR_COMMAND                    0x07
#define RTL8238B_INRUSH_LIMIT_EVENT_COMMAND                   0x08
#define RTL8238B_INRUSH_LIMIT_EVENT_CLEAR_COMMAND             0x09
#define RTL8238B_SUPPLY_EVENT_COMMAND                         0x0A
#define RTL8238B_SUPPLY_EVENT_CLEAR_COMMAND                   0x0B
#define RTL8238B_PORT_1_STATUS_COMMAND                        0x0C
#define RTL8238B_PORT_2_STATUS_COMMAND                        0x0D
#define RTL8238B_PORT_3_STATUS_COMMAND                        0x0E
#define RTL8238B_PORT_4_STATUS_COMMAND                        0x0F
#define RTL8238B_POWER_STATUS_COMMAND                         0x10
#define RTL8238B_I2C_SLAVE_ADDRESS_COMMAND                    0x11
#define RTL8238B_OPERATING_MODE_COMMAND                       0x12
#define RTL8238B_DISCONNECT_ENABLE_COMMAND                    0x13
#define RTL8238B_DETECT_CLASS_ENABLE_COMMAND                  0x14
#define RTL8238B_PORT_POWER_PRIORITY_COMMAND                  0x15
#define RTL8238B_TIMING_CONFIGURATION_COMMAND                 0x16
#define RTL8238B_GENERAL_MASK_1_COMMAND                       0x17
#define RTL8238B_DETECT_CLASS_RESTART_COMMAND                 0x18
#define RTL8238B_POWER_ENABLE_COMMAND                         0x19
#define RTL8238B_RESET_COMMAND                                0x1A
#define RTL8238B_ID_COMMAND                                   0x1B

#define RTL8238B_TEST_ENABLE_COMMAND                          0x1D

#define RTL8238B_LEGACY_DETECT_MODE_COMMAND                   0x20
#define RTL8238B_TWO_EVENT_CLASSIFICATION_COMMAND             0x21
#define RTL8238B_FOUR_PAIR_MODE_COMMAND                       0x22

#define RTL8238B_CLASS_FIVE_TIMER_ENABLE_COMMAND              0x27

#define RTL8238B_DISCONNECT_THRESHOLD_COMMAND                 0x29
#define RTL8238B_ICUT21_CONFIGURATION_COMMAND                 0x2A
#define RTL8238B_ICUT43_CONFIGURATION_COMMAND                 0x2B
#define RTL8238B_TEMPERATURE_COMMAND                          0x2C

#define RTL8238B_INPUT_VOLTAGE_COMMAND                        0x2E
#define RTL8238B_INPUT_VOLTAGE_LSB_COMMAND                    0x2E
#define RTL8238B_INPUT_VOLTAGE_MSB_COMMAND                    0x2F

#define RTL8238B_PORT_1_CURRENT_COMMAND                       0x30
#define RTL8238B_PORT_1_CURRENT_LSB_COMMAND                   0x30
#define RTL8238B_PORT_1_CURRENT_MSB_COMMAND                   0x31

#define RTL8238B_PORT_1_VOLTAGE_COMMAND                       0x32
#define RTL8238B_PORT_1_VOLTAGE_LSB_COMMAND                   0x32
#define RTL8238B_PORT_1_VOLTAGE_MSB_COMMAND                   0x33

#define RTL8238B_PORT_2_CURRENT_COMMAND                       0x34
#define RTL8238B_PORT_2_CURRENT_LSB_COMMAND                   0x34
#define RTL8238B_PORT_2_CURRENT_MSB_COMMAND                   0x35

#define RTL8238B_PORT_2_VOLTAGE_COMMAND                       0x36
#define RTL8238B_PORT_2_VOLTAGE_LSB_COMMAND                   0x36
#define RTL8238B_PORT_2_VOLTAGE_MSB_COMMAND                   0x37

#define RTL8238B_PORT_3_CURRENT_COMMAND                       0x38
#define RTL8238B_PORT_3_CURRENT_LSB_COMMAND                   0x38
#define RTL8238B_PORT_3_CURRENT_MSB_COMMAND                   0x39

#define RTL8238B_PORT_3_VOLTAGE_COMMAND                       0x3A
#define RTL8238B_PORT_3_VOLTAGE_LSB_COMMAND                   0x3A
#define RTL8238B_PORT_3_VOLTAGE_MSB_COMMAND                   0x3B

#define RTL8238B_PORT_4_CURRENT_COMMAND                       0x3C
#define RTL8238B_PORT_4_CURRENT_LSB_COMMAND                   0x3C
#define RTL8238B_PORT_4_CURRENT_MSB_COMMAND                   0x3D

#define RTL8238B_PORT_4_VOLTAGE_COMMAND                       0x3E
#define RTL8238B_PORT_4_VOLTAGE_LSB_COMMAND                   0x3E
#define RTL8238B_PORT_4_VOLTAGE_MSB_COMMAND                   0x3F

#define RTL8238B_POE_PLUS_COMMAND                             0x40
#define RTL8238B_FIRMWARE_REVISION_COMMAND                    0x41
#define RTL8238B_I2C_WATCHDOG_COMMAND                         0x42
#define RTL8238B_DEVICE_ID_COMMAND                            0x43

#define RTL8238B_COOL_DOWN_GATE_DRIVE_COMMAND                 0x45

#define RTL8238B_PORT_1_DETECT_RESISTANCE_COMMAND             0x60
#define RTL8238B_PORT_1_DETECT_RESISTANCE_LSB_COMMAND         0x60
#define RTL8238B_PORT_1_DETECT_RESISTANCE_MSB_COMMAND         0x61

#define RTL8238B_PORT_2_DETECT_RESISTANCE_COMMAND             0x62
#define RTL8238B_PORT_2_DETECT_RESISTANCE_LSB_COMMAND         0x62
#define RTL8238B_PORT_2_DETECT_RESISTANCE_MSB_COMMAND         0x63

#define RTL8238B_PORT_3_DETECT_RESISTANCE_COMMAND             0x64
#define RTL8238B_PORT_3_DETECT_RESISTANCE_LSB_COMMAND         0x64
#define RTL8238B_PORT_3_DETECT_RESISTANCE_MSB_COMMAND         0x65

#define RTL8238B_PORT_4_DETECT_RESISTANCE_COMMAND             0x66
#define RTL8238B_PORT_4_DETECT_RESISTANCE_LSB_COMMAND         0x66
#define RTL8238B_PORT_4_DETECT_RESISTANCE_MSB_COMMAND         0x67

#define RTL8238B_PORT_1_DETECT_VOLTAGE_DIFF_COMMAND           0x68
#define RTL8238B_PORT_1_DETECT_VOLTAGE_DIFF_LSB_COMMAND       0x68
#define RTL8238B_PORT_1_DETECT_VOLTAGE_DIFF_MSB_COMMAND       0x69

#define RTL8238B_PORT_2_DETECT_VOLTAGE_DIFF_COMMAND           0x6A
#define RTL8238B_PORT_2_DETECT_VOLTAGE_DIFF_LSB_COMMAND       0x6A
#define RTL8238B_PORT_2_DETECT_VOLTAGE_DIFF_MSB_COMMAND       0x6B

#define RTL8238B_PORT_3_DETECT_VOLTAGE_DIFF_COMMAND           0x6C
#define RTL8238B_PORT_3_DETECT_VOLTAGE_DIFF_LSB_COMMAND       0x6C
#define RTL8238B_PORT_3_DETECT_VOLTAGE_DIFF_MSB_COMMAND       0x6D

#define RTL8238B_PORT_4_DETECT_VOLTAGE_DIFF_COMMAND           0x6E
#define RTL8238B_PORT_4_DETECT_VOLTAGE_DIFF_LSB_COMMAND       0x6E
#define RTL8238B_PORT_4_DETECT_VOLTAGE_DIFF_MSB_COMMAND       0x6F

#define RTL8238B_4PWIRED_POWER_ALLOCATION_CONFIG_COMMAND      0x29
#define RTL8238B_4P_POLICE_12_CONFIG_COMMAND                  0x2A
#define RTL8238B_4P_POLICE_34_CONFIG_COMMAND                  0x2B
#define RTL8238B_TEMPERATURE_COMMAND                          0x2C
#define RTL8238B_4P_DISCONNECT_PCUT_ILIM_CONFIG_COMMAND       0x2D

#define RTL8238B_SRAM_CONTROL_COMMAND                         0x60
#define RTL8238B_SRAM_DATA_COMMAND                            0x61

#define RTL8238B_SRAM_START_ADDRESS_COMMAND                   0x62
#define RTL8238B_SRAM_START_ADDRESS_LSB_COMMAND               0x62
#define RTL8238B_SRAM_START_ADDRESS_MSB_COMMAND               0x63

uint32 poe_rtl_8238b_get(uint32 devNum);
uint32 poe_rtl_8238b_portEnable_set(uint32 devNum, uint32 devPort,uint32 Data);
uint32 board_poe_rtl8238b_init(uint32 index);
uint32 board_poe_rtl8238b_firmware_init(uint32 *listAddr, uint32 deviceNum, uint32 boardcastDev);

#endif 

