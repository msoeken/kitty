Examples
========

NPN canonization
----------------

The following two examples show NPN canonization on all :math:`n`
input functions or on a selection of randomly generated functions.

.. literalinclude:: ../examples/npn_enumeration.cpp
   :language: c++
   :lines: 26-58
   :caption: examples/npn_enumeration.cpp

Output:

::

   $ time ./examples/npn_enumeration
   [i] enumerated 65536 functions into 222 classes.
   ./examples/npn_enumeration  0.15s user 0.00s system 97% cpu 0.159 total

.. literalinclude:: ../examples/npn_random.cpp
   :language: c++
   :lines: 26-61
   :caption: examples/npn_random.cpp

Output:

::

   $ time ./examples/npn_random
   [i] partitioned 10000 random functions into 10000 classes.
   ./examples/npn_random  2.20s user 0.01s system 99% cpu 2.232 total
