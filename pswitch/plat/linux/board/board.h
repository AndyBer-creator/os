
#ifndef __BOARD_H__
#define __BOARD_H__

#include <common/sys_autoconf.h>
#include <common/sys_type.h>
#include <common/swdef/sys_swdef.h>
#include <common/sys_assert.h>
#include <common/sys_bitmap.h>
#include <common/sys_error.h>
#include <common/sys_list.h>
#include <common/sys_log.h>
#include <common/sys_log_desc_old.h>
#include <common/sys_util.h>
#include <common/sys_notifier.h>

#include <osal/cache.h>
#include <osal/inet.h>
#include <osal/isr.h>
#include <osal/lib.h>
#include <osal/memory.h>
#include <osal/print.h>
#include <osal/sem.h>
#include <osal/spl.h>
#include <osal/thread.h>
#include <linux/osal/time.h>
#include <linux/osal/timer.h>
#include <linux/osal/routine.h>

#include <board/vendor/board_vendor.h>
#include <board/vendor/board_gpio.h>
#include <board/include/port.h>
#include <board/include/fiber.h>
#include <board/include/button.h>
#include <board/include/led.h>
#include <board/include/watchdog.h>
#include <board/include/reset.h>
#include <board/include/dyinggasp.h>
#ifdef CONFIG_SYS_POE
#include <board/include/poe.h>
#endif
#ifdef CONFIG_SYS_UART1
#include <board/include/SerialPort.h>
#endif
#ifdef CONFIG_SYS_BOARD_RTC
#include <board/include/rtc_device.h>
#endif
#ifdef CONFIG_SYS_BOARD_FAN
#include <board/include/fan_device.h>
#endif
#ifdef CONFIG_SYS_BOARD_TEMP
#include <board/include/temp_device.h>
#endif

#define BOARD_DFLT_UNIT 0
#define BOARD_CONF_PROC_NAME "board"

typedef enum sys_flsh_layout_e
{
    SYS_FLSH_LAYOUT_TYPE1,
    SYS_FLSH_LAYOUT_TYPE2,
    SYS_FLSH_LAYOUT_TYPE3,
    SYS_FLSH_LAYOUT_TYPE4,
    SYS_FLSH_LAYOUT_TYPE5,
    SYS_FLSH_LAYOUT_TYPE_END
} sys_flsh_layout_t;

#ifdef CONFIG_SYS_SNMP
typedef struct sys_boardOid_s
{
    uint8  oidLen;
    uint32 oidAry[CAPA_SNMP_VIEW_SUBTREE_LEN];
} sys_boardOid_t;
#endif

typedef enum sys_port_numType_e
{
    PORT_NUMTYPE_ALL = 0,
    PORT_NUMTYPE_COPPER,
    PORT_NUMTYPE_FIBER,
    PORT_NUMTYPE_COMBO,
    PORT_NUMTYPE_END
} sys_port_numType_t;

typedef enum sys_bootmsg_level_e
{
    BOOTMSG_LEVEL_NONE = 0,
    BOOTMSG_LEVEL_FAIL,
    BOOTMSG_LEVEL_ALL,
    BOOTMSG_LEVEL_END
} sys_bootmsg_level_t;

typedef struct sys_boardModel_s
{
    sys_board_conf_t  boardConf;
    sys_flsh_layout_t flshLayout;
#ifdef CONFIG_SYS_SNMP
    sys_boardOid_t    oid;
#endif
    uint32            portNum[SYS_DEV_NUM_MAX][PORT_NUMTYPE_END];
    uint32            bootmsg;
} sys_boardModel_t;

typedef struct sys_board_s
{
    sys_boardModel_Id_t        boardModelId;
    const sys_port_conf_t      *pPortConf;
#ifdef CONFIG_SYS_WATCHDOG
    const sys_watchdog_conf_t  *pWatchdogConf;
#endif
#ifdef CONFIG_SYS_FIBER
    const sys_fiber_conf_t     *pFiberConf;
#endif
#ifdef CONFIG_SYS_LED
    const sys_led_conf_t       *pLedConf;
#endif
#ifdef CONFIG_SYS_POE
    const sys_poe_config_t     *pPoeConf;
#endif
#ifdef CONFIG_SYS_BUTTON
	const sys_button_conf_t    *pButtonConf;
#endif
#ifdef CONFIG_SYS_ENCRYPT
	const sys_encrypt_conf_t   *pEncryptConf;
#endif
#ifdef CONFIG_SYS_BOARD_RTC
	const sys_rtc_conf_t       *pRtcConf;
#endif
#ifdef CONFIG_SYS_BOARD_FAN
	const sys_fan_conf_t	   *pFanConf;
#endif
#ifdef CONFIG_SYS_BOARD_TEMP
	const sys_temperature_conf_t	*pTempConf;
#endif

} sys_board_t;

typedef int32 (*board_monitor_cb)(void *pParam);

extern sys_board_t boardMap[];

#define BOARD_DBG(a,args...) 

#define SYS_INIT_FAILURE_STR        "<!-RTK_INIT_FAILURE-!>"

#ifdef __KERNEL__
#define BOOTMSG_LEVEL_RET() (sys_board_bootmsgLevel_ret())
#else
#define BOOTMSG_LEVEL_RET() (sal_sys_boardBootmsgLevel_ret())
#endif

#ifdef __KERNEL__
#define _BOOTMSG_PRINTF(fmt, args...) \
do { \
    osal_printf(fmt, ## args); \
} while (0)
#else
#define _BOOTMSG_PRINTF(fmt, args...) \
do { \
    FILE *fp = fopen("/dev/kmsg", "a+"); \
    if (fp) \
    { \
        fprintf(fp, fmt, ## args); \
        fflush(fp); \
        fclose(fp); \
    } \
    else { \
        osal_printf(fmt, ## args); \
    } \
} while (0)
#endif

#define IS_BOOTMSG_PRINT_ALL() (BOOTMSG_LEVEL_ALL == BOOTMSG_LEVEL_RET())

#define BOOTMSG_PRINTF(fmt, args...) \
do { \
    _BOOTMSG_PRINTF(fmt, ## args); \
} while(0)

#define BOOTMSG_FAIL_PRINTF(fmt, args...) \
do { \
    _BOOTMSG_PRINTF("\n"); \
    _BOOTMSG_PRINTF("<3>" SYS_INIT_FAILURE_STR fmt, ## args); \
} while(0)

#define BOOTMSG_RET_PRINTF(ret, fmt, args...) \
do { \
    if (SYS_ERR_OK == ret) \
        BOOTMSG_PRINTF(fmt, ## args); \
    else \
        BOOTMSG_FAIL_PRINTF(fmt, ## args); \
} while(0)

extern int32 sys_boardModel_init(void);

extern int32 sys_boardModel_get(sys_boardModel_t *pBoardModel);

extern int32 sys_boardModel_set(sys_boardModel_t *pBoardModel);

extern sys_boardModel_Id_t sys_boardModelId_ret(void);

extern int32 sys_board_portNum_set(uint32 dev, sys_port_numType_t type, uint32 num);

extern uint32 sys_board_portNum_ret(uint32 dev, sys_port_numType_t type);

extern uint32 sys_board_bootmsgLevel_ret(void);

extern int32 board_conf_get(sys_board_t **ppConf);

extern int32 board_conf_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 board_init(void);

extern int32 board_monitor_register(board_monitor_cb monitorCb);

extern int32 board_button_reset_init(sys_button_reset_conf_t *pBtnConf);

extern int32 board_watchdog_register(board_monitor_cb monitorCb);

#endif 

