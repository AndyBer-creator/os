
#include <libsal/sal_util.h>
#include <libsal/sal_ssh.h>

#ifdef CONFIG_USER_SSH_SSHD

int32 sal_ssh_sshdEnable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_sshdEnable_set(enable));

    if (ENABLED == enable)
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_SSH_START);
    }
    else
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_SSH_STOP);

        system("handler -c ssh &");
    }

    SYS_ERR_CHK(sal_sys_inetdConfig_set());

    return SYS_ERR_OK;
}

int32 sal_sys_sshd_port_set(uint32 port)
{
	uint32        oldport = 0;
	FILE  *fp = NULL;
	char buf[SYS_BUF256_LEN];
	char telnetstr[SYS_BUF64_LEN] = {0};
	char serverstr[SYS_BUF64_LEN] = {0};
	
	SYS_ERR_CHK(cfg_sys_sshd_port_get(&oldport));

	if(port == oldport)
		return SYS_ERR_OK;
		
	if(port != DEFAULT_SSH_PORT_VAL) 
	{
		if (NULL == (fp = fopen(ETC_SERVERS_FILE, "r")))
			return SYS_ERR_OK;
		osal_sprintf(telnetstr, "ssh%u", port);	
		SYS_MEM_CLEAR(buf);
		while (fgets(buf, sizeof(buf), fp))
		{

			if(osal_strstr(buf, telnetstr))
			{
				fclose(fp);
				SYS_ERR_CHK(cfg_sys_sshd_port_set(port));
				return SYS_ERR_OK;
			}
			SYS_MEM_CLEAR(buf);
		}
		fclose(fp);

		if(NULL == (fp = fopen(ETC_SERVERS_FILE, "a+")))
			return SYS_ERR_FAILED;

		osal_sprintf(serverstr, "ssh%u %u/tcp\n", port, port);
		fputs(serverstr, fp);
		osal_sprintf(serverstr, "ssh%u %u/udp\n", port, port);
		fputs(serverstr, fp);
		fclose(fp);
	}

    SYS_ERR_CHK(cfg_sys_sshd_port_set(port));
    return SYS_ERR_OK;
}

int32 sal_ssh_sshdEnable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_sshdEnable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_sys_sshd_port_get(uint32 *port)
{
    SYS_PARAM_CHK((NULL == port), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_sshd_port_get(port));

    return SYS_ERR_OK;
}

#endif

