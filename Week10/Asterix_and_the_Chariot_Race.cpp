#include <vector>
#include <iostream>
#include <cmath>

int DP(int root, bool prev_taken, std::vector<int> &cost, std::vector<std::vector<int>> &adj,std::vector<std::vector<int>> &m){
  //only one city left 
  if (adj[root].begin() == adj[root].end()){
    if (prev_taken) return 0;
    else return cost[root];
  }
  if (m[root][prev_taken] != -1) return m[root][prev_taken]; //recall from memory
  
  //either the previous province in the tree had its streets restored or not.
  //In the first case we are free to chose to restore the streets in the current city or not
  //(if we do not restore it prev_taking for the directly following city is false, otherwise true)
  //In the second case we also have that choice, but if we do not restore the current city,
  //then at least one of its following cities MUST be restored, so we need to check for every neighbor
  //the best result if it is definitely restored, while the choice for the others is left open/the most optimal one.
  int res;
  if (prev_taken){
    int take_it = cost[root];
    int do_not_take_it = 0;
    for (auto it = adj[root].begin(); it != adj[root].end();++it){
      take_it += DP(*it,true,cost,adj,m);
      do_not_take_it += DP(*it,false,cost,adj,m);
    }
    res = std::min(do_not_take_it,take_it);
  }
  else {
    int take_it = cost[root];
    int do_not_take_it = 100000*pow(2,14);
    int tmp = 0;
    for (auto it = adj[root].begin(); it != adj[root].end();++it){
      take_it += DP(*it,true,cost,adj,m);
      tmp += DP(*it,false,cost,adj,m);
    }
    for (auto it = adj[root].begin(); it != adj[root].end();++it){
      int cur = cost[*it];
      for (auto it2 = adj[*it].begin(); it2 != adj[*it].end();++it2){
        cur += DP(*it2,true,cost,adj,m);
      }
      do_not_take_it = std::min(do_not_take_it, tmp + cur - DP(*it,false,cost,adj,m));
    }
    res = std::min(do_not_take_it,take_it);
  }
  m[root][prev_taken] = res; //memorize
  return res;
}


void testcase(){
  int n; std::cin >> n;
  std::vector<std::vector<int>> adj(n);
  for (int i = 0; i < n-1; ++i){
    int a, b;
    std::cin >> a >> b;
    adj[a].push_back(b);
  }
  
  std::vector<int> cost(n);
  
  for (int i = 0; i < n; ++i){
    std::cin >> cost[i];
  }
  
  std::vector<std::vector<int>> memo(n,std::vector<int>(2,-1));
  
  std::cout << DP(0,false,cost,adj,memo);
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