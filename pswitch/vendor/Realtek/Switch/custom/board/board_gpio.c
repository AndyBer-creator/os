
#include <rtk/switch.h>

#include <common/sys_type.h>
#include <common/sys_error.h>
#include <common/sys_def.h>

#include <board/board.h>
#include <osal/proc.h>

uint32 gGpioIdSum = 0;
sys_board_gpio_t *gpBoardGpio = NULL;
sys_board_gpio_t boardGpio[] =
{
    
    {   .id                    = SYS_GPIO_INTERNAL_0,},
    {   .id                    = SYS_GPIO_INTERNAL_1,},
    {   .id                    = SYS_GPIO_INTERNAL_2,},
    {   .id                    = SYS_GPIO_INTERNAL_3,},
    {   .id                    = SYS_GPIO_INTERNAL_4,},
    {   .id                    = SYS_GPIO_INTERNAL_5,},
    {   .id                    = SYS_GPIO_INTERNAL_6,},
    {   .id                    = SYS_GPIO_INTERNAL_7,},
    {   .id                    = SYS_GPIO_INTERNAL_8,},
    {   .id                    = SYS_GPIO_INTERNAL_9,},
    {   .id                    = SYS_GPIO_INTERNAL_10,},
    {   .id                    = SYS_GPIO_INTERNAL_11,},
    {   .id                    = SYS_GPIO_INTERNAL_12,},
    {   .id                    = SYS_GPIO_INTERNAL_13,},
    {   .id                    = SYS_GPIO_INTERNAL_14,},
    {   .id                    = SYS_GPIO_INTERNAL_15,},
    {   .id                    = SYS_GPIO_INTERNAL_16,},
    {   .id                    = SYS_GPIO_INTERNAL_17,},
    {   .id                    = SYS_GPIO_INTERNAL_18,},
    {   .id                    = SYS_GPIO_INTERNAL_19,},
    {   .id                    = SYS_GPIO_INTERNAL_20,},
    {   .id                    = SYS_GPIO_INTERNAL_21,},
    {   .id                    = SYS_GPIO_INTERNAL_22,},
    {   .id                    = SYS_GPIO_INTERNAL_23,},
    {   .id                    = SYS_GPIO_INTERNAL_24,},
    {   .id                    = SYS_GPIO_INTERNAL_25,},
    {   .id                    = SYS_GPIO_INTERNAL_26,},
    {   .id                    = SYS_GPIO_INTERNAL_27,},
    {   .id                    = SYS_GPIO_INTERNAL_28,},
    {   .id                    = SYS_GPIO_INTERNAL_29,},
    {   .id                    = SYS_GPIO_INTERNAL_30,},
    {   .id                    = SYS_GPIO_INTERNAL_31,},
    
    {   .id                    = SYS_GPIO_EXTERNAL_0,},
    {   .id                    = SYS_GPIO_EXTERNAL_1,},
    {   .id                    = SYS_GPIO_EXTERNAL_2,},
    {   .id                    = SYS_GPIO_EXTERNAL_3,},
    {   .id                    = SYS_GPIO_EXTERNAL_4,},
    {   .id                    = SYS_GPIO_EXTERNAL_5,},
    {   .id                    = SYS_GPIO_EXTERNAL_6,},
    {   .id                    = SYS_GPIO_EXTERNAL_7,},
    {   .id                    = SYS_GPIO_EXTERNAL_8,},
    {   .id                    = SYS_GPIO_EXTERNAL_9,},
    {   .id                    = SYS_GPIO_EXTERNAL_10,},
    {   .id                    = SYS_GPIO_EXTERNAL_11,},
    {   .id                    = SYS_GPIO_EXTERNAL_12,},
    {   .id                    = SYS_GPIO_EXTERNAL_13,},
    {   .id                    = SYS_GPIO_EXTERNAL_14,},
    {   .id                    = SYS_GPIO_EXTERNAL_15,},
    {   .id                    = SYS_GPIO_EXTERNAL_16,},
    {   .id                    = SYS_GPIO_EXTERNAL_17,},
    {   .id                    = SYS_GPIO_EXTERNAL_18,},
    {   .id                    = SYS_GPIO_EXTERNAL_19,},
    {   .id                    = SYS_GPIO_EXTERNAL_20,},
    {   .id                    = SYS_GPIO_EXTERNAL_21,},
    {   .id                    = SYS_GPIO_EXTERNAL_22,},
    {   .id                    = SYS_GPIO_EXTERNAL_23,},
    {   .id                    = SYS_GPIO_EXTERNAL_24,},
    {   .id                    = SYS_GPIO_EXTERNAL_25,},
    {   .id                    = SYS_GPIO_EXTERNAL_26,},
    {   .id                    = SYS_GPIO_EXTERNAL_27,},
    {   .id                    = SYS_GPIO_EXTERNAL_28,},
    {   .id                    = SYS_GPIO_EXTERNAL_29,},
    {   .id                    = SYS_GPIO_EXTERNAL_30,},
    {   .id                    = SYS_GPIO_EXTERNAL_31,},
    {   .id                    = SYS_GPIO_EXTERNAL_32,},
    {   .id                    = SYS_GPIO_EXTERNAL_33,},
    {   .id                    = SYS_GPIO_EXTERNAL_34,},
    {   .id                    = SYS_GPIO_EXTERNAL_35,},
    {   .id                    = SYS_GPIO_EXTERNAL_36,},
};

#ifdef CONFIG_SYS_RTL9300
sys_board_gpio_t boardGpio_930x[] =
{
    
    {   .id                    = SYS_GPIO_INTERNAL_0,},
    {   .id                    = SYS_GPIO_INTERNAL_1,},
    {   .id                    = SYS_GPIO_INTERNAL_2,},
    {   .id                    = SYS_GPIO_INTERNAL_3,},
    {   .id                    = SYS_GPIO_INTERNAL_4,},
    {   .id                    = SYS_GPIO_INTERNAL_5,},
    {   .id                    = SYS_GPIO_INTERNAL_6,},
    {   .id                    = SYS_GPIO_INTERNAL_7,},
    {   .id                    = SYS_GPIO_INTERNAL_8,},
    {   .id                    = SYS_GPIO_INTERNAL_9,},
    {   .id                    = SYS_GPIO_INTERNAL_10,},
    {   .id                    = SYS_GPIO_INTERNAL_11,},
    {   .id                    = SYS_GPIO_INTERNAL_12,},
    {   .id                    = SYS_GPIO_INTERNAL_13,},
    {   .id                    = SYS_GPIO_INTERNAL_14,},
    {   .id                    = SYS_GPIO_INTERNAL_15,},
    {   .id                    = SYS_GPIO_INTERNAL_16,},
    {   .id                    = SYS_GPIO_INTERNAL_17,},
    {   .id                    = SYS_GPIO_INTERNAL_18,},
    {   .id                    = SYS_GPIO_INTERNAL_19,},
    {   .id                    = SYS_GPIO_INTERNAL_20,},
    {   .id                    = SYS_GPIO_INTERNAL_21,},
    {   .id                    = SYS_GPIO_INTERNAL_22,},
    {   .id                    = SYS_GPIO_INTERNAL_23,},
    {   .id                    = SYS_GPIO_INTERNAL_24,},
    {   .id                    = SYS_GPIO_INTERNAL_25,},
    {   .id                    = SYS_GPIO_INTERNAL_26,},
    {   .id                    = SYS_GPIO_INTERNAL_27,},
    {   .id                    = SYS_GPIO_INTERNAL_28,},
    {   .id                    = SYS_GPIO_INTERNAL_29,},
    {   .id                    = SYS_GPIO_INTERNAL_30,},
    {   .id                    = SYS_GPIO_INTERNAL_31,},

    {   .id                    = SYS_GPIO_EXTERNAL_0,},
    {   .id                    = SYS_GPIO_EXTERNAL_1,},
    {   .id                    = SYS_GPIO_EXTERNAL_2,},
    {   .id                    = SYS_GPIO_EXTERNAL_3,},
    {   .id                    = SYS_GPIO_EXTERNAL_4,},
    {   .id                    = SYS_GPIO_EXTERNAL_5,},
    {   .id                    = SYS_GPIO_EXTERNAL_6,},
    {   .id                    = SYS_GPIO_EXTERNAL_7,},
    {   .id                    = SYS_GPIO_EXTERNAL_8,},
    {   .id                    = SYS_GPIO_EXTERNAL_9,},
    {   .id                    = SYS_GPIO_EXTERNAL_10,},
    {   .id                    = SYS_GPIO_EXTERNAL_11,},
    {   .id                    = SYS_GPIO_EXTERNAL_12,},
    {   .id                    = SYS_GPIO_EXTERNAL_13,},
    {   .id                    = SYS_GPIO_EXTERNAL_14,},
    {   .id                    = SYS_GPIO_EXTERNAL_15,},
    {   .id                    = SYS_GPIO_EXTERNAL_16,},
    {   .id                    = SYS_GPIO_EXTERNAL_17,},
    {   .id                    = SYS_GPIO_EXTERNAL_18,},
    {   .id                    = SYS_GPIO_EXTERNAL_19,},
    {   .id                    = SYS_GPIO_EXTERNAL_20,},
    {   .id                    = SYS_GPIO_EXTERNAL_21,},
    {   .id                    = SYS_GPIO_EXTERNAL_22,},
    {   .id                    = SYS_GPIO_EXTERNAL_23,},
    {   .id                    = SYS_GPIO_EXTERNAL_24,},
    {   .id                    = SYS_GPIO_EXTERNAL_25,},
    {   .id                    = SYS_GPIO_EXTERNAL_26,},
    {   .id                    = SYS_GPIO_EXTERNAL_27,},
    {   .id                    = SYS_GPIO_EXTERNAL_28,},
    {   .id                    = SYS_GPIO_EXTERNAL_29,},
    {   .id                    = SYS_GPIO_EXTERNAL_30,},
    {   .id                    = SYS_GPIO_EXTERNAL_31,},
    {   .id                    = SYS_GPIO_EXTERNAL_32,},
    {   .id                    = SYS_GPIO_EXTERNAL_33,},
    {   .id                    = SYS_GPIO_EXTERNAL_34,},
    {   .id                    = SYS_GPIO_EXTERNAL_35,},
    {   .id                    = SYS_GPIO_EXTERNAL_36,},
};
#endif

#ifdef CONFIG_SYS_RTL9310
sys_board_gpio_t boardGpio_931x[] =
{
    
    {   .id                    = SYS_GPIO_INTERNAL_0,},
    {   .id                    = SYS_GPIO_INTERNAL_1,},
    {   .id                    = SYS_GPIO_INTERNAL_2,},
    {   .id                    = SYS_GPIO_INTERNAL_3,},
    {   .id                    = SYS_GPIO_INTERNAL_4,},
    {   .id                    = SYS_GPIO_INTERNAL_5,},
    {   .id                    = SYS_GPIO_INTERNAL_6,},
    {   .id                    = SYS_GPIO_INTERNAL_7,},
    {   .id                    = SYS_GPIO_INTERNAL_8,},
    {   .id                    = SYS_GPIO_INTERNAL_9,},
    {   .id                    = SYS_GPIO_INTERNAL_10,},
    {   .id                    = SYS_GPIO_INTERNAL_11,},
    {   .id                    = SYS_GPIO_INTERNAL_12,},
    {   .id                    = SYS_GPIO_INTERNAL_13,},
    {   .id                    = SYS_GPIO_INTERNAL_14,},
    {   .id                    = SYS_GPIO_INTERNAL_15,},
    {   .id                    = SYS_GPIO_INTERNAL_16,},
    {   .id                    = SYS_GPIO_INTERNAL_17,},
    {   .id                    = SYS_GPIO_INTERNAL_18,},
    {   .id                    = SYS_GPIO_INTERNAL_19,},
    {   .id                    = SYS_GPIO_INTERNAL_20,},
    {   .id                    = SYS_GPIO_INTERNAL_21,},
    {   .id                    = SYS_GPIO_INTERNAL_22,},
    {   .id                    = SYS_GPIO_INTERNAL_23,},
    {   .id                    = SYS_GPIO_INTERNAL_24,},
    {   .id                    = SYS_GPIO_INTERNAL_25,},
    {   .id                    = SYS_GPIO_INTERNAL_26,},
    {   .id                    = SYS_GPIO_INTERNAL_27,},
    {   .id                    = SYS_GPIO_INTERNAL_28,},
    {   .id                    = SYS_GPIO_INTERNAL_29,},
    {   .id                    = SYS_GPIO_INTERNAL_30,},
    {   .id                    = SYS_GPIO_INTERNAL_31,},
    
    {   .id                    = SYS_GPIO_EXTERNAL_0,},
    {   .id                    = SYS_GPIO_EXTERNAL_1,},
    {   .id                    = SYS_GPIO_EXTERNAL_2,},
    {   .id                    = SYS_GPIO_EXTERNAL_3,},
    {   .id                    = SYS_GPIO_EXTERNAL_3,},
    {   .id                    = SYS_GPIO_EXTERNAL_4,},
    {   .id                    = SYS_GPIO_EXTERNAL_5,},
    {   .id                    = SYS_GPIO_EXTERNAL_6,},
    {   .id                    = SYS_GPIO_EXTERNAL_7,},
    {   .id                    = SYS_GPIO_EXTERNAL_8,},
    {   .id                    = SYS_GPIO_EXTERNAL_9,},
    {   .id                    = SYS_GPIO_EXTERNAL_10,},
    {   .id                    = SYS_GPIO_EXTERNAL_11,},
    {   .id                    = SYS_GPIO_EXTERNAL_12,},
    {   .id                    = SYS_GPIO_EXTERNAL_13,},
    {   .id                    = SYS_GPIO_EXTERNAL_14,},
    {   .id                    = SYS_GPIO_EXTERNAL_15,},
    {   .id                    = SYS_GPIO_EXTERNAL_16,},
    {   .id                    = SYS_GPIO_EXTERNAL_17,},
    {   .id                    = SYS_GPIO_EXTERNAL_18,},
    {   .id                    = SYS_GPIO_EXTERNAL_19,},
    {   .id                    = SYS_GPIO_EXTERNAL_20,},
    {   .id                    = SYS_GPIO_EXTERNAL_21,},
    {   .id                    = SYS_GPIO_EXTERNAL_22,},
    {   .id                    = SYS_GPIO_EXTERNAL_23,},
    {   .id                    = SYS_GPIO_EXTERNAL_24,},
    {   .id                    = SYS_GPIO_EXTERNAL_25,},
    {   .id                    = SYS_GPIO_EXTERNAL_26,},
    {   .id                    = SYS_GPIO_EXTERNAL_27,},
    {   .id                    = SYS_GPIO_EXTERNAL_28,},
    {   .id                    = SYS_GPIO_EXTERNAL_29,},
    {   .id                    = SYS_GPIO_EXTERNAL_30,},
    {   .id                    = SYS_GPIO_EXTERNAL_31,},
    {   .id                    = SYS_GPIO_EXTERNAL_32,},
    {   .id                    = SYS_GPIO_EXTERNAL_33,},
    {   .id                    = SYS_GPIO_EXTERNAL_34,},
    {   .id                    = SYS_GPIO_EXTERNAL_35,},
    {   .id                    = SYS_GPIO_EXTERNAL_36,},
};
#endif

static void board_gpio_sfp_txdisable(sys_boardGpio_id_t id, uint32 devId, uint32 pin, char * description)
{
    sys_board_gpio_t cfg = {
                               .id                    = id,
                               .gpio = {
                                    .unit              = SYS_DEV_ID_DEFAULT,
                                    .dev               = devId, 
                                    .pin               = pin,                        
                                    .pinConf = {
                                        .direction     = GPIO_DIR_OUT,              
                                        .default_value = GPIO_DFLT_LOW,
                                    },
                                    .active            = GPIO_ACTIVE_HIGH,
                                },
                                .inUse                 = TRUE,
                                .purpose               = "TX_DIS_FX_0",
                             };
    osal_memset(cfg.purpose, 0, sizeof(cfg.purpose));
    if(description != NULL)
    {
        osal_sprintf(cfg.purpose, "%s", description);
    }

    if(gpBoardGpio[id].id == id)
    {
        osal_memcpy(&gpBoardGpio[id], &cfg, sizeof(sys_board_gpio_t));
    }

}
static void board_gpio_sfp_exist(sys_boardGpio_id_t id, uint32 devId, uint32 pin, char * description)
{
    sys_board_gpio_t cfg = {
                               .id                    = id,
                               .gpio = {
                                    .unit              = SYS_DEV_ID_DEFAULT,
                                    .dev               = devId, 
                                    .pin               = pin,                        
                                    .pinConf = {
                                        .direction     = GPIO_DIR_IN,              
                                        .default_value = GPIO_DFLT_HIGH,
                                    },
                                    .active            = GPIO_ACTIVE_LOW,
                                },
                                .inUse                 = TRUE,
                                .purpose               = "MDO_FX_0",
                             };
    osal_memset(cfg.purpose, 0, sizeof(cfg.purpose));
    if(description != NULL)
    {
        osal_sprintf(cfg.purpose, "%s", description);
    }

    if(gpBoardGpio[id].id == id)
    {
        osal_memcpy(&gpBoardGpio[id], &cfg, sizeof(sys_board_gpio_t));
    }

}
static void board_gpio_sfp_los(sys_boardGpio_id_t id, uint32 devId, uint32 pin, char * description)
{
    sys_board_gpio_t cfg = {
                               .id                    = id,
                               .gpio = {
                                    .unit              = SYS_DEV_ID_DEFAULT,
                                    .dev               = devId, 
                                    .pin               = pin,                        
                                    .pinConf = {
                                        .direction     = GPIO_DIR_IN,              
                                        .default_value = GPIO_DFLT_HIGH,
                                    },
                                    .active            = GPIO_ACTIVE_LOW,
                                },
                                .inUse                 = TRUE,
                                .purpose               = "LOS_FX_0",
                             };
    osal_memset(cfg.purpose, 0, sizeof(cfg.purpose));
    if(description != NULL)
    {
        osal_sprintf(cfg.purpose, "%s", description);
    }

    if(gpBoardGpio[id].id == id)
    {
        osal_memcpy(&gpBoardGpio[id], &cfg, sizeof(sys_board_gpio_t));
    }

}

static void _board_gpio_extDev_init(uint32 devid, uint32 devAddr)
{
    drv_generalCtrlGpio_devConf_t devConf;

    osal_memset(&devConf, 0, sizeof(devConf));

    devConf.direction = GPIO_DIR_IN;
    devConf.ext_gpio.access_mode = EXT_GPIO_ACCESS_MODE_MDC;
    devConf.ext_gpio.page = 0;
    devConf.ext_gpio.address = devAddr;

    BOARD_INFO_MSG(("[GPIO] Initial External Device %d mode %d address %d\n",
                 devid,
                 devConf.ext_gpio.access_mode,
                 devConf.ext_gpio.address));

    drv_generalCtrlGPIO_dev_init(SYS_DEV_ID_DEFAULT, devid, &devConf);
    return;
}

int32 board_gpio_init(void)
{
    uint32 i = 0;
    uint32 boardId = HWP_IDENTIFIER_ID();

    switch(boardId)
    {
#ifdef CONFIG_SYS_RTL9310
        case SYS_BM_9311_48GE_6XGEF_DEMO:
        case SYS_BM_9311_48GE_6XGEF_YFD:
        case SYS_BM_9311_48GE_6XGEF_POE:
        case SYS_BM_9313_12XGEF:
            gpBoardGpio = boardGpio_931x;
            gGpioIdSum = SYS_GPIO_EXTERNAL_36 +1;
            break;
#endif
#ifdef CONFIG_SYS_RTL9300
        case SYS_BM_9301_24GE_4XGEF:
        case SYS_BM_9301_24GE_4XGEF_YFD:
        case SYS_BM_9301_24GE_4XGEF_POE:
            gpBoardGpio = boardGpio_930x;
            gGpioIdSum = SYS_GPIO_EXTERNAL_36 +1;
            break;
#endif
#ifdef CONFIG_SYS_RTL8380
        case SYS_BM_8380_8GE_2GEF:
        case SYS_BM_8380_8GE_2GEF_POE:
        case SYS_BM_8380_8GE_2GEF_POE_BT:
        case SYS_BM_8380_4GE_2GEF_YFD:
        case SYS_BM_8380_4GE_2GEF_POE:
        case SYS_BM_8380_4GE_2GEF_POE_BT:
        case SYS_BM_8382_24GE_4GEC:
        case SYS_BM_8382_24GE_4GEC_POE:
        case SYS_BM_8382_16GE_2GE:
        case SYS_BM_8382_16GE_2GE_POE:
        case SYS_BM_8382_10GE_4GF:
        case SYS_BM_8382_10GE_4GF_POE:
        case SYS_BM_8382_10GE_4GF_POE_BT:
            gpBoardGpio = boardGpio;
            gGpioIdSum = SYS_GPIO_EXTERNAL_36 +1;
            break;
#endif
        default:
            gpBoardGpio = boardGpio;
            gGpioIdSum = SYS_GPIO_EXTERNAL_36 +1;
            break;
    }

    if (NULL == gpBoardGpio)
    {
        BOOTMSG_FAIL_PRINTF("GPIO mapping table of boardId %u not found!\n", boardId);
        return SYS_ERR_FAILED;
    }

    if(gGpioIdSum > (SYS_GPIO_INTERNAL_31 + 1))
        _board_gpio_extDev_init(GEN_GPIO_DEV_ID1, 0);

    if(SYS_GPIO_EXTERNAL1_0 < gGpioIdSum)
    {
        _board_gpio_extDev_init(GEN_GPIO_DEV_ID2, 0x01);
    }

    if(SYS_GPIO_EXTERNAL2_0 < gGpioIdSum)
    {
        _board_gpio_extDev_init(GEN_GPIO_DEV_ID3, 0x02);
    }

#ifdef CONFIG_SYS_RTL9310
    if(boardId == SYS_BM_9313_2_5GE_4XGEF)
    {
        board_gpio_sfp_txdisable(SYS_GPIO_INTERNAL_10, GEN_GPIO_DEV_ID0_INTERNAL, 10, "TX_DIS_FX_0");
        board_gpio_sfp_txdisable(SYS_GPIO_INTERNAL_27, GEN_GPIO_DEV_ID0_INTERNAL, 27, "TX_DIS_FX_1");
        board_gpio_sfp_txdisable(SYS_GPIO_INTERNAL_30, GEN_GPIO_DEV_ID0_INTERNAL, 30, "TX_DIS_FX_2");
        board_gpio_sfp_txdisable(SYS_GPIO_INTERNAL_6, GEN_GPIO_DEV_ID0_INTERNAL, 6, "TX_DIS_FX_3");

        board_gpio_sfp_exist(SYS_GPIO_INTERNAL_9, GEN_GPIO_DEV_ID0_INTERNAL, 9, "MDO_FX_0");
        board_gpio_sfp_exist(SYS_GPIO_INTERNAL_12, GEN_GPIO_DEV_ID0_INTERNAL, 12, "MDO_FX_1");
        board_gpio_sfp_exist(SYS_GPIO_INTERNAL_29, GEN_GPIO_DEV_ID0_INTERNAL, 29, "MDO_FX_2");
        board_gpio_sfp_exist(SYS_GPIO_INTERNAL_7, GEN_GPIO_DEV_ID0_INTERNAL, 7, "MDO_FX_3");

        board_gpio_sfp_los(SYS_GPIO_INTERNAL_8, GEN_GPIO_DEV_ID0_INTERNAL, 8, "LOS_FX_0");
        board_gpio_sfp_los(SYS_GPIO_INTERNAL_11, GEN_GPIO_DEV_ID0_INTERNAL, 11, "LOS_FX_1");
        board_gpio_sfp_los(SYS_GPIO_INTERNAL_28, GEN_GPIO_DEV_ID0_INTERNAL, 28, "LOS_FX_2");
        board_gpio_sfp_los(SYS_GPIO_INTERNAL_31, GEN_GPIO_DEV_ID0_INTERNAL, 31, "LOS_FX_3");
    }

    if((boardId == SYS_BM_9311_48GE_6XGEF_YFD) || (boardId == SYS_BM_9311_48GE_6XGEF_POE))
    {
        board_gpio_sfp_txdisable(SYS_GPIO_INTERNAL_10, GEN_GPIO_DEV_ID0_INTERNAL, 10, "TX_DIS_FX_0");
        board_gpio_sfp_txdisable(SYS_GPIO_INTERNAL_14, GEN_GPIO_DEV_ID0_INTERNAL, 14, "TX_DIS_FX_1");
        board_gpio_sfp_txdisable(SYS_GPIO_INTERNAL_23, GEN_GPIO_DEV_ID0_INTERNAL, 23, "TX_DIS_FX_2");
        board_gpio_sfp_txdisable(SYS_GPIO_INTERNAL_26, GEN_GPIO_DEV_ID0_INTERNAL, 26, "TX_DIS_FX_3");
        board_gpio_sfp_txdisable(SYS_GPIO_INTERNAL_31, GEN_GPIO_DEV_ID0_INTERNAL, 31, "TX_DIS_FX_4");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_2 , GEN_GPIO_DEV_ID1,           2, "TX_DIS_FX_5");

        board_gpio_sfp_exist(SYS_GPIO_INTERNAL_9,  GEN_GPIO_DEV_ID0_INTERNAL,  9, "MDO_FX_0");
        board_gpio_sfp_exist(SYS_GPIO_INTERNAL_12, GEN_GPIO_DEV_ID0_INTERNAL, 12, "MDO_FX_1");
        board_gpio_sfp_exist(SYS_GPIO_INTERNAL_22, GEN_GPIO_DEV_ID0_INTERNAL, 22, "MDO_FX_2");
        board_gpio_sfp_exist(SYS_GPIO_INTERNAL_25, GEN_GPIO_DEV_ID0_INTERNAL, 25, "MDO_FX_3");
        board_gpio_sfp_exist(SYS_GPIO_INTERNAL_7,  GEN_GPIO_DEV_ID0_INTERNAL,  7, "MDO_FX_4");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_1,  GEN_GPIO_DEV_ID1,           1, "MDO_FX_5");

        board_gpio_sfp_los(SYS_GPIO_INTERNAL_8,  GEN_GPIO_DEV_ID0_INTERNAL, 8 , "LOS_FX_0");
        board_gpio_sfp_los(SYS_GPIO_INTERNAL_11, GEN_GPIO_DEV_ID0_INTERNAL, 11, "LOS_FX_1");
        board_gpio_sfp_los(SYS_GPIO_INTERNAL_21, GEN_GPIO_DEV_ID0_INTERNAL, 21, "LOS_FX_2");
        board_gpio_sfp_los(SYS_GPIO_INTERNAL_24, GEN_GPIO_DEV_ID0_INTERNAL, 24, "LOS_FX_3");
        board_gpio_sfp_los(SYS_GPIO_INTERNAL_6,  GEN_GPIO_DEV_ID0_INTERNAL, 6 , "LOS_FX_4");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_0,  GEN_GPIO_DEV_ID1,          0 , "LOS_FX_5");
    }
#endif
#ifdef CONFIG_SYS_RTL9300
    if((boardId == SYS_BM_9301_24GE_4XGEF)||(boardId == SYS_BM_9301_24GE_4XGEF_YFD)||(boardId == SYS_BM_9301_24GE_4XGEF_POE))
    {
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_6, GEN_GPIO_DEV_ID1, 6, "TX_DIS_FX_0");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_12, GEN_GPIO_DEV_ID1, 12, "TX_DIS_FX_1");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_24, GEN_GPIO_DEV_ID1, 24, "TX_DIS_FX_2");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_27, GEN_GPIO_DEV_ID1, 27, "TX_DIS_FX_3");

        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_7, GEN_GPIO_DEV_ID1, 7, "MDO_FX_0");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_13, GEN_GPIO_DEV_ID1, 13, "MDO_FX_1");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_25, GEN_GPIO_DEV_ID1, 25, "MDO_FX_2");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_28, GEN_GPIO_DEV_ID1, 28, "MDO_FX_3");

        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_8, GEN_GPIO_DEV_ID1, 8, "LOS_FX_0");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_14, GEN_GPIO_DEV_ID1, 14, "LOS_FX_1");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_26, GEN_GPIO_DEV_ID1, 26, "LOS_FX_2");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_29, GEN_GPIO_DEV_ID1, 29, "LOS_FX_3");
    }
#endif
#ifdef CONFIG_SYS_RTL8380
    if((boardId == SYS_BM_8380_8GE_2GEF)||(boardId == SYS_BM_8380_8GE_2GEF_POE)||(boardId == SYS_BM_8380_8GE_2GEF_POE_BT)
        ||(boardId == SYS_BM_8380_4GE_2GEF_YFD)||(boardId == SYS_BM_8380_4GE_2GEF_POE)||(boardId == SYS_BM_8380_4GE_2GEF_POE_BT))
    {
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_4,  GEN_GPIO_DEV_ID1, 4,  "TX_DIS_FX_0");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_9,  GEN_GPIO_DEV_ID1, 9,  "TX_DIS_FX_1");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_1, GEN_GPIO_DEV_ID1, 1, "MDO_FX_0");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_6, GEN_GPIO_DEV_ID1, 6, "MDO_FX_1");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_0,  GEN_GPIO_DEV_ID1, 0,  "LOS_FX_0");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_5,  GEN_GPIO_DEV_ID1, 5,  "LOS_FX_1");
    }
    else if((boardId == SYS_BM_8382_24GE_4GEC)||(boardId == SYS_BM_8382_24GE_4GEC_POE)
            ||(boardId == SYS_BM_8382_10GE_4GF)||(boardId == SYS_BM_8382_10GE_4GF_POE)
            ||(boardId == SYS_BM_8382_10GE_4GF_POE_BT))
    {
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_5,  GEN_GPIO_DEV_ID1, 5,  "TX_DIS_FX_0");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_0,  GEN_GPIO_DEV_ID1, 0,  "TX_DIS_FX_1");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_21, GEN_GPIO_DEV_ID1, 21, "TX_DIS_FX_2");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_10, GEN_GPIO_DEV_ID1, 10, "TX_DIS_FX_3");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_8,  GEN_GPIO_DEV_ID1, 8,  "MDO_FX_0");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_3,  GEN_GPIO_DEV_ID1, 3,  "MDO_FX_1");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_24, GEN_GPIO_DEV_ID1, 24, "MDO_FX_2");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_13, GEN_GPIO_DEV_ID1, 13, "MDO_FX_3");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_9,  GEN_GPIO_DEV_ID1, 9,  "LOS_FX_0");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_4,  GEN_GPIO_DEV_ID1, 4,  "LOS_FX_1");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_25, GEN_GPIO_DEV_ID1, 25, "LOS_FX_2");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_14, GEN_GPIO_DEV_ID1, 14, "LOS_FX_3");
    }
    else if((boardId == SYS_BM_8382_16GE_2GE)||(boardId == SYS_BM_8382_16GE_2GE_POE))
    {
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_5,  GEN_GPIO_DEV_ID1, 5,  "TX_DIS_FX_0");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_0,  GEN_GPIO_DEV_ID1, 0,  "TX_DIS_FX_1");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_21, GEN_GPIO_DEV_ID1, 21, "TX_DIS_FX_2");
        board_gpio_sfp_txdisable(SYS_GPIO_EXTERNAL_10, GEN_GPIO_DEV_ID1, 10, "TX_DIS_FX_3");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_8,  GEN_GPIO_DEV_ID1, 8,  "MDO_FX_0");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_3,  GEN_GPIO_DEV_ID1, 3,  "MDO_FX_1");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_24, GEN_GPIO_DEV_ID1, 24, "MDO_FX_2");
        board_gpio_sfp_exist(SYS_GPIO_EXTERNAL_13, GEN_GPIO_DEV_ID1, 13, "MDO_FX_3");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_9,  GEN_GPIO_DEV_ID1, 9,  "LOS_FX_0");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_4,  GEN_GPIO_DEV_ID1, 4,  "LOS_FX_1");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_25, GEN_GPIO_DEV_ID1, 25, "LOS_FX_2");
        board_gpio_sfp_los(SYS_GPIO_EXTERNAL_14, GEN_GPIO_DEV_ID1, 14, "LOS_FX_3");
    }
#endif

    for (i = 0; i < gGpioIdSum; i++)
    {
        if (gpBoardGpio[i].id != i)
        {
            SYS_PRINTF("GPIO Mapping error");
            return SYS_ERR_FAILED;
        }

        if (FALSE == gpBoardGpio[i].inUse || TRUE == gpBoardGpio[i].skipInit)
            continue;

        if (GEN_GPIO_DEV_ID0_INTERNAL == gpBoardGpio[i].gpio.dev)
        {
            BOARD_INFO_MSG(("[GPIO] Initial Internal pin %2u direction %3s default %d\n",
                          gpBoardGpio[i].gpio.pin,
                          ((GPIO_DIR_IN == gpBoardGpio[i].gpio.pinConf.direction) ? "IN" : "OUT"),
                          gpBoardGpio[i].gpio.pinConf.default_value));
        }
        else
        {
            BOARD_INFO_MSG(("[GPIO] Initial External%d pin %2u direction %3s default %d\n",
                          gpBoardGpio[i].gpio.dev,
                          gpBoardGpio[i].gpio.pin,
                          ((GPIO_DIR_IN == gpBoardGpio[i].gpio.pinConf.direction) ? "IN" : "OUT"),
                          gpBoardGpio[i].gpio.pinConf.default_value));
        }

        BOARD_GPIO_INIT(i);
    }

    if( (SYS_GPIO_EXTERNAL_0 < gGpioIdSum) && (gGpioIdSum <= SYS_GPIO_EXTERNAL1_0) )
    {
        drv_generalCtrlGPIO_devEnable_set(SYS_DEV_ID_DEFAULT, GEN_GPIO_DEV_ID1, ENABLED);
    }

    if( (SYS_GPIO_EXTERNAL1_0 < gGpioIdSum) && (gGpioIdSum <= SYS_GPIO_EXTERNAL2_0) )
    {
        drv_generalCtrlGPIO_devEnable_set(SYS_DEV_ID_DEFAULT, GEN_GPIO_DEV_ID2, ENABLED);
    }

    if( (SYS_GPIO_EXTERNAL2_0 < gGpioIdSum) && (gGpioIdSum <= SYS_GPIO_END) )
    {
        drv_generalCtrlGPIO_devEnable_set(SYS_DEV_ID_DEFAULT, GEN_GPIO_DEV_ID3, ENABLED);
    }

    return SYS_ERR_OK;
}

int32 board_gpio_model_init(uint32 boardId)
{
#ifdef CONFIG_SYS_RTL8390
    if (SYS_BM_8393_48GE_2GE_2GEC == boardId)
    {
        BOARD_GPIO_INIT(SYS_GPIO_INTERNAL_21);
        BOARD_GPIO_INIT(SYS_GPIO_INTERNAL_22);
    }
    else if (SYS_BM_8353_48FE_2GE_2GEC == boardId)
    {
        BOARD_GPIO_INIT(SYS_GPIO_INTERNAL_22);
        BOARD_GPIO_INIT(SYS_GPIO_INTERNAL_23);
    }
#endif

    return SYS_ERR_OK;
}

int32 board_gpio_dump(char *buf, int32 buflen, int32 *pLen)
{
    uint32 i = 0;
    uint32 data = 0;

    if (NULL == gpBoardGpio)
    {
        printk("GPIO mapping table not found!\n");
        return SYS_ERR_OK;
    }

    PROC_SEQ_PRINTF(buf, buflen, pLen, "Device  Pin  Direction  Default  Current  Purpose\n");
    PROC_SEQ_PRINTF(buf, buflen, pLen, "------- ---- ---------- -------- -------- --------\n"
);
    for (i = 0; i < gGpioIdSum; i++)
    {
        if (FALSE == gpBoardGpio[i].inUse)
            continue;

        drv_generalCtrlGPIO_dataBit_get(gpBoardGpio[i].gpio.unit,
                                        gpBoardGpio[i].gpio.dev,
                                        gpBoardGpio[i].gpio.pin,
                                        &data);

        if (IS_GPIO_INTERNAL(gpBoardGpio[i].gpio.dev))
        {
            PROC_SEQ_PRINTF(buf, buflen, pLen, "%-7s %-4d %-10s %-8d %-8d %s\n",
                            "INT",
                            gpBoardGpio[i].gpio.pin,
                            ((GPIO_DIR_IN == gpBoardGpio[i].gpio.pinConf.direction) ? "IN" : "OUT"),
                            gpBoardGpio[i].gpio.pinConf.default_value,
                            data, gpBoardGpio[i].purpose);
        }
        else
        {
            drv_gpio_direction_t dir = 0;
            drv_extGpio_direction_get(gpBoardGpio[i].gpio.unit,
                                      gpBoardGpio[i].gpio.dev - 1,
                                      gpBoardGpio[i].gpio.pin,
                                      &dir);

            PROC_SEQ_PRINTF(buf, buflen, pLen, "%-7s %-4d %-4s(%-4s) %-8d %-8d %s\n",
                            "EXT",
                            gpBoardGpio[i].gpio.pin,
                            ((GPIO_DIR_IN == gpBoardGpio[i].gpio.pinConf.direction) ? "IN" : "OUT"),
                            ((GPIO_DIR_IN == dir) ? "IN" : "OUT"),
                            gpBoardGpio[i].gpio.pinConf.default_value,
                            data, gpBoardGpio[i].purpose);
        }
    }

    return SYS_ERR_OK;
}

