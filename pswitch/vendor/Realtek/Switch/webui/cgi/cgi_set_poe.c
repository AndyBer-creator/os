
#include <include/cgi.h>
#include <libsal/sal_poe.h>

static int32 cgi_poe_settingEdit_set(void);
static int32 cgi_poe_timer_set(void);
static int32 cgi_poe_reboot_timer_Edit_set(void);

const cgi_cmdmap_t cmdSetPoeTbl[] =
{
    {CGI_CMD_POE_PORTEDIT,             cgi_poe_settingEdit_set},
    {CGI_CMD_POE_PORTTIMER,            cgi_poe_timer_set},
    {CGI_CMD_POE_PORTREBOOTEDIT,       cgi_poe_reboot_timer_Edit_set},
    {NULL, NULL}
};

static int32 cgi_poe_settingEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    char *value = NULL;
    sys_enable_t enable = DISABLED;
#ifdef CONFIG_SYS_POE_SWDOG
	sys_enable_t en_poe_wsdog = DISABLED;
#endif

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);

    if (CGI_IS_PARAM_EXIST(CGI_POE_PORT_ENABLE, value))
        enable = SYS_STR2UINT(value);

	#if 0
    if (CGI_IS_PARAM_EXIST(CGI_POE_PORT_POWER_LIMIT, value))
        portCfg.portPowerLimit = SYS_STR2UINT(value);
	#endif
#ifdef CONFIG_SYS_POE_SWDOG	
    if (CGI_IS_PARAM_EXIST(CGI_POE_PORT_WATCH_DOG, value))
        en_poe_wsdog = SYS_STR2UINT(value);
#endif
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_poe_portEnable_set(lp, enable), lp);

#ifdef CONFIG_SYS_POE_SWDOG
       CGI_SET_PORT_ERR_CONTINUE(sal_poe_port_watch_dog_Enable_set(lp, en_poe_wsdog), lp);
#endif
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_poe_timer_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_logic_port_t lp;
    char tmp[64];
	int32 i;
	uint32 hour;
	uint32 ulWeek;
	uint32 ulWeek_tmp = 0;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_PORT, value))
    {
        lp = SYS_STR2UINT(value);
    }
    else
        goto cgi_end;

    for (ulWeek = 1; ulWeek <= POE_WEEK_MAX; ulWeek++)
	{
		if (ulWeek < POE_WEEK_MAX)
		{
			ulWeek_tmp = ulWeek;
		}
		else
		{
			ulWeek_tmp = 0;
		}

		hour = 0;

	    for (i = 0; i < 24; i++)
	    {
			SYS_MEM_CLEAR(tmp);
			osal_sprintf(tmp, "%s%d_%d", "hour", i, ulWeek_tmp);

			if (CGI_IS_PARAM_EXIST(tmp, value))
			{
				hour = (0x00000001 << i) | hour;
			}
	    }
		CGI_SET_PORT_ERR_CONTINUE(sal_poe_portSchedule_set(lp, ulWeek_tmp, hour), lp);
	}

	cgi_end:
		cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_poe_reboot_timer_Edit_set(void)
{
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    char *value = NULL;
   	sal_poe_time_t rebTime;
	sal_poe_time_t delTime;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
	
    if (CGI_IS_PARAM_EXIST(CGI_POE_PORT_EDIT_RE_HOUR, value))
        rebTime.hour = SYS_STR2UINT(value);
        
    if (CGI_IS_PARAM_EXIST(CGI_POE_PORT_EDIT_RE_MINUTE, value))
    	rebTime.minute = SYS_STR2UINT(value);
    	
    if (CGI_IS_PARAM_EXIST(CGI_POE_PORT_EDIT_RE_SECOND, value))
    	rebTime.second = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_POE_PORT_EDIT_DE_HOUR, value))
    	delTime.hour = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_POE_PORT_EDIT_DE_MINUTE, value))
    	delTime.minute = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_POE_PORT_EDIT_DE_SECOND, value))
    	delTime.second = SYS_STR2UINT(value);
    	
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_poe_portAssignTime_work_set(lp, rebTime, delTime), lp);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

