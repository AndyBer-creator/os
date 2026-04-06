
#include <board/board.h>
#include <linux/osal/proc.h>

#ifdef CONFIG_SYS_RTL8380
extern const sys_port_conf_t portConf_8380_8ge_2gef[];
extern const sys_port_conf_t portConf_8382_24ge_4gec[];
extern const sys_port_conf_t portConf_8382_16ge_2ge[];
extern const sys_port_conf_t portConf_8380_4ge_2gef[];
extern const sys_port_conf_t portConf_8382_10ge_4gf[];
#ifdef CONFIG_SYS_FIBER
extern const sys_fiber_conf_t fiberConf_8380_8_2f;
extern const sys_fiber_conf_t fiberConf_8380_4_2f;
extern const sys_fiber_conf_t fiberConf_8382_24_4f;
extern const sys_fiber_conf_t fiberConf_8382_16_2f;
extern const sys_fiber_conf_t fiberConf_8382_10_4f;
#endif 
#ifdef CONFIG_SYS_BUTTON
extern const sys_button_conf_t buttonConf_8382_24_4f;
#endif 
#ifdef CONFIG_SYS_WATCHDOG
extern const sys_watchdog_conf_t watchdogConf;
#endif 
#ifdef CONFIG_SYS_LED
extern const sys_led_conf_t ledConf[];
#endif 
#ifdef CONFIG_SYS_POE
extern const sys_poe_config_t poeConf_8380_8_yfd;
extern const sys_poe_config_t poeConf_8380_8_yfd_bt;
extern const sys_poe_config_t poeConf_8382_24_yfd;
extern const sys_poe_config_t poeConf_8382_16_yfd;
extern const sys_poe_config_t poeConf_8380_4_yfd;
extern const sys_poe_config_t poeConf_8380_4_yfd_bt;
extern const sys_poe_config_t poeConf_8380_8_yfd_at_bt;
extern const sys_poe_config_t poeConf_8382_10_yfd_bt;
#endif 
#endif

#ifdef CONFIG_SYS_RTL9300
extern const sys_port_conf_t portConf_9300_24ge_4xgef_yfd[];
#ifdef CONFIG_SYS_FIBER
extern const sys_fiber_conf_t fiberConf_9301_24_4f_yfd;
#endif
#ifdef CONFIG_SYS_POE
extern const sys_poe_config_t poeConf_9301_24_4f_yfd;
#endif
#ifdef CONFIG_SYS_BUTTON
extern const sys_button_conf_t buttonConf_9301_24_4f;
#endif 
#ifdef CONFIG_SYS_WATCHDOG
extern const sys_watchdog_conf_t watchdogConf;
#endif 
#ifdef CONFIG_SYS_LED
extern const sys_led_conf_t ledConf[];
#endif 
#endif

#ifdef CONFIG_SYS_RTL9310
extern const sys_port_conf_t portConf_9310_48ge_6xgef[];
extern const sys_port_conf_t portConf_9310_48ge_4xgef[];
extern const sys_port_conf_t portConf_9313_12xgef[];
extern const sys_port_conf_t portConf_9313_2_5ge_4xgef[];
extern const sys_port_conf_t portConf_9313_24_2_5ge_6xgef_demo[];
#ifdef CONFIG_SYS_FIBER
extern const sys_fiber_conf_t fiberConf_9311_48_6f;
extern const sys_fiber_conf_t fiberConf_9311_48_4f;
extern const sys_fiber_conf_t fiberConf_9311_48_6f_yfd;
#endif 
#ifdef CONFIG_SYS_POE
extern const sys_poe_config_t poeConf_9311_48_6f_yfd;
#endif
#ifdef CONFIG_SYS_BUTTON
extern const sys_button_conf_t buttonConf_9311_48_4f;
#endif 
#ifdef CONFIG_SYS_WATCHDOG
extern const sys_watchdog_conf_t watchdogConf;
#endif 
#ifdef CONFIG_SYS_LED
extern const sys_led_conf_t ledConf[];
#endif 
#endif

sys_board_t *pBoardMap = NULL;

sys_board_t boardMap[] =
{
#ifdef CONFIG_SYS_RTL8380
    
    {
        .boardModelId  = SYS_BM_8380_8GE_2GEF,     
        .pPortConf     = portConf_8380_8ge_2gef,   
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8380_8_2f,     
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = NULL,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8380_8GE_2GEF_POE,     
        .pPortConf     = portConf_8380_8ge_2gef,   
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8380_8_2f,     
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_8380_8_yfd,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8380_8GE_2GEF_POE_BT,     
        .pPortConf     = portConf_8380_8ge_2gef,   
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8380_8_2f,     
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_8380_8_yfd_at_bt,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8382_24GE_4GEC,    
        .pPortConf     = portConf_8382_24ge_4gec,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8382_24_4f,    
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = NULL,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8382_24GE_4GEC_POE,    
        .pPortConf     = portConf_8382_24ge_4gec,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8382_24_4f,    
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_8382_24_yfd,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8382_16GE_2GE,    
        .pPortConf     = portConf_8382_16ge_2ge,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8382_16_2f,    
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = NULL,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
        .pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8382_16GE_2GE_POE,    
        .pPortConf     = portConf_8382_16ge_2ge,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8382_16_2f,    
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_8382_16_yfd,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
        .pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8380_4GE_2GEF_YFD,     
        .pPortConf     = portConf_8380_4ge_2gef,   
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8380_4_2f,     
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = NULL,           
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8380_4GE_2GEF_POE,     
        .pPortConf     = portConf_8380_4ge_2gef,   
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8380_4_2f,     
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_8380_4_yfd,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8380_4GE_2GEF_POE_BT,     
        .pPortConf     = portConf_8380_4ge_2gef,   
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8380_4_2f,     
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_8380_4_yfd_bt,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8382_10GE_4GF,    
        .pPortConf     = portConf_8382_10ge_4gf,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8382_10_4f,    
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = NULL,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_8382_10GE_4GF_POE,    
        .pPortConf     = portConf_8382_10ge_4gf,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8382_10_4f,    
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_8380_8_yfd,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },    
    {
        .boardModelId  = SYS_BM_8382_10GE_4GF_POE_BT,    
        .pPortConf     = portConf_8382_10ge_4gf,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,            
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_8382_10_4f,    
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                  
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_8382_10_yfd_bt,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_8382_24_4f,
      #endif
    },    
#endif

#ifdef CONFIG_SYS_RTL9300
    {
        .boardModelId  = SYS_BM_9301_24GE_4XGEF_YFD,    
        .pPortConf     = portConf_9300_24ge_4xgef_yfd,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,                
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_9301_24_4f_yfd,  
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                      
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = NULL,                     
      #endif
      #ifdef CONFIG_SYS_BUTTON
        .pButtonConf   = &buttonConf_9301_24_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_9301_24GE_4XGEF_POE,    
        .pPortConf     = portConf_9300_24ge_4xgef_yfd,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,                
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_9301_24_4f_yfd, 
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                      
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_9301_24_4f_yfd,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
        .pButtonConf   = &buttonConf_9301_24_4f,
      #endif
    },
#endif

#ifdef CONFIG_SYS_RTL9310
    {
        .boardModelId  = SYS_BM_9311_48GE_6XGEF,    
        .pPortConf     = portConf_9310_48ge_6xgef,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,                
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = NULL,                          
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                      
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = NULL,                     
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = NULL,
      #endif
    },
    {
        .boardModelId  = SYS_BM_9311_48GE_6XGEF_YFD,    
        .pPortConf     = portConf_9310_48ge_6xgef,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,                
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_9311_48_6f_yfd, 
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                      
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = NULL,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_9311_48_4f,
      #endif
    },
    {
        .boardModelId  = SYS_BM_9311_48GE_6XGEF_POE,    
        .pPortConf     = portConf_9310_48ge_6xgef,  
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = &watchdogConf,                
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = &fiberConf_9311_48_6f_yfd, 
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = ledConf,                      
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = &poeConf_9311_48_6f_yfd,       
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = &buttonConf_9311_48_4f,
      #endif
    },
#endif
    
    {
        .boardModelId  = SYS_BM_END,               
        .pPortConf     = NULL,                     
      #ifdef CONFIG_SYS_WATCHDOG
        .pWatchdogConf = NULL,                     
      #endif
      #ifdef CONFIG_SYS_FIBER
        .pFiberConf    = NULL,                     
      #endif
      #ifdef CONFIG_SYS_LED
        .pLedConf      = NULL,                     
      #endif
      #ifdef CONFIG_SYS_POE
        .pPoeConf      = NULL,                     
      #endif
      #ifdef CONFIG_SYS_BUTTON
      	.pButtonConf   = NULL,
      #endif
    }
};

int32 board_conf_get(sys_board_t **ppConf)
{
    uint32 idx = 0;

    if (NULL == pBoardMap)
    {
        idx = 0;
        while(boardMap[idx].boardModelId != SYS_BM_END)
        {
            if(boardMap[idx].boardModelId == sys_boardModelId_ret())
            {
                pBoardMap = &boardMap[idx];

                break;
            }
            idx++;
        }
    }

    if (NULL != pBoardMap)
        *ppConf = pBoardMap;
    else
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}

static char smi_typeStr[I2C_ADDR_WIDTH_BYTE_END][SYS_BUF8_LEN] = { "0 BITS", "8 BITS", "16 BITS", "24 BITS"};
static char stateStr[2][SYS_BUF8_LEN] = { "Disabled", "Enabled" };

static int32 _dump_port(const sys_port_conf_t *pPortConf, char *buf, int32 buflen, int32 *pLen)
{
    sys_logic_port_t lp = 0;
    char portTypeStr[PORT_TYPE_END][SYS_BUF16_LEN] =
        { "100M", "1000M",

"10G", "LAG", "Combo_100M", "Combo_1000M"};
    char mediaStr[PORT_MEDIA_END][SYS_BUF16_LEN] =
        { "Copper", "Fiber"};
    char speedStr[PORTCONF_SPEED_END][SYS_BUF16_LEN] =
        { "(F) 10M", "(F) 100M", "(F) 1000M", "(A) 10M", "(A) 100M", "(A) 1000M", "(A) 10M_100M", "(A) ALL" };
    char duplexStr[PORTCONF_DUPLEX_END][SYS_BUF16_LEN] =
        { "(F) Full", "(F) Half", "Auto" };

    PROC_SEQ_PRINTF(buf, buflen, pLen, "====== Port ==================\n");

    PROC_SEQ_PRINTF(buf, buflen, pLen, "Type        Usr  Phy     Media       Speed          Duplex   Attr\n");
    PROC_SEQ_PRINTF(buf, buflen, pLen, "----------- ---- ------- ----------- -------------- -------- -------\n");
    while (pPortConf[lp].devId != PORTCONF_END_VAL)
    {
        PROC_SEQ_PRINTF(buf, buflen, pLen, "%-11s %-4u (%-1u) %-3u %-11s %-14s %-8s %-7x\n",
                        portTypeStr[pPortConf[lp].portType],
                        pPortConf[lp].usrPort,
                        pPortConf[lp].devId,
                        pPortConf[lp].phyPort,
                        mediaStr[pPortConf[lp].mediaType],
                        speedStr[pPortConf[lp].speed],
                        duplexStr[pPortConf[lp].duplex],
                        pPortConf[lp].attr);
        lp++;
    }

    PROC_SEQ_PRINTF(buf, buflen, pLen, "\n\n");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_FIBER
static int32 _dump_fiber(const sys_fiber_conf_t *pFiberConf, char *buf, int32 buflen, int32 *pLen)
{
    uint32 i = 0;

    PROC_SEQ_PRINTF(buf, buflen, pLen, "====== Fiber =================\n");

    PROC_SEQ_PRINTF(buf, buflen, pLen, "Fiber Port Number: %d\n", pFiberConf->portNum);

    if (NULL != pFiberConf->pDetect)
    {
        sys_fiber_conf_detect_t *pDetectConf = (sys_fiber_conf_detect_t *) pFiberConf->pDetect;
        char detectStr[FIBER_DETECT_METHOD_END][SYS_BUF8_LEN] = { "None", "OE", "LOS" };

        PROC_SEQ_PRINTF(buf, buflen, pLen, "\n------------ Fiber Detect\n");
        PROC_SEQ_PRINTF(buf, buflen, pLen, "LPort  Present  MediaChg  OE Status              LOS Status\n");
        PROC_SEQ_PRINTF(buf, buflen, pLen, "------ -------- --------- ---------------------- ----------------------\n");
        for (i = 0; i < pFiberConf->portNum; i++)
        {
            PROC_SEQ_PRINTF(buf, buflen, pLen, "%-6u %-8s %-9s ",
                            pDetectConf[i].port,
                            detectStr[pDetectConf[i].present],
                            detectStr[pDetectConf[i].mediaChange]);

            if (NULL ==pDetectConf[i].pGpio_oe)
            {
                PROC_SEQ_PRINTF(buf, buflen, pLen, "%-8s (GPIO:%6s) ", stateStr[pDetectConf[i].enableOe], "NULL");
            }
            else
            {
                PROC_SEQ_PRINTF(buf, buflen, pLen, "%-8s (GPIO:%3s_%-2u) ",
                                stateStr[pDetectConf[i].enableOe],
                                (IS_GPIO_INTERNAL(pDetectConf[i].pGpio_oe->dev) ? "INT" : "EXT"),
                                pDetectConf[i].pGpio_oe->pin);
            }

            if (NULL == pDetectConf[i].pGpio_los)
            {
                PROC_SEQ_PRINTF(buf, buflen, pLen, "%-8s (GPIO:%6s)", stateStr[pDetectConf[i].enableLos], "NULL");
            }
            else
            {
                PROC_SEQ_PRINTF(buf, buflen, pLen, "%-8s (GPIO:%3s_%-2u)",
                                stateStr[pDetectConf[i].enableLos],
                                (IS_GPIO_INTERNAL(pDetectConf[i].pGpio_los->dev) ? "INT" : "EXT"),
                                pDetectConf[i].pGpio_los->pin);
            }
            PROC_SEQ_PRINTF(buf, buflen, pLen, "\n");
        }
    }

    if (NULL != pFiberConf->pOptical)
    {
        sys_fiber_conf_optical_t *pOpticalConf = (sys_fiber_conf_optical_t *) pFiberConf->pOptical;

        PROC_SEQ_PRINTF(buf, buflen, pLen, "\n------------ Fiber Optical ");

        if (I2C_INTF_SOFTWARE_DRV_ID == pOpticalConf[i].i2c.i2c_interface_id)
            PROC_SEQ_PRINTF(buf, buflen, pLen, "Connect by I2C Interface Software using GPIO\n");
        else if (I2C_INTF_CONTROLLER_ID0 == pOpticalConf[i].i2c.i2c_interface_id)
            PROC_SEQ_PRINTF(buf, buflen, pLen, "Connect by I2C Hardware Controller 0\n");
        else if (I2C_INTF_CONTROLLER_ID1 == pOpticalConf[i].i2c.i2c_interface_id)
            PROC_SEQ_PRINTF(buf, buflen, pLen, "Connect by I2C Hardware Controller 1\n");

        PROC_SEQ_PRINTF(buf, buflen, pLen, "LPort  I2C DEV  I2C TYPE  ID    Delay   SCK    SDA\n");
        PROC_SEQ_PRINTF(buf, buflen, pLen, "------ -------- --------- ----- ------- ------ ------\n");
        for (i = 0; i < pFiberConf->portNum; i++)
        {
            if (I2C_INTF_SOFTWARE_DRV_ID == pOpticalConf[i].i2c.i2c_interface_id)
            {
                PROC_SEQ_PRINTF(buf, buflen, pLen, "%-6u %-8u %-9s 0x%-3x %-7d %-3s_%-2u %-3s_%-2u\n",
                            pOpticalConf[i].port,
                            pOpticalConf[i].i2c.device_id,
                            smi_typeStr[pOpticalConf[i].i2c.mem_addr_width],
                            pOpticalConf[i].i2c.dev_addr,
                            pOpticalConf[i].i2c.scl_delay,
                            (IS_GPIO_INTERNAL(pOpticalConf[i].i2c.scl_dev) ? "INT" : "EXT"),
                            pOpticalConf[i].i2c.scl_pin_id,
                            (IS_GPIO_INTERNAL(pOpticalConf[i].i2c.sda_dev) ? "INT" : "EXT"),
                            pOpticalConf[i].i2c.sda_pin_id);
            }
            else
            {
                PROC_SEQ_PRINTF(buf, buflen, pLen, "%-6u %-8u %-9s 0x%-3x %-7d %-6u %-6u\n",
                            pOpticalConf[i].port,
                            pOpticalConf[i].i2c.device_id,
                            smi_typeStr[pOpticalConf[i].i2c.mem_addr_width],
                            pOpticalConf[i].i2c.dev_addr,
                            pOpticalConf[i].i2c.scl_delay,
                            pOpticalConf[i].i2c.scl_pin_id,
                            pOpticalConf[i].i2c.sda_pin_id);
            }
        }
    }

    PROC_SEQ_PRINTF(buf, buflen, pLen, "\n\n");

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_LED
static int32 _dump_led(const sys_led_conf_t *pLedConf, char *buf, int32 buflen, int32 *pLen)
{
    uint32 i = 0;
    char typeStr[SYS_LED_TYPE_END][SYS_BUF16_LEN] =
        { "SYS", "ALARM", "SYS_BLINK" };
    char ctrlStr[LED_CTRL_END][SYS_BUF16_LEN] =
        { "REG", "Port REG", "GPIO", "GPIO Blink" };

    PROC_SEQ_PRINTF(buf, buflen, pLen, "====== Led ===================\n");

    while (SYS_LED_TYPE_END != pLedConf[i].type)
    {
        sys_led_conf_ctrl_t *pCtrlConf = (sys_led_conf_ctrl_t *) pLedConf[i].pLedCtrlConf;
        PROC_SEQ_PRINTF(buf, buflen, pLen, "%s (%s)\n",
                        typeStr[pLedConf[i].type],
                        ctrlStr[pCtrlConf->ctrlType]);

        i++;
    }

    PROC_SEQ_PRINTF(buf, buflen, pLen, "\n\n");

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_WATCHDOG
static int32 _dump_watchdog(const sys_watchdog_conf_t *pWatchdogConf, char *buf, int32 buflen, int32 *pLen)
{
    char typeStr[WATCHDOG_TYPE_END][SYS_BUF8_LEN] = { "REG", "GPIO" };

    PROC_SEQ_PRINTF(buf, buflen, pLen, "====== WatchDog ==============\n");

    PROC_SEQ_PRINTF(buf, buflen, pLen, "Type: %s\n", typeStr[pWatchdogConf->type]);

    if (WATCHDOG_TYPE_GPIO == pWatchdogConf->type)
    {
        sys_watchdog_goio_conf_t *pGpioConf = (sys_watchdog_goio_conf_t *) pWatchdogConf->pConf;

        PROC_SEQ_PRINTF(buf, buflen, pLen, "Feed Time: %u\n", pGpioConf->feedTime);
        PROC_SEQ_PRINTF(buf, buflen, pLen, "GPIO Feed: %-3s_%-2u\n",
                      (IS_GPIO_INTERNAL(pGpioConf->pFeed->dev) ? "INT" : "EXT"),
                      pGpioConf->pFeed->pin);

        PROC_SEQ_PRINTF(buf, buflen, pLen, "GPIO State: %-3s_%-2u\n",
                      (IS_GPIO_INTERNAL(pGpioConf->pState->dev) ? "INT" : "EXT"),
                      pGpioConf->pState->pin);
    }

    PROC_SEQ_PRINTF(buf, buflen, pLen, "\n\n");

    return 0;
}
#endif

int32 board_conf_dump(char *buf, int32 buflen, int32 *pLen)
{
    sys_board_t *pBoardConf = NULL;
    sys_boardModel_t boardModel;
    uint32 dumpMode = 0;

    sys_boardModel_get(&boardModel);

    PROC_SEQ_PRINTF(buf, buflen, pLen, "****************************\n");
    PROC_SEQ_PRINTF(buf, buflen, pLen, " %s\n", boardModel.boardConf.boardDescp);
    PROC_SEQ_PRINTF(buf, buflen, pLen, "****************************\n");

    if (0 == dumpMode || 1 == dumpMode)
    {
        PROC_SEQ_PRINTF(buf, buflen, pLen, "============================\n");
        PROC_SEQ_PRINTF(buf, buflen, pLen, " Board GPIO\n");
        PROC_SEQ_PRINTF(buf, buflen, pLen, "============================\n");
        board_gpio_dump(buf, buflen, pLen);
    }

    PROC_SEQ_PRINTF(buf, buflen, pLen, "\n");

    if (0 == dumpMode || 2 == dumpMode)
    {
        PROC_SEQ_PRINTF(buf, buflen, pLen, "============================\n");
        PROC_SEQ_PRINTF(buf, buflen, pLen, " Board Configuration\n");
        PROC_SEQ_PRINTF(buf, buflen, pLen, "============================\n");
        board_conf_get(&pBoardConf);

        if (NULL == pBoardConf)
            return SYS_ERR_NULL_POINTER;

        if (NULL != pBoardConf->pPortConf)
            _dump_port(pBoardConf->pPortConf, buf, buflen, pLen);

#ifdef CONFIG_SYS_FIBER
        if (NULL != pBoardConf->pFiberConf)
            _dump_fiber(pBoardConf->pFiberConf, buf, buflen, pLen);
#endif

#ifdef CONFIG_SYS_LED
        if (NULL != pBoardConf->pLedConf)
            _dump_led(pBoardConf->pLedConf, buf, buflen, pLen);
#endif

#ifdef CONFIG_SYS_WATCHDOG
        if (NULL != pBoardConf->pWatchdogConf)
            _dump_watchdog(pBoardConf->pWatchdogConf, buf, buflen, pLen);
#endif
    }

    return SYS_ERR_OK;
}

