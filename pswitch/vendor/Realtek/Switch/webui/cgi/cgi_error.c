
#include <common/sys_def.h>
#include <include/cgi.h>

static cgi_err_t cgiError[] =
{
    
    { SYS_ERR_INPUT,                        CGI_LANG("error", "errInvalidValue")},
    { SYS_ERR_PASSWORD_INVALID,             CGI_LANG("login", "errLoginUserInvalid")},
    { SYS_ERR_LOCALUSER_FULL,               CGI_LANG_ARG("sys", "errMaxNumUserArg", SYS_STR_CONST(CAPA_SYS_LOCALUSER_NUM))},
    { CGI_ERR_LOCALUSER_EXIST,              CGI_LANG("sys", "errUserExist")},
    { CGI_ERR_OLDPASSWD_INVALID,            CGI_LANG("sys", "errInavlidOldPassword")},
    { SYS_ERR_LOCALUSER_NOT_EXIST,          CGI_LANG("sys", "errUserNotExist")},
    { SYS_ERR_LOCALUSER_DEFAULT_DEL,        CGI_LANG("sys", "errDeleteDeafultUser")},

    {CGI_ERR_SNMP_AUTHPASSWD_LENGTH_MIN,  CGI_LANG("snmp", "errUserAuthPwLenMin")},
    {CGI_ERR_SNMP_PRIVPASSWD_LENGTH_MIN,  CGI_LANG("snmp", "errUserPrivPwLenMin")},
    {CGI_ERR_SNMP_COMMUNITY_NO_GROUP,      CGI_LANG("snmp", "errComNoGrp")},
    {CGI_ERR_SNMP_USER_NO_GROUP,                CGI_LANG("snmp", "errUserNoGrp")},
    {CGI_ERR_SNMP_NOTIFY_NO_COMMUNITY,     CGI_LANG("snmp", "errNotifyNoCom")},
    {CGI_ERR_SNMP_NOTIFY_NO_USER,               CGI_LANG("snmp", "errNotifyNoUser")},

    {CGI_ERR_MCAST_NO_PORT_SEL,                CGI_LANG("mcast", "errMcastPortSel")},
    {CGI_ERR_MCAST_NO_VLAN_SEL,                CGI_LANG("mcast", "errMcastVlanSel")},

    { CGI_ERR_RMON_HISTORY_TBL_FULL,        CGI_LANG("rmon", "errHistoryTableOverflow")},
    { CGI_ERR_RMON_ALARM_TBL_FULL,          CGI_LANG("rmon", "errAlarmTableOverflow")},
    { CGI_ERR_RMON_EVENT_TBL_FULL,          CGI_LANG("rmon", "errEventTableOverflow")},

    { SYS_ERR_END, NULL}
};

char *cgi_errStr_ret(int32 errCode)
{
    cgi_err_t *ptr = NULL;

    if (!IS_CGI_ERR_INVALID(errCode))
    {

        for (ptr = cgiError; ptr->errStr; ptr++)
        {
            if (ptr->errCode == errCode)
                return (ptr->errStr);
        }
    }

    return "";
}

