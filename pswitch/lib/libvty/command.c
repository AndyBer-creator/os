/*
   $Id$

   Command interpreter routine for virtual terminal [aka TeletYpe]
   Copyright (C) 1997, 98, 99 Kunihiro Ishiguro

This file is part of GNU Zebra.

GNU Zebra is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation; either version 2, or (at your
option) any later version.

GNU Zebra is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Zebra; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include <zebra.h>
#include <readline/history.h>
//#include <assert.h>


#include "memory.h"
////#include "log.h"
#include <version.h>
////#include "thread.h"
#include "vector.h"
#include "vty.h"
#include "command.h"
////#include "workqueue.h"


#include <common/sys_def.h>
#include <libfds/fds_export.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_util.h>

//#define DUMP_ALL_CMD_SYNTAX 1
#ifdef DUMP_ALL_CMD_SYNTAX
uint32 g_id = 0;
#endif

#define HOSTNAME "Switch"

/* Command vector which includes some level of command lists. Normally
   each daemon maintains each own cmdvec. */
vector cmdvec = NULL;
unsigned int glHeaderLineCount = 0;

/* RTK: AAA global variables */
unsigned char currpriv = CMD_PRIV_15;
unsigned char currUserPriv = CMD_PRIV_15;
char currUserName[CAPA_SYS_USERNAME_LEN + 1];
char currPassWord[CAPA_SYS_PASSWORD_LEN + 1];
char currHost[CAPA_HOSTNAME_LEN + 1];
#ifdef CONFIG_SYS_AAA
int currAuthType = SYS_AUTH_TYPE_EMPTY;
#endif
int access_flag = SYS_ACCESS_CLI_CONSOLE;
int intr_forbid_flag = FALSE;
int parent_pid = 0;
int dump_cmd_syntax = 0;

#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
unsigned int glbConfigCmdIndex = 0;
unsigned int glbExecCmdIndex = 0;
#endif /* CONFIG_SYS_AAA_LOCALCMD_PRIVCHG */

char intf_key_str[PORT_TYPE_LAG + 1][CAPA_PORT_STR_LEN];
char intfs_key_str[PORT_TYPE_LAG + 1][CAPA_PORT_STR_LEN];
char intf_low_str[PORT_TYPE_LAG + 1][CAPA_PORT_STR_LEN];

struct desc desc_cr;
char *command_cr = NULL;

struct desc intf_fa;
struct desc intf_gi;
struct desc intf_mgi;
struct desc intf_te;
struct desc intf_po;
#ifdef CONFIG_SYS_L3_ROUTE
struct desc intf_vlan;
struct desc intf_lo;
#endif
struct desc intf_fas;
struct desc intf_gis;
struct desc intf_mgis;
struct desc intf_tes;
struct desc intf_pos;
#ifdef CONFIG_SYS_L3_ROUTE
struct desc intf_vlans;
struct desc intf_los;
#endif
struct desc intf_fa_devnum;
struct desc intf_gi_devnum;
struct desc intf_mgi_devnum;
struct desc intf_te_devnum;
struct desc intf_po_devnum;
#ifdef CONFIG_SYS_L3_ROUTE
struct desc intf_vlan_devnum;
struct desc intf_lo_devnum;
#endif

/* Host information structure. */
struct host host;

/* Standard command node structures. */

static struct cmd_node view_node =
{
  VIEW_NODE,
  DFLT_CLI_PROMPT_VIEW,
};

static struct cmd_node enable_node =
{
  ENABLE_NODE,
  DFLT_CLI_PROMPT_ENABLE,
};

static struct cmd_node config_node =
{
  CONFIG_NODE,
  DFLT_CLI_PROMPT_CONFIG,
  1
};

static struct cmd_node debug_node =
{
  DEBUG_NODE,
  DFLT_CLI_PROMPT_DEBUG,
  1
};

static struct cmd_node intf_node =
{
  IF_NODE,
  DFLT_CLI_PROMPT_IF,
  1,
};

static struct cmd_node intf_trk_node =
{
  IF_TRK_NODE,
  DFLT_CLI_PROMPT_IF,
  1,
};

#ifdef CONFIG_SYS_L3_ROUTE
static struct cmd_node intf_vlan_node =
{
  IF_VLAN_NODE,
  DFLT_CLI_PROMPT_IF,
  1,
};

static struct cmd_node intf_lo_node =
{
  IF_LO_NODE,
  DFLT_CLI_PROMPT_IF,
  1,
};

#endif

static struct cmd_node vty_node =
{
  VTY_NODE,
  DFLT_CLI_PROMPT_VTY,
  1,
};

static struct cmd_node vty_console_node =
{
  VTY_CONSOLE_NODE,
  DFLT_CLI_PROMPT_VTY,
  1,
};

static struct cmd_node vlan_db_node =
{
  VLAN_DB_NODE,
  DFLT_CLI_PROMPT_VLAN,
  1
};

#ifdef CONFIG_SYS_USER_DEFINED_ACL
static struct cmd_node acl_mac_node =
{
  ACL_MAC_NODE,
  DFLT_CLI_PROMPT_MAC_ACL,
  1
};

static struct cmd_node acl_ipv4_node =
{
  ACL_IPV4_NODE,
  DFLT_CLI_PROMPT_IPV4_ACL,
  1
};

static struct cmd_node acl_ipv6_node =
{
  ACL_IPV6_NODE,
  DFLT_CLI_PROMPT_IPV6_ACL,
  1
};
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
static struct cmd_node igmp_profile_node =
{
  IGMP_PROFILE_NODE,
  DFLT_CLI_PROMPT_IGMP_PROFILE,
  1
};
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
static struct cmd_node mld_profile_node =
{
  MLD_PROFILE_NODE,
  DFLT_CLI_PROMPT_MLD_PROFILE,
  1
};
#endif
#ifdef CONFIG_SYS_MGMT_ACL
static struct cmd_node mgmt_acl_node =
{
  MGMT_ACL_NODE,
  DFLT_CLI_PROMPT_MGMT_ACL,
  1
};
#endif
#ifdef CONFIG_SYS_PROTO_STP_MSTP
static struct cmd_node mst_cfg_node =
{
  MST_CFG_NODE,
  DFLT_CLI_PROMPT_MST,
  1
};
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
static struct cmd_node erps_inst_node =
{
  ERPS_INST_NODE,
  DFLT_CLI_PROMPT_ERPS_INST,
  1
};
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
static struct cmd_node ip_pool_node =
{
  IP_POOL_NODE,
  DFLT_CLI_PROMPT_DHCP_IPPOOL,
  1
};
#endif

#ifdef CONFIG_SYS_L3_ROUTE_RIP
static struct cmd_node rip_node =
{
  RIP_NODE,
  DFLT_CLI_PROMPT_RIP,
  1
};
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
static struct cmd_node ospf_node =
{
  OSPF_NODE,
  DFLT_CLI_PROMPT_OSPF,
  1
};
static struct cmd_node area_node =
{
  AREA_NODE,
  DFLT_CLI_PROMPT_OSPF_AREA,
  1
};
#endif
#ifdef CONFIG_SYS_L3_ROUTE_BGP
static struct cmd_node bgp_node =
{
  BGP_NODE,
  DFLT_CLI_PROMPT_BGP,
  1
};
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
static struct cmd_node cwmp_node =
{
  CWMP_NODE,
  DFLT_CLI_PROMPT_CWMP,
  1
};
#endif

static sys_enable_t cmd_textline_enable = DISABLED;
static cmd_textline_func_t cmd_textline_func = NULL;

int cmd_textline_enable_get(sys_enable_t *pEnable)
{
    *pEnable = cmd_textline_enable;

    return 0;
}

int cmd_textline_func_set(cmd_textline_func_t func)
{
    cmd_textline_func = func;

    return 0;
}

int cmd_textline_enable_set(sys_enable_t enable)
{
    cmd_textline_enable = enable;

    if (DISABLED == cmd_textline_enable)
        cmd_textline_func = NULL;

    return 0;
}

static int cmd_textline_func_execute(const char *line)
{
    return (cmd_textline_func) ? cmd_textline_func(line) : 0;
}

/* Default motd string. */
#if 0
static const char *default_motd =
"\r\n\
Hello, this is " QUAGGA_PROGNAME " (version " QUAGGA_VERSION ").\r\n\
" QUAGGA_COPYRIGHT "\r\n\
\r\n";
#endif

#if 0
static const char *default_motd =
"\r\n\
Switch Command Line Interface"" (version " DFLT_SYS_VER ")\r\n\
" QUAGGA_COPYRIGHT "\r\n\
\r\n";
#endif
char *default_motd = NULL;
char *default_motdfile = NULL;
void init_default_motd(void)
{
  char tmp[256] = "";
  sprintf(tmp, "\r\nSwitch Command Line Interface (Version %s)\r\n%s\r\n\r\n", DFLT_SYS_VER, QUAGGA_COPYRIGHT);
  default_motd = malloc(strlen(tmp)+1);
  memset(default_motd, 0, strlen(tmp) + 1);
  strcpy(default_motd, tmp);
}
void init_default_motd_file(void)
{
 	char tmp[24] = "";
   if(sal_util_file_exist (CONF_BANNER_USR_FILE) == 0)
   {
		default_motdfile = malloc(strlen(tmp)+1);
		memset(default_motdfile, 0, strlen(tmp) + 1);
		sprintf(default_motdfile, CONF_BANNER_USR_FILE);
   }
   else
   {
		default_motdfile = malloc(strlen(tmp)+1);
		memset(default_motdfile, 0, strlen(tmp) + 1);
		sprintf(default_motdfile, CONF_BANNER_DEF_FILE);
   }
}

/* Wrapper around strerror to handle case where it returns NULL. */
const char *
safe_strerror(int errnum)
{
  const char *s = strerror(errnum);
  return (s != NULL) ? s : "Unknown error";
}

/* This is called from main when a daemon is invoked with -v or --version. */
void
print_version (const char *progname)
{
  printf ("%s version %s\n", progname, QUAGGA_VERSION);
  printf ("%s\n", QUAGGA_COPYRIGHT);
}


/* Utility function to concatenate argv argument into a single string
   with inserting ' ' character between each argument.  */
char *
argv_concat (const char **argv, int argc, int shift)
{
  int i;
  size_t len;
  char *str;
  char *p;

  len = 0;
  for (i = shift; i < argc; i++)
    len += strlen(argv[i])+1;
  if (!len)
    return NULL;
  p = str = XMALLOC(MTYPE_TMP, len);
  for (i = shift; i < argc; i++)
    {
      size_t arglen;
      memcpy(p, argv[i], (arglen = strlen(argv[i])));
      p += arglen;
      *p++ = ' ';
    }
  *(p-1) = '\0';
  return str;
}

/* Install top node of command vector. */
void
install_node (struct cmd_node *node,
          int (*func) (struct vty *))
{
  if (dump_cmd_syntax)
  {
    printf("{%d} %s\n", node->node, node->prompt);
    return;
  }

  vector_set_index (cmdvec, node->node, node);
  node->func = func;
  node->cmd_vector = vector_init (VECTOR_MIN_SIZE);
}

/* Compare two command's string.  Used in sort_node (). */
static int
cmp_node (const void *p, const void *q)
{
  const struct cmd_element *a = *(struct cmd_element * const *)p;
  const struct cmd_element *b = *(struct cmd_element * const *)q;

  return strcmp (a->string, b->string);
}

static int
cmp_desc (const void *p, const void *q)
{
  const struct desc *a = *(struct desc * const *)p;
  const struct desc *b = *(struct desc * const *)q;

  return strcmp (a->cmd, b->cmd);
}

/* Sort each node's command element according to command string. */
void
sort_node ()
{
  unsigned int i, j;
  struct cmd_node *cnode;
  vector descvec;
  struct cmd_element *cmd_element;

  for (i = 0; i < vector_active (cmdvec); i++)
    if ((cnode = vector_slot (cmdvec, i)) != NULL)
      {
    vector cmd_vector = cnode->cmd_vector;
    qsort (cmd_vector->index, vector_active (cmd_vector),
           sizeof (void *), cmp_node);

    for (j = 0; j < vector_active (cmd_vector); j++)
      if ((cmd_element = vector_slot (cmd_vector, j)) != NULL
          && vector_active (cmd_element->strvec))
        {
          descvec = vector_slot (cmd_element->strvec,
                     vector_active (cmd_element->strvec) - 1);
          qsort (descvec->index, vector_active (descvec),
                 sizeof (void *), cmp_desc);
        }
      }
}

/* Breaking up string into each command piece. I assume given
   character is separated by a space character. Return value is a
   vector which includes char ** data element. */
vector
cmd_make_strvec (const char *string, enum node_type ntype)
{
    const char *cp, *start;
    char *token = NULL;
    int str_len = 0;
    vector strvec;
    vector cmd_vector;
    int endQuote;
    int index = 0;

    if (string == NULL)
        return NULL;

    cp = string;

    osal_memset(&strvec, 0, sizeof(strvec));
    osal_memset(&cmd_vector, 0, sizeof(cmd_vector));

    //cmd_vector = vector_copy (cmd_node_vector (cmdvec, vty->node));
    cmd_vector = vector_copy (cmd_node_vector (cmdvec, ntype));

    /* Skip white spaces. */
    while (isspace ((int) *cp) && *cp != '\0')
        cp++;

    /* Return if there is only white spaces */
    if (*cp == '\0')
    {
        vector_free (cmd_vector);
        return NULL;
    }

    if (*cp == '!' || *cp == '#')
    {
        vector_free (cmd_vector);
        return NULL;
    }

    /* Prepare return vector. */
    strvec = vector_init (VECTOR_MIN_SIZE);

    /* Copy each command piece and set into vector. */
    while (1)
    {
        start = cp;
        /* For quoted string support */
        /* Quoted string behavior
           (1) Only first char of token is " will be treated as quoted string token.
               (Allow space char and " will be removed)
           (2) If " is not in first char of token, the " will be treated as normal string
               and it will not be removed.
           (3) If first char of token is ", the last char of token must be " too.
               Otherwise, the token is invalid.
           (4) If the token is quoted string token, " is not allowd in string between quoted key.
           (5) The ' char will be treated as normal string.
         */

        if (*start == '"')
        {
            endQuote = 0;
            cp++;
            while (!(*cp == '\r' || *cp == '\n') && *cp != '\0')
            {
                /* Find end quote */
                if (*cp == '"')
                {
                    /* Next char of end quote should be one of the end token key */
                    if (!(isspace ((int) *(cp+1)) || *(cp+1) == '\r' || *(cp+1) == '\n')
                        && *(cp+1) != '\0')
                    {
                        if (strvec != NULL)
                            cmd_free_strvec(strvec);
                        printf("Invalid input\n");
                        vector_free (cmd_vector);
                        return NULL;
                    }
                    endQuote = 1;
                    break;
                }
                cp++;
            }

            /* Show warning message if end quote is not found */
            if (!endQuote)
            {
                if (strvec != NULL)
                    cmd_free_strvec(strvec);
                printf("Warning: Assumed end-quote for quoted string\n");
                vector_free (cmd_vector);
                return NULL;
            }

            /* Remove "" */
            str_len = cp - start - 1;
            if (0 == str_len)
            {
                /* RTK: If data is empty string, give it temp string with "" */
                token = XMALLOC (MTYPE_STRVEC, 3);
                osal_memset(token, 0, 3);
                osal_strncpy(token, "\"\"", 2);
                str_len = 2;
            }
            else
            {
                token = XMALLOC (MTYPE_STRVEC, str_len + 1);
                osal_memset(token, 0, str_len + 1);
                memcpy (token, start + 1, str_len);
            }
            /* Skip end quote */
            cp++;
        }
        else
        {
            int isSeqFound = 0;
            int isSeqNextTokFound = 0;
            int isCmdNotMatch = 0;
            const char *seqNextTok = NULL;
            const char *tmp = NULL;
            int i;
            int j = 0;
            uint32 tokStrlen = 0;
            uint32 seqTokStrlen = 0;
            struct cmd_element *cmd_element;
            vector descvec;
            vector descvec_idx;
            struct desc *desc;
            struct desc *desc_idx;

            /* Scan all command vector to find sequence command */
            for (i = 0; i < vector_active (cmd_vector); i++)
            {
                if ((cmd_element = vector_slot (cmd_vector, i)) != NULL)
                {
                    if (index >= vector_active(cmd_element->strvec))
                        continue;

                    descvec_idx = vector_slot (cmd_element->strvec, index);
                    desc_idx = vector_slot (descvec_idx, 0);

                    if (!CMD_SEQUENCE(desc_idx->cmd) && !CMD_IF(desc_idx->cmd))
                        continue;

                    isCmdNotMatch = FALSE;

                    for (j = index; j >= 0; j--)
                    {
                        descvec = vector_slot (cmd_element->strvec, j);

                        desc = vector_slot (descvec, 0);

                        if (CMD_ARGV(desc->cmd))
                            continue;

                        if (0 != osal_strncmp(vector_slot(strvec, j), desc->cmd, strlen(vector_slot(strvec, j))))
                        {
                            isCmdNotMatch = TRUE;
                            break;
                        }
                    }

                    if (TRUE == isCmdNotMatch)
                        continue;

                    //SYS_PRINTF("%s:%d desc_idx->cmd=%s\n", __func__, __LINE__, desc_idx->cmd);
#if 0
                    SYS_PRINTF("\n");
                    for (j = 0; j < vector_active(cmd_element->strvec); j++)
                    {
                        descvec = vector_slot (cmd_element->strvec, j);

                        desc = vector_slot (descvec, 0);

                        SYS_PRINTF("%s ", desc->cmd);
                    }
                    SYS_PRINTF("\n");
#endif
                    if (CMD_SEQUENCE(desc_idx->cmd) || CMD_IF(desc_idx->cmd))
                    {
                        if (CMD_IF(desc_idx->cmd))
                        {
                            char intfStr[CAPA_PORTMASK_STR_LEN];

                            tmp = cp;

                            tokStrlen = 0;
                            /* Calculate current token string length */
                            while ((*tmp != '\r') && (*tmp != '\n') && (*tmp != '\0') && (*tmp != ' '))
                            {
                                tokStrlen++;
                                tmp++;
                            }

                            if (tokStrlen > CAPA_PORTMASK_STR_LEN)
                                break;

                            SYS_MEM_CLEAR(intfStr);
                            osal_strncpy(intfStr, cp, tokStrlen);
                            sal_util_str2lowerStr_xlate(intfStr);

                            /* If current command is interface but input is not interface string,
                               do not need to make interface strvec.*/
                            if (0 != osal_strncmp(intfStr, IF_FA_LOWSTR, tokStrlen)
                                && 0 != osal_strncmp(intfStr, IF_GI_LOWSTR, tokStrlen)
                                && 0 != osal_strncmp(intfStr, IF_MGI_LOWSTR, tokStrlen)
                                && 0 != osal_strncmp(intfStr, IF_TE_LOWSTR, tokStrlen)
                                && 0 != osal_strncmp(intfStr, IF_PO_LOWSTR, tokStrlen)
#ifdef CONFIG_SYS_L3_ROUTE
                                && 0 != osal_strncmp(intfStr, IF_VLAN_LOWSTR, tokStrlen)
                                && 0 != osal_strncmp(intfStr, IF_LO_LOWSTR, tokStrlen)
#endif
                               )
                                break;
                        }

                        isSeqFound = TRUE;

                        /* Find if there is any next token after SEQUENCE token */
                        if ((index + 1 < vector_active(cmd_element->strvec))
                            && (NULL != (descvec = vector_slot(cmd_element->strvec, (index + 1)))))
                        {
                            char lpmStr[CAPA_PORTMASK_STR_LEN];
                            int32 ret = -1;
                            uint32 tokNum = 0;
                            sys_logic_portmask_t lpm;

                            isSeqNextTokFound = FALSE;
                            seqTokStrlen = 0;

                            seqNextTok = cp;

                            desc = vector_slot(descvec, 0);

                            while ((*seqNextTok != '\r') && (*seqNextTok != '\n') && (*seqNextTok != '\0'))
                            {
                                /* Find next word without ' ' */
                                if (*seqNextTok == ' ')
                                {
                                    seqNextTok++;
                                    continue;
                                }
                                else
                                {
                                    tokStrlen = 0;
                                    tmp = seqNextTok;

                                    /* Calculate current token string length */
                                    while ((*tmp != '\r') && (*tmp != '\n') && (*tmp != '\0') && (*tmp != ' '))
                                    {
                                        tokStrlen++;
                                        tmp++;
                                    }

                                    //SYS_PRINTF("tokNum=%d seqNextTok=%s desc->cmd=%s\n", tokNum, seqNextTok, desc->cmd);
                                    if (CMD_SEQUENCE(desc_idx->cmd))
                                    {
                                        /* Compare the input torken with current command token */
                                        if (0 == osal_strncmp(seqNextTok, desc->cmd, tokStrlen))
                                        {
                                            isSeqNextTokFound = TRUE;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        if (0 == tokNum)
                                        {
                                            SYS_MEM_CLEAR(lpmStr);
                                            osal_strncpy(lpmStr, seqNextTok, tokStrlen);

                                            ret = STR2LPM(lpmStr, &lpm);

                                            if (SYS_ERR_OK == ret)
                                            {
                                                seqNextTok += tokStrlen;
                                                seqTokStrlen += tokStrlen;

                                                while (*seqNextTok == ' ')
                                                {
                                                    seqNextTok++;
                                                    seqTokStrlen++;
                                                }
                                                //SYS_PRINTF("%s:%d lpmStr=%s seqNextTok=%s\n", __func__, __LINE__, lpmStr, seqNextTok);
                                                isSeqNextTokFound = TRUE;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            seqNextTok += tokStrlen;
                                            seqTokStrlen += tokStrlen;
                                            while (*seqNextTok == ' ')
                                            {
                                                seqNextTok++;
                                                seqTokStrlen++;
                                            }
                                            //SYS_PRINTF("%s:%d seqNextTok=%s\n", __func__, __LINE__, seqNextTok);
                                            isSeqNextTokFound = TRUE;
                                            break;
                                        }
                                    }

                                    seqNextTok += tokStrlen;
                                    tokNum++;
                                }
                            }

                            if (TRUE == isSeqNextTokFound)
                                break;
                        }
                    }
                }
            }

            /* Current token index is a SEQUENCE toekn */
            if (TRUE == isSeqFound)
            {
                /* No next token after SEQUENCE token, just copy all string to the end of string */
                if (FALSE == isSeqNextTokFound)
                {
                    while (!(*cp == '\r' || *cp == '\n' || *cp == '\0'))
                        cp++;

                    str_len = cp - start + 1;
                    token = XMALLOC (MTYPE_STRVEC, str_len + 1);
                    osal_memset(token, 0, str_len+1);
                    memcpy(token, start, str_len);

                    //*(token + str_len - 1) = ' ';
                    //*(token + str_len) = '\0';
                    vector_set (strvec, token);
                    vector_free (cmd_vector);

                    return strvec;
                }
                /* Copy SEQUENCE token string  */
                else
                {
                    //SYS_PRINTF("seqTokStrlen=%d seqNextTok=%s\n", seqTokStrlen, seqNextTok);
                    //SYS_PRINTF("seqNextTok-cp=%d cp=%s\n", seqNextTok-cp, cp);
                    str_len = seqNextTok - start;
                    token = XMALLOC (MTYPE_STRVEC, str_len + 1);
                    osal_memset(token, 0, str_len + 1);
                    memcpy (token, start, str_len);
                    cp += str_len;
                    //SYS_PRINTF("11 str_len=%d token=%s\n", str_len, token);
                    //SYS_PRINTF("11 cp=%s\n", cp);
                }

                isSeqFound = FALSE;
            }
            /* Current token index is a normal token */
            else
            {
                while (!(isspace ((int) *cp) || *cp == '\r' || *cp == '\n') && *cp != '\0')
                    cp++;

                str_len = cp - start;
                token = XMALLOC (MTYPE_STRVEC, str_len + 1);
                osal_memset(token, 0, str_len + 1);
                memcpy (token, start, str_len);
                //SYS_PRINTF("22 str_len=%d token=%s\n", str_len, token);
                //SYS_PRINTF("22 cp=%s\n", cp);
            }
        }

        *(token + str_len) = '\0';
        vector_set (strvec, token);

        while ((isspace ((int) *cp) || *cp == '\n' || *cp == '\r') && *cp != '\0')
            cp++;

        /* Go to next token index */
        index++;

        if (*cp == '\0')
        {
            vector_free (cmd_vector);
            return strvec;
        }
    }
}

/* Free allocated string vector. */
void
cmd_free_strvec (vector v)
{
  unsigned int i;
  char *cp;

  if (!v)
    return;

  for (i = 0; i < vector_active (v); i++)
    if ((cp = vector_slot (v, i)) != NULL)
      XFREE (MTYPE_STRVEC, cp);

  vector_free (v);
}

/* Fetch next description.  Used in cmd_make_descvec(). */
static char *
cmd_desc_str (const char **string)
{
  const char *cp, *start;
  char *token;
  int strlen;

  cp = *string;

  if (cp == NULL)
    return NULL;

  /* Skip white spaces. */
  while (isspace ((int) *cp) && *cp != '\0')
    cp++;

  /* Return if there is only white spaces */
  if (*cp == '\0')
    return NULL;

  start = cp;

  while (!(*cp == '\r' || *cp == '\n') && *cp != '\0')
    cp++;

  strlen = cp - start;
  token = XMALLOC (MTYPE_STRVEC, strlen + 1);
  memcpy (token, start, strlen);
  *(token + strlen) = '\0';

  *string = cp;

  return token;
}

/* New string vector. */
static vector
cmd_make_descvec (const char *string, const char *descstr)
{
  int multiple = 0;
  const char *sp;
  char *token;
  int len;
  const char *cp;
  const char *dp;
  vector allvec;
  vector strvec = NULL;
  struct desc *desc;

  cp = string;
  dp = descstr;

  if (cp == NULL)
    return NULL;

  allvec = vector_init (VECTOR_MIN_SIZE);

  while (1)
    {
      while (isspace ((int) *cp) && *cp != '\0')
    cp++;

      if (*cp == '(')
    {
      multiple = 1;
      cp++;
    }
      if (*cp == ')')
    {
      multiple = 0;
      cp++;
    }
      if (*cp == '|')
    {
      if (! multiple)
        {
          fprintf (stderr, "Command parse error!: %s\n", string);
          exit (1);
        }
      cp++;
    }

      while (isspace ((int) *cp) && *cp != '\0')
    cp++;

      if (*cp == '(')
    {
      multiple = 1;
      cp++;
    }

      if (*cp == '\0')
    return allvec;

      sp = cp;

      while (! (isspace ((int) *cp) || *cp == '\r' || *cp == '\n' || *cp == ')' || *cp == '|') && *cp != '\0')
    cp++;

      len = cp - sp;

      /* RTK: Expend IF_PORTS -> IF_FA_PORTS,IF_GI_PORTS,IF_PO_PORTS..... */
      if (NULL != descstr)
      {
          if (0 == osal_strncmp(sp, CMD_IF_PORTS_STR, osal_strlen(CMD_IF_PORTS_STR))
                  || 0 == osal_strncmp(sp, CMD_IF_NMLPORTS_STR, osal_strlen(CMD_IF_NMLPORTS_STR))
                  || 0 == osal_strncmp(sp, CMD_IF_ALLTYPES_STR, osal_strlen(CMD_IF_ALLTYPES_STR)))
          {
              strvec = vector_init (VECTOR_MIN_SIZE);
              vector_set (allvec, strvec);

              if (LPTYPENUM_100M() > 0)
                  vector_set (strvec, &intf_fas);

              if (LPTYPENUM_1000M() > 0)
                  vector_set (strvec, &intf_gis);
                  
              if (LPTYPENUM_2500M() > 0)
                  vector_set (strvec, &intf_mgis);

              if (LPTYPENUM_10G() > 0)
                  vector_set (strvec, &intf_tes);

              if (0 == osal_strncmp(sp, CMD_IF_PORTS_STR, osal_strlen(CMD_IF_PORTS_STR)))
              {
                  vector_set (strvec, &intf_pos);
              }
              else if (0 == osal_strncmp(sp, CMD_IF_ALLTYPES_STR, osal_strlen(CMD_IF_ALLTYPES_STR)))
              {
                  vector_set (strvec, &intf_pos);
#ifdef CONFIG_SYS_L3_ROUTE
                  vector_set (strvec, &intf_vlans);
                  vector_set (strvec, &intf_los);
#endif
              }

              continue;
          }
          else if (0 == osal_strncmp(sp, CMD_IF_PORT_STR, osal_strlen(CMD_IF_PORT_STR))
                  || 0 == osal_strncmp(sp, CMD_IF_NMLPORT_STR, osal_strlen(CMD_IF_NMLPORT_STR))
                  || 0 == osal_strncmp(sp, CMD_IF_ALLTYPE_STR, osal_strlen(CMD_IF_ALLTYPE_STR)))
          {
              strvec = vector_init (VECTOR_MIN_SIZE);
              vector_set (allvec, strvec);

              if (LPTYPENUM_100M() > 0)
                  vector_set (strvec, &intf_fa);

              if (LPTYPENUM_1000M() > 0)
                  vector_set (strvec, &intf_gi);
                  
              if (LPTYPENUM_2500M() > 0)
                  vector_set (strvec, &intf_mgi);

              if (LPTYPENUM_10G() > 0)
                  vector_set (strvec, &intf_te);

              if (0 == osal_strncmp(sp, CMD_IF_PORT_STR, osal_strlen(CMD_IF_PORT_STR)))
              {
                  vector_set (strvec, &intf_po);
              }
              else if (0 == osal_strncmp(sp, CMD_IF_ALLTYPE_STR, osal_strlen(CMD_IF_ALLTYPE_STR)))
              {
                  vector_set (strvec, &intf_po);
#ifdef CONFIG_SYS_L3_ROUTE
                  vector_set (strvec, &intf_vlan);
                  vector_set (strvec, &intf_lo);
#endif
              }

              continue;
          }
#ifdef CONFIG_SYS_L3_ROUTE
          else if (0 == osal_strncmp(sp, CMD_IF_L3_ALLS_STR, osal_strlen(CMD_IF_L3_ALLS_STR)))
          {
              strvec = vector_init (VECTOR_MIN_SIZE);
              vector_set (allvec, strvec);

              vector_set (strvec, &intf_vlans);
              vector_set (strvec, &intf_los);

              continue;
          }
          else if (0 == osal_strncmp(sp, CMD_IF_L3_ALL_STR, osal_strlen(CMD_IF_L3_ALL_STR)))
          {
              strvec = vector_init (VECTOR_MIN_SIZE);
              vector_set (allvec, strvec);

              vector_set (strvec, &intf_vlan);
              vector_set (strvec, &intf_lo);

              continue;
          }
          else if (0 == osal_strncmp(sp, CMD_IF_VLANS_STR, osal_strlen(CMD_IF_VLANS_STR)))
          {
              strvec = vector_init (VECTOR_MIN_SIZE);
              vector_set (allvec, strvec);
              vector_set (strvec, &intf_vlans);

              continue;
          }
          else if (0 == osal_strncmp(sp, CMD_IF_VLAN_STR, osal_strlen(CMD_IF_VLAN_STR)))
          {
              strvec = vector_init (VECTOR_MIN_SIZE);
              vector_set (allvec, strvec);
              vector_set (strvec, &intf_vlan);

              continue;
          }
          else if (0 == osal_strncmp(sp, CMD_IF_LOS_STR, osal_strlen(CMD_IF_LOS_STR)))
          {
              strvec = vector_init (VECTOR_MIN_SIZE);
              vector_set (allvec, strvec);
              vector_set (strvec, &intf_los);

              continue;
          }
          else if (0 == osal_strncmp(sp, CMD_IF_LO_STR, osal_strlen(CMD_IF_LO_STR)))
          {
              strvec = vector_init (VECTOR_MIN_SIZE);
              vector_set (allvec, strvec);
              vector_set (strvec, &intf_lo);

              continue;
          }
#endif
      }

      token = XMALLOC (MTYPE_STRVEC, len + 1);
      memcpy (token, sp, len);
      *(token + len) = '\0';

      desc = XCALLOC (MTYPE_DESC, sizeof (struct desc));
      desc->cmd = token;
      desc->str = cmd_desc_str (&dp);

      if (multiple)
    {
      if (multiple == 1)
        {
          strvec = vector_init (VECTOR_MIN_SIZE);
          vector_set (allvec, strvec);
        }
      multiple++;
    }
      else
    {
      strvec = vector_init (VECTOR_MIN_SIZE);
      vector_set (allvec, strvec);
    }
      vector_set (strvec, desc);
    }
}

/* Count mandantory string vector size.  This is to determine inputed
   command has enough command length. */
static int
cmd_cmdsize (vector strvec)
{
  unsigned int i;
  int size = 0;
  vector descvec;
  struct desc *desc;

  for (i = 0; i < vector_active (strvec); i++)
    if ((descvec = vector_slot (strvec, i)) != NULL)
    {
      if ((vector_active (descvec)) == 1
        && (desc = vector_slot (descvec, 0)) != NULL)
    {
      if (desc->cmd == NULL || CMD_OPTION (desc->cmd))
        return size;
      else
        size++;
    }
      else
    size++;
    }
  return size;
}

/* Return prompt character of specified node. */
const char *
cmd_prompt (enum node_type node)
{
  struct cmd_node *cnode;

  cnode = vector_slot (cmdvec, node);
  return cnode->prompt;
}

int
tbl_calc(int index, int opt_index)
{
    if (index & (1 << opt_index))
        return 0;
    else
        return 1;
}

/* maximum expansion per command: for example, 32 means there may be at most
   5 optional fields per command, since 2^5=32 */
#define MAX_EXPAND_PER_CMD	1024 /*32*/
/* maximum length per command */
#define MAX_LEN_PER_CMD		1024
/* maximum length per command's help string */
#define MAX_LEN_PER_HELP	2048

/* Install a command into a node. */
void
install_element (enum node_type ntype, char priv, char nosortindex, struct cmd_element *cmd)
{
    struct cmd_node *cnode;
    char    cmd_tbl[MAX_EXPAND_PER_CMD][MAX_LEN_PER_CMD];
    char    help_tbl[MAX_EXPAND_PER_CMD][MAX_LEN_PER_HELP];
    char    cmd_buf[MAX_LEN_PER_CMD];
    char    help_buf[MAX_LEN_PER_HELP];
    const char *cmd_cp;
    const char *help_cp;
    const char *last_cmd_cp;
    const char *last_help_cp;
    int cmd_len;
    int help_len;
    int num_of_opts;
    int symbol_unmatch;
    int num_of_cmds;
    int i;
    int num_of_help_lines;
    int opt_index;
    struct cmd_element *cmd_el;

    cnode = vector_slot(cmdvec, ntype);

    if (cnode == NULL) {
        //DBG0( mylog("command.log", "Command node %d doesn't exist.\n", ntype) );
        exit(1);
    }

    /* check for null command string */
    if (cmd->string == NULL) {
        //DBG0( mylog("command.log", "Command string is empty.\n") );
        exit(1);
    }

    /* check the length of the command string */
    if (strlen(cmd->string) > MAX_LEN_PER_CMD) {
        //DBG0( mylog("command.log", "Command string too long: %s\n", cmd->string) );
        exit(1);
    }

    /* check the length of the command's help string */
    if (strlen(cmd->doc) > MAX_LEN_PER_HELP) {
        //DBG0( mylog("command.log", "Command's help string too long: %s\n", cmd->doc) );
        exit(1);
    }

    /* parse command string to count the number of optional fields, and
       check if each "[" are properly matched by "]" */
    num_of_opts = 0;
    symbol_unmatch = 0;
    cmd_cp = cmd->string;
    while (*cmd_cp != '\0') {
        if (*cmd_cp == '[') {
            ++num_of_opts;
            if (!symbol_unmatch) {
                symbol_unmatch = 1;
            }
            else {
                //DBG0( mylog("command.log", "Optional symbol mismatch: %s\n", cmd->string) );
                exit(1);
            }
        }
        else if (*cmd_cp == ']') {
            if (symbol_unmatch) {
                symbol_unmatch = 0;
            }
            else {
                //DBG0( mylog("command.log", "Optional symbol mismatch: %s\n", cmd->string) );
                exit(1);
            }
        }
        ++cmd_cp;
    }
    num_of_cmds = 1;
    for (i = 0; i < num_of_opts; ++i) {
        num_of_cmds *= 2;
    }
    if (num_of_cmds > MAX_EXPAND_PER_CMD) {
        //DBG0( mylog("command.log", "Too many optional fields: %s\n", cmd->string) );
        exit(1);
    }

    /* build cmd_tbl and help_tbl */
    for (i = 0; i < num_of_cmds; ++i) {
        cmd_tbl[i][0] = '\0';
        help_tbl[i][0] = '\0';
    }
    cmd_cp = cmd->string;
    help_cp = cmd->doc;
#ifdef DUMP_ALL_CMD_SYNTAX
    printf("======================================\n");
    printf("id:%d node:%d priv:%d\n", g_id, ntype, priv);
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("%s\n", cmd->string);
    printf("--------------------------------------\n");
    printf("%s\n", cmd->doc);
    printf("--------------------------------------\n");
    g_id++;
#endif

    last_cmd_cp = cmd_cp;
    last_help_cp = help_cp;
    num_of_help_lines = 0;
    opt_index = -1;
    /* skip leading white spaces */
    while (isspace((int)*cmd_cp) && (*cmd_cp != '\0')) {
        ++cmd_cp;
    }
    while (*cmd_cp != '\0') {
        if (isspace((int)*cmd_cp)) {
            while (isspace((int)*cmd_cp) && (*cmd_cp != '\0')) {
                ++cmd_cp;
            }
            ++num_of_help_lines;
        }
        else if (*cmd_cp == '|') {
            ++cmd_cp;
            ++num_of_help_lines;
        }
        else if (*cmd_cp == '[') {
            ++opt_index;
            cmd_len = cmd_cp - last_cmd_cp;
            if (cmd_len) {
                /* copy command string */
                strncpy(cmd_buf, last_cmd_cp, cmd_len);
                cmd_buf[cmd_len] = '\0';
                for (i = 0; i < num_of_cmds; ++i) {
                    strcat(cmd_tbl[i], cmd_buf);
                }
                /* copy help string */
                for (i = 0; i < num_of_help_lines; ++i) {
                    while ((*help_cp != '\n') && (*help_cp != '\0')) {
                        ++help_cp;
                    }
                    if (*help_cp == '\n') {
                        ++help_cp;
                    }
                }
                num_of_help_lines = 0; /* reset */
                help_len = help_cp - last_help_cp;
                if (help_len) {
                    strncpy(help_buf, last_help_cp, help_len);
                    help_buf[help_len] = '\0';
                    for (i = 0; i < num_of_cmds; ++i) {
                        strcat(help_tbl[i], help_buf);
                    }
                    last_help_cp = help_cp;
                }
            }
            ++cmd_cp;
            last_cmd_cp = cmd_cp;
            while (isspace((int)*cmd_cp) && (*cmd_cp != '\0')) {
                ++cmd_cp;
            }
        }
        else if (*cmd_cp == ']') {
            ++num_of_help_lines;
            cmd_len = cmd_cp - last_cmd_cp;
            if (cmd_len) {
                /* copy command string */
                strncpy(cmd_buf, last_cmd_cp, cmd_len);
                cmd_buf[cmd_len] = '\0';
                for (i = 0; i < num_of_cmds; ++i) {
                    if (tbl_calc(i, opt_index)) {
                        strcat(cmd_tbl[i], cmd_buf);
                    }
                }
                /* copy help string */
                for (i = 0; i < num_of_help_lines; ++i) {
                    while ((*help_cp != '\n') && (*help_cp != '\0')) {
                        ++help_cp;
                    }
                    if (*help_cp == '\n') {
                        ++help_cp;
                    }
                }
                num_of_help_lines = 0; /* reset */
                help_len = help_cp - last_help_cp;
                if (help_len) {
                    strncpy(help_buf, last_help_cp, help_len);
                    help_buf[help_len] = '\0';
                    for (i = 0; i < num_of_cmds; ++i) {
                        if (tbl_calc(i, opt_index)) {
                            strcat(help_tbl[i], help_buf);
                        }
                    }
                    last_help_cp = help_cp;
                }
            }
            else {
                //DBG0( mylog("command.log", "Empty optional parameter: %s\n", cmd->string) );
                exit(1);
            }
            ++cmd_cp;
            last_cmd_cp = cmd_cp;
            while (isspace((int)*cmd_cp) && (*cmd_cp != '\0')) {
                ++cmd_cp;
            }
        }
        else {
            ++cmd_cp;
        }
    }
    /* last token */
    ++num_of_help_lines;
    cmd_len = cmd_cp - last_cmd_cp;
    if (cmd_len) {
        /* copy command string */
        strncpy(cmd_buf, last_cmd_cp, cmd_len);
        cmd_buf[cmd_len] = '\0';
        for (i = 0; i < num_of_cmds; ++i) {
            strcat(cmd_tbl[i], cmd_buf);
        }
        last_cmd_cp = cmd_cp + 1;
        /* copy help string */
        for (i = 0; i < num_of_help_lines; ++i) {
            while ((*help_cp != '\n') && (*help_cp != '\0')) {
                ++help_cp;
            }
            if (*help_cp == '\n') {
                ++help_cp;
            }
        }
        num_of_help_lines = 0; /* reset */
        help_len = help_cp - last_help_cp;
        if (help_len) {
            strncpy(help_buf, last_help_cp, help_len);
            help_buf[help_len] = '\0';
            for (i = 0; i < num_of_cmds; ++i) {
                strcat(help_tbl[i], help_buf);
            }
            last_help_cp = help_cp;
        }
    }

    for (i = 0; i < num_of_cmds; ++i) {
        if (dump_cmd_syntax)
        {
            printf("{%d} %s\n", ntype, cmd_tbl[i]);
            continue;
        }
        //cmd_el = xmalloc(sizeof(struct cmd_element));
        cmd_el = XMALLOC(ntype, sizeof(struct cmd_element));
        memset(cmd_el, 0, sizeof(struct cmd_element));
        vector_set(cnode->cmd_vector, cmd_el);
        /* RTK tmp */
        cmd_el->cmdpriv = priv;
        cmd_el->attr = cmd->attr;
        cmd_el->hidmatch = cmd->hidmatch;
#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
        {
            int8 cfgPriv = -1;
            uint32 index = 0;
            sys_aaa_cmdnodeType_t node = SYS_AAA_CMDNODE_END;

            if (ntype == CONFIG_NODE)
            {
                node = SYS_AAA_CMDNODE_CONFIG;
                index = glbConfigCmdIndex;
                sal_aaa_cmdPriv_get(node, glbConfigCmdIndex, &cfgPriv);
                glbConfigCmdIndex++;
            }
            else if (ntype == ENABLE_NODE)
            {
                node = SYS_AAA_CMDNODE_EXEC;
                index = glbExecCmdIndex;
                sal_aaa_cmdPriv_get(node, glbExecCmdIndex, &cfgPriv);
                glbExecCmdIndex++;
            }
            if (cfgPriv < 0)
            {
                if (node != SYS_AAA_CMDNODE_END)
                {
                    sal_aaa_cmdPriv_set(node, index, priv);
                }
                cmd_el->cmdcurrpriv = priv;
            }
            else
            {
                cmd_el->cmdcurrpriv = cfgPriv;
            }
        }
#endif /* CONFIG_SYS_AAA_LOCALCMD_PRIVCHG */
        cmd_el->func = cmd->func;
        cmd_el->strvec = cmd_make_descvec(cmd_tbl[i], help_tbl[i]);
        cmd_el->cmdsize = cmd_cmdsize(cmd_el->strvec);
        cmd_el->nosortidx = nosortindex;
    }
}

/* Install a command into a node for SAL initialization. (SAL do not need help string) */
void
sal_install_element (enum node_type ntype, struct cmd_element *cmd)
{
    struct cmd_node *cnode;
    char    cmd_tbl[MAX_EXPAND_PER_CMD][MAX_LEN_PER_CMD];
    char    cmd_buf[MAX_LEN_PER_CMD];
    const char *cmd_cp;
    const char *last_cmd_cp;
    int cmd_len;
    int num_of_opts;
    int symbol_unmatch;
    int num_of_cmds;
    int i;
    int opt_index;
    struct cmd_element *cmd_el;

    cnode = vector_slot(cmdvec, ntype);

    if (cnode == NULL) {
        //DBG0( mylog("command.log", "Command node %d doesn't exist.\n", ntype) );
        exit(1);
    }

    /* check for null command string */
    if (cmd->string == NULL) {
        //DBG0( mylog("command.log", "Command string is empty.\n") );
        exit(1);
    }

    /* check the length of the command string */
    if (strlen(cmd->string) > MAX_LEN_PER_CMD) {
        //DBG0( mylog("command.log", "Command string too long: %s\n", cmd->string) );
        exit(1);
    }

    /* parse command string to count the number of optional fields, and
       check if each "[" are properly matched by "]" */
    num_of_opts = 0;
    symbol_unmatch = 0;
    cmd_cp = cmd->string;
    while (*cmd_cp != '\0') {
        if (*cmd_cp == '[') {
            ++num_of_opts;
            if (!symbol_unmatch) {
                symbol_unmatch = 1;
            }
            else {
                //DBG0( mylog("command.log", "Optional symbol mismatch: %s\n", cmd->string) );
                exit(1);
            }
        }
        else if (*cmd_cp == ']') {
            if (symbol_unmatch) {
                symbol_unmatch = 0;
            }
            else {
                //DBG0( mylog("command.log", "Optional symbol mismatch: %s\n", cmd->string) );
                exit(1);
            }
        }
        ++cmd_cp;
    }

    num_of_cmds = 1;
    for (i = 0; i < num_of_opts; ++i) {
        num_of_cmds *= 2;
    }
    if (num_of_cmds > MAX_EXPAND_PER_CMD) {
        //DBG0( mylog("command.log", "Too many optional fields: %s\n", cmd->string) );
        exit(1);
    }

    /* build cmd_tbl */
    for (i = 0; i < num_of_cmds; ++i) {
        cmd_tbl[i][0] = '\0';
    }
    cmd_cp = cmd->string;
    last_cmd_cp = cmd_cp;
    opt_index = -1;
    /* skip leading white spaces */
    while (isspace((int)*cmd_cp) && (*cmd_cp != '\0')) {
        ++cmd_cp;
    }
    while (*cmd_cp != '\0') {
        if (isspace((int)*cmd_cp)) {
            while (isspace((int)*cmd_cp) && (*cmd_cp != '\0')) {
                ++cmd_cp;
            }
        }
        else if (*cmd_cp == '|') {
            ++cmd_cp;
        }
        else if (*cmd_cp == '[') {
            ++opt_index;
            cmd_len = cmd_cp - last_cmd_cp;
            if (cmd_len) {
                /* copy command string */
                strncpy(cmd_buf, last_cmd_cp, cmd_len);
                cmd_buf[cmd_len] = '\0';
                for (i = 0; i < num_of_cmds; ++i) {
                    strcat(cmd_tbl[i], cmd_buf);
                }
            }
            ++cmd_cp;
            last_cmd_cp = cmd_cp;
            while (isspace((int)*cmd_cp) && (*cmd_cp != '\0')) {
                ++cmd_cp;
            }
        }
        else if (*cmd_cp == ']') {
            cmd_len = cmd_cp - last_cmd_cp;
            if (cmd_len) {
                /* copy command string */
                strncpy(cmd_buf, last_cmd_cp, cmd_len);
                cmd_buf[cmd_len] = '\0';
                for (i = 0; i < num_of_cmds; ++i) {
                    if (tbl_calc(i, opt_index)) {
                        strcat(cmd_tbl[i], cmd_buf);
                    }
                }
            }
            else {
                //DBG0( mylog("command.log", "Empty optional parameter: %s\n", cmd->string) );
                exit(1);
            }
            ++cmd_cp;
            last_cmd_cp = cmd_cp;
            while (isspace((int)*cmd_cp) && (*cmd_cp != '\0')) {
                ++cmd_cp;
            }
        }
        else {
            ++cmd_cp;
        }
    }
    /* last token */
    cmd_len = cmd_cp - last_cmd_cp;
    if (cmd_len) {
        /* copy command string */
        strncpy(cmd_buf, last_cmd_cp, cmd_len);
        cmd_buf[cmd_len] = '\0';
        for (i = 0; i < num_of_cmds; ++i) {
            strcat(cmd_tbl[i], cmd_buf);
        }
        last_cmd_cp = cmd_cp + 1;
    }

    for (i = 0; i < num_of_cmds; ++i) {
        cmd_el = XMALLOC(ntype, sizeof(struct cmd_element));
        memset(cmd_el, 0, sizeof(struct cmd_element));
        cmd_el->func = cmd->func;
        cmd_el->strvec = cmd_make_descvec(cmd_tbl[i], cmd->doc);
        cmd_el->cmdsize = cmd_cmdsize(cmd_el->strvec);
        vector_set(cnode->cmd_vector, cmd_el);
    }

}


#if 0

static const unsigned char itoa64[] =
"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static void
to64(char *s, long v, int n)
{
  while (--n >= 0)
    {
      *s++ = itoa64[v&0x3f];
      v >>= 6;
    }
}

static char *
zencrypt (const char *passwd)
{
  char salt[6];
  struct timeval tv;
  char *crypt (const char *, const char *);

  gettimeofday(&tv,0);

  to64(&salt[0], random(), 3);
  to64(&salt[3], tv.tv_usec, 3);
  salt[5] = '\0';

  return crypt (passwd, salt);
}

/* This function write configuration of this host. */
static int
config_write_host (struct vty *vty)
{
  if (host.name)
    vty_out (vty, "hostname %s%s", host.name, VTY_NEWLINE);

  if (host.encrypt)
    {
      if (host.password_encrypt)
        vty_out (vty, "password 8 %s%s", host.password_encrypt, VTY_NEWLINE);
      if (host.enable_encrypt)
        vty_out (vty, "enable password 8 %s%s", host.enable_encrypt, VTY_NEWLINE);
    }
  else
    {
      if (host.password)
        vty_out (vty, "password %s%s", host.password, VTY_NEWLINE);
      if (host.enable)
        vty_out (vty, "enable password %s%s", host.enable, VTY_NEWLINE);
    }

  if (zlog_default->default_lvl != LOG_DEBUG)
    {
      vty_out (vty, "! N.B. The 'log trap' command is deprecated.%s",
           VTY_NEWLINE);
      vty_out (vty, "log trap %s%s",
           zlog_priority[zlog_default->default_lvl], VTY_NEWLINE);
    }

  if (host.logfile && (zlog_default->maxlvl[ZLOG_DEST_FILE] != ZLOG_DISABLED))
    {
      vty_out (vty, "log file %s", host.logfile);
      if (zlog_default->maxlvl[ZLOG_DEST_FILE] != zlog_default->default_lvl)
    vty_out (vty, " %s",
         zlog_priority[zlog_default->maxlvl[ZLOG_DEST_FILE]]);
      vty_out (vty, "%s", VTY_NEWLINE);
    }

  if (zlog_default->maxlvl[ZLOG_DEST_STDOUT] != ZLOG_DISABLED)
    {
      vty_out (vty, "log stdout");
      if (zlog_default->maxlvl[ZLOG_DEST_STDOUT] != zlog_default->default_lvl)
    vty_out (vty, " %s",
         zlog_priority[zlog_default->maxlvl[ZLOG_DEST_STDOUT]]);
      vty_out (vty, "%s", VTY_NEWLINE);
    }

  if (zlog_default->maxlvl[ZLOG_DEST_MONITOR] == ZLOG_DISABLED)
    vty_out(vty,"no log monitor%s",VTY_NEWLINE);
  else if (zlog_default->maxlvl[ZLOG_DEST_MONITOR] != zlog_default->default_lvl)
    vty_out(vty,"log monitor %s%s",
        zlog_priority[zlog_default->maxlvl[ZLOG_DEST_MONITOR]],VTY_NEWLINE);

  if (zlog_default->maxlvl[ZLOG_DEST_SYSLOG] != ZLOG_DISABLED)
    {
      vty_out (vty, "log syslog");
      if (zlog_default->maxlvl[ZLOG_DEST_SYSLOG] != zlog_default->default_lvl)
    vty_out (vty, " %s",
         zlog_priority[zlog_default->maxlvl[ZLOG_DEST_SYSLOG]]);
      vty_out (vty, "%s", VTY_NEWLINE);
    }

  if (zlog_default->facility != LOG_DAEMON)
    vty_out (vty, "log facility %s%s",
         facility_name(zlog_default->facility), VTY_NEWLINE);

  if (zlog_default->record_priority == 1)
    vty_out (vty, "log record-priority%s", VTY_NEWLINE);

  if (zlog_default->timestamp_precision > 0)
    vty_out (vty, "log timestamp precision %d%s",
         zlog_default->timestamp_precision, VTY_NEWLINE);

  if (host.advanced)
    vty_out (vty, "service advanced-vty%s", VTY_NEWLINE);

  if (host.encrypt)
    vty_out (vty, "service password-encryption%s", VTY_NEWLINE);

  if (host.lines >= 0)
    vty_out (vty, "service terminal-length %d%s", host.lines,
         VTY_NEWLINE);

  if (host.motdfile)
    vty_out (vty, "banner motd file %s%s", host.motdfile, VTY_NEWLINE);
  else if (! host.motd)
    vty_out (vty, "no banner motd%s", VTY_NEWLINE);

  return 1;
}
#endif

/* Utility function for getting command vector. */
vector
cmd_node_vector (vector v, enum node_type ntype)
{
  struct cmd_node *cnode = vector_slot (v, ntype);
  return cnode->cmd_vector;
}

#if 0
/* Filter command vector by symbol.  This function is not actually used;
 * should it be deleted? */
static int
cmd_filter_by_symbol (char *command, char *symbol)
{
  int i, lim;

  if (strcmp (symbol, "IPV4_ADDRESS") == 0)
    {
      i = 0;
      lim = strlen (command);
      while (i < lim)
    {
      if (! (isdigit ((int) command[i]) || command[i] == '.' || command[i] == '/'))
        return 1;
      i++;
    }
      return 0;
    }
  if (strcmp (symbol, "STRING") == 0)
    {
      i = 0;
      lim = strlen (command);
      while (i < lim)
    {
      if (! (isalpha ((int) command[i]) || command[i] == '_' || command[i] == '-'))
        return 1;
      i++;
    }
      return 0;
    }
  if (strcmp (symbol, "IFNAME") == 0)
    {
      i = 0;
      lim = strlen (command);
      while (i < lim)
    {
      if (! isalnum ((int) command[i]))
        return 1;
      i++;
    }
      return 0;
    }
  return 0;
}
#endif

/* Completion match types. */
enum match_type
{
  no_match,
  extend_match,
  bad_param_match,
  ipv4_prefix_match,
  ipv4_match,
  ipv4_mask_match,
  ipv6_prefix_match,
  ipv6_match,
  ipv6_mask_match,
  hostname_match,
  mac_addr_match,
  mac_addr_mask_match,
  mac_oui_match,
  port_match,
  port_list_match,
  normal_wotrkmbr_port_list_match,
  normal_port_list_match,
  intf_single_match,
  intf_range_match,
  vlan_list_match,
  queue_list_match,
  dscp_list_match,
  range_match,
  vararg_match,
  partly_match,
  trunk_mbr_match,
  invalid_port_match,
  invalid_portlist_match,
  invalid_vlan_match,
#ifdef CONFIG_SYS_L3_ROUTE
  invalid_lo_match,
#endif
  invalid_value_match,
  range_format_match,
  invalid_range_format_match,
  invalid_range_match,
  word_match,
  fs_match,
  invalid_ipmask_match,
  invalid_macmask_match,
  invalid_hostname_match,
  exact_match
};

static enum match_type
cmd_ipv4_match (const char *str)
{
  const char *sp;
  int dots = 0, nums = 0;
  char buf[4];

  if (str == NULL)
    return partly_match;

  for (;;)
    {
      memset (buf, 0, sizeof (buf));
      sp = str;
      while (*str != '\0')
    {
      if (*str == '.')
        {
          if (dots >= 3)
        return no_match;

          if (*(str + 1) == '.')
        return no_match;

          if (*(str + 1) == '\0')
        return no_match;

          dots++;
          break;
        }
      if (!isdigit ((int) *str))
        return no_match;

      str++;
    }

      if (str - sp > 3)
    return no_match;

      strncpy (buf, sp, str - sp);
      if (atoi (buf) > 255)
    return no_match;

      nums++;

      if (*str == '\0')
    break;

      str++;
    }

//  if (nums < 4)
//    return partly_match;
  if (nums != 4 || dots != 3)
    return no_match;

  return exact_match;
}

static enum match_type
cmd_ipv4_prefix_match (const char *str)
{
  const char *sp;
  int dots = 0;
  char buf[4];

  if (str == NULL)
    return partly_match;

  for (;;)
    {
      memset (buf, 0, sizeof (buf));
      sp = str;
      while (*str != '\0' && *str != '/')
    {
      if (*str == '.')
        {
          if (dots == 3)
        return no_match;

          if (*(str + 1) == '.' || *(str + 1) == '/')
        return no_match;

          if (*(str + 1) == '\0')
        return partly_match;

          dots++;
          break;
        }

      if (!isdigit ((int) *str))
        return no_match;

      str++;
    }

      if (str - sp > 3)
    return no_match;

      strncpy (buf, sp, str - sp);
      if (atoi (buf) > 255)
    return no_match;

      if (dots == 3)
    {
      if (*str == '/')
        {
          if (*(str + 1) == '\0')
        return partly_match;

          str++;
          break;
        }
      else if (*str == '\0')
        return partly_match;
    }

      if (*str == '\0')
    return partly_match;

      str++;
    }

  sp = str;
  while (*str != '\0')
    {
      if (!isdigit ((int) *str))
    return no_match;

      str++;
    }

  if (atoi (sp) > 32)
    return no_match;

  return exact_match;
}

static enum match_type
cmd_ipv4_mask_match (const char *str)
{
    char *ridx = NULL;
    char buf[64];

    if (str == NULL)
        return partly_match;

    ridx = rindex(str, '/');

    if (NULL == ridx)
        return no_match;

    memset(buf, 0, sizeof(buf));
    strncpy(buf, str, ridx-str);

    if (exact_match != (cmd_ipv4_match(buf)))
        return no_match;

    memset(buf, 0, sizeof(buf));
    strcpy(buf, ridx+1);

    if (exact_match != (cmd_ipv4_match(buf)))
        return no_match;

    return exact_match;
}

static enum match_type
cmd_host_match (const char *str)
{
    if (str == NULL)
        return partly_match;

    if (IS_HOSTNAME_INVALID((char*)str))
    {
        if (IS_IPV6_ADDRSTR_INVALID(str))
            return no_match;
    }

    return exact_match;
}

enum match_type
cmd_mac_addr_match (const char *str)
{
    char *    sp;
    int    colons = 0;
    int    nums = 0;
    int    len;
    int    end_with_symbol = 0;

    if (str == NULL) {
        return partly_match;
    }

    if ( !isxdigit((int) *str) ) {
        return no_match;
    }

    while (1) {
        sp = (char*)str;
        while (*str != '\0') {
            if (*str == ':' || *str == '-') {
                if (colons >= 5) {
                    return no_match;
                }

                if (*(str + 1) == ':' || *(str + 1) == '-') {
                    return no_match;
                }

                if (*(str + 1) == '\0') {
                    end_with_symbol = 1;
                }
                ++colons;
                break;
            }
            if ( !isxdigit((int) *str) ) {
                return no_match;
            }

            ++str;
        }

        len = str - sp;
        if (len > 2) {
            return no_match;
        }

        if (end_with_symbol) {
            return no_match;
        }

        ++nums;

        if (*str == '\0') {
            break;
        }
        ++str;
    }

    if (nums < 6 || colons != 5) {
        return no_match;
    }

    return exact_match;
}

static enum match_type
cmd_mac_addr_mask_match (const char *str)
{
    char *ridx = NULL;
    char buf[20];

    if (str == NULL)
        return partly_match;

    ridx = rindex(str, '/');

    if (NULL == ridx)
        return no_match;

    memset(buf, 0, sizeof(buf));
    strncpy(buf, str, ridx-str);

    if (exact_match != (cmd_mac_addr_match(buf)))
        return no_match;

    memset(buf, 0, sizeof(buf));
    strcpy(buf, ridx+1);

    if (exact_match != (cmd_mac_addr_match(buf)))
        return no_match;

    return exact_match;
}

enum match_type
cmd_mac_oui_match (const char *str)
{
    char *    sp;
    int    colons = 0;
    int    nums = 0;
    int    len;
    int    end_with_symbol = 0;

    if (str == NULL) {
        return partly_match;
    }

    if ( !isxdigit((int) *str) ) {
        return no_match;
    }

    while (1) {
        sp = (char*)str;
        while (*str != '\0') {
            if (*str == ':' || *str == '-') {
                if (colons >= 2) {
                    return no_match;
                }

                if (*(str + 1) == ':' || *(str + 1) == '-') {
                    return no_match;
                }

                if (*(str + 1) == '\0') {
                    end_with_symbol = 1;
                }
                ++colons;
                break;
            }
            if ( !isxdigit((int) *str) ) {
                return no_match;
            }

            ++str;
        }

        len = str - sp;
        if (len > 2) {
            return no_match;
        }

        if (end_with_symbol) {
            return no_match;
        }

        ++nums;

        if (*str == '\0') {
            break;
        }
        ++str;
    }

    if (nums < 3 || colons != 2) {
        return no_match;
    }

    return exact_match;
}


enum match_type
cmd_port_match (const char *arg)
{
    int32 ret;
    char buf[10] = {0};
    sys_logic_port_t logicPort;

    if (NULL == arg)
        return invalid_port_match;

    osal_strcpy(buf, arg);

    ret = STR2LP(buf, &logicPort);

    if (ret != SYS_ERR_OK)
        return invalid_port_match;

    if (IS_LOGIC_PORT_INVALID(logicPort))
        return invalid_port_match;

    return exact_match;
}

enum match_type
cmd_port_list_match (const char *arg)
{
    int32 ret;
    sys_logic_portmask_t logicPortmask;

    if (NULL == arg)
        return partly_match;

    osal_memset(&logicPortmask, 0, sizeof(logicPortmask));

    if ('a' == arg[0]) /* all */
    {
        LOGIC_PORTMASK_SET_ALL(logicPortmask);

        return exact_match;
    }

    ret = STR2LPM(arg, &logicPortmask);

    if (ret != SYS_ERR_OK)
        return invalid_portlist_match;

    return exact_match;
}

enum match_type
cmd_normal_wotrkmbr_port_list_match (const char *arg)
{
    int32 ret;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;

    if (NULL == arg)
        return partly_match;

    osal_memset(&logicPortmask, 0, sizeof(logicPortmask));

    ret = STR2LPM(arg, &logicPortmask);

    if (ret != SYS_ERR_OK)
        return invalid_portlist_match;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        if (IS_TRKLP(logicPort))
        {
            return invalid_portlist_match;
        }
        else if (IS_LP_TRKMBR(logicPort))
        {
            return trunk_mbr_match;
        }
    }

    return exact_match;
}

enum match_type
cmd_normal_port_list_match (const char *arg)
{
    int32 ret;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;

    if (NULL == arg)
        return partly_match;

    osal_memset(&logicPortmask, 0, sizeof(logicPortmask));

    if ('a' == arg[0]) /* all */
    {
        NORMAL_LOGIC_PORTMASK_SET_ALL(logicPortmask);

        return exact_match;
    }

    ret = STR2LPM(arg, &logicPortmask);

    if (ret != SYS_ERR_OK)
        return invalid_portlist_match;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        if (IS_TRKLP(logicPort))
            return invalid_portlist_match;
    }

    return exact_match;
}

enum match_type
cmd_if_match (const char *arg, uint32 isRange, char *cmdFmt)
{
    char str[SYS_BUF128_LEN];
    int32 ret = SYS_ERR_FAILED;
    uint32 i = 0;
    uint32 j = 0;
#ifdef CONFIG_SYS_L3_ROUTE
    uint32 isVlan = FALSE;
    uint32 isLo = FALSE;
#endif
    uint32 foundSpace = FALSE;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
#ifdef CONFIG_SYS_L3_ROUTE
    sys_vlanmask_t vmsk;
#endif

    if (NULL == arg)
        return partly_match;

    SYS_MEM_CLEAR(str);
    osal_strcpy(str, arg);

    sal_util_str2lowerStr_xlate(str);

    if (NULL != cmdFmt)
    {
        if ((CMD_IF_FA_PORT(cmdFmt) || CMD_IF_FA_PORTS(cmdFmt))  && 0 != osal_strncmp(IF_FA_LOWSTR, str, 1))
            return no_match;
        if ((CMD_IF_GI_PORT(cmdFmt) || CMD_IF_GI_PORTS(cmdFmt)) && 0 != osal_strncmp(IF_GI_LOWSTR, str, 1))
            return no_match;
        if ((CMD_IF_MGI_PORT(cmdFmt) || CMD_IF_MGI_PORTS(cmdFmt)) && 0 != osal_strncmp(IF_MGI_LOWSTR, str, 1))
            return no_match;
        if ((CMD_IF_TE_PORT(cmdFmt) || CMD_IF_TE_PORTS(cmdFmt)) && 0 != osal_strncmp(IF_TE_LOWSTR, str, 1))
            return no_match;
#ifdef CONFIG_SYS_L3_ROUTE
        if ((CMD_IF_PO_PORT(cmdFmt) || CMD_IF_PO_PORTS(cmdFmt)) && (1 == strlen(str)))
            return partly_match;
        else if ((CMD_IF_PO_PORT(cmdFmt) || CMD_IF_PO_PORTS(cmdFmt)) && 0 != osal_strncmp(IF_PO_LOWSTR, str, 2))
            return no_match;
        if ((CMD_IF_VLAN(cmdFmt) || CMD_IF_VLANS(cmdFmt)) && 0 != osal_strncmp(IF_VLAN_LOWSTR, str, 1))
            return no_match;
        if ((CMD_IF_LO(cmdFmt) || CMD_IF_LOS(cmdFmt)) && (1 == strlen(str)))
            return partly_match;
        else if ((CMD_IF_LO(cmdFmt) || CMD_IF_LOS(cmdFmt)) && 0 != osal_strncmp(IF_LO_LOWSTR, str, 2))
            return no_match;
#else
        if ((CMD_IF_PO_PORT(cmdFmt) || CMD_IF_PO_PORTS(cmdFmt)) && 0 != osal_strncmp(IF_PO_LOWSTR, str, 1))
            return no_match;

#endif
    }

    if (0 == osal_strncmp(IF_FA_LOWSTR, str, 1)    /* fastethernet */
        || 0 == osal_strncmp(IF_GI_LOWSTR, str, 1) /* gigabitethernet */
        || 0 == osal_strncmp(IF_MGI_LOWSTR, str, 1) /*multi gigabitethernet */
        || 0 == osal_strncmp(IF_TE_LOWSTR, str, 1) /* tengigabitethernet */
#if !defined(CONFIG_SYS_L3_ROUTE)
        || 0 == osal_strncmp(IF_PO_LOWSTR, str, 1) /* lag */
#endif
       )
    {
#ifdef CONFIG_SYS_L3_ROUTE
        isVlan = FALSE;
#else
#endif
    }
#ifdef CONFIG_SYS_L3_ROUTE
    else if ('l' == tolower(str[0])) /* loopback */
    {
        if (1 == strlen(str))
            return partly_match;

        if (0 == osal_strncmp(IF_LO_LOWSTR, str, 2))
            isLo = TRUE;

    }
    else if ('v' == tolower(str[0])) /* vlan */
        isVlan = TRUE;
#endif
    else if ('a' == tolower(str[0]))
        return exact_match;
    else
        return no_match;

#ifdef CONFIG_SYS_L3_ROUTE
    if (isVlan)
    {
        if (TRUE == isRange)
        {
            ret = STR2VMSK(str, &vmsk);
        }
        else
        {
            uint32 numIdx = 0;
            for (i = 0; i < osal_strlen(str); i++)
            {
                if (str[i] >= 0x30 && str[i] <= 0x39)
                {
                    numIdx = i;

                    break;
                }
            }

            if (IS_VID_INVALID(atoi(str + numIdx)))
                ret = SYS_ERR_VLAN_ID;
            else
                ret = SYS_ERR_OK;
        }
    }
    else if (isLo)
    {
        uint32 numIdx = 0;
        uint32 loId = 0;
        for (i = 0; i < osal_strlen(str); i++)
        {
            if (str[i] >= 0x30 && str[i] <= 0x39)
            {
                numIdx = i;

                break;
            }
        }

        loId = atoi(str + numIdx);

        if (loId < 1 || loId > CAPA_L3_INTFLO_NUM)
            ret = SYS_ERR_L3_INTF_LO_ID_INVALID;
        else
            ret = SYS_ERR_OK;
    }
    else
#endif
    {
        if (TRUE == isRange)
            ret = STR2LPM(str, &lpm);
        else
            ret = STR2LP(str, &lp);
    }

    if (SYS_ERR_OK != ret)
    {
        for (i = 0; i < osal_strlen(arg); i++)
        {
            if (' ' == arg[i])
            {
                for (j = i + 1; j < osal_strlen(arg); j++)
                {
                    if (' ' != arg[j] && '\0' != arg[j])
                    {
#ifdef CONFIG_SYS_L3_ROUTE
                        if (TRUE == isVlan)
                            return invalid_vlan_match;
                        else if (TRUE == isLo)
                            return invalid_lo_match;
                        else
#endif
                            return invalid_port_match;
                    }
                }

                foundSpace = TRUE;
            }
        }

        if (FALSE == foundSpace)
        {
#ifdef CONFIG_SYS_L3_ROUTE
            if (TRUE == isVlan)
                return invalid_vlan_match;
            else if (TRUE == isLo)
                return invalid_lo_match;
            else
#endif
                return invalid_port_match;
        }
    }

    return exact_match;
}

enum match_type
cmd_vlan_list_match (const char *arg)
{
    int32 ret = -1;
    sys_vlanmask_t vmsk;

    if (NULL == arg)
        return partly_match;

    SYS_MEM_CLEAR(vmsk);
    ret = sys_util_str2Vlanmask(arg, &vmsk);

    if (SYS_ERR_OK != ret)
        return invalid_vlan_match;

    return exact_match;
}

enum match_type
cmd_value_list_match (const char *arg, int min, int max)
{
    char *c = NULL, *token = NULL, buf[128] = {0};
    int val = 0, startVal = 0, endVal = 0;
    int range = FALSE, startFlag = FALSE, endFlag = FALSE;


    if (NULL == arg)
        return partly_match;

    if (*arg == ',' || *arg == '-')
        return no_match;

    strcpy(buf, arg);

    if (buf[1] == '-' || buf[2] == '-')
    {
        range = TRUE;
    }

    c = strtok_r(buf, ",-", &token);

    if (token != NULL && (*token == '-' || *token == '\0' || *token == ','))
    {
        return no_match;
    }

    if (range == TRUE)
    {
        if (startFlag == FALSE && endFlag == FALSE)
        {
            val = atoi(c);
            startVal = val;
            startFlag = TRUE;
        }

        c = strtok_r(NULL, ",-", &token);
    }

    while (c != NULL)
    {
        val = atoi(c);

        if (range == TRUE)
        {
            if (startFlag == FALSE && endFlag == FALSE)
            {
                startVal = val;
                startFlag = TRUE;
                goto PROCEED;
            }

            if (startFlag == TRUE && endFlag == FALSE)
            {
                endVal = val;
                endFlag = TRUE;
                goto PROCEED;
            }
        }

        PROCEED:

        if ((val < min) || (val > max) ||
            (token != NULL && (*token == '-' || *token == '\0' || *token == ',')))
        {
            return no_match;
        }

        if (TRUE == startFlag && TRUE == endFlag && startVal >= endVal)
        {
            return no_match;
        }

        if (NULL != token && (*(token + 1) == '-' || *(token + 2) == '-'))
        {
            range = TRUE;
            startFlag = endFlag = FALSE;
        }

        if (TRUE == startFlag && TRUE == endFlag)
        {
            startFlag = endFlag = range = FALSE;
        }

        c = strtok_r(NULL, ",-", &token);
    }

    return exact_match;
}

enum match_type
cmd_value_match (const char *arg)
{
    char buf[20];

    if (NULL == arg)
        return invalid_value_match;

    memset(buf, 0, sizeof(buf));



    /* Max value of 64bits is 18446744073709551615 or 0xFFFFFFFFFFFFFFFF.
       18 chars is enough */
    if (strlen(arg) > 18 || 0 == strlen(arg))
    {
        return invalid_value_match;
    }

    strcpy(buf, arg);

    if ((strlen(buf) <= 2) && IS_STR_HAS_INVALID_CHAR(buf, "0123456789"))
    {
        return invalid_value_match;
    }
    else if (('0' == buf[0]) && ('x' == buf[1]))
    {
        if (IS_STR_HAS_INVALID_CHAR(buf + 2, "0123456789abcdefABCDEF"))
            return invalid_value_match;
        else
            return exact_match;
    }
    else if (IS_STR_HAS_INVALID_CHAR(buf, "0123456789"))
    {
        return invalid_value_match;
    }

    return exact_match;
}


#define IPV6_ADDR_STR        "0123456789abcdefABCDEF:.%"
#define IPV6_PREFIX_STR        "0123456789abcdefABCDEF:.%/"
#define STATE_START        1
#define STATE_COLON        2
#define STATE_DOUBLE        3
#define STATE_ADDR        4
#define STATE_DOT               5
#define STATE_SLASH        6
#define STATE_MASK        7

#ifdef HAVE_IPV6

static enum match_type
cmd_ipv6_match (const char *str)
{
#if 0
  int state = STATE_START;
  int colons = 0, nums = 0, double_colon = 0;
  const char *sp = NULL;
#endif

  struct sockaddr_in6 sin6_dummy;
  int ret;

  if (str == NULL)
    return partly_match;

  if (strspn (str, IPV6_ADDR_STR) != strlen (str))
    return no_match;

  /* use inet_pton that has a better support,
   * for example inet_pton can support the automatic addresses:
   *  ::1.2.3.4
   */
  ret = inet_pton(AF_INET6, str, &sin6_dummy.sin6_addr);

  if (ret == 1)
    return exact_match;
  else
    return no_match;
#if 0
  while (*str != '\0')
    {
      switch (state)
    {
    case STATE_START:
      if (*str == ':')
        {
          if (*(str + 1) != ':' && *(str + 1) != '\0')
        return no_match;
               colons--;
          state = STATE_COLON;
        }
      else
        {
          sp = str;
          state = STATE_ADDR;
        }

      continue;
    case STATE_COLON:
      colons++;
      if (*(str + 1) == ':')
        state = STATE_DOUBLE;
      else
        {
          sp = str + 1;
          state = STATE_ADDR;
        }
      break;
    case STATE_DOUBLE:
      if (double_colon)
        return no_match;

      if (*(str + 1) == ':')
        return no_match;
      else
        {
          if (*(str + 1) != '\0')
        colons++;
          sp = str + 1;
          state = STATE_ADDR;
        }

      double_colon++;
      nums++;
      break;
    case STATE_ADDR:
      if (*(str + 1) == ':' || *(str + 1) == '\0')
        {
          if (str - sp > 3)
        return no_match;

          nums++;
          state = STATE_COLON;
        }
      if (*(str + 1) == '.')
        state = STATE_DOT;
      break;
    case STATE_DOT:
      state = STATE_ADDR;
      break;
    default:
      break;
    }

      if (nums > 8)
    return no_match;

      if (colons > 7)
    return no_match;

      str++;
    }

  if (nums < 11)
    return partly_match;
#endif /* 0 */

  return exact_match;
}

static enum match_type
cmd_ipv6_prefix_match (const char *str)
{
  int state = STATE_START;
  int colons = 0, nums = 0, double_colon = 0;
  int mask;
  const char *sp = NULL;
  char *endptr = NULL;

  if (str == NULL)
    return partly_match;

  if (strspn (str, IPV6_PREFIX_STR) != strlen (str))
    return no_match;

  while (*str != '\0' && state != STATE_MASK)
    {
      switch (state)
    {
    case STATE_START:
      if (*str == ':')
        {
          if (*(str + 1) != ':' && *(str + 1) != '\0')
        return no_match;
          colons--;
          state = STATE_COLON;
        }
      else
        {
          sp = str;
          state = STATE_ADDR;
        }

      continue;
    case STATE_COLON:
      colons++;
      if (*(str + 1) == '/')
        return no_match;
      else if (*(str + 1) == ':')
        state = STATE_DOUBLE;
      else
        {
          sp = str + 1;
          state = STATE_ADDR;
        }
      break;
    case STATE_DOUBLE:
      if (double_colon)
        return no_match;

      if (*(str + 1) == ':')
        return no_match;
      else
        {
          if (*(str + 1) != '\0' && *(str + 1) != '/')
        colons++;
          sp = str + 1;

          if (*(str + 1) == '/')
        state = STATE_SLASH;
          else
        state = STATE_ADDR;
        }

      double_colon++;
      nums += 1;
      break;
    case STATE_ADDR:
      if (*(str + 1) == ':' || *(str + 1) == '.'
          || *(str + 1) == '\0' || *(str + 1) == '/')
        {
          if (str - sp > 3)
        return no_match;

          for (; sp <= str; sp++)
        if (*sp == '/')
          return no_match;

          nums++;

          if (*(str + 1) == ':')
        state = STATE_COLON;
          else if (*(str + 1) == '.')
        state = STATE_DOT;
          else if (*(str + 1) == '/')
        state = STATE_SLASH;
        }
      break;
    case STATE_DOT:
      state = STATE_ADDR;
      break;
    case STATE_SLASH:
      if (*(str + 1) == '\0')
        return partly_match;

      state = STATE_MASK;
      break;
    default:
      break;
    }

      if (nums > 11)
    return no_match;

      if (colons > 7)
    return no_match;

      str++;
    }

  if (state < STATE_MASK)
    return partly_match;

  mask = strtol (str, &endptr, 10);
  if (*endptr != '\0')
    return no_match;

  if (mask < 0 || mask > 128)
    return no_match;

/* I don't know why mask < 13 makes command match partly.
   Forgive me to make this comments. I Want to set static default route
   because of lack of function to originate default in ospf6d; sorry
       yasu
  if (mask < 13)
    return partly_match;
*/

  return exact_match;
}

static enum match_type
cmd_ipv6_mask_match (const char *str)
{
    char *ridx = NULL;
    char buf[64];
    uint8 i = 0;
    uint32 prefix = 0;

    if (str == NULL)
        return partly_match;

    ridx = rindex(str, '/');

    if (NULL == ridx)
        return no_match;

    memset(buf, 0, sizeof(buf));
    strncpy(buf, str, ridx-str);

    if (exact_match != (cmd_ipv6_match(buf)))
        return no_match;

    ridx++;

    if (0 == osal_strlen(ridx) || osal_strlen(ridx) > 3)
        return no_match;

    for (i = 0; i < osal_strlen(ridx); i++)
    {
        if (!isdigit(ridx[i]))
            return no_match;
    }

    prefix = SYS_STR2UINT(ridx);

    if (prefix >= 129)
        return no_match;

    return exact_match;
}
#endif /* HAVE_IPV6  */

#define DECIMAL_STRLEN_MAX 10

static int
cmd_range_match (const char *range, const char *str)
{
  char *p;
  char buf[DECIMAL_STRLEN_MAX + 1];
  char *endptr = NULL;
  unsigned long min, max, val;
  char isHex = FALSE;

  if (str == NULL)
    return partly_match;

  if (0 != strstr(range, "0x"))
    isHex = TRUE;

   if (isHex)
   {
    if (!sal_util_validStringCheck_ret((char*)str, "1234567890xabcdefABCDEF"))
        return no_match;
   }
   else
   {
    if (!sal_util_validStringCheck_ret((char*)str, "1234567890"))
        return no_match;
   }

  if (osal_strlen(str) > DECIMAL_STRLEN_MAX)
    return bad_param_match;

  val = strtoul (str, &endptr, 0);
  if (*endptr != '\0')
    return no_match;

  if ((val == 0xFFFFFFFF) && (0 != osal_strcmp(str, "4294967295")))
    return bad_param_match;

  range++;
  p = strchr (range, '-');
  if (p == NULL)
    return no_match;
  if (p - range > DECIMAL_STRLEN_MAX)
    return no_match;
  strncpy (buf, range, p - range);
  buf[p - range] = '\0';

  if (isHex && 0 == strstr(buf, "0x"))
    return no_match;

  min = strtoul (buf, &endptr, 0);

  if (*endptr != '\0')
    return no_match;

  range = p + 1;
  p = strchr (range, '>');
  if (p == NULL)
    return no_match;
  if (p - range > DECIMAL_STRLEN_MAX)
    return no_match;
  strncpy (buf, range, p - range);
  buf[p - range] = '\0';
  max = strtoul (buf, &endptr, 0);

  if (isHex && 0 == strstr(buf, "0x"))
    return no_match;

  if (*endptr != '\0')
    return no_match;

  if (val < min || val > max)
    return bad_param_match;

  return exact_match;
}

static enum match_type
cmd_range_format_match (const char *range, const char *str)
{
    char buf[DECIMAL_STRLEN_MAX + 1];
    char *p = NULL;
    char *s = NULL;
    char *e = NULL;
    uint32 min;
    uint32 max;
    uint32 valmin;
    uint32 valmax;

    if (str == NULL)
        return partly_match;

    if (0 != osal_strncmp(range, "RANGE<", strlen("RANGE<")))
        return exact_match;

    /* Get command syntax range min and max value */
    s = (char *)(range + osal_strlen("RANGE<"));
    p = osal_strchr(s, '-');

    if (NULL == p)
        return invalid_range_format_match;

    if ((p - s) > DECIMAL_STRLEN_MAX)
        return invalid_range_format_match;

    osal_memset(buf, 0, sizeof(buf));
    osal_strncpy (buf, s, (p - s));
    min = SYS_STR2UINT(buf);

    p = p + 1;
    e = osal_strchr(p, '>');

    if (NULL == e)
        return invalid_range_format_match;

    if (0 == (e - p) || ((e - p) > DECIMAL_STRLEN_MAX))
        return invalid_range_format_match;

    osal_memset(buf, 0, sizeof(buf));
    osal_strncpy (buf, p, (e - p));
    max = SYS_STR2UINT(buf);

    if (min > max)
        return invalid_range_match;

    /* Get user input range min and max value */
    p = osal_strchr(str, '-');
    if (NULL == p)
    {
        if (strspn (str, VALID_NUM_STR) != strlen (str))
            return invalid_range_format_match;

        valmin = SYS_STR2UINT(str);
        valmax = valmin;

        if (valmin < min || valmin > max || valmax < min || valmax > max)
            return invalid_range_match;
        else
            return exact_match;
    }

    if ((p - str) > DECIMAL_STRLEN_MAX)
        return invalid_range_format_match;

    osal_memset(buf, 0, sizeof(buf));
    osal_strncpy (buf, str, p - str);

    if (strspn (buf, VALID_NUM_STR) != strlen (buf))
        return invalid_range_format_match;

    valmin = SYS_STR2UINT(buf);

    if (0 == osal_strlen(p + 1) || osal_strlen(p + 1) > DECIMAL_STRLEN_MAX)
        return invalid_range_format_match;

    if (strspn ((p + 1), VALID_NUM_STR) != strlen (p+1))
        return invalid_range_format_match;

    valmax = SYS_STR2UINT(p + 1);

    if (valmin > valmax)
        return invalid_range_format_match;

    if (valmin < min || valmin > max || valmax < min || valmax > max)
        return invalid_range_match;

    return exact_match;
}

static enum match_type
cmd_word_match (const char *range, const char *str)
{
    char buf[DECIMAL_STRLEN_MAX + 1];
    char *p = NULL;
    char *s = NULL;
    char *e = NULL;
    uint32 min;
    uint32 max;

    if (str == NULL)
        return partly_match;

    /* If parameter is not spcified range, do not check string length */
    if (0 == osal_strcmp(range, "WORD"))
        return exact_match;

    /* Get user input range min and max value */
    s = (char *)(range + osal_strlen("WORD<"));
    p = osal_strchr(s, '-');

    if (NULL == p)
        return no_match;

    if ((p - s) > DECIMAL_STRLEN_MAX)
        return bad_param_match;

    osal_memset(buf, 0, sizeof(buf));
    osal_strncpy (buf, s, (p - s));
    min = SYS_STR2UINT(buf);

    p = p + 1;
    e = osal_strchr(p, '>');

    if (NULL == e)
        return no_match;

    if (0 == (e - p) || ((e - p) > DECIMAL_STRLEN_MAX))
        return bad_param_match;

    osal_memset(buf, 0, sizeof(buf));
    osal_strncpy (buf, p, (e - p));
    max = SYS_STR2UINT(buf);

    if (min > max)
        return bad_param_match;

    if (osal_strlen(str) < min || osal_strlen(str) > max)
        return bad_param_match;

    return exact_match;
}

/* Make completion match and return match type flag. */
static enum match_type
cmd_filter_by_completion (vector vline, char *command, vector v, unsigned int index)
{
  unsigned int i;
  const char *str;
  struct cmd_element *cmd_element;
  enum match_type match_type;
  vector descvec;
  struct desc *desc;

  match_type = no_match;

  /* If command and cmd_element string does not match set NULL to vector */
  for (i = 0; i < vector_active (v); i++)
    if ((cmd_element = vector_slot (v, i)) != NULL)
      {
    if (index >= vector_active (cmd_element->strvec))
      vector_slot (v, i) = NULL;
    else
      {
        unsigned int j;
        int matched = 0;

        descvec = vector_slot (cmd_element->strvec, index);

        for (j = 0; j < vector_active (descvec); j++)
          if ((desc = vector_slot (descvec, j)))
        {
          str = desc->cmd;

          if (CMD_VARARG (str))
            {
              if (match_type < vararg_match)
            match_type = vararg_match;
              matched++;
            }
          else if (CMD_RANGE (str))
            {
              if (cmd_range_match (str, command))
            {
              if (match_type < range_match)
                match_type = range_match;

              matched++;
            }
            }
          else if (CMD_RANGE_FORMAT (str))
            {
              if (cmd_range_format_match (str, command))
            {
              if (match_type < range_format_match)
                match_type = range_format_match;
              matched++;
            }
            }

          else if (CMD_WORD (str))
            {
              if (cmd_word_match (str, command))
            {
              if (match_type < word_match)
                match_type = word_match;
              matched++;
            }
            }
#ifdef HAVE_IPV6
          else if (CMD_IPV6 (str))
            {
              if (cmd_ipv6_match (command))
            {
              if (match_type < ipv6_match)
                match_type = ipv6_match;

              matched++;
            }
            }
          else if (CMD_IPV6_PREFIX (str))
            {
              if (cmd_ipv6_prefix_match (command))
            {
              if (match_type < ipv6_prefix_match)
                match_type = ipv6_prefix_match;

              matched++;
            }
            }
            else if (CMD_IPV6_WMASK (str))
            {
              if (cmd_ipv6_mask_match (command))
            {
              if (match_type < ipv6_mask_match)
                match_type = ipv6_mask_match;

              matched++;
            }
            }
#endif /* HAVE_IPV6  */
          else if (CMD_IPV4 (str))
            {
              if (cmd_ipv4_match (command))
            {
              if (match_type < ipv4_match)
                match_type = ipv4_match;

              matched++;
            }
            }
            else if (CMD_IPV4_WMASK (str))
            {
                if (exact_match == cmd_ipv4_mask_match (command))
                {
                  if (match_type < ipv4_mask_match)
                    match_type = ipv4_mask_match;
                  matched++;
                }
                else
                {
                    match_type = invalid_ipmask_match;
                }
            }
          else if ( CMD_HOSTNAME(str) )
          {
              if (cmd_host_match(command))
              {
                  if (match_type < hostname_match) {
                      match_type = hostname_match;
                  }
              }
              else
              {
                  match_type = invalid_hostname_match;
              }
              matched++;
          }
          else if ( CMD_MAC_ADDR(str) )
          {
              if (cmd_mac_addr_match(command)) {
                  if (match_type < mac_addr_match) {
                      match_type = mac_addr_match;
                  }
                  matched++;
              }
          }
          else if ( CMD_MAC_ADDR_WMASK(str) )
          {
              if (exact_match == cmd_mac_addr_mask_match(command))
              {
                  if (match_type < mac_addr_mask_match)
                      match_type = mac_addr_mask_match;
                  matched++;
              }
              else
              {
                    match_type = invalid_macmask_match;
              }
          }
          else if ( CMD_OUI(str) )
          {
              if (cmd_mac_oui_match(command)) {
                  if (match_type < mac_oui_match) {
                      match_type = mac_oui_match;
                  }
                  matched++;
              }
          }
          else if ( CMD_IF_SINGLE(str))
          {
              if (cmd_if_match(command, FALSE, (char *) str)) {
                  if (match_type < intf_single_match) {
                      match_type = intf_single_match;
                  }
                  matched++;
              }
          }
          else if ( CMD_IF_RANGE(str))
          {
              if (cmd_if_match(command, TRUE, (char *) str)) {
                  if (match_type < intf_range_match) {
                      match_type = intf_range_match;
                  }
                  matched++;
              }
          }
          else if ( CMD_PORT(str) )
          {
              if (cmd_port_match(command))
              {
                  if (match_type < port_match)
                      match_type = port_match;

                  matched++;
              }
          }
          else if ( CMD_PORT_LIST(str) )
          {
              if (cmd_port_list_match(command))
              {
                  if (match_type < port_list_match)
                      match_type = port_list_match;

                  matched++;
              }
          }
          else if ( CMD_NORMAL_WOTRKMBR_PORT_LIST(str) )
          {
              if (cmd_normal_wotrkmbr_port_list_match(command))
              {
                  if (match_type < normal_wotrkmbr_port_list_match)
                      match_type = normal_wotrkmbr_port_list_match;

                  matched++;
              }
          }
          else if ( CMD_NORMAL_PORT_LIST(str) )
          {
              if (cmd_normal_port_list_match(command))
              {
                  if (match_type < normal_port_list_match)
                      match_type = normal_port_list_match;

                  matched++;
              }
          }
          else if ( CMD_VLAN_LIST(str) )
          {
              if (cmd_vlan_list_match(command))
              {
                  if (match_type < vlan_list_match)
                      match_type = vlan_list_match;

                  matched++;
              }
          }
          else if ( CMD_QUEUE_LIST(str) )
          {
              if (cmd_value_list_match(command, 0, 7))
              {
                  if (match_type < queue_list_match)
                      match_type = queue_list_match;

                  matched++;
              }
          }
          else if ( CMD_DSCP_LIST(str) )
          {
              if (cmd_value_list_match(command, 0, 63))
              {
                  if (match_type < dscp_list_match)
                      match_type = dscp_list_match;

                  matched++;
              }
          }
          else if ( CMD_FS(str) )
          {
              if (match_type < fs_match)
                  match_type = fs_match;

              matched++;
          }
          else if ( CMD_VALUE(str) )
          {
              if (cmd_value_match(command))
              {
                  if (match_type < invalid_value_match)
                      match_type = invalid_value_match;

                  matched++;
              }
          }
          else if (CMD_IPV4_PREFIX (str))
            {
              if (cmd_ipv4_prefix_match (command))
            {
              if (match_type < ipv4_prefix_match)
                match_type = ipv4_prefix_match;
              matched++;
            }
            }
          else
            /* Check is this point's argument optional ? */
          if (CMD_OPTION (str) || CMD_VARIABLE (str))
            {
              if (match_type < extend_match)
            match_type = extend_match;
              matched++;
            }
          else if (strncmp (command, str, strlen (command)) == 0)
            {
              if (strcmp (command, str) == 0)
            match_type = exact_match;
              else
            {
              if (match_type < partly_match)
                match_type = partly_match;
            }
              matched++;
            }
        }
        if (!matched)
          vector_slot (v, i) = NULL;
      }
      }
  return match_type;
}

#if 0
/* Filter vector by command character with index. */
static enum match_type
cmd_filter_by_string (char *command, vector v, unsigned int index)
{
  unsigned int i;
  const char *str;
  struct cmd_element *cmd_element;
  enum match_type match_type;
  vector descvec;
  struct desc *desc;

  match_type = no_match;

  /* If command and cmd_element string does not match set NULL to vector */
  for (i = 0; i < vector_active (v); i++)
    if ((cmd_element = vector_slot (v, i)) != NULL)
      {
    /* If given index is bigger than max string vector of command,
       set NULL */
    if (index >= vector_active (cmd_element->strvec))
      vector_slot (v, i) = NULL;
    else
      {
        unsigned int j;
        int matched = 0;

        descvec = vector_slot (cmd_element->strvec, index);

        for (j = 0; j < vector_active (descvec); j++)
          if ((desc = vector_slot (descvec, j)))
        {
          str = desc->cmd;

          if (CMD_VARARG (str))
            {
              if (match_type < vararg_match)
            match_type = vararg_match;
              matched++;
            }
          else if (CMD_RANGE (str))
            {
              if (cmd_range_match (str, command))
            {
              if (match_type < range_match)
                match_type = range_match;
              matched++;
            }
            }
#ifdef HAVE_IPV6
          else if (CMD_IPV6 (str))
            {
              if (cmd_ipv6_match (command) == exact_match)
            {
              if (match_type < ipv6_match)
                match_type = ipv6_match;
              matched++;
            }
            }
          else if (CMD_IPV6_PREFIX (str))
            {
              if (cmd_ipv6_prefix_match (command) == exact_match)
            {
              if (match_type < ipv6_prefix_match)
                match_type = ipv6_prefix_match;
              matched++;
            }
            }
#endif /* HAVE_IPV6  */
          else if (CMD_IPV4 (str))
            {
              if (cmd_ipv4_match (command) == exact_match)
            {
              if (match_type < ipv4_match)
                match_type = ipv4_match;
              matched++;
            }
            }
#if 0
          else if ( CMD_MAC_ADDR(str) ) {
              if (cmd_mac_addr_match(command) == exact_match) {
                  if (match_type < mac_addr_match) {
                      match_type = mac_addr_match;
                  }
                  matched++;
              }
          }
          else if ( CMD_PORT_LIST(str) ) {
              if (cmd_port_list_match(command) == exact_match) {
                  if (match_type < port_list_match) {
                      match_type = port_list_match;
                  }
                  matched++;
              }
          }
#endif
          else if (CMD_IPV4_PREFIX (str))
            {
              if (cmd_ipv4_prefix_match (command) == exact_match)
            {
              if (match_type < ipv4_prefix_match)
                match_type = ipv4_prefix_match;
              matched++;
            }
            }
          else if (CMD_OPTION (str) || CMD_VARIABLE (str))
            {
              if (match_type < extend_match)
            match_type = extend_match;
              matched++;
            }
          else
            {
              if (strcmp (command, str) == 0)
            {
              match_type = exact_match;
              matched++;
            }
            }
        }
        if (!matched)
          vector_slot (v, i) = NULL;
      }
      }
  return match_type;
}
#endif

/* Check ambiguous match */
static int
is_cmd_ambiguous (char *command, vector v, int index, enum match_type type)
{
  unsigned int i;
  unsigned int j;
  const char *str = NULL;
  struct cmd_element *cmd_element;
  const char *matched = NULL;
  vector descvec;
  struct desc *desc;

  for (i = 0; i < vector_active (v); i++)
    if ((cmd_element = vector_slot (v, i)) != NULL)
      {
    int match = 0;

    descvec = vector_slot (cmd_element->strvec, index);

    for (j = 0; j < vector_active (descvec); j++)
      if ((desc = vector_slot (descvec, j)))
        {
          enum match_type ret;

          str = desc->cmd;

          switch (type)
        {
        case exact_match:
          if (!(CMD_OPTION (str) || CMD_VARIABLE (str))
              && strcmp (command, str) == 0)
            match++;
          break;
        case partly_match:
          if (!(CMD_OPTION (str) || CMD_VARIABLE (str))
              && strncmp (command, str, strlen (command)) == 0)
            {
              if (matched && strcmp (matched, str) != 0)
            return CMD_ERR_AMBIGUOUS;    /* There is ambiguous match. */
              else
            matched = str;
              match++;
            }
          break;
        case range_match:
          if ((ret = cmd_range_match (str, command)) != no_match)
            {
              if (matched && 0 == strcmp(matched, "<0-0>") && 1 == strlen(command) && '0' == command[0])
              {
                break;
              }

              if (ret == bad_param_match)
                  return CMD_ERR_BAD_PARAM;
              else if (matched && strcmp (matched, str) != 0 && strcmp (matched, "<0-0>") != 0)
                  return CMD_ERR_AMBIGUOUS;
              else
                  matched = str;
              match++;
            }
          break;

        case range_format_match:
          if ((ret = cmd_range_format_match (str, command)) != no_match)
          {
              if (ret == partly_match)
                  return CMD_ERR_NO_MATCH;    /* There is incomplete match. */
              else if (ret == invalid_range_format_match)
                  return SYS_ERR_CMD_RANGE_FORMAT;
              else if (ret == invalid_range_match)
                  return SYS_ERR_CMD_RANGE;

              match++;
          }
          break;

        case word_match:
          if ((ret = cmd_word_match (str, command)) != no_match)
          {
              if (ret == partly_match)
                  return CMD_ERR_NO_MATCH;    /* There is incomplete match. */
              else if (ret == bad_param_match)
                  return SYS_ERR_INPUT;

              match++;
          }
          break;

        case fs_match:
          if ((CMD_FS_FLASH(str) && CMD_FS_FLASH(command))
              || (CMD_FS_TFTP(str) && CMD_FS_TFTP(command))
              || (CMD_FS_USB(str) && CMD_FS_USB(command)))
          {
            match++;
          }
          break;

#ifdef HAVE_IPV6
        case ipv6_match:
          if (CMD_IPV6 (str))
            match++;
          break;

        case ipv6_mask_match:
          if (CMD_IPV6_WMASK(str))
            match++;
          break;

        case ipv6_prefix_match:
          if ((ret = cmd_ipv6_prefix_match (command)) != no_match)
            {
              if (ret == partly_match)
            return CMD_ERR_NO_MATCH;    /* There is incomplete match. */

              match++;
            }
          break;
#endif /* HAVE_IPV6 */
        case ipv4_match:
          if (CMD_IPV4 (str))
            match++;
          break;
        case ipv4_mask_match:
          if (CMD_IPV4_WMASK(str))
            match++;
          break;

        case hostname_match:
          if (CMD_HOSTNAME (str))
            match++;
          break;

        case invalid_hostname_match:
            return SYS_ERR_HOSTNAME;
            break;

        case mac_addr_match:
            if ((ret = cmd_mac_addr_match (command)) != no_match)
            {
              if (ret == partly_match) {
                  return CMD_ERR_NO_MATCH; /* incomplete match */
              }
              match++;
            }
          break;

         case mac_addr_mask_match:
            if ((ret = cmd_mac_addr_mask_match (command)) != no_match)
            {
              if (ret == partly_match) {
                  return CMD_ERR_NO_MATCH; /* incomplete match */
              }
              match++;
            }
          break;

        case mac_oui_match:
            if ((ret = cmd_mac_oui_match (command)) != no_match)
            {
              if (ret == partly_match) {
                  return CMD_ERR_NO_MATCH; /* incomplete match */
              }
              match++;
            }
          break;

        case port_match:
          if ((ret = cmd_port_match (command)) != no_match)
          {
            if (ret == partly_match) {
                return CMD_ERR_NO_MATCH; /* incomplete match */
            }
            else if (ret == invalid_port_match) {
                return CMD_ERR_PORTID;
            }
            match++;
          }
          break;

        case port_list_match:
          if ((ret = cmd_port_list_match (command)) != no_match)
          {
            if (ret == partly_match) {
                return CMD_ERR_NO_MATCH; /* incomplete match */
            }
            else if (ret == trunk_mbr_match) {
                return CMD_ERR_TRUNK_MBR; /* trunk member port match */
            }
            else if (ret == invalid_portlist_match) {
                return CMD_ERR_PORTLIST;
            }
            match++;
          }
          break;

        case normal_wotrkmbr_port_list_match:
          if ((ret = cmd_normal_wotrkmbr_port_list_match (command)) != no_match)
          {
            if (ret == partly_match) {
                return CMD_ERR_NO_MATCH; /* incomplete match */
            }
            else if (ret == trunk_mbr_match) {
                return CMD_ERR_TRUNK_MBR; /* trunk member port match */
            }
            else if (ret == invalid_portlist_match)
            {
                return CMD_ERR_PORTLIST;
            }

            match++;
          }
          break;
        case normal_port_list_match:
          if ((ret = cmd_normal_port_list_match (command)) != no_match)
          {
            if (ret == partly_match) {
                return CMD_ERR_NO_MATCH; /* incomplete match */
            }
            else if (ret == trunk_mbr_match) {
                return CMD_ERR_TRUNK_MBR; /* trunk member port match */
            }
            else if (ret == invalid_portlist_match)
            {
                return CMD_ERR_PORTLIST;
            }

            match++;
          }
          break;

        case intf_single_match:
          if ((ret = cmd_if_match (command, FALSE, NULL)) != no_match)
          {
            if (ret == partly_match) {
                return CMD_ERR_NO_MATCH; /* incomplete match */
            }
            else if (ret == invalid_port_match)
            {
                return CMD_ERR_PORTID;
            }
#ifdef CONFIG_SYS_L3_ROUTE
            else if (ret == invalid_vlan_match)
            {
                return SYS_ERR_VLAN_ID;
            }
            else if (ret == invalid_lo_match)
            {
                return SYS_ERR_L3_INTF_LO_ID_INVALID;
            }
#endif

            match++;
          }
          break;

        case intf_range_match:
          if ((ret = cmd_if_match (command, TRUE, NULL)) != no_match)
          {
            if (ret == partly_match) {
                return CMD_ERR_NO_MATCH; /* incomplete match */
            }
            else if (ret == invalid_port_match)
            {
                return CMD_ERR_PORTID;
            }
#ifdef CONFIG_SYS_L3_ROUTE
            else if (ret == invalid_vlan_match)
            {
                return SYS_ERR_VLAN_ID;
            }
            else if (ret == invalid_lo_match)
            {
                return SYS_ERR_L3_INTF_LO_ID_INVALID;
            }
#endif

            match++;
          }
          break;

        case vlan_list_match:
          if ((ret = cmd_vlan_list_match (command)) != no_match)
          {
            if (ret == partly_match) {
                return CMD_ERR_NO_MATCH; /* incomplete match */
            }
            else if (ret == invalid_vlan_match)
            {
                return SYS_ERR_VLAN_ID;
            }
            match++;
          }
          break;

        case queue_list_match:
          if ((ret = cmd_value_list_match (command, 0, 7)) != no_match)
          {
            if (ret == partly_match) {
                return CMD_ERR_NO_MATCH; /* incomplete match */
            }
            match++;
          }
          break;

        case dscp_list_match:
          if ((ret = cmd_value_list_match (command, 0, 63)) != no_match)
          {
            if (ret == partly_match) {
                return CMD_ERR_NO_MATCH; /* incomplete match */
            }
            match++;
          }
          break;

        case invalid_value_match:
          if ((ret = cmd_value_match (command)) != no_match)
          {
            if (ret == invalid_value_match)
            {
                return CMD_ERR_VALUE;
            }

            match++;
          }
          break;

        case ipv4_prefix_match:
          if ((ret = cmd_ipv4_prefix_match (command)) != no_match)
            {
              if (ret == partly_match)
            return CMD_ERR_NO_MATCH;    /* There is incomplete match. */

              match++;
            }
          break;

        case extend_match:
          if (CMD_OPTION (str) || CMD_VARIABLE (str))
            match++;
          break;
        case no_match:
        default:
          break;
        }
        }
    if (!match)
      vector_slot (v, i) = NULL;
      }
  return SYS_ERR_OK;
}

/* If src matches dst return dst string, otherwise return NULL */
static const char *
cmd_entry_function (const char *src, const char *dst)
{
  /* Allow complete interface command */
  if (CMD_IF(dst))
  {
    char tmpSrc[SYS_BUF32_LEN];

    SYS_MEM_CLEAR(tmpSrc);

    if (src == NULL)
    {
        if (CMD_IF_FA_PORTS(dst) || CMD_IF_FA_PORT(dst))
            return IF_FA_STR;
        else if (CMD_IF_GI_PORTS(dst) || CMD_IF_GI_PORT(dst))
            return IF_GI_STR;
        else if (CMD_IF_MGI_PORTS(dst) || CMD_IF_MGI_PORT(dst))
            return IF_MGI_STR;
        else if (CMD_IF_TE_PORTS(dst) || CMD_IF_TE_PORT(dst))
            return IF_TE_STR;
        else if (CMD_IF_PO_PORTS(dst) || CMD_IF_PO_PORT(dst))
            return IF_PO_STR;
#ifdef CONFIG_SYS_L3_ROUTE
        else if (CMD_IF_VLANS(dst) || CMD_IF_VLAN(dst))
            return IF_VLAN_STR;
        else if (CMD_IF_LOS(dst) || CMD_IF_LO(dst))
            return IF_LO_STR;
#endif
    }
    else
    {
        osal_strcpy(tmpSrc, src);

        sal_util_str2lowerStr_xlate(tmpSrc);

        if ((CMD_IF_FA_PORTS(dst) || CMD_IF_FA_PORT(dst))
            && (osal_strncmp (tmpSrc, IF_FA_LOWSTR, strlen (src)) == 0))
            return IF_FA_STR;
        else if ((CMD_IF_GI_PORTS(dst) || CMD_IF_GI_PORT(dst))
                  && (osal_strncmp (tmpSrc, IF_GI_LOWSTR, strlen (src)) == 0))
            return IF_GI_STR;
        else if ((CMD_IF_MGI_PORTS(dst) || CMD_IF_MGI_PORT(dst))
                  && (osal_strncmp (tmpSrc, IF_MGI_LOWSTR, strlen (src)) == 0))
            return IF_MGI_STR;
        else if ((CMD_IF_TE_PORTS(dst) || CMD_IF_TE_PORT(dst))
                  && (osal_strncmp (tmpSrc, IF_TE_LOWSTR, strlen (src)) == 0))
            return IF_TE_STR;
        else if ((CMD_IF_PO_PORTS(dst) || CMD_IF_PO_PORT(dst))
                  && (osal_strncmp (tmpSrc, IF_PO_LOWSTR, strlen (src)) == 0))
            return IF_PO_STR;
#ifdef CONFIG_SYS_L3_ROUTE
        else if ((CMD_IF_VLANS(dst) || CMD_IF_VLAN(dst))
                  && (osal_strncmp (tmpSrc, IF_VLAN_LOWSTR, strlen (src)) == 0))
            return IF_VLAN_STR;
        else if ((CMD_IF_LOS(dst) || CMD_IF_LO(dst))
                  && (osal_strncmp (tmpSrc, IF_LO_LOWSTR, strlen (src)) == 0))
            return IF_LO_STR;
#endif
    }

    return NULL;
  }
  else if (CMD_FS(dst))
  {

  }

  /* Skip variable arguments. */
  if (CMD_OPTION (dst) || CMD_VARIABLE (dst) || CMD_VARARG (dst) ||
      CMD_IPV4 (dst) || CMD_IPV4_PREFIX (dst) || CMD_IPV4_WMASK (dst) ||
#ifdef HAVE_IPV6
      CMD_IPV6 (dst) || CMD_IPV6_PREFIX (dst) || CMD_IPV6_WMASK (dst) ||
#endif
      CMD_RANGE (dst) || CMD_MAC_ADDR(dst) || CMD_MAC_ADDR_WMASK(dst) || CMD_OUI(dst) ||
      CMD_PORT_LIST(dst) || CMD_NORMAL_WOTRKMBR_PORT_LIST(dst) ||
      CMD_VALUE(dst) || CMD_RANGE_FORMAT(dst) || CMD_WORD(dst) )
    return NULL;

  /* In case of 'command \t', given src is NULL string. */
  if (src == NULL)
    return dst;

  /* Matched with input string. */
  if (strncmp (src, dst, strlen (src)) == 0)
    return dst;

  return NULL;
}

/* If src matches dst return dst string, otherwise return NULL */
/* This version will return the dst string always if it is
   CMD_VARIABLE for '?' key processing */
static const char *
cmd_entry_function_desc (const char *src, const char *dst)
{
  if (CMD_VARARG (dst))
    return dst;

  if (CMD_RANGE (dst))
    {
      if (cmd_range_match (dst, src))
    return dst;
      else
    return NULL;
    }

  if (CMD_RANGE_FORMAT (dst))
    {
      if (cmd_range_format_match (dst, src))
    return dst;
      else
    return NULL;
    }

  if (CMD_WORD (dst))
    {
      if (cmd_word_match (dst, src))
    return dst;
      else
    return NULL;
    }

#ifdef HAVE_IPV6
  if (CMD_IPV6 (dst))
    {
      if (cmd_ipv6_match (src))
    return dst;
      else
    return NULL;
    }

      if (CMD_IPV6_WMASK(dst))
    {
      if (cmd_ipv6_mask_match (src))
    return dst;
      else
    return NULL;
    }

  if (CMD_IPV6_PREFIX (dst))
    {
      if (cmd_ipv6_prefix_match (src))
    return dst;
      else
    return NULL;
    }
#endif /* HAVE_IPV6 */

  if (CMD_IPV4 (dst))
    {
      if (cmd_ipv4_match (src))
    return dst;
      else
    return NULL;
    }

  if (CMD_IPV4_WMASK(dst))
    {
      if (cmd_ipv4_mask_match (src))
    return dst;
      else
    return NULL;
    }

  if (CMD_MAC_ADDR(dst)) {
      if ( cmd_mac_addr_match(src) ) {
          return dst;
      }
      else {
          return NULL;
      }
  }

    if (CMD_MAC_ADDR_WMASK(dst)) {
      if ( cmd_mac_addr_mask_match(src) ) {
          return dst;
      }
      else {
          return NULL;
      }
  }

  if (CMD_OUI(dst))
  {
      if ( cmd_mac_oui_match(src) )
          return dst;
      else
          return NULL;
  }

  if (CMD_PORT_LIST(dst))
  {
      if ( cmd_port_list_match(src) )
          return dst;
      else
          return NULL;
  }

  if (CMD_NORMAL_WOTRKMBR_PORT_LIST(dst))
  {
      if ( cmd_normal_wotrkmbr_port_list_match(src) )
          return dst;
      else
          return NULL;
  }

  if (CMD_NORMAL_PORT_LIST(dst))
  {
      if ( cmd_normal_port_list_match(src) )
          return dst;
      else
          return NULL;
  }

  if (CMD_VLAN_LIST(dst))
  {
      if ( cmd_vlan_list_match(src) )
          return dst;
      else
          return NULL;
  }

  if (CMD_QUEUE_LIST(dst))
  {
      if ( cmd_value_list_match(src, 0, 7) )
          return dst;
      else
          return NULL;
  }

  if (CMD_DSCP_LIST(dst))
  {
      if ( cmd_value_list_match(src, 0, 63) )
          return dst;
      else
          return NULL;
  }

  if (CMD_VALUE(dst))
  {
      if ( cmd_value_match(src) )
          return dst;
      else
          return NULL;
  }

  if (CMD_IPV4_PREFIX (dst))
    {
      if (cmd_ipv4_prefix_match (src))
    return dst;
      else
    return NULL;
    }

  if (CMD_IF(dst))
  {
      int32 ret;
      char tmpSrc[SYS_BUF128_LEN];
      char tmpDst[SYS_BUF128_LEN];

      if (NULL == dst)
          return NULL;

      if (NULL == src)
      {
          if (CMD_IF_FA_PORTS(dst) || CMD_IF_FA_PORT(dst))
              return IF_FA_STR;
          else if (CMD_IF_GI_PORTS(dst) || CMD_IF_GI_PORT(dst))
              return IF_GI_STR;
          else if (CMD_IF_MGI_PORTS(dst) || CMD_IF_MGI_PORT(dst))
              return IF_MGI_STR;
          else if (CMD_IF_TE_PORTS(dst) || CMD_IF_TE_PORT(dst))
              return IF_TE_STR;
          else if (CMD_IF_PO_PORTS(dst) || CMD_IF_PO_PORT(dst))
              return IF_PO_STR;
#ifdef CONFIG_SYS_L3_ROUTE
          else if (CMD_IF_VLANS(dst) || CMD_IF_VLAN(dst))
              return IF_VLAN_STR;
          else if (CMD_IF_LOS(dst) || CMD_IF_LO(dst))
              return IF_LO_STR;
#endif
      }

      if (CMD_IF_RANGE(dst))
          ret = cmd_if_match(src, TRUE, NULL);
      else
          ret = cmd_if_match(src, FALSE, NULL);

      SYS_MEM_CLEAR(tmpSrc);
      SYS_MEM_CLEAR(tmpDst);

      osal_strcpy(tmpSrc, src);
      if (CMD_IF_FA_PORTS(dst) || CMD_IF_FA_PORT(dst))
          osal_strcpy(tmpDst, IF_FA_STR);
      else if (CMD_IF_GI_PORTS(dst) || CMD_IF_GI_PORT(dst))
          osal_strcpy(tmpDst, IF_GI_STR);
      else if (CMD_IF_MGI_PORTS(dst) || CMD_IF_MGI_PORT(dst))
          osal_strcpy(tmpDst, IF_MGI_STR);
      else if (CMD_IF_TE_PORTS(dst) || CMD_IF_TE_PORT(dst))
          osal_strcpy(tmpDst, IF_TE_STR);
      else if (CMD_IF_PO_PORTS(dst) || CMD_IF_PO_PORT(dst))
          osal_strcpy(tmpDst, IF_PO_STR);
#ifdef CONFIG_SYS_L3_ROUTE
      else if (CMD_IF_VLAN(dst) || CMD_IF_VLANS(dst))
          osal_strcpy(tmpDst, IF_VLAN_STR);
      else if (CMD_IF_LO(dst) || CMD_IF_LOS(dst))
          osal_strcpy(tmpDst, IF_LO_STR);
#endif

      sal_util_str2lowerStr_xlate(tmpSrc);
      sal_util_str2lowerStr_xlate(tmpDst);

      if (invalid_port_match == ret
#ifdef CONFIG_SYS_L3_ROUTE
          || invalid_vlan_match == ret
          || invalid_lo_match == ret
#endif
         )
      {
          SYS_MEM_CLEAR(tmpSrc);
          osal_strcpy(tmpSrc, src);
          sal_util_str2lowerStr_xlate(tmpSrc);

          if (0 == osal_strncmp(tmpSrc, tmpDst, osal_strlen(tmpSrc)))
              return dst;

          return NULL;
      }
      else if (ret)
      {
          if (tmpSrc[0] == tmpDst[0])
              return dst;
          else
              return NULL;
      }
  }


  /* Optional or variable commands always match on '?' */
  if (CMD_OPTION (dst) || CMD_VARIABLE (dst) || CMD_FS(dst))
    return dst;

  /* In case of 'command \t', given src is NULL string. */
  if (src == NULL)
    return dst;

  if (strncmp (src, dst, strlen (src)) == 0)
    return dst;
  else
    return NULL;
}

/* Check same string element existence.  If it isn't there return
    1. */
static int
cmd_unique_string (vector v, const char *str)
{
  unsigned int i;
  char *match;

  for (i = 0; i < vector_active (v); i++)
    if ((match = vector_slot (v, i)) != NULL)
      if (strcmp (match, str) == 0)
    return 0;
  return 1;
}

/* Compare string to description vector.  If there is same string
   return 1 else return 0. */
static int
desc_unique_string (vector v, const char *str)
{
  unsigned int i;
  struct desc *desc;

  for (i = 0; i < vector_active (v); i++)
    if ((desc = vector_slot (v, i)) != NULL)
      if (strcmp (desc->cmd, str) == 0)
    return 1;
  return 0;
}

int
cmd_try_do_shortcut (enum node_type node, const char* first_word) {
  if ( first_word != NULL &&
       node != VIEW_NODE &&
       node != ENABLE_NODE &&
       node != DEBUG_NODE &&
       osal_strlen(first_word) > 3 &&
       0 == strncmp( "do ", first_word , 3) )
    return 1;
  return 0;
}

/* For interface mode description */
struct desc *_intf_devnum_string(vector vline, vector strvec, uint32 index)
{
    uint32 i = 0;
    uint32 j = 0;
    char *tmp_intf = NULL;
    char tmpSrc[SYS_BUF128_LEN];
    char tmpDst[SYS_BUF128_LEN];
    struct desc *desc = NULL;
    vector descvec = NULL;

    if (index < 1)
        return NULL;

    tmp_intf = vector_slot(vline, index);

    if (NULL == tmp_intf)
        return NULL;

    if (0 == osal_strlen(tmp_intf))
        return NULL;

    SYS_MEM_CLEAR(tmpSrc);

    /* Remove ' ' char */
    i = 0;
    for (j = 0; j < osal_strlen(tmp_intf); j++)
    {
        if (' ' == tmp_intf[j])
            continue;

        tmpSrc[i] = tolower(tmp_intf[j]);
        i++;
    }

    if (NULL != strvec)
    {
        descvec = vector_slot(strvec, index);
        desc = vector_slot(descvec, 0);
    }
    
    if (0 == osal_strncmp(desc->cmd, "WORD", osal_strlen("WORD")))
    {
        return NULL;
    }

    SYS_MEM_CLEAR(tmpDst);
    osal_strcpy(tmpDst, IF_FA_STR);
    sal_util_str2lowerStr_xlate(tmpDst);
    if (0 == osal_strncmp(tmpSrc, tmpDst, osal_strlen(tmpSrc)))
    {
        return &intf_fa_devnum;
    }

    SYS_MEM_CLEAR(tmpDst);
    osal_strcpy(tmpDst, IF_GI_STR);
    sal_util_str2lowerStr_xlate(tmpDst);
    if (0 == osal_strncmp(tmpSrc, tmpDst, osal_strlen(tmpSrc)))
    {
        return &intf_gi_devnum;
    }

    SYS_MEM_CLEAR(tmpDst);
    osal_strcpy(tmpDst, IF_MGI_STR);
    sal_util_str2lowerStr_xlate(tmpDst);
    if (0 == osal_strncmp(tmpSrc, tmpDst, osal_strlen(tmpSrc)))
    {
        return &intf_mgi_devnum;
    }

    SYS_MEM_CLEAR(tmpDst);
    osal_strcpy(tmpDst, IF_TE_STR);
    sal_util_str2lowerStr_xlate(tmpDst);
    if (0 == osal_strncmp(tmpSrc, tmpDst, osal_strlen(tmpSrc)))
    {
        return &intf_te_devnum;
    }

    SYS_MEM_CLEAR(tmpDst);
    osal_strcpy(tmpDst, IF_PO_STR);
    sal_util_str2lowerStr_xlate(tmpDst);
    if (0 == osal_strncmp(tmpSrc, tmpDst, osal_strlen(tmpSrc))
            && (CMD_IF_PORTS(desc->cmd) || CMD_IF_PORT(desc->cmd)))
    {
        return &intf_po_devnum;
    }

#ifdef CONFIG_SYS_L3_ROUTE
    SYS_MEM_CLEAR(tmpDst);
    osal_strcpy(tmpDst, IF_VLAN_STR);
    sal_util_str2lowerStr_xlate(tmpDst);
    if (0 == osal_strncmp(tmpSrc, tmpDst, osal_strlen(tmpSrc)))
    {
        if (index > 0)
        {
            if (1 == index)
            {
                tmp_intf = vector_slot(vline, index - 1);

                if (0 == osal_strncmp(tmp_intf, "interface", osal_strlen(tmp_intf)))
                    return &intf_vlan_devnum;
            }
            else if (2 == index)
            {
                tmp_intf = vector_slot(vline, index - 2);

                if (0 == osal_strncmp(tmp_intf, "interface", osal_strlen(tmp_intf)))
                {
                    tmp_intf = vector_slot(vline, index - 1);

                    if (0 == osal_strncmp(tmp_intf, "range", osal_strlen(tmp_intf)))
                        return &intf_vlan_devnum;
                }
            }
            else if (3 == index)
            {
            	/*show ip interface vlan <1-4064>*/
            	tmp_intf = vector_slot(vline, index - 2);
            	if (0 == osal_strncmp(tmp_intf, "ip", osal_strlen(tmp_intf)))
            	{
					tmp_intf = vector_slot(vline, index - 1);

                	if (0 == osal_strncmp(tmp_intf, "interface", osal_strlen(tmp_intf)))
						return &intf_vlan_devnum;
            	}
            }
        }
    }

    SYS_MEM_CLEAR(tmpDst);
    osal_strcpy(tmpDst, IF_LO_STR);
    sal_util_str2lowerStr_xlate(tmpDst);
    if (0 == osal_strncmp(tmpSrc, tmpDst, osal_strlen(tmpSrc)))
    {
        return &intf_lo_devnum;
    }
#endif

    return NULL;
}

/* '?' describe command support. */
static vector
cmd_describe_command_real (vector vline, struct vty *vty, int *status)
{
#define INIT_MATCHVEC_SIZE 10

    unsigned int i;
    vector cmd_vector;
    vector matchvec;
    struct cmd_element *cmd_element;
    unsigned int index;
    int ret;
    enum match_type match = no_match;
    char *command;
    int needsort = TRUE;

    /* Set index. */
    if (vector_active (vline) == 0)
    {
        *status = CMD_ERR_NO_MATCH;
        return NULL;
    }
    else
    {
        index = vector_active (vline) - 1;
    }

    /* Make copy vector of current node's command vector. */
    cmd_vector = vector_copy (cmd_node_vector (cmdvec, vty->node));

    /* Prepare match vector */
    matchvec = vector_init (INIT_MATCHVEC_SIZE);

    /* Filter commands. */
    /* Only words precedes current word will be checked in this loop. */
    for (i = 0; i < index; i++)
    {
        if ((command = vector_slot (vline, i)))
        {
            match = cmd_filter_by_completion (vline, command, cmd_vector, i);

            if (match == vararg_match)
            {
                struct cmd_element *cmd_element;
                vector descvec;
                unsigned int j, k;

                for (j = 0; j < vector_active (cmd_vector); j++)
                {
                    if ((cmd_element = vector_slot (cmd_vector, j)) != NULL
                            && (vector_active (cmd_element->strvec)))
                    {
                        descvec = vector_slot (cmd_element->strvec, vector_active (cmd_element->strvec) - 1);

                        for (k = 0; k < vector_active (descvec); k++)
                        {
                            struct desc *desc = vector_slot (descvec, k);
                            vector_set (matchvec, desc);
                        }
                    }
                }

                vector_set (matchvec, &desc_cr);
                vector_free (cmd_vector);

                return matchvec;
            }

            if (SYS_ERR_OK != (ret = is_cmd_ambiguous (command, cmd_vector, i, match))
                    && SYS_ERR_CMD_INCOMPLETE != ret)
            {
                *status = ret;

                vector_free (cmd_vector);
                vector_free (matchvec);
                return NULL;
            }
        }
    }

    /* Prepare match vector */
    /*  matchvec = vector_init (INIT_MATCHVEC_SIZE); */

    /* Make sure that cmd_vector is filtered based on current word */
    command = vector_slot (vline, index);
    if (command)
        match = cmd_filter_by_completion (vline, command, cmd_vector, index);

    /* Make description vector. */
    for (i = 0; i < vector_active (cmd_vector); i++)
    {
        if ((cmd_element = vector_slot (cmd_vector, i)) != NULL)
        {
            vector strvec = cmd_element->strvec;

            /* Skip hidden command */
            if (cmd_element->attr == CMD_ATTR_CONDITION_HIDDEN)
            {
                if (NULL != cmd_element->hidmatch && NULL != vty)
                {
                    if (TRUE == cmd_element->hidmatch(cmd_element, vty))
                        continue;
                }
                else
                {
                    continue;
                }
            }

            /* if command is NULL, index may be equal to vector_active */
            if (command && index >= vector_active (strvec))
            {
                vector_slot (cmd_vector, i) = NULL;
            }
            else
            {
                /* Check if command is completed. */
                if (command == NULL && index == vector_active (strvec))
                {
                    struct desc *desc;
#if 0
                    {
                        uint32 j = 0;
                        vector descvec_tmp;
                        SYS_PRINTF("%s:%d active=%d index=%d\n", __func__, __LINE__, vector_active(strvec), index);
                        for (j = 0; j < vector_active(strvec); j++)
                        {
                            descvec_tmp = vector_slot (strvec, j);

                            desc = vector_slot (descvec_tmp, 0);

                            SYS_PRINTF("%s ", desc->cmd);
                        }
                        SYS_PRINTF("\n");
                        SYS_PRINTF("%s:%d\n", __func__, __LINE__);
                    }
#endif

                    /* For interface mode description */
                    if ((index > 0) && NULL != (desc = _intf_devnum_string(vline, strvec, index - 1)))
                    {
                        vector_set (matchvec, desc);
                        break;
                    }
                    else if (!desc_unique_string (matchvec, command_cr))
                    {
                        vector_set (matchvec, &desc_cr);
                    }
                }
                else
                {
                    unsigned int j;
                    vector descvec = vector_slot (strvec, index);
                    struct desc *desc = NULL;
#if 0
                    {
                        vector descvec_tmp;
                        SYS_PRINTF("%s:%d active=%d index=%d\n", __func__, __LINE__, vector_active(strvec), index);
                        for (j = 0; j < vector_active(strvec); j++)
                        {
                            descvec_tmp = vector_slot (strvec, j);

                            desc = vector_slot (descvec_tmp, 0);

                            SYS_PRINTF("%s ", desc->cmd);
                        }
                        SYS_PRINTF("\n");
                        //SYS_PRINTF("%s:%d\n", __func__, __LINE__);
                    }
#endif

                    /* For interface mode description */
                    if ((index > 0) && NULL != (desc = _intf_devnum_string(vline, strvec, index - 1)))
                    {
                        vector_set (matchvec, desc);

                        break;
                    }
                    else
                    {
                        for (j = 0; j < vector_active (descvec); j++)
                        {
                            if ((desc = vector_slot (descvec, j)))
                            {
                                const char *string;

                                string = cmd_entry_function_desc (command, desc->cmd);

                                if (string)
                                {
                                    /* Uniqueness check */
                                #ifdef CONFIG_SYS_AAA
                                  #ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
                                    if (!desc_unique_string (matchvec, string) && cmd_element->cmdcurrpriv <= currpriv)
                                  #else
                                    if (!desc_unique_string (matchvec, string) && cmd_element->cmdpriv <= currpriv)
                                  #endif
                                #else
                                    if (!desc_unique_string (matchvec, string))
                                #endif /* CONFIG_SYS_AAA */
                                        vector_set (matchvec, desc);

                                    /*RTK: do not sort descp if current token index equal no sort index*/
                                    if ((index > 0) && (cmd_element->nosortidx > 0) && (index >= cmd_element->nosortidx))
                                        needsort = FALSE;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    vector_free (cmd_vector);

    if (vector_slot (matchvec, 0) == NULL)
    {
        vector_free (matchvec);

        if (match == invalid_ipmask_match)
            *status = SYS_ERR_CMD_IPMASK;
        else if (match == invalid_macmask_match)
            *status = SYS_ERR_CMD_MACMASK;
        else
            *status = CMD_ERR_NO_MATCH;

        return NULL;
    }

    vty->descpSortState = needsort;

    *status = CMD_SUCCESS;

    return matchvec;
}

vector
cmd_describe_command (vector vline, struct vty *vty, int *status)
{
#if 0
  vector ret;

  if ( cmd_try_do_shortcut(vty->node, vector_slot(vline, 0) ) )
    {
      enum node_type onode;
      vector shifted_vline;
      unsigned int index;

      onode = vty->node;
      vty->node = ENABLE_NODE;
      /* We can try it on enable node, cos' the vty is authenticated */

      shifted_vline = vector_init (vector_count(vline));
      /* use memcpy? */
      for (index = 1; index < vector_active (vline); index++)
    {
      vector_set_index (shifted_vline, index-1, vector_lookup(vline, index));
    }

      ret = cmd_describe_command_real (shifted_vline, vty, status);

      vector_free(shifted_vline);
      vty->node = onode;
      return ret;
  }
#endif

  return cmd_describe_command_real (vline, vty, status);
}


/* Check LCD of matched command. */
static int
cmd_lcd (char **matched)
{
  int i;
  int j;
  int lcd = -1;
  char *s1, *s2;
  char c1, c2;

  if (matched[0] == NULL || matched[1] == NULL)
    return 0;

  for (i = 1; matched[i] != NULL; i++)
    {
      s1 = matched[i - 1];
      s2 = matched[i];

      for (j = 0; (c1 = s1[j]) && (c2 = s2[j]); j++)
    if (c1 != c2)
      break;

      if (lcd < 0)
    lcd = j;
      else
    {
      if (lcd > j)
        lcd = j;
    }
    }
  return lcd;
}

/* Command line completion support. */
static char **
cmd_complete_command_real (vector vline, struct vty *vty, int *status)
{
#define INIT_MATCHVEC_SIZE 10
    unsigned int i;
    vector cmd_vector = vector_copy (cmd_node_vector (cmdvec, vty->node));
    vector matchvec;
    struct cmd_element *cmd_element;
    unsigned int index;
    char **match_str;
    struct desc *desc;
    vector descvec;
    char *command;
    int lcd;

    if (vector_active (vline) == 0)
    {
        vector_free (cmd_vector);
        *status = CMD_ERR_NO_MATCH;
        return NULL;
    }
    else
    {
        index = vector_active (vline) - 1;
    }

    /* First, filter by preceeding command string */
    for (i = 0; i < index; i++)
    {
        if ((command = vector_slot (vline, i)))
        {
            enum match_type match;
            int ret;

            /* First try completion match, if there is exactly match return 1 */
            match = cmd_filter_by_completion (vline, command, cmd_vector, i);

            /* If there is exact match then filter ambiguous match else check
                ambiguousness. */
            if (SYS_ERR_OK != (ret = is_cmd_ambiguous (command, cmd_vector, i, match)))
            {
                vector_free (cmd_vector);
                *status = ret;
                return NULL;
            }
        }
    }

    /* Prepare match vector. */
    matchvec = vector_init (INIT_MATCHVEC_SIZE);

    /* Now we got into completion */
    for (i = 0; i < vector_active (cmd_vector); i++)
    {
        if ((cmd_element = vector_slot (cmd_vector, i)))
        {
            const char *string;
            vector strvec = cmd_element->strvec;

            /* Skip hidden command */
            if (cmd_element->attr == CMD_ATTR_CONDITION_HIDDEN)
            {
                if (NULL != cmd_element->hidmatch && NULL != vty)
                {
                    if (TRUE == cmd_element->hidmatch(cmd_element, vty))
                        continue;
                }
                else
                {
                    continue;
                }
            }

            /* Check field length */
            if (index >= vector_active (strvec))
            {
                vector_slot (cmd_vector, i) = NULL;
            }
            else
            {
                unsigned int j;

                if ((index > 0) && NULL != (desc = _intf_devnum_string(vline, strvec, index - 1)))
                {
                    break;
                }
                else
                {
                    descvec = vector_slot (strvec, index);
                    for (j = 0; j < vector_active (descvec); j++)
                    {
                        if ((desc = vector_slot (descvec, j)))
                        {
                            if ((string =
                                    cmd_entry_function (vector_slot (vline, index),
                                    desc->cmd)))
                            {
                            #ifdef CONFIG_SYS_AAA
                              #ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
                                if (cmd_unique_string (matchvec, string) && cmd_element->cmdcurrpriv <= currpriv)
                              #else
                                if (cmd_unique_string (matchvec, string) && cmd_element->cmdpriv <= currpriv)
                              #endif
                            #else
                                if (cmd_unique_string (matchvec, string))
                            #endif /* CONFIG_SYS_AAA */
                                    vector_set (matchvec, XSTRDUP (MTYPE_TMP, string));
                            }
                        }
                    }
                }
            }
        }
    }

    /* We don't need cmd_vector any more. */
    vector_free (cmd_vector);

    /* No matched command */
    if (vector_slot (matchvec, 0) == NULL)
    {
        vector_free (matchvec);

        /* In case of 'command \t' pattern.  Do you need '?' command at
           the end of the line. */
        if (vector_slot (vline, index) == '\0')
            *status = CMD_ERR_NOTHING_TODO;
        else
            *status = CMD_ERR_NO_MATCH;
        return NULL;
    }

    /* Only one matched */
    if (vector_slot (matchvec, 1) == NULL)
    {
        match_str = (char **) matchvec->index;
        vector_only_wrapper_free (matchvec);
        if (CMD_FS(match_str[0]))
            *status = CMD_ERR_NOTHING_TODO;
        else
            *status = CMD_COMPLETE_FULL_MATCH;
        return match_str;
    }

    /* Make it sure last element is NULL. */
    vector_set (matchvec, NULL);

    /* Check LCD of matched strings. */
    if (vector_slot (vline, index) != NULL)
    {
        lcd = cmd_lcd ((char **) matchvec->index);

        if (lcd)
        {
            int len = strlen (vector_slot (vline, index));

            if (len < lcd)
            {
                char *lcdstr;

                lcdstr = XMALLOC (MTYPE_STRVEC, lcd + 1);
                memcpy (lcdstr, matchvec->index[0], lcd);
                lcdstr[lcd] = '\0';

                /* match_str = (char **) &lcdstr; */

                /* Free matchvec. */
                for (i = 0; i < vector_active (matchvec); i++)
                {
                    if (vector_slot (matchvec, i))
                    XFREE (MTYPE_STRVEC, vector_slot (matchvec, i));
                }
                vector_free (matchvec);

                /* Make new matchvec. */
                matchvec = vector_init (INIT_MATCHVEC_SIZE);
                vector_set (matchvec, lcdstr);
                match_str = (char **) matchvec->index;
                vector_only_wrapper_free (matchvec);

                *status = CMD_COMPLETE_MATCH;
                return match_str;
            }
        }
    }

    match_str = (char **) matchvec->index;
    vector_only_wrapper_free (matchvec);
    *status = CMD_COMPLETE_LIST_MATCH;
    return match_str;
}

char **
cmd_complete_command (vector vline, struct vty *vty, int *status)
{
#if 0
  char **ret;

  if ( cmd_try_do_shortcut(vty->node, vector_slot(vline, 0) ) )
    {
      enum node_type onode;
      vector shifted_vline;
      unsigned int index;

      onode = vty->node;
      vty->node = ENABLE_NODE;
      /* We can try it on enable node, cos' the vty is authenticated */

      shifted_vline = vector_init (vector_count(vline));
      /* use memcpy? */
      for (index = 1; index < vector_active (vline); index++)
    {
      vector_set_index (shifted_vline, index-1, vector_lookup(vline, index));
    }

      ret = cmd_complete_command_real (shifted_vline, vty, status);

      vector_free(shifted_vline);
      vty->node = onode;
      return ret;
  }
#endif

  return cmd_complete_command_real (vline, vty, status);
}

/* return parent node */
/* MUST eventually converge on CONFIG_NODE */
enum node_type
node_parent ( enum node_type node )
{
    enum node_type ret;

    //assert (node > CONFIG_NODE);

    switch (node)
    {
        default:
            ret = CONFIG_NODE;
    }

    return ret;
}



/* Execute command by argument vline vector. */
static int
cmd_execute_command_real (vector vline, struct vty *vty,
              struct cmd_element **cmd, const char *cmdline)
{
  int ret;
  unsigned int i;
  unsigned int index;
  vector cmd_vector;
  struct cmd_element *cmd_element;
  struct cmd_element *matched_element;
  unsigned int matched_count, incomplete_count;
  int argc;
  char *argv[CMD_ARGC_MAX];
  enum match_type match = 0;
  int varflag;
  char *command;

  /* Make copy of command elements. */
  cmd_vector = vector_copy (cmd_node_vector (cmdvec, vty->node));

  for (index = 0; index < vector_active (vline); index++)
    if ((command = vector_slot (vline, index)))
      {
    int ret;

    match = cmd_filter_by_completion (vline, command, cmd_vector, index);

    if (match == vararg_match)
      break;

    if (SYS_ERR_OK != (ret = is_cmd_ambiguous (command, cmd_vector, index, match)))
    {
        vector_free(cmd_vector);
        return ret;
    }
      }

  /* Check matched count. */
  matched_element = NULL;
  matched_count = 0;
  incomplete_count = 0;

  for (i = 0; i < vector_active (cmd_vector); i++)
    if ((cmd_element = vector_slot (cmd_vector, i)))
      {
    if (match == vararg_match || index >= cmd_element->cmdsize)
      {
        matched_element = cmd_element;
#if 0
        printf ("DEBUG: %s\n", cmd_element->string);
#endif
        matched_count++;
      }
    else
      {
        incomplete_count++;
      }
      }

  /* Finish of using cmd_vector. */
  vector_free (cmd_vector);

  /* To execute command, matched_count must be 1. */
  if (matched_count == 0)
    {
      if (incomplete_count)
    return CMD_ERR_INCOMPLETE;
      else
    return CMD_ERR_NO_MATCH;
    }

  /* RTK: Remove check for multiple option commands*/
  //if (matched_count > 1)
  //  return CMD_ERR_AMBIGUOUS;

  /* Argument treatment */
  varflag = 0;
  argc = 0;


    for (i = 0; i < vector_active (vline); i++)
    {
        if (varflag)
        {
            argv[argc++] = vector_slot (vline, i);
        }
        else
        {
            vector descvec = vector_slot (matched_element->strvec, i);
            struct desc *desc = vector_slot (descvec, 0);

            if (vector_active (descvec) == 1)
            {
                if (CMD_VARARG (desc->cmd))
                    varflag = 1;

                if (varflag || CMD_VARIABLE (desc->cmd) || CMD_OPTION (desc->cmd) || CMD_FS(desc->cmd))
                {
                    /* RTK: If data is "", it means empty string */
                    if (0 == osal_strcmp(vector_slot (vline, i), "\"\""))
                        argv[argc++] = "";
                    else
                        argv[argc++] = vector_slot (vline, i);

                }
            }
            else
            {
                if (0 == osal_strcmp(vector_slot (vline, i), "\"\""))
                    argv[argc++] = "";
                else
                    argv[argc++] = vector_slot (vline, i);

            }
        }

        if (argc >= CMD_ARGC_MAX)
            return CMD_ERR_EXEED_ARGC_MAX;
    }

    //SYS_PRINTF("accounting command=%s\n", acctCmd); /* Accounting command debug */

  /* For vtysh execution. */
  if (cmd)
    *cmd = matched_element;

  if (matched_element->daemon)
    return CMD_SUCCESS_DAEMON;


  matched_element->string = cmdline;

  matched_element->vline = vline;


  /* Execute matched command. */
  vty_echo_disable();

  ret = (*matched_element->func) (matched_element, vty, argc, argv);

  vty_echo_enable();

  return ret;
}

int
cmd_execute_command (vector vline, struct vty *vty, struct cmd_element **cmd,
             int vtysh, const char *cmdline) {
  int ret, saved_ret, tried = 0;
  enum node_type onode, try_node;

  onode = try_node = vty->node;

  /* status clean of vty_out() */
  vty_out_done();

#if 0
  if ( cmd_try_do_shortcut(vty->node, vector_slot(vline, 0) ) )
    {
      vector shifted_vline;
      unsigned int index;

      vty->node = ENABLE_NODE;
      /* We can try it on enable node, cos' the vty is authenticated */

      shifted_vline = vector_init (vector_count(vline));
      /* use memcpy? */
      for (index = 1; index < vector_active (vline); index++)
    {
      vector_set_index (shifted_vline, index-1, vector_lookup(vline, index));
    }

      ret = cmd_execute_command_real (shifted_vline, vty, cmd, cmdline);

      vector_free(shifted_vline);
      vty->node = onode;
      return ret;
  }
#endif

  if (cmd_try_do_shortcut(vty->node, cmdline))
  {
      vector shifted_vline;

      onode = vty->node;

      vty->node = ENABLE_NODE;
      shifted_vline = cmd_make_strvec (cmdline + 3, vty->node);

      if (NULL == shifted_vline)
        return CMD_ERR_NO_MATCH;

      ret = cmd_execute_command_real (shifted_vline, vty, cmd, cmdline);

      vector_free(shifted_vline);
      vty->node=onode;

      return ret;
  }
  else
  {
      saved_ret = ret = cmd_execute_command_real (vline, vty, cmd, cmdline);
  }

  //if (vtysh)
  //  return saved_ret;

  /* This assumes all nodes above CONFIG_NODE are childs of CONFIG_NODE */
  while ( (ret == CMD_WARNING || ret == CMD_ERR_NO_MATCH || ret == CMD_ERR_INCOMPLETE || ret == CMD_ERR_BAD_PARAM || ret == CMD_ERR_PORTLIST) && vty->node > CONFIG_NODE )
  {
      try_node = node_parent(try_node);
      vty->node = try_node;
      ret = cmd_execute_command_real (vline, vty, cmd, cmdline);
      tried = 1;
      if (ret != CMD_ERR_NO_MATCH && ret != CMD_ERR_AMBIGUOUS && ret != CMD_ERR_INCOMPLETE)
      {
        /* succesfull command, leave the node as is */
        return ret;
      }
  }
  /* no command succeeded, reset the vty to the original node and
     return the error for this node */
  if ( tried )
    vty->node = onode;
  return saved_ret;
}

/* Execute command by argument readline. */
int
cmd_execute_command_strict (vector vline, struct vty *vty,
                struct cmd_element **cmd)
{
  unsigned int i;
  unsigned int index;
  vector cmd_vector;
  struct cmd_element *cmd_element;
  struct cmd_element *matched_element;
  unsigned int matched_count, incomplete_count;
  int argc;
  char *argv[CMD_ARGC_MAX];
  int varflag;
  enum match_type match = 0;
  char *command;

  /* Make copy of command element */
  cmd_vector = vector_copy (cmd_node_vector (cmdvec, vty->node));

  for (index = 0; index < vector_active (vline); index++)
    if ((command = vector_slot (vline, index)))
      {
    int ret;

    //match = cmd_filter_by_string (vector_slot (vline, index),
    //                  cmd_vector, index);
    match = cmd_filter_by_completion (vline, command, cmd_vector, index);

    /* If command meets '.VARARG' then finish matching. */
    if (match == vararg_match)
      break;

    if (SYS_ERR_OK != (ret = is_cmd_ambiguous (command, cmd_vector, index, match)))
    {
        vector_free(cmd_vector);
        return ret;
    }
      }

  /* Check matched count. */
  matched_element = NULL;
  matched_count = 0;
  incomplete_count = 0;
  for (i = 0; i < vector_active (cmd_vector); i++)
    if (vector_slot (cmd_vector, i) != NULL)
      {
    cmd_element = vector_slot (cmd_vector, i);

    if (match == vararg_match || index >= cmd_element->cmdsize)
      {
        matched_element = cmd_element;
        matched_count++;
      }
    else
      incomplete_count++;
      }

  /* Finish of using cmd_vector. */
  vector_free (cmd_vector);

  /* To execute command, matched_count must be 1. */
  if (matched_count == 0)
    {
      if (incomplete_count)
    return CMD_ERR_INCOMPLETE;
      else
    return CMD_ERR_NO_MATCH;
    }

  /* RTK: Remove check for multiple option commands*/
  //if (matched_count > 1)
  //  return CMD_ERR_AMBIGUOUS;

  /* Argument treatment */
  varflag = 0;
  argc = 0;

  for (i = 0; i < vector_active (vline); i++)
  {
    if (varflag)
      argv[argc++] = vector_slot (vline, i);
    else
    {
      vector descvec = vector_slot (matched_element->strvec, i);

      if (vector_active (descvec) == 1)
      {
        struct desc *desc = vector_slot (descvec, 0);

        if (CMD_VARARG (desc->cmd))
          varflag = 1;

        if (varflag || CMD_VARIABLE (desc->cmd) || CMD_OPTION (desc->cmd) || CMD_FS(desc->cmd))
        {
          /* RTK: If data is "", it means empty string */
          if (0 == osal_strcmp(vector_slot (vline, i), "\"\""))
            argv[argc++] = "";
          else
            argv[argc++] = vector_slot (vline, i);
        }
      }
      else
      {
        /* RTK: If data is "", it means empty string */
        if (0 == osal_strcmp(vector_slot (vline, i), "\"\""))
          argv[argc++] = "";
        else
          argv[argc++] = vector_slot (vline, i);
      }
    }

    if (argc >= CMD_ARGC_MAX)
      return CMD_ERR_EXEED_ARGC_MAX;
  }

  /* For vtysh execution. */
  if (cmd)
    *cmd = matched_element;

  if (matched_element->daemon)
    return CMD_SUCCESS_DAEMON;

  matched_element->string = vty->buf;

  matched_element->vline = vline;

  /* Now execute matched command */
  return (*matched_element->func) (matched_element, vty, argc, argv);
}

uint32 cmd_valid_command_verify(char *cmdstr, int32 node)
{
    vector vline = NULL;
    vector cmd_vector = NULL;
    enum match_type match = 0;
    struct cmd_element *cmd_element;
    //struct cmd_element *matched_element;
    struct cmd_node *cnode = NULL;
    uint32 matched_count;
    uint32 incomplete_count;
    char *command = NULL;
    int32 ret;
    uint32 index;
    uint32 i;

    cnode = vector_slot (cmdvec, node);

    if (NULL == cnode)
        return FALSE;

    vline = cmd_make_strvec(cmdstr, node);

    if (NULL == vline)
        return TRUE;

    cmd_vector = vector_copy(cnode->cmd_vector);

    if (NULL == cmd_vector)
        return FALSE;

    for (index = 0; index < vector_active (vline); index++)
    {
        if ((command = vector_slot(vline, index)))
        {
            match = cmd_filter_by_completion (vline, command, cmd_vector, index);

            if (SYS_ERR_OK != (ret = is_cmd_ambiguous (command, cmd_vector, index, match)))
            {
                vector_free(cmd_vector);
                cmd_free_strvec(vline);

                return FALSE;
            }
        }
    }

    /* Check matched count. */
    //matched_element = NULL;
    matched_count = 0;
    incomplete_count = 0;
    for (i = 0; i < vector_active (cmd_vector); i++)
    {
        if (vector_slot (cmd_vector, i) != NULL)
        {
            cmd_element = vector_slot (cmd_vector, i);

            if (match == vararg_match || index >= cmd_element->cmdsize)
            {
                //matched_element = cmd_element;
                matched_count++;
            }
            else
            {
                incomplete_count++;
            }
        }
    }

    /* Finish of using cmd_vector. */
    vector_free(cmd_vector);
    cmd_free_strvec(vline);

    /* For valid command, matched_count must > 0. */
    if (matched_count == 0)
    {
        return FALSE;
    }

    return TRUE;
}

int32 config_valid_file_convert(char *file)
{
    FILE *fp = NULL;
    FILE *fp_tmp = NULL;
    char cmdstr[SYS_BUF1024_LEN];
    char tmpFile[CAPA_FILE_NAME_LEN];
    int32 curr_node;
    uint32 validCmd;
    uint32 headerChk = FALSE;

    if (NULL == (fp = fopen(file, "r")))
    {
        return SYS_ERR_FILE_OPEN;
    }

    SYS_MEM_CLEAR(tmpFile);
    sal_util_randomfileName_gen("/tmp", "config_valid_gen", tmpFile);

    if (NULL == (fp_tmp = fopen(tmpFile, "w")))
    {
        fclose(fp);
        return SYS_ERR_FILE_OPEN;
    }

    if (strlen(DFLT_SYS_CFG_HEAD_START) > 0)
        headerChk = TRUE;

    SYS_MEM_CLEAR(cmdstr);

    while (fgets(cmdstr, VTY_BUFSIZ, fp))
    {
        validCmd = FALSE;

        /* Skip configuration header */
        if (TRUE == headerChk)
        {
            if (0 == strncmp(cmdstr, DFLT_SYS_CFG_HEAD_START, strlen(cmdstr) - 1))
            {
                fprintf(fp_tmp, "%s", cmdstr);

                while (fgets(cmdstr, VTY_BUFSIZ, fp))
                {
                    fprintf(fp_tmp, "%s", cmdstr);
                    if (0 == strncmp(cmdstr, DFLT_SYS_CFG_HEAD_END, strlen(cmdstr) - 1))
                        break;
                }

                headerChk = FALSE;
            }
        }

        for (curr_node = CONFIG_NODE; curr_node < END_NODE; curr_node++)
        {
            if (TRUE == cmd_valid_command_verify(cmdstr, curr_node))
            {
                validCmd = TRUE;
                break;
            }
        }

        if (TRUE == validCmd)
        {
            fprintf(fp_tmp, "%s", cmdstr);
        }

        SYS_MEM_CLEAR(cmdstr);
    }

    fclose(fp);
    fclose(fp_tmp);

    sal_util_file_copy(tmpFile, file);
    unlink(tmpFile);

    return SYS_ERR_OK;
}

uint32 config_valid_file_check(char *file)
{
    FILE *fp = NULL;
    char cmdstr[SYS_BUF1024_LEN];
    int32 curr_node;
    uint32 headerChk = FALSE;
    uint32 validCmd = FALSE;

    if (NULL == (fp = fopen(file, "r")))
    {
        return SYS_ERR_FILE_OPEN;
    }

    if (strlen(DFLT_SYS_CFG_HEAD_START) > 0)
        headerChk = TRUE;

    SYS_MEM_CLEAR(cmdstr);

    while (fgets(cmdstr, VTY_BUFSIZ, fp))
    {
        /* Skip configuration header */
        if (TRUE == headerChk)
        {
			/*
			*	修改说明：原始的是通过比较配置文件头部开头和结尾字符串长度来进行判定的；
			*		这种存在一个bug,如果配置文件在win系统上进行编辑之后，则会导致头部字符串
			*		的长度增加一个字节；
			*   规避方案：通过判断查找配置头部字符串进行判断；
			*/
        #if 0  
            if (0 == strncmp(cmdstr, DFLT_SYS_CFG_HEAD_START, strlen(cmdstr) - 1))
            {
                while (fgets(cmdstr, VTY_BUFSIZ, fp))
                {
                    if (0 == strncmp(cmdstr, DFLT_SYS_CFG_HEAD_END, strlen(cmdstr) - 1))
                        break;
                }

                headerChk = FALSE;
                continue;
            }
        #else 
			if(osal_strstr(cmdstr, DFLT_SYS_CFG_HEAD_START))
			{
				while (fgets(cmdstr, VTY_BUFSIZ, fp))
                {
                    if(osal_strstr(cmdstr, DFLT_SYS_CFG_HEAD_END))
                        break;
                }

                headerChk = FALSE;
                continue;
			}
        #endif
        }

        validCmd = FALSE;
        for (curr_node = CONFIG_NODE; curr_node < END_NODE; curr_node++)
        {
            if (TRUE == cmd_valid_command_verify(cmdstr, curr_node))
            {
                validCmd = TRUE;
                break;
            }
        }

        if (FALSE == validCmd)
        {
            fclose(fp);
            return FALSE;
        }

        SYS_MEM_CLEAR(cmdstr);
    }

    fclose(fp);

    return TRUE;
}


/* Configration make from file. */
int
config_from_file (struct vty *vty, FILE *fp)
{
  int ret;
  vector vline;

  while (fgets (vty->buf, VTY_BUFSIZ, fp))
    {
      vline = cmd_make_strvec (vty->buf, vty->node);

      /* In case of comment line */
      if (vline == NULL)
    continue;
      /* Execute configuration command : this is strict match */
      ret = cmd_execute_command_strict (vline, vty, NULL);

      /* Try again with setting node to CONFIG_NODE */
      while (ret != CMD_SUCCESS && ret != CMD_WARNING
         && ret != CMD_ERR_NOTHING_TODO && vty->node != CONFIG_NODE)
    {
      vty->node = node_parent(vty->node);
      ret = cmd_execute_command_strict (vline, vty, NULL);
    }

      cmd_free_strvec (vline);

      if (ret != CMD_SUCCESS && ret != CMD_WARNING
      && ret != CMD_ERR_NOTHING_TODO)
    return ret;
    }
  return CMD_SUCCESS;
}

/* Configration make from file and not stop after reading invalid command */
int
config_init_from_file (struct vty *vty, FILE *fp)
{
    int ret;
    vector vline;
    int try_node;
    int old_node;
    int tried;
    sys_enable_t textline_enable = DISABLED;

    while (fgets (vty->buf, VTY_BUFSIZ, fp))
    {
        // RTK: print startup configuration file
        //SYS_PRINTF("cmd = %s", vty->buf);

        cmd_textline_enable_get(&textline_enable);
        if (textline_enable == ENABLED)
        {
            /* skip '\n' */
            vty->buf[strlen(vty->buf) - 1] = '\0';
            cmd_textline_func_execute(vty->buf);
            continue;
        }

        vline = cmd_make_strvec (vty->buf, vty->node);

        /* In case of comment line */
        if (vline == NULL)
            continue;

        /* Execute configuration command : this is strict match */
        ret = cmd_execute_command_strict (vline, vty, NULL);

#if 0 /* Do not allow it to change to CONFIG_NODE */
        /* Try again with setting node to CONFIG_NODE */
      while (ret != CMD_SUCCESS && ret != CMD_WARNING
         && ret != CMD_ERR_NOTHING_TODO && vty->node != CONFIG_NODE)
      {
          vty->node = node_parent(vty->node);
          ret = cmd_execute_command_strict (vline, vty, NULL);
      }
#endif
        tried = 0;
        try_node = 0;
        old_node = vty->node;

        if (CMD_ERR_NO_MATCH == ret && NULL == strstr(vty->buf, DFLT_SYS_CFG_HEAD_START))
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_COMMAND_FAIL,
                        vty->buf);
        }

        while ( ret != CMD_SUCCESS && ret != CMD_WARNING && vty->node > CONFIG_NODE )
        {
            try_node = node_parent(vty->node);
            vty->node = try_node;
            ret = cmd_execute_command_strict (vline, vty, NULL);
            tried = 1;
            if (ret == CMD_SUCCESS || ret == CMD_WARNING)
            {
                /* succesfull command, process next command */
                tried = 0;
                break;
            }

            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_COMMAND_FAIL,
                        vty->buf);
        }

        if ( tried )
            vty->node = old_node;

        cmd_free_strvec (vline);
    }

    return CMD_SUCCESS;
}

#if 0
/* Show version. */
DEFUN (show_version,
       show_version_cmd,
       "show version",
       SHOW_STR
       "Display software version\n")
{
#if 0
  vty_out (vty, "%s v%s (%s)%s", DFLT_SYS_PLATFORM_NAME, DFLT_SYS_VER,
           host.name ? host.name : "", VTY_NEWLINE);
  vty_out (vty, "%s%s", DFLT_SYS_COPYRIGHT, VTY_NEWLINE);
#endif
  //vty_out (vty, "Hostname : %s%s", host.name ? host.name : "", VTY_NEWLINE);
  //vty_out (vty, "Model    : %s%s", DFLT_SYS_PLATFORM_NAME, VTY_NEWLINE);
  vty_out (vty, "Version  : %s%s", DFLT_SYS_VER, VTY_NEWLINE);

  return CMD_SUCCESS;
}
#endif
#if 0

/* Help display function for all node. */
DEFUN (config_help,
       config_help_cmd,
       "help",
       "Description of the interactive help system\n")
{
  vty_out (vty,
       "Quagga VTY provides advanced help feature.  When you need help,%s\
anytime at the command line please press '?'.%s\
%s\
If nothing matches, the help list will be empty and you must backup%s\
 until entering a '?' shows the available options.%s\
Two styles of help are provided:%s\
1. Full help is available when you are ready to enter a%s\
command argument (e.g. 'show ?') and describes each possible%s\
argument.%s\
2. Partial help is provided when an abbreviated argument is entered%s\
   and you want to know what arguments match the input%s\
   (e.g. 'show me?'.)%s%s", VTY_NEWLINE, VTY_NEWLINE, VTY_NEWLINE,
       VTY_NEWLINE, VTY_NEWLINE, VTY_NEWLINE, VTY_NEWLINE, VTY_NEWLINE,
       VTY_NEWLINE, VTY_NEWLINE, VTY_NEWLINE, VTY_NEWLINE, VTY_NEWLINE);
  return CMD_SUCCESS;
}
#endif

/* Help display function for all node. */
DEFUN (config_list,
       config_list_cmd,
       "list",
       "Print command list\n")
{
  unsigned int i;
  struct cmd_node *cnode = vector_slot (cmdvec, vty->node);
  struct cmd_element *cmd;

  for (i = 0; i < vector_active (cnode->cmd_vector); i++)
    if ((cmd = vector_slot (cnode->cmd_vector, i)) != NULL
        && !(cmd->attr == CMD_ATTR_DEPRECATED
             || cmd->attr == CMD_ATTR_HIDDEN))
      vty_out (vty, "  %s%s", cmd->string,
           VTY_NEWLINE);
  return CMD_SUCCESS;
}

#if 0
/* Hostname configuration */
DEFUN (config_hostname,
       hostname_cmd,
       "hostname WORD",
       "Set system's network name\n"
       "This system's network name\n")
{
#if 0
  if (!isalpha((int) *argv[0]))
    {
      vty_out (vty, "Please specify string starting with alphabet%s", VTY_NEWLINE);
      return CMD_WARNING;
    }

  //if (host.name)
  //  XFREE (MTYPE_HOST, host.name);

  host.name = XSTRDUP (MTYPE_HOST, argv[0]);
  return CMD_SUCCESS;
#endif
    SYS_ERR_CHK(FUNC_CMD_SET(cmd_sys_systemName_set));
    return SYS_ERR_OK;
}
#endif

/* Set config filename.  Called from vty.c */
void
host_config_set (char *filename)
{
  if (host.config)
    XFREE (MTYPE_HOST, host.config);
  host.config = XSTRDUP (MTYPE_HOST, filename);
}

static void _cmd_node_install(uint32 is4Cli)
{
    /* Install top nodes. */
    if (is4Cli)
    {
        install_node (&view_node, NULL);
        install_node (&enable_node, NULL);
        install_node (&debug_node, NULL);
    }

    install_node (&config_node, NULL);
    install_node (&intf_node, NULL);
    install_node (&intf_trk_node, NULL);
#ifdef CONFIG_SYS_L3_ROUTE
    install_node (&intf_vlan_node, NULL);
    install_node (&intf_lo_node, NULL);
#endif
    install_node (&vty_node, NULL);
    install_node (&vty_console_node, NULL);
    install_node (&vlan_db_node , NULL);
#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    install_node (&igmp_profile_node, NULL);
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    install_node (&mld_profile_node, NULL);
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    install_node (&acl_mac_node, NULL);
    install_node (&acl_ipv4_node, NULL);
    install_node (&acl_ipv6_node, NULL);
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    install_node (&mgmt_acl_node , NULL);
#endif
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    install_node (&mst_cfg_node , NULL);
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
    install_node (&erps_inst_node , NULL);
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
    install_node (&ip_pool_node , NULL);
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
    install_node (&rip_node , NULL);
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	install_node (&ospf_node , NULL);
	install_node (&area_node , NULL);
#endif
#ifdef CONFIG_SYS_L3_ROUTE_BGP
	install_node (&bgp_node , NULL);
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	install_node (&cwmp_node , NULL);
#endif


}

/* Initialize command interface. Install basic nodes and commands. */
void
cmd_init (int terminal)
{
  char strBuf[SYS_BUF32_LEN];
  uint32 faNum = 0;
  uint32 giNum = 0;
  uint32 teNum = 0;
  uint32 mgiNum = 0;
  sys_logic_port_t lp;

  init_default_motd();
  init_default_motd_file();
  command_cr = XSTRDUP(MTYPE_STRVEC, "<cr>");
  desc_cr.cmd = command_cr;
  desc_cr.str = XSTRDUP(MTYPE_STRVEC, "");

  FOR_EACH_NORMAL_LOGIC_PORT(lp)
  {
      if (PORT_TYPE_100M == LP2TYPE(lp))
          faNum++;
      else if (PORT_TYPE_1000M == LP2TYPE(lp))
          giNum++;
      else if (PORT_TYPE_2500M == LP2TYPE(lp))
          mgiNum++;
      else if (PORT_TYPE_10G == LP2TYPE(lp))
          teNum++;
  }

  SYS_MEM_CLEAR(strBuf);
  osal_sprintf(strBuf, "<1-%d>", faNum);
  intf_fa_devnum.cmd = XSTRDUP(MTYPE_STRVEC, strBuf);
  //intf_fa_devnum.str = XSTRDUP(MTYPE_STRVEC, IF_FA_STR" device number");
  intf_fa_devnum.str = XMALLOC(MTYPE_STRVEC, osal_strlen(IF_FA_STR) + 1);
  osal_memset(intf_fa_devnum.str, 0, osal_strlen(IF_FA_STR) + 1);
  osal_strcpy(intf_fa_devnum.str, IF_FA_STR);

  SYS_MEM_CLEAR(strBuf);
  osal_sprintf(strBuf, "<1-%d>", giNum);
  intf_gi_devnum.cmd = XSTRDUP(MTYPE_STRVEC, strBuf);
  //intf_gi_devnum.str = XSTRDUP(MTYPE_STRVEC, IF_GI_STR" device number");
  intf_gi_devnum.str = XMALLOC(MTYPE_STRVEC, osal_strlen(IF_GI_STR) + 1);
  osal_memset(intf_gi_devnum.str, 0, osal_strlen(IF_GI_STR) + 1);
  osal_strcpy(intf_gi_devnum.str, IF_GI_STR);

  SYS_MEM_CLEAR(strBuf);
  osal_sprintf(strBuf, "<1-%d>", mgiNum);
  intf_mgi_devnum.cmd = XSTRDUP(MTYPE_STRVEC, strBuf);
  //intf_gi_devnum.str = XSTRDUP(MTYPE_STRVEC, IF_GI_STR" device number");
  intf_mgi_devnum.str = XMALLOC(MTYPE_STRVEC, osal_strlen(IF_MGI_STR) + 1);
  osal_memset(intf_mgi_devnum.str, 0, osal_strlen(IF_MGI_STR) + 1);
  osal_strcpy(intf_mgi_devnum.str, IF_MGI_STR);

  SYS_MEM_CLEAR(strBuf);
  osal_sprintf(strBuf, "<1-%d>", teNum);
  intf_te_devnum.cmd = XSTRDUP(MTYPE_STRVEC, strBuf);
  //intf_te_devnum.str = XSTRDUP(MTYPE_STRVEC, IF_TE_STR" device number");
  intf_te_devnum.str = XMALLOC(MTYPE_STRVEC, osal_strlen(IF_TE_STR) + 1);
  osal_memset(intf_te_devnum.str, 0, osal_strlen(IF_TE_STR) + 1);
  osal_strcpy(intf_te_devnum.str, IF_TE_STR);

  intf_po_devnum.cmd = XSTRDUP(MTYPE_STRVEC, "<1-8>");
  //intf_po_devnum.str = XSTRDUP(MTYPE_STRVEC, IF_PO_STR" interface number");
  intf_po_devnum.str = XMALLOC(MTYPE_STRVEC, osal_strlen(IF_PO_STR) + 1);
  osal_memset(intf_po_devnum.str, 0, osal_strlen(IF_PO_STR) + 1);
  osal_strcpy(intf_po_devnum.str, IF_PO_STR);

#ifdef CONFIG_SYS_L3_ROUTE
  intf_vlan_devnum.cmd = XSTRDUP(MTYPE_STRVEC, "<1-4094>");
  intf_vlan_devnum.str = XSTRDUP(MTYPE_STRVEC, "VLAN interface number");

  SYS_MEM_CLEAR(strBuf);
  osal_sprintf(strBuf, "<1-%d>", CAPA_L3_INTFLO_NUM);
  intf_lo_devnum.cmd = XSTRDUP(MTYPE_STRVEC, strBuf);
  intf_lo_devnum.str = XSTRDUP(MTYPE_STRVEC, "Loop interface number");
#endif

  /* Initialize interface lower string */
  SYS_MEM_CLEAR(intf_low_str);

  osal_strncpy(intf_low_str[PORT_TYPE_100M], DFLT_PORT_PREFIX_FA, CAPA_PORT_STR_LEN);
  osal_strncpy(intf_low_str[PORT_TYPE_1000M], DFLT_PORT_PREFIX_GE, CAPA_PORT_STR_LEN);
  osal_strncpy(intf_low_str[PORT_TYPE_2500M], DFLT_PORT_PREFIX_MGE, CAPA_PORT_STR_LEN);
  osal_strncpy(intf_low_str[PORT_TYPE_10G], DFLT_PORT_PREFIX_TE, CAPA_PORT_STR_LEN);
  osal_strncpy(intf_low_str[PORT_TYPE_LAG], DFLT_PORT_PREFIX_LAG, CAPA_PORT_STR_LEN);

  sal_util_str2lowerStr_xlate(intf_low_str[PORT_TYPE_100M]);
  sal_util_str2lowerStr_xlate(intf_low_str[PORT_TYPE_1000M]);
  sal_util_str2lowerStr_xlate(intf_low_str[PORT_TYPE_2500M]);
  sal_util_str2lowerStr_xlate(intf_low_str[PORT_TYPE_10G]);
  sal_util_str2lowerStr_xlate(intf_low_str[PORT_TYPE_LAG]);

  /* Initialize key string, key string should not the same as full prefix string but with equal
     string length*/
  SYS_MEM_CLEAR(intf_key_str);
  SYS_MEM_CLEAR(intfs_key_str);

  osal_strcpy(intf_key_str[PORT_TYPE_100M], "IF");
  osal_strcpy(intf_key_str[PORT_TYPE_1000M], "IF");
  osal_strcpy(intf_key_str[PORT_TYPE_2500M], "IF");
  osal_strcpy(intf_key_str[PORT_TYPE_10G], "IF");
  osal_strcpy(intf_key_str[PORT_TYPE_LAG], "IF");
  osal_strncpy(intf_key_str[PORT_TYPE_100M] + 2, DFLT_PORT_PREFIX_FA, osal_strlen(DFLT_PORT_PREFIX_FA) - 2);
  osal_strncpy(intf_key_str[PORT_TYPE_1000M] + 2, DFLT_PORT_PREFIX_GE, osal_strlen(DFLT_PORT_PREFIX_GE) - 2);
  osal_strncpy(intf_key_str[PORT_TYPE_2500M] + 2, DFLT_PORT_PREFIX_MGE, osal_strlen(DFLT_PORT_PREFIX_MGE) - 2);
  osal_strncpy(intf_key_str[PORT_TYPE_10G] + 2, DFLT_PORT_PREFIX_TE, osal_strlen(DFLT_PORT_PREFIX_TE) - 2);
  osal_strncpy(intf_key_str[PORT_TYPE_LAG] + 2, DFLT_PORT_PREFIX_LAG, osal_strlen(DFLT_PORT_PREFIX_LAG) - 2);
  osal_strcpy(intfs_key_str[PORT_TYPE_100M], "IS");
  osal_strcpy(intfs_key_str[PORT_TYPE_1000M], "IS");
  osal_strcpy(intfs_key_str[PORT_TYPE_2500M], "IS");
  osal_strcpy(intfs_key_str[PORT_TYPE_10G], "IS");
  osal_strcpy(intfs_key_str[PORT_TYPE_LAG], "IS");
  osal_strncpy(intfs_key_str[PORT_TYPE_100M] + 2, DFLT_PORT_PREFIX_FA, osal_strlen(DFLT_PORT_PREFIX_FA) - 2);
  osal_strncpy(intfs_key_str[PORT_TYPE_1000M] + 2, DFLT_PORT_PREFIX_GE, osal_strlen(DFLT_PORT_PREFIX_GE) - 2);
  osal_strncpy(intfs_key_str[PORT_TYPE_2500M] + 2, DFLT_PORT_PREFIX_MGE, osal_strlen(DFLT_PORT_PREFIX_MGE) - 2);
  osal_strncpy(intfs_key_str[PORT_TYPE_10G] + 2, DFLT_PORT_PREFIX_TE, osal_strlen(DFLT_PORT_PREFIX_TE) - 2);
  osal_strncpy(intfs_key_str[PORT_TYPE_LAG] + 2, DFLT_PORT_PREFIX_LAG, osal_strlen(DFLT_PORT_PREFIX_LAG) - 2);

  intf_fas.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_FA_PORTS_STR);
  intf_fas.str = XSTRDUP(MTYPE_STRVEC, "Fast ethernet interface to configure");

  intf_gis.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_GI_PORTS_STR);
  intf_gis.str = XSTRDUP(MTYPE_STRVEC, "Gigabit ethernet interface to configure");

  intf_mgis.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_MGI_PORTS_STR);
  intf_mgis.str = XSTRDUP(MTYPE_STRVEC, "Multi Gigabit ethernet interface to configure");

  intf_tes.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_TE_PORTS_STR);
  intf_tes.str = XSTRDUP(MTYPE_STRVEC, "10 Gigabit ethernet interface to configure");

  intf_pos.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_PO_PORTS_STR);
  intf_pos.str = XSTRDUP(MTYPE_STRVEC, "IEEE 802.3 Link Aggregateion interface");

#ifdef CONFIG_SYS_L3_ROUTE
  intf_vlans.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_KEY_VLANS_STR);
  intf_vlans.str = XSTRDUP(MTYPE_STRVEC, "Vlan interface");

  intf_los.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_KEY_LOS_STR);
  intf_los.str = XSTRDUP(MTYPE_STRVEC, "Loopback interface");
#endif

  intf_fa.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_FA_PORT_STR);
  intf_fa.str = XSTRDUP(MTYPE_STRVEC, "Fast ethernet interface to configure");

  intf_gi.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_GI_PORT_STR);
  intf_gi.str = XSTRDUP(MTYPE_STRVEC, "Gigabit ethernet interface to configure");

  intf_mgi.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_MGI_PORT_STR);
  intf_mgi.str = XSTRDUP(MTYPE_STRVEC, "Multi Gigabit ethernet interface to configure");

  intf_te.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_TE_PORT_STR);
  intf_te.str = XSTRDUP(MTYPE_STRVEC, "10 Gigabit ethernet interface to configure");

  intf_po.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_PO_PORT_STR);
  intf_po.str = XSTRDUP(MTYPE_STRVEC, "IEEE 802.3 Link Aggregateion interface");

#ifdef CONFIG_SYS_L3_ROUTE
  intf_vlan.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_KEY_VLAN_STR);
  intf_vlan.str = XSTRDUP(MTYPE_STRVEC, "Vlan interface");

  intf_lo.cmd = XSTRDUP(MTYPE_STRVEC, CMD_IF_KEY_LO_STR);
  intf_lo.str = XSTRDUP(MTYPE_STRVEC, "Loopback interface");
#endif

  /* Allocate initial top vector of commands. */
  cmdvec = vector_init (VECTOR_MIN_SIZE);

  /* Default host value settings. */
  {
      char sysname[CAPA_SYS_NAME_LEN + 1];
      memset(sysname, 0, sizeof(sysname));
      sal_sys_systemName_get(sysname);
      host.name = XSTRDUP (MTYPE_HOST, sysname);
    //host.name = HOSTNAME;
  }
  host.password = NULL;
  host.enable = NULL;
  host.logfile = NULL;
  host.config = NULL;
  host.lines = -1;
  host.motd = default_motd;
  host.motdfile = default_motdfile;

  _cmd_node_install(TRUE);

  /* Each node's basic commands. */
  //INSTALL_ELEMENT (VIEW_NODE,   CMD_PRIV_1,  &show_version_cmd);
  //INSTALL_SHOW_ELEMENT(CMD_PRIV_1,  &show_version_cmd);
  //INSTALL_ELEMENT (CONFIG_NODE, CMD_PRIV_15, &hostname_cmd);
  //INSTALL_ELEMENT (ENABLE_NODE, CMD_PRIV_15, &hostname_cmd);
  //INSTALL_ELEMENT (CONFIG_NODE, &no_hostname_cmd);

  srand(time(NULL));
}

/* Initialize command interface, basic nodes and commands for SAL cfg module. */
void
cmd_sal_init (void)
{
  command_cr = XSTRDUP(MTYPE_STRVEC, "<cr>");
  desc_cr.cmd = command_cr;
  desc_cr.str = XSTRDUP(MTYPE_STRVEC, "");

  /* Allocate initial top vector of commands. */
  cmdvec = vector_init (VECTOR_MIN_SIZE);

  _cmd_node_install(FALSE);
}

void cmd_node_dump(void)
{
    dump_cmd_syntax = TRUE;
    _cmd_node_install(TRUE);
    dump_cmd_syntax = FALSE;
}

void
cmd_terminate ()
{
  unsigned int i, j, k, l;
  struct cmd_node *cmd_node;
  struct cmd_element *cmd_element;
  struct desc *desc;
  vector cmd_node_v, cmd_element_v, desc_v;

  if (cmdvec)
    {
      for (i = 0; i < vector_active (cmdvec); i++)
        if ((cmd_node = vector_slot (cmdvec, i)) != NULL)
          {
            cmd_node_v = cmd_node->cmd_vector;

            for (j = 0; j < vector_active (cmd_node_v); j++)
              if ((cmd_element = vector_slot (cmd_node_v, j)) != NULL &&
                  cmd_element->strvec != NULL)
                {
                  cmd_element_v = cmd_element->strvec;

                  for (k = 0; k < vector_active (cmd_element_v); k++)
                    if ((desc_v = vector_slot (cmd_element_v, k)) != NULL)
                      {
                        for (l = 0; l < vector_active (desc_v); l++)
                          if ((desc = vector_slot (desc_v, l)) != NULL)
                            {
                              if (desc->cmd)
                                XFREE (MTYPE_STRVEC, desc->cmd);
                              if (desc->str)
                                XFREE (MTYPE_STRVEC, desc->str);

                              XFREE (MTYPE_DESC, desc);
                            }
                        vector_free (desc_v);
                      }

                  cmd_element->strvec = NULL;
                  vector_free (cmd_element_v);
                }

            vector_free (cmd_node_v);
          }

      vector_free (cmdvec);
      cmdvec = NULL;
    }

  if (command_cr)
    XFREE(MTYPE_STRVEC, command_cr);
  if (desc_cr.str)
    XFREE(MTYPE_STRVEC, desc_cr.str);
  if (host.name)
    XFREE (MTYPE_HOST, host.name);
  if (host.password)
    XFREE (MTYPE_HOST, host.password);
  if (host.password_encrypt)
    XFREE (MTYPE_HOST, host.password_encrypt);
  if (host.enable)
    XFREE (MTYPE_HOST, host.enable);
  if (host.enable_encrypt)
    XFREE (MTYPE_HOST, host.enable_encrypt);
  if (host.logfile)
    XFREE (MTYPE_HOST, host.logfile);
  if (host.motdfile)
    XFREE (MTYPE_HOST, host.motdfile);
  if (host.config)
    XFREE (MTYPE_HOST, host.config);
}

 /******************************************************************************
 * 函数名称: transLongStrToStrArray
 * 函数功能: 将长字符串转化为指定数组字符串
 * 输入参数: strBuf - 长字符串
			 strLenth - 指定的字符串长度
 * 输出参数: rtBuf - 转化后的字符串列表
 * 函数返回: 列表的个数
 *******************************************************************************/
 int transLongStrToStrArray(char *strBuf, char rtBuf[][128], int strLenth)
 {
	 int  i;
	 int  counter = 0;
	 int  strTotalLen = strlen(strBuf);
	 
	 if ((NULL == strBuf) || (NULL == rtBuf) || (strLenth <= 0) || (strLenth >= 128))
	 {
		 return 0;
	 }

	 for (i = 0; ((strTotalLen > 0) && (i < (strTotalLen / strLenth))); i++)
	 {
		 memcpy(rtBuf[i], &strBuf[i * strLenth], strLenth);
		 rtBuf[i][strLenth] = '\0';
		 counter++;
	 }
	 if (strTotalLen - (i * strLenth) > 0)
	 {
		 memcpy(rtBuf[i], &strBuf[i * strLenth], strTotalLen - (i * strLenth));
		 rtBuf[i][strTotalLen - (i * strLenth)] = '\0';
		 counter++;
	 }
	 
	 return counter;
 }

 /******************************************************************************
* 函数名称: format2Print
* 函数功能: 将输入数据格式化为指定的字符串格式
* 创建日期: 2002/07/3
* 作    者: 翟春耕
* 输入参数: format - 格式类型
*           string - 输入数据
* 输出参数: result - 格式化后的字符串
*           pcDisjunctive - 分割符
* 返    回: 返回数据长度
*******************************************************************************/
int format2Print(int format, char *string, char *result, char *pcDisjunctive)
{
    int valLen;
    unsigned int *value; 
    //int indexLoc, move;
    //char strTmp[16] = {0};
    //int stackEmpt=1;
    //int portStack[2]={0};
    //int stackDeep=0;

    if((string == NULL) || (result == NULL))
    {
        return 0;
    }

    *pcDisjunctive  = 0;

    switch(format & 0xff)
    {
    case PRINT_DECIMAL:
        value = (unsigned int *)string;
        sprintf(result, "%u", *value);
        break;

    case PRINT_FLOAT:
        {
            char *pFormat[] = {"%f", "%.1f", "%.2f", "%.3f", "%.4f", "%.5f", "%.6f", "%.7f"};
            int val = 0;
            double *data = (double *)string;
            val = ((format & 0xff00) >> 8) % (sizeof(pFormat) / sizeof(char *));

            sprintf(result, pFormat[val], *data);
        }
        break;

    case PRINT_IP_ADDRESS:
        if( (format & 0xff00) == (PRINT_DECIMAL << 8) )
        {
            unsigned int *pIPADDR = (unsigned int *)string;
            sprintf(result, "%u.%u.%u.%u", pIPADDR[0], pIPADDR[1], pIPADDR[2], pIPADDR[3]);
        }
        else
        {
            ulong_t ulTmp;
            UCHAR *pIPADDR = (UCHAR *)&ulTmp;
            
            memcpy(pIPADDR, string, 4);
            ulTmp = htonl(ulTmp);
            
            sprintf(result, "%d.%d.%d.%d", pIPADDR[0], pIPADDR[1], pIPADDR[2], pIPADDR[3]);
        }
        *pcDisjunctive  = '.';
        break;

    case PRINT_ENUM:
    case PRINT_STRING:
        valLen = strlen(string);
        memcpy(result, string, valLen);
        result[valLen] = '\0';
        break;

    case PRINT_CHAR:
        valLen = 1;
        sprintf(result, "%c", string[0]);
        break;

    case PRINT_MAC_ADDRESS:
        if( (format & 0xff00) == (PRINT_DECIMAL << 8))
        {
            unsigned int *pMac = (unsigned int *)string;

            sprintf(result, "%02x:%02x:%02x:%02x:%02x:%02x",
                pMac[0], pMac[1], pMac[2], pMac[3], pMac[4], pMac[5]);
        }
        else
        {
            UCHAR *pMac = (UCHAR *)string;

            sprintf(result, "%02x:%02x:%02x:%02x:%02x:%02x",
                pMac[0], pMac[1], pMac[2], pMac[3], pMac[4], pMac[5]);
        }   
        *pcDisjunctive  = ':';
        break;

    case PRINT_TIME:
        value = (unsigned int *)string;
        valLen = 4;
        {
            int day, hour, minute, second;

            /* second = *value /100; */
            second = *value;
            minute = second /60;
            hour = minute /60;
            day = hour / 24;
            hour %= 24;
            minute %= 60;
            second %= 60;

            if(day > 0)
            {
                sprintf(result, "%d day %d hour %d minute %d second", day, hour, minute, second);
            }
            else if(hour > 0)
            {
                sprintf(result, "%d hour %d minute %d second", hour, minute, second);
            }
            else if(minute > 0)
            {
                sprintf(result, "%d minute %d second",  minute, second);
            }
            else
            {
                sprintf(result, "%d second", second);    
            }
        }
        break;

    case PRINT_PORT:
        break;
    case PRINT_GGMPPORT:
       break;
    default:
        return 0;
    }

    return strlen(result);
}


int _add_value_to_printor(int format, void *value, char isNextLine, vector *pVector, int nLen)
{
    PRINT_DATA_ELEMENT *pElement;
    vector pLineElement = NULL;
    char *pData = NULL;
    int  nPrintLen = 0;

    if(*pVector == NULL)
    {
        *pVector = vector_init (VECTOR_MIN_SIZE);
    }

    if(*pVector == NULL)
    {
        return -1;
    }

    if(value == NULL)
    {
        value = "  ";
    }

    if (PRINT_STRING == format)
    {
        nPrintLen = strlen(value);
    }

    nPrintLen = (nPrintLen < MAX_PRINT_CHAR_LENGTH)?MAX_PRINT_CHAR_LENGTH:nPrintLen;

    pData = malloc(nPrintLen + sizeof(char));
    if(NULL == pData)
    {
        exit(1);
    }

    pElement = (PRINT_DATA_ELEMENT *)malloc(sizeof(PRINT_DATA_ELEMENT));

    if(pElement == NULL)
    {
        free(pData);
        return -1;
    }

    pData[0] = '\0';
    pData[nPrintLen] = '\0';
    pElement->nDataLength = format2Print(format, value, pData, &(pElement->cDisjunctive));

    if((0 > pElement->nDataLength)|| (nPrintLen < pElement->nDataLength))
    {
        free(pData);
        free(pElement);

        return 0;
    }
	//printf("\r\n%s %d\n", __func__, __LINE__);
    pElement->pszData = (char *)malloc(pElement->nDataLength + 1);

    if(pElement->pszData == NULL)
    {
        free(pData);
        free(pElement);
        return -1;
    }

    pElement->nMaxLen = nLen;
    if(pElement->nMaxLen)
    {
        pElement->nSpace = 0;
        if(pElement->nDataLength > pElement->nMaxLen)
        {
            pElement->nDataLength = pElement->nMaxLen;              
        }
    }
    else
    {
        pElement->nSpace = 1;
    }

    memcpy(pElement->pszData, pData, pElement->nDataLength);
    pElement->pszData[pElement->nDataLength] = '\0';

    if(isNextLine)
    {
        pLineElement = vector_init (VECTOR_MIN_SIZE);
        if(NULL == pLineElement)
        {
            free(pData);
            free(pElement);
            return -1;
        }

         vector_set(*pVector, pLineElement);
    }

    pLineElement = vector_lookup(*pVector, (vector_active(*pVector) - 1));

    vector_set(pLineElement, pElement);

    free(pData);

    return 0;
}

int free_printor_vector(vector *pVector)
{
	unsigned int index;
	unsigned int elementindex;
	vector pElement;
	PRINT_DATA_ELEMENT *print_element;
	
	if(pVector == NULL)
		return -1;

	for (index = 0; index < vector_active (*pVector); index++) 
	{
		pElement = vector_lookup(*pVector, index);
		if(pElement == NULL)
        {
            continue;
        }
		for (elementindex = 0; elementindex < vector_active (pElement); elementindex++)
		{
			print_element = vector_lookup(pElement, elementindex);
			if(NULL == print_element)
            {
                continue;
            }

            if(print_element->pszData)
            {
                free(print_element->pszData);
            }

            free(print_element);
		}

        vector_free (pElement);
	}
	vector_free (*pVector);

    return 0;
}
/*自动计算列的长度对齐*/
int add_value_to_new_line_printor(int format, void *value, vector *pVector)
{
    #define NEW_LINE_TO_PRINTOR 1

    return _add_value_to_printor(format, value, NEW_LINE_TO_PRINTOR, pVector, 0);  
}
/*自动计算列的长度对齐*/
int add_value_to_last_line_printor(int format, void * value, vector * pVector)
{
    #define LAST_LINE_TO_PRINTOR 0

    return _add_value_to_printor(format, value, LAST_LINE_TO_PRINTOR, pVector, 0);
}
/**
*根据输入的列元素长度对齐; 如果列的内容超过了列的最大长度时就需要再加一行输出；参考show vlan 命令实现
**/
int add_value_to_new_line_printor_bylen(int format, void *value, vector *pVector, char nlen)
{
    #define NEW_LINE_TO_PRINTOR 1

    return _add_value_to_printor(format, value, NEW_LINE_TO_PRINTOR, pVector, nlen);  
}

/**
*根据输入的列元素长度对齐； 如果列的内容超过了列的最大长度时就需要再加一行输出；参考show vlan 命令实现
**/
int add_value_to_last_line_printor_bylen(int format, void * value, vector * pVector, char nlen)
{
    #define LAST_LINE_TO_PRINTOR 0

    return _add_value_to_printor(format, value, LAST_LINE_TO_PRINTOR, pVector, nlen);
}


int getMaxColumnCount(vector cmd2print)
{
    int column;
    int i;
    vector pLineElement;

    for(column = 0, i = 0; i < vector_active(cmd2print); i++)
    {
        pLineElement = vector_lookup(cmd2print, i);

        if(column < vector_active(pLineElement))
        {
            column = vector_active(pLineElement);
        }
    }

    return column;
}

int getMaxElementLength(vector       cmd2print, int column)
{
    int colCount, nMaxLength;
    int i;
    vector  pLineElement;
    PRINT_DATA_ELEMENT *print_element;

    colCount = getMaxColumnCount(cmd2print);

    for(i = 0, nMaxLength = 0; i < vector_active(cmd2print); i++)
    {
        pLineElement = vector_lookup(cmd2print, i);

        if((NULL == pLineElement) || (colCount > vector_active(pLineElement)))
        {
            continue;
        }

        print_element = vector_lookup(pLineElement, column);

        if(nMaxLength < print_element->nDataLength)
        {
            nMaxLength = print_element->nDataLength;
        }
    }

    return nMaxLength;
}

int cmdGetLine2Print(char *pszDestination, char *pszSource, char keyWord, int MaxLen)
{
    int nlength = 0;

    if((NULL == pszDestination) || (NULL == pszSource))
    {
        return -1;
    }
    
    if(pszSource[MaxLen-1] == keyWord)
    {
        nlength = MaxLen;
    }
    else
    {
        nlength = MaxLen - 1;

        while((pszSource[nlength] != keyWord) && (nlength >= 0))
        {
            nlength --;
        }

        if(nlength < 0)
        {
            nlength = MaxLen - 1;
        }
    }

    if (nlength != MaxLen)
    {
        nlength++;
    }
    memcpy(pszDestination, pszSource, nlength);

    return nlength;
}

static int cmdPrintFormatBuffer(char *pFormatBuf, int moreLineCnt, int *pLineNum, int *pWaitLineCnt, char *pLastInputChar)
{
    char lineBuf[LINE_CHAR_MAX + 1] = {0};
    char *tmpPtr, *pTmp = pFormatBuf;
    unsigned int ulTmp;
    int i = 0, newMoreLineCnt = 0;
    int isNoWaitVersion = FALSE;

    assert(pFormatBuf != NULL);
    assert(moreLineCnt >= 0);

    if (pLastInputChar == NULL)
        isNoWaitVersion = TRUE;

    if (isNoWaitVersion)
    {
        /* Skip the first "\r\n" and must NOT increase newMoreLineCnt,
         * Start to increase it from the second occurrence of "\r\n". */
        tmpPtr = strstr(pTmp, "\r\n");
        if (tmpPtr)
        {
            pTmp = tmpPtr + strlen("\r\n");
            if (*pTmp)
            {
                do
                {
                    newMoreLineCnt++;
                    tmpPtr = strstr(pTmp, "\r\n");
                    pTmp = tmpPtr != NULL ? tmpPtr + strlen("\r\n") : pTmp + strlen(pTmp);
                } while (tmpPtr && *pTmp);
            }
        }

        pTmp = pFormatBuf;

        /* 必须先增加等待输出的行数统计值，否则调用OutStringMore时，
         * *pWaitLineCnt错误，导致输出最后两行时，按回车键可能输出多行信息 */
        *pWaitLineCnt += moreLineCnt + (newMoreLineCnt > moreLineCnt ? newMoreLineCnt - moreLineCnt : 0);

        for (i = 0; i <= newMoreLineCnt; i++)
        {
            tmpPtr = strstr(pTmp, "\r\n");
            ulTmp = tmpPtr != NULL ? tmpPtr - pTmp : strlen(pTmp);
            strncpy(lineBuf, pTmp, ulTmp);
            lineBuf[ulTmp] = '\0';
            XMORE("%s", lineBuf);

            (*pLineNum)++;
            (*pWaitLineCnt)--;

            pTmp += ulTmp + (tmpPtr != NULL ? strlen("\r\n") : 0);
        }
    }
    else
        XMORE("%s", pTmp);

    return 0;
}

/*打印一行信息*/
int cmdPrintOneLine(vector pLineElement, vector cmd2print, int nMaxColNum, int *pLineNum, int *pWaitLineCnt, char *pLastInputChar)
{
    int minRepeat;
    int CharCountToPrint, nPrintCharCount = 0, maxCharToPrint = 0;
    unsigned int nMaxElementLength;
    char tmpBuf[(LINE_CHAR_MAX + 2) * 3 + 1] = {0}; /* addintional 2 for '\r' and '\n', addintional 1 for '\0' */
    char *pszTmp = tmpBuf;
    unsigned int nLocation = 0;
    int iMove = 0, move = 0;
    int nMoreLine = 0;
    PRINT_DATA_ELEMENT *print_element;
    int lines = 0;

    for (minRepeat = 0, CharCountToPrint = 0; minRepeat < vector_active (pLineElement); minRepeat++)
    {
		print_element = vector_lookup(pLineElement, minRepeat);

        if ((NULL == print_element) || (0 > print_element->nDataLength))
        {
            continue;
        }

        if (print_element->nMaxLen)
        {
            nMaxElementLength = print_element->nMaxLen;
        }
        else
        {
            /*获取此列的最大输出长度*/
            nMaxElementLength = getMaxElementLength(cmd2print, minRepeat) + 1;
        }

        if (MAX_LINE_CHAR < nMaxElementLength)
        {
            nMaxElementLength = MAX_LINE_CHAR;
        }

        /*计算当前列最多可以打印字符数*/
        maxCharToPrint = MAX_LINE_CHAR - nPrintCharCount - iMove;
        maxCharToPrint = (maxCharToPrint >= MAX_LINE_CHAR) ? MAX_LINE_CHAR : maxCharToPrint;
        nLocation = 0;

        /*在两个情况下必须换行:
        1、此元素没有分段&&不是第一列&&数据长度大于等于此列最大输
           入字符数&&此元素不能在此列中完全打印&&剩余的空间小于16个字符；
        2、此列最大能打印的字符数小于等于0*/
        if (((nPrintCharCount + nMaxElementLength >= MAX_LINE_CHAR)
              && (print_element->cDisjunctive == 0)
              && (print_element->nDataLength >= maxCharToPrint)
              && (nPrintCharCount > 0) && (maxCharToPrint < 16))
            || (maxCharToPrint <= 0))
        {
            nPrintCharCount = 0;
            maxCharToPrint = MAX_LINE_CHAR;
            memcpy(pszTmp, "\r\n", strlen("\r\n"));
            pszTmp += strlen("\r\n");
            nMoreLine++;
        }

        while (maxCharToPrint < print_element->nDataLength)
        {
            /*根据分割符取出一次最大打印字符串和字符数*/
            CharCountToPrint = cmdGetLine2Print(pszTmp, print_element->pszData + nLocation, print_element->cDisjunctive, maxCharToPrint);
            /*要打印的字符数不合理则退出打印*/
            if( CharCountToPrint <= 0 )
            {
                break;
            }
            pszTmp += CharCountToPrint;
            /*添加换行符*/
            memcpy(pszTmp, "\r\n", strlen("\r\n"));
            pszTmp += strlen("\r\n");

            nMoreLine++;

            /*更新剩余字符数*/
            print_element->nDataLength -= CharCountToPrint;

            lines = (print_element->nDataLength + CharCountToPrint - 1) / CharCountToPrint;
            /* 如果已经是第3行且剩余的缓冲区不能容纳未格式化的字符串
             * 就输出已经格式化的字符串。继续处理未格式化的字符串。
             */
            if (((nMoreLine >= 2) && ((print_element->nDataLength + lines * (MAX_LINE_CHAR - CharCountToPrint)) >= (tmpBuf + sizeof(tmpBuf) - pszTmp))))
            {
                *pszTmp = '\0';

                if (cmdPrintFormatBuffer(tmpBuf, nMoreLine, pLineNum, pWaitLineCnt, pLastInputChar))
                {
                    return 1;
                }

                nMoreLine = 0;
                pszTmp = tmpBuf;
                memset(tmpBuf, '\0', sizeof(tmpBuf));
            }

            /*更新偏余*/
            nLocation += CharCountToPrint;
            /*右移对齐*/
            PRINTOR_MOVE_RIGHT(nPrintCharCount);
        }

        /*打印不足CharCountToPrint个数的字符串*/
        memcpy(pszTmp, print_element->pszData + nLocation, print_element->nDataLength);
        pszTmp += print_element->nDataLength;
        *pszTmp = '\0';

        if (minRepeat == vector_active(pLineElement) - 1)
        {
            continue;
        }

        /*end of Modify in 8.30*/
        /*计算右移字符数*/
        nPrintCharCount += nMaxElementLength;

        move = (nMaxElementLength > print_element->nDataLength) ? (nMaxElementLength - print_element->nDataLength) : 0;
        move = (nMaxElementLength >= (MAX_LINE_CHAR + move)) ? 0 : move;

        PRINTOR_MOVE_RIGHT(move);
        if (!print_element->nSpace)
        {
           continue;
        }

        if ((nMaxColNum == 2) && (vector_active(pLineElement) == 2) && (minRepeat == 0))
        {
            *pszTmp = ':';
            pszTmp++;
            iMove++;
            nPrintCharCount++;
        }

        nPrintCharCount++;
        PRINTOR_MOVE_RIGHT(print_element->nSpace);
     }

    *pszTmp = '\r';
    pszTmp++;
    *pszTmp = '\n';
    pszTmp++;
    *pszTmp = '\0';

    if (cmdPrintFormatBuffer(tmpBuf, nMoreLine, pLineNum, pWaitLineCnt, pLastInputChar))
    {
        return 1;
    }

    return 0;
}

int cmdPrintOutResult(vector cmd2print, char *pLastInputChar)
{
    int repeat = 0, curLine = 0, waitLine = 0;
    int nColCount = 0, rowsPerPage = ROWS_PER_PAGE;
    int rc = 0;
    vector pLineElement;
	unsigned int index;

    if (NULL == cmd2print)
        return 0;

    nColCount = getMaxColumnCount(cmd2print);

    //curLine = allLineCounter[ExecID];
    waitLine = vector_active(cmd2print);

    if (pLastInputChar)
    {
        if (curLine == 0)
        {
            XMORE("\r\n");
        }
    }
    else
    {
        XMORE( "\r\n");
    }

    for (index = 0; index < vector_active (cmd2print); index++) 
	{
	   pLineElement = vector_lookup(cmd2print, index);
	   /*打印一行信息*/
        rc = cmdPrintOneLine(pLineElement, cmd2print, nColCount, &curLine, &waitLine, pLastInputChar);
        if (rc == 1)
            return CTRL_C;

        /*如果满屏，打印提示信息*/
        if (glHeaderLineCount && (repeat >= glHeaderLineCount) &&
            (((rowsPerPage == 0) && (curLine % ROWS_PER_PAGE == 0)) || ((rowsPerPage != 0) && (curLine % rowsPerPage == 0))))
        {
            int outLineCount = 0;

            waitLine += glHeaderLineCount;

            /*打印标题信息,必须调用setHeaderLineCount()设置标题行数后才有效*/
            for (outLineCount = 0; outLineCount < glHeaderLineCount; outLineCount++)
            {
                pLineElement = vector_lookup(cmd2print, outLineCount);

                rc = cmdPrintOneLine(pLineElement, cmd2print, nColCount, &curLine, &waitLine, pLastInputChar);
                if (rc == 1)
                    return CTRL_C;
            }
        }
	}

    return 0;
}


