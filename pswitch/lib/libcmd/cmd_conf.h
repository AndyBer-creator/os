
#ifndef __CMD_CONF_H__
#define __CMD_CONF_H__

extern int32 cmd_conf_startupConfig_save(void);

extern int32 cmd_conf_startupConfig_copy(char *srcFname);

#ifdef CONFIG_SYS_BACKUP_CONFIGURATION

extern int32 cmd_conf_backupConfig_save(char *fname);
#endif

extern uint32 cmd_conf_configIdentical_ret(char *cfgFile1, char *cfgFile2);

extern int32 cmd_conf_consistent_check(void);

#endif 

