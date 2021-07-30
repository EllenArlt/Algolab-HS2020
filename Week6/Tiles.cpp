#include <iostream>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>
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



bool testcase() {
  int w, h; std::cin >> w >> h; //width and height of the garden space
  
  graph G(w*h); //create grid-like graph
  edge_adder adder(G);
  
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  int num_to_tile = 0; //will contain # of spaces to be tiled
  
  
  for (int i = 0; i < h; ++i){
    bool ieven = 1 - (i%2); //true when i is even
    bool jeven = 0;
    for (int j = 0; j < w; ++j){
      int v = i*w+j; //vertex in grid this translates to
      jeven = 1 - jeven;
      char c; std::cin >> c; // c == '.', if to be tiled
      
      if (c == '.'){
        ++num_to_tile;
        if ((ieven && jeven) || (!ieven && !jeven)){
          adder.add_edge(v_source, v, 1); //connect to source
          if (i > 0) adder.add_edge(v, (i-1)*w+j, 1);
          if (i < h-1) adder.add_edge(v, (i+1)*w+j, 1);
          if (j > 0) adder.add_edge(v, i*w+(j-1), 1);
          if (j < w-1) adder.add_edge(v, i*w+(j+1), 1);
        }
        else adder.add_edge(v, v_sink, 1); //connect to target
      }
      
    }
  }
  
  //we cannot cover an odd number of spaces with 2:1-tiles
  if (num_to_tile%2 != 0) return false;
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if (2*flow == num_to_tile) return true;
  return false;
  
  
}



int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
    // Solve a particular test case (return true iff tilable)
     bool dec = testcase(); 
     if (dec) std::cout << "yes";
     else std::cout << "no";
     std::cout << "\n";
  }
  return 0;

}