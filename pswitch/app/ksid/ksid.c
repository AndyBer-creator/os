/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 16227 $
 * $Date: 2011-03-10 18:23:52 +0800 (Thu, 10 Mar 2011) $
 *
 * Purpose : Kernel and Systerm communication interface user space daemon
 *
 * Feature : Kernel and Systerm communication interface user space daemon
 */

/*
 * Include Files
 */

#include <common/sys_util.h>
#include <ksi/ksi_core.h>
#include <ksi_core_usr.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <malloc.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/sem.h>
#include <linux/netlink.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include <ksi_init.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_crash.h>

/*
 * Symbol Definition
 */
#define MAX_PAYLOAD 1056

/*
 * Data Declaration
 */
struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;
static ksi_msg_list_t *gKsiMsgListHead = NULL;

struct sembuf ksid_lock = {0, -1, SEM_UNDO};
struct sembuf ksid_unlock = {0, 1, SEM_UNDO};
int32 glb_ksid_semid = -1;
/* Semaphore Union */
union semun
{
    int             val;
    struct semid_ds *buf;
    unsigned short  *array;
    struct seminfo  *__buf;
};

ksi_mapper_t mapper[KSI_MOD_END];

/*
 * Macro Definition
 */
#define KSI_MODID(cmd)       ((cmd) / KSI_MODCMD_TOTAL)
#define KSI_USR_SEM_LOCK()   _ksi_sem_lock()
#define KSI_USR_SEM_UNLOCK() _ksi_sem_unlock()

/*
 * Function Declaration
 */
static int32 _ksi_sem_create(void);
static int32 _ksi_sem_lock(void);
static int32 _ksi_sem_unlock(void);
static int32 _ksi_msg_add(ksi_msg_t *pMsg);
static int32 _ksi_msg_del(ksi_msg_t *pMsg);
static int32 _ksi_msg_executor(uint32 cmd, ksi_msg_t *pMsg);


/* Function Name:
 *      ksi_handler_register
 * Description:
 *      Register message process callback function for specific module
 * Input:
 *      modId     - id of moudule
 *      fCallback - call back function
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 * Note:
 *      None
 */
int32 ksi_handler_register(int modId, int (*fCallback)(unsigned int cmd, ksi_msg_t *pMsg))
{
    /* parameter check */
    SYS_PARAM_LOG((modId >= KSI_MOD_END), SYS_ERR_FAILED, "ksi handler: register failed\n");
    SYS_PARAM_CHK((NULL == fCallback), SYS_ERR_NULL_POINTER);

    /* register callback of module */
    mapper[modId].no = modId;
    mapper[modId].fCallback = fCallback;

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_msg_reply
 * Description:
 *      Reply with data to specific message coming from kernel
 * Input:
 *      pMsg    - pointer of reply message
 *      datalen - length of reply data
 *      data    - pointer of reply data
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 * Note:
 *      None
 */
int32 ksi_msg_reply(ksi_msg_t *pMsg, uint32 datalen, void *data)
{
    ksi_hdr_t hdr;
    uint32    leftBytes = datalen;
    uint32    sendBytes = 0;
    uint32    offset = 0;

    SYS_PARAM_CHK(NULL == pMsg, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((datalen > 0 && (NULL == data)), SYS_ERR_NULL_POINTER);

    osal_memcpy(&hdr, &pMsg->hdr, sizeof(ksi_hdr_t));

    hdr.tokrn_totlen = datalen;
    KSI_USR_SEM_LOCK();
    while (leftBytes > 0)
    {
        if (leftBytes > KSI_MAX_PAYLOAD)
        {
            sendBytes = KSI_MAX_PAYLOAD;
            hdr.msgtype = KSI_MSGTYPE_REPLY;
        }
        else
        {
            sendBytes = leftBytes;
            hdr.msgtype = KSI_MSGTYPE_REPLY_FIN;
        }

        hdr.curr_offset = offset;
        hdr.curr_len = sendBytes;

        osal_memset(NLMSG_DATA(nlh), 0, NLMSG_SPACE(KSI_MAX_PAYLOAD));
        osal_memcpy(NLMSG_DATA(nlh), &hdr, sizeof(ksi_hdr_t));
        osal_memcpy(NLMSG_DATA(nlh) + sizeof(ksi_hdr_t), data + offset, sendBytes);

        if(-1 == (sendmsg(sock_fd, &msg, 0)))
        {
            SYS_PRINTF("%s():%d - debug(sendmsg error)\n", __FUNCTION__, __LINE__);
        }

        offset += sendBytes;
        leftBytes -= sendBytes;
    }

    //_ksi_msg_del(pMsg);
    KSI_USR_SEM_UNLOCK();

    return SYS_ERR_OK;
}


static int32 _ksi_sem_create(void)
{
    union semun sem_val;
    int32 ret;

    glb_ksid_semid = semget(0xFFFFFFFF, 1, IPC_CREAT | 0777);
    if (glb_ksid_semid < 0)
    {
        SYS_PRINTF("[SEM] sem open fail error=%s\n", strerror(errno));
        return SYS_ERR_FAILED;
    }

    sem_val.val = 1;
    ret = semctl(glb_ksid_semid, 0, SETVAL, sem_val);
    if (ret < 0)
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

static int32 _ksi_sem_lock(void)
{
    int32 ret;

    if (glb_ksid_semid < 0)
        glb_ksid_semid = semget(0xFFFFFFFF, 1, IPC_CREAT | 0777);

    ret = semop(glb_ksid_semid, &ksid_lock, 1);

    if (ret < 0)
    {
        SYS_PRINTF("[SEM] sem open fail error=%s\n", strerror(errno));
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

static int32 _ksi_sem_unlock(void)
{
    int32 ret;

    if (glb_ksid_semid < 0)
        glb_ksid_semid = semget(0xFFFFFFFF, 1, IPC_CREAT | 0777);

    ret = semop(glb_ksid_semid, &ksid_unlock, 1);

    if (ret < 0)
    {
        SYS_PRINTF("[SEM] sem open fail error=%s\n", strerror(errno));
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}


static int32 _ksi_msg_add(ksi_msg_t *pMsg)
{
    ksi_msg_list_t *pMsgListNew;
    ksi_msg_list_t *pMsgList;
    ksi_msg_list_t *pMsgListLast;
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(NULL == pMsg, SYS_ERR_NULL_POINTER);

    if ((pMsgListNew = malloc(sizeof(ksi_msg_list_t))) != NULL)
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

    }
    else
    {
        SYS_PRINTF("%s():%d - debug(Out of memory)\n", __FUNCTION__, __LINE__);

        ret = SYS_ERR_OUT_OF_MEMORY;  /* Out of memory: failed */
    }

    return ret;
}

static int32 _ksi_msg_del(ksi_msg_t *pMsg)
{
    ksi_msg_list_t *pMsgList = NULL;
    ksi_msg_list_t *pMsgListLast = NULL;

    SYS_PARAM_CHK(NULL == pMsg, SYS_ERR_NULL_POINTER);

    pMsgList = gKsiMsgListHead;
    while (pMsgList != NULL)
    {
        if (pMsgList->msg == pMsg)
        {
            if (pMsgList == gKsiMsgListHead)
                gKsiMsgListHead = pMsgList->next;
            else
                pMsgListLast->next = pMsgList->next;

            osal_free(pMsgList);

            return SYS_ERR_OK;
        }

        pMsgListLast = pMsgList;
        pMsgList = pMsgList->next;
    }

    return SYS_ERR_ENTRY_NOT_FOUND; /* Not found */
}

static int32 _ksi_msg_executor(uint32 cmd, ksi_msg_t *pMsg)
{
    int32 ret = SYS_ERR_FAILED;
    if (KSI_MODID(cmd) < KSI_MOD_END)
    {
        if(NULL != mapper[KSI_MODID(cmd)].fCallback)
            ret = mapper[KSI_MODID(cmd)].fCallback(cmd, pMsg);
    }

    return ret;
}

void _ksi_execute_thread(void *pExecParams)
{
    ksi_execParam_t *pParams = (ksi_execParam_t *) pExecParams;

    /* make thread to detach state to make sure resource will be reclaimed after pthread_exit */
    pthread_detach(pthread_self());

    _ksi_msg_executor(pParams->cmd, pParams->pMsg);

    if (NULL != pParams->pMsg)
    {
        if (NULL != pParams->pMsg->tousr_data)
            osal_free(pParams->pMsg->tousr_data);

        osal_free(pParams->pMsg);
    }

    if (NULL != pExecParams)
        osal_free(pExecParams);

    pthread_exit(NULL);
}

static int32 ksi_msg_receive_handler(uint32 seq, ksi_hdr_t *hdr, void *data)
{
    int32 ret = 0;
    ksi_msg_list_t *pMsgList = gKsiMsgListHead;
    ksi_msg_t *pMsg = NULL;
    uint32 msgFound = FALSE;
    //pid_t pid = -1;
    pthread_t id;
    ksi_execParam_t *pExecParams = NULL;

    if (NULL == (pExecParams = osal_alloc(sizeof(ksi_execParam_t))))
        return SYS_ERR_OK;

    osal_memset(pExecParams, 0, sizeof(ksi_execParam_t));

    pExecParams->cmd = hdr->cmd;
    pExecParams->pMsg = NULL;

    /* NOTIFY type do not need to add to message link list */
    if (KSI_MSGTYPE_NOTIFY == hdr->msgtype)
    {
        if (0 != (ret = pthread_create(&id, NULL, (void *) _ksi_execute_thread, pExecParams)))
        {
            //printf("%s:%d Create theard fail with error %d\n", __func__, __LINE__, ret);
        }

        return SYS_ERR_OK;
    }

    /* Only SEND type will be added into msg list */
    KSI_USR_SEM_LOCK();
    while (pMsgList != NULL)
    {
        if (pMsgList->msg->id == seq)
        {
            msgFound = TRUE;
            break;
        }
        pMsgList = pMsgList->next;
    }

    if (msgFound == FALSE)
    {
        if (NULL != (pMsg = osal_alloc(sizeof(ksi_msg_t))))
        {
            osal_memset(pMsg, 0, sizeof(ksi_msg_t));
            pMsg->id = seq;
            osal_memcpy(&pMsg->hdr, hdr, sizeof(ksi_hdr_t));

            if (hdr->tousr_totlen > 0)
            {
                if (NULL != (pMsg->tousr_data = osal_alloc(pMsg->hdr.tousr_totlen)))
                {
                    osal_memcpy(pMsg->tousr_data, data, hdr->tousr_totlen);
                }
                else
                {
                    osal_free(pMsg);

                    goto _exit;
                }
            }

            if (KSI_MSGTYPE_SEND_FIN == hdr->msgtype)
            {
                pExecParams->pMsg = pMsg;

                KSI_USR_SEM_UNLOCK();

                if (0 != (ret = pthread_create(&id, NULL, (void *) _ksi_execute_thread, pExecParams)))
                {
                    //printf("%s:%d Create theard fail with error %d\n", __func__, __LINE__, ret);
                }

                return SYS_ERR_OK;
            }
            else
            {
                _ksi_msg_add(pMsg);
            }
        }
    }
    else
    {
        if (hdr->tousr_totlen > 0)
        {
            osal_memcpy(pMsgList->msg->tousr_data + hdr->curr_offset, (uint8*)data, hdr->curr_len);
        }

        if (KSI_MSGTYPE_SEND_FIN == hdr->msgtype)
        {
            pExecParams->pMsg = pMsgList->msg;

            _ksi_msg_del(pMsgList->msg);

            KSI_USR_SEM_UNLOCK();

            if (0 != (ret = pthread_create(&id, NULL, (void *) _ksi_execute_thread, pExecParams)))
            {
                //printf("%s:%d Create theard fail with error %d\n", __func__, __LINE__, ret);
            }

            return SYS_ERR_OK;
        }
    }

_exit:
    KSI_USR_SEM_UNLOCK();

    if (NULL != pExecParams)
        osal_free(pExecParams);

    return SYS_ERR_OK;
}

void ksi_core_init(void)
{
    ksi_hdr_t hdr;

    memset(&msg, 0 ,sizeof(msg));
    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();      /* self pid */
    src_addr.nl_groups = 0;          /* not in mcast groups */
    bind(sock_fd, (struct sockaddr*)&src_addr,sizeof(src_addr));

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;            /* For Linux Kernel */
    dest_addr.nl_groups = 0;         /* unicast */

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    /* Fill the netlink message header */
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();       /* self pid */
    nlh->nlmsg_flags = 0;

    /* Fill in the netlink message payload */
    osal_memset(&hdr, 0, sizeof(hdr));
    hdr.cmd = 0;
    hdr.msgtype = KSI_MSGTYPE_INIT;
    hdr.tokrn_totlen = sizeof(ksi_hdr_t);
    osal_memcpy(NLMSG_DATA(nlh), &hdr, sizeof(ksi_hdr_t));

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    _ksi_sem_create();

    /* Send a message to kernel to notify our pid */
    KSI_USR_SEM_LOCK();
    if(-1 == (sendmsg(sock_fd, &msg, 0)))
    {
        SYS_PRINTF("%s():%d - debug(sendmsg error)\n", __FUNCTION__, __LINE__);
    }
    KSI_USR_SEM_UNLOCK();
}

int main(int argc, char **argv)
{
    int32 ret = 0;


    /* Initial netlink and send message to kernel to notify our pid to kernel module */
    ksi_core_init();
    /* Create mapping of all modules */
    ksi_init();

    /* Loop forever to waiting message from kernel */
    while(1)
    {
        ksi_hdr_t *ksihdr = NULL;

        /* Waiting message from kernel */
        ret = recvmsg(sock_fd, &msg, 0);

        if (ret < 0)
        {
            //SYS_PRINTF("%s:%d recvmsg err=%d\n", __func__, __LINE__, ret);
            continue;
        }

        nlh = (struct nlmsghdr *)msg.msg_iov->iov_base;

        if (NULL == nlh)
            continue;

        /* first part of message data is our ksi header */
        ksihdr = NLMSG_DATA(nlh);

        /* process received message in receive handler */
        ksi_msg_receive_handler(nlh->nlmsg_seq, ksihdr, NLMSG_DATA(nlh)+sizeof(ksi_hdr_t));

        nlh = NULL;
    }

    close(sock_fd);

    return SYS_ERR_OK;
}

