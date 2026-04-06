#ifndef _INCLUDE_IF_ARP_H_
#define _INCLUDE_IF_ARP_H_

#include <libipc/vos.h>

struct	arphdr_tag {
	ushort_t ar_hrd;		
#define ARPHRD_ETHER 	1	
#define ARPHRD_FRELAY 	15	
	ushort_t ar_pro;		
	uchar_t	 ar_hln;		
	uchar_t	 ar_pln;		
	ushort_t ar_op;		
#define	ARPOP_REQUEST	1	
#define	ARPOP_REPLY	2	
#define	ARPOP_REVREQUEST 3	
#define	ARPOP_REVREPLY	4	
#define	ARPOP_INVREQUEST 8 	
#define	ARPOP_INVREPLY	9	

#ifdef COMMENT_ONLY
	uchar_t	ar_sha[];	
	uchar_t	ar_spa[];	
	uchar_t	ar_tha[];	
	uchar_t	ar_tpa[];	
#endif
};

struct	ether_arp {
	struct	arphdr_tag ea_hdr;	
	uchar_t	arp_sha[6];	
	uchar_t	arp_spa[4];	
	uchar_t	arp_tha[6];	
	uchar_t	arp_tpa[4];	
};
#define	arp_hrd	ea_hdr.ar_hrd
#define	arp_pro	ea_hdr.ar_pro
#define	arp_hln	ea_hdr.ar_hln
#define	arp_pln	ea_hdr.ar_pln
#define	arp_op	ea_hdr.ar_op

#endif 

