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

// Custom edge adder class
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

//Here I am using the Circulation problem with minimum edge constrainrs - technique
//from the tutorial

void testcase(){
  int l; //# of locations
  int p; //# of paths
  std::cin >> l >> p;
  
  std::vector<int> demands(l); //demands[i] = demand - supply
  //std::vector<int> demands(l);
  
  for (int i = 0; i < l; ++i){
    int g, d; //# of soldiers stationed and needed at location i
    std::cin >> g >> d;
    demands[i] = d - g;
  }
  
  graph G(l);
  edge_adder adder(G);  
  
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for (int i = 0; i < p; ++i){
    int a, b, minc, maxc; //path goes from a to b with minimum and maximum capacity
    std::cin >> a >> b >> minc >> maxc; 
    //adjust demands and capacities according to minimum capacity
    demands[a] += minc;
    demands[b] -= minc;
    adder.add_edge(a,b,maxc-minc);
  }
  
  long defense_needed = 0;//sum of all demands
  
  //connect to source and sink
  for (int i = 0; i < l; ++i){
    if (demands[i] > 0){
      defense_needed += demands[i];
      adder.add_edge(i,v_sink,demands[i]);
    }
    else if (demands[i] < 0){
      adder.add_edge(v_source,i,-demands[i]);
    }
  }
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  if (flow == defense_needed) std::cout << "yes";
  else std::cout << "no";
  
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