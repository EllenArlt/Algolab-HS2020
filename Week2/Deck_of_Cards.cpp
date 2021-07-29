#include <iostream>
#include <vector>
#include <cmath>



void testcase2() {
  
  int n; std::cin >> n; //number of cards
  int k; std::cin >> k; //target value


  std::vector<int> deck(n);  //deck of cards in order
  int val;

  for (int i = 0; i < n; ++i){
       std::cin >> val;
       deck[i] = val;

  }
  
  
  //remember best indices found yet
  int ires = 0;
  int jres = 0;
  int solval = 0; //best value encountered yet
  int curval = 0; //currently considered value
   
  int i = 0;
  int j = 0;
  curval = deck[0];
  
  //we use a sliding window approach 
  
  //while we are still in the bounds of deck and we have not encountered a 
  //perfect pair yet (if so we need to look no further)
  while(j < n && !(solval== k)){
    
    //check if the current pair is the best one encountered yet (better value)
    
    /*Note we are never checking a lexicographically smaller pair than before, hence by
    not changing the best pair when we get an equally good value, we are keeping the
    lexicographically smallest such solution*/
    if(std::abs(k-curval) < std::abs(k-solval)){
           solval = curval;
           ires = i;
           jres = j;
    }
    
    if (curval < k) {
      ++j;
      curval += deck[j];
    }
    else {
      curval -= deck[i];
      ++i;
      if (i > j){
        j = i;
        curval = deck[j];
      }
    }
    
    
      
  }
  
  //output incices of best pair encountered  
  std::cout << ires << " " << jres;
        
  
  
}



int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase2(); // Solve a particular test case
     std::cout << std::endl;
  }
  return 0;

}