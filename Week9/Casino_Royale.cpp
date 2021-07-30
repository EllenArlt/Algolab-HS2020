
// Includes
// ========
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
                boost::property <boost::edge_weight_t, long> > > > > graph; 

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

/*Instead of seeing this problem from the perspective of the agents, we need to see it from the perspective of the train.
  One can say this train has l empty seats usable by the agents, and at any station for every we can eitherfill it or leave it empty.
  Leaving it empty results in a flow to the next stop. Filling it with an assigned agent makes it flow directly to that agent's 
  destination and there join the "empty seat" path again. In summary, we have a straight path from stop to stop representing the
  seat is assignable from there and any deviation/other connection between vertices represents taking a mission.
  
  To ensure positive payments, we pay maxp, the maximum priority, to each empty journey between two stops and maxp times the jouneys 
  between two adjacent stops it takes for the mission minus its priority. That is, all in all, we pay maxp for each flow, for each 
  unordered pair of adjacent stops minus the sum of all priorities of missions carried out*/


void testcase() {
  
    int n; //number of train stops
    int m; //number of missions
    int l; //maximum number of agents that can be on the train between any two consecutive stops
    std::cin >> n >> m >> l;
    
    long maxp = pow(2L,8L); //maximum priority (used to ensure positive costs when minimizing the negative sum of priorities)
 
    // Create graph, edge adder class and propery maps
    graph G(n+1);
    edge_adder adder(G);  
    
    
    const int v_source = n;
    
    //maximum capacity from the source is the number of usable seats
    adder.add_edge(v_source, 0, l, 0);
    
    for (int i = 0; i < m; ++i){
      int x, y; //start and destination
      int q; //mission priority
      std::cin >> x >> y >> q;
      //pay maxp for each journey between stops along the mission and deduct the priority
      adder.add_edge(x, y, 1, (y-x)*maxp - q);
    }
    
    for (int i = 0; i < n-1; ++i){
      //pay maxp when the train travels to the next stop with a free seat (can have at most l free seats)
      adder.add_edge(i, i+1, l, maxp);
    }
   
    // Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, v_source, n-1);
    long cost2 = boost::find_flow_cost(G);
    //you pay maxp for each of l seats for each of n-1 connections between stops and deducted q for each mission carried out => positive sum of priorities is:
    std::cout << l*(n-1)*maxp - cost2 << "\n"; 


}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
  }
  return 0;

}