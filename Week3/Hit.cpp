#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;



//straightforward checking for each segment




void testcase(int n) {
  
  long x; std::cin >> x; 
  long y; std::cin >> y; 
  long a; std::cin >> a; 
  long b; std::cin >> b; 
  
  P p1(x,y), p2(a,b);
  R ray(p1,p2); //construct ray
  
  std::vector<S> Segments; //vector containing segments

  
  for (int i = 0; i < n; ++i){
    long r; std::cin >> r; 
    long s; std::cin >> s; 
    long t; std::cin >> t; 
    long u; std::cin >> u; 
    
    //construct segments and put in vector
    P ps1(r,s), ps2(t,u);
    S segment(ps1,ps2);
    Segments.push_back(segment);

  } 
  
  //check for each segment if they intersect with the ray
  for (std::vector<S>::iterator it = Segments.begin(); it != Segments.end(); ++it){
    if (CGAL::do_intersect(ray,*it)) {
      std::cout << "yes";
      return;
    }
  }
  
  std::cout << "no";
  
  

     
}
  


 



int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n; std::cin >> n;
  while (n != 0){
     testcase(n); // Solve a particular test case
     std::cout << std::endl;
     std::cin >> n;
  }
  return 0;

}