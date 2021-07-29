#include <iostream>

//Compute the sum from 1 to n
int main() {
  
  
  int t; //num test cases
  std::cin >> t;
  for (int iter_tests =0; iter_tests<t; iter_tests++) {
    int n; //num summands
    std::cin >> n;
    int sum = 0;
    for (int iter_sum =0; iter_sum<n; iter_sum++) {
    int a;
    std::cin >> a;
    sum += a;
    
    
  }

  std::cout << sum << std::endl;
  }
  
  
}