#include <iostream>
#include <iomanip>
#include <vector>
#include <CGAL/Segment_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

enum Color { Alive = 0, Dead = 1};
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<Color,K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle  Vertex;

void testcase(std::size_t n)
{

  int l, b, r, t;
  std::cin >> l >> b >> r >> t;
  
  // read points
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  // construct triangulation
  Triangulation tri;
  tri.insert(pts.begin(), pts.end());
  
  // The following vector will contain for each edge in the triangulation the 
  // vertices that edge connects as well as their squred length
  // it will also contain for each vertex the squared distance to all four sides
  // of the dish (these are mareked by both vertices being the same)
  std::vector<std::tuple<Vertex,Vertex,K::FT>> edges; 
  
  // put all triangulation edges in edges
  for (Edge_iterator e = tri.finite_edges_begin(); e != tri.finite_edges_end(); ++e){
    Vertex v1 = e->first->vertex((e->second + 1) % 3);
    Vertex v2 = e->first->vertex((e->second + 2) % 3);
    K::FT len2 = tri.segment(e).squared_length();
    edges.push_back({v1,v2,len2});
  }
  //mark all bacteria as alive and put all squared distances from the boundary
  // in edges (need to multiply by 2 before squaring for the distances to be comparable
  // to the pairwise vertex distances(as they kill each other in the middle))
  for (auto it = tri.finite_vertices_begin(); it != tri.finite_vertices_end(); ++it){
    it->info() = Alive; //mark all bacteria as alive
    edges.push_back({it,it,CGAL::square(2*(it->point().x() - l))});
    edges.push_back({it,it,CGAL::square(2*(r - it->point().x()))});
    edges.push_back({it,it,CGAL::square(2*(it->point().y() - b))});
    edges.push_back({it,it,CGAL::square(2*(t - it->point().y()))});
  }
  
  //sort edges by distance (the smaller the distance the earlier two bacteria kill each other or one commits suicide (boundary))
  std::sort(edges.begin(), edges.end(),
  [](const std::tuple<Vertex,Vertex,K::FT>& p1, const std::tuple<Vertex,Vertex,K::FT>& p2) -> bool {
    return std::get<2>(p1) < std::get<2>(p2);
  });
  
  std::size_t alive = n; //number of bacteria still alive
  int i = 0;//index in edges
  
  //will contain respective distances the bacterias grew just when they died
  K::FT firstdist = -1;
  K::FT middist = -1;
  K::FT lastdist = -1;
  
  //compute the < 50% mark (inclusive)
  std::size_t less_half = n/2;
  if (less_half*2 == n) less_half -= 1;
  
  while (alive > 0 && i < edges.size()){
    K::FT dist;
    Vertex v1;
    Vertex v2;
    std::tie(v1,v2,dist) = edges[i];
    //For each edge, mark the ones killed just as they grew 
    //enough to touch an obstacle along this edge as dead
    if (v1->info() == Alive){
      v1->info() = Dead;
      --alive;
    }
    if (v2->info() == Alive){
      v2->info() = Dead;
      --alive;
    }
    
    //set the sought for distnces accordingly
    if (firstdist == -1){ //will only be true at the first death
      firstdist = dist;
    }
    else if (middist == -1 && alive <= less_half){
      middist = dist;
    }
    else if (alive == 0) { //while loop ends just after this becomes true
      lastdist = dist; 
    }
    ++i;
  }
  
  //invert formula to get the desired output x = (2*(t^2 + 1/2))^2  (because of squared distances and as they meet in the middle)
  std::cout << std::ceil(sqrt(0.5*(sqrt(firstdist) - 1))) << " ";
  std::cout << std::ceil(sqrt(0.5*(sqrt(middist) - 1))) << " ";
  std::cout << std::ceil(sqrt(0.5*(sqrt(lastdist) - 1))) << "\n";
    

}




int main(){
  std::ios_base::sync_with_stdio(false);
  
  std::size_t n; std::cin >> n; // Read the number of bacteria
  while (n != 0){
     testcase(n); // Solve a particular test case
     std::cin >> n;
  }
  return 0;

}