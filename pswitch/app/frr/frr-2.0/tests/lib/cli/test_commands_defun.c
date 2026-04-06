#include <zebra.h>

#include "command.h"
#include "linklist.h"

#include "tests.h"

DEFSH (0, no_debug_zebra_fpm_cmd_vtysh, 
       "no debug zebra fpm", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug zebra FPM events\n")

DEFSH (0, no_debug_zebra_rib_detailed_cmd_vtysh, 
       "no debug zebra rib detailed", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug zebra RIB\n"
       "Detailed debugs\n")

DEFSH (0, no_debug_zebra_kernel_msgdump_cmd_vtysh, 
       "no debug zebra kernel msgdump {recv|send}", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra between kernel interface\n"
       "Dump raw netlink messages,  sent and received\n"
       "Dump raw netlink messages received\n"
       "Dump raw netlink messages sent\n")

DEFSH (0|0|0|0|0|0, no_vrf_debug_cmd_vtysh, 
      "no debug vrf", 
      "Negate a command or set its defaults\n"
      "Debugging functions (see also 'undebug')\n"
      "VRF Debugging\n")

DEFSH (0, debug_zebra_mpls_cmd_vtysh, 
       "debug zebra mpls", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra MPLS LSPs\n")

DEFSH (0, debug_zebra_nht_cmd_vtysh, 
       "debug zebra nht", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra next hop tracking\n")

DEFSH (0, debug_zebra_kernel_msgdump_cmd_vtysh, 
       "debug zebra kernel msgdump {recv|send}", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra between kernel interface\n"
       "Dump raw netlink messages,  sent and received\n"
       "Dump raw netlink messages received\n"
       "Dump raw netlink messages sent\n")

DEFSH (0, config_write_integrated_cmd_vtysh, 
 "write integrated", 
 "Write running configuration to memory,  network,  or terminal\n"
 "Write integrated all-daemon frr.conf file\n")

DEFSH (0, debug_zebra_packet_detail_cmd_vtysh, 
       "debug zebra packet (recv|send) detail", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra packet\n"
       "Debug option set for receive packet\n"
       "Debug option set for send packet\n"
       "Debug option set detailed information\n")

DEFSH (0, no_debug_zebra_packet_cmd_vtysh, 
       "no debug zebra packet", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra packet\n")

DEFSH (0, debug_zebra_fpm_cmd_vtysh, 
       "debug zebra fpm", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug zebra FPM events\n")

DEFSH (0, debug_zebra_packet_direct_cmd_vtysh, 
       "debug zebra packet (recv|send|detail)", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra packet\n"
       "Debug option set for receive packet\n"
       "Debug option set for send packet\n")

DEFSH (0, debug_zebra_rib_cmd_vtysh, 
       "debug zebra rib", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug RIB events\n")

DEFSH (0, debug_zebra_packet_cmd_vtysh, 
       "debug zebra packet", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra packet\n")

DEFSH (0, no_debug_zebra_nht_cmd_vtysh, 
       "no debug zebra nht", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra next hop tracking\n")

DEFSH (0, no_debug_zebra_rib_cmd_vtysh, 
       "no debug zebra rib", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug zebra RIB\n")

DEFSH (0, debug_zebra_events_cmd_vtysh, 
       "debug zebra events", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra events\n")

DEFSH (0, show_debugging_zebra_cmd_vtysh, 
       "show debugging zebra", 
       "Show running system information\n"
       "Debugging information\n"
       "Zebra configuration\n")

DEFSH (0, debug_zebra_rib_detailed_cmd_vtysh, 
       "debug zebra rib detailed", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug RIB events\n"
       "Detailed debugs\n")

DEFSH (0, debug_zebra_kernel_cmd_vtysh, 
       "debug zebra kernel", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra between kernel interface\n")

DEFSH (0, no_debug_zebra_kernel_cmd_vtysh, 
       "no debug zebra kernel", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra between kernel interface\n")

DEFSH (0|0|0|0|0|0, vrf_debug_cmd_vtysh, 
      "debug vrf", 
      "Debugging functions (see also 'undebug')\n"
      "VRF Debugging\n")

DEFSH (0, no_debug_zebra_events_cmd_vtysh, 
       "no debug zebra events", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra events\n")

DEFSH (0, no_debug_zebra_mpls_cmd_vtysh, 
       "no debug zebra mpls", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra MPLS LSPs\n")

void
test_init_cmd ()
{
  install_element (CONFIG_NODE, &debug_zebra_packet_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_packet_direct_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_kernel_msgdump_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_kernel_msgdump_cmd_vtysh);
  install_element (VIEW_NODE, &show_debugging_zebra_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_nht_cmd_vtysh);
  install_element (CONFIG_NODE, &vrf_debug_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_kernel_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_events_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_rib_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_fpm_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_fpm_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_packet_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_fpm_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_packet_cmd_vtysh);
  install_element (ENABLE_NODE, &config_write_integrated_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_kernel_msgdump_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_mpls_cmd_vtysh);
  install_element (CONFIG_NODE, &no_vrf_debug_cmd_vtysh);
  install_element (ENABLE_NODE, &vrf_debug_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_kernel_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_events_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_kernel_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_rib_detailed_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_packet_detail_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_rib_detailed_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_kernel_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_packet_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_nht_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_fpm_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_events_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_rib_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_rib_detailed_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_packet_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_mpls_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_rib_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_nht_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_packet_direct_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_events_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_kernel_msgdump_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_mpls_cmd_vtysh);
  install_element (ENABLE_NODE, &no_vrf_debug_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_nht_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_mpls_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_rib_detailed_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_rib_cmd_vtysh);
}
