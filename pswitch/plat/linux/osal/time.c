
#include <common/sys_def.h>
#ifdef __KERNEL__
#include <asm/uaccess.h>
#include <linux/unistd.h>       
  #ifdef CONFIG_SYS_KERNEL_LINUX_2_6_X
#include <linux/kernel.h>       
  #else
#include <linux/syscalls.h>
  #endif
#else
#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
#include <asm/posix_types.h>
#include <asm-generic/posix_types.h>
#endif
#include <sys/sysinfo.h>
#endif

#if defined(CONFIG_SYS_KERNEL_LINUX_2_6_X) || !defined(__KERNEL__)
static int32 errno;
#endif

#ifdef __KERNEL__
  #ifdef CONFIG_SYS_KERNEL_LINUX_2_6_X
_syscall1(int, sysinfo, struct sysinfo *, info);
  #else
  
  #endif
#endif

int32
osal_time_sysUptime_get(uint32 *pTime)
{
#ifdef __KERNEL__
  #ifdef CONFIG_SYS_KERNEL_LINUX_2_6_X
    mm_segment_t fs;
    struct sysinfo s_info;
    int errno;

    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    fs = get_fs();
    set_fs(get_ds());

    errno = sysinfo(&s_info);

    set_fs(fs);

    if (0 == errno)
    {
        *pTime = (s_info.uptime * 1000); 

        return SYS_ERR_OK;
    }
  #else

    *pTime = (get_seconds()) * 1000;

    return SYS_ERR_OK;
  #endif
#else
    struct sysinfo s_info;
    errno = sysinfo(&s_info);
    if(errno == 0)
    {
        *pTime = (s_info.uptime * 1000); 

        return SYS_ERR_OK;
    }
#endif

    return SYS_ERR_FAILED;
}

