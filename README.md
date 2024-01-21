# tracker

Tracker is an Ace of Spades master v31 implementation.

## Requirements

Assuming you already have basic build tools such as GCC/CMake install, you only need to
install these libraries and their `-dev` packages:

- json-c
- pthreads
- libmicrohttpd

## Installing

```
mkdir build && cd build
cmake ..
make -j`nproc`
./Tracker
```

## Credits

Base taken from [SpadesX](https://github.com/SpadesX/SpadesX).

## License

[GNU General Public License v3.0](LICENSE)
