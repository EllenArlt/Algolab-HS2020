///1
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <iostream>
#include <vector>
#include <queue>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Face_handle face;


/*Main idea: We triangulate the trees and then regarding the triangulation as a graph
  where the faces are the vertices and two vertices share an edge if the respective 
  faces are adjacent. The edges will have weight the squared distance between the 
  common vertices in the triangulation. We want to find for each vertex/ face the smallest
  ballon radius such that a ballon could escape from there. So we initialize all faces/vertices
  with the squared double distance of the "innermost point" to one of the vertices for that face
  (as depending on that we can take off inside that face or not) and then from the
  infinite faces we find in a Dijkstra-like fashion the maximum balloon radius for
  which the balloon can be moved to a takeoff location*/


void testcase(){
  int n, m; //#trees, #ballons
  long r; //tree radius
  std::cin >> n >> m >> r;
  
  //triangulate trees
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    long x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  std::priority_queue<std::pair<K::FT,face>> q{}; //priority queue
  
  //initialize with maximum squared radius for takeoff in face
  for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
    K::Point_2 p = t.dual(f);
    f->info() = CGAL::squared_distance(p, (f->vertex(0))->point());
    q.push({f->info(),f}); //push all faces on priority queue (sorted by radius)
  }
  
   //update outer faces maximum radius (by squared distance between common verices with infinite face)
   Triangulation::Face_circulator c = t.incident_faces(t.infinite_vertex());
   do {
     face f = c;
     for (int i = 0; i < 3; ++i){
       face fn = f->neighbor(i);
       if (!t.is_infinite(fn)) {
         K::FT dist = CGAL::squared_distance(f->vertex((i+1)%3)->point(),f->vertex((i+2)%3)->point());
         fn->info() = std::max(fn->info(),dist);
         q.push({(fn->info()),fn});
       }
     }
   } while (++c != t.incident_faces(t.infinite_vertex()));
   
   //Use Dijkstra
   while (!q.empty()){
     auto cur = q.top(); q.pop();
     K::FT d = cur.first; 
     face f = cur.second;
     if ( d < f->info()) continue; 
     for (int i = 0; i < 3; ++i){
       face fn = f->neighbor(i);
       if (!t.is_infinite(fn)) {
         K::FT dist = CGAL::squared_distance(f->vertex((i+1)%3)->point(),f->vertex((i+2)%3)->point());
         if (std::min(dist,f->info()) > fn->info()){
           fn->info() = std::min(dist,f->info());
           q.push({(fn->info()),fn});
         }
 
       }
     }
   }
   
   for (int i = 0; i < m; ++i){
     long x, y, s;
     std::cin >> x >> y >> s;
     K::FT rs = r + s;
     K::FT rs2 = rs*rs;
     
     K::Point_2 loc(x,y);
     
     if (CGAL::squared_distance(loc,t.nearest_vertex(loc)->point()) < rs2){//not enough distance from tree to inflate balloon
       std::cout << "n";
       continue;
     }
     face f = t.locate(loc);
     if (t.is_infinite(f) || f->info() >= 4*rs2){ //inside infinite face or movable to takeoff location
       std::cout << "y";
       continue;
     }
     
     std::cout << "n";
   }
   

}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of infected
  while (t>0){
     testcase(); // Solve a particular test case
     std::cout << "\n";
     --t;
  }
  return 0;

}