
#ifndef __SAL_MIB_PRIVATE_H__
#define __SAL_MIB_PRIVATE_H__

#define MIB_DUPLEX_AUTO 1
#define MIB_DUPLEX_HALF 2
#define MIB_DUPLEX_FULL 3

#define MIB_SPEED_AUTO        1
#define MIB_SPEED_AUTO_10M    11
#define MIB_SPEED_AUTO_100M   101
#define MIB_SPEED_AUTO_1000M  1001
#define MIB_SPEED_AUTO_10M_100M  1101
#define MIB_SPEED_AUTO_10G    10001
#define MIB_SPEED_10M         10
#define MIB_SPEED_100M        100
#define MIB_SPEED_1000M       1000
#define MIB_SPEED_2500M       2500
#define MIB_SPEED_10G         10000

#define MIB_VLANMODE_ACCESS  1
#define MIB_VLANMODE_HYBRID  2
#define MIB_VLANMODE_TRUNK   3
#define MIB_VLANMODE_TUNNEL  4  

#define MIB_ENABLE   1
#define MIB_DISABLE  2

#define UP  1
#define DOWN  2
#define TESTING  3
#define UNKNOWN  4
#define DORMANT  5
#define NOTPRESENT 6
#define LOWERLAYERDOWN 7

#define VLAN_MUN  256
#define SYS_USER_PORTMASK_LEN (1024/8)

typedef enum memory_type_s
{
	CMD_MEMORYTOTAL = 1,
	CMD_MEMORYUSED,
	CMD_MEMORYFREE,
	CMD_MEM_END
}memory_cmd_t;
typedef enum cpu_type_s
{
	CMD_CPUNAME = 4,
	CMD_CPUUSED,
	CMD_CPUTEMPERATURE,
	CMD_CPU_END
}cpu_cmd_t;

typedef enum port_ctrl_type_s
{
	CMD_PORTSPEED = 2,
	CMD_PORTDUPLEX,
	CMD_PORTVLANMODE,
	CMD_PORTPROTECTED,
	CMD_PORTLIMITINGRESS = 2,
	CMD_PORTLIMITEGRESS,
	CMD_PORTCTRL_END
}port_ctrl_cmd_t;

typedef enum port_static_vlan_type_s
{
	CMD_PORT_TAG = 2,
	CMD_PORT_UNTAG,
	CMD_PORT_STATUS,
	CMD_PORT_VLAN_END
}port_static_vlan_cmd_t;

typedef enum port_flow_rate_type_s
{
	TYPE_IN_PKT_RATE = 1,
	TYPE_IN_BYTE_RATE,
	TYPE_OUT_PKT_RATE,
	TYPE_OUT_BYTE_RATE,
	TYPE_RATE_END
}port_flow_rate_type_t;

typedef enum device_ip_s
{
	cmd_ipaddr = 2,
	cmd_ipmask,
	cmd_ip_end
}device_ip_t;

typedef enum sys_file_type_s
{
	SYS_TFTPSERVER = 2,
	SYS_FILENAME,
	SYS_IMGUPGRADE,
	SYS_DOWNLOAD = 5,
	SYS_DOWNLOAD_RUN_CONFIG = 4,
	SYS_DOWNLOAD_STARTUP_CONFIG = 5,
	SYS_DOWNLOAD_CLONE_CONFIG = 6,
	SYS_UPLOAD = 6,
	SYS_UPLOAD_RUN_CONFIG = 4,
	SYS_UPLOAD_STARTUP_CONFIG = 5,
	SYS_UPLOAD_CLONE_CONFIG = 6,
	SYS_BUILD_RUN_CONFIG = 7,
	SYS_BUILD_CLONE_CONFIG = 8,
	SYS_CONFIG_SAVE_FLASH = 9,
	SYS_CONFIG_STARTUP_TO_RUN = 10,
	SYS_CONFIG_CLONE_TO_RUN = 11,
	SYS_FILE_END
}sys_file_type_t;

typedef enum sys_trap_cfg_type_s
{
	SYS_TRAP_HOSTIPADDR = 2,
	SYS_TRAP_VERSION,
	SYS_TRAP_COMMUNITYNAME,
	SYS_TRAP_V3SECURITYLEVEL,
	SYS_TRAP_UDPPORT,
	SYS_TRAP_TRAPSTATUS,
	SYS_TRAP_END
}sys_trap_cfg_type_t;

typedef struct staticVlan_info_s
{
	sys_namestr_t name;
	sys_logic_portmask_t memberPorts;
	sys_logic_portmask_t untagPorts;
} staticVlan_t;

typedef enum sys_static_vlan_cfg_type_s
{
	SYS_VLAN_STATICVLANNAME = 1,
	SYS_VLAN_STATICVLANEGRESSPORTS,
	SYS_VLAN_STATICVLANUNTAGPORTS,
	SYS_VLAN_VLANROWSTATUS,
	SYS_VLAN_END
}sys_static_vlan_cfg_type_t;

#define SNMP_DBG_TARGET_FILE "/tmp/SNMP_DEBUG"
#if 0
#define SNMP_DBG(a,args...) \
 do { \
	 char tmp_str[256] = ""; \
	 char cmd_str[300] = ""; \
	 sprintf(tmp_str, "[SNMP_DBG] %s:%d:"#a"", __FUNCTION__,__LINE__, ##args); \
	 sprintf(cmd_str, "echo \"%s\" >> %s", tmp_str, SNMP_DBG_TARGET_FILE); \
	 system(cmd_str); \
 } while (0)
#else
#define SNMP_DBG(a,args...) {}
#endif

extern void sal_private_oid_oem_init(uint32* Oid);
#ifdef CONFIG_SYS_POE
extern uint32 sal_mib_poe_port_total_num_get(void);
extern uint32 sal_mib_poe_power_supply_total_get(int poePortNum);
extern uint32 sal_mib_poe_port_power_get(int port);
extern uint32 sal_mib_poe_port_reboot_timer_get(int port, char* pRestrTime, char* pDestrTime);
extern uint32 sal_mib_poe_port_reboot_timer_set(int port, char* pRestrTime, char* pDestrTime);
#endif 
extern int32 sal_mib_memory_info_get(int cmd, unsigned long* value);
extern int32 sal_mib_cpu_info_get(int cmd, void* value);
extern int32 sal_mib_interface_type_info_get(uint32 port, int* value);
extern int32 sal_mib_port_ctrl_get(uint32 port, uint32 cmd, uint32* value);
extern int32 sal_mib_port_ctrl_set(uint32 port, uint32 cmd, uint32 value);
extern int32 sal_mib_port_Attribute_get(uint32 port, uint32 cmd, uint32* value);
extern int32 sal_mib_port_Attribute_set(uint32 port, uint32 cmd, uint32 value);
extern int32 sal_mib_last_5_rate_set(sys_logic_port_t port, int type, uint64 pResult);
extern int32 sal_mib_last_5_rate_get(sys_logic_port_t port, int type, uint64 *pResult);
extern uint32 sal_mib_device_ip_get(int type, uint32* pValve);
extern uint32 sal_mib_file_upgrade_set(int type, void* value);
extern uint32 sal_mib_file_upgrade_get(int type, void* value);
extern uint32 sal_mib_poe_func_get(uint32* pValue);
extern uint32 sal_mib_config_file_status_get(int type, int* status);
extern uint32 sal_mib_build_config_file_set(int type);
extern uint32 sal_mib_file_config_save_flash_set(void);
extern uint32 sal_mib_file_startup_to_run_set(void);
extern uint32 sal_mib_file_clone_to_run_set(void);
extern uint32 sal_mib_snmp_trap_get(int type, int index, void* value);
extern uint32 sal_mib_snmp_trap_set(int type, int index, void* value);
extern uint32 sal_mib_static_vlan_by_index_getnet(int type, uint32* pVlanIndex, void* value);
extern uint32 sal_mib_static_vlan_by_index_get(int type, uint32* pVlanIndex, void* value);
extern uint32 sal_mib_port_static_vlan_set(int type, uint32 port, void* value, uint32 add);
extern uint32 sal_mib_static_vlan_get(void);
extern uint32 sal_mib_static_vlan_create(int type, uint32 vidindex, void* value, uint32 add);
#endif 

