
#ifndef __SAL_AUTHMGR_MACAUTH_H__
#define __SAL_AUTHMGR_MACAUTH_H__

extern int32 sal_authmgr_radiusUserFmt_set(authmgr_radiusUserFmt_t userFmt);

extern int32 sal_authmgr_radiusUserFmt_get(authmgr_radiusUserFmt_t *pUserFmt);

extern int32 sal_authmgr_radiusUser_get(sys_mac_t *pMac,
                                                  authmgr_radiusUserFmt_t *pUserFmt,
                                                  char *pUserStr);

extern int32 sal_authmgr_macauthLocalInfo_set(authmgr_macauth_local_t localInfo);

extern int32 sal_authmgr_macauthLocalInfo_del(sys_mac_t mac);

extern int32 sal_authmgr_macauthLocalInfo_getByIdx(uint32 index, authmgr_macauth_local_t *pLocalInfo);

extern int32 sal_authmgr_macauthLocal_authen(macauth_authInfo_t *pAuthInfo);

extern int32 sal_authmgr_macauthRadius_authen(macauth_authInfo_t *pAuthInfo);

extern int32 sal_authmgr_macauthStatus_set(macauth_authInfo_t *pAuthInfo);

#endif 

