
#ifndef __CMD_EXT_HIDDEN_H__
#define __CMD_EXT_HIDDEN_H__

extern int32 cmd_ext_hidden_parserOnly_chk(struct cmd_element *cmd, struct vty *vty);

extern int32 cmd_ext_hidden_if_chk(struct cmd_element *cmd, struct vty *vty);

#ifdef CONFIG_SYS_L3_ROUTE

extern int32 cmd_ext_hidden_ifRange_chk(struct cmd_element *cmd, struct vty *vty);
#endif

extern int32 cmd_ext_hidden_nonComboPort_chk(struct cmd_element *cmd, struct vty *vty);

extern int32 cmd_ext_hidden_nonFiberPort_chk(struct cmd_element *cmd, struct vty *vty);

extern int32 cmd_ext_hidden_nonGigaPort_chk(struct cmd_element *cmd, struct vty *vty);

extern int32 cmd_ext_hidden_nonTenGigaPort_chk(struct cmd_element *cmd, struct vty *vty);

extern int32 cmd_ext_hidden_tenGigaPort_chk(struct cmd_element *cmd, struct vty *vty);

extern int32 cmd_ext_hidden_2_5GigaPort_chk(struct cmd_element *cmd, struct vty *vty);

extern int32 cmd_ext_hidden_autoAllPort_chk(struct cmd_element *cmd, struct vty *vty);

extern int32 cmd_ext_hidden_auto1GPort_chk(struct cmd_element *cmd, struct vty *vty);

extern int32 cmd_ext_hidden_auto2500Port_chk(struct cmd_element *cmd, struct vty *vty);
#endif 

