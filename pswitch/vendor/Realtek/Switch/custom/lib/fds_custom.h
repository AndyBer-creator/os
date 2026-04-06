
#ifndef __FDS_CUSTOM_H__
#define __FDS_CUSTOM_H__

#define DFLT_CUSTOM_GLOBAL_EBL DISABLED
#define DFLT_CUSTOM_PORT_EBL   DISABLED

extern int32 fds_custom_default_init(void);

extern int32 fds_custom_port_restore(sys_logic_port_t port);
#endif

