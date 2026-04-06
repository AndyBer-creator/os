
#include <dirent.h>
#include <libgen.h>
#include <libfds/fds_default_init.h>
#include <libfds/fds_init.h>
#include <libcmd/cmd.h>

static void _wait_background_init(void)
{
    uint32 i = 0;

    while(SYS_ERR_OK != sal_util_file_exist("/tmp/init_port_done"))
    {
        usleep(20000);
        i++;
    }
    unlink("/tmp/init_port_done");
}

int32 cmd_run(void)
{
    char cmd[SYS_BUF128_LEN];
    int32 ret = SYS_ERR_FAILED;

    cmd_conf_consistent_check();

    if (SYS_ERR_OK != sal_util_file_exist(SYS_SEC_SSH_GEN_OK))
    {
        if (SYS_ERR_OK != sal_util_file_exist(SYS_SEC_FLH_KEY_DIR))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "mkdir %s", SYS_SEC_FLH_KEY_DIR);
            system(cmd);
        }

#ifdef CONFIG_USER_SSH_SSHD
        
        SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_RSA_V2));
        SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_DSA_V2));
#endif
        SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_RSA_HTTP));

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "touch %s", SYS_SEC_SSH_GEN_OK);
        system(cmd);
    }

    if (SYS_ERR_OK == sal_util_file_exist(CONF_STARTUP_FILE))
    {
        BOOTMSG_PRINTF("\n====== Defaults Initial [Start] ======\n");

        SYS_ERR_CHK(fds_init());
        _wait_background_init();

        BOOTMSG_PRINTF("====== Defaults Initial [Done] ======\n");

        BOOTMSG_PRINTF("\n====== Initial from startup-config [Start] ======\n");

        SYS_ERR_CHK(cmd_configFile2Shm_copy(CONF_STARTUP_FILE));

        BOOTMSG_PRINTF("====== Initial from startup-config [Done] ======\n");

        BOOTMSG_PRINTF("\n====== Post Initial [Start] ======\n");

        SYS_ERR_CHK(fds_post_init());

        BOOTMSG_PRINTF("====== Post Initial [Done] ======\n");

    }
    else
    {
        
        BOOTMSG_PRINTF("\n====== Factory Defaults Initial [Start] ======\n");

        SYS_ERR_CHK(fds_init());
        _wait_background_init();

        BOOTMSG_PRINTF("====== Factory Defaults Initial [Done] ======\n");

		if (SYS_ERR_OK == sal_util_file_exist(CONF_USRCFG_FLASH_FILE))
		{
			BOOTMSG_PRINTF("\n====== Initial from usr default [Start] ======\n");
		
			SYS_ERR_CHK(cmd_configFile2Shm_copy(CONF_USRCFG_FLASH_FILE));
		
			BOOTMSG_PRINTF("====== Initial from usr default [Done] ======\n");
		}
		
		else{
			BOOTMSG_PRINTF("\n====== Initial from default [Start] ======\n");

			SYS_ERR_CHK(cmd_configFile2Shm_copy(SYS_OEM_USR_CONF_FILE));

			BOOTMSG_PRINTF("====== Initial from default [Done] ======\n");
		}

        ret = fds_post_init();

        if (SYS_ERR_OK == ret)
        {
            
            ret = fds_entry_init();
#if 0
            if (SYS_ERR_OK == ret)
            {
                
                ret = cmd_conf_startupConfig_save();

                BOOTMSG_PRINTF("Generating file \"%s%s\"....%s\n",
                              CONF_STARTUP_BASENAME,
                              DFLT_SYS_CFG_EXTNAME,
                              ((ret != SYS_ERR_OK) ? "FAIL" : "OK"));
#endif

                sal_util_file_touch(CONF_FIRST_LOGIN_FLAG);
#if 0
            }
#endif

        }

    }

    return SYS_ERR_OK;
}

