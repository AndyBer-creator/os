
#include <rsd/rsd_nic.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_port.h>
#include <igmp/proto_igmp.h>
#include <igmp/proto_igmp_tx.h>

extern osal_mutex_t igmp_sem_pkt;
int32 mcast_snooping_tx(sys_nic_pkt_t * pkt, uint16 vid, uint32 length, sys_logic_portmask_t * portmask)
{
    sys_nic_pkt_t *new_pkt;

    IGMP_PKT_SEM_LOCK();

    if (SYS_ERR_OK != rsd_nic_pkt_alloc(length, &new_pkt))
    {
        MCASTDBG("mcast_snooping_tx(): Failed to allocate memory for tagged pkt");
        IGMP_PKT_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    osal_memcpy(new_pkt->data, pkt->data, length);
    new_pkt->tail += length;
    new_pkt->length = length;
#if defined(CONFIG_SDK_RTL8380) || defined(CONFIG_SDK_RTL8390)
    new_pkt->tx_tag.l2_learning = 0;
#endif
    if(SYS_ERR_OK != rsd_nic_pkt_tx(new_pkt, portmask, SYS_CPU_PORT_PRIORITY_IGMP, vid,RSD_NIC_TX_CARE_STP | RSD_NIC_TX_CARE_VLAN | RSD_NIC_TX_CARE_TRUNK | RSD_NIC_TX_CARE_DOT1X))
    {
        MCASTDBG("%s():%d mcast_snooping_tx failure!", __FUNCTION__, __LINE__);
    }

    IGMP_PKT_SEM_UNLOCK();

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_MVR
int32 mcast_mvr_tx(sys_nic_pkt_t * pkt, sys_vid_t vlanId,uint32 length, sys_logic_portmask_t * portmask)
{
    sys_nic_pkt_t *new_pkt;
    uint16 tagLen = 0;
    int32 ret = 0;

    IGMP_PKT_SEM_LOCK();
    if (SYS_ERR_OK != rsd_nic_pkt_alloc(length, &new_pkt))
    {
        MCASTDBG("mcast_mvr_tx(): Failed to allocate memory for tagged pkt");
        IGMP_PKT_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    if (TRUE == pkt->rx_tag.cvid_tagged)
        tagLen = tagLen + 4;

    if (TRUE == pkt->rx_tag.svid_tagged)
        tagLen = tagLen + 4;

    osal_memcpy(new_pkt->data, pkt->data, 12);
    osal_memcpy(new_pkt->data + 12, pkt->data + 12 + tagLen, length - 12 - tagLen);
    new_pkt->tail += length - tagLen;
    new_pkt->length = length - tagLen;
#if defined(CONFIG_SDK_RTL8380) || defined(CONFIG_SDK_RTL8390)
    new_pkt->tx_tag.l2_learning = 0;
#endif

    ret = rsd_nic_pkt_tx(new_pkt, portmask, SYS_CPU_PORT_PRIORITY_IGMP, vlanId, RSD_NIC_TX_CARE_STP|RSD_NIC_TX_CARE_TRUNK | RSD_NIC_TX_CARE_DOT1X);
    if (SYS_ERR_OK != ret)
    {
        MCASTDBG("%s():%d mcast_mvr_tx failure!", __FUNCTION__, __LINE__);
    }

    IGMP_PKT_SEM_UNLOCK();
    return SYS_ERR_OK;
}
#endif

