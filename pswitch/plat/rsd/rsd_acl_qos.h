
#ifndef __RSD_ACL_QOS_H__
#define __RSD_ACL_QOS_H__

extern int32 rsd_acl_entry_4qos_bind(sys_qos_policy_t *qosPolicy, sys_logic_portmask_t bindpm);

extern int32 rsd_acl_entry_4qos_unbind(sys_logic_portmask_t unbindpm);

extern int32 rsd_acl_entry_portBindingQoS_get(sys_namestr_t *aclname);

#endif 

