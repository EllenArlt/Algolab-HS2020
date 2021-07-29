#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

/*Sliding window*/

void testcase() {
  

  
  int n; std::cin >> n; //number of parasols
  

  std::vector<int> CDS(n);//coordinates of parasols
  
  
  //transitions
  for (int i = 0; i < n; ++i){
       int x; std::cin >> x; 
       CDS[i] = x;

  }
  
  std::sort(CDS.begin(), CDS.end()); //sort coordinates in increasing order

  
  /*in each iteration we make sure the parasols at coordinates from CDS[i] 
    to CDS[j] are exactly the ones within distance 100 of the currently considered
    bar position*/
  int i = 0; 
  int j = 0;
  //maximum number of parasols within range of any yet encountered bar position
  int maxpar = -1; 
  //minimum distance of furthest parasol from any yet encountered bar 
  //position with current maximum number of parasols
  int mindis = 101;
  //distance of furthest parasol from currently considered bar position
  int tempdis = 101;
  //number of parasols within range of currebtly considered bar position
  int temppar = 1;
  //currently considered bar position
  int loc = CDS[0];
  
  
  std::vector<int> optlocs; //optimal bar locations

  //sliding window
  while (i < n){
    //advance j for [CDS[i],CDS[j]] to contain all parasols within range
    while(j < n-1 && std::abs(CDS[j+1]-loc) <= 100){
      ++j;
      temppar += 1; //keep track of number of parasols
      
    }
    /*find distance of furthest parasol from currently considered bar position,
      can only be at either end of the window*/
    tempdis = std::max(std::abs(CDS[j]-loc),std::abs(CDS[i]-loc)); 
    //is this location the best yet in terms of number of parasols?
    if (temppar > maxpar){
      //erase all contents of optlocs and fill in new location
      optlocs.resize(0);
      optlocs.push_back(loc);
      //set new standard for best solution
      maxpar = temppar;
      mindis = tempdis;
      
    }
    //if this location is as good as the best yet in terms of number of parasols?
    else if (temppar == maxpar){
      //is this better in terms of minimum distance?
      if (tempdis < mindis){
        //erase all contents of optlocs and fill in new location
        optlocs.resize(0);
        optlocs.push_back(loc);
        //set new standard for best solution
        mindis = tempdis;
      }
      //Is this location just as good as the best encountered yet?
      else if (mindis == tempdis){
        //found another optimal location
        optlocs.push_back(loc);
      }
    }
    
    
    loc += 1; //advance potential bar location along line
    
    /*the first parasol in the old range might not be in the new range
      (only that one since positions are distinct and we only advance by 1*/
    if (std::abs(CDS[i]-loc) > 100){
      if (i < j){
        temppar -= 1;
        ++i;
      }
      /*if as a result there are no parasols in the range, we can immediately
        skip to the next parasol location for the next bar location as this location
        must give at least as many parasols as any location in between
        
        As the parasols to the right are found in the next iteration, it is sufficient
        to let i=j such that they contain the next parasol only, for now
        */
      else{
        ++j;
        loc = CDS[j];
        temppar = 1;
        ++i;
      }
      
    }
    
  }
  
  
  
  
  std::cout << maxpar << " " << mindis << "\n";
  for (std::vector<int>::iterator it = optlocs.begin(); it != optlocs.end(); ++it){
    std::cout << *it << " ";
  }
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