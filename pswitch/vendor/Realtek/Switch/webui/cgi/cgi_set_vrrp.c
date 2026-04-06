
#include <include/cgi.h>
#include <libsal/sal_l2.h>
#include <libipc/quaggaTask.h>
#include <libipc/vos.h>
#include <libsal/sal_l3_vrrp.h>

static int32 cgi_vrrp_V4Add_set(void);
static int32 cgi_vrrp_V4Edit_set(void);
static int32 cgi_vrrp_V4Del_set(void);

const cgi_cmdmap_t cmdSetVrrpTbl[] =
{
    {CGI_CMD_VRRP_V4INTFADD,     cgi_vrrp_V4Add_set},
    {CGI_CMD_VRRP_V4INTFEDIT,    cgi_vrrp_V4Edit_set},
    {CGI_CMD_VRRP_V4INTFDEL,     cgi_vrrp_V4Del_set},
    {NULL, NULL}
};

static int32 cgi_vrrp_V4Add_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
	sys_ipv4_t vipv4;
    int iErrNo = 0;
    int vid;
    int vrid;
    int priority;
    int adver;
    int preempt;
    int delay = 0;
	
    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        vid = SYS_STR2UINT(value);
    else
        goto cgi_end;
    if (CGI_IS_PARAM_EXIST(CGI_VRRP_VRID, value))
        vrid = SYS_STR2UINT(value);
    else
        goto cgi_end;
	
    if (CGI_IS_PARAM_EXIST(CGI_VRRP_VADDR, value))
        STR2IPV4(value, &vipv4);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VRRP_PRI, value))
        priority = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VRRP_ADVER, value))
        adver = SYS_STR2UINT(value);
    else
        goto cgi_end;
	
    if (CGI_IS_PARAM_EXIST(CGI_VRRP_PREEMPT, value))
        preempt = ENABLED;
    else
        preempt = DISABLED;
	
    if (CGI_IS_PARAM_EXIST(CGI_VRRP_DELAY, value))
        delay = SYS_STR2UINT(value);
		
	CGI_SET_ERR_HDL(sal_vrrp_vrid_vip_config(vid, vrid, vipv4, VRRP_OPCODE_ADDIP), ret);
	CGI_SET_ERR_HDL(sal_vrrp_vrid_priority_config(vid, vrid, priority, &iErrNo), ret);
	
	CGI_SET_ERR_HDL(sal_vrrp_vrid_preempt_mode_config(vid, vrid, preempt, delay, &iErrNo), ret);

	CGI_SET_ERR_HDL(sal_vrrp_vrid_advertise_timer_config(vid, vrid, adver, &iErrNo), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);
    return SYS_ERR_OK;
}

static int32 cgi_vrrp_V4Edit_set(void)
{
#if 0
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
        }

        CGI_SET_ERR_HDL(sal_l3_intfIPv4_add(&intfId, SYS_L3_IPTYPE_STATIC, &ip), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);
#endif
    return SYS_ERR_OK;
}

static int32 cgi_vrrp_V4Del_set(void)
{
    int32 ret = SYS_ERR_OK;
    
    uint32 i = 0;
    uint32 len = 0;
    char **array = NULL;
    char *token = NULL;
	sys_ipv4_t vipv4 = 0;
    int vid = 0;
    int vrid = 0;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_L3_INTF_NAME, array, &len))
    {
        for (i = 0; i < len; i++)
        {
    		
            token = strtok(array[i], "_");
            if( token != NULL ) 
            {
	            vid = SYS_STR2UINT(token);
			}
			
			token = strtok(NULL, "_");
			if( token != NULL ) 
            {
	            vrid = SYS_STR2UINT(token);
			}
			
			token = strtok(NULL, "_");
			if( token != NULL ) 
            {
			    STR2IPV4(token, &vipv4);
			}
        }
    }
	
	CGI_SET_ERR_HDL(sal_vrrp_vrid_vip_config(vid, vrid, vipv4, VRRP_OPCODE_DELIP), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);
	
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

