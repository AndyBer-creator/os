
#ifndef __RSD_VLAN_SURVEILLANCE_H__
#define __RSD_VLAN_SURVEILLANCE_H__

#include <surveillancevlan/proto_surveillancevlan.h>

typedef struct rsd_sv_entry_s
{
    uint8 oui[SNAPOUI_LEN];
    sys_vid_t vid;
    sys_pri_t pri;
    sys_qid_t qid;
    sys_enable_t remark;
    sys_logic_portmask_t spm;
} rsd_sv_entry_t;

extern int32 rsd_vlan_surveillance_chip_copyEntry_set(uint32 idx, rsd_sv_entry_t *sv);

extern int32 rsd_vlan_surveillance_chip_copyEntry_del(uint32 idx);

extern int32 rsd_vlan_surveillance_chip_cosSrcEntry_set(uint32 idx, rsd_sv_entry_t *sv);

extern int32 rsd_vlan_surveillance_chip_cosSrcEntry_del(uint32 idx);

extern int32 rsd_vlan_surveillance_chip_cosAllEntry_set(rsd_sv_entry_t *sv);

extern int32 rsd_vlan_surveillance_chip_cosAllEntry_del(void);
#endif

