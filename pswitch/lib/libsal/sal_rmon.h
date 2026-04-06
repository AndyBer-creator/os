
#ifndef __SAL_RMON_H__
#define __SAL_RMON_H__

#include <common/sys_def.h>
#include <libsal/sal_snmp.h>

#define RMON_COMMAND_TMPFILE        "/tmp/rmon_tmp"
#define RMON_COMMAND_POSTFIX        "1>> "RMON_COMMAND_TMPFILE" 2>> "RMON_COMMAND_TMPFILE
#define RMON_COMMAND_NULL_POSTFIX   "1>> /dev/null 2>> /dev/null"

#define RMON_EVENT_OID              ".1.3.6.1.2.1.16.9.1.1."
#define RMON_EVENT_INDEX_OID        RMON_EVENT_OID"1."
#define RMON_EVENT_DESCRIPTION_OID  RMON_EVENT_OID"2."
#define RMON_EVENT_TYPE_OID         RMON_EVENT_OID"3."
#define RMON_EVENT_COMMUNITY_OID    RMON_EVENT_OID"4."
#define RMON_EVENT_LASTTIME_OID     RMON_EVENT_OID"5."
#define RMON_EVENT_OWNER_OID        RMON_EVENT_OID"6."
#define RMON_EVENT_STATUS_OID       RMON_EVENT_OID"7."

#define RMON_EVENT_LOG_DFL          200

#define RMON_EVENT_LOG_OID          "iso.3.6.1.2.1.16.9.2.1."
#define RMON_EVENT_LOG_EVIDX_OID    RMON_EVENT_LOG_OID"1."
#define RMON_EVENT_LOG_INDEX_OID    RMON_EVENT_LOG_OID"2."
#define RMON_EVENT_LOG_TIME_OID     RMON_EVENT_LOG_OID"3."
#define RMON_EVENT_LOG_DESC_OID     RMON_EVENT_LOG_OID"4."

#define RMON_ALARM_OID              ".1.3.6.1.2.1.16.3.1.1."
#define RMON_ALARM_INDEX_OID        RMON_ALARM_OID"1."
#define RMON_ALARM_INTERVAL_OID     RMON_ALARM_OID"2."
#define RMON_ALARM_VARIABLE_OID     RMON_ALARM_OID"3."
#define RMON_ALARM_SAMPLE_TYPE_OID  RMON_ALARM_OID"4."
#define RMON_ALARM_ALARM_VALUE_OID  RMON_ALARM_OID"5."
#define RMON_ALARM_START_ALARM_OID  RMON_ALARM_OID"6."
#define RMON_ALARM_RISING_TH_OID    RMON_ALARM_OID"7."
#define RMON_ALARM_FALLING_TH_OID   RMON_ALARM_OID"8."
#define RMON_ALARM_RISING_EVENT_OID RMON_ALARM_OID"9."
#define RMON_ALARM_FALLING_EVENT_OID RMON_ALARM_OID"10."
#define RMON_ALARM_OWNER_OID        RMON_ALARM_OID"11."
#define RMON_ALARM_STATUS_OID       RMON_ALARM_OID"12."

#define RMON_HISTORY_BUCKET_DFL     50
#define RMON_HISTORY_INTERVAL_DFL   1800

#define RMON_HISTORY_OID            ".1.3.6.1.2.1.16.2.1.1."
#define RMON_HISTORY_INDEX_OID      RMON_HISTORY_OID"1."
#define RMON_HISTORY_INTERFACE_OID  RMON_HISTORY_OID"2."
#define RMON_HISTORY_BUCKET_REQUEST_OID  RMON_HISTORY_OID"3."
#define RMON_HISTORY_BUCKET_GRANTED_OID  RMON_HISTORY_OID"4."
#define RMON_HISTORY_INTERVAL_OID   RMON_HISTORY_OID"5."
#define RMON_HISTORY_OWNER_OID      RMON_HISTORY_OID"6."
#define RMON_HISTORY_STATUS_OID     RMON_HISTORY_OID"7."

#define RMON_HISTORY_LOG_OID                "iso.3.6.1.2.1.16.2.2.1"
#define RMON_HISTORY_LOG_INDEX_OID          RMON_HISTORY_LOG_OID".1"
#define RMON_HISTORY_LOG_SAMPLE_INDEX_OID   RMON_HISTORY_LOG_OID".2"
#define RMON_HISTORY_LOG_INTERVAL_START_OID RMON_HISTORY_LOG_OID".3"
#define RMON_HISTORY_LOG_DROP_EVENTS_OID    RMON_HISTORY_LOG_OID".4"
#define RMON_HISTORY_LOG_OCTETS_OID         RMON_HISTORY_LOG_OID".5"
#define RMON_HISTORY_LOG_PKTS_OID           RMON_HISTORY_LOG_OID".6"
#define RMON_HISTORY_LOG_BROADCAST_PKTS_OID RMON_HISTORY_LOG_OID".7"
#define RMON_HISTORY_LOG_MULTICAST_PKTS_OID RMON_HISTORY_LOG_OID".8"
#define RMON_HISTORY_LOG_CRCALIGN_ERROR_OID RMON_HISTORY_LOG_OID".9"
#define RMON_HISTORY_LOG_UNDERSIZE_PKTS_OID RMON_HISTORY_LOG_OID".10"
#define RMON_HISTORY_LOG_OVERSIZE_PKTS_OID  RMON_HISTORY_LOG_OID".11"
#define RMON_HISTORY_LOG_FRAGMENTS_OID      RMON_HISTORY_LOG_OID".12"
#define RMON_HISTORY_LOG_JABBERS_PKTS_OID   RMON_HISTORY_LOG_OID".13"
#define RMON_HISTORY_LOG_COLLISIONS_OID     RMON_HISTORY_LOG_OID".14"
#define RMON_HISTORY_LOG_ULTILIZATION_OID   RMON_HISTORY_LOG_OID".15"

#define RMON_ETHER_STATE_OID        ".1.3.6.1.2.1.16.1.1.1."
#define RMON_IFINDEX_OID            ".1.3.6.1.2.1.2.2.1.1."

#define RMON_RESPONSE_INT           "INTEGER:"
#define RMON_RESPONSE_TIME          "Timeticks:"
#define RMON_RESPONSE_COUNTER       "Counter32:"

#define RMON_OWNER_NAME_LEN         SYS_BUF32_LEN
#define RMON_ENTRY_INDEX_MAX        65535

#define RMON_ENENT_DESC_MAX_LEN     127
#define RMON_ALARM_MAX_INTERVAL     ((0x1UL << 31) -1)
#define RMON_ALARM_MAX_RISING       ((0x1UL << 31) -1)
#define RMON_ALARM_MAX_FALL         ((0x1UL << 31) -1)
#define RMON_HISTORY_MAX_BUCKET     50
#define RMON_HISTORY_MAX_INTERVAL   3600

typedef enum rmon_status_e
{
    RMON_STATUS_NONE = 0,
    RMON_STATUS_VALID,
    RMON_STATUS_CREATE_REQUEST,
    RMON_STATUS_UNDER_CREATION,
    RMON_STATUS_INVALID,
    RMON_STATUS_END
} rmon_status_t;

typedef enum rmon_event_type_e
{
    RMON_EVENT_UNUSE = 0,
    RMON_EVENT_NONE,
    RMON_EVENT_LOG,
    RMON_EVENT_TRAP,
    RMON_EVENT_LOG_AND_TRAP,
    RMON_EVENT_END
} rmon_event_type_t;

typedef enum rmon_alarm_sample_type_e
{
    RMON_ALARM_SAMPLE_NONE = 0,
    RMON_ALARM_SAMPLE_ABSOLUTE,
    RMON_ALARM_SAMPLE_DELTA,
    RMON_ALARM_SAMPLE_END
} rmon_alarm_sample_type_t;

typedef enum rmon_alarm_type_e
{
    RMON_ALARM_NONE = 0,
    RMON_ALARM_RISING,
    RMON_ALARM_FALLING,
    RMON_ALARM_FALLING_OR_RISING,
    RMON_ALARM_TYPE_END
} rmon_alarm_type_t;

typedef enum rmon_history_counter_e
{
    RMON_HISTORY_NOUSE0 = 0,
    RMON_HISTORY_NOUSE1,
    RMON_HISTORY_SAMPLE_INDEX,
    RMON_HISTORY_INTERVAL_START,
    RMON_HISTORY_DROPEVENTS,
    RMON_HISTORY_OCTETS,
    RMON_HISTORY_PKTS,
    RMON_HISTORY_BROADCASTPKTS,
    RMON_HISTORY_MULTICASTPKTS,
    RMON_HISTORY_CRCALIGNERRORS,
    RMON_HISTORY_UNDERSIZEPKTS,
    RMON_HISTORY_OVERSIZEPKTS,
    RMON_HISTORY_FRAGMENTS,
    RMON_HISTORY_JABBERS,
    RMON_HISTORY_COLLISIONS,
    RMON_HISTORY_UTILIZATION,
    RMON_HISTORY_END
} rmon_history_counter_t;

typedef enum rmon_alarm_sample_e
{
    RMON_SAMPLE_NOUSE0 = 0,
    RMON_SAMPLE_NOUSE1,
    RMON_SAMPLE_NOUSE2,
    RMON_SAMPLE_DROPEVENTS,
    RMON_SAMPLE_OCTETS,
    RMON_SAMPLE_PKTS,
    RMON_SAMPLE_BROADCASTPKTS,
    RMON_SAMPLE_MULTICASTPKTS,
    RMON_SAMPLE_CRCALIGNERRORS,
    RMON_SAMPLE_UNDERSIZEPKTS,
    RMON_SAMPLE_OVERSIZEPKTS,
    RMON_SAMPLE_FRAGMENTS,
    RMON_SAMPLE_JABBERS,
    RMON_SAMPLE_COLLISIONS,
    RMON_SAMPLE_PKTS64OCTETS,
    RMON_SAMPLE_PKTS65TO127OCTETS,
    RMON_SAMPLE_PKTS128TO255OCTETS,
    RMON_SAMPLE_PKTS256TO511OCTETS,
    RMON_SAMPLE_PKTS512TO1023OCTETS,
    RMON_SAMPLE_PKTS1024TO1518OCTETS,
    RMON_SAMPLE_END
} rmon_alarm_sample_t;

typedef struct sal_rmon_event_s
{
    uint32 arrayIndex;
    uint32 eventIndex;
    char eventLastTimeSent[SYS_BUF64_LEN];
    char description[SYS_BUF128_LEN];
    char community[CAPA_SNMP_COMMUNITY_LEN+1];
    char owner[RMON_OWNER_NAME_LEN];
    rmon_event_type_t eventType;
    rmon_status_t status;
} sal_rmon_event_t;

typedef struct sal_rmon_event_log_s
{
    uint32 eventIndex;
    uint32 index;
    char logTime[SYS_BUF64_LEN];
    char description[SYS_BUF256_LEN];
} sal_rmon_event_log_t;

typedef struct sal_rmon_alarm_s
{
    uint32 arrayIndex;
    uint32 alarmIndex;
    uint32 interval;
    uint32 risingThreshold;
    uint32 fallingThreshold;
    uint32 risingEventIndex;
    uint32 fallingEventIndex;
    char owner[RMON_OWNER_NAME_LEN];
    sys_logic_port_t samplePort;
    rmon_alarm_sample_t variable;
    rmon_alarm_sample_type_t sampleType;
    rmon_alarm_type_t alarmType;
    rmon_status_t status;
} sal_rmon_alarm_t;

typedef struct sal_rmon_history_s
{
    uint32 arrayIndex;
    uint32 historyIndex;
    uint32 bucketRequested;
    uint32 bucketGranted;
    uint32 interval;
    char owner[RMON_OWNER_NAME_LEN];
    sys_logic_port_t datasource;
    rmon_status_t status;
} sal_rmon_history_t;

typedef struct sal_rmon_historyLog_s
{
    uint32 sampleIndex;
    uint32 dropEvent;
    uint32 octets;
    uint32 pkts;
    uint32 broadcastPkts;
    uint32 multicastPkts;
    uint32 crcAlignErrors;
    uint32 undersizePkts;
    uint32 oversizePkts;
    uint32 fragments;
    uint32 jabbers;
    uint32 collisions;
    uint32 utilization;
    char intervalStart[SYS_BUF64_LEN];
} sal_rmon_history_log_t;

extern const sys_text_t text_rmon_event_type[];
extern const sys_text_t text_rmon_sample[];
extern const sys_text_t text_rmon_history[];
extern sys_text_t text_rmon_alarm_sample_type[];
extern sys_text_t text_rmon_alarm_type[];

extern char* sal_rmon_cstr_ret(void);

extern int32 sal_rmon_event_set(sal_rmon_event_t *pEvent);

extern int32 sal_rmon_event_get(sal_rmon_event_t *pEvent);

extern int32 sal_rmon_eventByIndex_get(sal_rmon_event_t *pEvent, uint32 eventIndex);

extern int32 sal_rmon_eventRuntime_get(sal_rmon_event_t *pEvent);

extern int32 sal_rmon_eventLog_get(sal_rmon_event_log_t *pEventLog, uint32 eventIndex);

extern int32 sal_rmon_event_apply(sal_rmon_event_t *pEvent);

extern int32 sal_rmon_event_unapply(sal_rmon_event_t *pEvent);

extern int32 sal_rmon_alarm_set(sal_rmon_alarm_t *pAlarm);

extern int32 sal_rmon_alarm_get(sal_rmon_alarm_t *pAlarm);

extern int32 sal_rmon_alarmByIndex_get(sal_rmon_alarm_t *pAlarm, uint32 alarmIndex);

extern int32 sal_rmon_alarm_apply(sal_rmon_alarm_t *pAlarm);

extern int32 sal_rmon_alarm_unapply(sal_rmon_alarm_t *pAlarm);

extern int32 sal_rmon_history_set(sal_rmon_history_t *pHistory);

extern int32 sal_rmon_history_get(sal_rmon_history_t *pHistory);

extern int32 sal_rmon_historyByIndex_get(sal_rmon_history_t *pHistory, uint32 historyIndex);

extern int32 sal_rmon_history_apply(sal_rmon_history_t *pHistory);

extern int32 sal_rmon_history_unapply(sal_rmon_history_t *pHistory);

extern int32 sal_rmon_historyTable_get(sal_rmon_history_log_t *pTable, uint32 historyIndex);

extern int32 sal_rmon_all_apply(void);

#endif

