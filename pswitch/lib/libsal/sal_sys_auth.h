
#ifndef __SAL_SYS_AUTH_H__
#define __SAL_SYS_AUTH_H__

#define SYS_LOCALUSER_PASSWDTIME_DIR       "/var/sys/auth"
#define SYS_LOCALUSER_PASSWDTIME_FLSHDIR   "/mnt/auth"
#define SYS_LOCALUSER_EMPTY_USER "__!empty!__"

typedef enum sys_passwdTime_sync_e
{
    SYS_PASSWDTIME_SYNC_FLSH2RAM,
    SYS_PASSWDTIME_SYNC_RAM2FLSH,
    SYS_PASSWDTIME_SYNC_END,
} sys_passwdTime_sync_t;

#define SYS_LOCALUSER_PRIV_ADMIN        15
#define SYS_LOCALUSER_PRIV_CLI_RW       7
#define SYS_LOCALUSER_PRIV_USER         1
typedef enum sys_passwd_type_e
{
    SYS_PASSWD_TYPE_NOPASSWD = 0,
    SYS_PASSWD_TYPE_CLEARTEXT,
    SYS_PASSWD_TYPE_ENCRYPT,
    SYS_PASSWD_TYPE_END
} sys_passwd_type_t;

typedef struct sys_userInfo_s
{
    char   username[CAPA_SYS_USERNAME_LEN + 1];
    char   password[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    int32  priv;
    sys_passwd_type_t passwdType;
    uint32 accessMethod;        
} sys_userInfo_t;

typedef struct sys_enblPasswd_s
{
    char   password[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    sys_passwd_type_t passwdType;
} sys_enblPasswd_t;

extern int32 sal_sys_localUser_add(sys_userInfo_t userInfo);

extern int32 sal_sys_localUserForce_add(sys_userInfo_t userInfo);

extern int32 sal_sys_localUserPasswdDirect_add(sys_userInfo_t userInfo);

extern int32 sal_sys_localUser_del(char *username);

extern int32 sal_sys_localUser_get(uint32 index, sys_userInfo_t *pUserInfo);

extern int32 sal_sys_localUser_getByName(char *username, sys_userInfo_t *pUserInfo);

extern int32 sal_sys_localLogin_auth(char *username, char *password, int32 *pPriv);

extern int32 sal_sys_enablePassword_set(int32 priv, sys_enblPasswd_t password);

extern int32 sal_sys_enablePasswordDirect_set(int32 priv, sys_enblPasswd_t password);

extern int32 sal_sys_enablePassword_get(int32 priv, sys_enblPasswd_t *pPassword);

extern int32 sal_sys_localUser_is_theLastUser(char *username);
extern int32 sal_sys_localUser_access_method_set(uint32 index, sys_userInfo_t userInfo);
extern int32 sal_sys_get_sys_defaul_user(sys_userInfo_t* userInfo);

#endif 

