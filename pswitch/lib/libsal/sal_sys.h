
#ifndef __SAL_SYS_H__
#define __SAL_SYS_H__

#include <rsd/rsd_sys.h>
#include <libsal/sal_sys_fw.h>
#include <libsal/sal_sys_auth.h>
#include <libsal/sal_sys_line.h>
#ifdef CONFIG_SYS_APP_SSH
#include <libsal/sal_ssh.h>
#endif
#ifdef CONFIG_SYS_ADVANCE_DNS
#include <rsd/rsd_dns.h>
#endif

#define SYS_FW_NAME_IMG0            "image0"
#define SYS_FW_NAME_IMG1            "image1"

#define SYS_SEC_FLH_KEY_DIR               "/mnt/ssh"
#define SYS_SEC_RAM_KEY_DIR               "/etc/ssh"
#define SYS_SEC_HTTP_RSA_FILE             SYS_SEC_FLH_KEY_DIR"/http_rsa_key"
#define SYS_SEC_HTTP_RSA_MODULUS_FILE     SYS_SEC_FLH_KEY_DIR"/http_rsa_modulus"
#define SYS_SEC_SSH_GEN_OK                SYS_SEC_FLH_KEY_DIR"/gen_ok"

#define SSL_CERT                          "ssl_cert"
#define SYS_SSL_CONFIG_FILE               "/usr/local/ssl/openssl.cnf"
#define SSL_CERT_FILE                     SYS_SEC_FLH_KEY_DIR"/ssl_cert.pem"
#define SSL_CERT_KEY_FILE                 SYS_SEC_FLH_KEY_DIR"/ssl_key.pem"
#define SSL_CERT_FILE_DFL                 "/etc/ssl_cert.pem"
#define SSL_CERT_KEY_DFL                  "/etc/ssl_key.pem"

#define IPV6_EMPTY_ADDR_STR         "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
#define _PATH_PROCNET_IFINET6       "/proc/net/if_inet6"
#define _PATH_PROCNET_IPV6ROUTE     "/proc/net/ipv6_route"

#define _SYS_PATH_PROC_MEMINFO      "/proc/meminfo"
#define _SYS_PATH_PROC_SWAPS        "/proc/swaps"
#define _SYS_PATH_PROC_STAT         "/proc/stat"
#define _SYS_PATH_PROC_PARTITIONS   "/proc/partitions"

#define SYS_POLLD_ACT_FWLOCK_TIMER  "/tmp/_polld_act_fwlock_timer"
#define SYS_POLLD_ACT_WEB_LOGIN     "/tmp/_polld_act_web_login"
#define SYS_POLLD_ACT_WEB_PING      "/tmp/_polld_act_web_ping"
#define SYS_POLLD_ACT_CUSTOM        "/tmp/_polld_act_custom"
#define SYS_POLLD_ACT_WEB_TRACEROUTE    "/tmp/_polld_act_web_traceroute"

#ifdef CONFIG_SYS_TECH_SUPPORT
#define SYS_POLLD_ACT_TECH_SUPPORT_GEN  "/tmp/_polld_act_tech_support"
#define SYS_POLLD_ACT_WEB_TECH_LOCK     "/tmp/_polld_act_tech_lock"
#endif

#define SYS_FWLOCK_TIMER_PID_FILE  "/var/run/fwlocktimer.pid"

#ifdef CONFIG_SYS_PING_TEST
#define SYS_PING_TEST_TMP_FILE              "/tmp/pingtest_tmp"
#define SYS_PING_TEST_REFRESH_TMP_FILE      "/tmp/pingtestRefresh_tmp"
#define SYS_PING_TEST_LOG_FILE              "/tmp/pingtest_log"
#endif

#define PROC_TCP4_FILE "/proc/net/tcp"
#define PROC_TCP_FILE "/proc/net/tcp6"
#define PROC_UDP_FILE "/proc/net/udp6"

#define SYS_DNS_RESOLV_FILE_DHCPCD  "/etc/resolv.conf.dhcpcd"
#define SYS_DNS_RESOLV_FILE_DHCPV6  "/etc/resolv.conf.dhcp6c-new"
#define SYS_DNS_RESOLV_FILE         "/etc/resolv.conf"
#define SYS_DNS_HOSTS_FILE          "/etc/hosts"
#define SYS_DNS_PROC_FILE           "hosts"

#define SYS_SYSTEM_READY_FLAG           "/var/system_ready"
#define SYS_SYSTEM_RESTART_FLAG         JFFS2_LOG_TARGET_DIR"/_sys_restart"
#define IS_SYSTEM_READY() (SYS_ERR_OK == sal_util_file_exist(SYS_SYSTEM_READY_FLAG))

#ifdef CONFIG_SYS_POE 

#define SYS_SYSTEM_POE_READY_FLAG           "/var/system_POE_ready"
#define POE_IS_SYSTEM_READY() (SYS_ERR_OK == sal_util_file_exist(SYS_SYSTEM_POE_READY_FLAG))
#endif

#define SYS_PING_TEST_TMP_FILE      "/tmp/pingtest_tmp"
#define SYS_TRACE_ROUTE_TMP_FILE    "/tmp/traceroute_tmp"
#define WEB_PING_LOCK_FILE          "/tmp/WEB_PING_LOCK"
#define WEB_PING_RESULT_FILE        "/tmp/web_ping_result"
#define SYS_TRACEROUTE_RESULT_FILE  "/tmp/_traceroute_result"
#define SYS_TRACEROUTE_LOCK_FILE    "/tmp/_traceroute_lock"

#define SYS_BOOTMSG_BACKUP_NUM      4
#define SYS_BOOTMSG_TEMP_FILE       "/var/log/boot.log"
#define SYS_BOOTMSG_BACKUP_FILE     JFFS2_LOG_TARGET_DIR"/boot.log"

#define SYS_OEM_USR_CONF_FILE       "/etc/usr_defconfig"
#define SYS_OEM_CONF_FILE           "/etc/oem_config"

#define SYS_USB_DIR            "/usb"

typedef enum sys_local_file_type_e
{
    SYS_LOCAL_FILE_CONFIG_STARTUP = 0,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    SYS_LOCAL_FILE_CONFIG_BACKUP,
#endif
    SYS_LOCAL_FILE_CONFIG_RUNNING,
    SYS_LOCAL_FILE_IMAGE_0,
    SYS_LOCAL_FILE_IMAGE_1,
    SYS_LOCAL_FILE_LOG_RAM,
    SYS_LOCAL_FILE_LOG_FLASH,
#ifdef CONFIG_SYS_APP_SSH
    SYS_LOCAL_FILE_SSH_RSA1,
    SYS_LOCAL_FILE_SSH_RSA2,
    SYS_LOCAL_FILE_SSH_DSA2,
#endif
    SYS_LOCAL_FILE_SSL_CERT,
    SYS_LOCAL_FILE_END
} sys_local_file_type_t;

typedef struct sys_mem_info_s
{
    unsigned long mem_total;
    unsigned long mem_used;
    unsigned long mem_free;
    unsigned long mem_shared;
    unsigned long mem_buffer;
    unsigned long mem_cached;

    unsigned long swap_total;
    unsigned long swap_used;
} sys_mem_info_t;

typedef struct sys_session_info_s
{
    char type[SYS_BUF8_LEN];
    char name[SYS_BUF16_LEN];
    char localIp[CAPA_IPV6_STR_LEN];
    char localPort[SYS_BUF8_LEN];
    char remoteIp[CAPA_IPV6_STR_LEN];
    char remotePort[SYS_BUF8_LEN];
    char tcpState[SYS_BUF16_LEN];
    uint32 isIPv6;
} sys_session_info_t;

typedef struct sys_dhcpv6_stateless_s
{
    sys_enable_t enable;
    uint32       refreshtime;
    uint32       min_refreshtime;
} sys_dhcpv6_stateless_t;

#ifdef CONFIG_SYS_ADVANCE_DNS
typedef enum sys_advDns_source_e
{
    SYS_ADVDNS_SRC_STATIC = 0,
    SYS_ADVDNS_SRC_DHCPV6,
    SYS_ADVDNS_SRC_DHCPV4,
    SYS_ADVDNS_SRC_END
} sys_advDns_source_t;

typedef struct sys_advDnsIP_s
{
    uint8 ipVer; 
    char ip[CAPA_IPV6_STR_LEN];
}sys_advDnsIP_t;

typedef struct sys_advDns_staticHost_s
{
    char host[SYS_BUF256_LEN];
    sys_advDnsIP_t ip;
}sys_advDns_staticHost_t;

typedef struct sys_advDns_s
{
    sys_enable_t enable;
    char domain[SYS_ADVDNS_SRC_END][SYS_BUF256_LEN];
    sys_advDnsIP_t nameServer[SYS_ADVDNS_SRC_END][CAPA_SYS_DNS_NAMESERVER_NUM];
    sys_advDns_staticHost_t staticHosts[CAPA_SYS_DNS_STATIC_HOST_NUM];
} sys_advDns_t;
#endif

extern const sys_text_t text_tcp_state[];

#define DO_POLLD_ACT(action)             \
do {                                     \
    sal_util_file_touch(action);         \
    system("killall -q -SIGUSR1 polld"); \
} while(0)

#define DO_POLLD_ACT_SIG()               \
do {                                     \
    system("killall -q -SIGUSR1 polld"); \
} while(0)

extern int32 sal_sys_init(void);

extern int32 sal_sys_devGeComboPortNum_get(uint32 unit, uint32 *pPortnum);

extern int32 sal_sys_devGeComboMinPortNum_get(uint32 unit, uint32 *pPortnum);

extern int32 sal_sys_boardModel_get(sys_boardModel_t *pBoardModel);

extern int32 sal_sys_boardConf_get(sys_board_conf_t *pBoardConf);

extern int32 sal_sys_boardModelId_get(sys_boardModel_Id_t *pBoardId);

extern int32 sal_sys_boardModelName_get(char *pBoardName);

extern int32 sal_sys_boardFlshLayout_get(sys_flsh_layout_t *pFlshLayout);

extern int32 sal_sys_boardPortConf_get(sys_logic_port_t port, sys_port_conf_t *pPortConf);

extern uint32 sal_sys_boardBootmsgLevel_ret(void);

extern int32 sal_sys_currSysCompiledTime_set(void);

extern int32 sal_sys_currSysCompiledTimeStr_get(char *timestr);

extern int32 sal_sys_currActiveImageIdx_get(uint32 *pIdx);

extern int32 sal_sys_sysinfo_get(sys_sysinfo_t *pSysinfo);

extern int32 sal_sys_systemName_set(char *pName);

extern int32 sal_sys_systemName_get(char *pName);

extern int32 sal_sys_systemLocation_set(char *pLocation);

extern int32 sal_sys_systemLocation_get(char *pLocation);

extern int32 sal_sys_systemContact_set(char *pContact);

extern int32 sal_sys_systemContact_get(char *pContact);

extern int32 sal_sys_system_softwareVersion_get(char *swVersion);

extern int32 sal_sys_macAddr_get(sys_mac_t *pMac);

extern int32 sal_sys_portMacAddr_get(sys_logic_port_t port, sys_mac_t *pMac);

extern int32 sal_sys_ip_get(uint32 *pIp, uint32 *pNetmask);

#ifdef CONFIG_SYS_ADVANCE_DNS

extern int32 sal_sys_dnsLookup_set(sys_enable_t enable);

extern int32 sal_sys_dnsLookup_get(sys_enable_t *pEnable);

extern int32 sal_sys_dnsDomain_set(char *pDoamin, sys_advDns_source_t src);

extern int32 sal_sys_dnsDomain_get(char *pDoamin, sys_advDns_source_t src);

extern int32 sal_sys_dnsNameServer_set(uint8 idx, sys_advDnsIP_t *pServer, sys_advDns_source_t src);

extern int32 sal_sys_dnsNameServer_get(uint8 idx, sys_advDnsIP_t *pServer, sys_advDns_source_t src);

extern int32 sal_sys_dnsNameServer_del(sys_advDnsIP_t *pServer);

extern int32 sal_sys_dnsStaticHost_set(sys_advDns_staticHost_t *pHost);

extern int32 sal_sys_dnsStaticHost_get(uint8 idx, sys_advDns_staticHost_t *pHost);

extern int32 sal_sys_dnsStaticIpByHost_get(char *pHost, char *pIp);

extern int32 sal_sys_dnsStaticHost_del(sys_advDns_staticHost_t *pHost);

extern int32 sal_sys_dnsResolvConf_create(void);

extern int32 sal_sys_dnsHosts_create(void);

extern int32 sal_sys_dnsDhcpConf_refresh(void);

extern int32 sal_sys_dnsDhcpv6Conf_refresh(void);
extern int32 sal_dynamic_host_clear(void);
#endif

extern int32 sal_sys_reboot(void);

extern int32 sal_sys_restore_defaults(sys_enable_t isFactory);

extern int32 sal_sys_htpasswd_create(void);

extern int32 sal_sys_passwd_create(void);

extern int32 sal_sys_jiffies_get(uint32 *pJiffies);

extern int32 sal_sys_hz_get(int32 *pHz);

extern int32 sal_sys_cpuFreq_get(uint32 *pFreq);

extern int32 sal_sys_serviceSessionProc_parse(char *line, sys_session_info_t *pInfo);

extern int32 sal_sys_backupConfig_sync(sys_conf_syncAction_t action);

extern int32 sal_sys_backupConfig_restore(void);

#ifndef CONFIG_FLASH_LAYOUT_TYPE1

extern int32 sal_sys_sysinfoVar_init(void);

extern int32 sal_sys_sysinfoVar_get(char *name, char *pValue);

extern int32 sal_sys_sysinfoVar_set(char *name, char *value);
#endif 

extern int32 sal_sys_resetStatus_get(uint32 *pResetStatus);

#ifdef CONFIG_SYS_CPU_UTILIZATION

extern int32 sal_sys_cpuUtilz_get(uint32 *pResult);
extern int32 sal_sys_cpuUtilz_sw_get(uint32 *pValue);
#endif

#ifdef CONFIG_SYS_MEM_UTILIZATION

extern int32 sal_sys_memInfo_get(sys_mem_info_t* pMemInfo);
#endif

extern int32 sal_sys_status_set(sys_status_t status);

extern int32 sal_sys_status_get(sys_status_t *pStatus);

extern void sal_sleep(uint32 sec);
extern void sal_msleep(uint32 sec);
extern int32 sal_sys_system_logo_set(char *pLoginLogo, char *pHomeLogo);
extern int32 sal_sys_system_home_logo_get(char *pName);
extern int32 sal_sys_system_login_logo_get(char *pName);
extern int32 sal_sys_title_set(char *pTitle);
extern int32 sal_sys_title_get(char *pName);
#ifdef CONFIG_SYS_HW_USB
extern int32 sal_sys_usb_enable(void);
extern int32 sal_config2usb(char *lFilename, char *rFilename);
extern int32 sal_usb_config_download(char *lFilename, char *rFilename);
extern int32 sal_sys_usb_disable(void);
extern int32 sal_sys_usb_show(void);
#endif

extern int32 sal_sys_systemSupportInfo_get(char *pSupportInfo);
extern int32 sal_sys_systemSupportInfo_set(char *pSupportInfo);
extern int32 sal_sys_systemManufacturer_set(char *pManufacturer);
extern int32 sal_sys_systemManufacturer_get(char *pManufacturer);
extern int32 sal_sys_systemTelephone_get(char * pTelephone);
extern int32 sal_sys_systemTelephone_set(char * pTelephone);
#ifdef CONFIG_SYS_BOARD_TEMP
extern int32 sal_sys_temperature_get(char *pTemp);
#endif
#endif 

