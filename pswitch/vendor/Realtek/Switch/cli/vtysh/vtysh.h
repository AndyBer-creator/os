
#ifndef VTYSH_H
#define VTYSH_H
#if 0
#define VTYSH_ZEBRA  0x01
#define VTYSH_RIPD   0x02
#define VTYSH_RIPNGD 0x04
#define VTYSH_OSPFD  0x08
#define VTYSH_OSPF6D 0x10
#define VTYSH_BGPD   0x20
#define VTYSH_ISISD  0x40
#define VTYSH_ALL      VTYSH_ZEBRA|VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ISISD
#define VTYSH_RMAP      VTYSH_ZEBRA|VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD
#define VTYSH_INTERFACE      VTYSH_ZEBRA|VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_ISISD
#endif

#define VTYSH_DEFAULT_CONFIG "vtysh.conf"

#ifdef CONFIG_SYS_DB_BOARD
#define VTYSH_CONFIG_MODE_ENTER "configure terminal"
#else 
#define VTYSH_CONFIG_MODE_ENTER "configure"
#endif

#define DIAGDBG_OP_NONE       0
#define DIAGDBG_OP_DEBUG      1
extern int diagdbg_flag;
#define DIAGDBG_AUTH_NONE     0
#define DIAGDBG_AUTH_DEBUG    1
extern int diagdbg_auth;

void vtysh_init_vty (void);
void vtysh_init_cmd (void);

void vtysh_readline_init (void);
void vtysh_user_init (void);

int vtysh_execute (const char *);
int vtysh_execute_no_pager (const char *);

char *vtysh_prompt (void);

void vtysh_config_write (void);

int vtysh_config_from_file (struct vty *, FILE *);

int vtysh_read_config (char *);

void vtysh_config_parse (char *);

void vtysh_config_dump (FILE *);

void vtysh_config_init (void);

void vtysh_pager_init (void);

void vtysh_init_timeout(int);

void vtysh_cmd_loop (void);

void timeout_reset (void);
int readln_timeout_chk(void);
void timeout_clear (void);
int32 login_authen(void);
int enable_authen (int enblPriv);

extern int execute_flag;

extern struct vty *vty;

extern int input_str(char *buf, int size, char echo);
extern int input_str_oversize_break(char *buf, int size, char echo);
extern void cli_exit(void);

#endif 

