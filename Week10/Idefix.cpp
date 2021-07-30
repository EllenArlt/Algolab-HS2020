#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <iomanip>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
  return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

void testcase() {
  Index n;
  std::cin >> n;
  
  int m, k;
  long s;
  std::cin >> m >> s >> k;

  // read points: first, we read all points and store them into a vector,
  // together with their indices
  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  // then we build the Delaunay triangulation in one shot, so as to leave the
  // choice of an efficient insertion order to the triangulation structure. By
  // giving the points paired with the indices, these indices are used to
  // initialize the vertex info accordingly.
  // This step takes O(n log n) time (for constructing the triangulation).
  Delaunay t;
  t.insert(points.begin(), points.end());

  // extract edges and sort by (squared) length
  // This step takes O(n log n) time (for the sorting).
  EdgeV edges;
  edges.reserve(3*n+m); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  
  
  for(int i = 0; i < m; ++i){
    int x, y;
    std::cin >> x >> y;
    K::Point_2 loc(x,y);
    auto v = t.nearest_vertex(loc);
    Index in = v->info();
    edges.emplace_back(in, in, 4*CGAL::squared_distance(loc,v->point()));
  }
  
  
  
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });
            
            
  

  // Compute EMST using Kruskal's algorithm. This step takes O(n alpha(n)) time
  // in theory; for all practical purposes alpha(n) is constant, so linear time.

  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  int a = -1;
  long b = -1;
  std::vector<int> bones(n,0);
  int maxbones = 0;
  // ... and process edges in order of increasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    K::FT dist = std::get<2>(*e);
    if (dist > s && a == -1){
     a = maxbones; 
    }
    // determine components of endpoints
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      Index common = uf.find_set(std::get<0>(*e));
      bones[common] = bones[c1] + bones[c2];
    }
    else if (std::get<0>(*e) == std::get<1>(*e)) {
      ++bones[c1];
    }
    Index common = uf.find_set(std::get<0>(*e));
    maxbones = std::max(maxbones,bones[common]);
    if (maxbones >= k && b == -1){
      b = dist;
    }
    if (a != -1 && b != -1) break;
  }
  
  if ( a == -1) a = maxbones;
  //std::setprecisio
  std::cout << a << " " << b;
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