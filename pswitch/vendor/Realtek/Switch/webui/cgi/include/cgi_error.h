
#ifndef __CGI_ERROR_H__
#define __CGI_ERROR_H__

typedef struct cgi_err_s
{
    int32   errCode;
    char    *errStr;
} cgi_err_t;

typedef enum cgi_err_code_e
{
    CGI_ERR_OK = 0xFFFFFF,                           
    CGI_ERR_ACT_SUCCESS,                             
    CGI_ERR_SESSION_TIMEOUT,                         
    CGI_ERR_LOCALUSER_EXIST,                         
    CGI_ERR_OLDPASSWD_INVALID,                       

    CGI_ERR_SNMP_AUTHPASSWD_LENGTH_MIN,
    CGI_ERR_SNMP_PRIVPASSWD_LENGTH_MIN,
    CGI_ERR_SNMP_COMMUNITY_NO_GROUP,
    CGI_ERR_SNMP_USER_NO_GROUP,
    CGI_ERR_SNMP_NOTIFY_NO_COMMUNITY,
    CGI_ERR_SNMP_NOTIFY_NO_USER,

    CGI_ERR_MCAST_NO_PORT_SEL,
    CGI_ERR_MCAST_NO_VLAN_SEL,

    CGI_ERR_RMON_HISTORY_TBL_FULL,                   
    CGI_ERR_RMON_ALARM_TBL_FULL,                     
    CGI_ERR_RMON_EVENT_TBL_FULL,                     

    CGI_ERR_END                                      

} cgi_err_code_t;

#define CGI_ERR_STR(errCode)        cgi_errStr_ret(errCode)

#define IS_CGI_ERR_INVALID(err)    \
    (((((err) >= SYS_ERR_END) || ((err) < SYS_ERR_FAILED)) && ((err) >= CGI_ERR_END))  ? 1: 0)

extern char *cgi_errStr_ret(int32 errCode);

#endif  

