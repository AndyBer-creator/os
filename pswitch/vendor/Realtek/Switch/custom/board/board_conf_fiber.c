
#include <board/board.h>

#ifdef CONFIG_SYS_RTL8380
const sys_fiber_conf_detect_t detectConf_8380_8_2f[] =
{
    {
        .port = 8, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_1),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_0),
    },
    {
        .port = 9, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_6),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_5),
    }
};

const sys_fiber_conf_optical_t opticalConf_8380_8_2f[] =
{
    {
        .port = 8, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 2,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 3,
        },
    },
    {
        .port = 9, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 7,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 8,
        },
    },
};

const sys_fiber_conf_t fiberConf_8380_8_2f = {
    .portNum = 2,
    .pDetect = detectConf_8380_8_2f,
    .pOptical = opticalConf_8380_8_2f,
};

const sys_fiber_conf_detect_t detectConf_8380_4_2f[] =
{
    {
        .port = 4, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_1),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_0),
    },
    {
        .port = 5, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_6),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_5),
    }
};

const sys_fiber_conf_optical_t opticalConf_8380_4_2f[] =
{
    {
        .port = 4, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 2,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 3,
        },
    },
    {
        .port = 5, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 7,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 8,
        },
    },
};

const sys_fiber_conf_t fiberConf_8380_4_2f = {
    .portNum = 2,
    .pDetect = detectConf_8380_4_2f,
    .pOptical = opticalConf_8380_4_2f,
};

const sys_fiber_conf_detect_t detectConf_8382_24_4f[] =
{
    {
        .port = 24, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_8),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_9),
    },
    {
        .port = 25, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_3),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_4),
    },
    {
        .port = 26, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_24),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_25),
    },
    {
        .port = 27, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_13),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_14),
    }
};

const sys_fiber_conf_optical_t opticalConf_8382_24_4f[] =
{
    {
        .port = 24, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 7,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 6,
        },
    },
    {
        .port = 25, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 2,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 1,
        },
    },
    {
        .port = 26, 
        .i2c = {
            .device_id = I2C_DEV_ID2,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 23,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 22,
        },
    },
    {
        .port = 27, 
        .i2c = {
            .device_id = I2C_DEV_ID3,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 12,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 11,
        },
    }
};

const sys_fiber_conf_t fiberConf_8382_24_4f = {
    .portNum = 4,
    .pDetect = detectConf_8382_24_4f,
    .pOptical = opticalConf_8382_24_4f,
};

const sys_fiber_conf_detect_t detectConf_8382_16_2f[] =
{
    {
        .port = 16, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_24),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_25),
    },
    {
        .port = 17, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_13),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_14),
    }
};

const sys_fiber_conf_optical_t opticalConf_8382_16_2f[] =
{
    {
        .port = 16, 
        .i2c = {
            .device_id = I2C_DEV_ID2,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 23,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 22,
        },
    },
    {
        .port = 17, 
        .i2c = {
            .device_id = I2C_DEV_ID3,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 12,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 11,
        },
    }
};

const sys_fiber_conf_t fiberConf_8382_16_2f = {
    .portNum = 2,
    .pDetect = detectConf_8382_16_2f,
    .pOptical = opticalConf_8382_16_2f,
};

const sys_fiber_conf_detect_t detectConf_8382_10_4f[] =
{
    {
        .port = 10, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_8),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_9),
    },
    {
        .port = 11, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_3),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_4),
    },
    {
        .port = 12, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_24),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_25),
    },
    {
        .port = 13, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_GPIO(SYS_GPIO_EXTERNAL_13),
        .pGpio_los = BOARD_GPIO(SYS_GPIO_EXTERNAL_14),
    }
};

const sys_fiber_conf_optical_t opticalConf_8382_10_4f[] =
{
    {
        .port = 10, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 7,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 6,
        },
    },
    {
        .port = 11, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 2,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 1,
        },
    },
    {
        .port = 12, 
        .i2c = {
            .device_id = I2C_DEV_ID2,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 23,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 22,
        },
    },
    {
        .port = 13, 
        .i2c = {
            .device_id = I2C_DEV_ID3,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID1,
            .scl_pin_id = 12,
            .sda_dev = GEN_GPIO_DEV_ID1,
            .sda_pin_id = 11,
        },
    }
};

const sys_fiber_conf_t fiberConf_8382_10_4f = {
    .portNum = 4,
    .pDetect = detectConf_8382_10_4f,
    .pOptical = opticalConf_8382_10_4f,
};
#endif

#ifdef CONFIG_SYS_RTL8390
const sys_fiber_conf_detect_t detectConf_8393_48_2f[] =
{
    {
        .port = 50, 
        .present = FIBER_DETECT_METHOD_LOS,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableLos = ENABLED,
        .pGpio_los = BOARD_GPIO(SYS_GPIO_INTERNAL_21),
    },
    {
        .port = 51, 
        .present = FIBER_DETECT_METHOD_LOS,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableLos = ENABLED,
        .pGpio_los = BOARD_GPIO(SYS_GPIO_INTERNAL_22),
    }
};

const sys_fiber_conf_detect_t detectConf_8353_48_2f[] =
{
    {
        .port = 50, 
        .present = FIBER_DETECT_METHOD_LOS,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableLos = ENABLED,
        .pGpio_los = BOARD_GPIO(SYS_GPIO_INTERNAL_23),
    },
    {
        .port = 51, 
        .present = FIBER_DETECT_METHOD_LOS,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableLos = ENABLED,
        .pGpio_los = BOARD_GPIO(SYS_GPIO_INTERNAL_22),
    }
};

const sys_fiber_conf_detect_t detectConf_8391_24_4f[] =
{
    {
        .port = 24, 
        .present = FIBER_DETECT_METHOD_LOS,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = DISABLED,
        .enableLos = ENABLED,
        .pGpio_los = BOARD_GPIO(SYS_GPIO_INTERNAL_19),
    },
    {
        .port = 25, 
        .present = FIBER_DETECT_METHOD_LOS,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = DISABLED,
        .enableLos = ENABLED,
        .pGpio_los = BOARD_GPIO(SYS_GPIO_INTERNAL_20),
    },
    {
        .port = 26, 
        .present = FIBER_DETECT_METHOD_LOS,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = DISABLED,
        .enableLos = ENABLED,
        .pGpio_los = BOARD_GPIO(SYS_GPIO_INTERNAL_21),
    },
    {
        .port = 27, 
        .present = FIBER_DETECT_METHOD_LOS,
        .mediaChange = FIBER_DETECT_METHOD_LOS,
        .enableOe = DISABLED,
        .enableLos = ENABLED,
        .pGpio_los = BOARD_GPIO(SYS_GPIO_INTERNAL_22),
    }
};

const sys_fiber_conf_optical_t opticalConf_8393_48_2f[] =
{
    {
        .port = 50, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .scl_pin_id = 17,
            .sda_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .sda_pin_id = 18,
        },
    },
    {
        .port = 51, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .scl_pin_id = 17,
            .sda_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .sda_pin_id = 19,
        },
    }
};

const sys_fiber_conf_optical_t opticalConf_8353_48_2f[] =
{
    {
        .port = 50, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .scl_pin_id = 15,
            .sda_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .sda_pin_id = 17,
        },
    },
    {
        .port = 51, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .scl_pin_id = 16,
            .sda_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .sda_pin_id = 17,
        },
    }
};

const sys_fiber_conf_optical_t opticalConf_8391_24_4f[] =
{
    {
        .port = 24, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .scl_pin_id = 13,
            .sda_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .sda_pin_id = 15,
        },
    },
    {
        .port = 25, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .scl_pin_id = 13,
            .sda_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .sda_pin_id = 16,
        },
    },
    {
        .port = 26, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .scl_pin_id = 14,
            .sda_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .sda_pin_id = 17,
        },
    },
    {
        .port = 27, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_FAST_MODE,
            .scl_delay = I2C_DFL_DELAY,
            .scl_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .scl_pin_id = 14,
            .sda_dev = GEN_GPIO_DEV_ID0_INTERNAL,
            .sda_pin_id = 18,
        },
    }
};

const sys_fiber_conf_t fiberConf_8393_48_2f = {
    .portNum = 2,
    .pDetect = detectConf_8393_48_2f,
    .pOptical = opticalConf_8393_48_2f,
};

const sys_fiber_conf_t fiberConf_8353_48_2f = {
    .portNum = 2,
    .pDetect = detectConf_8353_48_2f,
    .pOptical = opticalConf_8353_48_2f,
};

const sys_fiber_conf_t fiberConf_8391_24_4f = {
    .portNum = 4,
    .pDetect = detectConf_8391_24_4f,
    .pOptical = opticalConf_8391_24_4f,
};
#endif

#ifdef CONFIG_SYS_RTL9300
const sys_fiber_conf_detect_t detectConf_9301_24_4f_yfd[] =
{
    {
        .port = 24, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_OE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_930X_GPIO(SYS_GPIO_EXTERNAL_7),
        .pGpio_los = BOARD_930X_GPIO(SYS_GPIO_EXTERNAL_8),
    },
    {
        .port = 25, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_OE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_930X_GPIO(SYS_GPIO_EXTERNAL_13),
        .pGpio_los = BOARD_930X_GPIO(SYS_GPIO_EXTERNAL_14),
    },
    {
        .port = 26, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_OE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_930X_GPIO(SYS_GPIO_EXTERNAL_25),
        .pGpio_los = BOARD_930X_GPIO(SYS_GPIO_EXTERNAL_26),
    },
    {
        .port = 27, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_OE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = BOARD_930X_GPIO(SYS_GPIO_EXTERNAL_28),
        .pGpio_los = BOARD_930X_GPIO(SYS_GPIO_EXTERNAL_29),
    },
};

const sys_fiber_conf_optical_t opticalConf_9301_24_4f_yfd[] =
{
    {
        .port = 24, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 2,
        },
    },
    {
        .port = 25, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 4,
        },
    },
    {
        .port = 26, 
        .i2c = {
            .device_id = I2C_DEV_ID2,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 6,
        },
    },
    {
        .port = 27, 
        .i2c = {
            .device_id = I2C_DEV_ID3,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 7,
        },
    },
};

const sys_fiber_conf_t fiberConf_9301_24_4f_yfd = {
    .portNum = 4,
    .pDetect = detectConf_9301_24_4f_yfd,
    .pOptical = opticalConf_9301_24_4f_yfd,
};

#endif

#ifdef CONFIG_SYS_RTL9310
const sys_fiber_conf_detect_t detectConf_9311_48_6f[] =
{
    {
        .port = 48, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_9].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_8].gpio,
    },
    {
        .port = 49, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_12].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_11].gpio,
    },
    {
        .port = 50, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_22].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_21].gpio,
    },
    {
        .port = 51, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_25].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_24].gpio,
    },
    {
        .port = 52, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_30].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_29].gpio,
    },
    {
        .port = 53, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_EXTERNAL_1].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_EXTERNAL_0].gpio,
    },
};

const sys_fiber_conf_optical_t opticalConf_9311_48_6f[] =
{
    {
        .port = 48, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 0,
        },
    },
    {
        .port = 49, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 1,
        },
    },
    {
        .port = 50, 
        .i2c = {
            .device_id = I2C_DEV_ID2,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 2,
        },
    },
    {
        .port = 51, 
        .i2c = {
            .device_id = I2C_DEV_ID3,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 3,
        },
    },
    {
        .port = 52, 
        .i2c = {
            .device_id = I2C_DEV_ID4,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 4,
        },
    },
    {
        .port = 53, 
        .i2c = {
            .device_id = I2C_DEV_ID5,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 5,
        },
    },
};

const sys_fiber_conf_t fiberConf_9311_48_6f = {
    .portNum = 6,
    .pDetect = detectConf_9311_48_6f,
    .pOptical = opticalConf_9311_48_6f,
};

const sys_fiber_conf_detect_t detectConf_9311_48_4f[] =
{
    {
        .port = 48, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_9].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_8].gpio,
    },
    {
        .port = 49, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_12].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_11].gpio,
    },
    {
        .port = 50, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_22].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_21].gpio,
    },
    {
        .port = 51, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_25].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_24].gpio,
    },
};

const sys_fiber_conf_optical_t opticalConf_9311_48_4f[] =
{
    {
        .port = 48, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 0,
        },
    },
    {
        .port = 49, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 1,
        },
    },
    {
        .port = 50, 
        .i2c = {
            .device_id = I2C_DEV_ID2,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 2,
        },
    },
    {
        .port = 51, 
        .i2c = {
            .device_id = I2C_DEV_ID3,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 3,
        },
    },
};

const sys_fiber_conf_t fiberConf_9311_48_4f = {
    .portNum = 4,
    .pDetect = detectConf_9311_48_4f,
    .pOptical = opticalConf_9311_48_4f,
};

const sys_fiber_conf_detect_t detectConf_9311_48_6f_yfd[] =
{
    {
        .port = 48, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_9].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_8].gpio,
    },
    {
        .port = 49, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_12].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_11].gpio,
    },
    {
        .port = 50, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_22].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_21].gpio,
    },
    {
        .port = 51, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_25].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_24].gpio,
    },
    {
        .port = 52, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_INTERNAL_7].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_INTERNAL_6].gpio,
    },
    {
        .port = 53, 
        .present = FIBER_DETECT_METHOD_OE,
        .mediaChange = FIBER_DETECT_METHOD_NONE,
        .enableOe = ENABLED,
        .enableLos = ENABLED,
        .pGpio_oe = &boardGpio_931x[SYS_GPIO_EXTERNAL_1].gpio,
        .pGpio_los = &boardGpio_931x[SYS_GPIO_EXTERNAL_0].gpio,
    },
};

const sys_fiber_conf_optical_t opticalConf_9311_48_6f_yfd[] =
{
    {
        .port = 48, 
        .i2c = {
            .device_id = I2C_DEV_ID0,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 0,
        },
    },
    {
        .port = 49, 
        .i2c = {
            .device_id = I2C_DEV_ID1,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 1,
        },
    },
    {
        .port = 50, 
        .i2c = {
            .device_id = I2C_DEV_ID2,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 2,
        },
    },
    {
        .port = 51, 
        .i2c = {
            .device_id = I2C_DEV_ID3,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 3,
        },
    },
    {
        .port = 52, 
        .i2c = {
            .device_id = I2C_DEV_ID4,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 4,
        },
    },
    {
        .port = 53, 
        .i2c = {
            .device_id = I2C_DEV_ID5,
            .i2c_interface_id = I2C_INTF_CONTROLLER_ID0,
            .dev_addr = I2C_DFL_CHIP_ID,
            .clk_freq = I2C_CLK_RT_50K,
            .scl_delay = I2C_DFL_DELAY_4000,
            .mem_addr_width = I2C_ADDR_WIDTH_BYTE_1,
            .data_width = I2C_ADDR_WIDTH_BYTE_1,
            .read_type = I2C_INTF_READ_TYPE_RANDOM,
            .scl_pin_id = 0,
            .sda_pin_id = 5,
        },
    },
};

const sys_fiber_conf_t fiberConf_9311_48_6f_yfd = {
    .portNum = 6,
    .pDetect = detectConf_9311_48_6f_yfd,
    .pOptical = opticalConf_9311_48_6f_yfd,
};

#endif

