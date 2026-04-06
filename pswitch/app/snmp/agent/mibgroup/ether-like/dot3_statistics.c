
#include <net-snmp/net-snmp-config.h>
#include <sys/time.h>
#include <unistd.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "util_funcs.h"
#include "dot3_statistics.h"
        /*
         * Implementation headers
         */
#include "agutil_api.h"
//#include "row_api.h"

/*
 * RTK private header
 */
#include <common/sys_error.h>
#include <common/sys_def.h>
#include <common/sys_portmask.h>
#include <common/sys_util.h>

#include <libsal/sal_port.h>
#include <libsal/sal_trunk.h>
#include <libsal/sal_util.h>
#include <libsal/sal_mib.h>
#include <libsal/sal_port.h>

        /*
         * File scope definitions section
         */
        /*
         * from MIB compilation
         */
#define MIB_DESCR                               "Dot3Stat"
#define IDdot3StatsIndex                        1
#define IDdot3StatsAlignmentErrors              2
#define IDdot3StatsFCSErrors                    3
#define IDdot3StatsSingleCollisionFrames        4
#define IDdot3StatsMultipleCollisionFrames      5
#define IDdot3StatsSQETestErrors                6
#define IDdot3StatsDeferredTransmissions        7
#define IDdot3StatsLateCollisions               8
#define IDdot3StatsExcessiveCollisions          9
#define IDdot3StatsInternalMacTransmitErrors    10
#define IDdot3StatsCarrierSenseErrors           11
#define IDdot3StatsFrameTooLongs                13
#define IDdot3StatsInternalMacReceiveErrors     16
#define IDdot3StatsEtherChipSet                 17
#define dot3StatsIndex_MIN                      1
#define dot3StatsIndex_MAX                      17
#define dot3StatsOIDLen_MAX                     12

#if 0
     typedef struct {
         VAR_OID_T
             data_source;
         u_long
             etherStatsCreateTime;
         ETH_STATS_T
             eth;
     } CRTL_ENTRY_T;

/*
 * Main section
 */

     static TABLE_DEFINTION_T
         StatCtrlTable;
     static TABLE_DEFINTION_T *
         table_ptr = &
         StatCtrlTable;

/*
 * Control Table RowApi Callbacks
 */

     int
     stat_Create(RMON_ENTRY_T * eptr)
{                               /* create the body: alloc it and set defaults */
    CRTL_ENTRY_T   *body;
    static VAR_OID_T data_src_if_index_1 =
        { 11, {1, 3, 6, 1, 2, 1, 2, 2, 1, 1, 1} };

    eptr->body = AGMALLOC(sizeof(CRTL_ENTRY_T));
    if (!eptr->body)
        return -3;
    body = (CRTL_ENTRY_T *) eptr->body;

    /*
     * set defaults
     */
    memcpy(&body->data_source, &data_src_if_index_1, sizeof(VAR_OID_T));
    body->data_source.objid[body->data_source.length - 1] =
        eptr->ctrl_index;
    eptr->owner = AGSTRDUP("Startup Mgmt");
    memset(&body->eth, 0, sizeof(ETH_STATS_T));

    return 0;
}

int
stat_Validate(RMON_ENTRY_T * eptr)
{
    /*
     * T.B.D. (system dependent) check valid inteface in body->data_source;
     */

    return 0;
}

int
stat_Activate(RMON_ENTRY_T * eptr)
{
    CRTL_ENTRY_T   *body = (CRTL_ENTRY_T *) eptr->body;

    body->etherStatsCreateTime = AGUTIL_sys_up_time();

    return 0;
}

int
stat_Copy(RMON_ENTRY_T * eptr)
{
    CRTL_ENTRY_T   *body = (CRTL_ENTRY_T *) eptr->body;
    CRTL_ENTRY_T   *clone = (CRTL_ENTRY_T *) eptr->tmp;

    if (snmp_oid_compare
        (clone->data_source.objid, clone->data_source.length,
         body->data_source.objid, body->data_source.length)) {
        memcpy(&body->data_source, &clone->data_source, sizeof(VAR_OID_T));
    }

    return 0;
}

int
stat_Deactivate(RMON_ENTRY_T * eptr)
{
    CRTL_ENTRY_T   *body = (CRTL_ENTRY_T *) eptr->body;
    memset(&body->eth, 0, sizeof(ETH_STATS_T));
    return 0;
}

/***************************************************
 * Function:var_etherStats2Entry
 * Purpose: Handles the request for etherStats2Entry variable instances
 ***************************************************/
u_char         *
var_etherStats2Entry(struct variable * vp, oid * name, size_t * length,
                     int exact, size_t * var_len,
                     WriteMethod ** write_method)
{
    static long     long_return;
    static CRTL_ENTRY_T theEntry;
    RMON_ENTRY_T   *hdr;

    DEBUGMSGTL(("[RTK]", "RMON - var_etherStats2Entry() \n"));
    *write_method = NULL;

    hdr = ROWAPI_header_ControlEntry(vp, name, length, exact, var_len,
                                     table_ptr,
                                     &theEntry, sizeof(CRTL_ENTRY_T));
    if (!hdr)
        return NULL;

    *var_len = sizeof(long);    /* default */

    switch (vp->magic) {
    case IDetherStatsDroppedFrames:
        long_return = 0;
        return (u_char *) & long_return;
    case IDetherStatsCreateTime:
        long_return = theEntry.etherStatsCreateTime;
        return (u_char *) & long_return;
    default:
        ag_trace("%s: unknown vp->magic=%d", table_ptr->name,
                 (int) vp->magic);
        ERROR_MSG("");
    };                          /* of switch by 'vp->magic'  */

    return NULL;
}

/***************************************************
 * Function:write_etherStatsEntry
 ***************************************************/
static int
write_etherStatsEntry(int action, u_char * var_val, u_char var_val_type,
                      size_t var_val_len, u_char * statP,
                      oid * name, size_t name_len)
{
    long            long_temp;
    int             leaf_id, snmp_status;
    static int      prev_action = COMMIT;
    RMON_ENTRY_T   *hdr;
    CRTL_ENTRY_T   *cloned_body;
    CRTL_ENTRY_T   *body;

    DEBUGMSGTL(("[RTK]", "RMON - write_etherStatsEntry() action = %d\n", action));
    switch (action) {
    case RESERVE1:
    case FREE:
    case UNDO:
    case ACTION:
    case COMMIT:
    default:
        snmp_status =
            ROWAPI_do_another_action(name, etherStatsEntryFirstIndexBegin,
                                     action, &prev_action, table_ptr,
                                     sizeof(CRTL_ENTRY_T));
        if (SNMP_ERR_NOERROR != snmp_status) {
            ag_trace("failed action %d with %d", action, snmp_status);
        }
        break;

    case RESERVE2:
        /*
         * get values from PDU, check them and save them in the cloned entry
         */
        long_temp = name[etherStatsEntryFirstIndexBegin];
        leaf_id = (int) name[etherStatsEntryFirstIndexBegin - 1];
        ag_trace("[RTK] long_temp = %d", long_temp);
        ag_trace("[RTK] leaf_id = %d", leaf_id);
        hdr = ROWAPI_find(table_ptr, long_temp);        /* it MUST be OK */
        cloned_body = (CRTL_ENTRY_T *) hdr->tmp;
        body = (CRTL_ENTRY_T *) hdr->body;
        switch (leaf_id) {
        case Leaf_etherStatsDataSource:
            snmp_status = AGUTIL_get_oid_value(var_val, var_val_type,
                                               var_val_len,
                                               &cloned_body->data_source);
            if (SNMP_ERR_NOERROR != snmp_status) {
                return snmp_status;
            }
            if (RMON1_ENTRY_UNDER_CREATION != hdr->status &&
                snmp_oid_compare(cloned_body->data_source.objid,
                                 cloned_body->data_source.length,
                                 body->data_source.objid,
                                 body->data_source.length))
                return SNMP_ERR_BADVALUE;
            break;

            break;
        case Leaf_etherStatsOwner:
            if (hdr->new_owner)
                AGFREE(hdr->new_owner);
            hdr->new_owner = AGMALLOC(MAX_OWNERSTRING);;
            if (!hdr->new_owner)
                return SNMP_ERR_TOOBIG;
            snmp_status = AGUTIL_get_string_value(var_val, var_val_type,
                                                  var_val_len,
                                                  MAX_OWNERSTRING,
                                                  1, NULL, hdr->new_owner);
            if (SNMP_ERR_NOERROR != snmp_status) {
                return snmp_status;
            }
            break;
        case Leaf_etherStatsStatus:
            snmp_status = AGUTIL_get_int_value(var_val, var_val_type,
                                               var_val_len,
                                               RMON1_ENTRY_VALID,
                                               RMON1_ENTRY_INVALID,
                                               &long_temp);
            if (SNMP_ERR_NOERROR != snmp_status) {
                ag_trace("cannot browse etherStatsStatus");
                return snmp_status;
            }
            hdr->new_status = long_temp;
            break;
            break;
        default:
            ag_trace("%s:unknown leaf_id=%d\n", table_ptr->name,
                     (int) leaf_id);
            return SNMP_ERR_NOSUCHNAME;
        }                       /* of switch by 'leaf_id' */
        break;
    }                           /* of switch by 'action' */

    prev_action = action;
    return SNMP_ERR_NOERROR;
}
#endif

/***************************************************
 * Function:var_dot3StatsEntry
 * Purpose: Handles the request for dot3StatsEntry variable instances
 ***************************************************/

u_char         *
var_dot3StatsEntry(struct variable * vp, oid * name, size_t * length,
                    int exact, size_t * var_len,
                    WriteMethod ** write_method)
{
    static unsigned long     long_return;
    unsigned long   index;
    uint64 cntr64 = 0;

    DEBUGMSGTL(("[RTK]", "Dot3 - var_dot3StatsEntry(). exact = %d\n", exact));

    *write_method = NULL;

    if (AGUTIL_advance_index_name(vp, name, length, exact) != 0)
        return NULL;
    index = vp->namelen >= *length ? 0 : name[vp->namelen];
    if (exact) /* GET packet for one instance */
    {
        if ((index < dot3StatsIndex_MIN) || (index > dot3StatsIndex_MAX))
            return NULL;
        if (*length > dot3StatsOIDLen_MAX)
            return NULL;
    }
    else /* Get-Next packet */
    {
        index++;
        name[vp->namelen] = index;
        *length = vp->namelen + 1;
    }

    /* RTK: index is 1-based, LOGIC_PORT_END is 0-based */
    if (index > (LOGIC_PORT_END + 1))
        return NULL;

    *var_len = sizeof(long);
    DEBUGMSGTL(("[RTK]", "index = %d\n", index));

    switch (vp->magic) {
    case IDdot3StatsIndex:
        long_return = index;
        return (u_char *) & long_return;
    case IDdot3StatsAlignmentErrors:
        sal_mib_dot3_statsAlignmentErrors_get(index - 1, &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDdot3StatsFCSErrors:
        sal_mib_dot3StatsFCSErrors_get(index - 1, &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDdot3StatsSingleCollisionFrames:
        sal_mib_dot3StatsSingleCollisionFrames_get(index - 1, &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDdot3StatsMultipleCollisionFrames:
        sal_mib_dot3StatsMultipleCollisionFrames_get(index - 1, &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDdot3StatsSQETestErrors:
        long_return = 0;
        return (u_char *) & long_return;
    case IDdot3StatsDeferredTransmissions:
        sal_mib_dot3StatsDeferredTransmissions_get(index - 1, &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDdot3StatsLateCollisions:
        sal_mib_dot3StatsLateCollisions_get(index - 1, &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDdot3StatsExcessiveCollisions:
        sal_mib_dot3StatsExcessiveCollisions_get(index - 1, &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDdot3StatsInternalMacTransmitErrors:
        long_return = 0;
        return (u_char *) & long_return;
    case IDdot3StatsCarrierSenseErrors:
        long_return = 0;
        return (u_char *) & long_return;
    case IDdot3StatsFrameTooLongs:
        sal_mib_dot3StatsFrameTooLongs_get(index - 1, &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDdot3StatsInternalMacReceiveErrors:
        long_return = 0;
        return (u_char *) & long_return;
    case IDdot3StatsEtherChipSet:
/*
        *var_len = sizeof(dot3_chip_set_oid);
        return (unsigned char *) dot3_chip_set_oid;
*/
    default:
        ERROR_MSG("");
    };

    return NULL;
}

#if 0
int
add_dot3statistics_entry(int ctrl_index, int ifIndex)
{
    int             ierr;

    ierr = ROWAPI_new(table_ptr, ctrl_index);
    switch (ierr) {
    case -1:
        ag_trace("max. number exedes\n");
        break;
    case -2:
        ag_trace("malloc failed");
        break;
    case -3:
        ag_trace("ClbkCreate failed");
        break;
    case 0:
        break;
    default:
        ag_trace("Unknown code %d", ierr);
        break;
    }

    if (!ierr) {
        register RMON_ENTRY_T *eptr = ROWAPI_find(table_ptr, ctrl_index);
        if (!eptr) {
            ag_trace("cannot find it");
            ierr = -4;
        } else {
            CRTL_ENTRY_T   *body = (CRTL_ENTRY_T *) eptr->body;

            body->data_source.objid[body->data_source.length - 1] =
                ifIndex;

            eptr->new_status = RMON1_ENTRY_VALID;
            ierr = ROWAPI_commit(table_ptr, ctrl_index);
            if (ierr) {
                ag_trace("ROWAPI_commit returned %d", ierr);
            }
        }
    }

    return ierr;
}
#endif

/***************************************************
 * define Variables callbacks
 ***************************************************/
oid             oidDot3StatisticsVariablesOid[] = { 1, 3, 6, 1, 2, 1, 10, 7 };

struct variable7 oidDot3StatisticsVariables[] = {
    {IDdot3StatsIndex, ASN_INTEGER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 1}},
    {IDdot3StatsAlignmentErrors, ASN_OBJECT_ID, RWRITE, var_dot3StatsEntry, 3,
     {2, 1, 2}},
    {IDdot3StatsFCSErrors, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 3}},
    {IDdot3StatsSingleCollisionFrames, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 4}},
    {IDdot3StatsMultipleCollisionFrames, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 5}},
    {IDdot3StatsSQETestErrors, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 6}},
    {IDdot3StatsDeferredTransmissions, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 7}},
    {IDdot3StatsLateCollisions, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 8}},
    {IDdot3StatsExcessiveCollisions, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 9}},
    {IDdot3StatsInternalMacTransmitErrors, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 10}},
    {IDdot3StatsCarrierSenseErrors, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 11}},
    {IDdot3StatsFrameTooLongs, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 13}},
    {IDdot3StatsInternalMacReceiveErrors, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 16}},
    {IDdot3StatsEtherChipSet, ASN_COUNTER, RONLY, var_dot3StatsEntry, 3,
     {2, 1, 17}}
};

/***************************************************
 * Function:init_dot3Statistics
 * Purpose: register dot3 statistics objects in the agent
 ***************************************************/
void
init_dot3Statistics(void)
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;

    DEBUGMSGTL(("[RTK]", "init dot3 statistics table\n"));
    REGISTER_MIB(MIB_DESCR, oidDot3StatisticsVariables, variable7,
                 oidDot3StatisticsVariablesOid);
#if 0
    ROWAPI_init_table(&StatCtrlTable, MIB_DESCR, 0, &stat_Create, NULL, /* &stat_Clone, */
                      NULL,     /* &stat_Delete, */
                      &stat_Validate,
                      &stat_Activate, &stat_Deactivate, &stat_Copy);

    /* init dot3 statistic table */
    LOGIC_PORTMASK_SET_ALL(logicPortmask);
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        ag_trace("%s-%d: [RTK] logic port = %d\n", __FILE__, __LINE__, logicPort);

        /* RTK: logicPort is 0-based. Dot3 statistic table is 1-based */
        add_dot3statistics_entry(logicPort + 1, logicPort + 1);
    }
#endif
}

/*
 * end of file dot3_statistics.c
 */
