
#ifndef __CFG_H__
#define __CFG_H__

#include <libsal/sal_util.h>
#include <libvty/zebra.h>
#include <libvty/command.h>
#include <libcfgtbl/cfg_board.h>
#include <libcfg/cfg_sys.h>
#include <libcfg/cfg_vlan.h>
#include <libcfg/cfg_port.h>
#include <libcfg/cfg_trunk.h>
#include <libcfg/cfg_switch.h>
#ifdef CONFIG_SYS_MIRROR
#include <libcfg/cfg_mirror.h>
#endif
#include <libcfg/cfg_l2.h>
#ifdef CONFIG_SYS_L3_ROUTE
#include <libcfg/cfg_l3.h>
#endif
#include <libcfg/cfg_qos.h>
#ifdef CONFIG_SYS_SNMP
#include <libcfg/cfg_snmp.h>
#endif
#include <libcfg/cfg_rate.h>
#include <libcfg/cfg_syslog.h>
#include <libcfg/cfg_time.h>

#ifdef CONFIG_SYS_PORT_SECURITY
#include <libcfg/cfg_psecure.h>
#endif

#ifdef CONFIG_SYS_AAA
#include <libcfg/cfg_aaa.h>
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
#include <libcfg/cfg_authmgr.h>
#endif

#ifdef CONFIG_SYS_PROTO_LACP
#include <libcfg/cfg_lacp.h>
#endif

#ifdef CONFIG_SYS_PROTO_STP
#include <libcfg/cfg_stp.h>
#endif

#ifdef CONFIG_SYS_PROTO_LLDP
#include <libcfg/cfg_lldp.h>
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
#include <libcfg/cfg_lldp_med.h>
  #endif
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
#include <libcfg/cfg_igmp.h>
#endif

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#include <libcfg/cfg_mld.h>
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION
#include <libcfg/cfg_dos.h>
#endif

#ifdef CONFIG_SYS_LIB_CUSTOM
#include <libcustom/cfg_custom.h>
#endif

#ifdef CONFIG_SYS_PROTO_ISG
#include <libcfg/cfg_isg.h>
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
#include <libcfg/cfg_dhcp_snooping.h>
#endif

#ifdef CONFIG_SYS_PROTO_DAI
#include <libcfg/cfg_dai.h>
#endif

#ifdef CONFIG_SYS_LIB_RADIUS
#include <libcfg/cfg_radius.h>
#endif

#ifdef CONFIG_SYS_LIB_TACPLUS
#include <libcfg/cfg_tacplus.h>
#endif

#ifdef CONFIG_SYS_PROTO_GVRP
#include <libcfg/cfg_gvrp.h>
#endif

#ifdef CONFIG_SYS_PROTO_GARP
#include <libcfg/cfg_garp.h>
#endif

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
#include <libcfg/cfg_rmon.h>
#endif

#ifdef CONFIG_SYS_VOICEVLAN
#include <libcfg/cfg_vlan_voice.h>
#endif

#ifdef CONFIG_SYS_PROTOCOL_VLAN
#include <libcfg/cfg_vlan_proto.h>
#endif

#ifdef CONFIG_SYS_IP_SUBNET
#include <libcfg/cfg_vlan_ip_subnet.h>
#endif

#ifdef CONFIG_SYS_PROTO_UDLD
#include <libcfg/cfg_udld.h>
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
#include <libcfg/cfg_vlan_surveillance.h>
#endif

#ifdef CONFIG_SYS_PROTO_ERPS
#include <libcfg/cfg_erps.h>
#endif

#ifdef CONFIG_SYS_PROTO_LOOPBACK
#include <libcfg/cfg_loopback.h>
#endif

#ifdef CONFIG_SYS_POE
#include <libcfg/cfg_poe.h>
#endif

#ifdef CONFIG_SYS_DHCP_SERVER
#include <libcfg/cfg_dhcp_server.h>
#endif

#ifdef CONFIG_SYS_APP_EASYCWMP
#include <libcfg/cfg_easycwmp.h>
#endif

#define CFG_SHM_PATH "/cfgshm"
#define MAX_CFG_NAME_SIZE 32
#define MAX_CFG_FILE_NAME 32
#define MAX_CMD_STRLEN    CAPA_CLI_MAX_CMDSTR_LEN

#define CONF_STARTUP_BASENAME     "startup-config"
#define CONF_STARTUP_FILE         JFFS2_CFG_TARGET_DIR"/"CONF_STARTUP_BASENAME
#define CONF_DEFAULT_BASENAME     "startup-default"
#define CONF_DEFAULT_FILE         JFFS2_CFG_TARGET_DIR"/"CONF_DEFAULT_BASENAME
#define CONF_USR_DEFAULT_BASENAME "usr-default"
#define CONF_USR_DEFAULT_FILE     JFFS2_CFG_TARGET_DIR"/"CONF_USR_DEFAULT_BASENAME
#define CONF_STARTUP_TMPFILE      "/tmp/startup-config"
#define CONF_STARTUP_TMPSTATUFILE  "/tmp/mib-startup-config"
#define CONF_STARTUP_TO_RUN_TMPSTATUFILE  "/tmp/mib-startup-to-run-config"

#define CONF_RUNNING_BASENAME     "running-config"
#define CONF_RUNNING_FILE         "/var/config/"CONF_RUNNING_BASENAME
#define CONF_RUNNING_TMPFILE      "/tmp/tmp-running"
#define CONF_RUNNING_TMPSTATUSFILE "/tmp/mib-running-status"

#define CONF_CLONE_BASENAME     "clone-config"
#define CONF_CLONE_FILE         "/var/config/"CONF_CLONE_BASENAME
#define CONF_CLONE_TMPFILE         "/tmp/clone-config"

#define CONF_CLONE_TMPSTATUSFILE      "/tmp/mib-clone-status"
#define CONF_ClONE_TO_RUN_TMPSTATUFILE  "/tmp/mib-clone-to-run-config"
#define CONF_TFTP_UPLOAD_CFG_FILE  "/tmp/mib-tftp-upload-config"
#define CONF_TFTP_DOWNLOAD_CFG_FILE  "/tmp/mib-tftp-download-config"
#define CONF_TFTP_UPLOAD_STATU_FILE  "/tmp/mib-tftp-upload-statu"
#define CONF_TFTP_DOWNLOAD_STATU_FILE  "/tmp/mib-tftp-download-statu"

#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
#define CONF_BACKUP_BASENAME         "backup-config"
#define CONF_BACKUP_FILE             JFFS2_CFG_TARGET_DIR"/"CONF_BACKUP_BASENAME
#endif

#define CONF_BAKPART_STARTUP_FILE     JFFS2_LOG_TARGET_DIR"/"CONF_STARTUP_BASENAME
#define CONF_BAKPART_DEFAULT_FILE     JFFS2_LOG_TARGET_DIR"/"CONF_DEFAULT_BASENAME
#define CONF_BAKPART_USR_DEFAULT_FILE JFFS2_LOG_TARGET_DIR"/"CONF_USR_DEFAULT_BASENAME
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
#define CONF_BAKPART_BACKUP_FILE      JFFS2_LOG_TARGET_DIR"/"CONF_BACKUP_BASENAME
#endif

#define CONF_FIRST_LOGIN_FLAG       JFFS2_CFG_TARGET_DIR"/first_login"
#define CONF_FIRST_LOGIN_CHG_FLAG   "/tmp/first_login_chg"

#define CONF_HTTP_SAVERUNNING_STATUS  "/tmp/http_saverun_sts"
#define CONF_HTTP_SAVERUNNING_TMPFILE "/tmp/http_saverun_cfg"
#define CONF_HTTP_LOGO_TMPFILE "/tmp/http_logo_file.png"
#define CONF_HTTP_LOGO_TMPFILE_STATUS "/tmp/http_logo_file_sts"
#define CONF_HTTP_OEM_CFG    "/tmp/http_logo_file_cfg"

#ifdef CONFIG_SYS_TECH_SUPPORT
#define CONF_TECH_SUPPORT_NAME        "tech-support"
#define CONF_TECH_SUPPORT_FILE        "/tmp/tech-support"
#define CONF_TECH_GEN_FAIL            "/tmp/tech_genFail"
#define CONF_TECH_GEN_DONE            "/tmp/tech_genDone"
#endif

#define CONF_LOGO_FLASH_DIR     "/oeminfo/"
#define CONF_USRCFG_FLASH_FILE     "/oeminfo/usr_defconfig"
#define CONF_BANNER_USR_FILE     JFFS2_CFG_TARGET_DIR"/""motd"
#define CONF_BANNER_DEF_FILE     "/etc/motd"

#define CONF_HTTP_USRCFG_TMPFILE  "/tmp/http_usr_cfg"
#define CONF_USRCFG_TMPFILE  "custom_cfg"
#define CONF_HTTP_USRCFG_TMPFILE_STATUS "/tmp/http_usr_cfg_sts"

typedef enum cfg_id_e
{
    CFG_ID_DEVINFO = 0,
    CFG_ID_BOARD_MODEL,
    CFG_ID_SYSTEM,
    CFG_ID_TIME,
    CFG_ID_PORT,
#ifdef CONFIG_SYS_PORT_SECURITY
    CFG_ID_PSECURE,
#endif
    CFG_ID_SWITCH,
    CFG_ID_VLAN,
#ifdef CONFIG_SYS_VOICEVLAN
    CFG_ID_VLAN_VOICE,
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    CFG_ID_VLAN_PROTO,
#endif
#ifdef CONFIG_SYS_IP_SUBNET
	CFG_ID_VLAN_IP_SUBNET,
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    CFG_ID_VLAN_SURVEILLANCE,
#endif
#ifdef CONFIG_SYS_MIRROR
    CFG_ID_MIRROR,
#endif
    CFG_ID_L2,
#ifdef CONFIG_SYS_L3_ROUTE
    CFG_ID_L3,
#endif
    CFG_ID_TRUNK,
#ifdef CONFIG_SYS_PROTO_LACP
    CFG_ID_LACP,
#endif
    CFG_ID_QOS,
#ifdef CONFIG_SYS_UI_SNMP
    CFG_ID_SNMP,
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    CFG_ID_IGMP,
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    CFG_ID_MLD,
    #endif
#endif
    CFG_ID_RATE,
#ifdef CONFIG_SYS_PROTO_STP
    CFG_ID_STP,
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    CFG_ID_LLDP,
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    CFG_ID_LLDP_MED,
  #endif
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
    CFG_ID_DOS,
#endif
    CFG_ID_LOG,
#ifdef CONFIG_SYS_AAA
    CFG_ID_AAA,
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    CFG_ID_AUTHMGR,
#endif
#ifdef CONFIG_SYS_LIB_CUSTOM
    CFG_ID_CUSTOM,
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    CFG_ID_ISG,
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    CFG_ID_DHCP_SNOOPING,
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    CFG_ID_DAI,
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
    CFG_ID_RADIUS,
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
    CFG_ID_TACPLUS,
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    CFG_ID_GVRP,
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    CFG_ID_GARP,
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    CFG_ID_RMON,
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
    CFG_ID_UDLD,
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
    CFG_ID_ERPS,
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
    CFG_ID_LOOPBACK,
#endif
#ifdef CONFIG_SYS_POE
    CFG_ID_POE,
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	CFG_ID_DHCP_SERVER,
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	CFG_ID_EASYCWMP,
#endif

    CFG_ID_END
} cfg_id_t;

typedef struct cfg_map_s
{
    uint32 addr;
} cfg_map_t;

typedef void* cfg_addr;
typedef struct cfg_tbl_s
{
    cfg_id_t id;
    uint32 size;
} cfg_tbl_t;

union semun
{
    int             val;
    struct semid_ds *buf;
    unsigned short  *array;
    struct seminfo  *__buf;
};

#define CFG_DBG(a,args...) fprintf( stderr, "[CFG_DBG] %s:%d:"#a"\n", __FUNCTION__,__LINE__, ##args);
#define CFG_LOG(a,args...) fprintf( stderr, "[CFG_LOG] "#a"\n", ##args);

#define CFG_OPEN(id, ptr)           \
do{                                 \
    SYS_ERR_CHK(cfg_open(id, ptr)); \
}while(0)

#define CFG_CLOSE(id)               \
do{                                 \
    int32 ret = SYS_ERR_OK;         \
    SYS_ERR_CHK(cfg_close(id));     \
    return ret;                     \
}while(0)

#define CFG_NORET_CLOSE(id)         \
do{                                 \
    SYS_ERR_CHK(cfg_close(id));     \
}while(0)

#define CFG_ERR_CLOSE(id, errid )   \
do{                                 \
    SYS_ERR_CHK(cfg_close(id));     \
    return errid;                   \
}while(0)

extern int32 cfg_shm_init(void);

extern int32 cfg_currUnitId_set(uint32 unit);

extern uint32 cfg_currUnitId_ret(void);

extern int32 cfg_open(cfg_id_t id, cfg_addr *addr);

extern int32 cfg_close(cfg_id_t id);

extern int32 cfg_shm_dump(void);

#endif 

