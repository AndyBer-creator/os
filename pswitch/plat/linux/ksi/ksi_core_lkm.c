
#include <ksi/ksi_core.h>
#include <osal/memory.h>
#include <osal/sem.h>
#include <linux/jiffies.h>

#include <net/sock.h>
#include <net/netlink.h>
#include <linux/skbuff.h>
#include <linux/inet.h>

static struct sock    *netlink_sock;
static int32          ksid_pid = 0;
static uint32         _ksi_seq = 0;
static osal_mutex_t   _ksi_sem = 0;
static ksi_msg_list_t *gKsiMsgListHead = NULL;

#define KSI_SEM_LOCK()    \
do {\
    if (osal_sem_mutex_take(_ksi_sem, OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
    {\
        SYS_PRINTF("%s:%d sem mutex lock fail", __func__, __LINE__);\
        return RT_ERR_SEM_LOCK_FAILED;\
    }\
} while(0)
#define KSI_SEM_UNLOCK()   \
do {\
    if (osal_sem_mutex_give(_ksi_sem) != RT_ERR_OK)\
    {\
        SYS_PRINTF("%s:%d sem mutex unlock fail", __func__, __LINE__);\
        return RT_ERR_SEM_UNLOCK_FAILED;\
    }\
} while(0)

static int32 ksi_msg_send(ksi_msg_t *pMsg);

int32 ksi_msg_notify(ksi_msg_t *pMsg)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(NULL == pMsg, SYS_ERR_NULL_POINTER);

    KSI_SEM_LOCK();

    ret = ksi_msg_send(pMsg);

    KSI_SEM_UNLOCK();

    return ret;
}

int32 ksi_msg_add(ksi_msg_t *pMsg)
{
    ksi_msg_list_t *pMsgListNew;
    ksi_msg_list_t *pMsgList;
    ksi_msg_list_t *pMsgListLast;
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(NULL == pMsg, SYS_ERR_NULL_POINTER);

    KSI_SEM_LOCK();
    if ((pMsgListNew = osal_alloc(sizeof(ksi_msg_list_t))) != NULL)
    {
        pMsgListNew->msg = pMsg;

        if (NULL == gKsiMsgListHead)
        {
            pMsgListNew->next = NULL;
            gKsiMsgListHead = pMsgListNew;
        }
        else
        {
            pMsgListLast = gKsiMsgListHead;
            pMsgList = gKsiMsgListHead;
            while ((pMsgList != NULL))
            {
                pMsgListLast = pMsgList;
                pMsgList = pMsgList->next;
            }

            pMsgListNew->next = pMsgListLast->next;
            pMsgListLast->next = pMsgListNew;
        }

        ret = ksi_msg_send(pMsg);

    }
    else
    {
        SYS_PRINTF("%s():%d - debug(Out of memory)\n", __FUNCTION__, __LINE__);

        ret = SYS_ERR_OUT_OF_MEMORY;  
    }
    KSI_SEM_UNLOCK();

    return ret;
}

int32 ksi_msg_del(ksi_msg_t *pMsg)
{
    ksi_msg_list_t *pMsgList = NULL;
    ksi_msg_list_t *pMsgListLast = NULL;

    SYS_PARAM_CHK(NULL == pMsg, SYS_ERR_NULL_POINTER);

    KSI_SEM_LOCK();
    pMsgList = gKsiMsgListHead;
    while (pMsgList != NULL)
    {
        if (pMsgList->msg->id == pMsg->id)
        {
            if (pMsgList->msg->tousr_data != NULL)
            {
                osal_free(pMsgList->msg->tousr_data);
            }

            if (pMsgList->msg->tokrn_data != NULL)
            {
                osal_free(pMsgList->msg->tokrn_data);
            }

            if (pMsgList == gKsiMsgListHead)
                gKsiMsgListHead = pMsgList->next;
            else
                pMsgListLast->next = pMsgList->next;

            osal_free(pMsgList);
            KSI_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        pMsgListLast = pMsgList;
        pMsgList = pMsgList->next;
    }
    KSI_SEM_UNLOCK();

    return SYS_ERR_ENTRY_NOT_FOUND; 
}

static int32 _ksi_msg_send(ksi_hdr_t hdr, uint32 seq, uint16 datalen, void *payload)
{
    struct sk_buff  *skb;
    struct nlmsghdr *nlh;
    uint32          size = 0;
    uint32          retryNum = 0;
    uint32          sleepTime = 0;
    int32 ret;

    size = datalen + sizeof(ksi_hdr_t);

retry:
    skb = alloc_skb(NLMSG_SPACE(size), GFP_ATOMIC);
    if (!skb)
        return SYS_ERR_FAILED;

#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
    if (skb_tailroom(skb) < (int)NLMSG_SPACE(size))
        goto nlmsg_failure;
    nlh = nlmsg_put(skb, ksid_pid, seq, 0, size, 0);
#else
    nlh = NLMSG_PUT(skb, ksid_pid, seq, 0, size);
#endif
    nlh->nlmsg_flags = 0;

    memcpy(NLMSG_DATA(nlh), &hdr, sizeof(ksi_hdr_t));
    if (payload != NULL)
        memcpy(NLMSG_DATA(nlh) + sizeof(ksi_hdr_t), payload, datalen);

#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
    NETLINK_CB(skb).portid = 0;        
#else
    NETLINK_CB(skb).pid = 0;        
#endif
    NETLINK_CB(skb).dst_group = 0;  

    ret = netlink_unicast(netlink_sock, skb, ksid_pid, MSG_DONTWAIT);

    if (ret < 0)
    {
        if (retryNum < 50)
        {
            retryNum++;
            sleepTime = (retryNum * 10000);
            osal_time_usleep(sleepTime);
            goto retry;
        }

        SYS_PRINTF("send failed ret=%d ksid_pid=%d\n", ret, ksid_pid);
        return SYS_ERR_OK;
    }
    return SYS_ERR_OK;

nlmsg_failure:          
    if (skb)
        kfree_skb(skb);

    return SYS_ERR_FAILED;
}

static int32 ksi_msg_send(ksi_msg_t *pMsg)
{
    uint32 leftBytes = pMsg->hdr.tousr_totlen;
    uint32 sendBytes = 0;
    uint32 offset = 0;

    if (ksid_pid <= 0)
        return SYS_ERR_FAILED;

    pMsg->id = _ksi_seq;
    _ksi_seq++;

    if (leftBytes == 0)
    {
        if (pMsg->hdr.msgtype == KSI_MSGTYPE_SEND)
            pMsg->hdr.msgtype = KSI_MSGTYPE_SEND_FIN;

        _ksi_msg_send(pMsg->hdr, pMsg->id, 0, NULL);
    }
    else
    {
        while (leftBytes > 0)
        {
            if (leftBytes > KSI_MAX_PAYLOAD)
            {
                sendBytes = KSI_MAX_PAYLOAD;
                pMsg->hdr.msgtype = KSI_MSGTYPE_SEND;
            }
            else
            {
                sendBytes = leftBytes;
                pMsg->hdr.msgtype = KSI_MSGTYPE_SEND_FIN;
            }

            pMsg->hdr.curr_offset = offset;
            pMsg->hdr.curr_len = sendBytes;
            _ksi_msg_send(pMsg->hdr, pMsg->id, sendBytes, pMsg->tousr_data + offset);

            offset += sendBytes;
            leftBytes -= sendBytes;
        }
    }

    return SYS_ERR_OK;
}

static int32 ksi_msg_receive_handler(uint32 seq, ksi_hdr_t *hdr, void *data)
{
    ksi_msg_list_t *pMsgList = NULL;

    KSI_SEM_LOCK();
    pMsgList = gKsiMsgListHead;
    while(pMsgList != NULL)
    {
        if (pMsgList->msg->id == seq)
        {
            if (pMsgList->msg->tokrn_data == NULL)
            {
                pMsgList->msg->hdr.tokrn_totlen = hdr->tokrn_totlen;
                pMsgList->msg->tokrn_data = osal_alloc(hdr->tokrn_totlen);
                osal_memset(pMsgList->msg->tokrn_data, 0, hdr->tokrn_totlen);
            }

            osal_memcpy(pMsgList->msg->tokrn_data + hdr->curr_offset, (uint8*)data, hdr->curr_len);
            pMsgList->msg->hdr.msgtype = hdr->msgtype;
            break;
        }

        pMsgList = pMsgList->next;
    }
    KSI_SEM_UNLOCK();

    return SYS_ERR_OK;
}

static int32 _ksi_msg_receive(struct sk_buff *skb, struct nlmsghdr *nlh)
{
    void   *data;
    int    ret = SYS_ERR_OK;
    ksi_hdr_t *ksihdr;

    ksihdr = (ksi_hdr_t*)NLMSG_DATA(nlh);
    data = NLMSG_DATA(nlh) + sizeof(ksi_hdr_t);

    if (KSI_MSGTYPE_INIT == ksihdr->msgtype)
        ksid_pid = NETLINK_CREDS(skb)->pid;

    ret = ksi_msg_receive_handler(nlh->nlmsg_seq, ksihdr, data);

    return ret;
}

#ifdef CONFIG_SYS_KERNEL_LINUX_2_6_X
static void _ksi_receiver(struct sock *sk, int length)
{
    struct sk_buff  *skb;
    uint32          qlen;
    struct nlmsghdr *nlh;
    uint32          rlen;

    for (qlen = skb_queue_len(&sk->sk_receive_queue); qlen; qlen--)
    {
        skb = skb_dequeue(&sk->sk_receive_queue);

        while (skb->len >= NLMSG_SPACE(0))
        {
            nlh = (struct nlmsghdr *)skb->data;
            if (nlh->nlmsg_len < sizeof(*nlh) || skb->len < nlh->nlmsg_len)
                break;
            rlen = NLMSG_ALIGN(nlh->nlmsg_len);

            if (rlen > skb->len)
                rlen = skb->len;
            if (_ksi_msg_receive(skb, nlh) != SYS_ERR_OK)
                netlink_ack(skb, nlh, 0);
            else if (nlh->nlmsg_flags & NLM_F_ACK)
                netlink_ack(skb, nlh, 0);
            skb_pull(skb, rlen);
        }

        kfree_skb(skb);
    }
}
#else
static void _ksi_receiver(struct sk_buff *skb)
{
    struct nlmsghdr *nlh;
    uint32          rlen;

    if (NULL == skb)
        return;

    while (skb->len >= NLMSG_SPACE(0))
    {
        nlh = (struct nlmsghdr *)skb->data;
        if (nlh->nlmsg_len < sizeof(*nlh) || skb->len < nlh->nlmsg_len)
            break;
        rlen = NLMSG_ALIGN(nlh->nlmsg_len);

        if (rlen > skb->len)
            rlen = skb->len;
        if (_ksi_msg_receive(skb, nlh) != SYS_ERR_OK)
            netlink_ack(skb, nlh, 0);
        else if (nlh->nlmsg_flags & NLM_F_ACK)
            netlink_ack(skb, nlh, 0);
        skb_pull(skb, rlen);
    }

    return;
}
#endif

static int32 __init ksi_core_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init KSI Core Driver Module....");

#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
    {
        struct netlink_kernel_cfg cfg = {
            .input = (void *)_ksi_receiver,
        };
        netlink_sock = netlink_kernel_create(&init_net, NETLINK_USERSOCK, &cfg);
    }
#else
    netlink_sock = netlink_kernel_create(NETLINK_USERSOCK, 0, _ksi_receiver, THIS_MODULE);
#endif

    _ksi_sem = osal_sem_mutex_create();
    if (0 == _ksi_sem)
    {
        BOOTMSG_PRINTF("semaphore create failed");
        goto exit;
    }

    ret = SYS_ERR_OK;

exit:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init KSI Core Driver Module....FAILED\n");

    return ret;
}

static void __exit ksi_core_exit(void)
{
    sock_release(netlink_sock->sk_socket);
    osal_printf("Exit KSI Core Driver Module....OK");
}

module_init(ksi_core_init);
module_exit(ksi_core_exit);

MODULE_DESCRIPTION ("Switch KSI Core Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

