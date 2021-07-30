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

/*In this problem, we treat the hallway intersections as vertices and the hallways
as edges. Henc we are given that all edges have capacity 1 and all verices have
capacity c. To model vertex capacities, we need to create two copies of the graph.
In the first copy, all vertices have only in-going edges and in the other only
out-going edges. These in- and out-vertices are connected by an edge of capacity c
(from in- to out-vertex). 
Now it is just a flow-problem: How many knights can "flow" from their starting position
to one of the exits. So we need to connect each outer hallway-intersection to the
sink (once for each exit, the corner hallway intersections have two exit hallways)
and each knight starting position to the source*/

void testcase() {
  int m, n; std::cin >> m >> n; //# cloumns and rows
  int k; std::cin >> k; //# of knights
  int c; std::cin >> c; //vertex capacity
  
  graph G(2*m*n); //create two copies of the grid-like graph
  edge_adder adder(G);
  
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  //starting positions
  for (int i = 0; i < k; ++i){
    int x, y; std::cin >> x >> y; //starting cloumn and row coordinates
    int s = n*x+y; //translated to starting vertex
    //add ingoing edge from source to s (so first copy of grid)
    adder.add_edge(v_source, s, 1);
    
  }
  
  //connecting the two copies by capacity c edges
  for (int i = 0; i < m; ++i){
    for (int j = 0; j < n; ++j){
      adder.add_edge(i*n+j,m*n+i*n+j, c);
    }
  }

  //connecting all intersections
  for (int i = 0; i < m; ++i){
    for (int j = 0; j < n; ++j){
      int v = i*n+j; //vertex this translates to (in first copy)
      //connecting to target, if applicable
      if (i == 0 || j == 0 || i == m-1 || j == n-1){//outer hallway intersection
        //check if we are at a corner
        if ((i == 0 && j == 0) || (i == m-1 && j == 0) ||
            (i == 0 && j == n-1) || (i == m-1 && j == n-1))  {
           adder.add_edge(m*n+v,v_sink, 2); //outgoing from hallway
        }
        else adder.add_edge(m*n+v,v_sink, 1); //outgoing from hallway
      }
      
    //connecting intersections among themselves
    //always from "out-going"-copy to "in-going"-copy
    if (i > 0) adder.add_edge(m*n+v,(i-1)*n+j, 1);
    if (i < m-1) adder.add_edge(m*n+v,(i+1)*n+j, 1);
    if (j > 0) adder.add_edge(m*n+v,i*n+(j-1), 1);
    if (j < n-1) adder.add_edge(m*n+v,i*n+(j+1), 1);
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << flow;

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