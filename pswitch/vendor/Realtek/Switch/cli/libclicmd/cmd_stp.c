
#include <libsal/sal_stp.h>
#include <libcmd/cmd.h>

#define STP_CONF_TYPE_NUM_TWO       (2)     
#define STP_CONF_TYPE_NUM_THREE     (3)     

static int _stp_show_info(void)
{
    sys_stp_info_t stpInfo;
    sys_stp_mst_info_t mstInfo;
    sys_logic_port_t port;
    sys_stp_port_info_t portInfo;
    sys_stp_mst_port_info_t mstPortInfo;
    char strPort[16];
    char *strState[] = { "Dsbl", "Blk", "Lrn", "Frw" };
    char *strRole[] = { "Dsbl", "Mstr", "Root", "Desg", "Altn", "Bckp" };

    SYS_ERR_CHK(sal_stp_info_get(&stpInfo));
    SYS_ERR_CHK(sal_stp_mstInfo_get(0, &mstInfo));

    XMORE("\n");
    XMORE("\n");
    XMORE("  Root ID    Priority    %u\n", mstInfo.rootPriority.rootBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
    XMORE("             Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
        mstInfo.rootPriority.rootBridgeId.addr[0], \
        mstInfo.rootPriority.rootBridgeId.addr[1], \
        mstInfo.rootPriority.rootBridgeId.addr[2], \
        mstInfo.rootPriority.rootBridgeId.addr[3], \
        mstInfo.rootPriority.rootBridgeId.addr[4], \
        mstInfo.rootPriority.rootBridgeId.addr[5]);
    if (mstInfo.rootPortId.number > 0)
    {
        XMORE("             Cost        %u\n", mstInfo.rootPriority.externalRootPathCost);
        
        LP2STR((mstInfo.rootPortId.number - 1), strPort);
        XMORE("             Port        %s\n", strPort);
    }
    else
    {
        XMORE("             This switch is the root\n");
    }
    XMORE("             Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
        mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);

    if (mstInfo.rootPortId.number > 0)
    {
        XMORE("  Bridge ID  Priority    %u\n", mstInfo.bridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
        XMORE("             Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
            mstInfo.bridgeId.addr[0], \
            mstInfo.bridgeId.addr[1], \
            mstInfo.bridgeId.addr[2], \
            mstInfo.bridgeId.addr[3], \
            mstInfo.bridgeId.addr[4], \
            mstInfo.bridgeId.addr[5]);
        XMORE("             Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
            stpInfo.helloTime, stpInfo.maximumAge, stpInfo.forwardDelay);
    }
    XMORE("\n");
    XMORE("  Number of topology changes %u last change occurred %02u:%02u:%02u ago\n", \
        mstInfo.tcTimes, \
        (mstInfo.tcTimer / 3600), \
        ((mstInfo.tcTimer % 3600) / 60), \
        ((mstInfo.tcTimer % 3600) % 60));
    XMORE("  Times:  hold %u, topology change %u, notification %u\n", 0, 0 ,0); 
    XMORE("          hello %u, max age %u, forward delay %u\n", \
        mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);
    XMORE("\n");
    XMORE("Interfaces\n");
    XMORE("  Name     State   Prio.Nbr   Cost    Sts   Role EdgePort       Type\n");
    XMORE("--------- -------- -------- -------- ------ ---- -------- -----------------\n");

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CONTINUE(sal_stp_portInfo_get(port, &portInfo));
        SYS_ERR_CONTINUE(sal_stp_mstPortInfo_get(0, port, &mstPortInfo));

        if (IS_LP_TRKMBR(port) || (STP_PORT_STATE_DISABLED == mstPortInfo.portState))
            continue;

        LP2STR(port, strPort);
        XMORE(" %7s  %-8s  %3u.%-3u %7u   %-4s  %-4s %8s ", \
            strPort, \
            (portInfo.adminEnable)? "enabled" : "disabled", \
            portInfo.portId.priority, \
            portInfo.portId.number, \
            portInfo.operExtPathCost, \
            strState[mstPortInfo.portState], \
            strRole[mstPortInfo.portRole], \
            (portInfo.confEdge)? "Yes" : "No");

        XMORE("%s", (TRUE == portInfo.operPointToPointMac)? "P2P" : "Shared");
#ifdef CONFIG_SYS_PROTO_STP_MSTP
        if (STP_VERSION_MSTP == stpInfo.forceVersion)
        {   
            XMORE(" %s", (STP_PORT_TYPE_BOUNDARY == portInfo.type)? "Bound" : "Intr");
        }
        if (STP_PORT_MODE_MSTP != portInfo.mode)
        {
            XMORE(" %s", (STP_PORT_MODE_RSTP == portInfo.mode)? "(RSTP)" : "(STP)");
        }
#endif

        XMORE("\n");
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP
static int _stp_show_info_mst(void)
{
    sys_stp_info_t stpInfo;
    sys_stp_mst_info_t mstInfo;
    sys_logic_port_t port;
    sys_stp_port_info_t portInfo;
    sys_stp_mst_port_info_t mstPortInfo;
    char strPort[16];
    char *strState[] = { "Dsbl", 
                          "Blk",  
                          "Lrn",  
                          "Frw"    };
    char *strRole[] = { "Dsbl",  
                         "Mstr",  
                         "Root",  
                         "Desg",  
                         "Altn",  
                         "Bckp"    };
    uint32 msti;
    char strVlan[CAPA_VLANLIST_STR_LEN];
    uint8 isCistRoot;
    uint8 isIstMaster;
    uint8 isRegionalRoot;

    SYS_ERR_CHK(sal_stp_info_get(&stpInfo));

    XMORE("\n");
    XMORE("\n");
    XMORE("Gathering information ..........\n");

    for (msti=0; msti<CAPA_STP_MST_INSTANCE_NUM; msti++)
    {
        SYS_ERR_CHK(sal_stp_mstInfo_get(msti, &mstInfo));

        if ((MSTP_INSTANCE_CIST != msti) && IS_VLANMASK_CLEAR(mstInfo.vlanmask))
        {
            continue;
        }

        SYS_ERR_CHK(VMSK2STR(&mstInfo.vlanmask, strVlan));
        XMORE("###### MST %u Vlans Mapped: %s\n", msti, strVlan);

        if (MSTP_INSTANCE_CIST == msti)
        {   
            isCistRoot = (mstInfo.rootPortId.number == 0)? TRUE : FALSE;
            isIstMaster = IS_BID_EQUAL(&mstInfo.rootPriority.regionalRootBridgeId, &mstInfo.bridgeId)? TRUE : FALSE;

            XMORE("CST Root ID    Priority    %u\n", mstInfo.rootPriority.rootBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
            XMORE("               Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
                mstInfo.rootPriority.rootBridgeId.addr[0], \
                mstInfo.rootPriority.rootBridgeId.addr[1], \
                mstInfo.rootPriority.rootBridgeId.addr[2], \
                mstInfo.rootPriority.rootBridgeId.addr[3], \
                mstInfo.rootPriority.rootBridgeId.addr[4], \
                mstInfo.rootPriority.rootBridgeId.addr[5]);
            if (isCistRoot)
            {
                if (isIstMaster)
                {
                    XMORE("               This switch is root for CST and IST master\n");
                }

                XMORE("               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                    stpInfo.helloTime, stpInfo.maximumAge, stpInfo.forwardDelay);
                XMORE("               Max hops    %u\n", stpInfo.maxHops);
            }
            else
            {
                if (mstInfo.rootPriority.externalRootPathCost == 0)
                {
                    XMORE("               The IST ROOT is the CST ROOT\n");
                }
                else
                {
                    XMORE("               Path Cost   %u\n", mstInfo.rootPriority.externalRootPathCost);
                }
                
                LP2STR((mstInfo.rootPortId.number - 1), strPort);
                XMORE("               Root Port   %s\n", strPort);

                if (isIstMaster)
                {
                    XMORE("               This switch is the IST master\n");
                    XMORE("               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                        mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);
                }
                else
                {
                    XMORE("               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                        mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);

                }

                XMORE("Bridge ID      Priority    %u\n", mstInfo.bridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
                XMORE("               Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
                    mstInfo.bridgeId.addr[0], \
                    mstInfo.bridgeId.addr[1], \
                    mstInfo.bridgeId.addr[2], \
                    mstInfo.bridgeId.addr[3], \
                    mstInfo.bridgeId.addr[4], \
                    mstInfo.bridgeId.addr[5]);
                XMORE("               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                    stpInfo.helloTime, stpInfo.maximumAge, stpInfo.forwardDelay);
                XMORE("               Max hops    %u\n", stpInfo.maxHops);
            }

            XMORE("  Name     State   Prio.Nbr   Cost    Sts   Role EdgePort       Type\n");
            XMORE("--------- -------- -------- -------- ------ ---- -------- -----------------\n");
            FOR_EACH_LOGIC_PORT(port)
            {
                SYS_ERR_CONTINUE(sal_stp_portInfo_get(port, &portInfo));
                SYS_ERR_CONTINUE(sal_stp_mstPortInfo_get(msti, port, &mstPortInfo));

                if (IS_LP_TRKMBR(port) || (STP_PORT_STATE_DISABLED == mstPortInfo.portState))
                    continue;

                LP2STR(port, strPort);
                XMORE("%-9s %-8s %3u.%-4u %-8u %-6s %-4s %-8s ", \
                        strPort, \
                        (portInfo.adminEnable)? "enabled" : "disabled", \
                        portInfo.portId.priority, \
                        portInfo.portId.number, \
                        portInfo.operExtPathCost, \
                        strState[mstPortInfo.portState], \
                        strRole[mstPortInfo.portRole], \
                        (portInfo.confEdge)? "Yes" : "No");

                XMORE("%s", (TRUE == portInfo.operPointToPointMac)? "P2P" : "Shared");
                if (STP_VERSION_MSTP == stpInfo.forceVersion)
                {   
                    XMORE(" %s", (STP_PORT_TYPE_BOUNDARY == portInfo.type)? "Bound" : "Intr");

                }
                if (STP_PORT_MODE_MSTP != portInfo.mode)
                {
                    XMORE(" %s", (STP_PORT_MODE_RSTP == portInfo.mode)? "(RSTP)" : "(STP)");
                }

                XMORE("\n");
            }

            XMORE("\n\n\n\n");
        }
        else
        {   
            isRegionalRoot = IS_BID_EQUAL(&mstInfo.rootPriority.regionalRootBridgeId, &mstInfo.bridgeId)? TRUE : FALSE;

            XMORE("\n");
            XMORE("Root ID        Priority    %u\n", mstInfo.rootPriority.regionalRootBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
            XMORE("               Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
                mstInfo.rootPriority.regionalRootBridgeId.addr[0], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[1], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[2], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[3], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[4], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[5]);

            if (isRegionalRoot)
            {
                XMORE("               This switch is the regional Root\n");
            }
            else
            {
                XMORE("               Path Cost   %u\n", mstInfo.rootPriority.internalRootPathCost);
                
                LP2STR((mstInfo.rootPortId.number - 1), strPort);
                XMORE("               Root Port   %s\n", strPort);
                XMORE("               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                    mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);

                XMORE("\n");

                XMORE("Bridge ID      Priority    %u\n", mstInfo.bridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
                XMORE("               Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
                    mstInfo.bridgeId.addr[0], \
                    mstInfo.bridgeId.addr[1], \
                    mstInfo.bridgeId.addr[2], \
                    mstInfo.bridgeId.addr[3], \
                    mstInfo.bridgeId.addr[4], \
                    mstInfo.bridgeId.addr[5]);

            }

            XMORE("Interfaces\n");
            XMORE("Name       State     Prio.Nbr   Cost      Sts  Role  EdgePort  Type\n");
            XMORE("--------   --------  --------   --------- ---- ----  --------  ----------\n");
            FOR_EACH_LOGIC_PORT(port)
            {
                SYS_ERR_CONTINUE(sal_stp_portInfo_get(port, &portInfo));
                SYS_ERR_CONTINUE(sal_stp_mstPortInfo_get(msti, port, &mstPortInfo));

                if (IS_LP_TRKMBR(port) || (STP_PORT_STATE_DISABLED == mstPortInfo.portState))
                    continue;

                LP2STR(port, strPort);
                XMORE("%-8s   %-8s  %3u.%-4u   %-9u %-4s %-4s  %-8s  ", \
                        strPort, \
                        (portInfo.adminEnable)? "enabled" : "disabled", \
                        mstPortInfo.portId.priority, \
                        mstPortInfo.portId.number, \
                        mstPortInfo.operIntPathCost, \
                        strState[mstPortInfo.portState], \
                        strRole[mstPortInfo.portRole], \
                        (portInfo.confEdge)? "Yes" : "No");

                XMORE("%s", (TRUE == portInfo.operPointToPointMac)? "P2P" : "Shared");
                if (STP_VERSION_MSTP == stpInfo.forceVersion)
                {   
                    XMORE(" %s", (STP_PORT_TYPE_BOUNDARY == portInfo.type)? "Bound" : "Intr");

                }
                if (STP_PORT_MODE_MSTP != portInfo.mode)
                {
                    XMORE(" %s", (STP_PORT_MODE_RSTP == portInfo.mode)? "(RSTP)" : "(STP)");
                }

                XMORE("\n");
            }

            XMORE("\n");
        }

    }

    return SYS_ERR_OK;
}
#endif

static int32 _stp_show_port(sys_logic_port_t logicPort)
{
    sys_stp_info_t stpInfo;
    sys_stp_port_info_t portInfo;
    sys_stp_mst_port_info_t mstPortInfo;
    char strPort[CAPA_PORT_STR_LEN];
    char strPortMaster[CAPA_PORT_STR_LEN];
    char strPortType[SYS_BUF32_LEN];
    char *strState[] = { "disabled",   
                         "blocking",   
                         "learning",   
                         "forwarding"   };
    char *strRole[] = { "disabled",    
                        "master",      
                        "root",        
                        "designated",  
                        "alternated",  
                        "backup"        };

    SYS_ERR_CHK(sal_stp_info_get(&stpInfo));
    SYS_ERR_CHK(sal_stp_portInfo_get(logicPort, &portInfo));
    SYS_ERR_CHK(sal_stp_mstPortInfo_get(0, logicPort, &mstPortInfo));

    if (IS_LP_TRKMBR(logicPort))
    {
        LP2STR(logicPort, strPort);
        LP2STR(LP2TRKLP(logicPort), strPortMaster);
        XMORE("%s is a member of %s\n\n", strPort, strPortMaster);

        return SYS_ERR_OK;
    }

    LP2STR(logicPort, strPort);
    SYS_MEM_CLEAR(strPortType);
    osal_sprintf(strPortType, "%s", (TRUE == portInfo.operPointToPointMac)? "P2P" : "Shared");
    if (STP_VERSION_MSTP == stpInfo.forceVersion)
    {   
        osal_sprintf(strPortType, "%s %s", strPortType, (STP_PORT_TYPE_BOUNDARY == portInfo.type)? "Boundary" : "Internal");
    }
    if (STP_PORT_MODE_MSTP != portInfo.mode)
    {
        osal_sprintf(strPortType, "%s %s", strPortType, (STP_PORT_MODE_RSTP == portInfo.mode)? "(RSTP)" : "(STP)");
    }

    XMORE("Port %s %s\n", strPort, (portInfo.adminEnable)? "enabled" : "disabled");
    XMORE("State: %-10s                              Role: %-10s\n", \
        strState[mstPortInfo.portState], strRole[mstPortInfo.portRole]);
    XMORE("Port id: %3u.%-4u                              Port cost: %u\n", \
        portInfo.portId.priority, portInfo.portId.number, portInfo.operExtPathCost);
    XMORE("Type: %-32s         Edge Port: %s\n", \
        strPortType, (portInfo.operEdge)? "Yes" : "No");
    XMORE("Designated bridge Priority : %-5u             Address: %02x:%02x:%02x:%02x:%02x:%02x\n", \
        mstPortInfo.portPriority.designatedBridgeId.priority, \
        mstPortInfo.portPriority.designatedBridgeId.addr[0], \
        mstPortInfo.portPriority.designatedBridgeId.addr[1], \
        mstPortInfo.portPriority.designatedBridgeId.addr[2], \
        mstPortInfo.portPriority.designatedBridgeId.addr[3], \
        mstPortInfo.portPriority.designatedBridgeId.addr[4], \
        mstPortInfo.portPriority.designatedBridgeId.addr[5]);
    XMORE("Designated port id: %3u.%-4u                   Designated path cost: %u\n", \
        mstPortInfo.portPriority.designatedPortId.priority, \
        mstPortInfo.portPriority.designatedPortId.number, \
        mstPortInfo.portPriority.externalRootPathCost);

#ifdef CONFIG_SYS_STP_BPDU_FILTER
    XMORE("BPDU Filter: %-8s                          ",
          (portInfo.confBpduFilter) ? "Enabled" : "Disabled");
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD
    XMORE("BPDU guard: %-8s",
          (portInfo.confBpduGuard)? "Enabled" : "Disabled");

 #ifdef CONFIG_SYS_STP_BPDU_FILTER
    XMORE("\n");
 #else
    XMORE("                          ");
 #endif
#endif

    XMORE("BPDU: sent %u, received %u\n", \
        (portInfo.pktCount.txConfig + portInfo.pktCount.txTcn + portInfo.pktCount.txMstp), \
        (portInfo.pktCount.rxConfig + portInfo.pktCount.rxTcn + portInfo.pktCount.rxMstp));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP
static int _stp_show_cist(void)
{
    sys_stp_info_t stpInfo;
    sys_stp_mst_info_t stpMstInfo;
    char strVlan[CAPA_VLANLIST_STR_LEN];
    sys_logic_port_t port;
    sys_stp_port_info_t portInfo;
    sys_stp_mst_port_info_t mstPortInfo;
    char strPort[16];
    char *strState[] = { "DSB",  
                          "BLK",  
                          "LRN",  
                          "FWD"    };
    char *strRole[] = { "Dsbl",  
                         "Mstr",  
                         "Root",  
                         "Desg",  
                         "Altn",  
                         "Bckp"    };

    SYS_ERR_CHK(sal_stp_mstInfo_get(0, &stpMstInfo));
    SYS_ERR_CHK(sal_stp_info_get(&stpInfo));

    XMORE("                                                            \n");
    XMORE(" MST Instance Information                                   \n");
    XMORE("============================================================\n");
    XMORE("            Instance Type : %s (%u) \n", "CIST", 0);
#if 0
    XMORE("          Instance Status : %s \n", (stpMstInfo.enable)? "Enabled" : "Disabled");
#endif
    XMORE("        Bridge Identifier : %5u/%2u/%02X:%02X:%02X:%02X:%02X:%02X \n", \
        (stpMstInfo.bridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK), \
        (stpMstInfo.bridgeId.priority & MSTP_BRIDGE_EXTID_MASK), \
        stpMstInfo.bridgeId.addr[0], stpMstInfo.bridgeId.addr[1], \
        stpMstInfo.bridgeId.addr[2], stpMstInfo.bridgeId.addr[3], \
        stpMstInfo.bridgeId.addr[4], stpMstInfo.bridgeId.addr[5]);
    XMORE("------------------------------------------------------------\n");
    XMORE("   Designated Root Bridge : %5u/%2u/%02X:%02X:%02X:%02X:%02X:%02X \n", \
        (stpMstInfo.rootPriority.rootBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK), \
        (stpMstInfo.rootPriority.rootBridgeId.priority & MSTP_BRIDGE_EXTID_MASK), \
        stpMstInfo.rootPriority.rootBridgeId.addr[0], \
        stpMstInfo.rootPriority.rootBridgeId.addr[1], \
        stpMstInfo.rootPriority.rootBridgeId.addr[2], \
        stpMstInfo.rootPriority.rootBridgeId.addr[3], \
        stpMstInfo.rootPriority.rootBridgeId.addr[4], \
        stpMstInfo.rootPriority.rootBridgeId.addr[5]);
    XMORE("  External Root Path Cost : %u \n", \
        stpMstInfo.rootPriority.externalRootPathCost);
    XMORE("     Regional Root Bridge : %5u/%2u/%02X:%02X:%02X:%02X:%02X:%02X \n", \
        (stpMstInfo.rootPriority.regionalRootBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK), \
        (stpMstInfo.rootPriority.regionalRootBridgeId.priority & MSTP_BRIDGE_EXTID_MASK), \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[0], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[1], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[2], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[3], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[4], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[5]);
    XMORE("  Internal Root Path Cost : %u \n", \
        stpMstInfo.rootPriority.internalRootPathCost);
    XMORE("        Designated Bridge : %5u/%2u/%02X:%02X:%02X:%02X:%02X:%02X \n", \
        (stpMstInfo.rootPriority.designatedBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK), \
        (stpMstInfo.rootPriority.designatedBridgeId.priority & MSTP_BRIDGE_EXTID_MASK), \
        stpMstInfo.rootPriority.designatedBridgeId.addr[0], \
        stpMstInfo.rootPriority.designatedBridgeId.addr[1], \
        stpMstInfo.rootPriority.designatedBridgeId.addr[2], \
        stpMstInfo.rootPriority.designatedBridgeId.addr[3], \
        stpMstInfo.rootPriority.designatedBridgeId.addr[4], \
        stpMstInfo.rootPriority.designatedBridgeId.addr[5]);
    XMORE("                Root Port : %u/%u \n", \
        stpMstInfo.rootPortId.priority, stpMstInfo.rootPortId.number);
    XMORE("                  Max Age : %u \n", stpMstInfo.maxAge);
    XMORE("            Forward Delay : %u \n", stpMstInfo.forwardDelay);
    XMORE("         Topology changes : %u \n", stpMstInfo.tcTimes);
    XMORE("     Last Topology Change : %u \n", stpMstInfo.tcTimer);
    XMORE("------------------------------------------------------------\n");
    SYS_ERR_CHK(VMSK2STR(&stpMstInfo.vlanmask, strVlan));
    XMORE(" VLANs mapped: %s \n", strVlan);
    XMORE("============================================================\n\n");

    XMORE("Interface        Role Sts Cost      Prio.Nbr Type\n");
    XMORE("---------------- ---- --- --------- -------- --------------------------------\n");
    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CONTINUE(sal_stp_portInfo_get(port, &portInfo));
        SYS_ERR_CONTINUE(sal_stp_mstPortInfo_get(0, port, &mstPortInfo));

        if (IS_LP_TRKMBR(port) || (STP_PORT_STATE_DISABLED == mstPortInfo.portState))
            continue;

        LP2STR(port, strPort);
        XMORE("%-16s %-4s %-3s %-9u %3u.%-4u ", \
            strPort, \
            strRole[mstPortInfo.portRole], \
            strState[mstPortInfo.portState], \
            mstPortInfo.operIntPathCost, \
            mstPortInfo.portId.priority, mstPortInfo.portId.number);

        XMORE("%s", (TRUE == portInfo.operPointToPointMac)? "P2P" : "Shared");
        if (STP_VERSION_MSTP == stpInfo.forceVersion)
        {   
            XMORE(" %s", (STP_PORT_TYPE_BOUNDARY == portInfo.type)? "Bound" : "Intr");

        }
        if (STP_PORT_MODE_MSTP != portInfo.mode)
        {
            XMORE(" %s", (STP_PORT_MODE_RSTP == portInfo.mode)? "(RSTP)" : "(STP)");
        }

        XMORE("\n");
    }

    XMORE("\n\n");

    return SYS_ERR_OK;
}

static int _stp_show_mst(uint32 msti)
{
    sys_stp_info_t stpInfo;
    sys_stp_mst_info_t stpMstInfo;
    char strVlan[CAPA_VLANLIST_STR_LEN];
    sys_logic_port_t port;
    sys_stp_port_info_t portInfo;
    sys_stp_mst_port_info_t mstPortInfo;
    char strPort[16];
    char *strState[] = { "DSB",  
                          "BLK",  
                          "LRN",  
                          "FWD"    };
    char *strRole[] = { "Dsbl",  
                         "Mstr",  
                         "Root",  
                         "Desg",  
                         "Altn",  
                         "Bckp"    };

    SYS_ERR_CHK(sal_stp_mstInfo_get(msti, &stpMstInfo));
    SYS_ERR_CHK(sal_stp_info_get(&stpInfo));

    XMORE("                                                            \n");
    XMORE(" MST Instance Information                                   \n");
    XMORE("============================================================\n");
    XMORE("            Instance Type : %s (%u) \n", "MSTI", msti);
#if 0
    XMORE("          Instance Status : %s \n", (stpInfo.enable && (STP_VERSION_MSTP == stpInfo.forceVersion))? "Enabled" : "Disabled");
#endif
    XMORE("        Bridge Identifier : %5u/%2u/%02X:%02X:%02X:%02X:%02X:%02X \n", \
        (stpMstInfo.bridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK), \
        (stpMstInfo.bridgeId.priority & MSTP_BRIDGE_EXTID_MASK), \
        stpMstInfo.bridgeId.addr[0], stpMstInfo.bridgeId.addr[1], \
        stpMstInfo.bridgeId.addr[2], stpMstInfo.bridgeId.addr[3], \
        stpMstInfo.bridgeId.addr[4], stpMstInfo.bridgeId.addr[5]);
    XMORE("------------------------------------------------------------\n");
    XMORE("     Regional Root Bridge : %5u/%2u/%02X:%02X:%02X:%02X:%02X:%02X \n", \
        (stpMstInfo.rootPriority.regionalRootBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK), \
        (stpMstInfo.rootPriority.regionalRootBridgeId.priority & MSTP_BRIDGE_EXTID_MASK), \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[0], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[1], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[2], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[3], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[4], \
        stpMstInfo.rootPriority.regionalRootBridgeId.addr[5]);
    XMORE("  Internal Root Path Cost : %u \n", \
        stpMstInfo.rootPriority.internalRootPathCost);
    XMORE("           Remaining Hops : %u \n", stpMstInfo.remainingHops);
    XMORE("         Topology changes : %u \n", stpMstInfo.tcTimes);
    XMORE("     Last Topology Change : %u \n", stpMstInfo.tcTimer);
    XMORE("------------------------------------------------------------\n");
    SYS_ERR_CHK(VMSK2STR(&stpMstInfo.vlanmask, strVlan));
    XMORE(" VLANs mapped: %s \n", strVlan);
    XMORE("============================================================\n\n");

    XMORE("Interface        Role Sts Cost      Prio.Nbr Type\n");
    XMORE("---------------- ---- --- --------- -------- --------------------------------\n");
    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CONTINUE(sal_stp_portInfo_get(port, &portInfo));
        SYS_ERR_CONTINUE(sal_stp_mstPortInfo_get(msti, port, &mstPortInfo));

        if (IS_LP_TRKMBR(port) || (STP_PORT_STATE_DISABLED == mstPortInfo.portState))
            continue;

        LP2STR(port, strPort);
        XMORE("%-16s %-4s %-3s %-9u %3u.%-4u ", \
            strPort, \
            strRole[mstPortInfo.portRole], \
            strState[mstPortInfo.portState], \
            mstPortInfo.operIntPathCost, \
            mstPortInfo.portId.priority, mstPortInfo.portId.number);

        XMORE("%s", (TRUE == portInfo.operPointToPointMac)? "P2P" : "Shared");
        if (STP_VERSION_MSTP == stpInfo.forceVersion)
        {   
            XMORE(" %s", (STP_PORT_TYPE_BOUNDARY == portInfo.type)? "Bound" : "Intr");

        }
        if (STP_PORT_MODE_MSTP != portInfo.mode)
        {
            XMORE(" %s", (STP_PORT_MODE_RSTP == portInfo.mode)? "(RSTP)" : "(STP)");
        }

        XMORE("\n");
    }

    XMORE("\n\n");

    return SYS_ERR_OK;
}
#endif

int32 cmd_stp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_stp_info_t stpInfo;
    char *strMode[] = { "STP", "STP", "RSTP", "MSTP" };
    char *strPathcostMethod[] = { "long", "short" };

    SYS_ERR_CHK(sal_stp_info_get(&stpInfo));

    XMORE("\n");
    XMORE("\n");
    XMORE("Spanning tree %s", (ENABLED == stpInfo.enable)? "enabled" : "disabled");
    if (!stpInfo.enable)
    {
        XMORE(" (BPDU %s)", (ENABLED == stpInfo.forwardBpdu)? "flooding" : "filtering");
    }
    XMORE(" mode %s\n", strMode[(stpInfo.forceVersion % 4)]);
    XMORE("Default port cost method:  %s\n", strPathcostMethod[(stpInfo.pathcostMethod % 2)]);
    XMORE("\n");

    if (!stpInfo.enable)
    {
        return SYS_ERR_OK;
    }

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    if (STP_VERSION_MSTP <= stpInfo.forceVersion)
    {
        SYS_ERR_CHK(_stp_show_info_mst());
        return SYS_ERR_OK;
    }
#endif
    SYS_ERR_CHK(_stp_show_info());

    return SYS_ERR_OK;
}

int32 cmd_stp_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_stp_info_t stpInfo;
    sys_logic_portmask_t lpm;
    sys_logic_port_t lp;

    SYS_ERR_CHK(sal_stp_info_get(&stpInfo));

    if (DISABLED == stpInfo.enable)
    {
        XMORE("Spanning tree disabled\n");

        return SYS_ERR_OK;
    }

    XMORE("\n");

    CMD_PORTLIST_GET(0, &lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CMD_ERR_CONTINUE_PORT(_stp_show_port(lp), lp);
        XMORE("\n");
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP

int32 cmd_stp_mstConfig_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_stp_info_t stpInfo;
    sys_stp_mst_info_t mstInfo;
    uint32 cntMstiEnabled = 0;
    uint32 msti;
    char strVlan[CAPA_STP_MST_INSTANCE_NUM][CAPA_VLANLIST_STR_LEN];

    SYS_ERR_CHK(sal_stp_info_get(&stpInfo));

    for (msti = 0; msti < CAPA_STP_MST_INSTANCE_NUM; msti++)
    {
        SYS_ERR_CHK(sal_stp_mstInfo_get(msti, &mstInfo));
        SYS_ERR_CHK(VMSK2STR(&mstInfo.vlanmask, strVlan[msti]));
        if (osal_strcmp(strVlan[msti], ""))
            cntMstiEnabled++;
    }

    XMORE("Name      [%s]\n", stpInfo.mstConfId.name);
    XMORE("Revision  %-5u Instances configured %u\n", stpInfo.mstConfId.revision, cntMstiEnabled);
    XMORE("\n");
    XMORE("Instance  Vlans mapped\n");
    XMORE("--------  ---------------------------------------------------------------------\n");

    for (msti=0; msti<CAPA_STP_MST_INSTANCE_NUM; msti++)
    {
        if (osal_strcmp(strVlan[msti], ""))
        {
            XMORE("%-4u      %s\n", msti, strVlan[msti]);
        }
    }

    XMORE("-------------------------------------------------------------------------------\n");

    return SYS_ERR_OK;
}

int32 cmd_stp_mst_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 msti = SYS_STR2UINT(argv[0]);

    if (0 == msti)
        SYS_ERR_CHK(_stp_show_cist());
    else
        SYS_ERR_CHK(_stp_show_mst(msti));

    return SYS_ERR_OK;
}

int32 cmd_stp_portMst_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t adminEnable = DISABLED;
    uint32 instanceId = SYS_STR2UINT(argv[0]);
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_stp_port_info_t stpPortInfo;
    sys_stp_mst_port_info_t mstPortInfo;
    static char strPortRole[END_STP_PORT_ROLE][16] = { \
        "Disabled", "Master", "Root", "Designated", "Alternated", "Backup" };
    static char strPortState[END_STP_PORT_STATE][16] = { \
        "Disabled", "Discarding", "Learning", "Forwarding" };
    char strPortMember[CAPA_PORT_STR_LEN];
    char strPortMaster[CAPA_PORT_STR_LEN];

    CMD_PORTLIST_GET(1, &lpm);

    XMORE("                                                            \n");
    XMORE(" MST Port Information                                       \n");
    XMORE("============================================================\n");
    XMORE(" Instance Type : %s (%u) \n", (0 == instanceId)? "CIST" : "MSTI", instanceId);
    XMORE("------------------------------------------------------------\n\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        SYS_ERR_CHK(sal_stp_portInfo_get(lp, &stpPortInfo));
        SYS_ERR_CHK(sal_stp_mstPortInfo_get(instanceId, lp, &mstPortInfo));
        SYS_ERR_CHK(sal_stp_portAdminEnable_get(lp, &adminEnable));

        if (IS_LP_TRKMBR(lp))
        {
            LP2STR(lp, strPortMember);
            LP2STR(LP2TRKLP(lp), strPortMaster);
            XMORE("%s is a member of %s\n\n", strPortMember, strPortMaster);

            continue;
        }

        XMORE("      Port Identifier : %u/%u \n", mstPortInfo.portId.priority, mstPortInfo.portId.number);

        if (0 == instanceId)
            XMORE("   External Path-Cost : %-9u/%-9u \n", stpPortInfo.confExtPathCost, stpPortInfo.operExtPathCost);

        XMORE("   Internal Path-Cost : %-9u/%-9u \n", mstPortInfo.confIntPathCost, mstPortInfo.operIntPathCost);

        XMORE("------------------------------------------------------------\n");
        if (0 == instanceId)
        {
            XMORE("  Designated Root Bridge : %u/%02X:%02X:%02X:%02X:%02X:%02X \n", \
                mstPortInfo.portPriority.rootBridgeId.priority, \
                mstPortInfo.portPriority.rootBridgeId.addr[0], \
                mstPortInfo.portPriority.rootBridgeId.addr[1], \
                mstPortInfo.portPriority.rootBridgeId.addr[2], \
                mstPortInfo.portPriority.rootBridgeId.addr[3], \
                mstPortInfo.portPriority.rootBridgeId.addr[4], \
                mstPortInfo.portPriority.rootBridgeId.addr[5]);
            XMORE("      External Root Cost : %u \n", \
                mstPortInfo.portPriority.externalRootPathCost);
        }
        XMORE("    Regional Root Bridge : %u/%02X:%02X:%02X:%02X:%02X:%02X \n", \
            mstPortInfo.portPriority.regionalRootBridgeId.priority, \
            mstPortInfo.portPriority.regionalRootBridgeId.addr[0], \
            mstPortInfo.portPriority.regionalRootBridgeId.addr[1], \
            mstPortInfo.portPriority.regionalRootBridgeId.addr[2], \
            mstPortInfo.portPriority.regionalRootBridgeId.addr[3], \
            mstPortInfo.portPriority.regionalRootBridgeId.addr[4], \
            mstPortInfo.portPriority.regionalRootBridgeId.addr[5]);
        XMORE("      Internal Root Cost : %u \n", \
            mstPortInfo.portPriority.internalRootPathCost);
        XMORE("       Designated Bridge : %u/%02X:%02X:%02X:%02X:%02X:%02X \n", \
            mstPortInfo.portPriority.designatedBridgeId.priority, \
            mstPortInfo.portPriority.designatedBridgeId.addr[0], \
            mstPortInfo.portPriority.designatedBridgeId.addr[1], \
            mstPortInfo.portPriority.designatedBridgeId.addr[2], \
            mstPortInfo.portPriority.designatedBridgeId.addr[3], \
            mstPortInfo.portPriority.designatedBridgeId.addr[4], \
            mstPortInfo.portPriority.designatedBridgeId.addr[5]);
        XMORE(" Internal Port Path Cost : %u \n", mstPortInfo.operIntPathCost);

        if (ENABLED == adminEnable)
            XMORE("               Port Role : %s\n", strPortRole[mstPortInfo.portRole]);
        else
            XMORE("               Port Role : %s\n", strPortRole[STP_PORT_ROLE_DISABLED]);

        XMORE("              Port State : %s\n", strPortState[mstPortInfo.portState]);
        XMORE("------------------------------------------------------------\n\n");
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_stp_portStatistics_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    CMD_PORTLIST_GET(0, &lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        CMD_ERR_CONTINUE_PORT(sal_stp_portCounter_clear(lp), lp);

    return SYS_ERR_OK;
}

int32 cmd_stp_portStatistics_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_stp_port_info_t stpPortInfo;
    char portStr[CAPA_PORT_STR_LEN];

    CMD_PORTLIST_GET(0, &lpm);

    XMORE("                                                  \n");
    XMORE(" STP Port Statistics                              \n");
    XMORE("==================================================\n\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        SYS_MEM_CLEAR(stpPortInfo);

        LP2STR(lp, portStr);
        CMD_ERR_CONTINUE_PORT(sal_stp_portInfo_get(lp, &stpPortInfo), lp);
        XMORE("Port                            : %s\n", portStr);
        XMORE("Configuration BDPUs Received    : %u\n", stpPortInfo.pktCount.rxConfig);
        XMORE("TCN BDPUs Received              : %u\n", stpPortInfo.pktCount.rxTcn);
#ifdef CONFIG_SYS_PROTO_STP_MSTP
        XMORE("MSTP BDPUs Received             : %u\n", stpPortInfo.pktCount.rxMstp);
#endif
        XMORE("Configuration BDPUs Transmitted : %u\n", stpPortInfo.pktCount.txConfig);
        XMORE("TCN BDPUs Transmitted           : %u\n", stpPortInfo.pktCount.txTcn);
#ifdef CONFIG_SYS_PROTO_STP_MSTP
        XMORE("MSTP BDPUs Transmitted          : %u\n", stpPortInfo.pktCount.txMstp);
#endif
        XMORE("==================================================\n\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_stp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_stp_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_stp_forwardBpdu_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_stp_forwardBpdu_set(DFLT_STP_BPDU_FORWARD_EBL));
    else
        SYS_ERR_CHK(sal_stp_forwardBpdu_set((('l' == argv[0][1])) ? ENABLED : DISABLED));

    return SYS_ERR_OK;
}

int32 cmd_stp_forceVersion_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 forceVersion = STP_VERSION_STP;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_stp_forceVersion_set(DFLT_STP_FORCE_VERSION));

        return SYS_ERR_OK;
    }

    if (*argv[0] == 's')
        forceVersion = STP_VERSION_STP;
    else if (*argv[0] == 'r')
        forceVersion = STP_VERSION_RSTP;
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    else
        forceVersion = STP_VERSION_MSTP;
#endif

    SYS_ERR_CHK(sal_stp_forceVersion_set(forceVersion));

    return SYS_ERR_OK;
}

int32 cmd_stp_helloTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 helloTime;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_stp_helloTime_set(DFLT_STP_HELLO_TIME));

        return SYS_ERR_OK;
    }

    helloTime = SYS_STR2UINT(argv[0]);

    uint32 maximumAge;
    uint32 helloTimeMax;

    SYS_ERR_CHK(sal_stp_maximumAge_get(&maximumAge));
    helloTimeMax = (maximumAge / 2 - 1);

    if (CMD_IS_CLI_RUN())
    {
        if (helloTime > helloTimeMax)
        {
            XMORE("With maximum age of %d, hello time should be 1~%d.\n", maximumAge, helloTimeMax);
            return SYS_ERR_OK;
        }
    }

    SYS_ERR_CHK(sal_stp_helloTime_set(helloTime));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP

int32 cmd_stp_maxHops_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 maxHops;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_stp_maxHops_set(DFLT_STP_MAX_HOPS));
        return SYS_ERR_OK;
    }

    maxHops = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_stp_maxHops_set(maxHops));

    return SYS_ERR_OK;
}
#endif

int32 cmd_stp_forwardDelay_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 forwardDelay;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_stp_forwardDelay_set(DFLT_STP_FORWARD_DELAY));

        return SYS_ERR_OK;
    }

    forwardDelay = SYS_STR2UINT(argv[0]);

    if (CMD_IS_CLI_RUN())
    {
        uint32 maximumAge;
        uint32 forwardDelayMin;

        SYS_ERR_CHK(sal_stp_maximumAge_get(&maximumAge));
        forwardDelayMin = ((maximumAge + 1) / 2 + 1);
        if (forwardDelay < forwardDelayMin)
        {
            XMORE("With maximum age of %d, forward delay should be %d~30.\n", maximumAge, forwardDelayMin);
            return SYS_ERR_OK;
        }
    }

    SYS_ERR_CHK(sal_stp_forwardDelay_set(forwardDelay));

    return SYS_ERR_OK;
}

int32 cmd_stp_maximumAge_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 maximumAge;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_stp_maximumAge_set(DFLT_STP_MAX_AGE));

        return SYS_ERR_OK;
    }

    maximumAge = SYS_STR2UINT(argv[0]);

    if (CMD_IS_CLI_RUN())
    {
        uint32 forwardDelay;
        uint32 helloTime;
        uint32 maximumAgeMax;
        uint32 maximumAgeMin;

        SYS_ERR_CHK(sal_stp_forwardDelay_get(&forwardDelay));
        maximumAgeMax = (2 * (forwardDelay - 1));
        if (maximumAge > maximumAgeMax)
        {
            XMORE("With forward delay of %d, maximum age should be 6~%d.\n", forwardDelay, maximumAgeMax);
            return SYS_ERR_OK;
        }

        SYS_ERR_CHK(sal_stp_helloTime_get(&helloTime));
        maximumAgeMin = (2 * (helloTime + 1));
        if (maximumAge < maximumAgeMin)
        {
            XMORE("With hello time of %d, maximum age should be %d~40.\n", helloTime, maximumAgeMin);
            return SYS_ERR_OK;
        }
    }

    SYS_ERR_CHK(sal_stp_maximumAge_set(maximumAge));

    return SYS_ERR_OK;
}

int32 cmd_stp_txHoldCount_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 txHoldCount;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_stp_txHoldCount_set(DFLT_STP_TX_HOLD_COUNT));

        return SYS_ERR_OK;
    }

    txHoldCount = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_stp_txHoldCount_set(txHoldCount));

    return SYS_ERR_OK;
}

int32 cmd_stp_pathcostMethod_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 method = STP_PATHCOST_METHOD_LONG;

    method = ('l' == argv[0][0])? STP_PATHCOST_METHOD_LONG : STP_PATHCOST_METHOD_SHORT;

    SYS_ERR_CHK(sal_stp_pathcostMethod_set(method));

    return SYS_ERR_OK;
}

int32 cmd_stp_priority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 priority = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_stp_priority_set(DFLT_STP_PRI));

        return SYS_ERR_OK;
    }

    priority = SYS_STR2UINT(argv[0]);

    if (IS_PARAM_NUM_MULTIPLE_INVALID(priority, 0, BOND_STP_PRI_MAX, 4096))
        return SYS_ERR_STP_BRIDGE_PRIORITY;

    SYS_ERR_CHK(sal_stp_priority_set(priority));

    return SYS_ERR_OK;
}

int32 cmd_stp_portAdminEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_enable_t oldEnable = DISABLED;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_portAdminEnable_get(logicPort, &oldEnable), logicPort);

        if(enable == oldEnable)
            continue;

        CMD_ERR_CONTINUE_PORT(sal_stp_portAdminEnable_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_stp_portForwardBpdu_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_enable_t oldEnable = DISABLED;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        
        SYS_ERR_CHK(sal_stp_forwardBpdu_get(&enable));
    }
    else
    {
        if ('l' == argv[0][1])
            enable = ENABLED; 
        else
            enable = DISABLED; 
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_portForwardBpdu_get(logicPort, &oldEnable), logicPort);

        if(enable == oldEnable)
            continue;

        CMD_ERR_CONTINUE_PORT(sal_stp_portForwardBpdu_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_stp_portPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 priority;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        sys_logic_port_t lp;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            CMD_ERR_CONTINUE_PORT(sal_stp_portPriority_set(lp, DFLT_STP_PORT_PRI), lp);
        }

        return SYS_ERR_OK;
    }

    priority = SYS_STR2UINT(argv[0]);

    if (IS_PARAM_NUM_MULTIPLE_INVALID(priority, 0, BOND_STP_PORT_PRI_MAX, 16))
        return SYS_ERR_STP_PORT_PRIORITY;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_portPriority_set(logicPort, priority), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_stp_portPathCost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 pathCost;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        sys_logic_port_t lp;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            
            CMD_ERR_CONTINUE_PORT(sal_stp_portPathCost_set(lp, DFLT_STP_PORT_PATHCOST), lp);
        }

        return SYS_ERR_OK;
    }

    pathCost = SYS_STR2UINT(argv[0]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_portPathCost_set(logicPort, pathCost), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_stp_portEdgePort_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 portConf;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        portConf = STP_PORT_CONF_FALSE;
    }
    else
    {
		if (*argv[0] == 'a')
			portConf = STP_PORT_CONF_AUTO;
		else
	        portConf = STP_PORT_CONF_TRUE;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_portEdgePort_set(logicPort, portConf), logicPort);
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_STP_BPDU_FILTER

int32 cmd_stp_portBpduFilter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 portConf;
    sys_logic_port_t logicPort;

    portConf = (CMD_IS_NO_FORM_EXIST()) ? STP_PORT_CONF_FALSE : STP_PORT_CONF_TRUE;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_portBpduFilter_set(logicPort, portConf), logicPort);
    }

    return SYS_ERR_OK;
}
#endif  

#ifdef CONFIG_SYS_STP_BPDU_GUARD

int32 cmd_stp_portBpduGuard_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 portConf;
    sys_logic_port_t logicPort;

    portConf = (CMD_IS_NO_FORM_EXIST()) ? STP_PORT_CONF_FALSE : STP_PORT_CONF_TRUE;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_portBpduGuard_set(logicPort, portConf), logicPort);
    }

    return SYS_ERR_OK;
}
#endif  

int32 cmd_stp_portPointToPointMac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 portConf;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        sys_logic_port_t lp;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            
            CMD_ERR_CONTINUE_PORT(sal_stp_portPointToPointMac_set(lp, STP_PORT_CONF_AUTO), lp);
        }

        return SYS_ERR_OK;
    }

    portConf = (argv[0][0] == 'a')? STP_PORT_CONF_AUTO : \
               (argv[0][0] == 'p')? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_portPointToPointMac_set(logicPort, portConf), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_stp_portMcheck_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        
        CMD_ERR_CONTINUE_PORT(sal_stp_portMcheck_set(lp, TRUE), lp);
        
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP

int32 cmd_stp_mst_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    vty->node = MST_CFG_NODE;

    return SYS_ERR_OK;
}

int32 cmd_stp_mstConfigName_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_stp_mstConfigName_set(DFLT_STP_MST_CFG_NAME));

        return SYS_ERR_OK;
    }

    if (osal_strlen(argv[0]) > MSTP_CONF_NAME_LEN)
        XMORE("Warning: mst config name length exceeds recommended maximum %d characters\n", MSTP_CONF_NAME_LEN);

    SYS_ERR_CHK(sal_stp_mstConfigName_set((char *)argv[0]));

    return SYS_ERR_OK;
}

int32 cmd_stp_mstConfigRevision_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 revision;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_stp_mstConfigRevision_set(DFLT_STP_MST_CFG_REV));

        return SYS_ERR_OK;
    }

    revision = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_stp_mstConfigRevision_set(revision));

    return SYS_ERR_OK;
}

int32 cmd_stp_mstVlanInstance_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 instanceId = SYS_STR2UINT(argv[0]);
    sys_vlanmask_t vlanmask;

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (CMD_TOKENNUM_GET(cmd->vline) > 4)
        {
            uint32 instance;
            uint32 vlanId;
            sys_vlanmask_t vlanmask;

            if (0 == instanceId)
                return SYS_ERR_STP_CST_VLAN_DEL;

            if (argc < 2)
                VLANMASK_SET_ALL(vlanmask);
            else
                SYS_ERR_CHK(STR2VMSK(argv[1], &vlanmask));

            FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
            {
                CMD_ERR_CONTINUE_VLAN(sal_stp_mstVlanInstance_get(vlanId, &instance), vlanId);

                if (instance == instanceId)
                {
                    CMD_ERR_CONTINUE_VLAN(sal_stp_mstVlanInstance_set(vlanId, 0), vlanId);
                }
            }
        }
        else
        {
            uint32 instance;
            uint32 vid;

            if (0 == instanceId)
                return SYS_ERR_STP_CST_DEL;

            SYS_ERR_CHK(sal_stp_mstPriority_set(instanceId, DFLT_STP_MST_PRI));

            FOR_EACH_VID(vid)
            {
                SYS_ERR_CHK(sal_stp_mstVlanInstance_get(vid, &instance));

                if (instance == instanceId)
                {
                    SYS_ERR_CHK(sal_stp_mstVlanInstance_set(vid, 0));
                }
            }
        }

        return SYS_ERR_OK;
    }

    if (argc < 2)
        VLANMASK_SET_ALL(vlanmask);
    else
        SYS_ERR_CHK(STR2VMSK(argv[1], &vlanmask));

    SYS_ERR_CHK(sal_stp_mstVlanmask_set(instanceId, vlanmask));

    return SYS_ERR_OK;
}

int32 cmd_stp_mstPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 instanceId = 0;
    uint32 priority = 0;

    if (CMD_IS_NO_FORM_EXIST())
        priority = DFLT_STP_MST_PRI;
	else
		priority = SYS_STR2UINT(argv[1]);
		
    instanceId = SYS_STR2UINT(argv[0]);
    
    if (IS_PARAM_NUM_MULTIPLE_INVALID(priority, 0, BOND_STP_PRI_MAX, 4096))
        return SYS_ERR_STP_BRIDGE_PRIORITY;

    SYS_ERR_CHK(sal_stp_mstPriority_set(instanceId, priority));

    return SYS_ERR_OK;
}

int32 cmd_stp_mstPortPathCost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    uint32 instanceId = SYS_STR2UINT(argv[0]);
    uint32 pathCost;

    if (CMD_IS_NO_FORM_EXIST())
    {
        sys_logic_port_t lp;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            CMD_ERR_CONTINUE_PORT(sal_stp_mstPortPathCost_set(instanceId, lp, DFLT_STP_MST_PORT_PATHCOST), lp);
        }

        return SYS_ERR_OK;
    }

    pathCost = SYS_STR2UINT(argv[1]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_mstPortPathCost_set(instanceId, logicPort, pathCost), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_stp_mstPortPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 instanceId = SYS_STR2UINT(argv[0]);
    uint32 priority;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        sys_logic_port_t lp;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            CMD_ERR_CONTINUE_PORT(sal_stp_mstPortPriority_set(instanceId, lp, DFLT_STP_MST_PORT_PRI), lp);
        }

        return SYS_ERR_OK;
    }

    priority = SYS_STR2UINT(argv[1]);

    if (IS_PARAM_NUM_MULTIPLE_INVALID(priority, 0, BOND_STP_PORT_PRI_MAX, 16))
        return SYS_ERR_STP_PORT_PRIORITY;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_stp_mstPortPriority_set(instanceId, logicPort, priority), logicPort);
    }

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PROTO_STP_MSTP
static int32 _cmd_stp_mst_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char name[MSTP_CONF_NAME_LEN + 1];
    char strVlanList[CAPA_VLANLIST_STR_LEN];
    uint32 val;
    uint32 msti;
    uint32 vlanCount;
    sys_vlanmask_t vlanmask;

    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "spanning-tree mst configuration\n");
    CMD_WRITE_TO_FILE(fp, cmd);

    SYS_MEM_CLEAR(name);
    SYS_ERR_CHK(sal_stp_mstConfigName_get(name));

    if (0 != osal_strcmp(DFLT_STP_MST_CFG_NAME, name))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "name \"%s\"\n", name);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_stp_mstConfigRevision_get(&val));

    if (DFLT_STP_MST_CFG_REV != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "revision %u\n", val);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    for (msti = 0; msti < MSTP_INSTANCE_NUM; msti++)
    {
        
        SYS_MEM_CLEAR(vlanmask);

        SYS_ERR_CHK(sal_stp_mstVlanmask_get(msti, &vlanmask));
        VMSK2CNT(&vlanmask, &vlanCount);

        if ((0 != msti) && (0 != vlanCount))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(strVlanList);

            SYS_ERR_CHK(VMSK2STR(&vlanmask, strVlanList));
            osal_sprintf(cmd, "instance %u vlan %s\n", msti, strVlanList);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    for (msti = 0; msti < MSTP_INSTANCE_NUM; msti++)
    {
        
        SYS_ERR_CHK(sal_stp_mstPriority_get(msti, &val));

        if (DFLT_STP_MST_PRI != val)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "spanning-tree mst %u priority %u\n", msti, val);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}
#endif 

int32 cmd_stp_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char *pStrForceVersion[] = { "stp","stp", "rstp", "mstp" };
    char *pStrPathcostMethod[] = { "long", "short" };
    uint32 val;
    uint32 i = 0;
    uint32 sequence[3] = {0,1,2};
    sys_enable_t enable;

    SYS_ERR_CHK(sal_stp_enable_get(&enable));

    if (DFLT_STP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sspanning-tree\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_stp_forwardBpdu_get(&enable));

    if (DFLT_STP_BPDU_FORWARD_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree bpdu %s\n", (ENABLED == enable) ? "flooding" : "filtering");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_stp_forceVersion_get(&val));

    if (DFLT_STP_FORCE_VERSION != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree mode %s\n", pStrForceVersion[(val & 0x3)]);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_stp_priority_get(&val));

    if (DFLT_STP_PRI != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree priority %u\n", val);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_stp_timer_sequence_get(sequence));

    for( i = 0 ; i < 3; i++)
    {
         if(STP_FORWARD_DELAY == sequence[i])
         {
            
            SYS_ERR_CHK(sal_stp_forwardDelay_get(&val));

            if (DFLT_STP_FORWARD_DELAY != val)
            {
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "spanning-tree forward-delay %u\n", val);
                CMD_WRITE_TO_FILE(fp, cmd);
            }
         }

         if(STP_MAX_AGE == sequence[i])
         {
            
            SYS_ERR_CHK(sal_stp_maximumAge_get(&val));

            if (DFLT_STP_MAX_AGE != val)
            {
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "spanning-tree maximum-age %u\n", val);
                CMD_WRITE_TO_FILE(fp, cmd);
            }
         }

         if(STP_HELLO_TIME == sequence[i])
         {
            
            SYS_ERR_CHK(sal_stp_helloTime_get(&val));

            if (DFLT_STP_HELLO_TIME != val)
            {
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "spanning-tree hello-time %u\n", val);
                CMD_WRITE_TO_FILE(fp, cmd);
            }
         }
    }

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    
    SYS_ERR_CHK(sal_stp_maxHops_get(&val));

    if (DFLT_STP_MAX_HOPS != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree max-hops %u\n", val);
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

    SYS_ERR_CHK(sal_stp_txHoldCount_get(&val));

    if (DFLT_STP_TX_HOLD_COUNT != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree tx-hold-count %u\n", val);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_stp_pathcostMethod_get(&val));

    if (DFLT_STP_PATHCOST_METHOD != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree pathcost method %s\n", pStrPathcostMethod[val]);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    
    SYS_ERR_CHK(_cmd_stp_mst_save(fp));
#endif

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP
static int32 _cmd_stp_portMst_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 val;
    uint32 msti;

    for (msti = 0; msti < MSTP_INSTANCE_NUM; msti++)
    {
        
        SYS_ERR_CHK(sal_stp_mstPortPathCost_get(msti, port, &val));

        if (DFLT_STP_MST_PORT_PATHCOST != val)
        {
            osal_memset(cmd, 0, sizeof(cmd));
            osal_sprintf(cmd, "spanning-tree mst %u cost %u\n", msti, val);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        SYS_ERR_CHK(sal_stp_mstPortPriority_get(msti, port, &val));

        if (DFLT_STP_MST_PORT_PRI != val)
        {
            osal_memset(cmd, 0, sizeof(cmd));
            osal_sprintf(cmd, "spanning-tree mst %u port-priority %u\n", msti, val);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_stp_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 val;
#if 1
    sys_enable_t enable;

    SYS_ERR_CHK(sal_stp_portAdminEnable_get(port, &enable));

    if (DFLT_STP_PORT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sspanning-tree\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_stp_portForwardBpdu_get(port, &enable));

    if (DFLT_STP_PORT_BPDU_FORWARD_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree bpdu %s\n", (ENABLED == enable) ? "flooding" : "filtering");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

    SYS_ERR_CHK(sal_stp_portPathCost_get(port, &val));

    if (DFLT_STP_PORT_PATHCOST != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree cost %u\n", val);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_stp_portPriority_get(port, &val));

    if (DFLT_STP_PORT_PRI != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree port-priority %u\n", val);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_stp_portEdgePort_get(port, &val));

    if (DFLT_STP_PORT_EDGE != val)
    {
        SYS_MEM_CLEAR(cmd);
		if (STP_PORT_CONF_FALSE == val)
	        osal_sprintf(cmd, "no spanning-tree edge\n");
		else if (STP_PORT_CONF_TRUE == val)
	        osal_sprintf(cmd, "spanning-tree edge\n");
		else
	        osal_sprintf(cmd, "spanning-tree autoedge\n");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

#ifdef CONFIG_SYS_STP_BPDU_FILTER
    
    SYS_ERR_CHK(sal_stp_portBpduFilter_get(port, &val));

    if (DFLT_STP_PORT_BPDU_FILTER != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sspanning-tree bpdu-filter\n", (STP_PORT_CONF_TRUE == val) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD
    
    SYS_ERR_CHK(sal_stp_portBpduGuard_get(port, &val));

    if (DFLT_STP_PORT_BPDU_GUARD != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sspanning-tree bpdu-guard\n", (STP_PORT_CONF_TRUE == val) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

    SYS_ERR_CHK(sal_stp_portPointToPointMac_get(port, &val));

    if (STP_PORT_CONF_AUTO != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "spanning-tree link-type");

        switch (val)
        {
            case STP_PORT_CONF_FALSE:
                osal_strcat(cmd, " shared\n");
                break;

            case STP_PORT_CONF_TRUE:
                osal_strcat(cmd, " point-to-point\n");
                break;

            default:
                break;
        }

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    
    SYS_ERR_CHK(_cmd_stp_portMst_save(fp, port));
#endif

    return SYS_ERR_OK;
}

