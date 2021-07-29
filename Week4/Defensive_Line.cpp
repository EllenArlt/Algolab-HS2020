#include <iostream>
#include <vector>
#include <stack>

/*This problem is solved using Dynamic programming and a sliding window:

Let f[i,j] be the maximum number of attacked defenders for defensive line
v_i,...,v_n-1 using j attackers. Then clearly 
f[i,0] = 0 for all i, since we have no more attackers and
for convinience I let f[i,j] be some sufficiebtly low negative value, if (n-1-i) < j, ie
there are more attackers than defenders left. That way, if there is no way to use up all
attackers anymore, we cancel out all gain achieved through previous attackers
in that particular strategy and if all strategies fail, the maximum number of 
attacked defenders in the whole definsive line for m attackers will f[0,m] be negative. 
f[i,j] is the maximum of not starting an attacked segment at v_i and thus just incrementing i,
or, if possible, starting an attacked segment at v_i and so moving i to just after 
the last attacked defender and checking for one less defender in the rest.

f[i,j] = max(f[i+1,j],(e-i+1)+f[e+1,j-1]) if there exists e st. [i,e] is a valid interval
         f[i+1,j] otherwise
         
We precompute the endpoints of valid intervals starting from i = 0,..,n-1 using
a sliding window*/


//DP:top-down with memorizing
//g is the precomputed vector st. g[i] = -1 if there is no valid single attack interval starting at v_i and its endpoint otherwise
int maxAttack(const std::vector<int> &g, const int &nmax, int start, int attackersLeft, std::vector<std::vector<int>> &mem){
  if (mem[start][attackersLeft] != -1) return mem[start][attackersLeft]; //recall from memory,if possible
  if (attackersLeft == 0){//we can not attack anymore (f[i,0])
    mem[start][attackersLeft] = 0;
    return 0;
  }
 
  if (start + attackersLeft >= nmax){ //(f[i,j] st. (n-1-i) < j) there cannot be any valid attack anymore
    mem[start][attackersLeft] = -1000000;//remember
    return -1000000;//sufficiently negative value to cancel out previous gain from attacks
  }

  int res = maxAttack(g,nmax,start+1,attackersLeft,mem);//f[i+1,j]
  
  //f[i,j] = max(f[i+1,j],(e-i+1)+f[e+1,j-1]) if there exists e st. [i,e] is a valid interval
  if (g[start] != -1){
    res = std::max(res,g[start] - start + 1 + maxAttack(g,nmax,g[start]+1,attackersLeft-1,mem));
  }
  
  mem[start][attackersLeft] = res;//remember
  return res;
  
}



void testcase(){
  
  int n; std::cin >> n;//number defenders
  int m; std::cin >> m;//number attackers
  int k; std::cin >> k;//attack strength
  
  std::vector<int> defense(n); //defense[i] = v_i
  
  for (int i = 0; i < n; ++i){
    int v; std::cin >> v;
    defense[i] = v;
  }
  
  int sum = 0;
  
  //will have groups[i] = -1 if there is no valid single attack interval starting at v_i and its endpoint otherwise
  std::vector<int> groups(n,-1);
  
  
  //Use a sliding window [i,j] to find all valid single attack intervals
  int i = 0;
  int j = 0;
  int cursum = defense[0];//accumulated defense value in current subinterval
  
  while( i < n ){
    while ( cursum < k && j < n-1){ //as long as we can increase j without getting out of bounds and we have not yet reached the attack strength
      ++j;
      cursum += defense[j];
    }
    if ( cursum == k){//if our interval needs exactly the right attack strength, save it
      groups[i] = j;
    }
    //slide the leftmost end of the window to the right
    cursum -= defense[i];
    ++i;
    if ( j < i ) {
      ++j;
      cursum += defense[j];
    }
  }
  
  
  std::vector<std::vector<int>> memo(n+1,std::vector<int>(m+1,-1));//memory for DP
  
  //solution is f[0,m]
  sum = maxAttack(groups,n,0,m,memo);
  
  if (sum <= 0) std::cout << "fail"; //non-positive value implies -100000 was added to all strategies, ie all end in failure
  else std::cout << sum;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int nt; std::cin >> nt;
  for(int it = 0; it < nt; ++it){
    testcase();
    std::cout << "\n";
  }
  
  return 0;
}