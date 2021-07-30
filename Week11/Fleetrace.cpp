#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

//We solve this as a flow problem (bipartite matching) where we add an extra edge from source to sink of maximum cost
//and sufficient capacity (all other edges between boat owners and sailors is the maximum cost minus the 
// spectacle coefficient with capacity 1. The idea is that the extra edge is chosen to achieve the minimum cost when it
//is beneficial to the coefficient sum (as now the maxflow is always the same)

void testcase() {
  int b; //# boats
  int s; //# sailors
  int p; //# pairs
  std::cin >> b >> s >> p;

    const int N=b+s+3;
    const int v_source = b+s;
    const int v_source2 = b+s+1;
    const int v_target = b+s+2;

    // Create graph, edge adder class and propery maps
    graph G(N);
    edge_adder adder(G);  
    
    long maxcoeff = 50;
    
    for (int i = 0; i < p; ++i){
      int bi, si, ci; //boat id, sailor id, coefficient
      std::cin >> bi >> si >> ci;
      adder.add_edge(bi, b+si, 1, maxcoeff-ci);
    }
    
    for (int i = 0; i < b; ++i){
      adder.add_edge(v_source2, i, 1, 0);
    }
    
    for (int i = 0; i < s; ++i){
      adder.add_edge(b+i, v_target, 1, 0);
    }
    
    adder.add_edge(v_source, v_source2, b, 0);
    adder.add_edge(v_source2, v_target, b, maxcoeff);
    
    

    //sive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    std::cout << b*maxcoeff - cost2; // 12
   

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