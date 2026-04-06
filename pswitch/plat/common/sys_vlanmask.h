#ifndef __COMMON_SYS_VLANMASK_H__
#define __COMMON_SYS_VLANMASK_H__
#include <common/sys_bitmap.h>
typedef struct sys_vlanmask_s
{
    uint32 bits[SYS_BITS_TO_LONGS(SYS_VLAN_NUM_MAX)];
} sys_vlanmask_t;
#define VLANMASK_SET_VID(vlanmask, vid) \
do{ \
    if((vid) > SYS_VLAN_NUM_MAX) return SYS_ERR_VLAN_ID; \
    SYS_BITMAP_SET_BIT((vlanmask).bits, (vid)); \
}while(0)

#define VLANMASK_CLEAR_VID(vlanmask, vid) \
    do{ \
        if((vid) > SYS_VLAN_NUM_MAX) return SYS_ERR_VLAN_ID; \
        SYS_BITMAP_CLEAR_BIT((vlanmask).bits, (vid)); \
    }while(0)

#define VLANMASK_IS_VIDSET(vlanmask, vid) \
    (((vid) < SYS_VLAN_NUM_MAX) ? SYS_BITMAP_IS_BITSET((vlanmask).bits, (vid)) : 0)

#define FOR_EACH_VID(vid) \
    for((vid) = BOND_VLAN_MIN; (vid) <= BOND_VLAN_MAX; (vid)++)

#define FOR_EACH_VID_IN_VLANMASK(vid, vlanmask) \
    FOR_EACH_VID(vid) \
        if(VLANMASK_IS_VIDSET((vlanmask), (vid)))

#define FOR_EACH_SYSTEM_VID(vid) \
    for((vid) = BOND_VLAN_MIN; (vid) <= DFLT_VLAN_RESERVED_VLAN; (vid)++)

#define FOR_EACH_SYSTEM_VID_IN_VLANMASK(vid, vlanmask) \
    FOR_EACH_SYSTEM_VID(vid) \
        if(VLANMASK_IS_VIDSET((vlanmask), (vid)))

#define VLANMASK_SET_ALL(vlanmask) \
    do{ \
        SYS_BITMAP_SET_ALL((vlanmask).bits, SYS_VLAN_NUM_MAX); \
    }while(0)

#define VLANMASK_CLEAR_ALL(vlanmask) \
    do{ \
        SYS_BITMAP_CLEAR_ALL((vlanmask).bits, SYS_VLAN_NUM_MAX); \
    }while(0)

#define VLANMASK_AND(dst, vlanmask1, vlanmask2) \
    SYS_BITMAP_AND((dst).bits, (vlanmask1).bits, (vlanmask2).bits, SYS_VLAN_NUM_MAX)

#define VLANMASK_ANDNOT(dst, vlanmask1, vlanmask2) \
    SYS_BITMAP_ANDNOT((dst).bits, (vlanmask1).bits, (vlanmask2).bits, SYS_VLAN_NUM_MAX)

#define VLANMASK_OR(dst, vlanmask1, vlanmask2) \
    SYS_BITMAP_OR((dst).bits, (vlanmask1).bits, (vlanmask2).bits, SYS_VLAN_NUM_MAX)

#define VLANMASK_XOR(dst, vlanmask1, vlanmask2) \
    SYS_BITMAP_XOR((dst).bits, (vlanmask1).bits, (vlanmask2).bits, SYS_VLAN_NUM_MAX)

#define VLANMASK_IS_CLEAR(vlanmask, result) \
    SYS_BITMAP_IS_CLEAR((vlanmask).bits, SYS_VLAN_NUM_MAX, result)

#define IS_VLANMASK_CLEAR(vlanmask)  \
    (sys_util_bitmaskIsClear_ret((vlanmask).bits, 0, SYS_VLAN_NUM_MAX))

#define VLANMASK_COPY(dst, src) \
    SYS_BITMAP_COPY((dst).bits, (src).bits, SYS_VLAN_NUM_MAX)
#endif 

