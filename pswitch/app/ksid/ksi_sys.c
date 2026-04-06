/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 14054 $
 * $Date: 2010-11-09 19:03:46 +0800 (Tue, 09 Nov 2010) $
 *
 * Purpose : Kernel to system interface - System
 *
 * Feature : Kernel to system interface - System
 *
 */

/*
 * Include Files
 */
#include <ksi_sys.h>
#include <libsal/sal_led.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_util.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

/* Function Name:
 *      ksi_sys_cmd_mux
 * Description:
 *      Dispatch command in ksi message for sys module
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 * Note:
 *      None
 */
static int32 ksi_sys_cmd_mux(unsigned int cmd, ksi_msg_t *pMsg)
{
    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    switch (cmd)
    {
        case KSI_SYS_RESET_BUTTON_ACTION_EXEC:
            SYS_ERR_CHK(ksi_sys_resetButtonAction_exec(pMsg));
            break;

        case KSI_SYS_SNMP_MSG_SEND:
#ifdef CONFIG_SYS_UI_SNMP
            SYS_ERR_CHK(ksi_sys_snmp_msg_send(pMsg));
#endif
            break;

        case KSI_SYS_HOSTADDR_MSG_GET:
            SYS_ERR_CHK(ksi_sys_hostAddr_get(pMsg));


#ifdef CONFIG_SYS_ADVANCE_DNS
        case KSI_SYS_DNS_NOTIFY:
            SYS_ERR_CHK(sal_sys_dnsHosts_create());
            break;
#endif
        default:
            return SYS_ERR_KSI_CMD_NOT_FOUND;
    }

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_sys_init
 * Description:
 *      init function of ksi sys module
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 * Note:
 *      None
 */
int32 ksi_sys_init(void)
{
    SYS_ERR_CHK(ksi_handler_register(KSI_MOD_SYS, ksi_sys_cmd_mux));

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_sys_resetButtonAction_exec
 * Description:
 *      Execute reset button action base on status
 * Input:
 *      pMsg - pointer of kernel to user message
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 *      SYS_ERR_KSI_DATA_EMPTY
 * Note:
 *      None
 */
int32 ksi_sys_resetButtonAction_exec(ksi_msg_t *pMsg)
{
    uint32 *pResetStatus = NULL;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pResetStatus = (uint32*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    if (NULL == pResetStatus)
        return SYS_ERR_KSI_DATA_EMPTY;

    switch (*pResetStatus)
    {
        case SYS_RESET_ACT_RESTORE_FACTORY:
            SYS_ERR_CHK(sal_sys_restore_defaults(ENABLED));
            SYS_ERR_CHK(sal_led_systemCtrl_set(SYS_LED_STATE_ALLPORT_ON));
            SYS_PRINTF("Restore Factory Default Success!\n");
            break;

        case SYS_RESET_ACT_RESTORE_ACCOUNT:
            {
                uint32 i;
                sys_userInfo_t userInfo;

                for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
                {
                    osal_memset(&userInfo, 0, sizeof(userInfo));
                    SYS_ERR_CHK(sal_sys_localUser_get(i, &userInfo));
                    if (userInfo.priv < 0)
                        continue;

                    SYS_ERR_CHK(sal_sys_localUser_del(userInfo.username));
                }
                osal_memset(&userInfo, 0, sizeof(userInfo));
                sal_sys_get_sys_defaul_user(&userInfo);
                //osal_strcpy(userInfo.username, DFLT_SYS_ADMIN_USERNAME);
               // osal_strcpy(userInfo.password, DFLT_SYS_ADMIN_PASSWORD);
                userInfo.priv = DFLT_SYS_ADMIN_PRIV;
                userInfo.passwdType = SYS_PASSWD_TYPE_ENCRYPT;
                SYS_ERR_CHK(sal_sys_localUser_add(userInfo));
            }
            SYS_PRINTF("Restore Default Username/Password Success!\n");
            break;

        case SYS_RESET_ACT_REBOOT:
        default:
            break;
    }

    if (SYS_RESET_ACT_NONE != *pResetStatus)
    {
        SYS_PRINTF("Rebooting now...\n");
        SYS_ERR_CHK(sal_sys_reboot());
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_UI_SNMP
/* Function Name:
 *      ksi_sys_snmp_msg_send
 * Description:
 *      Send snmp trap message
 * Input:
 *      pMsg - pointer of kernel to user message
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 *      SYS_ERR_KSI_DATA_EMPTY
 * Note:
 *      None
 */
int32 ksi_sys_snmp_msg_send(ksi_msg_t *pMsg)
{
    ksi_sys_snmp_msg_t *data;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    data = (ksi_sys_snmp_msg_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    if (NULL == data)
        return SYS_ERR_KSI_DATA_EMPTY;

#ifdef CONFIG_SYS_MIB_TRAP
    SYS_ERR_CHK(sal_snmp_portSecurityTrap_send(data->oidStr, 0, data->msgStr));
#endif

    return SYS_ERR_OK;
}

#endif

int32 ksi_sys_hostAddr_get(ksi_msg_t *pMsg)
{
	int32   ret;
    char	*pHostName;
    uint32	*pSrvIp = NULL;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

	pHostName = (char*) pMsg->tousr_data;
	pSrvIp = osal_alloc(4);

	ret = sal_util_getHostByName(pSrvIp, pHostName);
	if (ret != SYS_ERR_OK)
	    *pSrvIp = -1;

	ksi_msg_reply(pMsg, 4, pSrvIp);
	osal_free(pSrvIp);

    return SYS_ERR_OK;
}

