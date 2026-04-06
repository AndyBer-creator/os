/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  File Name:  ip_pool.h
  Description: This file defines the data structure of the address pool management module.
  Autor: Kniazkov Andrew
  Other Notes: None.
********************************************************************************/
#ifndef _IP_POOL_H_
#define _IP_POOL_H_
#include "dhcps.h"
#ifdef      __cplusplus
extern      C{
#endif



#define  IPPOOL_MAX_POOL_NUM      (128)
#define  IPPOOL_MAX_IPSection_NUM   (8)
#define  IPPOOL_MAX_POOL_NAME       (32 + 1)     /* IP address pool name */
#define  IPPOOL_MAX_IPPerSection_NUM      1024//(512)
#define  IPPOOL_SECTION_MAX_DISABLEIP_NUM  (IPPOOL_MAX_IPPerSection_NUM)/*Maximum number of forbidden addresses per segment*/
#define  IPPOOL_ALL_MAX_DISABLEIP_NUM      (20000)
#define  IPPOOL_MAX_TOTAL_SECTION_NUM   (IPPOOL_MAX_POOL_NUM*2)
#define  IPPOOL_MAX_TOTAL_IPADDR_NUM   (IPPOOL_MAX_TOTAL_SECTION_NUM*IPPOOL_MAX_IPPerSection_NUM)
#define  SC_MIN_MAXBUFFER_ADDRNUM (IPPOOL_MAX_IPPerSection_NUM/ 2)
#define  SC_MAX_IPPOOL_BITMAP   (IPPOOL_MAX_IPPerSection_NUM/4)
#define  SC_BITMAP_UNIT         (8)     //Bitmap uses ULONG as STEP, a ulong contains 8 4-bit
#define  SC_MIN_IPPOOL_ALLOCINDEX   (0) //The minimum sequence number of the bitmap is 0
#define  SC_IP_POOL_BITMAP_STEP     (4) //Check a ulong each time
#define  IPPOOL_BITMAP_LITTLE_STEP  (4) //Each offset is 4BIT
#define  SC_IP_POOL_BIT_COUNTS_IN_ULONG (32) //The number of bits in each ULONG
#define  IPPOOL_MAX_DNS_SUFFIX_NAME    (32+1) /*Maximum length of the ip pool domain name suffix*/
#define  IPPOOL_MAX_DNS_NUM       4        /* Maximum number of domain name servers in ip pool*/

#define IPPOOL_CONFLICT_PERIOD (86400)
#define IPPOOL_IPSTATUS_CONFLICT (0x9)   //1001 If the address is valid but already in use, the bitmap is set to this value
#define IPPOOL_IPSTATUS_INVALID (0xA)   //1010 If the address check fails, the bitmap corresponding to this address is set to illegal
#define IPPOOL_IPSTATUS_DISABLE (0xC)   //1100 address is prohibited
#define IPPOOL_IPSTATUS_USED    (0x8)   //1000 If a bitmap is allocated, it is implicitly set to be in use
#define IPPOOL_IPSTATUS_UNUSED  (0x0)   //0000 When initialized, all bitmap states are not used
#define SC_IP_BITMAP_MASK (0XF)   //Mask used for 4BIT operations
#define SC_IP_ULONG_MASK (0x88888888)//Used to operate on a ulong in the bitmap
#define IPPOOL_NULL_USHORT (0xffff)
#define IPPOOL_CHAR_NULL    (0xff)
#define SC_IPPOOL_CYCLE_TIME  (30)	//second
#define SC_IPPOOL_MIN_DAY (0)    		//Minimum number of days in the address lease period
#define SC_IPPOOL_MAX_DAY (999)  		//The maximum number of days in the address lease period
#define SC_IPPOOL_USED_UNUSED_SCAPE (50)//The ratio limit between used addresses and unused addresses, add a percentage after the use
#define SC_IPPOOL_CONFLICT_TIMER (1)//SECOND
#define SC_IPPOOL_CONFLICT_TIMERALL  (300)  //second
#define SC_IPPOOL_MAX_LEASESECOND (SC_IPPOOL_MAX_DAY * 24 * 3600)
#define SC_IPPOOL_MAX_LEASEMINUTE (SC_IPPOOL_MAX_DAY * 24 * 60)
#define IPPOOL_MAX_DHCPOPTIONS 3
#define IPPOOL_DEFAULT_SUBNET_MASK  0xFFFF0000 //Default mask of the address pool
#define IPPOOL_DEFAULT_LEASETIME (1*24*60*60)//second test 5 min used 1 day

#define ROW_DESTROY 6
#define ROW_ACTIVE    1

typedef enum tagCreateSectionError
{
    ERROR_SECTION_STARTIP_INVALID = 1,
    ERROR_SECTION_ENDIP_INVALID,
    ERROR_SECTION_STARTIP_HIGHER,
    ERROR_SECTION_LENGTH_TOOBIG,
    ERROR_SECTION_INDEX_TOOBIG,
    ERROR_SECTION_NO_GATEWAY,
    ERROR_SECTION_IPGATE_NOSAMESUBNET,
    ERROR_SECTION_OVERLAP,
    ERROR_SECTION_NUM_FULL
} CreateSectionError;

typedef struct tagBitmap
{
    ushort_t    bIsAlloc;
    ulong_t*pbBitmapMem;
    ushort_t   wBitmapSize;
    ushort_t   wMinAllocNo;
    ushort_t   wMaxAllocNo;
    ushort_t     wFreeNum;
} IpSectionBitMap_S;

//Free list node structure
typedef struct tagIdleLinkNode
{
    ushort_t  usNext;  //Backward Index
    ushort_t  usPre;   //Previous Item Index
    //Does the address lease period affect the release of the address?
    //If the address is AAA, the address lease period should not work.
    //If the address is requested by DHCP, use the address lease period and the enumeration variable IPPOOL_LEASE_FLAG
    //uchar_t ucLeaseFlag; Due to insufficient memory, this flag is appended to the highest bit of the lease counter
    ulong_t   ulLeaseCount;//Time counter for the address's lease
    uchar_t   flag_discover;//DHCP server processing discover message flag
}IpLinkNode_S;


//Address chain structure
typedef struct tagIpPoolLink
{
    ushort_t  usMaxLength;  //The length of the linked list
    ushort_t  usIdleHead;   //Head pointer of the free chain
    ushort_t  usIdleTail;     //Increase the tail pointer of the free list
    ushort_t  usUsedHead;   //Head pointer of the address chain in use
    ushort_t  usConflictHead;//Link list head pointer of the conflicting address
//    ushort_t  usInValidHead ;//Head pointer at illegal address
    IpLinkNode_S  *pusIpLink; //IP address subscript index free chain
}IpPoolLink_S;

//Address segment structure
typedef struct tagIPSection_S
{
    ulong_t   ulStartIp;          //Starting IP address
    ulong_t   ulEndIp;            //Terminate IP Address
    ushort_t  usLength;           //Total number of IP addresses
    ulong_t   ulSectionRowStatus;
    ulong_t   ulInUsed;           //Valid leased IP addresses (if greater than 0, it means it is in use and cannot be modified
    // or deleted)
    ulong_t  ulUnUsed;           //Legal free addresses
    ulong_t  ulConflict;         //The number of conflicts not yet used
    ulong_t  ulInvalid;          //Invalid number of addresses
    ulong_t  ulDisable;          //Prohibited addresses
    IpSectionBitMap_S Bitmap;   //Placeholder map of IP addresses in the pool 
    //(4 bits each indicating the usage of an IP: unused, in effective use, conflicting and not in use, illegal IP address, etc.)
    IpPoolLink_S stIpLink;      //Free list (0052 should be the address chain, not necessarily the free chain)
}AAA_IPSection_S;

/*Disable address structure*/
typedef struct tagIPDisable_S
{
    ulong_t  ulDisableIp;          /*Prohibited addresses */
    ulong_t  ulPoolIndex;
    ulong_t  ulDisableRowStatus;
}IPPOOL_IPDisable_S;

#define IP_POOL_MSG_NUM 35//ip pool Address pool statistics and error message count

typedef struct tagIpPoolSaveStatistic{
	//No free addresses
	ulong_t ulNoFreeIp;
	//To release an address that is not in the address pool
	ulong_t ulIpNotInPool;
	//The address to be freed is not in use
	ulong_t ulIpNotInused;
	//The requested address cannot be allocated
	ulong_t ulIpNotAlloc;
	//Bitmap Error
	ulong_t ulBitMapError;
}IpPoolSaveStatistic;



typedef enum IpPoolMsgNumToName
{
    IpPool_request_alloc_Ip = 0,     //Count of application addresses
    IpPool_Alloc_success,        //Gives a count of ip addresses
    IpPool_Alloc_fail,           //No address count is given
    // The following are the reasons why address allocation fails
    //There is no address in the address pool
    IpPool_NO_FREE_IP,
    IpPool_SECTION_NO_FREE_IP,
    IpPool_Section_OutOfRange,
    //The requested address is not in the address pool
    IpPool_Alloc_NOT_IN_POOL,
    //The requested address is in use, conflicting, illegal, prohibited, or in an unknown state
    IpPool_Alloc_IN_USED,
    IpPool_Alloc_IN_CONFLICT,
    IpPool_Alloc_IN_INVALID,
    IpPool_Alloc_IN_DISABLE,
    IpPool_Alloc_IN_UNKNOW,
    //A bitmap error occurred during address reallocation.
    IpPool_Alloc_BITMAP_ERROR,
	IpPool_Renew_Request,
	IpPool_Renew_success,
	IpPool_Renew_used,
	IpPool_Renew_unused,
	IpPool_Renew_Fail,
	IpPool_Renew_conflict,
	IpPool_Renew_InValid,
	IpPool_Renew_disable,
	IpPool_Renew_unknown,
	IpPool_Renew_bitmap_error,
	IpPool_Renew_not_in_pool,
    IpPool_request_release_ip,   //Request count for releasing addresses
    IpPool_Timeout_release_ip,		//Release address when lease expires
	IpPool_release_ip_success,   //Release success count
    //The following are the reasons why the release address failed
	IpPool_release_ip_fail,      //Release failure count
    //The released address is not in the address pool
    IpPool_release_NOT_IN_POOL,
    //The released address is in a conflicting, illegal, prohibited, or unknown state. 
    //It is unreasonable for addresses in these states to be released.
    IpPool_release_IN_CONFLICT,
    IpPool_release_IN_INVALID,
    IpPool_release_IN_DISABLE,
    IpPool_release_IN_UNUSED,
    IpPool_release_IN_UNKNOW,
    //A bitmap error occurred during address reallocation.
    IpPool_release_BITMAP_ERROR //Terminator
}IpPoolMsgNumToName;
typedef struct tagIPPOOLStringList
{
    ulong_t ulErrorId; /* Error Code */
    uchar_t*  szErrorTextE;
    uchar_t*  szErrorTextC; /* Error message */
}IPPOOL_StringList_S ;

//Enumeration variable indicating whether the lease is effective
typedef enum tagIPPOOL_LEASE_FLAG
{
    LEASE_ABLE =0,
    LEASE_DISABLE
}IPPOOL_LEASE_FLAG;
//Address pool structure
typedef struct tagAAA_IPPool_DHCPOption
{
    ulong_t ulID;
    ulong_t ulIP;
} AAA_IPPool_DHCPOption;

#define IPPOOL_OPTION66_MAX_LEN 32      //For option 66
#define IPPOOL_OPTION67_MAX_LEN 32      //For option 67
#define IPPOOL_OPTION_FORMAT_ASCII 0
#define IPPOOL_OPTION_FORMAT_HEX   1
#define IPPOOL_OPTION_FORMAT_IPV4  2
#define IPPOOL_OPTION43_MAX_LEN 64
typedef struct IPPOOL_OPTION43_tag{
    uchar_t        ucFormat;
    uchar_t        ucValueLen;
    uchar_t        szValue[IPPOOL_OPTION43_MAX_LEN + 1];
}IPPOOL_OPTION43;

typedef struct tagAAA_IPPOOL
{
    uchar_t   szName[IPPOOL_MAX_POOL_NAME];
    ulong_t   IpPoolMsgNum[IP_POOL_MSG_NUM];
    ulong_t   ulSubnetMask;             
    ulong_t   ulGatewayIp;               
    ulong_t   ulRouterIp;                  
    ulong_t   ulDNSIp[IPPOOL_MAX_DNS_NUM];           
    uchar_t   szDnsSuffixName[IPPOOL_MAX_DNS_SUFFIX_NAME];   
  //NetBios Options
    ulong_t   ulPrimaryNBNSIp;           
    ulong_t   ulSecondNBNSIp;           

   //DHCP option
    ulong_t   ulLeaseTime;                //IP address lease period readonly
    AAA_IPPool_DHCPOption stDHCPOPT[IPPOOL_MAX_DHCPOPTIONS];

    AAA_IPSection_S   IpSection[IPPOOL_MAX_IPSection_NUM];
    IPPOOL_OPTION43  option43;
    /*option66*/
    uchar_t          tftpServer[IPPOOL_OPTION66_MAX_LEN+1];

    /*option67*/
    uchar_t          bootFile[IPPOOL_OPTION67_MAX_LEN+1];
}AAA_IPPOOL_S;
//Definition of the cause of address pool errors, used in external interfaces
typedef enum tagIpPoolError
{
    IP_SUCCESS = 0,
    FIND_NO_POOL_INDEX,
    IP_NOT_IN_POOL,
    NO_FREE_IP,
    IP_IN_USED,
    IP_IN_CONFLICT,
    IP_IN_INVALID,
    IP_IN_DISABLE,
    IP_IN_UNKNOW,
    IP_BITMAP_ERROR,
    POOL_NAME_ERROR,
    IP_GENERAL_ERROR
}IPPOOLERROR;
//The error type of the host address, as the return value of the function
typedef enum tagHostIptype
{
	HostIpType_NORMAL = 0,
	HostIpType_127,
	HostIpType_IP_MASK_NOT_MATCH,
	HostIpType_SUBNET_NET_BROAD,
	HostIpType_IP_BroadCast,
	HostIpType_IP_MultiCast
}HostIpType;
typedef enum tagCheckSectionErr
{
	Sect_Normal,
	Sect_InUsed,
	Sect_Overlap,
	Sect_Other
}CheckSectionErr;


/*declarations*/
extern STATUS ipPoolCreateIpSection(ulong_t ulIpPoolIndex,
                    ulong_t ulIpSectionIndex,
                    ulong_t ulStartIp,
                    ulong_t ulEndIp,
                    ulong_t ulMask);
extern STATUS ipPoolCheckIpAndMaskValid(ulong_t ulIpAddr,ulong_t ulMask);
extern STATUS ipPoolCheckIpValid(ulong_t ulIP);
extern int format2Stored(int format, void *pSource, void *pDestination, ULONG *valueLen);
extern STATUS ipPoolClearIpSection( AAA_IPSection_S *pIpSect );
extern STATUS ipPoolInitIpSection( AAA_IPSection_S *pIpSect );
extern STATUS ipPoolClearLeaseCount(IpPoolLink_S *pIpRefLink,ushort_t usIndex);
extern STATUS ipPoolChangeIpSectionBitMap(IpSectionBitMap_S * pRefBitmap, ushort_t wIndex,uchar_t ucStatus);
extern STATUS ipPoolSetLeaseFlag(IpPoolLink_S *pIpRefLink,ushort_t usIndex,uchar_t ucFlag);
extern STATUS ipPoolChangeSectionIpIdleLink(IpPoolLink_S *pIpLink,ushort_t usIndex,uchar_t ucFlag,
											ulong_t ucPoolIndex,ulong_t  ulIpSectIndex);

extern STATUS GetIpBitmapState ( IpSectionBitMap_S * pRefBitmap,
                                    ushort_t wIndex ,
                                    uchar_t *pStatus);

STATUS AllocIpAddrFromIpSection
(
AAA_IPSection_S *pIpSect,
ulong_t *pIpAddr,
uchar_t ucLeaseFlag,
ulong_t ulPoolIndex,
uchar_t ulSectIndex,
BOOL cantBeBound,
ulong_t vid
);


extern STATUS FindIpSectionByIp(ulong_t ulPoolIndex,ulong_t ulIpAddr,ulong_t *pulSectIndex,ushort_t *pIndex);
extern uchar_t  *ipPoolIPtoStr (ulong_t ulIPAddr, uchar_t * string);
extern STATUS FreeToIpSectionBitmap ( IpSectionBitMap_S * pRefBitmap, ushort_t wIndex );
extern STATUS ipPoolGetLeaseFlag(IpPoolLink_S *pIpRefLink,ushort_t usIndex,uchar_t *pucFlag);
extern STATUS ipPoolAddLeaseCount(IpPoolLink_S *pIpRefLink,ushort_t usIndex,uchar_t ucPool,uchar_t ucSect);
extern STATUS IpRenewLeaseTime(ulong_t ulPoolIndex, ulong_t ulSectIndex,
                                ushort_t usIndex);

/*Server interface*/
STATUS IPPOOL_GetAnyFreeIp
(
ulong_t  ulGatewayIp,
ulong_t *pstFreeIp ,
uchar_t ucLeaseFlag,
BOOL cantBeBound,
ulong_t vid
);
STATUS IPPOOL_GetSpecialIp(ulong_t  ulGatewayIp, ulong_t ulSpecialIp, BOOL cantBeBound, ulong_t vid);
STATUS ipPoolGetGatewayBySpecialIp(ulong_t ulSpecialIp, ulong_t *gateway);
extern STATUS IPPOOL_SetOpton43(ulong_t ulGatewayIp, IPPOOL_OPTION43 *pOption43);
extern STATUS IPPOOL_GetOpton43(ulong_t ulGatewayIp, IPPOOL_OPTION43 *pOption43);
extern STATUS IpPool_GetParameter(ulong_t ulGatewayIp,ulong_t *pulMask,
                                    ulong_t pulDnsIp[],
                                    ulong_t *pulPrimaryNBNSIp,
                                    ulong_t *pulSecondNBNSIp,
                                    ulong_t *pulLeaseTime,
                                    uchar_t *pszDnsSuffixName,
                                    ulong_t *pulRouterIp
                                    );
extern STATUS IpPool_GetParameter_tftp(ulong_t ulGatewayIp,uchar_t* pucTftpName,uchar_t* pucTftpBootfileName);
extern STATUS IPPOOL_SetConflictIp(ulong_t  ulGatewayIp, ulong_t ulSpecialIp );
extern STATUS IPPOOL_RenewIpTime(ulong_t  ulGatewayIp, ulong_t ulSpecialIp );
extern void IPPOOL_GetbackIp(ulong_t  ulGatewayIp, ulong_t ulSpecialIp );
extern void IPPOOL_GetbackIpWithoutGateway(ulong_t ulSpecialIp );
extern void HandIpAddrLeaseTimer(void);
void HandIpAddrConflictTimer(void);
extern STATUS InitIpPoolAll(void);
extern STATUS ipPoolChangeIpStatusInSection(ulong_t ulPoolIndex,ulong_t ulIpSectIndex,
                                    ushort_t usIndex ,uchar_t ucStatus,uchar_t ucLeaseFlag);
extern STATUS  ipPoolChangeIpStatusInPool(ulong_t ulPoolIndex,ulong_t ulIpAddr,
                                  uchar_t ucStatus,uchar_t ucLeaseFlag);

void ippool_debug_printf( char *exp, ...)
#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
;

STATUS ipPoolSetDiscoverFlag(ulong_t ulGatewayIp, ulong_t ulIp, uchar_t flag);
STATUS IPPOOL_CheckSpecialIp(ulong_t  ulGatewayIp, ulong_t ulSpecialIp );
int dhcpserverGatewayCfg(char *szPoolName, sys_ipv4_t addr, sys_ipv4_t mask);
int dhcpserverSectionCfg(char *szPoolName, uint32 id, sys_ipv4_t start_ip, sys_ipv4_t end_ip);


#ifdef      __cplusplus
}
#endif

#endif




