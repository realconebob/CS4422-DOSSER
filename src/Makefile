CC := /usr/bin/gcc
SHELL := /usr/bin/env
.SHELLFLAGS := -S bash -c

CFLAGS := -std=gnu2x $$(pkg-config --cflags libevent)
LDFLAGS := $$(pkg-config --libs libevent)

DEBUG_CFLAGS := -Wall -Wextra -Wpedantic -Og -ggdb -g3 -fsanitize=address,undefined -fanalyzer -Wanalyzer-too-complex
RELEASE_CFLAGS := -O3 -DNDEBUG -march=native -flto=auto
RELEASE_LDFLAGS := -flto=auto

DEFS := -D_GNU_SOURCE=1
DEBUG_DEFS := -DDEBUG=1
RELEASE_DEFS := -DRELEASE=1

TARGETS := dosser server

.PHONY: debug release c clean v val
.DELETE_ON_ERROR:
.ONESHELL:

debug: DEFS += ${DEBUG_DEFS}
debug: CFLAGS += ${DEBUG_CFLAGS}
release: DEFS += ${RELEASE_DEFS}
release: CFLAGS += ${RELEASE_CFLAGS}
release: LDFLAGS += ${RELEASE_LDFLAGS}

debug: ${TARGETS}
release: ${TARGETS}

${TARGETS}: %: %.c

c clean:
	rm -rvf ${TARGETS}

v val:
	${MAKE} debug -j
	valgrind --leak-check=yes ./${TARGETS}