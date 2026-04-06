
#include <common/sys_l2.h>
#include <libsal/sal_port.h>
#include <libsal/sal_optical.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_mib.h>
#include <libsal/sal_eee.h>
#include <libcmd/cmd.h>
#include <libsal/sal_mib_private.h>

const sys_text_t text_port_type[] =
{
    {       "Fast Ethernet"       },
    {      "Gigabit Ethernet"      },
    {      "2.5 Gigabit Ethernet"      },
    {        "Ten Gigabit Ethernet"     },

    {        "EtherChannel"  },
    {       "Fast Ethernet"       },
    {       "Gigabit Ethernet"       },
    {        ""       },

};

int32 cmd_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    sys_enable_t flowCtrl;
    sys_enable_t flowCtrlStatus;
    sys_enable_t flowCtrlAuto, backPressure;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_port_media_t media;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_port_linkStatus_t linkStatus;
    uint32 bitrate = 0;

    uint64 inPkts = 0;
    uint64 inOctets = 0;
    uint64 inBcasts = 0;
    uint64 inMcasts = 0;
    uint64 inRunts = 0;
    uint64 inGiants = 0;
    uint64 inThrottles = 0;
    uint64 inCRCs = 0;
    uint64 inFrames = 0;
    uint64 inErrs = 0;
    uint64 inPause = 0;
    uint64 inDribble = 0;
    uint64 inPktsRate = 0;
    uint64 inTypeRate = 0;
    uint64 inUcasts = 0;
    uint64 inDropPkts = 0;

    uint64 outPkts = 0;
    uint64 outOctets = 0;
    uint64 outUnderruns = 0;
    uint64 outErrs = 0;
    uint64 outCollisions = 0;
    uint64 outBabbles = 0;
    uint64 outLateCollisions = 0;
    uint64 outDeffered = 0;
    uint64 outPause = 0;
    uint64 outPktsRate = 0;
    uint64 outTypeRate = 0;
    uint64 outBcasts = 0;
    uint64 outMcasts = 0;
    uint64 outUcasts = 0;

    CMD_PORTLIST_GET(0, &lpm);
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        inPkts = inOctets = inBcasts = inMcasts = inRunts = inGiants = inThrottles = inCRCs = inDropPkts = 0;
        inFrames = inErrs = inPause = inDribble = inUcasts = outBcasts = outMcasts = outUcasts = 0;
        outPkts = outOctets = outUnderruns = outErrs = outCollisions = 0;
        outBabbles = outLateCollisions = outDeffered = outPause = 0;
        SYS_MEM_CLEAR(portStr);
        SYS_MEM_CLEAR(speed);
        SYS_MEM_CLEAR(duplex);

        LP2FULLSTR(lp, portStr);
        CMD_ERR_CONTINUE_PORT(sal_port_linkStatus_get(lp, &linkStatus), lp);
        CMD_ERR_CONTINUE_PORT(sal_port_linkMedia_get(lp, &media), lp);
        CMD_ERR_CONTINUE_PORT(sal_port_speed_get(lp, &speed), lp);
        CMD_ERR_CONTINUE_PORT(sal_port_duplex_get(lp, &duplex), lp);
        CMD_ERR_CONTINUE_PORT(sal_port_flowCtrl_get(lp, &flowCtrl, &flowCtrlStatus), lp);
        CMD_ERR_CONTINUE_PORT(sal_port_flowCtrlAuto_get(lp, &flowCtrlAuto), lp);
        CMD_ERR_CONTINUE_PORT(sal_port_backpressureEnable_get(lp, &backPressure), lp);

        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsPkts_get(lp, &inPkts), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsBcastPkts_get(lp, &inBcasts), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsMcastPkts_get(lp, &inMcasts), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_ifInUcastPkts_get(lp, &inUcasts), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsOctets_get(lp, &inOctets), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsUnderSizePkts_get(lp, &inRunts), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsOverSizePkts_get(lp, &inDribble), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsDropEvents_get(lp, &inThrottles), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsFCSErrors_get(lp, &inCRCs), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3_statsAlignmentErrors_get(lp, &inFrames), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3InPauseFrames_get(lp, &inPause), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsFrameTooLongs_get(lp, &inGiants), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot1dTpPortInDiscards_get(lp, &inDropPkts), lp);
        inErrs = inRunts + inGiants + inThrottles + inCRCs + inFrames;
        
        CMD_ERR_CONTINUE_PORT(sal_mib_last_5_rate_get(lp, TYPE_IN_PKT_RATE, &inPktsRate), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_last_5_rate_get(lp, TYPE_IN_BYTE_RATE, &inTypeRate), lp);
        
        CMD_ERR_CONTINUE_PORT(sal_mib_ifOutUcastPkts_get(lp, &outUcasts), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_ifOutBcastPkts_get(lp, &outBcasts), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_ifOutMcastPkts_get(lp, &outMcasts), lp);
        outPkts = outUcasts + outBcasts + outMcasts;
        CMD_ERR_CONTINUE_PORT(sal_mib_ifOutOctets_get(lp, &outOctets), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_ifOutDiscards_get(lp, &outUnderruns), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsExcessiveCollisions_get(lp, &outCollisions), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsJabbers_get(lp, &outBabbles), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsLateCollisions_get(lp, &outLateCollisions), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsDeferredTransmissions_get(lp, &outDeffered), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3OutPauseFrames_get(lp, &outPause), lp);
        outErrs = outUnderruns + outCollisions + outBabbles + outLateCollisions +\
                  outDeffered;
        
        CMD_ERR_CONTINUE_PORT(sal_mib_last_5_rate_get(lp, TYPE_OUT_PKT_RATE, &outPktsRate), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_last_5_rate_get(lp, TYPE_OUT_BYTE_RATE, &outTypeRate), lp);

        XMORE("%s is %s\n", portStr, (PORT_LINKUP == linkStatus) ? "up" : "down");
        XMORE("  Hardware is %s\n", text_port_type[LP2TYPE(lp)].text);

        if (ENABLED == duplex.autoDuplex)
            XMORE("  Auto-duplex, ");
        else if (SYS_PORT_DUPLEX_FULL == duplex.ability)
            XMORE("  Full-duplex, ");
        else
            XMORE("  Half-duplex, ");
        
        if (ENABLED == speed.autoSpeed)
            XMORE("Auto-speed, ");
        else if (SYS_PORT_SPEED_10M == speed.ability)
            XMORE("10Mb/s, ");
        else if (SYS_PORT_SPEED_100M == speed.ability)
            XMORE("100Mb/s, ");
        else if (SYS_PORT_SPEED_1000M == speed.ability)
            XMORE("1000Mb/s, ");
        else if (SYS_PORT_SPEED_10G == speed.ability)
        {
            
            if ((!IS_TRKLP(lp))&&
                (SYS_ERR_OK == sal_optical_bitRateNominal_get(lp, &bitrate))&&
                ((bitrate != 0)&&(bitrate < 10000)))
                XMORE("1000Mb/s, ");
            else
                XMORE("10Gb/s, ");
        }
        else if (SYS_PORT_SPEED_10G_DAC_50 == speed.ability)
        {
            XMORE("10Gb/s DAC 50 CM, ");
        }
        else if (SYS_PORT_SPEED_10G_DAC_100 == speed.ability)
        {
            XMORE("10Gb/s DAC 100 CM, ");
        }
        else if (SYS_PORT_SPEED_10G_DAC_300 == speed.ability)
        {
            XMORE("10Gb/s DAC 300 CM, ");
        }
        else if (SYS_PORT_SPEED_10G_DAC_500 == speed.ability)
        {
            XMORE("10Gb/s DAC 500 CM, ");
        }

        if (PORT_MEDIA_COPPER == media)
            XMORE("media type is Copper\n");
        else
            XMORE("media type is Fiber\n");

        #if 0 
        if(ENABLED == flowCtrlAuto)
            XMORE("  flow-control is auto\n");
        else
            XMORE("  flow-control is %s\n", (ENABLED == flowCtrl) ? "on" : "off");
        #endif
        if(ENABLED == flowCtrlAuto)
        {
            XMORE("  flow-control is auto, status is %s\n", (ENABLED == flowCtrlStatus) ? "on" : "off");
        }
        else
        {   
            if(flowCtrl == ENABLED)
            {
                XMORE("  flow-control is %s, status is %s\n", (ENABLED == flowCtrl) ? "enable" : "disable", (ENABLED == flowCtrlStatus) ? "on" : "off");
            }
        }

        XMORE("  back-pressure is %s\n", (ENABLED == backPressure) ? "enabled" : "disabled");

        XMORE("     %llu packets input, %llu bytes, %llu discarded packets\n", inPkts, inOctets, inDropPkts);
        XMORE("     %llu broadcasts  %llu multicasts %llu unicasts\n", inBcasts, inMcasts, inUcasts);
        XMORE("     %llu runts, %llu giants, %llu discarded event packets\n", inRunts, inGiants, inThrottles);
        XMORE("     %llu input errors, %llu CRC, %llu frame\n", inErrs, inCRCs, inFrames);
        XMORE("     %llu multicast, %llu pause input\n", inMcasts, inPause);
        XMORE("     %llu input packets with dribble condition detected\n", inDribble);
        XMORE("     last 5 minutes input rate %llu bits/sec, %llu packets/sec\n", (inTypeRate*8), inPktsRate);

        XMORE("\n");

        XMORE("     %llu packets output, %llu bytes, %llu discarded packets\n", outPkts, outOctets, outUnderruns);
        XMORE("     %llu broadcasts  %llu multicasts %llu unicasts\n", outBcasts, outMcasts, outUcasts);
        XMORE("     %llu output errors, %llu collisions\n", outErrs, outCollisions);
        XMORE("     %llu babbles, %llu late collision, %llu deferred\n", outBabbles, outLateCollisions, outDeffered);
        XMORE("     %llu PAUSE output\n", outPause);
        XMORE("     last 5 minutes output rate %llu bits/sec, %llu packets/sec\n", (outTypeRate*8), outPktsRate);
    }

    return SYS_ERR_OK;
}

int32 cmd_port_status_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_port_media_t media;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    sys_port_linkStatus_t linkStatus;
    sys_port_speedDuplex_t speedDuplex;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_enable_t enable;
    sys_vid_t pvid = 0;
    char port_str[CAPA_PORT_STR_LEN];
#ifdef CONFIG_SYS_PORT_DESCRIPTION
    char descp[CAPA_PORTDESC_STR_LEN + 1];
#endif

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE("Port  Name                 Status      Vlan  Duplex  Speed           Type \n");
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        osal_memset(port_str, 0, sizeof(port_str));
        SYS_ERR_CHK(LP2STR(logicPort, port_str));
        XMORE("%-4s  ", port_str);

#ifdef CONFIG_SYS_PORT_DESCRIPTION
        SYS_MEM_CLEAR(descp);
        SYS_ERR_CHK(sal_port_descp_get(logicPort, descp));
        XMORE("%-20s ", descp);
#endif
        SYS_ERR_CHK(sal_port_enable_get(logicPort, &enable));
        SYS_ERR_CHK(sal_port_linkStatus_get(logicPort, &linkStatus));
        if (DISABLED == enable)
            XMORE("disable     ");
        else if (ENABLED == enable && PORT_LINKUP == linkStatus)
            XMORE("connected   ");
        else
            XMORE("notconnect  ");

        SYS_ERR_CHK(sal_vlan_portOperPvid_get(logicPort, &pvid));
        XMORE("%-4d  ", pvid);

        osal_memset(&speed, 0, sizeof(speed));
        osal_memset(&duplex, 0, sizeof(duplex));
        SYS_ERR_CHK(sal_port_speed_get(logicPort, &speed));
        SYS_ERR_CHK(sal_port_duplex_get(logicPort, &duplex));

        if (PORT_LINKUP == linkStatus)
        {
            osal_memset(&speedDuplex, 0, sizeof(sys_port_speedDuplex_t));
            sal_port_speedDuplex_get(logicPort, &speedDuplex);

            if (ENABLED == duplex.autoDuplex)
            {
                if (PORT_FULL_DUPLEX == speedDuplex.duplex)
                    XMORE("a-full  ");
                else if (PORT_HALF_DUPLEX == speedDuplex.duplex)
                    XMORE("a-half  ");
            }
            else
            {
                if (PORT_FULL_DUPLEX == speedDuplex.duplex)
                    XMORE("full    ");
                else if (PORT_HALF_DUPLEX == speedDuplex.duplex)
                    XMORE("half    ");
            }

            if (ENABLED == speed.autoSpeed)
            {
                if (PORT_SPEED_10M == speedDuplex.speed)
                    XMORE("a-10M    ");
                else if (PORT_SPEED_100M == speedDuplex.speed)
                    XMORE("a-100M   ");
                else if (PORT_SPEED_1000M == speedDuplex.speed)
                    XMORE("a-1000M  ");
                else if (PORT_SPEED_2_5G == speedDuplex.speed)
                    XMORE("a-2500M  ");
            }
            else
            {
                if (PORT_SPEED_10M == speedDuplex.speed)
                    XMORE("10M      ");
                else if (PORT_SPEED_100M == speedDuplex.speed)
                    XMORE("100M     ");
                else if (PORT_SPEED_1000M == speedDuplex.speed)
                    XMORE("1000M    ");
                else if (PORT_SPEED_2_5G == speedDuplex.speed)
                    XMORE("2500M    ");
                else if (PORT_SPEED_10G == speedDuplex.speed)
                    XMORE("10G      ");
            }
        }
        else
        {
            if (ENABLED == duplex.autoDuplex)
            {
                XMORE("auto    ");
            }
            else
            {
                if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                    XMORE("full    ");
                else if (SYS_PORT_DUPLEX_HALF == duplex.ability)
                    XMORE("half    ");
            }

            if (ENABLED == speed.autoSpeed)
            {
                XMORE("auto     ");
            }
            else
            {
                if (SYS_PORT_SPEED_10M == speed.ability)
                    XMORE("10M      ");
                else if (SYS_PORT_SPEED_100M == speed.ability)
                    XMORE("100M     ");
                else if (SYS_PORT_SPEED_1000M == speed.ability)
                    XMORE("1000M    ");
                else if (SYS_PORT_SPEED_10G == speed.ability)
                    XMORE("10G      ");
                else if (SYS_PORT_SPEED_10G_DAC_50 == speed.ability)
                    XMORE("10G DAC 50");
                else if (SYS_PORT_SPEED_10G_DAC_100 == speed.ability)
                    XMORE("10G DAC 100");
                else if (SYS_PORT_SPEED_10G_DAC_300 == speed.ability)
                    XMORE("10G DAC 300");
                else if (SYS_PORT_SPEED_10G_DAC_500 == speed.ability)
                    XMORE("10G DAC 500");           
            }
        }

        SYS_ERR_CHK(sal_port_linkMedia_get(logicPort, &media));
        if (PORT_MEDIA_COPPER == media)
            XMORE("    Copper");
        else
            XMORE("    Fiber");

        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_port_status_brief(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int rc = 0;
    sys_port_media_t media;
    sys_logic_port_t logicPort;
    sys_port_linkStatus_t linkStatus;
    sys_port_speedDuplex_t speedDuplex;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_enable_t enable;
    sys_vid_t pvid = 0;
    vector printor = NULL;
    char port_str[CAPA_PORT_STR_LEN];
    char status_str[16];
    char speed_str[16];
    char duplex_str[16];
    char type_str[16];
#ifdef CONFIG_SYS_PORT_DESCRIPTION
    char descp[CAPA_PORTDESC_STR_LEN + 1];
#endif
    char  cInputChar = 0;
    int  displayUplink = 0;
    int  untagLine;
    char descp_buf[2][128];

    if (NULL != osal_strstr(cmd->string, "connected"))
    {
        displayUplink = 1;
    }

    add_value_to_new_line_printor_bylen (PRINT_STRING, "Port", &printor, 8);
    add_value_to_last_line_printor_bylen(PRINT_STRING, "Name", &printor, 18);
    add_value_to_last_line_printor_bylen(PRINT_STRING, "Status", &printor, 12);
    add_value_to_last_line_printor_bylen(PRINT_STRING, "Pvid", &printor, 7);
    add_value_to_last_line_printor_bylen(PRINT_STRING, "Duplex", &printor, 10);
    add_value_to_last_line_printor_bylen(PRINT_STRING, "Speed", &printor, 15);
    add_value_to_last_line_printor_bylen(PRINT_STRING, "Type", &printor, 8);

    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        rc = sal_port_linkStatus_get(logicPort, &linkStatus);
        if( rc != SYS_ERR_OK)
        {   
            return free_printor_vector(&printor);
        }
        
        if((1 == displayUplink) && (PORT_LINKDOWN == linkStatus))
        {
            continue;
        }

        osal_memset(port_str, 0, sizeof(port_str));
        rc = LP2STR(logicPort, port_str);
        if( rc != SYS_ERR_OK)
        {   
            return free_printor_vector(&printor);
        }

    #ifdef CONFIG_SYS_PORT_DESCRIPTION
        SYS_MEM_CLEAR(descp);
        rc = sal_port_descp_get(logicPort, descp);
        if( rc != SYS_ERR_OK)
        {   
            return free_printor_vector(&printor);
        }
        SYS_MEM_CLEAR(descp_buf);
        untagLine = 0;
        if(strlen(descp) > 0)
        {
            untagLine = transLongStrToStrArray(descp, descp_buf, 16);
        }
    #endif
        rc = sal_port_enable_get(logicPort, &enable);
        if( rc != SYS_ERR_OK)
        {   
            return free_printor_vector(&printor);
        }
        SYS_MEM_CLEAR(status_str);
        if (DISABLED == enable)
            osal_sprintf(status_str, "disable");
        else if (ENABLED == enable && PORT_LINKUP == linkStatus)
            osal_sprintf(status_str, "connect");
        else
            osal_sprintf(status_str, "notconnect");

        rc = sal_vlan_portOperPvid_get(logicPort, &pvid);
        if( rc != SYS_ERR_OK)
        {   
            return free_printor_vector(&printor);
        }

        osal_memset(&speed, 0, sizeof(speed));
        osal_memset(&duplex, 0, sizeof(duplex));
        rc = sal_port_speed_get(logicPort, &speed);
        if( rc != SYS_ERR_OK)
        {   
            return free_printor_vector(&printor);
        }
        rc = sal_port_duplex_get(logicPort, &duplex);
        if( rc != SYS_ERR_OK)
        {   
            return free_printor_vector(&printor);
        }
        SYS_MEM_CLEAR(duplex_str);
        SYS_MEM_CLEAR(speed_str);
        if (PORT_LINKUP == linkStatus)
        {
            osal_memset(&speedDuplex, 0, sizeof(sys_port_speedDuplex_t));
            rc = sal_port_speedDuplex_get(logicPort, &speedDuplex);
            if( rc != SYS_ERR_OK)
            {   
                return free_printor_vector(&printor);
            }

            if (ENABLED == duplex.autoDuplex)
            {
                if (PORT_FULL_DUPLEX == speedDuplex.duplex)
                    osal_sprintf(duplex_str, "a-full");
                else if (PORT_HALF_DUPLEX == speedDuplex.duplex)
                    osal_sprintf(duplex_str, "a-half");
                else
                    osal_sprintf(duplex_str, "unknown");
            }
            else
            {
                if (PORT_FULL_DUPLEX == speedDuplex.duplex)
                    osal_sprintf(duplex_str, "full");
                else if (PORT_HALF_DUPLEX == speedDuplex.duplex)
                    osal_sprintf(duplex_str, "half");
                else
                    osal_sprintf(duplex_str, "unknown");
            }

            if (ENABLED == speed.autoSpeed)
            {
               
                if (PORT_SPEED_10M == speedDuplex.speed)
                    osal_sprintf(speed_str, "a-10M");
                else if (PORT_SPEED_100M == speedDuplex.speed)
                    osal_sprintf(speed_str, "a-100M");
                else if (PORT_SPEED_1000M == speedDuplex.speed)
                    osal_sprintf(speed_str, "a-1000M");
                else if (PORT_SPEED_10G == speedDuplex.speed)
                    osal_sprintf(speed_str, "a-10G");
                else
                    osal_sprintf(speed_str, "unknown");
            }
            else
            {
                if (PORT_SPEED_10M == speedDuplex.speed)
                    osal_sprintf(speed_str, "10M");
                else if (PORT_SPEED_100M == speedDuplex.speed)
                    osal_sprintf(speed_str, "100M");
                else if (PORT_SPEED_1000M == speedDuplex.speed)
                    osal_sprintf(speed_str, "1000M");
                else if (PORT_SPEED_10G == speedDuplex.speed)
                    osal_sprintf(speed_str, "10G");
                else
                    osal_sprintf(speed_str, "unknown");
            }
        }
        else
        {
            if (ENABLED == duplex.autoDuplex)
            {
                osal_sprintf(duplex_str, "auto");
            }
            else
            {
                if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                    osal_sprintf(duplex_str, "full");
                else if (SYS_PORT_DUPLEX_HALF == duplex.ability)
                    osal_sprintf(duplex_str, "half");
                else
                    osal_sprintf(duplex_str, "unknown");
            }

            if (ENABLED == speed.autoSpeed)
            {
                osal_sprintf(speed_str, "auto");
            }
            else
            {
                if (SYS_PORT_SPEED_10M == speed.ability)
                    osal_sprintf(speed_str, "10M");
                else if (SYS_PORT_SPEED_100M == speed.ability)
                    osal_sprintf(speed_str, "100M");
                else if (SYS_PORT_SPEED_1000M == speed.ability)
                    osal_sprintf(speed_str, "1000M");
                else if (SYS_PORT_SPEED_10G == speed.ability)
                    osal_sprintf(speed_str, "10G");
                else if (SYS_PORT_SPEED_10G_DAC_50 == speed.ability)
                    osal_sprintf(speed_str, "10G DAC 50");
                else if (SYS_PORT_SPEED_10G_DAC_100 == speed.ability)
                    osal_sprintf(speed_str, "10G DAC 100");
                else if (SYS_PORT_SPEED_10G_DAC_300 == speed.ability)
                    osal_sprintf(speed_str, "10G DAC 300");
                else if (SYS_PORT_SPEED_10G_DAC_500 == speed.ability) 
                    osal_sprintf(speed_str, "10G DAC 500");
                else
                    osal_sprintf(speed_str, "unknown");            
            }
        }

        rc = sal_port_linkMedia_get(logicPort, &media);
        if( rc != SYS_ERR_OK)
        {   
            return free_printor_vector(&printor);
        }
        SYS_MEM_CLEAR(type_str);
        if (PORT_MEDIA_COPPER == media)
            osal_sprintf(type_str, "Copper");
        else
            osal_sprintf(type_str, "Fiber");

        add_value_to_new_line_printor_bylen(PRINT_STRING, port_str, &printor, 8);
        add_value_to_last_line_printor_bylen(PRINT_STRING, descp_buf[0], &printor, 18);
        add_value_to_last_line_printor_bylen(PRINT_STRING, status_str, &printor, 12);
        add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &pvid, &printor, 7);
        add_value_to_last_line_printor_bylen(PRINT_STRING, duplex_str, &printor, 10);
        add_value_to_last_line_printor_bylen(PRINT_STRING, speed_str, &printor, 15);
        add_value_to_last_line_printor_bylen(PRINT_STRING, type_str, &printor, 8);
        if(untagLine > 1)
        {
            add_value_to_new_line_printor_bylen(PRINT_STRING, "", &printor, 8);
            add_value_to_last_line_printor_bylen(PRINT_STRING, descp_buf[1], &printor, 18);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "", &printor, 12);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "", &printor, 7);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "", &printor, 10);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "", &printor, 10);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "", &printor, 8);
        }
    }
    if (printor != NULL)
    {
        cmdPrintOutResult(printor, &cInputChar);
        free_printor_vector(&printor);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_port_counter_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;

    CMD_PORTLIST_GET(0, &logicPortmask);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        SYS_ERR_CONTINUE(sal_mib_cntr_clr(logicPort));
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PORT_ISOLATION

int32 cmd_port_protected_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort = 0;
    sys_logic_portmask_t logicPortmask;
    sys_enable_t state;
    char port_str[CAPA_PORT_STR_LEN];

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE(" Port   | Protected State\n");
    XMORE("--------+-----------------\n");
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        LP2STR(logicPort, port_str);
        XMORE(" %6s |", port_str);

        SYS_ERR_CHK(sal_port_protected_get(logicPort, &state));
        XMORE("%s", text_enable[state].text);
        XMORE("\n");
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PORT_ERR_DISABLED

int32 cmd_port_errdisable_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char strPort[CAPA_PORT_STR_LEN];
    uint32 i;
    uint32 time = 0;
    sys_enable_t enable = 0;
    sys_logic_port_t logicPort;
    sys_port_errDisable_t reason;

    XMORE(" ErrDisable Reason        | Timer Status\n");
    XMORE("--------------------------+---------------\n");

    for (i = (SYS_PORT_ERRDISABLE_NONE + 1); i < SYS_PORT_ERRDISABLE_END; i++)
    {
        SYS_ERR_CONTINUE(sal_port_errDisableRecovery_get(i, &enable));

        XMORE(" %24s | %s\n", text_port_errdisable[i].text, text_enable[enable].text);
    }

    SYS_ERR_CHK(sal_port_errDisableTime_get(&time));

    XMORE("\nTimer Interval : %d seconds\n", time);

    XMORE("\n\nInterfaces that will be enabled at the next timeout:\n\n");

    XMORE("Port |  Error Disable Reason    | Time Left  \n");
    XMORE("-----+--------------------------+------------\n");

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        
        SYS_ERR_CONTINUE(sal_port_errDisable_get(logicPort, &reason));
        SYS_ERR_CONTINUE(sal_port_errDisableCurrTime_get(logicPort, &time));
        SYS_ERR_CONTINUE(sal_port_errDisableRecovery_get(reason, &enable));

        if (SYS_PORT_ERRDISABLE_NONE == reason)
            continue;

        SYS_MEM_CLEAR(strPort);
        SYS_ERR_CHK(LP2STR(logicPort, strPort));

        if (ENABLED == enable)
            XMORE("%4s | %24s | %d\n", strPort, text_port_errdisable[reason].text, time);
        else
            XMORE("%4s | %24s | ------\n", strPort, text_port_errdisable[reason].text);
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        enable = ENABLED;
    else
        enable = DISABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_port_enable_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_port_media_type_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_port_comboMedia_t comboMedia = PORT_COMBO_MEDIA_AUTO;

    if (CMD_IS_NO_FORM_EXIST()) 
    {
        comboMedia = PORT_COMBO_MEDIA_AUTO;
    }
    else 
    {
        if (1 == argc)
        {
            if (argv[0][0] == 'a')
                comboMedia = PORT_COMBO_MEDIA_AUTO;
            else if (argv[0][0] == 'r')
                comboMedia = PORT_COMBO_MEDIA_FORCE_COPPER;
            else if (argv[0][0] == 's')
                comboMedia = PORT_COMBO_MEDIA_FORCE_FIBER;
        }
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_port_comboMediaPrefer_set(lp, comboMedia), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_port_speed_no_nego_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST()) 
    {
        enable = ENABLED;
    }
    else 
    {
        enable = DISABLED;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_port_fiberAutoNegoEnable_set(lp, enable), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_port_speed_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_port_speed_t speed;
    sys_logic_port_t lp;

    osal_memset(&speed, 0, sizeof(speed));
    if ('a' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
    {
        speed.autoSpeed = ENABLED;

        if (1 == argc)
        {
            if (0 == osal_strcmp(argv[0], "10"))
                speed.ability = SYS_PORT_SPEED_10M;
            else if (0 == osal_strcmp(argv[0], "100"))
                speed.ability = SYS_PORT_SPEED_100M;
            else if (0 == osal_strcmp(argv[0], "1000"))
                speed.ability = SYS_PORT_SPEED_1000M;
#if 0
            else if (0 == osal_strcmp(argv[0], "2500"))
                speed.ability = SYS_PORT_SPEED_2500M;
#endif 
            else
                speed.ability = SYS_PORT_SPEED_10M_100M;
        }
        else
        {
            speed.ability = SYS_PORT_SPEED_ALL;
        }
    }
    else if ('d' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
    {
        if (0 == osal_strcmp(argv[0], "50"))
            speed.ability = SYS_PORT_SPEED_10G_DAC_50;
        else if (0 == osal_strcmp(argv[0], "100"))
            speed.ability = SYS_PORT_SPEED_10G_DAC_100;
        else if (0 == osal_strcmp(argv[0], "300"))
            speed.ability = SYS_PORT_SPEED_10G_DAC_300;
        else if (0 == osal_strcmp(argv[0], "500"))
            speed.ability = SYS_PORT_SPEED_10G_DAC_500;
    }
    else if (1 == argc)
    {
        speed.autoSpeed = DISABLED;

        if (0 == osal_strcmp(argv[0], "10"))
            speed.ability = SYS_PORT_SPEED_10M;
        else if (0 == osal_strcmp(argv[0], "100"))
            speed.ability = SYS_PORT_SPEED_100M;
        else if (0 == osal_strcmp(argv[0], "1000"))
            speed.ability = SYS_PORT_SPEED_1000M;
#if 0
        else if (0 == osal_strcmp(argv[0], "2500"))
            speed.ability = SYS_PORT_SPEED_2500M;
#endif 
        else if (0 == osal_strcmp(argv[0], "10000"))
            speed.ability = SYS_PORT_SPEED_10G;
    }
    else
    {
        return SYS_ERR_PORT_SPEED;
    }
    
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_port_speed_set(lp, speed), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_port_speed_dac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_port_10gMedia_t xgMedia = PORT_10GMEDIA_FIBER_10G;

    if (1 == argc)
    {
        if (0 == osal_strcmp(argv[0], "50"))
            xgMedia = PORT_10GMEDIA_DAC_50CM;
        else if (0 == osal_strcmp(argv[0], "100"))
            xgMedia = PORT_10GMEDIA_DAC_100CM;
        else if (0 == osal_strcmp(argv[0], "300"))
            xgMedia = PORT_10GMEDIA_DAC_300CM;
        else if (0 == osal_strcmp(argv[0], "500"))
            xgMedia = PORT_10GMEDIA_DAC_500CM;
    }
    else
    {
        return SYS_ERR_PORT_SPEED;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_port_10gMedia_set(lp, xgMedia), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_port_duplex_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_port_duplex_t duplex;
    sys_logic_port_t logicPort;

    osal_memset(&duplex, 0, sizeof(duplex));

    duplex.autoDuplex = DISABLED;

    if ('a' == argv[0][0])
        duplex.autoDuplex = ENABLED;
    else if ('f' == argv[0][0])
        duplex.ability = SYS_PORT_DUPLEX_FULL;
    else if ('h' == argv[0][0])
        duplex.ability = SYS_PORT_DUPLEX_HALF;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        if (IS_TGE_PORT(logicPort))
            continue;

        CMD_ERR_CONTINUE_PORT(sal_port_duplex_set(logicPort, duplex), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_port_flowCtrl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isAutoCfg = FALSE;
    uint8 isStateCfg = FALSE;
    sys_enable_t fcEnable = DISABLED, fcAuto = DISABLED;
    sys_logic_port_t lp;
    char portStr[CAPA_PORT_STR_LEN];

    if (CMD_IS_NO_FORM_EXIST())
    {
        fcAuto = DFLT_PORT_FLOW_CTRL_AUTO;
        isAutoCfg = TRUE;
        if(DISABLED == fcAuto)
        {
            isStateCfg = TRUE;
            fcEnable = DFLT_PORT_FLOW_CTRL;
        }
    }
    else
    {
        isAutoCfg = TRUE;
        if('a' == argv[0][0])
        {
            fcAuto = ENABLED;
        }
        else if ('n' == argv[0][1])
        {
            fcAuto = DISABLED;
            isStateCfg = TRUE;
            fcEnable = ENABLED;
        }
        else
        {
            fcAuto = DISABLED;
            isStateCfg = TRUE;
            fcEnable = DISABLED;
        }
    }

    if(TRUE == isAutoCfg)
    {
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            if(IS_COMBO_PORT(lp) || IS_FIBER_PORT(lp))
            {
                LP2STR(lp, portStr);
                XMORE("Flow Control AutoNegotiation is not supported on port %s\n", portStr);
            }
            CMD_ERR_CONTINUE_PORT(sal_port_flowCtrlAuto_set(lp, fcAuto), lp);
        }
    }

    if(TRUE == isStateCfg)
    {
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            CMD_ERR_CONTINUE_PORT(sal_port_flowCtrl_set(lp, fcEnable), lp);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_port_backPressure_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t backPressure = 0;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST())
    {
        backPressure = DISABLED;
    }
    else
    {
        backPressure = ENABLED;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_port_backpressureEnable_set(lp, backPressure), lp);
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PORT_DESCRIPTION

int32 cmd_port_descp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    char descp[CAPA_PORTDESC_STR_LEN + 1];

    SYS_MEM_CLEAR(descp);

    if (!CMD_IS_NO_FORM_EXIST())
    {
        if (osal_strlen(argv[0]) > CAPA_PORTDESC_STR_LEN)
                return SYS_ERR_STR_LEN_EXCEED;

        if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
            return SYS_ERR_STRING;

        strcpy(descp, argv[0]);
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_port_descp_set(lp, descp), lp);
    }

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PORT_ERR_DISABLED

int32 cmd_port_errDisableRecovery_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    if (argv[0][0] == 'a' && argv[0][1] == 'l')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_USER_DEFINED_ACL, enable));
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_BPDUGUARD, enable));
 #ifdef CONFIG_SYS_PROTO_UDLD
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_UDLD, enable));
 #endif
 #ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_SELFLOOP, enable));
 #endif
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_STORMBCAST, enable));
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_STORMUNKMCAST, enable));
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_STORMUNKUCAST, enable));
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_PORT_SECURITY, enable));
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_DHCP_RATE_LIMIT, enable));
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_ARP_RATE_LIMIT, enable));
    }
    else if (argv[0][0] == 'a' && argv[0][1] == 'c')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_USER_DEFINED_ACL, enable));
    }
    else if (argv[0][0] == 'a' && argv[0][1] == 'r')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_ARP_RATE_LIMIT, enable));
    }
    else if (argv[0][0] == 'b' && argv[0][1] == 'r')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_STORMBCAST, enable));
    }
    else if (argv[0][0] == 'b' && argv[0][1] == 'p')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_BPDUGUARD, enable));
    }
    else if (argv[0][0] == 'd')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_DHCP_RATE_LIMIT, enable));
    }
    else if (argv[0][0] == 'p')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_PORT_SECURITY, enable));
    }
    else if (argv[0][0] == 'u' && argv[0][2] == 'i')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_STORMUNKUCAST, enable));
    }
    else if (argv[0][0] == 'u' && argv[0][2] == 'k')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_STORMUNKMCAST, enable));
    }
 #ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
    else if (argv[0][0] == 's')
    {
        SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_SELFLOOP, enable));
    }
 #endif

    return SYS_ERR_OK;
}

int32 cmd_port_errDisableInterval_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_port_errDisableTime_set(SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_PROTO_UDLD

int32 cmd_port_errDisableRecoveryUDLD_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_port_errDisableRecovery_set(SYS_PORT_ERRDISABLE_UDLD, enable));

    return SYS_ERR_OK;
}
#endif
#endif 

#ifdef CONFIG_SYS_PORT_ISOLATION

int32 cmd_port_protected_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t state;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST())
        state = DISABLED;
    else
        state = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        SYS_ERR_CHK(sal_port_protected_set(lp, state));
    }

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
static int32 _cmd_port_errDisable_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 i = 0;
    uint32 time = 0;
    sys_enable_t enable;

    SYS_ERR_CHK(sal_port_errDisableTime_get(&time));

    if (DFLT_PORT_ERRDISABLE_RECOVERY_INTERVAL != time)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "errdisable recovery interval %d\n", time);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    for (i = SYS_PORT_ERRDISABLE_NONE + 1; i < SYS_PORT_ERRDISABLE_END; i++)
    {
        SYS_ERR_CONTINUE(sal_port_errDisableRecovery_get(i, &enable));

        if (DFLT_PORT_ERRDISABLE_RECOVERY != enable)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "%serrdisable recovery cause %s\n", (ENABLED == enable) ? "" : "no ", text_port_errdisable[i].text);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_port_save(FILE *fp)
{
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    SYS_ERR_CHK(_cmd_port_errDisable_save(fp));
#endif

    return SYS_ERR_OK;
}

static int32 _cmd_port_speed_save(FILE *fp, sys_port_speed_t speed, uint32 isFiber)
{
    char cmd[MAX_CMD_STRLEN];
    char auto_str[SYS_BUF8_LEN];
    char speed_str[SYS_BUF8_LEN];

    SYS_MEM_CLEAR(cmd);
    SYS_MEM_CLEAR(auto_str);
    SYS_MEM_CLEAR(speed_str);

    if (ENABLED == speed.autoSpeed)
    {
        osal_sprintf(auto_str, " auto");

        if (SYS_PORT_SPEED_10M == speed.ability)
            osal_sprintf(speed_str, " 10");
        else if (SYS_PORT_SPEED_100M == speed.ability)
            osal_sprintf(speed_str, " 100");
        else if (SYS_PORT_SPEED_1000M == speed.ability)
            osal_sprintf(speed_str, " 1000");
        else if (SYS_PORT_SPEED_10M_100M == speed.ability)
            osal_sprintf(speed_str, " 10/100");
    }
    else
    {
        if (SYS_PORT_SPEED_10M == speed.ability)
            osal_sprintf(speed_str, " 10");
        else if (SYS_PORT_SPEED_100M == speed.ability)
            osal_sprintf(speed_str, " 100");
        else if (SYS_PORT_SPEED_1000M == speed.ability)
            osal_sprintf(speed_str, " 1000");
        else if (SYS_PORT_SPEED_10G == speed.ability)
            osal_sprintf(speed_str, " 10000");
        else if (SYS_PORT_SPEED_10G_DAC_50 == speed.ability)
            osal_sprintf(speed_str, " dac 50");
        else if (SYS_PORT_SPEED_10G_DAC_100 == speed.ability)
            osal_sprintf(speed_str, " dac 100");
        else if (SYS_PORT_SPEED_10G_DAC_300 == speed.ability)
            osal_sprintf(speed_str, " dac 300");
        else if (SYS_PORT_SPEED_10G_DAC_500 == speed.ability)
            osal_sprintf(speed_str, " dac 500");

    }

    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "%s%s%s\n", "speed", auto_str, speed_str);
    CMD_WRITE_TO_FILE_INDENT1(fp, cmd);

    return SYS_ERR_OK;
}

static int32 _cmd_port_duplex_save(FILE *fp, sys_port_duplex_t duplex)
{
    char cmd[MAX_CMD_STRLEN];
    char duplex_str[SYS_BUF8_LEN];

    SYS_MEM_CLEAR(cmd);
    SYS_MEM_CLEAR(duplex_str);

    if (ENABLED == duplex.autoDuplex)
    {
        osal_sprintf(duplex_str, " auto");
    }
    else
    {
        if (SYS_PORT_DUPLEX_FULL == duplex.ability)
            osal_sprintf(duplex_str, " full");
        else if (SYS_PORT_DUPLEX_HALF == duplex.ability)
            osal_sprintf(duplex_str, " half");
    }

    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "duplex%s\n", duplex_str);
    CMD_WRITE_TO_FILE_INDENT1(fp, cmd);

    return SYS_ERR_OK;
}

int32 cmd_port_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;
    sys_enable_t fcStatus;
    sys_port_conf_t portConf;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_port_media_t media;
    sys_port_comboMedia_t comboMedia;
#ifdef CONFIG_SYS_PORT_DESCRIPTION
    char descp[CAPA_PORTDESC_STR_LEN + 1];
#endif

    SYS_MEM_CLEAR(portConf);
    SYS_MEM_CLEAR(speed);
    SYS_MEM_CLEAR(duplex);

    SYS_ERR_CHK(sal_port_enable_get(port, &enable));

    if (DFLT_PORT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sshutdown\n", (ENABLED == enable) ? "no " : "");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (IS_TGE_PORT(port))
    {
        SYS_ERR_CHK(sal_port_speed_get(port, &speed));

        switch(speed.ability)
        {
            case SYS_PORT_SPEED_10G_DAC_50:
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "speed dac 50\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;
            case SYS_PORT_SPEED_10G_DAC_100:
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "speed dac 100\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;
            case SYS_PORT_SPEED_10G_DAC_300:
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "speed dac 300\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;
            case SYS_PORT_SPEED_10G_DAC_500:
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "speed dac 500\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;
            case SYS_PORT_SPEED_1000M:
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "speed 1000\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;
            case SYS_PORT_SPEED_100M:
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "speed 100\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;
#if 0
            case SYS_PORT_SPEED_2500M:
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "speed 2500\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;
#endif 
            case SYS_PORT_SPEED_10G:
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "speed 10000\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;
            default: 
                break;
        };
        SYS_ERR_CHK(sal_port_flowCtrl_get(port, &enable, &fcStatus));

        if (DFLT_PORT_FLOW_CTRL != enable)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "flowcontrol %s\n", (ENABLED == enable) ? "on" : "off");
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        goto skip_speed_duplex;
    }

    if (IS_COMBO_PORT(port))
    {
        SYS_ERR_CHK(sal_port_comboMediaPrefer_get(port, &comboMedia));
        if (DFLT_PORT_COMBO_AUTO != comboMedia)
        {
            SYS_MEM_CLEAR(cmd);
            switch (comboMedia)
            {
                case PORT_COMBO_MEDIA_AUTO:
                    osal_sprintf(cmd, "media-type auto-select\n");
                    break;
                case PORT_COMBO_MEDIA_FORCE_COPPER:
                    osal_sprintf(cmd, "media-type rj45\n");
                    break;
                case PORT_COMBO_MEDIA_FORCE_FIBER:
                    osal_sprintf(cmd, "media-type sfp\n");
                    break;
                default:
                    break;
            }
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    if (!IS_TRKLP(port))
    {
        SYS_ERR_CHK(sal_port_linkMedia_get(port, &media));
        if (PORT_MEDIA_FIBER == media)
        {
            SYS_ERR_CHK(sal_port_fiberAutoNegoEnable_get(port, &enable));
            if (DFLT_PORT_FIBER_NEGO_EBL != enable)
            {
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "%sspeed nonegotiate\n", (ENABLED == enable) ? "no " : "");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
            }
        }
    }

    if (IS_TRKLP(port))
    {
        portConf.speed = PORTCONF_SPEED_AUTO_ALL;
        portConf.duplex = PORTCONF_DUPLEX_AUTO;
    }
    else
    {
        SYS_ERR_CHK(sal_sys_boardPortConf_get(port, &portConf));
    }

    SYS_ERR_CHK(sal_port_speed_get(port, &speed));

    switch (portConf.speed)
    {
        case PORTCONF_SPEED_FORCE_10M:
            if ((ENABLED == speed.autoSpeed) || (SYS_PORT_SPEED_10M != speed.ability))
            {
                SYS_ERR_CHK(_cmd_port_speed_save(fp, speed, FALSE));
            }
            break;

        case PORTCONF_SPEED_FORCE_100M:
            if ((ENABLED == speed.autoSpeed) || (SYS_PORT_SPEED_100M != speed.ability))
            {
                SYS_ERR_CHK(_cmd_port_speed_save(fp, speed, FALSE));
            }
            break;

        case PORTCONF_SPEED_FORCE_1000M:
            if ((ENABLED == speed.autoSpeed) || (SYS_PORT_SPEED_1000M != speed.ability))
            {
                SYS_ERR_CHK(_cmd_port_speed_save(fp, speed, FALSE));
            }
            break;

        case PORTCONF_SPEED_FORCE_10G:
            if ((ENABLED == speed.autoSpeed) || (SYS_PORT_SPEED_10G != speed.ability))
            {
                SYS_ERR_CHK(_cmd_port_speed_save(fp, speed, FALSE));
            }
            break;

        case PORTCONF_SPEED_AUTO_10M:
            if ((DISABLED == speed.autoSpeed) || (SYS_PORT_SPEED_10M != speed.ability))
            {
                SYS_ERR_CHK(_cmd_port_speed_save(fp, speed, FALSE));
            }
            break;

        case PORTCONF_SPEED_AUTO_100M:
            if ((DISABLED == speed.autoSpeed) || (SYS_PORT_SPEED_100M != speed.ability))
            {
                SYS_ERR_CHK(_cmd_port_speed_save(fp, speed, FALSE));
            }
            break;

        case PORTCONF_SPEED_AUTO_1000M:
            if ((DISABLED == speed.autoSpeed) || (SYS_PORT_SPEED_1000M != speed.ability))
            {
                SYS_ERR_CHK(_cmd_port_speed_save(fp, speed, FALSE));
            }
            break;

        case PORTCONF_SPEED_AUTO_10M_100M:
            if ((DISABLED == speed.autoSpeed) || (SYS_PORT_SPEED_10M_100M != speed.ability))
            {
                SYS_ERR_CHK(_cmd_port_speed_save(fp, speed, FALSE));
            }
            break;

        case PORTCONF_SPEED_AUTO_ALL:
            if ((DISABLED == speed.autoSpeed) || (SYS_PORT_SPEED_ALL != speed.ability))
            {
                SYS_ERR_CHK(_cmd_port_speed_save(fp, speed, FALSE));
            }
            break;

        default:
            break;
    }

    SYS_ERR_CHK(sal_port_duplex_get(port, &duplex));

    switch (portConf.duplex)
    {
        case PORTCONF_DUPLEX_FORCE_FULL:
            if ((ENABLED == duplex.autoDuplex) || (SYS_PORT_DUPLEX_FULL != duplex.ability))
            {
                SYS_ERR_CHK(_cmd_port_duplex_save(fp, duplex));
            }
            break;

        case PORTCONF_DUPLEX_FORCE_HALF:
            if ((ENABLED == duplex.autoDuplex) || (SYS_PORT_DUPLEX_HALF != duplex.ability))
            {
                SYS_ERR_CHK(_cmd_port_duplex_save(fp, duplex));
            }
            break;

        case PORTCONF_DUPLEX_AUTO:
            if (DISABLED == duplex.autoDuplex)
            {
                SYS_ERR_CHK(_cmd_port_duplex_save(fp, duplex));
            }
            break;

        default:
            break;
    }

    SYS_ERR_CHK(sal_port_flowCtrlAuto_get(port, &enable));
    if(DFLT_PORT_FLOW_CTRL_AUTO != enable && ENABLED == enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "flowcontrol auto\n");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if((DFLT_PORT_FLOW_CTRL_AUTO != enable && DISABLED == enable) || DFLT_PORT_FLOW_CTRL_AUTO == enable)
    {
        SYS_ERR_CHK(sal_port_flowCtrl_get(port, &enable, &fcStatus));
        if (DFLT_PORT_FLOW_CTRL != enable)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "flowcontrol %s\n", (ENABLED == enable) ? "on" : "off");
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    SYS_ERR_CHK(sal_port_backpressureEnable_get(port, &enable));

    if (DFLT_PORT_BACK_PRESSURE != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sback-pressure\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

skip_speed_duplex:

#ifdef CONFIG_SYS_PORT_DESCRIPTION
    
    SYS_MEM_CLEAR(descp);
    SYS_ERR_CHK(sal_port_descp_get(port, descp));

    if (0 != osal_strlen(descp))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "description \"%s\"\n", descp);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_PORT_ISOLATION
    
    SYS_ERR_CHK(sal_port_protected_get(port, &enable));

    if (DFLT_PORT_PROTECTED != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sprotected\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

    return SYS_ERR_OK;
}

