
#include <linux/version.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if.h>
#include <linux/rtnetlink.h>
#include <linux/init.h>
#include <linux/ethtool.h>
#include <linux/slab.h>
#include <linux/signal.h>
#include <linux/time.h>
#include <net/arp.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/time.h>
#include <dev_config.h>
#include <osal/print.h>
#include <drv/nic/nic.h>
#include <rtk/switch.h>
#include <rtk/l3.h>
#include <common/sys_def.h>
#include <common/sys_pktfmt.h>
#include <linux/net/net_svi.h>

#define PKTBUF_ALLOC(size)      kmalloc(size, GFP_ATOMIC)
#define PKTBUF_FREE(pktbuf)     kfree(pktbuf)

#define SVI_RX_HANDLER_PRIORITY   1
#define DUMP_RX_PKT 0
#define DUMP_TX_PKT 0

#define EMPTY_NETDEV -1

typedef struct svi_netdev_s
{
    uint8             active;
    int32             id;
    int32             vrid;
    struct net_device *dev;
} svi_netdev_t;

typedef struct svi_priv_s
{
    spinlock_t              lock;
    struct net_device       *pNdev;
    struct net_device_stats ndev_stats;
} svi_priv_t;

typedef struct svi_handler_list_s
{
    svi_handler_t handler;
    struct svi_handler_list_s *next;
} svi_handler_list_t;

svi_dev_t *gp_svi = NULL;
#ifdef CONFIG_SYS_L3_VRRP
svi_dev_t *gp_svi_vrrp = NULL;
#endif
static struct net_device *gp_netdevMgmtVlan = NULL;

#ifdef CONFIG_SYS_L3_ROUTE
static svi_intf_t *gp_intfVlan = NULL;
static svi_netdev_t *gp_netdevVlan = NULL;

static svi_netdev_t *gp_netdevLo = NULL;
#ifdef CONFIG_SYS_L3_VRRP
static svi_netdev_t *gp_netdevVrrp = NULL;
#endif
#endif

static sys_mac_t          g_sysMac;

static struct net_device *gp_ndev = NULL;
static svi_handler_list_t *gp_sviHandlerHead = NULL;

#define NETDEV(type, id) (_intfId2Netdev(type, id))
#define NETDEV_VLAN(vid) (_intfId2Netdev(SYS_L3_INTFTYPE_VLAN, vid))
#define NETDEV_MGMTVLAN() (_intfId2Netdev(SYS_L3_INTFTYPE_MGMTVLAN, 0))
#ifdef CONFIG_SYS_L3_VRRP
#define NETDEV_VRRP(vrid) (_intfId2Netdev(SYS_L3_INTFTYPE_VRRP, vrid))
#endif

struct net_device *_intfId2Netdev(sys_l3_intfType_t intfType, uint32 id)
{
#ifdef CONFIG_SYS_L3_ROUTE
    if (SYS_L3_INTFTYPE_VLAN == intfType)
    {
        if (gp_intfVlan[id].netdevIdx < 0)
            return NULL;

        return gp_netdevVlan[gp_intfVlan[id].netdevIdx].dev;
    }
    else if (SYS_L3_INTFTYPE_MGMTVLAN == intfType)
    {
        return gp_netdevMgmtVlan;
    }
    else if (SYS_L3_INTFTYPE_LO == intfType)
    {
        return gp_netdevLo[id].dev;
    }
#ifdef CONFIG_SYS_L3_VRRP
    else if (SYS_L3_INTFTYPE_VRRP == intfType)
    {
        return gp_netdevVrrp[id].dev;
    }
#endif
#else
    return gp_netdevMgmtVlan;
#endif

    return NULL;
}

static int32 _str2IntfId(char *devStr, sys_l3_intfId_t *pId)
{
#ifdef CONFIG_SYS_L3_ROUTE
    char *pIdxStart = NULL;
    int32 devIdx = -1;

    pIdxStart = devStr + L3_INTF_PREFIX_LEN;
    devIdx = simple_strtol(pIdxStart, NULL, 10);

    if (devIdx < 0)
        return SYS_ERR_FAILED;

    if (0 == osal_strncmp(devStr, L3_INTF_PREFIX_VLAN, osal_strlen(L3_INTF_PREFIX_VLAN)))
    {
        pId->type = SYS_L3_INTFTYPE_VLAN;
        pId->id = gp_netdevVlan[devIdx].id;
    }
    else if (0 == osal_strncmp(devStr, L3_INTF_MGMTVLAN, osal_strlen(L3_INTF_MGMTVLAN)))
    {
        pId->type = SYS_L3_INTFTYPE_MGMTVLAN;
    }
    else if (0 == osal_strncmp(devStr, L3_INTF_PREFIX_LO, osal_strlen(L3_INTF_PREFIX_LO)))
    {
        pId->type = SYS_L3_INTFTYPE_LO;
        pId->id = gp_netdevLo[devIdx].id;
    }  
#ifdef CONFIG_SYS_L3_VRRP
	else if (0 == osal_strncmp(devStr, L3_INTF_PREFIX_VRRP, osal_strlen(L3_INTF_PREFIX_VRRP)))
    {
		pIdxStart = devStr + L3_INTF_PREFIX_VRRP_LEN;
		devIdx = simple_strtol(pIdxStart, NULL, 10);
		if (gp_netdevVrrp[devIdx].dev)
		{
			pId->type = SYS_L3_INTFTYPE_VRRP;
            pId->id = gp_netdevVrrp[devIdx].id;
            pId->vrid = gp_netdevVrrp[devIdx].vrid;
		}
		else
		{
			return SYS_ERR_FAILED;
		}
    }
#endif
#else
    pId->type = SYS_L3_INTFTYPE_MGMTVLAN;
#endif

    return SYS_ERR_OK;
}

#if (DUMP_RX_PKT || DUMP_TX_PKT)
static void _dump_pkt(void *pPkt, uint32 len)
{
    uint32 i = 0;

    for (i = 0; i < len; i++)
    {
        SYS_PRINTF("%02x ", (*(char *)(pPkt + i)) & 0xFF);
        if ((i + 1) % 8 == 0)
            SYS_PRINTF("  ");
        if ((i + 1) % 16 == 0)
            SYS_PRINTF("\n");
    }

    SYS_PRINTF("\n\n");
}
#endif

static int32 _svi_pkt_alloc(uint32 unit, int32 size, uint32 flags, drv_nic_pkt_t **ppPacket)
{
    struct sk_buff *pSkb;
    drv_nic_pkt_t *pPacket;

    pPacket = PKTBUF_ALLOC(sizeof(drv_nic_pkt_t));
    if (NULL == pPacket)
    {
        return RT_ERR_FAILED;
    }

    pSkb = dev_alloc_skb(size + SVI_PKTLEN_RSVD);
    if (NULL == pSkb)
    {
        PKTBUF_FREE(pPacket);
        return RT_ERR_FAILED;
    }

    skb_reserve(pSkb, SVI_PKTLEN_RSVD);

    memset(pPacket, 0, sizeof(drv_nic_pkt_t));
    pPacket->head = pSkb->head;
    pPacket->data = pSkb->data;
    pPacket->tail = pSkb->data;
    pPacket->end = pSkb->end;
    pPacket->length = 0;
    pPacket->buf_id = (void *)pSkb;
    pPacket->next = NULL;

    *ppPacket = pPacket;

    return RT_ERR_OK;
} 

static int32 _svi_pkt_free(uint32 unit, drv_nic_pkt_t *pPacket)
{
    struct sk_buff *pSkb;

    if (NULL == pPacket)
    {
        osal_printf("Error: pPacket is NULL at %s():%d\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }

    pSkb = (struct sk_buff *)(pPacket->buf_id);
    if (NULL == pSkb)
    {
        osal_printf("Error: skb is NULL at %s():%d\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }

#if 0
    dev_kfree_skb_any(pSkb);
#else
    dev_kfree_skb(pSkb);
#endif

    PKTBUF_FREE(pPacket);

    return RT_ERR_OK;
} 

static int _svi_vlan_tag_strip(drv_nic_pkt_t *pPacket)
{
    int i = 0;
    uint16 proto = 0;

    proto = *(uint16 *)(pPacket->data + 12);

    if (0x8100 == proto || 0x88a8 == proto || 0x9100 == proto || 0x9200 == proto)
    {
#if (DUMP_RX_PKT)
        SYS_PRINTF("Before Strip Tag\n");
        _dump_pkt((void*)pPacket->data, pPacket->length);
#endif

        for (i = 15; i>3; i--)
            *(pPacket->data + i) = *(pPacket->data + i - 4);

        pPacket->data += 4;
        pPacket->length -= 4;

#if (DUMP_RX_PKT)
        SYS_PRINTF("After Strip Tag\n");
        _dump_pkt((void*)pPacket->data, pPacket->length);
#endif

        _svi_vlan_tag_strip(pPacket);
    }

    return SYS_ERR_OK;
}

static drv_nic_rx_t svi_rx_callback(uint32 unit, drv_nic_pkt_t *pPacket, void *pCookie)
{
    struct net_device *pNdev = NULL;
    svi_priv_t *pPriv = NULL;
    struct sk_buff *pSkb = NULL;
    int32  retval = 0;
    sys_l3_intfId_t intfId;

    if (NULL == gp_svi)
        goto _exit;

    if (NULL == pPacket)
    {
        osal_printf("Error: pPacket is NULL at %s():%d\n", __FUNCTION__, __LINE__);
        goto _exit;
    }

    _svi_vlan_tag_strip(pPacket);

    if (SYS_ERR_OK != gp_svi->cb_rx(pPacket, &intfId))
        goto _exit;

    if (SYS_L3_INTFTYPE_VLAN == intfId.type)
        pNdev = NETDEV_VLAN(intfId.id);
    else if (SYS_L3_INTFTYPE_MGMTVLAN == intfId.type)
        pNdev = NETDEV_MGMTVLAN();
#ifdef CONFIG_SYS_L3_VRRP
    else if (SYS_L3_INTFTYPE_VRRP == intfId.type)
        pNdev = NETDEV_VRRP(intfId.vrid-1);
#endif

    if (NULL == pNdev)
        goto _exit;

    pPriv = netdev_priv(pNdev);

#if (DUMP_RX_PKT)
    SYS_PRINTF("====== %s RX PKT====== \n", pNdev->name);
    _dump_pkt((void*)pPacket->data, pPacket->length);
#endif

    pSkb = (struct sk_buff *)(pPacket->buf_id);
    if (NULL == pSkb)
    {
        osal_printf("Error: skb is NULL at %s():%d\n", __FUNCTION__, __LINE__);
        goto _exit;
    }

    pSkb->data = pPacket->data;
    pSkb->tail = pPacket->tail;
    pSkb->len = pPacket->length;

    if (0x1 == pSkb->data[0] && 0x80 == pSkb->data[1] && 0xc2 == pSkb->data[2])
       goto  _exit;

    if (pPriv)
    {
        pPriv->ndev_stats.rx_packets++;
        pPriv->ndev_stats.rx_bytes += pSkb->len;
    }

#if defined(CONFIG_SYS_MGMT_ACL)
    pSkb->cb[39] = (unsigned char)pPacket->rx_tag.source_port;
    
    pSkb->cb[38] = (char)((pPacket->rx_tag.fvid)/100);
    pSkb->cb[37] = (char)((pPacket->rx_tag.fvid)%100);
#endif

    pSkb->dev = pNdev;
    pSkb->protocol = eth_type_trans(pSkb, pNdev);
    pSkb->ip_summed = CHECKSUM_UNNECESSARY;
    pNdev->last_rx = jiffies;
	
	pSkb->uIntftype = intfId.type;
	pSkb->uSport    = pPacket->rx_tag.source_port;
	pSkb->uFvlan    = intfId.id;

    PKTBUF_FREE(pPacket);

    retval = netif_rx(pSkb);  
    if (NET_RX_DROP == retval)
    {
        if (pPriv)
        {
            pPriv->ndev_stats.rx_dropped++;
        }
    }

    return NIC_RX_HANDLED_OWNED;

_exit:
    return NIC_RX_NOT_HANDLED;
}

static void svi_tx_callback(uint32 unit, drv_nic_pkt_t *pPacket, void *pCookie)
{
    struct net_device *pNdev = (struct net_device *)pCookie;
    svi_priv_t *pPriv = netdev_priv(pNdev);
    struct sk_buff *pSkb;

    if (NULL == pPacket)
    {
        osal_printf("Error: pPacket is NULL at %s():%d\n", __FUNCTION__, __LINE__);
        goto _exit;
    }

#if (DUMP_TX_PKT)
    SYS_PRINTF("====== %s TX PKT ====== \n", pNdev->name);
    _dump_pkt((void*)pPacket->data, pPacket->length);
#endif

    pSkb = (struct sk_buff *)(pPacket->buf_id);
    if (NULL == pSkb)
    {
        osal_printf("Error: skb is NULL at %s():%d\n", __FUNCTION__, __LINE__);
        PKTBUF_FREE(pPacket);
        goto _exit;
    }

    if (pPriv)
    {
        pPriv->ndev_stats.tx_packets++;
        pPriv->ndev_stats.tx_bytes += pSkb->len;
    }

    dev_kfree_skb_irq(pSkb);

    PKTBUF_FREE(pPacket);

_exit:
    netif_wake_queue(pNdev);
    return;
}

static int32 svi_open(struct net_device *pNdev)
{
#if 0
    svi_priv_t *pPriv = netdev_priv(pNdev);

    if (netif_msg_ifup(pPriv))
    {
        osal_printf("%s: enabling interface.\n", pNdev->name);
    }
#endif

    netif_start_queue(pNdev);

    return RT_ERR_OK;
}

static int32 svi_close(struct net_device *pNdev)
{
#if 0
    svi_priv_t *pPriv = netdev_priv(pNdev);

    if (netif_msg_ifdown(pPriv))
    {
        osal_printf("%s: disabling interface.\n", pNdev->name);
    }
#endif

    netif_stop_queue(pNdev);

    return RT_ERR_OK;
}

static struct net_device_stats *svi_get_stats(struct net_device *pNdev)
{
#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
    svi_priv_t *pPriv = (svi_priv_t *)netdev_priv(pNdev);
#else
    svi_priv_t *pPriv = pNdev->priv;
#endif

    return (&pPriv->ndev_stats);
}

static svi_tx_t _svi_handler_tx_callback(drv_nic_pkt_t *pPacket, void *pCookie)
{
    svi_handler_list_t *pHandlerList = gp_sviHandlerHead;
    svi_handler_t *handler;
    svi_tx_t action = SVI_TX_HANDLED;

    while (pHandlerList != NULL)
    {
        handler = &(pHandlerList->handler);

        if (NULL != handler->tx_cbf)
            action = handler->tx_cbf(pPacket, pCookie);

        pHandlerList = pHandlerList->next;
    }

    return action;
} 

static int32 svi_start_xmit(struct sk_buff *pSkb, struct net_device *pNdev)
{
    svi_priv_t *pPriv = netdev_priv(pNdev);
    drv_nic_pkt_t *pPacket;
    sys_l3_intfId_t id;

    netif_stop_queue(pNdev);

    pPacket = PKTBUF_ALLOC(sizeof(drv_nic_pkt_t));
    if (NULL == pPacket)
    {
        pPriv->ndev_stats.tx_errors++;
        dev_kfree_skb(pSkb);
        osal_printf("Error: Out of memory at %s():%d\n", __FUNCTION__, __LINE__);
        goto xmit_exit;
    }

    memset(pPacket, 0, sizeof(drv_nic_pkt_t));

    pPacket->as_txtag = 0;

    pPacket->buf_id = (void *)pSkb;
    pPacket->head = pSkb->head;
    pPacket->data = pSkb->data;
    pPacket->tail = pSkb->data + pSkb->len;
    pPacket->end = pSkb->end;
    pPacket->length = pSkb->len;
    pPacket->next = NULL;

    if (SVI_TX_NOT_HANDLED == _svi_handler_tx_callback(pPacket, NULL))
        goto xmit_exit;

    _str2IntfId(pNdev->name, &id);

    if (SYS_ERR_OK == gp_svi->cb_tx(pPacket, &id, svi_tx_callback, (void*)pNdev))
    {
        pNdev->trans_start = jiffies;
    }

    else
    {
        pPriv->ndev_stats.tx_dropped++;
        dev_kfree_skb(pSkb);
    }

xmit_exit:
    netif_start_queue(pNdev);

    return NETDEV_TX_OK;
}

static void svi_tx_timeout(struct net_device *pNdev)
{
    osal_printf("Tx Timeout!!! Can't send packet\n");
    return;
}

static int32 svi_ioctl (struct net_device *pNdev, struct ifreq *pRq, int32 cmd)
{
    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_KERNEL_LINUX_2_6_X

static void svi_set_multicast_list(struct net_device *pNdev)
{
    return;
}
#endif

static int32 svi_set_mac_address(struct net_device *pNdev, void *ptr)
{
    svi_priv_t *pPriv = netdev_priv(pNdev);
    unsigned long flags;
    struct sockaddr *sa = ptr;
    uint32 i;

    spin_lock_irqsave(&pPriv->lock, flags);

    for (i = 0; i < 6; ++i)
        pNdev->dev_addr[i] = sa->sa_data[i];

    spin_unlock_irqrestore(&pPriv->lock, flags);

    return SYS_ERR_OK;
}

static int32 svi_intf_down(sys_l3_intfId_t *pId)
{
    struct net_device *pNdev = NULL;
    struct ifreq ifr;

    SYS_MEM_CLEAR(ifr);
	
#ifdef CONFIG_SYS_L3_VRRP
	if (SYS_L3_INTFTYPE_VRRP == pId->type)
	{
		pNdev = NETDEV_VRRP(pId->vrid - 1);
	}
	else
#endif
    pNdev = NETDEV(pId->type, pId->id);

    if (NULL == pNdev)
        return SYS_ERR_NULL_POINTER;

    rtnl_lock();
    ifr.ifr_flags = dev_get_flags(pNdev);
    ifr.ifr_flags &= ~IFF_UP;
    dev_change_flags(pNdev, ifr.ifr_flags);
    rtnl_unlock();

    return SYS_ERR_OK;
}

static int32 svi_intf_up(sys_l3_intfId_t *pId)
{
    struct net_device *pNdev = NULL;
    struct ifreq ifr;

    SYS_MEM_CLEAR(ifr);
#ifdef CONFIG_SYS_L3_VRRP
	if (SYS_L3_INTFTYPE_VRRP == pId->type)
	{
		pNdev = NETDEV_VRRP(pId->vrid - 1);
	}
	else
#endif
    pNdev = NETDEV(pId->type, pId->id);

    if (NULL == pNdev)
        return SYS_ERR_NULL_POINTER;

    rtnl_lock();
    ifr.ifr_flags = dev_get_flags(pNdev);
    ifr.ifr_flags |= IFF_UP;
    dev_change_flags(pNdev, ifr.ifr_flags);
    rtnl_unlock();

    return SYS_ERR_OK;
}

static int32 svi_intf_add(sys_l3_intfId_t *pId, svi_intf_t *pIntf)
{
#ifdef CONFIG_SYS_L3_ROUTE
    int32 i = 0;
    int32 foundIdx = -1;
    struct net_device *pNdev = NULL;
    svi_priv_t *pPriv = NULL;
    svi_intf_t *pSviIntf = NULL;
    svi_netdev_t *pSviNdev = NULL;

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
    {
        if (NULL != NETDEV_VLAN(pId->id))
            return SYS_ERR_OK;

        for (i = 0; i < CAPA_L3_INTFVLAN_NUM; i++)
        {
            if (gp_netdevVlan[i].active)
                continue;

            foundIdx = i;
            break;
        }

        if (foundIdx < 0)
            return SYS_ERR_FAILED;

        pSviIntf = &gp_intfVlan[pId->id];
        pSviNdev = &gp_netdevVlan[foundIdx];
		pSviIntf->netdevIdx = foundIdx;
    }
#ifdef CONFIG_SYS_L3_VRRP
    else if (SYS_L3_INTFTYPE_VRRP == pId->type)
    {
    	#if 0
        if (NULL != NETDEV_VRRP(pId->vrid))
            return SYS_ERR_OK;

        for (i = 0; i < CAPA_L3_INTFVRRP_NUM; i++)
        {
            if (gp_netdevVrrp[i].active)
                continue;

            foundIdx = i;
            break;
        }

        if (foundIdx < 0)
            return SYS_ERR_FAILED;
        #endif
        if (NULL != NETDEV_VRRP(pId->vrid - 1))
    	{
			foundIdx = pId->vrid - 1;
		}
		else
		{
			for (i = 0; i < CAPA_L3_INTFVRRP_NUM; i++)
			{
				if (gp_netdevVrrp[i].active)
					continue;
			
				foundIdx = i;
				break;
			}
		}
		
        if (foundIdx < 0)
            return SYS_ERR_FAILED;
		
        pSviIntf = &gp_intfVlan[pId->id];
        pSviNdev = &gp_netdevVrrp[foundIdx];
		gp_intfVlan[pId->id].netdevVrrpIdx = foundIdx;
    }
#endif
    pNdev = pSviNdev->dev;
    pPriv = netdev_priv(pNdev);

    pSviNdev->active = TRUE;
    pSviNdev->id = pId->id;
    pSviNdev->vrid = pId->vrid;

    osal_memcpy(pIntf, pSviIntf, sizeof(svi_intf_t));
#endif

    return SYS_ERR_OK;
}

static int32 svi_intf_del(sys_l3_intfId_t *pId, svi_intf_t *pIntf)
{
#ifdef CONFIG_SYS_L3_ROUTE
    struct net_device *pNdev = NULL;
    svi_priv_t *pPriv = NULL;
    svi_intf_t *pSviIntf = NULL;
    svi_netdev_t *pSviNdev = NULL;

    if (SYS_L3_INTFTYPE_VLAN == pId->type)
    {
        pSviIntf = &gp_intfVlan[pId->id];

        if (EMPTY_NETDEV == pSviIntf->netdevIdx)
            return SYS_ERR_OK;

        pSviNdev = &gp_netdevVlan[pSviIntf->netdevIdx];
		pNdev = pSviNdev[pSviIntf->netdevIdx].dev;
    }
#ifdef CONFIG_SYS_L3_VRRP
    else if (SYS_L3_INTFTYPE_VRRP == pId->type)
    {
        pSviIntf = &gp_intfVlan[pId->id];

        if (EMPTY_NETDEV == pSviIntf->netdevVrrpIdx)
            return SYS_ERR_OK;

        pSviNdev = &gp_netdevVrrp[pSviIntf->netdevVrrpIdx];
		pNdev = pSviNdev[pSviIntf->netdevVrrpIdx].dev;
		pIntf->netdevVrrpIdx = pSviIntf->netdevVrrpIdx;
    }
#endif
    pIntf->netdevIdx = pSviIntf->netdevIdx;

    pPriv = netdev_priv(pNdev);

    svi_intf_down(pId);

    pSviNdev->active = FALSE;
    pSviNdev->id = -1;
	if (SYS_L3_INTFTYPE_VLAN == pId->type)
    pSviIntf->netdevIdx = EMPTY_NETDEV;
#ifdef CONFIG_SYS_L3_VRRP
	if (SYS_L3_INTFTYPE_VRRP == pId->type)
		pSviIntf->netdevVrrpIdx = EMPTY_NETDEV;
#endif
#endif

    return SYS_ERR_OK;
}

int32 svi_handler_register(svi_handler_t *pHandler)
{
    svi_handler_list_t *pHandlerListNew = NULL;
    svi_handler_list_t *pHandlerList = NULL;
    svi_handler_list_t *pHandlerListLast = NULL;
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(NULL == pHandler, SYS_ERR_NULL_POINTER);

    if ((pHandlerListNew = osal_alloc(sizeof(svi_handler_list_t))) != NULL)
    {
        osal_memcpy(&pHandlerListNew->handler, pHandler, sizeof(svi_handler_t));

        if (NULL == gp_sviHandlerHead)
        {
            
            pHandlerListNew->next = NULL;
            gp_sviHandlerHead = pHandlerListNew;
        }
        else
        {
            
            pHandlerList = gp_sviHandlerHead;

            while (NULL != pHandlerList)
            {
                if (pHandlerListNew->handler.priority < pHandlerList->handler.priority)
                {
                     pHandlerListNew->next = pHandlerList;

                     if (NULL != pHandlerListLast)
                         pHandlerListLast->next = pHandlerListNew;
                     else
                         gp_sviHandlerHead = pHandlerListNew;

                     break;
                }

                pHandlerListLast = pHandlerList;
                pHandlerList = pHandlerList->next;
            }

            if (NULL == pHandlerList)
            {
                pHandlerListLast->next = pHandlerListNew;
                pHandlerListNew->next = NULL;
            }
        }

        pHandlerList = gp_sviHandlerHead;

        while (pHandlerList != NULL)
        {
            pHandlerList = pHandlerList->next;
        }
    }
    else
    {
        ret = SYS_ERR_OUT_OF_MEMORY;  
    }

    return ret;
}

int32 svi_handler_unregister(svi_handler_t *pHandler)
{
    svi_handler_list_t *pHandlerList = gp_sviHandlerHead;
    svi_handler_list_t *pHandlerListLast = NULL;

    SYS_PARAM_CHK(NULL == pHandler, SYS_ERR_NULL_POINTER);

    while (pHandlerList != NULL)
    {
        if (0 == osal_memcmp(&pHandlerList->handler, pHandler, sizeof(svi_handler_t)))
        {
            if (pHandlerList == gp_sviHandlerHead)
            {
                gp_sviHandlerHead = pHandlerList->next;
            }
            else
            {
                pHandlerListLast->next = pHandlerList->next;
            }

            osal_free(pHandlerList);

            return SYS_ERR_OK;
        }

        pHandlerListLast = pHandlerList;
        pHandlerList = pHandlerList->next;
    }

    return SYS_ERR_ENTRY_NOT_FOUND; 
}

#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
const struct net_device_ops svi_netdev_ops = {
    .ndo_open = svi_open,
    .ndo_stop = svi_close,
    .ndo_do_ioctl = svi_ioctl,
    .ndo_tx_timeout = svi_tx_timeout,
    .ndo_start_xmit = svi_start_xmit,
    .ndo_set_mac_address = svi_set_mac_address,
    .ndo_get_stats = svi_get_stats,
};
#endif

static int32 _netdevCreate(sys_l3_intfType_t intfType, int32 idx)
{
    struct net_device *pNdev = NULL;
    svi_priv_t *pPriv = NULL;
    char intfName[SYS_BUF16_LEN];
    struct ifreq ifr;

    SYS_MEM_CLEAR(ifr);
    SYS_MEM_CLEAR(intfName);

    if (SYS_L3_INTFTYPE_MGMTVLAN == intfType)
        osal_sprintf(intfName, "%s", L3_INTF_MGMTVLAN);
#ifdef CONFIG_SYS_L3_ROUTE
    else if (SYS_L3_INTFTYPE_VLAN == intfType)
        osal_sprintf(intfName, "%s%d", L3_INTF_PREFIX_VLAN, idx);
    else if (SYS_L3_INTFTYPE_LO == intfType)
        osal_sprintf(intfName, "%s%d", L3_INTF_PREFIX_LO, idx);
#endif

    pNdev = alloc_etherdev(sizeof(svi_priv_t));

    if (NULL == pNdev)
        return SYS_ERR_FAILED;

    pPriv = netdev_priv(pNdev);

    osal_memset(pPriv, 0, sizeof(svi_priv_t));

#ifdef CONFIG_SYS_KERNEL_LINUX_2_6_X
    SET_MODULE_OWNER(pNdev);
#endif
    pPriv->pNdev = pNdev;
    spin_lock_init(&pPriv->lock);

    osal_strcpy(pNdev->name, intfName);

#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
    pNdev->netdev_ops = &svi_netdev_ops;
    pNdev->watchdog_timeo = SVI_TX_TIMEOUT;
    pNdev->irq = rtk_dev[SYS_DEV_ID_DEFAULT].irq;

  #ifdef CONFIG_SYS_L3_ROUTE
    pNdev->dev_addr = (uint8*)&g_sysMac;
  #else
    rtk_switch_mgmtMacAddr_get(SYS_DEV_ID_DEFAULT, &g_sysMac);
    pNdev->dev_addr = (uint8*)&g_sysMac;
  #endif
#else
    pNdev->open = svi_open;
    pNdev->stop = svi_close;
    pNdev->do_ioctl = svi_ioctl;
    pNdev->watchdog_timeo = SVI_TX_TIMEOUT;
    pNdev->tx_timeout = svi_tx_timeout;
    pNdev->hard_start_xmit = svi_start_xmit;
    pNdev->set_multicast_list = svi_set_multicast_list;
    pNdev->set_mac_address = svi_set_mac_address;
    pNdev->get_stats = svi_get_stats;
    pNdev->irq = rtk_dev[SYS_DEV_ID_DEFAULT].irq;

  #ifdef CONFIG_SYS_L3_ROUTE
    osal_memcpy(&pNdev->dev_addr, &g_sysMac, sizeof(sys_mac_t));
  #else
    rtk_switch_mgmtMacAddr_get(SYS_DEV_ID_DEFAULT, (sys_mac_t *)&pNdev->dev_addr);
  #endif
#endif

    if (register_netdev(pNdev) != 0)
    {
        if (NULL != pPriv)
            osal_free(pPriv);

        free_netdev(pNdev);

        return SYS_ERR_FAILED;
    }

    if (SYS_L3_INTFTYPE_MGMTVLAN == intfType)
        gp_netdevMgmtVlan = pNdev;
#ifdef CONFIG_SYS_L3_ROUTE
    else if (SYS_L3_INTFTYPE_VLAN == intfType)
        gp_netdevVlan[idx].dev = pNdev;
    else if (SYS_L3_INTFTYPE_LO == intfType)
        gp_netdevLo[idx].dev = pNdev;
#endif

    rtnl_lock();
    ifr.ifr_flags = dev_get_flags(pNdev);
    ifr.ifr_flags |= IFF_UP;
    dev_change_flags(pNdev, ifr.ifr_flags);
    rtnl_unlock();

    if (SYS_L3_INTFTYPE_MGMTVLAN == intfType)
        return SYS_ERR_OK;

    rtnl_lock();
    ifr.ifr_flags = dev_get_flags(pNdev);
    ifr.ifr_flags &= ~IFF_UP;
    dev_change_flags(pNdev, ifr.ifr_flags);
    rtnl_unlock();

    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_L3_VRRP
static int32 _vrrpNetdevCreate(sys_l3_intfType_t intfType, int32 vrid)
{
    struct net_device *pNdev = NULL;
    svi_priv_t *pPriv = NULL;
    char intfName[SYS_BUF16_LEN];
    struct ifreq ifr;
	sys_mac_t mac;

    SYS_MEM_CLEAR(ifr);
    SYS_MEM_CLEAR(intfName);
	SYS_MEM_CLEAR(mac);

    if (SYS_L3_INTFTYPE_VRRP == intfType)
        osal_sprintf(intfName, "vrrp%d", vrid);

    pNdev = alloc_etherdev(sizeof(svi_priv_t));

    if (NULL == pNdev)
        return SYS_ERR_FAILED;

    pPriv = netdev_priv(pNdev);

    osal_memset(pPriv, 0, sizeof(svi_priv_t));

#ifdef CONFIG_SYS_KERNEL_LINUX_2_6_X
    SET_MODULE_OWNER(pNdev);
#endif
    pPriv->pNdev = pNdev;
    spin_lock_init(&pPriv->lock);

    osal_strcpy(pNdev->name, intfName);

#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
    pNdev->netdev_ops = &svi_netdev_ops;
    pNdev->watchdog_timeo = SVI_TX_TIMEOUT;
    pNdev->irq = rtk_dev[SYS_DEV_ID_DEFAULT].irq;
    mac.octet[0] = 0x00;
    mac.octet[1] = 0x00;
    mac.octet[2] = 0x5e;
    mac.octet[3] = 0x00;
    mac.octet[4] = 0x01;
    mac.octet[5] = vrid+1;
	memcpy(pNdev->dev_addr, mac.octet, 6);
#else
    pNdev->open = svi_open;
    pNdev->stop = svi_close;
    pNdev->do_ioctl = svi_ioctl;
    pNdev->watchdog_timeo = SVI_TX_TIMEOUT;
    pNdev->tx_timeout = svi_tx_timeout;
    pNdev->hard_start_xmit = svi_start_xmit;
    pNdev->set_multicast_list = svi_set_multicast_list;
    pNdev->set_mac_address = svi_set_mac_address;
    pNdev->get_stats = svi_get_stats;
    pNdev->irq = rtk_dev[SYS_DEV_ID_DEFAULT].irq;
    
    mac.octet[0] = 0x00;
    mac.octet[1] = 0x00;
    mac.octet[2] = 0x5e;
    mac.octet[3] = 0x00;
    mac.octet[4] = 0x01;
    mac.octet[5] = vrid+1;
	memcpy(pNdev->dev_addr, mac.octet, 6);
#endif

    if (register_netdev(pNdev) != 0)
    {
        if (NULL != pPriv)
            osal_free(pPriv);

        free_netdev(pNdev);

        return SYS_ERR_FAILED;
    }

	if (SYS_L3_INTFTYPE_VRRP == intfType)
        gp_netdevVrrp[vrid].dev = pNdev;

    rtnl_lock();
    ifr.ifr_flags = dev_get_flags(pNdev);
    ifr.ifr_flags |= IFF_UP;
    dev_change_flags(pNdev, ifr.ifr_flags);
    rtnl_unlock();

    rtnl_lock();
    ifr.ifr_flags = dev_get_flags(pNdev);
    ifr.ifr_flags &= ~IFF_UP;
    dev_change_flags(pNdev, ifr.ifr_flags);
    rtnl_unlock();

    return SYS_ERR_OK;
}
#endif

int32 svi_init(void)
{
    drv_nic_initCfg_t initCfg;

    initCfg.pkt_alloc = _svi_pkt_alloc;
    initCfg.pkt_free = _svi_pkt_free;
    initCfg.pkt_size = SVI_MAX_PKTLEN;

    if (RT_ERR_OK != drv_nic_init(SVI_UNIT_ID, &initCfg))
    {
        osal_printf("FAIL#cause: drv_nic_init() failed!!\n");

        return SYS_ERR_FAILED;
    }

    if (RT_ERR_OK != drv_nic_rx_register(SYS_DEV_ID_DEFAULT, SVI_RX_HANDLER_PRIORITY, svi_rx_callback, (void *)gp_ndev, 0))
        goto out;

    if (RT_ERR_OK != drv_nic_rx_start(SVI_UNIT_ID))
        goto out;

    return SYS_ERR_OK;

out:
    osal_printf("svi_init failed!!\n");

    return SYS_ERR_FAILED;
}

uint32 svi_exit(void)
{
    osal_printf("Exit Switch Virtual Interface Driver Module....OK\n");

    return SYS_ERR_OK;
}

svi_dev_t *svi_create(sys_mac_t *pMac)
{
    int32 ret = -1;
#ifdef CONFIG_SYS_L3_ROUTE
    uint32 i = 0;
#endif

    if (NULL == (gp_svi = osal_alloc(sizeof(svi_dev_t))))
        return NULL;

    osal_memcpy(&g_sysMac, pMac, sizeof(sys_mac_t));

    ret = _netdevCreate(SYS_L3_INTFTYPE_MGMTVLAN, 0);

    if (SYS_ERR_OK != ret)
        goto fail;

#ifdef CONFIG_SYS_L3_ROUTE
    if (NULL == (gp_intfVlan = osal_alloc(sizeof(svi_intf_t) * SYS_VLAN_NUM_MAX)))
        goto fail;

    if (NULL == (gp_netdevVlan = osal_alloc(sizeof(svi_netdev_t) * CAPA_L3_INTFVLAN_NUM)))
        goto fail;

    if (NULL == (gp_netdevLo = osal_alloc(sizeof(svi_netdev_t) * CAPA_L3_INTFLO_NUM)))
        goto fail;
#ifdef CONFIG_SYS_L3_VRRP
    if (NULL == (gp_netdevVrrp = osal_alloc(sizeof(svi_netdev_t) * SYS_L3_INTFTYPE_VRRP)))
        goto fail;
#endif
    osal_memset(gp_svi, 0, sizeof(svi_dev_t));
    osal_memset(gp_intfVlan, 0, (sizeof(svi_intf_t) * SYS_VLAN_NUM_MAX));
    osal_memset(gp_netdevVlan, 0, (sizeof(svi_netdev_t) * CAPA_L3_INTFVLAN_NUM));
    osal_memset(gp_netdevLo, 0, (sizeof(svi_netdev_t) * CAPA_L3_INTFLO_NUM));
#ifdef CONFIG_SYS_L3_VRRP
    osal_memset(gp_netdevVrrp, 0, (sizeof(svi_netdev_t) * SYS_L3_INTFTYPE_VRRP));
#endif
    for (i = 0; i < SYS_VLAN_NUM_MAX; i++)
    {
        gp_intfVlan[i].netdevIdx = EMPTY_NETDEV;
        gp_intfVlan[i].netdevVrrpIdx = EMPTY_NETDEV;
    }

    for (i = 0; i < CAPA_L3_INTFVLAN_NUM; i++)
    {
        ret = _netdevCreate(SYS_L3_INTFTYPE_VLAN, i);

        if (SYS_ERR_OK != ret)
            goto fail;
    }

    for (i = 0; i < CAPA_L3_INTFLO_NUM; i++)
    {
        ret = _netdevCreate(SYS_L3_INTFTYPE_LO, i);

        if (SYS_ERR_OK != ret)
            goto fail;
    }
#ifdef CONFIG_SYS_L3_VRRP
    for (i = 0; i < CAPA_L3_VRRPID_NUM; i++)
    {
        ret = _vrrpNetdevCreate(SYS_L3_INTFTYPE_VRRP, i);

        if (SYS_ERR_OK != ret)
            goto fail;
    }
#endif

#endif

    gp_svi->intf_add = svi_intf_add;
    gp_svi->intf_del = svi_intf_del;
    gp_svi->intf_up = svi_intf_up;
    gp_svi->intf_down = svi_intf_down;

    return gp_svi;

fail:
    if (NULL != gp_svi)
        osal_free(gp_svi);

#ifdef CONFIG_SYS_L3_ROUTE
    if (NULL != gp_intfVlan)
        osal_free(gp_intfVlan);

    if (NULL != gp_netdevVlan)
        osal_free(gp_netdevVlan);

    if (NULL != gp_netdevLo)
        osal_free(gp_netdevLo);

#ifdef CONFIG_SYS_L3_VRRP
    if (NULL != gp_netdevVrrp)
        osal_free(gp_netdevVrrp);
#endif
#endif

    return NULL;
}

#ifdef CONFIG_SYS_L3_VRRP

svi_dev_t *svi_vrrp_create(sys_mac_t mac, int vrid)
{

    struct net_device *pNdev = NULL;
    svi_priv_t *pPriv = NULL;
	struct ifreq ifr;
	char intfName[SYS_BUF16_LEN];

    SYS_MEM_CLEAR(ifr);
    SYS_MEM_CLEAR(intfName);

    if (NULL == (gp_svi_vrrp = osal_alloc(sizeof(svi_dev_t) * SYS_VRRPID_NUM_MAX)))
        return NULL;

	osal_memset(gp_svi_vrrp, 0, (sizeof(svi_dev_t) * SYS_VRRPID_NUM_MAX));

	osal_sprintf(intfName, "vrrp.%d", vrid);
    pNdev = alloc_etherdev(sizeof(svi_priv_t));
    if (NULL == pNdev)
        return NULL;

    pPriv = netdev_priv(pNdev);
    osal_memset(pPriv, 0, sizeof(svi_priv_t));
    pPriv->pNdev = pNdev;
    spin_lock_init(&pPriv->lock);

    osal_strcpy(pNdev->name, intfName);

#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
	pNdev->netdev_ops = &svi_netdev_ops;
	pNdev->watchdog_timeo = SVI_TX_TIMEOUT;
	pNdev->irq = rtk_dev[SYS_DEV_ID_DEFAULT].irq;
	memcpy(pNdev->dev_addr, mac.octet, 6);
#endif

    if (register_netdev(pNdev) != 0)
    {
        if (NULL != pPriv)
            osal_free(pPriv);

        free_netdev(pNdev);

        return NULL;
    }

    rtnl_lock();
    ifr.ifr_flags = dev_get_flags(pNdev);
    ifr.ifr_flags |= IFF_UP;
    dev_change_flags(pNdev, ifr.ifr_flags);
    rtnl_unlock();

    rtnl_lock();
    ifr.ifr_flags = dev_get_flags(pNdev);
    ifr.ifr_flags &= ~IFF_UP;
    dev_change_flags(pNdev, ifr.ifr_flags);
    rtnl_unlock();

    gp_svi_vrrp[vrid].intf_add = svi_intf_add;
    gp_svi_vrrp[vrid].intf_del = svi_intf_del;
    gp_svi_vrrp[vrid].intf_up = svi_intf_up;
    gp_svi_vrrp[vrid].intf_down = svi_intf_down;

    return &gp_svi_vrrp[vrid];
}
#endif

EXPORT_SYMBOL(svi_create);
EXPORT_SYMBOL(svi_handler_register);
EXPORT_SYMBOL(svi_handler_unregister);
#ifdef CONFIG_SYS_L3_VRRP
EXPORT_SYMBOL(svi_vrrp_create);
#endif

