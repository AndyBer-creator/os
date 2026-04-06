
#include "mod_custom.h"
#include "ski_custom.h"

int32 ski_custom_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_custom_ioctl_t);
    ski_custom_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_CUSTOM_PORT_ENABLE_SET:
            SYS_ERR_HDL(mod_custom_portEnableExample_set(pData->port, pData->enable), _exit, ret);
            break;
        case SKI_CUSTOM_ENABLE_SET:
            SYS_ERR_HDL(mod_custom_enableExample_set(pData->enable), _exit, ret);
            break;
        case SKI_CUSTOM_PORT_ENABLE_GET:
            SYS_ERR_HDL(mod_custom_portEnableExample_get(pData->port, &pData->enable), _exit, ret);
            break;
        case SKI_CUSTOM_ENABLE_GET:
            SYS_ERR_HDL(mod_custom_enableExample_get(&pData->enable), _exit, ret);
            break;
        default:
            osal_printf("Unknown custom command!!\n");
            ret = SYS_ERR_FAILED;
            goto _exit;
        break;
    }

    if(cmd > SKI_CUSTOM_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

static int __init  ski_custom_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Custom Module....");

    SYS_ERR_HDL(mod_custom_init(),  error, ret);

    SYS_ERR_HDL(ski_handler_register(SKI_CORE_MOD_CUSTOM, ski_custom_cmd_mux),  error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Custom Module....FAILED\n");

    return SYS_ERR_OK;
}

static void __exit ski_custom_exit(void)
{

    osal_printf("Exit Custom Module....OK\n");
}

module_init(ski_custom_init);
module_exit(ski_custom_exit);
MODULE_DESCRIPTION ("Custom Kernel Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

