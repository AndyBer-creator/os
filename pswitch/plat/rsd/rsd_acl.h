
#ifndef __RSD_ACL_H__
#define __RSD_ACL_H__

#include <rtk/acl.h>
#include <common/sys_def.h>
#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
#include <rsd/rsd_acl_msw.h>
#endif
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
#include <rsd/rsd_acl_csw.h>
#endif

#if 1
#define ACLDBG(string, args...)         SYS_DBG(DBG_ACLMGR_API, string, ##args)
#else
#define ACLDBG(string, args...)         printk("%s(%u) "string"\n", __FUNCTION__, __LINE__, ##args)
#endif

#define ACLDUMP(string, args...)        ACLDBG(string, ##args)
#define ACLDUMPDATA(string, args...)    ACLDBG(string, ##args)

typedef uint32                  sys_acl_bitmap_t;
#define RSD_ACL_BMP_BIT_LEN     (sizeof(sys_acl_bitmap_t) * 8)
#define RSD_ACL_BMP_WIDTH(_len)    (((_len) + RSD_ACL_BMP_BIT_LEN - 1) / RSD_ACL_BMP_BIT_LEN)
#define RSD_ACL_FIELD_BMP_SET(_mask, _field)    \
        _mask[_field / RSD_ACL_BMP_BIT_LEN] |= (1 << (_field % RSD_ACL_BMP_BIT_LEN))

#define RSD_ACL_FIELD_BMP_UNSET(_mask, _field)  \
        _mask[_field / RSD_ACL_BMP_BIT_LEN] &= ~(1 << (_field % RSD_ACL_BMP_BIT_LEN))

#define RSD_ACL_FIELD_BMP_CHK(_mask, _field)    \
        (_mask[_field / RSD_ACL_BMP_BIT_LEN] & (1 << (_field % RSD_ACL_BMP_BIT_LEN)))

#define _WRITE(_unit, _id, _phase, _field, _data, _mask)    \
do{                                                         \
    uint8 __ret = rtk_acl_ruleEntryField_write(_unit, _phase, _id , _field, _data, _mask); \
    if (0 != __ret)                                         \
    {                                                       \
        ACLDBG("err %u", __ret);                            \
        return SYS_ERR_FAILED;                              \
    }                                                       \
}while(0);

#define RSD_ACL_SPM_U32_XLATE_U8(_unit, _in, _out)                  \
do {                                                                \
    uint8   cpuPort = PHYSIC_CPU_PORT(_unit);                       \
    uint8   *p = (uint8 *)_in;                                      \
    uint8   max, len, base;                                         \
    uint8   i, j;                                                   \
    max = (cpuPort + SYS_BITS_PER_BYTE + 1) / SYS_BITS_PER_BYTE;    \
    len = SYS_BITS_PER_LONG / SYS_BITS_PER_BYTE;                    \
    for (i = 0; i < max; ++i)                                       \
    {                                                               \
        j = max - i - 1;                                            \
        base = ((j + len)/ len) * len;                              \
        _out[i] = p[(base - (j % len) - 1)];                        \
    }                                                               \
} while(0)

#define RSD_ACL_SPM_SET(_u, _phase, _idx, _inLpm, _isCareCpu)               \
do {                                                                        \
    sys_logic_portmask_t   _lpm;                                            \
    sys_physic_portmask_t   _inPpm, _ppm;                                   \
    uint8                   _data[SYS_ACL_ENTRY_BYTEMAX];                   \
    uint8                   _mask[SYS_ACL_ENTRY_BYTEMAX];                   \
    sys_logic_to_physic_portmask(&_inLpm, &_inPpm);                         \
    LOGIC_PORTMASK_CLEAR_ALL(_lpm);                                         \
    LOGIC_PORTMASK_SET_ALL(_lpm);                                           \
    sys_logic_to_physic_portmask(&_lpm, &_ppm);                             \
    PHYSIC_PORTMASK_ANDNOT(_inPpm, _ppm, _inPpm);                           \
    osal_memset(&_data, 0, SYS_ACL_ENTRY_BYTEMAX);                          \
    osal_memset(&_mask, 0, SYS_ACL_ENTRY_BYTEMAX);                          \
    if (!_isCareCpu)                                                        \
        PHYSIC_PORTMASK_SET_CPU_PORT(_inPpm, _u);                           \
    RSD_ACL_SPM_U32_XLATE_U8(_u, _inPpm.portmask[_u].bits, _mask);          \
    SYS_ERR_XLATE_CHK(rtk_acl_ruleEntryField_write(_u, _phase, _idx, USER_FIELD_SPM, _data, _mask));   \
} while(0)

#define RSD_ACL_LOCK()    \
do{                             \
    if (SYS_ERR_OK != osal_sem_mutex_take(g_acl_sem, OSAL_SEM_WAIT_FOREVER)){    \
        SYS_DBG(DBG_ACLMGR_API, "ACL Lock Error");     \
        return SYS_ERR_SEM_LOCK;}          \
}while(0)

#define RSD_ACL_UNLOCK()    \
do{                     \
    if (SYS_ERR_OK != osal_sem_mutex_give(g_acl_sem)){    \
        SYS_DBG(DBG_ACLMGR_API, "ACL unLock Error");     \
        return SYS_ERR_SEM_UNLOCK;}          \
}while(0)

#define RSD_ACL_INIT_CHK(_state)    \
do {                                \
    if (ENABLED != (_state))        \
        return SYS_ERR_FAILED;      \
} while(0)

#define ACL_TMPL_PROC_NAME      "acl_template"
#define ACL_TMPL_PROC_SEQNAME    acl_template
#define ACL_ENTRY_PROC_NAME     "acl_entry"
#define ACL_ENTRY_PROC_SEQNAME   acl_entry
#define ACL_BLOCK_PROC_NAME     "acl_block"
#define ACL_BLOCK_PROC_SEQNAME   acl_block
#define ACL_RANGE_PROC_NAME     "acl_range"
#define ACL_RANGE_PROC_SEQNAME   acl_range
#define ACL_METER_PROC_NAME     "acl_meter"
#define ACL_METER_PROC_SEQNAME   acl_meter
#define ACL_PROTO_PROC_NAME     "acl_proto"
#define ACL_PTOTO_PROC_SEQNAME   acl_proto

#define ACL_PROC_BUFLEN         61440

#define SYS_ACL_NOT_ALLOC       0xffff

typedef enum rsd_acl_aggregatorType_e
{
    ACL_AGGREGATORTYPE_NONE,
    ACL_AGGREGATORTYPE_DOUBLE,
    ACL_AGGREGATORTYPE_QUADRUPLE,
    ACL_AGGREGATORTYPE_END,
} rsd_acl_aggregatorType_t;

typedef enum sys_acl_tagType_e
{
    SYS_ACL_TAGTYPE_UNTAG,
    SYS_ACL_TAGTYPE_TAG,
    SYS_ACL_TAGTYPE_UNTAG_PRIOTAG,
    SYS_ACL_TAGTYPE_END,
} sys_acl_tagType_t;

typedef enum sys_acl_protoType_e
{
    ACL_PROTOTYPE_SYSTEM,

#ifdef CONFIG_SYS_USER_DEFINED_ACL
    ACL_PROTOTYPE_USER_MAC,
    ACL_PROTOTYPE_USER_IP,
    ACL_PROTOTYPE_USER_IP6,
#endif

#ifdef CONFIG_SYS_PROTO_ISG
    ACL_PROTOTYPE_IPSG,
#endif

#ifdef CONFIG_SYS_VOICEVLAN
    ACL_PROTOTYPE_VOICE_VL,
 #ifdef CONFIG_SYS_RTL8390
    ACL_PROTOTYPE_VOICE_VL_RMK, 
 #endif
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    ACL_PROTOTYPE_SURVEILLANCE_VL,
 #ifdef CONFIG_SYS_RTL8390
    ACL_PROTOTYPE_SURVEILLANCE_VL_RMK, 
 #endif
#endif

#ifdef CONFIG_SYS_PROTOCOL_VLAN
    ACL_PROTOTYPE_PROTO_VL,
#endif

#ifdef CONFIG_SYS_MACVLAN
    ACL_PROTOTYPE_MACVLAN,
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    ACL_PROTOTYPE_AUTH,
#endif

#ifdef CONFIG_SYS_L3_ROUTE
    ACL_PROTOTYPE_L3_ROUTE,
    ACL_PROTOTYPE_L3_NROUTE,
#endif

    ACL_PROTOTYPE_END
}sys_acl_protoType_t;

typedef enum sys_acl_tmpl_e
{
    ACL_TMPL_PRE0 = 0,
    ACL_TMPL_PRE1,
    ACL_TMPL_PRE2,
    ACL_TMPL_PRE3,
    ACL_TMPL_PRE4,
    ACL_TMPL_PRE_MAX,
    
    ACL_TMPL_COMMON,
    ACL_TMPL_SRC_MACIPVID,
    ACL_TMPL_END
}sys_acl_tmpl_t;

typedef enum sys_acl_fieldTyep_e
{
    ACL_FIELD_SPN,
    ACL_FIELD_DPN,
    ACL_FIELD_SPM,
    ACL_FIELD_DPM,
    ACL_FIELD_CARE_CPU,
    ACL_FIELD_FRAMETYPE,
    ACL_FIELD_DMAC,
    ACL_FIELD_SMAC,
    ACL_FIELD_DIP,
    ACL_FIELD_SIP,
    ACL_FIELD_VID,
    ACL_FIELD_FVID,
    ACL_FIELD_ETHERTYPE,
    ACL_FIELD_L4SPORT,
    ACL_FIELD_L4DPORT,
    ACL_FIELD_L4SPORTRNG,
    ACL_FIELD_L4DPORTRNG,
    ACL_FIELD_IPPROTO,
    ACL_FIELD_DSCP,
    ACL_FIELD_PRECEDENCE,
    ACL_FIELD_TCPFLAG,
    ACL_FIELD_ICMPTYPE,
    ACL_FIELD_ICMPCODE,
    ACL_FIELD_IGMPTYPE,
    ACL_FIELD_8021P,
    ACL_FIELD_MGMTVID,
    ACL_FIELD_SWITCHMAC,
    ACL_FIELD_VLANRNG,
    ACL_FIELD_L2,
    ACL_FIELD_IP4,
    ACL_FIELD_IP6,
    ACL_FIELD_IP,   
    ACL_FIELD_CTAG_STATUS,
    ACL_FIELD_STAG_STATUS,
    ACL_FIELD_PHASE,
    ACL_FIELD_DPRI,
    ACL_FIELD_STPDROP,
    ACL_FIELD_END,
} sys_acl_fieldType_t;

typedef enum sys_acl_actionTyep_e
{
    ACL_ACTION_PERMIT,
    ACL_ACTION_DROP,
    ACL_ACTION_TRAP,
    ACL_ACTION_COPYTOCPU,
    ACL_ACTION_FLOOD_IN_VLAN,
    ACL_ACTION_REDIRECT,
    ACL_ACTION_ROUTE,
    ACL_ACTION_MIRROR,
    ACL_ACTION_RL,
    ACL_ACTION_DSCPREMARK,
    ACL_ACTION_OUTERREMARK,
    ACL_ACTION_INNERREMARK,
    ACL_ACTION_PRIO,
    ACL_ACTION_PRIO_CPU,
    ACL_ACTION_CVID,
    ACL_ACTION_SVID,
    ACL_ACTION_CTAG,
    ACL_ACTION_CUNTAG,
    ACL_ACTION_STAG,
    ACL_ACTION_SUNTAG,
    ACL_ACTION_PORTSHUTDOWN,
    ACL_ACTION_SKIPIGRVLANFILTER,
    ACL_ACTION_BYPASS_STP,
    ACL_ACTION_BYPASS_RATECTRL,
    ACL_ACTION_BYPASS_IGRDROP,
    ACL_ACTION_LOG,
    ACL_ACTION_END,
} sys_acl_actionType_t;

#define ACL_ACTIONMASK_PERMIT               SYS_SHIFT_BIT(ACL_ACTION_PERMIT)
#define ACL_ACTIONMASK_DROP                 SYS_SHIFT_BIT(ACL_ACTION_DROP)
#define ACL_ACTIONMASK_TRAP                 SYS_SHIFT_BIT(ACL_ACTION_TRAP)
#define ACL_ACTIONMASK_COPYTOCPU            SYS_SHIFT_BIT(ACL_ACTION_COPYTOCPU)
#define ACL_ACTIONMASK_FLOOD_IN_VLAN        SYS_SHIFT_BIT(ACL_ACTION_FLOOD_IN_VLAN)
#define ACL_ACTIONMASK_REDIRECT             SYS_SHIFT_BIT(ACL_ACTION_REDIRECT)
#define ACL_ACTIONMASK_ROUTE                SYS_SHIFT_BIT(ACL_ACTION_ROUTE)
#define ACL_ACTIONMASK_MIRROR               SYS_SHIFT_BIT(ACL_ACTION_MIRROR)
#define ACL_ACTIONMASK_RL                   SYS_SHIFT_BIT(ACL_ACTION_RL)
#define ACL_ACTIONMASK_DSCPREMARK           SYS_SHIFT_BIT(ACL_ACTION_DSCPREMARK)
#define ACL_ACTIONMASK_OUTERREMARK          SYS_SHIFT_BIT(ACL_ACTION_OUTERREMARK)
#define ACL_ACTIONMASK_INNERREMARK          SYS_SHIFT_BIT(ACL_ACTION_INNERREMARK)
#define ACL_ACTIONMASK_PRIO                 SYS_SHIFT_BIT(ACL_ACTION_PRIO)
#define ACL_ACTIONMASK_PRIO_CPU             SYS_SHIFT_BIT(ACL_ACTION_PRIO_CPU)
#define ACL_ACTIONMASK_CVID                 SYS_SHIFT_BIT(ACL_ACTION_CVID)
#define ACL_ACTIONMASK_SVID                 SYS_SHIFT_BIT(ACL_ACTION_SVID)
#define ACL_ACTIONMASK_CTAG                 SYS_SHIFT_BIT(ACL_ACTION_CTAG)
#define ACL_ACTIONMASK_CUNTAG               SYS_SHIFT_BIT(ACL_ACTION_CUNTAG)
#define ACL_ACTIONMASK_STAG                 SYS_SHIFT_BIT(ACL_ACTION_STAG)
#define ACL_ACTIONMASK_SUNTAG               SYS_SHIFT_BIT(ACL_ACTION_SUNTAG)
#define ACL_ACTIONMASK_PORTSHUTDOWN         SYS_SHIFT_BIT(ACL_ACTION_PORTSHUTDOWN)
#define ACL_ACTIONMASK_SKIPIGRVLANFILTER    SYS_SHIFT_BIT(ACL_ACTION_SKIPIGRVLANFILTER)
#define ACL_ACTIONMASK_BYPASS_STP           SYS_SHIFT_BIT(ACL_ACTION_BYPASS_STP)
#define ACL_ACTIONMASK_BYPASS_RATECTRL      SYS_SHIFT_BIT(ACL_ACTION_BYPASS_RATECTRL)
#define ACL_ACTIONMASK_BYPASS_IGRDROP       SYS_SHIFT_BIT(ACL_ACTION_BYPASS_IGRDROP)
#define ACL_ACTIONMASK_LOG                  SYS_SHIFT_BIT(ACL_ACTION_LOG)

typedef union sys_acl_entryKey_s
{
    uint16 idx;

    struct
    {
        uint32 type;
    } rsv;

#ifdef CONFIG_SYS_USER_DEFINED_ACL
    struct
    {
        sys_pri_t priority;
        uint32 aclIdx;
    } usr;
#endif

#ifdef CONFIG_SYS_PROTOCOL_VLAN
    struct
    {
        sys_logic_port_t port;
        uint32 grpIdx;
    } protoVlan;
#endif

#ifdef CONFIG_SYS_MACVLAN
    struct
    {
        sys_logic_port_t lp;
        sys_mac_t mac;
        sys_mac_t macMask;
    } macVlan;
#endif

#ifdef CONFIG_SYS_L3_ROUTE
    struct
    {
        sys_l3_af_t af;
        sys_l3_ip_t ip;
        sys_ip_t    nh;
    } l3Route;
#endif
} sys_acl_entryKey_t;

typedef struct sys_acl_rateLimit_s
{
    uint16              meterId;
    uint16              rateId;     
#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
    uint16              rate;
    uint16              burstSize;
#endif
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
    rtk_acl_meterEntry_t meterEntry;
#endif
} sys_acl_rateLimit_t;

typedef struct sys_acl_action_s
{
    sys_acl_rateLimit_t rate;
    uint32              actionMask;
    uint32              counterIdx;
    uint32              routeIdx;
    uint32              isDfltRoute;
    sys_vid_t           cvid;
    sys_vid_t           svid;
    sys_logic_port_t    lp;
    uint8               mirrorIdx;
    uint8               rmkVal;
    uint8               prio;
} sys_acl_action_t;

typedef struct sys_acl_entry_s
{
    uint32              idx;
    sys_enable_t        status;
    sys_enable_t        valid;
    sys_enable_t        counterSts;
    sys_acl_protoType_t type;
    sys_acl_entryKey_t  key;
    sys_acl_action_t    action;
} sys_acl_entry_t;

typedef struct sys_acl_cb_s
{
    int32   (*entry_prio_cmp)   (sys_acl_entry_t *, sys_acl_entry_t *);
    int32   (*entry_info_dump)  (sys_acl_entry_t *);
    int32   (*entry_str_get)    (sys_acl_entry_t *, char *, uint32);
} sys_acl_cb_t;

typedef struct sys_acl_field_s
{
    sys_acl_bitmap_t     fieldMask[RSD_ACL_BMP_WIDTH(ACL_FIELD_END)];
    sys_logic_port_t     spn;
    sys_logic_port_t     dpn;
    sys_logic_portmask_t spm;
    sys_logic_portmask_t dpm;
    sys_mac_t            dmac;
    sys_mac_t            smac;
    sys_mac_t            dmacMask;
    sys_mac_t            smacMask;
    sys_ipv6_t           sip6;
    sys_ipv6_t           dip6;
    sys_ipv6_t           sipMask6;
    sys_ipv6_t           dipMask6;
    sys_ipv4_t           sip;
    sys_ipv4_t           dip;
    sys_ipv4_t           sipMask;
    sys_ipv4_t           dipMask;
    uint32               vlanRngIdx;
    uint16               vid;
    uint16               fvid;
    uint16               ethType;
    uint16               srcPort;
    uint16               dstPort;
    uint8                l2FrameType;
    uint8                l4SrcPortRngIdx;
    uint8                l4DstPortRngIdx;
    uint8                protol;
    uint8                prio;
    uint8                dscp;
    uint8                ipPrecedence;
    uint8                tcpFlag;
    uint8                tcpFlagMask;
    uint8                icmpType;
    uint8                icmpCode;
    uint8                igmpType;
    uint8                value8021p;
    uint8                mask8021p;
    uint8                ctagStatus;     
    uint8                stagStatus;     
    uint8                stpDrop;
    uint8                stpDropMask;
} sys_acl_field_t;

typedef struct sys_acl_text_s
{
    uint32 type;
    char text[SYS_BUF32_LEN];
} sys_acl_text_t;

typedef struct rsd_acl_templateDef_s
{
    sys_acl_tmpl_t      type;
    rtk_acl_template_t  templateField;
} rsd_acl_templateDef_t;

typedef struct rsd_acl_protoInfo_s
{
    sys_acl_protoType_t type;
    uint16 blkEntryMax;     
    uint8 phase;
    uint8 blkMin;           
    uint8 blkMax;           
    uint8 blkNum;           
    uint8 tmplteNum;        
    uint8 entryNum;         
} rsd_acl_protoInfo_t;

typedef struct sys_logic_block_4show_s
{
    uint32  maxEnties;
    uint32  usage[ACL_PROTOTYPE_END];
} sys_logic_block_4show_t;

#define SYS_ACL_TMPL_NUM_MAX        (10)
#define SYS_ACL_TMPLNUM_PER_BLK     RTK_MAX_NUM_OF_ACL_BLOCK_TEMPLATE
#define SYS_ACL_TMPL_FIX_MAX        (5)
#define SYS_ACL_TMPL_USR_MAX        (SYS_ACL_TMPL_NUM_MAX - SYS_ACL_TMPL_FIX_MAX) 
#define SYS_ACL_TMPL_FIELD_MAX      RTK_MAX_NUM_OF_ACL_TEMPLATE_FIELD

#define SYS_ACL_BLK_NUM_MAX         (32)    
#define SYS_ACL_ENTRYNUM_PER_BLK    (128)   
#define SYS_ACL_ENTRY_BYTEMAX       (2 * SYS_ACL_TMPL_FIELD_MAX)

#define SYS_ACL_METER_BLK_MAX           (16)
#define SYS_ACL_METER_ENTRYNUM_PER_BLK  (128)
#define SYS_ACL_METER_ENTRY_MAX         (SYS_ACL_METER_BLK_MAX * SYS_ACL_METER_ENTRYNUM_PER_BLK)

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)

#define SYS_ACL_LOG_BLK_MAX             (16)
#define SYS_ACL_LOG_ENTRYNUM_PER_BLK    (256)
#define SYS_ACL_LOG_ENTRY_MAX           (SYS_ACL_LOG_ENTRYNUM_PER_BLK * SYS_ACL_LOG_BLK_MAX)
#define RSD_ACL_LOG_BMP_WIDTH           (RSD_ACL_BMP_WIDTH(SYS_ACL_LOG_ENTRYNUM_PER_BLK))

#define SYS_ACL_VLAN_RANGE_ENTRY_NUM_MAX            (16)    
#define SYS_ACL_VLAN_RANGE_NUM_PER_TEMPLATE         (16)    

#define SYS_ACL_L4PORT_RANGE_MAX                    (8)     
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
#define SYS_ACL_RANGE_ENTRY_NUM_MAX                 (16)    
#endif

typedef struct rsd_acl_jioc_s
{
    sys_acl_cb_t srv_cb[ACL_PROTOTYPE_END];
    int32 floodPmaskIdx;           
    int32 normalFloodPmaskIdx;     
    uint32 totalEntry;
    uint32 totalBlock;
    uint32 totalTemplate;
    uint32 totalFixTemplate;
    uint32 totalTemplateFiled;
    uint32 totalMeterBlock;
    uint32 entryPerBlock;
    uint32 templatePerBlock;
    uint32 meterPerMeterBlock;

    struct
    {
        uint32 defPhase;
        uint32 prio;
        uint32 tmplIdx[2][SYS_ACL_TMPLNUM_PER_BLK];
        rsd_acl_aggregatorType_t aggregateType;
        sys_text_t protoName;
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
        uint32 entryUsage;
        uint32 blockUsage;
        uint32 logicId2BlkId[SYS_ACL_BLK_NUM_MAX]; 
#endif

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
        uint32 meterUsed;
#endif
    } protoDef[ACL_PROTOTYPE_END];

    struct
    {
        sys_acl_protoType_t type;
        sys_acl_entry_t entries[SYS_ACL_ENTRYNUM_PER_BLK];
        uint32 baseBlk;   
        uint32 usedEntryNum;
        uint32 tmplIdx[SYS_ACL_TMPLNUM_PER_BLK];
        uint32 meterBlk[SYS_ACL_METER_BLK_MAX]; 
        uint32 meterBlkUsage[SYS_ACL_METER_BLK_MAX];
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
        uint32 logicIdx;
        uint32 inPhase;
#endif
    } blockInfo[SYS_ACL_BLK_NUM_MAX];

    struct
    {
        sys_acl_tmpl_t type;
        uint32 blkUsed;
    } templateInfo[SYS_ACL_TMPL_USR_MAX];

    struct
    {
        struct
        {
            rtk_acl_meterEntry_t meterEntry;
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            uint32 refAclEntryIdx; 
#endif
            uint32 usedCnt;
        } meterEntry[SYS_ACL_METER_ENTRYNUM_PER_BLK];

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
        rtk_acl_meterMode_t mode;
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
        uint32 entryUsedCnt;
#endif
        uint32 refAclBlk; 
    }meterBlkInfo[SYS_ACL_METER_BLK_MAX];

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
    struct
    {
        rtk_pie_rangeCheck_t info;
        uint32 usedCnt; 
    } range[SYS_ACL_RANGE_ENTRY_NUM_MAX];
#endif

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
    struct
    {
        sys_acl_bitmap_t idPool[RSD_ACL_BMP_WIDTH(SYS_ACL_METER_ENTRY_MAX)];
        uint32 meterRef[SYS_ACL_METER_ENTRY_MAX];
    } rateInfo;

    struct
    {
        rtk_acl_rangeCheck_vid_t info;
        uint32 usedCnt; 
    } vlanRange[SYS_ACL_VLAN_RANGE_ENTRY_NUM_MAX];

    struct
    {
        rtk_acl_rangeCheck_l4Port_t info;
        uint32 usedCnt; 
    } l4portRange[SYS_ACL_L4PORT_RANGE_MAX];

    struct
    {
        sys_acl_bitmap_t logEntries[RSD_ACL_LOG_BMP_WIDTH]; 
        uint32 usedBlk; 
        uint32 usedCnt; 
    } logInfo[SYS_ACL_LOG_BLK_MAX];

    uint32 totalLogBlock;
    uint32 logPerLogBlock;
    uint32 cutline;
    rtk_acl_meterBurstSize_t    burstSize[METER_MODE_END];
#endif
} rsd_acl_jioc_t;

typedef struct rsd_acl_mapper_s
{
    int32 (*rsd_acl_init)(void );
    int32 (*rsd_acl_protoInfo_get)(sys_acl_protoType_t protoType, rsd_acl_protoInfo_t *info);
    int32 (*rsd_acl_entry_add)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_acl_action_t *action, sys_acl_field_t *info, sys_enable_t enable);
    int32 (*rsd_acl_entry_get)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_acl_entry_t *entry);
    int32 (*rsd_acl_entry_del)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key);
    int32 (*rsd_acl_entry_clr)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key);
    int32 (*rsd_acl_entrySpm_set)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_logic_portmask_t *lportmask);
    int32 (*rsd_acl_entryInfo_set)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_acl_field_t *pRuleInfo);
    int32 (*rsd_acl_entryEnable_set)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_enable_t enable);
    int32 (*rsd_acl_entryAction_set)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_acl_action_t *pRuleAction);
    int32 (*rsd_acl_entryAction_get)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_acl_action_t *pRuleAction);
    int32 (*rsd_acl_entryCounter_get)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, uint64 *counter);
    int32 (*rsd_acl_entryCounter_clr)(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key);
} rsd_acl_mapper_t;

#define PROTO_NAME(_proto)                    g_acl_JIOC.protoDef[(_proto)].protoName.text
#define PROTO_PHASE(_proto)                   g_acl_JIOC.protoDef[(_proto)].defPhase
#define PROTO_BLK_PRIO(_proto)                g_acl_JIOC.protoDef[(_proto)].prio
#define PROTO_AGGREGATOR(_proto)              g_acl_JIOC.protoDef[(_proto)].aggregateType
#define PROTO_TEMPLATE(_proto, _idx1, _idx2)  g_acl_JIOC.protoDef[(_proto)].tmplIdx[_idx1][_idx2]
#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
#define PROTO_METER_USED(_proto)              g_acl_JIOC.protoDef[(_proto)].meterUsed
#endif
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
#define PROTO_BLOCK_USAGE(_proto)             g_acl_JIOC.protoDef[(_proto)].blockUsage
#define PROTO_ENTRY_USAGE(_proto)             g_acl_JIOC.protoDef[(_proto)].entryUsage
#define PROTO_LOGICID2BLKID(_proto, _id)      g_acl_JIOC.protoDef[(_proto)].logicId2BlkId[_id]
#endif

#define BLK_INFO(_i)                    g_acl_JIOC.blockInfo[(_i)]
#define BLK_BASE(_i)                    BLK_INFO((_i)).baseBlk
#define BLK_TYPE(_i)                    BLK_INFO((_i)).type
#define BLK_USAGE(_i)                   BLK_INFO((_i)).usedEntryNum
#define BLK_TEMPLATE(_i, _idx)          BLK_INFO((_i)).tmplIdx[_idx]
#define BLK_ENTRY(_i, _j)               BLK_INFO((_i)).entries[(_j)]
#define BLK_ENTRY_HWIDX(_i, _j, _k)     ((SYS_ACL_ENTRYNUM_PER_BLK * (_i)) + (_j) + (_k))
#define BLK_METER_BLK(_i, _j)           BLK_INFO((_i)).meterBlk[(_j)]
#define BLK_METER_BLKUSAGE(_i, _j)      BLK_INFO((_i)).meterBlkUsage[(_j)]
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
#define BLK_IN_PHASE(_i)                BLK_INFO((_i)).inPhase
#define BLK_LOGIC_ID(_i)                BLK_INFO((_i)).logicIdx
#endif

#define METER_INFO(_i)                  g_acl_JIOC.meterBlkInfo[(_i)]
#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
#define METER_BLK_MODE(_i)              METER_INFO((_i)).mode
#endif
#define METER_BLK_ENTRY_USAGE(_i, _j)   METER_BLK_ENTRY((_i), (_j)).usedCnt
#define METER_BLK_REFBLK(_i)            METER_INFO((_i)).refAclBlk
#define METER_BLK_ENTRY(_i, _j)         METER_INFO((_i)).meterEntry[(_j)]
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
#define METER_BLK_ENTRY_USED_CNT(_i)    METER_INFO((_i)).entryUsedCnt
#define METER_BLK_ENTRY_REFACLENTRY(_i, _j) METER_BLK_ENTRY((_i), (_j)).refAclEntryIdx
#endif

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
#define RATE_INFO()                     g_acl_JIOC.rateInfo
#define RATE_ID_POOL(_i)                RATE_INFO().idPool[(_i)]
#define RATE_METER_REF(_i)              RATE_INFO().meterRef[(_i)]
#endif

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
#define ACL_LOG_INFO(_i)                g_acl_JIOC.logInfo[(_i)]
#define ACL_LOG_BLK_ENTRIES(_i)         ACL_LOG_INFO((_i)).logEntries
#define ACL_LOG_BLK_USEDBLK(_i)         ACL_LOG_INFO((_i)).usedBlk
#define ACL_LOG_BLK_USAGE(_i)           ACL_LOG_INFO((_i)).usedCnt

#define ACL_VLRNG_ENTRY(_i)             g_acl_JIOC.vlanRange[(_i)]
#define ACL_VLRNG_ENTRYINFO(_i)         ACL_VLRNG_ENTRY((_i)).info
#define ACL_VLRNG_ENTRY_USAGE(_i)       ACL_VLRNG_ENTRY((_i)).usedCnt

#define ACL_L4PORTRNG_ENTRY(_i)         g_acl_JIOC.l4portRange[(_i)]
#define ACL_L4PORTRNG_ENTRYINFO(_i)     ACL_L4PORTRNG_ENTRY((_i)).info
#define ACL_L4PORTRNG_ENTRY_USAGE(_i)   ACL_L4PORTRNG_ENTRY((_i)).usedCnt
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
#define ACL_RANGE_ENTRY(_i)             g_acl_JIOC.range[(_i)]
#define ACL_RANGE_ENTRYINFO(_i)         ACL_RANGE_ENTRY((_i)).info
#define ACL_RANGE_ENTRY_USAGE(_i)       ACL_RANGE_ENTRY((_i)).usedCnt
#endif

#define ACL_CUTELINE                    g_acl_JIOC.cutline
#define ACL_BLOCKNUM_MAX                g_acl_JIOC.totalBlock
#define ACL_ENTRYNUM_MAX                g_acl_JIOC.totalEntry
#define ACL_ENTRYNUM_PER_BLK            g_acl_JIOC.entryPerBlock
#define ACL_TMPLNUM_MAX                 g_acl_JIOC.totalTemplate
#define ACL_TMPLFIXNUM_MAX              g_acl_JIOC.totalFixTemplate
#define ACL_TMPLFIELD_MAX               g_acl_JIOC.totalTemplateFiled
#define ACL_TMPLNUM_PER_BLK             g_acl_JIOC.templatePerBlock

#define ACL_TMPLTE(_idx)                g_acl_JIOC.templateInfo[(_idx)]
#define ACL_BURST(_mode)                g_acl_JIOC.burstSize[(_mode)]
#define ACL_FLOODPMASKIDX               g_acl_JIOC.floodPmaskIdx
#define ACL_NORMALFLOODPMASKIDX         g_acl_JIOC.normalFloodPmaskIdx
#define ACL_METER_BLK_MAX               g_acl_JIOC.totalMeterBlock
#define ACL_METER_BLK_ENTRY_MAX         g_acl_JIOC.meterPerMeterBlock
#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
#define ACL_LOG_BLK_MAX                 g_acl_JIOC.totalLogBlock
#define ACL_LOG_BLK_ENTRY_MAX           g_acl_JIOC.logPerLogBlock
#endif
#define ACL_SRV_CB(_type)               (g_acl_JIOC.srv_cb[_type])

extern rsd_acl_jioc_t g_acl_JIOC;
extern osal_mutex_t g_acl_sem;
extern sys_enable_t g_acl_init;
extern rsd_acl_templateDef_t g_acl_templatePreDef[SYS_ACL_TMPL_FIX_MAX];
extern rsd_acl_templateDef_t g_acl_templateDef[SYS_ACL_TMPL_USR_MAX];
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
extern rtk_acl_phase_t RSD_ACL_PHASE_VNGRESS;
#endif
extern rtk_acl_phase_t RSD_ACL_PHASE_INGRESS;
#ifdef CONFIG_SYS_RTL8390
extern rtk_acl_phase_t RSD_ACL_PHASE_EGRESS;
#endif
extern sys_acl_text_t g_tmpl_fields_text[];
extern sys_acl_text_t g_entry_fields_text[];
extern sys_acl_text_t g_action_fields_text[];
extern sys_acl_text_t g_phase_text[];
extern sys_acl_text_t g_rangeChk_text[];
extern sys_acl_text_t g_meterMode_text[];
extern sys_acl_text_t g_meterType_text[];

extern int32 rsd_acl_cb_register(sys_acl_protoType_t type, sys_acl_cb_t *cb_info);

extern int32 rsd_acl_entry_add(sys_acl_protoType_t type, sys_acl_entryKey_t *key, sys_acl_action_t *action, sys_acl_field_t *info, sys_enable_t enable);

extern int32 rsd_acl_entry_get(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_acl_entry_t *entry);

extern int32 rsd_acl_entry_del(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key);

extern int32 rsd_acl_entry_clr(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key);

extern int32 rsd_acl_entrySpm_set(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_logic_portmask_t *lportmask);

extern int32 rsd_acl_entryInfo_set(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_acl_field_t *pRuleInfo);

extern int32 rsd_acl_entryEnable_set(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_enable_t enable);

extern int32 rsd_acl_entryAction_set(sys_acl_protoType_t type, sys_acl_entryKey_t *key, sys_acl_action_t *pRuleAction);

extern int32 rsd_acl_entryAction_get(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, sys_acl_action_t *pRuleAction);

extern int32 rsd_acl_entryAction_byIdx_get(uint32 idx, sys_acl_action_t *action);

extern int32 rsd_acl_entryCounter_get(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key, uint64 *counter);

extern int32 rsd_acl_entryCounter_clr(sys_acl_protoType_t protoType, sys_acl_entryKey_t *key);

extern int32 rsd_acl_typeAvailable_get(sys_acl_protoType_t protoType, uint32 *entryNum);

extern int32 rsd_acl_floodPortMask_set(sys_logic_portmask_t newPM);

extern int32 rsd_acl_entry_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_acl_block_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_acl_range_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_acl_meter_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_acl_tmpl_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_acl_proto_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_acl_init(void);

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
extern rsd_acl_mapper_t _acl_msw_mapper;
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
extern rsd_acl_mapper_t _acl_csw_mapper;
#endif

int32 rsd_acl_typeMeterAvailable_get(sys_acl_protoType_t protoType, uint32 *entryNum);

int32 rsd_acl_info_dump(void *data);
#endif

