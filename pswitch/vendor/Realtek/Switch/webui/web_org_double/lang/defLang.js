var defLang_ch = {
<!--#ifdef CONFIG_SYS_AAA-->
/* AAA */
lang_aaa: {
  titLoginLists: "Authentication method list",
  lblListName: "Name",
  lblMethodList: "Serial number",
  errDfltListDel: "The default authentication method cannot be deleted",
  lblMethodEmpty: "Empty",
  lblMethodNone: "None",
  lblMethodLocal: "Local",
  lblMethodEnable: "Enable",
  lblMethodRadius: "RADIUS",
  lblMethodTacplus: "TACACS+",

  titAddLoginList: "Add authentication method",
  titEditLoginList: "Modify authentication method",
  lblMethod: "Method $1"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_USER_DEFINED_ACL-->
/* ACL */
lang_acl: {
  titAclTable: "ACL Table Items",
  titAceTable: "ACE Table Items",
  titAclACEAdd: "Add ACE",
  titAclACEEdit: "Modify ACE",
  titAclBindingTable: "ACL Binding Table",
  titAclBindingAdd: "Add ACL Binding",
  titAclBindingEdit: "Modify ACL Binding",
  msgAclBindingPorts: "Note: After an ACL is bound to a port, the rules cannot be modified.",
  msgAclEmptyACL: "Note: An ACL cannot be bound if no rules are configured",

  lblAclName: "ACL Name",
  lblAclAceCnts: "Rule",
  lblAclBindingPorts: "Port",
  lblAclSequence: "Sequence Number",
  lblAclAction: "Action",
  lblAclPermit: "Permit",
  lblAclDeny: "Deny",
  lblAclShutdown: "Shutdown Port",
  lblAclSource: "Source",
  lblAclDestination: "Destination",
  lblAclMac: "MAC ACL",
  lblAclIp: "IP",
  lblAclIpv4: "IPv4 ACL",
  lblAclIpv6: "IPv6 ACL",
  lblAclPort: "Port",
  lblAclAddress: "Address",
  lblAclMask: "Mask",
  lblAclPrefix: "Prefix",
  lblAcl8021p: "802.1p",
  lblAclValue: "Value",
  lblAclEth: "Ethernet Type",
  lblAclProto: "Protocol",
  lblAclSingle: "Single",
  lblAclRange: "Range",
  lblAclTcpFlag: "TCP Flag",
  lblAclSet: "Set",
  lblAclUnset: "Cancel",
  lblAclNone: "No Action",
  lblAclToS: "Type of Service",
  lblAclDSCP: "DSCP",
  lblAclIPP: "IP Precedence",
  lblAclTcpFlag: "TCP Flag",
  lblAclICMP: "ICMP",
  lblAclType: "Type",
  lblAclCode: "Field",
  lblAclAny: "All",
  lblAclSelctList: "Select",
  lblAclIpputValue: "Custom",
  errPortRange: "The ending port cannot be less than or equal to the starting port."
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_MGMT_ACL-->
/* ACL - Manageement*/
lang_acl_mgmt: {
  titMgmtACLTable: "Management ACL Table Entries",
  titMgmtACETable: "Management ACE Table Entries",
  titMgmtACEAdd: "Add Management ACE",
  titMgmtACEEdit: "Modify Management ACE",
  lblMgmtACLName: "ACL Name",
  lblMgmtACENUM: "Rule",
  lblMgmtACLActive: "Status",
  lblMgmtACLPri: "Priority",
  lblMgmtACLAction: "Action",
  lblMgmtACLService: "Service",
  lblMgmtACLSrcIp: "Address/Mask",
  lblMgmtACLPermit: "Allow",
  lblMgmtACLDeny: "Deny",
  lblMgmtACLAll: "All",
  lblMgmtACLHTTP:"Http",
  lblMgmtACLHTTPS:"Https",
  lblMgmtACLSNMP:"Snmp",
  lblMgmtACLSSH:"SSH",
  lblMgmtACLTelnet:"Telnet"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_AUTHMGR-->
lang_authmgr: {
  lblAuthType: "Authentication Type",
  lblDot1x: "802.1x",
  lblMac: "MAC-Based",
  lblWeb: "WEB-Based",
  lblGuestVlan: "Guest VLAN",
  lblMacRadiusUser: "MAC-Based Username Format",

  titAuthmgrPortModeTable: "Port Mode Table",
  titAuthmgrEditPortMode: "Modify Port Mode",
  titAuthmgrPortSettingTable: "Port Setting Table",
  titAuthmgrEditPortSetting: "Modify Port Settings",

  lblPortCtrl: "Port Control",
  lblPortCtrlDisabled: "Disable",
  lblPortCtrlAuto: "Auto",
  lblPortCtrlForceAuth: "Force Authentication",
  lblPortCtrlForceUnauth: "Force Unauthenticated",
  lblHostMode: "Host Mode",
  lblHostModeMultiAuth: "Multiple Authentication Mode",
  lblHostModeMultiHost: "Multiple Host Mode",
  lblHostModeSingleHost: "Single Host Mode",
  lblOrder: "Authentication Order",
  lblAvailType: "Available Type",
  lblSelType: "Selected Type",
  lblMethod: "Authentication Method",
  lblAvailMethod: "Available Method",
  lblSelMethod: "Selected Method",
  lblMethodRadius: "RADIUS",
  lblMethodLocal: "Local",
  lblReauth: "Reauthentication",
  lblMaxHost: "Maximum Number of Hosts",
  lblVlanAssign: "VLAN assignment mode",
  lblVlanAssignDisable: "Disable",
  lblVlanAssignReject: "Reject",
  lblVlanAssignStatic: "Static",
  lblCommonTimer: "Common timer",
  lblTimerReauth: "Reauthentication period",
  lblTimerInact: "Keep-alive period",
  lblTimerQuiet: "Quiet period",
  lblDot1xParam: "802.1x parameters",
  lblMaxReq: "Maximum number of requests",
  lblTxPeriod: "Send period",
  lblSuppTimeout: "Client timeout",
  lblSrvTimeout: "Server timeout",
  lblWebParam: "Web-based parameters",
  lblLoginAttempt: "Maximum number of failed login attempts",
  lblInfinite: "Infinite",

  errTypeOrderEmpty: "At least one authentication type must be selected."
  errTypeOrderWebNotLast: "Web-Based must be the last authentication type."
  errMethodEmpty: "At least one authentication method must be selected."

  titMacLocalAcctTable: "MAC-Based Local Account List",
  titAddMacLocalAcct: "Add a MAC-Based Local Account",
  titEditMacLocalAcct: "Modify a MAC-Based Local Account",
  titWebLocalAcctTable: "WEB-Based Local Account List",
  titAddWebLocalAcct: "Add a WEB-Based Local Account",
  titEditWebLocalAcct: "Modify a WEB-Based Local Account",
  lblAssignTimer: "Specify a Timer",
  lblCtrl: "Control Mode",
  lblTimeoutSec: "Timeout (Seconds)",
  lblMacAddr: "MAC Address",
  lblAssignVlan: "VLAN",

  lblUserName: "Username",
  lblPwd: "Password",
  lblPwdConfirm: "Confirm Password",

  titSessTable: "Session List",
  lblCurrType: "Authentication Type",
  lblStatus: "Status",
  lblStsRunning: "Authorizing",
  lblStsAuthorized: "Authorized",
  lblStsUnAuthorized: "Unauthenticated",
  lblStsLocked: "Locked",
  lblStsGuest: "Guest",
  lblAuhotrInfo: "Authorization Information",
  lblOperInfo: "Operation Information",
  lblSessId: "Session ID",
  lblReauthPeriod: "Reauthentication Period",
  lblInactTimeout: "Timeout",
  lblSessTime: "Authorized Time",
  lblInactedTime: "Deactivation Time",
  lblQuietTime: "Quiet Time"
},
<!--#endif-->

/* Common Message */
lang_common: {
  txtEnable:"Enable",
  txtDisable:"Disable",
  txtEnabled:"Enable",
  txtDisabled:"Disable",
  txtHexRangeArg:"($1 - $2 hexadecimal characters)",
  txtDefaultArg:"(default: $1)",
  txtRangeArg:"($1 - $2)",
  txtRangeArgDefault:"($1 - $2, default $3)",
  txtSecRangeArg:"Seconds ($1 - $2)",
  txtMinRangeArg:"Minutes ($1 - $2)",
  txtByteRangeArg:"Bytes ($1 - $2)",
  txtMwRangeArg:"mW ($1 - $2)",
  txtKbpsRangeArg:"Kbps ($1 - $2)",
  txtUnitRangeArg:"$1 ($2 - $3)",
  txtMSRangeArgDefault:"Centiseconds ($1 - $2, default $3)",
  txtSecRangeArgDefault:"Seconds ($1 - $2, default $3)",
  txtMinRangeArgDefault:"Minutes ($1 - $2, default $3)",
  txtByteRangeArgDefault:"Bytes ($1 - $2, default $3)",
  txtUnitRangeArgDefault:"$1 ($2 - $3, default $4)",
  txtAll:"All",
  txtAuto:"Auto",
  txtArg:"$1",
  btnApply:"Apply",
  btnCancel:"Cancel",
  btnClose:"Close",
  btnAdd:"Add&nbsp;&nbsp;&nbsp;",
  btnEdit:"Modify&nbsp;&nbsp;&nbsp;",
  btnDelete:"Delete",
  tipOnlyOneCanbeEdited:"Only one entry can be processed at a time.",
  tipNoEntrySelected: "No entry selected."
  btnDetail: "Details",
  btnRefresh: "Refresh",
  btnPreview: "Preview",
  btnClear: "Clear",
  btnBack: "Back",
  btnOK: "Confirm",
  btnDone: "Done",
  btnActive: "Active",
  btnDeactive: "Inactive",
  btnBind: "Bind",
  btnUnbind: "Unbind",
  tipProceed: "Proceed",
  titEditIntfSettings: "Modify port settings",
  tipButtonTransAdd: "Select an item to transfer",
  tipButtonTransRem: "Select an item to transfer",
  lblEdit: "Modify",
  lblNA: "N/A",
  lblYes: "Yes",
  lblNo: "No",
  lblSave: "Save",
  lblAlert: "Alert",
  lblIntfType: "Interface type",
  lblEqualTo: "Equal",
  msgNotifySave: "Success",
  msgSuccess: "Success",
  lblOperationalStatus: "Operational Status",
  lblMbrPorts: "Member",
  lblPort: "Port",
  lblPortList: "Port List",
  lblAvailablePort: "Available Port",
  lblSelPort: "Selected Port",
  lblVLAN: "VLAN",
  lblVlanList: "VLAN List",
  lblAvailableVlan: "Available VLAN",
  lblSelVlan: "Selected VLAN",
  lblIntfSettingTable: "Interface Setting Table",
  lblNoResultFound: "0 results found",
  lblName: "Name",
  lblRange: "Range",
  lblPageLeftCombo: "Display",
  lblPageRightCombo: "Entry",
  lblIntf: "Interface",
  lblLAG: "LAG",
  lblRefreshRate: "Refresh rate",
  lblRefreshRateSec: "Seconds",
  lblRefreshRateNone: "None",
  lblRefreshRate5s: "5 seconds",
  lblRefreshRate10s: "10 seconds",
  lblRefreshRate30s: "30 seconds",
  lblUseDefault: "Use default value",
  lblUserDefined: "User defined",
  lblSrvAddrType: "Address type",
  lblByIPAddress: "By IP address",
  lblByName: "By name",
  lblHostname: "Host name",
  lblIPv4: "IPv4",
  lblIPv6: "IPv6",
  lblIPVersion: "IP version",
  lblSrvIPHost: "Server address",
  lblSrvPort: "Server port number",
  lblForward: "Forward",
  lblRateLimit: "Rate limit",
  lblUnLimited: "Unlimited",
  lblHintUnLimited: "0 for unlimited speed",
  lblActionForward: "Forward",
  lblActionTrap: "Trap",
  lblActionLog: "Log",
  lblActionDiscard: "Discard",
  lblActionShutdown: "Shutdown",
  lblState: "Status",
  btnAddSection: "Add address segment"
},

<!--#ifdef CONFIG_SYS_PROTO_DAI-->
/* DAI*/
lang_dai: {
  lblDAIStatus: "State",
  titDaiPortSettingTable: "Port Setting Table",
  titDaiEditPortSetting: "Modify Port Settings",
  titTableDaiStatistic: "Message Statistics Table",
  lblTrusted: "Trusted",
  lblVerifySrcMac: "Source MAC Address",
  lblVerifyDstMac: "Destination MAC Address",
  lblVerifyIP: "IP Address",
  lblIPAllowZero: "Allow All Zeros",
  lblSMacErr: "Source MAC Verification Failed",
  lblDMacErr: "Destination MAC Verification Failed",
  lblSIPErr: "Source IP Verification Failed",
  lblDIPErr: "Destination IP Verification Failed",
  lblIPMACErr: "IP-MAC Match Failed"
},
<!--#endif-->

lang_diag: {
  btnCopperTest: "Copper Test",

  lblPortSelect: "Port",
  lblCopperTestResult: "Copper Test Result",
  lblCopperTestStatus: "Cable Status",
  lblCopperResult: "Result",
  lblCopperLength: "Length",
  lblCopperNormal:"OK",
  lblCopperShort:"Short Cable",
  lblCopperOpen:"Open Cable",
  lblCopperMisMatch:"Impedance Mismatch",
  lblCopperLineDrive:"Line Drive",
  lblCopperNotTested:"Not Tested",
  lblMeter:"M",

  btnDetail: "Details",
  txtOpticalIntfStatus: "Optical Module Status",
  lblOpticalModuleStatusTable: "Optical Module Table",
  lblTransceiverType: "Transceiver Type",
  lblHotPlug: "Hot Plug",
  lblConnectorType: "Connector Type",
  lblEthComplianceCode: "Ethernet Compliance Code",
  lblTransmissionMedia: "Transmission Medium",
  lblWaveLength: "Wavelength",
  lblBitrate: "Bit Rate",
  lblVendorOui: "Vendor OUI",
  lblVendorName: "Vendor Name",
  lblVendorPN: "Vendor PN",
  lblVendorRev: "Vendor Revision",
  lblVendorSN: "Vendor SN",
  lblDateCode: "Manufacture Date",
  lblTemperature: "Temperature (C)",
  lblVoltage: "Voltage (V)",
  lblCurrent: "Current (mA)",
  lblOutputPower: "Output Power (mW)",
  lblInputPower: "Input Power (mW)",
  lblLossOfSignal: "Loss of Signal",
  lblOePresent: "OE Present",
  lblNA: "N/A",
  lblNS: "N/S",
  lblInsert: "Insert",
  lblRemove: "Remove",
  lblLoss: "Discard",
  lblNormal: "Normal",

  btnActivePing: "Ping",
  btnStop: "Stop",
  lblPingNum: "Number of times",
  lblPingResult: "Ping result",
  lblPingPktStatus: "Packet status",
  lblPingTx: "Number of packets sent",
  lblPingRx: "Number of packets received",
  lblPingLoss: "Packet loss rate",
  lblPingRTT: "Latency",
  lblPingMinRTT: "Minimum value",
  lblPingMaxRTT: "Maximum value",
  lblPingAvgRTT: "Average value",
  lblPingMs: "ms",
  lblPingStatus: "Status",
  lblPingRun: "Ping in progress",
  lblPingTimeout: "Ping failed (timeout)",
  lblPingUH: "Ping failed (unknown host)",
  lblPingAbort: "Ping aborted",
  lblTraceTTL: "Time to Live",
  lblTraceResult: "Traceroute Result",
  txtTraceFailed: "Traceroute failed",
  txtTraceInProgress: "Currently performing a traceroute operation. Please try again later.",
  txtPingInProgress: "Currently performing a ping operation. Please try again later."
},

<!--#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING-->
/* DHCP Snooping */
lang_ds: {
  lblDSStatus: "Status",
  titDsPortSettingTable: "Port Setting Table",
  titDs82PortSettingTable: "Port Setting Table",
  titDsEditPortSetting: "Modify Port Settings",
  titDs82EditPortSetting: "Modify Port Settings",
  titDsStatisticTable: "Port Statistics Table",
  titDs82CIDTable: "Option82 Circuit ID Table",
  titAddOption82CID: "Add Option82 Circuit ID",
  titEditOption82CID: "Modify Option82 Circuit ID",
  lblDsTrusted: "Trusted",
  lblDsVerifyMac: "Client Address Check",
  lblChaddrErr: "Client Address Check <br> Discard",
  lblUntrustedErr: "Untrusted Port <br> Discard",
  lblUntrustedOption82Err: "Untrusted Port <br> Option82 Check <br> Discard",
  lblInvalidErr: "Validity Check" <br> Discard",
  lblRemoteId: "Remote ID",
  lblCircuitIdHint: "(Leave blank for no VLAN)",
  lblOption82Status: "Status",
  lblOption82Action: "Allow untrusted",
  lblKeep: "Keep",
  lblDrop: "Discard",
  lblReplace: "Replace",
  lblCircuitID:"Circuit ID"
},
<!--#endif-->

/* DoS */
<!--#ifdef CONFIG_SYS_DOS_PROTECTION-->
lang_dos: {
  titDosPortSettingTable: "Port Setting Table",
  titEditDosPortSetting: "Modify Port Settings",
  lblByte: "Bytes",
  lblNetmaskLen: "Mask Length",
  lblDmacEqSmac: "Discard packets with identical source and destination MAC addresses",
  lblLand: "Prevent Land Attacks",
  lblUdpBlat: "Discard packets with identical source and destination UDP port numbers",
  lblTcpBlat: "Discard packets with identical source and destination TCP port numbers",
  lblPod: "Prevent POD Attacks",
  lblIpv6MinFragment: "IPv6 Minimum Fragment",
  lblIcmpFragment: "Discard ICMP Fragmented Packets",
  lblIpv4PingMaxSize: "Maximum IPv4 Ping Packet Size",
  lblIpv6PingMaxSize: "Maximum IPv6 Ping Packet Size",
  lblPingMaxSizeSetting: "Maximum Ping Packet Size",
  lblipv4PingMaxSizeEnable: "Enable IPv4",
  lblipv6PingMaxSizeEnable: "Enable IPv6",
  lblSmurfAttack: "Prevent Smurf attacks",
  lblTcpMinHdrSize: "TCP maximum HDR size",
  lblTcpSynSport: "Discard TCP-SYN packets",
  txtTcpSynSport: "Note: When the source port number is < 1024",
  lblNullScanAttack: "Prevent Null Scan attacks",
  lblXMasScanAttack: "Prevent X-Mas Scan attacks",
  lblTcpSynFinAttack: "Prevent TCP SYN-FIN attacks",
  lblTcpSynRstAttack: "Prevent TCP SYN-RST attacks",
  lblTcpFragmentOffset: "Discard TCP fragment packets",
  txtTcpFragmentOffset: "Note: When Offset = 1",
  lblDosPortState: "Status"
},
<!--#endif-->

/* DNS */
<!--#ifdef CONFIG_SYS_ADVANCE_DNS-->
lang_dns: {
  titDnsConf: "DNS Settings",
  txtDnsStatus: "DNS Status",
  txtDnsDftName: "DNS Default Name",
  txtDnsDftNameLimit: "(<i>1 to 255 alphanumeric characters</i>)",
  titDnsSrvConf: "DNS Server Settings",
  txtId: "ID",
  txtDnsServer: "DNS Server",
  txtPreference: "Preference Value",

  titDnsHostConf: "DNS Host Settings",
  txtHostname: "Hostname (1 to 255 characters)",
  txtIpv4AndIpv6: "IPv4/IPv6 Address",
  txtIpv4: "IPv4 Address",
  titDynamicHostMapping: "Dynamic Host Mapping",
  txtHost: "Host",
  txtTotal: "Total",
  txtElapsed: "Time Elapsed",
  txtType: "Type"
},

<!--#endif-->

/* Common Error Message */
lang_error: {
  errEmptyValue: "Empty value is invalid."
  errOutOfRange: "Out of valid value range."
  errOutOfRangeArg: "Out of valid value range. Valid range is $1-$2."
  errStrOutOfRange: "The length of the input string has reached the maximum allowed."
  errInvalidValue: "Invalid value."
  errInvalidChar: "Contains illegal characters."
  errInvalidMacAddr: "Invalid MAC address."
  errInvalidUnicastMacAddr: "Invalid unicast MAC address."
  errInvalidIpAddr: "Invalid IP address."
  errInvalidIpMask: "Invalid network mask."
  errInvalidUnicastIpAddr: "Invalid unicast IP address."
  errInvalidMulticastIpv4Addr: "Invalid IPv4 multicast address."
  errInvalidMulticastIpv4FirstGroup: "The first group value is out of range. The valid range is 224 - 239."
  errInvalidMulticastIpv6Addr: "Invalid IPv6 multicast address."
  errInvalidIpv6GroupNum: "Invalid IPv6 address group number."
  errInvalidIpv6Addr: "Invalid IPv6 format."
  errInvalidMulticastIpv6FirstGroup: "The value of group 1 should be \"ff00\" or higher."
  errInvalidDnsName: "Invalid DNS name. Valid order: tag#1.tag#2...tag#n. Each tag must be between $1 and $2 in length. Each tag must start and end with a letter or number."
  errInvalidVlanRangeArg: "The target VLAN is not in the currently displayed range. Valid range: [$1]-[$2]."
  errValMustBePairToPairHexChars: "The value must be $1..$2 pairs of hexadecimal characters."
  errValMustBePairHexChars: "The value must be $1 pairs of hexadecimal characters."
  errAddEntryFailBecauseTableFull: "Cannot create a new entry in table $1. The table is full."
  errEntryAlreadyExists: "The entry already exists."
  errPortBlng2Trunk: "Port $1 belongs to a trunk group."
  errDNSLookupFailed: 'DNS lookup failed.'
  errDnsResolveError: "DNS domain name resolution failed. Please check the DNS server settings or the host name."
  errDNSDomainErr: "Error! Invalid domain name. In the domain name, each substring must be less than 64 characters long, separated by dots, and the total string must not exceed 255 characters."
  errDNSServerMax: "Error! The maximum number of DNS server entries has been reached."
  errDNSIPaddr: "Error! Invalid IPv4 or IPv6 address."
  errDNSIPaddrUcast: "Error! Invalid IPv4 or IPv6 address, must be a unicast address."
},

/* File Management */
lang_file:{
  lblImage: "Image",
  lblSelImage: "Firmware",
  lblRunningCfg: "Running Configuration",
  lblStartupCfg: "Startup Configuration",
<!--#ifdef CONFIG_SYS_BACKUP_CONFIGURATION-->
  lblBackupCfg: "Backup Configuration",
<!--#endif-->
  lblFlashLog: "Flash Log",
  lblBufferLog: "RAM Log",
  lblFileType: "Configuration",
  lblAction: "Action",
  lblUpgrade: "Upgrade",
  lblBackup: "Backup",
  lblTransferMethod: "Method",
  lblTFTP: "TFTP",
  lblHTTP: "HTTP",
  lblBrowseFile: "File Name",
  lblFileName: "File Name",
  lblSrcFileName: "File Name",
  lblDstFileName: "File Name",
  txtBackupOK: "Backup $1 Completed",
  txtBackupFail: "Backup $1 Failed",
  txtDownloadInProgress: "Downloading...",
  txtUpgradeOK: "Upgrade $1 Completed, The new image will take effect after the system is restarted."
  txtUpgradeFail:"Upgrade $1 failed",
  txtUploadInProgress:"Upgrading...",
  txtUpgradeImgSelActive:"The new image will take effect after setting it as the active image and restarting the system."

  lblActImg:"Active image",
  lblActImgAfterReboot:"Prompt: Select this image for the next boot",
  lblImgActName:"Active image",
  lblImgBakName:"Backup image",
  lblImgArg:"Image $1",
  lblImgName:"Name",
  lblImgVersion:"Version",
  lblImgSize:"Size",
  lblImgTime:"Creation time",
  lblBytesArg:"$1 bytes",
  lblSrcFile:"Source file",
  lblDstFile:"Destination file"
},

<!--#ifdef CONFIG_SYS_PROTO_GVRP-->
  /* GVRP */
lang_gvrp:{
  lblGvrpGlobalStatus: "Status",
  lblTableGvrp: "GVRP Setting Table",
  lblGvrpState: "Status",
  lblGvrDynamicVlan: "VLAN Creation Function",
  lblGvrpRegist: "Registration Mode",
  titGVRPPortTbl: "Port Setting Table",
  lblGvrpState: "Status",
  lblGvrpDynamicVlan: "VLAN Creation Function",
  lblGvrpRegist: "Registration Mode",
  lblNormal: "Normal",
  lblFixed: "Fixed",
  lblForbidden: "Forbidden",
  titEditGVRPPortSetting: "Modify Port Settings",
  lblTimeParm: "Timer Settings",
  lblJoin: "Join",
  lblLeave: "Leave",
  lblLeaveAll: "LeaveAll",

  titGVRPVlanTable: "Group Member List",
  lblMemberPort: "Member",
  lblDynMemberPort: "Dynamic Member",

  lblcntType: "Message Statistics",
  lblAll: "All Packets",
  lblRxCnt: "Received Packets",
  lblTxCnt: "Sent Packets",
  lblErrCnt: "Error Packets",

  errGvrpGlobal: "Global GVRP is not enabled",
  errGvrpPortmode: "GVRP can only be enabled on trunk ports",
  errGvrpPortMirror: "GVRP cannot be enabled on the mirrored destination port",
  errGvrpPortType: "GVRP can only be enabled on trunk ports"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_GARP-->
  /* GMRP */
lang_gmrp:{
  lblGmrpGlobalStatus: "Status",
  lblTableGmrp: "GMRP Setting Table",
  lblGmrpState: "Status",
  lblGmrpRegist: "Registration Mode",
  titGMRPPortTbl: "Port Setting Table",
  lblGmrpState: "Status",
  lblGmrpRegist: "Registration Mode",
  lblNormal: "Normal",
  lblFixed: "Fixed",
  lblForbidden: "Forbidden",
  titEditGMRPPortSetting: "Modify Port Settings",
  lblTimeParm: "Timer Settings",
  lblJoin: "Join",
  lblLeave: "Leave",
  lblLeaveAll: "LeaveAll",

  lblcntType: "Message Statistics",
  lblAll: "All Packets",
  lblRxCnt: "Received Packets",
  lblTxCnt: "Sent Packets",
  lblErrCnt: "Error Packets",

  errGmrpGlobal: "GMRP is not enabled globally."
  errGmrpPortmode: "GMRP can only be enabled on trunk ports."
  errGmrpPortMirror: "GMRP cannot be enabled on the mirrored destination port."
  errGmrpPortType: "GMRP can only be enabled on trunk ports."
},
<!--#endif-->
/* Link Aggregation */
lang_lag:{
  lblBalance: "Load Balancing Policy",
  lblBalanceMac: "Based on MAC Address",
  lblBalanceIpMac: "Based on IP-MAC Address",
  lblBalanceDstMac: "Based on Dst-MAC Address",
  lblBalanceSrcMac: "Based on Src-MAC Address",
  lblBalanceDstIp: "Based on Dst-IP Address",
  lblBalanceSrcIp: "Based on Src-IP Address",
  titMgmtTbl: "Link Aggregation Group Table",
  lblType: "Type",
  lblLACP: "LACP",
  lblStatic: "Static",
  lblLinkState: "Link State",
  lblLagActiveMbrs: "Active Member",
  lblLagInactiveMbrs: "Passive Member",
  titEditLAGGroup: "Modify Link Aggregation Group",
  titLAGPortTbl: "Port Settings Table",
  titEditLAGPort: "Modify Port Settings",
  lblTypeEthArg: "eth$1$2",
  lblLACPSysPriority: "System Priority",
  titLACPPortTbl: "LACP Port Settings Table",
  titEditLACPPort: "Modify LACP Port Settings Table",
  lblPortPriority: "Port Priority",
  lblLACPTimeout: "Timeout",
  lblLong: "Long Timeout",
  lblShort: "Short Timeout"
},

/* Line */
lang_line: {
  lblConsole:"Console",
  lblTelnet:"Telnet",
  <!--#ifdef CONFIG_SYS_APP_SSH-->
  lblSsh:"SSH",
  <!--#endif-->
  lblHttp:"HTTP",
  lblHttps:"HTTPS",
  <!--#ifdef CONFIG_SYS_UI_SNMP-->
  lblSnmp:"SNMP",
  <!--#endif-->
  lblMgmtService: Management and Maintenance
  lblSessTimeout: Session Timeout
  lblPassRetryCnt: Password Retry Times
  lblSilentTime: Silent Time
},

<!--#ifdef CONFIG_SYS_PROTO_LLDP-->
/* LLDP */
lang_lldp: {
  lblLLDPProp: "LLDP",
  lblLLDPStatus: "Status",
  lblLLDPFramesHandling: "LLDP message handling method",
  lblBridging: "Forwarding",
  lblFiltering: "Filtering",
  lblFlooding: "Flooding",
  lblTLVAdvertiseInterval: "Packet transmission interval",
  lblHoldMultiplier: "Hold Multiplier",
  lblReinitialDelay: "Reinitialization delay",
  lblTransmitDelay: "Transmit delay",
  <!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
  lblLLDPMEDProp:"LLDP-MED",
  lblFastStartRepeatCount:"Quick Start Repeat Count",
  <!--#endif-->

  lblAdminStatus: "Mode",
  lblSelOptionTLVs: "Selected TLVs",
  lblTxOnly: "Transmit Only",
  lblRxOnly: "Receive Only",
  lblTxAndRx: "Transmit and Receive",
  titEditLLDPPortSettings: "Modify Port Settings",
  lblTLVs: "Optional TLVs",
  lblAvailableOptionTLVs: "Available TLVs",
  lblPortDescription:"Port Description",
  lblSysName:"System Name",
  lblSysDescription:"System Description",
  lblSysCapabilities:"System Capabilities",
  lblMACPHY:"802.3 MAC-PHY",
  lblLinkAggregation:"802.3 Link Aggregation",
  lblMaxFrameSize:"802.3 Maximum Frame Size",
  lblMgmtIpAddr:"Management IP Address",
  lblPVIDTLV:"802.1 PVID",
  lblVLANTLV:"802.1 VLAN Name",

  <!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
  lblLLDPMEDNetworkPolicyTable: "MED Network Policy Table",
  lblNetworkPolicyNum: "Policy ID",
  lblApp: "Application Type",
  lblVlanID: "VLAN",
  lblVlanTag: "VLAN Tag",
  lblUserPriority: "Priority",
  lblDSCPValue:"DSCP",
  lblTagged:"Tagged",
  lblUntagged:"Untagged",
  lblVoice:"Voice",
  lblVoiceSignaling:"Voice Signaling",
  lblGuestVoice:"Guest Voice",
  lblGuestVoiceSignaling:"Guest Voice Signaling",
  lblSoftphoneVoice:"Softphone Voice",
  lblVideoConferencing:"Video Conferencing",
  lblStreamingVideo:"Streaming Video",
  lblVideoSignaling:"Video Signaling",
  titAddLLDPMEDNetworkPolicy:"Add MED Network Policy",
  titEditLLDPMEDNetworkPolicy:"Edit MED Network Policy",

  lblLLDPMEDPortSettingTable: "MED port settings",
  lblLLDPMEDStatus: "Status",
  lblUserDefinedNetworkPolicy: "Network Policy",
  lblLocation: "Location",
  lblInventory: "Inventory",
  lblActive: "Active",
  lblYes: "Yes",
  lblNo: "No",
  titEditLLDPMEDPortSettings: "Modify MED port settings",
  lblNetworkPolicy: "Network Policy",
  lblAvailableNetworkPolicies: "Effective Policies",
  lblSelNetworkPolicies: "Selected Policies",
  lblCivic: "Location Information",
  lblCoordinates: "Coordinate Location",
  lblECSELIN: "Emergency Call",
  lbl16PairsHexChars: "($1 pair of hexadecimal characters)",
  lbl6to160PairsHexChars: "($1 - $2 pairs of hexadecimal characters)",
  lbl10to25PairsHexChars:"($1 - $2 pairs of hexadecimal characters)",

  <!--#endif-->

  titLldpPacketView: "Packet Preview Table",
  lblTotalBytesInUse: "Used (Bytes)",
  lblAvailableBytesLeft: "Available (Bytes)",
  lblStatus: "Operation Status",
  lblOverloading: "Overload",
  lblNotOverloading: "Not Overloaded",
  titLLDPOverloadingDetails: "Packet Details",
  lblLLDPMandatoryTLVs: "Mandatory TLVs",
  lblLLDPMEDCapabilities: "MED Capability Information",
  lblLLDPMEDLocation: "MED Location Information",
  lblLLDPMEDNetworkPolicy: "MED Network Policy",
  lblLLDPMEDExtendedPowerViaMDI: "MED Power Supply Capability",
  lblDot3Tlvs: "802.3 TLVs",
  lblLLDPOptionalTLVs: "Optional TLVs",
  lblLLDPMEDInventory: "MED Inventory",
  lblDot1Tlvs: "802.1 TLVs",
  lblTransmitted: "Sent",
  lblSizeBytes: "Size (bytes)",
  lblTotalBytes: "Total (bytes)",

  titLocalSummary: "Device summary information",
  btnLLDPPortStatusTable: "Port status table",
  lblLLDPState: "LLDP operating mode",
  lblLLDPMEDState: "LLDP-MED status",
  btnLLDPLocalInfoDetail: "Device information details",
  lblChassisIDSubtype: "Chassis ID subtype",
  lblChassisID: "Chassis ID",
  lblSupportedSysCapabilities: "Capabilities supported by the device",
  lblEnabledSysCapabilities: "Capabilities enabled on the device",
  lblPortIDSubtype: "Port ID subtype",
  lblChassisComponent: "Chassis component",
  lblIntfAlias: "Interface alias",
  lblPortComponent: "Port component",
  lblMacAddr: "MAC address",
  lblNetworkAddr: "Network address",
  lblIntfName: "Interface name",
  lblAgentCircuitID: "Agent circuit ID",
  lblLocal: "Local",
  lblOther: "Other",
  lblRepeater: "Repeater",
  lblBridge: "Bridge",
  lblL3: "Bridge, Router",
  lblWLANAccessPoint: "Wireless AP",
  lblRouter: "Router",
  lblTelephone: "Telephone",
  lblDOCSISCableDevice: "DOCSIS Cable Device",
  lblStationOnly: "Station only",
  lblPowerSourcingEntity: "Power Sourcing Entity",
  lblPoweredDevice: "Powered Device",
  lblUnknown: "Unknown",
  lblPrimaryPowerSource: "Primary Power Source",
  lblBackupPowerSource: "Backup Power Source",
  lblFromPSE: "From PSE",
  lblLocalAndPSE: "Local and PSE",
  lblCritical: "Critical",
  lblHigh: "High",
  lblLow: "Low",
  lblWatts:"$1.$2 Watts",
  lblFromPowerSourcingEntity:"From Power Sourcing Entity",
  lblLocalAndFromPowerSourcingEntity:"Local and From Power Sourcing Entity",
  tipNoNeighbors: "No Neighbors",

  lblPortID: "Port ID",
  lblPortDescription: "Port Description",
  lblMgmtAddrTable: "Management Address Table",
  lblAddrSubtype: "Address Subtype",
  lblAddr: "Address",
  lblIntfSubtype: "Interface Subtype",
  lblIntfNum: "Interface Number",
  lblIPv4: "IPv4",
  lblIPv6:"IPv6",
  lblNsap:"nsap",
  lblHdlc:"hdlc",
  lblBbn1822:"bbn1822",
  lblAll802:"all802",
  lblE163:"e163",
  lblE164:"e164",
  lblF69:"f69",
  lblX121:"x121",
  lblIpx:"ipx",
  lblAppleTalk:"Apple Talk",
  lblDecentIV:"DecentIV",
  lblBanyanVines:"Banyan Vines",
  lblE164WithNsap:"e164 with Nsap",
  lblDNS:"DNS",
  lblDistinguishedName:"Distinguished Name",
  lblAsNum:"as number",
  lblXtpOverIpv4:"xtp over Ipv4",
  lblXtpOverIpv6:"xtp over Ipv6",
  lblXtpNativeModeXtp:"xtp native mode XTP",
  lblIfIndex:"Interface Index",
  lblSysPortNum:"System Port Number",
  lblMACPHYDetail:"MAC/PHY Detail",
  lblAutoNegoSupported:"Auto-Negotiation Supported",
  lblAutoNegoEnabled:"Auto-Negotiation Enabled",
  lblTrue:"True",
  lblFalse:"False",
  lblAutoNegoAdvertised:"Auto-Negotiation Advertised Capabilities",
  lblOperMAUType:"Operational MAU Type",

  lbl10BaseT:"10baseT",
  lbl10BaseTFD:"10baseTFD",
  lbl100BaseT4:"100baseT4",
  lbl100BaseTX:"100baseTX",
  lbl100BaseTXFD:"100baseTXFD",
  lbl100BaseT2:"100baseT2",
  lbl100BaseT2FD:"100baseT2FD",
  lblFdxPause:"FdxPause",
  lblFdxAPause:"FdxAPause",
  lblFdxSPause:"FdxSPause",
  lblFdxBPause:"FdxBPause",
  lbl1000BaseX:"1000baseX",
  lbl1000BaseXFD:"1000baseXFD",
  lbl1000BaseT:"1000baseT",
  lbl1000BaseTFD:"1000baseTFD",

  lblMauAui:"dot3MauTypeAUI",
  lblMau10Base5:"dot3MauType10Base5",
  lblMau10Foirl:"dot3MauTypeFoirl",
  lblMau10Base2:"dot3MauType10Base2",
  lblMau10BaseT:"dot3MauType10BaseT",
  lblMau10BaseFP:"dot3MauType10BaseFP",
  lblMau10BaseFB:"dot3MauType10BaseFB",
  lblMau10BaseFL:"dot3MauType10BaseFL",
  lblMau10Broad36:"dot3MauType10Broad36",
  lblMau10BaseTHD:"dot3MauType10BaseTHD",
  lblMau10BaseTFD:"dot3MauType10BaseTFD",
  lblMau10BaseFLHD:"dot3MauType10BaseFLHD",
  lblMau10BaseFLFD:"dot3MauType10BaseFLFD",
  lblMau100BaseT4:"dot3MauType100BaseT4",
  lblMau100BaseTXHD:"dot3MauType100BaseTXHD",
  lblMau100BaseTXFD:"dot3MauType100BaseTXFD",
  lblMau100BaseFXHD:"dot3MauType100BaseFXHD",
  lblMau100BaseFXFD:"dot3MauType100BaseFXFD",
  lblMau100BaseT2HD:"dot3MauType100BaseT2HD",
  lblMau100BaseT2FD:"dot3MauType100BaseT2FD",
  lblMau1000BaseXHD:"dot3MauType1000BaseXHD",
  lblMau1000BaseXFD:"dot3MauType1000BaseXFD",
  lblMau1000BaseLXHD:"dot3MauType1000BaseLXHD",
  lblMau1000BaseLXFD:"dot3MauType1000BaseLXFD",
  lblMau1000BaseSXHD:"dot3MauType1000BaseSXHD",
  lblMau1000BaseSXFD:"dot3MauType1000BaseSXFD",
  lblMau1000BaseCXHD:"dot3MauType1000BaseCXHD",
  lblMau1000BaseCXFD:"dot3MauType1000BaseCXFD",
  lblMau1000BaseTHD:"dot3MauType1000BaseTHD",
  lblMau1000BaseTFD:"dot3MauType1000BaseTFD",
  lblMau10GigBaseX:"dot3MauType10GigBaseX",
  lblMau10GigBaseLX4:"dot3MauType10GigBaseLX4",
  lblMau10GigBaseR:"dot3MauType10GigBaseR",
  lblMau10GigBaseER:"dot3MauType10GigBaseER",
  lblMau10GigBaseLR:"dot3MauType10GigBaseLR",
  lblMau10GigBaseSR:"dot3MauType10GigBaseSR",
  lblMau10GigBaseW:"dot3MauType10GigBaseW",
  lblMau10GigBaseEW:"dot3MauType10GigBaseEW",
  lblMau10GigBaseLW:"dot3MauType10GigBaseLW",
  lblMau10GigBaseSW:"dot3MauType10GigBaseSW",

  lblDot3Details:"802.3 Detail",
  lblDot3MaxFrameSize:"802.3 Maximum Frame Size",
  lblDot3LinkAggregation:"802.3 Link Aggregation",
  lblAggregationCapability:"Aggregation Capability",
  lblAggregationStatus:"Aggregation Status",
  lblAggregationPortID:"Aggregation Port ID",
  lblCapable:"Capable",
  lblNotCapable:"Not capable",
  lblAggr:"Aggregated",
  lblNotAggr:"Not aggregated",

  <!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
  lblMEDDetails:"MED Detail",
  lblCapabilitiesSupported:"Capabilities Supported",
  lblCurrentCapabilities:"Current Capabilities",
  lblDeviceClass:"Device Class",
  lblDot3PowerType:"Power Type",
  lblDot3PowerSource:"Power Source",
  lblDot3PowerPriority:"Power Priority",
  lblRequestPowerValue:"PD Request Power Value",
  lblAllocatedPowerValue:"PSE Allocated Power Value",
  lblPoEDeviceType:"PoE Device Type",
  lblPoEPowerSource:"PoE Power Source",
  lblPoEPowerPriority:"PoE Power Priority",
  lblPoEPowerValue:"PoE Power Value",
  lblHardwareRevision:"Hardware Revision",
  lblFirmwareRevision:"Firmware Revision",
  lblSoftwareRevision:"Software Revision",
  lblSerialNumber:"Serial Number",
  lblManufacturerName:"Manufacturer Name",
  lblModelName:"Model Name",
  lblAssetID:"Asset ID",
  lblCapabilities:"Capabilities",
  lblNetworkPolicy:"Network policy",
  lblExtPSE:"Extended PSE",
  lblExtPD:"Extended PD",
  lblNotDef:"Not Defined",
  lblEndClass1:"Endpoint Class 1",
  lblEndClass2:"Endpoint Class 2",
  lblEndClass3:"Endpoint Class 3",
  lblNetCon:"Network Connectivity",
  lblLocationInfo:"Location Information",
  lblNetworkPolicyTable:"Network Policy Table",
  lblAppType:"Application Type",
  lblVlanType:"VLAN Type",
  lblDSCP:"DSCP",
  <!--#endif-->

  btnNeighborTable:"Neighbor Table",
  btnLLDPNeighborDetail:"Neighbor Information Detail",
  lblLocalPort:"Local Port",
  lblTTL:"Time to Live",
  lblLLDPNeighborInfo:"LLDP Neighbor Information",
  lblPortDetails:"Port Detail",
  lblMSAPEntry:"MSAP Entry",
  lblBasicDetails:"Basic Detail",
  lblDot3PowerViaMDI:"802.3 Power via MDI",
  lblMDIPowerSupportPortClass:"MDI Power Support Port Class",
  lblPSEMDIPowerSupport:"PSE MDI Power Support",
  lblPSEMDIPowerState:"PSE MDI Power State",
  lblPSEPowerPairControlAbility:"PSE Power Pair Control Ability",
  lblPSEPowerPair:"PSE Power Pair",
  lblPSEPowerClass:"PSE Power Class",
  lblSignal:"Signal",
  lblSpare:"Spare",
  lblClassPSE:"pClassPSE",
  lblClassPD:"pClassPD",
  lblSupported:"Supported",
  lblNotSupported:"Not Supported",
  lblEnabled:"Enabled",
  lblDisabled:"Disabled",
  lblClass:"Class $1",
  lblDot1VlanAndProtocol:"802.1 VLAN and Protocol",
  lblPVID:"PVID",
  lblPPVIDs:"PPVIDs",
  lblPPVIDTable:"PPVID Table",
  lblVID:"VID",
  lblSupported:"Supported",
  lblVlanIDs:"VLAN IDs",
  lblVlanIDTable:"VLAN ID Table",
  lblVlanName:"VLAN Name",
  lblProtocolIDs:"Protocol IDs",
  lblProtocolIDTable:"Protocol ID Table",
  lblProtocolID:"Protocol ID",

  titLLDPGlobalStatististics:"Global Statistics",
  lblInsertions:"Insertions",
  lblDeletions:"Deletions",
  lblDrops:"Drops",
  lblAgeOuts:"AgeOuts",
  lblLLDPStatisticsTable:"Statistics Table",
  lblTxFrames:"Transmit Frame",
  lblRxFrames:"Receive Frame",
  lblRxTLVs:"Receive TLV",
  lblNeighborInfoDeleteCnt:"Neighbor<br>Timeout",
  lblTotal:"Total",
  lblDiscarded:"Discard",
  lblErrors:"Error",
  lblUnrecognized:"Unrecognized"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_SYSLOG-->
lang_log:{
  lblLoggingTable: "Log Information Table",
  lblLoggingType: "View",
  txtRam: "RAM",
  txtFile: "Flash",
  lblLogIdx: "Log ID",
  lblLogTime: "Time",
  lblLogSev: "Information Level",
  lblLogDesc: "Description",
  lblState: "Status",
  lblAggregation: "Information Merge",
  lblAggAgingTime: "Aging Time",
  lblConsoleLogging: "Console Log",
  lblRamLogging: "RAM Log",
  lblFileLogging: "Flash Log",
  lblMinSev: "Minimum Severity",
  lblRemoteLogServerTable: "Remote Server List",
  lblLogServer: "Log Server",
  lblFacility: "Facility",
  lblLocalArg: "Local $1",
  titAddRemoteLogServer: "Add Remote Server",
  titEditRemoteLogServer: "Edit Remote Server",
  txtEmergency: "Emergency",
  txtAlert: "Assertion",
  txtCritical: "Critical",
  txtError: "Error",
  txtWarning: "Warning",
  txtNotice: "Notice",
  txtInformational: "Information",
  txtDebug: "Debug"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_ISG-->
/* IP Source Guard */
lang_ipsg: {
  titIPSGSetting: "Port Setting Table",
  titIPSGEditPortSetting: "Modify Port Settings",
  titIPSGBinding: "IP-MAC-Port-VLAN Binding Table",
  titBindingEntryAdd: "Add IP-MAC-Port-VLAN Binding",
  titBindingEntryEdit: "Modify IP-MAC-Port-VLAN Binding",
  lblStatus: "Status",
  lblVerifySrc: "Verification Mode",
  lblCurEntryNo: "Number of Used Entries",
  lblMaxEntryNo: "Maximum Number of Entries",
  lblVerifySrcIP: "IP",
  lblVerifySrcMACIP: "IP-MAC",
  lblIPSGBinding: "Binding",
  lblIPPort: "IP-Port-VLAN",
  lblIPMacPort: "IP-MAC-Port-VLAN",
  lblMACAddr: "MAC Address",
  lblIPAddr: "IP Address",
  lblType: "Type",
  lblLeaseTime: "Lease Time",
  lblTypeStatic: "Static",
  lblTypeDynamic: "Dynamic",
  lblBKDBType: "Mode",
  lblBKDBTypeNone: "None",
  lblBKDBTypeFlash: "Flash",
  lblBKDBTypeTFTP: "TFTP",
  lblBKDBFileName: "File Name",
  lblBKDBWriteDelay: "Write Delay",
  lblBKDBTimeout: "Timeout"
},
<!--#endif-->

lang_mac:{
  titMacAddressTable: "MAC Address Table",
  titDynamicAddressTable: "Dynamic Address Table",
  titStaticMacTable: "Static Address Table",
  titStaticMacAdd: "Add Static Address",
  titStaticMacEdit: "Modify Static Address",
<!--#ifdef CONFIG_SYS_BLACK_HOLE_MAC-->
  titAddressFilterTable: "Add Address Filter Table",
  titAddressFilterAdd: "Add Filter Address",
  titAddressFilterEdit: "Modify Filter Address",
<!--#endif-->
  lblAgingTime: "Aging Time",
  lblAddToStatic: "Add Static Address",
  lblMac: "MAC Address",
  lblType: "Type",  
  lblCpuPort: "CPU",
  lblMacTypeManage: "Manage",
<!--#ifdef CONFIG_SYS_BLACK_HOLE_MAC-->
  lblMacTypeBlacklist: "Discard",
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_AUTHMGR--> 
  lblMacTypeAuth:"Authentication",
<!--#endif-->
<!--#ifdef CONFIG_SYS_PORT_SECURITY--> 
  lblMacTypeSecureDynamic:"SecureDynamic", 
  lblMacTypeSecureSticky:"SecureSticky", 
  lblMacTypeSecureConfigured:"SecureConfigured",
<!--#endif--> 
  lblMacTypeStatic:"static", 
  lblMacTypeDynamic:"dynamic"
},

/* Menu tree */
lang_menu: {
  fld1Status: "System Status",
  doc1StatusInfo: "System Information",
  doc1StatusSyslog: "Log Information",
  fld2StatusPort: "Port Information",
  doc2StatusPortCnt: "Port Statistics",
  <!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
  doc2StatusPortErrDisable: "Port Error Protection",
  <!--#endif-->
  doc2StatusPortBwUtiliz: "Bandwidth Utilization",
  doc1StatusLAG: "Link Aggregation",
  doc1StatusMac: "MAC Address Table",

  fld1Network: "Network Configuration",
  doc1NetworkIPAddr: "IP Address Settings",
  <!--#ifdef CONFIG_SYS_ADVANCE_DNS-->
  doc1NetworkDNS: "DNS Configuration",
  doc1NetworkDNSHosts: "DNS Host Configuration",
  <!--#endif-->
  doc1NetworkTimeSys: "System Time",

  fld1Port: "Port",
  ringPort: "ERPS",
  doc1PortSetting: "Port Configuration",
  <!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
  doc1PortErrDisable: "Port Error Protection",
  <!--#endif-->
  fld2PortLAG: "Link Aggregation",
  doc2PortLAGMgmt: "Aggregation Group Configuration",
  doc2PortLAGPort: "Port Configuration",
  doc2PortLACP: "LACP Configuration",
  <!--#ifdef CONFIG_SYS_EEE-->
  doc1PortEEE: "EEE Configuration",
  <!--#endif-->
  doc1PortJumbo: "Jumbo Frame Configuration",
  fld1Vlan: "VLAN Function",
  fld2Vlan: "VLAN Configuration",
  doc1VlanCreate: "Creating a VLAN",
  doc1VlanVLANConf: "Setting a VLAN",
  doc1VlanMembership: "Membership Configuration",
  doc1VlanPort: "Port Configuration",
  <!--#ifdef CONFIG_SYS_VOICEVLAN-->
  fld2VlanVoice: "Voice VLAN",
  doc2VlanVoiceProp: "Function Configuration",
  doc2VlanVoiceOUI: "Voice OUI Configuration",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTOCOL_VLAN-->
  fld2VlanProtocol: "Protocol VLAN Configuration",  
  doc1VlanProtoGroup: "Protocol Group Configuration",
  doc1VlanProtoPort: "Protocol Group Binding",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_IPS_SUBNET-->
  fld2VlanIPsubnet: "IP subnet VLAN configuration",
  doc1VlanIPsubnetGroup: "IP subnet VLAN group configuration",
  doc1VlanIPsubnetPort: "IP subnet group binding",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_MACVLAN-->
  fld2VlanMac: "MAC VLAN configuration",
  doc1VlanMacGroup: "MAC group configuration",
  doc1VlanMacGroupMapping: "MAC group binding",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_SURVEILLANCEVLAN-->
  fld2VlanSurveillance: "Surveillance VLAN",
  doc2VlanSurveillanceProp: "Function Configuration",
  doc2VlanSurveillanceOUI: "Surveillance OUI Configuration",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_GVRP-->
  fld2VlanGVRP: "GVRP",
  doc2VlanGVRPProperty: "Function Configuration",
  doc2VlanGVRPMembership: "Membership List",
  doc2VlanGVRPStatistics: "Message Statistics",
  doc2VlanGVRPErrorStatistics: "Error Message Statistics",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_GARP-->
  fld2GMRP: "GMRP",
  doc2GMRPProperty: "Function Configuration",
  doc2GMRPMembership: "Membership List",
  doc2GMRPStatistics: "Message Statistics",
  doc2GMRPErrorStatistics: "Error Message Statistics",
  <!--#endif-->
  fld1Mac: "MAC Address Table",
  doc1MacDynamic: "Dynamic MAC Address Table",
  doc1MacStatic: "Static MAC Address Table",
  <!--#ifdef CONFIG_SYS_BLACK_HOLE_MAC-->
  doc1MacFilter: "MAC Address Filter Table",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PORT_SECURITY-->
  doc1MacPsecure: "Port Security MAC Address Table",
  <!--#endif-->

  fld1STP: "Spanning Tree Protocol",
  doc1StpGlobal: "Function Settings",
  doc1StpIntf: "Port Settings",
  <!--#ifdef CONFIG_SYS_PROTO_STP_MSTP-->
  doc1StpMstInstance: "Instance Settings",
  doc1StpMstIntf: "Instance Port Settings",
  <!--#endif-->
  doc1StpStatistics: "Message Statistics",

  <!--#ifdef CONFIG_SYS_PROTO_LLDP-->
  fld1Discov: "Topology Discovery",
  fld2DiscovLLDP: "LLDP",
  doc2DiscovLLDPProp: "Function Configuration",
  doc2DiscovLLDPPort: "Port Configuration",
  <!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
  doc2DiscovLLDPMedPolicy: "MED Network Policy Configuration",
  doc2DiscovLLDPMedPort: "MED port configuration",
  <!--#endif-->
  doc2DiscovLLDPOverloading: "Message preview",
  doc2DiscovLLDPLocalInfo: "Local device information",
  doc2DiscovLLDPNeighbor: "Neighbor information",
  doc2DiscovLLDPStatistics: "Message statistics",
  <!--#endif-->

  fld1Mcast: "Multicast",
  fld2McastGeneral: "Basic Function",
  doc2McastGeneralProperty: "Function Configuration",
  doc2McastGeneralGroupAddr: "Static Multicast Configuration",
  doc2McastGeneralRouterPort: "Router Port Configuration",
  <!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD-->
  doc2McastGeneralForwardAll: "Forwarding Port Configuration",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING-->
  doc2McastGeneralThrottling: "Port Restriction",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING-->
  doc2McastIGMPGeneralFilterProfile: "Filtering Rule Configuration",
  doc2McastIGMPGeneralFilterBinding: "Filtering Rule Binding",
  <!--#endif-->
  fld2McastIGMP: "IGMP Snooping",
  doc2McastIGMPProperty: "Function Configuration",
  doc2McastIGMPQuerier: "Querier Configuration",
  doc2McastIGMPStatistics: "Message Statistics",
  <!--#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING-->
  fld2McastMLD: "MLD Snooping",
  doc2McastMLDProperty: "Function Configuration",
  doc2McastMLDStatistics: "Message Statistics",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_MVR-->
  fld2McastMVR: "MVR",
  doc2McastMVRProperty: "Function Configuration",
  doc2McastMVRPortSetting: "Port Configuration",
  doc2McastMVRGroupAddress: "Group Address Configuration",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_L3_ROUTE-->
  fld1IPConfig: "Routing",
  fld2IPv4: "IPv4 Management Interface",
  doc2IPv4Intf: "IPv4 Interface",
  doc2IPv4Route: "IPv4 Routing",
  doc2IPv4Arp: "ARP",
  doc2IPv4ArpProxy: "ARP Proxy",
  fld2IPv6: "IPv6 Management Interface",
  doc2IPv6Intf: "IPv6 Interface",
  doc2IPv6Addr: "IPv6 Address",
  doc2IPv6Route: "IPv6 Routing",
  doc2IPv6Neigh: "IPv6 Neighbor",
  fld2Rip: "Rip Routing Management",
  doc2IPv4Rip: "Rip Routing Configuration",
  fld2Ospf: "Ospf Routing Management",
  doc2IPv4Ospf: "Ospf Routing Configuration",
  fld2Vrrp: "VRRP Management",
  doc2IPv4Vrrp: "VRRP Interface Configuration",
  <!--#endif-->

  fld1Sec: "Security",
  <!--#ifdef CONFIG_SYS_LIB_RADIUS-->
  doc1SecRadius:"RADIUS",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_LIB_TACPLUS-->
  doc1SecTacplus:"TACACS+",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_AAA-->
  fld2SecAAA: "AAA",
  doc2SecAAALoginList: "Authentication method configuration",
  doc2SecAAALoginAuthen: "Login authentication",
  <!--#endif-->
  fld2SecMgmtAccess: "Management channel configuration",
  doc2SecMgmtVLAN: "Management VLAN",
  doc2SecMgmtService: "Management service",
  <!--#ifdef CONFIG_SYS_MGMT_ACL-->
  doc2SecMgmtACL: "Management ACL",
  doc2SecMgmtACE: "Management ACE",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_AUTHMGR-->
  fld2SecAuthmgr: "Authentication function",
  doc2SecAuthmgrProp: "Function configuration",
  doc2SecAuthmgrPort: "Port configuration",
  doc2SecAuthmgrMacLocal: "MAC-based local account",
  doc2SecAuthmgrWebLocal: "WEB-Based Local Account",
  doc2SecAuthmgrSess: "Session Information",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PORT_SECURITY-->
  doc1SecPort:"Port Security",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_ERPS-->
  ringPortErps: "ERPS Instance",
  ringStatusErps: "Function Configuration",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PORT_ISOLATION-->
  doc1SecProtected: "Port Isolation",
  <!--#endif-->
  doc1SecStorm: "Storm Control",
  <!--#ifdef CONFIG_SYS_DOS_PROTECTION-->
  fld2SecDos: "DoS Attack Prevention",
  doc2SecDosProp: "Function Configuration",
  doc2SecDosPort: "Port Configuration",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_ISG-->
  fld2SecISG: "IP Source Guard",
  doc2SecISGPort: "Port Configuration",
  doc2SecISGBind: "IMPV Binding",
  doc2SecISGDatabase: "Database Storage",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_DAI-->
  fld2SecDAI: "Dynamic ARP Inspection",
  doc2SecDAI: "Function Configuration",
  doc2SecDAIStatistic: "Message Statistics",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING-->
  fld2SecDS: "DHCP Snooping",
  doc2SecDS: "Function Configuration",
  doc2SecDSStatistic: "Message Statistics",
  doc2SecDSOpt82Global: "Option 82 Function Configuration",
  doc2SecDSOpt82Circuit: "Option 82 Circuit ID Configuration",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_USER_DEFINED_ACL-->
  fld1Acl: "ACL",
  doc1AclMAC: "MAC ACL Configuration",
  doc1AclMACAce: "MAC ACE Configuration",
  doc1AclIPv4: "IPv4 ACL Configuration",
  doc1AclIPv4Ace: "IPv4 ACE Configuration",
  doc1AclIPv6: "IPv6 ACL Configuration",
  doc1AclIPv6Ace: "IPv6 ACE Configuration",
  doc1AclBind: "ACL Binding",
  <!--#endif-->

  fld1Qos: "QoS",
  fld2QosGeneral: "Basic Functions",
  doc2QosGeneralProp: "Function Configuration",
  doc2QosGeneralQueue: "Queue Scheduling",
  doc2QosGeneralCoS: "CoS Mapping",
  doc2QosGeneralDSCP: "DSCP Mapping",
  doc2QosGeneralIPP: "IP Precedence Mapping",
  fld2QosRate: "Bandwidth rate limit",
  doc2QosRateBwctl: "Port rate limit",
  doc2QosRateEgressQueue: "Egress queue rate limit",

  fld1Diag: "Device Diagnosis",
  fld2DiagLog: "Log Function",
  doc2DiagLogProp: "Function Configuration",
  doc2DiagLogRemote: "Remote Server Configuration",
  doc1DiagMirror: "Mirror Function",
  doc1DiagPing: "Ping",
  doc1DiagTraceRoute: "Traceroute",
  doc1DiagCopper: "Electrical Port Test",
  <!--#ifdef CONFIG_SYS_OPTICALMOD_STATUS-->
  doc1DiagFiber: "Optical Module Information",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_UDLD-->
  fld2SecUDLD: "UDLD Protocol",
  doc2SecUDLDProp: "Function Configuration",
  doc2SecUDLDNeighbors: "Neighbor Information",
  <!--#endif-->
  doc1DiagReboot: "Reboot",
  fld1Mgmt: "Device Management",
  doc1MgmtAccount: "User Configuration",
  fld2MgmtFirmware: "Firmware Management",
  doc2MgmtUpgradeBackup: "Upgrade",
  doc2MgmtActiveImg: "Active Image",
  fld2MgmtConfig: "Configuration Management",
  doc2MgmtConfigSave: "Save Configuration",
  <!--#ifdef CONFIG_SYS_UI_SNMP-->

  fld2MgmtSNMP: "SNMP Configuration",

  <!--#ifdef CONFIG_SYS_SNMPV3-->
  doc2MgmtSNMPView: "View Configuration",
  doc2MgmtSNMPGroup: "Group Configuration",
  doc2MgmtSNMPCommunity: "Community Configuration",
  doc2MgmtSNMPUser: "User Configuration",

  doc2MgmtSNMPEngine: "Engine ID Configuration",
  doc2MgmtSNMPTrapEvent: "Trap Configuration",
  doc2MgmtSNMPNotify: "Notification Configuration",
  <!--#else-->
  doc2MgmtSNMPCommunity: "Community Configuration",
  doc2MgmtSNMPTrapEvent: "Trap Configuration",
  doc2MgmtSNMPNotify: "Notification Configuration",
  <!--#endif-->
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI-->
  fld2MgmtRMON: "RMON Configuration",
  doc2MgmtRMONStatistic: "Message Statistics",
  doc2MgmtRMONEvent: "Event Configuration",
  doc2MgmtRMONAlarm: "Alarm Configuration",
  doc2MgmtRMONHistory: "History Configuration",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_POE-->
  fld1Poe: "POE Settings",
  doc1PoePort: "POE Port Settings",
  doc1PoePortTimer: "POE Port Timer Settings",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_DHCP_SERVER-->
  fld1Dhcp: "DHCP",
  doc1Dhcp: "Function Configuration",
  doc1IPPool: "Address Pool Configuration",
  doc1vlanifpool: "VLAN Interface Address Group Configuration",
  doc1DhcpClients: "Client List",
  doc1DhcpStatic: "Client Static Binding Table",
  <!--#endif-->
  dummy:""
},

lang_mcast:{
  lblUnknownMcastAct: "Unknown multicast forwarding",
  lblDrop: "Drop",
  lblFlood: "Flood",
  lblForwardToRouterPort: "Forward to the router port",
  lblFwdMethod: "Multicast forwarding method",
  lblDmac: "Destination MAC-VID",
  lblDip: "Destination IP-VID",

  <!--#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING-->
  txtIPv4:"IPv4",
  txtIPv6:"IPv6",
  lblMLDv1:"MLDv1",
  lblMLDv2:"MLDv2",
  <!--#endif-->

  lblMcastGrpTable: "Multicast Table",
  lblMcastGroupAddr: "Group Address",
  lblMemberPort: "Member",
  lblStatic: "Static",
  lblForbid: "Disable",
  lblNone: "None",
  lblDynamic: "Dynamic",
  lblForbidden: "Disable",
  lblType: "Type",
  lblLifeSec: "Aging Time (Seconds)",

  titEditGroup: "Modify Multicast Table",
  titAddGroup: "Add Multicast Table",

  lblMcastRouterTable: "Router Port Table",
  lblRtMbrPort: "Port",
  <!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER-->
  titAddRouter: "Add a router port",
  titEditRouter: "Modify a router port",
  lblStRtMbrPort: "Static router port",
  lblForbidRtMbrPort: "Disable a router port",
  lblRtMbrPortType: "Type",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD-->
  lblMcastFwdAllTable: "Forwarding Port Table",
  titAddFwdAll: "Add Forwarding Port",
  titEditFwdAll: "Modify Forwarding Port",
  lblStFwdMbrPort: "Static Forwarding Port",
  lblForbidFwdMbrPort: "Disable Forwarding Port",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING-->
  lblDeny: "Discard",
  lblReplace: "Replace",
  txtMcastThrotTable: "Port Restriction Table",
  txtEditMcastThrotTable: "Modify Port Restriction",
  lblMaxGroup: "Maximum Number of Groups",
  lblMaxAct: "Overload Action",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING-->
  titProfilesTable: "Filter Rule Table",
  lblProfileIndex: "Rule ID",
  lblStartMulticastAddr: "Start Address",
  lblEndMulticastAddr: "End Address",
  lblAction: "Action",
  lblAllow: "Allow",
  lblProfDeny: "Discard",
  titAddProfiles: "Add Rule",
  titEditProfile: "Modify Rule",

  titMulticastFilterTable: "Filter Binding Table",
  titEditFilterSettings: "Modify Filter Binding",
  lblFilterProfileIndex: "Rule ID",
  <!--#endif-->

  lblMcastState: "State",
  lblMcastVersion: "Version",
  lblIGMPv1:"IGMPv1",
  lblIGMPv2:"IGMPv2",
  lblIGMPv3:"IGMPv3",

  lblReportSuppression: "Report Suppression Function",
  tblMcastVlan: "Multicast VLAN Configuration Table",
  tblMRouterPortsAutoLearn: "Router Port Learning",
  tblForbiddenPorts: "Port Disablement",
  tblForbiddenMRouterPorts: "Router Port Disablement",
  tblQueryRobustness: "Number of Queries",
  tblQueryInterval: "Query Interval",
  tblQueryMaxResponseInterval: "Maximum Query Response Time",
  tblLastMemberQueryCounter: "Number of Query Counts for a Specific Group",
  tblLastMemberQueryInterval: "Query Interval for a Specific Group",
  tblImmediateLeave: "Quick Leave",
  ttQuerierTable: "Querier Table",
  ttQuerierTableEdit: "Modify Querier",
  tblIGMPQuerierState: "Status",
  tblIGMPQuerierStatus: "Running Status",
  tblIGMPQuerierVersion: "Version",
  tblQuerierIPAddress: "Querier Address",

  titEditMcastVlan: "Modify multicast VLAN configuration",
  lblOperStatus: "Status",
  lblMRouterPortsAutoLearn: "Router port learning",
  lblQueryRobustness: "Query times",
  lblOperationalQueryRobustness: "Query times",
  lblQueryInterval: "Query interval",
  lblOperationalQueryInterval: "Query interval",
  lblQueryMaxResponseInterval: "Maximum query response time",
  lblOperationalQueryMaxResponseInterval: "Maximum query response time",
  lblLastMemberQueryCounter: "Specified group query times",
  lblOperationalLastMemberQueryCounter: "Specified group query times",
  lblLastMemberQueryInterval: "Specified group query interval",
  lblOperationalLastMemberQueryInterval: "Specified group query interval",
  lblImmediateleave: "Quick leave",
  lblIGMPQuerierVersion: "Version",
  lblSecArg: "$1 (seconds)",

  titIGMPStat: "Message Statistics",
  lblRX: "Received Messages",
  lblTX: "Sent Messages",
  lblTotal: "Total",
  lblValid: "Valid Messages",
  lblInValid: "Invalid Messages",
  lblOther: "Other Messages",
  lblLeave: "Leave Message",
  lblReport: "Report Message",
  lblGenQry: "General Query Message",
  lblSpecGryQry: "Group-Specific Query Message",
  lblSpecGrySrcQry: "Source-Specific Query Message",

  errMcastPortSel: "No Port Selected",
  errMcastVlanSel: "No VLAN Selected",
  lblUnused: "Unused"
},

<!--#ifdef CONFIG_SYS_PROTO_ERPS-->
lang_ring:{
  ringErpsStatuss: "Erps Status",
  ringErpsInstances: "Erps Instance",
  erpsInstance: "Instance",
  erpsRingStatus: "Ring Status",
  erpsRingMel: "Ring Level",
  erpsRingControlVlan: "Control VLAN",
  erpsRingWtrTime: "WTR Time",
  erpsRingGuardTime: "Guard Time",
  erpsRingWorkingMode: "Working Mode",
  erpsRingId: "Ring ID",
  erpsRingRingLevel: "Ring Type",
  erpsRingRingProtectedInstance: "Protected Instance",
  erpsRingPort0: "port0",
  erpsRingPort0Role: "Port Type",
  erpsRingPort0Status: "Port Status",
  erpsRingPort1: "port1",
  erpsRingPort1Role: "Port Type",
  erpsRingPort1Status: "Port Status",
  erpsRingNodeStatus: "Node Status",
  titEditRingInstance: "Ring Instance Configuration",
  erpsRingModeRevertive: "Reversible Mode",
  erpsRingModeNoRevertive: "Irreversible Mode",
  erpsInstanceAddOrDel: "Instance Operation",
  erpsInstanceAdd: "Add",
  erpsInstanceDelte: "Delete",
  erpsRingPort1RoleValue: "Port 1 Role",
  erpsRingPort0RoleValue: "Port 0 Role",
  erpsInstanceRingPortRoleRpl1: "Normal Port",
  erpsInstanceRingPortRoleOwner1: "Master Port",
  erpsInstanceRingPortRoleNeighbour1: "Neighbor Port",
  erpsInstanceRingPortRoleNextNeighbour1: "Next Neighbor Port",
  erpsInstanceRingPortRoleRpl0: "Normal Port",
  erpsInstanceRingPortRoleOwner0: "Primary Port",
  erpsInstanceRingPortRoleNeighbour0: "Neighbor Port",
  erpsInstanceRingPortRoleNextNeighbour0: "Next Neighbor Port", 
  
  
  
  erpsGlobalConfig: "ERPS instance configuration",
  erpsInsName: "instance",

  lblUnused: "unused"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_MVR-->
lang_mvr:{
lblState:"Status",
txtComp:"Compatible",
txtDyn:"Dynamic",
lblMVRGroupStart:"Starting Group Address",
lblMVRGroupSeq:"Number of Groups",
lblMVRQryTime:"Query Delay",
lblMVRMode:"Mode",
lblOperationalGroupNum:"Entry Status",
lblMaxGroupNum:"Entry Specification",
lblCurGroupNum:"Number of Used Groups",

lblGroupTable:"Group Address Table",
lblGroupAddress:"Group Address",
txtPortTable:"Port Configuration Table",
txtEditPortTable:"Modify Port Configuration",
lblPortType:"Role",
lblPortFastLeave:"Fast Leave",
lblNone:"None",
lblReceiver:"Receiver",
lblSource:"Multicast Source",

lblUnused:"Unused"
},
<!--#endif-->

/* Mirror */
<!--#ifdef CONFIG_SYS_MIRROR-->
lang_mirror: {
titMirrorSettingTbl: "Mirroring Table",
titEditMirroring: "Modify Mirror",
lblMirrorSessionId: "Session ID",
lblMirrorDestPort: "Destination Port",
lblMirrorIngressStateLabel: "Send and receive normal packets",
lblMirrorIngressState: "(Normal*)",
lblMirrorIngressStateHint: "\"*\" Allows the mirror port to send and receive normal packets",
lblMirrorSourceRXPort: "Source inbound port",
lblMirrorSourceTXPort: "Source outbound port"
},
<!--#endif-->

/* Port */
lang_port:{
  titPortSettingTable:"Port Configuration Table",
  titEditPortSetting:"Modify port configuration",
  lblEntryNo:"number",
  lblPort:"port",
  lblPortType:"type",
  lblDescription:"description",
  lblAdminStatus:"state",
  lblOperStatus:"Connection Status",
  lblSpeed:"rate",
  lblDuplex:"Duplex",
  lblFlowCtrl:"Flow Control",
  lblFlowCtrlOn:"Open",
  lblFlowCtrlOff:"closure",
  lblAuto:"Self-negotiation",
  lblAutoCapArg:"Self-negotiation - $1",
  txtUp:"Up",
  txtDown:"Down",
  txtDuplexFull:"Full",
  txtDuplexHalf:"Half",
  txtMediaCopper:"$1M Copper",
  txtMediaFiber:"$1M Fiber",
  txtMediaGFiber:"$1G Fiber",
  txtMediaComboC:"$1M Combo Copper",
  txtMediaComboF:"$1M Combo Fiber",
  <!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
  lblRecoveryInterval:"Recovery interval",
  lblACL:"ACL",
  lblARPRateLimit:"ARP packet rate limit",
  lblBPDUGuard:"BPDU Guard",
  lblBcastFlood:"Broadcast Flooding",
  lblDHCPRateLimit:"DHCP message rate limit",
  lblPortSecurity:"Port Security",
  lblSelfLoop:"Self-loop detection",
  lblUdld:"UDLD",
  lblUnicastFlood:"Unicast flooding",
  lblUnknownMcastFlood:"Unknown multicast flooding",
  <!--#endif-->
  lblJumboSize:"Jumbo Frames",
  <!--#ifdef CONFIG_SYS_EEE-->
  titEEESettingTable:"EEE Configuration Table",
  titEditEEESetting:"Modify EEE configuration",
  lblEEEAdminState:"state",
  lblEEEState:"Running status",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
  titPortErrorDisabledTable:"Error Disabled configuration table",
  lblErrorDisableReason:"reason",
  lblReasonBpduGuard:"BPDU Guard",
  lblReasonUdld:"UDLD",
  lblReasonSelfLoop:"Self-loop",
  lblReasonBroadcastFlood:"Broadcast Flooding",
  lblReasonUnknownMltiCastFlood:"Unknown multicast flooding",
  lblReasonUnicastFlood:"Unicast flooding",
  lblReasonAcl:"ACL",
  lblReasonPortSecurity:"Port Security",
  lblReasonDhcpRateLimit:"DHCP message rate limit",
  lblReasonArpRateLimit:"ARP packet rate limit ",
  lblTimeLeft:"Recovery time (seconds)",
  <!--#endif-->
  titProtectTable:"Isolated Port Table",
  titEditProtect:"Modify the isolated port",
  lblProtectState:"state",
  lblProtected:"isolation",
  lblUnprotected:"Non-isolated",

  lblIntf:"interface",
  lblEther:"Etherlike",
  lblRmon:"RMON",
  lblAll:"All",
  lblMibType:"MIB Counter",
  lblTxPercent:"Transmit (%)",
  lblRxPercent:"Receive (%)"
},

<!--#ifdef CONFIG_SYS_PORT_SECURITY-->
/* Port Security */
lang_psecure:{
  titSettingTable:"Port Security Table",
  titPsecureMacTable:"Port Security Address Table",
  titPsecureMacAdd:"Adding a port security address",
  titPsecureMacEdit:"Modify the port security address",
  titEditPsecure:"Modify port security",
  lblState:"state",
  lblRateLimit:"Rate Limiting",
  lblPps:"pps",
  lblAddrLimit:"Maximum number of MAC addresses",
  lblViolateAction:"Over-limit action",
  lblViolateProtect:"Protect",
  lblViolateRestrict:"Restrict",
  lblViolateShutdown:"Shutdown",
  lblSticky:"Sticky",
  lblTotalAddr:"Total",
  lblConfigAddr:"Configured",
  lblViolateNum:"Exceeding the limit"
},
<!--#endif-->

/* QoS */
lang_qos:{
  lblQosMode:"state",
  lblTrustType:"Trust Model",
  lblBasic:"Basic",
  titQosPortSettingTable:"Port Configuration Table",
  titQosEditPortSetting:"Modify port configuration",
  lblCoS:"CoS",
  lblCosDscp:"CoS-DSCP",
  lblTrustState:"Port Trust",
  lblRemark:"Re-marking",
  lblRemarkCoS:"CoS",
  lblRemarkDSCP:"DSCP",
  lblRemarkIPP:"IP Precedence",

  titQueueTable:"Queue schedule",
  lblQueue:"queue",
  lblScheduleMethod:"Scheduling method",
  lblStrictPriority:"Strict priority",
  lblWrr:"WRR",
  lblWrrWeight:"Weight",
  lblWrrBandwidthPercent:"WRR bandwidth(%)",

  titCos2QueueMap:"CoS-queue mapping table",
  titQueue2CosMap:"Queue-CoS mapping table",

  titDscp2QueueMap:"DSCP-Queue Mapping Table",
  titQueue2DscpMap:"Queue-DSCP mapping table",
  lblDSCP:"DSCP",

  titIpp2QueueMap:"IP priority-queue mapping table",
  titQueue2IppMap:"Queue-IP priority mapping table",
  lblIpp:"IP Precedence",

  titBwCtrlTable:"Port rate limit table",
  titEditBwCtrl:"Modify the port rate limit",
  lblIgr:"Entrance",
  lblEgr:"exit",
  lblState:"state",
  lblRate:"rate(Kbps)",

  titEgrQueBwCtrlTable:"Export Queue Table",
  titEditEgrBwQueCtrl:"Modify the egress queue",
  lblQueueNumArg:"Queue $1",
  lblCIR:"Average rate(Kbps)"
},

<!--#ifdef CONFIG_SYS_LIB_RADIUS-->
lang_radius:{
  lblUseDfltParam:"RADIUS default parameters",
  lblRetries:"Reconnection times",
  lblTimeout:"Timeout",
  lblDeadTime:"Interruption time",
  lblKeyStr:"Key",
  lblKeyStrEncrypt:"Key (encrypted)",

  titRadiusSrv:"RADIUS server list",
  lblPri:"Priority",
  lblUsgType:"use",
  lblTypeLogin:"Login authentication",
  lblTypeDot1x:"802.1X authentication",
  lblTypeAll:"all",

  titRadiusSrvAdd:"Adding a RADIUS Server",
  titRadiusSrvEdit:"Modify the RADIUS server",
  lblUseDflt:"Use default values"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI-->
lang_rmon: {
  /* RMON Statistic */
  /* Per Port */
  lblBytesRec:"Bytes Received",
  lblDropEvents:"Drop Events",
  lblPktsRec:"Packets Received",
  lblBPktsRec:"Broadcast Packets Received",
  lblMPktsRec:"Multicast Packets Received",
  lblCRCAlignErr:"CRC & Align Errors",
  lblUndersizePkts:"Undersize Packets",
  lblOversizePkts:"Oversize Packets",
  lblFragments:"Fragments",
  lblJabbers:"Jabbers",
  lblCollisions:"Collisions",
  lblFrames64B:"Frames of 64 Bytes",
  lblFrames65127B:"Frames of 65 to 127 Bytes",
  lblFrames128255B:"Frames of 128 to 255 Bytes",
  lblFrames256511B:"Frames of 256 to 511 Bytes",
  lblFrames5121023B:"Frames of 512 to 1023 Bytes",
  lblFramesOver1024B:"Frames Greater than 1024 Bytes",

  /*  All Port */
  txtRmonStatisticsTable:"Statistics Table",
  txtRmonStatisticsIntf:"View Port Statistics",
  thBytesRec:"Bytes<br>Received",
  thDropEvents:"Drop<br>Events",
  thPktsRec:"Packets<br>Received",
  thBPktsRec:"Broadcast Packets<br>Received",
  thMPktsRec:"Multicast Packets<br>Received",
  thCRCAlignErr:"CRC & Align<br>Errors",
  thUndersizePkts:"Undersize<br>Packets",
  thOversizePkts:"Oversize<br>Packets",
  thFragments:"Fragments",
  thJabbers:"Jabbers",
  thCollisions:"Collisions",
  thFrames64B:"Frames of<br>64 Bytes",
  thFrames65127B:"Frames of<br>65 to 127 Bytes",
  thFrames128255B:"Frames of<br>128 to 255 Bytes",
  thFrames256511B:"Frames of<br>256 to 511 Bytes",
  thFrames5121023B:"Frames of<br>512 to 1023 Bytes",
  thFramesOver1024B:"Frames Greater<br>than 1024 Bytes",

  btnStatsIntfView:"View",
  msgConfirmClearCounter:"This operation will clear the interface statistics, the Etherlike statistics, and the RMON statistics for the selected interface.",

  /* History */
  /* Control Table */
  txtView:"View",
  txtHistoryControlTable:"History Table",
  txtHistoryLogTable:"View History",
  thHisIdxBR:"Entry",
  thSrcInf:"Port",
  thOwner:"Owner",
  thBuckets:"Sample",
  thReqBuckets:"Maximum",
  thCurrentBuckets:"Current",

  /* Add/Edit Dialog */
  titAddHistory:"Add History",
  titEditHistory:"Edit History",
  lblNewHisIdx:"Entry",
  lblEditHisIdx:"Entry",
  lblSrcIntf:"Port",
  lblReqBuckets:"Max Sample",
  lblInterval:"Sampling Interval",
  lblOwner:"Owner",

  /* History Table */
  txtHisIdx:"Entry",
  thOwner:"Owner",
  thSampleNum:"Sample<br>No.",
  thDropEvents:"Drop<br>Events",
  thBytesRec:"Bytes<br>Received",
  thPktssRec:"Packets<br>Received",
  thBPktsRec:"Broadcast<br>Packets",
  thMPktsRec:"Multicast<br>Packets",
  thCRCAlignErr:"CRC & Align<br>Errors",
  thUndersizePkts:"Undersize<br>Packets",
  thOversizePkts:"Oversize<br>Packets",
  thFragments:"Fragments",
  thJabbers:"Jabbers",
  thCollisions:"Collisions",
  thUtilization:"Utilization",

  /* Event */
  /* Control Table */
  txtEventControlTable:"Event Table",
  txtEventLogTable:"View Event Log",
  thEventIdx:"Entry",
  thCommunity:"Community",
  thDescription:"Description",
  thNotifiType:"Notification",
  thTime:"Time",
  thOwner:"Owner",

  /* Add/Edit Dialog */
  titAddEvent:"Add Event",
  titEditEvent:"Edit Event",
  lblNewEventIdx:"Entry",
  lblEditEventIdx:"Entry",
  lblCommunity:"Community",
  lblDescription:"Description",
  lblNotifiType:"Notification",
  lblNotifiTypeNone:"None",
  lblNotifiTypeLog:"Event Log",
  lblNotifiTypeTrap:"Trap",
  lblNotifiTypeLogTrap:"Event Log and Trap",
  lblOwner:"Owner",

  /* Event Table */
  thLogNum:"Log ID",
  thLogTime:"Time",
  thDescription:"Description",

  /* RMON Alarem */
  /* Control Table */
  txtAlarmControlTable:"Alarm Table",
  thAlarmIdx:"Entry",
  thIntf:"Port",
  thAlarmCnt:"Counter",
  thCntName:"Name",
  thCntValue:"Value",
  thSampleType:"Sampling",
  thAlarmRising:"Rising",
  thAlarmFalling:"Falling",
  thAlarmThreshold:"Threshold",
  thAlarmEvent:"Event",
  thStartupAlarm:"Trigger",
  thInterval:"Interval",
  thOwner:"Owner",

  /* Add/Edit Dialog */
  titAddAlarm:"Add Alarm",
  titEditAlarm:"Edit Alarm",
  lblNewAlarmIdx:"Entry",
  lblEditAlarmIdx:"Entry",
  lblIntf:"Port",
  lblCntName:"Counter",
  lblCntValue:"Value",
  lblSampleType:"Sampling",
  lblNone:"None",
  lblAbsolute:"Absolute",
  lblDelta:"Delta",
  lblStartupAlarm:"Trigger",
  lblRising:"Rising",
  lblFalling:"Falling",
  lblRisingAndFalling:"Rising and Falling",
  lblInterval:"Interval",
  lblOwner:"Owner",

  lblBytesRec:"Received Bytes (Octets)",
  lblDropEvents:"Drop Events",
  lblPktsRec:"Received Packets",
  lblBPktsRec:"Broadcast Packets Received",
  lblMPktsRec:"Multicast Packets Received",
  lblCRCAlignErr:"CRC & Align Errors",
  lblUndersizePkts:"Undersize Packets",
  lblOversizePkts:"Oversize Packets",
  lblFragments:"Fragments",
  lblJabbers:"Jabbers",
  lblCollisions:"Collisions",
  lblFrames64B:"Frames of 64 Bytes",
  lblFrames65127B:"Frames of 65 to 127 Bytes",
  lblFrames128255B:"Frames of 128 to 255 Bytes",
  lblFrames256511B:"Frames of 256 to 511 Bytes",
  lblFrames5121023B:"Frames of 512 to 1023 Bytes",
  lblFramesOver1024B:"Frames Greater than 1024 Bytes",
  txtRmonSnmpDisabledInfo:"The SNMP service is currently disabled.<br> For RMON configuration to be effective, the $1 SNMP service $2 must be enabled.",
  txtRmonEventDisabledInfo:"For RMON Alarm configuration, the $1 RMON Event $2 must be added.",

  /* error message */
  errHistoryTableOverflow:"RMON history table is full",
  errAlarmTableOverflow:"RMON alarm table is full",
  errEventTableOverflow:"RMON event table is full"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_L3_ROUTE-->
lang_ipconf:{
 titAddIPv4Intf: "Add IPv4 Interface",  
  titEditIPv4Intf: "Edit IPv4 Interface",
  titAddIPv6Intf: "Add IPv6 Interface",
  titEditIPv6Intf: "Edit IPv6 Interface",
  titAddIPv4Route: "Add IPv4 Static Route",
  titEditIPv4Route: "Edit IPv4 Static Route",
  titAddIPv6Route: "Add IPv6 Static Route",
  titEditIPv6Route: "Edit IPv6 Static Route",
  titAddIPv4Arp: "Add ARP",
  titEditIPv4Arp: "Edit ARP",
  titAddIPv6Neigh: "Add Neighbor",
  titEditIPv6Neigh: "Edit Neighbor",

  lblIPv4IntfTable: "IPv4 Interface Table",
  lblIntf: "Interface",
  lblIPAddrType: "IP Address Type",
  lblIPAddr: "IP Address",
  lblIPv6Addr: "IPv6 Address",
  lblMacAddr: "MAC Address",
  lblMask: "Mask",
  lblStatus: "Status",
  lblRoles: "Roles",
  txtDynamic: "Dynamic",
  txtStatic: "Static",
  txtDHCP: "DHCP",
  txtStatusTentative: "Validation",
  txtStatusValid: "Valid",
  txtStatusValidDuplicated: "Valid Duplication",
  txtStatusDuplicated: "Duplication",
  txtStatusDelayed: "Delay",
  txtStatusNotReceived: "Not Received",
  txtIpPrimary: "Primary Address",
  txtIpSub: "Subaddress",
  txtRip:"Rip",
  txtRipng:"Ripng",
  txtOspf:"Ospf",
  txtOspf6:"Ospf6",
  txtIsis:"Isis",
  txtBgp:"Bgp",
	  
  lblLoopback: "Loopback",
  lblAddressType: "Address Type",
  lblMask: "Mask",
  lblNetworkMask: "Network Segment Mask",
  lblPrefixLength: "Prefix Length",

  lblIPv4RouteTable: "IPv4 Routing Table",
  lblDstIP: "Destination IP",
  lblDstIPPrefix: "Destination IP Prefix",
  lblRouteType: "Routing Type",
  lblNextHopAddr: "Next Hop Routing Address",
  lblRouteOwner: "Route Owner",
  lblMetric: "Metric",
  lblAdminDistance: "Administrative Distance",
  lblOutIntf: "Outgoing Interface",

  txtDefault: "Default",
  txtLocal: "Local",
  txtReject: "Reject",
  txtRemote: "Remote",
  txtDirectlyConnected: "Directly Connected",
  txtDHCP: "DHCP",

  lblIPv4ArpTable: "ARP Table",
  lblArpAgingTime: "ARP Table Entry Aging Time",
  lblArpClear: "Clear ARP Table Entry",
  lblDynamic: "Dynamic",
  lblStatic: "Static",
  lblNormalAging: "Normal Aging",
  lblStatus: "Status",
  txtIPv4ArpIntf: "Note: Only interfaces with valid IPv4 addresses are available",
  lblV4ArpProxy: "Arp Proxy",

  lblIPv6IntfTable: "IPv6 Interface Table",
  lblIPv6Routing: "IPv6 Unicast Routing",
  lblDhcpv6Client: "DHCPv6 Client",
  lblStateless: "Stateless",
  tblRefresh: "Information Refresh Time",
  lblRefresh: "Information Refresh Time",
  tblRefreshMin: "Minimum Information Refresh Time",
  lblRefreshMin: Minimum information refresh time
  lblAutoConf: Automatic configuration
  lblDadAttemps: DAD attempts
  tblSendICMPUnreachMsg: Send ICMPv6 Unreachable
  lblSendICMPUnreachMsg: Send ICMPv6 Unreachable
  lblGlobal: Global
  lblLinkLocal: Link local
  lblMcast: Multicast
  lblIPv6RouteTable: IPv6 routing table
  lblIPv6Prefix: IPv6 prefix
  lblIPv6PrefixLen: IPv6 prefix length
  lblNextHopType: Next hop type
  lblIPv6AddrTable: IPv6 address table
  lblIPv6AddrType: IPv6 address type
  lblIPv6Addr: IPv6 address
  lblDADStatus: DAD status

  lblTentative: "Verify",
  lblDuplicate: "Duplicate",
  lblActive: "Active",
  lblEUI64: "EUI-64",

  lblIPv6NeighTable: "IPv6 Neighbor Table",
  lblNeighClear: "Clear Neighbor Table",
  lblIPv6Router: "Routing"

},
<!--#endif-->

<!--#ifdef CONFIG_SYS_UI_SNMP-->
/*SNMP*/
lang_snmp:{

  <!--#ifdef CONFIG_SYS_SNMPV3-->
  titSnmpViewTable:"View Table",
  titAddSnmpView:"Add View",
  lblViewName:"View",
  lblObjectIDSubtree:"OID Subtree",
  lblObjectIDSubtreeView:"Type",
  lblIncluded:"Included",
  lblExcluded:"Excluded",

  titSnmpGroupTable:"Group Table",
  titAddSnmpGroup:"Add Group",
  titEditSnmpGroup:"Edit Group",
  lblGroupName:"Group",
  lblSecurityModel:"Version",
  lblSecurityLevel:"Security Level",
  lblNoAuth:"No Security",
  lblAuth:"Authentication",
  lblPriv:"Authentication and Privacy",
  lblView:"View",
  lblRead:"Read",
  lblWrite:"Write",
  lblNotify:"Notify",
  lblSNMPv3:"SNMPv3",
  lblNoAuthNoPriv:"No Authentication and No Privacy",
  lblAuthNoPriv:"Authentication and No Privacy",
  lblAuthPriv:"Authentication and Privacy",

  lblcommType:"Type",
  lblBasic:"Basic",
  lblAdvanced:"Advanced",

  titSnmpUserTable:"User Table",
  titAddSnmpUser:"Add User",
  titEditSnmpUser:"Edit User",
  lblUserName:"User",
  lblAuthTitle:"Authentication",
  lblAuthProt:"Method",
  lblAuthPswd:"Password",
  lblEncryTitle:"Privacy",
  lblEncryProt:"Method",
  lblEncryPswd:"Password",
  lblMD5:"MD5",
  lblSHA:"SHA",
  lblDES:"DES",
  lblNone:"None",

  lbllocalEngineID:"Local Engine ID",
  lblEngineID:"Engine ID",
  titRemtEngineID:"Remote Engine ID Table",
  titAddRemtEngineID:"Add Remote Engine ID",
  titEditRemtEngineID:"Edit Remote Engine ID",

  lblSnmpSecName:"Community / User",
  lblTimeout:"Timeout",
  lblRetries:"Retry",
  lblUseDflt:"Use Default",

  titEditSnmpCommunity:"Edit Community",

  txtGrpNeedViewInfo:"Configure $1 SNMP View $2 to associate a non-default view with a group.",
  txtComNeedGrpInfo:"The access right of a community is defined by a group under advanced mode.<br>Configure $1 SNMP Group $2 to associate a group with a community.",
  txtUserNeedGrpInfo:"Configure $1 SNMP Group $2 to associate an SNMPv3 group with an SNMPv3 user.",
  txtNotifyNeedSecInfo:"For SNMPv1,2 Notification, $1 SNMP Community $2 needs to be defined.<br>For SNMPv3 Notification, $3 SNMP User $4 must be created.",

  errMIBOIDNotValid:"MIB OID is not valid",
  errUserAuthPwLenMin:"Authentication password length must be equal or greater than 8",
  errUserPrivPwLenMin:"privacy password length must be equal or greater than 8",
  errNotEngineID:"Value is an illegal Engine ID",
  errComNoGrp:"There is no SNMPv1,2 group to be used by a community",
  errUserNoGrp:"There is no SNMPv3 group to be used by a user",
  errNotifyNoUser:"There is no user to be used by an SNMPv3 notification",
  <!--#endif-->

  txtNotifyNeedComInfo:"For SNMPv1,2 Notification, $1 SNMP Community $2 needs to be defined.",
  errNotifyNoCom:"There is no community to be used by a notification",

  titSnmpCommunityTable:"Community Table",
  titAddSnmpCommunity:"Add Community",
  lblCommunityName:"Community",
  lblSNMPv1:"SNMPv1",
  lblSNMPv2:"SNMPv2",
  lblAccess:"Access",
  lblReadOnly:"Read-Only",
  lblReadWrite:"Read-Write",

  lblAuthFail:"Authentication Failure",
  lblLinkUpDown:"Link Up / Down",
  lblColdStart:"Cold Start",
  lblWarmStart:"Warm Start",

  titSnmpNotifyTable:"Notification Table",
  titAddSnmpNotify:"Add Notification",
  titEditSnmpNotify:"Edit Notification",
  lblNotifyType:"Type",
  lblTraps:"Trap",
  lblInforms:"Inform",
  lblSnmpVer:"Version"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_STORM_CTRL-->
/* Storm Control */
lang_storm: {
  lblUnit: "Mode",
  lblpps: "pps",
  lblbps: "Kbps",
  txtpps: "pps",
  txtbps: "Kbps",
  lblIFG: "Interframe Gap",
  txtExclude: "Exclude",
  txtInclude: "Include",
  titStormPortSettingTable: "Port Configuration Table",
  titStormEditPortSetting: "Modify Port Configuration",
  lblState: "State",
  lblRate: "Rate",
  lblBroadcast: "Broadcast",
  lblUnknownMul: "Unknown Multicast",
  lblUnknownUni: "Unknown Unicast",
  lblAction: "Action",
  txtDrop: "Drop",
  txtShutdown: "Shutdown"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_STP-->
/* Spanning Tree */
lang_stp:{
  lblTrue: "True",
  lblFalse: "False",
  lblState: "State",
  lblOperMode: "Operation Mode",
  lblPathCost: "Path Cost Mode",
  lblBpduFwd: "BPDU Forwarding Mode",
  lblPriority: "Priority",
  lblHelloTime: "Hello Time",
  lblMaxAge: "Max Age",
  lblFwdDelay: "Forward Delay",
  lblTxHoldCnt: "Tx Hold Count",
  lblBridgeId: "Bridge ID",
  lblRootBridgeId: "Root Bridge ID",
  lblRootPort: "Root Port",
  lblRootCost: "Root Cost",
  lblRegionRootBridge: "Region Root",
  lblInternalRootCost: "Internal Cost",
  lblRemainingHop: "Remaining Hop",
  lblTcCnt: "Topology Change Count",
  lblTcLastTime: "Time of the most recent topology change"

  msgStpModeChange: "Changing the STP mode may cause connection interruption. Do you want to continue?"

lblPortAdminStatus: Status,
lblPortPathCost: Path Cost,
lblPortPriority: Priority,
lblPortEdge: Edge Port,
lblPortEdgeOper: Edge Port Status,
lblPortBpduFilter: BPDU Filter,
lblPortBpduGuard: BPDU Guard,
lblPortP2P: Point-to-Point Configuration,
lblPortP2POper: Point-to-Point Status,
lblPortMode: Mode,
lblPortType: Type,
lblPortRole: Port Role,
lblPortState: Port Status,
lblPortDesignatedBridge: Designated Bridge ID,
lblPortDesignatedPortId: Designated Port ID,
lblPortDesignatedCost: Port Cost,

lblRxCnt: "Receive BPDU",
lblTxCnt: "Send BPDU",
lblConfigBpdu: "Config",
lblTcnBpdu: "TCN",

txtSTP: "STP",
txtRSTP: "RSTP",
txtOperModeSTP: "STP",
txtOperModeRSTP: "RSTP",
txtPathCostLong: "Long mode",
txtPathCostShort: "Short Mode",
txtBpduFwdFiltering: "discard",
txtBpduFwdFlooding: "flooding",
txtDayHrMinSecArg:"$1D/$2H/$3M/$4S",

txtStpIntfStats: "Port Spanning Tree Message Statistics",
txtStpIntfSettingTable: "Port Configuration Table",
txtStpIntfStatsTable: "Message Statistics Table",
titStpIntfEdit: "Modify Port Configuration",

  txtMaster:"Master",
  txtRoot:"Root",
  txtDesignated:"Designated",
  txtAlternate:"Alternate",
  txtBackup:"Backup",
  txtDisabled:"Disabled",
  txtBlocking:"Blocking",
  txtListening:"Listening",
  txtDiscarding:"Discarding",
  txtLearning:"Learning",
  txtForwarding:"Forwarding",

  btnProtocolMigration:"Port protocol migration reset",
  btnStatsIntfView:"Check",

  errInvalidPriority:"Priority must be between $1 and $2, and a multiple of $3.",

<!--#ifdef CONFIG_SYS_PROTO_STP_MSTP-->
  txtMSTP: "MSTP",
  txtOperModeMSTP: "MSTP",

lblRegionName: "Domain Name",
lblRevision: "Revision",
lblMaxHop: "Max Hop",
lblMsti: "MSTI",
lblMstpBpdu: "MSTP",

titStpMstiEdit: "Modify MST Instance Configuration",
titStpMstiIntfEdit: "Modify MST Port Configuration",

txtMstiTable: "MST Instance Configuration Table",
txtMstIntfSettingTable: "MST Port Configuration Table",
txtInternal: "Internal",
txtBoundary: "Boundary",
<!--#endif-->

  txtEnd:"end"
},
<!--#endif-->

/* System */
lang_sys:{
titHome: "Intelligent Switch",
lblSysinfo: "System Information",
titEditSysinfo: "Modify System Information",
lblModel: "Device Model",
lblSysName: "System Name",
lblSysLocation: "Location Information",
lblSysContact: "Contact Information",
lblSysSN: "Device Serial Number",
lblMacAddress: "MAC Address",
lblLoaderVer: "Loader Version",
lblLoaderDate: "Loader Date",
lblFwVer: "Firmware Version",
lblFwDate: "Firmware Date",
lblSysObjID: "System OID",
lblSysUpTime: "System Online Time",
lblSysCurrTime: "Current Time",
txtSysUptimeArg: "$1 Day, $2 Hour, $3 Minute, $4 Second",
lblTime: "Time",

lblNetworkIPv4: "IPv4 Address",
lblNetworkSubIPv4: "Sub-IPv4 Address",
lblSubIPAddress: "IP Address",
lblSubMask: "Subnet Mask",
lblNetworkIPv6: "IPv6 Address",
lblIPAddressType: "Address Type",
lblDynamic: "Dynamic",
lblStatic: "Static",
lblIPAddress: "IP Address",
lblMask: "Subnet Mask",
lblAdminGateway: "Default Gateway",
lblDnsArg: "DNS Server $1",
lblIPv4Addr: "IPv4 Address",
lblIPv4Gateway: "IPv4 Default Gateway",
lblPrefixLength: "Prefix Length",
lblIPv6AddrAutoConfig: "Auto Configuration",
lblDHCPv6Client: "DHCPv6 Client",
lblIPv6Addr: "IPv6 Address",
lblIPv6Gateway: "IPv6 Gateway",
lblIPv6DefaultGateway: "IPv6 Default Gateway",
lblLinkLocalAddress: "Link Local Address",
lblIPv6InUseAddress: "IPv6 Address in Use",
lblIPv6InUseGateway: "IPv6 Gateway in Use",
lblStateless: "Stateless",
lblInfoRefreshTime: "Information Refresh Time",
lblMinInfoRefreshTime: "Minimum Information Refresh Time",
lblInfinite: "Unlimited",

titUserAccountTable: "User Accounts",
titAddUserAccount: "Add User Account",
titEditUserAccount: "Modify User Account",
lblUserName: "User Name",
lblPwd: "Password",
lblPwdConfirm: "Confirm Password",
lblPwdEncrypt: "Encrypt",
lblPwdText: "Use Plain Text",
lblPwdNone: "No Password",
lblPriv: "Permissions",
lblPrivTypeAdmin: "Administrator",
lblPrivTypeUser: "User",
lblPrivTypeOther: "Other",
errConfirmPassword: "The Password and Confirm Password fields must be the same.",
errInvalidASCII: "The password is an invalid ASCII character.",
errUserExist: "The entry already exists.",
errUserNotExist: "The user does not exist.",
errMaxNumUserArg: "The maximum number of local users supported is $1.",
errDeleteDeafultUser: "Unable to delete the default username.",

lblSave: "Save",
lblLogout: "Log out",
msgConfirmSave: "Save the running configuration to the startup configuration. Continue?",
msgConfirmReboot: "Reboot the system. Unsaved configuration changes will be lost. Continue?",
msgConfirmLogout: "Logging out will disconnect the current session. Continue?",
msgConfirmSupport: "Backing up technical support files to the local PC. The backup may take several seconds, depending on your configuration. Continue?",
msgRebooting: "System restarting...",
msgSaveFail: "Save failed",
msgSupportFail: "Backing up technical support files failed",

btnReboot: "Reboot",
btnRestore: "Factory Reset",
btnSupport: "Support",
btnLanguage: "English",
btnDebug: "Debug",
msgConfirmRestore: "Restore to factory settings. Requires device restart for this to take effect. Continue?",
msgConfirmRestoreReboot: "Reboot the device now. Continue?"
},

<!--#ifdef CONFIG_SYS_LIB_TACPLUS-->
lang_tacplus:{
lblUseDfltParam: "Use default parameters",
lblTimeout: "Timeout",
lblKeyStr: "Key string",
lblKeyStrEncrypt: "Key string (encrypted)",
titTacplusSrv: "TACACS+ server configuration",
lblPri: "Priority",
titTacplusSrvAdd: "Add TACACS+ server",
titTacplusSrvEdit: "Modify TACACS+ server",
lblUseDflt: "Use default values"
},
<!--#endif-->

lang_time:{
lbltimeSrc: "Time Source",
lbltimeSntp: "SNTP",
lblSntpServerPort: "Server Port Number",
lbltimeHost: "Get from Computer",
lbltimeManual: "Manual Configuration",
lbltimeTZ: "Time Zone",
lbltimeType: "Type",
lbltimeDST: "Daylight Saving Time",
lbltimeDSO: "Compensation Time",
lbltimeDSNone: "None",
lbltimeDSRec: "Recurring",
lbltimeDSNRec: "Non-Recurring",
lbltimeDSUSA: "USA",
lbltimeDSEU: "European",
lbltimeFrom: "From",
lbltimeTo: "To",
lbltimeNrecF: "Non-Recurring From",
lbltimeNrecT: "Non-Recurring To",
lbltimeDevice: "Current Time",
lblDay: "Day",
lblWeek: "Week of Week",
lblMonth: "Month",
lblDate: "Date",
lblTime: "Time",
lblSun: "Sunday",
lblMon: "Monday",
lblTue: "Tuesday",
lblWed: "Wednesday",
lblThu: "Thursday",
lblFri: "Friday",
lblSat: "Saturday",
lblFirst: "First",
lblLast: "Last",
lblJan: "January",
lblFeb: "February",
lblMar: "March",
lblApr: "April",
lblMay: "May",
lblJun: "June",
lblJul: "July",
lblAug: "August",
lblSep: "September",
lblOct: "October",
lblNov: "November",
lblDec: "December",
lbltimeState: "State",
lbltimePortSettingTable: "Port Setting Table",
lbltimePortEditSetting: "Edit port configuration",
errMsgDateFormat: "The date value format is incorrect",
errMsgTimeFormat: "The time value format is incorrect",
errMsgDLSEarlier: "The start date must be earlier than the end date.",
errMsgDLSGreaterOffset: "The difference between the daylight saving time from and to must be greater than the time setting offset",
lblHour: "Hours",
lblMinute: "Minutes",
lblSecond: "Seconds"
},


<!--#ifdef CONFIG_SYS_PROTO_UDLD-->
lang_udld: {
  titUdldPortSettingTable: "Port Configuration Table",
titUdldEditPortSetting: "Modify Port Configuration",
titTableUdldNeighbor: "Neighbor Table",
lblUdldMsgTime: "Message Sending Interval",
lblPortMode: "Mode",
lblUdldPortDisable: "Disable",
lblUdldPortNormal: "Normal",
lblUdldPortAggresive: "Active",
lblUdldBidirectionalState: "Bidirectional State",
lblUdldOperState: "Administrative State",
lblUdldNeighbors: "Neighbors",
lblUdldMsgInterval: "Message Interval",
lblUdldTimeoutInterval: "Timeout",
lblUdldExpirationTime: "Expiration Time",
lblUdldcurNeistate: "Current Neighbor State",
lblUdldDeviceID: "Device ID",
lblUdldDeviceName: "Device Name",
lblUdldPortID: "Port Number",
lblUdldBiState_0: "Unknown",
lblUdldBiState_1: "Receive/Transmit Loop",
lblUdldBiState_2: "Neighbor's echo is empty",
lblUdldBiState_3: "Neighbor State Mismatch",
lblUdldBiState_4: "Bidirectional",
lblUdldBiState_5: "Unidirectional",
  lblUdldOperState_0:"Link down",
  lblUdldOperState_1:"",
  lblUdldOperState_2:"Link up",
  lblUdldOperState_3:"Advertisement",
  lblUdldOperState_4:"Detection",
  lblUdldOperState_5:"Extended Detection",
  lblUdldOperState_6:"Advertisement - SINGLE NEIGHBOR DETECTED",
  lblUdldOperState_7:"Advertisement - MULTIPLE NEIGHBORS DETECTED",
  lblUdldOperState_8:"Disabled port",
  lblUdldOperState_9:"Link up"
},
<!--#endif-->

/* VLAN */
lang_vlan:{
titVLAN: "VLAN Table",
titVLANConfigurationTable: "VLAN Configuration Table",
titVLANPortSettingTable: "Port Configuration Table",
titVLANMembershipSettingTable: "Member List",
titVLANEdit: "Change VLAN",
titVLANNameEdit: "Change VLAN Name",
titVLANPortEdit: "Change Port Configuration",
lblVlanName: "Name",
lblVlanMode: "Mode",
lblVlanType: "Type",
lblVlanIntfState: "VLAN Interface State",
lblVlanMembership: "Member",
  lblVlanPVID:"PVID",
  lblVlanForbidden:"Forbidden",
  lblVlanExcluded:"Excluded",
  lblVlanTagged:"Tagged",
  lblVlanUntagged:"Untagged",
  lblVlanHybrid:"Hybrid",
  lblVlanTrunk:"Trunk",
  lblVlanAccess:"Access",
  lblVlanTunnel:"Tunnel",
  lblVlanAcceptFrameType:"Accept Frame Type",
  lblVlanIngressFiltering:"Ingress Filtering",
  lblVlanUplink:"Uplink",
  lblVlanTPID:"TPID",
  lblVlanAll:"All",
  lblVlanTagOnly:"Tag Only",
  lblVlanUntagOnly:"Untag Only",
  lblVlanDefault:"Default",
  lblVlanAdmin: "Management VLAN",
lblVlanOper: "Operational VLAN",
lblCreatedVlans: "Created VLAN",
lblManagementVLAN: "Management VLAN",

errMsgManagementVLAN: "Caution: Changing the management VLAN may cause a connection interruption."
},

<!--#ifdef CONFIG_SYS_MACVLAN-->
/* VLAN Mac */
lang_vlanmac:{
titMVGroupTable: "MAC Group Table",
titMVAddGroup: "Add MAC Group",
titMVEditGroup: "Modify MAC Group",
titMVBindingTable: "MAC Group Binding Table",
titMVAddBinding: "Add MAC Group Binding",
titMVEditBinding: "Modify MAC Group Binding",
lblMVMac: "MAC Address",
lblMVMacMask: "Mask",
lblMVGroup: "Group ID",
msgMVHybridPort: "Note: Only ports with the VLAN mode set to Hybrid can match MAC VLANs."
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_VOICEVLAN-->
/* VLAN Voice */
lang_vlanvoice:{
titVVPortSettingTable: "Port Configuration Table",
titVVEditPortSettingTable: "Modify Port Configuration",
titVVOUITable: "Voice OUI Table",
titVVOUIAdd: "Add Voice OUI",
titVVOUIEdit: "Modify Voice OUI",
lblVVState: "State",
lblVVRemark: "CoS / 802.1p Remark",
lblVVAgingTime: "Aging Time",
lblVVPortState: "State",
lblVVPortMode: "Mode",
lblVVPortCosMdoe: "QoS Policy",
lblVVAuto: "Auto",
lblVVManual: "Manual",
lblVVAll: "All",
lblVVSource: "Voice Packet",
lblVVOUIAddress: "OUI",
lblVVDescription: "Description"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTOCOL_VLAN-->
/* VLAN Protocol */
lang_vlanprotocol:{
titPVGroupTable: "Port Configuration Table",
titPVAddGroup: "Add Protocol Group",
titPVEditGroup: "Modify Protocol Group",
titPVBindingTable: "Protocol Group Binding Table",
titPVAddBinding: "Add Protocol Group Binding",
titPVEditBinding: "Modify Protocol Group Binding",
lblPVProtocolValue: "Protocol Value",
lblPVFrameType: "Message Type",
lblPVGroup: "Group ID",
lblPVEII: "Ethernet_II",
lblPVLLC: "IEEE802.3_LLC_Other",
lblPVSNAP: "RFC_1042",
msgPVHybridPort: "Note: Only ports with the VLAN mode set to Hybrid can match protocol VLANs."
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_-->
/* VLAN IP subnet */
lang_vlanIPsubnet:{
titIPsubnetGroupTable: "IP Subnet Configuration Table",
titIpSubnetAddGroup: "Add IP Subnet Group",
titIpSubnetEditGroup: "Modify IP Subnet Group",
titIpSubnetBindingTable: "IP Subnet Group Binding Table",
titPIpSubnetAddBinding: "Add IP Subnet Group Binding",
titIpSubnetEditBinding: "Modify IP Subnet Group Binding",
lblIPSubMask: "Subnet IP Mask",
lblIPSubAddr: "Subnet IP Address",
lblIPSubGroup: "Group ID",
lblIPSubVlanId: "Vlan ID",
lblIPSubPri: "Priority",
lblIPSubBindPort: "Bound Port",
msgIPSubHybridPort: "Note: Only ports with Hybrid VLAN mode can match protocol VLANs. One group can only be bound to one port",
msgIPSubBindPort: "Note: One group can only be bound to one port"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_SURVEILLANCEVLAN-->
/* Surveillance Voice */
lang_vlansurveillance:{
titSVPortSettingTable: "Port Configuration Table",
titSVEditPortSettingTable: "Modify Port Configuration",
titSVOUITable: "Surveillance OUI Table",
titSVOUIAdd: "Add Surveillance OUI",
titSVOUIEdit: "Modify Surveillance OUI",
lblSVState: "State",
lblSVRemark: "CoS / 802.1p Remark",
lblSVAgingTime: "Aging Time",
lblSVPortState: "State",
lblSVPortMode: "Mode",
lblSVPortCosMdoe: "QoS Policy",
lblSVAuto: "Automatic",
lblSVManual: "Manual",
lblSVAll: "All",
lblSVSource: "Video Packet",
lblSVOUIAddress: "OUI",
lblSVDescription: "Description"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_POE-->
/* POE */
lang_poe:{
titPoeSummary: "Device Information",
titPoePortSettingTable: "Port Configuration Table",
titPoeEditPortSetting: "Modify Port Configuration",
lblPoeDevPower: "Device Power (mW)",
lblPoeDevTemp: "Device Temperature (°C)",
lblPoeEntryNo: "Number",
lblPoePort: "Port",
lblPoePortEnable: "Management Status",
lblPoePortStatus: "Operational Status",
lblPoePortType: "Type",
lblPoePortLevel: "Level",
lblPoePortPowerLimit: "Rated Power (mW)",
lblPoePortPower: "Actual Power (mW)",
lblPoePortVoltage: "Voltage (V)",
lblPoePortCurrent: "Current (mA)",
lblPoePortWatchDog: "Watchdog Status",
  txtOn:"On",
  txtOff:"Off"
},
<!--#endif-->
lang_sysError:{
	<!--port err start-->
errPortAuto: "The port cannot be configured for auto-negotiation."
errFiberDuplex: "The optical port duplex mode configuration is invalid."
errPortAutoSpeed: "The port auto-negotiation rate is invalid."
errPortSpeed: "The port rate is invalid."
errPortDuplex: "The port duplex mode is invalid."
errPort1000NO: "The Fast Ethernet port does not support 1000M speed."
errPort1000HalfNO: "Gigabit Ethernet does not support 1000M half-duplex mode."
errFiberSpeed: "The optical port rate is invalid."
errPortNoVlan: "The port does not belong to a VLAN member."
errFiberNoEEE: "The optical port does not support EEE function."
errPortNoFlowCtrl: "The optical port and multiplexed port do not support flow control function."
	<!--port err end-->
	<!--trunk err start-->
errTrunkid: "Invalid trunk ID",
errTrunkPorts: "Invalid trunk member port",
errTrunkHash: "Invalid trunk hash algorithm",
errTrunkGroup: "Cannot add static member to dynamic LACP group",
errTrunkLess: "Trunk member must be at least 2",
errTrunkMore4: "Maximum 4 trunk members",
errTrunkMore8: "Maximum 8 trunk members",
errTrunkMore16: "Maximum 16 trunk members",
errTrunkAuth: "Authentication should not be enabled on the port",
errTrunkAnother: "Port already belongs to another trunk",
errTrunkVlan: "Trunk member port has different joining VLAN/tag",
errTrunkMirror: "Mirror port cannot be a trunk member port",
errTrunkRate: "Port should not be subject to ingress/egress rate limits",
errTrunkNego: "Trunk member ports must all have the same negotiation mode."
errTrunkSpeed: "Trunk member ports must all have the same negotiation mode."
errTrunkDuplex: "Trunk member ports must all be full-duplex."
errTrunkNoExist: "The trunk does not exist."
errTrunkBackupPort: "The port is already a trunk member."
errTrunkSecurity: "Port security should not be enabled on the port."
errTrunkEmpty: "There are no members in the trunk."
	<!--port err end-->
	<!--vlan err start-->
errVlanDef: "Default VLAN deletion is not allowed",
errVlanExists: "VLAN exists",
errVlanFrame: "VLAN frame type is invalid",
errVlanID: "VID is out of range",
errVlanMGMT: "Cannot configure the management VLAN as the default VLAN",
errVlanMemvership: "VLAN membership error",
errVlanMGMTDel: "Management VLAN deletion is not allowed",
errVlanMGMTempty: "Management VLAN cannot be created because the member port is empty",
errVlanNameFull: "Exceeds the valid value range",
errVlanNameExist: "VLAN alias exists",
errVlanNotExist: "VLAN alias does not exist",
errVlanNotStatic: "Not a static VLAN",
errVlanNotCreated: "Cannot be created by the user",
errVlanTrunk: "Port is a member of an aggregation group",
errVlanuplink: "Port mode is not an uplink port",
errVlanMVRDel: "MVR VLAN cannot be deleted."
errVlanNotSupport: "The maximum size in the VLAN specification table has been exceeded."
errVoiceID: "Voice VLAN cannot be created as the default VLAN."
errVoiceOUIexist: "The Voice VLAN OUI entry already exists."
errVoiceOUImcast: "The OUI MAC must be a unicast address."
errVoiceOUIzero: "The Voice VLAN OUI cannot be all zeros."
errVoiceauto: "Cannot edit a Voice VLAN auto-mode port in the Voice VLAN."
errVoiceOUIport: "The Voice VLAN state does not enable the OUI."
errVoiceportMode: "The port must be in hybrid or trunk mode."
errVoicePortStaic: "A static member cannot be a candidate member in the VoiceVLAN."
errVoiceOUIenable: "Voice VLAN is enabled on the port."
errVoiceOUIdef: "The default VLAN cannot be configured as a voice VLAN in OUI mode."
errVoiceVidDel: "Voice VLAN cannot be deleted."
errVoiceGustVlan: "Guest VLAN cannot be configured as a voice VLAN."
errVoiceSurveillance: "Surveillance VLAN cannot be configured as a voice VLAN."
errVoiceVidNotExist: "Voice VLAN cannot be enabled because no voice VLAN ID has been assigned."
errVoiceVidEnable: "The voice VLAN ID cannot be restored to None because the voice VLAN is enabled."
errProvlanPort: "Protocol VLAN: A protocol VLAN is configured on the port."
errProvlanPortMode: "The port must be in promiscuous mode."
errProvlanEnablePort: "The port is bound to a protocol VLAN."
errVlanMac: "The maximum value has been exceeded."
errVlanMacNotUni: "The range contains a multicast address."
errVlanMacOverlap: "The range or part of the range is already defined in the database."
errVlanMacLastDel: "Cannot delete the last range from the mapping group."
errVlanMacNoResource: "No resources for this range."
errVlanMacNoGid: "Attempted to map an illegal group number."
errVlanMacGeneral: "The port mode is not promiscuous."
errVlanMacPortMode: "A MAC VLAN-enabled port must be in promiscuous mode."
errSVOUIMcast: "The OUI MAC must be unicast."
errSVOUIExist: "A monitoring VLAN OUI entry exists in the database."
errSVOUIzero: "The monitoring VLAN OUI cannot be all zeros."
errSVGuest: "Guest VLAN cannot be configured as a monitoring VLAN."
errSVVoice: "Voice VLAN cannot be configured as a monitoring VLAN."
errSVDefailt: "The default VLAN cannot be configured as a monitoring VLAN."
errSVNotExist: "Cannot enable monitoring VLAN because no monitoring VLAN ID has been assigned."
errSVVidEnable: "Cannot enable monitoring VLAN ID because monitoring VLAN is already enabled."
errSVEnablePort: "Monitoring VLAN is already enabled on the port."
errSVPortMode: "The port must be in hybrid or mid-trunk mode."
errSVPortStatic: "The port cannot be a static member port of the Surveillance VLAN."
errSVGlobalDisable: "Monitoring VLAN global status is not enabled."
errVidSV: "Monitoring VLAN cannot be configured as the default VLAN."
errSVVidDel: "Monitoring VLAN destruction is not allowed."
errSVUsedPort: "A port in monitoring VLAN auto mode cannot be edited in the VLAN."
	<!--vlan err end-->
	<!--MGMT ACL start-->
errMGMTACLNodorm: "Cannot modify/delete active management access list",
errMGMTACLNodoadd: "Cannot modify active management access list",
errMGMTACLNoSuchlish: "No such management access list",
errMGMTACLNoALLlist: "No such management access list",
errMGMTACLPriexceed: "Value out of range",
errMGMTACLexists: "Entry already exists",
errMGMTACLNopri: "If there is no explicit priority input and the value is out of range, you can use the sequence command to continue input",
errMGMTACLNoSuchace: "Cannot delete a non-existent management access entry",
errMGMTACLentry: "Cannot create a new entry in the mgmt acl table because the table is full",
errMGMTACLportWrong: "Port is incorrectly set or not set",
errMGMTACLsourceIP: "Source IP should be unicast",
errMGMTACLsourceVid: "Wrong vid for source set",
errMGMTACLconsole: "Input profile rule name cannot be console-only, console-only, console-only, or console-only",
errMGMTACLname: "Please specify a list name",
	<!--MGMT ACL end-->
	<!--GVRP err start-->
errGvrpNoenable: "GVRP is not enabled globally."
errGvrpDB: "GVRP VLAN database is full."
errGvrpportNoTrunk: "Port mode is not trunk, GVRP cannot be enabled."
errGvrpMirror: "GVRP cannot be enabled on the mirrored destination port."
errGvrpPortmode: "GVRP port must be in trunk mode to enable."
errGvrpTimerDIV: "GVRP timer value must be 5."
errGvrpTimerJoin: "GVRP join timer must be less than half of leave timer."
errGvrpTimerLeave: "GVRP leave timer must be greater than twice the join time and less than the leave all time."
errGvrpTimerLeaveall: "GVRP leave timer must be greater than the leave time."
	<!--GVRP err end-->
	<!--stp err start-->
errstpHello: "Invalid STP parameter for hello-time",
errstpforward: "Invalid STP parameter for forward delay",
errstpage: "Invalid STP parameter for maximum aging time",
errstptimer: "Incorrect STP timer value. Timers should conform to the following formula:\n 2 * (forward delay - 1) >= maximum age, and maximum age >= 2 * (hello time + 1)",
errstpbridge: "Invalid STP parameter for bridge priority, must be a multiple of 4096",
errstpPortpri: "Invalid STP parameter for port priority, must be a multiple of 16",
errstpCst: "CST deletion is not allowed",
errstpCstVlan: "VLAN deletion from CST is not allowed",
errstpMstpLldp: "Enabling LLDP PDU flooding prevents MSTP from being enabled",
errstpshort: "The current pathcost method is short. The maximum effective cost value is 65535."
<!--stp err end-->
<!--lacp err start-->
errLacpPri: "Invalid LACP system priority."
errLacpActive: "Invalid LACP active port."
errLacpNotMember: "Some ports are not LACP member ports."
errLacpStatic: "Unable to add LACP trunk members to a static aggregation group."
<!--lacp err end>
<!--port security err start-->
errSecurityLess: "The maximum value is less than the number of currently protected MAC addresses."
errSecurityMAC: "The secure MAC address does not exist."
errSecurityMACFull: "The maximum number of secure MAC addresses on the port has been reached."
errSecuritTrunk: "The port belongs to a LAG."
errSecurityType: "Invalid port security MAC address type",
errSecurityAzero: "Security MAC address cannot be an all-zero MAC address",
errSecuritySYSMAC: "Security MAC address cannot be a system MAC address",
errSecurityMCAST: "Security MAC address cannot be a multicast MAC address",
errSecurityMACConfit: "A MAC entry exists in the port security table",
errSecurityForbid: "Cannot add a static address on the port because port security is enabled",
errSecurityStaticMac: "The port has a static address",
<!--port security err end-->
<!--user acl err start-->
errACLEmpty: "ACE does not exist",
errACLExists: "ACE already exists.",
errACLPri: "MIB index out of range. Index must be greater than 0 and an existing ifindex.",
errACLlistEmpty: "ACL does not exist",
errACLlistexists: "Another ACL of the same type already exists."
errACLlistbinding: "ACL list binding."
errACLemptybind: "Empty ACL cannot be attached to a class map or interface."
errACLillegalEmpty: "Illegal ACL type."
errACLinsuff: "Insufficient PIE entries."
errACDscp: "Invalid DSCP value, DSCP value should be 0-63."
errACLTCPFlag: "Illegal TCP flag character."
errACLPrecedence: "Invalid IP-Precedence value, IP-Precedence value should be 0-7."
errACLrange: "Invalid port range value."
errACLPortType: "Cannot apply ACL because another ACL of the same type is already applied."
errACLPortAcl: "Cannot apply ACL because another ACL is already applied."
errACLPortTRUNK: "Port channel member ports are not allowed to bind/unbind ACLs."
errACLEntryFULl: "Cannot create a new entry in the ACL table because the table is full."
errACLEntryPOOLFULL: "Cannot create a new ACE entry because the table is full."
<!--user acl err end-->
<!--igmp snooping err start -->
errIgmpPort: "Multicast port deleted successfully."
errIgmpAddr: "Invalid multicast address, reserved."
errIgmpgroup: "Invalid IGMP maximum group operation."
errIgmpQuerierChange: "The querier is not self, cannot modify."
errIgmpQuerierVersion: "The querier version cannot be greater than the IGMP version."
errIgmpQuery: "Invalid IGMP query interval."
errIgmpResponse: "Invalid IGMP response time."
errIgmpRouter: "Invalid IGMP router timeout."
errIgmpINTVl: "Invalid IGMP Last Member Query Interval",
errIgmpVar: "Invalid IGMP Robustness Variable",
errIgmpstaticEntry: "IGMP static group exists",
errIgmpGroupEntry: "IGMP group entry does not exist",
errIgmpRouterPort: "Invalid unknown multicast operation. When multicast snooping is disabled, the operating router port cannot be set",
errMCASTGroupType: "Incorrect multicast group type",
errMCASTADDR: "Incorrect multicast address",
errMCASTIPType: "Incorrect multicast IP type",
errMCASTVlan: "Multicast VLAN does not exist",
errMCASTDATA: "Multicast group database is full",
errIgmpProID: "IGMP profile ID not allowed",
errIgmpProType: "Incorrect IGMP profile type",
errIgmpProUsed: "IGMP Profile ID is in use and cannot be deleted or modified",
errIgmpProNotExist: "IGMP profile does not exist",
errIgmpProEmpty: "IGMP profile is empty",
errIgmpProIPType: "IP type of multicast profile does not match",
errMVRenable: "MVR is disabled, MVR settings cannot be configured",
errMVRType: "MVR wrong port type",
errMVRPortMode: "MVR port type cannot be configured for this VLAN port mode",
errMVRVlanPortType: "VLAN membership cannot be configured with MVR port type (source port must be in MVR VLAN, receiver port cannot be MVR VLAN)",
errMVRVlanPortSource: "MVR source port must be in MVR VLAN",
errMVRVlanPortReceiver: "MVR receiver port cannot be a member of MVR VLAN",
errMVRStaticGroupConflict: "MVR static group conflicts with MVR group range",
errMVRStaticGroupPortType: "MVR static group port member, in compatibility mode, applies only to receiver ports. In dynamic mode, it applies to both receiver ports and source ports."
errMVRVlanStaticSet: "MVR static group VLAN ID is not an MVR VLAN ID."
errMVRigmpStaticSet: "IGMP Snooping static group cannot use MVR."
errStaticGroup: "MVR is enabled, and the static group VLAN is an MVR VLAN."
<!--igmp snooping err end-->
<!--dhcp server err start -->
errLMT: "Lease time cannot exceed 999 days."
errLTE: "Lease time cannot be 0."
errLTER: "Time input error."
errDSSI: "Start address is invalid."
errDEI: "End address is invalid."
errDSSH: "Start address cannot be greater than end address."
errDSLT: "A maximum of 1024 addresses can be configured in an address segment",
errDSO: "Address conflict with other address segments",
errDSNF: "The index of the address segment cannot exceed 256",
errDSIN: "The address and gateway are not in the same subnet",
errDSNG: "No gateway is configured",
errDSNCG: "The gateway address should not be included in the address segment",
errDSII: "This IP address is not in the address pool and cannot be allocated",
errDSIM: "An IP address is in use and the current address pool cannot be modified",
<!--dhcp server err end -->
errDNSR: "Exceeded the maximum number of DNS servers (4)",
errEntryFull: "The database is full",
<!--igmp snooping err end-->
<!--vlan mac group err start -->
errMacVlanId: "The group index already exists",
errInvalidMacAddress: "Invalid MAC address",
<!--vlan mac group err end-->
<!--mac staticaddress err start -->
errMacFull: "MAC table is full",
errStaticMacZero: "The static MAC address table entry cannot be all zeros",
errMacStaticTable: "The MAC address already exists in the static table",
<!--mac staticaddress err end-->
<!--system err start -->
errFailed: "Failed",
errSuccess: "Success",
errActionNoAllowed: "Action not allowed",
errActionNoFinished: "Action not finished",
errBufferSizeError: "Buffer size error",
errChipNoSupported: "Chip not supported",
errInvalidDropPrecedence: "Invalid drop priority",
errInvalidDscpId: "Invalid DSCP index",
errEntryAlreadyExist: "The entry already exists",
errNameAlreadyExist: "The name already exists",
errInvalidEntryIndex: "Invalid entry index",
errSEntryNoExist: "The specified entry does not exist",
errEntryIsExist: "The interface already exists",
errDatabaseIsFull: "The database is full",
errInvalidEtherType: "Invalid Ethernet type",
errFiberPortNoSetSD: "Speed/duplex cannot be configured for fiber-only ports",
errFiberOnlySupport: "Fiber only supports 1 Gigabit speed",
errGatewayNoFound: "Gateway not found",
errInputParameterErr: "Input parameter error",
errInvalidIPAddr: "Invalid IP address",
errInvalidIPMask: "Invalid IPv4 address mask",
errIPAndGWNoInSameSub: "IPv4 address and gateway are not in the same subnet",
errInvalidIPv6Addr: "Invalid IPv6 address",
errIPv6AddrAndGWMust: "IPv6 address and gateway must be in the same subnet",
errInvalidHostName: "Invalid host name",
errInvalidMacAddr: "Invalid MAC address",
errSpecifiedValueOut: "Specified value is out of range",
errSpecifiedInvalid: "Specified range is invalid",
errInvalidPid: "Invalid port id",
errInvlaidPri: "Invalid priority",
errInvalidQueueId: "Invalid queue id",
errSNTPUpFail: "SNTP update failed",
errInvlaidStr: "Invalid string",
errTimeErr: "Error! Start time is greater than end time"
errSystemNameInvalid: "The image extension is invalid",
errSystemAllowIErr: "The image is being processed. Processing other images is not allowed",
errDnsFailed: "DNS resolution failed. Please check the DNS server settings or host name",
errUserNumMaxErr: "The maximum number of supported users has been exceeded",
errUserNoExist: "The user does not exist",
errSpecifyUserName: "Please specify a username",
errDefaultUserErr: "Unable to delete the default username",
errCurrentUserErr: "Unable to delete the current username",
errUseNaINvalidErr: "The username is invalid",
errPasswordInErr: "The password is invalid",
errUseNaLengthErr: "The username length is invalid",
errPasswordLengthErr: "The password length is invalid",
<!--system err end-->
<!--acl err start -->
errAuthLiFull: "Authentication list is full",
errAuthLiExist: "Authentication list does not exist",
errAuthLiName: "Please specify an authentication list name",
errAuthType: "Authentication type is invalid",
errACLPhInvalid: "ACL Phase is invalid",
errACLTypeIsInvalid: "ACL block aggregator type is invalid",
errACLBlUsedPr: "ACL block is used by protocol",
errACLBlInInvalid: "ACL block index is invalid",
errACLBlACLFull: "ACL block is full",
errACLTempIsFulle: "Template is full",
errACLTempIdInvalid: "Template id is invalid",
errACLFieldNOExist: "Field does not exist in template",
errACLRateRnIdErr: "Rate entry id is invalid",
errACLPoleFull: "Policer entry is full",
errACLVlanEnOccupie: "All VLAN range entries are occupied."
errACLEnRangeFull: "Cannot create a new entry in the port range table because the port range table is full."
errACLLogTableFull: "The log table is full."
errACEEntryTaFull: "Cannot create a new ACE entry in the ACL table because the table is full."
<!--acl err end-->
<!--isg err start -->
errIsgEntryReached: "The maximum number of entries for the port has been reached."
errIsgBindingNumber: "The maximum value cannot be less than the current number of bindings."
errIsgDataFull: "IP Source Guard database is full."
errIsgIpSourceEx: "IP Source Guard table entry exists."
errIsgMacAddr: "MAC address cannot be a multicast address."
<!--isg err end-->
<!--mac err start -->
errMacEntryCl: "Mac entry clear error type",
errMacEntryNoExist: "Mac entry does not exist",
errMacEntryExist: "Mac entry already exists",
errMacInvalidSet: "Invalid unicast port setting",
errMacInvalidUnAddr: "Invalid unicast MAC address",
errStaticMacAddr: "Static entry cannot be a system MAC address",
errStaticMulPort: "Static entry cannot be a multicast MAC address",
errStaticNoTrunkMem: "Static ingress port cannot be a trunk port",
errStaticNoEmPort: "Static ingress port cannot be an empty trunk port",
errDataIsBusy: "MAC address database busy. Please try again later",
errBlackEntryEx: "Mac entry exists in the filter table",
errBlackZebroAddr: "Filter entry cannot be all zero MAC addresses",
errBlackSysMac: "Filter entries cannot be system MAC addresses."
errBlackEntrMuMac: "Filter entries cannot be multicast MAC addresses."
	<!--mac err end-->
	<!--mirror err start -->
errMirrorNoSession: "Invalid mirror group ID",
errMirrorDstPort: "A port cannot be the destination port of multiple mirror groups",
errMirrorDstNoExist: "Mirror destination port does not exist",
errMirrorPortErr: "Mirror destination port error",
errMirrorSourNoList: "Mirror group destination port cannot appear in the source port list",
errMirrorDstMiPor: "Mirror destination port cannot be a mirror port",
errMirrorTrunkPort: "Mirror destination port cannot be a trunk port",
errMirrorMemPort: "Mirror destination port cannot be a trunk member port",
errMirrorGvrpEnable: "GVRP is enabled on the mirror destination port",
errMirrorEnableState: "Mirror group port is not enabled",
<!--mirror err end-->
<!--l3 err start -->
errL3InterfaceInval: "Invalid interface type",
errL3DatabaseFull: "Database full",
errL3EntryNodata: "No entry found in the database",
errL3NumInterNum: "Maximum number of interfaces exceeded",
errL3NoInterfaceId: "Invalid loopback interface ID",
errL3IpNoExist: "Interface IP address does not exist",
errL3IpPriNoExist: "Primary IP address does not exist. Please configure a primary IP address first",
errL3IpIsExist: "IP address already exists",
errL3IpPriIsExist: "IP address already exists. This address is the current primary IP address",
errL3IpSubIsExist: "Error: Please delete the sub-IP address first",
errL3IpIsPri: "Error: Cannot delete. This IP address is the primary IP address",
errL3IpIsSub: "Error: Cannot delete. This IP address is a sub-IP address",
errL3IpNumInterface: "The interface has more IP addresses than the maximum."
errL3RouteInterface: "Not an L3 routing interface."
errL3NoIpv6InterSet: "Cannot disable IPv6 processing on an interface configured with an explicit IPv6 address."
errL3SameSubnet: "Duplicate IP on the same subnet."
errL3IpTheSubnet: "Duplicate IP on the subnet."
errL3InvaliIpAddr: "Invalid IP address."
errL3InvalidIpMaskErr: "Invalid IP address mask."
errL3Ipv6NoValid: "Invalid IPv6 prefix."
errL3GWAddrNoDevi: "Gateway cannot be one of the addresses configured on this device."
errL3RouterDefGW: "Only one directly connected router can be defined as a gateway."
errL3GWDHCPInter: "Gateway cannot be on a DHCP-enabled interface."
errL3GWSameVlan: "The gateway cannot be the same subnet as the management VLAN."
errL3RouteViaInter: "IP static routes cannot be defined through a loopback interface."
errL3HopsNetExceed: "The maximum number of configured down-hops for this network has been exceeded."
errL3StaticFull: "The static route database is full."
errL3StaticExist: "The static route does not exist."
errL3StaticMustAddr: "The ARP MAC address must be a unicast address."
errL3MacAddrNoErr: "The MAC address cannot be the same as the device MAC."
errL3ArpNoMatch: "The ARP interface does not match the IP interface port."
errL3ArpNwDir: "The ARP IP must be from a directly connected network."
errL3ArpDefDevice: "The ARP IP cannot be an IP interface defined on this device."
errL3NoSuchExist: "No such instance exists."
errL3SetARPOverBy: "Setting ARP static configuration on an IP interface obtained by DHCP is not allowed."
errL3Ipv6NoEnableInter: "IPv6 is not enabled on the interface."
errL3NeighNoCreate: "Unable to create neighbor entry - no subnet defined on the device."
errL3VlanRelay: "The VLAN cannot be deleted because an IP interface or DHCP relay is defined on the VLAN."
	<!--l3 err end-->
	<!--igmp err start -->
errIgmpProGroup: "Invalid MLD max-group operation",
errIgmpProAllow: "MLD profile ID not allowed",
errIgmpProMldMod: "Using MLD profile ID, cannot be deleted or modified",
errIgmpProMldExist: "MLD profile does not exist",
errIgmpProMldZero: "MLD profile is empty",
errIgmpProMulMaximum: "Multicast port group maximum reached",
<!--igmp err end-->
<!--qoss err start -->
errQosMode: "Not currently in QoS mode",
errQosNoBasic: "Not currently in QoS basic mode",
errQosPriVal: "Invalid priority value",
errQosInvalidVal: "Invalid queue weight value",
errQosInvalidCos: "Invalid cos value",
errQosInvalidDscp: "Invalid DSCP value",
errQosPriVal: "Invalid IP precedence value",
errQosInvalQueId: "Invalid queue id",
errQosRemarkCon: "DSCP and IP precedence marking states conflict",
<!--qos err end-->
<!--rate err start -->
errRateRange: "Invalid: Out of range",
errRateInvalidMul: "Invalid rate: Must be a multiple of 16",
errRateMultiBur: "Invalid: Must be a multiple of 128",
errRateBurMulti: "Invalid: Must be a multiple of 1024",
<!--rate err end-->
<!--snmp err start -->
errSnmpInvalStr: "Invalid snmp string",
errSnmpTableFull: "Trap host table is full",
errSnmpConmmIsFull: "Community table is full",
errSnmpSecLelErr: "Wrong security level",
errSnmpNotiErr: "Wrong notification type",
errSnmpNotiMatch: "Notification type inform does not match version 1 (default version is 1)",
errSnmpNameCommErr: "Wrong SNMP community name",
errSnmpV3ViewFul: "View table is full",
errSnmpV3DefaultView: "View name is the default view, cannot be added or deleted",
errSnmpV3SpecialName: "Special view name, cannot be added",
errSnmpV3SecurityFull: "Security group table is full",
errSnmpV3AccessFull: "Access group table is full",
errSnmpV3GroupName: "SNMP group name is wrong",
errSnmpV3TableFull: "User group table is full",
errSnmpV3RemoteTabl: "Remote engine table is full",
errSnmpV3DefaultViNa: "Invalid id",
errSnmpV3UseNaVi: "Using the default view name is invalid",
errSnmpV3InvalOid: "Invalid OID length",
errSnmpV3NoOidMask: "Invalid OID mask",
errSnmpV3InvalidMaskLen: "Invalid OID mask length",
errSnmpV3CommunFound: "Community word not found",
errSnmpV3GrouExist: "Group exists",
errSnmpV3SecMoMath: "Group does not exist or security mode does not match",
errSnmpV3AcceCon: "Access group exists, but view conflicts",
errSnmpV3ReadExis: "Read view does not exist",
errSnmpV3MustAdd: "Read attribute view must be added",
errSnmpV3WritExi: "Write view does not exist",
errSnmpV3NotViNotEx: "Notification view does not exist",
errSnmpV3AcessGrCom: "List group is used by a group or user",
errSnmpV3EngForErr: "Incorrect id format",
errSnmpV3EngHexad: "id must contain 10 to 64 hexadecimal digits and must be divided by 2",
errSnmpV3UsNameNoFind: "Username not found",
errSnmpV3UserPLErr: "Incorrect username",
errSnmpV3PassWokErr: "Incorrect password",
errSnmpV3AuthProNo: "No authentication protocol",
errSnmpV3EncryProhf: "No encryption protocol."
<!--snmp err end-->
<!--mstp err start -->
errMstpActiFldWhe: "The LLDP flooding action cannot be configured because MSTP is running."
errMstpInterMul: "The LLDP transmission interval should be less than 65535."
errMstpDelayThan: "The LLDP transmission delay should not be greater than the LLDP transmission interval * 0.25."
errMstpNetExi: "The network policy does not exist."
errMstpTypeDup: "The network policy application type is duplicated."
errMstpBindInter: "The network policy bound to the interface cannot be modified or deleted."
errMstpNetConBec: "The voice-type network policy cannot be configured because the voice VLAN is in automatic update mode."
errMstpVoiceAu: "Voice VLAN cannot be in auto-update mode because a voice-type network policy exists."
errMstpParisHex: "Invalid input. Input should be in hexadecimal without ":", for example: 1122AB."
	<!--mstp err end-->
	<!--radius err start -->
errRadiusSerErr: "Radius server error",
errRadiusSerTheNo: "No radius server",
errRadiusOverSerNum: "Exceeded the maximum number of radius servers",
errRadiusSerAlEx: "Radius server already exists",
errRadiusSetUniqueVal: "Need to set a unique priority value",
errRadiusAuthCatErr: "Authentication error",
<!--radius err end-->
<!--syslogs err start -->
errSyslogSerIp: "Log server IP address error",
errSyslogRemoFul: "Remote host is full",
errSyslogNoExist: "Syslog remote host exists",
errSyslogHostNo: "Syslog remote host does not exist",
errSyslogInputSev: "System log input error",
errSyslogCateErr: "System log input category error",
<!--syslog err end-->
<!--tacacs err start -->
errTacSerErr: "tacacs+ server error",
errTacNoSer: "No tacacs+ server",
errTacMaxSerNum: "Exceeded the maximum number of tacacs+ servers",
errTacSerAlEx: "Tacacs+ server already exists",
errTacSetPriVal: "Need to set a unique priority value",
<!-- tacacs err end-->
<!--ospf err start -->
errOSPFIsClose: "Error: Please enable OSPF function first",
<!--ospf err end-->
<!--rip err start -->
errRipIsClose: "Error: Please enable RIP function first",
<!--rip err end-->
<!--vrrp err start -->
errVrrpSubif: "Virtual IP is not in the same subnet",
errVrrpMacsrv: "The maximum number of virtual routers has been reached",
errVrrpMacvip: "The maximum number of virtual IPs has been reached"
<!--vrrp err end-->
},
lang_warning:{
warningAclNotMod: "An activated ACL cannot be modified."
warningAclunderBind: "Unable to delete a bound management ACL."
warningAclName: "Name $1 already exists."
warningAclsequence: "Sequence already exists."
warningSelectEntry: "Please select only one entry to view."
warningLoopback: "The loopback interface cannot be edited."
warningAcldelate: "Unable to delete an active management ACL."
warningSelectActive: "Please select only one entry to activate."
warningSelectDeactive: "Please select only one entry to deactivate."
warningPriExist: "Priority already exists."
warningSelectEdit: "Please select only one entry to edit."
warningProtocolGroup: "Protocol group settings are the same as group ID $1."
warningVlanName: "Name already used by VLAN $1."
warningTrunkPort: "Trunk ports must be assigned a native VLAN."
warningAccessPort: "Access ports must be assigned an access VLAN."
warningHybridPort: "Hybrid ports must be assigned a PVID."
warningOnlyUntag: "Only an untagged VLAN can be selected."
warningOnlyPVID: "Only a PVID can be selected."
warningIpSubnetGroup: "The IP subnet group already exists."
},

<!--#ifdef CONFIG_SYS_DHCP_SERVER-->
/* DHCP */
lang_dr:{
lblDrStatus: "Status",
titDrPortSettingTable: "DHCP Port Configuration Table",
titDrEditPortSetting: "Modify Port Configuration",
lblDrEntryNo: "Number",
lblDrPort: "Port",
lblDrPortEnable: "Management Status",
lblIPPoolTable: "IP Address Pool Table",
lblPoolName: "Address Pool Name",
txtPoolNamelen: "(1 to 32 alphanumeric characters)",
lblPoolSection: "Address Segment Index",
lblPoolAddrStart: "Start Address",
lblPoolAddrEnd: "End Address",
lblGateway: "Gateway",
lblGatewayMask: "Mask",
lblBindStatus: "Static Binding Priority",
lblDNSPrimaryIp: "Primary DNS Server",
lblDNSSecondIp: "Second DNS Server",
lblOption43: "option 43",
lblOption43Str:"Address",
lblOption43Type:"Format",
txtAscii:"ascii",
txtHex:"hex",
lblLeaseTime:"Lease Time",
lblIPSectionTable:"IP Address Range",
lblVlanIfPoolTable:"VLAN Interface Address Pool Table",
lblDrGroupTable:"DHCP Server Group Table",
lblDrVlanifGroup:"DHCP Server Group",
lblDrGroupId:"Server Group ID",
lblDrGroupIp:"Server Group IP Address",
lblGroupBindVlanIf:"Interface Binding",
lblDrclients:"Client List",
lblDrVlanid:"VLAN",
lblDrUserName:"User Name",
lblDrClientStatic:"Static Address Binding Table",
lblDrClientStaticEdit:"Modify Static Address Binding Table",
lblDrClientStaticAdd:"Add Static Address Binding Table"
},
<!--#endif-->
<!--#ifdef CONFIG_SYS_L3_ROUTE_RIP-->
/* Rip */
lang_rip:{
lblRipStatus: "Rip routing status",
lblRipNetworkTable: "Working network configuration table",
lblRipSettingInfo: "Rip routing configuration information",
lblRipNetworkAddr: "Working network IPv4 address",
lblRipNetworkMask: "Working network mask"
},
<!--#endif-->
<!--#ifdef CONFIG_SYS_L3_ROUTE_OSPF-->
/* Ospf */
lang_ospf:{
lblOspfStatus: "OSPF routing status",
lblOspfAreaNetworkTable: "Area network configuration table",
lblOspfSettingInfo: "OSPF Routing configuration information",
lblOspfNetworkAddr:"Working network IPv4 address",
lblOspfNetworkMask:"Working network mask",
lblOspfArea:"Area identifier"
},
<!--#endif-->
<!--#ifdef CONFIG_SYS_L3_VRRP-->
/* vrrp */
lang_vrrp:{
titAddIPv4Intf:"Add VRRP interface",
txtInit:"Initial",
txtBackup:"Backup",
txtMaster:"Active",
lblVrrpIntfTable:"VRRP interface configuration table",
lblVrid:"VRRP identifier",
lblVifaddr:"Virtual address",
lblVifState:"State",
lblPri:"Priority",
lblAdver:"Advertisement interval",
lblPreempt:"Preemption mode",
lblDelay:"Preemption delay"
},
<!--#endif-->

lang_zzz:{
  dummy:""
}
};

var defLang_en = {
<!--#ifdef CONFIG_SYS_AAA-->
/* AAA */
lang_aaa: {
  titLoginLists:"Method List Table",
  lblListName:"Name",
  lblMethodList:"Sequence",
  errDfltListDel:"default list is not able to be deleted",
  lblMethodEmpty:"Empty",
  lblMethodNone:"None",
  lblMethodLocal:"Local",
  lblMethodEnable:"Enable",
  lblMethodRadius:"RADIUS",
  lblMethodTacplus:"TACACS+",

  titAddLoginList:"Add Method List",
  titEditLoginList:"Edit Method List",
  lblMethod:"Method $1"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_USER_DEFINED_ACL-->
/* ACL */
lang_acl: {
  titAclTable:"ACL Table",
  titAceTable:"ACE Table",
  titAclACEAdd:"Add ACE",
  titAclACEEdit:"Edit ACE",
  titAclBindingTable:"ACL Binding Table",
  titAclBindingAdd:"Add ACL Binding",
  titAclBindingEdit:"Edit ACL Binding",
  msgAclBindingPorts:"Note: Rules cannot be edited after ACL is bound to a port",
  msgAclEmptyACL:"Note: ACL without any rules cannot be bound",

  lblAclName:"ACL Name",
  lblAclAceCnts:"Rule",
  lblAclBindingPorts:"Port",
  lblAclSequence:"Sequence",
  lblAclAction:"Action",
  lblAclPermit:"Permit",
  lblAclDeny:"Deny",
  lblAclShutdown:"Shutdown",
  lblAclSource:"Source",
  lblAclDestination:"Destination",
  lblAclMac:"MAC ACL",
  lblAclIp:"IP",
  lblAclIpv4:"IPv4 ACL",
  lblAclIpv6:"IPv6 ACL",
  lblAclPort:"Port",
  lblAclAddress:"Address",
  lblAclMask:"Mask",
  lblAclPrefix:"Prefix",
  lblAcl8021p:"802.1p",
  lblAclValue:"Value",
  lblAclEth:"Ethertype",
  lblAclProto:"Protocol",
  lblAclSingle:"Single",
  lblAclRange:"Range",
  lblAclTcpFlag:"TCP Flags",
  lblAclSet:"Set",
  lblAclUnset:"Unset",
  lblAclNone:"Don't care",
  lblAclToS:"Type of Service",
  lblAclDSCP:"DSCP",
  lblAclIPP:"IP Precedence",
  lblAclTcpFlag:"TCP Flags",
  lblAclICMP:"ICMP",
  lblAclType:"Type",
  lblAclCode:"Code",
  lblAclAny:"Any",
  lblAclSelctList:"Select",
  lblAclIpputValue:"Define",
  errPortRange:"End port cannot small than or equal to start port."
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_MGMT_ACL-->
/* ACL - Manageement*/
lang_acl_mgmt: {
  titMgmtACLTable:"Management ACL Table",
  titMgmtACETable:"Management ACE Table",
  titMgmtACEAdd:"Add Managemet ACE",
  titMgmtACEEdit:"Edit Managemet ACE",
  lblMgmtACLName:"ACL Name",
  lblMgmtACENUM:"Rule",
  lblMgmtACLActive:"State",
  lblMgmtACLPri:"Priority",
  lblMgmtACLAction:"Action",
  lblMgmtACLService:"Service",
  lblMgmtACLSrcIp:"Address / Mask",
  lblMgmtACLPermit:"Permit",
  lblMgmtACLDeny:"Deny",
  lblMgmtACLAll:"All",
  lblMgmtACLHTTP:"Http",
  lblMgmtACLHTTPS:"Https",
  lblMgmtACLSNMP:"Snmp",
  lblMgmtACLSSH:"SSH",
  lblMgmtACLTelnet:"Telnet"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_AUTHMGR-->
lang_authmgr: {
  lblAuthType:"Authentication Type",
  lblDot1x:"802.1x",
  lblMac:"MAC-Based",
  lblWeb:"WEB-Based",
  lblGuestVlan:"Guest VLAN",
  lblMacRadiusUser:"MAC-Based User ID Format",

  titAuthmgrPortModeTable:"Port Mode Table",
  titAuthmgrEditPortMode:"Edit Port Mode",
  titAuthmgrPortSettingTable:"Port Setting Table",
  titAuthmgrEditPortSetting:"Edit Port Setting",

  lblPortCtrl:"Port Control",
  lblPortCtrlDisabled:"Disabled",
  lblPortCtrlAuto:"Auto",
  lblPortCtrlForceAuth:"Force Authorized",
  lblPortCtrlForceUnauth:"Force Unauthorized",
  lblHostMode:"Host Mode",
  lblHostModeMultiAuth:"Multiple Authentication",
  lblHostModeMultiHost:"Multiple Hosts",
  lblHostModeSingleHost:"Single Host",
  lblOrder:"Order",
  lblAvailType:"Available Type",
  lblSelType:"Select Type",
  lblMethod:"Method",
  lblAvailMethod:"Available Method",
  lblSelMethod:"Select Method",
  lblMethodRadius:"RADIUS",
  lblMethodLocal:"Local",
  lblReauth:"Reauthentication",
  lblMaxHost:"Max Hosts",
  lblVlanAssign:"VLAN Assign Mode",
  lblVlanAssignDisable:"Disable",
  lblVlanAssignReject:"Reject",
  lblVlanAssignStatic:"Static",
  lblCommonTimer:"Common Timer",
  lblTimerReauth:"Reauthentication",
  lblTimerInact:"Inactive",
  lblTimerQuiet:"Quiet",
  lblDot1xParam:"802.1x Parameters",
  lblMaxReq:"Max Request",
  lblTxPeriod:"TX Period",
  lblSuppTimeout:"Supplicant Timeout",
  lblSrvTimeout:"Server Timeout",
  lblWebParam:"Web-Based Parameters",
  lblLoginAttempt:"Max Login",
  lblInfinite:"Infinite",

  errTypeOrderEmpty:"Must select at least one type for authentication order",
  errTypeOrderWebNotLast:"Web-Based type must be the last one of authentication order",
  errMethodEmpty:"Must select at least one method for authentication",

  titMacLocalAcctTable:"MAC-Based Local Account Table",
  titAddMacLocalAcct:"Add MAC-Based Local Account",
  titEditMacLocalAcct:"Edit MAC-Based Local Account",
  titWebLocalAcctTable:"WEB-Based Local Account Table",
  titAddWebLocalAcct:"Add WEB-Based Local Account",
  titEditWebLocalAcct:"Edit WEB-Based Local Account",
  lblAssignTimer:"Assigned Timer",
  lblCtrl:"Control",
  lblTimeoutSec:"Timeout (Sec)",
  lblMacAddr:"MAC Address",
  lblAssignVlan:"VLAN",

  lblUserName:"Username",
  lblPwd:"Password",
  lblPwdConfirm:"Confirm Password",

  titSessTable:"Sessions Table",
  lblCurrType:"Current Type",
  lblStatus:"Status",
  lblStsRunning:"Running",
  lblStsAuthorized:"Authorized",
  lblStsUnAuthorized:"UnAuthorized",
  lblStsLocked:"Locked",
  lblStsGuest:"Guest",
  lblAuhotrInfo:"Authorized Information",
  lblOperInfo:"Operational Information",
  lblSessId:"Session ID",
  lblReauthPeriod:"Reauthentication<br>Period",
  lblInactTimeout:"Inactive<br>Timeout",
  lblSessTime:"Session<br>Time",
  lblInactedTime:"Inactived<br>Time",
  lblQuietTime:"Quiet<br>Time"
},
<!--#endif-->

/* Common Message */
lang_common: {
  txtEnable:"Enable",
  txtDisable:"Disable",
  txtEnabled:"Enabled",
  txtDisabled:"Disabled",
  txtHexRangeArg:"($1 - $2 Hexadecimal Characters)",
  txtDefaultArg:"(Default: $1)",
  txtRangeArg:"($1 - $2)",
  txtRangeArgDefault:"($1 - $2, default $3)",
  txtSecRangeArg:"Sec ($1 - $2)",
  txtMinRangeArg:"Min ($1 - $2)",
  txtByteRangeArg:"Byte ($1 - $2)",
  txtMwRangeArg:"mW ($1 - $2)",
  txtKbpsRangeArg:"Kbps ($1 - $2)",
  txtUnitRangeArg:"$1 ($2 - $3)",
  txtMSRangeArgDefault:"cs ($1 - $2, default $3)",
  txtSecRangeArgDefault:"Sec ($1 - $2, default $3)",
  txtMinRangeArgDefault:"Min ($1 - $2, default $3)",
  txtByteRangeArgDefault:"Byte ($1 - $2, default $3)",
  txtUnitRangeArgDefault:"$1 ($2 - $3, default $4)",
  txtAll:"All",
  txtAuto:"Auto",
  txtArg:"$1",
  btnApply:"Apply",
  btnCancel:"Cancel",
  btnClose:"Close",
  btnAdd:"Add",
  btnEdit:"Edit",
  btnDelete:"Delete",
  tipOnlyOneCanbeEdited:"Only a single entry can be processed at a time.",
  tipNoEntrySelected:"No entry selected.",
  btnDetail:"Detail",
  btnRefresh:"Refresh",
  btnPreview:"Preview",
  btnClear:"Clear",
  btnBack:"Back",
  btnOK:"OK",
  btnDone:"Done",
  btnActive:"Active",
  btnDeactive:"Deactive",
  btnBind:"Bind",
  btnUnbind:"Unbind",
  tipProceed:"Proceed",
  titEditIntfSettings:"Edit Interface Settings",
  tipButtonTransAdd:"Select an item to transfer",
  tipButtonTransRem:"Select an item to transfer",
  lblEdit:"Edit",
  lblNA:"N/A",
  lblYes:"Yes",
  lblNo:"No",
  lblSave:"Save",
  lblAlert:"Alert",
  lblIntfType:"Interface Type",
  lblEqualTo:"equals to",
  msgNotifySave:"Success.",
  msgSuccess:"Success.",
  lblOperationalStatus:"Operational Status",
  lblMbrPorts:"Member",
  lblPort:"Port",
  lblPortList:"Port List",
  lblAvailablePort:"Available Port",
  lblSelPort:"Selected Port",
  lblVLAN:"VLAN",
  lblVlanList:"VLAN List",
  lblAvailableVlan:"Available VLAN",
  lblSelVlan:"Selected VLAN",
  lblIntfSettingTable:"Interface Settings Table",
  lblNoResultFound:"0 results found.",
  lblName:"Name",
  lblRange:"Range",
  lblPageLeftCombo:"Showing",
  lblPageRightCombo:"entries",
  lblIntf:"Interface",
  lblLAG:"LAG",
  lblRefreshRate:"Refresh Rate",
  lblRefreshRateSec:"sec",
  lblRefreshRateNone:"None",
  lblRefreshRate5s:"5 sec",
  lblRefreshRate10s:"10 sec",
  lblRefreshRate30s:"30 sec",
  lblUseDefault:"Use Default",
  lblUserDefined:"User Defined",
  lblSrvAddrType:"Address Type",
  lblByIPAddress:"By IP Address",
  lblByName:"By Name",
  lblHostname:"Hostname",
  lblIPv4:"IPv4",
  lblIPv6:"IPv6",
  lblIPVersion:"IP Version",
  lblSrvIPHost:"Server Address",
  lblSrvPort:"Server Port",
  lblForward:"Forward",
  lblRateLimit:"Rate Limit",
  lblUnLimited:"Unlimited",
  lblHintUnLimited:"0 is Unlimited",
  lblActionForward:"Forward",
  lblActionTrap:"Trap",
  lblActionLog:"Log",
  lblActionDiscard:"Discard",
  lblActionShutdown:"Shutdown",
  lblState:"State",
  btnAddSection:"Add Section"
},

<!--#ifdef CONFIG_SYS_PROTO_DAI-->
/* DAI*/
lang_dai: {
  lblDAIStatus:"State",
  titDaiPortSettingTable:"Port Setting Table",
  titDaiEditPortSetting:"Edit Port Setting",
  titTableDaiStatistic:"Statistics Table",
  lblTrusted:"Trust",
  lblVerifySrcMac:"Source MAC Address",
  lblVerifyDstMac:"Destination MAC Address",
  lblVerifyIP:"IP Address",
  lblIPAllowZero:"Allow Zero",
  lblSMacErr:"Source MAC <br> Failure",
  lblDMacErr:"Destination MAC <br> Failure",
  lblSIPErr:"Source IP <br> Validation Failure",
  lblDIPErr:"Destination IP <br> Validation Failure",
  lblIPMACErr:"IP-MAC <br> Mismatch Failure"
},
<!--#endif-->

lang_diag: {
  btnCopperTest:"Copper Test",

  lblPortSelect:"Port",
  lblCopperTestResult:"Copper Test Result",
  lblCopperTestStatus:"Cable Status",
  lblCopperResult:"Result",
  lblCopperLength:"Length",
  lblCopperNormal:"OK",
  lblCopperShort:"Short Cable",
  lblCopperOpen:"Open Cable",
  lblCopperMisMatch:"Impedance Mismatch",
  lblCopperLineDrive:"Line Drive",
  lblCopperNotTested:"Not Tested",
  lblMeter:"M",

  btnDetail:"Detail",
  txtOpticalIntfStatus:"Fiber Module Status",
  lblOpticalModuleStatusTable:"Fiber Module Table",
  lblTransceiverType:"Transceiver Type",
  lblHotPlug:"Hot Plug",
  lblConnectorType:"Connector Type",
  lblEthComplianceCode:"Ethernet Compliance Code",
  lblTransmissionMedia:"Transmission Media",
  lblWaveLength:"Wavelength",
  lblBitrate:"Bitrate",
  lblVendorOui:"Vendor OUI",
  lblVendorName:"Vendor Name",
  lblVendorPN :"Vendor PN",
  lblVendorRev:"Vendor Revision",
  lblVendorSN:"Vendor SN",
  lblDateCode:"Date Code",
  lblTemperature:"Temperature (C)",
  lblVoltage:"Voltage (V)",
  lblCurrent:"Current (mA)",
  lblOutputPower:"Output Power (mW)",
  lblInputPower:"Input Power (mW)",
  lblLossOfSignal:"Loss of Signal",
  lblOePresent:"OE Present",
  lblNA:"N/A",
  lblNS:"N/S",
  lblInsert:"Insert",
  lblRemove:"Remove",
  lblLoss:"Loss",
  lblNormal:"Normal",

  btnActivePing:"Ping",
  btnStop:"Stop",
  lblPingNum:"Count",
  lblPingResult:"Ping Result",
  lblPingPktStatus:"Packet Status",
  lblPingTx:"Transmit Packet",
  lblPingRx:"Receive Packet",
  lblPingLoss:"Packet Lost",
  lblPingRTT:"Round Trip Time",
  lblPingMinRTT:"Min",
  lblPingMaxRTT:"Max",
  lblPingAvgRTT:"Average",
  lblPingMs:"ms",
  lblPingStatus:"Status",
  lblPingRun:"Ping in progress",
  lblPingTimeout:"Ping failed (timeout)",
  lblPingUH:"Ping failed (unknown host)",
  lblPingAbort:"Ping aborted",
  lblTraceTTL:"Time to Live",
  lblTraceResult:"Traceroute Result",
  txtTraceFailed:"Traceroute failed",
  txtTraceInProgress:"A traceroute operation is currently being performed. Please wait and try again.",
  txtPingInProgress:"A ping operation is currently being performed. Please wait and try again."
},

<!--#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING-->
/* DHCP Snooping */
lang_ds: {
  lblDSStatus:"State",
  titDsPortSettingTable:"Port Setting Table",
  titDs82PortSettingTable:"Port Setting Table",
  titDsEditPortSetting:"Edit Port Setting",
  titDs82EditPortSetting:"Edit Port Setting",
  titDsStatisticTable:"Statistics Table",
  titDs82CIDTable:"Option82 Circuit ID Table",
  titAddOption82CID:"Add Option82 Circuit ID",
  titEditOption82CID:"Edit Option82 Circuit ID",
  lblDsTrusted:"Trust",
  lblDsVerifyMac:"Verify Chaddr",
  lblChaddrErr:"Chaddr Check <br> Drop",
  lblUntrustedErr:"Untrust Port <br> Drop",
  lblUntrustedOption82Err:"Untrust Port <br> with Option82 <br> Drop",
  lblInvalidErr:"Invalid <br> Drop",
  lblRemoteId:"Remote ID",
  lblCircuitIdHint:"(Keep empty to set without VLAN)",
  lblOption82Status:"State",
  lblOption82Action:"Allow Untrust",
  lblKeep:"Keep",
  lblDrop:"Drop",
  lblReplace:"Replace",
  lblCircuitID:"Circuit ID"
},
<!--#endif-->

/* DoS */
<!--#ifdef CONFIG_SYS_DOS_PROTECTION-->
lang_dos: {
  titDosPortSettingTable:"Port Setting Table",
  titEditDosPortSetting:"Edit Port Setting",
  lblByte:"Byte",
  lblNetmaskLen:"Netmask Length",
  lblDmacEqSmac:"DMAC = SMAC",
  lblLand:"Land",
  lblUdpBlat:"UDP Blat",
  lblTcpBlat:"TCP Blat",
  lblPod:"POD",
  lblIpv6MinFragment:"IPv6 Min Fragment",
  lblIcmpFragment:"ICMP Fragment",
  lblIpv4PingMaxSize:"IPv4 Ping Max Size",
  lblIpv6PingMaxSize:"IPv6 Ping Max Size",
  lblPingMaxSizeSetting:"Ping Max Size",
  lblipv4PingMaxSizeEnable:"Enable IPv4",
  lblipv6PingMaxSizeEnable:"Enable IPv6",
  lblSmurfAttack:"Smurf Attack",
  lblTcpMinHdrSize:"TCP Min Hdr size",
  lblTcpSynSport:"TCP-SYN",
  txtTcpSynSport:"Note: Source Port < 1024",
  lblNullScanAttack:"Null Scan Attack",
  lblXMasScanAttack:"X-Mas Scan Attack",
  lblTcpSynFinAttack:"TCP SYN-FIN Attack",
  lblTcpSynRstAttack:"TCP SYN-RST Attack",
  lblTcpFragmentOffset:"TCP Fragment",
  txtTcpFragmentOffset:"Note: Offset = 1",
  lblDosPortState:"State"
},
<!--#endif-->

/* DNS */
<!--#ifdef CONFIG_SYS_ADVANCE_DNS-->
lang_dns: {
  titDnsConf:"DNS Configuration",
  txtDnsStatus:"DNS Status",
  txtDnsDftName:"DNS Default Name",
  txtDnsDftNameLimit:"(<i>1 to 255 alphanumeric characters</i>)",
  titDnsSrvConf:"DNS Server Configuration",
  txtId:"ID",
  txtDnsServer:"DNS Server",
  txtPreference:"Preference",

  titDnsHostConf:"DNS Host Configuration",
  txtHostname:"Host Name (1 to 255 characters)",
  txtIpv4AndIpv6:"IPv4/IPv6 Address",
  txtIpv4:"IPv4 Address",
  titDynamicHostMapping:"Dynamic Host Mapping",
  txtHost:"Host",
  txtTotal:"Total",
  txtElapsed:"Elapsed",
  txtType:"Type"
},

<!--#endif-->

/* Common Error Message */
lang_error: {
  errEmptyValue:"Empty value is invalid.",
  errOutOfRange:"Value is out of range.",
  errOutOfRangeArg:"Value is out of range. Legal range is $1-$2.",
  errStrOutOfRange:"The size of the input string has reached the maximum allowed",
  errInvalidValue:"Not a legal destination value.",
  errInvalidChar:"Value contains illegal characters.",
  errInvalidMacAddr:"Value is an illegal MAC address.",
  errInvalidUnicastMacAddr:"Value is an illegal Unicast MAC address.",
  errInvalidIpAddr:"Value is an illegal IP address.",
  errInvalidIpMask:"Value is an illegal network mask.",
  errInvalidUnicastIpAddr:"Value is an illegal unicast IP Address.",
  errInvalidMulticastIpv4Addr:"Value is an illegal IPv4 Multicast address.",
  errInvalidMulticastIpv4FirstGroup:"Value of first group is out of range. Legal range is 224 - 239",
  errInvalidMulticastIpv6Addr:"Value is an illegal IPv6 Multicast address.",
  errInvalidIpv6GroupNum:"Invalid number of groups for an IPv6 address.",
  errInvalidIpv6Addr:"IPv6 address format is illegal.",
  errInvalidMulticastIpv6FirstGroup:"Value of group 1 should be \"ff00\" or higher.",
  errInvalidDnsName:"Not a valid DNS name. Correct syntax: Label#1.Label#2...Label#n. Each label between $1 and $2 characters long. Each label starts and ends with a letter or a digit.",
  errInvalidVlanRangeArg:"The destination VLAN is not in the currently displayed range. Legal range: [$1]-[$2]",
  errValMustBePairToPairHexChars:"Value must be $1..$2 pairs of hexadecimal characters.",
  errValMustBePairHexChars:"Value must be $1 pairs of hexadecimal characters.",
  errAddEntryFailBecauseTableFull:"Cannot create new entry in the $1 table since the table is full..",
  errEntryAlreadyExists:"Entry already exists.",
  errPortBlng2Trunk:"Port $1 belongs to a LAG.",
  errDNSLookupFailed:'DNS Lookup failed.',
  errDnsResolveError:"DNS resolution failed. Please check DNS server setting or host name.",
  errDNSDomainErr:"Error! Invalid Domain Name. In Domain Name, each substring must be less than 64 characters in length,separated by a dot and the length of whole string must not exceed 255 characters.",
  errDNSServerMax:"Error! The maximum number of DNS server entries has been already reached.",
  errDNSIPaddr:"Error! Invalid IPv4 or IPv6 Address",
  errDNSIPaddrUcast:"Error! Invalid IPv4 or IPv6 Address,Must be a unicast address"
},

/* File Management */
lang_file:{
  lblImage:"Image",
  lblSelImage:"Firmware",
  lblRunningCfg:"Running Configuration",
  lblStartupCfg:"Startup Configuration",
  <!--#ifdef CONFIG_SYS_BACKUP_CONFIGURATION-->
  lblBackupCfg:"Backup Configuration",
  <!--#endif-->
  lblFlashLog:"Flash Log",
  lblBufferLog:"RAM Log",
  lblFileType:"Configuration",
  lblAction:"Action",
  lblUpgrade:"Upgrade",
  lblBackup:"Backup",
  lblTransferMethod:"Method",
  lblTFTP:"TFTP",
  lblHTTP:"HTTP",
  lblBrowseFile:"Filename",
  lblFileName:"Filename",
  lblSrcFileName:"Filename",
  lblDstFileName:"Filename",
  txtBackupOK:"Backup $1 Done",
  txtBackupFail:"Backup $1 Fail",
  txtDownloadInProgress:"Downloading in progress...",
  txtUpgradeOK:"Upgrade $1 Done, the new image will be used until you reboot the system.",
  txtUpgradeFail:"Upgrade $1 Fail",
  txtUploadInProgress:"Uploading in progress...",
  txtUpgradeImgSelActive:"The new image will be used until you set it as the active image and reboot the system.",

  lblActImg:"Active Image",
  lblActImgAfterReboot:"Note: the image was selected for the next boot",
  lblImgActName:"Active Image",
  lblImgBakName:"Backup Image",
  lblImgArg:"Image$1",
  lblImgName:"Name",
  lblImgVersion:"Version",
  lblImgSize:"Size",
  lblImgTime:"Created",
  lblBytesArg:"$1 Bytes",

  lblSrcFile:"Source File",
  lblDstFile:"Destination File"
},

<!--#ifdef CONFIG_SYS_PROTO_GVRP-->
  /* GVRP */
lang_gvrp:{
  lblGvrpGlobalStatus:"State",
  lblTableGvrp:"GVRP Setting Table",
  lblGvrpState:"State",
  lblGvrDynamicVlan:"VLAN Creation",
  lblGvrpRegist:"Registration",
  titGVRPPortTbl:"Port Setting Table",
  lblGvrpState:"State",
  lblGvrpDynamicVlan:"VLAN Creation",
  lblGvrpRegist:"Registration",
  lblNormal:"Normal",
  lblFixed:"Fixed",
  lblForbidden:"Forbidden",
  titEditGVRPPortSetting:"Edit Port Setting",
  lblTimeParm:"Operational Timeout",
  lblJoin:"Join",
  lblLeave:"Leave",
  lblLeaveAll:"LeaveAll",

  titGVRPVlanTable:"Membership Table",
  lblMemberPort:"Member",
  lblDynMemberPort:"Dynamic Member",

  lblcntType:"Statistics",
  lblAll:"All",
  lblRxCnt:"Receive",
  lblTxCnt:"Transmit",
  lblErrCnt:"Error",

  errGvrpGlobal:"GVRP not enable globally",
  errGvrpPortmode:"Only VLAN Trunk port can enable GVRP",
  errGvrpPortMirror:"Can not enable GVRP on mirror destination port",
  errGvrpPortType:"GVRP-enabled port must be in VLAN Trunk mode"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_GARP-->
  /* GMRP */
lang_gmrp:{
  lblGmrpGlobalStatus:"State",
  lblTableGmrp:"GVRP Setting Table",
  lblGmrpState:"State",
  lblGmrpRegist:"Registration",
  titGMRPPortTbl:"Port Setting Table",
  lblGmrpState:"State",
  lblGmrpRegist:"Registration",
  lblNormal:"Normal",
  lblFixed:"Fixed",
  lblForbidden:"Forbidden",
  titEditGMRPPortSetting:"Edit Port Setting",
  lblTimeParm:"Operational Timeout",
  lblJoin:"Join",
  lblLeave:"Leave",
  lblLeaveAll:"LeaveAll",

  lblcntType:"Statistics",
  lblAll:"All",
  lblRxCnt:"Receive",
  lblTxCnt:"Transmit",
  lblErrCnt:"Error",

  errGmrpGlobal:"GMRP not enable globally",
  errGmrpPortmode:"Only VLAN Trunk port can enable GMRP",
  errGmrpPortMirror:"Can not enable GMRP on mirror destination port",
  errGmrpPortType:"GMRP-enabled port must be in VLAN Trunk mode"
},
<!--#endif-->

/* Link Aggregation */
lang_lag:{
  lblBalance:"Load Balance Algorithm",
  lblBalanceMac:"MAC Address",
  lblBalanceIpMac:"IP-MAC Address",
  lblBalanceDstMac:"Dst-MAC Address",
  lblBalanceSrcMac:"Src-MAC Address",
  lblBalanceDstIp:"Dst-IP Address",
  lblBalanceSrcIp:"Src-IP Address",
  titMgmtTbl:"Link Aggregation Table",
  lblType:"Type",
  lblLACP:"LACP",
  lblStatic:"Static",
  lblLinkState:"Link Status",
  lblLagActiveMbrs:"Active Member",
  lblLagInactiveMbrs:"Inactive Member",
  titEditLAGGroup:"Edit Link Aggregation Group",
  titLAGPortTbl:"Port Setting Table",
  titEditLAGPort:"Edit Port Setting",
  lblTypeEthArg:"eth$1$2",
  lblLACPSysPriority:"System Priority",
  titLACPPortTbl:"LACP Port Setting Table",
  titEditLACPPort:"Edit LACP Port Setting",
  lblPortPriority:"Port Priority",
  lblLACPTimeout:"Timeout",
  lblLong:"Long",
  lblShort:"Short"
},

/* Line */
lang_line: {
  lblConsole:"Console",
  lblTelnet:"Telnet",
  <!--#ifdef CONFIG_SYS_APP_SSH-->
  lblSsh:"SSH",
  <!--#endif-->
  lblHttp:"HTTP",
  lblHttps:"HTTPS",
  <!--#ifdef CONFIG_SYS_UI_SNMP-->
  lblSnmp:"SNMP",
  <!--#endif-->
  lblMgmtService:"Management Service",
  lblSessTimeout:"Session Timeout",
  lblPassRetryCnt:"Password Retry Count",
  lblSilentTime:"Silent Time"
},

<!--#ifdef CONFIG_SYS_PROTO_LLDP-->
/* LLDP */
lang_lldp: {
  lblLLDPProp:"LLDP",
  lblLLDPStatus:"State",
  lblLLDPFramesHandling:"LLDP Handling",
  lblBridging:"Bridging",
  lblFiltering:"Filtering",
  lblFlooding:"Flooding",
  lblTLVAdvertiseInterval:"TLV Advertise Interval",
  lblHoldMultiplier:"Hold Multiplier",
  lblReinitialDelay:"Reinitializing Delay",
  lblTransmitDelay:"Transmit Delay",
  <!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
  lblLLDPMEDProp:"LLDP-MED",
  lblFastStartRepeatCount:"Fast Start Repeat Count",
  <!--#endif-->

  lblAdminStatus:"Mode",
  lblSelOptionTLVs:"Selected TLV",
  lblTxOnly:"Transmit",
  lblRxOnly:"Receive",
  lblTxAndRx:"Normal",
  titEditLLDPPortSettings:"Edit Port Setting",
  lblTLVs:"Optional TLV",
  lblAvailableOptionTLVs:"Available TLV",
  lblPortDescription:"Port Description",
  lblSysName:"System Name",
  lblSysDescription:"System Description",
  lblSysCapabilities:"System Capabilities",
  lblMACPHY:"802.3 MAC-PHY",
  lblLinkAggregation:"802.3 Link Aggregation",
  lblMaxFrameSize:"802.3 Maximum Frame Size",
  lblMgmtIpAddr:"Management IP Address",
  lblPVIDTLV:"802.1 PVID",
  lblVLANTLV:"802.1 VLAN Name",

  <!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
  lblLLDPMEDNetworkPolicyTable:"MED Network Policy Table",
  lblNetworkPolicyNum:"Policy ID",
  lblApp:"Application",
  lblVlanID:"VLAN",
  lblVlanTag:"VLAN Tag",
  lblUserPriority:"Priority",
  lblDSCPValue:"DSCP",
  lblTagged:"Tagged",
  lblUntagged:"Untagged",
  lblVoice:"Voice",
  lblVoiceSignaling:"Voice Signaling",
  lblGuestVoice:"Guest Voice",
  lblGuestVoiceSignaling:"Guest Voice Signaling",
  lblSoftphoneVoice:"Softphone Voice",
  lblVideoConferencing:"Video Conferencing",
  lblStreamingVideo:"Streaming Video",
  lblVideoSignaling:"Video Signaling",
  titAddLLDPMEDNetworkPolicy:"Add MED Network Policy",
  titEditLLDPMEDNetworkPolicy:"Edit MED Network Policy",

  lblLLDPMEDPortSettingTable:"MED Port Setting Table",
  lblLLDPMEDStatus:"State",
  lblUserDefinedNetworkPolicy:"Network Policy",
  lblLocation:"Location",
  lblInventory:"Inventory",
  lblActive:"Active",
  lblYes:"Yes",
  lblNo:"No",
  titEditLLDPMEDPortSettings:"Edit MED Port Setting",
  lblNetworkPolicy:"Network Policy",
  lblAvailableNetworkPolicies:"Available Policy",
  lblSelNetworkPolicies:"Selected Policy",
  lblCivic:"Civic",
  lblCoordinates:"Coordinate",
  lblECSELIN:"ECS ELIN",
  lbl16PairsHexChars:"($1 pairs of hexadecimal characters)",
  lbl6to160PairsHexChars:"($1 - $2 pairs of hexadecimal characters)",
  lbl10to25PairsHexChars:"($1 - $2 pairs of hexadecimal characters)",

  <!--#endif-->

  titLldpPacketView:"Packet View Table",
  lblTotalBytesInUse:"In-Use (Bytes)",
  lblAvailableBytesLeft:"Available (Bytes)",
  lblStatus:"Operational Status",
  lblOverloading:"Overloading",
  lblNotOverloading:"Not Overloading",
  titLLDPOverloadingDetails:"Packet View Detail",
  lblLLDPMandatoryTLVs:"Mandatory TLVs",
  lblLLDPMEDCapabilities:"MED Capabilities",
  lblLLDPMEDLocation:"MED Location",
  lblLLDPMEDNetworkPolicy:"MED Network Policy",
  lblLLDPMEDExtendedPowerViaMDI:"MED Extended Power via MDI",
  lblDot3Tlvs:"802.3 TLVs",
  lblLLDPOptionalTLVs:"Optional TLVs",
  lblLLDPMEDInventory:"MED Inventory",
  lblDot1Tlvs:"802.1 TLVs",
  lblTransmitted:"Transmitted",
  lblSizeBytes:"Size (Bytes)",
  lblTotalBytes:"Total (Bytes)",

  titLocalSummary:"Device Summary",
  btnLLDPPortStatusTable:"Port Status Table",
  lblLLDPState:"LLDP State",
  lblLLDPMEDState:"LLDP-MED State",
  btnLLDPLocalInfoDetail:"Local Information Detail",
  lblChassisIDSubtype:"Chassis ID Subtype",
  lblChassisID:"Chassis ID",
  lblSupportedSysCapabilities:"Supported Capabilities",
  lblEnabledSysCapabilities:"Enabled Capabilities",
  lblPortIDSubtype:"Port ID Subtype",
  lblChassisComponent:"Chassis component",
  lblIntfAlias:"Interface alias",
  lblPortComponent:"Port component",
  lblMacAddr:"MAC address",
  lblNetworkAddr:"Network address",
  lblIntfName:"Interface name",
  lblAgentCircuitID:"Agent circuit ID",
  lblLocal:"Local",
  lblOther:"Other",
  lblRepeater:"Repeater",
  lblBridge:"Bridge",
  lblL3:"Bridge, Router",
  lblWLANAccessPoint:"WLAN access point",
  lblRouter:"Router",
  lblTelephone:"Telephone",
  lblDOCSISCableDevice:"DOCSIS cable device",
  lblStationOnly:"Station only",
  lblPowerSourcingEntity:"Power Sourcing Entity",
  lblPoweredDevice:"Powered Device",
  lblUnknown:"Unknown",
  lblPrimaryPowerSource:"Primary Power Source",
  lblBackupPowerSource:"Backup Power Source",
  lblFromPSE:"From PSE",
  lblLocalAndPSE:"Local and PSE",
  lblCritical:"Critical",
  lblHigh:"High",
  lblLow:"Low",
  lblWatts:"$1.$2 Watts",
  lblFromPowerSourcingEntity:"From Power Sourcing Entity",
  lblLocalAndFromPowerSourcingEntity:"Local and From Power Sourcing Entity",
  tipNoNeighbors:"No Neighbors",

  lblPortID:"Port ID",
  lblPortDescription:"Port Description",
  lblMgmtAddrTable:"Management Address Table",
  lblAddrSubtype:"Address Subtype",
  lblAddr:"Address",
  lblIntfSubtype:"Interface Subtype",
  lblIntfNum:"Interface Number",
  lblIPv4:"IPv4",
  lblIPv6:"IPv6",
  lblNsap:"nsap",
  lblHdlc:"hdlc",
  lblBbn1822:"bbn1822",
  lblAll802:"all802",
  lblE163:"e163",
  lblE164:"e164",
  lblF69:"f69",
  lblX121:"x121",
  lblIpx:"ipx",
  lblAppleTalk:"Apple Talk",
  lblDecentIV:"DecentIV",
  lblBanyanVines:"Banyan Vines",
  lblE164WithNsap:"e164 with Nsap",
  lblDNS:"DNS",
  lblDistinguishedName:"Distinguished Name",
  lblAsNum:"as number",
  lblXtpOverIpv4:"xtp over Ipv4",
  lblXtpOverIpv6:"xtp over Ipv6",
  lblXtpNativeModeXtp:"xtp native mode XTP",
  lblIfIndex:"Interface Index",
  lblSysPortNum:"System Port Number",
  lblMACPHYDetail:"MAC/PHY Detail",
  lblAutoNegoSupported:"Auto-Negotiation Supported",
  lblAutoNegoEnabled:"Auto-Negotiation Enabled",
  lblTrue:"True",
  lblFalse:"False",
  lblAutoNegoAdvertised:"Auto-Negotiation Advertised Capabilities",
  lblOperMAUType:"Operational MAU Type",

  lbl10BaseT:"10baseT",
  lbl10BaseTFD:"10baseTFD",
  lbl100BaseT4:"100baseT4",
  lbl100BaseTX:"100baseTX",
  lbl100BaseTXFD:"100baseTXFD",
  lbl100BaseT2:"100baseT2",
  lbl100BaseT2FD:"100baseT2FD",
  lblFdxPause:"FdxPause",
  lblFdxAPause:"FdxAPause",
  lblFdxSPause:"FdxSPause",
  lblFdxBPause:"FdxBPause",
  lbl1000BaseX:"1000baseX",
  lbl1000BaseXFD:"1000baseXFD",
  lbl1000BaseT:"1000baseT",
  lbl1000BaseTFD:"1000baseTFD",

  lblMauAui:"dot3MauTypeAUI",
  lblMau10Base5:"dot3MauType10Base5",
  lblMau10Foirl:"dot3MauTypeFoirl",
  lblMau10Base2:"dot3MauType10Base2",
  lblMau10BaseT:"dot3MauType10BaseT",
  lblMau10BaseFP:"dot3MauType10BaseFP",
  lblMau10BaseFB:"dot3MauType10BaseFB",
  lblMau10BaseFL:"dot3MauType10BaseFL",
  lblMau10Broad36:"dot3MauType10Broad36",
  lblMau10BaseTHD:"dot3MauType10BaseTHD",
  lblMau10BaseTFD:"dot3MauType10BaseTFD",
  lblMau10BaseFLHD:"dot3MauType10BaseFLHD",
  lblMau10BaseFLFD:"dot3MauType10BaseFLFD",
  lblMau100BaseT4:"dot3MauType100BaseT4",
  lblMau100BaseTXHD:"dot3MauType100BaseTXHD",
  lblMau100BaseTXFD:"dot3MauType100BaseTXFD",
  lblMau100BaseFXHD:"dot3MauType100BaseFXHD",
  lblMau100BaseFXFD:"dot3MauType100BaseFXFD",
  lblMau100BaseT2HD:"dot3MauType100BaseT2HD",
  lblMau100BaseT2FD:"dot3MauType100BaseT2FD",
  lblMau1000BaseXHD:"dot3MauType1000BaseXHD",
  lblMau1000BaseXFD:"dot3MauType1000BaseXFD",
  lblMau1000BaseLXHD:"dot3MauType1000BaseLXHD",
  lblMau1000BaseLXFD:"dot3MauType1000BaseLXFD",
  lblMau1000BaseSXHD:"dot3MauType1000BaseSXHD",
  lblMau1000BaseSXFD:"dot3MauType1000BaseSXFD",
  lblMau1000BaseCXHD:"dot3MauType1000BaseCXHD",
  lblMau1000BaseCXFD:"dot3MauType1000BaseCXFD",
  lblMau1000BaseTHD:"dot3MauType1000BaseTHD",
  lblMau1000BaseTFD:"dot3MauType1000BaseTFD",
  lblMau10GigBaseX:"dot3MauType10GigBaseX",
  lblMau10GigBaseLX4:"dot3MauType10GigBaseLX4",
  lblMau10GigBaseR:"dot3MauType10GigBaseR",
  lblMau10GigBaseER:"dot3MauType10GigBaseER",
  lblMau10GigBaseLR:"dot3MauType10GigBaseLR",
  lblMau10GigBaseSR:"dot3MauType10GigBaseSR",
  lblMau10GigBaseW:"dot3MauType10GigBaseW",
  lblMau10GigBaseEW:"dot3MauType10GigBaseEW",
  lblMau10GigBaseLW:"dot3MauType10GigBaseLW",
  lblMau10GigBaseSW:"dot3MauType10GigBaseSW",

  lblDot3Details:"802.3 Detail",
  lblDot3MaxFrameSize:"802.3 Maximum Frame Size",
  lblDot3LinkAggregation:"802.3 Link Aggregation",
  lblAggregationCapability:"Aggregation Capability",
  lblAggregationStatus:"Aggregation Status",
  lblAggregationPortID:"Aggregation Port ID",
  lblCapable:"Capable",
  lblNotCapable:"Not capable",
  lblAggr:"Aggregated",
  lblNotAggr:"Not aggregated",

  <!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
  lblMEDDetails:"MED Detail",
  lblCapabilitiesSupported:"Capabilities Supported",
  lblCurrentCapabilities:"Current Capabilities",
  lblDeviceClass:"Device Class",
  lblDot3PowerType:"Power Type",
  lblDot3PowerSource:"Power Source",
  lblDot3PowerPriority:"Power Priority",
  lblRequestPowerValue:"PD Request Power Value",
  lblAllocatedPowerValue:"PSE Allocated Power Value",
  lblPoEDeviceType:"PoE Device Type",
  lblPoEPowerSource:"PoE Power Source",
  lblPoEPowerPriority:"PoE Power Priority",
  lblPoEPowerValue:"PoE Power Value",
  lblHardwareRevision:"Hardware Revision",
  lblFirmwareRevision:"Firmware Revision",
  lblSoftwareRevision:"Software Revision",
  lblSerialNumber:"Serial Number",
  lblManufacturerName:"Manufacturer Name",
  lblModelName:"Model Name",
  lblAssetID:"Asset ID",
  lblCapabilities:"Capabilities",
  lblNetworkPolicy:"Network policy",
  lblExtPSE:"Extended PSE",
  lblExtPD:"Extended PD",
  lblNotDef:"Not Defined",
  lblEndClass1:"Endpoint Class 1",
  lblEndClass2:"Endpoint Class 2",
  lblEndClass3:"Endpoint Class 3",
  lblNetCon:"Network Connectivity",
  lblLocationInfo:"Location Information",
  lblNetworkPolicyTable:"Network Policy Table",
  lblAppType:"Application Type",
  lblVlanType:"VLAN Type",
  lblDSCP:"DSCP",
  <!--#endif-->

  btnNeighborTable:"Neighbor Table",
  btnLLDPNeighborDetail:"Neighbor Information Detail",
  lblLocalPort:"Local Port",
  lblTTL:"Time to Live",
  lblLLDPNeighborInfo:"LLDP Neighbor Information",
  lblPortDetails:"Port Detail",
  lblMSAPEntry:"MSAP Entry",
  lblBasicDetails:"Basic Detail",
  lblDot3PowerViaMDI:"802.3 Power via MDI",
  lblMDIPowerSupportPortClass:"MDI Power Support Port Class",
  lblPSEMDIPowerSupport:"PSE MDI Power Support",
  lblPSEMDIPowerState:"PSE MDI Power State",
  lblPSEPowerPairControlAbility:"PSE Power Pair Control Ability",
  lblPSEPowerPair:"PSE Power Pair",
  lblPSEPowerClass:"PSE Power Class",
  lblSignal:"Signal",
  lblSpare:"Spare",
  lblClassPSE:"pClassPSE",
  lblClassPD:"pClassPD",
  lblSupported:"Supported",
  lblNotSupported:"Not Supported",
  lblEnabled:"Enabled",
  lblDisabled:"Disabled",
  lblClass:"Class $1",
  lblDot1VlanAndProtocol:"802.1 VLAN and Protocol",
  lblPVID:"PVID",
  lblPPVIDs:"PPVIDs",
  lblPPVIDTable:"PPVID Table",
  lblVID:"VID",
  lblSupported:"Supported",
  lblVlanIDs:"VLAN IDs",
  lblVlanIDTable:"VLAN ID Table",
  lblVlanName:"VLAN Name",
  lblProtocolIDs:"Protocol IDs",
  lblProtocolIDTable:"Protocol ID Table",
  lblProtocolID:"Protocol ID",

  titLLDPGlobalStatististics:"Global Statistics",
  lblInsertions:"Insertions",
  lblDeletions:"Deletions",
  lblDrops:"Drops",
  lblAgeOuts:"AgeOuts",
  lblLLDPStatisticsTable:"Statistics Table",
  lblTxFrames:"Transmit Frame",
  lblRxFrames:"Receive Frame",
  lblRxTLVs:"Receive TLV",
  lblNeighborInfoDeleteCnt:"Neighbor<br>Timeout",
  lblTotal:"Total",
  lblDiscarded:"Discard",
  lblErrors:"Error",
  lblUnrecognized:"Unrecognized"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_SYSLOG-->
lang_log:{
  lblLoggingTable:"Logging Message Table",
  lblLoggingType:"Viewing",
  txtRam:"RAM",
  txtFile:"Flash",
  lblLogIdx:"Log ID",
  lblLogTime:"Time",
  lblLogSev:"Severity",
  lblLogDesc:"Description",
  lblState:"State",
  lblAggregation:"Aggregation",
  lblAggAgingTime:"Aging Time",
  lblConsoleLogging:"Console Logging",
  lblRamLogging:"RAM Logging",
  lblFileLogging:"Flash Logging",
  lblMinSev:"Minimum&nbsp<br>Severity",
  lblRemoteLogServerTable:"Remote Server Table",
  lblLogServer:"Log Server",
  lblFacility:"Facility",
  lblLocalArg:"Local $1",
  titAddRemoteLogServer:"Add Remote Server",
  titEditRemoteLogServer:"Edit Remote Server",
  txtEmergency:"Emergency",
  txtAlert:"Alert",
  txtCritical:"Critical",
  txtError:"Error",
  txtWarning:"Warning",
  txtNotice:"Notice",
  txtInformational:"Informational",
  txtDebug:"Debug"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_ISG-->
/* IP Source Guard */
lang_ipsg: {
  titIPSGSetting:"Port Setting Table",
  titIPSGEditPortSetting:"Edit  Port Setting",
  titIPSGBinding:"IP-MAC-Port-VLAN Binding Table",
  titBindingEntryAdd:"Add IP-MAC-Port-VLAN Binding",
  titBindingEntryEdit:"Edit IP-MAC-Port-VLAN Binding",
  lblStatus:"State",
  lblVerifySrc:"Verify Source",
  lblCurEntryNo:"Current Entry",
  lblMaxEntryNo:"Max Entry",
  lblVerifySrcIP:"IP",
  lblVerifySrcMACIP:"IP-MAC",
  lblIPSGBinding:"Binding",
  lblIPPort:"IP-Port-VLAN",
  lblIPMacPort:"IP-MAC-Port-VLAN",
  lblMACAddr:"MAC Address",
  lblIPAddr:"IP Address",
  lblType:"Type",
  lblLeaseTime:"Lease Time",
  lblTypeStatic:"Static",
  lblTypeDynamic:"Dynamic",
  lblBKDBType:"Type",
  lblBKDBTypeNone:"None",
  lblBKDBTypeFlash:"Flash",
  lblBKDBTypeTFTP:"TFTP",
  lblBKDBFileName:"Filename",
  lblBKDBWriteDelay:"Write Delay",
  lblBKDBTimeout:"Timeout"
},
<!--#endif-->

lang_mac:{
  titMacAddressTable:"MAC Address Table",
  titDynamicAddressTable:"Dynamic Address Table",
  titStaticMacTable:"Static Address Table",
  titStaticMacAdd:"Add Static Address",
  titStaticMacEdit:"Edit Static Address",
<!--#ifdef CONFIG_SYS_BLACK_HOLE_MAC-->
  titAddressFilterTable:"Filtering Address Table",
  titAddressFilterAdd:"Add Filtering Address",
  titAddressFilterEdit:"Edit Filtering Address",
<!--#endif-->
  lblAgingTime:"Aging Time",
  lblAddToStatic:"Add Static Address",
  lblMac:"MAC Address",
  lblType:"Type",
  lblCpuPort:"CPU",
  lblMacTypeManage:"Management",
<!--#ifdef CONFIG_SYS_BLACK_HOLE_MAC-->
  lblMacTypeBlacklist:"Drop",
<!--#endif-->
<!--#ifdef CONFIG_SYS_PROTO_AUTHMGR-->
  lblMacTypeAuth:"Auth",
<!--#endif-->
<!--#ifdef CONFIG_SYS_PORT_SECURITY-->
  lblMacTypeSecureDynamic:"SecureDynamic",
  lblMacTypeSecureSticky:"SecureSticky",
  lblMacTypeSecureConfigured:"SecureConfigured",
<!--#endif-->
  lblMacTypeStatic:"Static",
  lblMacTypeDynamic:"Dynamic"
},

/* Menu tree */
lang_menu: {
  fld1Status:"Status",
  doc1StatusInfo:"System Information",
  doc1StatusSyslog:"Logging Message",
  fld2StatusPort:"Port",
  doc2StatusPortCnt:"Statistics",
  <!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
  doc2StatusPortErrDisable:"Error Disabled",
  <!--#endif-->
  doc2StatusPortBwUtiliz:"Bandwidth Utilization",
  doc1StatusLAG:"Link Aggregation",
  doc1StatusMac:"MAC Address Table",

  fld1Network:"Network",
  doc1NetworkIPAddr:"IP Address",
  <!--#ifdef CONFIG_SYS_ADVANCE_DNS-->
  doc1NetworkDNS:"DNS",
  doc1NetworkDNSHosts:"Hosts",
  <!--#endif-->
  doc1NetworkTimeSys:"System Time",

  fld1Port:"Port",
  ringPort:"ERPS",
  doc1PortSetting:"Port Setting",
  <!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
  doc1PortErrDisable:"Error Disabled",
  <!--#endif-->
  fld2PortLAG:"Link Aggregation",
  doc2PortLAGMgmt:"Group",
  doc2PortLAGPort:"Port Setting",
  doc2PortLACP:"LACP",
  <!--#ifdef CONFIG_SYS_EEE-->
  doc1PortEEE:"EEE",
  <!--#endif-->
  doc1PortJumbo:"Jumbo Frame",

  fld1Vlan:"VLAN",
  fld2Vlan:"VLAN",
  doc1VlanCreate:"Create VLAN",
  doc1VlanVLANConf:"VLAN Configuration",
  doc1VlanMembership:"Membership",
  doc1VlanPort:"Port Setting",

  <!--#ifdef CONFIG_SYS_VOICEVLAN-->
  fld2VlanVoice:"Voice VLAN",
  doc2VlanVoiceProp:"Property",
  doc2VlanVoiceOUI:"Voice OUI",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTOCOL_VLAN-->
  fld2VlanProtocol:"Protocol VLAN",
  doc1VlanProtoGroup:"Protocol Group",
  doc1VlanProtoPort:"Group Binding",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_IPS_SUBNET-->
  fld2VlanIPsubnet:"IP Subnet VLAN",
  doc1VlanIPsubnetGroup:"IP Subnet Group",
  doc1VlanIPsubnetPort:"Group Binding",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_MACVLAN-->
  fld2VlanMac:"MAC VLAN",
  doc1VlanMacGroup:"MAC Group",
  doc1VlanMacGroupMapping:"Group Binding",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_SURVEILLANCEVLAN-->
  fld2VlanSurveillance:"Surveillance VLAN",
  doc2VlanSurveillanceProp:"Property",
  doc2VlanSurveillanceOUI:"Surveillance OUI",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_GVRP-->
  fld2VlanGVRP:"GVRP",
  doc2VlanGVRPProperty:"Property",
  doc2VlanGVRPMembership:"Membership",
  doc2VlanGVRPStatistics:"Statistics",
  doc2VlanGVRPErrorStatistics:"Error Statistics",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_GARP-->
  fld2GMRP:"GMRP",
  doc2GMRPProperty:"Property",
  doc2GMRPMembership:"Membership",
  doc2GMRPStatistics:"Statistics",
  doc2GMRPErrorStatistics:"Error Statistics",
  <!--#endif-->
  fld1Mac:"MAC Address Table",
  doc1MacDynamic:"Dynamic Address",
  doc1MacStatic:"Static Address",
  <!--#ifdef CONFIG_SYS_BLACK_HOLE_MAC-->
  doc1MacFilter:"Filtering Address",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PORT_SECURITY-->
  doc1MacPsecure:"Port Security Address",
  <!--#endif-->

  fld1STP:"Spanning Tree",
  doc1StpGlobal:"Property",
  doc1StpIntf:"Port Setting",
  <!--#ifdef CONFIG_SYS_PROTO_STP_MSTP-->
  doc1StpMstInstance:"MST Instance",
  doc1StpMstIntf:"MST Port Setting",
  <!--#endif-->
  doc1StpStatistics:"Statistics",

  <!--#ifdef CONFIG_SYS_PROTO_LLDP-->
  fld1Discov:"Discovery",
  fld2DiscovLLDP:"LLDP",
  doc2DiscovLLDPProp:"Property",
  doc2DiscovLLDPPort:"Port Setting",
  <!--#ifdef CONFIG_SYS_PROTO_LLDP_MED-->
  doc2DiscovLLDPMedPolicy:"MED Network Policy",
  doc2DiscovLLDPMedPort:"MED Port Setting",
  <!--#endif-->
  doc2DiscovLLDPOverloading:"Packet View",
  doc2DiscovLLDPLocalInfo:"Local Information",
  doc2DiscovLLDPNeighbor:"Neighbor",
  doc2DiscovLLDPStatistics:"Statistics",
  <!--#endif-->

  fld1Mcast:"Multicast",
  fld2McastGeneral:"General",
  doc2McastGeneralProperty:"Property",
  doc2McastGeneralGroupAddr:"Group Address",
  doc2McastGeneralRouterPort:"Router Port",
  <!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD-->
  doc2McastGeneralForwardAll:"Forward All",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING-->
  doc2McastGeneralThrottling:"Throttling",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING-->
  doc2McastIGMPGeneralFilterProfile:"Filtering Profile",
  doc2McastIGMPGeneralFilterBinding:"Filtering Binding",
  <!--#endif-->
  fld2McastIGMP:"IGMP Snooping",
  doc2McastIGMPProperty:"Property",
  doc2McastIGMPQuerier:"Querier",
  doc2McastIGMPStatistics:"Statistics",
  <!--#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING-->
  fld2McastMLD:"MLD Snooping",
  doc2McastMLDProperty:"Property",
  doc2McastMLDStatistics:"Statistics",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_MVR-->
  fld2McastMVR:"MVR",
  doc2McastMVRProperty:"Property",
  doc2McastMVRPortSetting:"Port Setting",
  doc2McastMVRGroupAddress:"Group Address",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_L3_ROUTE-->
  fld1IPConfig:"Routing",
  fld2IPv4:"IPv4 Management and Interfaces",
  doc2IPv4Intf:"IPv4 Interface",
  doc2IPv4Route:"IPv4 Routes",
  doc2IPv4Arp:"ARP",
  doc2IPv4ArpProxy:"ARP Proxy",
  fld2IPv6:"IPv6 Management and Interfaces",
  doc2IPv6Intf:"IPv6 Interface",
  doc2IPv6Addr:"IPv6 Addresses",
  doc2IPv6Route:"IPv6 Routes",
  doc2IPv6Neigh:"IPv6 Neighbors",
  fld2Rip:"Rip Routes Management",
  doc2IPv4Rip:"Rip Routes Setting",
  fld2Ospf:"Ospf Routes Management",
  doc2IPv4Ospf:"Ospf Routes Setting",
  fld2Vrrp:"VRRP Management",
  doc2IPv4Vrrp:"VRRP Interfaces Setting",
  <!--#endif-->

  fld1Sec:"Security",
  <!--#ifdef CONFIG_SYS_LIB_RADIUS-->
  doc1SecRadius:"RADIUS",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_LIB_TACPLUS-->
  doc1SecTacplus:"TACACS+",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_AAA-->
  fld2SecAAA:"AAA",
  doc2SecAAALoginList:"Method List",
  doc2SecAAALoginAuthen:"Login Authentication",
  <!--#endif-->
  fld2SecMgmtAccess:"Management Access",
  doc2SecMgmtVLAN:"Management VLAN",
  doc2SecMgmtService:"Management Service",
  <!--#ifdef CONFIG_SYS_MGMT_ACL-->
  doc2SecMgmtACL:"Management ACL",
  doc2SecMgmtACE:"Management ACE",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_AUTHMGR-->
  fld2SecAuthmgr:"Authentication Manager",
  doc2SecAuthmgrProp:"Property",
  doc2SecAuthmgrPort:"Port Setting",
  doc2SecAuthmgrMacLocal:"MAC-Based Local Account",
  doc2SecAuthmgrWebLocal:"WEB-Based Local Account",
  doc2SecAuthmgrSess:"Sessions",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PORT_SECURITY-->
  doc1SecPort:"Port Security",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_ERPS-->
  ringPortErps:"ERPS Instance",
  ringStatusErps:"Propety",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PORT_ISOLATION-->
  doc1SecProtected:"Protected Port",
  <!--#endif-->
  doc1SecStorm:"Storm Control",
  <!--#ifdef CONFIG_SYS_DOS_PROTECTION-->
  fld2SecDos:"DoS",
  doc2SecDosProp:"Property",
  doc2SecDosPort:"Port Setting",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_ISG-->
  fld2SecISG:"IP Source Guard",
  doc2SecISGPort:"Port Setting",
  doc2SecISGBind:"IMPV Binding",
  doc2SecISGDatabase:"Save Database",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_DAI-->
  fld2SecDAI:"Dynamic ARP Inspection",
  doc2SecDAI:"Property",
  doc2SecDAIStatistic:"Statistics",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING-->
  fld2SecDS:"DHCP Snooping",
  doc2SecDS:"Property",
  doc2SecDSStatistic:"Statistics",
  doc2SecDSOpt82Global:"Option82 Property",
  doc2SecDSOpt82Circuit:"Option82 Circuit ID",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_USER_DEFINED_ACL-->
  fld1Acl:"ACL",
  doc1AclMAC:"MAC ACL",
  doc1AclMACAce:"MAC ACE",
  doc1AclIPv4:"IPv4 ACL",
  doc1AclIPv4Ace:"IPv4 ACE",
  doc1AclIPv6:"IPv6 ACL",
  doc1AclIPv6Ace:"IPv6 ACE",
  doc1AclBind:"ACL Binding",
  <!--#endif-->

  fld1Qos:"QoS",
  fld2QosGeneral:"General",
  doc2QosGeneralProp:"Property",
  doc2QosGeneralQueue:"Queue Scheduling",
  doc2QosGeneralCoS:"CoS Mapping",
  doc2QosGeneralDSCP:"DSCP Mapping",
  doc2QosGeneralIPP:"IP Precedence Mapping",
  fld2QosRate:"Rate Limit",
  doc2QosRateBwctl:"Ingress / Egress Port",
  doc2QosRateEgressQueue:"Egress Queue",

  fld1Diag:"Diagnostics",
  fld2DiagLog:"Logging",
  doc2DiagLogProp:"Property",
  doc2DiagLogRemote:"Remote Server",
  doc1DiagMirror:"Mirroring",
  doc1DiagPing:"Ping",
  doc1DiagTraceRoute:"Traceroute",
  doc1DiagCopper:"Copper Test",
  <!--#ifdef CONFIG_SYS_OPTICALMOD_STATUS-->
  doc1DiagFiber:"Fiber Module",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PROTO_UDLD-->
  fld2SecUDLD:"UDLD",
  doc2SecUDLDProp:"Property",
  doc2SecUDLDNeighbors:"Neighbor",
  <!--#endif-->
  doc1DiagReboot:"Reboot",

  fld1Mgmt:"Management",
  doc1MgmtAccount:"User Account",
  fld2MgmtFirmware:"Firmware",
  doc2MgmtUpgradeBackup:"Upgrade / Backup",
  doc2MgmtActiveImg:"Active Image",
  fld2MgmtConfig:"Configuration",
  doc2MgmtConfigSave:"Save Configuration",
  <!--#ifdef CONFIG_SYS_UI_SNMP-->
  fld2MgmtSNMP:"SNMP",
  <!--#ifdef CONFIG_SYS_SNMPV3-->
  doc2MgmtSNMPView:"View",
  doc2MgmtSNMPGroup:"Group",
  doc2MgmtSNMPCommunity:"Community",
  doc2MgmtSNMPUser:"User",
  doc2MgmtSNMPEngine:"Engine ID",
  doc2MgmtSNMPTrapEvent:"Trap Event",
  doc2MgmtSNMPNotify:"Notification",
  <!--#else-->
  doc2MgmtSNMPCommunity:"Community",
  doc2MgmtSNMPTrapEvent:"Trap Event",
  doc2MgmtSNMPNotify:"Notification",
  <!--#endif-->
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI-->
  fld2MgmtRMON:"RMON",
  doc2MgmtRMONStatistic:"Statistics",
  doc2MgmtRMONEvent:"Event",
  doc2MgmtRMONAlarm:"Alarm",
  doc2MgmtRMONHistory:"History",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_POE-->
  fld1Poe:"POE Setting",
  doc1PoePort:"POE Port Setting",
  doc1PoePortTimer:"POE Port Timer Setting",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_DHCP_SERVER-->
  fld1Dhcp:"DHCP",
  doc1Dhcp:"Property",
  doc1IPPool:"IP Pool Setting",
  doc1vlanifpool:"VLAN IF Address Group Setting",
  doc1DhcpClients:"Client List",
  doc1DhcpStatic:"Client Static Binding Table",
  <!--#endif-->
  dummy:""
},

lang_mcast:{
  lblUnknownMcastAct:"Unknown Multicast&nbsp<br>Action",
  lblDrop:"Drop",
  lblFlood:"Flood",
  lblForwardToRouterPort:"Forward to Router Port",
  lblFwdMethod:"Multicast Forward Method",
  lblDmac:"DMAC-VID",
  lblDip:"DIP-VID",

  <!--#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING-->
  txtIPv4:"IPv4",
  txtIPv6:"IPv6",
  lblMLDv1:"MLDv1",
  lblMLDv2:"MLDv2",
  <!--#endif-->

  lblMcastGrpTable:"Group Address Table",
  lblMcastGroupAddr:"Group Address",
  lblMemberPort:"Member",
  lblStatic:"Static",
  lblForbid:"Forbidden",
  lblNone:"None",
  lblDynamic:"Dynamic",
  lblForbidden:"Forbidden",
  lblType:"Type",
  lblLifeSec:"Life (Sec)",

  titEditGroup:"Edit Group Address",
  titAddGroup:"Add Group Address",

  lblMcastRouterTable:"Router Port Table",
  lblRtMbrPort:"Port",
  <!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER-->
  titAddRouter:"Add Router Port",
  titEditRouter:"Edit Router Port",
  lblStRtMbrPort:"Static Port",
  lblForbidRtMbrPort:"Forbidden Port",
  lblRtMbrPortType:"Type",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD-->
  lblMcastFwdAllTable:"Forward All Table",
  titAddFwdAll:"Add Forward All",
  titEditFwdAll:"Edit Forward All",
  lblStFwdMbrPort:"Static Port",
  lblForbidFwdMbrPort:"Forbidden Port",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING-->
  lblDeny:"Deny",
  lblReplace:"Replace",
  txtMcastThrotTable:"Throttling Table",
  txtEditMcastThrotTable:"Edit Throttling",
  lblMaxGroup:"Max Group",
  lblMaxAct:" Exceed Action",
  <!--#endif-->

  <!--#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING-->
  titProfilesTable:"Filtering Profile Table",
  lblProfileIndex:"Profile ID",
  lblStartMulticastAddr:"Start Address",
  lblEndMulticastAddr:"End Address",
  lblAction:"Action",
  lblAllow:"Allow",
  lblProfDeny:"Deny",
  titAddProfiles:"Add Profile",
  titEditProfile:"Edit Profile",

  titMulticastFilterTable:"Filtering Binding Table",
  titEditFilterSettings:"Edit Filtering Binding",
  lblFilterProfileIndex:"Profile ID",
  <!--#endif-->

  lblMcastState:"State",
  lblMcastVersion:"Version",
  lblIGMPv1:"IGMPv1",
  lblIGMPv2:"IGMPv2",
  lblIGMPv3:"IGMPv3",

  lblReportSuppression:"Report Suppression",
  titMcastVlan:"VLAN Setting Table",
  tblMRouterPortsAutoLearn:"Router Port<br>Auto Learn",
  tblForbiddenPorts:"Forbidden<br>Ports",
  tblForbiddenMRouterPorts:"Forbidden<br>Router Ports",
  tblQueryRobustness:"Query<br>Robustness",
  tblQueryInterval:"Query<br>Interval",
  tblQueryMaxResponseInterval:"Query Max<br>Response Interval",
  tblLastMemberQueryCounter:"Last Member<br>Query Counter",
  tblLastMemberQueryInterval:"Last Member<br>Query Interval",
  tblImmediateLeave:"Immediate Leave",
  titQuerierTable:"Querier Table",
  titQuerierTableEdit:"Edit Querier",
  tblIGMPQuerierState:"State",
  tblIGMPQuerierStatus:"Operational Status",
  tblIGMPQuerierVersion:"Version",
  tblQuerierIPAddress:"Querier Address",

  titEditMcastVlan:"Edit VLAN Setting",
  lblOperStatus:"Status",
  lblMRouterPortsAutoLearn:"Router Port Auto Learn",
  lblQueryRobustness:"Query Robustness",
  lblOperationalQueryRobustness:"Query Robustness",
  lblQueryInterval:"Query Interval",
  lblOperationalQueryInterval:"Query Interval",
  lblQueryMaxResponseInterval:"Query Max Response Interval",
  lblOperationalQueryMaxResponseInterval:"Query Max Response Interval",
  lblLastMemberQueryCounter:"Last Member Query Counter",
  lblOperationalLastMemberQueryCounter:"Last Member Query Counter",
  lblLastMemberQueryInterval:"Last Member Query Interval",
  lblOperationalLastMemberQueryInterval:"Last Member Query Interval",
  lblImmediateleave:"Immediate leave",
  lblIGMPQuerierVersion:"Version",
  lblSecArg:"$1 (Sec)",

  titIGMPStat:"Statistics",
  lblRX:"Receive Packet",
  lblTX:"Transmit Packet",
  lblTotal:"Total",
  lblValid:"Valid",
  lblInValid:"InValid",
  lblOther:"Other",
  lblLeave:"Leave",
  lblReport:"Report",
  lblGenQry:"General Query",
  lblSpecGryQry:"Special Group Query",
  lblSpecGrySrcQry:"Source-specific Group Query",

  errMcastPortSel:"No port select",
  errMcastVlanSel:"No vlan select",
  lblUnused:"Not use"
},


<!--#ifdef CONFIG_SYS_PROTO_ERPS-->
lang_ring:{

  ringErpsStatuss:"Erps Status",
  ringErpsInstances:"Erps Instance",
  erpsInstance:"Instance",
  erpsRingStatus:"Ring Status",
  erpsRingMel:"Mel",
  erpsRingControlVlan:"Control Vlan",
  erpsRingWtrTime:"WTR Time",
  erpsRingGuardTime:"Guard Time",
  erpsRingWorkingMode:"Work Mode",
  erpsRingId:"Ring ID",
  erpsRingRingLevel:"Ring Type",
  erpsRingRingProtectedInstance:"Protected Instance",
  erpsRingPort0:"Port0",
  erpsRingPort0Role:"Port Role",
  erpsRingPort0Status:"Port Status",
  erpsRingPort1:"Port1",
  erpsRingPort1Role:"Port Role",
  erpsRingPort1Status:"Port Status",
  erpsRingNodeStatus:"Node Status",
  titEditRingInstance:"Ring Instane Config",
  erpsRingModeRevertive:"Revertive",
  erpsRingModeNoRevertive:"Non_revertive",
  erpsGlobalConfig:"ERPS Instance Setting",
  erpsInstanceAddOrDel:"Ins Seting",
  erpsInstanceAdd:"Add",
  erpsInstanceDelte:"Delete",
  
  erpsRingPort1RoleValue:"Port1 Role",
  erpsRingPort0RoleValue:"Port0 Role",
  erpsInstanceRingPortRoleRpl1:"Normal",
  erpsInstanceRingPortRoleOwner1:"owner",
  erpsInstanceRingPortRoleNeighbour1:"neihbour",
  erpsInstanceRingPortRoleNextNeighbour1:"next-neighbour",
  erpsInstanceRingPortRoleRpl0:"Normal",
  erpsInstanceRingPortRoleOwner0:"owner",
  erpsInstanceRingPortRoleNeighbour0:"neihbour",
  erpsInstanceRingPortRoleNextNeighbour0:"next-neighbour",
  
  
  
  erpsInsName:"Ins",
  lblUnused:"Not use"
},
<!--#endif-->


<!--#ifdef CONFIG_SYS_PROTO_MVR-->
lang_mvr:{
  lblState:"State",
  txtComp:"Compatible",
  txtDyn:"Dynamic",
  lblMVRGroupStart:"Group Start",
  lblMVRGroupSeq:"Group Count",
  lblMVRQryTime:"Query Time",
  lblMVRMode:"Mode",
  lblOperationalGroupNum:"Operational Group",
  lblMaxGroupNum:"Maximum",
  lblCurGroupNum:"Current",

  lblGroupTable:"Group Address Table",
  lblGroupAddress:"Group Address",
  txtPortTable:"Port Setting Table",
  txtEditPortTable:"Edit Port Setting",
  lblPortType:"Role",
  lblPortFastLeave:"Immediate Leave",
  lblNone:"None",
  lblReceiver:"Receiver",
  lblSource:"Source",

  lblUnused:"Not use"
},
<!--#endif-->

/* Mirror */
<!--#ifdef CONFIG_SYS_MIRROR-->
lang_mirror: {
  titMirrorSettingTbl:"Mirroring Table",
  titEditMirroring:"Edit Mirroring",
  lblMirrorSessionId:"Session ID",
  lblMirrorDestPort:"Monitor Port",
  lblMirrorIngressStateLabel:"Send or Receive Normal Packet",
  lblMirrorIngressState:" (Normal*)",
  lblMirrorIngressStateHint:"\"*\" Allow the monitor port to send or receive normal packets",
  lblMirrorSourceRXPort:"Ingress Port",
  lblMirrorSourceTXPort:"Egress Port"
},
<!--#endif-->

/* Port */
lang_port:{
  titPortSettingTable:"Port Setting Table",
  titEditPortSetting:"Edit Port Setting",
  lblEntryNo:"Entry",
  lblPort:"Port",
  lblPortType:"Type",
  lblDescription:"Description",
  lblAdminStatus:"State",
  lblOperStatus:"Link Status",
  lblSpeed:"Speed",
  lblDuplex:"Duplex",
  lblFlowCtrl:"Flow Control",
  lblFlowCtrlOn:"On",
  lblFlowCtrlOff:"Off",
  lblAuto:"Auto",
  lblAutoCapArg:"Auto - $1",
  txtUp:"Up",
  txtDown:"Down",
  txtDuplexFull:"Full",
  txtDuplexHalf:"Half",
  txtMediaCopper:"$1M Copper",
  txtMediaFiber:"$1M Fiber",
  txtMediaGFiber:"$1G Fiber",
  txtMediaComboC:"$1M Combo Copper",
  txtMediaComboF:"$1M Combo Fiber",
  <!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
  lblRecoveryInterval:"Recovery Interval",
  lblACL:"ACL",
  lblARPRateLimit:"ARP Rate Limit",
  lblBPDUGuard:"BPDU Guard",
  lblBcastFlood:"Broadcast Flood",
  lblDHCPRateLimit:"DHCP Rate Limit",
  lblPortSecurity:"Port Security",
  lblSelfLoop:"Self Loop",
  lblUdld:"UDLD",
  lblUnicastFlood:"Unicast Flood",
  lblUnknownMcastFlood:"Unknown Multicast Flood",
  <!--#endif-->
  lblJumboSize:"Jumbo Frame",
  <!--#ifdef CONFIG_SYS_EEE-->
  titEEESettingTable:"EEE Setting Table",
  titEditEEESetting:"Edit EEE Setting",
  lblEEEAdminState:"State",
  lblEEEState:"Operational Status",
  <!--#endif-->
  <!--#ifdef CONFIG_SYS_PORT_ERR_DISABLED-->
  titPortErrorDisabledTable:"Error Disabled Table",
  lblErrorDisableReason:"Reason",
  lblReasonBpduGuard:"BPDU Guard",
  lblReasonUdld:"UDLD",
  lblReasonSelfLoop:"Self Loop",
  lblReasonBroadcastFlood:"Broadcast Flood",
  lblReasonUnknownMltiCastFlood:"Unknown Multicast Flood",
  lblReasonUnicastFlood:"Unicast Flood",
  lblReasonAcl:"ACL",
  lblReasonPortSecurity:"Port Security",
  lblReasonDhcpRateLimit:"DHCP Rate Limit",
  lblReasonArpRateLimit:"ARP Rate Limit",
  lblTimeLeft:"Time Left (sec)",
  <!--#endif-->
  titProtectTable:"Protected Port Table",
  titEditProtect:"Edit Protected Port",
  lblProtectState:"State",
  lblProtected:"Protected",
  lblUnprotected:"Unprotected",

  lblIntf:"Interface",
  lblEther:"Etherlike",
  lblRmon:"RMON",
  lblAll:"All",
  lblMibType:"MIB Counter",
  lblTxPercent:"Transmit (%)",
  lblRxPercent:"Receive (%)"
},

<!--#ifdef CONFIG_SYS_PORT_SECURITY-->
/* Port Security */
lang_psecure:{
  titSettingTable:"Port Security Table",
  titPsecureMacTable:"Port Security Address Table",
  titPsecureMacAdd:"Add Port Security Address",
  titPsecureMacEdit:"Edit Port Security Address",
  titEditPsecure:"Edit Port Security",
  lblState:"State",
  lblRateLimit:"Rate Limit",
  lblPps:"Packet / Sec",
  lblAddrLimit:"Address Limit",
  lblViolateAction:"Violate Action",
  lblViolateProtect:"Protect",
  lblViolateRestrict:"Restrict",
  lblViolateShutdown:"Shutdown",
  lblSticky:"Sticky",
  lblTotalAddr:"Total",
  lblConfigAddr:"Configured",
  lblViolateNum:"Violate Number"
},
<!--#endif-->

/* QoS */
lang_qos:{
  lblQosMode:"State",
  lblTrustType:"Trust Mode",
  lblBasic:"Basic",
  titQosPortSettingTable:"Port Setting Table",
  titQosEditPortSetting:"Edit Port Setting",
  lblCoS:"CoS",
  lblCosDscp:"CoS-DSCP",
  lblTrustState:"Trust",
  lblRemark:"Remarking",
  lblRemarkCoS:"CoS",
  lblRemarkDSCP:"DSCP",
  lblRemarkIPP:"IP Precedence",

  titQueueTable:"Queue Scheduling Table",
  lblQueue:"Queue",
  lblScheduleMethod:"Method",
  lblStrictPriority:"Strict Priority",
  lblWrr:"WRR",
  lblWrrWeight:"Weight",
  lblWrrBandwidthPercent:"WRR Bandwidth (%)",

  titCos2QueueMap:"CoS to Queue Mapping",
  titQueue2CosMap:"Queue to CoS Mapping",

  titDscp2QueueMap:"DSCP to Queue Mapping",
  titQueue2DscpMap:"Queue to DSCP Mapping",
  lblDSCP:"DSCP",

  titIpp2QueueMap:"IP Precedence to Queue Mapping",
  titQueue2IppMap:"Queue to IP Precedence Mapping",
  lblIpp:"IP Precedence",

  titBwCtrlTable:"Ingress / Egress Port Table",
  titEditBwCtrl:"Edit Ingress / Egress Port",
  lblIgr:"Ingress",
  lblEgr:"Egress",
  lblState:"State",
  lblRate:"Rate (Kbps)",

  titEgrQueBwCtrlTable:"Egress Queue Table",
  titEditEgrBwQueCtrl:"Edit Egress Queue",
  lblQueueNumArg:"Queue $1",
  lblCIR:"CIR (Kbps)"
},

<!--#ifdef CONFIG_SYS_LIB_RADIUS-->
lang_radius:{
  lblUseDfltParam:"Use Default Parameter",
  lblRetries:"Retry",
  lblTimeout:"Timeout",
  lblDeadTime:"Dead Time",
  lblKeyStr:"Key String",
  lblKeyStrEncrypt:"Key String (Encrypted)",

  titRadiusSrv:"RADIUS Table",
  lblPri:"Priority",
  lblUsgType:"Usage",
  lblTypeLogin:"Login",
  lblTypeDot1x:"802.1X",
  lblTypeAll:"All",

  titRadiusSrvAdd:"Add RADIUS Server",
  titRadiusSrvEdit:"Edit RADIUS Server",
  lblUseDflt:"Use Default"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI-->
lang_rmon: {
  /* RMON Statistic */
  /* Per Port */
  lblBytesRec:"Bytes Received",
  lblDropEvents:"Drop Events",
  lblPktsRec:"Packets Received",
  lblBPktsRec:"Broadcast Packets Received",
  lblMPktsRec:"Multicast Packets Received",
  lblCRCAlignErr:"CRC & Align Errors",
  lblUndersizePkts:"Undersize Packets",
  lblOversizePkts:"Oversize Packets",
  lblFragments:"Fragments",
  lblJabbers:"Jabbers",
  lblCollisions:"Collisions",
  lblFrames64B:"Frames of 64 Bytes",
  lblFrames65127B:"Frames of 65 to 127 Bytes",
  lblFrames128255B:"Frames of 128 to 255 Bytes",
  lblFrames256511B:"Frames of 256 to 511 Bytes",
  lblFrames5121023B:"Frames of 512 to 1023 Bytes",
  lblFramesOver1024B:"Frames Greater than 1024 Bytes",

  /*  All Port */
  txtRmonStatisticsTable:"Statistics Table",
  txtRmonStatisticsIntf:"View Port Statistics",
  thBytesRec:"Bytes<br>Received",
  thDropEvents:"Drop<br>Events",
  thPktsRec:"Packets<br>Received",
  thBPktsRec:"Broadcast Packets<br>Received",
  thMPktsRec:"Multicast Packets<br>Received",
  thCRCAlignErr:"CRC & Align<br>Errors",
  thUndersizePkts:"Undersize<br>Packets",
  thOversizePkts:"Oversize<br>Packets",
  thFragments:"Fragments",
  thJabbers:"Jabbers",
  thCollisions:"Collisions",
  thFrames64B:"Frames of<br>64 Bytes",
  thFrames65127B:"Frames of<br>65 to 127 Bytes",
  thFrames128255B:"Frames of<br>128 to 255 Bytes",
  thFrames256511B:"Frames of<br>256 to 511 Bytes",
  thFrames5121023B:"Frames of<br>512 to 1023 Bytes",
  thFramesOver1024B:"Frames Greater<br>than 1024 Bytes",

  btnStatsIntfView:"View",
  msgConfirmClearCounter:"This operation will clear the interface statistics, the Etherlike statistics, and the RMON statistics for the selected interface.",

  /* History */
  /* Control Table */
  txtView:"View",
  txtHistoryControlTable:"History Table",
  txtHistoryLogTable:"View History",
  thHisIdxBR:"Entry",
  thSrcInf:"Port",
  thOwner:"Owner",
  thBuckets:"Sample",
  thReqBuckets:"Maximum",
  thCurrentBuckets:"Current",

  /* Add/Edit Dialog */
  titAddHistory:"Add History",
  titEditHistory:"Edit History",
  lblNewHisIdx:"Entry",
  lblEditHisIdx:"Entry",
  lblSrcIntf:"Port",
  lblReqBuckets:"Max Sample",
  lblInterval:"Sampling Interval",
  lblOwner:"Owner",

  /* History Table */
  txtHisIdx:"Entry",
  thOwner:"Owner",
  thSampleNum:"Sample<br>No.",
  thDropEvents:"Drop<br>Events",
  thBytesRec:"Bytes<br>Received",
  thPktssRec:"Packets<br>Received",
  thBPktsRec:"Broadcast<br>Packets",
  thMPktsRec:"Multicast<br>Packets",
  thCRCAlignErr:"CRC & Align<br>Errors",
  thUndersizePkts:"Undersize<br>Packets",
  thOversizePkts:"Oversize<br>Packets",
  thFragments:"Fragments",
  thJabbers:"Jabbers",
  thCollisions:"Collisions",
  thUtilization:"Utilization",

  /* Event */
  /* Control Table */
  txtEventControlTable:"Event Table",
  txtEventLogTable:"View Event Log",
  thEventIdx:"Entry",
  thCommunity:"Community",
  thDescription:"Description",
  thNotifiType:"Notification",
  thTime:"Time",
  thOwner:"Owner",

  /* Add/Edit Dialog */
  titAddEvent:"Add Event",
  titEditEvent:"Edit Event",
  lblNewEventIdx:"Entry",
  lblEditEventIdx:"Entry",
  lblCommunity:"Community",
  lblDescription:"Description",
  lblNotifiType:"Notification",
  lblNotifiTypeNone:"None",
  lblNotifiTypeLog:"Event Log",
  lblNotifiTypeTrap:"Trap",
  lblNotifiTypeLogTrap:"Event Log and Trap",
  lblOwner:"Owner",

  /* Event Table */
  thLogNum:"Log ID",
  thLogTime:"Time",
  thDescription:"Description",

  /* RMON Alarem */
  /* Control Table */
  txtAlarmControlTable:"Alarm Table",
  thAlarmIdx:"Entry",
  thIntf:"Port",
  thAlarmCnt:"Counter",
  thCntName:"Name",
  thCntValue:"Value",
  thSampleType:"Sampling",
  thAlarmRising:"Rising",
  thAlarmFalling:"Falling",
  thAlarmThreshold:"Threshold",
  thAlarmEvent:"Event",
  thStartupAlarm:"Trigger",
  thInterval:"Interval",
  thOwner:"Owner",

  /* Add/Edit Dialog */
  titAddAlarm:"Add Alarm",
  titEditAlarm:"Edit Alarm",
  lblNewAlarmIdx:"Entry",
  lblEditAlarmIdx:"Entry",
  lblIntf:"Port",
  lblCntName:"Counter",
  lblCntValue:"Value",
  lblSampleType:"Sampling",
  lblNone:"None",
  lblAbsolute:"Absolute",
  lblDelta:"Delta",
  lblStartupAlarm:"Trigger",
  lblRising:"Rising",
  lblFalling:"Falling",
  lblRisingAndFalling:"Rising and Falling",
  lblInterval:"Interval",
  lblOwner:"Owner",

  lblBytesRec:"Received Bytes (Octets)",
  lblDropEvents:"Drop Events",
  lblPktsRec:"Received Packets",
  lblBPktsRec:"Broadcast Packets Received",
  lblMPktsRec:"Multicast Packets Received",
  lblCRCAlignErr:"CRC & Align Errors",
  lblUndersizePkts:"Undersize Packets",
  lblOversizePkts:"Oversize Packets",
  lblFragments:"Fragments",
  lblJabbers:"Jabbers",
  lblCollisions:"Collisions",
  lblFrames64B:"Frames of 64 Bytes",
  lblFrames65127B:"Frames of 65 to 127 Bytes",
  lblFrames128255B:"Frames of 128 to 255 Bytes",
  lblFrames256511B:"Frames of 256 to 511 Bytes",
  lblFrames5121023B:"Frames of 512 to 1023 Bytes",
  lblFramesOver1024B:"Frames Greater than 1024 Bytes",
  txtRmonSnmpDisabledInfo:"The SNMP service is currently disabled.<br> For RMON configuration to be effective, the $1 SNMP service $2 must be enabled.",
  txtRmonEventDisabledInfo:"For RMON Alarm configuration, the $1 RMON Event $2 must be added.",

  /* error message */
  errHistoryTableOverflow:"RMON history table is full",
  errAlarmTableOverflow:"RMON alarm table is full",
  errEventTableOverflow:"RMON event table is full"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_L3_ROUTE-->
lang_ipconf:{
  titAddIPv4Intf:"Add IPv4 Interface",
  titEditIPv4Intf:"Edit IPv4 Interface",
  titAddIPv6Intf:"Add IPv6 Interface",
  titEditIPv6Intf:"Edit IPv6 Interface",
  titAddIPv4Route:"Add IPv4 Static Route",
  titEditIPv4Route:"Edit IPv4 Static Route",
  titAddIPv6Route:"Add IPv6 Static Route",
  titEditIPv6Route:"Edit IPv6 Static Route",
  titAddIPv4Arp:"Add ARP",
  titEditIPv4Arp:"Edit ARP",
  titAddIPv6Neigh:"Add Neighbor",
  titEditIPv6Neigh:"Edit Neighbor",

  lblIPv4IntfTable:"IPv4 Interface Table",
  lblIntf:"Interface",
  lblIPAddrType:"IP Address Type",
  lblIPAddr:"IP Address",
  lblIPv6Addr:"IPv6 Address",
  lblMacAddr:"MAC Address",
  lblMask:"Mask",
  lblStatus:"Status",
  lblRoles:"Roles",
  txtDynamic:"Dynamic",
  txtStatic:"Static",
  txtDHCP:"DHCP",
  txtStatusTentative:"Tentative",
  txtStatusValid:"Valid",
  txtStatusValidDuplicated:"Valid Duplicated",
  txtStatusDuplicated:"Duplicated",
  txtStatusDelayed:"Delayed",
  txtStatusNotReceived:"Not Received",
  txtIpPrimary:"primary",
  txtIpSub:"sub",
  txtRip:"Rip",
  txtRipng:"Ripng",
  txtOspf:"Ospf",
  txtOspf6:"Ospf6",
  txtIsis:"Isis",
  txtBgp:"Bgp",
  
  lblLoopback:"Loopback",
  lblAddressType:"Address Type",
  lblMask:"Mask",
  lblNetworkMask:"Network Mask",
  lblPrefixLength:"Prefix Length",

  lblIPv4RouteTable:"IPv4 Routing Table",
  lblDstIP:"Destination IP",
  lblDstIPPrefix:"Destination IP Prefix",
  lblRouteType:"Route Type",
  lblNextHopAddr:"Next Hop Router IP Address",
  lblRouteOwner:"Route Owner",
  lblMetric:"Metric",
  lblAdminDistance:"Administrative Distance",
  lblOutIntf:"Outgoing Interface",

  txtDefault:"Default",
  txtLocal:"Local",
  txtReject:"Reject",
  txtRemote:"Remote",
  txtDirectlyConnected:"Directly Connected",
  txtDHCP:"DHCP",

  lblIPv4ArpTable:"ARP Table",
  lblArpAgingTime: "ARP Entry Age Out",
  lblArpClear: "Clear ARP Table Entries",
  lblDynamic:"Dynamic",
  lblStatic:"Static",
  lblNormalAging:"Normal Age Out",
  lblStatus:"Status",
  txtIPv4ArpIntf: "Note: Only interfaces with an valid IPv4 address are available for selection",
  lblV4ArpProxy: "Arp Proxy",

  lblIPv6IntfTable:"IPv6 Interface Table",
  lblIPv6Routing:"IPv6 Unicast Routing",
  lblDhcpv6Client:"DHCPv6 Client",
  lblStateless:"Stateless",
  tblRefresh:"Information<br>Refresh Time",
  lblRefresh:"Information Refresh Time",
  tblRefreshMin:"Minimum Information<br>Refresh Time",
  lblRefreshMin:"Minimum Information Refresh Time",
  lblAutoConf:"Auto Configuration",
  lblDadAttemps:"DAD Attempts",
  tblSendICMPUnreachMsg:"Send ICMPv6<br>Unreachable",
  lblSendICMPUnreachMsg:"Send ICMPv6 Unreachable",
  lblGlobal:"Global",
  lblLinkLocal:"Link Local",
  lblMcast:"Multicast",
  lblIPv6RouteTable:"IPv6 Routing Table",
  lblIPv6Prefix:"IPv6 Prefix",
  lblIPv6PrefixLen:"IPv6 Prefix Length",
  lblNextHopType:"Next Hop Type",

  lblIPv6AddrTable:"IPv6 Address Table",
  lblIPv6AddrType:"IPv6 Address Type",
  lblIPv6Addr:"IPv6 Address",
  lblDADStatus:"DAD Status",
  lblTentative:"Tentative",
  lblDuplicate:"Duplicate",
  lblActive:"Active",
  lblEUI64:"EUI-64",

  lblIPv6NeighTable:"IPv6 Neighbor Table",
  lblNeighClear: "Clear Neighbor Table",
  lblIPv6Router:"Router"

},
<!--#endif-->

<!--#ifdef CONFIG_SYS_UI_SNMP-->
/*SNMP*/
lang_snmp:{

  <!--#ifdef CONFIG_SYS_SNMPV3-->
  titSnmpViewTable:"View Table",
  titAddSnmpView:"Add View",
  lblViewName:"View",
  lblObjectIDSubtree:"OID Subtree",
  lblObjectIDSubtreeView:"Type",
  lblIncluded:"Included",
  lblExcluded:"Excluded",

  titSnmpGroupTable:"Group Table",
  titAddSnmpGroup:"Add Group",
  titEditSnmpGroup:"Edit Group",
  lblGroupName:"Group",
  lblSecurityModel:"Version",
  lblSecurityLevel:"Security Level",
  lblNoAuth:"No Security",
  lblAuth:"Authentication",
  lblPriv:"Authentication and Privacy",
  lblView:"View",
  lblRead:"Read",
  lblWrite:"Write",
  lblNotify:"Notify",
  lblSNMPv3:"SNMPv3",
  lblNoAuthNoPriv:"No Authentication and No Privacy",
  lblAuthNoPriv:"Authentication and No Privacy",
  lblAuthPriv:"Authentication and Privacy",

  lblcommType:"Type",
  lblBasic:"Basic",
  lblAdvanced:"Advanced",

  titSnmpUserTable:"User Table",
  titAddSnmpUser:"Add User",
  titEditSnmpUser:"Edit User",
  lblUserName:"User",
  lblAuthTitle:"Authentication",
  lblAuthProt:"Method",
  lblAuthPswd:"Password",
  lblEncryTitle:"Privacy",
  lblEncryProt:"Method",
  lblEncryPswd:"Password",
  lblMD5:"MD5",
  lblSHA:"SHA",
  lblDES:"DES",
  lblNone:"None",

  lbllocalEngineID:"Local Engine ID",
  lblEngineID:"Engine ID",
  titRemtEngineID:"Remote Engine ID Table",
  titAddRemtEngineID:"Add Remote Engine ID",
  titEditRemtEngineID:"Edit Remote Engine ID",

  lblSnmpSecName:"Community / User",
  lblTimeout:"Timeout",
  lblRetries:"Retry",
  lblUseDflt:"Use Default",

  titEditSnmpCommunity:"Edit Community",

  txtGrpNeedViewInfo:"Configure $1 SNMP View $2 to associate a non-default view with a group.",
  txtComNeedGrpInfo:"The access right of a community is defined by a group under advanced mode.<br>Configure $1 SNMP Group $2 to associate a group with a community.",
  txtUserNeedGrpInfo:"Configure $1 SNMP Group $2 to associate an SNMPv3 group with an SNMPv3 user.",
  txtNotifyNeedSecInfo:"For SNMPv1,2 Notification, $1 SNMP Community $2 needs to be defined.<br>For SNMPv3 Notification, $3 SNMP User $4 must be created.",

  errMIBOIDNotValid:"MIB OID is not valid",
  errUserAuthPwLenMin:"Authentication password length must be equal or greater than 8",
  errUserPrivPwLenMin:"privacy password length must be equal or greater than 8",
  errNotEngineID:"Value is an illegal Engine ID",
  errComNoGrp:"There is no SNMPv1,2 group to be used by a community",
  errUserNoGrp:"There is no SNMPv3 group to be used by a user",
  errNotifyNoUser:"There is no user to be used by an SNMPv3 notification",
  <!--#endif-->

  txtNotifyNeedComInfo:"For SNMPv1,2 Notification, $1 SNMP Community $2 needs to be defined.",
  errNotifyNoCom:"There is no community to be used by a notification",

  titSnmpCommunityTable:"Community Table",
  titAddSnmpCommunity:"Add Community",
  lblCommunityName:"Community",
  lblSNMPv1:"SNMPv1",
  lblSNMPv2:"SNMPv2",
  lblAccess:"Access",
  lblReadOnly:"Read-Only",
  lblReadWrite:"Read-Write",

  lblAuthFail:"Authentication Failure",
  lblLinkUpDown:"Link Up / Down",
  lblColdStart:"Cold Start",
  lblWarmStart:"Warm Start",

  titSnmpNotifyTable:"Notification Table",
  titAddSnmpNotify:"Add Notification",
  titEditSnmpNotify:"Edit Notification",
  lblNotifyType:"Type",
  lblTraps:"Trap",
  lblInforms:"Inform",
  lblSnmpVer:"Version"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_STORM_CTRL-->
/* Storm Control */
lang_storm: {
  lblUnit:"Mode",
  lblpps:"Packet / Sec",
  lblbps:"Kbits / Sec",
  txtpps:"pps",
  txtbps:"Kbps",
  lblIFG:"IFG",
  txtExclude:"Exclude",
  txtInclude:"Include",
  titStormPortSettingTable:"Port Setting Table",
  titStormEditPortSetting:"Edit Port Setting",
  lblState:"State",
  lblRate:"Rate",
  lblBroadcast:"Broadcast",
  lblUnknownMul:"Unknown Multicast",
  lblUnknownUni:"Unknown Unicast",
  lblAction:"Action",
  txtDrop:"Drop",
  txtShutdown:"Shutdown"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTO_STP-->
/* Spanning Tree */
lang_stp:{
  lblTrue:"True",
  lblFalse:"False",
  lblState:"State",
  lblOperMode:"Operation Mode",
  lblPathCost:"Path Cost",
  lblBpduFwd:"BPDU Handling",
  lblPriority:"Priority",
  lblHelloTime:"Hello Time",
  lblMaxAge:"Max Age",
  lblFwdDelay:"Forward Delay",
  lblTxHoldCnt:"Tx Hold Count",
  lblBridgeId:"Bridge Identifiter",
  lblRootBridgeId:"Designated Root Bridge",
  lblRootPort:"Root Port",
  lblRootCost:"Root Path Cost",
  lblRegionRootBridge:"Regional Root Bridge",
  lblInternalRootCost:"Internal Path Cost",
  lblRemainingHop:"Remaining Hop",
  lblTcCnt:"Topology Change Count",
  lblTcLastTime:"Last Topology Change",
  msgStpModeChange:"Change STP mode might cause the connection interrupted.<br>Do you want to continue?",

  lblPortAdminStatus:"State",
  lblPortPathCost:"Path Cost",
  lblPortPriority:"Priority",
  lblPortEdge:"Edge Port",
  lblPortEdgeOper:"Operational Edge",
  lblPortBpduFilter:"BPDU Filter",
  lblPortBpduGuard:"BPDU Guard",
  lblPortP2P: "Point-to-Point",
  lblPortP2POper: "Operational Point-to-Point",
  lblPortMode:"Mode",
  lblPortType:"Type",
  lblPortRole:"Port Role",
  lblPortState:"Port State",
  lblPortDesignatedBridge:"Designated Bridge",
  lblPortDesignatedPortId:"Designated Port ID",
  lblPortDesignatedCost:"Designated Cost",

  lblRxCnt:"Receive BPDU",
  lblTxCnt:"Transmit BPDU",
  lblConfigBpdu:"Config",
  lblTcnBpdu:"TCN",

  txtSTP: "STP",
  txtRSTP: "RSTP",
  txtOperModeSTP: "STP",
  txtOperModeRSTP: "RSTP",
  txtPathCostLong: "Long",
  txtPathCostShort: "Short",
  txtBpduFwdFiltering: "Filtering",
  txtBpduFwdFlooding: "Flooding",
  txtDayHrMinSecArg:"$1D/$2H/$3M/$4S",

  txtStpIntfStats:"STP Port Statistic",
  txtStpIntfSettingTable:"Port Setting Table",
  txtStpIntfStatsTable:"Statistics Table",

  titStpIntfEdit:"Edit Port Setting",

  txtMaster:"Master",
  txtRoot:"Root",
  txtDesignated:"Designated",
  txtAlternate:"Alternate",
  txtBackup:"Backup",
  txtDisabled:"Disabled",
  txtBlocking:"Blocking",
  txtListening:"Listening",
  txtDiscarding:"Discarding",
  txtLearning:"Learning",
  txtForwarding:"Forwarding",

  btnProtocolMigration:"Protocol Migration Check",
  btnStatsIntfView:"View",

  errInvalidPriority:"Priority should be between $1 and $2 in steps of $3.",

<!--#ifdef CONFIG_SYS_PROTO_STP_MSTP-->
  txtMSTP: "MSTP",
  txtOperModeMSTP: "MSTP",

  lblRegionName:"Region Name",
  lblRevision:"Revision",
  lblMaxHop:"Max Hop",
  lblMsti:"MSTI",
  lblMstpBpdu:"MSTP",

  titStpMstiEdit:"Edit MST Instance Setting",
  titStpMstiIntfEdit:"Edit MST Port Setting",

  txtMstiTable:"MST Instance Table",
  txtMstIntfSettingTable:"MST Port Setting Table",
  txtInternal:"Internal",
  txtBoundary:"Boundary",
<!--#endif-->

  txtEnd:"end"
},
<!--#endif-->

/* System */
lang_sys:{
  titHome:"Intelligent Switch",
  lblSysinfo:"System Information",
  titEditSysinfo:"Edit System Information",
  lblModel:"Model",
  lblSysName:"System Name",
  lblSysLocation:"System Location",
  lblSysContact:"System Contact",
  lblSysSN:"Serial Number",
  lblMacAddress:"MAC Address",
  lblLoaderVer:"Loader Version",
  lblLoaderDate:"Loader Date",
  lblFwVer:"Firmware Version",
  lblFwDate:"Firmware Date",
  lblSysObjID:"System OID",
  lblSysUpTime:"System Uptime",
  lblSysCurrTime:"Current Time",
  txtSysUptimeArg:"$1 day, $2 hr, $3 min and $4 sec",
  lblTime:"Time",

  lblNetworkIPv4:"IPv4 Address",
  lblNetworkSubIPv4:"Sub IPv4 Address",
  lblSubIPAddress:"IP Address",
  lblSubMask:"Subnet Mask",
  lblNetworkIPv6:"IPv6 Address",
  lblIPAddressType:"Address Type",
  lblDynamic:"Dynamic",
  lblStatic:"Static",
  lblIPAddress:"IP Address",
  lblMask:"Subnet Mask",
  lblAdminGateway:"Default Gateway",
  lblDnsArg:"DNS Server $1",
  lblIPv4Addr:"IPv4 Address",
  lblIPv4Gateway:"IPv4 Default Gateway",
  lblPrefixLength:"Prefix Length",
  lblIPv6AddrAutoConfig:"Auto Configuration",
  lblDHCPv6Client:"DHCPv6 Client",
  lblIPv6Addr:"IPv6 Address",
  lblIPv6Gateway:"IPv6 Gateway",
  lblIPv6DefaultGateway:"IPv6 Default Gateway",
  lblLinkLocalAddress:"Link Local Address",
  lblIPv6InUseAddress:"IPv6 Address In-Use",
  lblIPv6InUseGateway:"IPv6 Gateway In-Use",
  lblStateless:"Stateless",
  lblInfoRefreshTime:"Information Refresh Time",
  lblMinInfoRefreshTime:"Minimum Information Refresh Time",
  lblInfinite:"Infinite",

  titUserAccountTable:"User Account",
  titAddUserAccount:"Add User Account",
  titEditUserAccount:"Edit User Account",
  lblUserName:"Username",
  lblPwd:"Password",
  lblPwdConfirm:"Confirm Password",
  lblPwdEncrypt:"Encrypted",
  lblPwdText:"Clear Text",
  lblPwdNone:"No Password",
  lblPriv:"Privilege",
  lblPrivTypeAdmin:"Admin",
  lblPrivTypeUser:"User",
  lblPrivTypeOther:"Other",
  errConfirmPassword:"The Password and Confirm Password fields must be identical.",
  errInvalidASCII:"Password is invalid ASCII characters.",
  errUserExist:"Entry already exists.",
  errUserNotExist:"The user is not exist.",
  errMaxNumUserArg:"The maximum number of local users supported is $1.",
  errDeleteDeafultUser:"Default user name cannot be deleted.",

  lblSave:"Save",
  lblLogout:"Logout",
  msgConfirmSave:"Save running configuration to startup configuration. Do you want to continue?",
  msgConfirmReboot:"Reboot the system and unsaved changes in the configuration will be lost. Do you want to continue?",
  msgConfirmLogout:"Logout will disconnect your current session. Do you want to continue?",
  msgConfirmSupport:"Backup tech-support file to local PC. It will take few seconds to backup depends on your configuration. Do you wnat to continue?",
  msgRebooting:"System is rebooting...",
  msgSaveFail:"Save Fail",
  msgSupportFail:"Backup tech-support file fail",

  btnReboot:"Reboot",
  btnRestore:"Restore Factory Default",
  btnSupport:"Support",
  btnLanguage:"Chinese",
  btnDebug:"Debug",
  msgConfirmRestore:"Restore to factory defaults. You need to reboot the device to take effect. Do you want to continue?",
  msgConfirmRestoreReboot:"Reboot the device to take effect now. Do you want to continue?"
},

<!--#ifdef CONFIG_SYS_LIB_TACPLUS-->
lang_tacplus:{
  lblUseDfltParam:"Use Default Parameter",
  lblTimeout:"Timeout",
  lblKeyStr:"Key String",
  lblKeyStrEncrypt:"Key String (Encrypted)",
  titTacplusSrv:"TACACS+ Table",
  lblPri:"Priority",
  titTacplusSrvAdd:"Add TACACS+ Server",
  titTacplusSrvEdit:"Edit TACACS+ Server",
  lblUseDflt:"Use Default"
},
<!--#endif-->

lang_time:{
  lbltimeSrc:"Source",
  lbltimeSntp:"SNTP",
  lblSntpServerPort:"Server Port",
  lbltimeHost:"From Computer",
  lbltimeManual:"Manual Time",
  lbltimeTZ:"Time Zone",
  lbltimeType:"Type",
  lbltimeDST:"Daylight Saving Time",
  lbltimeDSO:"Offset",
  lbltimeDSNone:"None",
  lbltimeDSRec:"Recurring",
  lbltimeDSNRec:"Non-recurring",
  lbltimeDSUSA:"USA",
  lbltimeDSEU:"Europen",
  lbltimeFrom:"From",
  lbltimeTo:"To",
  lbltimeNrecF:"Non-recurring From",
  lbltimeNrecT:"Non-recurring To",
  lbltimeDevice:"Current Time",
  lblDay:"Day",
  lblWeek:"Week",
  lblMonth:"Month",
  lblDate:"Date",
  lblTime:"Time",
  lblSun:"Sun",
  lblMon:"Mon",
  lblTue:"Tue",
  lblWed:"Wed",
  lblThu:"Thu",
  lblFri:"Fri",
  lblSat:"Sat",
  lblFirst:"First",
  lblLast:"Last",
  lblJan:"Jan",
  lblFeb:"Feb",
  lblMar:"Mar",
  lblApr:"Apr",
  lblMay:"May",
  lblJun:"Jun",
  lblJul:"Jul",
  lblAug:"Aug",
  lblSep:"Sep",
  lblOct:"Oct",
  lblNov:"Nov",
  lblDec:"Dec",
  lbltimeState:"State",
  lbltimePortSettingTable:"Port Setting Table",
  lbltimePortEditSetting:"Edit Port Setting",
  errMsgDateFormat:"Date value format is not correct",
  errMsgTimeFormat:"Time value format is not correct",
  errMsgDLSEarlier:"Start date must be earlier than end date.",
  errMsgDLSGreaterOffset:"The difference between daylight saving From and To must be greater than Time Set Offset",
  lblHour:"Hour",
  lblMinute:"Minute",
  lblSecond:"Second"
},


<!--#ifdef CONFIG_SYS_PROTO_UDLD-->
lang_udld: {
  titUdldPortSettingTable:"Port Setting Table",
  titUdldEditPortSetting:"Edit Port Setting",
  titTableUdldNeighbor:"Neighbor Table",
  lblUdldMsgTime:"Message Time",
  lblPortMode:"Mode",
  lblUdldPortDisable:"Disabled",
  lblUdldPortNormal:"Normal",
  lblUdldPortAggresive:"Aggressive",
  lblUdldBidirectionalState:"Bidirectional State",
  lblUdldOperState:"Operational Status",
  lblUdldNeighbors:"Neighbor",
  lblUdldMsgInterval:"Message<br>Interval",
  lblUdldTimeoutInterval:"Timeout<br>Interval",
  lblUdldExpirationTime:"Expiration<br>Time",
  lblUdldcurNeistate:"Current Neighbor State",
  lblUdldDeviceID:"Device ID",
  lblUdldDeviceName:"Device Name",
  lblUdldPortID:"Port ID",
  lblUdldBiState_0:"Unknown",
  lblUdldBiState_1:"transmit/receive loop",
  lblUdldBiState_2:"Neighbor's echo is empty",
  lblUdldBiState_3:"Mismatch with neighbor state reported",
  lblUdldBiState_4:"Bidirectional",
  lblUdldBiState_5:"Unidirectional",
  lblUdldOperState_0:"Link down",
  lblUdldOperState_1:"",
  lblUdldOperState_2:"Link up",
  lblUdldOperState_3:"Advertisement",
  lblUdldOperState_4:"Detection",
  lblUdldOperState_5:"Extended Detection",
  lblUdldOperState_6:"Advertisement - SINGLE NEIGHBOR DETECTED",
  lblUdldOperState_7:"Advertisement - MULTIPLE NEIGHBORS DETECTED",
  lblUdldOperState_8:"Disabled port",
  lblUdldOperState_9:"Link up"
},
<!--#endif-->

/* VLAN */
lang_vlan:{
  titVLAN:"VLAN Table",
  titVLANConfigurationTable:"VLAN Configuration Table",
  titVLANPortSettingTable:"Port Setting Table",
  titVLANMembershipSettingTable:"Membership Table",
  titVLANEdit:"Edit VLAN",
  titVLANNameEdit:"Edit VLAN Name",
  titVLANPortEdit:"Edit Port Setting",
  lblVlanName:"Name",
  lblVlanMode:"Mode",
  lblVlanType:"Type",
  lblVlanIntfState:"VLAN Interface State",
  lblVlanMembership:"Membership",
  lblVlanPVID:"PVID",
  lblVlanForbidden:"Forbidden",
  lblVlanExcluded:"Excluded",
  lblVlanTagged:"Tagged",
  lblVlanUntagged:"Untagged",
  lblVlanHybrid:"Hybrid",
  lblVlanTrunk:"Trunk",
  lblVlanAccess:"Access",
  lblVlanTunnel:"Tunnel",
  lblVlanAcceptFrameType:"Accept Frame Type",
  lblVlanIngressFiltering:"Ingress Filtering",
  lblVlanUplink:"Uplink",
  lblVlanTPID:"TPID",
  lblVlanAll:"All",
  lblVlanTagOnly:"Tag Only",
  lblVlanUntagOnly:"Untag Only",
  lblVlanDefault:"Default",
  lblVlanAdmin:"Administrative VLAN",
  lblVlanOper:"Operational VLAN",
  lblCreatedVlans:"Created VLAN",
  lblManagementVLAN:"Management VLAN",

  errMsgManagementVLAN:"Note: Change Management VLAN may cause connection interrupted"
},

<!--#ifdef CONFIG_SYS_MACVLAN-->
/* VLAN Mac */
lang_vlanmac:{
  titMVGroupTable:"MAC Group Table",
  titMVAddGroup:"Add MAC Group",
  titMVEditGroup:"Edit MAC Group",
  titMVBindingTable:"Group Binding Table",
  titMVAddBinding:"Add Group Binding",
  titMVEditBinding:"Edit Group Binding",
  lblMVMac:"MAC Address",
  lblMVMacMask:"Mask",
  lblMVGroup:"Group ID",
  msgMVHybridPort:"Note: Only VLAN Hybrid port can be set MAC VLAN"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_VOICEVLAN-->
/* VLAN Voice */
lang_vlanvoice:{
  titVVPortSettingTable:"Port Setting Table",
  titVVEditPortSettingTable:"Edit Port Setting",
  titVVOUITable:"Voice OUI Table",
  titVVOUIAdd:"Add Voice OUI",
  titVVOUIEdit:"Edit Voice OUI",
  lblVVState:"State",
  lblVVRemark:"CoS / 802.1p&nbsp<br>Remarking",
  lblVVAgingTime:"Aging Time",
  lblVVPortState:"State",
  lblVVPortMode:"Mode",
  lblVVPortCosMdoe:"QoS Policy",
  lblVVAuto:"Auto",
  lblVVManual:"Manual",
  lblVVAll:"All",
  lblVVSource:"Voice Packet",
  lblVVOUIAddress:"OUI",
  lblVVDescription:"Description"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_PROTOCOL_VLAN-->
/* VLAN Protocol */
lang_vlanprotocol:{
  titPVGroupTable:"Protocol Group Table",
  titPVAddGroup:"Add Protocol Group",
  titPVEditGroup:"Edit Protocol Group",
  titPVBindingTable:"Group Binding Table",
  titPVAddBinding:"Add Group Binding",
  titPVEditBinding:"Edit Group Binding",
  lblPVProtocolValue:"Protocol Value",
  lblPVFrameType:"Frame Type",
  lblPVGroup:"Group ID",
  lblPVEII:"Ethernet_II",
  lblPVLLC:"IEEE802.3_LLC_Other",
  lblPVSNAP:"RFC_1042",
  msgPVHybridPort:"Note: Only VLAN Hybrid port can be set Protocol VLAN"
},
<!--#endif-->
<!--#ifdef CONFIG_SYS_-->
/* VLAN IP subnet */
lang_vlanIPsubnet:{
  titIPsubnetGroupTable:"VLAN IP Subnet Group Table",
  titIpSubnetAddGroup:"Add IP Subnet Group",
  titIpSubnetEditGroup:"Edit IP Subnet Group",
  titIpSubnetBindingTable:"Group Binding Table",
  titIpSubnetAddBinding:"Add Group Binding",
  titIpSubnetEditBinding:"Edit Group Binding",
  lblIPSubMask:"IPv4 Subnet Mask",
  lblIPSubAddr:"IPv4 Subnet Address",
  lblIPSubGroup:"Group ID",
  lblIPSubVlanId:"Vlan ID",
  lblIPSubPri:"Priority",
  lblIPSubBindPort:"Bind Port",
  msgIPSubHybridPort:"Note: Only VLAN Hybrid port can be set IP subnet VLAN and bound to a group",
  msgIPSubBindPort:"Note: Only one port can be bound to a group"
},
<!--#endif-->
<!--#ifdef CONFIG_SYS_SURVEILLANCEVLAN-->
/* Surveillance Voice */
lang_vlansurveillance:{
  titSVPortSettingTable:"Port Setting Table",
  titSVEditPortSettingTable:"Edit Port Setting",
  titSVOUITable:"Surveillance OUI Table",
  titSVOUIAdd:"Add Surveillance OUI",
  titSVOUIEdit:"Edit Surveillance OUI",
  lblSVState:"State",
  lblSVRemark:"CoS / 802.1p&nbsp<br>Remarking",
  lblSVAgingTime:"Aging Time",
  lblSVPortState:"State",
  lblSVPortMode:"Mode",
  lblSVPortCosMdoe:"QoS Policy",
  lblSVAuto:"Auto",
  lblSVManual:"Manual",
  lblSVAll:"All",
  lblSVSource:"Video Packet",
  lblSVOUIAddress:"OUI",
  lblSVDescription:"Description"
},
<!--#endif-->

<!--#ifdef CONFIG_SYS_POE-->
/* POE */
lang_poe:{
  titPoeSummary:"System info",
  titPoePortSettingTable:"Port Setting Table",
  titPoeEditPortSetting:"Edit Port Setting",
  lblPoeDevPower:"System Power(W)",
  lblPoeDevTemp:"System Temperature(C)",
  lblPoeEntryNo:"Entry",
  lblPoePort:"Port",
  lblPoePortEnable:"PortEnable",
  lblPoePortStatus:"Status",
  lblPoePortType:"Type",
  lblPoePortLevel:"Level",
  lblPoePortPowerLimit:"Power Limit(W)",
  lblPoePortPower:"Actual Power(W)",
  lblPoePortVoltage:"Voltage(V)",
  lblPoePortCurrent:"Current(mA)",
  lblPoePortWatchDog:"WatchDog",
  txtOn:"On",
  txtOff:"Off"
},
<!--#endif-->
lang_sysError:{
	<!--port err start-->
	errPortAuto:"Port do not allow auto negotiation",
	errFiberDuplex:"Fiber port duplex invalid",
	errPortAutoSpeed:"Port speed capability invalid",
	errPortSpeed:"Port speed invalid",
	errPortDuplex:"Port duplex invalid",
	errPort1000NO:"Fast Ethernet port does not support 1000M",
	errPort1000HalfNO:"Giga Ethernet port does not support 1000/Half",
	errFiberSpeed:"Fiber port speed invalid",
	errPortNoVlan:"Port is not the member port of the VLAN",
	errFiberNoEEE:"Fiber port do not support EEE function",
	errPortNoFlowCtrl:"Flow Control AutoNegotiation is not supported on combo or fiber ports",
	<!--port err end-->
	<!--trunk err start-->
	errTrunkid:"Invalid trunk ID",
	errTrunkPorts:"Invalid trunk member ports",
	errTrunkHash:"Invalid trunk hash algorithm",
	errTrunkGroup:"Can not add static trunk member to LACP trunk",
	errTrunkLess:"2 trunk member ports at least",
	errTrunkMore4:"4 trunk member ports at most" ,
	errTrunkMore8:"8 trunk member ports at most",
	errTrunkMore16:"16 trunk member ports at most",
	errTrunkAuth:"Port should not be authentication enabled",
	errTrunkAnother:"Port already in another trunk",
	errTrunkVlan:"Joined VLAN/Tag of trunk member ports are different",
	errTrunkMirror: "Mirroring port cannot be a trunk member port",
	errTrunkRate:"Port should not be ingress/egress rate limited",
	errTrunkNego:"Trunk member ports must all have the same negotiation mode",
	errTrunkSpeed:"Trunk member ports must all have the same speed",
	errTrunkDuplex:"Trunk member ports must all be full duplex",
	errTrunkNoExist:"Trunk does not exist",
	errTrunkBackupPort:"Port already used as trunk member",
	errTrunkSecurity:"Port should not be port security enabled",
	errTrunkEmpty:"No member ports in trunk",
	<!--port err end-->
	<!--vlan err start-->
	errVlanDef:"Not allowed to delete Default VLAN",
	errVlanExists:"VLAN exists",
	errVlanFrame:"Invalid VLAN frame type",
	errVlanID:"VID is out of range",
	errVlanMGMT:"A Management VLAN can not be configured as Default VLAN",
	errVlanMemvership:"VLAN membership error",
	errVlanMGMTDel:"Not allowed to delete Management VLAN",
	errVlanMGMTempty:"Management VLAN can not be created because member port is empty",
	errVlanNameFull:"Bad VLAN name length",
	errVlanNameExist:"VLAN name exists",
	errVlanNotExist:"VLAN does not exist",
	errVlanNotStatic:"VLAN is not static",
	errVlanNotCreated:"VLAN was not created by user",
	errVlanTrunk:"Port belongs to a port-channel",
	errVlanuplink:"Port mode is not uplink port",
	errVlanMVRDel:"MVR VLAN Can not be allowed to destory",
	errVlanNotSupport:"VLAN over max entry number",
	errVoiceID:"A Voice VLAN can not be configured as Default VLAN",
	errVoiceOUIexist:"Voice VLAN OUI entry is exist in database",
	errVoiceOUImcast:"OUI MAC must be unicast",
	errVoiceOUIzero:"Voice VLAN OUI entry is all zero",
	errVoiceauto:"Voice VLAN auto mode port can not be edited in Voice VLAN",
	errVoiceOUIport:"Voice VLAN State is not OUI-enabled",
	errVoiceportMode:"Port must be in hybrid or trunk mode",
	errVoicePortStaic:"static member cannot be a candidate in a Voice VLAN",
	errVoiceOUIenable:"Voice VLAN  has enabled ports ",
	errVoiceOUIdef:"A Default VLAN can not be configured as Voice VLAN in OUI mode",
	errVoiceVidDel:"Voice VLAN Can not be allowed to delete",
	errVoiceGustVlan:"A guest VLAN can not be configured as voice VLAN",
	errVoiceSurveillance:"A surveillance VLAN can not be configured as voice VLAN",
	errVoiceVidNotExist:"Can't enable voice VLAN becasue voice VLAN ID isn't assgined",
	errVoiceVidEnable:"Can't restore voice VLAN ID to none becasue voice VLAN is enabled",
	errProvlanPort:"Protocol VLAN : the Protocol VLAN is configured in the port",
	errProvlanPortMode:"Port must be in hybrid Mode",
	errProvlanEnablePort:"Port has bound Protocol VLAN Group",
	errVlanMac:"Number of ranges exceeds the maximum. ",
	errVlanMacNotUni:"Range contains Multicast addresses.",
	errVlanMacOverlap:"The range or part of it is already defined in the database.",
	errVlanMacLastDel:"Cannot remove last range from mapped group.",
	errVlanMacNoResource:"There are no resources for this range.",
	errVlanMacNoGid:"Attempting to map illegal group number.",
	errVlanMacGeneral:"Port mode is not hybrid.",
	errVlanMacPortMode:"Enable MAC VLAN port must be in hybrid mode",
	errSVOUIMcast:"OUI MAC must be Mcast",
	errSVOUIExist:"Surveillance VLAN OUI entry is exist in database",
	errSVOUIzero:"Surveillance VLAN OUI can't be all zero",
	errSVGuest:"A Guest VLAN can not be configured as Surveillance VLAN",
	errSVVoice:"A Voice VLAN can not be configured as Surveillance VLAN",
	errSVDefailt:"A Default VLAN can not be configured as Surveillance VLAN",
	errSVNotExist:"Can't enable Surveillance VLAN becasue Surveillance VLAN ID isn't assgined",
	errSVVidEnable:"Can't restore Surveillance VLAN ID to none becasue Surveillance VLAN is enabled",
	errSVEnablePort:"Surveillance VLAN has enabled ports" ,
	errSVPortMode:"Port must be in hybrid or trunk mode",
	errSVPortStatic:"Port can't be static member port of a Surveillance VLAN",
	errSVGlobalDisable:"Surveillance VLAN global state is not enabled",
	errVidSV:"A Surveillance VLAN can not be configured as Default VLAN",
	errSVVidDel:"Surveillance VLAN Can not be allowed to destory",
	errSVUsedPort:"Port in surveillance VLAN auto mode can not be edited in VLAN",
	<!--vlan err end-->
	<!--MGMT ACL start-->
	errMGMTACLNodorm:"Cannot modify/delete active management access list",
	errMGMTACLNodoadd:"Cannot modify active management access list",
	errMGMTACLNoSuchlish:"No such management access list",
	errMGMTACLNoALLlist:"No such management access list",
	errMGMTACLPriexceed:"Value is out of range",
	errMGMTACLexists:"Entry already exists",
	errMGMTACLNopri:"With no explicit priority input ,value is out of range,you can continue input with sequence command",
	errMGMTACLNoSuchace:"Cannot remove unexisting management access entry",
	errMGMTACLentry:"Cannot create new entry in mgmt acl table since the table is full",
	errMGMTACLportWrong:"Wrong or no port set",
	errMGMTACLsourceIP:"Source ip should be unicast",
	errMGMTACLsourceVid:"Wrong vid of source set",
	errMGMTACLconsole:"Input profile rule name mustn't  be Console-only,console-only,Console only or console only",
	errMGMTACLname:"Please specify a list name",
	<!--MGMT ACL end-->
	<!--GVRP err start-->
	errGvrpNoenable:"GVRP global not enable",
	errGvrpDB:"GVRP vlan db is full",
	errGvrpportNoTrunk:"Port mode is not trunk, can't enable gvrp",
	errGvrpMirror:"Mirror destination port can not enable gvrp",
	errGvrpPortmode:"GVRP Port enable must be in trunk mode",
	errGvrpTimerDIV:"GVRP Timer value must device by 5",
	errGvrpTimerJoin:"GVRP join timer must less than half of leave timer",
	errGvrpTimerLeave:"GVRP leave timer must larger than tiwce of join time and less than leaveall time",
	errGvrpTimerLeaveall:"GVRP leaveall timer must larger than leavetime",
	<!--GVRP err end-->
	<!--stp err start-->
	errstpHello:"Invalid STP parameter of hello-time",
	errstpforward:"Invalid STP parameter of forward-delay",
	errstpage:"Invalid STP parameter of maximum-age",
	errstptimer:"Wrong STP timer value. Timers should comply with the following formula:\n 2*(Forward-Delay - 1) >= Max-Age and Max-Age >= 2*(Hello-Time + 1)",
	errstpbridge:"Invalid STP parameter of bridge priority, it must multiples of 4096",
	errstpPortpri:"Invalid STP parameter of port priority, it must multiples of 16",
	errstpCst:"Not allowed to delete CST",
	errstpCstVlan:"Not allowed to delete VLAN from CST",
	errstpMstpLldp:"LLDP PDU Flooding Enabled prevents executing MSTP Enable",
	errstpshort:"Current Pathcost method is short,the valid max cost value is 65535",
	<!--stp err end-->
	<!--lacp err start-->
	errLacpPri:"Invalid LACP system priority",
	errLacpActive:"Invalid LACP active ports",
	errLacpNotMember:"Some ports are not LACP member ports",
	errLacpStatic:"Can not add LACP trunk member to static trunk",
	<!--lacp err end>
	<!--port security err start-->
	errSecurityLess:"Maximum is less than number of currently secured mac-addresses",
	errSecurityMAC:"Secure MAC address not exist",
	errSecurityMACFull:"Total secure mac-addresses on port has reached maximum limit",
	errSecuritTrunk:"Port belongs to a LAG",
	errSecurityType:"Invalid port security mac address type",
	errSecurityAzero:"Secure MAC address cannot be all zero MAC address",
	errSecuritySYSMAC:"Secure MAC address cannot be system MAC address",
	errSecurityMCAST:"Secure MAC address cannot be multicast MAC address",
	errSecurityMACConfit:"Mac entry exist in port security table",
	errSecurityForbid:"Cannot add static address on port because port security is enabled",
	errSecurityStaticMac:"Port has static address",
	<!--port security err end-->
	<!--user acl err start-->
	errACLEmpty:"ACE does not exist.",
	errACLExists:"ACE already exists.",
	errACLPri:"MIB Index is out of range.Index must be bigger then 0 and Existing ifindex.",
	errACLlistEmpty:"ACL does not exist.",
	errACLlistexists:"Different type of ACL with the same name already exists.",
	errACLlistbinding:"ACL list is binding",
	errACLemptybind:"Empty ACL Can Not be Attached to Class-map or interface.",
	errACLillegalEmpty:"Type of ACL is illegal.",
	errACLinsuff:"Insufficient PIE entries",
	errACDscp:"DSCP value is invalid. DSCP value should be 0-63",
	errACLTCPFlag:"Illegal Tcp Flags String.",
	errACLPrecedence:"IP-PRECEDENCE value is invalid.! IP-PRECEDENCE value should be 0-7",
	errACLrange:"Port range value is invalid",
	errACLPortType:"Can't apply acl since another same type acl already applied.",
	errACLPortAcl:"Can't apply acl since another acl already applied.",
	errACLPortTRUNK:"Port-Channel member port is not allowed to bind/unbind ACL",
	errACLEntryFULl:"Cannot create new entry in acl table since the table is full",
	errACLEntryPOOLFULL:"Cannot create new ace entry since the table is full",
	<!--user acl err end-->
	<!--igmp snooping err start -->
	errIgmpPort:"Mcast remove port OK",
	errIgmpAddr:"Invalid multicast address, it is reserved",
	errIgmpgroup:"Invalid IGMP max-group action",
	errIgmpQuerierChange:"Querier is not self,can not modify",
	errIgmpQuerierVersion:"Querier version can not larger than igmp version",
	errIgmpQuery:"Invalid IGMP Query interval",
	errIgmpResponse:"Invalid IGMP Response time",
	errIgmpRouter:"Invalid IGMP Router timeout",
	errIgmpINTVl:"Invalid IGMP Last member query interval",
	errIgmpVar:"Invalid IGMP Robustness variable",
	errIgmpstaticEntry:"IGMP Static Group is exist",
	errIgmpGroupEntry:"IGMP Group entry does not exist",
	errIgmpRouterPort:"Invalid unknown multicast action;\n when multicast snooping is disable, \naction router-port can not be set",
	errMCASTGroupType:"Multicast Group type error ",
	errMCASTADDR:"Multicast Address error ",
	errMCASTIPType:"Multicast ip type error ",
	errMCASTVlan:"Multicast vlan not exist",
	errMCASTDATA:"Multicast group database is full",
	errIgmpProID:"IGMP profile ID not allowed",
	errIgmpProType:"IGMP profile type is error",
	errIgmpProUsed:"IGMP profile ID is used, can not delete and modify",
	errIgmpProNotExist:"IGMP profile does not exist",
	errIgmpProEmpty:"IGMP profile is empty",
	errIgmpProIPType:"Multicast profile ip type is not match",
	errMVRenable:"MVR is Disabled, can not configure the setting of mvr",
	errMVRType:"MVR error port type",
	errMVRPortMode:"MVR port type can not configure for this vlan port mode",
	errMVRVlanPortType:"VLAN membership can not configure the MVR port type (source port must in MVR VLAN,receiver port must not be MVR VLAN)",
	errMVRVlanPortSource:"MVR source port must in mvr vlan",
	errMVRVlanPortReceiver:"MVR revciever port can not be mvr vlan member",
	errMVRStaticGroupConflict:"MVR static group conflict with MVR group range",
	errMVRStaticGroupPortType:"MVR static group port member,In compatible mode, applies to only receiver ports. In dynamic mode, it applies to receiver ports and source ports",
	errMVRVlanStaticSet:"MVR static group vlan id is not the MVR VLAN ID",
	errMVRigmpStaticSet:"IGMP Snooping static group can not use MVR group",
	errStaticGroup:"MVR is enabled and the Static group vlan is mvr vlan",
	<!--igmp snooping err end-->
	<!--dhcp server err start -->
	errLMT:"Lease time can not exceed 999 days!",
	errLTE:"Lease time can not be 0 !",
	errLTER:"Error date input!",
	errDSSI:"Start ip is invalid",
	errDEI:"End ip is invalid",
	errDSSH:"Start ip can not be larger than end ip",
	errDSLT:"There are only 1024 ip addresses in a section.",
	errDSO:"IP addresses conflict with another section",
	errDSNF:"Section index is out of range,you can configure no more than 256 sections",
	errDSIN:"The ip address and gateway ip are not in the same subnet.",
	errDSNG:"No gateway ip configured",
	errDSNCG:"There should not be any gateway ip inside a section",
	errDSII:"This ip does not exist in the ip address pool",
	errDSIM:"Ip is in use,current pool can not be modified",
	<!--dhcp server err end -->
	errDNSR:"Exceed the maximum number of DNS servers(4)",
	errEntryFull:"database is full",
	<!--igmp snooping err end-->
	<!--vlan mac group err start -->
	errMacVlanId:"Group idx is exist",
	errInvalidMacAddress:"Mac address is invalid",
	<!--vlan mac group err end-->
	<!--mac staticaddress err start -->
	errMacFull:"Mac table is full",
	errStaticMacZero:"Static entry cannot be all zero MAC address",
	errMacStaticTable:"Mac entry exist in static table",
	<!--mac staticaddress err end-->
	<!--based err start -->
	errFailed:"FAILED",
	errSuccess:"SUCCESS",
	errActionNoAllowed:"Action not allowed",
	errActionNoFinished:"Action not finished",
	errBufferSizeError:"Buffer size error",
	errChipNoSupported:"Chip not supported",
	errInvalidDropPrecedence:"Invalid drop precedence",
	errInvalidDscpId:"Invalid dscp id",
	errEntryAlreadyExist:"The entry already exists",
	errNameAlreadyExist:"The name already exists",
	errInvalidEntryIndex:"Invalid entry index",
	errSEntryNoExist:"Specified entry doesn't exist",
	errEntryIsExist:"entry is exist",
	errDatabaseIsFull:"database is full ",
	errInvalidEtherType:"Invalid ether type",
	errFiberPortNoSetSD:"A pure fiber port can not configure speed/duplex",
	errFiberOnlySupport:"Fiber only supports 1 gigabit speed",
	errGatewayNoFound:"Gateway not found",
	errInputParameterErr:"Input Parameter Error",
	errInvalidIPAddr:"Invalid IPv4 address",
	errInvalidIPMask:"Invalid IPv4 address mask",
	errIPAndGWNoInSameSub:"Ipv4 address and gateway not in the same subnet",
	errInvalidIPv6Addr:"Invalid IPv6 address",
	errIPv6AddrAndGWMust:"IPv6 address and gateway must in the same subnet",
	errInvalidHostName:"Invalid host name",
	errInvalidMacAddr:"Invalid MAC address",
	errSpecifiedValueOut:"Specified value is out of range",
	errSpecifiedInvalid:"Specified range is invalid",
	errInvalidPid:"Invalid port id",
	errInvlaidPri:"Invalid priority",
	errInvalidQueueId:"Invalid queue id",
	errSNTPUpFail:"SNTP update failed",
	errInvlaidStr:"Invalid string",
	errTimeErr:"Error! Start Time greater than End Time.",
	errTimeDaylight:"Error! The difference between daylight savings From and To must be greater than Time Set Offset",
	errEmptyStrInput:"Empty string input",
	errExceedStrLen:"Exceed recommended maximum string length",
	errInvlaidPortNum:"Invalid TCP/UDP port number",
	errOperationTimeOut:"Operation time out",
	errUnknownHost:"Unknown host",
	errConfigSuccess:"Syncronize config success",
	errConfigFailed:"Syncronize config failed",
	errConfigNameErr:"Configuration file extension name is invalid",
	errConfigFailErr:"Invalid config file",
	errFlashInvalid:"Flash image is invalid",
	errImageInvalid:"Image signature is invalid",
	errimageNoAllow:"Do not allow to earse active image partition",
	errSystemImageErr:"System do not have sufficient memory to do firmware image operation. Please turn off some memory consuming features such as SNMP to make operation available",
	errSystemNameInvalid:"Image extension name is invalid",
	errSystemAllowIErr:"Image is processing, do not allow to process another image",
	errDnsFailed:"DNS resolution failed. Please check DNS server setting or host name",
	errUserNumMaxErr:"Exceed max support user number",
	errUserNoExist:"The user does not exist",
	errSpecifyUserName:"Please specify a user name",
	errDefaultUserErr:"Default user name cannot be deleted.",
  errCurrentUserErr:"The current user name cannot be deleted.",
	errUseNaINvalidErr:"User name is invalid",
	errPasswordInErr:"Password is invalid" ,
  errUseNaLengthErr:"The length of the username is invalid",
  errPasswordLengthErr:"The length of the password is invalid",
	<!--based err end-->
	<!--AAA err start -->
	errAuthLiFull:"Auth Lists are full",
	errAuthLiExist:"Auth List not exist",
	errAuthLiName:"Please specify a Auth List name",
	errAuthType:"Invalid auth type",
	<!--AAA err end-->
	<!--ACL err start -->
	errACLPhInvalid:"Phase is invalid",
	errACLTypeIsInvalid:"Block aggregator type is invalid",
	errACLBlUsedPr:"Block is using by protocol",
	errACLBlInInvalid:"Block index is invalid",
	errACLBlACLFull:"Block in ACL is full",
	errACLTempIsFulle:"Template is fulle",
	errACLTempIdInvalid:"Template id is invalid",
	errACLFieldNOExist:"Field does not exist in the template",
	errACLRateRnIdErr:"Rate entry id is invalid",
	errACLPoleFull:"Policer table is full",
	errACLVlanEnOccupie:"All vlan range entries are occupied",
	errACLEnRangeFull:"Cannot create new entry in the Port Range table since the table is full",
	errACLLogTableFull:"Log table is full",
	errACEEntryTaFull:"Cannot create new ace entry in acl table since the table is full",
	<!--ACL err end-->
	<!--proto isg err start -->
	errIsgEntryReached:"Maximun number of entries of port has reached",
	errIsgBindingNumber:"Maximun number can't smaller than current binding number.",
	errIsgDataFull:"IP Source Guard database is full",
	errIsgIpSourceEx:"IP Source Guard Entry is exist",
	errIsgMacAddr:"MAC address can not be multicast address",
	<!--proto isg err end-->
	<!--mac addr err start -->
	errMacEntryCl:"Error type of mac entry clear",
	errMacEntryNoExist:"Mac entry does not exist",
	errMacEntryExist:"Mac entry does already exist",
	errMacInvalidSet:"Invalid unicast port setting",
	errMacInvalidUnAddr:"Invalid unicast mac address",
	errStaticMacAddr:"Static entry cannot be system MAC address",
	errStaticMulPort:"Static entry cannot be multicast MAC address",
	errStaticNoTrunkMem:"Static entry port can NOT be trunk member port",
	errStaticNoEmPort:"Static entry port can NOT be empty trunk port",
	errDataIsBusy:"MAC Address Database is busy. Please try it later",
	<!--mac addr err end-->
	<!--black mac addr err start -->
	errBlackEntryEx:"Mac entry exist in filter table",
	errBlackZebroAddr:"Filter entry cannot be all zero MAC address",
	errBlackSysMac:"Filter entry cannot be system MAC address",
	errBlackEntrMuMac:"Filter entry cannot be multicast MAC address",
	<!--black mac addr err end-->
	<!--mirror err start -->
	errMirrorNoSession:"Invalid mirror session id",
	errMirrorDstPort:"A port can not be monitor destination port for multiple sessions",
	errMirrorDstNoExist:"Monitor destination port does not exist",
	errMirrorPortErr:"Monitor destination port Error",
	errMirrorSourNoList:"Monitor Destination port can NOT appears in source port list",
	errMirrorDstMiPor:"Monitor Destination port can not be mirrored port",
	errMirrorTrunkPort:"Monitor Destination port can NOT trunk port",
	errMirrorMemPort:"Monitor Destination port can NOT be trunk member port",
	errMirrorGvrpEnable:"Monitor Destination port is GVRP Enabled",
	errMirrorEnableState:"Mirror session is not in port based enable state",
	<!--mirror err end-->
	<!--sys l3 err start -->
	errL3InterfaceInval:"Interface type invalid",
	errL3DatabaseFull:"Database is full",
	errL3EntryNodata:"Entry is not found in database",
	errL3NumInterNum:"Number of maximum interface number exceed",
	errL3NoInterfaceId:"Invalid loopback intreface id",
	errL3IpNoExist:"IP address does not exist on interface",
	errL3IpPriNoExist:"The primary IP address does not exist, please configure the primary IP address first",
	errL3IpIsExist:"IP address already exists",
	errL3IpPriIsExist:"IP address already exists, This IP address is the current primary IP address",
	errL3IpSubIsExist:"Error: Please delete the sub address in the interface view first.",
	errL3IpIsPri:"Error: The specified address cannot be deleted because it is not the primary address of this interface",
	errL3IpIsSub:"Error: The specified address cannot be deleted because it is not the sub address of this interface",
	errL3IpNumInterface:"IP address number exceed on interface",
	errL3RouteInterface:"Not L3 routing interface",
	errL3NoIpv6InterSet:"Cannot disable IPv6 processing on an interface that is configured with an explicit IPv6 address",
	errL3SameSubnet:"Duplicated IP on the same subnet",
	errL3IpTheSubnet:"Overlapped IP on the subnet",
	errL3InvaliIpAddr:"Invalid IP address",
	errL3InvalidIpMaskErr:"Invalid IP mask",
	errL3Ipv6NoValid:"IPv6 prefix is not valid",
	errL3GWAddrNoDevi:"Gateway cannot be one of the addresses configured on this device",
	errL3RouterDefGW:"Only a directly connected router can be defined as the gateway",
	errL3GWDHCPInter:"Gateway cannot on the DHCP enabled interface",
	errL3GWSameVlan:"Gateway cannot be the same subnet as the management VLAN",
	errL3RouteViaInter:"IP static route cannot be defined via loopback interface",
	errL3HopsNetExceed:"Max number of config next hops for this network has been exceeded",
	errL3StaticFull:"Static route database is full",
	errL3StaticExist:"Static route does not exist",
	errL3StaticMustAddr:"ARP MAC address must be Unicast address",
	errL3MacAddrNoErr:"MAC address can not same as device mac.",
	errL3ArpNoMatch:"ARP interface does not match IP interface port",
	errL3ArpNwDir:"ARP IP must be from a directly connected network",
	errL3ArpDefDevice:"ARP IP cannot be an IP interface defined on this device.",
	errL3NoSuchExist:"No such instance exists",
	errL3SetARPOverBy:"Setting ARP static configuration over IP interface obtained by DHCP is not allowed",
	errL3Ipv6NoEnbaleInter:"IPv6 is not enabled on the interface",
	errL3NeighNoCreate:"Neighbor entry cannot be created - subnet is not defined on the device",
	errL3VlanRelay:"VLAN can not be deleted because IP Interface or DHCP relay is defined on the VLAN.",
	<!--sys l3 err end-->
	<!--igmp err start -->	
	errIgmpProGroup:"Invalid MLD max-group action",
	errIgmpProAllow:"MLD profile ID not allowed",
	errIgmpProMldMod:"MLD profile ID is used, can not delete and modify",
	errIgmpProMldExist:"MLD profile does not exist",
	errIgmpProMldZero:"MLD profile is empty",
	errIgmpProMulMaximum:"Multicast port group arrived maximum",
	<!--igmp err end-->
	<!--qos err start -->
	errQosMode:"Currently not in QoS mode",
	errQosNoBasic:"Currently not in QoS basic mode",
	errQosPriVal:"Invalid priority value",
	errQosInvalidVal:"Invalid queue weight value",
	errQosInvalidCos:"Invalid CoS value" ,
	errQosInvalidDscp:"Invalid DSCP value",
	errQosPriVal:"Invalid IP Precedence value",
	errQosInvalQueId:"Invalid queue id",
	errQosRemarkCon:"DSCP and IP precedence remarking state conflict",
	<!--qos err end-->
	<!--rate err start -->
	errRateRange:"Rate invalid: out of range",
	errRateInvalidMul:"Rate invalid: must be a multiple of 16",
	errRateMultiBur:"BurstSize invalid: must be a multiple of 128",
	errRateBurMulti:"BurstSize invalid: must be a multiple of 1024",
	<!--rate err end-->
	<!--snmp err start -->
	errSnmpInvalStr:"Invalid snmp string",
	errSnmpTableFull:"Trap host table is full",
	errSnmpConmmIsFull:"Community table is full",
	errSnmpSecLelErr:"Security level is error",
	errSnmpNotiErr:"Notifaction type is error",
	errSnmpNotiMatch:"Notifaction type inform can not  match the version 1(default version is 1)",
	errSnmpNameCommErr:"SNMP communtiy name is error",
	errSnmpV3ViewFul:"view table is full",
	errSnmpV3DefaultView:"view name is default view, can not add and delete",
	errSnmpV3SpecialName:"special view name, can not be added",
	errSnmpV3SecurityFull:"security group table is full",
	errSnmpV3AccessFull:"access group table is full",
	errSnmpV3GroupName:"SNMP group name error",
	errSnmpV3TableFull:"user table is full",
	errSnmpV3RemoteTabl:"remote engine table is full",
	errSnmpV3DefaultViNa:"Invalid oid",
	errSnmpV3UseNaVi:"Invalid use default view name",
	errSnmpV3InvalOid:"Invalid oid length",
	errSnmpV3NoOidMask:"Invalid oid mask",
	errSnmpV3InvalidMaskLen:"Invalid oid mask length",
	errSnmpV3CommunFound:"Community is not found",
	errSnmpV3GrouExist:"Group is exist",
	errSnmpV3SecMoMath:"Group does not exist or security mode not match",
	errSnmpV3AcceCon:"Access group exist but the view conflict",
	errSnmpV3ReadExis:"Read View does not exist",
	errSnmpV3MustAdd:"Must add read view",
	errSnmpV3WritExi:"Write View does not exist",
	errSnmpV3NotViNotEx:"NotifyView does not exist",
	errSnmpV3AcessGrCom:"Acess group is used by community or user",
	errSnmpV3EngForErr:"Engine id format is error",
	errSnmpV3EngHexad:"EngineID must contain 10 through 64 hexadecimal digits, must divided by 2",
	errSnmpV3UsNameNoFind:"User name is not found",
	errSnmpV3UserPLErr:"Username error",
	errSnmpV3PassWokErr:"Password error",
	errSnmpV3AuthProNo:"No authetication protocol",
	errSnmpV3EncryProhf:"No encryption protocol",
	<!--snmp err end-->
	<!--mstp err start -->
	errMstpActiFldWhe:"Can not congiure LLDP disable action to flooding when MSTP is running",
	errMstpInterMul:"LLDP transmission interval multiplies holdtme multiplier should be smaller than 65535",
	errMstpDelayThan:"LLDP transmission delay should not be larger than LLDP transmission interval * 0.25",
	errMstpNetExi:"Network policy does not exist",
	errMstpTypeDup:"Network policy application type duplicated",
	errMstpBindInter:"Network policy binded to an interface can not be modified or removed",
	errMstpNetConBec:"A network policy of type Voice cannot be configured because the Voice VLAN is in automatic update mode",
	errMstpVoiceAu:"Voice VLAN cannot be in automatic update mode because there is a network policy of type VOICE",
	errMstpParisHex:"Invalid input. Input should be paris of hexadecimal value without \":\", ex: 1122AB",
	<!--mstp err end-->
	<!--radius err start -->
	errRadiusSerErr:"radius server error",
	errRadiusSerTheNo:"There is no radius server",
	errRadiusOverSerNum:"Over max radius server number",
	errRadiusSerAlEx:"Radius server already exist",
	errRadiusSetUniqueVal:"Setting unique priority value is needed",
	errRadiusAuthCatErr:"Authentication error",
	<!--radius err end-->
	<!--syslog err start -->	
	errSyslogSerIp:"Syslog server ip error",
	errSyslogRemoFul:"Syslog remote host is full",
	errSyslogNoExist:"Syslog remote host is exist",
	errSyslogHostNo:"Syslog remote host does not exist",
	errSyslogInputSev:"Syslog input severity error",
	errSyslogCateErr:"Syslog input category error",
	<!--syslog err end-->
	<!--tac err start -->
	errTacSerErr:"tacacs+ server error",
	errTacNoSer:"There is no tacacs+ server",
	errTacMaxSerNum:"Over max tacacs+ server number",
	errTacSerAlEx:"Tacacs+ server already exist",
	errTacSetPriVal:"Setting unique priority value is needed",
	<!--tac err end-->
	<!--ospf err start -->
	errOSPFIsClose:"Error: Please enable OSPF function first",
	<!--ospf err end-->
	<!--rip err start -->
	errRipIsClose:"Error: Please enable RIP function first",
	<!--rip err end-->
	<!--vrrp err start -->
	errVrrpSubif:"The virtual IP is not in the same subnet",
	errVrrpMacsrv:"The amount of virtual router has arrived the maximum",
	errVrrpMacvip:"The amount of virtual IP has arrived the maximum"
	<!--vrrp err end-->		
},
lang_warning:{
  warningAclNotMod:"Active ACL can not be modified",
  warningAclunderBind:"Cannot delete under binding management ACL",
  warningAclName:"Name $1 has existed",
  warningAclsequence:"Sequence is exist.",
  warningSelectEntry:"Please select only one entry to view",
  warningLoopback:"Loopback interface cannot be edited",
  warningAcldelate:"Cannot delete active management ACL",
  warningSelectActive:"Please select only one entry to active",
  warningSelectDeactive:"Please select only one entry to deactive",
  warningPriExist:"Priority is exist.",
  warningSelectEdit:"Please select only one entry to edit",
  warningProtocolGroup:"Protocol group settings is same as group id $1",
  warningVlanName:"Name had been used by VLAN $1",
  warningTrunkPort:"Trunk port must assign native VLAN",
  warningAccessPort:"Access port must assign access VLAN",
  warningHybridPort:"Hybrid port must assign PVID",
  warningOnlyUntag:"Only one untagged VLAN can be selected",
  warningOnlyPVID:"Only One PVID can be selected",
  warningIpSubnetGroup:"Ip subnet group is exist"
},

<!--#ifdef CONFIG_SYS_DHCP_SERVER-->
/* DHCP */
lang_dr:{
lblDrStatus:"State",
titDrPortSettingTable:"DHCP Port Setting Table",
titDrEditPortSetting:"Edit Port Setting",
lblDrEntryNo:"Entry",
lblDrPort:"Port",
lblDrPortEnable:"Enable",
lblIPPoolTable:"IP Pool Table",
lblPoolName:"Pool",
txtPoolNamelen:"(<i>1 to 32 alphanumeric characters</i>)",
lblPoolSection:"Section",
lblPoolAddrStart:"Start Address",
lblPoolAddrEnd:"End Address",
lblGateway:"Gateway",
lblGatewayMask:"Mask",
lblBindStatus:"Static Binding First",
lblDNSPrimaryIp:"DNS Primary Server",
lblDNSSecondIp:"DNS Second Server",
lblOption43:"option 43",
lblOption43Str:"Address",
lblOption43Type:"Format",
txtAscii:"ascii",
txtHex:"hex",
lblLeaseTime:"Lease time",
lblIPSectionTable:"IP Address Section",
lblVlanIfPoolTable:"Vlan Interface Address Pool Table",
lblDrGroupTable:"DHCP Server Group Table",
lblDrVlanifGroup:"DHCP Server Group",
lblDrGroupId:"Group ID",
lblDrGroupIp:"Group IP Address",
lblGroupBindVlanIf:"Bind VLAN Interface",
lblDrclients:"DHCP Client List",
lblDrVlanid:"VLAN",
lblDrUserName:"User Name",
lblDrClientStatic:"Static Binding Table",
lblDrClientStaticEdit:"Static Binding Table Edit",
lblDrClientStaticAdd:"Static Binding Table Add"
},
<!--#endif-->
<!--#ifdef CONFIG_SYS_L3_ROUTE_RIP-->
/* Rip */
lang_rip:{
lblRipStatus:"Rip Routes status",
lblRipNetworkTable:"Network Setting table",
lblRipSettingInfo:"Rip Routes Info",
lblRipNetworkAddr:"Network Ipv4 Address",
lblRipNetworkMask:"Network Mask"
},
<!--#endif-->
<!--#ifdef CONFIG_SYS_L3_ROUTE_OSPF-->
/* Ospf */
lang_ospf:{
lblOspfStatus:"OSPF Routes status",
lblOspfAreaNetworkTable:"Area Network Setting table",
lblOspfSettingInfo:"OSPF Routes Info",
lblOspfNetworkAddr:"Network Ipv4 Address",
lblOspfNetworkMask:"Network Mask",
lblOspfArea:"Area Id"
},
<!--#endif-->
<!--#ifdef CONFIG_SYS_L3_VRRP-->
/* vrrp */
lang_vrrp:{
titAddIPv4Intf:"Add IPv4 VRRP Interface",
txtInit:"init",
txtBackup:"backup",
txtMaster:"master",
lblVrrpIntfTable:"VRRP Interface Setting table",
lblVrid:"Router ID",
lblVifaddr:"Virtual IP",
lblVifState:"State",
lblPri:"Priority",
lblAdver:"Advertise",
lblPreempt:"Preempt",
lblDelay:"Delay"
},
<!--#endif-->

lang_zzz:{
  dummy:""
}
};


var ready_js_defLang = 1;

