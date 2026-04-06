
#ifndef __CMD_CUSTOM_H__
#define __CMD_CUSTOM_H__

#include <libcustom/sal_custom.h>

extern const cmd_inst_tbl_t cmdCustomSetTbl[];
extern const cmd_save_tbl_t cmdCustomSaveTbl[];
extern const cmd_save_intfPort_tbl_t cmdCustomSaveIntfPortTbl[];
extern const cmd_save_line_tbl_t cmdCustomSaveLineTbl[];

extern int32 cmd_custom_portEnableExample_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_custom_enableExample_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_custom_portEnableExample_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_custom_enableExample_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_custom_portEnableExample_save(FILE *fp, sys_logic_port_t port);

extern int32 cmd_custom_enableExample_save(FILE *fp);
#endif

