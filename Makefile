CC := /usr/bin/gcc
SHELL := /usr/bin/env
.SHELLFLAGS := -S bash -ec

.PHONY: debug release c clean deps
.DELETE_ON_ERROR:
.ONESHELL:

debug:
	${MAKE} -C src debug -j

release:
	${MAKE} -C src release -j

c clean:
	rm -rvf docs
	${MAKE} -C src clean -j

deps:
	@echo "You will need libevent and pkg-config to use this project correctly. This command assumes you're on a Debian/Ubuntu system, and runs `sudo apt install`"
	sudo apt install libevent-dev pkg-config