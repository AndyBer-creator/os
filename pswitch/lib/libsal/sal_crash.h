
#ifndef __SAL_CRASH_H__
#define __SAL_CRASH_H__

#include <libsal/sal_type.h>

#define MIPS_ADDIU_SP_SP_XXXX       (0x27bd0000) 
#define MIPS_SW_RA_XXXX_SP          (0xafbf0000) 
#define MIPS_ADDU_RA_ZERO_ZERO      (0x3c1c0000) 

#define FLASH_MOUNT_PATH            "/mnt/"
#define CRASHLOG_PATH               FLASH_MOUNT_PATH "crashlog/"
#define CRASHLOG_FILE_PREFIX        "error"
#define CRASHLOG_FILE_NAME          FLASH_MOUNT_PATH CRASHLOG_FILE_PREFIX ".txt"

extern int32 sal_crash_init(void);

extern int32 sal_crash_handler_register(void);

#endif 

