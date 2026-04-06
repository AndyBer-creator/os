
#include <common/sys_def.h>
#include <gvrp/proto_gvrp.h>
#include <libcfg/cfg.h>
#include <libcfg/cfg_gvrp.h>
#include <libsal/sal_util.h>
#include <libsal/sal_gvrp.h>

#ifdef CONFIG_SYS_MIRROR
#include <libsal/sal_mirror.h>
#endif

int32 sal_gvrp_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_ERR_CHK(gvrp_enable_set(enable));

    SYS_ERR_CHK(cfg_gvrp_enable_set(enable));

    return SYS_ERR_OK;
} 

int32 sal_gvrp_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);
    SYS_ERR_CHK(cfg_gvrp_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_gvrp_timer_set(gvrp_timer_type_t  timer_type, uint32 timeout)
{
    SYS_PARAM_CHK((timer_type >= GVRP_TIMER_END) || (timer_type < GVRP_TIMER_HOLD),
                                   SYS_ERR_INPUT);
    SYS_ERR_CHK(gvrp_timer_set(timer_type, timeout));
    SYS_ERR_CHK(cfg_gvrp_timer_set(timer_type, timeout));

    return SYS_ERR_OK;
}

int32 sal_gvrp_timer_get(gvrp_timer_type_t  timer_type, uint32 *pTimeout)
{
    SYS_PARAM_CHK((timer_type >= GVRP_TIMER_END) || (timer_type < GVRP_TIMER_HOLD),
                                   SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_gvrp_timer_get(timer_type, pTimeout));

    return SYS_ERR_OK;
}

int32 sal_gvrp_all_timer_set(uint32 joinTime, uint32 leaveTime, uint32 leaveAllTime)
{
	if ((joinTime < 20 || joinTime > 16375) || (joinTime % 5 != 0))
	{

		return SYS_ERR_GVRP_TIMER_DIV;
	}
	if (joinTime >= leaveTime/2)
	{

		return SYS_ERR_GVRP_TIMER_JOIN;
	}

	if ((leaveTime < 45 || leaveTime > 32760) || (leaveTime % 5 != 0))
	{

		return SYS_ERR_GVRP_TIMER_DIV;
	}

	if (leaveTime >= leaveAllTime || leaveTime < joinTime * 2)
	{

		return SYS_ERR_GVRP_TIMER_LEAVE;
	}

	if ((leaveAllTime < 65 || leaveAllTime > 32765) || (leaveAllTime % 5 != 0))
	{

		return SYS_ERR_GVRP_TIMER_DIV;
	}
	if (leaveAllTime < leaveTime)
	{

		return SYS_ERR_GVRP_TIMER_LEAVEALL;
	}

    SYS_ERR_CHK(gvrp_timer_set(GVRP_TIMER_JOIN, joinTime));
    SYS_ERR_CHK(cfg_gvrp_timer_set(GVRP_TIMER_JOIN, joinTime));

    SYS_ERR_CHK(gvrp_timer_set(GVRP_TIMER_LEAVE, leaveTime));
    SYS_ERR_CHK(cfg_gvrp_timer_set(GVRP_TIMER_LEAVE, leaveTime));

    SYS_ERR_CHK(gvrp_timer_set(GVRP_TIMER_LEAVEALL, leaveAllTime));
    SYS_ERR_CHK(cfg_gvrp_timer_set(GVRP_TIMER_LEAVEALL, leaveAllTime));

    return SYS_ERR_OK;
}

int32 sal_gvrp_port_statistics_get(sys_logic_port_t port,  gvrp_statistics_t  *pStat)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pStat, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(gvrp_port_statistics_get(port,  pStat));
    return SYS_ERR_OK;
}

int32  sal_gvrp_statistics_clear(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(gvrp_statistics_clear(port));
    return SYS_ERR_OK;
}

int32  sal_gvrp_error_statistics_clear(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(gvrp_error_statistics_clear(port));
    return SYS_ERR_OK;
}

int32 sal_gvrp_port_error_statistics_get(sys_logic_port_t port,  gvrp_error_statistics_t  *pStat)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pStat, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(gvrp_port_error_statistics_get(port,  pStat));
    return SYS_ERR_OK;
}

int32 sal_gvrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
#ifdef CONFIG_SYS_MIRROR
    sys_gid_t gid;
    sys_enable_t mirEnable;
    sys_logic_port_t mirroring_port;
#endif

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    if (ENABLED == enable)
    {
        if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_TRUNK)))
            return SYS_ERR_GVRP_PORT_NOT_TRUNK;
    }

#ifdef CONFIG_SYS_MIRROR
    if(ENABLED == enable)
    {
        FOR_EACH_GID(gid)
        {
            SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid,&mirroring_port,&mirEnable));
            if((ENABLED == mirEnable)&& (port == mirroring_port))
                return SYS_ERR_GVRP_MIRROR_TARGET;
        }
    }
#endif
    SYS_ERR_CHK(gvrp_port_enable_set(port, enable));
    SYS_ERR_CHK(cfg_gvrp_port_enable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_gvrp_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_gvrp_port_enable_get(port, pEnable));
    return SYS_ERR_OK;
}

int32 sal_gvrp_info_get(sys_gvrp_info_t *pInfo)
{
    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(gvrp_info_get(pInfo));

    return SYS_ERR_OK;
}

int32 sal_gvrp_port_regmode_set(sys_logic_port_t port, register_mode_t  regmode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((regmode >= GVRP_MODE_END || regmode < GVRP_NORMAL) , SYS_ERR_INPUT);

    SYS_ERR_CHK(gvrp_port_regmode_set(port, regmode));
    SYS_ERR_CHK(cfg_gvrp_port_regmode_set(port, regmode));

    return SYS_ERR_OK;
}

int32 sal_gvrp_port_regmode_get(sys_logic_port_t port, register_mode_t *pRegMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pRegMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_gvrp_port_regmode_get(port, pRegMode));
    return SYS_ERR_OK;
}

int32 sal_gvrp_port_forbidVlan_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(gvrp_port_forbidVlan_set(port, enable));
    SYS_ERR_CHK(cfg_gvrp_port_forbidVlan_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_gvrp_port_forbidVlan_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_gvrp_port_forbidVlan_get(port, pEnable));
    return SYS_ERR_OK;
}

int32 sal_gvrp_vlan_get(sys_vid_t vid, Gvd *vlandb)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == vlandb), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(gvrp_vlan_db_get(vid, vlandb));
    return SYS_ERR_OK;
}

int32 sal_gvrp_vlan_db_size_get(uint32 *pSize)
{
    SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(gvrp_vlan_db_size_get(pSize));
    return SYS_ERR_OK;
}

int32 sal_gvrp_vlanMaskExist_get(sys_vlanmask_t *pVlanMask)
{
    SYS_PARAM_CHK((NULL == pVlanMask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(gvrp_vlanMaskExist_get(pVlanMask));
    return SYS_ERR_OK;
}

int32 sal_gvrp_gidtt_machine_get(sys_logic_port_t port, sys_vid_t vid, Gid_machine *pMachine)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pMachine), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(gvrp_gidtt_machine_get(port, vid,pMachine));

    return SYS_ERR_OK;
}

