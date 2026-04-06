#ifndef _GARP_GVD_H__
#define _GARP_GVD_H__

#include "proto_gvrp_gvf.h"
extern void gvd_destroy_gvd(void);

extern BOOL gvd_find_entry(void *my_gvd, Vlan_id key,uint32 *found_at_index);
extern BOOL gvd_create_entry(void *my_gvd, Vlan_id key, uint8 isDyn, uint32 *created_at_index);
extern int32  gvd_delete_entry(void *my_gvd,uint32  delete_at_index);
extern BOOL gvd_get_key(void *my_gvd, uint32 index, Vlan_id *key);
extern BOOL gvd_is_dynamic_entry(Gvd *gvd, Vlan_id vid);
extern BOOL gvd_index_isExsit(void *gvd, uint32 index);

extern void set_gvrp_db_from_reg(void);
extern void set_gvrp_db_from_reg_init(void);
extern void reset_gvrp_db(void);
#endif 

