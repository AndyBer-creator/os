
#ifndef __SAL_ACL_MGMT_H__
#define __SAL_ACL_MGMT_H__

#include <rsd/rsd_acl_mgmt.h>

extern int32 sal_acl_mgmt_list_add(char *pcName);
extern int32 sal_acl_mgmt_list_num_get(uint32 *pcListNum);
extern int32 sal_acl_mgmt_list_acenum_get(char *pcName,uint32 *puiAceNum);
extern int32 sal_acl_mgmt_all_acenum_get(uint32 *puiAceNum);
extern int32 sal_acl_mgmt_list_del(char *pcName);
extern int32 sal_acl_mgmt_ace_add( char *pcListName, sys_acl_mgmt_ace_priFlag_t stAcepriFlag, sys_acl_mgmt_ace_t *pstAce);
extern int32 sal_acl_mgmt_name_get(char *pcListName,uint32 idx);
extern  int32 sal_acl_mgmt_ace_get( char *pcListName,uint32 idx,sys_acl_mgmt_ace_t *pstAce);
extern int32 sal_acl_mgmt_ace_del( char *pcListName, uint32 uiMgmtAcePri);
extern int32 sal_acl_mgmt_class_set( char *pcListName);
extern  int32 sal_acl_mgmt_class_get( char *pcListName);
extern int32 sal_acl_mgmt_class_del( void);
#endif

