
#include <include/cgi.h>
#include <libipc/quaggaTask.h>
#include <libipc/vos.h>
#include <libsal/sal_l3_route_ospf.h>

static int32 cgi_ospf_prop_set(void);
static int32 cgi_ospf_area_network_add(void);
static int32 cgi_ospf_area_network_del(void);

const cgi_cmdmap_t cmdSetOspfTbl[] =
{
    {CGI_CMD_OSPF_CFG,                    cgi_ospf_prop_set},
    {CGI_CMD_OSPF_AREA_NETWORK_ADD,       cgi_ospf_area_network_add},
    {CGI_CMD_OSPF_AREA_NETWORK_DEL,       cgi_ospf_area_network_del},
    {NULL, NULL}
};

static int32 cgi_ospf_prop_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    sys_enable_t enable = DISABLED;
	OSPF_INFO_T ospfinfo;
	int32 rc = SYS_ERR_OK;
	uint16 ProcessID = 1;

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        enable = ENABLED;
    else
        enable = DISABLED;

    SYS_MEM_CLEAR(ospfinfo);

	ospfinfo.instance = ProcessID;
	rc = sal_ospf_Info_get(&ospfinfo);
	if(rc != SYS_ERR_OK && enable)
	{
		CGI_SET_ERR_HDL(sal_l3_routeV4_ospf_open(ProcessID), ret);
	}
	else 
	{
		if(enable == DISABLED)
		{
			CGI_SET_ERR_HDL(sal_l3_routeV4_ospf_cloce(ProcessID), ret);
		}
	}
    
cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_ospf_area_network_add(void)
{
	int32 ret = SYS_ERR_OK;
	char *value = NULL;
	sys_ipv4_t network = 0;
	sys_ipv4_t mask = 0;
	sys_ipv4_t ospfAreaId = 0;
	uint32	 ospf_prosseId = 1;

	if (CGI_IS_PARAM_EXIST(CGI_OSPF_AREA_ID, value))
	{
		STR2IPV4(value, &ospfAreaId);
		CGI_SET_ERR_HDL(sal_l3_ospf_area_set(ENABLED, ospfAreaId, ospf_prosseId), ret);
	}

	if (CGI_IS_PARAM_EXIST(CGI_OSPF_NETWORKADDR, value))
	{
		STR2IPV4(value, &network);
	}

	if (CGI_IS_PARAM_EXIST(CGI_OSPF_NETWORKMASK, value))
	{
		STR2IPV4(value, &mask);
	}

	if(network && mask)
	{
		CGI_SET_ERR_HDL(sal_l3_ospf_network_area_set(DISABLED, network, mask, ospfAreaId, ospf_prosseId), ret);
	}
	else if(network == 0)
	{
		CGI_SET_ERR_HDL(sal_l3_ospf_network_area_set(DISABLED, network, mask, ospfAreaId, ospf_prosseId), ret);
	}

cgi_end:
	cgi_set_respGeneral_send(ret);

	return SYS_ERR_OK;
}

static int32 cgi_ospf_area_network_del(void)
{
	int32 ret = SYS_ERR_OK;
	sys_ipv4_t network = 0;
	sys_ipv4_t mask = 0;
	char *ridx = NULL;
	char networkIp[SYS_BUF32_LEN] = {0};
	char networkMask[SYS_BUF32_LEN] = {0};
	char areaId[SYS_BUF32_LEN] = {0};
	sys_ipv4_t ospfAreaId = 0;
	uint32	 ospf_prosseId = 1;
	uint32 len = 0;
	uint32 i = 0;
    char **array = NULL;

	if (CGI_GET_PARAM_ARRAY_DATA(CGI_OSPF_AREA_ID, array, &len))
	{
		for (i = 0; i < len; i++)
		{
		    SYS_MEM_CLEAR(network);
    		SYS_MEM_CLEAR(mask);
    		SYS_MEM_CLEAR(areaId);
			
			ridx = rindex(array[i], 'm');
			if(ridx == NULL)
			{
				goto cgi_end;
			}
			*ridx = '\0';
			ridx++;
			osal_memcpy(networkMask, ridx, strlen(ridx));

			ridx = rindex(array[i], 'n');
			*ridx = '\0';
			ridx++;
			osal_memcpy(networkIp, ridx, strlen(ridx));

			osal_memcpy(areaId, array[i], strlen(array[i]));

			STR2IPV4(areaId, &ospfAreaId);
			STR2IPV4(networkIp, &network);
			STR2IPV4(networkMask, &mask);
			if(network && mask)
			{
				CGI_SET_ERR_HDL(sal_l3_ospf_network_area_set(ENABLED, network, mask, ospfAreaId, ospf_prosseId), ret);
			}
			else if(network == 0)
			{
				CGI_SET_ERR_HDL(sal_l3_ospf_network_area_set(ENABLED, network, mask, ospfAreaId, ospf_prosseId), ret);
			}
		}
	}
		
cgi_end:
	cgi_set_respGeneral_send(ret);

	return SYS_ERR_OK;
}

