#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; 

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; 

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); 
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   
    w_map[rev_e] = -cost;   
  }
};

//Check if bags bags can be carried from s to t with budget budget (c is the number of cities and g the number of guides
// and paths is a vector of edges [from,to,cost,capacity])
bool cheap_enough(long bags, long budget, int c, int g, int s, int t, std::vector<std::vector<int>> paths){
  
  //recreate graph but the source sends at most bags bags
  graph G(c + 1);
  edge_adder adder(G);  
  const int v_source = c;
  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  adder.add_edge(v_source, s, bags, 0);
  
  for (int i = 0; i < g; ++i){
    adder.add_edge(paths[i][0], paths[i][1], paths[i][3], paths[i][2]);
  }
  
   // Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, v_source, t);
    long cost = boost::find_flow_cost(G);
    // Iterate over all edges leaving the source to sum up the flow values.
    long s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
    if (s_flow == bags && cost <= budget) return true; //check if all bags sent from source reach sink and we are within budget
    else return false; //otherwise we cannot send that many bags within budget (because mincost maxflow)
  
}



void testcase() {
    int c, g, k, a; //#cities, #guides, starting city, goal city
    long b; //budget
    std::cin >> c >> g >> b >> k >> a;
    
    std::vector<std::vector<int>> guides(g,std::vector<int>(4));
    
    long esum = 0; //sum of all edge capacities == upper bound on bags transportable
    //read edges
    for (int i = 0; i < g; ++i){
      std::cin >> guides[i][0] >> guides[i][1] >> guides[i][2] >> guides[i][3];
      if (guides[i][0] == k) esum += guides[i][3];
    }
    
    //binary search fur maximal # of bags in budget
    long high = esum+1;
    long low = 0;
    while (high > low){
      long mid = (high + low)/2;
      if (cheap_enough(mid,b,c,g,k,a,guides)) low = mid + 1;
      else high = mid;
    }
    
    std::cout << low-1 << "\n";
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    testcase();
  }
  return 0;
}
