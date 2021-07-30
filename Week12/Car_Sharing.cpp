#include <iostream>
#include <map>
#include <vector>
#include <set>
// BGL includes
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
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
typedef boost::graph_traits<graph>::edge_iterator    edge_it;    // to iterate over all edges


// Custom edge adder class
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
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;  
    w_map[rev_e] = -cost; 
  }
};

/*This is a mincost maxflow problem. We take as vertices the stations and times. 
For efficiency we create station-time-vertex only if that combination is needed in 
a booking request and adjust the cost to make up for the absence of the intermediate 
time steps. So on a booking request there is an an edge from start to target station
with the respective times and for a car to remain at a station there is an edge to 
the vertex at the next-higher time at the same station*/

void testcase() {
    int N, S; //#bookings, #stations
    std::cin >> N >> S;
    
    std::vector<std::set<int>> s_t_pairs(S, std::set<int>());
    
    std::vector<int> init(S);//cars initially parked at station i
    
    for (int i = 0; i < S; ++i){
      std::cin >> init[i];
    }
    
    graph G;
    edge_adder adder(G);  
    
    int maxa = 0;
    int mind = 100000;
    
    std::map<std::pair<int,int>,int> to_vertex;
    int v_ID = 1;
    for (int i = 0; i < N; ++i){
      int s, t, d, a, p; //start station, target station, departure time, arrival time, profit
      std::cin >> s >> t >> d >> a >> p;
      --s; --t; //normalize to 0-base
      //save that there is a point for station s (t) at time d (a) by station
      s_t_pairs[s].insert(d);
      s_t_pairs[t].insert(a);
      //create vertices (s,d) and (t,a) if necessary, by assigning an ID (save by station-time-pair)
      std::pair<int,int> start = std::make_pair(s,d);
      std::pair<int,int> dest = std::make_pair(t,a);
      if (to_vertex[start] == 0) { 
        to_vertex[start] = v_ID;
        ++v_ID;
      }
      if (to_vertex[dest] == 0) {
        to_vertex[dest] = v_ID;
        ++v_ID;
      }
      
      //cost needs to be some treshold minus the profit for successive shortest paths nonnegative weights
      //multiply treshold by time taken, as we do not create a vertex for every point in time
      adder.add_edge(to_vertex[start], to_vertex[dest], 1, (a-d)*100-p);
      
      //save first and last time needed
      maxa = std::max(maxa,a); 
      mind = std::min(mind,d);
      
    }
    
    const int src = 0;
    const int sink = v_ID;
    
    //connect same stations in time
    for (int i = 0; i < S; ++i){
      std::set<int>::iterator it = s_t_pairs[i].begin(); 
      if (it == s_t_pairs[i].end()) continue;//do not connect source to target
      int first = to_vertex[{i,*it}];
      int dtime = *it;
      adder.add_edge(src, first, init[i], (dtime-mind)*100);
      while (++it != s_t_pairs[i].end()){
        int second = to_vertex[{i,*it}];
        int atime = *it;
        adder.add_edge(first, second, 1000, (atime-dtime)*100);
        first = second;
        dtime = atime;
      }
      adder.add_edge(first, sink, 1000, (maxa - dtime)*100);
    }

    
    
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
   


   // Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost2 = boost::find_flow_cost(G);
    // Iterate over all edges leaving the source to sum up the flow values.
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
    //add time * treshold for each flow and subtract cost to end up with only the profit
    std::cout << 100*(maxa-mind)*s_flow - cost2 << "\n"; 

}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    testcase();
  }
  return 0;
}
