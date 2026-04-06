
#include <libcmd/cmd.h>

int32 cmd_snmp_demo_restart(void)
{
    sal_snmp_reload_t reload;

    if (CMD_IS_CLI_RUN())
    {
        SYS_ERR_CHK(sal_snmp_config_reload_get(&reload));
        if(reload == SNMP_RELOAD_AUTO)
        {
            SYS_ERR_CHK(sal_snmp_restart());
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_snmp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    #ifdef CONFIG_SYS_SNMPV3
    
    #else
    uint32 i = 0;
    uint32 count = 0;
    sal_snmp_target_t trap;
    sal_snmp_community_t community;
    #endif

    sys_enable_t enable;

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (ENABLED == enable)
        XMORE("SNMP is enabled.\n");
    else
        XMORE("SNMP is disabled.\n");

#ifdef CONFIG_SYS_SNMPV3
 
#else
    XMORE("Community Name  \tAccess Right \n");
    XMORE("-----------------------------------------\n");

    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        SYS_ERR_CHK(sal_snmp_community_get(i, &community));

        if (osal_strlen(community.name) != 0)
        {
            if (SNMP_COMMUNITY_ACCESS_READ_ONLY == community.access)
                XMORE("%-16s\t read-only\n", community.name);
            else
                XMORE("%-16s\t read-write\n", community.name);
            count++;
        }

    }
    XMORE("\nTotal Community Entries: %d\n", count);

    count = 0;
    XMORE("Server             Community Name   Notification Version    Notification Type    \n");
    XMORE("---------------------------------------------------------------------------------\n");
    for (i = 0; i < CAPA_SNMP_HOST_NUM; i++)
    {
        SYS_ERR_CHK(sal_snmp_target_get(i, &trap));

        if (osal_strlen(trap.server) != 0)
        {
            XMORE("%-18s %-17s%-23s %-20s \n", trap.server, trap.name,
                    gsecModel[trap.secModel], gtrapType[trap.trapType]);
            count++;
        }
    }
    XMORE("\nTotal Trap Entries: %d\n", count);
#endif

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MIB_TRAP

int32 cmd_snmp_trap_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t authEnable = DISABLED;
    sys_enable_t linkEnable = DISABLED;
    sys_enable_t warmEnable = DISABLED;
    sys_enable_t coldEnable = DISABLED;

    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &authEnable));
    XMORE("SNMP auth failed trap  : %s \n", (authEnable == ENABLED) ? "Enable" : "Disable");

    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_LINKUPDOWN, &linkEnable));
    XMORE("SNMP linkUpDown trap   : %s \n", (linkEnable == ENABLED) ? "Enable" : "Disable");

    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_COOL, &coldEnable));
    XMORE("SNMP cold-start trap   : %s \n", (coldEnable == ENABLED) ? "Enable" : "Disable");

    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_WARM, &warmEnable));
    XMORE("SNMP warm-start trap   : %s \n", (warmEnable == ENABLED) ? "Enable" : "Disable");

    return SYS_ERR_OK;
}
#endif

int32 cmd_snmp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t old_enable;
    sys_enable_t new_enable;

    SYS_ERR_CHK(sal_snmp_enable_get(&old_enable));

    if (CMD_IS_NO_FORM_EXIST())
        new_enable = DISABLED;
    else
        new_enable = ENABLED;

    SYS_ERR_CHK(sal_snmp_enable_set(new_enable));

    if (CMD_IS_CLI_RUN())
    {
        if (new_enable != old_enable)
        {
            if (ENABLED == new_enable)
                SYS_ERR_CHK(sal_snmp_restart());
            else
                SYS_ERR_CHK(sal_snmp_stop());
        }
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MIB_TRAP

int32 cmd_snmp_trapClass_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sal_snmp_trapClass_t trapClass = SNMP_TRAP_CLASS_END;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    if (argv[0][0] == 'a')
        trapClass = SNMP_TRAP_CLASS_AUTHENTICATE;
    else if (argv[0][0] == 'l')
        trapClass = SNMP_TRAP_CLASS_LINKUPDOWN;
    else if (argv[0][0] == 'w')
        trapClass = SNMP_TRAP_CLASS_BOOTUP_WARM;
    else if (argv[0][0] == 'c')
        trapClass = SNMP_TRAP_CLASS_BOOTUP_COOL;

    SYS_ERR_CHK(sal_snmp_trapEnable_set(trapClass, enable));

    return SYS_ERR_OK;
}
#endif

int32 cmd_snmp_host_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sal_snmp_target_t  hostEntry;
    sal_snmp_secmode_t  secModel = SNMP_SECMODEL_V1;
    sal_snmp_trapType_t  trapType = SNMP_TRAP_V1;
    uint16  destPort = DFLT_SNMP_NOTIFACTION_PORT;
    uint16  timeOut = DFLT_SNMP_INFORM_TIMEOUT;
    uint16  retries = DFLT_SNMP_INFORM_RETRY;
    uint8 isTrap = TRUE;
    uint8 isArgTrap = FALSE;
    uint8 isArgVersion = FALSE;
    uint8 isArgPort = FALSE;
    uint8 isArgTime = FALSE;
    uint8 isArgRetry = FALSE;

    osal_memset(&hostEntry, 0, sizeof(sal_snmp_target_t));
    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_HOSTNAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    if (IS_IPV4_UADDRSTR_INVALID(argv[0]))
    {
        if IS_IPV6_UADDRSTR_INVALID(argv[0])
        {
            if (IS_HOSTNAME_INVALID(argv[0]))
                return SYS_ERR_HOSTNAME;
        }
    }

    if (argc == 2)
    {
        secModel = SNMP_SECMODEL_V1;
        trapType = SNMP_TRAP_V1;

        osal_memcpy(hostEntry.server, argv[0], osal_strlen(argv[0]));

        if (osal_strlen(argv[1]) > CAPA_SNMP_COMMUNITY_LEN)
            return SYS_ERR_STR_LEN_EXCEED;

        osal_memcpy(hostEntry.name, argv[1], osal_strlen(argv[1]));
        hostEntry.secModel = secModel;
        hostEntry.trapType = trapType;
        hostEntry.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
        hostEntry.portNum = destPort;
        hostEntry.retries = DFLT_SNMP_INFORM_RETRY;
        hostEntry.timeout = DFLT_SNMP_INFORM_TIMEOUT;
        hostEntry.trapType = trapType;

        SYS_ERR_CHK(sal_snmp_target_add(&hostEntry));
    }
    else
    {
        if ((NULL != osal_strstr(cmd->string, "informs")) || (NULL != osal_strstr(cmd->string, "traps")))
        {
            isArgTrap = TRUE;

            if (NULL != osal_strstr(cmd->string, "informs"))
                isTrap = FALSE;
        }

        if (NULL != osal_strstr(cmd->string, "version"))
        {
            isArgVersion = TRUE;
        }

        if (NULL != osal_strstr(cmd->string, "udp-port"))
        {
            isArgPort = TRUE;
        }

        if (NULL != osal_strstr(cmd->string, "timeout"))
        {
            isArgTime = TRUE;
        }

        if (NULL != osal_strstr(cmd->string, "retries"))
        {
            isArgRetry = TRUE;
        }

        if (argc == 3)
        {
            if (isArgTrap)
            {
                if (FALSE == isTrap)
                {
                    return SYS_ERR_SNMP_INFORM;
                }
            }

            if (isArgVersion)
            {
                switch(argv[1][0])
                {
                    case '1':
                        trapType = SNMP_TRAP_V1;
                        break;
                    case '2':
                        if (isTrap)
                        {
                            trapType = SNMP_TRAP_V2;
                        }
                        else
                            trapType = SNMP_INFORM_V2;
                        secModel = SNMP_SECMODEL_V2C;
                        break;
                    default :
                        break;
                }
            }

            if (isArgPort)
            {
                destPort = (uint16)SYS_STR2UINT(argv[2]);
            }

            if (isArgTime)
            {
                timeOut = (uint16)SYS_STR2UINT(argv[2]);
            }

            if (isArgRetry)
            {
                retries = (uint16)SYS_STR2UINT(argv[2]);
            }
        }
        else if (argc == 4)
        {

            if (isArgVersion)
            {
                if (isArgTrap)
                {
                    switch(argv[2][0])
                    {
                        case '1':
                            secModel = SNMP_SECMODEL_V1;
                            break;
                        case '2':
                            secModel = SNMP_SECMODEL_V2C;
                            if (isTrap)
                                trapType = SNMP_TRAP_V2;
                            else
                                trapType = SNMP_INFORM_V2;
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    switch(argv[1][0])
                    {
                        case '1':
                            secModel = SNMP_SECMODEL_V1;
                            break;
                        case '2':
                            secModel = SNMP_SECMODEL_V2C;
                            if (isTrap)
                                trapType = SNMP_TRAP_V2;
                            else
                                trapType = SNMP_INFORM_V2;
                            break;
                        default:
                            break;
                    }
                 }
            }

           if (isArgPort)
           {
                if (isArgTrap || isArgVersion)
                    destPort = (uint16)SYS_STR2UINT(argv[3]);
                else
                    destPort = (uint16)SYS_STR2UINT(argv[2]);
           }

           if (isArgTime)
           {
                if (isArgTrap || isArgVersion || isArgPort)
                    timeOut = (uint16)SYS_STR2UINT(argv[3]);
                else
                    timeOut = (uint16)SYS_STR2UINT(argv[2]);
           }

          if (isArgRetry)
          {
                retries = (uint16)SYS_STR2UINT(argv[3]);
          }
        }
        else if (argc == 5)
        {
            if (isArgVersion)
            {
                if (isArgTrap)
                {
                    switch(argv[2][0])
                    {
                        case '1':
                            secModel = SNMP_SECMODEL_V1;
                            break;
                        case '2':
                            secModel = SNMP_SECMODEL_V2C;
                            if (isTrap)
                                trapType = SNMP_TRAP_V2;
                            else
                                trapType = SNMP_INFORM_V2;
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    switch(argv[1][0])
                    {
                        case '1':
                            secModel = SNMP_SECMODEL_V1;
                            break;
                        case '2':
                            secModel = SNMP_SECMODEL_V2C;
                            if (isTrap)
                                trapType = SNMP_TRAP_V2;
                            else
                                trapType = SNMP_INFORM_V2;
                            break;
                        default:
                            break;
                    }
                 }
            }

            if (isArgPort)
            {
                if (isArgTrap && isArgVersion)
                {
                    destPort = (uint16)SYS_STR2UINT(argv[4]);
                }
                else if (isArgTrap || isArgVersion)
                {
                    destPort = (uint16)SYS_STR2UINT(argv[3]);
                }
                else
                {
                    destPort = (uint16)SYS_STR2UINT(argv[2]);
                }
            }

            if (isArgTime)
            {
                if ((isArgTrap && isArgVersion) || (isArgTrap && isArgPort) || (isArgPort && isArgPort))
                {
                    timeOut = (uint16)SYS_STR2UINT(argv[4]);
                }
                else
                {
                    timeOut = (uint16)SYS_STR2UINT(argv[3]);
                }
            }

            if (isArgRetry)
            {
                retries = (uint16)SYS_STR2UINT(argv[4]);
            }
        }
        else if (argc == 6)
        {
            if (!isArgTrap)
            {
                  switch(argv[1][0])
                  {
                        case '1':
                            secModel = SNMP_SECMODEL_V1;
                            break;
                        case '2':
                            secModel = SNMP_SECMODEL_V2C;
                            if (isTrap)
                                trapType = SNMP_TRAP_V2;
                            else
                                trapType = SNMP_INFORM_V2;
                            break;
                        default:
                            break;
                  }

                 destPort = (uint16)SYS_STR2UINT(argv[3]);
                 timeOut = (uint16)SYS_STR2UINT(argv[4]);
                 retries = (uint16)SYS_STR2UINT(argv[5]);
            }
            else
            {
                  switch(argv[2][0])
                  {
                        case '1':
                            secModel = SNMP_SECMODEL_V1;
                            break;
                        case '2':
                            secModel = SNMP_SECMODEL_V2C;
                            if (isTrap)
                                trapType = SNMP_TRAP_V2;
                            else
                                trapType = SNMP_INFORM_V2;
                            break;
                        default:
                            break;
                  }
            }

            if (!isArgVersion)
            {
              destPort = (uint16)SYS_STR2UINT(argv[3]);
              timeOut = (uint16)SYS_STR2UINT(argv[4]);
              retries = (uint16)SYS_STR2UINT(argv[5]);
            }

            if (!isArgPort)
            {
              timeOut = (uint16)SYS_STR2UINT(argv[4]);
              retries = (uint16)SYS_STR2UINT(argv[5]);
            }

            if (!isArgTime)
            {
              destPort = (uint16)SYS_STR2UINT(argv[4]);
              retries = (uint16)SYS_STR2UINT(argv[5]);
            }

            if (!isArgRetry)
            {
              destPort = (uint16)SYS_STR2UINT(argv[4]);
              timeOut = (uint16)SYS_STR2UINT(argv[5]);
            }
        }
        else
        {
            switch(argv[2][0])
            {
                case '1':
                    secModel = SNMP_SECMODEL_V1;
                    break;
                case '2':
                    secModel = SNMP_SECMODEL_V2C;
                    if (isTrap)
                        trapType = SNMP_TRAP_V2;
                    else
                        trapType = SNMP_INFORM_V2;
                    break;
                default:
                    break;
            }
           destPort = (uint16)SYS_STR2UINT(argv[4]);
           timeOut = (uint16)SYS_STR2UINT(argv[5]);
           retries = (uint16)SYS_STR2UINT(argv[6]);
        }

        if (SNMP_SECMODEL_V1 == secModel &&  FALSE == isTrap)
              return SYS_ERR_SNMP_INFORM;

        if (isTrap && isArgTime)
        {
            XMORE("Notifaction type is trap , don't need input timeout!\n");
            return SYS_ERR_OK;
        }

        if (isTrap && isArgRetry)
        {
            XMORE("Notifaction type is trap , don't need input retries!\n");
            return SYS_ERR_OK;
        }

        osal_memcpy(hostEntry.server, argv[0], osal_strlen(argv[0]));
        if (isArgTrap && isArgVersion)
            osal_memcpy(hostEntry.name, argv[3], osal_strlen(argv[3]));
        else if(isArgTrap || isArgVersion)
            osal_memcpy(hostEntry.name, argv[2], osal_strlen(argv[2]));
        else
            osal_memcpy(hostEntry.name, argv[1], osal_strlen(argv[1]));

        hostEntry.trapType = trapType;
        hostEntry.secModel = secModel;
        hostEntry.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
        hostEntry.portNum = destPort;
        hostEntry.retries = retries;
        hostEntry.timeout = timeOut;

       SYS_ERR_CHK(sal_snmp_target_add(&hostEntry));
    }

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_host_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sal_snmp_target_t host;
    sal_snmp_trapType_t  type = SNMP_TRAP_V1;
    sal_snmp_secmode_t  secModel = SNMP_SECMODEL_V1;
    uint8 isInputTrap = FALSE;
    uint8 isInputVersion = FALSE;
    uint8 isTrap = TRUE;
    uint32 i;

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_HOSTNAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    if (argc == 2)
    {
        if ((NULL != osal_strstr(cmd->string, "traps")) || (NULL != osal_strstr(cmd->string, "informs")))
        {
            isInputTrap = TRUE;
            if (NULL != osal_strstr(cmd->string, "informs"))
            {
                isTrap = FALSE;
            }
        }

        if (NULL != osal_strstr(cmd->string, "version"))
        {
            isInputVersion = TRUE;
            switch(argv[1][0])
            {
                case '1':
                    secModel = SNMP_SECMODEL_V1;
                    break;
                case '2':
                    secModel = SNMP_SECMODEL_V2C;
                    break;
                case '3':
                    secModel = SNMP_SECMODEL_USM;
                    break;
                    default:
                        break;
            }
        }
    }
    else if (argc == 3)
    {
        isInputTrap = TRUE;
        isInputVersion = TRUE;
        if (NULL != osal_strstr(cmd->string, "informs"))
        {
            isTrap = FALSE;
        }

        switch(argv[2][0])
        {
            case '1':
                secModel = SNMP_SECMODEL_V1;
                break;
            case '2':
                secModel = SNMP_SECMODEL_V2C;
                break;
            case '3':
                secModel = SNMP_SECMODEL_USM;
                break;
                default:
                    break;
        }

        if (isTrap)
        {
            if (SNMP_SECMODEL_V1 == secModel)
                type = SNMP_TRAP_V1;
            if (SNMP_SECMODEL_V2C == secModel)
                type = SNMP_TRAP_V2;
            if (SNMP_SECMODEL_USM == secModel)
                type = SNMP_TRAP_V3;
        }
        else
        {
            if (SNMP_SECMODEL_V1 == secModel)
                return SYS_ERR_INPUT;
            if (SNMP_SECMODEL_V2C == secModel)
                type = SNMP_INFORM_V2;
            if (SNMP_SECMODEL_USM == secModel)
                type = SNMP_INFORM_V3;
        }

    }

    for (i = 0; i < CAPA_SNMP_HOST_NUM; i++)
    {
        osal_memset(&host, 0, sizeof(sal_snmp_target_t));
        SYS_ERR_CHK(sal_snmp_target_get(i, &host));
        if (0 != osal_strlen(host.server))
        {
            if (0 != osal_memcmp(argv[0], host.server, osal_strlen(argv[0])))
                continue;

            if (argc == 3)
            {
                if (type != host.trapType)
                    continue;
                 if (secModel != host.secModel)
                    continue;

                 SYS_ERR_CHK(sal_snmp_target_del(argv[0], host.secModel, host.trapType));
                 break;
            }

            if (argc == 2)
            {
                if (isInputTrap)
                {
                    if (isTrap)
                    {
                        if ((host.trapType == SNMP_INFORM_V2) || (host.trapType == SNMP_INFORM_V3) )
                            continue;
                    }
                    else
                    {
                        if ((host.trapType == SNMP_TRAP_V1) || (host.trapType ==  SNMP_TRAP_V2) || (host.trapType == SNMP_TRAP_V3) )
                            continue;
                    }
                }

                if (isInputVersion)
                {
                    if (secModel != host.secModel)
                        continue;
                }
            }

            SYS_ERR_CHK(sal_snmp_target_del(argv[0], host.secModel, host.trapType));
        }

    }

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_SNMPV3

int32 cmd_snmp_view_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32  index, count = 0;
    sal_snmp_view_t view;
    char oidStr[CAPA_SNMP_OID_STR_LEN];

    XMORE("View Name                      Subtree OID                               OID Mask                  View Type\n");
    XMORE("---------------------------    --------------------------------------    ----------------------    ---------\n");

    for (index = 0; index < CAPA_SNMP_VIEW_NUM; index++) {
        SYS_ERR_CHK(sal_snmp_view_get(index, &view));

        if (osal_strlen(view.viewName) != 0)
        {
            osal_memset(oidStr,0,sizeof(oidStr));
            sal_util_oid2str(view.subtreeOid.len, view.subtreeOid.id, oidStr);

            XMORE("%-31s",view.viewName);
            XMORE("%-42s", oidStr);
            XMORE("%-26s", (view.viewMaskStr[0] == 0) ? "all" : view.viewMaskStr);
            XMORE("%-9s\n",gviewTypeStr[view.viewType]);

            count++;
        }

    }
    XMORE("\nTotal Entries: %d\n", count);

    return SYS_ERR_OK;
}

int32 cmd_snmp_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32  index, count = 0;
    sal_snmp_access_t access;

    XMORE("Group Name                     Model  Level        ReadView                       WriteView                      NotifyView   \n");
    XMORE("------------------------------ ------ ------------ ------------------------------ ------------------------------ ------------------------------\n");

    for (index = 0; index < SYS_SNMP_ACCESS_NUM_MAX; index++) {
        SYS_ERR_CHK(sal_snmp_access_get(index, &access));

        if (osal_strlen(access.groupName) != 0)
        {
            if (0 == osal_memcmp(access.groupName, SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
                continue;

            XMORE("%-31s",access.groupName);
            XMORE("%-7s", gsecModel[access.secMode] );
            XMORE("%-13s",gsecLevel[access.secLevel]);

            if(osal_strlen(access.readView) == 0)
            XMORE("%-31s", "---");
            else
            XMORE("%-31s", access.readView);

            if(osal_strlen(access.writeView) == 0)
            XMORE("%-31s", "---");
            else
            XMORE("%-31s", access.writeView);

            if(osal_strlen(access.notifyView) == 0)
            XMORE("%-31s \n", "---");
            else
            XMORE("%-31s \n", access.notifyView);

            count++;
        }

    }
    XMORE("\nTotal Entries: %d\n", count);

    return SYS_ERR_OK;
}

int32 cmd_snmp_community_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32  index, count = 0;
    sal_snmp_com2sec_t com2sec;

    XMORE("Commnunity Name      Group Name                     View                           Access \n");
    XMORE("-------------------- ------------------------------ ------------------------------ ------------\n");

    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++) {
        osal_memset(&com2sec, 0, sizeof(sal_snmp_com2sec_t));
        SYS_ERR_CHK(sal_snmp_com2sec_get(index, &com2sec));

        if (osal_strlen(com2sec.comName) != 0)
        {
            XMORE("%-21s",com2sec.comName);
            if (SNMP_COM_MODE_BASIC ==  com2sec.mode)
            {
                XMORE("%-31s", "");
                XMORE("%-31s", com2sec.viewName);
                XMORE("%s\n", gaccess[com2sec.access]);
            }
            else
            {
                XMORE("%-31s", com2sec.groupName);
                XMORE("%-31s", "");
                XMORE("%s\n", "");
            }

            count++;
        }

    }
    XMORE("\nTotal Entries: %d\n", count);

    return SYS_ERR_OK;
}

int32 cmd_snmp_host_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char        ip_str[16];
    uint32      index, count = 0;
    sal_snmp_target_t host;
    char portStr[16], reTryStr[16], timeStr[16];

    osal_memset(ip_str, 0, sizeof(ip_str));

    XMORE("Server             Community/User Name   Notification Version    Notification Type    UDP Port    Retries     Timeout\n");
    XMORE("---------------------------------------------------------------------------------------------------------------------\n");
    for (index = 0; index < CAPA_SNMP_HOST_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_target_get(index, &host));
        if (0 != osal_strlen(host.server))
        {
            osal_memset(portStr, 0, sizeof(portStr));
            osal_memset(reTryStr, 0, sizeof(reTryStr));
            osal_memset(timeStr, 0, sizeof(timeStr));
            osal_sprintf(portStr,"%d", host.portNum);

            if (SNMP_INFORM_V2 == host.trapType || SNMP_INFORM_V3 == host.trapType)
            {
                osal_sprintf(reTryStr,"%d", host.retries);
                osal_sprintf(timeStr,"%d", host.timeout);
            }
            else
            {
                osal_sprintf(reTryStr,"%s", "--");
                osal_sprintf(timeStr,"%s", "--");
            }
            XMORE("%-18s %-22s%-23s %-20s %-12s%-12s %s  \n", host.server, host.name,
                    gsecModel[host.secModel], gtrapType[host.trapType], portStr, reTryStr, timeStr);

            count++;
        }
    }
    XMORE("\nTotal Entries: %d\n", count);

    return SYS_ERR_OK;
}

int32 cmd_snmp_user_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32      index, count = 0;
    sal_snmp_user_t user;

    for (index = 0; index < CAPA_SNMP_USER_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_user_get(index, &user));

        if(osal_strlen(user.userName) != 0)
        {
            XMORE("Username:                %s\n", user.userName);
            XMORE("Password:                ********\n");
            XMORE("Privilege Mode:          %s \n", gaccess[user.access]);
            XMORE("Access GroupName:        %s \n", user.groupName);
            XMORE("Authentication Protocol: %s\n", gsslType[user.auth_protp]);
            XMORE("Encryption Protocol:     %s\n",  gsslType[user.encrypt_proto]);
            XMORE("Access SecLevel:         %s\n\n",  gsecLevel[user.selLevel]);

            count++;
        }
    }
    XMORE("\nTotal Entries: %d\n", count);

    return SYS_ERR_OK;
}

int32 cmd_snmp_engineid_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char engineId[BOND_SNMP_ENGINEID_LEN_MAX+1];
    sal_snmp_remoteEngine_t remote;
    int32 count = 0, index;

    osal_memset(engineId, 0, sizeof(engineId));
    SYS_ERR_CHK(sal_snmp_engineid_get(engineId));
    XMORE("Local SNMPV3 Engine id: %s \n\n", engineId);

    XMORE("     IP address                   Remote SNMP engineID               \n");
    XMORE("--------------------------------- -----------------------------------------------------------------\n") ;

    for (index = 0; index < CAPA_SNMP_REMOTE_ENGINEID_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_remoteEngine_get(index, &remote));

        if(osal_strlen(remote.server) != 0)
        {
            XMORE("%-33s %s \n", remote.server, remote.engineId);
            count++;
        }
    }
    XMORE("\nTotal Entries: %d\n\n", count);

    return SYS_ERR_OK;
}

int32 cmd_snmp_view_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char msk[CAPA_SNMP_VIEW_SUBTREE_LEN] ;
    uint8 viewType = 0;
    uint16 mskLen = 0;
    uint32 i = 0;
    sal_snmp_oid_t subtree;

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_VIEW_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    if (0 == osal_strcmp(argv[0], DFLT_SNMP_VIEW_STR))
    {
        return SYS_ERR_SNMP_VIEW_DEFAULT;
    }

    if (0 == osal_strcmp(argv[0], "none"))
    {
        return SYS_ERR_SNMP_VIEW_NAME;
    }

    SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[0], FALSE), SYS_ERR_SNMP_STR);

    osal_memset(msk, 0, sizeof(msk));
    subtree.len = 0;

    SYS_ERR_CHK(sal_util_str2oid(argv[1], &subtree.len, subtree.id));

    if ('a' != argv[2][0])
    {
        mskLen = osal_strlen(argv[2]);
        if (mskLen > subtree.len)
            return SYS_ERR_OID_MASK_LEN;

        if ('0' == argv[2][0])
            return SYS_ERR_OID_MASK;

        for (i = 0; i < mskLen; i++)
        {
            if((argv[2][i] != '0') && (argv[2][i] != '1'))
                return SYS_ERR_OID_MASK;
        }

        osal_memcpy(msk,argv[2],mskLen);
    }

    switch (argv[3][0])
    {
        case 'i':
            viewType = SNMP_VIEWTYPE_INCLUDE;
            break;

        case 'e':
            viewType = SNMP_VIEWTYPE_EXCLUDE;
            break;

        default:
            break;
    }

    SYS_ERR_CHK(sal_snmp_view_add(argv[0], &subtree, viewType,msk));

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_view_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sal_snmp_view_t view;
    sal_snmp_view_t cmdView;
    uint32 find = FALSE;

    if (0 == osal_strcmp(argv[0], DFLT_SNMP_VIEW_STR))
    {
        return SYS_ERR_SNMP_VIEW_DEFAULT;
    }

    if ('a' == argv[1][0])
    {
        for (i = 0; i < CAPA_SNMP_VIEW_NUM; i++)
        {
            osal_memset(&view, 0, sizeof(sal_snmp_view_t));
            SYS_ERR_CONTINUE(sal_snmp_view_get(i, &view));
            if ((osal_strlen(view.viewName) == osal_strlen(argv[0])) &&
               (0 == osal_memcmp(view.viewName, argv[0], osal_strlen(argv[0]))))
            {
                SYS_ERR_CONTINUE(sal_snmp_view_del(argv[0], &view.subtreeOid));
                find = TRUE;
            }
        }
    }
    else
    {
        SYS_ERR_CHK(sal_util_str2oid(argv[1], &(cmdView.subtreeOid.len), cmdView.subtreeOid.id));
        SYS_ERR_CHK(sal_snmp_viewIndex_get(argv[0], &cmdView.subtreeOid, &i));
        if (i < CAPA_SNMP_VIEW_NUM)
        {
            SYS_ERR_CHK(sal_snmp_view_del(argv[0], &cmdView.subtreeOid));
            find = TRUE;
        }
    }

    if (FALSE == find)
    {
        return SYS_ERR_ENTRY_NOT_FOUND;
    }

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_group_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 secModel = SNMP_SECMODEL_V1;

    switch(argv[1][1])
    {
        case '2':
            secModel = SNMP_SECMODEL_V2C;
            break;

        case 's':
            secModel = SNMP_SECMODEL_USM;
            break;

        case '1':
        default:
            secModel = SNMP_SECMODEL_V1;
            break;
    }

    SYS_ERR_CHK(sal_snmp_group_add(argv[0], secModel, argv[2]));

    return SYS_ERR_OK;
}

int32 cmd_snmp_group_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 secModel = SNMP_SECMODEL_V1;

    switch(argv[1][1])
    {
        case '2':
            secModel = SNMP_SECMODEL_V2C;
            break;

        case 's':
            secModel = SNMP_SECMODEL_USM;
            break;

        case '1' :
        default:
            secModel = SNMP_SECMODEL_V1;
            break;
    }

    SYS_ERR_CHK(sal_snmp_group_del(argv[0], secModel));

    return SYS_ERR_OK;
}

int32 cmd_snmp_access_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sal_snmp_access_t access;

    osal_memset(&access, 0 , sizeof(sal_snmp_access_t));

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_GROUP_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(argv[3]) > CAPA_SNMP_VIEW_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[0], FALSE), SYS_ERR_SNMP_STR);

    if (0 == osal_memcmp(argv[0], SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
        return SYS_ERR_SNMP_ACCESS_GROUP_NAME;

    osal_memcpy(access.groupName, argv[0], osal_strlen(argv[0]));
    osal_memcpy(access.readView, argv[3], osal_strlen(argv[3]));

    if (NULL != osal_strstr(cmd->string, "write-view"))
    {
        SYS_PARAM_CHK((osal_strlen(argv[4]) > CAPA_SNMP_VIEW_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
        osal_memcpy(access.writeView, argv[4], osal_strlen(argv[4]));
    }

    if (NULL != osal_strstr(cmd->string, "notify-view"))
    {
        if (argc == 6)
        {
            SYS_PARAM_CHK((osal_strlen(argv[5]) > CAPA_SNMP_VIEW_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
            osal_memcpy(access.notifyView, argv[5], osal_strlen(argv[5]));
        }
        else
        {
            SYS_PARAM_CHK((osal_strlen(argv[4]) > CAPA_SNMP_VIEW_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
            osal_memcpy(access.notifyView, argv[4], osal_strlen(argv[4]));
        }
    }

    access.context[0] = '\0'; 
    access.prefix = SNMP_PREFIX_EXACT;
    access.groupcfMode = SNMP_CONFIG_MANUAL;
    access.viewcfMode = SNMP_CONFIG_MANUAL;

    switch (argv[1][0])
    {
        case '1': 
            access.secMode = SNMP_SECMODEL_V1;
            break;

        case '2': 
            access.secMode = SNMP_SECMODEL_V2C;
            break;

        case '3' : 
            access.secMode = SNMP_SECMODEL_USM;
            break;

        default:
            break;
    }

    switch(argv[2][0])
    {
        case 'n':
            access.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
            break;

        case 'a':
           access.secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
            break;

        case 'p':
           access.secLevel = SNMP_SECLEVEL_AUTHPRIV;
            break;

        default:
            access.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
            break;
    }

    if (SNMP_SECMODEL_USM != access.secMode)
    {
        if(SNMP_SECLEVEL_NOAUTHNOPRIV != access.secLevel)
            return SYS_ERR_SECURITY_LEVEL;
    }

    SYS_ERR_CHK(sal_snmp_accessView_test(&access));
    SYS_ERR_CHK(sal_snmp_access_add(&access));

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_access_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char context[] = "";
    uint8 secMode = SNMP_SECMODEL_V1;
    uint8 secLevel;

    uint32 i;
    uint32 find = FALSE;
    sal_snmp_access_t  access;

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_GROUP_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    switch(argv[1][0])
    {
        case '1':
            secMode = SNMP_SECMODEL_V1;
            break;
        case '2':
            secMode = SNMP_SECMODEL_V2C;
            break;
        case '3':
            secMode = SNMP_SECMODEL_USM;
            break;
    }

    for (i = 0; i < SYS_SNMP_ACCESS_NUM_MAX; i++)
    {
        SYS_ERR_CONTINUE(sal_snmp_access_get(i, &access));
        if((0 == osal_strcmp(access.groupName, argv[0])) && secMode == access.secMode)
        {
             find = TRUE;
             secLevel = access.secLevel;

            #if 0 
            for (Idx = 0; Idx < SYS_SNMP_SECGROUP_NUM_MAX; Idx++)
            {
                osal_memset(&groupSec, 0,sizeof(sal_snmp_group_t));
                SYS_ERR_CONTINUE(sal_snmp_group_get(Idx,&groupSec))
                if (osal_strlen(groupSec.groupName) != 0 && groupSec.secModel == secMode)
                {
                    len = osal_strlen(groupSec.groupName) > osal_strlen(access.groupName) ? osal_strlen(groupSec.groupName): osal_strlen(access.groupName);
                    if (osal_memcmp(groupSec.groupName, access.groupName, len) == 0)
                    {
                        return SYS_ERR_ACCESS_GROUP_USED;
                    }
                }
            }
            #endif

             SYS_ERR_CHK(sal_snmp_access_del(argv[0], context, secMode, secLevel));
             break;
        }
    }

    if(FALSE == find)
    {
        return SYS_ERR_ENTRY_NOT_FOUND;
    }

   SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_accessv3_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char context[] = "";
    uint8 secMode = SNMP_SECMODEL_USM;
    uint8 secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;

    uint32 i;
    uint32 find = FALSE;
    sal_snmp_access_t  access;

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_GROUP_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    if (argc > 1)
    {
        switch(argv[1][0])
        {
            case 'n':
               secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
            break;

            case 'a':
                secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
            break;

            case 'p':
                secLevel = SNMP_SECLEVEL_AUTHPRIV;
            break;
        }
    }

    for (i = 0; i < SYS_SNMP_ACCESS_NUM_MAX; i++)
    {
        SYS_ERR_CONTINUE(sal_snmp_access_get(i, &access));
        if((0 == osal_strcmp(access.groupName, argv[0])) && secMode == access.secMode)
        {
            if (argc == 1)
            {
                SYS_ERR_CHK(sal_snmp_access_del(argv[0], context, secMode, access.secLevel));
                find = TRUE;
            }
            else
            {
                if (secLevel == access.secLevel)
                {
                    find = TRUE;
                    SYS_ERR_CHK(sal_snmp_access_del(argv[0], context, secMode, secLevel));
                    break;
                }
            }
        }
    }

    if(FALSE == find)
    {
        return SYS_ERR_ENTRY_NOT_FOUND;
    }

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_comView_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char context[] =  "";
    char groupName[CAPA_SNMP_GROUP_NAME_LEN +1] ;
    uint8 Access = SNMP_COMMUNITY_ACCESS_READ_ONLY;
    uint32 index = 0;
    uint32 exist_v1 = 0;
    uint32 exist_v2 = 0;
    uint32 access_v1 = 0;
    uint32 access_v2 = 0;
    sal_snmp_access_t access;
    sal_snmp_com2sec_t comEntry;

    osal_memset(&access, 0, sizeof(sal_snmp_access_t));
    osal_memset(&groupName, 0, sizeof(groupName));
    osal_memset(&comEntry, 0, sizeof(sal_snmp_com2sec_t));

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_COMMUNITY_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[0], FALSE), SYS_ERR_SNMP_STR);

    osal_strcpy(comEntry.secName, argv[0]);
    osal_strcpy(comEntry.comName, argv[0]);
    osal_strcpy(comEntry.sourceNet, "default");
    comEntry.mode = SNMP_COM_MODE_BASIC;

    SYS_ERR_CHK(sal_snmp_groupIndex_get(argv[0], SNMP_SECMODEL_V1, &index));
    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
        exist_v1 = 1;

    SYS_ERR_CHK(sal_snmp_groupIndex_get(argv[0], SNMP_SECMODEL_V2C, &index));
    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
        exist_v2 = 1;

    if (exist_v1 || exist_v2)
        return SYS_ERR_DUPLICATE_ENTRY;

    if (2 == argc)
    {
        if (osal_strlen(argv[0]) > CAPA_SNMP_COMMUNITY_LEN)
            return SYS_ERR_STR_LEN_EXCEED;

        switch(argv[1][1])
        {
            case 'o':
                Access = SNMP_COMMUNITY_ACCESS_READ_ONLY;
                break;

            case 'w':
                Access = SNMP_COMMUNITY_ACCESS_READ_WRITE;
                break;

            default:
                break;
        }

        comEntry.access = Access;

        access.context[0] = '\0';
        osal_sprintf(groupName, "%s%s", SNMP_ACCESS_GROUP_PREFIX, argv[0]);
        osal_strcpy(access.groupName, groupName);
        osal_strcpy(comEntry.groupName, groupName);
        access.prefix = SNMP_PREFIX_EXACT;
        access.secMode = SNMP_SECMODEL_V1;
        access.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
        access.viewcfMode = SNMP_CONFIG_AUTO;
        access.groupcfMode = SNMP_CONFIG_AUTO;

        osal_strcpy(access.readView, DFLT_SNMP_VIEW_STR);
        osal_strcpy(comEntry.viewName, DFLT_SNMP_VIEW_STR);
        if (SNMP_COMMUNITY_ACCESS_READ_WRITE == Access)
        {
            osal_strcpy(access.writeView, DFLT_SNMP_VIEW_STR);
        }
    }
    else if (3 == argc)
    {
        switch(argv[2][1])
        {
            case 'o':
                Access = SNMP_COMMUNITY_ACCESS_READ_ONLY;
                break;

            case 'w':
                Access = SNMP_COMMUNITY_ACCESS_READ_WRITE;
                break;

            default:
                break;
        }

        if (osal_strlen(argv[1]) > CAPA_SNMP_VIEW_NAME_LEN)
            return SYS_ERR_STR_LEN_EXCEED;

        osal_strcpy(comEntry.viewName, argv[1]);
        comEntry.access = Access;

        access.context[0] = '\0';
        osal_sprintf(groupName, "%s%s", SNMP_ACCESS_GROUP_PREFIX,argv[0]);
        osal_strcpy(access.groupName, groupName);
        osal_strcpy(comEntry.groupName, groupName);
        access.prefix = SNMP_PREFIX_EXACT;
        access.secMode = SNMP_SECMODEL_V1;   
        access.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
        access.viewcfMode = SNMP_CONFIG_MANUAL;
        access.groupcfMode = SNMP_CONFIG_AUTO;

        if (Access == SNMP_COMMUNITY_ACCESS_READ_ONLY)
        {
            osal_strcpy(access.readView, argv[1]);
        }
        else if (Access == SNMP_COMMUNITY_ACCESS_READ_WRITE)
        {
            osal_strcpy(access.readView, argv[1]);
            osal_strcpy(access.writeView, argv[1]);
        }
    }

    SYS_ERR_CHK(sal_snmp_accessIndex_get(groupName, context, SNMP_SECMODEL_V1, SNMP_SECLEVEL_NOAUTHNOPRIV, &index));
    if (index < SYS_SNMP_ACCESS_NUM_MAX)
    {
        access_v1 = 1;
    }
    SYS_ERR_CHK(sal_snmp_accessIndex_get(groupName, context, SNMP_SECMODEL_V2C, SNMP_SECLEVEL_NOAUTHNOPRIV, &index));
    if (index < SYS_SNMP_ACCESS_NUM_MAX)
    {
        access_v2 = 1;
    }

    if (access_v1 || access_v2)
        return SYS_ERR_SNMP_COMMUNITY_NAME;

    SYS_ERR_CHK(sal_snmp_accessView_test(&access));
    SYS_ERR_CHK(sal_snmp_basic_accessAdd_test(&access));

    SYS_ERR_CHK(sal_snmp_com2sec_add(&comEntry));

    SYS_ERR_CHK(sal_snmp_group_add(argv[0], SNMP_SECMODEL_V1, groupName));
    SYS_ERR_CHK(sal_snmp_group_add(argv[0], SNMP_SECMODEL_V2C, groupName));

    access.groupcfMode = SNMP_CONFIG_AUTO;
    if (2 == argc)
        access.viewcfMode = SNMP_CONFIG_AUTO;
    else
        access.viewcfMode = SNMP_CONFIG_MANUAL;
    SYS_ERR_CHK(sal_snmp_access_add(&access));

    access.secMode = SNMP_SECMODEL_V2C; 
    SYS_ERR_CHK(sal_snmp_access_add(&access));

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_comGroup_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 index = 0;
    uint32 exist_v1 = 0;
    uint32 exist_v2 = 0;
    uint32 access_v1 = 0;
    uint32 access_v2 = 0;
    sal_snmp_com2sec_t comEntry;
    char context[] = "";
    char groupName[CAPA_SNMP_GROUP_NAME_LEN+1];

    osal_memset(&comEntry, 0, sizeof(comEntry));
    osal_memset(groupName, 0, sizeof(groupName));

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_COMMUNITY_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(argv[1]) > CAPA_SNMP_GROUP_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[0], FALSE), SYS_ERR_SNMP_STR);

    osal_strcpy(comEntry.secName, argv[0]);
    osal_strcpy(comEntry.comName, argv[0]);
    osal_strcpy(comEntry.sourceNet, "default");
    comEntry.mode = SNMP_COM_MODE_ADV;
    osal_strcpy(comEntry.groupName, argv[1]);

    SYS_ERR_CHK(sal_snmp_groupIndex_get(argv[0], SNMP_SECMODEL_V1, &index));
    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
        exist_v1 = 1;

    SYS_ERR_CHK(sal_snmp_groupIndex_get(argv[0], SNMP_SECMODEL_V2C, &index));
    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
        exist_v2 = 1;

    if (exist_v1 || exist_v2)
        return SYS_ERR_DUPLICATE_ENTRY;

    osal_strcpy(groupName, argv[1]);
    SYS_ERR_CHK(sal_snmp_accessIndex_get(groupName, context, SNMP_SECMODEL_V1, SNMP_SECLEVEL_NOAUTHNOPRIV, &index));
    if (index < SYS_SNMP_ACCESS_NUM_MAX)
    {
        access_v1 = 1;
    }

    SYS_ERR_CHK(sal_snmp_accessIndex_get(groupName, context, SNMP_SECMODEL_V2C, SNMP_SECLEVEL_NOAUTHNOPRIV, &index));
    if (index < SYS_SNMP_ACCESS_NUM_MAX)
    {
        access_v2 = 1;
    }

    if (0 == access_v1 && 0 == access_v2)
        return SYS_ERR_GROUP_NOT_EXIST;

    SYS_ERR_CHK(sal_snmp_com2sec_add(&comEntry));
    SYS_ERR_CHK(sal_snmp_group_add(argv[0], SNMP_SECMODEL_V1, groupName));
    SYS_ERR_CHK(sal_snmp_group_add(argv[0], SNMP_SECMODEL_V2C, groupName));

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_com2sec_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char secName[CAPA_SNMP_SEC_NAME_LEN+1];
    char groupName[CAPA_SNMP_GROUP_NAME_LEN+1];
    char context[] = "";
    uint32 i = 0;
    uint32 index = 0;
    uint32 found = FALSE;
    sal_snmp_com2sec_t com2sec;
    sal_snmp_group_t group;
    sal_snmp_access_t access;
    uint8 exsit_v1 = 0, exsit_v2 = 0;

    osal_memset(secName, 0, sizeof(secName));
    osal_memset(groupName,0,sizeof(groupName));

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_COMMUNITY_LEN), SYS_ERR_STR_LEN_EXCEED);

    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        SYS_ERR_CONTINUE(sal_snmp_com2sec_get(i, &com2sec));

        if ((FALSE == found) && (osal_strlen(argv[0]) == osal_strlen(com2sec.comName)) &&
            (osal_memcmp(com2sec.comName, argv[0], osal_strlen(argv[0])) == 0))
        {
            found = TRUE;
            osal_strcpy(secName, argv[0]);
            SYS_ERR_CONTINUE(sal_snmp_com2sec_del(secName));

            break;
        }
    }

    if (FALSE == found)
    {
        return SYS_ERR_ENTRY_NOT_FOUND;
    }

    SYS_ERR_CHK(sal_snmp_groupIndex_get(secName,SNMP_SECMODEL_V1, &index));
    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
    {
        exsit_v1 = 1;
        SYS_ERR_CHK(sal_snmp_group_get(index, &group));
    }

    SYS_ERR_CHK(sal_snmp_groupIndex_get(secName,SNMP_SECMODEL_V2C, &index));
    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
    {
        exsit_v2 = 1;
        SYS_ERR_CHK(sal_snmp_group_get(index, &group));
    }

    if (0 == exsit_v1 && 0 == exsit_v2)
        return SYS_ERR_GROUP_NOT_EXIST;

    if (1 == exsit_v1)
        SYS_ERR_CHK(sal_snmp_group_del(secName, SNMP_SECMODEL_V1));

    if (1 == exsit_v2)
        SYS_ERR_CHK(sal_snmp_group_del(secName, SNMP_SECMODEL_V2C));

    if (0 != osal_strlen(group.groupName))
        osal_strcpy(groupName, group.groupName);

    SYS_ERR_CHK(sal_snmp_accessIndex_get(groupName, "", SNMP_SECMODEL_V1, SNMP_SECLEVEL_NOAUTHNOPRIV, &i));
    if (i < SYS_SNMP_ACCESS_NUM_MAX)
    {
        osal_memset(&access,0,sizeof(sal_snmp_access_t));
        SYS_ERR_CHK(sal_snmp_access_get(i, &access));
        if (SNMP_CONFIG_MANUAL != access.groupcfMode)
        {
             
            SYS_ERR_CHK(sal_snmp_access_del(groupName, context, access.secMode, SNMP_SECLEVEL_NOAUTHNOPRIV));
        }
    }

    SYS_ERR_CHK(sal_snmp_accessIndex_get(groupName, "", SNMP_SECMODEL_V2C, SNMP_SECLEVEL_NOAUTHNOPRIV, &i));
    if (i < SYS_SNMP_ACCESS_NUM_MAX)
    {
        osal_memset(&access,0,sizeof(sal_snmp_access_t));
        SYS_ERR_CHK(sal_snmp_access_get(i, &access));
        if (SNMP_CONFIG_MANUAL != access.groupcfMode)
        {
             
            SYS_ERR_CHK(sal_snmp_access_del(groupName, context, access.secMode, SNMP_SECLEVEL_NOAUTHNOPRIV));
        }
    }

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_authUser_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char context[] = "";
    uint32 index;
    uint32 temp;
    uint32 group_v3 = 0;
    sal_snmp_access_t access;
    sal_snmp_user_t user;

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_USER_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[0], FALSE), SYS_ERR_SNMP_STR);

    osal_memset(&user, 0, sizeof(sal_snmp_user_t));
    if (argc == 2)
    {
        user.selLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
        user.auth_protp = SNMP_SSL_TYPE_NONE;
        user.encrypt_proto = SNMP_SSL_TYPE_NONE;
    }
    else
    {
        user.selLevel = SNMP_SECLEVEL_AUTHNOPRIV;

        switch (argv[2][0])
        {
            case 's':
                user.auth_protp = SNMP_SSL_TYPE_SHA;
                break;

            case 'm':
            default:
                user.auth_protp = SNMP_SSL_TYPE_MD5;
                break;
         }

        temp = osal_strlen(argv[3]);

        if ((temp < BOND_SNMP_AUTH_KEY_LEN_MIN) || (temp > BOND_SNMP_AUTH_KEY_LEN_MAX))
            return SYS_ERR_SNMPv3_PASSWORD_ERR;

        SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[3], FALSE), SYS_ERR_SNMP_STR);

        osal_strcpy(user.authKey, argv[3] );
    }

    SYS_ERR_CHK(sal_snmp_groupIndex_get(argv[0], SNMP_SECMODEL_USM, &index));

    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
        group_v3 = 1;

    SYS_ERR_CHK(sal_snmp_accessIndex_get(argv[1], context, SNMP_SECMODEL_USM, user.selLevel, &index));

    if (SYS_SNMP_ACCESS_NUM_MAX == index)
        return SYS_ERR_GROUP_NOT_EXIST;

    SYS_ERR_CHK(sal_snmp_access_get(index, &access));
    if (SNMP_CONFIG_MANUAL != access.groupcfMode)
        return SYS_ERR_GROUP_NOT_EXIST;

    if (0 != osal_strlen(access.writeView))
        user.access = SNMP_COMMUNITY_ACCESS_READ_WRITE;
    else if (0 != osal_strlen(access.readView))
        user.access = SNMP_COMMUNITY_ACCESS_READ_ONLY;

    temp = osal_strlen(argv[0]);

    osal_strcpy(user.userName, argv[0]);
    osal_strcpy(user.groupName, argv[1]);

    if ((SNMP_SSL_TYPE_NONE == user.auth_protp) &&
        ((SNMP_SECLEVEL_AUTHNOPRIV == user.selLevel) || (SNMP_SECLEVEL_AUTHPRIV == user.selLevel)))
        return SYS_ERR_SNMPv3_AUTH_ERR;

    if ((SNMP_SSL_TYPE_NONE == user.encrypt_proto) && (SNMP_SECLEVEL_AUTHPRIV == user.selLevel))
        return SYS_ERR_SNMPv3_ENCRYPT_ERR;

    SYS_ERR_CHK(sal_snmp_user_add(&user));

    if (0 == group_v3)
        SYS_ERR_CHK(sal_snmp_group_add(argv[0], SNMP_SECMODEL_USM, argv[1]));

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_privUser_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char context[] = "";
    uint32 index;
    uint32 temp;
    uint32 group_v3 = 0;
    sal_snmp_access_t access;
    sal_snmp_user_t user;

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_USER_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[0], FALSE), SYS_ERR_SNMP_STR);

    osal_memset(&user, 0, sizeof(sal_snmp_user_t));
    user.selLevel = SNMP_SECLEVEL_AUTHPRIV;
    user.encrypt_proto = SNMP_SSL_TYPE_DES;

    switch (argv[2][0])
    {
        case 's':
            user.auth_protp = SNMP_SSL_TYPE_SHA;
            break;

        case 'm':
        default:
            user.auth_protp = SNMP_SSL_TYPE_MD5;
            break;
    }

    if (SNMP_SSL_TYPE_NONE != user.auth_protp)
    {
        temp = osal_strlen(argv[3]);

        if ((temp < BOND_SNMP_AUTH_KEY_LEN_MIN) || (temp > BOND_SNMP_AUTH_KEY_LEN_MAX))
            return SYS_ERR_SNMPv3_PASSWORD_ERR;

        SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[3], FALSE), SYS_ERR_SNMP_STR);

        osal_strcpy(user.authKey, argv[3] );
    }
    else
    {
        if (0 != osal_memcmp(argv[3] ,"" ,sizeof("")))
            return SYS_ERR_SNMPv3_PASSWORD_ERR;
    }

    if (SNMP_SSL_TYPE_NONE != user.encrypt_proto)
    {
        temp = osal_strlen(argv[4]);

         if ((temp < BOND_SNMP_PRIV_KEY_LEN_MIN) || (temp > BOND_SNMP_PRIV_KEY_LEN_MAX))
            return SYS_ERR_SNMPv3_ENCRYPT_ERR;

        SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[4], FALSE), SYS_ERR_SNMP_STR);
        osal_strcpy(user.privKey, argv[4] );
    }
    else
    {
        if (0 != osal_memcmp(argv[4] ,"" ,sizeof("")))
            return SYS_ERR_SNMPv3_ENCRYPT_ERR;
    }

    SYS_ERR_CHK(sal_snmp_groupIndex_get(argv[0], SNMP_SECMODEL_USM, &index));

    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
        group_v3 = 1;

    SYS_ERR_CHK(sal_snmp_accessIndex_get(argv[1], context, SNMP_SECMODEL_USM, user.selLevel, &index));

    if (SYS_SNMP_ACCESS_NUM_MAX == index)
        return SYS_ERR_GROUP_NOT_EXIST;

    SYS_ERR_CHK(sal_snmp_access_get(index, &access));
    if (SNMP_CONFIG_MANUAL != access.groupcfMode)
        return SYS_ERR_GROUP_NOT_EXIST;

    if (0 != osal_strlen(access.writeView))
        user.access = SNMP_COMMUNITY_ACCESS_READ_WRITE;
    else if (0 != osal_strlen(access.readView))
        user.access = SNMP_COMMUNITY_ACCESS_READ_ONLY;

    temp = osal_strlen(argv[0]);

    osal_strcpy(user.userName, argv[0]);
    osal_strcpy(user.groupName, argv[1]);

    if ((SNMP_SSL_TYPE_NONE == user.auth_protp) &&
        ((SNMP_SECLEVEL_AUTHNOPRIV == user.selLevel) || (SNMP_SECLEVEL_AUTHPRIV == user.selLevel)))
        return SYS_ERR_SNMPv3_AUTH_ERR;

    if ((SNMP_SSL_TYPE_NONE == user.encrypt_proto) && (SNMP_SECLEVEL_AUTHPRIV == user.selLevel))
        return SYS_ERR_SNMPv3_ENCRYPT_ERR;

    SYS_ERR_CHK(sal_snmp_user_add(&user));

    if (0 == group_v3)
        SYS_ERR_CHK(sal_snmp_group_add(argv[0], SNMP_SECMODEL_USM, argv[1]));

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_user_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 index;

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_USER_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_user_del(argv[0]));

    SYS_ERR_CHK(sal_snmp_groupIndex_get(argv[0],SNMP_SECMODEL_USM, &index));
    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
    {
        SYS_ERR_CHK(sal_snmp_group_del(argv[0], SNMP_SECMODEL_USM));
    }

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_engineid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char engineStr[BOND_SNMP_ENGINEID_LEN_MAX + 1];

    if('d' == argv[0][0])
    {
        osal_memset(engineStr, 0, sizeof(engineStr));
        SYS_ERR_CHK(sal_snmp_defaultEngineId_get(engineStr));
        SYS_ERR_CHK(sal_snmp_engineid_set(engineStr));
        
    }
    else
    {
        if (SYS_ERR_OK != sal_util_check_hex(argv[0]))
            return SYS_ERR_ENGINEID_ERR;

        if (osal_strlen(argv[0]) % 2)
            return SYS_ERR_ENGINEID_ERR;

        SYS_ERR_CHK(sal_snmp_engineid_set(argv[0]));
    }

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_remoteEngineid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sal_snmp_remoteEngine_t remote;

    if (osal_strlen(argv[0]) > CAPA_HOSTNAME_LEN)
        return SYS_ERR_STR_LEN_EXCEED;

    if (IS_IPV4_UADDRSTR_INVALID(argv[0]))
    {
        if IS_IPV6_UADDRSTR_INVALID(argv[0])
        {
            if (IS_HOSTNAME_INVALID(argv[0]))
                return SYS_ERR_HOSTNAME;
        }
    }

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_snmp_remoteEngine_del(argv[0]));
    }
    else
    {
        if ((osal_strlen(argv[1]) < BOND_SNMP_ENGINEID_LEN_MIN) ||(osal_strlen(argv[1]) > BOND_SNMP_ENGINEID_LEN_MAX))
            return SYS_ERR_ENGINEID_ERR;

        if (SYS_ERR_OK != sal_util_check_hex(argv[1]))
            return SYS_ERR_ENGINEID_ERR;

        osal_memset(&remote, 0, sizeof(sal_snmp_remoteEngine_t));

        osal_memcpy(remote.server, argv[0], osal_strlen(argv[0]));
        osal_memcpy(remote.engineId, argv[1], osal_strlen(argv[1]));

        SYS_ERR_CHK(sal_snmp_remoteEngine_add(&remote));
    }

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_hostv3_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sal_snmp_target_t  hostEntry;
    sal_snmp_secmode_t  secModel = SNMP_SECMODEL_USM;
    sal_snmp_seclevel_t  secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;

    uint16  destPort = DFLT_SNMP_NOTIFACTION_PORT;
    uint16  timeOut = DFLT_SNMP_INFORM_TIMEOUT;
    uint16  retries = DFLT_SNMP_INFORM_RETRY;
    uint8 isTrap = TRUE;
    uint8 isArgTrap = FALSE;
    uint8 isArgVersion = FALSE;
    uint8 isArgPort = FALSE;
    uint8 isArgTime = FALSE;
    uint8 isArgRetry = FALSE;

    osal_memset(&hostEntry, 0, sizeof(sal_snmp_target_t));
    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_HOSTNAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    if (IS_IPV4_UADDRSTR_INVALID(argv[0]))
    {
        if IS_IPV6_UADDRSTR_INVALID(argv[0])
        {
            if (IS_HOSTNAME_INVALID(argv[0]))
                return SYS_ERR_HOSTNAME;
        }
    }

    if (argc == 2)
    {
        SYS_PARAM_CHK((osal_strlen(argv[1]) > CAPA_SNMP_USER_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
        osal_memcpy(hostEntry.server, argv[0], osal_strlen(argv[0]));
        osal_memcpy(hostEntry.name, argv[1], osal_strlen(argv[1]));
        hostEntry.secModel = secModel;
        hostEntry.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
        hostEntry.portNum = destPort;
        hostEntry.retries = DFLT_SNMP_INFORM_RETRY;
        hostEntry.timeout = DFLT_SNMP_INFORM_TIMEOUT;
        hostEntry.trapType = SNMP_TRAP_V3;
        SYS_ERR_CHK(sal_snmp_target_add(&hostEntry));
    }
    else
    {
        if ((NULL != osal_strstr(cmd->string, "informs")) || (NULL != osal_strstr(cmd->string, "traps")))
        {
            isArgTrap = TRUE;

            if (NULL != osal_strstr(cmd->string, "informs"))
                isTrap = FALSE;
        }

        if ((NULL != osal_strstr(cmd->string, "auth")) ||(NULL != osal_strstr(cmd->string, "noauth")) || (NULL != osal_strstr(cmd->string, "priv")) )
        {
            isArgVersion = TRUE;
        }

        if (NULL != osal_strstr(cmd->string, "udp-port"))
        {
            isArgPort = TRUE;
        }

        if (NULL != osal_strstr(cmd->string, "timeout"))
        {
            isArgTime = TRUE;
        }

        if (NULL != osal_strstr(cmd->string, "retries"))
        {
            isArgRetry = TRUE;
        }

        if (argc == 3)
        {
            if (isArgVersion)
            {
                switch(argv[1][0])
                {
                    case 'a':
                        secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                        break;
                    case 'p':
                        secLevel = SNMP_SECLEVEL_AUTHPRIV;
                        break;
                    case 'n':
                    default :
                        secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
                        break;
                }
            }

            if (isArgPort)
            {
                destPort = (uint16)SYS_STR2UINT(argv[2]);
            }

            if (isArgTime)
            {
                timeOut = (uint16)SYS_STR2UINT(argv[2]);
            }

            if (isArgRetry)
            {
                retries = (uint16)SYS_STR2UINT(argv[2]);
            }
        }
        else if (argc == 4)
        {

            if (isArgVersion)
            {
                if (isArgTrap)
                {
                    switch(argv[2][0])
                    {
                        case 'a':
                            secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                            break;
                        case 'p':
                            secLevel = SNMP_SECLEVEL_AUTHPRIV;
                            break;
                        case 'n':
                        default :
                            secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
                            break;
                    }
                }
                else
                {
                    switch(argv[1][0])
                    {
                        case 'a':
                            secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                            break;
                        case 'p':
                            secLevel = SNMP_SECLEVEL_AUTHPRIV;
                            break;
                        case 'n':
                        default :
                            secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
                            break;
                    }
                 }
            }

           if (isArgPort)
           {
                if (isArgTrap || isArgVersion)
                    destPort = (uint16)SYS_STR2UINT(argv[3]);
                else
                    destPort = (uint16)SYS_STR2UINT(argv[2]);
           }

           if (isArgTime)
           {
                if (isArgTrap || isArgVersion || isArgPort)
                    timeOut = (uint16)SYS_STR2UINT(argv[3]);
                else
                    timeOut = (uint16)SYS_STR2UINT(argv[2]);
           }

          if (isArgRetry)
          {
                retries = (uint16)SYS_STR2UINT(argv[3]);
          }
        }
        else if (argc == 5)
        {
            if (isArgVersion)
            {
                if (isArgTrap)
                {
                    switch(argv[2][0])
                    {
                        case 'a':
                            secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                            break;
                        case 'p':
                            secLevel = SNMP_SECLEVEL_AUTHPRIV;
                            break;
                        case 'n':
                        default :
                            secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
                            break;
                    }
                }
                else
                {
                    switch(argv[1][0])
                    {
                        case 'a':
                            secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                            break;
                        case 'p':
                            secLevel = SNMP_SECLEVEL_AUTHPRIV;
                            break;
                        case 'n':
                        default :
                            secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
                            break;
                    }
                 }
            }

            if (isArgPort)
            {
                if (isArgTrap && isArgVersion)
                {
                    destPort = (uint16)SYS_STR2UINT(argv[4]);
                }
                else if (isArgTrap || isArgVersion)
                {
                    destPort = (uint16)SYS_STR2UINT(argv[3]);
                }
                else
                {
                    destPort = (uint16)SYS_STR2UINT(argv[2]);
                }
            }

            if (isArgTime)
            {
                if ((isArgTrap && isArgVersion) || (isArgTrap && isArgPort) || (isArgPort && isArgPort))
                {
                    timeOut = (uint16)SYS_STR2UINT(argv[4]);
                }
                else
                {
                    timeOut = (uint16)SYS_STR2UINT(argv[3]);
                }
            }

            if (isArgRetry)
            {
                retries = (uint16)SYS_STR2UINT(argv[4]);
            }
        }
        else if (argc == 6)
        {
            if (!isArgTrap)
            {
                  switch(argv[1][0])
                  {
                        case 'a':
                            secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                            break;
                        case 'p':
                            secLevel = SNMP_SECLEVEL_AUTHPRIV;
                            break;
                        case 'n':
                        default :
                            secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
                            break;
                  }

                 destPort = (uint16)SYS_STR2UINT(argv[3]);
                 timeOut = (uint16)SYS_STR2UINT(argv[4]);
                 retries = (uint16)SYS_STR2UINT(argv[5]);
            }
            else
            {
                  switch(argv[2][0])
                  {
                        case 'a':
                            secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                            break;
                        case 'p':
                            secLevel = SNMP_SECLEVEL_AUTHPRIV;
                            break;
                        case 'n':
                        default :
                            secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
                            break;
                  }
            }

            if (!isArgVersion)
            {
              destPort = (uint16)SYS_STR2UINT(argv[3]);
              timeOut = (uint16)SYS_STR2UINT(argv[4]);
              retries = (uint16)SYS_STR2UINT(argv[5]);
            }

            if (!isArgPort)
            {
              timeOut = (uint16)SYS_STR2UINT(argv[4]);
              retries = (uint16)SYS_STR2UINT(argv[5]);
            }

            if (!isArgTime)
            {
              destPort = (uint16)SYS_STR2UINT(argv[4]);
              retries = (uint16)SYS_STR2UINT(argv[5]);
            }

            if (!isArgRetry)
            {
              destPort = (uint16)SYS_STR2UINT(argv[4]);
              timeOut = (uint16)SYS_STR2UINT(argv[5]);
            }
        }
        else
        {
            switch(argv[2][0])
            {
                case 'a':
                    secLevel = SNMP_SECLEVEL_AUTHNOPRIV;
                    break;
                case 'p':
                    secLevel = SNMP_SECLEVEL_AUTHPRIV;
                    break;
                case 'n':
                default :
                    secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
                    break;
            }
           destPort = (uint16)SYS_STR2UINT(argv[4]);
           timeOut = (uint16)SYS_STR2UINT(argv[5]);
           retries = (uint16)SYS_STR2UINT(argv[6]);
        }

        if (isTrap && isArgTime)
        {
            XMORE("Notifaction type is trap , don't need input timeout!\n");
            return SYS_ERR_OK;
        }

        if (isTrap && isArgRetry)
        {
            XMORE("Notifaction type is trap , don't need input retries!\n");
            return SYS_ERR_OK;
        }

        osal_memcpy(hostEntry.server, argv[0], osal_strlen(argv[0]));
        if (isArgTrap && isArgVersion)
        {
            SYS_PARAM_CHK((osal_strlen(argv[3]) > CAPA_SNMP_USER_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
            osal_memcpy(hostEntry.name, argv[3], osal_strlen(argv[3]));
        }
        else if(isArgTrap || isArgVersion)
        {
            SYS_PARAM_CHK((osal_strlen(argv[2]) > CAPA_SNMP_USER_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
            osal_memcpy(hostEntry.name, argv[2], osal_strlen(argv[2]));
        }
        else
        {
            SYS_PARAM_CHK((osal_strlen(argv[1]) > CAPA_SNMP_USER_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
            osal_memcpy(hostEntry.name, argv[1], osal_strlen(argv[1]));
        }

        if (isTrap)
            hostEntry.trapType = SNMP_TRAP_V3;
        else
            hostEntry.trapType = SNMP_INFORM_V3;

        hostEntry.secModel = secModel;
        hostEntry.secLevel = secLevel;
        hostEntry.portNum = destPort;
        hostEntry.retries = retries;
        hostEntry.timeout = timeOut;

       SYS_ERR_CHK(sal_snmp_target_add(&hostEntry));
    }

    SYS_ERR_CHK(cmd_snmp_demo_restart());

    return SYS_ERR_OK;
}

int32 cmd_snmp_restart(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    return sal_snmp_restart();
}

int32 cmd_snmp_config_reload_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sal_snmp_reload_t relaod;

    switch(argv[0][0])
    {
        case 'a':
            relaod = SNMP_RELOAD_AUTO;
            break;

        case 'm':
        default:
            relaod = SNMP_RELOAD_MANUAL;
           break;
    }

    SYS_ERR_CHK(sal_snmp_config_reload_set(relaod));

    return SYS_ERR_OK;
}
#else

int32 cmd_snmp_community_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 access = 0;

    SYS_PARAM_CHK((osal_strlen(argv[0]) > CAPA_SNMP_COMMUNITY_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK(SNMP_STRING_VALIDATE(argv[0], FALSE), SYS_ERR_SNMP_STR);

    switch (argv[1][1])
    {
        case 'o': 
            access = SNMP_COMMUNITY_ACCESS_READ_ONLY;
            break;

        case 'w': 
        default:
            access = SNMP_COMMUNITY_ACCESS_READ_WRITE;
            break;
    }

    SYS_ERR_CHK(sal_snmp_community_add(argv[0], access));

    if (CMD_IS_CLI_RUN())
    {
        SYS_ERR_CHK(sal_snmp_restart());
    }

    return SYS_ERR_OK;
}

int32 cmd_snmp_community_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_snmp_community_del(argv[0]));
    if (CMD_IS_CLI_RUN())
    {
        SYS_ERR_CHK(sal_snmp_restart());
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_SNMPV3
static int32 _cmd_snmp_v3_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char tmpStr[SYS_BUF32_LEN];
    char oidStr[CAPA_SNMP_OID_STR_LEN];
    char engineId[BOND_SNMP_ENGINEID_LEN_MAX + 1];
    char defaulEngineIdStr[BOND_SNMP_ENGINEID_LEN_MAX + 1];
    uint32 i;
    sal_snmp_com2sec_t com2sec;
    sal_snmp_view_t dfltView;
    sal_snmp_view_t view;
    sal_snmp_access_t dfltAccess;
    sal_snmp_access_t access;
    sal_snmp_user_t user;
    sal_snmp_remoteEngine_t remote;
    sal_snmp_reload_t reload;
    uint8 isDefaultComExist = FALSE;

    SYS_MEM_CLEAR(dfltView);
    osal_memcpy(dfltView.viewName, DFLT_SNMP_VIEW_STR, sizeof(DFLT_SNMP_VIEW_STR));
    dfltView.viewType = DFLT_SNMP_VIEWTYPE;
    SYS_ERR_CHK(sal_util_str2oid(DFLT_SNMP_VIEW_SUBTREE_STR, &dfltView.subtreeOid.len, dfltView.subtreeOid.id));

    for (i = 0; i < CAPA_SNMP_VIEW_NUM; i++)
    {
        osal_memset(&view, 0, sizeof(sal_snmp_view_t));
        SYS_ERR_CHK(sal_snmp_view_get(i, &view));

        if (0 != osal_strlen(view.viewName))
        {
            if ((0 == osal_strcmp(view.viewName, dfltView.viewName))
                  && (view.subtreeOid.len == dfltView.subtreeOid.len)
                  && (0 == osal_memcmp(&view.subtreeOid.id, &dfltView.subtreeOid.id, sizeof(uint32)* view.subtreeOid.len )))
                continue;

            SYS_MEM_CLEAR(cmd);
            SYS_ERR_CHK(sal_util_oid2str(view.subtreeOid.len, view.subtreeOid.id, oidStr));
            osal_sprintf(cmd, "snmp view \"%s\" subtree %s oid-mask %s viewtype %s\n",
                            view.viewName, oidStr+1,
                            (osal_strlen(view.viewMaskStr) == 0) ? "all" : view.viewMaskStr,
                            gviewTypeStr[view.viewType]);

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

#if 0
    
    for (i = 0; i < SYS_SNMP_SECGROUP_NUM_MAX; i++)
    {
        SYS_ERR_CONTINUE(sal_snmp_group_get(i, &group));

        if (0 != osal_strlen(group.groupName))
        {
            
            if ((0 == osal_strcmp(DFLT_SNMP_COMMUNITY, group.secName))
                    && (SNMP_SECMODEL_V1 == group.secModel || SNMP_SECMODEL_V2C == group.secModel)
                    && (0 == osal_strcmp("readgroup", group.groupName)))
            {
                continue;
            }
            else if ((0 == osal_strcmp(DFLT_SNMP_COMMUNITY_RW_STR, group.secName))
                        && (SNMP_SECMODEL_V1 == group.secModel || SNMP_SECMODEL_V2C == group.secModel)
                        && (0 == osal_strcmp("writegroup", group.groupName)))
            {
                continue;
            }

            osal_sprintf(cmd, "snmp security group %s %s security %s\n",
                                                    group.groupName,
                                                    gsecModel[group.secModel],
                                                    group.secName);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }
#endif

    SYS_MEM_CLEAR(dfltAccess);
    dfltAccess.secMode = SNMP_SECMODEL_V1;
    dfltAccess.secLevel = SNMP_SECLEVEL_NOAUTHNOPRIV;
    dfltAccess.prefix = SNMP_PREFIX_EXACT;
    osal_strcpy(dfltAccess.readView, DFLT_SNMP_VIEW_STR);
    dfltAccess.viewcfMode = SNMP_CONFIG_MANUAL;
    dfltAccess.groupcfMode = SNMP_CONFIG_MANUAL;

    for (i = 0; i < SYS_SNMP_ACCESS_NUM_MAX; i++)
    {
        SYS_ERR_CONTINUE(sal_snmp_access_get(i, &access));

        if (0 != osal_strlen(access.groupName))
        {
#if 0
            if ((0 == osal_strcmp("readgroup", access.groupName))
                    && access.secMode != SNMP_SECMODEL_USM)
            {
                osal_strcpy(dfltAccess.groupName, access.groupName);

                osal_strcpy(dfltAccess.readView, DFLT_SNMP_VIEW_STR);
                osal_strcpy(dfltAccess.writeView, "");

                dfltAccess.secMode = access.secMode;

                if (0 == osal_memcmp(&dfltAccess, &access, sizeof(sal_snmp_access_t)))
                    continue;
            }
            else if ((0 == osal_strcmp("writegroup", access.groupName))
                    && access.secMode != SNMP_SECMODEL_USM)
            {
                osal_strcpy(dfltAccess.groupName, access.groupName);

                osal_strcpy(dfltAccess.readView, DFLT_SNMP_VIEW_STR);
                osal_strcpy(dfltAccess.writeView, DFLT_SNMP_VIEW_STR);

                dfltAccess.secMode = access.secMode;

                if (0 == osal_memcmp(&dfltAccess, &access, sizeof(sal_snmp_access_t)))
                    continue;
            }
#endif
            if (SNMP_CONFIG_MANUAL ==  access.groupcfMode)
            {
                SYS_MEM_CLEAR(cmd);

                osal_sprintf(cmd, "snmp group \"%s\" version %s %s read-view \"%s\" ",
                                access.groupName,
                                gsecCfModel[access.secMode],
                                gsecLevel[access.secLevel],
                                access.readView);

                if (0 != osal_strlen(access.writeView))
                {
                     SYS_MEM_CLEAR(tmpStr);
                     osal_sprintf(tmpStr, "write-view \"%s\" ", access.writeView);
                     osal_strcat(cmd, tmpStr);
                }

                if (0 != osal_strlen(access.notifyView))
                {
                     SYS_MEM_CLEAR(tmpStr);
                     osal_sprintf(tmpStr, "notify-view \"%s\" ", access.notifyView);
                     osal_strcat(cmd, tmpStr);
                }
                osal_strcat(cmd, "\n");
                CMD_WRITE_TO_FILE(fp, cmd);
            }
        }
    }

    for (i = 0; i < CAPA_SNMP_USER_NUM; i++)
    {
        SYS_ERR_CONTINUE(sal_snmp_user_get(i, &user));

        if (0 != osal_strlen(user.userName))
        {
            SYS_MEM_CLEAR(cmd);
            if (SNMP_SECLEVEL_NOAUTHNOPRIV == user.selLevel)
            {
                osal_sprintf(cmd, "snmp user \"%s\" \"%s\" \n", user.userName, user.groupName);
            }
            else if (SNMP_SECLEVEL_AUTHNOPRIV == user.selLevel)
            {
                osal_sprintf(cmd, "snmp user \"%s\" \"%s\" auth %s \"%s\" \n",
                                                                     user.userName,
                                                                     user.groupName,
                                                                     gsslType[user.auth_protp],
                                                                     user.authKey);
            }
            else if (SNMP_SECLEVEL_AUTHPRIV == user.selLevel)
            {
                osal_sprintf(cmd, "snmp user \"%s\" \"%s\" auth %s \"%s\" priv \"%s\" \n",
                                                                     user.userName,
                                                                     user.groupName,
                                                                     gsslType[user.auth_protp],
                                                                     user.authKey,
                                                                     user.privKey);
            }
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        osal_memset(&com2sec, 0, sizeof(sal_snmp_com2sec_t));
        SYS_ERR_CONTINUE(sal_snmp_com2sec_get(i, &com2sec));

        if (0 != osal_strlen(com2sec.comName))
        {
            if ((0 == osal_strcmp(com2sec.comName, DFLT_SNMP_COMMUNITY)) && (DFLT_SNMP_COMMUNITY_ACCESS == com2sec.access)
                 && (SNMP_COM_MODE_BASIC == com2sec.mode))
            {
                isDefaultComExist = TRUE;
                break;
            }
        }
   }
    if (!isDefaultComExist)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "no snmp community \"%s\"\n", DFLT_SNMP_COMMUNITY);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        osal_memset(&com2sec, 0, sizeof(sal_snmp_com2sec_t));
        SYS_ERR_CONTINUE(sal_snmp_com2sec_get(i, &com2sec));

        if (0 != osal_strlen(com2sec.comName))
        {
            if (SNMP_COM_MODE_BASIC == com2sec.mode)
            {
                if ((0 == osal_strcmp(com2sec.comName, DFLT_SNMP_COMMUNITY)) && (DFLT_SNMP_COMMUNITY_ACCESS == com2sec.access)
                      && (0 == osal_strcmp(com2sec.viewName, DFLT_SNMP_VIEW_STR)))
                {
                    continue;
                }

                if (0 == osal_strcmp(com2sec.viewName, DFLT_SNMP_VIEW_STR))
                {
                    osal_sprintf(cmd, "snmp community \"%s\"  %s\n", com2sec.comName, gaccess[com2sec.access]);
                }
                else
                {
                    osal_sprintf(cmd, "snmp community \"%s\" view \"%s\" %s\n", com2sec.comName, com2sec.viewName, gaccess[com2sec.access]);
                }
            }
            else
            {
                osal_sprintf(cmd, "snmp community \"%s\" group \"%s\" \n", com2sec.comName, com2sec.groupName);
            }
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    SYS_MEM_CLEAR(engineId);
    SYS_MEM_CLEAR(defaulEngineIdStr);

    SYS_ERR_CHK(sal_snmp_engineid_get(engineId));
    SYS_ERR_CHK(sal_snmp_defaultEngineId_get(defaulEngineIdStr));

    if (0 != osal_strcmp(defaulEngineIdStr, engineId))
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "snmp engineid %s\n",  engineId);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    for (i = 0; i < CAPA_SNMP_REMOTE_ENGINEID_NUM; i++)
    {
        osal_memset(&remote, 0, sizeof(sal_snmp_remoteEngine_t));
        SYS_ERR_CONTINUE(sal_snmp_remoteEngine_get(i, &remote));
        if (0 != osal_strlen(remote.server))
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "snmp engineid remote %s %s \n",  remote.server, remote.engineId);

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    SYS_ERR_CHK(sal_snmp_config_reload_get(&reload));

    if (SNMP_RELOAD_AUTO != reload) 
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "snmp conf-reload %s\n", (reload == SNMP_RELOAD_AUTO) ? "auto" : "manual");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;

}
#endif

int32 cmd_snmp_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char tmpStr[SYS_BUF32_LEN];
    uint32 i = 0;
    sys_enable_t enable;
    sal_snmp_target_t host;

#ifdef CONFIG_SYS_MIB_TRAP
    
    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &enable));

    if (DFLT_SNMP_TRAP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%ssnmp trap auth\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_LINKUPDOWN, &enable));

    if (DFLT_SNMP_TRAP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%ssnmp trap linkUpDown\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_WARM, &enable));

    if (DFLT_SNMP_TRAP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%ssnmp trap warm-start\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_COOL, &enable));

    if (DFLT_SNMP_TRAP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%ssnmp trap cold-start\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_PORT_SEC, &enable));

    if (DFLT_SNMP_TRAP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%ssnmp trap port-security\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_SNMPV3
    
    SYS_ERR_CHK(_cmd_snmp_v3_save(fp));
#else
    
    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        sal_snmp_community_t community;

        SYS_MEM_CLEAR(community);

        SYS_ERR_CONTINUE(sal_snmp_community_get(i, &community));

        if (0 != osal_strlen(community.name))
        {
            if (SNMP_COMMUNITY_ACCESS_READ_ONLY == community.access)
                osal_sprintf(cmd, "snmp community \"%s\" ro\n", community.name);
            else if (SNMP_COMMUNITY_ACCESS_READ_WRITE == community.access)
                osal_sprintf(cmd, "snmp community \"%s\" rw\n", community.name);

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }
#endif

    for (i = 0; i < CAPA_SNMP_HOST_NUM; i++)
    {
        SYS_MEM_CLEAR(host);

        SYS_ERR_CHK(sal_snmp_target_get(i, &host));

        if (0 != osal_strlen(host.name))
        {
            if (SNMP_SECMODEL_V1 == host.secModel)
            {
                if (DFLT_SNMP_NOTIFACTION_PORT ==  host.portNum)
                    osal_sprintf(cmd, "snmp host %s \"%s\" \n", host.server, host.name);
                else
                    osal_sprintf(cmd, "snmp host %s \"%s\" udp-port %d\n", host.server, host.name, host.portNum);
            }
            else if (SNMP_SECMODEL_V2C == host.secModel)
            {
                if (SNMP_TRAP_V2 == host.trapType)
                {
                    if (DFLT_SNMP_NOTIFACTION_PORT ==  host.portNum)
                        osal_sprintf(cmd, "snmp host %s version %s \"%s\" \n",  host.server, "2c", host.name);
                    else
                        osal_sprintf(cmd, "snmp host %s traps version %s \"%s\" udp-port %d\n",  host.server, "2c", host.name, host.portNum);
                }
                else
                {
                    osal_sprintf(cmd, "snmp host %s informs version %s \"%s\" ",  host.server, "2c", host.name);
                    if (DFLT_SNMP_NOTIFACTION_PORT !=  host.portNum)
                    {
                        SYS_MEM_CLEAR(tmpStr);
                        osal_sprintf(tmpStr, "udp-port %d ", host.portNum);
                        osal_strcat(cmd, tmpStr);
                    }
                    if (DFLT_SNMP_INFORM_TIMEOUT !=  host.timeout)
                    {
                        SYS_MEM_CLEAR(tmpStr);
                        osal_sprintf(tmpStr, "timeout %d ", host.timeout);
                        osal_strcat(cmd, tmpStr);
                    }
                    if (DFLT_SNMP_INFORM_RETRY !=  host.retries)
                    {
                        SYS_MEM_CLEAR(tmpStr);
                        osal_sprintf(tmpStr, "retries %d ", host.retries);
                        osal_strcat(cmd, tmpStr);
                    }
                    osal_strcat(cmd, "\n");
                }
            }
            else if (SNMP_SECMODEL_USM == host.secModel)
            {
                if (SNMP_TRAP_V3 == host.trapType)
                {
                    osal_sprintf(cmd, "snmp host %s traps version 3 %s \"%s\" ", host.server, gsecLevel[host.secLevel], host.name);
                    if (DFLT_SNMP_NOTIFACTION_PORT !=  host.portNum)
                    {
                        SYS_MEM_CLEAR(tmpStr);
                        osal_sprintf(tmpStr, "udp-port %d ", host.portNum);
                        osal_strcat(cmd, tmpStr);
                    }
                }
                else
                {
                    osal_sprintf(cmd, "snmp host %s informs version 3 %s \"%s\" ",  host.server, gsecLevel[host.secLevel], host.name);
                    if (DFLT_SNMP_NOTIFACTION_PORT !=  host.portNum)
                    {
                        SYS_MEM_CLEAR(tmpStr);
                        osal_sprintf(tmpStr, "udp-port %d ", host.portNum);
                        osal_strcat(cmd, tmpStr);
                    }
                    if (DFLT_SNMP_INFORM_TIMEOUT !=  host.timeout)
                    {
                        SYS_MEM_CLEAR(tmpStr);
                        osal_sprintf(tmpStr, "timeout %d ", host.timeout);
                        osal_strcat(cmd, tmpStr);
                    }
                    if (DFLT_SNMP_INFORM_RETRY !=  host.retries)
                    {
                        SYS_MEM_CLEAR(tmpStr);
                        osal_sprintf(tmpStr, "retries %d ", host.retries);
                        osal_strcat(cmd, tmpStr);
                    }
                }
                osal_strcat(cmd, "\n");
            }
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DFLT_SNMP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%ssnmp\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

