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
make {v,val}    # Build debug binary and run it through valgrind
make {c,clean}  # Clean src dir
```

These (minus clean, obviously) all output a `main` executable in `src`
