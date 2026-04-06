
#ifndef __RSD_L3_INTF_H__
#define __RSD_L3_INTF_H__

#include <linux/net/net_svi.h>

#if 0
#define L3_INTF_DBG(fmt, args...) SYS_PRINTF(fmt, ##args)
#define IS_L3_INTF_DBG_SET() (1)
#else
#define L3_INTF_DBG(fmt, args...) SYS_DBG(DBG_L3_INTF, "[L3 INTF] "fmt, ##args);
#define IS_L3_INTF_DBG_SET() IS_DBG_SET(DBG_L3_INTF)
#endif

extern int32 rsd_l3_intf_init(void);

extern int32 rsd_l3_intfIp_add(sys_l3_af_t af, sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ip_t *pIp);

extern int32 rsd_l3_intfIp_del(sys_l3_af_t af, sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ip_t *pIp);


extern int32 rsd_l3_adminIntfEnbl_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 rsd_l3_adminIntfIPv6Enbl_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 rsd_l3_adminIntfIPv6DestUnreach_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 rsd_l3_adminIntfMgmt_get(sys_l3_intfMgmtAdmin_t *pMgmt);

extern int32 rsd_l3_intfMgmt_get(sys_l3_intfMgmt_t *pMgmt);

extern int32 rsd_l3_intfGratuitousArp_send(sys_l3_intfId_t *pId);
extern int32 rsd_l3_sviPkt_tx(drv_nic_pkt_t *pPkt, sys_l3_intfId_t *pId);
#endif 

