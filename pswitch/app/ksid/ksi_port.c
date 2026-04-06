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
#include <libsal/sal_util.h>
#include <libsal/sal_port.h>
#include <libsal/sal_fiber.h>
#ifdef CONFIG_SYS_SNMP
#include <libsal/sal_snmp.h>
#endif
#include <libsal/sal_optical.h>
#include <ksi_port.h>

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
static int32 _ksi_port_opticalInsert_exec(sys_logic_port_t);

/* Function Name:
 *      ksi_port_cmd_mux
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
static int32 ksi_port_cmd_mux(unsigned int cmd, ksi_msg_t *pMsg)
{
    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    switch (cmd)
    {
        case KSI_PORT_LINK_UP_EXEC:
            SYS_ERR_CHK(ksi_port_linkUp_exec(pMsg));
            break;

        case KSI_PORT_LINK_DOWN_EXEC:
            SYS_ERR_CHK(ksi_port_linkDown_exec(pMsg));
            break;

        case KSI_PORT_COMBO_OPTICAL_INSERT_EXEC:
            SYS_ERR_CHK(ksi_port_opticalInsert_exec(pMsg));
            break;

        case KSI_PORT_COMBO_OPTICAL_REMOVE_EXEC:
            SYS_ERR_CHK(ksi_port_opticalRemove_exec(pMsg));
            break;

        default:
            return SYS_ERR_KSI_CMD_NOT_FOUND;
    }

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_port_init
 * Description:
 *      init function of ksi port module
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
int32 ksi_port_init(void)
{
#ifdef CONFIG_SYS_FIBER
    sys_fiber_detect_status_t fiber_detect;
    sys_port_media_t media;
#endif
    sys_logic_port_t port;

#ifdef CONFIG_SYS_UI_SNMP
 #ifdef CONFIG_SYS_MIB_TRAP
    sys_port_linkStatus_t link;

    FOR_EACH_LOGIC_PORT(port)
    {
        sal_port_linkStatus_get(port, &link);

        if(PORT_LINKUP == link)
            sal_snmp_linkUpDownTrap_send_new(port, link);
    }
 #endif
#endif

#ifdef CONFIG_SYS_FIBER
    /* sychronize fiber media */
    FOR_EACH_LOGIC_PORT(port)
    {
        if (IS_COMBO_PORT(port))
        {
            fiber_detect = FIBER_DETECT_STATUS_OFF;
            media = PORT_MEDIA_COPPER;

            SYS_ERR_CONTINUE(sal_fiber_detectStatus_get(port, &fiber_detect));
            SYS_ERR_CONTINUE(sal_port_linkMedia_get(port, &media));

            if (FIBER_DETECT_STATUS_ON == fiber_detect && PORT_MEDIA_FIBER == media)
                _ksi_port_opticalInsert_exec(port);
        }
        else if (IS_FIBER_PORT(port))
        {
            fiber_detect = FIBER_DETECT_STATUS_OFF;

            SYS_ERR_CONTINUE(sal_fiber_detectStatus_get(port, &fiber_detect));

            if (FIBER_DETECT_STATUS_ON == fiber_detect)
                _ksi_port_opticalInsert_exec(port);
        }
    }
#endif

    SYS_ERR_CHK(ksi_handler_register(KSI_MOD_PORT, ksi_port_cmd_mux));

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_port_linkUp_exec
 * Description:
 *      Execute actions on port link up
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
int32 ksi_port_linkUp_exec(ksi_msg_t *pMsg)
{
    sys_logic_port_t *pLogicPort = NULL;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pLogicPort = (sys_logic_port_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    if (NULL == pLogicPort)
        return SYS_ERR_KSI_DATA_EMPTY;

#ifdef CONFIG_SYS_UI_SNMP
 #ifdef CONFIG_SYS_MIB_TRAP
    /* Send snmp link-up/down trap */
    SYS_ERR_CHK(sal_snmp_linkUpDownTrap_send_new(*pLogicPort, PORT_LINKUP));
 #endif
#endif

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_port_linkDown_exec
 * Description:
 *      Execute actions on port link down
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
int32 ksi_port_linkDown_exec(ksi_msg_t *pMsg)
{
    sys_logic_port_t *pLogicPort = NULL;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pLogicPort = (sys_logic_port_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    if (NULL == pLogicPort)
        return SYS_ERR_KSI_DATA_EMPTY;

#ifdef CONFIG_SYS_UI_SNMP
 #ifdef CONFIG_SYS_MIB_TRAP
    /* Send snmp link-up/down trap */
    SYS_ERR_CHK(sal_snmp_linkUpDownTrap_send_new(*pLogicPort, PORT_LINKDOWN));
 #endif
#endif

    return SYS_ERR_OK;
}

/* Function Name:
 *      _ksi_port_opticalInsert_exec
 * Description:
 *      Execute actions on fiber module inserted
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
static int32 _ksi_port_opticalInsert_exec(sys_logic_port_t port)
{
    sys_port_media_t media;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_enable_t enable;
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    sys_port_errDisable_t reason;
#endif

    SYS_ERR_CHK(sal_port_linkMedia_get(port, &media));

    if (PORT_MEDIA_FIBER != media)
        return SYS_ERR_OK;

    if (IS_TRKLP(port) || IS_LP_TRKMBR(port))
        return SYS_ERR_CMD_TRUNK_MBR;

    if (IS_TGE_PORT(port) && !IS_COMBO_PORT(port))
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_port_speed_get(port, &speed));
    SYS_ERR_CHK(sal_port_enable_get(port, &enable));

    /* fiber only has full duplex */
    duplex.autoDuplex = DISABLED;
    duplex.ability = SYS_PORT_DUPLEX_FULL;

    /* Fiber auto configuration */
    if (ENABLED == speed.autoSpeed && SYS_PORT_SPEED_ALL == speed.ability)
        SYS_ERR_CHK(sal_optical_autoSensingConfig_set(port));
    else
        SYS_ERR_CHK(sal_port_speedDuplex_set(port, speed, duplex, media, FALSE));

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    SYS_ERR_CHK(sal_port_errDisable_get(port, &reason));

    if(SYS_PORT_ERRDISABLE_NONE == reason)
        SYS_ERR_CHK(sal_port_enable_set(port, enable));
    else
        SYS_ERR_CHK(rsd_port_enable_set(port, DISABLED));
#else
    SYS_ERR_CHK(sal_port_enable_set(port, enable));
#endif

    return SYS_ERR_OK;
}

/* Function Name:
 *      _ksi_port_opticalRemove_exec
 * Description:
 *      Execute actions on fiber module removed
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
static int32 _ksi_port_opticalRemove_exec(sys_logic_port_t port)
{
    sys_port_media_t media;
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;
    sys_enable_t enable;
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    sys_port_errDisable_t reason;
#endif

    SYS_ERR_CHK(sal_port_linkMedia_get(port, &media));

    if (IS_TRKLP(port) || IS_LP_TRKMBR(port))
        return SYS_ERR_CMD_TRUNK_MBR;

    if (IS_TGE_PORT(port) && !IS_COMBO_PORT(port))
        return SYS_ERR_OK;

    /* Sync from cfg */
    SYS_ERR_CHK(sal_port_speed_get(port, &speed));
    SYS_ERR_CHK(sal_port_duplex_get(port, &duplex));
    SYS_ERR_CHK(sal_port_enable_get(port, &enable));

    SYS_ERR_CHK(sal_port_speedDuplex_set(port, speed, duplex, media, FALSE));

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    SYS_ERR_CHK(sal_port_errDisable_get(port, &reason));

    if(SYS_PORT_ERRDISABLE_NONE == reason)
        SYS_ERR_CHK(sal_port_enable_set(port, enable));
    else
        SYS_ERR_CHK(rsd_port_enable_set(port, DISABLED));
#else
    SYS_ERR_CHK(sal_port_enable_set(port, enable));
#endif

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_port_opticalInsert_exec
 * Description:
 *      Execute actions on fiber module inserted
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
int32 ksi_port_opticalInsert_exec(ksi_msg_t *pMsg)
{
    sys_logic_port_t *pLogicPort = NULL;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pLogicPort = (sys_logic_port_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);
    if (NULL == pLogicPort)
        return SYS_ERR_KSI_DATA_EMPTY;

    SYS_ERR_CHK(_ksi_port_opticalInsert_exec(*pLogicPort));

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_port_opticalRemoved_exec
 * Description:
 *      Execute actions on fiber module removed
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
int32 ksi_port_opticalRemove_exec(ksi_msg_t *pMsg)
{
    sys_logic_port_t *pLogicPort = NULL;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pLogicPort = (sys_logic_port_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);
    if (NULL == pLogicPort)
        return SYS_ERR_KSI_DATA_EMPTY;

    SYS_ERR_CHK(_ksi_port_opticalRemove_exec(*pLogicPort));

    return SYS_ERR_OK;
}
