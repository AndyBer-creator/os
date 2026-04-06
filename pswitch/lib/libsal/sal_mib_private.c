
#include <libsal/sal_util.h>
#include <libsal/sal_mib_private.h>
#include "libsal/sal_sys.h"
#ifdef CONFIG_SYS_POE 
#include "libsal/sal_poe.h"
#endif
#include <libcfg/cfg_port.h>
#include <libcmd/cmd_conf.h>

static staticVlan_t sVlan [CAPA_VLAN_NUM];
static sys_vlanmask_t existVlan;

void sal_private_oid_oem_init(uint32* Oid)
{
	sys_boardModel_t boardModel;

	SYS_MEM_CLEAR(boardModel);
    sal_sys_boardModel_get(&boardModel);
    memcpy(Oid, boardModel.oid.oidAry, sizeof(uint32)*boardModel.oid.oidLen);
}

#ifdef CONFIG_SYS_POE 

uint32 sal_mib_poe_port_total_num_get(void)
{
	uint32 g_poePortNum = 0;
	SYS_ERR_CHK(sal_poe_port_total_num_get( &g_poePortNum));
	return g_poePortNum;
}

uint32 sal_mib_poe_power_supply_total_get(int poePortNum)
{
	uint32 globalPower = 0;
	
	SYS_ERR_CHK(sal_poe_currentPower_sum_get(&globalPower));

    return globalPower;
}

uint32 sal_mib_poe_port_power_get(int port)
{
 	sal_poe_ioctl_t data;  

 	osal_memset(&data, 0, sizeof(sal_poe_ioctl_t));
 	SYS_ERR_CHK(sal_poe_port_info_get(port, &data));
    return data.portPower; 
}

uint32 sal_mib_poe_port_status_get(int port)
{
 	sal_poe_ioctl_t data;  

 	osal_memset(&data, 0, sizeof(sal_poe_ioctl_t));
 	SYS_ERR_CHK(sal_poe_port_info_get(port, &data));
    return data.portStatus; 
}

uint32 sal_mib_poe_port_Volatge_get(int port)
{
 	sal_poe_ioctl_t data;  

 	osal_memset(&data, 0, sizeof(sal_poe_ioctl_t));
 	SYS_ERR_CHK(sal_poe_port_info_get(port, &data));
    return data.portVolatge; 
}

uint32 sal_mib_poe_port_Current_get(int port)
{
 	sal_poe_ioctl_t data;  

 	osal_memset(&data, 0, sizeof(sal_poe_ioctl_t));
 	SYS_ERR_CHK(sal_poe_port_info_get(port, &data));
    return data.portCurrent; 
}

uint32 sal_mib_poe_port_reboot_timer_get(int port, char* pRestrTime, char* pDestrTime)
{
 	sal_poe_time_t rebootTime;
	sal_poe_time_t delayTime;

	SYS_PARAM_CHK((NULL == pRestrTime), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == pDestrTime), SYS_ERR_NULL_POINTER);
	
	SYS_MEM_CLEAR(rebootTime);
	SYS_MEM_CLEAR(delayTime);

 	SYS_ERR_CHK(sal_poe_portAssignTime_work_get(port, &rebootTime, &delayTime));
 	osal_sprintf(pRestrTime, "%02u:%02u:%02u", rebootTime.hour, rebootTime.minute, rebootTime.second);
 	osal_sprintf(pDestrTime, "%02u:%02u:%02u", delayTime.hour, delayTime.minute, delayTime.second);
 	
    return SYS_ERR_OK; 
}

uint32 sal_mib_poe_port_reboot_timer_set(int port, char* pRestrTime, char* pDestrTime)
{
 	sal_poe_time_t rebootTime;
	sal_poe_time_t delayTime;

	SYS_MEM_CLEAR(rebootTime);
	SYS_MEM_CLEAR(delayTime);

	SYS_ERR_CHK(sal_poe_portAssignTime_work_get(port, &rebootTime, &delayTime));
	
	if(NULL != pRestrTime)
	{
		SYS_ERR_CHK(sal_strtime_to_hour_min_sec(pRestrTime, &rebootTime.hour, &rebootTime.minute, &rebootTime.second));
	}
	
	if(NULL != pDestrTime)
	{
		SYS_ERR_CHK(sal_strtime_to_hour_min_sec(pDestrTime, &delayTime.hour, &delayTime.minute, &delayTime.second));
	}
	
 	SYS_ERR_CHK(sal_poe_portAssignTime_work_set(port, rebootTime, delayTime));
    return SYS_ERR_OK; 
}

#endif 

int32 sal_mib_memory_info_get(int cmd, unsigned long* value)
{
	sys_mem_info_t memInfo;

	osal_memset(&memInfo, 0, sizeof(sys_mem_info_t));
	SYS_ERR_CHK(sal_sys_memInfo_get(&memInfo)); 
	switch(cmd)
	{
		case CMD_MEMORYTOTAL:
			*value = memInfo.mem_total;
			break;
		case CMD_MEMORYUSED:
			*value = ((double)(memInfo.mem_total - memInfo.mem_free - memInfo.mem_buffer - memInfo.mem_cached) / (double)memInfo.mem_total) * 100;
			break;
		case CMD_MEMORYFREE:
			*value = memInfo.mem_free;
			break;
		default:
			break;
	}
	return SYS_ERR_OK;
}

int32 sal_mib_cpu_info_get(int cmd, void* pValue)
{

	switch(cmd)
	{
		case CMD_CPUNAME:
			osal_sprintf(pValue, "%s", "MIPS");
			break;
		case CMD_CPUUSED:
			SYS_ERR_CHK(sal_sys_cpuUtilz_sw_get(pValue));
			break;
		case CMD_CPUTEMPERATURE:
			*(uint32*)pValue = 0;
			break;
		default:
			break;
	}
	return SYS_ERR_OK;
}

int32 sal_mib_interface_type_info_get(uint32 port, int* value)
{
#define COPPER_TYPE   1
#define FIBER_TYPE    2
#define STACK_TYPE    3
	sys_port_media_t intfType;
	
	SYS_ERR_CHK(sal_port_linkMedia_get(port, &intfType)); 
	switch(intfType)
	{
		case PORT_MEDIA_COPPER:
			*value = COPPER_TYPE;
			break;
		case PORT_MEDIA_FIBER:
			*value = FIBER_TYPE;
			break;
	#if 0  
		case CMD_STACKEDTYPE:
			*value = STACK_TYPE;
			break;
	#endif
		default:
			*value = 0;
			break;
	}
	return SYS_ERR_OK;
}

uint32 sal_mib_poe_func_get(uint32* pValue)
{
#define POE_FUNC_ON    1
#define POE_FUNC_OFF   0
	static int32 poe_func = 0;
	#ifdef CONFIG_SYS_POE
	poe_func = POE_FUNC_ON;
	#else
	poe_func = POE_FUNC_OFF;	
	#endif
	*pValue = poe_func;
	return SYS_ERR_OK;
}

 int32 sal_mib_port_ctrl_get(uint32 port, uint32 cmd, uint32* value)
 {
 	sys_enable_t enable;
 	uint32 rate;
 	sys_vlan_portMode_t mode;
 		
	switch(cmd)
	{
		case CMD_PORTLIMITINGRESS:
			sal_rate_igrBandwidthCtrlEnable_get(port, &enable);
			if(enable)
			{
				sal_rate_igrBandwidthCtrlRate_get(port, &rate);
				*value = rate;
			}
			else 
			{
				*value = 0;
			}
			break;
		case CMD_PORTLIMITEGRESS:
			sal_rate_egrBandwidthCtrlEnable_get(port, &enable);
			if(enable)
			{
				sal_rate_egrBandwidthCtrlRate_get(port, &rate);
				*value = rate;
			}
			else 
			{
				*value = 0;
			}
			break;
		case CMD_PORTVLANMODE:
			sal_vlan_portAdminMode_get(port, &mode);
			if(mode == SYS_VLAN_PORT_MODE_GENERAL)
			{
				*value = MIB_VLANMODE_HYBRID;
			}
			else if(mode == SYS_VLAN_PORT_MODE_ACCESS)
			{
				*value = MIB_VLANMODE_ACCESS;
			}
			else if(mode == SYS_VLAN_PORT_MODE_TRUNK)
			{
				*value = MIB_VLANMODE_TRUNK;
			}
			else 
			{
				*value = MIB_VLANMODE_TUNNEL;
			}	
			break;
		case CMD_PORTPROTECTED:
			sal_port_protected_get(port, &enable);
			if(enable)
			{
				*value = MIB_ENABLE;
			}
			else
			{
				*value = MIB_DISABLE;
			}
			break;
		default:
			*value = 0;
			break;
	}
	return SYS_ERR_OK;
 }

 int32 sal_mib_port_ctrl_set(uint32 port, uint32 cmd, uint32 value)
 {
 	sys_vlan_portMode_t mode, oldmode;
	sys_enable_t enable, oldenable;

	switch(cmd)
	{
		case CMD_PORTLIMITINGRESS:
			if(value)
			{
				sal_rate_igrBandwidthCtrlRate_set(port, value);
				sal_rate_igrBandwidthCtrlEnable_set(port, TRUE);
			}
			else
			{
				sal_rate_igrBandwidthCtrlEnable_set(port, FALSE);
			}
			break;
		case CMD_PORTLIMITEGRESS:
			if(value)
			{
				sal_rate_egrBandwidthCtrlRate_set(port, value);
				sal_rate_egrBandwidthCtrlEnable_set(port, TRUE);
			}
			else 
			{
				sal_rate_egrBandwidthCtrlEnable_set(port, FALSE);
			}
			break;
		case CMD_PORTVLANMODE:
			if(value == MIB_VLANMODE_HYBRID)
			{
				mode = SYS_VLAN_PORT_MODE_GENERAL;
			}
			else if(value == MIB_VLANMODE_ACCESS)
			{
				mode = SYS_VLAN_PORT_MODE_ACCESS;
			}
			else if(value == MIB_VLANMODE_TRUNK)
			{
				mode = SYS_VLAN_PORT_MODE_TRUNK;
			}
			else 
			{
				mode = SYS_VLAN_PORT_MODE_DOT1QTUNNEL;
			}
			sal_vlan_portAdminMode_get(port, &oldmode);
			if(oldmode != mode)
			{
				sal_vlan_portAdminMode_set(port, mode);
			}
			break;
		case CMD_PORTPROTECTED:
			if(value == MIB_ENABLE)
			{
				enable = ENABLED;
			}
			else
			{
				enable = DISABLED;
			}
			sal_port_protected_get(port, &oldenable);
			if(oldenable != enable)
				sal_port_protected_set(port, enable);
			break;
		default:
			value = 0;
			break;
	}
	return SYS_ERR_OK;
 }

 int32 sal_mib_port_Attribute_get(uint32 port, uint32 cmd, uint32* value)
 {
 	sys_port_duplex_t portDuplex;
 	sys_port_speed_t portSpeed;
 		
	switch(cmd)
	{
		case CMD_PORTSPEED:
			sal_port_speed_get(port, &portSpeed);
			if (ENABLED == portSpeed.autoSpeed)
            {
            	switch(portSpeed.ability)
            	{
					case SYS_PORT_SPEED_10M:
						*value = MIB_SPEED_AUTO_10M;
						break;
					case SYS_PORT_SPEED_100M:
						*value = MIB_SPEED_AUTO_100M;
						break;
					case SYS_PORT_SPEED_1000M:
						*value = MIB_SPEED_AUTO_1000M;
						break;
					case SYS_PORT_SPEED_10M_100M:
						*value = MIB_SPEED_AUTO_10M_100M;
						break;
					case SYS_PORT_SPEED_10G:
						*value = MIB_SPEED_AUTO_10G;
						break;
					case SYS_PORT_SPEED_ALL:
						*value = MIB_SPEED_AUTO;	
						break;
					default:
						break;
            	}
            }
            else
            {
				switch (portSpeed.ability)
				{
					case SYS_PORT_SPEED_10G:
						*value = MIB_SPEED_10G;
						break;
				
					case SYS_PORT_SPEED_1000M:
						*value = MIB_SPEED_1000M;
						break;
				
					case SYS_PORT_SPEED_100M:
						*value = MIB_SPEED_100M;
						break;
				
					case SYS_PORT_SPEED_10M:
						*value = MIB_SPEED_10M;
						break;		
					default:
						break;
				}
            }
        	break;
		case CMD_PORTDUPLEX:
			sal_port_duplex_get(port, &portDuplex);	
			if(portDuplex.autoDuplex == ENABLED)
			{
				*value = MIB_DUPLEX_AUTO;
			}
			else
			{
				if (SYS_PORT_DUPLEX_FULL == portDuplex.ability)	
					*value = MIB_DUPLEX_FULL;
				else
					*value = MIB_DUPLEX_HALF;
			}
			break;
		default:
			*value = 0;
			break;
	}
	return SYS_ERR_OK;
 }

 int32 sal_mib_port_Attribute_set(uint32 port, uint32 cmd, uint32 value)
 {
 	sys_port_speed_t speed, oldspeed;
 	sys_port_duplex_t duplex, oldduplex;

	switch(cmd)
	{
		case CMD_PORTSPEED:	
			if(value & MIB_SPEED_AUTO)
			{
				speed.autoSpeed = ENABLED;
				if(value == MIB_SPEED_AUTO)
				{
					speed.ability = SYS_PORT_SPEED_ALL;
				}
				else if(value == MIB_SPEED_AUTO_10M)
				{
					speed.ability = SYS_PORT_SPEED_10M;
				}
				else if(value == MIB_SPEED_AUTO_100M)
				{
					speed.ability = SYS_PORT_SPEED_100M;
				}
				else if(value == MIB_SPEED_AUTO_1000M)
				{
					speed.ability = SYS_PORT_SPEED_1000M;
				}
			}
			else
			{
				speed.autoSpeed = DISABLED;
				if(value == MIB_SPEED_10M)
				{
					speed.ability = SYS_PORT_SPEED_10M;
				}
				else if(value == MIB_SPEED_100M)
				{
					speed.ability = SYS_PORT_SPEED_100M;
				}
				else if(value == MIB_SPEED_1000M)
				{
					speed.ability = SYS_PORT_SPEED_1000M;
				}
				else if(value == MIB_SPEED_10G)
				{
					speed.ability = SYS_PORT_SPEED_10G;
				}
			}
			sal_port_speed_get(port, &oldspeed);
			if(oldspeed.autoSpeed != speed.autoSpeed || oldspeed.ability != speed.ability)
			{
				SYS_ERR_CHK(sal_port_speed_set(port, speed));
			}
			break;
		case CMD_PORTDUPLEX:
			osal_memset(&duplex, 0, sizeof(duplex));
			if(MIB_DUPLEX_FULL == value)
			{
				duplex.autoDuplex = DISABLED;
				duplex.ability = SYS_PORT_DUPLEX_FULL;
			}
			else if(MIB_DUPLEX_HALF == value)
			{
				duplex.autoDuplex = DISABLED;
				duplex.ability = SYS_PORT_DUPLEX_HALF;
			}
			else
			{
				duplex.autoDuplex = ENABLED;
			}
			sal_port_duplex_get(port, &oldduplex);
			if(oldduplex.autoDuplex != duplex.autoDuplex || oldduplex.ability != duplex.ability) 
				sal_port_duplex_set(port, duplex);
			break;
		default:
			value = 0;
			break;
	}
	return SYS_ERR_OK;
 }

int32 sal_mib_last_5_rate_set(sys_logic_port_t port, int type, uint64 pResult)
{
	SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

	switch(type)
	{
		case TYPE_IN_PKT_RATE:
			cfg_port_in_pkt_rate_set(port, TYPE_IN_PKT_RATE, pResult);
			break;
		case TYPE_IN_BYTE_RATE:
			cfg_port_in_pkt_rate_set(port, TYPE_IN_BYTE_RATE, pResult);
			break;
		case TYPE_OUT_PKT_RATE:
			cfg_port_in_pkt_rate_set(port, TYPE_OUT_PKT_RATE, pResult);
			break;
		case TYPE_OUT_BYTE_RATE:
			cfg_port_in_pkt_rate_set(port, TYPE_OUT_BYTE_RATE, pResult);
			break;
		default:
			break;
	}
	
    return SYS_ERR_OK;
}

int32 sal_mib_last_5_rate_get(sys_logic_port_t port, int type, uint64 *pResult)
{
	SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pResult, SYS_ERR_NULL_POINTER);
	
	switch(type)
	{
		case TYPE_IN_PKT_RATE:
			cfg_port_in_pkt_rate_get(port, TYPE_IN_PKT_RATE, pResult);
			break;
		case TYPE_IN_BYTE_RATE:
			cfg_port_in_pkt_rate_get(port, TYPE_IN_BYTE_RATE, pResult);
			break;
		case TYPE_OUT_PKT_RATE:
			cfg_port_in_pkt_rate_get(port, TYPE_OUT_PKT_RATE, pResult);
			break;
		case TYPE_OUT_BYTE_RATE:
			cfg_port_in_pkt_rate_get(port, TYPE_OUT_BYTE_RATE, pResult);
			break;
		default:
			break;
	}
	
    return SYS_ERR_OK;
}

uint32 sal_mib_device_ip_get(int type, uint32* pValve)
{
	sys_l3_intfMgmt_t intfMgmt; 

	SYS_MEM_CLEAR(intfMgmt);
	sal_l3_intfMgmt_get( &intfMgmt);

	switch(type)
	{
		case cmd_ipaddr:
			*pValve = intfMgmt.ipv4.addr;
			break;
		case cmd_ipmask:
			*pValve = IPV4_PFX2MASK(intfMgmt.ipv4.plen);
			break;
		default:
			break;
	}
	return SYS_ERR_OK;
}

static char tftpServerIp[CAPA_IPV4_STR_LEN] = {0};
static char remoteFile[128] = {0};
static uint32 img_upgrade = 0;
uint32 sal_mib_file_upgrade_set(int type, void* value)
{
    int32 ret = SYS_ERR_OK;
    FILE *fp = NULL;
    uint32 currSelPart = 0;
	char localFile[256] = {0};
	switch(type)
	{
		case SYS_TFTPSERVER:
			SYS_MEM_CLEAR(tftpServerIp);
			strncpy(tftpServerIp, value, sizeof(tftpServerIp));
			return SYS_ERR_OK;
		case SYS_FILENAME:
			SYS_MEM_CLEAR(remoteFile);
			strncpy(remoteFile, value, sizeof(remoteFile));
			return SYS_ERR_OK;
		case SYS_IMGUPGRADE:
			img_upgrade = 1;
	        ret =  sal_sys_fwImageLock_check();

	        if (SYS_ERR_OK != ret)
	            return SYS_ERR_OK;

	        sal_sys_currActiveImageIdx_get(&currSelPart);

	        if (NULL == (fp =  fopen("/tmp/tftp_write2partition", "w")))
	        {
	            sal_sys_fwImageSaveMem_gen(TMP_FWIMG_FILENAME);
	            ret =  SYS_ERR_FILE_OPEN;
	             fclose(fp);
	        }

	    #ifdef CONFIG_SYS_DUAL_IMAGE
	        fprintf(fp, "%s %% %s %% %d", tftpServerIp, remoteFile, !currSelPart);
	    #else
	        fprintf(fp, "%s %% %s %% %d", tftpServerIp, remoteFile, currSelPart);
	    #endif
	        fclose(fp);

	        if (NULL != (fp = fopen(SYS_POLLD_ACT_CUSTOM, "w")))
	        {
	            fprintf(fp, "handler -f web_tftp_action dl_fw &");
	            fclose(fp);
	        }
	        DO_POLLD_ACT_SIG();
	        img_upgrade = TRUE;
			break;
		case SYS_UPLOAD:
			if(*(uint32*)value == SYS_UPLOAD_RUN_CONFIG)
			{
				osal_sprintf(localFile, CONF_RUNNING_FILE);
			}
			else if(*(uint32*)value == SYS_UPLOAD_STARTUP_CONFIG)
			{
				osal_sprintf(localFile, CONF_STARTUP_FILE);
			}
			else if(*(uint32*)value == SYS_UPLOAD_CLONE_CONFIG)
			{
				osal_sprintf(localFile, CONF_CLONE_FILE);
			}
			ret = sal_util_file_exist(CONF_TFTP_UPLOAD_CFG_FILE);
			if(ret != SYS_ERR_FILE_NOT_EXIST)
			{
				unlink(CONF_TFTP_UPLOAD_CFG_FILE);
			}
			
			if (NULL == (fp =  fopen(CONF_TFTP_UPLOAD_CFG_FILE, "w")))
	        {
	            fclose(fp);
	        }
	        fprintf(fp, "%s %% %s %% %s", tftpServerIp, remoteFile, localFile);
	        fclose(fp);

	        if (NULL == (fp =  fopen(CONF_TFTP_UPLOAD_STATU_FILE, "w")))
	        {
	            fclose(fp);
	        }
	        fclose(fp);
	        
	        system("handler -f mibtftpuploadconfig 1> /dev/null 2> /dev/null &");
			break;
		case SYS_DOWNLOAD:
			if(*(uint32*)value == SYS_DOWNLOAD_RUN_CONFIG)
			{
				osal_sprintf(localFile, CONF_RUNNING_TMPFILE);
			}
			else if(*(uint32*)value == SYS_DOWNLOAD_STARTUP_CONFIG)
			{
				osal_sprintf(localFile, CONF_STARTUP_TMPFILE);
			}
			else if(*(uint32*)value == SYS_DOWNLOAD_CLONE_CONFIG)
			{
				osal_sprintf(localFile, CONF_CLONE_TMPFILE);
			}
			
			ret = sal_util_file_exist(CONF_TFTP_DOWNLOAD_CFG_FILE);
			if(ret != SYS_ERR_FILE_NOT_EXIST)
			{
				unlink(CONF_TFTP_DOWNLOAD_CFG_FILE);
			}
			
			if (NULL == (fp =  fopen(CONF_TFTP_DOWNLOAD_CFG_FILE, "w")))
	        {
	            fclose(fp);
	        }
	        fprintf(fp, "%s %% %s %% %s", tftpServerIp, remoteFile, localFile);
	        fclose(fp);

	        if (NULL == (fp =  fopen(CONF_TFTP_DOWNLOAD_STATU_FILE, "w")))
	        {
	            fclose(fp);
	        }
	        fclose(fp);
	        
	        system("handler -f mibtftpdownloadconfig 1> /dev/null 2> /dev/null &");
			break;
		default:
			break;
	}

    return SYS_ERR_OK;
}

uint32 sal_mib_file_upgrade_get(int type, void* value)
{	
	FILE *fp = NULL;
	
	switch(type)
	{
		case SYS_TFTPSERVER:
			strncpy(value, tftpServerIp, sizeof(tftpServerIp));
			break;
		case SYS_FILENAME:
			strncpy(value, remoteFile, sizeof(remoteFile));
			break;
		case SYS_IMGUPGRADE:
			if(img_upgrade == 2)
				break;
				
			if((fp = fopen("/tmp/tftp_uploadfail", "r")) != NULL)
			{
				fclose(fp);
				unlink("/tmp/tftp_uploadfail");
				unlink("/tmp/tftp_uploadsize");
				img_upgrade = 3;
			}
			else if ((fp = fopen(TMP_FWIMG_WRITESTATUS, "r")) != NULL)
			{
				fclose(fp);
				unlink(TMP_FWIMG_WRITESTATUS);
				unlink("/tmp/tftp_uploadsize");
				img_upgrade = 2;
			}
			else 
			{
				if(img_upgrade == 1)	
					img_upgrade = 1;
			}
			*(uint32*)value = (uint32)img_upgrade;

			break;
		default:
			break;
	}

    return SYS_ERR_OK;
}

uint32 sal_mib_config_file_status_get(int type, int* status)
{
#define FAIL       3
#define SUCCEEDED  2
	int32 ret = SYS_ERR_FAILED;
	FILE *fp = NULL;
	char firstLine[SYS_BUF64_LEN];
	
	switch(type)
	{
			case SYS_BUILD_RUN_CONFIG:
				ret = sal_util_file_exist(CONF_RUNNING_TMPSTATUSFILE);
				if(SYS_ERR_FILE_NOT_EXIST == ret)
				{
					*status = 0;
				}
				else
				{
					if (NULL != (fp = fopen(CONF_RUNNING_TMPSTATUSFILE, "r")))
					{
						osal_memset(firstLine, 0, sizeof(firstLine));
					    fgets(firstLine, sizeof(firstLine), fp);
					    firstLine[strlen(firstLine)] = '\0';

					    fclose(fp);
					}
					
					if (0 == osal_strcmp(firstLine, "success"))
				    {
				        unlink(CONF_RUNNING_TMPSTATUSFILE);
				        *status = SUCCEEDED;
				    }
				    else if (0 == osal_strcmp(firstLine, "fail"))
				    {
				        unlink(CONF_RUNNING_TMPSTATUSFILE);
				        *status = FAIL;
				    }
				}		
				break;
			case SYS_BUILD_CLONE_CONFIG:
				ret = sal_util_file_exist(CONF_CLONE_TMPSTATUSFILE);
				if(SYS_ERR_FILE_NOT_EXIST == ret)
				{
					*status = 0;
				}
				else
				{
					if (NULL != (fp = fopen(CONF_CLONE_TMPSTATUSFILE, "r")))
					{
					    fgets(firstLine, sizeof(firstLine), fp);
					    firstLine[strlen(firstLine)] = '\0';
					    fclose(fp);
					}
					if (0 == osal_strcmp(firstLine, "success"))
				    {
				        unlink(CONF_CLONE_TMPSTATUSFILE);
				        *status = SUCCEEDED;
				    }
				    else if (0 == osal_strcmp(firstLine, "fail"))
				    {
				        unlink(CONF_CLONE_TMPSTATUSFILE);
				        *status = FAIL;
				    }
				}
				break;
			case SYS_CONFIG_SAVE_FLASH:
				ret = sal_util_file_exist(CONF_STARTUP_TMPSTATUFILE);
				if(SYS_ERR_FILE_NOT_EXIST == ret)
				{
					*status = 0;
				}
				else
				{
					if (NULL != (fp = fopen(CONF_STARTUP_TMPSTATUFILE, "r")))
					{
					    fgets(firstLine, sizeof(firstLine), fp);
					    firstLine[strlen(firstLine)] = '\0';
					    fclose(fp);
					}
					if (0 == osal_strcmp(firstLine, "success"))
				    {
				        unlink(CONF_STARTUP_TMPSTATUFILE);
				        *status = SUCCEEDED;
				    }
				    else if (0 == osal_strcmp(firstLine, "fail"))
				    {
				        unlink(CONF_STARTUP_TMPSTATUFILE);
				        *status = FAIL;
				    }
				}
				break;
			case SYS_CONFIG_STARTUP_TO_RUN:
				ret = sal_util_file_exist(CONF_STARTUP_TO_RUN_TMPSTATUFILE);
				if(SYS_ERR_FILE_NOT_EXIST == ret)
				{
					*status = 0;
				}
				else
				{
					if (NULL != (fp = fopen(CONF_STARTUP_TO_RUN_TMPSTATUFILE, "r")))
					{
					    fgets(firstLine, sizeof(firstLine), fp);
					    firstLine[strlen(firstLine)] = '\0';
					    fclose(fp);
					}
					if (0 == osal_strcmp(firstLine, "success"))
				    {
				        unlink(CONF_STARTUP_TO_RUN_TMPSTATUFILE);
				        *status = SUCCEEDED;
				    }
				    else if (0 == osal_strcmp(firstLine, "fail"))
				    {
				        unlink(CONF_STARTUP_TO_RUN_TMPSTATUFILE);
				        *status = FAIL;
				    }
				}
				break;
			case SYS_CONFIG_CLONE_TO_RUN:
				ret = sal_util_file_exist(CONF_ClONE_TO_RUN_TMPSTATUFILE);
				if(SYS_ERR_FILE_NOT_EXIST == ret)
				{
					*status = 0;
				}
				else
				{
					if (NULL != (fp = fopen(CONF_ClONE_TO_RUN_TMPSTATUFILE, "r")))
					{
					    fgets(firstLine, sizeof(firstLine), fp);
					    firstLine[strlen(firstLine)] = '\0';
					    fclose(fp);
					}
					if (0 == osal_strcmp(firstLine, "success"))
				    {
				        unlink(CONF_ClONE_TO_RUN_TMPSTATUFILE);
				        *status = SUCCEEDED;
				    }
				    else if (0 == osal_strcmp(firstLine, "fail"))
				    {
				        unlink(CONF_ClONE_TO_RUN_TMPSTATUFILE);
				        *status = FAIL;
				    }
				}
				break;
			case SYS_UPLOAD:
				ret = sal_util_file_exist(CONF_TFTP_UPLOAD_STATU_FILE);
				if(SYS_ERR_FILE_NOT_EXIST == ret)
				{
					*status = 0;
				}
				else
				{
					if (NULL != (fp = fopen(CONF_TFTP_UPLOAD_STATU_FILE, "r")))
					{
					    fgets(firstLine, sizeof(firstLine), fp);
					    firstLine[strlen(firstLine)] = '\0';
					    fclose(fp);
					}
					if (0 == osal_strcmp(firstLine, "success"))
				    {
				        unlink(CONF_TFTP_UPLOAD_STATU_FILE);
				        *status = SUCCEEDED;
				    }
				    else if (0 == osal_strcmp(firstLine, "fail"))
				    {
				        unlink(CONF_TFTP_UPLOAD_STATU_FILE);
				        *status = FAIL;
				    }
				}
				break;
			case SYS_DOWNLOAD:
				ret = sal_util_file_exist(CONF_TFTP_DOWNLOAD_STATU_FILE);
				if(SYS_ERR_FILE_NOT_EXIST == ret)
				{
					*status = 0;
				}
				else
				{
					if (NULL != (fp = fopen(CONF_TFTP_DOWNLOAD_STATU_FILE, "r")))
					{
					    fgets(firstLine, sizeof(firstLine), fp);
					    firstLine[strlen(firstLine)] = '\0';
					    fclose(fp);
					}
					if (0 == osal_strcmp(firstLine, "success"))
				    {
				        unlink(CONF_TFTP_DOWNLOAD_STATU_FILE);
				        *status = SUCCEEDED;
				    }
				    else if (0 == osal_strcmp(firstLine, "fail"))
				    {
				        unlink(CONF_TFTP_DOWNLOAD_STATU_FILE);
				        *status = FAIL;
				    }
				}
				break;
			default:
				break;
	}
	return SYS_ERR_OK;
}

uint32 sal_mib_build_config_file_set(int type)
{
	int32 ret = SYS_ERR_FAILED;
	FILE *fp = NULL;
	
	switch(type)
	{
			case SYS_BUILD_RUN_CONFIG:
				ret = sal_util_file_exist(CONF_RUNNING_TMPSTATUSFILE);
				if(ret == SYS_ERR_OK)
					unlink(CONF_RUNNING_TMPSTATUSFILE);
				
				fp = fopen(CONF_RUNNING_TMPSTATUSFILE, "w");
				fclose(fp);
				break;
			case SYS_BUILD_CLONE_CONFIG:
				ret = sal_util_file_exist(CONF_CLONE_TMPSTATUSFILE);
				if(ret == SYS_ERR_OK)
					unlink(CONF_CLONE_TMPSTATUSFILE);
				
				fp = fopen(CONF_CLONE_TMPSTATUSFILE, "w");
				fclose(fp);
				break;
			default:
				break;
	}
	return SYS_ERR_OK;
}

uint32 sal_mib_file_config_save_flash_set(void)
{
	int32 ret = SYS_ERR_FAILED;
	FILE *fp = NULL;
	
	ret = sal_util_file_exist(CONF_STARTUP_TMPSTATUFILE);
	if(ret == SYS_ERR_OK)
		unlink(CONF_RUNNING_TMPSTATUSFILE);
	
	fp = fopen(CONF_STARTUP_TMPSTATUFILE, "w");
	fclose(fp);

	return SYS_ERR_OK;
}

uint32 sal_mib_file_startup_to_run_set(void)
{
	int32 ret = SYS_ERR_FAILED;
	FILE *fp = NULL;
	
	ret = sal_util_file_exist(CONF_STARTUP_TO_RUN_TMPSTATUFILE);
	if(ret == SYS_ERR_OK)
		unlink(CONF_RUNNING_TMPSTATUSFILE);
	
	fp = fopen(CONF_STARTUP_TO_RUN_TMPSTATUFILE, "w");
	fclose(fp);

	return SYS_ERR_OK;
}

uint32 sal_mib_file_clone_to_run_set(void)
{
	int32 ret = SYS_ERR_FAILED;
	FILE *fp = NULL;
	
	ret = sal_util_file_exist(CONF_ClONE_TO_RUN_TMPSTATUFILE);
	if(ret == SYS_ERR_OK)
		unlink(CONF_RUNNING_TMPSTATUSFILE);
	
	fp = fopen(CONF_ClONE_TO_RUN_TMPSTATUFILE, "w");
	fclose(fp);

	return SYS_ERR_OK;
}

uint32 trapStatus[CAPA_SNMP_HOST_NUM_MAX];

uint32 sal_mib_snmp_trap_get(int type, int index, void* value)
{
	sal_snmp_target_t trapValue;
	SYS_ERR_CHK(sal_snmp_target_get(index, &trapValue));
	if (0 == osal_strlen(trapValue.server))
	{
		osal_memset(value, 0, sizeof(uint32));
		return SYS_ERR_OK; 
	}
	
	switch(type)
	{
		case SYS_TRAP_HOSTIPADDR:
			strncpy(value, trapValue.server, sizeof(trapValue.server));
			break;
		case SYS_TRAP_VERSION:
			*(uint32*)value = (uint32)trapValue.secModel + 1;
			break;
		case SYS_TRAP_COMMUNITYNAME:
			strncpy(value, trapValue.name, sizeof(trapValue.name));
			break;
		case SYS_TRAP_V3SECURITYLEVEL:

			*(uint32*)value = (uint32)trapValue.secLevel + 1;
			break;
		case SYS_TRAP_UDPPORT:
			*(uint32*)value = (uint32)trapValue.portNum;
			break;
		case SYS_TRAP_TRAPSTATUS:	
			*(uint32*)value = trapStatus[index];
			break;
		default:
			break;
	}
	return SYS_ERR_OK;
}

uint32 sal_mib_snmp_trap_set(int type, int index, void* value)
{
	FILE *fp = NULL;
	static sal_snmp_target_t  hostEntry;
	int rc = 0;
	uint32 tmpIndex = 0;
	switch(type)
	{
		case SYS_TRAP_HOSTIPADDR:
		    SYS_PARAM_CHK((osal_strlen(value) > CAPA_HOSTNAME_LEN), SYS_ERR_STR_LEN_EXCEED);
		    if (IS_IPV4_UADDRSTR_INVALID(value))
		    {
		        if IS_IPV6_UADDRSTR_INVALID(value)
		        {
		            if (IS_HOSTNAME_INVALID(value))
		                return SYS_ERR_HOSTNAME;
		        }
		    }
		    osal_memset(hostEntry.server, 0, sizeof(hostEntry.server));
		    osal_memcpy(hostEntry.server, value, osal_strlen(value));
			break;
		case SYS_TRAP_VERSION:
			if( 0 < *(uint32*)value && *(uint32*)value < 4)
			{
				hostEntry.secModel = *(uint32*)value - 1;
			}
			else
			{
				return SYS_ERR_RANGE;
			}
			break;
		case SYS_TRAP_COMMUNITYNAME:
			if (osal_strlen(value) > CAPA_SNMP_COMMUNITY_LEN)
				return SYS_ERR_STR_LEN_EXCEED;
			osal_memset(hostEntry.name, 0, sizeof(hostEntry.name));	
			osal_memcpy(hostEntry.name, value, osal_strlen(value));
			break;
		case SYS_TRAP_V3SECURITYLEVEL:

			 hostEntry.secLevel = *(uint32*)value - 1;
			break;
		case SYS_TRAP_UDPPORT:
			 hostEntry.portNum = *(uint32*)value;
			break;
		case SYS_TRAP_TRAPSTATUS:
			hostEntry.retries = DFLT_SNMP_INFORM_RETRY;
			hostEntry.timeout = DFLT_SNMP_INFORM_TIMEOUT;
			if(hostEntry.secModel == SNMP_SECMODEL_V1)
			{
				hostEntry.trapType = SNMP_TRAP_V1;
				hostEntry.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
				if(hostEntry.portNum == 0)
					hostEntry.portNum = DFLT_SNMP_NOTIFACTION_PORT;
			}
			else if(hostEntry.secModel == SNMP_SECMODEL_V2C)
			{
				hostEntry.trapType = SNMP_TRAP_V2;
				hostEntry.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
				if(hostEntry.portNum == 0)
					hostEntry.portNum = DFLT_SNMP_NOTIFACTION_PORT;
				
			}else if(hostEntry.secModel == SNMP_SECMODEL_USM)
			{
				hostEntry.trapType = SNMP_TRAP_V3;
				if(hostEntry.portNum == 0)
					hostEntry.portNum = DFLT_SNMP_NOTIFACTION_PORT;
				if(hostEntry.secLevel < SNMP_SECLEVEL_NOAUTHNOPRIV && hostEntry.secLevel > SNMP_SECLEVEL_AUTHPRIV )
				{
					hostEntry.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
				}
			}
			#ifdef CONFIG_SYS_SNMPV3
	    	if (hostEntry.trapType  < SNMP_TRAP_V3)
		    {
		        SYS_ERR_CHK(sal_snmp_com2secIndex_get(hostEntry.name, &tmpIndex));

		        if (CAPA_SNMP_COMMUNITY_NUM == tmpIndex)
		            return SYS_ERR_COMNUNITY_NOT_FOUND;
		    }
		    else
		    {
		        SYS_ERR_CHK(sal_snmp_userIndex_get(hostEntry.name, &tmpIndex));

		        if (CAPA_SNMP_USER_NUM == tmpIndex)
		            return SYS_ERR_USER_NOT_FOUND;
		    }
			#endif

			rc = sal_util_file_exist(SNMP_CONF_TRAP_FILE);
			if(rc != SYS_ERR_FILE_NOT_EXIST)	
				unlink(SNMP_CONF_TRAP_FILE);

			if (NULL == (fp = fopen(SNMP_CONF_TRAP_FILE, "w")))
    			return SYS_ERR_FAILED;

			if(*(uint32*)value == 1)
			{
			  	fprintf(fp, "%s\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%s\n", hostEntry.server, hostEntry.name, hostEntry.secModel,
            		hostEntry.trapType, hostEntry.secLevel, hostEntry.portNum, hostEntry.retries, hostEntry.timeout, "add");
            	trapStatus[index] = 1;
			}
			else
			{
		    	fprintf(fp, "%s\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%s\n", hostEntry.server, hostEntry.name, hostEntry.secModel,
                    hostEntry.trapType, hostEntry.secLevel, hostEntry.portNum, hostEntry.retries, hostEntry.timeout, "del");
                trapStatus[index] = 2;
            }
			fclose(fp);
			system("handler -f snmptarpconfig 1> /dev/null 2> /dev/null &");
			osal_memset(&hostEntry, 0, sizeof(hostEntry));	
			return SYS_ERR_OK;
			break;
		default:
			break;
	}
	return SYS_ERR_OK;
}

uint32 sal_mib_port_static_vlan_get(int type, uint32 port, void* value)
{	
	char untagstr[256] = {0};
	char tagstr[256] = {0};
	sys_vlan_portMode_t mode;
	sys_vlan_portMember_t portVlanMbr;
	sys_vlanmask_t tmpVm;
	sys_vid_t vid = 0;

	SYS_ERR_CHK(sal_vlan_portAdminMode_get(port, &mode));

	SYS_MEM_CLEAR(portVlanMbr);
	VLANMASK_CLEAR_ALL(tmpVm);
    SYS_ERR_CHK(sal_vlan_portAdminVlanMember_get(port, &portVlanMbr));
 
	if(mode == SYS_VLAN_PORT_MODE_GENERAL)
	{
		if(type == CMD_PORT_TAG)
		{

	        VLANMASK_ANDNOT(tmpVm, portVlanMbr.vmMember, portVlanMbr.vmUntag);
	        if (!IS_VLANMASK_CLEAR(tmpVm))
	        {
	            SYS_MEM_CLEAR(tagstr);
	            SYS_ERR_CHK(VMSK2STR(&tmpVm, tagstr));
	        }
	        osal_memcpy(value, &tagstr, sizeof(tagstr));
        }

        if(type == CMD_PORT_UNTAG)
        {

	        VLANMASK_COPY(tmpVm, portVlanMbr.vmUntag);
	        if (!IS_VLANMASK_CLEAR(tmpVm))
	        {
	            SYS_MEM_CLEAR(untagstr);
	            SYS_ERR_CHK(VMSK2STR(&tmpVm, untagstr));
	        }
	        osal_memcpy(value, &untagstr, sizeof(untagstr));
        }
	}
	else if(mode == SYS_VLAN_PORT_MODE_ACCESS)
	{
		if(type == CMD_PORT_UNTAG)
		{

	        VLANMASK_COPY(tmpVm, portVlanMbr.vmUntag);
	        
	        if (!IS_VLANMASK_CLEAR(tmpVm))
	        {
	            SYS_MEM_CLEAR(untagstr);
				SYS_ERR_CHK(VMSK2STR(&tmpVm, untagstr));
	        }
	        osal_memcpy(value, &untagstr, sizeof(untagstr));
	   	}
	}
	else if(mode == SYS_VLAN_PORT_MODE_TRUNK)
	{
		if(type == CMD_PORT_TAG)
		{

			SYS_ERR_CHK(sal_vlan_portTrunkAllowedVlan_get(port, &tmpVm));
			if (!IS_VLANMASK_CLEAR(tmpVm))
			{
				SYS_MEM_CLEAR(tagstr);
				SYS_ERR_CHK(VMSK2STR(&tmpVm, tagstr));
			}
			osal_memcpy(value, &tagstr, sizeof(tagstr));
		}
		else
		{
			SYS_ERR_CHK(sal_vlan_portOperPvid_get(port, &vid));
			if(vid != 0)
			{
				osal_sprintf(untagstr, "%d", vid);
			}
			osal_memcpy(value, &untagstr, sizeof(untagstr));
		}
	}
	return SYS_ERR_OK;
}

uint32 sal_mib_port_static_vlan_set(int type, uint32 port, void* value, uint32 add)
{	
	sys_vlan_portMode_t mode;
	static sys_vlanmask_t tagVm, untagVm;
	sys_vlan_membership_t vlanMembership = SYS_VLAN_TAGGED;
	sys_vlanmask_t vmAllowed;
	static sys_vid_t vid = 0;
	static int tagcnt =  0;
	static int untagcnt = 0;

	SYS_ERR_CHK(sal_vlan_portAdminMode_get(port, &mode));
	if(mode == SYS_VLAN_PORT_MODE_GENERAL)
	{
		if(type == CMD_PORT_TAG)
		{
			VLANMASK_CLEAR_ALL(tagVm);
			
			SYS_ERR_CHK(STR2VMSK((char*)value, &tagVm));
			tagcnt = 1;
        }

        if(type == CMD_PORT_UNTAG)
        {
        	VLANMASK_CLEAR_ALL(untagVm);
        	
        	SYS_ERR_CHK(STR2VMSK((char*)value, &untagVm));
        	untagcnt = 1;
        }
        if(type == CMD_PORT_STATUS)
        {
        	if(add)
			{
				if(untagcnt)
	        	{
	        		vlanMembership = SYS_VLAN_UNTAGGED;
					SYS_ERR_CHK(sal_vlan_portAdminVlanmask_add(port, vlanMembership, &untagVm));
				}
				if(tagcnt)
	        	{
	        		vlanMembership = SYS_VLAN_TAGGED;
					SYS_ERR_CHK(sal_vlan_portAdminVlanmask_add(port, vlanMembership, &tagVm));
				}
			}
			else
			{
				vlanMembership = SYS_VLAN_EXCLUDED;
				if(untagcnt)
	        	{
					SYS_ERR_CHK(sal_vlan_portAdminVlanmask_add(port, vlanMembership, &untagVm));
				}
				if(tagcnt)
	        	{
					SYS_ERR_CHK(sal_vlan_portAdminVlanmask_add(port, vlanMembership, &tagVm));
				}
			}
			VLANMASK_CLEAR_ALL(untagVm);
			VLANMASK_CLEAR_ALL(tagVm);
			untagcnt = 0;
			tagcnt = 0;
        }
	}
	else if(mode == SYS_VLAN_PORT_MODE_ACCESS)
	{
		if(type == CMD_PORT_UNTAG)
		{

			vid = SYS_STR2UINT((char*) value);

	   	}
	   	else if(type == CMD_PORT_STATUS)
        {
			if(!add)
			{
				SYS_ERR_CHK(sal_vlan_defaultVid_get(&vid));
			}
			
			if (!IS_STATIC_VLAN_EXIST(vid))
				return SYS_ERR_VLAN_WAS_NOT_CREATED_BY_USER;
			SYS_ERR_CHK(sal_vlan_portAdminPvid_set(port, vid));
			vid = 0;
        }
	   	else
	   		return SYS_ERR_FAILED;
	   		
	}
	else if(mode == SYS_VLAN_PORT_MODE_TRUNK)
	{
		if(type == CMD_PORT_TAG)
		{
			SYS_ERR_CHK(STR2VMSK((char*)value, &tagVm));
		}
		else if(type == CMD_PORT_STATUS)
        {
        	SYS_MEM_CLEAR(vmAllowed);
        	SYS_ERR_CHK(sal_vlan_portTrunkAllowedVlan_get(port, &vmAllowed));
			if (add)
			{
				VLANMASK_OR(vmAllowed, vmAllowed, tagVm);
			}
			else 
			{
				VLANMASK_ANDNOT(vmAllowed, vmAllowed, tagVm);
			}
			SYS_ERR_CHK(sal_vlan_portTrunkAllowedVlan_set(port, &vmAllowed));
			VLANMASK_CLEAR_ALL(tagVm);
        }
		else
			return SYS_ERR_FAILED;
	}
	return SYS_ERR_OK;
}

uint32 sal_mib_static_vlan_get(void)
{	

	sys_vid_t vid ;
	sys_vlan_operEntry_t portEntry;

	osal_memset(&existVlan, 0, sizeof(sys_vlanmask_t));
	osal_memset(sVlan, 0, sizeof(staticVlan_t)* CAPA_VLAN_NUM);
	sal_vlan_operEntryExist_get(&existVlan);
	FOR_EACH_VID(vid)
	{	
		if (VLANMASK_IS_VIDSET(existVlan, vid))
		{
			sal_vlan_operEntry_get(vid, &portEntry);
			osal_memcpy(&sVlan[vid-1].name, &portEntry.name, sizeof(sys_namestr_t));
			sVlan[vid-1].untagPorts = portEntry.lpmUntag;
			sVlan[vid-1].memberPorts = portEntry.lpmMember;
		}
	}
	return SYS_ERR_OK;
}
uint32 sal_mib_static_vlan_by_index_get(int type, uint32* pVlanIndex, void* value)
{	
	sys_logic_portmask_t uMember,uUntag;
	int i = 0;
	sys_vid_t vid = 0;
	sys_vlan_operEntry_t portEntry;
	
	vid = *pVlanIndex;

	while(IS_STATIC_VLAN_EXIST(vid) == FALSE)
	{
		vid ++;
		if (IS_VID_INVALID(vid))
		{
			*pVlanIndex = vid;
			return SYS_ERR_FAILED;
		}
	}
	sal_vlan_operEntry_get(vid, &portEntry);

	if(SYS_VLAN_STATICVLANNAME == type)
	{
		osal_memcpy(value, &portEntry.name, sizeof(sys_namestr_t));
	}
	else if(SYS_VLAN_STATICVLANEGRESSPORTS == type)
	{
		sys_port_lPortmask2lstrPortmask(&portEntry.lpmMember, &uMember);
		for(i = 0; i < SYS_BITS_TO_LONGS(SYS_LOGICPORT_NUM_MAX); i++)
		{
			osal_memcpy(value, &uMember.bits[i], 4);
			value += 4;
		}
	}
	else if(SYS_VLAN_STATICVLANUNTAGPORTS == type)
	{
		sys_port_lPortmask2lstrPortmask(&portEntry.lpmUntag, &uUntag);
		for(i = 0; i < SYS_BITS_TO_LONGS(SYS_LOGICPORT_NUM_MAX); i++)
		{
			osal_memcpy(value, &uUntag.bits[i], 4);
			value += 4;
		}
	}
	else if(SYS_VLAN_VLANROWSTATUS == type)
	{

	}
	*pVlanIndex = vid;
	return SYS_ERR_OK;
}

uint32 sal_mib_static_vlan_by_index_getnet(int type, uint32* pVlanIndex, void* value)
{	
	sys_logic_portmask_t uMember,uUntag;
	int i = 0;
	sys_vid_t vid = 0;

	vid = *pVlanIndex;
#if 0
	while(IS_STATIC_VLAN_EXIST(vid) == FALSE)
	{
		vid ++;
		if (IS_VID_INVALID(vid))
		{
			*pVlanIndex = vid;
			return SYS_ERR_FAILED;
		}
	}
#endif
	while(VLANMASK_IS_VIDSET(existVlan, vid) == 0)
	{
		vid ++;
		if (IS_VID_INVALID(vid))
		{
			*pVlanIndex = vid;
			return SYS_ERR_FAILED;
		}
	}

	if(SYS_VLAN_STATICVLANNAME == type)
	{
		osal_memcpy(value, &sVlan[vid-1].name, sizeof(sys_namestr_t));
	}
	else if(SYS_VLAN_STATICVLANEGRESSPORTS == type)
	{
		sys_port_lPortmask2lstrPortmask(&sVlan[vid-1].memberPorts, &uMember);
		for(i = 0; i < SYS_BITS_TO_LONGS(SYS_LOGICPORT_NUM_MAX); i++)
		{
			osal_memcpy(value, &uMember.bits[i], 4);
			value += 4;
		}
	}
	else if(SYS_VLAN_STATICVLANUNTAGPORTS == type)
	{
		sys_port_lPortmask2lstrPortmask(&sVlan[vid-1].untagPorts, &uUntag);
		for(i = 0; i < SYS_BITS_TO_LONGS(SYS_LOGICPORT_NUM_MAX); i++)
		{
			osal_memcpy(value, &uUntag.bits[i], 4);
			value += 4;
		}
	}
	else if(SYS_VLAN_VLANROWSTATUS == type)
	{

	}
	*pVlanIndex = vid;
	return SYS_ERR_OK;
}

uint32 sal_mib_static_vlan_create(int type, uint32 vidindex, void* value, uint32 add)
{	
	static sys_namestr_t  vidName;
	sys_namestr_t origName;
	sys_vid_t vid;

	if(SYS_VLAN_STATICVLANNAME == type)
	{
		osal_memcpy(&vidName, value, sizeof(sys_namestr_t));
		SYS_ERR_CHK(sal_vlan_name_get(vidindex, &origName));
        if (osal_strcmp(vidName.str, origName.str))
        {
            if (SYS_ERR_OK == sal_vlan_vidByName_get(&vidName, &vid))
            {
                return SYS_ERR_OK;
            }
            else
            {
                SYS_ERR_CHK(sal_vlan_name_set(vidindex, &vidName));	
            }
        }	
	}
	else if(SYS_VLAN_VLANROWSTATUS == type)
	{

		if(add)
		{
			if (TRUE == IS_STATIC_VLAN_EXIST(vidindex))
				return SYS_ERR_OK;
			SYS_ERR_CHK(sal_vlan_nameStr_get(vidindex, DFLT_VLAN_PREFIX_NAME, &vidName));	
			SYS_ERR_CHK(sal_vlan_adminEntry_create(vidindex, &vidName));
		}
		else
		{
			if (TRUE == IS_STATIC_VLAN_EXIST(vidindex))
			{
				SYS_ERR_CHK(sal_vlan_adminEntry_destroy(vidindex));

		        SYS_ERR_CHK(sal_l2_dynamicMac_clearByVlan(vidindex));
			}
		}		
	}

	return SYS_ERR_OK;
}

