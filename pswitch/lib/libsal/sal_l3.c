
#include <sys/un.h>
#include <sys/socket.h>

#include <common/sys_log.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>
#include <libsal/sal_l3_route_zebra.h>

#define ZEBRA_PATH "/var/run/zebra.vty"

const sys_text_t text_ipv6_dadSts[] =
{
    { ""          },
    { "ACTIVE"    },
    { "TENTATIVE" },
    { "DUPLICATE" },
};

#ifdef CONFIG_SYS_L3_ROUTE
static void _eng_close(int32 fd)
{
    if (fd >= 0)
        close (fd);
}

static int32 _eng_execute(int32 fd, const char *line, FILE *fp)
{
    int32 ret;
    char buf[SYS_BUF1024_LEN];
    int32 nbytes;
    int32 i;
    int32 numnulls = 0;

    if (fd < 0)
        return SYS_ERR_OK;

    ret = write (fd, line, strlen (line) + 1);
    if (ret <= 0)
    {
        _eng_close(fd);
        return SYS_ERR_OK;
    }

    while (1)
    {
        nbytes = read (fd, buf, sizeof(buf)-1);

        if (nbytes <= 0 )
        {
            _eng_close(fd);
            return SYS_ERR_OK;
        }

        if (nbytes > 0)
        {
            if ((numnulls == 3) && (nbytes == 1))
            return buf[0];

            buf[nbytes] = '\0';
            fputs(buf, fp);
            fflush(fp);

            if (nbytes >= 4)
            {
                i = nbytes-4;
                numnulls = 0;
            }
            else
            {
                i = 0;
            }

            while (i < nbytes && numnulls < 3)
            {
                if (buf[i++] == '\0')
                    numnulls++;
                else
                    numnulls = 0;
            }

            if ((numnulls >= 3) && (i < nbytes))
                return (buf[nbytes-1]);
        }
    }
}

static int32 _eng_connect(void)
{
    int ret;
    int sock, len;
    struct sockaddr_un addr;
    struct stat s_stat;

    ret = stat (ZEBRA_PATH, &s_stat);
    if (ret < 0 )
    {

        return -1;
    }

    if (ret >= 0)
    {
        if (! S_ISSOCK(s_stat.st_mode))
        {

            return -1;
        }
    }

    sock = socket (AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0)
    {

        return -1;
    }

    memset (&addr, 0, sizeof (struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy (addr.sun_path, ZEBRA_PATH, strlen(ZEBRA_PATH));
    len = sizeof (addr.sun_family) + strlen (addr.sun_path);

    ret = connect (sock, (struct sockaddr *) &addr, len);
    if (ret < 0)
    {

        close (sock);
        return -1;
    }

    return sock;
}

static int32 __eng_cmd_execute(char *cmd, FILE *fp)
{
    int32 fd = -1;
    int32 ret = SYS_ERR_OK;

    if ((fd = _eng_connect()) < 0)
    {
        return SYS_ERR_FAILED;
    }

    ret = _eng_execute(fd, cmd, fp);

    _eng_close(fd);

    usleep(30000);

    return ret;
}

static int32 _eng_cmd_execute(char *cmd)
{
    int32 ret = SYS_ERR_FAILED;

    L3_USER_LOCK();

    ret = __eng_cmd_execute(cmd, stdout);

    L3_USER_UNLOCK();

    return ret;
}

static int32 _eng_intfcmd_execute(sys_l3_intfId_t *pId, char *cmd)
{
    int32 ret = SYS_ERR_FAILED;
    char intfName[CAPA_L3_INTFNAME_LEN];
    char intfCmd[SYS_BUF128_LEN];

    SYS_MEM_CLEAR(intfName);
    SYS_MEM_CLEAR(intfCmd);

    INTFID2STR(pId, intfName);

    osal_sprintf(intfCmd, "interface %s", intfName);

    L3_USER_LOCK();

    ret = __eng_cmd_execute(intfCmd, stdout);
    if (SYS_ERR_OK != ret)
        goto _exit;

    ret = __eng_cmd_execute(cmd, stdout);
    __eng_cmd_execute("exit", stdout);

_exit:
    L3_USER_UNLOCK();

    return ret;
}

static int32 _eng_cmd_execute2fp(char *cmd, FILE *fp)
{
    int32 ret = SYS_ERR_OK;

    L3_USER_LOCK();

    ret = __eng_cmd_execute(cmd, fp);

    L3_USER_UNLOCK();

    return ret;
}

int32 sal_l3_engineCmd_exec(char *cmd)
{
    SYS_PARAM_CHK((NULL == cmd), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(_eng_cmd_execute(cmd));

    return SYS_ERR_OK;
}

int32 sal_l3_engineCmdNoLock_exec(char *cmd)
{
    SYS_PARAM_CHK((NULL == cmd), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(__eng_cmd_execute(cmd, stdout));

    return SYS_ERR_OK;
}

int32 sal_l3_engineCmd2File_exec(char *cmd, FILE *fp)
{
    SYS_PARAM_CHK((NULL == cmd), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == fp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(_eng_cmd_execute2fp(cmd, fp));

    return SYS_ERR_OK;
}

int32 sal_l3_engineIntfCmd_exec(sys_l3_intfId_t *pId, char *cmd)
{
    SYS_PARAM_CHK((NULL == cmd), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(_eng_intfcmd_execute(pId, cmd));

    return SYS_ERR_OK;
}

int32 sal_l3_engine_restart(void)
{
    uint32 i = 0;
    sys_l3_staticRouteV4_t v4Rules;
    sys_l3_staticRouteV6_t v6Rules;
    sys_l3_intfId_t intfId;
    sys_vid_t vid = 0;
    sys_vlanmask_t adminVm;

    unlink(L3_ENGINE_PID_FILE);
    unlink(L3_FPMD_PID_FILE);

    SYSTEM_CMD_BG("zebra -P 0");
    SYSTEM_CMD_BG("fpmd");

    SYS_MEM_CLEAR(v4Rules);
    SYS_MEM_CLEAR(v6Rules);

    while(1)
    {
        if ((SYS_ERR_OK == sal_util_file_exist(L3_ENGINE_PID_FILE))
            && (SYS_ERR_OK == sal_util_file_exist(L3_FPMD_PID_FILE)))
        {
            break;
        }

        usleep(300000);
    }

    sal_l3_staticRouteV4_get(&v4Rules);

    for (i = 0; i < v4Rules.num; i++)
    {

        sal_l3_ip_route_add(v4Rules.route[i].dst, IPV4_PFX2MASK(v4Rules.route[i].dstLen), v4Rules.route[i].gw, 0, 0);
    }

    sal_l3_staticRouteV6_get(&v6Rules);

    for (i = 0; i < v6Rules.num; i++)
    {

        sal_l3_ipv6_route_add(v6Rules.route[i].dst, v6Rules.route[i].dstLen, v6Rules.route[i].gw, v6Rules.route[i].ointf);
    }

    sal_vlan_adminEntryExist_get(&adminVm);
    FOR_EACH_VID_IN_VLANMASK(vid, adminVm)
    {
        SYS_MEM_CLEAR(intfId);
        intfId.type = SYS_L3_INTFTYPE_VLAN;
        intfId.id = vid;

    }

    return SYS_ERR_OK;
}

int32 sal_l3_fwdIPv4Enable_set(sys_enable_t enable)
{
    sys_enable_t oldEnbl = DISABLED;
    uint32 i = 0;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    cfg_l3_fwdIPv4Enable_get(&oldEnbl);

    if (oldEnbl == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_l3_fwdIPv4Enable_set(enable));

    for (i = 0; i < CAPA_L3_INTFVLAN_NUM; i++)
    {
        SYSTEM_CMD("echo %d > /proc/sys/net/ipv4/conf/%s%u/forwarding", enable, L3_INTF_PREFIX_VLAN, i);
    }

    SYS_ERR_CHK(cfg_l3_fwdIPv4Enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_l3_fwdIPv4Enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_fwdIPv4Enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_l3_fwdIPv6Enable_set(sys_enable_t enable)
{
    sys_enable_t oldEnbl = DISABLED;
    uint32 i = 0;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    cfg_l3_fwdIPv6Enable_get(&oldEnbl);

    if (oldEnbl == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_l3_fwdIPv6Enable_set(enable));

    SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/all/forwarding", enable);

    for (i = 0; i < CAPA_L3_INTFVLAN_NUM; i++)
    {
        SYSTEM_CMD("echo %d > /proc/sys/net/ipv6/conf/%s%u/forwarding", enable, L3_INTF_PREFIX_VLAN, i);
    }

    SYSTEM_CMD("echo 0 > /proc/sys/net/ipv6/conf/%s/forwarding", L3_INTF_MGMTVLAN);

    SYS_ERR_CHK(cfg_l3_fwdIPv6Enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_l3_fwdIPv6Enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_fwdIPv6Enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_l3_hopLimitIPv6_set(uint32 num)
{
    SYS_ERR_CHK(cfg_l3_hopLimitIPv6_set(num));

    return SYS_ERR_OK;
}

int32 sal_l3_hopLimitIPv6_get(uint32 *pNum)
{
    SYS_PARAM_CHK((NULL == pNum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_hopLimitIPv6_get(pNum));

    return SYS_ERR_OK;
}

int32 sal_l3_icmpv6RateInterval_set(uint32 time)
{
    SYS_ERR_CHK(cfg_l3_icmpv6RateInterval_set(time));

    return SYS_ERR_OK;
}

int32 sal_l3_icmpv6RateInterval_get(uint32 *pTime)
{
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_icmpv6RateInterval_get(pTime));

    return SYS_ERR_OK;
}

int32 sal_l3_icmpv6RateBurst_set(uint32 time)
{
    SYS_ERR_CHK(cfg_l3_icmpv6RateBurst_set(time));

    return SYS_ERR_OK;
}

int32 sal_l3_icmpv6RateBurst_get(uint32 *pTime)
{
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_l3_icmpv6RateBurst_get(pTime));

    return SYS_ERR_OK;
}
#endif

