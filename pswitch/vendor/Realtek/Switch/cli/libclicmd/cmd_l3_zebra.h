
#ifndef __CMD_L3_ZEBRA_H__
#define __CMD_L3_ZEBRA_H__

int32 cmd_l3_router_id_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_router_id_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_router_save(FILE *fp);

#endif 

