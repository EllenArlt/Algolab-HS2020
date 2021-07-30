// STL includes
#include <iostream>
#include <vector>
#include <stack>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

typedef boost::graph_traits<weighted_graph>::out_edge_iterator           out_edge_it; // Iterator
typedef boost::graph_traits<weighted_graph>::edge_iterator                         edge_it;

void testcase()
{
  int n, Tatooine; 
  std::cin >> n >> Tatooine; --Tatooine;
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  std::vector<std::vector<int>> costs(n,std::vector<int>(n));
  
  edge_desc e;
  for (int j = 1; j < n; ++j){
    for (int k = 1; k <= n-j; ++k){
      int cost; std::cin >> cost;
      e = boost::add_edge(j-1, j+k-1, G).first; weights[e]=cost;
      costs[j-1][j+k-1] = cost;
      costs[j+k-1][j-1] = cost;
    }
  }
 
  std::vector<vertex_desc> pred(n);
  boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(pred.begin(), boost::get(boost::vertex_index, G)),
      Tatooine);
  
  //construct Graph Leia (prims minimum spanning tree)
  weighted_graph Leia(n);
  weight_map weights_L = boost::get(boost::edge_weight, Leia);
  for (int i = 0; i < n; ++i){
    if (pred[i] != i){
      e = boost::add_edge(pred[i], i, Leia).first; 
      weights_L[e]=costs[pred[i]][i];
    }
  }
  
  /*To find a minimum spanning tree different from Leia's it is sufficient to
    exchange one edge, the one which causes minimal increase in cost. Thus,
    for each edge we could possibly add, we look at the so created cycle
    (as trees are just barely cycle-free) and find the edge of maximum cost in that cycle
    which we would remove in exchange. This is done for all addable edges
    
    To do this efficiently, note that in a path from u to v (which will be a 
    cycle if we add an edge from u to v) the edge of maximum cost is either the
    edge of maximum cost in the path from u to the predecessor w of v or the 
    edge from w to v.
    */
  
  std::vector<std::vector<int>> max_cost_in_cycle(n,std::vector<int>(n,-1));
 
  //traverse tree similar to a DFS to do the above (for all path starting nodes) to check all paths
  for (int i = 0; i < n; ++i){
    std::stack<vertex_desc> s;
    s.push(i);
    max_cost_in_cycle[i][i] = 0;
    std::vector<bool> vis(n,false);
    while (!s.empty()){
      vertex_desc cur = s.top();s.pop();
      vis[cur] = true;
      out_edge_it e, eend;
      for(boost::tie(e, eend) = boost::out_edges(boost::vertex(cur,Leia), Leia); e != eend; ++e) {
          int v = boost::target(*e, Leia);
          if (!vis[v]){ //so we do not check edges twice
            max_cost_in_cycle[i][v] = std::max(max_cost_in_cycle[i][cur],costs[cur][v]); //path from u to w or w-v-edge
            s.push(v);
          }
  
      }
    }
  }
  long Leia_network_cost = 0;
  int min_inc = pow(2,20);
  edge_it ebeg, eend;
  //add all costs in Leia's graph
  for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
    int u = boost::source(*ebeg, G); 
    int v = boost::target(*ebeg, G);
    if (boost::edge(u, v, Leia).second) Leia_network_cost += costs[u][v];
    
    else min_inc = std::min(min_inc, weights[*ebeg] - max_cost_in_cycle[u][v]); //find minimum cost increase through edge exchange
  }
  std::cout << Leia_network_cost + min_inc << "\n";
  
  
}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t;++i){
     testcase(); // Solve a particular test case
  }
  return 0;

}