
#include <libcfg/cfg.h>

int32 cfg_authmgr_typeEnbl_set(authmgr_type_t type, sys_enable_t enable)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    pAuthmgr->cfg.typeEnbl[type] = enable;

    CFG_CLOSE(CFG_ID_AUTHMGR);
}

int32 cfg_authmgr_typeEnbl_get(authmgr_type_t type, sys_enable_t *pEnable)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    *pEnable = pAuthmgr->cfg.typeEnbl[type];

    CFG_CLOSE(CFG_ID_AUTHMGR);
}

int32 cfg_authmgr_guestVlan_set(sys_vid_t vid)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    pAuthmgr->cfg.guestVlan = vid;

    CFG_CLOSE(CFG_ID_AUTHMGR);
}

int32 cfg_authmgr_guestVlan_get(sys_vid_t *pVid)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    *pVid = pAuthmgr->cfg.guestVlan;

    CFG_CLOSE(CFG_ID_AUTHMGR);
}

#ifdef CONFIG_SYS_PROTO_MACAUTH

int32 cfg_authmgr_radiusUserFmt_set(authmgr_radiusUserFmt_t userFmt)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    osal_memcpy(&pAuthmgr->cfg.radiusUserFmt, &userFmt, sizeof(authmgr_radiusUserFmt_t));

    CFG_CLOSE(CFG_ID_AUTHMGR);
}

int32 cfg_authmgr_radiusUserFmt_get(authmgr_radiusUserFmt_t *pUserFmt)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK((NULL == pUserFmt), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    osal_memcpy(pUserFmt, &pAuthmgr->cfg.radiusUserFmt, sizeof(authmgr_radiusUserFmt_t));

    CFG_CLOSE(CFG_ID_AUTHMGR);
}
#endif

int32 cfg_authmgr_port_set(sys_logic_port_t port, authmgr_cfg_port_t *pCfg)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCfg), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    osal_memcpy(&pAuthmgr->cfg.portCfg[port], pCfg, sizeof(authmgr_cfg_port_t));

    CFG_CLOSE(CFG_ID_AUTHMGR);
}

int32 cfg_authmgr_port_get(sys_logic_port_t port, authmgr_cfg_port_t *pCfg)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCfg), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    osal_memcpy(pCfg, &pAuthmgr->cfg.portCfg[port], sizeof(authmgr_cfg_port_t));

    CFG_CLOSE(CFG_ID_AUTHMGR);
}

#ifdef CONFIG_SYS_PROTO_MACAUTH

int32 cfg_authmgr_macauthLocalInfo_set(uint32 index, authmgr_macauth_local_t localInfo)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK(index >= CAPA_AUTHMGR_MACAUTH_LOCAL_NUM, SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    osal_memcpy(&pAuthmgr->macauthLocal[index], &localInfo, sizeof(authmgr_macauth_local_t));

    CFG_CLOSE(CFG_ID_AUTHMGR);
}

int32 cfg_authmgr_macauthLocalInfo_get(uint32 index, authmgr_macauth_local_t *pLocalInfo)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK(index >= CAPA_AUTHMGR_MACAUTH_LOCAL_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pLocalInfo, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    osal_memcpy(pLocalInfo, &pAuthmgr->macauthLocal[index], sizeof(authmgr_macauth_local_t));

    CFG_CLOSE(CFG_ID_AUTHMGR);
}
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH

int32 cfg_authmgr_webauthLocalInfo_set(uint32 index, authmgr_webauth_local_t localInfo)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK(index >= CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM, SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    osal_memcpy(&pAuthmgr->webauthLocal[index], &localInfo, sizeof(authmgr_webauth_local_t));

    CFG_CLOSE(CFG_ID_AUTHMGR);
}

int32 cfg_authmgr_webauthLocalInfo_get(uint32 index, authmgr_webauth_local_t *pLocalInfo)
{
    cfg_authmgr_t *pAuthmgr = NULL;

    SYS_PARAM_CHK(index >= CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM, SYS_ERR_INPUT);
    SYS_PARAM_CHK(NULL == pLocalInfo, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_AUTHMGR, (cfg_addr)&pAuthmgr);

    osal_memcpy(pLocalInfo, &pAuthmgr->webauthLocal[index], sizeof(authmgr_webauth_local_t));

    CFG_CLOSE(CFG_ID_AUTHMGR);
}
#endif

