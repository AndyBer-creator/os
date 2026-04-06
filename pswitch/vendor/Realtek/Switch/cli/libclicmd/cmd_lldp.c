
#include <libsal/sal_lldp.h>
#include <libsal/sal_l3.h>
#include <libcmd/cmd.h>

int32 cmd_lldp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t lldp_enabled;
    uint32       tx_interval;
    uint8        holdtime;
    uint8        reinit_delay;
    uint16       tx_delay;
    lldp_disable_action_t action;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    char lldpUserPort[SYS_BUF32_LEN];
    char strLine[SYS_BUF512_LEN], strTmp[SYS_BUF256_LEN], strTmp2[SYS_BUF256_LEN];
    uint8        lldpPortStatus;
    uint32 ipaddr;
    uint32 netmask;
    char ipaddr_str[SYS_BUF32_LEN];
    lldp_tlv_sel_t lldpTlvSel;
    char strVlanList[CAPA_VLANLIST_STR_LEN];
    sys_enable_t pvidEnable;
    sys_vlanmask_t vlanmask;

    osal_memset(strLine, 0, sizeof(strLine));
    osal_memset(strTmp, 0, sizeof(strTmp));
    osal_memset(strTmp2, 0, sizeof(strTmp2));
    osal_memset(ipaddr_str, 0, sizeof(ipaddr_str));

    SYS_ERR_CHK(sal_lldp_enable_get(&lldp_enabled));
    SYS_ERR_CHK(sal_lldp_tx_interval_get(&tx_interval));
    SYS_ERR_CHK(sal_lldp_holdtime_get(&holdtime));
    SYS_ERR_CHK(sal_lldp_reinit_delay_get(&reinit_delay));
    SYS_ERR_CHK(sal_lldp_tx_delay_get(&tx_delay));
    SYS_ERR_CHK(sal_lldp_disableAction_get(&action));

    XMORE("\n");
    XMORE(" State: %s\n", lldp_enabled?"Enabled":"Disabled");
    XMORE(" Timer: %d Seconds\n", tx_interval);
    XMORE(" Hold multiplier: %d\n", holdtime);
    XMORE(" Reinit delay: %d Seconds\n", reinit_delay);
    XMORE(" Tx delay: %d Seconds\n", tx_delay);
    switch(action)
    {
        case LLDP_DISABLE_ACTION_FILTER:
            XMORE(" LLDP packet handling: Filtering\n");
            break;
        case LLDP_DISABLE_ACTION_FLOOD_TO_VLAN:
            XMORE(" LLDP packet handling: Bridging\n");
            break;
        case LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS:
            XMORE(" LLDP packet handling: Flooding\n");
            break;
        default:
            break;
    }

    XMORE("\n");
    XMORE(" Port     |  State | Optional TLVs  | Address\n");
    XMORE(" -------- + ------ + -------------- + --------\n");

    NORMAL_LOGIC_PORTMASK_SET_ALL(logicPortmask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        osal_sprintf(strLine, "%9s |", lldpUserPort);
        SYS_ERR_CHK(sal_lldp_port_get(logicPort,&lldpPortStatus));
        switch(lldpPortStatus)
        {
            case LLDP_PORT_DISABLE:
                osal_sprintf(strTmp, "%7s |", "Disable");
                break;
            case LLDP_PORT_RXONLY:
                osal_sprintf(strTmp, "%7s |", "RX");
                break;
            case LLDP_PORT_TXONLY:
                osal_sprintf(strTmp, "%7s |", "TX");
                break;
            case LLDP_PORT_TXRX:
                osal_sprintf(strTmp, "%7s |", "RX,TX");
                break;
        }
        osal_strcat(strLine, strTmp);
        SYS_ERR_CHK(sal_lldp_portTlvSel_get(logicPort, &lldpTlvSel));
        osal_memset(strTmp, 0, sizeof(strTmp));
        if (TRUE == lldpTlvSel.portDesciptionSel)
            osal_strcat(strTmp, ", PD");
        if (TRUE == lldpTlvSel.sysNameSel)
            osal_strcat(strTmp, ", SN");
        if (TRUE == lldpTlvSel.sysDescriptionSel)
            osal_strcat(strTmp, ", SD");
        if (TRUE == lldpTlvSel.sysCapabilitySel)
            osal_strcat(strTmp, ", SC");

        if (osal_strlen(strTmp) > 0)
            osal_sprintf(strTmp2, "%15s |", &strTmp[2]);
        else
            osal_sprintf(strTmp2, "%15s |", strTmp);
        osal_strcat(strLine, strTmp2);

        SYS_ERR_CHK(sal_sys_ip_get(&ipaddr, &netmask));
        SYS_ERR_CHK(sys_util_ip2Str(ipaddr, ipaddr_str));
        osal_strcat(strLine, ipaddr_str);
        XMORE("%s", strLine);
        XMORE("\n");
    }

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        XMORE("\nPort ID: %s\n", lldpUserPort);
        SYS_ERR_CHK(sal_lldp_portTlvSel_get(logicPort, &lldpTlvSel));
        SYS_ERR_CHK(sal_lldp_portTlvPvidEnable_get(logicPort, &pvidEnable));
        SYS_ERR_CHK(sal_lldp_portTlvVlanmask_get(logicPort, &vlanmask));
        osal_memset(strTmp, 0, sizeof(strTmp));
        if (TRUE == lldpTlvSel.macPhySel)
            osal_strcat(strTmp, ", 802.3-mac-phy");
        if (TRUE == lldpTlvSel.linkAggregationSel)
            osal_strcat(strTmp, ", 802.3-lag");
        if (TRUE == lldpTlvSel.maxFrameSizeSel)
            osal_strcat(strTmp, ", 802.3-max-frame-size");
        if (TRUE == lldpTlvSel.managementAddrSel)
            osal_strcat(strTmp, ", management-addr");
        if (osal_strlen(strTmp) > 0)
            XMORE("802.3 optional TLVs: %s\n", &strTmp[2]);
        else
            XMORE("802.3 optional TLVs:\n");
        XMORE("802.1 optional TLVs\n");
        XMORE("PVID: %s\n", (ENABLED == pvidEnable)?"Enabled":"Disabled");
        osal_memset(strVlanList, 0, sizeof(strVlanList));
        VMSK2STR(&vlanmask, strVlanList);
        if (osal_strlen(strVlanList) > 0)
            XMORE("VLANs: %s\n", strVlanList);
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_lldp_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t lldp_enabled;
    uint32       tx_interval;
    uint8        holdtime;
    uint8        reinit_delay;
    uint16       tx_delay;
    lldp_disable_action_t action;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    char lldpUserPort[SYS_BUF32_LEN];
    char strLine[SYS_BUF512_LEN], strTmp[SYS_BUF256_LEN], strTmp2[SYS_BUF256_LEN];
    char strVlanList[CAPA_VLANLIST_STR_LEN];
    uint8        lldpPortStatus;
    uint32 ipaddr;
    uint32 netmask;
    char ipaddr_str[SYS_BUF32_LEN];
    lldp_tlv_sel_t lldpTlvSel;
    sys_enable_t pvidEnable;
    sys_vlanmask_t vlanmask;

    osal_memset(strLine, 0, sizeof(strLine));
    osal_memset(strTmp, 0, sizeof(strTmp));
    osal_memset(strTmp2, 0, sizeof(strTmp2));
    osal_memset(ipaddr_str, 0, sizeof(ipaddr_str));

    SYS_ERR_CHK(sal_lldp_enable_get(&lldp_enabled));
    SYS_ERR_CHK(sal_lldp_tx_interval_get(&tx_interval));
    SYS_ERR_CHK(sal_lldp_holdtime_get(&holdtime));
    SYS_ERR_CHK(sal_lldp_reinit_delay_get(&reinit_delay));
    SYS_ERR_CHK(sal_lldp_tx_delay_get(&tx_delay));
    SYS_ERR_CHK(sal_lldp_disableAction_get(&action));

    XMORE("\n");
    XMORE(" State: %s\n", lldp_enabled?"Enabled":"Disabled");
    XMORE(" Timer: %d Seconds\n", tx_interval);
    XMORE(" Hold multiplier: %d\n", holdtime);
    XMORE(" Reinit delay: %d Seconds\n", reinit_delay);
    XMORE(" Tx delay: %d Seconds\n", tx_delay);
    switch(action)
    {
        case LLDP_DISABLE_ACTION_FILTER:
            XMORE(" LLDP packet handling: Filtering\n");
            break;
        case LLDP_DISABLE_ACTION_FLOOD_TO_VLAN:
            XMORE(" LLDP packet handling: Bridging\n");
            break;
        case LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS:
            XMORE(" LLDP packet handling: Flooding\n");
            break;
        default:
            break;
    }

    XMORE("\n");
    XMORE(" Port     |  State | Optional TLVs  | Address\n");
    XMORE(" -------- + ------ + -------------- + --------\n");

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        osal_sprintf(strLine, "%9s |", lldpUserPort);
        SYS_ERR_CHK(sal_lldp_port_get(logicPort,&lldpPortStatus));
        switch(lldpPortStatus)
        {
            case LLDP_PORT_DISABLE:
                osal_sprintf(strTmp, "%7s |", "Disable");
                break;
            case LLDP_PORT_RXONLY:
                osal_sprintf(strTmp, "%7s |", "RX");
                break;
            case LLDP_PORT_TXONLY:
                osal_sprintf(strTmp, "%7s |", "TX");
                break;
            case LLDP_PORT_TXRX:
                osal_sprintf(strTmp, "%7s |", "RX,TX");
                break;
        }
        osal_strcat(strLine, strTmp);
        SYS_ERR_CHK(sal_lldp_portTlvSel_get(logicPort, &lldpTlvSel));
        osal_memset(strTmp, 0, sizeof(strTmp));
        if (TRUE == lldpTlvSel.portDesciptionSel)
            osal_strcat(strTmp, ", PD");
        if (TRUE == lldpTlvSel.sysNameSel)
            osal_strcat(strTmp, ", SN");
        if (TRUE == lldpTlvSel.sysDescriptionSel)
            osal_strcat(strTmp, ", SD");
        if (TRUE == lldpTlvSel.sysCapabilitySel)
            osal_strcat(strTmp, ", SC");

        if (osal_strlen(strTmp) > 0)
            osal_sprintf(strTmp2, "%15s |", &strTmp[2]);
        else
            osal_sprintf(strTmp2, "%15s |", strTmp);
        osal_strcat(strLine, strTmp2);

        SYS_ERR_CHK(sal_sys_ip_get(&ipaddr, &netmask));
        SYS_ERR_CHK(sys_util_ip2Str(ipaddr, ipaddr_str));
        osal_strcat(strLine, ipaddr_str);
        XMORE("%s", strLine);
        XMORE("\n");
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        XMORE("\nPort ID: %s\n", lldpUserPort);
        SYS_ERR_CHK(sal_lldp_portTlvSel_get(logicPort, &lldpTlvSel));
        SYS_ERR_CHK(sal_lldp_portTlvPvidEnable_get(logicPort, &pvidEnable));
        SYS_ERR_CHK(sal_lldp_portTlvVlanmask_get(logicPort, &vlanmask));
        osal_memset(strTmp, 0, sizeof(strTmp));
        if (TRUE == lldpTlvSel.macPhySel)
            osal_strcat(strTmp, ", 802.3-mac-phy");
        if (TRUE == lldpTlvSel.linkAggregationSel)
            osal_strcat(strTmp, ", 802.3-lag");
        if (TRUE == lldpTlvSel.maxFrameSizeSel)
            osal_strcat(strTmp, ", 802.3-max-frame-size");
        if (TRUE == lldpTlvSel.managementAddrSel)
            osal_strcat(strTmp, ", management-addr");
        if (osal_strlen(strTmp) > 0)
            XMORE("802.3 optional TLVs: %s\n", &strTmp[2]);
        else
            XMORE("802.3 optional TLVs:\n");
        XMORE("802.1 optional TLVs\n");
        XMORE("PVID: %s\n", (ENABLED == pvidEnable)?"Enabled":"Disabled");
        osal_memset(strVlanList, 0, sizeof(strVlanList));
        VMSK2STR(&vlanmask, strVlanList);
        if (osal_strlen(strVlanList) > 0)
            XMORE("VLANs: %s\n", strVlanList);
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_lldp_local_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_mac_t macaddr;
    char macaddr_str[CAPA_MAC_STR_LEN];
    display_entry_t lldp_entry;
    sys_l3_intfMgmt_t intfMgmt;
    char ipaddr_str[CAPA_IPV6_STR_LEN];
#ifdef CONFIG_SYS_IPV6
    uint32 i;
#endif

    SYS_ERR_CHK(sal_l3_intfMgmt_get(&intfMgmt));
    SYS_ERR_CHK(sal_sys_macAddr_get(&macaddr));
    SYS_ERR_CHK(sys_util_mac2Str(macaddr.octet, macaddr_str));

    IPV42STR(intfMgmt.ipv4.addr, ipaddr_str);

    SYS_ERR_CHK(sal_lldp_portLocalEntryInfo_get(0, &lldp_entry));
    XMORE("\n");
    XMORE("LLDP Local Device Information:\n");
    XMORE(" Chassis Type : Mac Address\n");
    XMORE(" Chassis ID   : %s\n",macaddr_str);
    XMORE(" System Name  : %s\n",lldp_entry.system_name);
    XMORE(" System Description  : %s\n",lldp_entry.system_description);
#ifdef CONFIG_SYS_L3_ROUTE
    XMORE(" System Capabilities Support : Bridge, Router\n");
    XMORE(" System Capabilities Enable  : Bridge, Router\n");
#else
    XMORE(" System Capabilities Support : Bridge \n");
    XMORE(" System Capabilities Enable  : Bridge \n");
#endif
    XMORE(" Management Address : %s(IPv4) \n",ipaddr_str);
#ifdef CONFIG_SYS_IPV6
    for (i = 0; i < CAPA_L3_INTFM_IPV6STATUS_NUM; i++)
    {
        if (IS_IPV6_ALLZERO(intfMgmt.ipv6[i].addr))
            continue;

        SYS_MEM_CLEAR(ipaddr_str);
        IPV62STR(intfMgmt.ipv6[i].addr, ipaddr_str);
        XMORE(" Management Address : %s(IPv6) \n",ipaddr_str);
    }
#endif
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_lldp_portLocal_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
#ifdef CONFIG_SYS_PROTO_LLDP_MED
    uint8 idx;
    display_med_entry_t lldpMed_entry;
#endif
    display_entry_t lldp_entry;
    char lldpUserPort[CAPA_PORT_STR_LEN];
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    sys_vid_t vid, lastVid;
    sys_namestr_t name;
    uint8 i;
    uint8 strIpAddr[CAPA_IPV6_STR_LEN];

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        osal_memset(&lldp_entry, 0, sizeof(display_entry_t));
        SYS_ERR_CHK(sal_lldp_portLocalEntryInfo_get(logicPort, &lldp_entry));

        if ((lldp_entry.logicPort >= 0) && (lldp_entry.logicPort <= SYS_LOGICPORT_NUM_MAX)
          && ( osal_strlen(lldp_entry.chasis_id.id) != 0 ))
        {
#ifdef CONFIG_SYS_PROTO_LLDP_MED
            SYS_ERR_CHK(sal_lldp_portMedLocalEntryInfo_get(logicPort, &lldpMed_entry));
#endif

            SYS_ERR_CONTINUE(LP2STR(lldp_entry.logicPort,lldpUserPort));

            XMORE("\nDevice ID: %s\n", lldp_entry.chasis_id.id);
            XMORE("Port ID: %s\n", lldpUserPort);
            XMORE("System Name: %s\n", lldp_entry.system_name);
            XMORE("Capabilities: %s\n", lldp_entry.system_capabilities.enable);
            XMORE("System description: %s\n", lldp_entry.system_description);
            XMORE("Port description: %s\n", lldp_entry.port_description);
            for (i = 0; i < MAX_LLDP_MANAGEMENT_ADDR_NUM; i ++)
            {
                if (TRUE == lldp_entry.manage_address[i].valid)
                {
                    sal_lldp_managementAddressStr_get(lldp_entry.manage_address[i], strIpAddr);
                    XMORE("Management address: %s\n", strIpAddr);
                }
            }
            XMORE("Time To Live: %d\n", lldp_entry.lldp_ttl);

            if (TRUE == lldp_entry.macPhy.valid)
            {
                XMORE("802.3 MAC/PHY Configur/Status\n");
                XMORE("Auto-negotiation support: %s\n", lldp_entry.macPhy.support?"Supported":"Not Supported");
                XMORE("Auto-negotiation status: %s\n", lldp_entry.macPhy.enable?"Enabled":"Disabled");
                XMORE("Auto-negotiation Advertised Capabilities: %s\n", lldp_entry.macPhy.capa);
                XMORE("Operational MAU type: %s\n", lldp_entry.macPhy.oper);
            }

            if (TRUE == lldp_entry.trunk.valid)
            {
                XMORE("802.3 Link Aggregation\n");
                XMORE("Aggregation capability: %s\n", lldp_entry.trunk.capa);
                XMORE("Aggregation status: %s\n", lldp_entry.trunk.state);
                XMORE("Aggregation port ID: %d\n", lldp_entry.trunk.id);
            }
            if (TRUE == lldp_entry.maxFrameSize.valid)
                XMORE("802.3 Maximum Frame Size: %d\n", lldp_entry.maxFrameSize.maxFrameSize);

            if (TRUE == lldp_entry.pvid.valid)
            {
                XMORE("802.1 PVID: %d\n", lldp_entry.pvid.pvid);
            }

            {
                char strVlanList[CAPA_VLANLIST_STR_LEN];
                sys_vlanmask_t vlanmask;

                SYS_ERR_CHK(sal_lldp_portTlvVlanmask_get(logicPort, &vlanmask));
                VMSK2STR(&vlanmask, strVlanList);
                if (osal_strlen(strVlanList) > 0)
                {
                    XMORE("802.1 VLAN: %s\n", strVlanList);
                    osal_printf("802.1 VLAN name: ");
                    lastVid = 1;
                    FOR_EACH_VID_IN_VLANMASK(vid,vlanmask)
                    {
                        lastVid = vid;
                    }

                    FOR_EACH_VID_IN_VLANMASK(vid,vlanmask)
                    {
                        SYS_ERR_CHK(sal_vlan_name_get(vid, &name));
                        osal_printf("%d(%s)", vid, name.str);
                        if (vid < lastVid)
                            osal_printf(",");
                    }
                    XMORE("\n");
                }
            }

#ifdef CONFIG_SYS_PROTO_LLDP_MED
            if (TRUE == lldpMed_entry.medCapa.valid)
            {
                XMORE("LLDP-MED capabilities: %s\n", lldpMed_entry.medCapa.capa);
                XMORE("LLDP-MED Device type: %s\n", lldpMed_entry.medCapa.devType);
            }
            for (idx = 0; idx < MAX_LLDP_MED_NETWORK_POLICY_RX; idx ++)
            {
                if (lldpMed_entry.networkPolicy[idx].valid)
                {
                    XMORE("LLDP-MED Network policy\n");
                    XMORE("Application type: %s\n", lldpMed_entry.networkPolicy[idx].type);
                    XMORE("Flags: %s\n", lldpMed_entry.networkPolicy[idx].flag);
                    XMORE("VLAN ID: %d\n", lldpMed_entry.networkPolicy[idx].vid);
                    XMORE("Layer 2 priority: %d\n", lldpMed_entry.networkPolicy[idx].l2Pri);
                    XMORE("DSCP: %d\n", lldpMed_entry.networkPolicy[idx].dscpPri);
                }
            }
            if (TRUE == lldpMed_entry.hardware_v.valid)
                XMORE("Hardware revision: %s\n", lldpMed_entry.hardware_v.strInventory);
            if (TRUE == lldpMed_entry.firmware_v.valid)
                XMORE("Firmware revision: %s\n", lldpMed_entry.firmware_v.strInventory);
            if (TRUE == lldpMed_entry.software_v.valid)
                XMORE("Software revision: %s\n", lldpMed_entry.software_v.strInventory);
            if (TRUE == lldpMed_entry.serial_num.valid)
                XMORE("Serial number: %s\n", lldpMed_entry.serial_num.strInventory);
            if (TRUE == lldpMed_entry.manufacturer.valid)
                XMORE("Manufacturer Name: %s\n", lldpMed_entry.manufacturer.strInventory);
            if (TRUE == lldpMed_entry.model_name.valid)
                XMORE("Model name: %s\n", lldpMed_entry.model_name.strInventory);
            if (TRUE == lldpMed_entry.asset.valid)
                XMORE("Asset ID: %s\n", lldpMed_entry.asset.strInventory);

            if (lldpMed_entry.locationCoor.valid || lldpMed_entry.locationCivic.valid || lldpMed_entry.locationEcs.valid)
            {
                XMORE("LLDP-MED Location\n");
                if (lldpMed_entry.locationCoor.valid)
                    XMORE("Coordinates: %s\n", lldpMed_entry.locationCoor.data);
                if (lldpMed_entry.locationCivic.valid)
                    XMORE("Civic-address: %s\n", lldpMed_entry.locationCivic.data);
                if (lldpMed_entry.locationEcs.valid)
                    XMORE("Ecs-elin: %s\n", lldpMed_entry.locationEcs.data);
            }
#endif
            XMORE("\n");
        }
    }
    return SYS_ERR_OK;

}

int32 cmd_lldp_remote_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 i ;
    display_entry_t lldp_entry;
    char lldpUserPort[CAPA_PORT_STR_LEN];
    sys_logic_port_t       logicPort;
    sys_logic_portmask_t   logicPortmask;

    XMORE("\n");
    XMORE(" Port |   Device ID       |     Port ID      |      SysName      |  Capabilities  |  TTL \n");
    XMORE(" ---- + ----------------- + ---------------- + ----------------- + -------------- + -----\n");

    NORMAL_LOGIC_PORTMASK_SET_ALL(logicPortmask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        for (i = 0 ;i < LLDP_MAX_ENTRY_PER_PORT; i++)
        {
            osal_memset(&lldp_entry, 0, sizeof(display_entry_t));
            SYS_ERR_CHK(sal_lldp_portRemoteEntryInfo_get(logicPort, i, &lldp_entry));
            if ((lldp_entry.logicPort >= 0) && (lldp_entry.logicPort <= SYS_LOGICPORT_NUM_MAX)
                && ( osal_strlen(lldp_entry.chasis_id.id) != 0 ))
            {
                SYS_ERR_CONTINUE(LP2STR(lldp_entry.logicPort,lldpUserPort));
                
                XMORE("%5s |", lldpUserPort);
                
                XMORE("%18s |", lldp_entry.chasis_id.id);
                
                XMORE("%17s |", lldp_entry.port_id.id);
                
                XMORE("%18s |", lldp_entry.system_name);
                
                XMORE("%15s |", lldp_entry.system_capabilities.enable);
                
                XMORE("%6d\n", lldp_entry.lldp_ttl);
            }
        }
    }
    XMORE("\n");
    return SYS_ERR_OK;
}

int32 cmd_lldp_portRemote_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 i, j;
    display_entry_t lldp_entry;
    char lldpUserPort[CAPA_PORT_STR_LEN];
    uint8 strIpAddr[CAPA_IPV6_STR_LEN];
    sys_logic_port_t       logicPort;
    sys_logic_portmask_t   logicPortmask;
    lldp_vlanName_tlv_t vlanTlv;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    uint8 idx;
    display_med_entry_t lldpMed_entry;
  #endif

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        for (i = 0 ;i < LLDP_MAX_ENTRY_PER_PORT; i++)
        {
            osal_memset(&lldp_entry, 0, sizeof(display_entry_t));
            SYS_ERR_CHK(sal_lldp_portRemoteEntryInfo_get(logicPort, i, &lldp_entry));
            if ((lldp_entry.logicPort >= 0) && (lldp_entry.logicPort <= SYS_LOGICPORT_NUM_MAX)
                && ( osal_strlen(lldp_entry.chasis_id.id) != 0 ))
            {
              #ifdef CONFIG_SYS_PROTO_LLDP_MED
                SYS_ERR_CHK(sal_lldp_portMedRemoteEntryInfo_get(logicPort, i, &lldpMed_entry));
              #endif

                SYS_ERR_CONTINUE(LP2STR(lldp_entry.logicPort,lldpUserPort));

                XMORE("\nDevice ID: %s\n", lldp_entry.chasis_id.id);
                XMORE("Port ID: %s\n", lldp_entry.port_id.id);
                XMORE("System Name: %s\n", lldp_entry.system_name);
                XMORE("Capabilities: %s\n", lldp_entry.system_capabilities.enable);
                XMORE("System description: %s\n", lldp_entry.system_description);
                XMORE("Port description: %s\n", lldp_entry.port_description);
                for (j = 0; j < MAX_LLDP_MANAGEMENT_ADDR_NUM; j ++)
                {
                    if (TRUE == lldp_entry.manage_address[j].valid)
                    {
                        sal_lldp_managementAddressStr_get(lldp_entry.manage_address[j], strIpAddr);
                        XMORE("Management address: %s\n", strIpAddr);
                    }
                }
                XMORE("Time To Live: %d\n", lldp_entry.lldp_ttl);
                if (TRUE == lldp_entry.macPhy.valid)
                {
                    XMORE("802.3 MAC/PHY Configur/Status\n");
                    XMORE("Auto-negotiation support: %s\n", lldp_entry.macPhy.support?"Supported":"Not Supported");
                    XMORE("Auto-negotiation status: %s\n", lldp_entry.macPhy.enable?"Enabled":"Disabled");
                    XMORE("Auto-negotiation Advertised Capabilities: %s\n", lldp_entry.macPhy.capa);
                    XMORE("Operational MAU type: %s\n", lldp_entry.macPhy.oper);
                }
                if (TRUE == lldp_entry.dot3power.valid)
                {
                    XMORE("802.3 Power via MDI\n");
                    XMORE("MDI Power support Port Class: %s\n", lldp_entry.dot3power.portClass);
                    XMORE("PSE MDI Power Support: %s\n", lldp_entry.dot3power.powerSupport);
                    XMORE("PSE MDI Power State: %s\n", lldp_entry.dot3power.powerState);
                    XMORE("PSE power pair control ability: %s\n", lldp_entry.dot3power.pairControlAbility);
                    XMORE("PSE Power Pair: %s\n", lldp_entry.dot3power.powerPair);
                    XMORE("PSE Power class: %s\n", lldp_entry.dot3power.powerClass);
                    if (TRUE == lldp_entry.dot3power.extValid)
                    {
                        XMORE("Device Type: %s\n", lldp_entry.dot3power.devType);
                        XMORE("Power Source: %s\n", lldp_entry.dot3power.src);
                        XMORE("Power priority: %s\n", lldp_entry.dot3power.pri);
                        XMORE("PD Requested Power value: %d.%d Watts\n", lldp_entry.dot3power.requestPower / 10, lldp_entry.dot3power.requestPower % 10);
                        XMORE("PSE Allocated Power value: %d.%d Watts\n", lldp_entry.dot3power.allocatePower / 10, lldp_entry.dot3power.allocatePower % 10);
                    }
                    else
                    {
                        XMORE("Device Type: N/A\n");
                        XMORE("Power Source: N/A\n");
                        XMORE("Power priority: N/A\n");
                        XMORE("PD Requested Power value: N/A\n");
                        XMORE("PSE Allocated Power value: N/A\n");
                    }
                }
                if (TRUE == lldp_entry.trunk.valid)
                {
                    XMORE("802.3 Link Aggregation\n");
                    XMORE("Aggregation capability: %s\n", lldp_entry.trunk.capa);
                    XMORE("Aggregation status: %s\n", lldp_entry.trunk.state);
                    XMORE("Aggregation port ID: %d\n", lldp_entry.trunk.id);
                }
                if (TRUE == lldp_entry.maxFrameSize.valid)
                    XMORE("802.3 Maximum Frame Size: %d\n", lldp_entry.maxFrameSize.maxFrameSize);
              #if 0
                XMORE("\n802.3 EEE\n");
                XMORE("Remote Tx: %d usec\n", 0);
                XMORE("Remote Rx: %d usec\n", 0);
                XMORE("Local Tx Echo: %s usec\n", "");
                XMORE("Local Rx Echo: %s usec\n", "");
              #endif

                if (TRUE == lldp_entry.pvid.valid)
                    XMORE("802.1 PVID: %d\n", lldp_entry.pvid.pvid);

                if (lldp_entry.vlanNum > 0)
                {
                    osal_printf("802.1 VLAN: ");

                    for (i = 0; i < lldp_entry.vlanNum; i ++)
                    {
                        sal_lldp_portRemoteVlanNameTlv_get(logicPort, i, &vlanTlv);

                        if (TRUE == vlanTlv.valid)
                        {
                            osal_printf("%d(%s)", vlanTlv.vid, vlanTlv.name.str);
                            if (i != lldp_entry.vlanNum - 1)
                                osal_printf(",");
                        }
                    }
                    XMORE("\n");
                }

              #ifdef CONFIG_SYS_PROTO_LLDP_MED
                if (TRUE == lldpMed_entry.medCapa.valid)
                {
                    XMORE("LLDP-MED capabilities: %s\n", lldpMed_entry.medCapa.capa);
                    XMORE("LLDP-MED Device type: %s\n", lldpMed_entry.medCapa.devType);
                }
                for (idx = 0; idx < MAX_LLDP_MED_NETWORK_POLICY_RX; idx ++)
                {
                    if (lldpMed_entry.networkPolicy[idx].valid)
                    {
                        XMORE("LLDP-MED Network policy\n");
                        XMORE("Application type: %s\n", lldpMed_entry.networkPolicy[idx].type);
                        XMORE("Flags: %s\n", lldpMed_entry.networkPolicy[idx].flag);
                        XMORE("VLAN ID: %d\n", lldpMed_entry.networkPolicy[idx].vid);
                        XMORE("Layer 2 priority: %d\n", lldpMed_entry.networkPolicy[idx].l2Pri);
                        XMORE("DSCP: %d\n", lldpMed_entry.networkPolicy[idx].dscpPri);
                    }
                }
                if (TRUE == lldpMed_entry.power.valid)
                {
                    XMORE("LLDP-MED Power over Ethernet\n");
                    XMORE("Device Type: %s\n", lldpMed_entry.power.devType);
                    XMORE("Power Source: %s\n", lldpMed_entry.power.src);
                    XMORE("Power priority: %s\n", lldpMed_entry.power.pri);
                    XMORE("Power value: %d.%d Watts\n", lldpMed_entry.power.powerVal / 10, lldpMed_entry.power.powerVal % 10);
                }
                if (TRUE == lldpMed_entry.hardware_v.valid)
                    XMORE("Hardware revision: %s\n", lldpMed_entry.hardware_v.strInventory);
                if (TRUE == lldpMed_entry.firmware_v.valid)
                    XMORE("Firmware revision: %s\n", lldpMed_entry.firmware_v.strInventory);
                if (TRUE == lldpMed_entry.software_v.valid)
                    XMORE("Software revision: %s\n", lldpMed_entry.software_v.strInventory);
                if (TRUE == lldpMed_entry.serial_num.valid)
                    XMORE("Serial number: %s\n", lldpMed_entry.serial_num.strInventory);
                if (TRUE == lldpMed_entry.manufacturer.valid)
                    XMORE("Manufacturer Name: %s\n", lldpMed_entry.manufacturer.strInventory);
                if (TRUE == lldpMed_entry.model_name.valid)
                    XMORE("Model name: %s\n", lldpMed_entry.model_name.strInventory);
                if (TRUE == lldpMed_entry.asset.valid)
                    XMORE("Asset ID: %s\n", lldpMed_entry.asset.strInventory);
                if (lldpMed_entry.locationCoor.valid || lldpMed_entry.locationCivic.valid || lldpMed_entry.locationEcs.valid)
                {
                    XMORE("LLDP-MED Location\n");
                    if (lldpMed_entry.locationCoor.valid)
                        XMORE("Coordinates: %s\n", lldpMed_entry.locationCoor.data);
                    if (lldpMed_entry.locationCivic.valid)
                        XMORE("Civic-address: %s\n", lldpMed_entry.locationCivic.data);
                    if (lldpMed_entry.locationEcs.valid)
                        XMORE("Ecs-elin: %s\n", lldpMed_entry.locationEcs.data);
                }
              #endif
                XMORE("\n");
            }
        }
    }
    return SYS_ERR_OK;
}

int32 cmd_lldp_globalStatistics_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_lldp_globalStatistics_clear());

    return SYS_ERR_OK;
}

int32 cmd_lldp_portStatistics_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_logic_portmask_t logicPortmask;

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, logicPortmask)
        CMD_ERR_CONTINUE_PORT(sal_lldp_portStatistics_clear(lp), lp);

    return SYS_ERR_OK;
}

int32 cmd_lldp_statistics_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    lldp_statistic_t lldp_stat;
    sys_logic_port_t logicPort;
    char lldpUserPort[CAPA_PORT_STR_LEN];

    osal_memset(&lldp_stat, 0, sizeof(lldp_stat));
    SYS_ERR_CHK(sal_lldp_statistic_get(&lldp_stat));
    XMORE("\n");
    XMORE("LLDP Global Statistics:\n");
    XMORE("Insertions : %d\n",lldp_stat.insertion);
    XMORE("Deletions  : %d\n",lldp_stat.deletion);
    XMORE("Drops      : %d\n",lldp_stat.drop);
    XMORE("Age Outs   : %d\n",lldp_stat.age_out);

    XMORE("\n");
    XMORE("        | TX Frames |         RX Frames          |          RX TLVs         | RX Ageouts\n");
    XMORE("  Port  |   Total   | Total | Discarded | Errors | Discarded | Unrecognized |    Total \n");
    XMORE("--------+-----------+-------+-----------+--------+-----------+--------------+----------\n");
    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        XMORE("%7s |",lldpUserPort);
        XMORE("%10d |",lldp_stat.port_statistics[logicPort].tx_frame);
        XMORE("%6d |",lldp_stat.port_statistics[logicPort].rx_frame);
        XMORE("%10d |",lldp_stat.port_statistics[logicPort].rx_discard_frame);
        XMORE("%7d |",lldp_stat.port_statistics[logicPort].rx_error_frame);
        XMORE("%10d |",lldp_stat.port_statistics[logicPort].rx_discard_tlv);
        XMORE("%13d |",lldp_stat.port_statistics[logicPort].rx_unRecognize_tlv);
        XMORE("%9d ",lldp_stat.port_statistics[logicPort].rx_ageOut);
        XMORE("\n");
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_lldp_portStatistics_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t        logicPort;
    sys_logic_portmask_t    logicPortmask;
    char lldpUserPort[CAPA_PORT_STR_LEN];
    lldp_statistic_t lldp_stat;

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    osal_memset(&lldp_stat, 0, sizeof(lldp_stat));
    SYS_ERR_CHK(sal_lldp_statistic_get(&lldp_stat));

    XMORE("\n");
    XMORE("LLDP Port Statistics:\n");
    XMORE("        | TX Frames |         RX Frames          |          RX TLVs         | RX Ageouts\n");
    XMORE("  Port  |   Total   | Total | Discarded | Errors | Discarded | Unrecognized |    Total \n");
    XMORE("--------+-----------+-------+-----------+--------+-----------+--------------+----------\n");
    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        XMORE("%7s |",lldpUserPort);
        XMORE("%10d |",lldp_stat.port_statistics[logicPort].tx_frame);
        XMORE("%6d |",lldp_stat.port_statistics[logicPort].rx_frame);
        XMORE("%10d |",lldp_stat.port_statistics[logicPort].rx_discard_frame);
        XMORE("%7d |",lldp_stat.port_statistics[logicPort].rx_error_frame);
        XMORE("%10d |",lldp_stat.port_statistics[logicPort].rx_discard_tlv);
        XMORE("%13d |",lldp_stat.port_statistics[logicPort].rx_unRecognize_tlv);
        XMORE("%9d ",lldp_stat.port_statistics[logicPort].rx_ageOut);
        XMORE("\n");
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_lldp_portTlvOverload_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t        logicPort;
    sys_logic_portmask_t    logicPortmask;
    char lldpUserPort[CAPA_PORT_STR_LEN], lineStr[SYS_BUF256_LEN];
    lldp_overLoading_t overload;

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    XMORE("\n");

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        XMORE("%s:\n\n",lldpUserPort);

        XMORE("          TLVs Group          |  Bytes  |      Status    \n");
        XMORE(" ---------------------------- + ------- + ----------------\n");

        SYS_ERR_CHK(sal_lldp_portTlvOverload_get(logicPort, &overload));
        if (overload.mandatory.byte > 0)
        {
            osal_sprintf(lineStr, "%29s |%8d |%15s", "Mandatory", overload.mandatory.byte,
                overload.mandatory.isOverload?"Overloading":"Transmitted");
            XMORE("%s\n", lineStr);
        }
      #ifdef CONFIG_SYS_PROTO_LLDP_MED
        if (overload.medCapability.byte > 0)
        {
            osal_sprintf(lineStr, "%29s |%8d |%15s", "LLDP-MED Capabilities", overload.medCapability.byte,
                overload.medCapability.isOverload?"Overloading":"Transmitted");
            XMORE("%s\n", lineStr);
        }
        if (overload.medLocation.byte > 0)
        {
            osal_sprintf(lineStr, "%29s |%8d |%15s", "LLDP-MED Location", overload.medLocation.byte,
                overload.medLocation.isOverload?"Overloading":"Transmitted");
            XMORE("%s\n", lineStr);
        }
        if (overload.medPolicy.byte > 0)
        {
            osal_sprintf(lineStr, "%29s |%8d |%15s", "LLDP-MED Network Policies", overload.medPolicy.byte,
                overload.medPolicy.isOverload?"Overloading":"Transmitted");
            XMORE("%s\n", lineStr);
        }
        if (overload.medPoe.byte > 0)
        {
            osal_sprintf(lineStr, "%29s |%8d |%15s", "LLDP-MED POE", overload.medPoe.byte,
                overload.medPoe.isOverload?"Overloading":"Transmitted");
            XMORE("%s\n", lineStr);
        }
      #endif
        if (overload.tlv802dot3.byte > 0)
        {
            osal_sprintf(lineStr, "%29s |%8d |%15s", "802.3", overload.tlv802dot3.byte,
                overload.tlv802dot3.isOverload?"Overloading":"Transmitted");
            XMORE("%s\n", lineStr);
        }
        if (overload.optional.byte > 0)
        {
            osal_sprintf(lineStr, "%29s |%8d |%15s", "Optional", overload.optional.byte,
                overload.optional.isOverload?"Overloading":"Transmitted");
            XMORE("%s\n", lineStr);
        }
      #ifdef CONFIG_SYS_PROTO_LLDP_MED
        if (overload.medInventory.byte > 0)
        {
            osal_sprintf(lineStr, "%29s |%8d |%15s", "LLDP-MED Inventory", overload.medInventory.byte,
                overload.medInventory.isOverload?"Overloading":"Transmitted");
            XMORE("%s\n", lineStr);
        }
      #endif
        if (overload.tlv802dot1.byte > 0)
        {
            osal_sprintf(lineStr, "%29s |%8d |%15s", "802.1", overload.tlv802dot1.byte,
                overload.tlv802dot1.isOverload?"Overloading":"Transmitted");
            XMORE("%s\n", lineStr);
        }
        XMORE("\n");
        XMORE("Total: %d bytes\n", overload.totalSize);
        XMORE("Left: %d bytes\n", overload.leftSize);
        XMORE("\n");
        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_lldp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t fcEnable;

    if (CMD_IS_NO_FORM_EXIST())
        fcEnable = DISABLED;
    else
        fcEnable = ENABLED;

    SYS_ERR_CHK(sal_lldp_enable_set(fcEnable));

    return SYS_ERR_OK;
}

int32 cmd_lldp_transmit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{

    sys_logic_port_t logicPort;
    uint8 lldpPortState;
    uint8 isNo;

    if (CMD_IS_NO_FORM_EXIST())
        isNo = TRUE;
    else
        isNo = FALSE;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lldp_port_get(logicPort, &lldpPortState), logicPort);
        switch(lldpPortState)
        {
            case LLDP_PORT_DISABLE:
                if (TRUE == isNo)
                    lldpPortState = LLDP_PORT_DISABLE;
                else
                    lldpPortState = LLDP_PORT_TXONLY;
                break;
            case LLDP_PORT_RXONLY:
                if (TRUE == isNo)
                    lldpPortState = LLDP_PORT_RXONLY;
                else
                    lldpPortState = LLDP_PORT_TXRX;
                break;
            case LLDP_PORT_TXONLY:
                if (TRUE == isNo)
                    lldpPortState = LLDP_PORT_DISABLE;
                else
                    lldpPortState = LLDP_PORT_TXONLY;
                break;
            case LLDP_PORT_TXRX:
                if (TRUE == isNo)
                    lldpPortState = LLDP_PORT_RXONLY;
                else
                    lldpPortState = LLDP_PORT_TXRX;
                break;
        }
        CMD_ERR_CONTINUE_PORT(sal_lldp_port_set(logicPort, lldpPortState), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_lldp_receive_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{

    sys_logic_port_t logicPort;
    uint8 lldpPortState;
    uint8 isNo;

    if (CMD_IS_NO_FORM_EXIST())
        isNo = TRUE;
    else
        isNo = FALSE;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lldp_port_get(logicPort, &lldpPortState), logicPort);
        switch(lldpPortState)
        {
            case LLDP_PORT_DISABLE:
                if (TRUE == isNo)
                    lldpPortState = LLDP_PORT_DISABLE;
                else
                    lldpPortState = LLDP_PORT_RXONLY;
                break;
            case LLDP_PORT_RXONLY:
                if (TRUE == isNo)
                    lldpPortState = LLDP_PORT_DISABLE;
                else
                    lldpPortState = LLDP_PORT_RXONLY;
                break;
            case LLDP_PORT_TXONLY:
                if (TRUE == isNo)
                    lldpPortState = LLDP_PORT_TXONLY;
                else
                    lldpPortState = LLDP_PORT_TXRX;
                break;
            case LLDP_PORT_TXRX:
                if (TRUE == isNo)
                    lldpPortState = LLDP_PORT_TXONLY;
                else
                    lldpPortState = LLDP_PORT_TXRX;
                break;
        }
        CMD_ERR_CONTINUE_PORT(sal_lldp_port_set(logicPort, lldpPortState), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_lldp_holdtime(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 holdtime_set;

    if (CMD_IS_NO_FORM_EXIST())
        holdtime_set = DFLT_LLDP_HOLDTIME_MUL;
    else
        holdtime_set = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_lldp_holdtime_set(holdtime_set));

    return SYS_ERR_OK;
}

int32 cmd_lldp_tx_interval(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 tx_interval_set;

    if (CMD_IS_NO_FORM_EXIST())
        tx_interval_set = DFLT_LLDP_TX_INTERVAL;
    else
        tx_interval_set = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_lldp_tx_interval_set(tx_interval_set));
    return SYS_ERR_OK;
}

int32 cmd_lldp_reinit_delay(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 reinit_delay_set;

    if (CMD_IS_NO_FORM_EXIST())
        reinit_delay_set = DFLT_LLDP_REINIT_DELAY;
    else
        reinit_delay_set = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_lldp_reinit_delay_set(reinit_delay_set));
    return SYS_ERR_OK;
}

int32 cmd_lldp_tx_delay(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint16 tx_delay_set;

    if (CMD_IS_NO_FORM_EXIST())
        tx_delay_set = DFLT_LLDP_TX_DELAY;
    else
        tx_delay_set = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_lldp_tx_delay_set(tx_delay_set));
    return SYS_ERR_OK;
}

int32 cmd_lldp_optional_tlv_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_logic_port_t port;
    lldp_tlv_sel_t lldpTlvSel;

    osal_memset(&lldpTlvSel, 0, sizeof(lldpTlvSel));

    if (CMD_IS_NO_FORM_EXIST())
    {
        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
        {
            CMD_ERR_CONTINUE_PORT(sal_lldp_portTlvSel_set(port, lldpTlvSel), port);

            CMD_ERR_CONTINUE_PORT(sal_lldp_txOverload_check(port), port);
        }
        return SYS_ERR_OK;
    }

    for (i = 0; i < argc; i++)
    {
        if (0 == osal_strcmp(argv[i], "port-desc"))
            lldpTlvSel.portDesciptionSel = TRUE;
        else if (0 == osal_strcmp(argv[i], "sys-name"))
            lldpTlvSel.sysNameSel = TRUE;
        else if (0 == osal_strcmp(argv[i], "sys-desc"))
            lldpTlvSel.sysDescriptionSel = TRUE;
        else if (0 == osal_strcmp(argv[i], "sys-cap"))
            lldpTlvSel.sysCapabilitySel = TRUE;
        else if (0 == osal_strcmp(argv[i], "mac-phy"))
            lldpTlvSel.macPhySel = TRUE;
        else if (0 == osal_strcmp(argv[i], "lag"))
            lldpTlvSel.linkAggregationSel = TRUE;
        else if (0 == osal_strcmp(argv[i], "max-frame-size"))
            lldpTlvSel.maxFrameSizeSel = TRUE;
        else if (0 == osal_strcmp(argv[i], "management-addr"))
            lldpTlvSel.managementAddrSel = TRUE;
        else
        {
            XMORE("%s is not a valid optional TLV\n", argv[i]);
            return SYS_ERR_OK;
        }
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lldp_portTlvSel_set(port, lldpTlvSel), port);

        CMD_ERR_CONTINUE_PORT(sal_lldp_txOverload_check(port), port);
    }
    return SYS_ERR_OK;
}

int32 cmd_lldp_optional_tlv_pvid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DFLT_LLDP_PORT_PVID_TLV_EBL;
    else
    {
        if (osal_strcmp(argv[0],"enable") == 0){
            enable = ENABLED;
        }else if (osal_strcmp(argv[0],"disable") == 0){
            enable = DISABLED;
        }
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lldp_portTlvPvidEnable_set(logicPort, enable), logicPort);

        CMD_ERR_CONTINUE_PORT(sal_lldp_txOverload_check(logicPort), logicPort);
    }

    return CMD_SUCCESS;
}

int32 cmd_lldp_optional_tlv_vlan_name_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isAdd = TRUE;
    sys_logic_port_t logicPort;
    sys_vlanmask_t vlanmask, vlanmaskModify, vlanmaskNew;

    if (osal_strcmp(argv[0],"add") == 0){
        isAdd = TRUE;
    }else if (osal_strcmp(argv[0],"remove") == 0){
        isAdd = FALSE;
    }

    SYS_ERR_CHK(STR2VMSK(argv[1], &vlanmaskModify));

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lldp_portTlvVlanmask_get(logicPort, &vlanmask), logicPort);
        if (isAdd == TRUE)
            VLANMASK_OR(vlanmaskNew, vlanmask, vlanmaskModify);
        else
            VLANMASK_ANDNOT(vlanmaskNew, vlanmask, vlanmaskModify);
        CMD_ERR_CONTINUE_PORT(sal_lldp_portTlvVlanmask_set(logicPort, vlanmaskNew), logicPort);

        CMD_ERR_CONTINUE_PORT(sal_lldp_txOverload_check(logicPort), logicPort);
    }

    return CMD_SUCCESS;
}

int32 cmd_lldp_lldpdu_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    lldp_disable_action_t action = LLDP_DISABLE_ACTION_FILTER;

    if (CMD_IS_NO_FORM_EXIST())
    {
        action = DFLT_LLDP_DISABLE_ACTION;
    }
    else
    {
        if (0 == osal_strcmp(argv[0], "filtering"))
            action = LLDP_DISABLE_ACTION_FILTER;
        else if (0 == osal_strcmp(argv[0], "bridging"))
            action = LLDP_DISABLE_ACTION_FLOOD_TO_VLAN;
        else if (0 == osal_strcmp(argv[0], "flooding"))
            action = LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS;
    }

    SYS_ERR_CHK(sal_lldp_disableAction_set(action));

    return SYS_ERR_OK;
}

int32 cmd_lldp_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint8 val = 0;
    uint16 val2 = 0;
    uint32 val3 = 0;
    sys_enable_t enable;
    lldp_disable_action_t action;

    SYS_ERR_CHK(sal_lldp_enable_get(&enable));

    if (DFLT_LLDP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%slldp\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_lldp_holdtime_get(&val));

    if (DFLT_LLDP_HOLDTIME_MUL != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lldp holdtime-multiplier %d\n", val);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_lldp_reinit_delay_get(&val));

    if (DFLT_LLDP_REINIT_DELAY != val)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lldp reinit-delay %d\n", val);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_lldp_tx_delay_get(&val2));

    if (DFLT_LLDP_REINIT_DELAY != val2)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lldp tx-delay %d\n", val2);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_lldp_tx_interval_get(&val3));

    if (DFLT_LLDP_TX_INTERVAL != val3)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lldp tx-interval %u\n", val3);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_lldp_disableAction_get(&action));

    if (DFLT_LLDP_DISABLE_ACTION != action)
    {
        SYS_MEM_CLEAR(cmd);

        switch(action)
        {
            case LLDP_DISABLE_ACTION_FILTER:
                osal_strcpy(cmd, "lldp lldpdu filtering\n");
                break;

            case LLDP_DISABLE_ACTION_FLOOD_TO_VLAN:
                osal_strcpy(cmd, "lldp lldpdu bridging\n");
                break;

            case LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS:
                osal_strcpy(cmd, "lldp lldpdu flooding\n");
                break;

            default:
                break;
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_lldp_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    char strVlanList[CAPA_VLANLIST_STR_LEN];
    uint8 lldpPortState = 0;
    sys_enable_t enable = 0;
    lldp_tlv_sel_t emptyTlvSel;
    lldp_tlv_sel_t lldpTlvSel;
    sys_vlanmask_t vlanmask;

    CMD_SAVE_IF_TRKLP_SKIP(port);

    SYS_ERR_CHK(sal_lldp_port_get(port, &lldpPortState));

    if (DFLT_LLDP_PORT_STATE != lldpPortState)
    {
        SYS_MEM_CLEAR(cmd);

        switch (lldpPortState)
        {
            case LLDP_PORT_DISABLE:
                osal_sprintf(cmd, "no lldp rx\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                osal_sprintf(cmd, "no lldp tx\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;

            case LLDP_PORT_RXONLY:
                osal_sprintf(cmd, "lldp rx\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                osal_sprintf(cmd, "no lldp tx\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;

            case LLDP_PORT_TXONLY:
                osal_sprintf(cmd, "no lldp rx\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                osal_sprintf(cmd, "lldp tx\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;

            case LLDP_PORT_TXRX:
                osal_sprintf(cmd, "lldp rx\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                osal_sprintf(cmd, "lldp tx\n");
                CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                break;

            default:
                break;
        }
    }

    SYS_MEM_CLEAR(lldpTlvSel);
    SYS_MEM_CLEAR(emptyTlvSel);

    SYS_ERR_CHK(sal_lldp_portTlvSel_get(port, &lldpTlvSel));

    if (0 != osal_memcmp(&lldpTlvSel, &emptyTlvSel, sizeof(lldp_tlv_sel_t)))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lldp tlv-select");

        if (TRUE == lldpTlvSel.portDesciptionSel)
            osal_strcat(cmd, " port-desc");

        if (TRUE == lldpTlvSel.sysNameSel)
            osal_strcat(cmd, " sys-name");

        if (TRUE == lldpTlvSel.sysDescriptionSel)
            osal_strcat(cmd, " sys-desc");

        if (TRUE == lldpTlvSel.sysCapabilitySel)
            osal_strcat(cmd, " sys-cap");

        if (TRUE == lldpTlvSel.macPhySel)
            osal_strcat(cmd, " mac-phy");

        if (TRUE == lldpTlvSel.linkAggregationSel)
            osal_strcat(cmd, " lag");

        if (TRUE == lldpTlvSel.maxFrameSizeSel)
            osal_strcat(cmd, " max-frame-size");

        if (TRUE == lldpTlvSel.managementAddrSel)
            osal_strcat(cmd, " management-addr");

        osal_strcat(cmd, "\n");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_lldp_portTlvPvidEnable_get(port, &enable));

    if (DFLT_LLDP_PORT_PVID_TLV_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lldp tlv-select pvid %s\n", (ENABLED == enable) ? "enable" : "disable");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_MEM_CLEAR(strVlanList);
    SYS_MEM_CLEAR(vlanmask);

    SYS_ERR_CHK(sal_lldp_portTlvVlanmask_get(port, &vlanmask));

    VMSK2STR(&vlanmask, strVlanList);

    if (0 != osal_strlen(strVlanList))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lldp tlv-select vlan-name add %s\n", strVlanList);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

