
#ifndef SNMPCMTRAPS_H
#define SNMPCMTRAPS_H

#include <libcustom/sal_custom.h>

#define SNMP_DBG_TARGET_FILE3 "/tmp/SNMP_DEBUG3"
#if 1
#define SNMP_DBG3(a,args...) \
 do { \
	 char tmp_str[256] = ""; \
	 char cmd_str[300] = ""; \
	 sprintf(tmp_str, "[SNMP_DBG] %s:%d:"#a"", __FUNCTION__,__LINE__, ##args); \
	 sprintf(cmd_str, "echo \"%s\" >> %s", tmp_str, SNMP_DBG_TARGET_FILE3); \
	 system(cmd_str); \
 } while (0)
#else
#define SNMP_DBG1(a,args...) {}
#endif

typedef struct snmp_trapFormat_s
{
	char trapType[24];    
	char trapTime[48];	  
	char trapUser[112];	  
    char trapDetail[512]; 
    char sysIp[48]; 
    uint32 portId; 
    uint32 status1;       
    uint32 status2;       
} snmp_trapFormat_t;

int32 netsnmp_cmTrap_send_mux(snmp_cmTrapCode_t trapCode, char *trapInfo);
int32 netsnmp_cmTrap_send_mux_new(snmp_cmTrapCode_t trapCode, snmp_trapFormat_t *trapInfo);
void netsnmp_trap_oid_oem_init(void);

//add trap function
int32 snmp_trap_test(char *trapInfo);
int32 snmp_trap_cpu_busy(char *trapInfo);
int32 snmp_trap_memory_high(char *trapInfo);
int32 snmp_trap_storm_ctrl(char *trapInfo);
int32 snmp_trap_reboot_operate(char *trapInfo);
int32 snmp_trap_upgrade_operate(char *trapInfo);
int32 snmp_trap_user_operate(char *trapInfo);
int32 snmp_trap_poe(char *trapInfo);
int32 snmp_trap_system_start_new(snmp_trapFormat_t *trapInfo);

int32 snmp_trap_user_operate_new(snmp_trapFormat_t *trapInfo);
int32 snmp_trap_port_link_new(snmp_trapFormat_t *trapInfo);
int32 snmp_trap_user_operation_new(snmp_trapFormat_t *trapInfo);
int32 snmp_trap_login_status_new(snmp_trapFormat_t *trapInfo);
int32 snmp_trap_password_change_new(snmp_trapFormat_t *trapInfo);
int32 snmp_trap_system_reboot_new(snmp_trapFormat_t *trapInfo);
int32 snmp_trap_user_operation_info_new(snmp_trapFormat_t *trapInfo);



#endif 

