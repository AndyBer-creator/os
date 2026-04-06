
#ifndef __CFG_EASYCWMP_H__
#define __CFG_EASYCWMP_H__

#include <common/sys_type.h>
#include <libsal/sal_dhcp_server.h>
#include <libipc/vos.h>

typedef enum auth_type_s {
	AUTH_BASIC,
	AUTH_DIGEST
}auth_type_t;

#define DEFAULT_CR_AUTH_TYPE AUTH_DIGEST;

typedef struct cfg_device_s
{
	char                       manufacturer[CAPA_SYS_MANUFACTURER];
	char                       product_class[CAPA_SYS_PRODUCT_CLASS];
    char                       sn[CAPA_SYS_SN_LEN];
    char                       hwVersion[CAPA_SYS_CHAR_64];
    char                       swVersion[CAPA_SYS_CHAR_64];
} cfg_device_t;

typedef struct cfg_acs_s
{
	char                       url[CAPA_SYS_CHAR_64];
	char                       username[CAPA_SYS_PRODUCT_CLASS];
    char                       password[CAPA_SYS_SN_LEN];
    uint32                     periodic_enable;
    uint32                     periodic_interval;
} cfg_acs_t;

typedef struct cfg_local_s
{
	uint32                     enable;
	char                       interface[CAPA_SYS_CHAR_64];
    uint32                     port;
    char                       username[CAPA_SYS_CHAR_64];
    char                       password[CAPA_SYS_CHAR_64];
    auth_type_t                authentication;
} cfg_local_t;

typedef struct cfg_easycwmp_s
{
	cfg_device_t       cwmpDevice;
	cfg_local_t        cwmpLocal;
	cfg_acs_t          cwmpAcs;

} cfg_easycwmp_t;

extern int32 cfg_easycwmp_db_init(void);
extern int32 cfg_easycwmp_device_get(cfg_device_t *pDeviceInfo);
extern int32 cfg_easycwmp_device_set(cfg_device_t *pDeviceInfo);

#endif 

