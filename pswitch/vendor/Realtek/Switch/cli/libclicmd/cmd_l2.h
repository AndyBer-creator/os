
#ifndef __CMD_L2_H__
#define __CMD_L2_H__

extern int32 cmd_l2_macTable_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l2_macTableVlan_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l2_macAging_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l2_macAddress_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l2_macAddressTable_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l2_macAddressTableCnt_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l2_agingtime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_l2_agingtime_never_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l2_staticMacEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32  cmd_l2_staticMacEntry_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_BLACK_HOLE_MAC

extern int32  cmd_l2_filterMacEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_l2_save(FILE *fp);

#endif 

