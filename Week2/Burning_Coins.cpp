#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

/*This problem is solved using DP:
Let f be an nxn matrix, where, for r > l, f[l,r] is the minimum amount you can get by playing
optimally, given the coins left are the ones from position l to position r. This is the same as the
maximum amount you can get when your opponent tries to minimize your score. Therefore
f[l,r] = max(take left coin + rest of game after opponent's turn, take right coin + rest of game after opponent's turn) 
       = max(value[l] + min(f[l+2,r],f[l+1,r-1]), value[r] + min(f[l+1,r-1],f[l,r-2]))
       
Clearly, if l = r, there is only one coin left so we take that one.
If l > r, no coins are left, so we cannot score in this subgame.

*/


//DP
int opt( std::vector<int> &p,std::vector<std::vector<int>> &m, int start, int end){
  
  if (start == end){ return p[start];} //only one coin left
  if (start > end){ //no coins left
    return 0;
  }
  if (m[start][end] != -1){ //recall from memory, if possible
    return m[start][end];
  }

  int min1 = p[start] + std::min(opt(p,m,start+2,end), opt(p,m,start+1,end-1));
  int min2 = std::min(opt(p,m,start+1,end-1) + p[end], opt(p,m,start,end-2) + p[end]);
  int res = std::max(min1, min2);
  
  m[start][end] = res; //store result in memory for later use
  
  return res;
}





void testcase() {
  
  typedef std::vector<int> V;
  
  int n; std::cin >> n; //number of coins


  std::vector<int> game(n); //values of coins
  int val;
  
  for (int i = 0; i < n; ++i){
       std::cin >> val;
       game[i] = val;

  }
  

  std::vector<V> memo(n,V(n,-1)); //memory
  
  //get best amount for the whole game (positions 0 to n-1)
  std::cout << opt(game,memo,0,n-1);
        
  
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