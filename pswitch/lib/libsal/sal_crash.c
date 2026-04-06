
#include <dirent.h>
#include <libgen.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <signal.h>
#include <ucontext.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <libsal/sal_crash.h>
#include <libsal/sal_util.h>

#define CRASHLOG_MAX_COUNT          (3)
#define CRASHLOG_BUFFER_LEN         (256)
#define CRASHLOG_SIZE_MAX           (8 * 1024)

#define min(x, y)               ((x) > (y) ? (y) : (x))

static char *_condenseStr(char *str)
{
    int source = 0;
    int dest = 0;

    while (str[source] != '\0')
    {
        if (!iscntrl(str[source]))
        {
            
            str[dest] = str[source];
            ++dest;
        }
        ++source; 
    }
    str[dest] = '\0'; 
    return str;
}

static int32 _writeConsole(char *pBuffer, uint32 len)
{
    osal_printf("%s", pBuffer);
    return SYS_ERR_OK;
}

static int32 _writeFile(char *pBuffer, uint32 len)
{
    int32 fd, n;
    int32 blocksize = 10 * 1024;
    int32 ret = SYS_ERR_OK;

    if ((fd = open(CRASHLOG_FILE_NAME, O_WRONLY | O_CREAT | O_APPEND | O_SYNC , 0644)) == -1)
    {
        return (SYS_ERR_FAILED);
    }

    while (len > 0)
    {
        n = write(fd, pBuffer, min(blocksize, len));
        if (n > 0)
        {
            pBuffer += n;
            len -= n;
        }
        else
        {
            ret = SYS_ERR_FAILED;
            break;
        }
    }

    sal_util_file_sync();

    return ret;
}

static void _crashLog(const char *pFmt, ...)
{
    char buf[CRASHLOG_BUFFER_LEN + 1];
    uint32 n;
    va_list args;

    va_start(args, pFmt);

    n = vsnprintf(buf, CRASHLOG_BUFFER_LEN, pFmt, args);

    _writeConsole(buf, n);

    _writeFile(buf, n);

    va_end(args);
}

int32 _sal_crash_log_move(void)
{
    uint32 i;
    char buf1[256];
    char buf2[256];

    mkdir(CRASHLOG_PATH, 0644);

    if (sal_util_file_exist(CRASHLOG_FILE_NAME) == SYS_ERR_OK)
    {
        for (i = (CRASHLOG_MAX_COUNT - 1); i > 0; i--)
        {
            osal_snprintf(buf1, sizeof(buf1) - 1, "%s%s.%d", CRASHLOG_PATH,
                          CRASHLOG_FILE_PREFIX, i - 1);
            osal_snprintf(buf2, sizeof(buf2) - 1, "%s%s.%d", CRASHLOG_PATH,
                          CRASHLOG_FILE_PREFIX, i);
            sal_util_file_rename(buf1, buf2);
        }
        osal_snprintf(buf1, sizeof(buf1) - 1, "%s%s.0", CRASHLOG_PATH,
                      CRASHLOG_FILE_PREFIX);
        sal_util_file_copy(CRASHLOG_FILE_NAME, buf1);
        unlink(CRASHLOG_FILE_NAME);
    }

    return SYS_ERR_OK;
}

static void _sal_crash_solib_dump(pid_t pid)
{
    uint32 addr_start;
    char cmd[256] = {0};
    char addr[128] = {0};
    char s[256] = {0};
    char *delim = " ";
    char *p;
    char *pName;
    FILE *fp;

    if (pid < 0)
    {
        return;
    }

    osal_sprintf(cmd, "cat /proc/%d/maps", pid);

    fp = popen(cmd, "r");
    while (fp && (fgets(s, sizeof(s), fp) != NULL))
    {
        
        if (strstr(s, ".so") && strstr(s, "-xp"))
        {
            pName = NULL;
            p = strtok(s, delim);
            osal_snprintf(addr, 128, "%s", p);
            while ((p = strtok(NULL, delim)))
            {
                if (strstr(p, ".so"))
                {
                    pName = p;
                }
            } 

            if (pName)
            {
                p = strtok(addr, "-");
                addr_start = (uint32)strtol(p, NULL, 16);
                _crashLog("0x%08x %s\n", addr_start, _condenseStr(pName));
            }
        }
    }
    pclose(fp);
}

static int _sal_crash_backtrace(void **buffer, uint32 size, ucontext_t const *uc)
{
    uint32 *addr;
    uint32 *pc, *ra, *sp;
    uint32 ra_offset, stack_size;
    int depth;

    if (size == 0)
        return 0;

    if (!buffer || size < 0 || !uc)
        return (-1);

    pc = (uint32 *)(uint32)uc->uc_mcontext.pc;
    ra = (uint32 *)(uint32)uc->uc_mcontext.gregs[31];
    sp = (uint32 *)(uint32)uc->uc_mcontext.gregs[29];

    buffer[0] = pc;

    if (size == 1)
        return 1;

    ra_offset = stack_size = 0;

    for (addr = pc; !ra_offset || !stack_size; --addr)
    {
        switch (*addr & 0xffff0000)
        {
            case MIPS_ADDIU_SP_SP_XXXX:
                stack_size = abs((int16)(*addr & 0xffff));
                break;
            case MIPS_SW_RA_XXXX_SP:
                ra_offset = (int16)(*addr & 0xffff);
                break;
            case MIPS_ADDU_RA_ZERO_ZERO:
                goto __out_of_loop;
            default:
                break;
        } 
    }

__out_of_loop:
    if (ra_offset)
        ra = *(uint32 **)((uint32)sp + ra_offset);
    if (stack_size)
        sp = (uint32 *)((uint32)sp + stack_size);

    for (depth = 1; depth < size && ra; ++depth)
    {
        buffer[depth] = ra;

        ra_offset = stack_size = 0;
        for (addr = ra; !ra_offset || !stack_size; --addr)
        {
            switch (*addr & 0xffff0000)
            {
                case MIPS_ADDIU_SP_SP_XXXX:
                    stack_size = abs((int16)(*addr & 0xffff));
                    break;
                case MIPS_SW_RA_XXXX_SP:
                    ra_offset = (int16)(*addr & 0xffff);
                    break;
                case MIPS_ADDU_RA_ZERO_ZERO:
                    return depth + 1;
                default:
                    break;
            } 
        }

        ra = *(uint32 **)((uint32)sp + ra_offset);
        sp = (uint32 *)((uint32)sp + stack_size);
    }

    return depth;
}

static void _sal_crash_sig_handler(int sig, siginfo_t *info, void *c)
{
    sys_time_str_t timeStr;
    const char *stars = "***************************************************";
    void *trace[128];
    char buf[SYS_BUF64_LEN];
    char **messages;
    uint32 i, n;
    pid_t pid;
    FILE *fp = NULL;

    SYS_MEM_CLEAR(timeStr);

    sal_time_currentTimeStr_get(&timeStr);

    _sal_crash_log_move();

    unlink(CRASHLOG_FILE_NAME);

    pid = getpid();
    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "/proc/%d/cmdline", pid);
    if (NULL != (fp = fopen(buf, "r")))
    {
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);
    }
    fclose(fp);

    _crashLog("\n%s\n", stars);
    _crashLog("* Pswitch Information *\n");
    _crashLog("%s\n", stars);
    _crashLog("pid:      \t%d\n", pid);
    _crashLog("name:     \t%s\n", buf);
    _crashLog("signal:   \t%s\n", strsignal(info->si_signo));
    _crashLog("date:     \t%s\n", timeStr.tstr);
    _crashLog("f/w:      \t%s\n", DFLT_SYS_VER);
    _crashLog("\n");
    _sal_crash_solib_dump(pid);
    _crashLog("\n------------ CALL STACK INFO ------------\n");

    n = _sal_crash_backtrace(trace, sizeof(trace) / sizeof(trace[0]), c);
    messages = backtrace_symbols(trace, n);

    for (i = 0; i < n && messages != NULL; ++i)
    {
        _crashLog("%s\n", messages[i]);
    }

    _crashLog("-----------------------------------------\n");

    if (messages != NULL)
    {
        free(messages);
    }

    signal(sig, SIG_DFL);
    raise(sig);
}

int32 sal_crash_init(void)
{
    
    SYS_ERR_CHK(_sal_crash_log_move());

    return SYS_ERR_OK;
}

int32 sal_crash_handler_register(void)
{
    struct sigaction sig_act;
    sigset_t BlockedSigs;
    int32 rc;

    sigemptyset(&BlockedSigs);
    sig_act.sa_sigaction = _sal_crash_sig_handler;
    sig_act.sa_mask = BlockedSigs;
    sig_act.sa_flags = SA_SIGINFO;

    rc = sigaction(SIGSEGV, &sig_act, 0);
    rc = sigaction(SIGILL, &sig_act, 0);
    rc = sigaction(SIGABRT, &sig_act, 0);
    rc = sigaction(SIGFPE, &sig_act, 0);
    if (rc != 0)
    {
        SYS_DBG(DBG_SYSTEM, "sigaction failed.");
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

