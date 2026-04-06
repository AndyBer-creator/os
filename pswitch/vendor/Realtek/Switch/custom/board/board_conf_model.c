
#include <board/board.h>
#include <common/sys_def.h>
#include <drv/gpio/generalCtrl_gpio.h>
#include <hwp/hw_profile.h>
#include <osal/print.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <pswitch/sysinfo.h>

#define SYSINFO_VAR_BOARDID "boardid"
#define SYSINFO_VAR_BOOTMSG "bootmsg"

#define REALTEK_OID 27282
#define SWITCH_OID   1
#define MODEL_RTL838x 1
#define MODEL_RTL930x 2
#define MODEL_RTL931x 3
const uint32 oidAry_rtk838x[] = {1,3,6,1,4,1,REALTEK_OID,SWITCH_OID,MODEL_RTL838x};
const uint32 oidAry_rtk930x[] = {1,3,6,1,4,1,REALTEK_OID,SWITCH_OID,MODEL_RTL930x};
const uint32 oidAry_rtk931x[] = {1,3,6,1,4,1,REALTEK_OID,SWITCH_OID,MODEL_RTL931x};

const sys_board_conf_t boardModelConfTbl[] =
{
    
#ifdef CONFIG_SYS_RTL8380
    {SYS_BM_8380_8GE_2GEF,       "GS8380-10", "RTL8380",  9, oidAry_rtk838x,  "RTL8380-8GE-2GEF", SYS_POE_CHIP_NONE},
    {SYS_BM_8380_8GE_2GEF_POE,   "GS8380-10", "RTL8380",  9, oidAry_rtk838x,  "RTL8380-8GE-2GEF", SYS_POE_CHIP_NONE},
    {SYS_BM_8380_8GE_2GEF_POE_BT,"GS8380-10", "RTL8380",  9, oidAry_rtk838x,  "RTL8380-8GE-2GEF", SYS_POE_CHIP_NONE},
    {SYS_BM_8382_24GE_4GEC,      "GS8382-28", "RTL8382",  9, oidAry_rtk838x,  "RTL8382-24GE-4GEC",SYS_POE_CHIP_NONE},
    {SYS_BM_8382_24GE_4GEC_POE,  "GS8382-28", "RTL8382",  9, oidAry_rtk838x,  "RTL8382-24GE-4GEC",SYS_POE_CHIP_NONE},
    {SYS_BM_8382_16GE_2GE,       "GS8382-18", "RTL8382",  9, oidAry_rtk838x,  "RTL8382-16GE-2GE", SYS_POE_CHIP_NONE},
    {SYS_BM_8382_16GE_2GE_POE,   "GS8382-18", "RTL8382",  9, oidAry_rtk838x,  "RTL8382-16GE-2GE", SYS_POE_CHIP_NONE},
    {SYS_BM_8380_4GE_2GEF_YFD,   "GS8380-6",  "RTL8380",  9, oidAry_rtk838x,  "RTL8380-4GE-2GEF", SYS_POE_CHIP_NONE},
    {SYS_BM_8380_4GE_2GEF_POE,   "GS8380-6",  "RTL8380",  9, oidAry_rtk838x,  "RTL8380-4GE-2GEF", SYS_POE_CHIP_NONE},
    {SYS_BM_8380_4GE_2GEF_POE_BT,"GS8380-6",  "RTL8380",  9, oidAry_rtk838x,  "RTL8380-4GE-2GEF", SYS_POE_CHIP_NONE},
    {SYS_BM_8382_10GE_4GF,       "GS8382-10", "RTL8382",  9, oidAry_rtk838x,  "RTL8382-10GE-4GF", SYS_POE_CHIP_NONE},
    {SYS_BM_8382_10GE_4GF_POE,   "GS8382-10", "RTL8382",  9, oidAry_rtk838x,  "RTL8382-10GE-4GF", SYS_POE_CHIP_NONE},
    {SYS_BM_8382_10GE_4GF_POE_BT,"GS8382-10", "RTL8382",  9, oidAry_rtk838x,  "RTL8382-10GE-4GF", SYS_POE_CHIP_NONE},
#endif

#ifdef CONFIG_SYS_RTL9300
    {SYS_BM_9301_24GE_4XGEF,     "GS9300-28", "RTL9301",  9, oidAry_rtk838x,  "RTL9301-28GE-4XGE", SYS_POE_CHIP_NONE},
    {SYS_BM_9301_24GE_4XGEF_YFD, "GS9300-28", "RTL9301",  9, oidAry_rtk838x,  "RTL9301-28GE-4XGE", SYS_POE_CHIP_NONE},
    {SYS_BM_9301_24GE_4XGEF_POE, "GS9300-28", "RTL9301",  9, oidAry_rtk838x,  "RTL9301-28GE-4XGE", SYS_POE_CHIP_NONE},
#endif

#ifdef CONFIG_SYS_RTL9310
    {SYS_BM_9311_48GE_6XGEF,         "GS9310-54", "RTL9311", 9, oidAry_rtk838x,  "RTL9311-48GE-6XGE Q/A", SYS_POE_CHIP_NONE},
    {SYS_BM_9311_48GE_6XGEF_YFD,     "GS9310-54", "RTL9311", 9, oidAry_rtk931x,  "RTL9311-48GE-6XGE", SYS_POE_CHIP_NONE},
    {SYS_BM_9311_48GE_6XGEF_POE,     "GS9310-54", "RTL9311", 9, oidAry_rtk931x,  "RTL9311-48GE-6XGE", SYS_POE_CHIP_NONE},
    {SYS_BM_9311_48GE_6XGEF_DEMO,    "GS9310-54", "RTL9311", 9, oidAry_rtk838x,  "RTL9311-48GE-6XGE DEMO", SYS_POE_CHIP_NONE},
    {SYS_BM_9313_12XGEF,             "GS9310-12", "RTL9313", 9, oidAry_rtk838x,  "RTL9313-12XGE DEMO", SYS_POE_CHIP_NONE},
    {SYS_BM_9313_24_2_5GE_6XGEF_DEMO,"GS9313-24", "RTL9313", 9, oidAry_rtk931x,  "RTL9313-24", SYS_POE_CHIP_NONE},
#endif
    
    {SYS_BM_END, "", "", 0, NULL, ""}
};

static void _sysinfo_var_get(char *name, char *value)
{
    int ret = 0;
    size_t retlen;
    struct mtd_info *mtd = NULL;
    char *buf;
    char *data_ptr;
    char *varname;
    int32 varval = 0;
    uint32 size = 0;
    uint32 i;
    uint32 nxt;

    mtd = get_mtd_device(mtd, 2);

    size = SYSINFO_ENV_SIZE;
    buf = kmalloc(size, GFP_KERNEL);
#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
    ret = mtd_read(mtd, 0, size, &retlen, buf);
#else
    ret = mtd->read(mtd, 0, size, &retlen, buf);
#endif
    data_ptr = buf + 4; 

    for (i = 0; *(data_ptr + i) != '\0'; i = nxt + 1)
    {
        varname = data_ptr + i;
        varval = 0;

        for (nxt = i; *(data_ptr + nxt) != '\0'; ++nxt)
        {
            if (nxt > size)
                break;

            if (*(data_ptr + nxt) == '=')
            {
                
                if (0 == osal_strncmp(name, varname, nxt - i))
                    varval = nxt + 1;
            }
        }
        if (varval != 0)
        {
            osal_strncpy(value, data_ptr + varval, nxt - varval);
            break;
        }
    }

    kfree(buf);
}

int32 sys_boardModel_init(void)
{
    sys_boardModel_t boardModel;
    uint32 index = 0;
    uint32 i = 0;

    sys_boardModel_get(&boardModel);

    board_gpio_init();

    printk("Board ID: %u\n", HWP_IDENTIFIER_ID());
    boardModel.boardConf.boardModelId = HWP_IDENTIFIER_ID();

    board_gpio_model_init(boardModel.boardConf.boardModelId);

    while(boardModelConfTbl[index].boardModelId != SYS_BM_END)
    {
        if(boardModelConfTbl[index].boardModelId == boardModel.boardConf.boardModelId)
        {
            osal_memcpy(&boardModel.boardConf, &boardModelConfTbl[index], sizeof(sys_board_conf_t));
            break;
        }
        index++;
    }

#ifdef CONFIG_SYS_UI_SNMP
    
    boardModel.oid.oidLen = boardModelConfTbl[index].oidLen;

    for (i = 0; i < boardModel.oid.oidLen; i++)
    {
        boardModel.oid.oidAry[i] = boardModelConfTbl[index].pOidAry[i];
    }
#endif

    {
        char bootmsgStr[SYS_BUF16_LEN];
        uint32 bootmsg = 0;

        SYS_MEM_CLEAR(bootmsgStr);
        _sysinfo_var_get(SYSINFO_VAR_BOOTMSG, bootmsgStr);

        if (0 != strlen(bootmsgStr))
            bootmsg = simple_strtol(bootmsgStr, NULL, 10);
        else
            bootmsg = BOOTMSG_LEVEL_ALL;

        boardModel.bootmsg = bootmsg;
    }

    sys_boardModel_set(&boardModel);

    return SYS_ERR_OK;
}

