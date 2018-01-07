[![Build Status](https://travis-ci.org/msoeken/kitty.svg?branch=master)](https://travis-ci.org/msoeken/kitty)
[![Documentation Status](https://readthedocs.org/projects/libkitty/badge/?version=latest)](http://libkitty.readthedocs.io/en/latest/?badge=latest)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/4c1fc5fbf68b48e79d9ae91a63b5af87)](https://www.codacy.com/app/msoeken/kitty?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=msoeken/kitty&amp;utm_campaign=Badge_Grade)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


# kitty

<img src="https://cdn.rawgit.com/msoeken/kitty/master/kitty.svg" width="64" height="64" align="left" style="margin-right: 12pt" />
kitty is a C++-14 truth table library.  It provides efficient implementations for basic truth table manipulations and various algorithms.

[Read the full documentation.](http://libkitty.readthedocs.io/en/latest/?badge=latest)

## Example

The following code snippet generates truth tables for the 3-variable functions `sum` and `carry` for a 1-bit full-adder with carry.

```c++
#include <kitty/kitty.hpp>

dynamic_truth_table a( 3 ), b( 3 ), c( 3 );

create_nth_var( a, 0 );
create_nth_var( b, 1 );
create_nth_var( c, 2 );

const auto sum = a ^ b ^ c;
const auto carry = ternary_majority( a, b, c );
```

One can use `static_truth_table` instead of `dynamic_truth_table`, if the number of variables are known at compile-time.  The interface stays the same.

```c++
#include <kitty/kitty.hpp>

static_truth_table<3> a, b, c;

create_nth_var( a, 0 );
create_nth_var( b, 1 );
create_nth_var( c, 2 );

const auto sum = a ^ b ^ c;
const auto carry = ternary_majority( a, b, c );
```
