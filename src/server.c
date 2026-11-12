/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */

// I didn't want to write a server-side to check this

/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */
/** WRITTEN BY CLAUDE */

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct conn {
    int id;
    int got_first;
    unsigned long long total;
};

static int next_id;

static void read_cb(struct bufferevent *bev, void *arg)
{
    struct conn *c = arg;
    struct evbuffer *in = bufferevent_get_input(bev);
    size_t len = evbuffer_get_length(in);

    if (!c->got_first && len >= 4) {
        unsigned char b[4];
        evbuffer_copyout(in, b, sizeof b);
        printf("[conn %d] first bytes: %02x %02x %02x %02x\n",
               c->id, b[0], b[1], b[2], b[3]);
        c->got_first = 1;
    }
    c->total += len;
    evbuffer_drain(in, len);            /* discard; we only care that it arrived */
}

static void event_cb(struct bufferevent *bev, short what, void *arg)
{
    struct conn *c = arg;
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        printf("[conn %d] closed (%s), %llu bytes received\n", c->id,
               (what & BEV_EVENT_EOF) ? "eof" : "error", c->total);
        bufferevent_free(bev);          /* also closes the fd */
        free(c);
    }
}

static void accept_cb(struct evconnlistener *lev, evutil_socket_t fd,
                      struct sockaddr *addr, int socklen, void *arg)
{
    (void)socklen;
    int port = (int)(intptr_t)arg;
    struct event_base *base = evconnlistener_get_base(lev);
    struct conn *c = calloc(1, sizeof *c);
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    if (!c || !bev) {
        free(c);
        if (bev) bufferevent_free(bev); else evutil_closesocket(fd);
        return;
    }
    c->id = next_id++;
    printf("[conn %d] accepted on port %d (peer port %d)\n", c->id, port,
           ntohs(((struct sockaddr_in *)addr)->sin_port));

    bufferevent_setcb(bev, read_cb, NULL, event_cb, c);
    bufferevent_enable(bev, EV_READ);
}

static void signal_cb(evutil_socket_t sig, short what, void *arg)
{
    (void)sig; (void)what;
    event_base_loopexit(arg, NULL);
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "usage: %s <base_port> [nports=1]\n", argv[0]);
        return 1;
    }
    int base_port = atoi(argv[1]);
    int nports = argc == 3 ? atoi(argv[2]) : 1;
    if (base_port < 1 || nports < 1 || base_port + nports > 65536) {
        fprintf(stderr, "bad port range\n");
        return 1;
    }

    struct event_base *base = event_base_new();
    if (!base) { fprintf(stderr, "event_base_new failed\n"); return 1; }

    struct evconnlistener **ls = calloc(nports, sizeof *ls);
    for (int i = 0; i < nports; i++) {
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof sin);
        sin.sin_family      = AF_INET;
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_port        = htons(base_port + i);
        ls[i] = evconnlistener_new_bind(base, accept_cb,
                    (void *)(intptr_t)(base_port + i),
                    LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
                    (struct sockaddr *)&sin, sizeof sin);
        if (!ls[i]) {
            fprintf(stderr, "cannot listen on port %d\n", base_port + i);
            return 1;
        }
    }
    printf("listening on ports %d-%d\n", base_port, base_port + nports - 1);

    struct event *sig_ev = evsignal_new(base, SIGINT, signal_cb, base);
    event_add(sig_ev, NULL);

    event_base_dispatch(base);

    event_free(sig_ev);
    for (int i = 0; i < nports; i++) evconnlistener_free(ls[i]);
    free(ls);
    event_base_free(base);
    return 0;
}