
#include <include/cgi.h>
#include <libsal/sal_mib.h>
#include <libsal/sal_l3.h>
#ifdef CONFIG_SYS_MGMT_ACL
#include <libsal/sal_acl_mgmt.h>
#endif
#include <libclicmd/cmd_time.h>

static int32 cgi_sys_sysinfo_set(void);
static int32 cgi_sys_ip_set(void);
static int32 cgi_sys_acctAdd_set(void);
static int32 cgi_sys_acctEdit_set(void);
static int32 cgi_sys_acctDel_set(void);
static int32 cgi_sys_reboot_set(void);
static int32 cgi_sys_restore_set(void);
static int32 cgi_sys_line_set(void);
#ifdef CONFIG_SYS_ADVANCE_DNS
static int32 cgi_sys_dns_set(void);
static int32 cgi_sys_dnsAdd_set(void);
static int32 cgi_sys_dnsDel_set(void);
static int32 cgi_sys_dnsHosts_set(void);
static int32 cgi_sys_dnsHostsAdd_set(void);
static int32 cgi_sys_dnsHostsDel_set(void);
#endif

const cgi_cmdmap_t cmdSetSysTbl[] =
{
    {CGI_CMD_SYS_SYSINFO,           cgi_sys_sysinfo_set},
    {CGI_CMD_SYS_IP,                cgi_sys_ip_set},
    {CGI_CMD_SYS_ACCTADD,           cgi_sys_acctAdd_set},
    {CGI_CMD_SYS_ACCTEDIT,          cgi_sys_acctEdit_set},
    {CGI_CMD_SYS_ACCTDEL,           cgi_sys_acctDel_set},
    {CGI_CMD_SYS_REBOOT,            cgi_sys_reboot_set},
    {CGI_CMD_SYS_RESTORE,           cgi_sys_restore_set},
    {CGI_CMD_SYS_LINE,              cgi_sys_line_set},
#ifdef CONFIG_SYS_ADVANCE_DNS
    {CGI_CMD_SYS_DNS,               cgi_sys_dns_set},
    {CGI_CMD_SYS_DNS_ADD,           cgi_sys_dnsAdd_set},
    {CGI_CMD_SYS_DNS_DEL,           cgi_sys_dnsDel_set},
    {CGI_CMD_SYS_DNS_HOSTS,         cgi_sys_dnsHosts_set},
    {CGI_CMD_SYS_DNS_HOSTS_ADD,     cgi_sys_dnsHostsAdd_set},
    {CGI_CMD_SYS_DNS_HOSTS_DEL,     cgi_sys_dnsHostsDel_set},
#endif
    {NULL, NULL}
};

static int32 cgi_sys_sysinfo_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    if (CGI_IS_PARAM_EXIST(CGI_SYS_HOSTNAME, value))
    {
        CGI_SET_ERR_HDL(sal_sys_systemName_set(value), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_SYS_LOCATION, value))
    {
        CGI_SET_ERR_HDL(sal_sys_systemLocation_set(value), ret);
    }
    else
    {
        CGI_SET_ERR_HDL(sal_sys_systemLocation_set(""), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_SYS_CONTACT, value))
    {
        CGI_SET_ERR_HDL(sal_sys_systemContact_set(value), ret);
    }
    else
    {
        CGI_SET_ERR_HDL(sal_sys_systemContact_set(""), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 _cgi_sys_ipv4_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);
    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV4_TYPE, value))
    {
        if (0 == osal_strcmp(value, "dynamic"))
        {
            intfMgmt.v4.dhcpType = SYS_L3_DHCPTYPE_DHCP;
        }
        else
        {
            intfMgmt.v4.dhcpType = SYS_L3_DHCPTYPE_DISABLE;

            if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV4_STATICIP, value))
                STR2IPV4(value, &intfMgmt.v4.ip.addr);

            if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV4_STATICMASK, value))
            {
                sys_ipv4_t mask;

                STR2IPV4(value, &mask);

                intfMgmt.v4.ip.plen = IPV4_MASK2PFX(mask);
            }

            if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV4_ADMIN_GW, value))
                STR2IPV4(value, &intfMgmt.v4.gw);
        }
    }
    
	if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV4_SUB_STATUS, value))
	{
        if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV4_SUB_STATICIP, value))
        {
            STR2IPV4(value, &intfMgmt.subv4.ip.addr);
            CGI_DBG("%s", value);
        }

        if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV4_SUB_STATICMASK, value))
        {
            sys_ipv4_t mask;

            STR2IPV4(value, &mask);

            intfMgmt.subv4.ip.plen = IPV4_MASK2PFX(mask);
            CGI_DBG("%s", value);
        }			
	}
	else 
	{
		intfMgmt.subv4.ip.addr = 0;
		intfMgmt.subv4.ip.plen = 0;
	}
	intfMgmt.subv4.dhcpType = SYS_L3_DHCPTYPE_DISABLE;
	
    CGI_SET_ERR_HDL(sal_l3_adminIntfMgmt_set(&intfMgmt), ret);

cgi_end:
    return ret;
}

#ifdef CONFIG_SYS_IPV6
static int32 _cgi_sys_ipv6_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *pStrValue = NULL;
    sys_l3_intfMgmtAdmin_t intfMgmt;

    SYS_MEM_CLEAR(intfMgmt);

    SYS_ERR_CHK(sal_l3_adminIntfMgmt_get(&intfMgmt));

    if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV6_AUTO_CONFIG, pStrValue))
        intfMgmt.v6.autoconfEnbl = ENABLED;
    else
        intfMgmt.v6.autoconfEnbl = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV6_DHCPV6, pStrValue))
        intfMgmt.v6.dhcpEnbl = ENABLED;
    else
        intfMgmt.v6.dhcpEnbl = DISABLED;

    if (DISABLED == intfMgmt.v6.dhcpEnbl && DISABLED == intfMgmt.v6.autoconfEnbl)
    {
        if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV6_STATIC_ADDR, pStrValue))
        {
            STR2IPV6(pStrValue, &intfMgmt.v6.ip[0].addr);
        }

        if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV6_PREFIX, pStrValue))
        {
            intfMgmt.v6.ip[0].plen = SYS_STR2UINT(pStrValue);

            if (intfMgmt.v6.ip[0].plen > 128)
            {
                ret = SYS_ERR_IPV6_ADDR;
                goto cgi_end;
            }
        }

        if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV6_STATIC_GATEWAY, pStrValue))
        {
            STR2IPV6(pStrValue, &intfMgmt.v6.gw);
        }
    }

    CGI_SET_ERR_HDL(sal_l3_adminIntfMgmt_set(&intfMgmt), ret);

cgi_end:
    return ret;
}
#endif

static int32 cgi_sys_ip_set(void)
{
    int32 ret = SYS_ERR_OK;

    CGI_SET_ERR_HDL(_cgi_sys_ipv4_set(), ret);
#ifdef CONFIG_SYS_IPV6
    CGI_SET_ERR_HDL(_cgi_sys_ipv6_set(), ret);
#endif

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_acctAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 i = 0;
    sys_userInfo_t userInfo;
    sys_userInfo_t userInfoExist;

    SYS_MEM_CLEAR(userInfo);

    userInfo.passwdType = SYS_PASSWD_TYPE_ENCRYPT;

    if (CGI_IS_PARAM_EXIST(CGI_SYS_ACCT_NAME, value))
    {
    
		if(osal_strlen(value) > CAPA_SYS_USERNAME_LEN)
		{
			ret = SYS_ERR_USERNAME_LENGTH_INVALID;
			goto cgi_end;
		}
		if (SYS_ERR_OK != STRING_VALIDATE(value, TRUE))
		{
			ret = SYS_ERR_USERNAME_INVALID;
			goto cgi_end;
		}
		
        strlcpy(userInfo.username, value, sizeof(userInfo.username));

        for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
        {
            SYS_MEM_CLEAR(userInfoExist);
            sal_sys_localUser_get(i, &userInfoExist);

            if (userInfoExist.priv < 0)
                continue;

            if (0 == osal_strcmp(userInfo.username, userInfoExist.username))
            {
                ret = CGI_ERR_LOCALUSER_EXIST;
                goto cgi_end;
            }
        }
    }
	else
	{
		ret = SYS_ERR_USERNAME_INVALID;
		goto cgi_end;
	}

    if (CGI_IS_PARAM_EXIST(CGI_HOME_LOGIN_PASSWORD, value))
    {
        if (getenv("ACCESS_TYPE") == NULL)
        {
            if(sal_util_rsa_decrypt(SYS_SEC_HTTP_RSA_FILE, value, userInfo.password) != SYS_ERR_OK)
                strlcpy(userInfo.password, value, sizeof(userInfo.password));
        }
        else
            strlcpy(userInfo.password, value, sizeof(userInfo.password));
    }
	else
	{
		ret = SYS_ERR_PASSWORD_INVALID;
		goto cgi_end;
	}
	
	if((osal_strlen(userInfo.password) > CAPA_SYS_PASSWORD_LEN)||(osal_strlen(userInfo.password) < CAPA_SYS_PASSWORD_MIX_LEN))
	{
		ret = SYS_ERR_PASSWORD_LENGTH_INVALID;
		goto cgi_end;
	}
	if (SYS_ERR_OK != STRING_VALIDATE(userInfo.password, TRUE))
	{
		ret = SYS_ERR_PASSWORD_INVALID;
		goto cgi_end;
	}

    if (CGI_IS_PARAM_EXIST(CGI_SYS_ACCT_PRIV, value))
        userInfo.priv = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_sys_localUser_add(userInfo), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_acctEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_userInfo_t userInfo;

    SYS_MEM_CLEAR(userInfo);

    if (CGI_IS_PARAM_EXIST(CGI_SYS_ACCT_NAME, value))
    {
        strlcpy(userInfo.username, value, sizeof(userInfo.username));

        sal_sys_localUser_getByName(value, &userInfo);
    }

    if (CGI_IS_PARAM_EXIST(CGI_HOME_LOGIN_PASSWORD, value))
    {
        if (getenv("ACCESS_TYPE") == NULL)
        {
            if(sal_util_rsa_decrypt(SYS_SEC_HTTP_RSA_FILE, value, userInfo.password) != SYS_ERR_OK)
                strlcpy(userInfo.password, value, sizeof(userInfo.password));
        }
        else
            strlcpy(userInfo.password, value, sizeof(userInfo.password));
    }
    else
    {
		ret = SYS_ERR_PASSWORD_INVALID;
		goto cgi_end;
    }
	
	if((osal_strlen(userInfo.password) > CAPA_SYS_PASSWORD_LEN)||(osal_strlen(userInfo.password) < CAPA_SYS_PASSWORD_MIX_LEN))
	{
		ret = SYS_ERR_PASSWORD_LENGTH_INVALID;
		goto cgi_end;
	}
	if (SYS_ERR_OK != STRING_VALIDATE(userInfo.password, TRUE))
	{
		ret = SYS_ERR_PASSWORD_INVALID;
		goto cgi_end;
	}
	
    if (CGI_IS_PARAM_EXIST(CGI_SYS_ACCT_PRIV, value))
        userInfo.priv = SYS_STR2UINT(value);

    CGI_SET_ERR_HDL(sal_sys_localUser_add(userInfo), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_acctDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i = 0;
    uint32 len = 0;
    char **array = NULL;

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_SYS_ACCT, array, &len))
    {
        for (i = 0; i < len; i++)
        {
			if (0 == osal_strcmp(array[i], g_username))
			{
				CGI_SET_ENTRY_ERR_CONTINUE(SYS_ERR_LOCALUSER_CURRENT_DEL, array[i]);
				continue;
			}
            CGI_SET_ENTRY_ERR_CONTINUE(sal_sys_localUser_del(array[i]), array[i]);
        }
        free(array);
    }

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_reboot_set(void)
{
    int32 ret = SYS_ERR_OK;

    system("handler -f reboot 1> /dev/null 2> /dev/null &");

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_restore_set(void)
{
    int32 ret = SYS_ERR_OK;

    ret = sal_sys_restore_defaults(DISABLED);

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_line_set(void)
{
    int32                ret = SYS_ERR_OK;
    char                 *value = NULL;
    char                 buf[SYS_BUF16_LEN];
    uint32               i = 0;
    uint32               val = 0;
    sys_enable_t         enable;
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    sys_cliRetrySilent_t retrySilent;
#endif

    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        if (SYS_ACCESS_CLI_CONSOLE == i)
            continue;

        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s_%d", CGI_SYS_LINE_STATE, i);

        if (CGI_IS_PARAM_EXIST(buf, value))
            enable = ENABLED;
        else
            enable = DISABLED;

        switch (i)
        {
#ifdef CONFIG_SYS_APP_TELNETD
            case SYS_ACCESS_CLI_TELNET:
                CGI_SET_ERR_HDL(sal_sys_telnetdEnable_set(enable), ret);
                break;
#endif

#ifdef CONFIG_USER_SSH_SSHD
            case SYS_ACCESS_CLI_SSH:
                CGI_SET_ERR_HDL(sal_ssh_sshdEnable_set(enable), ret);
                break;
#endif

            case SYS_ACCESS_WEB_HTTP:
                CGI_SET_ERR_HDL(sal_sys_httpEnable_set(enable), ret);
                break;
#ifdef CONFIG_USER_BOA_WITH_SSL
            case SYS_ACCESS_WEB_HTTPS:
                CGI_SET_ERR_HDL(sal_sys_httpsEnable_set(enable), ret);
                break;
#endif
            default:
                break;
        }
    }

    sal_sys_inetdConfig_set();
    sal_inetd_restart();

#ifdef CONFIG_SYS_UI_SNMP
    {
        sys_enable_t old_snmpEnbl;

        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s_%d", CGI_SYS_LINE_STATE, CGI_SYS_LINE_METHOD_SNMP_VAL);

        if (CGI_IS_PARAM_EXIST(buf, value))
            enable = ENABLED;
        else
            enable = DISABLED;

        CGI_SET_ERR_HDL(sal_snmp_enable_get(&old_snmpEnbl), ret);
        CGI_SET_ERR_HDL(sal_snmp_enable_set(enable), ret);

        if (enable != old_snmpEnbl)
        {
            if (ENABLED == enable)
                CGI_SET_ERR_HDL(sal_snmp_restart(), ret);
            else
                CGI_SET_ERR_HDL(sal_snmp_stop(), ret);
        }
    }
#endif

    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s_%d", CGI_SYS_LINE_TIMEOUT, i);

        if (CGI_IS_PARAM_EXIST(buf, value))
        {
            val = SYS_STR2UINT(value);

            switch (i)
            {
#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
                case SYS_ACCESS_CLI_CONSOLE:
                    CGI_SET_ERR_HDL(sal_sys_cliSessionTimeout_set(SYS_LINE_CLI_CONSOLE, val), ret);
                    break;

                case SYS_ACCESS_CLI_TELNET:
                    CGI_SET_ERR_HDL(sal_sys_cliSessionTimeout_set(SYS_LINE_CLI_TELNET, val), ret);
                    break;

#ifdef CONFIG_USER_SSH_SSHD
                case SYS_ACCESS_CLI_SSH:
                    CGI_SET_ERR_HDL(sal_sys_cliSessionTimeout_set(SYS_LINE_CLI_SSH, val), ret);
                    break;
#endif
#endif
                case SYS_ACCESS_WEB_HTTP:
                    CGI_SET_ERR_HDL(sal_sys_webSessionTimeout_set(SYS_LINE_WEB_HTTP, val), ret);
                    break;

                case SYS_ACCESS_WEB_HTTPS:
                    CGI_SET_ERR_HDL(sal_sys_webSessionTimeout_set(SYS_LINE_WEB_HTTPS, val), ret);
                    break;

                default:
                    break;
            }
        }
    }

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        if (SYS_ACCESS_WEB_HTTP == i || SYS_ACCESS_WEB_HTTPS == i)
            continue;

        SYS_MEM_CLEAR(retrySilent);

        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s_%d", CGI_SYS_LINE_RETRY, i);

        if (CGI_IS_PARAM_EXIST(buf, value))
            retrySilent.retryNum = SYS_STR2UINT(value);

        SYS_MEM_CLEAR(buf);
        osal_sprintf(buf, "%s_%d", CGI_SYS_LINE_SILENT, i);

        if (CGI_IS_PARAM_EXIST(buf, value))
            retrySilent.silentTime = SYS_STR2UINT(value);

        switch (i)
        {
            case SYS_ACCESS_CLI_CONSOLE:
                CGI_SET_ERR_HDL(sal_sys_cliRetrySilent_set(SYS_LINE_CLI_CONSOLE, retrySilent), ret);
                break;

            case SYS_ACCESS_CLI_TELNET:
                CGI_SET_ERR_HDL(sal_sys_cliRetrySilent_set(SYS_LINE_CLI_TELNET, retrySilent), ret);
                break;

#ifdef CONFIG_USER_SSH_SSHD
            case SYS_ACCESS_CLI_SSH:
                CGI_SET_ERR_HDL(sal_sys_cliRetrySilent_set(SYS_LINE_CLI_SSH, retrySilent), ret);
                break;
#endif

            default:
                break;
        }
    }
#endif

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_ADVANCE_DNS
static int32 cgi_sys_dns_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    char domain[SYS_BUF256_LEN];
    sys_enable_t enable = DISABLED;

    SYS_MEM_CLEAR(domain);

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        enable = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_SYS_HOSTNAME, value))
        strlcpy(domain, value, sizeof(domain));

    CGI_SET_ERR_HDL(sal_sys_dnsLookup_set(enable), ret);
    CGI_SET_ERR_HDL(sal_sys_dnsDomain_set(domain, SYS_ADVDNS_SRC_STATIC),ret);
    CGI_SET_ERR_HDL(sal_sys_dnsResolvConf_create(), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_dnsAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i = 0;
    char *value = NULL;
    sys_advDnsIP_t ns;
    sys_advDnsIP_t newns;
    uint32  curr_cnt = 0;

    SYS_MEM_CLEAR(newns);

    if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV4_IP, value))
        strlcpy(newns.ip, value, sizeof(newns.ip));
    else
        goto cgi_end;

    if (TRUE == sal_util_ipv4StrValidate_ret(newns.ip, SYS_IP_TYPE_UNICAST))
        newns.ipVer = 4;
    else
        newns.ipVer = 6;

    for(i = 0; i < CAPA_SYS_DNS_NAMESERVER_NUM; i++)
    {
        SYS_MEM_CLEAR(ns);
        sal_sys_dnsNameServer_get(i, &ns, SYS_ADVDNS_SRC_STATIC);

        if (0 != osal_strlen(ns.ip))
        {
        	curr_cnt ++;
            continue;
        }

        CGI_SET_ERR_HDL(sal_sys_dnsNameServer_set(i, &newns, SYS_ADVDNS_SRC_STATIC), ret);
        break;
    }
	if(i == CAPA_SYS_DNS_NAMESERVER_NUM && curr_cnt == CAPA_SYS_DNS_NAMESERVER_NUM)
	{
		CGI_SET_ERR_HDL(SYS_ERR_DNS_ENTRY_FULL, ret);
	}
    CGI_SET_ERR_HDL(sal_sys_dnsResolvConf_create(), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_dnsDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i = 0;
    uint32 idx = 0;
    char **sel = NULL;
    sys_advDnsIP_t ns;

    SYS_MEM_CLEAR(ns);

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_SYS_DNSS, sel, &len))
    {
        for(i = 0; i < len; i++)
        {
            idx = SYS_STR2UINT(sel[i]);

            if (idx < CAPA_SYS_DNS_NAMESERVER_NUM)
            {
                CGI_SET_ERR_CONTINUE(sal_sys_dnsNameServer_set(idx, &ns, SYS_ADVDNS_SRC_STATIC), ret);
            }
            else if ((idx >= (1 << 8)) && (idx < (1 << 16)))
            {
                idx -= (1 << 8);
                CGI_SET_ERR_CONTINUE(sal_sys_dnsNameServer_set(idx, &ns, SYS_ADVDNS_SRC_DHCPV4), ret);
            }
            else
            {
                idx -= (1 << 16);
                CGI_SET_ERR_CONTINUE(sal_sys_dnsNameServer_set(idx, &ns, SYS_ADVDNS_SRC_DHCPV6), ret);
            }
        }
    }

    len = CAPA_SYS_DNS_NAMESERVER_NUM;
    for(i = 0; i < CAPA_SYS_DNS_NAMESERVER_NUM; i++)
    {
        SYS_MEM_CLEAR(ns);
        sal_sys_dnsNameServer_get(i, &ns, SYS_ADVDNS_SRC_STATIC);

        if (0 == osal_strlen(ns.ip) && len == CAPA_SYS_DNS_NAMESERVER_NUM)
        {
            len = i;
            continue;
        }
        else if (0 != osal_strlen(ns.ip) && len != CAPA_SYS_DNS_NAMESERVER_NUM)
        {
            CGI_SET_ERR_HDL(sal_sys_dnsNameServer_set(len, &ns, SYS_ADVDNS_SRC_STATIC),ret);
            SYS_MEM_CLEAR(ns);
            CGI_SET_ERR_HDL(sal_sys_dnsNameServer_set(i, &ns, SYS_ADVDNS_SRC_STATIC),ret);
            i = 0;
            len = CAPA_SYS_DNS_NAMESERVER_NUM;
        }
    }

    CGI_SET_ERR_HDL(sal_sys_dnsResolvConf_create(), ret);
cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != sel)
        osal_free(sel);

    return SYS_ERR_OK;
}

static int32 cgi_sys_dnsHosts_set(void)
{
    int32 ret = SYS_ERR_OK;

    CGI_SET_ERR_HDL(sal_dynamic_host_clear(), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_dnsHostsAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_advDns_staticHost_t host;

    SYS_MEM_CLEAR(host);

    if (CGI_IS_PARAM_EXIST(CGI_SYS_IPV4_IP, value))
        strlcpy(host.ip.ip, value, sizeof(host.ip.ip));
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_SYS_HOSTNAME, value))
        strlcpy(host.host, value, sizeof(host.host));
    else
        goto cgi_end;

    if (TRUE == sal_util_ipv4StrValidate_ret(host.ip.ip, SYS_IP_TYPE_UNICAST))
        host.ip.ipVer = 4;
    else
        host.ip.ipVer = 6;

    CGI_SET_ERR_HDL(sal_sys_dnsStaticHost_set(&host),ret);
    CGI_SET_ERR_HDL(sal_sys_dnsHosts_create(), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_sys_dnsHostsDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i = 0;
    char *tmp = NULL;
    char **sel = NULL;
    sys_advDns_staticHost_t host;

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_SYS_HOSTS, sel, &len))
    {
        for(i = 0; i < len; i++)
        {
            tmp = index(sel[i], '-');
            *tmp = 0;
            tmp++;

            SYS_MEM_CLEAR(host);
            strlcpy(host.ip.ip, tmp, sizeof(host.ip.ip));
            strlcpy(host.host, sel[i], sizeof(host.host));

            CGI_SET_ERR_CONTINUE(sal_sys_dnsStaticHost_del(&host), ret);
        }
    }

    CGI_SET_ERR_HDL(sal_sys_dnsHosts_create(), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != sel)
        osal_free(sel);

    return SYS_ERR_OK;
}
#endif

