
#ifndef __RSD_ACL_MSW_H__
#define __RSD_ACL_MSW_H__

#include <rsd/rsd_acl.h>

#ifdef CONFIG_SYS_RTL8380
#define SYS_ACL_MAPLE_BLK_MAX                   (12)
#define SYS_ACL_MAPLE_METER_BLK_MAX             (1)
#define SYS_ACL_MAPLE_METER_BLK_ENTRY_NUM_MAX   (64) 
#define SYS_ACL_MAPLE_LOG_BLK_MAX               (1)
#define SYS_ACL_MAPLE_LOG_ENTRYNUM_PER_BLK      (256)

#define SYS_ACL_BYTE_SLB0_BURST             (55535)
#define SYS_ACL_BYTE_SLB1_BURST             SYS_ACL_BYTE_SLB0_BURST
#define SYS_ACL_PKT_SLB0_BURST              (10)
#define SYS_ACL_PKT_SLB1_BURST              (10)
#endif

#ifdef CONFIG_SYS_RTL8390
#define SYS_ACL_CYPRESS_BLK_MAX                 (18)
#define SYS_ACL_CYPRESS_IGRBLK_MAX              (16)
#define SYS_ACL_CYPRESS_METER_BLK_MAX           (16)
#define SYS_ACL_CYPRESS_METER_BLK_ENTRY_NUM_MAX (32) 
#define SYS_ACL_CYPRESS_LOG_BLK_MAX             (16)
#define SYS_ACL_CYPRESS_LOG_ENTRYNUM_PER_BLK    (128)

#define SYS_ACL_BYTE_DLB0_BURST             (65535)
#define SYS_ACL_BYTE_DLB1_BURST             SYS_ACL_BYTE_DLB0_BURST
#define SYS_ACL_BYTE_SRTCMLB0_BURST         (65535)
#define SYS_ACL_BYTE_SRTCMLB1_BURST         SYS_ACL_BYTE_SRTCMLB0_BURST
#define SYS_ACL_BYTE_TRTCMLB0_BURST         (65535)
#define SYS_ACL_BYTE_TRTCMLB1_BURST         SYS_ACL_BYTE_TRTCMLB0_BURST
#define SYS_ACL_PKT_DLB0_BURST              (10)
#define SYS_ACL_PKT_DLB1_BURST              (10)
#define SYS_ACL_PKT_SRTCMLB0_BURST          (10)
#define SYS_ACL_PKT_SRTCMLB1_BURST          (10)
#define SYS_ACL_PKT_TRTCMLB0_BURST          (10)
#define SYS_ACL_PKT_TRTCMLB1_BURST          (10)
#endif

int32 rsd_acl_msw_vlanRange_add(rtk_acl_rangeCheck_vid_t *rngInfo, uint32 *idx);

int32 rsd_acl_msw_vlanRange_del(uint32 idx);

int32 rsd_acl_msw_l4portRange_add(rtk_acl_rangeCheck_l4Port_t *rngInfo, uint32 *idx);

int32 rsd_acl_msw_l4portRange_get(uint32 idx, rtk_acl_rangeCheck_l4Port_t *rngInfo);

int32 rsd_acl_msw_l4portRange_del(uint32 idx);
#endif

