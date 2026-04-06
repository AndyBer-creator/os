
#include <include/cgi.h>
#include <libsal/sal_l2.h>

static int32 cgi_l3_intfV4Add_set(void);
static int32 cgi_l3_intfV4Edit_set(void);
static int32 cgi_l3_intfV4Del_set(void);
static int32 cgi_l3_routeV4Add_set(void);
static int32 cgi_l3_routeV4Edit_set(void);
static int32 cgi_l3_routeV4Del_set(void);
static int32 cgi_l3_v4Arp_set(void);
static int32 cgi_l3_v4ArpAdd_set(void);
static int32 cgi_l3_v4ArpEdit_set(void);
static int32 cgi_l3_v4ArpDel_set(void);
static int32 cgi_l3_v4ArpProxy_set(void);
static int32 cgi_l3_intfV6_set(void);
static int32 cgi_l3_intfV6Add_set(void);
static int32 cgi_l3_intfV6Edit_set(void);
static int32 cgi_l3_intfV6Del_set(void);
static int32 cgi_l3_addrV6Add_set(void);
static int32 cgi_l3_addrV6Del_set(void);
static int32 cgi_l3_routeV6Add_set(void);
static int32 cgi_l3_routeV6Edit_set(void);
static int32 cgi_l3_routeV6Del_set(void);
static int32 cgi_l3_v6Neigh_set(void);
static int32 cgi_l3_v6NeighAdd_set(void);
static int32 cgi_l3_v6NeighEdit_set(void);
static int32 cgi_l3_v6NeighDel_set(void);

const cgi_cmdmap_t cmdSetL3Tbl[] =
{
    {CGI_CMD_L3_V4INTFADD,     cgi_l3_intfV4Add_set},
    {CGI_CMD_L3_V4INTFEDIT,    cgi_l3_intfV4Edit_set},
    {CGI_CMD_L3_V4INTFDEL,     cgi_l3_intfV4Del_set},
    {CGI_CMD_L3_V4ROUTEADD,    cgi_l3_routeV4Add_set},
    {CGI_CMD_L3_V4ROUTEEDIT,   cgi_l3_routeV4Edit_set},
    {CGI_CMD_L3_V4ROUTEDEL,    cgi_l3_routeV4Del_set},
    {CGI_CMD_L3_V4ARP,         cgi_l3_v4Arp_set},
    {CGI_CMD_L3_V4ARPADD,      cgi_l3_v4ArpAdd_set},
    {CGI_CMD_L3_V4ARPEDIT,     cgi_l3_v4ArpEdit_set},
    {CGI_CMD_L3_V4ARPDEL,      cgi_l3_v4ArpDel_set},
    {CGI_CMD_L3_V4ARPPROXY,    cgi_l3_v4ArpProxy_set},
    {CGI_CMD_L3_V6INTF,        cgi_l3_intfV6_set},
    {CGI_CMD_L3_V6INTFADD,     cgi_l3_intfV6Add_set},
    {CGI_CMD_L3_V6INTFEDIT,    cgi_l3_intfV6Edit_set},
    {CGI_CMD_L3_V6INTFDEL,     cgi_l3_intfV6Del_set},
    {CGI_CMD_L3_V6ADDRADD,     cgi_l3_addrV6Add_set},
    {CGI_CMD_L3_V6ADDRDEL,     cgi_l3_addrV6Del_set},
    {CGI_CMD_L3_V6ROUTEADD,    cgi_l3_routeV6Add_set},
    {CGI_CMD_L3_V6ROUTEEDIT,   cgi_l3_routeV6Edit_set},
    {CGI_CMD_L3_V6ROUTEDEL,    cgi_l3_routeV6Del_set},
    {CGI_CMD_L3_V6NEIGH,       cgi_l3_v6Neigh_set},
    {CGI_CMD_L3_V6NEIGHADD,    cgi_l3_v6NeighAdd_set},
    {CGI_CMD_L3_V6NEIGHEDIT,   cgi_l3_v6NeighEdit_set},
    {CGI_CMD_L3_V6NEIGHDEL,    cgi_l3_v6NeighDel_set},
    {NULL, NULL}
};

static int32 cgi_l3_intfV4Add_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_intfId_t intfId;
    sys_enable_t dhcpEnbl;
    sys_l3_ipv4_t ip;
    sys_ipv4_t mask;

    SYS_MEM_CLEAR(intfId);
    SYS_MEM_CLEAR(ip);

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_INTFTYPE, value))
    {
        if (CGI_IS_VALUE_EQUAL(value, "vlan"))
        {
            intfId.type = SYS_L3_INTFTYPE_VLAN;
            if (CGI_IS_PARAM_EXIST("vid", value))
                intfId.id = SYS_STR2UINT(value);
        }
        else if (CGI_IS_VALUE_EQUAL(value, "lo"))
        {
            intfId.type = SYS_L3_INTFTYPE_LO;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_ADDRTYPE, value))
        dhcpEnbl = SYS_STR2UINT(value);
    else
        dhcpEnbl = DISABLED;

    CGI_SET_ERR_HDL(sal_l3_adminIntfDhcpV4_set(&intfId, dhcpEnbl), ret);

    if (DISABLED == dhcpEnbl)
    {
        if (CGI_IS_PARAM_EXIST(CGI_L3_IPADDR, value))
        {
            STR2IPV4(value, &ip.addr);
        }

        if (CGI_IS_PARAM_EXIST(CGI_L3_MASKTYPE, value))
        {
            if (0 == osal_strcmp(value, "network"))
            {
                if (CGI_IS_PARAM_EXIST(CGI_L3_IPMASK, value))
                {
                    STR2IPV4(value, &mask);
                    ip.plen = IPV4_MASK2PFX(mask);
                }
            }
            else
            {
                if (CGI_IS_PARAM_EXIST(CGI_L3_PLEN, value))
                {
                    ip.plen = SYS_STR2UINT(value);
                }
            }
        }
        if (CGI_IS_PARAM_EXIST(CGI_L3_ADDROLES, value))
        {
            if (0 == SYS_STR2UINT(value))
            {
            	
    			CGI_SET_ERR_HDL(sal_l3_intfIPv4_sub_add(&intfId, SYS_L3_IPTYPE_STATIC, &ip), ret);
            }
            else
            {
            	
				CGI_SET_ERR_HDL(sal_l3_intfIPv4_add(&intfId, SYS_L3_IPTYPE_STATIC, &ip), ret);
            }
        }

    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV4Edit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_intfId_t intfId;
    sys_enable_t dhcpEnbl;
    sys_l3_ipv4_t ip;
    sys_ipv4_t mask;

    SYS_MEM_CLEAR(intfId);
    SYS_MEM_CLEAR(ip);

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_NAME, value))
    {
        INTFNAME2ID(value, &intfId);
    }

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_ADDRTYPE, value))
        dhcpEnbl = SYS_STR2UINT(value);
    else
        dhcpEnbl = DISABLED;

    CGI_SET_ERR_HDL(sal_l3_adminIntfDhcpV4_set(&intfId, dhcpEnbl), ret);

    if (DISABLED == dhcpEnbl)
    {
        if (CGI_IS_PARAM_EXIST(CGI_L3_IPADDR, value))
        {
            STR2IPV4(value, &ip.addr);
        }

        if (CGI_IS_PARAM_EXIST(CGI_L3_MASKTYPE, value))
        {
            if (0 == osal_strcmp(value, "network"))
            {
                if (CGI_IS_PARAM_EXIST(CGI_L3_IPMASK, value))
                {
                    STR2IPV4(value, &mask);
                    ip.plen = IPV4_MASK2PFX(mask);
                }
            }
            else
            {
                if (CGI_IS_PARAM_EXIST(CGI_L3_PLEN, value))
                {
                    ip.plen = SYS_STR2UINT(value);
                }
            }

            if (CGI_IS_PARAM_EXIST(CGI_L3_ADDROLES, value))
	        {
	            if (0 == SYS_STR2UINT(value))
	            {
	            	
	    			CGI_SET_ERR_HDL(sal_l3_intfIPv4_sub_add(&intfId, SYS_L3_IPTYPE_STATIC, &ip), ret);
	            }
	            else
	            {
	            	
					CGI_SET_ERR_HDL(sal_l3_intfIPv4_add(&intfId, SYS_L3_IPTYPE_STATIC, &ip), ret);
	            }
	        }
        }

    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV4Del_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i = 0;
    uint32 len = 0;
    char **array = NULL;
    sys_l3_intfId_t intfId;

    char *token = NULL;
    sys_l3_ipv4_t ip;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_L3_INTF_NAME, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(intfId);
    		SYS_MEM_CLEAR(ip);
    		
            token = strtok(array[i], "_");
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
			    if (0 == osal_strcmp(token, "txtIpSub"))
			    {
					CGI_SET_ERR_HDL(sal_l3_intfStaticIPv4_sub_del(&intfId, &ip), ret);
			    }
			    else if(0 == osal_strcmp(token, "txtIpPrimary"))
			    {
					CGI_SET_ERR_HDL(sal_l3_intfStaticIPv4_del(&intfId, &ip), ret);
			    }
			}
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV4Add_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_routeV4_t route;
    sys_ipv4_t mask;

    SYS_MEM_CLEAR(route);

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPADDR, value))
        STR2IPV4(value, &route.dst);

    if (CGI_IS_PARAM_EXIST(CGI_L3_MASKTYPE, value))
    {
        if (0 == osal_strcmp(value, "network"))
        {
            if (CGI_IS_PARAM_EXIST(CGI_L3_IPMASK, value))
            {
                STR2IPV4(value, &mask);
                route.dstLen = IPV4_MASK2PFX(mask);
            }
        }
        else
        {
            if (CGI_IS_PARAM_EXIST(CGI_L3_PLEN, value))
            {
                route.dstLen = SYS_STR2UINT(value);
            }
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_GW, value))
        STR2IPV4(value, &route.gw);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_METRIC, value))
        route.pri = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_l3_staticRouteV4_add(&route), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV4Edit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_routeV4_t route;

    SYS_MEM_CLEAR(route);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_DST, value))
        STR2IPV4(value, &route.dst);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_PLEN, value))
        route.dstLen = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_GW, value))
        STR2IPV4(value, &route.gw);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_METRIC, value))
        route.pri = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_l3_staticRouteV4_add(&route), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV4Del_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    char *tok[4];
    uint32 i = 0;
    uint32 j = 0;
    uint32 len = 0;
    sys_l3_routeV4_t route;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_L3_ROUTES, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(route);
            j = 0;

            tok[j] = strtok(array[i], "_\n");
            while (tok[j] != NULL)
            {
                j++;
                tok[j] = strtok(NULL, "_\n");
            }

            STR2IPV4(tok[0], &route.dst);
            route.dstLen = SYS_STR2UINT(tok[1]);
            STR2IPV4(tok[2], &route.gw);

            CGI_SET_ENTRY_ERR_CONTINUE(sal_l3_staticRouteV4_del(&route), array[i]);
        }
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v4Arp_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 oldarpAgeTime = DFLT_L3_NEIGH_AGE_TIME;
    uint32 newarpAgeTime = DFLT_L3_NEIGH_AGE_TIME;
    uint32 clearType = 0;
    sys_l3_intfId_t intf;

    sal_l3_neigh_ageTime_get(&oldarpAgeTime);

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_AGE, value))
    {
        newarpAgeTime = SYS_STR2UINT(value);

        if (newarpAgeTime != oldarpAgeTime)
            CGI_SET_ERR_HDL(sal_l3_neigh_ageTime_set(newarpAgeTime), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_CLEAR_TYPE, value))
    {
        clearType = SYS_STR2UINT(value);

        SYS_MEM_CLEAR(intf);
        if (0 == clearType || 1 == clearType)
            CGI_SET_ERR_HDL(sal_l3_neigh_clear(SYS_L3_NEIGH_V4_DYNAMIC, intf), ret);

        if (0 == clearType || 2 == clearType)
            CGI_SET_ERR_HDL(sal_l3_neigh_clear(SYS_L3_NEIGH_V4_STATIC, intf), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v4ArpAdd_set(void)
{
    int32 ret = SYS_ERR_FAILED;
    sys_l3_intfId_t intf;
    sys_l3_ipv4Db_t *pIPv4Entry = NULL;
    sys_l3_ip_t l3ip;
    sys_ip_t ip;
    sys_vid_t vid;
    sys_mac_t mac;
    sys_enable_t enable;
    uint32 usedCnt = 0;
    char *value = NULL;
    char ipStr[CAPA_IPV4_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    char buf[SYS_BUF256_LEN];

    SYS_MEM_CLEAR(intf);
    SYS_MEM_CLEAR(ipStr);
    SYS_MEM_CLEAR(macStr);

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        vid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_IP, value))
    {
        strlcpy(ipStr, value, sizeof(ipStr));
        if (FALSE == IS_IPV4_UADDRSTR_VALID(ipStr))
        {
            ret = SYS_ERR_IPV4_ADDR;
            goto cgi_end;
        }

        CGI_SET_ERR_HDL(STR2IPV4(ipStr, &l3ip.v4.addr), ret);
        CGI_SET_ERR_HDL(sys_l3_ip2Intf(SYS_L3_AF_IPV4, l3ip, &intf), ret);
        sal_l3_adminIntfDhcpV4_get(&intf, &enable);

        if ( vid != intf.id)
        {
            ret = SYS_ERR_L3_STATIC_NEIGHV4_NOT_MATCH;
            goto cgi_end;
        }

        if (ENABLED == enable)
        {
            ret = SYS_ERR_L3_STATIC_NEIGHV4_INTF_DHCP;
            goto cgi_end;
        }

        SYS_ERR_CHK(sal_l3_neighCnt_get(&usedCnt));

        if (usedCnt >= CAPA_L3_HOST_ROUTE_ENTRY_NUM)
        {
            ret = SYS_ERR_ENTRY_FULL;
            goto cgi_end;
        }

        FOR_EACH_L3_IPV4_IN_INTF(&intf, pIPv4Entry)
        {
            if (l3ip.v4.addr == pIPv4Entry->ip.addr)
            {
                ret = SYS_ERR_L3_STATIC_NEIGHV4_SAME_DUT;
                goto cgi_end;
            }
        }
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_MAC, value))
    {
        strlcpy(macStr, value, sizeof(macStr));
        SYS_MEM_CLEAR(mac);
        SYS_ERR_CHK(sys_util_str2Mac(value, mac.octet));
        CGI_SET_ERR_HDL(sal_l3_neigh_mac_validate(&mac), ret);
    }
    else
        goto cgi_end;

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(intfName);
    INTFID2STR(&intf, intfName);
    sprintf(buf, "ip neigh add %s lladdr %s nud permanent dev %s 1>> /dev/null 2>> /dev/null", ipStr, macStr, intfName);

    if(0 != system(buf))
    {
        SYS_MEM_CLEAR(ip);
        ip.v4 = l3ip.v4.addr;
        sal_l3_hrouteEntry_del(ip, mac);
        SYS_MEM_CLEAR(buf);
        sprintf(buf, "ip neigh change %s lladdr %s nud permanent dev %s 1>> /dev/null 2>> /dev/null",
            ipStr, macStr, intfName);
        system(buf);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v4ArpEdit_set(void)
{
    int32 ret = SYS_ERR_FAILED;
    sys_l3_intfId_t intf;
    sys_enable_t enable;
    sys_mac_t mac;
    sys_ip_t ip;
    char *value = NULL;
    char ipStr[CAPA_IPV4_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    char buf[SYS_BUF256_LEN];

    SYS_MEM_CLEAR(intf);
    SYS_MEM_CLEAR(ipStr);
    SYS_MEM_CLEAR(macStr);
    SYS_MEM_CLEAR(ip);
    SYS_MEM_CLEAR(mac);

    intf.type = SYS_L3_INTFTYPE_VLAN;
    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        intf.id = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_IP, value))
        strlcpy(ipStr, value, sizeof(ipStr));
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_MAC, value))
        strlcpy(macStr, value, sizeof(macStr));
    else
        goto cgi_end;

    sal_l3_adminIntfDhcpV4_get(&intf, &enable);

    if (ENABLED == enable)
    {
        ret = SYS_ERR_L3_STATIC_NEIGHV4_INTF_DHCP;
        goto cgi_end;
    }

    STR2IPV4(ipStr, &ip.v4);
    sys_util_str2Mac(macStr, mac.octet);
    CGI_SET_ERR_HDL(sal_l3_neigh_mac_validate(&mac), ret);
    sal_l3_hrouteEntry_del(ip, mac);

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(intfName);
    INTFID2STR(&intf, intfName);
    sprintf(buf, "ip neigh change %s lladdr %s nud permanent dev %s 1>> /dev/null 2>> /dev/null", ipStr, macStr, intfName);
    system(buf);
    ret = SYS_ERR_OK;
cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v4ArpDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    
    char *ip = NULL;
    char *vlan = NULL;
    char buf[SYS_BUF256_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    uint32 i = 0;
    uint32 len = 0;
    sys_l3_intfId_t intf;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_L3_NEIGH_ENTRY, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            vlan = strtok(array[i], "_");
            ip = strtok(NULL, "_");
            
            intf.type = SYS_L3_INTFTYPE_VLAN;
            intf.id = SYS_STR2UINT(vlan);

            SYS_MEM_CLEAR(buf);
            SYS_MEM_CLEAR(intfName);
            INTFID2STR(&intf, intfName);
            sprintf(buf, "ip neigh del %s dev %s 1>> /dev/null 2>> /dev/null;", ip, intfName);
            system(buf);
        }
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v4ArpProxy_set(void)
{
    int32  ret = SYS_ERR_OK;
    char   *val;

    if (CGI_IS_PARAM_EXIST(CGI_L3_ARP_PROXY, val))
        CGI_SET_ERR_HDL(sal_l3_arpProxy_set(0, ENABLED), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV6_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_FWDENBL, value))
        CGI_SET_ERR_HDL(sal_l3_fwdIPv6Enable_set(ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_l3_fwdIPv6Enable_set(DISABLED), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV6Add_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_intfId_t intfId;
    uint32 num;

    SYS_MEM_CLEAR(intfId);

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_INTFTYPE, value))
    {
        if (CGI_IS_VALUE_EQUAL(value, "vlan"))
        {
            intfId.type = SYS_L3_INTFTYPE_VLAN;
            if (CGI_IS_PARAM_EXIST("vid", value))
                intfId.id = SYS_STR2UINT(value);
        }
        else if (CGI_IS_VALUE_EQUAL(value, "lo"))
        {
            intfId.type = SYS_L3_INTFTYPE_LO;
        }
    }

	CGI_SET_ERR_HDL(sal_l3_adminIntfIPv6Mode_set(&intfId, ENABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_AUTOCONF, value))
        CGI_SET_ERR_HDL(sal_l3_intfIPv6AutoconfEnbl_set(&intfId, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_l3_intfIPv6AutoconfEnbl_set(&intfId, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_DADATTEMPT, value))
    {
        num = SYS_STR2UINT(value);

        CGI_SET_ERR_HDL(sal_l3_intfIPv6DadAttempt_set(&intfId, num), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_STATELESS, value))
        CGI_SET_ERR_HDL(sal_l3_intfIPv6DhcpStatelessEnbl_set(&intfId, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_l3_intfIPv6DhcpStatelessEnbl_set(&intfId, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_REFRESH, value))
    {
        num = SYS_STR2UINT(value);

        CGI_SET_ERR_HDL(sal_l3_intfIPv6DhcpRefreshTime_set(&intfId, num), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_MINREFRESH, value))
    {
        num = SYS_STR2UINT(value);

        CGI_SET_ERR_HDL(sal_l3_intfIPv6DhcpMinRefreshTime_set(&intfId, num), ret);
    }

    CGI_SET_ERR_HDL(sal_l3_intfIPv6DhcpStateless_refresh(&intfId), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV6Edit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_intfId_t intfId;
    uint32 num;

    SYS_MEM_CLEAR(intfId);

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_NAME, value))
        INTFNAME2ID(value, &intfId);

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_AUTOCONF, value))
        CGI_SET_ERR_HDL(sal_l3_intfIPv6AutoconfEnbl_set(&intfId, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_l3_intfIPv6AutoconfEnbl_set(&intfId, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_DADATTEMPT, value))
    {
        num = SYS_STR2UINT(value);

        CGI_SET_ERR_HDL(sal_l3_intfIPv6DadAttempt_set(&intfId, num), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_STATELESS, value))
        CGI_SET_ERR_HDL(sal_l3_intfIPv6DhcpStatelessEnbl_set(&intfId, ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_l3_intfIPv6DhcpStatelessEnbl_set(&intfId, DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_REFRESH, value))
    {
        num = SYS_STR2UINT(value);

        CGI_SET_ERR_HDL(sal_l3_intfIPv6DhcpRefreshTime_set(&intfId, num), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_MINREFRESH, value))
    {
        num = SYS_STR2UINT(value);

        CGI_SET_ERR_HDL(sal_l3_intfIPv6DhcpMinRefreshTime_set(&intfId, num), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_intfV6Del_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i = 0;
    uint32 len = 0;
    char **array = NULL;
    sys_l3_intfId_t intfId;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_L3_INTF_NAME, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(intfId);
            INTFNAME2ID(array[i], &intfId);

            CGI_SET_ERR_HDL(sal_l3_adminIntfIPv6Enbl_set(&intfId, DISABLED), ret);
			CGI_SET_ERR_HDL(sal_l3_adminIntfIPv6Mode_set(&intfId, DISABLED), ret);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_l3_addrV6Add_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_intfId_t intfId;
    sys_l3_ipType_t ipType = SYS_L3_IPTYPE_STATIC;
    sys_l3_ipv6_t ip;

    SYS_MEM_CLEAR(intfId);
    SYS_MEM_CLEAR(ip);

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_NAME, value))
        INTFNAME2ID(value, &intfId);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ADDRTYPE, value))
        ipType = SYS_STR2UINT(value);

    CGI_DBG("ipType=%d intfType=%d intfId=%d", ipType, intfId.type, intfId.id);

    if (SYS_L3_IPTYPE_V6LINKLOCAL == ipType)
    {
        if (CGI_IS_PARAM_EXIST(CGI_L3_IPADDR, value))
            STR2IPV6(value, &ip.addr);

        ip.plen = 64;

        CGI_SET_ERR_HDL(sal_l3_intfStaticIPv6_add(&intfId, &ip), ret);
    }
    else
    {
        if (CGI_IS_PARAM_EXIST(CGI_L3_IPADDR, value))
            STR2IPV6(value, &ip.addr);

        if (CGI_IS_PARAM_EXIST(CGI_L3_IPV6_EUI64, value))
        {
            ip.plen = 64;
            sys_l3_ipv6SysEui64_convert(&ip.addr);
        }
        else
        {
            if (CGI_IS_PARAM_EXIST(CGI_L3_PLEN, value))
                ip.plen = SYS_STR2UINT(value);
        }

        CGI_SET_ERR_HDL(sal_l3_intfStaticIPv6_add(&intfId, &ip), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_addrV6Del_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i = 0;
    uint32 j = 0;
    uint32 len = 0;
    char *value = NULL;
    char **array = NULL;
    char *tok[4];
    sys_l3_intfId_t intfId;
    sys_l3_ipv6_t ip;

    if (CGI_IS_PARAM_EXIST(CGI_L3_INTF_NAME, value))
        INTFNAME2ID(value, &intfId);

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_L3_IPADDR, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            j = 0;

            tok[j] = strtok(array[i], "_\n");
            while (tok[j] != NULL)
            {
                j++;
                tok[j] = strtok(NULL, "_\n");
            }

            SYS_MEM_CLEAR(ip);
            STR2IPV6(tok[0], &ip.addr);
            ip.plen = SYS_STR2UINT(tok[1]);

            CGI_SET_ENTRY_ERR_CONTINUE(sal_l3_intfStaticIPv6_del(&intfId, &ip), array[i]);
        }
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV6Add_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_routeV6_t route;

    SYS_MEM_CLEAR(route);

    if (CGI_IS_PARAM_EXIST(CGI_L3_IPADDR, value))
        STR2IPV6(value, &route.dst);

    if (CGI_IS_PARAM_EXIST(CGI_L3_PLEN, value))
        route.dstLen = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_GW, value))
        STR2IPV6(value, &route.gw);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_METRIC, value))
        route.pri = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_l3_staticRouteV6_add(&route), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV6Edit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_routeV6_t route;

    SYS_MEM_CLEAR(route);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_DST, value))
        STR2IPV6(value, &route.dst);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_PLEN, value))
        route.dstLen = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_GW, value))
        STR2IPV6(value, &route.gw);

    if (CGI_IS_PARAM_EXIST(CGI_L3_ROUTE_METRIC, value))
        route.pri = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_l3_staticRouteV6_add(&route), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_routeV6Del_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    char *tok[4];
    uint32 i = 0;
    uint32 j = 0;
    uint32 len = 0;
    sys_l3_routeV6_t route;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_L3_ROUTES, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(route);
            j = 0;

            tok[j] = strtok(array[i], "_\n");
            while (tok[j] != NULL)
            {
                j++;
                tok[j] = strtok(NULL, "_\n");
            }

            STR2IPV6(tok[0], &route.dst);
            route.dstLen = SYS_STR2UINT(tok[1]);
            STR2IPV6(tok[2], &route.gw);

            CGI_SET_ENTRY_ERR_CONTINUE(sal_l3_staticRouteV6_del(&route), array[i]);
        }
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v6Neigh_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 clearType = 0;
    sys_l3_intfId_t intf;

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_CLEAR_TYPE, value))
    {
        clearType = SYS_STR2UINT(value);

        SYS_MEM_CLEAR(intf);
        if (0 == clearType || 1 == clearType)
            CGI_SET_ERR_HDL(sal_l3_neigh_clear(SYS_L3_NEIGH_V6_DYNAMIC, intf), ret);

        if (0 == clearType || 2 == clearType)
            CGI_SET_ERR_HDL(sal_l3_neigh_clear(SYS_L3_NEIGH_V6_STATIC, intf), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v6NeighAdd_set(void)
{
    int32 ret = SYS_ERR_FAILED;
    sys_l3_intfId_t intf;
    sys_l3_ip_t l3ip;
    sys_ip_t ip;
    sys_vid_t vid;
    sys_mac_t mac;
    sys_l3_ipv6Admin_t ipv6Admin;
    char *value = NULL;
    char ipv6Str[CAPA_IPV6_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    char buf[SYS_BUF256_LEN];

    SYS_MEM_CLEAR(intf);
    SYS_MEM_CLEAR(ipv6Str);
    SYS_MEM_CLEAR(macStr);

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        vid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_IPV6, value))
    {
        strlcpy(ipv6Str, value, sizeof(ipv6Str));
        if (FALSE == IS_IPV6_UADDRSTR_VALID(ipv6Str))
        {
            ret = SYS_ERR_IPV6_ADDR;
            goto cgi_end;
        }
        SYS_ERR_CHK(STR2IPV6(ipv6Str, &l3ip.v6.addr));
        intf.type = SYS_L3_INTFTYPE_VLAN;
        intf.id = vid;
        SYS_MEM_CLEAR(ipv6Admin);
        sal_l3_adminIntfIPv6_get(&intf, &ipv6Admin);

        if (0xfe != l3ip.v6.addr.hex[0] || 0x80 != l3ip.v6.addr.hex[1])
        {
            SYS_MEM_CLEAR(intf);
            SYS_ERR_CHK(sys_l3_ip2Intf(SYS_L3_AF_IPV6, l3ip, &intf));

            if (vid != intf.id)
            {
                ret = SYS_ERR_L3_STATIC_NEIGHV6_NOT_MATCH;
                goto cgi_end;
            }
        }

    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_MAC, value))
    {
        strlcpy(macStr, value, sizeof(macStr));
        SYS_MEM_CLEAR(mac);
        SYS_ERR_CHK(sys_util_str2Mac(value, mac.octet));
        CGI_SET_ERR_HDL(sal_l3_neigh_mac_validate(&mac), ret);
    }
    else
        goto cgi_end;

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(intfName);
    INTFID2STR(&intf, intfName);
    sprintf(buf, "ip -6 neigh add %s lladdr %s nud permanent dev %s 1>> /dev/null 2>> /dev/null", ipv6Str, macStr, intfName);

    if(0 != system(buf))
    {
        SYS_MEM_CLEAR(ip);
        osal_memcpy(&ip.v6, &l3ip.v6, sizeof(l3ip.v6));
        sal_l3_hrouteEntry_del(ip, mac);
        SYS_MEM_CLEAR(buf);
        sprintf(buf, "ip -6 neigh change %s lladdr %s nud permanent dev %s 1>> /dev/null 2>> /dev/null",
            ipv6Str, macStr, intfName);
        system(buf);
    }

    ret = SYS_ERR_OK;

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v6NeighEdit_set(void)
{
    int32 ret = SYS_ERR_FAILED;
    sys_l3_intfId_t intf;
    sys_enable_t enable;
    sys_mac_t mac;
    sys_ip_t ip;
    char *value = NULL;
    char ipStr[CAPA_IPV4_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    char intfName[CAPA_L3_INTFNAME_LEN];
    char buf[SYS_BUF256_LEN];

    SYS_MEM_CLEAR(intf);
    SYS_MEM_CLEAR(ipStr);
    SYS_MEM_CLEAR(macStr);
    SYS_MEM_CLEAR(ip);
    SYS_MEM_CLEAR(mac);

    intf.type = SYS_L3_INTFTYPE_VLAN;
    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        intf.id = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_IPV6, value))
        strlcpy(ipStr, value, sizeof(ipStr));
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_L3_NEIGH_MAC, value))
        strlcpy(macStr, value, sizeof(macStr));
    else
        goto cgi_end;

    SYS_ERR_CHK(sal_l3_adminIntfDhcpV4_get(&intf, &enable));

    if (ENABLED == enable)
    {
        ret = SYS_ERR_L3_STATIC_NEIGHV4_INTF_DHCP;
        goto cgi_end;
    }

    STR2IPV6(ipStr, &ip.v6);
    sys_util_str2Mac(macStr, mac.octet);
    CGI_SET_ERR_HDL(sal_l3_neigh_mac_validate(&mac), ret);
    sal_l3_hrouteEntry_del(ip, mac);

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(intfName);
    INTFID2STR(&intf, intfName);
    sprintf(buf, "ip -6 neigh change %s lladdr %s nud permanent dev %s 1>> /dev/null 2>> /dev/null", ipStr, macStr, intfName);
    system(buf);
    ret = SYS_ERR_OK;

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_l3_v6NeighDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    char *ip = NULL;
    char *vlan = NULL;
    char intfName[CAPA_L3_INTFNAME_LEN];
    char buf[SYS_BUF256_LEN];
    uint32 i = 0;
    uint32 len = 0;
    sys_l3_intfId_t intf;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_L3_NEIGH_ENTRY, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            vlan = strtok(array[i], "_");
            ip = strtok(NULL, "_");

            intf.type = SYS_L3_INTFTYPE_VLAN;
            intf.id = SYS_STR2UINT(vlan);

            SYS_MEM_CLEAR(buf);
            SYS_MEM_CLEAR(intfName);
            INTFID2STR(&intf, intfName);
            sprintf(buf, "ip -6 neigh del %s dev %s 1>> /dev/null 2>> /dev/null;", ip, intfName);
            system(buf);
        }
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

