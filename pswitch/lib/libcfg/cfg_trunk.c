
#include <libcfg/cfg.h>

int32 cfg_trunk_hashAlgo_set(sys_trunk_hashAlgo_t algo)
{
    cfg_trunk_t *pTrunk = NULL;

    CFG_OPEN(CFG_ID_TRUNK, (cfg_addr)&pTrunk);

    pTrunk->hashAlgo = algo;

    CFG_CLOSE(CFG_ID_TRUNK);
}

int32 cfg_trunk_hashAlgo_get(sys_trunk_hashAlgo_t *pAlgo)
{
    cfg_trunk_t *pTrunk = NULL;

    SYS_PARAM_CHK((NULL == pAlgo), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_TRUNK, (cfg_addr)&pTrunk);

    *pAlgo = pTrunk->hashAlgo;

    CFG_CLOSE(CFG_ID_TRUNK);
}

int32 cfg_trunk_member_set(uint32 trunkId, sys_logic_portmask_t *pMemberPm)
{
    cfg_trunk_t *pTrunk = NULL;

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pMemberPm), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_TRUNK, (cfg_addr)&pTrunk);

    pTrunk->memberPm[trunkId] = *pMemberPm;

    CFG_CLOSE(CFG_ID_TRUNK);
}

int32 cfg_trunk_member_get(uint32 trunkId, sys_logic_portmask_t *pMemberPm)
{
    cfg_trunk_t *pTrunk = NULL;

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pMemberPm), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_TRUNK, (cfg_addr)&pTrunk);

    *pMemberPm = pTrunk->memberPm[trunkId];

    CFG_CLOSE(CFG_ID_TRUNK);
}

