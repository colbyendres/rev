#
# Makefile
#
# makefile: ex1
#
# Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

.PHONY: src

EXAMPLE=topo_sort
CC=$(RVCC)
ARCH=rv64gc
INCLUDE= -I../../env  -I../../common

all: $(EXAMPLE).exe

$(EXAMPLE).exe: $(EXAMPLE).c dataset1.h
	$(CC) $(INCLUDE) -march=$(ARCH) -O0 -static -o $(EXAMPLE).exe $(EXAMPLE).c

clean:
	rm -Rf $(EXAMPLE).exe

#-- EOF
