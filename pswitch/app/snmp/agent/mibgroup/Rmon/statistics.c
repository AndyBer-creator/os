/**************************************************************
 * Copyright (C) 2001 Tali Rozin, Optical Access
 *
 *                     All Rights Reserved
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * TALI ROZIN DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * ALEX ROZIN BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 ******************************************************************/

#include <net-snmp/net-snmp-config.h>

#if HAVE_STDLIB
#include <stdlib.h>
#endif
#if TIME_WITH_SYS_TIME
# ifdef WIN32
#  include <sys/timeb.h>
# else
#  include <sys/time.h>
# endif
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "util_funcs.h"
#include "statistics.h"
        /*
         * Implementation headers
         */
#include "agutil_api.h"
#include "row_api.h"

/*
 * RTK-20100707: include RTK private header
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
#define MIB_DESCR   "EthStat"
#define etherStatsEntryFirstIndexBegin  11
#define IDetherStatsDroppedFrames        1
#define IDetherStatsCreateTime           2
#define IDetherStatsIndex                3
#define IDetherStatsDataSource           4
#define IDetherStatsDropEvents           5
#define IDetherStatsOctets               6
#define IDetherStatsPkts                 7
#define IDetherStatsBroadcastPkts        8
#define IDetherStatsMulticastPkts        9
#define IDetherStatsCRCAlignErrors       10
#define IDetherStatsUndersizePkts        11
#define IDetherStatsOversizePkts         12
#define IDetherStatsFragments            13
#define IDetherStatsJabbers              14
#define IDetherStatsCollisions           15
#define IDetherStatsPkts64Octets         16
#define IDetherStatsPkts65to127Octets    17
#define IDetherStatsPkts128to255Octets   18
#define IDetherStatsPkts256to511Octets   19
#define IDetherStatsPkts512to1023Octets  20
#define IDetherStatsPkts1024to1518Octets 21
#define IDetherStatsOwner                22
#define IDetherStatsStatus               23
#define Leaf_etherStatsDataSource        2
#define Leaf_etherStatsOwner             20
#define Leaf_etherStatsStatus            21
#define MIN_etherStatsIndex   1
#define MAX_etherStatsIndex   65535
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

/***************************************************
 * Function:var_etherStatsEntry
 * Purpose: Handles the request for etherStatsEntry variable instances
 ***************************************************/

u_char         *
var_etherStatsEntry(struct variable * vp, oid * name, size_t * length,
                    int exact, size_t * var_len,
                    WriteMethod ** write_method)
{
#if 1 /* RTK-20100709: get counters from Hardware */
    static unsigned long     long_return;
    static CRTL_ENTRY_T theEntry;
    RMON_ENTRY_T   *hdr;
    uint64 cntr64;

    DEBUGMSGTL(("[RTK]", "RMON - var_etherStatsEntry() \n"));
    *write_method = write_etherStatsEntry;

    hdr = ROWAPI_header_ControlEntry(vp, name, length, exact, var_len,
                                     table_ptr,
                                     &theEntry, sizeof(CRTL_ENTRY_T));
    if (!hdr)
        return NULL;

    *var_len = sizeof(long);
    DEBUGMSGTL(("[RTK]", "hdr->ctrl_index = %d\n", hdr->ctrl_index));

    switch (vp->magic) {
    case IDetherStatsIndex:
        long_return = hdr->ctrl_index;
        return (u_char *) & long_return;
    case IDetherStatsDataSource:
        *var_len = sizeof(oid) * theEntry.data_source.length;
        return (unsigned char *) theEntry.data_source.objid;
    case IDetherStatsDropEvents:
       // long_return = 0;        /* theEntry.eth.etherStatsDropEvents; */
        sal_mib_etherStatsDropEvents_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsOctets:
        sal_mib_etherStatsOctets_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsPkts:
        sal_mib_etherStatsPkts_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsBroadcastPkts:
        sal_mib_etherStatsBcastPkts_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsMulticastPkts:
        sal_mib_etherStatsMcastPkts_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsCRCAlignErrors:
        sal_mib_etherStatsCRCAlignErrs_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsUndersizePkts:
        sal_mib_etherStatsUnderSizePkts_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsOversizePkts:
        sal_mib_etherStatsOverSizePkts_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsFragments:
        sal_mib_etherStatsFragments_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsJabbers:
        sal_mib_etherStatsJabbers_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsCollisions:
        sal_mib_etherStatsCollisions_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsPkts64Octets:
        sal_mib_etherStatsPkts64Octets_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsPkts65to127Octets:
        sal_mib_etherStatsPkts65to127Octets_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsPkts128to255Octets:
        sal_mib_etherStatsPkts128to255Octets_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsPkts256to511Octets:
        sal_mib_etherStatsPkts256to511Octets_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsPkts512to1023Octets:
        sal_mib_etherStatsPkts512to1023Octets_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsPkts1024to1518Octets:
        sal_mib_etherStatsPkts1024to1518Octets_get(UP2LP(hdr->ctrl_index), &cntr64);
        long_return = (unsigned long) cntr64;
        return (u_char *) & long_return;
    case IDetherStatsOwner:
        if (hdr->owner) {
            *var_len = strlen(hdr->owner);
            return (unsigned char *) hdr->owner;
        } else {
            *var_len = 0;
            return (unsigned char *) "";
        }
    case IDetherStatsStatus:
        long_return = hdr->status;
        return (u_char *) & long_return;
    default:
        ERROR_MSG("");
    };
#else /* RTK-20100709: under is source code */
    static long     long_return;
    static CRTL_ENTRY_T theEntry;
    RMON_ENTRY_T   *hdr;

    DEBUGMSGTL(("[RTK]", "RMON - var_etherStatsEntry() \n"));
    *write_method = write_etherStatsEntry;

    hdr = ROWAPI_header_ControlEntry(vp, name, length, exact, var_len,
                                     table_ptr,
                                     &theEntry, sizeof(CRTL_ENTRY_T));
    if (!hdr)
        return NULL;


    if (RMON1_ENTRY_VALID == hdr->status)
        SYSTEM_get_eth_statistics(&theEntry.data_source, &theEntry.eth);

    *var_len = sizeof(long);

    switch (vp->magic) {
    case IDetherStatsIndex:
        long_return = hdr->ctrl_index;
        return (u_char *) & long_return;
    case IDetherStatsDataSource:
        *var_len = sizeof(oid) * theEntry.data_source.length;
        return (unsigned char *) theEntry.data_source.objid;
    case IDetherStatsDropEvents:
        long_return = 0;        /* theEntry.eth.etherStatsDropEvents; */
        return (u_char *) & long_return;
    case IDetherStatsOctets:
        long_return = theEntry.eth.octets;
        return (u_char *) & long_return;
    case IDetherStatsPkts:
        long_return = theEntry.eth.packets;
        return (u_char *) & long_return;
    case IDetherStatsBroadcastPkts:
        long_return = theEntry.eth.bcast_pkts;
        return (u_char *) & long_return;
    case IDetherStatsMulticastPkts:
        long_return = theEntry.eth.mcast_pkts;
        return (u_char *) & long_return;
    case IDetherStatsCRCAlignErrors:
        long_return = theEntry.eth.crc_align;
        return (u_char *) & long_return;
    case IDetherStatsUndersizePkts:
        long_return = theEntry.eth.undersize;
        return (u_char *) & long_return;
    case IDetherStatsOversizePkts:
        long_return = theEntry.eth.oversize;
        return (u_char *) & long_return;
    case IDetherStatsFragments:
        long_return = theEntry.eth.fragments;
        return (u_char *) & long_return;
    case IDetherStatsJabbers:
        long_return = theEntry.eth.jabbers;
        return (u_char *) & long_return;
    case IDetherStatsCollisions:
        long_return = theEntry.eth.collisions;
        return (u_char *) & long_return;
    case IDetherStatsPkts64Octets:
        long_return = theEntry.eth.pkts_64;
        return (u_char *) & long_return;
    case IDetherStatsPkts65to127Octets:
        long_return = theEntry.eth.pkts_65_127;
        return (u_char *) & long_return;
    case IDetherStatsPkts128to255Octets:
        long_return = theEntry.eth.pkts_128_255;
        return (u_char *) & long_return;
    case IDetherStatsPkts256to511Octets:
        long_return = theEntry.eth.pkts_256_511;
        return (u_char *) & long_return;
    case IDetherStatsPkts512to1023Octets:
        long_return = theEntry.eth.pkts_512_1023;
        return (u_char *) & long_return;
    case IDetherStatsPkts1024to1518Octets:
        long_return = theEntry.eth.pkts_1024_1518;
        return (u_char *) & long_return;
    case IDetherStatsOwner:
        if (hdr->owner) {
            *var_len = strlen(hdr->owner);
            return (unsigned char *) hdr->owner;
        } else {
            *var_len = 0;
            return (unsigned char *) "";
        }
    case IDetherStatsStatus:
        long_return = hdr->status;
        return (u_char *) & long_return;
    default:
        ERROR_MSG("");
    };                          /* of switch by 'vp->magic'  */
#endif /* RTK-20100709:  */

    return NULL;
}

#if 1                           /* debug, but may be used for init. TBD: may be token snmpd.conf ? */
int
add_statistics_entry(int ctrl_index, int ifIndex)
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
oid             oidstatisticsVariablesOid[] = { 1, 3, 6, 1, 2, 1, 16, 1 };

struct variable7 oidstatisticsVariables[] = {
    {IDetherStatsIndex, ASN_INTEGER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 1}},
    {IDetherStatsDataSource, ASN_OBJECT_ID, RWRITE, var_etherStatsEntry, 3,
     {1, 1, 2}},
    {IDetherStatsDropEvents, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 3}},
    {IDetherStatsOctets, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 4}},
    {IDetherStatsPkts, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 5}},
    {IDetherStatsBroadcastPkts, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 6}},
    {IDetherStatsMulticastPkts, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 7}},
    {IDetherStatsCRCAlignErrors, ASN_COUNTER, RONLY, var_etherStatsEntry,
     3, {1, 1, 8}},
    {IDetherStatsUndersizePkts, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 9}},
    {IDetherStatsOversizePkts, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 10}},
    {IDetherStatsFragments, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 11}},
    {IDetherStatsJabbers, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 12}},
    {IDetherStatsCollisions, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 13}},
    {IDetherStatsPkts64Octets, ASN_COUNTER, RONLY, var_etherStatsEntry, 3,
     {1, 1, 14}},
    {IDetherStatsPkts65to127Octets, ASN_COUNTER, RONLY,
     var_etherStatsEntry, 3, {1, 1, 15}},
    {IDetherStatsPkts128to255Octets, ASN_COUNTER, RONLY,
     var_etherStatsEntry, 3, {1, 1, 16}},
    {IDetherStatsPkts256to511Octets, ASN_COUNTER, RONLY,
     var_etherStatsEntry, 3, {1, 1, 17}},
    {IDetherStatsPkts512to1023Octets, ASN_COUNTER, RONLY,
     var_etherStatsEntry, 3, {1, 1, 18}},
    {IDetherStatsPkts1024to1518Octets, ASN_COUNTER, RONLY,
     var_etherStatsEntry, 3, {1, 1, 19}},
    {IDetherStatsOwner, ASN_OCTET_STR, RWRITE, var_etherStatsEntry, 3,
     {1, 1, 20}},
    {IDetherStatsStatus, ASN_INTEGER, RWRITE, var_etherStatsEntry, 3,
     {1, 1, 21}},
#if 1 /* RTK-20100707: doesn't show the value of unsupported objects */
    {IDetherStatsDroppedFrames, ASN_COUNTER, RONLY, var_etherStats2Entry,
     3, {4, 1, 1}},
    {IDetherStatsCreateTime, ASN_TIMETICKS, RONLY, var_etherStats2Entry, 3,
     {4, 1, 2}},
#endif
};

/***************************************************
 * Function:init_statistics
 * Purpose: register statistics objects in the agent
 ***************************************************/
void
init_statistics(void)
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;

    DEBUGMSGTL(("[RTK]", "RMON init statistics table\n"));
    REGISTER_MIB(MIB_DESCR, oidstatisticsVariables, variable7,
                 oidstatisticsVariablesOid);

    ROWAPI_init_table(&StatCtrlTable, MIB_DESCR, 0, &stat_Create, NULL, /* &stat_Clone, */
                      NULL,     /* &stat_Delete, */
                      &stat_Validate,
                      &stat_Activate, &stat_Deactivate, &stat_Copy);

#if 0                           /* debug */
    {
        int             iii;
        for (iii = 1; iii < 6; iii++) {
            add_statistics_entry(iii, iii);
        }

        add_statistics_entry(10, 16);
        add_statistics_entry(12, 11);
    }
#endif

#if 1 /* RTK-20100709: create statistic table <START> */
    LOGIC_PORTMASK_SET_ALL(logicPortmask);
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        /* RTK-20100709: logicPort is 0-based. RMON statistic table is 1-based */
        add_statistics_entry(LP2UP(logicPort), LP2UP(logicPort));
    }
#endif /* RTK-20100709: create statistic table <END> */

}

/*
 * end of file statistics.c
 */
