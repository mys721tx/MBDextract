# MBDextract
[![Build Status](https://travis-ci.org/mys721tx/MBDextract.svg?branch=master)](https://travis-ci.org/mys721tx/MBDextract)

By [Yishen Miao](https://github.com/mys721tx)

## Description
**MBDextract** will extract game scripts and other files from a MBD package.

## Installation

```console
./configure
make
make install
```

## Usage
```console
mbdextract file
```

### Known Issues
1. MBDextract 0.1 could not read from widecard.
2. MBDextract will encounter segfault when feeds with non MBD package.

## Author
* [Yishen Miao](https://github.com/mys721tx)

## License
[GNU General Public License, version 2](http://www.gnu.org/licenses/gpl-2.0.html)
