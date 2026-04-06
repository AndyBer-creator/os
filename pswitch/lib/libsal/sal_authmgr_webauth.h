
#ifndef __SAL_AUTHMGR_WEBAUTH_H__
#define __SAL_AUTHMGR_WEBAUTH_H__

extern int32 sal_authmgr_portWebauthLoginAttempt_set(sys_logic_port_t port, uint32 loginAttemp);

extern int32 sal_authmgr_portWebauthLoginAttempt_get(sys_logic_port_t port, uint32 *pLoginAttemp);

extern int32 sal_authmgr_webauthLocalInfo_set(authmgr_webauth_local_t localInfo);

extern int32 sal_authmgr_webauthLocalInfo_del(char *username);

extern int32 sal_authmgr_webauthLocalInfo_getByIdx(uint32 index, authmgr_webauth_local_t *pLocalInfo);

extern int32 sal_authmgr_webauthIpv6Neigh_refresh(void);

extern int32 sal_authmgr_webauthStatusByKey_get(uint32 key, authmgr_status_t *pStatus);

extern int32 sal_authmgr_webauthSuppInfoByKey_get(uint32 key, authmgr_suppInfo_t *pSupp);

extern int32 sal_authmgr_webauthLocal_authen(char *username, char *password, webauth_authInfo_t *pAuthInfo);

extern int32 sal_authmgr_webauthRadius_authen(char *username, char *password, webauth_authInfo_t *pAuthInfo);

extern int32 sal_authmgr_webauthStatus_set(webauth_authInfo_t *pAuthInfo);

#endif 

