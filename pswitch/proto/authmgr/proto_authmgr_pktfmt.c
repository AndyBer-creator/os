#include <common/sys_pktfmt.h>
#include <authmgr/proto_authmgr.h>
typedef struct tcpv6_pseudo_hdr_s
{
    sys_ipv6_t saddr;
    sys_ipv6_t daddr;
    uint32     tcpLen;
    uint8      zeros[3];
    uint8      nextHdr;
} tcpv6_pseudo_hdr_t;
uint16 ipChecksum(iphdr_t * pip)
{
    uint32 sum = 0, oddbyte = 0;
    uint16 *ptr = (uint16 *) pip;
    uint32 nbytes = HDR_DATALEN_TRANS(pip->ihl & 0xf);

    while (nbytes > 1)
    {
        sum += (*ptr++);
        nbytes -= 2;
    }
    if (nbytes == 1)
    {
        oddbyte = (*ptr & 0xff00);
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (~sum);
} 

uint16 tcpChecksum(iphdr_t* pip)
{
    int32 sum, nbytes, nhdr, i;
    uint16 *sip, *dip, *ptr;
    
    uint8 backup;

    nhdr = HDR_DATALEN_TRANS(pip->ihl & 0xf);
    nbytes = osal_ntohs(pip->tot_len) - nhdr;
    ptr = (uint16 *) ((uint8 *) pip + nhdr);
    sum = 0;

    backup = *(((uint8 *) pip) + nhdr + nbytes);
    *(((uint8 *) pip) + nhdr + nbytes) = (uint8) 0;
    for (i = 0; i < nbytes; i = i + 2)
    {
        sum += (unsigned long) osal_ntohs(*ptr);
        ptr++;
    }

    dip = (uint16 *) & pip->daddr;
    sum += osal_ntohs(*dip);
    dip++;
    sum += osal_ntohs(*dip);
    sip = (uint16 *) & pip->saddr;
    sum += osal_ntohs(*sip);
    sip++;
    sum += osal_ntohs(*sip);

    sum += nbytes;
    sum += ((uint16) pip->protocol);

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    *(((uint8 *) pip) + nhdr + nbytes) = backup;

    sum = ~sum;
    return osal_htons((uint16) sum);

} 

uint16 tcpv6Checksum(ipv6hdr_t *pIPv6)
{
    tcpv6_pseudo_hdr_t pseudoHdr;
    int32 sum, nbytes, nhdr, i;
    uint16 *ptr;
    
    uint8 backup;

    SYS_MEM_CLEAR(pseudoHdr);

    osal_memcpy(pseudoHdr.saddr.hex, pIPv6->saddr, sizeof(sys_ipv6_t));
    osal_memcpy(pseudoHdr.daddr.hex, pIPv6->daddr, sizeof(sys_ipv6_t));
    pseudoHdr.tcpLen = pIPv6->payload_len;
    pseudoHdr.nextHdr = 6; 

    nhdr = sizeof(ipv6hdr_t);
    nbytes = pIPv6->payload_len;
    ptr = (uint16 *) ((uint8 *) pIPv6 + nhdr);
    sum = 0;

    backup = *(((uint8 *) pIPv6) + nhdr + nbytes);
    *(((uint8 *) pIPv6) + nhdr + nbytes) = (uint8) 0;
    for (i = 0; i < nbytes; i = i + 2)
    {
        sum += (unsigned long) osal_ntohs(*ptr);
        ptr++;
    }

    ptr = (uint16 *) (&pseudoHdr);
    for (i = 0; i < sizeof(pseudoHdr); i = i + 2)
    {
        sum += (unsigned long) osal_ntohs(*ptr);
        ptr++;
    }

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    *(((uint8 *) pIPv6) + nhdr + nbytes) = backup;

    sum = ~sum;
    return osal_htons((uint16) sum);

} 

