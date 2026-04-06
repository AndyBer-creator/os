/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 16227 $
 * $Date: 2011-03-10 18:23:52 +0800 (Thu, 10 Mar 2011) $
 *
 * Purpose : Process special event which can't be processed in normal thread
 *
 * Feature : Process special event which can't be processed in normal thread
 */

/*
 * Include Files
 */
#include <stdarg.h>

#include <common/sys_util.h>
#include <libsal/sal_util.h>
#include <libsal/sal_crash.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */
extern void handleWebloginAuth(void);
extern void handleHttpd_restart(void);
extern void handleSaveOemtoFlash(void);
extern void handleSaveUsrCfgtoFlash(void);
extern void handleSnmpTrapConfig(void);
extern void handleMibtftpUpload(void);
extern void handleMibtftpDownload(void);
extern void handleMibStartupToRunconfig(void);
extern void handleMibCloneToRunconfig(void);
extern void handleMibSaveFlashConfig(void);
extern void handleBuildRunning(void);
extern void handleCloneRunning(void);
extern void handleReboot(void);
extern void handleUpgradeFw(void);
extern void handleWebTftpAction(char *action);
extern void handleSaveRunning(void);
extern void handleHttpdRestart(void);
extern void handleLockFwTimer(void);
extern void handleCliProgressPrint(char *pidFile);
#ifdef CONFIG_SYS_ADVANCE_DNS
extern void handleAdvDns(uint8 ver);
#endif
extern void handleDhcpDfltRoute(void);
#ifdef CONFIG_SYS_L3_ROUTE
extern void handleIpChgLineSessionClear(void);
extern void handleIPv6DupLog(char *intfStr, char *ipStr, char *plenStr);
extern void handleDHCPv6AddrUpdate(int32 action, char *intfStr, char *ipStr);
extern void handleL3IntfStsChg(char *intfStr, char *statusStr);
#endif
#ifdef CONFIG_SYS_PING_TEST
extern void handleWebPing(char *ipVer, char *cnt, char *address);
extern void handlePing(char *count, char *size, char *interval, char *pingIp); // old way
#endif
#ifdef CONFIG_SYS_TRACE_ROUTE
extern void handleTraceroute(char *host, uint32 ttl);
#endif

#ifdef CONFIG_SYS_UI_SNMP
  #ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
extern void handleRmonApply(void);
  #endif
#endif

#ifdef CONFIG_SYS_PROTO_ISG
extern int32 handleIpsgDBBackup(void);
extern int32 handleIpsgDBRestore(void);
#endif

#ifdef CONFIG_SYS_TECH_SUPPORT
extern void handleTechSupportGen(void);
#endif

extern void handleInitPort(void);
#ifdef CONFIG_SYS_L3_ROUTE
extern void handleInitL3(void);
#endif

int main(int argc, char **argv)
{
    char cmd[SYS_BUF128_LEN];


    SYS_MEM_CLEAR(cmd);

    if (argc >= 3)
    {
        switch (argv[1][1])
        {
			case 'w':
				if (0 == osal_strcmp(argv[2], "login")) //web login
                {
                    handleWebloginAuth();
                    return 0;
                }
                else if(0 == osal_strcmp(argv[2], "httpServer"))
                {
					handleHttpd_restart();
                    return 0;
                }
        
            case 'f':
                if (0 == osal_strcmp(argv[2], "fwupgrade")) //fwupgrade
                {
                    handleUpgradeFw();

                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "web_tftp_action")) //lockfwtimer
                {
                    if (argc >= 4)
                        handleWebTftpAction(argv[3]);
                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "lockfwtimer")) //lockfwtimer
                {
                    handleLockFwTimer();
                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "reboot")) //reboot
                {
                    handleReboot();

                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "saverunning")) //saverunning
                {
                    handleSaveRunning();

                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "print_cli_progress")) //print_cli_progress
                {
                    handleCliProgressPrint(argv[3]);

                    return 0;
                }
#ifdef CONFIG_SYS_TECH_SUPPORT
                else if (0 == osal_strcmp(argv[2], "tech_support_gen"))
                {
                    handleTechSupportGen();

                    return 0;
                }
#endif
                else if (0 == osal_strcmp(argv[2], "init_port"))
                {
                    handleInitPort();
                    return 0;
                }
#ifdef CONFIG_SYS_L3_ROUTE
                else if (0 == osal_strcmp(argv[2], "init_l3"))
                {
                    handleInitL3();
                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "ipv6_dup_log"))
                {
                    if (argc >= 6)
                        handleIPv6DupLog(argv[3], argv[4], argv[5]);

                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "l3intf_sts"))
                {
                    if (argc >= 5)
                        handleL3IntfStsChg(argv[3], argv[4]);

                    return 0;
                }
#endif
                else if (0 == osal_strcmp(argv[2], "runconfig")) //build runconfig
                {
                    handleBuildRunning();

                    return 0;
                }
                else if(0 == osal_strcmp(argv[2], "cloneconfig"))// build cloneconfig
                {
					handleCloneRunning();
                }
				else if(0 == osal_strcmp(argv[2], "mibsaverunconfig"))
				{
					handleMibSaveFlashConfig();
				}
				else if(0 == osal_strcmp(argv[2], "mibstartuptorunconfig"))
				{
					handleMibStartupToRunconfig();
				}
				else if(0 == osal_strcmp(argv[2], "mibclonetorunconfig"))
				{
					handleMibCloneToRunconfig();
				}
				else if(0 == osal_strcmp(argv[2], "mibtftpuploadconfig"))
				{
					handleMibtftpUpload();
				}
				else if(0 == osal_strcmp(argv[2], "mibtftpdownloadconfig"))
				{
					handleMibtftpDownload();
				}
				else if(0 == osal_strcmp(argv[2], "snmptarpconfig"))
				{
					handleSnmpTrapConfig();
				}
				else if (0 == osal_strcmp(argv[2], "saveOemtoFlash"))
                {
					handleSaveOemtoFlash();
					return 0;
                }
 				else if (0 == osal_strcmp(argv[2], "saveUsrCfgtoFlash"))
                {
					handleSaveUsrCfgtoFlash();
					return 0;
                }
                break;

            case 'd': /* dhcp */
                if (0 == osal_strcmp(argv[2], "ip"))
                {
                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "gw"))
                {
                    return 0;
                }
#ifdef CONFIG_SYS_ADVANCE_DNS
                else if (0 == osal_strcmp(argv[2], "advdns"))
                {
                    handleAdvDns(4);
                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "advdnsv6"))
                {
                    handleAdvDns(6);
                    return 0;
                }
#endif
                else if (0 == osal_strcmp(argv[2], "dflt_route"))
                {
                    handleDhcpDfltRoute();
                    return 0;
                }
#ifdef CONFIG_SYS_L3_ROUTE
                else if (0 == osal_strcmp(argv[2], "v6_addr"))
                {
                    char devStr[CAPA_L3_INTFNAME_LEN];
                    char ipStr[CAPA_IPV6_STR_LEN];

                    SYS_MEM_CLEAR(devStr);
                    SYS_MEM_CLEAR(ipStr);

                    osal_strncpy(devStr, argv[4], sizeof(devStr));
                    osal_strncpy(ipStr, argv[5], sizeof(ipStr));

                    if (argc >= 6)
                    {
                        if (0 == osal_strcmp(argv[3], "add"))
                        {
                            handleDHCPv6AddrUpdate(0, devStr, ipStr);
                        }
                        else if (0 == osal_strcmp(argv[3], "del"))
                        {
                            handleDHCPv6AddrUpdate(1, devStr, ipStr);
                        }
                    }
                    return 0;
                }
#endif
                break;

#ifdef CONFIG_SYS_SYSLOG
            case 'l':   /* syslog */
                if (argc >= 4)
                {
                    if (argc == 4)
                        sys_log(strtoul(argv[2], NULL, 10), strtoul(argv[3], NULL, 10));
                    else
                        sys_log2(strtoul(argv[2], NULL, 10), strtoul(argv[3], NULL, 10), (va_list)(argv + 4));

                    return 0;
                }

                break;
#endif

#ifdef CONFIG_SYS_UI_SNMP
            case 's': //snmp
                if ('m' == argv[2][1]) //rmonapply
                {
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
  #define RMON_TMP_LOCK_FILE  "/tmp/rmonapply.lock"

                    if (SYS_ERR_FILE_NOT_EXIST == sal_util_file_exist(RMON_TMP_LOCK_FILE))
                    {
                        system("touch "RMON_TMP_LOCK_FILE);

                        handleRmonApply();

                        unlink(RMON_TMP_LOCK_FILE);
                    }
#endif
                    return 0;
                }
#endif


#ifdef CONFIG_SYS_PROTO_ISG
            case 'i': //ip source guard
                if ('b' == argv[2][6]) //isgdb_backup
                    handleIpsgDBBackup();
                else if ('r' == argv[2][6]) //isgdb_restore
                    handleIpsgDBRestore();

                return 0;
#endif

            case 'c': //clear
                if ('t' == argv[2][0]) //telnet
                {
                    /* clear pid files */
                    //system("rm -f /tmp/telnet_*_pid");
                    system("rm -f "SYS_SESSION_DIR"/telnet_*_pid");
                    sal_util_process_kill("telnet");

                    return 0;
                }
                else if ('s' == argv[2][0]) //ssh
                {
                    sal_util_process_kill("sshd:");

                    return 0;
                }
                /* Kill original httpd and restart new one */
                else if ('b' == argv[2][0]) //boa
                {
                    handleHttpdRestart();

                    return 0;
                }
#ifdef CONFIG_SYS_L3_ROUTE
                else if ('i' == argv[2][0]) //ipchg
                {
                    handleIpChgLineSessionClear();

                    return 0;
                }
#endif
                break;
#ifdef CONFIG_SYS_PING_TEST
    // new way ..wait new web gui
            case 'P': //ping
                handleWebPing(argv[2],argv[3],argv[4]);
                return 0;
            case 'p': //ping -p count size interval 192.168.1.100(baidu.com)
                    handlePing(argv[2],argv[3],argv[4],argv[5]);
                    return 0;
#endif
#ifdef CONFIG_SYS_TRACE_ROUTE
            case 't':
                handleTraceroute(argv[2], strtoul(argv[3], NULL, 10));
                return 0;
#endif
#ifdef CONFIG_SYS_APP_MSNTP
            case 'u': // update
                if (0 == osal_strcmp(argv[2], "pooltime")) //update polling time
                {
                    sal_time_update(TRUE);
                    return 0;
                }
                else if (0 == osal_strcmp(argv[2], "time")) //update time
                {
                    sal_time_update(FALSE);
                    return 0;
                }
#endif
            default:
                break;
        }
    }

    printf("Usage: Help!!!!!! %s %s %s\n", argv[0],argv[1],argv[2]);

    return SYS_ERR_OK;
}

