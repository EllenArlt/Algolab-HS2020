// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
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

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_descw;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_descw;


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
    c_map[rev_e] = 0; 
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};




long dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<long> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}



//check if runners runners can run the marathon of distance d at the same time
//n,m : #crossings, #streets
//s,f : start, finish
//streets: from, to, capacities and lengths of streets
long feasible(std::vector<std::vector<int>> &streets,int n, int m, int s, int f,
              int d, long runners) {
    // Create graph of street network
    graph G(n+1);
    edge_adder adder(G);  
    
    const int src = n;
    
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    // Add the edges
    adder.add_edge(src, s, runners, 0); //only put runners runners at the start
    
    for (int i = 0; i < m; ++i){
      if (streets[i][0] != streets[i][1]){ //Self-loops are unnecessary
        adder.add_edge(streets[i][0], streets[i][1], streets[i][2], streets[i][3]);
        adder.add_edge(streets[i][1], streets[i][0], streets[i][2], streets[i][3]);
      }
    }
   
    // Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, src, f);
    long cost = boost::find_flow_cost(G);
    long s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
    

    //The number of runners is valid if all of them can flow from start to finish
    //and all run the minimal distance d, so costs add up to flow*d
    if (s_flow == runners && s_flow*d == cost) return -1;
    else return s_flow; //if not this is an upper bound on the number of runners that could possibly make it
}




void testcase()
{
  int n, m, s, f; //#crossings, #streets, start, finish
  std::cin >> n >> m >> s >> f;
  
  std::vector<std::vector<int>> streets(m,std::vector<int>(4));
  
  //graph used for dijkstra to get shortest distance from s to f
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  edge_descw e;
  
  long maxrunners = 0; //upper bound on #runners (sum of capacities of outgoing edges from s)
  
  //read street info (and determine maxrunners)
  for (int i = 0; i < m; ++i){
    std::cin >> streets[i][0] >> streets[i][1] >> streets[i][2] >> streets[i][3];
    e = boost::add_edge(streets[i][0],streets[i][1], G).first; weights[e]=streets[i][3];
    if (streets[i][0] == s || streets[i][1] == s) maxrunners += streets[i][2];
  }
  
  long d = dijkstra_dist(G,s,f); //find minimum distance to finish
  
  //binary search for optimal number of runners
  long high = maxrunners + 1;
  long low = 0;
  while (high > low){
    long mid = (high + low)/2;
    long res = feasible(streets,n,m,s,f,d,mid);
    if (res == -1) low = mid + 1;
    else high = std::min(mid,res+1);
  }
  
  std::cout << high-1 << "\n";

}
  
  

int main()
{
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    testcase();
  }

  return 0;
}