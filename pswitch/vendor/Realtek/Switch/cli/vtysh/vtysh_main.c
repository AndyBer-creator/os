
#include <libvty/zebra.h>
#include <sys/un.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <pwd.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <libvty/version.h>
#include "getopt.h"
#include <libvty/command.h>
#include <libvty/memory.h>
#include <libcmd/cmd.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_util.h>
#ifdef CONFIG_SYS_TECH_SUPPORT
#include <libsal/sal_sys_tech.h>
#endif

#include <libfds/fds_export.h>
#include <libfds/fds_default_init.h>
#ifdef CONFIG_SYS_AAA
#include <libsal/sal_aaa.h>
#endif

#include "vtysh.h"
#include "vtysh_user.h"
#include <termios.h>
#include <common/sys_type.h>
#include <common/sys_def.h>
#include <cli/cli_main.h>

#ifdef CONFIG_SYS_UI_CLI_SESSION
#define CLI_MAX_CONNECT CONFIG_SYS_UI_CLI_SESSION
#else
#define CLI_MAX_CONNECT 2
#endif
int diagdbg_flag = DIAGDBG_OP_NONE;
int diagdbg_auth = DIAGDBG_AUTH_NONE;

char ssh_username[CAPA_SYS_USERNAME_LEN];

char *progname;

char history_file[MAXPATHLEN];

int execute_flag = 0;

static sigjmp_buf jmpbuf;

static int jmpflag = 0;

static char *line_read;

struct thread_master *master;

static int current_node = VIEW_NODE;
static int exec_cmd = FALSE;

FILE *logfile;

static void
_terminal_initial(void)
{
    struct termios t;

    tcgetattr(STDIN_FILENO, &t);

    t.c_cc[VERASE] = 0x8;

    t.c_lflag |= (ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

#if 1 
int login_idle_timeout = -1;
static struct termios oldt;
static void input_str_timeout_exit(void)
{
    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
    cli_exit();
}

static int
_getch_with_timeout ( void )
{
    int ch;
    struct termios newt;

    if (login_idle_timeout < 0)
        login_idle_timeout = (int) DFLT_CLI_LOGIN_IDLE_TIMEOUT;

    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );

    signal(SIGALRM, (void *)input_str_timeout_exit);
    alarm(login_idle_timeout);

    ch = getchar();
    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

    alarm(0);

    return ch;
}

int
input_str_with_timeout(char *buf, int size, char echo)
{
    unsigned int pos = 0;
    char c = 0;

    if (buf == NULL)
            return -1;

    while (c != '\n' && c != '\r')
    {
        c = (char)_getch_with_timeout();

        if ((c == 8) || (c == 127)) {
            if (pos > 0) {
                putc('\b', stdout);
                putc(' ', stdout);
                putc('\b', stdout);
                pos --;
            }
            continue;
        }

        if (c < 32 || c > 126)
            continue;

        if (pos < size) {
            if (echo == 0)
                putc(c, stdout);
            else if (echo > 0)
                putc(echo, stdout);

            buf[pos] = c;
            pos++;
        }
    }

    putc('\n', stdout);
    if (pos < size) {
        buf[pos] = '\0';
    }

    return pos;
}
#endif 

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
void silent_time(void)
{
    struct termios oldt, newt;
    sys_cliRetrySilent_t retrySilent;
    sys_line_cliType_t type = SYS_LINE_CLI_CONSOLE;

    if (access_flag == SYS_ACCESS_CLI_CONSOLE)
        type = SYS_LINE_CLI_CONSOLE;
    else if (access_flag == SYS_ACCESS_CLI_TELNET)
        type = SYS_LINE_CLI_TELNET;
  #ifdef CONFIG_USER_SSH_SSHD
    else if (access_flag == SYS_ACCESS_CLI_SSH)
        type = SYS_LINE_CLI_SSH;
  #endif

    SYS_MEM_CLEAR(retrySilent);
    sal_sys_cliRetrySilent_get(type, &retrySilent);

    if (retrySilent.silentTime == 0)
    {
        if (SYS_ACCESS_CLI_CONSOLE != access_flag)
            cli_exit();
        else
            return;
    }

    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    
    newt.c_iflag |= IGNCR;
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );

    sleep(retrySilent.silentTime);

    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
}
#endif 

#ifdef CONFIG_SYS_AAA
int32 login_authen (void)
{
    int32   ret = SYS_ERR_FAILED;
    char    input_username[CAPA_SYS_USERNAME_LEN + 1];
    char    input_password[CAPA_SYS_PASSWORD_LEN + 1];
    uint32  userIdx;
    int32   userPriv;
    sys_auth_type_t typePrio[CAPA_AAA_AUTHTYPE_PRI_NUM] = {0};
    uint32  i = 0;
    uint32 retryCnt = 3; 
    uint32 isInputUser = FALSE;
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    sys_line_cliType_t type = SYS_LINE_CLI_CONSOLE;
    sys_cliRetrySilent_t retrySilent;
    uint32 cfgRetryCnt = 0;
#endif 

    sal_aaa_loginAuthTypePrio_get(access_flag, typePrio);

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    if (access_flag == SYS_ACCESS_CLI_CONSOLE)
        type = SYS_LINE_CLI_CONSOLE;
    else if (access_flag == SYS_ACCESS_CLI_TELNET)
        type = SYS_LINE_CLI_TELNET;
  #ifdef CONFIG_USER_SSH_SSHD
    else if (access_flag == SYS_ACCESS_CLI_SSH)
        type = SYS_LINE_CLI_SSH;
  #endif

    SYS_MEM_CLEAR(retrySilent);
    sal_sys_cliRetrySilent_get(type, &retrySilent);

    cfgRetryCnt = retrySilent.retryNum;

    if (cfgRetryCnt == 0)
        retryCnt = 0xFFFFFFFF;
    else
        retryCnt = cfgRetryCnt;
#endif 

    while (retryCnt > 0)
    {

        if (typePrio[0] == SYS_AUTH_TYPE_NONE)
        {
            return 1;
        }

        if (typePrio[0] == SYS_AUTH_TYPE_ENABLE)
        {
            retryCnt = 3;
            while (retryCnt > 0){
                printf("Password: ");
                osal_memset(input_password, 0, sizeof(input_password));
                input_str_with_timeout(input_password, CAPA_SYS_PASSWORD_LEN, '*');
                ret = sal_aaa_localEnablePassword_authen(15, input_password);
                if (ret == SYS_ERR_OK)
                {  
                    #ifdef CONFIG_SYS_MIB_TRAP
        	        
                    sal_snmp_system_auth_success_fail_Trap_send_new(TRUE,text_access_type[type].text,input_username,currHost);
                    #endif
                    return 1;

                 }
                printf("%% Authentication Failed\n\n");
                #ifdef CONFIG_SYS_MIB_TRAP
        	   
                 sal_snmp_system_auth_success_fail_Trap_send_new(FALSE,text_access_type[type].text,input_username, currHost);
                #endif
                retryCnt--;
            }
            
            return 0;
        }

        osal_memset(currUserName, 0, sizeof(currUserName));
        osal_memset(currPassWord, 0, sizeof(currPassWord));

        for (i = 0; i < CAPA_AAA_AUTHTYPE_PRI_NUM; i++)
        {
            if (SYS_AUTH_TYPE_EMPTY == typePrio[i])
                break;

            if ((i > 0) && (typePrio[i] > SYS_AUTH_TYPE_NONE))
            {
                SYS_LOG(LOG_CAT_AAA, LOG_AAA_METHOD_NEW,
                            text_aaa_auth_method[typePrio[i]].text);
            }

            switch(typePrio[i])
            {
                case SYS_AUTH_TYPE_LOCAL:
                    userIdx = 0;
                    userPriv = -1;
                    if (currUserName[0] == '\0')
                    {
                        printf("Username: ");
                        osal_memset(input_username, 0, sizeof(input_username));
                        input_str_with_timeout(input_username, CAPA_SYS_USERNAME_LEN, 0);
                        osal_strcpy(currUserName, input_username);

                        isInputUser = TRUE;
                    }
                    
                    ret = sal_aaa_loginAuthUserAccessMethod(access_flag, input_username);
                    if (ret != SYS_ERR_OK)
                        break;
                        
                    ret = sal_aaa_localLoginUsername_authen(input_username, &userIdx);
                    if (ret != SYS_ERR_OK)
                        break;

                    if (currPassWord[0] == '\0')
                    {
                        printf("Password: ");
                        osal_memset(input_password, 0, sizeof(input_password));
                        input_str_with_timeout(input_password, CAPA_SYS_PASSWORD_LEN, '*');
                        osal_strcpy(currPassWord, input_password);
                    }
                    ret = sal_aaa_localLoginPassword_authen(userIdx, input_password, &userPriv);

                    if (ret != SYS_ERR_OK)
                        goto retry;

                    currUserPriv = userPriv;

                    currAuthType = SYS_AUTH_TYPE_LOCAL;
                    break;
#ifdef CONFIG_SYS_LIB_TACPLUS
                case SYS_AUTH_TYPE_TACPLUS:
                {
                    struct session *pTacplusSession = NULL;

                    ret = sal_aaa_tacplusSession_init(&pTacplusSession);
                    
                    if (ret != SYS_ERR_OK)
                        break;

                    if (currUserName[0] == '\0')
                    {
                        printf("Username: ");
                        osal_memset(input_username, 0, sizeof(input_username));
                        input_str_with_timeout(input_username, CAPA_SYS_USERNAME_LEN, 0);
                        osal_strcpy(currUserName, input_username);

                        isInputUser = TRUE;
                    }
                    ret = sal_aaa_tacplusLoginUsername_authen(pTacplusSession, input_username);
                    if (ret != SYS_ERR_OK)
                    {
                        if (pTacplusSession != NULL)
                            sal_aaa_tacplusSession_close(pTacplusSession);
                        
                        if (ret == SYS_ERR_USERNAME_INVALID)
                            goto retry;
                        
                        else
                            break;
                    }

                    if (currPassWord[0] == '\0')
                    {
                        printf("Password: ");
                        osal_memset(input_password, 0, sizeof(input_password));
                        input_str_with_timeout(input_password, CAPA_SYS_PASSWORD_LEN, '*');
                        osal_strcpy(currPassWord, input_password);
                    }
                    ret = sal_aaa_tacplusLoginPassword_authen(pTacplusSession, input_password);
                    if (pTacplusSession != NULL)
                        sal_aaa_tacplusSession_close(pTacplusSession);
                    
                    if (ret != SYS_ERR_OK)
                        goto retry;

                    currAuthType = SYS_AUTH_TYPE_TACPLUS;
                    break;
                }
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
                case SYS_AUTH_TYPE_RADIUS:
                    if (currUserName[0] == '\0')
                    {
                        printf("Username: ");
                        osal_memset(input_username, 0, sizeof(input_username));
                        input_str_with_timeout(input_username, CAPA_SYS_USERNAME_LEN, 0);
                        osal_strcpy(currUserName, input_username);

                        isInputUser = TRUE;
                    }
                    if (currPassWord[0] == '\0')
                    {
                        printf("Password: ");
                        osal_memset(input_password, 0, sizeof(input_password));
                        input_str_with_timeout(input_password, CAPA_SYS_PASSWORD_LEN, '*');
                        osal_strcpy(currPassWord, input_password);
                    }
                    ret = sal_aaa_radiusLoginUsername_authen(input_username, input_password);

                    if (ret != SYS_ERR_OK)
                    {
                        
                        if (ret == SYS_ERR_RADIUS_AUTH_REJECT)
                            goto retry;
                        
                        else
                            break;
                    }
                    currAuthType = SYS_AUTH_TYPE_RADIUS;
                    break;
#endif
                case SYS_AUTH_TYPE_NONE:
                    
                    if ((0 == osal_strlen(currUserName)) && (TRUE == isInputUser))
                        goto retry;

                    ret = SYS_ERR_OK;
                    currAuthType = SYS_AUTH_TYPE_NONE;

                    currUserPriv = SYS_LOCALUSER_PRIV_ADMIN;

                    break;

                default:
                    break;
            }

            if (ret == SYS_ERR_OK)
            {
                SYS_LOG(LOG_CAT_AAA, LOG_AAA_AUTH_SUCCESS,
                            text_aaa_auth_method[typePrio[i]].text,
                            "login");

                SYS_LOG(LOG_CAT_AAA, LOG_AAA_CONNECT,
                        text_access_type[access_flag].text,
                        input_username,
                        (0 == osal_strlen(currHost)) ? "async" : currHost);
                #ifdef CONFIG_SYS_MIB_TRAP
        	   
                 sal_snmp_system_auth_success_fail_Trap_send_new(TRUE,text_access_type[type].text,input_username,currHost);
                #endif
                return 1;
            }
            else
            {
                SYS_LOG(LOG_CAT_AAA, LOG_AAA_METHOD_FAIL,
                            text_aaa_auth_method[typePrio[i]].text);
            }
        }

retry:
        if (ret != SYS_ERR_OK)
        {
            printf("%% Authentication Failed\n\n");
            retryCnt--;
            isInputUser = FALSE;

            SYS_LOG(LOG_CAT_AAA, LOG_AAA_AUTH_FAIL,
                        "login");

            if (SYS_ERR_USERNAME_INVALID != ret)
            {
                SYS_LOG(LOG_CAT_AAA, LOG_AAA_USER_REJECT,
                        text_access_type[access_flag].text,
                        input_username,
                        (0 == osal_strlen(currHost)) ? "async" : currHost);
            }
            else
            {
                SYS_LOG(LOG_CAT_AAA, LOG_AAA_REJECT,
                        text_access_type[access_flag].text,
                        (0 == osal_strlen(currHost)) ? "async" : currHost);
            }
            
             #ifdef CONFIG_SYS_MIB_TRAP
        	   
               sal_snmp_system_auth_success_fail_Trap_send_new(FALSE,text_access_type[type].text,input_username,currHost);
            #endif
            continue;
        }
    }

    return 0;
}

int enable_authen (int enblPriv)
{
    int32   ret = SYS_ERR_FAILED;
    char    input_password[CAPA_SYS_PASSWORD_LEN + 1];
    sys_auth_type_t typePrio[CAPA_AAA_AUTHTYPE_PRI_NUM] = {0};
    int     i = 0;

    sal_aaa_enableAuthTypePrio_get(access_flag, typePrio);

    if (typePrio[0] == SYS_AUTH_TYPE_NONE)
    {
        return SYS_ERR_OK;
    }

        printf("Password: ");
    osal_memset(input_password, 0, sizeof(input_password));
    ret = input_str(input_password, CAPA_SYS_PASSWORD_LEN, '*');
    if (ret < 0)
        return SYS_ERR_PASSWORD_INVALID;

    for (i = 0; i < CAPA_AAA_AUTHTYPE_PRI_NUM; i++)
    {
        if (SYS_AUTH_TYPE_EMPTY == typePrio[i])
            break;

        if ((i != 0) && (typePrio[i] > SYS_AUTH_TYPE_NONE))
        {
            SYS_LOG(LOG_CAT_AAA, LOG_AAA_METHOD_NEW,
                        text_aaa_auth_method[typePrio[i]].text);
        }

        switch(typePrio[i])
        {
            case SYS_AUTH_TYPE_ENABLE:
            {
                ret = sal_aaa_localEnablePassword_authen(enblPriv, input_password);
                break;
            }
#ifdef CONFIG_SYS_LIB_TACPLUS
            case SYS_AUTH_TYPE_TACPLUS:
            {
                struct session *pTacplusSession = NULL;
                char enblUsername[10] = {0};

                ret = sal_aaa_tacplusSession_init(&pTacplusSession);
                
                if (ret != SYS_ERR_OK)
                    break;

                osal_sprintf(enblUsername, "$enab%d$", enblPriv);
                ret = sal_aaa_tacplusLoginUsername_authen(pTacplusSession, enblUsername);
                if (ret != SYS_ERR_OK)
                {
                    if (pTacplusSession != NULL)
                        sal_aaa_tacplusSession_close(pTacplusSession);
                    
                    if (ret == SYS_ERR_USERNAME_INVALID)
                        goto enbl_fail;
                    
                    else
                        break;
                }
                ret = sal_aaa_tacplusLoginPassword_authen(pTacplusSession, input_password);
                if (pTacplusSession != NULL)
                        sal_aaa_tacplusSession_close(pTacplusSession);
                
                if (ret != SYS_ERR_OK)
                    goto enbl_fail;
                break;
            }
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
            case SYS_AUTH_TYPE_RADIUS:
            {
                char enblUsername[10] = {0};
                osal_sprintf(enblUsername, "$enab%d$", enblPriv);
                ret = sal_aaa_radiusLoginUsername_authen(enblUsername, input_password);
                
                if (ret != SYS_ERR_OK && ret != SYS_ERR_RADIUS_SRV_ERR)
                    goto enbl_fail;
                break;
            }
#endif

            case SYS_AUTH_TYPE_NONE:
                ret = SYS_ERR_OK;
                currAuthType = SYS_AUTH_TYPE_NONE;
                break;

            default:
                break;
        }

        if (ret == SYS_ERR_OK)
        {
            SYS_LOG(LOG_CAT_AAA, LOG_AAA_AUTH_SUCCESS,
                        text_aaa_auth_method[typePrio[i]].text,
                        "enable");

            SYS_LOG(LOG_CAT_AAA, LOG_AAA_ENABLE_SUCCESS,
                    currUserName,
                    text_access_type[access_flag].text,
                    enblPriv);
            return ret;
        }
        else
        {
            SYS_LOG(LOG_CAT_AAA, LOG_AAA_METHOD_FAIL,
                        text_aaa_auth_method[typePrio[i]].text);
        }
    }

#ifdef CONFIG_SYS_LIB_TACPLUS
enbl_fail:
#endif
    SYS_LOG(LOG_CAT_AAA, LOG_AAA_AUTH_FAIL,
                "enable");

    SYS_LOG(LOG_CAT_AAA, LOG_AAA_ENABLE_FAIL,
            currUserName,
            text_access_type[access_flag].text,
            enblPriv);

    return ret;
}

#else 
int32 login_authen (void)
{
    char    input_username[CAPA_SYS_USERNAME_LEN + 1] = {0};
    char    input_password[CAPA_SYS_PASSWORD_LEN + 1] = {0};
    sys_userInfo_t userInfo;
    uint32 i;
    uint32 isFound = FALSE;
    uint32 retryCnt = 3; 
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    sys_cliRetrySilent_t retrySilent;
    uint32 cfgRetryCnt = 0;
    sys_line_cliType_t type = 0;

    if (access_flag == SYS_ACCESS_CLI_CONSOLE)
        type = SYS_LINE_CLI_CONSOLE;
    else if (access_flag == SYS_ACCESS_CLI_TELNET)
        type = SYS_LINE_CLI_TELNET;
  #ifdef CONFIG_USER_SSH_SSHD
    else if (access_flag == SYS_ACCESS_CLI_SSH)
        type = SYS_LINE_CLI_SSH;
  #endif

    sal_sys_cliRetrySilent_get(type, &retrySilent);

    cfgRetryCnt = retrySilent.retryNum;

    if (cfgRetryCnt == 0)
        retryCnt = 0xFFFFFFFF;
    else
        retryCnt = cfgRetryCnt;
#endif 

    while (retryCnt > 0)
    {
        isFound = FALSE;

        osal_memset(input_username, 0, sizeof(input_username));
#ifdef CONFIG_USER_SSH_SSHD
        if (SYS_ACCESS_CLI_SSH == access_flag)
        {
            
            osal_strcpy(input_username, ssh_username);
        }
        else
#endif
        {
            printf("Username: ");
            input_str_with_timeout(input_username, CAPA_SYS_USERNAME_LEN, 0);
        }

        for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
        {
            osal_memset(&userInfo, 0, sizeof(userInfo));
            SYS_ERR_CONTINUE(sal_sys_localUser_get(i, &userInfo));

            if (userInfo.priv < 0)
                continue;

            if (0 == osal_strcmp(userInfo.username, input_username))
            {
                isFound = TRUE;
                break;
            }
        }

        if (isFound == FALSE)
        {
            printf("Incorrect User Name!!\n\n");
            retryCnt--;

            continue;
        }

        isFound = FALSE;

#ifdef CONFIG_USER_SSH_SSHD
        if (SYS_ACCESS_CLI_SSH == access_flag)
        {
            
            isFound = TRUE;
        }
        else
#endif
        {
            osal_memset(input_password, 0, sizeof(input_password));
            printf("Password: ");
            input_str_with_timeout(input_password, CAPA_SYS_PASSWORD_LEN, '*');

            if (SYS_PASSWD_TYPE_ENCRYPT == userInfo.passwdType)
            {
                char crypt_password[CAPA_SYS_CRYPT_PASSWORD_LEN] = {0};
                sal_util_password_encrypt(input_password, userInfo.password, crypt_password);
                if (0 == osal_strcmp(userInfo.password, crypt_password))
                    isFound = TRUE;
            }
            else if (SYS_PASSWD_TYPE_CLEARTEXT == userInfo.passwdType)
            {
                if (0 == osal_strcmp(userInfo.password, input_password))
                    isFound = TRUE;
            }
            else
            {
                isFound = TRUE;
            }
        }

        if (isFound == TRUE)
        {
            osal_strcpy(currUserName, input_username);
            currUserPriv = userInfo.priv;
            currpriv = userInfo.priv;
            if (userInfo.priv == SYS_LOCALUSER_PRIV_ADMIN)
                vty->node = ENABLE_NODE;

            SYS_LOG(LOG_CAT_AAA, LOG_AAA_CONNECT,
                    text_access_type[access_flag].text,
                    input_username,
                    (0 == osal_strlen(currHost)) ? "async" : currHost);
            #ifdef CONFIG_SYS_MIB_TRAP
        	   
               sal_snmp_system_auth_success_fail_Trap_send_new(isFound,text_access_type[type].text,input_username,currHost);
            #endif
            return 1;
        }

        retryCnt--;
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_USER_REJECT,
                text_access_type[access_flag].text,
                input_username,
                (0 == osal_strlen(currHost)) ? "async" : currHost);
        #ifdef CONFIG_SYS_MIB_TRAP
        	   
               sal_snmp_system_auth_success_fail_Trap_send_new(isFound,text_access_type[type].text,input_username,currHost);
        #endif
    }

    return 0;
}

int enable_authen (int enblPriv)
{
    int32   ret = SYS_ERR_PASSWORD_INVALID;
    char    input_password[CAPA_SYS_PASSWORD_LEN + 1];
    sys_enblPasswd_t adminEnblPasswd;

    if (currpriv == SYS_LOCALUSER_PRIV_ADMIN)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_sys_enablePassword_get(0, &adminEnblPasswd));

    if (SYS_PASSWD_TYPE_NOPASSWD == adminEnblPasswd.passwdType)
    {
        currpriv = SYS_LOCALUSER_PRIV_ADMIN;
        return SYS_ERR_OK;
    }

    printf("Password: ");
    osal_memset(input_password, 0, sizeof(input_password));
    ret = input_str(input_password, CAPA_SYS_PASSWORD_LEN, '*');
    if (ret < 0)
        return SYS_ERR_PASSWORD_INVALID;

    ret = SYS_ERR_PASSWORD_INVALID;

    if (SYS_PASSWD_TYPE_ENCRYPT == adminEnblPasswd.passwdType)
    {
        char crypt_password[CAPA_SYS_CRYPT_PASSWORD_LEN] = {0};
        sal_util_password_encrypt(input_password, adminEnblPasswd.password, crypt_password);
        if (0 == osal_strcmp(adminEnblPasswd.password, crypt_password))
            ret = SYS_ERR_OK;
    }
    else if (SYS_PASSWD_TYPE_CLEARTEXT == adminEnblPasswd.passwdType)
    {
        if (0 == osal_strcmp(adminEnblPasswd.password, input_password))
            ret = SYS_ERR_OK;
    }
    if (ret == SYS_ERR_OK)
    {
        currpriv = SYS_LOCALUSER_PRIV_ADMIN;
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_ENABLE_SUCCESS,
                currUserName,
                text_access_type[access_flag].text,
                currpriv);
    }
    else
    {
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_ENABLE_FAIL,
                currUserName,
                text_access_type[access_flag].text,
                currpriv);
    }

    return ret;
}

#endif 

static void _cfg_semaphore_unlock(void)
{
    uint32 i;

    for (i = 0; i < CFG_ID_END; i++)
    {
        cfg_close(i);
    }

    return;
}

static void _process_fd_close(void)
{
#define USER_FD_START 4
#define USER_FD_MAX   1024
    int32 fd;

    for (fd = USER_FD_START; fd < USER_FD_MAX; fd++)
    {
        close(fd);
    }

    return;
}

void
sigtstp (int sig)
{
  uint32 isCmdIntr = FALSE;
  sys_enable_t enable = DISABLED;

  if (TRUE == intr_forbid_flag)
      return;

  cmd_textline_enable_get(&enable);
  if (ENABLED == enable)
      return;

  _cfg_semaphore_unlock();
  _process_fd_close();

  if (TRUE == g_isCliFwProcess)
  {
      if (SYS_ERR_OK != sal_sys_fwImageLock_check())
          sal_sys_fwImageLock_give();

      cmd_util_progressPrint_stop();

      g_isCliFwProcess = FALSE;
  }

  if (SYS_ERR_OK != sal_tech_fileLock_check())
  {
      unlink(SYS_POLLD_ACT_WEB_TECH_LOCK);
      vty_more_normal();
      cmd_util_progressPrint_stop();
  }

  {
      struct termios t;

      tcgetattr(STDIN_FILENO, &t);

      if ((t.c_iflag & ICRNL))
      {
          vty_echo_enable();
          isCmdIntr = TRUE;
      }
  }

  if (TRUE == exec_cmd && TRUE == isCmdIntr)
  {
      vty->node = current_node;
      exec_cmd = FALSE;
  }
  
  else
  {
      if (vty->node >= ENABLE_NODE)
      {
          vty->node = ENABLE_NODE;
          current_node = ENABLE_NODE;
      }
  }

  rl_initialize ();
  printf ("\n");

  if (! jmpflag)
    return;

  jmpflag = 0;

  siglongjmp (jmpbuf, 1);
}

void
sigint (int sig)
{
    sys_enable_t enable = DISABLED;

    if (TRUE == intr_forbid_flag)
        return;

    cmd_textline_enable_get(&enable);
    if (ENABLED == enable)
        return;

    if (! execute_flag)
    {
        _cfg_semaphore_unlock();
        _process_fd_close();

        if (TRUE == g_isCliFwProcess)
        {
            if (SYS_ERR_OK != sal_sys_fwImageLock_check())
                sal_sys_fwImageLock_give();

            cmd_util_progressPrint_stop();

            g_isCliFwProcess = FALSE;
        }

        if (SYS_ERR_OK != sal_tech_fileLock_check())
        {
            unlink(SYS_POLLD_ACT_WEB_TECH_LOCK);
            vty_more_normal();
            cmd_util_progressPrint_stop();
        }

        if (TRUE == exec_cmd)
        {
            vty->node = current_node;
            exec_cmd = FALSE;
        }
        
        {
            struct termios t;

            tcgetattr(STDIN_FILENO, &t);

            if ((t.c_iflag & ICRNL))
            {
                vty_echo_enable();
            }
        }

        rl_initialize ();
        printf ("\n");

        if (! jmpflag)
            return;

        jmpflag = 0;

        siglongjmp (jmpbuf, 1);
    }
}

void
sigquit (int sig)
{
    
    char sel[2] = {0};
    char confirm[2];
    char input_username[CAPA_SYS_USERNAME_LEN + 1] = {0};
    char input_password[CAPA_SYS_PASSWORD_LEN + 1] = {0};

    sys_enable_t enable = DISABLED;

    cmd_textline_enable_get(&enable);
    if (ENABLED == enable)
        return;

    if (access_flag != SYS_ACCESS_CLI_CONSOLE)
    {
        printf("Do not allow remote user to launch password recovery\n");
        return;
    }

    printf("\nPassword Recovery Password: ");
    osal_memset(input_password, 0, sizeof(input_password));
    input_str(input_password, CAPA_SYS_PASSWORD_LEN, '*');

    if (0 == strcmp(input_password, DFLT_CLI_RECOVERY_PASSWORD))
    {
        sys_userInfo_t userInfo;
        uint32 i;

        printf("Password Recovery - Options\n");
        printf("[P] Password recovery for specific user\n");
        printf("[F] Factory default username/password recovery\n");
        printf("[R] Restore all configurations to factory defaults\n");
        printf("Enter Selection: ");
        input_str(sel, 1, 0);

        if ('p' == tolower(sel[0]))
        {
            uint32 i;
            uint32 isFound = FALSE;
            printf("User Name: ");
            input_str(input_username, CAPA_SYS_USERNAME_LEN, 0);
            for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
            {
                osal_memset(&userInfo, 0, sizeof(userInfo));
                SYS_ERR_CONTINUE(sal_sys_localUser_get(i, &userInfo));

                if (userInfo.priv < 0)
                    continue;

                if (0 == osal_strcmp(userInfo.username, input_username))
                {
                    isFound = TRUE;
                    break;
                }
            }

            if (isFound)
            {
                int32 ret = SYS_ERR_FAILED;

                printf("New Password: ");
                osal_memset(input_password, 0, sizeof(input_password));
                input_str(input_password, CAPA_SYS_PASSWORD_LEN, '*');

                osal_memset(userInfo.password, 0, sizeof(userInfo.password));
                osal_strcpy(userInfo.password, input_password);
                sal_sys_localUser_del(userInfo.username);
                ret = sal_sys_localUser_add(userInfo);

                if (SYS_ERR_OK == ret)
                {
                    printf("\nPassword recovery success.\n");
                }
                else
                {
                    SYS_PRINTF("%s\n", SYS_ERR_STR(ret));
                    return;
                }
            }
            else
            {
                printf("\nUser is not exist\n");
                return;
            }
        }
        else if ('f' == tolower(sel[0]))
        {
            
            for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
            {
                osal_memset(&userInfo, 0, sizeof(userInfo));
                SYS_ERR_CONTINUE(sal_sys_localUser_get(i, &userInfo));

                if (userInfo.priv < 0)
                    continue;

                sal_sys_localUser_del(userInfo.username);
            }
            
            fds_sys_localuser_dfltEntry_init();
            printf("\nPassword recovery success.\n");
        }
        else if ('r' == tolower(sel[0]))
        {
            printf("\nNeed to reboot to take effect.\nAre you sure? [y/n] ");
            input_str(confirm, 1, 0);
            if (confirm[0] == 'y')
            {
                sal_sys_restore_defaults(DISABLED);
                printf("Restore Default Success!\n");
                printf("Rebooting now...\n");
                sal_sys_reboot();
                return;
            }

            printf("\nExit password recovery procedure\n\n");
            return;
        }

        printf("Do you want to save recovery password to startup config now? [y/n] ");
        input_str(confirm, 1, 0);
        if (confirm[0] == 'y')
        {
            cmd_conf_startupConfig_save();
        }
    }
    else
    {
        printf("\nAuthentication Fail.\nDo now allow to launch password recovery procedure.\n");
    }
    printf("\nExit password recovery procedure\n\n");
}

int *
vtysh_signal_set (int signo, void (*func)(int))
{
  int ret;
  struct sigaction sig;
  struct sigaction osig;

  sig.sa_handler = func;
  sigemptyset (&sig.sa_mask);
  sig.sa_flags = 0;
#ifdef SA_RESTART
  sig.sa_flags |= SA_RESTART;
#endif 

  ret = sigaction (signo, &sig, &osig);

  if (ret < 0)
    return ((int*)SIG_ERR);
  else
    return ((int*)osig.sa_handler);
}

void
vtysh_signal_init (void)
{
  vtysh_signal_set (SIGINT, sigint);
  vtysh_signal_set (SIGTSTP, sigtstp);
  vtysh_signal_set (SIGQUIT, sigquit);
  vtysh_signal_set (SIGPIPE, SIG_IGN);
}

static void
usage (int status)
{
  if (status != 0)
    fprintf (stderr, "Try `%s --help' for more information.\n", progname);
  else
    printf ("Usage : %s [OPTION...]\n\n" \
        "Integrated shell for Quagga routing software suite. \n\n" \
        "-b, --boot               Execute boot startup configuration\n" \
        "-c, --command            Execute argument as command\n" \
        "-d, --daemon             Connect only to the specified daemon\n" \
        "-E, --echo               Echo prompt and command in -c mode\n" \
        "-C, --dryrun             Check configuration for validity and exit\n" \
        "-h, --help               Display this help and exit\n\n" \
        "Note that multiple commands may be executed from the command\n" \
        "line by passing multiple -c args, or by embedding linefeed\n" \
        "characters in one or more of the commands.\n\n" \
        "Report bugs to %s\n", progname, ZEBRA_BUG_ADDRESS);

  exit (status);
}

struct option longopts[] =
{
  { "boot",                 no_argument,             NULL, 'b'},
  
  { "eval",                 required_argument,       NULL, 'e'},
  { "command",              required_argument,       NULL, 'c'},
  { "daemon",               required_argument,       NULL, 'd'},
  { "echo",                 no_argument,             NULL, 'E'},
  { "dryrun",            no_argument,         NULL, 'C'},
  { "help",                 no_argument,             NULL, 'h'},
  { "noerror",            no_argument,         NULL, 'n'},
  { 0 }
};

char *
vtysh_rl_gets (void)
{
  HIST_ENTRY *last;
  
  if (line_read)
    {
      free (line_read);
      line_read = NULL;
    }

  line_read = readline (vtysh_prompt ());

  if (line_read && *line_read)
    {
      using_history();
      last = previous_history();
      if (!last || strcmp (last->line, line_read) != 0) {
    add_history (line_read);
    append_history(1,history_file);
      }
    }

  return (line_read);
}

static void log_it(const char *line)
{
  time_t t = time(NULL);
  struct tm *tmp = localtime(&t);
  char *user = getenv("USER") ? : "boot";
  char tod[64];

  strftime(tod, sizeof tod, "%Y%m%d-%H:%M.%S", tmp);

  fprintf(logfile, "%s:%s %s\n", tod, user, line);
}

void diagdbgOperation(void)
{
    char sel[2] = {0};
    int oldNode;
    if (DIAGDBG_OP_DEBUG == diagdbg_flag)
    {
        if (access_flag == SYS_ACCESS_CLI_CONSOLE)
        {
            printf("\n");
            printf("[D] Diag\n");
            printf("[S] Shell\n");
            printf("Enter Selection: ");
            input_str(sel, 1, 0);

            if ('e' == tolower(sel[0]))
            {
                char cmd[16] = {0};

                oldNode = vty->node;
                vty->node = DEBUG_NODE;
                osal_sprintf(cmd, "debug %d", oldNode);
                vtysh_execute (cmd);
            }
            
            else if ('d' == tolower(sel[0]))
            {
                oldNode = vty->node;
                vty->node = DEBUG_NODE;
                vtysh_execute ("diag");
                vty->node = oldNode;
            }
            
            else if ('s' == tolower(sel[0]))
            {
                if (SYS_ACCESS_CLI_CONSOLE == access_flag)
                {
                    unlink(SYS_CONSOLE_CLI_PID_FILE);
                    exit(0);
                }
                else if (SYS_ACCESS_CLI_TELNET == access_flag)
                    system("sh -a telnet");
#ifdef CONFIG_USER_SSH_SSHD
                else if (SYS_ACCESS_CLI_SSH == access_flag)
                    system("sh -a ssh");
#endif
            }
        }
        
        else
        {
            oldNode = vty->node;
            vty->node = DEBUG_NODE;
            vtysh_execute ("diag");
            vty->node = oldNode;
        }
    }
    diagdbg_flag = DIAGDBG_OP_NONE;

}

#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
void genLocalPrivCmdsFile(void)
{
    vector cmd_vector = NULL;
    vector descvec;
    struct cmd_element *cmd_element;
    uint32 i, j;
    const char *str = NULL;
    struct desc *desc;
    char cmd[MAX_CMD_STRLEN];
    FILE *fp;

    if (NULL == (fp = fopen(SYS_AAA_CMDPRIV_CONF_TMPFILE, "w")))
        return;

    cmd_vector = vector_copy (cmd_node_vector (cmdvec, CONFIG_NODE));
    for (i = 0; i < vector_active (cmd_vector); i++)
    {
        if ((cmd_element = vector_slot (cmd_vector, i)) != NULL)
        {
            if (cmd_element->cmdpriv != cmd_element->cmdcurrpriv)
            {
                memset(cmd, 0, sizeof(cmd));
                sprintf(cmd, "privilege config level %d \"", cmd_element->cmdcurrpriv);
                for (j = 0; j < vector_active (cmd_element->strvec); j++)
                {
                    descvec = vector_slot (cmd_element->strvec, j);
                    if ((desc = vector_slot (descvec, 0)))
                    {
                        str = desc->cmd;
                        strcat(cmd, str);
                        strcat(cmd, " ");
                    }
                }
                cmd[strlen(cmd) - 1] = '"';
                cmd[strlen(cmd)] = '\n';
                fprintf(fp, "%s", cmd);
            }
        }
    }
    vector_free (cmd_vector);

    cmd_vector = vector_copy (cmd_node_vector (cmdvec, ENABLE_NODE));
    for (i = 0; i < vector_active (cmd_vector); i++)
    {
        if ((cmd_element = vector_slot (cmd_vector, i)) != NULL)
        {
            if (cmd_element->cmdpriv != cmd_element->cmdcurrpriv)
            {
                memset(cmd, 0, sizeof(cmd));
                sprintf(cmd, "privilege exec level %d \"", cmd_element->cmdcurrpriv);
                for (j = 0; j < vector_active (cmd_element->strvec); j++)
                {
                    descvec = vector_slot (cmd_element->strvec, j);
                    if ((desc = vector_slot (descvec, 0)))
                    {
                        str = desc->cmd;
                        strcat(cmd, str);
                        strcat(cmd, " ");
                    }
                }
                cmd[strlen(cmd) - 1] = '"';
                cmd[strlen(cmd)] = '\n';
                fprintf(fp, "%s", cmd);
            }
        }
    }
    vector_free (cmd_vector);
    fclose(fp);

}

#endif 

void
vtysh_cmd_loop (void)
{
    
    timeout_reset();

    for (;;) {
        while ( vtysh_rl_gets() ) {
            
            exec_cmd = FALSE;

            if (access_flag == SYS_ACCESS_CLI_CONSOLE)
                _terminal_initial();

            if (diagdbg_flag != DIAGDBG_OP_NONE) {
                diagdbgOperation();
            }

            if (readln_timeout_chk() >= 0) {
                SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_CLI_TIMEOUT,
                            currUserName,
                            (0 == osal_strlen(currHost)) ? "async" : currHost);
#ifdef CONFIG_SYS_BUILD_RELEASE
                system("clear");
#endif
                printf("\n\nTimeout, console is locked.\n\n\n");
                sleep(1);
                cli_exit();
            }
            
            timeout_clear();
            
            exec_cmd = TRUE;
            current_node = vty->node;
            
            CLI_ERR_HDL(vtysh_execute(line_read));
            
            current_node = vty->node;
            
            timeout_reset();
        }
    }
}

int isOverMaxConn(void)
{
    DIR *dir = NULL;
    FILE *fp = NULL;
    int32 sessionNum = 0;
    int32 pid = 0;
    char cmd[SYS_BUF512_LEN];
    char fullname[CAPA_FILE_FULLNAME_LEN];
    struct dirent *entry = NULL;

    if ((dir = opendir("/proc")) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] == '.')
            {
                if((0 == entry->d_name[1] || (entry->d_name[1] == '.' && 0 == entry->d_name[2])))
                    continue;
            }

            if (!(entry->d_name[0] >= '0' && entry->d_name[0] <= '9'))
                continue;

            pid = SYS_STR2UINT(entry->d_name);

            SYS_MEM_CLEAR(fullname);
            osal_sprintf(fullname, "/proc/%d/cmdline", pid);

            fp = NULL;
            if (NULL == (fp = fopen(fullname, "r")))
                continue;

            SYS_MEM_CLEAR(cmd);
            fgets(cmd, sizeof(cmd), fp);
            fclose(fp);

            if (0 != osal_strstr(cmd, "cli"))
                sessionNum++;
        }
        closedir(dir);
    }

    if (sessionNum > (CLI_MAX_CONNECT + 1))
        return 1;

    return 0;
}

int processPpidGet(int pid)
{
    FILE *fp = NULL;
    char buf[32] = "";
    char tmp[128] = "";
    int ppid = -1;

    sprintf(buf, "/proc/%d/status", pid);
    if ((fp = fopen(buf, "r")) != NULL)
    {
        while(fgets(tmp, sizeof(tmp), fp))
        {
            if (0 == strncmp(tmp, "PPid:", 5))
            {
                osal_sscanf(tmp, "PPid:\t%d", &ppid);
                break;
            }
            memset(tmp, 0, sizeof(tmp));
        }
        fclose(fp);
    }
    return ppid;
}

void cli_exit(void)
{
    char typeStr[SYS_BUF8_LEN];
    if (access_flag == SYS_ACCESS_CLI_CONSOLE)
    {
        int pppid = processPpidGet(getppid());

        unlink(SYS_CONSOLE_CLI_PID_FILE);

        SYS_LOG(LOG_CAT_AAA, LOG_AAA_DISCONNECT,
                    text_access_type[access_flag].text,
                    currUserName,
                    "async");
                    
        osal_sprintf(typeStr, "%s", text_access_type[access_flag].text);
        #ifdef CONFIG_SYS_MIB_TRAP
        	    
                sal_snmp_system_auth_logout_Trap_send_new(currUserName, typeStr, currHost);
        #endif

        if (1 == pppid)
        {
            kill(getppid(), SIGKILL);
        }
        
        else
        {
            kill(pppid, SIGKILL);
        }
    }
    else if (access_flag == SYS_ACCESS_CLI_TELNET)
    {
		SYS_LOG(LOG_CAT_AAA, LOG_AAA_DISCONNECT,
                    text_access_type[access_flag].text,
                    currUserName,
                    "async");

        {
            kill(getppid(), SIGKILL);
            kill(getpid(), SIGKILL); 
        }

    }
    #ifdef CONFIG_USER_SSH_SSHD
    else if (access_flag == SYS_ACCESS_CLI_SSH)
    {
		SYS_LOG(LOG_CAT_AAA, LOG_AAA_DISCONNECT,
                    text_access_type[access_flag].text,
                    currUserName,
                    "async");

        {
            kill(getppid(), SIGKILL); 
            kill(getpid(), SIGKILL);
        }

    }
    #endif
	
    kill(getpid(), SIGTERM);
}
static int32 _ppid_read(void)
{
    int32 ret = 0;
    int ppid = 0;
    
	while(1)
	{
		ppid = getppid();
		
		if(ppid == 1)
		{
			kill(getpid(), SIGKILL);
		}
		sleep(3);
	}

    return SYS_ERR_OK;
}

int32 ppid_daemon_monitor(void)
{
    int32 ret = -1;
    pthread_t id;

    if (0 != (ret = pthread_create(&id, NULL, (void *)_ppid_read, NULL)))
    {

        return -1;
    }

    return 0;
}

int
main (int argc, char **argv, char **env)
{
  char *p;
  int opt;
  int dryrun = 0;

  struct cmd_rec {
    const char *line;
    struct cmd_rec *next;
  } *cmd = NULL;
  struct cmd_rec *tail = NULL;
  int echo_command = 0;
  int no_error = 0;
  unsigned int count = 0;
#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
  uint32 genLocalPrivCmds = 0;
#endif 
  char localIp[CAPA_IPV6_STR_LEN];

  if (isOverMaxConn())
  {
      int _i;

      SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_CLI_SESSION_FULL,
                 CLI_MAX_CONNECT);

      system("clear");
      printf("\n\nOver maximum cli session !!! Do not allow new session !!!\n\n");

      for(_i = 3; _i > 0; _i--)
      {
          printf("Close after %d seconds\n", _i);
          sleep(1);
          printf("\033[A\033[2K");
      }

      exit(0);
  }

  progname = ((p = strrchr (argv[0], '/')) ? ++p : argv[0]);

  if ((p = getenv("VTYSH_LOG")) != NULL)
      logfile = fopen(p, "a");

  while (1)
    {
#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
      opt = getopt_long (argc, argv, "be:c:d:a:u:H:L:nEhCg", longopts, 0);
#else 
      opt = getopt_long (argc, argv, "be:c:d:a:u:H:L:nEhC", longopts, 0);
#endif

      if (opt == EOF)
    break;

      switch (opt)
    {
    case 0:
      break;
    case 'b':
      
      break;
    case 'e':
    case 'c':
      if(0)
      {
        struct cmd_rec *cr;
        cr = XMALLOC(0, sizeof(*cr));
        cr->line = optarg;
        cr->next = NULL;
        if (tail)
          tail->next = cr;
        else
          cmd = cr;
        tail = cr;
      }
      else
      	return(0);
      break;
    case 'd':
      
      break;
    case 'a':
      if (strstr(optarg, "telnet") != 0)
      {
          
          char buf[32] = "";
          FILE *fp = NULL;
          int ppid = -1;

          ppid = processPpidGet(getpid());
          
          sprintf(buf, SYS_SESSION_DIR"/telnet_%d_cli_pid", ppid);
          if ((fp = fopen(buf, "w")) != NULL)
          {
          	  fprintf(fp, "%d\n%d",  getpid(), getppid());
              fclose(fp);
          }
		  ppid_daemon_monitor();
          access_flag = SYS_ACCESS_CLI_TELNET;
      }
#ifdef CONFIG_USER_SSH_SSHD
      else if (strstr(optarg, "ssh") != 0)
          access_flag = SYS_ACCESS_CLI_SSH;
#endif
      break;
   
   case 'u':
      memset(ssh_username, 0, sizeof(ssh_username));
      
      strncpy(ssh_username, (optarg + 1), strlen(optarg) - 2);
      break;
  
   case 'H':
      SYS_MEM_CLEAR(currHost);
      strcpy(currHost, optarg);
      break;
   case 'L':
      SYS_MEM_CLEAR(localIp);
      strcpy(localIp, optarg);
      break;
#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
    case 'g':
      genLocalPrivCmds = 1;
      break;
#endif 
    case 'n':
      no_error = 1;
      break;
    case 'E':
      echo_command = 1;
      break;
    case 'C':
      dryrun = 1;
      break;
    case 'h':
      usage (0);
      break;
    default:
      usage (1);
      break;
    }
    }

  line_read = NULL;
  setlinebuf(stdout);

#ifdef CONFIG_SYS_BUILD_RELEASE
  #ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
  if (0 == genLocalPrivCmds)
  #endif
      system("clear");
#endif
  
  vtysh_signal_init ();

  vtysh_init_vty ();

  vty_init_vtysh ();

  cli_cmd_init();

  _terminal_initial();

#ifdef CONFIG_SYS_AAA_LOCALCMD_PRIVCHG
  if (genLocalPrivCmds)
  {
    genLocalPrivCmdsFile();
    exit(0);
  }
  {
    FILE *tmpfp;

    if ((tmpfp = fopen(SYS_AAA_CMDPRIV_EXECCONF_TMPFILE, "r")) != NULL)
    {
        fclose(tmpfp);
        if ((tmpfp = fopen(CONF_STARTUP_FILE, "r")) != NULL)
        {
            int oldNode;
            char cmd[MAX_CMD_STRLEN];

            oldNode = vty->node;
            vty->node = CONFIG_NODE;

            while(fgets(cmd, sizeof(cmd), tmpfp))
            {
                if (0 == strncmp(cmd, "privilege config", strlen("privilege config"))
                    || 0 == strncmp(cmd, "privilege exec", strlen("privilege exec")))
                {
                    
                    vtysh_execute(cmd);

                }
            }
            fclose(tmpfp);

            vty->node = oldNode;
        }
        unlink(SYS_AAA_CMDPRIV_EXECCONF_TMPFILE);
    }
  }
#endif 

  if(dryrun)
    return(0);

  if (no_error)
    freopen("/dev/null", "w", stdout);

  while(1)
  {
    int32 ret;
    vty_hello (vty);
    if (access_flag == SYS_ACCESS_CLI_CONSOLE)
    {
        
        vty_out (vty, "%s%s", "Press any key to continue", VTY_NEWLINE);
        vty_getch();
    }
#ifdef CONFIG_SYS_CONSOLE_NO_AUTH
    else
#endif
    { 
	    ret = login_authen();
	    if (0 == ret)
	    {
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
	        silent_time();
	        vty_getch();
#else 
	        if (SYS_ACCESS_CLI_CONSOLE != access_flag)
	            cli_exit();
#endif
	        continue;
	    }
	}
    break;
  }

#ifdef CONFIG_SYS_AAA
    currpriv = currUserPriv;
#endif 

   if (currpriv == SYS_LOCALUSER_PRIV_ADMIN)
   {
     vty->node = ENABLE_NODE;
     current_node = ENABLE_NODE;
   }

#if 0
  
  vtysh_auth ();

  if (vtysh_connect_all (daemon_name) <= 0)
    {
      fprintf(stderr, "Exiting: failed to connect to any daemons.\n");
      exit(1);
    }
#endif
    
    {
        sys_line_session_t session;

        SYS_MEM_CLEAR(session);

        session.pid = getpid();
        osal_strcpy(session.user, currUserName);
        osal_strcpy(session.host, currHost);
        osal_strcpy(session.localIp, localIp);
        sal_sys_lineSession_set(access_flag, session);
    }
  
  if (cmd)
    {

      while (cmd != NULL)
        {
      int ret;
      char *eol;

      while ((eol = strchr(cmd->line, '\n')) != NULL)
        {
          *eol = '\0';

          if (echo_command)
        printf("%s%s\n", vtysh_prompt(), cmd->line);

          if (logfile)
        log_it(cmd->line);

          ret = vtysh_execute_no_pager(cmd->line);
          if (!no_error &&
          ! (ret == CMD_SUCCESS ||
             ret == CMD_SUCCESS_DAEMON ||
             ret == CMD_WARNING))
        exit(1);

          cmd->line = eol+1;
        }

      if (echo_command)
        printf("%s%s\n", vtysh_prompt(), cmd->line);

      if (logfile)
        log_it(cmd->line);

      ret = vtysh_execute_no_pager(cmd->line);
      if (!no_error &&
          ! (ret == CMD_SUCCESS ||
         ret == CMD_SUCCESS_DAEMON ||
         ret == CMD_WARNING))
        exit(1);

      {
        struct cmd_rec *cr;
        cr = cmd;
        cmd = cmd->next;
        XFREE(0, cr);
      }
        }
      exit (0);
    }

  vtysh_pager_init ();
  vtysh_init_timeout(VTY_TIMEOUT_DEFAULT);

  vtysh_readline_init ();

#if 0
  
  while (vtysh_rl_gets ())
    vtysh_execute (line_read);
#endif

  snprintf(history_file, sizeof(history_file), "%s/.history_quagga", getenv("HOME"));
  read_history(history_file);

    {
        sys_line_cliType_t type = SYS_LINE_CLI_CONSOLE;

        if (access_flag == SYS_ACCESS_CLI_CONSOLE)
            type = SYS_LINE_CLI_CONSOLE;
        else if (access_flag == SYS_ACCESS_CLI_TELNET)
            type = SYS_LINE_CLI_TELNET;
#ifdef CONFIG_SYS_APP_SSH
        else if (access_flag == SYS_ACCESS_CLI_SSH)
            type = SYS_LINE_CLI_SSH;
#endif

        sal_sys_cliHistory_count_get(type, &count);
        stifle_history(count);
    }

    if (access_flag == SYS_ACCESS_CLI_CONSOLE)
    {
        FILE *fp = NULL;

        if (NULL != (fp = fopen(SYS_CONSOLE_CLI_PID_FILE, "w")))
        {
            fprintf(fp, "%d", getpid());

            fclose(fp);
        }
    }

  sigsetjmp (jmpbuf, 1);
  jmpflag = 1;

  vtysh_cmd_loop();

  history_truncate_file(history_file,1000);
  printf ("\n");

  exit (0);
}
