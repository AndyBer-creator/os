
#ifndef __CFG_SYSTEM_H__
#define __CFG_SYSTEM_H__

#include <libsal/sal_sys.h>
#include <libsal/sal_tftp.h>
#ifdef CONFIG_SYS_APP_SSH
#include <libsal/sal_ssh.h>
#endif

#define SAL_SYS_HTPASSWD_FILE   "/etc/htpasswd"
#define SAL_SYS_PASSWD_FILE     "/etc/passwd"

#define MAX_KEY_LEN     10240

typedef struct cfg_sys_s
{
    
    uint32 currSysCompTime;
    
    uint32 currActImgIdx;

#ifdef CONFIG_SYS_ADVANCE_DNS
    sys_advDns_t advDns;
#endif

    uint32 syslogServer;
    uint32 syslogDetail;

    sys_userInfo_t   localUser[CAPA_SYS_LOCALUSER_NUM];

    sys_line_cli_t lineCli[SYS_LINE_CLI_END];
    sys_line_web_t lineWeb[SYS_LINE_WEB_END];

#ifdef CONFIG_SYS_AAA
    sys_enblPasswd_t enablePassword[CAPA_AAA_PRIV_NUM];
#else
    sys_enblPasswd_t enablePassword[1];
#endif

    sys_sysinfo_t sysinfo;

#ifdef CONFIG_USER_SSH_SSHD
    sys_enable_t sshd;
    uint32       sshport;
#endif

#ifdef CONFIG_SYS_APP_TELNETD
    sys_enable_t telnetd;
    uint32       telport;
#endif

#ifdef CONFIG_SYS_APP_HTTPD
    sys_enable_t http;
#endif

#ifdef CONFIG_USER_BOA_WITH_SSL
    sys_enable_t https;
#endif
	uint32 cpuUtilz;

} cfg_sys_t;

extern int32 cfg_sys_userInfo_init(void);

extern int32 cfg_sys_currSysCompiledTime_set(uint32 time);

extern int32 cfg_sys_currSysCompiledTime_get(uint32 *pTime);

extern int32 cfg_sys_currActiveImageIdx_set(uint32 idx);

extern int32 cfg_sys_currActiveImageIdx_get(uint32 *pIdx);

extern int32 cfg_sys_info_set(sys_sysinfo_t *pSysinfo);

extern int32 cfg_sys_info_get(sys_sysinfo_t *pSysinfo);

#ifdef CONFIG_SYS_ADVANCE_DNS

extern int32 cfg_sys_dnsEnable_set(sys_enable_t enable);

extern int32 cfg_sys_dnsEnable_get(sys_enable_t *pEnable);

extern int32 cfg_sys_dnsDomain_set(char *pDomain, sys_advDns_source_t src);

extern int32 cfg_sys_dnsDomain_get(char *pDomain, sys_advDns_source_t src);

extern int32 cfg_sys_dnsNameServer_set(uint8 idx, sys_advDnsIP_t *pServer, sys_advDns_source_t src);

extern int32 cfg_sys_dnsNameServer_get(uint8 idx, sys_advDnsIP_t *pServer, sys_advDns_source_t src);

extern int32 cfg_sys_dnsStaticHost_set(uint8 idx, sys_advDns_staticHost_t *pHost);

extern int32 cfg_sys_dnsStaticHost_get(uint8 idx, sys_advDns_staticHost_t *pHost);
#endif

extern int32 cfg_sys_localUser_set(uint32 index, sys_userInfo_t userInfo);

extern int32 cfg_sys_localUser_get(uint32 index, sys_userInfo_t *pUserInfo);

extern int32 cfg_sys_enablePassword_set(int32 priv, sys_enblPasswd_t password);

extern int32 cfg_sys_enablePassword_get(int32 priv, sys_enblPasswd_t *pPassword);

extern int32 cfg_sys_lineCli_set(sys_line_cliType_t type, sys_line_cli_t *pLine);

extern int32 cfg_sys_lineCli_get(sys_line_cliType_t type, sys_line_cli_t *pLine);

#ifdef CONFIG_SYS_UI_WEB

extern int32 cfg_sys_lineWeb_set(sys_line_webType_t type, sys_line_web_t *pLine);

extern int32 cfg_sys_lineWeb_get(sys_line_webType_t type, sys_line_web_t *pLine);
#endif

#ifdef CONFIG_USER_SSH_SSHD

extern int32 cfg_sys_sshdEnable_set(sys_enable_t enable);

extern int32 cfg_sys_sshdEnable_get(sys_enable_t *pEnable);
extern int32 cfg_sys_sshd_port_set(uint32 port);
extern int32 cfg_sys_sshd_port_get(uint32 *port);
#endif 

#ifdef CONFIG_SYS_APP_TELNETD

extern int32 cfg_sys_telnetdEnable_set(sys_enable_t enable);
extern int32 cfg_sys_telnetd_port_set(uint32 port);

extern int32 cfg_sys_telnetdEnable_get(sys_enable_t *pEnable);
extern int32 cfg_sys_telnetd_port_get(uint32 *pPort);
#endif 

extern int32 cfg_sys_httpEnable_set(sys_enable_t enable);

extern int32 cfg_sys_httpEnable_get(sys_enable_t *pEnable);

#ifdef CONFIG_USER_BOA_WITH_SSL

extern int32 cfg_sys_httpsEnable_set(sys_enable_t enable);

extern int32 cfg_sys_httpsEnable_get(sys_enable_t *pEnable);
#endif

#ifdef CONFIG_SYS_IPV6

#ifdef CONFIG_SYS_APP_DHCP6C

extern int32 cfg_sys_dhcp6c_set(sys_enable_t dhcp6c);

extern int32 cfg_sys_dhcp6c_get(sys_enable_t *pDhcp6c);

#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS

extern int32 cfg_sys_dhcp6cStateless_set(sys_dhcpv6_stateless_t *pDhcp6c);

extern int32 cfg_sys_dhcp6cStateless_get(sys_dhcpv6_stateless_t *pDhcp6c);
#endif
#endif 

extern int32 cfg_sys_ipv6Enable_set(sys_enable_t enable);

extern int32 cfg_sys_ipv6Enable_get(sys_enable_t *pEnable);

extern int32 cfg_sys_ipv6Autoconf_set(sys_enable_t enable);

extern int32 cfg_sys_ipv6Autoconf_get(sys_enable_t *pEnable);

extern int32 cfg_sys_ipv6Address_set(char *pIp, uint32 prefix);

extern int32 cfg_sys_ipv6Address_get(char *pIp, uint32 *pPrefix);

extern int32 cfg_sys_ipv6Gateway_set(char *pGateway);

extern int32 cfg_sys_ipv6Gateway_get(char *pGateway);
#endif 
extern int32 cfg_sys_cpuUtilz_get(uint32 *value);
extern int32 cfg_sys_cpuUtilz_set(uint32 value);
#endif 

