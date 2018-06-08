# bcstats

[![Build status](https://travis-ci.org/JonnyPtn/bcstats.svg?branch=master)](https://travis-ci.org/JonnyPtn/bcstats)
[![Build status](https://ci.appveyor.com/api/projects/status/ruacgilt9vd2lpqj?svg=true)](https://ci.appveyor.com/project/JonnyPtn/bcstats)

A tool for analysing bitcoin price history data from the [coindesk api](https://www.coindesk.com/api/).

## Usage

Usage:
```
  ./bcstats [OPTION...]

  -h, --help       Show this help
  -v, --verbose    Verbose output
  -f, --file arg   JSON file containing history data to analyze
  -r, --range arg  Date range to analyze data for [FROM TO] (YYYY-MM-DD)
  ```

## Building

### Requirements

A c++17 capable compiler.

Has been tested on Windows and Unix systems (see build status above)

This project uses cmake, so after cloning simply:

```
cmake .
cmake --build .
```

### Testing

To run tests, execute `./bctest`
