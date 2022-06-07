#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include "operators.hpp"
#include "constructors.hpp"
#include "bit_operations.hpp"


namespace kitty{
template <typename TT>
class karnaugh_map{
  public:
  karnaugh_map() = delete;
  karnaugh_map(TT tt) : truth_table( tt ) {
      uint64_t num_var = log2(tt.num_bits());
      vars_col = num_var >> 1;
      vars_row = num_var - vars_col;
      col_seq = compute_seq_1ham_dist(vars_col);
      row_seq = compute_seq_1ham_dist(vars_row);
  }

  void print(){
      print_space(vars_col);
      std::cout << "    ";
      for(const auto& i : row_seq){
          std::cout << binary(i, vars_row);
          print_space(vars_row);
          std::cout << "    ";
      }
      std::cout << std::endl << std::endl;

      for(const auto &j : col_seq){
          std::cout << binary(j, vars_col);
          std::cout << "    ";
          for(const auto &i : row_seq){
              uint8_t middle_space = 0;
              if(vars_row > 2)
                  middle_space = vars_row / 2;
              print_space(middle_space);
              std::cout << get_bit(truth_table, (j << vars_row) + i);
              print_space((vars_row << 1) - 1 - middle_space);
              std::cout << "    ";
          }
          std::cout << std::endl << std::endl;
      }
  }

private:
  TT truth_table;
  unsigned vars_row;    // 1 << vars_row = number of columns in the k-map
  unsigned vars_col;    // 1 << vars_col = number of rows in the k-map
  std::vector<uint8_t> row_seq;
  std::vector<uint8_t> col_seq;

    std::string binary( uint8_t long n, uint8_t max_var ){
    std::string result;
    uint8_t count = 0u;

    do {
        result.insert(result.begin(), '0' + (n & 1) );
        count++;
    }while (n >>= 1);

    for(uint8_t i = 0; i < max_var - count; i++){
        result.insert(result.begin(), '0');
    }

    return result;
    }

    void print_space(uint8_t val){
        for(uint8_t i = 0; i < val; i++){
            std::cout << " ";
        }
    }

    std::vector<uint8_t> compute_seq_1ham_dist(uint8_t num_var){
        if(num_var == 1){
            return {0, 1};
        }
        else{
            std::vector<uint8_t> res = compute_seq_1ham_dist(num_var - 1);
            std::vector<uint8_t> res_rev (res.rbegin(), res.rend());
            for(auto &i : res_rev){
                i += (1 << (num_var - 1));
            }
            res.insert(res.end(), res_rev.begin(), res_rev.end());
            return res;
        }
    }

};

}
