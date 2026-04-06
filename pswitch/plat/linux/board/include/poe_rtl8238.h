
#ifndef __RTL8238_H_
#define __RTL8238_H_

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
#define RTL8238_VOLTAGE_SCALE_FACTOR          250

#define CONVERT_TEMP_RTL8238(x)                        (((x*652)-20000)/1000)  

#define RTL8238_PORT_CTRL_COMMAND                             0x3C04   
#define RTL8238_PORT_STATE_COMMAND                            0x3C24   
#define RTL8238_DETECT_CLASS_ENABLE_COMMAND                   0x3200   
#define RTL8238_PORT_CURRENT_LSB_COMMAND					  0x2C00   
#define RTL8238_PORT_VOLTAGE_LSB_COMMAND                      0x2C20   

uint32 board_poe_rtl8238_init(uint32 index, uint32 deviceNum);
uint32 poe_rtl_8238_portEnable_set(uint32 devNum, uint32 devPort,uint32 Data);
uint32 poe_rtl_8238_get(uint32 devNum);

#endif 

