# $Id: Makefile,v 1.2 1999/11/08 15:29:35 brad Exp $

all clean:
	@for d in libhosts cmd kernel; do \
		(cd $$d; make $@); \
	done

.PHONY: TAGS
TAGS:
	find -name "*.[chCH]" -print | etags -

check: all
	-rmmod kernel/chaosnet.ko
	insmod kernel/chaosnet.ko
	cmd/chinit 1 local
	cmd/cheaddr enp0s25 401
	cmd/chtime local
