
#ifndef __CMD_UTIL_H__
#define __CMD_UTIL_H__

#include <common/sys_def.h>
#include <common/sys_error.h>

#define CMD_TOKENSTR_GET(vline, idx) ((char*) vector_slot(vline, idx))
#define CMD_TOKENNUM_GET(vline) (vector_count(vline))
#define CMD_PORTLIST_GET(tokenNum, portlist) \
do{ \
    SYS_ERR_CHK(cmd_util_portlist_get(argc, argv, tokenNum, portlist)); \
}while(0)

#define CMD_NORMALPORTLIST_GET(tokenNum, portlist) \
do{ \
    SYS_ERR_CHK(cmd_util_normalPortlist_get(argc, argv, tokenNum, portlist)); \
}while(0)

#define CMD_FMTSTR_PRINT(buf, fmt, args...) \
do { \
    sys_util_fmtString_get(buf, fmt, args);  \
    SYS_PRINTF("%s\n", buf); \
} while (0) \

#define CMD_ERR_CONTINUE_PORT(op, lport)            \
{                                                   \
    int32 errCode;                                  \
    if ((errCode = (op)) != SYS_ERR_OK)            \
    {                                               \
        char __portStr[4];                          \
        osal_memset(__portStr, 0, sizeof(__portStr));\
        LP2STR(lport, __portStr);                \
        SYS_PRINTF("Port %s: %s\n", __portStr, SYS_ERR_STR(errCode)); \
        continue;                                   \
    }                                               \
}

#define CMD_ERR_CHK_PORT(op, lport)                 \
do {                                                \
    int32 errCode;                                  \
    if ((errCode = (op)) != SYS_ERR_OK)            \
    {                                               \
        char __portStr[4];                          \
        osal_memset(__portStr, 0, sizeof(__portStr));\
        LP2STR(lport, __portStr);                \
        SYS_PRINTF("Port %s: %s\n", __portStr, SYS_ERR_STR(errCode)); \
        return SYS_ERR_OK;                          \
    }                                               \
} while (0)

#define CMD_ERR_CONTINUE_VLAN(op, vid)              \
{                                                   \
    int32 errCode;                                  \
    if ((errCode = (op)) != SYS_ERR_OK)            \
    {                                               \
        SYS_PRINTF("VLAN %d: %s\n", vid, SYS_ERR_STR(errCode)); \
        continue;                                   \
    }                                               \
}

#define CMD_ERR_CHK_VLAN(op, vid)                   \
do {                                                \
    int32 errCode;                                  \
    if ((errCode = (op)) != SYS_ERR_OK)            \
    {                                               \
        SYS_PRINTF("VLAN %d: %s\n", vid, SYS_ERR_STR(errCode)); \
        return SYS_ERR_OK;                          \
    }                                               \
} while (0)

#define CMD_ERR_CONTINUE_VLAN_EXIST(vid)            \
{                                                   \
    if (TRUE == sal_vlan_adminEntryExist_ret(vid))  \
    {                                               \
        SYS_PRINTF("VLAN %d: %s\n", vid, SYS_ERR_STR(SYS_ERR_VLAN_EXIST)); \
        continue;                                   \
    }                                               \
}

#define CMD_ERR_CONTINUE_VLAN_NOTEXIST(vid)         \
{                                                   \
    if (FALSE == sal_vlan_adminEntryExist_ret(vid)) \
    {                                               \
        SYS_PRINTF("VLAN %d: %s\n", vid, SYS_ERR_STR(SYS_ERR_VLAN_NOT_EXIST)); \
        continue;                                   \
    }                                               \
}

#define CMD_ERR_CHK_VLAN_EXIST(vid)                 \
do {                                                \
    if (TRUE == sal_vlan_adminEntryExist_ret(vid))  \
    {                                               \
        SYS_PRINTF("VLAN %d: %s\n", vid, SYS_ERR_STR(SYS_ERR_VLAN_EXIST)); \
        return SYS_ERR_OK;                  \
    }                                               \
} while (0)

#define CMD_ERR_CHK_VLAN_NOTEXIST(vid)              \
do {                                                \
    if (FALSE == sal_vlan_adminEntryExist_ret(vid)) \
    {                                               \
        SYS_PRINTF("VLAN %d: %s\n", vid, SYS_ERR_STR(SYS_ERR_VLAN_NOT_EXIST)); \
        return SYS_ERR_OK;              \
    }                                               \
} while (0)

#define CMD_TRUNK_STR_GET(trunkId, str) \
    osal_sprintf(str, "trunk%d", trunkId + 1);

#define CMD_IS_NO_FORM_EXIST()         \
    ((1 == strlen(CMD_TOKENSTR_GET(cmd->vline, 0)) && 'n' == CMD_TOKENSTR_GET(cmd->vline, 0)[0]) \
     || (0 == osal_strcmp("no", CMD_TOKENSTR_GET(cmd->vline, 0))))

#define PROGRESS_PID_FILE_PREFIX   "/tmp/_cli_progress_"

extern uint32               g_isCliFwProcess;

extern int32 cmd_util_install(
    uint32                nodemask,
    uint32                attrmask,
    cmd_set_func          cmdset,
    char                  *cmdstr,
    char                  *cmddesc,
    uint32                isConfig,
    cmd_ext_hidmatch_func hidmatch);

extern uint32 cmd_util_parserInstall_check(uint32 cmdidx);

extern int32 cmd_util_nosortCmd_install(
    uint32       nodemask,
    uint32       attrmask,
    cmd_set_func cmdset,
    char         *cmdstr,
    char         *cmddesc,
    uint32       isConfig,
    uint32       nosortidx);

extern int32 cmd_util_portlist_get(
    int                  argc,
    char                 *argv[],
    int                  token,
    sys_logic_portmask_t *pLogicPortmask);

extern int32 cmd_util_allportlist_get(
    int                  argc,
    char                 *argv[],
    int                  token,
    sys_logic_portmask_t *pLogicPortmask);

extern int32 cmd_util_normalPortlist_get(
    int                  argc,
    char                 *argv[],
    int                  token,
    sys_logic_portmask_t *pLogicPortmask);

extern int32 cmd_util_seqTokAry_get(
    char   *tokStr,
    uint32 maxTokSize,
    uint32 maxTokStrLen,
    char   **pTokAry);

extern int32 cmd_util_rangeValue_get(char *rangeStr, uint32 *pMin, uint32 *pMax);

extern void cmd_util_progressPrint_start(void);

extern void cmd_util_progressPrint_stop(void);

extern int32 cmd_util_cfgFileHdr_print(char *hdr);

#endif 

