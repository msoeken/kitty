Change Log
==========

v0.2 (not yet released)
-----------------------

* Generic algorithms: ``for_each_block``, ``for_each_block_reversed``
  `#14 <https://github.com/msoeken/kitty/pull/14>`_

* Moved generic operations from `operations.hpp` to `algorithm.hpp`
  `#14 <https://github.com/msoeken/kitty/pull/14>`_

* Added iterators to all truth tables: ``begin``, ``end``, ``rbegin``, ``rend``, ``cbegin``, ``cend``, ``crbegin``, ``crend``
  `#14 <https://github.com/msoeken/kitty/pull/14>`_

v0.1 (October 22, 2017)
-----------------------

* Initial release
* Data structures ``static_truth_table`` and ``dynamic_truth_table``
  `#1 <https://github.com/msoeken/kitty/pull/1>`_

* Bit functions: ``set_bit``, ``get_bit``, ``clear_bit``, ``clear``
  `#1 <https://github.com/msoeken/kitty/pull/1>`_
  `#8 <https://github.com/msoeken/kitty/pull/8>`_

* Constructors: ``create_nth_var``, ``create_from_binary_string``, ``create_from_hex_string``, ``create_random``, ``create_from_cubes``, and ``create_majority``
  `#1 <https://github.com/msoeken/kitty/pull/1>`_
  `#4 <https://github.com/msoeken/kitty/pull/4>`_
  `#5 <https://github.com/msoeken/kitty/pull/5>`_
  `#9 <https://github.com/msoeken/kitty/pull/9>`_
  `#11 <https://github.com/msoeken/kitty/pull/11>`_

* Unary and binary operations: ``unary_not``, ``unary_not_if``, ``binary_and``, ``binary_or``, and ``binary_xor``
  `#2 <https://github.com/msoeken/kitty/pull/2>`_
  `#8 <https://github.com/msoeken/kitty/pull/8>`_

* Ternary operations: ``ternary_majority`` and ``ternary_ite``
  `#3 <https://github.com/msoeken/kitty/pull/3>`_

* Binary predicates: ``equal``, ``less_than``
  `#4 <https://github.com/msoeken/kitty/pull/4>`_
  `#8 <https://github.com/msoeken/kitty/pull/8>`_

* Predicates: ``has_var``, ``is_const0``
  `#11 <https://github.com/msoeken/kitty/pull/11>`_

* Operators: ``~``, ``&``, ``&=``, ``|``, ``|=``, ``^``, ``^=``, ``==``, ``<``
  `#8 <https://github.com/msoeken/kitty/pull/8>`_
  `#11 <https://github.com/msoeken/kitty/pull/11>`_

* Swap adjacent variables: ``swap_adjacent_inplace``, ``swap_adjacent``
  `#6 <https://github.com/msoeken/kitty/pull/6>`_

* Swap variables: ``swap_inplace``, ``swap``
  `#8 <https://github.com/msoeken/kitty/pull/8>`_

* Flip variable: ``flip_inplace``, ``flip``
  `#7 <https://github.com/msoeken/kitty/pull/7>`_

* Enumerate truth tables: ``next_inplace``, ``next``
  `#10 <https://github.com/msoeken/kitty/pull/10>`_

* Compute co-factors: ``cofactor0_inplace``, ``cofactor0``, ``cofactor1_inplace``, ``cofactor1``
  `#11 <https://github.com/msoeken/kitty/pull/11>`_

* Compute minimum base based on functional support: ``min_base_inplace``, ``expand_inplace``
  `#12 <https://github.com/msoeken/kitty/pull/12>`_

* Compute hash values for truth table
  `#13 <https://github.com/msoeken/kitty/pull/13>`_

* NPN canonization: ``exact_npn_canonization``, ``create_from_npn_config``
  `#8 <https://github.com/msoeken/kitty/pull/8>`_

* Compute ISOP representation: ``isop``
  `#11 <https://github.com/msoeken/kitty/pull/11>`_
