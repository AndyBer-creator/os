
#include <common/sys_l2.h>
#include <common/sys_l3.h>
#include <common/sys_type.h>
#include <common/sys_vlan.h>
#include <libcfg/cfg_init.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_init.h>
#include <libsal/sal_util.h>
#include <libsal/sal_crash.h>

int32 sal_init(void)
{

    SYS_ERR_CHK(cfg_init());

    SYS_ERR_CHK(sal_util_lock_init());

    SYS_ERR_CHK(sys_dbg_init());

    SYS_ERR_CHK(sys_user_portview_init(SYS_DEV_ID_DEFAULT));

    SYS_ERR_CHK(sal_sys_init());

    SYS_ERR_CHK(sys_vlan_init());

#if defined(CONFIG_FLASH_LAYOUT_TYPE2) || defined(CONFIG_FLASH_LAYOUT_TYPE3) || defined(CONFIG_FLASH_LAYOUT_TYPE4) || defined(CONFIG_FLASH_LAYOUT_TYPE5)

#endif 

#ifdef CONFIG_SYS_FW_UPGRADE_SAVEMEM
    
    SYS_ERR_CHK(sal_sys_fwImageSaveMem_gen(TMP_FWIMG_FILENAME));
#endif 

    SYS_ERR_CHK(sal_sys_currSysCompiledTime_set());

    return SYS_ERR_OK;
}

