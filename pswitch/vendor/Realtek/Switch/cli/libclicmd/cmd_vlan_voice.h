
#ifndef __CMD_VLAN_VOICE_H__
#define __CMD_VLAN_VOICE_H__

#include <libsal/sal_vlan_voice.h>

extern int32 cmd_vlan_voice_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voiceType_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voiceVlanState_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voiceVlanVid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voiceVlanPri_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voiceVlanVpt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voiceVlanDscp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portvoiceVlancosMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voiceVlanAging_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voiceVlanOui_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voiceVlanOui_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portVoiceVlanEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portVoiceVlanMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_voice_save(FILE *fp);

extern int32 cmd_vlan_voice_port_save(FILE *fp, sys_logic_port_t port);
#endif

