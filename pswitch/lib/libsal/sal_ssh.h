
#ifndef __SAL_SSH_H__
#define __SAL_SSH_H__

#define SSH_RSA1                    "rsa1"
#define SSH_RSA2                    "rsa2"
#define SSH_DSA2                    "dsa2"

#define SYS_SSH_RSAV1_PRIKEY_FILE         SYS_SEC_FLH_KEY_DIR"/ssh_host_rsa_v1_key"
#define SYS_SSH_RSAV2_PRIKEY_FILE         SYS_SEC_FLH_KEY_DIR"/ssh_host_rsa_v2_key"
#define SYS_SSH_DSAV2_PRIKEY_FILE         SYS_SEC_FLH_KEY_DIR"/ssh_host_dsa_v2_key"

#define SYS_SSH_RSAV1_PUBKEY4SSHD_FILE    SYS_SEC_FLH_KEY_DIR"/ssh_host_rsa_v1_key.pub"
#define SYS_SSH_RSAV2_PUBKEY4SSHD_FILE    SYS_SEC_FLH_KEY_DIR"/ssh_host_rsa_v2_key.pub"
#define SYS_SSH_DSAV2_PUBKEY4SSHD_FILE    SYS_SEC_FLH_KEY_DIR"/ssh_host_dsa_v2_key.pub"

#define SYS_SSH_RSAV1_PUBKEY_FILE         SYS_SEC_RAM_KEY_DIR"/ssh_host_rsa_v1_pubkey"
#define SYS_SSH_RSAV2_PUBKEY_FILE         SYS_SEC_RAM_KEY_DIR"/ssh_host_rsa_v2_pubkey"
#define SYS_SSH_DSAV2_PUBKEY_FILE         SYS_SEC_RAM_KEY_DIR"/ssh_host_dsa_v2_pubkey"

#ifdef CONFIG_USER_SSH_SSHD

extern int32 sal_ssh_sshdEnable_set(sys_enable_t enable);

extern int32 sal_ssh_sshdEnable_get(sys_enable_t *pEnable);
extern int32 sal_sys_sshd_port_set(uint32 port);
extern int32 sal_sys_sshd_port_get(uint32 *port);
#endif

#endif 

