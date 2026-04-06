
#ifndef __CFG_TRUNK_H__
#define __CFG_TRUNK_H__

typedef struct cfg_trunk_s
{
    char trkName[CAPA_LAG_NAME_LEN];
    uint32 hashAlgo;
    sys_logic_portmask_t memberPm[CAPA_LAG_MBR_NORMAL_NUM];
} cfg_trunk_t;

extern int32 cfg_trunk_hashAlgo_set(sys_trunk_hashAlgo_t algo);

extern int32 cfg_trunk_hashAlgo_get(sys_trunk_hashAlgo_t *pAlgo);

extern int32 cfg_trunk_member_set(uint32 trunkId, sys_logic_portmask_t *pMemberPm);

extern int32 cfg_trunk_member_get(uint32 trunkId, sys_logic_portmask_t *pMemberPm);

#endif 

