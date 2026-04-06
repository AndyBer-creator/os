
#ifndef __SAL_L3_INTF_H__
#define __SAL_L3_INTF_H__

#include <rsd/rsd_l3.h>

extern int32 sal_l3_intf_primary_IPv4_find(sys_l3_intfId_t *pId, sys_l3_ipv4_t *priIPv4);

extern int32 sal_l3_adminIntf_get(sys_l3_intfId_t *pId, sys_l3_intfAdmin_t *pAdmin);

extern int32 sal_l3_adminIntfEnbl_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 sal_l3_adminIntfEnbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

extern int32 sal_l3_adminIntfExist_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 sal_l3_adminIntfExist_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

extern int32 sal_l3_adminIntfDhcpV4_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 sal_l3_adminIntfDhcpV4_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

extern int32 sal_l3_intfDhcpcV4_renew(sys_l3_intfId_t *pId);

extern int32 sal_l3_adminIntfIPv4_get(sys_l3_intfId_t *pId, sys_l3_ipv4Admin_t *pIpAdmin);

extern int32 sal_l3_intfStaticIPv4_add(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp);
extern int32 sal_l3_intfStaticIPv4_sub_add(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp);

extern int32 sal_l3_intfStaticIPv4_del(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp);
extern int32 sal_l3_intfStaticIPv4_sub_del(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp);

extern int32 sal_l3_intfStaticIPv4_clear(sys_l3_intfId_t *pId);

extern int32 sal_l3_intfIPv4_add(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv4_t *pIp);
extern int32 sal_l3_intfIPv4_sub_add(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv4_t *pIp);

extern int32 sal_l3_intfIPv4_del(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv4_t *pIp);

extern int32 sal_l3_intfIPv4_clear(sys_l3_intfId_t *pId, sys_l3_ipType_t ipType);

extern int32 sal_l3_intfIPv4_get(sys_l3_intfId_t *pId, sys_l3_ipv4_t *pIp);

extern int32 sal_l3_intfIPv4_ip_equal(sys_ipv4_t ip);

extern int32 sal_l3_intfIPv6_add(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv6_t *pIp);

extern int32 sal_l3_intfIPv6_del(sys_l3_intfId_t *pId, sys_l3_ipType_t type, sys_l3_ipv6_t *pIp);

extern int32 sal_l3_intfIPv6_clear(sys_l3_intfId_t *pId, sys_l3_ipType_t ipType);

extern int32 sal_l3_intfIPv6_refresh(sys_l3_intfId_t *pId);

extern int32 sal_l3_adminIntfIPv6_get(sys_l3_intfId_t *pId, sys_l3_ipv6Admin_t *pIpAdmin);;

extern int32 sal_l3_adminIntfIPv6Enbl_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 sal_l3_adminIntfIPv6Enbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

extern int32 sal_l3_adminIntfIPv6Mode_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 sal_l3_adminIntfIPv6Mode_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

extern int32 sal_l3_intfStaticIPv6_add(sys_l3_intfId_t *pId, sys_l3_ipv6_t *pIp);

extern int32 sal_l3_intfStaticIPv6_del(sys_l3_intfId_t *pId, sys_l3_ipv6_t *pIp);

extern int32 sal_l3_intfStaticIPv6_clear(sys_l3_intfId_t *pId);

extern int32 sal_l3_intfStaticIPv6AddrNum_ret(sys_l3_intfId_t *pId);

extern int32 sal_l3_intfIPv6AutoconfEnbl_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 sal_l3_intfIPv6AutoconfEnbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

extern int32 sal_l3_intfIPv6DUnreachableEnable_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 sal_l3_intfIPv6DUnreachableEnable_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

extern int32 sal_l3_intfIPv6DadAttempt_set(sys_l3_intfId_t *pId, uint32 num);

extern int32 sal_l3_intfIPv6DadAttempt_get(sys_l3_intfId_t *pId, uint32 *pNum);

extern  int32 sal_l3_intfIPv6DadStatus_get(sys_l3_intfId_t *pId, sys_l3_ipv6_t *pIp, sys_l3_ipv6DadSts_t *pStatus);

extern int32 sal_l3_intfIPv6Maddr_get(sys_l3_intfId_t *pId, sys_l3_ipv6MaddrInfo_t *pMaddrs);

extern int32 sal_l3_intfIPv6DhcpEnbl_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 sal_l3_intfIPv6DhcpEnbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS

extern int32 sal_l3_intfIPv6DhcpStatelessEnbl_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 sal_l3_intfIPv6DhcpStatelessEnbl_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

extern int32 sal_l3_intfIPv6DhcpRefreshTime_set(sys_l3_intfId_t *pId, uint32 time);

extern int32 sal_l3_intfIPv6DhcpRefreshTime_get(sys_l3_intfId_t *pId, uint32 *pTime);

extern int32 sal_l3_intfIPv6DhcpMinRefreshTime_set(sys_l3_intfId_t *pId, uint32 time);

extern int32 sal_l3_intfIPv6DhcpMinRefreshTime_get(sys_l3_intfId_t *pId, uint32 *pTime);

extern int32 sal_l3_intfIPv6DhcpStateless_refresh(sys_l3_intfId_t *pId);
#endif

extern int32 sal_l3_intfExist_set(sys_l3_intfId_t *pId, uint32 exist);

extern int32 sal_l3_intfLinkStatusChg_set(sys_l3_intfId_t *pId, sys_l3_intfStsChg_t *pStsChg);

extern int32 sal_l3_intfLinkStatusChg_get(sys_l3_intfId_t *pId, sys_l3_intfStsChg_t *pStsChg);

extern int32 sal_l3_intfLinkStatus_set(sys_l3_intfId_t *pId, sys_l3_intfLinkStatus_t linkStatus);

extern int32 sal_l3_intfvExist_set(sys_l3_intfId_t *pId, uint32 exist);

extern int32 sal_l3_intf_del(sys_l3_intfId_t *pId);

extern int32 sal_l3_intf_refresh(sys_l3_intfId_t *pId);

extern int32 sal_l3_adminIntfMgmt_set(sys_l3_intfMgmtAdmin_t *pMgmt);

extern int32 sal_l3_adminIntfMgmt_get(sys_l3_intfMgmtAdmin_t *pMgmt);

extern int32 sal_l3_intfMgmt_get(sys_l3_intfMgmt_t *pMgmt);

extern int32 sal_l3_intfMgmt_refresh(void);
extern int32 sal_def_intfMgmt_get(char *ipaddr, char *netmask, char* gateway);

extern int32 sal_l3_define_interface_set(sys_l3_intfdef_t *pDefConfig);
extern int32 sal_l3_Mgmt_IPv4_ip_equal(sys_ipv4_t ip);
extern int32 _sal_l3_intfMgmtSubV4_active(void);
extern int32 sal_l3_intf_network_equal(int vidIndex, sys_ipv4_t pIp);
extern int32 sal_l3_intf_network_chk(int vidIndex, sys_ipv4_t pIp);
extern int32 sal_IPv4Valid_chk(sys_ipv4_t ipv4);
extern int32 sal_l3_intf_ipv4_by_vid_get(int vidIndex, sys_ipv4_t* pIp);
#ifdef CONFIG_SYS_L3_VRRP
extern int32 sal_l3_vrrp_intf_IPv4_add(int vlanid, int vrid, sys_ipv4_t vip); 
extern int32 sal_l3_vrrp_intf_IPv4_del(int vlanid, int vrid, sys_ipv4_t vip);
extern int32 sal_l3_vrrp_send_gratuitous_arp(uint32 vlanid, uint32 vrid, uint32 vmac);
#endif
#endif 

