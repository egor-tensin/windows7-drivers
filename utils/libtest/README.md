# test driver utilities

[test] driver usage examples.

[test]: ../../src/test

## Usage

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

* [Building the utilities]
* [License]

[Building the utilities]: ../README.md#building-the-utilities
[License]: ../../README.md#license
