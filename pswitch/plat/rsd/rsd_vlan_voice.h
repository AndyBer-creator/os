
#ifndef __RSD_VLAN_VOICE_H__
#define __RSD_VLAN_VOICE_H__

#include <voice_vlan/proto_voice_vlan.h>

int32 rsd_vlan_voice_chip_mbrEntry_set(uint32 entryIdx,
    sys_vlan_voice_entry_t *voiceEntry);

int32 rsd_vlan_voice_entry_del(uint32 entryIdx);

int32 rsd_vlan_voice_chip_cosSrcEntry_set(uint32 entryIdx,
    sys_vlan_voice_entry_t *voiceEntry);

int32 rsd_vlan_voice_chip_cosSrcEntry_del(uint32 entryIdx);

int32 rsd_vlan_voice_chip_cosAllEntry_set(sys_vlan_voice_entry_t *voiceEntry);

int32 rsd_vlan_voice_chip_cosAllEntry_del(void);
#endif

