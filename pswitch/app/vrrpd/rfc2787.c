/* rfc2787.c - CCD+ Module source for rfc2787.mi2. */

/* Copyright 2005 Wind River, Inc. */

/*
modification history
--------------------
xxx,07Sep05,ccd Created by CCDGEN. Ver 2.28
%%BeginUserCode rfc2787_Modifications
%%EndUserCode   rfc2787_Modifications
*/

/*
DESCRIPTION
This module defines the CCD+ structures and CCD+ Object handler routines to
access the CCD+ objects defined in rfc2787.mi2.

INCLUDE FILES: rfc2787.h
*/

/* Includes */

/* %%BeginUserCode rfc2787_IncludeFiles */
#include <string.h>

#include <startup/configGLN.h>
#ifdef GBN_INCLUDE_VRRP
#include <h/glnTypes.h>
#include <h/vos/vosSystem.h>
#include <h/platform/ccd/ccdLib.h>
#include <h/platform/common/miscLib.h>
#include <h/l3/vrrp/vrrpTask.h>
#ifdef GLN_INCLUDE_TRACK
#include <h/platform/track/trackTask.h>
#endif

#include "vrrpd.h"
#include "vrrpFuncParse.h"
#include "rfc2787.h"
/* %%EndUserCode   rfc2787_IncludeFiles */

/* Defines */
/* %%BeginUserCode rfc2787_Defines */
/* %%EndUserCode   rfc2787_Defines */

/* Globals */
/* %%BeginUserCode rfc2787_Globals */
/* %%EndUserCode   rfc2787_Globals */

/* Locals */
LOCAL char * moduleName = "rfc2787";
/* %%BeginUserCode rfc2787_Locals */
/* %%EndUserCode   rfc2787_Locals */

/* Prototypes */
LOCAL STATUS rfc2787ScalarHandler ( ccdRequest_t request, ccd_t * ccd );
LOCAL STATUS vrrpOperEntryTableHandler ( ccdRequest_t request, ccd_t * ccd );
LOCAL STATUS vrrpAssoIpAddrEntryTableHandler ( ccdRequest_t request, ccd_t * ccd );
LOCAL STATUS vrrpTrackEntryTableHandler ( ccdRequest_t request, ccd_t * ccd );
LOCAL STATUS vrrpRouterStatsEntryTableHandler ( ccdRequest_t request, ccd_t * ccd );

/* %%BeginUserCode rfc2787_Prototypes */
LOCAL STATUS vrrpOperEntryTableHelper(ccdRequest_t request, ccd_t * ccd, vrrp_rt **ppVsrv);
LOCAL STATUS vrrpAssoIpAddrEntryTableHelper(ccdRequest_t request, ccd_t * ccd, vrrp_rt **ppVsrv, int *pPos);
LOCAL STATUS vrrpTrackEntryTableHelper(ccdRequest_t request, ccd_t * ccd, vrrp_rt * * ppVsrv, int * pPos);
/* %%EndUserCode   rfc2787_Prototypes */


/****************************************************************************
* CCD+ Module Object Descriptors
*/

LOCAL ccdObjDesc_t vrrpNodeVersion_objDesc[] = {
  { vrrpNodeVersion, 1, 4, /* localId=0  1.3.6.1.2.1.68.1.1 */
    { 0, 0 }, CCD_INTEGER, CCD_READ,
    (CCD_VALID), rfc2787ScalarHandler,
    (char *)0, "rfc2787:vrrpNodeVersion" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpNotificationCntl_objDesc[] = {
  { vrrpNotificationCntl, 2, 4, /* localId=1  1.3.6.1.2.1.68.1.2 */
    { 1, 2 }, CCD_INTEGER, (CCD_READ|CCD_WRITE),
    (CCD_VALID), rfc2787ScalarHandler,
    (char *)0, "rfc2787:vrrpNotificationCntl" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpOperEntry_objDesc[] = {
  { vrrpOperVrId, 1, 4, /* localId=2  1.3.6.1.2.1.68.1.3.1.1 */
    { 1, 255 }, CCD_INTEGER, CCD_NOT_ACCESSIBLE,
    (CCD_VALID|CCD_INDEX), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperVrId" },
  { vrrpOperVirtualMacAddr, 2, 6, /* localId=3  1.3.6.1.2.1.68.1.3.1.2 */
    { 0, 0 }, CCD_MacAddress, CCD_READ,
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperVirtualMacAddr" },
  { vrrpOperState, 3, 4, /* localId=4  1.3.6.1.2.1.68.1.3.1.3 */
    { 1, 3 }, CCD_INTEGER, CCD_READ,
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperState" },
  { vrrpOperAdminState, 4, 4, /* localId=5  1.3.6.1.2.1.68.1.3.1.4 */
    { 1, 2 }, CCD_INTEGER, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperAdminState" },
  { vrrpOperPriority, 5, 4, /* localId=6  1.3.6.1.2.1.68.1.3.1.5 */
    { 0, 255 }, CCD_INTEGER, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperPriority" },
  { vrrpOperIpAddrCount, 6, 4, /* localId=7  1.3.6.1.2.1.68.1.3.1.6 */
    { 0, 255 }, CCD_INTEGER, CCD_READ,
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperIpAddrCount" },
  { vrrpOperMasterIpAddr, 7, 4, /* localId=8  1.3.6.1.2.1.68.1.3.1.7 */
    { 0, 0 }, CCD_IpAddress, CCD_READ,
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperMasterIpAddr" },
  { vrrpOperPrimaryIpAddr, 8, 4, /* localId=9  1.3.6.1.2.1.68.1.3.1.8 */
    { 0, 0 }, CCD_IpAddress, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperPrimaryIpAddr" },
  { vrrpOperAuthType, 9, 4, /* localId=10  1.3.6.1.2.1.68.1.3.1.9 */
    { 1, 3 }, CCD_INTEGER, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperAuthType" },
  { vrrpOperAuthKey, 10, 255, /* localId=11  1.3.6.1.2.1.68.1.3.1.10 */
    { 0, 0 }, CCD_OCTET_STRING, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperAuthKey" },
  { vrrpOperAdvertisementInterval, 11, 4, /* localId=12  1.3.6.1.2.1.68.1.3.1.11 */
    { 1, 255 }, CCD_INTEGER, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperAdvertisementInterval" },
  { vrrpOperPreemptMode, 12, 4, /* localId=13  1.3.6.1.2.1.68.1.3.1.12 */
    { 1, 2 }, CCD_INTEGER, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperPreemptMode" },
  { vrrpOperVirtualRouterUpTime, 13, 4, /* localId=14  1.3.6.1.2.1.68.1.3.1.13 */
    { 0, 0 }, CCD_TimeTicks, CCD_READ,
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperVirtualRouterUpTime" },
  { vrrpOperProtocol, 14, 4, /* localId=15  1.3.6.1.2.1.68.1.3.1.14 */
    { 1, 4 }, CCD_INTEGER, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperProtocol" },
  { vrrpOperRowStatus, 15, 4, /* localId=16  1.3.6.1.2.1.68.1.3.1.15 */
    { 1, 6 }, CCD_RowStatus, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpOperEntryTableHandler,
    (char *)0, "rfc2787:vrrpOperRowStatus" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpAssoIpAddrEntry_objDesc[] = {
  { vrrpAssoIpAddr, 1, 4, /* localId=17  1.3.6.1.2.1.68.1.4.1.1 */
    { 0, 0 }, CCD_IpAddress, CCD_NOT_ACCESSIBLE,
    (CCD_VALID|CCD_INDEX), vrrpAssoIpAddrEntryTableHandler,
    (char *)0, "rfc2787:vrrpAssoIpAddr" },
  { vrrpAssoIpAddrRowStatus, 2, 4, /* localId=18  1.3.6.1.2.1.68.1.4.1.2 */
    { 1, 6 }, CCD_RowStatus, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpAssoIpAddrEntryTableHandler,
    (char *)0, "rfc2787:vrrpAssoIpAddrRowStatus" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpTrapPacketSrc_objDesc[] = {
  { vrrpTrapPacketSrc, 5, 4, /* localId=19  1.3.6.1.2.1.68.1.5 */
    { 0, 0 }, CCD_IpAddress, CCD_NOT_ACCESSIBLE,
    (CCD_VALID), rfc2787ScalarHandler,
    (char *)0, "rfc2787:vrrpTrapPacketSrc" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpTrapAuthErrorType_objDesc[] = {
  { vrrpTrapAuthErrorType, 6, 4, /* localId=20  1.3.6.1.2.1.68.1.6 */
    { 1, 3 }, CCD_INTEGER, CCD_NOT_ACCESSIBLE,
    (CCD_VALID), rfc2787ScalarHandler,
    (char *)0, "rfc2787:vrrpTrapAuthErrorType" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpTrackEntry_objDesc[] = {
  { vrrpTrackifIndex, 1, 4, /* localId=21  1.3.6.1.2.1.68.1.7.1.1 */
    { 0, 0 }, CCD_INTEGER, CCD_READ,
    (CCD_VALID|CCD_INDEX), vrrpTrackEntryTableHandler,
    (char *)0, "rfc2787:vrrpTrackifIndex" },
  { vrrpTrackPriReduced, 2, 4, /* localId=22  1.3.6.1.2.1.68.1.7.1.2 */
    { 0, 255 }, CCD_INTEGER, (CCD_READ|CCD_WRITE),
    (CCD_VALID), vrrpTrackEntryTableHandler,
    (char *)0, "rfc2787:vrrpTrackPriReduced" },
  { vrrpTrackRowStatus, 3, 4, /* localId=23  1.3.6.1.2.1.68.1.7.1.3 */
    { 1, 6 }, CCD_RowStatus, (CCD_READ|CCD_CREATE),
    (CCD_VALID), vrrpTrackEntryTableHandler,
    (char *)0, "rfc2787:vrrpTrackRowStatus" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpRouterChecksumErrors_objDesc[] = {
  { vrrpRouterChecksumErrors, 1, 4, /* localId=24  1.3.6.1.2.1.68.2.1 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), rfc2787ScalarHandler,
    (char *)0, "rfc2787:vrrpRouterChecksumErrors" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpRouterVersionErrors_objDesc[] = {
  { vrrpRouterVersionErrors, 2, 4, /* localId=25  1.3.6.1.2.1.68.2.2 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), rfc2787ScalarHandler,
    (char *)0, "rfc2787:vrrpRouterVersionErrors" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpRouterVrIdErrors_objDesc[] = {
  { vrrpRouterVrIdErrors, 3, 4, /* localId=26  1.3.6.1.2.1.68.2.3 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), rfc2787ScalarHandler,
    (char *)0, "rfc2787:vrrpRouterVrIdErrors" },
  { 0 }
};

LOCAL ccdObjDesc_t vrrpRouterStatsEntry_objDesc[] = {
  { vrrpStatsBecomeMaster, 1, 4, /* localId=27  1.3.6.1.2.1.68.2.4.1.1 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsBecomeMaster" },
  { vrrpStatsAdvertiseRcvd, 2, 4, /* localId=28  1.3.6.1.2.1.68.2.4.1.2 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsAdvertiseRcvd" },
  { vrrpStatsAdvertiseIntervalErrors, 3, 4, /* localId=29  1.3.6.1.2.1.68.2.4.1.3 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsAdvertiseIntervalErrors" },
  { vrrpStatsAuthFailures, 4, 4, /* localId=30  1.3.6.1.2.1.68.2.4.1.4 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsAuthFailures" },
  { vrrpStatsIpTtlErrors, 5, 4, /* localId=31  1.3.6.1.2.1.68.2.4.1.5 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsIpTtlErrors" },
  { vrrpStatsPriorityZeroPktsRcvd, 6, 4, /* localId=32  1.3.6.1.2.1.68.2.4.1.6 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsPriorityZeroPktsRcvd" },
  { vrrpStatsPriorityZeroPktsSent, 7, 4, /* localId=33  1.3.6.1.2.1.68.2.4.1.7 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsPriorityZeroPktsSent" },
  { vrrpStatsInvalidTypePktsRcvd, 8, 4, /* localId=34  1.3.6.1.2.1.68.2.4.1.8 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsInvalidTypePktsRcvd" },
  { vrrpStatsAddressListErrors, 9, 4, /* localId=35  1.3.6.1.2.1.68.2.4.1.9 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsAddressListErrors" },
  { vrrpStatsInvalidAuthType, 10, 4, /* localId=36  1.3.6.1.2.1.68.2.4.1.10 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsInvalidAuthType" },
  { vrrpStatsAuthTypeMismatch, 11, 4, /* localId=37  1.3.6.1.2.1.68.2.4.1.11 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsAuthTypeMismatch" },
  { vrrpStatsPacketLengthErrors, 12, 4, /* localId=38  1.3.6.1.2.1.68.2.4.1.12 */
    { 0, 0 }, CCD_Counter32, CCD_READ,
    (CCD_VALID), vrrpRouterStatsEntryTableHandler,
    (char *)0, "rfc2787:vrrpStatsPacketLengthErrors" },
  { 0 }
};

/*****************************************************************************
* CCD+ Module OID Table
*
* The OID list below (e.g.,{1,3,6,1, ...}) is the OID prefix minus the
* last sub-identifier.  The last sub-identifier is found in the
* object descriptor ccdObjDesc_t structure.
*/

LOCAL ccdOidTable_t oidTable[] = {
  /*   0: Scalar Object -- vrrpNodeVersion */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,1}, 9,   /*lint !e785 */
    0, 1, { 0 },
    vrrpNodeVersion_objDesc },

  /*   1: Scalar Object -- vrrpNotificationCntl */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,1}, 9,   /*lint !e785 */
    0, 1, { 0 },
    vrrpNotificationCntl_objDesc },

  /*   2: Table Object -- vrrpOperEntry */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,1,3,1}, 11,   /*lint !e785 */
    2, 2, { "rfc2233:ifIndex", "rfc2787:vrrpOperVrId" }, /*lint !e785 */
    vrrpOperEntry_objDesc },

  /*   3: Table Object -- vrrpAssoIpAddrEntry */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,1,4,1}, 11,   /*lint !e785 */
    3, 6, { "rfc2233:ifIndex", "rfc2787:vrrpOperVrId",
         "rfc2787:vrrpAssoIpAddr" }, /*lint !e785 */
    vrrpAssoIpAddrEntry_objDesc },

  /*   4: Scalar Object -- vrrpTrapPacketSrc */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,1}, 9,   /*lint !e785 */
    0, 1, { 0 },
    vrrpTrapPacketSrc_objDesc },

  /*   5: Scalar Object -- vrrpTrapAuthErrorType */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,1}, 9,   /*lint !e785 */
    0, 1, { 0 },
    vrrpTrapAuthErrorType_objDesc },

  /*   6: Table Object -- vrrpTrackEntry */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,1,7,1}, 11,   /*lint !e785 */
    3, 3, { "rfc2233:ifIndex", "rfc2787:vrrpOperVrId",
         "rfc2787:vrrpTrackifIndex" }, /*lint !e785 */
    vrrpTrackEntry_objDesc },

  /*   7: Scalar Object -- vrrpRouterChecksumErrors */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,2}, 9,   /*lint !e785 */
    0, 1, { 0 },
    vrrpRouterChecksumErrors_objDesc },

  /*   8: Scalar Object -- vrrpRouterVersionErrors */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,2}, 9,   /*lint !e785 */
    0, 1, { 0 },
    vrrpRouterVersionErrors_objDesc },

  /*   9: Scalar Object -- vrrpRouterVrIdErrors */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,2}, 9,   /*lint !e785 */
    0, 1, { 0 },
    vrrpRouterVrIdErrors_objDesc },

  /*  10: Table Object -- vrrpRouterStatsEntry */
  { CCD_SNMP_STRUCT, {1,3,6,1,2,1,68,2,4,1}, 11,   /*lint !e785 */
    2, 2, { "rfc2233:ifIndex", "rfc2787:vrrpOperVrId" }, /*lint !e785 */
    vrrpRouterStatsEntry_objDesc },

  { CCD_TERMINATOR }
};

/*****************************************************************************
* CCD+ Module Translate Table
*/

LOCAL ccdXlateTable_t xlateTable[] = {
    { &oidTable[  0], &vrrpNodeVersion_objDesc[0] },
    { &oidTable[  1], &vrrpNotificationCntl_objDesc[0] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[0] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[1] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[2] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[3] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[4] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[5] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[6] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[7] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[8] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[9] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[10] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[11] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[12] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[13] },
    { &oidTable[  2], &vrrpOperEntry_objDesc[14] },
    { &oidTable[  3], &vrrpAssoIpAddrEntry_objDesc[0] },
    { &oidTable[  3], &vrrpAssoIpAddrEntry_objDesc[1] },
    { &oidTable[  4], &vrrpTrapPacketSrc_objDesc[0] },
    { &oidTable[  5], &vrrpTrapAuthErrorType_objDesc[0] },
    { &oidTable[  6], &vrrpTrackEntry_objDesc[0] },
    { &oidTable[  6], &vrrpTrackEntry_objDesc[1] },
    { &oidTable[  6], &vrrpTrackEntry_objDesc[2] },
    { &oidTable[  7], &vrrpRouterChecksumErrors_objDesc[0] },
    { &oidTable[  8], &vrrpRouterVersionErrors_objDesc[0] },
    { &oidTable[  9], &vrrpRouterVrIdErrors_objDesc[0] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[0] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[1] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[2] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[3] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[4] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[5] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[6] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[7] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[8] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[9] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[10] },
    { &oidTable[ 10], &vrrpRouterStatsEntry_objDesc[11] },

    { 0 }
};

/******************************************************************************
* User Local Functions.
*/

/* %%BeginUserCode rfc2787_UserFunctions */

/*******************************************************************************
  函数: vrrpOperEntryTableHelper
  描述: 获取OperEntry表的当前项或下一项
  输入: request: ccd请求类型
        ccd: 请求参数结构指针
  输出: *ppVsrv: 找到的备份组项
  返回: OK: 执行成功；ERROR: 执行失败
  其它: 
*******************************************************************************/
LOCAL STATUS vrrpOperEntryTableHelper(ccdRequest_t request,
                ccd_t * ccd, vrrp_rt **ppVsrv)
{
    int i = 0;
    int iPos = -1;

    CHECK_VALID(ppVsrv, ERROR);
    
    if (ccd->indexSubIdCount == 0)
    {
        if (CCD_GET_NEXT != request)
        {
            return ERROR;
        }
        
        /* 获取第一个备份组项 */
        for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
        {
            if (!gVrrp_vsrv[i].used)
            {
                continue;
            }
            *ppVsrv = &gVrrp_vsrv[i];
            ccd->pIndex[0] = gVrrp_vsrv[i].vif.ifindex;
            ccd->pIndex[1] = gVrrp_vsrv[i].vrid;
            ccd->indexSubIdCount = ccd->indexSubIdCountMax;
            return OK;
        }        
        return ERROR;
    }
    else
    {
        if (2 != ccd->indexSubIdCount)
        {
            return ERROR;
        }

        /* 查找当前备份组项 */
        for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
        {
            if (!gVrrp_vsrv[i].used || gVrrp_vsrv[i].vif.ifindex != ccd->pIndex[0]
                            || gVrrp_vsrv[i].vrid != ccd->pIndex[1])
            {
                continue;
            }
            iPos = i;
            break;
        }
        /* 未找到匹配项 */
        if (-1 == iPos)
        {
            return ERROR;
        }

        if (CCD_GET_NEXT != request)
        {
            *ppVsrv = &gVrrp_vsrv[iPos];
            return OK;
        }

        /* 已找到匹配项，获取下一项 */
        for (i = iPos + 1; i < VRRP_VSRV_SIZE_MAX; i++)
        {
            if (!gVrrp_vsrv[i].used)
            {
                continue;
            }
            *ppVsrv = &gVrrp_vsrv[i];
            ccd->pIndex[0] = gVrrp_vsrv[i].vif.ifindex;
            ccd->pIndex[1] = gVrrp_vsrv[i].vrid;
            ccd->indexSubIdCount = ccd->indexSubIdCountMax;
            return OK;
        }
        /* 无下一项 */
        return ERROR;
    }
    
    return ERROR;
}

/*******************************************************************************
  函数: vrrpAssoIpAddrEntryTableHelper
  描述: 获取AssoIpAddrEntry表的当前项或下一项
  输入: request: ccd请求类型
        ccd: 请求参数结构指针
  输出: *ppVsrv: 找到的备份组项
        *pPos: IP地址在地址数组中的下标
  返回: OK: 执行成功；ERROR: 执行失败
  其它: 
*******************************************************************************/
LOCAL STATUS vrrpAssoIpAddrEntryTableHelper(ccdRequest_t request,
                ccd_t * ccd, vrrp_rt **ppVsrv, int *pPos)
{
    int i = 0;
    ulong_t ipaddr = 0;
    vrrp_rt *pVsrv = NULL;
    
    CHECK_VALID(ppVsrv, ERROR);

    if (CCD_GET_NEXT != request)
    {
        /* 查找对应的备份组项 */
        ccd->indexSubIdCount = 2;
        if (OK != vrrpOperEntryTableHelper(request, ccd, ppVsrv))
        {
            return ERROR;
        }

        pVsrv = *ppVsrv;
        ipaddr = miscIpIndex2IpAddr(ccd->pIndex + 2);
        /* 查找备份组项中匹配的IP地址 */
        for (i = 0; i < pVsrv->naddr; i++)
        {
            if (pVsrv->vaddr[i].addr == ipaddr)
            {
                *pPos = i;
                ccd->indexSubIdCount = ccd->indexSubIdCountMax;
                return OK;
            }
        }
        return ERROR;
    }

    /* CCD_GET_NEXT操作 */
    if (ccd->indexSubIdCount == 0)
    {
        /* 查找第一个存在的备份组项 */
        if (OK != vrrpOperEntryTableHelper(request, ccd, ppVsrv))
        {
            return ERROR;
        }
        /* 查找备份组项中的第一个IP地址 */
        pVsrv = *ppVsrv;
        if (pVsrv->naddr > 0)
        {
            *pPos = 0;
            miscIpAddr2IpIndex(pVsrv->vaddr[0].addr, ccd->pIndex + 2);
            return OK;
        }
    }
    else if (ccd->indexSubIdCount != 6)
    {
        return ERROR;
    }
    else
    {
        /* 查找匹配的备份组项及IP地址 */
        if (OK != vrrpAssoIpAddrEntryTableHelper(CCD_GET, ccd, ppVsrv, pPos))
        {
            return ERROR;
        }
        /* 查找备份组项的下一个IP地址 */
        pVsrv = *ppVsrv;
        if (pVsrv->naddr > *pPos + 1)
        {
            (*pPos)++;
            miscIpAddr2IpIndex(pVsrv->vaddr[*pPos].addr, ccd->pIndex + 2);
            return OK;
        }
    }

    /* 查找下一个存在IP地址的备份组项，获取其第一个IP地址 */
    for (;;)
    {
        ccd->indexSubIdCount = 2;
        if (OK != vrrpOperEntryTableHelper(CCD_GET_NEXT, ccd, ppVsrv))
        {
            return ERROR;
        }
        ccd->indexSubIdCount = ccd->indexSubIdCountMax;
        pVsrv = *ppVsrv;
        if (pVsrv->naddr > 0)
        {
            *pPos = 0;
            miscIpAddr2IpIndex(pVsrv->vaddr[0].addr, ccd->pIndex + 2);
            return OK;
        }
    }    
}

/*******************************************************************************
  函数: vrrpAssoIpAddrEntryTableHelper
  描述: 获取AssoIpAddrEntry表的当前项或下一项
  输入: request: ccd请求类型
        ccd: 请求参数结构指针
  输出: *ppVsrv: 找到的备份组项
        *pPos: 监视接口在数组中的下标
  返回: OK: 执行成功；ERROR: 执行失败
  其它: 
*******************************************************************************/
LOCAL STATUS vrrpTrackEntryTableHelper(ccdRequest_t request,
                ccd_t * ccd, vrrp_rt **ppVsrv, int *pPos)
{
    int i = 0;
    ulong_t trackifindex = 0;
    vrrp_rt *pVsrv = NULL;
    
    CHECK_VALID(ppVsrv, ERROR);

    if (CCD_GET_NEXT != request)
    {
        /* 查找对应的备份组项 */
        ccd->indexSubIdCount = 2;
        if (OK != vrrpOperEntryTableHelper(request, ccd, ppVsrv))
        {
            return ERROR;
        }

        pVsrv = *ppVsrv;
        trackifindex = ccd->pIndex[2];
        /* 查找备份组项中匹配的监视接口 */
        for (i = 0; i < pVsrv->niftrack; i++)
        {
            if (pVsrv->iftrack[i] == trackifindex)
            {
                *pPos = i;
                ccd->indexSubIdCount = ccd->indexSubIdCountMax;
                return OK;
            }
        }
        return ERROR;
    }

    /* CCD_GET_NEXT操作 */
    if (ccd->indexSubIdCount == 0)
    {
        /* 查找第一个存在的备份组项 */
        if (OK != vrrpOperEntryTableHelper(request, ccd, ppVsrv))
        {
            return ERROR;
        }
        /* 查找备份组项中的第一个监视接口 */
        pVsrv = *ppVsrv;
        if (pVsrv->niftrack > 0)
        {
            *pPos = 0;
            ccd->pIndex[2] = pVsrv->iftrack[0];
            return OK;
        }
    }
    else if (ccd->indexSubIdCount != 3)
    {
        return ERROR;
    }
    else
    {
        /* 查找匹配的备份组项及监视接口 */
        if (OK != vrrpTrackEntryTableHelper(CCD_GET, ccd, ppVsrv, pPos))
        {
            return ERROR;
        }
        /* 查找备份组项的下一个监视接口 */
        pVsrv = *ppVsrv;
        if (pVsrv->niftrack > *pPos + 1)
        {
            (*pPos)++;
            ccd->pIndex[2] = pVsrv->iftrack[*pPos];
            return OK;
        }
    }

    /* 查找下一个存在监视接口的备份组项，获取其第一个监视接口 */
    for (;;)
    {
        ccd->indexSubIdCount = 2;
        if (OK != vrrpOperEntryTableHelper(CCD_GET_NEXT, ccd, ppVsrv))
        {
            return ERROR;
        }
        ccd->indexSubIdCount = ccd->indexSubIdCountMax;
        pVsrv = *ppVsrv;
        if (pVsrv->niftrack > 0)
        {
            *pPos = 0;
            ccd->pIndex[2] = pVsrv->iftrack[0];
            return OK;
        }
    } 
}

/* %%EndUserCode   rfc2787_UserFunctions */


/****************************************************************************
*
* rfc2787Init - initialize/register rfc2787 and objects.
*
* This routine initializes the modules CCD routines.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS rfc2787Init
    (
    void * parm1,
    void * parm2
    )
    {
    STATUS    rc;

    /* %%BeginUserCode rfc2787Init_Prolog */
    /* %%EndUserCode   rfc2787Init_Prolog */

    rc = OK;

    /* %%BeginUserCode rfc2787Init_Body */
    /* %%EndUserCode rfc2787Init_Body */

    /* Register the module's information with CCD+ engine */
    ccdModuleRegister( moduleName, &xlateTable[0] );

    /* %%BeginUserCode rfc2787Init_Epilog */
    /* %%EndUserCode   rfc2787Init_Epilog */

    return( rc );
    }

/******************************************************************************
*
* rfc2787Terminate - Terminate rfc2787 and objects.
*
* This routine de-initializes the CCD module functions.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS rfc2787Terminate
    (
    void * parm
    )
    {
    STATUS    rc;

    /* %%BeginUserCode rfc2787Terminate_Prolog */
    /* %%EndUserCode   rfc2787Terminate_Prolog */

    rc = OK;

    /* De-register the module's information with CCD+ engine */
    ccdModuleDeRegister( moduleName );

    /* %%BeginUserCode rfc2787Terminate_Epilog */
    /* %%EndUserCode   rfc2787Terminate_Epilog */

    return( rc );
    }

/******************************************************************************
*
* rfc2787ScalarHandler - handles all scalar objects.
*
* This routine handles all scalar objects for this CCD+ Module.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS rfc2787ScalarHandler
    (
    ccdRequest_t request,
    ccd_t * ccd
    )
    {
    STATUS    rc;

    /* %%BeginUserCode rfc2787ScalarHandler_Prolog */
    /* %%EndUserCode   rfc2787Scalarhandler_Prolog */

    rc = OK;

    /* Validate the Scalar indexSubIdCount and instance value */
    if ( ccdScalarValidate( request, ccd ) == ERROR )
        return ccdErrorRequestMap( request );

    switch( ccd->localId )
    {
    default:
        rc = ccdErrorNoSuchInstance();
        break;

    /* ================================================================= */
    case vrrpNodeVersion:    /* INTEGER, read-only */
        {
        /* %%BeginUserCode vrrpNodeVersion_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
            /* Check to make sure the next object is valid. */
            rc = ccdNextScalarIndex(ccd);

            /* FALLTHRU */
        case CCD_GET:
            if ( rc == OK )
            {
                ulongSet(ccd, VRRP_VERSION);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpNodeVersion_Body */
        }
        break;

    /* ================================================================= */
    /*
    * EvrrpNotificationCntl_enabled 1
    * EvrrpNotificationCntl_disabled 2
    */
    case vrrpNotificationCntl:    /* INTEGER, read-write */
        {
        /* %%BeginUserCode vrrpNotificationCntl_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
            /* Check to make sure the next object is valid. */
            rc = ccdNextScalarIndex(ccd);

            /* FALLTHRU */
        case CCD_GET:
            if ( rc == OK )
            {
                ulongSet(ccd, gVrrp_NotificationCntl);
            }

            break;

        case CCD_VALIDATE:
            if ( (ulongGet(ccd) < 1) || (ulongGet(ccd) > 2) )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;


        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            gVrrp_NotificationCntl = ulongGet(ccd);
            break;
        }

        /* %%EndUserCode   vrrpNotificationCntl_Body */
        }
        break;

    /* ================================================================= */
    case vrrpTrapPacketSrc:    /* IpAddress, accessible-for-notify */
        {
        /* %%BeginUserCode vrrpTrapPacketSrc_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
            /* Check to make sure the next object is valid. */
            rc = ccdNextScalarIndex(ccd);

            /* FALLTHRU */
        case CCD_GET:
            if ( rc == OK )
            {
                miscIpAddr2OctetString(gVrrp_TrapPktSrc, octetGet(ccd));
                ccd->valueLen = 4;
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpTrapPacketSrc_Body */
        }
        break;

    /* ================================================================= */
    /*
    * EvrrpTrapAuthErrorType_invalidAuthType 1
    * EvrrpTrapAuthErrorType_authTypeMismatch 2
    * EvrrpTrapAuthErrorType_authFailure 3
    */
    case vrrpTrapAuthErrorType:    /* INTEGER, accessible-for-notify */
        {
        /* %%BeginUserCode vrrpTrapAuthErrorType_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
            /* Check to make sure the next object is valid. */
            rc = ccdNextScalarIndex(ccd);

            /* FALLTHRU */
        case CCD_GET:
            if ( rc == OK )
            {
                ulongSet(ccd, gVrrp_TrapAuthErrType);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpTrapAuthErrorType_Body */
        }
        break;

    /* ================================================================= */
    case vrrpRouterChecksumErrors:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpRouterChecksumErrors_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
            /* Check to make sure the next object is valid. */
            rc = ccdNextScalarIndex(ccd);

            /* FALLTHRU */
        case CCD_GET:
            if ( rc == OK )
            {
                ulongSet(ccd, gVrrp_ChecksumErrors);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpRouterChecksumErrors_Body */
        }
        break;

    /* ================================================================= */
    case vrrpRouterVersionErrors:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpRouterVersionErrors_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
            /* Check to make sure the next object is valid. */
            rc = ccdNextScalarIndex(ccd);

            /* FALLTHRU */
        case CCD_GET:
            if ( rc == OK )
            {
                ulongSet(ccd, gVrrp_VersionErrors);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpRouterVersionErrors_Body */
        }
        break;

    /* ================================================================= */
    case vrrpRouterVrIdErrors:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpRouterVrIdErrors_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
            /* Check to make sure the next object is valid. */
            rc = ccdNextScalarIndex(ccd);

            /* FALLTHRU */
        case CCD_GET:
            if ( rc == OK )
            {
                ulongSet(ccd, gVrrp_VrIdErrors);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpRouterVrIdErrors_Body */
        }
        break;

    }    

    /* %%BeginUserCode rfc2787Scalar_Epilog */
    /* %%EndUserCode   rfc2787Scalar_Epilog */

    return( rc );
    }

/******************************************************************************
*
* vrrpOperEntryTableHandler - handles table objects.
*
* This routine handles all objects for this table.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS vrrpOperEntryTableHandler
    (
    ccdRequest_t request,
    ccd_t * ccd
    )
    {
    STATUS    rc;

    /* %%BeginUserCode vrrpOperEntryTableHandler_Prolog */
    vrrp_rt *pVsrv = NULL;

    /* %%EndUserCode   vrrpOperEntryTablehandler_Prolog */

    rc = OK;

    switch( ccd->localId )
    {
    default:
        rc = ccdErrorNoSuchInstance();
        break;

    /* ================================================================= */
    case vrrpOperVirtualMacAddr:    /* MacAddress, read-only */
        {
        /* %%BeginUserCode vrrpOperVirtualMacAddr_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ccd->valueLen = 6;
                memcpy(ccd->pValueBuf, pVsrv->vhwaddr, ccd->valueLen);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpOperVirtualMacAddr_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperState:    /* INTEGER, read-only */
        {
        /* %%BeginUserCode vrrpOperState_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->state);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpOperState_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperAdminState:    /* INTEGER, read-create */
        {
        /* %%BeginUserCode vrrpOperAdminState_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->adminState);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( (ulongGet(ccd) < 1) || (ulongGet(ccd) > 2) )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpOperEntryTableHelper(CCD_GET, ccd,&pVsrv);
            if (OK != rc)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            if (ulongGet(ccd) == pVsrv->adminState)
            {
                break;
            }
            switch (ulongGet(ccd))
            {
            case EvrrpOperAdminState_down:
                pVsrv->adminState = EvrrpOperAdminState_down;
                pVsrv->state = EvrrpOperState_initialize;
                break;
            case EvrrpOperAdminState_up:
                if (pVsrv->rowStatus != EvrrpOperRowStatus_active)
                {
                    rc = ccdErrorRequestMap(request);
                    break;
                }
                pVsrv->adminState = EvrrpOperAdminState_up;
                break;    
            }
            break;
        }

        /* %%EndUserCode   vrrpOperAdminState_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperPriority:    /* INTEGER, read-create */
        {
        /* %%BeginUserCode vrrpOperPriority_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->priority);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( (ulongGet(ccd) < 1) || (ulongGet(ccd) > 254) )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpOperEntryTableHelper(CCD_GET, ccd,&pVsrv);
            if (OK != rc)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            if (pVsrv->adminState != EvrrpOperAdminState_down ||
                            pVsrv->rowStatus == EvrrpOperRowStatus_active)
            {
                rc = ccdErrorRequestMap(request);
                break;
            }
            pVsrv->priority = pVsrv->oldpriority = ulongGet(ccd);
            break;
        }

        /* %%EndUserCode   vrrpOperPriority_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperIpAddrCount:    /* INTEGER, read-only */
        {
        /* %%BeginUserCode vrrpOperIpAddrCount_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->naddr);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpOperIpAddrCount_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperMasterIpAddr:    /* IpAddress, read-only */
        {
        /* %%BeginUserCode vrrpOperMasterIpAddr_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                miscIpAddr2OctetString(pVsrv->vif.ipaddrM, octetGet(ccd));
                ccd->valueLen = 4;
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpOperMasterIpAddr_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperPrimaryIpAddr:    /* IpAddress, read-create */
        {
        /* %%BeginUserCode vrrpOperPrimaryIpAddr_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                miscIpAddr2OctetString(pVsrv->vif.ipaddrP, octetGet(ccd));
                ccd->valueLen = 4;
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpOperEntryTableHelper(CCD_GET, ccd,&pVsrv);
            if (OK != rc)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            if (pVsrv->adminState != EvrrpOperAdminState_down ||
                            pVsrv->rowStatus == EvrrpOperRowStatus_active)
            {
                rc = ccdErrorRequestMap(request);
                break;
            }
            pVsrv->vif.ipaddrP = miscOctetString2IpAddr(octetGet(ccd));
            break;
        }

        /* %%EndUserCode   vrrpOperPrimaryIpAddr_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperAuthType:    /* INTEGER, read-create */
        {
        /* %%BeginUserCode vrrpOperAuthType_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, EvrrpOperAuthType_noAuthentication);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( ulongGet(ccd) != EvrrpOperAuthType_noAuthentication )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpOperEntryTableHelper(CCD_GET, ccd,&pVsrv);
            if (OK != rc)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            if (pVsrv->adminState != EvrrpOperAdminState_down ||
                            pVsrv->rowStatus == EvrrpOperRowStatus_active)
            {
                rc = ccdErrorRequestMap(request);
                break;
            }
            break;
        }

        /* %%EndUserCode   vrrpOperAuthType_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperAuthKey:    /* OCTET STRING, read-create */
        {
        /* %%BeginUserCode vrrpOperAuthKey_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ccd->valueLen = 0;
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( ccd->valueLen > 0 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpOperEntryTableHelper(CCD_GET, ccd,&pVsrv);
            if (OK != rc)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            if (pVsrv->adminState != EvrrpOperAdminState_down ||
                            pVsrv->rowStatus == EvrrpOperRowStatus_active)
            {
                rc = ccdErrorRequestMap(request);
                break;
            }
            break;
        }

        /* %%EndUserCode   vrrpOperAuthKey_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperAdvertisementInterval:    /* INTEGER, read-create */
        {
        /* %%BeginUserCode vrrpOperAdvertisementInterval_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->adver_int / VRRP_TIMER_HZ);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( (ulongGet(ccd) < 1) || (ulongGet(ccd) > 255) )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpOperEntryTableHelper(CCD_GET, ccd,&pVsrv);
            if (OK != rc)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            if (pVsrv->adminState != EvrrpOperAdminState_down ||
                            pVsrv->rowStatus == EvrrpOperRowStatus_active)
            {
                rc = ccdErrorRequestMap(request);
                break;
            }
            pVsrv->adver_int = ulongGet(ccd) * VRRP_TIMER_HZ;
            break;
        }

        /* %%EndUserCode   vrrpOperAdvertisementInterval_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperPreemptMode:    /* INTEGER, read-create */
        {
        /* %%BeginUserCode vrrpOperPreemptMode_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->preempt ? EvrrpOperPreemptMode_true : EvrrpOperPreemptMode_false);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( (ulongGet(ccd) < 1) || (ulongGet(ccd) > 2) )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpOperEntryTableHelper(CCD_GET, ccd,&pVsrv);
            if (OK != rc)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            if (pVsrv->adminState != EvrrpOperAdminState_down ||
                            pVsrv->rowStatus == EvrrpOperRowStatus_active)
            {
                rc = ccdErrorRequestMap(request);
                break;
            }
            pVsrv->preempt = (ulongGet(ccd) == EvrrpOperPreemptMode_true) ? 1 : 0;
            break;
        }

        /* %%EndUserCode   vrrpOperPreemptMode_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperVirtualRouterUpTime:    /* TimeTicks, read-only */
        {
        /* %%BeginUserCode vrrpOperVirtualRouterUpTime_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulong_t ulSecs, ulTicks;

                vosUpTime(&ulSecs, &ulTicks);
                ulongSet(ccd, ulSecs - pVsrv->upTime);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpOperVirtualRouterUpTime_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperProtocol:    /* INTEGER, read-create */
        {
        /* %%BeginUserCode vrrpOperProtocol_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, EvrrpOperProtocol_ip);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( ulongGet(ccd) != EvrrpOperProtocol_ip )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpOperEntryTableHelper(CCD_GET, ccd,&pVsrv);
            if (OK != rc)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            if (pVsrv->adminState != EvrrpOperAdminState_down ||
                            pVsrv->rowStatus == EvrrpOperRowStatus_active)
            {
                rc = ccdErrorRequestMap(request);
                break;
            }
            break;
        }

        /* %%EndUserCode   vrrpOperProtocol_Body */
        }
        break;

    /* ================================================================= */
    case vrrpOperRowStatus:    /* INTEGER, read-create */
        {
        /* %%BeginUserCode vrrpOperRowStatus_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->rowStatus);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( (ulongGet(ccd) < 1) || (ulongGet(ccd) > 6) )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpOperEntryTableHelper(CCD_GET, ccd,&pVsrv);
            if (OK != rc && ulongGet(ccd) != EvrrpOperRowStatus_createAndGo &&
                            ulongGet(ccd) != EvrrpOperRowStatus_createAndWait)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            if (OK != rc)
            {
                /* 创建新的表项 */
                rc = Vrrp_VsrvConfig(ccd->pIndex[0], ccd->pIndex[1], 0, 1);
                if (OK != rc)
                {
                    rc = ccdErrorRequestMap(request);
                }
                break;
            }
            if (ulongGet(ccd) == pVsrv->rowStatus)
            {
                break;
            }
            if (EvrrpOperRowStatus_active == pVsrv->rowStatus &&
                            !(pVsrv->adminState == EvrrpOperAdminState_down &&
                            pVsrv->state == EvrrpOperState_initialize))
            {
                rc = ccdErrorRequestMap(request);
                break;
            }
            switch (ulongGet(ccd))
            {
            case EvrrpOperRowStatus_active:
                if (pVsrv->naddr < 1)
                {
                    rc = ccdErrorRequestMap(request);
                    break;
                }
                pVsrv->rowStatus = EvrrpOperRowStatus_active;
                break;
            case EvrrpOperRowStatus_notReady:
                pVsrv->rowStatus = EvrrpOperRowStatus_notReady;
                break;
            case EvrrpOperRowStatus_destroy:
                rc = Vrrp_VsrvConfig(ccd->pIndex[0], ccd->pIndex[1], 0, 3);
                if (OK != rc)
                {
                    rc = ccdErrorRequestMap(request);
                }
                break;
            default:
                break;
            }
            break;
        }

        /* %%EndUserCode   vrrpOperRowStatus_Body */
        }
        break;

    }

    /* %%BeginUserCode vrrpOperEntryTable_Epilog */
    /* %%EndUserCode   vrrpOperEntryTable_Epilog */

    return( rc );
    }

/******************************************************************************
*
* vrrpAssoIpAddrEntryTableHandler - handles table objects.
*
* This routine handles all objects for this table.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS vrrpAssoIpAddrEntryTableHandler
    (
    ccdRequest_t request,
    ccd_t * ccd
    )
    {
    STATUS    rc;

    /* %%BeginUserCode vrrpAssoIpAddrEntryTableHandler_Prolog */
    vrrp_rt *pVsrv = NULL;
    int pos = -1;
    ulong_t ipaddr = 0;

    /* %%EndUserCode   vrrpAssoIpAddrEntryTablehandler_Prolog */

    rc = OK;

    switch( ccd->localId )
    {
    default:
        rc = ccdErrorNoSuchInstance();
        break;

    /* ================================================================= */
    case vrrpAssoIpAddrRowStatus:    /* INTEGER, read-create */
        {
        /* %%BeginUserCode vrrpAssoIpAddrRowStatus_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpAssoIpAddrEntryTableHelper(request, ccd, &pVsrv, &pos);
            if ( rc == OK )
            {
                ulongSet(ccd, EvrrpAssoIpAddrRowStatus_active);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            if ( (ulongGet(ccd) < 1) || (ulongGet(ccd) > 6) )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpAssoIpAddrEntryTableHelper(CCD_GET, ccd, &pVsrv, &pos);
            if (OK != rc && ulongGet(ccd) != EvrrpOperRowStatus_active &&
                            ulongGet(ccd) != EvrrpOperRowStatus_createAndGo)
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            ipaddr = miscIpIndex2IpAddr(ccd->pIndex + 2);
            if (OK != rc)
            {
                ccd->indexSubIdCount = 2;
                rc = vrrpOperEntryTableHelper(CCD_GET, ccd, &pVsrv);
                ccd->indexSubIdCount = ccd->indexSubIdCountMax;
                if (OK != rc)
                {
                    rc = ccdErrorRequestMap(request);
                    break;
                }
                /* 创建新的表项 */
                rc = Vrrp_VsrvConfig(ccd->pIndex[0], ccd->pIndex[1], ipaddr, 1);
                if (OK != rc)
                {
                    rc = ccdErrorRequestMap(request);
                }
                break;
            }
            if (ulongGet(ccd) == EvrrpAssoIpAddrRowStatus_active &&
                            ulongGet(ccd) == EvrrpAssoIpAddrRowStatus_createAndGo)
            {
                break;
            }
            if (ulongGet(ccd) != EvrrpAssoIpAddrRowStatus_destroy &&
                            ulongGet(ccd) != EvrrpAssoIpAddrRowStatus_notInService)
            {
                rc = ccdErrorRequestMap(request);
                break;
            }
            /* 删除表项 */
            rc = Vrrp_VsrvConfig(ccd->pIndex[0], ccd->pIndex[1], ipaddr, 2);
            if (OK != rc)
            {
                rc = ccdErrorRequestMap(request);
            }
            break;
        }

        /* %%EndUserCode   vrrpAssoIpAddrRowStatus_Body */
        }
        break;

    }

    /* %%BeginUserCode vrrpAssoIpAddrEntryTable_Epilog */
    /* %%EndUserCode   vrrpAssoIpAddrEntryTable_Epilog */

    return( rc );
    }

/******************************************************************************
*
* vrrpTrackEntryTableHandler - handles table objects.
*
* This routine handles all objects for this table.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS vrrpTrackEntryTableHandler
    (
    ccdRequest_t request,
    ccd_t * ccd
    )
    {
    STATUS    rc;

    /* %%BeginUserCode vrrpTrackEntryTableHandler_Prolog */
    vrrp_rt *pVsrv = NULL;
    int pos = -1;
    ulong_t trackifindex = 0;
    int i;

    /* %%EndUserCode   vrrpTrackEntryTablehandler_Prolog */

    rc = OK;

    switch( ccd->localId )
    {
    default:
        rc = ccdErrorNoSuchInstance();
        break;

    /* ================================================================= */
    case vrrpTrackifIndex:    /* INTEGER, read-only */
        {
        /* %%BeginUserCode vrrpTrackifIndex_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpTrackEntryTableHelper(request, ccd, &pVsrv, &pos);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->iftrack[pos]);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }
            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpTrackifIndex_Body */
        }
        break;

    /* ================================================================= */
    case vrrpTrackPriReduced:    /* INTEGER, read-write */
        {
        /* %%BeginUserCode vrrpTrackPriReduced_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpTrackEntryTableHelper(request, ccd, &pVsrv, &pos);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->pritrack[pos]);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }
            break;

        case CCD_VALIDATE:
            if ( (ulongGet(ccd) < 1) || (ulongGet(ccd) > 254) )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpTrackEntryTableHelper(CCD_GET, ccd, &pVsrv, &pos);
            if ( rc == OK )
            {
                pVsrv->pritrack[pos] = ulongGet(ccd);
            }
            else
            {
                rc = ccdErrorNoSuchInstance();
            }
            break;
        }

        /* %%EndUserCode   vrrpTrackPriReduced_Body */
        }
        break;

    /* ================================================================= */
    case vrrpTrackRowStatus:    /* INTEGER, read-create */
        {
        /* %%BeginUserCode vrrpTrackRowStatus_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpTrackEntryTableHelper(request, ccd, &pVsrv, &pos);
            if ( rc == OK )
            {
                ulongSet(ccd, EvrrpTrackRowStatus_active);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }
            break;

        case CCD_VALIDATE:
            if ( (ulongGet(ccd) < 1) || (ulongGet(ccd) > 6) )
                rc = ccdErrorInvalidRange();
            else if ( ccd->valueLen > 4 )
                rc = ccdErrorInvalidLength();
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            rc = vrrpTrackEntryTableHelper(CCD_GET, ccd, &pVsrv, &pos);
            if ( (OK != rc) && ulongGet(ccd) != EvrrpOperRowStatus_active )
            {
                rc = ccdErrorNoSuchInstance();
                break;
            }
            trackifindex = ccd->pIndex[2];
            if ( (OK != rc) && (EvrrpAssoIpAddrRowStatus_active == ulongGet(ccd)) )
            {
                ccd->indexSubIdCount = 2;
                rc = vrrpOperEntryTableHelper(CCD_GET, ccd, &pVsrv);
                ccd->indexSubIdCount = ccd->indexSubIdCountMax;
                if (OK != rc)
                {
                    rc = ccdErrorRequestMap(request);
                    break;
                }
                /* 创建新的表项 */
            #ifdef GLN_INCLUDE_TRACK
                if ( (pVsrv->niftrack >= VRRP_IF_TRACK_MAX) || (trackifindex < 1)
                    || (trackifindex > TRACK_ENTRY_NUM) )
            #else
                if (pVsrv->niftrack >= VRRP_IF_TRACK_MAX || 0 == ifindex_to_vlanid(trackifindex))
            #endif
                {
                    rc = ccdErrorRequestMap(request);
                }
                else
                {
                #ifdef GLN_INCLUDE_TRACK
                    if ( OK != trackEntryAppReg(trackifindex, "vrrp", VRRP_PROCESS_NAME, 
                        VRRP_FUNC_TRACK_STATE_CHG) )
                    {
                        rc = ccdErrorRequestMap(request);
                        break;
                    }
                #endif
                
                    pVsrv->iftrack[pVsrv->niftrack] = trackifindex;
                    pVsrv->pritrack[pVsrv->niftrack] = VRRP_PRI_TRACK;
                #ifdef GLN_INCLUDE_TRACK
                    pVsrv->trackState[pVsrv->niftrack] = TRACK_STATE_INVALID;
                #endif
                    pVsrv->niftrack++;
                }
                break;
            }
            else if ( (OK == rc) && (EvrrpAssoIpAddrRowStatus_destroy == ulongGet(ccd)) )
            {
                /* 删除表项 */
            #ifdef GLN_INCLUDE_TRACK
                trackEntryAppDeReg(trackifindex, "vrrp");
            #endif
                
                for (i = pos; i < pVsrv->niftrack - 1; i++)
                {
                    pVsrv->iftrack[i] = pVsrv->iftrack[i+1];
                    pVsrv->pritrack[i] = pVsrv->pritrack[i + 1];
                #ifdef GLN_INCLUDE_TRACK
                    pVsrv->trackState[i] = pVsrv->trackState[i + 1];
                #endif
                }
                pVsrv->niftrack--;
            }
            break;
        }

        /* %%EndUserCode   vrrpTrackRowStatus_Body */
        }
        break;

    }

    /* %%BeginUserCode vrrpTrackEntryTable_Epilog */
    /* %%EndUserCode   vrrpTrackEntryTable_Epilog */

    return( rc );
    }

/******************************************************************************
*
* vrrpRouterStatsEntryTableHandler - handles table objects.
*
* This routine handles all objects for this table.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS vrrpRouterStatsEntryTableHandler
    (
    ccdRequest_t request,
    ccd_t * ccd
    )
    {
    STATUS    rc;

    /* %%BeginUserCode vrrpRouterStatsEntryTableHandler_Prolog */
    vrrp_rt *pVsrv = NULL;

    /* %%EndUserCode   vrrpRouterStatsEntryTablehandler_Prolog */

    rc = OK;

    switch( ccd->localId )
    {
    default:
        rc = ccdErrorNoSuchInstance();
        break;

    /* ================================================================= */
    case vrrpStatsBecomeMaster:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsBecomeMaster_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staBecomeMaster);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsBecomeMaster_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsAdvertiseRcvd:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsAdvertiseRcvd_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staAdverRcvd);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsAdvertiseRcvd_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsAdvertiseIntervalErrors:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsAdvertiseIntervalErrors_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staAdverIntErrors);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsAdvertiseIntervalErrors_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsAuthFailures:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsAuthFailures_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staAuthFailures);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsAuthFailures_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsIpTtlErrors:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsIpTtlErrors_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staIpTtlErrors);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsIpTtlErrors_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsPriorityZeroPktsRcvd:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsPriorityZeroPktsRcvd_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staPriZeroPktsRcvd);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsPriorityZeroPktsRcvd_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsPriorityZeroPktsSent:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsPriorityZeroPktsSent_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staPriZeroPktsSent);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsPriorityZeroPktsSent_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsInvalidTypePktsRcvd:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsInvalidTypePktsRcvd_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staInvTypePktsRcvd);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsInvalidTypePktsRcvd_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsAddressListErrors:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsAddressListErrors_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staAddrListErrors);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsAddressListErrors_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsInvalidAuthType:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsInvalidAuthType_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staInvAuthType);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsInvalidAuthType_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsAuthTypeMismatch:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsAuthTypeMismatch_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staAuthTypeMismatch);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsAuthTypeMismatch_Body */
        }
        break;

    /* ================================================================= */
    case vrrpStatsPacketLengthErrors:    /* Counter32, read-only */
        {
        /* %%BeginUserCode vrrpStatsPacketLengthErrors_Body */

        switch ( request )
        {
        case CCD_GET_NEXT:
        case CCD_GET:
            rc = vrrpOperEntryTableHelper(request, ccd,&pVsrv);
            if ( rc == OK )
            {
                ulongSet(ccd, pVsrv->staPktsLenErrors);
            }
            else
            {
                rc = ccdErrorRequestMap(request);
            }

            break;

        case CCD_VALIDATE:
            break;

        case CCD_SET_NVM:
            break;

        case CCD_COMMIT:
            break;
        }

        /* %%EndUserCode   vrrpStatsPacketLengthErrors_Body */
        }
        break;

    }

    /* %%BeginUserCode vrrpRouterStatsEntryTable_Epilog */
    /* %%EndUserCode   vrrpRouterStatsEntryTable_Epilog */

    return( rc );
    }
    
#endif /* GBN_INCLUDE_VRRP */

