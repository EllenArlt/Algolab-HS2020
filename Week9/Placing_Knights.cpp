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
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
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



void testcase() {
  
    int n; std::cin >> n; //side length of the board
    
    int n2 = n*n; //n squared = number of squares
    const int v_source = n2;
    const int v_target = n2 + 1;
    
    std::vector<std::vector<int>> present(n,std::vector<int>(n)); //present[j][j] == true if [i,j] is present

    // Create graph, edge adder class and propery maps
    graph G(n2 + 2);
    edge_adder adder(G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    
    for (int i = 0; i < n; ++i){
      for (int j = 0; j < n; ++j){
        int not_hole; std::cin >> not_hole;

        present[i][j] = not_hole;
          
      }
    }
    
    
    bool ieven = false;
    for (int i = 0; i < n; ++i){
      ieven = !ieven;
      bool jeven = false;
      for (int j = 0; j < n; ++j){
        jeven = !jeven;
        
        int pos = n*i + j; //position mapped to vertex in graph
        
   
        if (present[i][j]==1){ 
          //connect to source or target depending on the color of the square
          if (ieven == jeven ) adder.add_edge(v_source, pos, 1);
          else adder.add_edge(pos, v_target, 1);
        

          //connect two squares by an edge if knights on these positions threaten each other
          if (ieven == jeven ){
            if (i >=1){
              if (j >= 2){ if(present[i-1][j-2]==1) adder.add_edge(pos, (i-1)*n+(j-2), 1);}
              if (j <= n-3){ if(present[i-1][j+2]==1) adder.add_edge(pos, (i-1)*n+(j+2), 1);}
            }
            if (i >= 2){
              if (j >= 1){ if(present[i-2][j-1]==1) adder.add_edge(pos, (i-2)*n+(j-1), 1);}
              if (j <= n-2){ if(present[i-2][j+1]==1) adder.add_edge(pos, (i-2)*n+(j+1), 1);}
            }
            if (i <= n-2){
              if (j >= 2){ if(present[i+1][j-2]==1) adder.add_edge(pos, (i+1)*n+(j-2), 1);}
              if (j <= n-3){ if( present[i+1][j+2]==1) adder.add_edge(pos, (i+1)*n+(j+2), 1);}
            }
            if (i <= n-3){
              if (j >= 1 ){ if(present[i+2][j-1]==1) adder.add_edge(pos, (i+2)*n+(j-1), 1);}
              if (j <= n-2){ if(present[i+2][j+1]==1) adder.add_edge(pos, (i+2)*n+(j+1), 1);}
            }
          }

        }
        
      }
    }
    
   // Find a min cut via maxflow
  int flow = boost::push_relabel_max_flow(G, v_source, v_target);

  // BFS to find vertex set S
  std::vector<bool> vis(n2+2, false); // visited flags
  std::queue<int> Q; // BFS queue (from std:: not boost::)
  vis[v_source] = true; // Mark the source as visited 
  Q.push(v_source);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    //std::cout << u;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      //std::cout << " " << v << " ";
      // Only follow edges with spare capacity
      if (rc_map[*ebeg]== 0 || vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }
    //std::cout << "!";
  }
  
  int res = 0;
  ieven = false;
  for (int i = 0; i < n; ++i){
    ieven = !ieven;
    bool jeven = false;
    for (int j = 0; j < n; ++j){
      jeven = !jeven;
      
      int pos = n*i + j; //position mapped to vertex in graph
      
      //Find out if the corresponding vertex belongs to the independent set (and the square is present)
      //graph was designed so all holes are connected
      //only to source or vertex, hence are all part of the independent set
      if (ieven == jeven && vis[pos] && present[i][j]==1){ ++res;}
      if (ieven != jeven && !vis[pos] && present[i][j]==1){ ++res;}
      
    }
  }
  
  std::cout << res; 
  
  /*out_edge_it ebeg, eend;
    //std::cout << u;
    for (boost::tie(ebeg, eend) = boost::out_edges(2, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      std::cout <<  " " << v;
    }
      */                         
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