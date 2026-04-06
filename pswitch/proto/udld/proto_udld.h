
#ifndef __PROTO_UDLD_H__
#define __PROTO_UDLD_H__
#include <common/sys_type.h>
#include <rsd/rsd_port.h>
#define CAPA_UDLD_NEIGHBOR_NUM          5
#define CAPA_UDLD_ECHO_NUM              5
#define MAX_TLV_TYPE                    8
#define MAX_TLV_VALUE_LEN               (128-4)
#define MAX_ID_LEN                      (128)
#define UDLD_HEADER_LEN                 4
#define TLV_MIN_LEN                     4
#define TLV_HEADER_LEN                  4
#define UDLD_SEQ_TLV_LEN                8
#define TIMEOUT_TLV_LEN                 5
#define TIMEOUT_TLV_VAL                 5
#define UDLD_PID                        0x0111
#define CHECKSUM_OFFSET                 2
#define UDLD_LENGTH_OFFSET              2
#define TYPE_OFFSET                     2
#define VLANID_OFFSET                   4
#define DA_SA_OFFSET                    12
#define SA_OFFSET                       6
#define UDLDH_OFFSET                    22
#define UDLDTLV_OFFSET                  26
#define SNAPLEN_OFFSET                  12
#define FLAG_OFFSET                     23
#define FRMAE_LEN_OFFSET                2
#define LLC_LEN_OFFSET                  8
#define LLC_PID_OFFSET                  6
#define ECHO_PAIRSNUM_OFFSET            4
#define ECHO_LEN_OFFSET                 2
#define TEMPLATE_TX_ULDLD               512
#define UDLD_DEVICE_ID_TLV              0x0001
#define UDLD_PORT_ID_TLV                0x0002
#define UDLD_ECHO_TLV                   0x0003
#define UDLD_MESSAGE_INTERVAL_TLV       0x0004
#define UDLD_TIMEOUT_INTERVAL_TLV       0x0005
#define UDLD_DEVICE_NAME_TLV            0x0006
#define UDLD_SEQ_NUMBER_TLV             0x0007
#define OPCODE_RESERVED                 0x00
#define OPCODE_PROBE                    0x01
#define OPCODE_ECHO                     0x02
#define OPCODE_FLUSH                    0x03
#define DEFAULT_FLAG                    0x00
#define RT_RSY_FLAG                     0x03
#define RT_FLAG                         0x01

#define TLL_R                           3   
#define N_REESTABLISH_PROBE             8   

#define LINKUP_PHASE1_TIME              1
#define LINKUP_PHASE2_TIME              7
#define DETECTION_PHASE1_TIME           1
#define DETECTION_PHASE2_TIME           7
#define DETECTION_PHASE_N_ECHO_REPLY    5

typedef enum sys_udld_mode_s
{
    UDLD_MODE_DISABLE = 0,
    UDLD_MODE_NORMAL,
    UDLD_MODE_AGGRESSIVE,
    UDLD_MODE_END
} sys_udld_mode_t;

typedef enum sys_udld_bidir_state_s
{
    UDLD_BIDIR_UNDETERMINED = 0,
    UDLD_BIDIR_SELF_LOOP,
    UDLD_BIDIR_NEIGHBOR_ECHO_EMPTY,
    UDLD_BIDIR_NEIGHBOR_ECHO_MISMATCH,
    UDLD_BIDIR_BIDIRECTIONAL,
    UDLD_BIDIR_UNIDIRECTIONAL,
    UDLD_BIDIR_END
} sys_udld_bidir_state_t;

typedef enum sys_udld_op_state_s
{
    UDLD_OP_LINKDOWN = 0,
    UDLD_OP_INACTIVE,
    UDLD_OP_LINKUP_P1,
    UDLD_OP_LINKUP_P2,
    UDLD_OP_DETECTION,
    UDLD_OP_EXTENDED_DETECTION,
    UDLD_OP_ADVERTISEMENT_SINGLE,
    UDLD_OP_ADVERTISEMENT_MULTI,
    UDLD_OP_ERROR_DISABLE,
    UDLD_OP_REESTABLISH,
    UDLD_OP_END
} sys_udld_op_state_t;

typedef enum sys_udld_echoTlv_s
{
  UDLD_DEFUALT_ECHO = 0,
  UDLD_EMPTY_ECHO,
  UDLD_MATCH_ECHO,
  UDLD_MATCH_MULTI_ECHO,
  UDLD_UNMATCH_ECHO,
  UDLD_ECHO_END
} sys_udld_echoTlv_t;

typedef enum sys_udld_neighbor_save_result_s
{
  UDLD_NEIGHBOR_NEW = 0,
  UDLD_NEIGHBOR_OLD,
  UDLD_NEIGHBOR_FAILED,
  UDLD_NEIGHBOR_END
} sys_udld_neighbor_save_result_t;
typedef struct sys_udld_tok_s
{
    uint8 type;
    char  value[SYS_BUF32_LEN];
} sys_udld_tok_t;

typedef struct udld_pkt_s
{
    uint8 version:3   __attribute__((packed)) ;
    uint8 opcode:5    __attribute__((packed)) ;
    uint8 flags;
    uint16 checksum;
} __attribute__((packed)) udld_pkt_t;

typedef struct sys_udld_echo_pair_s
{
    char echo_device_id[MAX_ID_LEN];
    char echo_port_id[MAX_ID_LEN];
}sys_udld_echo_pair_t;

typedef struct sys_udld_neighbor_entry_s
{
    char status;
    char device_id[MAX_TLV_VALUE_LEN];
    char port_id[MAX_TLV_VALUE_LEN];
    char deviceName[MAX_TLV_VALUE_LEN];
    uint8 messageInterval;
    uint8 timeOutInterval;
    uint8 reSyncChance;
    uint16 hold_timer;
    uint32 echoNum;
    sys_udld_echo_pair_t *pEchoPair;
    struct sys_udld_neighbor_entry_s *pPreNbr;
    struct sys_udld_neighbor_entry_s *pNextNbr;
}sys_udld_neighbor_entry_t;

typedef struct sys_udld_port_entry_s
{
    uint8 udld_modeFollowGlobal;
    uint8 doRSY;
    uint8 rxPDU;
    uint8 rxNewPDU;
    uint8 lastTimeOut;
    uint8 lastFlush;
    uint16 sending_timer;
    uint16 neighborNum;
    uint32 txCount;
    sys_udld_mode_t udld_cfgMode;
    sys_udld_mode_t udld_operMode;
    sys_udld_op_state_t operational_state;
    sys_port_linkStatus_t port_link;
    sys_udld_bidir_state_t bidirectional_state;
    sys_udld_neighbor_entry_t *pNeighbor;
    sys_udld_neighbor_entry_t *pRxNeighbor;
}sys_udld_port_entry_t;

typedef struct sys_udld_port_table_s
{
    sys_udld_port_entry_t port_entry[SYS_LOGICPORT_NUM_MAX];
} sys_udld_port_table_t;

typedef struct sys_udld_echo_info_s
{
    uint8 valid;
    sys_udld_echo_pair_t echo;
}sys_udld_echo_info_t;
typedef struct sys_udld_neighbor_info_s
{
    char device_id[MAX_TLV_VALUE_LEN];
    char port_id[MAX_TLV_VALUE_LEN];
    char deviceName[MAX_TLV_VALUE_LEN];
    uint8 valid;
    uint8 status;
    uint8 messageInterval;
    uint8 timeOutInterval;
    uint16 hold_timer;
    uint32 echoNum;
}sys_udld_neighbor_info_t;
#define TLV_VALUE_LEN(len)          (((len)<TLV_MIN_LEN)?TLV_MIN_LEN:len)
#define HTONS(val)      (val)
#define READ16(ptr)     ((*(ptr) << 8) | (*(ptr + 1)))
#define READ32(ptr)     ((*(ptr) << 24) | (*(ptr + 1) << 16) | (*(ptr + 2) << 8) | *(ptr + 3))
#define WRITE32(ptr, val)\
{\
    *(ptr) = ((val) & 0xff000000) >> 24;\
    *(ptr + 1) = ((val) & 0x00ff0000) >> 16;\
    *(ptr + 2) = ((val) & 0x0000ff00) >> 8;\
    *(ptr + 3) = (val) & 0x000000ff;\
}
#define WRITE16(ptr, val)\
{\
    *(ptr) = ((val) & 0xff00) >> 8;\
    *(ptr + 1) = ((val) & 0x00ff);\
}
extern int32 udld_fiberMode_set(sys_udld_mode_t mode);
extern int32 udld_fiberMode_get(sys_udld_mode_t *pMode);
extern int32 udld_messageTime_set(uint32 interval);
extern int32 udld_messageTime_get(uint32 *pInterval);
extern int32 udld_portMode_set(sys_logic_port_t port, sys_udld_mode_t mode, uint8 isFollow);
extern int32 udld_portMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode, uint8 *pIsFollow);
extern int32 udld_portOperMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode);
extern int32 udld_portBidiretionState_get(sys_logic_port_t port, sys_udld_bidir_state_t *pBidirState);
extern int32 udld_portOperState_get(sys_logic_port_t port, sys_udld_op_state_t *pOperational_state);
extern int32 udld_portNeighborNumber_get(sys_logic_port_t port, uint16 *pNeighborNum);
extern int32 udld_portNeighbor_get(sys_logic_port_t port, uint16 neighborId, sys_udld_neighbor_info_t *pNbrInfo);
extern int32 udld_portNeighborEcho_get(sys_logic_port_t port, uint16 neighborId, uint16 echoId, sys_udld_echo_info_t  *pEchoInfo);
extern int32 udld_error_disable_reset(void);
extern int32 udld_init(void);
extern int32 udld_exit(void);
#endif

