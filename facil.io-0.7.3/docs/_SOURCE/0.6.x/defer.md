---
title: facil.io - lib defer, an event scheduling and thread pool library in C.
sidebar: 0.6.x/_sidebar.md
---
# `defer` - An event scheduling library in C.

`defer` is a simple thread pool library that defaults to POSIX threads (and could be easily ported to any thread system).

It uses a combination of micro-sleep and spin-locks for load-balancing across threads, making it more performant then the conditional variable approach as well as more portable.

Unlike most thread pool libraries, `defer` allows for two pointers to be passed to each task, allowing greater versatility and optimizations. For example, `facil.io` uses this approach to perform interactions between objects without requiring any additional allocations (which is very important for larger object collections).

The library is conveniently documented inside it's `defer.h` file.

`defer` can run in a single thread mode, without using a thread pool:

```c
#include "defer.h"
#include <stdio.h>
// an example task
void say_hi(void * arg1, void * arg2) {
  (void)arg2;
  printf("%s\n", (char * )arg1);
}

// an example usage
int main(void) {
  defer(say_hi, "Hello There!", NULL);
  printf("Running all scheduled tasks...\n");
  defer_perform();
  printf("Done.\n");
}
```

Or using a thread pool:

```c
#include "defer.h"
#include <stdint.h>
#include <stdio.h>
// an example task
void say_hi(void * arg1, void * arg2) {
  printf("%s (%lu)\n", (char * )arg1, (uintptr_t)arg2);
}

// an example usage
int main(void) {
  for (uintptr_t i = 0; i < 64; i++) {
    defer(say_hi, "Hello There!", (void *)i);
  }
  printf("Starting thread pool...\n");
  pool_pt pool = defer_pool_start(8);
  printf("Signaling thread pool to finish...\n");
  defer_pool_stop(pool);
  printf("... Waiting for thread pool to finish.\n");
  defer_pool_wait(pool);
  printf("Done.\n");
}
```

This is actually most of the API right there, but if you need more details you can find them in the [`defer.h` header file](https://github.com/boazsegev/facil.io/blob/master/lib/facil/core/defer.h).
