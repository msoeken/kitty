Reference
=========

Dynamic truth table
-------------------

A dynamic truth table can be initialized with a number of variables
that is computed at runtime.

.. doxygenfile:: dynamic_truth_table.hpp

Static truth table
------------------

A static truth table must be initialized with a number of variables
that is computed at compile time.  It performs much faster than the
dynamic truth table.  Also it is optimized for a small number of
variables, i.e., up to 6 variables.  Then a truth table fits into a
single block of 64 bits.  The interface makes this optimization
transparent to the user.

.. doxygenfile:: static_truth_table.hpp

Bit operations
--------------

.. doxygenfile:: bit_operations.hpp

Constructors
------------

.. doxygenfile:: constructors.hpp

Operations
----------

.. doxygenfile:: operations.hpp

Operators
---------

.. doxygenfile:: operators.hpp

