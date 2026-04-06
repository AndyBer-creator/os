
#ifndef _ZEBRA_PRIVS_H
#define _ZEBRA_PRIVS_H

typedef enum
{
  ZCAP_SETID,
  ZCAP_BIND,
  ZCAP_NET_ADMIN,
  ZCAP_SYS_ADMIN,
  ZCAP_NET_RAW,
  ZCAP_CHROOT,
  ZCAP_NICE,
  ZCAP_PTRACE,
  ZCAP_DAC_OVERRIDE,
  ZCAP_READ_SEARCH,
  ZCAP_FOWNER,
  ZCAP_MAX
} zebra_capabilities_t;

typedef enum
{
  ZPRIVS_LOWERED,
  ZPRIVS_RAISED,
  ZPRIVS_UNKNOWN,
} zebra_privs_current_t;

typedef enum
{
  ZPRIVS_RAISE,
  ZPRIVS_LOWER,
} zebra_privs_ops_t;

struct zebra_privs_t
{
  zebra_capabilities_t *caps_p;       
  zebra_capabilities_t *caps_i;       
  int cap_num_p;                      
  int cap_num_i;
  const char *user;                   
  const char *group;
  const char *vty_group;              
  
  int
    (*change) (zebra_privs_ops_t);    
  zebra_privs_current_t
    (*current_state) (void);          
};

struct zprivs_ids_t
{
  
  uid_t uid_priv;                     
  uid_t uid_normal;                   
  gid_t gid_priv;                     
  gid_t gid_normal;                   
  gid_t gid_vty;                      
};

extern void zprivs_init (struct zebra_privs_t *zprivs);
  
extern void zprivs_terminate (struct zebra_privs_t *);
  
extern void zprivs_get_ids(struct zprivs_ids_t *);

#endif 
