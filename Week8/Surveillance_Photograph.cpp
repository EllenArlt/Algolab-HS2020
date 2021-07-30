#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int n; std::cin >> n; //number of intersections
  int m; std::cin >> m; //number of streets
  int k; std::cin >> k; //number of police stations
  int l; std::cin >> l; //number of photos
  
  /*Create a graph of 2n nodes, where the first n nodes represent the network 
  for the way to the photo locations (ie policemen can go through streets as often
  as they like) and the other n nodes represent the network when policemen already obtained
  a photo and are on their way back (ie each street has capacity 1)*/
  graph G(2*n);
  edge_adder adder(G);

  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  
  for (int x = 0; x < k; ++x){
    int pol; std::cin >> pol;
    adder.add_edge(v_source, pol, 1);//where policemenn start (first n nodes)
    adder.add_edge(n+pol, v_sink, 1);//where policemen finish (last n nodes)
  }
  
  for (int x = 0; x < l; ++x){
    int photo; std::cin >> photo;
    //obtaining photo represents transition from first to last n nodes
    adder.add_edge(photo, n+photo, 1); 
  }
  
  for (int i = 0; i < m; ++i){
    int x; std::cin >> x;
    int y; std::cin >> y;
    adder.add_edge(x, y, k); //can enter street as often as they like
    adder.add_edge(n+x, n+y, 1); // can enter street only once
  }


  // Calculate flow from source to sink
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << flow << "\n";

}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    testcase();
  }

  return 0;
}