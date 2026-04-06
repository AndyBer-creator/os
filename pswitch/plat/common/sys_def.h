
#ifndef __COMMON_SYS_DEF_H__
#define __COMMON_SYS_DEF_H__
#include <common/sys_autoconf.h>
#include <common/sys_type.h>
#include <common/swdef/sys_swdef.h>
#include <common/sys_assert.h>
#include <common/sys_bitmap.h>
#include <common/sys_error.h>
#include <common/sys_list.h>
#include <common/sys_log.h>
#include <common/sys_log_desc.h>
#include <common/sys_dbg.h>
#include <common/sys_dbg_desc.h>
#include <common/sys_notifier.h>
#include <common/sys_portmask.h>
#include <common/sys_portview.h>
#include <common/sys_queuemask.h>
#include <common/sys_util.h>
#include <common/sys_vlanmask.h>
#include <common/sys_l2.h>
#include <common/sys_l3.h>

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
#include <defdb/defdb.h>
#define SYS_DEV_NUM                 (sys_dev_num_ret())
#define SYS_DEV_ID_DEFAULT          (0)

#define SYS_USER_PORT_NUM(unit)     (sys_user_port_num_ret(unit))
#define USER_PORT_START             (1)
#define USER_PORT_END(unit)         (SYS_USER_PORT_NUM(unit))

#define SYS_LOGIC_PORT_NUM          (sys_logic_port_num_ret())
#define LOGIC_PORT_START            (0)
#define LOGIC_PORT_END              (SYS_LOGIC_PORT_NUM - 1)
#define SYS_NORMAL_LOGIC_PORT_NUM   (sys_logic_port_num_ret() - CAPA_LAG_NUM)
#define NORMAL_LOGIC_PORT_START     (0)
#define NORMAL_LOGIC_PORT_END       (SYS_NORMAL_LOGIC_PORT_NUM - 1)
#define TRUNK_LOGIC_PORT_START      (SYS_NORMAL_LOGIC_PORT_NUM)
#define TRUNK_LOGIC_PORT_END        (LOGIC_PORT_END)

#define SYS_COMBO_PORT_NUM(dev)     (sys_combo_port_num_ret(dev))

#define PHYSIC_PORT_NUM(dev)        (sys_physic_port_num_ret(dev))
#define PHYSIC_PORT_START           (0)
#define PHYSIC_PORT_END(dev)        (sys_cpu_port_ret(dev) - 1)
#define PHYSIC_CPU_PORT(dev)        (sys_cpu_port_ret(dev))

#define VLAN_TABLE_START            (0)
#define VLAN_TABLE_CLEAR_START      (2)
#define VLAN_TABLE_END              (SYS_VLAN_NUM_MAX - 1)
#define SYS_THREAD_PRI_DFLT         (66)  
#define SYS_THREAD_PRI_HIGH         (35)  
#define SYS_THREAD_PRI_LOW          (100) 
extern const sys_text_t text_enable[];
#define IS_DEV_INVALID(dev)         \
    (((dev) >= SYS_DEV_NUM_MAX) ? 1: 0)

#define IS_IMGPART_INVALID(partition)    \
    ((((partition) < 0) || ((partition) > 1)) ? 1 : 0)

#define IS_USER_PORT_INVALID(userPort)    \
    ((((userPort.port) < USER_PORT_START) || ((userPort.port) > USER_PORT_END(userPort.unit))) ? 1 : 0)

#define IS_LOGIC_PORT_INVALID(port) \
    ((((port) < LOGIC_PORT_START) || ((port) > TRUNK_LOGIC_PORT_END)) ? 1 : 0)

#define IS_NORMAL_LOGIC_PORT_INVALID(port) \
    ((((port) < NORMAL_LOGIC_PORT_START) || ((port) > NORMAL_LOGIC_PORT_END)) ? 1 : 0)

#define IS_PHYSIC_PORT_INVALID(phyPort)    \
    ((((phyPort.port) < PHYSIC_PORT_START) || ((phyPort.port) > PHYSIC_PORT_END(phyPort.dev))) ? 1 : 0)

#define IS_TRUNK_INVALID(trunkId)    \
    ((((trunkId) < 0) || ((trunkId) >= CAPA_LAG_NUM)) ? 1 : 0)

#define IS_ENABLE_INVALID(enable)    \
    ((((enable) < DISABLED) || ((enable) > ENABLED)) ? 1 : 0)

#define IS_VID_INVALID(vid)    \
    ((((vid) < BOND_VLAN_MIN) || ((vid) > BOND_VLAN_MAX)) ? 1 : 0)

#define IS_SYSTEM_VID_INVALID(vid)    \
    ((((vid) < BOND_VLAN_MIN) || ((vid) > DFLT_VLAN_RESERVED_VLAN)) ? 1 : 0)

#define IS_VLAN_PORT_MODE_INVALID(mode)    \
    ((((mode) < 0) || ((mode) >= SYS_VLAN_PORT_MODE_END)) ? 1 : 0)

#define IS_VLAN_TAG_MODE_INVALID(mode)    \
    ((((mode) < 0) || ((mode) >= VLAN_TAG_MODE_END)) ? 1 : 0)

#define IS_VLAN_FRAME_TYPE_INVALID(frameType)    \
    ((((frameType) < ACCEPT_FRAME_TYPE_ALL) || ((frameType) >= ACCEPT_FRAME_TYPE_END)) ? 1 : 0)

#define IS_VLAN_MSTI_INVALID(instance)    \
    ((((instance) < 0) || ((instance) >= MSTP_INSTANCE_NUM)) ? 1 : 0)

#define IS_VLAN_TYPE_INVALID(type)    \
    ((((type) < SYS_VLAN_ALL) || ((type) >= SYS_VLAN_TYPE_END)) ? 1 : 0)

#define IS_PVLAN_GROUP_INVALID(groupIdx)    \
    ((((groupIdx) < 0) || ((groupIdx) >= CAPA_VLAN_PROTO_GROUP_NUM)) ? 1 : 0)

#define IS_L2_VLAN_MODE_INVALID(mode)    \
    ((((mode) < 0) || ((mode) >= FWD_VLAN_MODE_END)) ? 1 : 0)

#define IS_PRI_INVALID(pri)    \
    ((((pri) < BOND_QOS_PRI_MIN) || ((pri) > BOND_QOS_PRI_MAX)) ? 1 : 0)

#define IS_PARAM_NUM_MULTIPLE_VALID(num, min, max, multiple)    \
    ((num >= min) && (num <= max) && (0 == (num % multiple)))

#define IS_PARAM_NUM_VALID(num, min, max)    \
    IS_PARAM_NUM_MULTIPLE_VALID(num, min, max, 1)

#define IS_PARAM_NUM_MULTIPLE_INVALID(num, min, max, multiple)    \
    (!(IS_PARAM_NUM_MULTIPLE_VALID(num, min, max, multiple)))

#define IS_PARAM_NUM_INVALID(num, min, max)    \
    IS_PARAM_NUM_MULTIPLE_INVALID(num, min, max, 1)

#define IS_TCPUDP_PORT_INVALID(portnum) \
    ((((portnum) < 0) || ((portnum) > 65535)) ? 1 : 0)

#define IS_QID_INVALID(qid) \
    ((((qid) < 0) || ((qid) >= CAPA_QOS_QUEUE_NUM)) ? 1 : 0)

#define IS_QUEUE_WEIGHT_INVALID(weight) \
    ((((weight) < BOND_QOS_QUEUE_WEIGHT_MIN) || ((weight) > BOND_QOS_QUEUE_WEIGHT_MAX)) ? 1 : 0)

#define IS_COS_INVALID(cos) \
    ((((cos) < 0) || ((cos) >= CAPA_QOS_COS_NUM)) ? 1 : 0)

#define IS_DSCP_INVALID(dscp) \
    ((((dscp) < 0) || ((dscp) >= CAPA_QOS_DSCP_NUM)) ? 1 : 0)

#define IS_PRECEDENCE_INVALID(ipp) \
    ((((ipp) < 0) || ((ipp) >= CAPA_QOS_IPP_NUM)) ? 1 : 0)

#define IS_LEDTYPE_INVALID(type) \
    ((((type) < 0) || ((type) >= SYS_LED_TYPE_END)) ? 1 : 0)

#define IS_LEDSTATE_INVALID(state) \
    ((((state) < 0) || ((state) >= SYS_LED_STATE_END)) ? 1 : 0)

#define IS_GID_INVALID(gid) \
    ((((gid) < 0) || ((gid) >= CAPA_MIRROR_SESSION_NUM)) ? 1 : 0)

#ifdef CONFIG_SYS_RTL8380
#define IS_CHIP_MAPLE() (HWP_8380_30_FAMILY(HWP_MY_UNIT_ID()))
#else
#define IS_CHIP_MAPLE() (0)
#endif

#ifdef CONFIG_SYS_RTL8390
#define IS_CHIP_CYPRESS() (HWP_8390_50_FAMILY(HWP_MY_UNIT_ID()))
#else
#define IS_CHIP_CYPRESS() (0)
#endif

#if (defined(CONFIG_SYS_RTL8380) && defined(CONFIG_SYS_RTL8390))
#define IS_CHIP_MSW() (IS_CHIP_MAPLE() || IS_CHIP_CYPRESS())
#elif defined(CONFIG_SYS_RTL8380)
#define IS_CHIP_MSW() (IS_CHIP_MAPLE())
#elif defined(CONFIG_SYS_RTL8390)
#define IS_CHIP_MSW() (IS_CHIP_CYPRESS())
#else
#define IS_CHIP_MSW() (0)
#endif

#ifdef CONFIG_SYS_RTL9300
#define IS_CHIP_LONGAN() (HWP_9300_FAMILY_ID(HWP_MY_UNIT_ID()))
#else
#define IS_CHIP_LONGAN() (0)
#endif

#ifdef CONFIG_SYS_RTL9310
#define IS_CHIP_MANGO() (HWP_9310_FAMILY_ID(HWP_MY_UNIT_ID()))
#else
#define IS_CHIP_MANGO() (0)
#endif

#if (defined(CONFIG_SYS_RTL9300) && defined(CONFIG_SYS_RTL9310))
#define IS_CHIP_CSW() (IS_CHIP_LONGAN() || IS_CHIP_MANGO())
#elif defined(CONFIG_SYS_RTL9300)
#define IS_CHIP_CSW() (IS_CHIP_LONGAN())
#elif defined(CONFIG_SYS_RTL9310)
#define IS_CHIP_CSW() (IS_CHIP_MANGO())
#else
#define IS_CHIP_CSW() (0)
#endif

#ifdef CONFIG_SYS_BOARD_DEBUG_INFO
	#define BOARD_INFO_MSG(arg)                         osal_printf arg
#else
	#define BOARD_INFO_MSG(arg)                         
#endif
extern int32 sys_dev_init(void);
extern void *sys_dev_info_ret(uint32 unit);
extern int32 sys_port_num_init(void);
extern int32 sys_user_port_num_set(uint32 unit, uint32 number);
extern uint32 sys_user_port_num_ret(uint32 unit);
extern uint32 sys_dev_num_ret(void);
extern uint32 sys_dev_chipFamilyId_ret(uint32 devId);
extern uint32 sys_logic_port_num_ret(void);
extern uint32 sys_physic_port_num_ret(uint32 dev);
extern uint32 sys_combo_port_num_ret(uint32 dev);
extern uint32 sys_cpu_port_ret(uint32 dev);

#endif  

