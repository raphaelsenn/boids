# Tests

Please make sure [Unity](https://github.com/ThrowTheSwitch/Unity) is installed on your machine.

Compile:

```bash
clang -Wall -Wextra -O0 -g -I./src -I./libs/unity/src ./src/boid.c ./tests/test_boid.c ./libs/unity/src/unity.c $(pkg-config --cflags --libs raylib) -o test_boid
```

Run:

```bash
./test_boid
```
