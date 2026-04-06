/* OSPF VTY interface.
 * Copyright (C) 2000 Toshiaki Takada
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef _QUAGGA_OSPF_VTY_H
#define _QUAGGA_OSPF_VTY_H
#include <libipc/quaggaTask.h>

/* Macros. */
#define VTY_GET_OSPF_AREA_ID(V,F,STR)                                         \
{                                                                             \
  int retv;                                                                   \
  retv = ospf_str2area_id ((STR), &(V), &(F));                                \
  if (retv < 0)                                                               \
    {                                                                         \
      vty_out (vty, "%% Invalid OSPF area ID%s", VTY_NEWLINE);                \
      return CMD_WARNING;                                                     \
    }                                                                         \
}

#define VTY_GET_OSPF_AREA_ID_NO_BB(NAME,V,F,STR)                              \
{                                                                             \
  int retv;                                                                   \
  retv = ospf_str2area_id ((STR), &(V), &(F));                                \
  if (retv < 0)                                                               \
    {                                                                         \
      vty_out (vty, "%% Invalid OSPF area ID%s", VTY_NEWLINE);                \
      return CMD_WARNING;                                                     \
    }                                                                         \
  if (OSPF_IS_AREA_ID_BACKBONE ((V)))                                         \
    {                                                                         \
      vty_out (vty, "%% You can't configure %s to backbone%s",                \
               NAME, VTY_NEWLINE);                                            \
    }                                                                         \
}

struct ospf_vl_config_data {
	struct vty *vty;	  /* vty stuff */
	struct in_addr area_id;   /* area ID from command line */
	int format; 		  /* command line area ID format */
	struct in_addr vl_peer;   /* command line vl_peer */
	int auth_type;		  /* Authehntication type, if given */
	char *auth_key; 	  /* simple password if present */
	int crypto_key_id;		  /* Cryptographic key ID */
	char *md5_key;		  /* MD5 authentication key */
	int hello_interval; 		  /* Obvious what these are... */
	int retransmit_interval;
	int transmit_delay;
	int dead_interval;
};

/* Prototypes. */

extern void ospf_vty_init (void);
extern void ospf_vty_show_init (void);
extern void ospf_vty_clear_init (void);

extern int ospf_str2area_id (const char *, struct in_addr *, int *);
extern int ospf_open(u_int16_t ProcessID);
extern int ospf_close(u_int16_t ProcessID);
extern int ospfAreaSet(BOOL bNoFlag, struct in_addr area_id, u_int16_t ProcessID);
extern int ospfRouterIdSet(BOOL bNoFlag, struct in_addr router_id, u_int16_t ProcessID);
extern int ospfPassiveIntfDefaultSet(BOOL bNoFlag);
extern int ospfPassiveIntfSet(BOOL bNoFlag, char *ifname, struct in_addr addr);
extern int ospf_network_area_cfg(int bNoFlag, u_int32_t ulIp, u_int32_t ulMask, struct in_addr area_id, u_int16_t ProcessID);
extern int ospfInfoGet(OSPF_INFO_T *pOspfInfo);
extern int ospfRunInstanceGet(DATA_OP_ENT op, u_int16_t *instance);
extern int ospfAreaNetworkByGetAreaId(DATA_OP_ENT op, OSPF_AREA_NETWORK_T* pNetworkInfo);
extern int ospfRouteInfoGet(DATA_OP_ENT op, OSPF_ROUTE_INFO_T *pRouteInfo);
extern int ospfRtrRouteInfoGet(DATA_OP_ENT op, OSPF_RTR_ROUTE_INFO_T *pRtrRouteInfo);
extern int ospfPassiveIntfGet(DATA_OP_ENT op, OSPF_PASSIVE_INTF_INFO_T *pIntfInfo);
int ospfNbrInfoGet(DATA_OP_ENT op, OSPF_NBR_INFO_T *pNbrInfo);
int ospfNbmaNbrInfoGet(DATA_OP_ENT op, OSPF_NBR_INFO_T *pNbmaNbrInfo);
int ospfLsaInfoGet(DATA_OP_ENT op, OSPF_LSA_INFO_T *pLsaInfo);
int ospfStatusGet(int *pBEnable);
int ospfRouteNumGet(ulong_t *pulCnt);
int ospfAreaRangeByGetAreaId(DATA_OP_ENT op, OSPF_AREA_RANGE_T* pRangeInfo);
int ospfAreaInfoGet(DATA_OP_ENT op, OSPF_AREA_INFO_T *pAreaInfo);
int ospfRedistInfoGet(OSPF_REDIST_INFO_T *pRedistInfo);
int ospfVirIntfInfoGet(DATA_OP_ENT op, OSPF_INTF_INFO_T *pVirIntfInfo);
int ospfIntfInfoGet(DATA_OP_ENT op, OSPF_INTF_INFO_T *pIntfInfo);
int ospfDistanceIntfGet(DATA_OP_ENT op, OSPF_DISTANCE_INFO_T *pDistInfo);
int ospfMtuIgnoreSet(BOOL bNoFlag, char *ifname, struct in_addr addr);
int ospfDistancePrefixSet(BOOL bNoFlag, char *dis_str, char *prefix_str, char *alistName, u_int16_t ProcessID);
int ospfDistanceOspfSet(BOOL bNoFlag, ulong_t intra_dis, ulong_t inter_dis, ulong_t external_dis, u_int16_t ProcessID);
int ospfDistanceSet(BOOL bNoFlag, ulong_t distance, u_int16_t ProcessID);
int ospfDefaultMetricSet(BOOL bNoFlag, ulong_t metric, u_int16_t ProcessID);
int ospfDftInfoSet(BOOL bNoFlag, BOOL always, char *metric_str, char *type_str, char *rmapName, u_int16_t ProcessID);
int ospfDistrListOutSet(BOOL bNoFlag, char *alistName, char *protoName, u_int16_t ProcessID);
int ospfRedistributeSet(BOOL bNoFlag, char *protoName, char *metric_str,
							  char *type_str, char *rmapName, u_int16_t ProcessID);
int ospfIpNetworkSet(BOOL bNoFlag, char *ifname, int type);
int ospfIpDeadIntervalSet(BOOL bNoFlag, char *ifname, char *interval_str,
								char *fast_hello_str, struct in_addr addr);
int ospfIpParamSet(BOOL bNoFlag, char *ifname, ulong_t type, ulong_t value, struct in_addr addr);
int ospfIpAuthMd5Set(BOOL bNoFlag, char *ifname, ulong_t keyId, char *keyStr, struct in_addr addr);
int ospfIpAuthSimpleSet(BOOL bNoFlag, char *ifname, char *passwdStr, struct in_addr addr);
int ospfIpAuthModeSet(BOOL bNoFlag, char *ifname, int auth_type, struct in_addr addr);
int ospfAutoCostSet(BOOL bNoFlag, ulong_t refbw, u_int16_t ProcessID);
int ospfRefreshTimerSet(BOOL bNoFlag, ulong_t time, u_int16_t ProcessID);
int ospfSpfTimerSet(BOOL bNoFlag, ulong_t delay, ulong_t hold, ulong_t max, u_int16_t ProcessID);
int ospfStaticNeighborSet(BOOL bNoFlag, struct in_addr nbr_addr, ulong_t priority, ulong_t interval, u_int16_t ProcessID);
int ospfRfc1583compatibilitySet(BOOL bNoFlag, u_int16_t ProcessID);
int ospfLogAdjacencySet(BOOL bNoFlag, int detail, u_int16_t ProcessID);
int ospfAbrTypeSet(BOOL bNoFlag, char *abr_type_str, u_int16_t ProcessID);
int ospfAreaAuthTypeSet(BOOL bNoFlag, struct in_addr areaId, int auth_type, u_int16_t ProcessID);
int ospfAreaFilterListSet(BOOL bNoFlag, struct in_addr areaId, char *plistName, char *type_str, u_int16_t ProcessID);
int ospfAreaImportListSet(BOOL bNoFlag, struct in_addr areaId, char *alistName, u_int16_t ProcessID);
int ospfAreaExportListSet(BOOL bNoFlag, struct in_addr areaId, char *alistName, u_int16_t ProcessID);
int ospfAreaDefaultCostSet(BOOL bNoFlag, struct in_addr areaId, ulong_t cost, u_int16_t ProcessID);
int ospfNssaAreaSet(BOOL bNoFlag, struct in_addr areaID, char *translator_role,
						  int AreaSummaryStatus, u_int16_t ProcessID);
int ospfStubAreaSet(BOOL bNoFlag, struct in_addr areaID, int AreaSummaryStatus, u_int16_t ProcessID);
int ospfAreaShortCutSet(BOOL bNoFlag, struct in_addr areaId, char *type_st, u_int16_t ProcessIDr);
int ospfVirLinkSet(BOOL bNoFlag, struct ospf_vllink_config_data *vl_config, u_int16_t ProcessID);
int ospfAreaRangeSubstituteSet(BOOL bNoFlag, struct in_addr areaId, char *prefix_src,
										char *prefix_dst, u_int16_t ProcessID);
int ospfAreaRangeSet(BOOL bNoFlag, struct in_addr areaId, char *prefix_str, int advertise,
							ulong_t cost, u_int16_t ProcessID);
struct route_node *ospf_route_table_get_next (struct route_table *table, struct prefix *p, struct in_addr *area_id);

#endif /* _QUAGGA_OSPF_VTY_H */
