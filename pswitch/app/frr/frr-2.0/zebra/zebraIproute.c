/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 �ļ�����: zebraIproute.c
 ����: ���ļ���zebra_vty.c�ļ���д����ɾ�̬ip��ַ�����ò���
 �޶���¼��
 �޶�1��
*******************************************************************************/
/***********************************ͷ�ļ�*************************************/
#include <zebra.h>

#include "prefix.h"
#include "command.h"
#include "if.h"
#include "thread.h"
#include "stream.h"
#include "memory.h"
#include "zebra_memory.h"
#include "table.h"
#include "rib.h"
#include "network.h"
#include "sockunion.h"
#include "log.h"
#include "zclient.h"
#include "privs.h"
#include "network.h"
#include "buffer.h"
#include "nexthop.h"
#include "vrf.h"

#include "zebra/zserv.h"
#include "zebra/zebra_ns.h"
#include "zebra/zebra_vrf.h"
#include "zebra/router-id.h"
#include "zebra/redistribute.h"
#include "zebra/debug.h"
#include "zebra/ipforward.h"
#include "zebra/zebra_rnh.h"
#include "zebra/rt_netlink.h"
#include "zebra/interface.h"
#include "zebra/zebra_ptm.h"
#include "zebra/rtadv.h"
#include "zebra/zebra_mpls.h"
#include "zebra/zebra_fpm.h"
#include "zebra/zebraIproute.h"
#include "zebra/zebra_static.h"


#include <libipc/vos.h>



/* General function for static route. */
int
zebra_static_ipv4_pswitch (safi_t safi, int add_cmd,
		   const char *dest_str, const char *mask_str,
		   const char *gate_str, const char *flag_str,
		   const char *tag_str, const char *distance_str,
		   const char *vrf_id_str, const char *label_str)
{
  int ret;
  u_char distance;
  struct prefix p;
  struct in_addr gate;
  struct in_addr mask;
  u_char flag = 0;
  route_tag_t tag = 0;
  struct zebra_vrf *zvrf = NULL;
  unsigned int ifindex = 0;
  const char *ifname = NULL;
  u_char type = STATIC_BLACKHOLE;
  struct static_nh_label snh_label;

  memset (&snh_label, 0, sizeof (struct static_nh_label));
  ret = str2prefix (dest_str, &p);
  if (ret <= 0)
    {
      printf ("Malformed address");
      return VOS_ERR_PARAM;
    }

  /* Cisco like mask notation. */
  if (mask_str)
    {
      ret = inet_aton (mask_str, &mask);
      if (ret == 0)
        {
          printf ("Malformed address");
          return VOS_ERR_PARAM;
        }
      p.prefixlen = ip_masklen (mask);
    }

  /* Apply mask for given prefix. */
  apply_mask (&p);

  /* Administrative distance. */
  if (distance_str)
    distance = atoi (distance_str);
  else
    distance = ZEBRA_STATIC_DISTANCE_DEFAULT;

  /* tag */
  //if (tag_str)
   // VTY_GET_INTEGER_RANGE("tag", tag, tag_str, 0, 4294967295);

  /* VRF id */
  zvrf = zebra_vrf_lookup_by_name (vrf_id_str);

  if (!zvrf)
    {
      printf ("vrf %s is not defined", vrf_id_str);
      return VOS_ERR_FAIL;
    }

  /* Labels */
  if (label_str)
    {
      if (!mpls_enabled)
	{
	  printf (" MPLS not turned on in kernel, ignoring command");
	  return VOS_ERR_PARAM;
	}
      if (mpls_str2label (label_str, &snh_label.num_labels,
                          snh_label.label))
        {
          //vty_out (vty, "%% Malformed label(s)%s", VTY_NEWLINE);
          return VOS_ERR_FAIL;
        }
    }

  /* Null0 static route.  */
  if ((gate_str != NULL) && (strncasecmp (gate_str, "Null0", strlen (gate_str)) == 0))
    {
      if (flag_str)
        {
          printf ("can not have flag %s with Null0", flag_str);
          return CMD_WARNING;
        }
      if (add_cmd)
        static_add_route (AFI_IP, safi, type, &p, NULL, ifindex, ifname,
			  ZEBRA_FLAG_BLACKHOLE, tag, distance, zvrf, &snh_label);
      else
        static_delete_route (AFI_IP, safi, type, &p, NULL, ifindex, tag,
			     distance, zvrf, &snh_label);
      return VOS_ERR_NONE;
    }

  /* Route flags */
  if (flag_str) {
    switch(flag_str[0]) {
      case 'r':
      case 'R': /* XXX */
        SET_FLAG (flag, ZEBRA_FLAG_REJECT);
        break;
      case 'b':
      case 'B': /* XXX */
        SET_FLAG (flag, ZEBRA_FLAG_BLACKHOLE);
        break;
      default:
        //vty_out (vty, "%% Malformed flag %s %s", flag_str, VTY_NEWLINE);
        return VOS_ERR_FAIL;
    }
  }

  if (gate_str == NULL)
  {
    if (add_cmd)
      static_add_route (AFI_IP, safi, type, &p, NULL, ifindex, ifname, flag,
			tag, distance, zvrf, &snh_label);
    else
      static_delete_route (AFI_IP, safi, type, &p, NULL, ifindex, tag, distance,
			   zvrf, &snh_label);

    return VOS_ERR_NONE;
  }
  
  /* When gateway is A.B.C.D format, gate is treated as nexthop
     address other case gate is treated as interface name. */
  ret = inet_aton (gate_str, &gate);
  if (!ret)
    {
      struct interface *ifp = if_lookup_by_name_vrf (gate_str, zvrf_id (zvrf));
      if (!ifp)
        {
	  //vty_out (vty, "%% Unknown interface: %s%s", gate_str, VTY_NEWLINE);
          ifindex = IFINDEX_DELETED;
        }
      else
        ifindex = ifp->ifindex;
      ifname = gate_str;
      type = STATIC_IFINDEX;
    }
  else
    type = STATIC_IPV4_GATEWAY;

  if (add_cmd)
    static_add_route (AFI_IP, safi, type, &p,
		      ifindex ? NULL : (union g_addr *)&gate, ifindex, ifname,
		      flag, tag, distance, zvrf, &snh_label);
  else
    static_delete_route (AFI_IP, safi, type, &p,
			 ifindex ? NULL : (union g_addr *)&gate, ifindex, tag,
			 distance, zvrf, &snh_label);

  return CMD_SUCCESS;
}

/*******************************************************************************
�������ƣ�zebraIpRouteAdd
�������������Ӿ�̬·��
���������strNetwork��·�ɵ������ַ�������ʽA.B.C.D������10.0.0.2
          strNetmask��·�ɵ������ַ�������ʽA.B.C.D������255.0.0.0
          strGateway��·�ɵ������ַ���������Ϊ�������
                      A.B.C.D�ĸ�ʽ�����ʾIpv4��ַ��ʽ
                      ip�ӿ����ƣ����ʾָ��ip�ӿ���Ϊ����
                      null0�����ʾ��װ�ڶ�·��
          ulDistance��·�ɵľ��룬��ΧΪ1~255��Ϊ0��ʾ������
          ulFlags��·�ɵı�ǣ��μ�ZEBRA_ROUTE_FLAG_ENT
�����������
�������أ�VD_E_NONE, �ɹ�
          VD_E_FAIL��ִ��ʧ��
����˵������
*******************************************************************************/
int zebraIpRouteAdd(char *strNetwork, char *strNetmask, char *strGateway,
    ulong_t ulDistance, ulong_t ulFlags)
{
    char                    *pszDistance, *pszFlags;
    char                     szDistance[32], szFlags[32];

    pszFlags = NULL;
    if ( ulFlags )
    {
        pszFlags = szFlags;
        #if 0
        if ( ZEBRA_ROUTE_FLAG_REJECT & ulFlags )
        {
            szFlags[0] = 'R';
        }
        if ( ZEBRA_ROUTE_FLAG_BLACKHOLE & ulFlags )
        {
            szFlags[0] = 'B';
        }
        #endif
    }
    
    pszDistance = NULL;
    if ( ulDistance )
    {
        pszDistance = szDistance;
        sprintf(szDistance, "%d", ulDistance);
    }

    return zebra_static_ipv4_pswitch(SAFI_UNICAST, 1, strNetwork, strNetmask, strGateway, pszFlags, NULL, pszDistance, NULL, NULL);
}

/*******************************************************************************
�������ƣ�zebraIpRouteDelete
����������ɾ����̬·��
���������strNetwork��·�ɵ������ַ�������ʽA.B.C.D������10.0.0.2
          strNetmask��·�ɵ������ַ�������ʽA.B.C.D������255.0.0.0
          strGateway��·�ɵ������ַ���������Ϊ�������
                      A.B.C.D�ĸ�ʽ�����ʾIpv4��ַ��ʽ
                      ip�ӿ����ƣ����ʾָ��ip�ӿ���Ϊ����
                      null0�����ʾ��װ�ڶ�·��
          ulDistance��·�ɵľ��룬��ΧΪ1~255��Ϊ0��ʾ������
          ulFlags��·�ɵı�ǣ��μ�ZEBRA_ROUTE_FLAG_ENT
�����������
�������أ�VD_E_NONE, �ɹ�
          VD_E_FAIL��ִ��ʧ��
����˵������
*******************************************************************************/
int zebraIpRouteDelete(char *strNetwork, char *strNetmask, char *strGateway,
    ulong_t ulDistance, ulong_t ulFlags)
{
    char                    *pszDistance, *pszFlags;
    char                     szDistance[32], szFlags[32];

    pszFlags = NULL;
    if ( ulFlags )
    {
    #if 0
        pszFlags = szFlags;
        if ( ZEBRA_ROUTE_FLAG_REJECT & ulFlags )
        {
            szFlags[0] = 'R';
        }
        if ( ZEBRA_ROUTE_FLAG_BLACKHOLE & ulFlags )
        {
            szFlags[0] = 'B';
        }
    #endif
    }
    
    pszDistance = NULL;
    if ( ulDistance )
    {
        pszDistance = szDistance;
        sprintf(szDistance, "%d", ulDistance);
    }

    return zebra_static_ipv4_pswitch(SAFI_UNICAST ,0, strNetwork, strNetmask, strGateway, pszFlags, NULL, pszDistance, NULL, NULL);
}

/* General fucntion for IPv6 static route. */
static int
static_ipv6_func_pswitch (int add_cmd, const char *dest_str,
		  const char *gate_str, const char *ifname,
		  const char *flag_str, const char *tag_str,
          const char *distance_str, const char *vrf_id_str,
		  const char *label_str)

{
  int ret;
  u_char distance;
  struct prefix p;
  struct in6_addr *gate = NULL;
  struct in6_addr gate_addr;
  u_char type = STATIC_BLACKHOLE;
  u_char flag = 0;
  route_tag_t tag = 0;
  unsigned int ifindex = 0;
  struct interface *ifp = NULL;
  struct zebra_vrf *zvrf;
  struct static_nh_label snh_label;
  
  ret = str2prefix (dest_str, &p);
  if (ret <= 0)
    {
      //vty_out (vty, "%% Malformed address%s", VTY_NEWLINE);
      return VOS_ERR_PARAM;
    }

  /* Apply mask for given prefix. */
  apply_mask (&p);

  /* Administrative distance. */
  if (distance_str)
    distance = atoi (distance_str);
  else
    distance = ZEBRA_STATIC_DISTANCE_DEFAULT;

  /* tag */
  //if (tag_str)
    //VTY_GET_INTEGER_RANGE("tag", tag, tag_str, 0, 4294967295);

  /* When gateway is valid IPv6 addrees, then gate is treated as
     nexthop address other case gate is treated as interface name. */
  ret = inet_pton (AF_INET6, gate_str, &gate_addr);

  /* VRF id */
  zvrf = zebra_vrf_lookup_by_name (vrf_id_str);

  if (!zvrf)
    {
      //vty_out (vty, "%% vrf %s is not defined%s", vrf_id_str, VTY_NEWLINE);
      return VOS_ERR_PARAM;
    }

  /* Labels */
  memset (&snh_label, 0, sizeof (struct static_nh_label));
  if (label_str)
    {
      if (!mpls_enabled)
	{
	  //vty_out (vty, "%% MPLS not turned on in kernel, ignoring command%s",
		  // VTY_NEWLINE);
	  return VOS_ERR_PARAM;
	}
      if (mpls_str2label (label_str, &snh_label.num_labels,
                          snh_label.label))
        {
          //vty_out (vty, "%% Malformed label(s)%s", VTY_NEWLINE);
          return VOS_ERR_PARAM;
        }
    }

  /* Null0 static route.  */
  if ((gate_str != NULL) && (strncasecmp (gate_str, "Null0", strlen (gate_str)) == 0))
    {
      if (flag_str)
        {
          //vty_out (vty, "%% can not have flag %s with Null0%s", flag_str, VTY_NEWLINE);
          return VOS_ERR_PARAM;
        }
      if (add_cmd)
        static_add_route (AFI_IP6, SAFI_UNICAST, type, &p, NULL, ifindex, ifname,
			  ZEBRA_FLAG_BLACKHOLE, tag, distance, zvrf, &snh_label);
      else
        static_delete_route (AFI_IP6, SAFI_UNICAST, type, &p,  NULL, ifindex, tag,
			     distance, zvrf, &snh_label);
      return VOS_ERR_NONE;
    }

  /* Route flags */
  if (flag_str) {
    switch(flag_str[0]) {
      case 'r':
      case 'R': /* XXX */
        SET_FLAG (flag, ZEBRA_FLAG_REJECT);
        break;
      case 'b':
      case 'B': /* XXX */
        SET_FLAG (flag, ZEBRA_FLAG_BLACKHOLE);
        break;
      default:
        //vty_out (vty, "%% Malformed flag %s %s", flag_str, VTY_NEWLINE);
        return VOS_ERR_PARAM;
    }
  }

 if (ifname)
    {
      /* When ifname is specified.  It must be come with gateway
         address. */
      if (ret != 1)
	{
	  //vty_out (vty, "%% Malformed address%s", VTY_NEWLINE);
	  return VOS_ERR_PARAM;
	}
      type = STATIC_IPV6_GATEWAY_IFINDEX;
      gate = &gate_addr;
      ifp = if_lookup_by_name_vrf (ifname, zvrf_id (zvrf));
      if (!ifp)
	{
	  //vty_out (vty, "%% Malformed Interface name %s%s", ifname, VTY_NEWLINE);
	  return VOS_ERR_PARAM;
	}
      ifindex = ifp->ifindex;
    }
  else
    {
      if (ret == 1)
	{
	  type = STATIC_IPV6_GATEWAY;
	  gate = &gate_addr;
	}
      else
	{
	  type = STATIC_IFINDEX;
	  ifp = if_lookup_by_name_vrf (gate_str, zvrf_id (zvrf));
	  if (!ifp)
	    {
	      //vty_out (vty, "%% Malformed Interface name %s%s", gate_str, VTY_NEWLINE);
              ifindex = IFINDEX_DELETED;
	    }
          else
	    ifindex = ifp->ifindex;
	  ifname = gate_str;
	}
    }

  if (add_cmd)
    static_add_route (AFI_IP6, SAFI_UNICAST, type, &p, (union g_addr *)gate,
		      ifindex, ifname, flag, tag, distance, zvrf, &snh_label);
  else
    static_delete_route (AFI_IP6, SAFI_UNICAST, type, &p, (union g_addr *)gate,
			 ifindex, tag, distance, zvrf, &snh_label);

  return VOS_ERR_NONE;
}

/*******************************************************************************
�������ƣ�zebraIpRouteGet
�����������ں���Ч·�ɵĻ�ȡ����
���������op��֧��DATA_OP_GET��DATA_OP_GETNEXT����
          ulVrfId��vrf��ǣ�����ֻ֧��VD_VR_PUBLIC
          pIpRoute��������DATA_OP_GETNEXT��ѯ��һ��ʱ����ҪipΪ0��maskΪȫff
          safi SAFI_UNICAST  SAFI_MULTICAST
�����������
�������أ�VD_E_NONE, �ɹ�
          VD_E_FAIL��ִ��ʧ��
����˵������
*******************************************************************************/
int zebraIpRouteGet (DATA_OP_ENT op, const char *vrf_name, safi_t safi, ZEBRA_IP_ROUTE_STRC *pRoute)
{
	struct route_table *table;
	struct route_node *rn;
	struct rib *rib;
	struct nexthop          *nexthop;
	int first = 1;
	struct zebra_vrf *zvrf = NULL;
	char buf[BUFSIZ];
	struct prefix       ipPrefix;
	struct in_addr          mask;
	int                      iRet, iNexthopNum;

	if (!(zvrf = zebra_vrf_lookup_by_name (vrf_name)))
	{
	    printf ("vrf %s not defined", vrf_name);
	  return VOS_ERR_NONE;
	}

	if (zvrf_id (zvrf) == VRF_UNKNOWN)
	{
	    printf ("vrf %s inactive", vrf_name);
	  return VOS_ERR_NONE;
	}

	table = zebra_vrf_table (AFI_IP, safi, zvrf_id (zvrf));
	if (! table)
	{
	  	return VOS_ERR_NONE;
	}

	ipPrefix.family = AF_INET;
	//mask.s_addr = htonl(pRoute->ulDestMask);
	//ipPrefix.prefixlen = ip_masklen(mask);
	ipPrefix.prefixlen = pRoute->ulpen;
	ipPrefix.u.prefix4.s_addr = htonl(pRoute->ulDestIp);

	if ( DATA_OP_GET == op )
	{
		for (rn = route_top (table); rn; rn = route_next (rn))
		{
		    if (prefix_match (&ipPrefix, &rn->p))
		    {
		        break;
		    }
		}

		if ( NULL == rn )
		{
		    return VOS_ERR_FAIL;
		}

		RNODE_FOREACH_RIB (rn, rib)
		{
		    if ( (ZEBRA_ROUTE_KERNEL == rib->type) || (ZEBRA_ROUTE_CONNECT == rib->type) || (ZEBRA_ROUTE_STATIC == rib->type)
		        || CHECK_FLAG (rib->flags, ZEBRA_FLAG_SELECTED))
		    {
		        break;
		    }
		}
	}
	else if ( DATA_OP_GETNEXT == op )
    {
        if ( (0 == pRoute->ulDestIp) && (32 == pRoute->ulpen) )
        {
            rn = route_top(table);
        }
        else
        {
            rn = route_table_get_next(table, &ipPrefix);
        }

        while ( NULL != rn )
        {
            RNODE_FOREACH_RIB (rn, rib)
            {
                if ( (ZEBRA_ROUTE_KERNEL == rib->type) || (ZEBRA_ROUTE_CONNECT == rib->type) || (ZEBRA_ROUTE_STATIC == rib->type)
                    || (ZEBRA_ROUTE_OSPF == rib->type) || (ZEBRA_ROUTE_RIP == rib->type) || CHECK_FLAG (rib->flags, ZEBRA_FLAG_SELECTED))
                {
                    break;
                }
            }

            if ( NULL != rib )
            {
                break;
            }

            memcpy(&ipPrefix, &rn->p, sizeof(struct prefix));
            route_unlock_node(rn);
            rn = route_table_get_next(table, &ipPrefix);
        }

        if ( NULL == rn )
        {
            return VOS_ERR_FAIL;
        }
    }
    else
    {
        return VOS_ERR_FAIL;
    }

  iRet = VOS_ERR_FAIL;
  if ( NULL != rib )
  {
	  iRet = VOS_ERR_NONE;
	  pRoute->ulDestIp = ntohl(rn->p.u.prefix4.s_addr);
	  //masklen2ip(rn->p.prefixlen, &mask);
	  //pRoute->ulDestMask = ntohl(mask.s_addr);
	  pRoute->ulpen = rn->p.prefixlen;
	  pRoute->ulDistance = rib->distance;
	  pRoute->ulMetric = rib->metric;
	  //pRoute->ulRouteTotal = rib_stats.ulRepRouteIpv4Count;
	  pRoute->type = rib->type;
	  pRoute->select = CHECK_FLAG (rib->flags, ZEBRA_FLAG_SELECTED) ? 1 : 0;

	  iNexthopNum = 0;
	  memset(pRoute->ulNexthop, 0, sizeof(pRoute->ulNexthop));
	  memset(pRoute->ulIfIndex, 0, sizeof(pRoute->ulIfIndex));

	  for (nexthop = rib->nexthop; nexthop; nexthop = nexthop->next)
	  {  
	  	if ( CHECK_FLAG (nexthop->flags, NEXTHOP_FLAG_ACTIVE))	
	  		pRoute->active = 1;
	  	if ( CHECK_FLAG (nexthop->flags, NEXTHOP_FLAG_FIB))	
	  		pRoute->fib = 1;
	  		
		  switch (nexthop->type)
		  {
			case NEXTHOP_TYPE_IPV4:
			case NEXTHOP_TYPE_IPV4_IFINDEX:
				if (nexthop->ifindex)
					sprintf(pRoute->devStr, "%s", ifindex2ifname_vrf (nexthop->ifindex, rib->vrf_id));
				pRoute->ulNexthop[iNexthopNum] = ntohl(nexthop->gate.ipv4.s_addr);
				break;
		    case NEXTHOP_TYPE_IPV6:
			case NEXTHOP_TYPE_IPV6_IFINDEX:
				if (nexthop->ifindex)
					sprintf(pRoute->devStr, "%s", ifindex2ifname_vrf (nexthop->ifindex, rib->vrf_id));
				break;
			case NEXTHOP_TYPE_IFINDEX:
				sprintf(pRoute->devStr, "%s", ifindex2ifname_vrf (nexthop->ifindex, rib->vrf_id));
				break;
			case NEXTHOP_TYPE_BLACKHOLE:
				sprintf(pRoute->devStr, "Null0");
				break;
			default:
		  		break;
		  }
	  }
  }

  route_unlock_node(rn);
  
  return iRet;
}

/*******************************************************************************
�������ƣ�zebraIpv6RouteAdd
��������������ipv6��̬·��
���������strIpPrefixLen��·�ɵ������ַ�������ʽX:X::X:X/M ������3ffe:506::/32
          strGateway��·�ɵ������ַ�������ʽX:X::X:X������2ffe:506::1
          strIfName��·�ɵ����ؽӿ��ַ���������ΪNULL
�����������
�������أ�VD_E_NONE, �ɹ�
          VD_E_FAIL��ִ��ʧ��
����˵������
*******************************************************************************/
int zebraIpv6RouteAdd(char *strIpPrefixLen, char *strGateway, char *strIfName)
{
    return static_ipv6_func_pswitch(1, strIpPrefixLen, strGateway, strIfName, NULL, NULL, NULL, NULL, NULL);
}

/*******************************************************************************
�������ƣ�zebraIpv6RouteDelete
����������ɾ��ipv6��̬·��
���������strIpPrefixLen��·�ɵ������ַ�������ʽX:X::X:X/M ������3ffe:506::/32
          strGateway��·�ɵ������ַ�������ʽX:X::X:X������2ffe:506::1
          strIfName��·�ɵ����ؽӿ��ַ���������ΪNULL
�����������
�������أ�VD_E_NONE, �ɹ�
          VD_E_FAIL��ִ��ʧ��
����˵������
*******************************************************************************/
int zebraIpv6RouteDelete(char *strIpPrefixLen, char *strGateway, char *strIfName)
{
    return static_ipv6_func_pswitch(0, strIpPrefixLen, strGateway, strIfName, NULL, NULL, NULL, NULL, NULL);
}

/*******************************************************************************
�������ƣ�zebraIpRouteGet
�����������ں���Ч·�ɵĻ�ȡ����
���������op��֧��DATA_OP_GET��DATA_OP_GETNEXT����
          ulVrfId��vrf��ǣ�����ֻ֧��VD_VR_PUBLIC
          pIpv6Route��������DATA_OP_GETNEXT��ѯ��һ��ʱ����ҪipΪ0��prefixlenΪ128
�����������
�������أ�VD_E_NONE, �ɹ�
          VD_E_FAIL��ִ��ʧ��
����˵������
*******************************************************************************/
int zebraIpv6RouteGet(DATA_OP_ENT op, const char *vrf_name, ZEBRA_IPV6_ROUTE_STRC *pIpv6Route)
{
    int                      iRet, iNexthopNum;
    struct route_table     *table;
    struct route_node      *rn;
    struct rib             *rib;
	struct zebra_vrf *zvrf = NULL;
    struct nexthop         *nexthop;
    struct prefix           ipPrefix;
    uchar_t                  zeroIp[16] = { 0 };
    vrf_id_t vrf_id = VRF_DEFAULT;
	char buf[BUFSIZ];
	int i = 0;

    if ( NULL == pIpv6Route )
    {
        return VOS_ERR_FAIL;
    }

	if (!(zvrf = zebra_vrf_lookup_by_name (vrf_name)))
	   {
			printf ("vrf %s not defined", vrf_name);
		   return VOS_ERR_NONE;
	   }
	
	 if (zvrf_id (zvrf) == VRF_UNKNOWN)
	   {
		   printf ("vrf %s inactive", vrf_name);
		 return VOS_ERR_NONE;
	   }
	 else
	   vrf_id = zvrf_id (zvrf);

    table = zebra_vrf_table (AFI_IP6, SAFI_UNICAST, vrf_id);
	if (! table)
    {
      return VOS_ERR_NONE;
    }


    ipPrefix.family = AF_INET6;
    ipPrefix.prefixlen = pIpv6Route->ulPrefixLen;
    memcpy(ipPrefix.u.prefix6.s6_addr, pIpv6Route->ucDestIp, 16);

    if ( DATA_OP_GET == op )
    {
        for (rn = route_top (table); rn; rn = route_next (rn))
        {
            if (prefix_match (&ipPrefix, &rn->p))
            {
                break;
            }
        }

        if ( NULL == rn )
        {
            return VOS_ERR_FAIL;
        }

        RNODE_FOREACH_RIB (rn, rib)
        {
            if ( (ZEBRA_ROUTE_KERNEL == rib->type) || (ZEBRA_ROUTE_CONNECT == rib->type) || (ZEBRA_ROUTE_STATIC == rib->type)
                || CHECK_FLAG (rib->flags, ZEBRA_FLAG_SELECTED))
            {
                break;
            }
        }
    }
    else if ( DATA_OP_GETNEXT == op )
    {
        if ( (0 == memcmp(zeroIp, pIpv6Route->ucDestIp, 16)) && (128 == pIpv6Route->ulPrefixLen) )
        {
            rn = route_top(table);
        }
        else
        {
            rn = route_table_get_next(table, &ipPrefix);
        }

        while ( NULL != rn )
        {
            RNODE_FOREACH_RIB (rn, rib)
            {
                if ( (ZEBRA_ROUTE_KERNEL == rib->type) || (ZEBRA_ROUTE_CONNECT == rib->type) || (ZEBRA_ROUTE_STATIC == rib->type)
                    || CHECK_FLAG (rib->flags, ZEBRA_FLAG_SELECTED))
                {
                    break;
                }
            }

            if ( NULL != rib )
            {
                break;
            }

            memcpy(&ipPrefix, &rn->p, sizeof(struct prefix));
            route_unlock_node(rn);
            rn = route_table_get_next(table, &ipPrefix);
        }

        if ( NULL == rn )
        {
            return VOS_ERR_FAIL;
        }
    }
    else
    {
        return VOS_ERR_FAIL;
    }

    iRet = VOS_ERR_FAIL;
    if ( NULL != rib )
	{
		  iRet = VOS_ERR_NONE;
		  memcpy(pIpv6Route->ucDestIp, rn->p.u.prefix6.s6_addr, 16);
		  //masklen2ip(rn->p.prefixlen, &mask);
		  //pRoute->ulDestMask = ntohl(mask.s_addr);
		  pIpv6Route->ulPrefixLen = rn->p.prefixlen;
		  pIpv6Route->ulDistance = rib->distance;
		  pIpv6Route->ulMetric = rib->metric;

		  //pRoute->ulRouteTotal = rib_stats.ulRepRouteIpv4Count;
		  pIpv6Route->type = rib->type;
		  pIpv6Route->select = CHECK_FLAG (rib->flags, ZEBRA_FLAG_SELECTED) ? 1 : 0;
	
		  iNexthopNum = 0;
		  memset(pIpv6Route->ucNexthop, 0, sizeof(pIpv6Route->ucNexthop));

		  for (nexthop = rib->nexthop; nexthop; nexthop = nexthop->next)
		  {  
			if ( CHECK_FLAG (nexthop->flags, NEXTHOP_FLAG_ACTIVE))	
				pIpv6Route->active = 1;
			if ( CHECK_FLAG (nexthop->flags, NEXTHOP_FLAG_FIB)) 
				pIpv6Route->fib = 1;
				
			  switch (nexthop->type)
			  {
				case NEXTHOP_TYPE_IPV4:
				case NEXTHOP_TYPE_IPV4_IFINDEX:
					break;
				case NEXTHOP_TYPE_IPV6:
				case NEXTHOP_TYPE_IPV6_IFINDEX:
					memcpy(pIpv6Route->ucNexthop[iNexthopNum++], nexthop->gate.ipv6.s6_addr, 16);
					if (nexthop->ifindex)
						sprintf(pIpv6Route->devStr, "%s", ifindex2ifname_vrf (nexthop->ifindex, rib->vrf_id));
					break;
				case NEXTHOP_TYPE_IFINDEX:
					sprintf(pIpv6Route->devStr, "%s", ifindex2ifname_vrf (nexthop->ifindex, rib->vrf_id));
					break;
				case NEXTHOP_TYPE_BLACKHOLE:
					sprintf(pIpv6Route->devStr, "Null0");
					break;
				default:
					break;
			  }
		  }
		  pIpv6Route->ulNexthopNum = iNexthopNum;
	  }

    route_unlock_node(rn);

    return iRet;
}

