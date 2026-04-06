#ifndef _MIBGROUP_SYSTEM_MIB_H
#define _MIBGROUP_SYSTEM_MIB_H

#ifdef __cplusplus
extern "C" {
#endif

config_require(util_funcs mibII/updates)

void init_system_mib(void);

/* function declarations */
FindVarMethod var_system;
    WriteMethod write_sysContact;
    WriteMethod write_sysName;
    WriteMethod write_sysLocation;
#ifdef __cplusplus
}
#endif

#endif                          /* _MIBGROUP_SYSTEM_MIB_H */
