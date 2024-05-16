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

EXAMPLE=bfs
CC=$(RVCC)
ARCH=rv64gc
INCLUDE=

all: $(EXAMPLE).exe

$(EXAMPLE).exe: $(EXAMPLE).cpp dataset1.hpp
	$(CC) $(INCLUDE) -march=$(ARCH) -O0 -static -o $(EXAMPLE).exe $(EXAMPLE).cpp

clean:
	rm -Rf $(EXAMPLE).exe

#-- EOF
