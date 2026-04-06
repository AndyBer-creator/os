
#ifndef _NET_SVI_H__
#define _NET_SVI_H__

#include <drv/nic/nic.h>
#include <common/sys_l3.h>

#define SVI_UNIT_ID       (0)
#define SVI_TX_TIMEOUT    (10*HZ)
#define SVI_MAX_PKTLEN    (1600)
#define SVI_PKTLEN_RSVD   (2)     

#define SVI_INTF_MAX_NUM  (4)

#define SYS_VRRPID_NUM_MAX      (256)

typedef enum svi_tx_e
{
    SVI_TX_HANDLED = 0,
    SVI_TX_NOT_HANDLED,
} svi_tx_t;

typedef svi_tx_t (*svi_handler_usr_tx_cb_f)(drv_nic_pkt_t *pPkt, void *pCookie);

typedef struct svi_intf_s
{
    int32 netdevIdx;
    int32 netdevVrrpIdx;
} svi_intf_t;

typedef struct svi_handler_s
{
    uint8                   priority;
    svi_handler_usr_tx_cb_f tx_cbf;
} svi_handler_t;

typedef struct svi_dev_s
{
    
    int32             (*intf_add)(sys_l3_intfId_t *pId, svi_intf_t *pIntf);
    int32             (*intf_del)(sys_l3_intfId_t *pId, svi_intf_t *pIntf);
    int32             (*intf_up)(sys_l3_intfId_t *pId);
    int32             (*intf_down)(sys_l3_intfId_t *pId);
    int32             (*cb_tx)(drv_nic_pkt_t *pPkt, sys_l3_intfId_t *pId, void *pTxCb, void *pCookie);
    int32             (*cb_rx)(drv_nic_pkt_t *pPkt, sys_l3_intfId_t *pId);
} svi_dev_t;

extern svi_dev_t *svi_create(sys_mac_t *pMac);

extern int32 svi_handler_register(svi_handler_t *pHandler);

extern int32 svi_handler_unregister(svi_handler_t *pHandler);

extern int32 svi_init(void);

extern uint32 svi_exit(void);
#ifdef CONFIG_SYS_L3_VRRP
extern svi_dev_t *svi_vrrp_create(sys_mac_t mac, int vrid);
#endif
#endif  

