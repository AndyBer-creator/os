
#include <libsal/sal_util.h>
#include <libsal/sal_port.h>
#include <libsal/sal_poe.h>
#include <rsd/rsd_poe.h>

#ifdef CONFIG_SYS_POE
#include <libcfg/cfg_poe.h>
#endif

int32 sal_strtime_to_hour_min_sec(char* pStrtime, uint8* pHour, uint8* pMinute, uint8* pSecond)
{
	char *tmp = NULL;
	char *ridx = NULL;
	int i;
	
	SYS_PARAM_CHK((NULL == pHour), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == pMinute), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == pSecond), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == pStrtime), SYS_ERR_NULL_POINTER);

	tmp = pStrtime;
	ridx = rindex(tmp, ':');
	
	if (NULL == ridx)
		return SYS_ERR_INPUT;
	
	*ridx = '\0';
	ridx++;
	for (i = 0; i < osal_strlen(ridx) ; i++)
	{
		if (!isdigit(ridx[i]))
			return SYS_ERR_INPUT;
	}
	
	*pSecond = SYS_STR2UINT(ridx);
	
	if ( *pSecond > 59 )
		return SYS_ERR_INPUT;

	ridx = rindex(tmp, ':');
	
	if (NULL == ridx)
		return SYS_ERR_INPUT;
	
	*ridx = '\0';
	ridx++;
	for (i = 0; i < osal_strlen(ridx) ; i++)
	{
		if (!isdigit(ridx[i]))
			return SYS_ERR_INPUT;
	}
	*pMinute = SYS_STR2UINT(ridx);
	
	if ( *pMinute > 59 )
		return SYS_ERR_INPUT;

	*pHour = SYS_STR2UINT(tmp);
	for (i = 0; i < osal_strlen(tmp) ; i++)
	{
		if (!isdigit(tmp[i]))
			return SYS_ERR_INPUT;
	}
	
	if ( *pHour > 23 )
		return SYS_ERR_INPUT;

	return SYS_ERR_OK;
}

int32 sal_poe_portEnable_set(sys_logic_port_t port, uint32 Enable)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    if(port >= POE_MAXPORT_NUM)
    {
        return ret;
    }

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(data);

    data.value = Enable;

    SYS_ERR_CHK(rsd_poe_data_set(port, POE_CMD_SET_PORT_ENABLE, &data));
    SYS_ERR_CHK(cfg_poe_portEnable_set(port, Enable));

    return ret;
}

int32 sal_poe_portIcut_set(sys_logic_port_t port, uint32 Icut)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(data);

    data.value = Icut;

    SYS_ERR_CHK(cfg_poe_portIcut_set(port, Icut));

    return ret;
}

int32 sal_poe_portSchedule_set(sys_logic_port_t port, uint32 week, uint32 hour)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    if(port >= POE_MAXPORT_NUM)
    {
        return ret;
    }

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(data);

    data.value = (week << 24) | hour;

    SYS_ERR_TRK_CHK(rsd_poe_data_set(port, POE_CMD_SET_PORT_SCHEDULE, &data));
    SYS_ERR_CHK(cfg_poe_portSchedule_set(port, week, hour));

    return ret;
}

int32 sal_poe_timeZone_set(uint32 timeZone)
{
    int32 ret = SYS_ERR_OK;
	sys_logic_port_t port = 0;
    sys_port_poe_data_t data;

    SYS_MEM_CLEAR(data);

    data.value = timeZone;

    SYS_ERR_TRK_CHK(rsd_poe_data_set(port, POE_CMD_SET_TIMEZONE, &data));

    return ret;
}

#ifdef CONFIG_SYS_POE_SWDOG

int32 sal_poe_port_watch_dog_Enable_set(sys_logic_port_t port, uint32 Enable)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    if(port >= POE_MAXPORT_NUM)
    {
        return ret;
    }

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(data);

    data.value = Enable;
    SYS_ERR_CHK(rsd_poe_data_set(port, POE_CMD_SET_PORT_DOG, &data));
    SYS_ERR_CHK(cfg_poe_port_watchdog_Enable_set(port, Enable));
    return ret;
}

int32 sal_poe_port_watch_dog_Enable_get(sys_logic_port_t port, uint32 *pEnable)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_poe_port_watchdog_Enable_get(port, pEnable));

    return ret;
}
#endif

int32 sal_poe_portStatus_get(sys_logic_port_t port, uint32 *pStatus)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    ret = rsd_poe_data_get(port, POE_CMD_GET_PORT_STATUS, &data);

    *pStatus = data.value;

    return ret;
}

int32 sal_poe_portAbility_get(sys_logic_port_t port, uint32 *pEnable)
{
	int32 ret = SYS_ERR_OK;
	sys_port_poe_data_t data;

	SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

	SYS_MEM_CLEAR(data);

	ret = rsd_poe_data_get(port, POE_CMD_GET_PORT_ABILITY, &data);

	*pEnable = data.value;

	return ret;
}

int32 sal_poe_portEnable_get(sys_logic_port_t port, uint32 *pEnable)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_poe_portEnable_get(port, pEnable));

    return ret;
}

int32 sal_poe_portPwGood_get(sys_logic_port_t port, uint32 *pPwGood)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPwGood), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    ret = rsd_poe_data_get(port, POE_CMD_GET_PORT_PWGOOD, &data);

    *pPwGood = data.value;

    return ret;
}

int32 sal_poe_portCurrent_get(sys_logic_port_t port, uint32 *pCurrent)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCurrent), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    ret = rsd_poe_data_get(port, POE_CMD_GET_PORT_CURRENT, &data);

    *pCurrent = data.value;

    return ret;
}

int32 sal_poe_portVoltage_get(sys_logic_port_t port, uint32 *pVoltage)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVoltage), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    ret = rsd_poe_data_get(port, POE_CMD_GET_PORT_VOLTAGE, &data);

    *pVoltage = data.value;

    return ret;
}

int32 sal_poe_portIcut_get(sys_logic_port_t port, uint32 *pIcut)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pIcut), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_poe_portIcut_get(port, pIcut));

    return ret;
}

int32 sal_poe_devPower_get(sys_logic_port_t port, uint32 *pPower)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPower), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    ret = rsd_poe_data_get(port, POE_CMD_GET_DEV_POWER_STATUS, &data);

    *pPower = data.value;

    return ret;
}

int32 sal_poe_devTemperature_get(sys_logic_port_t port, uint32 *pTemperature)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTemperature), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(data);

    ret = rsd_poe_data_get(port, POE_CMD_GET_DEV_TEMPERATURE, &data);

    *pTemperature = data.value;

    return ret;
}

int32 sal_poe_portSchedule_get(sys_logic_port_t port, uint32 week, uint32 *hour)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(cfg_poe_portSchedule_get(port, week, hour));

    return ret;
}

int32 sal_poe_portInfo_get(sys_logic_port_t port, sal_poe_ioctl_t *pInfo)
{
    int32 ret = SYS_ERR_OK;
	uint32 data = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    ret = sal_poe_portEnable_get(port, &data);
    pInfo->portEnable = data;

	data = 0;
    ret = sal_poe_portPwGood_get(port, &data);
    pInfo->portStatus = data;

	data = 0;
    ret = sal_poe_portStatus_get(port, &data);
    pInfo->portLevel = data;

	data = 0;
    ret = sal_poe_portIcut_get(port, &data);
    pInfo->portPowerLimit = data;

	data = 0;
    ret = sal_poe_portVoltage_get(port, &data);
    pInfo->portVolatge = data;

	data = 0;
    ret = sal_poe_portCurrent_get(port, &data);
    pInfo->portCurrent = data;

    pInfo->portPower = (pInfo->portCurrent * pInfo->portVolatge);

	if( (PD_CLASS1_3_84W <= pInfo->portLevel) && (pInfo->portLevel <= PD_CLASS3_13W))
	{
		pInfo->portType = PD_MODE_AF;
	}
    else if (pInfo->portLevel == PD_CLASS4_25_5W)
    {
        pInfo->portType = PD_MODE_AT;
    }
    else if (pInfo->portLevel == 0)
    {
        pInfo->portType = PD_MODE_AT;
    }
    else if ( (PD_CLASS5_40W <= pInfo->portLevel) && (pInfo->portLevel <= PD_CLASS8_71W))
    {
        pInfo->portType = PD_MODE_BT;
    }
    else
    {
		pInfo->portType = PD_MODE_NONE;
    }
#ifdef CONFIG_SYS_POE_SWDOG
	data = 0;
	sal_poe_port_watch_dog_Enable_get(port, &data);
	pInfo->portWatchDog = data;
#endif	
    return ret;
}

int32 sal_poe_port_total_num_set(uint32 num)
{
    int32 ret = SYS_ERR_OK;

    SYS_ERR_CHK(cfg_poe_port_total_set(num));
    
    return ret;
}

int32 sal_poe_port_total_num_get(uint32 *num)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK((NULL == num), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_poe_port_total_get(num));

    return ret;
}

int32 sal_poe_status_info_updata(void)
{
	uint32 poe_sum = 0;
	uint32 lport = 0;
	uint32 currentPowerSum = 0;
	sal_poe_ioctl_t data;	
	sal_poe_port_total_num_get(&poe_sum);

	for(lport = 0; lport < poe_sum; lport++)
	{
		osal_memset(&data, 0, sizeof(sal_poe_ioctl_t));

		SYS_ERR_CHK(sal_poe_portPwGood_get(lport, &data.portStatus));
		
    	SYS_ERR_CHK(sal_poe_portVoltage_get(lport, &data.portVolatge));
    	
    	SYS_ERR_CHK(sal_poe_portCurrent_get(lport, &data.portCurrent));
    	
		data.portPower = data.portVolatge * data.portCurrent;
		
		SYS_ERR_CHK(cfg_poe_sw_info_port_set(lport, &data.portStatus, &data.portCurrent, &data.portVolatge, &data.portPower));
    	currentPowerSum += data.portPower;
	}

	SYS_ERR_CHK(cfg_poe_currentPower_sum_set(currentPowerSum));
	return SYS_ERR_OK;
}

int32 sal_poe_currentPower_sum_get(uint32 *pSum)
{
    SYS_PARAM_CHK((NULL == pSum), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(cfg_poe_currentPower_sum_get(pSum));
	return SYS_ERR_OK;
}

int32 sal_poe_port_info_get(sys_logic_port_t port, sal_poe_ioctl_t *pData)
{
    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(cfg_poe_sw_info_port_get(port, &pData->portStatus, &pData->portCurrent, &pData->portVolatge, &pData->portPower));
	return SYS_ERR_OK;
}

int32 sal_poe_portPower_get(sys_logic_port_t port, uint32 *pPower)
{
    int32 ret = SYS_ERR_OK;
    uint32 voldata = 0;
    uint32 curdata = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    ret = sal_poe_portVoltage_get(port, &voldata);
    ret = sal_poe_portCurrent_get(port, &curdata);

    *pPower = voldata * curdata;

    return ret;
}

int32 sal_poe_portAssignTime_work_set(sys_logic_port_t port, sal_poe_time_t rebootTime, sal_poe_time_t delayTime)
{
    int32 ret = SYS_ERR_OK;
    sys_port_poe_data_t data;
    cfg_poe_time_t retime, deytime;

    if(port >= POE_MAXPORT_NUM)
    {
        return ret;
    }

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(data);

    data.value = (rebootTime.hour << 24) | (rebootTime.minute << 16) | rebootTime.second;

    SYS_ERR_TRK_CHK(rsd_poe_data_set(port, POE_CMD_SET_TIME_CLOSE, &data));

    SYS_MEM_CLEAR(data);
    data.value = (delayTime.hour << 24) | (delayTime.minute << 16) | delayTime.second;

    SYS_ERR_TRK_CHK(rsd_poe_data_set(port, POE_CMD_SET_TIME_OPEN, &data));
    memcpy(&retime, &rebootTime, sizeof(sal_poe_time_t));
    memcpy(&deytime, &delayTime, sizeof(sal_poe_time_t));
    SYS_ERR_CHK(cfg_poe_portAssignTime_work_set(port, retime, deytime));

    return ret;
}

int32 sal_poe_portAssignTime_work_get(sys_logic_port_t port, sal_poe_time_t* pRebootTime, sal_poe_time_t* pDelayTime)
{
    int32 ret = SYS_ERR_OK;
    cfg_poe_time_t retime, deytime;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pRebootTime), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pDelayTime), SYS_ERR_NULL_POINTER);

	SYS_MEM_CLEAR(retime);
	SYS_MEM_CLEAR(deytime);

    SYS_ERR_CHK(cfg_poe_portAssignTime_work_get(port, &retime, &deytime));
    memcpy(pRebootTime, &retime, sizeof(sal_poe_time_t));
    memcpy(pDelayTime, &deytime, sizeof(sal_poe_time_t));
    return ret;
}

