CC := /usr/bin/gcc
SHELL := /usr/bin/env
.SHELLFLAGS := -S bash -c

.PHONY: debug release c clean v val deps
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

deps:
	@echo "You will need libevent and pkg-config to use this project correctly. This command assumes you're on a Debian/Ubuntu system"
	sudo apt install libevent-dev pkg-config