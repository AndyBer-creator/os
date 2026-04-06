#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "snmpCmtraps.h"

#include <common/sys_type.h>
#include <libsal/sal_sys_line.h>

static oid snmptrap_std_oid[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};
static oid snmptrap_logType_oid[] = {1, 3, 6, 1, 4, 1, 27282, 1, 1, 2, 1, 1};
static oid snmptrap_logTime_oid[] = {1, 3, 6, 1, 4, 1, 27282, 1, 1, 2, 1, 2};
static oid cpu_busy_trap_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,3,1 };
static oid cpu_busy_msg_oid[] = { 1,3,6,1,4,1,27282,1,1,1,1,2,2};
static oid memory_high_trap_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,3,2};
static oid memory_high_msg_oid[] = { 1,3,6,1,4,1,27282,1,1,1,1,1,2 };
static oid storm_ctrl_trap_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,3,4 };
static oid storm_ctrl_msg_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,3,3};
#ifdef CONFIG_SYS_OEM_IKUAI
static oid sys_reboot_trap_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,4 };
static oid sys_reboot_msg_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,3 };
static oid sys_upgrade_trap_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,4 };
static oid sys_upgrade_msg_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,3 };
#else
static oid sys_upgrade_trap_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,2 };
static oid sys_upgrade_msg_oid[] = { 1,3,6,1,4,1,27282,1,1,1,6,3,3 };
static oid sys_reboot_trap_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,1 };
static oid sys_reboot_msg_oid[] = { 1,3,6,1,4,1,27282,1,1,1,6,2,1 };
#endif
static oid op_user_trap_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,4 };
static oid op_user_msg_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,3 };
static oid sys_poe_trap_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,4 };
static oid sys_poe_msg_oid[] = { 1,3,6,1,4,1,27282,1,1,2,1,4,3 };


static oid sys_log_time_oid[] = { 1,3,6,1,2,1,1,3,0 };




#define SYSPORTLINKSTATUSUP 1

//tarpInfo �е�һ���ֽ�ΪtrapCode ���� ʱ��  ����

void netsnmp_trap_oid_oem_init(void)
{
	sal_private_oid_oem_init(snmptrap_logType_oid);
	sal_private_oid_oem_init(snmptrap_logTime_oid);
	sal_private_oid_oem_init(cpu_busy_trap_oid);
	sal_private_oid_oem_init(cpu_busy_msg_oid);
	sal_private_oid_oem_init(memory_high_trap_oid);
	sal_private_oid_oem_init(memory_high_msg_oid);
	sal_private_oid_oem_init(storm_ctrl_trap_oid);
	sal_private_oid_oem_init(storm_ctrl_msg_oid);
	sal_private_oid_oem_init(sys_reboot_trap_oid);
	sal_private_oid_oem_init(sys_reboot_msg_oid);
	sal_private_oid_oem_init(sys_upgrade_trap_oid);
	sal_private_oid_oem_init(sys_upgrade_msg_oid);
	sal_private_oid_oem_init(op_user_trap_oid);
	sal_private_oid_oem_init(op_user_msg_oid);
	sal_private_oid_oem_init(sys_poe_trap_oid);
	sal_private_oid_oem_init(sys_poe_msg_oid);
}
int32 netsnmp_cmTrap_send_mux(snmp_cmTrapCode_t trapCode, char *trapInfo)
{
   if (NULL == trapInfo)
        return SYS_ERR_FAILED;

    if (trapCode <= SNMP_CMTRAP_CODE_START)
    {
        snmp_log(LOG_DEBUG, "unknwon snmp trap code %d \n", trapCode);
        return SYS_ERR_FAILED;
    }

    switch(trapCode)
    {
        case SNMP_CMTRAP_CODE_TEST:
            snmp_trap_test(trapInfo);
            break;
        case SNMP_CMTRAP_CODE_CPU:
        	snmp_trap_cpu_busy(trapInfo);
        	break;
        case SNMP_CMTRAP_CODE_MEMORY:
        	snmp_trap_memory_high(trapInfo);
        	break;   
        case SNMP_CMTRAP_CODE_STORMCTRL:
        	snmp_trap_storm_ctrl(trapInfo);
        	break;
        	
        case SNMP_CMTRAP_CODE_POE:
        	snmp_trap_poe(trapInfo);
        	//snmp_log(LOG_DEBUG, " poe snmp trap code %d %s\n", trapCode, trapInfo);
        	break;
		
		case SNMP_CMTRAP_CODE_REBOOT:
			//snmp_trap_reboot_operate(trapInfo);
			snmp_trap_user_operate(trapInfo);
			//snmp_log(LOG_DEBUG, " reboot snmp trap code %d %s\n", trapCode, trapInfo);
			break;
			
		case SNMP_CMTRAP_CODE_UPGRADE:	
			snmp_trap_upgrade_operate(trapInfo);
			//snmp_log(LOG_DEBUG, " upgrade snmp trap code %d %s\n", trapCode, trapInfo);
			break;
		case SNMP_CMTRAP_CODE_OPERATE:  
			 snmp_trap_user_operate(trapInfo);
			 //snmp_log(LOG_DEBUG, " upgrade snmp trap code %d %s\n", trapCode, trapInfo);
			 break;

         default:
            snmp_log(LOG_DEBUG, "unknwon snmp trap code %d \n", trapCode);
            break;
    }

    return SYS_ERR_OK;
}

int32 netsnmp_cmTrap_send_mux_new(snmp_cmTrapCode_t trapCode, snmp_trapFormat_t *trapInfo)
{
   if (NULL == trapInfo)
        return SYS_ERR_FAILED;

    if (trapCode < SNMP_CMTRAP_CODE_START)
    {
        snmp_log(LOG_DEBUG, "unknwon snmp trap code %d \n", trapCode);
        return SYS_ERR_FAILED;
    }

    switch(trapCode)
    {
        case SNMP_CMTRAP_CODE_START:
            snmp_trap_system_start_new(trapInfo);
            break;
        case SNMP_CMTRAP_CODE_TEST:
            //snmp_trap_test(trapInfo);
            break;
        case SNMP_CMTRAP_CODE_CPU:
        	//snmp_trap_cpu_busy(trapInfo);
        	break;
        case SNMP_CMTRAP_CODE_MEMORY:
        	//snmp_trap_memory_high(trapInfo);
        	break;   
        case SNMP_CMTRAP_CODE_STORMCTRL:
        	//snmp_trap_storm_ctrl(trapInfo);
        	break;
        	
        case SNMP_CMTRAP_CODE_POE:
        	//snmp_trap_poe(trapInfo);
        	//snmp_log(LOG_DEBUG, " poe snmp trap code %d %s\n", trapCode, trapInfo);
        	break;
			
		case SNMP_CMTRAP_CODE_UPGRADE:	
			//snmp_trap_upgrade_operate(trapInfo);
			//snmp_log(LOG_DEBUG, " upgrade snmp trap code %d %s\n", trapCode, trapInfo);
			break;

		case SNMP_CMTRAP_CODE_LINK:	
			snmp_trap_port_link_new(trapInfo);
			//snmp_log(LOG_DEBUG, " upgrade snmp trap code %d %s\n", trapCode, trapInfo);
			break;	
		case SNMP_CMTRAP_CODE_OPERATE:  
			 snmp_trap_user_operate_new(trapInfo);
			 //snmp_log(LOG_DEBUG, " upgrade snmp trap code %d %s\n", trapCode, trapInfo);
			 break;
		case SNMP_CMTRAP_CODE_LOGIN:  
			 snmp_trap_login_status_new(trapInfo);
			 break;
		case SNMP_CMTRAP_CODE_PASSWORD_CHANGE:  
			 snmp_trap_password_change_new(trapInfo);
			 break;
        case SNMP_CMTRAP_CODE_REBOOT:
			snmp_trap_system_reboot_new(trapInfo);
			break;
		case SNMP_CMTRAP_CODE_USER_OPERATION:
			snmp_trap_user_operation_new(trapInfo);
			break;
	    case SNMP_CMTRAP_CODE_USER_OPERATION_INFO:
			snmp_trap_user_operation_info_new(trapInfo);
			break;
         default:
            snmp_log(LOG_DEBUG, "unknwon snmp trap code %d \n", trapCode);
            break;
    }

    return SYS_ERR_OK;
}



int32 snmp_trap_test(char *trapInfo)
{
    char *pTemp = NULL;
    netsnmp_variable_list  *var_list = NULL;

    oid stpTopChg_trap_oid[] = {1,3,6,1,4,1,27282, 100,2,8};
    oid stpTopChg_msg_oid[] = {1,3,6,1,4,1,27282, 100,2,7};

    pTemp = osal_strchr(trapInfo, ' ');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;

    pTemp++;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        stpTopChg_trap_oid, sizeof(stpTopChg_trap_oid));

    snmp_varlist_add_variable(&var_list,
        stpTopChg_msg_oid, OID_LENGTH(stpTopChg_msg_oid),
        ASN_OCTET_STR,
        (void *)pTemp, osal_strlen(pTemp));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}



int32 snmp_trap_cpu_busy(char *trapInfo)
{
    char *pTemp = NULL;
    char *pTemp2 = NULL;
    uint32 typelen = 0;
    netsnmp_variable_list  *var_list = NULL;
	//char logtype[]= "CPU usage exception";
	
    pTemp = osal_strchr(trapInfo, ' ');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;

    pTemp++;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        cpu_busy_trap_oid, sizeof(cpu_busy_trap_oid));
                
	//��־ʱ��
	snmp_varlist_add_variable(&var_list,
        snmptrap_logTime_oid, OID_LENGTH(snmptrap_logTime_oid),
        ASN_OCTET_STR,
        (void *)pTemp, 25);
        
  	//��־����
	pTemp = osal_strchr(trapInfo, '#');
	if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp++;  

    pTemp2 = osal_strchr(trapInfo, '*');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp2++;

	typelen = pTemp2 - pTemp - 1;

    snmp_varlist_add_variable(&var_list,
        snmptrap_logType_oid, OID_LENGTH(snmptrap_logType_oid),
        ASN_OCTET_STR,
        (void *)pTemp, typelen);     
        
    //��־����
    snmp_varlist_add_variable(&var_list,
        cpu_busy_msg_oid, OID_LENGTH(cpu_busy_msg_oid),
        ASN_OCTET_STR,
        (void *)pTemp2, osal_strlen(pTemp2));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}



int32 snmp_trap_memory_high(char *trapInfo)
{
    char *pTemp = NULL;
    netsnmp_variable_list  *var_list = NULL;
	//char logtype[]= "Memory usage exception";
	char *pTemp2 = NULL;
    uint32 typelen = 0;
	
    pTemp = osal_strchr(trapInfo, ' ');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;

    pTemp++;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        memory_high_trap_oid, sizeof(memory_high_trap_oid));
              
	//��־ʱ��
	snmp_varlist_add_variable(&var_list,
        snmptrap_logTime_oid, OID_LENGTH(snmptrap_logTime_oid),
        ASN_OCTET_STR,
        (void *)pTemp, 25);
        
	//��־����
	pTemp = osal_strchr(trapInfo, '#');
	if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp++;  

    pTemp2 = osal_strchr(trapInfo, '*');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp2++;

	typelen = pTemp2 - pTemp - 1;
	
    snmp_varlist_add_variable(&var_list,
        snmptrap_logType_oid, OID_LENGTH(snmptrap_logType_oid),
        ASN_OCTET_STR,
        (void *)pTemp, typelen);  
        
    //��־����
    snmp_varlist_add_variable(&var_list,
        memory_high_msg_oid, OID_LENGTH(memory_high_msg_oid),
        ASN_OCTET_STR,
        (void *)pTemp2, osal_strlen(pTemp2));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}


int32 snmp_trap_storm_ctrl(char *trapInfo)
{
    char *pTemp = NULL;
    netsnmp_variable_list  *var_list = NULL;
	//char logtype[]= "storm warning";
	char *pTemp2 = NULL;
    uint32 typelen = 0;
    
    pTemp = osal_strchr(trapInfo, ' ');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;

    pTemp++;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        storm_ctrl_trap_oid, sizeof(storm_ctrl_trap_oid));
        
	//��־ʱ��
	snmp_varlist_add_variable(&var_list,
        snmptrap_logTime_oid, OID_LENGTH(snmptrap_logTime_oid),
        ASN_OCTET_STR,
        (void *)pTemp, 25);

	//��־����
	pTemp = osal_strchr(trapInfo, '#');
	if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp++;  

    pTemp2 = osal_strchr(trapInfo, '*');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp2++;
    typelen = pTemp2 - pTemp - 1;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_logType_oid, OID_LENGTH(snmptrap_logType_oid),
        ASN_OCTET_STR,
        (void *)pTemp, typelen);

    //��־����
    snmp_varlist_add_variable(&var_list,
        storm_ctrl_msg_oid, OID_LENGTH(storm_ctrl_msg_oid),
        ASN_OCTET_STR,
        (void *)pTemp2, osal_strlen(pTemp2));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}


int32 snmp_trap_reboot_operate(char *trapInfo)
{
    char *pTemp = NULL;
    netsnmp_variable_list  *var_list = NULL;
	//char logtype[]= "reboot system";
	char *pTemp2 = NULL;
    uint32 typelen = 0;
    
    pTemp = osal_strchr(trapInfo, ' ');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;

    pTemp++;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        sys_reboot_trap_oid, sizeof(sys_reboot_trap_oid));
        
	//��־ʱ��
	snmp_varlist_add_variable(&var_list,
        snmptrap_logTime_oid, OID_LENGTH(snmptrap_logTime_oid),
        ASN_OCTET_STR,
        (void *)pTemp, 25);
        
   	//��־����
	pTemp = osal_strchr(trapInfo, '#');
	if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp++;  

    pTemp2 = osal_strchr(trapInfo, '*');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp2++;
    typelen = pTemp2 - pTemp - 1;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_logType_oid, OID_LENGTH(snmptrap_logType_oid),
        ASN_OCTET_STR,
        (void *)typelen, typelen);  
        
    //��־����
    snmp_varlist_add_variable(&var_list,
        sys_reboot_msg_oid, OID_LENGTH(sys_reboot_msg_oid),
        ASN_OCTET_STR,
        (void *)pTemp2, osal_strlen(pTemp2));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}



int32 snmp_trap_upgrade_operate(char *trapInfo)
{
    char *pTemp = NULL;
    netsnmp_variable_list  *var_list = NULL;
	//char logtype[]= "upgrade image file";
	char *pTemp2 = NULL;
    uint32 typelen = 0;
    
    pTemp = osal_strchr(trapInfo, ' ');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;

    pTemp++;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        sys_upgrade_trap_oid, sizeof(sys_upgrade_trap_oid));
        
        
	//��־ʱ��
	 
	//��־����
	pTemp = osal_strchr(trapInfo, '#');
	if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp++;  

    pTemp2 = osal_strchr(trapInfo, '*');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp2++;
    typelen = pTemp2 - pTemp - 1;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_logType_oid, OID_LENGTH(snmptrap_logType_oid),
        ASN_OCTET_STR,
        (void *)pTemp, typelen);
        
    //��־����
    snmp_varlist_add_variable(&var_list,
        sys_upgrade_msg_oid, OID_LENGTH(sys_upgrade_msg_oid),
        ASN_OCTET_STR,
        (void *)pTemp2, osal_strlen(pTemp2));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}



int32 snmp_trap_user_operate(char *trapInfo)
{
    char *pTemp = NULL;
    netsnmp_variable_list  *var_list = NULL;
	//char logtype[]= "upgrade image file";
	char *pTemp2 = NULL;
	char *pTemp3 = NULL;
    uint32 typelen = 0;
    int pid;
    sys_line_session_t session;
    char buf[516] = {0};
    char typestr[50] ={0};
    
    pTemp = osal_strchr(trapInfo, ' ');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;

    pTemp++;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        op_user_trap_oid, sizeof(op_user_trap_oid));
        
        
	//��־ʱ��
	#if 0
	snmp_varlist_add_variable(&var_list,
        snmptrap_logTime_oid, OID_LENGTH(snmptrap_logTime_oid),
        ASN_OCTET_STR,
        (void *)pTemp, 25);
    #endif
	//��־����
	pTemp = osal_strchr(trapInfo, '#');
	if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp++;  

    pTemp2 = osal_strchr(trapInfo, '*');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp2++;
    typelen = pTemp2 - pTemp - 1;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_logType_oid, OID_LENGTH(snmptrap_logType_oid),
        ASN_OCTET_STR,
        (void *)pTemp, typelen);

    //snmp_log(LOG_DEBUG, "trap info: %s \n", pTemp2);     
    //��־����
    //�޸��û�
    pTemp3 = osal_strchr(trapInfo, '$');
    if (NULL != pTemp3)
    {
		pid = atoi(pTemp2);
   		osal_memset(&session, 0, sizeof(sys_line_session_t));
    	sal_sys_lineSession_by_pid_get(pid, &session, typestr);
    	if(osal_strcmp(typestr, "snmp") != 0)
	    {	
	    	pTemp3++;
			osal_sprintf(buf,"%s user:%s %s :%s", typestr, session.user, session.host, pTemp3);
	    }
	    else
	    {
			pTemp3++;
			osal_sprintf(buf,"snmp user :%s", pTemp3);
	    }
    }
    else
    { 
		osal_sprintf(buf,"%s", pTemp2);
	}
   	//snmp_log(LOG_DEBUG, "send trap: %s \n", buf);
    snmp_varlist_add_variable(&var_list,
        op_user_msg_oid, OID_LENGTH(op_user_msg_oid),
        ASN_OCTET_STR,
        (void *)buf, osal_strlen(buf));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}


int32 snmp_trap_poe(char *trapInfo)
{
    char *pTemp = NULL;
    netsnmp_variable_list  *var_list = NULL;
	char *pTemp2 = NULL;
    uint32 typelen = 0;
    
    pTemp = osal_strchr(trapInfo, ' ');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;

    pTemp++;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        sys_poe_trap_oid, sizeof(sys_poe_trap_oid));
        
	//��־ʱ��
	snmp_varlist_add_variable(&var_list,
        snmptrap_logTime_oid, OID_LENGTH(snmptrap_logTime_oid),
        ASN_OCTET_STR,
        (void *)pTemp, 25);

	//��־����
	pTemp = osal_strchr(trapInfo, '#');
	if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp++;  

    pTemp2 = osal_strchr(trapInfo, '*');
    if (NULL == pTemp)
        return SYS_ERR_FAILED;
    pTemp2++;
    typelen = pTemp2 - pTemp - 1;
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_logType_oid, OID_LENGTH(snmptrap_logType_oid),
        ASN_OCTET_STR,
        (void *)pTemp, typelen);

    //��־����
    snmp_varlist_add_variable(&var_list,
       	sys_poe_msg_oid, OID_LENGTH(sys_poe_msg_oid),
        ASN_OCTET_STR,
        (void *)pTemp2, osal_strlen(pTemp2));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}



int32 snmp_trap_user_operate_new(snmp_trapFormat_t *trapInfo)
{
    netsnmp_variable_list  *var_list = NULL;
    char buf[516] = {0};
    
	oid sys_uc_mac_change_oid[] = { 1,3,6,1,4,1,49763,1,1,1 };
	
	oid sys_operate_msg_oid[] = { 1,3,6,1,4,1,49763,1,2,1 };
	oid sys_uc_vlan_change_oid[] = { 1,3,6,1,4,1,49763,1,1,2 };

	if(trapInfo == NULL)
    	return SYS_ERR_FAILED;

    //��־ʱ��
    #if 0
	snmp_varlist_add_variable(&var_list,
        sys_log_time_oid, OID_LENGTH(sys_log_time_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapTime, osal_strlen(trapInfo->trapTime));
    #endif  
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        sys_operate_msg_oid, sizeof(sys_operate_msg_oid));
    
    //��־����
    if(trapInfo->status1 == 1)
    {
    	
    	osal_sprintf(buf,"%s %s", trapInfo->trapUser, trapInfo->trapDetail);
    	snmp_varlist_add_variable(&var_list,
        sys_uc_vlan_change_oid, OID_LENGTH(sys_uc_vlan_change_oid),
        ASN_OCTET_STR,
        (void *)buf, osal_strlen(buf));
    }
    else if(trapInfo->status1 == 2)
    {
    	
    	osal_sprintf(buf,"%s %s", trapInfo->trapUser, trapInfo->trapDetail);
    	snmp_varlist_add_variable(&var_list,
        sys_uc_mac_change_oid, OID_LENGTH(sys_uc_mac_change_oid),
        ASN_OCTET_STR,
        (void *)buf, osal_strlen(buf));
    }


    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}



int32 snmp_trap_port_link_new(snmp_trapFormat_t *trapInfo)
{
    netsnmp_variable_list  *var_list = NULL;
    oid ifPortLinkUpStatus_oid[] = { 1, 3, 6, 1, 6, 3, 1, 1, 5, 4};
    oid ifPortLinkDownStatus_oid[] = { 1, 3, 6, 1, 6, 3, 1, 1, 5, 3};
    oid ifPortIdValue_oid[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 1};
    oid ifPortconfigAdminStatus_oid[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 7};
    oid ifPortLinkOperStatus_oid[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 8};
    oid ifSysTrapIpAddress_oid[] = { 1, 3, 6, 1, 6, 3, 18, 1, 3, 0};
    
	if(trapInfo == NULL)
    	return SYS_ERR_FAILED;
    #if 0	
	//��־ʱ��
	snmp_varlist_add_variable(&var_list,
        sys_log_time_oid, OID_LENGTH(sys_log_time_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapTime, osal_strlen(trapInfo->trapTime));
    #endif
    //trap oid    
	if(trapInfo->status1 == SYSPORTLINKSTATUSUP)
	{
		snmp_varlist_add_variable(&var_list,
		snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
		ASN_OBJECT_ID,
		ifPortLinkUpStatus_oid, sizeof(ifPortLinkUpStatus_oid));
	}
	else
	{
		snmp_varlist_add_variable(&var_list,
		snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
		ASN_OBJECT_ID,
		ifPortLinkDownStatus_oid, sizeof(ifPortLinkDownStatus_oid));
	}
	//�˿ں�
	snmp_varlist_add_variable(&var_list,
	ifPortIdValue_oid, OID_LENGTH(ifPortIdValue_oid),
	ASN_INTEGER,
	&(trapInfo->portId), sizeof(trapInfo->portId));
	//����״̬
	snmp_varlist_add_variable(&var_list,
	ifPortconfigAdminStatus_oid, OID_LENGTH(ifPortconfigAdminStatus_oid),
	ASN_INTEGER,
	&(trapInfo->status2), sizeof(trapInfo->status2));
	//����״̬
	snmp_varlist_add_variable(&var_list,
	ifPortLinkOperStatus_oid, OID_LENGTH(ifPortLinkOperStatus_oid),
	ASN_INTEGER,
	&(trapInfo->status1), sizeof(trapInfo->status1));
	//ϵͳIP
	snmp_varlist_add_variable(&var_list,
        ifSysTrapIpAddress_oid, OID_LENGTH(ifSysTrapIpAddress_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->sysIp, osal_strlen(trapInfo->sysIp));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}

int32 snmp_trap_user_operation_new(snmp_trapFormat_t *trapInfo)
{
    netsnmp_variable_list  *var_list = NULL;
    oid ifSysOperation_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 2, 3};  
	oid ifSysUserIndex_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 3, 1, 1};
	oid ifSysUserName_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 3, 1, 2};
	oid ifSysUserIp_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 3, 1, 3};
	oid ifSysUserDetail_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 3, 1, 4};
	uint32 tmpNum = 1;
    
	if(trapInfo == NULL)
    	return SYS_ERR_FAILED;
    #if 0	
	//��־ʱ��
	snmp_varlist_add_variable(&var_list,
        sys_log_time_oid, OID_LENGTH(sys_log_time_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapTime, osal_strlen(trapInfo->trapTime));
    #endif
    //trap oid    
	snmp_varlist_add_variable(&var_list,
	snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
	ASN_OBJECT_ID,
	ifSysOperation_oid, sizeof(ifSysOperation_oid));

	snmp_varlist_add_variable(&var_list,
	ifSysUserIndex_oid, OID_LENGTH(ifSysUserIndex_oid),
	ASN_INTEGER,
	&tmpNum, sizeof(uint32));
	
	snmp_varlist_add_variable(&var_list,
    ifSysUserName_oid, OID_LENGTH(ifSysUserName_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->trapUser, osal_strlen(trapInfo->trapUser));

    snmp_varlist_add_variable(&var_list,
    ifSysUserIp_oid, OID_LENGTH(ifSysUserIp_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->sysIp, osal_strlen(trapInfo->sysIp));

	snmp_varlist_add_variable(&var_list,
    ifSysUserDetail_oid, OID_LENGTH(ifSysUserDetail_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->trapDetail, osal_strlen(trapInfo->trapDetail));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}


int32 snmp_trap_user_operation_info_new(snmp_trapFormat_t *trapInfo)
{
    netsnmp_variable_list  *var_list = NULL;
    oid ifSysOperation_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 2, 3};  
	oid ifSysUserIndex_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 3, 1, 1};
	oid ifSysUserName_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 3, 1, 2};
	oid ifSysUserIp_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 3, 1, 3};
	oid ifSysUserCmd_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 3, 1, 4};
	uint32 tmpNum = 1;
    
	if(trapInfo == NULL)
    	return SYS_ERR_FAILED;
    	
	//��־ʱ��
	#if 0
	snmp_varlist_add_variable(&var_list,
        sys_log_time_oid, OID_LENGTH(sys_log_time_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapTime, osal_strlen(trapInfo->trapTime));
    #endif
    //trap oid    
	snmp_varlist_add_variable(&var_list,
	snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
	ASN_OBJECT_ID,
	ifSysOperation_oid, sizeof(ifSysOperation_oid));

	snmp_varlist_add_variable(&var_list,
	ifSysUserIndex_oid, OID_LENGTH(ifSysUserIndex_oid),
	ASN_INTEGER,
	&tmpNum, sizeof(uint32));
	
	snmp_varlist_add_variable(&var_list,
    ifSysUserName_oid, OID_LENGTH(ifSysUserName_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->trapUser, osal_strlen(trapInfo->trapUser));

    snmp_varlist_add_variable(&var_list,
    ifSysUserIp_oid, OID_LENGTH(ifSysUserIp_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->sysIp, osal_strlen(trapInfo->sysIp));

	snmp_varlist_add_variable(&var_list,
    ifSysUserCmd_oid, OID_LENGTH(ifSysUserCmd_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->trapDetail, osal_strlen(trapInfo->trapDetail));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}





int32 snmp_trap_login_status_new(snmp_trapFormat_t *trapInfo)
{
    netsnmp_variable_list  *var_list = NULL;
    oid ifSysLogin_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 2, 2};
	oid ifSysUserIndex_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 1};
	oid ifSysUserName_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 2};
	oid ifSysUserLoginType_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 3};
	oid ifSysUserLoginStatus_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 4};
	oid ifSysUserLoginPW_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 5};
	oid ifSysUserLoginIp_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 6};
	uint32 tmpNum = 1;
	char tmpStrVal[6] = "None";
    
	if(trapInfo == NULL)
    	return SYS_ERR_FAILED;
    	
	//��־ʱ��
	#if 0
	snmp_varlist_add_variable(&var_list,
        sys_log_time_oid, OID_LENGTH(sys_log_time_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapTime, osal_strlen(trapInfo->trapTime));
    #endif
    //trap oid    

	snmp_varlist_add_variable(&var_list,
	snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
	ASN_OBJECT_ID,
	ifSysLogin_oid, sizeof(ifSysLogin_oid));

	snmp_varlist_add_variable(&var_list,
	ifSysUserIndex_oid, OID_LENGTH(ifSysUserIndex_oid),
	ASN_INTEGER,
	&tmpNum, sizeof(uint32));

	snmp_varlist_add_variable(&var_list,
    ifSysUserName_oid, OID_LENGTH(ifSysUserName_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->trapUser, osal_strlen(trapInfo->trapUser));

	snmp_varlist_add_variable(&var_list,
	ifSysUserLoginType_oid, OID_LENGTH(ifSysUserLoginType_oid),
	ASN_INTEGER,
	&trapInfo->status1, sizeof(uint32));

	snmp_varlist_add_variable(&var_list,
	ifSysUserLoginStatus_oid, OID_LENGTH(ifSysUserLoginStatus_oid),
	ASN_INTEGER,
	&trapInfo->status2, sizeof(uint32));

	snmp_varlist_add_variable(&var_list,
    ifSysUserLoginPW_oid, OID_LENGTH(ifSysUserLoginPW_oid),
    ASN_OCTET_STR,
    (void *)tmpStrVal, osal_strlen(tmpStrVal));

    snmp_varlist_add_variable(&var_list,
    ifSysUserLoginIp_oid, OID_LENGTH(ifSysUserLoginIp_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->sysIp, osal_strlen(trapInfo->sysIp));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}


int32 snmp_trap_password_change_new(snmp_trapFormat_t *trapInfo)
{
    netsnmp_variable_list  *var_list = NULL;
    oid ifSysLogin_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 2, 2};
	oid ifSysUserIndex_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 1};
	oid ifSysUserName_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 2};
	oid ifSysUserLoginType_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 3};
	oid ifSysUserLoginStatus_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 4};
	oid ifSysUserLoginPW_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 5};
	oid ifSysUserLoginIp_oid[] = { 1, 3, 6, 1, 4, 1, 49763, 1, 1, 2, 1, 6};
	uint32 tmpNum = 1;
    //char buf[SYS_BUF128_LEN];
	if(trapInfo == NULL)
    	return SYS_ERR_FAILED;
    	
	//��־ʱ��
	#if 0
	snmp_varlist_add_variable(&var_list,
        sys_log_time_oid, OID_LENGTH(sys_log_time_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapTime, osal_strlen(trapInfo->trapTime));
    #endif
    //trap oid    

	snmp_varlist_add_variable(&var_list,
	snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
	ASN_OBJECT_ID,
	ifSysLogin_oid, sizeof(ifSysLogin_oid));

	snmp_varlist_add_variable(&var_list,
	ifSysUserIndex_oid, OID_LENGTH(ifSysUserIndex_oid),
	ASN_INTEGER,
	&tmpNum, sizeof(uint32));

	snmp_varlist_add_variable(&var_list,
    ifSysUserName_oid, OID_LENGTH(ifSysUserName_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->trapUser, osal_strlen(trapInfo->trapUser));

	snmp_varlist_add_variable(&var_list,
	ifSysUserLoginType_oid, OID_LENGTH(ifSysUserLoginType_oid),
	ASN_INTEGER,
	&trapInfo->status1, sizeof(uint32));

	snmp_varlist_add_variable(&var_list,
	ifSysUserLoginStatus_oid, OID_LENGTH(ifSysUserLoginStatus_oid),
	ASN_INTEGER,
	&trapInfo->status2, sizeof(uint32));

	//osal_sprintf(buf , "%s password was modified", trapInfo->trapUser);
	snmp_varlist_add_variable(&var_list,
    ifSysUserLoginPW_oid, OID_LENGTH(ifSysUserLoginPW_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->trapUser, osal_strlen(trapInfo->trapUser));
   
    snmp_varlist_add_variable(&var_list,
    ifSysUserLoginIp_oid, OID_LENGTH(ifSysUserLoginIp_oid),
    ASN_OCTET_STR,
    (void *)trapInfo->sysIp, osal_strlen(trapInfo->sysIp));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}


int32 snmp_trap_system_start_new(snmp_trapFormat_t *trapInfo)
{
    char *pTemp = NULL;
    netsnmp_variable_list  *var_list = NULL;
	char *pTemp2 = NULL;
    uint32 typelen = 0;
	if(trapInfo == NULL)
    	return SYS_ERR_FAILED;
	//snmp_log(LOG_DEBUG, "%s %d time=%s type=%s  detail=%s\n", __func__, __LINE__, trapInfo->trapTime, trapInfo->trapType , trapInfo->trapDetail);

    

    snmp_varlist_add_variable(&var_list,
    snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
    ASN_OBJECT_ID,
    op_user_trap_oid, sizeof(op_user_trap_oid));
    
    snmp_varlist_add_variable(&var_list,
        snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
        ASN_OBJECT_ID,
        sys_poe_trap_oid, sizeof(sys_poe_trap_oid));

	//��־ʱ��
	#if 0
	snmp_varlist_add_variable(&var_list,
        snmptrap_logTime_oid, OID_LENGTH(snmptrap_logTime_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapTime, osal_strlen(trapInfo->trapTime));
    #endif
    //��־����
    snmp_varlist_add_variable(&var_list,
        snmptrap_logType_oid, OID_LENGTH(snmptrap_logType_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapType, osal_strlen(trapInfo->trapType));

    //snmp_log(LOG_DEBUG, "trap info: %s \n", pTemp2);     


    //��־����
    snmp_varlist_add_variable(&var_list,
       	snmptrap_logType_oid, OID_LENGTH(snmptrap_logType_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapDetail, osal_strlen(trapInfo->trapDetail));
	 //snmp_log(LOG_DEBUG, "send trap detail: %s \n", trapInfo->trapDetail);

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}

int32 snmp_trap_system_reboot_new(snmp_trapFormat_t *trapInfo)
{
    netsnmp_variable_list  *var_list = NULL; 
    oid ifSysReboot_oid[] = { 1,3,6,1,6,3,1,1,5,1};
    oid ifSysRebootStr_oid[] = { 1,3,6,1,6,3,1,1,4,3,0};
	if(trapInfo == NULL)
    	return SYS_ERR_FAILED;
	//snmp_log(LOG_DEBUG, "%s %d time=%s type=%s  detail=%s\n", __func__, __LINE__, trapInfo->trapTime, trapInfo->trapType , trapInfo->trapDetail);

    
	//��־ʱ��
	#if 0
	snmp_varlist_add_variable(&var_list,
		snmptrap_logTime_oid, OID_LENGTH(snmptrap_logTime_oid),
		ASN_OCTET_STR,
		(void *)trapInfo->trapTime, osal_strlen(trapInfo->trapTime));
    #endif
    snmp_varlist_add_variable(&var_list,
    snmptrap_std_oid, OID_LENGTH(snmptrap_std_oid),
    ASN_OBJECT_ID,
    ifSysReboot_oid, sizeof(ifSysReboot_oid));

    //��־����
    snmp_varlist_add_variable(&var_list,
       	ifSysRebootStr_oid, OID_LENGTH(ifSysRebootStr_oid),
        ASN_OCTET_STR,
        (void *)trapInfo->trapDetail, osal_strlen(trapInfo->trapDetail));

    send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, var_list);
    snmp_free_varbind(var_list );

    return SYS_ERR_OK;
}



