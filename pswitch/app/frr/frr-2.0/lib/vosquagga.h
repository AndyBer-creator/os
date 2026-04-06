/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 文件名称: vosquagga.h
 描述: 在本文件中完成将vos的消息处理融进quagga的pthread(select)机制的操作
 修订记录：
 修订1：
*******************************************************************************/
#ifndef _INCLUDE_VOSQUAGGA_H_
#define _INCLUDE_VOSQUAGGA_H_
/**********************************头文件**************************************/
#include <libipc/vos.h>
#include <libipc/vosProcess.h>

/**********************************数据结构************************************/
/* smd read buffer size. */
#define SMD_READ_BUFSIZ 1024



/* smd struct. */
struct smd 
{
  /* File descripter of this vty. */
  int fd;

  /* Output buffer. */
  struct buffer *obuf;

  /* Command input buffer */
  char *buf;

  /* Command cursor point */
  int cp;

  /* Command length */
  int length;

  /* Command max length. */
  int max;

  /* Read and write thread. */
  struct thread *t_read;
  struct thread *t_write;

  /* Timeout seconds and thread. */
  unsigned long v_timeout;
  struct thread *t_timeout;

  /* What address is this vty comming from. */
  //char address[SU_ADDRSTRLEN];
  //VOS_MSG_ADDR *clientAddr;
};

/* smd events */
enum smd_event
{
  SMD_SERV,
  SMD_READ,
  SMD_WRITE,
  SMD_TIMEOUT_RESET,
};

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
int quaggaVosMsgInit(char *name, struct thread_master *master);
void smd_init (struct thread_master *master_thread);
int quaggaVosMsgReceive (struct thread *thread);
static void smd_event (enum smd_event event, int sock, struct smd *smd);

#endif /*_INCLUDE_VOSQUAGGA_H_*/

