
#ifndef __CFG_TACPLUS_H__
#define __CFG_TACPLUS_H__

#include <libsal/sal_tacplus.h>

#define TACPLUS_NOT_UESD_FLAG  (-1)

typedef struct cfg_tacplus_s
{
    sys_tacplus_t    tacDftParam;
    sys_tacplus_t    tacplusSrv[CAPA_TACPLUS_HOST_NUM_MAX];
} cfg_tacplus_t;

extern int32 cfg_tacplus_init(void);

extern int32 cfg_tacplus_dftParam_set(sys_tacplus_t tacplusSrv);

extern int32 cfg_tacplus_dftParam_get(sys_tacplus_t *pTacplusSrv);

extern int32 cfg_tacplus_server_set(uint32 index, sys_tacplus_t tacplusSrv);

extern int32 cfg_tacplus_server_get(uint32 index, sys_tacplus_t *pTacplusSrv);
#endif 

