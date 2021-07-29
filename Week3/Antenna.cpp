#include <iostream>
#include <iomanip>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt KE;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 PE;
typedef KE::Point_2 PE2;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef  CGAL::Min_circle_2_traits_2<KE>  Traits2;
typedef  CGAL::Min_circle_2<Traits2>      Min_circle2;



double ceil_to_double(const KE::FT& x)
{
 double a = std::ceil(CGAL::to_double(x));
 while (a < x) a += 1;
 while (a-1 >= x) a -= 1;
 return a;
}


void testcase(const int &n) {
  
  std::vector<PE> homes(n); //points, where the homes are
  
  for (int i = 0; i < n; ++i){
    long x; std::cin >> x; 
    long y; std::cin >> y; 
    
    PE home(x,y);
    homes[i] = home;

  } 
  
  //Construct minimum circle containing all homes
  Min_circle mc(homes.begin(), homes.end(), true);
  
  //find support points of that minimum circle to later construct
  //the circle in the squareroot kernel more cheaply
  std::vector<PE2> support;
  
  int num = mc.number_of_support_points();
  
  for (int i = 0; i < num; ++i){
    PE2 p(mc.support_point(i).x(),mc.support_point(i).y());
    support.push_back(p);
  }
  
  std::cout << std::setprecision(0);
  
  //construct same circle in the squareroot kernel from support points
  Min_circle2 mc2(support.begin(), support.end(),true);
  Traits2::Circle c2 = mc2.circle();
  //output squreroot of squared radius of minimum circle
  std::cout << std::fixed << ceil_to_double(CGAL::sqrt(c2.squared_radius()));
   
}
  


 



int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n; std::cin >> n;
  while (n != 0){
     testcase(n); // Solve a particular test case
     std::cout << "\n";
     std::cin >> n;
  }
  return 0;

}