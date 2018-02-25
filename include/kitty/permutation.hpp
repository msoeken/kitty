/* kitty: C++ truth table library
 * Copyright (C) 2017-2018  EPFL
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*!
  \file permutation.hpp
  \brief Efficient permutation of truth tables

  \author Mathias Soeken
*/

#pragma once

#include <numeric>
#include <vector>

#include "bit_operations.hpp"
#include "print.hpp"

namespace kitty
{

template<typename TT>
std::pair<TT, TT> compute_permutation_masks_pair( const TT& tt, std::vector<uint32_t>& left, std::vector<uint32_t>& right, unsigned step )
{
  const auto n = tt.num_vars();
  const auto loops = ( 1 << ( n - 1 - step ) );
  const auto diff = ( 1 << step );

  const auto offset = 1 << ( n - 1 );

  struct node_t
  {
    std::vector<uint32_t>::iterator a, b; /* numbers in left or right */
    unsigned lf, rf;                     /* left field right field */
    bool visited = false;
  };

  std::vector<node_t> nodes( tt.num_bits() );

  /* compute graph */
  auto idx1 = 0u, idx2 = 0u;
  auto it1 = std::begin( left );
  for ( auto l1 = 0; l1 < loops; ++l1 )
  {
    for ( auto c1 = 0; c1 < diff; ++c1 )
    {
      auto it2 = std::begin( right );
      idx2 = offset;

      nodes[idx1].a = it1;
      nodes[idx1].b = it1 + diff;

      for ( auto l2 = 0; l2 < loops; ++l2 )
      {
        for ( auto c2 = 0; c2 < diff; ++c2 )
        {
          if ( idx1 == 0u )
          {
            nodes[idx2].a = it2;
            nodes[idx2].b = it2 + diff;
          }

          /* pair elements */
          auto& n1 = nodes[idx1];
          auto& n2 = nodes[idx2];

          /* connect graph */
          if ( *n1.a == *n2.a )
          {
            n1.lf = idx2;
            n2.lf = idx1;
          }
          else if ( *n1.a == *n2.b )
          {
            n1.lf = idx2;
            n2.rf = idx1;
          }
          if ( *n1.b == *n2.a )
          {
            n1.rf = idx2;
            n2.lf = idx1;
          }
          else if ( *n1.b == *n2.b )
          {
            n1.rf = idx2;
            n2.rf = idx1;
          }

          ++it2;
          ++idx2;
        }
        it2 += diff;
      }
      ++it1;
      ++idx1;
    }
    it1 += diff;
  }

  /* traverse graph and compute masks */
  auto mask_left = tt.construct();
  auto mask_right = tt.construct();

  while ( true )
  {
    auto idx = 0;

    while ( idx < offset && nodes[idx].visited )
    {
      ++idx;
    }

    if ( idx == offset )
      break;

    auto left_side = true;
    auto nr = *nodes[idx].a;
    auto start = idx;

    do
    {
      auto& n = nodes[idx];

      auto match = *n.a == nr;

      nr = match ? *n.b : *n.a;
      idx = match ? n.rf : n.lf;
      n.visited = true;

      if ( left_side != match )
      {
        std::swap( *n.a, *n.b );

        if ( left_side )
        {
          set_bit( mask_left, std::distance( left.begin(), n.a ) );
        }
        else
        {
          set_bit( mask_right, std::distance( right.begin(), n.a ) );
        }
      }

      left_side = !left_side;

    } while ( idx != start );
  }

  return std::make_pair( mask_left, mask_right );
}

namespace detail
{
  void print_permutations( const std::vector<uint32_t>& left, const std::vector<uint32_t>& right )
  {
    std::cout << "left (" << left.size() << ") =";
    for ( auto i : left )
    {
      std::cout << " " << i;
    }
    std::cout << "\nright (" << right.size() << ") =";
    for ( auto i : right )
    {
      std::cout << " " << i;
    }
    std::cout << "\n" << std::flush;
  }
}

template<typename TT>
std::vector<TT> compute_permutation_masks( const TT& tt, const std::vector<uint32_t>& permutation )
{
  std::vector<TT> masks;

  std::vector<uint32_t> left( permutation.size() ), right = permutation;
  std::iota( left.begin(), left.end(), 0u );

  for ( auto i = 0u; i < tt.num_vars() - 1u; ++i )
  {
    const auto pair = compute_permutation_masks_pair( tt, left, right, i );

    masks.insert( masks.begin() + i, pair.second );
    masks.insert( masks.begin() + i, pair.first );
  }

  auto mask = tt.construct();
  for ( uint64_t i = 0u; i < ( tt.num_bits() >> uint64_t( 1 ) ); ++i )
  {
    if ( left[i] != right[i] )
    {
      set_bit( mask, i );
    }
  }
  masks.insert( masks.begin() + tt.num_vars() - 1, mask );

  return masks;
}

} // namespace kitty