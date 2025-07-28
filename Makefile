# Makefile -- build file for the sysvbanner program.
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Sun Jun 21 19:34:53 EEST 2020
# Copyright: (C) 2020 Luis Colorado.  All rights reserved.
# License: BSD.

targets = sysvbanner
toclean = $(targets)

RM              ?= rm -f
GZIP			?= gzip -v

prefix			?= /usr/local
exec_prefis     ?= $(prefix)
bindir          ?= $(prefix)/bin
rootdatadir     ?= $(prefix)/share
datadir         ?= $(rootdatadir)
mandir          ?= $(rootdatadir)/man
man1dir         ?= $(mandir)/man1

OS              != uname -o

OWN-FreeBSD     ?= root
GRP-FreeBSD     ?= wheel
OWN-GNU/Linux   ?= bin
GRP-GNU/Linux   ?= bin

own             ?= $(OWN-$(OS))
grp             ?= $(GRP-$(OS))
xmod            ?= 0711
dmod            ?= 0755

toinstall        = $(bindir)/sysvbanner $(man1dir)/sysvbanner.1.gz

all: $(targets)
clean:
	$(RM) $(toclean)
install: $(toinstall)
uninstall deinstall:
	$(RM) $(toinstall)

.SUFFIXES: .1.gz .1

.1.1.gz:
	$(GZIP) < $< > $@

$(bindir)/sysvbanner: sysvbanner $(bindir)
	$(INSTALL) -o $(own) -g $(grp) -m $(xmod) sysvbanner $@

$(man1dir)/sysvbanner.1.gz: sysvbanner.1.gz $(man1dir)
	$(INSTALL) -o $(own) -g $(grp) -m $(dmod) sysvbanner.1.gz $@

$(bindir) $(man1dir):
	$(INSTALL) -o $(own) -g $(grp) -m $(dmod) $@

sysvbanner_objs = banner.o
toclean += $(sysvbanner_objs)

sysvbanner: $(sysvbanner_objs)
	$(CC) $(LDFLAGS) -o $@ $($@_srcs) $($@_objs) $($@_ldflags) \
			$($@_libs) $(LIBS)

toclean += sysvbanner sysvbanner.1.gz
