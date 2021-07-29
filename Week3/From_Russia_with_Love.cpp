#include <iostream>
#include <vector>
#include <cmath>

/*This problem is solved using DP:
Let f be an nxn matrix, where, for r > l, f[l,r] is the minimum amount passenger k can get by playing
optimally, given the coins left are the ones from position l to position r and it is passenger k's turn next. This is the same as the
maximum amount passenger k can get when all opponents try to minimize passenger k's score. Therefore
f[l,r] = max(take left coin + rest of game after opponents' turn, take right coin + rest of game after opponents' turn) 
       = max(value[l] + min_{0<=i<=m-1}(f[l+1+i,r-(m-1-i)]), value[r] + min_{0<=i<=m-1}(f[l+i,r-1-(m-1-i)]))
       
Clearly, if l >= r and r-l+1 <= m, this is the last chance to take a coin anyway, so  passenger k takes the coin of maximal value
ie. f[l,r] = max(value[l],value[r]).
If l > r, no coins are left, so we cannot score in this subgame => f[l,r] = 0.

*/

void testcase() {
  
  int n; std::cin >> n; //number of coins
  int m; std::cin >> m; //number of passengers
  int k; std::cin >> k; //observed passenger index

  
  std::vector<int> vals(n); // coin values in order
  int x;

  for (int i = 0; i < n; ++i){
       std::cin >> x;
       vals[i] = x;

  }
  
  int maxwin = 0;//will contain result
  
  //(i,j)-entry : maximal score obtainable by player k if only 
  //coins xi up to xj are left (assuming other players play to minimize his score)
  std::vector<std::vector<int>> maxScore(n,std::vector<int>(n,-1));
  
  int lastchance = (n-k) % m; //number of coins at which k takes his last coin
  if (lastchance == 0){
    lastchance = m;  //want this to be before all coins are gone
  }
  
  
  //Use a bottom-up approach to fill all necessary entries of maxScore(can move in steps of m from last move)
  
  //On player k's last turn he takes the coin of maximum value
  for (int i = 0; i+lastchance-1 < n; ++i){
       maxScore[i][i+lastchance-1] = std::max(vals[i],vals[i+lastchance-1]);

  }
  int oldinterval = 0;////number of coins in previously considered interval (l,r)
  int interval = lastchance;//number of coins in currently considered interval (l,r)
  int min1 = 0;
  int min2 = 0;
  
  //increase considered interval of leftover coins until player k's previous turn
  //compute maxScore from following turns' results
  while (interval + m < n){
    oldinterval = interval;
    interval += m;
    for (int i = 0; i+interval-1 < n; ++i){
       min1 = 500000; //hopefully big enough
       min2 = 500000;
       //assume the other players try to minimize player k's score (ie.
       // they will choose the subinterval with the least score for
       // k's following turn)
       for (int j = i; j+oldinterval-1 <= i+interval-2; ++j){
         min1 = std::min(min1,maxScore[j][j+oldinterval-1]); 
       }
       for (int j = i+1; j+oldinterval-1 <= i+interval-1; ++j){
         min2 = std::min(min2,maxScore[j][j+oldinterval-1]); 
       }
       //player k maximizes his score wrt the other player's worst case actions
       maxScore[i][i+interval-1] = std::max(vals[i]+min2,vals[i+interval-1]+min1);
       
  }
} 


  //now the final result is the maximal score of the subinterval the other
  //players give k at his first turn, which I assume is the one where 
  //k's score is as small as possible
  int min = 5000000;
  for (int i = 0; i+interval-1 < n; ++i){
         min = std::min(min,maxScore[i][i+interval-1]); 
       }
       
  maxwin = min;
    
  std::cout << maxwin;
        
  
  

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