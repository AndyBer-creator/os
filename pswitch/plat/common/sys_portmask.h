#ifndef __COMMON_SYS_PORTMASK_H__
#define __COMMON_SYS_PORTMASK_H__
#include <common/sys_bitmap.h>
#define FOR_EACH_UNIT(unit) \
    for((unit) = 0; (unit) < SYS_DEV_NUM; (unit)++)

#define FOR_EACH_DEV(dev) \
    for((dev) = 0; (dev) < SYS_DEV_NUM; (dev)++)
#define LOGIC_PORTMASK_SET_PORT(portmask, port) \
do{ \
    SYS_BITMAP_SET_BIT((portmask).bits, (port)); \
} while(0)

#define LOGIC_PORTMASK_CLEAR_PORT(portmask, port) \
do{ \
    SYS_BITMAP_CLEAR_BIT((portmask).bits, (port)); \
} while(0)

#define LOGIC_PORTMASK_SET_ALL(lPortmask) \
do{ \
    SYS_BITMAP_SET_ALL((lPortmask).bits, SYS_LOGIC_PORT_NUM); \
} while(0)

#define LOGIC_PORTMASK_CLEAR_ALL(lPortmask) \
do{ \
    SYS_BITMAP_CLEAR_ALL((lPortmask).bits, SYS_LOGIC_PORT_NUM); \
} while(0)

#define LOGIC_PORTMASK_IS_CLEAR(portmask, result) \
    SYS_BITMAP_IS_CLEAR((portmask).bits, SYS_LOGIC_PORT_NUM, result)

#define LOGIC_PORTMASK_COPY(dst, src) \
    SYS_BITMAP_COPY((dst).bits, (src).bits, SYS_LOGICPORT_NUM_MAX)

#define LOGIC_PORTMASK_COMPLEMENT(dst, src) \
    SYS_BITMAP_COMPLEMENT((dst).bits, (src).bits, SYS_LOGICPORT_NUM_MAX)

#define LOGIC_PORTMASK_AND(dst, portmask1, portmask2) \
    SYS_BITMAP_AND((dst).bits, (portmask1).bits, (portmask2).bits, SYS_LOGICPORT_NUM_MAX)

#define LOGIC_PORTMASK_ANDNOT(dst, portmask1, portmask2) \
    SYS_BITMAP_ANDNOT((dst).bits, (portmask1).bits, (portmask2).bits, SYS_LOGICPORT_NUM_MAX)

#define LOGIC_PORTMASK_OR(dst, portmask1, portmask2) \
    SYS_BITMAP_OR((dst).bits, (portmask1).bits, (portmask2).bits, SYS_LOGICPORT_NUM_MAX)

#define LOGIC_PORTMASK_XOR(dst, portmask1, portmask2) \
    SYS_BITMAP_XOR((dst).bits, (portmask1).bits, (portmask2).bits, SYS_LOGICPORT_NUM_MAX)

#define FOR_EACH_LOGIC_PORT(port) \
    for((port) = LOGIC_PORT_START; (port) <= LOGIC_PORT_END; (port)++)

#define FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, portmask) \
    FOR_EACH_LOGIC_PORT((port)) \
        if(IS_LOGIC_PORTMASK_PORTSET((portmask), (port)))

#define IS_LOGIC_PORTMASK_PORTSET(portmask, port) \
    (((port) <= LOGIC_PORT_END) ? SYS_BITMAP_IS_BITSET((portmask).bits, (port)) : 0)

#define IS_LOGIC_PORTMASK_PORTCLEAR(portmask, port) \
    (((port) <= LOGIC_PORT_END) ? SYS_BITMAP_IS_BITCLEAR((portmask).bits, (port)) : 0)

#define IS_LOGIC_PORTMASK_CLEAR(portmask)  \
    (sys_util_bitmaskIsClear_ret(portmask.bits, LOGIC_PORT_START, SYS_LOGIC_PORT_NUM))

#define IS_LOGIC_PORTMASK_ALLSET(portmask)  \
    (sys_util_bitmaskIsAllSet_ret(portmask.bits, LOGIC_PORT_START, SYS_LOGIC_PORT_NUM))

#define IS_LOGIC_PORTMASK_EQUAL(portmask1, portmask2)  \
    (sys_util_bitmaskIsEqual_ret(portmask1.bits, portmask2.bits, SYS_LOGIC_PORT_NUM))
#define NORMAL_LOGIC_PORTMASK_SET_ALL(lPortmask) \
do{ \
    SYS_BITMAP_SET_ALL((lPortmask).bits, SYS_NORMAL_LOGIC_PORT_NUM); \
} while(0)

#define FOR_EACH_NORMAL_LOGIC_PORT(port) \
    for((port) = NORMAL_LOGIC_PORT_START; (port) <= NORMAL_LOGIC_PORT_END; (port)++)

#define FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, portmask) \
    FOR_EACH_NORMAL_LOGIC_PORT((port)) \
        if(IS_LOGIC_PORTMASK_PORTSET((portmask), (port)))

#define IS_NORMAL_LOGIC_PORTMASK_CLEAR(portmask)  \
    (sys_util_bitmaskIsClear_ret(portmask.bits, NORMAL_LOGIC_PORT_START, SYS_NORMAL_LOGIC_PORT_NUM))

#define IS_NORMAL_LOGIC_PORTMASK_ALLSET(portmask)  \
    (sys_util_bitmaskIsAllSet_ret(portmask.bits, NORMAL_LOGIC_PORT_START, SYS_NORMAL_LOGIC_PORT_NUM))

#define IS_LOGIC_PORTMASK_EQUAL(portmask1, portmask2)  \
    (sys_util_bitmaskIsEqual_ret(portmask1.bits, portmask2.bits, SYS_LOGIC_PORT_NUM))
#define FOR_EACH_TRUNK(trunkId) \
    for (trunkId = 0; trunkId < CAPA_LAG_NUM; trunkId++)

#define FOR_EACH_TRUNK_LOGIC_PORT(port) \
    for((port) = TRUNK_LOGIC_PORT_START; (port) <= TRUNK_LOGIC_PORT_END; (port)++)

#define FOR_EACH_TRUNK_PORT_IN_LOGIC_PORTMASK(port, portmask) \
    FOR_EACH_TRUNK_LOGIC_PORT((port)) \
        if(IS_LOGIC_PORTMASK_PORTSET((portmask), (port)))

#define IS_TRUNK_LOGIC_PORTMASK_CLEAR(portmask)  \
    (sys_util_bitmaskIsClear_ret(portmask.bits, TRUNK_LOGIC_PORT_START, CAPA_LAG_NUM))

#define IS_TRUNK_LOGIC_PORTMASK_ALLSET(portmask)  \
    (sys_util_bitmaskIsAllSet_ret(portmask.bits, TRUNK_LOGIC_PORT_START, CAPA_LAG_NUM))

#define TRUNK_LOGIC_PORTMASK_CLEAR_ALL(lPortmask) \
do{ \
    sys_logic_port_t __logicPort;\
    FOR_EACH_TRUNK_LOGIC_PORT(__logicPort)\
        LOGIC_PORTMASK_CLEAR_PORT(lPortmask, __logicPort);\
} while(0)
#define SYS_PHYSIC_PORT_SET(__op, __lport, ...) \
do { \
    sys_physic_port_t _physicPort; \
    sys_physic_portmask_t _physicPortmask; \
    osal_memset(&_physicPortmask, 0, sizeof(_physicPortmask));\
    SYS_ERR_CHK(sys_logic_port_to_physic_portmask(__lport, &_physicPortmask));\
    FOR_EACH_PORT_IN_PHYSIC_PORTMASK(_physicPort, _physicPortmask)\
    {\
        SYS_ERR_XLATE_CHK(__op(_physicPort.dev, _physicPort.port, __VA_ARGS__));\
    }\
} while(0)

#define SYS_PHYSIC_IGNORE_TRKMBR_PORT_SET(__op, __lport, ...) \
do { \
    sys_physic_port_t _physicPort; \
    sys_physic_portmask_t _physicPortmask; \
    osal_memset(&_physicPortmask, 0, sizeof(_physicPortmask));\
    SYS_ERR_TRK_CHK(sys_logic_port_to_physic_portmask(__lport, &_physicPortmask));\
    FOR_EACH_PORT_IN_PHYSIC_PORTMASK(_physicPort, _physicPortmask)\
    {\
        SYS_ERR_XLATE_CHK(__op(_physicPort.dev, _physicPort.port, __VA_ARGS__));\
    }\
} while(0)

#define SYS_PHYSIC_ALLOW_TRKMBR_PORT_SET(__op, __lport, ...) \
do { \
    int32 _ret;\
    sys_physic_port_t _physicPort; \
    sys_physic_portmask_t _physicPortmask; \
    osal_memset(&_physicPortmask, 0, sizeof(_physicPortmask));\
    _ret = sys_logic_port_to_physic_portmask(__lport, &_physicPortmask);\
    if (SYS_ERR_TRUNK_MEMBER_SKIP == _ret) \
    { \
        SYS_ERR_CHK(sys_logic_to_physic_port(__lport, &_physicPort)); \
        PHYSIC_PORTMASK_SET_PORT(_physicPortmask, _physicPort); \
    } \
    else if (SYS_ERR_OK != _ret) \
    { \
        return _ret; \
    } \
    FOR_EACH_PORT_IN_PHYSIC_PORTMASK(_physicPort, _physicPortmask)\
    {\
        SYS_ERR_XLATE_CHK(__op(_physicPort.dev, _physicPort.port, __VA_ARGS__));\
    }\
} while(0)

#define SYS_PHYSIC_PORT_GET(__op, __lport, ...) \
do { \
    sys_physic_port_t _physicPort; \
    SYS_ERR_CHK(sys_logic_to_physic_port(__lport, &_physicPort));\
    SYS_ERR_XLATE_CHK(__op(_physicPort.dev, _physicPort.port, __VA_ARGS__));\
} while(0)

#define SYS_PHYSIC_PORT_MIB_GET(__op, __lport, __mibType, __pResult) \
do { \
    sys_physic_port_t _physicPort; \
    sys_physic_portmask_t _physicPortmask; \
    uint64 _tmpResult = 0;\
    osal_memset(&_physicPortmask, 0, sizeof(_physicPortmask));\
    *__pResult = 0;\
    if (IS_TRKLP(__lport))\
    {\
        SYS_ERR_CHK(sys_logic_port_to_physic_portmask(__lport, &_physicPortmask));\
        FOR_EACH_PORT_IN_PHYSIC_PORTMASK(_physicPort, _physicPortmask)\
        {\
            SYS_ERR_XLATE_CHK(__op(_physicPort.dev, _physicPort.port, __mibType, &_tmpResult));\
            *__pResult += _tmpResult; \
            _tmpResult = 0;\
        }\
    }\
    else \
    {\
        SYS_ERR_CHK(sys_logic_to_physic_port(__lport, &_physicPort));\
        SYS_ERR_XLATE_CHK(__op(_physicPort.dev, _physicPort.port, __mibType, __pResult));\
    }\
} while(0)
#define USER_PORTMASK_SET_PORT(usrPortmask, usrPort) \
do{ \
    SYS_BITMAP_SET_BIT((usrPortmask).bits, (usrPort - 1)); \
} while(0)

#define USER_PORTMASK_CLEAR_PORT(usrPortmask, usrPort) \
do{ \
    SYS_BITMAP_CLEAR_BIT((usrPortmask).bits, (usrPort - 1)); \
} while(0)

#define IS_USER_PORTMASK_PORTSET(portmask, port) \
    (((port) <= SYS_USERPORT_NUM_MAX) ? SYS_BITMAP_IS_BITSET((portmask).bits, (port - 1)) : 0)

#define FOR_EACH_USER_PORT(port) \
    for((port) = 1; (port) <= SYS_USERPORT_NUM_MAX; (port)++)

#define FOR_EACH_PORT_IN_USER_PORTMASK(port, portmask) \
    FOR_EACH_USER_PORT((port)) \
        if(IS_USER_PORTMASK_PORTSET((portmask), (port)))

#define USERSTR_PORTMASK_SET_PORT(usrPortmask, usrPort) \
do{ \
    SYS_BITMAP_SET_BIT_ltoR((usrPortmask).bits, (usrPort-1)); \
} while(0)
#define PHYSIC_PORTMASK_SET_PORT(phyPortmask, phyPort) \
do{ \
    SYS_BITMAP_SET_BIT(((phyPortmask).portmask[phyPort.dev]).bits, (phyPort.port)); \
} while(0)

#define PHYSIC_PORTMASK_CLEAR_PORT(phyPortmask, phyPort) \
do{ \
    SYS_BITMAP_CLEAR_BIT(((phyPortmask).portmask[phyPort.dev]).bits, (phyPort.port)); \
} while(0)

#define PHYSIC_PORTMASK_SET_ALL(phyPortmask) \
do{ \
    uint32 _dev = 0; \
    FOR_EACH_DEV(_dev) \
        SYS_BITMAP_SET_ALL((phyPortmask).portmask[_dev].bits, PHYSIC_PORT_NUM(_dev)); \
} while(0)

#define PHYSIC_PORTMASK_CLEAR_ALL(phyPortmask) \
do{ \
    uint32 _dev = 0; \
    FOR_EACH_DEV(_dev) \
        SYS_BITMAP_CLEAR_ALL((phyPortmask).portmask[_dev].bits, PHYSIC_PORT_NUM(_dev)); \
} while(0)

#define PHYSIC_PORTMASK_IS_PORTSET(phyPortmask, phyPort) \
    (((phyPort.port) <= PHYSIC_PORT_END(phyPort.dev)) ? SYS_BITMAP_IS_BITSET((phyPortmask.portmask[phyPort.dev]).bits, (phyPort.port)) : 0)

#define PHYSIC_PORTMASK_IS_PORTCLEAR(phyPortmask, phyPort) \
    (((phyPort.port) <= PHYSIC_PORT_END(phyPort.dev)) ? SYS_BITMAP_IS_BITCLEAR((phyPortmask.portmask[phyPort.dev]).bits, (phyPort.port)) : 0)

#define PHYSIC_PORTMASK_ANDNOT(dst, phyPm1, phyPm2) \
do { \
        uint32 _dev = 0; \
        FOR_EACH_DEV(_dev) \
            SYS_BITMAP_ANDNOT((dst).portmask[_dev].bits, (phyPm1).portmask[_dev].bits, (phyPm2).portmask[_dev].bits, PHYSIC_PORT_NUM(_dev)); \
} while(0);

#define FOR_EACH_PHYSIC_PORT(physicPort) \
    FOR_EACH_DEV((physicPort).dev) \
        for((physicPort).port = PHYSIC_PORT_START; (physicPort).port <= PHYSIC_PORT_END((physicPort).dev); ((physicPort).port)++)

#define FOR_EACH_PHYSIC_PORT_IN_UNIT(physicPort) \
        for((physicPort).port = PHYSIC_PORT_START; (physicPort).port <= PHYSIC_PORT_END((physicPort).dev); ((physicPort).port)++)

#define FOR_EACH_PORT_IN_PHYSIC_PORTMASK(port, portmask) \
    FOR_EACH_PHYSIC_PORT((port)) \
        if(PHYSIC_PORTMASK_IS_PORTSET((portmask), (port)))
#define PHYSIC_PORTMASK_SET_CPU_PORT(phyPortmask, dev) \
do{ \
    SYS_BITMAP_SET_BIT(((phyPortmask).portmask[dev]).bits, PHYSIC_CPU_PORT(dev)); \
} while(0)

#define PHYSIC_PORTMASK_CLEAR_CPU_PORT(phyPortmask, dev) \
do{ \
    SYS_BITMAP_CLEAR_BIT(((phyPortmask).portmask[dev]).bits, PHYSIC_CPU_PORT(dev)); \
} while(0)

#define PHYSIC_PORTMASK_IS_CPU_PORTSET(phyPortmask, dev) \
    SYS_BITMAP_IS_BITSET((phyPortmask.portmask[dev]).bits, PHYSIC_CPU_PORT(dev))
#define FOR_EACH_POE_LOGIC_PORT(port) \
    for((port) = NORMAL_LOGIC_PORT_START; (port) <= NORMAL_LOGIC_PORT_END; (port)++) \
        if(IS_POE_PORT(port))

#define FOR_EACH_POE_PORT_IN_LOGIC_PORTMASK(port, portmask) \
    for((port) = NORMAL_LOGIC_PORT_START; (port) <= NORMAL_LOGIC_PORT_END; (port)++) \
        if(IS_POE_PORT(port) && IS_LOGIC_PORTMASK_PORTSET((portmask), (port)))
#endif 

