
#include <libsal/sal_isg.h>
#include <libcmd/cmd.h>

static int32 _show_table(sys_isg_entry_type_t filterType)
{
    char portStr[CAPA_PORT_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    char ipStr[CAPA_IPV4_STR_LEN];
    char maskStr[CAPA_IPV4_STR_LEN];
    uint32 i;
    sys_mac_t zeroMac;
    sys_isg_entry_t entry;

    osal_memset(&zeroMac, 0, sizeof(zeroMac));
    osal_memset(&macStr, 0, sizeof(macStr));
    osal_memset(&ipStr, 0, sizeof(ipStr));
    osal_memset(&maskStr, 0, sizeof(maskStr));

    XMORE("\nBind Table: Maximun Binding Entry Number %u\n", CAPA_IPSG_ENTRY_NUM);
#ifdef CONFIG_SYS_PROTO_ISG_NETMASK
    XMORE("  Port  | VID  |    MAC Address    |              IP                |    Type     | Lease Time\n");
    XMORE("--------+------+-------------------+--------------------------------+-------------+-----------\n");
#else
    XMORE("  Port  | VID  |    MAC Address    |       IP        |    Type     | Lease Time\n");
    XMORE("--------+------+-------------------+-----------------+-------------+-----------\n");
#endif
    for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
    {
        osal_memset(&entry, 0, sizeof(sys_isg_entry_t));
        SYS_ERR_CONTINUE(sal_isg_bind_entry_get(i, &entry));

        if (!entry.isUsed)
            continue;

        if ((filterType != ISG_ENTRY_END) && (filterType != entry.type))
            continue;

        if (ISG_ENTRY_ALL_DENY == entry.type)
            continue;

        SYS_ERR_CHK(LP2STR(entry.port, portStr));
        SYS_ERR_CHK(sys_util_ip2Str(entry.ip, ipStr));

        XMORE(" %6s |", portStr);
        XMORE(" %4d |", entry.vid);

        if (0 == osal_memcmp(&zeroMac, &entry.mac, sizeof(sys_mac_t)))
        {
            SYS_ERR_CHK(sys_util_ip2Str(entry.mask, maskStr));
            XMORE(" %17s |", "N/A");
            XMORE("%15s(%15s)|", ipStr, maskStr);
        }
        else
        {
            SYS_ERR_CHK(sys_util_mac2Str(entry.mac.octet, macStr));
            XMORE(" %17s |", macStr);
#ifdef CONFIG_SYS_PROTO_ISG_NETMASK
            XMORE("%15s(255.255.255.255)|", ipStr);
#else
            XMORE(" %15s |", ipStr);
#endif
        }

        if (ISG_ENTRY_STATIC == entry.type)
            XMORE("   Static    | NA\n");
        else
            XMORE("DHCP Snooping| %u\n", entry.leaseTime);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_isg_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    int32 num = -1;
    uint32 cnum = 0;
    sys_enable_t enable = DISABLED;
    sys_isg_verify_type_t verifyType = ISG_VERIFY_IP;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE("  Port  |     Status     | Max Entry | Current Entry \n");
    XMORE("--------+----------------+-----------+---------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        osal_memset(portStr, 0, sizeof(portStr));
        SYS_ERR_CHK(LP2STR(logicPort, portStr));
        SYS_ERR_CHK(sal_isg_port_get(logicPort, &enable, &verifyType));
        SYS_ERR_CHK(sal_isg_portMaxEntryNum_get(logicPort, &num));
        SYS_ERR_CHK(sal_isg_portCurrentEntryNum_get(logicPort, &cnum));

        XMORE(" %6s |", portStr);

        if (DISABLED == enable)
            XMORE(" %14s |", text_enable[enable].text);
        else
        {
            if (ISG_VERIFY_IP == verifyType)
                XMORE(" %14s |", "Verify IP");
            else
                XMORE(" %14s |", "Verify MAC+IP");
        }

        if (IPSG_ENTRY_UNLIMITED == num)
            XMORE(" No Limit  |");
        else
            XMORE (" %4d      |", num);

        XMORE (" %3d \n", cnum);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_isg_bind_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_isg_entry_type_t filterType = ISG_ENTRY_END;

    if (1 == argc)
    {
        if ('d' == argv[0][0])
            filterType = ISG_ENTRY_DYNAMIC;
        else if ('s' == argv[0][0])
            filterType = ISG_ENTRY_STATIC;
    }

    _show_table(filterType);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

int32 cmd_isg_dhcpSnoopingbind_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    _show_table(ISG_ENTRY_DYNAMIC);

    return SYS_ERR_OK;
}

int32 cmd_isg_dhcpSnoopingDb_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int32 tmptime = 0;
    uint32 timer = 0;
    uint32 uptime = 0;
    sys_isg_db_t db;
    sys_isg_db_statistics_t dbs;

    SYS_MEM_CLEAR(db);
    SYS_ERR_CHK(sal_isg_db_get(&db));
    SYS_ERR_CHK(sal_isg_db_statistics_get(&dbs));
    SYS_ERR_CHK(sal_isg_db_timer_get(&timer));

    XMORE("\n");
    if (ISG_DB_TYPE_FLASH == db.dbType)
        XMORE("Type : Flash\n");
    else if (ISG_DB_TYPE_TFTP == db.dbType)
        XMORE("Type : tftp: %s\n", db.server);
    else
        XMORE("Type : None\n");

    XMORE("FileName : %s\n", (ISG_DB_TYPE_TFTP == db.dbType) ? db.filename : "");
    XMORE("Write delay Timer : %u seconds\n", db.writeDelay);
    XMORE("Abort Timer : %u seconds\n", db.timeout);

    XMORE("\n");
    XMORE("Agent Running : %s\n", (dbs.isRunning) ? "Running" : "None");
    if (TRUE == dbs.isRunning)
        XMORE("Delay Timer Expiry : %u seconds\n", timer);
    else
        XMORE("Delay Timer Expiry : Not Running\n");

    if (TRUE == dbs.isRunning)
    {
        SYS_ERR_CHK(osal_time_sysUptime_get(&uptime));
        tmptime = db.timeout - ((uptime - dbs.startTime)/1000);
        XMORE("Abort Timer Expiry : %u\n", (tmptime > 0) ? tmptime : 0);
    }
    else
        XMORE("Abort Timer Expiry :Not Running\n");

    XMORE("\n");
    XMORE("Last Succeded Time : %s\n", (0 == osal_strlen(dbs.lastSuccessedTime.tstr)) ? "None" : dbs.lastSuccessedTime.tstr);
    XMORE("Last Failed Time : %s\n", (0 == osal_strlen(dbs.lastFailedTime.tstr)) ? "None" : dbs.lastFailedTime.tstr);
    XMORE("Last Failed Reason : %s\n", dbs.lastFaildReason);

    XMORE("\n");
    XMORE("Total Attempts       : %5u\n", dbs.totalAttempts);
    XMORE("Successful Transfers : %5u   Failed Transfers : %5u\n", dbs.successfulTransfer, dbs.failedTransfer);
    XMORE("Successful Reads     : %5u   Failed Reads     : %5u\n", dbs.successfulReads, dbs.failedReads);
    XMORE("Successful Writes    : %5u   Failed Writes    : %5u\n", dbs.successfulWrites, dbs.failedWrites);
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_isg_dhcpSnoopingDb_renew(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    ISG_DB_RESTORE();

    return SYS_ERR_OK;
}

int32 cmd_isg_dhcpSnoopingDb_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_isg_db_statistics_t dbs;

    SYS_MEM_CLEAR(dbs);
    SYS_ERR_CHK(sal_isg_db_statistics_set(&dbs));

    return SYS_ERR_OK;
}
#endif 

int32 cmd_isg_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 tokenCnt = 0;
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;
    sys_isg_verify_type_t verifyType = ISG_VERIFY_IP;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
    {
        enable = ENABLED;

        tokenCnt = CMD_TOKENNUM_GET(cmd->vline);

        if (4 == tokenCnt)
            verifyType = ISG_VERIFY_IP_MAC;
        else
            verifyType = ISG_VERIFY_IP;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_isg_port_set(logicPort, enable, verifyType), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_isg_portMaxEntryNum_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int32 num = IPSG_ENTRY_UNLIMITED;
    sys_logic_port_t logicPort;

    if ('n' == argv[0][0])
        num = IPSG_ENTRY_UNLIMITED;
    else
        num = SYS_STR2UINT(argv[0]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_isg_portMaxEntryNum_set(logicPort, num), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_isg_bindTblEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_isg_entry_t entry;

    osal_memset(&entry, 0, sizeof(sys_isg_entry_t));

    if (NULL == rindex(argv[0], ':'))
    {
        SYS_ERR_CHK(sys_util_str2Ip(argv[1], &entry.ip));
        SYS_ERR_CHK(sys_util_str2Ip(argv[2], &entry.mask));
        entry.vid = SYS_STR2UINT(argv[0]);
    }
    else
    {
        SYS_ERR_CHK(sys_util_str2Mac(argv[0], entry.mac.octet));
        SYS_ERR_CHK(sys_util_str2Ip(argv[2], &entry.ip));
        entry.mask = 0xffffffff;
        entry.isCareMac = TRUE;
        entry.vid = SYS_STR2UINT(argv[1]);

    }

    SYS_ERR_CHK(STR2LP(argv[3], &entry.port));
    entry.type = ISG_ENTRY_STATIC;
    entry.isUsed = TRUE;

    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_isg_bind_entry_del(&entry));
    else
        SYS_ERR_CHK(sal_isg_bind_entry_set(&entry));

    return SYS_ERR_OK;
}

int32 cmd_isg_db_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char *tmp = NULL;
    uint8 tokenCnt = 0;
    sys_isg_db_t db;

    SYS_MEM_CLEAR(db);
    SYS_ERR_CHK(sal_isg_db_get(&db));

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (0 == argc)
        {
            SYS_ERR_CHK(sal_isg_db_clear());
            SYS_MEM_CLEAR(db.filename);
            SYS_MEM_CLEAR(db.server);
            db.dbType = ISG_DB_TYPE_NONE;
        }
        else if ('t' == argv[0][0])
            db.timeout = DFLT_IPSG_DB_TIMEOUT;
        else if ('w' == argv[0][0])
            db.writeDelay = DFLT_IPSG_DB_WRITE_DELAY;
    }
    else
    {
        tokenCnt = CMD_TOKENNUM_GET(cmd->vline);
        tmp = CMD_TOKENSTR_GET(cmd->vline, tokenCnt-2);

        if (2 == argc) 
        {
            SYS_ERR_CHK(sal_isg_db_clear());

            SYS_MEM_CLEAR(db.filename);
            SYS_MEM_CLEAR(db.server);

            db.dbType = ISG_DB_TYPE_TFTP;

            if (osal_strlen(argv[0]) >= CAPA_HOSTNAME_LEN )
                return SYS_ERR_STR_LEN_EXCEED;

            if((IS_IPV4_ADDRSTR_INVALID(argv[0])) && (IS_HOSTNAME_INVALID(argv[0])))
                return SYS_ERR_HOSTNAME;

            osal_sprintf(db.server, argv[0]);

            if (osal_strlen(argv[1]) > CAPA_FILE_NAME_LEN )
                return SYS_ERR_STR_LEN_EXCEED;

            osal_sprintf(db.filename, "%s", argv[1]);
        }
        else if (0 == argc) 
        {
            SYS_ERR_CHK(sal_isg_db_clear());
            SYS_MEM_CLEAR(db.filename);
            SYS_MEM_CLEAR(db.server);

            db.dbType = ISG_DB_TYPE_FLASH;
            osal_sprintf(db.filename, "isg_db_wanted");
        }
        else
        {
            if ('t' == tmp[0]) 
                db.timeout = SYS_STR2UINT(argv[0]);
            else if ('w' == tmp[0]) 
                db.writeDelay = SYS_STR2UINT(argv[0]);
        }
    }

    SYS_ERR_CHK(sal_isg_db_set(&db));

    if (CMD_IS_CLI_RUN())
    {
        ISG_DB_BACKUP();
    }

    return SYS_ERR_OK;
}

static int32 _cmd_isg_db_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_isg_db_t db;

    SYS_MEM_CLEAR(db);
    SYS_ERR_CHK(sal_isg_db_get(&db));

    if (ISG_DB_TYPE_FLASH == db.dbType)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip dhcp snooping database flash\n");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    else if (ISG_DB_TYPE_TFTP == db.dbType)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip dhcp snooping database tftp %s \"%s\"\n", db.server, db.filename);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (DFLT_IPSG_DB_TIMEOUT != db.timeout)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip dhcp snooping database timeout %u\n", db.timeout);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (DFLT_IPSG_DB_WRITE_DELAY != db.writeDelay)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip dhcp snooping database write-delay %u\n", db.writeDelay);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_isg_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char ipStr[CAPA_IPV4_STR_LEN];
    char maskStr[CAPA_IPV4_STR_LEN];
    char portStr[CAPA_PORT_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    uint32 i;
    sys_isg_entry_t entry;
    sys_mac_t zeroMac;

    osal_memset(&zeroMac, 0, sizeof(zeroMac));

    for (i = 0; i < CAPA_IPSG_ENTRY_NUM; i++)
    {
        osal_memset(&entry, 0, sizeof(sys_isg_entry_t));
        SYS_ERR_CONTINUE(sal_isg_bind_entry_get(i, &entry));

        if ((TRUE == entry.isUsed) && (ISG_ENTRY_STATIC == entry.type))
        {
            osal_memset(cmd, 0, sizeof(cmd));
            SYS_ERR_CHK(LP2STR(entry.port, portStr));

            if (0 == osal_memcmp(&zeroMac, &entry.mac, sizeof(sys_mac_t)))
            {
                SYS_ERR_CHK(sys_util_ip2Str(entry.ip, ipStr));
                SYS_ERR_CHK(sys_util_ip2Str(entry.mask, maskStr));
                osal_sprintf(cmd, "ip source binding vlan %u %s %s interface %s\n", entry.vid, ipStr, maskStr, portStr);
            }
            else
            {
                SYS_ERR_CHK(sys_util_mac2Str(entry.mac.octet, macStr));
                SYS_ERR_CHK(sys_util_ip2Str(entry.ip, ipStr));
                osal_sprintf(cmd, "ip source binding %s vlan %u %s interface %s \n", macStr, entry.vid, ipStr, portStr);
            }

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    SYS_ERR_CHK(_cmd_isg_db_save(fp));

    return SYS_ERR_OK;
}

int32 cmd_isg_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    int32 num = IPSG_ENTRY_UNLIMITED;;
    sys_enable_t enable = DISABLED;
    sys_isg_verify_type_t verifyType = ISG_VERIFY_IP;

    SYS_ERR_CHK(sal_isg_port_get(port, &enable, &verifyType));

    if (DFLT_IPSG_PORT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip source verify %s\n", (ENABLED == enable) ? "" : "no ",
            (ISG_VERIFY_IP_MAC == verifyType) ? "mac-and-ip" : "");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_isg_portMaxEntryNum_get(port, &num));

    if (DFLT_IPSG_PORT_MAX_ENTRY != num)
    {
        SYS_MEM_CLEAR(cmd);

        if (IPSG_ENTRY_UNLIMITED == num)
            osal_sprintf(cmd, "ip source binding max-entry no-limit\n");
        else
            osal_sprintf(cmd, "ip source binding max-entry %u\n", num);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

