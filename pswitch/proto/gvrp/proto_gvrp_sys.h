#ifndef _GARP_SYS_H__
#define _GARP_SYS_H__

#include <common/sys_type.h>
#include <common/sys_portview.h>
#include <gvrp/proto_gvrp.h>

#include <rsd/rsd_nic.h>
#define __GVRP__
#define GMRP_PROCESS_ID 0x300
#define GVRP_PROCESS_ID 0x310
#define RAND_MAX  2147483647
#define GARP_ID         0x0001
#define END_MARK        0x00
#define DISCARDED_PDU   0xFFFF
#define GVRP_DB_END                 (0)
#define GID_MACHINE_MAX         (31)
#define END_OF_GIP                  (99)
#define MAX_PDU_LEN     1518 
#define MIN_PDU_LEN     43   
#ifndef BOOL
#define  BOOL  uint8
#endif
extern BOOL sysmalloc(uint32 size, void **allocated);
extern void  sysfree(void *allocated);
typedef enum Attrib_event_e
{
    LEAVE_ALL,
    JOIN_EMPTY,
    JOIN_IN,
    LEAVE_EMPTY,
    LEAVE_IN,
    EMPTY

} Attrib_event;
typedef struct  gvr_pdu_s
{
   uint16  uiLength;       
   uint8   *pucContent;    
}Gvr_pdu;
typedef struct gvrp_cfg_s
{
    sys_enable_t         gvrp_enable;
    register_mode_t     regMode[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t         portEnable[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t         forbidVlan[SYS_LOGICPORT_NUM_MAX];

    uint32      holdTime;
    uint32      joinTime;
    uint32      leaveTime;
    uint32      leaveAllTime;

}gvrp_cfg_t;
enum Applicant_states
{
 Va, 
 Aa, 
 Qa, 
 La, 
 Vp, 
 Ap, 
 Qp, 
 Vo, 
 Ao, 
 Qo, 
 Lo, 
 Von, 
 Aon, 
 Qon 
};
enum Registrar_states
{

 Inn, 
 Lv,L3,L2,L1,
 Mt,

 Inr, 
 Lvr,L3r,L2r,L1r,
 Mtr,

 Inf, 
 Lvf,L3f,L2f,L1f,
 Mtf
};
typedef struct  TstGarpTrk_s{
    uint8      ucTrkId;
    sys_logic_portmask_t    stPortMask;
} TstGarpTrk;
typedef enum   {All_attributes, Vlan_attribute}  Attribute_type;
extern int32   syspdu_alloc(uint16 size, sys_nic_pkt_t **pdu);
extern void    syspdu_free( sys_nic_pkt_t *pdu);
extern BOOL   syspdu_tx( sys_nic_pkt_t *pPkt, uint32 pktLen, sys_logic_portmask_t pmsk, gvrp_statistics_t *pcnt);
extern int32     gvrp_pdu_rcv(sys_nic_pkt_t *pPkt, void *pCookie);
#define GVRP_EABLE_SET(enable) do { \
        gvrp_cfg.gvrp_enable = enable; \
    }while(0)
#define GVRP_PORT_IS_ENABLE(port)    (gvrp_cfg.portEnable[port])
#define GVRP_PORT_ENABLE_SET(port, enable) do { \
    gvrp_cfg.portEnable[port] = enable; \
}while(0)

#define GVRP_PORT_REGMODE_GET(port)  (gvrp_cfg.regMode[port] )
#define GVRP_PORT_REGMODE_SET(port, regmode)  do { \
    gvrp_cfg.regMode[port] = regmode; \
}while(0)
#define GVRP_PORT_FORBIDVLAN_GET(port)  (gvrp_cfg.forbidVlan[port] )
#define GVRP_PORT_FORBIDVLAN_SET(port, enable)  do { \
    gvrp_cfg.forbidVlan[port] = enable; \
}while(0)
#define  GVRP_ENABLE()         (gvrp_cfg.gvrp_enable)
#define  GVRP_PORT_ENABLE(port)         (gvrp_cfg.portEnable[port])
#define  GVRP_HOLD_TIME()   (gvrp_cfg.holdTime)
#define  GVRP_JOIN_TIME()   (gvrp_cfg.joinTime)
#define  GVRP_LEAVE_TIME()   (gvrp_cfg.leaveTime)
#define  GVRP_LEAVE_ALL_TIME()   (gvrp_cfg.leaveAllTime)
#define  GVRP_LEAVE_ALL_TIME_N()   (gvrp_cfg.leaveAllTime/Gid_leaveall_count)
#define GVRP_PKT_SEM_LOCK()    \
do { } while(osal_sem_mutex_take(gvrp_sem_pkt, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)

#define GVRP_PKT_SEM_UNLOCK() \
do { } while(osal_sem_mutex_give(gvrp_sem_pkt) != SYS_ERR_OK)

#define GVRP_DB_SEM_LOCK()    \
do { } while(osal_sem_mutex_take(gvrp_sem_db, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)

#define GVRP_DB_SEM_UNLOCK() \
do { } while(osal_sem_mutex_give(gvrp_sem_db) != SYS_ERR_OK)
#define GVRP_FILL_PKT(pkt, field, offset, len) \
do {\
    osal_memcpy(&(pkt)->data[offset], (uint8*)(field), len);\
}while(0)
extern void systime_start_random_timer(uint32 process_id,
       void (*expiry_fn)(unsigned long),uint32 instance_id,uint32 timeout,struct timer_list *tEntry,
       gvrp_timer_type_t  tmType);

extern void systime_start_timer(uint32 process_id,
       void (*expiry_fn)(unsigned long),uint32 instance_id,uint32 timeout,struct timer_list *tEntry);

extern BOOL systime_stop_timer(struct timer_list *tEntry);

extern void systime_schedule(uint32 process_id,
       void (*expiry_fn)(unsigned long),uint32 instance_id,struct timer_list *tEntry);
extern void syserr_panic(void);
extern int32 gvrp_check_bridge_mode(uint16 srcPort);
extern int32 gvrp_remove_header(Gvr_pdu  *pdu);
extern int32 gid_trunk_add(Garp *application,TstGarpTrk stTrkInfo );
extern int32 gid_trunk_del(Garp *application,uint8 trunk_id);
#endif 

