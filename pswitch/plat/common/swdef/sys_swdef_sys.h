
#ifndef __SYS_SWDEF_SYS_H__
#define __SYS_SWDEF_SYS_H__
#define CAPA_SYS_NAME_LEN                           256
#define CAPA_SYS_LOCATION_LEN                       255
#define CAPA_SYS_CONTACT_LEN                        255
#define CAPA_SYS_DESC_LEN                           512
#define CAPA_SYS_SN_LEN                             64
#define CAPA_SYS_VER_LEN                            64
#define CAPA_SYS_USERNAME_LEN                       32
#define CAPA_SYS_PASSWORD_LEN                       32
#define CAPA_SYS_PASSWORD_MIX_LEN                   CONFIG_SYS_USER_PS_MIX_LENGTH
#define CAPA_SYS_CRYPT_PASSWORD_LEN                 64      
#define CAPA_SYS_LOCALUSER_NUM                      8
#define CAPA_SYS_WEB_SESSION_NUM                    10
#define CAPA_SYS_INTF_IPV6_ADDR_NUM                 16
#define CAPA_SYS_IPADDR_LEN                         16
#define CAPA_SYS_MANUFACTURER                       64
#define CAPA_SYS_PRODUCT_CLASS                      64
#define CAPA_SYS_CHAR_64                            64

#ifdef CONFIG_SYS_ADVANCE_DNS
#define CAPA_SYS_DNS_CACHE                          64
#define CAPA_SYS_DNS_NAMESERVER_NUM                 4
#define CAPA_SYS_DNS_STATIC_HOST_NUM                32
#endif
#if !defined(CONFIG_SYS_AAA)
#define BOND_AAA_PRIV_MIN                           0
#define BOND_AAA_PRIV_MAX                           15
#endif
#define BOND_SYS_LINE_TIMEOUT_MIN                   0
#define BOND_SYS_LINE_TIMEOUT_MAX                   65535
#define DFLT_SYS_VER                                DEFDB_STR(DEF_SYS_VERSION)
#define DFLT_SYS_PLATFORM_NAME                      DEFDB_STR(DEF_SYS_PLATFORM)
#define DFLT_SYS_COPYRIGHT                          DEFDB_STR(DEF_SYS_COPYRIGHT)
#define DFLT_SYS_MANUFACTURER                       DEFDB_STR(DEF_SYS_MANUFACTURER)
#define DFLT_SYS_WEB_URL                            DEFDB_STR(DEF_SYS_VENDOR_URL)
#define DFLT_SYS_NAME                               DEFDB_STR(DEF_SYS_NAME)
#define DFLT_SYS_LOCATION                           DEFDB_STR(DEF_SYS_LOCATION)
#define DFLT_SYS_CONTACT                            DEFDB_STR(DEF_SYS_CONTACT)
#define DFLT_SYS_LOGIN_LOGO                         DEFDB_STR(DEF_SYS_LOGIN_LOGO)
#define DFLT_SYS_HOME_LOGO                          DEFDB_STR(DEF_SYS_HOME_LOGO)
#define DFLT_SYS_SUPPORT_INFO                       DEFDB_STR(DEF_SYS_SUPPORT_INFO)
#define DFLT_SYS_CONTACT_TELE                       DEFDB_STR(DEF_SYS_CONTACT_TELE)

#define DFLT_SYS_SN_VAR                             DEFDB_STR(DEF_SYS_SN_VAR)
#define DFLT_SYS_HWVER_VAR                          DEFDB_STR(DEF_SYS_HWVER_VAR)

#define DFLT_SYS_IMG_EXTNAME                        DEFDB_STR(DEF_SYS_IMG_EXTNAME)
#define DFLT_SYS_CFG_EXTNAME                        DEFDB_STR(DEF_SYS_CFG_EXTNAME)
#define DFLT_SYS_CFG_HEAD_START                     DEFDB_STR(DEF_SYS_CFG_HEAD_START)
#define DFLT_SYS_CFG_HEAD_END                       DEFDB_STR(DEF_SYS_CFG_HEAD_END)
#define DFLT_SYS_FW_NAME                            DEFDB_STR(DEF_SYS_FW_NAME)

#define DFLT_SYS_ADMIN_USERNAME                     DEFDB_STR(DEF_SYS_ADMIN_USERNAME)
#define DFLT_SYS_ADMIN_PASSWORD                     DEFDB_STR(DEF_SYS_ADMIN_PASSWORD)
#define DFLT_SYS_ADMIN_PRIV                         15              
#define DFLT_SYS_ENABLE_PASSWORD                    DEFDB_STR(DEF_SYS_ENABLE_PASSWORD)

#if !defined(CONFIG_SYS_L3_ROUTE)
#define DFLT_SYS_IPV4_ADDR                          DEFDB_STR(DEF_SYS_IPV4_ADDR)
#define DFLT_SYS_IPV4_NETMASK                       DEFDB_STR(DEF_SYS_IPV4_NETMASK)
#define DFLT_SYS_IPV4_GW                            DEFDB_STR(DEF_SYS_IPV4_GW)
#define DFLT_SYS_IPV4_DHCPC_EBL                     DEFDB_VAL(DEF_SYS_IPV4_DHCPC_EBL)
#define DFLT_SYS_IPV6_ADDR                          DEFDB_STR(DEF_SYS_IPV6_ADDR)
#define DFLT_SYS_IPV6_PREFIX_LEN                    DEFDB_VAL(DEF_SYS_IPV6_PREFIX_LEN)
#define DFLT_SYS_IPV6_GW                            DEFDB_STR(DEF_SYS_IPV6_GW)
#define DFLT_SYS_IPV6_AUTOCONF_EBL                  DEFDB_VAL(DEF_SYS_AUTOCONF_EBL)
#define DFLT_SYS_IPV6_DHCPC_EBL                     DEFDB_VAL(DEF_SYS_IPV6_DHCPC_EBL)
#define DFLT_SYS_IPV6_DHCPC_STATELESS_EBL           ENABLED
#define DFLT_SYS_IPV6_DHCPC_STATELESS_REFRESH       86400
#define DFLT_SYS_IPV6_DHCPC_STATELESS_REFRESH_MIN   600
#endif

#ifdef CONFIG_SYS_ADVANCE_DNS
#define DFLT_SYS_ADV_DNS_ENBL                       ENABLED
#define DFLT_SYS_ADV_DNS_DOMAIN                     ""
#endif

#define DFLT_SYS_TELNET_EBL                         DEFDB_VAL(DEF_SYS_TELNET_EBL)
#ifdef CONFIG_USER_SSH_SSHD
#define DFLT_SYS_SSH_EBL                            DEFDB_VAL(DEF_SYS_SSH_EBL)
#endif
#define DFLT_SYS_HTTP_EBL                           DEFDB_VAL(DEF_SYS_HTTP_EBL)
#define DFLT_SYS_HTTPS_EBL                          DEFDB_VAL(DEF_SYS_HTTPS_EBL)
#define DFLT_SYS_SESSION_TIMEOUT_HTTP               DEFDB_VAL(DEF_SYS_SESSION_TIMEOUT_HTTP)    
#define DFLT_SYS_SESSION_TIMEOUT_HTTPS              DEFDB_VAL(DEF_SYS_SESSION_TIMEOUT_HTTPS)   
#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
#define DFLT_SYS_SESSION_TIMEOUT_CONSOLE            DEFDB_VAL(DEF_SYS_SESSION_TIMEOUT_CONSOLE) 
#define DFLT_SYS_SESSION_TIMEOUT_TELNET             DEFDB_VAL(DEF_SYS_SESSION_TIMEOUT_TELNET)  
#define DFLT_SYS_SESSION_TIMEOUT_SSH                DEFDB_VAL(DEF_SYS_SESSION_TIMEOUT_SSH)     
#endif
#define DFLT_SYS_USER_ACCESS_METH                   DEFDB_VAL(DEF_SYS_USER_ACCESS_METH)     
#define DFLT_SYS_USER_ACCESS_METH_ALL               0x1F
#endif 

