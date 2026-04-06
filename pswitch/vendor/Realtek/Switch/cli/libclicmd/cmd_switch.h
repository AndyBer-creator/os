
#ifndef __CMD_SWITCH_H__
#define __CMD_SWITCH_H__

#include <libsal/sal_switch.h>

extern int32 cmd_switch_maxFrameSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

int32 cmd_switch_save(FILE *fp);

#endif 

