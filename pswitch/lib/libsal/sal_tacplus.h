
#ifndef __SAL_TACPLUS_H__
#define __SAL_TACPLUS_H__

#include <libsal/sal_type.h>
#include <libsal/sal_sys_line.h>
#include <libtacplus/libtacplus.h>

#define SYS_AAA_TACPLUS_SRV_PRIO_AUTO_SEL   (-1)

typedef struct sys_tacplus_s
{
    char srvHost[CAPA_HOSTNAME_LEN + 1];
    uint32 srvPort;
    int32  priority;
    char   key[CAPA_AAA_KEY_LEN + 1];
    uint32 timeout;
    uint32 singleConnection;

    uint8  keyUseDft;
    uint8  timeoutUseDft;
} sys_tacplus_t;

extern int32 sal_tacplus_dftParam_set(sys_tacplus_t tacplusSrv);

extern int32 sal_tacplus_dftParam_get(sys_tacplus_t *pTacplusSrv);

extern int32 sal_tacplus_server_set(int32 index, sys_tacplus_t tacplusSrv);

extern int32 sal_tacplus_server_get(uint32 index, sys_tacplus_t *pTacplusSrv);

extern int32 sal_tacplus_server_del(uint32 index);

#endif 

