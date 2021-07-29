#include <iostream>


//count number of pairs of indices i, j such that the sum of integers from index
//i to index j is even for a given sequnce of n integers
int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; //num test cases
  std::cin >> t;
  for (int iter_tests =0; iter_tests<t; iter_tests++) {
    int n; //num summands
    std::cin >> n;
    int evens = 0;
    int odds = 0;
    int psum = 0;
    for (int iter_sum =0; iter_sum<n; iter_sum++) {
    int a;
    std::cin >> a;
    psum += a;
    if (psum % 2 == 0){
      evens += 1;
    }
    else {
      odds += 1;
    }
    
    
  }
  int res = 0;
  res += (evens*(evens-1))/2;
  res += (odds*(odds-1))/2;
  res += evens;
  
  //std::cout << evens << " ";
  //std::cout << odds << " ";
  std::cout << res << std::endl;
  }
  
  return 0;
}