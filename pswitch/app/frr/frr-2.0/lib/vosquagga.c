/*******************************************************************************
Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd. 
文件名称: vosquagga.c
描述: 在本文件中完成将vos的消息处理融进quagga的pthread(select)机制的操作
修订记录：
修订1：
*******************************************************************************/
/**********************************头文件**************************************/
#include <libipc/vos.h>
#include <libipc/vosMsg.h>
//#include <h/vos/vosPrint.h>
//#include <h/vos/vosDebug.h>

//#include <h/switch/smd/smdVDModule.h>
#include <libipc/smdMsgRequest.h>

#include <libipc/quaggaTask.h>
#include <zebra.h>

#include "thread.h"
#include "vosquagga.h"
#include "sockunion.h"
#include "memory.h"
#include "buffer.h"


/********************************外部声明**************************************/
extern int g_vosFdProc;
extern int g_vosFdProcHigh;

/**********************************数据****************************************/
struct thread_master *g_pQuaggaVosMaster = NULL;
int g_iQuaggaVosHandle = 0;

/* Master of the threads. */
static struct thread_master *smd_master;  

DEFINE_MTYPE_STATIC(LIB, SMD,         "SMD")

/************************************函数**************************************/

/* Allocate new smd struct. */
struct smd *
smd_new ()
{
  struct smd *new = XCALLOC (MTYPE_SMD, sizeof (struct smd));
  new->obuf = buffer_new(0);	/* Use default buffer size. */
  new->buf = XCALLOC (MTYPE_SMD, SMD_READ_BUFSIZ);
  new->max = SMD_READ_BUFSIZ;

  printf("\r\n %s %d\n", __func__, __LINE__);
 
  return new;
}

/* Read data via smd socket. */
static int
smd_read (struct thread *thread)
{
  int iRet = 0;
  int i;
  int nbytes;
  unsigned char buf[SMD_READ_BUFSIZ];
  SMD_MSG            *pSmdMsg;
  VOS_MSG_ADDR        clientAddr;

  int smd_sock = THREAD_FD (thread);
  struct smd *smd = THREAD_ARG (thread);
  smd->t_read = NULL;

  /* Read raw data from socket */
  iRet = vosMsgRecv(smd->fd, buf, SMD_READ_BUFSIZ, VOS_TIMEOUT_NOWAIT, &clientAddr);
  
  if (iRet < 0)
  {
      smd_event (SMD_READ, smd_sock, smd);
      return 0;
	  //smd->monitor = 0; /* disable monitoring to avoid infinite recursion */
	  //zlog_warn("%s: read error on vty client fd %d, closing: %s", __func__, smd->fd, safe_strerror(errno));
	  //buffer_reset(smd->obuf);
  }
  else
  {
	/*此处要做响应处理*/
	pSmdMsg = (SMD_MSG *)buf;
	switch ( pSmdMsg->usType )
    {
        case SMD_MSG_TYPE_APP:
            //iRet = ripFrrWrapProcess(pSmdMsg);
            break;
        
        default:
            printf("\r\n%s:vosMsgRecv receive unkown msg %d", __func__, pSmdMsg->usType);
            break;
    }
		//smd_event (SMD_WRITE, vty->wfd, vty);
		//smd_event (SMD_READ, smd_sock, smd);
	iRet = vosMsgResp(smd->fd, iRet, pSmdMsg, pSmdMsg->usArgsBufLen + sizeof(SMD_MSG), &clientAddr);
  }

  smd_event (SMD_READ, smd_sock, smd);
  return 0;
}

static void
smd_event (enum smd_event event, int sock, struct smd *smd)
{
  struct thread *smd_serv_thread;

  switch (event)
    {
    case SMD_SERV:
      //smd_serv_thread = thread_add_read (vty_master, vty_accept, vty, sock);
      //vector_set_index (Vvty_serv_thread, sock, vty_serv_thread);
      break;
    case SMD_READ:
      smd->t_read = thread_add_read (smd_master, smd_read, smd, sock);

      /* Time out treatment. */
      //if (vty->v_timeout)
	{
	 // if (vty->t_timeout)
	  //  thread_cancel (vty->t_timeout);
	 // vty->t_timeout = 
	  //  thread_add_timer (vty_master, vty_timeout, vty, vty->v_timeout);
	}
      break;
    case SMD_WRITE:
      //if (! vty->t_write)
	//vty->t_write = thread_add_write (vty_master, vty_flush, vty, sock);
      break;
    case SMD_TIMEOUT_RESET:
      //if (vty->t_timeout)
	{
	  //thread_cancel (vty->t_timeout);
	  //vty->t_timeout = NULL;
	}
      //if (vty->v_timeout)
	{
	  //vty->t_timeout = 
	  //  thread_add_timer (vty_master, vty_timeout, vty, vty->v_timeout);
	}
      break;
    }
}

/* Create new smd structure. */
static struct smd *
smd_create (int smd_sock)
{
  struct smd *smd;
  printf("\r\n %s %d\n", __func__, __LINE__);

  /* Allocate new smd structure and set up default values. */
  smd = smd_new ();
  smd->fd = smd_sock;
  smd->cp = 0;
  //vty_clear_buf (smd);
  smd->length = 0;
  //vector_set_index (vtyvec, vty_sock, vty);
  smd->v_timeout = 600;
  printf("\r\n %s %d\n", __func__, __LINE__);

  /* Add read/write thread. */
  //smd_event (SMD_WRITE, smd_sock, smd);
  smd_event (SMD_READ, smd_sock, smd);

  return smd;
}

/* Install smd's own commands like `who' command. */
void
smd_init (struct thread_master *master_thread)
{
  	smd_master = master_thread;
}

/* Determine address family to bind. */
void
smd_serv_sock (const char *path, VOS_MSG_SOURCE msgType, struct thread_master *master)
{
	int    iRet;
	int    sockFd = -1;
	iRet = vosMsgInit(path, VOS_MSG_SOURCE_PROCESS, &sockFd);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("\r\n%s:vosMsgInit failed", __func__);
        return VOS_ERR_FAIL;
    }
	printf("\r\n %s %d\n", __func__, __LINE__);

    smd_create(sockFd);
    printf("\r\n %s %d\n", __func__, __LINE__);
}

/*******************************************************************************
函数名称：quaggaVosMsgReceive
功能描述：thread_add_read的报文接收处理回调函数
输入参数：thread，线程指针
输出参数：无
函数返回：成功为0，失败为-1
其它说明：无
*******************************************************************************/
int quaggaVosMsgReceive (struct thread *thread)
{
    int                 iSocketFd, iRet;
    SMD_MSG            *pSmdMsg;
    VOS_MSG_ADDR        clientAddr;

    iSocketFd = (int)THREAD_ARG(thread);

    /*处理所有的vos消息，直到失败或者超时*/
    while ( 1 )
    {
        iRet = vosMsgRecv(g_iQuaggaVosHandle, g_smdMsgRecvBuffer, SMD_MSG_BUFFER_LEN, 
            VOS_TIMEOUT_NOWAIT, &clientAddr);
        if ( iRet <= 0 )
        {
            break;
        }

        pSmdMsg = (SMD_MSG *)g_smdMsgRecvBuffer;
        
        switch ( pSmdMsg->usType )
        {

            case SMD_MSG_TYPE_FRR:
                iRet = smdCfgRmWrapProcess(pSmdMsg);
                break;
            
            default:
                //vosPrintf("\r\n%s:vosMsgRecv receive unkown msg %d", __func__, pSmdMsg->usType);
                continue;
        }

        iRet = vosMsgResp(g_iQuaggaVosHandle, iRet, pSmdMsg, pSmdMsg->usArgsBufLen + sizeof(SMD_MSG), &clientAddr);
    }

    thread_add_read (g_pQuaggaVosMaster, quaggaVosMsgReceive, (void *)iSocketFd, iSocketFd);

    return 0;
}

/*******************************************************************************
函数名称：quaggaVosMsgInit
功能描述：功能初始化函数，quagga的相关进程调用此接口进行vos message的初始化注册操作
输入参数：name，进程名称，在quaggaTask.h中的定义
          master，进程的主线程结构指针
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int quaggaVosMsgInit(char *name, struct thread_master *master)
{
    int                 iRet;

    if ( NULL != g_pQuaggaVosMaster )
    {
        return VOS_ERR_FAIL;
    }

    //smdProcessInit(name);

    iRet = vosMsgInit(name, VOS_MSG_SOURCE_PROCESS, &g_iQuaggaVosHandle);
    if ( VOS_ERR_NONE != iRet )
    {
        //vosPrintf("\r\n%s:vosMsgInit failed", __func__);
        return VOS_ERR_FAIL;
    }

    g_pQuaggaVosMaster = master;

    //smdRMWrapInit(QUAGGA_FUNC_MAX);

    thread_add_read (g_pQuaggaVosMaster, quaggaVosMsgReceive, (void *)g_vosFdProc, g_vosFdProc);
    //thread_add_read (g_pQuaggaVosMaster, quaggaVosMsgReceive, (void *)g_vosFdProcHigh, g_vosFdProcHigh);

    return VOS_ERR_NONE;
}
