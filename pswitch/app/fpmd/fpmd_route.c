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
 * Purpose : Forward plane manager - route translate and set
 *
 * Feature : Forward plane manager - route translate and set
 */

/*
 * Include Files
 */
#include <net/if.h>
#include <libsal/sal_l3.h>

#include "fpmd.h"

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */
#define RTATTR_TYPE(p) (p->rta_type)
#define RTATTR_LEN(p) (p->rta_len)
#define RTATTR_DATA(p) ((char*)p + sizeof(fpm_rtattr_t))
#define RTATTR_DATALEN(p) (p->rta_len - sizeof(fpm_rtattr_t))

/*
 * Function Declaration
 */
void _data_dump(char *data, uint32 len, char *title)
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

void _hdr_dump(fpm_route_hdr_t *pHdr)
{
    FPMD_DBG("nl.nlmsg_len   =%08X\n", pHdr->nl.nlmsg_len);
    FPMD_DBG("nl.nlmsg_type  =%04X\n", pHdr->nl.nlmsg_type);
    FPMD_DBG("nl.nlmsg_flags =%04X\n", pHdr->nl.nlmsg_flags);
    FPMD_DBG("nl.nlmsg_seq   =%08X\n", pHdr->nl.nlmsg_seq);
    FPMD_DBG("nl.nlmsg_pid   =%08X\n", pHdr->nl.nlmsg_pid);

    FPMD_DBG("rt.rtm_family  =%02X\n", pHdr->rt.rtm_family);
    FPMD_DBG("rt.rtm_dst_len =%02X\n", pHdr->rt.rtm_dst_len);
    FPMD_DBG("rt.rtm_src_len =%02X\n", pHdr->rt.rtm_src_len);
    FPMD_DBG("rt.rtm_tos     =%02X\n", pHdr->rt.rtm_tos);
    FPMD_DBG("rt.rtm_table   =%02X\n", pHdr->rt.rtm_table);
    FPMD_DBG("rt.rtm_protocol=%02X\n", pHdr->rt.rtm_protocol);
    FPMD_DBG("rt.rtm_scope   =%02X\n", pHdr->rt.rtm_scope);
    FPMD_DBG("rt.rtm_type    =%02X\n", pHdr->rt.rtm_type);
    FPMD_DBG("rt.rtm_flags   =%08X\n", pHdr->rt.rtm_flags);
}

void _attr_dump(uint32 type, uint32 datalen, char *data)
{
    char title[64];
    char typeStr[16];

    SYS_MEM_CLEAR(title);
    SYS_MEM_CLEAR(typeStr);

    switch (type)
    {
        case RTA_DST:
            osal_sprintf(typeStr, "RTA_DST");
            break;

        case RTA_GATEWAY:
            osal_sprintf(typeStr, "RTA_GATEWAY");
            break;

        case RTA_OIF:
            osal_sprintf(typeStr, "RTA_OIF");
            break;

        case RTA_PRIORITY:
            osal_sprintf(typeStr, "RTA_PRIORITY");
            break;

        default:
            osal_sprintf(typeStr, "Unkonwn");
            break;
    }

    osal_sprintf(title, "Attr type %u (%s)", type, typeStr);

    _data_dump(data, datalen, title);
}

void _intf_transl(uint32 intfIdx, sys_l3_intfId_t *pIntf)
{
    char ifname[IFNAMSIZ];

    SYS_MEM_CLEAR(ifname);

    if_indextoname(intfIdx, ifname);

    FPMD_DBG("ifname %s\n", ifname);

    STR2INTFID(ifname, pIntf);
}

int32 attr_v4_transl(uint32 type, uint32 datalen, char *data, sys_l3_routeV4_t *pRoute)
{
    _attr_dump(type, datalen, data);

    switch (type)
    {
        case RTA_DST:
            osal_memcpy(&pRoute->dst, data, datalen);
            break;

        case RTA_GATEWAY:
            osal_memcpy(&pRoute->gw, data, datalen);
            break;

        case RTA_OIF:
            _intf_transl(*(uint32*)data, &pRoute->ointf);
            break;

        case RTA_PRIORITY:
            pRoute->pri = *(uint32*)data;
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}

int32 attr_v6_transl(uint32 type, uint32 datalen, char *data, sys_l3_routeV6_t *pRoute)
{
    _attr_dump(type, datalen, data);

    switch (type)
    {
        case RTA_DST:
            osal_memcpy(&pRoute->dst, data, datalen);
            break;

        case RTA_GATEWAY:
            osal_memcpy(&pRoute->gw, data, datalen);
            break;

        case RTA_OIF:
            _intf_transl(*(uint32*)data, &pRoute->ointf);
            break;

        case RTA_PRIORITY:
            pRoute->pri = *(uint32*)data;
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}

int32 route_v4_process(sys_l3_act_t act, sys_l3_routeV4_t *pRoute)
{
    char ipStr[CAPA_IPV4_STR_LEN];
    char gwStr[CAPA_IPV4_STR_LEN];

    switch (act)
    {
        case SYS_L3_ACT_ROUTEADD:
            /* Do not need add route for lo interface */
            //if (SYS_L3_INTFTYPE_LO == pRoute->ointf.type)
            //    return SYS_ERR_OK;

            if (SYS_L3_INTFTYPE_MGMTVLAN == pRoute->ointf.type)
            {
                if (IS_IPV4_ALLZERO(pRoute->dst))
                    return SYS_ERR_OK;
            }
            SYS_ERR_CHK(sal_l3_routeV4_add(pRoute));

            SYS_MEM_CLEAR(ipStr);
            SYS_MEM_CLEAR(gwStr);
            IPV42STR(pRoute->dst, ipStr);
            IPV42STR(pRoute->gw, gwStr);
            SYSTEM_CMD_HID("ip route add %s/%d via %s table %s", ipStr, pRoute->dstLen, gwStr, L3_INTF_MGMTVLAN);
            break;

        case SYS_L3_ACT_ROUTEDEL:
            /* Do not need delete route for lo interface */
            if (SYS_L3_INTFTYPE_LO == pRoute->ointf.type)
                return SYS_ERR_OK;
            SYS_ERR_CHK(sal_l3_routeV4_del(pRoute));

            SYS_MEM_CLEAR(ipStr);
            SYS_MEM_CLEAR(gwStr);
            IPV42STR(pRoute->dst, ipStr);
            IPV42STR(pRoute->gw, gwStr);
            SYSTEM_CMD_HID("ip route del %s/%d via %s table %s", ipStr, pRoute->dstLen, gwStr, L3_INTF_MGMTVLAN);
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}

int32 route_v6_process(sys_l3_act_t act, sys_l3_routeV6_t *pRoute)
{
    switch (act)
    {
        case SYS_L3_ACT_ROUTEADD:
            /* Do not need add route for lo interface */
            //if (SYS_L3_INTFTYPE_LO == pRoute->ointf.type)
            //    return SYS_ERR_OK;

            /* New default route is coming from mgmt vlan. */
            if (SYS_L3_INTFTYPE_MGMTVLAN == pRoute->ointf.type
                && IS_IPV6_ALLZERO(pRoute->dst))
            {
                char ipStr[CAPA_IPV6_STR_LEN];

                SYS_MEM_CLEAR(ipStr);

                IPV62STR(pRoute->gw, ipStr);
                SYSTEM_CMD_HID("ip -6 route del default dev %s", L3_INTF_MGMTVLAN);
                SYSTEM_CMD_HID("ip -6 route add default via %s dev %s table %s", ipStr, L3_INTF_MGMTVLAN, L3_INTF_MGMTVLAN);

                return SYS_ERR_OK;

            }

            SYS_ERR_CHK(sal_l3_routeV6_add(pRoute));
            break;

        case SYS_L3_ACT_ROUTEDEL:
            /* Do not need delete route for lo interface */
            if (SYS_L3_INTFTYPE_LO == pRoute->ointf.type)
                return SYS_ERR_OK;

            SYS_ERR_CHK(sal_l3_routeV6_del(pRoute));
            break;

        default:
            break;
    }
    return SYS_ERR_OK;
}

int32 route_transl(char *data, uint32 len)
{
    int32 currLen = len;
    int32 attrLen = 0;
    fpm_route_hdr_t *pHdr = (fpm_route_hdr_t*) data;
    fpm_rtattr_t *pAttrFirst = (fpm_rtattr_t*)((char *)pHdr + sizeof(fpm_route_hdr_t));
    fpm_rtattr_t *pAttr = pAttrFirst;
    sys_l3_act_t act = SYS_L3_ACT_NONE;
    sys_l3_af_t family = SYS_L3_AF_IPV4;
    sys_l3_routeV4_t *pRouteV4 = NULL;
    sys_l3_routeV6_t *pRouteV6 = NULL;

    _data_dump(data, len, "All datas");
    _hdr_dump(pHdr);

    /* Translate route action */
    switch (pHdr->nl.nlmsg_type)
    {
        case RTM_NEWROUTE:
            act = SYS_L3_ACT_ROUTEADD;
            break;

        case RTM_DELROUTE:
            act = SYS_L3_ACT_ROUTEDEL;
            break;

        default:
            break;
    }

    /* Translate route address family */
    switch (pHdr->rt.rtm_family)
    {
        case AF_INET:
            family = SYS_L3_AF_IPV4;

            if (NULL == (pRouteV4 = osal_alloc(sizeof(sys_l3_routeV4_t))))
                return SYS_ERR_OUT_OF_MEMORY;

            osal_memset(pRouteV4, 0, sizeof(sys_l3_routeV4_t));

            pRouteV4->dstLen = pHdr->rt.rtm_dst_len;

            /* reject route will not send ATTR RTA_OIF, so default assign NULL here */
            if (SYS_L3_ACT_ROUTEADD == act)
                pRouteV4->ointf.type = SYS_L3_INTFTYPE_NULL;
                break;

        case AF_INET6:
            family = SYS_L3_AF_IPV6;

            if (NULL == (pRouteV6 = osal_alloc(sizeof(sys_l3_routeV6_t))))
                return SYS_ERR_OUT_OF_MEMORY;

            osal_memset(pRouteV6, 0, sizeof(sys_l3_routeV6_t));

            pRouteV6->dstLen = pHdr->rt.rtm_dst_len;

            /* reject route will not send ATTR RTA_OIF, so default assign NULL here */
            if (SYS_L3_ACT_ROUTEADD == act)
                pRouteV6->ointf.type = SYS_L3_INTFTYPE_NULL;
            break;

        default:
            return SYS_ERR_FAILED;
    }

    /* Iterate and translate all attributes */
    currLen -= sizeof(fpm_route_hdr_t);
    attrLen = RTATTR_LEN(pAttr);

    while (currLen > 0)
    {
        if (pAttr != pAttrFirst)
            attrLen = RTATTR_LEN(pAttr);

        if (SYS_L3_AF_IPV4 == family)
            attr_v4_transl(RTATTR_TYPE(pAttr), RTATTR_DATALEN(pAttr), RTATTR_DATA(pAttr), pRouteV4);
        else if (SYS_L3_AF_IPV6 == family)
            attr_v6_transl(RTATTR_TYPE(pAttr), RTATTR_DATALEN(pAttr), RTATTR_DATA(pAttr), pRouteV6);

        pAttr = (fpm_rtattr_t*)((char *)pAttr + attrLen);
        currLen -= attrLen;
    }

    /* Process routing entry */
    if (SYS_L3_AF_IPV4 == family)
    {
        route_v4_process(act, pRouteV4);

        if (NULL != pRouteV4)
        {
            FPMD_DBG("%s route "FMT_IPV4"/%u GW:"FMT_IPV4" INTF:%d_%d\n",
                    (SYS_L3_ACT_ROUTEADD == act) ? "Add" : "Del",
                    SHOW_IPV4(pRouteV4->dst),
                    pRouteV4->dstLen,
                    SHOW_IPV4(pRouteV4->gw),
                    pRouteV4->ointf.type,
                    pRouteV4->ointf.id);

            osal_free(pRouteV4);
        }
    }
    else if (SYS_L3_AF_IPV6 == family)
    {
        route_v6_process(act, pRouteV6);

        FPMD_DBG("%s route "FMT_IPV6"/%u GW:"FMT_IPV6" INTF:%d_%d\n",
                    (SYS_L3_ACT_ROUTEADD == act) ? "Add" : "Del",
                    SHOW_IPV6(pRouteV6->dst),
                    pRouteV6->dstLen,
                    SHOW_IPV6(pRouteV6->gw),
                    pRouteV6->ointf.type,
                    pRouteV6->ointf.id);

        if (NULL != pRouteV6)
        {
            FPMD_DBG("%s route "FMT_IPV6"/%u GW:"FMT_IPV6" INTF:%d_%d\n",
                    (SYS_L3_ACT_ROUTEADD == act) ? "Add" : "Del",
                    SHOW_IPV6(pRouteV6->dst),
                    pRouteV6->dstLen,
                    SHOW_IPV6(pRouteV6->gw),
                    pRouteV6->ointf.type,
                    pRouteV6->ointf.id);

            osal_free(pRouteV6);
        }
    }

    return SYS_ERR_OK;
}

