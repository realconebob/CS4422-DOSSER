CC := /usr/bin/gcc
SHELL := /usr/bin/env
.SHELLFLAGS := -S bash -c

.PHONY: debug release c clean v val
.DELETE_ON_ERROR:
.ONESHELL:

debug:
	${MAKE} -C src debug -j

release:
	${MAKE} -C src release -j

c clean:
	rm -rvf docs
	${MAKE} -C src clean -j

v val:
	${MAKE} -C src val -j