#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <vector>
#include <iostream>
#include <iomanip>

// choose input type (input coefficients must fit)
typedef  long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;



double ceil_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 > x) a -= 1;
  return a;
}

/*My solution here is not as simple as it could be. My solution considers a case distinction on
 whether a is positive or not on line equation ax + by + c = 0.
 A simpler solution uses line equation y = ax + b*/



//solve LP for respective request: 
//nobles/common : vector of pairs (x,y) giving coordinates
// n = #nobles, m = #commoners
//a_pos : a is positive?
//Cersei_only : We only care about whether Cercei's request is fulfilled
//s : maximum allowable sum of lengths of all sewage pipes
long solveLP(std::vector<std::pair<long,long>> &nobles, std::vector<std::pair<long,long>> &common, int n, int m,
            bool a_pos, bool Cersei_only, long s )
{
  // create an LP with Ax <= b and no lower/upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  const int a = 0;
  const int b = 1;
  const int c = 2;
  const int c_fresh = 3; //c for fesh water pipe
  const int axlen = 4; // a times max length of fresh water pipes
  
//nobles and commoners on correct sides
  if (a_pos){
    for (int i = 0; i < n; ++i) {
      // set up constraint a x + b y + c >= 0
      lp.set_a(a, i, nobles[i].first); 
      lp.set_a(b, i, nobles[i].second);
      lp.set_a(c, i, 1);
    }
    for (int i = 0; i < m; ++i) {
      // set up constraint a x + b y + c <= 0
      lp.set_a(a, n+i, -common[i].first); 
      lp.set_a(b, n+i, -common[i].second);
      lp.set_a(c, n+i, -1);
    }
    lp.set_l(a, true, 1);
  }
  
  else{
    for (int i = 0; i < n; ++i) {
      // set up constraint a x + b y + c >= 0
      lp.set_a(a, i, -nobles[i].first); 
      lp.set_a(b, i, -nobles[i].second);
      lp.set_a(c, i, -1);
    }
    for (int i = 0; i < m; ++i) {
      // set up constraint a x + b y + c <= 0
      lp.set_a(a, n+i, common[i].first); 
      lp.set_a(b, n+i, common[i].second);
      lp.set_a(c, n+i, 1);
    }
    lp.set_u(a, true, -1);
  }
  
  if (Cersei_only){
    Solution sol = CGAL::solve_linear_program(lp, ET());
    if (sol.is_infeasible()) return -1;
    else return 1;
  }
  //constraint on the sum of the lengths of the sewer pipes
  if (s != -1){
    long asum = -s;
    long bsum = 0;
    long csum = 0;
    
    for (int i = 0; i < n; ++i) {
      asum -= nobles[i].first;
      bsum -= nobles[i].second;
      csum -= 1;
    }
    for (int i = 0; i < m; ++i) {
      asum += common[i].first;
      bsum += common[i].second;
      csum += 1;
    }
    
    if (a_pos){
      lp.set_a(a, n+m, asum); 
      lp.set_a(b, n+m, bsum);
      lp.set_a(c, n+m, csum);
      }
    else{
      lp.set_a(a, n+m, -asum); 
      lp.set_a(b, n+m, -bsum);
      lp.set_a(c, n+m, -csum);
    }
  }
  
  //constraining axlen
  if (a_pos){
    for (int i = 0; i < n; ++i) {
   
      lp.set_a(a, n+m+i+1, nobles[i].second); 
      lp.set_a(b, n+m+i+1, -nobles[i].first);
      lp.set_a(c_fresh, n+m+i+1, -1);
      lp.set_a(axlen, n+m+i+1, -1);
      lp.set_a(a, 2*(n+m)+i+1, -nobles[i].second); 
      lp.set_a(b, 2*(n+m)+i+1, nobles[i].first);
      lp.set_a(c_fresh, 2*(n+m)+i+1, 1);
      lp.set_a(axlen, 2*(n+m)+i+1, -1);
    }
    for (int i = 0; i < m; ++i) {
    
      lp.set_a(a, n+m+n+i+1, common[i].second); 
      lp.set_a(b, n+m+n+i+1, -common[i].first);
      lp.set_a(c_fresh, n+m+n+i+1, -1);
      lp.set_a(axlen, n+m+n+i+1, -1);
      lp.set_a(a, 2*(n+m)+n+i+1, -common[i].second); 
      lp.set_a(b, 2*(n+m)+n+i+1, common[i].first);
      lp.set_a(c_fresh, 2*(n+m)+n+i+1, 1);
      lp.set_a(axlen, 2*(n+m)+n+i+1, -1);
    }
    lp.set_l(axlen, true, 0);
    lp.set_c(axlen, 1);
  }
  
  else{
    for (int i = 0; i < n; ++i) {
    
      lp.set_a(a, n+m+i+1, nobles[i].second); 
      lp.set_a(b, n+m+i+1, -nobles[i].first);
      lp.set_a(c_fresh, n+m+i+1, -1);
      lp.set_a(axlen, n+m+i+1, 1);
      lp.set_a(a, 2*(n+m)+i+1, -nobles[i].second); 
      lp.set_a(b, 2*(n+m)+i+1, nobles[i].first);
      lp.set_a(c_fresh, 2*(n+m)+i+1, 1);
      lp.set_a(axlen, 2*(n+m)+i+1, 1);
    }
    for (int i = 0; i < m; ++i) {
     
      lp.set_a(a, n+m+n+i+1, common[i].second); 
      lp.set_a(b, n+m+n+i+1, -common[i].first);
      lp.set_a(c_fresh, n+m+n+i+1, -1);
      lp.set_a(axlen, n+m+n+i+1, 1);
      lp.set_a(a, 2*(n+m)+n+i+1, -common[i].second); 
      lp.set_a(b, 2*(n+m)+n+i+1, common[i].first);
      lp.set_a(c_fresh, 2*(n+m)+n+i+1, 1);
      lp.set_a(axlen, 2*(n+m)+n+i+1, 1);
    }
    lp.set_u(axlen, true, 0);
    lp.set_c(axlen, -1);
  }
  
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  
  
  if (sol.is_infeasible()) return -1;
  else{ 
      Solution::Variable_value_iterator opt = sol.variable_values_begin();
      CGAL::Quotient<ET> res = *opt;
      if (a_pos) return ceil_to_double(sol.objective_value()/res);
      else return ceil_to_double(-sol.objective_value()/res);
  }
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    int n, m; long s;
    std::cin >> n >> m >> s;
    std::vector<std::pair<long,long>> nobles(n);
    std::vector<std::pair<long,long>> common(m);
    for (int i = 0; i < n; ++i){
      std::cin >> nobles[i].first >> nobles[i].second;
    }
    for (int i = 0; i < m; ++i){
      std::cin >> common[i].first >> common[i].second;
    }
    
    if(solveLP(nobles,common,n,m,true,true,s) == -1 &&
       solveLP(nobles,common,n,m,false,true,s) == -1){
         std::cout << "Yuck!" << "\n";
         continue;
       }
    long res1 = solveLP(nobles,common,n,m,true,false,s);
    long res2 = solveLP(nobles,common,n,m,false,false,s);
    if (res1 == -1 && res2 == -1) {
      std::cout << "Bankrupt!" << "\n";
      continue;
    }
    std::setprecision(0);
    if (res1 > -1 && res2 > -1){
      std::cout << std::min(res1,res2)  << "\n";
      continue;
    }
    std::cout << std::max(res1,res2)  << "\n";
    continue;
  }
  
  return 0;

}