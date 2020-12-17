/* kitty: C++ truth table library
 * Copyright (C) 2017-2020  EPFL
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
  \file threshold_identification.hpp
  \brief Threshold logic function identification

  \author CS-472 2020 Fall students
*/


// Seriem Sid-Ahmed ALGORITHM


#pragma once

#include <vector>
#include <iostream>
#include <lpsolve/lp_lib.h> /* uncomment this line to include lp_solve */
#include "traits.hpp"
#include "operators.hpp"
#include "operations.hpp"
#include "properties.hpp"
#include "implicant.hpp"
#include "constructors.hpp"
#include "bit_operations.hpp"
#include "isop.hpp"

using namespace std;


namespace kitty
{

/*! \brief Threshold logic function identification

  Given a truth table, this function determines whether it is a threshold logic function (TF)
  and finds a linear form if it is. A Boolean function is a TF if it can be expressed as

  f(x_1, ..., x_n) = \sum_{i=1}^n w_i x_i >= T

  where w_i are the weight values and T is the threshold value.
  The linear form of a TF is the vector [w_1, ..., w_n; T].

  \param tt The truth table
  \param plf Pointer to a vector that will hold a linear form of `tt` if it is a TF.
             The linear form has `tt.num_vars()` weight values and the threshold value
             in the end.
  \return `true` if `tt` is a TF; `false` if `tt` is a non-TF.
*/    int powerof2(int n){
        int sortie = 1;
        for ( auto i = 1; i <= n; ++i ){
            sortie = 2*sortie;
        }
        return sortie;}

template<typename TT, typename = std::enable_if_t<is_complete_truth_table<TT>::value>>
bool is_threshold( const TT& tt, std::vector<int64_t>* plf = nullptr )
{ 
  TT tt2 = tt ; // We can't flip a const so we need to use a second truth table to flip it
  std::vector<int64_t> linear_form;
  std::vector<int8_t> positivevect ; // Store the positives variables
  std::vector<int8_t> negativevect ; // Store the negatives variables.

// Cube that will be used to get the onset and offset minterms for the constraints
  const auto onset_cube = isop(tt2);
  const auto offset_cube = isop(unary_not(tt2));
// Boolean that we help us to know if we find a positive and a negative bit to return false if it's the case
  bool neg = false;  
  bool pos = false;
  bool posandneg = false;
// Check if a truth table is positive or negative or binate and return false if it is binate 
  for (auto i = 0 ; i < tt.num_vars() ; i++){
    neg = false;
    pos = false;
    posandneg = false;
    auto const cof0 = cofactor0(tt,i);
    auto const cof1 = cofactor1(tt,i);
    for (auto k = 0 ; k < (powerof2((tt.num_vars()-1))); k ++ ){
      if(get_bit(cof0,k)< get_bit(cof1,k)){
        pos = true ; 
      }
      else if(get_bit(cof0,k)> get_bit(cof1,k)) {
        neg = true ;
      }
      else if (get_bit(cof0,k) == get_bit(cof1,k)){
        posandneg = true ;
      }
    }
    if (pos == neg){ // The case where a variable has positive and negative bits
      if(posandneg != true){ // If the bits are equal  pos = false and neg = false so pos = neg but we don't want to return false
        return false;
      }
     
    }
    if( pos && neg && posandneg){
      return false ;

    }
    if (neg == true){
        negativevect.push_back(i); // Store the negatives variables to use them for linear_form
        tt2 = flip(tt2, i); // Flip the negatives variables in order to work with a positive truth table
        
      }
    else{
        positivevect.push_back(i); // Not very useful we don't use the positives variables
      }
}
        
        lprec *lp;
        int Ncol, *colno = NULL, ret = 0;
        REAL *row = NULL;

        /* We will build the model row by row*/
        Ncol = tt.num_vars() + 1; // Num_vars weights and the treshold value
        lp = make_lp(0, Ncol);
        if (lp == NULL) {
            ret = 1 ; /* couldn't construct a new model... */
            return false;
        }
        /* let us name our variables. Not required, but can be useful for debugging */
        for(auto i = 1 ; i != tt.num_vars()-1; i++){
          set_col_name(lp, i, "x");
        }

         /* create space large enough for one row */
        colno = (int*) malloc(Ncol * sizeof(*colno));
        row = (REAL*) malloc(Ncol * sizeof(*row));
        if ((colno == NULL) || (row == NULL)) {
            ret = 2 ;
            return false;
        }

        set_add_rowmode(lp, TRUE);/* makes building the model faster if it is done rows by row */

        if (ret == 0) {

          // We just check that all the weights wi and the treshold value are positives 

          for(int j = 0; j <= tt.num_vars(); j++){
            colno[0] = j+1;
            row[j] = 1;
            add_constraintex(lp, 1, row, colno, GE, 0); // w1 + w2 ... + T > 0 
          }
        }
        if (ret == 0) {
            // We compute the onset minterms inequalities
            for (auto k = 0; k <= onset_cube.size()-1; k++) { // We compute for all the minterms
                for (auto i = 0; i < tt.num_vars(); i++) {
                    if (onset_cube.at(k).get_mask(i) == true) {
                        colno[i] =i+1;
                        row[i] = 1;
                    } 
                    
                    else {
                        colno[i] =i+1;
                        row[i] = 0;
                    }
                }
                colno[tt.num_vars()]=tt.num_vars()+1;  // We compute separately the treshold value
                row[tt.num_vars()] = -1; // We move the treshold on the left of the inequalities

                // w1*x1 + .... wnum_vars*xnum_vars -T >= 0   with xi the onset minterms

                add_constraintex(lp, Ncol, row, colno, GE, 0); 
            }
        }
            if (ret == 0) {
                //We compute the offset minterms inequalities 
                for (auto k = 0; k <= offset_cube.size()-1; k++) { // We compute for all the minterms
                    for (auto i = 0; i < tt.num_vars(); i++) {
                        if (offset_cube.at(k).get_mask(i)==false) {
                            colno[i] =i+1;
                            row[i] = 1;
                        } 
                        else {
                            colno[i] =i+1;
                            row[i] = 0;
                        }
                    }

                    colno[tt.num_vars()] = tt.num_vars()+1; // We compute separately the treshold value
                    row[tt.num_vars()] = -1; // We move the treshold to the left of the inequalities

                    // w1*x1 + .... wnum_vars*xnum_vars -T <= 0  with xi the offset minterms 
                    
                    add_constraintex(lp, Ncol, row, colno, LE, -1); 
                }
            }
                if (ret == 0) {
                    set_add_rowmode(lp, FALSE); /* rowmode should be turned off again when done building the model */

                    // We check that the sum of the weights values and treshold values are minimize ( min(w1+...w_num_vars + T ))
                    for (int j = 0; j < Ncol; j++) {
                        colno[j] = j + 1;
                        row[j] = 1;
                    }
                    set_obj_fnex(lp, Ncol, row, colno); // This is the objective function that we minimize after (w1+....w_num_vars+T)
                }
                    /* set the object direction to minimize */
                    set_minim(lp);
                    /* just out of curioucity, now show the model in lp format on screen */
                    /* this only works if this is a console application. If not, use write_lp and a filename */
                   // write_LP(lp, stdout);  uncomment to see the constraints
                    /* write_lp(lp, "model.lp"); */
                    /* I only want to see important messages on screen while solving */
                    set_verbose(lp, IMPORTANT);
                    /* Now let lpsolve calculate a solution */
                    ret = solve(lp);
                   if (ret == 0){
                    get_variables(lp, row);

                    for (int j = 0; j <= tt.num_vars(); j++) {
                        linear_form.push_back(row[j]); // We put all the results in linear form vector 
                    }
                   // All the variables that were negative and were changed into positives are again set negatives
                    for (auto k : negativevect) {
                        linear_form.at(k) = -linear_form.at(k);
                        linear_form.at(Ncol-1) = linear_form.at(Ncol-1) + linear_form.at(k);
                    }
                  if ( plf )
              	  {
		              *plf = linear_form; 
	                }
                /* free allocated memory to avoid issues with malloc in order to not break your computer */
                if ( colno != NULL )
                    free( colno );
                if ( row != NULL )
                    free( row );
                if ( lp != NULL )
                /* clean up such that all used memory by lpsolve is freed */
                    delete_lp( lp );
                return true;
            }
        }
    }
