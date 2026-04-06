
#include <board/board.h>
#ifdef CONFIG_SYS_POE

#define DEV_IN GEN_GPIO_DEV_ID0_INTERNAL
#define INF_SW I2C_INTF_SOFTWARE_DRV_ID
#define BYT_ON I2C_ADDR_WIDTH_BYTE_1
#define MOD_ST I2C_CLK_STD_MODE
#define MOD_FA I2C_CLK_FAST_MODE
#define DEL_20 I2C_DFL_DELAY_20
#define DEL_DF I2C_DFL_DELAY

#define dev_id  device_id
#define dev_ad  dev_addr
#define scl_pn  scl_pin_id
#define sda_pn  sda_pin_id
#define inf_id  i2c_interface_id
#define mem_wd  mem_addr_width
#define dat_wd  data_width
#define clk_fr  clk_freq
#define scl_dl  scl_delay
#define scl_de  scl_dev
#define sda_de  sda_dev

#ifdef CONFIG_SYS_RTL8380
const sys_poe_devConf_t poeDevConf_8380_8_2f[] =
{
    {.devType = PSE_TYPE_RTL8238B_L, .devNum = 0, .startp = 0,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = 0x3f, .scl_pn = 4, .sda_pn = 5, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = I2C_ADDR_WIDTH_BYTE_1, .dat_wd = I2C_DATA_WIDTH_BYTE_1,},},
	{.devType = PSE_TYPE_RTL8238B_H, .devNum = 1, .startp = 4,	.portNum = 4, .i2c = {.dev_id = I2C_DEV_ID7, .dev_ad = 0x3e, .scl_pn = 4, .sda_pn = 5, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = I2C_ADDR_WIDTH_BYTE_1, .dat_wd = I2C_DATA_WIDTH_BYTE_1,},},
    {.devType = PSE_TYPE_RTL8238B_L, .devNum =254,.startp =255, .portNum = 0, .i2c = {.dev_id = I2C_DEV_ID8, .dev_ad = 0x60, .scl_pn = 4, .sda_pn = 5, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = I2C_ADDR_WIDTH_BYTE_1, .dat_wd = I2C_DATA_WIDTH_BYTE_8238B_VB,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8380_8_2f[] =
{
    {.port = 0,  .devNum = 0, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 2,  .devNum = 0, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 3,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 4,  .devNum = 1, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 5,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 6,  .devNum = 1, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 7,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
};
const sys_poe_config_t poeConf_8380_8_2f =
{
    .portNum = 8,
    .startport = 0,
    .poeDevNum = 2,
    .poeLedEntity = 0,
    .pPoeDevConf = poeDevConf_8380_8_2f,
    .pPoePortConf = poePortConf_8380_8_2f,
};

const sys_poe_devConf_t poeDevConf_8382_24_4f[] =
{
    {.devType = 0, .devNum = 0, .startp = 0,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = PSE_I2C_ADDR_UNIT0, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = 0, .devNum = 1, .startp = 4,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID7, .dev_ad = PSE_I2C_ADDR_UNIT1, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = 0, .devNum = 3, .startp = 8,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID9, .dev_ad = PSE_I2C_ADDR_UNIT3, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = 0, .devNum = 2, .startp = 12, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID8, .dev_ad = PSE_I2C_ADDR_UNIT2, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = 0, .devNum = 4, .startp = 16, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID10,.dev_ad = PSE_I2C_ADDR_UNIT4, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = 0, .devNum = 5, .startp = 20, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID11,.dev_ad = PSE_I2C_ADDR_UNIT5, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = 0, .devNum =254,.startp =255, .portNum = 0, .i2c = {.dev_id = I2C_DEV_ID12,.dev_ad = PSE_I2C_ADDR_GLOBAL,.scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = 0, .devNum =255,.startp =255, .portNum = 0, .i2c = {.dev_id = I2C_DEV_ID13,.dev_ad = PSE_I2C_ADDR_TEMP,  .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8382_24_4f[] =
{
    {.port = 0,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 1,  .devNum = 0, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 2,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 3,  .devNum = 0, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 4,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 5,  .devNum = 1, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 6,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 7,  .devNum = 1, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 8,  .devNum = 3, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 9,  .devNum = 3, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 10, .devNum = 3, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 11, .devNum = 3, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 12, .devNum = 2, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 13, .devNum = 2, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 14, .devNum = 2, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 15, .devNum = 2, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 16, .devNum = 4, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 17, .devNum = 4, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 18, .devNum = 4, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 19, .devNum = 4, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 20, .devNum = 5, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 21, .devNum = 5, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 22, .devNum = 5, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 23, .devNum = 5, .devPort = 4, .mode = PSE_MODE_AT_AF, },
};
const sys_poe_config_t poeConf_8382_24_4f =
{
    .portNum = 24,
    .startport = 0,
    .poeDevNum = 6,
    .poeLedEntity = 2,
    .pPoeDevConf = poeDevConf_8382_24_4f,
    .pPoePortConf = poePortConf_8382_24_4f,
};

const sys_poe_devConf_t poeDevConf_8380_8_yfd[] =
{
    {.devType = PSE_TYPE_MP3924, .devNum = 0, .startp = 0,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = PSE_I2C_ADDR_UNIT0, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_MP3924, .devNum = 1, .startp = 4,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID7, .dev_ad = PSE_I2C_ADDR_UNIT1, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8380_8_yfd[] =
{
    {.port = 0,  .devNum = 0, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 2,  .devNum = 0, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 3,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 4,  .devNum = 1, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 5,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 6,  .devNum = 1, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 7,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
};
const sys_poe_config_t poeConf_8380_8_yfd =
{
    .portNum = 8,
    .startport = 0,
    .poeDevNum = 2,
    .poeLedEntity = 0,
    .pPoeDevConf = poeDevConf_8380_8_yfd,
    .pPoePortConf = poePortConf_8380_8_yfd,
};

const sys_poe_devConf_t poeDevConf_8380_8_yfd_bt[] =
{
    {.devType = PSE_TYPE_TI23880, .devNum = 0, .startp = 0,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = PSE_I2C_ADDR_UNIT0, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_TI23880, .devNum = 1, .startp = 2,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID7, .dev_ad = PSE_I2C_ADDR_UNIT1, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_TI23880, .devNum = 2, .startp = 4,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID8, .dev_ad = PSE_I2C_ADDR_UNIT2, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_TI23880, .devNum = 3, .startp = 6,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID9, .dev_ad = PSE_I2C_ADDR_UNIT3, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8380_8_yfd_bt[] =
{
    {.port = 0,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_BT,  },
    {.port = 2,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 3,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_BT,  },
    {.port = 4,  .devNum = 2, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 5,  .devNum = 2, .devPort = 3, .mode = PSE_MODE_BT,  },
    {.port = 6,  .devNum = 3, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 7,  .devNum = 3, .devPort = 3, .mode = PSE_MODE_BT,  },
};
const sys_poe_config_t poeConf_8380_8_yfd_bt =
{
    .portNum = 8,
    .startport = 0,
    .poeDevNum = 4,
    .poeLedEntity = 0,
    .pPoeDevConf = poeDevConf_8380_8_yfd_bt,
    .pPoePortConf = poePortConf_8380_8_yfd_bt,
};

const sys_poe_devConf_t poeDevConf_8382_24_yfd[] =
{
    {.devType = PSE_TYPE_MP3924, .devNum = 0, .startp = 0,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = PSE_I2C_ADDR_UNIT0, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_MP3924, .devNum = 1, .startp = 4,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID7, .dev_ad = PSE_I2C_ADDR_UNIT1, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_MP3924, .devNum = 2, .startp = 8,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID8, .dev_ad = PSE_I2C_ADDR_UNIT2, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_MP3924, .devNum = 3, .startp = 12, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID9, .dev_ad = PSE_I2C_ADDR_UNIT3, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_MP3924, .devNum = 4, .startp = 16, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID10,.dev_ad = PSE_I2C_ADDR_UNIT4, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_MP3924, .devNum = 5, .startp = 20, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID11,.dev_ad = PSE_I2C_ADDR_UNIT5, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8382_24_yfd[] =
{
    {.port = 0,  .devNum = 0, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 2,  .devNum = 0, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 3,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 4,  .devNum = 1, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 5,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 6,  .devNum = 1, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 7,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 8,  .devNum = 2, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 9,  .devNum = 2, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 10, .devNum = 2, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 11, .devNum = 2, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 12, .devNum = 3, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 13, .devNum = 3, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 14, .devNum = 3, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 15, .devNum = 3, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 16, .devNum = 4, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 17, .devNum = 4, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 18, .devNum = 4, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 19, .devNum = 4, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 20, .devNum = 5, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 21, .devNum = 5, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 22, .devNum = 5, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 23, .devNum = 5, .devPort = 1, .mode = PSE_MODE_AT_AF, },
};
const sys_poe_config_t poeConf_8382_24_yfd =
{
    .portNum = 24,
    .startport = 0,
    .poeDevNum = 6,
    .poeLedEntity = 2,
    .pPoeDevConf = poeDevConf_8382_24_yfd,
    .pPoePortConf = poePortConf_8382_24_yfd,
};

const sys_poe_devConf_t poeDevConf_8382_16_yfd[] =
{
    {.devType = PSE_TYPE_MP3924, .devNum = 0, .startp = 0,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = PSE_I2C_ADDR_UNIT0, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_MP3924, .devNum = 1, .startp = 4,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID7, .dev_ad = PSE_I2C_ADDR_UNIT1, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_MP3924, .devNum = 2, .startp = 8,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID8, .dev_ad = PSE_I2C_ADDR_UNIT2, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_MP3924, .devNum = 3, .startp = 12, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID9, .dev_ad = PSE_I2C_ADDR_UNIT3, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8382_16_yfd[] =
{
    {.port = 0,  .devNum = 0, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 2,  .devNum = 0, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 3,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 4,  .devNum = 1, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 5,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 6,  .devNum = 1, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 7,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 8,  .devNum = 2, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 9,  .devNum = 2, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 10, .devNum = 2, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 11, .devNum = 2, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 12, .devNum = 3, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 13, .devNum = 3, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 14, .devNum = 3, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 15, .devNum = 3, .devPort = 1, .mode = PSE_MODE_AT_AF, },
};
const sys_poe_config_t poeConf_8382_16_yfd =
{
    .portNum = 16,
    .startport = 0,
    .poeDevNum = 4,
    .poeLedEntity = 2,
    .pPoeDevConf = poeDevConf_8382_16_yfd,
    .pPoePortConf = poePortConf_8382_16_yfd,
};

const sys_poe_devConf_t poeDevConf_8380_4_yfd[] =
{
    {.devType = PSE_TYPE_MP3924, .devNum = 0, .startp = 0,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = PSE_I2C_ADDR_UNIT0, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8380_4_yfd[] =
{
    {.port = 0,  .devNum = 0, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 2,  .devNum = 0, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 3,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
};
const sys_poe_config_t poeConf_8380_4_yfd =
{
    .portNum = 4,
    .startport = 0,
    .poeDevNum = 1,
    .poeLedEntity = 0,
    .pPoeDevConf = poeDevConf_8380_4_yfd,
    .pPoePortConf = poePortConf_8380_4_yfd,
};

const sys_poe_devConf_t poeDevConf_8380_4_yfd_bt[] =
{
    {.devType = PSE_TYPE_TI23880, .devNum = 0, .startp = 0,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = PSE_I2C_ADDR_UNIT0, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_TI23880, .devNum = 1, .startp = 2,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID7, .dev_ad = PSE_I2C_ADDR_UNIT1, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8380_4_yfd_bt[] =
{
    {.port = 0,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_BT,  },
    {.port = 2,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 3,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_BT,  },
};
const sys_poe_config_t poeConf_8380_4_yfd_bt =
{
    .portNum = 4,
    .startport = 0,
    .poeDevNum = 2,
    .poeLedEntity = 0,
    .pPoeDevConf = poeDevConf_8380_4_yfd_bt,
    .pPoePortConf = poePortConf_8380_4_yfd_bt,
};

const sys_poe_devConf_t poeDevConf_8380_8_yfd_at_bt[] =
{
    {.devType = PSE_TYPE_MP3924,  .devNum = 0, .startp = 0,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = PSE_I2C_ADDR_UNIT0, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_TI23880, .devNum = 1, .startp = 4,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID7, .dev_ad = PSE_I2C_ADDR_UNIT2, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_TI23880, .devNum = 2, .startp = 6,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID8, .dev_ad = PSE_I2C_ADDR_UNIT3, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8380_8_yfd_at_bt[] =
{
    {.port = 0,  .devNum = 0, .devPort = 4, .mode = PSE_MODE_AT_AF,  },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_AT_AF,  },
    {.port = 2,  .devNum = 0, .devPort = 2, .mode = PSE_MODE_AT_AF,  },
    {.port = 3,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_AT_AF,  },
    {.port = 4,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 5,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_BT,  },
    {.port = 6,  .devNum = 2, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 7,  .devNum = 2, .devPort = 3, .mode = PSE_MODE_BT,  },
};
const sys_poe_config_t poeConf_8380_8_yfd_at_bt =
{
    .portNum = 8,
    .startport = 0,
    .poeDevNum = 3,
    .poeLedEntity = 0,
    .pPoeDevConf = poeDevConf_8380_8_yfd_at_bt,
    .pPoePortConf = poePortConf_8380_8_yfd_at_bt,
};

const sys_poe_devConf_t poeDevConf_8382_10_yfd_bt[] =
{
    {.devType = PSE_TYPE_TI23880, .devNum = 0, .startp = 0,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID6,  .dev_ad = PSE_I2C_ADDR_UNIT0, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_TI23880, .devNum = 1, .startp = 2,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID7,  .dev_ad = PSE_I2C_ADDR_UNIT1, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_TI23880, .devNum = 2, .startp = 4,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID8,  .dev_ad = PSE_I2C_ADDR_UNIT2, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_TI23880, .devNum = 3, .startp = 6,  .portNum = 2, .i2c = {.dev_id = I2C_DEV_ID9,  .dev_ad = PSE_I2C_ADDR_UNIT3, .scl_pn = PSE_I2C_SCL_PIN_ID_838X, .sda_pn = PSE_I2C_SDA_PIN_ID_838X, .clk_fr = MOD_ST, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_8382_10_yfd_bt[] =
{
    {.port = 0,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_BT,  },
    {.port = 2,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 3,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_BT,  },
    {.port = 4,  .devNum = 2, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 5,  .devNum = 2, .devPort = 3, .mode = PSE_MODE_BT,  },
    {.port = 6,  .devNum = 3, .devPort = 1, .mode = PSE_MODE_BT,  },
    {.port = 7,  .devNum = 3, .devPort = 3, .mode = PSE_MODE_BT,  },
};
const sys_poe_config_t poeConf_8382_10_yfd_bt =
{
    .portNum = 8,
    .startport = 0,
    .poeDevNum = 4,
    .poeLedEntity = 2,
    .pPoeDevConf = poeDevConf_8382_10_yfd_bt,
    .pPoePortConf = poePortConf_8382_10_yfd_bt,
};

#endif

#ifdef CONFIG_SYS_RTL9300
const sys_poe_devConf_t poeDevConf_9301_24_4f_yfd[] =
{
    {.devType = PSE_TYPE_IP808, .devNum = 0, .startp = 0,  .portNum = 8, .i2c = {.dev_id = I2C_DEV_ID6,  .dev_ad = 0x74, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_IP808, .devNum = 1, .startp = 8,  .portNum = 8, .i2c = {.dev_id = I2C_DEV_ID7,  .dev_ad = 0x70, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_IP808, .devNum = 2, .startp = 16, .portNum = 8, .i2c = {.dev_id = I2C_DEV_ID8,  .dev_ad = 0x71, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_9301_24_4f_yfd[] =
{
    {.port = 0,  .devNum = 0, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 1,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 2,  .devNum = 0, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 3,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 4,  .devNum = 0, .devPort = 6, .mode = PSE_MODE_AT_AF,},
    {.port = 5,  .devNum = 0, .devPort = 5, .mode = PSE_MODE_AT_AF,},
    {.port = 6,  .devNum = 0, .devPort = 8, .mode = PSE_MODE_AT_AF,},
    {.port = 7,  .devNum = 0, .devPort = 7, .mode = PSE_MODE_AT_AF,},
    {.port = 8,  .devNum = 1, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 9,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 10, .devNum = 1, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 11, .devNum = 1, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 12, .devNum = 1, .devPort = 6, .mode = PSE_MODE_AT_AF,},
    {.port = 13, .devNum = 1, .devPort = 5, .mode = PSE_MODE_AT_AF,},
    {.port = 14, .devNum = 1, .devPort = 8, .mode = PSE_MODE_AT_AF,},
    {.port = 15, .devNum = 1, .devPort = 7, .mode = PSE_MODE_AT_AF,},
    {.port = 16, .devNum = 2, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 17, .devNum = 2, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 18, .devNum = 2, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 19, .devNum = 2, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 20, .devNum = 2, .devPort = 6, .mode = PSE_MODE_AT_AF,},
    {.port = 21, .devNum = 2, .devPort = 5, .mode = PSE_MODE_AT_AF,},
    {.port = 22, .devNum = 2, .devPort = 8, .mode = PSE_MODE_AT_AF,},
    {.port = 23, .devNum = 2, .devPort = 7, .mode = PSE_MODE_AT_AF,},
};
const sys_poe_config_t poeConf_9301_24_4f_yfd =
{
    .portNum = 24,
    .startport = 0,
    .poeDevNum = 3,
    .poeLedEntity = 1,
    .pPoeDevConf = poeDevConf_9301_24_4f_yfd,
    .pPoePortConf = poePortConf_9301_24_4f_yfd,
};
const sys_poe_devConf_t poeDevConf_9301_16_4f[] =
{
    {.devType = PSE_TYPE_RTL8239_L, .devNum = 0, .startp = 0,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID6, .dev_ad = 0x3A, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8239_H, .devNum = 1, .startp = 2,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID7, .dev_ad = 0x3B, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_L, .devNum = 2, .startp = 4,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID8, .dev_ad = 0x3D, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_L, .devNum = 3, .startp = 8,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID9, .dev_ad = 0x3E, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_H, .devNum = 4, .startp = 12, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID10,.dev_ad = 0x3F, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8239_L, .devNum =255,.startp =255,  .portNum = 0,   .i2c = {.dev_id = I2C_DEV_ID13,.dev_ad = 0x3A, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = I2C_DATA_WIDTH_BYTE_8239_BT,},},
	{.devType = PSE_TYPE_RTL8238B_L, .devNum =254,.startp =255,  .portNum = 0,   .i2c = {.dev_id = I2C_DEV_ID14,.dev_ad = 0x60, .scl_pn = 3, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = I2C_DATA_WIDTH_BYTE_8238B_VB,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_9301_16_4f[] =
{
    {.port = 0,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_BT,},
    {.port = 1,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_BT,},
    {.port = 2,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_BT,},
    {.port = 3,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_BT,},
    {.port = 4,  .devNum = 2, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 5,  .devNum = 2, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 6,  .devNum = 2, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 7,  .devNum = 2, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 8,  .devNum = 3, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 9,  .devNum = 3, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 10, .devNum = 3, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 11, .devNum = 3, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 12, .devNum = 4, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 13, .devNum = 4, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 14, .devNum = 4, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 15, .devNum = 4, .devPort = 3, .mode = PSE_MODE_AT_AF, },
};
const sys_poe_config_t poeConf_9301_16_4f =
{
    .portNum = 16,
    .startport = 0,
    .poeDevNum = 5,
    .poeLedEntity = 1,
    .pPoeDevConf = poeDevConf_9301_16_4f,
    .pPoePortConf = poePortConf_9301_16_4f,
};

#endif

#ifdef CONFIG_SYS_RTL9310
const sys_poe_devConf_t poeDevConf_9311_48_4f[] =
{
    {.devType = PSE_TYPE_RTL8239_L, .devNum = 0, .startp = 0,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID6,  .dev_ad = 0x30, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8239_H, .devNum = 1, .startp = 2,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID7,  .dev_ad = 0x31, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_L,.devNum = 2, .startp = 4,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID8,  .dev_ad = 0x39, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_L,.devNum = 3, .startp = 8,  .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID9,  .dev_ad = 0x3C, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_H,.devNum = 4, .startp = 12, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID10, .dev_ad = 0x3D, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_L,.devNum = 5, .startp = 16, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID11, .dev_ad = 0x3E, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_H,.devNum = 6, .startp = 20, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID12, .dev_ad = 0x3F, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_L,.devNum = 7, .startp = 24, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID13, .dev_ad = 0x22, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_H,.devNum = 8, .startp = 28, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID14, .dev_ad = 0x23, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_L,.devNum = 9, .startp = 32, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID15, .dev_ad = 0x26, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_H,.devNum = 10,.startp = 36, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID16, .dev_ad = 0x27, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_L,.devNum = 11,.startp = 40, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID17, .dev_ad = 0x2E, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8238B_H,.devNum = 12,.startp = 44, .portNum = 4, .i2c = {.dev_id = I2C_DEV_ID18, .dev_ad = 0x2F, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_DF, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_RTL8239_L, .devNum =255,.startp =255, .portNum = 0, .i2c = {.dev_id = I2C_DEV_ID19, .dev_ad = 0x30, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = I2C_DATA_WIDTH_BYTE_8239_BT,},},
	{.devType = PSE_TYPE_RTL8238B_L,.devNum =254,.startp =255, .portNum = 0, .i2c = {.dev_id = I2C_DEV_ID20, .dev_ad = 0x60, .scl_pn = 29, .sda_pn = 30, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = I2C_DATA_WIDTH_BYTE_8238B_VB,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_9311_48_4f[] =
{
    {.port = 0,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_BT, },
    {.port = 1,  .devNum = 0, .devPort = 3, .mode = PSE_MODE_BT, },
    {.port = 2,  .devNum = 1, .devPort = 1, .mode = PSE_MODE_BT, },
    {.port = 3,  .devNum = 1, .devPort = 3, .mode = PSE_MODE_BT, },
    {.port = 4,  .devNum = 2, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 5,  .devNum = 2, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 6,  .devNum = 2, .devPort = 3, .mode = PSE_MODE_AT_AF, },
    {.port = 7,  .devNum = 2, .devPort = 4, .mode = PSE_MODE_AT_AF, },
    {.port = 8,  .devNum = 3, .devPort = 1, .mode = PSE_MODE_AT_AF, },
    {.port = 9,  .devNum = 3, .devPort = 2, .mode = PSE_MODE_AT_AF, },
    {.port = 10, .devNum = 3, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 11, .devNum = 3, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 12, .devNum = 4, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 13, .devNum = 4, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 14, .devNum = 4, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 15, .devNum = 4, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 16, .devNum = 5, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 17, .devNum = 5, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 18, .devNum = 5, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 19, .devNum = 5, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 20, .devNum = 6, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 21, .devNum = 6, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 22, .devNum = 6, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 23, .devNum = 6, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 24, .devNum = 7, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 25, .devNum = 7, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 26, .devNum = 7, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 27, .devNum = 7, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 28, .devNum = 8, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 29, .devNum = 8, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 30, .devNum = 8, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 31, .devNum = 8, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 32, .devNum = 9, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 33, .devNum = 9, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 34, .devNum = 9, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 35, .devNum = 9, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 36, .devNum = 10, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 37, .devNum = 10, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 38, .devNum = 10, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 39, .devNum = 10, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 40, .devNum = 11,.devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 41, .devNum = 11,.devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 42, .devNum = 11,.devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 43, .devNum = 11,.devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 44, .devNum = 12,.devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 45, .devNum = 12,.devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 46, .devNum = 12,.devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 47, .devNum = 12,.devPort = 4, .mode = PSE_MODE_AT_AF,},
};
const sys_poe_config_t poeConf_9311_48_4f =
{
    .portNum = 48,
    .startport = 0,
    .poeDevNum = 13,
    .poeLedEntity = 1,
    .pPoeDevConf = poeDevConf_9311_48_4f,
    .pPoePortConf = poePortConf_9311_48_4f,
};

const sys_poe_devConf_t poeDevConf_9311_48_6f_yfd[] =
{
    {.devType = PSE_TYPE_IP808, .devNum = 0, .startp = 0,  .portNum = 8, .i2c = {.dev_id = I2C_DEV_ID6,  .dev_ad = 0x70, .scl_pn = 5, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_IP808, .devNum = 1, .startp = 8,  .portNum = 8, .i2c = {.dev_id = I2C_DEV_ID7,  .dev_ad = 0x71, .scl_pn = 5, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_IP808, .devNum = 2, .startp = 16, .portNum = 8, .i2c = {.dev_id = I2C_DEV_ID8,  .dev_ad = 0x72, .scl_pn = 5, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_IP808, .devNum = 3, .startp = 24, .portNum = 8, .i2c = {.dev_id = I2C_DEV_ID9,  .dev_ad = 0x73, .scl_pn = 5, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_IP808, .devNum = 4, .startp = 32, .portNum = 8, .i2c = {.dev_id = I2C_DEV_ID10, .dev_ad = 0x74, .scl_pn = 5, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_IP808, .devNum = 5, .startp = 40, .portNum = 8, .i2c = {.dev_id = I2C_DEV_ID11, .dev_ad = 0x75, .scl_pn = 5, .sda_pn = 4, .clk_fr = MOD_FA, .scl_dl = DEL_20, .scl_de = DEV_IN, .sda_de = DEV_IN, .inf_id = INF_SW, .mem_wd = BYT_ON, .dat_wd = BYT_ON,},},
    {.devType = PSE_TYPE_END,},
};

const sys_poe_portConf_t poePortConf_9311_48_6f_yfd[] =
{
    {.port = 0,  .devNum = 4, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 1,  .devNum = 4, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 2,  .devNum = 4, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 3,  .devNum = 4, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 4,  .devNum = 4, .devPort = 5, .mode = PSE_MODE_AT_AF,},
    {.port = 5,  .devNum = 4, .devPort = 6, .mode = PSE_MODE_AT_AF,},
    {.port = 6,  .devNum = 4, .devPort = 7, .mode = PSE_MODE_AT_AF,},
    {.port = 7,  .devNum = 4, .devPort = 8, .mode = PSE_MODE_AT_AF,},
    {.port = 8,  .devNum = 0, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 9,  .devNum = 0, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 10, .devNum = 0, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 11, .devNum = 0, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 12, .devNum = 0, .devPort = 5, .mode = PSE_MODE_AT_AF,},
    {.port = 13, .devNum = 0, .devPort = 6, .mode = PSE_MODE_AT_AF,},
    {.port = 14, .devNum = 0, .devPort = 7, .mode = PSE_MODE_AT_AF,},
    {.port = 15, .devNum = 0, .devPort = 8, .mode = PSE_MODE_AT_AF,},
    {.port = 16, .devNum = 1, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 17, .devNum = 1, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 18, .devNum = 1, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 19, .devNum = 1, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 20, .devNum = 1, .devPort = 5, .mode = PSE_MODE_AT_AF,},
    {.port = 21, .devNum = 1, .devPort = 6, .mode = PSE_MODE_AT_AF,},
    {.port = 22, .devNum = 1, .devPort = 7, .mode = PSE_MODE_AT_AF,},
    {.port = 23, .devNum = 1, .devPort = 8, .mode = PSE_MODE_AT_AF,},
    {.port = 24, .devNum = 2, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 25, .devNum = 2, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 26, .devNum = 2, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 27, .devNum = 2, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 28, .devNum = 2, .devPort = 5, .mode = PSE_MODE_AT_AF,},
    {.port = 29, .devNum = 2, .devPort = 6, .mode = PSE_MODE_AT_AF,},
    {.port = 30, .devNum = 2, .devPort = 7, .mode = PSE_MODE_AT_AF,},
    {.port = 31, .devNum = 2, .devPort = 8, .mode = PSE_MODE_AT_AF,},
    {.port = 32, .devNum = 3, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 33, .devNum = 3, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 34, .devNum = 3, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 35, .devNum = 3, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 36, .devNum = 3, .devPort = 5, .mode = PSE_MODE_AT_AF,},
    {.port = 37, .devNum = 3, .devPort = 6, .mode = PSE_MODE_AT_AF,},
    {.port = 38, .devNum = 3, .devPort = 7, .mode = PSE_MODE_AT_AF,},
    {.port = 39, .devNum = 3, .devPort = 8, .mode = PSE_MODE_AT_AF,},
    {.port = 40, .devNum = 5, .devPort = 1, .mode = PSE_MODE_AT_AF,},
    {.port = 41, .devNum = 5, .devPort = 2, .mode = PSE_MODE_AT_AF,},
    {.port = 42, .devNum = 5, .devPort = 3, .mode = PSE_MODE_AT_AF,},
    {.port = 43, .devNum = 5, .devPort = 4, .mode = PSE_MODE_AT_AF,},
    {.port = 44, .devNum = 5, .devPort = 5, .mode = PSE_MODE_AT_AF,},
    {.port = 45, .devNum = 5, .devPort = 6, .mode = PSE_MODE_AT_AF,},
    {.port = 46, .devNum = 5, .devPort = 7, .mode = PSE_MODE_AT_AF,},
    {.port = 47, .devNum = 5, .devPort = 8, .mode = PSE_MODE_AT_AF,},
};
const sys_poe_config_t poeConf_9311_48_6f_yfd =
{
    .portNum = 48,
    .startport = 0,
    .poeDevNum = 6,
    .poeLedEntity = 1,
    .pPoeDevConf = poeDevConf_9311_48_6f_yfd,
    .pPoePortConf = poePortConf_9311_48_6f_yfd,
};
#endif
#endif 

