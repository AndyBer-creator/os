#include <common/sys_def.h>
#include <common/sys_dbg_desc.h>
#define DBG_CAT_DESC_INITIALIZER(_cat, _extnum)  \
[_cat] =                                                    \
{                                                           \
    .extnum     = _extnum,                                  \
}

const sys_dbg_desc_t dbg_cat_desc[DBG_CAT_ALL] =
{
    DBG_CAT_DESC_INITIALIZER(DBG_ACLMGR,        DBG_ACLMGR_ALL - DBG_EXT_BASE(DBG_ACLMGR)),
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    DBG_CAT_DESC_INITIALIZER(DBG_AUTHMGR,
                                                DBG_AUTHMGR_ALL - DBG_EXT_BASE(DBG_AUTHMGR)),
#endif
    DBG_CAT_DESC_INITIALIZER(DBG_L3,            DBG_L3_ALL - DBG_EXT_BASE(DBG_L3)),
#ifdef CONFIG_SYS_PROTO_LLDP
    DBG_CAT_DESC_INITIALIZER(DBG_LLDP,          DBG_LLDP_ALL - DBG_EXT_BASE(DBG_LLDP)),
#endif
    DBG_CAT_DESC_INITIALIZER(DBG_VLAN,          DBG_VLAN_ALL - DBG_EXT_BASE(DBG_VLAN)),
#ifdef CONFIG_SYS_DHCP_SERVER
	DBG_CAT_DESC_INITIALIZER(DBG_DHCP_SERVER,	DBG_DHCP_SERVER_ALL - DBG_EXT_BASE(DBG_DHCP_SERVER)),
#endif

};
