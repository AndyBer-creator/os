/* rfc2787.h - CCD+ Module include for rfc2787.mi2. */

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
This module defines the CCD+ Module Local ID enumerations, the MIB object
enumerations, the MIB object sizes and some CCD+ Module function prototypes.

INCLUDE FILES: N/A
*/

#ifndef __INCrfc2787h
#define __INCrfc2787h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Defines */
/* %%BeginUserCode rfc2787_Defs */
/* %%EndUserCode   rfc2787_Defs */

/*****************************************************************************
* CCD+ Module Local ID enumerations
*/
enum {
    vrrpNodeVersion = 0,
    vrrpNotificationCntl,
    vrrpOperVrId,
    vrrpOperVirtualMacAddr,
    vrrpOperState,
    vrrpOperAdminState,
    vrrpOperPriority,
    vrrpOperIpAddrCount,
    vrrpOperMasterIpAddr,
    vrrpOperPrimaryIpAddr,
    vrrpOperAuthType,
    vrrpOperAuthKey,
    vrrpOperAdvertisementInterval,
    vrrpOperPreemptMode,
    vrrpOperVirtualRouterUpTime,
    vrrpOperProtocol,
    vrrpOperRowStatus,
    vrrpAssoIpAddr,
    vrrpAssoIpAddrRowStatus,
    vrrpTrapPacketSrc,
    vrrpTrapAuthErrorType,
    vrrpTrackifIndex,
    vrrpTrackPriReduced,
    vrrpTrackRowStatus,
    vrrpRouterChecksumErrors,
    vrrpRouterVersionErrors,
    vrrpRouterVrIdErrors,
    vrrpStatsBecomeMaster,
    vrrpStatsAdvertiseRcvd,
    vrrpStatsAdvertiseIntervalErrors,
    vrrpStatsAuthFailures,
    vrrpStatsIpTtlErrors,
    vrrpStatsPriorityZeroPktsRcvd,
    vrrpStatsPriorityZeroPktsSent,
    vrrpStatsInvalidTypePktsRcvd,
    vrrpStatsAddressListErrors,
    vrrpStatsInvalidAuthType,
    vrrpStatsAuthTypeMismatch,
    vrrpStatsPacketLengthErrors,
    rfc2787MaxLocalIds  /* 39 - Number of objects in rfc2787 */
};

/****************************************************************************
* CCD+ Module MIB object enumerations
*/
enum {
    EvrrpNotificationCntl_enabled = 1,
    EvrrpNotificationCntl_disabled = 2,
    EvrrpOperState_initialize = 1,
    EvrrpOperState_backup = 2,
    EvrrpOperState_master = 3,
    EvrrpOperAdminState_up = 1,
    EvrrpOperAdminState_down = 2,
    EvrrpOperAuthType_noAuthentication = 1,
    EvrrpOperAuthType_simpleTextPassword = 2,
    EvrrpOperAuthType_ipAuthenticationHeader = 3,
    EvrrpOperPreemptMode_true = 1,
    EvrrpOperPreemptMode_false = 2,
    EvrrpOperProtocol_ip = 1,
    EvrrpOperProtocol_bridge = 2,
    EvrrpOperProtocol_decnet = 3,
    EvrrpOperProtocol_other = 4,
    EvrrpOperRowStatus_active = 1,
    EvrrpOperRowStatus_notInService = 2,
    EvrrpOperRowStatus_notReady = 3,
    EvrrpOperRowStatus_createAndGo = 4,
    EvrrpOperRowStatus_createAndWait = 5,
    EvrrpOperRowStatus_destroy = 6,
    EvrrpAssoIpAddrRowStatus_active = 1,
    EvrrpAssoIpAddrRowStatus_notInService = 2,
    EvrrpAssoIpAddrRowStatus_notReady = 3,
    EvrrpAssoIpAddrRowStatus_createAndGo = 4,
    EvrrpAssoIpAddrRowStatus_createAndWait = 5,
    EvrrpAssoIpAddrRowStatus_destroy = 6,
    EvrrpTrapAuthErrorType_invalidAuthType = 1,
    EvrrpTrapAuthErrorType_authTypeMismatch = 2,
    EvrrpTrapAuthErrorType_authFailure = 3,
    EvrrpTrackRowStatus_active = 1,
    EvrrpTrackRowStatus_notInService = 2,
    EvrrpTrackRowStatus_notReady = 3,
    EvrrpTrackRowStatus_createAndGo = 4,
    EvrrpTrackRowStatus_createAndWait = 5,
    EvrrpTrackRowStatus_destroy = 6
};
/*****************************************************************************
* CCD+ Module MIB object sizes
*/
enum {
    vrrpNodeVersion_sz = 4,
    vrrpNotificationCntl_sz = 4,
    vrrpOperVrId_sz = 4,
    vrrpOperVirtualMacAddr_sz = 6,
    vrrpOperState_sz = 4,
    vrrpOperAdminState_sz = 4,
    vrrpOperPriority_sz = 4,
    vrrpOperIpAddrCount_sz = 4,
    vrrpOperMasterIpAddr_sz = 4,
    vrrpOperPrimaryIpAddr_sz = 4,
    vrrpOperAuthType_sz = 4,
    vrrpOperAuthKey_sz = 255,
    vrrpOperAdvertisementInterval_sz = 4,
    vrrpOperPreemptMode_sz = 4,
    vrrpOperVirtualRouterUpTime_sz = 4,
    vrrpOperProtocol_sz = 4,
    vrrpOperRowStatus_sz = 4,
    vrrpAssoIpAddr_sz = 4,
    vrrpAssoIpAddrRowStatus_sz = 4,
    vrrpTrapPacketSrc_sz = 4,
    vrrpTrapAuthErrorType_sz = 4,
    vrrpTrackifIndex_sz = 4,
    vrrpTrackPriReduced_sz = 4,
    vrrpTrackRowStatus_sz = 4,
    vrrpRouterChecksumErrors_sz = 4,
    vrrpRouterVersionErrors_sz = 4,
    vrrpRouterVrIdErrors_sz = 4,
    vrrpStatsBecomeMaster_sz = 4,
    vrrpStatsAdvertiseRcvd_sz = 4,
    vrrpStatsAdvertiseIntervalErrors_sz = 4,
    vrrpStatsAuthFailures_sz = 4,
    vrrpStatsIpTtlErrors_sz = 4,
    vrrpStatsPriorityZeroPktsRcvd_sz = 4,
    vrrpStatsPriorityZeroPktsSent_sz = 4,
    vrrpStatsInvalidTypePktsRcvd_sz = 4,
    vrrpStatsAddressListErrors_sz = 4,
    vrrpStatsInvalidAuthType_sz = 4,
    vrrpStatsAuthTypeMismatch_sz = 4,
    vrrpStatsPacketLengthErrors_sz = 4
};

/* %%BeginUserCode rfc2787_Enums */
/* %%EndUserCode   rfc2787_Enums */

/* Structures */
/* %%BeginUserCode rfc2787_Structs */
/* %%EndUserCode   rfc2787_Structs */

/* Prototypes */
/* %%BeginUserCode rfc2787_Protos */
/* %%EndUserCode   rfc2787_Protos */

/****************************************************************************
* CCD+ Module Function Prototypes
*/

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __INCrfc2787h */
