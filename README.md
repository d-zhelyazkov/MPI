# MPI

Parallel computing exercises in C++ with OpenMPI and OpenMP.

| Folder | What it is |
|---|---|
| `game_of_life/` | Conway's Game of Life, serial and MPI-parallel versions with timing, plus a live SDL view of the grid being processed ([details](game_of_life/README.md)) |
| `img_reconstruct/` | Image reconstruction from edge data, serial and MPI-parallel |
| `traffic/` | Traffic cellular-automaton simulation |
| `PI_OpenMP/` | Estimating π with OpenMP |
| `hellompi/`, `pingpong/` | MPI basics: process setup and point-to-point messaging |

Requires `libopenmpi-dev` (and `libsdl2-gfx-dev` for the Game of Life GUI).
