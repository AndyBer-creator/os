
#include <libsal/sal_util.h>
#include <libsal/sal_mib_private.h>

#include <libsal/sal_snmp.h>
#include <libfds/fds_export.h>
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
#include <libsal/sal_rmon.h>
#endif
#include <libcustom/sal_custom.h>

#define SNMP_SYSUPTIME_FILE             "/etc/snmp/sysUpTime"
#define SNMPD_CONF_SYS_NAME             0
#define SNMPD_CONF_SYS_LOCATION         1
#define SNMPD_CONF_SYS_CONTACT          2
#define SNMPD_CONF_COMMUNITY            3
#define SNMPD_CONF_TRAPHOST             11

const sys_text_t text_snmp_access[] =
{
    {           "Read-Only"     },
    {          "Read-Write"    },
};

const char *gsecModel[] =
{
    "v1",
    "v2c",
    "v3",
};

const char *ggroupSecModel[] =
{
    "v1",
    "v2c",
    "usm",
    "any",
};

const char *gtrapStr[] =
{
    "trapsink",
    "trap2sink",

    "trapsess",
    "trapsess",
    "trapsess",
};

const char *gtrapType[] =
{
    "trap",
    "trap",
    "inform",
    "trap",
    "inform",
};

const char *gtrapVersion[] =
{
    "v1",
    "v2c",
    "v2c",
    "v3",
    "v3",
};

#ifdef CONFIG_SYS_MIB_TRAP
const char *snmpTrapClass[SNMP_TRAP_CLASS_END] =
{
    "authentication failure",
    "link up/link down",
    "warm start-up",
    "cool start-up",
    "port security",
};
#endif

#ifdef CONFIG_SYS_SNMPV3
const char *snmpv3_priv[SNMP_SSL_PRIV_END] =
{
    "noauth",
    "auth",
    "priv",
};

const char *gv3privOption[] =
{
    "noAuthNoPriv",
    "authNoPriv",
    "authPriv"
};

const char *gviewTypeStr[] =
{
    "none",
    "included",
    "excluded"
};

const char *gsecCfModel[] =
{
    "1",
    "2c",
    "3"
};

const char *gsecLevel[] =
{
    "noauth",
    "auth",
    "priv"
};

const char *gprefix[] =
{
    "exact",
    "prefix"
};

const char *gaccess[] =
{
    "ro",
     "rw"
};

const char *gsslType[] =
{
    "none",
    "md5",
     "sha",
     "des"
};

const char *snmpv3_ssl_type[SNMP_SSL_TYPE_END] =
{
    "None",
    "MD5",
    "SHA",
    "DES"
};
#else
static char *snmpdConfData[] =
{ "sysName %s\n",
  "sysLocation %s\n",
  "sysContact %s\n",
  "%s %s\n",
  "%s %s\n",
  "%s %s\n",
  "%s %s\n",
  "%s %s\n",
  "%s %s\n",
  "%s %s\n",
  "%s %s\n",
  "%s %s %s\n",
  "%s %s %s\n",
  "%s %s %s\n",
  "%s %s %s\n"
};

const char *snmpv3_access[SNMP_COMMUNITY_ACCESS_END] =
{
    "read-only",
    "read-write",
};

const char *gsecLevel[] =
{
    "noauth",
    "auth",
    "priv"
};
#endif  

#ifdef CONFIG_SYS_SNMPV3
static int32 _sal_snmp_mask2hexStr(const char *pBin , char *pHex);
#endif

#ifdef CONFIG_SYS_MIB_TRAP
int32 sal_snmp_systemTrap_send(char *pInfo)
{
    int32 i;
    sys_snmp_notifyMsg_t notifyMsg;

    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    if (osal_strlen(pInfo) > MAX_SNMP_TRAP_STRLEN)
    {

            return SYS_ERR_FAILED;
    }

    for (i = 0; i < SNMP_NOTIFY_MAX_BUF; i++)
    {
        SYS_ERR_CONTINUE(cfg_snmp_notifyMsg_get(i, &notifyMsg));
        if (!notifyMsg.valid)
        {
            SYS_MEM_CLEAR(notifyMsg);
            notifyMsg.valid = TRUE;
            osal_memcpy(notifyMsg.trapMsg, pInfo, osal_strlen(pInfo));
            SYS_ERR_CHK(cfg_snmp_notifyMsg_set(i, &notifyMsg));
                break;
        }
    }

    if (i == SNMP_NOTIFY_MAX_BUF)
    {

            return SYS_ERR_FAILED;
    }

#if 0
    ret = sal_util_file_exist(SNMP_TRAP_DB_FILE);

    fp = fopen(SNMP_TRAP_DB_FILE, "a+");

    while (fp == NULL)
    {
        usleep(100);
        fp = fopen(SNMP_TRAP_DB_FILE, "a+");
    }

    fprintf(fp, "%s\n", pInfo);
    fclose(fp);
#endif

    return SYS_ERR_OK;
}
#endif

int32 sal_snmp_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_snmp_enable_set(enable));
    if (DISABLED == enable)
    {
        _sal_snmp_notifyMsg_init();
    }

    return SYS_ERR_OK;
}

int32 sal_snmp_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_snmp_enable_get(pEnable));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MIB_TRAP

int32 sal_snmp_trapEnable_get(sal_snmp_trapClass_t trapclass, sys_enable_t *pEnable)
{
    uint32 mask = 0;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_snmp_trapEnable_get(&mask));

    if (0 == (mask & (1 << trapclass)))
        *pEnable = DISABLED;
    else
        *pEnable = ENABLED;

    return SYS_ERR_OK;
}

int32 sal_snmp_trapEnable_set(sal_snmp_trapClass_t trapclass, sys_enable_t enable)
{
    uint32 mask = 0;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_snmp_trapEnable_get(&mask));

    if (ENABLED == enable)
        mask |= 1 << trapclass;
    else
        mask &= ~(1<<trapclass);

    SYS_ERR_CHK(cfg_snmp_trapEnable_set(mask));

    return SYS_ERR_OK;
}
#endif

int32 sal_snmp_sysUpTime_set(void)
{
    struct timeval now;
    FILE    *fp;
    char    cmd[128];

    fp = fopen(SNMP_SYSUPTIME_FILE,"r");

    if (NULL != fp)
    {
        fclose(fp);
        osal_memset(cmd, 0, sizeof(cmd));
        osal_sprintf(cmd, "rm -f %s", SNMP_SYSUPTIME_FILE);
        system(cmd);
    }

    gettimeofday(&now, NULL);
    fp = fopen(SNMP_SYSUPTIME_FILE, "wt");

    if (NULL != fp)
    {
        fprintf(fp, "%lu\n", now.tv_sec);
        fprintf(fp, "%lu\n", now.tv_usec);
        fclose(fp);
    }
    else
    {
        return SYS_ERR_FILE_CREATE;
    }

    return SYS_ERR_OK;
}

int32 sal_snmp_sysUpTime_get(uint32 *pSec, uint32 *pUsec)
{
    FILE *fp = NULL;
    char sec[128];
    char usec[128];

    SYS_PARAM_CHK((NULL == pSec), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pUsec), SYS_ERR_NULL_POINTER);

    fp = fopen(SNMP_SYSUPTIME_FILE, "r");

    if (NULL != fp)
    {
        osal_memset(sec, 0, sizeof(sec));
        osal_memset(usec, 0, sizeof(usec));

        fgets(sec, sizeof(sec), fp);
        fgets(usec, sizeof(usec), fp);
        *pSec = atoi(sec);
        *pUsec = atoi(usec);
        fclose(fp);
    }
    else
    {
        return SYS_ERR_FILE_OPEN;
    }

    return SYS_ERR_OK;
}

int32 sal_snmp_sysUpTimeGep_get(uint64 *time)
{
    struct timeval now;
    struct timeval diff;
    uint32 sec = 0;
    uint32 usec = 0;

    SYS_PARAM_CHK((NULL == time), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_sysUpTime_get(&sec, &usec));
    gettimeofday(&now, NULL);

    diff.tv_sec = now.tv_sec - sec - 1;
    diff.tv_usec = now.tv_usec - usec + 1000000;
    *time = ((u_long) diff.tv_sec) * 100 + diff.tv_usec / 10000;

    return SYS_ERR_OK;
}

int32 sal_snmp_stop(void)
{
    unlink(SNMP_CONF_VAR_FILE);
    usleep(1500000);
    system("killall -9 snmpd 1> /dev/null 2> /dev/null");

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_SNMP_STOP);

    return SYS_ERR_OK;
}

int32 sal_snmp_start(void)
{
    char cmd[256];

    osal_memset(cmd, 0, sizeof(cmd));

#ifdef CONFIG_SYS_IPV6
    osal_sprintf(cmd, "snmpd -p /var/run/snmp.pid -C -c %s udp:161 udp6:161 1> /dev/null 2> /dev/null &", SNMP_CONF_FILE);
#else
    osal_sprintf(cmd, "snmpd -p /var/run/snmp.pid -C -c %s 1> /dev/null 2> /dev/null &", SNMP_CONF_FILE);
#endif

    system(cmd);

    SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_SNMP_START);

    return SYS_ERR_OK;
}

int32 sal_snmp_restart()
{
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(cfg_snmp_enable_get(&enable));

    if (ENABLED == enable)
    {
        SYS_ERR_CHK(sal_snmp_confFile_update());
        SYS_ERR_CHK(sal_snmp_stop());
        SYS_ERR_CHK(sal_snmp_start());

        sleep(1);

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
        system("handler -s rmonapply 1>> /dev/null 2>> /dev/null &");
#endif
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_MIB_TRAP

int32 sal_snmp_linkUpDownTrap_send(sys_logic_port_t logicPort, sys_port_linkStatus_t link)
{
    char buf[SYS_BUF32_LEN];
    sys_enable_t snmp_enable = DISABLED;
    sys_enable_t trap_enable = DISABLED;
    sys_time_str_t timestring;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((link >= PORT_LINKSTATUS_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_LINKUPDOWN, &trap_enable));

    if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d %s *%d", (PORT_LINKDOWN == link) ? SNMP_TRAP_CODE_LINKDOWN : SNMP_TRAP_CODE_LINKUP, timestring.tstr,logicPort);

    sal_snmp_systemTrap_send(buf);

    return SYS_ERR_OK;
}

int32 sal_snmp_startupTrap_send(sys_enable_t restart)
{
    char buf[SYS_BUF32_LEN];
    sys_enable_t snmp_enable = DISABLED;
    sys_enable_t trap_enable = DISABLED;
    sal_snmp_trapCode_t trapCode = SNMP_TRAP_CODE_END;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(restart), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));

    if (ENABLED == restart)
    {
        trapCode = SNMP_TRAP_CODE_WARM_START;
        SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_WARM, &trap_enable));
        if  (DISABLED == trap_enable)
        {
            SNMP_LOG(LOG_SNMP_TRAP_DROP);
            return SYS_ERR_OK;
        }
    }
    else
    {
        trapCode = SNMP_TRAP_CODE_COLD_START;
        SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_BOOTUP_COOL, &trap_enable));
        if  (DISABLED == trap_enable)
        {
            SNMP_LOG(LOG_SNMP_TRAP_DROP);
            return SYS_ERR_OK;
         }
    }

    if (DISABLED == snmp_enable)
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d", trapCode);

    sal_snmp_systemTrap_send(buf);
    return SYS_ERR_OK;
}

int32 sal_snmp_portSecurityTrap_send(char *pOid, sys_logic_port_t logicPort, char *pMsg)
{
    char buf[SYS_BUF256_LEN];
    sys_enable_t snmp_enable = DISABLED;
    sys_enable_t trap_enable = DISABLED;

    SYS_PARAM_CHK((NULL == pOid), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_PORT_SEC, &trap_enable));

    if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d %s", SNMP_TRAP_CODE_PORT_SECURITY, pMsg);

     sal_snmp_systemTrap_send(buf);
    return SYS_ERR_OK;
}

int32 sal_snmp_CPUTrap_send(char *pInfo)
{
    char buf[SYS_BUF256_LEN];
    sys_enable_t snmp_enable = DISABLED;

	sys_time_str_t timestring;

	SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

    if(DISABLED == snmp_enable)
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d %s %s", SNMP_CMTRAP_CODE_CPU, timestring.tstr, pInfo);

    sal_snmp_systemTrap_send(buf);

    return SYS_ERR_OK;
}

int32 sal_snmp_memoryTrap_send(char *pInfo)
{
    char buf[SYS_BUF256_LEN];
    sys_enable_t snmp_enable = DISABLED;

	sys_time_str_t timestring;

	SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

    if(DISABLED == snmp_enable)
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d %s %s", SNMP_CMTRAP_CODE_MEMORY, timestring.tstr, pInfo);

    sal_snmp_systemTrap_send(buf);

    return SYS_ERR_OK;
}

int32 sal_snmp_storm_ctrl_Trap_send(char *pInfo)
{
    char buf[SYS_BUF256_LEN];
    sys_enable_t snmp_enable = DISABLED;

	sys_time_str_t timestring;

	SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

    if(DISABLED == snmp_enable)
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d %s %s", SNMP_CMTRAP_CODE_STORMCTRL, timestring.tstr, pInfo);

    sal_snmp_systemTrap_send(buf);

    return SYS_ERR_OK;
}

int32 sal_snmp_systerm_reboot_Trap_send(char *pInfo)
{
    char buf[SYS_BUF256_LEN];
    sys_enable_t snmp_enable = DISABLED;

	sys_time_str_t timestring;

	SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

    if(DISABLED == snmp_enable)
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d %s %s", SNMP_CMTRAP_CODE_REBOOT, timestring.tstr, pInfo);

    sal_snmp_systemTrap_send(buf);

    return SYS_ERR_OK;
}

int32 sal_snmp_systerm_upgrade_Trap_send(char *pInfo)
{
    char buf[SYS_BUF256_LEN];
    sys_enable_t snmp_enable = DISABLED;

	sys_time_str_t timestring;

	SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

    if(DISABLED == snmp_enable)
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d %s %s", SNMP_CMTRAP_CODE_UPGRADE, timestring.tstr, pInfo);

    sal_snmp_systemTrap_send(buf);

    return SYS_ERR_OK;
}

int32 sal_snmp_operation_Trap_send(char *pInfo)
{
    char buf[SYS_BUF256_LEN];
    sys_enable_t snmp_enable = DISABLED;

	sys_time_str_t timestring;

	SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

    if(DISABLED == snmp_enable)
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d %s %s", SNMP_CMTRAP_CODE_OPERATE, timestring.tstr, pInfo);

    sal_snmp_systemTrap_send(buf);

    return SYS_ERR_OK;
}

int32 sal_snmp_poe_Trap_send(char *pInfo)
{
    char buf[SYS_BUF256_LEN];
    sys_enable_t snmp_enable = DISABLED;

	sys_time_str_t timestring;

	SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

    if(DISABLED == snmp_enable)
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%d %s %s", SNMP_CMTRAP_CODE_POE, timestring.tstr, pInfo);

    sal_snmp_systemTrap_send(buf);

    return SYS_ERR_OK;
}

#endif

int32 sal_snmp_target_get(uint32 index, sal_snmp_target_t *pTarget)
{
    SYS_PARAM_CHK((NULL == pTarget), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= CAPA_SNMP_HOST_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_target_get(index, pTarget));

    return SYS_ERR_OK;
}

int32 sal_snmp_target_set(uint32 index, sal_snmp_target_t *pTarget)
{
    SYS_PARAM_CHK((NULL == pTarget), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= CAPA_SNMP_HOST_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_target_set(index, pTarget));

    return SYS_ERR_OK;
}

int32 sal_snmp_targetEmpty_get(uint32 *pIndex)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_target_t target;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_HOST_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_target_get(i, &target));

        if ((FALSE == found) && (0 == osal_strlen(target.server)))
        {
            *pIndex = i;
            found = TRUE;
        }
    }

    if (FALSE == found)
        *pIndex = CAPA_SNMP_HOST_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_targetIndex_get(char *pServer, sal_snmp_secmode_t secModel, sal_snmp_trapType_t trapType, uint32 *index)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_target_t target;

    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pServer), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_HOST_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_target_get(i, &target));

        if ((FALSE == found) && (0 == osal_strcmp(pServer, target.server)) &&
            (secModel == target.secModel) && (trapType == target.trapType))
        {
             *index = i;
             found = TRUE;
        }
    }

    if (FALSE == found)
        *index = CAPA_SNMP_HOST_NUM;

    return SYS_ERR_OK;

}

int32 sal_snmp_target_add(sal_snmp_target_t *pHost)
{
    uint32 index = 0;

    SYS_PARAM_CHK((NULL == pHost), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pHost->secModel < SNMP_SECMODEL_V1 || pHost->secModel >= SNMP_SECMODEL_END) , SYS_ERR_SECURITY_LEVEL);
    SYS_PARAM_CHK((pHost->trapType < SNMP_TRAP_V1 || pHost->trapType >= SNMP_TRAP_END) , SYS_ERR_TRAP_TYPE);

#ifdef CONFIG_SYS_SNMPV3
    if (pHost->trapType  < SNMP_TRAP_V3)
    {
        SYS_ERR_CHK(sal_snmp_com2secIndex_get(pHost->name, &index));

        if (CAPA_SNMP_COMMUNITY_NUM == index)
            return SYS_ERR_COMNUNITY_NOT_FOUND;
    }
    else
    {
        SYS_ERR_CHK(sal_snmp_userIndex_get(pHost->name, &index));

        if (CAPA_SNMP_USER_NUM == index)
            return SYS_ERR_USER_NOT_FOUND;
    }
#endif

    SYS_ERR_CHK(sal_snmp_targetIndex_get(pHost->server, pHost->secModel, pHost->trapType, &index));

    if (index < CAPA_SNMP_HOST_NUM)
        return SYS_ERR_DUPLICATE_ENTRY;

    SYS_ERR_CHK(sal_snmp_targetEmpty_get(&index));

    if (CAPA_SNMP_HOST_NUM == index)
        return SYS_ERR_SNMP_TARGET_FULL;

    SYS_ERR_CHK(cfg_snmp_target_set(index, pHost));
    SYS_ERR_CHK(_sal_snmp_targetSort_update());

    return SYS_ERR_OK;
}

int32 sal_snmp_target_del(char *pServer,  sal_snmp_secmode_t secModel, sal_snmp_trapType_t trapType)
{
    uint32 index = 0;
    sal_snmp_target_t target;

    SYS_PARAM_CHK((NULL == pServer), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_targetIndex_get(pServer, secModel, trapType, &index));

    if (CAPA_SNMP_HOST_NUM == index)
        return SYS_ERR_ENTRY_NOT_FOUND;

    SYS_MEM_CLEAR(target.server);
    target.name[0] = '\0';
    target.secModel= SNMP_SECMODEL_END;
    target.trapType = SNMP_TRAP_END;

    SYS_ERR_CHK(cfg_snmp_target_set(index, &target));

    return SYS_ERR_OK;
}

int32 sal_snmp_targetByIdx_del(uint32 idx)
{
    sal_snmp_target_t target, tmpTarget;

    SYS_PARAM_CHK((idx >= CAPA_SNMP_HOST_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_MEM_CLEAR(target);
    SYS_MEM_CLEAR(tmpTarget);

    SYS_ERR_CHK(cfg_snmp_target_get(idx, &tmpTarget));
    SYS_ERR_CHK(cfg_snmp_target_set(idx, &target));

    return SYS_ERR_OK;
}

int32 _sal_snmp_targetSort_update(void)
{
    int i, j, entryNum = 0;
    sal_snmp_target_t hostArry[CAPA_SNMP_HOST_NUM + 2];
    sal_snmp_target_t tmpHost;

    SYS_MEM_CLEAR(hostArry);

    for (i = 0; i < CAPA_SNMP_HOST_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpHost);
        SYS_ERR_CONTINUE(sal_snmp_target_get(i, &tmpHost));
        if (0 == osal_strlen(tmpHost.server))
            continue;

        osal_memcpy(&hostArry[entryNum], &tmpHost, sizeof(sal_snmp_target_t)) ;
        entryNum++;
    }

   if (entryNum > 1)
   {
        for (j = 0; j < entryNum; j++)
        {
            for(i = 0; i < entryNum - j; i++)
            {
                if (0 == osal_strlen(hostArry[i+1].server))
                    continue;

                if (osal_strcmp(hostArry[i].server, hostArry[i+1].server) > 0)
                {
                    osal_memcpy(&tmpHost, &hostArry[i], sizeof(sal_snmp_target_t)) ;
                    osal_memcpy(&hostArry[i], &hostArry[i+1], sizeof(sal_snmp_target_t)) ;
                    osal_memcpy(&hostArry[i+1], &tmpHost, sizeof(sal_snmp_target_t)) ;
                }
             }
        }
    }

    SYS_ERR_CHK(cfg_snmp_targetArry_set(hostArry, entryNum));
    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_SNMPV3

int32 sal_snmp_view_get(uint32 index, sal_snmp_view_t *pView)
{
    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= CAPA_SNMP_VIEW_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_view_get(index, pView));

    return SYS_ERR_OK;
}

int32 sal_snmp_view_set(uint32 index, sal_snmp_view_t *pView)
{
    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= CAPA_SNMP_VIEW_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_view_set(index, pView));

    return SYS_ERR_OK;
}

int32 sal_snmp_viewEmpty_get(uint32 *pIndex)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_view_t view;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_VIEW_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_view_get(i, &view));

        if ((FALSE == found) && (0 == osal_strlen(view.viewName)))
        {
            *pIndex = i;
            found = TRUE;
        }
    }

    if (FALSE == found)
        *pIndex = CAPA_SNMP_VIEW_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_viewIndex_get(char *pView, sal_snmp_oid_t *pSubtree, uint32 *index)
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 found = FALSE;
    sal_snmp_view_t view;

    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pSubtree), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_VIEW_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_view_get(i, &view));

        if ((FALSE == found) && (osal_strlen(view.viewName) == osal_strlen(pView)) &&
            (0 == osal_memcmp(view.viewName, pView, osal_strlen(pView))))
        {
           if (pSubtree->len != view.subtreeOid.len)
                continue;

           for (j = 0; j < pSubtree->len; j++ )
           {
                if( pSubtree->id[j] != view.subtreeOid.id[j])
                    break;
           }

           if(j == pSubtree->len)
           {
             *index = i;
             found = TRUE;
           }
        }
    }

    if (FALSE == found)
        *index = CAPA_SNMP_VIEW_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_viewIndexByName_get(char *pView, uint32 *index)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_view_t view;

    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_VIEW_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_view_get(i, &view));

        if((FALSE == found) && (osal_strlen(view.viewName) == osal_strlen(pView)) &&
            (0 == osal_memcmp(view.viewName, pView, osal_strlen(pView))))
         {
            *index = i;
             found = TRUE;
         }
    }

    if (FALSE == found)
        *index = CAPA_SNMP_VIEW_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_view_add(char *pView, sal_snmp_oid_t *pSubtree, uint8 viewType,  char *pmsk)
{
    uint32 index = 0;
    sal_snmp_view_t view;
    char oidStr[CAPA_SNMP_OID_STR_LEN];

    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pSubtree), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pmsk), SYS_ERR_NULL_POINTER);

    SYS_PARAM_CHK((osal_strlen(pView) > CAPA_SNMP_VIEW_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((pSubtree->len > CAPA_SNMP_VIEW_SUBTREE_LEN), SYS_ERR_OID_LEN);
    SYS_PARAM_CHK((osal_strlen(pView) == 0), SYS_ERR_STR_LEN_EMPTY);

    osal_memset(&view, 0, sizeof(sal_snmp_view_t));
    
    SYS_ERR_CHK(sal_snmp_viewIndex_get(pView, pSubtree, &index));

    if (index < CAPA_SNMP_VIEW_NUM)
        return SYS_ERR_DUPLICATE_ENTRY;

    SYS_ERR_CHK(sal_snmp_viewEmpty_get(&index));

    if (CAPA_SNMP_VIEW_NUM == index)
        return SYS_ERR_SNMP_VIEW_FULL;

    osal_strcpy(view.viewName, pView);
    osal_memcpy(&(view.subtreeOid), pSubtree, sizeof(sal_snmp_oid_t));
    view.viewType =  viewType;

    if(0 != osal_strlen(pmsk))
        osal_memcpy(view.viewMaskStr, pmsk, osal_strlen(pmsk));
    else
        osal_memset(view.viewMaskStr,0, sizeof(view.viewMaskStr));

    SYS_ERR_CHK(cfg_snmp_view_set(index, &view));
    SYS_ERR_CHK(_sal_snmp_viewSort_update());

    SYS_ERR_CHK(sal_util_oid2str(pSubtree->len, pSubtree->id, oidStr));

    return SYS_ERR_OK;
}

int32 sal_snmp_view_del(char *pView, sal_snmp_oid_t *pSubtree)
{
    uint32 index;
    sal_snmp_view_t view;
    char oidStr[CAPA_SNMP_OID_STR_LEN];

    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pSubtree), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pView) > CAPA_SNMP_VIEW_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((pSubtree->len > CAPA_SNMP_VIEW_SUBTREE_LEN), SYS_ERR_OID_LEN);

    SYS_ERR_CHK(sal_snmp_viewIndex_get(pView,pSubtree, &index));

    if (CAPA_SNMP_VIEW_NUM == index)
        return SYS_ERR_ENTRY_NOT_FOUND;

    osal_memset(view.viewName,0,sizeof(view.viewName));
    view.viewType = SNMP_VIEWTYPE_END;
    osal_memset(&(view.subtreeOid), 0, sizeof(sal_snmp_oid_t));
    osal_memset(view.viewMaskStr, 0, sizeof(view.viewMaskStr));

    SYS_ERR_CHK(cfg_snmp_view_set(index, &view));
    SYS_ERR_CHK(sal_util_oid2str(pSubtree->len, pSubtree->id, oidStr));

    return SYS_ERR_OK;
}

int32 sal_snmp_viewByIdx_del(uint32 idx)
{
    sal_snmp_view_t view, tmpView;
    char oidStr[CAPA_SNMP_OID_STR_LEN];

    SYS_PARAM_CHK((idx >=  CAPA_SNMP_VIEW_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_MEM_CLEAR(view);
    SYS_MEM_CLEAR(tmpView);

   SYS_ERR_CHK(cfg_snmp_view_get(idx, &tmpView));

   SYS_ERR_CHK(cfg_snmp_view_set(idx, &view));

   SYS_ERR_CHK(sal_util_oid2str(tmpView.subtreeOid.len, tmpView.subtreeOid.id, oidStr));

    return SYS_ERR_OK;
}

int32 sal_snmp_group_get(uint32 index, sal_snmp_group_t *pGroup)
{
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= SYS_SNMP_SECGROUP_NUM_MAX), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_group_get(index, pGroup));

    return SYS_ERR_OK;
}

int32 sal_snmp_group_set(uint32 index, sal_snmp_group_t *pGroup)
{
   SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= SYS_SNMP_SECGROUP_NUM_MAX), SYS_ERR_ENTRY_INDEX);

   SYS_ERR_CHK(cfg_snmp_group_set(index, pGroup));

    return SYS_ERR_OK;
}

int32 sal_snmp_groupEmpty_get(uint32 *pIndex)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_group_t group;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    for (i = 0; i < SYS_SNMP_SECGROUP_NUM_MAX; i++)
    {
        SYS_ERR_CHK(cfg_snmp_group_get(i, &group));

        if ((FALSE == found) && (0 == osal_strlen(group.groupName)))
        {
            *pIndex = i;
            found = TRUE;
        }
    }

    if (FALSE == found)
        *pIndex = SYS_SNMP_SECGROUP_NUM_MAX;

    return SYS_ERR_OK;
}

int32 sal_snmp_groupIndex_get(char *pSecName, uint8 secModel, uint32 *index)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_group_t group;

    SYS_PARAM_CHK((NULL == pSecName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);

    for (i = 0; i < SYS_SNMP_SECGROUP_NUM_MAX; i++)
    {
        SYS_ERR_CHK(cfg_snmp_group_get(i, &group));

        if ((FALSE == found) && (osal_strlen(group.secName) == osal_strlen(pSecName)) &&
            (0 == osal_memcmp(group.secName, pSecName, osal_strlen(pSecName))))
        {
           if(secModel == group.secModel)
           {
             *index = i;
             found = TRUE;
           }
        }
    }

    if (FALSE == found)
        *index = SYS_SNMP_SECGROUP_NUM_MAX;

    return SYS_ERR_OK;
}

int32 sal_snmp_group_add(char *pSecName, uint8 secModel, char *pGroupName)
{
    uint32 index = 0;
    sal_snmp_group_t group;

    SYS_PARAM_CHK((NULL == pSecName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pGroupName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pGroupName) > CAPA_SNMP_GROUP_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(pGroupName) == 0), SYS_ERR_STR_LEN_EMPTY);

    SYS_ERR_CHK(sal_snmp_groupIndex_get(pSecName, secModel, &index));

    if (index < SYS_SNMP_SECGROUP_NUM_MAX)
        return SYS_ERR_DUPLICATE_ENTRY;

    SYS_ERR_CHK(sal_snmp_groupEmpty_get(&index));

    if (SYS_SNMP_SECGROUP_NUM_MAX == index)
        return SYS_ERR_SNMP_GROUP_FULL;

    osal_strcpy(group.groupName, pGroupName);
    osal_strcpy(group.secName, pSecName);
    group.secModel = secModel;

    SYS_ERR_CHK(cfg_snmp_group_set(index, &group));

    return SYS_ERR_OK;
}

int32 sal_snmp_group_del(char *pSecName, uint8 secModel)
{
    uint32 index = 0;
    sal_snmp_group_t group;

    SYS_PARAM_CHK((NULL == pSecName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pSecName) > CAPA_SNMP_GROUP_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_groupIndex_get(pSecName, secModel, &index));

    if (SYS_SNMP_SECGROUP_NUM_MAX == index)
        return SYS_ERR_ENTRY_NOT_FOUND;

    osal_memset(group.groupName, 0, sizeof(group.groupName));
    osal_memset(group.secName, 0, sizeof(group.secName));
    group.secModel = SNMP_SECMODEL_END;
    SYS_ERR_CHK(cfg_snmp_group_set(index, &group));

    return SYS_ERR_OK;
}

int32 sal_snmp_groupBySecurityName_get(char *pSecName, sal_snmp_group_t *pGroup)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_group_t group;

    SYS_PARAM_CHK((NULL == pSecName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

    for (i = 0; i < SYS_SNMP_SECGROUP_NUM_MAX; i++ )
    {
        SYS_ERR_CHK(cfg_snmp_group_get(i, &group));

        if ((FALSE == found) && (osal_strlen(group.secName) == osal_strlen(pSecName)) &&
            (0 == osal_memcmp(group.secName, pSecName, osal_strlen(pSecName))))
        {
            found = TRUE;
            osal_memcpy(pGroup, &group, sizeof(sal_snmp_group_t));
        }
    }

    if(found == FALSE)
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}

int32 sal_snmp_access_get(uint32 index, sal_snmp_access_t *pAccess)
{
    SYS_PARAM_CHK((NULL == pAccess), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= SYS_SNMP_ACCESS_NUM_MAX), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_access_get(index, pAccess));

    return SYS_ERR_OK;
}

int32 sal_snmp_access_set(uint32 index, sal_snmp_access_t *pAccess)
{
    SYS_PARAM_CHK((NULL == pAccess), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= SYS_SNMP_ACCESS_NUM_MAX), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_access_set(index, pAccess));

    return SYS_ERR_OK;
}

static int32 _sal_snmp_manuCfg_accGrpCnt_get(uint32 *pCnt)
{
     uint32 i = 0;
     sal_snmp_access_t access;
     uint32 tmpCnt = 0;

     SYS_PARAM_CHK((NULL == pCnt), SYS_ERR_NULL_POINTER);

    for (i = 0; i < SYS_SNMP_ACCESS_NUM_MAX; i++)
    {
        SYS_MEM_CLEAR(access);
        SYS_ERR_CHK(cfg_snmp_access_get(i, &access));
        if ((0 != osal_strlen(access.groupName)))
        {
            if (0 == osal_memcmp(access.groupName, SNMP_ACCESS_GROUP_PREFIX, osal_strlen(SNMP_ACCESS_GROUP_PREFIX)))
                    continue;
            tmpCnt++;
        }
    }

    *pCnt = tmpCnt;

    return SYS_ERR_OK;
}

int32 sal_snmp_accessEmpty_get(uint8 cfgMode, uint32 *pIndex)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_access_t access;
    uint32 usedNum = 0;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    if (SNMP_CONFIG_MANUAL == cfgMode)
    {
        _sal_snmp_manuCfg_accGrpCnt_get(&usedNum);
        if (usedNum >= CAPA_SNMP_GROUP_NUM)
        {
            *pIndex = SYS_SNMP_ACCESS_NUM_MAX;
            return SYS_ERR_OK;
        }
    }

    for (i = 0; i < SYS_SNMP_ACCESS_NUM_MAX; i++)
    {
        SYS_ERR_CHK(cfg_snmp_access_get(i, &access));

        if ((FALSE == found) && (0 == osal_strlen(access.groupName)))
        {
            *pIndex = i;
            found = TRUE;
        }
    }

    if (FALSE == found)
        *pIndex = SYS_SNMP_ACCESS_NUM_MAX;

    return SYS_ERR_OK;
}

int32 sal_snmp_accessIndex_get(char *pGroupName, char *pContext, uint8 secModel, uint8 secLevel, uint32 *index)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_access_t access;

    SYS_PARAM_CHK((NULL == pGroupName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pContext), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);

    for (i = 0; i < SYS_SNMP_ACCESS_NUM_MAX; i++)
    {
        SYS_ERR_CHK(cfg_snmp_access_get(i, &access));

        if ((FALSE == found) && (osal_strlen(access.groupName) == osal_strlen(pGroupName)) &&
            (0 == osal_memcmp(access.groupName, pGroupName, osal_strlen(pGroupName)))&&
            (osal_strlen(access.context) == osal_strlen(pContext))  &&
            (0 == osal_memcmp(access.context, pContext, osal_strlen(pContext))) &&
            (secModel == access.secMode) && (secLevel == access.secLevel))
        {
             *index = i;
             found = TRUE;
        }
    }

    if (FALSE == found)
        *index = SYS_SNMP_ACCESS_NUM_MAX;

    return SYS_ERR_OK;
}

int32 sal_snmp_accessView_test(sal_snmp_access_t  *access)
{
    uint32 index;

    SYS_PARAM_CHK((NULL == access), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(access->groupName) > CAPA_SNMP_GROUP_NAME_LEN),  SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(access->groupName) == 0),  SYS_ERR_STR_LEN_EMPTY);
    SYS_PARAM_CHK((osal_strlen(access->context) > CAPA_SNMP_CONTEXT_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_viewIndexByName_get(access->readView, &index));

    if (CAPA_SNMP_VIEW_NUM == index)
        return SYS_ERR_READ_VIEW_NOT_EXIST;

    if(0 != osal_strlen(access->writeView))
    {
        SYS_ERR_CHK(sal_snmp_viewIndexByName_get(access->writeView, &index));

        if (CAPA_SNMP_VIEW_NUM == index)
            return SYS_ERR_WRITE_VIEW_NOT_EXIST;
    }

    if(0 != osal_strlen(access->notifyView))
    {
         SYS_ERR_CHK(sal_snmp_viewIndexByName_get(access->notifyView, &index));

         if (CAPA_SNMP_VIEW_NUM == index)
            return SYS_ERR_NOTIFY_VIEW_NOT_EXIST;
    }

    return SYS_ERR_OK;

}

int32 sal_snmp_basic_accessAdd_test(sal_snmp_access_t  *access)
{
    uint32 index;
    sal_snmp_access_t tmpAccess;
    uint32 freeAccessEntryNum = 0;

    SYS_PARAM_CHK((NULL == access), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(access->groupName) > CAPA_SNMP_GROUP_NAME_LEN),  SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(access->groupName) == 0),  SYS_ERR_STR_LEN_EMPTY);
    SYS_PARAM_CHK((osal_strlen(access->context) > CAPA_SNMP_CONTEXT_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_accessIndex_get(access->groupName, access->context,
                                         access->secMode, access->secLevel,&index));

    if (index < SYS_SNMP_ACCESS_NUM_MAX)
        return SYS_ERR_DUPLICATE_ENTRY;

    osal_memset(&tmpAccess, 0, sizeof(tmpAccess) );
    
    for (index = 0; index < SYS_SNMP_ACCESS_NUM_MAX; index++)
    {
        SYS_ERR_CHK(cfg_snmp_access_get(index, &tmpAccess));
        if (0 == osal_strlen(tmpAccess.groupName))
        {
            freeAccessEntryNum++;
            if (2 == freeAccessEntryNum)
                break;
        }
    }

    if (freeAccessEntryNum < 2)
        return SYS_ERR_SNMP_ACCESS_FULL;

    return SYS_ERR_OK;
}

int32 sal_snmp_access_add(sal_snmp_access_t  *access)
{
    uint32 index;

    SYS_PARAM_CHK((NULL == access), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(access->groupName) > CAPA_SNMP_GROUP_NAME_LEN),  SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(access->groupName) == 0),  SYS_ERR_STR_LEN_EMPTY);
    SYS_PARAM_CHK((osal_strlen(access->context) > CAPA_SNMP_CONTEXT_LEN), SYS_ERR_STR_LEN_EXCEED);

    #if 0
    SYS_ERR_CHK(sal_snmp_viewIndexByName_get(access->readView, &index));

    if (CAPA_SNMP_VIEW_NUM == index)
        return SYS_ERR_READ_VIEW_NOT_EXIST;

    if(0 != osal_strlen(access->writeView))
    {
        SYS_ERR_CHK(sal_snmp_viewIndexByName_get(access->writeView, &index));

        if (CAPA_SNMP_VIEW_NUM == index)
            return SYS_ERR_WRITE_VIEW_NOT_EXIST;
    }

    if(0 != osal_strlen(access->notifyView))
    {
         SYS_ERR_CHK(sal_snmp_viewIndexByName_get(access->notifyView, &index));

         if (CAPA_SNMP_VIEW_NUM == index)
            return SYS_ERR_NOTIFY_VIEW_NOT_EXIST;
    }
    #endif

    SYS_ERR_CHK(sal_snmp_accessIndex_get(access->groupName, access->context,
                                         access->secMode, access->secLevel,&index));

    if (index < SYS_SNMP_ACCESS_NUM_MAX)
        return SYS_ERR_DUPLICATE_ENTRY;

    SYS_ERR_CHK(sal_snmp_accessEmpty_get(access->groupcfMode, &index));

    if (index == SYS_SNMP_ACCESS_NUM_MAX)
        return SYS_ERR_SNMP_ACCESS_FULL;

    SYS_ERR_CHK(cfg_snmp_access_set(index, access));
    SYS_ERR_CHK(_sal_snmp_accessSort_update());

    return SYS_ERR_OK;
}

int32 sal_snmp_access_del(char *pGroupName, char *pContext, uint8 secModel, uint8 secLevel)
{
    uint32 index = 0;
    sal_snmp_access_t access;

    SYS_PARAM_CHK((NULL == pGroupName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pContext), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pGroupName) > CAPA_SNMP_GROUP_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_accessIndex_get(pGroupName,pContext, secModel,secLevel, &index));

    if (index == SYS_SNMP_ACCESS_NUM_MAX)
        return SYS_ERR_ENTRY_NOT_FOUND;

    osal_memset(&access,0, sizeof(sal_snmp_access_t));
    osal_memset(access.groupName,0,sizeof(access.groupName));
    access.secMode = SNMP_SECMODEL_END;
    access.secLevel = SNMP_SECLEVEL_END;

    SYS_ERR_CHK(cfg_snmp_access_set(index, &access));

    return SYS_ERR_OK;
}

int32 sal_snmp_accessByIdx_del(uint32 idx)
{
    sal_snmp_access_t access, tmpAccess;

    SYS_PARAM_CHK((idx >=  SYS_SNMP_ACCESS_NUM_MAX), SYS_ERR_ENTRY_INDEX);

    SYS_MEM_CLEAR(access);
    SYS_MEM_CLEAR(tmpAccess);

   SYS_ERR_CHK(cfg_snmp_access_get(idx, &tmpAccess));

   SYS_ERR_CHK(cfg_snmp_access_set(idx, &access));

    return SYS_ERR_OK;
}

int32 sal_snmp_viewNameByGroupName_get(char *pGroup, uint32 access, char *pView)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_access_t Access;
    char viewName[CAPA_SNMP_VIEW_NAME_LEN + 1];

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pView), SYS_ERR_NULL_POINTER);

    osal_memset(viewName, 0, sizeof(viewName));

    for (i = 0; i < SYS_SNMP_ACCESS_NUM_MAX; i++ )
    {
        SYS_ERR_CHK(cfg_snmp_access_get(i, &Access));

        if ((osal_strlen(Access.groupName) == osal_strlen(pGroup)) &&
            (0 == osal_memcmp(Access.groupName, pGroup, osal_strlen(pGroup))) &&
            (Access.secMode < SNMP_SECMODEL_USM))
        {
            found = TRUE;

            osal_memset(viewName, 0, sizeof(viewName));

            if(SNMP_COMMUNITY_ACCESS_READ_ONLY == access)
                osal_strcpy(viewName, Access.readView);
            else if(SNMP_COMMUNITY_ACCESS_READ_WRITE == access)
                osal_strcpy(viewName, Access.writeView);

            if (SNMP_CONFIG_MANUAL == Access.groupcfMode)
                break;
        }
    }

    if(found == FALSE)
        return SYS_ERR_FAILED;

    osal_strcpy(pView, viewName);

    return SYS_ERR_OK;
}

int32 sal_snmp_accessGroupByGroupName_get(char *pGroup, sal_snmp_access_t *pAccess)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_access_t Access;

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pAccess), SYS_ERR_NULL_POINTER);

    for (i = 0; i < SYS_SNMP_ACCESS_NUM_MAX; i++ )
    {
        SYS_ERR_CHK(cfg_snmp_access_get(i, &Access));
        if ((FALSE == found) && (osal_strlen(Access.groupName) == osal_strlen(pGroup)) &&
            (0 == osal_memcmp(Access.groupName, pGroup, osal_strlen(pGroup))))
        {
            found = TRUE;
            osal_memcpy(pAccess, &Access, sizeof(sal_snmp_access_t));
        }
    }

    if(found == FALSE)
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}

int32 sal_snmp_com2sec_get(uint32 index, sal_snmp_com2sec_t *pCom2sec)
{
    SYS_PARAM_CHK((NULL == pCom2sec), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >=  CAPA_SNMP_COMMUNITY_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_com2sec_get(index, pCom2sec));

    return SYS_ERR_OK;
}

int32 sal_snmp_com2sec_set(uint32 index, sal_snmp_com2sec_t *pCom2sec)
{
    SYS_PARAM_CHK((NULL == pCom2sec), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >=  CAPA_SNMP_COMMUNITY_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_com2sec_set(index, pCom2sec));

    return SYS_ERR_OK;
}

int32 sal_snmp_com2secEmpty_get(uint32 *pIndex)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_com2sec_t com2sec;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_com2sec_get(i, &com2sec));

        if ((FALSE == found) &&  (0 == osal_strlen(com2sec.secName)))
        {
            *pIndex = i;
            found = TRUE;
        }
    }

    if (FALSE == found)
        *pIndex = CAPA_SNMP_COMMUNITY_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_com2secIndex_get(char *psecName, uint32 *index)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_com2sec_t com2sec;

    SYS_PARAM_CHK((NULL == psecName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_com2sec_get(i, &com2sec));

        if ((FALSE == found) && (osal_strlen(com2sec.secName) == osal_strlen(psecName)) &&
            (0 == osal_memcmp(com2sec.secName, psecName, osal_strlen(psecName))))
        {
             *index = i;
             found = TRUE;
        }
    }

    if (FALSE == found)
        *index = CAPA_SNMP_COMMUNITY_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_com2sec_add(sal_snmp_com2sec_t *pCom)
{
    uint32 index = 0;

    SYS_PARAM_CHK((NULL == pCom), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pCom->secName) > CAPA_SNMP_SEC_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(pCom->secName) == 0), SYS_ERR_STR_LEN_EMPTY);

    if (SNMP_COM_MODE_ADV == pCom->mode)
        SYS_PARAM_CHK((osal_strlen(pCom->groupName) == 0), SYS_ERR_STR_LEN_EMPTY);

    SYS_ERR_CHK(sal_snmp_com2secIndex_get(pCom->secName, &index));

    if (index < CAPA_SNMP_COMMUNITY_NUM)
        return SYS_ERR_DUPLICATE_ENTRY;

    SYS_ERR_CHK(sal_snmp_com2secEmpty_get(&index));

    if (CAPA_SNMP_COMMUNITY_NUM == index)
        return SYS_ERR_SNMP_COMMUNITY_FULL;

    SYS_ERR_CHK(sal_snmp_com2sec_set(index, pCom));
    SYS_ERR_CHK(_sal_snmp_comSort_update());

    return SYS_ERR_OK;
}

int32 sal_snmp_com2sec_del(char *pSecName)
{
    uint32 index = 0;
    sal_snmp_com2sec_t com2sec;

    SYS_PARAM_CHK((NULL == pSecName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pSecName) > CAPA_SNMP_SEC_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_com2secIndex_get(pSecName, &index));

    if (CAPA_SNMP_COMMUNITY_NUM == index)
        return SYS_ERR_ENTRY_NOT_FOUND;

    osal_memset(com2sec.secName,0,sizeof(com2sec.secName));
    osal_memset(com2sec.comName,0,sizeof(com2sec.comName));
    osal_memset(com2sec.sourceNet,0,sizeof(com2sec.sourceNet));
    com2sec.access = SNMP_COMMUNITY_ACCESS_END;

    SYS_ERR_CHK(cfg_snmp_com2sec_set(index, &com2sec));

    return SYS_ERR_OK;
}

int32 sal_snmp_user_get(uint32 index, sal_snmp_user_t *pUser)
{
    SYS_PARAM_CHK((NULL == pUser), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >=  CAPA_SNMP_USER_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_user_get(index, pUser));

    return SYS_ERR_OK;
}

int32 sal_snmp_userByuserName_get(char *pUserName, sal_snmp_user_t *pUser)
{
    int len ;
    int32 index;
    sal_snmp_user_t user;

    SYS_PARAM_CHK((NULL == pUser), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pUserName), SYS_ERR_NULL_POINTER);

    len = osal_strlen(pUserName);
    if (len == 0)
        return SYS_ERR_INPUT;

    for (index = 0; index < CAPA_SNMP_USER_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_user_get(index, &user));

        if(osal_strlen(user.userName) == len)
        {
            if (0 == osal_memcmp(user.userName, pUserName, len))
            {
                osal_memcpy(pUser, &user, sizeof(sal_snmp_user_t));
                break;
            }
        }
    }

    return SYS_ERR_OK;
}

int32 sal_snmp_user_set(uint32 index, sal_snmp_user_t *pUser)
{
    SYS_PARAM_CHK((NULL == pUser), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >=  CAPA_SNMP_USER_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_user_set(index, pUser));

    return SYS_ERR_OK;
}

int32 sal_snmp_userEmpty_get(uint32 *pIndex)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_user_t user;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_USER_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_user_get(i, &user));

        if ((FALSE == found) &&  (0 == osal_strlen(user.userName)))
        {
            *pIndex = i;
            found = TRUE;
        }
    }

    if (FALSE == found)
        *pIndex = CAPA_SNMP_USER_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_userIndex_get(char *pUserName, uint32 *index)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_user_t user;

    SYS_PARAM_CHK((NULL == pUserName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_USER_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_user_get(i, &user));

        if ((FALSE == found) && (osal_strlen(user.userName) == osal_strlen(pUserName)) &&
            (0 == osal_memcmp(user.userName, pUserName, osal_strlen(pUserName))))
        {
             *index = i;
             found = TRUE;
        }
    }

    if (FALSE == found)
        *index = CAPA_SNMP_USER_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_user_add(sal_snmp_user_t *pUser)
{
    uint32 index = 0;

    SYS_PARAM_CHK((NULL == pUser), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pUser->userName) == 0), SYS_ERR_STR_LEN_EMPTY);
    SYS_PARAM_CHK((osal_strlen(pUser->userName) > CAPA_SNMP_USER_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(pUser->groupName) > CAPA_SNMP_GROUP_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(pUser->authKey) > BOND_SNMP_AUTH_KEY_LEN_MAX), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(pUser->privKey) > BOND_SNMP_PRIV_KEY_LEN_MAX), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_userIndex_get(pUser->userName, &index));

    if (index < CAPA_SNMP_USER_NUM)
        return SYS_ERR_DUPLICATE_ENTRY;

    SYS_ERR_CHK(sal_snmp_userEmpty_get(&index));

    if (CAPA_SNMP_USER_NUM == index)
        return SYS_ERR_SNMP_USER_FULL;

    SYS_ERR_CHK(cfg_snmp_user_set(index, pUser));
    SYS_ERR_CHK(_sal_snmp_userSort_update());

    return SYS_ERR_OK;
}

int32 sal_snmp_user_del(char *pUserName)
{
    uint32 index = 0;
    sal_snmp_user_t user;

    SYS_PARAM_CHK((NULL == pUserName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pUserName) > CAPA_SNMP_USER_NAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_userIndex_get(pUserName, &index));

    if (CAPA_SNMP_USER_NUM == index)
        return SYS_ERR_ENTRY_NOT_FOUND;

    osal_memset(user.userName,0,sizeof(user.userName));
    osal_memset(user.groupName,0,sizeof(user.groupName));
    osal_memset(user.authKey,0,sizeof(user.authKey));
    osal_memset(user.privKey,0,sizeof(user.privKey));
    user.access = SNMP_COMMUNITY_ACCESS_END;
    user.auth_protp = SNMP_SSL_TYPE_END;
    user.encrypt_proto = SNMP_SSL_TYPE_END;
    user.selLevel = SNMP_SECLEVEL_END;

    SYS_ERR_CHK(cfg_snmp_user_set(index, &user));

    return SYS_ERR_OK;
}

int32 sal_snmp_remoteEngine_get(uint32 index, sal_snmp_remoteEngine_t *pRemote)
{
    SYS_PARAM_CHK((NULL == pRemote), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >=  CAPA_SNMP_REMOTE_ENGINEID_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_remoteEngine_get(index, pRemote));

    return SYS_ERR_OK;
}

int32 sal_snmp_remoteByHostName_get(char *pHostName, sal_snmp_remoteEngine_t *pRemote)
{
    int len ;
    int32 index;
    sal_snmp_remoteEngine_t remote;

    SYS_PARAM_CHK((NULL == pHostName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pRemote), SYS_ERR_NULL_POINTER);

    len = osal_strlen(pHostName);
    if (len == 0)
        return SYS_ERR_INPUT;

    for (index = 0; index < CAPA_SNMP_REMOTE_ENGINEID_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_remoteEngine_get(index, &remote));

        if (osal_strlen(remote.server) == len)
        {
            if (0 == osal_memcmp(remote.server, pHostName, len))
            {
                osal_memcpy(pRemote, &remote, sizeof(sal_snmp_remoteEngine_t));
                break;
            }
        }
    }

    return SYS_ERR_OK;
}

int32 sal_snmp_remoteEngine_set(uint32 index, sal_snmp_remoteEngine_t *pRemote)
{
    SYS_PARAM_CHK((NULL == pRemote), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >=  CAPA_SNMP_REMOTE_ENGINEID_NUM), SYS_ERR_ENTRY_INDEX);
    SYS_PARAM_CHK((osal_strlen(pRemote->engineId)%2), SYS_ERR_ENGINEID_LEN_ERR);
    SYS_PARAM_CHK((osal_strlen(pRemote->engineId) < 10) || (osal_strlen(pRemote->engineId) > 64) , SYS_ERR_ENGINEID_LEN_ERR);

    SYS_ERR_CHK(cfg_snmp_remoteEngine_set(index, pRemote));

    return SYS_ERR_OK;
}

int32 sal_snmp_remoteEngineEmpty_get(uint32 *pIndex)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_remoteEngine_t remote;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_REMOTE_ENGINEID_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_remoteEngine_get(i, &remote));

        if ((FALSE == found) &&  (0 == osal_strlen(remote.server)))
        {
            *pIndex = i;
            found = TRUE;
            break;
        }
    }

    if (FALSE == found)
        *pIndex = CAPA_SNMP_REMOTE_ENGINEID_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_remoteEngineIndex_get(char *pHostName, uint32 *index)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_remoteEngine_t remote;

    SYS_PARAM_CHK((NULL == pHostName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_REMOTE_ENGINEID_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_remoteEngine_get(i, &remote));

        if ((FALSE == found) && (osal_strlen(remote.server) == osal_strlen(pHostName)) &&
            (0 == osal_memcmp(remote.server, pHostName, osal_strlen(pHostName))))
        {
             *index = i;
             found = TRUE;
             break;
        }
    }

    if (FALSE == found)
        *index = CAPA_SNMP_REMOTE_ENGINEID_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_remoteEngine_add(sal_snmp_remoteEngine_t *pRemote)
{
    uint32 index = 0;

    SYS_PARAM_CHK((NULL == pRemote), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pRemote->server) == 0), SYS_ERR_STR_LEN_EMPTY);
    SYS_PARAM_CHK((osal_strlen(pRemote->server) > CAPA_HOSTNAME_LEN), SYS_ERR_STR_LEN_EXCEED);
    SYS_PARAM_CHK((osal_strlen(pRemote->engineId)%2), SYS_ERR_ENGINEID_LEN_ERR);
    SYS_PARAM_CHK((osal_strlen(pRemote->engineId) < BOND_SNMP_ENGINEID_LEN_MIN) || (osal_strlen(pRemote->engineId) > BOND_SNMP_ENGINEID_LEN_MAX) , SYS_ERR_ENGINEID_LEN_ERR);

    SYS_ERR_CHK(sal_snmp_remoteEngineIndex_get(pRemote->server, &index));

    if (index < CAPA_SNMP_REMOTE_ENGINEID_NUM)
        return SYS_ERR_DUPLICATE_ENTRY;

    SYS_ERR_CHK(sal_snmp_remoteEngineEmpty_get(&index));

    if (CAPA_SNMP_REMOTE_ENGINEID_NUM == index)
        return SYS_ERR_SNMP_REMOTE_FULL;

    SYS_ERR_CHK(sal_snmp_remoteEngine_set(index, pRemote));
    SYS_ERR_CHK(_sal_snmp_rmtEngineSort_update());

    return SYS_ERR_OK;
}

int32 sal_snmp_remoteEngine_del(char *pHostName)
{
    uint32 index = 0;
    sal_snmp_remoteEngine_t remote;

    SYS_PARAM_CHK((NULL == pHostName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pHostName) > CAPA_HOSTNAME_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_remoteEngineIndex_get(pHostName, &index));

    if (CAPA_SNMP_REMOTE_ENGINEID_NUM == index)
        return SYS_ERR_ENTRY_NOT_FOUND;

    osal_memset(&remote, 0,sizeof(sal_snmp_remoteEngine_t));

    SYS_ERR_CHK(cfg_snmp_remoteEngine_set(index, &remote));

    return SYS_ERR_OK;
}

int32 sal_snmp_engineid_get(char *pEngineid)
{
    SYS_PARAM_CHK((NULL == pEngineid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_snmp_engineid_get(pEngineid));

    return SYS_ERR_OK;
}

int32 sal_snmp_defaultEngineId_get(char *pEngineid)
{
    sys_mac_t sysMacAddr;
    char engineStr[BOND_SNMP_ENGINEID_LEN_MAX + 1];

    SYS_PARAM_CHK((NULL == pEngineid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_sys_macAddr_get(&sysMacAddr));

    osal_memset(engineStr, 0, sizeof(engineStr));
    osal_sprintf(engineStr,"80%s03%02X%02X%02X%02X%02X%02X",  DFLT_SNMP_ENTERPRISE_NUM,
                                                                                          sysMacAddr.octet[0],
                                                                                          sysMacAddr.octet[1],
                                                                                          sysMacAddr.octet[2],
                                                                                          sysMacAddr.octet[3],
                                                                                          sysMacAddr.octet[4],
                                                                                          sysMacAddr.octet[5]);
    SYS_ERR_CHK(sal_util_str2lowerStr_xlate(engineStr));
    osal_memcpy(pEngineid, engineStr, osal_strlen(engineStr));
    return SYS_ERR_OK;
}

int32 sal_snmp_engineid_set(char * pEngineid)
{
    SYS_PARAM_CHK((NULL == pEngineid), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pEngineid)%2), SYS_ERR_ENGINEID_LEN_ERR);
    SYS_PARAM_CHK((osal_strlen(pEngineid) < 10) || (osal_strlen(pEngineid) > 64) , SYS_ERR_ENGINEID_LEN_ERR);

    SYS_ERR_CHK(sal_util_str2lowerStr_xlate(pEngineid));
    SYS_ERR_CHK(cfg_snmp_engineid_set(pEngineid));

    return SYS_ERR_OK;
}

int32 sal_snmp_confFile_update(void)
{
    char    oidStr[CAPA_SNMP_OID_STR_LEN];
    char    viewMaskStr[4];
    char    writeViewName[CAPA_SNMP_VIEW_NAME_LEN +1];
    char    notifyViewName[CAPA_SNMP_VIEW_NAME_LEN +1];
    char    engineId[BOND_SNMP_ENGINEID_LEN_MAX+1];
    char    authPrivStr[SYS_BUF128_LEN];
    char    remoteEngineStr[SYS_BUF64_LEN + SYS_BUF8_LEN];
    char    hostSever[CAPA_HOSTNAME_LEN+SYS_BUF8_LEN];
    uint32  index;
    FILE    *pSnmpdConf = NULL;
    sal_snmp_com2sec_t com2sec;
    sal_snmp_group_t  group;
    sal_snmp_view_t view;
    sal_snmp_access_t access;
    sal_snmp_target_t  host;
    sal_snmp_user_t    user;
    sal_snmp_remoteEngine_t remote;
    uint8 len;
    int32 ret = SYS_ERR_OK;

    pSnmpdConf = fopen(SNMP_CONF_FILE, "wt");

    if (NULL == pSnmpdConf)
        return SYS_ERR_FILE_OPEN;

    fprintf(pSnmpdConf, "#         sec.name       source       community \n");

    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        ret = sal_snmp_com2sec_get(index, &com2sec);

        if (SYS_ERR_OK != ret)
        {
            fclose(pSnmpdConf);
            return ret;
        }

        if (0 != osal_strlen(com2sec.comName))
        {
            fprintf(pSnmpdConf, "com2sec  \"%s\"    %s    \"%s\" \n",
                        com2sec.secName,
                        com2sec.sourceNet,
                        com2sec.comName);

#ifdef CONFIG_SYS_IPV6
            fprintf(pSnmpdConf, "com2sec6  \"%s\"    %s     \"%s\" \n",
                        com2sec.secName,
                        com2sec.sourceNet,
                        com2sec.comName);
#endif
        }
    }

    fprintf(pSnmpdConf, "\n\n#         ip address    community \n");

    for(index = 0; index < CAPA_SNMP_HOST_NUM; index++)
    {
        ret = sal_snmp_target_get(index, &host);

        if (SYS_ERR_OK != ret)
        {
            fclose(pSnmpdConf);
            return ret;
        }

        if(0 != osal_strlen(host.server))
        {
             SYS_MEM_CLEAR(hostSever);
             if (IS_IPV6_ADDRSTR_VALID(host.server))
                osal_sprintf(hostSever,"udp6:[%s]", host.server);
             else
                osal_strcpy(hostSever, host.server);
             if (host.trapType < SNMP_TRAP_V3)
             {
                 if (SNMP_INFORM_V2 == host.trapType)
                    fprintf(pSnmpdConf, "%s  -r %d -t %d  -Ci -v 2c -u \"%s\"  %s:%d \n", gtrapStr[host.trapType],
                                                                                                            host.retries,
                                                                                                            host.timeout,
                                                                                                            host.name,
                                                                                                            hostSever,
                                                                                                            host.portNum);
                 else
                    fprintf(pSnmpdConf, "%s    %s:%d    \"%s\" \n", gtrapStr[host.trapType], hostSever, host.portNum ,host.name);
             }
             else
             {
                SYS_MEM_CLEAR(authPrivStr);
                SYS_MEM_CLEAR(user);

                ret = sal_snmp_userByuserName_get(host.name, &user);
                if (SYS_ERR_OK != ret)
                {
                    fclose(pSnmpdConf);
                    return ret;
                }

                if (0 != osal_strlen(user.userName))
                {
                    if (SNMP_SECLEVEL_AUTHNOPRIV == host.secLevel)
                    {
                        osal_sprintf(authPrivStr, "-a %s -A %s ", snmpv3_ssl_type[user.auth_protp], user.authKey);
                    }
                    else if (SNMP_SECLEVEL_AUTHPRIV == host.secLevel)
                    {
                        osal_sprintf(authPrivStr, "-a %s -A %s -x %s -X %s ", snmpv3_ssl_type[user.auth_protp], user.authKey,
                                                                                                         snmpv3_ssl_type[user.encrypt_proto], user.privKey);
                    }
                }

                if (SNMP_TRAP_V3 == host.trapType)
                    fprintf(pSnmpdConf, "%s  -v 3 -l %s -u \"%s\" %s %s:%d  \n", gtrapStr[host.trapType],
                                                                                                          gv3privOption[host.secLevel],
                                                                                                          host.name,
                                                                                                          authPrivStr,
                                                                                                          hostSever,
                                                                                                          host.portNum);
                else
                {
                    SYS_MEM_CLEAR(remoteEngineStr);
                    SYS_MEM_CLEAR(remote);

                    ret = sal_snmp_remoteByHostName_get(host.server, &remote);
                    if (SYS_ERR_OK != ret)
                    {
                        fclose(pSnmpdConf);
                        return ret;
                    }

                    if (0 != osal_strlen(remote.server))
                    {
                        osal_sprintf(remoteEngineStr, "-e 0x%s", remote.engineId);
                    }

                    fprintf(pSnmpdConf, "%s  -r %d -t %d -Ci -v 3 -l %s -u \"%s\" %s %s %s:%d  \n",  gtrapStr[host.trapType],
                                                                                                                                    host.retries,
                                                                                                                                    host.timeout,
                                                                                                                                     gv3privOption[host.secLevel],
                                                                                                                                     host.name,
                                                                                                                                     remoteEngineStr,
                                                                                                                                     authPrivStr,
                                                                                                                                     hostSever,
                                                                                                                                     host.portNum);
                }
             }
        }
    }

    fprintf(pSnmpdConf, "\n\n#        groupName securityModel securityName \n");

    for (index = 0; index < SYS_SNMP_SECGROUP_NUM_MAX; index++)
    {
        ret = sal_snmp_group_get(index, &group);

        if (SYS_ERR_OK != ret)
        {
            fclose(pSnmpdConf);
            return ret;
        }

        if (osal_strlen(group.groupName) != 0)
        {
            fprintf(pSnmpdConf, "group  \"%s\"    %s    \"%s\"   \n",
                        group.groupName,
                        ggroupSecModel[group.secModel],
                        group.secName);
        }
    }

    fprintf(pSnmpdConf, "\n\n#       name           incl/excl     subtree         mask(optional)  \n");
    for (index = 0; index < CAPA_SNMP_VIEW_NUM; index++)
    {
        ret = sal_snmp_view_get(index, &view);

        if (SYS_ERR_OK != ret)
        {
            fclose(pSnmpdConf);
            return ret;
        }

        ret = sal_util_oid2str(view.subtreeOid.len, view.subtreeOid.id, oidStr);
        if (SYS_ERR_OK != ret)
        {
            fclose(pSnmpdConf);
            return ret;
        }

        if(osal_strlen(view.viewName) != 0)
        {
            osal_memset(viewMaskStr, 0, sizeof(viewMaskStr));
            if (osal_strlen(view.viewMaskStr) > 0)
            {
               ret = _sal_snmp_mask2hexStr(view.viewMaskStr, viewMaskStr);
               if (SYS_ERR_OK != ret)
               {
                   fclose(pSnmpdConf);
                   return ret;
               }

               len = osal_strlen(viewMaskStr);
               if (len%2)
               {
                    if (len < 4)
                        viewMaskStr[len] = '0';
               }
            }
            fprintf(pSnmpdConf, "view    \"%s\"      %s     %s %s \n",
                        view.viewName,
                        gviewTypeStr[view.viewType],
                        oidStr,
                        viewMaskStr);
        }
    }

    fprintf(pSnmpdConf, "\n\n#     group   context  sec.model  sec.level  prefix  read   write   notify  \n");
    for (index = 0; index < SYS_SNMP_ACCESS_NUM_MAX; index++)
    {
        ret = sal_snmp_access_get(index, &access);
        if (SYS_ERR_OK != ret)
        {
            fclose(pSnmpdConf);
            return ret;
        }

        if(0 != osal_strlen(access.groupName))
        {
            if(0 == osal_strlen(access.notifyView))
                osal_strcpy(notifyViewName, "none");
            else
                osal_strcpy(notifyViewName, access.notifyView);

            if(0 == osal_strlen(access.writeView))
                osal_strcpy(writeViewName, "none");
            else
                osal_strcpy(writeViewName, access.writeView);

            fprintf(pSnmpdConf, "access    \"%s\"   %s  %s  %s %s \"%s\"  \"%s\"  \"%s\"  \n",
                                             access.groupName,
                                             "\"\"",
                                             ggroupSecModel[access.secMode],
                                             gsecLevel[access.secLevel],
                                             gprefix[access.prefix],
                                             access.readView,
                                             writeViewName,
                                             notifyViewName);
        }
    }

      fprintf(pSnmpdConf, "\n\n#     userName  auth-proto  passwd  encrypt-proto key \n");

      for (index = 0; index < CAPA_SNMP_USER_NUM; index++)
      {
         ret = cfg_snmp_user_get(index, &user);
         if (SYS_ERR_OK != ret)
         {
             fclose(pSnmpdConf);
             return ret;
         }

         if (0 != osal_strlen(user.userName))
         {
            if (0 == osal_strlen(user.authKey))
            {
                fprintf(pSnmpdConf, "createUser \"%s\"  \n", user.userName);
            }
            else
            {
                if (0 == osal_strlen(user.privKey))
                {
                    fprintf(pSnmpdConf, "createUser \"%s\" %s \"%s\" %s  %s \n",
                            user.userName,
                            (SNMP_SSL_TYPE_NONE != user.auth_protp) ? snmpv3_ssl_type[user.auth_protp] : "",
                            user.authKey,
                            (SNMP_SSL_TYPE_NONE != user.encrypt_proto) ? snmpv3_ssl_type[user.encrypt_proto] : "",
                            user.privKey);
                }
                else
                {
                    fprintf(pSnmpdConf, "createUser \"%s\" %s \"%s\" %s  \"%s\" \n",
                            user.userName,
                            (SNMP_SSL_TYPE_NONE != user.auth_protp) ? snmpv3_ssl_type[user.auth_protp] : "",
                            user.authKey,
                            (SNMP_SSL_TYPE_NONE != user.encrypt_proto) ? snmpv3_ssl_type[user.encrypt_proto] : "",
                            user.privKey);
                }
            }

            fprintf(pSnmpdConf, "%s \"%s\" %s \n\n",
                    (SNMP_COMMUNITY_ACCESS_READ_ONLY == user.access) ? "rouser" : "rwuser",
                    user.userName,
                    gsecLevel[user.selLevel]);
        }
    }

    SYS_MEM_CLEAR(engineId);
    ret = sal_snmp_engineid_get(engineId);
    if (SYS_ERR_OK != ret)
    {
        fclose(pSnmpdConf);
        return ret;
    }
    
    fprintf(pSnmpdConf, "oldEngineID  %s\n\n", engineId);

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    fprintf(pSnmpdConf, "com2sec  \"%s\"    default    \"%s\" \n", sal_rmon_cstr_ret(),sal_rmon_cstr_ret());
#ifdef CONFIG_SYS_IPV6
    fprintf(pSnmpdConf, "com2sec6  \"%s\"    default     \"%s\" \n", sal_rmon_cstr_ret(),sal_rmon_cstr_ret());
#endif
    fprintf(pSnmpdConf, "group   \"RMONMGMTUIGROUP\"      v1      \"%s\"   \n",sal_rmon_cstr_ret());
    fprintf(pSnmpdConf, "access    \"RMONMGMTUIGROUP\"     \"\"    v1   noauth  exact  \"%s\"  \"%s\"  \"none\"  \n", DFLT_SNMP_VIEW_STR, DFLT_SNMP_VIEW_STR);
#endif

    fclose(pSnmpdConf);

    return SYS_ERR_OK;
}

uint32 sal_util_str2oid(char *pStr, uint32 *len, uint32 *id)
{
    int32 ilen, i, ndot = 0;
    char buf[128] = {0};
    uint32 strLen;
    char *ptr;
    uint8  idBits = 0;

    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == len), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == id), SYS_ERR_NULL_POINTER);

    ptr = pStr;
    strLen = osal_strlen(pStr);

    for(i = 0; i < strLen; i++)
    {
        if((i == strLen -1) && (pStr[i] == '.'))
            break;
        if(pStr[i] == '.')
        {
            SYS_PARAM_CHK(((pStr[i+1]) == '.'), SYS_ERR_OID);
            ndot++;
            idBits = 0;
         }
        else
         {
            SYS_PARAM_CHK((isdigit(pStr[i]) == 0), SYS_ERR_OID);
            idBits++;
            SYS_PARAM_CHK((idBits > 8),SYS_ERR_OID);
         }
        SYS_PARAM_CHK((ndot > CAPA_SNMP_VIEW_SUBTREE_LEN),SYS_ERR_OID_LEN);
    }

   if(pStr[0] == '.')
   {
        ndot--;
        ptr = ptr + 1;
   }

    *len = ndot + 1;

    for(i = 0; i < ndot; i++)
    {
        SYS_ERR_CHK(sys_util_getnext(ptr, '.', buf, &ilen));
        id[i] = SYS_STR2UINT(buf);
        ptr += ilen;
    }

    id[i] = SYS_STR2UINT(ptr);

    return SYS_ERR_OK;
}

int32 sal_util_oid2str(uint32 len, uint32 *id,  char *pStr)
{
   char  *p;
   uint32 i;

   SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);

   p = pStr;
   for(i = 0; i < len; i++)
        p += osal_sprintf(p,".%u", id[i]);

   return SYS_ERR_OK;
}

int32 sal_util_check_hex(const char *pHex)
{
    uint32 len, i;

    SYS_PARAM_CHK((NULL == pHex), SYS_ERR_NULL_POINTER);
    len = osal_strlen(pHex);
    for(i = 0; i < len; i++)
    {
        if( !((pHex[i] >= '0' && pHex[i] <= '9') ||
            (pHex[i] >= 'a' && pHex[i] <= 'f') ||
            (pHex[i] >= 'A' && pHex[i] <= 'F')) )
        {
            return SYS_ERR_FAILED;
        }
    }

    return SYS_ERR_OK;
}

static int32 _sal_snmp_mask2hexStr(const char *pBin , char *pHex)
{
    uint32 len, i, j;
    uint8 tmp;
    char string[2];

    SYS_PARAM_CHK((NULL == pBin), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pHex), SYS_ERR_NULL_POINTER);

    len = osal_strlen(pBin);
    for(i = 0; i < len/4; i++)
    {
        tmp = 0;
        for (j = 0; j < 4; j++)
        {
            tmp = tmp | ((pBin[i*4 + j] - '0') << (3 - j));
        }
        osal_sprintf(string, "%x", tmp);
        osal_strcat(pHex, string);
    }

    if (len%4)
    {
        tmp = 0;
        for (j = 0; j < len%4; j++)
        {
            tmp = tmp | ((pBin[(len/4) * 4 + j] - '0') << (3 - j));
        }
        osal_sprintf(string, "%x", tmp);
        osal_strcat(pHex, string);
    }
    return SYS_ERR_OK;
}

int32 sal_util_snmpv3TrapHeader_get(sal_snmp_target_t *pHost, char *pHeader, int32 len)
{
    char  tmp[MAX_SNMPV3_TRAP_HEAD_LEN];
    char  trapEngineStr[144];
    sal_snmp_user_t user;
    sal_snmp_remoteEngine_t remote;
    char engineId[BOND_SNMP_ENGINEID_LEN_MAX+1];

    SYS_PARAM_CHK((NULL == pHost), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pHeader), SYS_ERR_NULL_POINTER);

    if (SNMP_SECMODEL_USM != pHost->secModel)
        return SYS_ERR_FAILED;

    osal_memset(tmp, 0, sizeof(tmp));
    osal_memset(trapEngineStr, 0, sizeof(trapEngineStr));
    osal_memset(&remote, 0, sizeof(sal_snmp_remoteEngine_t));

    osal_memset(&user, 0, sizeof(sal_snmp_user_t) );
    SYS_ERR_CHK(sal_snmp_userByuserName_get(pHost->name, &user));
    if (0 == osal_strlen(user.userName))
        return SYS_ERR_OK;

    if (user.selLevel != pHost->secLevel)
        return SYS_ERR_OK;

    osal_memset(engineId, 0, sizeof(engineId));
    SYS_ERR_CHK(sal_snmp_engineid_get(engineId));

     if (SNMP_TRAP_V3 == pHost->trapType)
    {

        osal_sprintf(trapEngineStr, " -e 0x%s -v 3 -u %s -E 0x%s ", engineId, pHost->name, engineId);

        if (SNMP_SECLEVEL_NOAUTHNOPRIV == pHost->secLevel)
            osal_sprintf(tmp, "snmptrap %s ",  trapEngineStr);
        else if (SNMP_SECLEVEL_AUTHNOPRIV == pHost->secLevel)
            osal_sprintf(tmp, "snmptrap %s  -a %s -A %s -l authNoPriv",  trapEngineStr,
                                                                                                       (user.auth_protp == SNMP_SSL_TYPE_MD5) ? "MD5" : "SHA",
                                                                                                       user.authKey);
        else if (SNMP_SECLEVEL_AUTHPRIV == pHost->secLevel)
            osal_sprintf(tmp, "snmptrap %s -a %s -A %s  -x DES -X  %s -l authPriv",
                                                      trapEngineStr,
                                                      (user.auth_protp == SNMP_SSL_TYPE_MD5) ? "MD5" : "SHA",
                                                      user.authKey,
                                                      user.privKey);

    }
    else
    {
        SYS_ERR_CHK(sal_snmp_remoteByHostName_get(pHost->server, &remote));
        if (0 != osal_strlen(remote.server))
        {
            osal_sprintf(trapEngineStr, " -e 0x%s -v 3 -u %s -E 0x%s ", remote.engineId, pHost->name, engineId);
        }
        else
        {
            osal_sprintf(trapEngineStr, " -e 0x%s -v 3 -u %s -E 0x%s ",engineId, pHost->name, engineId);
        }

        if (SNMP_SECLEVEL_NOAUTHNOPRIV == pHost->secLevel)
            osal_sprintf(tmp, "snmptrap  -Ci %s -r %d -t %d",  trapEngineStr, pHost->retries, pHost->timeout);
        else if (SNMP_SECLEVEL_AUTHNOPRIV == pHost->secLevel)
            osal_sprintf(tmp, "snmptrap  -Ci %s -r %d -t %d -a %s -A %s -l authNoPriv",  trapEngineStr,
                                                                                                                                pHost->retries,
                                                                                                                                pHost->timeout,
                                                                                                                                (user.auth_protp == SNMP_SSL_TYPE_MD5) ? "MD5" : "SHA",
                                                                                                                                user.authKey);
        else if (SNMP_SECLEVEL_AUTHPRIV == pHost->secLevel)
            osal_sprintf(tmp, "snmptrap  -Ci %s -r %d -t %d -a %s -A %s  -x DES -X  %s -l authPriv",
                                                      trapEngineStr,
                                                      pHost->retries,
                                                      pHost->timeout,
                                                      (user.auth_protp == SNMP_SSL_TYPE_MD5) ? "MD5" : "SHA",
                                                      user.authKey,
                                                      user.privKey);

    }

    if (len < osal_strlen(tmp))
        return SYS_ERR_FAILED;

    osal_memcpy(pHeader, tmp, osal_strlen(tmp));

    return SYS_ERR_OK;

}

int32 _sal_snmp_viewSort_update(void)
{
    int i, j, entryNum = 0;
    sal_snmp_view_t viewArry[CAPA_SNMP_VIEW_NUM];
    sal_snmp_view_t tmpView;

    SYS_MEM_CLEAR(viewArry);

    for (i = 0; i < CAPA_SNMP_VIEW_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpView);
        SYS_ERR_CONTINUE(sal_snmp_view_get(i, &tmpView));
        if (0 == osal_strlen(tmpView.viewName))
            continue;

        osal_memcpy(&viewArry[entryNum], &tmpView, sizeof(sal_snmp_view_t)) ;
        entryNum++;
    }

    if (entryNum > 1)
    {
        for (j = 0; j < entryNum; j++)
        {
            for(i = 0; i < entryNum - j; i++)
            {
                if (0 == osal_strlen(viewArry[i+1].viewName))
                    continue;

                if (osal_strcmp(viewArry[i].viewName, viewArry[i+1].viewName) > 0)
                {
                    osal_memcpy(&tmpView, &viewArry[i], sizeof(sal_snmp_view_t)) ;
                    osal_memcpy(&viewArry[i], &viewArry[i+1], sizeof(sal_snmp_view_t)) ;
                    osal_memcpy(&viewArry[i+1], &tmpView, sizeof(sal_snmp_view_t)) ;
                }
             }
        }
    }
    SYS_ERR_CHK(cfg_snmp_viewArry_set(viewArry, entryNum));
    return SYS_ERR_OK;
}

int32 _sal_snmp_comSort_update(void)
{
    int i, j, entryNum = 0;
    sal_snmp_com2sec_t comArry[CAPA_SNMP_COMMUNITY_NUM];
    sal_snmp_com2sec_t tmpCom;

    SYS_MEM_CLEAR(comArry);

    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpCom);
        SYS_ERR_CONTINUE(sal_snmp_com2sec_get(i, &tmpCom));
        if (0 == osal_strlen(tmpCom.comName))
            continue;

        osal_memcpy(&comArry[entryNum], &tmpCom, sizeof(sal_snmp_com2sec_t)) ;
        entryNum++;
    }

   if (entryNum > 1)
   {
        for (j = 0; j < entryNum; j++)
        {
            for(i = 0; i < entryNum - j; i++)
            {
                if (0 == osal_strlen(comArry[i+1].comName))
                    continue;

                if (osal_strcmp(comArry[i].comName, comArry[i+1].comName) > 0)
                {
                    osal_memcpy(&tmpCom, &comArry[i], sizeof(sal_snmp_com2sec_t)) ;
                    osal_memcpy(&comArry[i], &comArry[i+1], sizeof(sal_snmp_com2sec_t)) ;
                    osal_memcpy(&comArry[i+1], &tmpCom, sizeof(sal_snmp_com2sec_t)) ;
                }
             }
        }
    }

    SYS_ERR_CHK(cfg_snmp_comArry_set(comArry, entryNum));
    return SYS_ERR_OK;
}

int32 _sal_snmp_accessSort_update(void)
{
    int i, j, entryNum = 0;
    sal_snmp_access_t accessArry[SYS_SNMP_ACCESS_NUM_MAX];
    sal_snmp_access_t tmpAccess;

    SYS_MEM_CLEAR(accessArry);

    for (i = 0; i < SYS_SNMP_ACCESS_NUM_MAX; i++)
    {
        SYS_MEM_CLEAR(tmpAccess);
        SYS_ERR_CONTINUE(sal_snmp_access_get(i, &tmpAccess));
        if (0 == osal_strlen(tmpAccess.groupName))
            continue;

        osal_memcpy(&accessArry[entryNum], &tmpAccess, sizeof(sal_snmp_access_t)) ;
        entryNum++;
    }

   if (entryNum > 1)
   {
        for (j = 0; j < entryNum; j++)
        {
            for(i = 0; i < entryNum - j; i++)
            {
                if (0 == osal_strlen(accessArry[i+1].groupName))
                    continue;

                if (osal_strcmp(accessArry[i].groupName, accessArry[i+1].groupName) > 0)
                {
                    osal_memcpy(&tmpAccess, &accessArry[i], sizeof(sal_snmp_access_t)) ;
                    osal_memcpy(&accessArry[i], &accessArry[i+1], sizeof(sal_snmp_access_t)) ;
                    osal_memcpy(&accessArry[i+1], &tmpAccess, sizeof(sal_snmp_access_t)) ;
                }
             }
        }
    }

    SYS_ERR_CHK(cfg_snmp_groupArry_set(accessArry, entryNum));
    return SYS_ERR_OK;
}

int32 _sal_snmp_userSort_update(void)
{
    int i, j, entryNum = 0;
    sal_snmp_user_t userArry[CAPA_SNMP_USER_NUM];
    sal_snmp_user_t tmpUser;

    SYS_MEM_CLEAR(userArry);

    for (i = 0; i < CAPA_SNMP_USER_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpUser);
        SYS_ERR_CONTINUE(sal_snmp_user_get(i, &tmpUser));
        if (0 == osal_strlen(tmpUser.userName))
            continue;

        osal_memcpy(&userArry[entryNum], &tmpUser, sizeof(sal_snmp_user_t)) ;
        entryNum++;
    }

   if (entryNum > 1)
   {
        for (j = 0; j < entryNum; j++)
        {
            for(i = 0; i < entryNum - j; i++)
            {
                if (0 == osal_strlen(userArry[i+1].userName))
                    continue;

                if (osal_strcmp(userArry[i].userName, userArry[i+1].userName) > 0)
                {
                    osal_memcpy(&tmpUser, &userArry[i], sizeof(sal_snmp_user_t)) ;
                    osal_memcpy(&userArry[i], &userArry[i+1], sizeof(sal_snmp_user_t)) ;
                    osal_memcpy(&userArry[i+1], &tmpUser, sizeof(sal_snmp_user_t)) ;
                }
             }
        }
    }

    SYS_ERR_CHK(cfg_snmp_userArry_set(userArry, entryNum));
    return SYS_ERR_OK;
}

int32 _sal_snmp_rmtEngineSort_update(void)
{
    int i, j, entryNum = 0;
    sal_snmp_remoteEngine_t rmtArry[CAPA_SNMP_REMOTE_ENGINEID_NUM];
    sal_snmp_remoteEngine_t tmpRmt;

    SYS_MEM_CLEAR(rmtArry);

    for (i = 0; i < CAPA_SNMP_REMOTE_ENGINEID_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpRmt);
        SYS_ERR_CONTINUE(sal_snmp_remoteEngine_get(i, &tmpRmt));
        if (0 == osal_strlen(tmpRmt.server))
            continue;

        osal_memcpy(&rmtArry[entryNum], &tmpRmt, sizeof(sal_snmp_remoteEngine_t)) ;
        entryNum++;
    }

   if (entryNum > 1)
   {
        for (j = 0; j < entryNum; j++)
        {
            for(i = 0; i < entryNum - j; i++)
            {
                if (0 == osal_strlen(rmtArry[i+1].server))
                    continue;

                if (osal_strcmp(rmtArry[i].server, rmtArry[i+1].server) > 0)
                {
                    osal_memcpy(&tmpRmt, &rmtArry[i], sizeof(sal_snmp_remoteEngine_t)) ;
                    osal_memcpy(&rmtArry[i], &rmtArry[i+1], sizeof(sal_snmp_remoteEngine_t)) ;
                    osal_memcpy(&rmtArry[i+1], &tmpRmt, sizeof(sal_snmp_remoteEngine_t)) ;
                }
             }
        }
    }

    SYS_ERR_CHK(cfg_snmp_rmtEngineArry_set(rmtArry, entryNum));
    return SYS_ERR_OK;
}
#else

int32 sal_snmp_community_set(uint32 index, sal_snmp_community_t *pCommunity)
{
    SYS_PARAM_CHK((NULL == pCommunity), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= CAPA_SNMP_COMMUNITY_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_community_set(index, pCommunity));

    return SYS_ERR_OK;
}

int32 sal_snmp_community_get(uint32 index, sal_snmp_community_t *pCommunity)
{
    SYS_PARAM_CHK((NULL == pCommunity), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= CAPA_SNMP_COMMUNITY_NUM), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_community_get(index, pCommunity));

    return SYS_ERR_OK;
}

int32 sal_snmp_communityEmpty_get(uint32 *pIndex)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_community_t community;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_community_get(i, &community));

        if ((FALSE == found) && (0 == osal_strlen(community.name)))
        {
            *pIndex = i;
            found = TRUE;
        }
    }

    if (FALSE == found)
        *pIndex = CAPA_SNMP_COMMUNITY_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_communityIndex_get(char *pCommunity, uint32 *index)
{
    uint32 i = 0;
    uint32 found = FALSE;
    sal_snmp_community_t community;

    SYS_PARAM_CHK((NULL == pCommunity), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == index), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SNMP_COMMUNITY_NUM; i++)
    {
        SYS_ERR_CHK(cfg_snmp_community_get(i, &community));

        if ((FALSE == found) && (osal_strlen(community.name) == osal_strlen(pCommunity)) &&
            (0 == osal_memcmp(community.name, pCommunity, osal_strlen(pCommunity))))
        {
            *index = i;
            found = TRUE;
        }
    }

    if (FALSE == found)
        *index = CAPA_SNMP_COMMUNITY_NUM;

    return SYS_ERR_OK;
}

int32 sal_snmp_community_add(char *pCommunity, uint32 access)
{
    uint32 index = 0;
    sal_snmp_community_t community;

    SYS_PARAM_CHK((NULL == pCommunity), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pCommunity) > CAPA_SNMP_COMMUNITY_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_communityIndex_get(pCommunity, &index));

    if (index < CAPA_SNMP_COMMUNITY_NUM)
        return SYS_ERR_DUPLICATE_ENTRY;

    SYS_ERR_CHK(sal_snmp_communityEmpty_get(&index));

    if (CAPA_SNMP_COMMUNITY_NUM == index)
        return SYS_ERR_SNMP_COMMUNITY_FULL;

    osal_strcpy(community.name, pCommunity);
    community.access = access;

    SYS_ERR_CHK(cfg_snmp_community_set(index, &community));

    return SYS_ERR_OK;
}

int32 sal_snmp_community_del(char *pCommunity)
{
    uint32 index = 0;
    sal_snmp_community_t community;

    SYS_PARAM_CHK((NULL == pCommunity), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((osal_strlen(pCommunity) > CAPA_SNMP_COMMUNITY_LEN), SYS_ERR_STR_LEN_EXCEED);

    SYS_ERR_CHK(sal_snmp_communityIndex_get(pCommunity, &index));

    if (CAPA_SNMP_COMMUNITY_NUM == index)
        return SYS_ERR_ENTRY_NOT_FOUND;

    community.name[0] = '\0';
    community.access = SNMP_COMMUNITY_ACCESS_END;
    SYS_ERR_CHK(cfg_snmp_community_set(index, &community));

    return SYS_ERR_OK;
}

int32 sal_snmp_confFile_update(void)
{
    char    name[CAPA_SYS_NAME_LEN];
    char    location[CAPA_SYS_LOCATION_LEN];
    char    contact[CAPA_SYS_CONTACT_LEN];

    uint32  index = 0;
    FILE    *pSnmpdConf = NULL;
    sal_snmp_community_t community;
    sal_snmp_target_t host;

    pSnmpdConf = fopen(SNMP_CONF_FILE, "wt");

    if (NULL == pSnmpdConf)
        return SYS_ERR_FILE_OPEN;

    SYS_ERR_CHK(sal_sys_systemName_get(name));
    SYS_ERR_CHK(sal_sys_systemLocation_get(location));
    SYS_ERR_CHK(sal_sys_systemContact_get(contact));
    fprintf(pSnmpdConf, snmpdConfData[SNMPD_CONF_SYS_NAME], name);
    fprintf(pSnmpdConf, snmpdConfData[SNMPD_CONF_SYS_LOCATION], location);
    fprintf(pSnmpdConf, snmpdConfData[SNMPD_CONF_SYS_CONTACT], contact);

    fprintf(pSnmpdConf, "#         sec.name       source       community \n");

    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_ERR_CHK(cfg_snmp_community_get(index, &community));

        if (0 != osal_strlen(community.name))
        {
            fprintf(pSnmpdConf, "com2sec  \"%s\"   default  \"%s\" \n",
                        community.name,
                        community.name);

#ifdef CONFIG_SYS_IPV6
            fprintf(pSnmpdConf, "com2sec6  \"%s\"   default  \"%s\" \n",
                        community.name,
                        community.name);
#endif
        }
    }

#if 0
    
    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_ERR_CHK(cfg_snmp_community_get(index, &community));

        if ((0 != osal_strlen(community.name)) &&
            (SNMP_COMMUNITY_ACCESS_READ_ONLY == community.access))
        {
            fprintf(pSnmpdConf, snmpdConfData[SNMPD_CONF_COMMUNITY + index],
                        "rocommunity", community.name);

#ifdef CONFIG_SYS_IPV6
            fprintf(pSnmpdConf, snmpdConfData[SNMPD_CONF_COMMUNITY + index],
                        "rocommunity6", community.name);
#endif
        }
        else if (0 != (osal_strlen(community.name)) &&
                (SNMP_COMMUNITY_ACCESS_READ_WRITE == community.access))
        {
            fprintf(pSnmpdConf, snmpdConfData[SNMPD_CONF_COMMUNITY + index],
                        "rwcommunity", community.name);

#ifdef CONFIG_SYS_IPV6
            fprintf(pSnmpdConf, snmpdConfData[SNMPD_CONF_COMMUNITY + index],
                        "rwcommunity6", community.name);
#endif
        }
    }

    for (index = 0; index < CAPA_SNMP_HOST_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_target_get(index, &trap));

        if (0 != osal_strlen(trap.server))
        {
            fprintf(pSnmpdConf, snmpdConfData[SNMPD_CONF_TRAPHOST + index],
                        "trapsink", trap.server, trap.name);
        }
    }
#endif

    fprintf(pSnmpdConf, "\n\n#         ip address    community \n");

    for(index = 0; index < CAPA_SNMP_HOST_NUM; index++)
    {
        SYS_ERR_CHK(sal_snmp_target_get(index, &host));

        if(0 != osal_strlen(host.server))
        {
             fprintf(pSnmpdConf, "%s    %s:%d    \"%s\" \n", gtrapStr[host.trapType], host.server, host.portNum ,host.name);
        }
    }

    fprintf(pSnmpdConf, "\n\n#        groupName securityModel securityName \n");

    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_ERR_CHK(cfg_snmp_community_get(index, &community));

        if (osal_strlen(community.name) != 0)
        {
            fprintf(pSnmpdConf, "group   \"%s%s\"   %s     \"%s\"   \n ",
                        SNMP_ACCESS_GROUP_PREFIX,
                        community.name,
                        ggroupSecModel[SNMP_SECMODEL_V1],
                        community.name);

            fprintf(pSnmpdConf, "group   \"%s%s\"   %s    \"%s\"   \n ",
                        SNMP_ACCESS_GROUP_PREFIX,
                        community.name,
                        ggroupSecModel[SNMP_SECMODEL_V2C],
                        community.name);
        }
    }

    fprintf(pSnmpdConf, "\n\n#       name           incl/excl     subtree         mask(optional)  \n");
    fprintf(pSnmpdConf, "view    \"%s\"     included  .1  \n", DFLT_SNMP_VIEW_STR);

    fprintf(pSnmpdConf, "\n\n#     group   context  sec.model  sec.level  prefix  read   write   notify  \n");
    for (index = 0; index < CAPA_SNMP_COMMUNITY_NUM; index++)
    {
        SYS_ERR_CHK(cfg_snmp_community_get(index, &community));
        if(0 != osal_strlen(community.name))
        {
            fprintf(pSnmpdConf, "access    \"%s%s\"     %s    %s   %s  %s  \"%s\"  \"%s\"  \"%s\"  \n",
                                             SNMP_ACCESS_GROUP_PREFIX,community.name,
                                             "\"\"",
                                             ggroupSecModel[SNMP_SECMODEL_V1],
                                             gsecLevel[SNMP_SECLEVEL_NOAUTHNOPRIV],
                                             "exact",
                                             DFLT_SNMP_VIEW_STR,
                                             (SNMP_COMMUNITY_ACCESS_READ_WRITE == community.access) ? DFLT_SNMP_VIEW_STR : "none",
                                             DFLT_SNMP_VIEW_STR);

            fprintf(pSnmpdConf, "access    \"%s%s\"     %s    %s   %s  %s  \"%s\"  \"%s\"  \"%s\"  \n",
                                             SNMP_ACCESS_GROUP_PREFIX,community.name,
                                             "\"\"",
                                             ggroupSecModel[SNMP_SECMODEL_V2C],
                                             gsecLevel[SNMP_SECLEVEL_NOAUTHNOPRIV],
                                             "exact",
                                             DFLT_SNMP_VIEW_STR,
                                             (SNMP_COMMUNITY_ACCESS_READ_WRITE == community.access) ? DFLT_SNMP_VIEW_STR : "none",
                                             DFLT_SNMP_VIEW_STR);
        }
    }

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    fprintf(pSnmpdConf, "rwcommunity \"%s\"\n", sal_rmon_cstr_ret());
#ifdef CONFIG_SYS_IPV6
    fprintf(pSnmpdConf, "rwcommunity6 \"%s\"\n", sal_rmon_cstr_ret());
#endif
#endif

    fclose(pSnmpdConf);

    return SYS_ERR_OK;
}

#endif

int32 sal_snmp_config_reload_get(sal_snmp_reload_t *pReload)
{
    SYS_PARAM_CHK((NULL == pReload), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_snmp_config_reload_get(pReload));

    return SYS_ERR_OK;
}

int32 sal_snmp_config_reload_set(sal_snmp_reload_t reload)
{
    SYS_ERR_CHK(cfg_snmp_config_reload_set(reload));

    return SYS_ERR_OK;
}

int32 sal_snmp_statistic_get(uint32 index, uint32 *pCnt)
{
    SYS_PARAM_CHK((NULL == pCnt), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index >= SNMP_STAT_MAX_STATS), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_snmp_statistic_get(index, pCnt));

    return SYS_ERR_OK;
}

int32 sal_snmp_statistic_set(uint32 index, uint32 cnt)
{
    SYS_PARAM_CHK((index >= SNMP_STAT_MAX_STATS), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_snmp_statistic_set(index, cnt));
    return SYS_ERR_OK;
}

int32 sal_snmp_statistic_init(void)
{
    uint32 index;
    for (index = 0; index < SNMP_STAT_MAX_STATS; index++)
        SYS_ERR_CHK(cfg_snmp_statistic_set(index, 0));

    return SYS_ERR_OK;
}

int32 _sal_snmp_notifyMsg_init(void)
{
    SYS_ERR_CHK(cfg_snmp_notifyMsg_init());
    return SYS_ERR_OK;
}

int32 _sal_snmp_notifyMsg_set(uint32 index, sys_snmp_notifyMsg_t *pMsg)
{
    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index > SNMP_NOTIFY_MAX_BUF), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_notifyMsg_set(index, pMsg));

    return SYS_ERR_OK;
}

int32 _sal_snmp_notifyMsg_get(uint32 index,sys_snmp_notifyMsg_t *pMsg)
{
    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((index > SNMP_NOTIFY_MAX_BUF), SYS_ERR_ENTRY_INDEX);

    SYS_ERR_CHK(cfg_snmp_notifyMsg_get(index, pMsg));

    return SYS_ERR_OK;
}

int32 sal_snmp_name_isValid(char *pName)
{
    int32 i ,len = 0;

    if (NULL == pName)
        return FALSE;

    len = osal_strlen(pName);

    if (len < 1)
        return FALSE;

    for (i = 0; i < len ; i++)
    {
        if (' ' == pName[i]  || '"' == pName[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

int32 ipc_trap_snmp_msg(sal_snmp_trapMsg_t trapMsg)
{
	SMD_MSG 				  msg;
	int 					  iRet;
	ARG_DEF(trapMsg.num);

	smdMsgInit(&msg, SMD_MSG_TYPE_TRAP, trapMsg.trapName, SMD_GLOBAL, SMD_MSG_INFO);
	
	if( 0 < trapMsg.num && trapMsg.num <= MAX_SNMP_TRAP_ARG_MAX)
	{
		memcpy(ARG_GET(), trapMsg.args, sizeof(TRAP_ARG)*trapMsg.num);
	}
	else 
		return SYS_ERR_FAILED;

	iRet = smdMsgRequestByName(SNMP_TRAP_SERVER_PATH, &msg, ARG_GET(), trapMsg.num, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int32 sal_snmp_linkUpDownTrap_send_new(sys_logic_port_t logicPort, sys_port_linkStatus_t link)
{
    char sysIpBuf[SYS_BUF128_LEN];
    uint32 portLinkStatus = 0;
    uint32 portConfigStatus = 0;
    sys_enable_t snmp_enable = DISABLED;
    sys_enable_t trap_enable = DISABLED;
    sys_time_str_t timestring;
	sal_snmp_trapMsg_t		  msg;
	uint32 ipaddr;
    uint32 netmask;
    char ipaddr_str[SYS_BUF32_LEN];
    sys_enable_t enable;
    sys_user_port_t usr_port;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((link >= PORT_LINKSTATUS_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_LINKUPDOWN, &trap_enable));
    if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
    {
        SNMP_LOG(LOG_SNMP_TRAP_DROP);
        return SYS_ERR_OK;
    }
    usr_port = LP2UP(logicPort);
    memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));
	osal_memset(ipaddr_str, 0, sizeof(ipaddr_str));

	msg.trapName = TRAP_FUNC_PORT_LINK;	
    msg.num = 5;

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);
	
	msg.args[0].pBuf = timestring.tstr;
	msg.args[0].usFlags = 0;
	msg.args[0].usLen = strlen(timestring.tstr)+1;

	msg.args[1].pBuf = &usr_port;
	msg.args[1].usFlags = 0;
	msg.args[1].usLen = sizeof(usr_port);

	if(PORT_LINKDOWN == link)
	{
		portLinkStatus = 2;
	}
	else
	{
		portLinkStatus = 1;
	}	
	msg.args[2].pBuf = &portLinkStatus;
	msg.args[2].usFlags = 0;
	msg.args[2].usLen = sizeof(portLinkStatus);

    SYS_ERR_CHK(sal_sys_ip_get(&ipaddr, &netmask));
    SYS_ERR_CHK(sys_util_ip2Str(ipaddr, ipaddr_str));
    SYS_MEM_CLEAR(sysIpBuf);
	osal_sprintf(sysIpBuf, "%s", ipaddr_str);
	msg.args[3].pBuf = sysIpBuf;
	msg.args[3].usFlags = 0;
	msg.args[3].usLen = strlen(sysIpBuf)+1;

	SYS_ERR_CHK(sal_port_enable_get(logicPort, &enable));
	if(DISABLED == enable)
	{
		portConfigStatus = 2;
	}
	else
	{
		portConfigStatus = 1;
	}
	msg.args[4].pBuf = &portConfigStatus;
	msg.args[4].usFlags = 0;
	msg.args[4].usLen = sizeof(portConfigStatus);
	
	ipc_trap_snmp_msg(msg);	

    return SYS_ERR_OK;
}

void sal_snmp_trap_static_vlan_create(int pid, uint32 vid, char* vlanName, int32 ret)
{
	sal_snmp_trapMsg_t        msg;
	sys_time_str_t            timestring;
    char detailBuf[SYS_BUF128_LEN];
    uint32 option = 0;

	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_OPERATE;	
	msg.num = 3;

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);
	
	msg.args[0].pBuf = timestring.tstr;
	msg.args[0].usFlags = 0;
	msg.args[0].usLen = strlen(timestring.tstr)+1;

	SYS_MEM_CLEAR(detailBuf);
	if(ret == SYS_ERR_OK)
		osal_sprintf(detailBuf, "Static vlan %d(%s) is created", vid, vlanName);
	else if(ret == SYS_ERR_FAILED)
		osal_sprintf(detailBuf, "Failed to create static vlan %d(%s)", vid, vlanName);
		
	msg.args[1].pBuf = detailBuf;
	msg.args[1].usFlags = 0;
	msg.args[1].usLen = strlen(detailBuf)+1;

	option = 1;
	msg.args[2].pBuf = &option;
	msg.args[2].usFlags = 0;
	msg.args[2].usLen = sizeof(uint32);

	ipc_trap_snmp_msg(msg);	
}

void sal_snmp_trap_static_vlan_destroy(int pid, uint32 vid, int32 ret)
{
	sal_snmp_trapMsg_t        msg;
	sys_time_str_t            timestring;
	char buf[SYS_BUF128_LEN];
	uint32 option = 0;

	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_OPERATE;
	msg.num = 3;

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);
	
	msg.args[0].pBuf = timestring.tstr;
	msg.args[0].usFlags = 0;
	msg.args[0].usLen = strlen(timestring.tstr)+1;

	SYS_MEM_CLEAR(buf);
	if(ret == SYS_ERR_OK)
		osal_sprintf(buf, "Static vlan %d is destroy", vid);
	else if(ret == SYS_ERR_FAILED)
		osal_sprintf(buf, "Failed to destroy static vlan %d", vid);
		
	msg.args[1].pBuf = buf;
	msg.args[1].usFlags = 0;
	msg.args[1].usLen = strlen(buf)+1;

	option = 1;
	msg.args[2].pBuf = &option;
	msg.args[2].usFlags = 0;
	msg.args[2].usLen = sizeof(uint32);

	ipc_trap_snmp_msg(msg);	
}

void sal_snmp_trap_mac_option(int32 option,sys_l2_entry_t *pEntry)
{
	sal_snmp_trapMsg_t        msg;
	sys_time_str_t            timestring;
	char buf[SYS_BUF128_LEN];
	char macStr[SYS_BUF64_LEN];
	uint32 tmpOption = 0;
    sys_user_port_t usr_port;
    
	if(macStr == NULL)
	{
		return;
	}
	usr_port = LP2UP(pEntry->port);
	sys_util_mac2Str(pEntry->mac.octet, macStr);
	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_OPERATE;
	msg.num = 3;

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);
	
	msg.args[0].pBuf = timestring.tstr;
	msg.args[0].usFlags = 0;
	msg.args[0].usLen = strlen(timestring.tstr)+1;

	#if 0
	if(option)
	{
		osal_sprintf(buf, "add %u  %s  vlan%u",usr_port,macStr, pEntry->vid);
	}
	else
	#endif
	{
		osal_sprintf(buf, "%u %s %u",usr_port,macStr, pEntry->vid);
	}
	msg.args[1].pBuf = buf;
	msg.args[1].usFlags = 0;
	msg.args[1].usLen = strlen(buf)+1;

	tmpOption = 2;

	msg.args[2].pBuf = &tmpOption;
	msg.args[2].usFlags = 0;
	msg.args[2].usLen = sizeof(uint32);

	ipc_trap_snmp_msg(msg);	
}

int32 sal_snmp_startupTrap_send_new(sys_enable_t restart)
{

    SYS_PARAM_CHK(IS_ENABLE_INVALID(restart), SYS_ERR_INPUT);
    
    sal_snmp_trapMsg_t        msg;
	sys_time_str_t            timestring;
	char buf				  [SYS_BUF128_LEN];

	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_START;
	msg.num = 3;

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);
	
	msg.args[0].pBuf = timestring.tstr;
	msg.args[0].usFlags = 0;
	msg.args[0].usLen = strlen(timestring.tstr)+1;

	SYS_MEM_CLEAR(buf);
	osal_sprintf(buf, "System boot");
	msg.args[1].pBuf = buf;
	msg.args[1].usFlags = 0;
	msg.args[1].usLen = strlen(buf)+1;

	SYS_MEM_CLEAR(buf);
	
    if (ENABLED == restart)
    {

        osal_sprintf(buf, "System Warm Start");
    }
    else
    {

        osal_sprintf(buf, "System Cold Start");
    }
		
	msg.args[2].pBuf = buf;
	msg.args[2].usFlags = 0;
	msg.args[2].usLen = strlen(buf)+1;

	ipc_trap_snmp_msg(msg);	
    return SYS_ERR_OK;
}

int32 sal_snmp_systerm_reboot_Trap_send_new(int pid)
{
    sal_snmp_trapMsg_t        msg;
	sys_time_str_t            timestring;
	char buf[SYS_BUF128_LEN];

	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_REBOOT;
	msg.num = 4;

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);
	
	msg.args[0].pBuf = timestring.tstr;
	msg.args[0].usFlags = 0;
	msg.args[0].usLen = strlen(timestring.tstr)+1;

	SYS_MEM_CLEAR(buf);
	osal_sprintf(buf, "System reboot");
	msg.args[1].pBuf = buf;
	msg.args[1].usFlags = 0;
	msg.args[1].usLen = strlen(buf)+1;

	msg.args[2].pBuf = &pid;
	msg.args[2].usFlags = 0;
	msg.args[2].usLen = sizeof(int);

	SYS_MEM_CLEAR(buf);
	osal_sprintf(buf, "System reboot");
	msg.args[3].pBuf = buf;
	msg.args[3].usFlags = 0;
	msg.args[3].usLen = strlen(buf)+1;

	ipc_trap_snmp_msg(msg);	

    return SYS_ERR_OK;
}

uint32 sal_snmp_system_auth_success_fail_Trap_send_new(uint32 sOrF, const char *connectType,  char *username, char *hostIp)
{
    sys_enable_t snmp_enable = DISABLED;
    sys_enable_t trap_enable = DISABLED;
    sys_time_str_t timestring;
    sal_snmp_trapMsg_t		 msg;
    uint32 loginType = 0;
    uint32 statusType = 0;
 
    SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
    SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &trap_enable));
    if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
    {
 	   SNMP_LOG(LOG_SNMP_TRAP_DROP);
 	   return SYS_ERR_OK;
    }
 
    memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));
 
    msg.trapName = TRAP_FUNC_LOGIN_MESSAGES; 
    msg.num = 5;

    SYS_MEM_CLEAR(timestring);
    sal_time_currentTimeStr_get(&timestring);
    
    msg.args[0].pBuf = timestring.tstr;
    msg.args[0].usFlags = 0;
    msg.args[0].usLen = strlen(timestring.tstr)+1;

    msg.args[1].pBuf = username;
    msg.args[1].usFlags = 0;
    msg.args[1].usLen = strlen(username)+1;

	if(0 == osal_strcmp(connectType, "console"))
	{
		loginType = 8;
	}
	else if(0 == osal_strcmp(connectType, "telnet"))
	{
		loginType = 4;
	}
	else if(0 == osal_strcmp(connectType, "ssh"))
	{
		loginType = 5;
	}
	else if(0 == osal_strcmp(connectType, "http"))
	{
		loginType = 1;
	}
	else if(0 == osal_strcmp(connectType, "https"))
	{
		loginType = 2;
	}
	else 
	{
		loginType = 0;
	}

    msg.args[2].pBuf = &loginType;
    msg.args[2].usFlags = 0;
    msg.args[2].usLen = sizeof(uint32);

    if(sOrF == TRUE)
    {
 	   statusType = 1;
    }
    else
    {
 	   statusType = 4;
    }
    msg.args[3].pBuf = &statusType;
    msg.args[3].usFlags = 0;
    msg.args[3].usLen = sizeof(uint32);
    
	if(strlen(hostIp) == 0)
	{
		osal_sprintf(hostIp, "0.0.0.0");
	}
    msg.args[4].pBuf = hostIp;
    msg.args[4].usFlags = 0;
    msg.args[4].usLen = strlen(hostIp)+1;
    
    ipc_trap_snmp_msg(msg); 
 
    return SYS_ERR_OK;

}

uint32 sal_snmp_system_auth_logout_Trap_send_new( char *username, char *connectType, char *hostIp)
{
	sys_enable_t snmp_enable = DISABLED;
	sys_enable_t trap_enable = DISABLED;
	sys_time_str_t timestring;
	sal_snmp_trapMsg_t		msg;
	uint32 loginType = 0;
	uint32 statusType = 0;
	SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
	SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &trap_enable));
	if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
	{
	  SNMP_LOG(LOG_SNMP_TRAP_DROP);
	  return SYS_ERR_OK;
	}

	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_LOGIN_MESSAGES; 
	msg.num = 5;

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

	msg.args[0].pBuf = timestring.tstr;
	msg.args[0].usFlags = 0;
	msg.args[0].usLen = strlen(timestring.tstr)+1;

	msg.args[1].pBuf = username;
	msg.args[1].usFlags = 0;
	msg.args[1].usLen = strlen(username)+1;

	if(0 == osal_strcmp(connectType, "console"))
	{
	   loginType = 8;
	}
	else if(0 == osal_strcmp(connectType, "telnet"))
	{
	   loginType = 4;
	}
	else if(0 == osal_strcmp(connectType, "ssh"))
	{
	   loginType = 5;
	}
	else if(0 == osal_strcmp(connectType, "http"))
	{
	   loginType = 1;
	}
	else if(0 == osal_strcmp(connectType, "https"))
	{
	   loginType = 2;
	}
	else 
	{
	   loginType = 0;
	}

	msg.args[2].pBuf = &loginType;
	msg.args[2].usFlags = 0;
	msg.args[2].usLen = sizeof(uint32);

	statusType = 2;
	msg.args[3].pBuf = &statusType;
	msg.args[3].usFlags = 0;
	msg.args[3].usLen = sizeof(uint32);
	
	if(strlen(hostIp) == 0)
	{
	   osal_sprintf(hostIp, "0.0.0.0");
	}
	msg.args[4].pBuf = hostIp;
	msg.args[4].usFlags = 0;
	msg.args[4].usLen = strlen(hostIp)+1;
	ipc_trap_snmp_msg(msg); 

	return SYS_ERR_OK;

}

uint32 sal_snmp_system_auth_acount_Trap_send_new(uint32 opCode, int32 pid, char *username,uint32 accessMethod)
{
	sys_enable_t snmp_enable = DISABLED;
	sys_enable_t trap_enable = DISABLED;
	sys_time_str_t timestring;
	sal_snmp_trapMsg_t		msg;
	uint32 loginType = 0;
	uint32 statusType = 0;
	sys_line_session_t session;

	SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
	SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &trap_enable));
	if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
	{
	  SNMP_LOG(LOG_SNMP_TRAP_DROP);
	  return SYS_ERR_OK;
	}

	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_LOGIN_PASSWORD_CHANGE; 
	msg.num = 5;

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

	msg.args[0].pBuf = timestring.tstr;
	msg.args[0].usFlags = 0;
	msg.args[0].usLen = strlen(timestring.tstr)+1;
	if(opCode == 1)
	{
		osal_memset(&session, 0, sizeof(sys_line_session_t));

	    sal_sys_lineSession_by_pid_get_new(pid, &session);
	    if(strlen(session.host) == 0)
	    {
	   		osal_sprintf(session.host, "0.0.0.0");
	    }
	}
	else
	{

   		 sal_sys_get_current_web_session_info(&session);
	}
	msg.args[1].pBuf = username;
	msg.args[1].usFlags = 0;
	msg.args[1].usLen = strlen(username)+1;

	if(accessMethod == 0x01)
	{
	   loginType = 8;
	}
	else if(accessMethod == 0x02)
	{
	   loginType = 4;
	}
	else if(accessMethod == 0x04)
	{
	   loginType = 5;
	}
	else if(accessMethod == 0x08)
	{
	   loginType = 1;
	}
	else 
	{
	   loginType = 0;
	}

	msg.args[2].pBuf = &loginType;
	msg.args[2].usFlags = 0;
	msg.args[2].usLen = sizeof(uint32);

	statusType = 3;
	msg.args[3].pBuf = &statusType;
	msg.args[3].usFlags = 0;
	msg.args[3].usLen = sizeof(uint32);
	
	msg.args[4].pBuf = session.host;
	msg.args[4].usFlags = 0;
	msg.args[4].usLen = strlen(session.host)+1;

	ipc_trap_snmp_msg(msg); 

	return SYS_ERR_OK;
}

uint32 sal_snmp_system_user_operation_Trap_send_new(uint32 opCode,int32 sOrF, char *username)
{
	sys_enable_t snmp_enable = DISABLED;
	sys_enable_t trap_enable = DISABLED;
	sys_time_str_t timestring;
	sal_snmp_trapMsg_t		msg;
	char hostIp[SYS_BUF32_LEN] = "0.0.0.0";
	char buf[SYS_BUF128_LEN];

	SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
	SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &trap_enable));
	if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
	{
	  SNMP_LOG(LOG_SNMP_TRAP_DROP);
	  return SYS_ERR_OK;
	}

	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_USER_OPERATION; 
	msg.num = 4;

	SYS_MEM_CLEAR(timestring);
	sal_time_currentTimeStr_get(&timestring);

	msg.args[0].pBuf = timestring.tstr;
	msg.args[0].usFlags = 0;
	msg.args[0].usLen = strlen(timestring.tstr)+1;

	msg.args[1].pBuf = username;
	msg.args[1].usFlags = 0;
	msg.args[1].usLen = strlen(username)+1;
	
	msg.args[2].pBuf = hostIp;
	msg.args[2].usFlags = 0;
	msg.args[2].usLen = strlen(hostIp)+1;

	switch (opCode)
	{
		 case SNMP_ACOUNT_ADD:
		    if(sOrF == SYS_ERR_OK)
		 	   osal_sprintf(buf, "Acount %s add successfully ", username);
		 	else 
		 	   osal_sprintf(buf, "Acount %s add failed ", username);
		    break;

		 case SNMP_ACOUNT_CHANGE:
		    if(sOrF == SYS_ERR_OK)
		 	   osal_sprintf(buf, "Acount %s  password changed successfully ", username);
		 	else 
		 	   osal_sprintf(buf, "Acount %s password changed failed ", username);
		    break;
		 case SNMP_ACOUNT_DELETE:
		 default:
		    if(sOrF == SYS_ERR_OK)
		 	   osal_sprintf(buf, "Acount %s  delete successfully ", username);
		 	else 
		 	   osal_sprintf(buf, "Acount %s delete failed ", username);
		    break;
	}
	msg.args[3].pBuf = buf;
	msg.args[3].usFlags = 0;
	msg.args[3].usLen = strlen(buf)+1;
	ipc_trap_snmp_msg(msg); 

	return SYS_ERR_OK;
}

uint32 sal_snmp_system_web_user_operation_Trap_send_new(uint32 opCode,int32 sOrF, char *username)
{
	sys_enable_t snmp_enable = DISABLED;
	sys_enable_t trap_enable = DISABLED;
	sys_time_str_t timestring;
	sal_snmp_trapMsg_t		msg;
	char buf[SYS_BUF128_LEN];
    sys_line_session_t session;
    int ret;
	osal_memset(&session, 0, sizeof(sys_line_session_t));

    ret = sal_sys_get_current_web_session_info(&session);

	SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
	SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &trap_enable));
	if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
	{
	  SNMP_LOG(LOG_SNMP_TRAP_DROP);
	  return SYS_ERR_OK;
	}
	if (ret == SYS_ERR_OK)
	{
    	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));
    
    	msg.trapName = TRAP_FUNC_USER_OPERATION; 
    	msg.num = 4;

    	SYS_MEM_CLEAR(timestring);
    	sal_time_currentTimeStr_get(&timestring);
    
    	msg.args[0].pBuf = timestring.tstr;
    	msg.args[0].usFlags = 0;
    	msg.args[0].usLen = strlen(timestring.tstr)+1;
    
    	msg.args[1].pBuf = username;
    	msg.args[1].usFlags = 0;
    	msg.args[1].usLen = strlen(username)+1;
    	
    	msg.args[2].pBuf = session.host;
    	msg.args[2].usFlags = 0;
    	msg.args[2].usLen = strlen(session.host)+1;
    
    	switch (opCode)
    	{
    		 case SNMP_ACOUNT_ADD:
    		    if(sOrF == SYS_ERR_OK)
    		 	   osal_sprintf(buf, "Acount %s add successfully ", username);
    		 	else 
    		 	   osal_sprintf(buf, "Acount %s add failed ", username);
    		    break;
    
    		 case SNMP_ACOUNT_CHANGE:
    		    if(sOrF == SYS_ERR_OK)
    		 	   osal_sprintf(buf, "Acount %s  password changed successfully ", username);
    		 	else 
    		 	   osal_sprintf(buf, "Acount %s password changed failed ", username);
    		    break;
    		 case SNMP_ACOUNT_DELETE:
    		 default:
    		    if(sOrF == SYS_ERR_OK)
    		 	   osal_sprintf(buf, "Acount %s  delete successfully ", username);
    		 	else 
    		 	   osal_sprintf(buf, "Acount %s delete failed ", username);
    		    break;
    	}
    	msg.args[3].pBuf = buf;
    	msg.args[3].usFlags = 0;
    	msg.args[3].usLen = strlen(buf)+1;
    	ipc_trap_snmp_msg(msg); 
  }
	return SYS_ERR_OK;
}

uint32 sal_snmp_system_user_operation_info_Trap(int pid, char *cmdInfo)
{
	sys_enable_t snmp_enable = DISABLED;
	sys_enable_t trap_enable = DISABLED;
	sys_time_str_t timestring;
	sal_snmp_trapMsg_t		msg;

    sys_line_session_t session;
    int ret = SYS_ERR_OK;

	SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
	SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &trap_enable));
	if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
	{
	  SNMP_LOG(LOG_SNMP_TRAP_DROP);
	  return SYS_ERR_OK;
	}

	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_USER_OPERATION_INFO; 
	msg.num = 4;

   osal_memset(&session, 0, sizeof(sys_line_session_t));

   ret = sal_sys_lineSession_by_pid_get_new(pid, &session);
   if (ret == SYS_ERR_OK)
   {

    	SYS_MEM_CLEAR(timestring);
    	sal_time_currentTimeStr_get(&timestring);
    
    	msg.args[0].pBuf = timestring.tstr;
    	msg.args[0].usFlags = 0;
    	msg.args[0].usLen = strlen(timestring.tstr)+1;
    
    	msg.args[1].pBuf = session.user;
    	msg.args[1].usFlags = 0;
    	msg.args[1].usLen = strlen(session.user)+1;

    if(strlen(session.host) == 0)
	{
	   osal_sprintf(session.host, "0.0.0.0");
	}
    	msg.args[2].pBuf = session.host;
    	msg.args[2].usFlags = 0;
    	msg.args[2].usLen = strlen(session.host)+1;

    	msg.args[3].pBuf = cmdInfo;
    	msg.args[3].usFlags = 0;
    	msg.args[3].usLen = strlen(cmdInfo)+1;
    	ipc_trap_snmp_msg(msg); 
    	return SYS_ERR_OK;
   }
   else
   {
        return SYS_ERR_OK;
   }
}

uint32 sal_snmp_system_web_user_operation_info_Trap(char *cmdInfo)
{
	sys_enable_t snmp_enable = DISABLED;
	sys_enable_t trap_enable = DISABLED;
	sys_time_str_t timestring;
	sal_snmp_trapMsg_t		msg;

    sys_line_session_t session;
    int ret = SYS_ERR_OK;

	SYS_ERR_CHK(sal_snmp_enable_get(&snmp_enable));
	SYS_ERR_CHK(sal_snmp_trapEnable_get(SNMP_TRAP_CLASS_AUTHENTICATE, &trap_enable));
	if((DISABLED == snmp_enable) || (DISABLED == trap_enable))
	{
	  SNMP_LOG(LOG_SNMP_TRAP_DROP);
	  return SYS_ERR_OK;
	}

	memset(&msg, 0, sizeof(sal_snmp_trapMsg_t));

	msg.trapName = TRAP_FUNC_USER_OPERATION_INFO; 
	msg.num = 4;

   osal_memset(&session, 0, sizeof(sys_line_session_t));

   ret = sal_sys_get_current_web_session_info(&session);
   if (ret == SYS_ERR_OK)
   {

    	SYS_MEM_CLEAR(timestring);
    	sal_time_currentTimeStr_get(&timestring);
    
    	msg.args[0].pBuf = timestring.tstr;
    	msg.args[0].usFlags = 0;
    	msg.args[0].usLen = strlen(timestring.tstr)+1;
    
    	msg.args[1].pBuf = session.user;
    	msg.args[1].usFlags = 0;
    	msg.args[1].usLen = strlen(session.user)+1;

    	msg.args[2].pBuf = session.host;
    	msg.args[2].usFlags = 0;
    	msg.args[2].usLen = strlen(session.host)+1;

    	msg.args[3].pBuf = cmdInfo;
    	msg.args[3].usFlags = 0;
    	msg.args[3].usLen = strlen(cmdInfo)+1;
    	ipc_trap_snmp_msg(msg); 
    	return SYS_ERR_OK;
	}
	else
	{
        return SYS_ERR_OK;
  
	}
}

