# Boids

Boid simulation written in pure C using raylib.

![boids_gif](./assets/boids.gif)

Read more about boids here: [https://en.wikipedia.org/wiki/Boids](https://en.wikipedia.org/wiki/Boids).

## Usage

Please make sure [raylib](https://www.raylib.com/) is installed on your machine.

Compile:

```bash
clang -Wall -Wextra -O3 main.c src/boid.c src/boid_manager.c $(pkg-config --cflags --libs raylib) -o main
```

Run:

```bash
./main
```

## Citations

```bibtex
@inproceedings{Reynolds1987Boids,
  author    = {Craig W. Reynolds},
  title     = {Flocks, Herds and Schools: A Distributed Behavioral Model},
  booktitle = {Proceedings of the 14th Annual Conference on Computer Graphics and Interactive Techniques},
  series    = {SIGGRAPH '87},
  year      = {1987},
  pages     = {25--34},
  publisher = {ACM},
  address   = {New York, NY, USA},
  doi       = {10.1145/37401.37406}
}
```