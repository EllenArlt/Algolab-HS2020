#include <iostream>
#include <vector>
#include <stack>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;


//find, given minimum distance s, the number of families assignable
int num_families_assignable(int k, Triangulation &t, long s){
  /*To find out how many families can be assigned tents, we need to find the number 
  of connected components in in a graph where two vertices share an edge only if they are 
  less than the minimum distance away from neach other. Therefore we perform a DFS starting
  from every vertex,while keeping track of the number and sizes of connected components
  (if two vertices are less than the minimum distance apart, they cannot be assigned
  to different families*/
 
  //initially all unvisited
  for (auto it = t.finite_vertices_begin(); it != t.finite_vertices_end(); ++it){
    it->info() = -1;
  }
  
  int comp_num = 0; //keep track of number of components
  
  for (auto it = t.finite_vertices_begin(); it != t.finite_vertices_end(); ++it){
    if (it->info() == -1){
      std::stack<Triangulation::Vertex_handle> stack;
      stack.push(it);
      it->info() = comp_num; //visited in component com_num
      while (!stack.empty()){
        Triangulation::Vertex_handle v = stack.top(); stack.pop();
         Triangulation::Vertex_circulator c = t.incident_vertices(v);
         do {
           if (!t.is_infinite(c)){ //infinite vertex does not count
             if (c->info() == -1){
               if (CGAL::squared_distance(v->point(), c->point()) < s){ //check if connected
                 c->info() = comp_num; //visited in component com_num
                 stack.push(c);
               }
             }
           }
         } while (++c != t.incident_vertices(v));
      }
      ++comp_num; //stack is empty so all other vertices of minimum distance are not in this component
    }
  }
  
  std::vector<int> comp_sizes(comp_num,0);
  
  //compute sizes of components
  for (auto it = t.finite_vertices_begin(); it != t.finite_vertices_end(); ++it){
    ++comp_sizes[it->info()];
  }
  int comp_at_least_four = 0;
  int comp3 = 0;
  int comp2 = 0;
  int comp1 = 0;
  
  //group sizes (there are never more than four tents per family needed)
  for (int i = 0; i < comp_num; ++i){
    if (comp_sizes[i] >= 4) ++comp_at_least_four;
    else if (comp_sizes[i] == 3) ++comp3;
    else if (comp_sizes[i] == 2) ++comp2;
    else if (comp_sizes[i] == 1) ++comp1;
  }
  
  //divide smaller components among families 
  if (k == 1) return comp_at_least_four + comp3 + comp2 + comp1;
  if (k == 2) return comp_at_least_four + comp3 + comp2 + comp1/2;
  if (k == 3) {
    int tmp = std::min(comp2,comp1);
    return comp_at_least_four + comp3 + tmp + (comp2-tmp)/2 + (comp1-tmp)/3;
  }
  if (k == 4){
    int sol = comp_at_least_four;
    int tmp = comp2/2; sol += tmp; comp2 -= 2*tmp;
    tmp = std::min(comp3,comp1); sol += tmp; comp1 -= tmp; comp3 -= tmp;
    tmp = std::min(comp2,comp1/2); sol += tmp; comp1 -= 2*tmp; comp2 -= tmp;
    tmp = std::min(comp3,comp2); sol += tmp; comp3 -= tmp; comp2 -= tmp;
    tmp = comp3/2; sol += tmp; comp3 -= 2*tmp;
    tmp = comp1/4; sol += tmp; comp1 -= 4*tmp;
    if (3*comp3 + 2*comp2 + comp1 >= 4) std::cout << "ERROR";
    return sol;
  }
  

}

void testcase()
{
  // read number of points
  int n;
  std::cin >> n;
  int k; std::cin >> k; //min tents per family
  int f0;
  long s0;
  std::cin >> f0 >> s0;
  // read points
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  int f_sol = num_families_assignable(k,t,s0); //get number of families assignable given the distance
  
  //binary search the longest minimum distance given f by computing the number of families assignable
  long s_sol = pow(2L,50L);
  
  std::vector<long> distances;
    
  //must be among edge lengths in triangulation (in-between values will not 
  //change the behaviour of num_families_assignable compared to the next longer edge)
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
      distances.push_back(t.segment(*e).squared_length());
  }
  
  std::sort(distances.begin(), distances.end());


  int len = distances.size();
  
  long high = len;
  long low = 0;
  while (high > low){
    long mid = (high + low)/2;
    if (num_families_assignable(k,t,distances[mid]) >= f0) low = mid+1;
    else high = mid;
  }
  
  s_sol = distances[high-1];
  
  std::cout << s_sol << " " << f_sol;
  
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