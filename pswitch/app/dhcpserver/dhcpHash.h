/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  FileName:  DhcpHash.c
  Autor: Kniazkov Andrew
  Version:1.0.0        
  Date:2024/04/09
  Description: This file defines hash functions, hash allocation functions,
               hash release functions, and hash comparison functions used
               inside the DHCP module
  Other notes: None
********************************************************************************/
#ifndef _DHCP_HASH_H
#define	_DHCP_HASH_H

#include <libipc/vos.h>

#ifdef __cplusplus
extern "C"{
#endif

/*define return value*/
#ifndef OK
//#define OK 0
#define OK SYS_ERR_OK
#endif
#ifndef ERROR
//#define ERROR -1
#define ERROR SYS_ERR_FAILED
#endif


/*Supports a total of HASHFACTOR+HASHCOLLISIONLEN nodes*/
#define HASH_FACTOR  7001//2051    /*number of nodes in the hash area*/
#define HASH_COLLISIONLEN 1750//512    /*number of nodes in the collision area*/
#define HOSTNAME_LEN       16     /*record up to the first 15 characters of the computer name, consistent with NetBIOS*/
#define HOSTNAME_COPYLEN  (HOSTNAME_LEN-1)
#define record_hostname(dst,src,len) \
do{\
    memset((dst), 0, HOSTNAME_LEN);\
    if((src))\
        memcpy((dst), (src), ((len)<HOSTNAME_COPYLEN?(len):HOSTNAME_COPYLEN));\
}while(0)

#if 0
 /*�����û���Ϣ��HASH�ṹ*/
 typedef struct tagMACTOVLAN_HASHDATA
 {
	 uchar_t  ucUserMac[6]; /* �û�������MAC��ַ */
	 ushort_t usVlanId; 	  /* �û���Vlan ID  */
	 ushort_t usPort;		  /* �û������ڶ˿ں� */
	 ulong_t  ulIpAddr; 	  /*DHCP Server������û���IP��ַ*/
	 char	  hostname[HOSTNAME_LEN];
	 BOOL     bindFlag;
 }MACHASH_S;
#endif
 void  *hash_malloc_data(void *pId);
 void  hash_free_data(void *pData);
 ulong_t  hash_key(void *pId);
 ulong_t  hash_compare_key(void *pData,void *pId);


#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif   /* _DHCP_HASH_H */


