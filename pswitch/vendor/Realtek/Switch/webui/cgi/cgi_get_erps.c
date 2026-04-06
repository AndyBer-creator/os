
#include <include/cgi.h>

#include <libsal/sal_port.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_erps.h>

static int32 cgi_val_erps_get(void);
static int32 cgi_val_erps_get_edit(void);
static int32 cgi_val_erps_get_global_status(void);

const cgi_cmdmap_t cmdGetErpsTbl[] =
{
    {CGI_CMD_GET_ERPS,              cgi_val_erps_get},
    {CGI_CMD_GET_ERPS_EDIT,         cgi_val_erps_get_edit},
    {CGI_CMD_SET_ERPS_GLOBAL_STATUS,         cgi_val_erps_get_global_status},
    {NULL, NULL}
};

static int32 cgi_val_erps_get_global_status(void)
{
	cJSON *root = NULL;
    cJSON *data = NULL;
    sys_enable_t erpsStatus = DISABLED;
    
	CGI_GET_JSON_CREATE(root, data);
	sal_erps_enable_get(&erpsStatus);
	cJSON_AddBoolToObject(data, CGI_STATE, erpsStatus);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_val_erps_get(void)
{

    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *rings = NULL;
    cJSON *ring= NULL;
    uint32   instId = 0;
    sys_enable_t erpsStatus = DISABLED;
    uint32 bInstance = 0;
    uint32 ringState = 0;
    uint32 mel = 0;
    uint32 pCtrlVlanId = 0;
    uint32 wtrTime = 0;
    uint32 guardTime = 0;
	uint32 workMode = 0;
	uint32 ringId  = 0;
	uint32 ringLevel = 0;
	uint8  isPort0 = 0;
	int32 portId = 0;
	uint32 portRole = 0;
	char   tmpStr[SYS_BUF64_LEN] = {0};
	int32  ringPortStateStatus = 0;
	int32  ringStateStatus = 0;
	int32  instanceId = 0;
	uint32 gp_inst = 0;
	char   tmpStrProInst[SYS_BUF64_LEN] = {0};
	int32  tmpInstanceId = 0;
	int32  bFirst = 0;

	char   *portRoleStr[] = {"rpl", "owner", "neighbour", "next-neigubour"};
	char   *instRingPortStatusStr[] = {"disabled", "listening", "learning", "forwarding", "blocking"};
	char   *instRingStatusStr[] = {"init", "idle", "protection", "manual", "force", "pending"};

	CGI_GET_JSON_CREATE(root, data);
	sal_erps_enable_get(&erpsStatus);
	cJSON_AddBoolToObject(data, CGI_STATE, erpsStatus);
	rings = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_CMD_ERPS_ALL_ARREAY, rings);
	for(instId = 0; instId < MSTP_INSTANCE_NUM; instId++)
	{
		sal_erps_inst_get(instId, &bInstance);
		if(!bInstance)
		{
			ring = cJSON_CreateObject();
			cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_INSTANCE, instId);
			cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_INSTANCE_ENABLE, bInstance);
			cJSON_AddBoolToObject(ring, CGI_CMD_ERPS_INSTANCE_EXIST, bInstance);
			cJSON_AddItemToArray(rings, ring);
			if(!bFirst)
			{
				bFirst = 1;
				tmpInstanceId = instId;
			}
			continue;
		}
		instanceId++;

		ring = cJSON_CreateObject();
		cJSON_AddBoolToObject(ring, CGI_CMD_ERPS_INSTANCE_EXIST, bInstance);
		
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_INSTANCE, instId);
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_INSTANCE_ENABLE, bInstance);
		
		sal_erps_ringState_get(instId, &ringState);
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_RINGSTATUS, ringState);

		sal_erps_mel_get(instId, &mel);
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_RINGMEL, mel);

		sal_erps_ctrlVlan_get(instId, &pCtrlVlanId);
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_RINGCONTROLVLAN, pCtrlVlanId);

		sal_erps_wtrTimer_get(instId, &wtrTime);
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_RINGWTRTIME, wtrTime);

		sal_erps_guardTimer_get(instId, &guardTime);
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_RINGGUARDTIME, guardTime);

		sal_erps_workMode_get(instId, &workMode);
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_RINGWORKINGMODE, workMode);

		sal_erps_ringId_get(instId, &ringId);
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_RINGID, ringId);

		sal_erps_ringLevel_get(instId, &ringLevel);
		cJSON_AddNumberToObject(ring, CGI_CMD_ERPS_RINGRINGLEVEL, ringLevel);

		sal_erps_ring_protected_instance_get(instId, &gp_inst);
		if(SAL_ERPS_PROTECT_INSTANCE != gp_inst)
		{
			sprintf(tmpStrProInst,"%u",gp_inst);
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGRINGPROINST, tmpStrProInst);
		}
		else
		{
			sprintf(tmpStrProInst,"---");
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGRINGPROINST, tmpStrProInst);
		}

		isPort0 = TRUE;
		SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));
		if(SAL_ERPS_RING_PORT_ID_DEF == portId)
		{
			strcpy(tmpStr,"N/A");
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT0, tmpStr);
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT0ROLE, "N/A");
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT0STATUS, "N/A");
		}
		else
		{
			SYS_ERR_CHK(sal_erps_get_inst_port_state_status(portId, &ringPortStateStatus));
			SYS_ERR_CHK(LP2STR(portId, tmpStr));
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT0, tmpStr);
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT0ROLE, portRoleStr[portRole]);
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT0STATUS, instRingPortStatusStr[ringPortStateStatus]);
		}

		isPort0 = FALSE;
		SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));
		if(SAL_ERPS_RING_PORT_ID_DEF == portId)
		{
			strcpy(tmpStr,"N/A");
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT1, tmpStr);
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT1ROLE, "N/A");
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT1STATUS, "N/A");
		}
		else
		{
			SYS_ERR_CHK(sal_erps_get_inst_port_state_status(portId, &ringPortStateStatus));
			SYS_ERR_CHK(LP2STR(portId, tmpStr));
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT1, tmpStr);
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT1ROLE, portRoleStr[portRole]);
			cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGPORT1STATUS, instRingPortStatusStr[ringPortStateStatus]);
		}

		sal_erps_get_inst_state_status(instId, &ringStateStatus);
		cJSON_AddStringToObject(ring, CGI_CMD_ERPS_RINGNODESTATUS, instRingStatusStr[ringStateStatus]);

		cJSON_AddItemToArray(rings, ring);
	}
	#if 0
	if(tmpInstanceId != (MSTP_INSTANCE_NUM-1))
	{
		tmpInstanceId++;
	}
	#endif
	
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGNODEINSTANCEID, tmpInstanceId);
    cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGNODEINSTANCEIDMIN, 0);
    cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGNODEINSTANCEIDMAX, (MSTP_INSTANCE_NUM-1));
    cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGNODEINSTANCEIDDEF, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_val_erps_get_edit(void)
{

	char  *value = NULL;
	cJSON *root = NULL;
	cJSON *data = NULL;
	cJSON *ports = NULL;
	cJSON *port = NULL;
	uint32	 instId = 0;
	uint32   bInstance = 0;
	uint32 ringState = 0;
    uint32 mel = 0;
    uint32 pCtrlVlanId = 0;
    uint32 wtrTime = 0;
    uint32 guardTime = 0;
	uint32 workMode = 0;
	uint32 ringId  = 0;
	uint32 ringLevel = 0;
	uint8  isPort0 = 0;
	int32 portId = 0;
	uint32 portRole = 0;
	uint32 gp_inst = 0;
	
	sys_logic_port_t lp;
	sys_enable_t erpsStatus = DISABLED;
	char descp[CAPA_PORTDESC_STR_LEN + 1];

    if (CGI_IS_PARAM_EXIST(CGI_CMD_GET_ERPS_RING_INS, value))
        instId = SYS_STR2UINT(value);
        
    CGI_GET_JSON_CREATE(root, data);
    sal_erps_enable_get(&erpsStatus);
	cJSON_AddBoolToObject(data, CGI_STATE, erpsStatus);
	
	sal_erps_inst_get(instId, &bInstance);
	if(!bInstance)
	{
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_INSTANCE, instId);
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_INSTANCE_ENABLE, bInstance);

		ports = cJSON_CreateArray();
		cJSON_AddItemToObject(data, CGI_PORTS, ports);
		FOR_EACH_NORMAL_LOGIC_PORT(lp)
		{
			port = cJSON_CreateObject();
			cJSON_AddItemToArray(ports, port);
			cJSON_AddNumberToObject(port, CGI_VALUE, lp);
			sys_port_lPort2uStr(lp, SYS_PORT_STRFMT_WEB, descp);
	        cJSON_AddStringToObject(port, CGI_NAME, descp);
		}
	
		CGI_GET_JSON_RESPONSE_AND_DESTROY(root);
		return SYS_ERR_OK;
	}

	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_INSTANCE, instId);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_INSTANCE_ENABLE, bInstance);
	sal_erps_ringState_get(instId, &ringState);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGSTATUS, ringState);
	sal_erps_ringState_get(instId, &ringState);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGSTATUS, ringState);

	sal_erps_mel_get(instId, &mel);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGMEL, mel);

	sal_erps_ctrlVlan_get(instId, &pCtrlVlanId);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGCONTROLVLAN, pCtrlVlanId);

	sal_erps_wtrTimer_get(instId, &wtrTime);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGWTRTIME, wtrTime);

	sal_erps_guardTimer_get(instId, &guardTime);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGGUARDTIME, guardTime);

	sal_erps_workMode_get(instId, &workMode);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGWORKINGMODE, workMode);

	sal_erps_ringId_get(instId, &ringId);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGID, ringId);

	sal_erps_ringLevel_get(instId, &ringLevel);
	cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGRINGLEVEL, ringLevel);
	
	sal_erps_ring_protected_instance_get(instId, &gp_inst);
	if(SAL_ERPS_PROTECT_INSTANCE != gp_inst)
	{
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGRINGPROINST, gp_inst);
	}
	else
	{
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGRINGPROINST, 0);
	}

	isPort0 = TRUE;
	SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));
	if(SAL_ERPS_RING_PORT_ID_DEF == portId)
	{
		
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGPORT0, -1);
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGPORTROLEVALUE0, 0);
	}
	else
	{
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGPORT0, portId);
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGPORTROLEVALUE0, portRole);
	}

	isPort0 = FALSE;
	SYS_ERR_CHK( sal_erps_ringPort_get(instId, isPort0, &portId, &portRole));
	if(SAL_ERPS_RING_PORT_ID_DEF == portId)
	{
		
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGPORT1, -1);
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGPORTROLEVALUE1, 0);
	}
	else
	{
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGPORT1, portId);
		cJSON_AddNumberToObject(data, CGI_CMD_ERPS_RINGPORTROLEVALUE1, portRole);
	}

    ports = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_PORTS, ports);
	
	port = cJSON_CreateObject();
	cJSON_AddItemToArray(ports, port);
	cJSON_AddNumberToObject(port, CGI_VALUE, -1);
	cJSON_AddStringToObject(port, CGI_NAME, "N/A");

	FOR_EACH_NORMAL_LOGIC_PORT(lp)
	{
		port = cJSON_CreateObject();
		cJSON_AddItemToArray(ports, port);
		cJSON_AddNumberToObject(port, CGI_VALUE, lp);
		sys_port_lPort2uStr(lp, SYS_PORT_STRFMT_WEB, descp);
        cJSON_AddStringToObject(port, CGI_NAME, descp);
	}

	CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

