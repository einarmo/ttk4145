Both C and python behave in the very thready way of randomly running scheduling
    the threads according to the OS, so we end up with numbers between -1000000 and 1000000.
    In the case of Python and C we usually end up relatively close to 0,
    with any difference accounted for by random processes running in the background.

With Go I here end up with 0, but the reason why is that the program actually locks the number of
CPUS to 1, since my VM only has 2 cores.
