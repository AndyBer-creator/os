
#ifndef __SAL_RADIUS_H__
#define __SAL_RADIUS_H__

#include <libsal/sal_type.h>
#ifdef CONFIG_SYS_LIB_RADIUS
#include <libradius/radlib.h>
#endif

#define SYS_RADIUS_SRV_PRIO_AUTO_SEL        (-1)

#define RADIUS_NOT_UESD_FLAG  (-1)

typedef enum sys_radsrv_usgType_e
{
    SYS_RADIUS_SRV_USGTYPE_LOGIN = 0,
    SYS_RADIUS_SRV_USGTYPE_8021X,
    SYS_RADIUS_SRV_USGTYPE_ALL,
    SYS_RADIUS_SRV_USGTYPE_END
} sys_radsrv_usgType_t;

typedef struct sys_radius_s
{
    char   hostName[CAPA_HOSTNAME_LEN + 1];
    uint32 srvIp;
    uint32 srvPort;
    uint32 acctPort;
    char   key[CAPA_AAA_KEY_LEN + 1];
    uint32 timeout;
    uint32 retransmit;
    uint32 usgType;
    int32  priority;

    uint8  keyUseDft;
    uint8  timeoutUseDft;
    uint8  retransmitUseDft;
} sys_radius_t;

extern int32 sal_radiusSrv_dftParam_set(sys_radius_t radiusSrv);

extern int32 sal_radiusSrv_dftParam_get(sys_radius_t *pRadiusSrv);

extern int32 sal_radiusSrv_set(int32 index, sys_radius_t radiusSrv);

extern int32 sal_radiusSrv_get(uint32 index, sys_radius_t *pRadiusSrv);

extern int32 sal_radiusSrv_del(uint32 index);

#endif 

