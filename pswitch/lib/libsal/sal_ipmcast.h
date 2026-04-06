
#ifndef __SAL_IPMCAST_H__
#define __SAL_IPMCAST_H__

#include <rsd/rsd_ipmcast.h>

extern int32 sal_l2McastEntry_getFirst(sys_l2McastEntry_t *pEntry);

extern int32 sal_l2McastEntry_getNext(sys_l2McastEntry_t *pEntry);

extern int32 sal_ipMcastEntry_getFirst(sys_ipMcastEntry_t *pEntry);

extern int32 sal_ipMcastEntry_getNext(sys_ipMcastEntry_t *pEntry);
#endif 

