
#include <libvty/zebra.h>

#include <sys/un.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/stat.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <libvty/command.h>
#include <libvty/memory.h>
#include <libfds/fds_export.h>
#include <libsal/sal_util.h>
#include <libcmd/cmd.h>

#include <common/sys_error.h>
#include <common/sys_def.h>
#include <common/sys_type.h>
#include <common/sys_util.h>

#include "vtysh.h"
#include "log.h"

#define CONFIGFILE_MASK 0600

struct vty *vty;

char *vtysh_pager_name = NULL;

extern struct cmd_node vty_node;

int
vtysh_end (void)
{
  switch (vty->node)
    {
    case VIEW_NODE:
    case ENABLE_NODE:
      
      break;
    default:
      vty->node = ENABLE_NODE;
      break;
    }
  return CMD_SUCCESS;
}

void
vtysh_pager_init (void)
{
  char *pager_defined;

  pager_defined = getenv ("VTYSH_PAGER");

  if (pager_defined)
    vtysh_pager_name = strdup (pager_defined);
  else
    vtysh_pager_name = strdup ("more");
}

static int
vtysh_execute_func (const char *line, int pager)
{
  int ret, cmd_stat;
#if 0
  u_int i;
#endif
  vector vline;
  struct cmd_element *cmd;
  FILE *fp = NULL;
  int closepager = 0;

  vline = cmd_make_strvec (line, vty->node);

  if (vline == NULL)
    return CMD_SUCCESS;

  ret = cmd_execute_command (vline, vty, &cmd, 1, line);

#if 0
  
  while (ret != CMD_SUCCESS && ret != CMD_SUCCESS_DAEMON && ret != CMD_WARNING
     && vty->node > CONFIG_NODE)
    {
      vty->node = node_parent(vty->node);
      ret = cmd_execute_command (vline, vty, &cmd, 1, line);
      tried++;
    }

  vty->node = saved_node;

  if (ret == CMD_SUCCESS || ret == CMD_SUCCESS_DAEMON || ret == CMD_WARNING)
    {
      if ((saved_node == BGP_VPNV4_NODE || saved_node == BGP_IPV4_NODE
       || saved_node == BGP_IPV6_NODE || saved_node == BGP_IPV4M_NODE
       || saved_node == BGP_IPV6M_NODE)
      && (tried == 1))
    {
      vtysh_execute("exit-address-family");
    }
      else if ((saved_node == KEYCHAIN_KEY_NODE) && (tried == 1))
    {
      vtysh_execute("exit");
    }
      else if (tried)
    {
      vtysh_execute ("end");
      vtysh_execute (VTYSH_CONFIG_MODE_ENTER);
    }
    }
  
  else if (tried)
    {
      ret = saved_ret;
    }
#endif

  cmd_free_strvec (vline);

  cmd_stat = ret;
  switch (ret)
    {
    
    case CMD_WARNING:
      if (vty->type == VTY_FILE)
          
      break;
    case CMD_ERR_AMBIGUOUS:
      
      break;
    case CMD_ERR_NO_MATCH:
      
      break;
    case CMD_ERR_INCOMPLETE:
      
      break;
    case CMD_ERR_SLAVE_PORT:
      
      break;
    case CMD_ERR_TRUNK_MBR:
      
      break;
    case CMD_ERR_PORTID:
      
      break;
    case CMD_SUCCESS_DAEMON:
      {
    
    if (pager && vtysh_pager_name && (strncmp(line, "exit", 4) != 0))
      {
        fp = popen (vtysh_pager_name, "w");
        if (fp == NULL)
          {
        perror ("popen failed for pager");
        fp = stdout;
          }
        else
          closepager=1;
      }
    else
      fp = stdout;

    if (! strcmp(cmd->string, VTYSH_CONFIG_MODE_ENTER))
      {
#if 0
        for (i = 0; i < VTYSH_INDEX_MAX; i++)
          {
            cmd_stat = vtysh_client_execute(&vtysh_client[i], line, fp);
        if (cmd_stat == CMD_WARNING)
          break;
          }
#endif
        if (cmd_stat)
          {
        line = "end";
        vline = cmd_make_strvec (line, vty->node);

        if (vline == NULL)
          {
            if (pager && vtysh_pager_name && fp && closepager)
              {
            if (pclose (fp) == -1)
              {
                perror ("pclose failed for pager");
              }
            fp = NULL;
              }
            return CMD_SUCCESS;
          }

        ret = cmd_execute_command (vline, vty, &cmd, 1, line);
        cmd_free_strvec (vline);
        if (ret != CMD_SUCCESS_DAEMON)
          break;
          }
        else
          if (cmd->func)
        {
          (*cmd->func) (cmd, vty, 0, NULL);
          break;
        }
      }

    cmd_stat = CMD_SUCCESS;
#if 0
    for (i = 0; i < VTYSH_INDEX_MAX; i++)
      {
        if (cmd->daemon & vtysh_client[i].flag)
          {
            cmd_stat = vtysh_client_execute(&vtysh_client[i], line, fp);
        if (cmd_stat != CMD_SUCCESS)
          break;
          }
      }
#endif
    if (cmd_stat != CMD_SUCCESS)
      break;

    if (cmd->func)
      (*cmd->func) (cmd, vty, 0, NULL);
      }
    }
  if (pager && vtysh_pager_name && fp && closepager)
    {
      if (pclose (fp) == -1)
    {
      perror ("pclose failed for pager");
    }
      fp = NULL;
    }
  return cmd_stat;
}

int
vtysh_execute_no_pager (const char *line)
{
  return vtysh_execute_func (line, 0);
}

int
vtysh_execute (const char *line)
{
  char *cmd = NULL;
  int ret = 0;

  cmd = (char*)line;

  while (*cmd == ' ')
  {
    cmd++;
  }

  ret = vtysh_execute_func (cmd, 1);

  return ret;
}

#define CLI_CMD_DESCRIPTION_SORT 1
#define CLI_DESC_WIDTH 80
#define CLI_DESC_NEWLINE_KEY '|'
int
vtysh_rl_describe (int count, int ch)
{
  int ret;
  unsigned int i;
  vector vline = NULL;
  vector describe;
  int width;
  struct desc *desc;
  enum node_type old_node;
  sys_enable_t enable = DISABLED;

  cmd_textline_enable_get(&enable);
  if (ENABLED == enable)
  {
      fprintf(stdout, "%c", ch);
      return 0;
  }

  old_node = vty->node;
  if (cmd_try_do_shortcut(vty->node, rl_line_buffer))
  {
      vty->node = ENABLE_NODE;
      vline = cmd_make_strvec (rl_line_buffer + 3, vty->node);
  }
  else
  {
      vline = cmd_make_strvec (rl_line_buffer, vty->node);
  }

  if (vline == NULL)
    {
      vline = vector_init (1);
      vector_set (vline, '\0');
    }
  else
    if (rl_end && isspace ((int) rl_line_buffer[rl_end - 1]))
      vector_set (vline, '\0');

  describe = cmd_describe_command (vline, vty, &ret);

  vty->node = old_node;

  fprintf (stdout,"\n");

#if 0
  switch (ret)
    {
    case CMD_ERR_AMBIGUOUS:
      cmd_free_strvec (vline);
      fprintf (stdout,"Ambiguous command\n");
      rl_on_new_line ();
      return 0;
      break;
    case CMD_ERR_NO_MATCH:
      cmd_free_strvec (vline);
      fprintf (stdout,"There is no matched command\n");
      rl_on_new_line ();
      return 0;
      break;
    case CMD_ERR_SLAVE_PORT:
      cmd_free_strvec (vline);
      fprintf (stdout,"Trunk slave port can not be configured\n");
      rl_on_new_line ();
      return 0;
      break;
    case CMD_ERR_TRUNK_MBR:
      cmd_free_strvec (vline);
      fprintf (stdout,"Trunk member port can not be configured\n");
      rl_on_new_line ();
      return 0;
      break;
    case CMD_ERR_PORTID:
      cmd_free_strvec (vline);
      fprintf(stdout, "Valid port range should be %d~%d\n", USER_PORT_START, USER_PORT_END(SYS_DEV_ID_DEFAULT));
      fprintf (stdout,"Invalid port id\n");
      rl_on_new_line ();
      return 0;
    case CMD_ERR_PORTLIST:
      cmd_free_strvec (vline);
      fprintf (stdout,"Unkonwn port list\n");
      rl_on_new_line ();
      return 0;
      break;
    case CMD_ERR_VALUE:
      cmd_free_strvec (vline);
      fprintf (stdout,"%s\n", SYS_ERR_STR(CMD_ERR_VALUE));
      rl_on_new_line ();
      return 0;
      break;
    }
#else
    if (ret != SYS_ERR_OK)
    {
        cmd_free_strvec (vline);
        fprintf (stdout,"%s\n", SYS_ERR_STR(ret));
        rl_on_new_line ();
        return 0;
    }
#endif

  width = 0;
  for (i = 0; i < vector_active (describe); i++)
    if ((desc = vector_slot (describe, i)) != NULL)
      {
    int len;

    if (desc->cmd[0] == '\0')
      continue;

    len = strlen (desc->cmd);
    if (desc->cmd[0] == '.')
      len--;

    if (width < len)
      width = len;
      }

#if CLI_CMD_DESCRIPTION_SORT
{
    int32 isIntfFaPrt = FALSE;
    int32 isIntfGiPrt = FALSE;
    int32 isIntfMGiPrt = FALSE;
    int32 isIntfTePrt = FALSE;
    int32 isIntfPoPrt = FALSE;
#ifdef CONFIG_SYS_L3_ROUTE
    int32 isIntfVlanPrt = FALSE;
#endif
    typedef struct cli_descp_sort_s
    {
        char *cmd;
        char *str;
        struct cli_descp_sort_s *next;
    } cli_descp_sort_t;

    if (FALSE == vty->descpSortState)
    {
        for (i = 0; i < vector_active (describe); i++)
        {
            if ((desc = vector_slot (describe, i)) != NULL)
            {
                if (desc->cmd[0] == '\0')
                    continue;

                if (! desc->str)
                    fprintf (stdout,"  %-s\n", desc->cmd[0] == '.' ? desc->cmd + 1 : desc->cmd);
                else
                    fprintf (stdout,"  %-*s  %s\n",
                            width,
                            desc->cmd[0] == '.' ? desc->cmd + 1 : desc->cmd,
                            desc->str);
            }
        }
    }
    else
    {
        cli_descp_sort_t *glb_descp = NULL;
        cli_descp_sort_t *sort_descp = NULL;
        cli_descp_sort_t *sort_prev_descp = NULL;
        for (i = 0; i < vector_active(describe); i++)
        {
            
            if ((desc = vector_slot(describe, i )) != NULL)
            {
                cli_descp_sort_t *curr_descp = NULL;
                uint32 cmdLen = 0;
                uint32 strLen = 0;

                if (desc->cmd[0] == '\0')
                    continue;
                curr_descp = malloc(sizeof(cli_descp_sort_t));
                memset(curr_descp, 0, sizeof(cli_descp_sort_t));

                if (!desc->str)
                {
                    cmdLen = strlen(desc->cmd);
                    if (desc->cmd[0] == '.')
                    {
                        curr_descp->cmd = malloc(cmdLen +  1);
                        memset(curr_descp->cmd, 0, cmdLen);
                        strncpy(curr_descp->cmd, desc->cmd+1, cmdLen - 1);
                    }
                    else
                    {
                        curr_descp->cmd = malloc(cmdLen + 1);
                        memset(curr_descp->cmd, 0, cmdLen + 1);
                        strncpy(curr_descp->cmd, desc->cmd, cmdLen);
                    }
                }
                else
                {
                    cmdLen = strlen(desc->cmd);
                    strLen = strlen(desc->str);
                    if (desc->cmd[0] == '.')
                    {
                        curr_descp->cmd = malloc(cmdLen + 1);
                        memset(curr_descp->cmd, 0, cmdLen);
                        strncpy(curr_descp->cmd, desc->cmd+1, cmdLen - 1);

                    }
                    else
                    {
                        curr_descp->cmd = malloc(cmdLen + 1);
                        memset(curr_descp->cmd, 0, cmdLen + 1);
                        strncpy(curr_descp->cmd, desc->cmd, cmdLen);
                    }
                    curr_descp->str = malloc(strLen + 1);
                    memset(curr_descp->str, 0, strLen + 1);
                    strncpy(curr_descp->str, desc->str, strLen);
                }

                if (NULL == glb_descp)
                {
                    glb_descp = curr_descp;
                    glb_descp->next = NULL;
                }
                else
                {

                    uint32 i;
                    uint32 isMatchFound = FALSE;
                    cli_descp_sort_t *match_descp = NULL;
                    
                    sort_descp = glb_descp;
                    sort_prev_descp = NULL;
                    while(sort_descp != NULL)
                    {
                        for(i = 0; i < strlen(curr_descp->cmd); i++)
                        {
                            if (curr_descp->cmd[i] == sort_descp->cmd[i])
                                continue;

                            match_descp = sort_descp;
                            
                            if (curr_descp->cmd[i] < sort_descp->cmd[i])
                            {
                                isMatchFound = TRUE;
                                match_descp = sort_prev_descp;
                            }
                            break;
                        }
                        if (TRUE == isMatchFound)
                            break;
                        sort_prev_descp = sort_descp;
                        sort_descp = sort_descp->next;
                    }

                    if (NULL == match_descp)
                    {
                        curr_descp->next = glb_descp;
                        glb_descp = curr_descp;
                    }
                    else
                    {
                        curr_descp->next = match_descp->next;
                        match_descp->next = curr_descp;
                    }
                }
            }
        }

        sort_descp = glb_descp;
        while(sort_descp != NULL)
        {
            cli_descp_sort_t *tmp_descp = NULL;

            tmp_descp = sort_descp;

            if (!sort_descp->str)
            {
                fprintf(stdout, "  %-s\n", sort_descp->cmd);
            }
            else
            {
                sys_logic_portmask_t portmask;
                char portmaskStr[CAPA_PORTMASK_STR_LEN];

                osal_memset(portmaskStr, 0, sizeof(portmaskStr));
                osal_memset(&portmask, 0, sizeof(sys_logic_portmask_t));

                if (strstr(sort_descp->cmd, "NML-PORT-LIST"))
                {
                    NORMAL_LOGIC_PORTMASK_SET_ALL(portmask);
                    LPM2STR(&portmask, portmaskStr);

                    fprintf(stdout, "  %-*s  %s\n", width, sort_descp->cmd, portmaskStr);
                }
                else if (strstr(sort_descp->cmd, "NML-WOTRKMBR-PORT-LIST"))
                {
                    sys_logic_port_t port;

                    FOR_EACH_NORMAL_LOGIC_PORT(port)
                    {
                        if (!IS_LP_TRKMBR(port))
                        {
                            LOGIC_PORTMASK_SET_PORT(portmask, port);
                        }
                    }
                    LPM2STR(&portmask, portmaskStr);

                    fprintf(stdout, "  %-*s  %s\n", width, sort_descp->cmd, portmaskStr);
                }
                else if (strstr(sort_descp->cmd, "PORT-LIST"))
                {
                    sys_logic_portmask_t portmask;
                    char portmaskStr[CAPA_PORTMASK_STR_LEN];

                    osal_memset(portmaskStr, 0, sizeof(portmaskStr));

                    LOGIC_PORTMASK_SET_ALL(portmask);

                    sys_port_lPortmask2uStr(&portmask, SYS_PORT_STRFMT_BRIEF, FALSE, portmaskStr);

                    fprintf(stdout, "  %-*s  %s\n", width, sort_descp->cmd, portmaskStr);
                }
                else if (CMD_IF_FA_PORTS(sort_descp->cmd) || CMD_IF_FA_PORT(sort_descp->cmd))
                {
                    if (FALSE == isIntfFaPrt)
                    {
                        isIntfFaPrt = TRUE;
                        fprintf(stdout, "  %-*s  %s\n", width, IF_FA_STR, sort_descp->str);
                    }
                }
                else if (CMD_IF_GI_PORTS(sort_descp->cmd) || CMD_IF_GI_PORT(sort_descp->cmd))
                {
                    if (FALSE == isIntfGiPrt)
                    {
                        isIntfGiPrt = TRUE;
                        fprintf(stdout, "  %-*s  %s\n", width, IF_GI_STR, sort_descp->str);
                    }
                }
                else if (CMD_IF_MGI_PORTS(sort_descp->cmd) || CMD_IF_MGI_PORT(sort_descp->cmd))
                {
                    if (FALSE == isIntfMGiPrt)
                    {
                        isIntfMGiPrt = TRUE;
                        fprintf(stdout, "  %-*s  %s\n", width, IF_MGI_STR, sort_descp->str);
                    }
                }
                else if (CMD_IF_TE_PORTS(sort_descp->cmd) || CMD_IF_TE_PORT(sort_descp->cmd))
                {
                    if (FALSE == isIntfTePrt)
                    {
                        isIntfTePrt = TRUE;
                        fprintf(stdout, "  %-*s  %s\n", width, IF_TE_STR, sort_descp->str);
                    }
                }
                else if (CMD_IF_PO_PORTS(sort_descp->cmd) || CMD_IF_PO_PORT(sort_descp->cmd))
                {
                    if (FALSE == isIntfPoPrt)
                    {
                        isIntfPoPrt = TRUE;
                        fprintf(stdout, "  %-*s  %s\n", width, IF_PO_STR, sort_descp->str);
                    }
                }
#ifdef CONFIG_SYS_L3_ROUTE
                else if (CMD_IF_VLANS(sort_descp->cmd) || CMD_IF_VLAN(sort_descp->cmd))
                {
                    if (FALSE == isIntfVlanPrt)
                    {
                        isIntfVlanPrt = TRUE;
                        fprintf(stdout, "  %-*s  %s\n", width, IF_VLAN_STR, sort_descp->str);
                    }
                }
                else if (CMD_IF_LOS(sort_descp->cmd) || CMD_IF_LO(sort_descp->cmd))
                {
                    fprintf(stdout, "  %-*s  %s\n", width, IF_LO_STR, sort_descp->str);
                }
#endif
                
                else
                {
                    uint32 isFirstLine = TRUE;
                    uint32 len = 0;
                    char *currStr = NULL;
                    char *newline = NULL;
                    char buf[CLI_DESC_WIDTH + 1];

                    currStr = sort_descp->str;

                    if (0 == osal_strlen(currStr))
                        fprintf(stdout, "  %-*s  %s\n", width, sort_descp->cmd, sort_descp->str);

                    while (0 != osal_strlen(currStr))
                    {
                        len = osal_strlen(currStr) + width + 4;
                        if (NULL != (newline = osal_strchr(currStr, CLI_DESC_NEWLINE_KEY)))
                        {
                            osal_memset(buf, 0, sizeof(buf));
                            osal_strncpy(buf, currStr, newline - currStr);
                            fprintf(stdout, "  %-*s  %s\n", width, (isFirstLine) ? sort_descp->cmd : " ", buf);
                            currStr = newline + 1;
                        }
                        else if (len > CLI_DESC_WIDTH)
                        {
                            osal_memset(buf, 0, sizeof(buf));
                            osal_strncpy(buf, currStr, CLI_DESC_WIDTH - width - 4);
                            fprintf(stdout, "  %-*s  %s\n", width, (isFirstLine) ? sort_descp->cmd : " ", buf);
                            currStr += CLI_DESC_WIDTH - width - 4;
                        }
                        else
                        {
                            fprintf(stdout, "  %-*s  %s\n", width, (isFirstLine) ? sort_descp->cmd : " ", currStr);
                            break;
                        }

                        if (isFirstLine)
                            isFirstLine = FALSE;
                    }
                }
            }

            if (sort_descp->cmd != NULL)
                free(sort_descp->cmd);
            if (sort_descp->str != NULL)
                free(sort_descp->str);

            sort_descp = sort_descp->next;
            free(tmp_descp);
        }
    }
}
#else
  for (i = 0; i < vector_active (describe); i++)
    if ((desc = vector_slot (describe, i)) != NULL)
      {
    if (desc->cmd[0] == '\0')
      continue;

    if (! desc->str)
      fprintf (stdout,"  %-s\n",
           desc->cmd[0] == '.' ? desc->cmd + 1 : desc->cmd);
    else
      fprintf (stdout,"  %-*s  %s\n",
           width,
           desc->cmd[0] == '.' ? desc->cmd + 1 : desc->cmd,
           desc->str);
      }
#endif

  cmd_free_strvec (vline);
  vector_free (describe);

  rl_on_new_line();

  return 0;
}

int complete_status;

static char *
command_generator (const char *text, int state)
{
  vector vline;
  static char **matched = NULL;
  static int index = 0;
  enum node_type old_node;

  if (! state)
    {
      index = 0;

      old_node = vty->node;
      if (cmd_try_do_shortcut(vty->node, rl_line_buffer))
      {
          vty->node = ENABLE_NODE;
          vline = cmd_make_strvec (rl_line_buffer + 3, vty->node);
      }
      else
      {
          vline = cmd_make_strvec (rl_line_buffer, vty->node);
      }

      if (vline == NULL)
    return NULL;

      if (rl_end && isspace ((int) rl_line_buffer[rl_end - 1]))
    vector_set (vline, '\0');

      matched = cmd_complete_command (vline, vty, &complete_status);

      vty->node = old_node;
    }

  if (matched && matched[index])
    return matched[index++];

  return NULL;
}

static char **
new_completion (char *text, int start, int end)
{
  char **matches;
  sys_enable_t enable = DISABLED;

  cmd_textline_enable_get(&enable);
  if (ENABLED == enable)
      return NULL;

  matches = rl_completion_matches (text, command_generator);

  if (matches)
    {
      rl_point = rl_end;
      if (complete_status == CMD_COMPLETE_FULL_MATCH)
    rl_pending_input = ' ';
    }

  return matches;
}

DEFUN(
     vtysh_end_all,
     vtysh_end_all_cmd,
     "end",
     "End current mode and change to enable mode\n")
{
  return vtysh_end ();
}

DEFUN (
     vtysh_line_vty,
     vtysh_line_vty_cmd,
     "line vty",
     "Configure a terminal line\n"
     "Virtual terminal\n")
{
  vty->node = VTY_NODE;
  return CMD_SUCCESS;
}

#if 0 
DEFUN (
     vtysh_enable,
     vtysh_enable_cmd,
     "enable [<1-15>]",
     "Turn on privileged mode command\n"
     "Privilege level\n")
{
#ifdef CONFIG_SYS_AAA_ENABLE_AUTHENTICATION
    int32 ret = SYS_ERR_FAILED;

    if (argc > 0)
    {
        int32 priv = SYS_STR2UINT(argv[0]);
        if (priv < currpriv)
        {
            XMORE("Current privilege level %d is higher than %d\n", currpriv, priv);
            return SYS_ERR_OK;
        }
        else if (priv == currpriv)
        {
            XMORE("Equal to current privilege level %d\n", currpriv);
        }
        ret = enable_authen(priv);

        if (ret != SYS_ERR_OK)
        {
            XMORE("%% Authentication Failed\n\n");
            return SYS_ERR_OK;
        }

        currpriv = priv;
    }
    else
    {
        ret = enable_authen(15);

        if (ret != SYS_ERR_OK)
        {
            XMORE("%% Authentication Failed\n\n");
            return SYS_ERR_OK;
        }
        currpriv = 15;
    }
#endif
    vty->node = ENABLE_NODE;
    return SYS_ERR_OK;
}

DEFUN (
     vtysh_disable,
     vtysh_disable_cmd,
     "disable [<1-14>]",
     "Turn off privileged mode command\n"
     "Privilege level\n")
{
    if (argc > 0)
    {
        int32 priv = SYS_STR2UINT(argv[0]);
        if (priv > currpriv)
        {
            XMORE("Current privilege level %d is lower than %d\n", currpriv, priv);
            return SYS_ERR_OK;
        }
        currpriv = priv;
    }
    else
    {
        currpriv = 1;
    }

    if (vty->node == ENABLE_NODE)
        vty->node = VIEW_NODE;
  return CMD_SUCCESS;
}
#else
DEFUN (
     vtysh_enable,
     vtysh_enable_cmd,
     "enable",
     "Turn on privileged mode command\n")
{
    
	if(currpriv < 15)
	{
		XMORE("Current user cannot enter privileged mode\n");
		return SYS_ERR_OK;
	}
    vty->node = ENABLE_NODE;
    return SYS_ERR_OK;
}

DEFUN (
     vtysh_disable,
     vtysh_disable_cmd,
     "disable",
     "Turn off privileged mode command\n")
{
    currpriv = currUserPriv;
  if (vty->node == ENABLE_NODE)
    vty->node = VIEW_NODE;
  return CMD_SUCCESS;
}

#endif 

DEFUN (
     vtysh_config_terminal,
     vtysh_config_terminal_cmd,
     VTYSH_CONFIG_MODE_ENTER,
     "Configuration Mode\n")
{
  vty->node = CONFIG_NODE;
  return CMD_SUCCESS;
}

static int
vtysh_exit (struct vty *vty)
{
    switch (vty->node)
    {
        case VIEW_NODE:
            cli_exit();
            break;

        case ENABLE_NODE:
            currpriv = currUserPriv;
            vty->node = VIEW_NODE;
            break;

        case CONFIG_NODE:
            vty->node = ENABLE_NODE;
            break;

        case IF_NODE:
        case VTY_NODE:
        default:
            vty->node = CONFIG_NODE;
            break;
    }

    return CMD_SUCCESS;
}

DEFUN(
     vtysh_exit_all,
     vtysh_exit_all_cmd,
     "exit",
     "Exit current mode and down to previous mode\n")
{
  return vtysh_exit (vty);
}

#if 0
ALIAS (vtysh_exit_all,
       vtysh_quit_all_cmd,
       "quit",
       "Exit current mode and down to previous mode\n")

DEFUN (vtysh_terminal_length,
       vtysh_terminal_length_cmd,
       "terminal length <0-512>",
       "Set terminal line parameters\n"
       "Set number of lines on a screen\n"
       "Number of lines on screen (0 for no pausing)\n")
{
  int lines;
  char *endptr = NULL;
  char default_pager[10];

  lines = strtol (argv[0], &endptr, 10);
  if (lines < 0 || lines > 512 || *endptr != '\0')
    {
      vty_out (vty, "length is malformed%s", VTY_NEWLINE);
      return CMD_WARNING;
    }

  if (vtysh_pager_name)
    {
      free (vtysh_pager_name);
      vtysh_pager_name = NULL;
    }

  if (lines != 0)
    {
      snprintf(default_pager, 10, "more -%i", lines);
      vtysh_pager_name = strdup (default_pager);
    }

  return CMD_SUCCESS;
}

DEFUN (vtysh_terminal_no_length,
       vtysh_terminal_no_length_cmd,
       "terminal no length",
       "Set terminal line parameters\n"
       NO_STR
       "Set number of lines on a screen\n")
{
  if (vtysh_pager_name)
    {
      free (vtysh_pager_name);
      vtysh_pager_name = NULL;
    }

  vtysh_pager_init();
  return CMD_SUCCESS;
}
#endif

static int
execute_command (const char *command, int argc, const char *arg1,
         const char *arg2)
{
  int ret;
  pid_t pid;
  int status;

  pid = fork ();

  if (pid < 0)
    {
      
      fprintf (stderr, "Can't fork: %s\n", safe_strerror (errno));
      exit (1);
    }
  else if (pid == 0)
    {
      
      switch (argc)
    {
    case 0:
      ret = execlp (command, command, (const char *)NULL);
      break;
    case 1:
      ret = execlp (command, command, arg1, (const char *)NULL);
      break;
    case 2:
      ret = execlp (command, command, arg1, arg2, (const char *)NULL);
      break;
    }

      fprintf (stderr, "Can't execute %s: %s\n", command, safe_strerror (errno));
      exit (1);
    }
  else
    {
      
      execute_flag = 1;
      ret = wait4 (pid, &status, 0, NULL);
      execute_flag = 0;
    }
    status = ret;
  return 0;
}

DEFUN (vtysh_start_shell,
       vtysh_start_shell_cmd,
       "diag",
       "Start SDK diag\n")

{
    execute_command ("diag", 0, NULL, NULL);

    return CMD_SUCCESS;
}

static void
vtysh_install_default (enum node_type node)
{
  
  INSTALL_ELEMENT (node, CMD_PRIV_0, &vtysh_end_all_cmd);
}

static char *
vtysh_completion_entry_function (const char *ignore, int invoking_key)
{
  return NULL;
}

static int readln_counter_tick;
static int readln_counter_sec;
static int readln_timeout_sec;

void readln_counter_set(int counter)
{
    readln_counter_sec = counter;
}

void readln_timeout_set(int timeout)
{
    readln_timeout_sec = (timeout > 0)? timeout : 0;
}

int readln_timeout_chk(void)
{
    return (readln_timeout_sec)? (readln_counter_sec - readln_timeout_sec) : -1;
}

void readln_event_hook(void)
{
    readln_counter_tick += 1;   

    if (readln_counter_tick >= 10) {
        readln_counter_tick = 0;
        readln_counter_sec += 1;

        if (readln_timeout_sec && (readln_counter_sec >= readln_timeout_sec)) {
            
            rl_done = 1;    
        }
    }
}

void vtysh_diagDebug(void)
{
    sys_enable_t enable = DISABLED;

    cmd_textline_enable_get(&enable);
    if (ENABLED == enable)
        return;

    if (DISABLED == DFLT_CLI_REMOTE_DEBUG && access_flag != SYS_ACCESS_CLI_CONSOLE)
    {
        printf("\nDo not allow remote user to enter debug mode\n");
        return;
    }

#ifdef CONFIG_SYS_BUILD_RELEASE
    char input_password[CAPA_SYS_PASSWORD_LEN + 1];
    if (DIAGDBG_AUTH_DEBUG == diagdbg_auth)
    {
        printf("\n");
        diagdbg_flag = DIAGDBG_OP_DEBUG;
        return;
    }
    printf("\nDiagnostics: ");
    osal_memset(input_password, 0, sizeof(input_password));
    input_str(input_password, CAPA_SYS_PASSWORD_LEN, '*');

    if (0 == strcmp(input_password, DFLT_CLI_DEBUG_PASSWORD))
    {
        printf("Press ENTER to continue\n");
        diagdbg_flag = DIAGDBG_OP_DEBUG;
        diagdbg_auth = DIAGDBG_AUTH_DEBUG;
    }
    else
    {
        printf("Authentication Fail. Press ENTER to exit\n");
        diagdbg_flag = DIAGDBG_OP_NONE;
    }
#else
    diagdbg_flag = DIAGDBG_OP_DEBUG;
    diagdbg_auth = DIAGDBG_AUTH_DEBUG;
#endif

    return;
}

void
vtysh_readline_init (void)
{
  
  rl_set_keyboard_input_timeout(100000);  
  rl_event_hook = (rl_hook_func_t *)readln_event_hook;

  rl_bind_key ('?', (rl_command_func_t *) vtysh_rl_describe);
  
  rl_bind_keyseq("\\C-t\\C-i\\C-i", (rl_command_func_t *) vtysh_diagDebug);
  rl_bind_keyseq("\\C-t\\C-b\\C-b", (rl_command_func_t *) vtysh_diagDebug);

  rl_completion_entry_function = vtysh_completion_entry_function;

  rl_attempted_completion_function = (rl_completion_func_t *)new_completion;

  rl_completion_append_character = '\0';
}

static int    timeout_duration;

void
timeout_set_duration (
    int    duration
    )
{
    readln_timeout_set(duration);
    timeout_duration = duration;
}

int
timeout_get_duration (void)
{
    return timeout_duration;
}

void
timeout_clear (void)
{
    alarm(0);
}

void
timeout_exit (void)
{
    struct stat st;

    if (timeout_duration <= 0)
        return;

    if ((fstat(STDIN_FILENO, &st) == 0) && (S_ISCHR(st.st_mode)) && (major(st.st_rdev) == 4))    
    {

        return;
    } else {

        return;
    }
}

void
timeout_reset (void)
{
#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    
    uint32 timeout = 0;
    sys_line_cliType_t type = SYS_LINE_CLI_CONSOLE;

    if (access_flag == SYS_ACCESS_CLI_CONSOLE)
        type = SYS_LINE_CLI_CONSOLE;
    else if (access_flag == SYS_ACCESS_CLI_TELNET)
        type = SYS_LINE_CLI_TELNET;
  #ifdef CONFIG_SYS_APP_SSH
    else if (access_flag == SYS_ACCESS_CLI_SSH)
        type = SYS_LINE_CLI_SSH;
  #endif

    sal_sys_cliSessionTimeout_get(type, &timeout);

    timeout *= 60;

    if (timeout != timeout_duration)
        timeout_set_duration(timeout);
#else
    timeout_duration = 0; 
#endif 

    readln_counter_set(0);
    signal(SIGALRM, (void *)timeout_exit);
    alarm(timeout_duration);
}

void
vtysh_init_timeout(int duration)
{
    timeout_set_duration(duration);

    timeout_reset();
}

char *
vtysh_prompt (void)
{
#define MAX_HOST_PROMPT_LEN 200
  static struct utsname names;
  static char buf[100];
  const char*hostname;
  extern struct host host;
  char currSysName[CAPA_SYS_NAME_LEN + 1];
  char showHost[MAX_HOST_PROMPT_LEN + 1];

  osal_memset(currSysName, 0, sizeof(currSysName));
  sal_sys_systemName_get(currSysName);
  if (0 != osal_strcmp(currSysName, host.name))
  {
      XFREE (MTYPE_HOST, host.name);
      host.name = XSTRDUP (MTYPE_HOST, currSysName);
  }

  hostname = host.name;

  if (!hostname)
    {
      if (!names.nodename[0])
    uname (&names);
      hostname = names.nodename;
    }

    SYS_MEM_CLEAR(showHost);
    if (osal_strlen(hostname) > MAX_HOST_PROMPT_LEN)
        osal_strncpy(showHost, hostname, MAX_HOST_PROMPT_LEN);
    else
        osal_strcpy(showHost, hostname);

  SYS_MEM_CLEAR(buf);
  
  if ((IF_NODE == vty->node) || (IF_TRK_NODE == vty->node)
#ifdef CONFIG_SYS_L3_ROUTE
      || (IF_VLAN_NODE == vty->node || IF_LO_NODE == vty->node)
#endif
     )
  {

	    if (TRUE == g_isIntfRange)
	    {
	        snprintf (buf, sizeof(buf), DFLT_CLI_PROMPT_IF_RANGE_EX, showHost, vty->exPrompt);
	        return buf;
	    }
	    else
	    {
			snprintf (buf, sizeof(buf), DFLT_CLI_PROMPT_IF_EX, showHost, vty->exPrompt);
	        return buf;
	    }
  }

#ifdef CONFIG_SYS_DHCP_SERVER
  if (IP_POOL_NODE == vty->node)
  {
	 snprintf (buf, sizeof(buf), DFLT_CLI_PROMPT_DHCP_IPPOOL, showHost, vty->exPrompt);
	 return buf;
  }
#endif

#ifdef CONFIG_SYS_L3_ROUTE_OSPF 
  if (OSPF_NODE == vty->node)
  {
	 snprintf (buf, sizeof(buf), DFLT_CLI_PROMPT_OSPF, showHost, vty->exPrompt);
	 return buf;
  }
  if (AREA_NODE == vty->node)
  {
	 snprintf (buf, sizeof(buf), DFLT_CLI_PROMPT_OSPF, showHost, vty->exPrompt);
	 return buf;
  }
#endif

  snprintf (buf, sizeof(buf), cmd_prompt(vty->node), showHost);

  return buf;
}

void
vtysh_init_vty (void)
{
  
  vty = vty_new ();
  vty->type = VTY_SHELL;
  vty->node = VIEW_NODE;

  cmd_init (0);

  vtysh_install_default (ENABLE_NODE);
  vtysh_install_default (CONFIG_NODE);
  vtysh_install_default (IF_NODE);
  vtysh_install_default (IF_TRK_NODE);
#ifdef CONFIG_SYS_L3_ROUTE
  vtysh_install_default (IF_VLAN_NODE);
#endif
  vtysh_install_default (VTY_NODE);
  vtysh_install_default (VLAN_DB_NODE);
#if defined(CONFIG_SYS_PROTO_IGMP_FILTERING) || defined(CONFIG_SYS_PROTO_IGMP_THROTTLING)
  vtysh_install_default (IGMP_PROFILE_NODE);
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
  vtysh_install_default (MLD_PROFILE_NODE);
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
  vtysh_install_default (ACL_MAC_NODE);
  vtysh_install_default (ACL_IPV4_NODE);
  vtysh_install_default (ACL_IPV6_NODE);
#endif
#ifdef CONFIG_SYS_MGMT_ACL
  vtysh_install_default (MGMT_ACL_NODE);
#endif
#ifdef CONFIG_SYS_PROTO_STP_MSTP
  vtysh_install_default (MST_CFG_NODE);
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
  vtysh_install_default (ERPS_INST_NODE);
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	vtysh_install_default (IP_POOL_NODE);
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	vtysh_install_default (RIP_NODE);
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	vtysh_install_default (OSPF_NODE);
	vtysh_install_default (AREA_NODE);
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	vtysh_install_default (CWMP_NODE);
#endif

  INSTALL_ELEMENT (VIEW_NODE, CMD_PRIV_1, &vtysh_enable_cmd);
  INSTALL_ELEMENT (ENABLE_NODE, CMD_PRIV_15, &vtysh_config_terminal_cmd);
  INSTALL_ELEMENT (ENABLE_NODE, CMD_PRIV_1, &vtysh_disable_cmd);

  INSTALL_ELEMENT (VIEW_NODE, CMD_PRIV_0, &vtysh_exit_all_cmd);
  INSTALL_ELEMENT (CONFIG_NODE, CMD_PRIV_0, &vtysh_exit_all_cmd);
  INSTALL_ELEMENT (ENABLE_NODE, CMD_PRIV_0, &vtysh_exit_all_cmd);
  INSTALL_ELEMENT (VTY_NODE, CMD_PRIV_0, &vtysh_exit_all_cmd);

  INSTALL_ELEMENT (DEBUG_NODE, CMD_PRIV_0, &vtysh_start_shell_cmd);

#if 0
  install_element (VIEW_NODE, CMD_PRIV_1, &vtysh_terminal_length_cmd);
  install_element (ENABLE_NODE, CMD_PRIV_1, &vtysh_terminal_length_cmd);
  install_element (VIEW_NODE, CMD_PRIV_1, &vtysh_terminal_no_length_cmd);
  install_element (ENABLE_NODE, CMD_PRIV_1, &vtysh_terminal_no_length_cmd);
#endif
}

