
#include <include/cgi.h>
#include <libsal/sal_erps.h>

static int32 cgi_erps_prop_set(void);
static int32 cgi_erps_prop_set_edit(void);
static int32 cgi_erps_prop_instance_set(void);
static int32 cgi_erps_prop_instance_del(void);

const cgi_cmdmap_t cmdSetErpsTbl[] =
{
    {CGI_CMD_SET_ERPS,    cgi_erps_prop_set},
    {CGI_CMD_SET_ERPS_EDIT,    cgi_erps_prop_set_edit},
    {CGI_CMD_SET_ERPS_INSTANCE,    cgi_erps_prop_instance_set},
	{CGI_CMD_DEL_ERPS_INSTANCE,    cgi_erps_prop_instance_del},

    {NULL, NULL}
};

static int32 cgi_erps_prop_set_edit(void)
{
	int32 ret = SYS_ERR_OK;
	char *value = NULL;
	uint32   instId = 0;
	uint32 ringState = 0;
    uint32 mel = 0;
    uint32 pCtrlVlanId = 0;
    uint32 wtrTime = 0;
    uint32 guardTime = 0;
	uint32 workMode = 0;
	uint32 ringId  = 0;
	uint32 ringLevel = 0;
	uint8  isPort0 = 0;
	uint32 portId = 0;
	uint32 portRole = SAL_ERPS_RPL_PORT;
	
	uint32   bInstance = 0;
	uint32 pg_inst;

	if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_INSTANCE_EDIT_SEL_RING, value))
	{
		instId = SYS_STR2UINT(value);
		sal_erps_inst_get(instId, &bInstance);
		#if 0
		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_INSTANCE_ADD_AND_DEL, value))
		{
			erpsInstanceAddOrDel = SYS_STR2UINT(value);
			if(erpsInstanceAddOrDel == FALSE)
			{		
				if(bInstance)
				{
					
					CGI_SET_ERR_HDL(sal_erps_inst_del(instId), ret);
				}
				goto cgi_end;
				
			}			
		}
		#endif
		if(!bInstance)
		{
			sal_erps_inst_create(instId);
		}
		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGMEL, value))
		{
			mel = SYS_STR2UINT(value);
			CGI_SET_ERR_HDL(sal_erps_mel_set(instId, mel), ret);
		}

		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGWTRTIME, value))
		{
			wtrTime = SYS_STR2UINT(value);
			CGI_SET_ERR_HDL(sal_erps_wtrTimer_set(instId, wtrTime), ret);
		}

		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGGUARDTIME, value))
		{
			guardTime = SYS_STR2UINT(value);
			CGI_SET_ERR_HDL(sal_erps_guardTimer_set(instId, guardTime), ret);
		}

		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGWORKINGMODE, value))
		{
			workMode = SYS_STR2UINT(value);
			if((workMode != TRUE) && (workMode != FALSE))
			{
				goto cgi_end; 
			}
			CGI_SET_ERR_HDL(sal_erps_workMode_set(instId, workMode), ret);
		}

		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGID, value))
		{
			ringId = SYS_STR2UINT(value);
			CGI_SET_ERR_HDL(sal_erps_ringId_set(instId, ringId), ret);
		}

		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGRINGLEVEL, value))
		{
			ringLevel = SYS_STR2UINT(value);
			CGI_SET_ERR_HDL(sal_erps_ringLevel_set(instId, ringLevel), ret);
		}

		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGPORT0, value))
		{
			
			portId = atoi(value);
			isPort0 = TRUE;
			
			if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGPORTROLEVALUE0, value))
			{

				portRole = SYS_STR2UINT(value);
				if(portId == SAL_ERPS_RING_PORT_ID_DEF)
				{
					CGI_DBG("port0:%d",portId);
					sal_erps_ringPort_del(instId, isPort0);
				}
				else
				{
					CGI_SET_ERR_HDL(sal_erps_ringPort_add(instId, (int32)portId, portRole, isPort0), ret);
					sal_erps_ringPort_add(instId, (int32)portId, portRole, isPort0);
				}
			}
		}

		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGPORT1, value))
		{
			portId = atoi(value);
			isPort0 = FALSE;
			if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGPORTROLEVALUE1, value))
			{
				portRole = SYS_STR2UINT(value);
				if(portId == SAL_ERPS_RING_PORT_ID_DEF)
				{
					CGI_DBG("port1:%d",portId);
					sal_erps_ringPort_del(instId, isPort0);
				}
				else
				{
					CGI_SET_ERR_HDL(sal_erps_ringPort_add(instId, (int32)portId, portRole, isPort0), ret);
					sal_erps_ringPort_add(instId, (int32)portId, portRole, isPort0);
				}
			}
		}
		
		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGRINGPROINST, value))
		{
			pg_inst = SYS_STR2UINT(value);
			CGI_SET_ERR_HDL(sal_erps_ring_protected_instance_add(instId, pg_inst), ret);
		}
		
		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGCONTROLVLAN, value))
		{
			pCtrlVlanId = SYS_STR2UINT(value);
			CGI_SET_ERR_HDL(sal_erps_ctrlVlan_set(instId, pCtrlVlanId), ret);
		}
		if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGSTATUS, value))
		{
			ringState = SYS_STR2UINT(value);
			if((ringState != TRUE) && (ringState != FALSE))
			{
				goto cgi_end; 
			}
	
			CGI_SET_ERR_HDL(sal_erps_ringState_set(instId, ringState), ret);
		}		
		
	}

	cgi_end:

    cgi_set_respGeneral_send(ret);
    return SYS_ERR_OK;
}

static int32 cgi_erps_prop_set(void)
{
	sys_enable_t erpsStatus = DISABLED;
	char *val;
	int32 ret = SYS_ERR_OK;

	if (CGI_IS_PARAM_EXIST(CGI_STATE, val))
	{
		erpsStatus = SYS_STR2UINT(val);
	}
	else
	{
		goto cgi_end;
	}

	CGI_SET_ERR_HDL(sal_erps_enable_set(erpsStatus), ret);
	
	cgi_end:
	
	cgi_set_respGeneral_send(ret);
    return SYS_ERR_OK;
}

static int32 cgi_erps_prop_instance_set(void)
{
	uint32 erpsInstanceId = 0;
	char *val;
	int32 ret = SYS_ERR_OK;
	uint32 bInstance = 0;

	if (CGI_IS_PARAM_EXIST(CGI_CMD_ERPS_RINGNODEINSTANCEID, val))
	{
		erpsInstanceId = SYS_STR2UINT(val);
	}
	else
	{
		goto cgi_end;
	}
	sal_erps_inst_get(erpsInstanceId, &bInstance);
	if(bInstance)
	{
		goto cgi_end;
	}

	CGI_SET_ERR_HDL(sal_erps_inst_create(erpsInstanceId), ret);
	
	cgi_end:
	
	cgi_set_respGeneral_send(ret);
    return SYS_ERR_OK;
}

static int32 cgi_erps_prop_instance_del(void)
{
	
	int32 ret = SYS_ERR_OK;
	uint32 len = 0;
	uint32 index = 0;
	uint32 i = 0;
	char **array = NULL;

	if (!CGI_GET_PARAM_ARRAY_DATA(CGI_CMD_GET_ERPS_RING_INS, array, &len))
		goto cgi_end;

	for (i = 0; i < len; i++)
	{
		index = SYS_STR2UINT(array[i]);
		
		CGI_SET_ERR_HDL(sal_erps_inst_del(index), ret);

	}
	
	cgi_end:
	cgi_set_respGeneral_send(ret);
	if (NULL != array)
		osal_free(array);

	return SYS_ERR_OK;

}

