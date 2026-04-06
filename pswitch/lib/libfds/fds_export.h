
#ifndef __FDS_EXPORT_H__
#define __FDS_EXPORT_H__

#include <common/sys_type.h>

#ifdef CONFIG_SYS_SNMP
#define SYS_OBJECT_ID      (fds_sys_objid_ret())
#define SYS_OBJECT_ID_LEN  (fds_sys_objid_length_ret())
#endif
#define SYS_FIRMWARE_DATE  (fds_sys_firmwareDate_ret())
#define SYS_FIRMWARE_MAGICNUM  (fds_sys_firmwareMagicNumber_ret())

#ifdef CONFIG_SYS_SNMP

extern const char *fds_sys_objid_ret(void);

extern void fds_sys_objid_array_ret(uint8 *pObjectId);

extern int fds_sys_objid_length_ret(void);
#endif

extern const char *fds_sys_firmwareDate_ret(void);

extern uint32 fds_sys_firmwareMagicNumber_ret(void);

#endif 

