package main

import (
    . "fmt"
    "runtime"
)

var i = 0

func cases(quit, dec, inc chan int) {
    tt := 0
    for {
        select {
            case <-dec:
                tt++
            case <-inc:
                tt--
            case <-quit:
                Println("The magic number is:", tt)
                return
        }
    }
}


func incrementing(inc, fin chan int) {
    for j := 0; j < 1000000; j++ {
        inc <- 1
    }
    fin <- 1
}

func decrementing(dec, fin chan int) {
    for j := 0; j < 1000000; j++ {
        dec <- 1
    }
    fin <- 1
}

// I just realized that the reason why I get 0 here is that this ensures that we only use 1 core
// since I have only 2 cpus on my VM.
func main() {
    runtime.GOMAXPROCS(runtime.NumCPU())    // I guess this is a hint to what GOMAXPROCS does...
    quit := make(chan int)
    dec := make(chan int)
    inc := make(chan int)
    fin := make(chan int)
    go incrementing(inc, fin)
    go decrementing(dec, fin)
    go cases(quit, dec, inc)
    <-fin
    <-fin
    quit <- 1
}
