
#ifndef __RSD_GARP_H__
#define __RSD_GARP_H__

#include <common/sys_def.h>

extern int32 rsd_garp_init(void);

extern int32 rsd_gmrp_enable_set(sys_enable_t enable);

extern int32 rsd_gmrp_enable_get(sys_enable_t *pEnable);

#endif 

