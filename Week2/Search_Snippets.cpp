#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

/*Approach similar to sliding window. At each iteration we take out the first
 occuring word in the currently considered snippet and replace it by its next 
 occurence*/

void testcase() {
  

  
  int n; std::cin >> n; //number of words
  

  std::vector<int> m(n); //number of word occurences of each word 
  
  for (int i = 0; i < n; ++i){
       int x; std::cin >> x; 
       m[i] = x;

  }
  
  //word occurences: occ[i] is a vector containing ocurrences of word i in order 
  std::vector<std::vector<int>> occ(n,std::vector<int>(0)); 
  
  for (int i = 0; i < n; ++i){
       for (int j = 0; j < m[i]; ++j){
           int p; std::cin >> p; 
           occ[i].push_back(p);
       }
  }

  std::vector<int> index(n,0);//index of currently used occurence of each word in occ
  std::vector<int> cpos(n,0); //position of each word in the document for the current snippet
  
  for (int i = 0; i < n; ++i){
       cpos[i] = occ[i][0]; //first look at the first occurence of every word
  }

  int reslength = -1; //shortest snippet length encountered so far
  
  int cfirst = 0;//position of first snippet word in text
  int clast = 0;//position of last snippet word in text
  int firstWordID = 0; //ID/index of first word in snippet
  
  bool change = 1;
  
  while (change){
    change = 0;
    cfirst = *std::min_element(cpos.begin(),cpos.end()); //smallest position among word positions in snippet
    clast = *std::max_element(cpos.begin(),cpos.end());  //biggest position among word positions in snippet
    //is this the best encountered so far?
    if (reslength == -1 || clast - cfirst + 1 < reslength){
      reslength = clast - cfirst + 1;
    }
    firstWordID = std::min_element(cpos.begin(),cpos.end()) - cpos.begin(); //index of smallest position among word positions in snippet in cpos
    //the above line gives the ID of the used word so we can look up the next occurence in occ
    if (occ[firstWordID].begin() + index[firstWordID] + 1 != occ[firstWordID].end()){ //are there any more occurences of that word
      change = 1;//if there had not been another occurence we would have terminated 
      index[firstWordID] = index[firstWordID] + 1; //advance index used in occ for this word
      cpos[firstWordID] = occ[firstWordID][index[firstWordID]];//update currently considered word postion
    }
  }


  std::cout << reslength;
  

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