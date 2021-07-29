#include <iostream>
#include <vector>


int even_pairs(std::vector<int> &S, int size){
  //precompoute (see solution to Even Pairs)
  int sum = 0;
  int even = 0;
  int odd = 0;
  for (int i = 0; i < size; ++i){
    sum += S[i];
    if (sum%2 == 0) ++even;
    else ++odd;
  }
  return even + (odd*(odd-1))/2 + (even*(even-1))/2;
}


//find # of border index quadrupels of submatrices of even sum over all entries
void testcase(){
  int n; std::cin >>n;
  std::vector<std::vector<int>> mat(n,std::vector<int>(n));
  std::vector<std::vector<int>> prec_mat(n,std::vector<int>(n));
  std::vector<int> sums(n,0);
  //compute partial sums from index 0 onwards along each column
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < n; ++j){
      std::cin >> mat[i][j];
      sums[j] += mat[i][j];
      prec_mat[i][j] = sums[j];
    }
  }
  
  int tot_sum = 0;
  //Use method from even Pairs on each row 
  for (int i1 = 0; i1 < n; ++i1){
    for (int i2 = i1; i2 < n; ++i2){
      std::vector<int> S(n); //sequence of partial sums starting at index i1
      for (int i = 0; i < n; ++i){
        if (i1 == 0) S[i] = prec_mat[i2][i];
        else S[i] = prec_mat[i2][i] - prec_mat[i1-1][i];
      }
      tot_sum += even_pairs(S,n);
    }
  }
  
  std::cout << tot_sum;
}
  


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
     std::cout << std::endl;
  }
  return 0;

}