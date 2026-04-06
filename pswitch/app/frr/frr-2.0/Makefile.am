## Process this file with automake to produce Makefile.in.

SUBDIRS = lib qpb fpm @ZEBRA@ @LIBRFP@ @RFPTEST@ \
	 @RIPD@ @OSPFD@  \
         @WATCHFRR@ @VTYSH@ @OSPFCLIENT@ @DOC@ m4 @pkgsrcdir@ \
         redhat @SOLARIS@ tests tools cumulus snapcraft

DIST_SUBDIRS = lib qpb fpm zebra ripd ospfd \
	  watchfrr vtysh ospfclient doc m4 pkgsrc redhat tests \
	  solaris @LIBRFP@ @RFPTEST@ tools cumulus snapcraft

EXTRA_DIST = aclocal.m4 SERVICES REPORTING-BUGS \
	update-autotools \
	vtysh/Makefile.in vtysh/Makefile.am \
	tools/rrcheck.pl tools/rrlookup.pl tools/zc.pl \
	tools/zebra.el tools/multiple-bgpd.sh

ACLOCAL_AMFLAGS = -I m4

noinst_HEADERS = defaults.h
