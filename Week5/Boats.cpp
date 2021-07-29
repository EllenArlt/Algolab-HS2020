#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


void testcase() {
  

  
  int n; std::cin >> n; //number of wizards
  //vector of boats each described by a 2-element-vector containing
  //its lenght and the position of its ring
  std::vector<std::vector<int>> boats(n,std::vector<int>(2)); 

  //boats
  for (int i = 0; i < n; ++i){
       int l; std::cin >> l; 
       int p; std::cin >> p; 
       boats[i][0] = l;
       boats[i][1] = p;

  }
  
  //sort the boats by the position of their rings
  std::sort(boats.begin(), boats.end(),
          [](const std::vector<int>& a, const std::vector<int>& b) {
  return a[1] < b[1];
  });
  
  int res = 1;//will store the final result (max # boats)
  int lastend = boats[0][1]; //at which position our current subset of boats ends
  int curend = 0;
  bool found = 1; //indicates if it can still be possible to add another boat after the furthest right one
  int i = 1; //i wil be the index of the currently looked at boat (as potential next boat)
  int j = 1; //j will indicate which boat we have to look at next, once we added the next boat

  //at any time the optimal choice is to add the boat that ends first from the current end x
  //of the last added boat. That boat is the one for which max(x+l,p) is minimal
  while(found){
    int potlastend = 10000000; //will store min(max(x+l,p)) over boats
    found = 0;
    i = j; //as j says where we need to move on in the vector
    while (i < n && boats[i][1] <= potlastend){//while we are still in the bounds of boats and p <= the current min(max(x+l,p)) (neccessary condition to become the next minimum)
      if(boats[i][1] >= lastend){
        found = 1;
        curend = std::max(boats[i][1],lastend + boats[i][0]); //max(x+l,p)
        if (curend < potlastend) {//if this boat is a better candidate wrt the current last end x, update variables
          potlastend = curend;
          j = i+1; //since max(x+l,p) >= p and all p are distinct, and boats are sorted by position, none of the boats before this one in boats can be tied anymore
        }
      }
      ++i;
      
    }
    if (found){//if found update variables corresponding to the fixed substed we decided on until now
      lastend = potlastend;
      ++res;
    }
    
  
  }
  //output result
  std::cout << res;

}



int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
     std::cout << "\n";
  }
  return 0;

}