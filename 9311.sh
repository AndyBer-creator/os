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
make webenglishonly

make distclean
make yfd_9311_48

make distclean
make yfd_9311_48_poe

#rm -rf image_yfd_*/

echo "+++++++++++++++++++finish++++++++++++++++++++++++++++"
