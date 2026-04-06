
#include <libsal/sal_util.h>
#include <libsal/sal_port.h>
#include <libsal/sal_optical.h>
#ifdef CONFIG_SYS_MIRROR
#include <libsal/sal_mirror.h>
#endif

const sys_text_t text_port_speed[] =
{
    {       "10M"       },
    {      "100M"      },
    {     "1000M"     },
    {  "10M-100M"  },
    {       "ALL"       },
};

const sys_text_t text_port_duplex[] =
{
    {       "Full"    },
    {       "Half"    },
};

const sys_text_t text_port_errdisable[] =
{
    {              ""                        },
    {         "bpduguard"               },
#ifdef CONFIG_SYS_PROTO_UDLD
    {              "udld"                    },
#endif
#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
    {          "selfloop"                },
#endif
    {        "broadcast-flood"         },
    {     "unknown-multicast-flood" },
    {     "unicast-flood"           },
    {  "acl"                     },
    {     "psecure-violation"       },
    {   "dhcp-rate-limit"         },
    {    "arp-inspection"          },
};

static int32 _sal_port_speedDuplex_check(
    sys_logic_port_t  port,
    sys_port_speed_t  speed,
    sys_port_duplex_t duplex,
    sys_port_media_t  media);

int32 sal_port_linkChangeTime_get(sys_logic_port_t port, sys_port_linkChangeTime_t *pTime)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_TRK_CHK(rsd_port_linkChangeTime_get(port, pTime));

    return SYS_ERR_OK;
}

int32 sal_port_linkStatus_get(sys_logic_port_t port, sys_port_linkStatus_t *pLink)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLink), SYS_ERR_NULL_POINTER);

    SYS_ERR_TRK_CHK(rsd_port_linkStatus_get(port, pLink));

    return SYS_ERR_OK;
}

int32 sal_port_allLinkStatus_get(sys_port_allLinkStatus_t *pAllLink)
{
    SYS_PARAM_CHK((NULL == pAllLink), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_allLinkStatus_get(pAllLink));

    return SYS_ERR_OK;
}

int32 sal_port_linkMedia_get(sys_logic_port_t port, sys_port_media_t *pMedia)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMedia), SYS_ERR_NULL_POINTER);

    SYS_ERR_TRK_CHK(rsd_port_linkMedia_get(port, pMedia));

    return SYS_ERR_OK;
}

int32 sal_port_comboMediaPrefer_set(sys_logic_port_t port, sys_port_comboMedia_t media)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(rsd_port_comboMediaPrefer_set(port, media));

    return SYS_ERR_OK;
}

int32 sal_port_comboMediaPrefer_get(sys_logic_port_t port, sys_port_comboMedia_t *pMedia)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMedia), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_comboMediaPrefer_get(port, pMedia));

    return SYS_ERR_OK;
}

int32 sal_port_media_get(sys_logic_port_t port, sys_port_media_t *pMedia)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMedia), SYS_ERR_NULL_POINTER);

    SYS_ERR_TRK_CHK(rsd_port_media_get(port, pMedia));

    return SYS_ERR_OK;
}

int32 sal_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
  #ifdef CONFIG_SYS_MIRROR
    sys_gid_t gid;
    sys_logic_port_t dPort;
    sys_enable_t dPortEbl = DISABLED;
    sys_enable_t igrState = DISABLED;
  #endif

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_port_adminEnable_set(port, enable));
    SYS_ERR_CHK(cfg_port_enable_set(port, enable));

  #ifdef CONFIG_SYS_MIRROR
    if(ENABLED == enable)
    {
        FOR_EACH_GID(gid)
        {
            
            SYS_ERR_CHK(sal_mirror_group_targetPort_get(gid, &dPort, &dPortEbl));

            if(ENABLED == dPortEbl && port == dPort)
            {
                SYS_ERR_CHK(sal_mirror_group_igrState_get(gid, &igrState));
                SYS_ERR_CHK(sal_mirror_group_igrState_set(gid, igrState));
            }
        }
    }
  #endif

    return SYS_ERR_OK;
}

int32 sal_port_enableCfg_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_port_enable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_enable_get(port, pEnable));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PORT_ERR_DISABLED

int32 sal_port_errDisable_get(sys_logic_port_t port, sys_port_errDisable_t *pReason)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pReason), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_errDisable_get(port, pReason));

    return SYS_ERR_OK;
}

int32 sal_port_errDisableRecovery_set(sys_port_errDisable_t reason, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_port_errDisableRecovery_set(reason, enable));

    return SYS_ERR_OK;
}

int32 sal_port_errDisableRecovery_get(sys_port_errDisable_t reason, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_errDisableRecovery_get(reason, pEnable));

    return SYS_ERR_OK;
}

int32 sal_port_errDisableTime_set(uint32 time)
{
    SYS_ERR_CHK(rsd_port_errDisableTime_set(time));

    return SYS_ERR_OK;
}

int32 sal_port_errDisableTime_get(uint32 *pTime)
{
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_errDisableTime_get(pTime));

    return SYS_ERR_OK;
}

int32 sal_port_errDisableCurrTime_get(sys_logic_port_t port, uint32 *pCurrTime)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCurrTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_errDisableCurrTime_get(port, pCurrTime));

    return SYS_ERR_OK;
}

int32 sal_port_errDisable_recover(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(rsd_port_errDisable_recover(port));

    return SYS_ERR_OK;
}
#endif 

int32 sal_port_rxEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_port_rxEnable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_port_speedDuplex_get(sys_logic_port_t port, sys_port_speedDuplex_t *pSpeedDuplex)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pSpeedDuplex), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_speedDuplex_get(port, pSpeedDuplex));

    return SYS_ERR_OK;
}

int32 sal_port_fiberAutoNegoEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if(ENABLED == enable)
    {
        speed.autoSpeed = ENABLED;
        speed.ability = SYS_PORT_SPEED_ALL;
        duplex.autoDuplex = ENABLED;
        duplex.ability = SYS_PORT_DUPLEX_FULL;
    }
    else
    {
        speed.autoSpeed = DISABLED;
        speed.ability = SYS_PORT_SPEED_1000M;
        duplex.autoDuplex = DISABLED;
        duplex.ability = SYS_PORT_DUPLEX_FULL;
    }

    SYS_ERR_CHK(sal_port_speed_set(port, speed));
    SYS_ERR_CHK(sal_port_duplex_set(port, duplex));

    SYS_ERR_CHK(cfg_port_fiberAutoNegoEnable_set(port, enable));
    SYS_ERR_CHK(sal_optical_autoSensingConfig_set(port));

    return SYS_ERR_OK;
}

int32 sal_port_fiberAutoNegoEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(cfg_port_fiberAutoNegoEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_port_autoNegoEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    sys_port_speed_t speed;
    sys_port_duplex_t duplex;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_speed_get(port, &speed));
    SYS_ERR_CHK(cfg_port_duplex_get(port, &duplex));

    if ((ENABLED == speed.autoSpeed) || (ENABLED == duplex.autoDuplex))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}

static int32 _sal_port_speedDuplex_check(
    sys_logic_port_t  port,
    sys_port_speed_t  speed,
    sys_port_duplex_t duplex,
    sys_port_media_t  media)
{
    sys_enable_t autoEnable = DISABLED;
    sys_port_conf_t portConf;
    sys_port_media_t currPortMedia = PORT_MEDIA_COPPER;
    sys_port_type_t type;
    sys_logic_port_t rPort;
    sys_logic_portmask_t memberPm;

    osal_memset(&portConf, 0, sizeof(portConf));

    SYS_ERR_CHK(rsd_port_linkMedia_get(port, &currPortMedia));

    type = LP2TYPE(port);
    if(PORT_TYPE_LAG == type) 
    {
        LP2TRKMBRLPM(port, &memberPm);
        if(!IS_LOGIC_PORTMASK_CLEAR(memberPm))
        {
            FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(rPort,memberPm)
                break;

            type = LP2TYPE(rPort);
        }
    }

    if ((ENABLED == speed.autoSpeed) || (ENABLED == duplex.autoDuplex))
        autoEnable = ENABLED;

    if(PORT_TYPE_10G != type && PORT_TYPE_LAG != type)
    {
        if (SYS_PORT_SPEED_10G == speed.ability)
            return SYS_ERR_PORT_SPEED;
    }

    if(PORT_TYPE_100M == type || PORT_TYPE_COMBO_100M == type)
    {
        if (SYS_PORT_SPEED_1000M == speed.ability)
            return SYS_ERR_PORT_1000M_NOT_SUPPORT;
    }

    if ((SYS_PORT_SPEED_1000M == speed.ability)
          && (SYS_PORT_DUPLEX_HALF == duplex.ability))
        return SYS_ERR_PORT_1000M_HALF_NOT_SUPPORT;

    if(PORT_MEDIA_COPPER != currPortMedia)
    {
        SYS_ERR_CHK(sal_sys_boardPortConf_get(port, &portConf));

        if (IS_TGE_PORT(port))
        {

			#if 0
            if (SYS_PORT_SPEED_10G != speed.ability)
                    return SYS_ERR_PORT_FIBER_SPEED;
			#endif
            if (SYS_PORT_DUPLEX_FULL != duplex.ability)
                return SYS_ERR_PORT_FIBER_DUPLEX;
        }
        
        else if ((PORT_MEDIA_FIBER == portConf.mediaType) || (PORT_MEDIA_FIBER == media))
        {
            if (SYS_PORT_DUPLEX_HALF == duplex.ability)
                return SYS_ERR_PORT_FIBER_DUPLEX;

            if (ENABLED == autoEnable)
            {
                if ((SYS_PORT_SPEED_10M == speed.ability)
                        || (SYS_PORT_SPEED_100M == speed.ability)
                        || (SYS_PORT_SPEED_10M_100M == speed.ability))
                    return SYS_ERR_PORT_FIBER_SPEED;
            }
            else
            {
                if ((SYS_PORT_SPEED_10M == speed.ability)
                        || (SYS_PORT_SPEED_10M_100M == speed.ability))
                    return SYS_ERR_PORT_FIBER_SPEED;
            }

            if (PORTCONF_SPEED_FORCE_1000M == portConf.speed)
            {
                if (ENABLED == autoEnable)
                    return SYS_ERR_PORT_AUTO_NEGO_NOT_ALLOW;

                if (SYS_PORT_SPEED_1000M != speed.ability)
                    return SYS_ERR_PORT_FIBER_SPEED;
            }
            else if (PORTCONF_SPEED_AUTO_1000M == portConf.speed)
            {
                if (SYS_PORT_SPEED_1000M != speed.ability)
                    return SYS_ERR_PORT_FIBER_SPEED;
            }
            
            else if (PORTCONF_SPEED_FORCE_100M == portConf.speed)
            {
                if (ENABLED == autoEnable)
                    return SYS_ERR_PORT_AUTO_NEGO_NOT_ALLOW;

                if (SYS_PORT_SPEED_100M != speed.ability)
                    return SYS_ERR_PORT_FIBER_SPEED;
            }
            
            else if (PORTCONF_SPEED_AUTO_ALL == portConf.speed)
            {
                if ((SYS_PORT_SPEED_10M == speed.ability)
                        || (SYS_PORT_SPEED_10M_100M == speed.ability))
                    return SYS_ERR_PORT_FIBER_SPEED;
            }
        }
    }

    return SYS_ERR_OK;
}

int32 sal_port_speedDuplex_set(
    sys_logic_port_t  port,
    sys_port_speed_t  speed,
    sys_port_duplex_t duplex,
    sys_port_media_t  media,
    uint32            isTrkMbr)
{
    int32 ret = SYS_ERR_FAILED;
    uint32 isTrkLpSwitch = FALSE;
    uint32 isComboSwitch = FALSE;
    sys_enable_t autoEnable = DISABLED;
    sys_enable_t fmFlowCtrl = DISABLED, cfgFlowCtrl = DISABLED;
    sys_port_conf_t portConf;
    sys_port_media_t currMedia;
    sys_port_phyAbility_t phyAbility;
    sys_port_speedDuplex_t fmSpeedDuplex;
    sys_port_type_t type;
    sys_logic_port_t rPort;
    sys_logic_portmask_t memberPm;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(_sal_port_speedDuplex_check(port, speed, duplex, PORT_MEDIA_COPPER));

    osal_memset(&portConf, 0, sizeof(portConf));

    SYS_ERR_CHK(sal_sys_boardPortConf_get(port, &portConf));

    if ((ENABLED == speed.autoSpeed) || (ENABLED == duplex.autoDuplex))
        autoEnable = ENABLED;

    if (IS_COPPER_PORT(port) && (PORT_MEDIA_COPPER != media))
    {
        return SYS_ERR_PORT_SET_FIBER_TO_COPPER;
    }

    type = LP2TYPE(port);
    if(PORT_TYPE_LAG == type) 
    {
        LP2TRKMBRLPM(port, &memberPm);
        if(!IS_LOGIC_PORTMASK_CLEAR(memberPm))
        {
            FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(rPort,memberPm)
                break;

            type = LP2TYPE(rPort);
        }
    }

    SYS_ERR_CHK(rsd_port_linkMedia_get(port, &currMedia));

    if (IS_COMBO_PORT(port))
    {
        if (IS_TRKLP(port))
        {
            if (PORT_MEDIA_FIBER == currMedia)
            {
                media = PORT_MEDIA_COPPER;
                isTrkLpSwitch = TRUE;
            }
            else if (PORT_MEDIA_COPPER == currMedia)
            {
                media = PORT_MEDIA_FIBER;
                isTrkLpSwitch = TRUE;
            }
        }

        if (media != currMedia)
        {
            SYS_ERR_CHK(rsd_port_media_set(port, media));
            isComboSwitch = TRUE;
        }
    }

    if(IS_TGE_PORT(port))
    {
		return SYS_ERR_OK;
    }

    SYS_ERR_CHK(cfg_port_flowCtrl_get(port, &cfgFlowCtrl));
    SYS_ERR_CHK((ret = rsd_port_autoNegoEnable_set(port, autoEnable)));

    if (SYS_ERR_OK == ret && ENABLED == autoEnable)
    {
        SYS_ERR_CHK(rsd_port_autoNegoAbility_get(port, &phyAbility));

        phyAbility.Full_10   = FALSE;
        phyAbility.Half_10   = FALSE;
        phyAbility.Full_100  = FALSE;
        phyAbility.Half_100  = FALSE;
        phyAbility.Full_1000 = FALSE;
        phyAbility.Half_1000 = FALSE;
        phyAbility.adv_2_5G  = FALSE;

        switch(speed.ability)
        {
            case SYS_PORT_SPEED_10M:
                if (ENABLED == duplex.autoDuplex)
                {
                    phyAbility.Full_10 = TRUE;
                    phyAbility.Half_10 = TRUE;
                }
                else
                {
                    if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                        phyAbility.Full_10 = TRUE;
                    else
                        phyAbility.Half_10 = TRUE;
                }
                break;

            case SYS_PORT_SPEED_100M:
                if (ENABLED == duplex.autoDuplex)
                {
                    phyAbility.Full_100 = TRUE;
                    phyAbility.Half_100 = TRUE;
                }
                else
                {
                    if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                        phyAbility.Full_100 = TRUE;
                    else
                        phyAbility.Half_100 = TRUE;
                }
                break;

            case SYS_PORT_SPEED_1000M:
                if (ENABLED == duplex.autoDuplex)
                {
                    phyAbility.Full_1000 = TRUE;
                    phyAbility.Half_1000 = TRUE;
                }
                else
                {
                    if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                        phyAbility.Full_1000 = TRUE;
                    else
                    {

                        ret = SYS_ERR_CHIP_NOT_SUPPORTED;
                        goto out;
                    }
                }
                break;

            case SYS_PORT_SPEED_10M_100M:
                if (ENABLED == duplex.autoDuplex)
                {
                    phyAbility.Full_10  = TRUE;
                    phyAbility.Half_10  = TRUE;
                    phyAbility.Full_100 = TRUE;
                    phyAbility.Half_100 = TRUE;
                }
                else
                {
                    if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                    {
                        phyAbility.Full_10  = TRUE;
                        phyAbility.Full_100 = TRUE;
                    }
                    else
                    {
                        phyAbility.Half_10  = TRUE;
                        phyAbility.Half_100 = TRUE;
                    }
                }
                break;
            case SYS_PORT_SPEED_2500M:
                if (ENABLED == duplex.autoDuplex)
                {
                    phyAbility.adv_2_5G = TRUE;
                }
                else
                {
                    if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                        phyAbility.adv_2_5G = TRUE;
                    else
                    {
                        ret = SYS_ERR_CHIP_NOT_SUPPORTED;
                        goto out;
                    }
                }
                break;

            case SYS_PORT_SPEED_ALL:
                if (ENABLED == duplex.autoDuplex)
                {
                    phyAbility.Full_10   = TRUE;
                    phyAbility.Half_10   = TRUE;
                    phyAbility.Full_100  = TRUE;
                    phyAbility.Half_100  = TRUE;
                    if(PORT_TYPE_COMBO_100M != type && PORT_TYPE_100M != type)
                    {
                        phyAbility.Full_1000 = TRUE;
                        phyAbility.Half_1000 = TRUE;
                        phyAbility.adv_2_5G = TRUE;
                    }
                }
                else
                {
                    if (SYS_PORT_DUPLEX_FULL == duplex.ability)
                    {
                        phyAbility.Full_10   = TRUE;
                        phyAbility.Full_100  = TRUE;
                        if(PORT_TYPE_COMBO_100M != type && PORT_TYPE_100M != type)
                        {
                            phyAbility.Full_1000 = TRUE;
                            phyAbility.adv_2_5G = TRUE;
                        }
                    }
                    else
                    {
                        phyAbility.Half_10   = TRUE;
                        phyAbility.Half_100  = TRUE;
                        if(PORT_TYPE_COMBO_100M != type && PORT_TYPE_100M != type)
                        {
                            phyAbility.Half_1000 = TRUE;
                            phyAbility.adv_2_5G = TRUE;
                        }
                    }
                }
                break;

            default:
                break;
        }

        if (IS_TRKLP(port) || (TRUE == isTrkMbr))
        {
            phyAbility.Half_10   = FALSE;
            phyAbility.Half_100  = FALSE;
            phyAbility.Half_1000 = FALSE;
        }

        phyAbility.Half_1000 = FALSE;

        phyAbility.FC = phyAbility.AsyFC = cfgFlowCtrl;
        ret = rsd_port_autoNegoAbility_set(port, &phyAbility);

        if (SYS_ERR_PORT_AUTO_SPEED == ret)
        {
            
            if (SYS_PORT_SPEED_ALL == speed.ability)
            {
                phyAbility.Full_1000 = FALSE;
                phyAbility.Half_1000 = FALSE;
                phyAbility.adv_2_5G = FALSE;
                ret = rsd_port_autoNegoAbility_set(port, &phyAbility);
            }
        }

        if (TRUE == isComboSwitch)
        {
            SYS_ERR_CHK(rsd_port_media_set(port, currMedia));

            if (TRUE == isTrkLpSwitch)
            {
                ret = rsd_port_autoNegoEnable_set(port, autoEnable);
                ret = rsd_port_autoNegoAbility_set(port, &phyAbility);

                if (SYS_ERR_PORT_AUTO_SPEED == ret
                    && SYS_PORT_SPEED_ALL == speed.ability)
                {
                    
                    phyAbility.Full_1000 = FALSE;
                    phyAbility.Half_1000 = FALSE;
                    ret = rsd_port_autoNegoAbility_set(port, &phyAbility);
                }
            }
        }

        return ret;
    }
    else
    {
        if (DISABLED == autoEnable)
        {
            SYS_ERR_CHK((ret = rsd_port_forceModeAbility_get(port, &fmSpeedDuplex, &fmFlowCtrl)));

            if (SYS_ERR_OK == ret)
            {
                switch (speed.ability)
                {
                    case SYS_PORT_SPEED_10M:
                        fmSpeedDuplex.speed = PORT_SPEED_10M;
                        break;

                    case SYS_PORT_SPEED_100M:
                        fmSpeedDuplex.speed = PORT_SPEED_100M;
                        break;

                    case SYS_PORT_SPEED_1000M:
                        fmSpeedDuplex.speed = PORT_SPEED_1000M;
                        break;
                        
                    case SYS_PORT_SPEED_2500M:
                        fmSpeedDuplex.speed = PORT_SPEED_2_5G;
                        break;

                    default:
                        fmSpeedDuplex.speed = PORT_SPEED_100M;
                        break;
                }

                switch(duplex.ability)
                {
                    case SYS_PORT_DUPLEX_FULL:
                        fmSpeedDuplex.duplex = PORT_FULL_DUPLEX;
                        break;

                    case SYS_PORT_DUPLEX_HALF:
                        fmSpeedDuplex.duplex = PORT_HALF_DUPLEX;
                        break;

                    default:
                        fmSpeedDuplex.duplex = PORT_FULL_DUPLEX;
                        break;
                }

                if (IS_TRKLP(port) || (TRUE == isTrkMbr))
                {
                    fmSpeedDuplex.duplex = PORT_FULL_DUPLEX;
                }

                ret = rsd_port_forceModeAbility_set(port, &fmSpeedDuplex, cfgFlowCtrl);

                if (IS_COMBO_PORT(port))
                {
                    if (PORT_MEDIA_FIBER == media)
                        ret = rsd_port_autoNegoEnable_set(port, autoEnable);
                }
            }

            if (TRUE == isComboSwitch)
            {
                SYS_ERR_CHK(rsd_port_media_set(port, currMedia));

                if (TRUE == isTrkLpSwitch)
                {
                    ret = rsd_port_autoNegoEnable_set(port, autoEnable);
                    ret = rsd_port_forceModeAbility_set(port, &fmSpeedDuplex, cfgFlowCtrl);

                    if (PORT_MEDIA_FIBER == currMedia)
                        ret = rsd_port_autoNegoEnable_set(port, autoEnable);
                }
            }

            return ret;
        }
        else
        {
            
            ret = SYS_ERR_PORT_AUTO_NEGO;
        }
    }

out:
    
    if (TRUE == isComboSwitch)
        SYS_ERR_CHK(rsd_port_media_set(port, currMedia));

    return SYS_ERR_OK;
}

int32 sal_port_speed_set(sys_logic_port_t port, sys_port_speed_t speed)
{
    int32 ret = SYS_ERR_FAILED;
    sys_enable_t fmFlowCtrl;
    sys_port_conf_t portConf;
    sys_port_speed_t tmpSpeed;
    sys_port_duplex_t duplex;
    sys_port_speedDuplex_t fmSpeedDuplex;
    sys_port_media_t currMedia;
    sys_port_speed_t fiberSpeed;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    osal_memset(&fiberSpeed, 0, sizeof(fiberSpeed));

    osal_memset(&portConf, 0, sizeof(portConf));
    ret = sal_sys_boardPortConf_get(port, &portConf);

    if (SYS_ERR_TRUNK_EMPTY == ret)
        goto cfg_save;

    if (PORT_MEDIA_FIBER == portConf.mediaType)
    {
        duplex.autoDuplex = DISABLED;
        duplex.ability = SYS_PORT_DUPLEX_FULL;

        SYS_ERR_CHK(cfg_port_duplex_set(port, duplex));
        if (IS_COMBO_PORT(port) && !IS_TRKLP(port))
        {
	        if(speed.ability == SYS_PORT_SPEED_100M)
	    	{
	    	    fiberSpeed.autoSpeed = DISABLED;
	            fiberSpeed.ability = SYS_PORT_SPEED_100M;
				SYS_ERR_CHK(sal_port_combo_fiberSpeed_set(port, fiberSpeed));
				goto cfg_save;
	    	}
	    	else if(speed.ability == SYS_PORT_SPEED_1000M)
	    	{
	    	    fiberSpeed.autoSpeed = DISABLED;
	            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
				SYS_ERR_CHK(sal_port_combo_fiberSpeed_set(port, fiberSpeed));
				goto cfg_save;
				
	    	}
    	}
    }
    else
    {
        SYS_ERR_CHK(cfg_port_duplex_get(port, &duplex));
    }

    if (IS_COMBO_PORT(port) && !IS_TRKLP(port))
    {
        ret = rsd_port_media_get(port, &currMedia);
        if ((IS_LP_TRKMBR(port))
            || ((SYS_ERR_OK == ret) && (PORT_MEDIA_FIBER == currMedia)))
        {
            SYS_ERR_CHK(_sal_port_speedDuplex_check(port, speed, duplex, PORT_MEDIA_COPPER));
            goto cfg_save;
        }
    }
    
    if(portConf.speed == PORTCONF_SPEED_FORCE_COMBO_100M && speed.ability != SYS_PORT_SPEED_ALL)
    {
    	if(speed.ability == SYS_PORT_SPEED_100M)
    	{
    	    fiberSpeed.autoSpeed = DISABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_100M;
			SYS_ERR_CHK(sal_port_combo_fiberSpeed_set(port, fiberSpeed));
    	}
    	else if(speed.ability == SYS_PORT_SPEED_1000M)
    	{
    	    fiberSpeed.autoSpeed = DISABLED;
            fiberSpeed.ability = SYS_PORT_SPEED_1000M;
			SYS_ERR_CHK(sal_port_combo_fiberSpeed_set(port, fiberSpeed));
    	}
    	else 
    	{
			return SYS_ERR_PORT_FIBER_SPEED;
    	}
    	goto cfg_save;
    }
    
    if (IS_TGE_PORT(port))
    {
    	if(speed.autoSpeed)
    	{
    		if(speed.ability == SYS_PORT_SPEED_ALL)
    		{
				goto cfg_save;
			}
			else 
			{
				return SYS_ERR_PORT_FIBER_SPEED;
			}
    	}
    	else 
    	{
	    	
	    	switch(speed.ability)
	    	{
				case SYS_PORT_SPEED_10G:
					SYS_ERR_CHK(sal_port_10gMedia_set(port, PORT_10GMEDIA_FIBER_10G));
					break;
				case SYS_PORT_SPEED_2500M:
					SYS_ERR_CHK(sal_port_10gMedia_set(port, PORT_10GMEDIA_FIBER_2_5G));
					break;	
				case SYS_PORT_SPEED_1000M:
					SYS_ERR_CHK(sal_port_10gMedia_set(port, PORT_10GMEDIA_FIBER_1G));
					break;	
				case SYS_PORT_SPEED_100M:
					SYS_ERR_CHK(sal_port_10gMedia_set(port, PORT_10GMEDIA_FIBER_100M));
					break;				
				case SYS_PORT_SPEED_10G_DAC_50:
					SYS_ERR_CHK(sal_port_10gMedia_set(port, PORT_10GMEDIA_DAC_50CM));
					break;
				case SYS_PORT_SPEED_10G_DAC_100:
					SYS_ERR_CHK(sal_port_10gMedia_set(port, PORT_10GMEDIA_DAC_100CM));
					break;
				case SYS_PORT_SPEED_10G_DAC_300:
					SYS_ERR_CHK(sal_port_10gMedia_set(port, PORT_10GMEDIA_DAC_300CM));
					break;
				case SYS_PORT_SPEED_10G_DAC_500:
					SYS_ERR_CHK(sal_port_10gMedia_set(port, PORT_10GMEDIA_DAC_500CM));
					break;
				default:
					return SYS_ERR_PORT_FIBER_SPEED;
	    	}
    	}
		goto cfg_save;
    }

    ret = sal_port_speedDuplex_set(port, speed, duplex, PORT_MEDIA_COPPER, FALSE);

    if ((SYS_ERR_OK == ret) || (SYS_ERR_TRUNK_EMPTY == ret) || (SYS_ERR_TRUNK_MEMBER_SKIP == ret))
    {
        goto cfg_save;
    }
    else if (SYS_ERR_PORT_AUTO_NEGO == ret)
    {
        
        SYS_ERR_CHK((ret = rsd_port_forceModeAbility_get(port, &fmSpeedDuplex, &fmFlowCtrl)));

        if (SYS_ERR_OK == ret)
        {
            fmSpeedDuplex.speed = PORT_SPEED_100M;
            fmSpeedDuplex.duplex = PORT_FULL_DUPLEX;
            SYS_ERR_CHK(rsd_port_forceModeAbility_set(port, &fmSpeedDuplex, fmFlowCtrl));

            tmpSpeed.autoSpeed = DISABLED;
            tmpSpeed.ability = SYS_PORT_SPEED_100M;
            duplex.autoDuplex = DISABLED;
            duplex.ability = SYS_PORT_DUPLEX_FULL;
            SYS_ERR_CHK(cfg_port_speed_set(port, tmpSpeed));
            SYS_ERR_CHK(cfg_port_duplex_set(port, duplex));
        }
    }

    return ret;

cfg_save:
    SYS_ERR_CHK(cfg_port_speed_set(port, speed));

    return SYS_ERR_OK;
}

int32 sal_port_10gMedia_set(sys_logic_port_t port, sys_port_10gMedia_t xgMedia)
{

    SYS_ERR_CHK(rsd_port_10gMedia_set(port, xgMedia));
	SYS_ERR_CHK(rsd_port_fiber_force_set(port, ENABLED));

    return SYS_ERR_OK;
}

int32 sal_port_speed_get(sys_logic_port_t port, sys_port_speed_t *pSpeed)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pSpeed), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_speed_get(port, pSpeed));

    return SYS_ERR_OK;
}

int32 sal_port_duplex_set(sys_logic_port_t port, sys_port_duplex_t duplex)
{
    int32 ret = SYS_ERR_FAILED;
    sys_enable_t fmFlowCtrl;
    sys_port_speed_t speed;
    sys_port_duplex_t tmpDuplex;
    sys_port_speedDuplex_t fmSpeedDuplex;
    sys_port_media_t currMedia;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if (IS_TRKLP(port) && (SYS_PORT_DUPLEX_FULL != duplex.ability))
    {
        return SYS_ERR_PORT_DUPLEX;
    }

    SYS_ERR_CHK(cfg_port_speed_get(port, &speed));

    if (IS_COMBO_PORT(port) && !IS_TRKLP(port))
    {
        ret = rsd_port_media_get(port, &currMedia);
        if ((IS_LP_TRKMBR(port)) ||
            ((SYS_ERR_OK == ret) && (PORT_MEDIA_FIBER == currMedia)))
        {
            SYS_ERR_CHK(_sal_port_speedDuplex_check(port, speed, duplex, PORT_MEDIA_COPPER));
            goto cfg_save;
        }
    }

    ret = sal_port_speedDuplex_set(port, speed, duplex, PORT_MEDIA_COPPER, FALSE);

    if ((SYS_ERR_OK == ret) || (SYS_ERR_TRUNK_EMPTY == ret) || (SYS_ERR_TRUNK_MEMBER_SKIP == ret))
    {
        goto cfg_save;
    }
    else if (SYS_ERR_PORT_AUTO_NEGO == ret)
    {
        
        SYS_ERR_CHK((ret = rsd_port_forceModeAbility_get(port, &fmSpeedDuplex, &fmFlowCtrl)));

        if (SYS_ERR_OK == ret)
        {
            fmSpeedDuplex.speed = PORT_SPEED_100M;
            fmSpeedDuplex.duplex = PORT_FULL_DUPLEX;
            SYS_ERR_CHK(rsd_port_forceModeAbility_set(port, &fmSpeedDuplex, fmFlowCtrl));

            speed.autoSpeed = DISABLED;
            speed.ability = SYS_PORT_SPEED_100M;
            tmpDuplex.autoDuplex = DISABLED;
            tmpDuplex.ability = SYS_PORT_DUPLEX_FULL;
            SYS_ERR_CHK(cfg_port_speed_set(port, speed));
            SYS_ERR_CHK(cfg_port_duplex_set(port, tmpDuplex));
        }
    }

    return ret;

cfg_save:
    SYS_ERR_CHK(cfg_port_duplex_set(port, duplex));

    return SYS_ERR_OK;
}

int32 sal_port_duplex_get(sys_logic_port_t port, sys_port_duplex_t *pDuplex)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pDuplex), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_duplex_get(port, pDuplex));

    return SYS_ERR_OK;
}

int32 sal_port_fiberSpeed_set(sys_logic_port_t port, sys_port_speed_t speed)
{
    int32 ret = SYS_ERR_FAILED;
    sys_enable_t fmFlowCtrl;
    sys_port_speed_t tmpSpeed;
    sys_port_duplex_t duplex;
    sys_port_speedDuplex_t fmSpeedDuplex;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    duplex.autoDuplex = DISABLED;
    duplex.ability = SYS_PORT_DUPLEX_FULL;

    ret = sal_port_speedDuplex_set(port, speed, duplex, PORT_MEDIA_FIBER, FALSE);

    if ((SYS_ERR_OK == ret) || (SYS_ERR_TRUNK_EMPTY == ret) || (SYS_ERR_TRUNK_MEMBER_SKIP == ret))
    {
        goto cfg_save;
    }
    else if (SYS_ERR_PORT_AUTO_NEGO == ret)
    {
        
        SYS_ERR_CHK((ret = rsd_port_forceModeAbility_get(port, &fmSpeedDuplex, &fmFlowCtrl)));

        if (SYS_ERR_OK == ret)
        {
            fmSpeedDuplex.speed = PORT_SPEED_100M;
            fmSpeedDuplex.duplex = PORT_FULL_DUPLEX;
            SYS_ERR_CHK(rsd_port_forceModeAbility_set(port, &fmSpeedDuplex, fmFlowCtrl));

            tmpSpeed.autoSpeed = DISABLED;
            tmpSpeed.ability = SYS_PORT_SPEED_100M;
            SYS_ERR_CHK(cfg_port_fiberSpeed_set(port, tmpSpeed));
        }
    }
  
    return ret;

cfg_save:
    SYS_ERR_CHK(cfg_port_fiberSpeed_set(port, speed));

    return SYS_ERR_OK;
}

int32 sal_port_combo_fiberSpeed_set(sys_logic_port_t port,sys_port_speed_t speed)
{   
    if(speed.ability == SYS_PORT_SPEED_1000M)
       rsd_port_combo_1g_fiber_speed_set_new( port, PORT_COMBO_FIBER_1G_MEDIA_1000);
    if(speed.ability == SYS_PORT_SPEED_100M)
       rsd_port_combo_1g_fiber_speed_set_new( port, PORT_COMBO_FIBER_1G_MEDIA_100);  
    SYS_ERR_CHK(cfg_port_fiberSpeed_set(port, speed));

    return SYS_ERR_OK;
}

int32 sal_port_fiberSpeed_get(sys_logic_port_t port, sys_port_speed_t *pSpeed)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pSpeed), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_fiberSpeed_get(port, pSpeed));

    return SYS_ERR_OK;
}

int32 sal_port_flowCtrlAuto_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if(IS_COMBO_PORT(port) || IS_FIBER_PORT(port))
    {
        if(ENABLED == enable)
            return SYS_ERR_PORT_AUTO_FLOW_NOT_ALLOW_COMBO_FIBER;
    }

    SYS_ERR_TRK_CHK(rsd_port_flowCtrlAuto_set(port, enable));
    SYS_ERR_CHK(cfg_port_flowCtrlAuto_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_port_flowCtrlAuto_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_flowCtrlAuto_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_port_flowCtrl_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_port_flowCtrl_set(port, enable));

    SYS_ERR_CHK(cfg_port_flowCtrl_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_port_flowCtrl_get(sys_logic_port_t port, sys_enable_t *pEnable, sys_enable_t *pStatus)
{
    sys_port_linkStatus_t linkStatus;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_flowCtrl_get(port, pEnable));

    SYS_ERR_TRK_CHK(rsd_port_linkStatus_get(port, &linkStatus));

    if (PORT_LINKUP == linkStatus)
        SYS_ERR_TRK_CHK(rsd_port_flowCtrl_get(port, pStatus));
    else
        *pStatus = DISABLED;

    return SYS_ERR_OK;
}

int32 sal_port_flowCtrlStatus_get(
    sys_logic_port_t port,
    sys_enable_t *pTxStatus,
    sys_enable_t *pRxStatus)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pRxStatus), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pTxStatus), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_flowCtrlStatus_get(port, pTxStatus, pRxStatus));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PORT_ISOLATION

int32 sal_port_protected_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_port_protected_set(port, enable));

    SYS_ERR_CHK(cfg_port_protected_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_port_protected_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_protected_get(port, pEnable));

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_PORT_DESCRIPTION

int32 sal_port_descp_set(sys_logic_port_t port, char *pDescp)
{
    char tmpStr[CAPA_PORTDESC_STR_LEN + 1];

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pDescp), SYS_ERR_NULL_POINTER);

    if (SYS_ERR_OK != STRING_VALIDATE(pDescp, TRUE))
        return SYS_ERR_STRING;

    osal_memset(tmpStr, 0, sizeof(tmpStr));

    if (osal_strlen(pDescp) > CAPA_PORTDESC_STR_LEN)
    {
        osal_strncpy(tmpStr, pDescp, CAPA_PORTDESC_STR_LEN);
        tmpStr[CAPA_PORTDESC_STR_LEN] = '\0';

        SYS_ERR_CHK(rsd_port_descp_set(port, tmpStr));
        SYS_ERR_CHK(cfg_port_descp_set(port, tmpStr));
    }
    else
    {
        SYS_ERR_CHK(rsd_port_descp_set(port, pDescp));
        SYS_ERR_CHK(cfg_port_descp_set(port, pDescp));
    }

    return SYS_ERR_OK;
}

int32 sal_port_descp_get(sys_logic_port_t port, char *pDescp)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pDescp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_descp_get(port, pDescp));

    return SYS_ERR_OK;
}
#endif 

int32 sal_port_maxL2Entry_get(uint32 dev, uint32 *pMaxL2Entry)
{
    SYS_PARAM_CHK(IS_DEV_INVALID(dev), SYS_ERR_DEV_ID);
    SYS_PARAM_CHK((NULL == pMaxL2Entry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_devMaxL2Cnt_get(dev, pMaxL2Entry));

    return SYS_ERR_OK;
}

int32 sal_port_backpressureEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_port_backpressureEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_port_backpressureEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_TRK_CHK(rsd_port_backpressureEnable_set(port, enable));

    SYS_ERR_CHK(cfg_port_backpressureEnable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_port_logic2Physic_get(sys_logic_port_t port, sys_physic_port_t *pPhysicPort)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPhysicPort), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_logic2Physic_get(port, pPhysicPort));

    return SYS_ERR_OK;
}

int32 sal_port_physic2Logic_get(sys_physic_port_t physicPort, sys_logic_port_t *pPort)
{
    SYS_PARAM_CHK((NULL == pPort), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_port_physic2Logic_get(physicPort, pPort));

    return SYS_ERR_OK;
}

