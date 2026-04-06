
#ifndef __CFG_AAA_H__
#define __CFG_AAA_H__

#include <libsal/sal_aaa.h>

#define TACPLUS_NOT_UESD_FLAG  (-1)

typedef struct cfg_aaa_s
{
    sys_aaa_authList_t   authLists[CAPA_AAA_AUTHLIST_NUM];

    int32                loginConsole;
    int32                loginTelnet;
    int32                loginSsh;
    int32                loginHttp;
    int32                loginHttps;

    int32                enableConsole;
    int32                enableTelnet;
    int32                enableSsh;
} cfg_aaa_t;

extern int32 cfg_aaa_init(void);

extern int32 cfg_aaa_authList_set(int32 index, char *pAuthListName, sys_auth_type_t *pTypePrio, sys_authList_type_t authListType);

extern int32 cfg_aaa_authList_get(int32 index, char *pAuthListName, sys_auth_type_t *pTypePrio, sys_authList_type_t *pAuthListType);

extern int32 cfg_aaa_authListIndex_get(char *pAuthListName, sys_authList_type_t authListType, int32 *pIndex);

extern int32 cfg_aaa_authListFirstEmptyIndex_get(int32 *pIndex);

extern int32 cfg_aaa_authListName_get(int32 index, char *pAuthListName);

extern int32 cfg_aaa_authTypePrio_get(int32 index, sys_auth_type_t *pTypePrio);

extern int32 cfg_aaa_loginAuthListIndex_set(sys_access_type_t type, int32 index);

extern int32 cfg_aaa_loginAuthListIndex_get(sys_access_type_t type, int32 *pIndex);

extern int32 cfg_aaa_enableAuthListIndex_set(sys_access_type_t type, int32 index);

extern int32 cfg_aaa_enableAuthListIndex_get(sys_access_type_t type, int32 *pIndex);

#endif 

