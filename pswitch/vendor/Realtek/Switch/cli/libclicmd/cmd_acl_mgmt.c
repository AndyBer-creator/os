
#include <libsal/sal_acl_mgmt.h>
#include <libcmd/cmd.h>
#include <libvty/vty.h>

 static char g_Name [SYS_MGMT_ACL_NAME_LENTH_MAX+1] = "";
 static char acConsoleset[SYS_MGMT_ACL_NAME_LENTH_MAX+1 ] = "default";

#define ACL_MGMT_CONNAME_COMPARE() \
do {\
    if (0 == osal_strcmp(g_Name, acConsoleset))\
    {\
        XMORE("%s\n","Cannot modify console-only list");\
        return SYS_ERR_OK;\
    }\
} while(0)

static int32 _cmd_service_get(int  argc,char *argv[], int  token,uint32 *puiService)
{

    int32 iErrcode = SYS_ERR_OK;
    SYS_PARAM_CHK((NULL == puiService), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == argv[ token]), SYS_ERR_NULL_POINTER);
    if  ('h' == argv[ token][0])
    {
        if ( 's' == argv[ token][4] )
       {
            *puiService = SERVICE_HTTPS;
        }
        else
        {
            *puiService = SERVICE_HTTP;
         }
    }
    else if  ('s' == argv[ token][0])
    {
        if ('n' == argv[ token][1])
        {
            *puiService = SERVICE_SNMP;
        }
        else
        {
           *puiService = SERVICE_SSH;
        }
    }
    else if ('t' == argv[ token][0])
    {
       *puiService = SERVICE_TELNET;
    }
    else if ('a' == argv[ token][0])
    {
        *puiService = SERVICE_ALL;
    }
    else
    {
        iErrcode =SYS_ERR_FAILED;
    }

    return iErrcode;
}

static int32 _cmd_acl_mgmt_Arg_check(int argc, char * argv [ ])
{
   int i;
   if (0 == argc)
   {
       return SYS_ERR_FAILED;
   }
   for (i = 0; i < argc; i++)
   {
       if (NULL == argv[i])
       {
           return SYS_ERR_NULL_POINTER;
       }
   }
   return SYS_ERR_OK;
}

static int32 _cmd_acl_mgmt_ipSource_ip_get (uint16 offset, char *argv[], uint32  *puiSIpv4Addr,uint32  *puiSIpv4Netmask)
{
    uint16 uiIPv4Index = 0;
    uiIPv4Index  =uiIPv4Index  + offset;
    char *tmpStr = NULL;
    char buf[SYS_BUF64_LEN];

    tmpStr = rindex(argv[uiIPv4Index], '/');
    osal_memset(buf, 0, sizeof(buf));
    osal_strncpy(buf, argv[uiIPv4Index], tmpStr -argv[uiIPv4Index]);
    SYS_ERR_CHK(sys_util_str2Ip(buf, puiSIpv4Addr));
    osal_memset(buf, 0, sizeof(buf));
    osal_strcpy(buf, tmpStr+1);
    SYS_ERR_CHK(sys_util_str2Ip(buf, puiSIpv4Netmask));

    return SYS_ERR_OK;
}

static int32 _cmd_acl_mgmt_ipSource_ipv6_get (uint16 offset, char *argv[],  uint8 uaSIpv6Addr[ ],  uint32  *puiSIpv6PfLen )
{
    uint16 uiIPv6Index = 0;
    uiIPv6Index  =uiIPv6Index  + offset;
    char *tmpStr = NULL;
    char buf[SYS_BUF64_LEN];

    tmpStr = rindex(argv[uiIPv6Index], '/');
    osal_memset(buf, 0, sizeof(buf));
    osal_strncpy(buf, argv[uiIPv6Index], tmpStr -argv[uiIPv6Index]);
    inet_pton(AF_INET6, buf, uaSIpv6Addr);
    osal_memset(buf, 0, sizeof(buf));
    osal_strcpy(buf, tmpStr+1);
    *puiSIpv6PfLen = SYS_STR2UINT(buf);

    return SYS_ERR_OK;
}

static int32 _cmd_acl_mgmt_noIpSourceNopri_argTwo_set(int argc, char *argv[], uint16 offset, sys_acl_mgmt_careflag_t*pstCareFlag, uint32 *puiService,sys_logic_portmask_t *pstLogicPortmask)
{
    uint16 uiServiceIndex = 1;
    uint16 uiPortIndex = 0;
    uiServiceIndex = uiServiceIndex + offset;
    uiPortIndex = uiPortIndex + offset;

    SYS_PARAM_CHK((NULL == puiService), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL ==  pstLogicPortmask), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL==  pstCareFlag), SYS_ERR_NULL_POINTER);

    if (SYS_ERR_OK == cmd_util_portlist_get(argc, argv, uiPortIndex,pstLogicPortmask))
    {
        if (SYS_ERR_OK == _cmd_service_get(argc, argv,uiServiceIndex, puiService ))
        {
            return SYS_ERR_OK;
        }
    }
    return SYS_ERR_FAILED;
}

static int32 _cmd_acl_mgmt_ace_noIpSource_get(struct cmd_element *cmd,int argc, char *argv[], sys_acl_mgmt_care_value_t  *pstCareValue, sys_acl_mgmt_ace_priFlag_t *pstAcepriFlag, uint32 *puiPri)
 {
    sys_acl_mgmt_care_service_t  uiService;
    sys_logic_portmask_t stLogicPortmask;
    sys_acl_mgmt_careflag_t  stCareFlag;

    SYS_PARAM_CHK((NULL == pstCareValue), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == puiPri), SYS_ERR_NULL_POINTER);

    uiService          = SERVICE_ALL;
    LOGIC_PORTMASK_CLEAR_ALL(stLogicPortmask);
    osal_memset(&stCareFlag, 0, sizeof(sys_acl_mgmt_careflag_t));
    osal_memset(pstCareValue, 0, sizeof(sys_acl_mgmt_care_value_t));
    *puiPri                = 0;
    *pstAcepriFlag = PRIFLAG_INVALID;

    if ( 2 == argc )
    {
        SYS_ERR_CHK(_cmd_acl_mgmt_Arg_check(2,  argv));
        SYS_ERR_CHK(_cmd_acl_mgmt_noIpSourceNopri_argTwo_set(argc, argv, 0, &stCareFlag, &uiService, &stLogicPortmask));
    }
    else if(3 == argc )
    {
        SYS_ERR_CHK(_cmd_acl_mgmt_Arg_check(3,  argv));
        *puiPri  = SYS_STR2UINT(argv[0]);
        *pstAcepriFlag = PRIFLAG_VALID;
        SYS_ERR_CHK(_cmd_acl_mgmt_noIpSourceNopri_argTwo_set(argc, argv, 1, &stCareFlag, &uiService, &stLogicPortmask));
    }

    osal_memcpy(&(pstCareValue->stCareFlag), &stCareFlag, sizeof(sys_acl_mgmt_careflag_t));
    osal_memcpy(&(pstCareValue->logicPortmask), &stLogicPortmask, sizeof(sys_logic_portmask_t ));
    pstCareValue->uiService  =  uiService;

    return  SYS_ERR_OK;
 }

static int32 _cmd_acl_mgmt_ace_ipSource_get(struct cmd_element *cmd, int argc, char *argv[], sys_acl_mgmt_care_ipSource_t uiIPv4orIPv6,sys_acl_mgmt_care_value_t *pstCareValue, sys_acl_mgmt_ace_priFlag_t *pstAcepriFlag, uint32 *puiPri)
 {
    sys_acl_mgmt_ipSource_ipV4_t stIPv4addr;
    sys_acl_mgmt_ipSource_ipV6_t stIpV6addr;
    sys_acl_mgmt_care_service_t  uiService;
    sys_logic_portmask_t stLogicPortmask;
    sys_acl_mgmt_careflag_t  stCareFlag;

    SYS_PARAM_CHK((NULL == pstCareValue), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == puiPri), SYS_ERR_NULL_POINTER);

    osal_memset(&stIPv4addr, 0, sizeof(sys_acl_mgmt_ipSource_ipV4_t ));
    osal_memset(&stIpV6addr, 0, sizeof(sys_acl_mgmt_ipSource_ipV6_t));
    uiService          = SERVICE_ALL;
    LOGIC_PORTMASK_CLEAR_ALL(stLogicPortmask);
    osal_memset(&stCareFlag, 0, sizeof(sys_acl_mgmt_careflag_t));
    osal_memset(pstCareValue, 0, sizeof(sys_acl_mgmt_care_value_t));
    *puiPri                = 0;
    *pstAcepriFlag = PRIFLAG_INVALID;

    if (3 == argc)
    {
        SYS_ERR_CHK(_cmd_acl_mgmt_Arg_check(3,  argv));
        SYS_ERR_CHK(_cmd_acl_mgmt_noIpSourceNopri_argTwo_set(argc, argv, 1, &stCareFlag, &uiService, &stLogicPortmask));
        if (IPSOURCE_IPV4 == uiIPv4orIPv6)
        {
            SYS_ERR_CHK(_cmd_acl_mgmt_ipSource_ip_get(0, argv, &(stIPv4addr.uiSIpv4Addr), &(stIPv4addr.uiSIpv4Netmask)));
            stCareFlag.careipv4           = 1;
            stCareFlag.careipv4mask = 1;
        }
        else
        {
            SYS_ERR_CHK (_cmd_acl_mgmt_ipSource_ipv6_get  (0, argv, stIpV6addr.uaSIpv6Addr,  &(stIpV6addr.uiSIpv6PfLen)));
            stCareFlag.careipv6 = 1;
        }
    }
    else if  (4 == argc)
    {
        SYS_ERR_CHK(_cmd_acl_mgmt_Arg_check(4,  argv));
        *puiPri                 = SYS_STR2UINT(argv[0]);
        *pstAcepriFlag  = PRIFLAG_VALID;
        SYS_ERR_CHK(_cmd_acl_mgmt_noIpSourceNopri_argTwo_set(argc, argv, 2, &stCareFlag, &uiService, &stLogicPortmask));

        if (IPSOURCE_IPV4 == uiIPv4orIPv6)
        {
            SYS_ERR_CHK(_cmd_acl_mgmt_ipSource_ip_get(1, argv, &(stIPv4addr.uiSIpv4Addr), &(stIPv4addr.uiSIpv4Netmask)));
            stCareFlag.careipv4  = 1;
            stCareFlag.careipv4mask = 1;
        }
        else
        {
            SYS_ERR_CHK (_cmd_acl_mgmt_ipSource_ipv6_get  (1, argv, stIpV6addr.uaSIpv6Addr,  &(stIpV6addr.uiSIpv6PfLen)));
            stCareFlag.careipv6 = 1;
         }
    }
    osal_memcpy(&(pstCareValue->stIpV4addr), &stIPv4addr, sizeof(sys_acl_mgmt_ipSource_ipV4_t));
    osal_memcpy(&(pstCareValue->stIpV6addr), &stIpV6addr, sizeof(sys_acl_mgmt_ipSource_ipV6_t ));
    osal_memcpy(&(pstCareValue->stCareFlag), &stCareFlag, sizeof(sys_acl_mgmt_careflag_t));
    osal_memcpy(&(pstCareValue->logicPortmask), &stLogicPortmask, sizeof(sys_logic_portmask_t ));
    pstCareValue->uiService  =  uiService;

    return SYS_ERR_OK;
}

static  int32 _cmd_acl_mgmt_list_del(struct vty *vty, char *pcListName)
{
  SYS_PARAM_CHK((NULL == pcListName), SYS_ERR_NULL_POINTER);
  if (0 == osal_strcmp(pcListName, acConsoleset))
   {
       XMORE("%s\n","Cannot delete console-only list");
       return SYS_ERR_OK;
   }
  SYS_ERR_CHK(sal_acl_mgmt_list_del(pcListName));
  return SYS_ERR_OK;
}

static int32 _cmd_acl_mgmt_list_add(struct vty *vty, char *pcListName)
{

    sys_err_code_t ret = SYS_ERR_OK;
    SYS_PARAM_CHK((NULL == pcListName), SYS_ERR_NULL_POINTER);
    char acName[SYS_MGMT_ACL_NAME_LENTH_MAX +1] = "";

    if (osal_strlen(pcListName) > SYS_MGMT_ACL_NAME_LENTH_MAX)
        return SYS_ERR_STR_LEN_EXCEED;

    osal_strcpy(acName,pcListName);
    ret = sal_acl_mgmt_list_add(acName);
    if (( SYS_ERR_OK != ret ) && (SYS_ERR_MGMT_ACL_EXIST !=ret ))
    {
         return ret;
    }
   osal_memset(g_Name, 0, sizeof( g_Name));
   osal_memcpy( g_Name, pcListName, strlen(pcListName));

   vty->node = MGMT_ACL_NODE;
   return SYS_ERR_OK;
}

int32 cmd_acl_mgmt_ace2cmd_convert( sys_acl_mgmt_ace_t  *pstacl_mgmt_ace, char *buf)
{
    char iptmp[16]              = {0};
    char   strIpv6Addr[40] = {0};
    char strPortmask[CAPA_PORTMASK_STR_LEN] = {0};
    sys_vid_t   vid = 0;
    sys_acl_mgmt_care_value_t stCare;
    uint32 len = 0;

    SYS_PARAM_CHK((NULL == pstacl_mgmt_ace), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == buf), SYS_ERR_NULL_POINTER);

    osal_memset(&stCare, 0, sizeof(sys_acl_mgmt_care_value_t ));
    osal_memcpy(&stCare, &(pstacl_mgmt_ace->stCare), sizeof(sys_acl_mgmt_care_value_t ));

    len += osal_sprintf(buf + len , "sequence %u ", pstacl_mgmt_ace->uiMgmtAcePri);
    if (MGMTACTION_PERMIT == (pstacl_mgmt_ace->stAction))
    {
        len += osal_sprintf(buf + len , "%s","permit ");
    }
    else
    {
        len += osal_sprintf(buf + len , "%s", "deny ");
    }
     if ( 1 == stCare.stCareFlag.careipv4)
    {
        len += osal_sprintf(buf + len , "%s", "ip ");
        osal_memset(iptmp, 0, sizeof(iptmp));
        sys_util_ip2Str(stCare.stIpV4addr.uiSIpv4Addr, iptmp);
        len += osal_sprintf(buf + len , "%s/", iptmp);
        osal_memset(iptmp, 0, sizeof(iptmp));
        sys_util_ip2Str(stCare.stIpV4addr.uiSIpv4Netmask, iptmp);
        len += osal_sprintf(buf + len , "%s ", iptmp);
    }
    else if (1 == stCare.stCareFlag.careipv6)
    {
        len += osal_sprintf(buf + len , "%s", "ipv6 ");
        osal_memset(strIpv6Addr, 0, sizeof(strIpv6Addr));
        inet_ntop(AF_INET6, stCare.stIpV6addr.uaSIpv6Addr,  strIpv6Addr, sizeof( strIpv6Addr));
        len += osal_sprintf(buf + len , "%s/", strIpv6Addr);
        len += osal_sprintf(buf + len , "%u ", stCare.stIpV6addr.uiSIpv6PfLen);
    }
    LPM2STR(&(stCare.logicPortmask), strPortmask);
    len += osal_sprintf(buf + len , "interfaces %s ", strPortmask);
    if (1 == stCare.stCareFlag.carevlan)
    {
        vid = stCare.vid;
        len += osal_sprintf(buf + len , "vlan %u ", vid);
     }
    len += osal_sprintf(buf + len , "%s", "service ");
    if( SERVICE_HTTP == stCare.uiService)
    {
        len += osal_sprintf(buf + len , "%s", "http ");
    }
    else if(SERVICE_HTTPS== stCare.uiService)
    {
        len += osal_sprintf(buf + len , "%s", "https ");
    }
    else if(SERVICE_SNMP == stCare.uiService)
    {
        len += osal_sprintf(buf + len , "%s", "snmp ");
    }
    else if(SERVICE_SSH == stCare.uiService)
    {
        len += osal_sprintf(buf + len , "%s", "ssh ");
    }
    else if (SERVICE_TELNET == stCare.uiService)
    {
        len += osal_sprintf(buf + len , "%s", "telnet ");
    }
    else if (SERVICE_ALL== stCare.uiService)
    {
        len += osal_sprintf(buf + len , "%s", "all ");
    }
    len += osal_sprintf(buf + len , "\n");

    return SYS_ERR_OK;
}

#if 0

static int32 _cmd_acl_mgmt_list_show(struct vty *vty, char *pcListName, uint32 uiAceNum)
{

    uint32 len = 0;
    sys_acl_mgmt_ace_t  stacl_mgmt_ace;
    int i;
    char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1] = "";
    char buf[SYS_BUF1024_LEN] = {0};

    SYS_PARAM_CHK((NULL == pcListName), SYS_ERR_NULL_POINTER);

    osal_strcpy(acListName, pcListName);
    XMORE("\n");
    XMORE("%s\n", acListName);
    for (i = 0; i < (osal_strlen(acListName)); i++)
    {
        len += osal_sprintf(buf + len , "%s","-");
        buf[SYS_BUF1024_LEN -1] = '\0';
    }
    XMORE("%s\n", buf);
    osal_memset(buf, 0 , sizeof(buf));
    for (i = 0; i <uiAceNum; i++)
    {
        osal_memset(&stacl_mgmt_ace, 0 , sizeof(&stacl_mgmt_ace));
        osal_memset(buf, 0 , sizeof(buf));
        SYS_ERR_CHK(sal_acl_mgmt_ace_get(pcListName, i, &stacl_mgmt_ace));
        SYS_ERR_CHK(cmd_acl_mgmt_ace2cmd_convert(&stacl_mgmt_ace, buf));
        XMORE("  %s",buf);
    }
   XMORE("  %s\n","! (Note: all other access implicitly denied)");
   XMORE("\n");
   return SYS_ERR_OK;
}
#endif

int32 cmd_acl_mgmtList_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1] = "";
    char buf[SYS_BUF1024_LEN] = {0};
    uint32 uiAceNum = 0;
    uint32 uiListNum = 0;
    int i;
    int j;
    uint32 len = 0;
    sys_acl_mgmt_ace_t  stacl_mgmt_ace;

    SYS_ERR_CHK(sal_acl_mgmt_list_num_get( &uiListNum));
    XMORE("\n");

    if (0 == argc)
    {
        if (1 == uiListNum)
            XMORE("%s\n","1 management  access-list is created");
        else
            XMORE("%d management  access-lists are created\n", uiListNum);
    }

    for (i = 0; i < uiListNum; i++)
    {
        osal_memset(acListName, 0 , sizeof(acListName));
        SYS_ERR_CHK(sal_acl_mgmt_name_get(acListName,i));

        if (1 == argc)
        {
            if (0 != osal_strncmp(acListName, argv[0], osal_strlen(acListName)))
                continue;
        }

        SYS_ERR_CHK(sal_acl_mgmt_list_acenum_get(acListName,&uiAceNum));

        XMORE("\n");
        XMORE("%s\n", acListName);
        osal_memset(buf, 0 , sizeof(buf));
        len = 0;
        for (j = 0; j< (osal_strlen(acListName)); j++)
        {
            len += osal_sprintf(buf + len , "%s","-");
        }
        buf[SYS_BUF1024_LEN -1] = '\0';
        XMORE("%s\n", buf);

        for (j= 0; j <uiAceNum; j++)
        {
            osal_memset(&stacl_mgmt_ace, 0 , sizeof(stacl_mgmt_ace));
            osal_memset(buf, 0 , sizeof(buf));
            SYS_ERR_CHK(sal_acl_mgmt_ace_get(acListName, j, &stacl_mgmt_ace));
            SYS_ERR_CHK(cmd_acl_mgmt_ace2cmd_convert(&stacl_mgmt_ace, buf));
            XMORE("  %s",buf);
        }
        XMORE("  %s\n","! (Note: all other access implicitly denied)");
    }

    if (1 == argc && i == uiListNum)
        XMORE("%s\n","List does not exist");

    return SYS_ERR_OK;
}

int32 cmd_acl_mgmtClass_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1] = "";
    sys_err_code_t stRet;

    stRet = sal_acl_mgmt_class_get(acListName);

    if (SYS_ERR_MGMT_ACL_CLASS_DISABLE ==  stRet)
    {
        XMORE("%s\n","Management access-class is disabled");
    }
    else
    {
        XMORE("Management access-class is enabled, using access-list %s\n",acListName);
    }
    return SYS_ERR_OK;
}

int32 cmd_acl_mgmt_ace_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_PARAM_CHK((NULL == argv[0]), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_acl_mgmt_ace_del(g_Name, SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}

int32 cmd_acl_mgmt_list_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char  chListNameSet[SYS_MGMT_ACL_NAME_LENTH_MAX+1] = "";

    SYS_PARAM_CHK((NULL == argv[0]), SYS_ERR_NULL_POINTER);

    if (SYS_MGMT_ACL_NAME_LENTH_MAX < osal_strlen(argv[0]) )
    {
        return SYS_ERR_STR_LEN_EXCEED;
    }

    osal_strcpy(chListNameSet, argv[0]);

    if (CMD_IS_NO_FORM_EXIST())
    {

         SYS_ERR_CHK(_cmd_acl_mgmt_list_del(vty,chListNameSet));
    }
   else
   {
        SYS_ERR_CHK(_cmd_acl_mgmt_list_add(vty,chListNameSet));
   }
   return SYS_ERR_OK;

}

int32 cmd_acl_mgmt_list_exit(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 uiAceNum = 0;
    vty->node = CONFIG_NODE;

    if (0 == osal_strcmp(g_Name, acConsoleset))
    {
       return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_acl_mgmt_list_acenum_get(g_Name, &uiAceNum));

    if (0 == uiAceNum)
    {
         SYS_ERR_CHK(sal_acl_mgmt_list_del(g_Name));
    }

    return SYS_ERR_OK;
}

int32 cmd_acl_mgmt_ace_ipSourceNocare_permit(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_acl_mgmt_ace_t  stCareValue;
    sys_acl_mgmt_ace_priFlag_t stAcepriFlag;
    uint32 uiPri = 0;

    if (CMD_IS_CLI_RUN())
    {
        ACL_MGMT_CONNAME_COMPARE();
    }

    osal_memset (&stCareValue, 0, sizeof(sys_acl_mgmt_ace_t));
    SYS_ERR_CHK(_cmd_acl_mgmt_ace_noIpSource_get( cmd, argc, argv , &(stCareValue.stCare),  &stAcepriFlag, &uiPri));

    stCareValue.stAction                            =   MGMTACTION_PERMIT;
    stCareValue.uiMgmtAcePri                 =   uiPri;

    SYS_ERR_CHK(sal_acl_mgmt_ace_add(g_Name, stAcepriFlag,  &stCareValue));
    return SYS_ERR_OK;
}

 int32 cmd_acl_mgmt_ace_ipCare_permit(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
 {
    sys_acl_mgmt_ace_t  stCareValue;
    sys_acl_mgmt_ace_priFlag_t stAcepriFlag;
    uint32 uiPri = 0;

    if (CMD_IS_CLI_RUN())
    {
        ACL_MGMT_CONNAME_COMPARE();
    }

    osal_memset (&stCareValue, 0, sizeof(sys_acl_mgmt_ace_t));

    SYS_ERR_CHK(_cmd_acl_mgmt_ace_ipSource_get( cmd, argc, argv , IPSOURCE_IPV4, &(stCareValue.stCare),  &stAcepriFlag, &uiPri));

    stCareValue.stAction                            =   MGMTACTION_PERMIT;
    stCareValue.uiMgmtAcePri                 =   uiPri;

    SYS_ERR_CHK(sal_acl_mgmt_ace_add(g_Name, stAcepriFlag,  &stCareValue));

    return SYS_ERR_OK;
 }

int32 cmd_acl_mgmt_ace_ipv6Care_permit(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_acl_mgmt_ace_t  stCareValue;
    sys_acl_mgmt_ace_priFlag_t stAcepriFlag;
    uint32 uiPri = 0;

    if (CMD_IS_CLI_RUN())
    {
        ACL_MGMT_CONNAME_COMPARE();
    }

    osal_memset (&stCareValue, 0, sizeof(sys_acl_mgmt_ace_t));

    SYS_ERR_CHK(_cmd_acl_mgmt_ace_ipSource_get( cmd, argc, argv , IPSOURCE_IPV6, &(stCareValue.stCare),  &stAcepriFlag, &uiPri));

    stCareValue.stAction                            =   MGMTACTION_PERMIT;
    stCareValue.uiMgmtAcePri                 =   uiPri;

    SYS_ERR_CHK(sal_acl_mgmt_ace_add(g_Name, stAcepriFlag,  &stCareValue));

    return SYS_ERR_OK;
}

int32 cmd_acl_mgmt_ace_ipSourceNocare_deny(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_acl_mgmt_ace_t  stCareValue;
    sys_acl_mgmt_ace_priFlag_t stAcepriFlag;
    uint32 uiPri = 0;

    if (CMD_IS_CLI_RUN())
    {
        ACL_MGMT_CONNAME_COMPARE();
    }

    osal_memset (&stCareValue, 0, sizeof(sys_acl_mgmt_ace_t));

    SYS_ERR_CHK(_cmd_acl_mgmt_ace_noIpSource_get( cmd, argc, argv , &(stCareValue.stCare),  &stAcepriFlag, &uiPri));

    stCareValue.stAction                            =   MGMTACTION_DENY;
    stCareValue.uiMgmtAcePri                 =   uiPri;

    SYS_ERR_CHK(sal_acl_mgmt_ace_add(g_Name, stAcepriFlag,  &stCareValue));

   return SYS_ERR_OK;
}

 int32 cmd_acl_mgmt_ace_ipCare_deny(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{

    sys_acl_mgmt_ace_t  stCareValue;
    sys_acl_mgmt_ace_priFlag_t stAcepriFlag;
    uint32 uiPri = 0;

    if (CMD_IS_CLI_RUN())
    {
        ACL_MGMT_CONNAME_COMPARE();
    }

    osal_memset (&stCareValue, 0, sizeof(sys_acl_mgmt_ace_t));

    SYS_ERR_CHK(_cmd_acl_mgmt_ace_ipSource_get( cmd, argc, argv , IPSOURCE_IPV4, &(stCareValue.stCare),  &stAcepriFlag, &uiPri));

    stCareValue.stAction                            =   MGMTACTION_DENY;
    stCareValue.uiMgmtAcePri                 =   uiPri;

    SYS_ERR_CHK(sal_acl_mgmt_ace_add(g_Name, stAcepriFlag,  &stCareValue));

    return SYS_ERR_OK;
}

 int32 cmd_acl_mgmt_ace_ipv6Care_deny(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_acl_mgmt_ace_t  stCareValue;
    sys_acl_mgmt_ace_priFlag_t stAcepriFlag;
    uint32 uiPri = 0;

    if (CMD_IS_CLI_RUN())
    {
        ACL_MGMT_CONNAME_COMPARE();
    }

    osal_memset (&stCareValue, 0, sizeof(sys_acl_mgmt_ace_t));

    SYS_ERR_CHK(_cmd_acl_mgmt_ace_ipSource_get( cmd, argc, argv , IPSOURCE_IPV6, &(stCareValue.stCare),  &stAcepriFlag, &uiPri));

    stCareValue.stAction                            =   MGMTACTION_DENY;
    stCareValue.uiMgmtAcePri                 =   uiPri;

    SYS_ERR_CHK(sal_acl_mgmt_ace_add(g_Name, stAcepriFlag,  &stCareValue));

    return SYS_ERR_OK;
}

int32 cmd_acl_mgmt_class_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char acName [SYS_MGMT_ACL_NAME_LENTH_MAX+1] ;

    SYS_PARAM_CHK((NULL == argv[0]), SYS_ERR_NULL_POINTER);

    if (SYS_MGMT_ACL_NAME_LENTH_MAX < osal_strlen(argv[0]) )
    {
        return SYS_ERR_STR_LEN_EXCEED;
    }

    osal_memset(acName, 0, sizeof(acName));
    osal_strcpy(acName, argv[0]);

    SYS_ERR_CHK(sal_acl_mgmt_class_set(acName));
    return SYS_ERR_OK;
}

int32 cmd_acl_mgmt_class_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_acl_mgmt_class_del());
    return SYS_ERR_OK;
}

int32 cmd_acl_mgmt_save(FILE *fp)
{
    uint32 uiListNum = 0;
    uint32 uiAceNum = 0;
    char buf[SYS_BUF1024_LEN];
    char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX+1] = "" ;
    sys_acl_mgmt_ace_t  stacl_mgmt_ace;
    uint32 i = 0;
    uint32 j = 0;

    SYS_ERR_CHK(sal_acl_mgmt_list_num_get( &uiListNum));
    for (i = 0; i < uiListNum; i++)
    {
        osal_memset(buf, 0 , sizeof(buf));
        osal_memset(acListName, 0 , sizeof(acListName));
        SYS_ERR_CHK(sal_acl_mgmt_name_get(acListName,i));
        if (0 == osal_strcmp(acListName, acConsoleset))
        {
            continue;
        }
        osal_sprintf(buf,"management access-list \"%s\"\n",acListName);
        SYS_ERR_CHK(sal_acl_mgmt_list_acenum_get(acListName,&uiAceNum));
        CMD_WRITE_TO_FILE(fp, buf);
        for (j = 0; j < uiAceNum; j++)
        {
            osal_memset(&stacl_mgmt_ace, 0 , sizeof(stacl_mgmt_ace));
            osal_memset(buf, 0 , sizeof(buf));
            SYS_ERR_CHK(sal_acl_mgmt_ace_get(acListName, j, &stacl_mgmt_ace));
            SYS_ERR_CHK(cmd_acl_mgmt_ace2cmd_convert(&stacl_mgmt_ace, buf));
            CMD_WRITE_TO_FILE_INDENT1(fp, buf);
        }
    }

    SYS_ERR_CHK(sal_acl_mgmt_class_get(acListName));

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "management access-class \"%s\"\n",acListName);
    CMD_WRITE_TO_FILE(fp, buf);

    return SYS_ERR_OK;
}

