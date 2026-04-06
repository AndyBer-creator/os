
#ifndef __CMD_SYS_FS_H__
#define __CMD_SYS_FS_H__

extern uint32 isCopy2Running;

extern int32 cmd_sys_config_delete(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_dfltConfig_restore(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_DUAL_IMAGE

extern int32 cmd_sys_image_boot(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_image_delete(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_sys_flash_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_DUAL_IMAGE

extern int32 cmd_sys_bootvar_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_sys_config_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_config_save(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_file_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_config2Tftp_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_tftp2Config_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_config2Running_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_HW_USB
extern int32 cmd_sys_config2usb_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_usb2config_copy(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_usbDev_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_usb_enable(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_usb_disable(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_sys_config_show_features_module(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_banner_file_cfg(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif 

