#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>



typedef boost::adjacency_list<boost::vecS, 
                              boost::vecS, 
                              boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>
                              > weighted_graph;
                              

typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map; 
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


void kruskalplusdijikstra(const weighted_graph &G,int n) {
  std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)
  //compute the MST and store the edges in mst
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  int wsum = 0;
  int dist = 0;
  

  //go through all edges of the MST and add their weights to wsum
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
     

     int weight = get(boost::edge_weight_t(), G, *it);
     wsum += weight;

    
  }
  
  //Use Dijkstra and store all distances from vertex 0 in dist_map
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, 0,
                                boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
                                boost::get(boost::vertex_index, G))));
  
  //let dist be the maximum distance from vertex 0                               
  dist = *max_element(dist_map.begin(),dist_map.end());

  //output the results
  std::cout << wsum << " " << dist;
  
}

void testcase() {
  

  
  int n; std::cin >> n; //number of vertices
  

  int m; std::cin >> m; //number of edges
  
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  //read edges + weights
  for (int i = 0; i < m; ++i){
       int x; std::cin >> x; 
       int y; std::cin >> y; 
       int w; std::cin >> w;
       edge_desc e;
       e = boost::add_edge(x, y, G).first; weights[e]=w;

  }
  
  kruskalplusdijikstra(G,n);
  

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