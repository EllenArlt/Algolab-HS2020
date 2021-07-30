#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <queue>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;


void testcase(std::size_t n){

  // read points
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
  
  std::priority_queue<std::pair<K::FT,Triangulation::Face_handle>> q;
  
  //mark all finite faces as not visited
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f){ 
    
    if (!t.is_infinite(f)){
      f->info() = -1;
      q.push({f->info(),f});
    }
  } 
  
  //mark faces next to infinte faces with squared length of common edge (twice min dist to nearest infected squared)
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f){
    if (t.is_infinite(f)){
      for (int i = 0; i < 3; ++i){
        auto fn = f->neighbor(i);
        if (!t.is_infinite(fn)){
          K::FT dist = CGAL::squared_distance(f->vertex((i+1)%3)->point(),f->vertex((i+2)%3)->point());
          fn->info() = std::max(dist,fn->info());
          q.push({fn->info(),fn});
        }
      }
    }
    
  }
  
  //Use method simila to dijkstra to find path through longest possible min-size edges
  //(basically regard edges as edges connecting two faces/vertices and we want to find the 
  //path where the minimum weight along the path is as large as possible)
  while (!q.empty()){
    auto cur = q.top(); q.pop();
    auto f = cur.second;
    K::FT best = cur.first;
    if (best < f->info()) continue;
    for (int i = 0; i < 3; ++i){
        auto fn = f->neighbor(i);
        if (!t.is_infinite(fn)){
          K::FT dist = CGAL::squared_distance(f->vertex((i+1)%3)->point(),f->vertex((i+2)%3)->point());
          K::FT min = std::min(dist, f->info());
          if (fn->info() < min){
            fn->info() = min;
            q.push({fn->info(),fn});
          }
          
        }
    }
  }

  int m; std::cin >> m;
  
  for (int i = 0; i < m; ++i){
    int x, y;
    long d;
    std::cin >> x >> y >> d;
    
    
    K::Point_2 loc(x,y);
    //not enough  distance to nearest infected
    if (CGAL::squared_distance(t.nearest_vertex(loc)->point(),loc) < d){
      std::cout << "n";
      continue;
    }
    auto f = t.locate(loc);
    //already safe or the the path of largest posibble minim distance to any infected is ok
    if (t.is_infinite(f) || f->info() >= 4*d){
      std::cout << "y";
      continue;
    }
    
    std::cout << "n";
  }
}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  std::size_t n; std::cin >> n; // Read the number of infected
  while (n != 0){
     testcase(n); // Solve a particular test case
     std::cout << "\n";
     std::cin >> n;
  }
  return 0;

}