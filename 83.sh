#!/bin/bash

############################################################################
#
# build_product.sh
#
# This is a simple script that cleans up the SDE directory by removing
# driver logs, model logs, PCAP files, i.e. the files created by running
# various components.
#
# It does not remove any build artifacts!
#
###########################################################################

echo "   build product files "
make webnormal

make distclean
make yfd_8382_4

make distclean
make yfd_8382_4_poe

make distclean
make yfd_8382_4_poe_bt

make distclean
make yfd_8382_8

make distclean
make yfd_8382_8_poe

make distclean
make yfd_8382_8_poe_bt

make distclean
make yfd_8382_10

make distclean
make yfd_8382_10_poe

make distclean
make yfd_8382_10_poe_bt

make distclean
make yfd_8382_16

make distclean
make yfd_8382_16_poe

make distclean
make yfd_8382_24

make distclean
make yfd_8382_24_poe

#rm -rf image_yfd_*/

echo "+++++++++++++++++++finish++++++++++++++++++++++++++++"
