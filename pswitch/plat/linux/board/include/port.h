
#ifndef __BOARD_PORT_H__
#define __BOARD_PORT_H__

#define PORTCONF_END_VAL      0xFFFF

typedef enum sys_port_conf_speed_e
{
    PORTCONF_SPEED_FORCE_10M,
    PORTCONF_SPEED_FORCE_100M,
    PORTCONF_SPEED_FORCE_COMBO_100M,
    PORTCONF_SPEED_FORCE_1000M,
    PORTCONF_SPEED_FORCE_2500M,
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
    uint32 attr; 
} sys_port_conf_t;

typedef enum sys_port_attr_e
{
    PORT_ATTR_POE = 0, 
    PORT_ATTR_COMBO_FIBER = 1, 
} sys_port_attr_t;

extern int32 board_conf_port_get(uint32 unit, uint32 port, sys_port_conf_t *pPortConf);

#endif 

