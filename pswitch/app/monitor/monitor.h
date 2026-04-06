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
 * $Revision: 14469 $
 * $Date: 2010-11-29 10:50:21 +0800 (Mon, 29 Nov 2010) $
 *
 * Purpose : Board Configuration structure definition
 *
 * Feature : Board Configuration structure definition
 *
 */

#ifndef __BOARD_MONITOR_H__
#define __BOARD_MONITOR_H__

/*
 * Include Files
 */

/*
 * Symbol Definition
 */
#define PORTCONF_END_VAL      0xFFFF

/*
 * Data Type Declaration
 */

typedef enum sys_port_conf_speed_e
{
    PORTCONF_SPEED_FORCE_10M,
    PORTCONF_SPEED_FORCE_100M,
    PORTCONF_SPEED_FORCE_1000M,
    PORTCONF_SPEED_FORCE_10G,
    PORTCONF_SPEED_AUTO_10M,
    PORTCONF_SPEED_AUTO_100M,
    PORTCONF_SPEED_AUTO_1000M,
    PORTCONF_SPEED_AUTO_10M_100M,
    PORTCONF_SPEED_AUTO_10M_1000M,
    PORTCONF_SPEED_AUTO_100M_1000M,
    PORTCONF_SPEED_AUTO_ALL,
    PORTCONF_SPEED_END
} sys_port_conf_speed_t;

typedef enum sys_port_conf_duplex_e
{
    PORTCONF_DUPLEX_FORCE_FULL,
    PORTCONF_DUPLEX_FORCE_HALF,
    PORTCONF_DUPLEX_AUTO,
    PORTCONF_DUPLEX_END
} sys_port_conf_duplex_t;

typedef enum sys_port_type_e
{
    PORT_TYPE_100M = 0,
    PORT_TYPE_1000M,
    PORT_TYPE_2500M,
    PORT_TYPE_10G,
    PORT_TYPE_LAG,
    /* Combo Port */
    PORT_TYPE_COMBO_100M,
    PORT_TYPE_COMBO_1000M,
    PORT_TYPE_END,
} sys_port_type_t;

typedef struct sys_port_conf_s
{
    sys_port_type_t portType;
    uint32 usrPort;
    uint32 devId;
    uint32 phyPort;
    sys_port_media_t mediaType;
    sys_port_conf_speed_t  speed;
    sys_port_conf_duplex_t duplex;
    uint32 attr; /* bit0: PoE
                    .........
                  */
} sys_port_conf_t;

typedef enum sys_port_attr_e
{
    PORT_ATTR_POE = 0, /* bit0 */
} sys_port_attr_t;

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

/* Function Name:
 *      board_conf_port_get
 * Description:
 *      Get switch board port configuration
 * Input:
 *      unit   - unit id
 *      port   - physical port
 * Output:
 *      pPortConf - port config
 * Return:
 *      SYS_ERR_OK
 * Note:
 *      None
 */
extern int32 board_conf_port_get(uint32 unit, uint32 port, sys_port_conf_t *pPortConf);
extern int32 port_rate_monitor_init(void);

#endif /* __BOARD_PORT_H__ */

