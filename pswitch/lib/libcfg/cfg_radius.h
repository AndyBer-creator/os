
#ifndef __CFG_RADIUS_H__
#define __CFG_RADIUS_H__

#include <libsal/sal_radius.h>

typedef struct cfg_radius_s
{
    sys_radius_t radDftParam;
    sys_radius_t radiusSrv[CAPA_RADIUS_HOST_NUM_MAX];
} cfg_radius_t;

extern int32 cfg_radius_init(void);

extern int32 cfg_radiusSrv_dftParam_set(sys_radius_t radiusSrv);

extern int32 cfg_radiusSrv_dftParam_get(sys_radius_t *pRadiusSrv);

extern int32 cfg_radiusSrv_set(uint32 index, sys_radius_t radiusSrv);

extern int32 cfg_radiusSrv_get(uint32 index, sys_radius_t *pRadiusSrv);
#endif

