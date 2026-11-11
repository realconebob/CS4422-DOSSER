/**
 * @file main.c
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

#include <stdint.h>
#include <stdio.h>
#include <argp.h>

const char* argp_program_version = "0.1";
const char* argp_program_bug_address = "walst110@umn.edu";

const struct argp_option options[] = {
    {.name = "ip",      .key = 'i', .arg = "dest.ip.address", .flags = 0, .doc = "Target's IP Address"},
    {.name = "port",    .key = 'p', .arg = "dest-port-num", .flags = 0, .doc = "Target's Port Number"},
    {.name = "prstart", .key = 's', .arg = "min-port-num", .flags = 0, .doc = "Port number to begin range on local computer"},
    {.name = "prend",   .key = 'e', .arg = "max-port-num", .flags = 0, .doc = "Port number to end range on local computer"},
    {0}
};

struct inputs {
    char* ip;
    uint32_t dport, prstart, prend;
};

error_t parserfunc(int key, char *arg, struct argp_state *state) {
    struct inputs* passed = state->input;
    switch (key) {
        case 'i': // set ip
        passed->ip = arg;
        break;

        case 'p': // set dest port
        break;

        case 's': // set port range start
        break;

        case 'e': // set port range end
        break;

        default:
        // TODO: Consider using a helper function: https://sourceware.org/glibc/manual/latest/html_node/Argp-Helper-Functions.html
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

const struct argp parser = {
    .options = options,
    .parser = parserfunc,
    .doc = "",
};

// TODO: Now that I've got a rough outline, check out the examples to see what else I could do: https://sourceware.org/glibc/manual/latest/html_node/Argp-Examples.html

int main(int argc, char *argv[]) {
    int argidx;
    struct inputs parseres = {0};
    if(argp_parse(&parser, argc, argv, 0, &argidx, (void*)&parseres)) {
        // TODO: Error handling
    }

    printf("Gonna have to get libevent aren't I\nPassed options: ");
    for(int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");

    printf("dest ip: %s\n", (parseres.ip) ? parseres.ip : "<not specified>");

    return 0;
}