
#include <include/cgi.h>

#include <libsal/sal_mib.h>
#include <libsal/sal_rmon.h>

static int32 cgi_rmon_stats_get(void);
static int32 cgi_rmon_statsIntf_get(void);
static int32 cgi_rmon_history_get(void);
static int32 cgi_rmon_historyAdd_get(void);
static int32 cgi_rmon_historyEdit_get(void);
static int32 cgi_rmon_historyLog_get(void);
static int32 cgi_rmon_event_get(void);
static int32 cgi_rmon_eventAdd_get(void);
static int32 cgi_rmon_eventEdit_get(void);
static int32 cgi_rmon_eventLog_get(void);
static int32 cgi_rmon_alarm_get(void);
static int32 cgi_rmon_alarmAdd_get(void);
static int32 cgi_rmon_alarmEdit_get(void);

const cgi_cmdmap_t cmdGetRmonTbl[] =
{
    { CGI_CMD_RMON_STATS,           cgi_rmon_stats_get          },
    { CGI_CMD_RMON_STATS_INTF,      cgi_rmon_statsIntf_get      },
    { CGI_CMD_RMON_HISTORY,         cgi_rmon_history_get        },
    { CGI_CMD_RMON_HISTORY_ADD,     cgi_rmon_historyAdd_get     },
    { CGI_CMD_RMON_HISTORY_EDIT,    cgi_rmon_historyEdit_get    },
    { CGI_CMD_RMON_HISTORY_LOG,     cgi_rmon_historyLog_get     },
    { CGI_CMD_RMON_EVENT,           cgi_rmon_event_get          },
    { CGI_CMD_RMON_EVENT_ADD,       cgi_rmon_eventAdd_get       },
    { CGI_CMD_RMON_EVENT_EDIT,      cgi_rmon_eventEdit_get      },
    { CGI_CMD_RMON_EVENT_LOG,       cgi_rmon_eventLog_get       },
    { CGI_CMD_RMON_ALARM,           cgi_rmon_alarm_get          },
    { CGI_CMD_RMON_ALARM_ADD,       cgi_rmon_alarmAdd_get       },
    { CGI_CMD_RMON_ALARM_EDIT,      cgi_rmon_alarmEdit_get      },
    { NULL,                         NULL                        }
};

const sys_text_t text_rmon_sample[] =
{
    {                   "NOUSE"                 },
    {                   "NOUSE"                 },
    {                   "NOUSE"                 },
    {               "DropEvents"            },
    {                   "Octets"                },
    {                     "Pkts"                  },
    {            "BroadcastPkts"         },
    {            "MulticastPkts"         },
    {           "CRCAlignErrors"        },
    {            "UnderSizePkts"         },
    {             "OverSizePkts"          },
    {                "Fragments"             },
    {                  "Jabbers"               },
    {               "Collisions"            },
    {             "Pkts64Octets"          },
    {        "Pkts65to127Octets"     },
    {       "Pkts128to255Octets"    },
    {       "Pkts256to511Octets"    },
    {      "Pkts512to1023Octets"   },
    {     "Pkts1024to1518Octets"  },
};

const mib_cntr_tbl_t rmon_alarmSampleFunc[] =
{
    { "DropEvents",             sal_mib_etherStatsDropEvents_get            },
    { "Octets",                 sal_mib_etherStatsOctets_get                },
    { "Pkts",                   sal_mib_etherStatsPkts_get                  },
    { "BroadcastPkts",          sal_mib_etherStatsBcastPkts_get             },
    { "MulticastPkts",          sal_mib_etherStatsMcastPkts_get             },
    { "CRCAlignErrors",         sal_mib_etherStatsCRCAlignErrs_get          },
    { "UnderSizePkts",          sal_mib_etherStatsUnderSizePkts_get         },
    { "OverSizePkts",           sal_mib_etherStatsOverSizePkts_get          },
    { "Fragments",              sal_mib_etherStatsFragments_get             },
    { "Jabbers",                sal_mib_etherStatsJabbers_get               },
    { "Collisions",             sal_mib_etherStatsCollisions_get            },
    { "Pkts64Octets",           sal_mib_etherStatsPkts64Octets_get          },
    { "Pkts65to127Octets",      sal_mib_etherStatsPkts65to127Octets_get     },
    { "Pkts128to255Octets",     sal_mib_etherStatsPkts128to255Octets_get    },
    { "Pkts256to511Octets",     sal_mib_etherStatsPkts256to511Octets_get    },
    { "Pkts512to1023Octets",    sal_mib_etherStatsPkts512to1023Octets_get   },
    { "Pkts1024to1518Octets",   sal_mib_etherStatsPkts1024to1518Octets_get  },
    { NULL,                     NULL                                        }
};

static int32 cgi_rmon_stats_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;

    uint64 result;
    uint64 result_h;

    sys_logic_port_t lp;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        port = cJSON_CreateObject();

        result = 0;
        sal_mib_etherStatsOctets_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_RECV_BYTES, result);

        result = 0;
        sal_mib_etherStatsDropEvents_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_DROP_EVENTS, result);

        result = 0;
        sal_mib_etherStatsPkts_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_RECV_PACKETS, result);

        result = 0;
        sal_mib_etherStatsBcastPkts_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_RECV_BROADCAST, result);

        result = 0;
        sal_mib_etherStatsMcastPkts_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_RECV_MULTICAST, result);

        result = 0;
        sal_mib_etherStatsCRCAlignErrs_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_ERROR_ALIGN, result);

        result = 0;
        sal_mib_etherStatsUnderSizePkts_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_UNDERSIZE_PACKETS, result);

        result = 0;
        sal_mib_etherStatsOverSizePkts_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_OVERSIZE_PACKETS, result);

        result = 0;
        sal_mib_etherStatsFragments_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_FRAGMENTS, result);

        result = 0;
        sal_mib_etherStatsJabbers_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_JABBERS, result);

        result = 0;
        sal_mib_etherStatsCollisions_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_COLLISIONS, result);

        result = 0;
        sal_mib_etherStatsPkts64Octets_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_FRAMES_64B, result);

        result = 0;
        sal_mib_etherStatsPkts65to127Octets_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_FRAMES_65_127B, result);

        result = 0;
        sal_mib_etherStatsPkts128to255Octets_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_FRAMES_128_255B, result);

        result = 0;
        sal_mib_etherStatsPkts256to511Octets_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_FRAMES_256_511B, result);

        result = 0;
        sal_mib_etherStatsPkts512to1023Octets_get(lp, &result);
        cJSON_AddNumberToObject(port, CGI_RMON_FRAMES_512_1023B, result);

        result = result_h = 0;
        sal_mib_etherStatsOverSizePkts_get(lp, &result);
        sal_mib_etherStatsPkts1024to1518Octets_get(lp, &result_h);
        cJSON_AddNumberToObject(port, CGI_RMON_FRAMES_OVER_1024B, (result + result_h));

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_statsIntf_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    uint64 result;
    uint64 result_h;

    char *pStrValue = NULL;

    sys_logic_port_t lp = 0;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, pStrValue))
        lp = SYS_STR2UINT(pStrValue);

    CGI_GET_JSON_CREATE(root, data);

    result = 0;
    sal_mib_etherStatsOctets_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_RECV_BYTES, result);

    result = 0;
    sal_mib_etherStatsDropEvents_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_DROP_EVENTS, result);

    result = 0;
    sal_mib_etherStatsPkts_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_RECV_PACKETS, result);

    result = 0;
    sal_mib_etherStatsBcastPkts_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_RECV_BROADCAST, result);

    result = 0;
    sal_mib_etherStatsMcastPkts_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_RECV_MULTICAST, result);

    result = 0;
    sal_mib_etherStatsCRCAlignErrs_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_ERROR_ALIGN, result);

    result = 0;
    sal_mib_etherStatsUnderSizePkts_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_UNDERSIZE_PACKETS, result);

    result = 0;
    sal_mib_etherStatsOverSizePkts_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_OVERSIZE_PACKETS, result);

    result = 0;
    sal_mib_etherStatsFragments_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_FRAGMENTS, result);

    result = 0;
    sal_mib_etherStatsJabbers_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_JABBERS, result);

    result = 0;
    sal_mib_etherStatsCollisions_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_COLLISIONS, result);

    result = 0;
    sal_mib_etherStatsPkts64Octets_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_FRAMES_64B, result);

    result = 0;
    sal_mib_etherStatsPkts65to127Octets_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_FRAMES_65_127B, result);

    result = 0;
    sal_mib_etherStatsPkts128to255Octets_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_FRAMES_128_255B, result);

    result = 0;
    sal_mib_etherStatsPkts256to511Octets_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_FRAMES_256_511B, result);

    result = 0;
    sal_mib_etherStatsPkts512to1023Octets_get(lp, &result);
    cJSON_AddNumberToObject(data, CGI_RMON_FRAMES_512_1023B, result);

    result = result_h = 0;
    sal_mib_etherStatsOverSizePkts_get(lp, &result);
    sal_mib_etherStatsPkts1024to1518Octets_get(lp, &result_h);
    cJSON_AddNumberToObject(data, CGI_RMON_FRAMES_OVER_1024B, (result + result_h));

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_history_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *entries = NULL;
    cJSON *entry = NULL;

    int32 i;
    char portstr[CAPA_PORT_STR_LEN];

    sys_enable_t  enable = DISABLED;
    sal_rmon_history_t his;

    CGI_GET_JSON_CREATE(root, data);

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    cJSON_AddBoolToObject(data, CGI_RMON_SNMP_ENABLE, enable);

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        SYS_MEM_CLEAR(his);

        his.arrayIndex = i;

        SYS_ERR_CHK(sal_rmon_history_get(&his));

        if (0 == his.historyIndex)
            continue;

        if (NULL == entries)
            entries = cJSON_CreateArray();

        SYS_MEM_CLEAR(portstr);

        LP2WEBSTR(his.datasource, portstr);

        entry = cJSON_CreateObject();

        cJSON_AddNumberToObject(entry, CGI_RMON_INDEX, his.historyIndex);
        cJSON_AddStringToObject(entry, CGI_RMON_SRC_INTF, portstr);
        cJSON_AddNumberToObject(entry, CGI_RMON_REQUEST_BUCKETS, his.bucketRequested);
        cJSON_AddNumberToObject(entry, CGI_RMON_INTERVAL, his.interval);
        cJSON_AddStringToObject(entry, CGI_RMON_OWNER, his.owner);
        cJSON_AddNumberToObject(entry, CGI_RMON_CURRENT_BUCKETS, his.bucketRequested);

        cJSON_AddItemToArray(entries, entry);
    }

    if (!entries)
        entries = cJSON_CreateNull();

    cJSON_AddItemToObject(data, CGI_RMON_ENTRIES, entries);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_historyAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    int32 i;
    int32 j;
    uint32 idx_tmp;
    uint32 idx_min;
    uint32 his_tbl[CAPA_RMON_HISTORY_NUM];

    sal_rmon_history_t  his;

    SYS_MEM_CLEAR(his_tbl);

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        SYS_MEM_CLEAR(his);

        his.arrayIndex = i;

        SYS_ERR_CHK(sal_rmon_history_get(&his));

        if (0 != his.historyIndex)
            his_tbl[i] = his.historyIndex;
        else
            his_tbl[i] = (RMON_ENTRY_INDEX_MAX + 1);
    }

    for (i = 0, idx_min = 1; i < CAPA_RMON_HISTORY_NUM ; i++)
    {
        for (j = i; j < CAPA_RMON_HISTORY_NUM; j++)
        {
            if (his_tbl[i] && his_tbl[j] < his_tbl[i])
            {
                idx_tmp = his_tbl[j];
                his_tbl[j] = his_tbl[i];
                his_tbl[i] = idx_tmp;
            }
        }

        if (his_tbl[i] == idx_min)
        {
            idx_min = his_tbl[i] + 1;
        }
    }

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_RMON_INDEX, idx_min);
    cJSON_AddNumberToObject(data, CGI_RMON_REQUEST_BUCKETS, RMON_HISTORY_BUCKET_DFL);
    cJSON_AddNumberToObject(data, CGI_RMON_REQUEST_BUCKETS_MIN, 1);
    cJSON_AddNumberToObject(data, CGI_RMON_REQUEST_BUCKETS_MAX, RMON_HISTORY_MAX_BUCKET);
    cJSON_AddNumberToObject(data, CGI_RMON_REQUEST_BUCKETS_DFLT, RMON_HISTORY_BUCKET_DFL);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL, RMON_HISTORY_INTERVAL_DFL);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_MIN, 1);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_MAX, RMON_HISTORY_MAX_INTERVAL);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_DFLT, RMON_HISTORY_INTERVAL_DFL);
    cJSON_AddNumberToObject(data, CGI_RMON_OWNER_MAXLEN, RMON_OWNER_NAME_LEN -1);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_historyEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    int32 found = FALSE;
    int32 i;
    uint32 index;

    char *pStrValue = NULL;
    char portstr[CAPA_PORT_STR_LEN];

    sal_rmon_history_t his;

    if (CGI_IS_PARAM_EXIST(CGI_RMON_ENTRY, pStrValue))
        index = SYS_STR2UINT(pStrValue);
    else
        return SYS_ERR_OK;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_RMON_REQUEST_BUCKETS_MIN, 1);
    cJSON_AddNumberToObject(data, CGI_RMON_REQUEST_BUCKETS_MAX, RMON_HISTORY_MAX_BUCKET);
    cJSON_AddNumberToObject(data, CGI_RMON_REQUEST_BUCKETS_DFLT, RMON_HISTORY_BUCKET_DFL);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_MIN, 1);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_MAX, RMON_HISTORY_MAX_INTERVAL);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_DFLT, RMON_HISTORY_INTERVAL_DFL);
    cJSON_AddNumberToObject(data, CGI_RMON_OWNER_MAXLEN, RMON_OWNER_NAME_LEN - 1);

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        SYS_MEM_CLEAR(his);

        his.arrayIndex = i;

        sal_rmon_history_get(&his);

        if (index == his.historyIndex)
        {
            found = TRUE;
            break;
        }
    }

    if (found)
    {
        SYS_MEM_CLEAR(portstr);

        LP2WEBSTR(his.datasource, portstr);

        cJSON_AddNumberToObject(data, CGI_RMON_INDEX, index);
        cJSON_AddStringToObject(data, CGI_RMON_SRC_INTF, portstr);
        cJSON_AddNumberToObject(data, CGI_RMON_REQUEST_BUCKETS, his.bucketRequested);
        cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL, his.interval);
        cJSON_AddStringToObject(data, CGI_RMON_OWNER, his.owner);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_historyLog_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *entries = NULL;
    cJSON *entry = NULL;

    int32 found = FALSE;
    int32 i;
    uint32 index;

    char *pStrValue = NULL;

    sal_rmon_history_t his;
    sal_rmon_history_log_t his_log[RMON_HISTORY_BUCKET_DFL];

    SYS_MEM_CLEAR(his);
    SYS_MEM_CLEAR(his_log);

    CGI_GET_JSON_CREATE(root, data);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_ENTRY, pStrValue))
        index = SYS_STR2UINT(pStrValue);
    else
        goto cgi_out;

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        SYS_MEM_CLEAR(his);

        his.arrayIndex = i;

        sal_rmon_history_get(&his);

        if (index == his.historyIndex)
        {
            found = TRUE;
            break;
        }
    }

    if (!found)
        goto cgi_out;

    SYS_ERR_CHK(sal_rmon_historyTable_get(his_log, his.historyIndex));

    for (i = RMON_HISTORY_BUCKET_DFL - 1; i >= 0 ; i--)
    {
        if (0 == his_log[i].sampleIndex)
            continue;

        if (NULL == entries)
            entries = cJSON_CreateArray();

        entry = cJSON_CreateObject();

        cJSON_AddNumberToObject(entry, CGI_RMON_INDEX, his.historyIndex);
        cJSON_AddNumberToObject(entry, CGI_RMON_ENTRY_NUM, his_log[i].sampleIndex);
        cJSON_AddNumberToObject(entry, CGI_RMON_RECV_BYTES, his_log[i].octets);
        cJSON_AddNumberToObject(entry, CGI_RMON_DROP_EVENTS, his_log[i].dropEvent);
        cJSON_AddNumberToObject(entry, CGI_RMON_RECV_PACKETS, his_log[i].pkts);
        cJSON_AddNumberToObject(entry, CGI_RMON_RECV_BROADCAST, his_log[i].broadcastPkts);
        cJSON_AddNumberToObject(entry, CGI_RMON_RECV_MULTICAST, his_log[i].multicastPkts);
        cJSON_AddNumberToObject(entry, CGI_RMON_ERROR_ALIGN, his_log[i].crcAlignErrors);
        cJSON_AddNumberToObject(entry, CGI_RMON_UNDERSIZE_PACKETS, his_log[i].undersizePkts);
        cJSON_AddNumberToObject(entry, CGI_RMON_OVERSIZE_PACKETS, his_log[i].oversizePkts);
        cJSON_AddNumberToObject(entry, CGI_RMON_FRAGMENTS, his_log[i].fragments);
        cJSON_AddNumberToObject(entry, CGI_RMON_JABBERS, his_log[i].jabbers);
        cJSON_AddNumberToObject(entry, CGI_RMON_COLLISIONS, his_log[i].collisions);
        cJSON_AddNumberToObject(entry, CGI_RMON_UTILIZATION, his_log[i].utilization);

        cJSON_AddItemToArray(entries, entry);
    }

cgi_out:
    if (!entries)
        entries = cJSON_CreateNull();

    cJSON_AddItemToObject(data, CGI_RMON_ENTRIES, entries);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_event_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *entries = NULL;
    cJSON *entry = NULL;

    int32 i;
    char *str;;

    sys_enable_t enable;
    sal_rmon_event_t event;

    CGI_GET_JSON_CREATE(root, data);

    sal_snmp_enable_get(&enable);

    cJSON_AddBoolToObject(data, CGI_RMON_SNMP_ENABLE, enable);

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        SYS_MEM_CLEAR(event);

        event.arrayIndex = i;

        sal_rmon_event_get(&event);

        if (0 == event.eventIndex)
            continue;

        SYS_ERR_CHK(sal_rmon_eventRuntime_get(&event));

        if (NULL == entries)
            entries = cJSON_CreateArray();

        switch (event.eventType)
        {
            case RMON_EVENT_LOG:
                str = "lblNotifiTypeLog";
                break;

            case RMON_EVENT_TRAP:
                str = "lblNotifiTypeTrap";
                break;

            case RMON_EVENT_LOG_AND_TRAP:
                str = "lblNotifiTypeLogTrap";
                break;

            default:
                str = "lblNotifiTypeNone";
                break;
        }

        entry = cJSON_CreateObject();

        cJSON_AddNumberToObject(entry, CGI_RMON_INDEX, event.eventIndex);
        cJSON_AddStringToObject(entry, CGI_RMON_COMMUNITY, event.community);
        cJSON_AddStringToObject(entry, CGI_RMON_DESC, event.description);
        cJSON_AddStringToObject(entry, CGI_RMON_TIME, event.eventLastTimeSent);
        cJSON_AddStringToObject(entry, CGI_RMON_OWNER, event.owner);
        cJSON_AddLangStringToObject(entry, CGI_RMON_NOTIFICATION_TYPE, "rmon", str);

        cJSON_AddItemToArray(entries, entry);
    }

    if (!entries)
        entries = cJSON_CreateNull();

    cJSON_AddItemToObject(data, CGI_RMON_ENTRIES, entries);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_eventAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    int32 i;
    int32 j;
    uint32 idx_tmp;
    uint32 idx_min;
    uint32 event_tbl[CAPA_RMON_HISTORY_NUM];

    sal_rmon_event_t event;

    SYS_MEM_CLEAR(event_tbl);

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        SYS_MEM_CLEAR(event);

        event.arrayIndex = i;

        SYS_ERR_CHK(sal_rmon_event_get(&event));

        if (0 != event.eventIndex)
            event_tbl[i] = event.eventIndex;
        else
            event_tbl[i] = (RMON_ENTRY_INDEX_MAX + 1);
    }

    for (i = 0, idx_min = 1; i < CAPA_RMON_EVENT_NUM ; i++)
    {
        
        for (j = i; j < CAPA_RMON_EVENT_NUM; j++)
        {

            if (event_tbl[i] && event_tbl[j] < event_tbl[i])
            {
                idx_tmp = event_tbl[j];
                event_tbl[j] = event_tbl[i];
                event_tbl[i] = idx_tmp;
            }
        }

        if (event_tbl[i] == idx_min)
        {
            idx_min = event_tbl[i] + 1;
        }
    }

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_RMON_INDEX, idx_min);
    cJSON_AddNumberToObject(data, CGI_RMON_COMMUNITY_MAXLEN, CAPA_SNMP_COMMUNITY_LEN);
    cJSON_AddNumberToObject(data, CGI_RMON_DESC_MAXLEN, RMON_ENENT_DESC_MAX_LEN);
    cJSON_AddNumberToObject(data, CGI_RMON_OWNER_MAXLEN, RMON_OWNER_NAME_LEN - 1);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_eventEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    int32 found = FALSE;
    int32 i;
    uint32 index;

    char *pStrValue = NULL;

    sal_rmon_event_t event;

    if (CGI_IS_PARAM_EXIST(CGI_RMON_ENTRY, pStrValue))
        index = SYS_STR2UINT(pStrValue);
    else
        return SYS_ERR_OK;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_RMON_COMMUNITY_MAXLEN, CAPA_SNMP_COMMUNITY_LEN);
    cJSON_AddNumberToObject(data, CGI_RMON_DESC_MAXLEN, RMON_ENENT_DESC_MAX_LEN);
    cJSON_AddNumberToObject(data, CGI_RMON_OWNER_MAXLEN, RMON_OWNER_NAME_LEN - 1);

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        SYS_MEM_CLEAR(event);

        event.arrayIndex = i;

        sal_rmon_event_get(&event);

        if (index == event.eventIndex)
        {
            found = TRUE;
            break;
        }
    }

    if (found)
    {
        switch (event.eventType)
        {
            case RMON_EVENT_LOG:
                i = 1;
                break;

            case RMON_EVENT_TRAP:
                i = 2;
                break;

            case RMON_EVENT_LOG_AND_TRAP:
                i = 3;
                break;

            default:
                i = 0;
                break;
        }

        cJSON_AddNumberToObject(data, CGI_RMON_INDEX, event.eventIndex);
        cJSON_AddStringToObject(data, CGI_RMON_COMMUNITY, event.community);
        cJSON_AddStringToObject(data, CGI_RMON_DESC, event.description);
        cJSON_AddStringToObject(data, CGI_RMON_OWNER, event.owner);
        cJSON_AddNumberToObject(data, CGI_RMON_NOTIFICATION_TYPE, i);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_eventLog_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *entries = NULL;
    cJSON *entry = NULL;

    int32 found = FALSE;
    int32 i;
    uint32 index;

    char *pStrValue = NULL;

    sal_rmon_event_t event;
    sal_rmon_event_log_t event_log[RMON_EVENT_LOG_DFL];

    SYS_MEM_CLEAR(event);
    SYS_MEM_CLEAR(event_log);

    CGI_GET_JSON_CREATE(root, data);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_ENTRY, pStrValue))
        index = SYS_STR2UINT(pStrValue);
    else
        goto cgi_out;

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        SYS_MEM_CLEAR(event);

        event.arrayIndex = i;

        sal_rmon_event_get(&event);

        if (index == event.eventIndex)
        {
            found = TRUE;
            break;
        }
    }

    if (!found)
        goto cgi_out;

    SYS_ERR_CHK(sal_rmon_eventLog_get(event_log, event.eventIndex));

    for (i = RMON_HISTORY_BUCKET_DFL - 1; i >= 0 ; i--)
    {
        if (0 == event_log[i].index)
            continue;

        if (NULL == entries)
            entries = cJSON_CreateArray();

        entry = cJSON_CreateObject();

        cJSON_AddNumberToObject(entry, CGI_RMON_LOG_NUM, event_log[i].index);
        cJSON_AddStringToObject(entry, CGI_RMON_TIME, event_log[i].logTime);
        cJSON_AddStringToObject(entry, CGI_RMON_DESC, event_log[i].description);

        cJSON_AddItemToArray(entries, entry);
    }

cgi_out:
    if (!entries)
        entries = cJSON_CreateNull();

    cJSON_AddItemToObject(data, CGI_RMON_ENTRIES, entries);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_alarm_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *entries = NULL;
    cJSON *entry = NULL;

    int32 found = FALSE;
    int32 i;
    int32 j;
    uint64 result;
    char *str;
    char portstr[CAPA_PORT_STR_LEN];

    sys_enable_t enable;
    sal_rmon_event_t event;
    sal_rmon_alarm_t alarm;

    CGI_GET_JSON_CREATE(root, data);

    sal_snmp_enable_get(&enable);

    cJSON_AddBoolToObject(data, CGI_RMON_SNMP_ENABLE, enable);

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        SYS_MEM_CLEAR(alarm);
        result = 0;

        alarm.arrayIndex = i;

        sal_rmon_alarm_get(&alarm);

        if (0 == alarm.alarmIndex)
            continue;

        if (NULL == entries)
            entries = cJSON_CreateArray();

        LP2WEBSTR(alarm.samplePort, portstr);

        for (j = 0; rmon_alarmSampleFunc[j].name; j++)
        {
            if (!osal_strcmp(rmon_alarmSampleFunc[j].name, text_rmon_sample[alarm.variable].text))
            {
                rmon_alarmSampleFunc[j].sal_mib_func(alarm.samplePort, &result);
                break;
            }
        }

        entry = cJSON_CreateObject();

        cJSON_AddNumberToObject(entry, CGI_RMON_INDEX, alarm.alarmIndex);
        cJSON_AddStringToObject(entry, CGI_RMON_INTF, portstr);
        cJSON_AddStringToObject(entry, CGI_RMON_CNT_NAME, text_rmon_sample[alarm.variable].text);
        cJSON_AddNumberToObject(entry, CGI_RMON_CNT_VALUE, result);

        switch (alarm.sampleType)
        {
            case RMON_ALARM_SAMPLE_ABSOLUTE:
                str = "lblAbsolute";
                break;

            case RMON_ALARM_SAMPLE_DELTA:
                str = "lblDelta";
                break;

            default:
                str = "lblNone";
                break;
        }
        cJSON_AddLangStringToObject(entry, CGI_RMON_SAMPLE_TYPE, "rmon", str);

        SYS_MEM_CLEAR(event);
        sal_rmon_eventByIndex_get(&event, alarm.risingEventIndex);

        cJSON_AddNumberToObject(entry, CGI_RMON_RISING_THRESHOLD, alarm.risingThreshold);
        cJSON_AddStringToObject(entry, CGI_RMON_RISING_EVENT, event.description);

        SYS_MEM_CLEAR(event);
        sal_rmon_eventByIndex_get(&event, alarm.fallingEventIndex);

        cJSON_AddNumberToObject(entry, CGI_RMON_FALLING_THRESHOLD, alarm.fallingThreshold);
        cJSON_AddStringToObject(entry, CGI_RMON_FALLING_EVENT, event.description);

        switch (alarm.alarmType)
        {
            case RMON_ALARM_RISING:
                str = "lblRising";
                break;

            case RMON_ALARM_FALLING:
                str = "lblFalling";
                break;

            case RMON_ALARM_FALLING_OR_RISING:
                str = "lblRisingAndFalling";
                break;

            default:
                str = "lblNone";
                break;
        }
        cJSON_AddLangStringToObject(entry, CGI_RMON_STARTUP_ALARM, "rmon", str);

        cJSON_AddNumberToObject(entry, CGI_RMON_INTERVAL, alarm.interval);
        cJSON_AddStringToObject(entry, CGI_RMON_OWNER, alarm.owner);

        cJSON_AddItemToArray(entries, entry);
    }

    if (!entries)
        entries = cJSON_CreateNull();

    cJSON_AddItemToObject(data, CGI_RMON_ENTRIES, entries);

    for (i = 0, found = FALSE; i < CAPA_RMON_EVENT_NUM; i++)
    {
        SYS_MEM_CLEAR(event);

        event.arrayIndex = i;

        sal_rmon_event_get(&event);

        if (0 != event.eventIndex)
        {
            found = TRUE;
            break;
        }
    }

    cJSON_AddBoolToObject(data, CGI_RMON_HAS_EVENT, found);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_alarmAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *entries = NULL;
    cJSON *entry = NULL;

    int32 i;
    int32 j;
    uint32 idx_tmp;
    uint32 idx_min;
    uint32 alarm_tbl[CAPA_RMON_HISTORY_NUM];
    char str[SYS_BUF256_LEN];

    sal_rmon_alarm_t alarm;
    sal_rmon_event_t event;

    SYS_MEM_CLEAR(alarm_tbl);

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        SYS_MEM_CLEAR(alarm);

        alarm.arrayIndex = i;

        SYS_ERR_CHK(sal_rmon_alarm_get(&alarm));

        if (0 != alarm.alarmIndex)
            alarm_tbl[i] = alarm.alarmIndex;
        else
            alarm_tbl[i] = (RMON_ENTRY_INDEX_MAX + 1);
    }

    for (i = 0, idx_min = 1; i < CAPA_RMON_ALARM_NUM ; i++)
    {
        
        for (j = i; j < CAPA_RMON_ALARM_NUM; j++)
        {

            if (alarm_tbl[i] && alarm_tbl[j] < alarm_tbl[i])
            {
                idx_tmp = alarm_tbl[j];
                alarm_tbl[j] = alarm_tbl[i];
                alarm_tbl[i] = idx_tmp;
            }
        }

        if (alarm_tbl[i] == idx_min)
        {
            idx_min = alarm_tbl[i] + 1;
        }
    }

    CGI_GET_JSON_CREATE(root, data);

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        SYS_MEM_CLEAR(event);

        event.arrayIndex = i;

        sal_rmon_event_get(&event);

        if (event.eventIndex)
        {
            if (NULL == entries)
                entries = cJSON_CreateArray();

            entry = cJSON_CreateObject();

            snprintf(str, sizeof(str), "%u - %s", event.eventIndex, event.description);

            cJSON_AddNumberToObject(entry, CGI_VALUE, event.eventIndex);
            cJSON_AddStringToObject(entry, CGI_NAME, str);

            cJSON_AddItemToArray(entries, entry);
        }
    }

    if (!entries)
        entries = cJSON_CreateNull();

    cJSON_AddItemToObject(data, CGI_RMON_EVENTS, entries);

    cJSON_AddNumberToObject(data, CGI_RMON_INDEX, idx_min);
    cJSON_AddNumberToObject(data, CGI_RMON_RISING_THRESHOLD, 100);
    cJSON_AddNumberToObject(data, CGI_RMON_RISING_THRESHOLD_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_RMON_RISING_THRESHOLD_MAX, RMON_ALARM_MAX_RISING);
    cJSON_AddNumberToObject(data, CGI_RMON_RISING_THRESHOLD_DFLT, 100);
    cJSON_AddNumberToObject(data, CGI_RMON_FALLING_THRESHOLD, 20);
    cJSON_AddNumberToObject(data, CGI_RMON_FALLING_THRESHOLD_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_RMON_FALLING_THRESHOLD_MAX, RMON_ALARM_MAX_FALL);
    cJSON_AddNumberToObject(data, CGI_RMON_FALLING_THRESHOLD_DFLT, 20);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL, 100);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_MIN, 1);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_MAX, RMON_ALARM_MAX_INTERVAL);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_DFLT, 100);
    cJSON_AddNumberToObject(data, CGI_RMON_OWNER_MAXLEN, RMON_OWNER_NAME_LEN - 1);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_alarmEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *entries = NULL;
    cJSON *entry = NULL;

    int32 found = FALSE;
    int32 i;
    uint32 index;

    char *pStrValue = NULL;
    char str[SYS_BUF256_LEN];
    char portstr[CAPA_PORT_STR_LEN];

    sal_rmon_alarm_t alarm;
    sal_rmon_event_t event;

    if (CGI_IS_PARAM_EXIST(CGI_RMON_ENTRY, pStrValue))
        index = SYS_STR2UINT(pStrValue);
    else
        return SYS_ERR_OK;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_RMON_RISING_THRESHOLD_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_RMON_RISING_THRESHOLD_MAX, RMON_ALARM_MAX_RISING);
    cJSON_AddNumberToObject(data, CGI_RMON_RISING_THRESHOLD_DFLT, 100);
    cJSON_AddNumberToObject(data, CGI_RMON_FALLING_THRESHOLD_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_RMON_FALLING_THRESHOLD_MAX, RMON_ALARM_MAX_FALL);
    cJSON_AddNumberToObject(data, CGI_RMON_FALLING_THRESHOLD_DFLT, 20);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_MIN, 1);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_MAX, RMON_ALARM_MAX_INTERVAL);
    cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL_DFLT, 100);
    cJSON_AddNumberToObject(data, CGI_RMON_OWNER_MAXLEN, RMON_OWNER_NAME_LEN - 1);

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        SYS_MEM_CLEAR(event);

        event.arrayIndex = i;

        sal_rmon_event_get(&event);

        if (event.eventIndex)
        {
            if (NULL == entries)
                entries = cJSON_CreateArray();

            entry = cJSON_CreateObject();

            snprintf(str, sizeof(str), "%u - %s", event.eventIndex, event.description);

            cJSON_AddNumberToObject(entry, CGI_VALUE, event.eventIndex);
            cJSON_AddStringToObject(entry, CGI_NAME, str);

            cJSON_AddItemToArray(entries, entry);
        }
    }

    if (!entries)
        entries = cJSON_CreateNull();

    cJSON_AddItemToObject(data, CGI_RMON_EVENTS, entries);

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        SYS_MEM_CLEAR(alarm);

        alarm.arrayIndex = i;

        sal_rmon_alarm_get(&alarm);

        if (index == alarm.alarmIndex)
        {
            found = TRUE;
            break;
        }
    }

    if (found)
    {
        SYS_MEM_CLEAR(portstr);

        LP2WEBSTR(alarm.samplePort, portstr);

        cJSON_AddStringToObject(data, CGI_RMON_INTF, portstr);

        switch(alarm.variable)
        {
            case RMON_SAMPLE_DROPEVENTS:
                i = 0;
                break;

            case RMON_SAMPLE_OCTETS:
                i = 1;
                break;

            case RMON_SAMPLE_PKTS:
                i = 2;
                break;

            case RMON_SAMPLE_BROADCASTPKTS:
                i = 3;
                break;

            case RMON_SAMPLE_MULTICASTPKTS:
                i = 4;
                break;

            case RMON_SAMPLE_CRCALIGNERRORS:
                i = 5;
                break;

            case RMON_SAMPLE_UNDERSIZEPKTS:
                i = 6;
                break;

            case RMON_SAMPLE_OVERSIZEPKTS:
                i = 7;
                break;

            case RMON_SAMPLE_FRAGMENTS:
                i = 8;
                break;

            case RMON_SAMPLE_JABBERS:
                i = 9;
                break;

            case RMON_SAMPLE_COLLISIONS:
                i = 10;
                break;

            case RMON_SAMPLE_PKTS64OCTETS:
                i = 11;
                break;

            case RMON_SAMPLE_PKTS65TO127OCTETS:
                i = 12;
                break;

            case RMON_SAMPLE_PKTS128TO255OCTETS:
                i = 13;
                break;

            case RMON_SAMPLE_PKTS256TO511OCTETS:
                i = 14;
                break;

            case RMON_SAMPLE_PKTS512TO1023OCTETS:
                i = 15;
                break;

            case RMON_SAMPLE_PKTS1024TO1518OCTETS:
                i = 16;

            default:
                i = 0;
                break;
        }

        cJSON_AddNumberToObject(data, CGI_RMON_CNT_NAME, i);
        cJSON_AddNumberToObject(data, CGI_RMON_RISING_THRESHOLD, alarm.risingThreshold);
        cJSON_AddNumberToObject(data, CGI_RMON_SAMPLE_TYPE,
                                RMON_ALARM_SAMPLE_ABSOLUTE == alarm.sampleType ? 0 : 1);
        cJSON_AddNumberToObject(data, CGI_RMON_RISING_THRESHOLD, alarm.risingThreshold);
        cJSON_AddNumberToObject(data, CGI_RMON_RISING_EVENT, alarm.risingEventIndex);
        cJSON_AddNumberToObject(data, CGI_RMON_FALLING_THRESHOLD, alarm.fallingThreshold);
        cJSON_AddNumberToObject(data, CGI_RMON_FALLING_EVENT, alarm.fallingEventIndex);

        switch (alarm.alarmType)
        {
            case RMON_ALARM_RISING:
                i = 0;
                break;

            case RMON_ALARM_FALLING:
                i = 1;
                break;

            case RMON_ALARM_FALLING_OR_RISING:
                i = 2;
                break;

            default:
                i = 0;
                break;
        }
        cJSON_AddNumberToObject(data, CGI_RMON_STARTUP_ALARM, i);

        cJSON_AddNumberToObject(data, CGI_RMON_INTERVAL, alarm.interval);
        cJSON_AddStringToObject(data, CGI_RMON_OWNER, alarm.owner);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
