
#include <libcmd/cmd.h>

#include <libsal/sal_ssh.h>

int32 cmd_ssh_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
    {
        if ((argv[0][0] == 'a') || (argv[0][1] == '1'))
        {
            unlink(SYS_SSH_RSAV1_PRIKEY_FILE);
            unlink(SYS_SSH_RSAV1_PUBKEY4SSHD_FILE);
            unlink(SYS_SSH_RSAV1_PUBKEY_FILE);
        }
        if ((argv[0][0] == 'a') || (argv[0][1] == '2'))
        {
            unlink(SYS_SSH_RSAV2_PRIKEY_FILE);
            unlink(SYS_SSH_DSAV2_PRIKEY_FILE);
            unlink(SYS_SSH_RSAV2_PUBKEY4SSHD_FILE);
            unlink(SYS_SSH_DSAV2_PUBKEY4SSHD_FILE);
            unlink(SYS_SSH_RSAV2_PUBKEY_FILE);
            unlink(SYS_SSH_DSAV2_PUBKEY_FILE);
        }
    }
    else
    {
        if ((argv[0][0] == 'a') || (argv[0][1] == '1'))
        {
            SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_RSA_V1));
        }

        if ((argv[0][0] == 'a') || (argv[0][1] == '2'))
        {
            SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_RSA_V2));
            SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_DSA_V2));
        }
    }

    return SYS_ERR_OK;
}

