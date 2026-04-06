/*
 * Zebra configuration command interface routine
 * Copyright (C) 1997, 98 Kunihiro Ishiguro
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2, or (at your
 * option) any later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _ZEBRA_COMMAND_H
#define _ZEBRA_COMMAND_H

#include "vector.h"
#include "vty.h"
#include <libsal/sal_util.h>

#include <common/sys_error.h>
////#include "route_types.h"

#ifdef CONFIG_SYS_IPV6
#define HAVE_IPV6
#endif

#ifdef CONFIG_USER_SSH_SSHD
#define CLI_SLEEP_FOR_REMOTE()              \
do{                                         \
    if ((access_flag == SYS_ACCESS_CLI_TELNET) || (access_flag == SYS_ACCESS_CLI_SSH))  \
        sleep(1);   \
}while(0);
#else
#define CLI_SLEEP_FOR_REMOTE()              \
do{                                         \
    if (access_flag == SYS_ACCESS_CLI_TELNET)  \
        sleep(1);   \
}while(0);
#endif

#define CMD_PRIV_15  15
#define CMD_PRIV_14  14
#define CMD_PRIV_13  13
#define CMD_PRIV_12  12
#define CMD_PRIV_11  11
#define CMD_PRIV_10  10
#define CMD_PRIV_9   9
#define CMD_PRIV_8   8
#define CMD_PRIV_7   7
#define CMD_PRIV_6   6
#define CMD_PRIV_5   5
#define CMD_PRIV_4   4
#define CMD_PRIV_3   3
#define CMD_PRIV_2   2
#define CMD_PRIV_1   1
#define CMD_PRIV_0   0

#define PRINT_DECIMAL        1 /*按照整数进行打印*/
#define PRINT_IP_ADDRESS  2/*按照IP 地址进行打印*/
#define PRINT_STRING          4/*按照字符串进行打印*/
#define PRINT_CHAR             8/*按照字符行打印*/
#define PRINT_ENUM            16/*按照枚举进行打印*/
#define PRINT_MAC_ADDRESS 32/*按照MAC地址进行打印*/
#define PRINT_PORT      64 /*按照端口进行打印*/
#define PRINT_TIME      128/*按照时间进行打印*/
#define PRINT_FLOAT   3   /*按照浮点数进行打印*/
#define PRINT_GGMPPORT      5 /*直接打印端口号*/

/*规定一次最多打印字符的个数*/
#define MAX_COMMAND_LEN         1024
#define MAX_PRINT_CHAR_LENGTH   (MAX_COMMAND_LEN + 64)

#define ROWS_PER_PAGE  22
#define LINE_CHAR_MAX  256
#define MAX_LINE_CHAR  160

#define CTRL_C          0x03


#define PRINTOR_MOVE_RIGHT(x)\
{\
    memset(pszTmp, ' ', (x));\
    pszTmp += (x);\
}

#define INSTALL_ELEMENT(type, priv, cmd) install_element(type, priv, 0, cmd)
#define INSTALL_NOSORT_ELEMENT(type, priv, nosortidx, cmd) install_element(type, priv, nosortidx, cmd)

#define INSTALL_INTFPORT_ELEMENT(priv, cmd)    \
do {                                           \
    INSTALL_ELEMENT(IF_NODE, priv, cmd);     \
    INSTALL_ELEMENT(IF_TRK_NODE, priv, cmd); \
} while(0)

#define INSTALL_INTFNMLPORT_ELEMENT(priv, cmd) INSTALL_ELEMENT(IF_NODE, priv, cmd)
#ifdef CONFIG_SYS_L3_ROUTE
#define INSTALL_INTFVLAN_ELEMENT(priv, cmd) INSTALL_ELEMENT(IF_VLAN_NODE, priv, cmd)
#endif
#define INSTALL_PRIVMODE_ELEMENT(priv, cmd)      \
do {                                         \
    INSTALL_ELEMENT(ENABLE_NODE, priv, cmd); \
} while(0)

#define INSTALL_SHOW_ELEMENT(priv, cmd) INSTALL_PRIVMODE_ELEMENT(priv,cmd)


#define VALID_NUM_STR "0123456789"


/* Host configuration variable */
struct host
{
  /* Host name of this router. */
  char *name;

  /* Password for vty interface. */
  char *password;
  char *password_encrypt;

  /* Enable password */
  char *enable;
  char *enable_encrypt;

  /* System wide terminal lines. */
  int lines;

  /* Log filename. */
  char *logfile;

  /* config file name of this host */
  char *config;

  /* Flags for services */
  int advanced;
  int encrypt;

  /* Banner configuration. */
  const char *motd;
  char *motdfile;
};

typedef struct 
{
    int nDataLength;
    char *pszData; 
    char cDisjunctive;  /*分隔符: 打印时，只在分隔符处换行，避免换行时任意分割数据*/
    char nMaxLen;       /*指定最大长度，为0表示不指定长度*/
    char nSpace;        /*列之间空格数*/
} PRINT_DATA_ELEMENT;

/* There are some command levels which called from command node. */
enum node_type
{
    VIEW_NODE,            /* View node. Default mode of vty interface. */
    ENABLE_NODE,            /* Enable node. */
    CONFIG_NODE,            /* Config node. Default mode of config file. */
    IF_NODE,        /* Interface mode node. */
    IF_TRK_NODE,
#ifdef CONFIG_SYS_L3_ROUTE
    IF_VLAN_NODE,
    IF_LO_NODE,
#endif
    VTY_NODE,            /* Vty node. */
    VTY_CONSOLE_NODE,
    VLAN_DB_NODE,      /* VLAN Database mode */
#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    IGMP_PROFILE_NODE,
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    MLD_PROFILE_NODE,
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    ACL_MAC_NODE,
    ACL_IPV4_NODE,
    ACL_IPV6_NODE,
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    MGMT_ACL_NODE,
#endif
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    MST_CFG_NODE,      /* Spanning Tree MST configuration mode */
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
    IP_POOL_NODE,
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	RIP_NODE,
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	OSPF_NODE,
	AREA_NODE,
#endif
#ifdef CONFIG_SYS_L3_ROUTE_BGP
	BGP_NODE,
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	CWMP_NODE,
#endif

    DEBUG_NODE,            /* Debug node. */
    ERPS_INST_NODE,     /* ERPS instance node. */
    /* End of Node (Must be the last node) */
    END_NODE
};

/* Node which has some commands and prompt string and configuration
   function pointer . */
struct cmd_node
{
  /* Node index. */
  enum node_type node;

  /* Prompt character at vty interface. */
  const char *prompt;

  /* Is this node's configuration goes to vtysh ? */
  int vtysh;

  /* Node's configuration write function */
  int (*func) (struct vty *);

  /* Vector of this node's command list. */
  vector cmd_vector;
};

enum
{
  CMD_ATTR_DEPRECATED = 1,
  CMD_ATTR_HIDDEN,
  CMD_ATTR_CONDITION_HIDDEN,
};

/* Structure of command element. */
struct cmd_element
{
  const char *string;            /* Command specification by string. */
  int (*func) (struct cmd_element *, struct vty *, int, char *[]);
  const char *doc;            /* Documentation of this command. */
  int daemon;                   /* Daemon to which this command belong. */
  vector strvec;        /* Pointing out each description vector. */
  unsigned int cmdsize;        /* Command index count. */
  char *config;            /* Configuration string */
  vector subconfig;        /* Sub configuration string */
  u_char attr;            /* Command attributes */
  u_char nosortidx;       /* No sort index */
  unsigned char cmdpriv;  /* Command default privilege */
#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
  unsigned char cmdcurrpriv;  /* Command current privilege */
#endif /* CONFIG_SYS_AAA_LOCALCMD_PRIVCHG */
  vector vline;           /* Current input command vector */
  int (*hidmatch) (struct cmd_element *, struct vty *); /* Hidden match function */
};

typedef int32 (*cmd_hidmatch_func)(struct cmd_element *cmd, struct vty *vty);


/* Command description structure. */
struct desc
{
  char *cmd;                    /* Command string. */
  char *str;                    /* Command's description. */
};

/* Return value of the commands. */
#define CMD_SUCCESS              SYS_ERR_OK
#define CMD_WARNING              SYS_ERR_CMD_WARNING
#define CMD_ERR_NO_MATCH         SYS_ERR_CMD_NOT_MATCH
#define CMD_ERR_BAD_PARAM        SYS_ERR_CMD_BAD_PARAM
#define CMD_ERR_AMBIGUOUS        SYS_ERR_CMD_AMBIGUOUS
#define CMD_ERR_INCOMPLETE       SYS_ERR_CMD_INCOMPLETE
#define CMD_ERR_EXEED_ARGC_MAX   SYS_ERR_CMD_EXEED_ARGC_MAX
#define CMD_ERR_NOTHING_TODO     SYS_ERR_CMD_NOTHING_TODO
#define CMD_ERR_SLAVE_PORT       SYS_ERR_CMD_SLAVE_PORT
#define CMD_ERR_TRUNK_MBR        SYS_ERR_CMD_TRUNK_MBR
#define CMD_ERR_PORTID           SYS_ERR_PORT_ID
#define CMD_ERR_PORTLIST         SYS_ERR_CMD_PORTLIST
#define CMD_ERR_VALUE            SYS_ERR_CMD_VALUE
#define CMD_COMPLETE_FULL_MATCH  SYS_ERR_CMD_COMPLETE_FULL_MATCH
#define CMD_COMPLETE_MATCH       SYS_ERR_CMD_COMPLETE_MATCH
#define CMD_COMPLETE_LIST_MATCH  SYS_ERR_CMD_COMPLETE_LIST_MATCH
#define CMD_SUCCESS_DAEMON       SYS_ERR_CMD_SUCCESS_DAEMON

/* Argc max counts. */
#define CMD_ARGC_MAX   25

/* Turn off these macros when uisng cpp with extract.pl */
#ifndef VTYSH_EXTRACT_PL

/* cli and sal share the same set command function */
#define FUNC_CMD_SET(funcname) funcname(self, vty, argc, argv)

/* helper defines for end-user DEFUN* macros */
#define DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, attrs, dnum) \
  struct cmd_element cmdname = \
  { \
    .string = cmdstr, \
    .func = funcname, \
    .doc = helpstr, \
    .attr = attrs, \
    .daemon = dnum, \
  };

#define DEFUN_CMD_FUNC_DECL(funcname) \
  static int funcname (struct cmd_element *, struct vty *, int, char *[]);

#define DEFUN_CMD_FUNC_TEXT(funcname) \
  static int funcname \
    (struct cmd_element *self __attribute__ ((unused)), \
     struct vty *vty __attribute__ ((unused)), \
     int argc __attribute__ ((unused)), \
     char *argv[] __attribute__ ((unused)) )

/* DEFUN for vty command interafce. Little bit hacky ;-). */
#define DEFUN(funcname, cmdname, cmdstr, helpstr) \
  DEFUN_CMD_FUNC_DECL(funcname) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, 0, 0) \
  DEFUN_CMD_FUNC_TEXT(funcname)

#define DEFUN_ATTR(funcname, cmdname, cmdstr, helpstr, attr) \
  DEFUN_CMD_FUNC_DECL(funcname) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, attr, 0) \
  DEFUN_CMD_FUNC_TEXT(funcname)

#define DEFUN_HIDDEN(funcname, cmdname, cmdstr, helpstr) \
  DEFUN_ATTR (funcname, cmdname, cmdstr, helpstr, CMD_ATTR_HIDDEN)

#define DEFUN_DEPRECATED(funcname, cmdname, cmdstr, helpstr) \
  DEFUN_ATTR (funcname, cmdname, cmdstr, helpstr, CMD_ATTR_DEPRECATED) \

/* DEFUN_NOSH for commands that vtysh should ignore */
#define DEFUN_NOSH(funcname, cmdname, cmdstr, helpstr) \
  DEFUN(funcname, cmdname, cmdstr, helpstr)

/* DEFSH for vtysh. */
#define DEFSH(daemon, cmdname, cmdstr, helpstr) \
  DEFUN_CMD_ELEMENT(NULL, cmdname, cmdstr, helpstr, 0, daemon) \

/* DEFUN + DEFSH */
#define DEFUNSH(daemon, funcname, cmdname, cmdstr, helpstr) \
  DEFUN_CMD_FUNC_DECL(funcname) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, 0, daemon) \
  DEFUN_CMD_FUNC_TEXT(funcname)

/* DEFUN + DEFSH with attributes */
#define DEFUNSH_ATTR(daemon, funcname, cmdname, cmdstr, helpstr, attr) \
  DEFUN_CMD_FUNC_DECL(funcname) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, attr, daemon) \
  DEFUN_CMD_FUNC_TEXT(funcname)

#define DEFUNSH_HIDDEN(daemon, funcname, cmdname, cmdstr, helpstr) \
  DEFUNSH_ATTR (daemon, funcname, cmdname, cmdstr, helpstr, CMD_ATTR_HIDDEN)

#define DEFUNSH_DEPRECATED(daemon, funcname, cmdname, cmdstr, helpstr) \
  DEFUNSH_ATTR (daemon, funcname, cmdname, cmdstr, helpstr, CMD_ATTR_DEPRECATED)

/* ALIAS macro which define existing command's alias. */
#define ALIAS(funcname, cmdname, cmdstr, helpstr) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, 0, 0)

#define ALIAS_ATTR(funcname, cmdname, cmdstr, helpstr, attr) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, attr, 0)

#define ALIAS_HIDDEN(funcname, cmdname, cmdstr, helpstr) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, CMD_ATTR_HIDDEN, 0)

#define ALIAS_DEPRECATED(funcname, cmdname, cmdstr, helpstr) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, CMD_ATTR_DEPRECATED, 0)

#define ALIAS_SH(daemon, funcname, cmdname, cmdstr, helpstr) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, 0, daemon)

#define ALIAS_SH_HIDDEN(daemon, funcname, cmdname, cmdstr, helpstr) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, CMD_ATTR_HIDDEN, daemon)

#define ALIAS_SH_DEPRECATED(daemon, funcname, cmdname, cmdstr, helpstr) \
  DEFUN_CMD_ELEMENT(funcname, cmdname, cmdstr, helpstr, CMD_ATTR_DEPRECATED, daemon)

#endif /* VTYSH_EXTRACT_PL */

/* Some macroes */
#define CMD_OPTION(S)   ((S[0]) == '[')
#define CMD_VARIABLE(S) (((S[0]) >= 'A' && (S[0]) <= 'Z') || ((S[0]) == '<'))
#define CMD_VARARG(S)   ((S[0]) == '.')
#define CMD_RANGE(S)    ((S[0] == '<'))
#define CMD_ARGV(S)     ((((S[0]) >= 'A') && ((S[0]) <= 'Z')) || ((S[0]) == '<') || ((S[0]) == '(') || ((S[0]) == '['))

#define CMD_IPV4(S)       ((strcmp ((S), "A.B.C.D") == 0))
#define CMD_IPV4_PREFIX(S) ((strcmp ((S), "A.B.C.D/M") == 0))
#define CMD_IPV4_WMASK(S) ((strcmp ((S), "A.B.C.D/A.B.C.D") == 0))
#define CMD_IPV6(S)        ((strcmp ((S), "X:X::X:X") == 0))
#define CMD_IPV6_PREFIX(S) ((strcmp ((S), "X:X::X:X/M") == 0))
#define CMD_IPV6_WMASK(S) ((strcmp ((S), "X:X::X:X/<0-128>") == 0))
#define CMD_MAC_ADDR(S)        ( (strcmp((S), "A:B:C:D:E:F") == 0) )
#define CMD_MAC_ADDR_WMASK(S)  ( (strcmp((S), "A:B:C:D:E:F/A:B:C:D:E:F") == 0) )
#define CMD_OUI(S)             ( (strcmp((S), "A:B:C") == 0) )
#define CMD_PORT(S)        ( (strcmp((S), "PORT") == 0) )
#define CMD_PORT_LIST(S)        ( (strcmp((S), "PORT-LIST") == 0) )
#define CMD_NORMAL_WOTRKMBR_PORT_LIST(S)        ( (strcmp((S), "NML-WOTRKMBR-PORT-LIST") == 0) )
#define CMD_NORMAL_PORT_LIST(S)        ( (strcmp((S), "NML-PORT-LIST") == 0) )
#define CMD_VLAN_LIST(S)        ( (strcmp((S), "VLAN-LIST") == 0) )
#define CMD_QUEUE_LIST(S)        ( (strcmp((S), "QUEUE-LIST") == 0) )
#define CMD_DSCP_LIST(S)        ( (strcmp((S), "DSCP-LIST") == 0) )
#define CMD_VALUE(S)       ((strcmp((S), "VALUE") == 0))
#define CMD_SEQUENCE(S)    ((strncmp((S), "SEQUENCE", 8) == 0))
#define CMD_RANGE_FORMAT(S)    ((strncmp((S), "RANGE", 5) == 0))
#define CMD_WORD(S)            ((strncmp((S), "WORD", 4) == 0))
#define CMD_HOSTNAME(S)        ((strcmp ((S), "HOSTNAME") == 0))

#define IF_FA_STR      DFLT_PORT_PREFIX_FA
#define IF_GI_STR      DFLT_PORT_PREFIX_GE
#define IF_MGI_STR     DFLT_PORT_PREFIX_MGE
#define IF_TE_STR      DFLT_PORT_PREFIX_TE
#define IF_PO_STR      DFLT_PORT_PREFIX_LAG
#ifdef CONFIG_SYS_L3_ROUTE
#define IF_VLAN_STR    DFLT_PORT_PREFIX_VLAN
#define IF_LO_STR      "Loopback"
#endif
#define IF_FA_LOWSTR   intf_low_str[PORT_TYPE_100M]
#define IF_GI_LOWSTR   intf_low_str[PORT_TYPE_1000M]
#define IF_MGI_LOWSTR   intf_low_str[PORT_TYPE_2500M]
#define IF_TE_LOWSTR   intf_low_str[PORT_TYPE_10G]
#define IF_PO_LOWSTR   intf_low_str[PORT_TYPE_LAG]
#ifdef CONFIG_SYS_L3_ROUTE
#define IF_VLAN_LOWSTR DFLT_PORT_PREFIX_VLAN
#define IF_LO_LOWSTR   "loopback"
#endif
#define CMD_IF_ALLTYPES_STR  "IF_ALLS"
#define CMD_IF_ALLTYPE_STR   "IF_ALL"
#define CMD_IF_PORTS_STR     "IF_PORTS"
#define CMD_IF_PORT_STR      "IF_PORT"
#define CMD_IF_NMLPORTS_STR  "IF_NMLPORTS"
#define CMD_IF_NMLPORT_STR   "IF_NMLPORT"
#ifdef CONFIG_SYS_L3_ROUTE
#define CMD_IF_L3_ALLS_STR   "IF_L3ALLS"
#define CMD_IF_L3_ALL_STR    "IF_L3ALL"
#define CMD_IF_VLANS_STR     "IF_VLANS"
#define CMD_IF_VLAN_STR      "IF_VLAN"
#define CMD_IF_LOS_STR       "IF_LOS"
#define CMD_IF_LO_STR        "IF_LO"
#endif
#define CMD_IF_FA_PORTS_STR  intfs_key_str[PORT_TYPE_100M]
#define CMD_IF_FA_PORT_STR   intf_key_str[PORT_TYPE_100M]
#define CMD_IF_GI_PORTS_STR  intfs_key_str[PORT_TYPE_1000M]
#define CMD_IF_GI_PORT_STR   intf_key_str[PORT_TYPE_1000M]
#define CMD_IF_MGI_PORTS_STR  intfs_key_str[PORT_TYPE_2500M]
#define CMD_IF_MGI_PORT_STR   intf_key_str[PORT_TYPE_2500M]
#define CMD_IF_TE_PORTS_STR  intfs_key_str[PORT_TYPE_10G]
#define CMD_IF_TE_PORT_STR   intf_key_str[PORT_TYPE_10G]
#define CMD_IF_PO_PORTS_STR  intfs_key_str[PORT_TYPE_LAG]
#define CMD_IF_PO_PORT_STR   intf_key_str[PORT_TYPE_LAG]
#ifdef CONFIG_SYS_L3_ROUTE
#define CMD_IF_KEY_VLANS_STR "ISVLAN"
#define CMD_IF_KEY_VLAN_STR  "IFVLAN"
#define CMD_IF_KEY_LOS_STR   "ISLOOPBK"
#define CMD_IF_KEY_LO_STR    "IFLOOPBK"
#endif
#define CMD_IF_FA_PORTS(S) ((strcmp((S), CMD_IF_FA_PORTS_STR) == 0))
#define CMD_IF_GI_PORTS(S) ((strcmp((S), CMD_IF_GI_PORTS_STR) == 0))
#define CMD_IF_MGI_PORTS(S) ((strcmp((S), CMD_IF_MGI_PORTS_STR) == 0))
#define CMD_IF_TE_PORTS(S) ((strcmp((S), CMD_IF_TE_PORTS_STR) == 0))
#define CMD_IF_PO_PORTS(S) ((strcmp((S), CMD_IF_PO_PORTS_STR) == 0))
#ifdef CONFIG_SYS_L3_ROUTE
#define CMD_IF_VLANS(S)    ((strcmp((S), CMD_IF_KEY_VLANS_STR) == 0))
#define CMD_IF_LOS(S)      ((strcmp((S), CMD_IF_KEY_LOS_STR) == 0))
#endif
#define CMD_IF_FA_PORT(S) ((strcmp((S), CMD_IF_FA_PORT_STR) == 0))
#define CMD_IF_GI_PORT(S) ((strcmp((S), CMD_IF_GI_PORT_STR) == 0))
#define CMD_IF_MGI_PORT(S) ((strcmp((S), CMD_IF_MGI_PORT_STR) == 0))
#define CMD_IF_TE_PORT(S) ((strcmp((S), CMD_IF_TE_PORT_STR) == 0))
#define CMD_IF_PO_PORT(S) ((strcmp((S), CMD_IF_PO_PORT_STR) == 0))
#ifdef CONFIG_SYS_L3_ROUTE
#define CMD_IF_VLAN(S)    ((strcmp((S), CMD_IF_KEY_VLAN_STR) == 0))
#define CMD_IF_LO(S)      ((strcmp((S), CMD_IF_KEY_LO_STR) == 0))
#endif
#define CMD_IF_PORTS(S) ( CMD_IF_FA_PORTS(S) || CMD_IF_GI_PORTS(S) || CMD_IF_MGI_PORTS(S) || CMD_IF_TE_PORTS(S) || CMD_IF_PO_PORTS(S))
#define CMD_IF_PORT(S) (CMD_IF_FA_PORT(S) || CMD_IF_GI_PORT(S) || CMD_IF_MGI_PORT(S) ||CMD_IF_TE_PORT(S) || CMD_IF_PO_PORT(S))
#define CMD_IF_NMLPORTS(S) (CMD_IF_FA_PORTS(S) || CMD_IF_GI_PORTS(S) || CMD_IF_MGI_PORTS(S) || CMD_IF_TE_PORTS(S))
#define CMD_IF_NMLPORT(S) (CMD_IF_FA_PORT(S) || CMD_IF_GI_PORT(S) || CMD_IF_MGI_PORT(S) || CMD_IF_TE_PORT(S))
#ifdef CONFIG_SYS_L3_ROUTE
#define CMD_IF(S)        (CMD_IF_PORTS(S) || CMD_IF_PORT(S) || CMD_IF_NMLPORTS(S) || CMD_IF_NMLPORT(S) || CMD_IF_VLANS(S) || CMD_IF_VLAN(S) || CMD_IF_LOS(S) || CMD_IF_LO(S))
#define CMD_IF_SINGLE(S) (CMD_IF_PORT(S) || CMD_IF_NMLPORT(S) || CMD_IF_VLAN(S) || CMD_IF_LO(S))
#define CMD_IF_RANGE(S)  (CMD_IF_PORTS(S) || CMD_IF_NMLPORTS(S) || CMD_IF_VLANS(S) || CMD_IF_LOS(S))
#else
#define CMD_IF(S)        (CMD_IF_PORTS(S) || CMD_IF_PORT(S) || CMD_IF_NMLPORTS(S) || CMD_IF_NMLPORT(S))
#define CMD_IF_SINGLE(S) (CMD_IF_PORT(S) || CMD_IF_NMLPORT(S))
#define CMD_IF_RANGE(S)  (CMD_IF_PORTS(S) || CMD_IF_NMLPORTS(S))
#endif

#define CMD_FS_FLASH(S) ((strncmp((S), "flash://", 8) == 0))
#define CMD_FS_TFTP(S) ((strncmp((S), "tftp://", 7) == 0))
#define CMD_FS_USB(S) ((strncmp((S), "usb://", 6) == 0))
#define CMD_FS(S)   (CMD_FS_FLASH(S) || CMD_FS_TFTP(S) || CMD_FS_USB(S))


/* Common descriptions. */
#define PORTLIST_STR "Port list(e.g. 1,3,5-9)\n"
#define MACADDR_STR "MAC Address(e.g. xx:xx:xx:xx:xx:xx)\n"
#define IPADDR_STR "IP Address(e.g. x.x.x.x)\n"


#define SHOW_STR "Show running system information\n"
#define IP_STR "IP information\n"
#define IPV6_STR "IPv6 information\n"
#define NO_STR "Negate a command or set its defaults\n"
#define REDIST_STR "Redistribute information from another routing protocol\n"
#define CLEAR_STR "Reset functions\n"
#define RIP_STR "RIP information\n"
#define BGP_STR "BGP information\n"
#define OSPF_STR "OSPF information\n"
#define NEIGHBOR_STR "Specify neighbor router\n"
#define DEBUG_STR "Debugging functions (see also 'undebug')\n"
#define UNDEBUG_STR "Disable debugging functions (see also 'debug')\n"
#define ROUTER_STR "Enable a routing process\n"
#define AS_STR "AS number\n"
#define MBGP_STR "MBGP information\n"
#define MATCH_STR "Match values from routing table\n"
#define SET_STR "Set values in destination routing protocol\n"
#define OUT_STR "Filter outgoing routing updates\n"
#define IN_STR  "Filter incoming routing updates\n"
#define V4NOTATION_STR "specify by IPv4 address notation(e.g. 0.0.0.0)\n"
#define OSPF6_NUMBER_STR "Specify by number\n"
#define INTERFACE_STR "Interface infomation\n"
#define IFNAME_STR "Interface name(e.g. ep0)\n"
#define IP6_STR "IPv6 Information\n"
#define OSPF6_STR "Open Shortest Path First (OSPF) for IPv6\n"
#define OSPF6_ROUTER_STR "Enable a routing process\n"
#define OSPF6_INSTANCE_STR "<1-65535> Instance ID\n"
#define SECONDS_STR "<1-65535> Seconds\n"
#define ROUTE_STR "Routing Table\n"
#define PREFIX_LIST_STR "Build a prefix list\n"
#define OSPF6_DUMP_TYPE_LIST \
"(neighbor|interface|area|lsa|zebra|config|dbex|spf|route|lsdb|redistribute|hook|asbr|prefix|abr)"
#define ISIS_STR "IS-IS information\n"
#define AREA_TAG_STR "[area tag]\n"

#define CONF_BACKUP_EXT ".sav"

/* IPv4 only machine should not accept IPv6 address for peer's IP
   address.  So we replace VTY command string like below. */
#ifdef HAVE_IPV6
#define NEIGHBOR_CMD       "neighbor (A.B.C.D|X:X::X:X) "
#define NO_NEIGHBOR_CMD    "no neighbor (A.B.C.D|X:X::X:X) "
#define NEIGHBOR_ADDR_STR  "Neighbor address\nIPv6 address\n"
#define NEIGHBOR_CMD2      "neighbor (A.B.C.D|X:X::X:X|WORD) "
#define NO_NEIGHBOR_CMD2   "no neighbor (A.B.C.D|X:X::X:X|WORD) "
#define NEIGHBOR_ADDR_STR2 "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
#else
#define NEIGHBOR_CMD       "neighbor A.B.C.D "
#define NO_NEIGHBOR_CMD    "no neighbor A.B.C.D "
#define NEIGHBOR_ADDR_STR  "Neighbor address\n"
#define NEIGHBOR_CMD2      "neighbor (A.B.C.D|WORD) "
#define NO_NEIGHBOR_CMD2   "no neighbor (A.B.C.D|WORD) "
#define NEIGHBOR_ADDR_STR2 "Neighbor address\nNeighbor tag\n"
#endif /* HAVE_IPV6 */

/* Does the I/O error indicate that the operation should be retried later? */
#define ERRNO_IO_RETRY(EN) \
    (((EN) == EAGAIN) || ((EN) == EWOULDBLOCK) || ((EN) == EINTR))

/* Prototypes. */
extern void install_node (struct cmd_node *, int (*) (struct vty *));
extern void install_default (enum node_type);
extern void install_element (enum node_type, char, char, struct cmd_element *);
extern void sal_install_element (enum node_type, struct cmd_element *);
extern void sort_node (void);

/* Concatenates argv[shift] through argv[argc-1] into a single NUL-terminated
   string with a space between each element (allocated using
   XMALLOC(MTYPE_TMP)).  Returns NULL if shift >= argc. */
extern char *argv_concat (const char **argv, int argc, int shift);
extern const char *safe_strerror(int errnum);

extern vector cmd_make_strvec (const char *, enum node_type);
extern void cmd_free_strvec (vector);
extern int cmd_try_do_shortcut (enum node_type, const char*);
extern vector cmd_describe_command (vector, struct vty *, int *status);
extern char **cmd_complete_command (vector, struct vty *, int *status);
extern const char *cmd_prompt (enum node_type);
extern int config_from_file (struct vty *, FILE *);
extern int config_init_from_file(struct vty *, FILE *);
extern enum node_type node_parent (enum node_type);
extern int cmd_execute_command (vector, struct vty *, struct cmd_element **, int, const char *);
extern int cmd_execute_command_strict (vector, struct vty *, struct cmd_element **);
extern void config_replace_string (struct cmd_element *, char *, ...);
extern void cmd_init (int);
extern void cmd_sal_init (void);
extern void cmd_node_dump(void);
extern void cmd_terminate (void);
extern int32 config_valid_file_convert(char *);
extern uint32 config_valid_file_check(char *);

/* Export typical functions. */
extern struct cmd_element config_end_cmd;
extern struct cmd_element config_exit_cmd;
extern struct cmd_element config_quit_cmd;
extern struct cmd_element config_help_cmd;
extern struct cmd_element config_list_cmd;
extern char *host_config_file (void);
extern void host_config_set (char *);

extern void print_version (const char *);

/* struct host global, ick */
extern struct host host;

extern char intf_key_str[PORT_TYPE_LAG + 1][CAPA_PORT_STR_LEN];
extern char intfs_key_str[PORT_TYPE_LAG + 1][CAPA_PORT_STR_LEN];
extern unsigned char currpriv;
extern unsigned char currpriv;
extern unsigned char currUserPriv;
extern char currUserName[CAPA_SYS_USERNAME_LEN + 1];
extern char currPassWord[CAPA_SYS_PASSWORD_LEN + 1];
extern char currHost[CAPA_HOSTNAME_LEN + 1];
#ifdef CONFIG_SYS_AAA
extern int currAuthType;
#endif
extern int access_flag;
extern int intr_forbid_flag;
extern int parent_pid;
extern int dump_cmd_syntax;


#define CLI_INTR_FORBID()    \
do {                         \
    intr_forbid_flag = TRUE; \
} while(0)

#define CLI_INTR_ALLOW()      \
do {                          \
    intr_forbid_flag = FALSE; \
} while(0)


/* "<cr>" global */
extern char *command_cr;

extern vector cmdvec;
vector cmd_node_vector (vector v, enum node_type ntype);

typedef int (*cmd_textline_func_t)(const char *cmdline);
extern  int transLongStrToStrArray(char *strBuf, char rtBuf[][128], int strLenth);
extern int cmd_textline_enable_get(sys_enable_t *pEnable);
extern int cmd_textline_func_set(cmd_textline_func_t func);
extern int cmd_textline_enable_set(sys_enable_t enable);
extern int add_value_to_new_line_printor_bylen(int format, void *value, vector *pVector, char nlen);
extern int add_value_to_last_line_printor_bylen(int format, void * value, vector * pVector, char nlen);
extern int cmdPrintOutResult(vector cmd2print, char *pLastInputChar);
extern int free_printor_vector(vector *pVector);
extern int add_value_to_new_line_printor(int format, void *value, vector *pVector);
extern int add_value_to_last_line_printor(int format, void * value, vector * pVector);
#endif /* _ZEBRA_COMMAND_H */
