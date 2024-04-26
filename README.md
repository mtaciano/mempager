# Memory paging simulation

## About
This program aims at simulating two different paging algorithms hit-and-miss ratios as they move pages from the swap to the RAM.

The first one is a `FIFO` (First In First Out) pager, the first page loaded in memory is the first to get released independently of its modified or referenced status.

The second algorithm is a `CLOCK` pager, it uses a circular list to find the first non-referenced page that is loaded and release it from memory.

### Building
To build the program, you can call `cmake` with the configuration you want, or use the `./config.sh` script provided, which is the configuration that I used to write this program.

After that run `cmake --build build` and the executable will be in the `build` folder.

### Running
To run this program you can do:
```sh
./mempager <fifo|clock> <uniform|normal> <seed>
```
Note that the seed is optional, and used if you want both pagers to have the same inputs.

## License
MIT License.
