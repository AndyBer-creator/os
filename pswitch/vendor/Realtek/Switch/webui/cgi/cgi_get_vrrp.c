
#include <include/cgi.h>
#include <libipc/quaggaTask.h>
#include <libipc/vos.h>
#include <libsal/sal_l3_vrrp.h>

static int32 cgi_vrrp_V4_get(void);
static int32 cgi_vrrp_V4Add_get(void);
static int32 cgi_vrrp_V4Edit_get(void);

const cgi_cmdmap_t cmdGetVrrpTbl[] =
{
    {CGI_CMD_VRRP_V4INTF,        cgi_vrrp_V4_get},
    {CGI_CMD_VRRP_V4INTFADD,     cgi_vrrp_V4Add_get},
    {CGI_CMD_VRRP_V4INTFEDIT,    cgi_vrrp_V4Edit_get},
    {NULL, NULL}
};

static int32 cgi_vrrp_V4_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *intfs = NULL;
    cJSON *intf = NULL;
    
	vrrp_rt vsrvInfo;
    char ipStr[CAPA_IPV4_STR_LEN];
    
    uint32 j = 0;
    uint32 cnt = 0;
    int iIndex = -1;

    CGI_GET_JSON_CREATE(root, data);

    intfs = cJSON_CreateArray();
	
	iIndex = -1;
	while(1)
	{		
		if ( SYS_ERR_OK != sal_vrrp_vsrv_get_next(iIndex, &vsrvInfo) )
		{
			break;
		}

		iIndex = vsrvInfo.no;
		for (j = 0; j < vsrvInfo.naddr; j++)
		{
	        intf = cJSON_CreateObject();
			
	        cJSON_AddNumberToObject(intf, CGI_VLAN, vsrvInfo.vif.ifindex);
	        cJSON_AddNumberToObject(intf, CGI_VRRP_VRID, vsrvInfo.vrid);
	        //cJSON_AddStringToObject(intf, CGI_VRRP_INTFNAME, vsrvInfo.vif.ifname);
	        IPV42STR(vsrvInfo.vaddr[j].addr, ipStr);
	        cJSON_AddStringToObject(intf, CGI_VRRP_VADDR, ipStr);
	        cJSON_AddNumberToObject(intf, CGI_VRRP_STATE, vsrvInfo.state - 1);
	        cJSON_AddNumberToObject(intf, CGI_VRRP_PRI, vsrvInfo.priority);
			cJSON_AddNumberToObject(intf, CGI_VRRP_ADVER, vsrvInfo.adver_int / VRRP_TIMER_HZ);
			if (vsrvInfo.preempt)
				cJSON_AddBoolToObject(intf, CGI_VRRP_PREEMPT, ENABLED);
			else
				cJSON_AddBoolToObject(intf, CGI_VRRP_PREEMPT, DISABLED);
			cJSON_AddNumberToObject(intf, CGI_VRRP_DELAY, vsrvInfo.delay / VRRP_TIMER_HZ);

			#if 0
			XMORE("track interfaces  : \n");
			for (j = 0;j < vsrvInfo.niftrack; j++)
			{
				usVid = vsrvInfo.iftrack[j];
				if (usVid == 0)
				{
					continue;
				}
				l3info.type = SYS_L3_INTFTYPE_VLAN;
				l3info.id = usVid;
				usLinkup = IS_L3_INTF_LINKUP(&l3info);
				if ( !usLinkup )
				{
					XMORE(" VLAN-IF %d, reduced priority: %d, status: down\n", usVid, vsrvInfo.pritrack[j]);
				}
				else
				{
					XMORE(" VLAN-IF %d, reduced priority: %d, status: up\n", usVid, vsrvInfo.pritrack[j]);
				}
			}
			#endif
			
			cJSON_AddItemToArray(intfs, intf);
			cnt++;
		}
	}

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_VRRP_INTFS, intfs);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vrrp_V4Add_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *intfs = NULL;
    
    uint32 i = 0;
    sys_vid_t vid;
    sys_vlan_adminEntry_t adminEntry;

    CGI_GET_JSON_CREATE(root, data);

    intfs = cJSON_CreateArray();

    FOR_EACH_VLAN_STATIC_ENTRY_IDX(i)
    {
        SYS_MEM_CLEAR(adminEntry);

        SYS_ERR_CONTINUE(sal_vlan_adminEntrySort_get(i, &vid, &adminEntry));
		
        cJSON_AddItemToArray(intfs, cJSON_CreateNumber(vid));
    }

    cJSON_AddItemToObject(data, CGI_VLAN, intfs);
	
	cJSON_AddNumberToObject(data, CGI_VRRP_DEFPRI, VRRP_PRIO_DFL);
	cJSON_AddNumberToObject(data, CGI_VRRP_DEFADVER, VRRP_ADVER_DFL);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);
    return SYS_ERR_OK;
}

static int32 cgi_vrrp_V4Edit_get(void)
{
#if 0
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;
    char intfName[CAPA_L3_INTFNAME_LEN];
    
    sys_l3_intfId_t intfId;
    sys_l3_intfAdmin_t adminIntf;

    SYS_MEM_CLEAR(intfName);
    SYS_MEM_CLEAR(intfId);

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_NAME, value))
        osal_strncpy(intfName, value, sizeof(intfName));

    INTFNAME2ID(intfName, &intfId);
    SYS_ERR_CHK(sal_l3_adminIntf_get(&intfId, &adminIntf));

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_L3_INTF_TYPE, adminIntf.ipv4.dhcpEnbl);

#if 0
    if (0 == adminIntf.ipv4.ip.addr)
    {
        cJSON_AddStringToObject(data, CGI_L3_IPADDR, "");
        cJSON_AddStringToObject(data, CGI_L3_IPMASK, "");
        cJSON_AddStringToObject(data, CGI_L3_PLEN, "");
    }
    else
    {
        SYS_MEM_CLEAR(ipStr);
        sys_util_ip2Str(adminIntf.ipv4.ip.addr, ipStr);
        cJSON_AddStringToObject(data, CGI_L3_IPADDR, ipStr);

        SYS_MEM_CLEAR(ipStr);
        sys_util_ip2Str(IPV4_PFX2MASK(adminIntf.ipv4.ip.plen), ipStr);
        cJSON_AddStringToObject(data, CGI_L3_IPMASK, ipStr);

        cJSON_AddNumberToObject(data, CGI_L3_PLEN, adminIntf.ipv4.ip.plen);
    }
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);
#endif
    return SYS_ERR_OK;
}

