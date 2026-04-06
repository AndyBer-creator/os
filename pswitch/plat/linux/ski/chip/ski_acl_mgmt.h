
#ifndef __SKI_ACL_MGMT_H__
#define __SKI_ACL_MGMT_H__

#include <common/sys_def.h>
#include <ski/ski_core.h>
#include <rsd/rsd_acl_mgmt.h>

typedef enum ski_acl_mgmt_cmd_e
{
    SKI_MGMT_ACL_SET_BASE = (SKI_CORE_MOD_MGMTACL * SKI_CORE_MODCMD_TOTAL),
    SKI_MGMT_ACL_ADD_SET,
    SKI_MGMT_ACL_DEL_SET,
    SKI_MGMT_ACE_ADD_SET,
    SKI_MGMT_ACE_DEL_SET,
    SKI_MGMT_CLASS_ADD_SET,
    SKI_MGMT_CLASS_DEL_SET,

    SKI_MGMT_ACL_GET_BASE = (SKI_MGMT_ACL_SET_BASE  + SKI_CORE_MODCMD_RANGE),
    SKI_MGMT_ACL_NUM_GET,
    SKI_MGMT_ACL_NAME_GET,
    SKI_MGMT_ACE_NUM_GET,
    SKI_MGMT_ACE_ALLNUM_GET,
    SKI_MGMT_ACE_GET,
    SKI_MGMT_CLASS_GET,

    SKI_MGMT_ACL_CMD_END
} ski_acl_mgmt_cmd_t;

typedef struct ski_acl_mgmt_set_s
{
   char acListName[SYS_MGMT_ACL_NAME_LENTH_MAX +1];
} ski_acl_mgmt_set_t;

typedef struct ski_mgmt_list_ioctl_s
{
   ski_acl_mgmt_set_t  stIoctlList;
   sys_acl_mgmt_ace_t  stIoctlAce;
   sys_acl_mgmt_class_t stIoctlClass;
   uint32 uiListIndx;
   uint32 uiAceIndx;
   uint32 uiListNum;
   uint32 uiAceNum;
} ski_mgmt_list_ioctl_t;

extern int32 ski_acl_mgmt_init(void);

#endif  

