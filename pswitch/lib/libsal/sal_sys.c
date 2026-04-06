
#include <dirent.h>
#include <libgen.h>
#include <mtd/mtd-user.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/reboot.h>
#include <image.h>
#include <libfds/fds_export.h>
#include <libsal/sal_util.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_l3.h>
#ifdef CONFIG_SYS_PROTO_LLDP
#include <libsal/sal_lldp.h>
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
#include <libsal/sal_dhcp_server.h>
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
#include <libsal/sal_easycwmp.h>
#endif
#include <rsd/rsd_board_device.h>

#define DHCPC_PID_FILE "/etc/dhcpc/dhcpcd-eth0.pid"

const sys_text_t text_tcp_state[] =
{
    { ""            },
    { "ESTABLISHED" },
    { "SYN_SENT" },
    { "SYN_RECV" },
    { "FIN_WAIT1" },
    { "FIN_WAIT2" },
    { "TIME_WAIT" },
    { "CLOSE" },
    { "CLOSE_WAIT" },
    { "LAST_ACK" },
    { "LISTEN" },
    { "CLOSING" }
};

int32 sal_sys_init(void)
{
    char serialnum[CAPA_SYS_SN_LEN];
    sys_sysinfo_t sysinfo;
    char sysModel[CAPA_SYS_NAME_LEN + 1];

    SYS_MEM_CLEAR(serialnum);
    SYS_MEM_CLEAR(sysinfo);
    SYS_MEM_CLEAR(sysModel);

	sal_sys_bdinfoVar_get("serialnumber", serialnum);

    sal_sys_bdinfoVar_get("modelname", sysModel);

    osal_strcpy(sysinfo.description, sysModel);
    osal_strcpy(sysinfo.serialnum, serialnum);
    osal_strcpy(sysinfo.versionNum, DFLT_SYS_VER);
#ifdef CONFIG_SYS_APP_EASYCWMP
	sal_easycwmp_swVersion_set(sysinfo.versionNum);
	sal_easycwmp_sn_set(serialnum);
	sal_easycwmp_modelName_set(sysinfo.description);
#endif
    SYS_ERR_CHK(cfg_sys_info_set(&sysinfo));

    SYS_ERR_CHK(rsd_sys_info_set(&sysinfo));

    return SYS_ERR_OK;
}

int32 sal_sys_devGeComboPortNum_get(uint32 unit, uint32 *pPortnum)
{
    SYS_PARAM_CHK((NULL == pPortnum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_devGeComboPortNum_get(unit, pPortnum));

    return SYS_ERR_OK;
}

int32 sal_sys_devGeComboMinPortNum_get(uint32 unit, uint32 *pPortnum)
{
    SYS_PARAM_CHK((NULL == pPortnum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_devGeComboMinPortNum_get(unit, pPortnum));

    return SYS_ERR_OK;
}

int32 sal_sys_boardModel_get(sys_boardModel_t *pBoardModel)
{
    SYS_PARAM_CHK((NULL == pBoardModel), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_boardModel_get(pBoardModel));

    return SYS_ERR_OK;
}

int32 sal_sys_boardConf_get(sys_board_conf_t *pBoardConf)
{
    SYS_PARAM_CHK((NULL == pBoardConf), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_boardConf_get(pBoardConf));

    return SYS_ERR_OK;
}

int32 sal_sys_boardModelId_get(sys_boardModel_Id_t *pBoardId)
{
    SYS_PARAM_CHK((NULL == pBoardId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_boardModelId_get(pBoardId));

    return SYS_ERR_OK;
}

int32 sal_sys_boardModelName_get(char *pBoardName)
{
    SYS_PARAM_CHK((NULL == pBoardName), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_boardModelName_get(pBoardName));

    return SYS_ERR_OK;
}

int32 sal_sys_boardFlshLayout_get(sys_flsh_layout_t *pFlshLayout)
{
    SYS_PARAM_CHK((NULL == pFlshLayout), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_boardFlshLayout_get(pFlshLayout));

    return SYS_ERR_OK;
}

int32 sal_sys_boardPortConf_get(sys_logic_port_t port, sys_port_conf_t *pPortConf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPortConf), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_boardPortConf_get(port, pPortConf));

    return SYS_ERR_OK;
}

uint32 sal_sys_boardBootmsgLevel_ret(void)
{
    sys_boardModel_t boardModel;

    SYS_MEM_CLEAR(boardModel);

    cfg_sys_boardModel_get(&boardModel);

    return boardModel.bootmsg;
}

int32 sal_sys_currSysCompiledTime_set(void)
{
    uint32 currSelPart = 0;
    uint32 magic = 0;
    uint32 time = 0;
    uint32 size = 0;
    char name[SYS_BUF32_LEN];

#ifdef CONFIG_SYS_DUAL_IMAGE
    sal_sys_dualImageCurrSel_get(&currSelPart);
    cfg_sys_currActiveImageIdx_set(currSelPart);
#endif
    sal_sys_fwImageHdrInfo_get(currSelPart, &magic, &time, &size, name);

    cfg_sys_currSysCompiledTime_set(time);

    return SYS_ERR_OK;
}

int32 sal_sys_currSysCompiledTimeStr_get(char *timestr)
{
    struct tm *tm_time;
    time_t timefmt;
    uint32 time = 0;

    SYS_PARAM_CHK((NULL == timestr), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_currSysCompiledTime_get(&time));

    timefmt = (time_t) ntohl(time);
    tm_time = gmtime(&timefmt);
    strftime(timestr, SYS_BUF64_LEN, "%b %d %G - %T", tm_time);

    return SYS_ERR_OK;
}

int32 sal_sys_currActiveImageIdx_get(uint32 *pIdx)
{
    SYS_PARAM_CHK((NULL == pIdx), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_currActiveImageIdx_get(pIdx));

    return SYS_ERR_OK;
}

int32 sal_sys_sysinfo_get(sys_sysinfo_t *pSysinfo)
{
    SYS_PARAM_CHK((NULL == pSysinfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_info_get(pSysinfo));

    return SYS_ERR_OK;
}

int32 sal_sys_systemName_set(char *pName)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    SYS_MEM_CLEAR(sysinfo.name);

    osal_strcpy(sysinfo.name, pName);

    SYS_ERR_CHK(rsd_sys_info_set(&sysinfo));

    SYS_ERR_CHK(cfg_sys_info_set(&sysinfo));

#ifdef CONFIG_SYS_DHCP_SERVER
	SYS_ERR_CHK(ipc_dhcp_hostname_set(pName));
#endif
    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_NAME,
            pName);

    return SYS_ERR_OK;
}

int32 sal_sys_systemName_get(char *pName)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(pName, sysinfo.name);

    return SYS_ERR_OK;
}

int32 sal_sys_systemLocation_set(char *pLocation)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pLocation), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(osal_strlen(pLocation) > CAPA_SYS_LOCATION_LEN, SYS_ERR_STR_LEN_EXCEED);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_memset(sysinfo.location, 0, sizeof(sysinfo.location));

    osal_strcpy(sysinfo.location, pLocation);

    SYS_ERR_CHK(rsd_sys_info_set(&sysinfo));

    SYS_ERR_CHK(cfg_sys_info_set(&sysinfo));

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_LOCATION,
            pLocation);

    return SYS_ERR_OK;
}

int32 sal_sys_systemLocation_get(char *pLocation)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pLocation), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(pLocation, sysinfo.location);

    return SYS_ERR_OK;
}

int32 sal_sys_systemContact_set(char *pContact)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pContact), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(osal_strlen(pContact) > CAPA_SYS_CONTACT_LEN, SYS_ERR_STR_LEN_EXCEED);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_memset(sysinfo.contact, 0, sizeof(sysinfo.contact));

    osal_strcpy(sysinfo.contact, pContact);

    SYS_ERR_CHK(rsd_sys_info_set(&sysinfo));

    SYS_ERR_CHK(cfg_sys_info_set(&sysinfo));

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_CONTACT,
            pContact);

    return SYS_ERR_OK;
}

int32 sal_sys_systemContact_get(char *pContact)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pContact), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(pContact, sysinfo.contact);

    return SYS_ERR_OK;
}

int32 sal_sys_system_softwareVersion_get(char *swVersion)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == swVersion), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(swVersion, sysinfo.versionNum);

    return SYS_ERR_OK;
}

int32 sal_sys_macAddr_get(sys_mac_t *pMac)
{
    uint32 unit = SYS_DEV_ID_DEFAULT; 

    SYS_PARAM_CHK((NULL == pMac), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_sys_macAddr_get(unit, pMac));

    return SYS_ERR_OK;
}

int32 sal_sys_portMacAddr_get(sys_logic_port_t port, sys_mac_t *pMac)
{
    SYS_PARAM_CHK((NULL == pMac), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_sys_portMacAddr_get(port, pMac));

    return SYS_ERR_OK;
}

int32 sal_sys_ip_get(uint32 *pIp, uint32 *pNetmask)
{
#ifdef CONFIG_SYS_L3_ROUTE
    int32 fd;
    struct ifreq ifr;
    char devName[SYS_BUF16_LEN];

    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pNetmask), SYS_ERR_NULL_POINTER);

    *pIp = 0;
    *pNetmask = 0;

    {
        sys_l3_intfId_t intfId;

        SYS_MEM_CLEAR(intfId);

        intfId.type = SYS_L3_INTFTYPE_VLAN;
#ifdef CONFIG_SYS_L3_ARCHITECTURE
		sal_vlan_defaultVid_get(&intfId.id);
#else
        sal_vlan_mgmtVid_get(&intfId.id);
#endif

        if (IS_L3_INTF_EXIST(&intfId))
        {
            INTFID2STR(&intfId, devName);
        }
        else
        {
            intfId.type = SYS_L3_INTFTYPE_MGMTVLAN;

            INTFID2STR(&intfId, devName);
        }

        INTFID2STR(&intfId, devName);
    }

    if ((fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        return SYS_ERR_SOCKET;
    }

    osal_strcpy(ifr.ifr_name, devName);
    if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0)
    {
        close(fd);

        return SYS_ERR_IOCTL;
    }

    if (ifr.ifr_flags & (IFF_UP |IFF_RUNNING))
    {
        ifr.ifr_addr.sa_family = AF_INET;

        if (0 == ioctl(fd, SIOCGIFADDR, &ifr))
        {
            *pIp = htonl(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr);
        }

        if (ioctl(fd, SIOCGIFNETMASK, &ifr) >= 0)
        {
            *pNetmask = htonl(((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr.s_addr);
        }
    }

    close(fd);
#else
    sys_l3_intfMgmt_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);

    sal_l3_intfMgmt_get(&intfMgmt);

    *pIp = intfMgmt.ipv4.addr;
    *pNetmask = IPV4_PFX2MASK(intfMgmt.ipv4.plen);
#endif

    return SYS_ERR_OK;
}

int32 sal_sys_system_logo_set(char *pLoginLogo, char *pHomeLogo)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pLoginLogo), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == pHomeLogo), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));
	osal_strcpy(sysinfo.loginLogoName, pLoginLogo);
	osal_strcpy(sysinfo.homeLogoName, pHomeLogo);
    SYS_ERR_CHK(cfg_sys_info_set(&sysinfo));

    return SYS_ERR_OK;
}

int32 sal_sys_system_home_logo_get(char *pName)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(pName, sysinfo.homeLogoName);

    return SYS_ERR_OK;
}

int32 sal_sys_system_login_logo_get(char *pName)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(pName, sysinfo.loginLogoName);

    return SYS_ERR_OK;
}

int32 sal_sys_title_set(char *pTitle)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pTitle), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));
	osal_strcpy(sysinfo.title, pTitle);
    SYS_ERR_CHK(cfg_sys_info_set(&sysinfo));

    return SYS_ERR_OK;
}

int32 sal_sys_title_get(char *pName)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(pName, sysinfo.title);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_ADVANCE_DNS
int32 _sal_sys_dnsd_stop(void)
{
    unlink(SYS_DNS_RESOLV_FILE);
    unlink(SYS_DNS_HOSTS_FILE);

    return SYS_ERR_OK;
}

int32 _sal_sys_dnsd_start(void)
{
    SYS_ERR_CHK(sal_sys_dnsResolvConf_create());
    SYS_ERR_CHK(sal_sys_dnsHosts_create());

    return SYS_ERR_OK;
}

int32 sal_sys_dnsLookup_set(sys_enable_t enable)
{
    sys_enable_t now_enable  = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_dnsEnable_get(&now_enable));

    if (now_enable == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(cfg_sys_dnsEnable_set(enable));
    SYS_ERR_CHK(rsd_dns_enable_set(enable));

    if (ENABLED == enable)
        _sal_sys_dnsd_start();
    else
        _sal_sys_dnsd_stop();

    return SYS_ERR_OK;
}

int32 sal_sys_dnsLookup_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_dnsEnable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_sys_dnsDomain_set(char *pDoamin, sys_advDns_source_t src)
{
    SYS_PARAM_CHK(NULL == pDoamin, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(src >= SYS_ADVDNS_SRC_END, SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_dnsDomain_set(pDoamin, src));

    return SYS_ERR_OK;
}

int32 sal_sys_dnsDomain_get(char *pDoamin, sys_advDns_source_t src)
{
    SYS_PARAM_CHK(NULL == pDoamin, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(src >= SYS_ADVDNS_SRC_END, SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_dnsDomain_get(pDoamin, src));

    return SYS_ERR_OK;
}

int32 sal_sys_dnsNameServer_set(uint8 idx, sys_advDnsIP_t *pServer, sys_advDns_source_t src)
{
    SYS_PARAM_CHK(idx >= CAPA_SYS_DNS_NAMESERVER_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pServer, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(src >= SYS_ADVDNS_SRC_END, SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_dnsNameServer_set(idx, pServer, src));

    return SYS_ERR_OK;
}

int32 sal_sys_dnsNameServer_get(uint8 idx, sys_advDnsIP_t *pServer, sys_advDns_source_t src)
{
    SYS_PARAM_CHK(idx >= CAPA_SYS_DNS_NAMESERVER_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pServer, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(src >= SYS_ADVDNS_SRC_END, SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_dnsNameServer_get(idx, pServer, src));

    return SYS_ERR_OK;
}

int32 sal_sys_dnsNameServer_del(sys_advDnsIP_t *pServer)
{
    uint8 i = 0;
    sys_advDnsIP_t ip;

    SYS_PARAM_CHK(NULL == pServer, SYS_ERR_NULL_POINTER);

    for (i = 0 ; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
    {
        SYS_MEM_CLEAR(ip);
        SYS_ERR_CHK(cfg_sys_dnsNameServer_get(i, &ip, SYS_ADVDNS_SRC_STATIC));

        if (0 != osal_strlen(ip.ip))
        {
            if(0 == osal_strcmp(ip.ip, pServer->ip))
            {
                SYS_MEM_CLEAR(ip);
                SYS_ERR_CHK(cfg_sys_dnsNameServer_set(i, &ip, SYS_ADVDNS_SRC_STATIC));

                return SYS_ERR_OK;
            }
        }
    }

    return SYS_ERR_ENTRY_NOT_FOUND;
}

int32 sal_sys_dnsStaticHost_set(sys_advDns_staticHost_t *pHost)
{
    uint8 i = 0;
    uint8 emptyIdx = CAPA_SYS_DNS_STATIC_HOST_NUM;
    sys_advDns_staticHost_t host;

    SYS_PARAM_CHK(NULL == pHost, SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SYS_DNS_STATIC_HOST_NUM ; i++)
    {
        SYS_MEM_CLEAR(host);
        SYS_ERR_CHK(sal_sys_dnsStaticHost_get(i, &host));

        if (CAPA_SYS_DNS_STATIC_HOST_NUM == emptyIdx && 0 == osal_strlen(host.host))
            emptyIdx = i;

        if (0 == osal_strcmp(host.host, pHost->host))
        {
            SYS_ERR_CHK(cfg_sys_dnsStaticHost_set(i, pHost));

            return SYS_ERR_OK;
        }
    }

    if (emptyIdx != CAPA_SYS_DNS_STATIC_HOST_NUM)
    {
        SYS_ERR_CHK(cfg_sys_dnsStaticHost_set(emptyIdx, pHost));

        return SYS_ERR_OK;
    }

    return SYS_ERR_ENTRY_FULL;
}

int32 sal_sys_dnsStaticHost_get(uint8 idx, sys_advDns_staticHost_t *pHost)
{
    SYS_PARAM_CHK(idx >= CAPA_SYS_DNS_STATIC_HOST_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pHost, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_dnsStaticHost_get(idx, pHost));

    return SYS_ERR_OK;
}

int32 sal_sys_dnsStaticIpByHost_get(char *pHost, char *pIp)
{
    uint32 i = 0;
    sys_advDns_staticHost_t host;

    SYS_PARAM_CHK(NULL == pIp, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == pHost, SYS_ERR_NULL_POINTER);

    for (i = 0 ; i < CAPA_SYS_DNS_STATIC_HOST_NUM ; i++)
    {
        SYS_MEM_CLEAR(host);
        SYS_ERR_CHK(sal_sys_dnsStaticHost_get(i, &host));

        if (0 == osal_strncmp(host.host, pHost, strlen(pHost)))
        {
            osal_strcpy(pIp, host.ip.ip);
            break;
        }
    }

    return SYS_ERR_OK;
}

int32 sal_sys_dnsStaticHost_del(sys_advDns_staticHost_t *pHost)
{
    uint8 i = 0;
    sys_advDns_staticHost_t host;

    SYS_PARAM_CHK(NULL == pHost, SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SYS_DNS_STATIC_HOST_NUM ; i++)
    {
        SYS_MEM_CLEAR(host);
        SYS_ERR_CHK(sal_sys_dnsStaticHost_get(i, &host));

        if (0 == osal_strcmp(host.host, pHost->host))
        {
            SYS_MEM_CLEAR(host);
            SYS_ERR_CHK(cfg_sys_dnsStaticHost_set(i, &host));

            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_ENTRY_NOT_FOUND;
}

int32 sal_sys_dnsResolvConf_create(void)
{
    FILE *fp = NULL;
    uint8 i = 0;
    uint8 j = 0;
    char domain[SYS_BUF256_LEN];
    sys_enable_t enable = DISABLED;
    sys_advDnsIP_t ns;

    SYS_ERR_CHK(sal_sys_dnsLookup_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (NULL == (fp = fopen(SYS_DNS_RESOLV_FILE, "w")))
        return SYS_ERR_FILE_OPEN;

    for (j = 0; j < SYS_ADVDNS_SRC_END ; j++)
    {
        for (i = 0; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
        {
            SYS_MEM_CLEAR(ns);
            sal_sys_dnsNameServer_get(i, &ns, j);

            if (0 != osal_strlen(ns.ip))
                fprintf(fp, "nameserver %s\n", ns.ip);
        }
    }

    fprintf(fp, "search ");

    for (j = 0; j < SYS_ADVDNS_SRC_END ; j++)
    {
        SYS_MEM_CLEAR(domain);
        sal_sys_dnsDomain_get(domain, j);

        if (0 != strlen(domain))
            fprintf(fp, "%s ", domain);
    }

    fprintf(fp, "\n");
    fclose(fp);

    return SYS_ERR_OK;
}

int32 sal_sys_dnsHosts_create(void)
{
    FILE *fp = NULL;
    FILE *pfp = NULL;
    uint8 i = 0;
    char tmp[SYS_BUF512_LEN];
    char buf[SYS_BUF512_LEN];
    char *ip = NULL;
    char *hostname = NULL;
    sys_advDns_staticHost_t host;
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_sys_dnsLookup_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (NULL == (fp = fopen(SYS_DNS_HOSTS_FILE, "w")))
        return SYS_ERR_FILE_OPEN;

    for (i = 0 ; i < CAPA_SYS_DNS_STATIC_HOST_NUM ; i++)
    {
        SYS_MEM_CLEAR(host);

        sal_sys_dnsStaticHost_get(i, &host);

        if (0 != osal_strlen(host.host))
        {
            SYS_MEM_CLEAR(tmp);
            osal_sprintf(tmp, "%s %s\n",host.ip.ip, host.host);
            fprintf(fp, tmp);
        }
    }

    if (NULL != (pfp = popen("cat /proc/"SYS_DNS_PROC_FILE, "r")))
    {
        while (fgets(buf, sizeof(buf), pfp))
        {
            ip = strtok(buf, ", ");

            if ('Z' == ip[0]) 
                break;

            hostname = strtok(NULL, ", ");

            SYS_MEM_CLEAR(tmp);
            osal_sprintf(tmp, "%s %s\n",ip, hostname);
            fprintf(fp, tmp);
        }

        pclose(pfp);
    }

    fclose(fp);

    return SYS_ERR_OK;
}

int32 sal_sys_dnsDhcpConf_refresh(void)
{
    FILE *fp = NULL;
    uint8 i = 0;
    char *pTmp = NULL;
    char buf[SYS_BUF256_LEN];
    sys_advDnsIP_t ns;

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(ns);

    SYS_ERR_CHK(sal_sys_dnsDomain_set(buf, SYS_ADVDNS_SRC_DHCPV4));

    for (i = 0 ; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
        SYS_ERR_CHK(sal_sys_dnsNameServer_set(i, &ns, SYS_ADVDNS_SRC_DHCPV4));

    {
        sys_l3_intfMgmtAdmin_t adminMgmt;
        SYS_MEM_CLEAR(adminMgmt);

        SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&adminMgmt));

        if (SYS_L3_DHCPTYPE_DISABLE == adminMgmt.v4.dhcpType)
        {
            SYS_ERR_CHK(sal_sys_dnsResolvConf_create());

            return SYS_ERR_OK;
        }
    }

    if (NULL == (fp = fopen(SYS_DNS_RESOLV_FILE_DHCPCD, "r")))
        return SYS_ERR_OK;

    i = 0;

    while (fgets(buf, sizeof(buf), fp))
    {
        buf[strlen(buf)-1] = 0;

        if ('n' == buf[0])
        {
            
            if (i >= 4)
                continue;

            pTmp = rindex(buf, ' ');
            pTmp++;

            if (0 == osal_strcmp("0.0.0.0", pTmp))
                continue;

            SYS_MEM_CLEAR(ns);
            ns.ipVer = 4;
            osal_strcpy(ns.ip, pTmp);

            if (IS_IPV4_ADDRSTR_VALID(ns.ip))
            {
                sal_sys_dnsNameServer_set(i, &ns, SYS_ADVDNS_SRC_DHCPV4);
                i++;
            }
        }
        else if ('s' == buf[0])
        {
            pTmp = rindex(buf, ' ');
            pTmp++;

            if (IS_HOSTNAME_VALID(pTmp))
            {
                sal_sys_dnsDomain_set(pTmp, SYS_ADVDNS_SRC_DHCPV4);
            }
        }
    }

    fclose(fp);

    SYS_ERR_CHK(sal_sys_dnsResolvConf_create());

    return SYS_ERR_OK;
}

int32 sal_sys_dnsDhcpv6Conf_refresh(void)
{
    FILE *fp = NULL;
    uint8 i = 0;
    char *pTmp = NULL;
    char buf[SYS_BUF256_LEN];
    sys_advDnsIP_t ns;

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(ns);

    SYS_ERR_CHK(sal_sys_dnsDomain_set(buf, SYS_ADVDNS_SRC_DHCPV6));

    for (i = 0 ; i < CAPA_SYS_DNS_NAMESERVER_NUM ; i++)
        SYS_ERR_CHK(sal_sys_dnsNameServer_set(i, &ns, SYS_ADVDNS_SRC_DHCPV6));

    if (NULL == (fp = fopen(SYS_DNS_RESOLV_FILE_DHCPV6, "r")))
        return SYS_ERR_OK;

    i = 0;

    while (fgets(buf, sizeof(buf), fp))
    {
        buf[strlen(buf)-1] = 0;

        if ('n' == buf[0])
        {
            
            if (i >= 4)
                continue;

            pTmp = rindex(buf, ' ');
            pTmp++;

            SYS_MEM_CLEAR(ns);
            ns.ipVer = 6;
            osal_strcpy(ns.ip, pTmp);

            if (IS_IPV6_ADDRSTR_VALID(ns.ip))
            {
                sal_sys_dnsNameServer_set(i, &ns, SYS_ADVDNS_SRC_DHCPV6);
                i++;
            }
        }
        else if ('s' == buf[0])
        {
            pTmp = rindex(buf, ' ');
            pTmp++;
            *(pTmp + strlen(pTmp) -1) = 0;

            if (IS_HOSTNAME_VALID(pTmp))
                sal_sys_dnsDomain_set(pTmp, SYS_ADVDNS_SRC_DHCPV6);
        }
    }

    fclose(fp);

    SYS_ERR_CHK(sal_sys_dnsResolvConf_create());

    return SYS_ERR_OK;
}

int32 sal_dynamic_host_clear(void)
{
    SYS_ERR_CHK(rsd_dns_clear_set());

    return SYS_ERR_OK;
}

#endif

int32 sal_sys_reboot(void)
{
    char cmd[64];
    sys_logic_port_t port_id;
    sys_boardModel_Id_t boardModelId;
    sys_enable_t enable;
    sys_port_media_t media;

    SYS_MEM_CLEAR(boardModelId);
    SYS_ERR_CHK(sal_sys_boardModelId_get(&boardModelId));

#ifdef CONFIG_SYS_BOARD_RTC
	system("handler -u sysToRtc &");
#endif

    FOR_EACH_NORMAL_LOGIC_PORT(port_id)
    {
        
        if(IS_COMBO_PORT(port_id))
        {
            enable = DISABLED;
            media = PORT_MEDIA_COPPER;
            rsd_port_media_set(port_id, media);
            sal_port_enable_set(port_id, enable);

            media = PORT_MEDIA_FIBER;
            rsd_port_media_set(port_id, media);
            sal_port_enable_set(port_id, enable);
        }
        else
        {
            enable = DISABLED;
            sal_port_enable_set(port_id, enable);
        }
    }

    sleep(1);

    if (SYS_ERR_OK == sal_util_file_exist(LOG_FILE_FLASH_TMP))
    {
        osal_memset(cmd, 0 , sizeof(cmd));
        osal_sprintf(cmd, "cp %s %s", LOG_FILE_FLASH_TMP, LOG_FILE_FLASH);
        system(cmd);
    }

    sal_util_file_touch(SYS_SYSTEM_RESTART_FLAG);

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_REBOOT);
    sleep(2);
    
    system("killall -q syslogd > /dev/null 2>&1");
    sleep(2);

    signal(SIGTERM, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    setpgrp();

    reboot(RB_ENABLE_CAD);
    sync();

#ifdef CONFIG_SYS_KERNEL_LINUX_2_6_X
    kill(-1, SIGKILL);
    sleep(1);
    sync();
#endif

    reboot(RB_AUTOBOOT);

	kill(-1, SIGTERM);
	sleep(1);
	sync();

    exit(0); 
}

int32 sal_sys_restore_defaults(sys_enable_t isFactory)
{
    char cmd[SYS_BUF128_LEN];

    if (DISABLED == isFactory)
    {
        
        unlink(CONF_STARTUP_FILE);

        unlink(CONF_BAKPART_STARTUP_FILE);

        osal_memset(cmd, 0, sizeof(cmd));
        osal_sprintf(cmd, "rm -rf %s", SYS_SEC_FLH_KEY_DIR);
        system(cmd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_RESTORE);
    }
    else
    {
#ifdef CONFIG_SYS_DUAL_IMAGE
        osal_memset(cmd, 0, sizeof(cmd));
        osal_snprintf(cmd, sizeof(cmd), "cp %s* /tmp/", FLH_FWIMG_FILENAME);
        system(cmd);
#endif

        osal_memset(cmd, 0, sizeof(cmd));
        osal_sprintf(cmd, "rm -rf %s/*", JFFS2_CFG_TARGET_DIR);
        system(cmd);

        osal_memset(cmd, 0, sizeof(cmd));
        osal_sprintf(cmd, "rm -rf %s/*", JFFS2_LOG_TARGET_DIR);
        system(cmd);

#ifdef CONFIG_SYS_DUAL_IMAGE
        osal_memset(cmd, 0, sizeof(cmd));
        osal_snprintf(cmd, sizeof(cmd), "cp /tmp/%s* /mnt/", FLH_FWIMG_FILENAME);
        system(cmd);
#endif

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FACTORY_RESTORE);
    }

    return SYS_ERR_OK;
}

int32 sal_sys_htpasswd_create(void)
{
    char crypt_password[CAPA_SYS_CRYPT_PASSWORD_LEN];
    char line[CAPA_SYS_USERNAME_LEN + CAPA_SYS_CRYPT_PASSWORD_LEN + 3];
    FILE *fp = NULL;
    uint32 i;
    sys_userInfo_t userInfo;
    int32 ret = SYS_ERR_OK;

    osal_memset(crypt_password, 0, sizeof(crypt_password));
    osal_memset(line, 0, sizeof(line));

    if (NULL == (fp = fopen(SAL_SYS_HTPASSWD_FILE, "w")))
        return SYS_ERR_FILE_OPEN;

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        osal_memset(&userInfo, 0, sizeof(userInfo));

        ret = cfg_sys_localUser_get(i, &userInfo);
        if (SYS_ERR_OK != ret)
        {
            fclose(fp);
            return ret;
        }

        if (userInfo.priv < 0)
            continue;

        if (SYS_PASSWD_TYPE_ENCRYPT == userInfo.passwdType)
        {
            osal_sprintf(line, "%s:%s:%d\n", userInfo.username, userInfo.password, userInfo.priv);
        }
        else
        {
            ret = sal_util_password_oldcrypt(userInfo.password, NULL, crypt_password);
            if (SYS_ERR_OK != ret)
            {
                fclose(fp);
                return ret;
            }
            osal_sprintf(line, "%s:%s:%d\n", userInfo.username, crypt_password, userInfo.priv);
        }

        fwrite(line, 1, osal_strlen(line), fp);
    }

    fclose(fp);

    return SYS_ERR_OK;
}

 int32 sal_sys_passwd_create(void)
{
    char buf[2048];
    char crypt_password[CAPA_SYS_CRYPT_PASSWORD_LEN + 1] = {0};
    char line[256];
    char gcosfield[] = "root";
    char homedir[] = "/";
    char loginshell[] = "";
    char *c = NULL;
    char *p = buf;
    FILE *fp;
    uint32 groupid = 0;
    uint32 userid = 0;
    uint32 filesize;
    uint32 i;
    sys_userInfo_t userInfo;

    osal_memset(buf, 0, sizeof(buf));
    osal_memset(crypt_password, 0, sizeof(crypt_password));
    osal_memset(line, 0, sizeof(line));

    if (NULL == (fp = fopen(SAL_SYS_PASSWD_FILE, "r")))
        return SYS_ERR_FILE_OPEN;

    if (0 != fseek(fp, 0, SEEK_END))
    {
        fclose(fp);

        return SYS_ERR_FILE_SEEK;
    }

    filesize = ftell(fp);

    if (filesize <= 0)
    {
        fclose(fp);

        return SYS_ERR_FILE_SIZE;
    }

    if (sizeof(buf) < filesize)
    {
        fclose(fp);

        return SYS_ERR_FAILED;
    }

    if (0 != fseek(fp, 0L, SEEK_SET))
    {
        fclose(fp);

        return SYS_ERR_FILE_SEEK;
    }

    if (fread(buf, 1, filesize, fp) != filesize)
    {
        fclose(fp);

        return SYS_ERR_FILE_READ;
    }

    fclose(fp);

    c = osal_strstr(buf, "\n");
#ifdef CONFIG_USER_SSH_SSHD
    c = osal_strstr(c + 1, "\n");
#endif
    if (NULL != c)
        buf[c - p + 1] = '\0';

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        osal_memset(&userInfo, 0, sizeof(userInfo));

        SYS_ERR_CHK(cfg_sys_localUser_get(i, &userInfo));

        if (userInfo.priv < 0)
            continue;

        if (SYS_PASSWD_TYPE_ENCRYPT == userInfo.passwdType)
        {
            osal_sprintf(line, "%s:%s:%d:%d:%s:%s:%s\n", userInfo.username,
                                                        userInfo.password,
                                                        userid,groupid,
                                                        gcosfield,
                                                        homedir,
                                                        loginshell);
        }
        else
        {
            SYS_ERR_CHK(sal_util_password_encrypt(userInfo.password, NULL, crypt_password));
            osal_sprintf(line, "%s:%s:%d:%d:%s:%s:%s\n", userInfo.username,
                                                        crypt_password,
                                                        userid,
                                                        groupid,
                                                        gcosfield,
                                                        homedir,
                                                        loginshell);
        }

        osal_strcat(buf + osal_strlen(buf), line);
     }

    if (NULL == (fp = fopen(SAL_SYS_PASSWD_FILE, "w")))
        return SYS_ERR_FILE_OPEN;

    fwrite(buf, 1, osal_strlen(buf), fp);
    fclose(fp);

    return SYS_ERR_OK;

}

int32 sal_sys_jiffies_get(uint32 *pJiffies)
{
    SYS_PARAM_CHK((NULL == pJiffies), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_sys_jiffies_get(pJiffies));

    return SYS_ERR_OK;
}

int32 sal_sys_hz_get(int32 *pHz)
{
    SYS_PARAM_CHK((NULL == pHz), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_sys_hz_get(pHz));

    return SYS_ERR_OK;
}

int32 sal_sys_cpuFreq_get(uint32 *pFreq)
{
    SYS_PARAM_CHK((NULL == pFreq), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_sys_cpuFreq_get(pFreq));

    return SYS_ERR_OK;
}

int32 sal_sys_serviceSessionProc_parse(char *line, sys_session_info_t *pInfo)
{
    char local_addr[SYS_BUF64_LEN];
    char remote_addr[SYS_BUF64_LEN];
    int32 d;
    int32 local_port;
    int32 remote_port;
    int32 state;
    struct in6_addr in6;

    SYS_PARAM_CHK((NULL == line), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(local_addr);
    SYS_MEM_CLEAR(remote_addr);
    osal_sscanf(line, "%d: %64[0-9A-Fa-f]:%X %64[0-9A-Fa-f]:%X %X %*s:%*s %*s:%*s %*s %*s %*s %*s %*s\n",
                     &d,
                     local_addr, &local_port,
                     remote_addr, &remote_port,
                     &state);

    if (0 == osal_strcmp(pInfo->type, "tcp"))
        osal_strcpy(pInfo->tcpState, text_tcp_state[state].text);

    osal_sscanf(local_addr, "%08X%08X%08X%08X", &in6.s6_addr32[0], &in6.s6_addr32[1], &in6.s6_addr32[2], &in6.s6_addr32[3]);
    if ((0 == in6.s6_addr32[0]) && (0 == in6.s6_addr32[1]) && (0 == in6.s6_addr32[2]) && (0 == in6.s6_addr32[3]))
    {
        osal_strcpy(pInfo->localIp, "*");
    }
    else if ((0 == in6.s6_addr32[0]) && (0 == in6.s6_addr32[1]) && (0xFFFF == in6.s6_addr32[2]))
    {
        sys_util_ip2Str(in6.s6_addr32[3], pInfo->localIp);
    }
    else
    {
        inet_ntop(AF_INET6, &in6, pInfo->localIp, CAPA_IPV6_STR_LEN);
        pInfo->isIPv6 = TRUE;
        osal_strcat(pInfo->type, "6");
    }

    if (0 == local_port)
        osal_strcpy(pInfo->localPort, "*");
    else
        osal_sprintf(pInfo->localPort, "%d", local_port);

    osal_sscanf(remote_addr, "%08X%08X%08X%08X", &in6.s6_addr32[0], &in6.s6_addr32[1], &in6.s6_addr32[2], &in6.s6_addr32[3]);
    if ((0 == in6.s6_addr32[0]) && (0 == in6.s6_addr32[1]) && (0 == in6.s6_addr32[2]) && (0 == in6.s6_addr32[3]))
    {
        osal_strcpy(pInfo->remoteIp, "*");
    }
    else if ((0 == in6.s6_addr32[0]) && (0 == in6.s6_addr32[1]) && (0xFFFF == in6.s6_addr32[2]))
    {
        sys_util_ip2Str(in6.s6_addr32[3], pInfo->remoteIp);
    }
    else
    {
        inet_ntop(AF_INET6, &in6, pInfo->remoteIp, CAPA_IPV6_STR_LEN);
    }

    if (0 == remote_port)
        osal_strcpy(pInfo->remotePort, "*");
    else
        osal_sprintf(pInfo->remotePort, "%d", remote_port);

    switch(local_port)
    {
        case 22:
            osal_strcpy(pInfo->name, "ssh");
            break;

        case 23:
            osal_strcpy(pInfo->name, "telnet");
            break;

        case 80:
            osal_strcpy(pInfo->name, "http");
            break;

        case 161:
            osal_strcpy(pInfo->name, "snmp");
            break;

        case 443:
            osal_strcpy(pInfo->name, "https");
            break;

        case 5353:
            osal_strcpy(pInfo->name, "bonjour");
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}

static int32 _sal_sys_backupPart_sync(char *srcFname, char *dstFname)
{
    uint32 exist = FALSE;

    if (SYS_ERR_OK == sal_util_file_exist(dstFname))
        exist = TRUE;

    if (FALSE == exist)
    {
        sal_util_file_copy(srcFname, dstFname);
    }
    
    else if (TRUE == exist)
    {
        if (FALSE == sal_util_isFileConsistent_ret(srcFname, dstFname))
            sal_util_file_copy(srcFname, dstFname);
    }

    return SYS_ERR_OK;
}

int32 sal_sys_backupConfig_sync(sys_conf_syncAction_t action)
{
  #if defined(CONFIG_FLASH_LAYOUT_TYPE3) || defined(CONFIG_FLASH_LAYOUT_TYPE4) || defined(CONFIG_FLASH_LAYOUT_TYPE5)
    if (SYS_CONF_SYNC_ALL == action)
    {
        _sal_sys_backupPart_sync(CONF_DEFAULT_FILE, CONF_BAKPART_DEFAULT_FILE);
        _sal_sys_backupPart_sync(CONF_USR_DEFAULT_FILE, CONF_BAKPART_USR_DEFAULT_FILE);
    }

    if ((SYS_CONF_SYNC_STARTUP == action) || (SYS_CONF_SYNC_ALL == action))
    {
        _sal_sys_backupPart_sync(CONF_STARTUP_FILE, CONF_BAKPART_STARTUP_FILE);
    }
    #ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    if ((SYS_CONF_SYNC_BACKUP == action) || (SYS_CONF_SYNC_ALL == action))
    {
        _sal_sys_backupPart_sync(CONF_BACKUP_FILE, CONF_BAKPART_BACKUP_FILE);
    }
    #endif
  #endif 
    return SYS_ERR_OK;
}

int32 sal_sys_backupConfig_restore(void)
{
  #if defined(CONFIG_FLASH_LAYOUT_TYPE3) || defined(CONFIG_FLASH_LAYOUT_TYPE4) || defined(CONFIG_FLASH_LAYOUT_TYPE5)
    _sal_sys_backupPart_sync(CONF_BAKPART_STARTUP_FILE, CONF_STARTUP_FILE);

    #ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    _sal_sys_backupPart_sync(CONF_BAKPART_BACKUP_FILE, CONF_BACKUP_FILE);
    #endif

    _sal_sys_backupPart_sync(CONF_BAKPART_DEFAULT_FILE, CONF_DEFAULT_FILE);

    _sal_sys_backupPart_sync(CONF_BAKPART_USR_DEFAULT_FILE, CONF_USR_DEFAULT_FILE);
  #endif 

    return SYS_ERR_OK;
}

#ifndef CONFIG_FLASH_LAYOUT_TYPE1

int32 sal_sys_sysinfoVar_init(void)
{
    char *buf = NULL;
    char *data_ptr = NULL;
    char devStr[32];
    char str[16];
    int32 fd = -1;
    uint32 calchksum = 0;
    uint32 crc = 0;
    uint32 size = 0;
    mtd_info_t mtd_info;

    osal_memset(devStr, 0, sizeof(devStr));
    osal_memset(str, 0, sizeof(str));

    FLSH_SYSINFO_MTDDEV_GET(devStr);

    if ((fd = open(devStr, O_RDONLY)) < 0)
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_OPEN;
    }

    if (ioctl(fd, MEMGETINFO, &mtd_info) < 0)
    {
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_IOCTL;
    }

    size = SYSINFO_ENV_SIZE;

    if (size > mtd_info.size)
        size = mtd_info.size;

    buf = malloc(size);
    
    data_ptr = buf + 4;

    if (read(fd, buf, size) < 0)
    {
        close(fd);
        free(buf);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        return SYS_ERR_DEV_READ;
    }

    close(fd);

    crc = ((buf[0] & 0xFF) << 24) + ((buf[1] & 0xFF) << 16) + ((buf[2] & 0xFF) << 8) + (buf[3] & 0xFF);
    calchksum = sal_util_crc32_ret(calchksum, data_ptr, size - 4);

    free(buf);

    if (crc != calchksum)
    {

#ifdef CONFIG_SYS_DUAL_IMAGE
        osal_memset(str, 0, sizeof(str));
        osal_sprintf(str, "%d", IMG_PART0);

        SYS_ERR_CHK(sal_sys_sysinfoVar_set(SYSINFO_VAR_DUALACTPART, str));
#endif 
    }

    return SYS_ERR_OK;
}

int32 sal_sys_sysinfoVar_get(char *name, char *pValue)
{
    char *buf = NULL;
    char *data_ptr = NULL;
    char *varname = NULL;
    char devStr[16];
    int32 ret = SYS_ERR_OK;
    int32 fd = -1;
    int32 i = 0;
    int32 nxt = 0;
    int32 varval = 0;
    uint32 calchksum = 0;
    uint32 crc = 0;
    uint32 size = 0;
    mtd_info_t mtd_info;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pValue), SYS_ERR_NULL_POINTER);

    osal_memset(devStr, 0, sizeof(devStr));

    FLSH_SYSINFO_MTDDEV_GET(devStr);

    USER_LOCK(SYS_USER_LOCK_FLSHINFO);

    if ((fd = open(devStr, O_RDONLY)) < 0)
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        ret = SYS_ERR_DEV_OPEN;

        goto _exit;
    }

    if (ioctl(fd, MEMGETINFO, &mtd_info) < 0)
    {
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        ret = SYS_ERR_IOCTL;

        goto _exit;
    }

    size = SYSINFO_ENV_SIZE;

    if (size > mtd_info.size)
        size = mtd_info.size;

    buf = malloc(size);
    osal_memset(buf, 0, size);
    
    data_ptr = buf + 4;

    if (read(fd, buf, size) < 0)
    {
        close(fd);
        free(buf);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        ret = SYS_ERR_DEV_READ;

        goto _exit;
    }

    close(fd);

    crc = ((buf[0] & 0xFF) << 24) + ((buf[1] & 0xFF) << 16) + ((buf[2] & 0xFF) << 8) + (buf[3] & 0xFF);

    calchksum = sal_util_crc32_ret(calchksum, data_ptr, size - 4);
    
    if (crc != calchksum)
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_SYSINFO_CHKSUM_ERROR);

    }

    for (i = 0; *(data_ptr + i) != '\0'; i = nxt + 1)
    {
        varname = data_ptr + i;
        varval = 0;

        for (nxt = i; *(data_ptr + nxt) != '\0'; ++nxt)
        {
            if (nxt > size)
            {
                free(buf);

                ret = SYS_ERR_INFO_NOT_EXIST;

                goto _exit;
            }

            if ('=' == *(data_ptr + nxt))
            {
                if ((strlen(name) == (nxt - i)) && 0 == osal_strncmp(name, varname, nxt - i))
                {
                    varval = nxt + 1;
                }
            }
        }
        if (0 != varval)
        {
            osal_strncpy(pValue, data_ptr + varval, nxt - varval);
            pValue[nxt - varval] = '\0';
            free(buf);

            ret = SYS_ERR_OK;
            goto _exit;
        }
    }

    free(buf);

    ret = SYS_ERR_INFO_NOT_EXIST;

_exit:
    USER_UNLOCK(SYS_USER_LOCK_FLSHINFO);

    return ret;
}

int32 sal_sys_sysinfoVar_set(char *name, char *value)
{
    char *buf = NULL;
    char *buf2 = NULL;
    char *data_ptr = NULL;
    char *data_ptr2 = NULL;
    char *varname = NULL;
    char *varname_end = NULL;
    char devStr[16];
    int32 ret = SYS_ERR_OK;
    int32 fd = -1;
    int32 i = 0;
    int32 nxt = 0;
    int32 varval = 0;
    uint32 crc = 0;
    uint32 idx = 0;
    uint32 size = 0;
    uint32 varSet = FALSE;
    uint32 varExist = FALSE;
    mtd_info_t mtd_info;
    erase_info_t erase_info;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == value), SYS_ERR_NULL_POINTER);

    osal_memset(devStr, 0, sizeof(devStr));

    FLSH_SYSINFO_MTDDEV_GET(devStr);

    USER_LOCK(SYS_USER_LOCK_FLSHINFO);

    if ((fd = open(devStr, O_RDONLY)) < 0)
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        ret = SYS_ERR_DEV_OPEN;

        goto _exit;
    }

    if (ioctl(fd, MEMGETINFO, &mtd_info) < 0)
    {
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        ret = SYS_ERR_IOCTL;

        goto _exit;
    }

    size = SYSINFO_ENV_SIZE;

    if (size > mtd_info.size)
        size = mtd_info.size;

    buf = malloc(size);
    buf2 = malloc(size);
    osal_memset(buf, 0, size);
    osal_memset(buf2, 0, size);

    data_ptr = buf + 4;
    data_ptr2 = buf2 + 4;

    if (read(fd, buf, size) < 0)
    {
        close(fd);
        free(buf);
        free(buf2);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        ret = SYS_ERR_DEV_READ;

        goto _exit;
    }

    close(fd);

    for (i = 0; *(data_ptr + i) != '\0'; i = nxt + 1)
    {
        varname = data_ptr + i;
        varval = 0;
        varExist = FALSE;

        for (nxt = i; *(data_ptr + nxt) != '\0'; ++nxt)
        {
            if (nxt > size)
            {
                free(buf);
                free(buf2);

                ret = SYS_ERR_FAILED;

                goto _exit;
            }

            if (*(data_ptr + nxt) == '=')
            {
                
                if ((strlen(name) == (nxt - i)) && 0 == osal_strncmp(name, varname, nxt - i))
                {
                    varname_end = data_ptr + nxt;
                    varExist = TRUE;
                }
                varval = nxt + 1;
            }
        }

        if ((0 != varval) && (FALSE == varExist))
        {
            osal_strncpy((data_ptr2 + idx), varname, (data_ptr + nxt) - varname);

            idx += (data_ptr + nxt) - varname;
            data_ptr2[idx] = '\0';
            idx++;
        }
        
        else if ((varval != 0) && (varExist == TRUE))
        {
            if (0 != osal_strcmp(value, ""))
            {
                osal_strncpy((data_ptr2 + idx), varname, varname_end - varname);

                idx += varname_end - varname;
                data_ptr2[idx] = '=';
                idx++;

                osal_strcpy((data_ptr2 + idx), value);
                idx += osal_strlen(value);
                data_ptr2[idx] = '\0';
                idx++;
            }
            varSet = TRUE;
        }
    }

    if ((FALSE == varSet) && (0 != osal_strcmp(value, "")))
    {
        osal_sprintf((data_ptr2 + idx), "%s=%s", name, value);
    }

    free(buf);

    crc = sal_util_crc32_ret(0, data_ptr2, size - 4);
    buf2[0] = (crc >> 24) & 0xFF;
    buf2[1] = (crc >> 16) & 0xFF;
    buf2[2] = (crc >> 8) & 0xFF;
    buf2[3] = crc & 0xFF;

    if ((fd = open(devStr, O_RDWR)) < 0)
    {
        free(buf2);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        ret = SYS_ERR_DEV_OPEN;

        goto _exit;
    }

    erase_info.start = 0x0;
    erase_info.length = mtd_info.size;
    if (ioctl(fd, MEMERASE, &erase_info) < 0)
    {
        free(buf2);
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        ret = SYS_ERR_IOCTL;

        goto _exit;
    }

    if (write(fd, buf2, size) < 0)
    {
        free(buf2);
        close(fd);

        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_FLASH_OPERATE_ERROR);

        ret = SYS_ERR_DEV_WRITE;

        goto _exit;
    }
    close(fd);

    free(buf2);

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_SYSINFO_CFG,
                name,
                value);

_exit:
    USER_UNLOCK(SYS_USER_LOCK_FLSHINFO);

    return ret;
}
#endif 

int32 sal_sys_resetStatus_get(uint32 *pResetStatus)
{
    SYS_PARAM_CHK((NULL == pResetStatus), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_sys_resetStatus_get(pResetStatus));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_CPU_UTILIZATION

int32 sal_sys_cpuUtilz_get(uint32 *pResult)
{
    FILE *fp;

    int i;
    char buf[SYS_BUF128_LEN * 2];

    char *str1;
    char *str2;

    unsigned long long jiffy1[8];
    unsigned long long jiffy2[8];

    SYS_PARAM_CHK((NULL == pResult), SYS_ERR_NULL_POINTER);

    memset(jiffy1, 0, sizeof(jiffy1));
    memset(jiffy2, 0, sizeof(jiffy2));

    str1 = buf;
    str2 = buf + SYS_BUF128_LEN;

    for (i = 0; i < 2; i++)
    {
        fp = fopen(_SYS_PATH_PROC_STAT, "r");
        if (NULL == fp)
            return SYS_ERR_FILE_OPEN;

        fgets(buf + (i * SYS_BUF128_LEN), SYS_BUF128_LEN, fp);

        fclose(fp);
        usleep(1000000);
    }

    sscanf(str1, "cp%*s %llu %llu %llu %llu %llu %llu %llu %llu",
                 &jiffy1[0], &jiffy1[1], &jiffy1[2], &jiffy1[3], &jiffy1[4], &jiffy1[5], &jiffy1[6], &jiffy1[7]);
    sscanf(str2, "cp%*s %llu %llu %llu %llu %llu %llu %llu %llu",
                 &jiffy2[0], &jiffy2[1], &jiffy2[2], &jiffy2[3], &jiffy2[4], &jiffy2[5], &jiffy2[6], &jiffy2[7]);

    for (i = 0; i < 8; i++)
        *pResult += (jiffy2[i] - jiffy1[i]);

    *pResult = 100 - (100 * (double)(jiffy2[3] - jiffy1[3]) / *pResult);

	cfg_sys_cpuUtilz_set(*pResult);
    return SYS_ERR_OK;
}

int32 sal_sys_cpuUtilz_sw_get(uint32 *pValue)
{
	int32 rc = SYS_ERR_OK;
	if(NULL == pValue)
		return SYS_ERR_NULL_POINTER;

	rc = cfg_sys_cpuUtilz_get(pValue);
	return rc;
}
#endif

#ifdef CONFIG_SYS_MEM_UTILIZATION

int32 sal_sys_memInfo_get(sys_mem_info_t* pMemInfo)
{
    FILE *fp;
    unsigned long swap_temp[2] = {0};
    char buf[SYS_BUF128_LEN];

    SYS_PARAM_CHK((NULL == pMemInfo), SYS_ERR_NULL_POINTER);

    fp = fopen(_SYS_PATH_PROC_MEMINFO, "r");

    if (NULL == fp)
        return SYS_ERR_FILE_OPEN;

    if (2 != fscanf(fp, "MemTotal: %lu %s\n", &pMemInfo->mem_total, buf))
    {
        fgets(buf, sizeof(buf), fp);
        fscanf(fp, "Mem: %lu %lu %lu %lu %lu %lu",
               &pMemInfo->mem_total, &pMemInfo->mem_used, &pMemInfo->mem_free,
               &pMemInfo->mem_shared, &pMemInfo->mem_buffer, &pMemInfo->mem_cached);
    }
    else
    {
        fscanf(fp, "MemFree: %lu %s\n", &pMemInfo->mem_free, buf);

        if (2 != fscanf(fp, "MemShared: %lu %s\n", &pMemInfo->mem_shared, buf))
            pMemInfo->mem_shared = 0;

        fscanf(fp, "Buffers: %lu %s\n", &pMemInfo->mem_buffer, buf);
        fscanf(fp, "Cached: %lu %s\n", &pMemInfo->mem_cached, buf);

        pMemInfo->mem_used = pMemInfo->mem_total - pMemInfo->mem_free;
    }

    fclose(fp);

    fp = fopen(_SYS_PATH_PROC_SWAPS, "r");

    if (NULL != fp) {

        fgets(buf, sizeof(buf), fp);

        while (fgets(buf, sizeof(buf), fp))
        {
            if (2 == sscanf(buf, "%*s %*s %lu %lu %*s", &swap_temp[0], &swap_temp[1]))
            {
                pMemInfo->swap_total += swap_temp[0];
                pMemInfo->swap_used += swap_temp[1];
            }
        }
        fclose(fp);
    }

    return SYS_ERR_OK;
}
#endif

int32 sal_sys_status_set(sys_status_t status)
{
    SYS_PARAM_CHK(SYS_STATUS_END <= status, SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_sys_status_set(status));

    return SYS_ERR_OK;
}

int32 sal_sys_status_get(sys_status_t *pStatus)
{
    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_sys_status_get(pStatus));

    return SYS_ERR_OK;
}

void sal_sleep(uint32 sec)
{
	sleep(sec);
}

void sal_msleep(uint32 msec)
{
	usleep(1000 * msec);
}

#ifdef CONFIG_SYS_HW_USB
int32 sal_sys_usb_enable(void)
{
    FILE *fp = NULL;
    uint32 i;
	uint32 total_line = 0;
    int32 ret = SYS_ERR_OK;
    char buf[SYS_BUF128_LEN * 10];
	uint32 major = {0};
	uint32 minor = {0};
	uint32 block = {0};
	char name[32];
    char cmd[SYS_BUF128_LEN];

    fp = fopen(_SYS_PATH_PROC_PARTITIONS, "r");
	if (NULL == fp)
    {
        osal_printf("\nCan not open proc partitions\n");
		return SYS_ERR_FILE_OPEN;
    }

	osal_memset(buf, 0, sizeof(buf));
	osal_memset(name, 0, sizeof(name));

    for (i = 0; i < 10; i++)
    {
        fgets(buf + (i * SYS_BUF128_LEN), SYS_BUF128_LEN, fp);
		if(buf[i * SYS_BUF128_LEN] ==  0)
		{
			total_line = i;
			break;
		}

    }
	fclose(fp);

	for(i = total_line - 1; i < total_line; i++)
	{
		sscanf(&buf[i * SYS_BUF128_LEN], "%u %u %u %s", &major, &minor, &block, name);
	}

	if(osal_strncmp("sd", name, 2) != 0)
	{
    	osal_printf("\nSystem can not found usb device.\n");
		return SYS_ERR_FAILED;
	}
	
	osal_memset(cmd, 0, sizeof(cmd));
	osal_sprintf(cmd, "mknod /dev/sda b %u %u 1>/dev/null 2>/dev/null", major, minor);
	ret = system(cmd);
	usleep(1000000);
    if(ret != 0)
    {
        osal_printf("\nmknod failed. ret=%d\n", ret);
    }

	osal_memset(cmd, 0, sizeof(cmd));
	osal_sprintf(cmd, "mkdir %s 1>/dev/null 2>/dev/null", SYS_USB_DIR);
	ret = system(cmd);
	usleep(1000000);
    if(ret != 0)
    {
        osal_printf("\nmkdir failed. ret=%d\n", ret);
    }

	osal_memset(cmd, 0, sizeof(cmd));
	osal_sprintf(cmd, "mount -t vfat /dev/sda %s 1>/dev/null 2>/dev/null", SYS_USB_DIR);
	ret = system(cmd);
	usleep(2000000);

    if(ret != 0)
    {
        osal_printf("\nmount fat mode failed. ret=%d\n", ret);
        
        osal_memset(cmd, 0, sizeof(cmd));
        osal_sprintf(cmd, "rm -rf %s 1>/dev/null 2>/dev/null", SYS_USB_DIR);
        ret = system(cmd);
        usleep(1000000);
        if(ret != 0)
        {
            osal_printf("\nrm dir failed. ret=%d\n", ret);
        }

        osal_memset(cmd, 0, sizeof(cmd));
        osal_sprintf(cmd, "rm /dev/sda 1>/dev/null 2>/dev/null");
        ret = system(cmd);
        usleep(1000000);
        if(ret != 0)
        {
            osal_printf("\nrm dev failed. ret=%d\n", ret);
        }

        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

int32 sal_sys_usb_show(void)
{
    int32 ret = SYS_ERR_OK;
    char cmd[SYS_BUF128_LEN];

	osal_memset(cmd, 0, sizeof(cmd));
	osal_sprintf(cmd, "ls %s", SYS_USB_DIR);
	ret = system(cmd);
    usleep(1000000);
    if(ret != 0)
    {
        osal_printf("\nls dir failed. ret=%d\n", ret);
    }

    return SYS_ERR_OK;
}

int32 sal_sys_usb_disable(void)
{
    int32 ret = SYS_ERR_OK;
    char cmd[SYS_BUF128_LEN];

	osal_memset(cmd, 0, sizeof(cmd));
	osal_sprintf(cmd, "umount /dev/sda 1>/dev/null 2>/dev/null");
    ret = system(cmd);
    usleep(1000000);
    if(ret != 0)
    {
        osal_printf("\nsal_sys_usb_disable umount failed. ret=%d\n", ret);
    }

	osal_memset(cmd, 0, sizeof(cmd));
	osal_sprintf(cmd, "rm -rf %s 1>/dev/null 2>/dev/null", SYS_USB_DIR);
    ret = system(cmd);
    usleep(1000000);
    if(ret != 0)
    {
        osal_printf("\nsal_sys_usb_disable rm dir failed. ret=%d\n", ret);
    }

	osal_memset(cmd, 0, sizeof(cmd));
	osal_sprintf(cmd, "rm /dev/sda 1>/dev/null 2>/dev/null");
    ret = system(cmd);
    usleep(1000000);
    if(ret != 0)
    {
        osal_printf("\nsal_sys_usb_disable rm dev failed. ret=%d\n", ret);
    }

    return SYS_ERR_OK;
}

int32 sal_config2usb(char *lFilename, char *rFilename)
{
    int32 ret;
    char cmd[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == lFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == rFilename), SYS_ERR_NULL_POINTER);

    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "cp -f %s %s/%s 1> /dev/null 2> /dev/null", lFilename, SYS_USB_DIR, rFilename);
    ret = system(cmd);

	usleep(2000000);
	
    if (SYS_ERR_OK != ret)
        return ret;

    return SYS_ERR_OK;
}

int32 sal_usb_config_download(char *lFilename, char *rFilename)
{
    int32 ret;
    char cmd[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == lFilename), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == rFilename), SYS_ERR_NULL_POINTER);

    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "cp -f %s/%s %s 1> /dev/null 2> /dev/null", SYS_USB_DIR, rFilename, lFilename);
    ret = system(cmd);

    if (SYS_ERR_OK != ret)
    {
        osal_printf("Copying file failed. ret=%d\n", ret);
        return ret;
    }
    sal_sys_backupConfig_sync(SYS_CONF_SYNC_STARTUP);
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    sal_sys_backupConfig_sync(SYS_CONF_SYNC_BACKUP);
#endif

    return SYS_ERR_OK;
}
#endif

int32 sal_sys_systemManufacturer_set(char *pManufacturer)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pManufacturer), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(osal_strlen(pManufacturer) > CAPA_SYS_LOCATION_LEN, SYS_ERR_STR_LEN_EXCEED);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_memset(sysinfo.manufacturerName, 0, sizeof(sysinfo.manufacturerName));

    osal_strcpy(sysinfo.manufacturerName, pManufacturer);

    SYS_ERR_CHK(rsd_sys_info_set(&sysinfo));

    SYS_ERR_CHK(cfg_sys_info_set(&sysinfo));

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_LOCATION,
            pManufacturer);

    return SYS_ERR_OK;
}

int32 sal_sys_systemManufacturer_get(char *pManufacturer)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pManufacturer), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(pManufacturer, sysinfo.manufacturerName);

    return SYS_ERR_OK;
}

int32 sal_sys_systemSupportInfo_set(char *pSupportInfo)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pSupportInfo), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(osal_strlen(pSupportInfo) > CAPA_SYS_LOCATION_LEN, SYS_ERR_STR_LEN_EXCEED);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_memset(sysinfo.supportInformation, 0, sizeof(sysinfo.supportInformation));

    osal_strcpy(sysinfo.supportInformation, pSupportInfo);

    SYS_ERR_CHK(rsd_sys_info_set(&sysinfo));

    SYS_ERR_CHK(cfg_sys_info_set(&sysinfo));

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_LOCATION,
            pSupportInfo);

    return SYS_ERR_OK;
}

int32 sal_sys_systemSupportInfo_get(char *pSupportInfo)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pSupportInfo), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(pSupportInfo, sysinfo.supportInformation);

    return SYS_ERR_OK;
}

int32 sal_sys_systemTelephone_set(char *pTelephone)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pTelephone), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(osal_strlen(pTelephone) > CAPA_SYS_NAME_LEN, SYS_ERR_STR_LEN_EXCEED);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_memset(sysinfo.telephoneNumber, 0, sizeof(sysinfo.telephoneNumber));

    osal_strcpy(sysinfo.telephoneNumber, pTelephone);

    SYS_ERR_CHK(rsd_sys_info_set(&sysinfo));

    SYS_ERR_CHK(cfg_sys_info_set(&sysinfo));

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_LOCATION,
            pTelephone);

    return SYS_ERR_OK;
}

int32 sal_sys_systemTelephone_get(char *pTelephone)
{
    sys_sysinfo_t sysinfo;

    SYS_PARAM_CHK((NULL == pTelephone), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(sysinfo);

    SYS_ERR_CHK(cfg_sys_info_get(&sysinfo));

    osal_strcpy(pTelephone, sysinfo.telephoneNumber);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_BOARD_FAN
int32 sal_sys_fan_status_get(uint32* pFanSpeed, uint32* pFanStatus)
{  
	SYS_PARAM_CHK((NULL == pFanSpeed), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == pFanStatus), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(rsd_sys_fan_get(pFanSpeed, pFanStatus));

    return SYS_ERR_OK;
}
int32 sal_sys_fan_speed_set(uint32 fanSpeed)
{
    
	SYS_ERR_CHK(rsd_sys_fan_set(fanSpeed));

    return SYS_ERR_OK;
}
#endif
#ifdef CONFIG_SYS_BOARD_TEMP
int32 sal_sys_temperature_get(char *pTemp)
{  
	SYS_PARAM_CHK((NULL == pTemp), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(rsd_sys_temp_get(pTemp));

    return SYS_ERR_OK;
}
#endif

