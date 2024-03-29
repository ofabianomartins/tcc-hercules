Hercules
===========

This adds the hello-world process in hello-world.c to the platform build, which
prints "Hello-world" to stdout on startup.

The entire platform is built, with uip stack, radio drivers, routing, etc.
So it is not usually a simple build! The native platform is the default:

    make
    ./hello-world.native
    Starting Contiki
    Hello, world

When switching between ipv4 and ipv6 builds on a platform,

    make TARGET=<platform> clean

else the library for that platform will contain duplicate or unresolved
modules.

For example, using a loopback interface with the minimal-net platform:

    cd /examples/hello-world
    make TARGET=minimal-net
    ./hello-world.minimal-net
    Hello, world
    IP Address:  10.1.1.1
    Subnet Mask: 255.0.0.0
    Def. Router: 10.1.1.100
    ^C

    make TARGET=minimal-net clean
    make UIP_CONF_IPV6=1 TARGET=minimal-net
    ./hello-world.minimal-net
    Hello, world
    IPV6 Address: [aaaa::206:98ff:fe00:232]
    IPV6 Address: [fe80::206:98ff:fe00:232]
    ^C

Note to AVR Raven users: Output goes to UART1, not the LCD. To see it,

    make TARGET=avr-raven hello-world.elf

Load the .elf in AVR Studio and connect a hapsim terminal to the 1284p simulation.
