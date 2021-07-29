// Includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,boost::property<boost::edge_name_t,int>// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
    >          graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)). 
typedef boost::graph_traits<graph>::edge_descriptor    edge_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_iterator    edge_it;    // to iterate over all edges
typedef boost::property_map<graph,boost::edge_name_t>::type   ComponentMap;



void testcase() {
  int n, m;
  std::cin >> n >> m;
  graph G(n);
  
  for (int i = 0; i < m; ++i){
    int a, b; std::cin >> a >> b;
    boost::add_edge(a, b, G);
  }
  
  ComponentMap cmap = boost::get(boost::edge_name,G);
  int c = boost::biconnected_components(G,cmap);
  
  std::vector<edge_desc> comps(c);
  std::vector<int> comps_ctr(c,0);
  
  auto edge_iters = boost::edges(G);
  for (auto edge_it = edge_iters.first; edge_it != edge_iters.second; ++edge_it) {
    int comp = cmap[*edge_it];
    comps[comp] = *edge_it;
    ++comps_ctr[comp];

  }
  
  std::vector<std::pair<int,int>> res;
  int k = 0;
  
  for (int i = 0; i < c; ++i){
    if (comps_ctr[i] == 1) {
      int x = boost::source(comps[i], G);
      int y = boost::target(comps[i], G);
      if (x < y) res.push_back({x,y});
      else res.push_back({y,x});
      ++k;
    }
  }
  
  std::cout << k << "\n";
  
  std::sort(res.begin(),res.end());
  
  for (auto it : res){
    std::cout << it.first << " " << it.second << "\n";
  }

}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
  }
  return 0;
}