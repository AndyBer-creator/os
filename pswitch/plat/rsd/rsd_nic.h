
#ifndef __RSD_NIC_H__
#define __RSD_NIC_H__

#include <common/sys_def.h>
#include <drv/nic/nic.h>

#define VLANID_OFFSET    4

typedef enum rsd_nic_rx_e
{
    RSD_NIC_RX_NOT_HANDLED = NIC_RX_NOT_HANDLED,
    RSD_NIC_RX_HANDLED = NIC_RX_HANDLED,
    RSD_NIC_RX_HANDLED_OWNED = NIC_RX_HANDLED_OWNED,
} rsd_nic_rx_t;

typedef enum rsd_nic_tx_caretype_e
{
    TX_CARE_STP = 0,
    TX_CARE_TRUNK,
    TX_CARE_VLAN,
    TX_CARE_DOT1X,
    TX_CARE_END
} rsd_nic_tx_caretype_t;

typedef enum rsd_nic_rx_caretype_e
{
    RX_CARE_TRUNK = 0,
    RX_CARE_END
} rsd_nic_rx_caretype_t;

typedef enum l2g_act_e
{
    RSD_NIC_HANDLER_ACT_NONE = 0,
    RSD_NIC_HANDLER_ACT_TRAP,
    RSD_NIC_HANDLER_ACT_COPY,
    RSD_NIC_HANDLER_ACT_DROP,
    RSD_NIC_HANDLER_ACT_KERNEL,
    RSD_NIC_HANDLER_ACT_END,
} l2g_act_t;

typedef enum l2g_handler_pri_e
{
    
    RSD_NIC_HANDLER_PRI_HIGH = 0,
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    RSD_NIC_HANDLER_PRI_ACL,
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    RSD_NIC_HANDLER_PRI_AUTHMGR,
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
    RSD_NIC_HANDLER_PRI_PSECURE,
#endif
#if defined(CONFIG_SYS_PROTO_AUTHMGR) || defined(CONFIG_SYS_PORT_SECURITY)
    RSD_NIC_HANDLER_PRI_LIMITLEARN,
#endif
#ifdef CONFIG_SYS_PROTO_STP
    RSD_NIC_HANDLER_PRI_STP,
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
	RSD_NIC_HANDLER_PRI_ERPS,
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
	RSD_NIC_HANDLER_PRI_LOOPBACK,
#endif

#ifdef CONFIG_SYS_PROTO_LACP
    RSD_NIC_HANDLER_PRI_LACP,
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
    RSD_NIC_HANDLER_PRI_UDLD,
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    RSD_NIC_HANDLER_PRI_DAI,
#endif
#ifdef CONFIG_SYS_PROTO_DHCP
    RSD_NIC_HANDLER_PRI_DHCP_CLIENT,
    RSD_NIC_HANDLER_PRI_DHCP_SERVER,
#endif

    RSD_NIC_HANDLER_PRI_DFLT,
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    RSD_NIC_HANDLER_PRI_IGMP,
  #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    RSD_NIC_HANDLER_PRI_MLD,
  #endif
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    RSD_NIC_HANDLER_PRI_GVRP,
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    RSD_NIC_HANDLER_PRI_GMRP,
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    RSD_NIC_HANDLER_PRI_VOICE_VLAN_OUI,
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    RSD_NIC_HANDLER_PRI_SURVEILLANCE_VLAN_OUI,
#endif
#ifdef CONFIG_SYS_ADVANCE_DNS
    RSD_NIC_HANDLER_PRI_DNS,
#endif
    
    RSD_NIC_HANDLER_PRI_LOW,
#ifdef CONFIG_SYS_PROTO_LLDP
    RSD_NIC_HANDLER_PRI_LLDP,
#endif

    RSD_NIC_HANDLER_PRI_END
} l2g_handler_pri_t;

typedef drv_nic_pkt_t sys_nic_pkt_t;
typedef drv_nic_tx_cb_f sys_nic_tx_cb_f;
typedef rsd_nic_rx_t (*rsd_nic_rx_cb_f)(sys_nic_pkt_t *pPacket, void *pCookie);

typedef int32 (*rsd_nic_handler_rx_cb_f)(sys_nic_pkt_t *pPkt, void *pCookie);
typedef l2g_act_t (*rsd_nic_handler_pre_chk_cb_f)(sys_nic_pkt_t *pPkt, void *pCookie);

typedef struct rsd_nic_handler_s
{
    uint8       priority;
    uint8       noCondition;
    uint8       dmac[6];
    uint8       dmac_caremask[6];
    uint8       smac[6];
    uint8       smac_caremask[6];
    uint16      ethertype;
    uint8       proto;
    uint16      dport;
    uint16      sport;

    l2g_act_t   action;
    rsd_nic_handler_rx_cb_f rx_cbf;
    rsd_nic_handler_pre_chk_cb_f chk_cbf;
    uint8       rx_careMask;
    char        pName[CAPA_NAME_STR_LEN];
    void        *pCookie;
} rsd_nic_handler_t;

#define RSD_NIC_TX_CARE_NONE       (0)
#define RSD_NIC_TX_CARE_STP        (1 << TX_CARE_STP)
#define RSD_NIC_TX_CARE_TRUNK      (1 << TX_CARE_TRUNK)
#define RSD_NIC_TX_CARE_VLAN       (1 << TX_CARE_VLAN)
#define RSD_NIC_TX_CARE_DOT1X      (1 << TX_CARE_DOT1X)
#define RSD_NIC_TX_CARE_ALL        (RSD_NIC_TX_CARE_STP | RSD_NIC_TX_CARE_TRUNK | RSD_NIC_TX_CARE_VLAN | RSD_NIC_TX_CARE_DOT1X)

#define RSD_NIC_RX_CARE_NONE       (0)
#define RSD_NIC_RX_CARE_TRUNK      (1 << RX_CARE_TRUNK)

extern int32 rsd_nic_handler_register(rsd_nic_handler_t *pHandler);

extern int32 rsd_nic_handler_unregister(rsd_nic_handler_t *pHandler);

extern int32 rsd_nic_pkt_alloc(int32 size, sys_nic_pkt_t **ppPacket);

extern int32 rsd_nic_pkt_free(sys_nic_pkt_t *pPacket);

extern int32 rsd_nic_pkt_clone(int32 size, sys_nic_pkt_t **ppNewPacket, sys_nic_pkt_t *pOriPacket);

extern int32 rsd_nic_pktBlkPort_flood(sys_nic_pkt_t *pPacket, sys_vid_t vid, sys_logic_port_t blockPort, sys_pri_t priority);

extern int32 rsd_nic_pktBlkPM_flood(sys_nic_pkt_t *pPacket, sys_vid_t vid, sys_logic_portmask_t blockPm, sys_pri_t priority);

extern int32 rsd_nic_pktByDmacBlkPort_tx(sys_nic_pkt_t *pPkt, sys_pri_t priority, sys_vid_t vid, sys_logic_port_t blkPort);

extern int32 rsd_nic_pktByDmacBlkPM_tx(sys_nic_pkt_t *pPkt, sys_pri_t priority, sys_vid_t vid, sys_logic_portmask_t blkpm);

extern int32 rsd_nic_pkt_tx(sys_nic_pkt_t *pPacket, sys_logic_portmask_t *pPortmask, sys_pri_t priority,
                        sys_vid_t  vid, uint8 careMask);

extern int32 rsd_nic_pkt_txcb(sys_nic_pkt_t *pPacket, sys_logic_portmask_t *pPortmask, sys_pri_t priority,
                        sys_vid_t  vid, uint8 careMask, void *pTxCb, void *pCookie);

extern int32 rsd_nic_pktByVlanDmac_txcb(sys_nic_pkt_t *pPacket, sys_vid_t vid, void *pTxCb, void *pCookie);

extern int32 rsd_nic_isPortSTPForwad_ret(sys_logic_port_t port, sys_vid_t vlan);

extern int32 rsd_nic_isPktMyMac_ret(sys_nic_pkt_t *pPacket);

extern int32 rsd_nic_pktVid_get(sys_nic_pkt_t *pPacket, sys_vid_t *pVlan);

extern uint16 rsd_nic_checksum_get(uint16 *addr, int32 len, uint16* pChksum);

extern int32 rsd_nic_init(void);
#endif

