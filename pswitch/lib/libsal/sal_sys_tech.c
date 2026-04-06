
#include <dirent.h>
#include <sys/sysinfo.h>
#include <libcmd/cmd.h>
#include <rtk_flash_common.h>

#include <libsal/sal_mib.h>
#include <ski/chip/ski_debug.h>

#ifdef CONFIG_SYS_FIBER
#include <libsal/sal_fiber.h>
#include <libsal/sal_optical.h>
#endif
#include <libsal/sal_cablediag.h>
#include <libsal/sal_util.h>
#include <libsal/sal_sys_tech.h>
#include <libsal/sal_l3.h>
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
#include <libsal/sal_igmp.h>
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#include <libsal/sal_mld.h>
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
#include <libsal/sal_udld.h>
#endif

typedef int32 (*sal_tech_show_func)(FILE *fp);
typedef struct sal_tech_tbl_s
{
    sal_tech_show_func showFunc;
    char               *title;
    char               *key;
} sal_tech_tbl_t;

typedef struct {
    int pid;
    char user[9];
    char state[4];
    unsigned long rss;
    int ppid;
    unsigned pcpu;
    unsigned long stime, utime;

    char short_cmd[16];
} procps_status_t;

static const char* mibDebugGlbCntrName[] =
{
    "ALE RX good packets",
    " 1: MAC RX drop(CRC error or MAX_LEN_RX)",
    " 2: ACL forwarding action",
    " 3: Attack prevention drop",
    " 4: RMA",
    " 5: VLAN Ingress filter",
    " 6: CFI(inner/outer CFI=1)",
    " 7: SA port move",
    " 8: New SA",
    " 9: System MAC constraint",
    "10: VLAN-based MAC constraint",
    "11: Port-based MAC constraint",
    "12: Switch MAC",
    "13: Routing exception/Route invalid NHP(next hop entry)",
    "14: DA_LK_MISS(L2UC/L2MC/IPMC/IPv6MC lookup miss)",
    "15: RSPAN",
    "16: ACL_LK_MISS",
    "17: ACL_DROP",
    "18: IBC(Ingress Bandwidth Control/Input queue drop)",
    "19: ACL_METER(ACL meter)",
    "20: ACCEPT_FRAME_TYPE(Acceptable frame type)",
    "21: STP_IGR(Spanning Tree Protocol ingress filter)",
    "22: Invalid SA(Multicast/Broadcast SA)",
    "23: SA block",
    "24: DA block",
    "25: Invalid DPM in L2 entry",
    "26: DPM in multicast table entry is NULL",
    "27: Rx flow control",
    "28: Storm suppression",
    "29: Link aggregation load sharing",
    "30: Egress VLAN filter",
    "31: Egress STP filter",
    "32: Source port self filter",
    "33: Port isolation",
    "34: ACL filter action",
    "35: Mirroring port normal traffic egress filter",
    "36: TX_MAX_LEN",
    "37: Link down/disable Tx",
    "38: Flow control",
    "1: Error packet",
    "2: Egress ACL drop",
    "3: Egress meter drop ",
    "4: OAM",
    "5: CFM",
    "6: VLAN ingress filter",
    "7: VLAN Error(VID=4095 or MBR=0)",
    "8: inner/outer CFI=1",
    "9: VLAN tag format",
    "10: Source port spending tree",
    "11: Input bandwidth",
    "12: RMA",
    "13: hardware attack prevention",
    "14: protocol storm",
    "15: multicast SA",
    "16: Ingress ACL drop",
    "17: Ingress Meter drop",
    "18: Default action for miss ACL &C2SC",
    "19: New SA",
    "20: Port move",
    "21: SA blocking",
    "22: routing exception",
    "23: Source port spending tree(non forwarding)",
    "24: MAC limit",
    "25: Unknow storm",
    "26: Miss Drop",
    "27: CPU MAC drop",
    "28: DA blocking",
    "29: Source port filter(before Egress ACL)",
    "30: VLAN egress filter",
    "31: Spanning tree",
    "32: Port isolation",
    "33: OAM",
    "34: Mirror isolation",
    "35: Max length(before Egress ACL)",
    "36: Source port filter(before Mirror)",
    "37: Max length(before Mirror)",
    "38: Special congest(before Mirror)",
    "39: Link status(before Mirror)",
    "40: WRED(before Mirror)",
    "41: Max length(after Mirror)",
    "42: Special congest(after Mirror)",
    "43: Link status(after Mirror)",
    "44: WRED(after Mirror)",
};

#ifdef CONFIG_SYS_PROTO_UDLD
const sys_text_t text_udldMode[] =
{
    {         "Disabled"    },
    {           "Enabled"     },
    {       "Enabled / in aggressive mode"     },
};
#endif

#define TECH_PRINTF(fp, fmt, args...)   fprintf(fp, fmt, ## args)

#if 0
int32 _sal_tech_console_append(int32 (*fun)(void))
{
    FILE *tmpFP = NULL;

    SYS_PARAM_CHK((NULL == fun), SYS_ERR_NULL_POINTER);

    if (NULL == (tmpFP = freopen(CONF_TECH_SUPPORT_FILE, "a", stdout)))
    {
        return SYS_ERR_FILE_OPEN;
    }

    fun();
    system("dmesg -c -s 250000");

    tmpFP = freopen("/dev/tty", "a", stdout);

    return SYS_ERR_OK;
}
#endif

int32 _sal_tech_system_append(FILE *fp, char *cmd)
{
    FILE *tmpFP = NULL;
    char tmpCMD[MAX_CMD_STRLEN];
    char cmdstr[MAX_CMD_STRLEN];
    SYS_MEM_CLEAR(tmpCMD);

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);
    SYS_PARAM_CHK((NULL == cmd), SYS_ERR_NULL_POINTER);

    osal_sprintf(tmpCMD, "%s\n", cmd);

    if (NULL == (tmpFP = popen(tmpCMD, "r")))
    {
        return SYS_ERR_FILE_OPEN;
    }

    while (fgets(cmdstr, MAX_CMD_STRLEN, tmpFP))
    {
        cmdstr[osal_strlen(cmdstr) - 1] = '\0';
        TECH_PRINTF(fp, "%s\n", cmdstr);
        SYS_MEM_CLEAR(cmdstr);
    }
    pclose(tmpFP);

    return SYS_ERR_OK;
}

int32 _sal_tech_sys_info_show(FILE *fp)
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
	char sysModel[CAPA_SYS_NAME_LEN];

    int32 updays, uphrs, upmins, upsecs;
    struct sysinfo info;
    struct tm *current_time;
    time_t current_secs;
	uint32 ip = 0;
    uint32 netmask = 0;
    sys_vid_t dfltVid = 0;
    sys_l3_ipv4Db_t *pCurr = NULL;
    int32 i;

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    SYS_MEM_CLEAR(macaddr_str);
    SYS_MEM_CLEAR(ipaddr_str);
    SYS_MEM_CLEAR(netmask_str);
    SYS_MEM_CLEAR(loaderVer);
    SYS_MEM_CLEAR(loaderDate);
    SYS_MEM_CLEAR(name);
    SYS_MEM_CLEAR(location);
    SYS_MEM_CLEAR(contact);
    SYS_MEM_CLEAR(sysModel);

    SYS_ERR_CHK(sal_sys_bdinfoVar_get("modelname", sysModel));
    SYS_ERR_CHK(sal_sys_systemName_get(name));
    SYS_ERR_CHK(sal_sys_systemLocation_get(location));
    SYS_ERR_CHK(sal_sys_systemContact_get(contact));
    TECH_PRINTF(fp, "Model            : %s\n", sysModel);
    TECH_PRINTF(fp, "System Name      : %s\n", name);
    TECH_PRINTF(fp, "System Location  : %s\n", location);
    TECH_PRINTF(fp, "System Contact   : %s\n", contact);

    SYS_ERR_CHK(sal_sys_macAddr_get(&macaddr));
    SYS_ERR_CHK(sys_util_mac2Str(macaddr.octet, macaddr_str));
    TECH_PRINTF(fp, "MAC Address      : %s\n", macaddr_str);
	{
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
	}
    TECH_PRINTF(fp, "IP Address       : %s\n", ipaddr_str);
    TECH_PRINTF(fp, "Subnet Mask      : %s\n", netmask_str);

    SYS_ERR_CHK(sal_sys_loaderInfo_get(loaderVer, loaderDate));
    TECH_PRINTF(fp, "Loader Version   : %s\n", loaderVer);
    TECH_PRINTF(fp, "Loader Date      : %s\n", loaderDate);

    TECH_PRINTF(fp, "Firmware Version : %s\n", DFLT_SYS_VER);
    TECH_PRINTF(fp, "Firmware Date    : %s\n", SYS_FIRMWARE_DATE);

    time(&current_secs);
    current_time = localtime(&current_secs);
    sysinfo(&info);
    updays = (int) info.uptime / (60*60*24);
    upmins = (int) info.uptime / 60;
    uphrs = (upmins / 60) %24;
    upmins %= 60;
    upsecs = current_time->tm_sec;
    TECH_PRINTF(fp, "System Up Time   : %d days, %d hours, %d mins, %d secs\n", updays, uphrs, upmins, upsecs);

    return SYS_ERR_OK;

}

int32 _sal_tech_time_show(FILE *fp)
{
    sys_time_str_t timeStr;
    sys_time_t systime;

    SYS_MEM_CLEAR(timeStr);
    SYS_MEM_CLEAR(systime);
    SYS_ERR_CHK(sal_time_get(&systime));
    SYS_ERR_CHK(sal_time_currentTimeStr_get(&timeStr));

    TECH_PRINTF(fp, "\n%s\n", timeStr.tstr);

    if (SNTP_DISABLED == systime.sntpEnable)
        TECH_PRINTF(fp, "Time set manually\n\n");
    else
        TECH_PRINTF(fp, "Time source is sntp\n\n");

    TECH_PRINTF(fp, "Time zone:\n");
    TECH_PRINTF(fp, "Acronym is %s\n", systime.tzAcronym);
    TECH_PRINTF(fp, "Offset is %s\n\n", timeStr.tzstr);

    if (0 == systime.dls.dls_offset)
        return SYS_ERR_OK;

    TECH_PRINTF(fp, "Summertime:\n");
    TECH_PRINTF(fp, "Acronym is %s\n", systime.dls.dlsAcronym);

    if (SYS_TIME_DLS_TYPE_NON_RECURRING == systime.dls.dlsType)
    {
        TECH_PRINTF(fp, "Starting and ending on a specific date.\n");
        TECH_PRINTF(fp, "Begins at %u %u %u %u:%u\n", systime.dls.dls_non_month_start, systime.dls.dls_non_date_start,
            systime.dls.dls_non_year_start-2000, systime.dls.dls_non_hour_start, systime.dls.dls_non_min_start);
        TECH_PRINTF(fp, "Ends at %u %u %u %u:%u\n", systime.dls.dls_non_month_end, systime.dls.dls_non_date_end,
            systime.dls.dls_non_year_end-2000, systime.dls.dls_non_hour_end, systime.dls.dls_non_min_end);

        TECH_PRINTF(fp, "Offset is %u minutes.\n", systime.dls.dls_offset);
    }
    else
    {
        TECH_PRINTF(fp, "Recurring every year.\n");
        TECH_PRINTF(fp, "Begins at %u %u %u %u:%u\n", systime.dls.dls_rec_week_start, systime.dls.dls_rec_day_start,
            systime.dls.dls_rec_month_start, systime.dls.dls_rec_hour_start, systime.dls.dls_rec_min_start);
        TECH_PRINTF(fp, "Ends at %u %u %u %u:%u\n", systime.dls.dls_rec_week_end, systime.dls.dls_rec_day_end,
            systime.dls.dls_rec_month_end, systime.dls.dls_rec_hour_end, systime.dls.dls_rec_min_end);
        TECH_PRINTF(fp, "Offset is %u minutes.\n", systime.dls.dls_offset);
    }

    TECH_PRINTF(fp, "\n");

#ifdef CONFIG_SYS_APP_MSNTP
    if (SNTP_DISABLED == systime.sntpEnable)
        TECH_PRINTF(fp, "SNTP is Disabled\n");
    else
        TECH_PRINTF(fp, "SNTP is Enabled\n");

    TECH_PRINTF(fp, "SNTP Server address: %s\n", systime.sntpServer);
    TECH_PRINTF(fp, "SNTP Server port: %u\n\n", systime.sntpPort);
#endif

    return SYS_ERR_OK;

}

int32 _sal_tech_sys_lineUsers_show(FILE *fp)
{
    uint32 i;
    sys_line_session_t session;

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    sal_sys_lineSessionInfo_clear();

    TECH_PRINTF(fp, "   Username       Protocol          Location\n");
    TECH_PRINTF(fp, "--------------- ------------ -----------------------\n");

    SYS_MEM_CLEAR(session);
    sal_sys_lineSession_get(SYS_ACCESS_CLI_CONSOLE, 0, &session);
    TECH_PRINTF(fp, "%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_CLI_CONSOLE].text, "0.0.0.0");

    for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_CLI_TELNET); i++)
    {
        SYS_MEM_CLEAR(session);
        SYS_ERR_CONTINUE(sal_sys_lineSession_get(SYS_ACCESS_CLI_TELNET, i, &session));

        TECH_PRINTF(fp, "%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_CLI_TELNET].text, session.host);
    }

#ifdef CONFIG_USER_SSH_SSHD
    for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_CLI_SSH); i++)
    {
        SYS_MEM_CLEAR(session);
        SYS_ERR_CONTINUE(sal_sys_lineSession_get(SYS_ACCESS_CLI_SSH, i, &session));

        TECH_PRINTF(fp, "%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_CLI_SSH].text, session.host);
    }
#endif

    for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_WEB_HTTP); i++)
    {
        SYS_MEM_CLEAR(session);
        SYS_ERR_CONTINUE(sal_sys_lineSession_get(SYS_ACCESS_WEB_HTTP, i, &session));

        TECH_PRINTF(fp, "%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_WEB_HTTP].text, session.host);
    }

    for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_WEB_HTTPS); i++)
    {
        SYS_MEM_CLEAR(session);
        SYS_ERR_CONTINUE(sal_sys_lineSession_get(SYS_ACCESS_WEB_HTTPS, i, &session));

        TECH_PRINTF(fp, "%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_WEB_HTTPS].text, session.host);
    }

    return SYS_ERR_OK;

}

int32 _sal_tech_sys_startup_config_show(FILE *fp)
{
    FILE *startupfp;
    char fullname[CAPA_FILE_FULLNAME_LEN];
    char filename[CAPA_FILE_NAME_LEN];
    char cmdstr[MAX_CMD_STRLEN];

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    SYS_MEM_CLEAR(fullname);
    SYS_MEM_CLEAR(filename);

    osal_strcpy(fullname, CONF_STARTUP_FILE);
    osal_strcpy(filename, CONF_STARTUP_BASENAME);

    if ((NULL == (startupfp = fopen(fullname , "r"))))
    {
        TECH_PRINTF(fp, "Config file %s is not existed\n", filename);
        return SYS_ERR_FILE_OPEN;
    }

    SYS_MEM_CLEAR(cmdstr);

    while (fgets(cmdstr, MAX_CMD_STRLEN, startupfp))
    {
        cmdstr[osal_strlen(cmdstr) - 1] = '\0';
        TECH_PRINTF(fp, "%s\n", cmdstr);
        SYS_MEM_CLEAR(cmdstr);
    }

    fclose(startupfp);

    return SYS_ERR_OK;

}

int32 _sal_tech_sys_running_config_show(FILE *fp)
{
    FILE *runningfp;
    char fullname[CAPA_FILE_FULLNAME_LEN];
    char filename[CAPA_FILE_NAME_LEN];
    char cmdstr[MAX_CMD_STRLEN];

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    SYS_MEM_CLEAR(fullname);
    SYS_MEM_CLEAR(filename);

    SYS_ERR_CHK(cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE));
    osal_strcpy(fullname, CONF_RUNNING_FILE);
    osal_strcpy(filename, CONF_RUNNING_BASENAME);

    if ((NULL == (runningfp = fopen(fullname , "r"))))
    {
        TECH_PRINTF(fp, "Config file %s is not existed\n", filename);
        return SYS_ERR_FILE_OPEN;
    }

    SYS_MEM_CLEAR(cmdstr);

    while (fgets(cmdstr, MAX_CMD_STRLEN, runningfp))
    {
        cmdstr[osal_strlen(cmdstr) - 1] = '\0';
        TECH_PRINTF(fp, "%s\n", cmdstr);
        SYS_MEM_CLEAR(cmdstr);
    }

    fclose(runningfp);

    return SYS_ERR_OK;

}

int32 _sal_tech_board_config_show(FILE *fp)
{
    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    _sal_tech_system_append(fp, "cat /proc/board");

    return SYS_ERR_OK;
}

int32 _sal_tech_l3_intf_show(FILE *fp)
{
#ifdef CONFIG_SYS_L3_ROUTE
    char buf[SYS_BUF64_LEN];
    char buf2[SYS_BUF64_LEN];
    char ipStr[CAPA_IPV6_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    uint32 i = 0;
    uint32 glbAddrNum = 0;
    uint32 llAddrNum = 0;
    sys_enable_t adminEnbl = 0;
    sys_enable_t fwdEnbl = 0;
    sys_l3_ipv4Db_t *pCurr = NULL;
    sys_l3_ipv6Db_t *pCurrV6 = NULL;
    sys_l3_intfId_t intfId;
    sys_l3_ipv6DadSts_t dadSts = SYS_L3_IPV6_DADSTS_NONE;

    sal_l3_fwdIPv6Enable_get(&fwdEnbl);

    TECH_PRINTF(fp, "IP Address         I/F      I/F Status Type    Status\n");
    TECH_PRINTF(fp, "                            admin/oper\n");
    TECH_PRINTF(fp, "------------------ -------- ---------- ------- ------\n");

    FOR_EACH_L3_IPV4(i, pCurr)
    {
        
        SYS_MEM_CLEAR(buf);
        IPV42STR(pCurr->ip.addr, ipStr);
        osal_sprintf(buf, "%s/%d", ipStr, pCurr->ip.plen);
        TECH_PRINTF(fp, "%-18s ", buf);

        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&pCurr->intfId, intfName);
        TECH_PRINTF(fp, "%-8s ", intfName);

        sal_l3_adminIntfEnbl_get(&pCurr->intfId, &adminEnbl);
        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s/%s",
                        (adminEnbl) ? "UP" : "DOWN",
                        (IS_L3_INTF_LINKUP(&pCurr->intfId)) ? "UP" : "DOWN");
        TECH_PRINTF(fp, "%-10s ", buf);

        TECH_PRINTF(fp, "%-7s ", (SYS_L3_IPTYPE_STATIC == pCurr->list.type) ? "Static" : "DHCP");

        TECH_PRINTF(fp, "%s ", (FALSE) ? "Not Received" : "Valid");

        TECH_PRINTF(fp, "\n");
    }

    TECH_PRINTF(fp, "\n\n");

    intfId.type = SYS_L3_INTFTYPE_VLAN;
    FOR_EACH_VID(i)
    {
        intfId.id = i;

        if (!IS_L3_INTF_EXIST(&intfId))
            continue;

        if (!IS_L3_INTF_IPV6ENBL(&intfId))
            continue;

        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&intfId, intfName);
        sal_l3_adminIntfEnbl_get(&intfId, &adminEnbl);
        TECH_PRINTF(fp, "%s is %s/%s\n",
                  intfName,
                  (adminEnbl) ? "up" : "down",
                  (IS_L3_INTF_LINKUP(&intfId)) ? "up" : "down");

        FOR_EACH_L3_IPV6_IN_INTF(&intfId, pCurrV6)
        {
            if (SYS_L3_IPTYPE_V6LINKLOCAL != pCurrV6->list.type)
                continue;

            SYS_MEM_CLEAR(ipStr);
            IPV62STR(pCurrV6->ip.addr, ipStr);
            TECH_PRINTF(fp, "IPv6 is enabled, link-local address is %s", ipStr);

            sal_l3_intfIPv6DadStatus_get(&intfId, &pCurrV6->ip, &dadSts);
            if (SYS_L3_IPV6_DADSTS_TENTATIVE == dadSts || SYS_L3_IPV6_DADSTS_DUPLICATE == dadSts)
                TECH_PRINTF(fp, " (%s)\n", text_ipv6_dadSts[dadSts].text);
            else
                TECH_PRINTF(fp, "\n");

            llAddrNum++;
        }

        if (0 == llAddrNum)
        {
            sys_l3_ipv6_t sysLinkLocalAddr;

            SYS_MEM_CLEAR(sysLinkLocalAddr);

            sys_l3_ipv6SysLinkLocal_get(&sysLinkLocalAddr);

            SYS_MEM_CLEAR(ipStr);
            IPV62STR(sysLinkLocalAddr.addr, ipStr);

            TECH_PRINTF(fp, "IPv6 is enabled, link-local address is %s (TENTATIVE)\n", ipStr);
        }

        XMORE("IPv6 Forwarding is %s\n", text_enable[fwdEnbl].text);

        glbAddrNum = 0;
        FOR_EACH_L3_IPV6_IN_INTF(&intfId, pCurrV6)
        {
            if (SYS_L3_IPTYPE_V6LINKLOCAL == pCurrV6->list.type)
                continue;

            if (0 == glbAddrNum)
            {
                TECH_PRINTF(fp, "Global unicast address(es):\n");
                TECH_PRINTF(fp, "IPv6 Global Address                        Type\n");
            }

            glbAddrNum++;

            SYS_MEM_CLEAR(buf);
            SYS_MEM_CLEAR(buf2);
            SYS_MEM_CLEAR(ipStr);

            IPV62STR(pCurrV6->ip.addr, ipStr);
            osal_sprintf(buf, "%s/%d", ipStr, pCurrV6->ip.plen);

            sal_l3_intfIPv6DadStatus_get(&intfId, &pCurrV6->ip, &dadSts);
            if (SYS_L3_IPV6_DADSTS_TENTATIVE == dadSts || SYS_L3_IPV6_DADSTS_DUPLICATE == dadSts)
            {
                char tmp[SYS_BUF8_LEN];
                SYS_MEM_CLEAR(tmp);
                osal_strncpy(tmp, text_ipv6_dadSts[dadSts].text, 3);
                osal_sprintf(buf2, "%s (%s)", buf, tmp);
            }
            else
            {
                osal_strcpy(buf2, buf);
            }

            TECH_PRINTF(fp, "%-42s %s\n", buf2, (SYS_L3_IPTYPE_V6AUTOCONF == pCurrV6->list.type) ? "Auto" : "Manual");
        }

        if (0 == glbAddrNum)
            TECH_PRINTF(fp, "No global unicast address is configured\n");

        if (IS_L3_INTF_LINKUP(&intfId))
        {
            sys_l3_ipv6MaddrInfo_t maddrs;
            uint32 j = 0;

            SYS_MEM_CLEAR(maddrs);

            sal_l3_intfIPv6Maddr_get(&intfId, &maddrs);

            TECH_PRINTF(fp, "Joined group address(es):\n");

            for (j = 0; j < maddrs.num; j++)
            {
                SYS_MEM_CLEAR(ipStr);

                IPV62STR(maddrs.addrs[j], ipStr);

                TECH_PRINTF(fp, "%s\n", ipStr);
            }
        }

        TECH_PRINTF(fp, "\n");
    }

    TECH_PRINTF(fp, "\n============ System Info\n");
    {
        FILE *cmd_fp = NULL;
        char buf[SYS_BUF256_LEN];

        TECH_PRINTF(fp, "------ IPv4 Info\n");

        if (NULL != (cmd_fp = popen("ip -4 addr show", "r")))
        {
            while (fgets(buf, sizeof(buf), cmd_fp))
            {
                TECH_PRINTF(fp, "%s", buf);
            }

            pclose(cmd_fp);
        }

        TECH_PRINTF(fp, "------ IPv6 Info\n");
        if (NULL != (cmd_fp = popen("ip -6 addr show", "r")))
        {
            while (fgets(buf, sizeof(buf), cmd_fp))
            {
                TECH_PRINTF(fp, "%s", buf);
            }

            pclose(cmd_fp);
        }

        TECH_PRINTF(fp, "------ Link Info\n");
        if (NULL != (cmd_fp = popen("ip link show", "r")))
        {
            while (fgets(buf, sizeof(buf), cmd_fp))
            {
                TECH_PRINTF(fp, "%s", buf);
            }

            pclose(cmd_fp);
        }
    }
#else

#endif
    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_ADVANCE_DNS
int32 _sal_tech_advDns_show(FILE *fp)
{
    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_L3_ROUTE
int32 _sal_tech_static_route_show(FILE *fp)
{
    TECH_PRINTF(fp, "============ IPv4 User Routing Table\n");
    {
        char ipStr[CAPA_IPV4_STR_LEN];
        char intfName[CAPA_L3_INTFNAME_LEN];
        uint32 i = 0;
        sys_l3_routeV4InfoDb_t infoDb;

        SYS_MEM_CLEAR(infoDb);
        SYS_ERR_CHK(sal_l3_routeV4Info_get(&infoDb));

        TECH_PRINTF(fp, "Codes: > - best, C - connected, S - static\n\n");

        for (i = 0; i < infoDb.num; i++)
        {
            
            switch (infoDb.info[i].type)
            {
                case SYS_L3_ROUTETYPE_KERNEL:
                    TECH_PRINTF(fp, "D");
                    break;

                case SYS_L3_ROUTETYPE_CONNECT:
                    TECH_PRINTF(fp, "C");
                    break;

                case SYS_L3_ROUTETYPE_STATIC:
                    TECH_PRINTF(fp, "S");
                    break;

                default:
                    TECH_PRINTF(fp, " ");
                    break;
            }

            if (TRUE == infoDb.info[i].select)
                TECH_PRINTF(fp, ">");
            else
                TECH_PRINTF(fp, " ");

            TECH_PRINTF(fp, " ");

            TECH_PRINTF(fp, " ");

            SYS_MEM_CLEAR(ipStr);
            IPV42STR(infoDb.info[i].route.dst, ipStr);
            TECH_PRINTF(fp, "%s/%d ", ipStr, infoDb.info[i].route.dstLen);

            if (SYS_L3_ROUTETYPE_CONNECT == infoDb.info[i].type)
            {
                TECH_PRINTF(fp, "is directly connected, ");
            }
            else
            {
                if (0 == infoDb.info[i].route.dst)
                    TECH_PRINTF(fp, "[1/1] ");
                else
                    TECH_PRINTF(fp, "[%d/%d] ", infoDb.info[i].distance, infoDb.info[i].metric);

                TECH_PRINTF(fp, "via ");

                SYS_MEM_CLEAR(ipStr);
                IPV42STR(infoDb.info[i].route.gw, ipStr);
                TECH_PRINTF(fp, "%s, ", ipStr);
            }

            INTFID2NAME(&infoDb.info[i].route.ointf, intfName);
            TECH_PRINTF(fp, "%s", intfName);

            TECH_PRINTF(fp, "\n");
        }
        TECH_PRINTF(fp, "\n");
    }
    TECH_PRINTF(fp, "============ IPv6 User Routing Table\n");
    {
        char ipStr[CAPA_IPV6_STR_LEN];
        char intfName[CAPA_L3_INTFNAME_LEN];
        uint32 i = 0;
        sys_l3_routeV6InfoDb_t infoDb;

        SYS_MEM_CLEAR(infoDb);
        SYS_ERR_CHK(sal_l3_routeV6Info_get(&infoDb));

        TECH_PRINTF(fp, "Codes: > - best, C - connected, S - static\n\n");

        for (i = 0; i < infoDb.num; i++)
        {
            
            switch (infoDb.info[i].type)
            {
                case SYS_L3_ROUTETYPE_KERNEL:
                    TECH_PRINTF(fp, "D");
                    break;

                case SYS_L3_ROUTETYPE_CONNECT:
                    TECH_PRINTF(fp, "C");
                    break;

                case SYS_L3_ROUTETYPE_STATIC:
                    TECH_PRINTF(fp, "S");
                    break;

                default:
                    TECH_PRINTF(fp, " ");
                    break;
            }

            if (TRUE == infoDb.info[i].select)
                TECH_PRINTF(fp, ">");
            else
                TECH_PRINTF(fp, " ");

            TECH_PRINTF(fp, " ");

            TECH_PRINTF(fp, " ");

            SYS_MEM_CLEAR(ipStr);
            IPV62STR(infoDb.info[i].route.dst, ipStr);
            TECH_PRINTF(fp, "%s/%d ", ipStr, infoDb.info[i].route.dstLen);

            if (SYS_L3_ROUTETYPE_CONNECT == infoDb.info[i].type)
            {
                TECH_PRINTF(fp, "is directly connected, ");
            }
            else
            {
                if (IS_IPV6_ALLZERO(infoDb.info[i].route.dst))
                    TECH_PRINTF(fp, "[1/1] ");
                else
                    TECH_PRINTF(fp, "[%d/%d] ", infoDb.info[i].distance, infoDb.info[i].metric);

                TECH_PRINTF(fp, "via ");

                SYS_MEM_CLEAR(ipStr);
                IPV62STR(infoDb.info[i].route.gw, ipStr);
                TECH_PRINTF(fp, "%s, ", ipStr);
            }

            INTFID2NAME(&infoDb.info[i].route.ointf, intfName);
            TECH_PRINTF(fp, "%s", intfName);

            TECH_PRINTF(fp, "\n");
        }
        TECH_PRINTF(fp, "\n");
    }
    TECH_PRINTF(fp, "\n============ System Routing Table\n");
    {
        FILE *cmd_fp = NULL;
        char buf[SYS_BUF256_LEN];
        uint32 i = 0;
        int32 id = -1;
        sys_l3_intfId_t intfId;

        TECH_PRINTF(fp, "------ DEV to Intf Map\n");
        for (i = 0; i < CAPA_L3_INTFVLAN_NUM; i++)
        {
            SYS_MEM_CLEAR(intfId);
            SYS_MEM_CLEAR(buf);

            intfId.type = SYS_L3_INTFTYPE_VLAN;
            id = INTFIDX2ID(SYS_L3_INTFTYPE_VLAN, i);
            if (id >= 0)
            {
                intfId.id = id;
                INTFID2NAME(&intfId, buf);

                TECH_PRINTF(fp, "%s%d: %s\n", L3_INTF_PREFIX_VLAN, i, buf);
            }
        }

        TECH_PRINTF(fp, "------ IPv4 Info\n");

        if (NULL != (cmd_fp = popen("ip -4 route show", "r")))
        {
            while (fgets(buf, sizeof(buf), cmd_fp))
            {
                TECH_PRINTF(fp, "%s", buf);
            }

            pclose(cmd_fp);
        }

        TECH_PRINTF(fp, "------ IPv6 Info\n");
        if (NULL != (cmd_fp = popen("ip -6 route show", "r")))
        {
            while (fgets(buf, sizeof(buf), cmd_fp))
            {
                TECH_PRINTF(fp, "%s", buf);
            }

            pclose(cmd_fp);
        }

        TECH_PRINTF(fp, "\n");
    }
    TECH_PRINTF(fp, "============ Final Routing Table\n");
    {
        char buf[SYS_BUF256_LEN];
        FILE *procfp = NULL;

        if (NULL == (procfp = fopen("/proc/"L3_PROC_NAME, "r")))
        {
            fclose(fp);
            return SYS_ERR_OK;
        }

        while (fgets(buf, sizeof(buf), procfp))
        {
            TECH_PRINTF(fp, "%s", buf);
        }
        TECH_PRINTF(fp, "\n");

        fclose(procfp);
    }
    TECH_PRINTF(fp, "============ Neighbors\n");
    {
        FILE *procfp = NULL;
        char buf[SYS_BUF256_LEN];
        sys_ipv6_t v6addr;
        char v6addrStr[CAPA_IPV6_STR_LEN];
        char *tok[5];
        uint32 addrNum = 0;

        if (NULL == (procfp = popen("cat /proc/"L3_NEIGH_V4_PROC_NAME, "r")))
            return SYS_ERR_OK;

        TECH_PRINTF(fp, "  VLAN Interface    IP address        HW address       Status  \n");
        TECH_PRINTF(fp, "----------------- --------------- ------------------ ----------\n");

        SYS_MEM_CLEAR(buf);
        while (fgets(buf, sizeof(buf), procfp))
        {
            tok[0] = strtok(buf, ", "); 

            if (!isdigit(tok[0][0])) 
                break;

            tok[1] = strtok(NULL, ", "); 
            tok[2] = strtok(NULL, ", "); 
            tok[3] = strtok(NULL, ", "); 
            tok[4] = strtok(NULL, ", "); 

            TECH_PRINTF(fp, "vlan %-12s %-15s %-18s %s\n", tok[0], tok[1], tok[2], (tok[3][0] == '1') ? "Static": "Dynamic");

            SYS_MEM_CLEAR(buf);
            addrNum++;
        }

        TECH_PRINTF(fp, "\nTotal number of entries: %u\n\n", addrNum);
        pclose(procfp);

        if (NULL == (procfp = popen("cat /proc/"L3_NEIGH_V6_PROC_NAME, "r")))
            return SYS_ERR_OK;

        addrNum = 0;
        TECH_PRINTF(fp, "  VLAN Interface                  IPv6 address                 HW address       Status   Router\n");
        TECH_PRINTF(fp, "----------------- ---------------------------------------- ------------------ ---------- ------\n");

        SYS_MEM_CLEAR(buf);
        while (fgets(buf, sizeof(buf), procfp))
        {
            tok[0] = strtok(buf, ", "); 

            if (!isdigit(tok[0][0])) 
                break;

            tok[1] = strtok(NULL, ", "); 
            tok[2] = strtok(NULL, ", "); 
            tok[3] = strtok(NULL, ", "); 
            tok[4] = strtok(NULL, ", "); 

            SYS_MEM_CLEAR(v6addr);
            SYS_MEM_CLEAR(v6addrStr);
            STR2IPV6(tok[1], &v6addr);
            IPV62STR(v6addr, v6addrStr);
            TECH_PRINTF(fp, "vlan %-12s %-40s %-18s %-10s %s\n", tok[0], v6addrStr, tok[2],
                (tok[3][0] == '3') ? "Static": "Dynamic", (tok[4][0] == '1') ? "Yes": "No");

            SYS_MEM_CLEAR(buf);
            addrNum++;
        }

        TECH_PRINTF(fp, "\nTotal number of entries: %u\n\n", addrNum);
        pclose(procfp);
    }

    return SYS_ERR_OK;
}
#endif

int32 _sal_tech_port_show(FILE *fp)
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

    uint64 outTmp = 0;
    uint64 outPkts = 0;
    uint64 outOctets = 0;
    uint64 outUnderruns = 0;
    uint64 outErrs = 0;
    uint64 outCollisions = 0;
    uint64 outBabbles = 0;
    uint64 outLateCollisions = 0;
    uint64 outDeffered = 0;
    uint64 outPause = 0;

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    LOGIC_PORTMASK_SET_ALL(lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        inPkts = inOctets = inBcasts = inMcasts = inRunts = inGiants = inThrottles = inCRCs = 0;
        inFrames = inErrs = inPause = inDribble = 0;
        outTmp = outPkts = outOctets = outUnderruns = outErrs = outCollisions = 0;
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
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsOctets_get(lp, &inOctets), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsUnderSizePkts_get(lp, &inRunts), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsOverSizePkts_get(lp, &inGiants), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsDropEvents_get(lp, &inThrottles), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsFCSErrors_get(lp, &inCRCs), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3_statsAlignmentErrors_get(lp, &inFrames), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3InPauseFrames_get(lp, &inPause), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsFrameTooLongs_get(lp, &inDribble), lp);
        inErrs = inRunts + inGiants + inThrottles + inCRCs + inFrames;
        
        CMD_ERR_CONTINUE_PORT(sal_mib_ifOutUcastPkts_get(lp, &outPkts), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_ifOutNUcastPkts_get(lp, &outTmp), lp);
        outPkts += outTmp;
        CMD_ERR_CONTINUE_PORT(sal_mib_ifOutOctets_get(lp, &outOctets), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_ifOutDiscards_get(lp, &outUnderruns), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsExcessiveCollisions_get(lp, &outCollisions), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_etherStatsJabbers_get(lp, &outBabbles), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsLateCollisions_get(lp, &outLateCollisions), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3StatsDeferredTransmissions_get(lp, &outDeffered), lp);
        CMD_ERR_CONTINUE_PORT(sal_mib_dot3OutPauseFrames_get(lp, &outPause), lp);
        outErrs = outUnderruns + outCollisions + outBabbles + outLateCollisions +\
                  outDeffered;

        TECH_PRINTF(fp, "%s is %s\n", portStr, (PORT_LINKUP == linkStatus) ? "up" : "down");
        TECH_PRINTF(fp, "  Hardware is %s\n", (PORT_TYPE_1000M == LP2TYPE(lp)) ? "Gigabit Ethernet" : "Fast Ethernet");

        if (ENABLED == duplex.autoDuplex)
            TECH_PRINTF(fp, "  Auto-duplex, ");
        else if (SYS_PORT_DUPLEX_FULL == duplex.ability)
            TECH_PRINTF(fp, "  Full-duplex, ");
        else
            TECH_PRINTF(fp, "  Half-duplex, ");
        
        if (ENABLED == speed.autoSpeed)
            TECH_PRINTF(fp, "Auto-speed, ");
        else if (SYS_PORT_SPEED_10M == speed.ability)
            TECH_PRINTF(fp, "10Mb/s, ");
        else if (SYS_PORT_SPEED_100M == speed.ability)
            TECH_PRINTF(fp, "100Mb/s, ");
        else if (SYS_PORT_SPEED_1000M == speed.ability)
            TECH_PRINTF(fp, "1000Mb/s, ");
        
        if (PORT_MEDIA_COPPER == media)
            TECH_PRINTF(fp, "media type is Copper\n");
        else
            TECH_PRINTF(fp, "media type is Fiber\n");

        if(ENABLED == flowCtrlAuto)
            TECH_PRINTF(fp, "  flow-control is auto\n");
        else
            TECH_PRINTF(fp, "  flow-control is %s\n", (ENABLED == flowCtrl) ? "on" : "off");

        TECH_PRINTF(fp, "  back-pressure is %s\n", (ENABLED == backPressure) ? "enabled" : "disabled");

        TECH_PRINTF(fp, "     %llu packets input, %llu bytes, %llu throttles\n", inPkts, inOctets, inThrottles);
        TECH_PRINTF(fp, "     Received %llu broadcasts (%llu multicasts)\n", inBcasts, inMcasts);
        TECH_PRINTF(fp, "     %llu runts, %llu giants, %llu throttles\n", inRunts, inGiants, inThrottles);
        TECH_PRINTF(fp, "     %llu input errors, %llu CRC, %llu frame\n", inErrs, inCRCs, inFrames);
        TECH_PRINTF(fp, "     %llu multicast, %llu pause input\n", inMcasts, inPause);
        TECH_PRINTF(fp, "     %llu input packets with dribble condition detected\n", inDribble);
        
        TECH_PRINTF(fp, "     %llu packets output, %llu bytes, %llu underrun\n", outPkts, outOctets, outUnderruns);
        TECH_PRINTF(fp, "     %llu output errors, %llu collisions\n", outErrs, outCollisions);
        TECH_PRINTF(fp, "     %llu babbles, %llu late collision, %llu deferred\n", outBabbles, outLateCollisions, outDeffered);
        TECH_PRINTF(fp, "     %llu PAUSE output\n", outPause);
    }

    return SYS_ERR_OK;

}

int32 _sal_tech_port_status_show(FILE *fp)
{
    sys_port_media_t media;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    sys_port_linkStatus_t linkStatus;
    sys_port_speedDuplex_t speedDuplex;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_enable_t enable;
    char port_str[CAPA_PORT_STR_LEN];

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    LOGIC_PORTMASK_SET_ALL(logicPortmask);

    TECH_PRINTF(fp, "Port  Status      Duplex  Speed    Type \n");
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        osal_memset(port_str, 0, sizeof(port_str));
        SYS_ERR_CHK(LP2STR(logicPort, port_str));
        TECH_PRINTF(fp, "%-4s  ", port_str);

        SYS_ERR_CHK(sal_port_enable_get(logicPort, &enable));
        SYS_ERR_CHK(sal_port_linkStatus_get(logicPort, &linkStatus));
        if (DISABLED == enable)
            TECH_PRINTF(fp, "disable     ");
        else if (ENABLED == enable && PORT_LINKUP == linkStatus)
            TECH_PRINTF(fp, "connected   ");
        else
            TECH_PRINTF(fp, "notconnect  ");

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
                    TECH_PRINTF(fp, "a-full  ");
                else if (PORT_HALF_DUPLEX == speedDuplex.duplex)
                    TECH_PRINTF(fp, "a-half  ");
            }
            else
            {
                if (PORT_FULL_DUPLEX == speedDuplex.duplex)
                    TECH_PRINTF(fp, "full    ");
                else if (PORT_HALF_DUPLEX == speedDuplex.duplex)
                    TECH_PRINTF(fp, "half    ");
            }

            if (ENABLED == speed.autoSpeed)
            {
                if (PORT_SPEED_10M == speedDuplex.speed)
                    TECH_PRINTF(fp, "a-10M    ");
                else if (PORT_SPEED_100M == speedDuplex.speed)
                    TECH_PRINTF(fp, "a-100M   ");
                else if (PORT_SPEED_1000M == speedDuplex.speed)
                    TECH_PRINTF(fp, "a-1000M  ");
            }
            else
            {
                if (PORT_SPEED_10M == speedDuplex.speed)
                    TECH_PRINTF(fp, "10M      ");
                else if (PORT_SPEED_100M == speedDuplex.speed)
                    TECH_PRINTF(fp, "100M     ");
                else if (PORT_SPEED_1000M == speedDuplex.speed)
                    TECH_PRINTF(fp, "1000M    ");
            }
        }
        else
        {
            if (ENABLED == duplex.autoDuplex)
            {
                TECH_PRINTF(fp, "auto    ");
            }
            else
            {
                if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                    TECH_PRINTF(fp, "full    ");
                else if (SYS_PORT_DUPLEX_HALF == duplex.ability)
                    TECH_PRINTF(fp, "half    ");
            }

            if (ENABLED == speed.autoSpeed)
            {
                TECH_PRINTF(fp, "auto     ");
            }
            else
            {
                if (SYS_PORT_SPEED_10M == speed.ability)
                    TECH_PRINTF(fp, "10M      ");
                else if (SYS_PORT_SPEED_100M == speed.ability)
                    TECH_PRINTF(fp, "100M     ");
                else if (SYS_PORT_SPEED_1000M == speed.ability)
                    TECH_PRINTF(fp, "1000M    ");
            }
        }

        SYS_ERR_CHK(sal_port_linkMedia_get(logicPort, &media));
        if (PORT_MEDIA_COPPER == media)
            TECH_PRINTF(fp, "Copper");
        else
            TECH_PRINTF(fp, "Fiber");

        TECH_PRINTF(fp, "\n");
    }

    return SYS_ERR_OK;

}

int32 _sal_tech_l2_macAddressTable_show(FILE *fp)
{
    uint32 i;
    uint32 count = 0;
    uint32 tableSize;
    char macStr[SYS_BUF32_LEN];
    char portStr[CAPA_PORT_STR_LEN];
    sys_mac_t cpuMac;
    sys_vid_t mgmtVid = DFLT_VLAN_DFLT_VLAN;
    sys_l2_entry_t *pEntry = NULL;

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    SYS_MEM_CLEAR(portStr);
    SYS_ERR_CHK(sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &tableSize));

    TECH_PRINTF(fp, " VID  | MAC Address       | Type              | Ports          \n");
    TECH_PRINTF(fp, "------+-------------------+-------------------+----------------\n");

    SYS_ERR_CHK(sal_sys_macAddr_get(&cpuMac));
    sys_util_mac2Str(cpuMac.octet, macStr);

    TECH_PRINTF(fp, " %4d | %17s | %-17s | %s \n",
              mgmtVid,
              macStr,
              text_l2_type[SYS_L2_TYPE_MGMT].text,
              "CPU");
    count++;

    for (i = 0; i < tableSize; i++)
    {
        pEntry = L2_SORT_ENTRY_PTR(i);

        if (SYS_L2_TYPE_MGMT == pEntry->type)
        {
            continue;
        }

        if (0 == pEntry->vid)
            continue;

        sys_util_mac2Str(pEntry->mac.octet, macStr);

        SYS_MEM_CLEAR(portStr);
        LP2STR(pEntry->port, portStr);

#ifdef CONFIG_SYS_L3_ROUTE
        if (IS_L2_ENTRY_NEXTHOP(pEntry))
        {
            char tmp[SYS_BUF32_LEN];

            SYS_MEM_CLEAR(tmp);

            osal_sprintf(tmp, "%s (R)", text_l2_type[pEntry->type].text);

            TECH_PRINTF(fp, " %4d | %17s | %-17s | %s \n",
                  pEntry->vid,
                  macStr,
                  tmp,
                  portStr);
        }
        else
#endif
        {
            TECH_PRINTF(fp, " %4d | %17s | %-17s | %s \n",
                      pEntry->vid,
                      macStr,
                      text_l2_type[pEntry->type].text,
                      portStr);
        }

        count++;
    }

    TECH_PRINTF(fp, "\nTotal number of entries: %d\n", count);

    return SYS_ERR_OK;

}

int32 _sal_tech_l2_macAddressCounters_show(FILE *fp)
{
    uint32 usage = 0;
    uint32 available = 0;
    uint32 tableSize = 0;
    sys_l2_dbNum_t entryNums;
    sys_logic_port_t lp;
    char portStr[CAPA_PORT_STR_LEN];

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    SYS_ERR_CHK(sal_l2_sort_start(SYS_L2_SORTMETH_MACVLAN, &tableSize));

    SYS_MEM_CLEAR(entryNums);
    SYS_ERR_CHK(sys_l2_dbNum_get(&entryNums));

    TECH_PRINTF(fp, "Port  Dynamic \n");
    TECH_PRINTF(fp, "----- --------------\n");
    FOR_EACH_LOGIC_PORT(lp)
    {
        osal_memset(portStr, 0, sizeof(portStr));
        SYS_ERR_CHK(LP2STR(lp, portStr));
        TECH_PRINTF(fp, "%-4s  ", portStr);

        TECH_PRINTF(fp, "%d", entryNums.portDynamics[lp]);

#ifdef CONFIG_SYS_L3_ROUTE
        TECH_PRINTF(fp, " (%d)", entryNums.portNexthopsDyn[lp]);
#endif

        TECH_PRINTF(fp, "\n");
    }

    TECH_PRINTF(fp, "----------------\n");
    TECH_PRINTF(fp, "Maximum:   %d\n", entryNums.hwTotal);
    TECH_PRINTF(fp, "CPU:       %d\n", 1);
    TECH_PRINTF(fp, "Dynamic:   %d\n", entryNums.dynamics);
    TECH_PRINTF(fp, "Static:    %d\n", entryNums.statics);
    TECH_PRINTF(fp, "Multicast: %d\n", entryNums.multicasts);
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    TECH_PRINTF(fp, "Blacklist: %d\n", entryNums.blacklist);
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    TECH_PRINTF(fp, "----------------\n");
    TECH_PRINTF(fp, "Routing Total:   %d\n", entryNums.nexthops);
    TECH_PRINTF(fp, "Routing Active:  %d\n", entryNums.nexthopsAct);
    TECH_PRINTF(fp, "Routing Dynamic: %d\n", entryNums.nexthopsDyn);
#endif
    TECH_PRINTF(fp, "----------------\n");

    usage += 1; 
    usage += entryNums.dynamics;
    usage += entryNums.statics;
    usage += entryNums.multicasts;
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    usage += entryNums.blacklist;
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    usage += (entryNums.nexthops - entryNums.nexthopsAct);
#endif

    available = entryNums.hwTotal - usage;

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "Usage:     %d\n", usage);
    TECH_PRINTF(fp, "Available: %d\n", available);

    return SYS_ERR_OK;

}

int32 _sal_tech_acl_show(FILE *fp)
{
    _sal_tech_system_append(fp, "cat /proc/acl_template");
    _sal_tech_system_append(fp, "cat /proc/acl_entry");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_USER_DEFINED_ACL
static int32 _idtoName_convert(sal_acl_proto_id_t *proto, uint32 protoCnt, uint16 id, char *buf)
{
    uint32 i = 0;

    for (i = 0; i < protoCnt ; i++)
    {
        if (proto[i].value == id)
        {
            strcpy(buf, proto[i].proto);
            break;
        }
    }

    return SYS_ERR_OK;
}

int32 _acl_aceMac2cmd_convert(sys_ace_content_t *ace_content, char *buf)
{
    char tmp[SYS_BUF32_LEN];
    uint32 len = 0;
    sys_ace_mac_t *mac = NULL;

    SYS_PARAM_CHK((NULL == ace_content), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == buf), SYS_ERR_NULL_POINTER);

    mac = &ace_content->mac;

    len += sprintf(buf + len , "sequence %u ", ace_content->priority);

    if (SYS_PKT_ACTION_FORWARD == ace_content->action)
        len += sprintf(buf + len , "permit ");
    else
        len += sprintf(buf + len , "deny ");

    if (TRUE == mac->isCareSmac)
    {
        osal_memset(tmp, 0, sizeof(tmp));
        SYS_ERR_CHK(sys_util_mac2Str(mac->smac.octet, tmp));
        len += sprintf(buf + len , "%s/", tmp);
        osal_memset(tmp, 0, sizeof(tmp));
        SYS_ERR_CHK(sys_util_mac2Str(mac->smacMask.octet, tmp));
        len += sprintf(buf + len , "%s ", tmp);
    }
    else
    {
        len += sprintf(buf + len , "any ");
    }

    if (TRUE == mac->isCareDmac)
    {
        osal_memset(tmp, 0, sizeof(tmp));
        SYS_ERR_CHK(sys_util_mac2Str(mac->dmac.octet, tmp));
        len += sprintf(buf + len , "%s/", tmp);
        osal_memset(tmp, 0, sizeof(tmp));
        SYS_ERR_CHK(sys_util_mac2Str(mac->dmacMask.octet, tmp));
        len += sprintf(buf + len , "%s ", tmp);
    }
    else
    {
        len += sprintf(buf + len , "any ");
    }

    if (TRUE == mac->isCareVid)
        len += sprintf(buf + len , "vlan %u ", mac->vid);

    if (TRUE == mac->isCare8021p)
        len += sprintf(buf + len , "cos %u %u ", mac->value8021p, mac->mask8021p);

    if (TRUE == mac->isCareEthtype)
        len += sprintf(buf + len , "ethtype 0x%x ", mac->ethType);

    if (SYS_PKT_ACTION_SHUTDOWN_PORT == ace_content->action)
        len += sprintf(buf + len , "shutdown ");

    len += sprintf(buf + len , "\n");

    return SYS_ERR_OK;
}

int32 _acl_aceIp2cmd_convert(sys_ace_content_t *ace_content, char *buf)
{
    char tmp[SYS_BUF64_LEN];
    uint32 len = 0;
    sys_ace_ip_t *ip = NULL;

    SYS_PARAM_CHK((NULL == ace_content), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == buf), SYS_ERR_NULL_POINTER);

    ip = &ace_content->ip;

#define V4IP2STR(_ip, _mask)                \
do{                                         \
    osal_memset(tmp, 0, sizeof(tmp));       \
    SYS_ERR_CHK(sys_util_ip2Str(_ip, tmp)); \
    len += sprintf(buf + len , "%s/", tmp);  \
    osal_memset(tmp, 0, sizeof(tmp));        \
    SYS_ERR_CHK(sys_util_ip2Str(_mask, tmp));  \
    len += sprintf(buf + len , "%s ", tmp);   \
}while(0);

#define V6IP2STR(_ip, _prefix)                \
do{                                         \
    osal_memset(tmp, 0, sizeof(tmp));       \
    inet_ntop(AF_INET6, _ip, tmp, sizeof(tmp));  \
    len += sprintf(buf + len , "%s/", tmp);  \
    len += sprintf(buf + len , "%u ", _prefix);   \
}while(0);

#define SIP2STR()                \
do{                                         \
    if (ip->isCareSip){                     \
        if (ip->isV4)                       \
        {    V4IP2STR(ip->v4sip, ip->v4sipMask);} \
        else                                \
        {    V6IP2STR(ip->v6sip.hex, ip->v6sip_prefix);}}\
    else                                        \
        len += sprintf(buf + len , "any ");     \
}while(0);

#define DIP2STR()                \
do{                                         \
    if (ip->isCareDip){                     \
        if (ip->isV4)                       \
        {   V4IP2STR(ip->v4dip, ip->v4dipMask);} \
        else                                \
        {   V6IP2STR(ip->v6dip.hex, ip->v6dip_prefix);}}\
    else                                        \
        len += sprintf(buf + len , "any ");     \
}while(0);

#define SPORT2STR(_proto, _protonum)    \
do{                                     \
    sys_l4port_range_t __pr;       \
    osal_memset(&__pr, 0, sizeof(__pr));      \
    if (TRUE == ip->isCareSrcPort) {        \
        osal_memset(tmp, 0, sizeof(tmp));   \
        _idtoName_convert(_proto, _protonum, ip->srcPort, tmp); \
        if (0 == strlen(tmp))                                   \
            len += sprintf(buf + len , "%u ", ip->srcPort);      \
        else                                                     \
            len += sprintf(buf + len , "%s ", tmp);}             \
    else if (TRUE == ip->isCareSrcPortRange) {                   \
        SYS_ERR_CHK(sal_acl_l4port_range_get(&__pr, ip->l4SrcPortRangeIdx));    \
        len += sprintf(buf + len , "%u-%u ", __pr.portStart, __pr.portEnd);} \
    else                                        \
        len += sprintf(buf + len , "any ");     \
}while(0);

#define DPORT2STR(_proto, _protonum)    \
do{                                     \
    sys_l4port_range_t __pr;       \
    osal_memset(&__pr, 0, sizeof(__pr));      \
    if (TRUE == ip->isCareDstPort) {        \
        osal_memset(tmp, 0, sizeof(tmp));   \
        _idtoName_convert(_proto, _protonum, ip->dstPort, tmp); \
        if (0 == strlen(tmp))                                   \
            len += sprintf(buf + len , "%u ", ip->dstPort);      \
        else                                                     \
            len += sprintf(buf + len , "%s ", tmp);}             \
    else if (TRUE == ip->isCareDstPortRange) {                   \
        SYS_ERR_CHK(sal_acl_l4port_range_get(&__pr, ip->l4DstPortRangeIdx));    \
        len += sprintf(buf + len , "%u-%u ", __pr.portStart, __pr.portEnd);} \
    else                                        \
        len += sprintf(buf + len , "any ");     \
}while(0);

    len += sprintf(buf + len , "sequence %u ", ace_content->priority);

    if (SYS_PKT_ACTION_FORWARD == ace_content->action)
        len += sprintf(buf + len , "permit ");
    else
        len += sprintf(buf + len , "deny ");

    if (TRUE == ip->isCareProto)
    {
        if (((ip->isV4) && (1 == ip->protol)) || ((!ip->isV4) && (58 == ip->protol))) 
        {
            len += sprintf(buf + len , "icmp ");

            SIP2STR();
            DIP2STR();

            if (TRUE == ip->isCareICMPType)
            {
                osal_memset(tmp, 0, sizeof(tmp));
                if (ip->isV4)
                    _idtoName_convert(acl_icmp_type, ACL_ICMP_TYPE_NUM, ip->icmpType, tmp);
                else
                    _idtoName_convert(acl_icmpv6_type, ACL_ICMPV6_TYPE_NUM, ip->icmpType, tmp);

                if (0 == strlen(tmp))
                    len += sprintf(buf + len , "%u ", ip->icmpType);
                else
                    len += sprintf(buf + len , "%s ", tmp);
            }
            else
            {
                len += sprintf(buf + len , "any ");
            }

            if (TRUE == ip->isCareICMPCode)
                len += sprintf(buf + len , "%u ", ip->icmpCode);
            else
                len += sprintf(buf + len , "any ");
        }
        else if (6 == ip->protol) 
        {
            len += sprintf(buf + len , "tcp ");

            SIP2STR();
            SPORT2STR(acl_tcp_type, ACL_TCP_TYPE_NUM);
            DIP2STR();
            DPORT2STR(acl_tcp_type, ACL_TCP_TYPE_NUM);

            if (ip->isCareTcpFlag)
            {
                len += sprintf(buf + len , "match-all ");

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_FIN))
                    len += sprintf(buf + len , "%cfin", ip->tcpflag & ((1 << SYS_ACL_TCP_FLAG_FIN)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_SYN))
                    len += sprintf(buf + len , "%csyn", ip->tcpflag & ((1 << SYS_ACL_TCP_FLAG_SYN)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_RST))
                    len += sprintf(buf + len , "%crst", (ip->tcpflag & (1 << SYS_ACL_TCP_FLAG_RST)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_PSH))
                    len += sprintf(buf + len , "%cpsh", (ip->tcpflag & (1 << SYS_ACL_TCP_FLAG_PSH)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_ACK))
                    len += sprintf(buf + len , "%cack", (ip->tcpflag & (1 << SYS_ACL_TCP_FLAG_ACK)) ? '+' : '-');

                if (ip->tcpflagMask & (1 << SYS_ACL_TCP_FLAG_URG))
                    len += sprintf(buf + len , "%curg", (ip->tcpflag & (1 << SYS_ACL_TCP_FLAG_URG)) ? '+' : '-');

                len += sprintf(buf + len , " ");
            }
        }
        else if (17 == ip->protol) 
        {
            len += sprintf(buf + len , "udp ");

            SIP2STR();
            SPORT2STR(acl_udp_type, ACL_UDP_TYPE_NUM);
            DIP2STR();
            DPORT2STR(acl_udp_type, ACL_UDP_TYPE_NUM);
        }
        else
        {
            osal_memset(tmp, 0, sizeof(tmp));

            if (ip->isV4)
                _idtoName_convert(acl_ip_proto, ACL_IP_PROTO_NUM, ip->protol, tmp);
            else
                _idtoName_convert(acl_ipv6_proto, ACL_IPV6_PROTO_NUM, ip->protol, tmp);

            if (0 == strlen(tmp))
                len += sprintf(buf + len , "%u ", ip->protol);
            else
                len += sprintf(buf + len , "%s ", tmp);

            SIP2STR();
            DIP2STR();
        }
    }
    else
    {
         len += sprintf(buf + len , (ip->isV4) ? "ip ": "ipv6 ");
         SIP2STR();
         DIP2STR();
    }

    if (TRUE == ip->isCareDscp)
        len += sprintf(buf + len , "dscp %u ", ip->dscp);
    else if (TRUE == ip->isCareIpPrecedence)
        len += sprintf(buf + len , "precedence %u ", ip->ipPrecedence);

    if (SYS_PKT_ACTION_SHUTDOWN_PORT == ace_content->action)
        len += sprintf(buf + len , "shutdown");

    len += sprintf(buf + len , "\n");

    return SYS_ERR_OK;
}

int32 _sal_tech_user_acl_show(FILE *fp)
{
    char tmp[SYS_BUF512_LEN];
    uint32 i = 0;
    uint32 j = 0;
    uint32 aclType = 0;
    uint32 aclCnt = 0;
    uint32 aceCnt = 0;
    sys_namestr_t name;
    sys_ace_content_t ace_content;

    osal_memset(tmp, 0, sizeof(tmp));

    for (aclType = SYS_ACL_MAC_BASE; aclType < SYS_ACL_TYPE_END; aclType++)
    {
        SYS_ERR_CHK(sal_acl_entry_countByType_get(aclType, &aclCnt));

        for (i = 0; i < aclCnt ; i++)
        {
            osal_memset(&name, 0 , sizeof(name));
            SYS_ERR_CHK(sal_acl_entry_nameByIdx_get(name.str, aclType, i));

            if (SYS_ACL_MAC_BASE == aclType)
                TECH_PRINTF(fp, "\nMAC access list %s\n", name.str);
            else if (SYS_ACL_IPV4_BASE == aclType)
                TECH_PRINTF(fp, "\nIP access list %s\n", name.str);
            else if (SYS_ACL_IPV6_BASE == aclType)
                TECH_PRINTF(fp, "\nIPv6 access list %s\n", name.str);

            SYS_ERR_CHK(sal_ace_entry_count_get(name.str, aclType, &aceCnt));

            for (j = 0; j < aceCnt ; j++)
            {
                osal_memset(tmp, 0 , strlen(tmp));
                osal_memset(&ace_content, 0 , sizeof(ace_content));
                SYS_ERR_CHK(sal_ace_entry_byIdx_get(name.str, &ace_content, aclType, j));

                if (SYS_ACL_MAC_BASE == aclType)
                    _acl_aceMac2cmd_convert(&ace_content, tmp);
                else
                    _acl_aceIp2cmd_convert(&ace_content, tmp);

                TECH_PRINTF(fp, "    %s", tmp);
            }
        }
    }

    TECH_PRINTF(fp, "\n");

    return SYS_ERR_OK;
}
#endif

int32 _sal_tech_vlan_show(FILE *fp)
{
    sys_vid_t vid;
    sys_namestr_t name;
    sys_vlanmask_t vmList;
    sys_vlan_operEntry_t operEntry;
    sys_vlan_portMode_t mode = 0;
    sys_vlan_frameType_t type;
    sys_vlan_portMember_t portMember;
    sys_logic_portmask_t lpmTagged;
    sys_logic_port_t lp;
    sys_enable_t enable;
    sys_enable_t uplink = DISABLED;
    char strVlan[CAPA_VLANLIST_STR_LEN];
    char strPmTagged[CAPA_PORTMASK_STR_LEN];
    char strPmUntagged[CAPA_PORTMASK_STR_LEN];
    char strPort[CAPA_PORT_STR_LEN];
    const char *pVlanTypeStr[] = {
        "None",
        "Default",
        "Static",
        "Dynamic"
    };

    const char *pVlanModeStr[] = {
        "Hybrid",       
        "Access",       
        "Trunk",        
        "Tunnel"        
    };
    char *pStrVlanFrameType[] = { "all",
                                  "tagged-only",
                                  "untagged-only" };

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    
    SYS_ERR_CHK(sal_vlan_mgmtVid_get(&vid));
    SYS_ERR_CHK(sal_vlan_name_get(vid, &name));
    TECH_PRINTF(fp, "Management VLAN-ID : %s(%u)\n", name.str, vid);
#endif

    SYS_ERR_CHK(sal_vlan_defaultVid_get(&vid));
    TECH_PRINTF(fp, "Default VLAN-ID : %u\n", vid);

    SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vmList));
    TECH_PRINTF(fp, "  VID  |     VLAN Name    |        Untagged Ports        |        Tagged Ports          |  Type   \n");
    TECH_PRINTF(fp, "-------+------------------+------------------------------+------------------------------+---------\n");

    FOR_EACH_VID_IN_VLANMASK(vid, vmList)
    {
        if (!IS_VLAN_OPER_ENTRY_EXIST(vid))
            continue;

        SYS_MEM_CLEAR(operEntry);
        SYS_MEM_CLEAR(strPmUntagged);
        SYS_MEM_CLEAR(strPmTagged);
        SYS_ERR_CHK(sal_vlan_operEntry_get(vid, &operEntry));
        LPM2STR(&operEntry.lpmUntag, strPmUntagged);
        LOGIC_PORTMASK_CLEAR_ALL(lpmTagged);
        LOGIC_PORTMASK_ANDNOT(lpmTagged, operEntry.lpmMember, operEntry.lpmUntag);
        LPM2STR(&lpmTagged, strPmTagged);

        TECH_PRINTF(fp, " %5d | %16s | %28s | %28s | %s \n", vid, operEntry.name.str,
              strPmUntagged[0] ? strPmUntagged : "---", strPmTagged[0] ? strPmTagged : "---", pVlanTypeStr[operEntry.type]);
    }
    TECH_PRINTF(fp, "\n");

    FOR_EACH_LOGIC_PORT(lp)
    {
        LP2STR(lp, strPort);
        TECH_PRINTF(fp, "Port : %s\n", strPort);

        SYS_ERR_CHK(sal_vlan_portAdminMode_get(lp, &mode));
        SYS_ERR_CHK(sal_vlan_portAdminRoleUplinkEnable_get(lp, &uplink));

        TECH_PRINTF(fp, "Port Mode : %s%s\n", \
            pVlanModeStr[mode], \
            (ENABLED == uplink)? " (uplink)" : "");

#ifdef CONFIG_SYS_PROTO_GVRP
        
        SYS_ERR_CHK(sal_gvrp_port_enable_get(lp, &enable));

        TECH_PRINTF(fp, "Gvrp Status : %s\n", text_enable[enable].text);
#endif
        
        SYS_ERR_CHK(sal_vlan_portAdminIgrFilterEnable_get(lp, &enable));

        TECH_PRINTF(fp, "Ingress Filtering : %s\n", text_enable[enable].text);

        SYS_ERR_CHK(sal_vlan_portAdminAcceptFrameType_get(lp, &type));

        TECH_PRINTF(fp, "Acceptable Frame Type : %s\n", pStrVlanFrameType[type]);

        SYS_ERR_CHK(sal_vlan_portOperPvid_get(lp, &vid));

        TECH_PRINTF(fp, "Ingress UnTagged VLAN ( NATIVE ) : %d\n", vid);

        SYS_ERR_CHK(sal_vlan_portTrunkAllowedVlan_get(lp, &vmList));
        SYS_ERR_CHK(VMSK2STR(&vmList, strVlan));
        TECH_PRINTF(fp, "Trunking VLANs Enabled: %s\n", strVlan);

#if 0
        
        TECH_PRINTF(fp, "\nPort is member in:\n");
        TECH_PRINTF(fp, " Vlan            Name              Egress rule  \n");
        TECH_PRINTF(fp, "------- ----------------------- -----------------\n");
        FOR_EACH_VID(vid)
        {
            if (!IS_VLAN_OPER_ENTRY_EXIST(vid))
                continue;

            SYS_MEM_CLEAR(operEntry);
            SYS_ERR_CHK(sal_vlan_operEntry_get(vid, &operEntry));

            if (!IS_LOGIC_PORTMASK_PORTSET(operEntry.lpmMember, lp))
                continue;

            TECH_PRINTF(fp, " %4d ", vid);
            TECH_PRINTF(fp, " %21s ", operEntry.name.str);
            if (IS_LOGIC_PORTMASK_PORTSET(operEntry.lpmUntag, lp))
                TECH_PRINTF(fp, " %13s \n", "Untagged");
            else
                TECH_PRINTF(fp, " %13s \n", "Tagged");
        }
#endif

        SYS_ERR_CHK(rsd_vlan_portAdminVlanMember_get(lp, &portMember));

        TECH_PRINTF(fp, "\nForbidden VLANs:\n");
        TECH_PRINTF(fp, " Vlan            Name          \n");
        TECH_PRINTF(fp, "------- -----------------------\n");
        FOR_EACH_VID_IN_VLANMASK(vid, portMember.vmForbidden)
        {
            SYS_ERR_CHK(sal_vlan_name_get(vid, &name));

            TECH_PRINTF(fp, " %4d  %21s \n", vid, name.str);
        }
        TECH_PRINTF(fp, "\n\n");
    }

    _sal_tech_system_append(fp, "cat /proc/vlan");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTOCOL_VLAN
int32 _sal_tech_proto_vlan_show(FILE *fp)
{
    sys_vlan_protoGroup_t  protoGroup;
    uint32 groupIdx;
    char strPort[CAPA_PORT_STR_LEN] = {0};
    sys_logic_port_t logicPort = 0;
    sys_vlan_protoVlanCfg_t vlanCfg;
    sys_enable_t enable;

    SYS_MEM_CLEAR(protoGroup);
    SYS_MEM_CLEAR(vlanCfg);

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "   Group ID    |     Status    |      Type      |        value     \n");
    TECH_PRINTF(fp, "---------------+---------------+----------------+------------------\n");

    for (groupIdx = 0; groupIdx < CAPA_VLAN_PROTO_GROUP_NUM; groupIdx++)
    {
        SYS_ERR_CHK(sal_vlan_protoGroup_get(groupIdx, &protoGroup));

        TECH_PRINTF(fp, "      %u        |", groupIdx + 1);

        if (protoGroup.frametype == FRAME_TYPE_END)
        {
            TECH_PRINTF(fp, "  Disabled     |       --       |         --       \n");
        }
        else
        {
            TECH_PRINTF(fp, "  Enabled      |");
            switch (protoGroup.frametype)
            {
                case FRAME_TYPE_ETHERNET:
                    TECH_PRINTF(fp, "    Ethernet    |");
                    break;

                case FRAME_TYPE_RFC1042:
                    TECH_PRINTF(fp, "     SNAP_1024  |");
                    break;

                case FRAME_TYPE_LLCOTHER:
                    TECH_PRINTF(fp, "   LLC other    |");
                    break;

                default:
                    TECH_PRINTF(fp, "   Not support  |");
                    break;
            }

            TECH_PRINTF(fp, "       0x%04X     \n", protoGroup.framevalue);
        }
    }

    TECH_PRINTF(fp, "\n");

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        LP2STR(logicPort, strPort);

        TECH_PRINTF(fp, "Port %s : \n", strPort);

        for (groupIdx = 0; groupIdx < CAPA_VLAN_PROTO_GROUP_NUM; groupIdx++)
        {

            TECH_PRINTF(fp, "  Group %u \n", groupIdx + 1);

            SYS_ERR_CONTINUE(sal_vlan_portProtoVlan_get(logicPort, groupIdx, &vlanCfg, &enable));

            if (enable == ENABLED)
            {
                TECH_PRINTF(fp, "    Status \t: Enabled\n");
                TECH_PRINTF(fp, "    VLAN ID\t: %u\n", vlanCfg.vid);
            }
            else
            {
                TECH_PRINTF(fp, "    Status \t: Disabled\n");
            }
        }

        TECH_PRINTF(fp, "\n");
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_VOICEVLAN
int32 _sal_tech_voice_vlan_show(FILE *fp)
{
    uint32 aging = 0;
    uint32 index = 0;
    sys_enable_t enable;

    sys_vid_t voiceVid;
    voice_vlan_state_t state;
    sys_vlan_voiceQos_t voiceQos;
    voice_vlan_port_mode_t portMode;
    voice_vlan_cos_mode_t cosMode;
    sys_vlan_voiceVlan_oui_t ouiCfg;
    sys_logic_port_t logicPort;
    char strPort[CAPA_PORT_STR_LEN];
    sys_namestr_t name;

    SYS_ERR_CHK(sal_vlan_voiceVlanState_get(&state));

    TECH_PRINTF(fp, "Administrate Voice VLAN state   : %s\n", (VOICE_VLAN_DISABLED == state) ? "disabled":"enabled");

    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(sal_vlan_voiceVlanQos_get(&voiceQos));
    SYS_ERR_CHK(sal_vlan_voiceVlanAging_get(&aging));
    if(DFLT_VLAN_RESERVED_VLAN != voiceVid)
        TECH_PRINTF(fp, "Voice VLAN ID       : %u\n",voiceVid);
    else
        TECH_PRINTF(fp, "Voice VLAN ID       : %s\n","none (disable)");

    if(VOICE_VLAN_AUTO_ENABLED != state)
    {
        TECH_PRINTF(fp, "Voice VLAN Aging    : %u minutes\n", aging);
        TECH_PRINTF(fp, "Voice VLAN CoS      : %u\n", voiceQos.pri);

        SYS_ERR_CHK(sal_vlan_voiceVlan1pRemarkEnable_get(&enable));
        TECH_PRINTF(fp, "Voice VLAN 1p Remark: %s\n", text_enable[enable].text);
    }

    osal_memset(&ouiCfg, 0, sizeof(sys_vlan_voiceVlan_oui_t));
    osal_memset(&name, 0, sizeof(sys_namestr_t));
    SYS_MEM_CLEAR(strPort);

    SYS_ERR_CHK(sal_vlan_voiceVlanQos_get(&voiceQos));
    SYS_ERR_CHK(sal_vlan_voiceVlanAging_get(&aging));

    TECH_PRINTF(fp, "Voice VLAN Aging    : %u minutes\n", aging);
    TECH_PRINTF(fp, "Voice VLAN CoS      : %u\n", voiceQos.pri);

    SYS_ERR_CHK(sal_vlan_voiceVlan1pRemarkEnable_get(&enable));
    TECH_PRINTF(fp, "Voice VLAN 1p Remark: %s\n", text_enable[enable].text);

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "OUI table\n");
    TECH_PRINTF(fp, "   OUI MAC    |   Description\n");
    TECH_PRINTF(fp, "--------------+-----------------\n");

    for (index = 0; index < CAPA_VLAN_VOICE_OUI_NUM; index++)
    {
        SYS_ERR_CHK(sal_vlan_voiceVlanOui_get(index, &ouiCfg, &name));

        if (DISABLED == ouiCfg.enable)
            continue;

        TECH_PRINTF(fp, "   %02X:%02X:%02X   | %s\n",
            ouiCfg.oui[0], ouiCfg.oui[1], ouiCfg.oui[2],
            name.str);
    }

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "  Port | State    | Port Mode   | Cos Mode\n");
    TECH_PRINTF(fp, "-------+----------+-------------+-----------\n");

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        LP2STR(logicPort, strPort);

        SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(logicPort, &enable));
        SYS_ERR_CHK(sal_vlan_portVoiceVlanMode_get(logicPort, &portMode));
        SYS_ERR_CHK(sal_vlan_portVoiceVlancosMode_get(logicPort, &cosMode));

        TECH_PRINTF(fp, "%-6s | %8s |  %6s     | %3s\n", strPort, enable?"Enabled":"Disabled",
                (VOICE_VLAN_MODE_AUTO == portMode)?"Auto":"Manual",
                (VOICE_VLAN_COS_SRC == cosMode) ? "Src" : "All");
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
int32 _sal_tech_surveillance_vlan_show(FILE *fp)
{
    uint32 i = 0;
    uint32 aging = 0;
    sys_vid_t vid =0;
    sys_pri_t pri =0;
    sys_enable_t enable = DISABLED;
    surveillance_vlan_oui_t oui;
    char strPort[CAPA_PORT_STR_LEN];
    sys_logic_port_t lp;
    surveillance_vlan_port_mode_t portMode;
    surveillance_vlan_cos_mode_t cosMode;

    SYS_ERR_CHK(sal_surveillance_vlanState_get(&enable));
    TECH_PRINTF(fp, "Administrate Surveillance VLAN state   : %s\n", (DISABLED == enable) ? "disabled":"enabled");

    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&vid));
    SYS_ERR_CHK(sal_surveillance_vlanPri_get(&pri));
    SYS_ERR_CHK(sal_surveillance_vlanAging_get(&aging));
    SYS_ERR_CHK(sal_surveillance_vlan1pRemarkEnable_get(&enable));

    if(DFLT_VLAN_RESERVED_VLAN != vid)
        TECH_PRINTF(fp, "Surveillance VLAN ID       : %u\n",vid);
    else
        TECH_PRINTF(fp, "Surveillance VLAN ID       : %s\n","none (disable)");

    TECH_PRINTF(fp, "Surveillance VLAN Aging    : %u minutes\n", aging);
    TECH_PRINTF(fp, "Surveillance VLAN CoS      : %u\n", pri);
    TECH_PRINTF(fp, "Surveillance VLAN 1p Remark: %s\n", text_enable[enable].text);

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "OUI table\n");
    TECH_PRINTF(fp, "  OUI MAC   |  Description\n");
    TECH_PRINTF(fp, "------------+-----------------\n");

    for (i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        SYS_MEM_CLEAR(oui);
        SYS_ERR_CHK(sal_surveillance_vlanOuibyIdx_get(i, &oui));

        if (!IS_OUI_CLEAR(oui.oui))
            TECH_PRINTF(fp, "  %02X:%02X:%02X  | %s\n", oui.oui[0], oui.oui[1], oui.oui[2], oui.ouiName.str);
    }

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "  Port | State    | Port Mode   | Cos Mode\n");
    TECH_PRINTF(fp, "-------+----------+-------------+-----------\n");

    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(strPort);
        LP2STR(lp, strPort);

        SYS_ERR_CHK(sal_surveillance_vlan_portEnable_get(lp, &enable));
        SYS_ERR_CHK(sal_surveillance_vlan_portMode_get(lp, &portMode));
        SYS_ERR_CHK(sal_surveillance_vlan_portCosMode_get(lp, &cosMode));

        TECH_PRINTF(fp, "%-6s | %8s |  %6s     | %3s\n", strPort, enable?"Enabled":"Disabled",
                (SURVEILLANCE_VLAN_PORT_MODE_AUTO == portMode)?"Auto":"Manual",
                (SURVEILLANCE_VLAN_PORT_COS_SRC == cosMode) ? "Src" : "All");
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PORT_ISOLATION
int32 _sal_tech_port_isolation_show(FILE *fp)
{
    sys_logic_port_t logicPort = 0;
    sys_enable_t state;
    char port_str[CAPA_PORT_STR_LEN];

    TECH_PRINTF(fp, " Port   | Protected State\n");
    TECH_PRINTF(fp, "--------+-----------------\n");
    FOR_EACH_LOGIC_PORT(logicPort)
    {
        LP2STR(logicPort, port_str);
        TECH_PRINTF(fp, " %6s |", port_str);

        SYS_ERR_CHK(sal_port_protected_get(logicPort, &state));
        TECH_PRINTF(fp, "%s", text_enable[state].text);
        TECH_PRINTF(fp, "\n");
    }

    return SYS_ERR_OK;
}
#endif

int32 _sal_tech_trunk_show(FILE *fp)
{
    uint32 trunkId;
    char strPortmask[CAPA_PORTMASK_STR_LEN];
    char strPortmaskFinal[CAPA_PORTMASK_STR_LEN*2];
    sys_logic_portmask_t memberPm;
    sys_logic_portmask_t activePm;
    sys_logic_portmask_t standbyPm;

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    TECH_PRINTF(fp, "\n");

    TECH_PRINTF(fp, " Group ID |          Ports\n");
    TECH_PRINTF(fp, "----------+----------------------------------------------\n");
    FOR_EACH_TRUNK(trunkId)
    {
        osal_memset(&memberPm, 0, sizeof(memberPm));
        osal_memset(&activePm, 0, sizeof(activePm));
        osal_memset(&standbyPm, 0, sizeof(standbyPm));

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));
        SYS_ERR_CHK(sal_trunk_activeMember_get(trunkId, &activePm));

        osal_memset(strPortmaskFinal, 0, sizeof(strPortmaskFinal));

        TECH_PRINTF(fp, "  %2d      |", TRK2UID(trunkId));

        LOGIC_PORTMASK_ANDNOT(standbyPm, memberPm, activePm);

        if (!IS_LOGIC_PORTMASK_CLEAR(activePm))
        {
            osal_memset(strPortmask, 0, sizeof(strPortmask));
            LPM2STR(&activePm, strPortmask);
            osal_strcat(strPortmaskFinal, " Active: ");
            osal_strcat(strPortmaskFinal, strPortmask);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(standbyPm))
        {
            osal_memset(strPortmask, 0, sizeof(strPortmask));
            LPM2STR(&standbyPm, strPortmask);
            osal_strcat(strPortmaskFinal, " Inactive: ");
            osal_strcat(strPortmaskFinal, strPortmask);
        }

        TECH_PRINTF(fp, " %s\n", strPortmaskFinal);
    }

    TECH_PRINTF(fp, "\n");
    _sal_tech_system_append(fp, "cat /proc/trunk");

    return SYS_ERR_OK;

}

#ifdef CONFIG_SYS_PORT_SECURITY
int32 _sal_tech_psecure_show(FILE *fp)
{
    char portStr[CAPA_PORT_STR_LEN];
    sys_logic_port_t lp;
    uint32 i = 0;
    uint32 allocSize = 0;
    uint32 portAddrs = 0;
    char macStr[SYS_BUF32_LEN];
    sys_l2_entry_t *pEntry = NULL;
    sys_l2_entry_t *pSortDb = NULL;

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    _sal_tech_system_append(fp, "cat /proc/psecure");

    sal_l2_db_gen();

    TECH_PRINTF(fp, "VID    MAC Address         Type                Port           \n");
    TECH_PRINTF(fp, "------ ------------------- ------------------- ----------------\n");

    allocSize = (sizeof(sys_l2_entry_t) * CAPA_PSECURE_LIMIT_NUM);

    if (NULL == (pSortDb = osal_alloc(allocSize)))
        return SYS_ERR_OK;

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        portAddrs = 0;
        osal_memset(pSortDb, 0, allocSize);

        for (i = 0; i < CAPA_PSECURE_LIMIT_NUM; i++)
        {
            pEntry = sal_psecure_portMacEntryByIdx_ret(lp, i);

            if (IS_PSECURE_ENTRY_EMPTY(pEntry))
                continue;

            osal_memcpy(&pSortDb[portAddrs], pEntry, sizeof(sys_l2_entry_t));

            portAddrs++;
        }

        sys_l2_qsort(pSortDb, portAddrs, sizeof(sys_l2_entry_t), SYS_L2_SORTDATA_ENTRY, SYS_L2_SORTMETH_MACVLAN);

        for (i = 0; i < portAddrs; i++)
        {
            pEntry = &pSortDb[i];

            sys_util_mac2Str(pEntry->mac.octet, macStr);
            LP2STR(pEntry->port, portStr);

            TECH_PRINTF(fp, "%-4d   %17s   %-17s   %s \n",
                        pEntry->vid,
                        macStr,
                        text_l2_type[pEntry->type].text,
                        portStr);
        }
    }

    osal_free(pSortDb);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION

static int32 _sal_tech_dos_show(FILE *fp)
{
    sys_enable_t enable;
    uint32 length;
    sys_logic_port_t port = 0;
    char port_str[CAPA_PORT_STR_LEN];
    sys_enable_t dosEnable;

    TECH_PRINTF(fp, "  Type                      | State (Length)                  \n");
    TECH_PRINTF(fp, "----------------------------+---------------------------------\n");
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_DAEQSA, &enable));
    TECH_PRINTF(fp, "  DMAC equal to SMAC        | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_LAND, &enable));
    TECH_PRINTF(fp, "  Land (DIP = SIP)          | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_UDPBLAT, &enable));
    TECH_PRINTF(fp, "  UDP Blat (DPORT = SPORT)  | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_TCPBLAT, &enable));
    TECH_PRINTF(fp, "  TCP Blat (DPORT = SPORT)  | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_POD, &enable));
    TECH_PRINTF(fp, "  POD (Ping of Death)       | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_IPV6_MIN_FRAG_SIZE, &enable));
    SYS_ERR_CHK(sal_dos_minIPv6FragLen_get(&length));
    TECH_PRINTF(fp, "  IPv6 Min Fragment Size    | %-8s (%d Bytes)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_ICMP_FRAG_PKTS, &enable));
    TECH_PRINTF(fp, "  ICMP Fragment Packets     | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_IPV4_PING_MAX, &enable));
    SYS_ERR_CHK(sal_dos_maxPingLen_get(&length));
    TECH_PRINTF(fp, "  IPv4 Ping Max Packet Size | %-8s (%d Bytes)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_IPV6_PING_MAX, &enable));
    TECH_PRINTF(fp, "  IPv6 Ping Max Packet Size | %-8s (%d Bytes)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SMURF, &enable));
    SYS_ERR_CHK(sal_dos_smurfNetmaskLen_get(&length));
    TECH_PRINTF(fp, "  Smurf Attack              | %-8s (Netmask Length: %d)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_TCPHDR_MIN, &enable));
    SYS_ERR_CHK(sal_dos_minTCPHdrLen_get(&length));
    TECH_PRINTF(fp, "  TCP Min Header Length     | %-8s (%d Bytes)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SYN_SPORTL1024, &enable));
    TECH_PRINTF(fp, "  TCP Syn (SPORT < 1024)    | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_NULLSCAN, &enable));
    TECH_PRINTF(fp, "  Null Scan Attack          | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_XMA, &enable));
    TECH_PRINTF(fp, "  X-Mas Scan Attack         | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SYNFIN, &enable));
    TECH_PRINTF(fp, "  TCP SYN-FIN Attack        | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SYNRST, &enable));
    TECH_PRINTF(fp, "  TCP SYN-RST Attack        | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_TCP_FRAG_OFF_MIN, &enable));
    TECH_PRINTF(fp, "  TCP Fragment (Offset = 1) | %-8s \n", text_enable[enable].text);
    TECH_PRINTF(fp, "\n");

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "  Port    | DoS Protection \n");
    TECH_PRINTF(fp, "----------+----------------\n");

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_MEM_CLEAR(port_str);
        LP2STR(port, port_str);

        SYS_ERR_CHK(sal_dos_portAttackPreventEnable_get(port, &dosEnable));

        TECH_PRINTF(fp, "  %6s  |    %-8s    \n", port_str, text_enable[dosEnable].text);
    }

    TECH_PRINTF(fp, "\n");

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_STORM_CTRL
int32 _sal_tech_stormControl_show(FILE *fp)
{
    char port_str[16];
    uint32 i;
    uint32 rate[STORM_GROUP_END];
    sys_enable_t typeEnbl[STORM_GROUP_END];
    sys_enable_t portEnbl;
    sys_logic_port_t logicPort = 0;
    sys_rate_storm_action_t stormAction;
    sys_rate_storm_rateMode_t mode;

    SYS_ERR_CHK(sal_rate_stormCtrlRefreshMode_get(&mode));

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "  Port    | State |  Broadcast  |  Unkown-Multicast  | Unknown-Unicast |   Action  \n");
    if (BASED_ON_BYTE == mode)
        TECH_PRINTF(fp, "          |       |     kbps    |        kbps        |      kbps       |           \n");
    else
        TECH_PRINTF(fp, "          |       |     pps     |        pps         |      pps        |           \n");
    TECH_PRINTF(fp, "----------+-------+-------------+--------------------+-----------------|-----------\n");

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        LP2STR(logicPort, port_str);

        for (i = 0; i < STORM_GROUP_END; i++)
        {
            SYS_ERR_CONTINUE(sal_rate_stormControlRate_get(logicPort, i, &rate[i]));

            SYS_ERR_CONTINUE(sal_rate_stormControlTypeEnable_get(logicPort, i, &typeEnbl[i]));
        }

        SYS_ERR_CHK(sal_rate_stormControlPortEnable_get(logicPort, &portEnbl));
        SYS_ERR_CHK(sal_rate_stormControlAction_get(logicPort, &stormAction));

        TECH_PRINTF(fp, "  %-6s  ", port_str);

        if (ENABLED == portEnbl)
            TECH_PRINTF(fp, " %7s", " enable");
        else
            TECH_PRINTF(fp, " %7s", "disable");

        if (DISABLED == typeEnbl[STORM_GROUP_BROADCAST])
            TECH_PRINTF(fp, " Off(%7d) ", rate[STORM_GROUP_BROADCAST]);
        else
            TECH_PRINTF(fp, "  %9d   ", rate[STORM_GROUP_BROADCAST]);

        if ((DISABLED == typeEnbl[STORM_GROUP_MULTICAST]))
            TECH_PRINTF(fp, "  Off(%7d)       ", rate[STORM_GROUP_MULTICAST]);
        else
            TECH_PRINTF(fp, "  %7d            ", rate[STORM_GROUP_MULTICAST]);

        if (DISABLED == typeEnbl[STORM_GROUP_UNICAST])
            TECH_PRINTF(fp, "  Off(%7d)    ", rate[STORM_GROUP_UNICAST]);
        else
            TECH_PRINTF(fp, "  %7d         ", rate[STORM_GROUP_UNICAST]);

        switch (stormAction)
        {
            case STORM_ACTION_NONE:
                TECH_PRINTF(fp, "  %8s ", text_storm_action[STORM_ACTION_NONE].text);
                break;

            case STORM_ACTION_SHUTDOWN:
                TECH_PRINTF(fp, "  %8s ", text_storm_action[STORM_ACTION_SHUTDOWN].text);
                break;

            default:
                break;
        }

       TECH_PRINTF(fp, "\n");
    }
    TECH_PRINTF(fp, "\n");

    return SYS_ERR_OK;
}
#endif

int32 _sal_tech_qos_show(FILE *fp)
{
    _sal_tech_system_append(fp, "cat /proc/qos");

    return SYS_ERR_OK;
}

int32 _sal_tech_rate_show(FILE *fp)
{
    uint32 igsRate = 0;
    uint32 egsRate = 0;
    sys_enable_t igsEnable = 0;
    sys_enable_t egsEnable = 0;
    sys_logic_port_t lp;
    char portStr[CAPA_PORT_STR_LEN];

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    TECH_PRINTF(fp, "Port    IGS State (Rate)     EGS State (Rate)\n");
    TECH_PRINTF(fp, "------- -------------------- ---------------------\n");
    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        LP2STR(lp, portStr);

        SYS_ERR_CHK(sal_rate_igrBandwidthCtrlEnable_get(lp, &igsEnable));
        SYS_ERR_CHK(sal_rate_igrBandwidthCtrlRate_get(lp, &igsRate));
        SYS_ERR_CHK(sal_rate_egrBandwidthCtrlEnable_get(lp, &egsEnable));
        SYS_ERR_CHK(sal_rate_egrBandwidthCtrlRate_get(lp, &egsRate));

        TECH_PRINTF(fp, "%-7s ", portStr);
        TECH_PRINTF(fp, "%-8s(%10u) ", text_enable[igsEnable].text, igsRate);
        TECH_PRINTF(fp, "%-8s(%10u)", text_enable[egsEnable].text, egsRate);

        TECH_PRINTF(fp, "\n");
    }

    return SYS_ERR_OK;
}

int32 _sal_tech_sys_memory_show(FILE *fp)
{
    sys_mem_info_t meminfo;

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    SYS_MEM_CLEAR(meminfo);

    SYS_ERR_CHK(sal_sys_memInfo_get(&meminfo));

    TECH_PRINTF(fp, "           total(KB)     used(KB)     free(KB)   shared(KB)   buffer(KB)    cache(KB)\n");
    TECH_PRINTF(fp, "--------------------+------------+------------+------------+------------+------------\n");
    TECH_PRINTF(fp, "%-7s %12lu %12lu %12lu %12lu %12lu %12lu\n",
          "Mem:",
          meminfo.mem_total, meminfo.mem_used, meminfo.mem_free,
          meminfo.mem_shared, meminfo.mem_buffer, meminfo.mem_cached);
    TECH_PRINTF(fp, "-/+ buffers/cache:   %12lu %12lu\n",
          meminfo.mem_total - meminfo.mem_free - meminfo.mem_buffer - meminfo.mem_cached,
          meminfo.mem_free + meminfo.mem_buffer + meminfo.mem_cached);
    TECH_PRINTF(fp, "%-7s %12lu %12lu %12lu\n",
          "Swap:",
          meminfo.swap_total, meminfo.swap_used,
          meminfo.swap_total - meminfo.swap_used);

    return SYS_ERR_OK;
}

int32 _sal_tech_sys_cpu_process_show(FILE *fp)
{
    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);
	TECH_PRINTF(fp, "================== CPU process usage ==================\n");

    _sal_tech_system_append(fp, "top -bn1");
	TECH_PRINTF(fp, "=======================================================\n");
	return SYS_ERR_OK;
}

static int32 _sal_tech_diag_dbgInfo_show(FILE *fp)
{
    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    _sal_tech_system_append(fp, "diag -d");

    return SYS_ERR_OK;
}

static int32 _sal_tech_sys_processInfo_show(FILE *fp)
{
    static DIR *dir;
    struct dirent *entry;
    char *name;
    int n;
    char status[32];
    char buf[1024];
    FILE *procfp;
    procps_status_t curstatus;
    int pid;
    long tasknice;
    struct stat sb;

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    if (!dir) {
        dir = opendir("/proc");
        if(!dir)
            return SYS_ERR_FILE_OPEN;
    }

    TECH_PRINTF(fp, "  PID    MEM(KB) COMMAND\n");
    for(;;) {
        if((entry = readdir(dir)) == NULL) {
            closedir(dir);
            dir = 0;
            return 0;
        }
        name = entry->d_name;
        if (!(*name >= '0' && *name <= '9'))
            continue;

        memset(&curstatus, 0, sizeof(procps_status_t));
        pid = atoi(name);
        curstatus.pid = pid;

        sprintf(status, "/proc/%d", pid);
        if(stat(status, &sb))
            continue;

        sprintf(status, "/proc/%d/stat", pid);
        if((procfp = fopen(status, "r")) == NULL)
            continue;

        name = fgets(buf, sizeof(buf), procfp);
        fclose(procfp);
        if(name == NULL)
            continue;
        name = strrchr(buf, ')'); 
        if(name == 0 || name[1] != ' ')
            continue;
        *name = 0;
        sscanf(buf, "%*s (%15c", curstatus.short_cmd);
        n = sscanf(name+2,
        "%c %d "
        "%*s %*s %*s %*s "     
        "%*s %*s %*s %*s %*s " 
        "%lu %lu "
        "%*s %*s %*s "         
        "%ld "
        "%*s %*s %*s "         
        "%*s "                 
        "%ld",
        curstatus.state, &curstatus.ppid,
        &curstatus.utime, &curstatus.stime,
        &tasknice,
        &curstatus.rss);
        if(n != 6)
            continue;

#if 0
        if (curstatus.rss == 0 && curstatus.state[0] != 'Z')
            curstatus.state[1] = 'W';
        else
            curstatus.state[1] = ' ';
        if (tasknice < 0)
            curstatus.state[2] = '<';
        else if (tasknice > 0)
            curstatus.state[2] = 'N';
        else
            curstatus.state[2] = ' ';
#endif
        curstatus.rss *= (getpagesize() >> 10);     

        TECH_PRINTF(fp, "%5d %10lu %s\n", curstatus.pid, curstatus.rss, curstatus.short_cmd);
    }

    return SYS_ERR_OK;
}

static int32 _sal_tech_sys_fs_show(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];

    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    TECH_PRINTF(fp, "================== /mnt ==================\n");
    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "ls -l /mnt");
    _sal_tech_system_append(fp, cmd);

    TECH_PRINTF(fp, "\n");

    TECH_PRINTF(fp, "================== /mntlog ==================\n");
    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "ls -l /mntlog");
    _sal_tech_system_append(fp, cmd);

    return SYS_ERR_OK;
}

static int32 _sal_tech_sys_mtd_show(FILE *fp)
{
    FILE *fp_mtd = NULL;
    char mtdInfo[128] = {0};
    char dev[8];
    char name[32];
    char size[10];
    char erasesize[10];
    char tmp[16];
    uint32 partsize = 0;
    uint32 currsize = 0;

    if (NULL == (fp_mtd = fopen("/proc/mtd", "r")))
        return SYS_ERR_FILE_OPEN;

    currsize = FLASH_BASE;
    
    fgets(mtdInfo, sizeof(mtdInfo), fp_mtd);
    osal_memset(mtdInfo, 0, sizeof(mtdInfo));
    while(fgets(mtdInfo, sizeof(mtdInfo), fp_mtd) != NULL)
    {
        osal_memset(dev, 0, sizeof(dev));
        osal_memset(name, 0, sizeof(name));
        osal_memset(size, 0, sizeof(size));
        osal_memset(erasesize, 0, sizeof(erasesize));
        osal_memset(tmp, 0, sizeof(tmp));
        osal_sscanf(mtdInfo, "%s %s %s %s", dev, size, erasesize, name);
        osal_strcpy(tmp, "0x");
        osal_strcat(tmp, size);
        partsize = SYS_STR2UINT(tmp);
        TECH_PRINTF(fp, "%s 0x%08x-0x%08x %s\n",
                        dev,
                        currsize,
                        currsize + partsize - 1,
                        name);
        currsize += partsize;
        osal_memset(mtdInfo, 0, sizeof(mtdInfo));
    }

    fclose(fp_mtd);

    return SYS_ERR_OK;
}

static int32 _sal_tech_sys_cfgAnalysis_show(FILE *fp)
{
    uint32 i = 0;
    char catname[SYS_BUF64_LEN];
    struct timeval start;
    struct timeval end;
    double time = 0;
    double toltime = 0;

    toltime = 0;
    for (i = CAT_ID_SPECIAL_END + 1; i < CAT_ID_END; i++)
    {
        time = 0;
        SYS_MEM_CLEAR(catname);
        SYS_MEM_CLEAR(start);
        SYS_MEM_CLEAR(end);

        cmd_catName_get(i, catname);

        if (0 == osal_strlen(catname))
            continue;

        gettimeofday(&start, NULL);

        SYS_ERR_CHK(cmd_parser_runningFile_print(i, CONF_RUNNING_FILE));

        gettimeofday(&end, NULL);

        time = (end.tv_sec - start.tv_sec);
        time += ((double)(end.tv_usec - start.tv_usec) / 1000000);
        toltime += time;

        TECH_PRINTF(fp, "########## [%02d] Time %.2f seconds [%s]\n", i, time, catname);
    }

    TECH_PRINTF(fp, "########## Toltal Time %.2f seconds ##########\n", toltime);

    time = 0;
    SYS_MEM_CLEAR(catname);
    SYS_MEM_CLEAR(start);
    SYS_MEM_CLEAR(end);

    cmd_catName_get(CAT_ID_END, catname);

    gettimeofday(&start, NULL);
    SYS_ERR_CHK(cmd_parser_runningFile_print(i, CONF_RUNNING_FILE));
    gettimeofday(&end, NULL);

    time = (end.tv_sec - start.tv_sec);
    time += ((double)(end.tv_usec - start.tv_usec) / 1000000);
    TECH_PRINTF(fp, "\n########## Precise All Categories Time ##########\n");
    TECH_PRINTF(fp, "########## [%02d] Time %.2f seconds [%s]\n", CAT_ID_END, time, catname);

    return SYS_ERR_OK;
}

int32 _sal_tech_debug_glbDbgCnt_show(FILE *fp)
{
    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    uint32 i;
    uint64 result;

    TECH_PRINTF(fp, "MIB Counter Value | MIB Counter Name   \n");
    TECH_PRINTF(fp, "==============================================================\n");
    for (i = 0; i < MIB_DEBUG_GLOBAL_TBL_SIZE; i++)
    {
        result = 0;

        if (i == DEBUG_MIB_GET_ALE_TX_GOOD_PKT_CNT)
        {
            SYS_ERR_CONTINUE(sal_mib_debug_globalCntr_get(i, &result));
            TECH_PRINTF(fp, "%16llu   %s\n", result, mibDebugGlbCntrName[i]);
        }

        if ((DEBUG_MIB_GET_MAC_RX_DROP_PKT_CNT <= i) && (i <= DEBUG_MIB_GET_FLOW_CONTROL_PKT_CNT))
        {
            SYS_ERR_CONTINUE(sal_mib_debug_globalCntr_get(i, &result));
            TECH_PRINTF(fp, "%16llu   %s\n", result, mibDebugGlbCntrName[i]);
        }
    }

    return SYS_ERR_OK;
}

int32 _sal_tech_debug_glbCnt_show(FILE *fp)
{
    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    uint32 i = 0;
    uint64 result;

    TECH_PRINTF(fp, "MIB Counter Value | MIB Counter Name   \n");
    TECH_PRINTF(fp, "==============================================================\n");

    while(mibGlbTbl[i].name !=NULL)
    {
        result = 0;

        if (mibGlbTbl[i].sal_mib_func != NULL)
            SYS_ERR_CONTINUE(mibGlbTbl[i].sal_mib_func(&result));

        TECH_PRINTF(fp, "%16llu   %s", result, mibGlbTbl[i].name);
        TECH_PRINTF(fp, "\n");
        i++;
    }

    return SYS_ERR_OK;
}

int32 _sal_tech_debug_portCnt_show(FILE *fp)
{
    SYS_PARAM_CHK((NULL == fp), SYS_ERR_FILE_OPEN);

    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    char port_str[CAPA_PORT_STR_LEN];
    uint32 i = 0;
    uint32 j = 0;
    uint32 ret = SYS_ERR_OK;
    uint64 result;
    char *error = NULL;

    LOGIC_PORTMASK_SET_ALL(logicPortmask);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        osal_memset(port_str, 0, sizeof(port_str));
        SYS_ERR_CHK(LP2FULLSTR(logicPort, port_str));
        i = 0;

        TECH_PRINTF(fp, "==============================================================\n");
        TECH_PRINTF(fp, " %s\n", port_str);
        while(dbg_mibtbls[i].name != NULL)
        {
            j = 0;
            while(dbg_mibtbls[i].mib_cntr[j].name !=NULL)
            {
                result = 0;

                if (dbg_mibtbls[i].mib_cntr[j].sal_mib_func != NULL)
                    ret = dbg_mibtbls[i].mib_cntr[j].sal_mib_func(logicPort, &result);

                if(ret != SYS_ERR_OK)
                {
                    error = sys_errStr_ret(ret);
                    TECH_PRINTF(fp, " %s   %s", error, dbg_mibtbls[i].mib_cntr[j].name);
                }
                else
                    TECH_PRINTF(fp, " %16llu   %s", result, dbg_mibtbls[i].mib_cntr[j].name);
                TECH_PRINTF(fp, "\n");
                j++;
            }

            i++;
        }
    }
    TECH_PRINTF(fp, "\n");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_AAA
static int32 _sal_tech_aaa_show(FILE *fp)
{
    {
        uint32 i, j;
        char            authListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
        sys_auth_type_t authTypePrio[CAPA_AAA_AUTHTYPE_PRI_NUM];
        sys_authList_type_t authListType = SYS_AUTHLIST_TYPE_NONE;
        sys_authList_type_t selAuthListType = SYS_AUTHLIST_TYPE_NONE;

        selAuthListType = SYS_AUTHLIST_TYPE_AUTHEN_LOGIN;
        TECH_PRINTF(fp, " Login List Name    Authentication Method List\n");
        TECH_PRINTF(fp, "------------------ -------------------------------\n");
        for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
        {
            osal_memset(authListName, 0, sizeof(authListName));
            osal_memset(authTypePrio, 0, sizeof(authTypePrio));
            SYS_ERR_CHK(sal_aaa_authList_getByIdx(i, authListName, authTypePrio, &authListType));
            if (0 == osal_strcmp(authListName, "")
                || (authListType != selAuthListType))
                continue;
            TECH_PRINTF(fp, " %16s  ", authListName);
            for(j = 0; j < CAPA_AAA_AUTHTYPE_PRI_NUM; j++)
            {
                if (SYS_AUTH_TYPE_NONE == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_NONE);
                else if (SYS_AUTH_TYPE_LOCAL == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_LOCAL);
                else if (SYS_AUTH_TYPE_ENABLE == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_ENABLE);
#ifdef CONFIG_SYS_LIB_RADIUS
                else if (SYS_AUTH_TYPE_RADIUS == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_RADIUS);
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
                else if (SYS_AUTH_TYPE_TACPLUS == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_TACPLUS);
#endif
            }
            TECH_PRINTF(fp, "\n");
        }

        selAuthListType = SYS_AUTHLIST_TYPE_AUTHEN_ENABLE;
        TECH_PRINTF(fp, " Enable List Name   Authentication Method List\n");
        TECH_PRINTF(fp, "------------------ -------------------------------\n");
        for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
        {
            osal_memset(authListName, 0, sizeof(authListName));
            osal_memset(authTypePrio, 0, sizeof(authTypePrio));
            SYS_ERR_CHK(sal_aaa_authList_getByIdx(i, authListName, authTypePrio, &authListType));
            if (0 == osal_strcmp(authListName, "")
                || (authListType != selAuthListType))
                continue;
            TECH_PRINTF(fp, " %16s  ", authListName);
            for(j = 0; j < CAPA_AAA_AUTHTYPE_PRI_NUM; j++)
            {
                if (SYS_AUTH_TYPE_NONE == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_NONE);
                else if (SYS_AUTH_TYPE_LOCAL == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_LOCAL);
                else if (SYS_AUTH_TYPE_ENABLE == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_ENABLE);
#ifdef CONFIG_SYS_LIB_RADIUS
                else if (SYS_AUTH_TYPE_RADIUS == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_RADIUS);
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
                else if (SYS_AUTH_TYPE_TACPLUS == authTypePrio[j])
                    TECH_PRINTF(fp, " %s ", SYS_AAA_AUTH_METHOD_STR_TACPLUS);
#endif
            }
            TECH_PRINTF(fp, "\n");
        }
    }
    TECH_PRINTF(fp, "\n");
    {
        uint32 i;
        uint32 j;
        char authListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];

        TECH_PRINTF(fp, "  Line Type  |    AAA Type     |  List Name\n");
        TECH_PRINTF(fp, "-------------+-----------------+-----------------\n");

        for (i = 0; i < SYS_ACCESS_END; i++)
        {
            switch (i)
            {
                case SYS_ACCESS_CLI_CONSOLE:
                    TECH_PRINTF(fp, " %11s |", "console");
                    break;

                case SYS_ACCESS_CLI_TELNET:
                    TECH_PRINTF(fp, " %11s |", "telnet");
                    break;

                case SYS_ACCESS_CLI_SSH:
                    TECH_PRINTF(fp, " %11s |", "ssh");
                    break;

                case SYS_ACCESS_WEB_HTTP:
                    TECH_PRINTF(fp, " %11s |", "http");
                    break;

                case SYS_ACCESS_WEB_HTTPS:
                    TECH_PRINTF(fp, " %11s |", "https");
                    break;

                default:
                    break;
            }

            for (j = SYS_AUTHLIST_TYPE_AUTHEN_LOGIN; j < SYS_AUTHLIST_TYPE_END; j++)
            {
                if ((i == SYS_ACCESS_WEB_HTTP || i == SYS_ACCESS_WEB_HTTPS) && j != SYS_AUTHLIST_TYPE_AUTHEN_LOGIN)
                    continue;

                if (j != SYS_AUTHLIST_TYPE_AUTHEN_LOGIN)
                    TECH_PRINTF(fp, "             |");
                switch (j)
                {
                    case SYS_AUTHLIST_TYPE_AUTHEN_LOGIN:
                        SYS_ERR_CHK(sal_aaa_loginAuthListName_get(i, authListName));
                        TECH_PRINTF(fp, " %15s |", "login");
                        TECH_PRINTF(fp, " %s\n", authListName);
                        break;

                    case SYS_AUTHLIST_TYPE_AUTHEN_ENABLE:
                        SYS_ERR_CHK(sal_aaa_enableAuthListName_get(i, authListName));
                        TECH_PRINTF(fp, " %15s |", "enable");
                        TECH_PRINTF(fp, " %s\n", authListName);
                        break;

                    default:
                        break;
                }
            }

        }
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
static int32 _sal_tech_authmgr_show(FILE *fp)
{
    TECH_PRINTF(fp, "============ Authentication Manager Configuration Information\n");
    {
        uint32               i = 0;
        sys_logic_port_t     lp;
        sys_enable_t         enable;
        char                 tmp[SYS_BUF64_LEN];
#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
        uint32               isTitleShow = FALSE;
#endif
        authmgr_cfg_port_t   portCfg;
        sys_vid_t               guestVid = 0;
#ifdef CONFIG_SYS_PROTO_MACAUTH
        char                    radiusUser[CAPA_MAC_STR_LEN];
        authmgr_macauth_local_t localInfo;
        sys_mac_t               emptyMac;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
        authmgr_webauth_local_t webLocalInfo;
#endif

        for (i = 0; i < AUTHMGR_TYPE_END; i++)
        {
            if (AUTHMGR_TYPE_NONE == i)
                continue;

            SYS_ERR_CHK(sal_authmgr_typeEnbl_get(i, &enable));

            SYS_MEM_CLEAR(tmp);
            osal_sprintf(tmp, "Autentication %s state", text_authmgr_type[i].text);
            TECH_PRINTF(fp, "%-30s: %s\n", tmp, text_enable[enable].text);
        }

        SYS_ERR_CHK(sal_authmgr_guestVlan_get(&guestVid));

        if (0 == guestVid)
            TECH_PRINTF(fp, "%-30s: %s\n", "Guest VLAN", text_enable[DISABLED].text);
        else
            TECH_PRINTF(fp, "%-30s: %s (%u)\n", "Guest VLAN", text_enable[ENABLED].text, guestVid);

#ifdef CONFIG_SYS_PROTO_MACAUTH
        
        SYS_MEM_CLEAR(radiusUser);
        SYS_ERR_CHK(sal_authmgr_radiusUser_get(NULL, NULL, radiusUser));

        TECH_PRINTF(fp, "%-30s: %s\n", "Mac-auth Radius User ID Format", radiusUser);

        TECH_PRINTF(fp, "\n");
        SYS_MEM_CLEAR(emptyMac);

        TECH_PRINTF(fp, "%-30s: \n", "Mac-auth Local Entry");

        for (i = 0; i < CAPA_AUTHMGR_MACAUTH_LOCAL_NUM; i++)
        {
            SYS_MEM_CLEAR(localInfo);
            SYS_ERR_CONTINUE(sal_authmgr_macauthLocalInfo_getByIdx(i, &localInfo));

            if (0 == osal_memcmp(&localInfo.mac, &emptyMac, sizeof(sys_mac_t)))
                continue;

            if (FALSE == isTitleShow)
            {
                TECH_PRINTF(fp, "                                          Reauth     Inactive\n");
                TECH_PRINTF(fp, "MAC Address         Control        VLAN   Period     Timeout\n");
                TECH_PRINTF(fp, "------------------- -------------- ------ ---------- ---------\n");

                isTitleShow = TRUE;
            }

            TECH_PRINTF(fp, "%02X:%02X:%02X:%02X:%02X:%02X  ",
                  localInfo.mac.octet[0],
                  localInfo.mac.octet[1],
                  localInfo.mac.octet[2],
                  localInfo.mac.octet[3],
                  localInfo.mac.octet[4],
                  localInfo.mac.octet[5]);

            TECH_PRINTF(fp, " %-12s  ", (AUTHMGR_PORTCTRL_FORCE_AUTHORIZED == localInfo.state) ? "Authorized" : "UnAuthorized");

            if (localInfo.authorInfo.authVid != 0)
                TECH_PRINTF(fp, " %-6u", localInfo.authorInfo.authVid);
            else
                TECH_PRINTF(fp, " %-6s", "N/A");

            if (localInfo.authorInfo.reauthTime != 0)
                TECH_PRINTF(fp, " %-10u", localInfo.authorInfo.reauthTime);
            else
                TECH_PRINTF(fp, " %-10s", "N/A");

            if (localInfo.authorInfo.inactTime != 0)
                TECH_PRINTF(fp, " %-5u", localInfo.authorInfo.inactTime);
            else
                TECH_PRINTF(fp, " %-5s", "N/A");

            TECH_PRINTF(fp, "\n");
        }
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        
        TECH_PRINTF(fp, "\n");

        TECH_PRINTF(fp, "%-30s: \n", "Web-auth Local Entry");

        isTitleShow = FALSE;
        for (i = 0; i < CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM; i++)
        {
            SYS_MEM_CLEAR(webLocalInfo);
            SYS_ERR_CONTINUE(sal_authmgr_webauthLocalInfo_getByIdx(i, &webLocalInfo));

            if (0 == osal_strlen(webLocalInfo.username))
                continue;

            if (FALSE == isTitleShow)
            {
                TECH_PRINTF(fp, "                                        Reauth     Inactive\n");
                TECH_PRINTF(fp, "User Name                        VLAN   Period     Timeout\n");
                TECH_PRINTF(fp, "-------------------------------- ------ ---------- ---------\n");

                isTitleShow = TRUE;
            }

            TECH_PRINTF(fp, "%-32s", webLocalInfo.username);

            if (webLocalInfo.authorInfo.authVid != 0)
                TECH_PRINTF(fp, " %-6u", webLocalInfo.authorInfo.authVid);
            else
                TECH_PRINTF(fp, " %-6s", "N/A");

            if (webLocalInfo.authorInfo.reauthTime != 0)
                TECH_PRINTF(fp, " %-10u", webLocalInfo.authorInfo.reauthTime);
            else
                TECH_PRINTF(fp, " %-10s", "N/A");

            if (webLocalInfo.authorInfo.inactTime != 0)
                TECH_PRINTF(fp, " %-5u", webLocalInfo.authorInfo.inactTime);
            else
                TECH_PRINTF(fp, " %-5s", "N/A");

            TECH_PRINTF(fp, "\n");
        }
#endif

        TECH_PRINTF(fp, "\n");

        TECH_PRINTF(fp, "%-30s\n", "Interface Configurations");

        FOR_EACH_NORMAL_LOGIC_PORT(lp)
        {
            SYS_MEM_CLEAR(portCfg);

            SYS_ERR_CONTINUE(sal_authmgr_portCfg_get(lp, &portCfg));

            TECH_PRINTF(fp, "\n");
            TECH_PRINTF(fp, "Interface %s\n", LPSTR(lp));

            TECH_PRINTF(fp, "  %-23s: %s\n", "Admin Control", text_authmgr_ctrlType[portCfg.ctrlType].text);

            TECH_PRINTF(fp, "  %-23s: %s\n", "Host Mode", text_authmgr_mode[portCfg.mode].text);

            for (i = 0; i < AUTHMGR_TYPE_END; i++)
            {
                if (AUTHMGR_TYPE_NONE == i)
                    continue;

                SYS_MEM_CLEAR(tmp);
                osal_sprintf(tmp, "Type %s State", text_authmgr_type[i].text);
                TECH_PRINTF(fp, "  %-23s: %s\n", tmp, text_enable[portCfg.typeEnbl[i]].text);
            }

            SYS_MEM_CLEAR(tmp);
            for (i = 0; i < (AUTHMGR_TYPE_END - 1); i++)
            {
                if (AUTHMGR_TYPE_NONE == portCfg.typeOrder.order[i])
                    break;

                osal_strcat(tmp, text_authmgr_type[portCfg.typeOrder.order[i]].text);
                osal_strcat(tmp, " ");
            }
            TECH_PRINTF(fp, "  %-23s: %s\n", "Type Order", tmp);

#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
            
            SYS_MEM_CLEAR(tmp);
            for (i = 0; i < (AUTHMGR_METHOD_END - 1); i++)
            {
                if (AUTHMGR_METHOD_NONE == portCfg.methodOrder.order[i])
                    break;

                osal_strcat(tmp, text_authmgr_method[portCfg.methodOrder.order[i]].text);
                osal_strcat(tmp, " ");
            }
            TECH_PRINTF(fp, "  %-23s: %s\n", "MAC/WEB Method Order", tmp);
#endif

            TECH_PRINTF(fp, "  %-23s: %s\n", "Guest VLAN", text_enable[portCfg.guestVlanEnbl].text);

            TECH_PRINTF(fp, "  %-23s: %s\n", "Reauthentication", text_enable[portCfg.reauthEnbl].text);

            TECH_PRINTF(fp, "  %-23s: %u\n", "Max Hosts", portCfg.maxHosts);

            TECH_PRINTF(fp, "  %-23s: %s\n", "VLAN Assign Mode", text_authmgr_vlanAssign[portCfg.vlanAssignMode].text);

            TECH_PRINTF(fp, "  %-23s\n", "Common Timers");

            TECH_PRINTF(fp, "    %-21s: %u\n", "Reauthenticate Period", portCfg.reauthPeriod);

            TECH_PRINTF(fp, "    %-21s: %u\n", "Inactive Timeout", portCfg.inactTimeout);

            TECH_PRINTF(fp, "    %-21s: %u\n", "Quiet Period", portCfg.quietPeriod);

#ifdef CONFIG_SYS_PROTO_DOT1X
            
            TECH_PRINTF(fp, "  %-23s\n", "802.1x Parameters");

            TECH_PRINTF(fp, "    %-21s: %u\n", "EAP Max Request", portCfg.maxReq);

            TECH_PRINTF(fp, "    %-21s: %u\n", "EAP TX Period", portCfg.txPeriod);

            TECH_PRINTF(fp, "    %-21s: %u\n", "Supplicant Timeout", portCfg.suppTimeout);

            TECH_PRINTF(fp, "    %-21s: %u\n", "Server Timeout", portCfg.serverTimeout);
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
            
            TECH_PRINTF(fp, "  %-23s\n", "Web-auth Parameters");

            if (0 == portCfg.loginAttempt)
                TECH_PRINTF(fp, "    %-21s: %s\n", "Login Attempt", "infinite");
            else
                TECH_PRINTF(fp, "    %-21s: %u\n", "Login Attempt", portCfg.loginAttempt);
#endif
        }
        TECH_PRINTF(fp, "\n");
    }
    TECH_PRINTF(fp, "============ Authentication Sessions\n");
    {
        uint32 i = 0;
        uint32 sessCnt = 0;
        char macStr[CAPA_MAC_STR_LEN];
        authmgr_sess_info_t sessInfo;

        SYS_ERR_CHK(sal_authmgr_sessCnt_get(&sessCnt));

        if (0 == sessCnt)
        {
            TECH_PRINTF(fp, "No Auth Manager sessions currently exist\n");

            return SYS_ERR_OK;
        }

        for (i = 0; i < sessCnt; i++)
        {
            SYS_MEM_CLEAR(sessInfo);

            SYS_ERR_CONTINUE(sal_authmgr_sessInfo_getByIdx(i, &sessInfo));

            TECH_PRINTF(fp, "%-23s: %s\n", "Interface", LPSTR(sessInfo.suppInfo.port));

            SYS_MEM_CLEAR(macStr);
            sys_util_mac2Str(sessInfo.suppInfo.mac.octet, macStr);
            TECH_PRINTF(fp, "%-23s: %s\n", "MAC Address", macStr);

            TECH_PRINTF(fp, "%-23s: %08X%08X\n", "Session ID", sessInfo.sessId.cnt, sessInfo.sessId.time);

            TECH_PRINTF(fp, "%-23s: %s\n", "Current Type", text_authmgr_type[sessInfo.currType].text);

            TECH_PRINTF(fp, "%-23s: %s\n", "Status", text_authmgr_status[sessInfo.status.authStatus].text);

            TECH_PRINTF(fp, "%s\n", "Authorized Information");

            if (0 != sessInfo.status.authorInfo.authVid)
                TECH_PRINTF(fp, "  %-21s: %u (from %s)\n", "VLAN", sessInfo.status.authorInfo.authVid, text_authmgr_authorSrc[sessInfo.status.authorSrc.vidSrc].text);
            else
                TECH_PRINTF(fp, "  %-21s: N/A\n", "VLAN");

            if (0 != sessInfo.status.authorInfo.reauthTime)
                TECH_PRINTF(fp, "  %-21s: %u (from %s)\n", "Reauthenticate Period", sessInfo.status.authorInfo.reauthTime, text_authmgr_authorSrc[sessInfo.status.authorSrc.reauthTimeSrc].text);
            else
                TECH_PRINTF(fp, "  %-21s: N/A\n", "Reauthenticate Period");

            if (0 != sessInfo.status.authorInfo.inactTime)
                TECH_PRINTF(fp, "  %-21s: %u (from %s)\n", "Inactive Timeout", sessInfo.status.authorInfo.inactTime, text_authmgr_authorSrc[sessInfo.status.authorSrc.inactTimeSrc].text);
            else
                TECH_PRINTF(fp, "  %-21s: N/A\n", "Inactive Timeout");

            TECH_PRINTF(fp, "%s\n", "Operational Information");

            TECH_PRINTF(fp, "  %-21s: %u\n", "VLAN", sessInfo.status.currVid);

            if (AUTHMGR_STATUS_AUTHED == sessInfo.status.authStatus)
            {
                
                TECH_PRINTF(fp, "  %-21s: %u\n", "Session Time", sessInfo.status.authedWhile);

                TECH_PRINTF(fp, "  %-21s: %u\n", "Inactive Time", sessInfo.status.inactWhile);

                TECH_PRINTF(fp, "  %-21s: %s\n", "Quiet Time", "N/A");
            }
            else
            {
                
                TECH_PRINTF(fp, "  %-21s: %s\n", "Session Time", "N/A");

                TECH_PRINTF(fp, "  %-21s: %s\n", "Inactive Time", "N/A");

                TECH_PRINTF(fp, "  %-21s: %u\n", "Quiet Time", sessInfo.status.quietWhile);
            }

            TECH_PRINTF(fp, "\n");
        }

    }
    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING

static int32 _sal_tech_igmp_show(FILE *fp)
{
    int32 i;

    sys_enable_t enable;
    sys_mcast_info_t info;
    igmp_lookup_mode_t mode;
    igmp_unknMcastAction_t unknMcastAction;

    uint32 cnt = 0;
    uint32 dbSize = 0;

    char sip[CAPA_IPV4_STR_LEN];
    char dip[CAPA_IPV4_STR_LEN];
    char portInfo[CAPA_PORTMASK_STR_LEN];
    char export_str[CAPA_PORTMASK_STR_LEN];
    char lifeStr[8];

    sys_igmp_db_t db;

#ifdef CONFIG_SYS_PROTO_MVR
    sys_enable_t mvrEn;
    sys_vid_t mvrVlanId = 0;
#endif

    osal_memset(&info, 0, sizeof(sys_mcast_info_t));

    SYS_ERR_CHK(sal_igmp_info_get(&info));
    SYS_ERR_CHK(sal_igmp_enable_get(&enable));
    SYS_ERR_CHK(sal_igmp_lookupMode_get(&mode));
    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&unknMcastAction));

    TECH_PRINTF(fp, "\n\n\t\tIGMP Snooping Status\n");
    TECH_PRINTF(fp, "\t\t--------------------\n\n");

    TECH_PRINTF(fp, "    Snooping                        : %s\n", text_enable[enable].text);
    TECH_PRINTF(fp, "    Report Suppression              : %s\n", text_enable[enable].text);
    TECH_PRINTF(fp, "    Operation Version               : %s\n",
                (info.operVersion >= IGMP_VERSION_V3_BASIC) ? "v3" : "v2" );
    TECH_PRINTF(fp, "    Forward Method                  : %s\n", gLookupMode[mode]);
    TECH_PRINTF(fp, "    Unknown IP Multicast Action     : %s\n", gUnknownMcastAct[unknMcastAction]);

    TECH_PRINTF(fp, "\n\n\t\tPacket Statistics\n");

    for (i = 0; i < MCAST_STAT_END; i++)
        TECH_PRINTF(fp, "    %-32s :  %d \n",  mcastStatCntName[i], info.totStat[i]);

    TECH_PRINTF(fp, "\n");

    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));

    #ifdef CONFIG_SYS_PROTO_MVR
    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&mvrEn));
    if (ENABLED == mvrEn)
    {
        SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlanId));
    }
    #endif

    if (IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
    {
        TECH_PRINTF(fp, " VLAN | Group IP Address |  Type  | Life(Sec) | Port      \n");
        TECH_PRINTF(fp, "------+------------------+--------+-----------+------------------\n");
    }
    else
    {
        TECH_PRINTF(fp, " VLAN | Group IP Address |  SIP Address  |  Type  | Life(Sec) | Port \n");
        TECH_PRINTF(fp, "------+------------------+---------------+--------+-----------+------------------\n");
    }

    for (i = 0; i < dbSize; i++)
    {
        osal_memset(&db, 0, sizeof(sys_igmp_db_t));

        db.indx = i;
        SYS_ERR_CHK(sal_mcast_database_get(&db));

        if (MULTICAST_TYPE_IPV4 != db.ipType)
            continue;

        #ifdef CONFIG_SYS_PROTO_MVR
        if (mvrVlanId == db.vid)
        {
            if (sal_igmp_mvr_group_ret(&db))
                continue;
        }
        #endif

        if (!IGMP_IN_MULTICAST(db.dip))
            continue;

        cnt++;

        SYS_ERR_CHK(sys_util_ip2Str(db.sip, sip));
        SYS_ERR_CHK(sys_util_ip2Str(db.dip, dip));

        osal_memset(lifeStr, 0, sizeof(lifeStr));
        if(0 == db.life)
            osal_sprintf(lifeStr, "--");
        else
            osal_sprintf(lifeStr, "%d", db.life);

        if (IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
        {
            osal_memset(portInfo, 0, sizeof(portInfo));
            osal_memset(export_str, 0, sizeof(export_str));

            if (IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            {
                LPM2STR(&db.portmask, portInfo);
                if ((0 == osal_strlen(portInfo)) && (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == unknMcastAction))
                    osal_sprintf(portInfo,"%s", "router");
            }
            else
            {
                LPM2STR(&db.staticInMbr, portInfo);
            }

            TECH_PRINTF(fp, " %4d | %16s |%8s| %4s      | %s\n", db.vid, dip,
                        (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                        lifeStr, portInfo);
        }
        else
        {
            if (IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            {
                LPM2STR(&db.portmask, portInfo);
                if (0 == osal_strlen(portInfo))
                    osal_sprintf(portInfo,"%s", gUnknownMcastAct[unknMcastAction]);
            }
            else
                LPM2STR(&db.staticInMbr, portInfo);

            TECH_PRINTF(fp, " %4d | %16s |%15s|%8s| %4s      | %s\n", db.vid, dip, sip,
                        (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                        lifeStr, portInfo);
        }
    }

    TECH_PRINTF(fp, "\n\nTotal Number of Entry = %d\n\n", cnt);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING

static int32 _sal_tech_mld_show(FILE *fp)
{
    int32 i;

    sys_enable_t enable;
    sys_mcast_info_t info;
    igmp_lookup_mode_t mode;
    igmp_unknMcastAction_t unknMcastAction;

    uint32 cnt = 0;
    uint32 dbSize = 0;

    char strIpv6AddrSip[CAPA_IPV6_STR_LEN];
    char strIpv6AddrDip[CAPA_IPV6_STR_LEN];
    char portInfo[CAPA_PORTMASK_STR_LEN];
    char export_str[CAPA_PORTMASK_STR_LEN];
    char lifeStr[8];

    sys_igmp_db_t db;

    osal_memset(&info, 0, sizeof(sys_mcast_info_t));

    SYS_ERR_CHK(sal_mld_enable_get(&enable));
    SYS_ERR_CHK(sal_mld_info_get(&info));
    SYS_ERR_CHK(sal_mld_lookupMode_get(&mode));
    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&unknMcastAction));

    TECH_PRINTF(fp, "\n\n\t\tMLD Snooping Status\n");
    TECH_PRINTF(fp, "\t\t--------------------\n\n");

    TECH_PRINTF(fp, "    Snooping                        : %s\n", text_enable[enable].text);
    TECH_PRINTF(fp, "    Report Suppression              : %s\n", text_enable[enable].text);
    TECH_PRINTF(fp, "    Operation Version               : %s\n",
          (info.oper_version >= MLD_VERSION_V2_BASIC) ? "v2" : "v1");
    TECH_PRINTF(fp, "    Forward Method                  : %s\n", gLookupMode[mode]);
    TECH_PRINTF(fp, "    Unknown IPv6 Multicast Action   : %s\n", gUnknownMcastAct[unknMcastAction]);

    TECH_PRINTF(fp, "\n\n\t\tPacket Statistics\n");

    for (i = 0; i < MCAST_STAT_END; i++)
        TECH_PRINTF(fp, "    %-32s :  %d \n",  mcastStatCntName[i], info.totStat[i]);

    TECH_PRINTF(fp, "\n");

    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));

    if (IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
    {
        TECH_PRINTF(fp, " VLAN |            Group IP Address            |  Type  | Life(Sec) | Port      \n");
        TECH_PRINTF(fp, "------+----------------------------------------+--------+-----------+------------------\n");
    }
    else
    {
        TECH_PRINTF(fp, " VLAN |            Group IP Address            |              SIP Address               |  Type  | Life(Sec) | Port \n");
        TECH_PRINTF(fp, "------+----------------------------------------+----------------------------------------+--------+-----------+------------------\n");
    }

    for (i = 0; i < dbSize; i++)
    {
        osal_memset(&db, 0, sizeof(sys_igmp_db_t));

        db.indx = i;
        SYS_ERR_CHK(sal_mcast_database_get(&db));

        if (MULTICAST_TYPE_IPV6 != db.ipType)
            continue;

        if (!MLD_IN_MULTICAST(db.dipv6))
            continue;

        cnt++;

        inet_ntop(AF_INET6, db.dipv6, strIpv6AddrDip, sizeof(strIpv6AddrDip));

        osal_memset(lifeStr, 0, sizeof(lifeStr));
        if(0 == db.life)
            osal_sprintf(lifeStr, "--");
        else
            osal_sprintf(lifeStr, "%d", db.life);

        if (IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
        {
            osal_memset(portInfo, 0, sizeof(portInfo));
            osal_memset(export_str, 0, sizeof(export_str));

            if (IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            {
                LPM2STR(&db.portmask, portInfo);
                if ((0 == osal_strlen(portInfo)) && (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == unknMcastAction))
                    osal_sprintf(portInfo,"%s", "router");
            }
            else
            {
                LPM2STR(&db.staticInMbr, portInfo);
            }

            TECH_PRINTF(fp, " %4d | %38s |%8s| %4s      | %s\n", db.vid, strIpv6AddrDip,
                        (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                        lifeStr, portInfo);
        }
        else
        {
            inet_ntop(AF_INET6, db.sipv6, strIpv6AddrSip, sizeof(strIpv6AddrSip));
            if( IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            {
                LPM2STR(&db.portmask, portInfo);
                if (0 == osal_strlen(portInfo))
                    osal_sprintf(portInfo,"%s", gUnknownMcastAct[unknMcastAction]);
            }
            else
            {
                LPM2STR(&db.staticInMbr, portInfo);
            }

            TECH_PRINTF(fp, " %4d | %38s | %38s |%8s| %4s      | %s\n", db.vid, strIpv6AddrDip, strIpv6AddrSip,
                        (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                        lifeStr, portInfo);
        }
    }

    TECH_PRINTF(fp, "\n\nTotal Number of Entry = %d\n\n", cnt);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_LLDP
static int32 _sal_tech_lldp_show(FILE *fp)
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

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, " State: %s\n", lldp_enabled?"Enabled":"Disabled");
    TECH_PRINTF(fp, " Timer: %d Seconds\n", tx_interval);
    TECH_PRINTF(fp, " Hold multiplier: %d\n", holdtime);
    TECH_PRINTF(fp, " Reinit delay: %d Seconds\n", reinit_delay);
    TECH_PRINTF(fp, " Tx delay: %d Seconds\n", tx_delay);
    switch(action)
    {
        case LLDP_DISABLE_ACTION_FILTER:
            TECH_PRINTF(fp, " LLDP packet handling: Filtering\n");
            break;
        case LLDP_DISABLE_ACTION_FLOOD_TO_VLAN:
            TECH_PRINTF(fp, " LLDP packet handling: Bridging\n");
            break;
        case LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS:
            TECH_PRINTF(fp, " LLDP packet handling: Flooding\n");
            break;
        default:
            break;
    }

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, " Port     |  State | Optional TLVs  | Address\n");
    TECH_PRINTF(fp, " -------- + ------ + -------------- + --------\n");

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
        TECH_PRINTF(fp, "%s", strLine);
        TECH_PRINTF(fp, "\n");
    }

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        SYS_ERR_CONTINUE(LP2STR(logicPort,lldpUserPort));
        TECH_PRINTF(fp, "\nPort ID: %s\n", lldpUserPort);
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
            TECH_PRINTF(fp, "802.3 optional TLVs: %s\n", &strTmp[2]);
        else
            TECH_PRINTF(fp, "802.3 optional TLVs:\n");
        TECH_PRINTF(fp, "802.1 optional TLVs\n");
        TECH_PRINTF(fp, "PVID: %s\n", (ENABLED == pvidEnable)?"Enabled":"Disabled");
        osal_memset(strVlanList, 0, sizeof(strVlanList));
        VMSK2STR(&vlanmask, strVlanList);
        if (osal_strlen(strVlanList) > 0)
            TECH_PRINTF(fp, "VLANs: %s\n", strVlanList);
    }
    TECH_PRINTF(fp, "\n");

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_MVR

static int32 _sal_tech_mvr_show(FILE *fp)
{
    uint32 maxGroupNum = 0, curGroupNum = 0;
    uint32 startIp = 0, endIp = 0;
    uint32 responseTime;
    mvr_mode_t mvrMode;
    char startIpStr[CAPA_IPV4_STR_LEN];
    char endIpStr[CAPA_IPV4_STR_LEN];

    sys_enable_t enable;
    sys_vid_t mvrVlanId;

    uint32 cnt = 0;
    uint32 i = 0;
    uint32 dbSize = 0;

    char sip[CAPA_IPV4_STR_LEN];
    char dip[CAPA_IPV4_STR_LEN];
    char portInfo[CAPA_PORTMASK_STR_LEN];
    char export_str[CAPA_PORTMASK_STR_LEN];
    char lifeStr[8];

    sys_igmp_db_t db;
    igmp_lookup_mode_t mode;
    igmp_unknMcastAction_t action;

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));
    SYS_ERR_CHK(sal_igmp_mvr_mode_get(&mvrMode));
    SYS_ERR_CHK(sal_igmp_mvr_querytime_get(&responseTime));
    SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlanId));
    SYS_ERR_CHK(sal_igmp_mvr_groupRange_get(&startIp, &endIp));
    SYS_ERR_CHK(sal_igmp_mvrGroupNum_get(&curGroupNum));

    maxGroupNum = CAPA_IGMP_MVR_GROUP_NUM;

    TECH_PRINTF(fp, "MVR Running : %s\n", text_enable[enable].text);
    TECH_PRINTF(fp, "MVR Multicast VLAN : %d\n", mvrVlanId);

    if (0 == startIp)
    {
        TECH_PRINTF(fp, "MVR Group Range :  None\n");
    }
    else
    {
        osal_memset(startIpStr, 0, sizeof(startIpStr));
        osal_memset(endIpStr, 0, sizeof(endIpStr));

        SYS_ERR_CHK(sys_util_ip2Str(startIp, startIpStr));
        SYS_ERR_CHK(sys_util_ip2Str(endIp, endIpStr));

        TECH_PRINTF(fp, "MVR Group Range :  %s ~ %s\n", startIpStr, endIpStr);
    }

    TECH_PRINTF(fp, "MVR Max Multicast Groups : %d\n", maxGroupNum);
    TECH_PRINTF(fp, "MVR Current Multicast Groups : %d\n", curGroupNum);
    TECH_PRINTF(fp, "MVR Global query response time : %d sec\n", responseTime);
    TECH_PRINTF(fp, "MVR Mode : %s\n\n", mvrMode == MVR_MODE_COMPATABLE ? "compatible" : "dynamic");

    if (DISABLED == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_igmp_lookupMode_get(&mode));
    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&action));
    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));

    if (IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
    {
        TECH_PRINTF(fp, " Gourp IP Address |  Type  | Life(Sec) | Port      \n");
        TECH_PRINTF(fp, "------------------+--------+-----------+------------------\n");
    }
    else
    {
        TECH_PRINTF(fp, " Group IP Address |  SIP Address  |  Type  | Life(Sec) | Port \n");
        TECH_PRINTF(fp, "------------------+---------------+--------+-----------+------------------\n");
    }

    for (i = 0; i < dbSize; i++)
    {
        osal_memset(&db, 0, sizeof(sys_igmp_db_t));

        db.indx = i;
        SYS_ERR_CHK(sal_mcast_database_get(&db));

        if (MULTICAST_TYPE_IPV4 != db.ipType)
            continue;

        if (mvrVlanId != db.vid)
            continue;

        if (db.dip < startIp || db.dip > endIp)
            continue;

        cnt++;

        SYS_ERR_CHK(sys_util_ip2Str(db.sip, sip));
        SYS_ERR_CHK(sys_util_ip2Str(db.dip, dip));

        osal_memset(lifeStr, 0, sizeof(lifeStr));
        if(0 == db.life)
            osal_sprintf(lifeStr, "--");
        else
            osal_sprintf(lifeStr, "%d", db.life);

        if (IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
        {
            osal_memset(portInfo, 0, sizeof(portInfo));
            osal_memset(export_str, 0, sizeof(export_str));

            if (IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            {
                LPM2STR(&db.portmask, portInfo);
                if ((0 == osal_strlen(portInfo)) && (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == action))
                    osal_sprintf(portInfo,"%s", "router");
            }
            else
            {
                LPM2STR(&db.staticInMbr, portInfo);
            }

            TECH_PRINTF(fp, " %16s |%8s| %4s      | %s\n",  dip,
                        (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                        lifeStr, portInfo);
        }
        else
        {
            if(IGMP_GROUP_ENTRY_DYNAMIC == db.form)
                LPM2STR(&db.portmask, portInfo);
            else
                LPM2STR(&db.staticInMbr, portInfo);

            TECH_PRINTF(fp, " %16s |%15s|%8s| %4s      | %s\n", dip, sip,
                        (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                        lifeStr, portInfo);
        }
    }

    TECH_PRINTF(fp, "\n\nTotal Number of Entry = %d\n\n", cnt);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_CABLE_DIAG

static int32 _sal_tech_cablediag_show(FILE *fp)
{
    uint8                   i;
    sys_logic_port_t        lp;
    sys_port_media_t        media;
    sys_rtctResult_t        result;
    char                    port_str[SYS_BUF16_LEN];
    sys_port_speed_t        speed;
    const uint8             CHANNEL_NUM = 4;
    uint32                  channelLenM, channelLenCM, channelOpen, channelShort, channelMismatch, lineDriver;

    osal_memset(&result, 0, sizeof(sys_rtctResult_t));
    osal_memset(&media, 0, sizeof(sys_port_media_t));

    TECH_PRINTF(fp, " Port   |  Speed | Local pair | Pair length | Pair status   \n");
    TECH_PRINTF(fp, "--------+--------+------------+-------------+---------------\n");

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        osal_memset(port_str, 0, sizeof(port_str));

        SYS_ERR_CHK(LP2STR(lp, port_str));

        SYS_ERR_CHK(sal_port_linkMedia_get(lp, &media));

        if (PORT_MEDIA_FIBER == media)
        {
            TECH_PRINTF(fp, " %4s   |  Fiber |    ---\n\n", port_str);
            continue;
        }

        SYS_MEM_CLEAR(speed);
        SYS_ERR_CHK(sal_port_speed_get(lp, &speed));
        SYS_ERR_CHK(sal_cablediag_copper_set(lp));
        SYS_ERR_CHK(sal_cablediag_copper_get(lp, &result));

        for (i = 0; i < CHANNEL_NUM; i++)
        {
            if (i == 0)
            {
                TECH_PRINTF(fp, " %4s   |", port_str);

                if (ENABLED == speed.autoSpeed)
                    TECH_PRINTF(fp, "  auto  |");
                else if (SYS_PORT_SPEED_10M == speed.ability)
                    TECH_PRINTF(fp, "  10Mb  |");
                else if (SYS_PORT_SPEED_100M == speed.ability)
                    TECH_PRINTF(fp, " 100Mb  |");
                else if (SYS_PORT_SPEED_1000M == speed.ability)
                    TECH_PRINTF(fp, " 1000Mb |");
            }
            else
            {
                TECH_PRINTF(fp, "                  ");
            }

            TECH_PRINTF(fp, "    Pair %c  |", 'A' + i);

            if (PORT_SPEED_10M == result.linkType || PORT_SPEED_100M == result.linkType)
            {
                if (i == 0)
                {
                    TECH_PRINTF(fp, "  %4d.%02d    |", result.un.fe_result.rxLen / 100, result.un.fe_result.rxLen % 100);

                    if (result.un.fe_result.isRxShort)
                        TECH_PRINTF(fp, " Short");
                    else if (result.un.fe_result.isRxOpen)
                        TECH_PRINTF(fp, " Open");
                    else if (result.un.fe_result.isRxMismatch)
                        TECH_PRINTF(fp, " ImpedanceMis");
                    else if (result.un.fe_result.isRxLinedriver)
                        TECH_PRINTF(fp, " LineDriver");
                    else
                        TECH_PRINTF(fp, " Normal");
                }
                else if (i == 1)
                {
                    TECH_PRINTF(fp, "  %4d.%02d    |", result.un.fe_result.txLen / 100, result.un.fe_result.txLen % 100);

                    if (result.un.fe_result.isTxShort)
                        TECH_PRINTF(fp, " Short");
                    else if (result.un.fe_result.isTxOpen)
                        TECH_PRINTF(fp, " Open");
                    else if (result.un.fe_result.isTxMismatch)
                        TECH_PRINTF(fp, " ImpedanceMis");
                    else if (result.un.fe_result.isTxLinedriver)
                        TECH_PRINTF(fp, " LineDriver");
                    else
                        TECH_PRINTF(fp, " Normal");
                }
                else
                    TECH_PRINTF(fp, "     N/A     | Not Supported ");
            }
            else if (PORT_SPEED_1000M == result.linkType)
            {
                if (i == 0)
                {
                    channelLenM = result.un.ge_result.channelALen / 100;
                    channelLenCM = result.un.ge_result.channelALen % 100;
                    channelOpen = result.un.ge_result.channelAOpen;
                    channelShort = result.un.ge_result.channelAShort;
                    channelMismatch = result.un.ge_result.channelAMismatch;
                    lineDriver = result.un.ge_result.channelALinedriver;
                }
                else if (i == 1)
                {
                    channelLenM = result.un.ge_result.channelBLen / 100;
                    channelLenCM = result.un.ge_result.channelBLen % 100;
                    channelOpen = result.un.ge_result.channelBOpen;
                    channelShort = result.un.ge_result.channelBShort;
                    channelMismatch = result.un.ge_result.channelBMismatch;
                    lineDriver = result.un.ge_result.channelBLinedriver;
                }
                else if (i == 2)
                {
                    channelLenM = result.un.ge_result.channelCLen / 100;
                    channelLenCM = result.un.ge_result.channelCLen % 100;
                    channelOpen = result.un.ge_result.channelCOpen;
                    channelShort = result.un.ge_result.channelCShort;
                    channelMismatch = result.un.ge_result.channelCMismatch;
                    lineDriver = result.un.ge_result.channelCLinedriver;
                }
                else
                {
                    channelLenM = result.un.ge_result.channelDLen / 100;
                    channelLenCM = result.un.ge_result.channelDLen % 100;
                    channelOpen = result.un.ge_result.channelDOpen;
                    channelShort = result.un.ge_result.channelDShort;
                    channelMismatch = result.un.ge_result.channelDMismatch;
                    lineDriver = result.un.ge_result.channelDLinedriver;
                }

                TECH_PRINTF(fp, "  %4d.%02d    |", channelLenM, channelLenCM);
                if (channelShort)
                    TECH_PRINTF(fp, " Short");
                else if (channelOpen)
                    TECH_PRINTF(fp, " Open");
                else if (channelMismatch)
                    TECH_PRINTF(fp, " ImpedanceMis");
                else if (lineDriver)
                    TECH_PRINTF(fp, " LineDriver");
                else
                    TECH_PRINTF(fp, " Normal");
            }
            TECH_PRINTF(fp, "\n");
        }
        TECH_PRINTF(fp, "\n");
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_MIRROR

static int32 _sal_tech_mirror_show(FILE *fp)
{
    sys_gid_t gid;
    sys_logic_port_t destPort;
    sys_mirror_state_t state;
    sys_logic_portmask_t rxPortmask;
    sys_logic_portmask_t txPortmask;
    sys_enable_t enable;
    char port_str[8];
    char ingportmask_str[64];
    char egportmask_str[64];

    for(gid = 0; gid < CAPA_MIRROR_SESSION_NUM; gid ++)
    {
        TECH_PRINTF(fp, "\nSession %d Configuration\n", GID2UGID(gid));

        SYS_ERR_CHK(sal_mirror_group_state_get(gid,&state));

        if(PORTBASED_ENABLED == state)
        {
            SYS_ERR_CHK(sal_mirror_group_sourcePort_get(gid,&rxPortmask, &txPortmask));
            SYS_ERR_CHK(LPM2STR(&rxPortmask, ingportmask_str));
            SYS_ERR_CHK(LPM2STR(&txPortmask, egportmask_str));

            TECH_PRINTF(fp, "Source RX Port    : %s\n", ingportmask_str);
            TECH_PRINTF(fp, "Source TX Port    : %s\n", egportmask_str);

        }
        else
        {
            TECH_PRINTF(fp, "Mirrored source   : Not Config\n");
        }

        SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid, &destPort, &enable));
        if (ENABLED == enable)
        {
            SYS_ERR_CHK(LP2STR(destPort, port_str));
            TECH_PRINTF(fp, "Destination port  : %s\n", port_str);

            SYS_ERR_CHK(sal_mirror_group_igrState_get(gid,&enable));

            TECH_PRINTF(fp, "Ingress State: %s\n",text_enable[enable].text);
        }
        else
        {
            TECH_PRINTF(fp, "Destination port  : Not Config\n");
        }

       TECH_PRINTF(fp, "\n");
    }

    return SYS_ERR_OK;
}

#endif

#ifdef CONFIG_SYS_FIBER

static int32 _sal_tech_optical_show(FILE *fp)
{
    uint32 bitrate;
    char portstr[SYS_BUF16_LEN];

    sys_logic_port_t port;
    sys_port_speed_t speed;
    sys_port_media_t media;
    sys_port_mediaType_t type;
    sys_fiber_detect_status_t detect;
    sys_fiber_detect_status_t oe;
    sys_fiber_detect_status_t los;
    sys_optical_data_t data;

    TECH_PRINTF(fp, " Port |  Type |  Media |  Status (OE/LOS) | Nominal Bitrate (SFP) | Config \n");
    TECH_PRINTF(fp, "------+-------+--------+------------------+-----------------------+--------\n");

    FOR_EACH_LOGIC_PORT(port)
    {
        detect = FIBER_DETECT_STATUS_OFF;
        oe = FIBER_DETECT_STATUS_OFF;
        los = FIBER_DETECT_STATUS_OFF;
        media = PORT_MEDIA_COPPER;
        type = SYS_PORT_MEDIA_COPPER;

        type = LP2MEDIA(port);

        if (SYS_PORT_MEDIA_COMBO == type)
        {
            SYS_ERR_CONTINUE(sal_fiber_detectStatus_get(port, &detect));
            SYS_ERR_CONTINUE(sal_fiber_oeStatus_get(port, &oe));
            SYS_ERR_CONTINUE(sal_fiber_losStatus_get(port, &los));
            SYS_ERR_CONTINUE(sal_port_linkMedia_get(port, &media));
        }
        else if (SYS_PORT_MEDIA_FIBER == type)
        {
            media = PORT_MEDIA_FIBER;
            SYS_ERR_CONTINUE(sal_fiber_detectStatus_get(port, &detect));
            SYS_ERR_CONTINUE(sal_fiber_oeStatus_get(port, &oe));
            SYS_ERR_CONTINUE(sal_fiber_losStatus_get(port, &los));
        }
        else
        {
            continue;
        }

        SYS_ERR_CONTINUE(LP2STR(port, portstr));

        TECH_PRINTF(fp, " %4s | %-5s | %-6s | %-6s (%3s/%3s) |",
                    portstr, SYS_PORT_MEDIA_FIBER == type ? "Fiber" : "Combo",
                    PORT_MEDIA_FIBER == media ? "Fiber" : "Copper",
                    FIBER_DETECT_STATUS_ON == detect ? "Insert" : "---",
                    FIBER_DETECT_STATUS_ON == oe ? "Ins" : "---",
                    FIBER_DETECT_STATUS_OFF == los ? "Ins" : "---");

        if (FIBER_DETECT_STATUS_ON == detect)
        {
            bitrate = 0;
            SYS_MEM_CLEAR(data);

            rsd_optical_data_get(port, OPTICAL_CMD_GET_BR_NOMINAL, &data);
            SYS_ERR_CONTINUE(sal_optical_bitRateNominal_get(port, &bitrate));
            SYS_ERR_CONTINUE(sal_port_speed_get(port, &speed));

            TECH_PRINTF(fp, " %-8u (0x%08X) | %-6s", bitrate, data.word[0],
                        (ENABLED == speed.autoSpeed && SYS_PORT_SPEED_ALL == speed.ability) ? "AUTO" : "USER");
        }

        TECH_PRINTF(fp, "\n");
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
static int32 _sal_tech_rmon_show(FILE *fp)
{
    sys_logic_port_t logicPort;
    char portStr[CAPA_PORT_STR_LEN];
    uint32 i = 0;
    uint32 j = 0;
    uint32 ret = SYS_ERR_OK;
    uint64 result = 0;
    uint32 eventIndex = 0;
    uint32 historyIndex = 0;
    sal_rmon_event_t event;
    sal_rmon_event_log_t eventLog[RMON_EVENT_LOG_DFL];
    sal_rmon_alarm_t alarm;
    sal_rmon_history_t history;
    sal_rmon_history_log_t historyLog[RMON_HISTORY_BUCKET_DFL];

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        i = 0;
        osal_memset(portStr, 0, sizeof(portStr));
        SYS_ERR_CHK(LP2STR(logicPort, portStr));

        TECH_PRINTF(fp, "==== Port %2s =============================   \n", portStr);

        while(rmonMibTbl[i].name !=NULL)
        {
            if (rmonMibTbl[i].sal_mib_func != NULL)
                ret = rmonMibTbl[i].sal_mib_func(logicPort, &result);

            if (SYS_ERR_OK == ret)
                TECH_PRINTF(fp, "%-32s: %llu\n", rmonMibTbl[i].name, result);

            i++;
        }

        TECH_PRINTF(fp, "\n");
    }

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        osal_memset(&event, 0 , sizeof(sal_rmon_event_t));
        event.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_event_get(&event));

        if (0 != event.eventIndex)
        {
            SYS_ERR_CHK(sal_rmon_eventRuntime_get(&event));

            TECH_PRINTF(fp, "Rmon Event Index       : %u \n", event.eventIndex);
            TECH_PRINTF(fp, "Rmon Event Type        : %s  \n", text_rmon_event_type[event.eventType].text);
            TECH_PRINTF(fp, "Rmon Event Community   : %s  \n", event.community);
            TECH_PRINTF(fp, "Rmon Event Description : %s  \n", event.description);
            TECH_PRINTF(fp, "Rmon Event Last Sent   : %s  \n", event.eventLastTimeSent);
            TECH_PRINTF(fp, "Rmon Event Owner       : %s  \n", event.owner);
            TECH_PRINTF(fp, "\n");

            osal_memset(&eventLog, 0 , sizeof(eventLog));
            SYS_ERR_CHK(sal_rmon_eventLog_get(eventLog, eventIndex));

            for (j = RMON_EVENT_LOG_DFL -1; j >= 0 ; j--)
            {
                if (0 == eventLog[j].index)
                    continue;

                TECH_PRINTF(fp, "=======================================================\n");
                TECH_PRINTF(fp, "Index       : %u\n", eventLog[j].index);
                TECH_PRINTF(fp, "Time        : %s\n", eventLog[j].logTime);
                TECH_PRINTF(fp, "Description : %s\n", eventLog[j].description);
                TECH_PRINTF(fp, "\n");
            }
        }
    }

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        SYS_MEM_CLEAR(alarm);
        alarm.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_alarm_get(&alarm));

        if (0 != alarm.alarmIndex)
        {
            
            SYS_MEM_CLEAR(portStr);
            SYS_ERR_CHK(LP2STR(alarm.samplePort, portStr));

            TECH_PRINTF(fp, "Rmon Alarm Index             : %u  \n", alarm.alarmIndex);
            TECH_PRINTF(fp, "Rmon Alarm Sample Interval   : %u  \n", alarm.interval);
            TECH_PRINTF(fp, "Rmon Alarm Sample Interface  : %s  \n", portStr);
            TECH_PRINTF(fp, "Rmon Alarm Sample Variable   : %s  \n", text_rmon_sample[alarm.variable].text);
            TECH_PRINTF(fp, "Rmon Alarm Sample Type       : %s  \n", text_rmon_alarm_sample_type[alarm.sampleType].text);
            TECH_PRINTF(fp, "Rmon Alarm Type              : %s  \n", text_rmon_alarm_type[alarm.alarmType].text);
            TECH_PRINTF(fp, "Rmon Alarm Rising Threshold  : %u  \n", alarm.risingThreshold);
            TECH_PRINTF(fp, "Rmon Alarm Rising Event      : %u  \n", alarm.risingEventIndex);
            TECH_PRINTF(fp, "Rmon Alarm Falling Threshold : %u  \n", alarm.fallingThreshold);
            TECH_PRINTF(fp, "Rmon Alarm Falling Event     : %u  \n", alarm.fallingEventIndex);
            TECH_PRINTF(fp, "Rmon Alarm Owner             : %s  \n", alarm.owner);
            TECH_PRINTF(fp, "\n");
        }
    }

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        SYS_MEM_CLEAR(history);

        history.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_history_get(&history));

        if (0 != history.historyIndex)
        {
            SYS_MEM_CLEAR(portStr);
            SYS_ERR_CHK(sal_rmon_history_get(&history));
            SYS_ERR_CHK(LP2STR(history.datasource, portStr));

            TECH_PRINTF(fp, "Rmon History Index       : %u \n", history.historyIndex);
            TECH_PRINTF(fp, "Rmon Collection Interface: %s \n", portStr);
            TECH_PRINTF(fp, "Rmon History Bucket      : %u \n", history.bucketRequested);
            TECH_PRINTF(fp, "Rmon history Interval    : %u \n", history.interval);
            TECH_PRINTF(fp, "Rmon History Owner       : %s \n", history.owner);
            TECH_PRINTF(fp, "\n");

            osal_memset(historyLog,0 ,sizeof(historyLog));

            SYS_ERR_CHK(sal_rmon_historyTable_get(historyLog, historyIndex));

            for (j = RMON_HISTORY_BUCKET_DFL -1; j >= 0 ; j--)
            {
                if (0 == historyLog[i].sampleIndex)
                    continue;

                TECH_PRINTF(fp, "=======================================================\n");
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_SAMPLE_INDEX].text, historyLog[j].sampleIndex);
                TECH_PRINTF(fp, "%-20s : %s\n", text_rmon_history[RMON_HISTORY_INTERVAL_START].text, historyLog[j].intervalStart);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_DROPEVENTS].text, historyLog[j].dropEvent);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_OCTETS].text, historyLog[j].octets);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_PKTS].text, historyLog[j].pkts);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_BROADCASTPKTS].text, historyLog[j].broadcastPkts);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_MULTICASTPKTS].text, historyLog[j].multicastPkts);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_CRCALIGNERRORS].text, historyLog[j].crcAlignErrors);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_UNDERSIZEPKTS].text, historyLog[j].undersizePkts);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_OVERSIZEPKTS].text, historyLog[j].oversizePkts);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_FRAGMENTS].text, historyLog[j].fragments);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_JABBERS].text, historyLog[j].jabbers);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_COLLISIONS].text, historyLog[j].collisions);
                TECH_PRINTF(fp, "%-20s : %u\n", text_rmon_history[RMON_HISTORY_UTILIZATION].text, historyLog[j].utilization);
                TECH_PRINTF(fp, "\n");
            }

        }
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
static int32 _sal_tech_ds_show(FILE *fp)
{
    char vlanStr[CAPA_VLANLIST_STR_LEN];
    sys_vlanmask_t vlanmask;
    sys_enable_t enable = DISABLED;
    sys_dhcp_option82_id_t id;
    sys_mac_t sysMac;
    char portStr[CAPA_PORT_STR_LEN];
    uint32 rate = 0;
    sys_logic_port_t lp = 0;
    sys_enable_t trust = DISABLED;
    sys_enable_t ccenable = DISABLED;
    sys_enable_t enable82 = DISABLED;
    sys_dhcp_mib_t counter;

    VLANMASK_CLEAR_ALL(vlanmask);
    osal_memset(&vlanStr, 0, sizeof(vlanStr));
    osal_memset(&id, 0 ,sizeof(id));

    SYS_ERR_CHK(sal_dhcp_snooping_enable_get(&enable));
    SYS_ERR_CHK(sal_dhcp_snooping_vlanmask_enable_get(&vlanmask));
    SYS_ERR_CHK(sal_dhcp_option82_remoteID_get(&id));
    VMSK2STR(&vlanmask, vlanStr);

    if (0 == osal_strlen(id.str))
    {
        SYS_ERR_CHK(sal_sys_macAddr_get(&sysMac));
            osal_memset(&id, 0, sizeof(sys_dhcp_option82_id_t));
            osal_sprintf(id.str,"%02x:%02x:%02x:%02x:%02x:%02x (Switch Mac in Byte Order)",
            sysMac.octet[0], sysMac.octet[1], sysMac.octet[2],
            sysMac.octet[3], sysMac.octet[4], sysMac.octet[5]);
    }

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "DHCP Snooping             : %s\n", text_enable[enable].text);
    TECH_PRINTF(fp, "Enable on following Vlans : %s\n", (0 != osal_strlen(vlanStr)) ? vlanStr : "None");
    TECH_PRINTF(fp, "    circuit-id default format: vlan-port\n");
    TECH_PRINTF(fp, "    remote-id:               : %s\n\n", id.str);

    TECH_PRINTF(fp, " Interfaces | Trust State | Rate (pps) | hwaddr Check | Insert Option82 |\n");
    TECH_PRINTF(fp, "------------+-------------+------------+--------------+-----------------+\n");

    FOR_EACH_LOGIC_PORT(lp)
    {
        LP2STR(lp, portStr);
        SYS_ERR_CHK(sal_dhcp_snooping_port_trust_get(lp, &trust));
        SYS_ERR_CHK(sal_dhcp_snooping_rateLimit_get(lp, &rate));
        SYS_ERR_CHK(sal_dhcp_snooping_portChaddrChk_get(lp, &ccenable));
        SYS_ERR_CHK(sal_dhcp_option82_enable_get(lp, &enable82));

        TECH_PRINTF(fp, " %-10s |", portStr);
        TECH_PRINTF(fp, " %-11s |", (DISABLED == trust) ? "Untrusted" : "Trusted");

        if (rate != BOND_RATE_UNLIMITED)
            TECH_PRINTF(fp, " %-10d |", rate);
        else
            TECH_PRINTF(fp, " %-10s |", "None");

        TECH_PRINTF(fp, " %-12s |", text_enable[ccenable].text);
        TECH_PRINTF(fp, " %-15s |", text_enable[enable82].text);
        TECH_PRINTF(fp, "\n");
    }

    osal_memset(&id, 0 ,sizeof(id));
    SYS_ERR_CHK(sal_dhcp_option82_remoteID_get(&id));

    TECH_PRINTF(fp, "Remote ID: %s\n", id.str);

    TECH_PRINTF(fp, " Interfaces | Forwarded | Chaddr Check Dropped | Untrust Port Dropped | Untrust Port With Option82 Dropped | Invalid Drop\n");
    TECH_PRINTF(fp, "-----------+-----------+----------------------+----------------------+------------------------------------+--------------\n");

    FOR_EACH_LOGIC_PORT(lp)
    {
        LP2STR(lp, portStr);

        TECH_PRINTF(fp, " %-10s |", portStr);

        SYS_ERR_CHK(sal_dhcp_snooping_portMib_get(lp, &counter));
        TECH_PRINTF(fp, " %-9u | %-20u | %-20u | %-34u | %-u\n", counter.dhcpForward, counter.dhcpChaddrChkDrop,
                counter.dhcpUntrustDrop, counter.dhcpUntrustWithOption82Drop, counter.dhcpInvalidDrop);
    }

    TECH_PRINTF(fp, "\n");

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_DAI
static int32 _sal_tech_dai_show(FILE *fp)
{
    char vlanStr[CAPA_VLANLIST_STR_LEN];
    sys_vlanmask_t vlanmask;
    sys_enable_t enable = DISABLED;
    char portStr[CAPA_PORT_STR_LEN];
    uint32 rate = 0;
    sys_enable_t trust = DISABLED;
    sys_logic_port_t lp;
    sys_enable_t smacChk = DISABLED;
    sys_enable_t dmacChk = DISABLED;
    sys_enable_t ipChk = DISABLED;
    sys_enable_t ipZeroAllow = DISABLED;
    sys_arp_mib_t counter;

    VLANMASK_CLEAR_ALL(vlanmask);
    osal_memset(&vlanStr, 0, sizeof(vlanStr));

    SYS_ERR_CHK(sal_dai_vlanmask_enable_get(&vlanmask));
    SYS_ERR_CHK(sal_dai_enable_get(&enable));
    VMSK2STR(&vlanmask, vlanStr);

    TECH_PRINTF(fp, "Dynamic ARP Inspection     : %s\n", text_enable[enable].text);
    TECH_PRINTF(fp, "Enable on Vlans            : %s\n\n", (0 != osal_strlen(vlanStr)) ? vlanStr : "None");

    TECH_PRINTF(fp, " Interfaces | Trust State | Rate (pps) | SMAC Check | DMAC Check | IP Check/Allow Zero |\n");
    TECH_PRINTF(fp, "------------+-------------+------------+------------|------------+---------------------+\n");

    FOR_EACH_LOGIC_PORT(lp)
    {
        LP2STR(lp, portStr);
        SYS_ERR_CHK(sal_dai_port_trust_get(lp, &trust));
        SYS_ERR_CHK(sal_dai_rateLimit_get(lp, &rate));
        SYS_ERR_CHK(sal_dai_port_smacChk_get(lp, &smacChk));
        SYS_ERR_CHK(sal_dai_port_dmacChk_get(lp, &dmacChk));
        SYS_ERR_CHK(sal_dai_port_ipChk_get(lp, &ipChk));
        SYS_ERR_CHK(sal_dai_port_ipZeroAllow_get(lp, &ipZeroAllow));

        TECH_PRINTF(fp, " %-10s |", portStr);
        TECH_PRINTF(fp, " %-11s |", (DISABLED == trust) ? "Untrusted" : "Trusted");

        if (rate != BOND_RATE_UNLIMITED)
            TECH_PRINTF(fp, " %-10d |", rate);
        else
            TECH_PRINTF(fp, " %-10s |", "None");

        TECH_PRINTF(fp, " %-10s |", text_enable[smacChk].text);
        TECH_PRINTF(fp, " %-10s |", text_enable[dmacChk].text);
        TECH_PRINTF(fp, " %-8s/%-10s", text_enable[ipChk].text, text_enable[ipZeroAllow].text);
        TECH_PRINTF(fp, "\n");
    }

    TECH_PRINTF(fp, "Port| Forward |Source MAC Failures|Dest MAC Failures|SIP Validation Failures|DIP Validation Failures|IP-MAC Mismatch Failures\n");
    TECH_PRINTF(fp, "----+---------+-------------------+-----------------+-----------------------+-----------------------+------------------------\n");

    FOR_EACH_LOGIC_PORT(lp)
    {
        LP2STR(lp, portStr);
        SYS_ERR_CHK(sal_dai_portMib_get(lp, &counter));

        TECH_PRINTF(fp, "%4s|", portStr);
        TECH_PRINTF(fp, " %7u |        %10u |      %10u |            %10u |            %10u |          %u\n",
            counter.arpForward, counter.arpSmacDrop, counter.arpDmacDrop, counter.arpSIPDrop,
            counter.arpDIPDrop, counter.arpIPMACMismatchDrop);
    }

    TECH_PRINTF(fp, "\n");

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_ISG
static int32 _sal_tech_isg_show(FILE *fp)
{
    char portStr[CAPA_PORT_STR_LEN];
    int32 num = -1;
    uint32 cnum = 0;
    sys_enable_t enable = DISABLED;
    sys_isg_verify_type_t verifyType = ISG_VERIFY_IP;
    sys_logic_port_t logicPort;
    char macStr[CAPA_MAC_STR_LEN];
    char ipStr[CAPA_IPV4_STR_LEN];
    char maskStr[CAPA_IPV4_STR_LEN];
    uint32 i;
    sys_mac_t zeroMac;
    sys_isg_entry_t entry;

    TECH_PRINTF(fp, "  Port  |     Status     | Max Entry | Current Entry \n");
    TECH_PRINTF(fp, "--------+----------------+-----------+---------------\n");

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        osal_memset(portStr, 0, sizeof(portStr));
        SYS_ERR_CHK(LP2STR(logicPort, portStr));
        SYS_ERR_CHK(sal_isg_port_get(logicPort, &enable, &verifyType));
        SYS_ERR_CHK(sal_isg_portMaxEntryNum_get(logicPort, &num));
        SYS_ERR_CHK(sal_isg_portCurrentEntryNum_get(logicPort, &cnum));

        TECH_PRINTF(fp, " %6s |", portStr);

        if (DISABLED == enable)
            TECH_PRINTF(fp, " %14s |", text_enable[enable].text);
        else
        {
            if (ISG_VERIFY_IP == verifyType)
                TECH_PRINTF(fp, " %14s |", "Verify IP");
            else
                TECH_PRINTF(fp, " %14s |", "Verify MAC+IP");
        }

        if (IPSG_ENTRY_UNLIMITED == num)
            TECH_PRINTF(fp, " No Limit  |");
        else
            TECH_PRINTF(fp, " %4d      |", num);

        TECH_PRINTF(fp, " %3d \n", cnum);
    }

    TECH_PRINTF(fp, "\n");

    osal_memset(&zeroMac, 0, sizeof(zeroMac));
    osal_memset(&macStr, 0, sizeof(macStr));
    osal_memset(&ipStr, 0, sizeof(ipStr));
    osal_memset(&maskStr, 0, sizeof(maskStr));

    TECH_PRINTF(fp, "\nBind Table: Maximun Binding Entry Number %u\n", CAPA_IPSG_ENTRY_NUM);
#ifdef CONFIG_SYS_PROTO_ISG_NETMASK
    TECH_PRINTF(fp, "  Port  | VID  |    MAC Address    |              IP                |    Type     | Lease Time\n");
    TECH_PRINTF(fp, "--------+------+-------------------+--------------------------------+-------------+-----------\n");
#else
    TECH_PRINTF(fp, "  Port  | VID  |    MAC Address    |       IP        |    Type     | Lease Time\n");
    TECH_PRINTF(fp, "--------+------+-------------------+-----------------+-------------+-----------\n");
#endif
    for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
    {
        osal_memset(&entry, 0, sizeof(sys_isg_entry_t));
        SYS_ERR_CONTINUE(sal_isg_bind_entry_get(i, &entry));

        if (!entry.isUsed)
            continue;

        if (ISG_ENTRY_ALL_DENY == entry.type)
            continue;

        SYS_ERR_CHK(LP2STR(entry.port, portStr));
        SYS_ERR_CHK(sys_util_ip2Str(entry.ip, ipStr));

        TECH_PRINTF(fp, " %6s |", portStr);
        TECH_PRINTF(fp, " %4d |", entry.vid);

        if (0 == osal_memcmp(&zeroMac, &entry.mac, sizeof(sys_mac_t)))
        {
            SYS_ERR_CHK(sys_util_ip2Str(entry.mask, maskStr));
            TECH_PRINTF(fp, " %17s |", "N/A");
            TECH_PRINTF(fp, "%15s(%15s)|", ipStr, maskStr);
        }
        else
        {
            SYS_ERR_CHK(sys_util_mac2Str(entry.mac.octet, macStr));
            TECH_PRINTF(fp, " %17s |", macStr);
#ifdef CONFIG_SYS_PROTO_ISG_NETMASK
            TECH_PRINTF(fp, "%15s(255.255.255.255)|", ipStr);
#else
            TECH_PRINTF(fp, " %15s |", ipStr);
#endif
        }

        if (ISG_ENTRY_STATIC == entry.type)
            TECH_PRINTF(fp, "   Static    | NA\n");
        else
            TECH_PRINTF(fp, "DHCP Snooping| %u\n", entry.leaseTime);
    }

    TECH_PRINTF(fp, "\n");

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    int32 tmptime = 0;
    uint32 timer = 0;
    uint32 uptime = 0;
    sys_isg_db_t db;
    sys_isg_db_statistics_t dbs;

    SYS_MEM_CLEAR(db);
    SYS_ERR_CHK(sal_isg_db_get(&db));
    SYS_ERR_CHK(sal_isg_db_statistics_get(&dbs));
    SYS_ERR_CHK(sal_isg_db_timer_get(&timer));

    TECH_PRINTF(fp, "\n");
    if (ISG_DB_TYPE_FLASH == db.dbType)
        TECH_PRINTF(fp, "Type : Flash\n");
    else if (ISG_DB_TYPE_TFTP == db.dbType)
        TECH_PRINTF(fp, "Type : tftp: %s\n", db.server);
    else
        TECH_PRINTF(fp, "Type : None\n");

    TECH_PRINTF(fp, "FileName : %s\n", (ISG_DB_TYPE_TFTP == db.dbType) ? db.filename : "");
    TECH_PRINTF(fp, "Write delay Timer : %u seconds\n", db.writeDelay);
    TECH_PRINTF(fp, "Abort Timer : %u seconds\n", db.timeout);

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "Agent Running : %s\n", (dbs.isRunning) ? "Running" : "None");
    if (TRUE == dbs.isRunning)
        TECH_PRINTF(fp, "Delay Timer Expiry : %u seconds\n", timer);
    else
        TECH_PRINTF(fp, "Delay Timer Expiry : Not Running\n");

    if (TRUE == dbs.isRunning)
    {
        SYS_ERR_CHK(osal_time_sysUptime_get(&uptime));
        tmptime = db.timeout - ((uptime - dbs.startTime)/1000);
        TECH_PRINTF(fp, "Abort Timer Expiry : %u\n", (tmptime > 0) ? tmptime : 0);
    }
    else
        TECH_PRINTF(fp, "Abort Timer Expiry :Not Running\n");

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "Last Succeded Time : %s\n", (0 == osal_strlen(dbs.lastSuccessedTime.tstr)) ? "None" : dbs.lastSuccessedTime.tstr);
    TECH_PRINTF(fp, "Last Failed Time : %s\n", (0 == osal_strlen(dbs.lastFailedTime.tstr)) ? "None" : dbs.lastFailedTime.tstr);
    TECH_PRINTF(fp, "Last Failed Reason : %s\n", dbs.lastFaildReason);

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "Total Attempts       : %5u\n", dbs.totalAttempts);
    TECH_PRINTF(fp, "Successful Transfers : %5u   Failed Transfers : %5u\n", dbs.successfulTransfer, dbs.failedTransfer);
    TECH_PRINTF(fp, "Successful Reads     : %5u   Failed Reads     : %5u\n", dbs.successfulReads, dbs.failedReads);
    TECH_PRINTF(fp, "Successful Writes    : %5u   Failed Writes    : %5u\n", dbs.successfulWrites, dbs.failedWrites);
    TECH_PRINTF(fp, "\n");

#endif
    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_UI_SNMP
static int32 _sal_tech_snmp_show(FILE *fp)
{
    #ifdef CONFIG_SYS_SNMPV3
        sal_snmp_com2sec_t com2sec;
        sal_snmp_remoteEngine_t remote;
        sal_snmp_access_t access;
        sal_snmp_user_t user;
        sal_snmp_view_t view;
        char engineId[BOND_SNMP_ENGINEID_LEN_MAX+1];
        char ip_str[16];
        sal_snmp_target_t host;
        char portStr[16], reTryStr[16], timeStr[16];
        char oidStr[CAPA_SNMP_OID_STR_LEN];
        uint32  index;
    #else
        uint32 i = 0;
        sal_snmp_target_t trap;
        sal_snmp_community_t community;
    #endif

        sys_enable_t authEnable = DISABLED;
        sys_enable_t linkEnable = DISABLED;
        sys_enable_t warmEnable = DISABLED;
        sys_enable_t coldEnable = DISABLED;
        sys_enable_t enable;
        uint32 count = 0;

        SYS_ERR_CHK(sal_snmp_enable_get(&enable));

        if (ENABLED == enable)
            TECH_PRINTF(fp,"SNMP is enabled.\n");
        else
            TECH_PRINTF(fp,"SNMP is disabled.\n");

        TECH_PRINTF(fp,"\n");

#ifdef CONFIG_SYS_SNMPV3
     
#else
        TECH_PRINTF(fp,"Community Name  \tAccess Right \n");
        TECH_PRINTF(fp,"-----------------------------------------\n");

        for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
        {
            SYS_ERR_CHK(sal_snmp_community_get(i, &community));

            if (osal_strlen(community.name) != 0)
            {
                if (SNMP_COMMUNITY_ACCESS_READ_ONLY == community.access)
                    TECH_PRINTF(fp,"%-16s\t read-only\n", community.name);
                else
                    TECH_PRINTF(fp,"%-16s\t read-write\n", community.name);
                count++;
            }

        }
        TECH_PRINTF(fp,"\nTotal Community Entries: %d\n", count);

        count = 0;
        TECH_PRINTF(fp,"Server             Community Name   Notification Version    Notification Type    \n");
        TECH_PRINTF(fp,"---------------------------------------------------------------------------------\n");
        for (i = 0; i < CAPA_SNMP_HOST_NUM; i++)
        {
            SYS_ERR_CHK(sal_snmp_target_get(i, &trap));

            if (osal_strlen(trap.server) != 0)
            {
                TECH_PRINTF(fp,"%-18s %-17s%-23s %-20s \n", trap.server, trap.name,
                        gsecModel[trap.secModel], gtrapType[trap.trapType]);
                count++;
            }
        }
        TECH_PRINTF(fp,"\nTotal Trap Entries: %d\n", count);
        TECH_PRINTF(fp,"\n");
#endif

        SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &authEnable));
        TECH_PRINTF(fp,"SNMP auth failed trap  : %s \n", (authEnable == ENABLED) ? "Enable" : "Disable");

        SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_LINKUPDOWN, &linkEnable));
        TECH_PRINTF(fp,"SNMP linkUpDown trap   : %s \n", (linkEnable == ENABLED) ? "Enable" : "Disable");

        SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_COOL, &coldEnable));
        TECH_PRINTF(fp,"SNMP cold-start trap   : %s \n", (coldEnable == ENABLED) ? "Enable" : "Disable");

        SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_WARM, &warmEnable));
        TECH_PRINTF(fp,"SNMP warm-start trap   : %s \n", (warmEnable == ENABLED) ? "Enable" : "Disable");
        TECH_PRINTF(fp,"\n");

#ifdef CONFIG_SYS_SNMPV3
        
        count = 0;
        TECH_PRINTF(fp,"Commnunity Name      Group Name                     View                           Access \n");
        TECH_PRINTF(fp,"-------------------- ------------------------------ ------------------------------ ------------\n");

    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++) {
        osal_memset(&com2sec, 0, sizeof(sal_snmp_com2sec_t));
        SYS_ERR_CHK(sal_snmp_com2sec_get(index, &com2sec));

        if (osal_strlen(com2sec.comName) != 0)
        {
            TECH_PRINTF(fp,"%-21s",com2sec.comName);
            if (SNMP_COM_MODE_BASIC ==  com2sec.mode)
            {
                TECH_PRINTF(fp,"%-31s", "");
                TECH_PRINTF(fp,"%-31s", com2sec.viewName);
                TECH_PRINTF(fp,"%s\n", gaccess[com2sec.access]);
            }
            else
            {
                TECH_PRINTF(fp,"%-31s", com2sec.groupName);
                TECH_PRINTF(fp,"%-31s", "");
                TECH_PRINTF(fp,"%s\n", "");
            }

            count++;
        }

    }
    TECH_PRINTF(fp,"\nTotal Entries: %d\n", count);
    TECH_PRINTF(fp,"\n");

    count = 0;

    osal_memset(engineId, 0, sizeof(engineId));
    SYS_ERR_CHK(sal_snmp_engineid_get(engineId));
    TECH_PRINTF(fp,"Local SNMPV3 Engine id: %s \n\n", engineId);

    TECH_PRINTF(fp,"     IP address                   Remote SNMP engineID               \n");
    TECH_PRINTF(fp,"--------------------------------- -----------------------------------------------------------------\n") ;

    for (index = 0; index < CAPA_SNMP_REMOTE_ENGINEID_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_remoteEngine_get(index, &remote));

        if(osal_strlen(remote.server) != 0)
        {
            TECH_PRINTF(fp,"%-33s %s \n", remote.server, remote.engineId);
            count++;
        }
    }
    TECH_PRINTF(fp,"\nTotal Entries: %d\n\n", count);
    TECH_PRINTF(fp,"\n");

    count = 0;

    TECH_PRINTF(fp,"Group Name                     Model  Level        ReadView                       WriteView                      NotifyView   \n");
    TECH_PRINTF(fp,"------------------------------ ------ ------------ ------------------------------ ------------------------------ ------------------------------\n");

    for (index = 0; index < SYS_SNMP_ACCESS_NUM_MAX; index++) {
        SYS_ERR_CHK(sal_snmp_access_get(index, &access));

        if (osal_strlen(access.groupName) != 0)
        {
            if (0 == osal_memcmp(access.groupName, SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
                continue;

            TECH_PRINTF(fp,"%-31s",access.groupName);
            TECH_PRINTF(fp,"%-7s", gsecModel[access.secMode] );
            TECH_PRINTF(fp,"%-13s",gsecLevel[access.secLevel]);

            if(osal_strlen(access.readView) == 0)
            TECH_PRINTF(fp,"%-31s", "---");
            else
            TECH_PRINTF(fp,"%-31s", access.readView);

            if(osal_strlen(access.writeView) == 0)
            TECH_PRINTF(fp,"%-31s", "---");
            else
            TECH_PRINTF(fp,"%-31s", access.writeView);

            if(osal_strlen(access.notifyView) == 0)
            TECH_PRINTF(fp,"%-31s \n", "---");
            else
            TECH_PRINTF(fp,"%-31s \n", access.notifyView);

            count++;
        }

    }
    TECH_PRINTF(fp,"\nTotal Entries: %d\n", count);
    TECH_PRINTF(fp,"\n");

    count = 0;

    osal_memset(ip_str, 0, sizeof(ip_str));

    TECH_PRINTF(fp,"Server             Community/User Name   Notification Version    Notification Type    UDP Port    Retries     Timeout\n");
    TECH_PRINTF(fp,"---------------------------------------------------------------------------------------------------------------------\n");
    for (index = 0; index < CAPA_SNMP_HOST_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_target_get(index, &host));
        if (0 != osal_strlen(host.server))
        {
            osal_memset(portStr, 0, sizeof(portStr));
            osal_memset(reTryStr, 0, sizeof(reTryStr));
            osal_memset(timeStr, 0, sizeof(timeStr));
            osal_sprintf(portStr,"%d", host.portNum);

            if (SNMP_INFORM_V2 == host.trapType || SNMP_INFORM_V3 == host.trapType)
            {
                osal_sprintf(reTryStr,"%d", host.retries);
                osal_sprintf(timeStr,"%d", host.timeout);
            }
            else
            {
                osal_sprintf(reTryStr,"%s", "--");
                osal_sprintf(timeStr,"%s", "--");
            }
            TECH_PRINTF(fp,"%-18s %-22s%-23s %-20s %-12s%-12s %s  \n", host.server, host.name,
                    gsecModel[host.secModel], gtrapType[host.trapType], portStr, reTryStr, timeStr);

            count++;
        }
    }
    TECH_PRINTF(fp,"\nTotal Entries: %d\n", count);
    TECH_PRINTF(fp,"\n");

    count = 0;

    for (index = 0; index < CAPA_SNMP_USER_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_user_get(index, &user));

        if(osal_strlen(user.userName) != 0)
        {
            TECH_PRINTF(fp,"Username:                %s\n", user.userName);
            TECH_PRINTF(fp,"Password:                ********\n");
            TECH_PRINTF(fp,"Privilege Mode:          %s \n", gaccess[user.access]);
            TECH_PRINTF(fp,"Access GroupName:        %s \n", user.groupName);
            TECH_PRINTF(fp,"Authentication Protocol: %s\n", gsslType[user.auth_protp]);
            TECH_PRINTF(fp,"Encryption Protocol:     %s\n",  gsslType[user.encrypt_proto]);
            TECH_PRINTF(fp,"Access SecLevel:         %s\n\n",  gsecLevel[user.selLevel]);

            count++;
        }
    }
    TECH_PRINTF(fp,"\nTotal Entries: %d\n", count);
    TECH_PRINTF(fp,"\n");

    count = 0;

    TECH_PRINTF(fp,"View Name                      Subtree OID                               OID Mask                  View Type\n");
    TECH_PRINTF(fp,"---------------------------    --------------------------------------    ----------------------    ---------\n");

    for (index = 0; index < CAPA_SNMP_VIEW_NUM; index++) {
        SYS_ERR_CHK(sal_snmp_view_get(index, &view));

        if (osal_strlen(view.viewName) != 0)
        {
            osal_memset(oidStr,0,sizeof(oidStr));
            sal_util_oid2str(view.subtreeOid.len, view.subtreeOid.id, oidStr);

            TECH_PRINTF(fp,"%-31s",view.viewName);
            TECH_PRINTF(fp,"%-42s", oidStr);
            TECH_PRINTF(fp,"%-26s", (view.viewMaskStr[0] == 0) ? "all" : view.viewMaskStr);
            TECH_PRINTF(fp,"%-9s\n",gviewTypeStr[view.viewType]);

            count++;
        }

    }
    TECH_PRINTF(fp,"\nTotal Entries: %d\n", count);
    TECH_PRINTF(fp,"\n");
#endif

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_APP_SSH
static int32 _sal_tech_ssh_show(FILE *fp)

{
    uint32 cnt;
    sys_enable_t enable;
#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    uint32 timeout;
#endif
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    sys_cliRetrySilent_t retrySilent;
#endif 

#ifdef CONFIG_USER_SSH_SSHD
    SYS_ERR_CHK(sal_ssh_sshdEnable_get(&enable));
    TECH_PRINTF(fp,"SSH Server      : %s\n", text_enable[enable].text);
#endif

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    SYS_ERR_CHK(sal_sys_cliSessionTimeout_get(SYS_LINE_CLI_SSH, &timeout));
    TECH_PRINTF(fp,"Session Timeout : %d (minutes)\n", timeout);
#endif 

    SYS_ERR_CHK(sal_sys_cliHistory_count_get(SYS_LINE_CLI_SSH, &cnt));
    TECH_PRINTF(fp,"History Count   : %d\n", cnt);

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    SYS_MEM_CLEAR(retrySilent);
    SYS_ERR_CHK(sal_sys_cliRetrySilent_get(SYS_LINE_CLI_SSH, &retrySilent));
    TECH_PRINTF(fp,"Password Retry  : %d\n", retrySilent.retryNum);
    TECH_PRINTF(fp,"Silent Time     : %d (seconds)\n", retrySilent.silentTime);
#endif 

    TECH_PRINTF(fp,"\n");

    _sal_tech_system_append(fp, "openssl version");
    TECH_PRINTF(fp,"\n");

    return SYS_ERR_OK;
}
#endif

static int32 _sal_tech_diag_register_show(FILE *fp)

{

#if 0
    _sal_tech_console_append(&rsd_debug_socRegister_dump);
    _sal_tech_console_append(&rsd_debug_macRegister_dump);
    _sal_tech_console_append(&rsd_debug_phyRegister_dump);
    _sal_tech_console_append(&rsd_debug_macTable_dump);
#else
    rsd_debug_socRegister_dump();
    rsd_debug_macRegister_dump();
    rsd_debug_phyRegister_dump();
    rsd_debug_macTable_dump();
#endif

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP
#ifdef CONFIG_SYS_PROTO_STP_MSTP
static int32 _sal_tech_mstp_show(FILE *fp)
{
    sys_stp_info_t stpInfo;
    sys_stp_mst_info_t mstInfo;
    sys_logic_port_t port;
    sys_stp_port_info_t portInfo;
    sys_stp_mst_port_info_t mstPortInfo;
    char strPort[16];
    char *strState[] = { "Dsbl", "Blk", "Lrn", "Frw" };
    char *strRole[] = { "Dsbl", "Mstr", "Root", "Desg", "Altn", "Bckp" };
    uint32 msti;
    char strVlan[CAPA_VLANLIST_STR_LEN];
    uint8 isCistRoot;
    uint8 isIstMaster;
    uint8 isRegionalRoot;

    SYS_ERR_CHK(sal_stp_info_get(&stpInfo));

    for (msti=0; msti<CAPA_STP_MST_INSTANCE_NUM; msti++)
    {
        SYS_ERR_CHK(sal_stp_mstInfo_get(msti, &mstInfo));

        if ((MSTP_INSTANCE_CIST != msti) && IS_VLANMASK_CLEAR(mstInfo.vlanmask))
        {
            continue;
        }

        SYS_ERR_CHK(VMSK2STR(&mstInfo.vlanmask, strVlan));
        TECH_PRINTF(fp, "###### MST %u Vlans Mapped: %s\n", msti, strVlan);

        if (MSTP_INSTANCE_CIST == msti)
        {   
            isCistRoot = (mstInfo.rootPortId.number == 0)? TRUE : FALSE;
            isIstMaster = IS_BID_EQUAL(&mstInfo.rootPriority.regionalRootBridgeId, &mstInfo.bridgeId)? TRUE : FALSE;

            TECH_PRINTF(fp, "CST Root ID    Priority    %u\n", mstInfo.rootPriority.rootBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
            TECH_PRINTF(fp, "               Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
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
                    TECH_PRINTF(fp, "               This switch is root for CST and IST master\n");
                }

                TECH_PRINTF(fp, "               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                    stpInfo.helloTime, stpInfo.maximumAge, stpInfo.forwardDelay);
                TECH_PRINTF(fp, "               Max hops    %u\n", stpInfo.maxHops);
            }
            else
            {
                if (mstInfo.rootPriority.externalRootPathCost == 0)
                {
                    TECH_PRINTF(fp, "               The IST ROOT is the CST ROOT\n");
                }
                else
                {
                    TECH_PRINTF(fp, "               Path Cost   %u\n", mstInfo.rootPriority.externalRootPathCost);
                }
                
                LP2STR((mstInfo.rootPortId.number - 1), strPort);
                TECH_PRINTF(fp, "               Root Port   %s\n", strPort);

                if (isIstMaster)
                {
                    TECH_PRINTF(fp, "               This switch is the IST master\n");
                    TECH_PRINTF(fp, "               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                        mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);
                }
                else
                {
                    TECH_PRINTF(fp, "               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                        mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);

                }

                TECH_PRINTF(fp, "Bridge ID      Priority    %u\n", mstInfo.bridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
                TECH_PRINTF(fp, "               Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
                    mstInfo.bridgeId.addr[0], \
                    mstInfo.bridgeId.addr[1], \
                    mstInfo.bridgeId.addr[2], \
                    mstInfo.bridgeId.addr[3], \
                    mstInfo.bridgeId.addr[4], \
                    mstInfo.bridgeId.addr[5]);
                TECH_PRINTF(fp, "               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                    stpInfo.helloTime, stpInfo.maximumAge, stpInfo.forwardDelay);
                TECH_PRINTF(fp, "               Max hops    %u\n", stpInfo.maxHops);
            }

            TECH_PRINTF(fp, "  Name     State   Prio.Nbr   Cost    Sts   Role EdgePort       Type\n");
            TECH_PRINTF(fp, "--------- -------- -------- -------- ------ ---- -------- -----------------\n");
            FOR_EACH_LOGIC_PORT(port)
            {
                SYS_ERR_CONTINUE(sal_stp_portInfo_get(port, &portInfo));
                SYS_ERR_CONTINUE(sal_stp_mstPortInfo_get(msti, port, &mstPortInfo));

                if (IS_LP_TRKMBR(port) || (STP_PORT_STATE_DISABLED == mstPortInfo.portState))
                    continue;

                LP2STR(port, strPort);
                TECH_PRINTF(fp, "%-9s %-8s %3u.%-4u %-8u %-6s %-4s %-8s ", \
                        strPort, \
                        (portInfo.adminEnable)? "enabled" : "disabled", \
                        portInfo.portId.priority, \
                        portInfo.portId.number, \
                        portInfo.operExtPathCost, \
                        strState[mstPortInfo.portState], \
                        strRole[mstPortInfo.portRole], \
                        (portInfo.confEdge)? "Yes" : "No");

                TECH_PRINTF(fp, "%s", (TRUE == portInfo.operPointToPointMac)? "P2P" : "Shared");
                if (STP_VERSION_MSTP == stpInfo.forceVersion)
                {   
                    TECH_PRINTF(fp, " %s", (STP_PORT_TYPE_BOUNDARY == portInfo.type)? "Bound" : "Intr");

                }
                if (STP_PORT_MODE_MSTP != portInfo.mode)
                {
                    TECH_PRINTF(fp, " %s", (STP_PORT_MODE_RSTP == portInfo.mode)? "(RSTP)" : "(STP)");
                }

                TECH_PRINTF(fp, "\n");
            }

            TECH_PRINTF(fp, "\n");
        }
        else
        {   
            isRegionalRoot = IS_BID_EQUAL(&mstInfo.rootPriority.regionalRootBridgeId, &mstInfo.bridgeId)? TRUE : FALSE;

            TECH_PRINTF(fp, "\n");
            TECH_PRINTF(fp, "Root ID        Priority    %u\n", mstInfo.rootPriority.regionalRootBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
            TECH_PRINTF(fp, "               Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
                mstInfo.rootPriority.regionalRootBridgeId.addr[0], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[1], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[2], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[3], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[4], \
                mstInfo.rootPriority.regionalRootBridgeId.addr[5]);

            if (isRegionalRoot)
            {
                TECH_PRINTF(fp, "               This switch is the regional Root\n");
            }
            else
            {
                TECH_PRINTF(fp, "               Path Cost   %u\n", mstInfo.rootPriority.internalRootPathCost);
                
                LP2STR((mstInfo.rootPortId.number - 1), strPort);
                TECH_PRINTF(fp, "               Root Port   %s\n", strPort);
                TECH_PRINTF(fp, "               Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
                    mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);

                TECH_PRINTF(fp, "\n");

                TECH_PRINTF(fp, "Bridge ID      Priority    %u\n", mstInfo.bridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
                TECH_PRINTF(fp, "               Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
                    mstInfo.bridgeId.addr[0], \
                    mstInfo.bridgeId.addr[1], \
                    mstInfo.bridgeId.addr[2], \
                    mstInfo.bridgeId.addr[3], \
                    mstInfo.bridgeId.addr[4], \
                    mstInfo.bridgeId.addr[5]);

            }
            
            TECH_PRINTF(fp, "Interfaces\n");
            TECH_PRINTF(fp, "Name       State     Prio.Nbr   Cost      Sts  Role  EdgePort  Type\n");
            TECH_PRINTF(fp, "--------   --------  --------   --------- ---- ----  --------  ----------\n");
            FOR_EACH_LOGIC_PORT(port)
            {
                SYS_ERR_CONTINUE(sal_stp_portInfo_get(port, &portInfo));
                SYS_ERR_CONTINUE(sal_stp_mstPortInfo_get(msti, port, &mstPortInfo));

                if (IS_LP_TRKMBR(port) || (STP_PORT_STATE_DISABLED == mstPortInfo.portState))
                    continue;

                LP2STR(port, strPort);
                TECH_PRINTF(fp, "%-8s   %-8s  %3u.%-4u   %-9u %-4s %-4s  %-8s  ", \
                        strPort, \
                        (portInfo.adminEnable)? "enabled" : "disabled", \
                        mstPortInfo.portId.priority, \
                        mstPortInfo.portId.number, \
                        mstPortInfo.operIntPathCost, \
                        strState[mstPortInfo.portState], \
                        strRole[mstPortInfo.portRole], \
                        (portInfo.confEdge)? "Yes" : "No");

                TECH_PRINTF(fp, "%s", (TRUE == portInfo.operPointToPointMac)? "P2P" : "Shared");
                if (STP_VERSION_MSTP == stpInfo.forceVersion)
                {   
                    TECH_PRINTF(fp, " %s", (STP_PORT_TYPE_BOUNDARY == portInfo.type)? "Bound" : "Intr");

                }
                if (STP_PORT_MODE_MSTP != portInfo.mode)
                {
                    TECH_PRINTF(fp, " %s", (STP_PORT_MODE_RSTP == portInfo.mode)? "(RSTP)" : "(STP)");
                }

                TECH_PRINTF(fp, "\n");
            }

            TECH_PRINTF(fp, "\n");
        }
    }

    return SYS_ERR_OK;
}
#else
static int32 _sal_tech_rstp_show(FILE *fp)
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

    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "  Root ID    Priority    %u\n", mstInfo.rootPriority.rootBridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
    TECH_PRINTF(fp, "             Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
        mstInfo.rootPriority.rootBridgeId.addr[0], \
        mstInfo.rootPriority.rootBridgeId.addr[1], \
        mstInfo.rootPriority.rootBridgeId.addr[2], \
        mstInfo.rootPriority.rootBridgeId.addr[3], \
        mstInfo.rootPriority.rootBridgeId.addr[4], \
        mstInfo.rootPriority.rootBridgeId.addr[5]);
    if (mstInfo.rootPortId.number > 0)
    {
        TECH_PRINTF(fp, "             Cost        %u\n", mstInfo.rootPriority.externalRootPathCost);
        
        LP2STR((mstInfo.rootPortId.number - 1), strPort);
        TECH_PRINTF(fp, "             Port        %s\n", strPort);
    }
    else
    {
        TECH_PRINTF(fp, "             This switch is the root\n");
    }
    TECH_PRINTF(fp, "             Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
        mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);

    if (mstInfo.rootPortId.number > 0)
    {
        TECH_PRINTF(fp, "  Bridge ID  Priority    %u\n", mstInfo.bridgeId.priority & MSTP_BRIDGE_PRIORITY_MASK);
        TECH_PRINTF(fp, "             Address     %02x:%02x:%02x:%02x:%02x:%02x\n", \
            mstInfo.bridgeId.addr[0], \
            mstInfo.bridgeId.addr[1], \
            mstInfo.bridgeId.addr[2], \
            mstInfo.bridgeId.addr[3], \
            mstInfo.bridgeId.addr[4], \
            mstInfo.bridgeId.addr[5]);
        TECH_PRINTF(fp, "             Hello Time  %u sec  Max Age %u sec  Forward Delay %u sec\n", \
            stpInfo.helloTime, stpInfo.maximumAge, stpInfo.forwardDelay);
    }
    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "  Number of topology changes %u last change occurred %02u:%02u:%02u ago\n", \
        mstInfo.tcTimes, \
        (mstInfo.tcTimer / 3600), \
        ((mstInfo.tcTimer % 3600) / 60), \
        ((mstInfo.tcTimer % 3600) % 60));
    TECH_PRINTF(fp, "  Times:  hold %u, topology change %u, notification %u\n", 0, 0 ,0); 
    TECH_PRINTF(fp, "          hello %u, max age %u, forward delay %u\n", \
        mstInfo.helloTime, mstInfo.maxAge, mstInfo.forwardDelay);
    TECH_PRINTF(fp, "\n");
    TECH_PRINTF(fp, "Interfaces\n");
    TECH_PRINTF(fp, "  Name     State   Prio.Nbr   Cost    Sts   Role EdgePort       Type\n");
    TECH_PRINTF(fp, "--------- -------- -------- -------- ------ ---- -------- -----------------\n");

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CONTINUE(sal_stp_portInfo_get(port, &portInfo));
        SYS_ERR_CONTINUE(sal_stp_mstPortInfo_get(0, port, &mstPortInfo));

        if (IS_LP_TRKMBR(port) || (STP_PORT_STATE_DISABLED == mstPortInfo.portState))
            continue;

        LP2STR(port, strPort);
        TECH_PRINTF(fp, " %7s  %-8s  %3u.%-3u %7u   %-4s  %-4s %8s ", \
            strPort, \
            (portInfo.adminEnable)? "enabled" : "disabled", \
            portInfo.portId.priority, \
            portInfo.portId.number, \
            portInfo.operExtPathCost, \
            strState[mstPortInfo.portState], \
            strRole[mstPortInfo.portRole], \
            (portInfo.confEdge)? "Yes" : "No");

        TECH_PRINTF(fp, "%s", (TRUE == portInfo.operPointToPointMac)? "P2P" : "Shared");
#ifdef CONFIG_SYS_PROTO_STP_MSTP
        if (STP_VERSION_MSTP == stpInfo.forceVersion)
        {   
            TECH_PRINTF(fp, " %s", (STP_PORT_TYPE_BOUNDARY == portInfo.type)? "Bound" : "Intr");
        }
        if (STP_PORT_MODE_MSTP != portInfo.mode)
        {
            TECH_PRINTF(fp, " %s", (STP_PORT_MODE_RSTP == portInfo.mode)? "(RSTP)" : "(STP)");
        }
#endif
        TECH_PRINTF(fp, "\n");
    }

    return SYS_ERR_OK;
}
#endif

static int32 _sal_tech_stp_show(FILE *fp)
{
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    uint32 i;
#endif
    char *strState[] = { "Dsbl", "Blk", "Lrn", "Frw" };

    sys_logic_port_t port;
    sys_port_stateCtrl_t stpStateCtrl;
    sys_port_stateCtrl_t chipStateCtrl;

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    for (i = 0; i < CAPA_STP_MST_INSTANCE_NUM; i++)
    {
        FOR_EACH_LOGIC_PORT(port)
        {
            SYS_MEM_CLEAR(stpStateCtrl);
            SYS_MEM_CLEAR(chipStateCtrl);

            stpStateCtrl.msti = i;
            stpStateCtrl.port = port;
            stpStateCtrl.proto = SYS_PORT_STATECTRLPROTO_STP;

            SYS_ERR_CONTINUE(rsd_port_state_get(&stpStateCtrl));

            chipStateCtrl.msti = i;
            chipStateCtrl.port = port;
            chipStateCtrl.proto = SYS_PORT_STATECTRLPROTO_STP;

            SYS_ERR_CONTINUE(rsd_port_state_get(&chipStateCtrl));

            TECH_PRINTF(fp, "msti %u port %u state %s (%s)\n",
                        i, port, strState[stpStateCtrl.state], strState[chipStateCtrl.state]);
        }
        TECH_PRINTF(fp, "\n");
    }

    SYS_ERR_CHK(_sal_tech_mstp_show(fp));

    return SYS_ERR_OK;
#else
    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_MEM_CLEAR(stpStateCtrl);
        SYS_MEM_CLEAR(chipStateCtrl);

        stpStateCtrl.port = port;
        stpStateCtrl.proto = SYS_PORT_STATECTRLPROTO_STP;

        SYS_ERR_CONTINUE(rsd_port_state_get(&stpStateCtrl));

        chipStateCtrl.port = port;
        chipStateCtrl.proto = SYS_PORT_STATECTRLPROTO_STP;

        SYS_ERR_CONTINUE(rsd_port_state_get(&chipStateCtrl));

        TECH_PRINTF(fp, "port %u state %s (%s)\n",
                    port, strState[stpStateCtrl.state], strState[chipStateCtrl.state]);
    }
    TECH_PRINTF(fp, "\n");

    SYS_ERR_CHK(_sal_tech_rstp_show(fp));
#endif

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_SYSLOG

static int32 _sal_tech_logging_show(FILE *fp)
{
    char buf[SYS_BUF1024_LEN] = "";
    uint32 i = 0;
    uint32 j = 0;
    sal_log_target_t target;
    sys_enable_t enable = DISABLED;

    uint32 ratelimitInterval = 0;
    sys_enable_t ratelimitEnable = DISABLED;

    SYS_ERR_CHK(sal_log_enable_get(&enable));
    SYS_ERR_CHK(sal_log_ratelimitEnable_get(&ratelimitEnable));
    SYS_ERR_CHK(sal_log_ratelimitInterval_get(&ratelimitInterval));

    TECH_PRINTF(fp, "\nLogging service is %s\n\n", text_enable[enable].text);

    TECH_PRINTF(fp, "Aggregation: %s\n", text_enable[ratelimitEnable].text);
    TECH_PRINTF(fp, "Aggregation aging time: %u sec\n\n", ratelimitInterval);

    for (i = LOG_LOCAL; i < LOG_LOCAL_MAX; i++)
    {
        j = 0;
        SYS_MEM_CLEAR(buf);
        SYS_MEM_CLEAR(target);

        SYS_ERR_CHK(sal_log_target_get(i, &target));

        switch (i)
        {
            case LOG_CONSOLE:
                j = osal_sprintf(buf, "Console Logging: ");
                break;
            case LOG_RAM:
                j = osal_sprintf(buf, "Buffer Logging : ");
                break;
            case LOG_FLASH:
                j = osal_sprintf(buf, "File Logging   : ");
                break;
        }

        if (ENABLED == target.enable)
            j += osal_sprintf(buf + j, "level %s", text_log_sev[target.severity].text);
        else
            j += osal_sprintf(buf + j, "%s", text_enable[target.enable].text);

        TECH_PRINTF(fp, "%s\n", buf);
    }

    TECH_PRINTF(fp, "\n");

    for (i = LOG_REMOTE; i < LOG_TARGET_MAX; i++)
    {
        SYS_MEM_CLEAR(buf);
        SYS_MEM_CLEAR(target);

        SYS_ERR_CHK(sal_log_target_get(i, &target));

        if (0 == osal_strlen(target.host))
            continue;

        TECH_PRINTF(fp, "Logging Server: %s, port %u, level %s, facility local%u\n",
               target.host, target.port, text_log_sev[target.severity].text,
               target.facility - LOG_FACILITY_LOCAL0);
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_JUMBO_FRAME
static int32 _sal_tech_jumbo_show(FILE *fp)
{
    uint32 maxFrameSzie = 0;

    SYS_ERR_CHK(sal_switch_maxFrameSize_get(&maxFrameSzie));

    TECH_PRINTF(fp, "jumbo-frame: %d\n", maxFrameSzie);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_UDLD
static int32 _sal_tech_udld_show(FILE *fp)
{
    sys_logic_portmask_t   logicPortmask;
    sys_logic_port_t       logicPort;
    sys_udld_bidir_state_t bidirState;
    sys_udld_op_state_t    operational_state;
    char                   port_str[8], adminStr[32], operStr[32], bidirStateStr[32], operStateStr[32];
    sys_udld_mode_t        portUdldMode;
    uint32  msgTime;
    uint16  nbrNum, nbrId = 0, echoId = 0;
    sys_udld_neighbor_info_t nbrInfo;
    sys_udld_echo_info_t echoInfo;

    NORMAL_LOGIC_PORTMASK_SET_ALL(logicPortmask);

    TECH_PRINTF(fp, "\n");

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        osal_memset(port_str, 0, sizeof(port_str));
        SYS_ERR_CHK(LP2STR(logicPort, port_str));

        TECH_PRINTF(fp, "Interface %s\n", port_str);
        TECH_PRINTF(fp, "---\n");

        SYS_ERR_CHK(sal_udld_portMode_get(logicPort, &portUdldMode));

        osal_strcpy(adminStr, text_udldMode[portUdldMode].text);
        TECH_PRINTF(fp, "Port enable administrative configuration setting: %s\n", adminStr);

        SYS_ERR_CHK(sal_udld_portOperMode_get(logicPort, &portUdldMode));
        osal_strcpy(operStr, text_udldMode[portUdldMode].text);
        TECH_PRINTF(fp, "Port enable operational state: %s\n", operStr);

        SYS_ERR_CHK(sal_udld_portBidiretionState_get(logicPort, &bidirState));
        osal_strcpy(bidirStateStr, text_udldBidirState[bidirState].text);
        TECH_PRINTF(fp, "Current bidirectional state: %s\n", bidirStateStr);

        if(UDLD_MODE_DISABLE != portUdldMode)
        {
            SYS_ERR_CHK(sal_udld_portOperState_get(logicPort, &operational_state));
            osal_strcpy(operStateStr, text_udldOperState[operational_state].text);
            TECH_PRINTF(fp, "Current operational state: %s\n", operStateStr);
            if(UDLD_BIDIR_BIDIRECTIONAL == bidirState)
            {
                SYS_ERR_CHK(sal_udld_messageTime_get(&msgTime));
                TECH_PRINTF(fp, "Message interval: %d\n", msgTime);
            }
            else
                TECH_PRINTF(fp, "Message interval: %d\n", 7);
            TECH_PRINTF(fp, "Time out interval: %d\n", 5);

            SYS_ERR_CHK(sal_udld_portNeighborNumber_get(logicPort, &nbrNum));

            if(nbrNum != 0)
            {
                for(nbrId = 0; nbrId < nbrNum; nbrId ++)
                {
                    SYS_ERR_CHK(sal_udld_portNeighbor_get(logicPort, nbrId, &nbrInfo));

                    if(TRUE == nbrInfo.valid)
                    {
                        TECH_PRINTF(fp, "\n\tEntry %d\n", nbrId + 1);
                        TECH_PRINTF(fp, "\t---\n");
                        TECH_PRINTF(fp, "\tExpiration time: %d\n", nbrInfo.hold_timer);
                        TECH_PRINTF(fp, "\tCurrent neighbor state: %s\n", text_udldBidirState[nbrInfo.status].text);
                        TECH_PRINTF(fp, "\tDevice ID  : %s\n", nbrInfo.device_id);
                        TECH_PRINTF(fp, "\tDevice name: %s\n", nbrInfo.deviceName);
                        TECH_PRINTF(fp, "\tPort ID: %s\n", nbrInfo.port_id);
                        TECH_PRINTF(fp, "\tMessage interval: %d\n", nbrInfo.messageInterval);
                        TECH_PRINTF(fp, "\tTime out interval: %d\n", nbrInfo.timeOutInterval);

                        if(nbrInfo.echoNum != 0)
                        {
                            for(echoId = 0; echoId < nbrInfo.echoNum; echoId ++)
                            {
                                SYS_ERR_CHK(sal_udld_portNeighborEcho_get(logicPort, nbrId, echoId, &echoInfo));
                                if(TRUE == echoInfo.valid)
                                {
                                    TECH_PRINTF(fp, "\tNeighbor echo %d device: %s\n", echoId+1, echoInfo.echo.echo_device_id);
                                    TECH_PRINTF(fp, "\tNeighbor echo %d port: %s\n\n", echoId+1, echoInfo.echo.echo_port_id);
                                }
                                else
                                    break;
                            }
                        }

                        if(0 == nbrInfo.echoNum || 0 == echoId)
                        {
                            TECH_PRINTF(fp, "\tNeighbor echo empty\n");
                        }

                    }
                    else
                        break;
                }

            }

            if(0 == nbrNum || 0 == nbrId)
            {
                TECH_PRINTF(fp, "No neighbor cache information stored\n");
            }

        }

        TECH_PRINTF(fp, "\n");
    }

    return SYS_ERR_OK;

}
#endif

const sal_tech_tbl_t salTechShowTbl[] =
{
    
    {_sal_tech_sys_info_show,             "System Information",     "system"},
    {_sal_tech_time_show,                 "Time",                   "time"},
    {_sal_tech_sys_lineUsers_show,        "User Status",            "users"},
    {_sal_tech_sys_startup_config_show,   "Startup-config",         ""},
    {_sal_tech_sys_running_config_show,   "Running-config",         ""},
    {_sal_tech_board_config_show,         "Board Configuration",    "board"},
    {_sal_tech_l3_intf_show,              "IP Configuration",       "ip"},
#ifdef CONFIG_SYS_ADVANCE_DNS
    {_sal_tech_advDns_show,               "ADV DNS",                    "advdns"},
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    {_sal_tech_static_route_show,         "Static Route",           "static-route"},
#endif
#ifdef CONFIG_SYS_PROTO_STP
    {_sal_tech_stp_show,                  "Spanning-Tree",          "spanning-tree"},
#endif
#ifdef CONFIG_SYS_SYSLOG
    {_sal_tech_logging_show,              "Logging",                "logging"},
#endif

#ifdef CONFIG_SYS_AAA
    {_sal_tech_aaa_show,                  "AAA",                    "aaa"},
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    {_sal_tech_authmgr_show,              "Authentication Manager", "authmgr"},
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    {_sal_tech_igmp_show,                 "IGMP Snooping",          "igmp-snooping"},
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    {_sal_tech_mld_show,                  "MLD Snooping",           "mld-snooping"},
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    {_sal_tech_lldp_show,                 "LLDP",                   "lldp"},
#endif
#ifdef CONFIG_SYS_PROTO_MVR
    {_sal_tech_mvr_show,                  "MVR",                    "mvr"},
#endif
    {_sal_tech_port_show,                 "Interfaces",             "interfaces"},
    {_sal_tech_port_status_show,          "Interfaces Status",      "interfaces-status"},
    {_sal_tech_l2_macAddressTable_show,   "MAC Address Table",      "mac-address-table"},
    {_sal_tech_l2_macAddressCounters_show,"MAC Address Counters",   "mac-address-counters"},
    {_sal_tech_acl_show,                  "ACL",                    "acl"},
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    {_sal_tech_user_acl_show,             "User-defined ACL",       "user-defined-acl"},
#endif
    {_sal_tech_vlan_show,                 "VLAN",                   "vlan"},
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {_sal_tech_proto_vlan_show,           "Protocol VLAN",          "protocol-vlan"},
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    {_sal_tech_voice_vlan_show,           "Voice VLAN",             "voice-vlan"},
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {_sal_tech_surveillance_vlan_show,    "Surveillance VLAN",      "surveillance-vlan"},
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
    {_sal_tech_port_isolation_show,       "Port Isolation",         "port-isolation"},
#endif
    {_sal_tech_trunk_show,                "LAG Summary",            "lag"},
#ifdef CONFIG_SYS_PORT_SECURITY
    {_sal_tech_psecure_show,              "Port Security",          "port-security"},
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
    {_sal_tech_dos_show,                  "DoS Protection",         "dos"},
#endif
#ifdef CONFIG_SYS_STORM_CTRL
    {_sal_tech_stormControl_show,         "Storm Control",          "storm-control"},
#endif
    {_sal_tech_qos_show,                  "QoS",                    "qos"},
    {_sal_tech_rate_show,                 "Rate Limit",             "rate-limit"},
#ifdef CONFIG_SYS_CABLE_DIAG
    {_sal_tech_cablediag_show,            "Cable Diagnostic",       "cable-diag"},
#endif
#ifdef CONFIG_SYS_MIRROR
    {_sal_tech_mirror_show,               "Mirror",                 "mirror"},
#endif
#ifdef CONFIG_SYS_FIBER
    {_sal_tech_optical_show,              "Optical Module",         "optical-module"},
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    {_sal_tech_rmon_show,                 "RMON",                   "rmon"},
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {_sal_tech_ds_show,                   "DHCP Snooping",          "dhcp-snooping"},
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    {_sal_tech_dai_show,                  "DAI",                    "dai"},
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    {_sal_tech_isg_show,                  "IP Source Guard",        "isg"},
#endif
#ifdef CONFIG_SYS_UI_SNMP
    {_sal_tech_snmp_show,                 "SNMP",                   "snmp"},
#endif
#ifdef CONFIG_SYS_APP_SSH
    {_sal_tech_ssh_show,                  "SSH",                    "ssh"},
#endif
#ifdef CONFIG_SYS_JUMBO_FRAME
    {_sal_tech_jumbo_show,                "Jumbo Frame",            "jumbo"},
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
    {_sal_tech_udld_show,                 "UDLD",                   "udld"},
#endif

    {_sal_tech_sys_memory_show,           "Process Memory",         "memory"},
    {_sal_tech_sys_processInfo_show,      NULL,                     "memory"},
    {_sal_tech_sys_cpu_process_show,      "Process CPU utilization",  "cpu"},
    {_sal_tech_sys_fs_show,               "File System",            "filesystem"},
    {_sal_tech_sys_mtd_show,              "MTD Information",        "mtd"},
    {_sal_tech_sys_cfgAnalysis_show,      "Config Analysis",        "config-analysis"},

    {_sal_tech_debug_glbDbgCnt_show,      "Mib Counters",           "mib-counters"},
    {_sal_tech_debug_glbCnt_show,         NULL,                     "mib-counters"},
    {_sal_tech_debug_portCnt_show,        NULL,                     "mib-counters"},
    {_sal_tech_diag_dbgInfo_show,         "Chip Information",       "chip"},
    {_sal_tech_diag_register_show,        "Table & Registers",      "register"},
    {NULL, NULL}
};

int32 sal_tech_fileLock_check(void)
{
    FILE *fp;

    fp = fopen(SYS_POLLD_ACT_WEB_TECH_LOCK, "r");

    if (NULL != fp)
    {
        fclose(fp);
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

int32 sal_tech_file_gen(char *pKey)
{
    uint32 i = 0;
    FILE *fp;

    SYS_PARAM_CHK((NULL == pKey), SYS_ERR_NULL_POINTER);

    if (NULL == (fp = fopen(CONF_TECH_SUPPORT_FILE, "w")))
    {
        return SYS_ERR_FILE_OPEN;
    }

    while (NULL != salTechShowTbl[i].showFunc)
    {
        if (0 == osal_strlen(pKey))
        {
            if (_sal_tech_diag_dbgInfo_show == salTechShowTbl[i].showFunc)
                sleep(1);

            if (_sal_tech_diag_register_show == salTechShowTbl[i].showFunc)
            {
                i++;
                continue;
            }

            if (NULL != salTechShowTbl[i].title)
                TECH_PRINTF(fp, "\n------------------ %s ------------------\n\n", salTechShowTbl[i].title);

            salTechShowTbl[i].showFunc(fp);

            TECH_PRINTF(fp, "\n");
        }
        else
        {
            if (0 == osal_strncmp(salTechShowTbl[i].key, pKey, osal_strlen(pKey)))
            {
                if (_sal_tech_diag_dbgInfo_show == salTechShowTbl[i].showFunc)
                    sleep(1);

                if (NULL != salTechShowTbl[i].title)
                    TECH_PRINTF(fp, "\n------------------ %s ------------------\n\n", salTechShowTbl[i].title);

                salTechShowTbl[i].showFunc(fp);

                TECH_PRINTF(fp, "\n");
            }
        }

        i++;
    }

    fclose(fp);

    return SYS_ERR_OK;
}

