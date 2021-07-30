#include <stack>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

enum Color { Black = 0, White = 1, Red = 2 };
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<std::pair<Color,int>,K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Vertex_handle Vertex;

void testcase()
{
  std::size_t n; //number of stations
  std::size_t m; //number of clues
  long r; //operation range
  std::cin >> n >> m >> r;
  
  long range = r*r; //square range for comparison with squared lengths 
  
  // read stations and make a triangulation of them
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  bool nonetwork = false; //true if there is no network
  
  if (n != 1){//if n==1 there is only one station, hence only one 2-colorable component
  
    //find out if the graph induced by the edges in the triangulation of length < (2r)^2 is 2-colorable
    //and keep track of connected components in this induced graph
    
    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end();++v){
      v->info().first = Black; //mark all vertices as uncolored
    }
    
    int numcomp = 0; //Component ID
    
    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end();++v){
      if (nonetwork) break;
      if (v->info().first == Black){ //if vertex is uncolored (we need to start a new connected component)
        std::stack<Vertex> s; //stack for DFS
        v->info().first = White; //color first verex in component
        v->info().second = numcomp; //save comonent ID
        s.push(v);
        while (!s.empty()){
          //for each vertex on the stack go through all neighbours in the range and color them greedily
          // in the opposite color (can do this as for the graph to be 2-colorable they need to have opposite colours)
          Vertex u = s.top();
          s.pop();
           Triangulation::Vertex_circulator c = t.incident_vertices(u);
           do {
           if (!t.is_infinite(c)) {
             if (CGAL::squared_distance(u->point(),c->point()) <= range){
               if (c->info().first == Black){
                 c->info().second = numcomp;
                 s.push(c);
                 if (u->info().first == White) c->info().first = Red;
                 else c->info().first = White;
               }
               //a lazy insufficient check for contradictions in the 2-coloring
               //if two neighboring verices in range have the same colour, the induced graph is not 2-colorable
               // and we can give up on the 2-coloring early
               else if (c->info().first == u->info().first){ 
                 nonetwork = true;
                 break;
               }
             }
           }
  
           } while (++c != t.incident_vertices(u));
          
        }
        
        ++numcomp;
      }
      
    }
    
    if(!nonetwork){ //thorough check if there were no contradictions in the 2-colouring
    
    /*Create a triangulation of each 1-colour subgrapph and check if any of the edges has length <= range*/
    
      // construct triangulation of reds stations
      std::vector<K::Point_2> red_pts;
      red_pts.reserve(n);
      for (Triangulation::Finite_vertices_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end();++v){
        if (v->info().first == Red) red_pts.push_back(v->point());
      }
      
      Triangulation t2;
      t2.insert(red_pts.begin(), red_pts.end());
      
      
      for (auto e = t2.finite_edges_begin(); e != t2.finite_edges_end(); ++e){
        if ((t2.segment(e)).squared_length() <= range) nonetwork = true;
      }
      
      // construct triangulation of white stations
      std::vector<K::Point_2> white_pts;
      white_pts.reserve(n);
      for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end();++v){
        if (v->info().first == White) white_pts.push_back(v->point());
      }
      
      Triangulation t3;
      t3.insert(white_pts.begin(), white_pts.end());
      
      
      for (auto e = t3.finite_edges_begin(); e != t3.finite_edges_end(); ++e){
        if ((t3.segment(e)).squared_length() <= range) nonetwork = true;
      }
    
    }
  }
  
  //read clues
  if (nonetwork){
    for (std::size_t i = 0; i < m; ++i) {
      int ax, ay, bx, by;
      std::cin >> ax >> ay >> bx >> by;
      std::cout << "n";
    }
    return;
  }
  
  for (std::size_t i = 0; i < m; ++i) {
    int ax, ay, bx, by;
    std::cin >> ax >> ay >> bx >> by;
    K::Point_2 pta(ax, ay);
    K::Point_2 ptb(bx, by);
    if (CGAL::squared_distance(pta,ptb) <= range){
      std::cout << "y";
      continue;
    }
    
    Vertex va = t.nearest_vertex(pta);
    Vertex vb = t.nearest_vertex(ptb);
    
    if (va->info().second == vb->info().second && CGAL::squared_distance(pta,va->point()) <= range
        && CGAL::squared_distance(ptb,vb->point()) <= range){
      std::cout << "y";
    }
    else std::cout << "n";
    
    
  }
  
  
}



int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of testcases
  for(int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
     std::cout << "\n";
  }
  return 0;

}