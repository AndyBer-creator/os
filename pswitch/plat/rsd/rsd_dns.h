
#ifndef __RSD_DNS_H__
#define __RSD_DNS_H__

#define DNS_PROC_NAME    "hosts"
#define DNS_PROC_SEQNAME dns
#define DNS_PROC_BUFLEN  8192

typedef struct sys_dns_host_s
{
    struct sys_dns_host_s *prev;
    struct sys_dns_host_s *next;

    char hostname[CAPA_HOSTNAME_LEN];
    uint32 total;
    uint32 elapsede;
    sys_l3_af_t af;
    sys_ip_t ip;
} sys_dns_host_t;

typedef struct sys_dns_format_s
{
    uint16 transactionId;
    uint16 flags;
    uint16 questions;
    uint16 answersRRs;
    uint16 authorityRRs;
    uint16 additionalRRs;
} sys_dns_format_t;

extern int32 rsd_dns_enable_set(sys_enable_t enable);
extern int32 rsd_dns_clear_set(void);
extern int32 rsd_dns_init(void);
extern int32 rsd_dns_dump(char *buf, int32 buflen, int32 *pLen);

#endif

