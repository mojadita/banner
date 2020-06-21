# Makefile -- build file for the sysvbanner program.
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Sun Jun 21 19:34:53 EEST 2020
# Copyright: (C) 2020 Luis Colorado.  All rights reserved.
# License: BSD.

targets = sysvbanner
toclean = $(targets)
RM ?= rm -f

all: $(targets)
clean:
	$(RM) $(toclean)

sysvbanner_objs = banner.o
toclean += $(sysvbanner_objs)

sysvbanner: $(sysvbanner_objs)
	$(CC) $(LDFLAGS) -o $@ $($@_srcs) $($@_objs) $($@_ldflags) $($@_libs) $(LIBS)
