
#include <libsal/sal_util.h>
#include <libsal/sal_authmgr.h>

const sys_text_t text_authmgr_type[] =
{
    {    ""      },
#ifdef CONFIG_SYS_PROTO_DOT1X
    {   "dot1x" },
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    {     "mac"   },
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    {     "web"   },
#endif
};

#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
const sys_text_t text_authmgr_method[] =
{
    {    ""        },
    {  "radius"  },
    {   "local"   },
};
#endif

const sys_text_t text_authmgr_mode[] =
{
    {   "multi-auth"  },
    {   "multi-host"  },
    {  "single-host" },
};

const sys_text_t text_authmgr_ctrlType[] =
{
    {              "disable"    },
    {    "force-auth"    },
    {  "force-unauth"  },
    {                  "auto"          },
};

const sys_text_t text_authmgr_vlanAssign[] =
{
    {   "disable"  },
    {    "reject"   },
    {    "static"   },
};

const sys_text_t text_authmgr_status[] =
{
    {   "Disable"      },
    {      "Running"      },
    {    "Authorized"   },
    {  "UnAuthorized" },
    {    "Locked"       },
    {     "Guest"        },
};

const sys_text_t text_authmgr_authorSrc[] =
{
    {     "Config"   },
    {   "Loacl"    },
    {  "RADIUS"   },
};

int32 sal_authmgr_typeEnbl_set(authmgr_type_t type, sys_enable_t enable)
{
    sys_enable_t orgEnbl = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_authmgr_typeEnbl_get(type, &orgEnbl));

    if (orgEnbl != enable)
    {
        SYS_ERR_CHK(cfg_authmgr_typeEnbl_set(type, enable));
        SYS_ERR_CHK(authmgr_typeEnbl_set(type, enable));

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        if (AUTHMGR_TYPE_WEB == type)
        {

			system("handler -w httpServer 1> /dev/null 2> /dev/null &");
            SYS_ERR_CHK(sal_authmgr_webauthIpv6Neigh_refresh());
        }
#endif
    }

    return SYS_ERR_OK;
}

int32 sal_authmgr_typeEnbl_get(authmgr_type_t type, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_authmgr_typeEnbl_get(type, pEnable));

    return SYS_ERR_OK;
}

int32 sal_authmgr_guestVlan_set(sys_vid_t vid)
{
    SYS_ERR_CHK(sal_vlan_guestVid_set(vid));
    SYS_ERR_CHK(authmgr_guestVlan_set(vid));
    SYS_ERR_CHK(cfg_authmgr_guestVlan_set(vid));

    return SYS_ERR_OK;
}

int32 sal_authmgr_guestVlan_get(sys_vid_t *pVid)
{
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_authmgr_guestVlan_get(pVid));

    return SYS_ERR_OK;
}

int32 sal_authmgr_guestVlanPortmask_get(sys_logic_portmask_t *pLpm)
{
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    authmgr_cfg_port_t authPortCfg;

    SYS_PARAM_CHK((NULL == pLpm), SYS_ERR_NULL_POINTER);

    LOGIC_PORTMASK_CLEAR_ALL(lpm);

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(authPortCfg);
        SYS_ERR_CHK(cfg_authmgr_port_get(lp, &authPortCfg));

        if (ENABLED == authPortCfg.guestVlanEnbl)
            LOGIC_PORTMASK_SET_PORT(lpm, lp);
    }

    LOGIC_PORTMASK_COPY(*pLpm, lpm);

    return SYS_ERR_OK;
}

int32 sal_authmgr_portGuestVlanEnbl_set(sys_logic_port_t port, sys_enable_t enable)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.guestVlanEnbl = enable;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portGuestVlanEnbl_get(sys_logic_port_t port, sys_enable_t *pEable)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pEable = portCfg.guestVlanEnbl;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portCfg_set(sys_logic_port_t port, authmgr_cfg_port_t *pPortCfg)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPortCfg), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_authmgr_port_set(port, pPortCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, pPortCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portCfg_get(sys_logic_port_t port, authmgr_cfg_port_t *pPortCfg)
{
    SYS_PARAM_CHK((NULL == pPortCfg), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_authmgr_port_get(port, pPortCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portCtrl_set(sys_logic_port_t port, authmgr_portCtrl_t ctrl)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if (AUTHMGR_PORTCTRL_DISABLE != ctrl)
    {
        if(IS_LP_TRKMBR(port))
            return SYS_ERR_AUTH_PORT_TRUNKMBR;
    }

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.ctrlType = ctrl;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    if (AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED == ctrl)
    {
        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_FORCE_UNAUTH,
                    LPSTR(port));
    }
    else if (AUTHMGR_PORTCTRL_FORCE_AUTHORIZED == ctrl)
    {
        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_FORCE_AUTH,
                    LPSTR(port));
    }

    return SYS_ERR_OK;
}

int32 sal_authmgr_portCtrl_get(sys_logic_port_t port, authmgr_portCtrl_t *pCtrl)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCtrl), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pCtrl = portCfg.ctrlType;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portMode_set(sys_logic_port_t port, authmgr_mode_t mode)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    if (AUTHMGR_MODE_MULTI_HOST == mode)
    {
        if (ENABLED == portCfg.typeEnbl[AUTHMGR_TYPE_WEB])
            return SYS_ERR_WEBAUTH_MULTIHOST_WEBAUTH;
    }
#endif

    portCfg.mode = mode;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portMode_get(sys_logic_port_t port, authmgr_mode_t *pMode)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pMode = portCfg.mode;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portOrder_set(sys_logic_port_t port, authmgr_order_t order)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    osal_memcpy(&portCfg.typeOrder, &order, sizeof(authmgr_order_t));

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portOrder_get(sys_logic_port_t port, authmgr_order_t *pOrder)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pOrder), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    osal_memcpy(pOrder, &portCfg.typeOrder, sizeof(authmgr_order_t));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portMethodOrder_set(sys_logic_port_t port, authmgr_methodOrder_t order)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    osal_memcpy(&portCfg.methodOrder, &order, sizeof(authmgr_methodOrder_t));

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portMethodOrder_get(sys_logic_port_t port, authmgr_methodOrder_t *pOrder)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pOrder), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    osal_memcpy(pOrder, &portCfg.methodOrder, sizeof(authmgr_methodOrder_t));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portTypeEnbl_set(sys_logic_port_t port, authmgr_type_t type, sys_enable_t enable)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    if (AUTHMGR_TYPE_WEB == type && ENABLED == enable)
    {
        if (AUTHMGR_MODE_MULTI_HOST == portCfg.mode)
            return SYS_ERR_WEBAUTH_WEBAUTH_MULTIHOST;
    }
#endif

    portCfg.typeEnbl[type] = enable;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portTypeEnbl_get(sys_logic_port_t port, authmgr_type_t type, sys_enable_t *pEnable)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pEnable = portCfg.typeEnbl[type];

    return SYS_ERR_OK;
}

int32 sal_authmgr_portReauthEnbl_set(sys_logic_port_t port, sys_enable_t enable)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.reauthEnbl = enable;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portReauthEnbl_get(sys_logic_port_t port, sys_enable_t *pEable)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pEable = portCfg.reauthEnbl;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portMaxHost_set(sys_logic_port_t port, uint32 maxHost)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((maxHost < BOND_AUTHMGR_MAX_HOST_MIN) || (maxHost > BOND_AUTHMGR_MAX_HOST_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.maxHosts = maxHost;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portMaxHost_get(sys_logic_port_t port, uint32 *pMaxHost)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMaxHost), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pMaxHost = portCfg.maxHosts;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portVlanAssign_set(sys_logic_port_t port, authmgr_vlanAssign_t mode)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.vlanAssignMode = mode;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portVlanAssign_get(sys_logic_port_t port, authmgr_vlanAssign_t *pMode)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pMode = portCfg.vlanAssignMode;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portReauthPeriod_set(sys_logic_port_t port, uint32 time)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((time < BOND_AUTHMGR_REAUTH_PERIOD_MIN) || (time > BOND_AUTHMGR_REAUTH_PERIOD_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.reauthPeriod = time;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portReauthPeriod_get(sys_logic_port_t port, uint32 *pTime)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pTime = portCfg.reauthPeriod;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portInactTimeout_set(sys_logic_port_t port, uint32 time)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((time < BOND_AUTHMGR_INACT_TIMEOUT_MIN) || (time > BOND_AUTHMGR_INACT_TIMEOUT_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.inactTimeout = time;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portInactTimeout_get(sys_logic_port_t port, uint32 *pTime)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pTime = portCfg.inactTimeout;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portQuietPeriod_set(sys_logic_port_t port, uint32 time)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((time < BOND_AUTHMGR_QUIET_PERIOD_MIN) || (time > BOND_AUTHMGR_QUIET_PERIOD_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.quietPeriod = time;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portQuietPeriod_get(sys_logic_port_t port, uint32 *pTime)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pTime = portCfg.quietPeriod;

    return SYS_ERR_OK;
}

int32 sal_authmgr_sessCnt_get(uint32 *pCnt)
{
    SYS_PARAM_CHK((NULL == pCnt), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(authmgr_sessCnt_get(pCnt));

    return SYS_ERR_OK;
}

int32 sal_authmgr_sessInfo_getByIdx(uint32 idx, authmgr_sess_info_t *pSessInfo)
{
    SYS_PARAM_CHK((NULL == pSessInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(authmgr_sessInfo_getByIdx(idx, pSessInfo));

    return SYS_ERR_OK;
}

int32 sal_authmgr_sess_clear(authmgr_sess_clear_t *pSessClear)
{
    SYS_PARAM_CHK((NULL == pSessClear), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(authmgr_sess_clear(pSessClear));

    return SYS_ERR_OK;
}

