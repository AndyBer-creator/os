
#ifndef __CMD_VLAN_H__
#define __CMD_VLAN_H__

#include <libsal/sal_vlan.h>

#define CMD_ERR_CONTINUE_VLAN_PORTMODE(mode, lport) \
{                                                   \
    sys_vlan_portMode_t __mode;                     \
    sal_vlan_portAdminMode_get(lport, &__mode);     \
    if (mode != __mode)                             \
    {                                               \
        char __portStr[CAPA_PORT_STR_LEN];           \
        SYS_MEM_CLEAR(__portStr);                   \
        LP2STR(lport, __portStr);                   \
        SYS_PRINTF("Port %s: Port mode is not %s\n", __portStr, text_vlan_portMode[mode].text); \
        continue;                                   \
    }                                               \
}

extern int32 cmd_vlan_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_dfltVlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portMembership_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTOCOL_VLAN

extern int32 cmd_vlan_proto_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portProto_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_MANAGEMENT_VLAN

extern int32 cmd_vlan_mgmt_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_vlan_create_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_name_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_defaultVid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portPvid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_igrFilterEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portIgrFilterEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portAcceptFrameType_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portGeneralVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portAccessVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_VLAN_QINQ

extern int32 cmd_vlan_portDot1qtunnelVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portDot1qTunnelTpid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_vlan_portTrunkVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portDfltVlanTagged_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portForbidDfltVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portForbidVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_MANAGEMENT_VLAN

extern int32 cmd_vlan_mgmVid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_vlan_save(FILE *fp);

extern int32 cmd_vlan_port_save(FILE *fp, sys_logic_port_t port);

#endif
