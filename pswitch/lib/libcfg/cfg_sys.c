
#include <libcfg/cfg.h>

int32 cfg_sys_userInfo_init(void)
{
    cfg_sys_t *pSys = NULL;
    uint32 i;

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
        pSys->localUser[i].priv = -1;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_currSysCompiledTime_set(uint32 time)
{
    cfg_sys_t *pSys = NULL;

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    pSys->currSysCompTime = time;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_currSysCompiledTime_get(uint32 *pTime)
{
    cfg_sys_t *pSys = NULL;

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    *pTime = pSys->currSysCompTime;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_currActiveImageIdx_set(uint32 idx)
{
    cfg_sys_t *pSys = NULL;

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    pSys->currActImgIdx = idx;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_currActiveImageIdx_get(uint32 *pIdx)
{
    cfg_sys_t *pSys = NULL;

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    *pIdx = pSys->currActImgIdx;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_info_set(sys_sysinfo_t *pSysinfo)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK((NULL == pSysinfo), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(&pSys->sysinfo, pSysinfo, sizeof(sys_sysinfo_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_info_get(sys_sysinfo_t *pSysinfo)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK((NULL == pSysinfo), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(pSysinfo, &pSys->sysinfo, sizeof(sys_sysinfo_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

#ifdef CONFIG_SYS_ADVANCE_DNS

int32 cfg_sys_dnsEnable_set(sys_enable_t enable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    sys->advDns.enable = enable;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_dnsEnable_get(sys_enable_t *pEnable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    *pEnable = sys->advDns.enable;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_dnsDomain_set(char *pDomain, sys_advDns_source_t src)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(NULL == pDomain, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    SYS_MEM_CLEAR(sys->advDns.domain[src]);
    osal_strcpy(sys->advDns.domain[src], pDomain);

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_dnsDomain_get(char *pDomain, sys_advDns_source_t src)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(NULL == pDomain, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    osal_strcpy(pDomain, sys->advDns.domain[src]);

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_dnsNameServer_set(uint8 idx, sys_advDnsIP_t *pServer, sys_advDns_source_t src)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(idx >= CAPA_SYS_DNS_NAMESERVER_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pServer, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    osal_memcpy(&sys->advDns.nameServer[src][idx], pServer, sizeof(sys->advDns.nameServer[src][idx]));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_dnsNameServer_get(uint8 idx, sys_advDnsIP_t *pServer, sys_advDns_source_t src)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(idx >= CAPA_SYS_DNS_NAMESERVER_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pServer, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    osal_memcpy(pServer, &sys->advDns.nameServer[src][idx], sizeof(sys->advDns.nameServer[src][idx]));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_dnsStaticHost_set(uint8 idx, sys_advDns_staticHost_t *pHost)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(idx >= CAPA_SYS_DNS_STATIC_HOST_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pHost, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    osal_memcpy(&sys->advDns.staticHosts[idx], pHost, sizeof(sys_advDns_staticHost_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_dnsStaticHost_get(uint8 idx, sys_advDns_staticHost_t *pHost)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(idx >= CAPA_SYS_DNS_STATIC_HOST_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pHost, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    osal_memcpy(pHost, &sys->advDns.staticHosts[idx], sizeof(sys_advDns_staticHost_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}
#endif

int32 cfg_sys_localUser_set(uint32 index, sys_userInfo_t userInfo)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK(index >= CAPA_SYS_LOCALUSER_NUM, SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(&pSys->localUser[index], &userInfo, sizeof(sys_userInfo_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_localUser_get(uint32 index, sys_userInfo_t *pUserInfo)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK(index >= CAPA_SYS_LOCALUSER_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pUserInfo, SYS_ERR_NULL_POINTER);

    osal_memset(pUserInfo, 0, sizeof(sys_userInfo_t));

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(pUserInfo, &pSys->localUser[index], sizeof(sys_userInfo_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_enablePassword_set(int32 priv, sys_enblPasswd_t password)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK(priv < BOND_AAA_PRIV_MIN || priv > BOND_AAA_PRIV_MAX, SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(&pSys->enablePassword[priv], &password, sizeof(sys_enblPasswd_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_enablePassword_get(int32 priv, sys_enblPasswd_t *pPassword)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK(priv < BOND_AAA_PRIV_MIN || priv > BOND_AAA_PRIV_MAX, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pPassword, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(pPassword, &pSys->enablePassword[priv], sizeof(sys_enblPasswd_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_lineCli_set(sys_line_cliType_t type, sys_line_cli_t *pLine)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK(NULL == pLine, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(&pSys->lineCli[type], pLine, sizeof(sys_line_cli_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_lineCli_get(sys_line_cliType_t type, sys_line_cli_t *pLine)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK(NULL == pLine, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(pLine, &pSys->lineCli[type], sizeof(sys_line_cli_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

#ifdef CONFIG_SYS_UI_WEB

int32 cfg_sys_lineWeb_set(sys_line_webType_t type, sys_line_web_t *pLine)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK(NULL == pLine, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(&pSys->lineWeb[type], pLine, sizeof(sys_line_web_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_lineWeb_get(sys_line_webType_t type, sys_line_web_t *pLine)
{
    cfg_sys_t *pSys = NULL;

    SYS_PARAM_CHK(NULL == pLine, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);

    osal_memcpy(pLine, &pSys->lineWeb[type], sizeof(sys_line_web_t));

    CFG_CLOSE(CFG_ID_SYSTEM);
}
#endif

#ifdef CONFIG_USER_SSH_SSHD

int32 cfg_sys_sshdEnable_set(sys_enable_t enable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    sys->sshd = enable;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_sshdEnable_get(sys_enable_t *pEnable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    *pEnable = sys->sshd;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_sshd_port_set(uint32 port)
{
    cfg_sys_t *sys = NULL;

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    sys->sshport = port;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_sshd_port_get(uint32 *port)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(NULL == port, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    *port = sys->sshport;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

#endif

#ifdef CONFIG_SYS_APP_TELNETD

int32 cfg_sys_telnetdEnable_set(sys_enable_t enable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    sys->telnetd = enable;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_telnetd_port_set(uint32 port)
{
    cfg_sys_t *sys = NULL;

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    sys->telport = port;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_telnetdEnable_get(sys_enable_t *pEnable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    *pEnable = sys->telnetd;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_telnetd_port_get(uint32 *pPort)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(NULL == pPort, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    *pPort = sys->telport;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

#endif

#ifdef CONFIG_SYS_UI_WEB

int32 cfg_sys_httpEnable_set(sys_enable_t enable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    sys->http = enable;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_httpEnable_get(sys_enable_t *pEnable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    *pEnable = sys->http;

    CFG_CLOSE(CFG_ID_SYSTEM);
}
#endif

#ifdef CONFIG_USER_BOA_WITH_SSL

int32 cfg_sys_httpsEnable_set(sys_enable_t enable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    sys->https = enable;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_httpsEnable_get(sys_enable_t *pEnable)
{
    cfg_sys_t *sys = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&sys);

    *pEnable = sys->https;

    CFG_CLOSE(CFG_ID_SYSTEM);
}
#endif

int32 cfg_sys_cpuUtilz_get(uint32 *value)
{
    SYS_PARAM_CHK(NULL == value, SYS_ERR_NULL_POINTER);

	cfg_sys_t *pSys = NULL;

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);
	*value = pSys->cpuUtilz;

    CFG_CLOSE(CFG_ID_SYSTEM);
}

int32 cfg_sys_cpuUtilz_set(uint32 value)
{
    if(value > 100 && value < 0)
    	return SYS_ERR_FAILED;

	cfg_sys_t *pSys = NULL;

    CFG_OPEN(CFG_ID_SYSTEM, (cfg_addr)&pSys);
	pSys->cpuUtilz = value;
    CFG_CLOSE(CFG_ID_SYSTEM);
}

