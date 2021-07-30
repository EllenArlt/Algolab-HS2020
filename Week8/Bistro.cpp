#include <iostream>
#include <iomanip>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase(std::size_t n)
{
  
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
  
  // read number of possible restaurants
  std::size_t m;
  std::cin >> m;
 
  std::setprecision(0);//correct output format
  for (std::size_t i = 0; i < m; ++i) {
    int x, y;
    std::cin >> x >> y;
    K::Point_2 loc(x,y);
    //compute squared distance to nearest existing restaurant
    long res = CGAL::squared_distance(t.nearest_vertex(loc)->point(),loc);
    std::cout << std::fixed << res << "\n";
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  std::size_t n; std::cin >> n; // Read the number of existing restaurants
  while (n != 0){
     testcase(n); // Solve a particular test case
     std::cin >> n;
  }
  return 0;

}