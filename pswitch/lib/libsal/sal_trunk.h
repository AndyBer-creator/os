
#ifndef __SAL_TRUNK_H__
#define __SAL_TRUNK_H__

#include <rsd/rsd_trunk.h>
#include <libsal/sal_trunk_chk.h>

extern const sys_text_t text_trunk_algo[];

extern int32 sal_trunk_hashAlgo_set(sys_trunk_hashAlgo_t algo);

extern int32 sal_trunk_hashAlgo_get(sys_trunk_hashAlgo_t *pAlgo);

extern int32 sal_trunk_activeMember_get(uint32 trunkId, sys_logic_portmask_t *pPortmask);

extern int32 sal_trunk_group_del(uint32 trunkId);

extern int32 sal_trunk_staticMember_set (uint32 trunkId, sys_logic_portmask_t *pPortmask);

extern int32 sal_trunk_staticGroup_del(uint32 trunkId);

extern int32 sal_trunk_member_get(uint32 trunkId, sys_logic_portmask_t *pMemberPm);

#endif 

