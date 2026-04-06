
#ifndef __SAL_AAA_H__
#define __SAL_AAA_H__

#include <libsal/sal_type.h>
#include <libsal/sal_sys_line.h>
#ifdef CONFIG_SYS_LIB_TACPLUS
#include <libsal/sal_tacplus.h>
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
#include <libradius/radlib.h>
#endif

#define SYS_AAA_TACPLUS_TTY_PORT            "tty10"
#define SYS_AAA_AUTH_METHOD_STR_NONE        "none"
#define SYS_AAA_AUTH_METHOD_STR_LOCAL       "local"
#define SYS_AAA_AUTH_METHOD_STR_ENABLE      "enable"
#define SYS_AAA_AUTH_METHOD_STR_TACPLUS     "tacacs+"
#define SYS_AAA_AUTH_METHOD_STR_RADIUS      "radius"
#define SYS_AAA_DFLT_AUTHEN_LOGIN_LIST      "default"
#define SYS_AAA_DFLT_AUTHEN_ENABLE_LIST     "default"
#define SYS_AAA_DFLT_AUTHOR_EXEC_LIST       "default"
#define SYS_AAA_DFLT_AUTHOR_CMDS_LIST       "default"
#define SYS_AAA_DFLT_LIST                   "default"

#define AAA_MIDDLE_STR_LENGTH               16
#define AAA_LITTLE_STR_LENGTH               8

typedef enum sys_aaa_type_e
{
    SYS_AAA_TYPE_AUTHEN = 0,
    SYS_AAA_TYPE_AUTHOR,
    SYS_AAA_TYPE_ACCUNT,
    SYS_AAA_TYPE_END
} sys_aaa_type_t;

typedef enum sys_auth_type_e
{
    SYS_AUTH_TYPE_EMPTY = 0,
    SYS_AUTH_TYPE_NONE,
    SYS_AUTH_TYPE_LOCAL,
    SYS_AUTH_TYPE_LINE,
    SYS_AUTH_TYPE_ENABLE,
#ifdef CONFIG_SYS_LIB_RADIUS
    SYS_AUTH_TYPE_RADIUS,
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
    SYS_AUTH_TYPE_TACPLUS,
#endif
    SYS_AUTH_TYPE_END
} sys_auth_type_t;

typedef enum sys_authList_type_e
{
    SYS_AUTHLIST_TYPE_NONE = 0,
    SYS_AUTHLIST_TYPE_AUTHEN_LOGIN,
    SYS_AUTHLIST_TYPE_AUTHEN_ENABLE,
    SYS_AUTHLIST_TYPE_END
} sys_authList_type_t;

typedef struct sys_aaa_authList_s
{
    char                name[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    sys_auth_type_t     typePrio[CAPA_AAA_AUTHTYPE_PRI_NUM];
    sys_authList_type_t authListType;
} sys_aaa_authList_t;

extern const sys_text_t text_aaa_auth_method[];

extern int32 sal_aaa_authList_set(
    char                *pAuthListName,
    sys_auth_type_t     *pTypePrio,
    sys_authList_type_t authListType);

extern int32 sal_aaa_authList_getByIdx(
    int32               index,
    char                *pAuthListName,
    sys_auth_type_t     *pTypePrio,
    sys_authList_type_t *pAuthListType);

extern int32 sal_aaa_authList_del(char *pAuthListName, sys_authList_type_t authListType);

extern int32 sal_aaa_authListTypePrio_get(
    char                *authListName,
    sys_authList_type_t authListType,
    sys_auth_type_t     *pTypePrio);

extern int32 sal_aaa_loginAuthList_set(sys_access_type_t type, char *pAuthListName);

extern int32 sal_aaa_loginAuthListName_get(sys_access_type_t type, char *pAuthListName);

extern int32 sal_aaa_loginAuthTypePrio_get(sys_access_type_t type, sys_auth_type_t *pTypePrio);

extern int32 sal_aaa_enableAuthList_set(sys_access_type_t type, char *pAuthListName);

extern int32 sal_aaa_enableAuthListName_get(sys_access_type_t type, char *pAuthListName);

extern int32 sal_aaa_enableAuthTypePrio_get(sys_access_type_t type, sys_auth_type_t *pTypePrio);

extern int32 sal_aaa_localLoginUsername_authen(char *username, uint32 *pLocalUsrIdx);

extern int32 sal_aaa_localLoginPassword_authen(uint32 localUsrIdx, char *password, int32 *pPriv);

extern int32 sal_aaa_localEnablePassword_authen(int32 priv, char *password);

#ifdef CONFIG_SYS_LIB_TACPLUS

extern int32 sal_aaa_tacplusSession_init(struct session **ppTacplusSession);

extern int32 sal_aaa_tacplusSession_close(struct session *pTacplusSession);

extern int32 sal_aaa_tacplusLoginUsername_authen(
    struct session *pTacplusSession,
    char           *username);

extern int32 sal_aaa_tacplusLoginPassword_authen(
    struct session *pTacplusSession,
    char           *password);

#endif

#ifdef CONFIG_SYS_LIB_RADIUS

extern int32 sal_aaa_radiusServerRank_get(int32 rank, int32 *pServerIndex);

extern int32 sal_aaa_radiusLoginUsername_authen(char *user, char *passwd);

#endif

extern int32 sal_aaa_loginAuthUserAccessMethod(sys_access_type_t type, char *username);
#endif 

