
#ifndef __RSD_SNMP_H__
#define __RSD_SNMP_H__

#include <common/sys_type.h>
#include <common/sys_def.h>

extern int32 rsd_snmp_systemName_get(char *acName);

extern int32 rsd_snmp_systemName_set(char *acName);

extern int32 rsd_snmp_systemLocation_get(char *acLocation);

extern int32 rsd_snmp_systemLocation_set(char *acLocation);

extern int32 rsd_snmp_systemContact_get(char *acContact);

extern int32 rsd_snmp_systemContact_set(char *acContact);

#endif 

