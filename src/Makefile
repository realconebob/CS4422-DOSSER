CC := /usr/bin/gcc
SHELL := /usr/bin/env
.SHELLFLAGS := -S bash -c

CPPFLAGS := -D_GNU_SOURCE=1
CFLAGS := -std=gnu2x $$(pkg-config --cflags libevent)
LDLIBS := $$(pkg-config --libs libevent)

DEBUG_CFLAGS := -Wall -Wextra -Wpedantic -Og -ggdb -g3 -fsanitize=address,undefined -fanalyzer -Wanalyzer-too-complex
RELEASE_CFLAGS := -O3 -DNDEBUG -march=native -flto=auto
RELEASE_LDFLAGS := -flto=auto

DEBUG_DEFS := -DDEBUG=1
RELEASE_DEFS := -DRELEASE=1

TARGETS := dosser server

.PHONY: debug release c clean
.DELETE_ON_ERROR:
.ONESHELL:

debug: CPPFLAGS += ${DEBUG_DEFS}
debug: CFLAGS += ${DEBUG_CFLAGS}
release: CPPFLAGS += ${RELEASE_DEFS}
release: CFLAGS += ${RELEASE_CFLAGS}
release: LDFLAGS += ${RELEASE_LDFLAGS}

debug: ${TARGETS}
release: ${TARGETS}

${TARGETS}: %: %.c

c clean:
	rm -rvf ${TARGETS}