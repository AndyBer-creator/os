
#include <time.h>
#include <sys/sysinfo.h>
#include <common/sys_def.h>
#include <common/sys_log.h>
#include <libsal/sal_sys.h>
#include <libfds/fds_export.h>
#ifdef CONFIG_SYS_KERNEL_LINUX_2_6_X
#include <linux/compile.h>
#endif

#ifdef CONFIG_SYS_SNMP
char *pOidStr = NULL;
char oidStr[CAPA_SNMP_OID_STR_LEN];
#endif

#ifdef CONFIG_SYS_SNMP

const char *fds_sys_objid_ret(void)
{
    char tmp[SYS_BUF32_LEN];
    uint32 i;
    sys_boardModel_t boardModel;

    if (NULL == pOidStr)
    {
        SYS_MEM_CLEAR(boardModel);
        SYS_MEM_CLEAR(oidStr);

        sal_sys_boardModel_get(&boardModel);

        for (i = 0; i < boardModel.oid.oidLen; i++)
        {
            osal_sprintf(tmp, "%d.", boardModel.oid.oidAry[i]);
            osal_strcat(oidStr, tmp);
        }

        if (oidStr > 0)
            oidStr[osal_strlen(oidStr) - 1] = '\0';

        pOidStr = oidStr;
    }

    return pOidStr;
}

void fds_sys_objid_array_get(uint8 *pObjectId)
{
    sys_boardModel_t boardModel;

    SYS_MEM_CLEAR(boardModel);

    sal_sys_boardModel_get(&boardModel);

    osal_memcpy(pObjectId, boardModel.oid.oidAry, boardModel.oid.oidLen * sizeof(uint32));

    return;
}

int fds_sys_objid_length_ret(void)
{
    sys_boardModel_t boardModel;

    SYS_MEM_CLEAR(boardModel);

    sal_sys_boardModel_get(&boardModel);

    return boardModel.oid.oidLen;
}
#endif

const char *fds_sys_firmwareDate_ret(void)
{
    static char date[SYS_BUF64_LEN];

    osal_memset(date, 0, sizeof(date));

    sal_sys_currSysCompiledTimeStr_get(date);

    return date;
}

uint32 fds_sys_firmwareMagicNumber_ret(void)
{
    
    return CONFIG_IH_MAGIC_NUMBER;
}

