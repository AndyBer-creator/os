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
 * Purpose : Forward plane manager - ip address monitor
 *
 * Feature : Forward plane manager - ip address monitor
 */

/*
 * Include Files
 */
#include <libsal/sal_l3.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "fpmd.h"

/*
 * Symbol Definition
 */
#define NL_PKT_BUF_SIZE 8192

/*
 * Data Declaration
 */
static uint32 debug_kernel = 1;
struct nlsock
{
  int sock;
  int seq;
  struct sockaddr_nl snl;
  const char *name;
} netlink = { -1, 0, {0}, "netlink-listen"};     /* kernel messages */

/*
 * Macro Definition
 */
#define IFA_RTA(r)  ((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct ifaddrmsg))))

/*
 * Function Declaration
 */
static void _addr_data_dump(char *data, uint32 len, char *title)
{
    int32 i = 0;
    char tmp[SYS_BUF8_LEN];
    char buf[SYS_BUF128_LEN];

    FPMD_DBG("\n%s\n", title);

    SYS_MEM_CLEAR(buf);
    for (i = 0; i < len; i++)
    {
        if (0 == (i % 16))
            osal_sprintf(buf, "[%04d] ", i);

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%02X ", *(uint8 *)(data + i));
        osal_strcat(buf, tmp);

        if (15 == (i % 16))
        {
            FPMD_DBG("%s\n", buf);
            SYS_MEM_CLEAR(buf);
        }
    }
    FPMD_DBG("\n");
}

static int32 _netlink_socket(struct nlsock *nl)
{
    int32 ret;
    struct sockaddr_nl snl;
    int32 sock;
    int32 namelen;
    int32 save_errno;
    unsigned long groups = RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;

    sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock < 0)
    {
        FPMD_DBG("Can't open %s socket: %s", nl->name, safe_strerror(errno));
        return -1;
    }

    memset (&snl, 0, sizeof snl);
    snl.nl_family = AF_NETLINK;
    snl.nl_groups = groups;

    /* Bind the socket to the netlink structure for anything. */
    ret = bind (sock, (struct sockaddr *) &snl, sizeof(snl));
    save_errno = errno;

    if (ret < 0)
    {
        FPMD_DBG("Can't bind %s socket to group 0x%x: %s",
                     nl->name, snl.nl_groups, safe_strerror (save_errno));
        close (sock);
        return -1;
    }

    /* multiple netlink sockets will have different nl_pid */
    namelen = sizeof snl;
    ret = getsockname(sock, (struct sockaddr *) &snl, (socklen_t *) &namelen);
    if (ret < 0 || namelen != sizeof(snl))
    {
        FPMD_DBG("Can't get %s socket name: %s", nl->name, safe_strerror (errno));
        close (sock);
        return -1;
    }

    nl->snl = snl;
    nl->sock = sock;

    return ret;
}

static void netlink_parse_rtattr (struct rtattr **tb, int max, struct rtattr *rta, int len)
{
    while (RTA_OK (rta, len))
    {
        if (rta->rta_type <= max)
            tb[rta->rta_type] = rta;
        rta = RTA_NEXT (rta, len);
    }
}

static int32 netlink_interface_addr(struct sockaddr_nl *snl, struct nlmsghdr *h)
{
    int32 len;
    struct ifaddrmsg *ifa;
    struct rtattr *tb[IFA_MAX + 1];
    //struct interface *ifp;
    void *addr;
    //void *broad;
    //u_char flags = 0;
    //char *label = NULL;
    char buf[SYS_BUF16_LEN];
    char *ifname = NULL;
    sys_l3_intfId_t intfId;
#ifdef CONFIG_SYS_L3_ROUTE
    sys_l3_ipDbList_t *pIpDb = NULL;
#endif
    sys_l3_ip_t l3Ip;

    ifa = NLMSG_DATA (h);

    if (ifa->ifa_family != AF_INET && ifa->ifa_family != AF_INET6)
        return 0;

    if (h->nlmsg_type != RTM_NEWADDR && h->nlmsg_type != RTM_DELADDR)
        return 0;

    len = h->nlmsg_len - NLMSG_LENGTH (sizeof (struct ifaddrmsg));
    if (len < 0)
        return -1;

    memset (tb, 0, sizeof tb);

    netlink_parse_rtattr (tb, IFA_MAX, IFA_RTA (ifa), len);

    SYS_MEM_CLEAR(ifname);
    if (NULL == (ifname = if_indextoname(ifa->ifa_index, buf)))
    {
        FPMD_DBG("netlink_interface_addr can't find interface by index %d", ifa->ifa_index);
        return SYS_ERR_FAILED;
    }

    //ifp = if_indextoname (ifa->ifa_index);
    //if (ifp == NULL)
    //{
    //    FPMD_DBG("netlink_interface_addr can't find interface by index %d", ifa->ifa_index);
    //    return -1;
    //}

    if (debug_kernel)
    {
        char buf[SYS_BUF128_LEN];

        FPMD_DBG("netlink_interface_addr %u %s:\n",
                     h->nlmsg_type,
                     ifname);

        if (tb[IFA_LOCAL])
            FPMD_DBG("  IFA_LOCAL     %s/%d\n",
                         inet_ntop(ifa->ifa_family, RTA_DATA(tb[IFA_LOCAL]), buf, SYS_BUF128_LEN),
                         ifa->ifa_prefixlen);
        if (tb[IFA_ADDRESS])
            FPMD_DBG("  IFA_ADDRESS   %s/%d\n",
                         inet_ntop(ifa->ifa_family, RTA_DATA(tb[IFA_ADDRESS]), buf, SYS_BUF128_LEN),
                         ifa->ifa_prefixlen);
        if (tb[IFA_BROADCAST])
            FPMD_DBG("  IFA_BROADCAST %s/%d\n",
                         inet_ntop(ifa->ifa_family, RTA_DATA(tb[IFA_BROADCAST]), buf, SYS_BUF128_LEN),
                         ifa->ifa_prefixlen);
        if (tb[IFA_LABEL] && strcmp (ifname, RTA_DATA (tb[IFA_LABEL])))
            FPMD_DBG("  IFA_LABEL     %s\n", (char *)RTA_DATA (tb[IFA_LABEL]));

        if (tb[IFA_CACHEINFO])
        {
            struct ifa_cacheinfo *ci = RTA_DATA (tb[IFA_CACHEINFO]);
            FPMD_DBG ("  IFA_CACHEINFO pref %d, valid %d\n",
                          ci->ifa_prefered, ci->ifa_valid);
        }
    }

    /* logic copied from iproute2/ip/ipaddress.c:print_addrinfo() */
    if (tb[IFA_LOCAL] == NULL)
        tb[IFA_LOCAL] = tb[IFA_ADDRESS];
    if (tb[IFA_ADDRESS] == NULL)
        tb[IFA_ADDRESS] = tb[IFA_LOCAL];

    /* local interface address */
    addr = (tb[IFA_LOCAL] ? RTA_DATA(tb[IFA_LOCAL]) : NULL);

#if 0
    /* is there a peer address? */
    if (tb[IFA_ADDRESS] &&
        memcmp(RTA_DATA(tb[IFA_ADDRESS]), RTA_DATA(tb[IFA_LOCAL]), RTA_PAYLOAD(tb[IFA_ADDRESS])))
    {
        broad = RTA_DATA(tb[IFA_ADDRESS]);
        //SET_FLAG (flags, ZEBRA_IFA_PEER);
    }
    else
    {
        /* seeking a broadcast address */
        broad = (tb[IFA_BROADCAST] ? RTA_DATA(tb[IFA_BROADCAST]) : NULL);
    }
#endif

    /* addr is primary key, SOL if we don't have one */
    if (addr == NULL)
    {
        FPMD_DBG("%s: NULL address\n", __func__);
        return SYS_ERR_FAILED;
    }

    /* Flags. */
    //if (ifa->ifa_flags & IFA_F_SECONDARY)
    //    SET_FLAG (flags, ZEBRA_IFA_SECONDARY);

#if 0
    /* Label */
    if (tb[IFA_LABEL])
        label = (char *) RTA_DATA(tb[IFA_LABEL]);

    //if (ifp && label && strcmp (ifp->name, label) == 0)
    //    label = NULL;
#endif

    SYS_MEM_CLEAR(intfId);
    SYS_MEM_CLEAR(l3Ip);
    SYS_ERR_CHK_RET(STR2INTFID(ifname, &intfId), SYS_ERR_OK);

    /* Register interface address to the interface. */
    if (ifa->ifa_family == AF_INET)
    {
        sys_l3_ipv4_t subnet;
        char subnetStr[CAPA_IPV4_STR_LEN];
        char ipStr[CAPA_IPV4_STR_LEN];
        char devStr[CAPA_L3_INTFNAME_LEN];

        osal_memcpy(&l3Ip.v4.addr, addr, sizeof(sys_ipv4_t));
        l3Ip.v4.plen = ifa->ifa_prefixlen;

        if (h->nlmsg_type == RTM_NEWADDR)
        {
            SYS_MEM_CLEAR(subnet);
            SYS_MEM_CLEAR(subnetStr);
            SYS_MEM_CLEAR(ipStr);
            SYS_MEM_CLEAR(devStr);

            INTFID2STR(&intfId, devStr);
            IPV4_L3IP2SUBNET(&l3Ip, &subnet);
            IPV42STR(subnet.addr, subnetStr);
            IPV42STR(l3Ip.v4.addr, ipStr);
            SYSTEM_CMD_HID("ip route add %s/%d dev %s src %s table %s", subnetStr, subnet.plen, devStr, ipStr, L3_INTF_MGMTVLAN);
        }
        else
        {
            SYS_MEM_CLEAR(subnet);
            SYS_MEM_CLEAR(subnetStr);
            SYS_MEM_CLEAR(devStr);

            INTFID2STR(&intfId, devStr);
            IPV4_L3IP2SUBNET(&l3Ip, &subnet);
            IPV42STR(subnet.addr, subnetStr);
            SYSTEM_CMD_HID("ip route del %s/%d dev %s table %s", subnetStr, subnet.plen, devStr, L3_INTF_MGMTVLAN);
        }

        if (SYS_L3_INTFTYPE_MGMTVLAN == intfId.type)
        {
            if (h->nlmsg_type == RTM_NEWADDR)
                sal_l3_intfIPv4_add(&intfId, SYS_L3_IPTYPE_ANY, (sys_l3_ipv4_t*)&l3Ip);
            else
                sal_l3_intfIPv4_del(&intfId, SYS_L3_IPTYPE_ANY, (sys_l3_ipv4_t*)&l3Ip);

            return SYS_ERR_OK;
        }

#ifdef CONFIG_SYS_L3_ROUTE
        if (h->nlmsg_type == RTM_NEWADDR)
        {
#ifdef CONFIG_SYS_L3_VRRP
			if ((strlen(ifname) > 3) && osal_strncmp (ifname, "vrrp", 4) == 0)
			{
				return SYS_ERR_OK;
			}
#endif			
            pIpDb = sys_l3_ipPoolIntfEntryPtr_ret(SYS_L3_AF_IPV4, &intfId, &l3Ip);

            if (NULL == pIpDb)
            {
                sal_l3_intfIPv4_add(&intfId, SYS_L3_IPTYPE_DHCP, (sys_l3_ipv4_t*)&l3Ip);
            }
        }
        else
        {
            pIpDb = sys_l3_ipPoolIntfEntryPtr_ret(SYS_L3_AF_IPV4, &intfId, &l3Ip);

            if (NULL != pIpDb)
            {
                if (SYS_L3_IPTYPE_STATIC != pIpDb->type)
                    sal_l3_intfIPv4_del(&intfId, SYS_L3_IPTYPE_ANY, (sys_l3_ipv4_t*)&l3Ip);
            }
        }
#endif
    }

    if (ifa->ifa_family == AF_INET6)
    {
        sys_l3_ipv6_t subnet;
        char subnetStr[CAPA_IPV6_STR_LEN];
        char ipStr[CAPA_IPV6_STR_LEN];
        char devStr[CAPA_L3_INTFNAME_LEN];

        osal_memcpy(&l3Ip.v6.addr, addr, sizeof(sys_ipv6_t));
        l3Ip.v6.plen = ifa->ifa_prefixlen;

        if (h->nlmsg_type == RTM_NEWADDR)
        {
            SYS_MEM_CLEAR(subnet);
            SYS_MEM_CLEAR(subnetStr);
            SYS_MEM_CLEAR(ipStr);
            SYS_MEM_CLEAR(devStr);

            INTFID2STR(&intfId, devStr);
            IPV6_L3IP2SUBNET(&l3Ip, &subnet);
            IPV62STR(subnet.addr, subnetStr);
            IPV62STR(l3Ip.v6.addr, ipStr);
            SYSTEM_CMD_HID("ip -6 route add %s/%d dev %s src %s table %s", subnetStr, subnet.plen, devStr, ipStr, L3_INTF_MGMTVLAN);
        }
        else
        {
            SYS_MEM_CLEAR(subnet);
            SYS_MEM_CLEAR(subnetStr);
            SYS_MEM_CLEAR(devStr);

            INTFID2STR(&intfId, devStr);
            IPV6_L3IP2SUBNET(&l3Ip, &subnet);
            IPV62STR(subnet.addr, subnetStr);
            SYSTEM_CMD_HID("ip -6 route del %s/%d dev %s table %s", subnetStr, subnet.plen, devStr, L3_INTF_MGMTVLAN);
        }

        if (SYS_L3_INTFTYPE_MGMTVLAN == intfId.type)
        {
            if (h->nlmsg_type == RTM_NEWADDR)
                sal_l3_intfIPv6_add(&intfId, SYS_L3_IPTYPE_ANY, (sys_l3_ipv6_t*)&l3Ip);
            else
                sal_l3_intfIPv6_del(&intfId, SYS_L3_IPTYPE_ANY, (sys_l3_ipv6_t*)&l3Ip);

            return SYS_ERR_OK;
        }

#ifdef CONFIG_SYS_L3_ROUTE
        if (h->nlmsg_type == RTM_NEWADDR)
        {
            pIpDb = sys_l3_ipPoolIntfEntryPtr_ret(SYS_L3_AF_IPV6, &intfId, &l3Ip);

            if (NULL == pIpDb)
            {
                if (IS_IPV6_LINKLOCAL(l3Ip.v6.addr))
                {
                    sys_l3_ipv6_t linkLocalIp;

                    if (SYS_ERR_OK != sys_l3_ipv6LinkLocalAddr_get(&intfId, &linkLocalIp))
                    {
                        sal_l3_intfIPv6_add(&intfId, SYS_L3_IPTYPE_V6LINKLOCAL, (sys_l3_ipv6_t*)&l3Ip);
                    }
                    /* Link local address already exist */
                    else
                    {
                        char cmd[SYS_BUF128_LEN];
                        char ipStr[CAPA_IPV6_STR_LEN];

                        sys_l3_ipv6SysLinkLocal_get(&linkLocalIp);

                        if (IS_IPV6_EQUAL(linkLocalIp.addr, l3Ip.v6.addr))
                        {
                            SYS_MEM_CLEAR(cmd);
                            SYS_MEM_CLEAR(ipStr);
                            IPV62STR(linkLocalIp.addr, ipStr);

                            osal_sprintf(cmd, "ip -6 addr del %s/%d dev %s 1> /dev/null 2> /dev/null",
                                            ipStr,
                                            linkLocalIp.plen,
                                            ifname);
                            system(cmd);
                        }
                    }
                }
                else
                {
                    sal_l3_intfIPv6_add(&intfId, SYS_L3_IPTYPE_V6AUTOCONF, (sys_l3_ipv6_t*)&l3Ip);
                }
            }
        }
        else
        {
            pIpDb = sys_l3_ipPoolIntfEntryPtr_ret(SYS_L3_AF_IPV6, &intfId, &l3Ip);

            if (NULL != pIpDb)
            {
                if (SYS_L3_IPTYPE_STATIC == pIpDb->type
                        || SYS_L3_IPTYPE_V6LINKLOCAL == pIpDb->type)
                {
                    return SYS_ERR_OK;
                }

                sal_l3_intfIPv6_del(&intfId, SYS_L3_IPTYPE_ANY, (sys_l3_ipv6_t*)&l3Ip);
            }
        }
#endif
    }

  return SYS_ERR_OK;
}


static int32 netlink_parse_info(struct nlsock *nl)
{
    int32 status;
    int32 ret = 0;
    //int32 error;

    while (1)
    {
        char buf[NL_PKT_BUF_SIZE];
        struct iovec iov = { buf, sizeof(buf) };
        struct sockaddr_nl snl;
        struct msghdr msg = { (void *) &snl, sizeof(snl), &iov, 1, NULL, 0, 0 };
        struct nlmsghdr *h;

        status = recvmsg (nl->sock, &msg, 0);
        if (status < 0)
        {
            if (errno == EINTR)
                continue;
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                break;
            FPMD_DBG("%s recvmsg overrun: %s\n", nl->name, safe_strerror(errno));

            continue;
        }

        if (status == 0)
        {
            FPMD_DBG("%s EOF", nl->name);
            return SYS_ERR_FAILED;
        }

        if (msg.msg_namelen != sizeof snl)
        {
            FPMD_DBG("%s sender address length error: length %d\n", nl->name, msg.msg_namelen);
            return SYS_ERR_FAILED;
        }

        for (h = (struct nlmsghdr *) buf; NLMSG_OK (h, (unsigned int) status);
           h = NLMSG_NEXT (h, status))
        {
        	_addr_data_dump(buf, h->nlmsg_len, "intf addr netlink data!!!");
            /* Finish of reading. */
            if (h->nlmsg_type == NLMSG_DONE)
                return ret;

            /* Error handling. */
            if (h->nlmsg_type == NLMSG_ERROR)
            {
                struct nlmsgerr *err = (struct nlmsgerr *) NLMSG_DATA (h);
                int errnum = err->error;
                int msg_type = err->msg.nlmsg_type;

                /* If the error field is zero, then this is an ACK */
                if (err->error == 0)
                {
                    if (debug_kernel)
                    {
                        FPMD_DBG("%s:%d %s ACK: type=%u, seq=%u, pid=%u\n",
                                     __func__,
                                     __LINE__,
                                     nl->name,
                                     err->msg.nlmsg_type,
                                     err->msg.nlmsg_seq,
                                     err->msg.nlmsg_pid);
                    }

                    /* return if not a multipart message, otherwise continue */
                    if (!(h->nlmsg_flags & NLM_F_MULTI))
                    {
                        return 0;
                    }
                  continue;
                }

                if (h->nlmsg_len < NLMSG_LENGTH (sizeof (struct nlmsgerr)))
                {
                    FPMD_DBG("%s error: message truncated\n",
                                 nl->name);
                    return SYS_ERR_FAILED;
                }

                FPMD_DBG("%s error: %s, type=%u, seq=%u, pid=%u\n",
                            nl->name,
                            safe_strerror(-errnum),
                            msg_type,
                            err->msg.nlmsg_seq,
                            err->msg.nlmsg_pid);
                return SYS_ERR_FAILED;
            }

            /* OK we got netlink message. */
            if (debug_kernel)
                FPMD_DBG("netlink_parse_info: %s type %u, seq=%u, pid=%u\n",
                            nl->name,
                            h->nlmsg_type,
                            h->nlmsg_seq,
                            h->nlmsg_pid);

            netlink_interface_addr(&snl, h);
        }

        /* After error care. */
        if (msg.msg_flags & MSG_TRUNC)
        {
            FPMD_DBG("%s error: message truncated\n", nl->name);
            continue;
        }

        if (status)
        {
            FPMD_DBG("%s error: data remnant size %d\n", nl->name, status);
            return SYS_ERR_FAILED;
        }
    }
  return ret;
}


static int32 _kernel_read(void)
{
    int32 ret = 0;
    pthread_t id;

    netlink_parse_info(&netlink);

    if (0 != (ret = pthread_create(&id, NULL, (void *)_kernel_read, NULL)))
    {
        FPMD_DBG("%s:%d Create theard fail with error %d\n", __func__, __LINE__, ret);

        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

int32 addr_monitor_create(void)
{
    int32 ret = SYS_ERR_FAILED;
    pthread_t id;

    SYS_MEM_CLEAR(netlink);

    SYS_ERR_CHK(_netlink_socket(&netlink));

    if (0 != (ret = pthread_create(&id, NULL, (void *)_kernel_read, NULL)))
    {
        FPMD_DBG("%s:%d Create theard fail with error %d\n", __func__, __LINE__, ret);

        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

