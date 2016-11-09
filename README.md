# Bindings for Pairing-Based Cryptography (libpbc)

This project offers (partial) C++14 and SWIG bindings for libpbc.
Noted that not all of libpbc C functions have been ported.

## How to use?

### C++14

C++14 binding is head-only. Just copy the files inside `include/` into your project.

```cpp
#include <pbc/pbc.hpp>
using namespace pbc;
```

### SWIG

[SWIG](http://www.swig.org) offers bindings in a variety of high-level programming languages.
Currently Python and Ruby are supported and tested.

* Python
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_PYTHON=ON
make
```
Python library files will be built into `swig/python`.

* Ruby
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_RUBY=ON
make
```
Ruby library files will be built into `swig/ruby`.

## Development

To run test case:

```bash
make test
#OR
make test-verbose
```

## Author

* [Xu Cheng](https://xuc.me)

## License

MIT License.

