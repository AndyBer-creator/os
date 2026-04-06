
#ifndef _INCLUDE_VOSSYSTEM_H_
#define _INCLUDE_VOSSYSTEM_H_

#ifdef MAKE_KERNEL_MODULE
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#else
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#endif
#include <libipc/vosPrint.h>
#ifndef MAKE_KERNEL_MODULE
#include <libipc/vosProcess.h>
#endif

#ifdef MAKE_KERNEL_MODULE
#define ASSERTSTOP() BUG()
#define PROCNAME() current->comm
#define PROCID() current->pid
#else
#define ASSERTSTOP() abort()
#define PROCNAME() vosProcessGetName()
#define PROCID() getpid()
#endif

#define vosAssert(expr) \
    if ( !(expr) ) \
    { \
        vosPrintf("\r\n%s(%d):Assertion failed!%s, %s, %s, line=%d.\r\n", \
            PROCNAME(), PROCID(), #expr, __FILE__, __FUNCTION__, __LINE__); \
        ASSERTSTOP(); \
    }

typedef enum LANG_TYPE_tag{
    LANG_ENGLISH = 0, 
    LANG_CHINESS
}LANG_TYPE;

#define TIME_BUFF_LEN 100

typedef enum VOS_SEM_VALUE_tag{
    VOS_SEM_EMPTY = 0,
    VOS_SEM_FULL = 1
}VOS_SEM_VALUE;

#ifdef MAKE_KERNEL_MODULE

typedef atomic_t vosAtomic_t;

typedef spinlock_t * vosLock_t;

typedef struct semaphore * vosSem_t;

typedef struct completion * vosWait_t;

typedef void (*vosTimerFun)(void *pData);

typedef struct {
    vosTimerFun                process; 
    void                      *pData;   
    int                        iMsTime; 
    BOOL                       bAlways; 
    struct timer_list         timer; 
}VOS_TIMER_STC;

typedef unsigned long vos_jiffies_t;

#define VOS_JIFFIES_ZERO(x) x = 0

#define VOS_JIFFIES_AFTER_ONE_SEC(x) x = (jiffies + (1 * HZ))
#define VOS_JIFFIES_AFTER_TWO_SEC(x) x = (jiffies + (2 * HZ))

#define VOS_JIFFIES_AFTER_USECS(x, usecs) x = (jiffies + usecs_to_jiffies(usecs))

#define VOS_JIFFIES_AFTER_EQ(x) time_after_eq(jiffies, x)

#else

typedef enum {
    VOS_SEM_TYEP_B, 
    VOS_SEM_TYEP_M  
}VOS_SEM_TYEP_ENT;

typedef struct {
    VOS_SEM_TYEP_ENT      type;
    sem_t                 semB;
    pthread_mutex_t       semM;
}vosPSem_t;

typedef vosPSem_t * vosSem_t;

typedef vosPSem_t * vosLock_t; 

typedef struct {
    int                   iCount;
    pthread_mutex_t       mutex;
    pthread_cond_t        cond;
}vosPWait_t;

typedef vosPWait_t * vosWait_t;

typedef struct {
    int              iValue;
    vosSem_t         sem;
}vosAtomic_t;

#define MSEC_PER_SEC    1000L
#define USEC_PER_MSEC   1000L
#define NSEC_PER_USEC   1000L
#define NSEC_PER_MSEC   1000000L
#define USEC_PER_SEC    1000000L
#define NSEC_PER_SEC    1000000000L

typedef struct timespec vos_jiffies_t;

#define VOS_JIFFIES_ZERO(x) memset(&x, 0, sizeof(vos_jiffies_t))

#define VOS_JIFFIES_AFTER_ONE_SEC(x) vosJiffiesAfterUsecs(&x, USEC_PER_SEC)
#define VOS_JIFFIES_AFTER_TWO_SEC(x) vosJiffiesAfterUsecs(&x, 2 * USEC_PER_SEC)

#define VOS_JIFFIES_AFTER_USECS(x, usecs) vosJiffiesAfterUsecs(&x, usecs)

#define VOS_JIFFIES_AFTER_EQ(x) vosTimeAterEq(x)

typedef enum {
    START_TYPE_COLD,
    START_TYPE_WARM
}START_TYPE_ENT;

typedef void (*vosTimerFun)(void *pData);

typedef struct {
    vosTimerFun                process; 
    void                      *pData;   
    int                        iMsTime; 
    ulong_t                    bAlways; 
    timer_t                    timerId; 
}VOS_TIMER_STC;

#endif

void vosSleep(ulong_t ulSecs);

void vosUSleep(ulong_t uluSecs);

int vosGetTimeOfDay(struct timeval *tv);

int vosHostNameGet(char *name, int len);

int vosHostNameSet(char *name, int len);

int vosUpTime(unsigned long *pulSecs, unsigned long *pulTicks);

ulong_t vosUpSeconds(void);

#ifndef MAKE_KERNEL_MODULE

time_t vosTimeGet(time_t *pTime);

int vosTimeSet(time_t *pTime);
#endif

int vosClockOsToHw(void);

int vosClockHwToOs(void);

#ifndef MAKE_KERNEL_MODULE

int vosClockHwGet(struct tm *pTime);

unsigned long long tickGet(void);

ulong_t sysClkRateGet(void);
#endif

int vosMemoryTotalGet(void);

int vosMemoryFreeGet(void);

int vosCpuIdleGet(void);

#ifdef GLN_SYSTEM_LINUX

int errnoSet(int errnoValue);

int errnoGet(void);
#endif

void vosErrnoToStr(int errnoValue, char *buf, int iBufLen);

void vosShell(void);

void vosReboot(ulong_t bSoftReboot);

int vosBuildTimeGet(LANG_TYPE type, char szTime[TIME_BUFF_LEN], int iLen);

int vosSystem(char *command, ulong_t bCtrlC);

int vosTerminalCtrlClear(void);

int vosAtomicInit(vosAtomic_t *pAtomic, int iValue);

void vosAtomicSet(vosAtomic_t *pAtomic, int iValue);

int vosAtomicRead(vosAtomic_t *pAtomic);

void vosAtomicInc(vosAtomic_t *pAtomic);

ulong_t vosAtomicIncAndTest(vosAtomic_t *pAtomic);

void vosAtomicDec(vosAtomic_t *pAtomic);

ulong_t vosAtomicDecAndTest(vosAtomic_t *pAtomic);

vosLock_t vosLockCreate(void);

int vosLockTake(vosLock_t lock, int timeout);

int vosLockGive(vosLock_t lock);

int vosLockIrqTake(vosLock_t lock, unsigned long *pulIrq, int timeout);

int vosLockIrqGive(vosLock_t lock, unsigned long ulIrq);

int vosLockDelete(vosLock_t lock);

vosSem_t vosSemMCreate(VOS_SEM_VALUE defValue);

int vosSemDelete(vosSem_t sem);

int vosSemTake(vosSem_t sem, int timeout);

int vosSemGive(vosSem_t sem);

vosWait_t vosWaitCreate(void);

int vosWaitDelete(void *wait);

int vosWaitTake(vosWait_t wait, int timeout);

int vosWaitGive(vosWait_t wait);

#ifdef MAKE_KERNEL_MODULE

unsigned long vosIntLock(void);

void vosIntUnLock(unsigned long ulIrqFlags);

#else 

vosSem_t vosSemBCreate(VOS_SEM_VALUE defValue);

int vosIfNameToKernelIndex(char *ifName, ulong_t *pulIfKernelIndex);

char *vosIfKernelIndexToName(ulong_t ulIfKernelIndex, char *ifName);

int vosStartTypeGet(START_TYPE_ENT *pType);

static inline void vosTvsub(struct timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0) {
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

int vosJiffiesAfterUsecs(vos_jiffies_t *pJiffies, ulong_t ulUSecs);

int vosTimeAterEq(vos_jiffies_t jiffies);

#endif 

int vosTimerAdd(VOS_TIMER_STC *pTimer);

int vosTimerDel(VOS_TIMER_STC *pTimer);

int vosInInterrupt(void);

#endif 

