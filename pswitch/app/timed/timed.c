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
 * Purpose : Polling some events and information
 *
 * Feature : Polling some events and information
 */

/*
 * Include Files
 */
#include <signal.h>
#include <time.h>
#include <libsal/sal_util.h>
#include <libsal/sal_time.h>
#include <libsal/sal_crash.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
static  sys_time_t systime;

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */
static void msntp_change(int signo);
static void msntp_change(int signo)
{
    sal_time_get(&systime);
}

#ifdef CONFIG_SYS_APP_MSNTP
static void monitorSntp(void)
{
    if(SNTP_ENABLED == systime.sntpEnable)
    {
        system("handler -u pooltime &");
    }

    return;
}
#endif

int main(int argc, char **argv)
{
    uint32 monitorTime = 0;
    uint32 nowTime;
    time_t nowTimet;
    FILE *fp = NULL;


    /* Initialize monitor */
#ifdef CONFIG_SYS_APP_MSNTP
    signal(SIGFPE,  msntp_change);
#endif

    sal_time_get(&systime);

    fp = fopen(TIME_CURRENT, "w");
    if(fp == NULL)
    {
        perror("time init failed\n");
        exit(0);
    }

    /* Start monitor */
    while(1)
    {
        /* Check every 1 second */
        sal_time_currentTimeInt_get(&nowTime);
        nowTimet = nowTime;
        rewind(fp);
        fwrite(&nowTimet, sizeof(time_t), 1, fp);
        fflush(fp);

        /* Check every 30 seconds */
        if (0 == (monitorTime % 30))
        {
#ifdef CONFIG_SYS_APP_MSNTP
            monitorSntp();
#endif
        }

        monitorTime++;
        sleep(1);
    }

    return SYS_ERR_OK;
}

