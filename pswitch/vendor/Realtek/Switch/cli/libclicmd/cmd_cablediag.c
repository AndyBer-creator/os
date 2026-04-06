
#include <libcmd/cmd.h>
#include <libsal/sal_cablediag.h>

#include <libsal/sal_fiber.h>
#include <libsal/sal_optical.h>

int32 cmd_cablediag_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8                   i;
    sys_port_type_t         portType;
    sys_logic_port_t        logicPort;
    sys_logic_portmask_t    logicPortmask;
    sys_port_media_t        portMedia;
    sys_rtctResult_t        rtctResult;
    char                    port_str[8];
    sys_port_speed_t        speed;
    const uint8             CHANNEL_NUM = 4;
    uint32                  channelLenM, channelLenCM, channelOpen, channelShort, channelMismatch, lineDriver;

    osal_memset(&rtctResult, 0, sizeof(sys_rtctResult_t));
    osal_memset(&portMedia, 0, sizeof(sys_port_media_t));
    osal_memset(&logicPortmask, 0, sizeof(sys_logic_portmask_t));

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE(" Port   |  Speed | Local pair | Pair length | Pair status \n");
    XMORE("--------+--------+------------+-------------+---------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        osal_memset(port_str, 0, sizeof(port_str));
        SYS_ERR_CHK(LP2STR(logicPort, port_str));

        SYS_ERR_CHK(sal_port_linkMedia_get(logicPort, &portMedia));
        portType = LP2TYPE(logicPort);
        if (PORT_MEDIA_FIBER == portMedia)
        {
            XMORE(" %4s   |     Fiber", port_str);
        }
        else if (PORT_TYPE_LAG == portType)
        {
             
            continue;
        }
        else
        {
            
            SYS_MEM_CLEAR(speed);
            SYS_ERR_CHK(sal_port_speed_get(logicPort, &speed));
            SYS_ERR_CHK(sal_cablediag_copper_set(logicPort));
            SYS_ERR_CHK(sal_cablediag_copper_get(logicPort, &rtctResult));

            for (i = 0; i < CHANNEL_NUM; i++)
            {
                if (i == 0)
                {
                    XMORE(" %4s   |", port_str);

                    if (ENABLED == speed.autoSpeed)
                        XMORE("  auto  |");
                    else if (SYS_PORT_SPEED_10M == speed.ability)
                        XMORE("  10Mb  |");
                    else if (SYS_PORT_SPEED_100M == speed.ability)
                        XMORE(" 100Mb  |");
                    else if (SYS_PORT_SPEED_1000M == speed.ability)
                        XMORE(" 1000Mb |");
                }
                else
                {
                    XMORE("                  ");
                }

                XMORE("    Pair %c  |", 'A' + i);

                if (PORT_SPEED_10M == rtctResult.linkType || PORT_SPEED_100M == rtctResult.linkType)
                {
                    if (i == 0)
                    {
                        XMORE("  %4d.%02d    |", rtctResult.un.fe_result.rxLen/100, rtctResult.un.fe_result.rxLen%100);
                        if (rtctResult.un.fe_result.isRxShort)
                            XMORE(" Short");
                        else if (rtctResult.un.fe_result.isRxOpen)
                            XMORE(" Open");
                        else if (rtctResult.un.fe_result.isRxMismatch)
                            XMORE(" ImpedanceMis");
                        else if (rtctResult.un.fe_result.isRxLinedriver)
                            XMORE(" LineDriver");
                        else
                            XMORE(" Normal");
                    }
                    else if (i == 1)
                    {
                        XMORE("  %4d.%02d    |", rtctResult.un.fe_result.txLen/100, rtctResult.un.fe_result.txLen%100);
                        if (rtctResult.un.fe_result.isTxShort)
                            XMORE(" Short");
                        else if (rtctResult.un.fe_result.isTxOpen)
                            XMORE(" Open");
                        else if (rtctResult.un.fe_result.isTxMismatch)
                            XMORE(" ImpedanceMis");
                        else if (rtctResult.un.fe_result.isTxLinedriver)
                            XMORE(" LineDriver");
                        else
                            XMORE(" Normal");
                    }
                    else
                        XMORE("     N/A     | Not Supported ");
                }
                else if (PORT_SPEED_1000M == rtctResult.linkType)
                {
                    if (i == 0)
                    {
                        channelLenM = rtctResult.un.ge_result.channelALen / 100;
                        channelLenCM = rtctResult.un.ge_result.channelALen % 100;
                        channelOpen = rtctResult.un.ge_result.channelAOpen;
                        channelShort = rtctResult.un.ge_result.channelAShort;
                        channelMismatch = rtctResult.un.ge_result.channelAMismatch;
                        lineDriver = rtctResult.un.ge_result.channelALinedriver;
                    }
                    else if (i == 1)
                    {
                        channelLenM = rtctResult.un.ge_result.channelBLen / 100;
                        channelLenCM = rtctResult.un.ge_result.channelBLen % 100;
                        channelOpen = rtctResult.un.ge_result.channelBOpen;
                        channelShort = rtctResult.un.ge_result.channelBShort;
                        channelMismatch = rtctResult.un.ge_result.channelBMismatch;
                        lineDriver = rtctResult.un.ge_result.channelBLinedriver;
                    }
                    else if (i == 2)
                    {
                        channelLenM = rtctResult.un.ge_result.channelCLen / 100;
                        channelLenCM = rtctResult.un.ge_result.channelCLen % 100;
                        channelOpen = rtctResult.un.ge_result.channelCOpen;
                        channelShort = rtctResult.un.ge_result.channelCShort;
                        channelMismatch = rtctResult.un.ge_result.channelCMismatch;
                        lineDriver = rtctResult.un.ge_result.channelCLinedriver;
                    }
                    else
                    {
                        channelLenM = rtctResult.un.ge_result.channelDLen / 100;
                        channelLenCM = rtctResult.un.ge_result.channelDLen % 100;
                        channelOpen = rtctResult.un.ge_result.channelDOpen;
                        channelShort = rtctResult.un.ge_result.channelDShort;
                        channelMismatch = rtctResult.un.ge_result.channelDMismatch;
                        lineDriver = rtctResult.un.ge_result.channelDLinedriver;
                    }

                    XMORE("  %4d.%02d    |", channelLenM, channelLenCM);
                    if (channelShort)
                        XMORE(" Short");
                    else if (channelOpen)
                        XMORE(" Open");
                    else if (channelMismatch)
                        XMORE(" ImpedanceMis");
                    else if (lineDriver)
                        XMORE(" LineDriver");
                    else
                        XMORE(" Normal");
                }
                XMORE("\n");
            }
        }
        XMORE("\n");
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_OPTICALMOD_STATUS

int32 cmd_cablediag_fiber_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char                      port_str[8];
    sys_logic_port_t          logicPort;
    sys_logic_portmask_t      logicPortmask;
    sys_port_media_t          portMedia;
    sys_optical_diagStatus_t  diagStatus;
    sys_optical_diagMeasure_t diagMeasure;
    sys_fiber_detect_status_t detect_status;

    osal_memset(&portMedia, 0, sizeof(sys_port_media_t));
    osal_memset(&logicPortmask, 0, sizeof(sys_logic_portmask_t));

    CMD_PORTLIST_GET(0, &logicPortmask);
#ifdef CONFIG_SYS_FIBER
    XMORE(" Port   | Temperature  | Voltage     | Current      | Output power | Input power | OE-Present | LOS \n");
    XMORE("        | [C]          | [Volt]      | [mA]         | [mWatt]      | [mWatt]     |            |     \n");
    XMORE("===================================================================================================\n");
#else
    XMORE(" Port   | Temperature  | Voltage     | Current      | Output power | Input power \n");
    XMORE("        | [C]          | [Volt]      | [mA]         | [mWatt]      | [mWatt]     \n");
    XMORE("================================================================================\n");
#endif
    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {

        SYS_MEM_CLEAR(port_str);
        SYS_MEM_CLEAR(diagStatus);
        SYS_MEM_CLEAR(diagMeasure);

        SYS_ERR_CHK(sal_port_linkMedia_get(logicPort, &portMedia));

        SYS_ERR_CHK(LP2STR(logicPort, port_str));
        if (PORT_MEDIA_FIBER != portMedia)
        {
            continue;
        }
        XMORE(" %4s   |", port_str);

        if (SYS_ERR_OK == sal_optical_diagMeasure_get(logicPort, &diagMeasure))
        {
            if (DISABLED == diagMeasure.enable)
            {
                XMORE(" %-12s |", " N/S");
                XMORE(" %-11s |", " N/S");
                XMORE(" %-12s |", " N/S");
                XMORE(" %-12s |", " N/S");
                XMORE(" %-11s |", " N/S");
            }
            else if (SYS_ERR_OK == sal_optical_diagStatus_get(logicPort, &diagStatus))
            {
                XMORE(" %-6.2f ", diagMeasure.temp * 0.00391f);
                switch (diagStatus.temp)
                {
                    case SYS_OPTICAL_DIAG_STATUS_UNKNOWN:
                        XMORE("(N/S) |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_NORMAL:
                        XMORE("(OK)  |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_WARNING:
                        XMORE("(W)   |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_ALARM:
                        XMORE("(E)   |");
                        break;

                    default:
                        XMORE("(N/A) |");
                        break;
                }

                XMORE(" %-5.2f ", diagMeasure.volt * 0.0001f);
                switch (diagStatus.volt)
                {
                    case SYS_OPTICAL_DIAG_STATUS_UNKNOWN:
                        XMORE("(N/S) |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_NORMAL:
                        XMORE("(OK)  |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_WARNING:
                        XMORE("(W)   |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_ALARM:
                        XMORE("(E)   |");
                        break;

                    default:
                        XMORE("(N/A) |");
                        break;
                }

                XMORE(" %-6.2f ", diagMeasure.bias * 0.002f);
                switch (diagStatus.bias)
                {
                    case SYS_OPTICAL_DIAG_STATUS_UNKNOWN:
                        XMORE("(N/S) |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_NORMAL:
                        XMORE("(OK)  |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_WARNING:
                        XMORE("(W)   |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_ALARM:
                        XMORE("(E)   |");
                        break;

                    default:
                        XMORE("(N/A) |");
                        break;
                }

                XMORE(" %-5.2f ", diagMeasure.txPwr * 0.0001f);
                switch (diagStatus.txPwr)
                {
                    case SYS_OPTICAL_DIAG_STATUS_UNKNOWN:
                        XMORE("(N/S)  |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_NORMAL:
                        XMORE("(OK)   |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_WARNING:
                        XMORE("(W)    |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_ALARM:
                        XMORE("(E)    |");
                        break;

                    default:
                        XMORE("(N/A)  |");
                        break;
                }

                XMORE(" %-5.2f ", diagMeasure.rxPwr * 0.0001f);
                switch (diagStatus.rxPwr)
                {
                    case SYS_OPTICAL_DIAG_STATUS_UNKNOWN:
                        XMORE("(N/S) |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_NORMAL:
                        XMORE("(OK)  |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_WARNING:
                        XMORE("(W)   |");
                        break;

                    case SYS_OPTICAL_DIAG_STATUS_ALARM:
                        XMORE("(E)   |");
                        break;

                    default:
                        XMORE("(N/A) |");
                        break;
                }
            }
            else
            {
                XMORE(" %-6.2f       | %-5.2f       | %-6.2f       | %-5.2f        | %-5.2f       |",
                      diagMeasure.temp * 0.00391f,
                      diagMeasure.volt * 0.0001f,
                      diagMeasure.bias * 0.002f,
                      diagMeasure.txPwr * 0.0001f,
                      diagMeasure.rxPwr * 0.0001f);
            }
        }
        else
        {
            XMORE(" %-12s |", "N/A");
            XMORE(" %-11s |", "N/A");
            XMORE(" %-12s |", "N/A");
            XMORE(" %-12s |", "N/A");
            XMORE(" %-11s |", "N/A");
        }

#ifdef CONFIG_SYS_FIBER
        if (SYS_ERR_OK == sal_fiber_oeStatus_get(logicPort, &detect_status))
        {
            if (FIBER_DETECT_STATUS_OFF == detect_status)
                XMORE(" Remove  ");
            else if (FIBER_DETECT_STATUS_ON == detect_status)
                XMORE(" Insert     ");
            else
                XMORE(" N/S ");
        }
        else
        {
            XMORE(" N/A ");
        }
        XMORE("|");

        if (SYS_ERR_OK == sal_fiber_losStatus_get(logicPort, &detect_status))
        {
            if (FIBER_DETECT_STATUS_OFF == detect_status)
                XMORE(" Loss ");
            else if (FIBER_DETECT_STATUS_ON == detect_status)
                XMORE(" Normal ");
            else
                XMORE(" N/S ");
        }
        else
        {
            XMORE(" N/A ");
        }
#endif

        XMORE("\n");
    }

    XMORE("\n");
    XMORE(" Temp         - Internally measured transceiver temperature\n");
    XMORE(" Voltage      - Internally measured supply voltage\n");
    XMORE(" Current      - Measured TX bias current\n");
    XMORE(" Output Power - Measured TX output power in milliWatts\n");
    XMORE(" Input Power  - Measured RX received power in milliWatts\n");
#ifdef CONFIG_SYS_FIBER
    XMORE(" OE-Present   - SFP Presetn or Not Present\n");
    XMORE(" LOS          - Loss of signal\n");
#endif
    XMORE(" N/A - Not Available, N/S - Not Supported, W - Warning, E - Error\n");

    {
        sys_optical_trxCapacity_t cap;
        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
        {
            sal_optical_trxCapacity_get(logicPort, &cap);
        }
    }

    return SYS_ERR_OK;
}

int cmd_cablediag_fiber_module_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t      logicPortmask;

    char str[SYS_BUF128_LEN];
    
    sys_logic_port_t lp = 0;
    sys_port_media_t media;
    
    uint32 bitrate;
    uint32 wavelength;
    sys_optical_trxType_t trxType;
    sys_optical_vendorInfo_t vendor;
    sys_optical_trxCapacity_t trxCapacity;
    sys_optical_connectType_t connectType;
    sys_optical_diagMeasure_t diagMeasure;
    sys_fiber_detect_status_t detectStatus;

    osal_memset(&logicPortmask, 0, sizeof(sys_logic_portmask_t));

    CMD_PORTLIST_GET(0, &logicPortmask);
    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, logicPortmask)
    {
       media = PORT_MEDIA_COPPER;
   
       sal_port_linkMedia_get(lp, &media);

       if (PORT_MEDIA_FIBER != media)
       {
           return SYS_ERR_PORT_NOT_FIBER;
       }

       SYS_MEM_CLEAR(str);
       LP2STR(lp, str);
       XMORE(" Port                    : %s\n",str);
   
       if (SYS_ERR_OK == sal_fiber_losStatus_get(lp, &detectStatus))
       {
   
          switch (detectStatus)
           {
               case FIBER_DETECT_STATUS_OFF:
                  
                   XMORE(" OE Present              : Remove\n");
                   break;
   
               case FIBER_DETECT_STATUS_ON:
                  
                   XMORE(" OE Present              : Insert\n");
                   break;
   
               default:
                 
                   XMORE(" OE Present              : N/S\n");
                   break;
           }
       }

       if (SYS_ERR_OK == sal_fiber_oeStatus_get(lp, &detectStatus))
       {
   
           switch (detectStatus)
           {
               case FIBER_DETECT_STATUS_OFF:
                
                   XMORE(" Loss of Signal          : Loss\n");
                   break;
   
               case FIBER_DETECT_STATUS_ON:
                
                   XMORE(" Loss of Signal          : Nomarl\n");
                   break;
   
               default:
                
                   XMORE(" Loss of Signal          : N/S\n");
                   break;
           }
       }

       if (SYS_ERR_OK == sal_optical_trxType_get(lp, &trxType))
       {

           XMORE(" Transceiver Type        : %s\n",text_optical_trxType[trxType].text);
       }
       else
       {

          XMORE(" Transceiver Type        : Unknown\n");
       }

       if (SYS_ERR_OK == sal_optical_connectType_get(lp, &connectType))
       {
           XMORE(" Connector Type          : %s\n",text_optical_connectType[connectType].text);
       }
       else
       {
           XMORE(" Connector Type          :  Unknown\n");
       }

       if (SYS_ERR_OK == sal_optical_trxCapacity_get(lp, &trxCapacity))
       {
          XMORE(" Ethernet Compliance Code: %s\n",text_optical_ethCompliance[trxCapacity.ethCompliance].text);
          XMORE(" Transmission Media      : %s\n",text_optical_trxMedia[trxCapacity.txMedia].text);
       }
       else
       {
          XMORE(" Ethernet Compliance Code: Unknown\n");
          XMORE(" Transmission Media      : Unknown\n");
       }

       if (SYS_ERR_OK == sal_optical_wavelength_get(lp, &wavelength))
       {
           if (wavelength)
           {
               XMORE(" Wavelength              : %u",wavelength);
               XMORE(" nm\n");
           }
           else
           {
               XMORE(" Wavelength              : N/S\n");
           }
       }
       else
       {
           XMORE(" Wavelength              : N/S\n");

       }

       if (SYS_ERR_OK == sal_optical_bitRateNominal_get(lp, &bitrate))
       {
           if (bitrate)
           {
               XMORE(" Bitrate                 : %u  Mbps\n",bitrate);
           }
           else
           {
               XMORE(" Bitrate                 : N/S\n");
           }
       }
       else
       {
          XMORE(" Bitrate                 : N/A\n");
       }

       SYS_MEM_CLEAR(vendor);
   
       if (SYS_ERR_OK == sal_optical_vendorInfo_get(lp, &vendor))
       {
           if (vendor.oui)
           {
               XMORE(" Vendor OUI              : %02x-%02x-%02x\n",
                        (vendor.oui >> 16) & 0xFF,
                        (vendor.oui >> 8) & 0xFF,
                        vendor.oui & 0xFF);
           }
           else
           {
               XMORE(" Vendor OUI              : N/S\n");
           }
   
           if (strlen(vendor.name))
           {
               XMORE(" Vendor Name             : %s\n",vendor.name);
           }
           else
           {
               XMORE(" Vendor Name             : N/S\n");
           }
   
           if (strlen(vendor.pn))
           {
               XMORE(" Vendor PN               : %s\n",vendor.pn);
           }
           else
           {
               XMORE(" Vendor PN               : N/S\n");
           }
   
           if (strlen(vendor.rev))
           {
               XMORE(" Vendor Revision         : %s\n",vendor.rev);
           }
           else
           {
               XMORE(" Vendor Revision         : N/S\n");
           }
   
           if (strlen(vendor.sn))
           {
               XMORE(" Vendor SN               : %s\n",vendor.sn);
           }
           else
           {
               XMORE(" Vendor SN               : N/S\n");
           }
   
           if (strlen(vendor.date))
           {
               XMORE(" Date Code               : %s\n",vendor.date);
           }
           else
           {
               XMORE(" Date Code               : N/S\n");
           }
       }
       else
       {
           XMORE(" Vendor OUI              : N/A\n");
           XMORE(" Vendor Name             : N/A\n");
           XMORE(" Vendor PN               : N/A\n");
           XMORE(" Vendor Revision         : N/A\n");
           XMORE(" Vendor SN               : N/A\n");
           XMORE(" Date Code               : N/A\n");
       }

       SYS_MEM_CLEAR(diagMeasure);
   
       if (SYS_ERR_OK == sal_optical_diagMeasure_get(lp, &diagMeasure))
       {
           if (DISABLED == diagMeasure.enable)
           {
              XMORE(" Temperature (C)         : N/S\n");
              XMORE(" Voltage (V)             : N/S\n");
              XMORE(" Current (mA)            : N/S\n");
              XMORE(" Output Power (mW)       : N/S\n");
              XMORE(" Input Power (mW)        : N/S\n");
           }
           else
           {
             
               XMORE(" Temperature (C)         :%6.2f\n",diagMeasure.temp * 0.00391f);

               XMORE(" Voltage (V)             :%5.2f\n",diagMeasure.volt * 0.0001f);

               XMORE(" Current (mA)            :%5.2f\n",diagMeasure.bias * 0.002f);

               XMORE(" Output Power (mW)       :%5.2f\n",diagMeasure.txPwr * 0.0001f);

               XMORE(" Input Power (mW)        :%5.2f\n",diagMeasure.rxPwr* 0.0001f);
           }
       }
       else
       {
           XMORE(" Temperature (C)         : N/A\n");
           XMORE(" Voltage (V)             : N/A\n");
           XMORE(" Current (mA)            : N/A\n");
           XMORE(" Output Power (mW)       : N/A\n");
           XMORE(" Input Power (mW)        : N/A\n");
       }
       XMORE("\n");
    }

   return SYS_ERR_OK;

}

#endif

