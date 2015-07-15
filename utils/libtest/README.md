# libtest

[test](../../src/test) usage examples.

## Usage

### libtest.lib

Wraps `test`'s virtual device interface using [libservice](../libservice).
Include the headers by `#include`ing `libtest/all.hpp`, which includes all the
other header files.

### exchange_ints.exe

    Usage: exchange_ints.exe N

Parses a given `unsigned int` and exchanges it with the value stored in
driver's memory.
Usage example (assuming `test` is already loaded):

    > exchange_ints.exe 1
    42

    > exchange_ints.exe 32
    1

    > exchange_ints.exe 100500
    32

## Building

See [Building](../README.md#building).

## Licensing

This project is licensed under the terms of the MIT License.
See [Licensing](../../README.md#licensing) for details.
