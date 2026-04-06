
#ifndef __RSD_ACL_CSW_H__
#define __RSD_ACL_CSW_H__

extern int32 rsd_acl_csw_entryRange_add(rtk_pie_rangeCheck_t *rngInfo, uint32 *idx);

extern int32 rsd_acl_csw_entryRange_get(uint32 idx, rtk_pie_rangeCheck_t *rngInfo);

extern int32 rsd_acl_csw_entryRange_del(uint32 idx);
#endif

