
#include <sys/sysinfo.h>
#include <include/cgi.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_mib.h>
#include <libsal/sal_l3.h>
#include <libfds/fds_export.h>

static int32 cgi_sys_sysinfo_get(void);
static int32 cgi_sys_sysinfoEdit_get(void);
#if defined(CONFIG_SYS_CPU_UTILIZATION) || defined(CONFIG_SYS_MEM_UTILIZATION)
static int32 cgi_sys_cpuMem_get(void);
#endif
static int32 cgi_sys_ip_get(void);
static int32 cgi_sys_acct_get(void);
static int32 cgi_sys_acctAdd_get(void);
static int32 cgi_sys_acctEdit_get(void);
static int32 cgi_sys_line_get(void);
#ifdef CONFIG_SYS_ADVANCE_DNS
static int32 cgi_sys_dns_get(void);
static int32 cgi_sys_dnsHosts_get(void);
#endif
static int32 cgi_sys_sysTime_get(void);

const cgi_cmdmap_t cmdGetSysTbl[] =
{
    {CGI_CMD_SYS_SYSINFO,           cgi_sys_sysinfo_get},
    {CGI_CMD_SYS_SYSINFOEDIT,       cgi_sys_sysinfoEdit_get},
#if defined(CONFIG_SYS_CPU_UTILIZATION) || defined(CONFIG_SYS_MEM_UTILIZATION)
    {CGI_CMD_SYS_CPUMEM,            cgi_sys_cpuMem_get},
#endif
    {CGI_CMD_SYS_IP,                cgi_sys_ip_get},
    {CGI_CMD_SYS_ACCT,              cgi_sys_acct_get},
    {CGI_CMD_SYS_ACCTADD,           cgi_sys_acctAdd_get},
    {CGI_CMD_SYS_ACCTEDIT,          cgi_sys_acctEdit_get},
    {CGI_CMD_SYS_LINE,              cgi_sys_line_get},
#ifdef CONFIG_SYS_ADVANCE_DNS
    {CGI_CMD_SYS_DNS,               cgi_sys_dns_get},
    {CGI_CMD_SYS_DNS_HOSTS,         cgi_sys_dnsHosts_get},
#endif
	{CGI_CMD_SYS_SYSTIME,			cgi_sys_sysTime_get},
    {NULL, NULL}
};

const sys_text_t cgi_text_sys_line[] =
{
    {    "lang('line','lblConsole')"  },
    {     "lang('line','lblTelnet')"   },
#ifdef CONFIG_USER_SSH_SSHD
    {        "lang('line','lblSsh')"      },
#endif
    {       "lang('line','lblHttp')"     },
    {      "lang('line','lblHttps')"    },
};

static int32 cgi_sys_sysinfo_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *methods = NULL;
    cJSON *method = NULL;
    char sysname[CAPA_SYS_NAME_LEN + 1];
    char contact[CAPA_SYS_CONTACT_LEN + 1];
    char location[CAPA_SYS_LOCATION_LEN + 1];
    char oidStr[SYS_BUF128_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    char tmp[SYS_BUF64_LEN];
    char sn[SYS_BUF64_LEN];
    sys_mac_t sysMacAddr;
    sys_boardModel_t boardModel;
    uint32 i = 0;
    uint32 ip = 0;
    uint32 netmask = 0;
    char loaderVer[SYS_BUF32_LEN];
    char loaderDate[SYS_BUF32_LEN];
    char ipStr[CAPA_IPV4_STR_LEN];
#ifdef CONFIG_SYS_BOARD_TEMP
    char currTemp[SYS_BUF32_LEN];
#endif
#ifdef CONFIG_SYS_IPV6
    FILE *fp = NULL;
    char addr6p[8][5];
    char devname[20];
    char defaultName[SYS_BUF16_LEN];
    uint32 plen = 0;
    uint32 scope = 0;
    uint32 dad_status = 0;
    uint32 if_idx = 0;
    int ret = 0;
    int flag = 0;

    char strIpv6AddrDisplay[SYS_BUF1024_LEN];
    char strIpv6Addr[CAPA_IPV6_STR_LEN] = "";
    char sysIpv6Addr[CAPA_IPV6_BYTE_NUM] = "";
#endif
    sys_enable_t stateAry[SYS_ACCESS_END];

    int32 updays, uphrs, upmins, upsecs;
    struct sysinfo info;
    struct tm *current_time;
    time_t current_secs;
    sys_time_str_t timeStr;
    uint32 sysSec = 0;

    SYS_MEM_CLEAR(sysname);
    SYS_MEM_CLEAR(contact);
    SYS_MEM_CLEAR(location);
    SYS_MEM_CLEAR(sysMacAddr);
    SYS_MEM_CLEAR(boardModel);
    SYS_MEM_CLEAR(oidStr);
    SYS_MEM_CLEAR(loaderVer);
    SYS_MEM_CLEAR(loaderDate);
    SYS_MEM_CLEAR(timeStr);
    SYS_MEM_CLEAR(ipStr);
    SYS_MEM_CLEAR(stateAry);
    SYS_MEM_CLEAR(tmp);
    SYS_MEM_CLEAR(sn);
#ifdef CONFIG_SYS_BOARD_TEMP
    SYS_MEM_CLEAR(currTemp);
#endif
#ifdef CONFIG_SYS_IPV6
    SYS_MEM_CLEAR(defaultName);
#endif

    sal_sys_systemName_get(sysname);
    sal_sys_systemContact_get(contact);
    sal_sys_systemLocation_get(location);
    sal_sys_macAddr_get(&sysMacAddr);
    sal_sys_boardModel_get(&boardModel);
    sal_sys_loaderInfo_get(loaderVer, loaderDate);
    sal_time_currentTimeStr_get(&timeStr);
    sal_sys_ip_get(&ip, &netmask);
    sys_util_ip2Str(ip, ipStr);
    sal_time_currentTimeInt_get(&sysSec);
#ifdef CONFIG_SYS_BOARD_TEMP
    sal_sys_temperature_get(currTemp);
#endif
#ifdef CONFIG_SYS_SNMP
    for (i = 0; i < boardModel.oid.oidLen; i++)
    {
        osal_sprintf(tmp, "%d", boardModel.oid.oidAry[i]);
        strlcat(oidStr, tmp, sizeof(oidStr));
        strlcat(oidStr, ".", sizeof(oidStr));
    }
    oidStr[osal_strlen(oidStr) - 1] = '\0';
#endif
    sys_util_mac2Str(sysMacAddr.octet, macStr);

    time(&current_secs);
    current_time = localtime(&current_secs);
    sysinfo(&info);
    updays = (int) info.uptime / (60*60*24);
    upmins = (int) info.uptime / 60;
    uphrs = (upmins / 60) %24;
    upmins %= 60;
    upsecs = current_time->tm_sec;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddStringToObject(data, CGI_SYS_LOCATION, location);
    cJSON_AddStringToObject(data, CGI_SYS_CONTACT, contact);
    cJSON_AddStringToObject(data, CGI_SYS_HOSTNAME, sysname);
	if (SYS_ERR_OK == sal_sys_bdinfoVar_get("serialnumber", sn))
	    cJSON_AddStringToObject(data, CGI_HOME_SYSTEM_SN, sn);

    cJSON_AddStringToObject(data, CGI_SYS_OBJID, oidStr);
    cJSON_AddLangStringArgToObject(data, CGI_SYS_UPTIME, "sys", "txtSysUptimeArg", "%d,%d,%d,%d", updays, uphrs, upmins, upsecs);
    cJSON_AddStringToObject(data, CGI_SYS_CURRTIME, timeStr.tstr);
    cJSON_AddNumberToObject(data, CGI_SYS_SEC, sysSec);
    cJSON_AddStringToObject(data, CGI_SYS_MAC, macStr);
    cJSON_AddStringToObject(data, CGI_SYS_CURRIPV4, ipStr);
    cJSON_AddStringToObject(data, CGI_SYS_LOADER_VER, loaderVer);
    cJSON_AddStringToObject(data, CGI_SYS_LOADER_DATE, loaderDate);
    cJSON_AddStringToObject(data, CGI_SYS_FW_VER, DFLT_SYS_VER);
    cJSON_AddStringToObject(data, CGI_SYS_FW_DATE, SYS_FIRMWARE_DATE);
    #ifdef CONFIG_SYS_BOARD_TEMP
	cJSON_AddLangStringArgToObject(data, CGI_SYS_CURTEMP, "sys", "txtSysUpTempArg", "%s", currTemp);
	#endif
#ifdef CONFIG_SYS_IPV6
    if (NULL != (fp = fopen(_PATH_PROCNET_IFINET6, "r")))
    {
        cJSON *ips = NULL;
        ips = cJSON_CreateArray();

        do
        {
            ret = fscanf(fp, "%4s%4s%4s%4s%4s%4s%4s%4s %02x %02x %02x %02x %20s\n",
                         addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4], addr6p[5],
                         addr6p[6], addr6p[7], &if_idx, &plen, &scope, &dad_status, devname);

#ifdef CONFIG_SYS_L3_ROUTE
            {
                sys_l3_intfId_t intfId;

                SYS_MEM_CLEAR(intfId);

                intfId.type = SYS_L3_INTFTYPE_VLAN;
                sal_vlan_defaultVid_get(&intfId.id);

                INTFID2STR(&intfId, defaultName);
            }
#else
            strlcpy(defaultName, L3_INTF_MGMTVLAN, sizeof(defaultName));
#endif
            if ((EOF != ret) && (0 == osal_strcmp(devname, defaultName)))
            {
                flag = 1;
                osal_sprintf(strIpv6Addr, "%s:%s:%s:%s:%s:%s:%s:%s",
                             addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4], addr6p[5],
                             addr6p[6], addr6p[7]);

                inet_pton(AF_INET6, strIpv6Addr, sysIpv6Addr);
                inet_ntop(AF_INET6, sysIpv6Addr, strIpv6Addr, sizeof(strIpv6Addr));

                snprintf(strIpv6AddrDisplay, sizeof(strIpv6AddrDisplay), "%s/%u", strIpv6Addr, plen);
                cJSON_AddItemToArray(ips, cJSON_CreateString(strIpv6AddrDisplay));
            }

        } while (EOF != ret);
        if(0 == flag)
        {
            fseek(fp, 0, SEEK_SET);
            do
            {
                ret = fscanf(fp, "%4s%4s%4s%4s%4s%4s%4s%4s %02x %02x %02x %02x %20s\n",
                         addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4], addr6p[5],
                         addr6p[6], addr6p[7], &if_idx, &plen, &scope, &dad_status, devname);
                if ((EOF != ret) && (0 == osal_strcmp(devname, "intfm")))
                {
                    osal_sprintf(strIpv6Addr, "%s:%s:%s:%s:%s:%s:%s:%s",
                             addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4], addr6p[5],
                             addr6p[6], addr6p[7]);

                    inet_pton(AF_INET6, strIpv6Addr, sysIpv6Addr);
                    inet_ntop(AF_INET6, sysIpv6Addr, strIpv6Addr, sizeof(strIpv6Addr));

                    snprintf(strIpv6AddrDisplay, sizeof(strIpv6AddrDisplay), "%s/%u", strIpv6Addr, plen);
                    cJSON_AddItemToArray(ips, cJSON_CreateString(strIpv6AddrDisplay));
                }
            } while (EOF != ret);
        }
        fclose(fp);

        cJSON_AddItemToObject(data, CGI_SYS_IPV6_CURRENT_ADDR, ips);
    }
#endif

#ifdef CONFIG_SYS_APP_TELNETD
    sal_sys_telnetdEnable_get(&stateAry[SYS_ACCESS_CLI_TELNET]);
#endif
#ifdef CONFIG_USER_SSH_SSHD
    sal_ssh_sshdEnable_get(&stateAry[SYS_ACCESS_CLI_SSH]);
#endif
    sal_sys_httpEnable_get(&stateAry[SYS_ACCESS_WEB_HTTP]);
#ifdef CONFIG_USER_BOA_WITH_SSL
    sal_sys_httpsEnable_get(&stateAry[SYS_ACCESS_WEB_HTTPS]);
#endif

    methods = cJSON_CreateArray();
    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        if (SYS_ACCESS_CLI_CONSOLE == i)
            continue;

        method = cJSON_CreateObject();

        cJSON_AddStringToObject(method, CGI_TXT, cgi_text_sys_line[i].text);
        cJSON_AddBoolToObject(method, CGI_SYS_LINE_STATE, stateAry[i]);

        cJSON_AddItemToArray(methods, method);
    }
#ifdef CONFIG_SYS_UI_SNMP
    {
        sys_enable_t snmpState = DISABLED;

        sal_snmp_enable_get(&snmpState);

        method = cJSON_CreateObject();

        cJSON_AddLangStringToObject(method, CGI_TXT, "line", "lblSnmp");
        cJSON_AddBoolToObject(method, CGI_SYS_LINE_STATE, snmpState);
        cJSON_AddItemToArray(methods, method);
    }
#endif

    cJSON_AddItemToObject(data, CGI_SYS_LINE_METHODS, methods);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_sys_sysinfoEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char sysname[CAPA_SYS_NAME_LEN + 1];
    char contact[CAPA_SYS_CONTACT_LEN + 1];
    char location[CAPA_SYS_LOCATION_LEN + 1];

    SYS_MEM_CLEAR(sysname);
    SYS_MEM_CLEAR(contact);
    SYS_MEM_CLEAR(location);

    sal_sys_systemName_get(sysname);
    sal_sys_systemContact_get(contact);
    sal_sys_systemLocation_get(location);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddStringToObject(data, CGI_SYS_LOCATION, location);
    cJSON_AddStringToObject(data, CGI_SYS_CONTACT, contact);
    cJSON_AddStringToObject(data, CGI_SYS_HOSTNAME, sysname);
    cJSON_AddNumberToObject(data, CGI_SYS_LOCATION_MAXLEN, CAPA_SYS_LOCATION_LEN);
    cJSON_AddNumberToObject(data, CGI_SYS_CONTACT_MAXLEN, CAPA_SYS_CONTACT_LEN);
    cJSON_AddNumberToObject(data, CGI_SYS_HOSTNAME_MAXLEN, CAPA_SYS_NAME_LEN);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#if defined(CONFIG_SYS_CPU_UTILIZATION) || defined(CONFIG_SYS_MEM_UTILIZATION)
static int32 cgi_sys_cpuMem_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    uint32 cpu = 0;
    uint32 tcpu = 0;
    double mem = 0;
    double tmem = 0;
    sys_mem_info_t meminfo;

    SYS_MEM_CLEAR(meminfo);

    sal_sys_cpuUtilz_get(&cpu);
    sal_sys_memInfo_get(&meminfo);
    mem = ((double)(meminfo.mem_total - meminfo.mem_free - meminfo.mem_buffer - meminfo.mem_cached) / (double)meminfo.mem_total) * 100;

    tcpu = (cpu * 100) / 188;
    tmem = (mem * 1000) / 1920;
    
    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_SYS_CPU, tcpu);
    cJSON_AddNumberToObject(data, CGI_SYS_MEM, (uint32)(tmem));

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

static int32 _cgi_sys_ipv4_get(cJSON *data)
{
    #define SYS_DFT_EMPTY_IP "0.0.0.0"
    char ipStr[CAPA_IPV4_STR_LEN];
    char maskStr[CAPA_IPV4_STR_LEN];
    char gwStr[CAPA_IPV4_STR_LEN];
    char ipSttcStr[CAPA_IPV4_STR_LEN];
    char gwSttcStr[CAPA_IPV4_STR_LEN];
    char subipStr[CAPA_IPV4_STR_LEN];
    char submaskStr[CAPA_IPV4_STR_LEN];
    int subIpv4Status = 0;

    sys_l3_intfMgmt_t intfMgmt;
    sys_l3_intfMgmtAdmin_t adminMgmt;
    sys_ipv4_t v4Mask = 0;
    sys_l3_routeV4_t gw;

    SYS_MEM_CLEAR(ipStr);
    SYS_MEM_CLEAR(maskStr);
    SYS_MEM_CLEAR(gwStr);
    SYS_MEM_CLEAR(ipSttcStr);
    SYS_MEM_CLEAR(gwSttcStr);
    SYS_MEM_CLEAR(intfMgmt);
    SYS_MEM_CLEAR(adminMgmt);
    SYS_MEM_CLEAR(gw);
	SYS_MEM_CLEAR(subipStr);
	SYS_MEM_CLEAR(submaskStr);

    sal_l3_intfMgmt_get(&intfMgmt);
    sal_l3_adminIntfMgmt_get(&adminMgmt);
    sal_l3_routeV4DefalutGw_get(&gw);

    switch (adminMgmt.v4.dhcpType)
    {
        case SYS_L3_DHCPTYPE_DISABLE:
            cJSON_AddStringToObject(data, CGI_SYS_IPV4_TYPE, "static");
            break;
        case SYS_L3_DHCPTYPE_DHCP:
            cJSON_AddStringToObject(data, CGI_SYS_IPV4_TYPE, "dynamic");
            break;
        default:
            break;
    }

    if (SYS_L3_DHCPTYPE_DISABLE == adminMgmt.v4.dhcpType)
    {
        IPV42STR(adminMgmt.v4.ip.addr, ipStr);
        v4Mask = sys_l3_ipv4Prefix2Mask_ret(adminMgmt.v4.ip.plen);
        IPV42STR(v4Mask, maskStr);
        IPV42STR(adminMgmt.v4.gw, gwStr);
    }
    else
    {
        IPV42STR(intfMgmt.ipv4.addr, ipStr);
        v4Mask = sys_l3_ipv4Prefix2Mask_ret(intfMgmt.ipv4.plen);
        IPV42STR(v4Mask, maskStr);

        if (SYS_L3_INTFTYPE_MGMTVLAN == gw.ointf.type)
        {
            SYS_MEM_CLEAR(gwStr);
            IPV42STR(gw.gw, gwStr);
        }
        else
        {
            SYS_MEM_CLEAR(gwStr);
            osal_sprintf(gwStr, "0.0.0.0");
        }
    }

	if(intfMgmt.subIpv4.addr)
	{
		IPV42STR(intfMgmt.subIpv4.addr, subipStr);
		v4Mask = sys_l3_ipv4Prefix2Mask_ret(intfMgmt.subIpv4.plen);
        IPV42STR(v4Mask, submaskStr);
        subIpv4Status = TRUE;
	}

    if (0 == osal_strlen(ipStr))
        osal_sprintf(ipStr, "%s", SYS_DFT_EMPTY_IP);

    if (0 == osal_strlen(maskStr))
        osal_sprintf(maskStr, "%s", SYS_DFT_EMPTY_IP);

    if (0 == osal_strlen(gwStr))
        osal_sprintf(gwStr, "%s", SYS_DFT_EMPTY_IP);
        
    if (0 == osal_strlen(subipStr))
        osal_sprintf(subipStr, "%s", SYS_DFT_EMPTY_IP);

    if (0 == osal_strlen(submaskStr))
        osal_sprintf(submaskStr, "%s", SYS_DFT_EMPTY_IP);

    IPV42STR(adminMgmt.v4.ip.addr, ipSttcStr);
    IPV42STR(adminMgmt.v4.gw, gwSttcStr);

    cJSON_AddStringToObject(data, CGI_SYS_IPV4_IP, ipStr);
    cJSON_AddStringToObject(data, CGI_SYS_IPV4_STATICIP, ipSttcStr);
    cJSON_AddStringToObject(data, CGI_SYS_IPV4_STATICMASK, maskStr);
    cJSON_AddStringToObject(data, CGI_SYS_IPV4_ADMIN_GW, gwSttcStr);
    cJSON_AddStringToObject(data, CGI_SYS_IPV4_OPER_GW, gwStr);
    cJSON_AddStringToObject(data, CGI_SYS_IPV4_SUB_STATICIP, subipStr);
    cJSON_AddStringToObject(data, CGI_SYS_IPV4_SUB_STATICMASK, submaskStr);
    cJSON_AddBoolToObject(data, CGI_SYS_IPV4_SUB_STATUS, subIpv4Status);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_IPV6
static int32 _cgi_sys_ipv6_get(cJSON *data)
{
    cJSON *ipv6 = NULL;
    cJSON *gw = NULL;
    char ipStr[CAPA_IPV6_STR_LEN];
    char buf[SYS_BUF256_LEN];
    sys_l3_intfMgmt_t intfMgmt;
    uint32 i = 0;
    sys_l3_ipv6_t linkLocalIp;
    sys_l3_routeV6_t dfltGw;

    SYS_MEM_CLEAR(ipStr);
    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(intfMgmt);
    SYS_MEM_CLEAR(linkLocalIp);
    SYS_MEM_CLEAR(dfltGw);

    sal_l3_intfMgmt_get(&intfMgmt);

    cJSON_AddBoolToObject(data, CGI_SYS_IPV6_AUTO_CONFIG, intfMgmt.admin.v6.autoconfEnbl);
    cJSON_AddBoolToObject(data, CGI_SYS_IPV6_DHCPV6, intfMgmt.admin.v6.dhcpEnbl);

    cJSON_AddNumberToObject(data, CGI_SYS_IPV6_PREFIX, intfMgmt.admin.v6.ip[0].plen);
    SYS_MEM_CLEAR(ipStr);
    IPV62STR(intfMgmt.admin.v6.ip[0].addr, ipStr);
    cJSON_AddStringToObject(data, CGI_SYS_IPV6_STATIC_ADDR, ipStr);

    SYS_MEM_CLEAR(ipStr);
    IPV62STR(intfMgmt.admin.v6.gw, ipStr);
    if (0 == osal_strcmp(ipStr, "::"))
        cJSON_AddStringToObject(data, CGI_SYS_IPV6_STATIC_GATEWAY, "");
    else
        cJSON_AddStringToObject(data, CGI_SYS_IPV6_STATIC_GATEWAY, ipStr);

    SYS_MEM_CLEAR(ipStr);
    sys_l3_ipv6SysLinkLocal_get(&linkLocalIp);
    IPV62STR(linkLocalIp.addr, ipStr);
    osal_sprintf(buf, "%s/%d", ipStr, linkLocalIp.plen);
    cJSON_AddStringToObject(data, CGI_SYS_IPV6_LINK_LOCAL_ADDR, buf);

    gw = cJSON_CreateArray();

    SYS_MEM_CLEAR(ipStr);
    sal_l3_routeV6DefalutGw_get(&dfltGw);
    IPV62STR(dfltGw.gw, ipStr);
    cJSON_AddItemToArray(gw, cJSON_CreateString(ipStr));

    if(0 != cJSON_GetArraySize(gw))
        cJSON_AddItemToObject(data, CGI_SYS_IPV6_CURRENT_GW, gw);
    else
        cJSON_Delete(gw);

    ipv6 = cJSON_CreateArray();

    for (i = 0; i < CAPA_L3_INTFM_IPV6STATUS_NUM; i++)
    {
        if (IS_IPV6_ALLZERO(intfMgmt.ipv6[i].addr))
            continue;

        if (IS_IPV6_LINKLOCAL(intfMgmt.ipv6[i].addr))
            continue;

        SYS_MEM_CLEAR(buf);
        SYS_MEM_CLEAR(ipStr);
        IPV62STR(intfMgmt.ipv6[i].addr, ipStr);
        osal_sprintf(buf, "%s/%d", ipStr, intfMgmt.ipv6[i].plen);

        cJSON_AddItemToArray(ipv6, cJSON_CreateString(buf));
    }

    if(0 != cJSON_GetArraySize(ipv6))
        cJSON_AddItemToObject(data, CGI_SYS_IPV6_CURRENT_ADDR, ipv6);
    else
        cJSON_Delete(ipv6);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_sys_ip_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    _cgi_sys_ipv4_get(data);
#ifdef CONFIG_SYS_IPV6
    _cgi_sys_ipv6_get(data);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_sys_acct_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *users = NULL;
    cJSON *user = NULL;
    uint32 i;
    uint32 cnt = 0;
    sys_userInfo_t userInfo;

    CGI_GET_JSON_CREATE(root, data);

    users = cJSON_CreateArray();

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        SYS_MEM_CLEAR(userInfo);
        sal_sys_localUser_get(i, &userInfo);

        if (userInfo.priv < 0)
            continue;

        user = cJSON_CreateObject();
        cJSON_AddStringToObject(user, CGI_SYS_ACCT, userInfo.username);

        cJSON_AddNumberToObject(user, CGI_SYS_ACCT_PRIV, userInfo.priv);
        cJSON_AddItemToArray(users, user);
        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_SYS_ACCTS, users);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_sys_acctAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_SYS_ACCT_NAME_MAXLEN, CAPA_SYS_USERNAME_LEN);
    cJSON_AddNumberToObject(data, CGI_SYS_ACCT_PASSWD_MAXLEN, CAPA_SYS_PASSWORD_LEN);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_sys_acctEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    uint32 i;
    sys_userInfo_t userInfo;
    char *value;
    char selUser[CAPA_SYS_USERNAME_LEN + 1];

    SYS_MEM_CLEAR(selUser);

    if (CGI_IS_PARAM_EXIST(CGI_SYS_ACCT, value))
        strlcpy(selUser, value, sizeof(selUser));

    CGI_GET_JSON_CREATE(root, data);

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        SYS_MEM_CLEAR(userInfo);
        sal_sys_localUser_get(i, &userInfo);

        if (userInfo.priv < 0)
            continue;

        if (0 != osal_strcmp(selUser, userInfo.username))
            continue;

        cJSON_AddNumberToObject(data, CGI_SYS_ACCT_PRIV, userInfo.priv);

        break;
    }

    cJSON_AddNumberToObject(data, CGI_SYS_ACCT_PASSWD_MAXLEN, CAPA_SYS_PASSWORD_LEN);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_sys_line_get(void)
{
    cJSON                *root = NULL;
    cJSON                *data = NULL;
    cJSON                *methods = NULL;
    cJSON                *method = NULL;
    cJSON                *timeouts = NULL;
    cJSON                *timeout = NULL;
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    cJSON                *retries = NULL;
    cJSON                *retry = NULL;
    cJSON                *silents = NULL;
    cJSON                *silent = NULL;
#endif
    uint32               i = 0;
    sys_enable_t         stateAry[SYS_ACCESS_END];
    uint32               timeoutAry[SYS_ACCESS_END];
    uint32               dfltTimeoutAry[SYS_ACCESS_END];
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    sys_cliRetrySilent_t retrySilentAry[SYS_ACCESS_END];
    uint32               dfltRetryAry[SYS_ACCESS_END];
    uint32               dfltSilentAry[SYS_ACCESS_END];
#endif

    SYS_MEM_CLEAR(stateAry);
    SYS_MEM_CLEAR(timeoutAry);
    SYS_MEM_CLEAR(dfltTimeoutAry);
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    SYS_MEM_CLEAR(retrySilentAry);
    SYS_MEM_CLEAR(dfltRetryAry);
    SYS_MEM_CLEAR(dfltSilentAry);
#endif

#ifdef CONFIG_SYS_APP_TELNETD
    sal_sys_telnetdEnable_get(&stateAry[SYS_ACCESS_CLI_TELNET]);
#endif
#ifdef CONFIG_USER_SSH_SSHD
    sal_ssh_sshdEnable_get(&stateAry[SYS_ACCESS_CLI_SSH]);
#endif
    sal_sys_httpEnable_get(&stateAry[SYS_ACCESS_WEB_HTTP]);
#ifdef CONFIG_USER_BOA_WITH_SSL
    sal_sys_httpsEnable_get(&stateAry[SYS_ACCESS_WEB_HTTPS]);
#endif

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    sal_sys_cliSessionTimeout_get(SYS_LINE_CLI_CONSOLE, &timeoutAry[SYS_ACCESS_CLI_CONSOLE]);
    sal_sys_cliSessionTimeout_get(SYS_LINE_CLI_TELNET, &timeoutAry[SYS_ACCESS_CLI_TELNET]);
  #ifdef CONFIG_USER_SSH_SSHD
    sal_sys_cliSessionTimeout_get(SYS_LINE_CLI_SSH, &timeoutAry[SYS_ACCESS_CLI_SSH]);
  #endif
#endif
    sal_sys_webSessionTimeout_get(SYS_LINE_WEB_HTTP, &timeoutAry[SYS_ACCESS_WEB_HTTP]);
    sal_sys_webSessionTimeout_get(SYS_LINE_WEB_HTTPS, &timeoutAry[SYS_ACCESS_WEB_HTTPS]);

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    sal_sys_cliRetrySilent_get(SYS_LINE_CLI_CONSOLE, &retrySilentAry[SYS_ACCESS_CLI_CONSOLE]);
    sal_sys_cliRetrySilent_get(SYS_LINE_CLI_TELNET, &retrySilentAry[SYS_ACCESS_CLI_TELNET]);
  #ifdef CONFIG_USER_SSH_SSHD
    sal_sys_cliRetrySilent_get(SYS_LINE_CLI_SSH, &retrySilentAry[SYS_ACCESS_CLI_SSH]);
  #endif
#endif

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    dfltTimeoutAry[SYS_ACCESS_CLI_CONSOLE] = DFLT_SYS_SESSION_TIMEOUT_CONSOLE;
    dfltTimeoutAry[SYS_ACCESS_CLI_TELNET] = DFLT_SYS_SESSION_TIMEOUT_TELNET;
  #ifdef CONFIG_USER_SSH_SSHD
    dfltTimeoutAry[SYS_ACCESS_CLI_SSH] = DFLT_SYS_SESSION_TIMEOUT_SSH;
  #endif
#endif
    dfltTimeoutAry[SYS_ACCESS_WEB_HTTP] = DFLT_SYS_SESSION_TIMEOUT_HTTP;
#ifdef CONFIG_USER_BOA_WITH_SSL
    dfltTimeoutAry[SYS_ACCESS_WEB_HTTPS] = DFLT_SYS_SESSION_TIMEOUT_HTTPS;
#endif

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    dfltRetryAry[SYS_ACCESS_CLI_CONSOLE] = DFLT_CLI_PASSRETRY_CONSOLE;
    dfltRetryAry[SYS_ACCESS_CLI_TELNET] = DFLT_CLI_PASSRETRY_TELNET;
  #ifdef CONFIG_USER_SSH_SSHD
    dfltRetryAry[SYS_ACCESS_CLI_SSH] = DFLT_CLI_PASSRETRY_SSH;
  #endif

    dfltSilentAry[SYS_ACCESS_CLI_CONSOLE] = DFLT_CLI_SILENTTIME_CONSOLE;
    dfltSilentAry[SYS_ACCESS_CLI_TELNET] = DFLT_CLI_SILENTTIME_TELNET;
  #ifdef CONFIG_USER_SSH_SSHD
    dfltSilentAry[SYS_ACCESS_CLI_SSH] = DFLT_CLI_SILENTTIME_SSH;
  #endif
#endif

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_SYS_LINE_TIMEOUT_MIN, BOND_SYS_LINE_TIMEOUT_MIN);
    cJSON_AddNumberToObject(data, CGI_SYS_LINE_TIMEOUT_MAX, BOND_SYS_LINE_TIMEOUT_MAX);
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    cJSON_AddNumberToObject(data, CGI_SYS_LINE_RETRY_MIN, BOND_CLI_PASSRETRY_MIN);
    cJSON_AddNumberToObject(data, CGI_SYS_LINE_RETRY_MAX, BOND_CLI_PASSRETRY_MAX);
    cJSON_AddNumberToObject(data, CGI_SYS_LINE_SILENT_MIN, BOND_CLI_SILENTTIME_MIN);
    cJSON_AddNumberToObject(data, CGI_SYS_LINE_SILENT_MAX, BOND_CLI_SILENTTIME_MAX);
#endif

    methods = cJSON_CreateArray();
    for (i = 0; i < SYS_ACCESS_END; i++)
    {
#ifndef CONFIG_SYS_UI_CLI_LINE_MGMT
        if (i < SYS_ACCESS_WEB_HTTP)
            continue;
#endif
        if (SYS_ACCESS_CLI_CONSOLE == i)
            continue;

        method = cJSON_CreateObject();

        cJSON_AddNumberToObject(method, CGI_VALUE, i);
        cJSON_AddStringToObject(method, CGI_TXT, cgi_text_sys_line[i].text);
        cJSON_AddBoolToObject(method, CGI_SYS_LINE_STATE, stateAry[i]);

        cJSON_AddItemToArray(methods, method);
    }
#ifdef CONFIG_SYS_UI_SNMP
    {
        sys_enable_t snmpState = DISABLED;

        sal_snmp_enable_get(&snmpState);

        method = cJSON_CreateObject();

        cJSON_AddNumberToObject(method, CGI_VALUE, CGI_SYS_LINE_METHOD_SNMP_VAL);
        cJSON_AddLangStringToObject(method, CGI_TXT, "line", "lblSnmp");
        cJSON_AddBoolToObject(method, CGI_SYS_LINE_STATE, snmpState);
        cJSON_AddItemToArray(methods, method);
    }
#endif

    cJSON_AddItemToObject(data, CGI_SYS_LINE_METHODS, methods);

    timeouts = cJSON_CreateArray();
    for (i = 0; i < SYS_ACCESS_END; i++)
    {
#ifndef CONFIG_SYS_UI_CLI_LINE_MGMT
        if (i < SYS_ACCESS_WEB_HTTP)
            continue;
#endif
        timeout = cJSON_CreateObject();

        cJSON_AddNumberToObject(timeout, CGI_VALUE, i);
        cJSON_AddStringToObject(timeout, CGI_TXT, cgi_text_sys_line[i].text);
        cJSON_AddNumberToObject(timeout, CGI_SYS_LINE_TIMEOUT, timeoutAry[i]);
        cJSON_AddNumberToObject(timeout, CGI_SYS_LINE_TIMEOUT_DFLT, dfltTimeoutAry[i]);

        cJSON_AddItemToArray(timeouts, timeout);
    }
    cJSON_AddItemToObject(data, CGI_SYS_LINE_TIMEOUTS, timeouts);

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    
    retries = cJSON_CreateArray();
    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        if (SYS_ACCESS_WEB_HTTP == i || SYS_ACCESS_WEB_HTTPS == i)
            continue;

        retry = cJSON_CreateObject();

        cJSON_AddNumberToObject(retry, CGI_VALUE, i);
        cJSON_AddStringToObject(retry, CGI_TXT, cgi_text_sys_line[i].text);
        cJSON_AddNumberToObject(retry, CGI_SYS_LINE_RETRY, retrySilentAry[i].retryNum);
        cJSON_AddNumberToObject(retry, CGI_SYS_LINE_RETRY_DFLT, dfltRetryAry[i]);

        cJSON_AddItemToArray(retries, retry);
    }
    cJSON_AddItemToObject(data, CGI_SYS_LINE_RETRIES, retries);

    silents = cJSON_CreateArray();
    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        if (SYS_ACCESS_WEB_HTTP == i || SYS_ACCESS_WEB_HTTPS == i)
            continue;

        silent = cJSON_CreateObject();

        cJSON_AddNumberToObject(silent, CGI_VALUE, i);
        cJSON_AddStringToObject(silent, CGI_TXT, cgi_text_sys_line[i].text);
        cJSON_AddNumberToObject(silent, CGI_SYS_LINE_SILENT, retrySilentAry[i].silentTime);
        cJSON_AddNumberToObject(silent, CGI_SYS_LINE_SILENT_DFLT, dfltSilentAry[i]);

        cJSON_AddItemToArray(silents, silent);
    }
    cJSON_AddItemToObject(data, CGI_SYS_LINE_SILENTS, silents);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_ADVANCE_DNS
static int32 cgi_sys_dns_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *dnss = NULL;
    cJSON *dns = NULL;
    uint32 i = 0;
    uint32 cnt = 0;
    char domain[SYS_BUF256_LEN];
    sys_enable_t enable = DISABLED;
    sys_advDnsIP_t ns;

    SYS_MEM_CLEAR(domain);

    sal_sys_dnsLookup_get(&enable);
    sal_sys_dnsDomain_get(domain, SYS_ADVDNS_SRC_STATIC);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_MAXLEN, CAPA_SYS_DNS_NAMESERVER_NUM);
    cJSON_AddNumberToObject(data, CGI_STATE, enable);
    cJSON_AddStringToObject(data, CGI_SYS_HOSTNAME, domain);

    dnss = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_SYS_DNSS, dnss);

    for (i = 0 ; i < CAPA_SYS_DNS_STATIC_HOST_NUM ; i++)
    {
        SYS_MEM_CLEAR(ns);

        sal_sys_dnsNameServer_get(i, &ns, SYS_ADVDNS_SRC_STATIC);

        if (0 == osal_strlen(ns.ip))
            continue;

        dns = cJSON_CreateObject();
        cJSON_AddItemToArray(dnss, dns);

        cJSON_AddStringToObject(dns, CGI_SYS_IPV4_IP, ns.ip);
        cJSON_AddNumberToObject(dns, CGI_SYS_PREF, i);
    }

    for (i = 0 ; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
    {
        SYS_MEM_CLEAR(ns);

        sal_sys_dnsNameServer_get(i, &ns, SYS_ADVDNS_SRC_DHCPV4);

        if (0 == osal_strlen(ns.ip))
            continue;

        dns = cJSON_CreateObject();
        cJSON_AddItemToArray(dnss, dns);

        cJSON_AddStringToObject(dns, CGI_SYS_IPV4_IP, ns.ip);
        cJSON_AddNumberToObject(dns, CGI_SYS_PREF, cnt++);
        cJSON_AddNumberToObject(dns, CGI_IDX, (i + (1 << 8)));
    }

    for (i = 0 ; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
    {
        SYS_MEM_CLEAR(ns);

        sal_sys_dnsNameServer_get(i, &ns, SYS_ADVDNS_SRC_DHCPV6);

        if (0 == osal_strlen(ns.ip))
            continue;

        dns = cJSON_CreateObject();
        cJSON_AddItemToArray(dnss, dns);

        cJSON_AddStringToObject(dns, CGI_SYS_IPV4_IP, ns.ip);
        cJSON_AddNumberToObject(dns, CGI_SYS_PREF, cnt++);
        cJSON_AddNumberToObject(dns, CGI_IDX, (i + (1 << 16)));
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_sys_dnsHosts_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *hosts = NULL;
    cJSON *dhosts = NULL;
    cJSON *host = NULL;
    FILE *fp = NULL;
    char buf[SYS_BUF256_LEN];
    char v6addrStr[CAPA_IPV6_STR_LEN];
    char *ip = NULL;
    char *hostname = NULL;
    char *total = NULL;
    char *elapsed = NULL;
    char *type = NULL;
    sys_ipv6_t v6addr;

    uint32 i = 0;

    sys_advDns_staticHost_t statichost;

    CGI_GET_JSON_CREATE(root, data);
    hosts = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_SYS_HOSTS, hosts);

    for (i = 0 ; i < CAPA_SYS_DNS_STATIC_HOST_NUM ; i++)
    {
        SYS_MEM_CLEAR(statichost);
        SYS_ERR_CHK(sal_sys_dnsStaticHost_get(i, &statichost));

        if (0 == osal_strlen(statichost.host))
            continue;

        if (0 == osal_strlen(statichost.ip.ip))
            continue;

        host = cJSON_CreateObject();
        cJSON_AddItemToArray(hosts, host);
        cJSON_AddStringToObject(host, CGI_SYS_HOSTNAME, statichost.host);
        cJSON_AddStringToObject(host, CGI_SYS_IPV4_IP, statichost.ip.ip);
    }

    if (NULL != (fp = popen("cat /proc/"SYS_DNS_PROC_FILE, "r")))
    {
        dhosts = cJSON_CreateArray();
        cJSON_AddItemToObject(data, CGI_SYS_DHOSTS, dhosts);

        while (fgets(buf, sizeof(buf), fp))
        {
            ip = strtok(buf, ", ");

            if ('Z' == ip[0]) 
                break;

            hostname = strtok(NULL, ", ");
            type = strtok(NULL, ", ");
            total = strtok(NULL, ", ");
            elapsed = strtok(NULL, ", ");

            host = cJSON_CreateObject();
            cJSON_AddItemToArray(dhosts, host);
            cJSON_AddStringToObject(host, CGI_SYS_HOSTNAME, hostname);
            cJSON_AddStringToObject(host, CGI_SYS_DNS_TOTAL, total);
            cJSON_AddStringToObject(host, CGI_SYS_DNS_ELAPSED, elapsed);
            cJSON_AddStringToObject(host, CGI_TYPE, type);

            if (SYS_STR2UINT(type) == SYS_L3_AF_IPV6)
            {
                SYS_MEM_CLEAR(v6addr);
                SYS_MEM_CLEAR(v6addrStr);
                STR2IPV6(ip, &v6addr);
                IPV62STR(v6addr, v6addrStr);
                cJSON_AddStringToObject(host, CGI_SYS_IPV4_IP, v6addrStr);
            }
            else
            {
                cJSON_AddStringToObject(host, CGI_SYS_IPV4_IP, ip);
            }
        }

        pclose(fp);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_sys_sysTime_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    int32 updays, uphrs, upmins, upsecs;
    struct sysinfo info;
    sys_time_str_t timeStr;
#ifdef CONFIG_SYS_BOARD_TEMP
	char currTemp[SYS_BUF32_LEN];
	SYS_MEM_CLEAR(currTemp);
	sal_sys_temperature_get(currTemp);
#endif

    SYS_MEM_CLEAR(timeStr);
    sal_time_currentTimeStr_get(&timeStr);

    sysinfo(&info);
    updays = (int) info.uptime / (60*60*24);
    upmins = (int) info.uptime / 60;
    uphrs = (upmins / 60) % 24;
    upmins %= 60;
    upsecs = (int) info.uptime % 60;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddLangStringArgToObject(data, CGI_SYS_UPTIME, "sys", "txtSysUptimeArg", "%d,%d,%d,%d", updays, uphrs, upmins, upsecs);
    cJSON_AddStringToObject(data, CGI_SYS_CURRTIME, timeStr.tstr);
#ifdef CONFIG_SYS_BOARD_TEMP
	cJSON_AddLangStringArgToObject(data, CGI_SYS_CURTEMP, "sys", "txtSysUpTempArg", "%s", currTemp);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

