Cube data structure
===================

The header ``<kitty/cube.hpp>`` implements a cube data structure to
represent a cube with up to 32 literals.  This data structure is used
by algorithms that find 2-level representations for truth tables,
e.g., ``isop`` and ``esop_from_optimum_pkrm``.

The class :cpp:class:`kitty::cube` provides the following public
member functions.

+--------------------------------------+-------------------------------------------------------------------------+
| Function                             | Description                                                             |
+======================================+=========================================================================+
| ``cube()``                           | Constructs the empty cube                                               |
+--------------------------------------+-------------------------------------------------------------------------+
| ``cube(bits, mask)``                 | Constructs a cube from bits and mask                                    |
+--------------------------------------+-------------------------------------------------------------------------+
| ``num_literals()``                   | Returns number of literals                                              |
+--------------------------------------+-------------------------------------------------------------------------+
| ``distance(that)``                   | Returns the distance to another cube                                    |
+--------------------------------------+-------------------------------------------------------------------------+
| ``operator==(that)``                 | Checks whether two cubes are equivalent                                 |
+--------------------------------------+-------------------------------------------------------------------------+
| ``operator!=(that)``                 | Checks whether two cubes are not equivalent                             |
+--------------------------------------+-------------------------------------------------------------------------+
| ``merge(that)``                      | Merges two cubes of distance-1                                          |
+--------------------------------------+-------------------------------------------------------------------------+
| ``add_literal(var_index, polarity)`` | Add literal to cube                                                     |
+--------------------------------------+-------------------------------------------------------------------------+
| ``remove_literal(var_index)``        | Removes literal from cube                                               |
+--------------------------------------+-------------------------------------------------------------------------+
| ``nth_var_cube(var_index)``          | Constructs the elementary cube representing a single variable           |
+--------------------------------------+-------------------------------------------------------------------------+
| ``pos_cube(k)``                      | Constructs the elementary cube containing the first k positive literals |
+--------------------------------------+-------------------------------------------------------------------------+
| ``neg_cube(k)``                      | Constructs the elementary cube containing the first k negative literals |
+--------------------------------------+-------------------------------------------------------------------------+
| ``print(length, os)``                | Prints a cube                                                           |
+--------------------------------------+-------------------------------------------------------------------------+

You can use ``kitty::hash<cube>`` to hash cubes, e.g., in a hash map or set.
