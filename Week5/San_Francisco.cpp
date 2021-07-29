#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

/*Here we use a greedy approach. For each vertex in the game board we save the maximum number of points when finishing the 
  i-th turn there, where we progress i one by one. We basically simulate all possible i-th moves, and if that gives us a better score than finishing
  any earlier move there, we save the best score and consider that vertex as a starting point for the i+1-th move (no need to consider it as a starting
  point otherwise, as in that case we must have already considered starting there in an earlier turn, so we would just be repeating some chain of moves)*/


int testcase(){
  
  int n; std::cin >> n; //#holes
  int m; std::cin >> m; //#canals
  long x; std::cin >> x; //claimed score
  int k; std::cin >> k; //#turns
  
  
  std::vector<std::vector<std::pair<int,int>>> adj(n); //adj[i] contains all vertices that can be reached vertex i from as pairs containing the vertex and the number of points reached coming from that vertex
  
  for (int i=0; i < m; ++i){
    int u,v,p;
    std::cin >> u >> v >> p;
    adj[u].push_back(std::make_pair(v,p));
  }
  
  int numturns = 0; //turns taken;
  std::vector<long> best_points(n); //best_points[i] is the best score that can be obtained when finishing the numturns-th turn at vertex i
  std::vector<long> best_points_old(n); //best_points for the previous turn
  std::queue<int> q;
  q.push(0); //start from start
  q.push(-1); //indicates all starting points for this turn have been considered
  bool score_reached = false; //true if we managed to score at least x in less than k turns
  
  while (numturns < k && !score_reached){
    ++numturns;
    best_points_old = best_points;
    while(q.front() != -1){
      int cur = q.front(); q.pop();
      for (auto it = adj[cur].begin(); it != adj[cur].end(); ++it){
        int v = (*it).first;
        int p = (*it).second;
        if (best_points_old[cur] + p > best_points[v]){//need to compare to the old best score + p!!! otherwise we might add multple channels in the same turn to some vertex
          best_points[v] = best_points_old[cur] + p;
          if (best_points[v] >= x){ score_reached = true; break;}//if we reached scoore >= x, we are done
          //in the case below we get the free move to vertex 0, so we need to compare to the best score at vertex 0, and if that is better that woill be the start of the next turn
          if (adj[v].begin() == adj[v].end()){
            if (best_points[0] < best_points[v]){
              best_points[0] = best_points[v];
              q.push(0);
            }
          }
          else q.push(v);//
        }
      }
    }
    //we considered all possible turns for this move and so we set a new turn-break
    q.pop();
    q.push(-1);
  }
  
  if (score_reached) return numturns;
  else return -1;
}


int main(){
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    int res = testcase();
    if (res == -1) std::cout << "Impossible";
    else std::cout << res;
    std::cout << "\n";
  }
}