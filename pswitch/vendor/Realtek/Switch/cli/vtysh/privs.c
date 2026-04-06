
#include <zebra.h>
#include "log.h"
#include "privs.h"
#include "memory.h"

#ifdef HAVE_CAPABILITIES

#ifdef HAVE_LCAPS

struct _pset {
  int num;
  cap_value_t *caps;
};
typedef cap_value_t pvalue_t;
typedef struct _pset pset_t;
typedef cap_t pstorage_t;

#elif defined (HAVE_SOLARIS_CAPABILITIES)
typedef priv_t pvalue_t;
typedef priv_set_t pset_t;
typedef priv_set_t *pstorage_t;
#else 
#error "HAVE_CAPABILITIES defined, but neither LCAPS nor Solaris Capabilties!"
#endif 
#endif 

static zebra_privs_current_t zprivs_null_state = ZPRIVS_RAISED;

static struct _zprivs_t
{
#ifdef HAVE_CAPABILITIES
  pstorage_t caps;        
  pset_t *syscaps_p;        
  pset_t *syscaps_i;         
#endif 
  uid_t zuid,                 
        zsuid;                
  gid_t zgid;                 
  gid_t vtygrp;               
} zprivs_state;

#ifdef HAVE_CAPABILITIES
int zprivs_change_caps (zebra_privs_ops_t);
zebra_privs_current_t zprivs_state_caps (void);
#endif 
int zprivs_change_uid (zebra_privs_ops_t);
zebra_privs_current_t zprivs_state_uid (void);
int zprivs_change_null (zebra_privs_ops_t);
zebra_privs_current_t zprivs_state_null (void);

#ifdef HAVE_CAPABILITIES

static pset_t *zcaps2sys (zebra_capabilities_t *, int);
static void zprivs_caps_init (struct zebra_privs_t *);
static void zprivs_caps_terminate (void);

static struct
{
  int num;
  pvalue_t *system_caps;
} cap_map [ZCAP_MAX] =
{
#ifdef HAVE_LCAPS 
  [ZCAP_SETID] =     { 2, (pvalue_t []) { CAP_SETGID,
                                             CAP_SETUID         }, },
  [ZCAP_BIND] =        { 2, (pvalue_t []) { CAP_NET_BIND_SERVICE,
                                             CAP_NET_BROADCAST         }, },
  [ZCAP_NET_ADMIN] =    { 1, (pvalue_t []) { CAP_NET_ADMIN        }, },
  [ZCAP_NET_RAW] =     { 1, (pvalue_t []) { CAP_NET_RAW        }, },
  [ZCAP_CHROOT] =     { 1, (pvalue_t []) { CAP_SYS_CHROOT,        }, },
  [ZCAP_NICE] =     { 1, (pvalue_t []) { CAP_SYS_NICE         }, },
  [ZCAP_PTRACE] =      { 1, (pvalue_t []) { CAP_SYS_PTRACE         }, },
  [ZCAP_DAC_OVERRIDE] = { 1, (pvalue_t []) { CAP_DAC_OVERRIDE         }, },
  [ZCAP_READ_SEARCH] =  { 1, (pvalue_t []) { CAP_DAC_READ_SEARCH     }, },
  [ZCAP_SYS_ADMIN] =    { 1, (pvalue_t []) { CAP_SYS_ADMIN         }, },
  [ZCAP_FOWNER] =     { 1, (pvalue_t []) { CAP_FOWNER            }, },
#elif defined(HAVE_SOLARIS_CAPABILITIES) 
  
  [ZCAP_SETID] =    { 1, (pvalue_t []) { PRIV_PROC_SETID        }, },
  [ZCAP_BIND] =     { 1, (pvalue_t []) { PRIV_NET_PRIVADDR        }, },
  
#ifdef PRIV_SYS_IP_CONFIG
  [ZCAP_NET_ADMIN] =    { 1, (pvalue_t []) { PRIV_SYS_IP_CONFIG    }, },
#else
  [ZCAP_NET_ADMIN] =    { 1, (pvalue_t []) { PRIV_SYS_NET_CONFIG    }, },
#endif
  [ZCAP_NET_RAW] =     { 2, (pvalue_t []) { PRIV_NET_RAWACCESS,
                                             PRIV_NET_ICMPACCESS     }, },
  [ZCAP_CHROOT] =     { 1, (pvalue_t []) { PRIV_PROC_CHROOT        }, },
  [ZCAP_NICE] =     { 1, (pvalue_t []) { PRIV_PROC_PRIOCNTL        }, },
  [ZCAP_PTRACE] =    { 1, (pvalue_t []) { PRIV_PROC_SESSION        }, },
  [ZCAP_DAC_OVERRIDE] = { 2, (pvalue_t []) { PRIV_FILE_DAC_EXECUTE,
                                             PRIV_FILE_DAC_READ,
                                             PRIV_FILE_DAC_SEARCH,
                                             PRIV_FILE_DAC_WRITE,
                                             PRIV_FILE_DAC_SEARCH    }, },
  [ZCAP_READ_SEARCH] =    { 2, (pvalue_t []) { PRIV_FILE_DAC_SEARCH,
                                             PRIV_FILE_DAC_READ        }, },
  [ZCAP_SYS_ADMIN] =    { 1, (pvalue_t []) { PRIV_SYS_ADMIN        }, },
  [ZCAP_FOWNER] =    { 1, (pvalue_t []) { PRIV_FILE_OWNER        }, },
#endif 
};

#ifdef HAVE_LCAPS

static pset_t *
zcaps2sys (zebra_capabilities_t *zcaps, int num)
{
  pset_t *syscaps;
  int i, j = 0, count = 0;

  if (!num)
    return NULL;

  for (i= 0; i < num; i++)
    count += cap_map[zcaps[i]].num;

  if ( (syscaps = XCALLOC (MTYPE_PRIVS, (sizeof(pset_t) * num))) == NULL)
    {
      fprintf (stderr, "%s: could not allocate syscaps!", __func__);
      return NULL;
    }

  syscaps->caps = XCALLOC (MTYPE_PRIVS, (sizeof (pvalue_t) * count));

  if (!syscaps->caps)
    {
      fprintf (stderr, "%s: could not XCALLOC caps!", __func__);
      return NULL;
    }

  count = 0;
  for (i=0; i < num; i++)
    for (j = 0; j < cap_map[zcaps[i]].num; j++)
      syscaps->caps[count++] = cap_map[zcaps[i]].system_caps[j];

  syscaps->num = count;

  return syscaps;
}

int
zprivs_change_caps (zebra_privs_ops_t op)
{
  cap_flag_value_t cflag;

  assert (zprivs_state.syscaps_p && zprivs_state.caps);
  if (! (zprivs_state.syscaps_p && zprivs_state.caps))
    exit (1);

  if (op == ZPRIVS_RAISE)
    cflag = CAP_SET;
  else if (op == ZPRIVS_LOWER)
    cflag = CAP_CLEAR;
  else
    return -1;

  if ( !cap_set_flag (zprivs_state.caps, CAP_EFFECTIVE,
                       zprivs_state.syscaps_p->num,
                       zprivs_state.syscaps_p->caps,
                       cflag))
    return cap_set_proc (zprivs_state.caps);
  return -1;
}

zebra_privs_current_t
zprivs_state_caps (void)
{
  int i;
  cap_flag_value_t val;

  assert (zprivs_state.syscaps_p && zprivs_state.caps);
  if (! (zprivs_state.syscaps_p && zprivs_state.caps))
    exit (1);

  for (i=0; i < zprivs_state.syscaps_p->num; i++)
    {
      if ( cap_get_flag (zprivs_state.caps, zprivs_state.syscaps_p->caps[i],
                         CAP_EFFECTIVE, &val) )
        {
          zlog_warn ("zprivs_state_caps: could not cap_get_flag, %s",
                     safe_strerror (errno) );
          return ZPRIVS_UNKNOWN;
        }
      if (val == CAP_SET)
        return ZPRIVS_RAISED;
    }
  return ZPRIVS_LOWERED;
}

static void
zprivs_caps_init (struct zebra_privs_t *zprivs)
{
  zprivs_state.syscaps_p = zcaps2sys (zprivs->caps_p, zprivs->cap_num_p);
  zprivs_state.syscaps_i = zcaps2sys (zprivs->caps_i, zprivs->cap_num_i);

  if ( prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0) == -1 )
    {
      fprintf (stderr, "privs_init: could not set PR_SET_KEEPCAPS, %s\n",
                safe_strerror (errno) );
      exit(1);
    }

  if ( !zprivs_state.syscaps_p )
    {
      fprintf (stderr, "privs_init: capabilities enabled, "
                       "but no capabilities supplied\n");
    }

  if (zprivs_state.zuid)
    {
      if ( setreuid (zprivs_state.zuid, zprivs_state.zuid) )
        {
          fprintf (stderr, "zprivs_init (cap): could not setreuid, %s\n",
                     safe_strerror (errno));
          exit (1);
        }
    }

  if ( !(zprivs_state.caps = cap_init()) )
    {
      fprintf (stderr, "privs_init: failed to cap_init, %s\n",
               safe_strerror (errno));
      exit (1);
    }

  if ( cap_clear (zprivs_state.caps) )
    {
      fprintf (stderr, "privs_init: failed to cap_clear, %s\n",
               safe_strerror (errno));
      exit (1);
    }

  cap_set_flag(zprivs_state.caps, CAP_PERMITTED,
               zprivs_state.syscaps_p->num,
               zprivs_state.syscaps_p->caps,
               CAP_SET);

  if (zprivs_state.syscaps_i && zprivs_state.syscaps_i->num)
    {
      cap_set_flag(zprivs_state.caps, CAP_INHERITABLE,
                   zprivs_state.syscaps_i->num,
                   zprivs_state.syscaps_i->caps,
                   CAP_SET);
    }

  if ( cap_set_proc (zprivs_state.caps) )
    {
      fprintf (stderr, "privs_init: initial cap_set_proc failed\n");
      exit (1);
    }

  zprivs->change = zprivs_change_caps;
  zprivs->current_state = zprivs_state_caps;
}

static void
zprivs_caps_terminate (void)
{
  
  if (zprivs_state.caps)
      cap_clear (zprivs_state.caps);

  if ( cap_set_proc (zprivs_state.caps) )
    {
      fprintf (stderr, "privs_terminate: cap_set_proc failed, %s",
                safe_strerror (errno) );
      exit (1);
    }

  if (zprivs_state.syscaps_p->num)
    {
      XFREE (MTYPE_PRIVS, zprivs_state.syscaps_p->caps);
      XFREE (MTYPE_PRIVS, zprivs_state.syscaps_p);
    }

  if (zprivs_state.syscaps_i && zprivs_state.syscaps_i->num)
    {
      XFREE (MTYPE_PRIVS, zprivs_state.syscaps_i->caps);
      XFREE (MTYPE_PRIVS, zprivs_state.syscaps_i);
    }

  cap_free (zprivs_state.caps);
}
#elif defined (HAVE_SOLARIS_CAPABILITIES) 

static pset_t *
zcaps2sys (zebra_capabilities_t *zcaps, int num)
{
  pset_t *syscaps;
  int i, j = 0;

  if ((syscaps = priv_allocset()) == NULL)
    {
      fprintf (stderr, "%s: could not allocate syscaps!\n", __func__);
      exit (1);
    }

  priv_emptyset (syscaps);

  for (i=0; i < num; i++)
    for (j = 0; j < cap_map[zcaps[i]].num; j++)
      priv_addset (syscaps, cap_map[zcaps[i]].system_caps[j]);

  return syscaps;
}

int
zprivs_change_caps (zebra_privs_ops_t op)
{

  assert (zprivs_state.syscaps_p);
  if (!zprivs_state.syscaps_p)
    {
      fprintf (stderr, "%s: Eek, missing caps!", __func__);
      exit (1);
    }

  if (op == ZPRIVS_RAISE)
    priv_copyset (zprivs_state.syscaps_p, zprivs_state.caps);
  else if (op == ZPRIVS_LOWER)
    priv_emptyset (zprivs_state.caps);
  else
    return -1;

  if (setppriv (PRIV_SET, PRIV_EFFECTIVE, zprivs_state.caps) != 0)
    return -1;

  return 0;
}

zebra_privs_current_t
zprivs_state_caps (void)
{
  zebra_privs_current_t result;
  pset_t *effective;

  if ( (effective = priv_allocset()) == NULL)
    {
      fprintf (stderr, "%s: failed to get priv_allocset! %s\n", __func__,
               safe_strerror (errno));
      return ZPRIVS_UNKNOWN;
    }

  if (getppriv (PRIV_EFFECTIVE, effective))
    {
      fprintf (stderr, "%s: failed to get state! %s\n", __func__,
               safe_strerror (errno));
      result = ZPRIVS_UNKNOWN;
    }
  else
    {
      if (priv_isemptyset (effective) == B_TRUE)
        result = ZPRIVS_LOWERED;
      else
        result = ZPRIVS_RAISED;
    }

  if (effective)
    priv_freeset (effective);

  return result;
}

static void
zprivs_caps_init (struct zebra_privs_t *zprivs)
{
  pset_t *basic;
  pset_t *empty;

  zprivs_state.syscaps_p = zcaps2sys (zprivs->caps_p, zprivs->cap_num_p);
  zprivs_state.syscaps_i = zcaps2sys (zprivs->caps_i, zprivs->cap_num_i);

  if (!zprivs_state.syscaps_p)
    {
      fprintf (stderr, "%s: capabilities enabled, "
                       "but no valid capabilities supplied\n",
                       __func__);
    }

  if ((basic = priv_str_to_set("basic", ",", NULL)) == NULL)
    {
      fprintf (stderr, "%s: couldn't get basic set!\n", __func__);
      exit (1);
    }

  priv_union (basic, zprivs_state.syscaps_p);
  priv_freeset (basic);

  if ( (empty = priv_allocset()) == NULL)
    {
      fprintf (stderr, "%s: couldn't get empty set!\n", __func__);
      exit (1);
    }
  priv_emptyset (empty);

  if (setpflags (PRIV_AWARE, 1))
    {
      fprintf (stderr, "%s: error setting PRIV_AWARE!, %s\n", __func__,
               safe_strerror (errno) );
      exit (1);
    }

  assert (zprivs_state.syscaps_i && zprivs_state.syscaps_p);

  if (zprivs_state.zuid)
    {
      if ( setreuid (zprivs_state.zuid, zprivs_state.zuid) )
        {
          fprintf (stderr, "%s: could not setreuid, %s\n",
                   __func__, safe_strerror (errno));
          exit (1);
        }
    }

  if (setppriv (PRIV_SET, PRIV_PERMITTED, zprivs_state.syscaps_p))
    {
      fprintf (stderr, "%s: error setting permitted set!, %s\n", __func__,
               safe_strerror (errno) );
      exit (1);
    }

  if (setppriv (PRIV_SET, PRIV_INHERITABLE, zprivs_state.syscaps_i))
    {
      fprintf (stderr, "%s: error setting inheritable set!, %s\n", __func__,
               safe_strerror (errno) );
      exit (1);
    }

  if (setppriv (PRIV_SET, PRIV_EFFECTIVE, empty))
    {
      fprintf (stderr, "%s: error setting effective set!, %s\n", __func__,
               safe_strerror (errno) );
      exit (1);
    }

  zprivs_state.caps = empty;

  zprivs->change = zprivs_change_caps;
  zprivs->current_state = zprivs_state_caps;
}

static void
zprivs_caps_terminate (void)
{
  assert (zprivs_state.caps);

  priv_emptyset (zprivs_state.caps);
  setppriv (PRIV_SET, PRIV_EFFECTIVE, zprivs_state.caps);
  setppriv (PRIV_SET, PRIV_PERMITTED, zprivs_state.caps);
  setppriv (PRIV_SET, PRIV_INHERITABLE, zprivs_state.caps);

  if (zprivs_state.syscaps_p)
    priv_freeset (zprivs_state.syscaps_p);
  if (zprivs_state.syscaps_i)
    priv_freeset (zprivs_state.syscaps_i);

  priv_freeset (zprivs_state.caps);
}
#else 
#error "Neither Solaris nor Linux capabilities, dazed and confused..."
#endif 
#endif 

int
zprivs_change_uid (zebra_privs_ops_t op)
{

  if (op == ZPRIVS_RAISE)
    return seteuid (zprivs_state.zsuid);
  else if (op == ZPRIVS_LOWER)
    return seteuid (zprivs_state.zuid);
  else
    return -1;
}

zebra_privs_current_t
zprivs_state_uid (void)
{
  return ( (zprivs_state.zuid == geteuid()) ? ZPRIVS_LOWERED : ZPRIVS_RAISED);
}

int
zprivs_change_null (zebra_privs_ops_t op)
{
  return 0;
}

zebra_privs_current_t
zprivs_state_null (void)
{
  return zprivs_null_state;
}

void
zprivs_init(struct zebra_privs_t *zprivs)
{
  struct passwd *pwentry = NULL;
  struct group *grentry = NULL;

  if (!zprivs)
    {
      fprintf (stderr, "zprivs_init: called with NULL arg!\n");
      exit (1);
    }

  if (! (zprivs->user || zprivs->group
         || zprivs->cap_num_p || zprivs->cap_num_i) )
    {
      zprivs->change = zprivs_change_null;
      zprivs->current_state = zprivs_state_null;
      return;
    }

  if (zprivs->user)
    {
      if ( (pwentry = getpwnam (zprivs->user)) )
        {
          zprivs_state.zuid = pwentry->pw_uid;
        }
      else
        {
          
          fprintf (stderr, "privs_init: could not lookup user %s\n",
                   zprivs->user);
          exit (1);
        }
    }

  grentry = NULL;

  if (zprivs->vty_group)
    
    {
      if ( (grentry = getgrnam (zprivs->vty_group)) )
        {
          zprivs_state.vtygrp = grentry->gr_gid;
          if ( setgroups (1, &zprivs_state.vtygrp) )
            {
              fprintf (stderr, "privs_init: could not setgroups, %s\n",
                         safe_strerror (errno) );
              exit (1);
            }
        }
      else
        {
          fprintf (stderr, "privs_init: could not lookup vty group %s\n",
                   zprivs->vty_group);
          exit (1);
        }
    }

  if (zprivs->group)
    {
      if ( (grentry = getgrnam (zprivs->group)) )
        {
          zprivs_state.zgid = grentry->gr_gid;
        }
      else
        {
          fprintf (stderr, "privs_init: could not lookup group %s\n",
                   zprivs->group);
          exit (1);
        }
      
      if ( setregid (zprivs_state.zgid, zprivs_state.zgid) )
        {
          fprintf (stderr, "zprivs_init: could not setregid, %s\n",
                    safe_strerror (errno) );
          exit (1);
        }
    }

#ifdef HAVE_CAPABILITIES
  zprivs_caps_init (zprivs);
#else 
  
  zprivs_state.zsuid = geteuid();
  if ( zprivs_state.zuid )
    {
      if ( setreuid (-1, zprivs_state.zuid) )
        {
          fprintf (stderr, "privs_init (uid): could not setreuid, %s\n",
                   safe_strerror (errno));
          exit (1);
        }
    }

  zprivs->change = zprivs_change_uid;
  zprivs->current_state = zprivs_state_uid;
#endif 
}

void
zprivs_terminate (struct zebra_privs_t *zprivs)
{
  if (!zprivs)
    {
      fprintf (stderr, "%s: no privs struct given, terminating", __func__);
      exit (0);
    }

#ifdef HAVE_CAPABILITIES
  zprivs_caps_terminate();
#else 
  if (zprivs_state.zuid)
    {
      if ( setreuid (zprivs_state.zuid, zprivs_state.zuid) )
        {
          fprintf (stderr, "privs_terminate: could not setreuid, %s",
                     safe_strerror (errno) );
          exit (1);
        }
     }
#endif 

  zprivs->change = zprivs_change_null;
  zprivs->current_state = zprivs_state_null;
  zprivs_null_state = ZPRIVS_LOWERED;
  return;
}

void
zprivs_get_ids(struct zprivs_ids_t *ids)
{

   ids->uid_priv = getuid();
   (zprivs_state.zuid) ? (ids->uid_normal = zprivs_state.zuid)
                     : (ids->uid_normal = -1);
   (zprivs_state.zgid) ? (ids->gid_normal = zprivs_state.zgid)
                     : (ids->gid_normal = -1);
   (zprivs_state.vtygrp) ? (ids->gid_vty = zprivs_state.vtygrp)
                       : (ids->gid_vty = -1);

   return;
}
