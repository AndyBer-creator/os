
#include <include/cgi.h>
#include <libipc/quaggaTask.h>
#include <libipc/vos.h>
#include <libsal/sal_l3_route_ospf.h>

static int32 cgi_ospf_prop_get(void);

const cgi_cmdmap_t cmdGetOspfTbl[] =
{
    {CGI_CMD_OSPF_CFG,               cgi_ospf_prop_get},
    {NULL, NULL}
};

static int32 cgi_ospf_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *areaId = NULL;
    cJSON *network = NULL;
    int32  enable = ENABLED;
	int32	rc = SYS_ERR_OK;
	char ospfNetworkIp[SYS_BUF32_LEN] = {0};
	char ospfNetworkMask[SYS_BUF32_LEN] = {0};
	char ospfareaId[SYS_BUF32_LEN] = {0};
	OSPF_INFO_T ospfinfo;
	OSPF_AREA_INFO_T areaInfo;
	OSPF_AREA_NETWORK_T areaNetwork;

	SYS_MEM_CLEAR(ospfinfo);

	ospfinfo.instance = 1;

    rc = sal_ospf_Info_get(&ospfinfo);
    if(rc != SYS_ERR_OK)
	{
		enable = DISABLED;
	}

    CGI_GET_JSON_CREATE(root, data);
    areaId = cJSON_CreateArray();

    cJSON_AddBoolToObject(data, CGI_STATE, enable);
    cJSON_AddItemToObject(data, CGI_OSPF_AREA, areaId);

	SYS_MEM_CLEAR(areaInfo);
    areaInfo.bFirst = TRUE;
    areaInfo.instance = ospfinfo.instance;
    while (1)
    {
        rc = sal_ospf_area_info_get(DATA_OP_GETNEXT, &areaInfo);
        if (SYS_ERR_OK != rc)
        {
            goto cgi_end;
        }
        IPV42STR(areaInfo.area_id, ospfareaId);
	    areaNetwork.networkIp = 0;
    	areaNetwork.prefixlen = 32;
    	areaNetwork.instance = areaInfo.instance;
    	areaNetwork.areaId = areaInfo.area_id;
	    while(1)
    	{
			rc = sal_ospf_area_network_get(DATA_OP_GETNEXT, &areaNetwork);
			if (SYS_ERR_OK != rc)
	        {
	            break;
	        }
	        SYS_MEM_CLEAR(ospfNetworkIp);
	        IPV42STR(areaNetwork.networkIp, ospfNetworkIp);
	        IPV42STR(IPV4_PFX2MASK(areaNetwork.prefixlen), ospfNetworkMask);
	        network = cJSON_CreateObject();
	        cJSON_AddItemToArray(areaId, network);
	        cJSON_AddStringToObject(network, CGI_OSPF_AREA_ID, ospfareaId);
	        cJSON_AddStringToObject(network, CGI_OSPF_NETWORKADDR, ospfNetworkIp);
       		cJSON_AddStringToObject(network, CGI_OSPF_NETWORKMASK, ospfNetworkMask);
    	}
    }
cgi_end:

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
