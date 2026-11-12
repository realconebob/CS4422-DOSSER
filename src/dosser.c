/**
 * @file dosser.c
 * @author Connor Walstrom (walst110@umn.edu)
 * @version 0.1
 * @date 2026-11-11
 *
 * @copyright
~~~{.txt}
BSD 3-Clause License

Copyright (c) 2026, Connor Walstrom

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
~~~
*/

#include <event2/event.h>
#include <event2/bufferevent.h>

// TODO: Really should put a version check in here but that can wait

#include <stdint.h>
#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <signal.h>

const char* argp_program_version = "0.1";
const char* argp_program_bug_address = "<walst110@umn.edu>";

const struct argp_option options[] = {
    {.name = "ip",      .key = 'i', .arg = "<dest.ip.address>", .flags = 0, .doc = "Target's IP Address"},
    {.name = "port",    .key = 'p', .arg = "<dest-port-num>",   .flags = 0, .doc = "Target's Port Number"},
    {.name = "pstart",  .key = 's', .arg = "<min-port-num>",    .flags = 0, .doc = "Port number to begin range on local computer"},
    {.name = "pend",    .key = 'e', .arg = "<max-port-num>",    .flags = 0, .doc = "Port number to end range on local computer (can be same as start)"},
    {0}
};

struct inputs {
    char* ip;
    int32_t dport, prstart, prend;
};

error_t parserfunc(int key, char *arg, struct argp_state *state) {
    struct inputs* passed = state->input;
    switch (key) {
        case 'i': // set ip
        passed->ip = arg;
        printf("Set ip: %s\n", passed->ip);
        break;

        case 'p': // set dest port
        passed->dport = atoi(arg);
        printf("Set dest port: %d\n", passed->dport);
        break;

        case 's': // set port range start
        passed->prstart = atoi(arg);
        printf("Set pr start: %d\n", passed->prstart);
        break;

        case 'e': // set port range end
        passed->prend = atoi(arg);
        printf("Set pr end: %d\n", passed->prend);
        break;

        case ARGP_KEY_ARG:
        if(state->arg_num) argp_error(state, "non-option arguments are not allowed"); // Don't want any non-option arguments
        break;

        case ARGP_KEY_END:
        // Check that everything has been set
        if(state->arg_num) argp_error(state, "non-option arguments are not allowed"); // Don't want any non-option arguments

        if(!passed->ip) argp_error(state, "target ip not set");
        if(!passed->dport) argp_error(state, "target port not set");
        if(!passed->prstart) argp_error(state, "port range start not set");
        if(!passed->prend) argp_error(state, "port range end not set");

        if(passed->dport < 0) argp_error(state, "target port less than zero");
        if(passed->prstart < 0) argp_error(state, "port range start than zero");
        if(passed->prend < 0) argp_error(state, "port range end less than zero");

        if(passed->prend < passed->prstart) argp_error(state, "port range end lower than start");
        break;

        default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

const struct argp parser = {
    .options = options,
    .parser = parserfunc,
    .doc = "Slam a target with tcp data",
};

static void signal_cb(evutil_socket_t sig, short what, void *arg) {
    (void)sig; (void)what;
    struct event_base *base = arg;
    printf("SIGINT received, shutting down\n");
    event_base_loopexit(base, NULL);
}

void writercb(evutil_socket_t fd, short what, void *arg) {
    (void)fd; (void)what; (void)arg;
}

int main(int argc, char *argv[]) {
    int res = 0;
    int32_t bea_size = 0;
    struct bufferevent** evs = NULL;

    // Parse arguments
    struct inputs parseres = {0};
    if(argp_parse(&parser, argc, argv, 0, NULL, (void*)&parseres)) {
        error(1, EINVAL, "%s: Could not parse input", argv[0]);
    }

    // Set up libevent
    struct event_base* base = event_base_new();
    if(!base) error(1, errno, "Could not get event base");
    printf("Running libevent version %s, backend: %s\n", event_get_version(), event_base_get_method(base));

    // Create and add ctrl+c close signal

    struct event* ctrlc_ev = evsignal_new(base, SIGINT, signal_cb, base);
    if(!ctrlc_ev || event_add(ctrlc_ev, NULL) < 0) {
        error(0, errno, "Could not create ctrl+c signal event");
        res = 1;
        goto cleanup;
    }

    // Create events, one per port in given range
    bea_size = parseres.prend - parseres.prstart;
    evs = calloc(bea_size + 1, sizeof(struct bufferevent*));
    if(!evs) {error(0, errno, "Could not create bufferevent* array"); goto cleanup;}
    evs[bea_size] = NULL;

    for(int32_t i = 0; i < bea_size; i++) {/* Make and bind individual connections here */}

    event_base_loop(base, EVLOOP_NO_EXIT_ON_EMPTY);

cleanup:
    event_free(ctrlc_ev);

    for(int32_t i = 0; i < bea_size; i++) {bufferevent_free(evs[i]);}
    free(evs);

    event_base_free(base);
    libevent_global_shutdown();

    exit(res);
}