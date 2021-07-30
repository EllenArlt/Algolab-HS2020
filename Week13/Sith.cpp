#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <stack>
#include <vector>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<bool,K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

/*Main idea: Do a binary search over the number k of planets attainable for the rebellion.
  For each number check if there is a "connected component" of size k among the planets 
  not taken by the empire after k days*/
  
//Check if it is feasible to take k planets with squared range rs and planet coordinates pts
bool feasible(std::vector<K::Point_2> &pts, int k, long rs){
  
  //triangulate leftover planetes after k days
  Triangulation t;
  t.insert(pts.begin()+k, pts.end());
  
  //find the maximal component size, that is vertices that form a connected component
  //when only edges of squared length at most rs in the triangulation are considered
  //Do so via a DFS
  for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end();++v){
    v->info() = false;
  }
  int max_comp_size = 0;
  for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end();++v){
    int comp_size = 0;
    if (!v->info()){
      std::stack<Triangulation::Vertex_handle> s;
      v->info() = true;
      s.push(v);
      ++comp_size;
      while (!s.empty()){
        Triangulation::Vertex_handle u = s.top();
        s.pop();
         auto c = t.incident_vertices(u);
         do {
           if (!t.is_infinite(c)) { 
             if (!c->info() && CGAL::squared_distance(u->point(),c->point()) <= rs){
               s.push(c);
               ++comp_size;
               c->info() = true;
             }
            }
         } while (++c != t.incident_vertices(u));
      }
    }
    max_comp_size = std::max(max_comp_size,comp_size);
  }
  if (max_comp_size >= k) return true;
  else return false;
  
}

void testcase()
{
  std::size_t n; //#planets
  long r; //range
  std::cin >> n >> r;
  long rs = r*r; //square for efficiency
  
  // read points
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  

  if (n <= 3){  //pick starting planet. On the second day all other planets are taken by the empire
    std::cout << 1 << "\n";
    return;
  }
  
  //binary search viable number of planets
  int high = n/2+1;
  int low = 0;
  while (low < high){
    int mid = (low+high)/2;
    if (feasible(pts,mid,rs)) low = mid +1;
    else high = mid;
  }

  std::cout << high-1 << "\n";
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    testcase();
  }
}