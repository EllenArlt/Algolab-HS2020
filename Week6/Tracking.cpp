// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

/*As k is <= 10, the trick is to create a directed graph which contains k+1 copies
  of the graph given by the input. Each of the k+1 copies/sugraphs should have the
  property that, if we reached a verex in the j-th copy (starting from 0), then 
  we have already crossed j rivers. Therefore normal roads are represented as two 
  directed edges (two-way) within a copy, while roads along rivers are represented
  by two directed edges between different copies (as we add to the river counter when
  using them). Note that after we met the minimum requirement of edges along rivers,
  we can still use them, so in the k-th copy we need to treat them like the
  normal roads.*/

void testcase(){
  int n; //# of cities/vertices
  int m; //# of roads/edges
  int k; //# of rivers we want to go along
  int x; //starting city/vertex
  int y; //finishing city/vertex
  std::cin >> n >> m >> k >> x >> y;
  
  weighted_graph G((k+1)*n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  //reading roads/edges;
  edge_desc e;
  for (int i = 0; i < m; ++i){
    int a, b;
    std::cin >> a >> b; //road connecting cities a and b
    int c; std::cin >> c; //travel time along road in minutes
    bool d; std::cin >> d; //Does this road go along a river?
    
    if (d){//this road goes along a river
      for (int j = 0; j < k; ++j){
        //pass a river, so using this road either way leads from subgraph "crossed j rivers" to subgraph "crossed j+1 rivers"
        e = boost::add_edge(j*n+a, (j+1)*n+b, G).first; weights[e]=c;
        e = boost::add_edge(j*n+b, (j+1)*n+a, G).first; weights[e]=c;
      }
      //in the subgraph "crossed k rivers" we should be able to use this road without having to add to the total
      e = boost::add_edge(k*n+a, k*n+b, G).first; weights[e]=c;
      e = boost::add_edge(k*n+b, k*n+a, G).first; weights[e]=c;
    }
    else {//this road does NOT go along a river
      //can use this road either way in each subgraph without incrementing the number of rivers we pass (so only connects within each subgraph)
      for (int j = 0; j <= k; ++j){
        e = boost::add_edge(j*n+a, j*n+b, G).first; weights[e]=c;
        e = boost::add_edge(j*n+b, j*n+a, G).first; weights[e]=c;
      }
    }
  }
  
   //use dijkstra to find the shortest path from x to y (y after crossing k rivers is k*n+y)
  int res = dijkstra_dist(G,x,k*n+y);
  std::cout << res;
  
}

int main(){
  
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  
  for(int i = 0; i < t; ++i){
    testcase();
    std::cout << std::endl;
  }
}
