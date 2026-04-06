
#include <libcfg/cfg.h>
#include <libsal/sal_util.h>
#include <libsal/sal_lacp.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_trunk_chk.h>

const sys_text_t text_trunk_algo[] =
{
    {     "src-port"      },
    {     "src-dst-mac"   },
    {  "src-dst-mac-ip"},
};

int32 sal_trunk_hashAlgo_set(sys_trunk_hashAlgo_t algo)
{
    SYS_ERR_CHK(rsd_trunk_hashAlgo_set(algo));

    SYS_ERR_CHK(cfg_trunk_hashAlgo_set(algo));

    return SYS_ERR_OK;
}

int32 sal_trunk_hashAlgo_get(sys_trunk_hashAlgo_t *pAlgo)
{
    SYS_ERR_CHK(cfg_trunk_hashAlgo_get(pAlgo));

    return SYS_ERR_OK;
}

int32 sal_trunk_activeMember_get(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pPortmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_trunk_activeMember_get(trunkId, pPortmask));

    return SYS_ERR_OK;
}

int32 sal_trunk_group_del(uint32 trunkId)
{
    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);

    if (!IS_TRK_LACP(trunkId))
        sal_trunk_staticGroup_del(trunkId);
#ifdef CONFIG_SYS_PROTO_LACP
    else
        sal_lacp_group_del(trunkId);
#endif

    return SYS_ERR_OK;
}

int32 sal_trunk_staticMember_set(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t oldMbrPm;

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pPortmask), SYS_ERR_NULL_POINTER);

    osal_memset(&oldMbrPm, 0, sizeof(oldMbrPm));

    SYS_ERR_CHK(cfg_trunk_member_get(trunkId, &oldMbrPm));

    if (IS_TRK_LACP(trunkId) && !IS_NORMAL_LOGIC_PORTMASK_CLEAR(oldMbrPm))
        return SYS_ERR_TRUNK_LACP_GROUP;

    SYS_ERR_CHK(sal_trunk_memberNum_check(SYS_PORT_TRUNK_STATIC, pPortmask));

    SYS_ERR_CHK(sal_trunk_valid_check(trunkId, pPortmask));

    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        if (IS_LOGIC_PORTMASK_PORTSET(*pPortmask, logicPort)
                && !IS_LOGIC_PORTMASK_PORTSET(oldMbrPm, logicPort))
        {
           SYS_ERR_CHK(sal_trunk_portRole_change(TRK2LP(trunkId), logicPort));
        }
    }

    SYS_ERR_CHK(rsd_trunk_staticGroup_set(trunkId, pPortmask));
    SYS_ERR_CHK(cfg_trunk_member_set(trunkId, pPortmask));

    FOR_EACH_NORMAL_LOGIC_PORT(logicPort)
    {
        if (IS_LOGIC_PORTMASK_PORTSET(oldMbrPm, logicPort)
                && !IS_LOGIC_PORTMASK_PORTSET(*pPortmask, logicPort))
        {
            SYS_ERR_CHK(sal_trunk_portRole_change(logicPort, logicPort));
        }
    }

    SYS_ERR_CHK(sal_trunk_setting_refresh(trunkId));

    return SYS_ERR_OK;
}

int32 sal_trunk_staticGroup_del(uint32 trunkId)
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t mbrPm;

    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);

    SYS_ERR_CHK(rsd_trunk_staticGroup_del(trunkId));

    osal_memset(&mbrPm, 0, sizeof(mbrPm));
    SYS_ERR_CHK(cfg_trunk_member_get(trunkId, &mbrPm));

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, mbrPm)
    {
        
        SYS_ERR_CHK(sal_trunk_portRole_change(logicPort, logicPort));
    }

    osal_memset(&mbrPm, 0, sizeof(mbrPm));
    SYS_ERR_CHK(cfg_trunk_member_set(trunkId, &mbrPm));

    SYS_ERR_CHK(sal_trunk_setting_refresh(trunkId));

    return SYS_ERR_OK;
}

int32 sal_trunk_member_get(uint32 trunkId, sys_logic_portmask_t *pMemberPm)
{
    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pMemberPm), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_trunk_member_get(trunkId, pMemberPm));

    return SYS_ERR_OK;
}

