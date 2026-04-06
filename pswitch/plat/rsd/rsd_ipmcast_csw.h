
#ifndef __RSD_IPMCAST_CSW_H__
#define __RSD_IPMCAST_CSW_H__

extern int32 rsd_ipmcast_csw_ipmcastGroup_create(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipmcastGroup_destroy(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipv6mcastGroup_create(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipv6mcastGroup_destroy(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipmcastGroup_egressIntf_set(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipmcastGroup_egressIntf_get(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipmcastGroup_egressIntf_del(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipmcastGroup_egressIntfCPU_del(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipv6mcastGroup_egressIntf_set(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipv6mcastGroup_egressIntf_get(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipv6mcastGroup_egressIntf_del(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipv6mcastGroup_egressIntfCPU_del(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipmcastEntry_set(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipmcastEntry_get(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipmcastEntry_del(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipmcastEntry_clear(void);

extern int32 rsd_ipmcast_csw_ipv6mcastEntry_set(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipv6mcastEntry_del(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ipmcast_csw_ipv6mcastEntry_clear(void);

#endif

