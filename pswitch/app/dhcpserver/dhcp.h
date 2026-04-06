#ifndef _DHCP_H
#define	_DHCP_H

#ifdef __cplusplus
extern "C"{
#endif
#include <libipc/vos.h>


#define DHCP_SERVER_DBG(string, args...) SYS_DBG(DBG_DHCP_SERVER_COMMON, "[DHCP SERVER DBG] "string"", ##args)
#define DHCP_SERVER_RX_DBG(string, args...) SYS_DBG(DBG_DHCP_SERVER_RX, "[DHCP SERVER Rx DBG] "string"", ##args)
#define DHCP_SERVER_TX_DBG(string, args...) SYS_DBG(DBG_DHCP_SERVER_TX, "[DHCP SERVER Tx DBG] "string"", ##args)
#define DHCP_SERVER_CMD_DBG(string, args...) SYS_DBG(DBG_DHCP_SERVER_CMD, "[DHCP SERVER Cmd DBG] "string"", ##args)
#define DHCP_SERVER_PKT_DBG(string, args...) SYS_DBG(DBG_DHCP_SERVER_PKT, "[DHCP SERVER Pkt DBG] "string"", ##args)


typedef int32 STATUS;

#define DHCP_FULL_UCHAR ((UCHAR)-1)
#define DHCP_FULL_USHORT ((USHORT)-1)
#define DHCP_FULL_ULONG  ((ULONG)-1)

#define	DHCPSERVER_PORT		67    /*DHCP server port*/
#define	DHCPCLIENT_PORT		68    /*DHCP client port*/

#define	DHCP_REQUIRE	1    /*dhcp packet request*/
#define	DHCP_REPLY		2    /*dhcp packet response*/

#define ETHERNET_HARDWARETYPE    1    /*Ethernet hardware address type*/
#define ETHERNET_HARDWARELEN    6    /*Ethernet hardware address length*/

#define  MAGIC_COOKIE         0x63825363    /*option magic cookie option the first 4 bytes of field*/
#define  ENDOFF_OPTIONS        0xff    /*DHCP message end standart*/

#define DHCP_OPTION_COUNT     21

/*DHCP the code of the"options" field in the message*/
#define DHCP_SUBNETMASK_CODE  0x01 /*DHCP field contains "options" the field contains "Subnet mask"option indentifier*/
#define DHCP_ROUTERIP_CODE    0x03 /*DHCP field contains "options"the field contains"Routet "option indentifier*/
#define DHCP_DNSIP_CODE       0x06 /*DHCP field contains "options"the field contains"DNS server "option indentifier*/
#define DHCP_HOSTNAME_CODE    0x0c /*DHCP field contains "options"the field contains"Host name"option indentifier*/
#define DHCP_DOMAINNAME_CODE  0x0f /*DHCP field contains "options"the field contains"Domain name"option indentifier*/
#define DHCP_TFTPSERVER_NAME_CODE 0x42 /*DHCP field contains "options"the field contains"tftp server name"option indentifier,option 66*/
#define DHCP_BOOTFILE_NAME_CODE 0x43 /*DHCP field contains "options"the field contains"Bootfile name"option indentifier,option 67*/

#define DHCP_OPTION43_CODE 0x2b

#define DHCP_NBNS_CODE        0x2c /*DHCP field contains "options"the field contains"Netbios name server"option indentifier*/
#define DHCP_NBDS_CODE        0x2d /*DHCPfield contains "options"the field contains"Netbios data distribute server"option indentifier*/
#define DHCP_NBNT_CODE        0x2e /*DHCPfield contains "options"the field contains"Netbios node type"option indentifier*/
#define DHCP_NBSCOPE_CODE     0x2f /*DHCPfield contains "options"the field contains"Netbios scope"option indentifier*/

#define DHCP_REQUESTEDIP_CODE 0x32  /*DHCPfield contains "options"the field contains"Requested Ip"option indentifier*/
#define DHCP_IPLEASETIME_CODE 0x33  /*DHCPfield contains "options"the field contains"Dhcp Ip address lease time"option indentifier*/
#define DHCP_MESSAGETYPE_CODE 0x35  /*DHCPfield contains "options"the field contains"Dhcp Message Type"option indentifier*/
#define DHCP_SERVERIP_CODE    0x36  /*DHCPfield contains "options"the field contains"Dhcp Server Ip"option indentifier*/
#define DHCP_REQPARAM_CODE    0x37  /*DHCPfield contains "options"the field contains"Requested parameter list"option indentifier*/
#define DHCP_RENEWTIME_CODE   0x3a  /*DHCPfield contains "options"the field contains"Renew time"option indentifier*/
#define DHCP_REBINDTIME_CODE  0x3b  /*DHCPfield contains "options"the field contains"Rebind time"option indentifier*/
#define DHCP_OPTION60_CODE    60  /*DHCPfield contains "options"the field contains"Vendor class identifier"option indentifier,option 60*/
#define DHCP_CLIENTID_CODE    0x3d  /*DHCPfield contains "options"the field contains"Dhcp client Id"option indentifier*/
#define DHCP_VENDORINFO_CODE 0x29 /*"Vendor information"option*/


/*Flags corresponding to the "option" field in a DHCP message*/
#define DHCP_SUBNETMASK_CODE_FLAGBIT  1 
#define DHCP_ROUTERIP_CODE_FLAGBIT    (1<<1) 
#define DHCP_DNSIP_CODE_FLAGBIT       (1<<2)
#define DHCP_DOMAINNAME_CODE_FLAGBIT  (1<<3)

#define DHCP_NBNS_CODE_FLAGBIT        (1<<4) 
#define DHCP_NBDS_CODE_FLAGBIT        (1<<5) 
#define DHCP_NBNT_CODE_FLAGBIT        (1<<6) 
#define DHCP_NBSCOPE_CODE_FLAGBIT     (1<<7) 

#define DHCP_IPLEASETIME_CODE_FLAGBIT (1<<8)  
#define DHCP_RENEWTIME_CODE_FLAGBIT   (1<<9)  
#define DHCP_REBINDTIME_CODE_FLAGBIT  (1<<10)  

#define DHCP_OPTION43_CODE_FLAGBIT (1<<11)
#define DHCP_TFTPSERVER_NAME_CODE_FLAGBIT       (1<<12)
#define DHCP_BOOTFILE_NAME_CODE_FLAGBIT     (1<<13)

#define DHCP_IP_HEAD_LEN         20
#define DHCP_UDP_HEAD_LEN       8
#define DHCP_HEAD_LEN    236
#define DHCP_MIN_LEN         300
/*According to the original RFC2131 protocol, the maximum legth of the "option" field is 312byte,*/
/*which means the maximum legth of the message is 312+236byte*/
//#define DHCP_MAX_LEN  548  
#define DHCP_MAX_LEN  (1500-60-8)  

#define DHCP_MAX_HOPS    8 /*8;maximumDHCP hop couint 4 or 16*/

#define DHCP_CHADDR_LENGTH    16
#define DHCP_SNAME_LENGTH     64
#define DHCP_BOOTFILE_LENGTH  128

/*DHCP message structure*/
typedef	struct	tagDhcpHead
{
    uchar_t     ucop;    /*1=request;2=replay*/
    uchar_t     uchtype;    /*client hardware address:1=Ethernet */
    uchar_t     uchlen;    /*client hardware address legth:Ethernet=6 */
    uchar_t     uchops;    /*DHCP RELAY hop count passed*/
    ulong_t     ulxid;    /*packet random number*/
    ushort_t    ussecs;
    ushort_t    usflag;    /*broadcast flag bit*/
    ulong_t     ulciaddr;    /*client IP address*/
    ulong_t     ulyiaddr;    /*IP address assigned to the client by the server*/
    ulong_t     ulsiaddr;
    ulong_t     ulgiaddr;    /*IP address of the Relay Agent*/
    uchar_t     szchaddr[DHCP_CHADDR_LENGTH];    /*client hardware address*/
    uchar_t     szsname[DHCP_SNAME_LENGTH];
    uchar_t     szfile[DHCP_BOOTFILE_LENGTH];
}DHCP_HEAD_S;


/*Structure for storing pointer to DHCP options data*/
typedef struct tagDhcpOption
{
    ulong_t *pulSubnetMask;       /*subnet mask option:code = 1 */
    ulong_t *pulRouterIp;               /*default gateway option:code = 3 */
    uchar_t ucRouterIpLen;
    ulong_t *pulDNSIp;                  /*DNS server option:code = 6 */
    uchar_t ucDNSIpLen;
    uchar_t *pucHostName;            /*hostname option:code = 12	*/
    uchar_t ucHostNameLen;
    uchar_t *pucDomainName;       /*domain nameoption:code = 15 */
    uchar_t ucDomainNameLen;
    /*NetBios NameServer*/
    ulong_t *pulNBNSIp;                /*Netbios NameServer option:code = 44 */
    uchar_t ucNBNSIpLen;
    ulong_t *pulNBDSIp;                /*Netbios data distribution serve option:code = 45 */
    uchar_t ucNBDSIpLen; 
    uchar_t *pucNBNodeType;        /*Netbios node typeoption:code = 46 */
    uchar_t *pucNBScope;              /*Netbios scope option:code = 47 */
    uchar_t ucNBScopeLen;
    /*DHCP option */
    ulong_t *pulReqIp;                   /*Request IP address option:code = 50 */
    ulong_t *pulLeaseTime;            /*IP address lease time option:code = 51 */
    uchar_t *pucDHCPMsgType;      /*DHCP message type option:code = 53 */
    ulong_t *pulDHCPServerIp;       /*DHCP server identifier option:code = 54 */
    uchar_t *pucReqParamList;        /*Parameter request list option:code = 55 */
    uchar_t ucReqParamListLen;
    ulong_t *pulRenewTime;            /*Renewwal time value option:code = 58 */
    ulong_t *pulRebindTime;            /*Rebinding time value option:code = 59 */
    uchar_t *pucClientId;                /*Client identifier option:code = 61 */
    uchar_t ucClientIdLen;
    uchar_t *pucVendorOption;           /*Vendor specific information option:code = 43 */
    uchar_t ucVendorOptinLen;
    uchar_t *pucOption60;           /*vender class identifier option:code = 60 */
    uchar_t ucOption60Len;
    uchar_t *puctftpServer;           /*TFTP server name option:code = 66 */
    uchar_t uctftpServerLen;
    uchar_t *pucBootfile;           /*TFTP bootfile name option:code = 67 */
    uchar_t ucBootfileLen;
    /*record the position of the 'option' terminator*/
    uchar_t *pucEndOfOption;
}DHCP_OPTION_S;

#define DHCP_OPTION82_CODE    0x52 /*option 82*/

#define DHCP_OPTION82_LEN           20
#define DHCP_OPTION82_POIICY_VLAN_NUM    128

/*Option 82 relay agent information*/
#define OPTION82_VERBOSE_NODEID_MAC          "\"mac\""
#define OPTION82_VERBOSE_NODEID_HOSTNAME     "\"hostname\""

#define OPTION82_VERBOS_NODEID_LEN 60
#define OPTION82_CIRCUITID_LEN 80
#define OPTION82_REMOTEID_LEN 80

typedef struct dhcpOption82Policy_s
{
    BOOL  option82Enabled; /*option82 switch*/
    BOOL  option82DevEnabled; /*option82 circuit id device-relayted switch*/
    int  opion82Strategy;  /*option82 processing strategy*/
    int  option82Format;   /*option82 format*/
    int  opion82InfoFormat; /*option82 information format structure*/
    uchar_t option82VerboseNodeId[OPTION82_VERBOS_NODEID_LEN + 1]; /*option82 relay agent information*/
    uchar_t option82CircuitId[OPTION82_CIRCUITID_LEN + 1];    /*option82 circuit id*/
    uchar_t option82RemoteId[OPTION82_REMOTEID_LEN + 1];     /*option82 remote id*/   
}dhcpOption82Policy_t;

#define OPTION82_SUBOPTION9_LEN 80

typedef struct dhcpOption82Policy_port_s
{
    BOOL  option82Enabled; 
    int  opion82Strategy;  
     uchar_t option82CircuitId[OPTION82_CIRCUITID_LEN + 1];    
    uchar_t option82RemoteId[OPTION82_REMOTEID_LEN + 1];      
    uchar_t option82SubOpt9[OPTION82_SUBOPTION9_LEN + 1];       
}dhcpOption82Policy_port_t;

typedef struct dhcpOption82Policy_vlan_s
{
//    AVL_NODE node;
    ulong_t ulVlan;
    BOOL  option82Enabled; 
    int  opion82Strategy;  
    uchar_t option82CircuitId[OPTION82_CIRCUITID_LEN + 1];    
    uchar_t option82RemoteId[OPTION82_REMOTEID_LEN + 1];        
    uchar_t option82SubOpt9[80];      
}dhcpOption82Policy_vlan_t;

#define OPTION60_EQUALS 1
#define OPTION60_STARTS_WITH 2

#define OPTION60_FORMAT_ASCII 1
#define OPTION60_FORMAT_HEX   2
typedef struct dhcpOption60Match_s
{
    char matchType;
    char format;
    char matchContent[80];
    ulong_t contentlen;
    ulong_t gateway;
    ulong_t servergroup;
    char replyFormat;
    char replyContent[80];
    ulong_t replyLen;
}dhcpOption60Match_t;

#define DHCP_MAX_GROUP_NUM      256
#define DHCP_MAX_INTERFACE_NUM  32//(NUM_SW_DEVICES)

#ifdef __cplusplus
}  
#endif  /* end of __cplusplus */

#endif   /* _DHCP_H */

