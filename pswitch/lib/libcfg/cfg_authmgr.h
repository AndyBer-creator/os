
#ifndef __CFG_AUTHMGR_H__
#define __CFG_AUTHMGR_H__

#include <libsal/sal_authmgr.h>

typedef struct cfg_authmgr_s
{
    authmgr_cfg_t           cfg;
#ifdef CONFIG_SYS_PROTO_MACAUTH
    authmgr_macauth_local_t macauthLocal[CAPA_AUTHMGR_MACAUTH_LOCAL_NUM];
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    authmgr_webauth_local_t webauthLocal[CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM];
#endif
} cfg_authmgr_t;

extern int32 cfg_authmgr_typeEnbl_set(authmgr_type_t type, sys_enable_t enable);

extern int32 cfg_authmgr_typeEnbl_get(authmgr_type_t type, sys_enable_t *pEnable);

extern int32 cfg_authmgr_guestVlan_set(sys_vid_t vid);

extern int32 cfg_authmgr_guestVlan_get(sys_vid_t *pVid);

#ifdef CONFIG_SYS_PROTO_MACAUTH

extern int32 cfg_authmgr_radiusUserFmt_set(authmgr_radiusUserFmt_t userFmt);

extern int32 cfg_authmgr_radiusUserFmt_get(authmgr_radiusUserFmt_t *pUserFmt);
#endif

extern int32 cfg_authmgr_port_set(sys_logic_port_t port, authmgr_cfg_port_t *pCfg);

extern int32 cfg_authmgr_port_get(sys_logic_port_t port, authmgr_cfg_port_t *pCfg);

#ifdef CONFIG_SYS_PROTO_MACAUTH

extern int32 cfg_authmgr_macauthLocalInfo_set(uint32 index, authmgr_macauth_local_t localInfo);

extern int32 cfg_authmgr_macauthLocalInfo_get(uint32 index, authmgr_macauth_local_t *pLocalInfo);
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH

extern int32 cfg_authmgr_webauthLocalInfo_set(uint32 index, authmgr_webauth_local_t localInfo);

extern int32 cfg_authmgr_webauthLocalInfo_get(uint32 index, authmgr_webauth_local_t *pLocalInfo);
#endif

#endif 

