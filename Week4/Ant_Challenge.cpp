#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


/*The algorithm works as follows:
-First create the MST (the private network) for each species using Kruskal (O(s*n^2*log(n^2)))
-For each edge in the forest, pick the smallest edge weight of all MST (if existent)
 and put an edge of that weight in a new weighted graph G (O(s*n^2))
-Do dijkstra on G (O(n*log(n) + n^2)) 
 */



typedef boost::adjacency_list<boost::vecS, 
                              boost::vecS, 
                              boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>
                              > weighted_graph;
                              

typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


//The first two steps of the algorithm
//G will become the common network while graphs is an input vector of graphs where
//graphs[i] contains the complete forest graph where the edge weights correspond to species i
//hives[i] contains the location of the hive of species i
void make_conv_graph(weighted_graph &G, int s, int n, const std::vector<weighted_graph> &graphs,
                    const std::vector<int> &hives, const std::vector<weight_map> &weights) {
  
  //mst[i] will strore the MST edges of the MST for species i
  std::vector<std::vector<edge_desc>> mst(s);    // vector to store MST edges (not a property map!)
  //compute MSTs (private networks)
  for (int i = 0; i < s; ++i){
    boost::kruskal_minimum_spanning_tree(graphs[i], std::back_inserter(mst[i]));
  }
  
  weight_map wG = boost::get(boost::edge_weight, G);
  
  //for each private network, go through all its edges
  for (int j = 0; j < s; ++j){
    for (std::vector<edge_desc>::iterator it = mst[j].begin(); it != mst[j].end(); ++it) {
        //find endpoints in graphs[j] to look for the edge in G
        vertex_desc x = boost::source(*it, graphs[j]);
        vertex_desc y = boost::target(*it, graphs[j]); 
        //check if that edge already exists in the common network G
        //if not, add it and give it the weight corresponding to species j
        if (!boost::edge(x,y,G).second){
          edge_desc ed2;
          ed2 = boost::add_edge(x, y, G).first;
          wG[ed2] = weights[j][*it];
          
        }
        //if it does already exist and that species ids faster along that edge,
        //adjust the weight (we only want the fasted species for each edge 
        //to get the best possible speed)
        else {
          edge_desc edge = boost::edge(x,y,G).first;
          if (wG[edge] > weights[j][*it]){
            wG[edge] = weights[j][*it];
          }
        }
        
      }
  
  }
    
}



//use dijkstra to get the shortest/fastest path from s to t in the common network
void dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  std::cout << dist_map[t];
}

void testcase() {
  

  
  int n; std::cin >> n; //number of trees
  int e; std::cin >> e; //number of edges
  int s; std::cin >> s; //number of species
  int a; std::cin >> a; //start tree
  int b; std::cin >> b; //finish tree
  
  std::vector<weighted_graph> graphs(s,weighted_graph(n));//graphs with edge weights corresponding to species
  std::vector<weight_map> weights(s);//corresponding weight-maps
  
  for (int i = 0; i < s; ++i){
       weights[i] = boost::get(boost::edge_weight, graphs[i]);
  }
  //read edges
  for (int i = 0; i < e; ++i){
       int x; std::cin >> x; 
       int y; std::cin >> y; 
       edge_desc ed;
        
       for (int j = 0; j < s; ++j){
            ed = boost::add_edge(x, y, graphs[j]).first;
            int w; std::cin >> w; 
            weights[j][ed] = w;
    
       }

  }
  
  std::vector<int> hives(s);//hive locations
  for (int i = 0; i < s; ++i){
       int loc; std::cin >> loc; 
       hives[i] = loc;

  }
  
  weighted_graph G(n);
  
  //create common network
  make_conv_graph(G,s,n,graphs,hives,weights);
  
  //use dijkstra to get the shortest/fastest path from a to b in the common network
  dijkstra_dist(G,a,b);

}



int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
     std::cout << std::endl;
  }
  return 0;

}