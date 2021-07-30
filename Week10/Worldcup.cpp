#include <vector>
#include <iostream>
#include <vector>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Line_2.h>
#include <iomanip>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;



// choose input type (input coefficients must fit)
typedef CGAL::Gmpz IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase()
{ 
  
  int n;
  int m;
  int c;
  std::cin >> n >> m >> c;
  
  std::vector<K::Point_2> npos(n);
  std::vector<K::Point_2> mpos(m);
  
  
  std::vector<int> alc(n); //vector of alcohol percentages for each beverage
  std::vector<int> supply(n); //vector of warehouse supplies 
  
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  
  for (int i = 0; i < n; ++i){
    int x, y, s, a;
    std::cin >> x >> y >> s >> a;
    alc[i] = a;
    supply[i] = s;
    K::Point_2 pt(x,y);
    pts.push_back(pt);
    npos[i] = pt;
  }
  
  // construct triangulation of warehouses
  Triangulation tw;
  tw.insert(pts.begin(), pts.end());
  
  std::vector<int> demand(m); //vector of stadium demands
  std::vector<int> upper(m); //vector of upper limit on pure alcohol
  
  std::vector<K::Point_2> pts2;
  pts2.reserve(m);
  
  for (int i = 0; i < m; ++i){
    int x, y, d, u;
    std::cin >> x >> y >> d >> u;
    demand[i] = d;
    upper[i] = u;
    K::Point_2 pt(x,y);
    pts2.push_back(pt);
    
    mpos[i] = pt;
  }
  
  // construct triangulation of stadiums
  Triangulation ts;
  ts.insert(pts2.begin(), pts2.end());
  
  std::vector<std::vector<int>> revenue(n,std::vector<int>(m)); //matrix of revenues for stadium-warehouse combinations  
  
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < m; ++j){
      int r;
      std::cin >> r;
      revenue[i][j] = r;
    }
  }
  
  std::vector<std::vector<int>> trav(n,std::vector<int>(m,0)); //number of circles traversed for each combination
  
  int num_in = 0;
  for (int l = 0; l < c; ++l){ //go through each circle
    int x, y;
    long r;
    std::cin >> x >> y >> r;
    long r2 = r*r; 
    K::Point_2 pt(x,y);
    if (num_in == 100) continue; //know there are at most 100 buildings in a circle 
    K::FT dist1 = CGAL::squared_distance(pt,tw.nearest_vertex(pt)->point()); //find closest warehouse to circle center
    K::FT dist2 = CGAL::squared_distance(pt,ts.nearest_vertex(pt)->point()); //find closest stadium to circle center
    if (dist1 < r2 || dist2 < r2){ //if either is still in the circle
      ++num_in; //must be one of the at most 100 circles
      if (dist1 < r2){ //if nearest warehouse is in circle need to update trav for all stadiums out of circle (and warehoses in circle)
        for (int i = 0; i < n; ++i){
          K::FT dist_in = CGAL::squared_distance(pt,npos[i]);
          if (dist_in < r2){
            for (int j = 0; j < m; ++j){
              K::FT dist_out = CGAL::squared_distance(pt,mpos[j]);
              if (dist_out > r2){
                trav[i][j] += 1;
              }
            }
          }
        }
      }
      if (dist2 < r2){ //if nearest stadium is in circle need to update trav for all warehouses out of circle (and stadiums in circle)
        for (int j = 0; j < m; ++j){
          K::FT dist_in = CGAL::squared_distance(pt,mpos[j]);
          if (dist_in < r2){
            for (int i = 0; i < n; ++i){
              K::FT dist_out = CGAL::squared_distance(pt,npos[i]);
              if (dist_out > r2){
                trav[i][j] += 1;
              }
            }
          }
        }
      }
    }
  }
  
  //now that whe have all the info it is an LP problem (maximize profit subject to given constraints)
  
    // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  
  for(int j = 0; j < n; ++j){
    
    for(int k = 0; k < m; ++k){
      lp.set_c(k*n+j, (-100*revenue[j][k] + trav[j][k]));
      lp.set_a(k*n+j, j, 1);
    }
    
    lp.set_b(j, supply[j]);
    
  }
  
  for(int i = 0; i < m; ++i){
    for(int j = 0; j < n; ++j){
      lp.set_a(i*n+j, n+i, 1);
      lp.set_a(i*n+j, n+m+i, -1);
      lp.set_a(i*n+j, n+m+m+i, alc[j]);
    }
    lp.set_b(n+i, demand[i]);
    lp.set_b(n+m+i, -demand[i]);
    lp.set_b(n+m+m+i, 100*upper[i]);
  }
  
  Solution s;
  
  s = CGAL::solve_linear_program(lp, ET());
  //assert(s.solves_linear_program(lp));
  CGAL::Gmpq hundred(-100,1);
  // output solution
  std::cout << std::setprecision(0);
  if (s.is_optimal()) {  
    std::cout << std::fixed << floor_to_double(s.objective_value()/hundred) << "\n";
  }
  else std::cout << "RIOT!" << "\n";
  

  
}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
  }
  return 0;

}