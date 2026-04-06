#ifndef __L2G_AUTHMGR_PKTFMT_H__
#define __L2G_AUTHMGR_PKTFMT_H__
#include <common/sys_def.h>
#include <common/sys_type.h>
typedef struct ethhdr_s
{
    uint8  h_dest[CAPA_MAC_BYTE_NUM];   
    uint8  h_source[CAPA_MAC_BYTE_NUM]; 
    uint16 h_proto;     
}__attribute__ ((aligned(1), packed))ethhdr_t;

typedef struct arp_pkt_s
{
    uint16     htype;     
    uint16     ptype;     
    uint8      hlen;      
    uint8      plen;      
    uint16     operation; 
    sys_mac_t  sHaddr;    
    sys_ipv4_t sInaddr;   
    sys_mac_t  tHaddr;    
    sys_ipv4_t tInaddr;   
} __attribute__ ((aligned(1), packed)) arp_pkt_t;

typedef struct iphdr_s
{
    uint8  version:4, 
           ihl:4;     
    uint8  tos;       
    uint16 tot_len;   
    uint16 id;        
    uint16 frag_off;  
    uint8  ttl;       
    uint8  protocol;  
    uint16 check;     
    uint32 saddr;     
    uint32 daddr;     
}__attribute__ ((aligned(1), packed))iphdr_t;

typedef struct ipv6hdr_s
{
    uint8  version:4,   
           priority:4;  
    uint8  flow_lbl[3]; 
    uint16 payload_len; 
    uint8  nexthdr;     
    uint8  hop_limit;   
    uint8  saddr[16];   
    uint8  daddr[16];   
}__attribute__ ((aligned(1), packed))ipv6hdr_t;

typedef struct icmpv6hdr_s
{
    uint8  icmp6_type;
    uint8  icmp6_code;
    uint16 icmp6_cksum;
    union {
        uint32 un_data32[1];
        uint16 un_data16[2];
        uint8  un_data8[4];
        struct icmpv6_echo {
            uint16 identifier;
            uint16 sequence;
        } u_echo;
        struct icmpv6_nd_advt {
            uint32 router:1,
                   solicited:1,
                   override:1,
                   reserved:29;
        } u_nd_advt;
        struct icmpv6_nd_ra {
            uint8 hop_limit;
            uint8 managed:1,
                  other:1,
                  router_pref:2,
                  reserved:4;
        } u_nd_ra;
        struct icmpv6_ns {
            uint32 reserved;    
            uint8  taddr[16];   
        } u_ns;
    } icmp6_dataun;
}__attribute__ ((aligned(1), packed))icmp6hdr_t;

typedef struct tcphdr_s
{
    uint16 source;    
    uint16 dest;      
    uint32 seq;       
    uint32 ack_seq;   
    uint16 doff:4,    
           res1:4,    
           cwr:1,
           ece:1,
           urg:1,
           ack:1,
           psh:1,
           rst:1,
           syn:1,
           fin:1;
    uint16 window;    
    uint16 check;     
    uint16 urg_ptr;   
}__attribute__ ((aligned(1), packed))tcphdr_t;

typedef struct udphdr_s
{
    uint16 sport;  
    uint16 dport;  
    uint16 len;    
    uint16 sum;    
}__attribute__ ((aligned(1), packed))udphdr_t;

typedef struct tcpdata_s
{
    uint32 len;
    uint8  *data;
} tcpdata_t;
#define HDR_DATALEN_TRANS(val) ((val) << 2)
extern uint16 ipChecksum(iphdr_t *pip);
extern uint16 tcpChecksum(iphdr_t *pip);
extern uint16 tcpv6Checksum(ipv6hdr_t *pIPv6);

#endif 
