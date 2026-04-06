
#include <board/board.h>
#ifdef CONFIG_SYS_BOARD_RTC

const sys_rtc_conf_t rtcConf_8382_dayou =
{
    .i2c = {
            .device_id = I2C_DEV_ID15,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
    		.data_width = I2C_DATA_WIDTH_BYTE_1,
            .dev_addr = RTC_I2C_DEV_ADDR,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = 150,
            .scl_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .scl_pin_id = 2,
            .sda_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .sda_pin_id = 3,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
        },
    .devId = RTC_TYPE_LK8563,
};

#endif 

