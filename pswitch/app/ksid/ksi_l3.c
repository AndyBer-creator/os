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
 * Purpose : Kernel to system interface - Port
 *
 * Feature : Kernel to system interface - Port
 *
 */

/*
 * Include Files
 */
#include <libsal/sal_l3.h>
#include <libsal/sal_util.h>
#include <ksi_l3.h>

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
static int32 _ksi_l3_intfLinkStatus_exec(ksi_msg_t *pMsg, sys_l3_intfLinkStatus_t status);
static int32 _ksi_l3_intfExist_exec(ksi_msg_t *pMsg, uint32 exist);

/* Function Name:
 *      ksi_l3_cmd_mux
 * Description:
 *      Dispatch command in ksi message for l3 module
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
static int32 ksi_l3_cmd_mux(unsigned int cmd, ksi_msg_t *pMsg)
{
    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    switch (cmd)
    {
        case KSI_L3_INTF_LINKDOWN_EXEC:
            SYS_ERR_CHK(_ksi_l3_intfLinkStatus_exec(pMsg, SYS_L3_INTFLINK_DOWN));
            break;

        case KSI_L3_INTF_LINKUP_EXEC:
            SYS_ERR_CHK(_ksi_l3_intfLinkStatus_exec(pMsg, SYS_L3_INTFLINK_UP));
            break;

        case KSI_L3_INTF_EXIST_EXEC:
            SYS_ERR_CHK(_ksi_l3_intfExist_exec(pMsg, TRUE));
            break;

        case KSI_L3_INTF_NOEXIST_EXEC:
            SYS_ERR_CHK(_ksi_l3_intfExist_exec(pMsg, FALSE));
            break;

        case KSI_L3_INTF_LINKADMINIPCHG_EXEC:
            SYS_ERR_CHK(_ksi_l3_intfLinkStatus_exec(pMsg, SYS_L3_INTFLINK_ADMINIPCHG));
            break;

        case KSI_L3_INTF_MGMTVLAN_CHG_EXEC:
            SYS_ERR_CHK(sal_l3_intfMgmt_refresh());
            break;

        default:
            return SYS_ERR_KSI_CMD_NOT_FOUND;
    }

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_l3_init
 * Description:
 *      init function of ksi l3 module
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
int32 ksi_l3_init(void)
{
    SYS_ERR_CHK(ksi_handler_register(KSI_MOD_L3, ksi_l3_cmd_mux));

    return SYS_ERR_OK;
}

static int32 _ksi_l3_intfLinkStatus_exec(ksi_msg_t *pMsg, sys_l3_intfLinkStatus_t status)
{
    sys_l3_intfId_t *pId = NULL;
    sys_l3_intfStsChg_t stsChg;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pId = (sys_l3_intfId_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    if (NULL == pId)
        return SYS_ERR_KSI_DATA_EMPTY;

#if 0
#ifdef CONFIG_SYS_UI_SNMP
 #ifdef CONFIG_SYS_MIB_TRAP
    /* Send snmp link-up/down trap */
    SYS_ERR_CHK(sal_snmp_linkUpDownTrap_send(*pLogicPort, PORT_LINKUP));
 #endif
#endif
#endif

    SYS_MEM_CLEAR(stsChg);

    sal_l3_intfLinkStatusChg_get(pId, &stsChg);

    stsChg.stsChg = TRUE;
    stsChg.linkup = status;

    sal_l3_intfLinkStatusChg_set(pId, &stsChg);

    return SYS_ERR_OK;
}

static int32 _ksi_l3_intfExist_exec(ksi_msg_t *pMsg, uint32 exist)
{
    sys_l3_intfId_t *pId = NULL;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pId = (sys_l3_intfId_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    if (NULL == pId)
        return SYS_ERR_KSI_DATA_EMPTY;

    SYS_ERR_CHK(sal_l3_intfvExist_set(pId, exist));

    return SYS_ERR_OK;
}


