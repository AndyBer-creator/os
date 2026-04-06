
#include <sys/sysinfo.h>

#include <libsal/sal_sys.h>
#ifdef CONFIG_SYS_SNMP
#include <libsal/sal_snmp.h>
#endif
#include <libsal/sal_mib.h>
#include <libcmd/cmd.h>
#include <libsal/sal_l3.h>

int32 cmd_sys_version_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char loaderVer[SYS_BUF16_LEN];
    char loaderDate[SYS_BUF32_LEN];

    SYS_MEM_CLEAR(loaderVer);
    SYS_MEM_CLEAR(loaderDate);

    SYS_ERR_CHK(sal_sys_loaderInfo_get(loaderVer, loaderDate));
    XMORE("Loader Version   : %s\n", loaderVer);
    XMORE("Loader Date      : %s\n", loaderDate);
    XMORE("Firmware Version : %s\n", DFLT_SYS_VER);
    XMORE("Firmware Date    : %s\n", SYS_FIRMWARE_DATE);

    return SYS_ERR_OK;
}

int32 cmd_sys_info_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_mac_t macaddr;
    char macaddr_str[CAPA_MAC_STR_LEN];
    char ipaddr_str[CAPA_IPV4_STR_LEN];
    char netmask_str[CAPA_IPV4_STR_LEN];
    char loaderVer[SYS_BUF32_LEN];
    char loaderDate[SYS_BUF32_LEN];
    char name[CAPA_SYS_NAME_LEN+1];
    char location[CAPA_SYS_LOCATION_LEN+1];
    char contact[CAPA_SYS_CONTACT_LEN+1];
	#ifdef CONFIG_SYS_L3_ARCHITECTURE
	uint32 ip = 0;
    uint32 netmask = 0;
    sys_vid_t dfltVid = 0;
    sys_l3_ipv4Db_t *pCurr = NULL;
    int32 i;
	#endif

    int32 updays, uphrs, upmins, upsecs;
    struct sysinfo info;
    struct tm *current_time;
    time_t current_secs;

    SYS_MEM_CLEAR(macaddr_str);
    SYS_MEM_CLEAR(ipaddr_str);
    SYS_MEM_CLEAR(netmask_str);
    SYS_MEM_CLEAR(loaderVer);
    SYS_MEM_CLEAR(loaderDate);
    SYS_MEM_CLEAR(name);
    SYS_MEM_CLEAR(location);
    SYS_MEM_CLEAR(contact);

    SYS_ERR_CHK(sal_sys_systemName_get(name));
    SYS_ERR_CHK(sal_sys_systemLocation_get(location));
    SYS_ERR_CHK(sal_sys_systemContact_get(contact));
    XMORE("System Name        : %s\n", name);
    XMORE("System Location    : %s\n", location);
    XMORE("System Contact     : %s\n", contact);

    SYS_ERR_CHK(sal_sys_macAddr_get(&macaddr));
    SYS_ERR_CHK(sys_util_mac2Str(macaddr.octet, macaddr_str));
    XMORE("MAC Address        : %s\n", macaddr_str);
    
    #ifdef CONFIG_SYS_L3_ARCHITECTURE
    sal_vlan_defaultVid_get(&dfltVid);
    FOR_EACH_L3_IPV4(i, pCurr)
    {
		if (SYS_L3_INTFTYPE_VLAN == pCurr->intfId.type && pCurr->intfId.id == dfltVid)
		{
			ip = pCurr->ip.addr;
			netmask = IPV4_PFX2MASK(pCurr->ip.plen);
		}
    }
    sys_util_ip2Str(ip, ipaddr_str);
    sys_util_ip2Str(netmask, netmask_str);
    #else
    {
        sys_l3_intfMgmt_t intfMgmt;
        SYS_MEM_CLEAR(intfMgmt);

        SYS_ERR_CHK(sal_l3_intfMgmt_get(&intfMgmt));
        SYS_ERR_CHK(sys_util_ip2Str(intfMgmt.ipv4.addr, ipaddr_str));
        IPV42STR(IPV4_PFX2MASK(intfMgmt.ipv4.plen), netmask_str);
    }
    #endif

    XMORE("Default IP Address : %s\n", ipaddr_str);
    XMORE("Subnet Mask        : %s\n", netmask_str);
    
    SYS_ERR_CHK(sal_sys_loaderInfo_get(loaderVer, loaderDate));
    XMORE("Loader Version     : %s\n", loaderVer);
    XMORE("Loader Date        : %s\n", loaderDate);
    
    XMORE("Firmware Version   : %s\n", DFLT_SYS_VER);
    XMORE("Firmware Date      : %s\n", SYS_FIRMWARE_DATE);
#ifdef CONFIG_SYS_SNMP
    XMORE("System Object ID   : %s\n", SYS_OBJECT_ID);
#endif

    time(&current_secs);
    current_time = localtime(&current_secs);
    sysinfo(&info);
    updays = (int) info.uptime / (60*60*24);
    upmins = (int) info.uptime / 60;
    uphrs = (upmins / 60) %24;
    upmins %= 60;
    upsecs = current_time->tm_sec;
    XMORE("System Up Time     : %d days, %d hours, %d mins, %d secs\n", updays, uphrs, upmins, upsecs);
#ifdef CONFIG_SYS_BOARD_TEMP
	SYS_MEM_CLEAR(loaderVer);
	SYS_ERR_CHK(sal_sys_temperature_get(loaderVer));
	XMORE("Temperature	   : %sC\n", loaderVer);
#endif
    return SYS_ERR_OK;
}

int32 cmd_sys_ip_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char buf[CAPA_IPV4_STR_LEN];
    sys_l3_intfMgmt_t intfMgmt;
    sys_l3_intfMgmtAdmin_t adminMgmt;
    sys_l3_routeV4_t gw;

    SYS_MEM_CLEAR(intfMgmt);
    SYS_MEM_CLEAR(gw);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&adminMgmt));
    SYS_ERR_CHK(sal_l3_intfMgmt_get(&intfMgmt));
    SYS_ERR_CHK(sal_l3_routeV4DefalutGw_get(&gw));

    XMORE("###### Config ######\n");
    SYS_MEM_CLEAR(buf);
    IPV42STR(adminMgmt.v4.ip.addr, buf);
    XMORE("IP Address: %s\n", buf);

    SYS_MEM_CLEAR(buf);
    IPV42STR(IPV4_PFX2MASK(adminMgmt.v4.ip.plen), buf);
    XMORE("Subnet Netmask: %s\n", buf);

    SYS_MEM_CLEAR(buf);
    IPV42STR(adminMgmt.v4.gw, buf);
    XMORE("Default Gateway: %s\n", buf);

    XMORE("\n");
    
	if (intfMgmt.subIpv4.addr)
	{
	    XMORE("###### Sub IP Config######\n");
	    SYS_MEM_CLEAR(buf);
	    IPV42STR(intfMgmt.subIpv4.addr, buf);
	    XMORE("IP Address: %s\n", buf);

	    SYS_MEM_CLEAR(buf);
	    IPV42STR(IPV4_PFX2MASK(intfMgmt.subIpv4.plen), buf);
	    XMORE("Subnet Netmask: %s\n", buf);
	    XMORE("\n");
	}
	
    XMORE("###### Status ######\n");
    SYS_MEM_CLEAR(buf);
    IPV42STR(intfMgmt.ipv4.addr, buf);
    XMORE("IP Address: %s\n", buf);

    SYS_MEM_CLEAR(buf);
    IPV42STR(IPV4_PFX2MASK(intfMgmt.ipv4.plen), buf);
    XMORE("Subnet Netmask: %s\n", buf);

    if (SYS_L3_INTFTYPE_MGMTVLAN == gw.ointf.type)
    {
        SYS_MEM_CLEAR(buf);
        IPV42STR(gw.gw, buf);
        XMORE("Default Gateway: %s\n", buf);
    }
    else
    {
        XMORE("Default Gateway: 0.0.0.0\n");
    }
	
   	if (intfMgmt.subIpv4.addr)
	{
		XMORE("\n");
	    SYS_MEM_CLEAR(buf);
	    IPV42STR(intfMgmt.subIpv4.addr, buf);
	    XMORE("IP Sub Address: %s\n", buf);

	    SYS_MEM_CLEAR(buf);
	    IPV42STR(IPV4_PFX2MASK(intfMgmt.subIpv4.plen), buf);
	    XMORE("Subnet Netmask: %s\n", buf);
	}
	
	XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_sys_ipDhcp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (SYS_L3_DHCPTYPE_DHCP == intfMgmt.v4.dhcpType)
        XMORE("DHCP Status : %s\n", "Enabled");
    else if (SYS_L3_DHCPTYPE_BOOTP == intfMgmt.v4.dhcpType)
        XMORE("DHCP Status : %s\n", "Enabled (Bootp)");
    else
        XMORE("DHCP Status : %s\n", "Disabled");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_ADVANCE_DNS

int32 cmd_sys_advDns_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 i = 0;
    char domain[SYS_BUF256_LEN];
    sys_enable_t enable = DISABLED;
    sys_advDnsIP_t ns;
    sys_advDns_staticHost_t host;

    SYS_ERR_CHK(sal_sys_dnsLookup_get(&enable));

    XMORE("\n");
    XMORE("Name/address lookup is %s\n", text_enable[enable].text);
    XMORE("\n\n");
    XMORE("Default Domain Table\n\n");
    XMORE("                 Domain                        Source   Preference\n");
    XMORE("--------------------------------------------- --------- ----------\n");

    SYS_MEM_CLEAR(domain);
    SYS_ERR_CHK(sal_sys_dnsDomain_get(domain, SYS_ADVDNS_SRC_STATIC));

    if (0 != osal_strlen(domain))
        XMORE("%-46s Static   1\n", domain);

    SYS_MEM_CLEAR(domain);
    SYS_ERR_CHK(sal_sys_dnsDomain_get(domain, SYS_ADVDNS_SRC_DHCPV6));

    if (0 != osal_strlen(domain))
        XMORE("%-46s DHCPv6   1\n", domain);

    SYS_MEM_CLEAR(domain);
    SYS_ERR_CHK(sal_sys_dnsDomain_get(domain, SYS_ADVDNS_SRC_DHCPV4));

    if (0 != osal_strlen(domain))
        XMORE("%-46s DHCPv4   1\n", domain);

    XMORE("\n\n");
    XMORE("Name Server Table\n\n");
    XMORE("              IP Address                       Source   Preference\n");
    XMORE("--------------------------------------------- --------- ----------\n");

    for (i = 0 ; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
    {
        SYS_MEM_CLEAR(ns);

        SYS_ERR_CHK(sal_sys_dnsNameServer_get(i, &ns, SYS_ADVDNS_SRC_STATIC));

        if (0 != osal_strlen(ns.ip))
            XMORE("%-46s Static   %u\n", ns.ip, i+1);
    }

    for (i = 0 ; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
    {
        SYS_MEM_CLEAR(ns);

        SYS_ERR_CHK(sal_sys_dnsNameServer_get(i, &ns, SYS_ADVDNS_SRC_DHCPV6));

        if (0 != osal_strlen(ns.ip))
            XMORE("%-46s DHCPv6   %u\n", ns.ip, i+1);
    }

    for (i = 0 ; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
    {
        SYS_MEM_CLEAR(ns);

        SYS_ERR_CHK(sal_sys_dnsNameServer_get(i, &ns, SYS_ADVDNS_SRC_DHCPV4));

        if (0 != osal_strlen(ns.ip))
            XMORE("%-46s DHCPv4   %u\n", ns.ip, i+1);
    }

    XMORE("\n\n");
    XMORE("Cache Table\n");
    XMORE("Flags: (STA, OK)\n");
    XMORE("STA - Static\n");
    XMORE("OK - Okay\n\n");
    XMORE("          Host                  IP Address            Type    State  \n");
    XMORE("------------------------- ------------------------ --------- --------\n");

    for (i = 0 ; i < CAPA_SYS_DNS_STATIC_HOST_NUM ; i++)
    {
        SYS_MEM_CLEAR(host);

        SYS_ERR_CHK(sal_sys_dnsStaticHost_get(i, &host));

        if (0 != osal_strlen(host.host))
        {
            XMORE("%-25s %-25s   %4s   STA,OK\n", host.host, host.ip.ip,
                (host.ip.ipVer == 4) ? "IPv4" : "IPv6");
        }
    }

    XMORE("\n\n");

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PING_TEST

int32 cmd_sys_ping_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 count = 4;
    char cmdstr[SYS_BUF128_LEN];

    if (argc >= 2)
        count = SYS_STR2UINT(argv[1]);

    osal_memset(cmdstr, 0 ,sizeof(cmdstr));

    if (IS_IPV6_ADDRSTR_VALID(argv[0]))
        osal_sprintf(cmdstr, "ping6 -c %d %s", count, argv[0]);
    else
        osal_sprintf(cmdstr, "ping -c %d %s", count, argv[0]);

    system(cmdstr);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_TRACE_ROUTE

int32 cmd_sys_traceroute_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char cmdstr[SYS_BUF128_LEN];
    uint32 maxhop = 30;

    if (argc >= 2)
        maxhop = SYS_STR2UINT(argv[1]);

    osal_memset(cmdstr, 0 ,sizeof(cmdstr));
    osal_sprintf(cmdstr, "traceroute -I -m %u %s", maxhop, argv[0]);
    system(cmdstr);

    return SYS_ERR_OK;
}
#endif

int32 cmd_sys_reboot_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char confirm[2];

	XMORE("Whether to save the configuration to startup-config [y/n] ");
	input_str(confirm, 1, 0);

    if (confirm[0] == 'y')
    {
    	
        cmd_conf_startupConfig_save();
        
        sal_sys_backupConfig_sync(SYS_CONF_SYNC_ALL);
    }

    sal_sys_reboot();

    return SYS_ERR_OK;
}

int32 cmd_sys_ssl_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    vty_echo_enable();

    SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_SSL_HTTPS));

    vty_echo_disable();

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_CPU_UTILIZATION

int32 cmd_sys_cpuUtilz_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 percentage = 0;
    uint32 tpercentage = 0;

    SYS_ERR_CHK(sal_sys_cpuUtilz_get(&percentage));
    tpercentage = (percentage * 100) / 188;

    XMORE("CPU utilization\n");
    XMORE("---------------\n");
    XMORE("Current: %u%%\n", tpercentage);

    return SYS_ERR_OK;
}

int32 cmd_sys_cpuRate_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int i = 0;
    uint64 pkts[2] = {0};

    for (i = 0; i < 2; i++)
    {
        SYS_ERR_CHK(sal_mib_cpuStatsPkts_get(&pkts[i]));
        usleep(1000000);
    }

    XMORE("Input Rate to CPU is %llu pps\n", pkts[1] - pkts[0]);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_MEM_UTILIZATION

int32 cmd_sys_memory_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_mem_info_t meminfo;
    unsigned long tmem = 0;

    SYS_MEM_CLEAR(meminfo);

    SYS_ERR_CHK(sal_sys_memInfo_get(&meminfo));
    tmem = (meminfo.mem_used * 1000) /1920;

    XMORE("           total(KB)     used(KB)     free(KB)   shared(KB)   buffer(KB)    cache(KB)\n");
    XMORE("--------------------+------------+------------+------------+------------+------------\n");
    XMORE("%-7s %12lu %12lu %12lu %12lu %12lu %12lu\n",
          "Mem:",
          meminfo.mem_total, tmem, (meminfo.mem_total - tmem),
          meminfo.mem_shared, meminfo.mem_buffer, meminfo.mem_cached);
    XMORE("-/+ buffers/cache:   %12lu %12lu\n",
          meminfo.mem_total - meminfo.mem_free - meminfo.mem_buffer - meminfo.mem_cached,
          meminfo.mem_free + meminfo.mem_buffer + meminfo.mem_cached);
    XMORE("%-7s %12lu %12lu %12lu\n",
          "Swap:",
          meminfo.swap_total, meminfo.swap_used,
          meminfo.swap_total - meminfo.swap_used);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_IPV6

int32 cmd_sys_ipv6_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmt_t intfMgmt;
    char ipStr[CAPA_IPV6_STR_LEN];
    uint32 i = 0;
    sys_l3_routeV6_t dfltGw;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_intfMgmt_get(&intfMgmt));

    XMORE("###### Config ######\n");
    XMORE("%15s: %s\n", "State", text_enable[intfMgmt.admin.v6.enbl].text);
    XMORE("%15s: %s\n", "Auto Config", text_enable[intfMgmt.admin.v6.autoconfEnbl].text);
    XMORE("%15s: %s\n", "DHCPv6", text_enable[intfMgmt.admin.v6.dhcpEnbl].text);

    SYS_MEM_CLEAR(ipStr);
    IPV62STR(intfMgmt.admin.v6.gw, ipStr);
    XMORE("%15s: %s\n", "Gateway", ipStr);

    for (i = 0; i < CAPA_L3_INTFM_IPV6ADMIN_NUM; i++)
    {
        if (IS_IPV6_ALLZERO(intfMgmt.admin.v6.ip[i].addr))
            continue;

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(intfMgmt.admin.v6.ip[i].addr, ipStr);
        XMORE("%15s: %s/%u\n", "IP Address", ipStr, intfMgmt.admin.v6.ip[i].plen);
    }

    XMORE("\n");

    XMORE("###### Status ######\n");
    for (i = 0; i < CAPA_L3_INTFM_IPV6STATUS_NUM; i++)
    {
        if (IS_IPV6_ALLZERO(intfMgmt.ipv6[i].addr))
            continue;

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(intfMgmt.ipv6[i].addr, ipStr);
        XMORE("%15s: %s/%u\n", "IP Address", ipStr, intfMgmt.ipv6[i].plen);
    }

    SYS_MEM_CLEAR(dfltGw);
    SYS_MEM_CLEAR(ipStr);
    sal_l3_routeV6DefalutGw_get(&dfltGw);
    IPV62STR(dfltGw.gw, ipStr);
    XMORE("%15s: %s\n", "Default Gateway", ipStr);

    return SYS_ERR_OK;
}
#endif

int32 cmd_sys_systemName_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char name[CAPA_SYS_NAME_LEN+1];

    osal_memset(name, 0, sizeof(name));

    if (osal_strlen(argv[0]) > CAPA_SYS_NAME_LEN)
        return SYS_ERR_STR_LEN_EXCEED;

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
        return SYS_ERR_STRING;

    osal_strcpy(name, argv[0]);

    SYS_ERR_CHK(sal_sys_systemName_set(name));

    if (NULL != host.name)
        free(host.name);

    host.name = malloc(osal_strlen(name) + 1);

    if (NULL != host.name)
    {
        osal_memset(host.name, 0, osal_strlen(name) + 1);
        osal_strcpy(host.name, name);
    }

    return SYS_ERR_OK;
}

int32 cmd_sys_systemLocation_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char location[CAPA_SYS_LOCATION_LEN+1];

    osal_memset(location, 0, sizeof(location));

    if (osal_strlen(argv[0]) > CAPA_SYS_LOCATION_LEN)
        return SYS_ERR_STR_LEN_EXCEED;

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
        return SYS_ERR_STRING;

    osal_strcpy(location, argv[0]);

    SYS_ERR_CHK(sal_sys_systemLocation_set(location));

    return SYS_ERR_OK;
}

int32 cmd_sys_systemContact_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char contact[CAPA_SYS_CONTACT_LEN+1];

    osal_memset(contact, 0, sizeof(contact));

    if (osal_strlen(argv[0]) > CAPA_SYS_CONTACT_LEN)
        return SYS_ERR_STR_LEN_EXCEED;

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
        return SYS_ERR_STRING;

    osal_strcpy(contact, argv[0]);

    SYS_ERR_CHK(sal_sys_systemContact_set(contact));

    return SYS_ERR_OK;
}

int32 cmd_sys_dhcp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (CMD_IS_NO_FORM_EXIST())
        intfMgmt.v4.dhcpType = SYS_L3_DHCPTYPE_DISABLE;
    else
        intfMgmt.v4.dhcpType = SYS_L3_DHCPTYPE_DHCP;

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&intfMgmt));

    return SYS_ERR_OK;
}

int32 cmd_sys_ipAddr_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;
    sys_ipv4_t netmask = 0;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    intfMgmt.v4.dhcpType = SYS_L3_DHCPTYPE_DISABLE;
    STR2IPV4(argv[0], &intfMgmt.v4.ip.addr);
    STR2IPV4(2 == argc ? argv[1] : "255.255.255.0", &netmask);
    intfMgmt.v4.ip.plen = IPV4_MASK2PFX(netmask);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&intfMgmt));

    return SYS_ERR_OK;
}

int32 cmd_sys_ipAddr_sub_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;
    sys_ipv4_t netmask = 0;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));
    intfMgmt.subv4.dhcpType = SYS_L3_DHCPTYPE_DISABLE;

    if (CMD_IS_NO_FORM_EXIST())
	{
		if (0 == intfMgmt.subv4.ip.addr)
		    return SYS_ERR_OK;

		intfMgmt.subv4.ip.addr = 0;
	    intfMgmt.subv4.ip.plen = 0;
	}
	else
	{
	    STR2IPV4(argv[0], &intfMgmt.subv4.ip.addr);
	    STR2IPV4(2 == argc ? argv[1] : "255.255.255.0", &netmask);
	    intfMgmt.subv4.ip.plen = IPV4_MASK2PFX(netmask);
	}

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&intfMgmt));

    return SYS_ERR_OK;
}

int32 cmd_sys_ipDfltGateway_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (CMD_IS_NO_FORM_EXIST())
        intfMgmt.v4.gw = 0;
    else
        STR2IPV4(argv[0], &intfMgmt.v4.gw);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&intfMgmt));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_ADVANCE_DNS

int32 cmd_sys_dnsLookup_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_sys_dnsLookup_set(CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_sys_dnsDomainName_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char domain[SYS_BUF256_LEN];

    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_sys_dnsDomain_set("", SYS_ADVDNS_SRC_STATIC));
    else
    {
        SYS_MEM_CLEAR(domain);
        SYS_ERR_CHK(sal_sys_dnsDomain_get(domain, SYS_ADVDNS_SRC_STATIC));

        if (0 != strlen(domain))
        {
            XMORE("Domain-name cannot be inserted. Only one Default domain-name can be configured\n");

            return SYS_ERR_OK;
        }

        if (!IS_HOSTNAME_VALID(argv[0]))
        {
            XMORE("Each label starts and ends with a letter or digit, inner characters can be hyphen also.\n");
            XMORE("Default domain-name is not inserted\n");

            return SYS_ERR_OK;
        }

        SYS_ERR_CHK(sal_sys_dnsDomain_set(argv[0], SYS_ADVDNS_SRC_STATIC));
    }

    SYS_ERR_CHK(sal_sys_dnsResolvConf_create());

    return SYS_ERR_OK;
}

int32 cmd_sys_dnsNameServer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 i = 0;

    sys_advDnsIP_t ns[CAPA_SYS_DNS_NAMESERVER_NUM];

    SYS_MEM_CLEAR(ns);

    for (i = 0; i < argc; i++)
    {
        if (IS_IPV4_UADDRSTR_VALID(argv[i]))
        {
            ns[i].ipVer = 4;
            osal_strcpy(ns[i].ip, argv[i]);
        }
        else if (IS_IPV6_UADDRSTR_VALID(argv[i]))
        {
            ns[i].ipVer = 6;
            osal_strcpy(ns[i].ip, argv[i]);
        }
        else
        {
            XMORE("Name server address '%s' is invalid\n", argv[i]);
        }
    }

    for (i = 0; i < CAPA_SYS_DNS_NAMESERVER_NUM; i++)
    {
        if (CMD_IS_NO_FORM_EXIST())
        {
            if (0 != osal_strlen(ns[i].ip))
            {
                if (SYS_ERR_ENTRY_NOT_FOUND == sal_sys_dnsNameServer_del(&ns[i]))
                    XMORE("Name server address '%s' does not exist or is not statically configured\n", ns[i].ip);
            }
        }
        else
        {
            SYS_ERR_CHK(sal_sys_dnsNameServer_set(i, &ns[i], SYS_ADVDNS_SRC_STATIC));
        }
    }

    SYS_ERR_CHK(sal_sys_dnsResolvConf_create());

    return SYS_ERR_OK;
}

int32 cmd_sys_dnsHostMapping_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_advDns_staticHost_t staticHost;

    SYS_MEM_CLEAR(staticHost);

    if (IS_HOSTNAME_INVALID(argv[0]))
    {
        if (CMD_IS_NO_FORM_EXIST())
        {
            XMORE("Static mappings for hostname %s does not exist\n", argv[0]);
        }
        else
        {
            XMORE("Each label starts and ends with a letter or digit, inner characters can be hyphen also.\n");
            XMORE("Mapping of hostname to IP address is not inserted\n");
        }

        return SYS_ERR_OK;
    }

    osal_strcpy(staticHost.host, argv[0]);

    if (!CMD_IS_NO_FORM_EXIST())
    {
        if (IS_IPV4_UADDRSTR_VALID(argv[1]))
        {
            staticHost.ip.ipVer = 4;
            osal_strcpy(staticHost.ip.ip, argv[1]);
        }
        else if (IS_IPV6_UADDRSTR_VALID(argv[1]))
        {
            staticHost.ip.ipVer = 6;
            osal_strcpy(staticHost.ip.ip, argv[1]);
        }
        else
        {
            XMORE("Host address '%s' is invalid\n", argv[1]);
            return SYS_ERR_OK;
        }
    }

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (SYS_ERR_ENTRY_NOT_FOUND == sal_sys_dnsStaticHost_del(&staticHost))
            XMORE("Static mappings for hostname %s does not exist\n", staticHost.host);
    }
    else
    {
        if (SYS_ERR_ENTRY_FULL == sal_sys_dnsStaticHost_set(&staticHost))
        {
            XMORE("Names table is full.\n");
            XMORE("Mapping of hostname to IP address is not inserted\n");
        }
    }

    SYS_ERR_CHK(sal_sys_dnsHosts_create());

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_IPV6
#ifdef CONFIG_SYS_APP_DHCP6C

int32 cmd_sys_dhcp6c_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (CMD_IS_NO_FORM_EXIST())
        intfMgmt.v6.dhcpEnbl = DISABLED;
    else
        intfMgmt.v6.dhcpEnbl = ENABLED;

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&intfMgmt));

    return SYS_ERR_OK;
}
#endif 

int32 cmd_sys_ipv6Enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (CMD_IS_NO_FORM_EXIST())
        intfMgmt.v6.enbl = DISABLED;
    else
        intfMgmt.v6.enbl = ENABLED;

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&intfMgmt));

    return SYS_ERR_OK;
}

int32 cmd_sys_ipv6Autoconf_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (CMD_IS_NO_FORM_EXIST())
        intfMgmt.v6.autoconfEnbl = DISABLED;
    else
        intfMgmt.v6.autoconfEnbl = ENABLED;

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&intfMgmt));

    return SYS_ERR_OK;
}

int32 cmd_sys_ipv6Addr_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;
    sys_ipv6_t v6addr;
    uint32 i = 0;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    SYS_MEM_CLEAR(v6addr);
    STR2IPV6(argv[0], &v6addr);

    if (CMD_IS_NO_FORM_EXIST())
    {
        for (i = 0; i < CAPA_L3_INTFM_IPV6ADMIN_NUM; i++)
        {
            if (IS_IPV6_EQUAL(v6addr, intfMgmt.v6.ip[i].addr))
            {
                osal_memset(&intfMgmt.v6.ip[i], 0, sizeof(sys_l3_ipv6_t));

                goto _set;
            }
        }
    }
    else
    {
        int32 emptyIdx = -1;
        uint32 prefix = 0;

        prefix = SYS_STR2UINT(argv[1]);

        for (i = 0; i < CAPA_L3_INTFM_IPV6ADMIN_NUM; i++)
        {
            if (IS_IPV6_ALLZERO(intfMgmt.v6.ip[i].addr))
            {
                if (emptyIdx < 0)
                    emptyIdx = i;
                continue;
            }
            if (IS_IPV6_EQUAL(v6addr, intfMgmt.v6.ip[i].addr))
            {
                intfMgmt.v6.ip[i].plen = prefix;
                goto _set;
            }
        }

        if (emptyIdx < 0)
            return SYS_ERR_FAILED;

        osal_memcpy(&intfMgmt.v6.ip[emptyIdx].addr, &v6addr, sizeof(sys_ipv6_t));
        intfMgmt.v6.ip[emptyIdx].plen = prefix;
    }

_set:
    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&intfMgmt));

    return SYS_ERR_OK;
}

int32 cmd_sys_ipv6Gateway_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (CMD_IS_NO_FORM_EXIST())
        SYS_MEM_CLEAR(intfMgmt.v6.gw);
    else
        STR2IPV6(argv[0], &intfMgmt.v6.gw);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_set(&intfMgmt));

    return SYS_ERR_OK;
}
#endif

static int32 _cmd_sys_info_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char name[CAPA_SYS_NAME_LEN+1];
    char location[CAPA_SYS_LOCATION_LEN+1];
    char contact[CAPA_SYS_CONTACT_LEN+1];

    SYS_MEM_CLEAR(name);

    SYS_ERR_CHK(sal_sys_systemName_get(name));

    if (0 != osal_strcmp(DFLT_SYS_NAME, name))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "system name \"%s\"\n", name);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_MEM_CLEAR(location);
    SYS_ERR_CHK(sal_sys_systemLocation_get(location));

    if (0 != osal_strcmp(DFLT_SYS_LOCATION, location))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "system location \"%s\"\n", location);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_MEM_CLEAR(contact);
    SYS_ERR_CHK(sal_sys_systemContact_get(contact));

    if (0 != osal_strcmp(DFLT_SYS_CONTACT, contact))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "system contact \"%s\"\n", contact);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}
#ifndef CONFIG_SYS_L3_ARCHITECTURE
static int32 _cmd_sys_network_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char ipstr[CAPA_IPV4_STR_LEN];
    char maskstr[CAPA_IPV4_STR_LEN];
    char gwstr[CAPA_IPV4_STR_LEN];
    sys_l3_intfMgmtAdmin_t adminMgmt;
    sys_l3_intfMgmt_t intfMgmt;
    sys_l3_routeV4_t gw;
    sys_enable_t enable = DISABLED;
    uint8 i = 0;

    SYS_MEM_CLEAR(cmd);

    SYS_MEM_CLEAR(adminMgmt);
    SYS_MEM_CLEAR(intfMgmt);
    SYS_MEM_CLEAR(gw);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&adminMgmt));

    SYS_ERR_CHK(sal_l3_routeV4DefalutGw_get(&gw));

    if (SYS_L3_DHCPTYPE_DISABLE == adminMgmt.v4.dhcpType)
    {
        if (DFLT_L3_IPV4_DHCPC_EBL == ENABLED)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "no ip dhcp\n");
            CMD_WRITE_TO_FILE(fp, cmd);
        }

        SYS_MEM_CLEAR(ipstr);
        IPV42STR(adminMgmt.v4.ip.addr, ipstr);

        SYS_MEM_CLEAR(maskstr);
        IPV42STR(IPV4_PFX2MASK(adminMgmt.v4.ip.plen), maskstr);

        osal_sprintf(cmd, "ip address %s mask %s\n", ipstr, maskstr);
        CMD_WRITE_TO_FILE(fp, cmd);

        SYS_MEM_CLEAR(gwstr);
        IPV42STR(adminMgmt.v4.gw, gwstr);

        if (osal_strcmp(DFLT_L3_IPV4_GW, gwstr))
        {
            if (0 != adminMgmt.v4.gw)
            {
                osal_sprintf(cmd, "ip default-gateway %s\n", gwstr);
                CMD_WRITE_TO_FILE(fp, cmd);
            }
            else
            {
                osal_sprintf(cmd, "no ip default-gateway\n");
                CMD_WRITE_TO_FILE(fp, cmd);
            }
        }
    }
    else
    {
        if (DFLT_L3_IPV4_DHCPC_EBL == DISABLED)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "ip dhcp\n");
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

	if (adminMgmt.subv4.ip.addr)
	{
        SYS_MEM_CLEAR(ipstr);
        IPV42STR(adminMgmt.subv4.ip.addr, ipstr);

        SYS_MEM_CLEAR(maskstr);
        IPV42STR(IPV4_PFX2MASK(adminMgmt.subv4.ip.plen), maskstr);

        osal_sprintf(cmd, "ip sub-address %s mask %s\n", ipstr, maskstr);
		CMD_WRITE_TO_FILE(fp, cmd);
	}

#ifdef CONFIG_SYS_ADVANCE_DNS
    char domain[SYS_BUF256_LEN];
    sys_advDnsIP_t ns;
    sys_advDns_staticHost_t host;

    SYS_ERR_CHK(sal_sys_dnsLookup_get(&enable));
    SYS_ERR_CHK(sal_sys_dnsDomain_get(domain, SYS_ADVDNS_SRC_STATIC));

    SYS_MEM_CLEAR(cmd);

    if (DFLT_SYS_ADV_DNS_ENBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip domain lookup\n",(DISABLED == enable) ? "no " : "");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (0 != osal_strcmp(DFLT_SYS_ADV_DNS_DOMAIN, domain))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip domain name %s\n", domain);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "ip name-server ");

    for (i = 0 ; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
    {
        SYS_MEM_CLEAR(ns);
        SYS_ERR_CHK(sal_sys_dnsNameServer_get(i, &ns, SYS_ADVDNS_SRC_STATIC));

        if (0 != osal_strlen(ns.ip))
            osal_sprintf(cmd + strlen(cmd), "%s ", ns.ip);
    }

    osal_sprintf(cmd + strlen(cmd), "\n");

    if (osal_strlen(cmd) > 24)
        CMD_WRITE_TO_FILE(fp, cmd);

    for (i = 0 ; i < CAPA_SYS_DNS_STATIC_HOST_NUM ; i++)
    {
        SYS_MEM_CLEAR(host);

        SYS_ERR_CHK(sal_sys_dnsStaticHost_get(i, &host));

        if (0 != osal_strlen(host.host))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "ip host %s ", host.host);
            osal_sprintf(cmd + strlen(cmd), "%s ", host.ip.ip);
            osal_sprintf(cmd + strlen(cmd), "\n");
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }
#endif

#ifdef CONFIG_SYS_IPV6
    SYS_ERR_CHK(sal_l3_intfMgmt_get(&intfMgmt));
    
    enable = intfMgmt.admin.v6.enbl;
    if (DFLT_L3_IPV6_ADMIN_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

  #ifdef CONFIG_SYS_APP_DHCP6C

    enable = intfMgmt.admin.v6.dhcpEnbl;
    if (DFLT_L3_IPV6_DHCPC_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 dhcp\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
  #endif

    enable = intfMgmt.admin.v6.autoconfEnbl;
    if (DFLT_L3_IPV6_AUTOCONF_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 autoconfig\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    {
        char ipv6Str[CAPA_IPV6_STR_LEN];
        char v6GwStr[CAPA_IPV6_STR_LEN];
        
        for (i = 0; i < CAPA_L3_INTFM_IPV6ADMIN_NUM; i++)
        {
            if (IS_IPV6_ALLZERO(intfMgmt.admin.v6.ip[i].addr))
                continue;

            SYS_MEM_CLEAR(ipv6Str);
            IPV62STR(intfMgmt.admin.v6.ip[i].addr, ipv6Str);

            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "ipv6 address %s prefix %u\n", ipv6Str, intfMgmt.admin.v6.ip[i].plen);

            if (DFLT_L3_IPV6_PREFIX_LEN != intfMgmt.admin.v6.ip[i].plen)
            {
                CMD_WRITE_TO_FILE(fp, cmd);
            }
            else if (0 != osal_strcmp(DFLT_L3_IPV6_ADDR, ipv6Str))
            {
                if ((strlen(DFLT_L3_IPV6_ADDR) && strlen(ipv6Str))
                    || (!strlen(DFLT_L3_IPV6_ADDR) && osal_strcmp("::", ipv6Str))
                    || (!strlen(ipv6Str) && osal_strcmp("::", DFLT_L3_IPV6_ADDR)))
                {
                    CMD_WRITE_TO_FILE(fp, cmd);
                }
            }
        }

        SYS_MEM_CLEAR(v6GwStr);
        IPV62STR(intfMgmt.admin.v6.gw, v6GwStr);

        if (0 != osal_strcmp(DFLT_L3_IPV6_GW, v6GwStr))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "ipv6 default-gateway %s\n", v6GwStr);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }
#endif

    return SYS_ERR_OK;
}
#endif

int32 cmd_sys_save(FILE *fp)
{
    
    SYS_ERR_CHK(_cmd_sys_info_save(fp));

#ifndef CONFIG_SYS_L3_ARCHITECTURE
    
    SYS_ERR_CHK(_cmd_sys_network_save(fp));
#endif
    return SYS_ERR_OK;
}

int32 cmd_sys_systemSupportInfo_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	char support[CAPA_SYS_LOCATION_LEN + 1];

    osal_memset(support, 0, sizeof(support));

    if (osal_strlen(argv[0]) > CAPA_SYS_CONTACT_LEN)
        return SYS_ERR_STR_LEN_EXCEED;

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
        return SYS_ERR_STRING;

    osal_strcpy(support, argv[0]);

    SYS_ERR_CHK(sal_sys_systemSupportInfo_set(support));

    return SYS_ERR_OK;
}

int32 cmd_sys_systemManufacturer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char manufacturer[CAPA_SYS_LOCATION_LEN + 1];

    osal_memset(manufacturer, 0, sizeof(manufacturer));

    if (osal_strlen(argv[0]) > CAPA_SYS_CONTACT_LEN)
        return SYS_ERR_STR_LEN_EXCEED;

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
        return SYS_ERR_STRING;

    osal_strcpy(manufacturer, argv[0]);

    SYS_ERR_CHK(sal_sys_systemManufacturer_set(manufacturer));

    return SYS_ERR_OK;
}

int32 cmd_sys_systemTelephone_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	char telephone[CAPA_SYS_NAME_LEN + 1];

    osal_memset(telephone, 0, sizeof(telephone));

    if (osal_strlen(argv[0]) > CAPA_SYS_CONTACT_LEN)
        return SYS_ERR_STR_LEN_EXCEED;

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
        return SYS_ERR_STRING;

    osal_strcpy(telephone, argv[0]);

    SYS_ERR_CHK(sal_sys_systemTelephone_set(telephone));

    return SYS_ERR_OK;
}

