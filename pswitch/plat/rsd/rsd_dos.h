
#ifndef __RSD_DOS_H__
#define __RSD_DOS_H__

#include <common/sys_def.h>

typedef enum sys_dos_type_e
{
    DOS_SYNFIN = 0,
    DOS_SYNRST,             
    DOS_XMA,
    DOS_NULLSCAN,
    DOS_SYN_SPORTL1024,
    DOS_TCP_FRAG_OFF_MIN,   
    DOS_ICMP_FRAG_PKTS,
    DOS_POD,
    DOS_TCPBLAT,
    DOS_UDPBLAT,
    DOS_LAND,
    DOS_DAEQSA,
    
    DOS_LEN_TYPE_START,
    DOS_SMURF,
    DOS_TCPHDR_MIN,
    DOS_ICMP_PING_MAX,      
    DOS_IPV4_PING_MAX,
    DOS_IPV6_PING_MAX,
    DOS_IPV6_MIN_FRAG_SIZE,
    DOS_TYPE_END,
    
    DOS_PORT_ENABLE,
    DOS_GARP,
    DOS_SPECIAL_END
} sys_dos_type_t;

typedef enum sys_dos_tcpSynAction_e
{
    SYS_DOS_TCPSYN_ACT_DISABLE,
    SYS_DOS_TCPSYN_ACT_REPORT,
    SYS_DOS_TCPSYN_ACT_REPORT_AND_BLOCK,
    SYS_DOS_TCPSYN_ACT_END,
} sys_dos_tcpSynAction_t;

typedef enum sys_dos_tcpSynStatus_e
{
    SYS_DOS_TCPSYN_STATUS_NORMAL,
    SYS_DOS_TCPSYN_STATUS_ATTACKED,
    SYS_DOS_TCPSYN_STATUS_BLOCKED,
    SYS_DOS_TCPSYN_STATUSEND,
} sys_dos_tcpSynStatus_t;

typedef struct sys_dos_tcpSynPortStatus_s
{
    uint32 lastAttackTime;
    sys_dos_tcpSynStatus_t status;
    sys_dos_tcpSynAction_t lastAct;
} sys_dos_tcpSynPortStatus_t;

extern int32 rsd_dos_typeEnable_set(sys_dos_type_t type, sys_enable_t enable);

extern int32 rsd_dos_maxPingLen_set(uint32 length);

extern int32 rsd_dos_minIPv6FragLen_set(uint32 length);

extern int32 rsd_dos_smurfNetmaskLen_set(uint32 length);

extern int32 rsd_dos_minTCPHdrLen_set(uint32 length);

extern int32 rsd_dos_portEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_dos_init(void);
#endif

