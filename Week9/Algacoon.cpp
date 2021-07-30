// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;
// Interior Property Maps
typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;


class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

//This computes and outputs the minimal mincut among all partitions
void testcase() {
  
  int n; //#figures
  int m; //#limbs
  std::cin >> n >> m;
  
  graph G(n);
  edge_adder adder(G);

  for (int i = 0; i < m; ++i){
    int a, b, c;
    std::cin >> a >> b >> c;
    adder.add_edge(a, b, c);
    
  }
  
  //set some cut as first indicator to the minimal mincut
  long minres = boost::push_relabel_max_flow(G, 0, n-1);

  //As you both get at least one figure there must be figures i and i+1 which belong 
  //to one of you each (the first is either yours or your partner's)
  for (int i = 1; i < n; ++ i){
    minres = std::min(boost::push_relabel_max_flow(G, i-1, i),minres);
    minres = std::min(boost::push_relabel_max_flow(G, i, i-1),minres);
  }


  
  std::cout << minres << "\n";


}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
  }
  return 0;

}
