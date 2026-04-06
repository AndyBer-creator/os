
#include <include/cgi.h>

static int32 cgi_l3_intfV4_get(void);
static int32 cgi_l3_intfV4Add_get(void);
static int32 cgi_l3_intfV4Edit_get(void);
static int32 cgi_l3_routeV4_get(void);
static int32 cgi_l3_routeV4Add_get(void);
static int32 cgi_l3_routeV4Edit_get(void);
static int32 cgi_l3_v4Arp_get(void);
static int32 cgi_l3_v4ArpAdd_get(void);
static int32 cgi_l3_v4ArpProxy_get(void);
static int32 cgi_l3_intfV6_get(void);
static int32 cgi_l3_intfV6Add_get(void);
static int32 cgi_l3_intfV6Edit_get(void);
static int32 cgi_l3_addrV6_get(void);
static int32 cgi_l3_addrV6Add_get(void);
static int32 cgi_l3_routeV6_get(void);
static int32 cgi_l3_routeV6Add_get(void);
static int32 cgi_l3_routeV6Edit_get(void);
static int32 cgi_l3_v6Neigh_get(void);
static int32 cgi_l3_v6NeighAdd_get(void);

const cgi_cmdmap_t cmdGetL3Tbl[] =
{
    {CGI_CMD_L3_V4INTF,        cgi_l3_intfV4_get},
    {CGI_CMD_L3_V4INTFADD,     cgi_l3_intfV4Add_get},
    {CGI_CMD_L3_V4INTFEDIT,    cgi_l3_intfV4Edit_get},
    {CGI_CMD_L3_V4ROUTE,       cgi_l3_routeV4_get},
    {CGI_CMD_L3_V4ROUTEADD,    cgi_l3_routeV4Add_get},
    {CGI_CMD_L3_V4ROUTEEDIT,   cgi_l3_routeV4Edit_get},
    {CGI_CMD_L3_V4ARP,         cgi_l3_v4Arp_get},
    {CGI_CMD_L3_V4ARPADD,      cgi_l3_v4ArpAdd_get},
    {CGI_CMD_L3_V4ARPPROXY,    cgi_l3_v4ArpProxy_get},
    {CGI_CMD_L3_V6INTF,        cgi_l3_intfV6_get},
    {CGI_CMD_L3_V6INTFADD,     cgi_l3_intfV6Add_get},
    {CGI_CMD_L3_V6INTFEDIT,    cgi_l3_intfV6Edit_get},
    {CGI_CMD_L3_V6ADDR,        cgi_l3_addrV6_get},
    {CGI_CMD_L3_V6ADDRADD,     cgi_l3_addrV6Add_get},
    {CGI_CMD_L3_V6ROUTE,       cgi_l3_routeV6_get},
    {CGI_CMD_L3_V6ROUTEADD,    cgi_l3_routeV6Add_get},
    {CGI_CMD_L3_V6ROUTEEDIT,   cgi_l3_routeV6Edit_get},
    {CGI_CMD_L3_V6NEIGH,       cgi_l3_v6Neigh_get},
    {CGI_CMD_L3_V6NEIGHADD,    cgi_l3_v6NeighAdd_get},
    {NULL, NULL}
};

static int32 cgi_l3_intfV4_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *intfs = NULL;
    cJSON *intf = NULL;
    uint32 i = 0;
    uint32 cnt = 0;
    char ipStr[CAPA_IPV4_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    sys_l3_ipv4Db_t *pCurr = NULL;
    sys_vid_t dfltVid = 0;
	sys_l3_ipv4_t firstIpv4;

    sal_vlan_defaultVid_get(&dfltVid);

    CGI_GET_JSON_CREATE(root, data);

    intfs = cJSON_CreateArray();

    FOR_EACH_L3_IPV4(i, pCurr)
    {
        intf = cJSON_CreateObject();

        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&pCurr->intfId, intfName);
        cJSON_AddStringToObject(intf, CGI_L3_INTF_NAME, intfName);

        if (SYS_L3_INTFTYPE_VLAN == pCurr->intfId.type && pCurr->intfId.id == dfltVid)
        {
            sys_enable_t dhcpEnbl;

            sal_l3_adminIntfDhcpV4_get(&pCurr->intfId, &dhcpEnbl);

            if (SYS_L3_IPTYPE_STATIC == pCurr->list.type)
            {
                if (ENABLED == dhcpEnbl)
                    cJSON_AddNumberToObject(intf, CGI_L3_INTF_TYPE, 2);
                else
                    cJSON_AddNumberToObject(intf, CGI_L3_INTF_TYPE, 1);
            }
            else
            {
                cJSON_AddNumberToObject(intf, CGI_L3_INTF_TYPE, 0);
            }

        }
        else
        {
            if (SYS_L3_IPTYPE_STATIC == pCurr->list.type)
                cJSON_AddNumberToObject(intf, CGI_L3_INTF_TYPE, 1);
            else
                cJSON_AddNumberToObject(intf, CGI_L3_INTF_TYPE, 0);
        }

        SYS_MEM_CLEAR(ipStr);
        sys_util_ip2Str(pCurr->ip.addr, ipStr);
        cJSON_AddStringToObject(intf, CGI_L3_IPADDR, ipStr);

        SYS_MEM_CLEAR(ipStr);
        sys_util_ip2Str(IPV4_PFX2MASK(pCurr->ip.plen), ipStr);
        cJSON_AddStringToObject(intf, CGI_L3_IPMASK, ipStr);

        cJSON_AddNumberToObject(intf, CGI_L3_INTF_STATUS, (0 == pCurr->ip.addr) ? 5 : 1);
		SYS_MEM_CLEAR(firstIpv4);
        if(sal_l3_intf_primary_IPv4_find(&pCurr->intfId, &firstIpv4) == SYS_ERR_OK)
        {
			if(osal_memcmp(&firstIpv4, &pCurr->ip, sizeof(sys_l3_ipv4_t)) == 0)
			{
				cJSON_AddStringToObject(intf, CGI_L3_ROLES, "txtIpPrimary");
			}
			else
			{
				cJSON_AddStringToObject(intf, CGI_L3_ROLES, "txtIpSub");
			}
        }

        cJSON_AddItemToArray(intfs, intf);
        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_L3_INTFS, intfs);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV4Add_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vids = NULL;
    uint32 i = 0;
    sys_vid_t vid;
    sys_vlan_adminEntry_t adminEntry;

    CGI_GET_JSON_CREATE(root, data);

    vids = cJSON_CreateArray();

    FOR_EACH_VLAN_STATIC_ENTRY_IDX(i)
    {
        SYS_MEM_CLEAR(adminEntry);

        SYS_ERR_CONTINUE(sal_vlan_adminEntrySort_get(i, &vid, &adminEntry));

        cJSON_AddItemToArray(vids, cJSON_CreateNumber(vid));
    }

    cJSON_AddItemToObject(data, CGI_VLAN, vids);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV4Edit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;
    char intfName[CAPA_L3_INTFNAME_LEN];
    char ipStr[CAPA_IPV4_STR_LEN];
    sys_l3_intfId_t intfId;
	char* token = NULL;
	sys_l3_ipv4_t ip;
	sys_l3_ipv4Db_t *pCurr = NULL;
	
    SYS_MEM_CLEAR(intfName);
    SYS_MEM_CLEAR(intfId);
    SYS_MEM_CLEAR(ip);

    CGI_GET_JSON_CREATE(root, data);

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_NAME, value))
    {

        token = strtok(value, "_");
        if( token != NULL ) 
        {
		    INTFNAME2ID(token, &intfId);
		}

		token = strtok(NULL, "_");
		if( token != NULL ) 
        {
		    STR2IPV4(token, &ip.addr);
		}
		
		token = strtok(NULL, "_");
		if( token != NULL ) 
        {
            FOR_EACH_L3_IPV4_IN_INTF(&intfId, pCurr)
		    {
				if (SYS_L3_IPTYPE_STATIC == pCurr->list.type)
					cJSON_AddNumberToObject(data, CGI_L3_INTF_TYPE, 0);
				else 
					cJSON_AddNumberToObject(data, CGI_L3_INTF_TYPE, 1);

				if(pCurr->ip.addr == ip.addr)
				{
				    
			        SYS_MEM_CLEAR(intfName);
			        INTFID2NAME(&pCurr->intfId, intfName);
			        cJSON_AddStringToObject(data, CGI_L3_INTF_NAME, intfName);
			        
					if (0 == osal_strcmp(token, "txtIpSub"))
				    {
						cJSON_AddNumberToObject(data, CGI_L3_ROLES, 0);
				    }
				    else if(0 == osal_strcmp(token, "txtIpPrimary"))
				    {
						cJSON_AddNumberToObject(data, CGI_L3_ROLES, 1);
				    }

			        SYS_MEM_CLEAR(ipStr);
			        sys_util_ip2Str(pCurr->ip.addr, ipStr);
			        cJSON_AddStringToObject(data, CGI_L3_IPADDR, ipStr);

			        SYS_MEM_CLEAR(ipStr);
			        sys_util_ip2Str(IPV4_PFX2MASK(pCurr->ip.plen), ipStr);
			        cJSON_AddStringToObject(data, CGI_L3_IPMASK, ipStr);
				}
		    }
		}
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV4_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *rules = NULL;
    cJSON *rule = NULL;
    char ipStr[CAPA_IPV4_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    uint32 i = 0;
    uint32 cnt = 0;
    sys_l3_routeV4InfoDb_t infoDb;

    CGI_GET_JSON_CREATE(root, data);

    SYS_MEM_CLEAR(infoDb);
    SYS_ERR_CHK(sal_l3_routeV4Info_get(&infoDb));

    rules = cJSON_CreateArray();

    for (i = 0; i < infoDb.num; i++)
    {
        rule = cJSON_CreateObject();

        SYS_MEM_CLEAR(ipStr);
        sys_util_ip2Str(infoDb.info[i].route.dst, ipStr);
        cJSON_AddStringToObject(rule, CGI_L3_ROUTE_DST, ipStr);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_PLEN, infoDb.info[i].route.dstLen);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_TYPE, infoDb.info[i].type);

        if (SYS_L3_ROUTETYPE_CONNECT != infoDb.info[i].type)
        {
            
            SYS_MEM_CLEAR(ipStr);
            sys_util_ip2Str(infoDb.info[i].route.gw, ipStr);
            cJSON_AddStringToObject(rule, CGI_L3_ROUTE_GW, ipStr);

            cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_METRIC, infoDb.info[i].metric);

            cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_DISTANCE, infoDb.info[i].distance);
        }

        SYS_MEM_CLEAR(intfName);
        if (infoDb.info[i].select)
        {
            INTFID2NAME(&infoDb.info[i].route.ointf, intfName);
            intfName[osal_strlen(intfName)] = '*';
        }
        else
        {
            INTFID2NAME(&infoDb.info[i].route.ointf, intfName);
        }
        cJSON_AddStringToObject(rule, CGI_L3_INTF_NAME, intfName);

        cJSON_AddItemToArray(rules, rule);
        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_L3_ROUTES, rules);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV4Add_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_MIN, BOND_L3_STATICROUTE_METRIC_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_MAX, BOND_L3_STATICROUTE_METRIC_MAX);
    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_DFLT, DFLT_L3_STATICROUTE_METRIC);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV4Edit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *rules = NULL;
    cJSON *rule = NULL;
    char ipStr[CAPA_IPV4_STR_LEN];
    uint32 i = 0;
    sys_l3_staticRouteV4_t routes;

    SYS_MEM_CLEAR(routes);
    SYS_ERR_CHK(sal_l3_staticRouteV4_get(&routes));

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_MIN, BOND_L3_STATICROUTE_METRIC_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_MAX, BOND_L3_STATICROUTE_METRIC_MAX);
    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_DFLT, DFLT_L3_STATICROUTE_METRIC);

    rules = cJSON_CreateArray();

    for (i = 0; i < routes.num; i++)
    {
        rule = cJSON_CreateObject();

        cJSON_AddNumberToObject(rule, "idx", i);

        SYS_MEM_CLEAR(ipStr);
        sys_util_ip2Str(routes.route[i].dst, ipStr);
        cJSON_AddStringToObject(rule, CGI_L3_ROUTE_DST, ipStr);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_PLEN, routes.route[i].dstLen);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_TYPE, SYS_L3_ROUTETYPE_STATIC);

        SYS_MEM_CLEAR(ipStr);
        sys_util_ip2Str(routes.route[i].gw, ipStr);
        cJSON_AddStringToObject(rule, CGI_L3_ROUTE_GW, ipStr);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_METRIC, routes.route[i].pri);

        cJSON_AddItemToArray(rules, rule);
    }

    cJSON_AddItemToObject(data, CGI_L3_ROUTES, rules);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v4Arp_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *arps = NULL;
    cJSON *arp = NULL;
    FILE *fp = NULL;
    char buf[SYS_BUF256_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    char *tok[4];
    uint32 addrNum = 0;
    uint32 arpAgeTime = 0;
    sys_l3_intfId_t intfId;

    CGI_GET_JSON_CREATE(root, data);

    sal_l3_neigh_ageTime_get(&arpAgeTime);
    cJSON_AddNumberToObject(data, CGI_L3_NEIGH_AGE, arpAgeTime);
    cJSON_AddNumberToObject(data, CGI_L3_NEIGH_AGE_MIN, BOND_L3_NEIGH_AGE_TIME_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_NEIGH_AGE_MAX, BOND_L3_NEIGH_AGE_TIME_MAX);
    cJSON_AddNumberToObject(data, CGI_L3_NEIGH_AGE_DFLT, DFLT_L3_NEIGH_AGE_TIME);

    if (NULL == (fp = popen("cat /proc/"L3_NEIGH_V4_PROC_NAME, "r")))
    {
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);
        goto cgi_end;
    }

    arps = cJSON_CreateArray();

    SYS_MEM_CLEAR(buf);
    while (fgets(buf, sizeof(buf), fp))
    {
        tok[0] = strtok(buf, ", "); 

        if (!isdigit(tok[0][0])) 
            break;

        tok[1] = strtok(NULL, ", "); 
        tok[2] = strtok(NULL, ", "); 
        tok[3] = strtok(NULL, ", "); 

        arp = cJSON_CreateObject();

        SYS_MEM_CLEAR(intfName);
        SYS_MEM_CLEAR(intfId);
        intfId.type = SYS_L3_INTFTYPE_VLAN;
        intfId.id = SYS_STR2UINT(tok[0]);
        INTFID2NAME(&intfId, intfName);

        cJSON_AddStringToObject(arp, CGI_L3_INTF_NAME, intfName);
        cJSON_AddStringToObject(arp, CGI_L3_NEIGH_IP, tok[1]);
        cJSON_AddStringToObject(arp, CGI_L3_NEIGH_MAC, tok[2]);
        cJSON_AddNumberToObject(arp, CGI_VLAN, intfId.id);
        cJSON_AddBoolToObject(arp, CGI_L3_NEIGH_STATIC, (tok[3][0] == '1'));

        cJSON_AddItemToArray(arps, arp);
        addrNum++;
    }

    pclose(fp);

    if (addrNum > 0)
        cJSON_AddItemToObject(data, CGI_L3_NEIGH_ENTRY, arps);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

cgi_end:
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v4ArpAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vids = NULL;
    uint32 i = 0;
    sys_vid_t vid;
    sys_vlan_adminEntry_t adminEntry;

    CGI_GET_JSON_CREATE(root, data);

    vids = cJSON_CreateArray();

    FOR_EACH_VLAN_STATIC_ENTRY_IDX(i)
    {
        SYS_MEM_CLEAR(adminEntry);

        SYS_ERR_CONTINUE(sal_vlan_adminEntrySort_get(i, &vid, &adminEntry));

        cJSON_AddItemToArray(vids, cJSON_CreateNumber(vid));
    }

    cJSON_AddItemToObject(data, CGI_VLAN, vids);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v4ArpProxy_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_enable_t enable = DISABLED;

    CGI_GET_JSON_CREATE(root, data);

    sal_l3_arpProxy_get(0, &enable);
    cJSON_AddBoolToObject(data, CGI_L3_ARP_PROXY, enable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV6_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *intfs = NULL;
    cJSON *intf = NULL;
    uint32 i = 0;
    uint32 cnt = 0;
    char intfName[CAPA_L3_INTFNAME_LEN];
    sys_l3_intfId_t intfId;
    sys_vid_t vid;
    sys_enable_t fwdEnbl = 0;
    sys_vlanmask_t existVmsk;
    sys_l3_ipv6Admin_t ipv6Admin;

    SYS_MEM_CLEAR(existVmsk);
    SYS_MEM_CLEAR(ipv6Admin);

    CGI_GET_JSON_CREATE(root, data);

    sal_l3_fwdIPv6Enable_get(&fwdEnbl);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_FWDENBL, fwdEnbl);

    intfs = cJSON_CreateArray();

    intfId.type = SYS_L3_INTFTYPE_LO;
    for (i = 0; i < CAPA_L3_INTFLO_NUM; i++)
    {
        intfId.id = i;

        if (!IS_L3_INTF_EXIST(&intfId))
            continue;

        if (!IS_L3_INTF_IPV6ENBL(&intfId))
            continue;

        intf = cJSON_CreateObject();

        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&intfId, intfName);
        cJSON_AddStringToObject(intf, CGI_L3_INTF_NAME, intfName);

        cJSON_AddItemToArray(intfs, intf);

        cnt++;
    }

    sal_vlan_adminEntryExist_get(&existVmsk);

    intfId.type = SYS_L3_INTFTYPE_VLAN;
    FOR_EACH_VID_IN_VLANMASK(vid, existVmsk)
    {
        intfId.id = vid;

        if (!IS_L3_INTF_EXIST(&intfId))
            continue;

        if (!IS_L3_INTF_IPV6ENBL(&intfId))
            continue;

        SYS_MEM_CLEAR(ipv6Admin);
        SYS_ERR_CONTINUE(sal_l3_adminIntfIPv6_get(&intfId, &ipv6Admin));

        intf = cJSON_CreateObject();

        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&intfId, intfName);
        cJSON_AddStringToObject(intf, CGI_L3_INTF_NAME, intfName);

        cJSON_AddNumberToObject(intf, CGI_L3_IPV6_AUTOCONF, ipv6Admin.autoconfEnbl);

        cJSON_AddNumberToObject(intf, CGI_L3_IPV6_DADATTEMPT, ipv6Admin.dadTxNum);

        cJSON_AddNumberToObject(intf, CGI_L3_IPV6_STATELESS, ipv6Admin.dhcp6c.stateless);

        cJSON_AddNumberToObject(intf, CGI_L3_IPV6_REFRESH, ipv6Admin.dhcp6c.refreshtime);

        cJSON_AddNumberToObject(intf, CGI_L3_IPV6_MINREFRESH, ipv6Admin.dhcp6c.min_refreshtime);

        cJSON_AddItemToArray(intfs, intf);
        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_L3_INTFS, intfs);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV6Add_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vids = NULL;
    uint32 i = 0;
    sys_vid_t vid;
    sys_vlan_adminEntry_t adminEntry;

    CGI_GET_JSON_CREATE(root, data);

    vids = cJSON_CreateArray();

    FOR_EACH_VLAN_STATIC_ENTRY_IDX(i)
    {
        SYS_MEM_CLEAR(adminEntry);

        SYS_ERR_CONTINUE(sal_vlan_adminEntrySort_get(i, &vid, &adminEntry));

        cJSON_AddItemToArray(vids, cJSON_CreateNumber(vid));
    }

    cJSON_AddItemToObject(data, CGI_VLAN, vids);

    cJSON_AddNumberToObject(data, CGI_L3_IPV6_AUTOCONF_DFLT, DFLT_L3_IPV6_AUTOCONF_EBL);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_DADATTEMPT_DFLT, DFLT_L3_IPV6_DADATTEMPT);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_DADATTEMPT_MIN, BOND_L3_IPV6_DADATTEMPT_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_DADATTEMPT_MAX, BOND_L3_IPV6_DADATTEMPT_MAX);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_STATELESS_DFLT, DFLT_L3_IPV6_DHCPC_STATELESS_EBL);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_REFRESH_DFLT, DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_REFRESH_MIN, BOND_L3_IPV6_STATELESS_REFRESH_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_REFRESH_MAX, BOND_L3_IPV6_STATELESS_REFRESH_MAX);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_MINREFRESH_DFLT, DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_MINREFRESH_MIN, BOND_L3_IPV6_STATELESS_MINREFRESH_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_MINREFRESH_MAX, BOND_L3_IPV6_STATELESS_MINREFRESH_MAX);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV6Edit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;
    char intfName[CAPA_L3_INTFNAME_LEN];
    sys_l3_intfId_t intfId;
    sys_l3_ipv6Admin_t adminIpv6;

    SYS_MEM_CLEAR(intfName);
    SYS_MEM_CLEAR(intfId);

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_NAME, value))
        osal_strncpy(intfName, value, sizeof(intfName));

    INTFNAME2ID(intfName, &intfId);
    SYS_MEM_CLEAR(adminIpv6);
    SYS_ERR_CHK(sal_l3_adminIntfIPv6_get(&intfId, &adminIpv6));

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_L3_IPV6_AUTOCONF, adminIpv6.autoconfEnbl);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_AUTOCONF_DFLT, DFLT_L3_IPV6_AUTOCONF_EBL);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_DADATTEMPT, adminIpv6.dadTxNum);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_DADATTEMPT_DFLT, DFLT_L3_IPV6_DADATTEMPT);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_DADATTEMPT_MIN, BOND_L3_IPV6_DADATTEMPT_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_DADATTEMPT_MAX, BOND_L3_IPV6_DADATTEMPT_MAX);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_STATELESS, adminIpv6.dhcp6c.stateless);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_STATELESS_DFLT, DFLT_L3_IPV6_DHCPC_STATELESS_EBL);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_REFRESH, adminIpv6.dhcp6c.refreshtime);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_REFRESH_DFLT, DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_REFRESH_MIN, BOND_L3_IPV6_STATELESS_REFRESH_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_REFRESH_MAX, BOND_L3_IPV6_STATELESS_REFRESH_MAX);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_MINREFRESH, adminIpv6.dhcp6c.min_refreshtime);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_MINREFRESH_DFLT, DFLT_L3_IPV6_DHCPC_STATELESS_REFRESH_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_MINREFRESH_MIN, BOND_L3_IPV6_STATELESS_MINREFRESH_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_IPV6_MINREFRESH_MAX, BOND_L3_IPV6_STATELESS_MINREFRESH_MAX);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_addrV6_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *intfs = NULL;
    cJSON *intf = NULL;
    cJSON *addrs = NULL;
    cJSON *addr = NULL;
    uint32 i = 0;
    uint32 cnt = 0;
    char *value = NULL;
    char intfName[CAPA_L3_INTFNAME_LEN];
    char ipStr[CAPA_IPV6_STR_LEN];
    sys_l3_intfId_t intfId;
    sys_l3_intfId_t selIntfId;
    sys_vid_t vid;
    sys_vid_t firstVid = 0;
    sys_vlanmask_t existVmsk;
    sys_l3_ipv6Db_t *pCurr = NULL;
    sys_l3_ipv6MaddrInfo_t maddrs;

    SYS_MEM_CLEAR(existVmsk);
    SYS_MEM_CLEAR(intfName);

    CGI_GET_JSON_CREATE(root, data);

    intfs = cJSON_CreateArray();

    sal_vlan_adminEntryExist_get(&existVmsk);

    intfId.type = SYS_L3_INTFTYPE_VLAN;
    FOR_EACH_VID_IN_VLANMASK(vid, existVmsk)
    {
        intfId.id = vid;

        if (!IS_L3_INTF_EXIST(&intfId))
            continue;

        if (!IS_L3_INTF_IPV6ENBL(&intfId))
            continue;

        if (0 == firstVid)
            firstVid = vid;

        intf = cJSON_CreateObject();

        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&intfId, intfName);
        cJSON_AddStringToObject(intf, CGI_NAME, intfName);
        cJSON_AddStringToObject(intf, CGI_VALUE, intfName);

        cJSON_AddItemToArray(intfs, intf);

        cnt++;
    }

    SYS_MEM_CLEAR(intfName);
    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_NAME, value))
        osal_strncpy(intfName, value, sizeof(intfName));

    if (0 == osal_strlen(intfName))
    {
        selIntfId.type = SYS_L3_INTFTYPE_VLAN;
        selIntfId.id = firstVid;
    }
    else
    {
        INTFNAME2ID(intfName, &selIntfId);
    }

    intfId.type = SYS_L3_INTFTYPE_LO;
    for (i = 0; i < CAPA_L3_INTFLO_NUM; i++)
    {
        intfId.id = i;

        if (!IS_L3_INTF_EXIST(&intfId))
            continue;

        if (!IS_L3_INTF_IPV6ENBL(&intfId))
            continue;

        intf = cJSON_CreateObject();

        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&intfId, intfName);
        cJSON_AddStringToObject(intf, CGI_NAME, intfName);
        cJSON_AddStringToObject(intf, CGI_VALUE, intfName);

        cJSON_AddItemToArray(intfs, intf);

        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_L3_INTFS, intfs);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    cnt = 0;
    addrs = cJSON_CreateArray();

    FOR_EACH_L3_IPV6_IN_INTF(&selIntfId, pCurr)
    {
        sys_l3_ipv6DadSts_t dadSts = SYS_L3_IPV6_DADSTS_NONE;

        addr = cJSON_CreateObject();

        cJSON_AddNumberToObject(addr, CGI_L3_ADDRTYPE, pCurr->list.type);

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(pCurr->ip.addr, ipStr);
        cJSON_AddStringToObject(addr, CGI_L3_IPADDR, ipStr);

        cJSON_AddNumberToObject(addr, CGI_L3_PLEN, pCurr->ip.plen);

        sal_l3_intfIPv6DadStatus_get(&pCurr->intfId, &pCurr->ip, &dadSts);

        if (SYS_L3_IPV6_DADSTS_NONE != dadSts)
            cJSON_AddNumberToObject(addr, CGI_L3_IPV6_DADSTATUS, dadSts);

        cJSON_AddItemToArray(addrs, addr);

        cnt++;
    }

    SYS_MEM_CLEAR(maddrs);

    sal_l3_intfIPv6Maddr_get(&selIntfId, &maddrs);

    for (i = 0; i < maddrs.num; i++)
    {
        addr = cJSON_CreateObject();

        cJSON_AddNumberToObject(addr, CGI_L3_ADDRTYPE, SYS_L3_IPTYPE_V6MCAST);

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(maddrs.addrs[i], ipStr);
        cJSON_AddStringToObject(addr, CGI_L3_IPADDR, ipStr);

        cJSON_AddItemToArray(addrs, addr);

        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_L3_IPADDRS, addrs);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_addrV6Add_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_EMPTY, 1);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV6_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *rules = NULL;
    cJSON *rule = NULL;
    char ipStr[CAPA_IPV6_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    uint32 i = 0;
    uint32 cnt = 0;
    sys_l3_routeV6InfoDb_t infoDb;

    CGI_GET_JSON_CREATE(root, data);

    SYS_MEM_CLEAR(infoDb);
    SYS_ERR_CHK(sal_l3_routeV6Info_get(&infoDb));

    rules = cJSON_CreateArray();

    for (i = 0; i < infoDb.num; i++)
    {
        rule = cJSON_CreateObject();

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(infoDb.info[i].route.dst, ipStr);
        cJSON_AddStringToObject(rule, CGI_L3_ROUTE_DST, ipStr);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_PLEN, infoDb.info[i].route.dstLen);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_TYPE, infoDb.info[i].type);

        if (SYS_L3_ROUTETYPE_CONNECT != infoDb.info[i].type)
        {
            
            SYS_MEM_CLEAR(ipStr);
            IPV62STR(infoDb.info[i].route.gw, ipStr);
            cJSON_AddStringToObject(rule, CGI_L3_ROUTE_GW, ipStr);

            cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_METRIC, infoDb.info[i].metric);

            cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_DISTANCE, infoDb.info[i].distance);
        }

        SYS_MEM_CLEAR(intfName);
        if (infoDb.info[i].select)
        {
            INTFID2NAME(&infoDb.info[i].route.ointf, intfName);
            intfName[osal_strlen(intfName)] = '*';
        }
        else
        {
            INTFID2NAME(&infoDb.info[i].route.ointf, intfName);
        }
        cJSON_AddStringToObject(rule, CGI_L3_INTF_NAME, intfName);

        cJSON_AddItemToArray(rules, rule);
        cnt++;
    }

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_L3_ROUTES, rules);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV6Add_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_MIN, BOND_L3_STATICROUTE_METRIC_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_MAX, BOND_L3_STATICROUTE_METRIC_MAX);
    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_DFLT, DFLT_L3_STATICROUTE_METRIC);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV6Edit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *rules = NULL;
    cJSON *rule = NULL;
    char ipStr[CAPA_IPV6_STR_LEN];
    uint32 i = 0;
    sys_l3_staticRouteV6_t routes;

    SYS_MEM_CLEAR(routes);
    SYS_ERR_CHK(sal_l3_staticRouteV6_get(&routes));

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_MIN, BOND_L3_STATICROUTE_METRIC_MIN);
    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_MAX, BOND_L3_STATICROUTE_METRIC_MAX);
    cJSON_AddNumberToObject(data, CGI_L3_ROUTE_METRIC_DFLT, DFLT_L3_STATICROUTE_METRIC);

    rules = cJSON_CreateArray();

    for (i = 0; i < routes.num; i++)
    {
        rule = cJSON_CreateObject();

        cJSON_AddNumberToObject(rule, "idx", i);

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(routes.route[i].dst, ipStr);
        cJSON_AddStringToObject(rule, CGI_L3_ROUTE_DST, ipStr);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_PLEN, routes.route[i].dstLen);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_TYPE, 2);

        SYS_MEM_CLEAR(ipStr);
        IPV62STR(routes.route[i].gw, ipStr);
        cJSON_AddStringToObject(rule, CGI_L3_ROUTE_GW, ipStr);

        cJSON_AddNumberToObject(rule, CGI_L3_ROUTE_METRIC, routes.route[i].pri);

        cJSON_AddItemToArray(rules, rule);
    }

    cJSON_AddItemToObject(data, CGI_L3_ROUTES, rules);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v6Neigh_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *arps = NULL;
    cJSON *arp = NULL;
    FILE *fp = NULL;
    char buf[SYS_BUF256_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    char *tok[5];
    char v6addrStr[CAPA_IPV6_STR_LEN];
    uint32 addrNum = 0;
    sys_l3_intfId_t intfId;
    sys_ipv6_t v6addr;

    CGI_GET_JSON_CREATE(root, data);

    if (NULL == (fp = popen("cat /proc/"L3_NEIGH_V6_PROC_NAME, "r")))
    {
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);
        goto cgi_end;
    }

    arps = cJSON_CreateArray();

    SYS_MEM_CLEAR(buf);
    while (fgets(buf, sizeof(buf), fp))
    {
        tok[0] = strtok(buf, ", "); 

        if (!isdigit(tok[0][0])) 
            break;

        tok[1] = strtok(NULL, ", "); 
        tok[2] = strtok(NULL, ", "); 
        tok[3] = strtok(NULL, ", "); 
        tok[4] = strtok(NULL, ", "); 

        arp = cJSON_CreateObject();

        SYS_MEM_CLEAR(intfName);
        SYS_MEM_CLEAR(intfId);
        intfId.type = SYS_L3_INTFTYPE_VLAN;
        intfId.id = SYS_STR2UINT(tok[0]);
        INTFID2NAME(&intfId, intfName);

        SYS_MEM_CLEAR(v6addr);
        SYS_MEM_CLEAR(v6addrStr);
        STR2IPV6(tok[1], &v6addr);
        IPV62STR(v6addr, v6addrStr);

        cJSON_AddStringToObject(arp, CGI_L3_INTF_NAME, intfName);
        cJSON_AddStringToObject(arp, CGI_L3_NEIGH_IPV6, v6addrStr);
        cJSON_AddStringToObject(arp, CGI_L3_NEIGH_MAC, tok[2]);
        cJSON_AddNumberToObject(arp, CGI_VLAN, intfId.id);
        cJSON_AddBoolToObject(arp, CGI_L3_NEIGH_STATIC, (tok[3][0] == '3'));
        cJSON_AddBoolToObject(arp, CGI_L3_NEIGH_ROUTER, (tok[4][0] == '1'));

        cJSON_AddItemToArray(arps, arp);
        addrNum++;
    }

    pclose(fp);

    if (addrNum > 0)
        cJSON_AddItemToObject(data, CGI_L3_NEIGH_ENTRY, arps);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

cgi_end:
    
    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v6NeighAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vids = NULL;
    uint32 i = 0;
    sys_vid_t vid;
    sys_vlan_adminEntry_t adminEntry;

    CGI_GET_JSON_CREATE(root, data);

    vids = cJSON_CreateArray();

    FOR_EACH_VLAN_STATIC_ENTRY_IDX(i)
    {
        SYS_MEM_CLEAR(adminEntry);

        SYS_ERR_CONTINUE(sal_vlan_adminEntrySort_get(i, &vid, &adminEntry));

        cJSON_AddItemToArray(vids, cJSON_CreateNumber(vid));
    }

    cJSON_AddItemToObject(data, CGI_VLAN, vids);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
