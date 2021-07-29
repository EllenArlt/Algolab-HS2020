#include <iostream>
#include <vector>







void testcase() {
  
  int n; std::cin >> n; //number dominoes

  
    
  std::vector<int> h(n); //heights of dominoes
  
  for (int i = 0; i < n; ++i){
       int a; std::cin >> a;
 
       h[i] = a;
  }
  
  int num_tiles = 1; //number of tiles falling
  int range = h[0]-1; //current remaining hit range after last domino
  int index = 1;
  
  while (range > 0 && index < n){
    num_tiles += 1;
    if (h[index] > range){
      range = h[index];
    }
    index += 1;
    range -= 1;
    
  }
  

    
      std::cout << num_tiles;
        
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