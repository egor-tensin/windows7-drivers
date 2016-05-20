# test driver utilities

[test] driver usage examples.

## Library

[test] driver's virtual device is wrapped into a separate library `libtest`
using [libservice].
`#include <libtest/all.hpp>` and link with `libtest.lib` to use the library.

## Utilities

### exchange_ints.exe

    Usage: exchange_ints.exe N

Parses its argument as an `unsigned int` and exchanges it with the one stored
in [test] driver's memory.
For example:

    > exchange_ints.exe 1
    42

    > exchange_ints.exe 32
    1

    > exchange_ints.exe 100500
    32

## See also

* [Building]
* [License]



[building]: ../README.md#building
[license]: ../../README.md#license
[test]: ../../src/test
[libservice]: ../libservice/README.md
