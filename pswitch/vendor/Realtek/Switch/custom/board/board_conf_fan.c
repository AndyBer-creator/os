
#include <board/board.h>
#ifdef CONFIG_SYS_BOARD_FAN

sys_gpio_t  fanCtrlGpio = 
{
	.unit			   = 0,
	.dev			   = 1,
	.pin			   = 5, 					   
	.pinConf = {
		.direction	   = GPIO_DIR_OUT,				 
		.default_value = GPIO_ACTIVE_LOW,
	},
	.active 		   = GPIO_ACTIVE_HIGH,
};

sys_gpio_t  alarmCtrlGpio = 
{
	.unit			   = 0,
	.dev			   = 1,
	.pin			   = 6, 					   
	.pinConf = {
		.direction	   = GPIO_DIR_IN,				 
		.default_value = GPIO_ACTIVE_HIGH,
	},
	.active 		   = GPIO_ACTIVE_LOW,
};

sys_fan_gpio_conf_t fanConfSys_dayou =
{
    .pState = &fanCtrlGpio,
    .pCheck = &alarmCtrlGpio,
    .alarmTime = 60,
    .changeSpeed = 120,
};

const sys_fan_conf_t fanConf =
{
    .type = FAN_TYPE_GPIO,
    .pConf = &fanConfSys_dayou,
};

sys_extDev_i2c_conf_t extDevTca9548= 
{
	.chipModel = SYS_EXT_DEV_CHIP_TCA9548,
	.pin = 0x00,
	.i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = 0x77,
            .clk_freq = I2C_CLK_STD_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 0,
        },
};

sys_temp_chip_conf_t tempDs75 =
{
	.chipModel = TEMP_CHIP_TYPE_DS75,
	.i2c = {
            .device_id = I2C_DEV_ID4,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = 0x48,
            .clk_freq = I2C_CLK_STD_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_2,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 0,
            .dev_9548 = {
					.dev_addr = I2C_DEV_ID3,
					.dev_reg = 0,
					.dev_data = 0x01,
            	},
        },
};

const sys_temperature_conf_t temperature =
{
	.pExtDevConf = &extDevTca9548,
	.pTempConf = &tempDs75,
};
#endif
