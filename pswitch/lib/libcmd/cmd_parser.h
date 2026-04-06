
#ifndef __CMD_PARSER_H__
#define __CMD_PARSER_H__
#include <libcmd/cmd.h>

#define MAX_CMD_WORDS 20
#define MAX_CMD_ARGVS 10
#define CMD_TMP_DIFF_FILE_BASENAME "/tmp/cmd_tmp_diff"
#define SYS_CMD_CONFIGFILEHDR_LEN   1024

extern uint32 g_cmdIsDiff;
extern uint32 g_cmdIsParserRun;
extern uint32 g_isCopy2Running;

#define CMD_IS_PARSER_RUN() (cmd_isParserRun_ret())
#define CMD_IS_CLI_RUN()    (!cmd_isParserRun_ret())
#define IS_CONFIG_FILE_VALID(filename)   (TRUE == cmd_configValidFileCheck_ret(filename))
#define IS_CONFIG_FILE_INVALID(filename) (FALSE == cmd_configValidFileCheck_ret(filename))
#define IS_CONFIG_FILEHDR_VALID(filename)   (TRUE == cmd_configValidFileHdrCheck_ret(filename))
#define IS_CONFIG_FILEHDR_INVALID(filename) (FALSE == cmd_configValidFileHdrCheck_ret(filename))

#define CMD_WRITE_TO_FILE(fp, str)          \
do {                                        \
    fwrite(str, 1, osal_strlen(str), fp);   \
    g_cmdIsDiff = TRUE;                     \
} while(0)

#define CMD_WRITE_INDENT(fp, num)           \
do {                                        \
    uint32 __i;                             \
    for (__i = 0; __i < num; __i++)         \
        fputc(' ', fp);                     \
} while (0)

#define CMD_WRITE_TO_FILE_INDENT1(fp, str) \
do {                                       \
    CMD_WRITE_INDENT(fp, 1);               \
    CMD_WRITE_TO_FILE(fp, str);            \
} while (0)

#define CMD_WRITE_TO_FILE_INDENT2(fp, str) \
do {                                       \
    CMD_WRITE_INDENT(fp, 2);               \
    CMD_WRITE_TO_FILE(fp, str);            \
} while (0)

#define CMD_WRITE_TO_FILE_INDENT3(fp, str) \
do {                                       \
    CMD_WRITE_INDENT(fp, 3);               \
    CMD_WRITE_TO_FILE(fp, str);            \
} while (0)

#define CMD_WRITE_TO_FILE_INTFVLAN(fp, str, vmskStr)       \
do {                                                       \
    char _cmd[MAX_CMD_STRLEN];                             \
    osal_sprintf(_cmd, "interface range vlan%s\n", vmskStr); \
    CMD_WRITE_TO_FILE(fp, _cmd);                           \
    CMD_WRITE_TO_FILE_INDENT1(fp, str);                    \
    CMD_WRITE_TO_FILE_INDENT1(fp, "exit\n");               \
} while (0)

#define CMD_SAVE_IF_TRKLP_SKIP(__port)   \
do {                                       \
    if (IS_TRKLP(__port))                  \
    {                                      \
        return SYS_ERR_OK;                 \
    }                                      \
} while (0)

extern int32 cmd_parser_init(void);

extern int32 cmd_parser_run(FILE *fp);

extern int32 cmd_configFile2Shm_copy(char *filename);

extern int32 cmd_configValidFile_convert(char *filename);

extern uint32 cmd_configValidFileCheck_ret(char *filename);

extern uint32 cmd_configValidFileHdrCheck_ret(char *filename);

extern int32 cmd_parser_intfPortCmd_save(uint32 catid, sys_logic_port_t port, FILE *fp);

#ifdef CONFIG_SYS_L3_ROUTE

extern int32 cmd_parser_intfVlanCmd_save(uint32 catid, sys_vid_t vid, FILE *fp);

extern int32 cmd_parser_intfLoCmd_save(uint32 catid, uint32 id, FILE *fp);
#endif

extern int32 cmd_parser_runningFile_print(uint32 catid, char *toFile);

extern int32 cmd_catName_get(cat_id_t id, char *catname);

extern uint32 cmd_isParserRun_ret(void);
extern int32 cmd_parser_common_running_clone_config_File_print(uint32 catid, char *toFile);
extern int32 cmd_parser_config_by_module_print(char* pkey, char *toFile);
#endif 

