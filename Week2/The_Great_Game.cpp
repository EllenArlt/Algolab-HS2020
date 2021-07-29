#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

/*This problem is solved using DP:
Since both players will always try to move their own meeple so it reaches the goal as fast as possible
and the other meeple so it reaches the goal as late as possible (assuming the other player plays optimally)
independently of the position of the meeple not moved this turn, we can do DP on the number of turns it takes 
for a meeple to reach the goal. Each subproblem depends on the number of turns played yet and the position of
the meeple. On even turns both players use their own meeple, so they try to minimize the number of turns
in the game resulting from playing that move. Otherwise they move the other meeple so thy want to maximize the number 
of turns.
Let f[i,turn parity] be the number of turns for a meeple to reach the goal. Then by the above
f[i,even] = 1 + min_{j reachable from i via a direct transition} f[j,odd]
f[i,odd] = 1 + max_{j reachable from i via a direct transition} f[j,even]

Clearly f[n,even] = f[n,odd] = 0
*/



//DP returning #turns until a meeple wins from position meeple, on turn numturns
int timing(int meeple, const int nmax, int numturns, const std::vector<std::vector<int>> &t, std::vector<std::vector<int>> &m) {
    if (m[meeple][numturns] != -1){//recall from memory, if possible
    return m[meeple][numturns];
  }
  if (meeple == nmax){ //If we are in the goal, no more turns needed
    return 0;
  }
  int timet = 0;
  
    if (numturns == 0){// moves own meeple,  so we want to maximize the # of turns needed
      int min = 5000000;
      for (std::vector<int>::const_iterator itr = t[meeple].begin(); itr != t[meeple].end(); ++itr){
        min = std::min(min, timing(*itr,nmax,(numturns+1)%2,t,m));
      }
      timet = 1 + min;
    
    }
    else{//moves other meeple, so we want to maximize the # of turns needed
      int max = 0;
      for (std::vector<int>::const_iterator itr = t[meeple].begin(); itr != t[meeple].end(); ++itr){
        max = std::max(max, timing(*itr,nmax,(numturns+1)%2,t,m));
      }
      timet = 1 + max;
    
    }
  
  
  m[meeple][numturns] = timet;
  return timet;
  
}





void testcase() {
  
  typedef std::vector<int> V;
  typedef std::vector<std::vector<int>> V2;

  
  int n; std::cin >> n; //number of positions
  int m; std::cin >> m; //number of transitions
  
  int r; std::cin >> r; //starting position of red meeple
  int b; std::cin >> b; //starting position of black meeple

  std::vector<V> transitions(n+1);  
  
  //transitions
  for (int i = 0; i < m; ++i){
       int u; std::cin >> u; 
       int v; std::cin >> v; 
       transitions[u].push_back(v);

  }
  
  
  V2 times(n+1, V(2,-1));
  

  //find #turns it takes to reach the goal for each meeple from its starting position
  int tr = timing(r, n, 0, transitions, times);
  int tb = timing(b, n, 0, transitions, times);
  
  //if the black meeple takes strictly less turns, black wins
  if (tr > tb) std::cout << 1;
  else if (tr == tb && tr%2 == 0) std::cout << 1; //otherwise black wins if they reach the goal on the same turn and black goes first, ie. they finish on an even turn
  else std::cout << 0;
  
  
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