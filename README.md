# CS4422 Lab 3 (DOSSER)

Write a program that can make many TCP connections to a destination IP address. For example, you may use all your local ports (1024-65535) to connect to a given port (such as port 22) on a destination computer.

## Program inputs

1. Destination IP address and port number
2. The total number of connections;

The program will then make TCP connections to the destination IP.

***please do not use Windows or Linux shell script in this assignment.**

## Building

The Makefile in this directory has 4 options:

```bash
make debug      # Build with debug options (compiler warnings, debugging symbols for gdb, asan)
make release    # Build with optimization options (-O3, no warnings)
make {c,clean}  # Clean src dir
```

These (minus clean, obviously) all output a `dosser` and `server` executable in `src`

## Running

Running is simple:

```bash
# server
./server starting-port extra-ports # don't worry about extra ports, it's basically unused. Just enter 1

# dosser
./dosser -i target.ip.addr -p port-num -s start-port-range -e end-port-range
```

The server will output the first byte acquired from the client, then output the number of bytes sent by each connection once a connection drops. Hitting ctrl+c will cleanly exit both executables

Example:

```text
# ./server 9999 1
listening on ports 9999-9999
[conn 0] accepted on port 9999 (peer port 40978)
[conn 1] accepted on port 9999 (peer port 40992)
[conn 0] first bytes: de ad de a1
[conn 1] first bytes: de ad de a1
[conn 1] closed (eof), 7470540 bytes received
[conn 0] closed (eof), 7470540 bytes received

# ./dosser -i 127.0.0.1 -p 9999 -s 16777 -e 16778
Set ip: 127.0.0.1
Set dest port: 9999
Set pr start: 16777
Set pr end: 16778
Running libevent version 2.1.12-stable, backend: epoll
SIGINT received, shutting down
```
