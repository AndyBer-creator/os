
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <board/board.h>
#include <linux/osal/proc.h>

#define BOARD_PROC_BUFLEN 8192

PROC_SEQ_INIT(board, BOARD_PROC_BUFLEN, board_conf_dump);

static int32 __init board_conf_init(void)
{
    int32 ret = SYS_ERR_FAILED;
    sys_boardModel_t boardModel;

    sys_boardModel_get(&boardModel);
#if defined(CONFIG_FLASH_LAYOUT_TYPE1)
    boardModel.flshLayout = SYS_FLSH_LAYOUT_TYPE1;
#elif defined(CONFIG_FLASH_LAYOUT_TYPE2)
    boardModel.flshLayout = SYS_FLSH_LAYOUT_TYPE2;
#elif defined(CONFIG_FLASH_LAYOUT_TYPE3)
    boardModel.flshLayout = SYS_FLSH_LAYOUT_TYPE3;
#elif defined(CONFIG_FLASH_LAYOUT_TYPE4)
    boardModel.flshLayout = SYS_FLSH_LAYOUT_TYPE4;
#elif defined(CONFIG_FLASH_LAYOUT_TYPE5)
    boardModel.flshLayout = SYS_FLSH_LAYOUT_TYPE5;
#endif
    sys_boardModel_set(&boardModel);

    SYS_ERR_HDL(sys_boardModel_init(), error, ret);

    PROC_SEQ_CREATE(BOARD_CONF_PROC_NAME, board);

 error:
    BOOTMSG_RET_PRINTF(ret, "Init Board Configuration Module....%s\n", (ret) ? "FAILED" : "OK");

    return ret;
}

static void __exit board_conf_exit(void)
{
    PROC_REMOVE(BOARD_CONF_PROC_NAME);
    
}

module_init(board_conf_init);
module_exit(board_conf_exit);

MODULE_DESCRIPTION ("Switch Board Configuration Module");

MODULE_LICENSE("GPL");

