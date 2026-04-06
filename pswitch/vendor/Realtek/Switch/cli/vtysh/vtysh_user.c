
#include <zebra.h>
#include <lib/version.h>

#include <pwd.h>

#ifdef USE_PAM
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#ifdef HAVE_PAM_MISC_H
#include <security/pam_misc.h>
#endif
#ifdef HAVE_OPENPAM_H
#include <security/openpam.h>
#endif
#endif 

#include "memory.h"
#include "linklist.h"
#include "command.h"

#define PAM_CONV_FUNC misc_conv

#ifdef USE_PAM
static struct pam_conv conv =
{
  PAM_CONV_FUNC,
  
  NULL
};

int
vtysh_pam (const char *user)
{
  int ret;
  pam_handle_t *pamh = NULL;

  ret = pam_start(QUAGGA_PROGNAME, user, &conv, &pamh);

  if (ret == PAM_SUCCESS)
    ret = pam_authenticate (pamh, 0);

#if 0
  
  if (ret == PAM_SUCCESS)
    ret = pam_acct_mgmt (pamh, 0);
  printf ("ret %d\n", ret);

  if (ret == PAM_AUTHINFO_UNAVAIL)
    ret = PAM_SUCCESS;
#endif 

#ifdef DEBUG
  if (ret == PAM_SUCCESS)
    printf("Authenticated\n");
  else
    printf("Not Authenticated\n");
#endif 

  if (pam_end (pamh, ret) != PAM_SUCCESS)
    {
      pamh = NULL;
      fprintf(stderr, "vtysh_pam: failed to release authenticator\n");
      exit(1);
    }

  return ret == PAM_SUCCESS ? 0 : 1;
}
#endif 

struct vtysh_user
{
  char *name;
  u_char nopassword;
};

struct list *userlist;

struct vtysh_user *
user_new (void)
{
  return XCALLOC (0, sizeof (struct vtysh_user));
}

void
user_free (struct vtysh_user *user)
{
  XFREE (0, user);
}

struct vtysh_user *
user_lookup (const char *name)
{
  struct listnode *node, *nnode;
  struct vtysh_user *user;

  for (ALL_LIST_ELEMENTS (userlist, node, nnode, user))
    {
      if (strcmp (user->name, name) == 0)
    return user;
    }
  return NULL;
}

void
user_config_write (void)
{
  struct listnode *node, *nnode;
  struct vtysh_user *user;

  for (ALL_LIST_ELEMENTS (userlist, node, nnode, user))
    {
      if (user->nopassword)
    printf (" username %s nopassword\n", user->name);
    }
}

struct vtysh_user *
user_get (const char *name)
{
  struct vtysh_user *user;
  user = user_lookup (name);
  if (user)
    return user;

  user = user_new ();
  user->name = strdup (name);
  listnode_add (userlist, user);

  return user;
}

DEFUN (username_nopassword,
       username_nopassword_cmd,
       "username WORD nopassword",
       "\n"
       "\n"
       "\n")
{
  struct vtysh_user *user;
  user = user_get (argv[0]);
  user->nopassword = 1;
  return CMD_SUCCESS;
}

int
vtysh_auth (void)
{
  struct vtysh_user *user;
  struct passwd *passwd;

  passwd = getpwuid (geteuid ());

  user = user_lookup (passwd->pw_name);
  if (user && user->nopassword)
    ;
  else
    {
#ifdef USE_PAM
      if (vtysh_pam (passwd->pw_name))
    exit (0);
#endif 
    }
  return 0;
}

void
vtysh_user_init (void)
{
  userlist = list_new ();
  install_element (CONFIG_NODE, &username_nopassword_cmd);
}
